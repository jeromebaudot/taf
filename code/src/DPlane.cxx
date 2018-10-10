// @(#)maf/dtools:$Name:  $:$Id: DPlane.cxx,v.3 2005/10/02 18:03:46 sha Exp $
// Author   : ?
// Last Modified, JB, 2008/10/17
// Last Modified, JB, 2009/05/06
// Last Modified, JB, 2009/05/18
// Last Modified, RDM, 2009/05/20
// Last Modified, JB, 2009/05/25
// Last Modified, RDM, 2009/05/26
// Last Modified, JB, 2009/08/21 binary hit finding
// Last Modified, JB, 2009/08/23
// Last Modified, JB, 2009/09/01 in constructor
// Last Modified, YV, 2010/09/11 deal with noise
// Last Modified, JB, 2010/09/20 MIMOSA 5 readout and Update return flag
// Last Modified, JB, 2010/10/06 better management of SN seed cut in find_hits
// Last Modified, JB, 2010/10/25 try all pixels as seed in find_hits
// Last Modified: JB, 2010/11/25 Use new DPrecAlign mechanism for Plane position
// Last Modified: JB, 2011/04/05 ShowAlignement & Align (final fix ?)
// Last Modified: JB, 2011/04/15 Update() and analyze_basics
// Last Modified: JB, 2011/07/07 DPlane() to localize path names
// Last Modified: JB, 2011/07/21 introduce fSession member pointer
// Last Modified: JB, 2011/07/25 Align() with output
// Last Modified: SS, 2012/08/10 Update() for new readout 32
// Last Modified: JB, 2012/08/17 Upgraded management of non-sparsified data
// Last Modified: AB, 2012/10/22 FindNeighbours
// Last Modified: JB, 2012/11/21 DPlane & ComputeStripPosition
// Last Modified: JB, 2012/11/24 ComputeStripPosition
// Last Modified: JB, 2013/08/19 new readout modes in Update
// Last Modified: JB, 2013/08/29 DPlane, Update, Digitize, DigitizeMatrix
// Last Modified: JB, 2014/01/07 Noise computation: DPlane
// Last Modified: JB, 2014/04/24 PlaneToTracker, Intersection, DeformedLocalPoint
// Last Modified: AP, 2014/06/26 ComputeStripPosition_UVToColRow
// Last Modified: VR, 2014/06/29 Add method for graphic alignment : ChangePositionBy*() and ChangeTiltBy*()
// Last Modified: VR, 2014/07/12 replace aPixel->SetSize( (*fSize)) by aPixel->SetSize( (*fPitch))
// Last Modified: VR, 2014/07/12 add the DHit::Analyse_2_cgo method when fHitFinder == 2)
// Last Modified: AP, 2014/07 find_hits()
// Last Modified: VR, 2014/08/28 add mecanism to keep untracked hits from last event
// Last Modified: LC, 2014/12/05 AlignData : clean old code.
// Last Modified: LC, 2014/12/15 MonteCarlo Infos now in DPixel and DHit :: See DPixel : std::vector<Double_t> _monteCarloInfo.
// Last Modified: AP, 2015/03/09 Adding hit spation resolution information in AlignData
// Last Modified: AB, 2015/03/31 ComputeStripPosition
// Last Modified: JB, 2015/05/26 DPlane to hadle timelimit
// Last Modified: AP, 2015/06/08 Added bool parameter (UseAllHits) to AlignPrec, to decide if doing alignment with all hits or with the closest one.
// Last Modified: JB, 2016/07/20 DPlane, analyse_basics for better management of fNoiseRun option
// Last Modified: JB, 2016/08/17 Updates for multiframe mode
// Last Modified: JB, 2016/08/19 DPlane, allow fNoiserun option for mode 232
// Last Modified: JB, 2016/10/17 DPlane::DPlane
// Last Modified: JB, 2018/05/04 DPlane::Update
// Last Modified: JB, 2018/07/04 Dplane, Update, SetPixelGainFromHisto for pixel gain map usage

/////////////////////////////////////////////////////////////
// Class Description of DPlane                             //
//                                                         //
// + finds pedestal                                        //
// + calculates pulseheight                                //
// + corrects for common mode (shift)                      //
// + calculates single noise                               //
//                                                         //
/////////////////////////////////////////////////////////////


#include "Riostream.h"
#include <math.h>
#include "DPlane.h"
#include "DSetup.h"
#include "DSession.h"
#include "DAcq.h"
//#include "DAcqModule.h"
//#include "DInp.h"
#include "DTracker.h"
#include "DStrip.h"
#include "DHit.h"
//#include "DHitMonteCarlo.h"
#include "DTrack.h"
#include "DLine.h"
#include "DCut.h"
#include "DPrecAlign.h"
#include "DAlign.h"
#include "DR3.h"
#include "DGlobalTools.h"
#include <stdlib.h>
#include "TMimosa24_25Map.h" //RDM120509
#include "TSpectrum2.h"

#include <assert.h>

ClassImp(DPlane) // Description of DPlane

//______________________________________________________________________________
//

DPlane::DPlane()
{

  fDebugPlane=0;

  rand = new TRandom(182984);

  spectrum = NULL;
  //if(fHitFinder == 1 && GetAnalysisMode() == 2) spectrum = new TSpectrum2();

}

//______________________________________________________________________________
//

DPlane::DPlane(DTracker& aTracker, const Int_t aPlaneNumber, const Int_t aLadderNumber)
{
  // Main constructor of the DPLane class.
  //
  // Last Modified: JB 2009/08/21
  // Last Modified: JB 2009/09/01, skip DStrip list construction for sparsified data
  // Last Modified: JB 2011/07/07 to localize path names
  // Last Modified: JB 2012/11/21 include Mapping for strip position computation
  // Last Modified: LC 2013/Sping introducing the ladder that this plane may belong to
  // Last Modified: JB 2013/07/17 new control parameters (HitFinder, IfDigitize)
  // Last Modified: JB 2013/08/15 new parameter fAcqChannelOffset
  // Last Modified: JB 2014/04/21 get deformation parameters
  // Last Modified: VR 2014/07/12 replace aPixel->SetSize( (*fSize)) by aPixel->SetSize( (*fPitch))
  // Last Modified: JB 2015/05/26 handle time limit
  // Last Modified: JB 2015/10/31 pass deformation parameters to DPrecAlign
  // Last Modified: JB 2016/10/17 cope with Plane with no DAQ modules (simple material)
  // Last Modified: JB 2018/07/04 load pixel gain histo from file if required

  cout << endl << " -*-*- DPlane " << aPlaneNumber << " User Constructor -*-*- " << endl;

  fTracker = &aTracker;
  fAcq     = fTracker->GetAcq();        // set pointer to Data Acquisition
  fc       = fTracker->GetSetup();      // set pointer to Configuration
  fSession = fc->GetSession();          // set pointer to Session, JB 2011/07/21
  fDebugPlane = fc->GetDebug();
  Int_t   st,ch;

  //Get MC information from fAcq
  //1st check that the data being readout is MC
  MCInfoHolder = NULL;
  if(fAcq->GetIfMCBoardReader()) MCInfoHolder = fAcq->GetMCInfoHolder();

  fLadderNumber        = aLadderNumber; // LC 2013/Spring
  fPlaneNumber         = aPlaneNumber;   // 1... planes
  fPlaneName           = fc->GetPlanePar(fPlaneNumber).Name;
  fStatus              = fc->GetPlanePar(fPlaneNumber).Status;
  fAnalysisMode        = fc->GetPlanePar(fPlaneNumber).AnalysisMode;
  fReadout             = fc->GetPlanePar(fPlaneNumber).Readout;
  fMimosaType          = fc->GetPlanePar(fPlaneNumber).MimosaType; //RDM210509
  fMapping             = fc->GetPlanePar(fPlaneNumber).Mapping; // JB 2012/11/21
  fPlanePurpose        = fc->GetPlanePar(fPlaneNumber).Purpose;   //YV 08/02/2010
  fHitFinder           = fc->GetPlanePar(fPlaneNumber).HitFinder; // JB 2013/07/17

  spectrum = NULL;
  if(fHitFinder == 1 && GetAnalysisMode() == 2) spectrum = new TSpectrum2();

  rand                 = new TRandom(fc->GetAnalysisPar().MCSeed + 3*fPlaneNumber);

  fInitialCounter = 0; // used to check initialization

  fPlaneThickness      = fc->GetPlanePar(fPlaneNumber).PlaneThickness;//QL 2016/06/07
  fPlaneMaterial       = fc->GetPlanePar(fPlaneNumber).PlaneMaterial;//QL 2016/06/07
  sigma_thetaMS         = fTool.scatteringAngle(fSession->GetSetup()->GetTrackerPar().BeamType.Data(),
                                                TMath::Abs(fSession->GetSetup()->GetTrackerPar().BeamMomentum),
                                                fSession->GetSetup()->GetPlanePar(aPlaneNumber).PlaneMaterial.Data(),
                                                fSession->GetSetup()->GetPlanePar(aPlaneNumber).PlaneThickness,
                                                false);

  cout << "Plane " << fPlaneName << " for " <<  fc->GetPlanePar(fPlaneNumber).Purpose;
  if (fStatus == 0) { cout << " (fixed ref and tracking seed) "; } // new status, JB 2011/03/14
  else if (fStatus == 1) { cout << " (fixed ref) "; }
  else if (fStatus == 2) { cout << " (variable ref) "; }
  else if (fStatus == 3) { cout << " (DUT) "; }
  if (fAnalysisMode == 0) { cout << " NOT ANALYZED"; }
  else if (fAnalysisMode == 1){ cout << " is a STRIP plane"; }
  else if (fAnalysisMode >= 2){ cout << " is a PIXEL plane"; }
  if( fLadderNumber > 0 ) { cout << " associated to LADDER " << fLadderNumber; }
  cout << endl;
  cout << "  Hit finder algorith is "<< fHitFinder;
  if( fHitFinder > 0 ) { cout << " (Dynamic clustering for digital readout)"; }
  else { cout << " (Search area from seed)"; }
  cout << endl;

  // Indeed, a plane can use up to 4 inputs of a module,
  // JB, 2009/05/07
  fAcqInputsN = fc->GetPlanePar(fPlaneNumber).Inputs;
  fChannelsN = 0;
  fAcqModuleTypeNumber = 0;
  for( Int_t iInp=0; iInp<fAcqInputsN; iInp++) {
	  fAcqModuleTypeNumber = fc->GetPlanePar(fPlaneNumber).ModuleType[iInp];
	  fAcqModuleNumber     = fc->GetPlanePar(fPlaneNumber).ModuleNumber[iInp];
	  fAcqInputNumber      = fc->GetPlanePar(fPlaneNumber).InputNumber[iInp];
	  fAcqChannelNumber    = fc->GetPlanePar(fPlaneNumber).ChannelNumber[iInp];
    fAcqChannelOffset    = fc->GetPlanePar(fPlaneNumber).ChannelOffset[iInp]; // JB 2013/08/15
	  fChannelsN           += fc->GetPlanePar(fPlaneNumber).Channels[iInp];
	  fAcqModuleChannels   = fc->GetPlanePar(fPlaneNumber).Channels[iInp]; // print the #channels from the plane config, not from the module one, JB 2009/05/25
	  if (fDebugPlane) {
	    cout << "  fAcqInputsN=" << fAcqInputsN << endl;
	    cout << "  fAcqModuleTypeNumber=" << fAcqModuleTypeNumber << endl;
	    cout << "  fAcqModuleNumber " << fAcqModuleNumber << endl;
	    cout << "  fAcqInputNumber " << fAcqInputNumber << endl;
	    cout << "  fAcqChannelNumber " << fAcqChannelNumber << endl;
	    cout << "  fAcqChannelOffset " << fAcqChannelOffset << endl;
	    cout << "  fAcqModuleChannels " << fAcqModuleChannels << endl;
	    cout << "  fChannelsN " << fChannelsN << endl;
	  }
  }

  // Some acquisition modules use a Timestamp,
  // in which case, a time limit to associate to pixel in a hit is required
  // otherwise this limit is set to 0.
  // JB 2015/05/26
  fTimeLimit = fc->GetPlanePar(fPlaneNumber).TimeLimit;
  if( fAcqInputsN!=0 && fAcqModuleTypeNumber!=0 && fAcq->GetUsageTimestamp( fAcqModuleTypeNumber, fAcqModuleNumber) ) {
    if( fTimeLimit < 0 ) { // <0 if never set, complain
      cout << "WARNING: plane " << fPlaneNumber << " uses timestamp but no individual time limit set, trying tracker limit: " << fc->GetTrackerPar().TimeLimit << endl;
      fTimeLimit = fc->GetTrackerPar().TimeLimit;
      if( fTimeLimit < 0 ) {
        cout << endl << "ERROR, plane " << fPlaneNumber << " uses timestamp but no time limit is provided in config file !!!" << endl << endl;
        return;
      }
    }
    else {
      if (fDebugPlane) cout << " this plane uses a time limit at = " << fTimeLimit << " to constraint pixel timestamps in the same hit." << endl;
    }
  }
  else {
    fTimeLimit = 0;
    if (fDebugPlane) cout << " this plane don't use timestamp." << endl;
  }

  fStripsNu            = (Int_t) fc->GetPlanePar(fPlaneNumber).Strips(0);
  fStripsNv            = (Int_t) fc->GetPlanePar(fPlaneNumber).Strips(1);
  fStripsN             = fStripsNu*fStripsNv;
  if (fDebugPlane) cout << "  fStrips " << fStripsN << endl;

  if( fStripsN != fChannelsN ) {
    printf("\nWARNING #strips(uxv) %d IS NOT #channels %d...do you know what you are doing ?\n", fStripsN, fChannelsN);
  }

  fCut                 = new DCut(*this);
  if( fAcqModuleTypeNumber!=0 )fPulseRangeMaximum   = pow(2.,fc->GetModulePar(fAcqModuleTypeNumber).SigBits[fAcqInputNumber]); // ???, JB, 2008/10/13


  //-+-+-+   Potential digitization emulation of this plane
  // IfDigitize = 0 for no emulation
  // IfDigitize = 1 to emulate a simple discriminator (1 threshold)
  // IfDigitize > 1 to emulate an ADC with n bits sucha as 2^n = IfDigitize thresholds
  // JB 2013/07/17, mimicking the MAF ADC emulation

  fIfDigitize = fc->GetPlanePar(fPlaneNumber).IfDigitize;
  if( fIfDigitize>0 ) { // if Digitization emulation required
    cout << "  Emulating digitization over " << fIfDigitize << " threshold(s):";

    fDigitizeThresholds = new Int_t[fIfDigitize];
    for (Int_t ithre=0; ithre<fIfDigitize; ithre++) {
      fDigitizeThresholds[ithre] = fc->GetPlanePar(fPlaneNumber).DigitizeThresholds[ithre];
      cout << " " << fDigitizeThresholds[ithre];
    }
    cout << endl;

  }  // end if Digitization emulation required


  //-+-+-+   Geometry of the plane

  fPitch               = new DR3(fc->GetPlanePar(fPlaneNumber).Pitch);
  //fSize                = new DR3(fc->GetPlanePar(fPlaneNumber).Size);
  fSize = new DR3( fStripsNu*(*fPitch)(0), fStripsNv*(*fPitch)(1), (*fPitch)(2));
  fGeometry            = new TBRIK(fPlaneName,"DT","void",(*fSize)(0),(*fSize)(1),(*fSize)(2));

  // Taking into account deformation or not
  // JB 2014/04/21
  fIfDeformation = fc->GetPlanePar(fPlaneNumber).IfDeformed;
  fUDeformationCoef[0] = (*fSize)(0)/2;
  fVDeformationCoef[0] = (*fSize)(1)/2;
  for ( int i=1; i<8; i++) {
    fUDeformationCoef[i] = fc->GetPlanePar(fPlaneNumber).CoeffLegendreU[i-1];
    fVDeformationCoef[i] = fc->GetPlanePar(fPlaneNumber).CoeffLegendreV[i-1];
  }
  if (fDebugPlane) {
    if (fIfDeformation) {
      printf("  DPlane %2d, surface deformation applied with coeeficients:\n", fPlaneNumber);
      printf("    along U:");
      for ( int i=0; i<8; i++) {
        printf(" %.3f", fUDeformationCoef[i]);
      }
      printf("\n");
      printf("    along V:");
      for ( int i=0; i<8; i++) {
        printf(" %.3f", fVDeformationCoef[i]);
      }
      printf("\n");
    }
    else {
      printf("  DPlane %2d, no surface deformation applied\n", fPlaneNumber);
    }
  }


  //-+-+-+   Position of the plane

  // initial tilt about the axis perpendicular to the plane (w-axis)
  fTilt                = new DR3(fc->GetPlanePar(fPlaneNumber).Tilt);
  if (fDebugPlane)   {printf("  DPlane %2d,  Intitial Tilt around z, y, x axis [rad]  : ",fPlaneNumber);  fTilt->Print();}

  // initial position in xyz coordinates
  fPosition            = new DR3(fc->GetPlanePar(fPlaneNumber).Position);
  if (fDebugPlane) {printf("              Initial Position(in xyz) [um]: "); fPosition->Print();}

  // Initial alignment parameters
  Int_t DPrecAlignMethod = fc->GetTrackerPar().DPrecAlignMethod;   // LC 2015/01/31
  fPrecAlign = new DPrecAlign(DPrecAlignMethod);                   // LC 2015/01/31
  fPrecAlign->SetTranslation( *fPosition); // BEWARE, translation to be set first, always!
  fPrecAlign->SetRotations( *fTilt);       // because SetRot method compute the plane
  // Passing deformation coeff to DPrecAlign object
  if (fIfDeformation) {
    fPrecAlign->SetDeformation( fUDeformationCoef, fVDeformationCoef);
  }
  if (fDebugPlane) fPrecAlign->PrintAll();

  // Additional alignment parameters
  // Rotation around W axis
  // Translations along U and V
  fTiltW = fc->GetPlanePar(fPlaneNumber).AlignmentTilt;
  fPositionU =  fc->GetPlanePar(fPlaneNumber).AlignmentU;
  fPositionV =  fc->GetPlanePar(fPlaneNumber).AlignmentV;
  if (fDebugPlane) {printf("              TiltW    [rad]: %f\n",fTiltW);}
  if (fDebugPlane) {printf("              PositionU [um]: %f\n",fPositionU);}
  if (fDebugPlane) {printf("              PositionV [um]: %f\n",fPositionV);}

  // Convolute the UVW alignment with global one (in XYZ)
  // so far, the new rotation angles are not updated, just the matrices (2010/11/25)
  fPrecAlign->ConvoluteUVWAlignment( fPositionU, fPositionV, fTiltW);
  fTilt->SetValue( (Float_t)fPrecAlign->GetRotations()[0], (Float_t)fPrecAlign->GetRotations()[1], (Float_t)fPrecAlign->GetRotations()[2] );
  fPosition->SetValue( (Float_t)fPrecAlign->GetTranslation()[0], (Float_t)fPrecAlign->GetTranslation()[1], (Float_t)fPrecAlign->GetTranslation()[2]);
  if (fDebugPlane) fPrecAlign->PrintAll();

  if (fDebugPlane) { printf("              Final   Tilt    [rad]: "); fTilt->Print();}
  if (fDebugPlane) { printf("              Final   Position [um]: "); fPosition->Print();}

  fPlaneNode           = new TNode(fPlaneName,fPlaneName,fPlaneName,(*fPosition)(0),(*fPosition)(1),(*fPosition)(2));


  //-+-+-+   event count necessary for initialization

  fInitialPedestal = fc->GetPlanePar(fPlaneNumber).InitialPedestal; // note this is never used
  fInitialNoise = fc->GetPlanePar(fPlaneNumber).InitialNoise;

  if (fDebugPlane)  printf("  DPlane %d : InitPed= %d, InitNoise=%d\n", fPlaneNumber, fInitialPedestal, fInitialNoise);


  //-+-+-+ If a noise run has been specified
  // YV 21/10/09 for S/N ratio and pedestal subtraction
  // If analysis mode corresponds to real binary output data,
  //  force fNoiseRun to 0, JB 2014/01/08
  // Moved up here because needed in Strip creation if fNoiseRun>0

  if( fAnalysisMode==3 && !fIfDigitize ) { // if real binary output
    fNoiseRun = 0;
  }
  else { // otherwise
    fNoiseRun = fc->GetRunPar().NoiseRun;
  }

  Char_t    NoiseFileDataPath[350];
  Char_t    histoName[400];
  //  sprintf(NoiseFileDataPath,"%s/Noise_run%d.root",fSession->GetResultDirName().Data(), fNoiseRun);
  sprintf(NoiseFileDataPath,"Results/%d/Noise_run%d.root",fNoiseRun, fNoiseRun); // changed JB 2014/01/07
  sprintf(NoiseFileDataPath,"%s", fTool.LocalizeDirName( NoiseFileDataPath)); // JB 2011/07/07
  if( fDebugPlane) printf("name of noise file = %s \n",NoiseFileDataPath);
  if( fNoiseRun > 0 /*&& fReadout != 126*/ && fReadout!= 0 ) { // test if noise run provided, JB 2010/04/27
    // Exclusion of real binary output sensors not needed anymore, JB 2014/01/07
    fNoiseFile = new TFile(NoiseFileDataPath,"read");
    sprintf( histoName, "hnoisepl%d", fPlaneNumber);
    fHNoise = (TH2F*)fNoiseFile->Get(histoName);
    sprintf( histoName, "hPedestalpl%d", fPlaneNumber);
    fHPedestal = (TH2F*)fNoiseFile->Get(histoName);
    if( fHPedestal!=nullptr  && fHNoise!=nullptr ) {
      printf("  Dplane %d, noise map extracted properly from %s\n", fPlaneNumber, NoiseFileDataPath);
    }
    //fHPedestal->Draw("colz");
  }
  else {
    fNoiseFile = 0;
    fHNoise = 0;
    fHPedestal = 0;
  }

  //-+-+-+ If a pixel gain map has been specified
  // If analysis mode corresponds to real binary output data,
  //  force fPixelGainRun to 0
  // JB 2018/07/01

  if( fAnalysisMode==3 && !fIfDigitize ) { // if real binary output
    fPixelGainRun = 0;
  }
  else { // otherwise
    fPixelGainRun = fc->GetRunPar().PixelGainRun;
  }

  Char_t    gainFileDataPath[350];
  sprintf(gainFileDataPath,"Results/%d/PixelGain_run%d.root",fPixelGainRun, fPixelGainRun);
  sprintf(gainFileDataPath,"%s", fTool.LocalizeDirName( gainFileDataPath)); // JB 2011/07/07
  if( fDebugPlane) printf("name of pixelgain file = %s \n",gainFileDataPath);
  if( fPixelGainRun > 0 && fReadout!= 0 ) {
    fGainFile = new TFile(gainFileDataPath,"read");
    sprintf( histoName, "hpixelgainpl%d", fPlaneNumber);
    fHPixelGain = (TH2F*)fGainFile->Get(histoName);
    if( fHPixelGain!=nullptr ) {
      printf("  Dplane %d, pixel gain map extracted properly from %s\n", fPlaneNumber, gainFileDataPath);
    }
  }
  else {
    fGainFile = 0;
    fHPixelGain = 0;
  }


  //=======
  if( fReadout!=0 && ( fReadout < 100 || fReadout == 232 ) ) { // Useless part for sparsified data and unread plane, JB 2010/11/25, 2011/02/09, 2013/06/23
    //=======

    //-+-+-+   create arrays for computing pedestal, noise, pulsheight, etc.

    fRegions             = fc->GetPlanePar(fPlaneNumber).CommonRegions;

    fChannel             = new Long_t[fChannelsN];   // index array of channels
    fChannelGood         = new Long_t[fChannelsN];   // array of good channels
    fStripIndices        = new Long_t[fStripsN];     // index array of strips

    fCommonShift         = new Float_t[fRegions];
    fCommonChannels      = new Long_t[fRegions];

    for(Int_t region = 0; region < fRegions; region++) {
      fCommonShift[region]    = 0.;
      fCommonChannels[region] = 0;
    }
    // mark all channels as good:

    for (ch = 1; ch <=fChannelsN; ch++) {
      fChannelGood[ch] = 1;
    }

    for (ch = 0; ch < fChannelsN; ch++){
      fChannel[ch]     = ch+1;
    }

    //-+-+-+   Building strips
    // NOTE THAT STRIP COUNTING STARTS AT 0
    // the assigned position there does not take into account the plane alignement,
    // i.e. position (0,0) is the center of the plane.
    // i.e. position (-fStripsNu/2,-fStripsNv/2) is the left bottom of the plane.
    // checked by JB 2008/10/17

    if (fDebugPlane)   printf("  DPlane: Constructing DStrips: %dx%d = %d\n", fStripsNu, fStripsNv, fStripsN);

    fStripList           = new DStrip*[fStripsN];

    DR3      tPosition(0,0,0);
    DR3      tSize(0,0,0);
    Double_t  u,v,w;
    u=v=w=0;

    for (Int_t stv = 0; stv < fStripsNv; stv++) { // lpop on rows
      for (Int_t stu = 0; stu < fStripsNu; stu++) { // loop on columns
        st = stv*fStripsNu + stu;
        if (fDebugPlane && st<0) {
          printf("WARNING Strip number < 0 !! st=%d for stu=%d stv=%d\n",st,stu,stv);
        }
        //if( st<128) cout << "Creating strip " << st << endl;
        ComputeStripPosition( stu, stv, u, v, w); // new function introduced, JB 2012/11/21
        //        u = ((2*stu - fStripsNu + 1 ) * fc->GetPlanePar(fPlaneNumber).Pitch(0))/2 ;
        //        v = ((2*stv - fStripsNv + 1 ) * fc->GetPlanePar(fPlaneNumber).Pitch(1))/2 ;
        //        w = fc->GetPlanePar(fPlaneNumber).Pitch(2);
        //if( fAnalysisMode >= 2 && 200<st && st<260 ) cout << " strip " << st << " (" << stu << ", " << stv << "): u=" << u << " v=" << v << " w=" << w << endl;
        tPosition.SetValue(u,v,w);
        //if( st<3) cout << "   size " << fc->GetPlanePar(fPlaneNumber).StripSize(0) << " " << fc->GetPlanePar(fPlaneNumber).StripSize(1) << " " << fc->GetPlanePar(fPlaneNumber).StripSize(2) << endl;
        fStripList[st]  = new DStrip(*this, st, tPosition, (*fPitch));

        // If a noise run has been specified, set the pedestal and noise
        // JB 2014/01/07
        if( fNoiseRun && fHNoise!=NULL && fHPedestal!=NULL ) {
          GetStrip(st)->SetPedestal( fHPedestal->GetBinContent( stu+1, stv+1));
          GetStrip(st)->SetNoise( fHNoise->GetBinContent( stu+1, stv+1));
	  if( fDebugPlane>3) printf("DPlane: strip %d (col=%d, row=%d), ped=%.1f, noise=%.1f\n", st, stu, stv, fHPedestal->GetBinContent( stu+1, stv+1), fHNoise->GetBinContent( stu+1, stv+1));
        }

      } // lpop on columns
    } // lpop on rows

    FindNeighbours();

    //=======
  } // end useless for sparsified data
  //=======


  //-+-+-+   Hit list

  fHitMax = fc->GetTrackerPar().HitsInPlaneMaximum;
  fKeepUnTrackedHitsBetw2evts = fc->GetTrackerPar().KeepUnTrackedHitsBetw2evts; // VR 2014.08.28
  fHitsUnTrackedLastEventN = 0; // VR 2014.08.28
  DR3  aZero;



  //------------------------
  // If asked, prepare for Keep UnTrackedHits Betwen 2 events // VR 2014.08.28
  //------------------------
  if(fKeepUnTrackedHitsBetw2evts == 0)
  {
    fHit = new DHit*[fHitMax];

    for (Int_t ht = 0; ht < fHitMax; ht++)
    {
      fHit[ht] = new DHit(aZero, *this, ht+1);
    }
  }
  else
  {
    printf("Keeping untracked hits from an event to the next one in enabled \n");

    fHit = new DHit*[2*fHitMax];

    for (Int_t ht = 0; ht < 2*fHitMax; ht++)
    {
      fHit[ht] = new DHit(aZero, *this, ht+1);
    }

    fHitUnTrackedLastEvent = new DHit*[fHitMax];
    for (Int_t ht = 0; ht < fHitMax; ht++)
    {
      fHitUnTrackedLastEvent[ht] = new DHit(aZero, *this, ht+1);
    }
  }

  //-+-+-+   Alignement object

  fAlign = new DAlign( fPlaneNumber, fDebugPlane);
  fAlign->SetEvents( fc->GetTrackerPar().EventsForAlignmentFit ); // JB 2009/05/25


  //-+-+-+  connect this plane to its raw data array  -+-+-+-//

  //fRawData = fAcq->GetRawData( fAcqModuleTypeNumber, fAcqModuleNumber, fAcqInputNumber);
  fListOfPixels = fAcq->GetListOfPixels( fPlaneNumber); // JB, 2009/05/03
  //if(fIfHitMonteCarlo==1) fListOfMonteCarlo = fAcq->GetListOfMonteCarlo( fPlaneNumber); // LC 2012/10/19
  if (fDebugPlane)   printf(" RawData array associated at %d-%d-%d\n", fAcqModuleTypeNumber, fAcqModuleNumber, fAcqInputNumber);


  //-+-+-+  connect to histos or tables for eta distribution -+-+-+-//
  // method to connect 2D-eta added, JB Sept 2007

  //TFile *tWeightFile = fSession->GetWeightFile();

  fChargeFractionDensityList      = new TObjArray(10*10);
  fChargeFractionDistributionList = new TObjArray(10*10);
  Char_t tChargeFractionDataName[100];
  TH1F  *tH1 = 0;

  fEtaIntU = 0;
  fEtaIntV = 0;
  fEtaIntU2 = 0;
  fEtaIntV2 = 0;
  fDigitalStripResponse = kFALSE;

  TString WeightFile = fSession->GetResultDirName();
  WeightFile += "eta";
  WeightFile += long(fSession->GetRunNumber());
  WeightFile += ".root";
  fTool.LocalizeDirName( &WeightFile);
  FileStat_t Fstat;
  TFile *tWeightFile = NULL;
  if(gSystem->GetPathInfo(WeightFile.Data(),Fstat) == 0) {
    tWeightFile = new TFile(WeightFile.Data(),"READ");
    if( tWeightFile == NULL) {
      fDigitalStripResponse = kTRUE;
      if( fc->GetPlanePar(fPlaneNumber).HitPositionAlgorithm == 2 ) { // if eta algorithm required
        cout << "WARNING from DPlane::DPlane(), " << "file " << WeightFile.Data() << " Doesn't exits!!" << endl;
      } //end if eta algorithm required
    }
    else if( tWeightFile->IsZombie() || !(tWeightFile->GetNkeys()) ) {
      fDigitalStripResponse = kTRUE;
      if( fc->GetPlanePar(fPlaneNumber).HitPositionAlgorithm == 2 ) { // if eta algorithm required
        if( tWeightFile->IsZombie())          cout << "WARNING from DPlane::DPlane(), " << "file " << WeightFile.Data() << " is Zombie!!!" << endl;
        else if(!(tWeightFile->GetNkeys()) )  cout << "WARNING from DPlane::DPlane(), " << "file " << WeightFile.Data() << " has no keys!!!" << endl;
      } //end if eta algorithm required
    }
  }
  else {
    fDigitalStripResponse = kTRUE;
    if( fc->GetPlanePar(fPlaneNumber).HitPositionAlgorithm == 2 ) { // if eta algorithm required
      cout << "WARNING from DPlane::DPlane(), " << "file " << WeightFile.Data() << " Doesn't exits!!" << endl;
    } //end if eta algorithm required
  }

  if (!fDigitalStripResponse && fAnalysisMode == 1){ // STRIPS
    // use only 2-strip clusters
    // now the loop over strips with decending pulseheights, start with seed, which has highest signal

    for(Int_t tSk = 0; tSk < 2; tSk++) {  // loop over strips with highest pulseheights
      sprintf(tChargeFractionDataName,"HEtaDenPk%d",fPlaneNumber);
      tH1 =  (TH1F*)tWeightFile->Get(tChargeFractionDataName);
      if (tH1==0) {
        fDigitalStripResponse = kTRUE;
        if (fDebugPlane) cout << " No charge fraction hist!\n";
      }
      tH1->SetDirectory(0);
      fChargeFractionDensityList->AddAt( tH1, tSk);
    }
  }
  else if (!fDigitalStripResponse && fAnalysisMode >= 2){ // PIXELS
    sprintf(tChargeFractionDataName,"HEtaIntUPk%d",fPlaneNumber);
    fEtaIntU = (TH1F*)tWeightFile->Get(tChargeFractionDataName);
    sprintf(tChargeFractionDataName,"HEtaIntVPk%d",fPlaneNumber);
    fEtaIntV = (TH1F*)tWeightFile->Get(tChargeFractionDataName);
    sprintf(tChargeFractionDataName,"HEtaInt2UPk%d",fPlaneNumber);
    fEtaIntU2 = (TH1F*)tWeightFile->Get(tChargeFractionDataName);
    sprintf(tChargeFractionDataName,"HEtaInt2VPk%d",fPlaneNumber);
    fEtaIntV2 = (TH1F*)tWeightFile->Get(tChargeFractionDataName);
    if (fEtaIntU==0 || fEtaIntV==0 || fEtaIntU2==0 || fEtaIntV2==0) {
      fDigitalStripResponse = kTRUE;
      if (fDebugPlane) cout << " No hist with charge density in 2D for eta algo!\n";
    }
    fEtaIntU->SetDirectory(0);
    fEtaIntV->SetDirectory(0);
    fEtaIntU2->SetDirectory(0);
    fEtaIntV2->SetDirectory(0);
  }

  // issue a warning if Eta algo required and no Eta file available
  if ( fDigitalStripResponse && fc->GetPlanePar(fPlaneNumber).HitPositionAlgorithm==2 ) {
    if(gSystem->GetPathInfo(WeightFile.Data(),Fstat) != 0) {
      printf("\nWARNING from DPlane::DPlane(), file %s does not exist (access %d)\n",
	     WeightFile.Data(),
	     gSystem->AccessPathName(WeightFile.Data()));
    }
    else {
      printf("\nWARNING from DPlane::DPlane(), file %s does exist (access %d), but either it is empty (zombie %d, keys %d), either it does not contain the proper histos, using CoG for position method only\n",
	     WeightFile.Data(),
	     gSystem->AccessPathName(WeightFile.Data()),
	     tWeightFile->IsZombie(),
	     tWeightFile->GetNkeys());
    }

    printf("    you have to make a new one (use gTAF->MakeEta())!\n\n");
  }

  if(gSystem->GetPathInfo(WeightFile.Data(),Fstat) == 0) {
    tWeightFile->Close();
    delete tWeightFile;
  }


  if (tH1) {tH1 = 0;}


  //__________________________________________________________________________________________



  cout << " -*-*- DPlane " << aPlaneNumber << " User Constructor DONE -*-*- " << endl;

}



