// @(#)MAlign.cxx  $:$Id: MAlign.cxx v.1 2005/10/02 18:03:46 sha Exp $
// Author: A. Shabetai 
// Last Modified: JB 2009/07/19
// Last Modified: JB 2009/08/26 AlignTracker
// Last Modified: JB 2009/09/08 AlignTracker
// Last Modified: JB 2009/09/18 AlignTracker
// Last Modified: JB 2010/08/25 AlignTracker
// Last Modified: JB 2010/09/05 AlignMimosa
// Last Modified: JB 2010/11/25 AlignMimosa
// Last Modified: JB 2010/12/14 AlignMimosa
// Last Modified: JB 2011/03/14 AlignTracker
// Last Modified: JB 2011/04/05 AlignTracker
// Last Modified: JB 2011/06/18 AlignMimosa
// Last Modified: JB 2011/06/21 AlignTracker
// Last Modified: JB 2011/07/21 introduce fSession member pointer
// Last Modified: JB 2011/07/25 AlignTracker
// Last Modified: JB 2011/09/05 AlignMimosa
// Last Modified: LC 2012/09/06 AlignTrackerMinuit
// Last Modified: JB 2013/06/10 AlignTracker & MimosaAlignAnalysis
// Last Modified: BH 2013/08/20, warning corrections from clang
// Last Modified: JB 2013/08/22, AlignPrecMimosa for histo range
// Last Modified: LC 2013/09/10 AlignLadderMV
// Last Modified: LC 2013/11/15 AlignLadderMV2
// Last Modified: LC 2013/01/20 AlignTrackerMinuit.
// Last Modified: LC 2013/01/20 AlignTrackerMinuitLadder.
// Last Modified: LC 2013/01/20 fcnLadder.
// Last Modified: LC 2013/01/20 AlignPrecMimosaLadder.
// Last Modified: LC 2014/12/05 AlignTrackerMinuitLadder (custom objects)
// Last Modified: LC 2014/12/05 fcnLadder -> Print Residuals in tracker frame.
// Last Modified: LC 2014/12/08 Merge Branch with MV alignement
// Last Modified: LC 2014/12/08 fcnLadder, fcnMinivectors, fcnCutData, AlignLadderMV, associateMiniVectors.
// Last Modified: LC 2014/12/08 AlignMimosaLadder, AlignMimosaMV, AlignMimosaAssociationMV.
// Last Modified: LC 2014/12/15 Remove Class DMonteCarlo et DHitMonteCarlo : infos MC included in DPixel et DHit.
// Last Modified: LC 2014/12/18 AlignLadder commented (Outdated).
// Last Modified: LC 2014/12/20 Now we use MiniVector Object + big code simplifications
// Last Modified: LC 2014/12/20 Now we use Minuit2 with mini-vector Alignement
// Last Modified: AP 2015/03/09 Added hit resolution U and V to chi2 function for alignement (fcn and fcnLadder)


  /////////////////////////////////////////////////////////////
  //                                                         //
  //  Mimosa chip and ref. system alignement class           //
  //  This class performs the alignement of the tested       //
  //  Mimosa chip and of the associated test telescope.      //
  //                                                         //
  //                                                         //   
  ////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <sstream>

#include "TROOT.h"
#include "MAlign.h"
#include "DGlobalTools.h"
#include "DSession.h"
#include "DLine.h"
#include "DHit.h"
#include "DMiniVector.h"
#include "MMillepede.h"

// include Minuit2
#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
//#include "Math/GSLMinimizer.h"
#include "Math/Functor.h"
//#include "Math/GSLSimAnMinimizer.h"
//#include "Math/MinimizerOptions.h"
//#include "Minuit2/Minuit2Minimizer.h"
#include "MGlobalAlign.h"

MimosaAlignAnalysis* MimosaAlignAnalysis::fgInstance = 0;

ClassImp(MimosaAlignAnalysis)
  

//______________________________________________________________________________
MimosaAlignAnalysis::MimosaAlignAnalysis( DSession *aSession)
{ 

  // MimosaAlignAnalysis default constructor
  //
  // Modified: JB 2011/07/21 to include Session member pointer
  // Modified: JB 2013/06/11 to take geometrical limits for tracks into account
  // Modified: JB 2013/07/14 to take chi2 limits for tracks into account

  fSession = aSession;
  fAlignement = fGeom =  0;
  fDu = fDv = 0; 
  fCnv = fCcr = 0;
  fMyfit = 0;
  fLimitsX[0] = 0.; // JB 2013/06/10
  fLimitsX[1] = 0.;
  fLimitsY[0] = 0.;
  fLimitsY[1] = 0.;
  fSession->GetTrackGeoLimitsForAlign( fLimitsX[0], fLimitsX[1], fLimitsY[0], fLimitsY[1]);
  fChi2Limit = fSession->GetTrackChi2LimitForAlign(); // JB 2013/07/14
  
  fAlignment = new MAlignment(); // LC 20/12/24.

  if (fgInstance) Warning("MimosaAlignAnalysis", "object already instantiated");
  else            fgInstance = this;

  AlignDebug = fSession->GetDebug(); // JB 2009/09/18
}

MimosaAlignAnalysis::~MimosaAlignAnalysis() 
{
  delete fAlignment;
  delete fAlignement;
  delete fAlignement0;
  delete ladderToAlign;
  delete ladder0;
  delete fGeom;
  delete fCnv;
  delete fCcr;
  delete fDu;
  delete fDv;
  delete fDw;
  delete fDdu;
  delete fDdv;
  delete fDdw;
  delete fMyfit; 
}

//______________________________________________________________________________
//  
void MimosaAlignAnalysis::SetTrackChi2Limit( Double_t aChi2Limit)
{
  // Define the upper limit for the chi2 of tracks to be considered for alignment.
  //
  // JB 2013/07/14

  fChi2Limit = aChi2Limit;
  
  printf("--> Align procedure is restricted to tracks with chi2<=%f.\n", fChi2Limit);

}


//______________________________________________________________________________
//  
void MimosaAlignAnalysis::SetTrackGeoLimits(Double_t xmin, Double_t xmax, Double_t ymin, Double_t ymax)
{
  // Define geometrical limits of tracks to be considered for alignment.
  // Those limits are given in the tracker frame AND FOR Z=0.
  //
  // JB 2013/06/10
  
  fLimitsX[0] = xmin;
  fLimitsX[1] = xmax;
  fLimitsY[0] = ymin;
  fLimitsY[1] = ymax;
  
  printf("--> Align procedure is restricted to tracks with (at Z=0):");
  printf("  %.2f < X < %.2f   and   %.2f < Y < %.2f\n\n", fLimitsX[0], fLimitsX[1], fLimitsY[0], fLimitsY[1]);
  
}



//______________________________________________________________________________
//  
Bool_t MimosaAlignAnalysis::CheckTrackInGeoLimits( DTrack *aTrack)
{

  // Check the given track is located in the geometrical limits,
  //  return 1 if yes, 0 otherwise.
  // Do not consider limits if min is near the max (given in microns). 
  //
  // JB 2013/06/11
  
  Double_t trackX, trackY;

  trackX = aTrack->GetLinearFit().GetOrigin()(0);
  trackY = aTrack->GetLinearFit().GetOrigin()(1);

  if(  ( fLimitsX[1]-fLimitsX[0]>10. && (trackX<fLimitsX[0] || fLimitsX[1]<trackX) ) 
     || ( fLimitsY[1]-fLimitsY[0]>10. && (trackY<fLimitsY[0] || fLimitsY[1]<trackY) ) ) {
    if(AlignDebug>1) printf("MimosaAlignAnalysis::AlignTracker track at X=%.1f, Y=%.1f, Z=0 rejected because outside limits in X (%.1f-%.1f) or Y (%.1f-%.1f)\n", trackX, trackY, fLimitsX[0], fLimitsX[1], fLimitsY[0], fLimitsY[1]);
    return 0;
  }
  
  return 1;
}


//______________________________________________________________________________  
//
void MimosaAlignAnalysis::AlignTracker(const Double_t tiniBoundU,
				       const Double_t tiniBoundV,
				       Int_t nAlignEvents, 
				       Int_t nAdditionalEvents,
				       bool UseAllHits)
{
// Macro performs an alignement of the telescope in the coordinate system of 
// primary reference planes. Secondary planes are align relative to them. 
//
// This method fits only 2 shifts (vertical and horizontal) and 1 rotation angle
//  (axis perpendicular to plane).
//
// bool parameter UseAllHits is used to decide if doing aligment with all hits within a distance tiniBoundU/V from the track (UseAllHits = true), 
// or if using only the hit closest to the track within the tidiBoundU/V distance (UseAllHits = false). Default value is false.
//  
// Note that the types of plane alignes depends on the tracker alignement status:
//  0: align primary ref. (planeStatus=1) and secondary ref. (planeStatus=2) and DUT (planeStaus=3)
//  1: align secondary ref. (planeStatus=2) and DUT (planeStaus=3)
//  2: align DUT (planeStaus=3)
//
// ==> Calling gTAF->SetAlignStatus(aStatus) before this method changes which planes are aligned
// ==> Use gTAF->SetTrackChi2LimitForAlign( aLimit) to set chi2 upper limit to usable tracks
// ==> Use gTAF->SetTrackGeoLimitsForAlign( xmin, xmax, ymin, ymax) to set geometrical limits to usable tracks
//
// tracks built from primary reference planes are used to accumulate
// residues with hits (if hits are at a distance smaller than tBound) 
// from secondary reference planes. A given number
// (~200, hard coded in DAlign) of events is used for this and then the alignement 
// parameters (translation and rotation coeff.) are fit (done in DPlane and DAlign methods).
// Do this until the shift in translation parameters is below a limit around .5 microns,
// see parameter limit hard coded in this function. At each iteration the tBound distance
// to associate track and hits is automatically decreased (in Dplane and DAlign).
//
// Then a second number of events (the rest) is used to obtain the residu distribution 
// which is fit again (in this function) to check the result.
//
// Methods revised by JB to allow variable numbers of secondary reference planes, 2007 June
// modified 2008/10/19 for additional # events, JB
// modified 2009/05/13 for display of additional events, JB
// modified JB 2009/08/26 start track counter at 1
// modified JB 2009/09/08 multi-tracks in accumulation step
// modified JB 2009/09/18 force fRequiredHits for tracks when alignStatus==1 (suppressed 2011/06/21)
// modified JB 2010/08/25 add limit on tilt to decide to stop alignment
// modified JB 2010/11/25 replace notation tiltZ -> tiltW
// modified JB 2010/12/14 convert into degrees before updating the cfg file
// modified JB 2011/03/14 refine number of hits required for tracks
// modified JB 2011/04/05 update method and info for cfg file
// modified JB 2011/06/21 new management of tracker alignment status to take planeStatus=0 into account
// modified JB 2011/07/25 condition to display new params changed
// modified JB 2013/06/13 different bounds in U and V directions introduced
// modified JB 2013/06/13 Geometrical limits to consider tracks for alignment
// modified JB 2013/07/11 chi2 limit to consider tracks for alignment
// modified AP 2015/06/10 added bool parameter (UseAllHits) to decice if doing alignment with all hits (UseAllHits = true) or with the closest one (UseAllHits = false). Default value is false.

  Double_t limitShift=0.5; // microns, if the shift in position falls under this limit, stop the alignment (microns)
  Double_t limitTilt=0.1; // degrees JB 2010/08/25

  // requires init Session calls in order to run!

  DTracker *tTracker  =  fSession->GetTracker();

  Int_t  nSecondaryPlanes; // #planes to align
  Int_t *SecondaryPlaneNo; // list of planes to align

  gStyle->SetTitleSize(0.08,"xyz");
  gStyle->SetLabelSize(0.06,"xyz");

  if( tTracker->GetAlignmentStatus() == 0 ) { // all (prim.+sec.) references to be aligned
    nSecondaryPlanes = tTracker->GetNFixRef()+tTracker->GetNVarRef();
    SecondaryPlaneNo = new Int_t[nSecondaryPlanes];
    for( Int_t ip=0; ip<tTracker->GetNFixRef(); ip++ ) {
      SecondaryPlaneNo[ip] = tTracker->GetListFixRef()[ip];
    }
    for( Int_t ip=0; ip<tTracker->GetNVarRef(); ip++ ) {
      SecondaryPlaneNo[ip+tTracker->GetNFixRef()] = tTracker->GetListVarRef()[ip];
    }
  }
  else if( tTracker->GetAlignmentStatus() == 1 ) { // only secondary references to be aligned
    nSecondaryPlanes = tTracker->GetNVarRef();
    SecondaryPlaneNo = tTracker->GetListVarRef();
  }
  else { // only DUTs to be aligned
    nSecondaryPlanes = tTracker->GetNDUT();
    SecondaryPlaneNo = tTracker->GetListDUT();
  }

  DPlane **SecPlane = new DPlane*[nSecondaryPlanes];
  Bool_t *stopAlign = new Bool_t[nSecondaryPlanes]; // JB 2009/08/21

  Double_t *posU    = new Double_t[nSecondaryPlanes];
  Double_t *difU    = new Double_t[nSecondaryPlanes];
  Double_t *posUold = new Double_t[nSecondaryPlanes];

  Double_t *posV    = new Double_t[nSecondaryPlanes];
  Double_t *difV    = new Double_t[nSecondaryPlanes];
  Double_t *posVold = new Double_t[nSecondaryPlanes];

  Double_t *posZ    = new Double_t[nSecondaryPlanes];
  Double_t *difZ    = new Double_t[nSecondaryPlanes];
  Double_t *posZold = new Double_t[nSecondaryPlanes];
  
  Double_t *tiltW    = new Double_t[nSecondaryPlanes];
  Double_t *diftiltW = new Double_t[nSecondaryPlanes];
  Double_t *tiltWold = new Double_t[nSecondaryPlanes];
  
  Double_t *tiltY    = new Double_t[nSecondaryPlanes];
  Double_t *diftiltY = new Double_t[nSecondaryPlanes];
  Double_t *tiltYold = new Double_t[nSecondaryPlanes];

  Double_t *tiltX    = new Double_t[nSecondaryPlanes];
  Double_t *diftiltX = new Double_t[nSecondaryPlanes];
  Double_t *tiltXold = new Double_t[nSecondaryPlanes];

  printf("\nAlignTracker ALIGNING tracker (%d secondary reference planes) with:\n  %d events for fit,\n  hits-to-track max distance %.1f/%.1f,\n  track within geometrical  limits x[%.1f, %.1f] y[%.1f, %.1f],\n  max chi2 for tracks %f (0 means no limit),\n  stopping when shift changes below %.3f um and tilt change below %.3f rad.\n\n", nSecondaryPlanes, nAlignEvents, tiniBoundU, tiniBoundV, fLimitsX[0], fLimitsX[1], fLimitsY[0], fLimitsY[1], fChi2Limit, limitShift, limitTilt);

  /* ------------------------
  //useless, JB, 2007 June
  Int_t PrimaryPlaneNo[4];
  DPlane *PriPlane[4];
  Int_t iPrimaryPlane=0;
  Int_t iSecondaryPlane=0;
  for(Int_t ipl=1; ipl<9; ipl++){
    if(tTracker->GetPlane(ipl)->GetStatus()==1){
      //primary plane
      PrimaryPlaneNo[iPrimaryPlane++]=ipl;
    }
    else if(tTracker->GetPlane(ipl)->GetStatus()==2){
      //secondary plane
      SecondaryPlaneNo[iSecondaryPlane++]=ipl;
    }
  }
  --------------------------- */

  DTrack *track=0;

  TH2F **hposu  = new TH2F*[nSecondaryPlanes];
  TH2F **hposv  = new TH2F*[nSecondaryPlanes];
  TH2F **htiltz = new TH2F*[nSecondaryPlanes];
  Double_t minDiffU = -5.;
  Double_t maxDiffU = 5.;
  Double_t minDiffV = -5.;
  Double_t maxDiffV = 5.;
  Double_t minDiffTiltW = -.1;
  Double_t maxDiffTiltW = .1;
  Char_t hname[50];
  Char_t htitle[50];
  for( Int_t iPl=0; iPl<nSecondaryPlanes; iPl++) {
    stopAlign[iPl] = false;

    sprintf( hname, "hu%d", iPl);
    sprintf( htitle, "Position in U");
    hposu[iPl] = new TH2F( hname,htitle,nAlignEvents/100,0,nAlignEvents,600,minDiffU,maxDiffU);
    hposu[iPl]->SetMarkerStyle(20);
    hposu[iPl]->SetMarkerSize(.8);
    hposu[iPl]->SetMarkerColor(iPl+1);
    hposu[iPl]->GetXaxis()->SetTitle("# of event");
    hposu[iPl]->GetYaxis()->SetTitle("Position of secondary planes");

    sprintf( hname, "hv%d", iPl);
    sprintf( htitle, "Position in V");
    hposv[iPl] = new TH2F( hname,htitle,nAlignEvents/100,0,nAlignEvents,600,minDiffV,maxDiffV);
    hposv[iPl]->SetMarkerStyle(21);
    hposv[iPl]->SetMarkerSize(.8);
    hposv[iPl]->SetMarkerColor(iPl+1);
    hposv[iPl]->GetXaxis()->SetTitle("# of event");
    hposv[iPl]->GetYaxis()->SetTitle("Position of secondary planes");

    sprintf( hname, "htilt%d", iPl);
    sprintf( htitle, "Tilt in W");
    htiltz[iPl] = new TH2F( hname,htitle,nAlignEvents/100,0,nAlignEvents,200,minDiffTiltW,maxDiffTiltW);
    htiltz[iPl]->SetMarkerStyle(22);
    htiltz[iPl]->SetMarkerSize(.8);
    htiltz[iPl]->SetMarkerColor(iPl+1);
    htiltz[iPl]->GetXaxis()->SetTitle("# of event");
    htiltz[iPl]->GetYaxis()->SetTitle("Tilt of secondary planes");
  }


  for(Int_t i=0; i<nSecondaryPlanes; i++){ 
    SecPlane[i] =  tTracker->GetPlane(SecondaryPlaneNo[i]);
    SecPlane[i]->GetAlignment()->SetBoundings(tiniBoundU, tiniBoundV);
    posUold[i] = SecPlane[i]->GetPosition()(0);
    posVold[i] = SecPlane[i]->GetPosition()(1);
    posZold[i] = SecPlane[i]->GetPosition()(2);
    tiltWold[i]= SecPlane[i]->GetTilt()(0);
    tiltYold[i]= SecPlane[i]->GetTilt()(1);
    tiltXold[i]= SecPlane[i]->GetTilt()(2);
    difU[i] = 100000.; // necessary for init
    difV[i] = 100000.;
    diftiltW[i] = 360.;
    hposu[i]->Fill(0.,posUold[i]);
    hposv[i]->Fill(0.,posVold[i]);
    htiltz[i]->Fill(0.,tiltWold[i]);
    if(AlignDebug) printf("MimosaAlignAnalysis::AlignTracker got sec plane %d  pos(%.1f, %.1f) tiltZ %f\n", SecPlane[i]->GetPlaneNumber(), posUold[i], posVold[i], tiltWold[i]);
    //PriPlane[i] =  tTracker->GetPlane(PrimaryPlaneNo[i]); //useless, JB, 2007 June
  }
  // == information on who is primary/secondary plane is set in the config file of the run

  TCanvas *AlignMonitor = new TCanvas("AlignMonitor","align monitor",0,0,800,600);
  // removed because doesn't work with Root 5.28
//   Char_t head[50];
//   sprintf(head,"Alignment evolution");
//   cout << "Creating Pad" << endl;
//   TPaveLabel *hi=new TPaveLabel(1000,105,4000,120,head);
//   hi->SetFillColor(0);
//   hi->Draw();
  AlignMonitor->cd();
  AlignMonitor->Update();
  AlignMonitor->Divide(1,3);
  AlignMonitor->Modified();
  AlignMonitor->Draw();
  AlignMonitor->cd(1);
  TLegend *legend = new TLegend( .4,.7, .55,.95, "Planes:");
  for(Int_t i=0; i<nSecondaryPlanes; i++){
    sprintf( htitle, "%d", SecPlane[i]->GetPlaneNumber());
    legend->AddEntry(hposu[i], htitle ,"p");
  }

  //  cut on distance (track position with primary planes - position in secondary plane) methode arg
  
  if(AlignDebug>1) printf ("MimosaAlignAnalysis::AlignTracker starting the loop over %d events\n", nAlignEvents); // JB 2011/04/18
  fSession->SetEvents(nAlignEvents);
  Bool_t stop = false;
  Int_t counts=0;
  while(fSession->NextRawEvent() == kTRUE
	&& !stop
        //&& counts<nAlignEvents
	) {    

    counts = fSession->GetCurrentEventNumber();
    if(AlignDebug>1) cout << "Reading event " << counts << endl; // JB 2011/04/18
    //===================
    tTracker->Update();
    //===================
    if(AlignDebug>1) printf ("MimosaAlignAnalysis::AlignTracker event %d read and %d tracks found\n", fSession->GetCurrentEventNumber(), tTracker->GetTracksN()); // JB 2011/04/18
    //if ( fSession->GetCurrentEventNumber()% 50 == 0 ) printf ("%d Event Read\n", fSession->GetCurrentEventNumber() );
    //if (tTracker->GetPlanesStatus() >=1) {
	  
    //if (tTracker->GetTracksN() >= 1 && fSession->GetCurrentEventNumber()>1){ // if a track is found
    Int_t	iTrack = 1; // track counter starts at 1, JB 2009/08/26
    Bool_t      fitdone = kFALSE;
    while ( iTrack <= tTracker->GetTracksN() ) { // while there is still another track, JB 2009/07/17
      track = tTracker->GetTrack( iTrack++); //tTracker->GetPrincipalTrack();	

      // Reject tracks not in the geometrical limits at z=0,
      // JB 2013/06/10
      if( !CheckTrackInGeoLimits( track) 
        || ( fChi2Limit>0.1 && track->GetChiSquare()>fChi2Limit )
         ) {
        continue;
      }

      // align RS with given track
      for(Int_t i=0; i<nSecondaryPlanes; i++){ // loop on secondary ref planes
        if(AlignDebug>2) printf ("MimosaAlignAnalysis::AlignTracker aligning plane %d with track %d\n", SecPlane[i]->GetPlaneNumber(), iTrack); // JB 2011/04/18
        //====================
        // align in 2D if necessary, but tilt is taken from delta_U vs V only, JB 2007 June
        if( !stopAlign[i] ) fitdone = SecPlane[i]->Align(track, kTRUE, UseAllHits); // stop alignment when it is already good, JB 2009/08/21
//        if( !stopAlign[i] ) fitdone = SecPlane[i]->AlignAllHits(track, kTRUE); // stop alignment when it is already good, JB 2009/08/21
        //====================
			  
        if(AlignDebug>2) printf ("MimosaAlignAnalysis::AlignTracker status plane %d: stop? %d, mode %d enough? %d\n", SecPlane[i]->GetPlaneNumber(), stopAlign[i], SecPlane[i]->GetAnalysisMode(), SecPlane[i]->GetAlignment()->Enough2D());
        if (  !stopAlign[i] && fitdone ) {  // condition changed (the others were obsolete) JB 2011/07/25
          posU[i] = SecPlane[i]->GetPosition()(0);	   
          posV[i] = SecPlane[i]->GetPosition()(1);	   
          posZ[i] = SecPlane[i]->GetPosition()(2);	   
          tiltW[i] = SecPlane[i]->GetTilt()(0)*180./M_PI;
          tiltY[i] = SecPlane[i]->GetTilt()(1)*180./M_PI;
          tiltX[i] = SecPlane[i]->GetTilt()(2)*180./M_PI;
          difU[i]=fabs(posU[i]-posUold[i]);
          difV[i]=fabs(posV[i]-posVold[i]);
          difZ[i]=fabs(posZ[i]-posZold[i]);
          diftiltW[i]=fabs(tiltW[i]-tiltWold[i]);
          diftiltY[i]=fabs(tiltY[i]-tiltYold[i]);
          diftiltX[i]=fabs(tiltX[i]-tiltXold[i]);
          if(AlignDebug > 2) {
            cout << " plane " << i << " posX=" << posU[i] << " posXold= " << posUold[i];
            cout << ";  posY=" << posV[i] << " posYold= " << posVold[i];
            cout << ";  posZ=" << posZ[i] << " posZold= " << posZold[i];
            cout << ";  tilt Z " << tiltW[i] << "  tilt Z old=" << tiltWold[i] << "deg" << endl;
            cout << ";  tilt Y " << tiltY[i] << "  tilt Y old=" << tiltYold[i] << "deg" << endl;
            cout << ";  tilt X " << tiltX[i] << "  tilt X old=" << tiltXold[i] << "deg" << endl;
          }
          
          // draw change in position instead of position itself, JB 2013/05/01
          hposu[i]->Fill(counts,posU[i]-posUold[i]);
          hposv[i]->Fill(counts,posV[i]-posVold[i]);
          htiltz[i]->Fill(counts,tiltW[i]-tiltWold[i]);
          
          // Adjust range of histos displaying the change in the parameters
          if( posU[i]-posUold[i] > maxDiffU ) maxDiffU = posU[i]-posUold[i];
          else if( posU[i]-posUold[i] < minDiffU ) minDiffU = posU[i]-posUold[i];
          hposu[0]->SetBins(nAlignEvents/100,0,nAlignEvents,600,minDiffU,maxDiffU);
          if( posV[i]-posVold[i] > maxDiffV ) maxDiffV = posV[i]-posVold[i];
          else if( posV[i]-posVold[i] < minDiffV ) minDiffV = posV[i]-posVold[i];
          hposv[0]->SetBins(nAlignEvents/100,0,nAlignEvents,600,minDiffV,maxDiffV);
          if( tiltW[i]-tiltWold[i] > maxDiffV ) maxDiffV = tiltW[i]-tiltWold[i];
          else if( tiltW[i]-tiltWold[i] < minDiffV ) minDiffV = tiltW[i]-tiltWold[i];
          htiltz[0]->SetBins(nAlignEvents/100,0,nAlignEvents,200,minDiffTiltW,maxDiffTiltW);
          
          posUold[i]=posU[i];
          posVold[i]=posV[i];
          posZold[i]=posZ[i];
          tiltWold[i]=tiltW[i];
          tiltYold[i]=tiltY[i];
          tiltXold[i]=tiltX[i];
				  
          
          // Redraw all planes
          AlignMonitor->cd(1);
          gPad->SetGrid(1,1);
          hposu[0]->Draw("P");
          for(Int_t i2=1; i2<nSecondaryPlanes; i2++){
            hposu[i2]->Draw("PSAME");
          }
          legend->Draw();
          AlignMonitor->cd(2);
          gPad->SetGrid(1,1);
          hposv[0]->Draw("P");
          for(Int_t i2=1; i2<nSecondaryPlanes; i2++){
            hposv[i2]->Draw("PSAME");
          }
          AlignMonitor->cd(3);
          gPad->SetGrid(1,1);
          htiltz[0]->Draw("P");
          for(Int_t i2=1; i2<nSecondaryPlanes; i2++){
            htiltz[i2]->Draw("PSAME");
          }
          AlignMonitor->Update();
          
          stopAlign[i] = ((difU[i]<limitShift) & (difV[i]<limitShift)) & (diftiltW[i]<limitTilt); // works even with 1D because difV=0 then, limitTilt added JB 2010/08/25
          printf("MAlign: plane %d, difX=%.2f < %.2f ? %d, difY=%.2f < %.2f ? %d, (difZ=%.2f), difTiltZ=%.3f < %.3f ? %d, (difTiltY=%.3f, difTiltX=%.3f), stop=%d\n", i, difU[i], limitShift, (difU[i]<limitShift), difV[i], limitShift, (difV[i]<limitShift), difZ[i], diftiltW[i], limitTilt, diftiltW[i]<limitTilt, diftiltY[i], diftiltX[i], stopAlign[i]); // JB 2009/08/21
          
        } // end if enough event for new position
			  
      } // end loop on sec ref planes
		  
      // compute STOP condition = shift for all planes below limit
      // modified JB 2009/08/21
      stop = true;
      for(Int_t i=0; i<nSecondaryPlanes; i++){
        stop &= stopAlign[i];
      }
      if( stop ) printf("  Alignement can stop now\n");
      if(AlignDebug > 2) printf("MAlign: alignement stop=%d\n", stop);
		  
    } // end if a track is found

  } // end of while
  
 
  Int_t rn = fSession->GetRunNumber();
  Char_t AlgnMonitorPs[50];
  sprintf(AlgnMonitorPs,"AlignMonitor_%d.root",rn);
  AlignMonitor->SaveAs(AlgnMonitorPs); 
 

  //-----------------------------------
  // Now accumulate some more events in the Tree 
  // to fit final alignment results
  printf("\n\n\n  Reading %d additional events for alignment fit results\n", nAdditionalEvents);

  // Create display and histos for each secondary planes
  for(Int_t i=0; i<nSecondaryPlanes; i++){ // loop on secondary ref planes
    //SecPlane[i]->GetAlignment()->SetBounding(tiniBound); // re-use initial bound, more when debugging
    SecPlane[i]->GetAlignment()->CreateDisplay( -SecPlane[i]->GetStripPitch()(0)*SecPlane[i]->GetStripsNu()/2., SecPlane[i]->GetStripPitch()(0)*SecPlane[i]->GetStripsNu()/2., -SecPlane[i]->GetStripPitch()(1)*SecPlane[i]->GetStripsNv()/2., SecPlane[i]->GetStripPitch()(1)*SecPlane[i]->GetStripsNv()/2.);
  }

  // Loop on additional events
  // modified to cope with multi-tracks, JB 2009/09/08
  fSession->SetEvents(nAdditionalEvents);
  while(fSession->NextRawEvent() == kTRUE) {    

    tTracker->Update();

    for ( Int_t it=1; it <= tTracker->GetTracksN(); it++ ) { // loop on tracks
      track = tTracker->GetTrack( it);

      // Reject tracks not in the geometrical limits at z=0,
      // JB 2013/06/10
      if( !CheckTrackInGeoLimits( track) ) {
        continue;
      }
      
      for(Int_t i=0; i<nSecondaryPlanes; i++){ // loop on secondary ref planes
        SecPlane[i]->Align(track, kFALSE, UseAllHits); //store with no fit
//        SecPlane[i]->AlignAllHits(track, kFALSE); //store with no fit
      } // end loop on sec ref planes

    } // end loop on tracks

  } // end of while
  
  // Now display the filled histos
  printf("------------------------------------------------------------\n");
  printf("--------------------- Alignment RESULT --------------------\n");
  printf("------------------------------------------------------------\n");
  Char_t  RsAlignPs[50];
  for(Int_t i=0; i<nSecondaryPlanes; i++){ // loop on secondary ref planes
    cout << "----------------------------------------------" << endl;
    cout << " MAlign: displaying alignment result for plane " << SecPlane[i]->GetPlaneNumber() << ", over " << SecPlane[i]->GetAlignment()->GetCounts() << " events, with bounds U=" << SecPlane[i]->GetAlignment()->GetBoundingU() << " V=" << SecPlane[i]->GetAlignment()->GetBoundingV() << endl; // JB 2013/06/10
    SecPlane[i]->GetAlignment()->Display();
    sprintf(RsAlignPs,"RsAlign_%d_pl%d.root", rn, SecPlane[i]->GetPlaneNumber());
    SecPlane[i]->GetAlignment()->GetDisplay()->SaveAs(RsAlignPs); // JB 2010/12/09
    SecPlane[i]->GetAlignment()->SaveDisplay(RsAlignPs); // JB 2014/04/06  
    SecPlane[i]->ShowAlignment();
    SecPlane[i]->GetAlignment()->ShowStoredResolution();
  }


  //-----------------------------------
  // call the script to modify the runXXX.cfg file ::: modified by JB 2009/07/19
  // Handle any number of planes
  // Only if there was really a fit (nAlignEvents>0)

  if( nAlignEvents>0 ) {
    cout << endl << "***************************************" << endl;
    cout << endl << "Applying results ...." << endl;
    Char_t update_cfg_string[1000];
    sprintf(update_cfg_string,"Update_conf_align2D %s/run%d.cfg", fSession->GetConfigPath().Data(), fSession->GetRunNumber());
    for(Int_t i=0; i<nSecondaryPlanes; i++){ // loop on secondary ref planes
      sprintf( update_cfg_string, "%s %d %10.5f %10.5f %10.5f %10.5f %10.5f %10.5f", update_cfg_string, SecPlane[i]->GetPlaneNumber(), SecPlane[i]->GetPrecAlignment()->GetTranslation()[0]/1000., SecPlane[i]->GetPrecAlignment()->GetTranslation()[1]/1000., SecPlane[i]->GetPrecAlignment()->GetTranslation()[2]/1000., SecPlane[i]->GetPrecAlignment()->GetRotations()[0]*TMath::RadToDeg(), SecPlane[i]->GetPrecAlignment()->GetRotations()[1]*TMath::RadToDeg(), SecPlane[i]->GetPrecAlignment()->GetRotations()[2]*TMath::RadToDeg()); // JB 2011/04/05
    }
    sprintf(update_cfg_string,"%s", update_cfg_string); // add "-d" at the end for debug
    cout<<update_cfg_string<<endl;
    gSystem->Exec(update_cfg_string);
  }
  

  //-----------------------------------
  // The End
  /*
  //clean up
  if(AlignMonitor) {delete AlignMonitor; AlignMonitor = 0;}
  if(RsAlign) {delete RsAlign; RsAlign = 0;}
  */

  //  fSession->Finish();
      
  
}

//______________________________________________________________________________
//
void MimosaAlignAnalysis::StudyDeformation(const Float_t tiniBound, Int_t nEvents, Bool_t fitMode, Float_t minU, Float_t maxU, Float_t minV, Float_t maxV){

    // Method to display the shape of the track-hit residual deviations.
    // Use the results stored from the method AlignTracker to display profiles of the residuals as a function of the position of the hit on the plane.
    //
    // When fitMode = 1, the method try to fit Legendre polynomials of the 7th order on the profile plots.
    //
    // BB 2014/05/20

    DTracker *tTrackerDeformation = fSession->GetTracker();
    Int_t nSecondaryPlanesDeformation = 0;
    Int_t *SecondaryPlanesNoDeformation = NULL;
    DTrack *trackDeformation=0;
    Int_t rnDeformation = fSession->GetRunNumber();

    if(tTrackerDeformation->GetAlignmentStatus()>=2){
        nSecondaryPlanesDeformation = tTrackerDeformation->GetNDUT();
        SecondaryPlanesNoDeformation = tTrackerDeformation->GetListDUT();
    }
    else{
        cout << endl;
        cout << "/!\\-------------------------------------------------/!\\" << endl;
        cout << " The study of sensor's shape is only available for DUT." << endl;
        cout << "      Please, set SetAlignStatus to 2 or higher.       " << endl;
        cout << "/!\\-------------------------------------------------/!\\" << endl;
        cout << endl;
        return;
    }
    
    DPlane **SecPlaneDeformation = new DPlane*[nSecondaryPlanesDeformation];

  //-----------------------------------
  // Accumulate events to show profile of 
  // residual as a function of the hit 
  // position and to fit final alignment results
  printf("\n\n\n  Reading %d events to display shape of DUT and to fit results\n", nEvents);

  // Create display and histos for each secondary planes
  
  fSession->SetEvents(nEvents);
  for(Int_t i=0; i<nSecondaryPlanesDeformation; i++){ // loop on secondary ref planes
      
    SecPlaneDeformation[i] =  tTrackerDeformation->GetPlane(SecondaryPlanesNoDeformation[i]);
    SecPlaneDeformation[i]->GetAlignment()->SetBoundings(tiniBound, tiniBound);
    SecPlaneDeformation[i]->GetAlignment()->CreateDisplayDeformation( -SecPlaneDeformation[i]->GetStripPitch()(0)*SecPlaneDeformation[i]->GetStripsNu()/2., SecPlaneDeformation[i]->GetStripPitch()(0)*SecPlaneDeformation[i]->GetStripsNu()/2., -SecPlaneDeformation[i]->GetStripPitch()(1)*SecPlaneDeformation[i]->GetStripsNv()/2., SecPlaneDeformation[i]->GetStripPitch()(1)*SecPlaneDeformation[i]->GetStripsNv()/2.);
  }

  // Loop on additional events
  // modified to cope with multi-tracks, JB 2009/09/08
 
  //fSession->SetEvents(nEvents);
  while(fSession->NextRawEvent() == kTRUE) {    

    tTrackerDeformation->Update();

    for ( Int_t it=1; it <= tTrackerDeformation->GetTracksN(); it++ ) { // loop on tracks
      trackDeformation = tTrackerDeformation->GetTrack( it);

      // Reject tracks not in the geometrical limits at z=0,
      // JB 2013/06/10
      if( !CheckTrackInGeoLimits( trackDeformation) ) {
        continue;
      }
      
      for(Int_t i=0; i<nSecondaryPlanesDeformation; i++){ // loop on secondary ref planes
	    SecPlaneDeformation[i]->Align(trackDeformation, kFALSE); //store with no fit
      } // end loop on sec ref planes

    } // end loop on tracks

  } // end of while
  
  // Now display the filled histos
  printf("\n------------------------------------------------------------\n");
  printf("----------------------- Sensor's shape ---------------------\n");
  printf("------------------------------------------------------------\n");
  Char_t  RsProfileAlignPs[50];
  Char_t  RsProfileAlignFittedPs[50];

  for(Int_t i = 0; i<nSecondaryPlanesDeformation; i++){ // loop on secondary ref planes
    cout << endl;
    cout << "------------------------------------------------" << endl;
    cout << " MAlign: displaying alignment result for plane " << SecPlaneDeformation[i]->GetPlaneNumber() << ", over " << SecPlaneDeformation[i]->GetAlignment()->GetCounts() << " events, with bounds U = " << SecPlaneDeformation[i]->GetAlignment()->GetBoundingU() << " V = " << SecPlaneDeformation[i]->GetAlignment()->GetBoundingV() << endl; // JB 2013/06/10

    if(fitMode){
      SecPlaneDeformation[i]->GetAlignment()->FitDeformation(SecPlaneDeformation[i]->GetCoeffLegendreU(), SecPlaneDeformation[i]->GetCoeffLegendreV(), minU, maxU, minV, maxV);    
      SecPlaneDeformation[i]->GetAlignment()->DisplayDeformationFitted();
      sprintf(RsProfileAlignFittedPs, "RsAlign_%d_pl%d.root", rnDeformation, SecPlaneDeformation[i]->GetPlaneNumber());
      SecPlaneDeformation[i]->GetAlignment()->GetDisplayDeformation()->SaveAs(RsProfileAlignFittedPs);
    }
    else{
      SecPlaneDeformation[i]->GetAlignment()->DisplayDeformation();
      sprintf(RsProfileAlignPs, "RsAlign_%d_pl%d.root", rnDeformation, SecPlaneDeformation[i]->GetPlaneNumber());
      SecPlaneDeformation[i]->GetAlignment()->GetDisplayDeformation()->SaveAs(RsProfileAlignPs);
    }
    SecPlaneDeformation[i]->ShowAlignment();
  }

}
//______________________________________________________________________________
//

void MimosaAlignAnalysis::AlignTrackerMinuit(Bool_t modeAuto,
					     const Double_t tiniBound,
					     Int_t nAlignEvents,
					     Int_t nAlignHitsInPlane,
					     Int_t nAdditionalEvents,
					     Double_t chi2Limit,
					     bool UseAllHits)
{

  // Macro performs an alignement of the telescope in the coordinate system of 
  // primary reference planes. Secondary planes are align relative to them. 
  //
  // Over "nAlignEvents" events, 
  //  a fit will be performed every "nAlignHitsInPlane" associations hit-plane.
  // After those events, additional "nAdditionalEvents" events are taken 
  //  to estimate the residual.
  //
  // Use bool parameter (UseAllHits) to decide if
  // - Doing alignment with all the hits    within a tidiBound distance to the track (UseAllHits = true)
  // - Doing alignment with the closest hit within a tidiBound distance to the track (UseAllHits = false)
  // Default value of UseAllHits is true
  //
  // This method allows to fit all parameters: 3 shifts and 3 rotation angles.
  //
  // Note that the types of plane aligned depends on the tracker alignement status:
  //  0: align fixed ref. (planeStatus=1) and secondary ref. (planeStatus=2) and DUT (planeStaus=3)
  //  1: align secondary ref. (planeStatus=2) and DUT (planeStatus=3)
  //  2: align DUT (planeStatus=3)
  //
  // ==> Calling gTAF->SetAlignStatus(aStatus) before this method changes which planes are aligned
  // ==> Use gTAF->SetTrackChi2LimitForAlign( aLimit) to set chi2 upper limit to usable tracks
  // ==> Use gTAF->SetTrackGeoLimitsForAlign( xmin, xmax, ymin, ymax) to set geometrical limits to usable tracks
  //
  // New method : alignment made by mimuit algorithms.
  // The data are filled in DPlane::AlignPrec.
  //
  // LC 2012/09/06.
  //
  // modified JB 2013/06/13 Geometrical limits to consider tracks for alignment
  // modified JB+LC 2013/08/21 new conditions on #events accumulated to start fit

   std::cout<<std::endl;
   std::cout<<"AlignTracker with Minuit methods"<<std::endl;
   std::cout<<std::endl;

   if( modeAuto!=0 && modeAuto!=1 ) modeAuto = 1;

   // Keep the strongest chi2 upper limit
   if( fChi2Limit>0. && fChi2Limit < chi2Limit ) chi2Limit = fChi2Limit;
  
   std::cout<<"Automatic mode : "<<modeAuto<<std::endl;
   std::cout<<"Bound = "<<tiniBound<<std::endl;
   std::cout<<"Event number to align = "<<nAlignEvents<<std::endl;
   std::cout<<"Event number to print = "<<nAdditionalEvents<<std::endl;
   if( chi2Limit==0. ) {
     std::cout<<"No chi2 cut"<<std::endl;    
   }
   else {
     std::cout<<"Max track chi2 = "<<chi2Limit<<std::endl;
   }
   std::cout<<"Track geometrial limits x=["<<fLimitsX[0]<<", "<<fLimitsX[1]<<"] y=["<<fLimitsY[0]<<", "<<fLimitsY[1]<<"]"<<std::endl;
   std::cout<<std::endl;

  DTracker *tTracker  =  fSession->GetTracker();
  
  Int_t  nSecondaryPlanes; // #planes to align
  Int_t *SecondaryPlaneNo; // list of planes to align

  if( tTracker->GetAlignmentStatus() == 0 ) { // all (prim.+sec.) references to be aligned
    nSecondaryPlanes = tTracker->GetNFixRef()+tTracker->GetNVarRef();
    SecondaryPlaneNo = new Int_t[nSecondaryPlanes];
    for( Int_t ip=0; ip<tTracker->GetNFixRef(); ip++ ) {
      SecondaryPlaneNo[ip] = tTracker->GetListFixRef()[ip];
    }
    for( Int_t ip=0; ip<tTracker->GetNVarRef(); ip++ ) {
      SecondaryPlaneNo[ip+tTracker->GetNFixRef()] = tTracker->GetListVarRef()[ip];
    }
  }
  else if( tTracker->GetAlignmentStatus() == 1 ) { // only secondary references to be aligned
    nSecondaryPlanes = tTracker->GetNVarRef();
    SecondaryPlaneNo = tTracker->GetListVarRef();
  }
  else { // only DUTs to be aligned
    nSecondaryPlanes = tTracker->GetNDUT();
    SecondaryPlaneNo = tTracker->GetListDUT();
  }

  DPlane **SecPlane = new DPlane*[nSecondaryPlanes];
  Bool_t *stopAlign = new Bool_t[nSecondaryPlanes];

  DTrack *track=0;

  cout << " AlignTrackerMinuit will align " << nSecondaryPlanes << " planes." << endl;
  for(Int_t i=0; i<nSecondaryPlanes; ++i){ 
    SecPlane[i] =  tTracker->GetPlane(SecondaryPlaneNo[i]);
    SecPlane[i]->GetAlignment()->SetBounding(tiniBound);
    stopAlign[i] = kFALSE;
   }
  
  if(AlignDebug>1) printf ("MimosaAlignAnalysis::AlignTrackerMinuit starting the loop over %d events\n", nAlignEvents);
  fSession->SetEvents(nAlignEvents);
  Int_t counts=0;
 
  while(fSession->NextRawEvent() == kTRUE	) {    
   
    counts = fSession->GetCurrentEventNumber();
    
    if(AlignDebug>1) cout << "Reading event " << counts << endl;
   
    //===================
    tTracker->Update();
    //===================
    
    if(AlignDebug>1) printf ("MimosaAlignAnalysis::AlignTrackerMinuit event %d read and %d tracks found\n", fSession->GetCurrentEventNumber(), tTracker->GetTracksN()); // JB 2011/04/18

    Int_t iTrack = 1;

    while ( iTrack <= tTracker->GetTracksN() ) {   // while there is still another track
      track = tTracker->GetTrack( iTrack++); //tTracker->GetPrincipalTrack();	
 
      // Reject tracks not in the geometrical limits at z=0,
      // JB 2013/06/10
      if( !CheckTrackInGeoLimits( track) ) {
        continue;
      }
      
      for(Int_t i=0; i<nSecondaryPlanes; i++){ // loop on secondary ref planes
        
        if( chi2Limit == 0. || 
	    //(track->GetChiSquare() >= 0. && track->GetChiSquare() < chi2Limit) ) SecPlane[i]->AlignPrec(track, SecPlane[i]->GetAlignment()->GetBounding());
	    (track->GetChiSquare() >= 0. && track->GetChiSquare() < chi2Limit) ) SecPlane[i]->AlignPrec(track, SecPlane[i]->GetAlignment()->GetBounding(),UseAllHits);
        
        if(AlignDebug>2) std::cout<<"fSession->GetCurrentEventNumber() = "<<fSession->GetCurrentEventNumber()<<"   SecPlane[i]->GetPrecAlignment()->DataSize() : "<<  SecPlane[i]->GetPrecAlignment()->DataSize()<<"  trackChi2 = "<<tTracker->GetTrack( iTrack++)->GetChiSquare()<<std::endl;
        
        if( /*fSession->NextRawEvent() == kFALSE*/ SecPlane[i]->GetPrecAlignment()->DataSize()>=nAlignHitsInPlane && stopAlign[i]==kFALSE  ) { // if enough events for a fit, condition ==nAlignHitsInPlane changed to >=, JB+LC 2013/08/21
          
          if(AlignDebug) std::cout<<"DataSize() = "<<SecPlane[i]->GetPrecAlignment()->DataSize()<<std::endl;
          
          cout << endl << "============="<<endl<< " Alignment of plane " << SecPlane[i]->GetPlaneNumber() << " with MINUIT and bound=" << SecPlane[i]->GetAlignment()->GetBounding() << endl<<"============="<< endl << endl;
          AlignPrecMimosa(SecPlane[i]->GetPrecAlignment(), modeAuto, SecPlane[i]->GetAlignment()->GetBounding());
          
          SecPlane[i]->AlignPrecUpdateConfig();
          SecPlane[i]->ShowAlignment();
          
          if( modeAuto==0 ) { // if manuall mode, continue, JB 2013/08/29
            Double_t newBound = 0.;
            Char_t answer[100];
            while ( 0.<=newBound && newBound<0.1 ) {
              newBound = 0;
              cout << endl<<endl<<endl << "Choose a new minimal track-hit distance (um) for next iteration, set a negative number to stop alignment: " << endl<<endl;
              cin >> answer;
              newBound = atof( answer);
            }
            if( newBound<0. ) {
              stopAlign[i]=kTRUE;              
            }
            else {
              SecPlane[i]->GetAlignment()->SetBounding(newBound);
              SecPlane[i]->GetPrecAlignment()->ResetDataPoints();
            }
          }
          else { // if automatic mode, stop
            stopAlign[i]=kTRUE;
            cout << " --> will stop alignment for plane " << SecPlane[i]->GetPlaneNumber() << endl;
          }
          
        } // if enough event for a fit
        
      } // end loop on sec ref planes
      
    } // end if a track is found
    
  } // end of while
 
  Int_t rn = fSession->GetRunNumber();

  //---------------------------------------------
  // Now accumulate some more events in the Tree 
  // to fit final alignment results

  printf("\n\n\n  Reading %d additional events for alignment fit results\n", nAdditionalEvents);

  // Create display and histos for each secondary planes
  
  for(Int_t i=0; i<nSecondaryPlanes; i++){ // loop on secondary ref planes
  
    //SecPlane[i]->GetAlignment()->SetBounding(tiniBound); // re-use initial bound, more when debugging, commented JB 2013/08/29
    SecPlane[i]->GetAlignment()->CreateDisplay( -SecPlane[i]->GetStripPitch()(0)*SecPlane[i]->GetStripsNu()/2., SecPlane[i]->GetStripPitch()(0)*SecPlane[i]->GetStripsNu()/2., -SecPlane[i]->GetStripPitch()(1)*SecPlane[i]->GetStripsNv()/2., SecPlane[i]->GetStripPitch()(1)*SecPlane[i]->GetStripsNv()/2.);

   }

  // Loop on additional events
  
  fSession->SetEvents(nAdditionalEvents);
  
  while(fSession->NextRawEvent() == kTRUE) {    

    tTracker->Update();

    for ( Int_t it=1; it <= tTracker->GetTracksN(); it++ ) { // loop on tracks
      track = tTracker->GetTrack( it);

      // Reject tracks not in the geometrical limits at z=0,
      // JB 2013/06/10
      if( !CheckTrackInGeoLimits( track) ) {
        continue;
      }
      
      for(Int_t i=0; i<nSecondaryPlanes; i++){ // loop on secondary ref planes
	//SecPlane[i]->AlignPrec(track, SecPlane[i]->GetAlignment()->GetBounding());
	SecPlane[i]->AlignPrec(track, SecPlane[i]->GetAlignment()->GetBounding(),UseAllHits);

     } // end loop on sec ref planes

    } // end loop on tracks

  } // end of while

  // Now display the filled histos
  printf("------------------------------------------------------------\n");
  printf("--------------------- Alignment RESULT ---------------------\n");
  printf("------------------------------------------------------------\n");
 
  std::map<Int_t,std::vector<Int_t> > AlignedPlaneInLadder; // key = ladder, value = plane number.

  for(Int_t i=0; i<nSecondaryPlanes; i++){ // loop on secondary ref planes
    
    cout << "----------------------------------------------" << endl;
    cout << " Alignement result for ladder " << SecPlane[i]->GetLadderNumber() << std::endl;
    cout << " MAlign: displaying alignment result for plane " << SecPlane[i]->GetPlaneNumber() << ", over " << SecPlane[i]->GetAlignment()->GetCounts() << " events, with bound " << SecPlane[i]->GetAlignment()->GetBounding() << endl; // JB 2009/09/08
    if(i==0){

      TString dirName = "run";
      dirName += fSession->GetRunNumber();
 
      if( gSystem->mkdir(dirName,0)== -1 ) cout<<"Directory always exist or Cannot create directory "<<fSession->GetRunNumber()<<std::endl;
      if( gSystem->cd(dirName)== 0 ) cout<<"Cannot open repertory "<<fSession->GetRunNumber()<<std::endl; 
      
      }
    SecPlane[i]->GetAlignment()->Display( fSession->GetRunNumber() );
    SecPlane[i]->ShowAlignment();
    SecPlane[i]->GetAlignment()->ShowStoredResolution();
    
    if( i==nSecondaryPlanes-1 ) gSystem->cd("..");

   }
  
  //-----------------------------------
  // call the script to modify the runXXX.cfg file ::: modified by JB 2009/07/19
  // Handle any number of planes
  // Only if there was really a fit (nAlignEvents>0)
  
  if( nAlignEvents>0 ) {
    cout << endl << "***************************************" << endl;
    cout << endl << "Applying results ...." << endl;
    Char_t update_cfg_string[1000];
    sprintf(update_cfg_string,"Update_conf_align2D %s/run%d.cfg", fSession->GetConfigPath().Data(), fSession->GetRunNumber());
    for(Int_t i=0; i<nSecondaryPlanes; i++){ // loop on secondary ref planes
     
      sprintf( update_cfg_string, "%s %d %10.5f %10.5f %10.5f %10.5f %10.5f %10.5f", update_cfg_string, SecPlane[i]->GetPlaneNumber(), SecPlane[i]->GetPrecAlignment()->GetTranslation()[0]/1000., SecPlane[i]->GetPrecAlignment()->GetTranslation()[1]/1000., SecPlane[i]->GetPrecAlignment()->GetTranslation()[2]/1000., SecPlane[i]->GetPrecAlignment()->GetRotations()[0]*TMath::RadToDeg(), SecPlane[i]->GetPrecAlignment()->GetRotations()[1]*TMath::RadToDeg(), SecPlane[i]->GetPrecAlignment()->GetRotations()[2]*TMath::RadToDeg());
    }
    sprintf(update_cfg_string,"%s", update_cfg_string); // add "-d" at the end for debug
    cout<<update_cfg_string<<endl;
    gSystem->Exec(update_cfg_string);
  }

  //-----------------------------------
  // The End

}

//______________________________________________________________________________
//

void MimosaAlignAnalysis::AlignTrackerMinuitLadder(Bool_t modeAuto, const Double_t tiniBounds, Int_t nAlignEvents, Int_t nAlignHitsInPlane, Int_t nAdditionalEvents, Double_t chi2Limit)
{

// Macro performs an alignement of the telescope in the coordinate system of 
// primary reference planes. Secondary planes are align relative to them. 
//
// Note that the types of plane aligned depends on the tracker alignement status:
//  0: align fixed ref. (planeStatus=1) and secondary ref. (planeStatus=2) and DUT (planeStaus=3)
//  1: align secondary ref. (planeStatus=2) and DUT (planeStatus=3)
//  2: align DUT (planeStatus=3)
//
// ==> Calling gTAF->SetAlignStatus(aStatus) before this method changes which planes are aligned
//
// LC 2012/09/06.
//
// New method : alignment made by mimuit algorithms.
// The data are filled in DPlane::AlignPrec.
//

// Modified: LC, 2014/12/05 Custim Ladders.

  std::cout<<std::endl;
  std::cout<<"AlignTracker with Minuit methods"<<std::endl;
  std::cout<<std::endl;

  std::cout<<"Mode Automatique : "<<modeAuto<<std::endl;
  std::cout<<"Bound = "<<tiniBounds<<std::endl;
  std::cout<<"Event number to align = "<<nAlignEvents<<std::endl;
  std::cout<<"Event number to print = "<<nAdditionalEvents<<std::endl;
  std::cout<<std::endl;

  Double_t tiniBound = tiniBounds;

  DTracker *tTracker  =  fSession->GetTracker();
  
  Int_t  numberOfLadders = tTracker->GetNumberOfLadders();
  Int_t  numberOfPlanes  = tTracker->GetPlanesN();

  if(numberOfLadders==0) cout<<"ERROR !!! No ladder. Must have at least one ladder."<<endl;
  std::cout<<"number Of Ladders = "<<numberOfLadders<<std::endl;

  Int_t numberOfSimplePlane = numberOfPlanes;

  DLadder **myLadders = new DLadder*[numberOfLadders];
  Bool_t *stopAlignLadder = new Bool_t[numberOfLadders];

  DTrack *track=0;
  Int_t totalHitsInAlignment[numberOfLadders];

  for(Int_t j=0 ; j<numberOfLadders ; ++j) {

    myLadders[j] = tTracker->GetLadder(j+1);
    totalHitsInAlignment[j]=0;

    if( !(myLadders[j]->GetLadderType() == "Custom" || myLadders[j]->GetLadderType() == "custom") ) {
       std::cout<<"No custom ladder found."<<std::endl;
       return;
    }
    
    myLadders[j]->UpdateAlignment();
    stopAlignLadder[j] = kFALSE;
    numberOfSimplePlane -= myLadders[j]->GetNumberOfPlanes();
  }
 
  cout<<"Number of simple planes = "<<numberOfSimplePlane<<endl;
  
  if(AlignDebug>1) printf ("MimosaAlignAnalysis::AlignTracker starting the loop over %d events\n", nAlignEvents);
  
  fSession->SetEvents(nAlignEvents);
  Int_t counts=0;
 
  while(fSession->NextRawEvent() == kTRUE	) {    
   
    counts = fSession->GetCurrentEventNumber();
    
    if(AlignDebug>1) cout << "Reading event " << counts << endl;
   
    //===================
    tTracker->Update();
    //===================
    
    if(AlignDebug>1) printf ("MimosaAlignAnalysis::AlignTracker event %d read and %d tracks found\n", fSession->GetCurrentEventNumber(), tTracker->GetTracksN()); // JB 2011/04/18

    Int_t iTrack = 1;

    while ( iTrack <= tTracker->GetTracksN() ) {   // while there is still another track
      track = tTracker->GetTrack( iTrack++); //tTracker->GetPrincipalTrack();	
 
      for(Int_t k=0 ; k<numberOfLadders ; ++k){
        
        Int_t startIndex = 0; 
        Int_t endIndex = 0;       

        startIndex = myLadders[k]->GetFirstPlane();
        endIndex = startIndex + myLadders[k]->GetNumberOfPlanes();

	for(Int_t p=startIndex ; p<endIndex ; ++p) { 
         
          DPlane* myPlane = myLadders[k]->GetPlane(p);

          if( myLadders[k]->GetStatus()==0 ) {
            continue; 
          }
          else if( tTracker->GetAlignmentStatus() == 0 && myLadders[k]->GetStatus()>0 ){              
	    std::vector<Double_t> myData;
            myPlane->AlignData(track, tiniBound, myData);
  
            if( myData.size()!=0 ){
              totalHitsInAlignment[k]++;
              if(chi2Limit==0) myLadders[k]->AlignLadder(myData, k, p);
              else if (track->GetChiSquare() <= chi2Limit) myLadders[k]->AlignLadder(myData, k, p);
            }
            myData.clear();
          }
          else if( tTracker->GetAlignmentStatus() == 1 && myLadders[k]->GetStatus()>1 ){
            std::vector<Double_t> myData;
            myPlane->AlignData(track, tiniBound, myData);

            if( myData.size()!=0 ){
              totalHitsInAlignment[k]++;
              if(chi2Limit==0) myLadders[k]->AlignLadder(myData, k, p);
              else if (track->GetChiSquare() <= chi2Limit) myLadders[k]->AlignLadder(myData, k, p);
            }
            myData.clear();
          }
          else if( tTracker->GetAlignmentStatus() >=2 && myLadders[k]->GetStatus()>2 ){ // align just the DUT (Status 3)
            std::vector<Double_t> myData;
            myPlane->AlignData(track, tiniBound, myData);
            
	    if( myData.size()!=0 ){
	      totalHitsInAlignment[k]++;	 
              if(chi2Limit==0) myLadders[k]->AlignLadder(myData, k, p);
              else if (track->GetChiSquare() <= chi2Limit) myLadders[k]->AlignLadder(myData, k, p);
            }
            myData.clear();
          }

         if( /*myLadders[k]->GetPlane(startIndex)->GetPrecAlignment()->DataSize()*/totalHitsInAlignment[k]>=nAlignHitsInPlane && stopAlignLadder[k]==0  ) {
               
               std::cout<<endl;
               std::cout<<"//***********************************************************//"<<std::endl;
               std::cout<<"//****************** MINUIT MINIMIZATION ********************//"<<std::endl;
               std::cout<<"//***********************************************************//"<<std::endl;
               
               if(AlignDebug) {
                  std::cout<<"DataSize() = "<<myLadders[k]->GetPlane(myLadders[k]->GetFirstPlane())->GetPrecAlignment()->DataSize()<<std::endl;                 
               }
               
               cout << endl << "============="<<endl<< " Alignment of Ladder " << k+1 << " with MINUIT and bound=" << tiniBound << endl<<"============="<< endl << endl;

               AlignPrecMimosaLadder(myLadders[k], modeAuto, tiniBound);
               //AlignPrecMimosaLadder2(myLadders[k], modeAuto, tiniBound); // New method.
               //myLadders[k]->UpdateAlignment();

               if( modeAuto==0 ) {
                 Double_t newBound = 0.;
                 Char_t answer[100];
                 
                 while ( 0.<=newBound && newBound<0.1 ) {
                  newBound = 0;
                  cout << endl<<endl<<endl << "Choose a new minimal track-hit distance (um) for next iteration, set a negative number to stop alignment: " << endl<<endl;
                  cin >> answer;
                  newBound = atof( answer);
                 }
                 
                 if( newBound<0. ) {
                  stopAlignLadder[k]=1;              
                 }
                 else {
                   tiniBound = newBound;
                  
                   for(Int_t i=myLadders[k]->GetFirstPlane() ; i<(myLadders[k]->GetFirstPlane()+myLadders[k]->GetNumberOfPlanes()) ; ++i) {
                     myLadders[k]->GetPlane(i)->GetPrecAlignment()->ResetDataPoints();
                   }
                 }
              }
              else { // if automatic mode, stop
                stopAlignLadder[k]=1;
                cout << " --> will stop alignment for ladder " << k+1 << endl;
              }
        } // if enough event for a fit
        
      } // end loop on planes.
     } // end loop on ladders
    } // end if a track is found

  } // end of while
  
  for( Int_t l=0 ; l<numberOfLadders ; ++l ) { // loop on ladders
   
    for( Int_t pId=myLadders[l]->GetFirstPlane() ; pId<myLadders[l]->GetFirstPlane()+myLadders[l]->GetNumberOfPlanes() ; ++pId ) { // planes in ladder
      
      //std::cout<<" pId = "<<pId<<std::endl;
      Int_t DPrecAlignMethod = fSession->GetSetup()->GetTrackerPar().DPrecAlignMethod;   // LC 2015/01/31
      DPrecAlign* planeAlignment = new DPrecAlign(DPrecAlignMethod);

      planeAlignment->CopyAlignment( myLadders[l]->GetPlane(pId)->GetPrecAlignment() );
      //printf("  --> Current absolute alignment:\n");
      //myLadders[l]->GetPlane(pId)->GetPrecAlignment()->PrintAll();
      //planeAlignment->PrintAll();
      //printf("  --> Current Ladder alignment:\n");
      //myLadders[l]->GetPrecAlign()->PrintAll();
      printf("  --> Relative alignment:\n");
      //myLadders[l]->GetPlane(pId)->GetPrecAlignment()->DeconvoluteAlignment( myLadders[l]->GetPrecAlign() );
      //myLadders[l]->GetPlane(pId)->GetPrecAlignment()->PrintAll();
      planeAlignment->DeconvoluteAlignment( myLadders[l]->GetPrecAlign() );
      //planeAlignment->PrintAll();
      DPrecAlign* relativePlaneInLadder = myLadders[l]->GetRelativePlaneAlign(pId-myLadders[l]->GetFirstPlane()+1);
      relativePlaneInLadder->CopyAlignment( planeAlignment );
      relativePlaneInLadder->PrintAll();
    }
  }

 //Int_t rn = fSession->GetRunNumber();

  fSession->SetEvents(nAdditionalEvents);
 
  while(fSession->NextRawEvent() == kTRUE ) {    
   
    //===================
    tTracker->Update();
    //===================
    
    Int_t iTrack = 1;

    while ( iTrack <= tTracker->GetTracksN() ) {   // while there is still another track
      track = tTracker->GetTrack( iTrack++);	
 
      for(Int_t k=0 ; k<numberOfLadders ; ++k){
      
        if( !(myLadders[k]->GetLadderType() == "Custom" || myLadders[k]->GetLadderType() == "custom") ) {
          std::cout<<"No custom ladder found."<<std::endl;
          return;
        }
       
        myLadders[k]->UpdateAlignment(); 

        Int_t startIndex = 0; 
        Int_t endIndex = 0;       

        startIndex = myLadders[k]->GetFirstPlane();
        endIndex = startIndex + myLadders[k]->GetNumberOfPlanes();

        for(Int_t p=startIndex ; p<endIndex ; ++p) { 
         
          DPlane* myPlane = myLadders[k]->GetPlane(p);

          std::vector<Double_t> myData;
          myPlane->AlignData(track, tiniBound, myData);

          if( myData.size()!=0 ){
            if (chi2Limit == 0) myLadders[k]->FillResidus(myData, p);
            else if (track->GetChiSquare() <= chi2Limit) myLadders[k]->FillResidus(myData, p);
          }
          myData.clear();
        } // end loop on planes.
      } // end loop on ladders
    } // end if a track is found

  } // end of while

//-----------------------------------
  // call the script to modify the runXXX.cfg file ::: modified by JB 2009/07/19
  // Handle any number of planes
  // Only if there was really a fit (nAlignEvents>0)
  
  //if( nAlignEvents>=0 ) {
    cout << endl << "***************************************" << endl;
    cout << endl << "Applying results ...." << endl;
    Char_t update_cfg_string[5000];
    Char_t update_cfg_string_ladders[2000];
    Char_t update_cfg_string_ladder_planes[2000];

    sprintf(update_cfg_string,"Update_conf_align2D %s/run%d.cfg", fSession->GetConfigPath().Data(), fSession->GetRunNumber());    // See the perl scripts in Scripts/ directory
    sprintf(update_cfg_string_ladders,"Update_conf_Ladders %s/run%d.cfg", fSession->GetConfigPath().Data(), fSession->GetRunNumber());
    sprintf(update_cfg_string_ladder_planes,"Update_conf_Ladder_Planes %s/run%d.cfg", fSession->GetConfigPath().Data(), fSession->GetRunNumber());

    for(Int_t k=0; k<numberOfLadders ; ++k){
    
      if( myLadders[k]->GetStatus()==0 ) { cout<<"Ladder status = 0, writting translations and tilts skipped !"<<endl; continue; }
      
       sprintf( update_cfg_string_ladders, "%s %d %10.5f %10.5f %10.5f %10.5f %10.5f %10.5f", update_cfg_string_ladders, k+1, myLadders[k]->GetPrecAlign()->GetTranslation()[0]/1000., myLadders[k]->GetPrecAlign()->GetTranslation()[1]/1000., myLadders[k]->GetPrecAlign()->GetTranslation()[2]/1000., myLadders[k]->GetPrecAlign()->GetRotations()[0]*TMath::RadToDeg(), myLadders[k]->GetPrecAlign()->GetRotations()[1]*TMath::RadToDeg(), myLadders[k]->GetPrecAlign()->GetRotations()[2]*TMath::RadToDeg());

      Int_t startIndex = 0; 
      Int_t endIndex = 0;       

      startIndex = myLadders[k]->GetFirstPlane();
      endIndex = startIndex + myLadders[k]->GetNumberOfPlanes();

      for(Int_t i=startIndex; i<endIndex ; ++i){ // loop on ladder planes
         
        myLadders[k]->UpdateAlignment();
        
        DPlane* myPlane = myLadders[k]->GetPlane(i);

        if(AlignDebug >= 1) std::cout<<"Writing positions and tilts for plane "<<myPlane->GetPlaneNumber()<<std::endl; 

        Double_t planeOffSetX = myPlane->GetPrecAlignment()->GetTranslation()[0];
        Double_t planeOffSetY = myPlane->GetPrecAlignment()->GetTranslation()[1];
        Double_t planeOffSetZ = myPlane->GetPrecAlignment()->GetTranslation()[2];

        Double_t planeTiltOffsetX =  myPlane->GetPrecAlignment()->GetRotations()[2];
        Double_t planeTiltOffsetY =  myPlane->GetPrecAlignment()->GetRotations()[1];
        Double_t planeTiltOffsetZ =  myPlane->GetPrecAlignment()->GetRotations()[0];
     
        DPrecAlign* localFrameAlign = myLadders[k]->GetRelativePlaneAlign(i-startIndex+1);

        Double_t localTranslationU = localFrameAlign->GetTranslation()[0];
        Double_t localTranslationV = localFrameAlign->GetTranslation()[1];
        Double_t localTranslationW = localFrameAlign->GetTranslation()[2];
       
        Double_t localRotationU = localFrameAlign->GetRotations()[2];
        Double_t localRotationV = localFrameAlign->GetRotations()[1];
        Double_t localRotationW = localFrameAlign->GetRotations()[0];
        
        if(AlignDebug >=1) {
          std::cout<<"Plane number = "<<i<<"  planeOffSetX = "<<planeOffSetX/1000.<<"  planeOffSetY = "<<planeOffSetY/1000.<<"  planeOffSetZ = "<<planeOffSetZ/1000.<<std::endl;
          std::cout<<"Plane number = "<<i<<"  planeTiltOffSetX = "<<planeTiltOffsetX<<"  planeTiltOffSetY = "<<planeTiltOffsetY<<"  planeTiltOffSetZ = "<<planeTiltOffsetZ<<std::endl;
        }

        sprintf( update_cfg_string, "%s %d %10.5f %10.5f %10.5f %10.5f %10.5f %10.5f", update_cfg_string, myPlane->GetPlaneNumber(), planeOffSetX/1000., planeOffSetY/1000., planeOffSetZ/1000., planeTiltOffsetZ*TMath::RadToDeg(), planeTiltOffsetY*TMath::RadToDeg(), planeTiltOffsetX*TMath::RadToDeg());

        sprintf( update_cfg_string_ladder_planes, "%s %d %10.5f %10.5f %10.5f %10.5f %10.5f %10.5f", update_cfg_string_ladder_planes, myPlane->GetPlaneNumber(), localTranslationU/1000., localTranslationV/1000., localTranslationW/1000., localRotationU*TMath::RadToDeg(), localRotationV*TMath::RadToDeg(), localRotationW*TMath::RadToDeg());

        
      } //end loop on ladder planes


      std::cout<<"Writing residus ..." <<std::endl;
      myLadders[k]->PrepareDisplay();
      myLadders[k]->Display( fSession->GetRunNumber() );

    } // end loop on ladders
    
    sprintf(update_cfg_string,"%s", update_cfg_string); // add "-d" at the end for debug
    sprintf(update_cfg_string_ladders,"%s", update_cfg_string_ladders); // add "-d" at the end for debug
    sprintf(update_cfg_string_ladder_planes,"%s", update_cfg_string_ladder_planes); // add "-d" at the end for debug
    cout<<update_cfg_string_ladders<<endl;
    cout<<update_cfg_string_ladder_planes<<endl;
    cout<<update_cfg_string<<endl;

    gSystem->Exec(update_cfg_string_ladders);
    gSystem->Exec(update_cfg_string_ladder_planes);
    gSystem->Exec(update_cfg_string);
  //}

  //-----------------------------------
  // The End

}

/*
//------- OUTDATED -------//
//______________________________________________________________________________
//
void MimosaAlignAnalysis::AlignLadder(Bool_t modeAuto, const Double_t tiniBound, Int_t nAlignEvents, Int_t nAlignHitsInPlane, Int_t nAdditionalEvents, Int_t ladderFace, Double_t chi2Limit)
{

  DTracker *tTracker  =  fSession->GetTracker();          
  Int_t  numberOfLadders = tTracker->GetNumberOfLadders();
  //Int_t  numberOfPlanes  = tTracker->GetPlanesN();
  
  DLadder **myLadders = new DLadder*[numberOfLadders]; 
  Bool_t *stopAlignLadder = new Bool_t[numberOfLadders];

  //Int_t numberOfSimplePlane = numberOfPlanes;

  for(Int_t j=0 ; j<numberOfLadders ; ++j) {
    myLadders[j] = tTracker->GetLadder(j+1);
    stopAlignLadder[j] = kFALSE;
    //numberOfSimplePlane -= myLadders[j]->GetNumberOfPlanes();
  }

  //std::cout<<"Number of simple planes = "<<numberOfSimplePlane<<std::endl;

  Int_t counts = 0;
  Int_t planeIter = 0;

  if(ladderFace == 1) planeIter=1;
  else if(ladderFace == 2) planeIter=myLadders[0]->GetNumberOfPlanes()/2+1;


  fSession->SetEvents(nAlignEvents);

  while(fSession->NextRawEvent() == kTRUE ) {    
    
    counts = fSession->GetCurrentEventNumber();   
    
   //------------------
    tTracker->Update();
   //------------------
    
    for(Int_t itPlane=1 ; itPlane <= myLadders[0]->GetNumberOfPlanes()/2 ; ++itPlane){ 
      
      myLadders[0]->MakeMiniVectors(itPlane);
      std::vector<DTrack*> myTracks = myLadders[0]->GetMiniVectors();
      
      //std::cout<<"Number of tracks = "<<myTracks.size()<<std::endl;
      
      if(myTracks.size() != 0 ) {

        for(Int_t j=0 ; j<(Int_t)myTracks.size() ; ++j) { // removing warning: adding cast when comparing 'Int_t' and 'size_type' (aka 'unsigned long') BH 2013/08/20


        for(Int_t itLadder=0 ; itLadder<numberOfLadders ; ++itLadder)
        {
          for(Int_t p=planeIter ; p<planeIter+myLadders[itLadder]->GetNumberOfPlanes()/2. ; ++p ) {
 
            if(myLadders[itLadder]->GetStatus() == 0) continue;

            DPlane* myPlane = myLadders[itLadder]->GetPlane(p);
          
            std::vector<Double_t> myData;
            myPlane->AlignData(myTracks[j], tiniBound, myData);

            if( myData.size()!=0 ){
              if(chi2Limit == 0.) myLadders[itLadder]->AlignLadder(myData, itLadder, p);
              else if(myTracks[j]->GetChiSquare() <= chi2Limit) myLadders[itLadder]->AlignLadder(myData, itLadder, p);
            }
            myData.clear();
        } // end loop on plane

        if( myLadders[itLadder]->GetPrecAlign()->DataSize()>=nAlignHitsInPlane && stopAlignLadder[itLadder]==kFALSE) {
           AlignPrecMimosa(myLadders[itLadder]->GetPrecAlign(), modeAuto);
          stopAlignLadder[itLadder]=kTRUE; // correcting bug: restoring assignment instead of useless condition BH 2013/08/20
        }
        
        } // end loop on ladders
      } // end loop on tracks
    }// end if

    } //end loop on planes

  } // end loop on events
 
  // write residu histogram.

  fSession->SetEvents(nAdditionalEvents);

  while(fSession->NextRawEvent() == kTRUE ) {    
    
    counts = fSession->GetCurrentEventNumber();   
    
    //----------------
    tTracker->Update();
   //---------------
   
   for(Int_t itPlane=1 ; itPlane<=myLadders[0]->GetNumberOfPlanes()/2 ; ++itPlane){ 
      
      myLadders[0]->MakeMiniVectors(itPlane);
      std::vector<DTrack*> myTracks = myLadders[0]->GetMiniVectors();
       
      if(myTracks.size() != 0 ) {

       for(Int_t j=0 ; j<myTracks.size() ; ++j) {


        for(Int_t itLadder=1 ; itLadder<numberOfLadders ; ++itLadder)
        {
          for(Int_t p=planeIter ; p<planeIter+myLadders[itLadder]->GetNumberOfPlanes()/2 ; ++p ) {
            DPlane* myPlane = myLadders[itLadder]->GetPlane(p);
              
              std::vector<Double_t> myData;
              myPlane->AlignData(myTracks[j], tiniBound, myData);

              if( myData.size()!=0 ){
                if(chi2Limit == 0) myLadders[itLadder]->FillResidus(myData, p );
                else if(myTracks[j]->GetChiSquare() <= chi2Limit) myLadders[itLadder]->FillResidus(myData, p);
              }
              myData.clear();
          } // end loop on plane
        
        } // end loop on ladders
      } // end loop on tracks
    }// end if

    } //end loop on planes

  } // end loop on events


  if( nAlignEvents>0 ) {
    cout << endl << "***************************************" << endl;
    cout << endl << "Applying results ...." << endl;
    Char_t update_cfg_string[5000];
    Char_t update_cfg_string_ladders[2000];

    sprintf(update_cfg_string,"Update_conf_align2D %s/run%d.cfg", fSession->GetConfigPath().Data(), fSession->GetRunNumber());
    sprintf(update_cfg_string_ladders,"Update_conf_Ladders %s/run%d.cfg", fSession->GetConfigPath().Data(), fSession->GetRunNumber());
    
    for(Int_t k=0; k<numberOfLadders ; ++k){
    
      sprintf( update_cfg_string_ladders, "%s %d %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f", update_cfg_string_ladders, k+1, myLadders[k]->GetPrecAlign()->GetTranslation()[0]/1000., myLadders[k]->GetPrecAlign()->GetTranslation()[1]/1000., myLadders[k]->GetPrecAlign()->GetTranslation()[2]/1000., myLadders[k]->GetPrecAlign()->GetRotations()[0]*TMath::RadToDeg(), myLadders[k]->GetPrecAlign()->GetRotations()[1]*TMath::RadToDeg(), myLadders[k]->GetPrecAlign()->GetRotations()[2]*TMath::RadToDeg());

      if( k==0 && myLadders[k]->GetLadderType() == "SimpleFace" ) k++;

      for(Int_t i=1; i<=myLadders[k]->GetNumberOfPlanes() ; i++){ // loop on secondary ref planes
        
        //Int_t indexPlane = k*12+i;
        DPlane* myPlane = myLadders[k]->GetPlane(i);

        //std::cout<<"Writing positions and tilts for plane "<<myPlane->GetPlaneNumber()<<std::endl; 

        Double_t planeOffSetX = myLadders[k]->GetPlanePosition(i)(0);
        Double_t planeOffSetY = myLadders[k]->GetPlanePosition(i)(1);
        Double_t planeOffSetZ = myLadders[k]->GetPlanePosition(i)(2);

        sprintf( update_cfg_string, "%s %d %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f", update_cfg_string, myPlane->GetPlaneNumber(), myLadders[k]->GetPrecAlign()->GetTranslation()[0]/1000.+planeOffSetX/1000., myLadders[k]->GetPrecAlign()->GetTranslation()[1]/1000.+planeOffSetY/1000., myLadders[k]->GetPrecAlign()->GetTranslation()[2]/1000.+planeOffSetZ/1000., myLadders[k]->GetPrecAlign()->GetRotations()[0]*TMath::RadToDeg(), myLadders[k]->GetPrecAlign()->GetRotations()[1]*TMath::RadToDeg(), myLadders[k]->GetPrecAlign()->GetRotations()[2]*TMath::RadToDeg());

      } //end loop on planes
     
     if( k!=0 ) {
       std::cout<<"Writing residus ..." <<std::endl;
       myLadders[k]->PrepareDisplay();
       myLadders[k]->Display( fSession->GetRunNumber() );
     }

    } // end loop on ladders

    sprintf(update_cfg_string,"%s", update_cfg_string); // add "-d" at the end for debug
    sprintf(update_cfg_string_ladders,"%s", update_cfg_string_ladders); // add "-d" at the end for debug
    cout<<update_cfg_string_ladders<<endl;
    cout<<update_cfg_string<<endl;
    gSystem->Exec(update_cfg_string_ladders);
    gSystem->Exec(update_cfg_string);
  }

  //-----------------------------------
  // The End

}
*/

//______________________________________________________________________________
//
void MimosaAlignAnalysis::AlignLadderMV(Bool_t modeAuto, const Double_t tiniBound, const Double_t boundSlopes, Int_t nAlignEvents, Int_t nGoodTracks, Int_t nAdditionalEvents, Int_t chi2Limit, Int_t mode)
{

//******************************************************************************************//
//  LC 2013/09
//
//  --> This method make the alignment of two ladder on their overlapping region.
//  --> One ladder took like reference. The other is aligned.
//  --> The alignment use minivectors.
//
//  Last Modified: LC, 2014/12/05 : Upgrade AlignMV method.
//  Last Modified: LC, 2014/12/18 : Cleaning code
//  Last Modified: LC, 2014/12/20 : Now the method use fLadders and MiniVector Objects.
//  Last Modified: LC, 2014/12/20 : Big code cleaning and simplifications (Old code removed)
//  Last Modified: LC, 2014/12/20 : Reading results part : currently disable. To be updated.
//******************************************************************************************//

  DTracker *tTracker  =  fSession->GetTracker();          
  Int_t  numberOfLadders = tTracker->GetNumberOfLadders();
  Double_t boundDistance = tiniBound;


  if( numberOfLadders<=0 ) {
    std::cout<<"No ladder to align found."<<std::endl;
    return;
  }
    
  std::vector<Bool_t> stopAlignLadder;

  for(Int_t j=0 ; j<numberOfLadders ; ++j) {

    fLadders.push_back( tTracker->GetLadder(j+1) );
    fLadders[j]->UpdateAlignment();
     
    if( !(fLadders[j]->GetLadderType() == "Custom" || fLadders[j]->GetLadderType() == "custom") ) {
       std::cout<<"No custom ladder found."<<std::endl;
       return;
    }

    if(fLadders[j]->GetMiniVectorsSize() != 0) fLadders[j]->ResetMiniVectors(); // Reset Mini-vectors if they already  exist
    //fLadders[j]->UpdateAlignment();
    stopAlignLadder.push_back(0);
  }
 
  std::string myPath = gSystem->WorkingDirectory();
  
  int pos = myPath.rfind('/');
  
  // Get the good path
  std::string path = myPath.substr(0,pos);
  std::cout << "Path: " << path << std::endl;

  // To use with macro "miniVectorsBatch.C" --> Batch to make N alignment with M mini-vectors pairs. 
  TString trackIndexLocation = path + "/miniVectorAlignment/trackIndexInFile.log";
  std::cout<<trackIndexLocation<<std::endl;

  trackIndexFile.open( trackIndexLocation.Data(), ios::trunc);
  Int_t counts = 0;

  fSession->SetEvents(nGoodTracks + 100*nAlignEvents);  // Arbitrary ... to be updated ...
  
  Bool_t testNextEvent = 1;

  while(testNextEvent == 1) { 
 
  testNextEvent = fSession->NextRawEvent();
  counts = fSession->GetCurrentEventNumber();

  if(testNextEvent==0) {
     break;
  }
    
  //------------------
  tTracker->Update();
  //------------------

  for(Int_t j=0 ; j<numberOfLadders ; ++j) {
    fLadders[j]->UpdateAlignment();
    fLadders[j]->MakeMiniVectors();
    std::cout<<"#### Ladder"<<j<<" : "<<fLadders[j]->GetMiniVectorsSize()<<" mini-vectors reconstructed ####"<<std::endl;

  }

  std::pair<Int_t, Int_t> laddersAssociated(0,1);
  
  std::cout<<"#########    MiniVectors Association   #########"<<std::endl;
  std::cout<<"     --> Ladder"<<laddersAssociated.first<<" + Ladder"<<laddersAssociated.second<<std::endl;

  if(fLadders[0]->GetMiniVectorsSize()!=0 && fLadders[1]->GetMiniVectorsSize()!=0) AssociateMiniVectors(boundDistance, boundSlopes, mode);
  else {
    std::cout<<"     --> "<<fLadders[0]->GetAssociatedMiniVectorsSize()<<" mini-vector pairs associated"<<std::endl;
    std::cout<<"################################################"<<std::endl;
    std::cout<<std::endl;
  }
 
  for(Int_t j=0 ; j<numberOfLadders ; ++j) {
   if(fLadders[j]->IfAssociated()==0) fLadders[j]->ResetMiniVectors();           // Delete all the MiniVector* object and clear the container.
  }   
 
  if( fLadders[1]->GetAssociatedMiniVectorsSize()>=nAlignEvents /*&& counts>nAlignEvents*/ && stopAlignLadder[1]==kFALSE) {
           
    std::cout<<"//***********************************************************//"<<std::endl;
    std::cout<<"//****************** MINUIT MINIMIZATION ********************//"<<std::endl;
    std::cout<<"//***********************************************************//"<<std::endl;
 
    TString logLocation = path + "/miniVectorAlignment/logs.log";   
    file.open(logLocation.Data(), ios::app); 
    
    file.precision(10);  
    file.setf( std::ios::fixed, std:: ios::floatfield );
    file<<"### Before alignment : "<<std::endl;
    file<<"### Rotation Z,Y,X Translations X,Y,Z"<<std::endl;
    file<<fLadders[1]->GetPrecAlign()->GetRotations()[0]<<endl;
    file<<fLadders[1]->GetPrecAlign()->GetRotations()[1]<<endl;
    file<<fLadders[1]->GetPrecAlign()->GetRotations()[2]<<endl;
    file<<fLadders[1]->GetPrecAlign()->GetTranslation()[0]<<endl;
    file<<fLadders[1]->GetPrecAlign()->GetTranslation()[1]<<endl;
    file<<fLadders[1]->GetPrecAlign()->GetTranslation()[2]<<endl;
    file<<std::endl; 
    
    FitInterface( modeAuto, tiniBound, boundSlopes );
    //AlignMimosaMV(fLadders[1], fLadders[1]->GetPrecAlign(), fLadders[0]->GetPrecAlign() , modeAuto, chi2Limit);

    stopAlignLadder[1]=kTRUE;
    
    file.precision(10);
    file.setf( std::ios::fixed, std:: ios::floatfield );
    file<<"### After alignment : "<<std::endl;
    file<<"### Rotation Z,Y,X Translations X,Y,Z"<<std::endl;
    file<<fLadders[1]->GetPrecAlign()->GetRotations()[0]<<endl;
    file<<fLadders[1]->GetPrecAlign()->GetRotations()[1]<<endl;
    file<<fLadders[1]->GetPrecAlign()->GetRotations()[2]<<endl;
    file<<fLadders[1]->GetPrecAlign()->GetTranslation()[0]<<endl;
    file<<fLadders[1]->GetPrecAlign()->GetTranslation()[1]<<endl;
    file<<fLadders[1]->GetPrecAlign()->GetTranslation()[2]<<endl;
    file<<std::endl;
  
    file.close();
    std::cout<<logLocation<<std::endl;

    break;
  
  } // end if
  
 } // end loop on events
 
 trackIndexFile<<int(counts)<<std::endl;
 trackIndexFile.close();
 std::cout<<"Writing evt index :)"<<std::endl;
 // write residu histogram.

Bool_t printResidues = 1;

if(printResidues==1) {

 std::cout<<"Reading additional events ..."<<std::endl;

 fSession->SetEvents(nAdditionalEvents);

 while(fSession->NextRawEvent() == kTRUE ) {    
         
   counts = fSession->GetCurrentEventNumber();   
          
   //----------------
   tTracker->Update();
   //---------------


   for(Int_t j=0 ; j<numberOfLadders ; ++j) {
     fLadders[j]->ResetMiniVectors();
     fLadders[j]->UpdateAlignment();
     fLadders[j]->MakeMiniVectors();
     //std::cout<<"#### Ladder"<<j<<" : "<<fLadders[j]->GetMiniVectorsSize()<<" mini-vectors reconstructed ####"<<std::endl;
   }

   std::pair<Int_t, Int_t> laddersAssociated(0,1);
  
   //std::cout<<"#########    MiniVectors Association   #########"<<std::endl;
   //std::cout<<"     --> Ladder"<<laddersAssociated.first<<" + Ladder"<<laddersAssociated.second<<std::endl;

   if(fLadders[0]->GetMiniVectorsSize()!=0 && fLadders[1]->GetMiniVectorsSize()!=0) AssociateMiniVectors(boundDistance, boundSlopes, mode);
   /*
   else {
     std::cout<<"     --> "<<fLadders[0]->GetAssociatedMiniVectorsSize()<<" mini-vector pairs associated"<<std::endl;
     std::cout<<"################################################"<<std::endl;
     std::cout<<std::endl;
   }
   */

   for(Int_t j=0 ; j<numberOfLadders ; ++j) {
    if(fLadders[j]->IfAssociated()==0) fLadders[j]->ResetMiniVectors();           // Delete all the MiniVector* object and clear the container if MVs not associated.
   }  
 
   std::cout<<"Associated Mini-Vectors  = "<<fLadders[1]->GetAssociatedMiniVectorsSize()<<std::endl;

 } // end loop en events.

 FillMiniVectorsResiduals( fLadders[0]->GetAssociatedMiniVectors() , fLadders[1]->GetAssociatedMiniVectors() );
 FillMiniVectorsResidualsMC( fLadders[0]->GetAssociatedMiniVectors() , fLadders[1]->GetAssociatedMiniVectors() );
      
  if( nAlignEvents>0 ) {
    cout << endl << "***************************************" << endl;
    cout << endl << "Applying results ...." << endl;
    Char_t update_cfg_string[5000];
    Char_t update_cfg_string_ladders[2000];

     sprintf(update_cfg_string,"Update_conf_align2D %s/run%d.cfg", fSession->GetConfigPath().Data(), fSession->GetRunNumber());
     sprintf(update_cfg_string_ladders,"Update_conf_Ladders %s/run%d.cfg", fSession->GetConfigPath().Data(), fSession->GetRunNumber());
          
     for(Int_t k=0; k<numberOfLadders ; ++k){
          
       sprintf( update_cfg_string_ladders, "%s %d %10.5f %10.5f %10.5f %10.5f %10.5f %10.5f", update_cfg_string_ladders, k+1, fLadders[k]->GetPrecAlign()->GetTranslation()[0]/1000., fLadders[k]->GetPrecAlign()->GetTranslation()[1]/1000., fLadders[k]->GetPrecAlign()->GetTranslation()[2]/1000., fLadders[k]->GetPrecAlign()->GetRotations()[0]*TMath::RadToDeg(), fLadders[k]->GetPrecAlign()->GetRotations()[1]*TMath::RadToDeg(), fLadders[k]->GetPrecAlign()->GetRotations()[2]*TMath::RadToDeg());

       //if( k==0 && fLadders[k]->GetLadderType() == "SimpleFace" ) k++;

       fLadders[k]->UpdateAlignment();

       for(Int_t i=1; i<=fLadders[k]->GetNumberOfPlanes() ; i++){ // loop on secondary ref planes
              
       //Int_t indexPlane = k*12+i;
       DPlane* myPlane = fLadders[k]->GetPlane(i);

       //std::cout<<"Writing positions and tilts for plane "<<myPlane->GetPlaneNumber()<<std::endl; 

      sprintf( update_cfg_string, "%s %d %10.5f %10.5f %10.5f %10.5f %10.5f %10.5f", update_cfg_string, myPlane->GetPlaneNumber(), myPlane->GetPrecAlignment()->GetTranslation()[0]/1000. , myPlane->GetPrecAlignment()->GetTranslation()[1]/1000. , myPlane->GetPrecAlignment()->GetTranslation()[2]/1000. , myPlane->GetPrecAlignment()->GetRotations()[0]*TMath::RadToDeg(), myPlane->GetPrecAlignment()->GetRotations()[1]*TMath::RadToDeg(), myPlane->GetPrecAlignment()->GetRotations()[2]*TMath::RadToDeg());

      } //end loop on planes
           
      if( k!=0 ) {
      std::cout<<"Writing residus ..." <<std::endl;
      fLadders[k]->PrepareDisplay();
      fLadders[k]->Display( fSession->GetRunNumber() );
      }

    } // end loop on ladders

    sprintf(update_cfg_string,"%s", update_cfg_string); // add "-d" at the end for debug
    sprintf(update_cfg_string_ladders,"%s", update_cfg_string_ladders); // add "-d" at the end for debug
    cout<<update_cfg_string_ladders<<endl;
    cout<<update_cfg_string<<endl;
    gSystem->Exec(update_cfg_string_ladders);
    gSystem->Exec(update_cfg_string);
  }
 
  //-----------------------------------
  // The End

}// if print residues

}

void MimosaAlignAnalysis::AssociateMiniVectors( Double_t boundDistance, Double_t boundSlopes, Int_t mode)
{

//*********************************************************************************************//
//  AssociateMiniVectors :: LC 2014
//
//  --> This method provide tools to associate the two mini-vectors on the overlapping region.
//  
//  --> Method 0 : (By default) : Spatial earch in predifined windows + Cuts in MVs tilts.
//      Work well with small hit density.
//
//  --> Method 1 : Histogramming Method (Outdated)
//  --> Method 2 : Just To Plot the Mini-vectors
//
//  --> LC 2014/12/19 : Now one can use MiniVector* objects.
//  --> Fill _associatedMiniVectors container of DLadder
//  
//  Last Modified: LC, 2014/12
//********************************************************************************************//

  if(mode==0) {

    /* // Pour apres. Pour le moment on utilise juste les ladder0 et 1.
    std::map<Int_t, std::vector<MiniVector*> > laddersMiniVectorsPerEvent;
    std::map<Int_t, DPrecAlign*> laddersDPrecAlign;

    std::vector<DLadder*>::iterator iterLadder=myLadders.begin();
    for( ; iterLadder!=myLadders.end() ; ++ iterLadder) {
      
      iterLadder->SetAssociated();
      laddersMiniVectorsPerEvent[iterLadder->GetLadderID()] = iterLadder->GetMiniVectors();
      laddersDPrecAlign[iterLadder->GetLadderID()] = iterLadder->GetDPrecAlign();
    }
    */
     
    // Only Ladder 0 and 1 for the moment.
    DLadder* ladder0 = fLadders[0];
    DLadder* ladder1 = fLadders[1];

    std::vector<MiniVector*>& miniVectorsLadder0 = ladder0->GetMiniVectors();   //laddersMiniVectorsPerEvent[0];
    std::vector<MiniVector*>& miniVectorsLadder1 = ladder1->GetMiniVectors();   //laddersMiniVectorsPerEvent[1];    

    DPrecAlign* DPrecAlignLadder0 = ladder0->GetPrecAlign();                   //laddersDPrecAlign[0];
    DPrecAlign* DPrecAlignLadder1 = ladder1->GetPrecAlign();                   //laddersDPrecAlign[1];

    std::vector<MiniVector*>::iterator itMiniVectorLadder0 = miniVectorsLadder0.begin();
    for( ; itMiniVectorLadder0!=miniVectorsLadder0.end() ; ++itMiniVectorLadder0 ) {

      DR3 currentCenterMV_Ladder0 = (*itMiniVectorLadder0)->GetMiniVectorLadderCenter(); // Coord of current MV in ladder1 frame
      DR3 currentTiltsLadder0     = (*itMiniVectorLadder0)->GetMiniVectorDirection();
      
      std::map<Double_t, MiniVector*> candidateMiniVectors;

      std::vector<MiniVector*>::iterator itMiniVectorLadder1 = miniVectorsLadder1.begin();  
      for( ; itMiniVectorLadder1!=miniVectorsLadder1.end() ; ++itMiniVectorLadder1 ) {

        DR3 currentCenterMV_Ladder1 = (*itMiniVectorLadder1)->CalculateMiniVectorIntersectionLadder(DPrecAlignLadder0);
        DR3 currentTiltsLadder1     = (*itMiniVectorLadder1)->GetMiniVectorDirection();

        DR3 diffPos = currentCenterMV_Ladder1 - currentCenterMV_Ladder0;
        DR3 diffSlopes = (currentTiltsLadder1 - currentTiltsLadder0)*1000.;

        currentCenterMV_Ladder0.Print();
        currentCenterMV_Ladder1.Print();
        diffPos.Print();
        currentTiltsLadder0.Print();
        currentTiltsLadder1.Print();
        diffSlopes.Print();

        if( fabs(diffPos(0))<=boundDistance && fabs(diffPos(1))<=boundDistance && fabs(diffSlopes(0))<=boundSlopes && fabs(diffSlopes(1))<=boundSlopes ) {
          
          Double_t distance2D = diffPos(0)*diffPos(0) + diffPos(1)*diffPos(1);
          Double_t tiltIndicator = (diffSlopes(0)+diffSlopes(1))*1000;
          
          candidateMiniVectors[distance2D*tiltIndicator] = *itMiniVectorLadder1;   
        } // end if
 
      } // end for Ladder0
      
      // Filling _associatedMiniVectors :
      if(candidateMiniVectors.size() != 0) {
        
        ladder1->AddAssociatedMiniVector( candidateMiniVectors.begin()->second );
        ladder0->AddAssociatedMiniVector( *itMiniVectorLadder0 );
        candidateMiniVectors.clear();
      } // end if

    } // end for Ladder1
    
    std::cout<<"     --> "<<ladder0->GetAssociatedMiniVectorsSize()<<" mini-vector pairs associated"<<std::endl;
    std::cout<<"################################################"<<std::endl;
    std::cout<<std::endl;
    
    /*
    miniVectorsLadder0.clear();        
    miniVectorsLadder1.clear();  
    laddersMiniVectorsPerEvent.clear();
    laddersDPrecAlign.clear();
    */
    
    ladder0->ResetMiniVectors();   // Delete All MiniVector objects in DLadder::_miniVectors
    ladder1->ResetMiniVectors();
  }

/* //---------  OLD CODE --------//
  std::vector<DataPoints**> miniVectorDataPoints;

 if(mode==0) {

  std::map<Int_t, std::map<Int_t, DataPoints**> > dataMap;

  std::map<Int_t, std::map<Int_t, DR3> > rightDistances;
  std::map<Int_t, std::map<Int_t, DR3> > leftDistances;
      
  //create the file, the Tree and few branches
  //TFile myFile("tree.root","recreate");
  //TTree myTree("tree","Tree");
  
  for(Int_t i=0; i<MV1.size(); ++i) {

    DR3 positionPointLadder1 = MV1[i]->GetLinearFit().GetOrigin();
    DR3 tiltPointLadder1     = MV1[i]->GetLinearFit().GetSlopeZ(); 
                
    DataPoints* myDataPoint1 = new DataPoints();
           
    myDataPoint1->Set(MV1Pos[i](0), MV1Pos[i](1), positionPointLadder1(0), positionPointLadder1(1), positionPointLadder1(2), tiltPointLadder1(0), tiltPointLadder1(1) );
                 
    ladder0->GetPrecAlign()->CalculateIntersection(myDataPoint1);
    DR3 projectionPoint1 = ladder0->GetPrecAlign()->GetTrackPosition();
           
    myDataPoint1->Set(MV1Pos[i](0), MV1Pos[i](1), positionPointLadder1(0), positionPointLadder1(1), MV1Pos[i](2), tiltPointLadder1(0), tiltPointLadder1(1) );  // /!\ Astuce :: centerLadderPosition1[j](2) a la place de positionPointLadder1(2). positionPointLadder1(2) = 0 donc on peut mettre une autre donnee qui nous interresse a la place ;) /!\
     
     //ladder1->GetPrecAlign()->NewMiniVector1(MV1[i]);
    //std::cout<<"positionPointLadder1(2) = "<<positionPointLadder1(2)<<std::endl;
      
    std::map<Int_t, DataPoints**> distanceMap;
    DataPoints** myData = new DataPoints*[2];
    DTrack** myDataMV = new DTrack*[2];

    for(Int_t j=0 ; j<MV0.size() ; ++j) {
    
      DR3 positionPointLadder0 = MV0[j]->GetLinearFit().GetOrigin();
      DR3 tiltPointLadder0     = MV0[j]->GetLinearFit().GetSlopeZ(); 
                
      DataPoints* myDataPoint0 = new DataPoints();
           
      myDataPoint0->Set(MV0Pos[j](0), MV0Pos[j](1), positionPointLadder0(0), positionPointLadder0(1), positionPointLadder0(2), tiltPointLadder0(0), tiltPointLadder0(1) );
                 
      ladder0->GetPrecAlign()->CalculateIntersection(myDataPoint0);
      DR3 projectionPoint0 = ladder0->GetPrecAlign()->GetTrackPosition();
           
      myDataPoint0->Set(MV0Pos[j](0), MV0Pos[j](1), positionPointLadder0(0), positionPointLadder0(1), MV0Pos[j](2), tiltPointLadder0(0), tiltPointLadder0(1) );  // /!\ Astuce :: centerLadderPosition1[j](2) a la place de positionPointLadder1(2). positionPointLadder1(2) = 0 donc on peut mettre une autre donnee qui nous interresse a la place ;) /!\
    
      //ladder1->GetPrecAlign()->NewMiniVector(MV0[j]);
     // std::cout<<"positionPointLadder0(2) = "<<positionPointLadder0(2)<<std::endl;
       
      DR3 diffPos = projectionPoint1 - projectionPoint0;
      DR3 diffSlopes = (tiltPointLadder1 - tiltPointLadder0)*1000.;

      Double_t distance = sqrt( diffPos(0)*diffPos(0) + diffPos(1)*diffPos(1) + diffPos(2)*diffPos(2) );

      myData[0] = myDataPoint0;
      myData[1] = myDataPoint1;

      DR3 hitLadder1 = myDataPoint1->GetHitPosition();  // In tracker coord.
      DR3 trackOriginLadder1 = myDataPoint1->GetTrackOrigin();  // In tracker coord.
 
      hitLadder1(2) = trackOriginLadder1(2);
      trackOriginLadder1(2) = 0.;
      hitLadder1.Print();

      distanceMap[j] = myData;
      
      myDataMV[0] = MV0[j];
      myDataMV[1] = MV1[i];
       
      if( mode==0 && fabs(diffPos(0))<=boundDistance && fabs(diffPos(1))<=boundDistance && fabs(diffSlopes(0))<=boundSlopes && fabs(diffSlopes(1))<=boundSlopes ) {
        
        miniVectorDataPoints.push_back(myData);
        
        std::cout<<"diffPos = "<<std::endl;
        diffPos.Print();
        std::cout<<"diffSlopes = "<<std::endl;
        diffSlopes.Print();
        
        //MV0Pos[j].Print();
        std::cout<<"MiniVector center = "<<std::endl;
        MV1Pos[i].Print();

        DHit** hitsMiniVector1 = new DHit*[2];
        hitsMiniVector1[0] = myDataMV[1]->GetHit(0);
        hitsMiniVector1[1] = myDataMV[1]->GetHit(1);
    
        DR3 HitPlanePos0(hitsMiniVector1[0]->GetPositionUhitCG(), hitsMiniVector1[0]->GetPositionVhitCG(), hitsMiniVector1[0]->GetPositionWhitCG()); 
        DR3 HitPlanePos1(hitsMiniVector1[1]->GetPositionUhitCG(), hitsMiniVector1[1]->GetPositionVhitCG(), hitsMiniVector1[1]->GetPositionWhitCG());

        std::cout<<"Coord Hits MV1 = "<<std::endl;
        HitPlanePos0.Print();
        HitPlanePos1.Print();
        
        ladder1->GetPrecAlign()->NewMiniVector(myDataMV[0]);
        ladder1->GetPrecAlign()->NewMiniVector1(myDataMV[1]);
        
        std::cout<<"Addr = "<<myDataMV[1]->GetHit(1)<<std::endl;
         
        ladder1->GetPrecAlign()->NewData(myData[0]);
        ladder1->GetPrecAlign()->NewData1(myData[1]);

        TList *miniVectsLadder1 = ladder1->GetPrecAlign()->GetMiniVectors1();
        DTrack* myTrack = (DTrack*)miniVectsLadder1->Last();

        DHit** myHits = new DHit*[2];
        myHits[0] = myTrack->GetHit(0);
        myHits[1] = myTrack->GetHit(1);

        DR3 Hit1(myHits[1]->GetPositionUhitCG(), myHits[1]->GetPositionVhitCG(), myHits[1]->GetPositionWhitCG()); 
        std::cout<<myHits[1]<<"   "<<myTrack->GetHit(1)<<std::endl;
        Hit1.Print();

        ladder1->GetPrecAlign()->ListDataPoints();

      } // end if
 
      std::map<Int_t, DR3> mapDistance0;

      for(Int_t i_left=0 ; i_left<MV0.size() ; ++i_left) {
      
        if(i_left!=j) {
      
          DR3 positionLadderLeft = MV0[i_left]->GetLinearFit().GetOrigin();
          DR3 tiltLadderLeft = MV0[i_left]->GetLinearFit().GetSlopeZ(); 
                
          DataPoints* myDataPointLeft = new DataPoints();
           
          myDataPointLeft->Set(MV0Pos[i_left](0), MV0Pos[i_left](1), positionLadderLeft(0), positionLadderLeft(1), positionLadderLeft(2), tiltLadderLeft(0), tiltLadderLeft(1) );
                 
          ladder0->GetPrecAlign()->CalculateIntersection(myDataPointLeft);
          DR3 projectionLeft = ladder0->GetPrecAlign()->GetTrackPosition();
     
          mapDistance0[i_left] = projectionPoint0 - projectionLeft;
        } // end if
      } // end for

      leftDistances[j] = mapDistance0;
       
    } // end for MV0.

    dataMap[i]=distanceMap;

    std::map<Int_t, DR3> mapDistance1;

    for(Int_t i_right=0 ; i_right<MV1.size() ; ++i_right) { // Distances between right hits.
      
      if(i_right!=i) {
      
        DR3 positionLadderRight = MV1[i_right]->GetLinearFit().GetOrigin();
        DR3 tiltLadderRight = MV1[i_right]->GetLinearFit().GetSlopeZ(); 
                
          DataPoints* myDataPointRight = new DataPoints();
           
          myDataPointRight->Set(MV1Pos[i_right](0), MV1Pos[i_right](1), positionLadderRight(0), positionLadderRight(1), positionLadderRight(2), tiltLadderRight(0), tiltLadderRight(1) );
                 
          ladder0->GetPrecAlign()->CalculateIntersection(myDataPointRight);
          DR3 projectionRight = ladder0->GetPrecAlign()->GetTrackPosition();

        
        mapDistance1[i_right] =  projectionPoint1 - projectionRight;
      }
    } // end for hit_right

    rightDistances[i] = mapDistance1;
  //#####end comment
 
   } // end for MV1.

 } // end if mode==0
*/
/*
  if(mode==1) {

  std::map<Double_t, std::vector<Int_t> > deltaDistances;

  std::cout<<"map size (Right || Left) = "<<rightDistances.size()<<"  "<<leftDistances.size()<<std::endl;

  std::map<Int_t, std::map<Int_t, DR3> >::iterator rightHitIter = rightDistances.begin();

  for( ; rightHitIter != rightDistances.end() ; ++rightHitIter ) {
    
    Int_t hitRightNumber1 = rightHitIter->first;
    Int_t hitRightNumber2 = -1;
    DR3 distanceRight = DR3(0,0,0);


    std::map<Int_t, DR3>& mapHitRight = rightHitIter->second;
    std::map<Int_t, DR3>::iterator rightHitIterator2 = mapHitRight.begin();
    
    for( ; rightHitIterator2 != mapHitRight.end() ; ++rightHitIterator2 ) {
      
      hitRightNumber2 = rightHitIterator2->first;
      DR3 distanceRight = rightHitIterator2->second;
      
      std::map<Int_t, std::map<Int_t, DR3> >::iterator leftHitIter = leftDistances.begin();
      
      for( ; leftHitIter != leftDistances.end() ; ++leftHitIter ) {
      
        Int_t hitLeftNumber1 = leftHitIter->first;
        Int_t hitLeftNumber2 = -1;
        DR3 distanceLeft = DR3(0,0,0);

        std::map<Int_t, DR3>& mapHitLeft = leftHitIter->second;
        std::map<Int_t, DR3>::iterator leftHitIterator2 = mapHitLeft.begin();

        for( ; leftHitIterator2 != mapHitLeft.end() ; ++leftHitIterator2 ) {
             
          hitLeftNumber2 = leftHitIterator2->first;
          DR3 distanceLeft = leftHitIterator2->second;
          
          Double_t deltaD = fabs( sqrt(distanceRight(0)*distanceRight(0)+distanceRight(1)*distanceRight(1)) 
                                 - sqrt(distanceLeft(0)*distanceLeft(0)+distanceLeft(1)*distanceLeft(1)) );
          
         // std::cout<<"delta D = "<<deltaD<<std::endl;
         
          std::vector<Int_t> shiftHits; 
          shiftHits.push_back( hitRightNumber1-hitLeftNumber1 );
          shiftHits.push_back( hitRightNumber1 );
          shiftHits.push_back( hitLeftNumber1 );
          
          if( deltaD < 20. ) deltaDistances[deltaD] = shiftHits;

        }
      }
    }
  }
  
  std::map<Double_t, std::vector<Int_t> >::iterator iterDistance = deltaDistances.begin();

  for( ; iterDistance!=deltaDistances.end() ; ++iterDistance ) {
  
    std::vector<Int_t>& shiftHits = iterDistance->second;
    std::cout<<"Association = "<<shiftHits[0]<<"  hitLeft = "<<shiftHits[2]<<"  hitRight = "<<shiftHits[1]<<" Delta distance between hits = "<<iterDistance->first<<std::endl;
    
    DataPoints** myData = new DataPoints*[2];
   
    std::map<Int_t, DataPoints**>& myMap = dataMap[shiftHits[1]];
    
    myData[0] = myMap[shiftHits[2]][0];
    myData[1] = myMap[shiftHits[2]][1];
   
    ladder0->GetPrecAlign()->CalculateIntersection(myData[0]);
    DR3 projectionPoint0 = ladder0->GetPrecAlign()->GetTrackPosition();
    
    ladder0->GetPrecAlign()->CalculateIntersection(myData[1]);
    DR3 projectionPoint1 = ladder0->GetPrecAlign()->GetTrackPosition();

   if( fabs(projectionPoint1(0)-projectionPoint0(0))<=boundDistance && fabs(projectionPoint1(1)-projectionPoint0(1))<=boundDistance ) miniVectorDataPoints.push_back(myData);

  } // end for

  }
*/
/*
 if(mode==1) {
     
  std::map<Int_t, std::vector<DataPoints*> > miniVectorLadder1;
  std::map<Int_t, std::vector<DataPoints*> > miniVectorLadder0;

  for(Int_t i=0; i<MV1.size(); ++i) {

    DR3 positionPointLadder1 = MV1[i]->GetLinearFit().GetOrigin();
    DR3 tiltPointLadder1     = MV1[i]->GetLinearFit().GetSlopeZ(); 
                
    DataPoints* myDataPoint1 = new DataPoints();
           
    myDataPoint1->Set(MV1Pos[i](0), MV1Pos[i](1), positionPointLadder1(0), positionPointLadder1(1), positionPointLadder1(2), tiltPointLadder1(0), tiltPointLadder1(1) );
    
    std::vector<DataPoints*> myDataPoints;
    myDataPoints.push_back(myDataPoint1);
     
    ladder0->GetPrecAlign()->CalculateIntersection(myDataPoint1);
    DR3 projectionPoint1 = ladder0->GetPrecAlign()->GetTrackPosition();
    std::cout<<projectionPoint1(0)<<"  "<<projectionPoint1(1)<<"  "<<projectionPoint1(2)<<"  ";                                                                                        

    ladder1->GetPrecAlign()->CalculateIntersection(myDataPoint1);
    DR3 projectionPoint2 = ladder1->GetPrecAlign()->GetTrackPosition();
    std::cout<<projectionPoint2(0)<<"  "<<projectionPoint2(1)<<"  "<<projectionPoint2(2)<<std::endl;

           
    myDataPoint1->Set(MV1Pos[i](0), MV1Pos[i](1), positionPointLadder1(0), positionPointLadder1(1), MV1Pos[i](2), tiltPointLadder1(0), tiltPointLadder1(1) );  // /!\ Astuce :: centerLadderPosition1[j](2) a la place de positionPointLadder1(2). positionPointLadder1(2) = 0 donc on peut mettre une autre donnee qui nous interresse a la place ;) /!\
    
    myDataPoints.push_back(myDataPoint1);

    miniVectorLadder1[i] = myDataPoints;

  } // end for

  for(Int_t j=0 ; j<MV0.size() ; ++j) {
    
    DR3 positionPointLadder0 = MV0[j]->GetLinearFit().GetOrigin();
    DR3 tiltPointLadder0     = MV0[j]->GetLinearFit().GetSlopeZ(); 
                
    DataPoints* myDataPoint0 = new DataPoints();
           
    myDataPoint0->Set(MV0Pos[j](0), MV0Pos[j](1), positionPointLadder0(0), positionPointLadder0(1), positionPointLadder0(2), tiltPointLadder0(0), tiltPointLadder0(1) );
    
    std::vector<DataPoints*> myDataPoints;
    myDataPoints.push_back(myDataPoint0);
                 
    ladder0->GetPrecAlign()->CalculateIntersection(myDataPoint0);
    DR3 projectionPoint0 = ladder0->GetPrecAlign()->GetTrackPosition();
    std::cout<<projectionPoint0(0)<<"  "<<projectionPoint0(1)<<"  "<<projectionPoint0(2)<<std::endl;
           
    myDataPoint0->Set(MV0Pos[j](0), MV0Pos[j](1), positionPointLadder0(0), positionPointLadder0(1), MV0Pos[j](2), tiltPointLadder0(0), tiltPointLadder0(1) );  // /!\ Astuce :: centerLadderPosition1[j](2) a la place de positionPointLadder1(2). positionPointLadder1(2) = 0 donc on peut mettre une autre donnee qui nous interresse a la place ;) /!\
    
    myDataPoints.push_back(myDataPoint0);
    miniVectorLadder0[j] = myDataPoints;
  } // end for

  //std::cout<<"test0"<<std::endl;

  for(Int_t p=0; p<miniVectorLadder1.size(); ++p) {
   
    if( miniVectorLadder1.size()-p<=0) break;

    miniVectorDataPoints.clear();
    
    TH1F* histo = new TH1F("distance","Distance UV",40000,-200000,200000);

    for(Int_t k=0; k<miniVectorLadder0.size(); ++k) {
  
      //std::cout<<"size MV0 = "<<miniVectorLadder0.size()<<"  size MV0 - p = "<<miniVectorLadder0.size()-p<<std::endl;
      
      if( (k+p)>=miniVectorLadder1.size() && miniVectorLadder0.size()>=k) break;

      DataPoints** data = new DataPoints*[2];
      data[0] = miniVectorLadder0[k][1];
      data[1] = miniVectorLadder1[k+p][1];

      miniVectorDataPoints.push_back(data);

      ladder0->GetPrecAlign()->CalculateIntersection( miniVectorLadder1[k+p][0] );
      DR3 projectionPoint1 = ladder0->GetPrecAlign()->GetTrackPosition();

      ladder0->GetPrecAlign()->CalculateIntersection( miniVectorLadder0[k][0]);
      DR3 projectionPoint0 = ladder0->GetPrecAlign()->GetTrackPosition();

      (projectionPoint1-projectionPoint0).Print();

      histo->Fill( sqrt( (projectionPoint1(0)-projectionPoint0(0))*(projectionPoint1(0)-projectionPoint0(0)) + (projectionPoint1(1)-projectionPoint0(1))*(projectionPoint1(1)-projectionPoint0(1)) ) );
     
      //ladder1->GetPrecAlign()->NewData(data[0]);
      //ladder1->GetPrecAlign()->NewData1(data[1]);

      //std::cout<<"k = "<<k<<"   p = "<<p<<std::endl;
    } // end for k

    //std::cout<<"test1"<<std::endl;
    histo->Fit("gaus","q");
    
    TF1* fitHisto = histo->GetFunction("gaus");
    Double_t meanHisto = fitHisto->GetParameter(1);
    Double_t sigmaHisto = fitHisto->GetParameter(2);

    std::cout<<" Mean = "<<meanHisto<<"  Sigma = "<<sigmaHisto<<std::endl;
    //AlignMimosaAssociationMV(ladder1, ladder1->GetPrecAlign(), ladder0->GetPrecAlign(), 1, 0.);

    //ladder1->GetPrecAlign()->ResetDataPoints();
    //ladder1->GetPrecAlign()->ResetDataPoints1();

    histo->Delete();

  } // end for p
  
 } // end if mode==1
*/
/*
 if(mode==2) {
   
  TMultiGraph *mg = new TMultiGraph();
  mg->SetTitle("Mini-Vectors");
      
  std::map<Int_t, std::vector<Double_t> > mapArray;

  for(Int_t i=0; i<MV0.size(); ++i) {

    DR3 positionPointLadder0 = MV0[i]->GetLinearFit().GetOrigin();
    DR3 tiltPointLadder0     = MV0[i]->GetLinearFit().GetSlopeZ(); 
     
    DataPoints* myDataPoint0 = new DataPoints(); 
                
    myDataPoint0->Set(MV0Pos[i](0), MV0Pos[i](1), positionPointLadder0(0), positionPointLadder0(1), positionPointLadder0(2), tiltPointLadder0(0), tiltPointLadder0(1) );
    
    ladder1->GetPrecAlign()->CalculateIntersection(myDataPoint0);
    DR3 projectionPoint1 = ladder1->GetPrecAlign()->GetTrackPosition();
   
    ladder0->GetPrecAlign()->CalculateIntersection(myDataPoint0);
    DR3 projectionPoint2 = ladder0->GetPrecAlign()->GetTrackPosition();
   
    std::vector<Double_t> vect;
    
   if( fabs(projectionPoint1(1))<10000 ) {
          
      vect.push_back( projectionPoint1(2) );
      vect.push_back( projectionPoint2(2) );
      vect.push_back( projectionPoint1(1) );
      vect.push_back( projectionPoint2(1) );
   } // end if

   if(vect.size()!=0) mapArray[i] = vect;
  
  } // end for
  
  std::vector<Double_t> vectY;
  std::vector<Double_t> vectZ;
   
  for(Int_t j=0 ; j<MV1.size() ; ++j) {
    
    DR3 positionPointLadder1 = MV1[j]->GetLinearFit().GetOrigin();
    DR3 tiltPointLadder1     = MV1[j]->GetLinearFit().GetSlopeZ(); 
                
    DataPoints* myDataPoint1 = new DataPoints();
           
    myDataPoint1->Set(MV1Pos[j](0), MV1Pos[j](1), positionPointLadder1(0), positionPointLadder1(1), positionPointLadder1(2), tiltPointLadder1(0), tiltPointLadder1(1) );
    
    ladder1->GetPrecAlign()->CalculateIntersection(myDataPoint1);
    DR3 projectionPoint1 = ladder1->GetPrecAlign()->GetTrackPosition();
    
    vectY.push_back( projectionPoint1(1) );
    vectZ.push_back( projectionPoint1(2) );
               
  } // end for

  TCanvas *c1 = new TCanvas("myCanvas","canvas",200,200,500,500);
  c1->cd();
                  
  //TH2F* hframe = new TH2F("hframe"," ",5000,25000,45000,5000,-10000,10000);                                                            
  //hframe->Draw();
  //TH1F *hframe = c1->DrawFrame(25000,-10000,45000,10000);
                                         
  TGraph *gr1 = new TGraph( vectY.size(), &(vectZ[0]), &(vectY[0]) );
  gr1->SetMarkerColor(2);
  gr1->SetMarkerStyle(21);
                                                                                    
  std::vector<TGraph*> myGraphs;
  std::map<Int_t, std::vector<Double_t> >::iterator iterMap = mapArray.begin();
                                                                                           
  for( ; iterMap != mapArray.end() ; ++iterMap) {
    //if(iterMap->second.size() != 0 ) {
      TGraph* gr = new TGraph(2,&(iterMap->second[0]), &(iterMap->second[2]) );
      gr->SetMarkerColor(4);
      gr->SetMarkerStyle(21);
      gr->SetLineColor(4);
      gr->SetLineWidth(2);
      myGraphs.push_back(gr);
    //}
  }
                                                                                                                          
  
  for(Int_t i=0 ; i<mapArray.size() ; ++i) {
     mg->Add(myGraphs[i]);
   }
                                                                                                                                      
   mg->Add(gr1);                                                                                                                                           

   mg->Draw("ACP");
 
   c1->Draw();
   
   
   //TAxis* axis = mg->GetYaxis();
   //axis->SetLimits(-10000, 10000);
   
   
   c1->Modified();
   c1->Update();

   std::cout<<"Press enter to continue ... ";
   std::cin.get();
   std::cout<<std::endl;
   
   mg->Delete();                                                                                                                                      
   c1->Close();
 
 } // end if mode==2
*/


  //return miniVectorDataPoints;
}

void MimosaAlignAnalysis::FillMiniVectorsResiduals( std::vector<MiniVector*> MV0, std::vector<MiniVector*> MV1 )
{

  std::cout<<"Number of Associated Mini-vectors : "<<MV0.size()<<std::endl;

  DPrecAlign* ladder1DPrecAlign = fLadders[1]->GetPrecAlign();
  DPrecAlign* ladder0DPrecAlign = fLadders[0]->GetPrecAlign();

  for( Int_t i=0 ; i<int(MV0.size()) ; ++i) {

    // Ladder0 :
    DR3 trackerPositionMV_0 = MV0[i]->GetMiniVectorTrackerCenter();  // current MV ladder0 in tracker frame 
    DR3 ladderPositionMV_0  = MV0[i]->GetMiniVectorLadderCenter();   // current MV ladder0 in ladder frame
    DR3 tiltsMV_0           = MV0[i]->GetMiniVectorDirection();      // current MV direction on ladder0 in tracker frame
      
    // Ladder1 :
    DR3 trackerPositionMV_1 = MV1[i]->GetMiniVectorTrackerCenter();  // current MV ladder1 in tracker frame
    DR3 ladderPositionMV_1  = MV1[i]->GetMiniVectorLadderCenter();   // current MV ladder1 in ladder1 frame
    DR3 tiltsMV_1           = MV1[i]->GetMiniVectorDirection();      // current MV  direction on ladder1 in tracker frame
  
    DPrecAlign* DPrecAlignLeft  = MV1[i]->GetDPrecAlignLeft();
    DPrecAlign* DPrecAlignRight = MV1[i]->GetDPrecAlignRight();

    DPrecAlign* DPrecAlignLeft0  = MV0[i]->GetDPrecAlignLeft();
    DPrecAlign* DPrecAlignRight0 = MV0[i]->GetDPrecAlignRight();

    // Projections :
    DR3 projectionMV0_OnLadder1_TrackerFrame = MV0[i]->CalculateMiniVectorIntersectionTracker(ladder1DPrecAlign);  // projection of current MV of ladder0 on ladder1 in tracKER frame
    DR3 projectionMV0_OnLadder1_LadderFrame  = MV0[i]->CalculateMiniVectorIntersectionLadder(ladder1DPrecAlign);   // projection of current MV of ladder0 on ladder1 in ladder1 frame
    
    DR3 residualsOnLadder1 = ladderPositionMV_1 - projectionMV0_OnLadder1_LadderFrame;
   
    std::vector<Double_t> myData;
    myData.push_back(0); // 0
    myData.push_back(0); // 1
    myData.push_back(0); // 2
    myData.push_back(0); // 3
    myData.push_back(0); // 4
    myData.push_back(0); // 5
    myData.push_back(0); // 6
    myData.push_back( ladderPositionMV_1(0) ); // 7 --> U Ladder 1
    myData.push_back( ladderPositionMV_1(1) ); // 8 --> V Ladder 1
    myData.push_back(0); // 9
    myData.push_back( residualsOnLadder1(0) ); // 10 --> DU On Ladder 1
    myData.push_back( residualsOnLadder1(1) ); // 11 --> DV On Ladder 1
    myData.push_back( residualsOnLadder1(2) ); // 12 --> DW On Ladder 1          
                   
    fLadders[1]->FillResidus(myData, 0);
  
  } // end for

}

void MimosaAlignAnalysis::FillMiniVectorsResidualsMC( std::vector<MiniVector*> MV0, std::vector<MiniVector*> MV1 )
{

  std::cout<<"Number of Associated Mini-vectors : "<<MV0.size()<<std::endl;

  DPrecAlign* ladder1DPrecAlign = fLadders[1]->GetPrecAlign();
  DPrecAlign* ladder0DPrecAlign = fLadders[0]->GetPrecAlign();

  for( Int_t i=0 ; i<int(MV0.size()) ; ++i) {

    // Ladder0 :
    DR3 trackerPositionMV_0 = MV0[i]->GetMiniVectorTrackerCenterMC();  // current MV ladder0 in tracker frame 
    DR3 ladderPositionMV_0  = MV0[i]->GetMiniVectorLadderCenterMC();   // current MV ladder0 in ladder frame
    DR3 tiltsMV_0           = MV0[i]->GetMiniVectorDirectionMC();      // current MV direction on ladder0 in tracker frame
      
    // Ladder1 :
    DR3 trackerPositionMV_1 = MV1[i]->GetMiniVectorTrackerCenterMC();  // current MV ladder1 in tracker frame
    DR3 ladderPositionMV_1  = MV1[i]->GetMiniVectorLadderCenterMC();   // current MV ladder1 in ladder1 frame
    DR3 tiltsMV_1           = MV1[i]->GetMiniVectorDirectionMC();      // current MV  direction on ladder1 in tracker frame
  
    DR3 planeFrameLeft      = MV1[i]->GetHitLeftPlanePositionMC();
    DR3 planeFrameRight     = MV1[i]->GetHitRightPlanePositionMC();

    DR3 realPlaneFrameLeft  = MV1[i]->GetRealLeftPlaneFrameMC();
    DR3 realPlaneFrameRight = MV1[i]->GetRealRightPlaneFrameMC();

    DPrecAlign* DPrecAlignLeft  = MV1[i]->GetDPrecAlignLeft();
    DPrecAlign* DPrecAlignRight = MV1[i]->GetDPrecAlignRight();

    DPrecAlign* DPrecAlignLeft0  = MV0[i]->GetDPrecAlignLeft();
    DPrecAlign* DPrecAlignRight0 = MV0[i]->GetDPrecAlignRight();

    // Projections :
    DR3 projectionMV0_OnLadder1_TrackerFrame = MV0[i]->CalculateMiniVectorIntersectionTrackerMC(ladder1DPrecAlign);  // projection of current MV of ladder0 on ladder1 in tracKER frame
    DR3 projectionMV0_OnLadder1_LadderFrame  = MV0[i]->CalculateMiniVectorIntersectionLadderMC(ladder1DPrecAlign);   // projection of current MV of ladder0 on ladder1 in ladder1 frame

    DR3 residualsOnLadder1 = ladderPositionMV_1 - projectionMV0_OnLadder1_LadderFrame;
   
    std::vector<Double_t> myData;
    myData.push_back( residualsOnLadder1(0) ); // 10 --> DU On Ladder 1
    myData.push_back( residualsOnLadder1(1) ); // 11 --> DV On Ladder 1
    myData.push_back( residualsOnLadder1(2) ); // 12 --> DW On Ladder 1          
                   
    fLadders[1]->FillResidusMC(myData, 0);
  
  } // end for

}

//______________________________________________________________________________
//
void  MimosaAlignAnalysis::UpdateConfAlign2D(Int_t nSecPlanes, DPlane** SecPlane)
{
  fstream  configFileOld;
  fstream  configFileNew;
  Char_t configFileName[5000];
  sprintf(configFileName,"%s/run%d.cfg", /*DSession::Instance()*/fSession->GetConfigPath().Data(), /*DSession::Instance()*/fSession->GetRunNumber());
  configFileOld.open(configFileName, ios::in);
  sprintf(configFileName,"%s/run%d_new.cfg", /*DSession::Instance()*/fSession->GetConfigPath().Data(), /*DSession::Instance()*/fSession->GetRunNumber());
  printf("MAlign: creating new config file %s\n", configFileName);
  configFileNew.open(configFileName, ios::out);
  Char_t tmp[255];
         
  TString key;
  //  sprintf( update_cfg_string, "%s %d %.3f %.3f %.3f", update_cfg_string, SecPlane[i]->GetPlaneNumber(), SecPlane[i]->GetTiltW()*TMath::RadToDeg(), SecPlane[i]->GetPositionU(), SecPlane[i]->GetPositionV()); // conversion for the tilt: radians->degrees, JB 2010/12/14
         
         
   while (!configFileOld.eof()) {
     configFileOld.getline(tmp, 255);
     key = tmp;
     configFileNew << tmp;
     configFileNew << "\n";
     if (key.Contains("InputNumber:")) {
       Int_t pos = key.First(':');
       TString sIdx = key(pos+1, key.Length());
       Int_t idx = (Int_t)sIdx.Atoi();
       for (Int_t i = 0; i < nSecPlanes; ++i) {
         if (idx == SecPlane[i]->GetPlaneNumber()){
           UpdateConfAlignUVW(configFileOld, configFileNew, SecPlane[i]);
           break;
         }
      }
    } 
  }
  configFileOld.close();
}


//______________________________________________________________________________  
//
void MimosaAlignAnalysis::UpdateConfAlignUVW(fstream &fileIn, fstream &fileOut, DPlane* SecPlane)
{
  Char_t tmp[255];
  TString key;
  while (!fileIn.eof()) {
    fileIn.getline(tmp, 255);
    key = tmp;
    if (key.Contains("AlignementU:")) {
      key = "AlignementU:";
      TString sIdx = Form("       %8.3f\n", SecPlane->GetPositionU());
      TString line = key+sIdx;
      fileOut << line.Data();
      fileIn.getline(tmp, 255);
      key = "AlignementV:";
      sIdx =  Form("       %8.3f\n", SecPlane->GetPositionV());
      line = key + sIdx;
      fileOut << line.Data();
      fileIn.getline(tmp, 255);
      key = "AlignementTilt:";
      sIdx =  Form("    %6.3f\n", SecPlane->GetTiltW()*TMath::RadToDeg());
      line = key + sIdx;
      fileOut << line.Data();
      break;
    }
    else {
       fileOut << tmp;
       fileOut << "\n";
    }
  } 
}


void MimosaAlignAnalysis::Gener(Double_t* xp, Double_t* yp, Double_t& aX, Double_t& bX, Double_t& aY, Double_t& bY, Double_t* sigX, Double_t* disX, Double_t* sigY, Double_t* disY, Double_t* z, Double_t* phi)
{
  aX = 10.*gRandom->Uniform(-1,1);
  bX = gRandom->Gaus(0, 0.3);
  aY = 10.*gRandom->Uniform(-1,1);
  bY = gRandom->Gaus(0, 0.3);

  Int_t nPlanes = 48;

  Double_t x[nPlanes], y[nPlanes];
         
  for (Int_t i = 0; i < nPlanes; ++i) {
    x[i] = (aX + bX*z[i] + sigX[i]*gRandom->Uniform(0,1));
    y[i] = (aY + bY*z[i] + sigY[i]*gRandom->Uniform(0,1));
    //	  Double_t cosPhi = TMath::Cos(phi[i]);
    //	  Double_t sinPhi = TMath::Sin(phi[i]);
           
    //	  xp[i] =  x[i]*cosPhi - y[i]*sinPhi + disX[i];
    //	  yp[i] =  x[i]*sinPhi + y[i]*cosPhi + disY[i];
           
    xp[i] =  x[i]        - y[i]*phi[i] + disX[i];
    yp[i] =  x[i]*phi[i] + y[i]        + disY[i];
  }
}

//______________________________________________________________________________  
//
void MimosaAlignAnalysis::AlignTrackerMillepede(Int_t nAlignEvents)
{
         // Macro performs an alignement of the telescope in the coordinate system of 
         // tracker using the Millipede package
         fSession->SetEvents(nAlignEvents);   
         DTracker* tTracker = fSession->GetTracker();
         Int_t     nPlanes  = tTracker->GetPlanesN();
         
         std::cout<<"Read "<< nPlanes << " planes." << std::endl;
         // events to read
         //fSession->SetEvents(nAlignEvents); // (bellow 5000)
      /*
      // Macro Millepede :)
         Int_t nPlanes = 48;

         Int_t nParPlanes = 3;
         Int_t nLocPar    = 4;
         Int_t nParam     = nParPlanes*nPlanes;


         MMillepede* alignment = new MMillepede();
         alignment->InitMille(nParam, nLocPar, 3, 10, 10);
         alignment->SetIterations(50);

         Double_t z[]    = {5.,    9., 20.,   25.,   30.,   35.,   40.,    45.};
         Double_t sigX[] = {0.02,  0.02, 0.03, 0.03  ,0.03,  0.03,  0.03,   0.03};
         Double_t sigY[] = {0.02,  0.02, 0.03, 0.03,  0.03,  0.03,  0.03,   0.03};

         
         Double_t disX[] = {0.00, -0.04, 0.15, 0.03, -0.075, 0.045, 0.035, -0.08};
         Double_t disY[] = {0.00, -0.03, 0.10, 0.02, -0.070, 0.040, 0.030, -0.07};
       
         Double_t x[nPlanes];
         Double_t y[nPlanes];
         Double_t aX,bX;
         Double_t aY,bY;
         
         Double_t parameters[nParam];
         Double_t errors[nParam];
         Double_t pulls[nParam];
         Double_t gloPar[nParam];
         Double_t locPar[nLocPar];
         Double_t gloConst[nParam];

         Double_t phi[] = {0., 2., -1., 1., 1., -1., 3., 1.};
        // Double_t phi[] = {0,0,0,0,0,0,0,0};
         
         for(Int_t k = 0; k < nPlanes; ++k)  {
           phi[k] = phi[k]*TMath::DegToRad();
           disX[k] += 2;
           disY[k] += 1;
         }
         
         for(Int_t k = 0; k < nParam; ++k) {
           parameters[k]=0.;
           errors[k]=0.;
           pulls[k]=0.;
         }
            
         gloConst[0] = 0;
         gloConst[1] = 0;
         gloConst[2] = 0;

         for(Int_t k = 1; k < nPlanes; ++k) {
           gloConst[nParPlanes*k+0] = 0.;
           gloConst[nParPlanes*k+1] = 0.;
           gloConst[nParPlanes*k+2] = 0.;
           alignment->SetParSigma(nParPlanes*k+0, 0.05);
           alignment->SetParSigma(nParPlanes*k+1, 0.05);
           alignment->SetParSigma(nParPlanes*k+2, 0.02);
         }
         
         alignment->SetParSigma(0, 0.0);
         alignment->SetParSigma(1, 0.0);
         alignment->SetParSigma(2, 0.0);
         
         Double_t trackParams[nLocPar*2];
         for(Int_t k = 0; k < nLocPar*2; ++k) 
           trackParams[k] = 0.;

        // alignment->SetGlobalConstraint(gloConst, 0.0);
         alignment->SetGlobalParameters(parameters);
            
         for (Int_t iEvent = 0; iEvent < nAlignEvents; ++iEvent) {
           Gener(x, y, aX, bX, aY, bY, sigX, disX, sigY, disY, z, phi);
           for (Int_t iPlane = 0; iPlane < nPlanes; ++iPlane) {
             locPar[0] = 1;
             locPar[1] = z[iPlane];
             locPar[2] = 0;
             locPar[3] = 0;
             gloPar[nParPlanes*iPlane+0] = 0;
             gloPar[nParPlanes*iPlane+1] = 0;
             gloPar[nParPlanes*iPlane+2] = 0;//-y[iPlane];//-(aY + bY*z[iPlane]);
             alignment->SetLocalEquation(gloPar, locPar, x[iPlane], sigX[iPlane]);
             locPar[0] = 0;
             locPar[1] = 0;
             locPar[2] = 1;
             locPar[3] = z[iPlane];
             gloPar[nParPlanes*iPlane+0] = 0;
             gloPar[nParPlanes*iPlane+1] = 0;
             gloPar[nParPlanes*iPlane+2] = 0;//x[iPlane];//(aX + bX*z[iPlane]);
             alignment->SetLocalEquation(gloPar, locPar, y[iPlane], sigY[iPlane]);
           }
           
           alignment->LocalFit(iEvent, trackParams, 1);
           
           for (Int_t iPlane = 0; iPlane < nPlanes; ++iPlane) {
             locPar[0] = 1;
             locPar[1] = z[iPlane];
             locPar[2] = 0;
             locPar[3] = 0;
             gloPar[nParPlanes*iPlane+0] = 1;
             gloPar[nParPlanes*iPlane+1] = 0;
             gloPar[nParPlanes*iPlane+2] = -(trackParams[4]+trackParams[6]*z[iPlane]);
             alignment->SetLocalEquation(gloPar, locPar, x[iPlane], sigX[iPlane]);
             locPar[0] = 0;
             locPar[1] = 0;
             locPar[2] = 1;
             locPar[3] = z[iPlane];
             gloPar[nParPlanes*iPlane+0] = 0;
             gloPar[nParPlanes*iPlane+1] = 1;
             gloPar[nParPlanes*iPlane+2] = (trackParams[0]+trackParams[2]*z[iPlane]);
             alignment->SetLocalEquation(gloPar, locPar, y[iPlane], sigY[iPlane]);
           }
           
           Int_t iRes = alignment->LocalFit(iEvent, trackParams, 0);

      //	  printf("Res %d param %6.3f %6.3f %6.3f %6.3f    %6.3f %6.3f %6.3f %6.3f\n", iRes,
      //			 aX, trackParams[0], bX, trackParams[2], 
      //			 aY, trackParams[4], bY, trackParams[6] );
           if (iRes)
             alignment->SetNLocalEquations(alignment->GetNLocalEquations()+1);
         }// event
         alignment->GlobalFit(parameters, errors, pulls);   
         
         for (Int_t iPlane = 0; iPlane < nPlanes; ++iPlane) {
           printf("Plane %2d: DisplacementX %7.4f %7.4f %7.4f  ", iPlane+1, disX[iPlane], parameters[nParPlanes*iPlane+0]+2, errors[nParPlanes*iPlane+0]);
           printf("DisplacementY %7.4f %7.4f %7.4f   ", disY[iPlane], parameters[nParPlanes*iPlane+1]+1, errors[nParPlanes*iPlane+1]);
           printf("Rotation %7.4f %7.4f\n", phi[iPlane], parameters[nParPlanes*iPlane+2]);
         }
      */

        DPlane **Planes = new DPlane*[nPlanes];

        for(Int_t i=1; i<=nPlanes; ++i){
          Planes[i] =  tTracker->GetPlane(i);
          //SecPlane[i]->GetAlignment()->SetBounding(tiniBound);
        }

         //init Millipede
        
         Int_t nParPlanes = 3;
         Int_t nParam     = nParPlanes*nPlanes;
         Int_t nLocPar    = 4;
         Int_t stdDev     = 3;

         Double_t parameters[nParam];
         Double_t errors[nParam];
         Double_t pulls[nParam];
         Double_t gloConst[nPlanes];
         
         for(Int_t k = 0; k < nParam; ++k) {
           parameters[k] = 0.;
           errors[k]     = 0.;
           pulls[k]      = 0.;
           gloConst[k]   = 0.;
         }
         
         Double_t trackParams[nLocPar*2];
         for(Int_t k = 0; k < nLocPar*2; ++k) 
           trackParams[k] = 0.;
         
         // init millepede
         fAlignment->Init(nParam, nLocPar, stdDev, nPlanes);
         fAlignment->SetIterations(20);

         for(Int_t k = 1; k <= nPlanes; ++k) {
           DPlane* tPlane = tTracker->GetPlane(k);
           if(k==1) {
              DR3 planePosition = Planes[k]->GetPosition();
              fAlignment->FixParameter(nParPlanes*k-3, /*planePosition(0)*/0.);  // PositionX: -50.73  PositionsY:   0  PositionsZ:     38.9705
              fAlignment->FixParameter(nParPlanes*k-2, /*planePosition(1)*/0.);
              fAlignment->FixParameter(nParPlanes*k-1, /*planePosition(2)*/0.1);
           } 
           else {
              DR3 planePosition = Planes[k]->GetPosition();
              Double_t tiltW = Planes[k]->GetTiltW();
              parameters[nParPlanes*k-3] = planePosition(0);
              parameters[nParPlanes*k-2] = planePosition(1);
              parameters[nParPlanes*k-1] = tiltW;

              //fAlignment->FixParameter(nParPlanes*k+0, planePosition(0) );
              //fAlignment->FixParameter(nParPlanes*k+1, planePosition(1) );
              //fAlignment->FixParameter(nParPlanes*k+2, planePosition(2) );
           }
               
             if (tPlane->GetStatus() == 1) {
             fAlignment->FixPlane(k);
           } else {
             fAlignment->FixParameter(nParPlanes*k+0, 0.);
             fAlignment->FixParameter(nParPlanes*k+1, 0.);
             fAlignment->FixParameter(nParPlanes*k+2, 0.2);
           }
           
           }

         // fix first plane.  
         fAlignment->FixPlane(0);

         fAlignment->InitGlobalParameters(parameters);
         fAlignment->AddConstraint(gloConst, 0.0);
         
         // loop over events
         Int_t nTrack = 0;
         
         while(fSession->NextRawEvent() == kTRUE) {    
           
           tTracker->Update();
           
           for( Int_t iTrack = 0; iTrack < tTracker->GetTracksN(); ++iTrack ) { 
             DTrack* aTrack = tTracker->GetTrack(iTrack);
             
             //Double_t ux = aTrack->GetLinearFit().GetOrigin()(0);
             //Double_t slopeX = aTrack->GetLinearFit().GetSlopeZ()(0);
             //Double_t slopeY = aTrack->GetLinearFit().GetSlopeZ()(1);
             //std::cout<<"ux = "<<ux<<"   slopeX = "<< slopeX<<" slopeY = "<< slopeY <<std::endl;

      //		 if (fabs(slopeX) < 5e-3 || fabs(slopeY) < 5e-3) continue;
               
             fAlignment->ProcessTrack(aTrack);
             fAlignment->LocalFit(nTrack,trackParams,1);
             
             fAlignment->ProcessTrack(aTrack, trackParams);
             fAlignment->LocalFit(nTrack++,trackParams,0);
         
            /* 
             printf("param %6.3f %6.3f %e %e    %6.3f %6.3f %e %e\n",
                  aTrack->GetLinearFit().GetOrigin()(0), trackParams[0], aTrack->GetLinearFit().GetSlopeZ()(0), trackParams[2], 
                  aTrack->GetLinearFit().GetOrigin()(1), trackParams[4], aTrack->GetLinearFit().GetSlopeZ()(1), trackParams[6] );
            */
           } // end loop on tracks
               
         }   
         fAlignment->GlobalFit(parameters,errors,pulls);
            
         // update
         Double_t       tNewTiltW;
         Double_t       tNewOffsetU;
         Double_t       tNewOffsetV;
         
         DPlane **secPlane = new DPlane*[nPlanes];

         for(Int_t i = 0; i < nPlanes; i++) {
           secPlane[i] =  tTracker->GetPlane(i+1);
           tNewOffsetU = secPlane[i]->GetPositionU() - parameters[i*fAlignment->GetNParPlane() + 0];
           std::cout<<"tNewOffsetU = "<<tNewOffsetU<<std::endl;
           
           secPlane[i]->SetPositionU(tNewOffsetU);
           tNewOffsetV = secPlane[i]->GetPositionV() - parameters[i*fAlignment->GetNParPlane() + 1];
           std::cout<<"tNewOffsetV = "<<tNewOffsetV<<std::endl;

           secPlane[i]->SetPositionV(tNewOffsetV);
           tNewTiltW   = secPlane[i]->GetTiltW()     - parameters[i*fAlignment->GetNParPlane() + 2];
           secPlane[i]->SetTiltW(tNewTiltW);
           std::cout<<"tNewTiltW  = "<<tNewTiltW<<std::endl;
         }
         
         UpdateConfAlign2D(nPlanes, secPlane);
         delete [] secPlane;

      }

//______________________________________________________________________________
//
void MimosaAlignAnalysis::AlignTrackerGlobal(const Int_t refPlane, const Int_t alignEvents, const Int_t iterationNumber, const Bool_t alignConstrainsts, const Bool_t trackConstrainsts, const Bool_t multipleScatteringFit)
{

  // L. Cousin 2015 : Global Align Method For Telescope Alignment.

  // This method provides global alignment for a telscope geometry.
  //
  // General description :
  // The method in adapted from ATLAS global alignement method.
  // This method fit simultaneously the alignement and the tracks pramaters. 
  // The method in adapted from ATLAS global alignement method.
  // Matrix form of chi2 and linear least square method is used. (See ATLAS Papers)
  //
  // Constraints :
  // This method also provides constraints for tracks and alignment parameters. 
  // Constrainsts are added from two new chi2. One for alignement parameters and another for tracks parameters.
  // The idea for constrainsts is to fix the parameters value with a certain resolution using new chi2.
  //
  // Mutiple scattering effects for tracks are included but not tested. For that we define a new global covariance matrix and a new global residual.

  // In order to compose with non linearities and multiple scattering, several iteration of the method can be necessary.

  // Some comments about evolution for this method :
  //
  // Dynamic Form for Plane gestion. --> Add/Remove Planes with map. ==> the best way to align :)


  // About fixed parameters :
  // --> For alignment parameters : hard coded.
  //     So, we can add a function or a new part in the config file to fix these parameters. To be discuss.
  //
  // --> For track parameters : hard coded. 
  //     Convergence strongly depend on this part. 
  //     So we can improved this part with a new function to fix track parameters or we can add a new part in the config file to fix these parameters. To be discuss.

  // Marketing : 
  //
  // Fill and Draw some plots with residuals, chi2, etc ...
  // Residuals plots and track parameters plots already included. Can be improved with colors and more aesthetic plots, etc ...
  // Iterations (with only 1 telescope) : plots with value at each iteration already included.


  std::cout<<"Welcome in AlignTrackerGlobal. This method perform the global algnment of your telescope setup :)"<<std::endl;
  
  DTracker* tTracker = fSession->GetTracker();
  DSetup* fc = tTracker->GetSetup();
  Int_t planeNumber  = tTracker->GetPlanesN();
  std::vector<Int_t> fixedPlanes;
 
  std::cout<<"Global Alignment Procedure with "<<planeNumber<<" Planes"<<std::endl;
  
  for(int p=1 ; p<=planeNumber ; ++p) {
    if(fc->GetPlanePar(p).FixedGlobalAlign == 1) { 
      fixedPlanes.push_back(p);
      std::cout<<"fixed Plane Index : "<<p<<std::endl;
    }
  }
  
  std::map<const Int_t, DPlane*> mapPlanes;
  std::map<Int_t, Int_t> mapPlanesIndex;
  std::map<Int_t, Int_t> mapFixedPlanesIndex;

  std::map<Int_t, DPlane*> mapPlane_OrderZ;

  for(Int_t i=0 ; i<planeNumber; ++i) {

    DPlane* aPlane = tTracker->GetPlane(i+1);
    Int_t planeReadout = aPlane->GetReadout();

    if(planeReadout>0) {
      
      DPrecAlign* alignParam = aPlane->GetPrecAlignment();
      DR3 translation = alignParam->GetTranslationsDR3();
      Double_t positionZ = translation(2);
      Int_t positionZ_Int = Int_t(positionZ);
       
      if(mapPlane_OrderZ.count(positionZ_Int) == 0) mapPlane_OrderZ[positionZ_Int] = aPlane;
      else {
       while(mapPlane_OrderZ.count(positionZ_Int) != 0)  positionZ_Int += 1;
       mapPlane_OrderZ[ Int_t(positionZ_Int) ] = aPlane; 
      }
    }

  }

  // Order Plane By Z Value. Plane index mapped 1..N by Z value.

  Int_t index = 0;

  std::map<Int_t, DPlane*>::iterator itPlane = mapPlane_OrderZ.begin();
  for( ; itPlane!=mapPlane_OrderZ.end() ; ++itPlane) {
    
    DPlane* myPlane = itPlane->second;
    Int_t realPlaneIndex = myPlane->GetPlaneNumber();
    
    Bool_t isFixed = 0;
    for(int j=0 ; j<int(fixedPlanes.size()) ; ++j) if(realPlaneIndex == fixedPlanes[j]) isFixed = 1;
  
    mapPlanes[index+1] = myPlane;
    mapPlanesIndex[realPlaneIndex] = index+1;

    if(isFixed) mapFixedPlanesIndex[realPlaneIndex] = index+1;
    
    if(!isFixed) std::cout<<" Pl"<<realPlaneIndex<<" Mapped to Pl"<<index+1<<std::endl;
    else std::cout<<" Pl"<<realPlaneIndex<<" Mapped to Pl"<<index+1<<" Fixed Plane"<<std::endl;

    index++;

  }

  if(mapFixedPlanesIndex.count(refPlane)==0) mapFixedPlanesIndex[refPlane] = mapPlanesIndex[refPlane];

  planeNumber = mapPlanes.size();
  
  std::cout<<"Alignment with "<< planeNumber << " planes" << std::endl;

  // Constraints from config file :
  Double_t FixTrackParamX    = fc->GetGlobalAlignPar().FixTrackParamX;
  Double_t FixTrackParamY    = fc->GetGlobalAlignPar().FixTrackParamY;
  Double_t ResoTrackParamX   = fc->GetGlobalAlignPar().ResoTrackParamX;
  Double_t ResoTrackParamY   = fc->GetGlobalAlignPar().ResoTrackParamY;
  Double_t ResoAlignParamTr  = fc->GetGlobalAlignPar().ResoAlignParamTr;
  Double_t ResoAlignParamRot = fc->GetGlobalAlignPar().ResoAlignParamRot;
  Double_t ResoAlignParamZ   = fc->GetGlobalAlignPar().ResoAlignParamZ; 
  Double_t ResoFixPlaneTr    = fc->GetGlobalAlignPar().ResoFixPlaneTr;
  Double_t ResoFixPlaneRot   = fc->GetGlobalAlignPar().ResoFixPlaneRot;

  // Definition of telescope planes :
  //DPlane **Planes = new DPlane*[planeNumber];

  // Definition : Number of Parameters :
  Int_t trackParametersNumber = 4;   // dirX, dirY, dirZ; with dirZ=1.(dirZ not fitted) ==> 2 parameters + OriginX et OriginY
  Int_t planeDoF = 6 ;               // 3 translations (Param0=TrX, Param1=TrY, Param2=TrZ) 
                                     // 3 Rotations (Param3=RotX, Param4=RotY, param5=RotZ)
  Int_t axisNumberInFrame = 2;       // U and V in local frame
  Double_t bound = 25.;              // Set bound to print U/V Residuals of each plane. (To set the width of the residuals plots)

  // Method iterations :
  for(Int_t alignIterations=0 ; alignIterations<=iterationNumber ; ++alignIterations)
  {
 
    if(alignIterations != 0) fSession->ResetDaq();
    fSession->SetEvents(alignEvents);

    if(iterationNumber>1 && alignIterations==0) {
    
    // TGraphs to print values evolution after each ieration.
    alignParametersIterations = new TGraph*[planeDoF*planeNumber];
     
      for(Int_t p=0 ; p<planeNumber ; ++p) {
          
        for(Int_t i=0 ; i<planeDoF ; ++i) {

          TString paramName;
          paramName += "Pl";
          paramName += planeNumber;
          paramName += "_"; 
          if(i==0) paramName = "translationX";
          else if(i==1) paramName = "translationY";
          else if(i==2) paramName = "translationZ";
          else if(i==3) paramName = "rotationX";
          else if(i==4) paramName = "rotationY";
          else if(i==5) paramName = "rotationZ";
          
          alignParametersIterations[planeDoF*p+i] = new TGraph();
          alignParametersIterations[planeDoF*p+i]->SetTitle( paramName.Data() );
        }
      }
    }
   
    if(alignIterations!=iterationNumber) std::cout<<"Global Alignment :: Starting Iteration number "<<alignIterations<<std::endl;
    else std::cout<<"Re-process tracks for Residuals plots."<<std::endl;

    MimosaGlobalAlign* globalAlignment = new MimosaGlobalAlign(planeNumber, planeDoF, trackParametersNumber, axisNumberInFrame, bound, alignConstrainsts, trackConstrainsts, multipleScatteringFit);  
    // Global align method in class MimosaGlobalAlign.
    // Definition --> set the matrices size
    // Boolean to enable/disable align/track parameters constrainsts and multiple scattering for track fitting.

    // Fill initial alignment parameters :
    // Fix all parameters with the initial value with a big resolution (the parameters are free)  (ConstraintResiduals = 0.)
    globalAlignment->SetInitialAlignementParameters( mapPlanes, ResoAlignParamTr, ResoAlignParamRot );
    if(alignIterations!=iterationNumber) std::cout<<"Iteration number "<<alignIterations<<" --> Initial Alignment Parameters Filled"<<std::endl;
    else std::cout<<"Filling Residuals Plots : --> Initial Alignment Parameters Filled"<<std::endl;

    // To FIX a VALUE for a PARAMETER : first set the value, after set the resolution associated to this value.
    // By default the value is set to 0 in SetInitialAlignementParameters( mapPlanes, planeNumber);
    // By default : Resolution : Transalation : 100 um / Rotation : 0.1 rad.
    // planeIndex start from 1.
    // parameterIndex start from 0.
    // 0 --> X | 1 --> Y | 2 --> Z
    // 3 --> RotX | 4 --> Rot Y | 5 --> rotZ
    // To set a new value for a given parameter, use :
    // globalAlignment->SetConstraintValueOnParameter(Int_t planeIndex, Int_t parameterIndex, Double_t value);

    // To Fix Parameters --> FixParameter(planeIndex, parameterIndex, sigma);
    // Sigma represent the resolution on the parameter value (100um, 0.1 rad, etc ).
    // If sigma --> 0   : infinite precision on the parameter = fixed parameter. // To be superior to 1e-8 for inversion
    // If sigma --> inf : no precision on the parameter value = free parameter.
    // Ex : FixParameter(2, 2, 1e-3) --> Fix TranslationZ for plane 2
    // for(Int_t iterPlane=1 ; iterPlane<=planeNumber ; ++iterPlane ) globalAlignment->FixParameter(iterPlane, 2, 1e-3);  // Fix all Z positions
 
    std::cout<<" Reference Plane = Plane "<<refPlane<<std::endl;
    Int_t refPlaneIndex = mapPlanesIndex[refPlane]; // To convert the the plane index to the plane index of global alignment
    std::cout<<" Reference Plane : Index = "<<refPlaneIndex<<std::endl;

    // Verbose : align/track parameters constrainsts enable/disable.
    if( globalAlignment->IfAlignConstrainsts()==true ) std::cout<<"Alignment Constrainsts Enable !"<<std::endl;
    else std::cout<<"Alignment Constrainsts Disable !"<<std::endl;

    if( globalAlignment->IfTrackConstrainsts()== true ) std::cout<<"Track Constrainsts Enable !"<<std::endl;
    else std::cout<<"Track Constrainsts Disable !"<<std::endl;

    // To Fix Planes or Alignment paramters (if IfAlignConstrainsts()==true) :
    if( globalAlignment->IfAlignConstrainsts()==true ) {
      
      // To fix the reference planes :
      std::map<Int_t, Int_t>:: iterator planeIt = mapFixedPlanesIndex.begin();
      
      for( ; planeIt!=mapFixedPlanesIndex.end() ; ++planeIt ) {

        globalAlignment->FixParameter(planeIt->second, 0, ResoFixPlaneTr);
        globalAlignment->FixParameter(planeIt->second, 1, ResoFixPlaneTr);
        globalAlignment->FixParameter(planeIt->second, 2, ResoFixPlaneTr);
        globalAlignment->FixParameter(planeIt->second, 3, ResoFixPlaneRot);
        globalAlignment->FixParameter(planeIt->second, 4, ResoFixPlaneRot);
        globalAlignment->FixParameter(planeIt->second, 5, ResoFixPlaneRot);
      
      }

      for(Int_t iterPlane=1 ; iterPlane<=planeNumber ; ++iterPlane ) if(iterPlane!=refPlaneIndex) globalAlignment->FixParameter(iterPlane, 2, ResoAlignParamZ);  // To fix all Z positions.
    }

    /*
    globalAlignment->FixParameter(refPlaneIndex, 3, 5e-3); // release tilts of reference plane
    globalAlignment->FixParameter(refPlaneIndex, 4, 5e-3);    
    globalAlignment->FixParameter(refPlaneIndex, 5, 5e-3);
    */
       
    // To Fix Track Parameters :
    // Global definition :
    //FixTrackDirections(0., 0.);
    //FixTrackResolution(1.,1.);
     
    // LOOP ON TRACKS AND PLANES.
    
    std::cout<<"Global Alignment :: Iteration Number "<<alignIterations<<" --> Filling Matrices ..."<<std::endl; 
    std::cout<<std::endl;

    Int_t trackCounter = 0;
    Int_t hitsCounter  = 0;

    while(fSession->NextRawEvent() == kTRUE) {    
           
      tTracker->Update();
      
      Int_t iTrack = 1;
      DTrack* aTrack;
       
      while ( iTrack <= tTracker->GetTracksN() ) {   // while there is still another track
      
        aTrack = tTracker->GetTrack( iTrack++); //tTracker->GetPrincipalTrack();	
        // We need a test to see the telescope number ==> We have to test the plane index of the track to get the telescope number.

        // If enable, To Fix Track Direction, Track by Track : 
        if( globalAlignment->IfTrackConstrainsts()==true ) {
         
          globalAlignment->FixTrackDirections(FixTrackParamX, FixTrackParamY);
          globalAlignment->SetTrackDirectionResolution(ResoTrackParamX, ResoTrackParamY); // Around 10 times the parameter width
        }

        globalAlignment->ProcessTrack(aTrack);
        
        trackCounter++;
        hitsCounter += aTrack->GetHitsNumber();

      } // end loop on tracks             

    } // end loop on events

    std::cout<<trackCounter<<" tracks processed !"<<std::endl;
    std::cout<<"Total hit number processed : "<<hitsCounter<<std::endl;

    // To print residuals at the begining and the end.
    if(alignIterations==0 || alignIterations==iterationNumber) globalAlignment->PrintResiduals();
    Int_t testAlignment = 0;
    
    globalAlignment->PrintResults(alignIterations);

    while(testAlignment!=1) {

      if(alignIterations>0 && alignIterations<iterationNumber) { testAlignment=1; break; }
      
      if(alignIterations==0) std::cout<<"Compute Alignment ? [1/0]"<<std::endl;
      else std::cout<<"Compute New Iteration ? [0/1]"<<std::endl;
      
      std::cin>>testAlignment;
      
      if(testAlignment==0) break;
    }

    if(testAlignment==0) break;

    // Matrix Inversion :
    std::cout<<std::endl;
    std::cout<<"Iteration number "<<alignIterations<<" --> Matrix Inversion ..."<<std::endl;
    globalAlignment->InvertFinalMatrix();

    // SVD Decomposition for weak modes analisys and matrix inversion (Not fully implemented.)
    std::cout<<"Iteration number "<<alignIterations<<" --> SVD Decomposition ..."<<std::endl;
    globalAlignment->DecomposeSVD_FinalMatrix();
    
    // To compute alignment parameters corrections :
    std::cout<<"Iteration number "<<alignIterations<<" --> Compute Alignment Corrections"<<std::endl;
    globalAlignment->ComputeAlignmentCorrections();
    
    //std::cout<<" Alignment Corrections : "<<std::endl;
    //globalAlignment->PrintAlignmentCorrections();
 
    //std::cout<<" New Alignment Parameters : "<<std::endl;
    //globalAlignment->PrintNewAlignmentParameters();
   
    globalAlignment->PrintResults(alignIterations);

    std::cout<<"Set New Alignment Parameters ..."<<std::endl;
    globalAlignment->SetNewAlignmentParameters();

    // Fill Result graph in function of iteration number :
    if(iterationNumber>1) {
      
      for(Int_t p=0 ; p<planeNumber ; ++p) {
      
        for(Int_t i=0 ; i<planeDoF ; ++i) {
          alignParametersIterations[p*planeDoF+i]->SetPoint(alignIterations, alignIterations, globalAlignment->GetParameterNewValue(p+1, i) );
        }
      }
    }

    delete globalAlignment;
  
  } // end loop on iterations
 
  
  if(iterationNumber>1) {
 
    std::cout<<"Print parameters values vs iterations ..."<<std::endl;
  
    alignParamsCanvas = new TCanvas*[planeDoF];

    for(Int_t p=0 ; p<planeNumber ; ++p) {
      TString name = "Plane_";
      name+= p+1;
      TString title = "Alignment Parameters Plane";
      title += p+1;
      alignParamsCanvas[p] = new TCanvas(name.Data(),title.Data(),410,10,750,500);
      alignParamsCanvas[p]->Divide(3,2);
      alignParamsCanvas[p]->Update();
  
      for(Int_t i=0 ; i<planeDoF; ++i) {

        alignParamsCanvas[p]->cd(i+1);

        alignParametersIterations[p*planeDoF+i]->SetMarkerStyle(21);
        alignParametersIterations[p*planeDoF+i]->SetMarkerColor(2);

        alignParametersIterations[p*planeDoF+i]->Draw("AP");
      }

      alignParamsCanvas[p]->Modified();
      alignParamsCanvas[p]->Update();

    }
  }
  
  Bool_t test = 0;

  while(test!=1) {
    std::cout<<"Write Alignment Parameters ? (0/1)"<<std::endl;
    std::cin>>test;
    if(test==0) break;
  }
  
  //-----------------------------------
  // call the script to modify the runXXX.cfg file
  // Handle any number of planes
  // Only if there was really a fit (nAlignEvents>0)
  
  if( alignEvents>0 && test==1) {

    cout << endl << "***************************************" << endl;
    cout << endl << "Applying results ...." << endl;
    Char_t update_cfg_string[1000];
    
    sprintf(update_cfg_string,"Update_conf_align2D %s/run%d.cfg", fSession->GetConfigPath().Data(), fSession->GetRunNumber());
   
    std::map<const Int_t, DPlane*>::iterator it = mapPlanes.begin();
    
    for( ; it!=mapPlanes.end() ; ++it){ // loop on secondary ref planes
     
      sprintf( update_cfg_string, "%s %d %10.6f %10.6f %10.6f %10.6f %10.6f %10.6f", update_cfg_string, it->second->GetPlaneNumber(), it->second->GetPrecAlignment()->GetTranslation()[0]/1000., it->second->GetPrecAlignment()->GetTranslation()[1]/1000., it->second->GetPrecAlignment()->GetTranslation()[2]/1000., it->second->GetPrecAlignment()->GetRotations()[0]*TMath::RadToDeg(), it->second->GetPrecAlignment()->GetRotations()[1]*TMath::RadToDeg(), it->second->GetPrecAlignment()->GetRotations()[2]*TMath::RadToDeg());
    }

    sprintf(update_cfg_string,"%s", update_cfg_string); // add "-d" at the end for debug
    
    cout<<update_cfg_string<<endl;
    
    gSystem->Exec(update_cfg_string);
  }

  //-----------------------------------
  // The End

}


//______________________________________________________________________________
//

void FCNAlignMimosa(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag)
{
  // static fcn wrapper for Minuit

  //MimosaAlignAnalysis::Instance()->fcn(n,gin,f,par,iflag);
   MimosaAlignAnalysis::Instance()->fcn(n, gin, f, par , iflag); 
  //fgInstance->fcn(n,gin,f,par,iflag);

} 

//______________________________________________________________________________
//
void FCNAlignMimosaLadder(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag)
{
  // static fcn wrapper for Minuit

  //MimosaAlignAnalysis::Instance()->fcn(n,gin,f,par,iflag);
   MimosaAlignAnalysis::Instance()->fcnLadder(n, gin, f, par , iflag); 
  //fgInstance->fcn(n,gin,f,par,iflag);

}

//______________________________________________________________________________
//
void FCNAlignMimosaLadder2(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag)
{
  // static fcn wrapper for Minuit
   MimosaAlignAnalysis::Instance()->fcnLadder2(n, gin, f, par , iflag); 

}

//______________________________________________________________________________
//
/*
void FCNAlignMimosaMV(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag)
{
  // static fcn wrapper for Minuit

  //MimosaAlignAnalysis::Instance()->fcn(n,gin,f,par,iflag);
   MimosaAlignAnalysis::Instance()->fcnMiniVectors(n, gin, f, par , iflag); 
  //fgInstance->fcn(n,gin,f,par,iflag);

}
*/
//______________________________________________________________________________
//
void  MimosaAlignAnalysis::fcn(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag)
{
  // fcn function used by the MINUIT Fitter
  // 2015/03/09, AP Adding hit resolution U and V to chi2
       
  DR3 CurPoint;
  DR3 CurHit;
  DR3 Correction;

  //Double_t resolution[3] = {5,5,100.0} ; // ??
 
  if(AlignDebug >= 3) 
  { 
    printf("fcn: Rotations : %11f %11f %11f\n",par[0],par[1],par[2]);
    printf("fcn: Translat. : %11f %11f %11f \n",par[3],par[4],par[5]);
    cout << "+++++++++++++++++++++++++++++++" << endl;
  }  
       
  fAlignement->SetTranslation(par[3],par[4],par[5]);
  fAlignement->SetRotations(par[0],par[1],par[2]);
  if(AlignDebug >= 3) 
  { 
    fAlignement->SetDebug(1);
    fAlignement->PrintAll();
    cout << "+++++++++++++++++++++++++++++++" << endl;
  }
  //Compute the chi-square
  f=0.0 ;

  TList *data = fAlignement->GetDataPoints() ;

  if (iflag==8) {
    fDu->Reset();
    fDv->Reset();
  }

  // cout << "+++++++++++++++++++++++++++++++" << endl;
  DataPoints *current ;
  current = (DataPoints*) data->First();
  Int_t ipt=0 ;

  while(current) {
    // CurPoint = fAlignement->TransformTrackToPlane(current);
    CurHit   = current->GetHitPosition(); // get hit position measured in local system
    
    if(AlignDebug >= 3) { cout << "fcn: got hit position, x=" << CurHit(0) << ", y=" << CurHit(1) << ", z=" << CurHit(2) << endl;}
    
    DR3 dirTrack      = current->GetTrackDirection();
    DR3 hitresolution = current->GetHitResolution();
    
    DR3 dirTrackNormalized(dirTrack(0), dirTrack(1), dirTrack(2));
    dirTrackNormalized(0) /= sqrt( dirTrack(0)*dirTrack(0) + dirTrack(1)*dirTrack(1) + dirTrack(2)*dirTrack(2) );
    dirTrackNormalized(1) /= sqrt( dirTrack(0)*dirTrack(0) + dirTrack(1)*dirTrack(1) + dirTrack(2)*dirTrack(2) );
    dirTrackNormalized(2) /= sqrt( dirTrack(0)*dirTrack(0) + dirTrack(1)*dirTrack(1) + dirTrack(2)*dirTrack(2) );
    
    /*
     Double_t roll  = atan2(dirTrackNormalized(1), dirTrackNormalized(2)); 
     Double_t pitch = atan2(dirTrackNormalized(0), dirTrackNormalized(2));
     
     Double_t diffAngleX = par[2]*180./TMath::Pi() - roll*180./TMath::Pi();
     Double_t diffAngleY = par[1]*180./TMath::Pi() - pitch*180./TMath::Pi(); 
     
     diffAngleX = fabs(diffAngleX);
     diffAngleY = fabs(diffAngleY);
     
     if(diffAngleX<=2.5) resolution[1] = 3.5;
     else if(diffAngleX>2.5 && diffAngleX<=5) resolution[1] = 3.7;
     else if(diffAngleX>5  && diffAngleX<=25) resolution[1] = 3.9;
     else if(diffAngleX>25 && diffAngleX<=30) resolution[1] = 4.0;
     else if(diffAngleX>30 && diffAngleX<=40) resolution[1] = 4.25;
     else if(diffAngleX>40 && diffAngleX<=50) resolution[1] = 4.4;
     else if(diffAngleX>50 && diffAngleX<=60) resolution[1] = 4.75;
     else if(diffAngleX>60) resolution[1] = 5.0; 
     
     if(diffAngleY<=2.5) resolution[0] = 3.5;
     else if(diffAngleY>2.5 && diffAngleY<=5) resolution[0] = 3.7;
     else if(diffAngleY>5  && diffAngleY<=25) resolution[0] = 3.9;
     else if(diffAngleY>25 && diffAngleY<=30) resolution[0] = 4.0;
     else if(diffAngleY>30 && diffAngleY<=40) resolution[0] = 4.25;
     else if(diffAngleY>40 && diffAngleY<=50) resolution[0] = 4.4;
     else if(diffAngleY>50 && diffAngleY<=60) resolution[0] = 4.75;
     else if(diffAngleY>60) resolution[0] = 5.0; 
     */
    
    fAlignement->CalculateIntersection(current);  // get hit position measured by RS in xyz
    CurPoint = fAlignement->TransformTrackToPlane(); // transform xyz -> uv
    if(AlignDebug >= 3) { cout << "fcn: got track position, x=" << CurPoint(0) << ", y=" << CurPoint(1) << ", z=" << CurPoint(2) << endl;}
    
    //for (Int_t i=0 ; i<3 ; i++){
    for (Int_t i=0 ; i<2 ; i++){
      //Double_t tmp = (CurPoint(i)-CurHit(i))/resolution[i] ;
      Double_t tmp = (CurPoint(i)-CurHit(i))/hitresolution(i) ;
      f+= tmp*tmp;
    }
    if (iflag==8) {
      DR3 temp = CurPoint-CurHit ;
      
      if(AlignDebug >= 3) 
      { 
        printf("Point : ") ; CurPoint.Print() ;
        printf("Hit   : ") ; CurHit.Print() ;
        printf("diff  : ") ; temp.Print() ;
        printf("\n") ;
      }
      
      fGraph[0].SetPoint(ipt,CurHit(0),temp(0));
      fGraph[1].SetPoint(ipt,CurHit(0),temp(1));
      fGraph[2].SetPoint(ipt,CurHit(1),temp(0));
      fGraph[3].SetPoint(ipt,CurHit(1),temp(1));
      fDu->Fill(temp(0));
      fDv->Fill(temp(1));
    }
    
    current=(DataPoints*) data->After(current) ;
    ipt++;
  }
  
  if (iflag==9) {
    fAlignement->ListDataPoints();
  }
  if (iflag==10) {
    fGraph[0].Print();
  }

  if (iflag==8) 
  {
    if(AlignDebug >= 3) {fAlignement->PrintRotationMatrix();}
    TPostScript ps("residus.ps"); //,-113
    ps.NewPage();
    //fCnv = new TCanvas("Cnv","Residues after alignment",2,10,400,500);
    //fCnv->Divide(1,2);
    fCnv->cd(1);
    fDu->Fit("gaus","q");
    //fDu->Draw();
    //fCnv->Update();
           
    fCnv->cd(2);
    //gStyle->SetOptFit(1);
    /*
    fMyfit->SetParameter(0,250);
    fMyfit->SetParameter(1,50); // was 10
    fMyfit->SetParameter(2,4);
    fMyfit->SetParameter(3,500);
    fMyfit->SetParameter(4,-20);
    fMyfit->SetParameter(5,5);
    */
    fDv->Fit("gaus","q");
    //fDv->Draw();
    fCnv->Modified();
    fCnv->Update();
    fCnv->Draw();
    // fCnv->Paint();
    ps.NewPage();

    for (Int_t i=0; i<4 ; i++) {
      fCcr->cd(i+1);
      fGraph[i].SetMarkerStyle(24);
      fGraph[i].SetMarkerColor(2);
      fGraph[i].SetMarkerSize(0.2);
      fGraph[i].Draw("AP");
    } 
    fGraph[0].GetHistogram()->SetXTitle("U hit");
    fGraph[0].GetHistogram()->SetYTitle("DU");
    //  fGraph[0].GetHistogram()->SetMinimum(-2000.);
    //  fGraph[0].GetHistogram()->SetMaximum(2000.);
    fGraph[1].GetHistogram()->SetXTitle("U hit");
    fGraph[1].GetHistogram()->SetYTitle("DV");
    // fGraph[1].GetHistogram()->SetMinimum(-2000.);
    // fGraph[1].GetHistogram()->SetMaximum(2000.);
    fGraph[2].GetHistogram()->SetXTitle("V hit");
    fGraph[2].GetHistogram()->SetYTitle("DU");
    // fGraph[2].GetHistogram()->SetMinimum(-2000.);
    // fGraph[2].GetHistogram()->SetMaximum(2000.);
    fGraph[3].GetHistogram()->SetXTitle("V hit");
    fGraph[3].GetHistogram()->SetYTitle("DV");
    // fGraph[3].GetHistogram()->SetMinimum(-2000.);
    //fGraph[3].GetHistogram()->SetMaximum(2000.);
    fCcr->Update();
    fCcr->Draw();
    // fCcr->Paint();

    gROOT->GetListOfCanvases()->Write();
    ps.Close();    
       
  }

  Double_t mypar[3];
  for(Int_t i=0; i<3; i++){
    mypar[i] = par[i]*180.00/3.141592653;
    mypar[i] = fmod(mypar[i],360.);
  }
       
  if(AlignDebug >=3) { printf(" tilt Z= %f \n tilt Y= %f \n tilt Z= %f \n Shift X= %f \n Shift Y= %f \n Z= %f \n N of entries %d Chi2 %f Chi2/DoF %f\n",mypar[0],mypar[1],mypar[2],par[3],par[4],par[5], ipt, f ,f/ipt);}

  //if (batch_mode == 1) cnv->SaveAs("t.ps");
}
      
 
//______________________________________________________________________________
//  
void  MimosaAlignAnalysis::fcnLadder(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag)
{
  // fcn function used by the MINUIT Fitter
  // 2015/03/09, AP Adding hit resolution U and V to chi2
       
  DR3 CurPoint;
  DR3 CurHit;
  DR3 Correction;

  //Double_t resolution[3] = {3.5,3.5,0.0} ; // =(sigmaU, sigmaV, sigmaW)
 
  if(AlignDebug >= 3) 
  { 
    printf("Rotations : %11f %11f %11f\n",par[0],par[1],par[2]);
    printf("Translat. : %11f %11f %11f \n",par[3],par[4],par[5]);
    cout << "+++++++++++++++++++++++++++++++" << endl;
  }  
    
  std::cout<<" Th0 = "<<par[0]*180/3.141592653<<" Th1 = "<<par[1]*180/3.141592653<<" Th3 = "<<par[2]*180/3.141592653<<std::endl;
  std::cout<<"   X = "<<par[3]<<"   Y = "<<par[4]<<"   Z = "<<par[5]<<std::endl;

  fAlignement->SetTranslation(par[3],par[4],par[5]);
  fAlignement->SetRotations(par[0],par[1],par[2]); 
 
  ladderToAlign->UpdateAlignment(); // Update the DPrecAlign of the planes :)
  
  //Compute the chi-square
  f=0.0 ;

  if (iflag==8) {
    // fDu->Reset();
    // fDv->Reset();
    fDx->Reset(); //qyliu: ladder
    fDy->Reset();
    fDz->Reset();
  }

  Int_t ipt=0;

  Int_t startIndex = 0;
  Int_t numberOfPlanes = 0;

  if(ladderToAlign->GetLadderType()=="Custom" || ladderToAlign->GetLadderType()=="custom") {
    startIndex = ladderToAlign->GetFirstPlane();
    numberOfPlanes = ladderToAlign->GetNumberOfPlanes();
  }
  else {
    std::cout<<"No custom ladder found."<<std::endl;
    return;
  }

  for(Int_t i=startIndex ; i<startIndex+numberOfPlanes ; ++i) {
    
     TList* data = NULL;
     DPrecAlign* fAlignPlane = NULL;

     data = ladderToAlign->GetPlane(i)->GetPrecAlignment()->GetDataPoints();
     fAlignPlane = ladderToAlign->GetPlane(i)->GetPrecAlignment();
    
     DataPoints *current ;
     current = (DataPoints*) data->First();

     while(current) {
       CurHit   = current->GetHitPosition(); // get hit position measured in local system
       DR3 CurHitTracker =  fAlignPlane->TransformHitToTracker(CurHit);
       
       if(AlignDebug >= 3) { cout << "fcnLadder: got hit position, x=" << CurHit(0) << ", y=" << CurHit(1) << ", z=" << CurHit(2) << endl;}
       
       DR3 dirTrack      = current->GetTrackDirection();
       DR3 hitresolution = current->GetHitResolution();
     
       DR3 dirTrackNormalized(dirTrack(0), dirTrack(1), dirTrack(2));
       dirTrackNormalized(0) /= sqrt( dirTrack(0)*dirTrack(0) + dirTrack(1)*dirTrack(1) + dirTrack(2)*dirTrack(2) );
       dirTrackNormalized(1) /= sqrt( dirTrack(0)*dirTrack(0) + dirTrack(1)*dirTrack(1) + dirTrack(2)*dirTrack(2) );
       dirTrackNormalized(2) /= sqrt( dirTrack(0)*dirTrack(0) + dirTrack(1)*dirTrack(1) + dirTrack(2)*dirTrack(2) );

       Double_t roll  = atan2(dirTrackNormalized(1), dirTrackNormalized(2)); 
       Double_t pitch = atan2(dirTrackNormalized(0), dirTrackNormalized(2));
    
       Double_t diffAngleX = par[2]*180./TMath::Pi() - roll*180./TMath::Pi();
       Double_t diffAngleY = par[1]*180./TMath::Pi() - pitch*180./TMath::Pi(); 
    
       diffAngleX = fabs(diffAngleX);
       diffAngleY = fabs(diffAngleY);
      
       // resolution in function of tilts : (G4 Simu numbers)
       /* 
       if(diffAngleX<=2.5) resolution[1] = 3.5;
       else if(diffAngleX>2.5 && diffAngleX<=5) resolution[1] = 3.7;
       else if(diffAngleX>5  && diffAngleX<=25) resolution[1] = 3.9;
       else if(diffAngleX>25 && diffAngleX<=30) resolution[1] = 4.0;
       else if(diffAngleX>30 && diffAngleX<=40) resolution[1] = 4.25;
       else if(diffAngleX>40 && diffAngleX<=50) resolution[1] = 4.4;
       else if(diffAngleX>50 && diffAngleX<=60) resolution[1] = 4.75;
       else if(diffAngleX>60) resolution[1] = 5.0; 

       if(diffAngleY<=2.5) resolution[0] = 3.5;
       else if(diffAngleY>2.5 && diffAngleY<=5) resolution[0] = 3.7;
       else if(diffAngleY>5  && diffAngleY<=25) resolution[0] = 3.9;
       else if(diffAngleY>25 && diffAngleY<=30) resolution[0] = 4.0;
       else if(diffAngleY>30 && diffAngleY<=40) resolution[0] = 4.25;
       else if(diffAngleY>40 && diffAngleY<=50) resolution[0] = 4.4;
       else if(diffAngleY>50 && diffAngleY<=60) resolution[0] = 4.75;
       else if(diffAngleY>60) resolution[0] = 5.0;
       */

       fAlignPlane->CalculateIntersection(current);  // get hit position measured by RS in xyz
       DR3 CurPointTracker = fAlignPlane->GetTrackPosition();
       CurPoint = fAlignPlane->TransformTrackToPlane(); // transform xyz -> uv
       //DR3 CurPointTracker = fAlignPlane->TransformHitToTracker(CurPoint);
       /*
       cout<<"Print : "<<endl;
       CurHit.Print();
       CurPoint.Print();
       (CurPoint-CurHit).Print();
       */

       // Computation of the errors in (x,y,z) in function of (u,v,w=0)
       // X = (x,y,z) | U = (u,v,0) | T = translations = (Tx,Ty,Tz)
       // X = Rinv U + T
       // Values of sigmaX, sigmaY, sigmaZ in function of sigmaU and sigma V

       // OLD DPrecAlign (Rinv)Transposee = fAlignPlane->GetTorationMatrix();
       // NEW DprecALign : R is not the transposed matrix :)

       //Double_t sigmaU = resolution[0];
       //Double_t sigmaV = resolution[1];
       Double_t sigmaU = hitresolution(0);
       Double_t sigmaV = hitresolution(1);
       
       Double_t* RinvTransposee = NULL;
       Double_t* Rinv = NULL;   
      
       Double_t sigmaX_Square = 0;
       Double_t sigmaY_Square = 0;
       Double_t sigmaZ_Square = 0; 
       
       Int_t DPrecAlignMethod = fSession->GetSetup()->GetTrackerPar().DPrecAlignMethod;
        
       if(DPrecAlignMethod==0) {
         
         RinvTransposee = fAlignPlane->GetTorationMatrix();

         // OLD DPrecAlign
         //sigmaX_Square = RinvTransposee[0]*RinvTransposee[0]*sigmaU*sigmaU + RinvTransposee[3]*RinvTransposee[3]*sigmaV*sigmaV;
         //sigmaY_Square = RinvTransposee[1]*RinvTransposee[1]*sigmaU*sigmaU + RinvTransposee[4]*RinvTransposee[4]*sigmaV*sigmaV;
         //sigmaZ_Square = RinvTransposee[2]*RinvTransposee[2]*sigmaU*sigmaU + RinvTransposee[5]*RinvTransposee[5]*sigmaV*sigmaV;
         sigmaX_Square = pow(RinvTransposee[0]*sigmaU + RinvTransposee[3]*sigmaV,2);
         sigmaY_Square = pow(RinvTransposee[1]*sigmaU + RinvTransposee[4]*sigmaV,2);
         sigmaZ_Square = pow(RinvTransposee[2]*sigmaU + RinvTransposee[5]*sigmaV,2);
       }
       else if(DPrecAlignMethod==1){

         Rinv = fAlignPlane->GetTorationMatrix();

         // NEW DPrecAlign
         //sigmaX_Square = Rinv[0]*Rinv[0]*sigmaU*sigmaU + Rinv[1]*Rinv[1]*sigmaV*sigmaV;
         //sigmaY_Square = Rinv[3]*Rinv[3]*sigmaU*sigmaU + Rinv[4]*Rinv[4]*sigmaV*sigmaV;
         //sigmaZ_Square = Rinv[6]*Rinv[6]*sigmaU*sigmaU + Rinv[7]*Rinv[7]*sigmaV*sigmaV;
         sigmaX_Square = pow(Rinv[0]*sigmaU + Rinv[1]*sigmaV,2);
         sigmaY_Square = pow(Rinv[3]*sigmaU + Rinv[4]*sigmaV,2);
         sigmaZ_Square = pow(Rinv[6]*sigmaU + Rinv[7]*sigmaV,2);
       }
       else std::cout<<"BEWARE DPrecAlignMethod = "<<DPrecAlignMethod<<"don't exist !"<<std::endl;

       Double_t resolutionXYZ[3]= {sigmaX_Square, sigmaY_Square, sigmaZ_Square};

       for (Int_t j=0 ; j<3 ; j++){
         Double_t tmp = (CurPointTracker(j)-CurHitTracker(j))*(CurPointTracker(j)-CurHitTracker(j))/resolutionXYZ[j] ;  // In tracker frame.
         f+= tmp;
       }
       if (iflag==8) {
         DR3 temp = CurPoint-CurHit;
         DR3 tempTracker = CurPointTracker-CurHitTracker;
       
         if(AlignDebug >= 3) 
         {
           printf("Point : ") ; CurPoint.Print() ;
           printf("Hit   : ") ; CurHit.Print() ;
           printf("diff  : ") ; temp.Print() ;
           printf("\n") ;
         }
        
        // fGraph[0].SetPoint(ipt,CurHitTracker(0),tempTracker(0));
        // fGraph[1].SetPoint(ipt,CurHitTracker(0),tempTracker(1));
        // fGraph[2].SetPoint(ipt,CurHitTracker(1),tempTracker(0));
        // fGraph[3].SetPoint(ipt,CurHitTracker(1),tempTracker(1));

	fGraph[0].SetPoint(ipt,CurHitTracker(0),tempTracker(0)); // qyliu: add plots as ladder alignment is in x, y, z
        fGraph[1].SetPoint(ipt,CurHitTracker(0),tempTracker(1));
        fGraph[2].SetPoint(ipt,CurHitTracker(0),tempTracker(2));
        fGraph[3].SetPoint(ipt,CurHitTracker(1),tempTracker(0));
        fGraph[4].SetPoint(ipt,CurHitTracker(1),tempTracker(1));
        fGraph[5].SetPoint(ipt,CurHitTracker(1),tempTracker(2));
        fGraph[6].SetPoint(ipt,CurHitTracker(2),tempTracker(0));
        fGraph[7].SetPoint(ipt,CurHitTracker(2),tempTracker(1));
        fGraph[8].SetPoint(ipt,CurHitTracker(2),tempTracker(2));

	// fDu->Fill(tempTracker(0));
        // fDv->Fill(tempTracker(1));
	fDx->Fill(tempTracker(0));  // qyliu: ladders use global coordinates
	fDy->Fill(tempTracker(1));
	fDz->Fill(tempTracker(2));
       }
 
       current=(DataPoints*) data->After(current) ;
       ipt++;
     }
   }
 
  if (iflag==10) {
    fGraph[0].Print();
  }

  if (iflag==8) 
  {
   // TPostScript ps("residus.ps"); //,-113
   // ps.NewPage();
//    fCnv = new TCanvas("Cnv","Residues after alignment",2,10,400,500);
//    fCnv->Divide(1,2);
    fCnv->cd(1);
    // fDu->Draw();
    // fDu->Fit("gaus","q");
    fDx->Draw();
    fDx->Fit("gaus","q");
    //fCnv->Update();
           
    fCnv->cd(2);
    //gStyle->SetOptFit(1);
    /*
    fMyfit->SetParameter(0,250);
    fMyfit->SetParameter(1,50); // was 10
    fMyfit->SetParameter(2,4);
    fMyfit->SetParameter(3,500);
    fMyfit->SetParameter(4,-20);
    fMyfit->SetParameter(5,5);
    */
    // fDv->Draw();
    // fDv->Fit("gaus","q");
    fDy->Draw();
    fDy->Fit("gaus","q");
    fCnv->cd(3);
    fDz->Draw();
    fDz->Fit("gaus","q");
    fCnv->Modified();
    fCnv->Update();
    fCnv->Draw();
    // fCnv->Paint();
    // ps.NewPage();

    for (Int_t i=0; i<9 ; i++) {   // qyliu: changed from i < 4
      fCcr->cd(i+1);
      fGraph[i].SetMarkerStyle(24);
      fGraph[i].SetMarkerColor(2);
      fGraph[i].SetMarkerSize(0.2);
      fGraph[i].Draw("AP");
    } 
    
    // fGraph[0].GetHistogram()->SetXTitle("U hit");
    // fGraph[0].GetHistogram()->SetYTitle("DU");
    // //  fGraph[0].GetHistogram()->SetMinimum(-2000.);
    // //  fGraph[0].GetHistogram()->SetMaximum(2000.);
    // fGraph[1].GetHistogram()->SetXTitle("U hit");
    // fGraph[1].GetHistogram()->SetYTitle("DV");
    // fGraph[1].GetHistogram()->SetMinimum(-2000.);
    // fGraph[1].GetHistogram()->SetMaximum(2000.);
    // fGraph[2].GetHistogram()->SetXTitle("V hit");
    // fGraph[2].GetHistogram()->SetYTitle("DU");
    // // fGraph[2].GetHistogram()->SetMinimum(-2000.);
    // // fGraph[2].GetHistogram()->SetMaximum(2000.);
    // fGraph[3].GetHistogram()->SetXTitle("V hit");
    // fGraph[3].GetHistogram()->SetYTitle("DV");
    // // fGraph[3].GetHistogram()->SetMinimum(-2000.);
    // //fGraph[3].GetHistogram()->SetMaximum(2000.);
    fGraph[0].GetHistogram()->SetXTitle("X hit"); // qyliu: ladder use global coordinate
    fGraph[0].GetHistogram()->SetYTitle("DX");
    fGraph[1].GetHistogram()->SetXTitle("X hit");
    fGraph[1].GetHistogram()->SetYTitle("DY");
    fGraph[2].GetHistogram()->SetXTitle("X hit");
    fGraph[2].GetHistogram()->SetYTitle("DZ");
    fGraph[3].GetHistogram()->SetXTitle("Y hit"); // qyliu: ladder use global coordinate
    fGraph[3].GetHistogram()->SetYTitle("DX");
    fGraph[4].GetHistogram()->SetXTitle("Y hit");
    fGraph[4].GetHistogram()->SetYTitle("DY");
    fGraph[5].GetHistogram()->SetXTitle("Y hit");
    fGraph[5].GetHistogram()->SetYTitle("DZ");
    fGraph[6].GetHistogram()->SetXTitle("Z hit"); // qyliu: ladder use global coordinate
    fGraph[6].GetHistogram()->SetYTitle("DX");
    fGraph[7].GetHistogram()->SetXTitle("Z hit");
    fGraph[7].GetHistogram()->SetYTitle("DY");
    fGraph[8].GetHistogram()->SetXTitle("Z hit");
    fGraph[8].GetHistogram()->SetYTitle("DZ");

    fCcr->Update();
    fCcr->Draw();
    // fCcr->Paint();

    //gROOT->GetListOfCanvases()->Write();
    //ps.Close();    

  }

  Double_t mypar[3];
  for(Int_t i=0; i<3; i++){
    mypar[i] = par[i]*180.00/3.141592653;
    mypar[i] = fmod(mypar[i],360.);
  }
       
  if(AlignDebug >=3) { printf(" tilt Z= %f \n tilt Y= %f \n tilt Z= %f \n Shift X= %f \n Shift Y= %f \n Z= %f \n N of entries %d Chi2 %f Chi2/DoF %f\n",mypar[0],mypar[1],mypar[2],par[3],par[4],par[5], ipt, f ,f/ipt);}

}

//______________________________________________________________________________
//  
void  MimosaAlignAnalysis::fcnLadder2(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag)
{
  // fcn function used by the MINUIT Fitter
  // 2015/03/09, AP Adding hit resolution U and V to chi2
       
  DR3 CurPoint;
  DR3 CurHit;
  DR3 Correction;

  //Double_t resolution[3] = {3.5,3.5,0.0} ; // =(sigmaU, sigmaV, sigmaW)
 
  if(AlignDebug >= 3) 
  { 
    printf("Rotations : %11f %11f %11f\n",par[0],par[1],par[2]);
    printf("Translat. : %11f %11f %11f \n",par[3],par[4],par[5]);
    cout << "+++++++++++++++++++++++++++++++" << endl;
  }  
    
  std::cout<<" Th0 = "<<par[0]*180/3.141592653<<" Th1 = "<<par[1]*180/3.141592653<<" Th3 = "<<par[2]*180/3.141592653<<std::endl;
  std::cout<<"   X = "<<par[3]<<"   Y = "<<par[4]<<"   Z = "<<par[5]<<std::endl;
  
  std::cout<<" Th0Pl1 = "<<par[6]*180/3.141592653<<" Th1Pl1 = "<<par[7]*180/3.141592653<<" Th3Pl1 = "<<par[8]*180/3.141592653<<std::endl;
  std::cout<<"   U1 = "<<par[9]<<"   V1 = "<<par[10]<<"   W1 = "<<par[11]<<std::endl;

  std::cout<<" Th0Pl2 = "<<par[12]*180/3.141592653<<" Th1Pl2 = "<<par[13]*180/3.141592653<<" Th3Pl2 = "<<par[14]*180/3.141592653<<std::endl;
  std::cout<<"   U2 = "<<par[15]<<"   V2 = "<<par[16]<<"   W2 = "<<par[17]<<std::endl;

  std::cout<<" Th0Pl3 = "<<par[18]*180/3.141592653<<" Th1Pl3 = "<<par[19]*180/3.141592653<<" Th3Pl3 = "<<par[20]*180/3.141592653<<std::endl;
  std::cout<<"   U3 = "<<par[21]<<"   V3 = "<<par[22]<<"   W3 = "<<par[23]<<std::endl;

  std::cout<<" Th0Pl4 = "<<par[24]*180/3.141592653<<" Th1Pl4 = "<<par[25]*180/3.141592653<<" Th3Pl4 = "<<par[26]*180/3.141592653<<std::endl;
  std::cout<<"   U4 = "<<par[27]<<"   V4 = "<<par[28]<<"   W4 = "<<par[29]<<std::endl;

  fAlignement->SetTranslation(par[3],par[4],par[5]);
  fAlignement->SetRotations(par[0],par[1],par[2]); 
 
  ladderToAlign->UpdateAlignment(); // Update the DPrecAlign of the planes :)
  // Plane local parameters updated to their initial values.

  //Compute the chi-square
  f=0.0 ;

  if (iflag==8) {
    // fDu->Reset();
    // fDv->Reset();
    fDx->Reset(); //qyliu: ladder
    fDy->Reset();
    fDz->Reset();
  }

  Int_t ipt=0;

  Int_t startIndex = 0;
  Int_t numberOfPlanes = 0;

  if(ladderToAlign->GetLadderType()=="Custom" || ladderToAlign->GetLadderType()=="custom") {
    startIndex = ladderToAlign->GetFirstPlane();
    numberOfPlanes = ladderToAlign->GetNumberOfPlanes();
  }
  else {
    std::cout<<"No custom ladder found."<<std::endl;
    return;
  }

  for(Int_t i=startIndex ; i<startIndex+numberOfPlanes ; ++i) {
    
     TList* data = NULL;
     DPrecAlign* fAlignPlane = NULL;

     data = ladderToAlign->GetPlane(i)->GetPrecAlignment()->GetDataPoints();
     fAlignPlane = ladderToAlign->GetPlane(i)->GetPrecAlignment();
  
     if(i==startIndex) {
       fAlignPlane->SetTranslation(par[9],par[10],par[11]);
       fAlignPlane->SetRotations(par[6],par[7],par[8]); 
     } else if(i==startIndex+1){
       fAlignPlane->SetTranslation(par[15],par[16],par[17]);
       fAlignPlane->SetRotations(par[12],par[13],par[14]); 
     } else if(i==startIndex+2){
       fAlignPlane->SetTranslation(par[21],par[22],par[23]);
       fAlignPlane->SetRotations(par[18],par[19],par[20]); 
     } else if(i==startIndex+3){
       fAlignPlane->SetTranslation(par[27],par[28],par[29]);
       fAlignPlane->SetRotations(par[24],par[25],par[26]); 
     }

     DataPoints *current ;
     current = (DataPoints*) data->First();

     while(current) {
       CurHit   = current->GetHitPosition(); // get hit position measured in local system
       DR3 CurHitTracker =  fAlignPlane->TransformHitToTracker(CurHit);
       
       if(AlignDebug >= 3) { cout << "fcnLadder2: got hit position, x=" << CurHit(0) << ", y=" << CurHit(1) << ", z=" << CurHit(2) << endl;}
       
       DR3 dirTrack      = current->GetTrackDirection();
       DR3 hitresolution = current->GetHitResolution();
     
       DR3 dirTrackNormalized(dirTrack(0), dirTrack(1), dirTrack(2));
       dirTrackNormalized(0) /= sqrt( dirTrack(0)*dirTrack(0) + dirTrack(1)*dirTrack(1) + dirTrack(2)*dirTrack(2) );
       dirTrackNormalized(1) /= sqrt( dirTrack(0)*dirTrack(0) + dirTrack(1)*dirTrack(1) + dirTrack(2)*dirTrack(2) );
       dirTrackNormalized(2) /= sqrt( dirTrack(0)*dirTrack(0) + dirTrack(1)*dirTrack(1) + dirTrack(2)*dirTrack(2) );

       Double_t roll  = atan2(dirTrackNormalized(1), dirTrackNormalized(2)); 
       Double_t pitch = atan2(dirTrackNormalized(0), dirTrackNormalized(2));
    
       Double_t diffAngleX = par[2]*180./TMath::Pi() - roll*180./TMath::Pi();
       Double_t diffAngleY = par[1]*180./TMath::Pi() - pitch*180./TMath::Pi(); 
    
       diffAngleX = fabs(diffAngleX);
       diffAngleY = fabs(diffAngleY);
      
       // resolution in function of tilts : (G4 Simu numbers)
       /* 
       if(diffAngleX<=2.5) resolution[1] = 3.5;
       else if(diffAngleX>2.5 && diffAngleX<=5) resolution[1] = 3.7;
       else if(diffAngleX>5  && diffAngleX<=25) resolution[1] = 3.9;
       else if(diffAngleX>25 && diffAngleX<=30) resolution[1] = 4.0;
       else if(diffAngleX>30 && diffAngleX<=40) resolution[1] = 4.25;
       else if(diffAngleX>40 && diffAngleX<=50) resolution[1] = 4.4;
       else if(diffAngleX>50 && diffAngleX<=60) resolution[1] = 4.75;
       else if(diffAngleX>60) resolution[1] = 5.0; 

       if(diffAngleY<=2.5) resolution[0] = 3.5;
       else if(diffAngleY>2.5 && diffAngleY<=5) resolution[0] = 3.7;
       else if(diffAngleY>5  && diffAngleY<=25) resolution[0] = 3.9;
       else if(diffAngleY>25 && diffAngleY<=30) resolution[0] = 4.0;
       else if(diffAngleY>30 && diffAngleY<=40) resolution[0] = 4.25;
       else if(diffAngleY>40 && diffAngleY<=50) resolution[0] = 4.4;
       else if(diffAngleY>50 && diffAngleY<=60) resolution[0] = 4.75;
       else if(diffAngleY>60) resolution[0] = 5.0;
       */

       fAlignPlane->CalculateIntersection(current);  // get hit position measured by RS in xyz
       DR3 CurPointTracker = fAlignPlane->GetTrackPosition();
       CurPoint = fAlignPlane->TransformTrackToPlane(); // transform xyz -> uv
       //DR3 CurPointTracker = fAlignPlane->TransformHitToTracker(CurPoint);
       /*
       cout<<"Print : "<<endl;
       CurHit.Print();
       CurPoint.Print();
       (CurPoint-CurHit).Print();
       */

       // Computation of the errors in (x,y,z) in function of (u,v,w=0)
       // X = (x,y,z) | U = (u,v,0) | T = translations = (Tx,Ty,Tz)
       // X = Rinv U + T
       // Values of sigmaX, sigmaY, sigmaZ in function of sigmaU and sigma V

       // OLD DPrecAlign (Rinv)Transposee = fAlignPlane->GetTorationMatrix();
       // NEW DprecALign : R is not the transposed matrix :)

       //Double_t sigmaU = resolution[0];
       //Double_t sigmaV = resolution[1];
       Double_t sigmaU = hitresolution(0);
       Double_t sigmaV = hitresolution(1);
       
       Double_t* RinvTransposee = NULL;
       Double_t* Rinv = NULL;   
      
       Double_t sigmaX_Square = 0;
       Double_t sigmaY_Square = 0;
       Double_t sigmaZ_Square = 0; 
       
       Int_t DPrecAlignMethod = fSession->GetSetup()->GetTrackerPar().DPrecAlignMethod;
        
       if(DPrecAlignMethod==0) {
         
         RinvTransposee = fAlignPlane->GetTorationMatrix();

         // OLD DPrecAlign
         //sigmaX_Square = RinvTransposee[0]*RinvTransposee[0]*sigmaU*sigmaU + RinvTransposee[3]*RinvTransposee[3]*sigmaV*sigmaV;
         //sigmaY_Square = RinvTransposee[1]*RinvTransposee[1]*sigmaU*sigmaU + RinvTransposee[4]*RinvTransposee[4]*sigmaV*sigmaV;
         //sigmaZ_Square = RinvTransposee[2]*RinvTransposee[2]*sigmaU*sigmaU + RinvTransposee[5]*RinvTransposee[5]*sigmaV*sigmaV;
         sigmaX_Square = pow(RinvTransposee[0]*sigmaU + RinvTransposee[3]*sigmaV,2);
         sigmaY_Square = pow(RinvTransposee[1]*sigmaU + RinvTransposee[4]*sigmaV,2);
         sigmaZ_Square = pow(RinvTransposee[2]*sigmaU + RinvTransposee[5]*sigmaV,2);
       }
       else if(DPrecAlignMethod==1){

         Rinv = fAlignPlane->GetTorationMatrix();

         // NEW DPrecAlign
         //sigmaX_Square = Rinv[0]*Rinv[0]*sigmaU*sigmaU + Rinv[1]*Rinv[1]*sigmaV*sigmaV;
         //sigmaY_Square = Rinv[3]*Rinv[3]*sigmaU*sigmaU + Rinv[4]*Rinv[4]*sigmaV*sigmaV;
         //sigmaZ_Square = Rinv[6]*Rinv[6]*sigmaU*sigmaU + Rinv[7]*Rinv[7]*sigmaV*sigmaV;
         sigmaX_Square = pow(Rinv[0]*sigmaU + Rinv[1]*sigmaV,2);
         sigmaY_Square = pow(Rinv[3]*sigmaU + Rinv[4]*sigmaV,2);
         sigmaZ_Square = pow(Rinv[6]*sigmaU + Rinv[7]*sigmaV,2);
       }
       else std::cout<<"BEWARE DPrecAlignMethod = "<<DPrecAlignMethod<<"don't exist !"<<std::endl;

       Double_t resolutionXYZ[3] = {sigmaX_Square, sigmaY_Square, sigmaZ_Square};
       Double_t resolutionUV[2] = {sigmaU*sigmaU, sigmaV*sigmaV};

       for (Int_t j=0 ; j<3 ; j++){
         
         Double_t chi2_ladder = (CurPointTracker(j)-CurHitTracker(j))*(CurPointTracker(j)-CurHitTracker(j))/resolutionXYZ[j] ;  // In tracker frame.
         
         Double_t chi2_local_plane = 0.;
         if(j<2) chi2_local_plane = pow( (CurPoint(j)-CurHit(j)), 2)/resolutionUV[j];  // Necessary or not ? Can be check :)
         
         f += chi2_ladder;
         f += chi2_local_plane;
       }
       if (iflag==8) {
         DR3 temp = CurPoint-CurHit;
         DR3 tempTracker = CurPointTracker-CurHitTracker;
       
         if(AlignDebug >= 3) 
         {
           printf("Point : ") ; CurPoint.Print() ;
           printf("Hit   : ") ; CurHit.Print() ;
           printf("diff  : ") ; temp.Print() ;
           printf("\n") ;
         }
        
        // fGraph[0].SetPoint(ipt,CurHitTracker(0),tempTracker(0));
        // fGraph[1].SetPoint(ipt,CurHitTracker(0),tempTracker(1));
        // fGraph[2].SetPoint(ipt,CurHitTracker(1),tempTracker(0));
        // fGraph[3].SetPoint(ipt,CurHitTracker(1),tempTracker(1));

	fGraph[0].SetPoint(ipt,CurHitTracker(0),tempTracker(0)); // qyliu: add plots as ladder alignment is in x, y, z
        fGraph[1].SetPoint(ipt,CurHitTracker(0),tempTracker(1));
        fGraph[2].SetPoint(ipt,CurHitTracker(0),tempTracker(2));
        fGraph[3].SetPoint(ipt,CurHitTracker(1),tempTracker(0));
        fGraph[4].SetPoint(ipt,CurHitTracker(1),tempTracker(1));
        fGraph[5].SetPoint(ipt,CurHitTracker(1),tempTracker(2));
        fGraph[6].SetPoint(ipt,CurHitTracker(2),tempTracker(0));
        fGraph[7].SetPoint(ipt,CurHitTracker(2),tempTracker(1));
        fGraph[8].SetPoint(ipt,CurHitTracker(2),tempTracker(2));

	// fDu->Fill(tempTracker(0));
        // fDv->Fill(tempTracker(1));
	fDx->Fill(tempTracker(0));  // qyliu: ladders use global coordinates
	fDy->Fill(tempTracker(1));
	fDz->Fill(tempTracker(2));
       }
 
       current=(DataPoints*) data->After(current) ;
       ipt++;
     }
   }
 
  if (iflag==10) {
    fGraph[0].Print();
  }

  if (iflag==8) 
  {
   // TPostScript ps("residus.ps"); //,-113
   // ps.NewPage();
//    fCnv = new TCanvas("Cnv","Residues after alignment",2,10,400,500);
//    fCnv->Divide(1,2);
    fCnv->cd(1);
    // fDu->Draw();
    // fDu->Fit("gaus","q");
    fDx->Draw();
    fDx->Fit("gaus","q");
    //fCnv->Update();
           
    fCnv->cd(2);
    //gStyle->SetOptFit(1);
    /*
    fMyfit->SetParameter(0,250);
    fMyfit->SetParameter(1,50); // was 10
    fMyfit->SetParameter(2,4);
    fMyfit->SetParameter(3,500);
    fMyfit->SetParameter(4,-20);
    fMyfit->SetParameter(5,5);
    */
    // fDv->Draw();
    // fDv->Fit("gaus","q");
    fDy->Draw();
    fDy->Fit("gaus","q");
    fCnv->cd(3);
    fDz->Draw();
    fDz->Fit("gaus","q");
    fCnv->Modified();
    fCnv->Update();
    fCnv->Draw();
    // fCnv->Paint();
    // ps.NewPage();

    for (Int_t i=0; i<9 ; i++) {   // qyliu: changed from i < 4
      fCcr->cd(i+1);
      fGraph[i].SetMarkerStyle(24);
      fGraph[i].SetMarkerColor(2);
      fGraph[i].SetMarkerSize(0.2);
      fGraph[i].Draw("AP");
    } 
    
    // fGraph[0].GetHistogram()->SetXTitle("U hit");
    // fGraph[0].GetHistogram()->SetYTitle("DU");
    // //  fGraph[0].GetHistogram()->SetMinimum(-2000.);
    // //  fGraph[0].GetHistogram()->SetMaximum(2000.);
    // fGraph[1].GetHistogram()->SetXTitle("U hit");
    // fGraph[1].GetHistogram()->SetYTitle("DV");
    // fGraph[1].GetHistogram()->SetMinimum(-2000.);
    // fGraph[1].GetHistogram()->SetMaximum(2000.);
    // fGraph[2].GetHistogram()->SetXTitle("V hit");
    // fGraph[2].GetHistogram()->SetYTitle("DU");
    // // fGraph[2].GetHistogram()->SetMinimum(-2000.);
    // // fGraph[2].GetHistogram()->SetMaximum(2000.);
    // fGraph[3].GetHistogram()->SetXTitle("V hit");
    // fGraph[3].GetHistogram()->SetYTitle("DV");
    // // fGraph[3].GetHistogram()->SetMinimum(-2000.);
    // //fGraph[3].GetHistogram()->SetMaximum(2000.);
    fGraph[0].GetHistogram()->SetXTitle("X hit"); // qyliu: ladder use global coordinate
    fGraph[0].GetHistogram()->SetYTitle("DX");
    fGraph[1].GetHistogram()->SetXTitle("X hit");
    fGraph[1].GetHistogram()->SetYTitle("DY");
    fGraph[2].GetHistogram()->SetXTitle("X hit");
    fGraph[2].GetHistogram()->SetYTitle("DZ");
    fGraph[3].GetHistogram()->SetXTitle("Y hit"); // qyliu: ladder use global coordinate
    fGraph[3].GetHistogram()->SetYTitle("DX");
    fGraph[4].GetHistogram()->SetXTitle("Y hit");
    fGraph[4].GetHistogram()->SetYTitle("DY");
    fGraph[5].GetHistogram()->SetXTitle("Y hit");
    fGraph[5].GetHistogram()->SetYTitle("DZ");
    fGraph[6].GetHistogram()->SetXTitle("Z hit"); // qyliu: ladder use global coordinate
    fGraph[6].GetHistogram()->SetYTitle("DX");
    fGraph[7].GetHistogram()->SetXTitle("Z hit");
    fGraph[7].GetHistogram()->SetYTitle("DY");
    fGraph[8].GetHistogram()->SetXTitle("Z hit");
    fGraph[8].GetHistogram()->SetYTitle("DZ");

    fCcr->Update();
    fCcr->Draw();
    // fCcr->Paint();

    //gROOT->GetListOfCanvases()->Write();
    //ps.Close();    

  }

  Double_t mypar[3];
  for(Int_t i=0; i<3; i++){
    mypar[i] = par[i]*180.00/3.141592653;
    mypar[i] = fmod(mypar[i],360.);
  }
       
  if(AlignDebug >=3) { printf(" tilt Z= %f \n tilt Y= %f \n tilt Z= %f \n Shift X= %f \n Shift Y= %f \n Z= %f \n N of entries %d Chi2 %f Chi2/DoF %f\n",mypar[0],mypar[1],mypar[2],par[3],par[4],par[5], ipt, f ,f/ipt);}

}

//______________________________________________________________________________
//
/*
void MimosaAlignAnalysis::cutData(Int_t nSigmas, Double_t* par)
{

  fAlignement->SetTranslation(par[3],par[4],par[5]);
  fAlignement->SetRotations(par[0],par[1],par[2]);
     
  DR3 rotations(par[2], par[1], par[0]);
      
  //ladderToAlign->UpdateAlignment( rotations );  // for rotations :D
  ladderToAlign->UpdateAlignment(); // for translations :D

  std::cout<<" Th0 = "<<par[0]*180./3.141592653<<" Th1 = "<<par[1]*180./3.141592653<<" Th3 = "<<par[2]*180./3.141592653<<std::endl;
  std::cout<<"   X = "<<par[3]<<"   Y = "<<par[4]<<"   Z = "<<par[5]<<std::endl;

  //Compute the chi-square
  Double_t f=0.0;

  TList *dataMV0 =  fAlignement->GetDataPoints();
  TList *dataMV =  fAlignement->GetDataPoints1();

  TList *miniVectorsLadder0 = fAlignement->GetMiniVectors();
  TList *miniVectorsLadder1 = fAlignement->GetMiniVectors1();

  Int_t sizeMin = TMath::Min( fAlignement->DataSize(), fAlignement->DataSize1() );
      
  fDu->Reset();
  fDv->Reset();
  fDw->Reset();
  fDdu->Reset();
  fDdv->Reset();
  fDdw->Reset();

  DataPoints *currentMV0;
  DataPoints *currentMV;

  DataPoints *currentMV0_bis;
  DataPoints *currentMV_bis;

  currentMV0 = (DataPoints*) dataMV0->First();
  currentMV = (DataPoints*) dataMV->First();

  DTrack *miniVectorLadder0 = dynamic_cast<DTrack*>(miniVectorsLadder0->First());
  DTrack *miniVectorLadder1 = dynamic_cast<DTrack*>(miniVectorsLadder1->First());

  DTrack *miniVectorLadder0_bis;
  DTrack *miniVectorLadder1_bis;

  Double_t sigmaU = 4.0;
  Double_t sigmaV = 3.5;
  Double_t sigmaZ = 10.0;

  Int_t ipt=0 ;

  while( sizeMin>0) {
  
    // Ladder0 :

    DR3 trackOriginLadder0 = currentMV0->GetTrackOrigin(); // In tracker coord.
    DR3 hitLadder0 = currentMV0->GetHitPosition(); // In tracker coord.
    
    hitLadder0(2) = trackOriginLadder0(2);
    trackOriginLadder0(2) = 0.;
     
    DR3 tiltLadder0 = currentMV0->GetTrackDirection();

    DataPoints* myNewMV0 = new DataPoints( 0., 0., hitLadder0(0), hitLadder0(1), hitLadder0(2), tiltLadder0(0),  tiltLadder0(1) );

    fAlignement->CalculateIntersection(myNewMV0);
    DR3 hitLadder0_tracker = fAlignement->GetTrackPosition();
    DR3 hitLadder0_plane = fAlignement->TransformTrackToPlane();

    // Ladder1 :

    DR3 hitLadder1 = currentMV->GetHitPosition();  // In tracker coord.
    DR3 trackOriginLadder1 = currentMV->GetTrackOrigin();  // In tracker coord.

    hitLadder1(2) = trackOriginLadder1(2);
    trackOriginLadder1(2) = 0.;

    DR3 tiltLadder1 = currentMV->GetTrackDirection();

    // The ladder move. We compute the new positions and tilts of minivectors.
    
    Double_t planeResolution = 3.5;
    Int_t maxHit = 2;
    
    DHit** hitsMiniVector1 = miniVectorLadder1->GetHitArray();

    DR3 HitPlanePos0(hitsMiniVector1[0]->GetPositionUhitCG(), hitsMiniVector1[0]->GetPositionVhitCG(), hitsMiniVector1[0]->GetPositionWhitCG()); 
    DR3 HitPlanePos1(hitsMiniVector1[1]->GetPositionUhitCG(), hitsMiniVector1[1]->GetPositionVhitCG(), hitsMiniVector1[1]->GetPositionWhitCG());
    
    miniVectorLadder1->Analyze(0, hitsMiniVector1, maxHit, planeResolution);
 
    DR3 newSlopes = miniVectorLadder1->GetLinearFit().GetSlopeZ();
    
    DPrecAlign* planeAlignement0 = hitsMiniVector1[0]->GetPlane()->GetPrecAlignment();
    DPrecAlign* planeAlignement1 = hitsMiniVector1[1]->GetPlane()->GetPrecAlignment();
    
    DR3 newPositionHitLeft  =  planeAlignement0->TransformHitToTracker( HitPlanePos0 );
    DR3 newPositionHitRight =  planeAlignement1->TransformHitToTracker( HitPlanePos1 );

    DR3 afterRotation1 = DR3( (newPositionHitLeft(0)+newPositionHitRight(0))/2., (newPositionHitLeft(1)+newPositionHitRight(1))/2., (newPositionHitLeft(2)+newPositionHitRight(2))/2. );
     
    DataPoints* afterMoveLadder = new DataPoints( 0., 0., afterRotation1(0), afterRotation1(1), afterRotation1(2), newSlopes(0),  newSlopes(1) );
  
    // Compute the intersection with Ladder0

    fAlignement->CalculateIntersection(afterMoveLadder);
    DR3 pointLadderToAlign = fAlignement->GetTrackPosition();
    DR3 ladderToAlignUV = fAlignement->TransformTrackToPlane();
    
    DR3 tiltLadderToAlign = newSlopes;
  
    delete afterMoveLadder;
  
    hitLadder0 = hitLadder0_plane;

    DPlane* myPlane = ladderToAlign->GetPlane(1);
    DPrecAlign* myPrecAlignPlane = myPlane->GetPrecAlignment();
    myPrecAlignPlane->CalculateIntersection( afterMoveLadder ); 
    DR3 trackerPos = myPrecAlignPlane->GetTrackPosition();
    DR3 planePos = myPrecAlignPlane->TransformHitToPlane(trackerPos);

    DPlane* myPlane2 = ladderToAlign->GetPlane( ladderToAlign->GetNumberOfPlanes()/2+1 );
    DPrecAlign* myPrecAlignPlane2 = myPlane2->GetPrecAlignment();
    myPrecAlignPlane2->CalculateIntersection( afterMoveLadder ); 
    DR3 trackerPos2 = myPrecAlignPlane2->GetTrackPosition();
    DR3 planePos2 = myPrecAlignPlane2->TransformHitToPlane(trackerPos2);

    Double_t deltaZ = fabs( trackerPos2(2)-trackerPos(2) );
    Double_t deltaY = fabs( trackerPos2(1)-trackerPos(1) );
    Double_t deltaX = fabs( trackerPos2(0)-trackerPos(0) ); // Rotation 180 deg en X ;)

    Double_t A = (hitLadder0(0)-ladderToAlignUV(0))*(hitLadder0(0)-ladderToAlignUV(0))/(sigmaU*sigmaU/2.);
    Double_t B = (hitLadder0(1)-ladderToAlignUV(1))*(hitLadder0(1)-ladderToAlignUV(1))/(sigmaV*sigmaV/2.); // erreur = \sigma_y^2 / 2
   
    Double_t denomC = 2*sigmaU*sigmaU/(deltaZ*deltaZ) + 2*sigmaZ*sigmaZ*deltaX*deltaX/(deltaZ*deltaZ*deltaZ*deltaZ);

    Double_t C = (tiltLadder0(0)-tiltLadderToAlign(0))*(tiltLadder0(0)-tiltLadderToAlign(0))/(denomC);
   
    Double_t denomD = 2*sigmaV*sigmaV/(deltaZ*deltaZ) + 2*sigmaZ*sigmaZ*deltaY*deltaY/(deltaZ*deltaZ*deltaZ*deltaZ);

    Double_t D = (tiltLadder0(1)-tiltLadderToAlign(1))*(tiltLadder0(1)-tiltLadderToAlign(1))/(denomD) ;

    f +=  (A + B + (C + D)); // Calcul du Chi2. 
    
    DR3 temp = ladderToAlignUV - hitLadder0;
    DR3 tiltTemp = tiltLadderToAlign - tiltLadder0;

    fGraph[0].SetPoint(ipt,ladderToAlignUV(0),temp(0));
    fGraph[1].SetPoint(ipt,ladderToAlignUV(0),temp(1));
    fGraph[2].SetPoint(ipt,ladderToAlignUV(1),temp(0));
    fGraph[3].SetPoint(ipt,ladderToAlignUV(1),temp(1));
    fGraph[4].SetPoint(ipt,tiltLadderToAlign(0), tiltTemp(0));
    fGraph[5].SetPoint(ipt,tiltLadderToAlign(1), tiltTemp(1));
    fDu->Fill(temp(0));
    fDv->Fill(temp(1));
    fDw->Fill(temp(2));
    fDdu->Fill(tiltTemp(0));
    fDdv->Fill(tiltTemp(1));
    fDdw->Fill(f);
    
    currentMV0 = (DataPoints*) dataMV0->After(currentMV0);
    currentMV  = (DataPoints*) dataMV->After(currentMV);

    miniVectorLadder0 = (DTrack*) miniVectorsLadder0->After(miniVectorLadder0);
    miniVectorLadder1 = (DTrack*) miniVectorsLadder1->After(miniVectorLadder1);

    sizeMin--;
    ipt++;
  } // end while 
  
   fCnv->cd(1);
   fDu->Fit("gaus","q");
   fCnv->cd(2);
   fDv->Fit("gaus","q");
   fCnv->Update();
   fCnv->Draw();
      
   fCnv->cd(3);                                                                                                                                                            
   fDw->Fit("gaus","q");
   fCnv->Update();
   fCnv->Draw();

   fCnv->cd(4);
   fDdu->Fit("gaus","q");
   fCnv->Update();
   fCnv->Draw();
      
   fCnv->cd(5);
   fDdv->Fit("gaus","q");
   fCnv->Update();
   fCnv->Draw();

   fCnv->cd(6);
   fDdw->Fit("gaus","q");
   fCnv->Update();
   fCnv->Draw();
      
   for (Int_t i=0; i<6 ; i++) {
     fCcr->cd(i+1);
     fGraph[i].SetMarkerStyle(24);
     fGraph[i].SetMarkerColor(2);
     fGraph[i].SetMarkerSize(0.2);
     fGraph[i].Draw("AP");
   } 

   fGraph[0].GetHistogram()->SetXTitle("U hit");
   fGraph[0].GetHistogram()->SetYTitle("DU");
   
   fGraph[1].GetHistogram()->SetXTitle("U hit");
   fGraph[1].GetHistogram()->SetYTitle("DV");
      
   fGraph[2].GetHistogram()->SetXTitle("V hit");
   fGraph[2].GetHistogram()->SetYTitle("DU");
      
   fGraph[3].GetHistogram()->SetXTitle("V hit");
   fGraph[3].GetHistogram()->SetYTitle("DV");
      
   fGraph[4].GetHistogram()->SetXTitle("DU");
   fGraph[4].GetHistogram()->SetYTitle("Delta DU");
      
   fGraph[5].GetHistogram()->SetXTitle("DV");
   fGraph[5].GetHistogram()->SetYTitle("Delta DV");
      
   fCcr->Update();
   fCcr->Draw();

   TF1* fitU = fDu->GetFunction("gaus");
   Double_t meanU = fitU->GetParameter(1);
   Double_t sigmaU = fitU->GetParameter(2);

   std::cout<<"Mean Fit in U direction  = "<<meanU<<std::endl;
   std::cout<<"Width Fit in U direction = "<<sigmaU<<std::endl;

   TF1* fitV = fDv->GetFunction("gaus");
   Double_t meanV = fitV->GetParameter(1);
   Double_t sigmaV = fitV->GetParameter(2);
                                                               
   std::cout<<"Mean Fit in V direction  = "<<meanV<<std::endl;
   std::cout<<"Width Fit in V direction = "<<sigmaV<<std::endl;

   TF1* fitDu = fDdu->GetFunction("gaus");
   Double_t meanDu = fitDu->GetParameter(1);
   Double_t sigmaDu = fitDu->GetParameter(2);
                                                               
   std::cout<<"Mean fit slope dx/dz  = "<<meanDu<<std::endl;
   std::cout<<"Width fit slope dx/dz = "<<sigmaDu<<std::endl;

   TF1* fitDv = fDdv->GetFunction("gaus");
   Double_t meanDv = fitDv->GetParameter(1);
   Double_t sigmaDv = fitDv->GetParameter(2);
                                                               
   std::cout<<"Mean fit slope dy/dz  = "<<meanDv<<std::endl;
   std::cout<<"Width fit slope dy/dz = "<<sigmaDv<<std::endl;
 
 // Cutting at N sigmas ...

  sizeMin = TMath::Min( fAlignement->DataSize(), fAlignement->DataSize1() );
  
  f=0.0;    
  
  //if (iflag==8) {
    fDu->Reset();
    fDv->Reset();
    fDw->Reset();
    fDdu->Reset();
    fDdv->Reset();
    fDdw->Reset();
  //}

  currentMV0 = (DataPoints*) dataMV0->First();
  currentMV = (DataPoints*) dataMV->First();
   
  miniVectorLadder0 = dynamic_cast<DTrack*>(miniVectorsLadder0->First()); 
  miniVectorLadder1 = dynamic_cast<DTrack*>(miniVectorsLadder1->First());

  ipt=0;

  while(sizeMin>0) {
   
    // Ladder0 :

    DR3 trackOriginLadder0 = currentMV0->GetTrackOrigin(); // In tracker coord.
    DR3 hitLadder0 = currentMV0->GetHitPosition(); // In tracker coord.
    
    hitLadder0(2) = trackOriginLadder0(2);
    trackOriginLadder0(2) = 0.;
    
    DR3 tiltLadder0 = currentMV0->GetTrackDirection();

    DataPoints* myNewMV0 = new DataPoints( 0., 0., hitLadder0(0), hitLadder0(1), hitLadder0(2), tiltLadder0(0),  tiltLadder0(1) );

    // Ladder1 :

    DR3 hitLadder1 = currentMV->GetHitPosition();  // In tracker coord.
    DR3 trackOriginLadder1 = currentMV->GetTrackOrigin();  // In tracker coord.

    hitLadder1(2) = trackOriginLadder1(2);
    trackOriginLadder1(2) = 0.;

    DR3 tiltLadder1 = currentMV->GetTrackDirection();

    // The ladder move. We compute the new positions and tilts of minivectors.
   
    Double_t planeResolution = 3.5;
    Int_t maxHit = 2;
    
    DHit** hitsMiniVector1 = miniVectorLadder1->GetHitArray();

    DR3 HitPlanePos0(hitsMiniVector1[0]->GetPositionUhitCG(), hitsMiniVector1[0]->GetPositionVhitCG(), hitsMiniVector1[0]->GetPositionWhitCG()); 
    DR3 HitPlanePos1(hitsMiniVector1[1]->GetPositionUhitCG(), hitsMiniVector1[1]->GetPositionVhitCG(), hitsMiniVector1[1]->GetPositionWhitCG());
    
    miniVectorLadder1->Analyze(0, hitsMiniVector1, maxHit, planeResolution);
 
    DR3 newSlopes = miniVectorLadder1->GetLinearFit().GetSlopeZ();
    
    DPrecAlign* planeAlignement0 = hitsMiniVector1[0]->GetPlane()->GetPrecAlignment();
    DPrecAlign* planeAlignement1 = hitsMiniVector1[1]->GetPlane()->GetPrecAlignment();
    
    DR3 newPositionHitLeft  =  planeAlignement0->TransformHitToTracker( HitPlanePos0 );
    DR3 newPositionHitRight =  planeAlignement1->TransformHitToTracker( HitPlanePos1 );

    DR3 afterRotation1 = DR3( (newPositionHitLeft(0)+newPositionHitRight(0))/2., (newPositionHitLeft(1)+newPositionHitRight(1))/2., (newPositionHitLeft(2)+newPositionHitRight(2))/2. );
 
    DataPoints* afterMoveLadder = new DataPoints( 0., 0., afterRotation1(0), afterRotation1(1), afterRotation1(2), newSlopes(0),  newSlopes(1) );
 
    fAlignement->CalculateIntersection(afterMoveLadder);
    DR3 pointLadderToAlign = fAlignement->GetTrackPosition();
    DR3 ladderToAlignUV = fAlignement->TransformTrackToPlane();
    
    DR3 tiltLadderToAlign = newSlopes;
 
    // ladder 0 
    
    fAlignement->CalculateIntersection(myNewMV0);
    DR3 hitLadder0_tracker = fAlignement->GetTrackPosition();
    DR3 hitLadder0_plane = fAlignement->TransformTrackToPlane();

    hitLadder0 = hitLadder0_plane;

    delete afterMoveLadder;
    delete myNewMV0;

    DPlane* myPlane = ladderToAlign->GetPlane(1);
    DPrecAlign* myPrecAlignPlane = myPlane->GetPrecAlignment();
    myPrecAlignPlane->CalculateIntersection( afterMoveLadder ); 
    
    DR3 trackerPos = myPrecAlignPlane->GetTrackPosition();
    DR3 planePos = myPrecAlignPlane->TransformHitToPlane(trackerPos);

    DPlane* myPlane2 = ladderToAlign->GetPlane( ladderToAlign->GetNumberOfPlanes()/2+1 );
    DPrecAlign* myPrecAlignPlane2 = myPlane2->GetPrecAlignment();
    myPrecAlignPlane2->CalculateIntersection( afterMoveLadder ); 
    
    DR3 trackerPos2 = myPrecAlignPlane2->GetTrackPosition();
    DR3 planePos2 = myPrecAlignPlane2->TransformHitToPlane(trackerPos2);

    DR3 temp = ladderToAlignUV - hitLadder0;
    DR3 tiltTemp = tiltLadderToAlign - tiltLadder0;
    
    currentMV0_bis = (DataPoints*) dataMV0->After(currentMV0);
    currentMV_bis  = (DataPoints*) dataMV->After(currentMV);

    miniVectorLadder0_bis = (DTrack*) miniVectorsLadder0->After(miniVectorLadder0);
    miniVectorLadder1_bis = (DTrack*) miniVectorsLadder1->After(miniVectorLadder1);
    
    if( temp(0)<(meanU-nSigmas*sigmaU) || temp(0)>(meanU+nSigmas*sigmaU) || temp(1)<(meanV-nSigmas*sigmaV) || temp(1)>(meanV+nSigmas*sigmaV) 
        || tiltTemp(0)<(meanDu-nSigmas*sigmaDu) || tiltTemp(0)>(meanDu+nSigmas*sigmaDu) || tiltTemp(1)<(meanDv-nSigmas*sigmaDv) || tiltTemp(1)>(meanDv+nSigmas*sigmaDv) ) {
      std::cout<<"Removing Data ..."<<std::endl;
      dataMV0->Remove(currentMV0);
      dataMV->Remove(currentMV);
      miniVectorsLadder0->Remove(miniVectorLadder0);
      miniVectorsLadder1->Remove(miniVectorLadder1);
    }

    currentMV0 = currentMV0_bis;
    currentMV  = currentMV_bis;

    miniVectorLadder0 = miniVectorLadder0_bis;
    miniVectorLadder1 = miniVectorLadder1_bis;

    sizeMin--;
    ipt++;
  } // end for 
 
}
*/

//______________________________________________________________________________
//
Double_t MimosaAlignAnalysis::ComputeTotalChiSquareMV(const Double_t* par)
{
  // fcn function used by the MINUIT Fitter
  // For alignment with mini-vectors (AlignLadderMV)
  // 2014/12/19, LC: Now we use MiniVector object.
  // 2014/12/20, MVs saved in DLadder + code simplifiactions.
  // 2014/12/20, Minuit2

  Bool_t setMonteCarlo = 0;

  DPrecAlign* ladder1DPrecAlign = fLadders[1]->GetPrecAlign();
  DPrecAlign* ladder0DPrecAlign = fLadders[0]->GetPrecAlign();
  
  ladder1DPrecAlign->SetTranslation(par[3],par[4],par[5]);
  ladder1DPrecAlign->SetRotations(par[0],par[1],par[2]);
     
  fLadders[1]->UpdateAlignment();
  
  if(printResiduals==0) {

    std::cout<<" Th0 = "<<par[0]*180./TMath::Pi()<<" Th1 = "<<par[1]*180./TMath::Pi()<<" Th3 = "<<par[2]*180./TMath::Pi()<<std::endl;
    std::cout<<"   X = "<<par[3]<<"   Y = "<<par[4]<<"   Z = "<<par[5]<<std::endl;
  }

  //Compute the chi-square
  double_t chiSquare = 0.0;
  double_t chiSquare1Pair = 0.0;

  std::vector<MiniVector*> miniVectorsLadder0 = fLadders[0]->GetAssociatedMiniVectors();
  std::vector<MiniVector*> miniVectorsLadder1 = fLadders[1]->GetAssociatedMiniVectors();

  Double_t sigmaU = 3.5;
  Double_t sigmaV = 3.5;
  Double_t sigmaW = 10.0;
  Double_t sigmaU0 = 3.5;
  Double_t sigmaV0 = 3.5;

  if(printResiduals==1) {
    
    fDu->Reset();
    fDv->Reset();
    fDw->Reset();
    fDdu->Reset();
    fDdv->Reset();
    fDdw->Reset();

    fChiSquareX->Reset();
    fChiSquareY->Reset();
    fChiSquareZ->Reset();
    fChiSquareTiltX->Reset();
    fChiSquareTiltY->Reset();

    for(Int_t i=0 ; i<6 ; ++i) fGraph[i].Set(0);
  }

  for( Int_t i=0 ; i<int(miniVectorsLadder0.size()) ; ++i) {

      // Ladder0 :
      DR3 trackerPositionMV_0;
      DR3 ladderPositionMV_0;
      DR3 tiltsMV_0;
      
      // Ladder1 :
      DR3 trackerPositionMV_1;
      DR3 ladderPositionMV_1;
      DR3 tiltsMV_1;
  
      DR3 planeFrameLeft;
      DR3 planeFrameRight;

      DR3 realPlaneFrameLeft;
      DR3 realPlaneFrameRight;
   
    if(setMonteCarlo == 0) { 
    
      // Ladder0 :
      trackerPositionMV_0 = miniVectorsLadder0[i]->GetMiniVectorTrackerCenter();  // current MV ladder0 in tracker frame 
      ladderPositionMV_0  = miniVectorsLadder0[i]->GetMiniVectorLadderCenter();   // current MV ladder0 in ladder frame
      tiltsMV_0           = miniVectorsLadder0[i]->GetMiniVectorDirection();      // current MV direction on ladder0 in tracker frame
      
      // Ladder1 :
      trackerPositionMV_1 = miniVectorsLadder1[i]->GetMiniVectorTrackerCenter();  // current MV ladder1 in tracker frame
      ladderPositionMV_1  = miniVectorsLadder1[i]->GetMiniVectorLadderCenter();   // current MV ladder1 in ladder1 frame
      tiltsMV_1           = miniVectorsLadder1[i]->GetMiniVectorDirection();      // current MV  direction on ladder1 in tracker frame
  
      planeFrameLeft          = miniVectorsLadder1[i]->GetHitLeftPlanePosition();
      planeFrameRight         = miniVectorsLadder1[i]->GetHitRightPlanePosition();

      realPlaneFrameLeft      = miniVectorsLadder1[i]->GetRealLeftPlaneFrame();
      realPlaneFrameRight     = miniVectorsLadder1[i]->GetRealRightPlaneFrame();
    }
    else if(setMonteCarlo == 1) {
      if(miniVectorsLadder0[i]->IfMonteCarlo() == 1 && miniVectorsLadder1[i]->IfMonteCarlo() == 1 ) {
  
        // Ladder0 :
        trackerPositionMV_0 = miniVectorsLadder0[i]->GetMiniVectorTrackerCenterMC();  // current MV ladder0 in tracker frame 
        ladderPositionMV_0  = miniVectorsLadder0[i]->GetMiniVectorLadderCenterMC();   // current MV ladder0 in ladder frame
        tiltsMV_0           = miniVectorsLadder0[i]->GetMiniVectorDirectionMC();      // current MV direction on ladder0 in tracker frame
      
        // Ladder1 :
        trackerPositionMV_1 = miniVectorsLadder1[i]->GetMiniVectorTrackerCenterMC();  // current MV ladder1 in tracker frame
        ladderPositionMV_1  = miniVectorsLadder1[i]->GetMiniVectorLadderCenterMC();   // current MV ladder1 in ladder1 frame
        tiltsMV_1           = miniVectorsLadder1[i]->GetMiniVectorDirectionMC();      // current MV  direction on ladder1 in tracker frame
  
        planeFrameLeft          = miniVectorsLadder1[i]->GetHitLeftPlanePositionMC();
        planeFrameRight         = miniVectorsLadder1[i]->GetHitRightPlanePositionMC();

        realPlaneFrameLeft      = miniVectorsLadder1[i]->GetRealLeftPlaneFrameMC();
        realPlaneFrameRight     = miniVectorsLadder1[i]->GetRealRightPlaneFrameMC();

      }
      else continue;
    }

    DPrecAlign* DPrecAlignLeft  = miniVectorsLadder1[i]->GetDPrecAlignLeft();
    DPrecAlign* DPrecAlignRight = miniVectorsLadder1[i]->GetDPrecAlignRight();

    DPrecAlign* DPrecAlignLeft0  = miniVectorsLadder0[i]->GetDPrecAlignLeft();
    DPrecAlign* DPrecAlignRight0 = miniVectorsLadder0[i]->GetDPrecAlignRight();

/* 
    std::cout<<" Diff = "<<std::endl;
    (planeFrameLeft-realPlaneFrameLeft).Print();
    (planeFrameRight-realPlaneFrameRight).Print();
    std::cout<<std::endl;
*/ 
   /* 
    std::cout<<"Tilts : "<<std::endl;
    tiltsMV_0.Print();
    tiltsMV_1.Print();
   */
    DR3 projectionMV0_OnLadder1_TrackerFrame;   
    DR3 projectionMV0_OnLadder1_LadderFrame; 
    
    if(setMonteCarlo == 0) {
      
      // Projections :
      projectionMV0_OnLadder1_TrackerFrame = miniVectorsLadder0[i]->CalculateMiniVectorIntersectionTracker(ladder1DPrecAlign);  // projection of current MV of ladder0 on ladder1 in tracKER frame
      projectionMV0_OnLadder1_LadderFrame  = miniVectorsLadder0[i]->CalculateMiniVectorIntersectionLadder(ladder1DPrecAlign);   // projection of current MV of ladder0 on ladder1 in ladder1 frame
    }
    else if(setMonteCarlo == 1) {
      
      // Projections :
      projectionMV0_OnLadder1_TrackerFrame = miniVectorsLadder0[i]->CalculateMiniVectorIntersectionTrackerMC(ladder1DPrecAlign);  // projection of current MV of ladder0 on ladder1 in tracKER frame
      projectionMV0_OnLadder1_LadderFrame  = miniVectorsLadder0[i]->CalculateMiniVectorIntersectionLadderMC(ladder1DPrecAlign);   // projection of current MV of ladder0 on ladder1 in ladder1 frame

    }
    
    // Compute relative MV tilt wrt the ladder
    Double_t roll  = atan2(tiltsMV_1(1), tiltsMV_1(2));
    Double_t pitch = atan2(tiltsMV_1(0), tiltsMV_1(2));
    
    Double_t diffAngleX = par[2]*180./TMath::Pi() - roll*180./TMath::Pi();    // incidence MV tiltX on ladder1
    Double_t diffAngleY = par[1]*180./TMath::Pi() - pitch*180./TMath::Pi();  // incidence MV tiltY on ladder1
    
    diffAngleX = fabs(diffAngleX);
    diffAngleY = fabs(diffAngleY);

/*
    std::cout<<"diffAngleX = "<<diffAngleX<<std::endl;
    std::cout<<"diffAngleY = "<<diffAngleY<<std::endl;
*/

    // Resolution in function of the tilts :: G4 Simu numbers.
  
    if(diffAngleX<=2.5) sigmaV = 3.5;
    else if(diffAngleX>2.5 && diffAngleX<=5) sigmaV = 3.7;
    else if(diffAngleX>5  && diffAngleX<=25) sigmaV = 3.9;
    else if(diffAngleX>25 && diffAngleX<=30) sigmaV = 4.0;
    else if(diffAngleX>30 && diffAngleX<=40) sigmaV = 4.25;
    else if(diffAngleX>40 && diffAngleX<=50) sigmaV = 4.4;
    else if(diffAngleX>50 && diffAngleX<=60) sigmaV = 4.75;
    else if(diffAngleX>60) sigmaV = 5.0; 

    if(diffAngleY<=2.5) sigmaU = 3.5;
    else if(diffAngleY>2.5 && diffAngleY<=5) sigmaU = 3.7;
    else if(diffAngleY>5  && diffAngleY<=25) sigmaU = 3.9;
    else if(diffAngleY>25 && diffAngleY<=30) sigmaU = 4.0;
    else if(diffAngleY>30 && diffAngleY<=40) sigmaU = 4.25;
    else if(diffAngleY>40 && diffAngleY<=50) sigmaU = 4.4;
    else if(diffAngleY>50 && diffAngleY<=60) sigmaU = 4.75;
    else if(diffAngleY>60) sigmaU = 5.0; 
   
    DR3 deltaCoordMV1;
    DR3 deltaCoordMV0;
        
    if(setMonteCarlo == 0) {
    
      deltaCoordMV1 = miniVectorsLadder1[i]->GetHitRightTrackerPosition() - miniVectorsLadder1[i]->GetHitLeftTrackerPosition();
      deltaCoordMV0 = miniVectorsLadder0[i]->GetHitRightTrackerPosition() - miniVectorsLadder0[i]->GetHitLeftTrackerPosition();
    }
    else if(setMonteCarlo == 1 ) {

      deltaCoordMV1 = miniVectorsLadder1[i]->GetHitRightTrackerPositionMC() - miniVectorsLadder1[i]->GetHitLeftTrackerPositionMC();
      deltaCoordMV0 = miniVectorsLadder0[i]->GetHitRightTrackerPositionMC() - miniVectorsLadder0[i]->GetHitLeftTrackerPositionMC();
    }

    Double_t deltaZ = fabs( deltaCoordMV1(2) );
    Double_t deltaY = fabs( deltaCoordMV1(1) );
    Double_t deltaX = fabs( deltaCoordMV1(0) );

    Double_t deltaZ_MV0 = fabs( deltaCoordMV0(2) );
    Double_t deltaY_MV0 = fabs( deltaCoordMV0(1) );
    Double_t deltaX_MV0 = fabs( deltaCoordMV0(0) );

    //std::cout<<deltaX<<"  "<<deltaY<<"  "<<deltaZ<<std::endl;

    DR3 MV1            = ladderPositionMV_1;
    DR3 projectionMV0  = projectionMV0_OnLadder1_LadderFrame;
    DR3 projectionMV0_Tracker = projectionMV0_OnLadder1_TrackerFrame;
    
    DR3 tiltMV0 = tiltsMV_0;
    DR3 tiltMV1 = tiltsMV_1; 
   
/*    
    std::cout<<"Tilts = "<<std::endl;
    tiltsMV_0.Print();
    tiltsMV_1.Print();
    std::cout<<"Projections = "<<std::endl;
    MV1.Print();
    projectionMV0.Print();
    std::cout<<"Delta = "<<std::endl;
    std::cout<<deltaX<<"  "<<deltaY<<"  "<<deltaZ<<std::endl;
*/

/*  
    sigmaU=3.5;
    sigmaV=3.5;
*/    
/*
    std::cout<<"index = "<<i<<" Coord MV1 = ";
    MV1.Print();
    std::cout<<" Coord MV0 projection (ladder1) = ";
    projectionMV0.Print();
    std::cout<<" Coord MV0 projection (tracker) = ";
    projectionMV0_OnLadder1_TrackerFrame.Print();
    std::cout<<"params = "<<par[0]<<"  "<<par[1]<<"  "<<par[2]<<"  "<<par[3]<<"  "<<par[4]<<"  "<<par[5]<<std::endl;
    ladderPositionMV_0.Print();
    ladderPositionMV_1.Print();
    std::cout<<std::endl;
*/
//    ladder1DPrecAlign->PrintAll();

    Double_t* rotationMatrixLeft  = DPrecAlignLeft->GetTorationMatrix(); 
    Double_t* rotationMatrixRight = DPrecAlignRight->GetTorationMatrix();
    Double_t* rotationMatrixLadder = ladder1DPrecAlign->GetTorationMatrix();

    Double_t* rotationMatrixLeft0  = DPrecAlignLeft0->GetTorationMatrix(); 
    Double_t* rotationMatrixRight0 = DPrecAlignRight0->GetTorationMatrix();
    Double_t* rotationMatrixLadder0 = ladder0DPrecAlign->GetTorationMatrix();

    // MV1
    Double_t sigmaXLeftSquare = pow(rotationMatrixLeft[0]*sigmaU + rotationMatrixLeft[1]*sigmaV, 2);
    Double_t sigmaXRightSquare = pow(rotationMatrixRight[0]*sigmaU + rotationMatrixRight[1]*sigmaV, 2);

    Double_t sigmaYLeftSquare = pow(rotationMatrixLeft[3]*sigmaU + rotationMatrixLeft[4]*sigmaV, 2);
    Double_t sigmaYRightSquare = pow(rotationMatrixRight[3]*sigmaU + rotationMatrixRight[4]*sigmaV, 2);

    Double_t sigmaZLeftSquare = pow(rotationMatrixLeft[6]*sigmaU + rotationMatrixLeft[7]*sigmaV, 2);
    Double_t sigmaZRightSquare = pow(rotationMatrixRight[6]*sigmaU + rotationMatrixRight[7]*sigmaV, 2);

    Double_t zRightMinusZLeft_Square = deltaZ*deltaZ;

    Double_t sigmaTiltXSquare = (sigmaXLeftSquare+sigmaXRightSquare)/(zRightMinusZLeft_Square) + ( (tiltMV1(0)*tiltMV1(0))/(zRightMinusZLeft_Square) )*(sigmaZLeftSquare+sigmaZRightSquare) ;
    Double_t sigmaTiltYSquare = (sigmaYLeftSquare+sigmaYRightSquare)/(zRightMinusZLeft_Square) + ( (tiltMV1(1)*tiltMV1(1))/(zRightMinusZLeft_Square) )*(sigmaZLeftSquare+sigmaZRightSquare) ;

    // MV0
    Double_t sigmaXLeftSquareMV0 = pow(rotationMatrixLeft0[0]*sigmaU0 + rotationMatrixLeft0[1]*sigmaV0, 2);
    Double_t sigmaXRightSquareMV0 = pow(rotationMatrixRight0[0]*sigmaU0 + rotationMatrixRight0[1]*sigmaV0, 2);

    Double_t sigmaYLeftSquareMV0 = pow(rotationMatrixLeft0[3]*sigmaU0 + rotationMatrixLeft0[4]*sigmaV0, 2);
    Double_t sigmaYRightSquareMV0 = pow(rotationMatrixRight0[3]*sigmaU0 + rotationMatrixRight0[4]*sigmaV0, 2);

    Double_t sigmaZLeftSquareMV0 = pow(rotationMatrixLeft0[6]*sigmaU0 + rotationMatrixLeft0[7]*sigmaV0, 2);
    Double_t sigmaZRightSquareMV0 = pow(rotationMatrixRight0[6]*sigmaU0 + rotationMatrixRight0[7]*sigmaV0, 2);

    Double_t zRightMinusZLeft_SquareMV0 = deltaZ_MV0*deltaZ_MV0;

    Double_t sigmaTiltXSquareMV0 = (sigmaXLeftSquareMV0+sigmaXRightSquareMV0)/(zRightMinusZLeft_SquareMV0) + ( (tiltMV0(0)*tiltMV0(0))/(zRightMinusZLeft_SquareMV0) )*(sigmaZLeftSquareMV0+sigmaZRightSquareMV0) ;
    Double_t sigmaTiltYSquareMV0 = (sigmaYLeftSquareMV0+sigmaYRightSquareMV0)/(zRightMinusZLeft_SquareMV0) + ( (tiltMV0(1)*tiltMV0(1))/(zRightMinusZLeft_SquareMV0) )*(sigmaZLeftSquareMV0+sigmaZRightSquareMV0) ;

/*
    std::cout<<rotationMatrixLeft[0]<<"  "<<rotationMatrixLeft[3]<<"  "<<rotationMatrixLeft[6]<<"   sigmaYLeftSquare = "<<sigmaYLeftSquare<<std::endl;
    std::cout<<"sigmaZLeftSquare = "<<sigmaZLeftSquare<<std::endl;
    std::cout<<"sigmaZRightSquare = "<<sigmaZRightSquare<<std::endl;
    std::cout<<" tiltMV1(0) = "<<tiltMV1(0)<<"  tiltMV1(1) = "<<tiltMV1(1)<<std::endl;
*/
    // Chi2 for tilts :
    Double_t squarePullTiltX = (tiltMV0(0)-tiltMV1(0))*(tiltMV0(0)-tiltMV1(0))/(sigmaTiltXSquare+sigmaTiltXSquareMV0);
    Double_t squarePullTiltY = (tiltMV0(1)-tiltMV1(1))*(tiltMV0(1)-tiltMV1(1))/(sigmaTiltYSquare+sigmaTiltYSquareMV0);

    //std::cout<<" part1 = "<<(sigmaXLeftSquare+sigmaXRightSquare)/(zRightMinusZLeft_Square)<<"   part 2 = "<<( (tiltMV1(0)*tiltMV1(0))/(z1MinusZ0_Square) )*(sigmaZLeftSquare+sigmaZRightSquare)<<std::endl;

    //std::cout<<"sigmaTiltXSquare = "<<sigmaTiltXSquare<<"  sigmaTiltYSquare = "<<sigmaTiltYSquare<<std::endl;
    
    // MV1 center resolution : 
    Double_t sigmaXSquare = (rotationMatrixLeft[0]+rotationMatrixRight[0])*(rotationMatrixLeft[0]+rotationMatrixRight[0])*sigmaU*sigmaU/2. +  (rotationMatrixLeft[1]+rotationMatrixRight[1])*(rotationMatrixLeft[1]+rotationMatrixRight[1])*sigmaV*sigmaV/2.;
    Double_t sigmaYSquare = (rotationMatrixLeft[3]+rotationMatrixRight[3])*(rotationMatrixLeft[3]+rotationMatrixRight[3])*sigmaU*sigmaU/2. +  (rotationMatrixLeft[4]+rotationMatrixRight[4])*(rotationMatrixLeft[4]+rotationMatrixRight[4])*sigmaV*sigmaV/2.;
    Double_t sigmaZSquare = (rotationMatrixLeft[6]+rotationMatrixRight[6])*(rotationMatrixLeft[6]+rotationMatrixRight[6])*sigmaU*sigmaU/2. +  (rotationMatrixLeft[7]+rotationMatrixRight[7])*(rotationMatrixLeft[7]+rotationMatrixRight[7])*sigmaV*sigmaV/2.;

    // MV0 center resolution :
    Double_t sigmaXSquareMV0 = (rotationMatrixLeft0[0]+rotationMatrixRight0[0])*(rotationMatrixLeft0[0]+rotationMatrixRight0[0])*sigmaU0*sigmaU0/2. +  (rotationMatrixLeft0[1]+rotationMatrixRight0[1])*(rotationMatrixLeft0[1]+rotationMatrixRight0[1])*sigmaV0*sigmaV0/2.;
    Double_t sigmaYSquareMV0 = (rotationMatrixLeft0[3]+rotationMatrixRight0[3])*(rotationMatrixLeft0[3]+rotationMatrixRight0[3])*sigmaU0*sigmaU0/2. +  (rotationMatrixLeft0[4]+rotationMatrixRight0[4])*(rotationMatrixLeft0[4]+rotationMatrixRight0[4])*sigmaV0*sigmaV0/2.;
    Double_t sigmaZSquareMV0 = (rotationMatrixLeft0[6]+rotationMatrixRight0[6])*(rotationMatrixLeft0[6]+rotationMatrixRight0[6])*sigmaU0*sigmaU0/2. +  (rotationMatrixLeft0[7]+rotationMatrixRight0[7])*(rotationMatrixLeft0[7]+rotationMatrixRight0[7])*sigmaV0*sigmaV0/2.;
   
    // MV0 X0=(x0,y0,z0) errors (Origin is defined by z0=0, so the sigmaDx0=0)
    Double_t sigmaSquareOriginX_MV0 = sigmaXSquareMV0 + trackerPositionMV_0(2)*trackerPositionMV_0(2)*sigmaTiltXSquareMV0 + tiltMV0(0)*tiltMV0(0)*sigmaZSquareMV0;
    Double_t sigmaSquareOriginY_MV0 = sigmaYSquareMV0 + trackerPositionMV_0(2)*trackerPositionMV_0(2)*sigmaTiltYSquareMV0 + tiltMV0(1)*tiltMV0(1)*sigmaZSquareMV0;
   
    // Errors on MV0 projection on Ladder1 :
    // In X direction we have : sigmaX_OnLadder1 = D*sigmaSquareOriginX_MV0 + E*sigmaSquareOriginY_MV0 + F*sigmaSquareOriginZ_MV0 + G*sigmaTiltXSquareMV0 + H*sigmaTiltYSquareMV0
    // We use : 
    // --> the ladder1 plane equation. With the coeff A, B, C.
    // --> the errors on the MV0 origin and titls (track vector : V=(tiltX, tiltY, tilZ=1.) )
    // Finally we have :
    // D = (dx_ladder1/dxOrigin)^2 = ( 1 - A*tiltX/(A*tiltX + B*tiltY +1.0) )^2
    // E = (dx_ladder1/dyOrigin)^2 = ( -B*tiltY/(A*tiltX + B*tiltY +1.0) )^2
    // F = (dx_ladder1/dzOrigin)^2 = ( -TiltX/(A*tiltX + B*tiltY +1.0) )^2
    // G = (dx_ladder1/dTiltX)^2   = -(A*originX + B*OriginY + OriginZ - C)/(A*tiltX + B*tiltY +1.0) + A*tiltX*(A*originX + B*OriginY + OriginZ - C)/((A*tiltX + B*tiltY +1.0)^2) )^2
    // H = (dx_ladder1/dTiltY)^2   = ( B*tiltX*(A*originX + B*OriginY + OriginZ - C)/((A*tiltX + B*tiltY +1.0)^2) )^2
    // sigmaSquareOriginZ_MV0 = 0 because the origin definition is z0 = 0.

    DR3 coeffEquationLadder1 = ladder1DPrecAlign->GetCoeffs();
    Double_t A = coeffEquationLadder1(0);
    Double_t B = coeffEquationLadder1(1);
    Double_t C = coeffEquationLadder1(2);

    Double_t denominator = A*tiltMV0(0)+B*tiltMV0(1) + 1.;
    DR3 originMV0 = miniVectorsLadder0[i]->GetMiniVectorOrigin();
    Double_t numerator = A*originMV0(0) + B*originMV0(1) + originMV0(2) + C;

    Double_t Dx = ( 1 - A*tiltMV0(0) / denominator )*( 1 - A*tiltMV0(0) / denominator );
    Double_t Ex = ( -B*tiltMV0(0) / denominator )*( -B*tiltMV0(0) / denominator );
    Double_t Gx = ( -numerator/denominator + A*tiltMV0(0)*numerator/(denominator*denominator) )*( -numerator/denominator + A*tiltMV0(0)*numerator/(denominator*denominator) );
    Double_t Hx = ( B*tiltMV0(0)*numerator/(denominator*denominator) )*( B*tiltMV0(0)*numerator/(denominator*denominator) );

    Double_t sigmaXSquare_OnLadder1 = Dx*sigmaSquareOriginX_MV0 + Ex*sigmaSquareOriginY_MV0 + Gx*sigmaTiltXSquareMV0 + Hx*sigmaTiltYSquareMV0;

    Double_t Dy = ( -tiltMV0(1)*A / denominator )*( -tiltMV0(1)*A / denominator );
    Double_t Ey = ( 1 - B*tiltMV0(1) / denominator )*( 1 - B*tiltMV0(1) / denominator );
    Double_t Gy = ( A*tiltMV0(1)*numerator / (denominator*denominator) )*( A*tiltMV0(1)*numerator / (denominator*denominator) );
    Double_t Hy = ( B*tiltMV0(1)*numerator/(denominator*denominator) - numerator/denominator )*( B*tiltMV0(1)*numerator/(denominator*denominator) - numerator/denominator );

    Double_t sigmaYSquare_OnLadder1 = Dy*sigmaSquareOriginX_MV0 + Ey*sigmaSquareOriginY_MV0 + Gy*sigmaTiltXSquareMV0 + Hy*sigmaTiltYSquareMV0;

    Double_t Dz = ( -A/denominator )*( -A/denominator );
    Double_t Ez = ( -B/denominator )*( -B/denominator );
    Double_t Gz = ( A*numerator/(denominator*denominator) )*( A*numerator/(denominator*denominator) );
    Double_t Hz = ( B*numerator/(denominator*denominator) )*( B*numerator/(denominator*denominator) );

    Double_t sigmaZSquare_OnLadder1 = Dz*sigmaSquareOriginX_MV0 + Ez*sigmaSquareOriginY_MV0 + Gz*sigmaTiltXSquareMV0 + Hz*sigmaTiltYSquareMV0;

    Double_t squarePullX = (trackerPositionMV_1(0) - projectionMV0_Tracker(0))*(trackerPositionMV_1(0) - projectionMV0_Tracker(0))/ (sigmaXSquare/*+sigmaXSquare_OnLadder1*/);
    Double_t squarePullY = (trackerPositionMV_1(1) - projectionMV0_Tracker(1))*(trackerPositionMV_1(1) - projectionMV0_Tracker(1))/ (sigmaYSquare/*+sigmaYSquare_OnLadder1*/);
    Double_t squarePullZ = (trackerPositionMV_1(2) - projectionMV0_Tracker(2))*(trackerPositionMV_1(2) - projectionMV0_Tracker(2))/ (sigmaZSquare/*+sigmaZSquare_OnLadder1*/);

    Double_t squarePullTrackX = (trackerPositionMV_1(0) - projectionMV0_Tracker(0))*(trackerPositionMV_1(0) - projectionMV0_Tracker(0))/ (/*sigmaXSquare+*/sigmaXSquare_OnLadder1);
    Double_t squarePullTrackY = (trackerPositionMV_1(1) - projectionMV0_Tracker(1))*(trackerPositionMV_1(1) - projectionMV0_Tracker(1))/ (/*sigmaYSquare+*/sigmaYSquare_OnLadder1);
    Double_t squarePullTrackZ = (trackerPositionMV_1(2) - projectionMV0_Tracker(2))*(trackerPositionMV_1(2) - projectionMV0_Tracker(2))/ (/*sigmaZSquare+*/sigmaZSquare_OnLadder1);

    Double_t squarePullTotalX = (trackerPositionMV_1(0) - projectionMV0_Tracker(0))*(trackerPositionMV_1(0) - projectionMV0_Tracker(0))/ (sigmaXSquare+sigmaXSquare_OnLadder1);
    Double_t squarePullTotalY = (trackerPositionMV_1(1) - projectionMV0_Tracker(1))*(trackerPositionMV_1(1) - projectionMV0_Tracker(1))/ (sigmaYSquare+sigmaYSquare_OnLadder1);
    Double_t squarePullTotalZ = (trackerPositionMV_1(2) - projectionMV0_Tracker(2))*(trackerPositionMV_1(2) - projectionMV0_Tracker(2))/ (sigmaZSquare+sigmaZSquare_OnLadder1);

/*
    std::cout<<"sigmaXSquare = "<<sigmaXSquare<<"  sigmaXSquare_OnLadder1 = "<<sigmaXSquare_OnLadder1<<std::endl;
    std::cout<<"sigmaYSquare = "<<sigmaYSquare<<"  sigmaYSquare_OnLadder1 = "<<sigmaYSquare_OnLadder1<<std::endl;
    std::cout<<"sigmaSquareOriginX_MV0 = "<<sigmaSquareOriginX_MV0<<"  sigmaSquareOriginY_MV0 = "<<sigmaSquareOriginY_MV0<<"  sigmaTiltXSquareMV0 = "<<sigmaTiltXSquareMV0<<"  sigmaTiltYSquareMV0 = "<<sigmaTiltYSquareMV0<<std::endl;
    std::cout<<"A  = "<<A<<"  B = "<<B<<"  C = "<<C<<" Denom = "<<denominator<<"  Dz = "<<Dz<<"Ez = "<<Ez<<"  Gz = "<<Gz<<"  Hz = "<< Hz <<std::endl;
    std::cout<<"sigmaZSquare = "<<sigmaZSquare<<"  sigmaZSquare_OnLadder1 = "<<sigmaZSquare_OnLadder1<<std::endl;
*/

    //Double_t squareResidualsU = (projectionMV0(0)-MV1(0))*(projectionMV0(0)-MV1(0))/(sigmaU*sigmaU/2.);
    //Double_t squareResidualsV = (projectionMV0(1)-MV1(1))*(projectionMV0(1)-MV1(1))/(sigmaV*sigmaV/2.); // erreur = \sigma_y^2 / 2
    
    //Double_t denomTiltX = 2.*sigmaU*sigmaU/(deltaZ*deltaZ) + /*2.*/sigmaZ*sigmaZ*deltaX*deltaX/(deltaZ*deltaZ*deltaZ*deltaZ);
    //Double_t squareResidualsTiltX = (tiltMV0(0)-tiltMV1(0))*(tiltMV0(0)-tiltMV1(0))/(denomTiltX); /* (2*sigmaU*sigmaU/(deltaZ*deltaZ)); */
    //Double_t denomTiltY = 2.*sigmaV*sigmaV/(deltaZ*deltaZ) + /*2.*/sigmaZ*sigmaZ*deltaY*deltaY/(deltaZ*deltaZ*deltaZ*deltaZ);
    //Double_t squareResidualsTiltY = (tiltMV0(1)-tiltMV1(1))*(tiltMV0(1)-tiltMV1(1))/(denomTiltY); /* (2*sigmaV*sigmaV/(deltaZ*deltaZ)); */

    //Double_t squareResidualsTiltX = (tiltMV0(0)-tiltMV1(0))*(tiltMV0(0)-tiltMV1(0))/sigmaTiltXSquare;
    //Double_t squareResidualsTiltY = (tiltMV0(1)-tiltMV1(1))*(tiltMV0(1)-tiltMV1(1))/sigmaTiltYSquare;

    Double_t coeff = 1;
    chiSquare1Pair = 0.;
    chiSquare1Pair = squarePullX + squarePullY + squarePullZ /*+ squarePullTrackX + squarePullTrackY + squarePullTrackZ*/ /*squarePullTotalX + squarePullTotalY + squarePullTotalZ*/ + coeff*(squarePullTiltX + squarePullTiltY) ; // Calcul du Chi2. 
    //chiSquare1Pair = /*squarePullX + squarePullY+ squarePullZ + squarePullTrackX + squarePullTrackY + squarePullTrackZ */squarePullTotalX + squarePullTotalY + squarePullTotalZ + coeff*(squarePullTiltX + squarePullTiltY) ; // Calcul du Chi2. 

    chiSquare += chiSquare1Pair;

//    std::cout<<"sigmaTiltXSquare = "<<sigmaTiltXSquare<<"  sigmaTiltYSquare = "<<sigmaTiltYSquare<<std::endl;
//    std::cout<<" squareResidualsU = "<<squareResidualsU<<" squareResidualsV = "<<squareResidualsU<<" squareResidualsTiltX = "<<squareResidualsTiltX<<" squareResidualsTiltY = "<<squareResidualsTiltX<<std::endl;
    
    if(printResiduals==1) {
     
      DR3 temp = ladderPositionMV_1 - projectionMV0_OnLadder1_LadderFrame;
      DR3 tiltTemp = tiltsMV_1 - tiltsMV_0;

      fGraph[0].SetPoint( i, ladderPositionMV_1(0), temp(0) );
      fGraph[1].SetPoint( i, ladderPositionMV_1(0), temp(1) );
      fGraph[2].SetPoint( i, ladderPositionMV_1(1), temp(0) );
      fGraph[3].SetPoint( i, ladderPositionMV_1(1), temp(1) );
      fGraph[4].SetPoint( i, tiltsMV_1(0), tiltTemp(0) );
      fGraph[5].SetPoint( i, tiltsMV_1(1), tiltTemp(1) );
      fDu->Fill( temp(0) );
      fDv->Fill( temp(1) );
      fDw->Fill( temp(2) );
      fDdu->Fill( 1000.*tiltTemp(0) );
      fDdv->Fill( 1000.*tiltTemp(1) );
      fDdw->Fill( chiSquare1Pair );

      fChiSquareX->Fill(squarePullTotalX);
      fChiSquareY->Fill(squarePullTotalY);
      fChiSquareZ->Fill(squarePullTotalZ);
      fChiSquareTiltX->Fill(squarePullTiltX);
      fChiSquareTiltY->Fill(squarePullTiltY);

    } // end if printResiduals

  } // end for

  //chiSquare /= miniVectorsLadder0.size();
  std::cout<<"Total Chi2 = "<<chiSquare<<std::endl;
  return chiSquare;

}


//______________________________________________________________________________
//
void MimosaAlignAnalysis::DrawResiduals()
{
  // Draw the residuals
  // Use the new functions computation.
  // Faire fonction pour calculer les projections :)

  fCnv->cd(1);
  fDu->Fit("gaus","q");
  fDu->SaveAs("fDu.root");
      
  fCnv->cd(2);
      
  fDv->Fit("gaus","q");
  fDv->SaveAs("fDv.root");
  fCnv->Update();
  fCnv->Draw();
     
  fCnv->cd(3);                                                                                                                                                            
    
  fDw->Fit("gaus","q");
  fDw->SaveAs("fDw.root");
  fCnv->Update();
  fCnv->Draw();

  fCnv->cd(4);
      
  fDdu->Fit("gaus","q");
  fDdu->SaveAs("fDdu.root");
  fCnv->Update();
  fCnv->Draw();
      
  fCnv->cd(5);
      
  fDdv->Fit("gaus","q");
  fDdv->SaveAs("fDdv.root");
  fCnv->Update();
  fCnv->Draw();

  fCnv->cd(6);
      
  fDdw->Fit("gaus","q");
  fDdw->SaveAs("fDdw.root");
  fCnv->Update();
  fCnv->Draw();

  fCanvasChiSquare->cd(1);
  fChiSquareX->Draw();
  fCanvasChiSquare->Draw();
  fCanvasChiSquare->Update();

  fCanvasChiSquare->cd(2);
  fChiSquareY->Draw();
  fCanvasChiSquare->Draw();
  fCanvasChiSquare->Update();

  fCanvasChiSquare->cd(3);
  fChiSquareZ->Draw();
  fCanvasChiSquare->Draw();
  fCanvasChiSquare->Update();

  fCanvasChiSquare->cd(4);
  fChiSquareTiltX->Draw();
  fCanvasChiSquare->Draw();
  fCanvasChiSquare->Update();

  fCanvasChiSquare->cd(5);
  fChiSquareTiltY->Draw();
  fCanvasChiSquare->Draw();
  fCanvasChiSquare->Update();
      
  for(Int_t j=0; j<6 ; j++) {
        
    fCcr->cd(j+1);
    fGraph[j].SetMarkerStyle(24);
    fGraph[j].SetMarkerColor(2);
    fGraph[j].SetMarkerSize(0.2);
    fGraph[j].Draw("AP");
  }

  fGraph[0].GetHistogram()->SetXTitle("U hit");
  fGraph[0].GetHistogram()->SetYTitle("DU");
    
  fGraph[1].GetHistogram()->SetXTitle("U hit");
  fGraph[1].GetHistogram()->SetYTitle("DV");
     
  fGraph[2].GetHistogram()->SetXTitle("V hit");
  fGraph[2].GetHistogram()->SetYTitle("DU");
    
  fGraph[3].GetHistogram()->SetXTitle("V hit");
  fGraph[3].GetHistogram()->SetYTitle("DV");
      
  fGraph[4].GetHistogram()->SetXTitle("DU");
  fGraph[4].GetHistogram()->SetYTitle("Delta DU");
      
  fGraph[5].GetHistogram()->SetXTitle("DV");
  fGraph[5].GetHistogram()->SetYTitle("Delta DV");
      
  fCcr->Update();
  fCcr->Draw(); 

}

//______________________________________________________________________________
//
DPrecAlign* MimosaAlignAnalysis::AlignMimosa(DPrecAlign* initAlignment, TFile* aCorFile, const char* MimosaResultDir, Double_t aDistance)
{
  // Mimosa alignement method:
  // Take an input aligment (initAlignement) and fit its data to determine
  //  the best plane alignment (3 shifts and 3 angles).
  // The fit is actually done within an interactive MINUIT session.
  //
  // The original version was considering z=0 as fixed,
  //  the present version allows for z!=0 and fit it.
  //
  // After the fit, the user can choose between saving or not the
  //  fitted parameters in the given CorParFile.
  //
  // Modified JB 2010/09/08
  // Modified JB 2011/06/18, some reshape + new arguments
  // Modified JB 2012/05/12, use new arg aDistance for plot range
  // Modified SS 2012/09/05, datapoints cleaning before storing and SET STRATEGY
  // Modified JB 2013/06/22, correct z parameter fixed in fit
  // Modified JB 2013/07/14 name of angles changed from th0/1/2 to tiltx/y/z 

  Info("AlignMimosa","Aligning Mimosa Now...");
//  AlignDebug = 3;

  if(!initAlignment || !(initAlignment->GetDataPoints()->First())) Fatal("AlignMimosa","No initial alignment found or empty!");
       
  //init
  fAlignement = initAlignment;
  fDu = fDv = 0; 
  fCnv = fCcr = 0;
  //fMyfit = 0;

  //function for fit
  /*fMyfit = new TF1("myfit","[0]/sqrt(2*3.1416)/[2] * exp(-(x-[1])*(x-[1])/(2.0*[2]*[2])) + [3]/sqrt(2.0*3.1416)/[5]* exp(-(x-[4])*(x-[4])/(2.0*[5]*[5])) ", 0, 2);

  fMyfit->SetParameter(0,500);
  fMyfit->SetParameter(1,0);
  fMyfit->SetParameter(2,5);
  fMyfit->SetParameter(3,500);
  fMyfit->SetParameter(4,-20);
  fMyfit->SetParameter(5,5);
  */
  //cd to result dir
  gSystem->cd(MimosaResultDir);
  if(AlignDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl;

  // save all open canvas in a root file 
  Char_t Header[200];
  sprintf(Header,"Align_Plots_run%d_Pl%d.root", fSession->GetRunNumber(), fSession->GetPlaneNumber());
  TFile AlignPlots(Header,"update");
  if (AlignPlots.IsZombie() || !AlignPlots.IsOpen()) {
    AlignPlots.Close();
    AlignPlots.ReOpen("create");
  }

  // moved here from FCN, to avoid deletion after procedure, JB 2013/09/19
  fCnv = new TCanvas("Cnv","Residues after alignment",2,10,400,500);
  fCnv->Divide(1,2);
  fCnv->Update();

  fCcr = new TCanvas("ccr","Correlations",410,10,500,500);
  fCcr->Divide(2,2);
  fCcr->Update();

  for (Int_t i=0; i<4 ; i++) {
    fGraph[i].SetPoint(0,0,0);
  }
  

  if(AlignDebug >= 3) 
    {  
      cout << "Run Number = " <<  fSession->GetRunNumber() <<endl;
      cout << "Plane Number = " << fSession->GetPlaneNumber() <<endl;
      cout << "Initial parameters" << endl;
      fAlignement->PrintAll();
      cout << "Gonna list data " << endl;
      fAlignement->ListDataPoints();
    }

  Double_t  *rot = 0;
  Double_t  *tra = 0;
  Double_t  theta[3]={0,0,0}; // JB, removed for inline compilation
  Double_t  trU,trV, trW; // trW added, JB 2010/09/08
  trU=trV=trW=0;
  rot = fAlignement->GetRotations();
  tra = fAlignement->GetTranslation();
  for(Int_t it=0; it<3; it++){
    theta[it]=*(rot+it);
  }
  trU=*tra; trV=*(tra+1); trW=*(tra+2); // trW added, JB 2010/09/08
  
 
  TMinuit* fit = new TMinuit(6);
  //fit->SetFCN(FCNAlignMimosa);
  fit->SetFCN(FCNAlignMimosa);
 
  Double_t arglist[10];
  Int_t ierflg = 0;
  fit->mnhelp();
  
  arglist[0] = 1;
  fit->mnexcm("SET ERR", arglist ,1,ierflg);
  arglist[0] = 2;
  fit->mnexcm("SET STRATEGY ", arglist ,1,ierflg); // JB 2012/09/05
  arglist[0] = -1; // -1 for no output
  fit->mnexcm("SET PRINTOUT ", arglist,1,ierflg); // JB 2013/09/27

  // Set starting values and step sizes for parameters; 
  //static Double_t vstart[9] = {theta[0], theta[1] ,theta[2],trU, trV, trW}; //, 0., 0., 0.}; // JB, removed for inline compilation 
  Double_t vstart[9] = {theta[0], theta[1] ,theta[2], trU, trV, trW}; //, 0., 0., 0.};  
  
  /* old values
     static Double_t step[9] = {1. ,1. , 1. , 200.0,200.0, 200.0, 1.0, 1.0, 1.0};
     static Double_t step[9] = {.1 ,.1 , .1 , 500.0,500.0, 10.0, 1.0, 1.0, 1.0};
  */
  
  static Double_t step[9] = {.1 ,.1 , .1 , 4.0, 4.0, 5.0, 1.0, 1.0, 1.0};
  fit->mnparm(0, "tiltz", vstart[0], step[0], -6.30,6.3,ierflg); // was 0,0 instead of -6.3,6.3 before
  fit->mnparm(1, "tilty", vstart[1], step[1], -6.30,6.30,ierflg);
  fit->mnparm(2, "tiltx", vstart[2], step[2], -6.30,6.30,ierflg);
  fit->mnparm(3, "x", vstart[3], step[3], 0,0,ierflg);
  fit->mnparm(4, "y", vstart[4], step[4], 0,0,ierflg);
  fit->mnparm(5, "z", vstart[5], step[5], 0,0,ierflg);
  
  if(AlignDebug >= 1) 
    { 
      fit->mnparm(6, "a(x,y)", vstart[6], step[6], 0,0,ierflg); // not sure work with TMinuit(6) declaration above
      fit->mnparm(7, "cu", vstart[7], step[7], 0,0,ierflg);
      fit->mnparm(8, "cv", vstart[8], step[8], 0,0,ierflg);
      
      for (Int_t i=1 ; i <=9 ; i++){
        arglist[0] = i;
        fit->mnexcm("fix ", arglist ,1,ierflg);
      }
      
      arglist[0] = 1;
      fit->mnexcm("rel ", arglist ,1,ierflg);
      arglist[0] = 2;
      fit->mnexcm("rel ", arglist ,1,ierflg);
      arglist[0] = 3;
      fit->mnexcm("rel ", arglist ,1,ierflg);
      arglist[0] = 4;
      fit->mnexcm("rel ", arglist ,1,ierflg);
      arglist[0] = 5;
      fit->mnexcm("rel ", arglist ,1,ierflg);
      arglist[0] = 6;
      fit->mnexcm("rel ", arglist ,1,ierflg);
    }

  // Uncomment to fix Z position
  // JB 2011/10/31, param 5 instead of 6 was fixed -> corrected JB 2013/06/22
  arglist[0] = 6;
  fit->mnexcm("fix ", arglist ,1,ierflg);

  // Now ready for minimization step
  // new range definition, JB 2012/05/11
  fDu = new TH1F("du","Delta U",200,-1.1*aDistance,1.1*aDistance);
  fDv = new TH1F("dv","Delta V",200,-1.1*aDistance,1.1*aDistance);
  
  arglist[0] = 500;
  arglist[1] = 1.;
  
  //fit->mnexcm("MIGRAD", arglist ,2,ierflg);
  
  // Print results
  //??  arglist[0] = 8;
  //fit->mnexcm("cali", arglist ,1,ierflg);

  Double_t amin,edm,errdef;
  Int_t nvpar,nparx,icstat;
  fit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
  fit->mnprin(3,amin);

  arglist[0] = 1;
  // fit->mnexcm("scan", arglist ,1,ierflg);
  arglist[0] = 1;
  fit->mnexcm("SHOW PAR", arglist ,0,ierflg); // added, JB 2012/09/05

  int batch_mode =  gROOT->IsBatch() ; //1;
  Char_t commande[250];
  Int_t retour = 0; // removing warning: variable 'retour' should be initialized BH 2013/08/20
 
  if (batch_mode == 0) {
    arglist[0] = 8;       fit->mnexcm("cali", arglist ,1,ierflg);
    fit->mncomd("SHOW PAR",retour); // first display    
    cout << "Enter MINUIT command, type help to get a list:" << endl;  
  }
  while(retour!=11){
    //if (batch_mode == 0)  fit->mnhelp(); // avoid large printout!
    if (batch_mode == 0) cin >> commande;
    for (Int_t i=0 ; i<250 ; i++) if (commande[i]==',') commande[i]=' ' ;
    if (batch_mode == 0)  fit->mncomd(commande,retour);
    if (batch_mode == 1)  fit->mncomd("MINIMIZE",retour);
    arglist[0] = 8;       fit->mnexcm("cali", arglist ,1,ierflg);
    printf("command : %s  --> return code : %d \n\n",commande,retour);
    if (batch_mode == 0) cout << "Enter MINUIT command, type help to get a list:" << endl;  
    if (batch_mode == 1) fit->mncomd("exit",retour);
  }

  /*
  fCnv->Close();
  fCcr->Close();
  if(fDu) {delete fDu; fDu  = 0;}
  if(fDv) {delete fDv; fDv  = 0;}
  AlignPlots.Close(); 
  */

  // ccr->SaveAs("RUNN5981_t2.ps");
  
  Int_t yyy;
  if (batch_mode == 0) cout << " Store the results of alignement? (1/0) " << endl;
  if (batch_mode == 0) cin >> yyy;
  if (batch_mode == 1) yyy = 1; // force storing in batch, JB 2011/06/17
  Int_t err;
  Double_t par[6],erpar[6];
  for(Int_t ip=0; ip<6; ip++){
    err=fit->GetParameter(ip,par[ip],erpar[ip]);
  }

  Info("AlignMimosa","Alignement results...");
    
  fAlignement->SetTranslation(par[3],par[4],par[5]);
  fAlignement->SetRotations(par[0],par[1],par[2]);
  fAlignement->PrintAll();
  //fAlignement->GetDataPoints()->Clear(); // required before storing, JB 2011/06/19

  if(yyy==1)  
    {
      Info("AlignMimosa","Saving now to %s ...", aCorFile->GetName());    
      //if(AlignDebug >= 2) 
      aCorFile->ReOpen("update");
      //if(AlignDebug >= 2)
      fAlignement->ResetDataPoints(); // clean the points so that there are not stored, SS 2012/09/05
      fAlignement->Write("alignement");
      //  aCorFile->Close(); 
    }
  else {aCorFile->ReOpen("update");}

  /*
  if(fCnv) {delete fCnv; fCnv = 0;}
  if(fMyfit) {delete fMyfit; fMyfit = 0;}
  if(fCcr) {delete fCcr; fCcr = 0;}

  fDu = fDv = 0; 
  fCnv = fCcr = 0;
  fMyfit = 0;
  */

  Info("AlignMimosa","Mimosa Alignement Done!");
  
  return fAlignement;
}

//______________________________________________________________________________
//
DPrecAlign* MimosaAlignAnalysis::AlignPrecMimosaLadder(DLadder* myLadder, Bool_t modeAuto, Double_t aDistance)
{
  // Mimosa alignement method
  // Take an input aligment and fit its data to 
  // The present version allows for z!=0 and fit it
  //
  // LC 2012/09/06.
  // Modified: JB, 2013/07/14 name of angles changed from th0/1/2 to tiltx/y/z 
  // Modified: JB, 2013/08/22 aDistance parameter to manage histo range
  
  if(AlignDebug) Info("AlignMimosa","Aligning Mimosa Now...");
  
  //if(!(initAlignment->GetDataPoints()->First())) Fatal("AlignMimosa","No initial alignment found or empty!");
  
  ladderToAlign = myLadder;

  //init
  fAlignement = ladderToAlign->GetPrecAlign();
  ladderToAlign->UpdateAlignment();
  
  // fDu = NULL; 
  // fDv = NULL;
  fDx = NULL; // qyliu: ladder
  fDy = NULL; // qyliu: ladder
  fDz = NULL; // qyliu: ladder
  fCnv = NULL;
  fCcr = NULL;
  fMyfit = 0;
  
  // moved here from FCN, to avoid deletion after procedure, JB 2013/09/19
  fCnv = new TCanvas("Cnv","Residues after alignment",2,10,400,500);
  fCnv->Divide(1,3); // qyliu: changed from (1,2);
  fCnv->Update();

  fCcr = new TCanvas("ccr","Correlations",410,10,500,500);
  fCcr->Divide(3,3); // qyliu: changed from (2,2)
  fCcr->Update();
  
  for (Int_t i=0; i<9 ; i++) { // qyliu: changed from i<4
    fGraph[i].SetPoint(0,0,0);
  }
  
  fDx = new TH1F("dx","Delta X",200,-1.*aDistance,1.*aDistance); // qyliu: ladder
  fDy = new TH1F("dy","Delta Y",200,-1.*aDistance,1.*aDistance); // qyliu: ladder
  fDz = new TH1F("dz","Delta Z",200,-1.*aDistance,1.*aDistance); // qyliu: ladder
  
  if(AlignDebug >= 3) 
  {  
    cout << "Run Number = " <<  fSession->GetRunNumber() <<endl;
    cout << "Plane Number = " << fSession->GetPlaneNumber() <<endl;
    cout << "Initial parameters" << endl;
    fAlignement->PrintAll();
    cout << "Gonna list data " << endl;
    fAlignement->ListDataPoints();
  }
  
  Double_t  *rot = 0;
  Double_t  *tra = 0;
  Double_t  theta[3]={0,0,0}; // JB, removed for inline compilation
  Double_t  trU,trV, trW; // trW added, JB 2010/09/08
  trU=trV=trW=0;
  rot = fAlignement->GetRotations();
  tra = fAlignement->GetTranslation();
  for(Int_t it=0; it<3; it++){
    theta[it]=*(rot+it);
  }
  trU=*tra; trV=*(tra+1); trW=*(tra+2); // trW added, JB 2010/09/08

  TMinuit* fit = new TMinuit(6);
  fit->SetFCN(FCNAlignMimosaLadder);
  //fit->SetFCN(fcn);
  
  Double_t arglist[10];
  Int_t ierflg = 0;
  //fit->mnhelp(); // to limit printout
  
  arglist[0] = 1;
  fit->mnexcm("SET ERR", arglist ,1,ierflg);
  arglist[0] = 2;
  fit->mnexcm("SET STRATEGY ", arglist,1,ierflg); // JB 2013/09/27
  arglist[0] = -1; // -1 for no output
  fit->mnexcm("SET PRINTOUT ", arglist,1,ierflg); // JB 2013/09/27
  
  // Set starting values and step sizes for parameters; 
  // static Double_t vstart[9] = {theta[0], theta[1] ,theta[2],trU, trV, trW}; //, 0., 0., 0.}; // JB, removed for inline compilation 
  Double_t vstart[9] = {theta[0], theta[1] ,theta[2], trU, trV, trW}; //, 0., 0., 0.};  
  
  static Double_t step[9] = {.01 ,.01 , .01 , 5.0, 5.0, 5.0, 1.0, 1.0, 1.0};
  fit->mnparm(0, "tiltz", vstart[0], step[0], 0., 0, ierflg); // was 0,0 instead of -6.3,6.3 before
  fit->mnparm(1, "tilty", vstart[1], step[1], 0., 0, ierflg);
  fit->mnparm(2, "tiltx", vstart[2], step[2], 0., 0, ierflg);
  fit->mnparm(3, "x", vstart[3], step[3], 0,0,ierflg);
  fit->mnparm(4, "y", vstart[4], step[4], 0,0,ierflg);
  fit->mnparm(5, "z", vstart[5], step[5], 0,0,ierflg);
  
  
  if(AlignDebug >= 1) 
  { 
    fit->mnparm(6, "a(x,y)", vstart[6], step[6], 0,0,ierflg); // not sure work with TMinuit(6) declaration above
    fit->mnparm(7, "cu", vstart[7], step[7], 0,0,ierflg);
    fit->mnparm(8, "cv", vstart[8], step[8], 0,0,ierflg);
    
    for (Int_t i=1 ; i <=9 ; i++){
      arglist[0] = i;
      fit->mnexcm("fix ", arglist ,1,ierflg);
    }
    
    arglist[0] = 1;
    fit->mnexcm("rel ", arglist ,1,ierflg);
    arglist[0] = 2;
    fit->mnexcm("rel ", arglist ,1,ierflg);
    arglist[0] = 3;
    fit->mnexcm("rel ", arglist ,1,ierflg);
    arglist[0] = 4;
    fit->mnexcm("rel ", arglist ,1,ierflg);
    arglist[0] = 5;
    fit->mnexcm("rel ", arglist ,1,ierflg);
    arglist[0] = 6;
    fit->mnexcm("rel ", arglist ,1,ierflg);
  }
 /* 
  // Uncomment to fix Z position
  arglist[0] = 6;
  //fit->mnexcm("fix ", arglist ,1,ierflg);
  arglist[1] = trW-5000.;
  arglist[2] = trW+5000.;
  fit->mnexcm("set limit ", arglist ,3,ierflg);
 */
  // Now ready for minimization step
  // Decide the range from the bound, JB 2013/08/22
  //fDu = new TH1F("du","Delta U",200,-300,300); // previously 100,-50,50
  //fDv = new TH1F("dv","Delta V",200,-300,300);
  //fDu = new TH1F("du","Delta U",200,-1.*aDistance,1.*aDistance);
  //fDv = new TH1F("dv","Delta V",200,-1.*aDistance,1.*aDistance);
  
  arglist[0] = 500000;
  arglist[1] = 1.; 
  //fit->mnexcm("MIGRAD", arglist ,2,ierflg);
  
  Double_t amin,edm,errdef;
  Int_t nvpar,nparx,icstat;
  fit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
  fit->mnprin(3,amin);
  
  arglist[0] = 1;
  
  int batch_mode =  gROOT->IsBatch() ;
  if (modeAuto==0 || modeAuto==1) batch_mode = modeAuto; // batch_mode = 1 Minuit auto | batch_mode = 0 Minuit by hand.
  else batch_mode = 0;
  
  Char_t commande[250];
  Int_t retour = 0; // removing warning: variable 'retour' should be initialized BH 2013/08/20
  
  if (batch_mode == 0) {
    arglist[0] = 8;       fit->mnexcm("cali", arglist ,1,ierflg);
    fit->mncomd("SHOW PAR",retour); // first display    
    cout << "Enter MINUIT command, type help to get a list:" << endl;  
  }
  while(retour!=11){
    //if (batch_mode == 0)  fit->mnhelp(); // avoid large printout!
    if (batch_mode == 0) cin >> commande;
    for (Int_t i=0 ; i<250 ; i++) if (commande[i]==',') commande[i]=' ' ;
    if (batch_mode == 0)  fit->mncomd(commande,retour);
    if (batch_mode == 1)  fit->mncomd("MINIMIZE",retour);
    arglist[0] = 8;       fit->mnexcm("cali", arglist ,1,ierflg);
    printf("command : %s  --> return code : %d \n\n",commande,retour);
    if (batch_mode == 0) cout << "Enter MINUIT command, type help to get a list:" << endl;  
    if (batch_mode == 1) fit->mncomd("exit",retour);
  }

  fCnv->Close();
  fCcr->Close();
  // if(fDu) {delete fDu; fDu  = NULL;}
  // if(fDv) {delete fDv; fDv  = NULL;}
  if(fDx) {delete fDx; fDx  = NULL;} // qyliu:ladder
  if(fDy) {delete fDy; fDy  = NULL;} // qyliu:ladder
  if(fDz) {delete fDz; fDz  = NULL;} // qyliu:ladder
  //AlignPlots.Close(); 
  
  /* Useless so far, JB 2013/07/14
   Int_t yyy;
   if (batch_mode == 0) cout << " Store the results of alignement? (1/0) " << endl;
   if (batch_mode == 0) cin >> yyy;
   if (batch_mode == 1) yyy = 1; // force storing in batch, JB 2011/06/17
   */
  
  Int_t err;
  Double_t par[6],erpar[6];
  for(Int_t ip=0; ip<6; ip++){
    err=fit->GetParameter(ip,par[ip],erpar[ip]);
  }
  
  Info("AlignMimosa","Alignement results...");
  
  //std::cout<<"test rgfez"<<std::endl;
  
  fAlignement->SetTranslation(par[3],par[4],par[5]);
  fAlignement->SetRotations(par[0],par[1],par[2]);
  fAlignement->PrintAll();
  //fAlignement->GetDataPoints()->Clear(); // required before storing, JB 2011/06/19
  
  //fAlignement->ResetDataPoints(); //SS 2012/01/20
  
  if(fCnv) {delete fCnv; fCnv = NULL;}
  delete fMyfit;  
  fMyfit = NULL;
  if(fCcr) {delete fCcr; fCcr = NULL;}
  
  /*  
  fDu = fDv = 0; 
  fCnv = fCcr = 0;
  fMyfit = 0;
  */
  Info("AlignMimosa","Mimosa Alignement Done!");
  
  return fAlignement;
}


//______________________________________________________________________________
//

DPrecAlign* MimosaAlignAnalysis::AlignPrecMimosaLadder2(DLadder* myLadder, Bool_t modeAuto, Double_t aDistance)
{
  // Mimosa alignement method
  // Take an input aligment and fit its data to 
  // The present version allows for z!=0 and fit it
  //
  // LC 2012/09/06.
  // Modified: JB, 2013/07/14 name of angles changed from th0/1/2 to tiltx/y/z 
  // Modified: JB, 2013/08/22 aDistance parameter to manage histo range
  
  if(AlignDebug) Info("AlignMimosa","Aligning Mimosa Now...");
  
  //if(!(initAlignment->GetDataPoints()->First())) Fatal("AlignMimosa","No initial alignment found or empty!");
  
  ladderToAlign = myLadder;

  //init
  fAlignement = ladderToAlign->GetPrecAlign();
  ladderToAlign->UpdateAlignment();
  
  // fDu = NULL; 
  // fDv = NULL;
  fDx = NULL; // qyliu: ladder
  fDy = NULL; // qyliu: ladder
  fDz = NULL; // qyliu: ladder
  fCnv = NULL;
  fCcr = NULL;
  fMyfit = 0;
  
  // moved here from FCN, to avoid deletion after procedure, JB 2013/09/19
  fCnv = new TCanvas("Cnv","Residues after alignment",2,10,400,500);
  fCnv->Divide(1,3); // qyliu: changed from (1,2);
  fCnv->Update();

  fCcr = new TCanvas("ccr","Correlations",410,10,500,500);
  fCcr->Divide(3,3); // qyliu: changed from (2,2)
  fCcr->Update();
  
  for (Int_t i=0; i<9 ; i++) { // qyliu: changed from i<4
    fGraph[i].SetPoint(0,0,0);
  }
  
  fDx = new TH1F("dx","Delta X",200,-1.*aDistance,1.*aDistance); // qyliu: ladder
  fDy = new TH1F("dy","Delta Y",200,-1.*aDistance,1.*aDistance); // qyliu: ladder
  fDz = new TH1F("dz","Delta Z",200,-1.*aDistance,1.*aDistance); // qyliu: ladder
  
  if(AlignDebug >= 3) 
  {  
    cout << "Run Number = " <<  fSession->GetRunNumber() <<endl;
    cout << "Plane Number = " << fSession->GetPlaneNumber() <<endl;
    cout << "Initial parameters" << endl;
    fAlignement->PrintAll();
    cout << "Gonna list data " << endl;
    fAlignement->ListDataPoints();
  }
  
  Double_t  *rot = 0;
  Double_t  *tra = 0;
  Double_t  theta[3]={0,0,0}; // JB, removed for inline compilation
  Double_t  trU,trV, trW; // trW added, JB 2010/09/08
  trU=trV=trW=0;
  rot = fAlignement->GetRotations();
  tra = fAlignement->GetTranslation();
  for(Int_t it=0; it<3; it++){
    theta[it]=*(rot+it);
  }
  trU=*tra; trV=*(tra+1); trW=*(tra+2); // trW added, JB 2010/09/08

  // Get the alignment parameters in the local frmae of the ladder. 
  std::map<Int_t, std::vector<Double_t> > localPlanesParameters; 
  
  for( Int_t planeIndex=ladderToAlign->GetFirstPlane() ; planeIndex<ladderToAlign->GetFirstPlane()+ladderToAlign->GetNumberOfPlanes() ; ++planeIndex ) {

    DPrecAlign* localPlaneDPrecAlign = ladderToAlign->GetPlane(planeIndex)->GetPrecAlignment();
    DR3 translations = localPlaneDPrecAlign->GetTranslationsDR3();
    DR3 rotations    = localPlaneDPrecAlign->GetRotationsDR3();    
   
    std::vector<Double_t> initialLocalPlaneParameters;
    
   for(Int_t i=0 ; i<3 ; ++i) {
      initialLocalPlaneParameters.push_back( rotations(i) );      
    }
 
    for(Int_t i=0 ; i<3 ; ++i) {
      initialLocalPlaneParameters.push_back( translations(i) );      
    }

    localPlanesParameters[planeIndex] = initialLocalPlaneParameters;
 
  }

  TMinuit* fit = new TMinuit(30);
  fit->SetFCN(FCNAlignMimosaLadder2);
  //fit->SetFCN(fcn);
  
  Double_t arglist[10];
  Int_t ierflg = 0;
  //fit->mnhelp(); // to limit printout
  
  arglist[0] = 1;
  fit->mnexcm("SET ERR", arglist ,1,ierflg);
  arglist[0] = 2;
  fit->mnexcm("SET STRATEGY ", arglist,1,ierflg); // JB 2013/09/27
  arglist[0] = -1; // -1 for no output
  fit->mnexcm("SET PRINTOUT ", arglist,1,ierflg); // JB 2013/09/27
  
  // Set starting values and step sizes for parameters; 
  // static Double_t vstart[9] = {theta[0], theta[1] ,theta[2],trU, trV, trW}; //, 0., 0., 0.}; // JB, removed for inline compilation 
  Double_t vstart[30];
  vstart[0] = theta[0];
  vstart[1] = theta[1];
  vstart[2] = theta[2];
  vstart[3] = trU;
  vstart[4] = trV;
  vstart[5] = trW;  
  
  std::map<Int_t, std::vector<Double_t> >::iterator planeParams = localPlanesParameters.begin(); 

  for( ; planeParams!=localPlanesParameters.end() ; ++planeParams ) {
    
    Int_t planeNumber = planeParams->first-ladderToAlign->GetFirstPlane(); // = 0->3
    std::vector<Double_t>& alignPlaneParams = planeParams->second;
    
    for(Int_t j=0 ; j<6 ; ++j) {
      cout<<alignPlaneParams[j]<<std::endl;
      vstart[(planeNumber+1)*6+j] = alignPlaneParams[j]; // 1*6 -> 30
    }
  }
  
  Double_t limitTilts = 0.04;
  Double_t limitTranslation = 50;

  static Double_t step[30] = {.01 ,.01 , .01 , 5.0, 5.0, 5.0, .01 ,.01 , .01 , 5.0, 5.0, 5.0, .01 ,.01 , .01 , 5.0, 5.0, 5.0, .01 ,.01 , .01 , 5.0, 5.0, 5.0, .01 ,.01 , .01 , 5.0, 5.0, 5.0};
  fit->mnparm(0, "tiltz", vstart[0], step[0], vstart[0]-limitTilts, vstart[0]+limitTilts, ierflg); // was 0,0 instead of -6.3,6.3 before
  fit->mnparm(1, "tilty", vstart[1], step[1], vstart[1]-limitTilts, vstart[1]+limitTilts, ierflg);
  fit->mnparm(2, "tiltx", vstart[2], step[2], vstart[2]-limitTilts, vstart[2]+limitTilts, ierflg);
  fit->mnparm(3, "x", vstart[3], step[3], vstart[3]-limitTranslation, vstart[3]+limitTranslation,ierflg);
  fit->mnparm(4, "y", vstart[4], step[4], vstart[4]-limitTranslation, vstart[4]+limitTranslation,ierflg);
  fit->mnparm(5, "z", vstart[5], step[5], vstart[5]-limitTranslation, vstart[5]+limitTranslation,ierflg);
  fit->mnparm(6, "tiltw1", vstart[6], step[6], vstart[6]-limitTilts, vstart[6]+limitTilts, ierflg); // was 0,0 instead of -6.3,6.3 before
  fit->mnparm(7, "tiltv1", vstart[7], step[7], vstart[7]-limitTilts, vstart[7]+limitTilts, ierflg);
  fit->mnparm(8, "tiltu1", vstart[8], step[8], vstart[8]-limitTilts, vstart[8]+limitTilts, ierflg);
  fit->mnparm(9, "u1", vstart[9], step[3], vstart[9]-limitTranslation, vstart[9]+limitTranslation,ierflg);
  fit->mnparm(10, "v1", vstart[10], step[10], vstart[10]-limitTranslation,vstart[10]+limitTranslation,ierflg);
  fit->mnparm(11, "w1", vstart[11], step[11], vstart[11]-limitTranslation,vstart[11]+limitTranslation,ierflg);
  fit->mnparm(12, "tiltw2", vstart[12], step[12], vstart[12]-limitTilts, vstart[12]+limitTilts, ierflg); // was 0,0 instead of -6.3,6.3 before
  fit->mnparm(13, "tiltv2", vstart[13], step[13], vstart[13]-limitTilts, vstart[13]+limitTilts, ierflg);
  fit->mnparm(14, "tiltu2", vstart[14], step[14], vstart[14]-limitTilts, vstart[14]+limitTilts, ierflg);
  fit->mnparm(15, "u2", vstart[15], step[15], vstart[15]-limitTranslation,vstart[15]+limitTranslation,ierflg);
  fit->mnparm(16, "v2", vstart[16], step[16], vstart[16]-limitTranslation,vstart[16]+limitTranslation,ierflg);
  fit->mnparm(17, "w2", vstart[17], step[17], vstart[17]-limitTranslation,vstart[17]+limitTranslation,ierflg);
  fit->mnparm(18, "tiltw3", vstart[18], step[18], vstart[18]-limitTilts, vstart[18]+limitTilts, ierflg); // was 0,0 instead of -6.3,6.3 before
  fit->mnparm(19, "tiltv3", vstart[19], step[19], vstart[19]-limitTilts, vstart[19]+limitTilts, ierflg);
  fit->mnparm(20, "tiltu3", vstart[20], step[20], vstart[20]-limitTilts, vstart[20]+limitTilts, ierflg);
  fit->mnparm(21, "u3", vstart[21], step[21], vstart[21]-limitTranslation,vstart[21]+limitTranslation,ierflg);
  fit->mnparm(22, "v3", vstart[22], step[22], vstart[22]-limitTranslation,vstart[22]+limitTranslation,ierflg);
  fit->mnparm(23, "w3", vstart[23], step[23], vstart[23]-limitTranslation,vstart[23]+limitTranslation,ierflg);
  fit->mnparm(24, "tiltw4", vstart[24], step[24], vstart[24]-limitTilts, vstart[24]+limitTilts, ierflg); // was 0,0 instead of -6.3,6.3 before
  fit->mnparm(25, "tiltv4", vstart[25], step[25], vstart[25]-limitTilts, vstart[25]+limitTilts, ierflg);
  fit->mnparm(26, "tiltu4", vstart[26], step[26], vstart[26]-limitTilts, vstart[26]+limitTilts, ierflg);
  fit->mnparm(27, "u4", vstart[27], step[27], vstart[27]-limitTranslation,vstart[27]+limitTranslation,ierflg);
  fit->mnparm(28, "v4", vstart[28], step[28], vstart[28]-limitTranslation,vstart[28]+limitTranslation,ierflg);
  fit->mnparm(29, "w4", vstart[29], step[29], vstart[29]-limitTranslation,vstart[29]+limitTranslation,ierflg);

/* 
  if(AlignDebug >= 1) 
  { 
    fit->mnparm(6, "a(x,y)", vstart[6], step[6], 0,0,ierflg); // not sure work with TMinuit(6) declaration above
    fit->mnparm(7, "cu", vstart[7], step[7], 0,0,ierflg);
    fit->mnparm(8, "cv", vstart[8], step[8], 0,0,ierflg);
    
    for (Int_t i=1 ; i <=9 ; i++){
      arglist[0] = i;
      fit->mnexcm("fix ", arglist ,1,ierflg);
    }
    
    arglist[0] = 1;
    fit->mnexcm("rel ", arglist ,1,ierflg);
    arglist[0] = 2;
    fit->mnexcm("rel ", arglist ,1,ierflg);
    arglist[0] = 3;
    fit->mnexcm("rel ", arglist ,1,ierflg);
    arglist[0] = 4;
    fit->mnexcm("rel ", arglist ,1,ierflg);
    arglist[0] = 5;
    fit->mnexcm("rel ", arglist ,1,ierflg);
    arglist[0] = 6;
    fit->mnexcm("rel ", arglist ,1,ierflg);
  }
*/ 
  // Uncomment to fix Z position
  arglist[0] = 4;
  fit->mnexcm("fix ", arglist ,1, ierflg);
  arglist[0] = 5;
  fit->mnexcm("fix ", arglist ,1, ierflg);
  arglist[0] = 6;
  fit->mnexcm("fix ", arglist ,1, ierflg);

  arglist[0] = 12;
  fit->mnexcm("fix ", arglist ,1, ierflg);
 
  arglist[0] = 18;
  fit->mnexcm("fix ", arglist ,1, ierflg);

  arglist[0] = 24;
  fit->mnexcm("fix ", arglist ,1, ierflg);

  arglist[0] = 30;
  fit->mnexcm("fix ", arglist ,1, ierflg);

/*  
  arglist[1] = trW-5000.;
  arglist[2] = trW+5000.;
  fit->mnexcm("set limit ", arglist ,3,ierflg);
*/

  // Now ready for minimization step
  // Decide the range from the bound, JB 2013/08/22
  //fDu = new TH1F("du","Delta U",200,-300,300); // previously 100,-50,50
  //fDv = new TH1F("dv","Delta V",200,-300,300);
  //fDu = new TH1F("du","Delta U",200,-1.*aDistance,1.*aDistance);
  //fDv = new TH1F("dv","Delta V",200,-1.*aDistance,1.*aDistance);
  
  arglist[0] = 500000;
  arglist[1] = 1.; 
  //fit->mnexcm("MIGRAD", arglist ,2,ierflg);
  
  Double_t amin,edm,errdef;
  Int_t nvpar,nparx,icstat;
  fit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
  fit->mnprin(3,amin);
  
  arglist[0] = 1;
  
  int batch_mode =  gROOT->IsBatch() ;
  if (modeAuto==0 || modeAuto==1) batch_mode = modeAuto; // batch_mode = 1 Minuit auto | batch_mode = 0 Minuit by hand.
  else batch_mode = 0;
  
  Char_t commande[250];
  Int_t retour = 0; // removing warning: variable 'retour' should be initialized BH 2013/08/20
  
  if (batch_mode == 0) {
    arglist[0] = 8;       fit->mnexcm("cali", arglist ,1,ierflg);
    fit->mncomd("SHOW PAR",retour); // first display    
    cout << "Enter MINUIT command, type help to get a list:" << endl;  
  }
  while(retour!=11){
    //if (batch_mode == 0)  fit->mnhelp(); // avoid large printout!
    if (batch_mode == 0) cin >> commande;
    for (Int_t i=0 ; i<250 ; i++) if (commande[i]==',') commande[i]=' ' ;
    if (batch_mode == 0)  fit->mncomd(commande,retour);
    if (batch_mode == 1)  fit->mncomd("MINIMIZE",retour);
    arglist[0] = 8;       fit->mnexcm("cali", arglist ,1,ierflg);
    printf("command : %s  --> return code : %d \n\n",commande,retour);
    if (batch_mode == 0) cout << "Enter MINUIT command, type help to get a list:" << endl;  
    if (batch_mode == 1) fit->mncomd("exit",retour);
  }

  fCnv->Close();
  fCcr->Close();
  // if(fDu) {delete fDu; fDu  = NULL;}
  // if(fDv) {delete fDv; fDv  = NULL;}
  if(fDx) {delete fDx; fDx  = NULL;} // qyliu:ladder
  if(fDy) {delete fDy; fDy  = NULL;} // qyliu:ladder
  if(fDz) {delete fDz; fDz  = NULL;} // qyliu:ladder
  //AlignPlots.Close(); 
  
  /* Useless so far, JB 2013/07/14
   Int_t yyy;
   if (batch_mode == 0) cout << " Store the results of alignement? (1/0) " << endl;
   if (batch_mode == 0) cin >> yyy;
   if (batch_mode == 1) yyy = 1; // force storing in batch, JB 2011/06/17
   */
  
  Int_t err;
  Double_t par[30],erpar[30];
  for(Int_t ip=0; ip<30; ip++){
    err=fit->GetParameter(ip,par[ip],erpar[ip]);
  }
  
  Info("AlignMimosa","Alignement results...");
  
  //std::cout<<"test rgfez"<<std::endl;
  
  fAlignement->SetTranslation(par[3],par[4],par[5]);
  fAlignement->SetRotations(par[0],par[1],par[2]);
  fAlignement->PrintAll();
  
  for( Int_t planeIndex=ladderToAlign->GetFirstPlane() ; planeIndex<ladderToAlign->GetFirstPlane()+ladderToAlign->GetNumberOfPlanes() ; ++planeIndex ) {

    DPrecAlign* localPlaneDPrecAlign = ladderToAlign->GetPlane(planeIndex)->GetPrecAlignment();
    Int_t k = (planeIndex-ladderToAlign->GetFirstPlane()+1)*6;

    localPlaneDPrecAlign->SetTranslation(par[k+3], par[k+4], par[k+5]);
    localPlaneDPrecAlign->SetRotations(par[k], par[k+1], par[k+2]);    
  
    localPlaneDPrecAlign->PrintAll(); 
  } 
  //fAlignement->GetDataPoints()->Clear(); // required before storing, JB 2011/06/19
  
  //fAlignement->ResetDataPoints(); //SS 2012/01/20
  
  if(fCnv) {delete fCnv; fCnv = NULL;}
  delete fMyfit;  
  fMyfit = NULL;
  if(fCcr) {delete fCcr; fCcr = NULL;}
  
  /*  
  fDu = fDv = 0; 
  fCnv = fCcr = 0;
  fMyfit = 0;
  */
  Info("AlignMimosa","Mimosa Alignement Done!");
  
  return fAlignement;
}

//______________________________________________________________________________
//
Int_t MimosaAlignAnalysis::PrintMinimizationChoice()
{
  Int_t choice = 0;

  std::cout<<"##########################################################"<<std::endl;
  std::cout<<"################# Minimisation tool ######################"<<std::endl;
  std::cout<<"##### Choose your minimizer and minimization method ######"<<std::endl;
  std::cout<<"  Minuit2                 ||   GSL Minimizer              "<<std::endl;
  std::cout<<"   --> Combined (0)       ||    --> ConjugateFR      (5)  "<<std::endl;
  std::cout<<"   --> Migrad   (1)       ||    --> ConjugateFR      (6)  "<<std::endl;
  std::cout<<"   --> Simplex  (2)       ||    --> VectorBFGS       (7)  "<<std::endl;
  std::cout<<"   --> Fumili2  (3)       ||    --> VectorBFGS2      (8)  "<<std::endl;
  std::cout<<"   --> Scan     (4)       ||    --> SteepestDescent  (9)  "<<std::endl;
  std::cout<<"##########################################################"<<std::endl;
  
  while (!(cin >> choice) && !(choice==0 || choice==1 || choice==2 || choice==3 ||
  choice==4 || choice==5 || choice==6 || choice==7 || choice==8 || choice==9) ) // as long as the number entered is not an int or in the preselected values.
  {

    std::cout<<"##########################################################"<<std::endl;
    std::cout<<"################# Minimisation tool ######################"<<std::endl;
    std::cout<<"##### Choose your minimizer and minimization method ######"<<std::endl;
    std::cout<<"  Minuit2                 ||   GSL Minimizer              "<<std::endl;
    std::cout<<"   --> Combined (0)       ||    --> ConjugateFR      (5)  "<<std::endl;
    std::cout<<"   --> Migrad   (1)       ||    --> ConjugateFR      (6)  "<<std::endl;
    std::cout<<"   --> Simplex  (2)       ||    --> VectorBFGS       (7)  "<<std::endl;
    std::cout<<"   --> Fumili   (3)       ||    --> VectorBFGS2      (8)  "<<std::endl;
    std::cout<<"   --> Scan     (4)       ||    --> SteepestDescent  (9)  "<<std::endl;
    std::cout<<"##########################################################"<<std::endl;
  
    cin.clear();//clear input buffer
  }
  
  return choice;
}

//______________________________________________________________________________
//
std::string MimosaAlignAnalysis::PrintMenu()
{ 
  std::string chain= "";

  std::cout<<"##########################################################"<<std::endl;
  std::cout<<"############### Minimisation Tool Menu ###################"<<std::endl;
  std::cout<<"##                                                      ##"<<std::endl;
  std::cout<<"##   Choose your minimizer ("<<algoName<<")    [\"choose\"]   ##"<<std::endl;
  std::cout<<"##   View Or Set Parameter Values        [\"show\"]     ##"<<std::endl;
  std::cout<<"##   Minimize()                          [\"mini\"]     ##"<<std::endl;
  std::cout<<"##   PrintLevel ("<<printLevel<<")                     [\"level\"]    ##"<<std::endl;
  std::cout<<"##   Exit                                [\"exit\"]     ##"<<std::endl;
  std::cout<<"##                                                      ##"<<std::endl;
  std::cout<<"##########################################################"<<std::endl;

  while (!(cin >> chain) && !(chain.compare("choose")==0 || chain.compare("mini")==0 || chain.compare("show")==0 || chain.compare("level")==0 || chain.compare("exit")==0 ) )
  {

    std::cout<<"##########################################################"<<std::endl;
    std::cout<<"############### Minimisation Tool Menu ###################"<<std::endl;
    std::cout<<"##                                                      ##"<<std::endl;
    std::cout<<"##   Choose your minimizer ("<<algoName<<")    [\"choose\"]   ##"<<std::endl;
    std::cout<<"##   View Or Set Parameter Values        [\"show\"]     ##"<<std::endl;
    std::cout<<"##   Minimize()                          [\"mini\"]     ##"<<std::endl;
    std::cout<<"##   PrintLevel ("<<printLevel<<")                     [\"level\"]    ##"<<std::endl;
    std::cout<<"##   Exit                                [\"exit\"]     ##"<<std::endl;
    std::cout<<"##                                                      ##"<<std::endl;
    std::cout<<"##########################################################"<<std::endl;

    cin.clear();
  }
  
  return chain;
}

//______________________________________________________________________________
//
Int_t MimosaAlignAnalysis::SetPrintLevel()
{ 
  Int_t print = 0;

  std::cout<<"##########################################################"<<std::endl;
  std::cout<<"############### Minimisation Tool Menu ###################"<<std::endl;
  std::cout<<"##                                                      ##"<<std::endl;
  std::cout<<"##   Set the PrintLevel ("<<printLevel<<") [0] [1] [2]             ##"<<std::endl;
  std::cout<<"##                                                      ##"<<std::endl;
  std::cout<<"##########################################################"<<std::endl;

  while( !(cin>>print) || print<0 || print>2 )
  {

    std::cout<<"##########################################################"<<std::endl;
    std::cout<<"############### Minimisation Tool Menu ###################"<<std::endl;
    std::cout<<"##                                                      ##"<<std::endl;
    std::cout<<"##   Set the PrintLevel ("<<printLevel<<") [0] [1] [2]             ##"<<std::endl;
    std::cout<<"##                                                      ##"<<std::endl;
    std::cout<<"##########################################################"<<std::endl;
 
    cin.clear();
  }
  
  //printLevel = print;
  return print;
}

//______________________________________________________________________________
//
void MimosaAlignAnalysis::SplitString(const string& s, char c, vector<string>& v) 
{
  // To Split a string in function of a delimiter and save it in a vector.
  string::size_type i = 0;
  string::size_type j = s.find(c);

  while (j != string::npos) {
    v.push_back(s.substr(i, j-i));
    i = ++j;
    j = s.find(c, j);
    if (j == string::npos) v.push_back(s.substr(i, s.length()));
  } //end while  
}
 
//______________________________________________________________________________
//
std::string MimosaAlignAnalysis::ShowAndSetParameters()
{

  Bool_t validLine  = 0;
  Int_t indexParam  = -1;
  Double_t valueParam = 0;
  std::string chain = "";

  std::vector<Double_t> par;
  par.push_back(fAlignement->GetRotations()[0] );
  par.push_back(fAlignement->GetRotations()[1] );
  par.push_back(fAlignement->GetRotations()[2] );
  par.push_back(fAlignement->GetTranslation()[0] );
  par.push_back(fAlignement->GetTranslation()[1] );
  par.push_back(fAlignement->GetTranslation()[2] );

  std::cout<<"##########################################################"<<std::endl;
  std::cout<<"############### Show And Set Parameters ##################"<<std::endl;
  std::cout<<"##                                                      ##"<<std::endl;
  std::cout<<"##   [1] tiltZ =  "<<par[0]<<" rad                                  ##"<<std::endl;
  std::cout<<"##   [2] tiltY =  "<<par[1]<<" rad                                  ##"<<std::endl;
  std::cout<<"##   [3] tiltX =  "<<par[2]<<" rad                                  ##"<<std::endl;
  std::cout<<"##   [4]   X   =  "<<par[3]<<" um                                   ##"<<std::endl;
  std::cout<<"##   [5]   Y   =  "<<par[4]<<" um                                   ##"<<std::endl;
  std::cout<<"##   [6]   Z   =  "<<par[5]<<" um                                   ##"<<std::endl;
  std::cout<<"##                                                      ##"<<std::endl;
  std::cout<<"##  Set Value To Parameter [x] type : set,par,x,value   ##"<<std::endl;
  std::cout<<"##  Minimize() : type [\"mini\"]                          ##"<<std::endl;
  std::cout<<"##  Quit       : type [\"exit\"]                          ##"<<std::endl;
  std::cout<<"##########################################################"<<std::endl;

  cin.clear();
  cin.ignore();
   
  while( !(chain.compare("exit")==0 || chain.compare("mini")==0 ) )
  {
    
    // Parser :)
    getline(cin,chain);

    if(chain.compare(0,4,"exit")==0 || chain.compare(0,4,"mini")==0 ) break;

    std::vector<std::string> input;
    SplitString(chain, ',' , input);  // delimiter = coma ','

    //for(size_t i=0; i<input.size() ; ++i) std::cout<<input[i]<<std::endl;  

    if(input.size()!=4) {    
      validLine = 0;
      std::cout<<"Wrong entry. Retry."<<std::endl;
      continue;
    }

    if( input[0].compare("set")==0 ) validLine = 1;
    else validLine = 0;
    if( input[1].compare("par")==0 && validLine==1 ) validLine = 1;
    else validLine = 0;
    if( StringToNumber<int>(input[2]) && validLine==1 && StringToNumber<int>(input[2])>0 && StringToNumber<int>(input[2])<7 ) { 
        indexParam = StringToNumber<int>(input[2]);
        validLine = 1;
    }
    else validLine = 0;
    if(StringToNumber<double>(input[3]) && validLine==1) { 
      valueParam = StringToNumber<double>(input[3]);
      validLine = 1;
    }
    else validLine = 0;

    if(validLine==0) std::cout<<"Wrong entry. Retry."<<std::endl;
    if(validLine==0 || validLine==1) {   
      
      if(validLine==1) {
      
        par[indexParam-1] = valueParam;
  
        fAlignement->SetRotations( par[0], par[1], par[2] );
        fAlignement->SetTranslation( par[3], par[4], par[5] ); 
      }

      std::cout<<"##########################################################"<<std::endl;
      std::cout<<"############### Show And Set Parameters ##################"<<std::endl;
      std::cout<<"##                                                      ##"<<std::endl;
      std::cout<<"##   [1] tiltZ =  "<<par[0]<<" rad                                  ##"<<std::endl;
      std::cout<<"##   [2] tiltY =  "<<par[1]<<" rad                                  ##"<<std::endl;
      std::cout<<"##   [3] tiltX =  "<<par[2]<<" rad                                  ##"<<std::endl;
      std::cout<<"##   [4]   X   =  "<<par[3]<<" um                                   ##"<<std::endl;
      std::cout<<"##   [5]   Y   =  "<<par[4]<<" um                                   ##"<<std::endl;
      std::cout<<"##   [6]   Z   =  "<<par[5]<<" um                                   ##"<<std::endl;
      std::cout<<"##                                                      ##"<<std::endl;
      std::cout<<"##  Set Value To Parameter [x] type : set,par,x,value   ##"<<std::endl;
      std::cout<<"##  MiniMize() : type [\"mini\"]                          ##"<<std::endl;
      std::cout<<"##  Quit       : type [\"exit\"]                          ##"<<std::endl;
      std::cout<<"##########################################################"<<std::endl;
    }

    //cin.ignore();
    cin.clear();

 } // end while

  return chain;
}

//______________________________________________________________________________
//
void MimosaAlignAnalysis::FitInterface(Bool_t modeAuto,const  Double_t& aDistance, const Double_t& aSlopeMax)
{
  // Ici on cree une interface pour minimiser avec minuit2 :)
  // une fonction qui calcule le chi2
  // une fonction qui affiche
  // une boucle pour gerer les parametres.
  // une fonction qui affiche les parametres
  // une fonction pour modifier les parametres
  // une fonction qui passe le type de minimisation
  // ... etc ...

  // The different option are :
  // 
  // Minuit/Minuit2 : Choose method upon creation between:
  // kMigrad, kSimplex, kCombined, 
  // kScan, kFumili
  //
  // GSL minimizer : 
  // ROOT::Math::GSLMinimizer min( ROOT::Math::kSteepestDescent );
  // ROOT::Math::GSLSimAnMinimizer min;  
  // Choose method upon creation between:
  // kConjugateFR, kConjugatePR, kVectorBFGS,
  // kVectorBFGS2, kSteepestDescent
  // ROOT::Math::GSLMinimizer min( ROOT::Math::kSteepestDescent );
  // ROOT::Math::GSLSimAnMinimizer min;
  // 
  // Other choice :
  //  GSLMultiFit
  //  GSLSimAn
  //  Genetic

  std::cout<<"Welcome in Fit Interface"<<std::endl; 

  std::map<Int_t, TString> minimizerChoice;
  minimizerChoice[0] = "Minuit2";
  minimizerChoice[1] = "Minuit2";
  minimizerChoice[2] = "Minuit2";
  minimizerChoice[3] = "Minuit2";
  minimizerChoice[4] = "Minuit2";
  minimizerChoice[5] = "GSLMultiMin";
  minimizerChoice[6] = "GSLMultiMin";
  minimizerChoice[7] = "GSLMultiMin";
  minimizerChoice[8] = "GSLMultiMin";
  minimizerChoice[9] = "GSLMultiMin";

  std::map<Int_t, TString> algoChoice; 
  algoChoice[0] = "kCombined";
  algoChoice[1] = "kMigrad";
  algoChoice[2] = "kSimplex";
  algoChoice[3] = "kFumili2";
  algoChoice[4] = "kScan";
  algoChoice[5] = "kConjugateFR";
  algoChoice[6] = "kConjugatePR";
  algoChoice[7] = "kVectorBFGS";
  algoChoice[8] = "kVectorBFGS2";
  algoChoice[9] = "kSteepestDescent";
 
  // Minimizer by default :
  minimizerName = "Minuit2"; // "Minuit"
  //minimizerName = "GSLMultiMin";
  //algoName      = "kCombined";
  algoName      =  "kCombined";

  printResiduals = 0;
  
  Int_t   maxCall       = 100000;
  Int_t   maxIter       = 100000;
  Double_t   tolerance  = 1.e-3;  // Not used in the fit.
          printLevel    = 2;
  Int_t   strategy      = 2;
 
  fAlignement = fLadders[1]->GetPrecAlign();
  
  //fAlignement->SetInitialRotations();
  //fAlignement->SetInitialPosition(); 

  // Intractive mode. With display :
  if(modeAuto==0) {
    fDu = fDv = fDw = fDdu = fDdv = fDdw = fChiSquareX = fChiSquareY = fChiSquareZ = fChiSquareTiltX = fChiSquareTiltY = 0; 
    fCnv = fCcr = fCanvasChiSquare = 0;
 
    fCnv = new TCanvas("Cnv","Residues after alignment",410,10,750,500);
    fCnv->Divide(3,2);
    fCnv->Update();

    fCcr = new TCanvas("ccr","Correlations",410,10,750,500);
    fCcr->Divide(3,2);
    fCcr->Update();
 
    fCanvasChiSquare = new TCanvas("fCanvasChiSquare", "ChiSquare", 410,10,750,500);
    fCanvasChiSquare->Divide(3,2);
    fCanvasChiSquare->Update();
 
    fDu  = new TH1F("du","Delta U", 200, -2.*aDistance, +2.*aDistance); // previously 100,-50,50
    fDv  = new TH1F("dv","Delta V", 200, -2.*aDistance, +2.*aDistance);
    fDw  = new TH1F("dw","Delta W", 200, -2.*aDistance, +2.*aDistance);  
    fDdu = new TH1F("Ddu","Delta dU", 200, -2.*aSlopeMax, +2.*aSlopeMax); 
    fDdv = new TH1F("Ddv","Delta dV", 200, -2.*aSlopeMax, +2.*aSlopeMax);
    fDdw = new TH1F("Chi2","Chi2", 200, 0, 10000);

    fChiSquareX  = new TH1F("chiSquareX","Chi Square X", 100, 0, 0.5);
    fChiSquareY  = new TH1F("chiSquareY","Chi Square Y", 100, 0, 0.5);
    fChiSquareZ  = new TH1F("chiSquareZ","Chi Square Z", 100, 0, 0.5);
    fChiSquareTiltX  = new TH1F("chiSquareTiltX","Chi Square TiltX", 200, 0, 20);
    fChiSquareTiltY  = new TH1F("chiSquareTiltY","Chi Square TiltY", 200, 0, 20);


    for (Int_t i=0; i<4 ; i++) {
      fGraph[i].SetPoint(0,0,0);
    }

    Double_t* par = new Double_t[6];
    for(int i=0 ; i<3 ; ++i) par[i] = fAlignement->GetRotations()[i];
    for(int i=3 ; i<6 ; ++i) par[i] = fAlignement->GetTranslation()[i-3];
   
    printResiduals=1;
    ComputeTotalChiSquareMV(par);
    DrawResiduals();
    printResiduals=0;
  
    Bool_t exit = 0;
    //TString myAlgo = "";
    std::string menu = "";
    
    while(exit==0) {

   // S'occuper de l'affichage :)

      menu = PrintMenu();
        
      if(menu.compare("choose")==0 ) { 
        Int_t choice  = Int_t( PrintMinimizationChoice() );
        minimizerName = minimizerChoice[choice];
        algoName      = algoChoice[choice];
      }
      else if(menu.compare("show")==0 ) {
        std::string action = ShowAndSetParameters();
        if(action=="mini") { printResiduals=0; MakeFit( minimizerName, algoName, maxCall, maxIter, tolerance, printLevel, strategy); }
        printResiduals=1;
        ComputeTotalChiSquareMV(par);
        DrawResiduals();
        printResiduals=0;
      }
      else if(menu.compare("mini")==0 ) {
        printResiduals=0;      
        MakeFit( minimizerName, algoName, maxCall, maxIter, tolerance, printLevel, strategy);
      }
      else if(menu.compare("level")==0 ) {
        printLevel = SetPrintLevel();
      }
      else if(menu.compare("exit")==0 ) {
        exit=1;
        std::cout<<"Minimization Tool Exit"<<std::endl;
      } else {
        std::cout<<"Error, wrong entry. Retry."<<std::endl;
      }
       
      //fAlignement->SetInitialRotations();
      //fAlignement->SetInitialPosition(); 

    } // end while
  }
  else if(modeAuto==1) {

    std::cout<<"mode Auto"<<std::endl;
 
    //fAlignement->SetInitialRotations();
    //fAlignement->SetInitialPosition();

    printResiduals=0;
    MakeFit( minimizerName, algoName, maxCall, maxIter, tolerance, printLevel, strategy);
  }
}

//______________________________________________________________________________
//
void MimosaAlignAnalysis::MakeFit(TString minimizerName, TString algoName, Int_t maxCall, Int_t maxIter, Double_t tolerance, Int_t printLevel, Int_t strategy)
{

  std::cout<<"Fitting With Minuit2/GSL"<<std::endl;

  Double_t* initialTranslations = fAlignement->GetTranslation();
  Double_t* initialRotations = fAlignement->GetRotations();

  ROOT::Math::Minimizer* min = ROOT::Math::Factory::CreateMinimizer(minimizerName.Data(), algoName.Data());

  // Set fit parameters (tolerance, max iterations, etc ...)
  min->SetMaxFunctionCalls(maxCall); // for Minuit/Minuit2 
  min->SetMaxIterations(maxIter);  // for GSL 
  //min->SetTolerance(tolerance);
  //min->SetPrecision(1e-9); // set the machine precison. For unknow reason = 1e-9 in sbgpcs119.
  min->SetPrintLevel(printLevel);
  min->SetStrategy(strategy); 

  std::cout<<"Minimizer        = "<<minimizerName<<std::endl;
  std::cout<<"Algorithm        = "<<algoName<<std::endl;
  std::cout<<"PrintLevel       = "<<printLevel<<std::endl;
  std::cout<<"MaxFunctionCalls = "<<maxCall<<std::endl;
  std::cout<<"MaxIteration     = "<<maxIter<<std::endl;

  // create funciton wrapper for minmizer
  ROOT::Math::Functor f(this, &MimosaAlignAnalysis::ComputeTotalChiSquareMV,6);     // Can we pass the ref ? With a map <Int_t, Functor> ?
  //ROOT::Math::Functor f(this, &MimosaAlignAnalysis::fcnMiniVectorsMinuit2, 6); 
  min->SetFunction(f); 

  Double_t step[6] = {0.001, 0.001, 0.001, 0.1, 0.1, 0.1};
  Double_t variable[6] = { initialRotations[0], initialRotations[1] , initialRotations[2], initialTranslations[0], initialTranslations[1], initialTranslations[2] };
                                 
  // Set the free variables to be minimized!

  //min->SetLimitedVariable(0, "tiltz", variable[0], step[0], /*initialRotations[0]*/-60./180.*TMath::Pi(), /*initialRotations[0]*/+60./180.*TMath::Pi());
  //min->SetLimitedVariable(1, "tilty", variable[1], step[1], /*initialRotations[1]-*/-60./180.*TMath::Pi(), /*initialRotations[1]+*/+60./180.*TMath::Pi());
  //min->SetLimitedVariable(2, "tiltx", variable[2], step[2], initialRotations[2]-30./180.*TMath::Pi(), initialRotations[2]+30./180.*TMath::Pi());
  
  min->SetVariable(0,"tiltz",variable[0], step[0]);
  min->SetVariable(1,"tilty",variable[1], step[1]);
  min->SetVariable(2,"tiltx",variable[2], step[2]);
  
/*
  min->SetLimitedVariable(3,"x",variable[3], step[3], -5000., +5000.);
  min->SetLimitedVariable(4,"y",variable[4], step[4], 0., 15000.);
  min->SetLimitedVariable(5,"z",variable[5], step[5], 30000., 40000.);                                           
*/
  min->SetVariable(3,"x",variable[3], step[3]);
  min->SetVariable(4,"y",variable[4], step[4]);
  min->SetVariable(5,"z",variable[5], step[5]);                                           


  //min->SetFunction(f); 
  
  //Minimize :)
  min->Minimize(); 
                                                   
  const Double_t *par = min->X();  // Get the fit parameters after minimization.
  cout << "Minimum: f(" << par[0] << "," << par[1] << "," << par[2] << "," << par[3] << "," << par[4] << "," << par[5] << "): "<< ComputeTotalChiSquareMV(par) << endl;

  fAlignement->SetTranslation( par[3], par[4], par[5] );
  fAlignement->SetRotations(par[0], par[1], par[2]);

  min->Clear();
  delete min;
  
  fAlignement->PrintAll(); 

}

//______________________________________________________________________________
//
/*
std::vector<Double_t>& MimosaAlignAnalysis::AlignMimosaAssociationMV(DLadder* myLadderToAlign, DPrecAlign* initAlignment, DPrecAlign* fAlignmentLadder0, Bool_t modeAuto, Int_t chi2Limit)
{
  // Mimosa alignement method
  // Take an input aligment and fit its data to 
  // The present version allows for z!=0 and fit it
  //
  // LC 2012/09/06.
  // Modified: JB, 2013/07/14 name of angles changed from th0/1/2 to tiltx/y/z 
  
  Info("AlignMimosa","Aligning Mimosa Now...");
  
  //if(!(initAlignment->GetDataPoints()->First())) Fatal("AlignMimosa","No initial alignment found or empty!");
  if(!(initAlignment->GetDataPoints()->First())) Fatal("AlignMimosa","No initial alignment for mini-vectors found or empty!");
  
  //init
  
  ladderToAlign=myLadderToAlign;
  //ladderToAlign->UpdateAlignment();
  fAlignement = initAlignment;
  fAlignement0 = fAlignmentLadder0;
  aChi2Limit = chi2Limit;
  
  fAlignement->SetInitialRotations();
  fAlignement->SetInitialPosition(); 
  

  fDu = fDv = fDw = fDdu = fDdv = fDdw = 0; 
  fCnv = fCcr = 0;
  
  // save all open canvas in a root file 
  Char_t Header[200];
  sprintf(Header,"Align_Plots.root");
  TFile AlignPlots(Header,"update");
  if (AlignPlots.IsZombie() || !AlignPlots.IsOpen()) {
    AlignPlots.Close();
    AlignPlots.ReOpen("create");
  }

  // moved here from FCN, to avoid deletion after procedure, JB 2013/09/19
  fCnv = new TCanvas("Cnv","Residues after alignment",410,10,750,500);
  fCnv->Divide(3,2);
  fCnv->Update();

  fCcr = new TCanvas("ccr","Correlations",410,10,750,500);
  fCcr->Divide(3,2);
  fCcr->Update();
  
  for (Int_t i=0; i<6 ; i++) {
    fGraph[i].SetPoint(0,0,0);
  }
  
  
  if(AlignDebug >= 3) 
  {  
    cout << "Run Number = " <<  fSession->GetRunNumber() <<endl;
    cout << "Plane Number = " << fSession->GetPlaneNumber() <<endl;
    cout << "Initial parameters" << endl;
    fAlignement->PrintAll();
    cout << "Gonna list data " << endl;
    fAlignement->ListDataPoints();
  }
  
  Double_t  *rot = 0;
  Double_t  *tra = 0;
  Double_t  theta[3]={0,0,0}; // JB, removed for inline compilation
  Double_t  trU,trV, trW; // trW added, JB 2010/09/08
  trU=trV=trW=0;
  rot = fAlignement->GetRotations();
  tra = fAlignement->GetTranslation();
  for(Int_t it=0; it<3; it++){
    theta[it]=*(rot+it);
  }
  trU=*tra; trV=*(tra+1); trW=*(tra+2); // trW added, JB 2010/09/08
  
  TMinuit* fit = new TMinuit(6);
  fit->SetFCN(FCNAlignMimosaMV);
  //fit->SetFCN(fcn);
  
  Double_t arglist[10];
  Int_t ierflg = 0;
  //fit->mnhelp();
  
  arglist[0] = 1;
  fit->mnexcm("SET ERR", arglist ,1,ierflg);
  arglist[0] = 2;
  fit->mnexcm("SET STRATEGY ", arglist,1,ierflg); // JB 2012/09/05
  arglist[0] = -1; // -1 for no output
  fit->mnexcm("SET PRINTOUT ", arglist,1,ierflg);



  // Set starting values and step sizes for parameters; 
  // static Double_t vstart[9] = {theta[0], theta[1] ,theta[2],trU, trV, trW}; //, 0., 0., 0.}; // JB, removed for inline compilation 
  Double_t vstart[9] = {theta[0], theta[1] ,theta[2],trU, trV, trW}; //, 0., 0., 0.};  
  
  static Double_t step[9] = {0.001 ,0.001 , 0.001 , 0.1, 0.1, 0.1, 1.0, 1.0, 1.0};
  fit->mnparm(0, "tiltz", vstart[0], step[0], 0.,6.30,ierflg); // was 0,0 instead of -6.3,6.3 before
  fit->mnparm(1, "tilty", vstart[1], step[1], 0.,6.30,ierflg);
  fit->mnparm(2, "tiltx", vstart[2], step[2], 0.,6.30,ierflg);
  fit->mnparm(3, "x", vstart[3], step[3], 0.,0.,ierflg);
  fit->mnparm(4, "y", vstart[4], step[4], 0.,0.,ierflg);
  fit->mnparm(5, "z", vstart[5], step[5], 0.,0.,ierflg);
  
  // Uncomment to fix Z position
  //arglist[0] = 6;
  //fit->mnexcm("fix ", arglist ,1,ierflg);
  
  // Now ready for minimization step
  fDu = new TH1F("du","Delta U",200,-1000,1000); // previously 100,-50,50
  fDv = new TH1F("dv","Delta V",200,-1000,1000);
  fDw = new TH1F("dw","Delta W",200,-300,300);  
  fDdu = new TH1F("Ddu","Delta dU",200,-0.10,0.10); 
  fDdv = new TH1F("Ddv","Delta dV",200,-0.10,0.10);
  fDdw = new TH1F("Chi2","Chi2",200,0,100000000);

  //cutData(5,vstart);   // Menage au dessus de 5 sigmas ;)

  arglist[0] = 50000;
  arglist[1] = 0.01; 
  //fit->mnexcm("MIGRAD", arglist ,2,ierflg);
  
  Double_t amin,edm,errdef=0;
  Int_t nvpar,nparx,icstat=0;
  fit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
  fit->mnprin(3,amin);
 
  //arglist[0] = 1;
  
  Int_t batch_mode =  gROOT->IsBatch() ;
  if (modeAuto==0 || modeAuto==1) batch_mode = modeAuto; // batch_mode = 1 Minuit auto | batch_mode = 0 Minuit by hand.
  else batch_mode = 0;
  
  Char_t commande[250];
  Int_t retour = 0;
 
  if (batch_mode == 0) {
    arglist[0] = 8;       fit->mnexcm("CALl",arglist,1,ierflg);
    fit->mncomd("SHOW PAR",retour); // first display    
    cout << "Enter MINUIT command, type help to get a list:" << endl;  
  }
  while(retour!=11){
    //if (batch_mode == 0)  fit->mnhelp(); // avoid large printout!
    if (batch_mode == 0) cin >> commande;
    for (Int_t i=0 ; i<250 ; i++) if (commande[i]==',') commande[i]=' ' ;
    if (batch_mode == 0)  fit->mncomd(commande,retour);
    if (batch_mode == 1)  fit->mncomd("MINIMIZE",retour);
    arglist[0] = 8;       fit->mnexcm("CALl", arglist ,1,ierflg);
    printf("command : %s  --> retour code : %d \n\n",commande,retour);
    if (batch_mode == 0) cout << "Enter MINUIT command, type help to get a list:" << endl;  
    if (batch_mode == 1) fit->mncomd("exit",retour);
  }
  
  fCcr->Close();
  if(fDu) {delete fDu; fDu  = 0;}
  if(fDv) {delete fDv; fDv  = 0;}
  if(fDw) {delete fDw; fDw  = 0;}
  if(fDdu) {delete fDdu; fDdu  = 0;}
  if(fDdv) {delete fDdv; fDdv  = 0;}
  if(fDdw) {delete fDdw; fDdw  = 0;}
  
  AlignPlots.Close(); 
  
  std::vector<Double_t> errorPar;
  Int_t err;
  Double_t par[6],erpar[6];
  for(Int_t ip=0; ip<6; ip++){
    err=fit->GetParameter(ip,par[ip],erpar[ip]);
    errorPar.push_back(erpar[ip]);
  }
  
  Info("AlignMimosa","Alignement results...");
  
  Double_t* initialPosition = fAlignement->GetInitialPosition();

  fAlignement->SetTranslation( par[3], par[4], par[5] );
  fAlignement->SetRotations(par[0], par[1], par[2]);
  
  fAlignement->PrintAll();
  //fAlignement->GetDataPoints()->Clear(); // required before storing, JB 2011/06/19
  
  //fAlignement->ResetDataPoints(); //SS 2012/01/20
  //fAlignement->ResetDataPoints1();
  
  if(fCnv) {delete fCnv; fCnv = 0;}
  if(fCcr) {delete fCcr; fCcr = 0;}
  
  fDu = fDv = fDw = fDdu = fDdv = fDdw = 0; 
  fCnv = fCcr = 0;
  //fMyfit = 0;
  
  Info("AlignMimosa","Mimosa Alignement Done!");
  
  return errorPar;
}
*/

//______________________________________________________________________________
//

DPrecAlign* MimosaAlignAnalysis::AlignPrecMimosa(DPrecAlign* initAlignment, Bool_t modeAuto, Double_t aDistance)
{
  // Mimosa alignement method
  // Take an input aligment and fit its data to 
  // The present version allows for z!=0 and fit it
  //
  // LC 2012/09/06.
  // Modified: JB, 2013/07/14 name of angles changed from th0/1/2 to tiltx/y/z 
  // Modified: JB, 2013/08/22 aDistance parameter to manage histo range
  
  if(AlignDebug) Info("AlignMimosa","Aligning Mimosa Now...");
  
  if(!(initAlignment->GetDataPoints()->First())) Fatal("AlignMimosa","No initial alignment found or empty!");
  
  //init
  fAlignement = initAlignment;
  fDu = fDv = 0; 
  fCnv = fCcr = 0;
  fMyfit = 0;
  
  //function for fit
  fMyfit = new TF1("myfit","[0]/sqrt(2*3.1416)/[2] * exp(-(x-[1])*(x-[1])/(2.0*[2]*[2])) + [3]/sqrt(2.0*3.1416)/[5]* exp(-(x-[4])*(x-[4])/(2.0*[5]*[5]))", 0, 2);
  fMyfit->SetParameter(0,500);
  fMyfit->SetParameter(1,0);
  fMyfit->SetParameter(2,5);
  fMyfit->SetParameter(3,500);
  fMyfit->SetParameter(4,-20);
  fMyfit->SetParameter(5,5);
  
  // save all open canvas in a root file 
  Char_t Header[200];
  sprintf(Header,"Align_Plots.root");
  TFile AlignPlots(Header,"update");
  if (AlignPlots.IsZombie() || !AlignPlots.IsOpen()) {
    AlignPlots.Close();
    AlignPlots.ReOpen("create");
  }
  
  // moved here from FCN, to avoid deletion after procedure, JB 2013/09/19
  fCnv = new TCanvas("Cnv","Residues after alignment",2,10,400,500);
  fCnv->Divide(1,2);
  fCnv->Update();
  
  fCcr = new TCanvas("ccr","Correlations",410,10,500,500);
  fCcr->Divide(2,2);
  fCcr->Update();
  
  for (Int_t i=0; i<4 ; i++) {
    fGraph[i].SetPoint(0,0,0);
  }
  
  
  if(AlignDebug >= 3) 
  {  
    cout << "Run Number = " <<  fSession->GetRunNumber() <<endl;
    cout << "Plane Number = " << fSession->GetPlaneNumber() <<endl;
    cout << "Initial parameters" << endl;
    fAlignement->PrintAll();
    cout << "Gonna list data " << endl;
    fAlignement->ListDataPoints();
  }
  
  Double_t  *rot = 0;
  Double_t  *tra = 0;
  Double_t  theta[3]={0,0,0}; // JB, removed for inline compilation
  Double_t  trU,trV, trW; // trW added, JB 2010/09/08
  trU=trV=trW=0;
  rot = fAlignement->GetRotations();
  tra = fAlignement->GetTranslation();
  for(Int_t it=0; it<3; it++){
    theta[it]=*(rot+it);
  }
  trU=*tra; trV=*(tra+1); trW=*(tra+2); // trW added, JB 2010/09/08
  
  
  TMinuit* fit = new TMinuit(6);
  fit->SetFCN(FCNAlignMimosa);
  //fit->SetFCN(fcn);
  
  Double_t arglist[10];
  Int_t ierflg = 0;
  //fit->mnhelp(); // to limit printout
  
  arglist[0] = 1;
  fit->mnexcm("SET ERR", arglist ,1,ierflg);
  arglist[0] = 2;
  fit->mnexcm("SET STRATEGY ", arglist,1,ierflg); // JB 2013/09/27
  arglist[0] = -1; // -1 for no output
  fit->mnexcm("SET PRINTOUT ", arglist,1,ierflg); // JB 2013/09/27
  
  // Set starting values and step sizes for parameters; 
  // static Double_t vstart[9] = {theta[0], theta[1] ,theta[2],trU, trV, trW}; //, 0., 0., 0.}; // JB, removed for inline compilation 
  Double_t vstart[9] = {theta[0], theta[1] ,theta[2],trU, trV, trW}; //, 0., 0., 0.};  
  
  static Double_t step[9] = {.01 ,.01 , .01 , 40.0, 40.0, 5.0, 1.0, 1.0, 1.0};
  fit->mnparm(0, "tiltz", vstart[0], step[0], -6.30,6.3,ierflg); // was 0,0 instead of -6.3,6.3 before
  fit->mnparm(1, "tilty", vstart[1], step[1], -6.30,6.30,ierflg);
  fit->mnparm(2, "tiltx", vstart[2], step[2], -6.30,6.30,ierflg);
  fit->mnparm(3, "x", vstart[3], step[3], 0,0,ierflg);
  fit->mnparm(4, "y", vstart[4], step[4], 0,0,ierflg);
  fit->mnparm(5, "z", vstart[5], step[5], 0,0,ierflg);
  
  
  if(AlignDebug >= 1) 
  { 
    fit->mnparm(6, "a(x,y)", vstart[6], step[6], 0,0,ierflg); // not sure work with TMinuit(6) declaration above
    fit->mnparm(7, "cu", vstart[7], step[7], 0,0,ierflg);
    fit->mnparm(8, "cv", vstart[8], step[8], 0,0,ierflg);
    
    for (Int_t i=1 ; i <=9 ; i++){
      arglist[0] = i;
      fit->mnexcm("fix ", arglist ,1,ierflg);
    }
    
    arglist[0] = 1;
    fit->mnexcm("rel ", arglist ,1,ierflg);
    arglist[0] = 2;
    fit->mnexcm("rel ", arglist ,1,ierflg);
    arglist[0] = 3;
    fit->mnexcm("rel ", arglist ,1,ierflg);
    arglist[0] = 4;
    fit->mnexcm("rel ", arglist ,1,ierflg);
    arglist[0] = 5;
    fit->mnexcm("rel ", arglist ,1,ierflg);
    arglist[0] = 6;
    fit->mnexcm("rel ", arglist ,1,ierflg);
  }
  
  // Uncomment to fix Z position
  arglist[0] = 6;
  fit->mnexcm("fix ", arglist ,1,ierflg);
  arglist[1] = trW-5000.;
  arglist[2] = trW+5000.;
  fit->mnexcm("set limit ", arglist ,3,ierflg);
  
  // Now ready for minimization step
  // Decide the range from the bound, JB 2013/08/22
  //fDu = new TH1F("du","Delta U",200,-300,300); // previously 100,-50,50
  //fDv = new TH1F("dv","Delta V",200,-300,300);
  fDu = new TH1F("du","Delta U",200,-1.*aDistance,1.*aDistance);
  fDv = new TH1F("dv","Delta V",200,-1.*aDistance,1.*aDistance);
  
  arglist[0] = 500;
  arglist[1] = 1.; 
  //fit->mnexcm("MIGRAD", arglist ,2,ierflg);
  
  Double_t amin,edm,errdef;
  Int_t nvpar,nparx,icstat;
  fit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
  fit->mnprin(3,amin);
  
  arglist[0] = 1;
  
  int batch_mode =  gROOT->IsBatch() ;
  if (modeAuto==0 || modeAuto==1) batch_mode = modeAuto; // batch_mode = 1 Minuit auto | batch_mode = 0 Minuit by hand.
  else batch_mode = 0;
  
  Char_t commande[250];
  Int_t retour = 0; // removing warning: variable 'retour' should be initialized BH 2013/08/20
  
  if (batch_mode == 0) {
    arglist[0] = 8;       fit->mnexcm("cali", arglist ,1,ierflg);
    fit->mncomd("SHOW PAR",retour); // first display    
    cout << "Enter MINUIT command, type help to get a list:" << endl;  
  }
  while(retour!=11){
    //if (batch_mode == 0)  fit->mnhelp(); // avoid large printout!
    if (batch_mode == 0) cin >> commande;
    for (Int_t i=0 ; i<250 ; i++) if (commande[i]==',') commande[i]=' ' ;
    if (batch_mode == 0)  fit->mncomd(commande,retour);
    if (batch_mode == 1)  fit->mncomd("MINIMIZE",retour);
    arglist[0] = 8;       fit->mnexcm("cali", arglist ,1,ierflg);
    printf("command : %s  --> return code : %d \n\n",commande,retour);
    if (batch_mode == 0) cout << "Enter MINUIT command, type help to get a list:" << endl;  
    if (batch_mode == 1) fit->mncomd("exit",retour);
  }
  
  fCnv->Close();
  fCcr->Close();
  if(fDu) {delete fDu; fDu  = 0;}
  if(fDv) {delete fDv; fDv  = 0;}
  AlignPlots.Close(); 
  
  /* Useless so far, JB 2013/07/14
   Int_t yyy;
   if (batch_mode == 0) cout << " Store the results of alignement? (1/0) " << endl;
   if (batch_mode == 0) cin >> yyy;
   if (batch_mode == 1) yyy = 1; // force storing in batch, JB 2011/06/17
   */
  
  Int_t err;
  Double_t par[6],erpar[6];
  for(Int_t ip=0; ip<6; ip++){
    err=fit->GetParameter(ip,par[ip],erpar[ip]);
  }
  
  Info("AlignMimosa","Alignement results...");
  
  //std::cout<<"test rgfez"<<std::endl;
  
  fAlignement->SetTranslation(par[3],par[4],par[5]);
  fAlignement->SetRotations(par[0],par[1],par[2]);
  fAlignement->PrintAll();
  //fAlignement->GetDataPoints()->Clear(); // required before storing, JB 2011/06/19
  
  fAlignement->ResetDataPoints(); //SS 2012/01/20
  
  if(fCnv) {delete fCnv; fCnv = 0;}
  if(fMyfit) {delete fMyfit; fMyfit = 0;}
  if(fCcr) {delete fCcr; fCcr = 0;}
  
  fDu = fDv = 0; 
  fCnv = fCcr = 0;
  fMyfit = 0;
  
  Info("AlignMimosa","Mimosa Alignement Done!");
  
  return fAlignement;
}