//______________________________________________________________________________
//


DPlane::~DPlane(){
  delete [] fStripList;
  delete fListOfPixels;
  delete [] fHit;
  delete [] fHitUnTrackedLastEvent;
  delete fCut;
  delete fGeometry;
  delete fPlaneNode;
  delete fPlaneNodeName;
  delete fChargeFractionDensityList;
  delete fChargeFractionDistributionList;
  delete fEtaIntU;
  delete fEtaIntV;
  delete fEtaIntU2;
  delete fEtaIntV2;
  delete fNoiseFile;  //YV 27/11/09

  delete spectrum;

}

//______________________________________________________________________________
//

void DPlane::SetDebug(Int_t aDebug)
{
  // Initialize or update the debug level
  // update also the level for hits, align objects
  // JB 2009/05/12
  // Modified JB 2015/10/31 to update DPrecAlign

  fDebugPlane = aDebug;

  fAlign->SetDebug( aDebug); // JB 2010/12/14

  if( fPrecAlign!=NULL ) fPrecAlign->SetDebug( aDebug); // JB 2015/10/31

  for (Int_t ht = 0; ht < fHitMax; ht++) {
    fHit[ht]->SetDebug( aDebug);
  }

  cout << "DPlane " << fPlaneNumber << " debug updated to " << fDebugPlane << endl;

}

//______________________________________________________________________________
//

TH1F* DPlane::GetChargeFractionDensity(Int_t tStripIndex) {
  return (TH1F*)fChargeFractionDensityList->At(tStripIndex);
}

//______________________________________________________________________________
//

TH1F* DPlane::GetChargeFractionDistribution(Int_t tStripIndex) {
  return (TH1F*)fChargeFractionDistributionList->At(tStripIndex);
}

//______________________________________________________________________________
//

void DPlane::ShowAlignment(){

  // Display the current alignement (offsets and tilt) of the plane
  // JB 2009/05/18
  // Modified JB 2011/04/05

  printf("Position of plane %d:\n", fPlaneNumber);
  printf("  Alignment U=%.1f um, V=%.1f, tiltW=%.3f [deg]\n", fPositionU, fPositionV, fTiltW*TMath::RadToDeg());
  fPrecAlign->PrintAll();

}
//______________________________________________________________________________
//

Bool_t DPlane::Align(DTrack  *aTrack, Bool_t ifFit, bool UseAllHits){

  // Compute the  alignment parameters
  //
  // Loop on the hits in the plane to find the one nearest to the track within a given bound.
  // Submit this hit to the DAlign object which will return alignement parameters
  // and update the bound to associate track and hit,
  // once enough hits have been accumulated (limit from config file).
  // Do it for both direction U, V if PIXELS, only in U for STRIPS.
  //
  // Return true if a fit was performed (enough hits), false otherwise.
  //
  // Modified by JB to allow 2D plane alignment, 2007 August
  // Modified to select best hit in 2D, JB 2009/05/09
  // Modified to allow no fit ifFit argumnet), JB 2009/08/23
  // Modified to update the global alignement with the DPrecAlign class, JB 2010/11/25
  // Modified to take properly into account the alignment corrections, JB 2011/04/05
  // Modified to have as output wether the fit was done or not, JB 2011/07/25
  // Modified AP 2015/06/10: added bool parameter (UseAllHits) to decide if doing alignment with all hits (UseAllHits = true),
  //                         or the closest one (UseAllHits = false). Default value is false.

  Bool_t        fitDone = kFALSE;

  Float_t       tDistanceU = 0, tDistanceV=0., tDist2D=0.;
  Float_t       tMinDistanceU = 0, tMinDistanceV = 0, tMinDistance2D = 0;
  DR3           tTrackPos;       // the Track Position in uvw coordinates
  //DR3          *tHitPos;         // the Hit Position in uvw coordinates, JB 2009/05/09

  // find the hit nearest to the track

  tTrackPos = Intersection( aTrack); // trackPosition in uvw frame with new class, JB 2010/11/25
  if( fDebugPlane>1 ) printf(" DPLane::Align plane %d Having track at position in plane: (XY)=(%f,%f), (UV)=(%f,%f)\n", fPlaneNumber, fPrecAlign->GetTrackPosition()(0), fPrecAlign->GetTrackPosition()(1), tTrackPos(0), tTrackPos(1));

  // take hit - track position in uvd Plane frame, the order/sign matters!!

  if(GetHitsN() > 0) {
    if(UseAllHits) {
      for (Int_t k=1; k<= GetHitsN(); k++) { // loop on hits
        //tHitPos    = GetHit(k)->GetPosition();
        tDistanceU = GetHit(k)->GetPositionUhit() - tTrackPos(0); // strips and pixels

        //if(fIfDeformation){ //BB
        //  Double_t tPositionHitU = GetHit(k)->GetPositionUhit();//BB
        //  Double_t tPositionTrackU = tTrackPos(0);//BB
        //}

        if(fAnalysisMode>=2 ) { // PIXELS only
          tDistanceV = GetHit(k)->GetPositionVhit() - tTrackPos(1);
          tDist2D    = sqrt( tDistanceU*tDistanceU + tDistanceV*tDistanceV );

          tMinDistance2D = tDist2D;
          tMinDistanceU  = tDistanceU;
          tMinDistanceV  = tDistanceV;
        }
        else { // STRIPS
          if (fabs(tDistanceU) <= fabs(tMinDistanceU)) {
            tMinDistanceU = tDistanceU;
          }
        }

        if( fDebugPlane ) printf("   DPLane::Align plane %d Accumulating with distance (u=%f,v=%f) 2d=%f\n", fPlaneNumber, tMinDistanceU, tMinDistanceV, tMinDistance2D);

        // Only store the distances if no fit recquired, JB 2009/05/13
        if( ifFit==kFALSE ) {
          if(fAnalysisMode>=2 ) { // PIXELS only
            if( fabs(tMinDistanceU)<fAlign->GetBounding() && fabs(tMinDistanceU)<fAlign->GetBounding() )
              fAlign->Store2D( tTrackPos, tMinDistanceU, tMinDistanceV);
          }
          else { // STRIPS
            fAlign->StoreU( tTrackPos, tMinDistanceU);
          }
          if(k == GetHitsN()) return fitDone;
        }

        //------------------------------------------
        // Below do the accumulation and fitting if ready

        //if( 1000<tTrackPos(1) && tTrackPos(1)<2500 ) { // Use this condition to align only with a given area
        if(fAnalysisMode>=2 ) { // PIXELS only
          fAlign->Accumulate2D(tTrackPos, tMinDistanceU, tMinDistanceV); // this line replace the two followings, JB 2009/05/09
        }
        else { // strips
          fAlign->AccumulateU(tTrackPos, tMinDistanceU);
        }

      }
    }
    else {
      tMinDistanceU  = 1000000.; // init
      tMinDistanceV  = 1000000.;
      tMinDistance2D = 1000000.;
      for (Int_t k=1; k<= GetHitsN(); k++) { // loop on hits
        //tHitPos    = GetHit(k)->GetPosition();
        tDistanceU = GetHit(k)->GetPositionUhit() - tTrackPos(0); // strips and pixels

        if(fAnalysisMode>=2 ) { // PIXELS only
          tDistanceV = GetHit(k)->GetPositionVhit() - tTrackPos(1);
          tDist2D    = sqrt( tDistanceU*tDistanceU + tDistanceV*tDistanceV );
          if( fabs(tDist2D) <= fabs(tMinDistance2D)) {
            tMinDistance2D = tDist2D;
            tMinDistanceU  = tDistanceU;
            tMinDistanceV  = tDistanceV;
          }
        }
        else { // STRIPS
          if (fabs(tDistanceU) <= fabs(tMinDistanceU)) {
            tMinDistanceU = tDistanceU;
          }
        }

        if( fDebugPlane>1 ) printf("   DPLane::Align plane %d Trying hit %d and distance (u=%f,v=%f) 2d=%f\n", fPlaneNumber, k, tDistanceU, tDistanceV, tDist2D);
      } // end loop on hits

      if( fDebugPlane ) printf("   DPLane::Align plane %d Accumulating with distance (u=%f,v=%f) 2d=%f\n", fPlaneNumber, tMinDistanceU, tMinDistanceV, tMinDistance2D);

      // Only store the distances if no fit recquired, JB 2009/05/13
      if( ifFit==kFALSE ) {
        if(fAnalysisMode>=2 ) { // PIXELS only
          if( fabs(tMinDistanceU)<fAlign->GetBounding() && fabs(tMinDistanceU)<fAlign->GetBounding() )
            fAlign->Store2D( tTrackPos, tMinDistanceU, tMinDistanceV);
        }
        else { // STRIPS
          fAlign->StoreU( tTrackPos, tMinDistanceU);
        }
        return fitDone;
      }

      //------------------------------------------
      // Below do the accumulation and fitting if ready

      //if( 1000<tTrackPos(1) && tTrackPos(1)<2500 ) { // Use this condition to align only with a given area
      if(fAnalysisMode>=2 ) { // PIXELS only
        fAlign->Accumulate2D(tTrackPos, tMinDistanceU, tMinDistanceV); // this line replace the two followings, JB 2009/05/09
        //fAlign->AccumulateV(tTrackPos, tMinDistanceV);
        //fAlign->AccumulateU(tTrackPos, tMinDistanceU);
      }
      else { // strips
        fAlign->AccumulateU(tTrackPos, tMinDistanceU);
      }
      //} // end if given area
      //------------------------------------------
    } // end else of if(UseAllHits)
  } // enf if there is some hits

  if (  ifFit==kTRUE && ( // JB 2009/08/23
                         (fAnalysisMode==1 && fAlign->EnoughU() == kTRUE) // strips
                         ||(fAnalysisMode>=2 && fAlign->EnoughU() == kTRUE && fAlign->EnoughV() == kTRUE) // pixels
                         ||(fAnalysisMode>=2 && fAlign->Enough2D() == kTRUE) // pixels, JB 2009/05/13
                         ) ) {
    fitDone = kTRUE;

    Int_t eventN = fSession->GetCurrentEventNumber();
    Int_t counts = fAlign->GetCounts();
    printf(" ---------------------------------------------------------- \n");
    printf(" Interactive Alignment of plane %d %s (swap %.2f deg): Event %d kept %d with bound %.1f um\n",fPlaneNumber, fPlaneName, GetTilt()(2)*180./M_PI, eventN, counts, fAlign->GetBounding());
    printf(" ---------------------------------------------------------- \n");
    cout << " Current position" << endl;
    ShowAlignment();

    // printout computed corrections for this iteration
    fAlign->ShowCorrection();

    // Compute the new position taking into account alignment params
    // offset are now added, to match sign changed in DPrecAlign, JB 2011/07/25
    fPrecAlign->ConvoluteUVWAlignment( +fAlign->GetOffsetU(), +fAlign->GetOffsetV(), +fAlign->GetTiltW());
    fTilt->SetValue( (Float_t)fPrecAlign->GetRotations()[0], (Float_t)fPrecAlign->GetRotations()[1], (Float_t)fPrecAlign->GetRotations()[2] );
    fPosition->SetValue( (Float_t)fPrecAlign->GetTranslation()[0], (Float_t)fPrecAlign->GetTranslation()[1], (Float_t)fPrecAlign->GetTranslation()[2]);

    // Reset alignment params
    fTiltW     = 0.;
    fPositionU = 0.;
    fPositionV = 0.;

    cout << " New position" << endl;
    ShowAlignment();

    // re-init align counters for next iteration
    fAlign->Modified();

  } // end if enough events

  return fitDone;

}


//______________________________________________________________________________
//

Bool_t DPlane::AlignAllHits(DTrack  *aTrack, Bool_t ifFit){

  // Compute the  alignment parameters,
  //  !!! COPY OF ALIGN method above, but all hits considered for each track !!
  //
  // Created JB, 2014/05/14

  Bool_t        fitDone = kFALSE;

  Float_t       tDistanceU, tDistanceV=0., tDist2D=0.;
  DR3           tTrackPos;       // the Track Position in uvw coordinates
  //DR3          *tHitPos;         // the Hit Position in uvw coordinates, JB 2009/05/09

  // find the hit nearest to the track

  tTrackPos = Intersection( aTrack); // trackPosition in uvw frame with new class, JB 2010/11/25
  if( fDebugPlane>1 ) printf(" DPLane::Align plane %d Having track at position in plane: (XY)=(%f,%f), (UV)=(%f,%f)\n", fPlaneNumber, fPrecAlign->GetTrackPosition()(0), fPrecAlign->GetTrackPosition()(1), tTrackPos(0), tTrackPos(1));

  // take hit - track position in uvd Plane frame, the order/sign matters!!

  if (GetHitsN() > 0) {

    for (Int_t k=1; k<= GetHitsN(); k++) { // loop on hits

      //tHitPos = GetHit(k)->GetPosition();
      tDistanceU = GetHit(k)->GetPositionUhit() - tTrackPos(0); // strips and pixels

      if(fAnalysisMode>=2 ) { // PIXELS only
        tDistanceV = GetHit(k)->GetPositionVhit() - tTrackPos(1);
        tDist2D = sqrt( tDistanceU*tDistanceU + tDistanceV*tDistanceV );
      }

      if( fDebugPlane ) printf("   DPLane::Align plane %d Trying hit %d and distance (u=%f,v=%f) 2d=%f\n", fPlaneNumber, k, tDistanceU, tDistanceV, tDist2D);

      // Only store the distances if no fit recquired, JB 2009/05/13
      if( ifFit==kFALSE ) {
        if(fAnalysisMode>=2 ) { // PIXELS only
          if( fabs(tDistanceU)<fAlign->GetBounding() && fabs(tDistanceU)<fAlign->GetBounding() )
            fAlign->Store2D( tTrackPos, tDistanceU, tDistanceV);
        }
        else { // STRIPS
          fAlign->StoreU( tTrackPos, tDistanceU);
        }
        return fitDone;
      }

      //------------------------------------------
      // Below do the accumulation and fitting if ready

      //if( 1000<tTrackPos(1) && tTrackPos(1)<2500 ) { // Use this condition to align only with a given area
      if(fAnalysisMode>=2 ) { // PIXELS only
        fAlign->Accumulate2D(tTrackPos, tDistanceU, tDistanceV); // this line replace the two followings, JB 2009/05/09
        //fAlign->AccumulateV(tTrackPos, tDistanceV);
        //fAlign->AccumulateU(tTrackPos, tDistanceU);
      }
      else { // strips
        fAlign->AccumulateU(tTrackPos, tDistanceU);
      }
      //} // end if given area
      //------------------------------------------

    } // end loop on hits

  } // enf if there is some hits

  if (  ifFit==kTRUE && ( // JB 2009/08/23
                         (fAnalysisMode==1 && fAlign->EnoughU() == kTRUE) // strips
                         ||(fAnalysisMode>=2 && fAlign->EnoughU() == kTRUE && fAlign->EnoughV() == kTRUE) // pixels
                         ||(fAnalysisMode>=2 && fAlign->Enough2D() == kTRUE) // pixels, JB 2009/05/13
                         ) ) {
    fitDone = kTRUE;

    Int_t eventN = fSession->GetCurrentEventNumber();
    Int_t counts = fAlign->GetCounts();
    printf(" ---------------------------------------------------------- \n");
    printf(" Interactive Alignment of plane %d %s (swap %.2f deg): Event %d kept %d with bound %.1f um\n",fPlaneNumber, fPlaneName, GetTilt()(2)*180./M_PI, eventN, counts, fAlign->GetBounding());
    printf(" ---------------------------------------------------------- \n");
    cout << " Current position" << endl;
    ShowAlignment();

    // printout computed corrections for this iteration
    fAlign->ShowCorrection();

    // Compute the new position taking into account alignment params
    // offset are now added, to match sign changed in DPrecAlign, JB 2011/07/25
    fPrecAlign->ConvoluteUVWAlignment( +fAlign->GetOffsetU(), +fAlign->GetOffsetV(), +fAlign->GetTiltW());
    fTilt->SetValue( (Float_t)fPrecAlign->GetRotations()[0], (Float_t)fPrecAlign->GetRotations()[1], (Float_t)fPrecAlign->GetRotations()[2] );
    fPosition->SetValue( (Float_t)fPrecAlign->GetTranslation()[0], (Float_t)fPrecAlign->GetTranslation()[1], (Float_t)fPrecAlign->GetTranslation()[2]);

    // Reset alignment params
    fTiltW     = 0.;
    fPositionU = 0.;
    fPositionV = 0.;

    cout << " New position" << endl;
    ShowAlignment();

    // re-init align counters for next iteration
    fAlign->Modified();

  } // end if enough events

  return fitDone;

}


//______________________________________________________________________________
//

void DPlane::UpdatePositions()
{

  // Update the rotation and translation vectors according to the values
  //  stored in the DPrecAlign object associated with this plane.
  //
  // LC

  fTilt->SetValue( (Float_t)fPrecAlign->GetRotations()[0], (Float_t)fPrecAlign->GetRotations()[1], (Float_t)fPrecAlign->GetRotations()[2] );
  fPosition->SetValue( (Float_t)fPrecAlign->GetTranslation()[0], (Float_t)fPrecAlign->GetTranslation()[1], (Float_t)fPrecAlign->GetTranslation()[2]);

}

//______________________________________________________________________________
//

void DPlane::AlignPrec(DTrack  *aTrack,
                       const Float_t tiniBound,
                       bool UseAllHits){

  /*
   LC 2012/09/06 : New method to fit with Minuit.
   This methos add data before the fit with Minuit.
   AP 2015/06/08 : added bool parameter (UseAllHits) to decide if doing alignement with all hits or with the one closest to the track
   - If UseAllHits = true  => Use all hits within a distance tiniBound to the track
   - If UseAllHits = false => Use the closest hits within a distance tiniBound to the track
   */

  Float_t       tDistanceU=0., tDistanceV=0., tDist2D=0.;

  DR3           tTrackPos;       // the Track Position in uvw coordinates.
  //DR3           *tHitPos;        // the Hit Position in uvw coordinates.

  tTrackPos = Intersection( aTrack); // trackPosition in uvw frame with new class, JB 2010/11/25

  if( fDebugPlane>1 ) printf(" DPLane::Align plane %d Having track at position in plane: (XY)=(%f,%f), (UV)=(%f,%f)\n", fPlaneNumber, fPrecAlign->GetTrackPosition()(0), fPrecAlign->GetTrackPosition()(1), tTrackPos(0), tTrackPos(1));

  Float_t tDist2Dmin     = 1.0e+20;
  Int_t   idx_closestHit = -999;
  for (Int_t k=1 ; k<= GetHitsN() ; k++) {

    tDistanceU = GetHit(k)->GetPositionUhit() - tTrackPos(0);
    tDistanceV = GetHit(k)->GetPositionVhit() - tTrackPos(1);
    tDist2D    = sqrt( tDistanceU*tDistanceU + tDistanceV*tDistanceV );

    if(!UseAllHits) {
      // Looking for the hit cloest to the track
      if(tDist2Dmin > tDist2D) {
        tDist2Dmin     = tDist2D;
        idx_closestHit = k;
      }
    }
    else {
      if (fAnalysisMode>=2 && tDist2D < tiniBound ) { // tDist2D < tiniBound.
        fPrecAlign->NewData( GetHit(k)->GetPositionUhit(), GetHit(k)->GetPositionVhit(),
                            GetHit(k)->GetResolutionUhit(), GetHit(k)->GetResolutionVhit(),
                            aTrack->GetLinearFit().GetOrigin()(0), aTrack->GetLinearFit().GetOrigin()(1), aTrack->GetLinearFit().GetOrigin()(2),
                            aTrack->GetLinearFit().GetSlopeZ()(0), aTrack->GetLinearFit().GetSlopeZ()(1) );

        if( fDebugPlane>1 ) printf(" DPLane::Align Event %i plane %d Accumulating data with parameters : (Hu,Hv)=(%f,%f), (Tx,Ty,Tz,Tdx,Tdy)=(%f,%f,%f,%f,%f)\n", fAcq->GetEventNumber(), fPlaneNumber, GetHit(k)->GetPositionUhit(), GetHit(k)->GetPositionVhit(), aTrack->GetLinearFit().GetOrigin()(0), aTrack->GetLinearFit().GetOrigin()(1), aTrack->GetLinearFit().GetOrigin()(2), aTrack->GetLinearFit().GetSlopeZ()(0), aTrack->GetLinearFit().GetSlopeZ()(1) );

        fAlign->Store2D( tTrackPos, tDistanceU, tDistanceV);  // To fill the graphs after the fit.

      } // end if.
    } //end of else

  } // end for.

  if(!UseAllHits && idx_closestHit >= 1) {
    tDistanceU = GetHit(idx_closestHit)->GetPositionUhit() - tTrackPos(0);
    tDistanceV = GetHit(idx_closestHit)->GetPositionVhit() - tTrackPos(1);
    tDist2D    = sqrt( tDistanceU*tDistanceU + tDistanceV*tDistanceV );

    if (fAnalysisMode>=2 && tDist2D < tiniBound ) { // tDist2Dmin < tiniBound.
      fPrecAlign->NewData( GetHit(idx_closestHit)->GetPositionUhit(),   GetHit(idx_closestHit)->GetPositionVhit(),
                          GetHit(idx_closestHit)->GetResolutionUhit(), GetHit(idx_closestHit)->GetResolutionVhit(),
                          aTrack->GetLinearFit().GetOrigin()(0), aTrack->GetLinearFit().GetOrigin()(1), aTrack->GetLinearFit().GetOrigin()(2),
                          aTrack->GetLinearFit().GetSlopeZ()(0), aTrack->GetLinearFit().GetSlopeZ()(1) );

      if( fDebugPlane>1 ) printf(" DPLane::Align Event %i plane %d Accumulating data with parameters : (Hu,Hv)=(%f,%f), (Tx,Ty,Tz,Tdx,Tdy)=(%f,%f,%f,%f,%f)\n", fAcq->GetEventNumber(), fPlaneNumber, GetHit(idx_closestHit)->GetPositionUhit(), GetHit(idx_closestHit)->GetPositionVhit(), aTrack->GetLinearFit().GetOrigin()(0), aTrack->GetLinearFit().GetOrigin()(1), aTrack->GetLinearFit().GetOrigin()(2), aTrack->GetLinearFit().GetSlopeZ()(0), aTrack->GetLinearFit().GetSlopeZ()(1) );

      fAlign->Store2D( tTrackPos, tDistanceU, tDistanceV);  // To fill the graphs after the fit.

    } // end if.
  } //end if (UseAllHits)

}

void DPlane::AlignData(DTrack *aTrack, const Double_t tiniBound, std::vector<Double_t>& myData)
{


  // Modified: LC 2014/12/05 : clean old code.
  Double_t       tDistanceU=0., tDistanceV=0., tDist2D=0.;
  //Double_t       tDistanceW=0.;

  DR3           tTrackPos;       // the Track Position in uvw coordinates.

  myData.clear();

  tTrackPos = Intersection( aTrack); // trackPosition in uvw frame with new class, JB 2010/11/25
  DR3 trackInTrackerFrame = fPrecAlign->TransformHitToTracker(tTrackPos);

  for (Int_t k=1 ; k<= GetHitsN() ; k++) {

    tDistanceU = GetHit(k)->GetPositionUhitCG() - tTrackPos(0);
    tDistanceV = GetHit(k)->GetPositionVhitCG() - tTrackPos(1);
    double resolutionU = GetHit(k)->GetResolutionUhit();
    double resolutionV = GetHit(k)->GetResolutionVhit();

    DR3 hitInTrackerFrame = fPrecAlign->TransformHitToTracker( DR3(GetHit(k)->GetPositionUhitCG(), GetHit(k)->GetPositionVhitCG(), 0.) );

    Double_t trackerFrameDistanceX = hitInTrackerFrame(0) - trackInTrackerFrame(0);
    Double_t trackerFrameDistanceY = hitInTrackerFrame(1) - trackInTrackerFrame(1);
    Double_t trackerFrameDistanceZ = hitInTrackerFrame(2) - trackInTrackerFrame(2);

    tDist2D = sqrt( tDistanceU*tDistanceU + tDistanceV*tDistanceV );

    if (fAnalysisMode>=2 && tDist2D < tiniBound ) {

      myData.push_back( GetHit(k)->GetPositionUhitCG() ); // 0
      myData.push_back( GetHit(k)->GetPositionVhitCG() ); // 1
      myData.push_back( aTrack->GetLinearFit().GetOrigin()(0) );  // 2
      myData.push_back( aTrack->GetLinearFit().GetOrigin()(1) );  // 3
      myData.push_back( aTrack->GetLinearFit().GetOrigin()(2) );  // 4
      myData.push_back( aTrack->GetLinearFit().GetSlopeZ()(0) );  // 5
      myData.push_back( aTrack->GetLinearFit().GetSlopeZ()(1) );  // 6
      myData.push_back( hitInTrackerFrame(0) );                   // 7
      myData.push_back( hitInTrackerFrame(1) );                   // 8
      myData.push_back( hitInTrackerFrame(2) );                   // 9

      /*
       myData.push_back( GetHit(k)->GetPositionUhitCG() - tTrackPos(0) ); //tDistanceU // 7
       myData.push_back( GetHit(k)->GetPositionVhitCG() - tTrackPos(1) ); //tDistanceV // 8
       myData.push_back( tDist2D );                                                    // 9
       */
      /*
       myData.push_back( GetHit(k)->GetPositionUhitCG22() - tTrackPos(0) );  // 10
       myData.push_back( GetHit(k)->GetPositionVhitCG22() - tTrackPos(1) );  // 11
       myData.push_back( GetHit(k)->GetPositionWhitCG() - tTrackPos(2) );    // 12
       */
      myData.push_back( trackerFrameDistanceX );                            // 10
      myData.push_back( trackerFrameDistanceY );                            // 11
      myData.push_back( trackerFrameDistanceZ );                            // 12

      myData.push_back( resolutionU );                                      // 13
      myData.push_back( resolutionV );                                      // 14

    }
  }

}
//______________________________________________________________________________
//

void  DPlane::AlignPrecUpdateConfig()
{

  /*
   LC 2012/09/06 : To Update the config file with fit parameters.
   */

  std::cout<<"   AlignPrecUpdateConfig()   "<<std::endl;
  fPrecAlign->PrintAll();

  fTilt->SetValue( (Float_t)fPrecAlign->GetRotations()[0], (Float_t)fPrecAlign->GetRotations()[1], (Float_t)fPrecAlign->GetRotations()[2] );
  fPosition->SetValue( (Float_t)fPrecAlign->GetTranslation()[0], (Float_t)fPrecAlign->GetTranslation()[1], (Float_t)fPrecAlign->GetTranslation()[2]);

}

//______________________________________________________________________________
//

Float_t DPlane::GetCommonModeRegion(Int_t aRegion) const {
  if (aRegion <= fRegions && aRegion > 0)
    return  fCommonShift[aRegion-1];
  else
    return  0.;
}

//______________________________________________________________________________
//

DStrip* DPlane::NearestStrip(DR3& aPosition){

  DStrip    *guess;

  Float_t    tDistance, tMinimum;
  Int_t      tK = 0;
  DR3        tPosition(0,0,0); // explicit zero
  tPosition  = aPosition;
  guess    = GetStrip(tK);
  tDistance = guess->DistanceU(tPosition);
  tMinimum = fabs(tDistance);
  // this is a very time consuming loop!
  for(Int_t k = 1; k < fStripsN; k++){
    guess    = GetStrip(k);
    tDistance = guess->DistanceU(tPosition);
    if ( fabs(tDistance) < tMinimum ) {
      tMinimum = fabs(tDistance);
      tK = k;
    }
  }

  return GetStrip(tK);
}

//______________________________________________________________________________
//

Float_t  DPlane::GetPulseRangeMaximum(){
  return fPulseRangeMaximum;
}

//______________________________________________________________________________
//

Float_t DPlane::GetPulseHeightToNoise(Int_t aSk){
  return GetStrip(aSk)->GetPulseHeightToNoise();
}


//______________________________________________________________________________
//

Float_t DPlane::GetCommonMode(Int_t aSk) {
  return  GetStrip(aSk)->GetCommonMode();
}

//______________________________________________________________________________
//

Float_t DPlane::GetRawValue(Int_t aSk) {
  return  GetStrip(aSk)->GetRawValue();
}



Float_t DPlane::GetRawFrame1Value(Int_t aSk) {
  return  GetStrip(aSk)->GetRawFrame1Value();
}

//

Float_t DPlane::GetRawFrame2Value(Int_t aSk) {
  return  GetStrip(aSk)->GetRawFrame2Value();
}

//______________________________________________________________________________
//

Float_t DPlane::GetPedestal(Int_t aSk) {
  return  GetStrip(aSk)->GetPedestal();
}

//______________________________________________________________________________
//

Float_t DPlane::GetPulseHeight(Int_t aSk) {
  return  GetStrip(aSk)->GetPulseHeight();
}

//______________________________________________________________________________
//

Float_t DPlane::GetNoise(Int_t aSk) {
  return  GetStrip(aSk)->GetNoise();
}

//______________________________________________________________________________
//

void DPlane::SetCDSvariance(Float_t aVar){
  fCDSvariance= aVar;
}

//______________________________________________________________________________
//

DPixel*  DPlane::GetPixelFromList(Int_t aSk)
{

  DPixel* aPixel = NULL;
  if(aSk+1 > (int)(fListOfPixels->size())) return  aPixel;
  else                              return  fListOfPixels->at(aSk);

}
//_________________________________________________________
//
DHit* DPlane::GetPrincipalHit() {
  // Modified: JB 2012/08/27 to return NULL if no hits
  if( fHitsN > 0 ) return  fHit[0];
  else return NULL;
}
//______________________________________________________________________________
//

Bool_t DPlane::Update(){

  // Calculate or update pedestal, pulseheights, noise, common mode shift on channels
  // then search for hits in plane
  //
  // Returns kFALSE if data are OK, kTRUE otherwise
  //
  // Pay attention that the old frawdata is now untrustable, use systematically GetStrip(st)->GetRawValue()
  //
  // Readout: (usually the number of the MIMOSA sensor)
  //  is used tp re-arrange raw data if needed
  // 0 = don't read
  // 1-99 = planes with non zero-supressed data
  // 100- = planes with zero-supressed data
  //
  // Last Modified, JB 2009/05/12
  // Last Modified, JB 2009/05/25
  // Last Modified, RDM 2009/05/26
  // Last Modified, JB 2009/08/17 for MIMOSA26 - PXI reading
  // Last Modified, JB 2010/09/20 added flag planeReady to indicate no problem even if no analysis
  // Last Modified, MB 2010/11/10 method to take into account pedestal corrected
  // Last Modified, SS 2012/08/10 for readout 32 (MIMOSA 32)
  // Last Modified, JB 2012/11/21 use method ComputeStripPosition
  // Last Modified, JB 2013/06/23 first MultiFrame Readout mode 232 for MIMOSA 32
  // Last Modified: VR 2014/07/12 replace aPixel->SetSize( (*fSize)) by aPixel->SetSize( (*fPitch))
  // Last Modified, JB 2016/07/20 better management of fNoiseRun option
  // Last Modified, JB 2016/08/17 signed value in multiframe mode
  // Last Modified, JB 2016/08/19 allow NoieRun option for readmode 232
  // Last Modified, JB 2018/05/04 new readout==3 mode for polarity inversion

  Bool_t goForAnalysis = kTRUE ;
  Bool_t planeReady = kTRUE ; // JB 2010/09/20
  fKillNoise=kFALSE;
  fHitsN = 0; // necessary otherwise DSession::FillTree may screw up, JB 2007 June


  DPixel  *aPixel;
  Int_t    st=0, colPhys=0, linPhys=0, stPhys=0;
  DR3      tPosition(0,0,0);
  DR3      tSize(0,0,0);
  Double_t u=0,v=0,w=0;
  Float_t  aRawvalue, aNoise, aPedestal;

  if(fDebugPlane) printf(" DPLane::Update Updating plane %d with readout %d status=%d analysisMode=%d mimosaType=%d noiseRun=%d\n", fPlaneNumber, fReadout, fStatus, fAnalysisMode, fMimosaType, fNoiseRun);


  //======================
  // Skip if readout is 0
  if( fReadout==0 ) {
    if( fDebugPlane) printf(" DPlane::Update skipping plane %d because readout=%d\n", fPlaneNumber, fReadout);
    return !planeReady;
  }

  //======================
  // Readout 1
  // - without zero suppression
  // - no need to re-order channel numbering wrt strip numbering
  // - no CDS required or CDS already performed in BoardReader class
  // - further analysis done with DStrip object
  // - does not allow to take into account external noise or gain file
  // Checked JB 2012/08/21
  else if ( fReadout==1 ){
    fPixelsN = fListOfPixels->size(); // update number of hit pixels
    if( fDebugPlane>1 ) printf(" DPlane::Update: Plane %d has %d pixels\n", fPlaneNumber, fPixelsN);
    for (Int_t tci = 0; tci < fPixelsN; tci++) { // loop over hit pixels
      aPixel = fListOfPixels->at(tci);
      st = aPixel->GetPixelIndex();
      aPixel->SetPixelLine( st / fStripsNu);
      aPixel->SetPixelColumn( st % fStripsNu);

      if( fMimosaType==60 && 60600<=fSession->GetRunNumber() && fSession->GetRunNumber()<60641) { // row 0-3 -> 4-7 and 4-7 -> 0-3
        aPixel->SetPixelLine( (st/fStripsNu+4)%8 );
        st = ((st/fStripsNu+4)%8)*fStripsNu + st%fStripsNu;
      }

      GetStrip(st)->SetPixelIndex( tci);
      GetStrip(st)->SetRawValue( aPixel->GetRawValue() );
      //GetStrip(st)->SetRawValue(fRawData[st]);  // old way

      if ((fMimosaType==33 && tci<27)
          ) {
        aPixel->SetRawValue(0.);
        aPixel->SetPulseHeight(0.);
        GetStrip(st)->SetRawValue( 0.);
      }

      if( fDebugPlane>3 ) printf("DPlane:Update  pixel %d with index %d, at (line,col)=(%d,%d) and raw value %.1f or %.1f\n", tci, st, st / fStripsNu, st & fStripsNu, aPixel->GetRawValue(), GetStrip(st)->GetRawValue());

    }
  } //end readout==1

  //======================
  // Readout 2
  // - without zero suppression
  // - re-ordering of the channel numbering wrt strip numbering
  // - no CDS required or CDS already performed in BoardReader class
  // - allows to take into account external noise or gain file
  // - further analysis done with DPixel or DStrip objects
  // JB 2013/08/14
  else if ( fReadout==2 ){
    fPixelsN = fListOfPixels->size(); // update number of hit pixels
    if( fDebugPlane>1 ) printf(" DPlane::Update: Plane %d has %d pixels\n", fPlaneNumber, fPixelsN);
    for (Int_t tci = 0; tci < fPixelsN; tci++) { // loop over hit pixels
      aPixel = fListOfPixels->at(tci);
      st = aPixel->GetPixelIndex();
      linPhys = st / fStripsNu;
      colPhys = st % fStripsNu;
      stPhys = colPhys + linPhys*fStripsNu;
      if( fDebugPlane>2 && stPhys>fStripsN-1) printf(" Pb1 with st %d, (line,col)=(%d,%d), physIndex %d, value %f\n", st, linPhys, colPhys, stPhys, aPixel->GetPulseHeight());
      ComputeStripPosition( colPhys, linPhys, u, v, w);
      tPosition.SetValue(u,v,w); //

      // update the pixel
      aPixel->SetSize( (*fPitch));
      aPixel->SetPixelLine( linPhys);
      aPixel->SetPixelColumn( colPhys);
      aPixel->SetPosition( tPosition);

      if(180000 <= fSession->GetRunNumber() && fSession->GetRunNumber() <= 180500 && linPhys==0 && (colPhys==2 || colPhys==3) ) {
        aPixel->SetRawValue(1.0e-6);
        aPixel->SetPulseHeight(1.0e-6);
      }

      if ( fPixelGainRun ) {
        SetPixelGainFromHisto( colPhys, linPhys, aPixel);
      }

      if( fNoiseRun) {
        SetPedandNoiseFromHisto( colPhys, linPhys, aPixel);
      }

      // update the strip
      GetStrip(stPhys)->SetPixelIndex( tci);
      GetStrip(stPhys)->SetRawValue( aPixel->GetRawValue() );

      if( fDebugPlane>3 ) printf("DPlane:Update  pixel %d with index %d updated at (line,col)=(%d,%d) channel=%d and rawvalue %f\n", tci, st, linPhys, colPhys, stPhys, aPixel->GetRawValue());

    }
  } //end readout==2

  //======================
  // Readout 3
  // - same as Readout==1 BUT REVERESE POLARITY
  //  JB, MK 2018/05/04
  else if ( fReadout==3 ){
    fPixelsN = fListOfPixels->size(); // update number of hit pixels
    if( fDebugPlane>1 ) printf(" DPlane::Update: Plane %d has %d pixels\n", fPlaneNumber, fPixelsN);
    for (Int_t tci = 0; tci < fPixelsN; tci++) { // loop over hit pixels
      aPixel = fListOfPixels->at(tci);
      st = aPixel->GetPixelIndex();
      aPixel->SetPixelLine( st / fStripsNu);
      aPixel->SetPixelColumn( st % fStripsNu);

      // reverse polarity here
      aPixel->SetRawValue( -aPixel->GetRawValue());

      GetStrip(st)->SetPixelIndex( tci);
      GetStrip(st)->SetRawValue( aPixel->GetRawValue() );

      if( fDebugPlane>3 ) printf("DPlane:Update  pixel %d with index %d, at (line,col)=(%d,%d) and raw value %.1f or %.1f\n", tci, st, st / fStripsNu, st & fStripsNu, aPixel->GetRawValue(), GetStrip(st)->GetRawValue());

    }
  } //end readout==3

  //======================
  // Readout 102
  // - with zero suppression
  // - re-ordering of the channel numbering wrt strip numbering
  // - no CDS required or CDS already performed in BoardReader class
  // - allows to take into account external noise file
  // - further analysis done with DPixel object
  // JB 2013/08/18
  else if ( fReadout==102 ){

    if(fMimosaType == 225) {
      //Subtracting the fist 8 columns from the pixel list. Only applies to Mi22THR
      //temporal list with hit pixels excluding hot-pixels
     std::vector<DPixel*> Temp_list;
      Temp_list.clear();
      for(int iPix=0;iPix<int(fListOfPixels->size());iPix++) {
        aPixel = fListOfPixels->at(iPix);
        if(aPixel->GetPixelIndex() % fStripsNu >= 8) Temp_list.push_back(aPixel);
        else                                         delete aPixel;
      }
      fListOfPixels->clear();
      for(int iPix=0;iPix<int(Temp_list.size());iPix++) {
        fListOfPixels->push_back(Temp_list[iPix]);
      }
      Temp_list.clear();
    }

    fPixelsN = fListOfPixels->size(); // update number of hit pixels
    for (Int_t tci = 0; tci < fPixelsN; tci++) { // loop over hit pixels
      aPixel = fListOfPixels->at(tci);
      st = aPixel->GetPixelIndex();
      linPhys = st / fStripsNu;
      colPhys = st % fStripsNu;
      stPhys = colPhys + linPhys*fStripsNu;
      //if( fDebugPlane>2 && stPhys>fStripsN-1) printf(" Pb1 with st %d, (line,col)=(%d,%d), physIndex %d, value %f\n", st, linPhys, colPhys, stPhys, aPixel->GetPulseHeight());
      ComputeStripPosition( colPhys, linPhys, u, v, w);
      tPosition.SetValue(u,v,w); //

      if(fMimosaType == 226 && !(124<=linPhys && linPhys <= 248) ) {
        aPixel->SetRawValue(0);
        aPixel->SetPulseHeight(0);
        if( colPhys>63 ) {
          colPhys -= 64;
        } else {
          colPhys += 64;
        }
      }

      // update the pixel
      aPixel->SetSize( (*fPitch));
      aPixel->SetPixelLine( linPhys);
      aPixel->SetPixelColumn( colPhys);
      aPixel->SetPixelIndex( stPhys); // JB 2014/08/26
      aPixel->SetPosition( tPosition);

      //if(fMimosaType == 225) {
      //if(colPhys >= 0 && colPhys <= 7) {
      //  aPixel->SetRawValue(-1.0);
      //  aPixel->SetPulseHeight(1.0);
      //}
      //}



      if( fNoiseRun) {
        SetPedandNoiseFromHisto( colPhys, linPhys, aPixel);
        //aPixel->SetPixelIndex(aPixel->GetPixelLine()*fStripsNu + aPixel->GetPixelColumn());
      }

      if( fDebugPlane>3 ) printf("DPlane:Update  pixel %d with index %d updated at (line,col)=(%d,%d) channel=%d, position (%.1f, %.1f, %.1f), size (%.1f, %.1f, %.1f) raw value %.1f  pulse height %.1f  S/N %.2f   ped=%.1f noise=%.1f\n", tci, st, linPhys, colPhys, stPhys, u, v, w, (*fPitch)(0), (*fPitch)(1), (*fPitch)(2), aPixel->GetRawValue(), aPixel->GetPulseHeight(), aPixel->GetPulseHeightToNoise(), aPixel->GetPedestal(), aPixel->GetNoise());
    } // end loop over hit pixels

    if( fPixelsN==0 ) {
      goForAnalysis = kFALSE;
      //cout <<"WARNING Plane " << fPlaneNumber << " has no hit pixels, no more analysis for it in event " << fAcq->GetEventNumber() << endl;
    }
  } //end readout==102


  //======================
  // Readout without zero suppression BUT recquiring re-ordering
  // of channels wrt strips
  // Readout adapted to Mimosa 5
  else if ( fReadout==5 ){
    // the plane data corresponds to 2 inputs of the daq module
    // so basically st = 512*...
    // we inverse all the channel indexes for the second half or input
    //   (that is complementing the column number to 511)
    // this is due to the readout of scheme of sub-matrices
    // which is symetric wrt the center of the chip
    Int_t nPix = 512; // actual number of pixels of submatrix
    fPixelsN = fListOfPixels->size(); // update number of hit pixels

    for (Int_t tci = 0; tci < fPixelsN; tci++) { // loop over hit pixels
      aPixel = fListOfPixels->at(tci);
      st = aPixel->GetPixelIndex();

      if( st >= 3*nPix*nPix ) { // submatrix A3
        linPhys = nPix-1 - st/nPix; // flip line 511->0 & 0->511
        colPhys = st % nPix + fStripsNu-nPix; // shift by nPix if fStripsNu=2xnPix
      }
      else if( st >= 2*nPix*nPix ) { // submatrix A2
        linPhys = nPix-1 -st/nPix + fStripsNv-nPix; // flip line 511->0 & 0->511 and shift by nPix if fStripsNv=2xnPix
        colPhys = st % nPix + fStripsNu-nPix; // shift by nPix if fStripsNu=2xnPix
      }
      else if( st >= nPix*nPix ) { // submatrix A1
        linPhys = nPix-1 - st/nPix + fStripsNv-nPix; // flip line 511->0 & 0->511 and shift by nPix if fStripsNv=2xnPix
        colPhys = st % nPix;
      }
      else { // submatrix A0
        linPhys = nPix-1 - st/nPix; // flip line 511->0 & 0->511
        colPhys = st % nPix;
      }
      stPhys = colPhys + linPhys*fStripsNu;
      if( fDebugPlane>2 && stPhys>fStripsN-1) printf(" Pb1 with st %d, (line,col)=(%d,%d), physIndex %d, value %f\n", st, linPhys, colPhys, stPhys, aPixel->GetPulseHeight());

      ComputeStripPosition( colPhys, linPhys, u, v, w); // JB 2012/11/21
      //      u = ((2*colPhys - fStripsNu + 1 ) * fc->GetPlanePar(fPlaneNumber).aPitch(0))/2 ;
      //      v = ((2*linPhys - fStripsNv + 1 ) * fc->GetPlanePar(fPlaneNumber).Pitch(1))/2 ;
      //      w = fc->GetPlanePar(fPlaneNumber).Pitch(2);
      tPosition.SetValue(u,v,w); //

      // update the pixel
      aPixel->SetSize( (*fPitch));
      aPixel->SetPixelLine( linPhys);
      aPixel->SetPixelColumn( colPhys);
      aPixel->SetPosition( tPosition);

      // update the strip
      GetStrip(stPhys)->SetPixelIndex( tci);
      GetStrip(stPhys)->SetRawValue( aPixel->GetPulseHeight());
      //GetStrip(stPhys)->UpdateSignal(); // done at call for analyze_basic, JB 2011/04/15

      if( fDebugPlane>3 ) printf("DPlane:Update  pixel %d with index %d updated at (line,col)=(%d,%d) channel=%d, value %f\n", tci, st, linPhys, colPhys, stPhys, aPixel->GetPulseHeight());
    } // end loop over hit pixels

    if( fPixelsN==0 ) {
      goForAnalysis = kFALSE;
      cout <<"WARNING Plane " << fPlaneNumber << " has no hit pixels, no more analysis for it in real event " << fAcq->GetEventNumber() << endl;
    }

  } //end if Readout==5


  //======================
  // Readout with zero-suppression using a list of hit pixels
  // MIMOSA5 type indexes, i.e. 4 submatrices from equal size
  else if ( fReadout==105 ){

    fPixelsN = fListOfPixels->size(); // update number of hit pixels
    Int_t nPix = 512; // size of one submatrix
    for (Int_t tci = 0; tci < fPixelsN; tci++) { // loop over hit pixels
      aPixel = fListOfPixels->at(tci);
      st = aPixel->GetPixelIndex();
      // special treatment for November 2009 PLUME test
      if(fSession->GetRunNumber()/100==205 && fPlaneNumber==6 ) { // NCS 07/11/09
        //cout << "fPlaneNumber=" << fPlaneNumber << endl ;
        st = st +1;
      }
      // compute line and column index from the index
      // test order modified, JB 2009/05/25
      if( st >= 3*nPix*nPix ) { // submatrix A3
        linPhys = nPix-1 - st/nPix; // flip line 511->0 & 0->511
        colPhys = st % nPix + fStripsNu-nPix; // shift by nPix if fStripsNu=2xnPix
      }
      else if( st >= 2*nPix*nPix ) { // submatrix A2
        linPhys = nPix-1 -st/nPix + fStripsNv-nPix; // flip line 511->0 & 0->511 and shift by nPix if fStripsNv=2xnPix
        colPhys = st % nPix + fStripsNu-nPix; // shift by nPix if fStripsNu=2xnPix
      }
      else if( st >= nPix*nPix ) { // submatrix A1
        linPhys = nPix-1 - st/nPix + fStripsNv-nPix; // flip line 511->0 & 0->511 and shift by nPix if fStripsNv=2xnPix
        colPhys = st % nPix;
      }
      else { // submatrix A0
        linPhys = nPix-1 - st/nPix; // flip line 511->0 & 0->511
        colPhys = st % nPix;
      }
      ComputeStripPosition( colPhys, linPhys, u, v, w); // JB 2012/11/21
      //      u = ((2*colPhys - fStripsNu + 1 ) * fc->GetPlanePar(fPlaneNumber).Pitch(0))/2 ;
      //      v = ((2*linPhys - fStripsNv + 1 ) * fc->GetPlanePar(fPlaneNumber).Pitch(1))/2 ;
      //      w = fc->GetPlanePar(fPlaneNumber).Pitch(2);
      tPosition.SetValue(u,v,w); //
      aPixel->SetSize( (*fPitch));
      aPixel->SetPixelLine( linPhys);
      aPixel->SetPixelColumn( colPhys);
      aPixel->SetPosition( tPosition);
      if( fNoiseRun) {
        SetPedandNoiseFromHisto( colPhys, linPhys, aPixel);
        aPixel->SetPixelIndex(aPixel->GetPixelLine()*fStripsNu + aPixel->GetPixelColumn());	// added MB/10/11/2010
        if( TMath::Abs(aPixel->GetPedestal()) == 1023 ) {
          aPixel->SetPulseHeight( 0.);
        }
      }
      if( fDebugPlane>3 ) printf("DPlane:Update  pixel %d with index %d updated at (line,col)=(%d,%d), position (%.1f, %.1f, %.1f), size (%.1f, %.1f, %.1f) raw value %.1f  pulse height %.1f  S/N %.2f   ped=%.1f noise=%.1f\n", tci, st, linPhys, colPhys, u, v, w, (*fPitch)(0), (*fPitch)(1), (*fPitch)(2), aPixel->GetRawValue(), aPixel->GetPulseHeight(), aPixel->GetPulseHeightToNoise(), aPixel->GetPedestal(), aPixel->GetNoise());
    } // end loop over hit pixels
    if( fPixelsN==0 ) {
      goForAnalysis = kFALSE;
      //cout <<"WARNING Plane " << fPlaneNumber << " has no hit pixels, no more analysis for it in event " << fAcq->GetEventNumber() << endl;
    }
  } //end readout==105


  //======================
  // Readout without zero suppression BUT recquiring re-ordering
  // of channels wrt strips
  // Readout adapted to Mimosa 18
  else if ( fReadout==18 ){
    // the plane data corresponds to 2 inputs of the daq module
    // so basically st = 256*...
    // we inverse all the channel indexes for the second half or input
    //   (that is complementing the column number to 255)
    // this is due to the readout of scheme of sub-matrices
    // which is symetric wrt the center of the chip
    Int_t nPix = 256; // actual number of pixels of submatrix
    fPixelsN = fListOfPixels->size(); // update number of hit pixels

    for (Int_t tci = 0; tci < fPixelsN; tci++) { // loop over hit pixels
      aPixel = fListOfPixels->at(tci);
      st = aPixel->GetPixelIndex();

      if( st >= 3*nPix*nPix ) { // submatrix A3
        linPhys = st / nPix - 3*nPix; // shift from 768->1023 to 0->256
        colPhys = (nPix-1) - st % nPix + nPix; // flip from 0->255 to 255->0 and shift by 256
      }
      else if( st >= 2*nPix*nPix ) { // submatrix A2
        linPhys = (4*nPix-1) - st / nPix; // flip from 512->767 to 511->256
        colPhys = (nPix-1) - st % nPix + nPix; // flip from 0->255 to 255->0 and shift by 256
      }
      else if( st >= nPix*nPix ) { // submatrix A1
        linPhys = 3*nPix - (st / nPix)-1; // flip from 256->511 to 511->256
        colPhys = st % nPix;
      }
      else { // submatrix A0
        linPhys = st / nPix;
        colPhys = st % nPix;
      }
      stPhys = colPhys + linPhys*fStripsNu;
      if( fDebugPlane>2 && stPhys>fStripsN-1) printf(" Pb1 with st %d, (line,col)=(%d,%d), physIndex %d, value %f\n", st, linPhys, colPhys, stPhys, aPixel->GetPulseHeight());

      ComputeStripPosition( colPhys, linPhys, u, v, w); // JB 2012/11/21
      //      u = ((2*colPhys - fStripsNu + 1 ) * fc->GetPlanePar(fPlaneNumber).Pitch(0))/2 ;
      //      v = ((2*linPhys - fStripsNv + 1 ) * fc->GetPlanePar(fPlaneNumber).Pitch(1))/2 ;
      //      w = fc->GetPlanePar(fPlaneNumber).Pitch(2);
      tPosition.SetValue(u,v,w); //

      // update the pixel
      aPixel->SetSize( (*fPitch));
      aPixel->SetPixelLine( linPhys);
      aPixel->SetPixelColumn( colPhys);
      aPixel->SetPosition( tPosition);

      if(180000 <= fSession->GetRunNumber() && fSession->GetRunNumber() <= 180500 && linPhys==0 && (colPhys==2 || colPhys==3) ) {
        aPixel->SetRawValue(1.0e-6);
        aPixel->SetPulseHeight(1.0e-6);
      }

      // update the strip
      GetStrip(stPhys)->SetPixelIndex( tci);
      GetStrip(stPhys)->SetRawValue( aPixel->GetPulseHeight()); // put minus sign if needed here
      //GetStrip(stPhys)->UpdateSignal(); // done at call for analyze_basic, JB 2011/04/15

      if( fDebugPlane>4 ) printf("DPlane:Update  pixel %d with index %d updated at (line,col)=(%d,%d) channel=%d, value %f\n", tci, st, linPhys, colPhys, stPhys, aPixel->GetPulseHeight());
    } // end loop over hit pixels

    if( fPixelsN==0 ) {
      goForAnalysis = kFALSE;
      cout <<"WARNING Plane " << fPlaneNumber << " has no hit pixels, no more analysis for it in real event " << fAcq->GetEventNumber() << endl;
    }

  } //end if Readout==18


  //======================
  // Readout with zero-suppression using a list of hit pixels
  // MIMOSA18 type indexes, i.e. 4 submatrices from equal size
  else if ( fReadout==118 ){

    fPixelsN = fListOfPixels->size(); // update number of hit pixels
    Int_t nPix = 256; // size of one submatrix, JB 2009/05/25
    for (Int_t tci = 0; tci < fPixelsN; tci++) { // loop over hit pixels
      aPixel = fListOfPixels->at(tci);
      st = aPixel->GetPixelIndex();
      // special treatment for November 2009 PLUME test
      if(fSession->GetRunNumber()/100==205 && fPlaneNumber==6 ) { // NCS 07/11/09
        //cout << "fPlaneNumber=" << fPlaneNumber << endl ;
        st = st +1;
      }
      // compute line and column index from the index
      // test order modified, JB 2009/05/25
      if( st >= 4*nPix*nPix ) { // submatrix imaginary !
        cout << "WARNING DPlane: M-18 readout 118: pixel index crazy = " << st << endl;
      }
      else if( st >= 3*nPix*nPix ) { // submatrix A3
        linPhys = st / nPix - 3*nPix; // shift from 768->1023 to 0->256
        colPhys = (nPix-1) - st % nPix + nPix; // flip from 0->255 to 255->0 and shift by 256
      }
      else if( st >= 2*nPix*nPix ) { // submatrix A2
        linPhys = (4*nPix-1) - st / nPix; // flip from 512->767 to 511->256
        colPhys = (nPix-1) - st % nPix + nPix; // flip from 0->255 to 255->0 and shift by 256
      }
      else if( st >= nPix*nPix ) { // submatrix A1
        linPhys = 3*nPix - (st / nPix)-1; // flip from 256->511 to 511->256
        colPhys = st % nPix;
      }
      else { // submatrix A0
        linPhys = st / nPix;
        colPhys = st % nPix;
      }

      // Specific since Oct 2011
      if( 0<=colPhys && colPhys<28 ) {
        colPhys +=228;
      }
      else if( 28<=colPhys && colPhys<256 ) {
        colPhys -=28;
      }
      else if( 256<=colPhys && colPhys<512-28 ) {
        colPhys +=28;
      }
      else if( 512-28<=colPhys && colPhys<512 ) {
        colPhys -=228;
      }


      ComputeStripPosition( colPhys, linPhys, u, v, w); // JB 2012/11/21
      //      u = ((2*colPhys - fStripsNu + 1 ) * fc->GetPlanePar(fPlaneNumber).Pitch(0))/2 ;
      //      v = ((2*linPhys - fStripsNv + 1 ) * fc->GetPlanePar(fPlaneNumber).Pitch(1))/2 ;
      //      w = fc->GetPlanePar(fPlaneNumber).Pitch(2);
      tPosition.SetValue(u,v,w); //
      aPixel->SetSize( (*fPitch));
      aPixel->SetPixelLine( linPhys);
      aPixel->SetPixelColumn( colPhys);
      aPixel->SetPixelIndex( aPixel->GetPixelLine()*fStripsNu + aPixel->GetPixelColumn()); // JB 2014/08/26
      aPixel->SetPosition( tPosition);

      if(180000 <= fSession->GetRunNumber() && fSession->GetRunNumber() <= 180500 && linPhys==0 && (colPhys==2 || colPhys==3) ) {
        aPixel->SetRawValue(1.0e-6);
        aPixel->SetPulseHeight(1.0e-6);
      }

      if( fNoiseRun) {
        SetPedandNoiseFromHisto( colPhys, linPhys, aPixel);
        //aPixel->SetPixelIndex(aPixel->GetPixelLine()*fStripsNu + aPixel->GetPixelColumn());
        if( TMath::Abs(aPixel->GetPedestal()) == 1023 ) {
          aPixel->SetPulseHeight( 0.);
        }
      }

      // Specific for LNF beam test May 2014
      // useless since June 2014
      //      if( colPhys==367 && linPhys<257 &&  fSession->GetRunNumber()<100) {
      //        aPixel->SetPulseHeight( 0.);
      //      }

      if( fDebugPlane>3 ) printf("DPlane:Update  pixel %d with index %d updated at (line,col)=(%d,%d), position (%f, %f, %f), value %f\n", tci, st, linPhys, colPhys, u, v, w, aPixel->GetPulseHeight());
      //         if( fDebugPlane>3 ) printf("DPlane:Update  pixel %d with index %d updated at (line,col)=(%d,%d), position (%f, %f, %f), size (%f, %f, %f) value %f\n", tci, st, linPhys, colPhys, u, v, w, (*fPitch)(0), (*fPitch)(1), (*fPitch)(2), aPixel->GetPulseHeight());
    } // end loop over hit pixels
    if( fPixelsN==0 ) {
      goForAnalysis = kFALSE;
      //cout <<"WARNING Plane " << fPlaneNumber << " has no hit pixels, no more analysis for it in event " << fAcq->GetEventNumber() << endl;
    }
  } //end readout==118


  //======================
  // Readout using a list of hit pixels
  // MIMOSA20 type indexes, i.e. 5 submatrices 320 ligns * 66 columns
  // with the 2 first pixels out of 66 being markers and not used
  // NCS 27/09/09
  else if ( fReadout==20 )
  {
    fPixelsN = fListOfPixels->size(); // update number of hit pixels
    Int_t nPixPerBloc = 66; // size of one submatrix, in Nb of Columns NCS 27/09/09
    Int_t nPixPerLign = 330; // size of one submatrix in Nb of columns, includes the 10 marker pixels
    Int_t bloc=0 ; // to know in which submatrix we are (between 0 and 4)
    Int_t colInsideBloc=0; // Column Number inside the submatrix
    Int_t indexBloc=0; // Starting index of the submatrix
    Int_t NbOfMarkers =0;

    if(fDebugPlane>8) cout << "DPlane::Update, readout = "<< fReadout <<", fPixelsN=" << fPixelsN << endl ;
    for (Int_t tci = 0; tci < fPixelsN; tci++) // loop over hit pixels
    {
      aPixel = fListOfPixels->at(tci);
      st = aPixel->GetPixelIndex(); // TNT pixel index NCS 01/10/09
      // ----- Organisation of indexes in physical Lines and columns ---- //
      if(st<105600) // pixel TNT numbering ie with the markers submatrix A1
      {
        // Determination of the lign in the matrix
        linPhys=st/nPixPerLign; // entire part of the division
        // For chip in vertical position Height 640 width 320
        //++colPhys=st/nPixPerLign; // entire part of the division
        // Determination of the submatrix  number (between 0 and 4) = rest of the division by 5 gives the number of the submatrix
        bloc=st%5;
        // Determination of the column number inside a submatrix
        colInsideBloc=(st-linPhys*nPixPerLign-bloc)/5 - 2 ; // shift of 2 because of the markers
        // For chip in vertical position Height 640 width 320
        //++colInsideBloc=(st-colPhys*nPixPerLign-bloc)/5 - 2 ; //=> LineInsideBloc en fait puisque vertical
        if(colInsideBloc<0)// a negative colInsideBloc indicates a marker
        {
          cout << "fPixelsN =" << fPixelsN << "** tci marker=" << tci << "** st=" << st << endl ;
          //tci++ ;
          NbOfMarkers ++;
          //if(tci<fPixelsN) goto WasAMarker20;
          continue;
          //else goto WasAMarkerEnd20;
        }

        // Shift index with respect to the beginning of the global matrix
        indexBloc= bloc*(nPixPerBloc-2);
        // Column number inside the general matrix
        colPhys = indexBloc+colInsideBloc;
        // For chip in vertical position Height 640 width 320
        //++linPhys = indexBloc+colInsideBloc;

      }
      else if(st>105599)// pixel TNT numbering, submatrix A0
      {
        // Determination of the lign in the matrix
        //linPhys=st/nPixPerLign; // entire part of the division
        // For chip in vertical position Height 640 width 320
        colPhys=(st-105600)/nPixPerLign; // entire part of the division
        // Determination of the submatrix  number (between 0 and 4) = rest of the division by 5 gives the number of the submatrix
        bloc=(st-105600)%5;
        // Determination of the column number inside a submatrix
        //colInsideBloc=(st-105600-linPhys*nPixPerLign-bloc)/5 - 2 ; // shift of 2 because of the markers
        // For chip in vertical position Height 640 width 320
        colInsideBloc=(st-105600-colPhys*nPixPerLign-bloc)/5 - 2 ;
        if(colInsideBloc<0)// a negative colInsideBloc indicates a marker
        {
          cout << "fPixelsN =" << fPixelsN << "** tci marker=" << tci << "** st=" << st << endl ;
          //tci++;
          NbOfMarkers ++;
          //if(tci<fPixelsN) goto WasAMarker;
          continue;
          //else goto WasAMarkerEnd;

        }

        // Shift index with respect to the beginning of the global matrix
        indexBloc= (bloc+5)*(nPixPerBloc-2); // adding matrix A1 columns (5 blocs)
        // Column number inside the general matrix

        //colPhys = indexBloc+colInsideBloc;
        // For chip in vertical position Height 640 width 320
        linPhys = indexBloc+colInsideBloc;

        //cout << "NCS colPhys=" << colPhys << " linPhys=" << linPhys << endl ;


      } // end pixel TNT numbering > 105600 (after shift), matrix A0


      // physical index start
      stPhys = colPhys + linPhys*fStripsNu;
      aPixel->SetPixelLine( linPhys);
      aPixel->SetPixelColumn( colPhys);
      if (fDebugPlane>8) cout << "DPlane::Update() EvtNumber=" << fSession->GetCurrentEventNumber()<< "** Plane=" << fSession->GetPlaneNumber() << "** tci= " << tci << "** st= " << st << "** colPhys=" << colPhys << "**linPhys=" << linPhys << "** stPhys=" << stPhys << "** value=" << aPixel->GetPulseHeight()<<endl ;

      // update the strip
      GetStrip(stPhys)->SetPixelIndex( tci);
      GetStrip(stPhys)->SetRawValue( aPixel->GetPulseHeight());
      //cout <<  "GetStrip(stPhys)->SetRawValue( aPixel->GetPulseHeight())=" << aPixel->GetPulseHeight()<< endl ;
      //GetStrip(stPhys)->UpdateSignal();  // done at call for analyze_basic, JB 2011/04/15 // Remark : NCS 05/10/09 update only for signals


    } // end loop over hit pixels
    if( fPixelsN==0 )
    {
      goForAnalysis = kFALSE;
      cout <<"WARNING Plane " << fPlaneNumber << " has no hit pixels, no more analysis for it in event " << fAcq->GetEventNumber() << endl;
    }


  } //end readout==20, fin NCS 27/09/09 Mimosa20 readout

  //======================
  // Readout with zero-suppression using a list of hit pixels
  // MIMOSA20 type indexes
  else if ( fReadout==120 )
  {
    //if(fPlaneNumber==1 || fPlaneNumber==3)
    //cout << "M20 DPlane.cxx st=" << st << " fPlaneNumber=" << fPlaneNumber <<", evtnumber = " << fSession->GetCurrentEventNumber()<< endl;
    fPixelsN = fListOfPixels->size(); // update number of hit pixels
    Int_t nPixPerBloc = 66; // size of one submatrix, in Nb of Columns NCS 27/09/09
    Int_t nPixPerLign = 330; // size of one submatrix in Nb of columns, includes the 10 marker pixels
    Int_t bloc=0 ; // to know in which submatrix we are (between 0 and 4)
    Int_t colInsideBloc=0; // Column Number inside the submatrix
    Int_t indexBloc=0; // Starting index of the submatrix
    Int_t NbOfMarkers =0;

    if(fDebugPlane>8) cout << "DPlane::Update, readout = "<< fReadout <<", fPixelsN=" << fPixelsN << endl ;
    for (Int_t tci = 0; tci < fPixelsN; tci++) // loop over hit pixels
    {

      aPixel = fListOfPixels->at(tci);
      st = aPixel->GetPixelIndex(); // TNT pixel index NCS 01/10/09
      //cout << endl  << "M20 st=" << st << " fPlaneNumber=" << fPlaneNumber <<", evtnumber = " << fSession->GetCurrentEventNumber()<< endl;

      //if(st<11){ cout << "st<11=" << st << endl ; getchar();}
      //cout << " st = " << st << endl ;
      //cout << "aPixel->GetPulseHeight()=" << aPixel->GetPulseHeight()<< endl ;
      //if(fSession->GetPlaneNumber()==0){h1->Fill(st);h5->Fill(aPixel->GetPulseHeight());}
      //if(aPixel->GetPulseHeight()>3300) {cout << "st=" << st << " GetPulseHeight=" << aPixel->GetPulseHeight()<< endl ;}
      //if(st==65012){ cout << "marker!"<< endl ; getchar();}

      // ----- Organisation of indexes in physical Lines and columns ---- //
      if(st<105600) // pixel TNT numbering ie with the markers submatrix A1
      {
        //cout << "NCS DPlane.cxx st<105600 : st=" << st << endl ;
        // Determination of the lign in the matrix
        //linPhys=st/nPixPerLign; // entire part of the division
        // For chip in vertical position Height 640 width 320
        colPhys=st/nPixPerLign; // entire part of the division
        //cout << "colPhys=st/330=" << colPhys << endl ;
        // Determination of the submatrix  number (between 0 and 4) = rest of the division by 5 gives the number of the submatrix
        bloc=st%5;
        //cout << "bloc=st%5=" << bloc << endl ;

        // Determination of the column number inside a submatrix
        //colInsideBloc=(st-linPhys*nPixPerLign-bloc)/5 - 2 ; // shift of 2 because of the 2 markers of each bloc
        // For chip in vertical position Height 640 width 320
        colInsideBloc=(st-colPhys*nPixPerLign-bloc)/5 - 2 ; //=> LineInsideBloc en fait puisque vertical
        //cout << "colInsideBloc=(st-colPhys*330-bloc)/5 - 2 =" << colInsideBloc<< endl ;
        if(colInsideBloc<0)// a negative colInsideBloc indicates a marker
        {
          //if(st!=0){cout << "fPixelsN =" << fPixelsN << "** tci marker=" << tci << "** st=" << st << endl ;getchar();}
          //if(st==65012 ||st==79861 ){ cout << "marker!"<< endl ; return 0;}
          //tci++ ;
          NbOfMarkers ++;
          //if(tci<fPixelsN)
          //continue;
          //else goto WasAMarkerEnd;
        }
        else if(colInsideBloc>=0)
        {
          // Shift index with respect to the beginning of the global matrix
          indexBloc= bloc*(nPixPerBloc-2);
          //cout << "indexBloc= bloc*(nPixPerBloc-2)=" << indexBloc <<endl ;
          // Column number inside the general matrix
          //colPhys = indexBloc+colInsideBloc;
          // For chip in vertical position Height 640 width 320
          linPhys = indexBloc+colInsideBloc;
          //cout << "linPhys = indexBloc+colInsideBloc=" << linPhys << endl ;
          //******************************************************************************
          ComputeStripPosition( colPhys, linPhys, u, v, w); // JB 2012/11/21
          //      u = ((2*colPhys - fStripsNu + 1 ) * fc->GetPlanePar(fPlaneNumber).Pitch(0))/2 ;
          //		  v = ((2*linPhys - fStripsNv + 1 ) * fc->GetPlanePar(fPlaneNumber).Pitch(1))/2 ;
          //		  w = fc->GetPlanePar(fPlaneNumber).Pitch(2);
          u=-u;// NCS 101209 OK for PLUME VERTICAL
          v=-v; // NCS 101209 OK for PLUME VERTICAL
          //temp=u; // NCS 101209 for PLUME HORIZONTAL
          //u=v;
          //v=temp;

          //cout << "tci=" << tci << " colPhys=" << colPhys << "  u=" << u << endl ;
          //cout << "linPhys=" << linPhys << "  v=" << v << endl ;
          //cout << "(u,v)=("<< u <<","<< v<<")" << " (line,col)=(" << linPhys << "," << colPhys <<")" << endl ;

          //if(TMath::Abs(u)<3000 && v > (-7500) && v < (-500)  )
          {
            tPosition.SetValue(u,v,w); //
            aPixel->SetSize( (*fPitch));
            aPixel->SetPixelLine( linPhys);
            aPixel->SetPixelColumn( colPhys);
            aPixel->SetPosition( tPosition);
          }
          //else{cout << "je suis sur la matrice du haut "<< endl ;}
          //******************************************************************************
        }

      }
      else if(st>105599)// pixel TNT numbering, submatrix A0
      {
        //cout << "NCS DPlane.cxx st>105599 : st=" << st << endl ;
        // Determination of the lign in the matrix
        //linPhys=(st-105600)/nPixPerLign; // entire part of the division
        // For chip in vertical position Height 640 width 320
        //cout << "NCS DPlane.cxx st=+3200 : st=" << st << endl ;
        colPhys=(st-105600)/nPixPerLign; // entire part of the division
        //cout << "colPhys=(st-105600)/330=" << colPhys << endl ;

        // Determination of the submatrix  number (between 0 and 4) = rest of the division by 5 gives the number of the submatrix
        bloc=(st-105600)%5;
        //cout << "bloc=(st-105600)%5=" << bloc <<endl ;
        // Determination of the column number inside a submatrix
        //colInsideBloc=(st-105600-linPhys*nPixPerLign-bloc)/5 - 2 ; // shift of 2 because of the markers
        // For chip in vertical position Height 640 width 320
        colInsideBloc=(st-105600-colPhys*nPixPerLign-bloc)/5 - 2 ;
        //cout << "colInsideBloc=(st-105600-colPhys*330-bloc)/5 - 2=" << colInsideBloc << endl ;
        if(colInsideBloc<0)// a negative colInsideBloc indicates a marker
        {
          //if(st!=105600) {cout << "fPixelsN =" << fPixelsN << "** tci marker=" << tci << "** st=" << st << endl ; getchar();}
          //tci++;
          //if(st==65012 ||st==79861 ){ cout << "marker!"<< endl ;return 0;}
          NbOfMarkers ++;
          //if(tci<fPixelsN)
          //continue;
          //else goto WasAMarkerEnd;

        }
        else if(colInsideBloc>=0)
        {
          // Shift index with respect to the beginning of the global matrix
          indexBloc= (bloc+5)*(nPixPerBloc-2); // adding matrix A1 columns (5 blocs)
          //cout << "indexBloc= (bloc+5)*(nPixPerBloc-2)=" << indexBloc << endl ;

          // Column number inside the general matrix
          //colPhys = indexBloc+colInsideBloc;
          // For chip in vertical position Height 640 width 320
          linPhys = indexBloc+colInsideBloc;
          //cout << "linPhys = indexBloc+colInsideBloc=" << linPhys << endl ;
          //******************************************************************************
          ComputeStripPosition( colPhys, linPhys, u, v, w); // JB 2012/11/21
          //      u = ((2*colPhys - fStripsNu + 1 ) * fc->GetPlanePar(fPlaneNumber).Pitch(0))/2 ;
          //		  v = ((2*linPhys - fStripsNv + 1 ) * fc->GetPlanePar(fPlaneNumber).Pitch(1))/2 ;
          //		  w = fc->GetPlanePar(fPlaneNumber).Pitch(2);
          u=-u;// NCS 101209 OK for PLUME VERTICAL
          v=-v;// NCS 101209 OK for PLUME VERTICAL
          //temp=u; // NCS 101209 for PLUME HORIZONTAL
          //u=v;
          //v=temp;

          //cout << "tci=" << tci << " colPhys=" << colPhys << "  u=" << u << endl ;
          //cout << "linPhys=" << linPhys << "  v=" << v << endl ;
          //cout << "(u,v)=("<< u <<","<< v<<")" << " (line,col)=(" << linPhys << "," << colPhys <<")" << endl ;


          //if(TMath::Abs(u)<3000 && v > (-7500) && v < (-500)  )
          {
            tPosition.SetValue(u,v,w); //
            aPixel->SetSize( (*fPitch));
            aPixel->SetPixelLine( linPhys);
            aPixel->SetPixelColumn( colPhys);
            aPixel->SetPosition( tPosition);
          }
          //else{cout << "je suis sur la matrice du haut "<< endl ;}
          //******************************************************************************
        }
      } // end pixel TNT numbering > 105600 (after shift), matrix A0

      if( fDebugPlane>8 ) printf("DPlane::Update()  pixel %d with index %d updated at (line,col)=(%d,%d), position (%f, %f, %f), size (%f, %f, %f) value %f\n", tci, st, linPhys, colPhys, u, v, w, (*fPitch)(0), (*fPitch)(1), (*fPitch)(2), aPixel->GetPulseHeight());

    } // end loop over hit pixels
    if( fPixelsN==0 )
    {
      goForAnalysis = kFALSE;
      cout <<"WARNING Plane " << fPlaneNumber << " has no hit pixels, no more analysis for it in event " << fAcq->GetEventNumber() << endl;
    }
  } //end readout==120, fin NCS 30/10/09 Mimosa20 readout


  //======================
  // Readout :
  // - without zero suppression
  // - recquiring re-ordering of channels wrt strips
  // - with binary output
  // Readout adapted to Mimosa 22 digital part
  // JB 2013/08/14
  else if ( fReadout==22 ){
    // the plane data corresponds to 2 inputs of the daq module
    // so basically st = 256*...
    // we inverse all the channel indexes for the second half or input
    //   (that is complementing the column number to 255)
    // this is due to the readout of scheme of sub-matrices
    // which is symetric wrt the center of the chip
    fPixelsN = fListOfPixels->size(); // update number of hit pixels

    for (Int_t tci = 0; tci < fPixelsN; tci++) { // loop over hit pixels
      aPixel = fListOfPixels->at(tci);
      st = aPixel->GetPixelIndex();
      if(fMimosaType != 225) {
        linPhys = st / fStripsNu;
        colPhys = st % fStripsNu;
      }
      else {
        linPhys = st / fStripsNu;
        if((st / (fStripsNu/2)) % 2) colPhys = 2*(st % (fStripsNu/2)) + 1;
        else                         colPhys = 2*(st % (fStripsNu/2));
      }
      stPhys = colPhys + linPhys*fStripsNu;
      if( fDebugPlane>2 && stPhys>fStripsN-1) printf(" Pb1 with st %d, (line,col)=(%d,%d), physIndex %d, value %f\n", st, linPhys, colPhys, stPhys, aPixel->GetPulseHeight());
      ComputeStripPosition( colPhys, linPhys, u, v, w); // JB 2012/11/21
      tPosition.SetValue(u,v,w); //

      // update the pixel
      aPixel->SetSize( (*fPitch));
      aPixel->SetPixelLine( linPhys);
      aPixel->SetPixelColumn( colPhys);
      aPixel->SetPosition( tPosition);

      // update the strip
      GetStrip(stPhys)->SetPixelIndex( tci); // JB 2013/08/20
      GetStrip(stPhys)->SetRawValue( aPixel->GetPulseHeight());
      //GetStrip(stPhys)->UpdateSignal(); // done at call for analyze_basic, JB 2011/04/15

      // Trick to consider only one part of MIMOSA 22
      if (
          (22700<=fSession->GetRunNumber() && fSession->GetRunNumber()<22800 && colPhys>=320) // 22AHR: S1-S10 col<320, S11-S16 col>=320
          ||
          (22800<=fSession->GetRunNumber() && fSession->GetRunNumber()<22900 && colPhys>96) // 22AHR: S1-S4 col:96/192/256
          ) {
        aPixel->SetRawValue(0);
        aPixel->SetPulseHeight(0);
        GetStrip(stPhys)->SetRawValue(0);
      }


      if( fDebugPlane>3 ) printf("DPlane:Update  pixel %d with index %d updated at (line,col)=(%d,%d) channel=%d, value %f\n", tci, st, linPhys, colPhys, stPhys, aPixel->GetPulseHeight());
    } // end loop over hit pixels

    if( fPixelsN==0 ) {
      goForAnalysis = kFALSE;
      cout <<"WARNING Plane " << fPlaneNumber << " has no hit pixels, no more analysis for it in real event " << fAcq->GetEventNumber() << endl;
    }

  } //end if Readout==22


  //======================
  // Readout without zero-suppression BUT recquiring re-ordering
  // MIMOSA25 type indexes
  // RDM 210509, JB 2009/05/25
  else if ( fReadout==25 ){

    TMimosa24_25Map *Mi=0;
    switch (fMimosaType) {
      case 20:
        Mi = new TMimosa24_25Map("Mi25B_20um"); //20um pitch
        break;
      case 30:
        Mi = new TMimosa24_25Map("Mi25B_30um"); //30um pitch
        break;
      case 40:
        Mi = new TMimosa24_25Map("Mi25B_40um"); //40um pitch
        break;
      default:
        cout << "MimosaType " << fMimosaType << " unknown for Mimosa25" << endl;
        Fatal("DPlane:Update", "==> CHANGE CONFIG FILE, I AM STOPPING! <==");
    };
    fPixelsN = fListOfPixels->size(); // update number of hit pixels
    Int_t nPix = TMath::Max( fStripsNu/2, fStripsNv*2); // size of submatrix is the smallest

    for (Int_t tci = 0; tci < fPixelsN; tci++) { // loop over hit pixels
      aPixel = fListOfPixels->at(tci);
      st = aPixel->GetPixelIndex();
      // compute line and column index from the index
      switch (fMimosaType) {
        case 20:
          linPhys = Mi->get_submatrix(st / nPix,st % nPix)*32+Mi->get_row(st / nPix,st % nPix);
          break;
        case 30:
          if (Mi->get_submatrix(st / nPix,st % nPix)==0)
            linPhys = Mi->get_row(st / nPix,st % nPix);
          else if (Mi->get_submatrix(st / nPix,st % nPix)==1)
            linPhys = 21 + Mi->get_row(st / nPix,st % nPix);
          else if (Mi->get_submatrix(st / nPix,st % nPix)==2)
            linPhys = 43 + Mi->get_row(st / nPix,st % nPix);
          break;
        case 40:
          linPhys = Mi->get_submatrix(st / nPix,st % nPix)*16+Mi->get_row(st / nPix,st % nPix);
          break;
      };

      colPhys = Mi->get_column(st / nPix,st % nPix);
      // physical index
      stPhys = colPhys + linPhys*fStripsNu;

      aPixel->SetPixelLine( linPhys); // YV, 2009/06/05
      aPixel->SetPixelColumn( colPhys); // YV, 2009/06/05

      // update the strip
      GetStrip(stPhys)->SetPixelIndex( tci);
      GetStrip(stPhys)->SetRawValue( aPixel->GetPulseHeight());
      //GetStrip(stPhys)->UpdateSignal(); // done at call for analyze_basic, JB 2011/04/15
      if( fDebugPlane>3 ) printf("DPlane:Update  pixel %d with index %d updated at (line,col)=(%d,%d) channel=%d, value %f\n", tci, st, linPhys, colPhys, stPhys, aPixel->GetPulseHeight());
    } // end loop over hit pixels
    if( fPixelsN==0 ) {
      goForAnalysis = kFALSE;
      cout <<"WARNING Plane " << fPlaneNumber << " has no hit pixels, no more analysis for it in real event " << fAcq->GetEventNumber() << endl;
    }
    delete Mi;  //RDM290509
  } //end readout==25


  //======================
  // Readout with zero-suppression using a list of hit pixels
  // RDM 210509 largely modified!!!
  // MIMOSA25 type indexes
  else if ( fReadout==125 ){

    TMimosa24_25Map *Mi=0;
    switch (fMimosaType) {
      case 20:
        Mi = new TMimosa24_25Map("Mi25A_20um"); //20um pitch
        break;
      case 30:
        Mi = new TMimosa24_25Map("Mi25A_30um"); //30um pitch
        break;
      case 40:
        Mi = new TMimosa24_25Map("Mi25A_40um"); //40um pitch
        break;
      default:
        cout << "MimosaType " << fMimosaType << " unknown for Mimosa25" << endl;
        Fatal("DPlane:Update", "==> CHANGE CONFIG FILE, I AM STOPPING! <==");
    };
    fPixelsN = fListOfPixels->size(); // update number of hit pixels
    Int_t nPix = TMath::Max( fStripsNu/2, fStripsNv*2); // size of submatrix is the smallest

    for (Int_t tci = 0; tci < fPixelsN; tci++) { // loop over hit pixels
      aPixel = fListOfPixels->at(tci);
      st = aPixel->GetPixelIndex();
      // compute line and column index from the index
      switch (fMimosaType) {
        case 20:
          linPhys = Mi->get_submatrix(st / nPix,st % nPix)*32+Mi->get_row(st / nPix,st % nPix);
          break;
        case 30:
          if (Mi->get_submatrix(st / nPix,st % nPix)==0)
            linPhys = Mi->get_row(st / nPix,st % nPix);
          else if (Mi->get_submatrix(st / nPix,st % nPix)==1)
            linPhys = 21 + Mi->get_row(st / nPix,st % nPix);
          else if (Mi->get_submatrix(st / nPix,st % nPix)==2)
            linPhys = 43 + Mi->get_row(st / nPix,st % nPix);
          break;
        case 40:
          linPhys = Mi->get_submatrix(st / nPix,st % nPix)*16+Mi->get_row(st / nPix,st % nPix);
          break;
      };

      colPhys = Mi->get_column(st / nPix,st % nPix);
      ComputeStripPosition( colPhys, linPhys, u, v, w); // JB 2012/11/21
      //      u = ((2*colPhys - fStripsNu + 1 ) * fc->GetPlanePar(fPlaneNumber).Pitch(0))/2 ;
      //      v = ((2*linPhys - fStripsNv + 1 ) * fc->GetPlanePar(fPlaneNumber).Pitch(1))/2 ;
      //
      //      w = fc->GetPlanePar(fPlaneNumber).Pitch(2);
      tPosition.SetValue(u,v,w); //
      aPixel->SetSize( (*fPitch));
      aPixel->SetPixelLine( linPhys);
      aPixel->SetPixelColumn( colPhys);
      aPixel->SetPosition( tPosition);

      aPixel->SetPixelIndex(colPhys + linPhys*16); //RDM060809

      if( fDebugPlane>3 ) printf("DPlane:Update  pixel %d with index %d updated at (line,col)=(%d,%d), position (%f, %f, %f), size (%f, %f, %f) value %f\n", tci, st, linPhys, colPhys, u, v, w, (*fPitch)(0), (*fPitch)(1), (*fPitch)(2), aPixel->GetPulseHeight());
    } // end loop over hit pixels
    if( fPixelsN==0 ) {
      goForAnalysis = kFALSE;
      if( fDebugPlane>2 ) cout <<"WARNING Plane " << fPlaneNumber << " has no hit pixels, no more analysis for it in real event " << fAcq->GetEventNumber() << endl; //RDM310509 add if debug
    }
    delete Mi; //RDM290509
  } //end readout==125


  //======================
  // Readout without zero suppression BUT recquiring re-ordering
  // of channels wrt strips
  // RDM 210509, then JB 2009/05/25, YV 27/11/09
  // MIMOSA24 type indexes

  else if ( fReadout==24 ){

    TMimosa24_25Map *Mi;
    Mi = new TMimosa24_25Map("Mi24");
    fPixelsN = fListOfPixels->size(); // update number of hit pixels
    Int_t nPix = 4096 ;     //YV 09/06/09

    for (Int_t tci = 0; tci < fPixelsN; tci++) { // loop over hit pixels
      aPixel = fListOfPixels->at(tci);
      st = aPixel->GetPixelIndex();

      // compute line and column index from the index
      switch(Mi->get_submatrix(st / nPix,st % nPix)) {
        case 0:// OK ?
          linPhys = 62 - Mi->get_row(st / nPix,st % nPix);
          colPhys = Mi->get_column(st / nPix,st % nPix);
          break;
        case 1: // OK
          linPhys = 31 - Mi->get_row(st / nPix,st % nPix);
          colPhys = Mi->get_column(st / nPix,st % nPix);
          break;
        case 2: // OK
          linPhys = 64 + Mi->get_row(st / nPix,st % nPix);
          colPhys = Mi->get_column(st / nPix,st % nPix);
          break;
        case 3: // OK
          linPhys = 95 + Mi->get_row(st / nPix,st % nPix);
          colPhys = Mi->get_column(st / nPix,st % nPix);
          break;
        case 4: // OK
          linPhys = 64 + Mi->get_row(st / nPix,st % nPix);
          colPhys = 127 - Mi->get_column(st / nPix,st % nPix);
          break;
        case 5: // OK
          linPhys = 95 + Mi->get_row(st / nPix,st % nPix);
          colPhys = 127 - Mi->get_column(st / nPix,st % nPix);
          break;
        case 6: // OK
          linPhys = 30 - Mi->get_row(st / nPix,st % nPix);
          //colPhys = 31 - Mi->get_column(st / nPix,st % nPix);
          colPhys = 127 - Mi->get_column(st / nPix,st % nPix);  //YV 11/10/09
          break;
        case 7: // OK
          linPhys = 15 - Mi->get_row(st / nPix,st % nPix);
          //colPhys = 31 - Mi->get_column(st / nPix,st % nPix);
          colPhys = 127 - Mi->get_column(st / nPix,st % nPix);  //YV 11/10/09
          break;
        default: // OK
          linPhys = -1111;
          colPhys = -1111;
          break;
      };

      //commented out 30/06/09
      //linPhys = Mi->get_submatrix(st / nPix,st % nPix)*32+Mi->get_row(st / nPix,st % nPix);
      //colPhys = Mi->get_column(st / nPix,st % nPix);

      //phys. index
      stPhys = colPhys + linPhys*fStripsNu;
      aPixel->SetPixelLine( linPhys); // YV, 2009/06/02
      aPixel->SetPixelColumn( colPhys); // YV, 2009/06/02

      // update the strip
      GetStrip(stPhys)->SetPixelIndex( tci);
      GetStrip(stPhys)->SetRawValue( aPixel->GetPulseHeight());
      //GetStrip(stPhys)->UpdateSignal();   // done at call for analyze_basic, JB 2011/04/15
      if( fDebugPlane>3 ) printf("DPlane:Update  pixel %d with index %d updated at (line,col)=(%d,%d) channel=%d, value %f\n", tci, st, linPhys, colPhys, stPhys, aPixel->GetPulseHeight());
    } // end loop over hit pixels
    if( fPixelsN==0 ) {
      goForAnalysis = kFALSE;
    }
    delete Mi;  //RDM290509

  } //end readout==24


  //======================
  // Readout with zero-suppression using a list of hit pixels
  // RDM 210509
  // RDM 220509
  // YV  27/11/09
  // MIMOSA24 type indexes

  else if ( fReadout==124 ){

    TMimosa24_25Map *Mi;
    Mi = new TMimosa24_25Map("Mi24");
    fPixelsN = fListOfPixels->size(); // update number of hit pixels
    Int_t nPix = 4096;

    for (Int_t tci = 0; tci < fPixelsN; tci++) { // loop over hit pixels
      aPixel = fListOfPixels->at(tci);
      st = aPixel->GetPixelIndex();

      //readout M24 as 1 plane

      switch(Mi->get_submatrix(st / nPix,st % nPix)) {
        case 0:// OK ?
          linPhys = 62 - Mi->get_row(st / nPix,st % nPix);
          colPhys = Mi->get_column(st / nPix,st % nPix);
          break;
        case 1: // OK
          linPhys = 31 - Mi->get_row(st / nPix,st % nPix);
          colPhys = Mi->get_column(st / nPix,st % nPix);
          break;
        case 2: // OK
          linPhys = 64 + Mi->get_row(st / nPix,st % nPix);
          colPhys = Mi->get_column(st / nPix,st % nPix);
          break;
        case 3: // OK
          linPhys = 95 + Mi->get_row(st / nPix,st % nPix);
          colPhys = Mi->get_column(st / nPix,st % nPix);
          break;
        case 4: // OK
          linPhys = 64 + Mi->get_row(st / nPix,st % nPix);
          colPhys = 127 - Mi->get_column(st / nPix,st % nPix);
          break;
        case 5: // OK
          linPhys = 95 + Mi->get_row(st / nPix,st % nPix);
          colPhys = 127 - Mi->get_column(st / nPix,st % nPix);
          break;
        case 6: // OK
          linPhys = 30 - Mi->get_row(st / nPix,st % nPix);
          //colPhys = 31 - Mi->get_column(st / nPix,st % nPix);
          colPhys = 127 - Mi->get_column(st / nPix,st % nPix);   //YV 11/10/09
          break;
        case 7: // OK
          linPhys = 15 - Mi->get_row(st / nPix,st % nPix);
          //colPhys = 31 - Mi->get_column(st / nPix,st % nPix);
          colPhys = 127 - Mi->get_column(st / nPix,st % nPix);   //YV 11/10/09
          break;
        default: // OK
          //printf("pixel index = %d, row = %d, column = %d, submatrix = %d \n", st, Mi->get_row(st / nPix,st % nPix), Mi->get_column(st / nPix,st % nPix), Mi->get_submatrix(st / nPix,st % nPix)); //YV 23/09/09
          linPhys = -1111;
          colPhys = -1111;
          break;
      };


      ComputeStripPosition( colPhys, linPhys, u, v, w); // JB 2012/11/21
      //      u = ((2*colPhys - fStripsNu + 1 ) * fc->GetPlanePar(fPlaneNumber).Pitch(0))/2 ;
      //      v = ((2*linPhys - fStripsNv + 1 ) * fc->GetPlanePar(fPlaneNumber).Pitch(1))/2 ;
      //      w = fc->GetPlanePar(fPlaneNumber).Pitch(2);
      tPosition.SetValue(u,v,w);
      aPixel->SetSize( (*fPitch));
      aPixel->SetPixelLine( linPhys);
      aPixel->SetPixelColumn( colPhys);
      aPixel->SetPosition( tPosition);

      //YV 21/10/09

      TH2F *h2 = fHNoise;
      aNoise = h2->GetBinContent(st%64,64-(st/64));  //1st channel
      //aNoise = h2->GetBinContent(st%64,(st-4096)/64);  //2nd channel
      //aNoise = h2->GetBinContent(st%64,(st-8192)/64);   //3rd channel
      //aNoise = h2->GetBinContent((st%64)+1,(st-12288)/64+32);   //4rth channel
      aPixel->SetNoise(aNoise);
      //printf("pixel %d, noise = %f\n",st,aNoise);

      TH2F *h3 = fHPedestal;
      //aPedestal = h3->GetBinContent((st%64)+1,(st/64));  //channel 1
      //aPedestal = h3->GetBinContent((st%64)+1,(st-4096)/64);  //channel 2
      //aPedestal = h3->GetBinContent((st%64)+1,(st-8192)/64);   //channel 3
      aPedestal = h3->GetBinContent((st%64)+1,(st-12288)/64+32);   //channel 4
      aPixel->SetPedestal(aPedestal);
      Float_t newRawValue = aPixel->GetPulseHeight() - aPedestal;
      aPixel->SetPulseHeight(newRawValue);
      //printf("pixel: %d, new raw value=%f \n",st,aPixel->GetPulseHeight());

      if( fDebugPlane>3 ) printf("DPlane:Update  pixel %d with index %d updated at (line,col)=(%d,%d) channel=%d and rawvalue (pix)%f (strip)%f\n", tci, st, linPhys, colPhys, stPhys, aPixel->GetRawValue(), GetStrip(stPhys)->GetRawValue());

    } // end loop over hit pixels
    if( fPixelsN==0 ) {
      goForAnalysis = kFALSE;
    }
    delete Mi; //RDM290509

  } //end readout==124


  //======================
  // Readout with binary zero-suppression using a list of hit pixels
  // JB 2009/08/17
  // MIMOSA26 && MIMOSA28 && FSBB
  else if ( fReadout==126 ){

    fPixelsN = fListOfPixels->size(); // update number of hit pixels

    for (Int_t tci = 0; tci < fPixelsN; tci++) { // loop over hit pixels
      aPixel = fListOfPixels->at(tci);
      aPixel->SetPixelIndex(aPixel->GetPixelLine()*fStripsNu + aPixel->GetPixelColumn());
      ComputeStripPosition( aPixel->GetPixelColumn(), aPixel->GetPixelLine(), u, v, w); // JB 2012/11/21
      //      u = ((2*aPixel->GetPixelColumn() - fStripsNu + 1 ) * fc->GetPlanePar(fPlaneNumber).Pitch(0))/2 ;
      //      v = ((2*aPixel->GetPixelLine() - fStripsNv + 1 ) * fc->GetPlanePar(fPlaneNumber).Pitch(1))/2 ;
      //      w = fc->GetPlanePar(fPlaneNumber).Pitch(2);
      tPosition.SetValue(u,v,w); //
      aPixel->SetSize( (*fPitch));
      aPixel->SetPosition( tPosition);
      if( fDebugPlane>3 ) printf("DPlane:Update  pixel %d with index %d at (line,col)=(%d,%d), position (%f, %f, %f), size (%f, %f, %f) value %f\n", tci, aPixel->GetPixelIndex(), aPixel->GetPixelLine(), aPixel->GetPixelColumn(), u, v, w, (*fPitch)(0), (*fPitch)(1), (*fPitch)(2), aPixel->GetPulseHeight());
    } // end loop over hit pixels
    if( fPixelsN==0 ) {
      goForAnalysis = kFALSE;
    }

  } //end readout==126


  //======================
  // Readout without zero suppression for Mimosa 32A
  else if ( fReadout==32 ){
    fPixelsN = fListOfPixels->size(); // update number of hit pixels

    //    if( fPixelsN!=fChannelsN ) {
    //      cout <<"WARNING Plane " << fPlaneNumber << " has " << fPixelsN << " hit pixels which is different from the exepected " << fChannelsN << "channels in non-sparsified mode." << endl;
    //    }
    for (Int_t tci = 0; tci < fPixelsN; tci++) { // loop over hit pixels
      aPixel = fListOfPixels->at(tci);
      st = aPixel->GetPixelIndex();

      linPhys = st / fStripsNu ;
      colPhys = st % fStripsNu ;
      //SS 2012.08.10 - Solution for the wrong line order
      // for run taken with PXIe (>328xx)
      if( fSession->GetRunNumber()>=32800 ) {
        if (colPhys==0) colPhys=63;
        else colPhys--;
        //JB 2012/08/18 - swapping of real column groups 8-11 with 12-15
        //  note that real columns are lines in our config
        // ONLY FOR RUN 32824
        if( fSession->GetRunNumber()==32824 ) {
          if( 8<=linPhys && linPhys<=11) linPhys += 4;
          else if( 12<=linPhys && linPhys<=15) linPhys -= 4;
        }
      }

      // update the pixel
      stPhys = colPhys + linPhys*fStripsNu;
      aPixel->SetPixelLine( linPhys);
      aPixel->SetPixelColumn( colPhys);
      aPixel->SetPixelIndex( stPhys);

      // update the strip,
      // take into account that after noise initialization
      //  we consider only absolute value of frame1-frame2.
      GetStrip(stPhys)->SetPixelIndex( tci);
      if(fInitialCounter <= fInitialNoise) {
        GetStrip(stPhys)->SetRawValue( aPixel->GetRawValue() );
      }
      else {
        GetStrip(stPhys)->SetRawValue( fabs(aPixel->GetRawValue()) );
      }

      if( fDebugPlane>3 ) printf("DPlane:Update  pixel %d with index %d, physical index %d at (line,col)=(%d,%d) and raw value %.1f or %.1f\n", tci, st, stPhys, linPhys, colPhys, aPixel->GetRawValue(), GetStrip(stPhys)->GetRawValue());


      if ( fIfDigitize && fInitialCounter > fInitialNoise ) { // if some digitization is required and after initialization
        // further update the pixel
        ComputeStripPosition( colPhys, linPhys, u, v, w);
        tPosition.SetValue(u,v,w); //
        aPixel->SetSize( (*fPitch) );
        aPixel->SetPosition( tPosition);

        aRawvalue = fabs(aPixel->GetRawValue());
        aPedestal = GetStrip(stPhys)->GetPedestal();
        aNoise = GetStrip(stPhys)->GetNoise();
        aPixel->SetNoise( aNoise);
        aPixel->SetPedestal( aPedestal);
        //aRawvalue = Digitize( aRawvalue - aPedestal );
        //aPixel->SetRawValue( aRawvalue );
        aPixel->SetPulseHeight( aRawvalue - aPedestal );

        if( fDebugPlane>3 ) printf("DPlane:Update  pixel %d with index %d, physical index %d at (line,col)=(%d,%d) and raw value %.1f, pulse height %.1f\n", tci, st, stPhys, linPhys, colPhys, aPixel->GetRawValue(), aPixel->GetPulseHeight());

      } // end some digitization is required

    } // end loop over hit pixels

    if( fPixelsN==0 ) {
      goForAnalysis = kFALSE;
      cout <<"WARNING Plane " << fPlaneNumber << " has no hit pixels, no more analysis for it in real event " << fAcq->GetEventNumber() << endl;
    }

  } //end if Readout==32

  //======================
  // Readout with zero suppression for Mimosa 32A
  else if ( fReadout==132 ){
    fPixelsN = fListOfPixels->size(); // update number of hit pixels

    //    if( fPixelsN!=fChannelsN ) {
    //      cout <<"WARNING Plane " << fPlaneNumber << " has " << fPixelsN << " hit pixels which is different from the exepected " << fChannelsN << "channels in non-sparsified mode." << endl;
    //    }
    for (Int_t tci = 0; tci < fPixelsN; tci++) { // loop over hit pixels
      aPixel = fListOfPixels->at(tci);
      st = aPixel->GetPixelIndex();

      linPhys = st / fStripsNu ;
      colPhys = st % fStripsNu ;
      //SS 2012.08.10 - Solution for the wrong line order
      // for run taken with PXIe (>328xx)
      if( fSession->GetRunNumber()>=32800 ) {
        if (colPhys==0) colPhys=63;
        else colPhys--;
        //JB 2012/08/18 - swapping of real column groups 8-11 with 12-15
        //  note that real columns are lines in our config
        // ONLY FOR RUN 32824
        if( fSession->GetRunNumber()==32824 ) {
          if( 8<=linPhys && linPhys<=11) linPhys += 4;
          else if( 12<=linPhys && linPhys<=15) linPhys -= 4;
        }
      }

      stPhys = colPhys + linPhys*fStripsNu;
      aPixel->SetPixelLine( linPhys);
      aPixel->SetPixelColumn( colPhys);
      aPixel->SetPixelIndex( stPhys);
      ComputeStripPosition( colPhys, linPhys, u, v, w);
      tPosition.SetValue(u,v,w); //
      aPixel->SetSize( (*fPitch));
      aPixel->SetPosition( tPosition);

      if( fNoiseRun) {
        SetPedandNoiseFromHisto( colPhys, linPhys, aPixel);
        aPixel->SetPixelIndex(aPixel->GetPixelLine()*fStripsNu + aPixel->GetPixelColumn());
      }

      if( fDebugPlane>3 ) printf("DPlane:Update  pixel %d with index %d, physical index %d at (line,col)=(%d,%d) and raw value %.1f\n", tci, st, stPhys, linPhys, colPhys, aPixel->GetRawValue());
    } // end loop over hit pixels

    if( fPixelsN==0 ) {
      goForAnalysis = kFALSE;
      cout <<"WARNING Plane " << fPlaneNumber << " has no hit pixels, no more analysis for it in real event " << fAcq->GetEventNumber() << endl;
    }

  } //end if Readout==132

  //======================
  // Readout without zero suppression AND multiframe for Mimosa 32
  else if ( fReadout==232 ){
    fPixelsN = fListOfPixels->size(); // update number of hit pixels
    if( fDebugPlane>3 ) Printf( "Update:: %d hits to update\n", fPixelsN);

    for (Int_t tci = 0; tci < fPixelsN; tci++) { // loop over hit pixels
      aPixel = fListOfPixels->at(tci);
      st = aPixel->GetPixelIndex();

      linPhys = st / fStripsNu ;
      colPhys = st % fStripsNu ;
      //SS 2012.08.10 - Solution for the wrong line order
      // for run taken with PXIe (>328xx)
      if( fSession->GetRunNumber()>=32800 && fSession->GetRunNumber() < 33000) {
        if (colPhys==0) colPhys=63;
        else colPhys--;
        //JB 2012/08/18 - swapping of real column groups 8-11 with 12-15
        //  note that real columns are lines in our config
        // ONLY FOR RUN 32824
        if( fSession->GetRunNumber()==32824 ) {
          if( 8<=linPhys && linPhys<=11) linPhys += 4;
          else if( 12<=linPhys && linPhys<=15) linPhys -= 4;
        }
      }
      stPhys = colPhys + linPhys*fStripsNu;
      aPixel->SetPixelLine( linPhys);
      aPixel->SetPixelColumn( colPhys);
      ComputeStripPosition( colPhys, linPhys, u, v, w);
      tPosition.SetValue(u,v,w);
      aPixel->SetPosition( tPosition);
      aPixel->SetSize( (*fPitch));

      if( fDebugPlane>3 ) printf("DPlane:Update  pixel %d with index %d, physical index %d at (line,col)=(%d,%d), raw value %.1f\n", tci, st, stPhys, linPhys, colPhys, aPixel->GetRawValue());

      // update the corresponding strip and pixel,
      // Note that after noise initialization
      //  we consider the signed value of frame1-frame2 to properly observe pixel behavior
      // Case for external noise computation (fNoiseRun==kTrue) added.
      // JB 2016/08/17
      GetStrip(stPhys)->SetPixelIndex( tci);
      if( fNoiseRun) {
        SetPedandNoiseFromHisto( colPhys, linPhys, aPixel);
        GetStrip(stPhys)->SetRawValue( aPixel->GetRawValue() );
      }
      else {
        if(fInitialCounter <= fInitialNoise ) {
          //GetStrip(stPhys)->SetRawValue( aPixel->GetRawValue() );
          if( Int_t(aPixel->GetRawValue()) != 0 ) GetStrip(stPhys)->SetRawValue( aPixel->GetRawValue() );
          aPixel->SetPulseHeight( aPixel->GetRawValue() );
        }
        else {
          aRawvalue = aPixel->GetRawValue();
          aPedestal = GetStrip(stPhys)->GetPedestal();
          aNoise = GetStrip(stPhys)->GetNoise();
          GetStrip(stPhys)->SetRawValue( aRawvalue );
          aPixel->SetNoise( aNoise);
          aPixel->SetPedestal( aPedestal);
          aPixel->SetPulseHeight( aRawvalue - aPedestal);
          //if( aRawvalue>200 ) printf("     pixel %d index %d at (line,col)=(%d,%d), timestamp %d, raw value %.1f, pedestal %.1f, noise %.1f, pulseheight %.1f\n", tci, st, stPhys, linPhys, aPixel->GetTimestamp(), GetStrip(stPhys)->GetRawValue(), aPixel->GetPedestal(), aPixel->GetNoise(), aPixel->GetPulseHeight());
        }
      }

      if( fDebugPlane>3 ) printf("              pixel %d raw value %.1f, pedestal %.1f, noise %.1f, pulseheight %.1f\n", tci, GetStrip(stPhys)->GetRawValue(), aPixel->GetPedestal(), aPixel->GetNoise(), aPixel->GetPulseHeight());
    } // end loop over hit pixels

    if( fPixelsN==0 ) {
      goForAnalysis = kFALSE;
      cout <<"WARNING Plane " << fPlaneNumber << " has no hit pixels, no more analysis for it in real event " << fAcq->GetEventNumber() << endl;
    }

  } //end if Readout==232


  //======================
  // ANY OTHER READOUT issue a warning
  else {
    planeReady = kFALSE;
    printf( "WARNING, DPlane:Update readout %d unknown, no analysis !\n", fReadout);
  }


  //======================
  // Call the analysis now
  //======================

  if (goForAnalysis) {

    //==============
    // compute pedestal, noise and common noise shift
    // or update the strip values
    // exclude zero-suppressed data (readout>100), JB 2009/05/25
    // exclude noise computed from previous run, JB 2014/01/07
    // include multi-frame readout (232) only for init step, JB 2013/
    // modified condition fInitialCounter<fInitialNoise instead of <=, JB 2016/08/23
    if( (fReadout<100) || (fReadout==232 && fInitialCounter<fInitialNoise) ) {
      analyze_basics();
    }

    //==============
    // Analysis
    // not performed if analysisMode==0, JB 2011/04/15
    if (fSession->GetStatus() != 0 && fAnalysisMode>0) {

      //==============
      // Digitize the matrix if required
      if( fIfDigitize>0 ) {
        DigitizeMatrix();
      }

      //==============
      // potentially update pedestal, noise
      // or update the strip values
      // only for not zero-suppressed data, JB 2011/04/15
      //      if( fReadout<100 && fAnalysisMode<=1 ) {
      //        UpdatePedestalAndNoise();
      //      } // end if readout<100

      //==============
      // look for hits
      find_hits();
      if (fDebugPlane) cout << "  Plane " << fPlaneNumber << " : " << fHitsN << " new  hits found" << endl;

    }

  } // end if goForAnalysis
  else {
    if (fDebugPlane) cout << "Plane " << fPlaneNumber << ": can't analyse or no data " << endl ;
  }

  if ( !planeReady && fDebugPlane) cout << "Plane " << fPlaneNumber << ": bad raw data " << endl ;

  if(fAcq->GetIfMCBoardReader()) { // AP, 2016/07/27. Do some operations only in the case of reading MC-data
    //Now, if some hits where not digitized, then generate hits with the MC information
    CheckNonDigitizedMCHits();

    //Do hit truth matching and get the particle generating this hit
    MCHitsTruthMatching();
  } // end if reading MC-data

  return !planeReady; // JB 2010/09/20

}
//______________________________________________________________________________
//
DStrip* DPlane::GetStrip(Int_t aSk)
{
  // Modified by JB, to start at 0 up to fStripsN-1, 2008/10/17

  if( aSk >= 0 && aSk < fStripsN ) {
    return  fStripList[aSk];
  }
  else {
    //printf("WARNING DPlane::GetStrip() request to non-existing strip %d, investigate!\n Returning strip 0.\n", aSk);
    return  fStripList[0];
  }
}

//______________________________________________________________________________
//

void DPlane::ComputeStripPosition(Int_t col, Int_t lin, Double_t &u, Double_t &v, Double_t &w)
{

  fTool.ComputeStripPosition( fMapping, col, lin, u, v, w, fStripsNu, fStripsNv, (*fPitch)(0), (*fPitch)(1), (*fPitch)(2));

  /*
   // Compute the 3D position of the strip at column "col" and line "lin",
   //  set the values in the "u, v, w" variables.
   // The strip position depends on the mapping, look below.
   //
   // JB 2012/11/21
   // Modified CLM 2012/11/24 correction for P31 of M32ter
   // Modified AB 2015/03/31  new case 7 adapted to M22thrb6,7

   switch (fMapping) {

   // When pixels are organized *on an orthogonal grid
   case 1:
   default:
   u = ((2*col - fStripsNu + 1 ) * (*fPitch)(0))/2 ;
   //printf( " u = (2x%d-%d+1) * %f/2 = %d*%f = %f\n", col, fStripsNu, (*fPitch)(0), (2*col - fStripsNu + 1 ), (*fPitch)(0)/2, u);
   v = ((2*lin - fStripsNv + 1 ) * (*fPitch)(1))/2 ;
   //printf( " v = (2x%d-%d+1) * %f/2 = %d*%f = %f\n", lin, fStripsNv, (*fPitch)(1), (2*lin - fStripsNv + 1 ), (*fPitch)(1)/2, v);
   break;

   // When pixels are staggered from one column to the other
   case 2:
   u = ((2*col - fStripsNu + 1 ) * (*fPitch)(0))/2 ;
   // v = ((2*lin + 1./2. - lin%2 - fStripsNv + 1 ) * (*fPitch)(1))/2 ;
   // v = ((2*lin - 1./2. + lin%2 - fStripsNv + 1 ) * (*fPitch)(1))/2 ; //clm
   //printf( " u = (2x%d-%d+1) * %f/2 = %d*%f = %f\n", col, fStripsNu, (*fPitch)(0), (2*col - fStripsNu + 1 ), (*fPitch)(0)/2, u);

   if ( col%2 == 0 ) v = (((lin - fStripsNv/2.0 + 1./2. ) * (*fPitch)(1))) + 0.30 * (*fPitch)(1);  //clm 2012.11.24
   else              v = (((lin - fStripsNv/2.0 + 1./2. ) * (*fPitch)(1))) - 0.19 * (*fPitch)(1);  //clm 2012.11.24
   //printf( " v = (2x%d-%d+1) * %f/2 = %d*%f = %f\n", lin, fStripsNv, (*fPitch)(1), (2*lin - fStripsNv + 1 ), (*fPitch)(1)/2, v);
   break;
   case 3:
   //Clm Mapping For M32 L8_2
   if ( lin==7 && col == 10) u = ((2*col - fStripsNu + 1 ) * (*fPitch)(0))/2 ;
   else           u = -9999; //((2*col+2 - fStripsNu + 1 ) * (*fPitch)(0))/2 ;
   v = ((2*lin - fStripsNv + 1 ) * (*fPitch)(1))/2 ;
   break;
   // MonteCarlo Simulation. // LC 2014/01/10.
   case 4:
   u = ((2.*col - fStripsNu + 2. ) * (*fPitch)(0))/2. ;  // LC 2015/02/17 : +2 commented
   //printf( " u = (2x%d-%d+1) * %f/2 = %d*%f = %f\n", col, fStripsNu, (*fPitch)(0), (2*col - fStripsNu + 1 ), (*fPitch)(0)/2, u);
   v = ((2.*lin - fStripsNv + 2. ) * (*fPitch)(1))/2. ;  // LC 2015/02/17 : +2 commented
   //printf( " v = (2x%d-%d+1) * %f/2 = %d*%f = %f\n", lin, fStripsNv, (*fPitch)(1), (2*lin - fStripsNv + 1 ), (*fPitch)(1)/2, v);
   break;
   case 5:
	 {
   u = (2*col + 1 - fStripsNu) * (*fPitch)(0)/2.0;
   double fraction = 0.25;
   //double fraction = 0.75;
   if ( col%2 == 0 ) v = 0.5*(fStripsNv - 2*lin - 2*(fraction    )) * (*fPitch)(1);
   else              v = 0.5*(fStripsNv - 2*lin - 2*(1 - fraction)) * (*fPitch)(1);
   break;
	 }
   case 7: //M22-THRB7 and B6 -> set the matrix type  and Mapping accordingly in the config file !
   u = ((2*col - fStripsNu + 1 ) * (*fPitch)(0))/2 ;
   if ( col%2 == 0 ) v = (((lin - fStripsNv/2.0 + 1./2. ) * (*fPitch)(1))) - 0.50 * (*fPitch)(1);  //correct +0.5
   else              v = (((lin - fStripsNv/2.0 + 1./2. ) * (*fPitch)(1))) - 0.00 * (*fPitch)(1);  //correct -0.5
   //cout<<"  DPlane::ComputeStripPosition "<<u<<" "<<v<<" "<<col<<" "<<lin<<endl;
   break;

   }


   w = (*fPitch)(2);

   //printf(" --> N(%d,%d), pitch(%.1f,%.1f), (%d, %d): %.1f, %.1f\n", fStripsNu, fStripsNv, (*fPitch)(0), (*fPitch)(1), col, lin, u, v);

   */

  return;

}

//______________________________________________________________________________
//
void DPlane::ComputeStripPosition_UVToColRow( Double_t u, Double_t v, double &col, double &lin)
{

  fTool.ComputeStripPosition_UVToColRow( fMapping, u, v, col, lin, fStripsNu, fStripsNv, (*fPitch)(0), (*fPitch)(1));

  /*
   // Compute the 2D position of a strip in the variables u and v to the set of variables column and line
   // The strip position depends on the mapping, look below.
   // AP 2014/06/26
   // Modified AB 2015/03/31  new case 7 adapted to M22thrb6,7

   //cout << (*fPitch)(0) << "  " << (*fPitch)(1) << endl;

   switch (fMapping) {
   // When pixels are organized on an orthogonal grid
   case 1:
   default:
   col = (u/(*fPitch)(0)) + ((fStripsNu - 1)/2.);
   lin = (v/(*fPitch)(1)) + ((fStripsNv - 1)/2.);
   break;
   // When pixels are staggered from one column to the other
   case 2:
   col = (u/(*fPitch)(0)) + ((fStripsNu - 1)/2.);
   if(int(col)%2 == 0) lin = (v/(*fPitch)(1)) + ((fStripsNv - 1)/2.) - 0.30;
   else                lin = (v/(*fPitch)(1)) + ((fStripsNv - 1)/2.) + 0.19;
   break;
   case 3:
   //Clm Mapping For M32 L8_2
   col = (u/(*fPitch)(0)) + ((fStripsNu - 1)/2.);
   lin = (v/(*fPitch)(1)) + ((fStripsNv - 1)/2.);
   break;
   // MonteCarlo Simulation. // LC 2014/01/10.
   case 4:
   col = (u/(*fPitch)(0)) + ((fStripsNu)/2.); // LC 2015/02/17 : -2 commented
   lin = (v/(*fPitch)(1)) + ((fStripsNv)/2.); // LC 2015/02/17 : -2 commented
   break;

   case 5:
	 {
   col = (u/(*fPitch)(0)) + ((fStripsNu - 1)/2.);

   double fraction = 0.25;
   //double fraction = 0.75;

   int IntCol = int(col);
   double delta = col - IntCol;
   if(delta >= 0.5) IntCol++;

   if(IntCol%2 == 0) lin = ((fStripsNv - 2*(fraction    ))/2.0)  -  (v/(*fPitch)(1));
   else              lin = ((fStripsNv - 2*(1 - fraction))/2.0)  -  (v/(*fPitch)(1));

   //cout << "(u,v) = (" << u << "," << v << ")" << endl;
   //cout << "col    = " << col    << endl;
   //cout << "IntCol = " << IntCol << endl;
   //cout << "delta  = " << delta  << endl;
   //cout << "IntCol = " << IntCol << endl;
   //cout << "lin    = " << lin << endl;

   break;
	 }
   case 7://M22-THRB7 and B6 -> set the matrix type and Mapping accordingly in the config file !
   col = (u/(*fPitch)(0)) + ((fStripsNu - 1)/2.);
   if(int(col)%2 == 0) lin = (v/(*fPitch)(1)) + ((fStripsNv - 1)/2.) + 0.50; //correct -0.5
   else                lin = (v/(*fPitch)(1)) + ((fStripsNv - 1)/2.) + 0.0; //correct +0.5
   //cout<<"  DPlane::ComputeStripPosition_UVToColRow "<<u<<" "<<v<<" "<<col<<" "<<lin<<endl;
   break;

   }

   */

  return;

}

//______________________________________________________________________________
//
void DPlane::FindNeighbours(){
  //-+-+-+   Finding neighbours
  // checked by JB, 2008/10/17
  // Modified: AB 2012/10/22 bug fixed in computation of neighbourLines/Columns

  if (fDebugPlane) printf("  DPlane: Finding neighbourgs for %d strips with mode %d\n", fStripsN, fAnalysisMode);

  // Method for strips
  if (fAnalysisMode < 2){

    // Method starting from the strip itself and going out of it
    Int_t stTest, rc, diffU;
    for ( Int_t st = 0; st < fStripsN; st++) {

      fStripList[st]->Introduce(*fStripList[st]); // Introduce itself

      diffU = rc = 0;
      while ( !rc && abs(diffU)<fStripsNu ) {

        stTest = st + diffU;
        if ( 0<=stTest && stTest<fStripsN && stTest!=st ) {
          rc = fStripList[st]->Introduce(*fStripList[stTest]);
        }
        diffU++;

      }

      diffU = -1;
      rc = 0;
      while ( !rc && abs(diffU)<fStripsNu ) {

        stTest = st + diffU;
        if ( 0<=stTest && stTest<fStripsN  && stTest!=st ) {
          rc = fStripList[st]->Introduce(*fStripList[stTest]);
        }
        diffU--;
      }
    }
  }
  // Method for pixels
  // bug fixed for neighbourLines/Columns -> Limit(1,0)/Pitch(1,0), AB 2012/10/22
  else if (fAnalysisMode >= 2){

    // only for debug messages
    Int_t minSt=200, maxSt=260;

    Int_t  tNeighbourLines   = (Int_t) (fc->GetPlanePar(fPlaneNumber).ClusterLimit(1)/fc->GetPlanePar(fPlaneNumber).Pitch(1));
    Int_t  tNeighbourColumns = (Int_t) (fc->GetPlanePar(fPlaneNumber).ClusterLimit(0)/fc->GetPlanePar(fPlaneNumber).Pitch(0));
    if (fDebugPlane)  cout << "FindNeighbours:   tNeighbourLines ="    << tNeighbourLines << " of " << (*fPitch)(0) << " um and tNeighbourColumns=" << tNeighbourColumns << " of " << (*fPitch)(1) << " um." << endl;
    for (Int_t sta = 0; sta < fStripsN; sta++) { // loop on all strips
      Int_t iLine = sta/fStripsNu;   // Line of this pixel
      Int_t iCol  = sta%fStripsNu;   // Colomn of this pixel
      if (fDebugPlane>3 && minSt<sta && sta<maxSt)  cout << "---------- FindNeighbours:   strip " << sta << " at index " << fStripList[sta]->GetStripIndex() << " ( " << iCol << ", " << iLine << ") introducing: ";
      fStripList[sta]->Introduce(*fStripList[sta]);
      for(Int_t il=iLine-tNeighbourLines; il<iLine+tNeighbourLines+1; il++){
        for(Int_t ic=iCol-tNeighbourColumns; ic<iCol+tNeighbourColumns+1; ic++){
          if (fDebugPlane>3 && minSt<sta && sta<maxSt)  cout << "| " << ic << ", " << il << " at " << il*fStripsNu+ic;
          if(il>=0 && ic>=0 && il<fStripsNv && ic<fStripsNu && !(il==iLine && ic==iCol)){
            if (fDebugPlane>2 && minSt<sta && sta<maxSt)  cout << " INTRODUCING " << fStripList[il*fStripsNu+ic]->GetStripIndex() << endl;
            fStripList[sta]->Introduce(*fStripList[il*fStripsNu+ic]);
            // so pixel can have a maximum number of neighbours
            //but can have less as well if it is near the edge
          }
        }
      }
      if (fDebugPlane>3 && minSt<sta && sta<maxSt)  cout << endl;
    } // end loop on all strips

  } //end if pixels

}

//______________________________________________________________________________
//

void DPlane::SetPedandNoiseFromHisto( Int_t col, Int_t row, DPixel *aPixel) {

  // Set the pixel pedestal and noise from a previous computation
  //  done with another run and stored in a histogram.
  // The pulseheight value of the pixel is updated accordingly (pedestal subtracted).
  // It assumes that the column and line of the pixel are already known.
  // This computation is valid if the following variables are true:
  //  fNoiseRun, fHNoise, fHPedestal.
  //
  // JB 2014/01/07, adapted from YV original code

  if( !fNoiseRun || fHNoise==NULL || fHPedestal==NULL ) return;

  Float_t  aRawvalue, aNoise, aPedestal;

  aRawvalue = aPixel->GetRawValue();
  aPedestal = fHPedestal->GetBinContent( col+1, row+1);
  aNoise = fHNoise->GetBinContent( col+1, row+1);
  if (fDebugPlane>3)  printf("SetPedandNoiseFromHisto:pixel c=%d, r=%d updated with raw=%.1f, ped=%.1f, noise=%.1f\n", col+1, row+1, aRawvalue, aPedestal, aNoise);
  aPixel->SetNoise( aNoise);
  aPixel->SetPedestal( aPedestal);
  aPixel->SetPulseHeight( aRawvalue - aPedestal);

}

//______________________________________________________________________________
//

void DPlane::SetPixelGainFromHisto( Int_t col, Int_t row, DPixel *aPixel) {

  // Set the pixel gain from a previous computation
  // This computation is valid if the following variables are true:
  //  fPixelGainRun, fHPixelGain
  //
  // JB 2018/08/01

  if( !fPixelGainRun || fHPixelGain==NULL ) return;

  Float_t  aRawvalue = aPixel->GetRawValue() / fHPixelGain->GetBinContent( col+1, row+1);
  aPixel->SetRawValue( aRawvalue);
  if (fDebugPlane>3)  printf("SetPixelGainFromHisto:pixel c=%d, r=%d updated with raw=%.1f/%.1f=%.1f\n", col+1, row+1, aPixel->GetRawValue(), fHPixelGain->GetBinContent( col+1, row+1), aRawvalue);

}

//______________________________________________________________________________
//

void DPlane::analyze_basics(){

  // Depending on the initializing status, change the strip related values
  // before init: accumulate data,
  // at init    : compute initial values for pedestal and noise
  // after init : update the noise
  // Operations before and after init are only performed for fired strips.
  //
  // JB, 2008/10/13
  // Last Modified, JB 2010/09/20 allow computation only for fired strips (=pixels)
  // Last Modified, JB 2011/04/15 allow update only for fired strips (=pixels)
  // Last Modified, JB 2016/07/20 better management of fNoiseRun option

  Int_t  region, st;
  DPixel *aPixel;
  DStrip *aStrip;

  //================
  // for first events just accumulate pedestals and their squares
  if(fInitialCounter <= fInitialNoise && !fNoiseRun) {
    fInitialCounter++;
    if(fDebugPlane) printf("DPlane::analyze_basics called for plane %d accumulating data with event %d where %d are required for init\n", fPlaneNumber, fInitialCounter, fInitialNoise);

    // Loop only on fired pixels, JB 2010/09/20
    for (Int_t tci = 0; tci < fPixelsN; tci++) { // loop over hit pixels
      aPixel = fListOfPixels->at(tci);
      st     = fStripsNu*aPixel->GetPixelLine() + aPixel->GetPixelColumn();
      aStrip = GetStrip(st);
      if( fDebugPlane>2) printf("analyze_basics: pix=%d, st=%d, rawdata=%.1f (pixVal %.1f)\n", tci, st, aStrip->GetRawValue(), aPixel->GetPulseHeight());
      aStrip->SumValue();
      aStrip->SumSquareValue();
      aStrip->UpdateSignal(); // added JB 2012/08/17, rawData=pulseheight since ped and CMS are still 0 for now
    }
  }


  //================
  // When init is over calculate pedestal and noise
  if (fInitialCounter == fInitialNoise && !fNoiseRun) {

    if(fDebugPlane) printf("DPlane::analyze_basics called for plane %d InitNoiseAndPedestal : fInitialCounter = %d\n", fPlaneNumber, fInitialCounter  );

    for(st = 0; st < fStripsN; st++) {
      aStrip = GetStrip(st);
      aStrip->InitNoiseAndPedestal();
      aStrip->UpdateSignal(); // added JB 2012/08/17
      aStrip->SetFound( kFALSE); // JB 2012/08/20
      if(fDebugPlane && (st<=5 || (fStripsN-st)<5)) printf("DPlane::analyse_basics Pl %d st %d ped=%f noise=%f\n", fPlaneNumber, st, aStrip->GetPedestal(), aStrip->GetNoise());
    }
    cout << "-- Plane " << fPlaneNumber << " Pedestals and Noises initialized at event " << fInitialCounter << " --" << endl;
  }


  //================
  // For all events after the init step
  else if (fInitialCounter >= fInitialNoise) {

    // calculate common mode shift
    //CalculateCommonMode(); // Do not work yet, JB 2020/09/20

    // Loop only on fired pixels, JB 2011/04/15
    for (Int_t tci = 0; tci < fPixelsN; tci++) { // loop over hit pixels
      aPixel = fListOfPixels->at(tci);
      st     = fStripsNu*aPixel->GetPixelLine() + aPixel->GetPixelColumn();
      aStrip = GetStrip(st);
      region = (Int_t)((1.*(st-1))/fStripsN * fRegions);
      aStrip->SetCommonMode(fCommonShift[region]);
      //aStrip->Update();
      aStrip->UpdateSignal();
      aStrip->SetFound( kFALSE); // JB 2012/08/20
      // The two following lines are required because the pixel list
      //  is used to find hits, JB 2012/08/17
      // Additional possibility for digitization emulation, JB 2013/08/29
      //      if( !fIfDigitize ) {
      aPixel->SetPulseHeight( aStrip->GetPulseHeight() );
      aPixel->SetNoise( aStrip->GetNoise() );
      //      }
      //      else {
      //        Int_t aValue = Digitize( aStrip->GetPulseHeight() );
      //        aPixel->SetRawValue( aValue );
      //        aPixel->SetPulseHeight( aValue );
      //      }

      if(fDebugPlane>3 /*&& st<=5*/) printf("DPlane::analyse_basics Pl %d st %d raw=%.1f ped=%.1f noise=%.1f cms=%.1f pulse=%.1f snr=%.1f and pixel pulse=%.1f snr=%.1f\n", fPlaneNumber, st, aStrip->GetRawValue(), aStrip->GetPedestal(), aStrip->GetNoise(), aStrip->GetCommonMode(), aStrip->GetPulseHeight(), aStrip->GetPulseHeightToNoise(), aPixel->GetPulseHeight(), aPixel->GetPulseHeightToNoise());
    }

  }
}

void DPlane::UpdatePedestalAndNoise(){
  for(Int_t st = 0; st < fStripsN; st++) {
    // -- if strip (pixel) doesn't belong to a cluster, update ped and noise
    if(!(GetStrip(st)->Found())){
      GetStrip(st)->UpdatePedestalAndNoise();
      if(fDebugPlane && st<=5) printf("DPlane::UpdatePedestalAndNoise Pl %d st %d ped=%f noise=%f\n", fPlaneNumber, st, GetStrip(st)->GetPedestal(), GetStrip(st)->GetNoise());
    }
  }
}

void DPlane::CalculateCommonMode() {

  // -+-+- Calculate the common signal shift on several channels in the detector
  //       in order to exclude channels with hit do the following
  //       split detector in different regions
  //       calculate shift for each region, exclude bad channels
  //       (given by strip map)  choose the smallest shift

  Int_t region, st;
  Float_t  r, tNoiseCut;

  Int_t fReadout;
  fReadout=fc->GetPlanePar(fPlaneNumber).Readout;

  for (region = 0; region < fRegions; region++) {   // loop over regions
    fCommonShift[region]    = 0.0;                        // reset the comon shift
    fCommonChannels[region] = 0;                          // and the channel counter
  }

  for (st = 0; st < fStripsN; st++) {
    region = (Int_t)((1.*st)/fStripsN * fRegions);
    if (fChannelGood[st] == 1) {                         // limit on good channels i.e. exclude bad
      // exclude hits from calculation
      tNoiseCut = fc->GetAnalysisPar().CmsNoiseCut * GetStrip(st)->GetNoise();
      if (fabs( r =GetStrip(st)->GetRawValue()-GetStrip(st)->GetPedestal() )< tNoiseCut){
        fCommonShift[region]+=r;
        fCommonChannels[region]++;
      }
    }
  }

  if( fReadout==10 || fReadout==12 || fReadout==13){ // exclude some readout mode
    fCommonShift[region] = 0.0;
  }
  else { // for all other readout

    for (region = 0; region < fRegions; region++) { // loop over regions
      if (fCommonChannels[region] > 0){
        fCommonShift[region] /= fCommonChannels[region];  // calculate shift for each regions
      }
      else {
        fCommonShift[region] = 0.0;                       // do not shift, if no channel good

        //if(fPlaneNumber>2){ // don't see why we should keep this, historical, JB, 2007, June
        printf(" DPlane, Plane %d, CommonMode Error in Region %d (# channels selected=%ld), investigate !!!\n",fPlaneNumber,region+1,fCommonChannels[region]);
        //}

      }

      if(fDebugPlane>1) printf("DPLane::CalculateCommonMode pl %d, region %d, channels used %ld, cms %f\n", fPlaneNumber, region, fCommonChannels[region], fCommonShift[region]);
    } //end loop on regions

  }

}

//______________________________________________________________________________
//

Float_t DPlane::minimum(const Float_t* d, const Int_t N){
  Float_t m;

  m = fabs(d[N-1]);
  for (Int_t k = 0; k < N-1; k++)
    m = (m < fabs(d[k])) ? m : fabs(d[k]);
  printf("in Dlane:minimum: m=%f",m);

  return m;
}

//______________________________________________________________________________
//

Int_t DPlane::extremumIndex(const Float_t* d,
                            const Int_t N){
  Int_t ext;
  ext = N-1;

  for (Int_t k = 0; k < N-1; k++)
    ext = (fabs(d[ext]) > fabs(d[k])) ? ext : k;
  return ext;
}

//______________________________________________________________________________
//
Bool_t DPlane::CheckSaturation(){

  //Quality check to eliminate "dead" events: when both frames saturate, CDS is zero flat.
  // We exclude such events.
  // But the procedure can be useless if we apply first "cleaning" by KillNoise
  //
  // This is the only place where we can still use the fRawData[] array
  // because we don't care about the ordering of channels here, JB 2009/05/25

  Bool_t QC = kFALSE;
  Double_t RawDataMean=0;
  Double_t RawDataVar=0;
  Double_t RawData2=0;
  for(Int_t i=0; i<fChannelsN/2; i++){
    RawDataMean += fRawData[i];
    RawData2 += fRawData[i]*fRawData[i];
    if (fDebugPlane>3 && i<5) {
      printf("DPLane::CheckSaturation rawData[%d]=%d\n", i, fRawData[i]);
    }
  }
  RawDataMean /= fChannelsN/2;
  RawData2 /= fChannelsN/2;
  RawDataVar=RawData2-RawDataMean*RawDataMean;
  fCDSvariance = RawDataVar;
  //---abm8 mimosa 8 digital case: don't check saturation:
  Int_t fReadout;
  fReadout=fc->GetPlanePar(fPlaneNumber).Readout;
  if (fDebugPlane>2) {
    printf("DPLane::CheckSaturation for readout %d", fReadout);
    printf("  mean=%f, squareMean=%f, var=%f, CDSvar=%f\n", RawDataMean, RawData2, RawDataVar, fCDSvariance);
  }
  //----ADC
  if((fReadout!=10)&&(fReadout!=12)&&(fReadout!=13)){ //mimosa 8 digital ,
    if(RawDataMean == 0. || RawDataVar < 0.1){
      cout << "Dead (saturated) event # " << fAcq->GetEventNumber()  << "; Plane "<< fPlaneNumber << endl;
      QC=kTRUE;
    }
  }
  //----ADC

  return QC;
}

//______________________________________________________________________________at(
//

Int_t DPlane::Digitize( Float_t aValue ){

  // Digitize an analogue value according to the thresholds provided.
  // Number of thresholds (i.e. digital values) is given by fIfDigitize
  //  and threshold by array fDigitizeThresholds.
  // PAY ATTENTION, those thresholds are not ADC-bit limits,
  //  rather the limits inbetween the fIfDigitize possible values.
  // The maximum digital value returned is fIfDigitize.
  //
  // Examples: with fIfDigitize =2 and fDigitizeThresholds[2]={ 100, 150}
  //  value   53.5 -> 0
  //  value  104.9 -> 1
  //  value  208.3 -> 2
  //
  // JB 2013/08/29

  if ( fIfDigitize==0 ) {
    Warning("Digitize", "There is no thresholds settings to digitize --> returning 0");
    return 0;
  }

  Int_t digital_signal=0;
  Int_t iThreshold=0;

  while( aValue>=fDigitizeThresholds[iThreshold] && iThreshold<fIfDigitize ){
    digital_signal++;
    iThreshold++;
  }

  return digital_signal;

}

//______________________________________________________________________________at(
//

void DPlane::DigitizeMatrix() {

  // Digitize full matrix.
  // The pixels getting a value of 0, are discarded from the list.
  //
  // JB 2013/08/29

  if( fDebugPlane ) printf("DPlane:DigitizeMatrix Plane %d with %d fired pixels will be digitized.\n", fPlaneNumber, (Int_t)fListOfPixels->size());

 std::vector<DPixel*> temporaryList; // pointer to temporary list of hit pixel
  DPixel *aPixel;
  Int_t digitalValue;

  // Generate list of fired pixels after digitization
  for (Int_t tci = 0; tci < (Int_t)fListOfPixels->size(); tci++) { // loop over pixels
    aPixel = fListOfPixels->at(tci);
    digitalValue = Digitize( aPixel->GetPulseHeight() );
    if ( digitalValue>0 ) {
      aPixel->SetRawValue( digitalValue );
      aPixel->SetPulseHeight( digitalValue );
      temporaryList.push_back( aPixel);
      if( fDebugPlane>3 ) printf("DPlane:DigitizeMatrix  pixel %d with index %d, has been digitized to raw value %.1f and pulse height %.1f, -> temporary pixel list has %d elements\n", tci, aPixel->GetPixelIndex(), aPixel->GetRawValue(), aPixel->GetPulseHeight(), (Int_t)temporaryList.size());
    }
    else { // if pixel not selected, delete the object (otherwise not deleted)
      delete aPixel;
    }
  } // end over pixels

  // Replace original list with new list containing only fired pixels (sparsification)
  fListOfPixels->clear();
  //  for (Int_t tci = 0; tci < (Int_t)temporaryList.size(); tci++) { // loop over pixels
  //    aPixel = temporaryList.at(tci);
  //    fListOfPixels->push_back( aPixel );
  //  }
  fListOfPixels->insert( fListOfPixels->begin(), temporaryList.begin(), temporaryList.end());
  fPixelsN = (Int_t)fListOfPixels->size();

  if( fDebugPlane ) printf("DPlane:DigitizeMatrix After digitization, plane %d has now %d fired pixels.\n", fPlaneNumber, (Int_t)fListOfPixels->size());

  temporaryList.clear();
}

//______________________________________________________________________________at(
//

void DPlane::find_hits(){

  // -+-+- Searches hits in plane
  // then submit the analysis
  //
  // Orders the strips or pixels by either pulseheight or pulseheight ratio,
  //  the method may differ depending on the type of plane (Strip or Pixel)
  //  and the readout type,
  // then try to see if each strips may be used as a seed to make a hit
  //
  // Last modified JB 2009/05/12
  // Last modified JB 2009/08/21 deal with binary hit finding (AnalysisMode==3)
  // Last modified JB 2010/10/06 call to DPixel::GetPulseHeightToNoise for cut
  // Last modified JB 2012/08/18 management of non-sparsified clustering with DStrip objects
  // Last modified AP 2014/07 addition of hit finder option

  if( fDebugPlane>1 ) printf("DPlane: finding hits in plane %d with readout=%d analysis=%d over %d pixels\n", fPlaneNumber, fReadout, fAnalysisMode, fPixelsN);

  fHitsN = 0;

  std::vector<int> SeedPixelsList;
  SeedPixelsList.clear();

  if(fHitFinder == 1 && GetAnalysisMode() == 2) {
    // Here there is a new algorithm for cluster reconstruction with pixelated sensor with analogue readout
    // The first step is to find the local maxima on the pixel map (seed pixels)
    // Only select those seed pixels passing the selection requirements: passing the threshold on seed-charge or seed S/N
    // Then perform the analysis to add pixels to this seed (clustering) as usual.

    //Get the list of time-stamps in this event
    std::vector<int> ListOfTS;
    ListOfTS.clear();
    for (Int_t tci = 0; tci < fPixelsN; tci++){
      int ts = fListOfPixels->at(tci)->GetTimestamp();
      bool IsNotIn = true;
      for(int itstmp=0;itstmp<int(ListOfTS.size());itstmp++) {
        if(ts == ListOfTS[itstmp]) {
          IsNotIn = false;
          break;
        }
      }
      if(!IsNotIn) continue;

      ListOfTS.push_back(ts);
    }
    //Order time-stamp list from lowest to highest
    for(int iii=2;iii<=int(ListOfTS.size());iii++) {
      for(int jjj=0;jjj<=int(ListOfTS.size())-iii;jjj++) {
        int ts_jjj   = ListOfTS[jjj];
        int ts_jjjp1 = ListOfTS[jjj+1];

        if(ts_jjj > ts_jjjp1) {
          int  ts_aux     = ListOfTS[jjj];
          ListOfTS[jjj]   = ListOfTS[jjj+1];
          ListOfTS[jjj+1] = ts_aux;
        }
      }
    }

    //Initialize variables to find the local maxima in the pixel matrix
    int   IdxInList_ColRow[ListOfTS.size()][fStripsNu][fStripsNv];
    float Value_ColRow[ListOfTS.size()][fStripsNu][fStripsNv];
    //float SON_ColRow[ListOfTS.size()][fStripsNu][fStripsNv];
    for(int its = 0; its < int(ListOfTS.size()); its++) { // begin time-stamp loop
      for(int icol = 0; icol < fStripsNu; icol++) { // begin column loop
        for(int irow = 0; irow < fStripsNv; irow++) { // begin row loop
          IdxInList_ColRow[its][icol][irow] = -1;
          Value_ColRow[its][icol][irow]     = -1.0e+10;
          //SON_ColRow[its][icol][irow]       = -1.0e+10;
        } // end row loop
      } // end column loop
    } // end time-stamp loop

    //Fill variables separated in time-stamp
    for (Int_t tci = 0; tci < fPixelsN; tci++) { // being loop over pixels
      int   ts  = fListOfPixels->at(tci)->GetTimestamp();
      int   col = fListOfPixels->at(tci)->GetPixelColumn();
      int   row = fListOfPixels->at(tci)->GetPixelLine();
      float val = fListOfPixels->at(tci)->GetPulseHeight();
      //float sOn = fListOfPixels->at(tci)->GetPulseHeightToNoise();
      //cout << "col = " << col << ", row = " << row << ", tci = " << tci << ", index = " << col + row*GetStripsNu() << endl;

      for(int its=0;its<int(ListOfTS.size());its++) { // begin time-stamp loop
        if(ts == ListOfTS[its]) {
          IdxInList_ColRow[its][col][row] = tci;
          Value_ColRow[its][col][row]     = val;
          //SON_ColRow[its][col][row]       = sOn;
          break;
        }
      } // end time-stamp loop
    } //end loop over pixels

    if(fDebugPlane>3) {
      for(int its = 0; its < int(ListOfTS.size()); its++) {
        for(int icol = 0; icol < fStripsNu; icol++) {
          for(int irow = 0; irow < fStripsNv; irow++) {
            if(IdxInList_ColRow[its][icol][irow] == -1) cout << "DPlane::WARNING: (col,row,ts) = (" << icol << "," << irow << "," << ListOfTS[its] << ") has index = -1" << endl;
          }
        }
      }
    }

    //Now find the local maxima
    for(int its = 0; its < int(ListOfTS.size()); its++) {  // begin time-stamp loop

      for(int icol=0;icol<fStripsNu;icol++) { // begin column loop
        for(int irow=0;irow<fStripsNv;irow++) { // begin row loop
          if(IdxInList_ColRow[its][icol][irow] < 0) continue;

          //Loop over the pixels around current pixel to check if it is a local maxima
          bool IsLocalMax = true;
          for(int i=0;i<3;i++) {
            int col_test = icol + i - 1;
            if(col_test < 0 || col_test > fStripsNu-1) continue;
            for(int j=0;j<3;j++) {
              int row_test = irow + j - 1;
              if(row_test < 0 || row_test > fStripsNv-1) continue;

              if(icol == col_test && irow == row_test) continue;

              if(Value_ColRow[its][icol][irow] < Value_ColRow[its][col_test][row_test]) {
                IsLocalMax = false;
                break;
              }

            }
            if(!IsLocalMax) break;
          }
          if(!IsLocalMax) continue;

          //Now check if local maxima pixel passes additional selection requirements
          if(fListOfPixels->at(IdxInList_ColRow[its][icol][irow])->GetPulseHeightToNoise() > fCut->GetSeedPulseHeightToNoise() &&
             fListOfPixels->at(IdxInList_ColRow[its][icol][irow])->GetNoise()              < fCut->GetMaximalNoise()           &&
             fListOfPixels->at(IdxInList_ColRow[its][icol][irow])->GetNoise()              > 0.0) {

            //If local maximum passes additional requirements put it on list for further analysis
            SeedPixelsList.push_back(IdxInList_ColRow[its][icol][irow]);
          }

        } // end row loop
      } // end column loop
    }  // end time-stamp loop

    ListOfTS.clear();

    //Order possible seed pixels by charge from highest to lowest
    for(int iii=2;iii<=int(SeedPixelsList.size());iii++) {
      for(int jjj=0;jjj<=int(SeedPixelsList.size())-iii;jjj++) {
        double charge_jjj   = fListOfPixels->at(SeedPixelsList[jjj])->GetPulseHeight();
        double charge_jjjp1 = fListOfPixels->at(SeedPixelsList[jjj+1])->GetPulseHeight();
        if(charge_jjj < charge_jjjp1) {
          int aux_idx           = SeedPixelsList[jjj];
          SeedPixelsList[jjj]   = SeedPixelsList[jjj+1];
          SeedPixelsList[jjj+1] = aux_idx;
        }
      }
    }

    if(fDebugPlane > 1) {
      for(int iii=0;iii<int(SeedPixelsList.size());iii++) {
        int col,row;
        col = fListOfPixels->at(SeedPixelsList[iii])->GetPixelColumn();
        row = fListOfPixels->at(SeedPixelsList[iii])->GetPixelLine();

        cout << "Found local maximum at (col,row) = (" << col << "," << row << ")"
        << ", index = " <<  SeedPixelsList[iii]
        << ", charge = " << fListOfPixels->at(SeedPixelsList[iii])->GetPulseHeight()
        << ", S/N = " << fListOfPixels->at(SeedPixelsList[iii])->GetPulseHeightToNoise()
        << endl;
      }
    }



  } // end of condition HitFinder == 1 and AnalysisMode == 2
  else {
    //If sensor is not analogue output or HitFinder algorithm is not 1 then put in list to check all the pixels
    for (Int_t tci = 0; tci < fPixelsN; tci++) SeedPixelsList.push_back(tci);
  }

  Bool_t *tested = new Bool_t[fPixelsN];
  for (Int_t tci = 0; tci < fPixelsN; tci++) tested[tci] = kFALSE;

  int potential_seed_counter = 0;

  Bool_t hitOK   = kTRUE; // true while hits are found
  while ( hitOK == kTRUE && fHitsN < fHitMax ) { // Main loop to find hits

    Int_t  seed = -1; // seed undefined by default
    hitOK  = kFALSE;  // set false by default for this search

    //for (Int_t tci = 0; tci < fPixelsN; tci++){ // loop over hit pixels
    if(fHitFinder == 1 && GetAnalysisMode() == 2 && potential_seed_counter < (int)(SeedPixelsList.size())) {
      if(!fListOfPixels->at(SeedPixelsList[potential_seed_counter])->Found()) seed = SeedPixelsList[potential_seed_counter];

      if(seed < 0) hitOK  = kTRUE;

      if(fDebugPlane > 1) {
	int col,row;
	col = fListOfPixels->at(SeedPixelsList[potential_seed_counter])->GetPixelColumn();
	row = fListOfPixels->at(SeedPixelsList[potential_seed_counter])->GetPixelLine();

	cout << endl;
	cout << "DPlane::hind_hits(): test seed pixel with index " << SeedPixelsList[potential_seed_counter] << " (col,row) = (" << col << "," << row << "), has index " << seed << " for further analysis!" << endl;
	cout << endl;
      }

      potential_seed_counter++;
    }
    else {
      for(Int_t iseed = 0; iseed < int(SeedPixelsList.size()); iseed++){ // loop over hit pixels
        Int_t tci = SeedPixelsList[iseed];
        //================
        // This loop on strips (tci), sort them in decreasing "significance",
        // in order to find hits also with decreasing significance.
        // This significance may be the PulseHeightToNoise ratio or the Pulseheight
        // of the strip.
        // Seed is the index of the current most significant strip.
        // For one loop on all strips, the next most significant strip is checked to built a hit.
        //================

        if (fListOfPixels->at(tci)->Found() == kFALSE &&
            tested[tci] == kFALSE &&
            fListOfPixels->at(tci)->GetPulseHeightToNoise() > fCut->GetSeedPulseHeightToNoise() &&
            fListOfPixels->at(tci)->GetNoise()<fCut->GetMaximalNoise() ) { // test if already found and not tested as seed (JB 2010/10/25)

          // Algo for STRIP detector
          // fAnalysisMode = 0,1
          // condition on STRIP only modified by JB, 2007, June
          // Remember that tci is the current strip index and seed the current most significant strip
          // Treat specific cases first, then the general one
          if ( fAnalysisMode<2 ){
            if( seed < 0 ) {
              seed = tci;
            }
            else {
              seed = (
                      fabs(fListOfPixels->at(seed)->GetPulseHeight())
                      > fabs(fListOfPixels->at(tci)->GetPulseHeight())
                      ) ? seed : tci;
            }
          } //end if fAnalysisMode<2

          // Algo for PIXEL detectors with analogue data
          // fAnalysisMode == 2
          // condition on PIXEL modified by JB, 2007, June
          // condition on tested pixel added by JB 2010/10/25
          else if ( fAnalysisMode==2 ){
            if( fDebugPlane>3 &&
               fListOfPixels->at(tci)->GetPulseHeightToNoise() > 0) printf("DPlane: finding (analysisMode %d) hits try pixel %d with PulseHeightToNoise %.1f PulseHeight %.1f (already found? %d or tested? %d), for now seed is %d\n", fAnalysisMode, tci, fListOfPixels->at(tci)->GetPulseHeightToNoise(), fListOfPixels->at(tci)->GetPulseHeight(), fListOfPixels->at(tci)->Found(), tested[tci], seed);
            if( seed < 0 ) {
              seed = tci;
            }
            else {
              seed = (
                      fabs(fListOfPixels->at(tci)->GetPulseHeight())
                      > fabs(fListOfPixels->at(seed)->GetPulseHeight())
                      ) ? tci : seed;
            }
          } //end if fAnalysisMode==2

          // Algo for PIXEL detectors with binary data
          // fAnalysisMode == 3
          // All pusleheight is 1 so all pixels are seed candidates
          // JB 2009/08/21
          else if ( fAnalysisMode==3 ){
            if( fDebugPlane>3) printf("DPlane: finding hits try pixel %d with Pulseheight %f\n", tci, fListOfPixels->at(tci)->GetPulseHeight());

            seed = tci;
            break;

          } //end if fAnalysisMode==3

          // for other fAnalysisMode, algo unknown !
          // condition modified by JB, 2007, June
          else {
            printf("DPlane::Find_hits WARNING analysis mode %d UNKNOWN !!\n", fAnalysisMode);
          } //end test on fAnalysisMode

        }  // end test if already found

      } //end loop over strips
    }


    if( seed > -1 ) { // if a seed is defined


      if( fDebugPlane>1 ) printf("DPlane: finding hits found potential seed pixel %d with pulse %.3f and SN %.3f (<?> %f)\n", seed, fListOfPixels->at(seed)->GetPulseHeight(), fListOfPixels->at(seed)->GetPulseHeightToNoise(), fCut->GetSeedPulseHeightToNoise());

      fListOfPixels->at(seed)->SetFound(kTRUE); // mark the strip as found
      tested[seed] = kTRUE; // mark the strip as already tested for seed

      if ( fListOfPixels->at(seed)->GetPulseHeightToNoise() > fCut->GetSeedPulseHeightToNoise() ) { // pixel class knows by itself when to return PH or PH/Noise, JB 2010/10/05

        Int_t stPhys = fListOfPixels->at(seed)->GetPixelColumn()+fListOfPixels->at(seed)->GetPixelLine()*fStripsNu;

        // for non zero-suppressed read-out
        if( fReadout<100 && !fIfDigitize ) { // new condition to exclude digital-emulated data which, JB 2013/08/29
          //cout << " using analyse dstrip " << endl ;
          if(fHitFinder == 0) {
            //Reseach region clustering algorith
            hitOK = fHit[fHitsN]->Analyse( GetStrip( stPhys));
          }
          else if(fHitFinder == 1) {
            //Iterative clustering algorithm
            bool IsBigCluster = false;
            int MaxClusterSize = fc->GetPlanePar(fPlaneNumber).MaxNStrips;
            hitOK = fHit[fHitsN]->Analyse_Iterative( GetStrip( stPhys),IsBigCluster,MaxClusterSize);
            if(IsBigCluster) cout << "  DHit: Found the big cluster at Evt " << fSession->GetCurrentEventNumber()-1 << endl << endl;
          }
          else {
            //Reseach region clustering algorith as default:
            hitOK = fHit[fHitsN]->Analyse( GetStrip( stPhys));
          }
        }
        // for zero-suppressed read-out
        // hit finder option added, 2014/07 AP
        else {
          if(fHitFinder == 0) {
            //Reseach region clustering algorith
            hitOK = fHit[fHitsN]->Analyse( seed, fListOfPixels);
          }
          else if(fHitFinder == 1) {
            //Iterative clustering algorithm
            bool IsBigCluster = false;
            int MaxClusterSize = fc->GetPlanePar(fPlaneNumber).MaxNStrips;
            hitOK = fHit[fHitsN]->Analyse_Iterative( seed, fListOfPixels,IsBigCluster,MaxClusterSize);
            if(IsBigCluster) cout << "  DHit: Found the big cluster at Evt " << fSession->GetCurrentEventNumber()-1 << endl << endl;
          }
          else if(fHitFinder == 2){
            // Compare the distance from real center of gravity to the pixel position with a search radius to associate this pixel
            hitOK = fHit[fHitsN]->Analyse_2_cgo( seed, fListOfPixels);
          }
          else {
            //Reseach region clustering algorith as default:
            hitOK = fHit[fHitsN]->Analyse( seed, fListOfPixels);
          }

          //Setting hit resolution. Only for digitized output
          double u,v,col,lin;
          u = fHit[fHitsN]->GetPositionUhit();
          v = fHit[fHitsN]->GetPositionVhit();
          ComputeStripPosition_UVToColRow(u,v,col,lin);

          TVector2 HitResolution(0.0,0.0);
          GetHitResolution(col,lin,fHit[fHitsN]->GetStripsInCluster(),HitResolution);
          fHit[fHitsN]->SetResolutionUVhit(HitResolution.X(),HitResolution.Y());
        }


        if ( hitOK == kTRUE ) {
          // If non-sparsified readout, it is required to mark the object
          //  DPixel to found from their corresponding DStrip object.
          // Note that we set all pixels even if seed shall have been set found
          //  already. But seed may have changed...
          // This is not required otherwise because method Analyze with DPixel
          //  does it already.
          if( fReadout<100 && !fIfDigitize ) { // new condition to exclude digital-emulated data which, JB 2013/08/29
            if( fDebugPlane>1 ) printf("DPlane: Set found status to the %d strips of hit %d.\n", fHit[fHitsN]->GetStripsInCluster(), fHitsN );
            if(fHitFinder != 1) {
              for( Int_t iStrip=0; iStrip<fHit[fHitsN]->GetStripsInCluster(); iStrip++ ) {
                fListOfPixels->at( fHit[fHitsN]->GetMinor(iStrip)->GetPixelIndex() )->SetFound(kTRUE);
                if( fDebugPlane>3 ) printf("    neighbour %d, strip %d (pixel %d) found status set to %d.\n",
                                           iStrip,
                                           fHit[fHitsN]->GetMinor(iStrip)->GetStripIndex(),
                                           fHit[fHitsN]->GetMinor(iStrip)->GetPixelIndex(),
                                           fListOfPixels->at( fHit[fHitsN]->GetMinor(iStrip)->GetPixelIndex() )->Found()
                                           );
              }
            }

          }
          fHitsN++;
        }
        else if( (fAnalysisMode==2 || fAnalysisMode==3) && seed<fPixelsN ) {
          // trick to continue searching hits even if current seed was rejected
          // up to the last pixel
          // JB 2010/10/25
          // AnalysisMode=3 added to the exception
          // SS 2012/02/16
          hitOK = kTRUE;
        }

      } //end cut on seed values

    } // end if a seed is defined

  } // End Main loop to find hits

  delete[] tested; // reduce memory leakage, BH 2013/08/21

  if( fDebugPlane>1 ) printf("         %d hits found\n", fHitsN);

  return;

}
//______________________________________________________________________________
//
DR3 DPlane::DeformedLocalPoint(DR3& aPlaneCoordinate){

  // Use the model for the plane deformation
  //  to change the w coordinate of the given point in the local plane frame.
  // Note that only w is changed, (u,v) are unchanged.
  //
  // JB 2014/04/21
  // ==> call DPrecAlign equivalent since 2015/10/31

  return fPrecAlign->DeformedLocalPoint( aPlaneCoordinate);

//  DR3 aDeformedPoint = aPlaneCoordinate;
//  double unorm = aPlaneCoordinate(0);
//  aDeformedPoint += DR3( 0., 0., fTool.CLof7LegendrePol( &unorm, fUDeformationCoef) );
//  double vnorm = aPlaneCoordinate(1);
//  aDeformedPoint += DR3( 0., 0., fTool.CLof7LegendrePol( &vnorm, fVDeformationCoef) );
//  if(fDebugPlane) {
//    printf("DPlane::DeformedLocalPoint plane %d deformation applied.\n", fPlaneNumber);
//    printf("  from: %.1f, %.1f, %1.f to %.1f, %.1f, %1.f\n", aPlaneCoordinate(0), aPlaneCoordinate(1), aPlaneCoordinate(2), aDeformedPoint(0), aDeformedPoint(1), aDeformedPoint(2));
//  }
//
//  return aDeformedPoint;
}


//______________________________________________________________________________
//
DR3 DPlane::PlaneToTracker(DR3& aPlaneCoordinate){

  // converts a point given in the Plane coordinate sytem (f-system) UVW
  // to the tracker coordinate system (e-system) XYZ.
  //
  // JB 2010/11/25
  // Modified: JB 2014/04/21 for potential deformation
  //  => cancelled, since taken care of DPrecAlign since 2015/10/31

  return fPrecAlign->TransformHitToTracker( aPlaneCoordinate);

  // Not useful anymore since taken care into DPrecAlign directly
  //     // Take into account deformation if required
  //     DR3 aPoint = aPlaneCoordinate;
  //     if( fIfDeformation ) {
  //       DR3 aPoint = DeformedLocalPoint( aPlaneCoordinate);
  //       if (fDebugPlane) {
  //         printf("DPlane::PlaneToTracker plane %d deformation applied.\n", fPlaneNumber);
  //         printf("  from: %.1f, %.1f, %1.f to %.1f, %.1f, %1.f\n", aPlaneCoordinate(0), aPlaneCoordinate(1), aPlaneCoordinate(2), aPoint(0), aPoint(1), aPoint(2));
  //       }
  //     }
  //     return fPrecAlign->TransformHitToTracker( aPoint);

}


//______________________________________________________________________________
//
DR3 DPlane::TrackerToPlane(DR3& aTrackerCoordinate){

  // From a 3D point in the tracker coordinates (e-system) (X,Y,Z),
  // compute the same 3D point in the plane coordinates (f-system) (U,V, Z)
  // Note that no deformation is taken into account here,
  //   as if the tracker coordinate is the intersection of a track
  //   with the plane at normal incidence.
  //  -> If you need for a general track-plane intersection mechanism,
  //      use DPlane::Intersection( DTrack *aTrack)
  //
  // JB 2010/11/25

  return fPrecAlign->TransformHitToPlane( aTrackerCoordinate);

}


//_____________________________________________________________________________

DR3 DPlane::Intersection(DTrack *aTrack) {

  // Return the intersection of a track with this plane in the Plane frame.
  // If required, take into account deformation:
  //  1) get the intersection of the track with the plane considered flat,
  //  2) use intersection(u,v) to get the deformation height from the parametrisation,
  //  3) add this height to the intersection(w) = new(w)
  //  4) compute new(u,v) from the new(w) and the track slope
  //
  // Use new DPrecAlign mechanism, JB 2010/11/25
  // Modified: JB 2014/04/21, take into account deformation
  //  => cancelled, since taken care of DPrecAlign since 2015/10/31

  DR3 aFlatPoint = fPrecAlign->TransformTrackToPlane( aTrack->GetLinearFit().GetOrigin()(0), aTrack->GetLinearFit().GetOrigin()(1), aTrack->GetLinearFit().GetOrigin()(2), aTrack->GetLinearFit().GetSlopeZ()(0), aTrack->GetLinearFit().GetSlopeZ()(1));

  return aFlatPoint;

  // the following is now useless since deformation taken care in DPrecAlign

  // Deformation
  //     if( fIfDeformation ) {
  //       DR3 aDeformedPoint = DeformedLocalPoint( aFlatPoint);
  //       DR3 trackSlopeInPlane = fPrecAlign->RotateToPlane( aTrack->GetLinearFit().GetSlopeZ() );
  //       if (fDebugPlane) {
  //         printf("DPlane::Intersection plane %d deformation applied.\n", fPlaneNumber);
  //         printf("  from: %.1f, %.1f, %1.f to %.1f, %.1f, %1.f\n", aFlatPoint(0), aFlatPoint(1), aFlatPoint(2), aDeformedPoint(0), aDeformedPoint(1), aDeformedPoint(2));
  //       }
  //       Double_t deltaZU = aDeformedPoint(2)/tan(fc->GetPlanePar(fPlaneNumber).Tilt(1)) - aFlatPoint(2);
  //       Double_t deltaZV = (aDeformedPoint(2) -aFlatPoint(2));///tan(fc->GetPlanePar(fPlaneNumber).Tilt(0));
  //    if((fc->GetPlanePar(fPlaneNumber).Tilt(2)*180./M_PI) < 180. && abs(fc->GetPlanePar(fPlaneNumber).Tilt(2)*180./M_PI) > 175. ) {
  //    aDeformedPoint -= DR3( trackSlopeInPlane(0)*deltaZU, trackSlopeInPlane(1)*deltaZV, 0.);
  //    }
  //    else{
  //    aDeformedPoint += DR3( trackSlopeInPlane(0)*deltaZU, trackSlopeInPlane(1)*deltaZV, 0.);
  //    }
  //    // Double_t deltaZ = aDeformedPoint(2)-aFlatPoint(2);
  //    //aDeformedPoint += DR3( (trackSlopeInPlane(0))*deltaZ, trackSlopeInPlane(1)*deltaZ, 0.);
  //       if (fDebugPlane) {
  //         printf("  then with deltaZ=(%.1f,%.1f) and thetaPlane=%.1f to %.1f, %.1f, %1.f\n", deltaZU, deltaZV, fc->GetPlanePar(fPlaneNumber).Tilt(2)*180./M_PI, aDeformedPoint(0), aDeformedPoint(1), aDeformedPoint(2));
  //       }
  //       return aDeformedPoint;
  //     }
  //
  //     // No deformation (= flat plane)
  //     else {
  //       return aFlatPoint;
  //     }

}


//______________________________________________________________________________
//
void DPlane::SetPositionU(Float_t posU)
{
  fPositionU = posU;
}

void DPlane::SetPositionV(Float_t posV)
{
  fPositionV = posV;
}

void DPlane::SetPositionZ(Float_t posZ)
{
  (*fPosition)(2) = posZ;
}

void DPlane::SetTiltW(Float_t tiltW)
{
  fTiltW = tiltW;
}

void DPlane::ChangePositionByOffset(DR3& offset)
{
  //VR 2014/06/29
  *fPosition+=offset;
  fPrecAlign->SetTranslation( *fPosition);
}

void DPlane::ChangePositionByValue(DR3& newvalue)
{
  //VR 2014/06/29
  *fPosition=newvalue;
  fPrecAlign->SetTranslation( *fPosition);
}

void DPlane::ChangeTiltByOffset(DR3& offset)
{
  //VR 2014/06/29
  *fTilt+=offset;
  fPrecAlign->SetRotations( *fTilt);
}

void DPlane::ChangeTiltByValue(DR3& newvalue)
{
  //VR 2014/06/29
  *fTilt=newvalue;
  fPrecAlign->SetRotations( *fTilt);
}

void DPlane::PrintPosition(void)
{
  //VR 2014/06/29
  cout << "    PositionX = " << (*fPosition)(0)/1000 << " ; PositionY = " << (*fPosition)(1)/1000 << " ; PositionZ = " << (*fPosition)(2)/1000 << " [mm]" << endl;
}

void DPlane::PrintTilt(void)
{
  //VR 2014/06/29
  cout << "    TiltX = " << -TMath::RadToDeg()*(*fTilt)(2) << " ; TiltY = " << -TMath::RadToDeg()*(*fTilt)(1) << " ; TiltZ = " << -TMath::RadToDeg()*(*fTilt)(0) << " [°]" << endl;
}



//______________________________________________________________________________
//
/*
 DR3 DPlane::Rotate_PlaneToTracker(DR3& aPlaneCoordinate){

 // functions deprecated USE NEW DPRECALIGN MECHANISM (JB 2010/11/25)

 // converts a point given in the Plane coordinate sytem (f-system)
 // to the tracker coordinate system (e-system).
 // For that consider the position of the plane in the e-system
 // and the tilting angle(s) in the e-system.
 // The transformation rotates u,v,w using the rotation matrix
 // which gives a point in the e-system. Later the position of the Plane
 // in the e-system can be added.
 // result = postion(of Plane in e-system) + rotation(of point given in f-system)
 // the rotation matrix rot_ij is defined via the unit vectors e_j for the
 // e-system and f_k for the f-system:
 // rot_jk = e_j * f_k       ; * is inner product between both vectors
 // 980110: just implement one angle, rotation around z-axis,
 // one can generalize later for three angle rotation with e.g. euler angles

 DR3    rot[3];
 DR3    hp;     // position in xyz coordinates

 Float_t c[3];
 Float_t s[3];

 for(Int_t i=0 ; i<3 ; i++) {
 c[i] = cos((*fTilt)(i)) ;
 s[i] = sin((*fTilt)(i)) ;
 }

 rot[0].SetValue( c[0]*c[2] + s[0]*s[1]*s[2] ,
 s[0]*c[2] - c[0]*s[1]*s[2] ,
 c[1]*s[2]                   );

 rot[1].SetValue(-s[0]*c[1] ,
 c[0]*c[1] ,
 s[1]       );

 rot[2].SetValue(-s[2]*c[0] + s[0]*s[1]*c[2] ,
 -s[2]*s[0] - c[0]*s[1]*c[2] ,
 c[1]*c[2]                   );


 rot[0] *= aPlaneCoordinate(0);
 rot[1] *= aPlaneCoordinate(1);
 rot[2] *= aPlaneCoordinate(2);

 hp  = rot[0];
 hp += rot[1];
 hp += rot[2];


 return hp;
 }
 */
//______________________________________________________________________________
//
/*
 DR3 DPlane::Rotate_TrackerToPlane(DR3& aTrackerCoordinate){

 // functions deprecated USE NEW DPRECALIGN MECHANISM (JB 2010/11/25)

 // From a 3D point in the tracker coordinates (e-system) (X,Y,Z),
 // compute the same 3D point in the plane coordinates (f-system) (U,V, Z)
 // WITHOUT OFFSET !!!
 //
 // irot is the inverted matrix to rot (see above)

 DR3   hp(aTrackerCoordinate);     // hit position in  Tracker Coordinates (X,Y,Z)
 DR3   irot[3];                    // this is the rotation matrix


 Float_t c[3];
 Float_t s[3];

 for(Int_t i=0 ; i<3 ; i++) {
 c[i] = cos((*fTilt)(i)) ;
 s[i] = sin((*fTilt)(i)) ;
 }

 irot[0].SetValue( c[0]*c[2] + s[0]*s[1]*s[2] ,
 -s[0]*c[1]                  ,
 -s[2]*c[0] + s[0]*s[1]*c[2]  );

 irot[1].SetValue( s[0]*c[2] - c[0]*s[1]*s[2] ,
 c[0]*c[1]                  ,
 -s[2]*s[0] - c[0]*s[1]*c[2]  );

 irot[2].SetValue( c[1]*s[2]   ,
 s[1]        ,
 c[1]*c[2]    );

 irot[0] *= hp(0);
 irot[1] *= hp(1);
 irot[2] *= hp(2);

 hp  = irot[0];
 hp += irot[1];
 hp += irot[2];

 return hp;
 }
 */
//______________________________________________________________________________
//
void DPlane::AddMCGeneratedHit(DR3 aPosition,DR3 aResolution,Int_t hitMC)
{

  if(fHitsN == fHitMax) {
    cout << endl;
    cout << "Number of hits " << fHitsN << " is at maximum " << fHitMax << endl;
    cout << "Doing nothing!!!" << endl;
    cout << endl;
  }

  fHit[fHitsN]->SetHitPosition(aPosition,aResolution);
  fHit[fHitsN]->SetMCHitID(hitMC);
  fHitsN++;

  return;

}
//______________________________________________________________________________
//
Int_t DPlane::Compare( const TObject * obj) const {
  // QL 04/06/2016
  // to enable Sort method in a TList
  // Sorted by comparing zPosition of the plane center

  if (this == obj)
       return 0;
  DR3 posThis = *fPosition;
  DR3 posObj  = ((DPlane*)obj)->GetPosition();
  if (posThis(2) > posObj(2))
       return 1;
  else
       return -1;
}
//______________________________________________________________________________
//
void DPlane::Print(const Option_t*) const
{
  printf("Plane %i ", fPlaneNumber);
  fPosition->Print();

}
//______________________________________________________________________________
//
void  DPlane::GetHitResolution(int col, int lin, int ClusterMult,TVector2 &HitResolution)
{

  //AP 2016/07/27: Function to find the hit resolution depending on it position in plane

  int MyClusterMult = ClusterMult;

  int idx_reg = GetRegion(col,lin);
  //Once the region is found look if there is a multiplicity dependent resolution in the region
  if(fc->GetPlanePar(fPlaneNumber).PlanePerformancesList[idx_reg].ResolutionU.size() > 0) {
    //Get the cluster multiplicity
    if(MyClusterMult > int(fc->GetPlanePar(fPlaneNumber).PlanePerformancesList[idx_reg].ResolutionU.size())) {
      //If cluster multiplicity is higher than the multiplicity dependent resolution list, use the las entry to set up the resolutions on U and V
      MyClusterMult = fc->GetPlanePar(fPlaneNumber).PlanePerformancesList[idx_reg].ResolutionU.size();
    }
    HitResolution.Set(fc->GetPlanePar(fPlaneNumber).PlanePerformancesList[idx_reg].ResolutionU[ClusterMult-1],
		      fc->GetPlanePar(fPlaneNumber).PlanePerformancesList[idx_reg].ResolutionV[ClusterMult-1]);
  }
  else if(fc->GetPlanePar(fPlaneNumber).PlanePerformancesList[idx_reg].GlobalPlaneResolutionU != -1) {
    //There is defined a global resolution in the region different for U and V
    HitResolution.Set(fc->GetPlanePar(fPlaneNumber).PlanePerformancesList[idx_reg].GlobalPlaneResolutionU,
		      fc->GetPlanePar(fPlaneNumber).PlanePerformancesList[idx_reg].GlobalPlaneResolutionV);
  }
  else if(fc->GetPlanePar(fPlaneNumber).PlanePerformancesList[idx_reg].GlobalPlaneResolution != -1) {
    //There is defined a global resolution in the region the same for U and V
    HitResolution.Set(fc->GetPlanePar(fPlaneNumber).PlanePerformancesList[idx_reg].GlobalPlaneResolution,
		      fc->GetPlanePar(fPlaneNumber).PlanePerformancesList[idx_reg].GlobalPlaneResolution);
  }
  else {
    cout << endl;
    cout << "ERROR: No resolution defined for region " << idx_reg+1 << " of plane " << fPlaneNumber << ". Check your inputs. Exiting now!!!" << endl;
    cout << endl;
    assert(false);
  }

  return;

}
//______________________________________________________________________________
//
int   DPlane::GetRegion(int col, int lin)
{
  //AP 2016/07/27: Function returns the regions index for position col,lin
  int idx_reg = 0;

  for(int ireg=0;ireg<int(fSession->GetSetup()->GetPlanePar(fPlaneNumber).PlanePerformancesList.size());ireg++) {
    //There is a list of regions in which the resolution is defined.
    //Get the region in which the hit is in.
    //If the (col,lin) is in no region, use the multiplicity dependent resolution of region 0
    if(col >= fSession->GetSetup()->GetPlanePar(fPlaneNumber).PlanePerformancesList[ireg].Region.R_col[0] &&
       col <= fSession->GetSetup()->GetPlanePar(fPlaneNumber).PlanePerformancesList[ireg].Region.R_col[1] &&
       lin >= fSession->GetSetup()->GetPlanePar(fPlaneNumber).PlanePerformancesList[ireg].Region.R_lin[0] &&
       lin <= fSession->GetSetup()->GetPlanePar(fPlaneNumber).PlanePerformancesList[ireg].Region.R_lin[1]) {
      idx_reg = ireg;
      break;
    }
  }

  return idx_reg;

}
//______________________________________________________________________________
//
int   DPlane::GenerateMCMultiplicity(int col, int lin)
{
  //AP 2016/07/27: Function to generate a MC multiplicity based in measurements

  int ClusterMult = -999;
  int idx_reg = GetRegion(col,lin);
  if(fSession->GetSetup()->GetPlanePar(fPlaneNumber).PlanePerformancesList[idx_reg].ResolutionU.size() > 0) {
    double prob = rand->Uniform(0.0,1.0);
    for(int imult=0;imult<int(fSession->GetSetup()->GetPlanePar(fPlaneNumber).PlanePerformancesList[idx_reg].MultProbCumul.size());imult++) {
      if(prob <= fSession->GetSetup()->GetPlanePar(fPlaneNumber).PlanePerformancesList[idx_reg].MultProbCumul[imult]) ClusterMult = imult+1;
      break;
    }
  }

  return ClusterMult;

}
//______________________________________________________________________________
//
void  DPlane::MCHitsTruthMatching(void)
{

  //AP 2016/07/27: Function to perform the truth matching of the reconstructed hits

  for(int ihit=0;ihit<fHitsN;ihit++) {
    int MCHitID, NpixelsMCHitID;
    MCInfoHolder->DoHitTruthMatching(fHit[ihit],MCHitID,NpixelsMCHitID);

    fHit[ihit]->SetMCHitID(MCHitID);
    fHit[ihit]->SetStripsFromMCHitID(NpixelsMCHitID);
  }

  return;

}
//______________________________________________________________________________
//
void  DPlane::CheckNonDigitizedMCHits(void)
{
  //AP 2016/07/27: In case there are non digitized hits in this plane, this function generate a hit by smearing with the plane sigma_sp

  // Loop over all the particles in event
  //cout << endl;
  //cout << "Plane " << fPlaneNumber << endl;
  //cout << "Nparticles = " << MCInfoHolder->GetNSimParticles() << endl;
  for(int ipart=0;ipart<MCInfoHolder->GetNSimParticles();ipart++) {
    int FirstHitIdx = MCInfoHolder->GetASimParticle(ipart).FirstHitIdx;
    // Loop over all the hits of this particle
    for(int ihit=0;ihit<MCInfoHolder->GetASimParticle(ipart).NHits;ihit++) {
      //Check that the hit has not being digitized
      if(MCInfoHolder->GetASimHit(FirstHitIdx + ihit).Npixels != -1) continue;

      //Check that the hit is on the current plane
      if(fPlaneNumber != MCInfoHolder->GetASimHit(FirstHitIdx + ihit).sensorID+1) continue;

      //Get the hit local position
      double u = MCInfoHolder->GetASimHit(FirstHitIdx + ihit).PosAveUVmm.X()*1.0e+3;
      double v = MCInfoHolder->GetASimHit(FirstHitIdx + ihit).PosAveUVmm.Y()*1.0e+3;

      //Get the closest pixel to this poistion
      double col, row;
      ComputeStripPosition_UVToColRow(u,v,col,row);

      //Generate a cluster multiplicity from measurement if available
      int ClusterMult = GenerateMCMultiplicity(col,row);

      //Get a hit resolution based on its position and multiplicity
      TVector2 MyHitResolution(0.0,0.0);
      GetHitResolution(col,row,ClusterMult,MyHitResolution);

      //Now will smear the position
      double u_rec, v_rec;
      u_rec = rand->Gaus(u,MyHitResolution.X());
      v_rec = rand->Gaus(v,MyHitResolution.Y());

      //Add this hit to the list of hits
      DR3 HitPosition(u_rec,v_rec,0.0);
      DR3 HitResolution(MyHitResolution.X(),MyHitResolution.Y(),0.0);
      AddMCGeneratedHit(HitPosition,HitResolution,FirstHitIdx + ihit);

      //cout << "hit " << ihit+1 << " of particle " << ipart+1 << " of this event was not digitized."
      //     << " Adding a hit with position (u,v) = (" << HitPosition(0) << "," << HitPosition(1) << ") um, and resolution (" << HitResolution(0) << "," << HitResolution(1) << ") um"
      //   << endl;

    }
  }
  //cout << endl;

  return;

}
//______________________________________________________________________________
//
