// @(#)MRaw.cxx  $:$Id: MRaw.cxx,v.1 2005/10/02 18:03:46 sha Exp $
// Author: A. Shabetai
// Last Modified: JB, 2008/10/15
// Last Modified: JB, 2009/05/06
// Last Modified: RDM, 2009/05/21
// Last Modified: JB, 2009/06/26
// Last Modified: JB, 2009/09/01
// Last Modified: JB, 2009/09/18 DisplayRawData2D
// Last Modified: JB, 2010/06/16 DisplayHits2D
// Last Modified: JB, 2010/10/25 DisplayRawData2D
// Last Modified: JB, 2011/02/09 verbosity
// Last Modified: JB, 2011/07/21 introduce fSession member pointer
// Last Modified: JB, 2011/07/25 LaserStudy
// Last Modified: JB, 2011/07/26 new method for track display
// Last Modified: JB, 2011/07/27 new method for vertices display
// Last Modified: JB, 2011/09/16 LaserStudy for MIMOSA 26
// Last Modified: JB, 2011/11/02 new method DisplayCumulatedHits2D & DisplayGeometry
// Last Modified: JB, 2011/11/22 new method FakeRateBinaryFromRawData
// Last Modified: MG, 2012/03/14 new method DisplayCumulatedOverflow for PLUME line overflow study
// Last Modified: MG, 2012/03/14 FakeRateBinaryFromRawData : results store in a file
// Last Modified: JB+VR, 2012/04/02 upgraded methods DisplayCumulatedHits2D, DisplayCumulatedRawData2D
// Last Modified: JB, 2012/05/03 new method DumpEventHeader
// Last Modified: JB, 2012/05/07 upgraded method DisplayCumulatedTracks
// Last Modified: JB, 2012/08/17 new plots for analog output sensors
// Last Modified: JB, 2013/05/26 DisplayRawData2D, DisplayCumulatedRawData2D, UserPlot
// Last Modified: BH, 2013/08/20 warning corrections from clang
// Last Modified: JB, 2013/08/21 DisplayRawData
// Last Modified: JB, 2013/08/29 DisplayNoise
// Last Modified: JB, 2013/11/08 DisplayCumulatedHits2D
// Last Modified: JB, 2013/12/03 DisplayNoise
// Last Modified: JB, 2014/03/11 DisplayCumulatedRawData2D
// Last Modified: JB, 2014/06/02 DisplayImage
// Last Modified: JB, 2014/06/02 DisplayHistory
// Last Modified: JB, 2014/09/28 DisplayCumulatedTracks
// Last Modified: AP, 2014/10/22 DisplayCumulatedRawData2D
// Last Modified: JB, 2014/11/23 DisplayCumulatedTracks
// Last Modified: JB, 2015/03/18 DisplayCumulatedTracks
// Last Modified: JB, 2015/05/25 DisplayCumulatedHits2D, DisplaySpectrum
// Last Modified: QL, 2015/06/10 StudyTrackingEfficiency
// Last Modified: JB, 2015/08/21 StudyMultipleScattering
// Last Modified: JH, 2016/07/20 XrayAnalysis
// Last Modified: AP, 2016/08/19 BetaSourceMultiFrameAnalysis
// Last Modified: AP, 2017/05/09 Added FillnTupleForTMVA function fill tree for TMVA analysis
// Last Modified: JB, 2019/03/10 Sitrineo analysis

/////////////////////////////////////////////////////////////
//                                                         //
//  Describes all Raw data analysis methods                //
//                                                         //
// Provides event by event analysis
//  and various display methods (raw data, hits, track).
//
// Usage: call gTAF->GetRaw() and use the pop-up menu.
//	or directyl gTAF->GetRaw()->MethodName(...)
//
// See method PrepareRaw() for the list of available methods
//
/////////////////////////////////////////////////////////////

#include "MRaw.h"
#include "TLine.h"
#include "DR3.h"
#include "DLine.h"
#include "TEllipse.h"
#include "DTrack.h"
#include "TPolyLine3D.h"
#include "TH3F.h"
#include "TPolyMarker3D.h"
#include "TArrow.h"
#include "TRandom.h"
#include "DLadder.h"
#include "TImage.h"
#include "TProfile.h"
#include "TSpectrum.h"
//DANIEL
#include "TH1D.h"
#include "DPlane.h"
#include "DTracker.h"
#include "DEventMC.h"
#include "MCBoardReader.h"
#include "DSetup.h"
#include <algorithm>
#include <TMinuit.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <math.h>
#include <iomanip>
#include <assert.h>
#include <vector>
#include <cmath>

#ifdef USETMVA
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#include "TMVA/TMVARegGui.h"
//#include "../tmva/test/TMVARegGui.C"

using namespace TMVA;
#endif // USETMVA

ClassImp(MRaw)

//______________________________________________________________________________
//
void  MRaw::PrepareRaw()
{

  Char_t *goal = fSession->GetSetup()->GetAnalysisPar().AnalysisGoal;
  cout << "Run number is " << fSession->GetRunNumber() << " with analysis goal = " << goal << "." << endl;

  if (!gROOT->IsBatch())
    {
      if(fDebugRaw) printf( "MRAW::PrepareRaw generating Menu\n");
      bar3  = new TControlBar("vertical","Raw Data", 600, 600); // place the menu on upper right corner, JB 2009/05/25
//      bar3->AddButton("Init Scanning ","gTAF->GetRaw()->InitScan();", "Accumulate certain amount of events for scanning");
//      bar3->AddButton("MIMOSA event display","gTAF->GetRaw()->MimosaDisplay()", "MIMOSA event display.RUN IT AFTER Inialisation + Init Scan");
//      bar3->AddButton("RS display","gTAF->GetRaw()->RSDisplay()", "Display Ref.system events");
//      bar3->AddButton("INSPECT EVENT","gTAF->GetRaw()->InspectScan()", "RUN IT AFTER MIMOSA event display");
//      bar3->AddButton("INSPECT FAKES","gTAF->GetRaw()->Inspectfake()", "RUN IT AFTER MIMOSA event display");
//      bar3->AddButton("JUMP EVENTS","gTAF->GetRaw()->MimosaJump()", "RUN IT AFTER MIMOSA event display");
//      bar3->AddButton("RAW CHANNELS", "gTAF->GetRaw()->DisplayRawData(1.)", "Display raw channels per plane and event");
      bar3->AddButton("RAW CHANNELS 2D", "gTAF->GetRaw()->DisplayRawData2D()", "Display raw channels per plane and event in 2D");
      bar3->AddButton("CUMULATE RAW CHANNELS 2D", "gTAF->GetRaw()->DisplayCumulatedRawData2D(500)", "Display raw data per plane cumulated over 500 events in 2D");
      bar3->AddButton("HITS 2D", "gTAF->GetRaw()->DisplayHits2D(2,1,0)", "Display hits per plane and event in 2D");
      bar3->AddButton("CUMULATE HITS 2D", "gTAF->GetRaw()->DisplayCumulatedHits2D(500)", "Display hits per plane cumulated over 500 events in 2D");
      bar3->AddButton("CUMUL HITS 2D LADDER (exp)", "gTAF->GetRaw()->DisplayLadderCumulatedHits2D(500)", "Display hits per ladder cumulated over 500 events in 2D");
      bar3->AddButton("NOISE MAP", "gTAF->GetRaw()->DisplayNoise()", "Display noise per pixel for each plane");

      bar3->AddButton("BINARY FAKE", "gTAF->GetRaw()->FakeRateBinaryFromRawData(100000, 100, 0.01)", "Compute the fake rate for binary output sensors");
      bar3->AddButton("CUMULATE LINE OVERFLOW", "gTAF->GetRaw()->DisplayCumulatedOverflow()", "Display line overflow cumulated over 500 events in several views");
      bar3->AddButton("RAW SPECTRUM", "gTAF->GetRaw()->DisplaySpectrum( 1000, 3, 2048, 0)", "Display the spectrum from raw data");

      bar3->AddButton("CUMULATE MONTE CARLO", "gTAF->GetRaw()->DisplayCumulatedMonteCarlo2D(500)", "Display Monte Carlo hits cumulated over 500 events in several views");


      if( strstr(goal, "track") || strstr(goal, "Track") ) {
        bar3->AddButton("TRACKS", "gTAF->GetRaw()->DisplayTracks()", "Display tracks per event in several views");
        bar3->AddButton("CUMULATE TRACKS", "gTAF->GetRaw()->DisplayCumulatedTracks()", "Display tracks cumulated over 500 events in several views");
        bar3->AddButton("HITS-TRACKS HISTORY", "gTAF->GetRaw()->DisplayHistory()", "Display hits and tracks properties wrt event#");
        bar3->AddButton("Study Tracking Efficiency", "gTAF->GetRaw()->StudyTrackingEfficiency(10000)", "Study Tracking Efficiency for planes in the telescope");
        bar3->AddButton("Study Beam Profile", "gTAF->GetRaw()->StudyBeamProfile(1000)", "Study Beam Profile");
      }

      if( strstr(goal, "vertex") || strstr(goal, "Vertex") ) {
        bar3->AddButton("VERTEX STUDY", "gTAF->GetRaw()->VertexStudy()","Vertices from tracks and a fixed beam line");
      }

      if( strstr(goal, "laser") || strstr(goal, "Laser") ) {
        bar3->AddButton("LASER STUDY", "gTAF->GetRaw()->LaserStudy( 125, 125, 100)", "Laser detection efficiency");
      }

      if( strstr(goal, "calib") || strstr(goal, "Calib") ) {
        bar3->AddButton("X-ray", "gTAF->GetRaw()->XrayAnalysis(500)", "Analysis for X-ray source over 500 events");
        bar3->AddButton("Gain Map", "gTAF->GetRaw()->BuildPixelGainMap( 100000)", "Build gain map for each pixel");
      }

      if( strstr(goal, "sitrineo") || strstr(goal, "Sitrineo") ) {
        bar3->AddButton("SITRINEO/EVT", "gTAF->GetRaw()->SitrineoByEvent(2)", "Sitrineo analysis event-by-event");
        bar3->AddButton("SITRINEO/CUMUL", "gTAF->GetRaw()->SitrineoCumul(1000,2)", "Sitrineo analysis cumulated over X events");
      }

      bar3->AddButton("DISPLAY IMAGE", "gTAF->GetRaw()->DisplayImage(100)","Build a GIF image with 100 events");
      bar3->AddButton("DISPLAY GEOMETRY", "gTAF->GetRaw()->DisplayGeometry()","Telescope geometry");
      bar3->AddButton("USER PLOT", "gTAF->GetRaw()->UserPlot()", "Plots built by user function");
      bar3->AddButton("TOGGLE VERBOSITY", "gTAF->GetRaw()->ToggleVerbosity()", "Switch On/Off messages");
      bar3->AddButton("SKIP 100 EVENTS", "gTAF->GetRaw()->SkipEvent(100)", "Skip 100 events without analyzing them for hits");
      bar3->AddButton("Clear / Close","gTAF->GetRaw()->Clear()","Clear everything to start over");
      bar3->Show();
    }

  gStyle->SetPalette(1);
  gStyle->SetTitleSize(0.06,"xyz");
  gStyle->SetTitleOffset(1.1,"y");
  gStyle->SetTitleOffset(1.1,"y");
  gStyle->SetLabelSize(0.06,"xyz");

}

//______________________________________________________________________________
//

int save_tiff_file(unsigned char * buffer,unsigned int width, unsigned int height, double pixelSize, unsigned int nBits, char *filename)
{
  // Generate a TIFF image from a buffer
  //
  //
  //
  /*
  cout << "saving tiff file : "<<filename << " with " << width << "x" << height << " pixels of " << pixelSize << endl;

  // Define an image
  TIFF *image;

  // Open the TIFF file
  if((image = TIFFOpen(filename, "w")) == NULL){
    printf("Could not open output.tif for writing\n");
    return 1;
  }

  // We need to set some values for basic tags before we can add any data
  TIFFSetField(image, TIFFTAG_IMAGEWIDTH, width );
  TIFFSetField(image, TIFFTAG_IMAGELENGTH, height);
  TIFFSetField(image, TIFFTAG_BITSPERSAMPLE, nBits );
  TIFFSetField(image, TIFFTAG_SAMPLESPERPIXEL, 1);
  TIFFSetField(image, TIFFTAG_ROWSPERSTRIP, height);

  TIFFSetField(image, TIFFTAG_MINSAMPLEVALUE, 0);
  TIFFSetField(image, TIFFTAG_MAXSAMPLEVALUE, pow(2, nBits)-1);

  TIFFSetField(image, TIFFTAG_COMPRESSION, COMPRESSION_NONE );
  TIFFSetField(image, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
  TIFFSetField(image, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
  TIFFSetField(image, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

  TIFFSetField(image, TIFFTAG_XRESOLUTION, 1);
  TIFFSetField(image, TIFFTAG_YRESOLUTION, 1);
  TIFFSetField(image, TIFFTAG_RESOLUTIONUNIT, RESUNIT_CENTIMETER);

  int w, h, b;
  double s;
  TIFFGetField(image, TIFFTAG_IMAGEWIDTH, &w );
  TIFFGetField(image, TIFFTAG_IMAGELENGTH, &h);
  TIFFGetField(image, TIFFTAG_BITSPERSAMPLE, &b);
  TIFFGetField(image, TIFFTAG_XRESOLUTION, &s);
  cout << "  ==> width = " << w << ",  height = " << h << endl;
  cout << "  ==> pixel/cm = " << s << " cm,  nbits = " << b << endl;


//  cout << "  image properties: " << TIFFTAG_IMAGEWIDTH << "x" << TIFFTAG_IMAGELENGTH << " pixels of " << TIFFTAG_XRESOLUTION << " resolution, over " << TIFFTAG_BITSPERSAMPLE << " bits." << endl;

  // Write the information to the file
  TIFFWriteEncodedStrip(image, 0, buffer, width * height); // *2 si 16 bits

  // Close the file
  TIFFClose(image);

  cout << "  file created: " << filename << endl;
*/
  return 0;
}



//______________________________________________________________________________
//
void MRaw::ToggleVerbosity()
{
  // Toggle between the verbose or quiet mode
  // JB 2011/02/09

  fVerbose = !fVerbose;
  printf( "\n--> Turning %s.\n\n", fVerbose?"verbose":"quiet");

}

//______________________________________________________________________________
//
void Remove(TObject* newHistName)
{
  //Renoves some object from the list of histograms

  TObject* old=gDirectory->GetList()->FindObject(newHistName);
  gDirectory->GetList()->Remove(old);

}


//______________________________________________________________________________
//
void MRaw::Clear(const Option_t*)
{
  // Clean up

  if(bar3) {bar3->Hide(); delete bar3 ; bar3=0;}
  if(cnv) { delete cnv ; cnv=0;}
}

//______________________________________________________________________________
//
Bool_t MRaw::CheckIfDone(const Option_t* Option)
{
  // Checks if the requiered methods have been called before processing

  TString opts(Option);
  opts.ToLower();
  Bool_t aReturnCode = kTRUE;

  if(opts.Contains("display"))
    { if (!fMDisplayDone)
	{ Warning("CheckIfDone()","Please run MimosaDisplay() First");aReturnCode= kFALSE;};
    }
  if(opts.Contains("init"))
    { if (!fInitDone)
	{ Warning("CheckIfDone()","Please run InitScan() First");aReturnCode = kFALSE;};
    }

  return aReturnCode;
}

//______________________________________________________________________________
//
MRaw* MRaw::fgInstanceRaw = 0; // returns pointer to global object


//______________________________________________________________________________
//
MRaw::MRaw( DSession *aSession)
{

  // MRaw default constructor
  //
  // Modified: JB 2011/07/21, fSession var introduced
  // Modified: JB 2013/07/18, for MultiFrame parameters
  // Modified: JB 2014/05/29, Debug level added

  fSession = aSession;

  fUserFileNumber=0;

  gStyle->SetTitleSize(0.06);
  gStyle->SetLabelSize(0.06,"xyz");
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetTitleH(.085);
  if (fgInstanceRaw) Warning("MRaw", "object already instantiated");
  else   fgInstanceRaw = this;

  fInitDone =  fMDisplayDone = 0;
  fVerbose = kFALSE;
  MultiFramePlane = 0; // JB 2013/07/18
  MultiFrameNFrames = 1;
  NbinsReductionFactor = 1.; // JB 2014/12/08

  fDebugRaw = aSession->GetDebug(); // JB 2014/05/29

  if (!gROOT->IsBatch()) {PrepareRaw();}

}

//______________________________________________________________________________
//
void MRaw::InitScan(Int_t Events2Scan, Float_t SignalOverNoiseCut )
{

  // Initialize raw-data processing

  //if(!gTAF->CheckIfDone("init")) return;
  if(!CheckIfDone("init")) return;

  gStyle->SetPalette(1,0);

  // requires call to init Session in order to run!
  tTracker  =  fSession->GetTracker();
  Int_t PlaneNumber=fSession->GetPlaneNumber();

  if( !PlaneNumber) { // protection against plane=0, JB, 2007, june
    cout << endl << "WARNING: you need to initialize a plane number in InifSession !!" << endl;
    return;
  }

  cout<<" PlaneNumber " <<PlaneNumber<<endl;
  cout << "inits for scanning over " << Events2Scan << " events..." << endl;
  fSession->SetEvents(Events2Scan); // < 5000 1600 Max number of event you can look at.
  //cout << "Take first  events?" << endl;;
  Int_t EventNumber(Events2Scan);//mi5 = 100//200 // these events will be used to compute pedestal and noise.
  Int_t Binnumber ;

  startMIMOSA=kTRUE;
  startRS=kTRUE;
  cout<<"hit separation studies ..."<<endl;
  for (Int_t i=0 ; i<EventNumber ; i++ ){
    //FILE *test = thePl->GetFilePointer();
    //test.dat print
    //fprintf(test,"\n ------------------------EVENT = %d --------------------------- \n66",i+1);
    if(i/50*50 == i) cout << "event " << i << "\n";
    //cout << "----------------------------------------------event " << i << "\n";
    fSession->NextRawEvent() ;
    tTracker->Update();
    gStyle->SetOptStat(0);

    //-------------------PRINT RAW DATA IN A SIGNAL FOR HIT SEPARATION STUDIES.
    DPlane *thePlMraw = fSession->GetTracker()->GetPlane(PlaneNumber);
    Int_t nuraw =thePlMraw->GetStripsNu() ;
    Int_t nvraw =thePlMraw->GetStripsNv() ;
    Binnumber = nvraw * nuraw;
    //cout<<"hit separation studies ..."<<endl;
    Char_t Header_hit[100];
    sprintf(Header_hit,"%sHitSeparation_rawdata_%d_%d.dat",fSession->GetResultDirName().Data(),fSession->GetRunNumber(),PlaneNumber);
    sprintf(Header_hit,"%s", fTool.LocalizeDirName( Header_hit)); // JB 2011/07/07
    ofstream outFile(Header_hit);
    Int_t EventNumber2=0;
    for (Int_t i=0 ; i<EventNumber2 ; i++ ){
      for ( Int_t j=1 ; j<= Binnumber; j++) {
	Float_t pulse = thePlMraw->GetPulseHeight(j);
	Float_t noiseval = thePlMraw->GetNoise(j);
	if (noiseval<0.000010) {
	  //   printf ("pixel %f : Noise = %f\n",j,noiseval);
	  noiseval=0.01 ;
	}
	if( (pulse/noiseval) < SignalOverNoiseCut){
	  outFile << thePlMraw->GetPulseHeight(j) << " ";
	}
	outFile << " \n";
	// cout<<"pulse= "<<pulse<<" noiseval="<<noiseval<<" RAW="<< thePlMraw->GetRawValue(j)<<" pulse/noiseval="<<pulse/noiseval<<endl;
      }
    }
  }
  cout<<"...hit separation studies done"<<endl;

  fInitDone=kTRUE;


}


//______________________________________________________________________________
//
void MRaw::RSDisplay()
{

  // Displays reference planes

  //if(!gTAF->CheckIfDone("init")) return;
  if(!CheckIfDone("init,display")) return;

  // Macro to show Reference Silicon Strip Detector planes signal
  // requires call to MAinifSession.C in order to run!

  cnv=0 ;
  DPlane *somePl=0;
  Int_t nu=0  ;

  TH1F*  hRSraw=0 ;
  TH1F* hRSnoise=0 ;
  TH1F* hRSnoise1=0 ;
  TH1F* hRSson1=0;
  TH1F* hRSson=0;
  TH1F* hRSpu=0;
  TH1F* hRSnoisd=0;
  TH1F* hRSped=0;
  Char_t titre[80] ;

  if(startRS)
    {
      sprintf(titre,"Tracker hits: event %d",fSession->GetCurrentEventNumber());
      cnv = new TCanvas("cnv",titre,200,0,600,900);
      cnv->Divide(2,4);
      // find number of strips in planes:
      somePl = fSession->GetTracker()->GetPlane(2);
      nu = somePl->GetStripsNu() ;

      hRSraw =  new TH1F("RSraw",titre,nu,0.5,nu+0.5);
      hRSnoise = new TH1F("RSnoise",titre,nu,0.5,nu+0.5);
      hRSnoise1 = new  TH1F("RSnoise1",titre,nu,0,nu);
      hRSson1 = new  TH1F("RSson",titre,nu,0,nu);
      hRSson = new  TH1F("hRSson","signal/bruit",250,-2.5,3.0);
      hRSpu =  new TH1F("hRSpu","signal",250,-5.,100.);
      hRSnoisd =  new TH1F("RSnoisd",titre,250,0,5.);
      hRSped = new TH1F("RSped",titre,nu,0,nu);
      startRS=kFALSE;
    }
  else{
    delete cnv; cnv=0;
    sprintf(titre,"Tracker hits: event %d",fSession->GetCurrentEventNumber());

    cnv = new TCanvas("cnv",titre,200,0,600,900);

    cnv->Divide(2,4);

    somePl = fSession->GetTracker()->GetPlane(2);
    nu = somePl->GetStripsNu() ;

  }
  cnv->Draw();
  showit = kFALSE ;


  while(!showit){
    fSession->NextRawEvent();
    tTracker->Update();
    cout << "\n Event number : " << fSession->GetCurrentEventNumber()-1<<" ";

    showit = kTRUE ;
  }
  Char_t name[100];

  for(Int_t ipl=1; ipl<=8; ipl++){
    DPlane  *thePl=fSession->GetTracker()->GetPlane(ipl);
    nh=thePl->GetHitsN();
    //  cout << "   List of  " << nh << " hits in the plane :" << endl ;

    for (Int_t iu=1 ; iu<=nu ; iu++){
      Float_t value = thePl->GetRawValue(iu);
      Float_t noiseval = thePl->GetNoise(iu);
      if(noiseval == 0) noiseval=0.1;
      Float_t pulse = thePl->GetPulseHeight(iu);
      hRSraw->Fill(iu,value);
      //    cout << iu << ":" << value << "  "<< noiseval << "  " << pulse << endl;

      if (pulse/noiseval<-1.0)
	hRSnoise->Fill(iu,1);
      hRSnoise1->Fill(iu,noiseval);
      hRSpu->Fill(pulse);
      hRSnoisd->Fill(noiseval);
      hRSped->Fill(iu,somePl->GetPedestal(iu)/8.);
      if (noiseval<0.010) {
	//printf ("pixel %f : Noise = %f\n",iu,noiseval);
	noiseval=0.01 ;
      }
      hRSson1->Fill(iu,pulse/noiseval);
      hRSson->Fill(pulse/noiseval);


    }
    //---
    Int_t np=thePl->GetPlaneNumber();
    cout << "plane " << np << thePl->GetPlanePurpose()<<endl;

    nh=thePl->GetHitsN();
    cout << "   List of  " << nh << " hits in the plane " << np <<" :" << endl ;
    for (Int_t ih=1; ih<=nh ;ih++){
      cout << ih ;
      DHit *aHit = thePl->GetHit(ih) ;
      Int_t ise = aHit->GetIndexSeed();
      cout << " u : " << aHit->GetSeedU()
	   << "    cluster charge : " << ise <<
	" " << aHit->GetClusterPulseSum() << endl ;

    }

    //---
    cnv->cd(ipl);
    sprintf(name, "%s", thePl->GetPlanePurpose());
    cout << name << endl;
    TText *txt = new TText(0.3,0.05,name);
    txt->SetNDC(kTRUE);
    hRSson1->Draw();
    gPad->Update();
    txt->Draw();
    txt->SetTextSize(0.1);

    hRSson1->SetFillColor(5);
  }

  for(Int_t ipl=1; ipl<=8; ipl++)
    {
      cnv->cd(ipl);
      gPad->Modified();
      gPad->Update();
    }

  startRS=kTRUE;

  Remove(hRSson1);
  Remove(hRSraw);
  Remove(hRSnoise);
  Remove(hRSnoise1);
  Remove(hRSson1);
  Remove(hRSson);
  Remove(hRSpu);
  Remove(hRSnoisd);
  Remove(hRSped);
}


//______________________________________________________________________________
//
void MRaw::MimosaDisplay(Int_t NEVENT /*=1*/ )
{
  //raw events display
  //
  // Modified: JB 2009/09/01, to get rid of call to GetParameters()

  //if(!gTAF->CheckIfDone("init")) return;
  // requires call to init Session in order to run!
  if(!CheckIfDone("init")) return;

  char titre[50] ;
  sprintf(titre,"Run %d  Event POS %d",fSession->GetRunNumber(),fSession->GetCurrentEventNumber());

  cnv = new TCanvas("cnv", titre,400,0,700,950);
  cnv->UseCurrentStyle();

  DTracker *tTracker  =  fSession->GetTracker();

  Int_t NPlaneChosen = fSession->GetPlaneNumber();
  DPlane* thePlM = fSession->GetTracker()->GetPlane(NPlaneChosen);
  Int_t nu = thePlM->GetStripsNu() ;
  Int_t nv = thePlM->GetStripsNv() ;
  //----ab limit the maximum size for 1D histo: max seems to be around 50 000
  Int_t numax  = nu;
  Int_t nvmax = nv;
  showit = kFALSE ;
  Float_t NoiseCut = 5.;
  TPolyMarker *pm = new TPolyMarker;
  Float_t tPixelPitchU = thePlM->GetStripPitch()(0); // JB 2009/09/01
  Float_t tPixelPitchV = thePlM->GetStripPitch()(1); // JB 2009/09/01

  //-----noise studies.
  const Int_t hitarraysize = 1000;
  Float_t Hitarray[hitarraysize][2]; //[0] = u position, [1] = v position
  Float_t hitseedcutsize = 10.0 ; //in microns.
  Float_t hitregioncutsize = 50.0 ; //in microns.
  Int_t xminpixel = 4; //in pixel number
  Int_t xmaxpixel = 509; //in pixel number
  //------------Scans NEVENTS events (1) by defaults

  //-------iterated histogram setting:
  iterated_bin_number = 400;
  iterated_xmin_number = -100;
  iterated_xmax_number = +100;
  //----ab limit the maximum size for 1D histo: max seems to be around 50 000
  if(numax*nvmax > 50000){nvmax = 50;}

  pm->SetMarkerStyle(5);
  pm->SetMarkerSize(5);
  pm->SetMarkerColor(1);


  // NEVENTS LOOP
  for (Int_t iii=0; iii<NEVENT ;iii++){
    showit = kFALSE ;
    cout<<"-----------------------------------------------------------------"<<endl;
    cout<<"NEVENT = "<<iii<<"  --------------------"<<endl;
    cout<<"-----------------------------------------------------------------"<<endl;
    //

    while(!showit){
      fSession->NextRawEvent();
      fSession->SetStatus(1);
      tTracker->Update();
      cout << "\n Event number POS: " << fSession->GetCurrentEventNumber() -1<<endl ;
      Int_t number=thePlM->GetPlaneNumber();
      Char_t *PlaName=thePlM->GetPlanePurpose();

      nh=thePlM->GetHitsN();
      printf(" Plane Number = %d,\n Plane Name is %s, \n Number of hits = %d \n",number,PlaName,nh);
      showit = kTRUE ;

      if (nh==0) {
	cout << "  No hits in this plane. " << nh <<endl;
      }
      else {
	cout << " List of " << nh << " hits in the " << number << " plane"<< endl ;
	for (Int_t ih=1; ih<=nh ;ih++){
	  cout << ih << ": ";
	  DHit *aHit = thePlM->GetHit(ih);
	  DStrip *theStrip = aHit->GetSeed();
	  cout << " u= " << aHit->GetSeedU() << "   v= " << aHit->GetSeedV()
	       << " seed S/N = " << theStrip->GetPulseHeightToNoise() << endl;
	  //   << "    seed= " << ise << " uc= " << ise/64*20-630 << " vc= " << (ise%64)*20-630
	  //	   << " Cluster Pulse Sum= " << aHit->GetClusterPulseSum() << endl ;
	  if(ih<hitarraysize){
	    Hitarray[ih-1][0]=aHit->GetSeedU();
	    Hitarray[ih-1][1]=aHit->GetSeedV();
	  }else{
	    cout<<"-------------------------WARNING TOO MANY HITS IN THIS EVENTS"<<endl;
	  }

	  if (aHit->GetClusterPulseSum()>0) showit=kTRUE;
	}
      }

    }


    if(startMIMOSA){

      raw1 = new TH2F("raw1","raw signal before CDS",nu,0.5,nu+0.5,nv,0.5,nv+0.5);
      raw2 = new TH2F("raw2","raw signal before CDS",nu,0.5,nu+0.5,nv,0.5,nv+0.5);
      raww = new TH2F("raww","raw signal after CDS",nu,0.5,nu+0.5,nv,0.5,nv+0.5);
      raww1 = new TH1F("raww1","raw signal after CDS",300,-150.,150.);
      hit0 = new TH2F("hit0","raw signal-pedestal",nu,0.5,nu+0.5,nv,0.5,nv+0.5);

      hit = new TH2F("hit","signal/noise",nu,-nu/2*tPixelPitchU,nu/2*tPixelPitchU,nv,-nv/2*tPixelPitchV,nv/2*tPixelPitchV);
      hnnois = new TH2F("hnnoise","noise",nu,0.5,nu+0.5,nv,0.5,nv+0.5);
      hNoisyChannels = new TH2F("hnnois","noise",nu,0.5,nu+0.5,nv,0.5,nv+0.5);

      hnnois1 = new TH1F("hnnois1","noise",numax*nvmax,0,float(numax*nvmax));
      hraw1 = new TH1F("hraw1","raw signal before CDS",numax*nvmax,0,float(numax*nvmax));
      hson1 = new TH1F("hson1","signal/noise",numax*nvmax,0,float(numax*nvmax));
      hson = new TH1F("hson","signal/noise",400,-100.,100.0);
      //--------
      hson_iterated = new TH1F("hson_iterated","signal/noise iterated",iterated_bin_number,iterated_xmin_number,iterated_xmax_number);
      hson_iterated2 = new TH1F("hson_iterated2","signal/noise it., no border",iterated_bin_number,iterated_xmin_number,iterated_xmax_number);  //ADAPTED FOR MI5 !!!!!!!!
      hson_iterated3 = new TH1F("hson_iterated3","signal/noise it., no seed",iterated_bin_number,iterated_xmin_number,iterated_xmax_number);  //ADAPTED FOR MI5 !!!!!!!!
      hson_iterated4 = new TH1F("hson_iterated4","signal/noise it., no hits",iterated_bin_number,iterated_xmin_number,iterated_xmax_number);  //ADAPTED FOR MI5 !!!!!!!!
      hson_substract2_3 = new TH1F(" hson_substract2_3","signal/noise it., only seed",iterated_bin_number,iterated_xmin_number,iterated_xmax_number);  //ADAPTED FOR MI5
      hson_substract3_4 = new TH1F(" hson_substract3_4","signal/noise it., only neighbours",iterated_bin_number,iterated_xmin_number,iterated_xmax_number);
      hhitmap = new TH2F("hhitmap","accumlutaed hit map",nu,-nu/2*tPixelPitchU,nu/2*tPixelPitchU,nv,-nv/2*tPixelPitchV,nv/2*tPixelPitchV);

      hpu = new TH1F("hpu","signal",100,-50.,50.);
      hpu1 = new TH1F("hpu1","signal",numax*nvmax,0,float(numax*nvmax));
      hnoisd = new TH1F("hnoisd","noise",250,0,12.5);
      hped = new TH1F("ped","pedestal",numax*nvmax,1,float(numax*nvmax+1));
      hpedd = new TH1F("pedd","pedestal distribution",100,-10,10);

      startMIMOSA=kFALSE;
    }
    else{
      raw1->Reset();
      raw2->Reset();
      raww->Reset();
      raww1->Reset();
      hit0->Reset();
      hit->Reset();
      hnnois->Reset();
      hNoisyChannels->Reset();
      hnnois1->Reset();
      hraw1->Reset();
      hson1->Reset();
      hson->Reset();
      hpu->Reset();
      hpu1->Reset();
      hnoisd->Reset();
      hped->Reset();
      hpedd->Reset();
      raw1->Reset();
    }
    Int_t ist = 1;

    //    if(nh!=0){
      for (Int_t iv=1 ; iv<=nv; iv++){
	for (Int_t iu=1 ; iu<=nu; iu++){
	  Float_t value = thePlM->GetRawValue(ist);
	  if(value>0x0fff)value-=65535;
	  Float_t fr1 = thePlM->GetRawFrame1Value(ist);
	  Float_t fr2 = thePlM->GetRawFrame2Value(ist);
	  Float_t noiseval = thePlM->GetNoise(ist);
	  if(noiseval > NoiseCut){
	    hNoisyChannels->Fill(iu,iv,1.);
	  }
	  Float_t pulse = thePlM->GetPulseHeight(ist);
	  //    printf("Noiseval= %f\n",noiseval);
	  raww->Fill(iu,iv,value);
	  raww1->Fill(value);
	  raw1->Fill(iu,iv,fr1);
	  raw2->Fill(iu,iv,fr2);
	  if (noiseval<0.000010) {
	    //    printf ("pixel %f : Noise = %f\n",ist,noiseval);
	    noiseval=0.01 ;
	  }
	  if (noiseval>100) {
	    //  printf ("pixel %f : Noise = %f\n",ist,noiseval);
	  }
	  if (pulse/noiseval<-1.0) hnnois->Fill(iu,iv,1);
	  hnnois->Fill(iu,iv,noiseval);
	  hit0->Fill(iu,iv,pulse);
	  hit->SetBinContent(iu,iv,pulse/noiseval);
	  hraw1->Fill(ist,fr1);

	  if((iu<=numax)&&(iv<=nvmax)){
	    hnnois1->Fill(ist,noiseval);
	    hson1->Fill(ist,pulse/noiseval);
	    hpu->Fill(pulse);
	    hpu1->Fill(ist,pulse);
	    hped->Fill(ist,thePlM->GetPedestal(ist));/*thePlM->GetPedestal(ist)*/
	    hpedd->Fill(thePlM->GetPedestal(ist));
	  }
	  hnoisd->Fill(noiseval);
	  //----------single event S/N:
	  hson->Fill(pulse/noiseval);

	  //----------&-----------------------------------------------------------------
	  // fake studi&es
	  //---------------------------------------------------------------------------

	  //---------iterated S/N:
	  hson_iterated->Fill(pulse/noiseval);
	  //----------iterated S/N: remove border regions.
	  if((iu >  xminpixel) && ( iu < xmaxpixel ) && (iv > 1) && (iv < nv) ){
	    hson_iterated2->Fill(pulse/noiseval);
	  }
	  Float_t tempxpos = tPixelPitchU * float(iu-1) - (tPixelPitchU * float(nu) / 2.0 ) + tPixelPitchU / 2.0;
	  Float_t tempypos = tPixelPitchV * float(iv-1) - (tPixelPitchV * float(nv) / 2.0 ) + tPixelPitchV / 2.0;
	  Int_t excluded = 0;
	  for (Int_t ih=0; ih<nh ;ih++){
	    if( (abs(Hitarray[ih][0] - tempxpos) < hitseedcutsize) && (abs(Hitarray[ih][1] - tempypos) < hitseedcutsize) ){
	      excluded = 1;
	    }
	  }
	  Int_t excluded2 = 0;
	  for (Int_t ih=0; ih<nh ;ih++){
	    if( (abs(Hitarray[ih][0] - tempxpos) < hitregioncutsize) && (abs(Hitarray[ih][1] - tempypos) < hitregioncutsize) ){
	      excluded2 = 1;
	    }
	  }

	  if((iu >  xminpixel) && ( iu < xmaxpixel ) && (iv > 1) && (iv < nv) && (excluded == 0) ){
	    hson_iterated3->Fill(pulse/noiseval);
	  }
	  if((iu >  xminpixel) && ( iu < xmaxpixel ) && (iv > 1) && (iv < nv) && (excluded2 == 0) ){
	    hson_iterated4->Fill(pulse/noiseval);
	  }
	  if((iu >  xminpixel) && ( iu < xmaxpixel ) && (iv > 1) && (iv < nv) && (excluded == 1) ){
	    cout<<"hson_substract2_3 iu iv xpos ypos SN "<<iu <<" "<<iv <<" "<<tempxpos<<" "<<tempypos<<" "<<pulse/noiseval<<endl;
	    hson_substract2_3->Fill(pulse/noiseval);
	  }
	  if((iu >  xminpixel) && ( iu < xmaxpixel ) && (iv > 1) && (iv < nv) && (excluded2 == 1) ){
	    hson_substract3_4->Fill(pulse/noiseval);
	  }

	  // END OF fake studies

	  ist++ ;
	}
      }

      for (Int_t ih=0; ih<nh ;ih++){
	hhitmap->Fill(Hitarray[ih][0],Hitarray[ih][1],1.0);
      }

      //    }else{  //end of if no hits.
      //      cout<<"----------------------> No hit detected. So no plots are filled."<<endl;
      //    }
    /* for (ist=2095;ist<2100;ist++){
       cout << "pixel "<< ist << "pedestal = " <<thePlM->GetPedestal(ist)<< endl;}
    */

    cout << "pixel read " << ist << endl;

    cout << "pixel nu*nv " << nu*nv << endl;


    // usual PLOTS

    cnv->Clear();
    cnv->Divide(1,2);
    cnv->cd(1);
    hit->Draw("colz");
    pm->Draw();
    cnv->cd(2);
    raww->Draw("colz");
    cnv->Update();

    // END OF  usual PLOTS

    cout<<"end of mimosa display"<<endl;

  } // END OF NEVENTS LOOP

  fMDisplayDone=kTRUE;

}

//______________________________________________________________________________
//
void MRaw::InspectScan()
{

  // scans eventsn.
  // A besson september 2003

  //if(!gTAF->CheckIfDone("init")) return;
  if(!CheckIfDone("init,display")) return;
  //run it AFTER MIMOSA DISPLAY


  cnv->Clear();
  cnv->UseCurrentStyle();
  cnv->Divide(3,3);
  cnv->cd(1);
  raw1->Draw("colz");
  cnv->cd(2);
  raww->Draw("colz");
  cnv->cd(3);
  raww1->Draw();
  raww1->SetStats(111111);
  raww1->Fit("gaus");
  cnv->cd(4);
  hit->Draw("colz");
  cnv->cd(5);
  hnnois->Draw("colz");
  cnv->cd(6);
  raw2->Draw("colz");
  Int_t Binnumber = 262144; //  MI5 512 x 512
  Float_t Average = 0.0;

  for ( Int_t i=1 ; i<= Binnumber; i++) {
    Average += hraw1->GetBinContent(i) ;
  }

  Average = Average / float(Binnumber);
  cout<<" AVERAGE = "<<Average<<endl;

  cnv->cd(7);
  hnoisd->Draw();
  cnv->cd(8);
  hpu1->Draw();
  cnv->cd(9);

  hson->SetStats(111111);
  hson->Draw();
}

//______________________________________________________________________________
//
void MRaw::Inspectfake()
{
  //inspect fakes
  //abesson 05 jannuary 2005

  //if(!gTAF->CheckIfDone("init")) return;
  if(!CheckIfDone("init,display")) return;
  // requires call to init Session in order to run!


  gStyle->SetOptStat(kTRUE);
  gStyle->SetOptStat(111111);

  char titre[50] ;
  sprintf(titre,"Run %d  Event POS %d",fSession->GetRunNumber(),fSession->GetCurrentEventNumber());
  TCanvas *cinspectfake = new TCanvas("cinspectfake", titre,400,0,1200,700);
  cinspectfake->UseCurrentStyle();


  cinspectfake->Clear();
  cinspectfake->UseCurrentStyle();
  cinspectfake->Divide(4,3);


  cinspectfake->cd(1);
  gPad->SetLogy(1);
  hson_iterated->SetAxisRange(-6,35,"X");
  gStyle->SetOptStat(1111111);
  if(!hson_iterated->GetEntries()) {Warning("InspectFake()","No entry, bad event?"); return;}
  hson_iterated->Draw();
  TF1 *fiterated = new TF1("fiterated","gaus",-5,20);
  if(fiterated) hson_iterated->Fit("fiterated","RF");
  gStyle->SetOptStat(1111111);

  cinspectfake->cd(2);
  gPad->SetLogy(1);
  hson_iterated2->SetAxisRange(-6,35,"X");
  hson_iterated2->Draw();
  TF1 *fiterated2 = new TF1("fiterated2","gaus",-5,20);
  if(fiterated2) hson_iterated2->Fit("fiterated2","RF");
  gStyle->SetOptStat(1111111);

  cinspectfake->cd(3);
  gPad->SetLogy(1);
  hson_iterated3->SetAxisRange(-6,35,"X");
  hson_iterated3->Draw();
  TF1 *fiterated3 = new TF1("fiterated3","gaus",-5,20);
  if(fiterated3) hson_iterated3->Fit("fiterated3","RF");
  gStyle->SetOptStat(1111111);

  cinspectfake->cd(4);
  gPad->SetLogy(1);
  hson_iterated4->SetAxisRange(-6,35,"X");
  hson_iterated4->Draw();
  TF1 *fiterated4 = new TF1("fiterated4","gaus",-5,20);
  hson_iterated4->Fit("fiterated4","RF");
  gStyle->SetOptStat(1111111);

  //--------
  TH1F *hson_iteratedClone = (TH1F*)hson_iterated->Clone("hson_iteratedClone");
  TH1F *hson_iteratedClone2 = (TH1F*)hson_iterated2->Clone("hson_iteratedClone2");
  TH1F *hson_iteratedClone3 = (TH1F*)hson_iterated3->Clone("hson_iteratedClone3");
  TH1F *hson_iteratedClone4 = (TH1F*)hson_iterated4->Clone("hson_iteratedClone4");

  cinspectfake->cd(5);
  gPad->SetLogy(1);
  hson_iteratedClone->SetAxisRange(-5,35,"X");
  hson_iteratedClone->SetNormFactor(1);
  hson_iteratedClone->Draw();
  gStyle->SetOptStat(1111111);

  cinspectfake->cd(6);
  gPad->SetLogy(0);
  hson_iteratedClone2->SetAxisRange(-5,35,"X");
  hson_iteratedClone2->SetNormFactor(1);
  hson_iteratedClone2->Draw();
  gStyle->SetOptStat(1111111);

  cinspectfake->cd(7);
  gPad->SetLogy(0);
  hson_iteratedClone3->SetAxisRange(-5,35,"X");
  hson_iteratedClone3->SetNormFactor(1);
  hson_iteratedClone3->Draw();
  gStyle->SetOptStat(1111111);

  cinspectfake->cd(8);
  gPad->SetLogy(0);
  hson_iteratedClone4->SetAxisRange(-5,35,"X");
  hson_iteratedClone4->SetNormFactor(1);
  hson_iteratedClone4->Draw();
  gStyle->SetOptStat(1111111);

  cinspectfake->cd(9);
  gPad->SetLogy(0);
  hson_substract2_3->SetAxisRange(0,80,"X");
  hson_substract2_3->Draw();
  gStyle->SetOptStat(1111111);

  cinspectfake->cd(10);
  gPad->SetLogy(1);
  hson_substract3_4->SetAxisRange(-6,80,"X");
  hson_substract3_4->Draw();
  gStyle->SetOptStat(1111111);
  gStyle->SetOptFit();

  cinspectfake->cd(11);
  hhitmap->Draw("colz");

  //-----compare integrals:
  const Int_t number_of_integral = 7;
  Float_t Integral_cut[number_of_integral] = {3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0};

  for (Int_t ii=0; ii<number_of_integral ;ii++){
    Int_t borne_min = (Int_t) float( float(Integral_cut[ii] - iterated_xmin_number)
				     / float(iterated_xmax_number - iterated_xmin_number) * float(iterated_bin_number));
    cout<<"-----------------"<<endl;
    cout<<"borne_min Integral_cut[ii]"<<borne_min<<" "<<Integral_cut[ii]<<endl;
    cout<<"hson_iterated4 fiterated4  "<<hson_iterated4->Integral( borne_min,iterated_bin_number)<<" "<<fiterated4->Integral(Integral_cut[ii],20)<<endl;
    Float_t fitintegralvalue, histointegralvalue;
    fitintegralvalue=histointegralvalue=0;

    if( ( (hson_iterated4->Integral(0,iterated_bin_number) ) != 0.0) && (fiterated4->Integral(-5,20) ) ){
      histointegralvalue=hson_iterated4->Integral( borne_min,iterated_bin_number) / hson_iterated4->Integral(0,iterated_bin_number);
      fitintegralvalue=fiterated4->Integral(Integral_cut[ii],20) / fiterated4->Integral(-5,20);
    }
    cout<<"NORMALISED -> HISTO FIT "<<histointegralvalue<<"  "<<fitintegralvalue<<endl;
  }
  cout<<"il faut normaliser les 2 integrales separement !"<<endl;
}



//______________________________________________________________________________
//
void MRaw::MimosaJump(Int_t Njump /* = 10 */ )
{
  // Jumps a given number of events (10 by default) while scanning raw events.
  // A.Besson, August 2004.


  //if(!gTAF->CheckIfDone("init")) return;
  if(!CheckIfDone("init")) return;
  // requires call to init Session in order to run!

  if (!CheckIfDone("init")) return;

  DTracker *tTracker  =  fSession->GetTracker();

  for (Int_t ii=0 ; ii<Njump; ii++){
    fSession->NextRawEvent();
    fSession->SetStatus(1);

    tTracker->Update();
    cout<<ii+1<<" events jumped..."<<endl;
  }
  cout<<"... You have just jumped "<<Njump<<" events (POS counter). Enjoy your scan."<<endl;

}


//______________________________________________________________________________
//
void MRaw::SetMultiFrame( Int_t aPlane, Int_t nFrames )
{
  // Set the plane concerns by multi-frame,
  //  meaning one event contains  frames of this plane.
  //
  // Note there can be only one such plane currently (July 2013).
  //
  // JB 2013/07/18

  MultiFramePlane = aPlane;
  MultiFrameNFrames = nFrames;

  printf("\nMRaw: MULTIFRAME option set for plane %d with %d frames.\n\n", MultiFramePlane, MultiFrameNFrames);

}

//______________________________________________________________________________
//
void MRaw::SetNbinsReductionFactor( Float_t aFactor)
{
  // Reduce the number of bins in "map" histograms
  // Used in DisplayRawData2D
  //
  // JB 2014/12/08

  if( 0.<aFactor && aFactor<1. ) {
    NbinsReductionFactor = aFactor;
  }
  else {
    NbinsReductionFactor = 1.;
  }
  printf("\nMRaw: In \"map\" histograms, the number of bins (usually equal to #pixels) is reduced by a factor %.2e.\n\n", NbinsReductionFactor);

}

//______________________________________________________________________________
//
void MRaw::InspectRawChannels( Int_t nEvents, Int_t thePlaneNumber, Int_t firstChannel, Int_t nChannels, Int_t maxAutoCorr)
{
  // Display the raw data for each plane cumulated over the requested number of events.
  // minSN indicates the lowest value of the pixel S/N to display the pixel.
  // occurences_min/_max are occurence limits used in the noisy pixel study.
  // If "storeOccurence" is used, a text file will be created with the position
  //  (column and row) of all pixels above the specified "minOccurence*nEvents".
  //
  // JB 2012/09/20

  fSession->SetEvents(nEvents);

  DTracker *tTracker  =  fSession->GetTracker();
  DPlane* thePlane = tTracker->GetPlane( thePlaneNumber);

  Double_t pedestal, noise;

  // Canvas and histos for Raw Data map
  TCanvas **cChannel = new TCanvas*[nChannels];
  TH1F    **hChannelEvt = new TH1F*[nChannels];
  TH1F    **hChannelDis = new TH1F*[nChannels];
  TH1D    **hChannelAutoCorr = new TH1D*[nChannels];
  Char_t name[50], title[100];

  /*TObject* g;
  gROOT->FindObject("ccumulrd") ;
  if (g) { ccumulrd = (TCanvas*)g; }
  else {
    ccumulrd = new TCanvas("ccumulrd", "Cumulate RawData", 5, 5,800,700);
  }
  ccumulrd->Clear();
  ccumulrd->UseCurrentStyle();*/

  for( Int_t iChannel=0; iChannel<nChannels; iChannel++) {

    sprintf(name, "cPl%dChan%d", thePlaneNumber, iChannel);
    sprintf(title, "Run %d, plane %d, channel %d", fSession->GetRunNumber(), thePlane->GetPlaneNumber(), iChannel+firstChannel);
    cChannel[iChannel] = new TCanvas( name, title, 5*iChannel, 5*iChannel,800,700);

    sprintf( name, "hevtchan%d", iChannel+firstChannel);
    sprintf( title, "Evolution of channel %d of plane %d", iChannel+firstChannel, thePlaneNumber);
    hChannelEvt[iChannel] = new TH1F(name, title, nEvents, 0, nEvents);
    hChannelEvt[iChannel]->SetXTitle("event#");
    hChannelEvt[iChannel]->SetYTitle("raw value");
    hChannelEvt[iChannel]->SetMarkerStyle(20);
    hChannelEvt[iChannel]->SetMarkerSize(.4);
    hChannelEvt[iChannel]->SetMarkerColor(1);
    hChannelEvt[iChannel]->SetStats(kFALSE);

    sprintf( name, "hdischan%d", iChannel+firstChannel);
    sprintf( title, "Distribution of channel %d of plane %d", iChannel+firstChannel, thePlaneNumber);
    hChannelDis[iChannel] = new TH1F(name, title, 100, 0., 0.);
    hChannelDis[iChannel]->SetXTitle("raw value");

    sprintf( name, "hautocorrchan%d", iChannel+firstChannel);
    sprintf( title, "Autocorrelation of channel %d of plane %d", iChannel+firstChannel, thePlaneNumber);
    hChannelAutoCorr[iChannel] = new TH1D(name, title, maxAutoCorr, 0, maxAutoCorr);
    hChannelAutoCorr[iChannel]->SetXTitle("#evts");
    hChannelAutoCorr[iChannel]->Sumw2();

    printf( "Histos #%d, created for plane %d and channel %d\n", iChannel, thePlaneNumber, iChannel+firstChannel);
  }


  //Loop over the requested number of events
  for( Int_t iEvt=0; iEvt < nEvents; iEvt++) {

    if( !fSession->NextRawEvent() ) break;
    tTracker->Update();
    for( Int_t iChannel=0; iChannel<nChannels; iChannel++) {
      //printf( "Evt %d, getting value %d for channel %d\n", iEvt, iChannel, iChannel+firstChannel);
      hChannelEvt[iChannel]->SetBinContent( iEvt+1, thePlane->GetRawValue(iChannel+firstChannel));
      hChannelDis[iChannel]->Fill( thePlane->GetRawValue(iChannel+firstChannel));
    } // end loop on channels

  } // end loop over events


  // Compute autocorrelation,
  //  that is autocorr(n) = sqrt( < h[i]*h[i+n] > ) (average on i)
  //  here n=iEvt and i=iBin
  for( Int_t iChannel=0; iChannel<nChannels; iChannel++) { // loop on channels

    pedestal = hChannelDis[iChannel]->GetMean();
    noise    = hChannelDis[iChannel]->GetRMS();

    for( Int_t iEvt=0; iEvt < maxAutoCorr; iEvt++) {

      for( Int_t iBin=1; iBin<=hChannelEvt[iChannel]->GetNbinsX(); iBin++ ) {
        if( iBin+iEvt<=hChannelEvt[iChannel]->GetNbinsX() ) {
          hChannelAutoCorr[iChannel]->Fill( iEvt, (hChannelEvt[iChannel]->GetBinContent(iBin)-pedestal) * (hChannelEvt[iChannel]->GetBinContent(iBin+iEvt)-pedestal) /(hChannelEvt[iChannel]->GetNbinsX()-iEvt) );
        }
      }

    }

    hChannelAutoCorr[iChannel]->Scale(1./noise/noise); // allows to have autocorr=1 at 0

  } // end loop on channels

  // Now display
  for( Int_t iChannel=0; iChannel<nChannels; iChannel++) { // loop on channels

    pedestal = hChannelDis[iChannel]->GetMean();
    noise    = hChannelDis[iChannel]->GetRMS();
    hChannelEvt[iChannel]->SetMinimum(pedestal-10*noise);
    hChannelEvt[iChannel]->SetMaximum(pedestal+10*noise);

    cChannel[iChannel]->cd();
    cChannel[iChannel]->Divide(3,1);
    cChannel[iChannel]->cd(1);
    hChannelEvt[iChannel]->DrawCopy("P");
    cChannel[iChannel]->cd(2);
    hChannelDis[iChannel]->DrawCopy();
    cChannel[iChannel]->cd(3);
    hChannelAutoCorr[iChannel]->DrawCopy("e");

  } // end loop on channels

}

//______________________________________________________________________________
//
void MRaw::DisplayRawData( Float_t nChanToDisplay)
{
  // Simply display the raw data (after CDS) for each plane event by event
  // nChanToDisplay is the proportion of channels to display
  // Call by gTAF->GetRaw()->DisplayRawData()
  // JB, June 2007,
  // modified 2008/10/15
  // modified JB, August 2012, add hit and track display
  // modified JB, 2013/08/21
  // modified JB, 2013/08/27 stripIndex shifted by +1 for display, since starts at 0

  fSession->SetEvents(3);

  TCanvas *cdisplayraw;
  TObject* g = gROOT->FindObject("cdisplayraw") ;
  if (g) {
    cdisplayraw = (TCanvas*)g;
  }
  else {
    cdisplayraw = new TCanvas("cdisplayraw", "Inspect Raw Data", 5, 5, 800, 700);
  }
  cdisplayraw->Clear();
  cdisplayraw->UseCurrentStyle();
  TPaveLabel* label = new TPaveLabel();
  Char_t canvasTitle[200];
  TPad *pad = new TPad("pad","",0.,0.,1.,0.965);
  pad->Draw();

  DTracker *tTracker  =  fSession->GetTracker();
  Int_t nPlanes = tTracker->GetPlanesN();
  pad->Divide( (Int_t)ceil(nPlanes/2.), 2);

  DPlane* tPlane;
  Int_t *nChanToDisplayPl = new Int_t[nPlanes];
  TH1F **hRawData = new TH1F*[nPlanes];
  TH1F **hHitIndex = new TH1F*[nPlanes];
  TH1F **hTrackIndex = new TH1F*[nPlanes];
  TH1F **hHitInTrackIndex = new TH1F*[nPlanes];
  Char_t name[50], title[100];
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    tPlane = tTracker->GetPlane(iPlane);
    // below 1000 channels, display everything,
    // otherwise use the reduction factor
    if( tPlane->GetChannelsN()<1000 ) {
      nChanToDisplayPl[iPlane-1] =  (Int_t)(tPlane->GetChannelsN());
    }
    else {
      nChanToDisplayPl[iPlane-1] =  (Int_t)(tPlane->GetChannelsN()*nChanToDisplay);
    }
    sprintf( name, "hrawdatapl%d", iPlane);
    sprintf( title, "Raw data of plane %d, %s", iPlane, tPlane->GetPlaneName());
    hRawData[iPlane-1] = new TH1F(name, title, nChanToDisplayPl[iPlane-1], 1, nChanToDisplayPl[iPlane-1]+1);
    hRawData[iPlane-1]->SetStats(kFALSE);
    if( iPlane<=8) hRawData[iPlane-1]->SetMaximum(240);
    //printf( "MRaw::DisplayRawData created %s histo with %d channels\n", name, nChanToDisplayPl[iPlane-1]);

    sprintf( name, "hhitindexpl%d", iPlane);
    sprintf( title, "Hit index of plane (%d) %s", iPlane, tPlane->GetPlanePurpose());
    hHitIndex[iPlane-1] = new TH1F(name, title, tPlane->GetStripsNu(), 0, tPlane->GetStripsNu());
    hHitIndex[iPlane-1]->SetLineColor(2);

    sprintf( name, "htrackindexpl%d", iPlane);
    sprintf( title, "Track index of plane (%d) %s", iPlane, tPlane->GetPlanePurpose());
    hTrackIndex[iPlane-1] = new TH1F(name, title, tPlane->GetStripsNu(), 0, tPlane->GetStripsNu());
    hTrackIndex[iPlane-1]->SetLineColor(3);
    hTrackIndex[iPlane-1]->SetLineStyle(2);

    sprintf( name, "hhitintrackpl%d", iPlane);
    sprintf( title, "Hit in track index of plane (%d) %s", iPlane, tPlane->GetPlanePurpose());
    hHitInTrackIndex[iPlane-1] = new TH1F(name, title, tPlane->GetStripsNu(), 0, tPlane->GetStripsNu());
    hHitInTrackIndex[iPlane-1]->SetLineColor(4);

  }

  fSession->NextRawEvent();
  //cout << "DisplayRawData --> Event OK" << endl;
  tTracker->Update();
  //cout << "DisplayRawData --> Analysis OK" << endl;

  sprintf(canvasTitle, "Run %d Event %d", fSession->GetRunNumber(), fSession->GetCurrentEventNumber()-1);
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);

  Int_t index; // start at 0, so for Setting Bin, use index+1
  Float_t value;
  DStrip *aSeed;
  DPixel *aPSeed;
  DTrack *aTrack;
  DPlane *aPlane;
  DHit   *aHit;
  //Bool_t planeZRotated = kFALSE;

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    tPlane = tTracker->GetPlane(iPlane);
    if( tPlane->GetReadout()==0 || tPlane->GetAnalysisMode()>1 ) continue; // PIXEL case removed due to crash, JB 2013/08/21
    //cout << "Asking for raw values of plane " << iPlane <<endl;

    if(fVerbose) printf("MRaw::DisplayRawData Displaying for plane %d with tiltZ=%.2f\n", tPlane->GetPlaneNumber(), tPlane->GetTilt()(0)*TMath::RadToDeg());

    // Determine plane orientation in the X-Y plane
    //if ( (80.<tPlane->GetTilt()(0)*TMath::RadToDeg() && tPlane->GetTilt()(0)*TMath::RadToDeg()<100.)
    //    || (260.<tPlane->GetTilt()(0)*TMath::RadToDeg() && tPlane->GetTilt()(0)*TMath::RadToDeg()<280.) ) {
    //  planeZRotated = kTRUE;
    //}
    //else {
    //  planeZRotated = kFALSE;
    //}

    if ( tPlane->GetReadout() >= 100 ){ // if sparsified readout
     std::vector<DPixel*> *aList = tPlane->GetListOfPixels();
      for( Int_t iPix=0; iPix<(Int_t)aList->size(); iPix++) { // loop on fired pixels
        index = aList->at(iPix)->GetPixelIndex();
        hRawData[iPlane-1]->SetBinContent( index, aList->at(iPix)->GetPulseHeight());
        //if(fVerbose) printf("MRaw::DisplayRawData  pl %d, pixel[%d]=%f\n", iPlane, index, aList->at(iPix)->GetPulseHeight());
      } //end loop on fired pixels
    } // end if sparsified readout

    else { // if non-sparsified readout
      for( Int_t iStrip=1; iStrip<nChanToDisplayPl[iPlane-1]; iStrip++) {
        hRawData[iPlane-1]->SetBinContent( iStrip, tPlane->GetRawValue(iStrip));
        //if(fVerbose) printf("MRaw::DisplayRawData  pl %d, strip[%d]=%f\n", iPlane, iStrip, tPlane->GetRawValue(iStrip));
      } //end loop on strips
    } // end if non-sparsfied readout

    if(fVerbose) cout << "Plane " << iPlane << " has " << tPlane->GetHitsN() << " hits reconstructed" << endl;
    for( Int_t iHit=1; iHit<=tPlane->GetHitsN(); iHit++) { //loop on hits (starts at 1 !!)
      // Separate case of STRIP and PIXEL, JB 2013/08/21
      if( tPlane->GetAnalysisMode()<2 ) { // if STRIP case
        //cout << "getting hit " << iHit << " and strip seed pointer is " << ((tPlane->GetHit( iHit)->GetSeed())!=NULL?"OK":"NULL!") << endl;
        aSeed = (DStrip*)(tPlane->GetHit( iHit)->GetSeed());
        index = aSeed->GetStripIndex()+1; // since strip indexing start at 0, JB 2013/08/27
        value = aSeed->GetRawValue()/2.; // division by 2 allows nicer display
        if( aSeed==NULL ) {
          index = 0;
          value = 0.;
          Warning("MRaw","Plane %d, hit %d point to null strip seed!", iPlane, iHit);
        }
      } // end if STRIP case
      else { // if PIXEL case
        //cout << "getting hit " << iHit << " and pixel seed pointer is " << ((tPlane->GetHit( iHit)->GetPSeed())!=NULL?"OK":"NULL!") << endl;
        aPSeed = (DPixel*)(tPlane->GetHit( iHit)->GetPSeed());
        index = aPSeed->GetPixelIndex();
        value = aPSeed->GetRawValue()/2.; // division by 2 allows nicer display
        if( aSeed==NULL ) {
          index = 0;
          value = 0.;
          Warning("MRaw","Plane %d, hit %d point to null pixel seed!", iPlane, iHit);
        }
      } // end if PIXEL case
      hHitIndex[iPlane-1]->SetBinContent( index, value); // JB 2009/08/31
      if(fVerbose) printf("MRaw::DisplayRawData  plane %d, hit[%d] index = %d rawValue = %.1f.\n", iPlane, iHit, index, value);
    } //end loop on hits

    if(fVerbose) cout << "Tracker has " << tTracker->GetTracksN() << " tracks reconstructed" << endl;
    for( Int_t iTrack=1; iTrack<=tTracker->GetTracksN(); iTrack++ ) { // loop on tracks
      aTrack = tTracker->GetTrack(iTrack);
      // For strip (1D) detectors
      if ( tPlane->GetAnalysisMode()==1 ) {
          index = (Int_t)tPlane->Intersection(aTrack)(0)/tPlane->GetStripPitch()(0)+tPlane->GetStripsNu()/2;
          hTrackIndex[iPlane-1]->Fill( index , hRawData[iPlane-1]->GetMaximum()*1.);
          if(fVerbose) printf("MRaw::DisplayRawData2D  pl %d, track[%d] Upos = %1f Index = %d Chi2 = %.1f.\n", iPlane, iTrack, tPlane->Intersection(aTrack)(0), index, aTrack->GetChiSquare());

        // Also display hit belonging to track, JB 2012/08/28
        for( Int_t iHit=0; iHit<aTrack->GetHitsNumber(); iHit++ ) { // loop on the hits in the track
          aHit   = (DHit*)aTrack->GetHit( iHit);
          aPlane = (DPlane*)aHit->GetPlane();
          if( aPlane->GetPlaneNumber() == tPlane->GetPlaneNumber() ) {
            aSeed = (DStrip*)aHit->GetSeed();
            hHitInTrackIndex[iPlane-1]->SetBinContent( aSeed->GetStripIndex()+1, aSeed->GetRawValue()/2.);
            if(fVerbose) printf("MRaw::DisplayRawData  plane %d, hit[%d] in track at index = %d rawValue = %.1f.\n", iPlane, iHit, aSeed->GetStripIndex()+1, aSeed->GetRawValue());
          }
        } // end loop on the hits in the track

      }
      // For pixels (2D) detectors
      //  fill both X and Y positions
      else {
        index = (Int_t)tPlane->Intersection(aTrack)(0)/tPlane->GetStripPitch()(0)+tPlane->GetStripsNu()/2;
        hTrackIndex[iPlane-1]->Fill( index , hRawData[iPlane-1]->GetMaximum()*1.);
        index = (Int_t)tPlane->Intersection(aTrack)(1)/tPlane->GetStripPitch()(1)+tPlane->GetStripsNv()/2;
        hTrackIndex[iPlane-1]->Fill( index , hRawData[iPlane-1]->GetMaximum()*.6);
        if(fVerbose) printf("MRaw::DisplayRawData2D  pl %d, track[%d] = (%.1f,%.1f) in plane.\n", iPlane, iTrack
                            ,(Int_t)tPlane->Intersection(aTrack)(0)/tPlane->GetStripPitch()(0)+tPlane->GetStripsNu()/2
                            ,(Int_t)tPlane->Intersection(aTrack)(1)/tPlane->GetStripPitch()(1)+tPlane->GetStripsNv()/2 );
      }
    } // end loop on tracks

    pad->cd(iPlane);
    hRawData[iPlane-1]->DrawCopy();
    hHitIndex[iPlane-1]->DrawCopy("same");
    hHitIndex[iPlane-1]->Delete();
    hHitInTrackIndex[iPlane-1]->DrawCopy("same");
    hHitInTrackIndex[iPlane-1]->Delete();
    hTrackIndex[iPlane-1]->DrawCopy("same");
    hTrackIndex[iPlane-1]->Delete();
    hRawData[iPlane-1]->DrawCopy("same");
    hRawData[iPlane-1]->Delete();

  } //end loop on planes
  cout << "\n Event number : " << fSession->GetCurrentEventNumber()-1<<" ";

  cdisplayraw->Update();

}

//
//______________________________________________________________________________
void MRaw::DisplayRawData2D( Float_t minSN, Bool_t withHits, Bool_t withTracks, Int_t triggerNb, Int_t clusterMultiplicity,
			     double Rminu,
			     double Rmaxu,
			     double Rminv,
			     double Rmaxv)
{
  // Simply display the raw data (after CDS) for each plane event by event
  //  pixel values below minSN are not displayed.
  //
  // If a trigger number is specified, the event corresponding to it
  //  will be displayed.
  //
  // If withHits>0 (default):
  //   pixels belonging to a reconstructed cluster(hit) are marked with a "X"
  //   the cluster reconstructed position is indicated by a "O"
  //
  // If withTracks>0 (default):
  //   the track extrapolated position is indicated by a cross marker
  //
  // The behavior of this method could be altered by globale variables:
  //  o NbinsReductionFactor: to reduce the #bins
  //      set with gTAF->GetRaw()->SetNbinsReductionFactor( yourFactor)
  //  o MultiFramePlane: to plot event with multiframes
  //      set with gTAF->GetRaw()->SetMultiFrame( aPlane, nFrames )
  //
  // Call by gTAF->GetRaw()->DisplayRawData2D()
  //
  // ATTENTION results can be very different if you choose to display:
  //  o Signal (variable PulseHeight)
  //  o Signal/Noise (variable PulseHeightToNoise)
  //-> check SetBinContent method of hRawData histograms!
  //
  // JB, Jan 2008
  // Modified 2008/10/15, 2009/05/06
  // Last Modified RDM 2009/05/21
  // Last Modified JB 2009/08/31 binning of histos and bin index in filling
  // Last Modified JB 2009/09/18 directory name of outputfile
  // Last Modified JB 2010/10/25 display reconstructed hits on top of raw data
  // Last Modified JB 2013/05/26 test readout not 0
  // Last Modified AP 2014/07/   more accurate cluster representation

  fSession->SetEvents(3);

  TObject* gtmp;

  TCanvas *cdisplayraw;
  TObject* g = gROOT->FindObject("cdisplayraw") ;
  if (g) {
    cdisplayraw = (TCanvas*)g;
  }
  else {
    cdisplayraw = new TCanvas("cdisplayraw", "Inspect Raw Data", 5, 5, 800, 700);
  }
  cdisplayraw->Clear();
  cdisplayraw->UseCurrentStyle();
  cdisplayraw->cd();
  TPaveLabel* label = new TPaveLabel();
  cdisplayraw->cd();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,"");
  //TPad *pad = new TPad("pad","",0.,0.,1.,0.965);
  //pad->Draw();

  DTracker *tTracker  =  fSession->GetTracker();
  Int_t nPlanes = tTracker->GetPlanesN();
  //pad->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);

  TCanvas *cdisplaymultiframe = NULL;
  TPaveLabel* labelframe = NULL;
  TPad *padframe = NULL;
  if( MultiFramePlane>0 ) {
    g = gROOT->FindObject("cdisplaymultiframe") ;
    if (g) {
      cdisplaymultiframe = (TCanvas*)g;
    }
    else {
      cdisplaymultiframe = new TCanvas("cdisplaymultiframe", "Multi-frame plane", 15, 15, 800, 700);
    }
    cdisplaymultiframe->Clear();
    cdisplaymultiframe->UseCurrentStyle();
    cdisplaymultiframe->cd();

    int NPadCols,NPadRows;
    if(MultiFrameNFrames%3 == 0) {
      NPadCols = MultiFrameNFrames/3;
      NPadRows = 3;
    }
    else {
      NPadCols = (Int_t)ceil(MultiFrameNFrames/2.);
      NPadRows = (MultiFrameNFrames>1)?2:1;
    }

    padframe = new TPad("padframe","",0.,0.,1.,0.965);
    padframe->Draw();
    padframe->Divide(NPadCols,NPadRows);

    //cdisplaymultiframe->Divide(NPadCols,NPadRows);

    labelframe = new TPaveLabel();
    labelframe->DrawPaveLabel(0.3,0.97,0.7,0.9999,"");
  }

  // Multiframe histograms
  Char_t name[50], title[1000];
  DPlane* tPlane;
  DHit* aHit;
  DTrack* aTrack;
  TH2F **hRawDataMultiFrame = NULL;
  if( MultiFramePlane>0 ) { // loop on frames
    hRawDataMultiFrame = new TH2F*[MultiFrameNFrames];
    tPlane = tTracker->GetPlane(MultiFramePlane);
    for(int iFrame=0; iFrame<MultiFrameNFrames; iFrame++) {
      sprintf( name, "hrawdatapl%dfr%d", MultiFramePlane, iFrame);
      sprintf( title, "Raw data of plane %d frame %d", MultiFramePlane, iFrame);

      gtmp = NULL;
      gtmp = gROOT->FindObject(name);
      if(gtmp) {
	hRawDataMultiFrame[iFrame] = (TH2F*)gtmp;
      }
      else {
	hRawDataMultiFrame[iFrame] = new TH2F(name, title,
					      tPlane->GetStripsNu()*NbinsReductionFactor, 0-0.5, tPlane->GetStripsNu()-0.5,
					      tPlane->GetStripsNv()*NbinsReductionFactor, 0-0.5, tPlane->GetStripsNv()-0.5);
	hRawDataMultiFrame[iFrame]->SetMarkerStyle(20);
	hRawDataMultiFrame[iFrame]->SetMarkerSize(.4);
	hRawDataMultiFrame[iFrame]->SetMarkerColor(1);
	hRawDataMultiFrame[iFrame]->SetStats(kFALSE);
      }
    }
  } // end loop on frames

  const int Ncolors(10);
  int Colors_tmp[Ncolors];
  Colors_tmp[0] = kBlack;
  Colors_tmp[1] = kGreen+2;
  Colors_tmp[2] = kBlue;
  Colors_tmp[3] = kGray+2;
  Colors_tmp[4] = kOrange-3;
  Colors_tmp[5] = kCyan;
  Colors_tmp[6] = kMagenta;
  Colors_tmp[7] = kYellow;
  Colors_tmp[8] = kOrange;
  Colors_tmp[9] = kTeal+1;

  TLatex* latex = new TLatex();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.04);
  latex->SetTextColor(kBlue);

  TLatex* latex2 = new TLatex();
  latex2->SetTextAlign(12);
  latex2->SetTextSize(0.06);
  latex2->SetTextColor(kBlue);

  // FIXME Antonin Maire, 7 oct 2015, clang complains about this rough declaration used previously
  //   i.e. std::vector<double> _HitPositions_U[nPlanes];
  //        "variable length array of non-POD element type"
  //    And in fact, it does not understand it is a matrix and consider it has to deal with 1D array...
  //    --> proper ISO C++11 declaration to make clang 3.4 happy
  std::vector<std::vector<double> > _HitPositions_U        ;  _HitPositions_U        .resize(nPlanes);
  std::vector<std::vector<double> > _HitPositions_V        ;  _HitPositions_V        .resize(nPlanes);
  std::vector<std::vector<int> >    _HitColors             ;  _HitColors             .resize(nPlanes);
  std::vector<std::vector<int> >    _HitTS                 ;  _HitTS                 .resize(nPlanes);

  std::vector<std::vector<double> > _PixelsInHitPositions_U;  _PixelsInHitPositions_U.resize(nPlanes);
  std::vector<std::vector<double> > _PixelsInHitPositions_V;  _PixelsInHitPositions_V.resize(nPlanes);
  std::vector<std::vector<int> >    _PixelsInHitColors     ;  _PixelsInHitColors     .resize(nPlanes);
  std::vector<std::vector<int> >    _PixelsInHitTS         ;  _PixelsInHitTS         .resize(nPlanes);

  std::vector<std::vector<double> > _TrkPositions_U        ;  _TrkPositions_U        .resize(nPlanes);
  std::vector<std::vector<double> > _TrkPositions_V        ;  _TrkPositions_V        .resize(nPlanes);
  std::vector<std::vector<int> >    _TrkIdx                ;  _TrkIdx                .resize(nPlanes);
  // NOTE (Antonin) :
  //    set up the size of the 2D arrays  (nPlanes(fixed nb) x nTracks(changing from evt to evt...) )
  //    One resizes with the number of planes and push_back as needed as far as tracks are concerned

  // Plane histograms
  int    NGoodPlanes = 0;
  bool   PlotPlane[nPlanes];
  TH2F **hRawData = new TH2F*[nPlanes];
  TH2F **hTrackMapIndex = new TH2F*[nPlanes];
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planesw
    tPlane = tTracker->GetPlane(iPlane);
    if(tPlane->GetReadout() > 0) PlotPlane[iPlane-1] = true;
    else                         PlotPlane[iPlane-1] = false;
    if(PlotPlane[iPlane-1]) NGoodPlanes++;


    _HitPositions_U[iPlane-1].clear();
    _HitPositions_V[iPlane-1].clear();
    _HitColors[iPlane-1].clear();
    _HitTS[iPlane-1].clear();

    _PixelsInHitPositions_U[iPlane-1].clear();
    _PixelsInHitPositions_V[iPlane-1].clear();
    _PixelsInHitColors[iPlane-1].clear();
    _PixelsInHitTS[iPlane-1].clear();

    _TrkPositions_U[iPlane-1].clear();
    _TrkPositions_V[iPlane-1].clear();
    _TrkIdx[iPlane-1].clear();

    double R_RawData_U[2];
    int Nbins_RawData_U;
    double R_RawData_V[2];
    int Nbins_RawData_V;
    if((Rminu >= -0.5 && Rmaxu <= tPlane->GetStripsNu()-0.5) &&
       (Rminv >= -0.5 && Rmaxv <= tPlane->GetStripsNv()-0.5)) {
      R_RawData_U[0] = int(Rminu) - 0.5;
      R_RawData_U[1] = int(Rmaxu) + 0.5;
      Nbins_RawData_U = int(R_RawData_U[1] - R_RawData_U[0])*NbinsReductionFactor;

      R_RawData_V[0] = int(Rminv) - 0.5;
      R_RawData_V[1] = int(Rmaxv) + 0.5;
      Nbins_RawData_V = int(R_RawData_V[1] - R_RawData_V[0])*NbinsReductionFactor;
    }
    else {
      R_RawData_U[0] = -0.5;
      R_RawData_U[1] =  tPlane->GetStripsNu()-0.5;
      Nbins_RawData_U = tPlane->GetStripsNu()*NbinsReductionFactor;

      R_RawData_V[0] = -0.5;
      R_RawData_V[1] = tPlane->GetStripsNv()-0.5;
      Nbins_RawData_V = tPlane->GetStripsNv()*NbinsReductionFactor;
    }

    if(PlotPlane[iPlane-1]) {
      sprintf( name, "hrawdatapl%d", iPlane);
      sprintf( title, "Raw data of plane (%d) %s", iPlane, tPlane->GetPlanePurpose());
      gtmp = NULL;
      gtmp = gROOT->FindObject(name);
      if(gtmp) {
	hRawData[iPlane-1] = (TH2F*)gtmp;
      }
      else {
        hRawData[iPlane-1] = new TH2F(name, title,
                                      Nbins_RawData_U,R_RawData_U[0],R_RawData_U[1],
                                      Nbins_RawData_V,R_RawData_V[0],R_RawData_V[1]);
        hRawData[iPlane-1]->SetMarkerStyle(20);
        hRawData[iPlane-1]->SetMarkerSize(.4);
        hRawData[iPlane-1]->SetMarkerColor(1);
        hRawData[iPlane-1]->SetStats(kFALSE);
      }
    }

    if( withTracks && PlotPlane[iPlane-1]) {
      sprintf( name, "htrackmapipl%d", iPlane);
      sprintf( title, "Track map of plane (%d) %s", iPlane, tPlane->GetPlanePurpose());
      gtmp = NULL;
      gtmp = gROOT->FindObject(name);
      if(gtmp) {
        hTrackMapIndex[iPlane-1] = (TH2F*)gtmp;
      }
      else {
        hTrackMapIndex[iPlane-1] = new TH2F(name,
                                            title,
					    10*tPlane->GetStripsNu()*NbinsReductionFactor, 0-0.5, tPlane->GetStripsNu()-0.5,
                                            10*tPlane->GetStripsNv()*NbinsReductionFactor, 0-0.5, tPlane->GetStripsNv()-0.5);
        //hTrackMapIndex[iPlane-1]->SetName( name);
        //hTrackMapIndex[iPlane-1]->SetTitle( title);
        hTrackMapIndex[iPlane-1]->SetMarkerStyle(28);
        hTrackMapIndex[iPlane-1]->SetMarkerSize(2.0);
        //hTrackMapIndex[iPlane-1]->SetMarkerColor(2.0);
        hTrackMapIndex[iPlane-1]->SetMarkerColor(4);
        hTrackMapIndex[iPlane-1]->SetLineColor(4);
        hTrackMapIndex[iPlane-1]->SetLineWidth(3);
        hTrackMapIndex[iPlane-1]->SetStats(kFALSE);
      }
    } // end if withTracks

  } // end loop on planes

  cdisplayraw->cd();
  TPad *pad = new TPad("pad","",0.,0.,1.,0.965);
  pad->Draw();
  //pad->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);
  pad->Divide( (Int_t)ceil(NGoodPlanes/2.), (NGoodPlanes>1)?2:1);


  // Prepare a file for print output
  // The file is updated and not renewed !
  Char_t fileName[1000];
  sprintf( fileName, "%srawData_run%d.txt", fSession->GetResultDirName().Data(), fSession->GetRunNumber()); // directory updated, JB 2009/09/18
  sprintf(fileName,"%s", fTool.LocalizeDirName( fileName)); // JB 2011/07/07
  FILE *outFile;
  outFile = fopen( fileName, "a");
  fprintf( outFile, "\n\nRun %d Event %d", fSession->GetRunNumber(), fSession->GetCurrentEventNumber()-1);

  fSession->NextRawEvent( triggerNb);
  tTracker->Update();

  Char_t canvasTitle[200];
  sprintf(canvasTitle, "Run %d Event %d", fSession->GetRunNumber(), fSession->GetCurrentEventNumber()-1);
  cdisplayraw->cd();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  if( MultiFramePlane>0 ) {
    cdisplaymultiframe->cd();
    labelframe->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  }

  Int_t iCol, iRow; // start at 0, so for Setting Bin, use iCol/iRow +1
  Bool_t testMult;

  int GoodPlaneCounter = 0;
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    if(!PlotPlane[iPlane-1]) continue;

    tPlane = tTracker->GetPlane(iPlane);

    if ( tPlane->GetReadout() >= 100 || tPlane->GetDigitalThresholdNb() > 0 ){ // sparsified readout RDM210509, and digitized non-sparse readout JB 2013/08/29

      std::vector<DPixel*> *aList = tPlane->GetListOfPixels();
      if(fVerbose) cout << "Plane " << iPlane << " has " << aList->size() << " fired pixels" << endl;
      fprintf( outFile, "\n** plane %2d", iPlane);

      for( Int_t iPix=0; iPix<(Int_t)aList->size(); iPix++) { // loop on fired pixels
        iCol = aList->at(iPix)->GetPixelColumn();
        iRow = aList->at(iPix)->GetPixelLine();
        if( aList->at(iPix)->GetPulseHeight()>=minSN ) {
          int idx_U = hRawData[iPlane-1]->GetXaxis()->FindBin(iCol);
          int idx_V = hRawData[iPlane-1]->GetYaxis()->FindBin(iRow);
          hRawData[iPlane-1]->SetBinContent(idx_U,idx_V, aList->at(iPix)->GetPulseHeight());
        }

        fprintf( outFile, "\n%3d |c%3d| %.1f", iRow, iCol, aList->at(iPix)->GetPulseHeight());
        //printf("MRaw::DisplayRawData  pl %d, strip[%d(%d,%d)]=%f\n", iPlane, iStrip, iStrip%tPlane->GetStripsNu(), iStrip/tPlane->GetStripsNu(), tPlane->GetRawValue(iStrip));

        if( iPlane==MultiFramePlane ) {
          if( aList->at(iPix)->GetPulseHeight()>=minSN ) {
            int idx_U = hRawDataMultiFrame[ aList->at(iPix)->GetTimestamp() ]->GetXaxis()->FindBin(iCol);
            int idx_V = hRawDataMultiFrame[ aList->at(iPix)->GetTimestamp() ]->GetYaxis()->FindBin(iRow);
            //printf("MRaw::DisplayRawData  pl %d, pix[%d(%d,%d,t=%d)]=%f\n", iPlane, iPix, iCol, iRow, aList->at(iPix)->GetTimestamp(), aList->at(iPix)->GetPulseHeight());
            hRawDataMultiFrame[ aList->at(iPix)->GetTimestamp() ]->SetBinContent( idx_U, idx_V, aList->at(iPix)->GetPulseHeight());
          }
        }

      } //end loop on fired pixels

    } //end of sparsified readout
    else if ( tPlane->GetReadout() < 100 && tPlane->GetReadout() > 0 ){ // if non-sparsified readout, test if not 0 added (JB 2013/05/26)

      for( Int_t iStrip=0; iStrip<tPlane->GetStripsN(); iStrip++) {
        iCol = iStrip%tPlane->GetStripsNu();
        iRow = iStrip/tPlane->GetStripsNu();

        int idx_U = hRawData[iPlane-1]->GetXaxis()->FindBin(iCol);
        int idx_V = hRawData[iPlane-1]->GetYaxis()->FindBin(iRow);
        if( tPlane->GetPulseHeight(iStrip) >=minSN ) hRawData[iPlane-1]->SetBinContent( idx_U, idx_V, tPlane->GetPulseHeight(iStrip));

        //if( iRow<20 ) {
        if( iCol==0 ) { fprintf( outFile, "\n%3d |c%3d|", iRow, iCol);}
        else if( iCol%50==0 ) { fprintf( outFile, "|c%3d|", iCol); }
        fprintf( outFile, "%.1f ", tPlane->GetPulseHeight(iStrip));
        //}
        //printf("MRaw::DisplayRawData  pl %d, strip[%d(%d,%d)]=%f\n", iPlane, iStrip, iStrip%tPlane->GetStripsNu(), iStrip/tPlane->GetStripsNu(), tPlane->GetPulseHeight(iStrip));
      } //end loop on strips

    } //end of non-sparsified readout


    if( withHits ) {
      if(fVerbose) cout << "Plane " << iPlane << " has " << tPlane->GetHitsN() << " hits reconstructed" << endl;
      int color_counter = 0;
      for( Int_t iHit=1; iHit<=tPlane->GetHitsN(); iHit++) { //loop on hits (starts at 1 !!)
        aHit = (DHit*)tPlane->GetHit( iHit);

//        printf("Getting seed index for hit %d (address %x) at plane %d\n", iHit, aHit, iPlane);
        double u = aHit->GetPositionUhit();
        double v = aHit->GetPositionVhit();
        double TheCol,TheRow;
        tPlane->ComputeStripPosition_UVToColRow(u,v,TheCol,TheRow);
        iCol = TheCol;
        iRow = TheRow;

        int Npixels_in_Hit = aHit->GetStripsInCluster();

        if ( clusterMultiplicity == -1 )                    testMult = kTRUE;
        else if ( Npixels_in_Hit == clusterMultiplicity )   testMult = kTRUE;
        else                                                testMult = kFALSE;

//        printf(" hit %d, (col,row)=(%3d, %3d), (u,v)=(%.1f, %.1f), Npixels=%d, testMult=%d\n", iHit, (int)TheCol, (int)TheRow, u, v, Npixels_in_Hit, testMult);

        if ( testMult ) {
	  int TS = 0;
	  if(tPlane->GetReadout() > 100) aHit->GetPSeed()->GetTimestamp();
          _HitPositions_U[iPlane-1].push_back(TheCol);
          _HitPositions_V[iPlane-1].push_back(TheRow);
          _HitTS[iPlane-1].push_back(aHit->GetTimestamp());
          _HitColors[iPlane-1].push_back(Colors_tmp[color_counter]);

          vector<DPixel*> *aList = tPlane->GetListOfPixels();
          int idex_pixel;
          double Pixel_col;
          double Pixel_row;
//          if(fVerbose) cout << "Plane " << iPlane << " has " << aList->size() << " fired pixels" << endl;

            for(int ipixInHit=0;ipixInHit < Npixels_in_Hit;ipixInHit++) {
            if( aHit->GetPSeed()==NULL ) { // Hit built with DStrip object
              idex_pixel = aHit->GetIndex(ipixInHit) - 1;
              Pixel_col = idex_pixel%tPlane->GetStripsNu();
              Pixel_row = idex_pixel/tPlane->GetStripsNu();
            }
            else { // Hit built with DPixel object
              idex_pixel   = aHit->GetIndexInOriginalList(ipixInHit);
              Pixel_col = aList->at(idex_pixel)->GetPixelColumn();
              Pixel_row = aList->at(idex_pixel)->GetPixelLine();
            }
//            printf("    pix %d, index = %d, (col,row)=(%3d, %3d)\n", ipixInHit, idex_pixel, Pixel_col, Pixel_row );

            _PixelsInHitPositions_U[iPlane-1].push_back(Pixel_col);
            _PixelsInHitPositions_V[iPlane-1].push_back(Pixel_row);
            _PixelsInHitTS[iPlane-1].push_back(aHit->GetTimestamp());
            _PixelsInHitColors[iPlane-1].push_back(Colors_tmp[color_counter]);
          }

          color_counter++;
          if(color_counter == Ncolors) color_counter = 0;
        }


        if(fVerbose) printf("MRaw::DisplayRawData2D  pl %d, hit[%d] = (%.1f,%.1f) in plane.\n", iPlane, iHit, aHit->GetPositionUhit(), aHit->GetPositionVhit());

      } //end loop on hits
    } // end if withHits


    if( withTracks ) {
      if(fVerbose) cout << "Tracker has " << tTracker->GetTracksN() << " tracks reconstructed" << endl;
      for( Int_t iTrack=1; iTrack<=tTracker->GetTracksN(); iTrack++ ) { // loop on tracks
        aTrack = tTracker->GetTrack(iTrack);
        double u = tPlane->Intersection(aTrack)(0);
        double v = tPlane->Intersection(aTrack)(1);
        double TheCol,TheRow;
        tPlane->ComputeStripPosition_UVToColRow(u,v,TheCol,TheRow);

        if((TheCol >= hTrackMapIndex[iPlane-1]->GetXaxis()->GetXmin() && TheCol <= hTrackMapIndex[iPlane-1]->GetXaxis()->GetXmax()) &&
           (TheRow >= hTrackMapIndex[iPlane-1]->GetYaxis()->GetXmin() && TheRow <= hTrackMapIndex[iPlane-1]->GetYaxis()->GetXmax())) {
          hTrackMapIndex[iPlane-1]->Fill(TheCol, TheRow, 1);

          _TrkPositions_U[iPlane-1].push_back(TheCol);
          _TrkPositions_V[iPlane-1].push_back(TheRow);
          _TrkIdx[iPlane-1].push_back(iTrack);
        }

        if(fVerbose) printf("MRaw::DisplayRawData2D  pl %d, track[%d] = (%.1f,%.1f) in plane.\n", iPlane, iTrack, tPlane->Intersection(aTrack)(0), tPlane->Intersection(aTrack)(1));
      } // end loop on tracks
    } // end if withTracks


    if(iPlane==MultiFramePlane ) {

      //double Minimum = +1.0e+20;
      //double Maximum = -1.0e+20;
      //for(int iFrame=0; iFrame<MultiFrameNFrames; iFrame++) {
      //if(Minimum > hRawDataMultiFrame[iFrame]->GetMinimum()) Minimum = hRawDataMultiFrame[iFrame]->GetMinimum();
      //if(Maximum < hRawDataMultiFrame[iFrame]->GetMaximum()) Maximum = hRawDataMultiFrame[iFrame]->GetMaximum();
      //}

      cdisplaymultiframe->cd();
      for(int iFrame=0; iFrame<MultiFrameNFrames; iFrame++) {
        //padframe->cd(iFrame+1);
        //cdisplaymultiframe->cd(iFrame+1);
        padframe->cd(iFrame+1);
        gPad->SetFillColor(10);
        gPad->SetFrameFillColor(10);
        gPad->SetTopMargin(0.15);
        gPad->SetBottomMargin(0.15);
        gPad->SetLeftMargin(0.15);
        gPad->SetRightMargin(0.15);
        gPad->SetTickx(1);
        gPad->SetTicky(1);
        gPad->SetGridx(1);
        gPad->SetGridy(1);
        //hRawDataMultiFrame[iFrame]->SetMinimum(Minimum);
        //hRawDataMultiFrame[iFrame]->SetMaximum(Maximum);
        hRawDataMultiFrame[iFrame]->DrawCopy("colz");
        hRawDataMultiFrame[iFrame]->Delete();

        if( withHits ) {
          //Plot positions of pixels beloning to the rec hits:
          for(int iPixelInHits=0;iPixelInHits<int(_PixelsInHitPositions_U[iPlane-1].size());iPixelInHits++) {
            if(iFrame != _PixelsInHitTS[iPlane-1][iPixelInHits]) continue;
            latex->SetTextColor(_PixelsInHitColors[iPlane-1][iPixelInHits]);
            latex->DrawLatex(_PixelsInHitPositions_U[iPlane-1][iPixelInHits],
                             _PixelsInHitPositions_V[iPlane-1][iPixelInHits],
                             "X");
          }

          //Plot positions of rec hits:
          //latex->SetTextColor(kBlue);
          for(int iHits=0;iHits<int(_HitPositions_U[iPlane-1].size());iHits++) {
            if(iFrame != _HitTS[iPlane-1][iHits]) continue;
            latex->SetTextColor(_HitColors[iPlane-1][iHits]);
            latex->DrawLatex(_HitPositions_U[iPlane-1][iHits],
                             _HitPositions_V[iPlane-1][iHits],
                             "O");
          }
        } // end if withHits

        if(withTracks) {
          for(int iTrk=0;iTrk<int(_TrkPositions_U[iPlane-1].size());iTrk++) {
            double shift = 2.0*(tPlane->GetStripsNv()/(tPlane->GetStripsNv()*NbinsReductionFactor));
            //double shift = 2.0*hHitMapIndex[iPlane-1]->GetYaxis()->GetBinWidth(1);
            char ttt[300];
            sprintf(ttt,"%d",_TrkIdx[iPlane-1][iTrk]);
            latex2->DrawLatex(_TrkPositions_U[iPlane-1][iTrk],
                              //_TrkPositions_V[iPlane-1][iTrk] + shift,
                              _TrkPositions_V[iPlane-1][iTrk],
                              ttt);
          }
        } // end if withTracks

      } // End of Loop over frame of multi-frame event

      cdisplaymultiframe->Update();

    }
    else {
      // Display
      double R_tmp_U[2];
      double R_tmp_V[2];
      R_tmp_U[0] = hRawData[iPlane-1]->GetXaxis()->GetXmin();
      R_tmp_U[1] = hRawData[iPlane-1]->GetXaxis()->GetXmax();
      R_tmp_V[0] = hRawData[iPlane-1]->GetYaxis()->GetXmin();
      R_tmp_V[1] = hRawData[iPlane-1]->GetYaxis()->GetXmax();

      cdisplayraw->cd();
      GoodPlaneCounter++;
      pad->cd();
      pad->cd(GoodPlaneCounter);
      pad->cd(GoodPlaneCounter)->SetGridx(1);
      pad->cd(GoodPlaneCounter)->SetGridy(1);
      pad->cd(GoodPlaneCounter)->SetTickx(1);
      pad->cd(GoodPlaneCounter)->SetTicky(1);
      hRawData[iPlane-1]->DrawCopy("colz"); //RDM210509
      hRawData[iPlane-1]->Delete();         //RDM210509

      if( withHits ) {
        //Plot positions of pixels beloning to the rec hits:
        for(int iPixelInHits=0;iPixelInHits<int(_PixelsInHitPositions_U[iPlane-1].size());iPixelInHits++) {
          if((_PixelsInHitPositions_U[iPlane-1][iPixelInHits] >= R_tmp_U[0] &&
              _PixelsInHitPositions_U[iPlane-1][iPixelInHits] <= R_tmp_U[1])
             &&
             (_PixelsInHitPositions_V[iPlane-1][iPixelInHits] >= R_tmp_V[0] &&
              _PixelsInHitPositions_V[iPlane-1][iPixelInHits] <= R_tmp_V[1])
             ) {
            latex->SetTextColor(_PixelsInHitColors[iPlane-1][iPixelInHits]);
            latex->DrawLatex(_PixelsInHitPositions_U[iPlane-1][iPixelInHits],
                             _PixelsInHitPositions_V[iPlane-1][iPixelInHits],
                             "X");
          }
        }

        //Plot positions of rec hits:
        //latex->SetTextColor(kBlue);
        for(int iHits=0;iHits<int(_HitPositions_U[iPlane-1].size());iHits++) {
          if((_HitPositions_U[iPlane-1][iHits] >= R_tmp_U[0] &&
              _HitPositions_U[iPlane-1][iHits] <= R_tmp_U[1])
             &&
             (_HitPositions_V[iPlane-1][iHits] >= R_tmp_V[0] &&
              _HitPositions_V[iPlane-1][iHits] <= R_tmp_V[1])
             ) {
            latex->SetTextColor(_HitColors[iPlane-1][iHits]);
            latex->DrawLatex(_HitPositions_U[iPlane-1][iHits],
                             _HitPositions_V[iPlane-1][iHits],
                             "O");
          }
        }

      } // end if withHits

      if(withTracks) {
        for(int iTrk=0;iTrk<int(_TrkPositions_U[iPlane-1].size());iTrk++) {
	  if((_TrkPositions_U[iPlane-1][iTrk] >= R_tmp_U[0] &&
	      _TrkPositions_U[iPlane-1][iTrk] <= R_tmp_U[1])
	     &&
	     (_TrkPositions_V[iPlane-1][iTrk] >= R_tmp_V[0] &&
	      _TrkPositions_V[iPlane-1][iTrk] <= R_tmp_V[1])
	     ) {
	     double shift = 2.0*(tPlane->GetStripsNv()/(tPlane->GetStripsNv()*NbinsReductionFactor));
	     //double shift = 2.0*hHitMapIndex[iPlane-1]->GetYaxis()->GetBinWidth(1);
	     char ttt[300];
	     sprintf(ttt,"%d",_TrkIdx[iPlane-1][iTrk]);
	     latex2->DrawLatex(_TrkPositions_U[iPlane-1][iTrk],
	  		       //_TrkPositions_V[iPlane-1][iTrk] + shift,
			       _TrkPositions_V[iPlane-1][iTrk],
			       ttt);
	  }
        }
      } // end if withTracks

      cdisplayraw->Update();

    } //End of else is MultiFrame


  } //end loop on planes
  cout << "\n Event number : " << fSession->GetCurrentEventNumber()-1<<" " << endl;

  //if(MultiFramePlane>0) cdisplaymultiframe->Update();
  //cdisplayraw->Update();

  fclose( outFile);

}

//______________________________________________________________________________
//
void MRaw::DisplayHits2D( Int_t ifIndex, Int_t ifTrack, Int_t geomatrix)
{
  // Display the hit position for each plane event by event
  // ifIndex=0 -> position in microns (plane frame), =1 -> position in index, =2 -> pos in microns (telescope frame)
  // ifTrack=0 -> don't display tracks on top, =1(default) tracks displayed as numbers, =2 -> tracks displayes as circles
  // geomatrix indicates which limits from the config file
  //   will be used to display a rectangle with dotted line
  //   (usefull to point DUT postion)
  //
  //
  // Call by gTAF->GetRaw()->DisplayHits2D()
  //
  // JB, 2008/10/23
  // Last modified: RDM 2009/05/21
  // Last Modified JB 2009/08/31 binning of histos and bin index in filling
  // Last Modified JB 2009/09/09 display each tracks only once
  // last Modified JB 2010/06/16 display geomatrix and trigger info, and complete bin index
  // last Modified JB+VR 2012/04/02 new options to display track number and in telescope frame
  // Last Modified JB 2012/08/20 improved geomatrix in different frame
	// Last Modified JB 2014/03/18 display additional Ladder

  fSession->SetEvents(3);
  DAcq *tAcq = fSession->GetDataAcquisition(); //JB 2010/06/16

  TCanvas *cdisplayhit;
  TObject* g = gROOT->FindObject("cdisplayhit") ;
  if (g) {
    cdisplayhit = (TCanvas*)g;
  }
  else {
    cdisplayhit = new TCanvas("cdisplayhit", "Inspect Hits", 5, 5,800,700);
  }
  cdisplayhit->Clear();
  cdisplayhit->UseCurrentStyle();
  TPaveLabel* label = new TPaveLabel();
  Char_t canvasTitle[200];
  TPad *pad = new TPad("pad","",0.,0.,1.,0.965);
  pad->Draw();

  DTracker *tTracker  =  fSession->GetTracker();
  Int_t nPlanes = tTracker->GetPlanesN();
  pad->Divide( (Int_t)ceil(nPlanes/4.), (nPlanes>4)?4:nPlanes); // LC 2012/01/04. 4 lines instead of 2.

  // Get the first DUT among the planes and remember it,
  //  it is considered as the one providing the coordinate frame for the geomatrix
  // If no DUT (status=3), set to first plane
  // JB 2012/0820
  DPlane* DUTplane = tTracker->GetPlane(1);
  DPlane* tPlane = NULL;
  for( Int_t iPlane=2; iPlane<=nPlanes; iPlane++) { // loop on planes
    tPlane = tTracker->GetPlane(iPlane);
    if( tPlane->GetStatus()==3 ) {
      DUTplane = tPlane;
      break;
    }
  }
  // geomatrix coordinates
  DR3 gUVWmin( fSession->GetSetup()->GetAnalysisPar().Umin[0][geomatrix], fSession->GetSetup()->GetAnalysisPar().Vmin[0][geomatrix], DUTplane->GetPosition()(2));
  DR3 gUVWmax( fSession->GetSetup()->GetAnalysisPar().Umax[0][geomatrix], fSession->GetSetup()->GetAnalysisPar().Vmax[0][geomatrix], DUTplane->GetPosition()(2));
  // Transform those DUTplane frame coordinates into the telescope frame
  DR3 gXYZmin = DUTplane->PlaneToTracker(gUVWmin);
  DR3 gXYZmax = DUTplane->PlaneToTracker(gUVWmax);
  cout << " DUT plane is got as plane " << DUTplane->GetPlaneNumber() << endl;

  TH2F **hHitMapIndex = new TH2F*[nPlanes];
  TH2F **hHitMap = new TH2F*[nPlanes];
  TH2F **hHitMapTel = new TH2F*[nPlanes];
  TH2F **hHitFoundMapTel = new TH2F*[nPlanes];
  TH2F **hTrackMapIndex = new TH2F*[nPlanes];
  TH2F **hTrackMap = new TH2F*[nPlanes];
  TH2F **hTrackMapTel = new TH2F*[nPlanes];
/*
  TH2F **hVertexMapIndex = new TH2F*[nPlanes]; // LC 2012/12/18
  TH2F **hVertexMap = new TH2F*[nPlanes];
  TH2F **hVertexMapTel = new TH2F*[nPlanes];
*/
  TLine **triggerLine = new TLine*[nPlanes];
  TBox **geomBoxIndex = new TBox*[nPlanes];
  TBox **geomBox = new TBox*[nPlanes];
  TBox **geomPlaneBox = new TBox*[nPlanes];
  Float_t *geomUmin = new Float_t[nPlanes];
  Float_t *geomUmax = new Float_t[nPlanes];
  Float_t *geomVmin = new Float_t[nPlanes];
  Float_t *geomVmax = new Float_t[nPlanes];
  Int_t *geomBinUmin = new Int_t[nPlanes];
  Int_t *geomBinUmax = new Int_t[nPlanes];
  Int_t *geomBinVmin = new Int_t[nPlanes];
  Int_t *geomBinVmax = new Int_t[nPlanes];
  Int_t *centerUbin = new Int_t[nPlanes];
  Int_t *centerVbin = new Int_t[nPlanes];

  // Determine extrema of planes position in telescope frame
  Double_t xmin=1e6, xmax=-1e6;
  Double_t ymin=1e6, ymax=-1e6;
  Double_t planeBox[2][2] = {{0,0},{0,0}};
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    tPlane = tTracker->GetPlane(iPlane);
   // bottom left corner
    DR3 posInPlane, posBLInTracker, posURInTracker;
    posInPlane.SetValue( -tPlane->GetStripsNu() * tPlane->GetStripPitch()(0) / 2.
			,-tPlane->GetStripsNv() * tPlane->GetStripPitch()(1) / 2.
			,0.);
    posBLInTracker = tPlane->PlaneToTracker( posInPlane);
    if( posBLInTracker(0)<xmin ) xmin = posBLInTracker(0);
    if( posBLInTracker(1)<ymin ) ymin = posBLInTracker(1);
    if( posBLInTracker(0)>xmax ) xmax = posBLInTracker(0);
    if( posBLInTracker(1)>ymax ) ymax = posBLInTracker(1);
   // upper right corner
    posInPlane.SetValue( +tPlane->GetStripsNu() * tPlane->GetStripPitch()(0) / 2.
			,+tPlane->GetStripsNv() * tPlane->GetStripPitch()(1) / 2.
			,0.);
    posURInTracker = tPlane->PlaneToTracker( posInPlane);
    if( posURInTracker(0)<xmin ) xmin = posURInTracker(0);
    if( posURInTracker(1)<ymin ) ymin = posURInTracker(1);
    if( posURInTracker(0)>xmax ) xmax = posURInTracker(0);
    if( posURInTracker(1)>ymax ) ymax = posURInTracker(1);
    planeBox[0][0] = TMath::Min( posBLInTracker(0), posURInTracker(0));
    planeBox[0][1] = TMath::Max( posBLInTracker(0), posURInTracker(0));
    planeBox[1][0] = TMath::Min( posBLInTracker(1), posURInTracker(1));
    planeBox[1][1] = TMath::Max( posBLInTracker(1), posURInTracker(1));
    geomPlaneBox[iPlane-1] = new TBox( planeBox[0][0], planeBox[1][0], planeBox[0][1], planeBox[1][1]);
    geomPlaneBox[iPlane-1]->SetFillStyle(0);
  } // end loop on planes

  // Create the histograms
  Char_t name[50], title[100];
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    tPlane = tTracker->GetPlane(iPlane);

    // -- Histo for hits with index
    sprintf( name, "hhitmapipl%d", iPlane);
    sprintf( title, "Hit map of plane (%d) %s", iPlane, tPlane->GetPlanePurpose());
    hHitMapIndex[iPlane-1] = new TH2F(name, title, tPlane->GetStripsNu(), 0, tPlane->GetStripsNu(), tPlane->GetStripsNv(), 0, tPlane->GetStripsNv());
    hHitMapIndex[iPlane-1]->SetMarkerStyle(20);
    hHitMapIndex[iPlane-1]->SetMarkerSize(.6);
    hHitMapIndex[iPlane-1]->SetMarkerColor(1);
    hHitMapIndex[iPlane-1]->SetStats(kFALSE);
    //printf( "MRaw::DisplayRawData created %s histo with %dx%d pixels\n", name, tPlane->GetStripsNu(), tPlane->GetStripsNv());

    // -- Histo for hits with microns in plane frame
    sprintf( name, "hhitmappl%d", iPlane);
    sprintf( title, "Hit map of plane (%d) %s", iPlane, tPlane->GetPlanePurpose());
    hHitMap[iPlane-1] = new TH2F(name, title, 100, -tPlane->GetStripPitch()(0)*tPlane->GetStripsNu()/2., tPlane->GetStripPitch()(0)*tPlane->GetStripsNu()/2., 100, -tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2., tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2.);
    hHitMap[iPlane-1]->SetMarkerStyle(20);
    hHitMap[iPlane-1]->SetMarkerSize(.6);
    hHitMap[iPlane-1]->SetMarkerColor(1);
    hHitMap[iPlane-1]->SetStats(kFALSE);
    //printf( "MRaw::DisplayRawData created %s histo with %dx%d pixels\n", name, tPlane->GetStripsNu(), tPlane->GetStripsNv());

    // -- Histo for hits with microns in telescope frame
    sprintf( name, "hhitmaptelpl%d", iPlane);
    sprintf( title, "Hit map of plane (%d) %s (tel frame)", iPlane, tPlane->GetPlanePurpose());
   /*
   // bottom left corner
    DR3 posInPlane, posInTracker;
    posInPlane.SetValue( -tPlane->GetStripsNu() * tPlane->GetStripPitch()(0) / 2.
			,-tPlane->GetStripsNv() * tPlane->GetStripPitch()(1) / 2.
			,0.);
    posInTracker = tPlane->PlaneToTracker( posInPlane);
    Double_t xmin = posInTracker(0);
    Double_t ymin = posInTracker(1);
   // upper right corner
    posInPlane.SetValue( +tPlane->GetStripsNu() * tPlane->GetStripPitch()(0) / 2.
			,+tPlane->GetStripsNv() * tPlane->GetStripPitch()(1) / 2.
			,0.);
    posInTracker = tPlane->PlaneToTracker( posInPlane);
    Double_t xmax = posInTracker(0);
    Double_t ymax = posInTracker(1);
    */
    hHitMapTel[iPlane-1] = new TH2F(name, title, 100, xmin, xmax, 100, ymin, ymax);
    hHitMapTel[iPlane-1]->SetMarkerStyle(20);
    hHitMapTel[iPlane-1]->SetMarkerSize(.6);
    hHitMapTel[iPlane-1]->SetMarkerColor(1);
    hHitMapTel[iPlane-1]->SetStats(kFALSE);
    //printf( "MRaw::DisplayRawData created %s histo with %dx%d pixels\n", name, tPlane->GetStripsNu(), tPlane->GetStripsNv());

    // -- Histo for found (associated with tracks) hits with microns in telescope frame
    sprintf( name, "hhitfoundmaptelpl%d", iPlane);
    sprintf( title, "Found hit map of plane (%d) %s (tel frame)", iPlane, tPlane->GetPlanePurpose());
    hHitFoundMapTel[iPlane-1] = new TH2F(name, title, 100, xmin, xmax, 100, ymin, ymax);
    hHitFoundMapTel[iPlane-1]->SetMarkerStyle(2);
    hHitFoundMapTel[iPlane-1]->SetMarkerSize(1.);
    hHitFoundMapTel[iPlane-1]->SetMarkerColor(2);
    hHitFoundMapTel[iPlane-1]->SetStats(kFALSE);
    //printf( "MRaw::DisplayRawData created %s histo with %dx%d pixels\n", name, tPlane->GetStripsNu(), tPlane->GetStripsNv());


    // -- Histo for tracks with microns in plane frame
    sprintf( name, "htrackmappl%d", iPlane);
    sprintf( title, "Track map of plane (%d) %s", iPlane, tPlane->GetPlanePurpose());
    hTrackMap[iPlane-1] = new TH2F( *(hHitMap[iPlane-1])); // JB 2009/09/09
    //name, title, hHitMap[iPlane-1]->GetNbinsX(), hHitMap[iPlane-1]->GetNbinsX(), tPlane->GetStripPitch()(0)*tPlane->GetStripsNu()/2., 100, -tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2., tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2.);
    hTrackMap[iPlane-1]->SetName( name);
    hTrackMap[iPlane-1]->SetTitle( title);
    hTrackMap[iPlane-1]->SetMarkerStyle(24);
    hTrackMap[iPlane-1]->SetMarkerSize(2.);
    hTrackMap[iPlane-1]->SetStats(kFALSE);

    // -- Histo for tracks with index
    sprintf( name, "htrackmapipl%d", iPlane);
    sprintf( title, "Track map of plane (%d) %s", iPlane, tPlane->GetPlanePurpose());
    hTrackMapIndex[iPlane-1] = new TH2F( *(hHitMapIndex[iPlane-1])); // JB 2009/09/09
    //name, title, hHitMap[iPlane-1]->GetNbinsX(), hHitMap[iPlane-1]->GetNbinsX(), tPlane->GetStripPitch()(0)*tPlane->GetStripsNu()/2., 100, -tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2., tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2.);
    hTrackMapIndex[iPlane-1]->SetName( name);
    hTrackMapIndex[iPlane-1]->SetTitle( title);
    hTrackMapIndex[iPlane-1]->SetMarkerStyle(24);
    hTrackMapIndex[iPlane-1]->SetMarkerSize(2.);
    hTrackMapIndex[iPlane-1]->SetStats(kFALSE);

    // -- Histo for tracks with microns in plane frame
    sprintf( name, "htrackmappl%d", iPlane);
    sprintf( title, "Track map of plane (%d) %s", iPlane, tPlane->GetPlanePurpose());
    hTrackMap[iPlane-1] = new TH2F( *(hHitMap[iPlane-1])); // JB 2009/09/09
    //name, title, hHitMap[iPlane-1]->GetNbinsX(), hHitMap[iPlane-1]->GetNbinsX(), tPlane->GetStripPitch()(0)*tPlane->GetStripsNu()/2., 100, -tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2., tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2.);
    hTrackMap[iPlane-1]->SetName( name);
    hTrackMap[iPlane-1]->SetTitle( title);
    hTrackMap[iPlane-1]->SetMarkerStyle(24);
    hTrackMap[iPlane-1]->SetMarkerSize(2.);
    hTrackMap[iPlane-1]->SetStats(kFALSE);

    // -- Histo for tracks with microns in telescope frame
    sprintf( name, "htrackmaptelpl%d", iPlane);
    sprintf( title, "Track map of plane (%d) %s (tel frame)", iPlane, tPlane->GetPlanePurpose());
    hTrackMapTel[iPlane-1] = new TH2F( *(hHitMapTel[iPlane-1])); // JB 2009/09/09
    //name, title, hHitMapTel[iPlane-1]->GetNbinsX(), hHitMap[iPlane-1]->GetNbinsX(), tPlane->GetStripPitch()(0)*tPlane->GetStripsNu()/2., 100, -tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2., tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2.);
    hTrackMapTel[iPlane-1]->SetName( name);
    hTrackMapTel[iPlane-1]->SetTitle( title);
    hTrackMapTel[iPlane-1]->SetMarkerStyle(24);
    hTrackMapTel[iPlane-1]->SetMarkerSize(2.);
    hTrackMapTel[iPlane-1]->SetStats(kFALSE);
/*
     // -- Histo for vertex. LC 2012/12/18.
    sprintf( name, "hvertexmappl%d", iPlane);
    sprintf( title, "Vertex map of plane (%d) %s", iPlane, tPlane->GetPlanePurpose());
    hVertexMap[iPlane-1] = new TH2F( *(hHitMap[iPlane-1]));
    hVertexMap[iPlane-1]->SetName( name);
    hVertexMap[iPlane-1]->SetTitle( title);
    hVertexMap[iPlane-1]->SetMarkerStyle(28);
    hVertexMap[iPlane-1]->SetMarkerSize(2.);
    hVertexMap[iPlane-1]->SetStats(kFALSE);

    // -- Histo for vertex in telescope frame
    sprintf( name, "hvertexmaptelpl%d", iPlane);
    sprintf( title, "Vertex map of plane (%d) %s (tel frame)", iPlane, tPlane->GetPlanePurpose());
    hVertexMapTel[iPlane-1] = new TH2F( *(hHitMapTel[iPlane-1]));
    hVertexMapTel[iPlane-1]->SetName( name);
    hVertexMapTel[iPlane-1]->SetTitle( title);
    hVertexMapTel[iPlane-1]->SetMarkerStyle(28);
    hVertexMapTel[iPlane-1]->SetMarkerSize(2.);
    hVertexMapTel[iPlane-1]->SetStats(kFALSE);

    // -- Histo for vertex with index
    sprintf( name, "hvertexmapipl%d", iPlane);
    sprintf( title, "Vertex map of plane (%d) %s", iPlane, tPlane->GetPlanePurpose());
    hVertexMapIndex[iPlane-1] = new TH2F( *(hHitMapIndex[iPlane-1])); // JB 2009/09/09
    //name, title, hHitMap[iPlane-1]->GetNbinsX(), hHitMap[iPlane-1]->GetNbinsX(), tPlane->GetStripPitch()(0)*tPlane->GetStripsNu()/2., 100, -tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2., tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2.);
    hVertexMapIndex[iPlane-1]->SetName( name);
    hVertexMapIndex[iPlane-1]->SetTitle( title);
    hVertexMapIndex[iPlane-1]->SetMarkerStyle(28);
    hVertexMapIndex[iPlane-1]->SetMarkerSize(2.);
    hVertexMapIndex[iPlane-1]->SetStats(kFALSE);
*/

    // Get the limit of the geomatrix
    //  originally, those points are given in the DUT plane frame,
    //  they need to be transformed for index=2 and for other planes
    centerUbin[iPlane-1] = tPlane->GetStripsNu()/2;
    centerVbin[iPlane-1] = tPlane->GetStripsNv()/2;
    if( geomatrix<0 ) { // replace geomatrix with plane limits
      geomUmin[iPlane-1] = -tPlane->GetStripPitch()(0)*tPlane->GetStripsNu()/2.;
      geomUmax[iPlane-1] =  tPlane->GetStripPitch()(0)*tPlane->GetStripsNu()/2.;
      geomVmin[iPlane-1] = -tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2.;
      geomVmax[iPlane-1] =  tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2.;
    }
    else if( ifIndex<=1 ) { // geomatrix in plane frame
      geomUmin[iPlane-1] = tPlane->TrackerToPlane(gXYZmin)(0);
      geomUmax[iPlane-1] = tPlane->TrackerToPlane(gXYZmax)(0);
      geomVmin[iPlane-1] = tPlane->TrackerToPlane(gXYZmin)(1);
      geomVmax[iPlane-1] = tPlane->TrackerToPlane(gXYZmax)(1);
    }
    else if( ifIndex==2 ) { // geomatrix in telescope frame
      geomUmin[iPlane-1] = gXYZmin(0);
      geomUmax[iPlane-1] = gXYZmax(0);
      geomVmin[iPlane-1] = gXYZmin(1);
      geomVmax[iPlane-1] = gXYZmax(1);
    }
    if(fVerbose) printf("  geomatrix %d for plane %d = u[%.1f->%.1f] ; v[%.1f->%.1f]\n", geomatrix, tPlane->GetPlaneNumber(), geomUmin[iPlane-1], geomUmax[iPlane-1], geomVmin[iPlane-1], geomVmax[iPlane-1]);

    geomBinUmin[iPlane-1] = (Int_t)(geomUmin[iPlane-1]/tPlane->GetStripPitch()(0))+centerUbin[iPlane-1];
    geomBinUmax[iPlane-1] = (Int_t)(geomUmax[iPlane-1]/tPlane->GetStripPitch()(0))+centerUbin[iPlane-1];
    geomBinVmin[iPlane-1] = (Int_t)(geomVmin[iPlane-1]/tPlane->GetStripPitch()(1))+centerVbin[iPlane-1];
    geomBinVmax[iPlane-1] = (Int_t)(geomVmax[iPlane-1]/tPlane->GetStripPitch()(1))+centerVbin[iPlane-1];
    if(fVerbose) printf(" Geomatrix %d box is [(%d; %d) - (%d; %d)] or [(%.0f; %.0f) - (%.0f; %.0f)]\n", geomatrix, geomBinUmin[iPlane-1], geomBinVmin[iPlane-1], geomBinUmax[iPlane-1], geomBinVmax[iPlane-1], geomUmin[iPlane-1], geomVmin[iPlane-1], geomUmax[iPlane-1], geomVmax[iPlane-1]);


    // Box for the geomatrix for the two coordinates systems
    geomBoxIndex[iPlane-1] = new TBox( geomBinUmin[iPlane-1], geomBinVmin[iPlane-1], geomBinUmax[iPlane-1], geomBinVmax[iPlane-1]);
    geomBoxIndex[iPlane-1]->SetFillStyle(0);
    geomBox[iPlane-1] = new TBox( geomUmin[iPlane-1], geomVmin[iPlane-1], geomUmax[iPlane-1], geomVmax[iPlane-1]);
    geomBox[iPlane-1]->SetFillStyle(0);
    geomBox[iPlane-1]->SetLineStyle(2);

    // Line to indicate trigger arrival time
    triggerLine[iPlane-1] = new TLine();

    // change the color of the track pos wrt to status of plane
    if( tPlane->GetStatus() == 0 || tPlane->GetStatus() == 1 || (tPlane->GetStatus() == 2 && tTracker->GetAlignmentStatus() >= 2) ) {
      hTrackMap[iPlane-1]->SetMarkerColor(3);
      hTrackMapTel[iPlane-1]->SetMarkerColor(3);
      hTrackMapIndex[iPlane-1]->SetMarkerColor(3);
//      hVertexMapIndex[iPlane-1]->SetMarkerColor(3); // LC 2012/12/18
//      hVertexMap[iPlane-1]->SetMarkerColor(3);
//      hVertexMapTel[iPlane-1]->SetMarkerColor(3);
      geomBox[iPlane-1]->SetLineColor(3);
      geomBoxIndex[iPlane-1]->SetLineColor(3);
      triggerLine[iPlane-1]->SetLineColor(3);
    }
    else if( tPlane->GetStatus() == 3 ) {
      hTrackMap[iPlane-1]->SetMarkerColor(4);
      hTrackMapTel[iPlane-1]->SetMarkerColor(4);
      hTrackMapIndex[iPlane-1]->SetMarkerColor(4);
//      hVertexMapIndex[iPlane-1]->SetMarkerColor(4); // LC 2012/12/18
//      hVertexMap[iPlane-1]->SetMarkerColor(4);
//      hVertexMapTel[iPlane-1]->SetMarkerColor(4);
      geomBox[iPlane-1]->SetLineColor(4);
      geomBoxIndex[iPlane-1]->SetLineColor(4);
      triggerLine[iPlane-1]->SetLineColor(4);
    }
    else {
      hTrackMap[iPlane-1]->SetMarkerColor(2);
      hTrackMapTel[iPlane-1]->SetMarkerColor(2);
      hTrackMapIndex[iPlane-1]->SetMarkerColor(2);
//      hVertexMapIndex[iPlane-1]->SetMarkerColor(2); // LC 2012/12/18
//      hVertexMap[iPlane-1]->SetMarkerColor(2);
//      hVertexMapTel[iPlane-1]->SetMarkerColor(2);
      geomBox[iPlane-1]->SetLineColor(2);
      geomBoxIndex[iPlane-1]->SetLineColor(2);
      triggerLine[iPlane-1]->SetLineColor(2);
    }

  } // end loop on planes



  // Booking for ladders
  // JB 2014/02/17
  Int_t nLadders = tTracker->GetNumberOfLadders();
  DLadder *tLadder;
  TCanvas *cdisplayladder = NULL;
  //TH2F **hRawDataLadder = NULL;
  TH2F **hHitMapLadder = NULL;
  TH2F **hHitFoundMapLadder = NULL;
  TH2F **hTrackMapLadder = NULL;
  if( nLadders>0 ) { // if ladders

    // Canvas for ladder
    g = gROOT->FindObject("cdisplayladder") ;
    if (g) {
      cdisplayladder = (TCanvas*)g;
    }
    else {
      cdisplayladder = new TCanvas("cdisplayladder", "Ladder view", 30, 30, 800, 700);
    }
    cdisplayladder->Clear();
    cdisplayladder->UseCurrentStyle();
    cdisplayladder->cd();
    cdisplayladder->Divide((Int_t)ceil(nLadders/2.), (nLadders>1)?2:1);

    // Ladder histograms
//    hRawDataLadder = new TH2F*[nPlanes];
    hHitMapLadder = new TH2F*[nLadders];
    hHitFoundMapLadder = new TH2F*[nLadders];
    hTrackMapLadder = new TH2F*[nLadders];
    for( Int_t iLadder=1; iLadder<=nLadders; iLadder++) { // loop on ladders
      tLadder = tTracker->GetLadder(iLadder);
      //cout << "Generating histo for ladder " << iLadder << " name=" << tLadder->GetLadderType() << endl;

//      sprintf( name, "hrawdatalad%d", iLadder);
//      sprintf( title, "Raw data of ladder (%d) %s", iLadder, tLadder->GetLadderType());
//      hRawDataLadder[iLadder-1] = new TH2F(name, title, tPlane->GetStripsNu(), 0, tPlane->GetStripsNu(), tPlane->GetStripsNv(), 0, tPlane->GetStripsNv());
//      hRawDataLadder[iLadder-1]->SetMarkerStyle(20);
//      hRawDataLadder[iLadder-1]->SetMarkerSize(.4);
//      hRawDataLadder[iLadder-1]->SetMarkerColor(1);
//      hRawDataLadder[iLadder-1]->SetStats(kFALSE);
      //printf( "MRaw::DisplayRawData created %s histo with %dx%d pixels\n", name, tPlane->GetStripsNu(), tPlane->GetStripsNv());

      sprintf( name, "hhitmapilad%d", iLadder);
      sprintf( title, "Hit map of ladder (%d) %s", iLadder, tLadder->GetLadderType().Data());
      hHitMapLadder[iLadder-1] = new TH2F(name, title, 100, xmin, xmax, 100, ymin, ymax);
      hHitMapLadder[iLadder-1]->SetMarkerStyle(20);
      hHitMapLadder[iLadder-1]->SetMarkerSize(0.6);
      hHitMapLadder[iLadder-1]->SetMarkerColor(1);
      hHitMapLadder[iLadder-1]->SetStats(kFALSE);
      //cout << "Ladder hit map done" << endl;

      sprintf( name, "hhitfoundmapladpl%d", iLadder);
      sprintf( title, "Found hit map of ladder (%d) %s (tel frame)", iLadder, tLadder->GetLadderType().Data());
      hHitFoundMapLadder[iLadder-1] = new TH2F(name, title, 100, xmin, xmax, 100, ymin, ymax);
      hHitFoundMapLadder[iLadder-1]->SetMarkerStyle(2);
      hHitFoundMapLadder[iLadder-1]->SetMarkerSize(1.);
      hHitFoundMapLadder[iLadder-1]->SetMarkerColor(2);
      hHitFoundMapLadder[iLadder-1]->SetStats(kFALSE);
      //cout << "Ladder found hit map done" << endl;

      sprintf( name, "htrackmapilad%d", iLadder);
      sprintf( title, "Track map of ladder (%d) %s", iLadder, tLadder->GetLadderType().Data());
      hTrackMapLadder[iLadder-1] = new TH2F( *(hHitMapLadder[iLadder-1]));
      hTrackMapLadder[iLadder-1]->SetName( name);
      hTrackMapLadder[iLadder-1]->SetTitle( title);
      hTrackMapLadder[iLadder-1]->SetMarkerStyle(24);
      hTrackMapLadder[iLadder-1]->SetMarkerSize(2.0);
      hTrackMapLadder[iLadder-1]->SetMarkerColor(2);
      hTrackMapLadder[iLadder-1]->SetStats(kFALSE);
      if( tLadder->GetStatus() == 0 && tLadder->GetStatus() <= tTracker->GetAlignmentStatus() ) {
        hTrackMapLadder[iLadder-1]->SetMarkerColor(3);
      }
      else if( tPlane->GetStatus() == 3 ) {
        hTrackMapLadder[iLadder-1]->SetMarkerColor(4);
      }
      //cout << "Ladder track map done" << endl;

    } // end loop on ladders

  } //end if ladders


	// === Analysis ====
  fSession->NextRawEvent();
  tTracker->Update();
	// =================

  sprintf(canvasTitle, "Run %d Event %d", fSession->GetRunNumber(), fSession->GetCurrentEventNumber());
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);

  DHit *aHit;
  DTrack* aTrack;
  Int_t iCol, iRow;
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    tPlane = tTracker->GetPlane(iPlane);

    if( tPlane->GetHitsN()>0 ) { // if some hits present
      for( Int_t iHit=1; iHit<=tPlane->GetHitsN(); iHit++) { //loop on hits (starts at 1 !!)
        aHit = (DHit*)tPlane->GetHit( iHit);
        //printf("Getting seed index for hit %d (address %x) at plane %d\n", iHit, aHit, iPlane);
        iCol = aHit->GetIndexSeed()%tPlane->GetStripsNu();
        iRow = aHit->GetIndexSeed()/tPlane->GetStripsNu();
        hHitMapIndex[iPlane-1]->SetBinContent( iCol+1, iRow+1, 1); // JB 2009/08/31
        hHitMap[iPlane-1]->Fill( aHit->GetPositionUhit(), aHit->GetPositionVhit(), 1);
        hHitMapTel[iPlane-1]->Fill( tPlane->PlaneToTracker(*(aHit->GetPosition()))(0), tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), 1);
        if( aHit->GetFound() ) hHitFoundMapTel[iPlane-1]->Fill( tPlane->PlaneToTracker(*(aHit->GetPosition()))(0), tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), 1);
        if(fVerbose) printf("MRaw::DisplayHits2D  pl %d, hit[%d=(%d,%d)=(%f,%f)]%f\n", iPlane, iHit, aHit->GetIndexSeed()%tPlane->GetStripsNu(), aHit->GetIndexSeed()/tPlane->GetStripsNu(), aHit->GetPositionUhit(), aHit->GetPositionVhit(), aHit->GetClusterPulseSum());

        // If plane belongs to some ladder
        if( nLadders>0 && tPlane->GetLadderNumber()>0 ) {
          if(fVerbose) cout << "Filling hits of plane " << iPlane << " into ladder " << tPlane->GetLadderNumber() << endl;
          hHitMapLadder[tPlane->GetLadderNumber()-1]->Fill(tPlane->PlaneToTracker(*(aHit->GetPosition()))(0), tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), 1);
          if( aHit->GetFound() ) hHitFoundMapLadder[tPlane->GetLadderNumber()-1]->Fill( tPlane->PlaneToTracker(*(aHit->GetPosition()))(0), tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), 1);
        }

      } //end loop on hits

    } // end if some hits present

    for( Int_t iTrack=1; iTrack<=tTracker->GetTracksN(); iTrack++ ) { // loop on tracks, JB 2009/09/09
      aTrack = tTracker->GetTrack(iTrack);
      hTrackMapIndex[iPlane-1]->Fill( (Int_t)tPlane->Intersection(aTrack)(0)/tPlane->GetStripPitch()(0)+centerUbin[iPlane-1], (Int_t)tPlane->Intersection(aTrack)(1)/tPlane->GetStripPitch()(1)+centerVbin[iPlane-1], aTrack->GetNumber());
      hTrackMap[iPlane-1]->Fill( tPlane->Intersection(aTrack)(0), tPlane->Intersection(aTrack)(1), aTrack->GetNumber());
      DR3 posInPlane = aTrack->Intersection(tPlane);
      hTrackMapTel[iPlane-1]->Fill( tPlane->PlaneToTracker(posInPlane)(0), tPlane->PlaneToTracker(posInPlane)(1), aTrack->GetNumber());
      /*
       hVertexMapIndex[iPlane-1]->Fill( aTrack->GetVertexX(), aTrack->GetVertexY(), aTrack->GetNumber() );       // LC 2012/12/18.
       hVertexMap[iPlane-1]->Fill( aTrack->GetVertexX(), aTrack->GetVertexY(), aTrack->GetNumber() );            // LC 2012/12/18.
       hVertexMapTel[iPlane-1]->Fill( aTrack->GetVertexX(), aTrack->GetVertexY(), aTrack->GetNumber() );         // LC 2012/12/18.
       */
      if(fVerbose) printf("MRaw::DisplayHits2D  pl %d, track[%d] = (%.1f,%.1f) in plane, (%.1f, %.1f) in telescope.\n", iPlane, iTrack, tPlane->Intersection(aTrack)(0), tPlane->Intersection(aTrack)(1), tPlane->PlaneToTracker(posInPlane)(0), tPlane->PlaneToTracker(posInPlane)(1));

      if( nLadders>0 && tPlane->GetLadderNumber()>0 ) { // if ladders
        if(fVerbose) cout << "Filling tracks in plane " << iPlane << " into ladder " << tPlane->GetLadderNumber() << endl;
        hTrackMapLadder[tPlane->GetLadderNumber()-1]->Fill( tPlane->PlaneToTracker(posInPlane)(0), tPlane->PlaneToTracker(posInPlane)(1), aTrack->GetNumber());
      } // end if ladders

    } // end loop on tracks

    pad->cd(iPlane);
    if( ifIndex==1 ) {
      hHitMapIndex[iPlane-1]->DrawCopy("P");
      if( ifTrack==2 ) {
        hTrackMapIndex[iPlane-1]->DrawCopy("Psame");
        //        hVertexMapIndex[iPlane-1]->DrawCopy("Psame");
      }
      else if( ifTrack==1 ) {
        hTrackMapIndex[iPlane-1]->DrawCopy("TextSame");
        //        hVertexMapIndex[iPlane-1]->DrawCopy("TextSame");
      }
    }
    else if( ifIndex==0 ) {
      hHitMap[iPlane-1]->DrawCopy("P"); // RDM210509
      if( ifTrack==2 ) {
        hTrackMap[iPlane-1]->DrawCopy("Psame");
        //        hVertexMap[iPlane-1]->DrawCopy("Psame");
      }
      else if( ifTrack==1 ) {
        hTrackMap[iPlane-1]->DrawCopy("TextSame");
        //        hVertexMap[iPlane-1]->DrawCopy("TextSame");
      }
    }
    else if( ifIndex==2 ) {
      hHitMapTel[iPlane-1]->DrawCopy("P");
      hHitFoundMapTel[iPlane-1]->DrawCopy("Psame");
      if( ifTrack==2 ) {
        hTrackMapTel[iPlane-1]->DrawCopy("Psame");
        //        hVertexMapTel[iPlane-1]->DrawCopy("Psame");
      }
      else if( ifTrack==1 ) {
        hTrackMapTel[iPlane-1]->DrawCopy("TextSame");
        //        hVertexMapTel[iPlane-1]->DrawCopy("TextSame");
      }
    }

    for( Short_t iTrig=0; iTrig<tAcq->GetNumberOfTriggers(); iTrig++) {
      if( ifIndex==1 ) {
        triggerLine[iPlane-1]->DrawLine( 0, tAcq->GetTriggerAt(iTrig), tPlane->GetStripsNu(), tAcq->GetTriggerAt(iTrig));
        if(fVerbose) printf("Drawing trigger line at v=%d\n", tAcq->GetTriggerAt(iTrig));
      }
      else if( ifIndex==0 ){
        triggerLine[iPlane-1]->DrawLine( -tPlane->GetStripPitch()(0)*tPlane->GetStripsNu()/2., (tAcq->GetTriggerAt(iTrig)-centerVbin[iPlane-1])*tPlane->GetStripPitch()(1), tPlane->GetStripPitch()(0)*tPlane->GetStripsNu()/2., (tAcq->GetTriggerAt(iTrig)-centerVbin[iPlane-1])*tPlane->GetStripPitch()(1));
        if(fVerbose) printf("Drawing trigger line (%d) at v=%.0f\n", tAcq->GetTriggerAt(iTrig), (tAcq->GetTriggerAt(iTrig)-centerVbin[iPlane-1])*tPlane->GetStripPitch()(1));
      }
    }

    //if (tPlane->GetStatus() == 3) {
    if( ifIndex==1 ) {
      geomBoxIndex[iPlane-1]->Draw("l");
    }
    else {
      geomBox[iPlane-1]->Draw("l");
      geomPlaneBox[iPlane-1]->Draw("l");
    }
    //}

    hHitMapIndex[iPlane-1]->Delete(); //RDM210509
    hHitMap[iPlane-1]->Delete(); //RDM210509
    hTrackMap[iPlane-1]->Delete(); //RDM210509
    hTrackMapIndex[iPlane-1]->Delete();
    hHitMapTel[iPlane-1]->Delete(); //RDM210509
    hHitFoundMapTel[iPlane-1]->Delete(); //RDM210509
    hTrackMapTel[iPlane-1]->Delete(); //RDM21050
    //    hVertexMapIndex[iPlane-1]->Delete();
    //    hVertexMapTel[iPlane-1]->Delete();
    //    hVertexMap[iPlane-1]->Delete();

  } //end loop on planes

  cdisplayhit->Update();


  if( nLadders>0 ) { // if ladders
    for( Int_t iLadder=1; iLadder<=nLadders; iLadder++) { // loop on ladders
      if( fVerbose ) cout << "Displaying for ladder " << iLadder << endl;
      cdisplayladder->cd( iLadder);
      hHitMapLadder[iLadder-1]->DrawCopy("P");
      hHitFoundMapLadder[iLadder-1]->DrawCopy("Psame");
      if( ifTrack==2 ) {
        hTrackMapLadder[iLadder-1]->DrawCopy("Psame");
      }
      else if( ifTrack==1 ) {
        hTrackMapLadder[iLadder-1]->DrawCopy("TextSame");
      }
    } // end loop on ladders
  } // end if ladders


  cout << "\n Event number : " << fSession->GetCurrentEventNumber()<<" ";
  cout << " timestamp : " << fSession->GetCurrentEventNumber()<<" ";

}

//______________________________________________________________________________
//
void MRaw::DisplayTracks()
{
  // Display the tracks event by event
  // in several views
  // Call by gTAF->GetRaw()->DisplayTracks()
  //
  // JB, 2011/07/26

  // Geometry (um)
  Double_t minZ = -10000;
  Double_t maxZ = +10000;
  Double_t minX = -10000;
  Double_t maxX = +10000;
  Double_t minY = -10000;
  Double_t maxY = +10000;

  fSession->SetEvents(3);
  DTracker *tTracker  =  fSession->GetTracker();
  Int_t nPlanes = tTracker->GetPlanesN();
  Double_t zFirstPlane = tTracker->GetPlane(1)->GetPosition()(2);
  Double_t zLastPlane  = tTracker->GetPlane(nPlanes)->GetPosition()(2);

  TCanvas *cdisplaytrack;
  TObject* g = gROOT->FindObject("cdisplaytrack") ;
  if (g) {
    cdisplaytrack = (TCanvas*)g;
  }
  else {
    cdisplaytrack = new TCanvas("cdisplaytrack", "Inspect Tracks", 5, 5,800,700);
  }
  cdisplaytrack->Clear();
  cdisplaytrack->UseCurrentStyle();
  TPaveLabel* label = new TPaveLabel();
  Char_t canvasTitle[200];
  TPad *pad = new TPad("pad","",0.,0.,1.,0.965);
  pad->Draw();
  pad->Divide( 2, 2);
  gStyle->SetOptStat(0);

  TH2F *hTrackMapXY = new TH2F( "htrackmapxy", "Tracks in XY", 100, minX, maxX, 100, minY, maxY);
  hTrackMapXY->SetXTitle("X (#mum)");
  hTrackMapXY->SetYTitle("Y (#mum)");
  TH2F *hTrackMapXZ = new TH2F( "htrackmapxz", "Tracks in XZ", 100, minX, maxX, 100, minZ, maxZ);
  hTrackMapXZ->SetXTitle("X (#mum)");
  hTrackMapXZ->SetYTitle("Z (#mum)");
  TH2F *hTrackMapYZ = new TH2F( "htrackmapyz", "Tracks in YZ", 100, minY, maxY, 100, minZ, maxZ);
  hTrackMapYZ->SetXTitle("Y (#mum)");
  hTrackMapYZ->SetYTitle("Z (#mum)");
  TH3F *hTrackMapXYZ = new TH3F( "htrackmapxyz", "Tracks in XYZ", 100, minX, maxX, 100, minY, maxY, 100, minZ, maxZ);
  hTrackMapXYZ->SetXTitle("X (#mum)");
  hTrackMapXYZ->SetYTitle("Y (#mum)");
  hTrackMapXYZ->SetZTitle("Z (#mum)");

  // === Analysis ====
  fSession->NextRawEvent();
  tTracker->Update();
  // =================

  // === Display ====
  Int_t nTracks = tTracker->GetTracksN();

  sprintf(canvasTitle, "Run %d Event %d, %d tracks", fSession->GetRunNumber(), fSession->GetCurrentEventNumber(), nTracks);
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);

  pad->cd(1);
  hTrackMapXY->DrawCopy();
  pad->cd(2);
  hTrackMapXZ->DrawCopy();
  pad->cd(3);
  hTrackMapYZ->DrawCopy();
  pad->cd(4);
  hTrackMapXYZ->DrawCopy();

  DTrack *aTrack;
  TLine   aLine; // solid line from first to last plane of the track
  TLine   bLine; // dotted line over the whole tracker dimension

  aLine.SetLineStyle(1);
  aLine.SetLineColor(1);
  bLine.SetLineStyle(2);
  bLine.SetLineColor(1);

  Double_t x[2], y[2], z[2];
  cout << "--> " << nTracks << " tracks to display" << endl;
  for( Int_t iTrack=1; iTrack<=nTracks; iTrack++) { // loop on tracks

    aTrack = (DTrack*)tTracker->GetTrack( iTrack);

    bLine.SetLineColor( bLine.GetLineColor()+1);
    aLine.SetLineColor( aLine.GetLineColor()+1);

    DR3 trackSlope = aTrack->GetLinearFit().GetSlopeZ();
    DR3 trackOrigin = aTrack->GetLinearFit().GetOrigin();

    z[0] = minZ;
    z[1] = maxZ;
    x[0] = trackOrigin(0)+trackSlope(0)*z[0];
    x[1] = trackOrigin(0)+trackSlope(0)*z[1];
    y[0] = trackOrigin(1)+trackSlope(1)*z[0];
    y[1] = trackOrigin(1)+trackSlope(1)*z[1];

    if(fVerbose) printf(" track %d: x[%.1f, %.1f], y[%.1f, %.1f], z[%.1f, %.1f]\n", aTrack->GetNumber(), x[0], x[1], y[0], y[1], z[0], z[1]);

    // XY plane
    pad->cd(1);
    bLine.DrawLine( x[0], y[0], x[1], y[1] );
    // XZ plane
    pad->cd(2);
    bLine.DrawLine( x[0], z[0], x[1], z[1]);
    // YZ plane
    pad->cd(3);
    bLine.DrawLine( y[0], z[0], y[1], z[1]);

    z[0] = zFirstPlane;
    z[1] = zLastPlane;
    x[0] = trackOrigin(0)+trackSlope(0)*z[0];
    x[1] = trackOrigin(0)+trackSlope(0)*z[1];
    y[0] = trackOrigin(1)+trackSlope(1)*z[0];
    y[1] = trackOrigin(1)+trackSlope(1)*z[1];

    if(fVerbose) printf("      : x[%.1f, %.1f], y[%.1f, %.1f], z[%.1f, %.1f]\n", x[0], x[1], y[0], y[1], z[0], z[1]);

    // XY plane
    pad->cd(1);

    aLine.DrawLine( x[0], y[0], x[1], y[1] );

    // XZ plane
    pad->cd(2);
    aLine.DrawLine( x[0], z[0], x[1], z[1]);
    // YZ plane
    pad->cd(3);
    aLine.DrawLine( y[0], z[0], y[1], z[1]);
    // XYZ 3Dplane
    pad->cd(4);

    TPolyLine3D *line3D = new TPolyLine3D( 2, x, y, z);
    line3D->SetLineColor(aLine.GetLineColor());
    line3D->SetLineWidth(2); // removing warning: from '1.5' to '2' (has to be 'short') BH 2013/08/20
    line3D->Draw();

    TEllipse *ellipse1 = new TEllipse(aTrack->GetVertexX(),aTrack->GetVertexY(),50.0,50.0);  // a=b arbitrary = 100.
    TEllipse *ellipse2 = new TEllipse(aTrack->GetVertexX(),aTrack->GetVertexZ(),50.0,50.0);
    TEllipse *ellipse3 = new TEllipse(aTrack->GetVertexY(),aTrack->GetVertexZ(),50.0,50.0);

    pad->cd(1);
    //TEllipse ellipse1(aTrack->GetVertexX(), aTrack->GetVertexY(),10,10);
    ellipse1->SetLineColor(1);
    ellipse1->Draw();

    pad->cd(2);
    //TEllipse ellipse2(aTrack->GetVertexX(), aTrack->GetVertexZ(),10,10);
    ellipse2->SetLineColor(1);
    ellipse2->Draw();

    pad->cd(3);
    //TEllipse ellipse3(aTrack->GetVertexY(), aTrack->GetVertexZ(),10,10);
    ellipse3->SetLineColor(1);
    ellipse3->Draw();

    //if(iTrack==nTracks) pad->Update();

  } //end loop on tracks
  if(fVerbose) cout << "\n Event number : " << fSession->GetCurrentEventNumber()<<" ";

  hTrackMapXY->Delete();
  hTrackMapXZ->Delete();
  hTrackMapYZ->Delete();
  hTrackMapXYZ->Delete();

  cdisplaytrack->Update();
  // =================

}


//______________________________________________________________________________
//
void MRaw::DisplayCumulatedTracks( Int_t nEvents)
{
  // Display the tracks cumulated over nEvents events
  // in several views
  // Call by gTAF->GetRaw()->DisplayCumulatedTracks( nEvents)
  //
  // JB, 2011/07/26
  // Modified JB+VR, 2012/04/02 add slopes display
  // Modified JB, 2012/05/07 add chi2 and residuals display
  // Modified JB, 2014/08/29 add histo with #tracks per plane
  // Modified JB, 2014/11/23 add study of track multiplicity (call to)
  // Modified JB, 2015/03/15 estimation of det.eff. through multiplicity
  // Modified JB, 2015/03/16 fit of track chi2 distribution

  // Geometry (um)
  Double_t minZ =  -5000;
  Double_t maxZ = +50000;
  Double_t minX = -30000;
  Double_t maxX = +30000;
  Double_t minY = -30000;
  Double_t maxY = +30000;

  fSession->SetEvents(nEvents);

  DTracker *tTracker  =  (DTracker*)fSession->GetTracker();
  Int_t nPlanes = tTracker->GetPlanesN();
  Double_t zFirstPlane = tTracker->GetPlane(1)->GetPosition()(2);
  Double_t zLastPlane  = tTracker->GetPlane(nPlanes)->GetPosition()(2);

  // === Display 1 ====

  // Book histograms
#if 0
  TH2F *hTrackMapXY = new TH2F( "htrackmapxy", "Tracks in XY", 100, minX, maxX, 100, minY, maxY);
  hTrackMapXY->SetStats(kFALSE);
  hTrackMapXY->SetXTitle("X (#mum)");
  hTrackMapXY->SetYTitle("Y (#mum)");
  TH2F *hTrackMapXZ = new TH2F( "htrackmapxz", "Tracks in XZ", 100, minX, maxX, 100, minZ, maxZ);
  hTrackMapXZ->SetStats(kFALSE);
  hTrackMapXZ->SetXTitle("X (#mum)");
  hTrackMapXZ->SetYTitle("Z (#mum)");
  TH2F *hTrackMapYZ = new TH2F( "htrackmapyz", "Tracks in YZ", 100, minY, maxY, 100, minZ, maxZ);
  hTrackMapYZ->SetStats(kFALSE);
  hTrackMapYZ->SetXTitle("Y (#mum)");
  hTrackMapYZ->SetYTitle("Z (#mum)");
  TH2F *hTrackMapOrig = new TH2F( "htrackmaporig", "Tracks map at z=0", 200, 10*minX, 10*maxX, 200, 10*minY, 10*maxY);
  hTrackMapOrig->SetStats(1111111);
  hTrackMapOrig->SetXTitle("X (#mum)");
  hTrackMapOrig->SetYTitle("Y (#mum)");
  TH3F *hTrackMapXYZ = new TH3F( "htrackmapxyz", "Tracks in XYZ", 100, minX, maxX, 100, minY, maxY, 100, minZ, maxZ);
  hTrackMapXYZ->SetStats(kFALSE);
  hTrackMapXYZ->SetXTitle("X (#mum)");
  hTrackMapXYZ->SetYTitle("Y (#mum)");
  hTrackMapXYZ->SetZTitle("Z (#mum)");
#endif
  TH1F *hTrackSlopeX = new TH1F( "htrackslopex", "Track slopes X=f(Z)", 100, 0., 0.);
  hTrackSlopeX->SetXTitle("slope");
  TH1F *hTrackSlopeY = new TH1F( "htrackslopey", "Track slopes Y=f(Z)", 100, 0., 0.);
  hTrackSlopeY->SetXTitle("slope");
  TH1F *hTrackNHits = new TH1F( "htacknhits", "Track hit multiplicity", nPlanes+1, -.5, nPlanes+.5);
  hTrackNHits->SetXTitle("# hits in track");
  TH1F *hTrackPlanesUsed = new TH1F( "htackplanesused", "Planes used in track fit; Plane number; # tracks", nPlanes+1, -.5, nPlanes+.5);

  int Nbins_Chi2 = 500;
  double R_Chi2[2];
  R_Chi2[0] =   0.0;
  R_Chi2[1] = 250.0;
  TH1F *hTrackChi2 = new TH1F( "htrackchi2", "#chi2/ndof of tracks",Nbins_Chi2,R_Chi2[0],R_Chi2[1]);
  hTrackChi2->SetXTitle("#chi2/ndof");

  int Nbins_TrkPerEvt = 5000;
  double R_TrkPerEvt[2];
  R_TrkPerEvt[0] = 0.0 - 0.5;
  R_TrkPerEvt[1] = Nbins_TrkPerEvt + 0.5;
  Nbins_TrkPerEvt++;
  TH1F *hNTrackPerEvent = new TH1F("hNTrackPerEvent","Reconstructed Tracks per event",
				   Nbins_TrkPerEvt,R_TrkPerEvt[0],R_TrkPerEvt[1]);
  hNTrackPerEvent->SetXTitle("Nb tracks / event");

  TH1F **hResidualU = new TH1F*[nPlanes];
  TH1F **hResidualV = new TH1F*[nPlanes];
  Char_t name[50], title[100];
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {

    sprintf( name, "hresidualupl%d", iPlane);
    sprintf( title, "Hit-track horizontal residual, plane %d", iPlane);
    hResidualU[iPlane-1] = new TH1F(name, title, 200, -50., +50.);
    hResidualU[iPlane-1]->SetXTitle("U residue (#mum)");

    sprintf( name, "hresidualvpl%d", iPlane);
    sprintf( title, "Hit-track vertical residual, plane %d", iPlane);
    hResidualV[iPlane-1] = new TH1F(name, title, 200, -50., +50.);
    hResidualV[iPlane-1]->SetXTitle("V residue (#mum)");
}

  // Canvas for projective display
  TObject* g = NULL;

#if 0
  TCanvas *ccumultrack;
  TObject* g = gROOT->FindObject("ccumultrack") ;
  if (g) {
    ccumultrack = (TCanvas*)g;
  }
  else {
    ccumultrack = new TCanvas("ccumultrack", "Cumulate tracks", 5, 5,800,700);
  }
  ccumultrack->Clear();
  ccumultrack->UseCurrentStyle();
  TPaveLabel* label = new TPaveLabel();
  Char_t canvasTitle[200];
  TPad *pad = new TPad("pad","",0.,0.,1.,0.965);
  pad->Draw();
  pad->Divide( 2, 2);

  sprintf(canvasTitle, "Run %d, cumul over %d events", fSession->GetRunNumber(), nEvents);
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);

  pad->cd(1);
  hTrackMapXY->DrawCopy();
  pad->cd(2);
  hTrackMapXZ->DrawCopy();
  pad->cd(3);
  hTrackMapYZ->DrawCopy();

  // Canvas for 3D display
  TCanvas *ccumultrack2;
  g = gROOT->FindObject("ccumultrack2") ;
  if (g) {
    ccumultrack2 = (TCanvas*)g;
  }
  else {
    ccumultrack2 = new TCanvas("ccumultrack2", "Cumulate tracks 3D", 25, 25,800,700);
  }
  ccumultrack2->Clear();
  ccumultrack2->UseCurrentStyle();
  ccumultrack2->cd();
  hTrackMapXYZ->DrawCopy();
#endif

  // Canvases for track properties
  TCanvas *ccumultrack3;
  g = gROOT->FindObject("ccumultrack3") ;
  if (g) {
    ccumultrack3 = (TCanvas*)g;
  }
  else {
    ccumultrack3 = new TCanvas("ccumultrack3", "Cumulate tracks Properties", 50, 50,900,700);
  }
  ccumultrack3->Clear();
  ccumultrack3->UseCurrentStyle();
  ccumultrack3->Divide(3,2);

  TCanvas *ccumultrack4;
  g = gROOT->FindObject("ccumultrack4") ;
  if (g) {
    ccumultrack4 = (TCanvas*)g;
  }
  else {
    ccumultrack4 = new TCanvas("ccumultrack4", "Cumulate tracks Residual-U", 75, 75,800,700);
  }
  ccumultrack4->Clear();
  ccumultrack4->UseCurrentStyle();
  ccumultrack4->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);

  TCanvas *ccumultrack5;
  g = gROOT->FindObject("ccumultrack5") ;
  if (g) {
    ccumultrack5 = (TCanvas*)g;
  }
  else {
    ccumultrack5 = new TCanvas("ccumultrack5", "Cumulate tracks Residual-V", 100, 100,800,700);
  }
  ccumultrack5->Clear();
  ccumultrack5->UseCurrentStyle();
  ccumultrack5->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);


  // === Event Loop ====

  DTrack *aTrack;
  DPlane *aPlane;
  DHit   *aHit;
  TLine   aLine, bLine;
  aLine.SetLineStyle(1);
  aLine.SetLineColor(1);
  bLine.SetLineStyle(2);
  bLine.SetLineColor(1);
  Double_t x[2], y[2], z[2];

  //Loop over the requested number of events
  for( Int_t iEvt=0; iEvt < nEvents; iEvt++) {
    if( !(fSession->NextRawEvent()) ) break; // JB 2009/06/26

    int NtrkEvtCounter = 0;

    // === Analysis ====
    tTracker->Update();
    Int_t nTracks = tTracker->GetTracksN();
    // =================

    //cout << "evt " << iEvt << ": " << nTracks << " tracks to display" << endl;
    for( Int_t iTrack=1; iTrack<=nTracks; iTrack++) { // loop on tracks
      aTrack = (DTrack*)tTracker->GetTrack( iTrack);
      bLine.SetLineColor( 1+bLine.GetLineColor()%10);
      aLine.SetLineColor( 1+aLine.GetLineColor()%10);

      //if(aTrack->GetChiSquare() > 2.0) continue;

      DR3 trackSlope = aTrack->GetLinearFit().GetSlopeZ();
      DR3 trackOrigin = aTrack->GetLinearFit().GetOrigin();
      //hTrackMapOrig->Fill( trackOrigin(0), trackOrigin(1));
      hTrackSlopeX->Fill( trackSlope(0));
      hTrackSlopeY->Fill( trackSlope(1));
      hTrackNHits->Fill( aTrack->GetHitsNumber());
      hTrackChi2->Fill( aTrack->GetChiSquare()); // this is directly chi2/ndf

      for( Int_t iHit=0; iHit<aTrack->GetHitsNumber( ); iHit++ ) {
        aHit   = (DHit*)aTrack->GetHit( iHit);
        aPlane = (DPlane*)aHit->GetPlane();
        hTrackPlanesUsed->Fill( aPlane->GetPlaneNumber());
        DR3 impactPosition = aTrack->Intersection( aPlane);
        impactPosition -= *(aHit->GetPosition());
        hResidualU[aPlane->GetPlaneNumber()-1]->Fill( impactPosition(0));
        hResidualV[aPlane->GetPlaneNumber()-1]->Fill( impactPosition(1));
      }

#if 0
      z[0] = zFirstPlane;
      z[1] = zLastPlane;
      x[0] = trackOrigin(0)+trackSlope(0)*z[0];
      x[1] = trackOrigin(0)+trackSlope(0)*z[1];
      y[0] = trackOrigin(1)+trackSlope(1)*z[0];
      y[1] = trackOrigin(1)+trackSlope(1)*z[1];
      //printf("      : x[%.1f, %.1f], y[%.1f, %.1f], z[%.1f, %.1f]\n", x[0], x[1], y[0], y[1], z[0], z[1]);
      // XY plane
      pad->cd(1);
      aLine.DrawLine( x[0], y[0], x[1], y[1]);
      // XZ plane
      pad->cd(2);
      aLine.DrawLine( x[0], z[0], x[1], z[1]);
      // YZ plane
      pad->cd(3);
      aLine.DrawLine( y[0], z[0], y[1], z[1]);

      z[0] = minZ;
      z[1] = maxZ;
      x[0] = trackOrigin(0)+trackSlope(0)*z[0];
      x[1] = trackOrigin(0)+trackSlope(0)*z[1];
      y[0] = trackOrigin(1)+trackSlope(1)*z[0];
      y[1] = trackOrigin(1)+trackSlope(1)*z[1];
      //printf(" track %d: x[%.1f, %.1f], y[%.1f, %.1f], z[%.1f, %.1f]\n", aTrack->GetNumber(), x[0], x[1], y[0], y[1], z[0], z[1]);
      // XY plane
      pad->cd(1);
      bLine.DrawLine( x[0], y[0], x[1], y[1]);
      // XZ plane
      pad->cd(2);
      bLine.DrawLine( x[0], z[0], x[1], z[1]);
      // YZ plane
      pad->cd(3);
      bLine.DrawLine( y[0], z[0], y[1], z[1]);

      // XYZ 3Dplane
      ccumultrack2->cd();
      TPolyLine3D *line3D = new TPolyLine3D( 2, x, y, z);
      line3D->SetLineColor(aLine.GetLineColor());
      line3D->SetLineWidth(2); // removing warning: from '1.5' to '2' (has to be 'short') BH 2013/08/20
      line3D->Draw();
#endif

      NtrkEvtCounter++;
    } //end loop on tracks

    hNTrackPerEvent->Fill(NtrkEvtCounter);
    //if(NtrkEvtCounter > 10) cout << "In " << iEvt << " found " << NtrkEvtCounter << " tracks!!!" << endl;

  } // END LOOP ON EVENTS
  fSession->GetDataAcquisition()->PrintStatistics();
  tTracker->PrintStatistics();

#if 0
  // Track origins in XY at z=0
  pad->cd(4);
  hTrackMapOrig->DrawCopy();
#endif

  // track properties
  ccumultrack3->cd(1);
  if(hTrackSlopeX->GetEntries()>0) {
    TF1* fTrackSlopeX = new TF1("fTrackSlopeX",
				"[2]*(1.0/(sqrt(2.0*TMath::Pi())*[1]))*TMath::Exp(-0.5*TMath::Power((x-[0])/[1],2))",
				hTrackSlopeX->GetMean() - 3.0*hTrackSlopeX->GetRMS(),
				hTrackSlopeX->GetMean() + 3.0*hTrackSlopeX->GetRMS());
    fTrackSlopeX->SetParameter(0,hTrackSlopeX->GetMean());
    fTrackSlopeX->SetParameter(1,hTrackSlopeX->GetRMS());
    fTrackSlopeX->SetParameter(2,hTrackSlopeX->GetEntries());
    fTrackSlopeX->SetParName(0,"#mu");
    fTrackSlopeX->SetParName(1,"#sigma");
    fTrackSlopeX->SetParName(2,"Yield");

    fTrackSlopeX->SetLineColor(2);
    hTrackSlopeX->GetXaxis()->SetNdivisions(605);
    hTrackSlopeX->Fit(fTrackSlopeX,"R");
    hTrackSlopeX->SetLineColor(4);
    hTrackSlopeX->SetLineWidth(2);
    hTrackSlopeX->Draw();
  }
  ccumultrack3->cd(2);
  if(hTrackSlopeY->GetEntries()>0) {
    TF1* fTrackSlopeY = new TF1("fTrackSlopeY",
				"[2]*(1.0/(sqrt(2.0*TMath::Pi())*[1]))*TMath::Exp(-0.5*TMath::Power((x-[0])/[1],2))",
				hTrackSlopeY->GetMean() - 3.0*hTrackSlopeY->GetRMS(),
				hTrackSlopeY->GetMean() + 3.0*hTrackSlopeY->GetRMS());
    fTrackSlopeY->SetParameter(0,hTrackSlopeY->GetMean());
    fTrackSlopeY->SetParameter(1,hTrackSlopeY->GetRMS());
    fTrackSlopeY->SetParameter(2,hTrackSlopeY->GetEntries());
    fTrackSlopeY->SetParName(0,"#mu");
    fTrackSlopeY->SetParName(1,"#sigma");
    fTrackSlopeY->SetParName(2,"Yield");
    fTrackSlopeY->SetLineColor(2);

    hTrackSlopeY->GetXaxis()->SetNdivisions(605);
    hTrackSlopeY->Fit(fTrackSlopeY,"R");
    hTrackSlopeY->SetLineColor(4);
    hTrackSlopeY->SetLineWidth(2);
    hTrackSlopeY->Draw();
  }
  ccumultrack3->cd(3);
  double NTrackMax = -999.0;
  for(int iTrk=0;iTrk<hNTrackPerEvent->GetXaxis()->GetNbins();iTrk++) {
    if(hNTrackPerEvent->GetBinContent(hNTrackPerEvent->GetXaxis()->GetNbins() - iTrk) > 0) {
      int idx = hNTrackPerEvent->GetXaxis()->GetNbins() - iTrk;
      NTrackMax = hNTrackPerEvent->GetBinCenter(idx) + 0.5*hNTrackPerEvent->GetBinWidth(idx);
      //cout << "bin = " << idx << "  NTrackMax = " << NTrackMax << endl;
      break;
    }
  }
  double NTrackMin = -999.0;
  for(int iTrk=0;iTrk<hNTrackPerEvent->GetXaxis()->GetNbins();iTrk++) {
    if(hNTrackPerEvent->GetBinContent(iTrk+1) > 0) {
      int idx = iTrk+1;
      NTrackMin = hNTrackPerEvent->GetBinCenter(idx) - 0.5*hNTrackPerEvent->GetBinWidth(idx);
      //cout << "bin = " << idx << "  NTrackMin = " << NTrackMin << endl;
      break;
    }
  }
  double porcent_tmp = 0.05;
  double delta       = NTrackMax - NTrackMin;
  NTrackMin -= porcent_tmp*delta;
  NTrackMax += porcent_tmp*delta;
  if(NTrackMin < hNTrackPerEvent->GetBinCenter(1) - 0.5*hNTrackPerEvent->GetBinWidth(1)) {
    NTrackMin = hNTrackPerEvent->GetBinCenter(1) - 0.5*hNTrackPerEvent->GetBinWidth(1);
  }
  //cout << "(NTrackMin,NTrackMax) = " << NTrackMin << "  " << NTrackMax << endl;
  hNTrackPerEvent->SetAxisRange(NTrackMin,NTrackMax,"X");
  hNTrackPerEvent->Draw();
  ccumultrack3->cd(4);
  if(hTrackNHits->GetEntries()>0) {
    hTrackNHits->Draw();
    gPad->SetGridx();
  }
  ccumultrack3->cd(5);
  if(hTrackPlanesUsed->GetEntries()>0) {
    hTrackPlanesUsed->Draw();
    gPad->SetGridx();
  }
  ccumultrack3->cd(6);
  TF1 *fchi2 = new TF1("fchi2","[0]*TMath::GammaDist(x,[1]/2.,0.,2.)",0,20);
  if(hTrackChi2->GetEntries()>0) {
    double Max = -1.0e+20;
    if(hTrackChi2->GetBinContent(hTrackChi2->GetXaxis()->GetNbins()) > 0.0) {
      Max  = hTrackChi2->GetBinCenter(hTrackChi2->GetXaxis()->GetNbins());
      Max += hTrackChi2->GetBinWidth(hTrackChi2->GetXaxis()->GetNbins())*0.48;
    }
    else {
      for(int ichi2=0;ichi2<hTrackChi2->GetXaxis()->GetNbins();ichi2++) {
	int idx = hTrackChi2->GetXaxis()->GetNbins() - ichi2;
	if(hTrackChi2->GetBinContent(idx) > 0.0) {
	  Max  = hTrackChi2->GetBinCenter(idx);
	  Max += hTrackChi2->GetBinWidth(idx)*0.48;
	  break;
	}
      }
    }
    hTrackChi2->SetAxisRange(hTrackChi2->GetXaxis()->GetXmin(),
			     Max,"X");

    hTrackChi2->SetLineColor(4);
    hTrackChi2->SetLineWidth(2);
    hTrackChi2->Draw();
    fchi2->SetParNames( "#tracks", "ndf");
    fchi2->SetParameters( hTrackChi2->Integral(), nPlanes-tTracker->GetNDUT());
    //fchi2->FixParameter( 0, hTrackChi2->Integral());
    fchi2->SetParLimits( 1, 0., 100.);
    hTrackChi2->Fit("fchi2");
  }

  // Study track multiplicity to infer det. eff.
  // JB 2014/11/15
  StudyTrackMultiplicity( hTrackPlanesUsed, hTrackNHits, nEvents);

  // Study Tracking Efficiency
  // QL, 2015/04/10
  // StudyTrackingEfficiency(nEvents); // Already used for bar3->AddButton

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    ccumultrack4->cd(iPlane);
    if(hResidualU[iPlane-1]->GetEntries()>0) hResidualU[iPlane-1]->Draw();
    ccumultrack5->cd(iPlane);
    if(hResidualV[iPlane-1]->GetEntries()>0) hResidualV[iPlane-1]->Draw();
  }

#if 0
  hTrackMapXY->Delete();
  hTrackMapXZ->Delete();
  hTrackMapYZ->Delete();
#endif

#if 0
  ccumultrack->Update();
  ccumultrack2->Update();
#endif
  ccumultrack3->Update();
  ccumultrack4->Update();
  ccumultrack5->Update();

  // =================
  // Save canvas and histos
  //cd to result dir
  // added JB 2012/05/11
  Char_t rootFile[300];
  sprintf(rootFile,"%sTracksMap_run%d.root",fSession->GetResultDirName().Data(),fSession->GetRunNumber());
  sprintf(rootFile,"%s", fTool.LocalizeDirName( rootFile)); // JB 2011/07/07
  cout << "\n-- Saving histos and canvas into " << rootFile << endl;
  TFile fRoot(rootFile,"RECREATE");
  //ccumultrack->Write();
  //ccumultrack2->Write();
  ccumultrack3->Write();
  ccumultrack4->Write();
  ccumultrack5->Write();
  //hTrackMapOrig->Write();
  hTrackSlopeX->Write();
  hTrackSlopeY->Write();
  hTrackNHits->Write();
  hTrackChi2->Write();
  hNTrackPerEvent->Write();
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    hResidualU[iPlane-1]->Write();
    hResidualV[iPlane-1]->Write();
  }
  fRoot.Close();

}



//______________________________________________________________________________
//
void MRaw::DisplayCumulatedSubtracks( Int_t nEvents, Int_t dutPlaneNb)
{
  // Display over nEvents events
  //  the residual between the subtrack extrapolation to a given DUT (dutPlaneNb)
  //  and the hit in this DUT.
  // Note that subtrack are track refit from a full track (see DSetup),
  //  if no subtrack setup was required, use normal full tracks.
  // It no DUT specified, pick-up the first one.
  //
  // JB, 2014/12/23

  fSession->SetEvents(nEvents);

  // Find DUT plane
  DTracker *tTracker  =  (DTracker*)fSession->GetTracker();

  DPlane *dutPlane = NULL;
  if( 0<dutPlaneNb && dutPlaneNb<=tTracker->GetPlanesN() ) { // if DUT specified
    dutPlane = tTracker->GetPlane( dutPlaneNb);
  }
  else { // no DUT specified pick-up first one
    for ( Int_t iPlane=1; iPlane<=tTracker->GetPlanesN(); iPlane++) {
      dutPlane = tTracker->GetPlane( iPlane);
      if( dutPlane->GetStatus() == 3 ) {
        dutPlaneNb = dutPlane->GetPlaneNumber();
        break;
      }
    }
  }
  if( dutPlane==NULL ) {
    cout << "WARNING, DisplayCumulatedSubtrack did not find a dutPlane! ->LEAVING" <<endl;
    return;
  }
  cout << " Will use plane " << dutPlane->GetPlaneNumber() << " as DUT plane." << endl;


  // Book histograms
  TH1F *hTrackSlopeX = new TH1F( "htrackslopex", "Track slopes X=f(Z);slope", 100, 0., 0.);
  hTrackSlopeX->SetLineStyle(2);
  TH1F *hTrackSlopeY = new TH1F( "htrackslopey", "Track slopes Y=f(Z);slope", 100, 0., 0.);
  hTrackSlopeY->SetLineStyle(2);
  Char_t name[50], title[100];
  sprintf( name, "hresidualupl%d", dutPlaneNb);
  sprintf( title, "Hit-track horizontal residual, plane %d;U residue (#mum)", dutPlaneNb);
  TH1F *hResidualU = new TH1F(name, title, 200, -50., +50.);
  hResidualU->SetLineStyle(2);
  sprintf( name, "hresidualvpl%d", dutPlaneNb);
  sprintf( title, "Hit-track vertical residual, plane %d;V residue (#mum)", dutPlaneNb);
  TH1F *hResidualV = new TH1F(name, title, 200, -50., +50.);
  hResidualV->SetLineStyle(2);

  TH1F *hSubTrackSlopeX = new TH1F( "hsubtrackslopex", "SubTrack slopes X=f(Z);slope", 100, 0., 0.);
  TH1F *hSubTrackSlopeY = new TH1F( "hsubtrackslopey", "SubTrack slopes Y=f(Z);slope", 100, 0., 0.);
  sprintf( name, "hsubresidualupl%d", dutPlaneNb);
  sprintf( title, "Hit-subtrack horizontal residual, plane %d", dutPlaneNb);
  TH1F *hSubResidualU = new TH1F(name, title, 200, -50., +50.);
  hSubResidualU->SetXTitle("U residue (#mum)");
  sprintf( name, "hsubresidualvpl%d", dutPlaneNb);
  sprintf( title, "Hit-subtrack vertical residual, plane %d", dutPlaneNb);
  TH1F *hSubResidualV = new TH1F(name, title, 200, -50., +50.);
  hSubResidualV->SetXTitle("V residue (#mum)");

  // Canvas for projective display
  TCanvas *ccumulsubtrack;
  TObject* g = gROOT->FindObject("ccumulsubtrack") ;
  if (g) {
    ccumulsubtrack = (TCanvas*)g;
  }
  else {
    ccumulsubtrack = new TCanvas("ccumulsubtrack", "Cumulate subtracks", 5, 5,800,700);
  }
  ccumulsubtrack->Clear();
  ccumulsubtrack->UseCurrentStyle();
  TPaveLabel* label = new TPaveLabel();
  Char_t canvasTitle[200];
  TPad *pad = new TPad("pad","",0.,0.,1.,0.965);
  pad->Draw();
  pad->Divide( 2, 2);

  sprintf(canvasTitle, "Run %d, cumul over %d events", fSession->GetRunNumber(), nEvents);
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);


  // === Event Loop ====

  DTrack *aTrack, *aSubTrack;
  DHit   *aHit;
  Bool_t  hitAssociated = kFALSE;

  //Loop over the requested number of events
  for( Int_t iEvt=0; iEvt < nEvents; iEvt++) {
    if( !(fSession->NextRawEvent()) ) break; // JB 2009/06/26

    // === Analysis ====
    tTracker->Update();
    Int_t nTracks = tTracker->GetTracksN();
    // =================

    if(fDebugRaw) cout << "evt " << iEvt << ": " << nTracks << " tracks to display" << endl;
    for( Int_t iTrack=1; iTrack<=nTracks; iTrack++) { // loop on tracks
      aTrack = (DTrack*)tTracker->GetTrack( iTrack);

      DR3 trackSlope = aTrack->GetLinearFit().GetSlopeZ();
      hTrackSlopeX->Fill( trackSlope(0));
      hTrackSlopeY->Fill( trackSlope(1));

      aHit   = (DHit*)tTracker->nearest_hit( aTrack, dutPlaneNb, hitAssociated);
      if( hitAssociated ) printf("WARNING in DisplayCumulatedSubtrack, event %d, for track %d and plane %d (status %d), hit %d was found associated with the track!\n", iEvt, aTrack->GetNumber(), dutPlaneNb, dutPlane->GetStatus(), aHit->GetNumber());
      if ( aHit != NULL ) { // If there is some hit

        DR3 impactPosition = aTrack->Intersection( dutPlane);
        if(fDebugRaw) {
          printf("impactPosition: %.1f, %.1f, %.1f\n", impactPosition(0), impactPosition(1), impactPosition(2));
          DR3 *hitPosition = aHit->GetPosition();
          printf("hitPosition: %.1f, %.1f, %.1f\n", (*hitPosition)(0), (*hitPosition)(1), (*hitPosition)(2));
        }
        impactPosition -= *(aHit->GetPosition());
        if(fDebugRaw) printf("residual: %.1f, %.1f, %.1f\n", impactPosition(0), impactPosition(1), impactPosition(2));
        hResidualU->Fill( impactPosition(0));
        hResidualV->Fill( impactPosition(1));

        if( tTracker->GetNPlanesForSubTrack()>1 ) {
          aSubTrack = (DTrack*)(tTracker->GetSubTrack( aTrack->GetNumber()));
          DR3 subtrackSlope = aSubTrack->GetLinearFit().GetSlopeZ();
          hSubTrackSlopeX->Fill( subtrackSlope(0));
          hSubTrackSlopeY->Fill( subtrackSlope(1));
          DR3 trackPos = aSubTrack->Intersection( dutPlane);
          if(fDebugRaw) printf("SubimpactPosition: %.1f, %.1f, %.1f\n", trackPos(0), trackPos(1), trackPos(2));
          trackPos -= *(aHit->GetPosition());
          if(fDebugRaw) printf("subresidual: %.1f, %.1f, %.1f\n", trackPos(0), trackPos(1), trackPos(2));
          hSubResidualU->Fill( trackPos(0));
          hSubResidualV->Fill( trackPos(1));
        }

      } // end If there is some hit

    } //end loop on tracks

  } // END LOOP ON EVENTS
  fSession->GetDataAcquisition()->PrintStatistics();
  tTracker->PrintStatistics();


  // === Display ====

  pad->cd(1);
  hTrackSlopeX->DrawCopy();
  if( tTracker->GetNPlanesForSubTrack()>1 ) hSubTrackSlopeX->DrawCopy("same");
  pad->cd(2);
  hTrackSlopeY->DrawCopy();
  if( tTracker->GetNPlanesForSubTrack()>1 ) hSubTrackSlopeY->DrawCopy("same");
  pad->cd(3);
  hResidualU->DrawCopy();
  if( tTracker->GetNPlanesForSubTrack()>1 ) hSubResidualU->DrawCopy("same");
  pad->cd(4);
  hResidualV->DrawCopy();
  if( tTracker->GetNPlanesForSubTrack()>1 ) hSubResidualV->DrawCopy("same");

  ccumulsubtrack->Update();


  // =================
  // Save canvas and histos
  //cd to result dir
  // added JB 2012/05/11
  Char_t rootFile[300];
  sprintf(rootFile,"%sSubtrack_run%d.root",fSession->GetResultDirName().Data(),fSession->GetRunNumber());
  sprintf(rootFile,"%s", fTool.LocalizeDirName( rootFile)); // JB 2011/07/07
  cout << "\n-- Saving histos and canvas into " << rootFile << endl;
  TFile fRoot(rootFile,"RECREATE");
  ccumulsubtrack->Write();
  hTrackSlopeX->Write();
  hTrackSlopeY->Write();
  hResidualU->Write();
  hResidualV->Write();
  if( tTracker->GetNPlanesForSubTrack()>1 ) {
    hSubTrackSlopeX->Write();
    hSubTrackSlopeY->Write();
    hSubResidualU->Write();
    hSubResidualV->Write();
  }
  fRoot.Close();

  // Delete histo in case the method is called again
  hTrackSlopeX->Delete();
  hTrackSlopeY->Delete();
  hResidualU->Delete();
  hResidualV->Delete();
  hSubTrackSlopeX->Delete();
  hSubTrackSlopeY->Delete();
  hSubResidualU->Delete();
  hSubResidualV->Delete();


}


//______________________________________________________________________________
//
void MRaw::DisplayCumulatedHits2D( Int_t nEvents, Bool_t ifDrawTrack,
				   Bool_t Define_Range,
				   int bins,
				   double Xmin, double Xmax,
           double Ymin, double Ymax,
           Bool_t Define_QRange,
           int qbins,
           double qmax)
{
  // Display the hit position for each plane cumulated over the requested number of events
  //  and display hit properties
  //
  // Call by gTAF->GetRaw()->DisplayCumulatedHits2D()
  //
  // Inputs:
  //   o nEvents is the nb of events to analyse
  //   o ifDrawTrack turns on the display of the track extrapolation map
  //   o if Define_Range is true, use arguments bins and X/Y_min/max to define 2D range
  //   o if Define_QRange is true, use arguments qbins and qmax to define charge range
  //
  // Outputs:
  //   o HitMap_run%d.root with all plots
  //
  // JB, 2009/05/12
  // Last Modified JB 2009/08/31 binning of histos and bin index in filling
  // Last Modified JB 2010/10/25 counter of reconstructed hits
  // Last Modified JB 2010/11/02 superpose display of track impacts
  // Last Modified JB 2012/08/17 add plots for hit properties (analog output)
  // Last Modified JB 2013/10/08 additional analog hit plots
  // Last Modified JB 2013/10/30 additional analog hit plots and #hits/event
  // Last Modified JB 2013/11/11 protection against empty histograms
  // Last Modified JB 2013/11/08 use of GetSNseed()
  // Last Modified JB 2014/05/14 plot #hits/event on independant canvas
  // Last Modified JB 2015/05/25 plot hit timestamp added
  // Last Modified QL 2015/10/23 plot #hits vs #event added

  fSession->SetEvents(nEvents);

  Int_t nHitsReconstructed = 0;

  TCanvas *ccumulhit;
  TObject* g = gROOT->FindObject("ccumulhit") ;
  if (g) {
    ccumulhit = (TCanvas*)g;
  }
  else {
    ccumulhit = new TCanvas("ccumulhit", "Cumulate Hits", 5, 5,800,700);
  }
  ccumulhit->Clear();
  ccumulhit->UseCurrentStyle();
  TPaveLabel* label = new TPaveLabel();
  Char_t canvasTitle[200];
  sprintf(canvasTitle, "Run %d, cumul over %d events", fSession->GetRunNumber(), nEvents);
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad = new TPad("pad","",0.,0.,1.,0.965);
  pad->Draw();

  DTracker *tTracker  =  fSession->GetTracker();
  DPlane* tPlane = NULL;
  DTrack *aTrack = NULL;
  DHit *aHit = NULL;
  //DPixel *aPixel;
  Double_t seedSN;
  //Double_t seedN;

  Int_t nPlanes = tTracker->GetPlanesN();
  if( nPlanes>6 ) {
    pad->Divide( (Int_t)ceil(nPlanes/4.), (nPlanes>4)?4:nPlanes);  // LC 4 lines instead of 2.
  }
  else {
    pad->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);
  }

  // Get the first DUT among the planes and remember it,
  //  it considered as the one providing the coordinate frame for the geomatrix
  // If no DUT (status=3), set to first plane
  // JB 2012/0820
  DPlane* DUTplane = tTracker->GetPlane(1);
  for( Int_t iPlane=2; iPlane<=nPlanes; iPlane++) { // loop on planes
    tPlane = tTracker->GetPlane(iPlane);
    if( tPlane->GetStatus()==3 ) {
      DUTplane = tPlane;
      break;
    }
  }
  // geomatrix coordinates and box
  Int_t geomatrix=3;
  DR3 gUVWmin( fSession->GetSetup()->GetAnalysisPar().Umin[0][geomatrix], fSession->GetSetup()->GetAnalysisPar().Vmin[0][geomatrix], DUTplane->GetPosition()(2));
  DR3 gUVWmax( fSession->GetSetup()->GetAnalysisPar().Umax[0][geomatrix], fSession->GetSetup()->GetAnalysisPar().Vmax[0][geomatrix], DUTplane->GetPosition()(2));
  // Transform those DUTplane frame coordinates into the telescope frame
  DR3 gXYZmin = DUTplane->PlaneToTracker(gUVWmin);
  DR3 gXYZmax = DUTplane->PlaneToTracker(gUVWmax);
  printf("Geomatrix: U-range %.0f, %.0f, V-range %.0f, %.0f\n", gUVWmin(0), gUVWmax(0), gUVWmin(1), gUVWmax(1));
  printf("Geomatrix: X-range %.0f, %.0f, Y-range %.0f, %.0f\n", gXYZmin(0), gXYZmax(0), gXYZmin(1), gXYZmax(1));
  cout << " DUT plane is got as plane " << DUTplane->GetPlaneNumber() << endl;

  TBox **geomBox = new TBox*[nPlanes];
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    geomBox[iPlane-1] = new TBox( gXYZmin(0), gXYZmin(1), gXYZmax(0), gXYZmax(1) );
    geomBox[iPlane-1]->SetFillStyle(0);
    geomBox[iPlane-1]->SetLineStyle(1);
    geomBox[iPlane-1]->SetLineWidth(2);
    geomBox[iPlane-1]->SetLineColor(1);
  } // end loop on planes

  // Determine extrema of planes position in telescope frame
  Double_t xmin=1e6, xmax=-1e6;
  Double_t ymin=1e6, ymax=-1e6;
  Double_t planeBox[2][2] = {{0,0},{0,0}};
  TBox **geomPlaneBox = new TBox*[nPlanes];
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    tPlane = tTracker->GetPlane(iPlane);
    // bottom left corner
    DR3 posInPlane, posBLInTracker, posURInTracker;
    posInPlane.SetValue( -tPlane->GetStripsNu() * tPlane->GetStripPitch()(0) / 2.
                        ,-tPlane->GetStripsNv() * tPlane->GetStripPitch()(1) / 2.
                        ,0.);
    posBLInTracker = tPlane->PlaneToTracker( posInPlane);
    if( posBLInTracker(0)<xmin ) xmin = posBLInTracker(0);
    if( posBLInTracker(1)<ymin ) ymin = posBLInTracker(1);
    if( posBLInTracker(0)>xmax ) xmax = posBLInTracker(0);
    if( posBLInTracker(1)>ymax ) ymax = posBLInTracker(1);
    // upper right corner
    posInPlane.SetValue( +tPlane->GetStripsNu() * tPlane->GetStripPitch()(0) / 2.
                        ,+tPlane->GetStripsNv() * tPlane->GetStripPitch()(1) / 2.
                        ,0.);
    posURInTracker = tPlane->PlaneToTracker( posInPlane);
    if( posURInTracker(0)<xmin ) xmin = posURInTracker(0);
    if( posURInTracker(1)<ymin ) ymin = posURInTracker(1);
    if( posURInTracker(0)>xmax ) xmax = posURInTracker(0);
    if( posURInTracker(1)>ymax ) ymax = posURInTracker(1);
    planeBox[0][0] = TMath::Min( posBLInTracker(0), posURInTracker(0));
    planeBox[0][1] = TMath::Max( posBLInTracker(0), posURInTracker(0));
    planeBox[1][0] = TMath::Min( posBLInTracker(1), posURInTracker(1));
    planeBox[1][1] = TMath::Max( posBLInTracker(1), posURInTracker(1));
    geomPlaneBox[iPlane-1] = new TBox( planeBox[0][0], planeBox[1][0], planeBox[0][1], planeBox[1][1]);
    geomPlaneBox[iPlane-1]->SetFillStyle(0);
  } // end loop on planes

  int NbinsX = tPlane->GetStripsNu();
  int NbinsY = tPlane->GetStripsNv();
  if(Define_Range) {
    xmin  = Xmin;
    xmax  = Xmax;
    ymin  = Ymin;
    ymax  = Ymax;
    NbinsX = bins;
    NbinsY = bins;
  }

  int Qbins = 200;
  double Qmax = 0.;
  if(Define_QRange) {
    Qmax  = qmax;
    Qbins = qbins;
  }

  TH2F *hHitMapDUT;
  TH2F **hHitMap = new TH2F*[nPlanes];
  TH2F **hTrackMap = new TH2F*[nPlanes]; // JB 2011/11/02
  TH1F **hHitPixMult = new TH1F*[nPlanes];
  TH1F **hNHitsPerEvent = new TH1F*[nPlanes];
  //TH2F **hNHitsPerEventCorr = new TH2F*[nPlanes*(nPlanes - 1)/2];
  TH1F **hHitSeedSN = new TH1F*[nPlanes]; // JB 2012/08/17
  TH1F **hHitNeighbourSN = new TH1F*[nPlanes];
  TH2F **hHitSeedVsNeighbourSN = new TH2F*[nPlanes];
  TH1F **hHitCharge = new TH1F*[nPlanes];
  TH1F **hHitSeedCharge = new TH1F*[nPlanes]; // JB 2013/10/08
  TH1F **hHitNeighbourCharge = new TH1F*[nPlanes];
  TH2F **hHitSeedVsNeighbourCharge = new TH2F*[nPlanes];
  TH2F **hHitSeedVsHitCharge = new TH2F*[nPlanes];
  TH2F **hHitSeedSNVsSeedCharge = new TH2F*[nPlanes];
  TH1F **hHitTimeStamp = new TH1F*[nPlanes]; // JB 2015/05/25
  TH1F **hNHitsVSEventN = new TH1F*[nPlanes]; // QL 2015/10/23
  TH1F **hHitStoNover2 = new TH1F*[nPlanes]; // JB 2017/11/26
  TH1F **hHitSeedNoise = new TH1F*[nPlanes]; // JB 2017/11/26
  TCanvas **cHitProperties = new TCanvas*[nPlanes]; // JB 2013/10/30
  Char_t name[50], title[100];
  int counter_planes_pairs = 0;
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    tPlane = tTracker->GetPlane(iPlane);

    // -- Histo with microns
    sprintf( name, "hhitmappl%d", iPlane);
    sprintf( title, "Hit map of plane %d - %s;X (#mum);Y (#mum)", iPlane, tPlane->GetPlanePurpose());
    // check if the histo is existing or not
    hHitMap[iPlane-1] = new TH2F(name, title, NbinsX, xmin, xmax, NbinsY, ymin, ymax);
    hHitMap[iPlane-1]->SetMarkerStyle(20);
    hHitMap[iPlane-1]->SetMarkerSize(.1);
    hHitMap[iPlane-1]->SetMarkerColor(1);
    hHitMap[iPlane-1]->SetStats(kFALSE);
    //printf( "MRaw::DisplayRawData created %s histo with %dx%d pixels\n", name, tPlane->GetStripsNu(), tPlane->GetStripsNv());

    sprintf( name, "hhitmapdut");
    sprintf( title, "Hit map for DUTs;X (#mum);Y (#mum)");
    // check if the histo is existing or not
    hHitMapDUT = new TH2F(name, title, NbinsX, xmin, xmax, NbinsY, ymin, ymax);
    hHitMapDUT->SetMarkerStyle(20);
    hHitMapDUT->SetMarkerSize(.2);
    hHitMapDUT->SetMarkerColor(1);
    hHitMapDUT->SetStats(kFALSE);

    // -- Histo for tracks with microns, JB 2011/11/02
    sprintf( name, "htrackmappl%d", iPlane);
    sprintf( title, "Track map of plane (%d) %s;X (#mum);Y (#mum)", iPlane, tPlane->GetPlanePurpose());
    hTrackMap[iPlane-1] = new TH2F( *(hHitMap[iPlane-1])); // JB 2009/09/09
    //name, title, hHitMap[iPlane-1]->GetNbinsX(), hHitMap[iPlane-1]->GetNbinsX(), tPlane->GetStripPitch()(0)*tPlane->GetStripsNu()/2., 100, -tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2., tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2.);
    hTrackMap[iPlane-1]->SetName( name);
    hTrackMap[iPlane-1]->SetTitle( title);
    hTrackMap[iPlane-1]->SetMarkerStyle(24);
    hTrackMap[iPlane-1]->SetMarkerSize(.2);
    hTrackMap[iPlane-1]->SetMarkerColor(2);
    hTrackMap[iPlane-1]->SetStats(kFALSE);

    // -- Pixel multiplicity in hits
    Int_t nPixMax = fSession->GetSetup()->GetPlanePar(iPlane).MaxNStrips;
    sprintf( name, "hhitpixpl%d", iPlane);
    sprintf( title, "Pixel multiplicity in hits of plane %d - %s - %s", iPlane, tPlane->GetPlaneName(), tPlane->GetPlanePurpose());
    if( nPixMax==0 ) {
      hHitPixMult[iPlane-1] = new TH1F(name, title, 25, 0, 0);
    }
    else {
      hHitPixMult[iPlane-1] = new TH1F(name, title, nPixMax+1, -.5, nPixMax+0.5);
    }
    hHitPixMult[iPlane-1]->SetXTitle("# pixels in hit");

    // -- Pixel time stamp
    // JB 2015/05/25
    sprintf( name, "hhittimepl%d", iPlane);
    sprintf( title, "Hit time stamp of plane %d - %s - %s; Time stamp", iPlane, tPlane->GetPlaneName(), tPlane->GetPlanePurpose());
    hHitTimeStamp[iPlane-1] = new TH1F(name, title, 100, 0, 0);

    // -- #Hits VS #Event
    // QL 2015/10/23
    sprintf( name, "hNHitsVSEventNPl%d", iPlane);
    sprintf( title, "Number of Clusters VS Event Number for plane %d - %s - %s", iPlane, tPlane->GetPlaneName(), tPlane->GetPlanePurpose());
    hNHitsVSEventN[iPlane-1] = new TH1F(name, title, nEvents, 0, nEvents );
    hNHitsVSEventN[iPlane-1]->SetXTitle("Event Number");

    // -- Number of hits per event
    sprintf( name, "hnhitspereventpl%d", iPlane);
    sprintf( title, "Number of hits per event of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    //hNHitsPerEvent[iPlane-1] = new TH1F(name, title, 100, 0, 0);
    hNHitsPerEvent[iPlane-1] = new TH1F(name, title, 751, -0.5, 750.5); // QL 2015/10/23, High trigger rate
    hNHitsPerEvent[iPlane-1]->SetXTitle("# hits per event");

    // -- Analog hit properties
    sprintf( name, "hhitseedsn%d", iPlane);
    sprintf( title, "Hit S/N for seed pixel of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitSeedSN[iPlane-1] = new TH1F(name, title, 200, 0, 0);
    hHitSeedSN[iPlane-1]->SetXTitle("S/N");
    sprintf( name, "hhitneighboursn%d", iPlane);
    sprintf( title, "Hit S/N for neighbour pixels of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitNeighbourSN[iPlane-1] = new TH1F(name, title, 100, 0, 0);
    hHitNeighbourSN[iPlane-1]->SetXTitle("S/N");
    sprintf( name, "hhitseedneighbourgsn%d", iPlane);
    sprintf( title, "Hit S/N for neighbour pixels VS seed of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitSeedVsNeighbourSN[iPlane-1] = new TH2F(name, title, 200, 0, 0, 50, 0, 0);
    hHitSeedVsNeighbourSN[iPlane-1]->SetXTitle("seed S/N");
    hHitSeedVsNeighbourSN[iPlane-1]->SetYTitle("neighbourgs S/N");
    sprintf( name, "hhitcharge%d", iPlane);
    sprintf( title, "Hit charge of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitCharge[iPlane-1] = new TH1F(name, title, Qbins, 0, Qmax);
    hHitCharge[iPlane-1]->SetXTitle("ADC unit");
     // JB 2013/10/08
    sprintf( name, "hhitseedq%d", iPlane);
    sprintf( title, "Seed pixel charge of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitSeedCharge[iPlane-1] = new TH1F(name, title, Qbins, 0, Qmax);
    hHitSeedCharge[iPlane-1]->SetXTitle("charge (ADCu)");
    sprintf( name, "hhitneighbourq%d", iPlane);
    sprintf( title, "Neighbour pixels charge of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitNeighbourCharge[iPlane-1] = new TH1F(name, title, Qbins/2, 0, Qmax/2);
    hHitNeighbourCharge[iPlane-1]->SetXTitle("charge (ADCu)");
    sprintf( name, "hhitseedneighbourgq%d", iPlane);
    sprintf( title, "Neighbour pixels VS seed charge of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitSeedVsNeighbourCharge[iPlane-1] = new TH2F(name, title, Qbins/2, 0, Qmax, Qbins/4, 0, Qmax/2);
    hHitSeedVsNeighbourCharge[iPlane-1]->SetXTitle("seed charge (ADCu)");
    hHitSeedVsNeighbourCharge[iPlane-1]->SetYTitle("neighbourgs charge (ADCu)");
    sprintf( name, "hhitseedhitq%d", iPlane);
    sprintf( title, "Hit pixels VS seed charge of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitSeedVsHitCharge[iPlane-1] = new TH2F(name, title, Qbins/2, 0, Qmax, Qbins/2, 0, Qmax);
    hHitSeedVsHitCharge[iPlane-1]->SetXTitle("seed charge (ADCu)");
    hHitSeedVsHitCharge[iPlane-1]->SetYTitle("hit charge (ADCu)");
    sprintf( name, "hhitseedsnseedq%d", iPlane);
    sprintf( title, "Seed pixel S/N VS seed charge of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitSeedSNVsSeedCharge[iPlane-1] = new TH2F(name, title, Qbins, 0, Qmax, 100, 0, 0);
    hHitSeedSNVsSeedCharge[iPlane-1]->SetXTitle("seed charge (ADCu)");
    hHitSeedSNVsSeedCharge[iPlane-1]->SetYTitle("seed S/N");
    sprintf( name, "hhitstonover2%d", iPlane);
    sprintf( title, "# neighbour pixel with SNR>2 of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitStoNover2[iPlane-1] = new TH1F(name, title, 100, 0, 0);
    hHitStoNover2[iPlane-1]->SetXTitle("# pixels");
    sprintf( name, "hhitseednoise%d", iPlane);
    sprintf( title, "Seed pixel noise of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitSeedNoise[iPlane-1] = new TH1F(name, title, 100, 0, 0);
    hHitSeedNoise[iPlane-1]->SetXTitle("seed noise (ADCu)");

#if 0
    for( Int_t iPlane2=1; iPlane2<=nPlanes; iPlane2++) {
      if(iPlane >= iPlane2) continue;
      DPlane* tPlane2 = tTracker->GetPlane(iPlane2);

      // -- Correlations among the planes of number of hits per event
      sprintf( name, "hnhitspereventForpl%d_vs_pl%d", iPlane, iPlane2);
      sprintf( title, "Number of hits per event of plane %d (%s) vs %d (%s)", iPlane, tPlane->GetPlanePurpose(),iPlane2, tPlane2->GetPlanePurpose());
      hNHitsPerEventCorr[counter_planes_pairs] = new TH2F(name, title,
							  hNHitsPerEvent[iPlane-1]->GetXaxis()->GetNbins(),
							  hNHitsPerEvent[iPlane-1]->GetXaxis()->GetXmin(),
							  hNHitsPerEvent[iPlane-1]->GetXaxis()->GetXmax(),
							  hNHitsPerEvent[iPlane-1]->GetXaxis()->GetNbins(),
							  hNHitsPerEvent[iPlane-1]->GetXaxis()->GetXmin(),
							  hNHitsPerEvent[iPlane-1]->GetXaxis()->GetXmax());
      sprintf( name, "# hits per event pl %d", iPlane);
      hNHitsPerEventCorr[counter_planes_pairs]->SetXTitle(name);
      sprintf( name, "# hits per event pl %d", iPlane2);
      hNHitsPerEventCorr[counter_planes_pairs]->SetYTitle(name);
      counter_planes_pairs++;
    }
#endif
  }

  //Loop over the requested number of events
  for( Int_t iEvt=0; iEvt < nEvents; iEvt++) {
    if( !(fSession->NextRawEvent()) ) break; // JB 2009/06/26
    tTracker->Update();

    counter_planes_pairs = 0;
    for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
      tPlane = tTracker->GetPlane(iPlane);

      //cout << "CumulateHits2D:: plane " << tPlane->GetPlaneNumber() << " has " << tPlane->GetHitsN() << endl;
      hNHitsPerEvent[iPlane-1]->Fill( tPlane->GetHitsN() );

#if 0
      for( Int_t iPlane2=1; iPlane2<=nPlanes; iPlane2++) {
        if(iPlane >= iPlane2) continue;
        DPlane* tPlane2 = tTracker->GetPlane(iPlane2);
        hNHitsPerEventCorr[counter_planes_pairs]->Fill(tPlane->GetHitsN(),tPlane2->GetHitsN());
        counter_planes_pairs++;
      }
#endif


      if( tPlane->GetHitsN()>0 ) {
        nHitsReconstructed += tPlane->GetHitsN();
        //hNHitsPerEvent[iPlane-1]->Fill( tPlane->GetHitsN() );
        for( Int_t iHit=1; iHit<=tPlane->GetHitsN(); iHit++) { //loop on hits (starts at 1 !!)
          aHit = (DHit*)tPlane->GetHit( iHit);
          //printf("Getting seed index for hit %d (address %x) at plane %d\n", iHit, aHit, iPlane);
          hHitMap[iPlane-1]->Fill( tPlane->PlaneToTracker(*(aHit->GetPosition()))(0), tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), 1); // weight could be aHit->GetClusterPulseSum()
          if( tPlane->GetStatus()==3 ) hHitMapDUT->Fill( tPlane->PlaneToTracker(*(aHit->GetPosition()))(0), tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), 1);

          // Filling hit properties depends on analysis mode
          // JB 2012/08/17
          //if( aHit->GetFound() )
          //printf("  hit %d at plane %d: #strips=%d, timestamp=%d\n", iHit, iPlane, aHit->GetStripsInCluster(), aHit->GetTimestamp());
          hHitPixMult[iPlane-1]->Fill( aHit->GetStripsInCluster()); // JB 2011/07/26
          hHitTimeStamp[iPlane-1]->Fill( aHit->GetTimestamp()); // JB 2015/05/25
          hNHitsVSEventN[iPlane-1]->Fill(iEvt);
          if( tPlane->GetAnalysisMode()<3 ) { // analog output
            //seedN = aHit->GetNoise(0);
            seedSN = aHit->GetSNseed(); // JB, 2013/11/08
            // computation below useless since new GetSNseed() method
//            if( seedN>1.e-5 ) {
//              seedSN = aHit->GetPulseHeight(0)/seedN;
//            }
//            else {
//              seedSN = 0.;
//            }
            hHitSeedSN[iPlane-1]->Fill( seedSN);
            hHitNeighbourSN[iPlane-1]->Fill( aHit->GetSNneighbour() );
            hHitSeedVsNeighbourSN[iPlane-1]->Fill( seedSN, aHit->GetSNneighbour() );
            hHitCharge[iPlane-1]->Fill( aHit->GetClusterPulseSum() );
            hHitSeedCharge[iPlane-1]->Fill( aHit->GetPulseHeight(0));
            hHitNeighbourCharge[iPlane-1]->Fill( aHit->GetClusterAreaPulseSum());
            hHitSeedVsNeighbourCharge[iPlane-1]->Fill( aHit->GetPulseHeight(0), aHit->GetClusterAreaPulseSum());
            hHitSeedVsHitCharge[iPlane-1]->Fill( aHit->GetPulseHeight(0), aHit->GetClusterPulseSum());
            hHitSeedSNVsSeedCharge[iPlane-1]->Fill( aHit->GetPulseHeight(0), seedSN);
	    hHitSeedVsHitCharge[iPlane-1]->Fill( aHit->GetPulseHeight(0), aHit->GetClusterPulseSum() );
            hHitStoNover2[iPlane-1]->Fill( aHit->GetStoNover2());
            hHitSeedNoise[iPlane-1]->Fill( aHit->GetSeedNoise());
          }
          if(fVerbose) printf("MRaw::DisplayCumulHits2D  pl %d, hit[%d=(%d,%d)=(%f,%f)]%f, mult=%d\n", iPlane, iHit, aHit->GetIndexSeed()%tPlane->GetStripsNu(), aHit->GetIndexSeed()/tPlane->GetStripsNu(), tPlane->PlaneToTracker(*(aHit->GetPosition()))(0), tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), aHit->GetClusterPulseSum(), aHit->GetStripsInCluster());
        } //end loop on hits
      }

      if( tTracker->GetTracksN()>0 ) {
        for( Int_t iTrack=1; iTrack<=tTracker->GetTracksN(); iTrack++ ) { // loop on tracks, JB 2011/11/02
          aTrack = tTracker->GetTrack(iTrack);
          DR3 posInPlane = aTrack->Intersection(tPlane);
          hTrackMap[iPlane-1]->Fill( tPlane->PlaneToTracker(posInPlane)(0), tPlane->PlaneToTracker(posInPlane)(1), 1);
          if(fVerbose) printf("MRaw::DisplayCumulHits2D  pl %d, track[%d=(%f,%f)\n", iPlane, iTrack, tPlane->PlaneToTracker(posInPlane)(0), tPlane->PlaneToTracker(posInPlane)(1));
        } // end loop on tracks
      }

    } //end loop on planes

  } // END LOOP ON EVENTS

  fSession->GetDataAcquisition()->PrintStatistics();
  tTracker->PrintStatistics();

  // Now display
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    pad->cd(iPlane);
    pad->cd(iPlane)->SetTickx(1);
    pad->cd(iPlane)->SetTicky(1);
    if( ifDrawTrack && tTracker->GetTracksMaximum() > 0) {
      hTrackMap[iPlane-1]->DrawCopy(); // JB 2011/11/02
      hHitMap[iPlane-1]->DrawCopy("same");
    }
    else {
      hHitMap[iPlane-1]->DrawCopy("colz");
    }
    geomPlaneBox[iPlane-1]->Draw("l");
    geomBox[iPlane-1]->Draw("l");
    cout << "Plane "<<iPlane<<" has seen "<<hHitMap[iPlane-1]->GetEntries()<<" hits."<< endl;
  }
  ccumulhit->Update();

  // this canvas is intended mainly for binary outputs
  TCanvas *ccumulhit2;
  g = gROOT->FindObject("ccumulhit2") ;
  if (g) {
    ccumulhit2 = (TCanvas*)g;
  }
  else {
    ccumulhit2 = new TCanvas("ccumulhit2", "Pixel multiplicitiy in hits", 5, 5,800,700);
  }
  ccumulhit2->Clear();
  ccumulhit2->UseCurrentStyle();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad2 = new TPad("pad2","",0.,0.,1.,0.965);
  pad2->Draw();
  pad2->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    if( 0 < tTracker->GetPlane(iPlane)->GetAnalysisMode()  ) {
      pad2->cd(iPlane);
      hHitPixMult[iPlane-1]->DrawCopy();
      //hHitPixMult[iPlane-1]->Delete();
    }
  }
  ccumulhit2->Update();

  // Time stamp of hits
  // JB 2015/05/25
  TCanvas *ccumulhit21;
  g = gROOT->FindObject("ccumulhit21") ;
  if (g) {
    ccumulhit21 = (TCanvas*)g;
  }
  else {
    ccumulhit21 = new TCanvas("ccumulhit21", "Hit time stamp", 5, 5,800,700);
  }
  ccumulhit21->Clear();
  ccumulhit21->UseCurrentStyle();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad21 = new TPad("pad21","",0.,0.,1.,0.965);
  pad21->Draw();
  pad21->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    if( 0 < tTracker->GetPlane(iPlane)->GetAnalysisMode()  ) {
      pad21->cd(iPlane);
      if( hHitTimeStamp[iPlane-1]->GetEntries()>0 ) hHitTimeStamp[iPlane-1]->DrawCopy();
    }
  }
  ccumulhit21->Update();

  // # Hits VS Event Number
  // QL 2015/10/23
  TCanvas *ccumulhit22;
  g = gROOT->FindObject("ccumulhit22") ;
  if (g) {
    ccumulhit22 = (TCanvas*)g;
  }
  else {
    ccumulhit22 = new TCanvas("ccumulhit22", "# hits VS event number", 5, 5,800,700);
  }
  ccumulhit22->Clear();
  ccumulhit22->UseCurrentStyle();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad22 = new TPad("pad22","",0.,0.,1.,0.965);
  pad22->Draw();
  pad22->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    if( 0 < tTracker->GetPlane(iPlane)->GetAnalysisMode()  ) {
      pad22->cd(iPlane);
      if( hNHitsVSEventN[iPlane-1]->GetEntries()>0 ) hNHitsVSEventN[iPlane-1]->DrawCopy();
    }
  }
  ccumulhit22->Update();

  // Number of hits per event
  double MaxAxisRanges = -1.0e+20;
  TCanvas *ccumulhit3;
  g = gROOT->FindObject("ccumulhit3") ;
  if (g) {
    ccumulhit3 = (TCanvas*)g;
  }
  else {
    ccumulhit3 = new TCanvas("ccumulhit3", "Nb of hits per event", 5, 5,800,700);
  }
  ccumulhit3->Clear();
  ccumulhit3->UseCurrentStyle();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad3 = new TPad("pad3","",0.,0.,1.,0.965);
  pad3->Draw();
  pad3->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    if( 0 < tTracker->GetPlane(iPlane)->GetAnalysisMode()  ) {
      pad3->cd(iPlane);

      double MyMaxRange = -999.0;
      for(int i=0;i<hNHitsPerEvent[iPlane-1]->GetXaxis()->GetNbins();i++) {
        int index = hNHitsPerEvent[iPlane-1]->GetXaxis()->GetNbins() - i;
        if(hNHitsPerEvent[iPlane-1]->GetBinContent(index) > 0.0) {
          MyMaxRange  = hNHitsPerEvent[iPlane-1]->GetBinCenter(index);
          MyMaxRange += 0.5*hNHitsPerEvent[iPlane-1]->GetBinWidth(index);
          break;
        }
      }
      MyMaxRange += 0.10*(MyMaxRange - hNHitsPerEvent[iPlane-1]->GetXaxis()->GetXmin());
      if(MaxAxisRanges < MyMaxRange) MaxAxisRanges = MyMaxRange;
      hNHitsPerEvent[iPlane-1]->SetAxisRange(hNHitsPerEvent[iPlane-1]->GetXaxis()->GetXmin(),
					     MyMaxRange,
					     "X");
      hNHitsPerEvent[iPlane-1]->DrawCopy();
    }
  }
  ccumulhit3->Update();

  TLatex* latex = new TLatex();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.06);
  latex->SetTextColor(kRed);
#if 0
  TLine* CorrLines[(nPlanes*(nPlanes - 1)/2)];
  int NcanvasX = 3;
  int NcanvasY = 2;
  int Ncanvas = (nPlanes*(nPlanes - 1)/2)/(NcanvasX*NcanvasY);
  int rest    = (nPlanes*(nPlanes - 1)/2) - (NcanvasX*NcanvasY)*Ncanvas;
  if(rest > 0) Ncanvas++;
  const int RealNcanvas(Ncanvas);
  TCanvas *ccumulhit3_2[RealNcanvas];
  int counter_plots = 0;
  for(int i=0;i<RealNcanvas;i++) {

    TString TheName = TString("ccumulhit3_2_") + long(i+1);

    g = gROOT->FindObject(TheName.Data()) ;
    if (g) {
      ccumulhit3_2[i] = (TCanvas*)g;
    }
    else {
      ccumulhit3_2[i] = new TCanvas(TheName.Data(), "Correlation among the planes of Nb of hits per event", 5, 5,800,700);
    }
    ccumulhit3_2[i]->Divide(NcanvasX,NcanvasY);
    for(int ipad=0;ipad<NcanvasX*NcanvasY;ipad++) {
      ccumulhit3_2[i]->cd(ipad+1);
      counter_plots++;
      if(counter_plots > (nPlanes*(nPlanes - 1)/2)) continue;

      ccumulhit3_2[i]->cd(ipad+1)->SetFillColor(10);
      ccumulhit3_2[i]->cd(ipad+1)->SetTickx(1);
      ccumulhit3_2[i]->cd(ipad+1)->SetTicky(1);
      ccumulhit3_2[i]->cd(ipad+1)->SetLeftMargin(0.20);
      ccumulhit3_2[i]->cd(ipad+1)->SetBottomMargin(0.20);
      ccumulhit3_2[i]->cd(ipad+1)->SetRightMargin(0.20);
      ccumulhit3_2[i]->cd(ipad+1)->SetLogz(1);
      hNHitsPerEventCorr[counter_plots-1]->SetAxisRange(hNHitsPerEventCorr[counter_plots-1]->GetXaxis()->GetXmin(),
							MaxAxisRanges,"X");
      hNHitsPerEventCorr[counter_plots-1]->SetAxisRange(hNHitsPerEventCorr[counter_plots-1]->GetYaxis()->GetXmin(),
							MaxAxisRanges,"Y");
      hNHitsPerEventCorr[counter_plots-1]->Draw("colz");

      CorrLines[counter_plots-1] = new TLine(hNHitsPerEventCorr[counter_plots-1]->GetXaxis()->GetXmin(),
					     hNHitsPerEventCorr[counter_plots-1]->GetYaxis()->GetXmin(),
					     MaxAxisRanges,
					     MaxAxisRanges);
      CorrLines[counter_plots-1]->SetLineColor(2);
      CorrLines[counter_plots-1]->SetLineWidth(3);
      CorrLines[counter_plots-1]->SetLineStyle(2);
      CorrLines[counter_plots-1]->Draw();
      //GGG

      double Xpos,Ypos;
      Xpos  = hNHitsPerEventCorr[counter_plots-1]->GetXaxis()->GetXmin();
      Xpos += 0.20*(MaxAxisRanges - hNHitsPerEventCorr[counter_plots-1]->GetXaxis()->GetXmin());
      Ypos  = MaxAxisRanges;
      Ypos -= 0.05*(MaxAxisRanges - hNHitsPerEventCorr[counter_plots-1]->GetYaxis()->GetXmin());

      char yyy[300];
      sprintf(yyy,"correlation = %.3f",hNHitsPerEventCorr[counter_plots-1]->GetCorrelationFactor());
      latex->DrawLatex(Xpos,Ypos,yyy);
    }
  }
#endif


  // For analog output sensors
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    tPlane = tTracker->GetPlane(iPlane);
    if( 0 < tPlane->GetAnalysisMode() && tPlane->GetAnalysisMode() <= 2 ) {

      sprintf( name, "chitpl%d", iPlane);
      sprintf( title, "Hit properties of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
      g = gROOT->FindObject(name) ;
      if (g) {
        cHitProperties[iPlane-1] = (TCanvas*)g;
      }
      else {
        cHitProperties[iPlane-1] = new TCanvas(name, title, 5, 5+5*(iPlane-1),900,700);
      }
      cHitProperties[iPlane-1]->Clear();
      cHitProperties[iPlane-1]->UseCurrentStyle();
      cHitProperties[iPlane-1]->Divide(4,3);

      cHitProperties[iPlane-1]->cd(1); //1-1
      if( hHitSeedSN[iPlane-1]->GetEntries()>0 ) hHitSeedSN[iPlane-1]->DrawCopy();
      cHitProperties[iPlane-1]->cd(2); //1-2
      if( hHitNeighbourSN[iPlane-1]->GetEntries()>0 ) hHitNeighbourSN[iPlane-1]->DrawCopy();
      cHitProperties[iPlane-1]->cd(3); //1-3
      if( hHitSeedVsNeighbourSN[iPlane-1]->GetEntries()>0 ) hHitSeedVsNeighbourSN[iPlane-1]->DrawCopy("colz");
      cHitProperties[iPlane-1]->cd(4); //1-4
      hNHitsPerEvent[iPlane-1]->DrawCopy();
      cHitProperties[iPlane-1]->cd(5); //2-1
      if( hHitSeedCharge[iPlane-1]->GetEntries()>0 )  hHitSeedCharge[iPlane-1]->DrawCopy();
      cHitProperties[iPlane-1]->cd(6); //2-2
      if( hHitNeighbourCharge[iPlane-1]->GetEntries()>0 )  hHitNeighbourCharge[iPlane-1]->DrawCopy();
      cHitProperties[iPlane-1]->cd(7); //2-3
      if( hHitSeedVsNeighbourCharge[iPlane-1]->GetEntries()>0 )  hHitSeedVsNeighbourCharge[iPlane-1]->DrawCopy("colz");
      cHitProperties[iPlane-1]->cd(8); //2-4
      if( hHitSeedSNVsSeedCharge[iPlane-1]->GetEntries()>0 ) hHitSeedSNVsSeedCharge[iPlane-1]->DrawCopy("colz");
      cHitProperties[iPlane-1]->cd(9); //3-1
      if( hHitSeedNoise[iPlane-1]->GetEntries()>0 )  hHitSeedNoise[iPlane-1]->DrawCopy();
      cHitProperties[iPlane-1]->cd(10); //3-2
      if( hHitCharge[iPlane-1]->GetEntries()>0 ) hHitCharge[iPlane-1]->DrawCopy();
      cHitProperties[iPlane-1]->cd(11); //3-3
      if( hHitSeedVsHitCharge[iPlane-1]->GetEntries()>0 ) hHitSeedVsHitCharge[iPlane-1]->DrawCopy("colz");
      cHitProperties[iPlane-1]->cd(12); //3-4
      if( hHitStoNover2[iPlane-1]->GetEntries()>0 )  hHitStoNover2[iPlane-1]->DrawCopy();
    }
  }

  // Save canvas and histos
  //cd to result dir
  // added JB 2011/03/14
  Char_t rootFile[300];
  sprintf(rootFile,"%sHitMap_run%d.root",fSession->GetResultDirName().Data(),fSession->GetRunNumber());
  sprintf(rootFile,"%s", fTool.LocalizeDirName( rootFile)); // JB 2011/07/07
  cout << "\n-- Saving histos and canvas into " << rootFile << endl;
  TFile fRoot(rootFile,"RECREATE");
  ccumulhit->Write();
  ccumulhit2->Write();
  ccumulhit22->Write(); // QL 2015/10/23
  ccumulhit3->Write();
  //for(int i=0;i<RealNcanvas;i++) ccumulhit3_2[i]->Write();

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    tPlane = tTracker->GetPlane(iPlane);
    hHitMapDUT->Write();
    if( 0 < tPlane->GetAnalysisMode()  ) {
      hHitMap[iPlane-1]->Write();
      hTrackMap[iPlane-1]->Write(); // JB 2011/11/02
      hHitPixMult[iPlane-1]->Write();
      hHitTimeStamp[iPlane-1]->Write();
      hNHitsVSEventN[iPlane-1]->Write(); // QL 2015/10/23
      hNHitsPerEvent[iPlane-1]->Write();
      if( tPlane->GetAnalysisMode() <= 2 ) {
        hHitSeedSN[iPlane-1]->Write();
        hHitNeighbourSN[iPlane-1]->Write();
        hHitSeedVsNeighbourSN[iPlane-1]->Write();
        hHitCharge[iPlane-1]->Write();
        hHitSeedCharge[iPlane-1]->Write();
        hHitNeighbourCharge[iPlane-1]->Write();
        hHitSeedVsNeighbourCharge[iPlane-1]->Write();
        hHitSeedVsHitCharge[iPlane-1]->Write();
        hHitSeedSNVsSeedCharge[iPlane-1]->Write();
        hHitStoNover2[iPlane-1]->Write();
        hHitSeedNoise[iPlane-1]->Write();
        cHitProperties[iPlane-1]->Write();
      }
    }
  }
  //for(int ipair = 0; ipair<(nPlanes*(nPlanes - 1)/2); ipair++) hNHitsPerEventCorr[ipair]->Write();

  fRoot.Close();

}


//______________________________________________________________________________
void MRaw::CumulateTxtFrames( Int_t nEvents, Int_t nCumulFrames)
{
  // Store nCumulFrames frames under Txt fromat
  //
  // JB, 2017/11/09

  fSession->SetEvents(nEvents);

  FILE *txtFile;
  txtFile = fopen("mimosa.txt","w");

  DTracker *tTracker  =  fSession->GetTracker();
  DPlane* tPlane = tPlane = tTracker->GetPlane(1);
  DHit *aHit = NULL;
  Int_t aIndex;
  //DPixel *aPixel;

  TH2F *hFrame = new TH2F("hframe", "frame", tPlane->GetStripsNu(), 0, tPlane->GetStripsNu(),
                                       tPlane->GetStripsNv(), 0, tPlane->GetStripsNv());

  Int_t frameCounter = 0;
  Int_t hitCounter = 0;

  //Loop over the requested number of events
  for( Int_t iEvt=0; iEvt < nEvents; iEvt++) {
    if( !(fSession->NextRawEvent()) ) break;
    frameCounter++;
    if( fDebugRaw) printf("   Reading frame %d (so far %d hits)\n", frameCounter, hitCounter);

    // If enough frames added in superFrame, generate a new superFrame
    if ( frameCounter==nCumulFrames ) {
      if( fDebugRaw) printf("SuperFrame completed with %d frame, including %d hits\n", frameCounter, hitCounter);
//      txtFile = fopen("mimosa.txt","a");
      for ( Int_t iBinX=1; iBinX<=hFrame->GetNbinsX(); iBinX++) {
        for ( Int_t iBinY=1; iBinY<=hFrame->GetNbinsY(); iBinY++) {
          fprintf( txtFile, "%d\t", (Int_t)hFrame->GetBinContent( iBinX, iBinY) );
        }
        fprintf( txtFile, "\n");
      }
//      fclose(txtFile);
      hFrame->Reset();
      frameCounter = 0;
      hitCounter = 0;
    }


    tTracker->Update();

      if( tPlane->GetHitsN()>0 ) {

        for( Int_t iHit=1; iHit<=tPlane->GetHitsN(); iHit++) { //loop on hits (starts at 1 !!)
          aHit = (DHit*)tPlane->GetHit( iHit);
          hitCounter += aHit->GetStripsInCluster();
          if( fDebugRaw) printf("      found hit %d with %d pixels\n", iHit, aHit->GetStripsInCluster());

          for( Int_t iStrip=0; iStrip<=aHit->GetStripsInCluster(); iStrip++ ) {
              aIndex = aHit->GetIndex( iStrip);
              hFrame->Fill( aIndex%tPlane->GetStripsNu(), aIndex/tPlane->GetStripsNu(), aHit->GetPulseHeight(iStrip));
              if( fDebugRaw>1) printf("        strip[%d,%d] = %d\n", aIndex%tPlane->GetStripsNu(), aIndex/tPlane->GetStripsNu(), (Int_t)aHit->GetPulseHeight(iStrip));
          }

        } //end loop on hits
      }

  } // END LOOP ON EVENTS

  fSession->GetDataAcquisition()->PrintStatistics();
  tTracker->PrintStatistics();

  fclose(txtFile);

}


//______________________________________________________________________________
//
void MRaw::DisplayLadderCumulatedHits2D( Int_t nEvents, Bool_t ifDrawTrack, Bool_t Define_Range, Int_t bins, Double_t Xmin, Double_t Xmax, Double_t Ymin, Double_t Ymax)
{
  fSession->SetEvents(nEvents);

  //Int_t nHitsReconstructed = 0;

  TCanvas *cumulHitsInLadder;
  TObject* g = gROOT->FindObject("cumulHitsInLadder") ;
  if (g) {
    cumulHitsInLadder = (TCanvas*)g;
  }
  else {
    cumulHitsInLadder = new TCanvas("cumulHitsInLadder", "Cumulate Hits In Ladders", 5, 5,800,700);
  }

  cumulHitsInLadder->Clear();
  cumulHitsInLadder->UseCurrentStyle();


  TPaveLabel* label = new TPaveLabel();
  Char_t canvasTitle[200];
  sprintf(canvasTitle, "Run %d, cumul over %d events", fSession->GetRunNumber(), nEvents);
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad = new TPad("pad","",0.,0.,1.,0.965);
  pad->Draw();

  DTracker *tTracker = fSession->GetTracker();
  DLadder* aLadder;
  DPlane*  aPlane;
  //DTrack*  aTrack;
  DHit*    aHit;

  Int_t nLadders = tTracker->GetNumberOfLadders();

  if(nLadders<1) {
     std::cout<<"No ladder found."<<std::endl;
     return;
  }

  pad->Divide( 1 , 2*nLadders);  // One ladder per line.

  aLadder = tTracker->GetLadder(1);
  Int_t planeIndexInLadder = aLadder->GetFirstPlane();
  // Determine extrema of planes position in telescope frame
  Double_t xmin=aLadder->GetSeparatorTotalLentgh()/2+(aLadder->GetNumberOfPlanes()*aLadder->GetPlane(planeIndexInLadder)->GetStripsNv()*aLadder->GetPlane(planeIndexInLadder)->GetStripPitch()(0))/2.;
  Double_t xmax=-xmin;
  Double_t ymin= -aLadder->GetPlane(planeIndexInLadder)->GetStripsNv()/2*aLadder->GetPlane(planeIndexInLadder)->GetStripPitch()(1);
  Double_t ymax=-ymin;

/*
  for( Int_t iLadder=1; iLadder<=nLadders; iLadder++) { // loop on planes

    aLadder = tTracker->GetLadder(iLadder);

    Int_t nPlanesInLadder = aLadder->GetNumberOfPlanes();
    Int_t firstPlaneInLadder = aLadder->GetFirstPlane();

    DPlane* aPlaneLeft = aLadder->GetPlane(firstPlaneInLadder);
    DPlane* aPlaneRigth = aLadder->GetPlane(firstPlaneInLadder+(nPlanesInLadder/2)-1);
    DPlane* aPlaneLeft1 = aLadder->GetPlane(firstPlaneInLadder+(nPlanesInLadder/2));
    DPlane* aPlaneRight1 = aLadder->GetPlane(firstPlaneInLadder+nPlanesInLadder-1)

    DR3 posInPlane, posInTracker;

    posInTracker.SetValue( -aPlaneLeft->GetStripsNu() * tPlane->GetStripPitch()(0) / 2.
                        ,-aPlaneLeft->GetStripsNv() * tPlane->GetStripPitch()(1) / 2.
                        ,0.);

    posInTracker = aPlaneLeft->PlaneToTracker( posInPlane);
    if( posBLInTracker(0)<xmin ) xmin = posBLInTracker(0);
    if( posBLInTracker(1)<ymin ) ymin = posBLInTracker(1);
    if( posBLInTracker(0)>xmax ) xmax = posBLInTracker(0);
    if( posBLInTracker(1)>ymax ) ymax = posBLInTracker(1);

    // upper right corner
    posInPlane.SetValue( +tPlane->GetStripsNu() * tPlane->GetStripPitch()(0) / 2.
                        ,+tPlane->GetStripsNv() * tPlane->GetStripPitch()(1) / 2.
                        ,0.);
    posURInTracker = tPlane->PlaneToTracker( posInPlane);
    if( posURInTracker(0)<xmin ) xmin = posURInTracker(0);
    if( posURInTracker(1)<ymin ) ymin = posURInTracker(1);
    if( posURInTracker(0)>xmax ) xmax = posURInTracker(0);
    if( posURInTracker(1)>ymax ) ymax = posURInTracker(1);

  } // end loop on ladders
*/
  //aLadder = tTracker->GetLadder(1);
  //Int_t planeIndexInLadder = aLadder->GetFirstPlane();
  Int_t NbinsX = aLadder->GetNumberOfPlanes()*aLadder->GetPlane(planeIndexInLadder)->GetStripsNu()+aLadder->GetSeparatorTotalLentgh()/aLadder->GetPlane(planeIndexInLadder)->GetStripPitch()(0);
  Int_t NbinsY = aLadder->GetPlane(planeIndexInLadder)->GetStripsNv();

  std::cout<<"rowNumber = "<<NbinsY<<" columnNumber = "<<NbinsX<<std::endl;

  //if(Define_Range) {
    xmin  = Xmin;
    xmax  = Xmax;
    ymin  = Ymin;
    ymax  = Ymax;
    NbinsX = bins;
    NbinsY = bins;
  //}

  TH2F **hHitMap = new TH2F*[nLadders*2];

  Char_t name[50], title[100];
  //int counter_planes_pairs = 0;

  for( Int_t iLadder=1; iLadder<=nLadders; iLadder++) {

    aLadder = tTracker->GetLadder(iLadder);

    Int_t face=1;
    // -- Histo for hits position
    sprintf( name, "hhitmapladder%d", iLadder);
    sprintf( title, "Hit map of ladder %d , face %d;X (#mum);Y (#mum)", iLadder, face);
    // check if the histo is existing or not
    hHitMap[iLadder*2-1] = new TH2F(name, title, NbinsX, xmin, xmax, NbinsY, ymin, ymax);
    hHitMap[iLadder*2-1]->SetMarkerStyle(20);
    hHitMap[iLadder*2-1]->SetMarkerSize(.1);
    hHitMap[iLadder*2-1]->SetMarkerColor(1);
    hHitMap[iLadder*2-1]->SetStats(kFALSE);

    face=2;
    sprintf( title, "Hit map of ladder %d , face %d;X (#mum);Y (#mum)", iLadder, face);

    hHitMap[iLadder*2] = new TH2F(name, title, NbinsX, xmin, xmax, NbinsY, ymin, ymax);
    hHitMap[iLadder*2]->SetMarkerStyle(20);
    hHitMap[iLadder*2]->SetMarkerSize(.1);
    hHitMap[iLadder*2]->SetMarkerColor(1);
    hHitMap[iLadder*2]->SetStats(kFALSE);

/*
    // -- Histo for track position
    hTrackMap[iLadder-1] = new TH2F( *(hHitMap[iLadder-1])); // JB 2009/09/09
    hTrackMap[iLadder-1]->SetName( name);
    hTrackMap[iLadder-1]->SetTitle( title);
    hTrackMap[iLadder-1]->SetMarkerStyle(24);
    hTrackMap[iLadder-1]->SetMarkerSize(.2);
    hTrackMap[iLadder-1]->SetMarkerColor(2);
    hTrackMap[iLadder-1]->SetStats(kFALSE);
*/

  }

  //Loop over the requested number of events
  for( Int_t iEvt=0; iEvt < nEvents; iEvt++) {

    if( !(fSession->NextRawEvent()) ) break;

    tTracker->Update();

    for( Int_t iLadder=1; iLadder<=nLadders; iLadder++) { // loop on planes

      aLadder = tTracker->GetLadder(iLadder);

      Int_t nPlanesInLadder = aLadder->GetNumberOfPlanes();
      Int_t firstPlaneInLadder = aLadder->GetFirstPlane();

      for(Int_t iPlane=firstPlaneInLadder ; iPlane<firstPlaneInLadder+nPlanesInLadder ; ++iPlane) {

        aPlane=aLadder->GetPlane(iPlane);

        Int_t faceNumber=0;
        if( iPlane<(aLadder->GetFirstPlane()+aLadder->GetNumberOfPlanes()/2) ) faceNumber=1;
        else faceNumber=0;

        if( aPlane->GetHitsN()>0 ) {

          for( Int_t iHit=1; iHit<=aPlane->GetHitsN(); iHit++) { //loop on hits (starts at 1 !!)

            aHit = (DHit*)aPlane->GetHit( iHit);
            //printf("Getting seed index for hit %d (address %x) at plane %d\n", iHit, aHit, iPlane);
            DR3 hitPosition =  DR3( (*aHit->GetPosition())(0), (*aHit->GetPosition())(1), 0.);
            std::cout<<"ladderNumber = "<<iLadder<<" planeNumber = "<< iPlane <<std::endl;
            hitPosition.Print();
            DR3 hitPositionInLadder = aLadder->PlaneToLadder( hitPosition, iPlane-firstPlaneInLadder+1);
            hHitMap[iLadder*2-faceNumber]->Fill( hitPositionInLadder(0), hitPositionInLadder(1), 1 );
          } //end loop on hits
        } // end if
      }  // end loopon planes

/*    NEEDS an intersection method in DLadder :)
      if( tTracker->GetTracksN()>0 ) {

        for( Int_t iTrack=1; iTrack<=tTracker->GetTracksN(); iTrack++ ) { // loop on tracks, JB 2011/11/02

          aTrack = tTracker->GetTrack(iTrack);
          DR3 posInPlane = aTrack->Intersection(tPlane);
          hTrackMap[iPlane-1]->Fill( tPlane->PlaneToTracker(posInPlane)(0), tPlane->PlaneToTracker(posInPlane)(1), 1);
          if(fVerbose) printf("MRaw::DisplayCumulHits2D  pl %d, track[%d=(%f,%f)\n", iPlane, iTrack, tPlane->PlaneToTracker(posInPlane)(0), tPlane->PlaneToTracker(posInPlane)(1));
        } // end loop on tracks
      } // end if
*/
    } //end loop on ladders

  } // END LOOP ON EVENTS

  // Now display
  for( Int_t iLadder=1; iLadder<=nLadders; iLadder++) {

    pad->cd(iLadder*2-1);
    pad->cd(iLadder*2-1)->SetTickx(1);
    pad->cd(iLadder*2-1)->SetTicky(1);

    hHitMap[iLadder*2-1]->DrawCopy("colz");

    cout << "Ladder "<<iLadder<<" face 1 has seen "<<hHitMap[iLadder*2-1]->GetEntries()<<" hits."<< endl;

    pad->cd(iLadder*2);
    pad->cd(iLadder*2)->SetTickx(1);
    pad->cd(iLadder*2)->SetTicky(1);

    hHitMap[iLadder*2]->DrawCopy("colz");

    cout << "Ladder "<<iLadder<<" face 2 has seen "<<hHitMap[iLadder*2]->GetEntries()<<" hits."<< endl;
  }


  cumulHitsInLadder->Update();

}

//______________________________________________________________________________
//
void MRaw::DisplayCumulatedRawData2D(Int_t nEvents,
             TString str_EvtParsingMode,     // FIXME Antonin Maire, 7 oct 2015
				     Float_t minSN,
				     Float_t occurence_min,
				     Float_t occurence_max,
				     Int_t nOccurences,
				     Float_t minOccurence,
				     Bool_t storeOccurence,
				     double Colmin, double Colmax,
				     double Linmin, double Linmax)
{
  // Display two sets of plots fr each plane in the tracker:
  //  - the raw data cumulated over the requested number of events,
  //  - the distribution of the pixel occupancy.
  //
  // Additional information for "hot" pixels can be also retrieve see below.
  //
  // Parameters:
  //  - str_EvtParsingMode = continuous(default) / jumpy (read events 5 by 5)
  //  - minSN = indicates the lowest value of the pixel S/N to display the pixel,
  //  - nOccurences, occurence_min, occurence_max =
  //                      nb of bins, min, max for the occupancy plot.
  //
  // Hot or noisy pixels study:
  //  - set storeOccurence=true  to get the outputs,
  //  - Output 1 = a text file is created with the position
  //  (column and row) of all pixels above the specified "minOccurence*nEvents",
  //  - Output 2 = a root file with cumulated raw data normalized by nEvents.
  //
  // The Colmin,Colmax,Linmin,Linmax parameters:
  // If the values of the parameters Colmin,Colmax,Linmin,Linmax are specified
  // look only inside the region defined by them. Posibility of doing fake-rate analysis
  // inside the region
  //
  // RDM280509
  // Last Modified JB 2009/08/31 binning of histos and bin index in filling
  // Last Modified JB 2011/03/14 use threshold for pixel S/N and occurence
  // Last Modified JB 2011/07/08 condition to stop event loop
  // Last Modified VR 2012/04/02 complete noisy pixel studies based on firing occurence
  // Last Modified JB 2012/05/10 lowest minOccurence allowed set to 0.005
  // Last Modified JB 2013/05/26 test readout not 0
  // Last Modified JB 2014/03/11 store histos for HotPixelMap in MCommands
  // Last Modified VR 2014/07/10 log scale for pixel occurence
  // Last Modified AP 2014/10/22 Added limits on column/line for the occupancy analysis

  fSession->SetEvents(nEvents);

  DTracker *tTracker  =  fSession->GetTracker();
  Int_t nPlanes = tTracker->GetPlanesN();
  DPlane* tPlane;

  // Canvas for Raw Data map
  TCanvas *ccumulrd;
  TObject* g = gROOT->FindObject("ccumulrd") ;
  if (g) {
    ccumulrd = (TCanvas*)g;
  }
  else {
    ccumulrd = new TCanvas("ccumulrd", "Cumulate RawData", 5, 5,800,700);
  }
  ccumulrd->Clear();
  ccumulrd->UseCurrentStyle();
  TPaveLabel* label = new TPaveLabel();
  Char_t canvasTitle[200];
  sprintf(canvasTitle, "Run %d, cumul over %d events", fSession->GetRunNumber(), nEvents);
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad = new TPad("pad","",0.,0.,1.,0.965);
  pad->Draw();
  pad->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);

  // Canvas for noisy pixel study
  TCanvas *cnoisypixels;
  g = gROOT->FindObject("cnoisypixels") ;
  if (g) cnoisypixels= (TCanvas*)g;
  else cnoisypixels = new TCanvas("cnoisypixels", "Noisy pixels Study", 5, 5,800,700);
  cnoisypixels->Clear();
  cnoisypixels->UseCurrentStyle();
  sprintf(canvasTitle, "Run %d, cumul over %d events", fSession->GetRunNumber(), nEvents);
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad2 = new TPad("pad2","",0.,0.,1.,0.965);
  pad2->Draw();
  pad2->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);

  double R_RawData_U[2][nPlanes];
  int Nbins_RawData_U[nPlanes];
  double R_RawData_V[2][nPlanes];
  int Nbins_RawData_V[nPlanes];

  // Histograms
  TH2F **hRDMap = new TH2F*[nPlanes];
  TGraph **gnoisypixels = new TGraph*[nPlanes];
  Bool_t ifUseDaqIndex[nPlanes];
  TH1F **h1HotPixelList = new TH1F*[nPlanes];
  TH1F **h1FiredPixelsPerEvent = new TH1F*[nPlanes];
  Char_t name[50], title[100];
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    tPlane = tTracker->GetPlane(iPlane);
    ifUseDaqIndex[iPlane-1] = kFALSE;
    // -- Histo with pixel number
    sprintf( name, "hrdmappl%d", iPlane);
    sprintf( title, "Raw data map of plane %d - %s, S/N>%.1f", iPlane, tPlane->GetPlanePurpose(), minSN);
    // check if the histo is existing or not

    if( iPlane == 1 &&
       (Colmin >= 0 && Colmax <= tPlane->GetStripsNu()) &&
       (Linmin >= 0 && Linmax <= tPlane->GetStripsNv()) ) {
      R_RawData_U[0][iPlane-1] = int(Colmin) - 0.5;
      R_RawData_U[1][iPlane-1] = int(Colmax) + 0.5;
      Nbins_RawData_U[iPlane-1] = int(R_RawData_U[1] - R_RawData_U[0]);

      R_RawData_V[0][iPlane-1] = int(Linmin) - 0.5;
      R_RawData_V[1][iPlane-1] = int(Linmax) + 0.5;
      Nbins_RawData_V[iPlane-1] = int(R_RawData_V[1] - R_RawData_V[0]);
    }
    else {
      R_RawData_U[0][iPlane-1] = -0.5;
      R_RawData_U[1][iPlane-1] =  tPlane->GetStripsNu()-0.5;
      Nbins_RawData_U[iPlane-1] = tPlane->GetStripsNu();

      R_RawData_V[0][iPlane-1] = -0.5;
      R_RawData_V[1][iPlane-1] = tPlane->GetStripsNv()-0.5;
      Nbins_RawData_V[iPlane-1] = tPlane->GetStripsNv();
    }


    hRDMap[iPlane-1] = new TH2F(name, title,
				Nbins_RawData_U[iPlane-1],R_RawData_U[0][iPlane-1],R_RawData_U[1][iPlane-1],
				Nbins_RawData_V[iPlane-1],R_RawData_V[0][iPlane-1],R_RawData_V[1][iPlane-1]);
				//tPlane->GetStripsNu(),
				//-0.5,tPlane->GetStripsNu() - 0.5,
				//tPlane->GetStripsNv(),
				//-0.5,tPlane->GetStripsNv() - 0.5);
    hRDMap[iPlane-1]->SetMarkerStyle(20);
    hRDMap[iPlane-1]->SetMarkerSize(.1);
    hRDMap[iPlane-1]->SetMarkerColor(1);
    hRDMap[iPlane-1]->SetStats(kFALSE);

    sprintf( name, "hpixelspereventpl%d", iPlane);
    sprintf( title, "Nb of pixels per event of plane %d; # pixels; # events", iPlane, tPlane->GetPlanePurpose(), minSN);
    h1FiredPixelsPerEvent[iPlane-1] = new TH1F( name, title, 1000, 0, 10000);
  }

  int TheNEvts_counter = 0;
  //Loop over the requested number of events
  std::vector<DPixel*> *aList;
  for( Int_t iEvt=0; iEvt < nEvents; iEvt++) {

    // FIXME Antonin Maire, 7 oct 2015
    // Switch between 2 reading options : "continuous, evt after evt" vs "jumpy, every N evt"
    if( str_EvtParsingMode.EqualTo("continuous") ){
      if( !fSession->NextRawEvent() ) break; // Stop when no more events to read, JB 2011/07/08

    }// end continuous reading
    else if( str_EvtParsingMode.EqualTo("jumpy") ){
      ULong_t lRunNb = fSession->GetCurrentEventNumber();
      if( !fSession->GoToEvent(lRunNb +5)   ) break; // Stop when no more events to read, JB 2011/07/08
    }// end jumpy reading events
    else{
      Printf("MRaw::DisplayCumulatedRawData2D : pb in the definition of the event parsing... exit!");
      break;
    }

    // Printf("\n\n MRaw::DisplayCumulatedRawData2D :  Event[%d] \n", fSession->GetCurrentEventNumber() );

    tTracker->Update();
    TheNEvts_counter++;

    //    DHit *aHit;
    Int_t iCol, iRow;
    for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
      tPlane = tTracker->GetPlane(iPlane);

      if ( tPlane->GetReadout() >= 100){ // if sparsified readout

        aList = tPlane->GetListOfPixels();
        h1FiredPixelsPerEvent[iPlane-1]->Fill( aList->size());

        // Specific test for first event in order to generate
        //  list of DAQ index ordered pixel map
        if( storeOccurence && !ifUseDaqIndex[iPlane-1]
           && aList->size()>0 && aList->at(0)->GetDAQIndex()>=0 ) {
          ifUseDaqIndex[iPlane-1] = kTRUE;
          cout << "Plane " << iPlane << " uses DAQ pixel index !" << endl;
          h1HotPixelList[iPlane-1] = new TH1F( Form("h1HotPixelListPl%d",iPlane), "Occupancy versus DAQ index; DAQ index; Occupancy (%)", tPlane->GetStripsNu()*tPlane->GetStripsNv(), -0.5, tPlane->GetStripsNu()*tPlane->GetStripsNv()-0.5);
        }

        for( Int_t iPix=0; iPix<(Int_t)aList->size(); iPix++) { // loop on fired pixels
          iCol = aList->at(iPix)->GetPixelColumn();
          iRow = aList->at(iPix)->GetPixelLine();
          if( aList->at(iPix)->GetPulseHeight()>minSN ) {
            if( ( Colmin<0 || (iCol >= R_RawData_U[0][iPlane-1] && iCol <= R_RawData_U[1][iPlane-1])) &&
               ( Linmin<0 || (iRow >= R_RawData_V[0][iPlane-1] && iRow <= R_RawData_V[1][iPlane-1])) ) {
              hRDMap[iPlane-1]->Fill( iCol, iRow, 1); // JB 2009/08/31
              if( ifUseDaqIndex[iPlane-1] ) h1HotPixelList[iPlane-1]->Fill( aList->at(iPix)->GetDAQIndex() );
            }
          }

        } //end loop on fired pixels

      } //end of sparsified readout

      else if ( tPlane->GetReadout() < 100 && tPlane->GetReadout() > 0 ){ // non-sparsified readout, test if not 0 added (JB 2013/05/26)

        for( Int_t iStrip=0; iStrip<tPlane->GetStripsN(); iStrip++) {
          iCol = iStrip%tPlane->GetStripsNu();
          iRow = iStrip/tPlane->GetStripsNu();
          hRDMap[iPlane-1]->Fill( iCol, iRow, tPlane->GetPulseHeight(iStrip));
        } //end loop on strips

      } //end of non-sparsified readout


    } //end of loop on planes
  } // END LOOP ON EVENTS

  fSession->GetDataAcquisition()->PrintStatistics();
  tTracker->PrintStatistics();

  // Occurence and Noisy pixels study
  Float_t occurence;
  //Float_t occurence_step=(occurence_max-occurence_min)/(nOccurences-1);
  Float_t occurence_step_log = log10(occurence_max/occurence_min)/Float_t(nOccurences-1);
  Float_t *occurence_list=new Float_t[nOccurences];
  Float_t **nbPIPOT = new Float_t*[nPlanes]; //nb Pixels In Plane Over Threshold
  for (Int_t ioccurence_list=0; ioccurence_list<nOccurences; ioccurence_list++) {
    //linear
    //occurence_list[ioccurence_list]=occurence_min+occurence_step*ioccurence_list;

    //log //VR 2014.07.10
    occurence_list[ioccurence_list]=occurence_min * pow(10,occurence_step_log*ioccurence_list);
  }



  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    nbPIPOT[iPlane-1] = new Float_t[nOccurences]; //nb Pixels In Plane Over Threshold
    for (Int_t ioccurence_list=0; ioccurence_list<nOccurences; ioccurence_list++) nbPIPOT[iPlane-1][ioccurence_list]=0;
    for( Int_t xBin=0; xBin<hRDMap[iPlane-1]->GetNbinsX(); xBin++) {
      for( Int_t yBin=0; yBin<hRDMap[iPlane-1]->GetNbinsY(); yBin++) {
        occurence = hRDMap[iPlane-1]->GetBinContent( xBin+1, yBin+1)/(Float_t)nEvents;
        for (Int_t ioccurence_list=0; ioccurence_list<nOccurences; ioccurence_list++) {
          if (occurence >= occurence_list[ioccurence_list]) nbPIPOT[iPlane-1][ioccurence_list]+=1;
        }
      }
    }
  } // end loop on planes

  // Now display
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes

    pad->cd(iPlane);
    pad->cd(iPlane)->SetTickx(1);
    pad->cd(iPlane)->SetTicky(1);
    hRDMap[iPlane-1]->SetMinimum( minOccurence*nEvents); // JB 2011/03/14
    hRDMap[iPlane-1]->DrawCopy("colz");

    pad2->cd(iPlane);
    gnoisypixels[iPlane-1]= new TGraph(nOccurences,occurence_list,nbPIPOT[iPlane-1]);
    sprintf( name, "gnoisypixelspl%d", iPlane);
    sprintf( title, "Plane %s (#%d)", tTracker->GetPlane(iPlane)->GetPlanePurpose(), iPlane );
    gnoisypixels[iPlane-1]->SetName( name);
    gnoisypixels[iPlane-1]->SetTitle( title);
    gnoisypixels[iPlane-1]->Draw("AL*");
    gPad->SetLogx();
    gPad->SetGrid(1,1);
    gnoisypixels[iPlane-1]->GetXaxis()->SetTitle("relative occurence");
    gnoisypixels[iPlane-1]->GetXaxis()->SetLabelSize(0.05);
    gnoisypixels[iPlane-1]->GetXaxis()->SetTitleOffset();
    gnoisypixels[iPlane-1]->GetXaxis()->SetLabelOffset();
    gnoisypixels[iPlane-1]->GetXaxis()->SetTitleSize(0.05);
    gnoisypixels[iPlane-1]->GetYaxis()->SetTitle("number of pixels above relative occurence");
    gnoisypixels[iPlane-1]->GetYaxis()->SetLabelSize(0.05);
    gnoisypixels[iPlane-1]->GetYaxis()->SetTitleOffset(1.4);
    gnoisypixels[iPlane-1]->GetYaxis()->SetLabelOffset();
    gnoisypixels[iPlane-1]->GetYaxis()->SetTitleSize(0.05);

  } // end loop on planes
  ccumulrd->Update();
  cnoisypixels->Update();


#if 1
  // Prepare a file for print output of high occurence pixels
  if( storeOccurence ) {
    // Force the min occurence to be at least 0.01 hits/pixel otherwise all pixels will be registered
    Float_t minOccurenceForFile = (minOccurence<0.005)?0.005:minOccurence;
    Char_t fileName[100];
    sprintf( fileName, "%spixelOccurence_run%d_minocc%d.txt",
	     fSession->GetResultDirName().Data(),
	     fSession->GetRunNumber(),
	     Int_t(1000*minOccurence) ); // directory updated, JB 2009/09/18
    sprintf(fileName,"%s", fTool.LocalizeDirName( fileName)); // JB 2011/07/07
    cout << "\n-- Saving list of high occurence pixels into " << fileName << endl;

    FILE *outFile;
    outFile = fopen( fileName, "w");
    fprintf( outFile, "\nRun %d over %d events\n", fSession->GetRunNumber(), nEvents);
    fprintf( outFile, " Pixel (row,col) with occurence higher than %.1f percent\n", minOccurenceForFile*100.);
    for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
      fprintf( outFile, "\n** Plane %d BEWARE THIS MAY NOT BE THE INPUT NUMBER NEEDED FOR BoardReader!", iPlane);
      for( Int_t xBin=0; xBin<hRDMap[iPlane-1]->GetNbinsX(); xBin++) { // loop on X bins
        for( Int_t yBin=0; yBin<hRDMap[iPlane-1]->GetNbinsY(); yBin++) { // loop on Y bins
          occurence = hRDMap[iPlane-1]->GetBinContent( xBin+1, yBin+1)/(Float_t)nEvents;
          if( occurence > minOccurenceForFile ) {
            fprintf( outFile, "\n%4d %4d", yBin, xBin);
          }
        }  // end loop on Y bins
      } // end loop on X bins
    } // end llop on planes
  }
#endif

  // Save canvas and histos
  // cd to result dir
  // added JB 2011/03/14
  Char_t rootFile[300];
  sprintf(rootFile,"%sRawDataMap_run%d.root",fSession->GetResultDirName().Data(),fSession->GetRunNumber());
  sprintf(rootFile,"%s", fTool.LocalizeDirName( rootFile)); // JB 2011/07/07
  cout << "\n-- Saving histos and canvas into " << rootFile << endl;
  TFile fRoot(rootFile,"RECREATE");
  ccumulrd->Write();
  cnoisypixels->Write();
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    hRDMap[iPlane-1]->Write();
    //hRDMap[iPlane-1]->Delete();
    gnoisypixels[iPlane-1]->Write();
    h1FiredPixelsPerEvent[iPlane-1]->Write();
  }
  fRoot.Close();

  // Store histo with occurence for a specific plane
  //  in order to be used as a HotHitMap
  // added, JB 2014/03/11
  Char_t HotPixelFileName[500];
  TFile *HotPixelFile;
  TH2F *h2HotPixelMap;
  if( storeOccurence ) {
    for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
      sprintf(HotPixelFileName,"%shotPixelMap_run%d_pl%d.root",fSession->GetResultDirName().Data(),fSession->GetRunNumber(),iPlane);
      HotPixelFile = new TFile(HotPixelFileName,"RECREATE");
      h2HotPixelMap = (TH2F*)hRDMap[iPlane-1]->Clone("h2HotPixelMap");
      cout << "Plane " << iPlane << "  Integral = " << h2HotPixelMap->Integral() << "  Nevts = " << TheNEvts_counter << ", Ratio = " << h2HotPixelMap->Integral()/TheNEvts_counter << endl;
      h2HotPixelMap->Scale(100./(Float_t)TheNEvts_counter);
      h2HotPixelMap->SetXTitle("col #");
      h2HotPixelMap->GetXaxis()->CenterTitle(true);
      h2HotPixelMap->SetYTitle("lin #");
      h2HotPixelMap->GetYaxis()->CenterTitle(true);
      h2HotPixelMap->SetZTitle("Fake Rate (%)");
      h2HotPixelMap->GetZaxis()->CenterTitle(true);

      if( ifUseDaqIndex[iPlane-1] ) h1HotPixelList[iPlane-1]->Scale(100./(Float_t)nEvents);

      const int Nsteps = 100;
      double R_fakeRateCut[2];
      R_fakeRateCut[0] = 0.0*100.0;
      R_fakeRateCut[1] = 1.0e-2*100.0;

      double factor = 1.0e-5;
      TH1F* hAveFakeRate = new TH1F("hAveFakeRate","Average Fake Rate vs singe pixel fake rate cut",Nsteps,R_fakeRateCut[0],R_fakeRateCut[1]);
      hAveFakeRate->SetXTitle("Pixel fake rate cut (%)");
      hAveFakeRate->GetXaxis()->CenterTitle(true);
      hAveFakeRate->SetYTitle("<Fake Rate> (#times10^{-5})");
      hAveFakeRate->GetYaxis()->CenterTitle(true);
      hAveFakeRate->SetLineColor(kBlue);
      hAveFakeRate->SetLineWidth(2);
      hAveFakeRate->SetStats(kFALSE);

      TH1F* hRejectedPixels = new TH1F("hRejectedPixels","Fraction of rejected pixels vs singe pixel fake rate cut",Nsteps,R_fakeRateCut[0],R_fakeRateCut[1]);
      hRejectedPixels->SetXTitle("Pixel fake rate cut (%)");
      hRejectedPixels->GetXaxis()->CenterTitle(true);
      //hRejectedPixels->SetYTitle("Frac. Reject. Pixels (%)");
      hRejectedPixels->SetYTitle("# Reject. Pixels");
      hRejectedPixels->GetYaxis()->CenterTitle(true);
      hRejectedPixels->SetLineColor(kBlue);
      hRejectedPixels->SetLineWidth(2);
      hRejectedPixels->SetStats(kFALSE);

      TH1F* hAveFakeRateTot = new TH1F("hAveFakeRateTot","Average Fake Rate in whole matrix",1,0.0,1.0);
      hAveFakeRateTot->SetXTitle("");
      hAveFakeRateTot->GetXaxis()->CenterTitle(true);
      hAveFakeRateTot->SetYTitle("<Fake Rate> (#times10^{-5})");
      hAveFakeRateTot->GetYaxis()->CenterTitle(true);
      hAveFakeRateTot->SetLineColor(kBlue);
      hAveFakeRateTot->SetLineWidth(2);
      hAveFakeRateTot->SetStats(kFALSE);
      hAveFakeRateTot->GetXaxis()->SetBinLabel(1,"");

      int NHotPixels = 20;
      TString HistTitle = TString("Average Fake Rate in whole matrix minus the ") + long(NHotPixels) + TString(" hotest pixels");
      TH1F* hAveFakeRateMinusHot = new TH1F("hAveFakeRateMinusHot",
                                            HistTitle.Data(),
                                            1,0.0,1.0);
      hAveFakeRateMinusHot->SetXTitle("");
      hAveFakeRateMinusHot->GetXaxis()->CenterTitle(true);
      hAveFakeRateMinusHot->SetYTitle("<Fake Rate> (#times10^{-5})");
      hAveFakeRateMinusHot->GetYaxis()->CenterTitle(true);
      hAveFakeRateMinusHot->SetLineColor(kRed);
      hAveFakeRateMinusHot->SetLineWidth(2);
      hAveFakeRateMinusHot->SetStats(kFALSE);
      hAveFakeRateMinusHot->GetXaxis()->SetBinLabel(1,"");

      int NAceptpixels[Nsteps];
      double AveFake[Nsteps];
      //double FracReject[Nsteps];
      for(int istep=0;istep<Nsteps;istep++) {
        AveFake[istep]      = 0.0;
        //FracReject[istep]   = 0.0;
        NAceptpixels[istep] = 0;
      }

      double Total_AveFake = 0.0;
      for(int ix=0;ix<h2HotPixelMap->GetXaxis()->GetNbins();ix++) {
        for(int iy=0;iy<h2HotPixelMap->GetYaxis()->GetNbins();iy++) {
          double fake_rate = h2HotPixelMap->GetBinContent(ix+1,iy+1)/100.0;
          Total_AveFake += fake_rate;
        }
      }
      Total_AveFake /= (h2HotPixelMap->GetXaxis()->GetNbins()*h2HotPixelMap->GetYaxis()->GetNbins());
      hAveFakeRateTot->SetBinContent(1,Total_AveFake/factor);

      for(int istep=0;istep<Nsteps;istep++) {
        for(int ix=0;ix<h2HotPixelMap->GetXaxis()->GetNbins();ix++) {
          for(int iy=0;iy<h2HotPixelMap->GetYaxis()->GetNbins();iy++) {
            double fake_rate = h2HotPixelMap->GetBinContent(ix+1,iy+1)/100.0;

            if(fake_rate < hAveFakeRate->GetBinCenter(istep+1)/100.0) {
              AveFake[istep] += fake_rate;
              NAceptpixels[istep]++;
            }

          }
        }
      }

      int TotNpixels = h2HotPixelMap->GetXaxis()->GetNbins()*h2HotPixelMap->GetYaxis()->GetNbins();
      for(int istep=0;istep<Nsteps;istep++) {
        AveFake[istep] /= NAceptpixels[istep];
        hAveFakeRate->SetBinContent(istep+1,AveFake[istep]/factor);

        //double Reject  = (double)NAceptpixels[istep]/TotNpixels;
        //Reject         = 1.0 - Reject;
        //Reject        *= 100.0
        int Reject  = TotNpixels - NAceptpixels[istep];
        hRejectedPixels->SetBinContent(istep+1,Reject);
      }

      //int NHotPixels = 20;
      if(hRejectedPixels->GetBinContent(hRejectedPixels->GetXaxis()->GetNbins()) > NHotPixels) {
        hAveFakeRateMinusHot->SetBinContent(1,hAveFakeRate->GetBinContent(hRejectedPixels->GetXaxis()->GetNbins()));
      }
      else if(hRejectedPixels->GetBinContent(1) < NHotPixels) {
        hAveFakeRateMinusHot->SetBinContent(1,hAveFakeRate->GetBinContent(1));
      }
      else {
        double fake_rate_tmp = -999.0;
        for(int istep=0;istep<hRejectedPixels->GetXaxis()->GetNbins()-1;istep++) {
          double x1 = hRejectedPixels->GetBinCenter(istep+1);
          double x2 = hRejectedPixels->GetBinCenter(istep+2);
          double y1 = hRejectedPixels->GetBinContent(istep+1);
          double y2 = hRejectedPixels->GetBinContent(istep+2);

          if(y1 >= NHotPixels &&
             y2 <  NHotPixels) {
            double a = (y2-y1)/(x2-x1);
            double b = y2 - a*x2;
            if(TMath::Abs(a) > 1.0e-8) {
              fake_rate_tmp  = (NHotPixels - b)/a;
            }
            else fake_rate_tmp = 0.5*(x2 + x1);

            break;
          }
        }

        hAveFakeRateMinusHot->SetBinContent(1,hAveFakeRate->GetBinContent(hAveFakeRate->FindBin(fake_rate_tmp)));
      }

      TCanvas cFakeRate("cFakeRate","Hot pixels plots", 5, 5,800,700);
      cFakeRate.SetFillColor(10);
      cFakeRate.SetFrameFillColor(10);
      cFakeRate.Divide(2,2);
      cFakeRate.cd(1);
      gPad->SetFillColor(10);
      gPad->SetFrameFillColor(10);
      gPad->SetTickx(1);
      gPad->SetTicky(1);
      gPad->SetLeftMargin(0.20);
      gPad->SetBottomMargin(0.20);
      gPad->SetRightMargin(0.20);
      h2HotPixelMap->Draw("colz");
      cFakeRate.cd(2);
      gPad->SetFillColor(10);
      gPad->SetFrameFillColor(10);
      gPad->SetTickx(1);
      gPad->SetTicky(1);
      gPad->SetLeftMargin(0.20);
      gPad->SetBottomMargin(0.20);
      gPad->SetRightMargin(0.20);
      hAveFakeRate->Draw();
      cFakeRate.cd(3);
      gPad->SetFillColor(10);
      gPad->SetFrameFillColor(10);
      gPad->SetTickx(1);
      gPad->SetTicky(1);
      gPad->SetLeftMargin(0.20);
      gPad->SetBottomMargin(0.20);
      gPad->SetRightMargin(0.20);
      hRejectedPixels->Draw();
      cFakeRate.cd(4);
      gPad->SetFillColor(10);
      gPad->SetFrameFillColor(10);
      gPad->SetTickx(1);
      gPad->SetTicky(1);
      gPad->SetLeftMargin(0.20);
      gPad->SetBottomMargin(0.20);
      gPad->SetRightMargin(0.20);
      double Maximum = TMath::Max(hAveFakeRateTot->GetMaximum(),
                                  hAveFakeRateMinusHot->GetMaximum());
      double Minimum = TMath::Min(hAveFakeRateTot->GetMaximum(),
                                  hAveFakeRateMinusHot->GetMaximum());
      double porcent = 0.50;
      hAveFakeRateTot->SetMaximum(Maximum + porcent*(Maximum-Minimum));
      hAveFakeRateTot->SetMinimum(Minimum - porcent*(Maximum-Minimum));
      hAveFakeRateMinusHot->SetMaximum(Maximum + porcent*(Maximum-Minimum));
      hAveFakeRateMinusHot->SetMinimum(Minimum - porcent*(Maximum-Minimum));
      hAveFakeRateTot->Draw();
      hAveFakeRateMinusHot->Draw("same");
      char ytitle[200];
      sprintf(ytitle,"<Fake> = %.5f #times 10^{-5}",hAveFakeRateTot->GetBinContent(1));
      TLatex* latex = new TLatex();
      latex->SetTextAlign(12);
      latex->SetTextSize(0.04);
      latex->SetTextColor(kBlack);
      porcent = 0.05;
      double porcentY = 0.05;
      latex->DrawLatex(hAveFakeRateTot->GetXaxis()->GetXmin() + porcent*(hAveFakeRateTot->GetXaxis()->GetXmax() - hAveFakeRateTot->GetXaxis()->GetXmin()),
                       hAveFakeRateTot->GetMaximum() - porcentY*(hAveFakeRateTot->GetMaximum() - hAveFakeRateTot->GetMinimum()),
                       ytitle);

      sprintf(ytitle,"<Fake> = %.5f #times 10^{-5} (- %d hot pix)",hAveFakeRateMinusHot->GetBinContent(1),NHotPixels);
      latex->SetTextColor(kRed);
      porcentY = 0.15;
      latex->DrawLatex(hAveFakeRateTot->GetXaxis()->GetXmin() + porcent*(hAveFakeRateTot->GetXaxis()->GetXmax() - hAveFakeRateTot->GetXaxis()->GetXmin()),
                       hAveFakeRateTot->GetMaximum() - porcentY*(hAveFakeRateTot->GetMaximum() - hAveFakeRateTot->GetMinimum()),
                       ytitle);


      cFakeRate.Write();
      h2HotPixelMap->Write();
      if( ifUseDaqIndex[iPlane-1] ) h1HotPixelList[iPlane-1]->Write();
      hAveFakeRate->Write();
      hRejectedPixels->Write();
      hAveFakeRateTot->Write();
      hAveFakeRateMinusHot->Write();

      cout << "-------- The HOT PIXEL MAP FOR PLANE " << iPlane << " HAS BEEN FILLED !"<<endl;
      HotPixelFile->Close();
    }
  }

}

//______________________________________________________________________________
//
void MRaw::DisplayCumulatedOverflow( Int_t nEvents)
{
  // Display the line overflow for each plane cumulated over the requested number of events
  // Originally used for PLUME
  // MG 2012/03/14

  fSession->SetEvents(nEvents);

  TCanvas *ccumulovf;
  TObject* g = gROOT->FindObject("ccumulovf") ;
  if (g) {
    ccumulovf = (TCanvas*)g;
  }
  else {
    ccumulovf = new TCanvas("ccumulovf", "Cumulated Line Overflow", 5, 5,800,700);
  }
  ccumulovf->Clear();
  ccumulovf->UseCurrentStyle();
  TPaveLabel* label = new TPaveLabel();
  Char_t canvasTitle[200];
  sprintf(canvasTitle, "Run %d, cumul over %d events", fSession->GetRunNumber(), nEvents);
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad = new TPad("pad","",0.,0.,1.,0.965);
  pad->Draw();

  DTracker *tTracker  =  fSession->GetTracker();
  DAcq *tDAcq = fSession->GetDataAcquisition();//MG 2012/02/21

  Int_t nPlanes = tTracker->GetPlanesN();
  Int_t iCol, iRow;
  Int_t OverflowN[nPlanes], EventOverflow[nPlanes];
  for (int i=0;i<nPlanes;i++) {
    OverflowN[i]=0.;
    EventOverflow[i]=0.;
  }

  //  pad->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);
  pad->Divide( (Int_t)ceil(nPlanes/2.), 3);

  DPlane* tPlane;
  TH1F **hOVFPlot = new TH1F*[nPlanes];
  TH2F **hOVFMap = new TH2F*[nPlanes];
  TGraphErrors ** grOVF = new TGraphErrors*[nPlanes];
  Double_t LineOverflowN[nPlanes][nEvents];
  Double_t xnum[nEvents];
  Int_t Hits_subA[nPlanes];
  Int_t const numpoints = nEvents;//eventsRead / NeventRangeForEfficiency;
  //greff = new TGraphErrors(numpoints,xnum,temp_Efficiency_array,xerr,yerr);
  Char_t name[50], title[100];
 std::vector<int> *ListOfLineOverflow=tDAcq->GetLineOverflow();//MG 2012/02/21
  Int_t fLineOverflowN[nPlanes];

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    // -- Histo with pixel number
    tPlane = tTracker->GetPlane(iPlane);
    sprintf( name, "hovfplotpl%d", iPlane);
    sprintf( title, "Line Overflow of plane %d", iPlane);

    if(iPlane == 7) sprintf( title, "Line Overflow of plane 8");
    if(iPlane == 8) sprintf( title, "Line Overflow of plane 7");
    // check if the histo is existing or not
    hOVFPlot[iPlane-1] = new TH1F(name, title, tPlane->GetStripsNv(), 0, tPlane->GetStripsNv());//MG pipo
    sprintf( name, "hovfmap%d", iPlane);
    sprintf( title, "Hit map of plane %d", iPlane);
    hOVFMap[iPlane-1] = new TH2F(name, title, tPlane->GetStripsNu(), 0, tPlane->GetStripsNu(), tPlane->GetStripsNv(), 0, tPlane->GetStripsNv());

    hOVFPlot[iPlane-1]->SetMarkerStyle(20);
    hOVFPlot[iPlane-1]->SetMarkerSize(.1);
    hOVFPlot[iPlane-1]->SetMarkerColor(1);
    Hits_subA[iPlane-1]=0;//MG init
  }

  for( Int_t iEvt=0; iEvt < nEvents; iEvt++) {
    if( !fSession->NextRawEvent() ) break; // Stop when no more events to read, JB 2011/07/08
    ListOfLineOverflow=tDAcq->GetLineOverflow();//MG 2012/02/21

    for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
     std::vector<DPixel*> *aList = tDAcq->GetListOfPixels(iPlane);

      for( Int_t iPix=0; iPix<(Int_t)aList->size(); iPix++) { // loop on fired pixels
        iCol = aList->at(iPix)->GetPixelColumn();
        iRow = aList->at(iPix)->GetPixelLine();
        if( aList->at(iPix)->GetPulseHeight()>0 ) hOVFMap[iPlane-1]->Fill( iCol, iRow, 1); // JB 2009/08/31*/
        if( aList->at(iPix)->GetPulseHeight()>0 && iCol<288) Hits_subA[iPlane-1]++;
      } //end loop on fired pixels

      fLineOverflowN[iPlane-1]=ListOfLineOverflow[iPlane-1].size() ;
      LineOverflowN[iPlane-1][iEvt]=fLineOverflowN[iPlane-1];
      OverflowN[iPlane-1]+=fLineOverflowN[iPlane-1];
      if(fLineOverflowN[iPlane-1]>0) EventOverflow[iPlane-1]++;
      xnum[iEvt]=iEvt;
      for( Int_t iLOv=0; iLOv<fLineOverflowN[iPlane-1]; iLOv++) {
        hOVFPlot[iPlane-1]->Fill(ListOfLineOverflow[iPlane-1].at( iLOv));
        ///for( Int_t iLOv=0; iLOv<fLineOverflowN[aInput-1]; iLOv++) {
        ///	  hOVFPlot[iPlane-1]->Fill(ListOfLineOverflow[aInput-1].at( iLOv));
        ///    if (iPlane==8)cout << ListOfLineOverflow[iPlane].at( iLOv) << ", ";
      }
      //	      if(fLineOverflowN[iPlane]!=0)cout<<endl;
    }

  } // END LOOP ON EVENTS

  // Now display
  // and compute & print occurence
  for( Int_t iPlane=5; iPlane<=nPlanes; iPlane++) { // loop on planes

    Int_t MatchingPlane = iPlane;
    if(iPlane==8) MatchingPlane = 7;
    if(iPlane==7) MatchingPlane = 8;
    cout << "Be carefull : Plane 7 and 8 are exchanged for the ovf plots" << endl << "Check config file!" << endl;


    pad->cd(iPlane-4);
    hOVFMap[iPlane-1]->DrawCopy("colz");

    pad->cd(iPlane);
    hOVFPlot[MatchingPlane-1]->DrawCopy("");
    ///    hOVFPlot[iPlane-1]->DrawCopy("");

    pad->cd(iPlane+4);
    grOVF[MatchingPlane-1] = new TGraphErrors(numpoints, xnum, LineOverflowN[MatchingPlane-1], 0, 0);
    ///    grOVF[MatchingPlane-1] = new TGraphErrors(numpoints, xnum, LineOverflowN[iPlane-1], 0, 0);
    sprintf( title, "Line overflow of plane %d per event", iPlane);
    grOVF[MatchingPlane-1]->SetTitle(title);
    grOVF[MatchingPlane-1]->Draw("ALP");
    /*    grOVF[iPlane-1] = new TGraphErrors(numpoints, xnum, LineOverflowN[iPlane-1], 0, 0);
     sprintf( title, "Line overflow of plane %d per event", iPlane);
     grOVF[iPlane-1]->SetTitle(title);
     grOVF[iPlane-1]->Draw("ALP");*/

    cout<<"Plane "<< iPlane <<" ovf "<< EventOverflow[iPlane-1] << " Fake A " << Hits_subA[iPlane-1]/165888./nEvents << endl;

    //    }
  } // end loop on planes
  //    ccumulovf->Update();

  //  fRoot.Close();

}

//______________________________________________________________________________
//
void MRaw::DisplayNoise( Int_t thePlaneNumber, Float_t calibFactor, Float_t maxAxisNoise, Float_t maxAxisPed, Bool_t ifPixelHistory)
{
  // Display the noise (and pedestal) per strip or pixel for each plane
  // after it has been computed for the required amount of events.
  //
  // If a plane number is specified (-1 means no plane),
  //  only this plane will be displayed
  //  and its autocorrelation between 2 consecutive frames is computed.
  //  A plane number is required for the "ifPixelHistory" option described below.
  //
  // If a calibFactor is specified (-1 means no factor),
  //  the noise value are scaled by this factor
  //  and the units are e- instead of ADCu.
  //
  // If maxAxisNoise/Ped positive, will be used to define axis range of relevant histos
  //
  // If ifPixelHistory is true (false by default),
  //  a histogram for the signal distribution of EACH pixel is filled.
  //  This options requires that thePlaneNumber is set.
  //
  // Call by gTAF->GetRaw()->DisplayNoise()
  //
  // JB, September 2007
  // Modified: JB 2012/09/22 autocorrelation and reshaping
  // Modified: JB 2013/08/29 manage digitize data whith non-sparsified readout
  // Modified: JB 2013/09/16 introduction of calibration factor
  // Modified: JB 2013/12/03 histos for individual pixel distribution
  // Modified: JB 2017/04/21 axis range for distribution

  Int_t events2Read = 1;

  DTracker *tTracker  =  fSession->GetTracker();
  Int_t nPlanes = tTracker->GetPlanesN();
  DPlane* tPlane = NULL;
  Int_t iRow, iCol, st;
  Double_t aNoise, aPedestal, autocorr;
  Char_t name[50], title[100];

  // Taking into account potential calibration
  // JB 2013/09/16
  Char_t units[20];
  if( calibFactor>0. ) { // use the calibration
    sprintf( units, "(e-)");
  }
  else { // set the scale to 1.
    calibFactor = 1.;
    sprintf( units, "(ADCu)");
  }


  // Analysis of a single plane
  // For autocorrelation computation
  // For display of individual pixel distribution, JB 2013/12/03
  DPlane *thePlane = 0;
  Double_t *previousValue = NULL;
  TH1F **hPixelDistri = NULL;
  if( thePlaneNumber>0 ) { // if a plane is specified
    thePlaneNumber = thePlaneNumber<=nPlanes?thePlaneNumber:nPlanes;
    cout << "The plane considered is " << thePlaneNumber << endl;
    thePlane = tTracker->GetPlane( thePlaneNumber);

    previousValue = new Double_t[thePlane->GetStripsN()];
    fSession->NextRawEvent();
    tTracker->Update();

    hPixelDistri = new TH1F*[thePlane->GetStripsN()];
    for (Int_t iStrip=0; iStrip<thePlane->GetStripsN(); iStrip++) {
      //cout << "strip " << iStrip << ", value = " << thePlane->GetRawValue(iStrip+1) << endl;
      previousValue[iStrip] = thePlane->GetRawValue(iStrip+1);
      sprintf( name, "hPixelDistri%d", iStrip);
      sprintf( title, "Pixel %d signal distribution over %d events", iStrip, thePlane->GetInitialNoise());
      hPixelDistri[iStrip] = new TH1F( name, title, 50, -200., 200.);
      hPixelDistri[iStrip]->SetXTitle(units);
      hPixelDistri[iStrip]->SetLineColor(1+iStrip%9);
    }

  } // end if a plane is specified


  // Define histos for each plane
  TH1F **hPedDistri = new TH1F*[nPlanes];
  TH1F **h1Pedestal = new TH1F*[nPlanes];
  TH2F **h2Pedestal = new TH2F*[nPlanes];
  TH1F **hNoiseDistri = new TH1F*[nPlanes];
  TH1F **hAutocorDistri = new TH1F*[nPlanes];
  TH1F **h1Noise = new TH1F*[nPlanes];
  TH2F **h2Noise = new TH2F*[nPlanes];
  TH2F **h2Autocor = new TH2F*[nPlanes];
  Int_t *tMode     = new Int_t[nPlanes];
  Int_t *tReadout  = new Int_t[nPlanes];
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    tPlane        = tTracker->GetPlane(iPlane);
    tMode[iPlane-1] = tPlane->GetAnalysisMode();
    tReadout[iPlane-1] = tPlane->GetReadout();

    if( events2Read < (Int_t)(tPlane->GetInitialNoise()) ) {
      events2Read = (Int_t)(tPlane->GetInitialNoise())/*+1*/;
    }

    if( tMode[iPlane-1]==1 ) { // STRIPS
      sprintf( name, "hPedestalpl%d", iPlane);
      sprintf( title, "Pedestal of plane %d", iPlane);
      h1Pedestal[iPlane-1] = new TH1F(name, title, tPlane->GetStripsNu(), 1, tPlane->GetStripsNu()+1);
      h1Pedestal[iPlane-1]->SetXTitle("strip index");
      h1Pedestal[iPlane-1]->SetYTitle(units);
      sprintf( name, "hnoisepl%d", iPlane);
      sprintf( title, "Noise of plane %d", iPlane);
      h1Noise[iPlane-1] = new TH1F(name, title, tPlane->GetStripsNu(), 1, tPlane->GetStripsNu()+1);
      h1Noise[iPlane-1]->SetXTitle("strip index");
      h1Noise[iPlane-1]->SetYTitle(units);
      if(fVerbose) printf( "MRaw::DisplayNoise created %s histo with %d channels\n", name, tPlane->GetStripsNu());
    }
    else if ( (tMode[iPlane-1]==2 || tPlane->GetDigitalThresholdNb()>0) ) { // PIXELS
      sprintf( name, "hPedestalpl%d", iPlane);
      sprintf( title, "Pedestal of plane %d", iPlane);
      h2Pedestal[iPlane-1] = new TH2F(name, title,
				      tPlane->GetStripsNu(), -0.5, tPlane->GetStripsNu()-1+0.5,
				      tPlane->GetStripsNv(), -0.5, tPlane->GetStripsNv()-1+0.5);
      h2Pedestal[iPlane-1]->SetStats(0);
      h2Pedestal[iPlane-1]->SetXTitle("column index");
      h2Pedestal[iPlane-1]->SetYTitle("row index");
      h2Pedestal[iPlane-1]->SetZTitle(units);
      sprintf( name, "hnoisepl%d", iPlane);
      sprintf( title, "Noise of plane %d", iPlane);
      h2Noise[iPlane-1] = new TH2F(name, title,
				   tPlane->GetStripsNu(), -0.5, tPlane->GetStripsNu()-1+0.5,
				   tPlane->GetStripsNv(), -0.5, tPlane->GetStripsNv()-1+0.5);
      h2Noise[iPlane-1]->SetStats(0);
      h2Noise[iPlane-1]->SetXTitle("column index");
      h2Noise[iPlane-1]->SetYTitle("row index");
      h2Noise[iPlane-1]->SetZTitle(units);
      sprintf( name, "hautocorpl%d", iPlane);
      sprintf( title, "Autocorrelation of plane %d", iPlane);
      h2Autocor[iPlane-1] = new TH2F(name, title, tPlane->GetStripsNu(), 1, tPlane->GetStripsNu()+1, tPlane->GetStripsNv(), 1, tPlane->GetStripsNv()+1);
      h2Autocor[iPlane-1]->SetStats(0);
      h2Autocor[iPlane-1]->Sumw2();
      if(fVerbose) printf( "MRaw::DisplayNoise created %s histo with %d x %d channels\n", name, tPlane->GetStripsNu(), tPlane->GetStripsNv());
    }
    else if ( tMode[iPlane-1]==3 ) { // PIXELS with binary output
      if( fVerbose ) printf("\n-*-* INFO: there is no noise computation in mode %d for plane %d\n", tPlane->GetAnalysisMode(), iPlane);
    }
    else { // unknown case
      printf("\n-*-* WARNING: unknown analysis mode %d for plane %d\n", tMode[iPlane-1], iPlane);
    }
  } //end loop on planes


  // Loop on events
  fSession->SetEvents(events2Read);
  printf("\n Will read %d events to compute noise\n", events2Read);
  while( fSession->NextRawEvent() ){ // event loop
    tTracker->Update();

    // For autocorrelation computation and pixel distribution
    if( thePlaneNumber>0 ) {
      for (Int_t iStrip=0; iStrip<thePlane->GetStripsN(); iStrip++) {
        iCol = iStrip%tPlane->GetStripsNu() + 1;
        iRow = iStrip/tPlane->GetStripsNu() + 1;
        //cout << "strip " << iStrip << ", [" << iCol << ", " << iRow << "], value = " << thePlane->GetRawValue(iStrip+1) << ", prevalue = " << previousValue[iStrip] << endl;
        h2Autocor[thePlaneNumber-1]->Fill( iCol, iRow, thePlane->GetRawValue(iStrip+1)*previousValue[iStrip] );
        previousValue[iStrip] = thePlane->GetRawValue(iStrip+1);
        hPixelDistri[iStrip]->Fill( thePlane->GetRawValue(iStrip+1) * calibFactor );
      }
    }

  } //end loop on event


  // Get the noise and pedestal for each strip

  Double_t minNoise=1.e5, maxNoise=-1.e5;
  Double_t minPed=1.e5, maxPed=-1.e5;

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    tPlane        = tTracker->GetPlane(iPlane);
    if(fVerbose) cout << endl << "PLANE " << iPlane << endl;
    if( tMode[iPlane-1]==1 && tReadout[iPlane-1]>0 ) { // STRIPS
      for( Int_t iStrip=1; iStrip<=h1Noise[iPlane-1]->GetNbinsX(); iStrip++) {
        aNoise = tPlane->GetNoise(iStrip) * calibFactor;
        if(fVerbose) cout<<"aNoise["<<iStrip<<"]: " << aNoise << endl;
        if( aNoise<minNoise) minNoise = aNoise;
        if( maxNoise<aNoise) maxNoise = aNoise;
        h1Noise[iPlane-1]->SetBinContent( iStrip, aNoise);
        aPedestal = tPlane->GetPedestal(iStrip) * calibFactor;
        if(fVerbose) cout<<"aPedestal["<<iStrip<<"]: " << aPedestal << endl;
        if( aPedestal<minPed) minPed = aPedestal;
        if( maxPed<aPedestal) maxPed = aPedestal;
        h1Pedestal[iPlane-1]->SetBinContent( iStrip, aPedestal);
      }
    }
    else if ( (tMode[iPlane-1]==2 || tPlane->GetDigitalThresholdNb()>0) && tReadout[iPlane-1]>0 ) { // PIXELS
      for( Int_t iStripU=1; iStripU<=h2Noise[iPlane-1]->GetNbinsX(); iStripU++) {
        for( Int_t iStripV=1; iStripV<=h2Noise[iPlane-1]->GetNbinsY(); iStripV++) {
          st = iStripU+(iStripV-1)*tPlane->GetStripsNu()-1;
          aNoise = tPlane->GetNoise( st) * calibFactor;
          if( aNoise<minNoise) minNoise = aNoise;
          if( maxNoise<aNoise) maxNoise = aNoise;
          h2Noise[iPlane-1]->SetBinContent( iStripU, iStripV, aNoise);
          if(fVerbose) printf("MRaw::DisplayNoise plane %d, strips( %d, %d) channels %d, noise %f\n", iPlane, iStripU, iStripV, st, aNoise);
          aPedestal = tPlane->GetPedestal(st) * calibFactor;
          if( aPedestal<minPed) minPed = aPedestal;
          if( maxPed<aPedestal) maxPed = aPedestal;
          h2Pedestal[iPlane-1]->SetBinContent( iStripU, iStripV, aPedestal);
          if(fVerbose) printf("MRaw::DisplayPedestal plane %d, strips( %d, %d) channels %d, pedestal %f\n", iPlane, iStripU, iStripV, st, aPedestal);
        }
      }
    }

  } //end loop on planes


  // Display the noises & pedestal histos
  cout << "Display the noises & pedestal histos" << endl;
  gStyle->SetPalette(1);
  Char_t canvasTitle[200];
  sprintf(canvasTitle, "Display Noise for run %d", fSession->GetRunNumber());
  TCanvas *cnoise = new TCanvas("cnoise", canvasTitle, 0, 0, 800, 700);
  cnoise->UseCurrentStyle();

  sprintf(canvasTitle, "Display Pedestal for run %d", fSession->GetRunNumber());
  TCanvas *cpedestal = new TCanvas("cpedestal", canvasTitle, 15, 0, 800, 700);
  cpedestal->UseCurrentStyle();

  if ( thePlaneNumber>0 ) { // when only one plane is displayed
    if( tMode[thePlaneNumber-1]==1 && tReadout[thePlaneNumber-1]>0 ) { // STRIPS
      cnoise->cd();
      h1Noise[thePlaneNumber-1]->Draw();
      cpedestal->cd();
      h1Pedestal[thePlaneNumber-1]->Draw();
    }
    else if ( (tMode[thePlaneNumber-1]==2 || tPlane->GetDigitalThresholdNb()>0) && tReadout[thePlaneNumber-1]>0 ) { // PIXELS
      cnoise->cd();
      cnoise->SetBottomMargin(0.20);
      cnoise->SetLeftMargin(0.15);
      cnoise->SetRightMargin(0.15);
      h2Noise[thePlaneNumber-1]->Draw("colz");
      cpedestal->cd();
      cpedestal->SetBottomMargin(0.15);
      cpedestal->SetLeftMargin(0.15);
      cpedestal->SetRightMargin(0.15);
      h2Pedestal[thePlaneNumber-1]->Draw("colz");
    }

  }
  else { // when several planes are displayed
    Int_t nCanvasRows = 1;
    if ( 9<nPlanes) nCanvasRows = 5;
    else if ( 4<nPlanes ) nCanvasRows = 3;
    else if ( 1<nPlanes ) nCanvasRows = 2;
    cnoise->Divide( (Int_t)ceil(nPlanes/(Float_t)nCanvasRows), nCanvasRows);
    cpedestal->Divide( (Int_t)ceil(nPlanes/(Float_t)nCanvasRows), nCanvasRows);
    for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
      tPlane        = tTracker->GetPlane(iPlane);
      if( tMode[iPlane-1]==1 && tReadout[iPlane-1]>0 ) { // STRIPS
        cnoise->cd(iPlane);
        h1Noise[iPlane-1]->Draw();
        cpedestal->cd(iPlane);
        h1Pedestal[iPlane-1]->Draw();
      }
      else if ( (tMode[iPlane-1]==2 || tPlane->GetDigitalThresholdNb()>0) && tReadout[iPlane-1]>0 ) { // PIXELS
        if(fVerbose) cout << "Drawing noise for plane " << tPlane->GetPlaneNumber() << endl;
        cnoise->cd(iPlane);
        cnoise->SetBottomMargin(0.15);
        cnoise->SetLeftMargin(0.15);
        cnoise->SetRightMargin(0.15);
        h2Noise[iPlane-1]->Draw("colz");
        cpedestal->cd(iPlane);
        cpedestal->SetBottomMargin(0.15);
        cpedestal->SetLeftMargin(0.15);
        cpedestal->SetRightMargin(0.15);
        h2Pedestal[iPlane-1]->Draw("colz");
      }
    }
  }
  cnoise->Update();
  cpedestal->Update();


  // Display the distributions in histos
  cout << "Display the distributions in histos" << endl;
  sprintf(canvasTitle, "Display Distributions for run %d", fSession->GetRunNumber());
  TCanvas *cdistri = new TCanvas("cdistri", canvasTitle, 30, 0, 800, 700);
  cdistri->UseCurrentStyle();
  cdistri->Divide( 2, nPlanes);

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {

    tPlane        = tTracker->GetPlane(iPlane);

    double R_pedestal[2];
    double R_noise[2];
    R_pedestal[0] = R_noise[0] = +1.0e+20;
    R_pedestal[1] = R_noise[1] = -1.0e+20;
    if( tMode[iPlane-1]==1 && tReadout[iPlane-1]>0 ) { // STRIPS
      for( Int_t iStrip=1; iStrip<=tPlane->GetStripsNu(); iStrip++) {
        if(R_pedestal[0] > tPlane->GetPedestal(iStrip) * calibFactor) R_pedestal[0] = tPlane->GetPedestal(iStrip) * calibFactor;
        if(R_pedestal[1] < tPlane->GetPedestal(iStrip) * calibFactor) R_pedestal[1] = tPlane->GetPedestal(iStrip) * calibFactor;

        if(R_noise[0] > tPlane->GetNoise(iStrip) * calibFactor) R_noise[0] = tPlane->GetNoise(iStrip) * calibFactor;
        if(R_noise[1] < tPlane->GetNoise(iStrip) * calibFactor) R_noise[1] = tPlane->GetNoise(iStrip) * calibFactor;
      }
    }
    else if ( (tMode[iPlane-1]==2 || tPlane->GetDigitalThresholdNb()>0) && tReadout[iPlane-1]>0 ) { // PIXELS with analog output
      for( Int_t iStrip=1; iStrip<=tPlane->GetStripsNu()*tPlane->GetStripsNv(); iStrip++) {

        if(R_pedestal[0] > tPlane->GetPedestal(iStrip) * calibFactor) R_pedestal[0] = tPlane->GetPedestal(iStrip) * calibFactor;
        if(R_pedestal[1] < tPlane->GetPedestal(iStrip) * calibFactor) R_pedestal[1] = tPlane->GetPedestal(iStrip) * calibFactor;

        if(R_noise[0] > tPlane->GetNoise(iStrip) * calibFactor) R_noise[0] = tPlane->GetNoise(iStrip) * calibFactor;
        if(R_noise[1] < tPlane->GetNoise(iStrip) * calibFactor) R_noise[1] = tPlane->GetNoise(iStrip) * calibFactor;
      }
    }

    double porcent,delta;
    porcent = 0.50;
    delta = R_pedestal[1] - R_pedestal[0];
    R_pedestal[0] -= delta*porcent;
    R_pedestal[1] += delta*porcent;
    double pedestal_max = TMath::Max(TMath::Abs(R_pedestal[0]),TMath::Abs(R_pedestal[1]));
    R_pedestal[0] = -pedestal_max;
    R_pedestal[1] = +pedestal_max;
    porcent = 0.50;
    delta = R_noise[1] - R_noise[0];
    R_noise[0] =  0.0;
//    R_noise[1] += delta*porcent; // does not always work

    printf(" Limits for distributions of plane %d:\n", iPlane);

    sprintf( name, "hPedDistripl%d", iPlane);
    sprintf( title, "Pedestal distribution of plane %d", iPlane);
    if ( maxAxisPed>0 ) {
      printf("   pedestal: %.0f to %.0f\n", -maxAxisPed, maxAxisPed);
      hPedDistri[iPlane-1] = new TH1F(name, title, 100, -maxAxisPed, maxAxisPed);
    }
    else {
      printf("   pedestal: %.0f to %.0f\n", R_pedestal[0], R_pedestal[1]);
//      hPedDistri[iPlane-1] = new TH1F(name, title, 100, TMath::Min( minPed, -50.), TMath::Max( maxPed, 50.));
      hPedDistri[iPlane-1] = new TH1F(name, title, 100, R_pedestal[0], R_pedestal[1]);
    }
      hPedDistri[iPlane-1]->SetXTitle(units);
    sprintf( name, "hNoiseDistripl%d", iPlane);
    sprintf( title, "Noise distribution of plane %d", iPlane);
    if ( maxAxisNoise>0 ) {
      printf("   nois: %.0f to %.0f\n", 0., maxAxisNoise);
      hNoiseDistri[iPlane-1] = new TH1F(name, title, 100, 0., maxAxisNoise);
    }
    else {
      printf("   nois: %.0f to %.0f\n", R_noise[0], R_noise[1]);
//      hNoiseDistri[iPlane-1] = new TH1F(name, title, 100, 0., TMath::Max( maxNoise, 50.));
      hNoiseDistri[iPlane-1] = new TH1F(name, title, 100, R_noise[0],R_noise[1]);
    }
    hNoiseDistri[iPlane-1]->SetXTitle(units);

    if( tMode[iPlane-1]==1 && tReadout[iPlane-1]>0 ) { // STRIPS
      for( Int_t iStrip=1; iStrip<=tPlane->GetStripsNu(); iStrip++) {
        hPedDistri[iPlane-1]->Fill( tPlane->GetPedestal(iStrip) * calibFactor);
        hNoiseDistri[iPlane-1]->Fill( tPlane->GetNoise(iStrip) * calibFactor);
      }
    }
    else if ( (tMode[iPlane-1]==2 || tPlane->GetDigitalThresholdNb()>0) && tReadout[iPlane-1]>0 ) { // PIXELS with analog output
      for( Int_t iStrip=1; iStrip<=tPlane->GetStripsNu()*tPlane->GetStripsNv(); iStrip++) {
        hPedDistri[iPlane-1]->Fill( tPlane->GetPedestal(iStrip) * calibFactor);
        hNoiseDistri[iPlane-1]->Fill( tPlane->GetNoise(iStrip) * calibFactor);
      }
    }

    cdistri->cd(2*iPlane-1);
    hPedDistri[iPlane-1]->Draw();
    cdistri->cd(2*iPlane);
    hNoiseDistri[iPlane-1]->Draw();

  } //end loop on planes
  cdistri->Update();


  // For autocorrelation computation and pixel distribution

  TCanvas *cautocor = NULL;
  TCanvas *cpixels = NULL, *cpixels2 = NULL;
  TLatex *t = new TLatex();
  //t->SetTextColor(2);
  Char_t string[200];

  if( thePlaneNumber>0 ) { // if a plane specified
    sprintf(canvasTitle, "Display Autocorrelation and pixel distrib. for plane %d and run %d", thePlaneNumber, fSession->GetRunNumber());

    // autocorrelation
    cautocor = new TCanvas("cautocor", canvasTitle, 45, 0, 800, 700);
    cautocor->UseCurrentStyle();
    cautocor->Divide( 2, 1);

    sprintf( name, "hAutocorDistripl%d", thePlaneNumber);
    sprintf( title, "Autocorrelation distribution of plane %d", thePlaneNumber);
    hAutocorDistri[thePlaneNumber-1] = new TH1F(name, title, 100, 0., 0.);

    for( Int_t iStripU=1; iStripU<=h2Autocor[thePlaneNumber-1]->GetNbinsX(); iStripU++) {
      for( Int_t iStripV=1; iStripV<=h2Autocor[thePlaneNumber-1]->GetNbinsY(); iStripV++) {
        st = iStripU+(iStripV-1)*thePlane->GetStripsNu();
        aNoise    = thePlane->GetNoise( st);
        aPedestal = thePlane->GetPedestal(st);
        autocorr  = h2Autocor[thePlaneNumber-1]->GetBinContent( iStripU, iStripV);
        autocorr  = (autocorr / (events2Read-1) - aPedestal*aPedestal);
        if(fVerbose) printf("MRaw::DisplayAutocorr plane %d, strips( %d, %d) channels %d, noise %f pedestal %f autocor (%f) %f\n", thePlaneNumber, iStripU, iStripV, st, aNoise, aPedestal, h2Autocor[thePlaneNumber-1]->GetBinContent( iStripU, iStripV), autocorr);
        h2Autocor[thePlaneNumber-1]->SetBinContent( iStripU, iStripV, autocorr);
        hAutocorDistri[thePlaneNumber-1]->Fill( autocorr);
      }
    }

    cautocor->cd(1);
    h2Autocor[thePlaneNumber-1]->Draw("colz");
    cautocor->cd(2);
    hAutocorDistri[thePlaneNumber-1]->Draw();


    // Pixel distribution
    cpixels = new TCanvas("cpixels", "Single pixel distributions", 60, 0, 800, 700);
    cpixels->UseCurrentStyle();
    cpixels->Divide( 5, 4);

    for (Int_t i=0; i<20; i++) {
      cpixels->cd(i+1);
      Int_t iStrip = (Int_t)(gRandom->Uniform(0,thePlane->GetStripsN()));
      hPixelDistri[iStrip]->Draw();
      iCol = iStrip%thePlane->GetStripsNu() + 1;
      iRow = iStrip/thePlane->GetStripsNu() + 1;
      sprintf( string, "autocorr = %.1f", h2Autocor[thePlaneNumber-1]->GetBinContent( iCol, iRow));
      t->DrawLatex( (hPixelDistri[iStrip]->GetXaxis()->GetXmax())*.5, (hPixelDistri[iStrip]->GetMaximum())*.5, string);
    }
    cpixels->Update();

    cpixels2 = new TCanvas("cpixels2", "Single pixel distributions overlayed", 75, 0, 800, 700);
    cpixels2->UseCurrentStyle();
    cpixels2->cd();

    hPixelDistri[0]->Draw();
    for (Int_t iStrip=1; iStrip<thePlane->GetStripsN(); iStrip++) {
      hPixelDistri[iStrip]->Draw("same");
    }
    cpixels2->Update();

  } // end if a plane specified


  // Save canvas and histos
  //cd to result dir
  cout << "Save canvas and histos" << endl;
  Char_t rootFile[300];
  sprintf(rootFile,"%sNoise_run%d.root",fSession->GetResultDirName().Data(),fSession->GetRunNumber());
  sprintf(rootFile,"%s", fTool.LocalizeDirName( rootFile)); // JB 2011/07/07
  cout << "\n-- Saving histos and canvas into " << rootFile << endl;
  TFile fRoot(rootFile,"RECREATE");
  cnoise->Write();
  cpedestal->Write();
  cdistri->Write();
  if( thePlaneNumber>0 ) {
    cautocor->Write();
    cpixels->Write();
    cpixels2->Write();
  }
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    hPedDistri[iPlane-1]->Write();
    hNoiseDistri[iPlane-1]->Write();
    if( thePlaneNumber>0 )  {
      hAutocorDistri[thePlaneNumber-1]->Write();
      h2Autocor[thePlaneNumber-1]->Write();
      for (Int_t iStrip=1; iStrip<thePlane->GetStripsN(); iStrip++) {
        hPixelDistri[iStrip]->Write();
      }
    }
    if( tMode[iPlane-1]==1 ) { // STRIPS
      h1Pedestal[iPlane-1]->Write();
      h1Noise[iPlane-1]->Write();
    }
    else if ( tMode[iPlane-1]==2 || tPlane->GetDigitalThresholdNb()>0 ) { // PIXELS
      h2Pedestal[iPlane-1]->Write();
      h2Noise[iPlane-1]->Write();
    }
  }
  fRoot.Close();

}

//______________________________________________________________________________
//
void MRaw::DisplayCumulatedClusters( Int_t planeNumber, Int_t nEvents)
{
  // Display the cluster characteristics for a given plane cumulated over the requested number of events
  // Call by gTAF->GetRaw()->DisplayCumulatedClusters()
  //
  // JB, 2011/02/09
  // Modified: JB, 2013/11/08 use of GetSNseed()

  fSession->SetEvents(nEvents);

  DTracker *tTracker  =  fSession->GetTracker();
  DPlane* tPlane= tTracker->GetPlane( planeNumber);

  Int_t nHitsReconstructed = 0;

  TCanvas *ccmumulcluster;
  Char_t name[50], title[100];
  sprintf( name, "ccmumulcluster%d", planeNumber);
  sprintf( title, "Cumulate clusters for plane %d", planeNumber);
  TObject* g = gROOT->FindObject(name) ;
  if (g) {
    ccmumulcluster = (TCanvas*)g;
  }
  else {
    ccmumulcluster = new TCanvas(name, title, 5, 5,800,700);
  }
  ccmumulcluster->Clear();
  ccmumulcluster->UseCurrentStyle();
  TPaveLabel* label = new TPaveLabel();
  Char_t canvasTitle[200];
  sprintf(canvasTitle, "Run %d - Plane %d - Event %d", fSession->GetRunNumber(), planeNumber, 0);
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad = new TPad("pad","",0.,0.,1.,0.965);
  pad->Draw();
  pad->Divide( 3, 3);

  TH1F *hClusterQ = new TH1F( "hclusterq", "Cluster total charge", 100, 0, 500);
  hClusterQ->SetFillColor(41);
  hClusterQ->SetXTitle("charge (ADC units)");
  TH1F *hClusterN = new TH1F( "hclustern", "Cluster noise", 100, 0, 50);
  hClusterN->SetFillColor(41);
  hClusterN->SetXTitle("charge (ADC units)");
  TH1F *hNeighbourSN = new TH1F( "hneighboursn", "Neighbours (wo seed) SNR", 100, 0, 100);
  hNeighbourSN->SetFillColor(41);
  hNeighbourSN->SetXTitle("signal / noise");
  TH1F *hSeedQ = new TH1F( "hseedq", "Seed charge", 100, 0, 500);
  hSeedQ->SetFillColor(45);
  hSeedQ->SetXTitle("charge (ADC units)");
  TH1F *hSeedN = new TH1F( "hseedn", "Seed noise", 100, 0, 30);
  hSeedN->SetFillColor(45);
  hSeedN->SetXTitle("charge (ADC units)");
  TH1F *hSeedSN = new TH1F( "hseedsn", "Seed SNR", 100, 0, 100);
  hSeedSN->SetFillColor(45);
  hSeedSN->SetXTitle("signal / noise");
  TH1F *hnofPixels = new TH1F( "hnofpixels", "Number of pixels", 50, -.5, 49.5);
  hnofPixels->SetFillColor(41);
  hnofPixels->SetXTitle("# pixels");
  TH2F *h2NeighbourSeed = new TH2F( "h2neighbourseed", "Neighbours (wo seed) SNR vs Seed SNR", 100, 0, 100, 100, 0, 100);
  h2NeighbourSeed->SetXTitle("seed signal / noise");
  h2NeighbourSeed->SetYTitle("neighbours signal / noise");

  //Loop over the requested number of events
  for( Int_t iEvt=0; iEvt < nEvents; iEvt++) {
    if( !(fSession->NextRawEvent()) ) break;
    tTracker->Update();

    sprintf(canvasTitle, "Run %d - Plane %d - Event %d", fSession->GetRunNumber(), planeNumber, nEvents);
    label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);

    DHit *aHit;
    Double_t seedN, seedSN;
    if( tPlane->GetHitsN()>0 ) {
      nHitsReconstructed += tPlane->GetHitsN();
      for( Int_t iHit=1; iHit<=tPlane->GetHitsN(); iHit++) { //loop on hits (starts at 1 !!)
        aHit = (DHit*)tPlane->GetHit( iHit);

        hClusterQ->Fill( aHit->GetClusterPulseSum());
        hClusterN->Fill( aHit->GetClusterNoiseAverage());
        hNeighbourSN->Fill( aHit->GetSNneighbour());
        hSeedQ->Fill( aHit->GetPulseHeight(0));
        seedN = aHit->GetNoise(0);
        hSeedN->Fill( seedN);
        seedSN = aHit->GetSNseed(); // JB, 2013/11/08
        // computation below useless since new GetSNseed() method
        //	if( seedN>1.e-5 ) {
        //	  seedSN = aHit->GetPulseHeight(0)/seedN;
        //	}
        //	else {
        //	  seedSN = 0.;
        //	}
        hSeedSN->Fill( seedSN);
        h2NeighbourSeed->Fill( seedSN, aHit->GetSNneighbour());
        hnofPixels->Fill( aHit->GetStripsInCluster());

        if(fVerbose) printf("MRaw::DisplayCumulClusters  pl %d, hit %d, Cluster q=%.1f n=%.1f, Neigbours SN=%.1f, Seed q=%.1f n=%.1f SN=%.1f, #pixels=%d\n", planeNumber, iHit, aHit->GetClusterPulseSum(), aHit->GetClusterNoiseAverage(), aHit->GetSNneighbour(), aHit->GetPulseHeight(0), seedN, seedSN, aHit->GetStripsInCluster());

      } //end loop on hits
    } //end if hits

    // Now display
    pad->cd(1);
    hClusterQ->DrawCopy("");
    pad->cd(2);
    hNeighbourSN->DrawCopy("");
    pad->cd(3);
    h2NeighbourSeed->DrawCopy("colz");
    pad->cd(4);
    hSeedQ->DrawCopy("");
    pad->cd(5);
    hSeedSN->DrawCopy("");
    pad->cd(7);
    hClusterN->DrawCopy("");
    pad->cd(8);
    hSeedN->DrawCopy("");
    pad->cd(9);
    hnofPixels->DrawCopy("");

    ccmumulcluster->Update();

  } // END LOOP ON EVENTS

  hClusterQ->Delete();
  hNeighbourSN->Delete();
  h2NeighbourSeed->Delete();
  hSeedQ->Delete();
  hSeedSN->Delete();
  hClusterN->Delete();
  hSeedN->Delete();
  hnofPixels->Delete();

}

//______________________________________________________________________________
//
void MRaw::SkipEvent( Int_t nEventsToSkip)
{

  // Simply skip the required number of events
  // skip = read them but don't analyse them
  // JB, 2008/10/09

  if(fDebugRaw) printf( "MRaw::SkipEvent start to skip %d events.\n", nEventsToSkip);

  fSession->SetEvents(nEventsToSkip);
  for( Int_t iEvent=0; iEvent<nEventsToSkip; iEvent++ ) {
    if( !(fSession->NextRawEvent()) ) break; // JB 2009/06/26
  }

}

//______________________________________________________________________________
//
void MRaw::DisplaySpectrum( Int_t nEvents, Int_t minSN, Int_t minValue, Int_t maxValue )
{
  // Display the signal distribution of fired pixels
  // minSN could be used to select pixels, need to be uncommented in code
  // histogram range = minValue, maxValue
  //
  // 25/05/2009 YV
  // Modified JB 2015/05/25 to add timestamp histo

  fSession->SetEvents(nEvents);

  DTracker *tTracker  =  fSession->GetTracker();
  Int_t nPlanes = tTracker->GetPlanesN();

  DPlane* tPlane;
  TH1F **spectrum = new TH1F*[nPlanes];
  TH1F **time = new TH1F*[nPlanes];
  Char_t name[50], title[100];
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    tPlane = tTracker->GetPlane(iPlane);
    sprintf( name, "spectrum_pl%d", iPlane);
    sprintf( title, "Spectrum of plane %d", iPlane);
    //spectrum[iPlane-1] = new TH1F(name, title, tPlane->GetStripsN(), 0, tPlane->GetStripsN()-1);
    spectrum[iPlane-1] = new TH1F(name, title, TMath::Min(maxValue-minValue,500), minValue, maxValue);
    spectrum[iPlane-1]->SetMarkerStyle(20);
    spectrum[iPlane-1]->SetMarkerSize(.4);
    spectrum[iPlane-1]->SetMarkerColor(1);
    spectrum[iPlane-1]->SetStats(kTRUE);

    sprintf( name, "time_pl%d", iPlane);
    sprintf( title, "Time within event of plane %d", iPlane);
    time[iPlane-1] = new TH1F(name, title, 100, 0, 0);
    time[iPlane-1]->SetMarkerStyle(20);
    time[iPlane-1]->SetMarkerSize(.4);
    time[iPlane-1]->SetMarkerColor(1);
    time[iPlane-1]->SetStats(kTRUE);
}

  //Loop over the requested number of events
  for( Int_t iEvt=0; iEvt < nEvents; iEvt++) {
    fSession->NextRawEvent();
    tTracker->Update();

    //cout << " checking event " << iEvt << endl ;

    for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
      tPlane = tTracker->GetPlane(iPlane);

      //cout << " checking plane " << iPlane << " with readout " << tPlane->GetReadout() << " and analysis " << tPlane->GetAnalysisMode() << endl ;

      if ( (tPlane->GetReadout()>=3 || tPlane->GetReadout()==24 || tPlane->GetReadout()==25) /*&& tPlane->GetAnalysisMode()<3*/ ){

       std::vector<DPixel*> *aList = tPlane->GetListOfPixels();
        //cout << " number of fired pixels = " << aList->size() << endl ;
        for( Int_t iPix=0; iPix<(Int_t)aList->size(); iPix++) {
          //cout << "   pixels " << iPix << "  pulse = " << aList->at(iPix)->GetPulseHeight() << " time = " << aList->at(iPix)->GetTimestamp() << endl ;
          //if(aList->at(iPix)->GetPulseHeight()>=minSN){
            //spectrum[iPlane-1]->SetBinContent( aList->at(iPix)->GetPixelIndex(), aList->at(iPix)->GetPulseHeight());
            spectrum[iPlane-1]->Fill(aList->at(iPix)->GetPulseHeight());
          //}
          time[iPlane-1]->Fill(aList->at(iPix)->GetTimestamp()); // JB 2015/05/25
        } //end loop on fired pixels

      } //end of sparsified readout

    } //end loop on planes

  Int_t frequency = 1;
  Int_t eventCount = fSession->GetCurrentEventNumber()-1;
  if( eventCount > 100000) frequency = 10000;
  else if( eventCount > 10000) frequency = 5000;
  else if( eventCount > 100) frequency = 500;
  else if( eventCount > 10) frequency = 50;
  if( eventCount/frequency*frequency == eventCount) {
    cout << "\n Event number : " << eventCount <<" ";
  }

  }//end loop on evts


  // Display of spectrum

  TCanvas *cdisplayspectrum = new TCanvas("cdisplayspectrum", "spectrum",800,700);
  cdisplayspectrum->UseCurrentStyle();
  TPaveLabel* label = new TPaveLabel();
  Char_t canvasTitle[200];
  sprintf(canvasTitle, "Run %d over %d events", fSession->GetRunNumber(), nEvents);
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad = new TPad("pad","",0.,0.,1.,0.965);
  pad->Draw();
  pad->Divide( (Int_t)ceil(nPlanes/2.), 2);
  pad->SetLogy();

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    pad->cd(iPlane);
    if( tTracker->GetPlane(iPlane)->GetAnalysisMode()<3 ) {
      spectrum[iPlane-1]->DrawCopy();
      //spectrum[iPlane-1]->Fit("gaus","q");
      //spectrum[iPlane-1]->Delete();
    }
  }
  cdisplayspectrum->Update();


  // Display of timestamp

  TCanvas *cdisplaytime = new TCanvas("cdisplaytime", "timestamp",5,5,800,700);
  cdisplaytime->UseCurrentStyle();
//  TPaveLabel* label = new TPaveLabel();
//  Char_t canvasTitle[200];
  sprintf(canvasTitle, "Run %d over %d events", fSession->GetRunNumber(), nEvents);
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad1 = new TPad("pad1","",0.,0.,1.,0.965);
  pad1->Draw();
  pad1->Divide( (Int_t)ceil(nPlanes/2.), 2);
//  pad1->SetLogy();

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    pad1->cd(iPlane);
    if( time[iPlane-1]->GetEntries() ) time[iPlane-1]->DrawCopy();
  }
  cdisplaytime->Update();

}

//______________________________________________________________________________
//
void MRaw::LaserStudy( Int_t Xpixel, Int_t Ypixel, Int_t nPixels, Int_t nEvents, Int_t cycleLength, Int_t nSpots)
{
  // Study efficiency of pixel when illuminated by laser,
  // Xpixel and Ypixel define the pixel to look at.
  // nPixels define the area for the laser plot drawing.
  //
  // Look in a given area of 3x3 pixels centered on Xpixel, Ypixel,
  //  if more than minNbFiredPix pixels are fired the spot is detected.
  //
  // cycleLength is the laser cycle duration expressed in number of frames
  // nSpots is the expected number of laser spots within a single cycle
  //
  // Some parameters are hard coded:
  //  plane investigated (only one), histos binning
  //
  // JB+MaxBarbier 2011/02/25
  // Modified: JB 2011/07/25 some bug fix (row-col swap)
  // Modified: JB 2011/09/16 expected cycleLenght added as input & protection against nbspot>10

  Int_t debug = 0;
  Int_t thePlaneNumber = 1;
  Int_t minNbFiredPix = 4; //
  Float_t minPulsehieght = 0.;
  Int_t maxEmptyframes = 2*cycleLength;
  Float_t maxSpectrum  = 3.; // M18: 10000, M26: 3
  fSession->SetEvents(nEvents);

  // Limit the size to investigate
  if( nPixels>100 ) {
    printf( "WARNING you required a too large area %d pixels, set to 100 pixels", nPixels);
    nPixels = 100;
  }

  TCanvas *claser = new TCanvas("claser", "Laser study",800,700);
  claser->UseCurrentStyle();
  TPaveLabel* label = new TPaveLabel();
  Char_t canvasTitle[200];
  sprintf(canvasTitle, "Run %d over %d events", fSession->GetRunNumber(), nEvents);
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad = new TPad("pad","",0.,0.,1.,0.965);
  pad->Draw();

  DTracker *tTracker  =  fSession->GetTracker();
  DPlane* tPlane = tTracker->GetPlane(thePlaneNumber);

  pad->Divide( 2, 2);

  TH2F *hSpotMap = new TH2F( "hspotmap", "Spot map", 2*nPixels+1, Xpixel-nPixels, Xpixel+nPixels, 2*nPixels+1, Ypixel-nPixels, Ypixel+nPixels);
  hSpotMap->SetXTitle("# pixel in X");
  hSpotMap->SetYTitle("# pixel in Y");
  TH1F *hSpotX = new TH1F( "hspotx", "Spot projection in X", 2*nPixels+1, Xpixel-nPixels, Xpixel+nPixels);
  hSpotX->SetXTitle("# pixel in X");
  TH1F *hSpotY = new TH1F( "hspotY", "Spot projection in Y", 2*nPixels+1, Ypixel-nPixels, Ypixel+nPixels);
  hSpotY->SetXTitle("# pixel in y");
  TH1F *hEmptyIntervals = new TH1F( "hemptyintervals", "Empty frames between two hits", maxEmptyframes, -0.5, maxEmptyframes-0.5);
  TH1F *hSpectrum = new TH1F( "hspectrum", "Signal distribution on pixel", TMath::Min(Int_t(maxSpectrum),500), 0., maxSpectrum);
  TH2F *hSpectrum_EmptyIntervals = new TH2F( "hSpectrum_EmptyIntervals", "hSpectrum_EmptyIntervals", TMath::Min(Int_t(maxSpectrum),500), 0., maxSpectrum, maxEmptyframes, -0.5, maxEmptyframes-0.5);

  TH1F **hSpotSpectrum = new TH1F*[nSpots];
  Char_t name[50], title[100];
  for( Int_t iSpot=1; iSpot<=nSpots; iSpot++) {
    sprintf( name, "specspot%d", iSpot);
    sprintf( title, "Spectrum of spot %d", iSpot);
    hSpotSpectrum[iSpot-1] = new TH1F(name, title, TMath::Min(Int_t(maxSpectrum),500), 0., maxSpectrum);
    hSpotSpectrum[iSpot-1]->SetLineColor(iSpot);
  }

  // Flag to start and stop
  Bool_t start = kFALSE;
  Bool_t stop = kFALSE;
  // Counters
  Int_t nFrames = 0;
  Int_t nbEmptyFrames = 0;
  Int_t nbFiredFrames = 0;
  Int_t nbEmptyIntervals = 0;
  Int_t nbFramesSinceSpot1 = -1;
  Int_t spotNb = 0;

 std::vector<DPixel*> *aList;
  DPixel *tpix;
  Int_t iCol, iRow; // start at 0, so for Setting Bin, use iCol/iRow +1
  Double_t pulseheight = 0;
  Int_t pixelFound = 0;
  Bool_t spotFound = kFALSE;

  //Loop over the requested number of events
  Int_t iEvt = 0;
  while( !stop && iEvt<nEvents ) {
    //for( Int_t iEvt=0; iEvt < nEvents; iEvt++) {
    if( !fSession->NextRawEvent() ) break; // Stop when no more events to read, JB 2011/07/25
    tTracker->Update();
    if( debug>1) printf( "EVENT %d over %d\n", iEvt, nEvents);

    // init for each event
    aList = tPlane->GetListOfPixels();
    pixelFound = 0;
    spotFound = kFALSE;

    for( Int_t iPix=0; iPix<(Int_t)aList->size(); iPix++) { // loop over pixels
      tpix = aList->at(iPix);
      // Fill the histograms for the spot
      iCol = tpix->GetPixelColumn();
      iRow = tpix->GetPixelLine();
      if( abs(iCol-Xpixel)<=nPixels && abs(iRow-Ypixel)<=nPixels ) {
	hSpotMap->Fill( iCol+1, iRow+1, tpix->GetPulseHeight());
	hSpotX->Fill( iCol+1, tpix->GetPulseHeight());
	hSpotY->Fill( iRow+1, tpix->GetPulseHeight());
      }
      // Test if the pixel selected is fired
      //  or if a direct neighbour is fired
      // JB 2011/07/25
      if( debug>2) printf( "  testing pix (%4d, %4d) / (%4d, %4d), value=%f\n", tpix->GetPixelColumn(), tpix->GetPixelLine(), Xpixel, Ypixel, tpix->GetPulseHeight());
      if(   iCol == Xpixel && iRow == Ypixel
	 && tpix->GetPulseHeight()>=minPulsehieght ){
	spotFound = kTRUE;
	pulseheight = tpix->GetPulseHeight();
      }
      else if(   abs(iCol-Xpixel)<2 && abs(iRow-Ypixel)<2
	      && tpix->GetPulseHeight()>=minPulsehieght ){
	pixelFound++;
      }
    } //end loop on fired pixels

    // If the spot was found
    if( spotFound && pixelFound>=minNbFiredPix ) {
      if( debug) printf( "  SPOT  evt %d, empty %d, fired %d, empty since last fired %d, all since 1st spot %d\n", iEvt, nbEmptyFrames, nbFiredFrames, nbEmptyIntervals, nbFramesSinceSpot1);

      // start the counters on frame with a laser spot
      if( !start ) {
	printf("STARTING counters at frame %d\n", iEvt);
	start = kTRUE;
	nFrames = 0;
	nbEmptyFrames = 0;
	nbFiredFrames = -1;
	nbEmptyIntervals = 0;
	nbFramesSinceSpot1 = -1;
	spotNb = 0;
      }
      // stop the counters on frame with a laser spot
      // 50 is larger thant the spots frequency expressed in frames
      else if( iEvt>nEvents-2*cycleLength ) {
	printf("STOPING counters at frame %d\n", iEvt);
	stop = kTRUE;
      }

      nbFiredFrames++;
      if( nbFramesSinceSpot1>cycleLength-2 ) { // condition for new burst
	spotNb = 0;
	nbFramesSinceSpot1 = -1;
      }
      spotNb++;
      if( debug>1) printf( "  spot %d found, %d frames since 1st spot: filling histos\n", spotNb, nbFramesSinceSpot1);
      hSpectrum->Fill( pulseheight);
      if( spotNb<=nSpots) {
	hSpotSpectrum[spotNb-1]->Fill( pulseheight);
      }
      else {
	printf( "WARNING: you reached a spot number %d higher than the maximum allowed (%d).\n", spotNb, nSpots);
	printf( "    #spots has been reset, please tune the values cycleLength=%d and nSpots=%d to avoid this.\n", cycleLength, nSpots);
      }
      hEmptyIntervals->Fill( nbEmptyIntervals);
      hSpectrum_EmptyIntervals->Fill(pulseheight,nbEmptyIntervals);
      nbEmptyIntervals = 0;
    }
    // end If the spot was found

    // If the spot was not found
    else {
      nbEmptyFrames++;
      nbEmptyIntervals++;
      if( debug>1) printf( "  NO SPOT: empty %d, fired %d, empty since last fired %d\n", nbEmptyFrames, nbFiredFrames, nbEmptyIntervals);
    }

    nbFramesSinceSpot1++;
    nFrames++;
    iEvt++;
  }//end loop on evts


  fSession->GetDataAcquisition()->PrintStatistics();
  tTracker->PrintStatistics();

  printf("\n");
  printf( "Nb of frames read: %d\n", nFrames);
  printf( "Pixel scrutinize (line=%d, column=%d\n", Xpixel, Ypixel);
  printf( "Nb of frames fired: %d\n", nbFiredFrames);
  printf( "Nb of frames empty: %d\n", nbEmptyFrames);
  printf( "Nb of estimated cycles (of size %d frames): %d\n", cycleLength, (Int_t)hEmptyIntervals->Integral(10,50));
  printf( "Mean charge for 1st spot = %.1f", hSpotSpectrum[0]->GetMean());
  if( nSpots>0 ) {
    printf( ", for 2nd spot = %.2f", hSpotSpectrum[1]->GetMean());
    printf( ", ratio = %.3f", hSpotSpectrum[1]->GetMean()/hSpotSpectrum[0]->GetMean());
  }
  printf("\n\n");

  pad->cd(1);
  hEmptyIntervals->Draw();
  gPad->SetGrid(1,1);
  pad->cd(2);
  hSpectrum->Draw();
  pad->cd(3);
  hSpectrum_EmptyIntervals->Draw("colz");
  gPad->SetGrid(1,1);
  pad->cd(4);
  hSpotSpectrum[0]->Draw();
  for( Int_t iSpot=2; iSpot<=nSpots; iSpot++) {
    hSpotSpectrum[iSpot-1]->Draw("same");
  }

  //pad->SetLogy();

  claser->Update();

  sprintf(canvasTitle, "Laser spot: run %d over %d events", fSession->GetRunNumber(), nEvents);
  TCanvas *claserspot = new TCanvas("claserspot", canvasTitle,800,700);
  claserspot->UseCurrentStyle();
  claserspot->Divide(2,2);
  claserspot->cd(1);
  hSpotMap->Draw("lego2");
  claserspot->cd(2);
  hSpotX->Draw();
  claserspot->cd(3);
  hSpotY->Draw();
  claserspot->cd(4);
  hSpotMap->Draw("colz");


   sprintf( title, "laserFile_c%d_l%d.root", Xpixel, Ypixel);
   TString filename = fSession->GetResultDirName() + title;
   fTool.LocalizeDirName( &filename); // JB 2011/07/07
   TFile outLaserFile( filename.Data(), "recreate");
   claser->Write();
   hEmptyIntervals->Write();
   hSpectrum_EmptyIntervals->Write();
   hSpectrum->Write();
   for( Int_t iSpot=1; iSpot<=nSpots; iSpot++) {
   hSpotSpectrum[iSpot-1]->Write();
   }
  claserspot->Write();
  hSpotMap->Write();
  hSpotX->Write();
  hSpotY->Write();
   outLaserFile.Close();

}

//______________________________________________________________________________
//
void MRaw::DisplayGeometry()
{
  // Display the geometry defined in the config file with alignment.
  //
  // JB 2011/11/01

  //-- Useful variables

  Int_t nPlanes = fSession->GetTracker()->GetPlanesN();

  DPlane* tPlane;
  DPrecAlign* tAlign;
  DR3 posInPlane, posInTracker;
  TBox **boxXY = new TBox*[nPlanes];
  TLine **lineXZ = new TLine*[nPlanes];
  TLine **lineYZ = new TLine*[nPlanes];
  TText* number = new TText();
  TArrow* direction = new TArrow();
  Char_t tnum[20];
  Double_t bottomCorner[nPlanes][3];
  Double_t topCorner[nPlanes][3];
  Double_t limit[3][2] = { {0.,0.}, {0.,0.}, {0.,0.} }; // (x,y,z) (min,max)
  Double_t cumulatedXwidth = 0.;
  Color_t *color = new Color_t[nPlanes];


  //-- Read plane positions

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    tPlane = fSession->GetTracker()->GetPlane(iPlane);
    printf("\n================================\nGenerating geometry for plane %d, %s:\n", iPlane, tPlane->GetPlaneName());
    color[iPlane-1] = (iPlane==10)?49:iPlane;
    tAlign = (DPrecAlign*)tPlane->GetPrecAlignment();
    tAlign->PrintAll();

    // -- box for the XY transverse view
    // bottom left corner
    posInPlane.SetValue( -tPlane->GetStripsNu() * tPlane->GetStripPitch()(0) / 2.
			,-tPlane->GetStripsNv() * tPlane->GetStripPitch()(1) / 2.
			,0.);
    posInTracker = tAlign->TransformHitToTracker( posInPlane);
    bottomCorner[iPlane-1][0] = posInTracker(0);
    bottomCorner[iPlane-1][1] = posInTracker(1);
    bottomCorner[iPlane-1][2] = posInTracker(2);
    // top right corner
    posInPlane.SetValue( +tPlane->GetStripsNu() * tPlane->GetStripPitch()(0) / 2.
			,+tPlane->GetStripsNv() * tPlane->GetStripPitch()(1) / 2.
			,0.);
    posInTracker = tAlign->TransformHitToTracker( posInPlane);
    topCorner[iPlane-1][0] = posInTracker(0);
    topCorner[iPlane-1][1] = posInTracker(1);
    topCorner[iPlane-1][2] = posInTracker(2);
    printf( " corners: bottom (%.1f, %.1f, %.1f), top (%.1f, %.1f, %.1f)\n", bottomCorner[iPlane-1][0], bottomCorner[iPlane-1][1], bottomCorner[iPlane-1][2], topCorner[iPlane-1][0], topCorner[iPlane-1][1], topCorner[iPlane-1][2]);
    boxXY[iPlane-1] = new TBox( bottomCorner[iPlane-1][0], bottomCorner[iPlane-1][1], topCorner[iPlane-1][0], topCorner[iPlane-1][1]);
    boxXY[iPlane-1]->SetFillStyle(1001);
    boxXY[iPlane-1]->SetFillColor(color[iPlane-1]);

    // -- line for the XZ top view
    lineXZ[iPlane-1] = new TLine();
    lineXZ[iPlane-1]->SetLineWidth(3);
    lineXZ[iPlane-1]->SetLineColor(color[iPlane-1]);
    // middle left corner
    posInPlane.SetValue( -tPlane->GetStripsNu() * tPlane->GetStripPitch()(0) / 2.
			,0.
			,0.);
    posInTracker = tAlign->TransformHitToTracker( posInPlane);
    lineXZ[iPlane-1]->SetX1( posInTracker(2));
    lineXZ[iPlane-1]->SetY1( posInTracker(0));
    // middle right corner
    posInPlane.SetValue( +tPlane->GetStripsNu() * tPlane->GetStripPitch()(0) / 2.
			,0.
			,0.);
    posInTracker = tAlign->TransformHitToTracker( posInPlane);
    lineXZ[iPlane-1]->SetX2( posInTracker(2));
    lineXZ[iPlane-1]->SetY2( posInTracker(0));
    printf( " XZ-line: left (%.1f, %.1f), right (%.1f, %.1f)\n", lineXZ[iPlane-1]->GetY1(), lineXZ[iPlane-1]->GetX1(), lineXZ[iPlane-1]->GetY2(), lineXZ[iPlane-1]->GetX2() );

    // -- line for the YZ top view
    lineYZ[iPlane-1] = new TLine();
    lineYZ[iPlane-1]->SetLineWidth(3);
    lineYZ[iPlane-1]->SetLineColor(color[iPlane-1]);
    // bottom middle corner
    posInPlane.SetValue( 0.
			,-tPlane->GetStripsNv() * tPlane->GetStripPitch()(1) / 2.
			,0.);
    posInTracker = tAlign->TransformHitToTracker( posInPlane);
    lineYZ[iPlane-1]->SetX1( posInTracker(2));
    lineYZ[iPlane-1]->SetY1( posInTracker(1));
    // top middle corner
    posInPlane.SetValue( 0.
			,+tPlane->GetStripsNv() * tPlane->GetStripPitch()(1) / 2.
			,0.);
    posInTracker = tAlign->TransformHitToTracker( posInPlane);
    lineYZ[iPlane-1]->SetX2( posInTracker(2));
    lineYZ[iPlane-1]->SetY2( posInTracker(1));
    printf( " YZ-line: left (%.1f, %.1f), right (%.1f, %.1f)\n", lineYZ[iPlane-1]->GetY1(), lineYZ[iPlane-1]->GetX1(), lineYZ[iPlane-1]->GetY2(), lineYZ[iPlane-1]->GetX2() );

    // update limits
    if( bottomCorner[iPlane-1][0] < limit[0][0] ) limit[0][0] = bottomCorner[iPlane-1][0];
    if( bottomCorner[iPlane-1][1] < limit[1][0] ) limit[1][0] = bottomCorner[iPlane-1][1];
    if( bottomCorner[iPlane-1][2] < limit[2][0] ) limit[2][0] = bottomCorner[iPlane-1][2];
    if( limit[0][1] < topCorner[iPlane-1][0] ) limit[0][1] = topCorner[iPlane-1][0];
    if( limit[1][1] < topCorner[iPlane-1][1] ) limit[1][1] = topCorner[iPlane-1][1];
    if( limit[2][1] < topCorner[iPlane-1][2] ) limit[2][1] = topCorner[iPlane-1][2];
    if( topCorner[iPlane-1][0] < limit[0][0] ) limit[0][0] = topCorner[iPlane-1][0];
    if( topCorner[iPlane-1][1] < limit[1][0] ) limit[1][0] = topCorner[iPlane-1][1];
    if( topCorner[iPlane-1][2] < limit[2][0] ) limit[2][0] = topCorner[iPlane-1][2];
    if( limit[0][1] < bottomCorner[iPlane-1][0] ) limit[0][1] = bottomCorner[iPlane-1][0];
    if( limit[1][1] < bottomCorner[iPlane-1][1] ) limit[1][1] = bottomCorner[iPlane-1][1];
    if( limit[2][1] < bottomCorner[iPlane-1][2] ) limit[2][1] = bottomCorner[iPlane-1][2];
    cumulatedXwidth += 1000 + abs(bottomCorner[iPlane-1][0]-topCorner[iPlane-1][0]);

  } // end loop on planes

  printf( "\n Found following limits (um) for display:\n  x: %.0f, %.0f\n  y: %.0f, %.0f\n  z: %.0f, %.0f\n", limit[0][0], limit[0][1], limit[1][0], limit[1][1], limit[2][0], limit[2][1]);

  //-- histograms for reference frame

  TH2F *hXY = new TH2F("hXY","Pseudo XY projection - TRANSVERSE VIEW", 20, boxXY[0]->GetX1(), boxXY[0]->GetX1()+cumulatedXwidth, 20, limit[1][0]-1000, limit[1][1]+1000);
  hXY->SetXTitle("X (#mum)");
  hXY->SetYTitle("Y (#mum)");
  hXY->SetTitleSize( 0.06, "XY");
  hXY->SetLabelSize( 0.06, "XY");
  hXY->SetStats(0);
  TH2F *hXZ = new TH2F("hXZ","X-Z projection - TOP VIEW", 20, limit[2][0]-10000, limit[2][1]+10000, 20, limit[0][0]-1000, limit[0][1]+1000);
  hXZ->SetXTitle("Z (#mum)");
  hXZ->SetYTitle("X (#mum)");
  hXZ->SetTitleSize( 0.06, "XY");
  hXZ->SetLabelSize( 0.06, "XY");
  hXZ->SetStats(0);
  TH2F *hYZ = new TH2F("hYZ","Y-Z projection - SIDE VIEW", 20, limit[2][0]-10000, limit[2][1]+10000, 20, limit[1][0]-1000, limit[1][1]+1000);
  hYZ->SetXTitle("Z (#mum)");
  hYZ->SetYTitle("Y (#mum)");
  hYZ->SetTitleSize( 0.06, "XY");
  hYZ->SetLabelSize( 0.06, "XY");
  hYZ->SetStats(0);


  //-- Draw

  gStyle->SetLabelSize(0.06);
  gStyle->SetTitleSize(0.06);

  TCanvas *cgeometry = new TCanvas( "cgeometry", "Telescope geometry", 5, 5,500*TMath::Min(1., fabs(limit[2][0]-limit[2][1])*2/fabs(limit[1][0]-limit[1][1]) ),500);

  cgeometry->cd();
  TPad* viewxy = new TPad("viewxy","view xy",0.01,0.51,0.99,0.99);
  viewxy->Draw();
  viewxy->cd();
  hXY->Draw();
  boxXY[0]->Draw();
  sprintf( tnum, "%d", 1);
  number->SetTextSize(.08);
  number->DrawText( boxXY[0]->GetX1()+100, boxXY[0]->GetY2()+100, tnum);
  direction->SetLineColor(0);
  direction->SetLineWidth(2);
  direction->DrawArrow( bottomCorner[0][0], bottomCorner[0][1], topCorner[0][0], topCorner[0][1], 0.05,">");

  cgeometry->cd();
  TPad* viewz = new TPad("viewz","view z",0.01,0.01,0.99,0.50);
  viewz->Draw();
  viewz->cd();
  viewz->Divide(2,1);
  viewz->cd(1);
  hYZ->Draw();
  gPad->SetGrid(1,1);
  viewz->cd(2);
  hXZ->Draw();
  gPad->SetGrid(1,1);

  printf("=====\n");
  Double_t shift=0.;
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    if( iPlane>1) {
      shift = 1000+boxXY[iPlane-2]->GetX2();
      boxXY[iPlane-1]->SetX2( boxXY[iPlane-1]->GetX2()-boxXY[iPlane-1]->GetX1()+shift);
      boxXY[iPlane-1]->SetX1( shift);
      viewxy->cd(); boxXY[iPlane-1]->Draw();
      sprintf( tnum, "%d", iPlane);
      number->SetTextColor( color[iPlane-1]);
      number->DrawText( boxXY[iPlane-1]->GetX1(), boxXY[iPlane-1]->GetY2()+100, tnum);
      direction->SetLineColor(1);
      direction->SetLineWidth(2);
      shift += (boxXY[iPlane-1]->GetX2()-boxXY[iPlane-1]->GetX1())/2.;
      printf(" plane %d: shift=%.0f, corners (%.0f, %.0f) (%.0f, %.0f), box (%.0f, %.0f) (%.0f, %.0f)\n", iPlane, shift
	     , bottomCorner[iPlane-1][0], bottomCorner[iPlane-1][1], topCorner[iPlane-1][0], topCorner[iPlane-1][1]
	     , boxXY[iPlane-1]->GetX1(), boxXY[iPlane-1]->GetY1(), boxXY[iPlane-1]->GetX2(), boxXY[iPlane-1]->GetY2());
      direction->DrawArrow( shift+bottomCorner[iPlane-1][0], bottomCorner[iPlane-1][1], shift+topCorner[iPlane-1][0], topCorner[iPlane-1][1], 0.05,">");
    }
    viewz->cd(1); lineYZ[iPlane-1]->Draw();
    viewz->cd(2); lineXZ[iPlane-1]->Draw();
  }

}

//______________________________________________________________________________
//
void MRaw::MakeLadderGeometry( Int_t *planeList, Int_t numberOfPlanes, Int_t aRefPlaneNumber)
{
  // Generate the geometry of a ladder relative to a reference plane,
  //   made of numberOfPlanes provided in planeList.
  // The center of the ladder is fixed by the center of the reference plane.
  // The relative (wrt the previous center) positions of each ladder is then computed.
  //
  // JB 2014/11/24
  if (numberOfPlanes<=0) {
    printf("MakeLadderGeometry: WARNING wrong number of planes: %d !\n", numberOfPlanes);
    return;
  }
  //---- Get pointers to each planes
  DTracker *tTracker  =  fSession->GetTracker();
  DPlane *refPlane = tTracker->GetPlane(aRefPlaneNumber);
  DPlane *tPlane[numberOfPlanes];
  std::cout << "MakeLadderGeometry: GetPlane" << '\n';
  for( Int_t iPlane=0; iPlane<numberOfPlanes; iPlane++) {
    tPlane[iPlane] = tTracker->GetPlane(planeList[iPlane]);
  }

  //---- Prepare output text file with ladder geometry
  Char_t fileName[300];
  sprintf( fileName, "%sladder_withPlanes", fSession->GetResultDirName().Data());
  for (Int_t iPlane=0; iPlane<numberOfPlanes; iPlane++) {
    sprintf( fileName, "%s%d_", fileName, planeList[iPlane]);
  }
  sprintf( fileName, "%srun%d.txt", fileName, fSession->GetRunNumber());
  sprintf(fileName,"%s", fTool.LocalizeDirName( fileName));
  FILE *outFile;
  outFile = fopen( fileName, "w");
  fprintf( outFile, "");
  //---- Information general to the ladder
  fprintf( outFile, "LadderID:     %d\n", refPlane->GetPlaneNumber());
  fprintf( outFile, "LadderName:   \"Custom\"\n");
  fprintf( outFile, "Status:      %d\n", tPlane[0]->GetStatus());
  fprintf( outFile, "LadderPositionX:    %.3f  Y: %.3f  Z: %.3f\n", (refPlane->GetPosition())(0)/1000., (refPlane->GetPosition())(1)/1000., (refPlane->GetPosition())(2)/1000.);
  fprintf( outFile, "LadderTiltZ:    %.3f  Y: %.3f  X: %.3f\n", TMath::RadToDeg()*(refPlane->GetTilt())(0), TMath::RadToDeg()*(refPlane->GetTilt())(1), TMath::RadToDeg()*(refPlane->GetTilt())(2));
  fprintf( outFile, "NbOfPlanes:     %d", numberOfPlanes);

  DPrecAlign *refAlignment = refPlane->GetPrecAlignment();
  printf("\n Alignment of reference plane %d:\n", refPlane->GetPlaneNumber());
  refAlignment->PrintAll();

  //---- Information specific for each plane
  DR3 relativePosition;
  DR3 relativeTilt;
  for (Int_t iPlane=0; iPlane<numberOfPlanes; iPlane++) {
    printf("\n For ladder element %d: plane %d:\n", iPlane, tPlane[iPlane]->GetPlaneNumber());

    Int_t DPrecAlignMethod = fSession->GetSetup()->GetTrackerPar().DPrecAlignMethod;   // LC 2015/01/31
    DPrecAlign planeAlignment(DPrecAlignMethod);

    planeAlignment.CopyAlignment( tPlane[iPlane]->GetPrecAlignment());
    printf("  --> Current absolute alignment:\n");
    planeAlignment.PrintAll();
    printf("  --> Relative alignment:\n");
    planeAlignment.DeconvoluteAlignment( refAlignment);
//    planeAlignment.PrintAll();
    fprintf( outFile, "// ladder plane %d out of %d\n", iPlane+1, numberOfPlanes);
    fprintf( outFile, "IncludedPlane:   %d\n", tPlane[iPlane]->GetPlaneNumber());
    relativePosition = refPlane->TrackerToPlane( tPlane[iPlane]->GetPosition());
//    fprintf( outFile, "PlaneShiftU %.3f  V: %.3f  W: %.3f\n", relativePosition(0), relativePosition(1), relativePosition(2));
    fprintf( outFile, "PlaneShiftU:    %.3f  V: %.3f  W: %.3f\n", (planeAlignment.GetTranslation())[0]/1000., (planeAlignment.GetTranslation())[1]/1000., (planeAlignment.GetTranslation())[2]/1000.);
    //planeAlignment.GetTranslation();
//    fprintf( outFile, "PlaneTiltU %.3f  V: %.3f  W: %.3f\n", relativeTilt(2), relativeTilt(1), relativeTilt(0));
    fprintf( outFile, "PlaneTiltU:    %.3f  V: %.3f  W: %.3f\n", TMath::RadToDeg()*(planeAlignment.GetRotations())[2], TMath::RadToDeg()*(planeAlignment.GetRotations())[1], TMath::RadToDeg()*(planeAlignment.GetRotations())[0]);
  }

  //---- End
  fclose( outFile);

  printf("The configuration is written in the following file: %s\n", fileName);

}
//______________________________________________________________________________
//
void MRaw::VertexStudy( Int_t nEvents, Double_t beamX, Double_t beamY, Double_t beamTilt)
{
  // Display the vertex positions computed for each track reconstructed.
  // The vertex is actually the point of closest approach between the track
  //  and the horizontal beam with origin (beamX, beamY) and tilt beamTilt.
  // Distances are in microns and angle in degrees.
  //
  // Call by gTAF->GetRaw()->VertexStudy( nEvents)
  //
  // JB, 2011/07/26

  // Geometry (um)
  Double_t minZ =   -50000;
  Double_t maxZ =  +120000;
  Double_t minX = -15000;
  Double_t maxX = +15000;
  Double_t minY = -15000;
  Double_t maxY = +15000;


  fSession->SetEvents(nEvents);

  DTracker *tTracker  =  fSession->GetTracker();
  Double_t zTrackMin = tTracker->GetPlane(1)->GetPosition()(2);
  DTrack *aTrack;
  Double_t trackPoint1[3], trackPoint2[3];
  Double_t linePoint1[3], linePoint2[3];
  Double_t *vertex;
  vertex = new Double_t(3);
  vertex[0] = vertex[1] = vertex[2] = 0.;

  linePoint1[0] = beamX;
  linePoint1[1] = beamY;
  linePoint1[2] = 0.;
  linePoint2[0] = beamX*cos( beamTilt*TMath::DegToRad())+zTrackMin*sin( beamTilt*TMath::DegToRad());
  linePoint2[1] = beamY;
  linePoint2[2] = zTrackMin;

  gStyle->SetTitleSize(0.06,"xyz");
  gStyle->SetLabelSize(0.06,"xyz");
  gStyle->SetLabelOffset(0.01,"xyz");

  TCanvas *cvertex;
  TObject* g = gROOT->FindObject("cvertex") ;
  if (g) {
    cvertex = (TCanvas*)g;
  }
  else {
    cvertex = new TCanvas("cvertex", "Cumulate tracks", 5, 5,1000,700);
  }
  cvertex->Clear();
  //cvertex->UseCurrentStyle();
  TPaveLabel* label = new TPaveLabel();
  Char_t canvasTitle[200];
  TPad *pad = new TPad("pad","",0.,0.,1.,0.965);
  pad->Draw();
  sprintf(canvasTitle, "Run %d, vertices over %d events", fSession->GetRunNumber(), nEvents);
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  pad->Divide( 3, 2);

  TH1F *hVertexX = new TH1F( "hvertexX", "Vertices projection on X (rot)", 500, minX, maxX);
  //hVertexX->SetStats(kFALSE);
  hVertexX->SetXTitle("X (#mum)");
  TH1F *hVertexY = new TH1F( "hvertexY", "Vertices projection on Y (rot)", 500, minY, maxY);
  //hVertexY->SetStats(kFALSE);
  hVertexY->SetXTitle("Y (#mum)");
  TH1F *hVertexZ = new TH1F( "hvertexZ", "Vertices projection on Z (rot)", 500, minZ, maxZ);
  //hVertexZ->SetStats(kFALSE);
  hVertexZ->SetXTitle("Z (#mum)");
  TH1F *hDist = new TH1F( "hdistance", "3D-distance track-beam", 500, 0., 50000.);
  hDist->SetXTitle("distance (#mum)");
  TH1F *hDistCut = new TH1F( "hdistancecut", "3D-distance track-beam", 500, 0., 50000.);
  hDistCut->SetXTitle("distance (#mum)");
  hDistCut->SetLineColor(2);
  TH2F *hTrackMapOrig = new TH2F( "htrackmaporig", "Tracks map at z=0", 200, 2*minX, 2*maxX, 200, 2*minY, 2*maxY);
  hTrackMapOrig->SetStats(1111111);
  hTrackMapOrig->SetXTitle("X (#mum)");
  hTrackMapOrig->SetYTitle("Y (#mum)");
  TH2F *hTrackMapOrigCut = new TH2F( "htrackmaporigcut", "Tracks map at z=0", 200, 2*minX, 2*maxX, 200, 2*minY, 2*maxY);
  hTrackMapOrigCut->SetStats(1111111);
  hTrackMapOrigCut->SetXTitle("X (#mum)");
  hTrackMapOrigCut->SetYTitle("Y (#mum)");
  hTrackMapOrigCut->SetMarkerColor(2);
  TH3F *hVertex3D = new TH3F( "hvertex3d", "Vertices in XYZ (rot)", 100, minX, maxX, 100, minY, maxY, 100, minZ, maxZ);
  hVertex3D->SetStats(kFALSE);
  hVertex3D->SetXTitle("X (#mum)");
  hVertex3D->SetYTitle("Y (#mum)");
  hVertex3D->SetZTitle("Z (#mum)");

  pad->cd(4);
  hVertex3D->DrawCopy();

  //Loop over the requested number of events
  for( Int_t iEvt=0; iEvt < nEvents; iEvt++) {
    if( !(fSession->NextRawEvent()) ) break;

    // === Analysis ====
    fSession->NextRawEvent();
    tTracker->Update();
    Int_t nTracks = tTracker->GetTracksN();
    // =================

    //cout << "evt " << iEvt << ": " << nTracks << " tracks to display" << endl;
    for( Int_t iTrack=1; iTrack<=nTracks; iTrack++) { // loop on tracks
      aTrack = (DTrack*)tTracker->GetTrack( iTrack);
      DR3 trackSlope = aTrack->GetLinearFit().GetSlopeZ();
      DR3 trackOrigin = aTrack->GetLinearFit().GetOrigin();
      trackPoint1[0] = trackOrigin(0)+trackSlope(0)*0.;//zTrackMin;
      //trackPoint1[0] = 0.; // test
      trackPoint1[1] = trackOrigin(1)+trackSlope(1)*0.;//zTrackMin;
      trackPoint1[2] = 0.;//zTrackMin;
      trackPoint2[0] = trackOrigin(0)+trackSlope(0)*zTrackMin;
      //trackPoint2[0] = trackSlope(0)*1.; // test
      trackPoint2[1] = trackOrigin(1)+trackSlope(1)*zTrackMin;
      //rackPoint2[1] = trackOrigin(1)+trackSlope(1)*1.;
      trackPoint2[2] = zTrackMin;

      // === vertex computing
      Double_t distance = fTool.BuildVertex( linePoint1, linePoint2, trackPoint1, trackPoint2, vertex);
      if( fVerbose) printf( "ievt %d track %d: orig %.1f, %.1f - slope %.1f %.1f, vertex %.1f, %.1f, %.1f, shortest dist %.1f\n", iEvt, iTrack, trackOrigin(0), trackOrigin(1), trackSlope(0), trackSlope(1), vertex[0], vertex[1], vertex[2], distance);

      hDist->Fill( distance);
      hTrackMapOrig->Fill( trackOrigin(0), trackOrigin(1));

      // === potential selection
      if( 0
	 //|| aTrack->GetHitsNumber() < 3
	 //|| tTracker->GetTracksN()!=1
	 || trackOrigin(0)>12000 || -2000>trackOrigin(0)
	 || trackOrigin(1)>8000 || -6000>trackOrigin(1)
	 //|| trackOrigin(1)>-7000
	 //|| distance>1000
	 ) continue;

      // === rotate back vertex coordinates
      Double_t xv = vertex[0]*cos( beamTilt*TMath::DegToRad()) - vertex[2]*sin( beamTilt*TMath::DegToRad());
      //vertex[1] /= 1.;
      Double_t zv = vertex[2]*cos( beamTilt*TMath::DegToRad()) + vertex[0]*sin( beamTilt*TMath::DegToRad());
      vertex[0] = xv;
      vertex[2] = zv;

      hDistCut->Fill( distance);
      hTrackMapOrigCut->Fill( trackOrigin(0), trackOrigin(1));
      hVertexX->Fill( vertex[0]);
      hVertexY->Fill( vertex[1]);
      hVertexZ->Fill( vertex[2]);
      hVertex3D->Fill( vertex[0], vertex[1], vertex[2]);

      // 3D display
      pad->cd(4);
      TPolyMarker3D *point3D = new TPolyMarker3D( 1, vertex, 20);
      point3D->SetMarkerColor(2);
      point3D->SetMarkerSize(0.2);
      point3D->DrawClone();
      //delete point3D;

    } //end loop on tracks

  } // END LOOP ON EVENTS
  fSession->GetDataAcquisition()->PrintStatistics();
  tTracker->PrintStatistics();

  // Display vertex projections
  pad->cd(2);
  hVertexX->DrawCopy();
  pad->cd(3);
  hVertexY->DrawCopy();
  pad->cd(1);
  hVertexZ->DrawCopy();
  pad->cd(5);
  hDist->DrawCopy();
  hDistCut->DrawCopy("same");
  pad->cd(6);
  hTrackMapOrig->DrawCopy();
  hTrackMapOrigCut->DrawCopy("same");

  cvertex->Update();

  Char_t title[500];
  sprintf( title, "vertexFile_run%d.root", fSession->GetRunNumber());
  TString filename = fSession->GetResultDirName() + title;
  fTool.LocalizeDirName( &filename);
  TFile outVertexFile( filename.Data(), "recreate");
  cvertex->Write();
  hVertexX->Write();
  hVertexY->Write();
  hVertexZ->Write();
  hDist->Write();
  hTrackMapOrig->Write();
  hDistCut->Write();
  hTrackMapOrigCut->Write();
  hVertex3D->Write();
  outVertexFile.Close();

}


//______________________________________________________________________________
//
void MRaw::FakeRateBinaryFromRawData( Int_t nEvents, Int_t aPlaneNumber, Int_t maxPixPerEvent, Double_t maxRateForTruncation, Int_t lowestCol, Int_t highestCol, Int_t lowestRow, Int_t highestRow)
{
  // Compute the fake hit rate from the cumul of the raw data
  //  for a given plane ().
  // The method only work for a sensor with binary outputs.
  //
  // INPUTS:
  //  - nEvents = number of events to read
  //  - aPlaneNumber = plane to be studied
  //  - maxPixPerEvent = scale of the histogram distributing the # fired pixels per event
  //  - maxRateForTruncation = maximum fake hit rate to include pixels in the computation of the truncated average fake hit rate
  //  - lowestCol, highestCol, lowestRow, highestRow = limits in pixel indexes of the interested zone, set to 0 if no limit
  //
  // JB 2011/11/22
  // Modified: JB 2011/11/25 corrected count of fired pixels in the case of a restricted geometric range
  // Modified: JB 2011/11/28 remove non-fired pixels from the distribution per pixel
  // Modified: JB 2011/12/29 change in bin for distribution per pixel and count of fired pixels
  // Modified: MG 2012/03/14 results store in a file


  // -- Initialisation

  fSession->SetEvents(nEvents);


  DTracker *tTracker  =  fSession->GetTracker();
  DPlane* tPlane = tTracker->GetPlane( aPlaneNumber);

  // set the column and row ranges wrt to provided information
  //  only if they make sense
  Int_t nPixelsU = tPlane->GetStripsNu();
  Int_t nPixelsV = tPlane->GetStripsNv();
  Int_t minCol   = 0;
  Int_t maxCol   = nPixelsU-1;
  Int_t minRow   = 0;
  Int_t maxRow   = nPixelsV-1;
  if( lowestCol < highestCol && 0<=lowestCol && highestCol<=nPixelsU ) {
    minCol = lowestCol;
    maxCol = highestCol;
    nPixelsU = maxCol - minCol + 1;
  }
  if( lowestRow < highestRow && 0<=lowestRow && highestRow<=nPixelsV ) {
    minRow = lowestRow;
    maxRow = highestRow;
    nPixelsV = maxRow - minRow + 1;
  }

  Char_t name[50], title[100];

  sprintf( name, "hrdmappl%d", aPlaneNumber);
  sprintf( title, "Raw data map of plane %d - %s", aPlaneNumber, tPlane->GetPlanePurpose());
  TH2F *hRDMap = new TH2F(name, title, tPlane->GetStripsNu(), 0, tPlane->GetStripsNu(), tPlane->GetStripsNv(), 0, tPlane->GetStripsNv());
  hRDMap->SetStats(kFALSE);
  hRDMap->SetXTitle("column number");
  hRDMap->SetXTitle("row number");

  sprintf( name, "hpixmulteventpl%d", aPlaneNumber);
  sprintf( title, "# pixels per event of plane %d - %s", aPlaneNumber, tPlane->GetPlanePurpose());
  TH1F *hpixmultevent = new TH1F ( name, title, maxPixPerEvent, 0, maxPixPerEvent);
  hpixmultevent->SetXTitle("# pixels per event");
  hpixmultevent->SetFillColor(5);
  hpixmultevent->SetFillStyle(1001);

  const Int_t nbins = 11;
  Double_t lowBinEdge[nbins+1] = {1.e-10, 1.e-9, 1.e-8, 1.e-7, 1.e-6, 1.e-5, 1.e-4, 1.e-3, 1.e-2, 1.e-1, 1., 10.};

  sprintf( name, "hfakeperpixpl%d", aPlaneNumber);
  sprintf( title, "Fake rate distribution per pixel of plane %d - %s", aPlaneNumber, tPlane->GetPlanePurpose());
  TH1F *hfakeperpix = new TH1F( name, title, nbins, lowBinEdge);
  hfakeperpix->SetXTitle("Fake rate per pixel");
  hfakeperpix->SetFillColor(5);
  hfakeperpix->SetFillStyle(1001);

  sprintf( name, "hfakecumpl%d", aPlaneNumber);
  sprintf( title, "Proportion of pixels below a given fake rate of plane %d - %s", aPlaneNumber, tPlane->GetPlanePurpose());
  TH1F *hfakecum = new TH1F( name, title, nbins, lowBinEdge);
  hfakecum->SetXTitle("Fake rate per pixel relative to average rate");
  hfakecum->SetLineWidth(2.);
  hfakecum->SetStats(0);

  TLine *l = new TLine();
  l->SetLineColor(2);
  l->SetLineWidth(2.);
  TLatex *t = new TLatex();
  t->SetTextAngle(90.);
  t->SetTextColor(2);
  Char_t string[500];
  sprintf( string, "average");

  Int_t nFiredPixels = 0;
  Int_t nProportionFiredPixels = 0; // JB 2011/12/29
  Int_t EventsTaken=0;

  // -- Loop over the requested number of events

  for( Int_t iEvt=0; iEvt < nEvents; iEvt++) {

    if( !fSession->NextRawEvent() ) break; // Stop when no more events to read, JB 2011/07/08
    EventsTaken++;
    tTracker->Update();
    Int_t iCol, iRow;
    Int_t nFiredPixelsInEvent = 0;
   std::vector<DPixel*> *aList = tPlane->GetListOfPixels();

    for( Int_t iPix=0; iPix<(Int_t)aList->size(); iPix++) { // loop on fired pixels
      iCol = aList->at(iPix)->GetPixelColumn();
      iRow = aList->at(iPix)->GetPixelLine();
      if( minCol <= iCol && iCol <= maxCol && minRow <= iRow && iRow <= maxRow) {
	nFiredPixelsInEvent++;
	hRDMap->Fill( iCol, iRow, 1);
      }
    } //end loop on fired pixels

    nFiredPixels += nFiredPixelsInEvent; // incrementation corrected, JB 2011/11/25
    hpixmultevent->Fill( nFiredPixelsInEvent);

  } // END LOOP ON EVENTS

  fSession->GetDataAcquisition()->PrintStatistics();
  tTracker->PrintStatistics();
  if(EventsTaken<nEvents){ //if there is less events then was requested SS 2011.11.28
    cout << "There was only "<<EventsTaken<<" events in the run. Will use this number for normalization."<<endl;
    nEvents=EventsTaken;
  }

  TCanvas *cfakerate;
  TObject* g = gROOT->FindObject("cfakerate") ;
  if (g) {
    cfakerate = (TCanvas*)g;
  }
  else {
    cfakerate = new TCanvas("cfakerate", "Fake rate from RawData", 5, 5,800,700);
  }
  cfakerate->Clear();
  cfakerate->UseCurrentStyle();
  TPaveLabel* label = new TPaveLabel();
  Char_t canvasTitle[200];
  sprintf(canvasTitle, "Run %d, cumul over %d events", fSession->GetRunNumber(), nEvents);
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad = new TPad("pad","",0.,0.,1.,0.965);
  pad->Draw();
  pad->Divide(2,2);
  // -- Analysis

  Int_t	   nPixels =  nPixelsU*nPixelsV;

  Double_t averageFakeRate = hpixmultevent->GetMean() / (Double_t)nPixels;
  Double_t rmsFakeRate = hpixmultevent->GetRMS()/sqrt(hpixmultevent->GetEntries()) / (Double_t)nPixels;

  Double_t countingFakeRate = nFiredPixels / (Double_t)nEvents / (Double_t)nPixels;
  Double_t poissonUncertainty = sqrt(nFiredPixels) / (Double_t)nEvents / (Double_t)nPixels;

  // Fill the rate per pixel
  //  attribute any rate in overflow in the last bin
  Int_t    nFiredPixelsTruncated = 0;
  Int_t	   nPixelsTruncated =  0;
  Double_t aRate = 1.;
  //Double_t minRate = 3./(Double_t)nEvents; // minimal rate according to Poisson statistics (expected counts<3 at 95% CL)
  for( Int_t ibx=1; ibx<=hRDMap->GetNbinsX(); ibx++) {
    for( Int_t iby=1; iby<=hRDMap->GetNbinsY(); iby++) {
      if( minCol+1 <= ibx && ibx <= maxCol+1 && minRow+1 <= iby && iby <= maxRow+1) { // if the correct geometric range

	aRate = hRDMap->GetBinContent( ibx, iby)/(Double_t)nEvents;
	if( hfakeperpix->GetBinLowEdge(hfakeperpix->GetNbinsX()+1) < aRate ) {
	  hfakeperpix->Fill( hfakeperpix->GetBinCenter(hfakeperpix->GetNbinsX()));
	}
	else {
	  hfakeperpix->Fill( aRate);
	}
	if( aRate < maxRateForTruncation ) {
	  nFiredPixelsTruncated += hRDMap->GetBinContent( ibx, iby);
	  nPixelsTruncated++;
	}

	if( hRDMap->GetBinContent( ibx, iby) > 0 ) { // JB, 2011/12/29
	  nProportionFiredPixels++;
	}

      } // end if the correct geometric range
    }
  }
  hfakeperpix->Scale( 1./nPixels );

  // Fill the cumulative distribution
  for( Short_t ib=1; ib<=nbins; ib++) {
    for( Short_t jb=0; jb<=ib; jb++) {
      hfakecum->AddBinContent( ib, hfakeperpix->GetBinContent( jb) );
    }
  }

  //Double_t proportionFiredPixels = 1.*nProportionFiredPixels / nPixels;
  Double_t truncatedFakeRate = nFiredPixelsTruncated / (Double_t)nEvents / (Double_t)nPixelsTruncated;
  Double_t truncatedUncertainty = sqrt(nFiredPixelsTruncated) / (Double_t)nEvents / (Double_t)nPixelsTruncated;


  cout << "---- Fake rate - Plane " << aPlaneNumber << " -----" << endl;
  cout << "Computed over " << nEvents << " events and for " << nPixels << " pixels" << endl;
  cout << " geometrical limits for columns [ "<< minCol<<", "<< maxCol<<"] and for rows ["<< minRow<<", "<< maxRow<<"]" << endl;
  cout << "Counting the # fired pixels: " << endl;
  cout << "  average fake rate = " << countingFakeRate << ", uncertainty from Poisson = " << poissonUncertainty << endl;
  cout << "Using distribution of fired pixels per event: " << hpixmultevent->GetName() << endl;
  cout << "  average fake rate = " << averageFakeRate << " RMS over events = " << rmsFakeRate << endl;
  cout << "Using distribution of fake rate per pixel: " << hfakeperpix->GetName() << endl;
  cout << "  average fake rate = " << hfakeperpix->GetMean() <<  " RMS over pixels = " << hfakeperpix->GetRMS() << endl;
  cout << "Truncated average with the " << nPixelsTruncated << " pixels having a rate below " << maxRateForTruncation << ":" << endl;
  cout << "  average fake rate = " << truncatedFakeRate << ", uncertainty = " << truncatedUncertainty << endl;
  cout << endl;


  // -- Display
  gStyle->SetOptStat(1111111);

  pad->cd(1);
  hRDMap->DrawCopy("colz");
  pad->cd(2);
  hpixmultevent->DrawCopy();
  l->DrawLine(averageFakeRate*nPixels, hpixmultevent->GetMinimum(), averageFakeRate*nPixels, hpixmultevent->GetMaximum());
  t->DrawLatex( averageFakeRate*nPixels, (hpixmultevent->GetMaximum()-hpixmultevent->GetMinimum())/30., string);
  pad->cd(3);
  hfakeperpix->DrawCopy();
  l->DrawLine(averageFakeRate, hfakeperpix->GetMinimum(), averageFakeRate, hfakeperpix->GetMaximum());
  t->DrawLatex( averageFakeRate, (hfakeperpix->GetMaximum()-hfakeperpix->GetMinimum())/30., string);
  gPad->SetLogy();
  gPad->SetLogx();
  pad->cd(4);
  hfakecum->DrawCopy();
  l->DrawLine(averageFakeRate, hfakecum->GetMinimum(), averageFakeRate, hfakecum->GetMaximum());
  t->DrawLatex( averageFakeRate, (hfakecum->GetMaximum()-hfakecum->GetMinimum())/30., string);
  gPad->SetLogx();
  cfakerate->Update();



  // -- Save canvas and histos
  ofstream csvfile("Fake_results.csv",ios::app);
  TDatime aTime;
  csvfile<<aTime.AsString()<<",";
  csvfile << fSession->GetRunNumber() << "," << aPlaneNumber << "," << lowestCol << "," << highestCol << "," << lowestRow << "," << highestRow << "," << nEvents << "," << countingFakeRate << "," << poissonUncertainty <<endl;
  csvfile.close();

  Char_t rootFile[300];
  sprintf(rootFile,"%s/FakeRate_run%d_pl%d.root",fSession->GetResultDirName().Data(),fSession->GetRunNumber(), aPlaneNumber);
  sprintf(rootFile,"%s", fTool.LocalizeDirName( rootFile));
  cout << "\n-- Saving histos and canvas into " << rootFile << endl;
  TFile fRoot(rootFile,"RECREATE");
  cfakerate->Write();
  hRDMap->Write();
  hpixmultevent->Write();
  hfakeperpix->Write();
  hfakecum->Write();
  fRoot.Close();

}



//______________________________________________________________________________
//
void MRaw::DumpEventHeader( Int_t nEvents)
{
  // Dump the header of the required # events into a file.
  // The header contains the event number, trigger and frame information.
  //
  // Currently print noly the first trigger/frame/stamp of the list.
  //
  // JB 2012/05/03

  fSession->SetEvents(nEvents);
  DAcq *acquisition = fSession->GetDataAcquisition();

  FILE *outFile;
  Char_t fileName[300];
  sprintf( fileName, "%s/eventHeaders_dump_run%d.txt", fSession->GetResultDirName().Data(), fSession->GetRunNumber());
  sprintf( fileName,"%s", fTool.LocalizeDirName( fileName));
  outFile = fopen( fileName, "w");
  if( outFile==NULL ) {
    cout << "ERROR: CANNOT CREATE FILE " << fileName << " to dump header info." << endl;
    return;
  }
  fprintf( outFile,  "Event# DAQEvent# #triggers 1stTrigger #frames 1stFrame #timestamps 1stTimestamp\n");


  for( Int_t iEvt=0; iEvt < nEvents; iEvt++) { // LOOP ON EVENTS

    if( !fSession->NextRawEvent() ) break;

    fprintf( outFile, "%d %d %d ", acquisition->GetEventNumber(), acquisition->GetRealEventNumber(), acquisition->GetNumberOfTriggers());
    if( acquisition->GetNumberOfTriggers()>0) {
      fprintf( outFile,  "%d ", acquisition->GetTriggerAt(0));
    }
    else {
      fprintf( outFile,  "%d ", -1);
    }
    fprintf( outFile, "%d ", acquisition->GetNumberOfFrames());
    if( acquisition->GetNumberOfFrames()>0 ) {
      fprintf( outFile,  "%d ", acquisition->GetFrameAt(0));
    }
    else {
      fprintf( outFile,  "%d ", -1);
    }
    fprintf( outFile,  "%d ", acquisition->GetNumberOfTimestamps());
    if( acquisition->GetNumberOfTimestamps()>0 ) {
      fprintf( outFile,  "%d ", acquisition->GetTimestampAt(0));
    }
    else {
      fprintf( outFile,  "%d ", -1);
    }
    fprintf( outFile, "\n");

  } // END LOOP ON EVENTS

  acquisition->PrintStatistics();

  Info( "Text file %s has been created with dump of event headers: ", fileName);
  fclose( outFile);

}



//______________________________________________________________________________
//
void MRaw::UserPlot( Int_t nEvents)
{
  // Do whatever you want there
  //

  // ------------------------------------------------------
  // Example to plot frame differences between IHEP ladders wrt event numbers

  TCanvas *cdisplayuser = new TCanvas("cdisplayuser", "User plot", 5, 5, 900, 700);
  cdisplayuser->Divide(1,2);
  cdisplayuser->UseCurrentStyle();
  TProfile *hptdiffevt = new TProfile( "hptdiffevt", "Frame differences between ladders wrt event numbers;evt# x1000;#Delta Frame", 500, 0, 500, -100, 100);
  hptdiffevt->SetMarkerStyle(20);
  TH1F *h1tdiffracevt = new TH1F( "h1tdiffracevt", "Fraction of frame differences between ladders wrt event numbers;evt# x1000;#Delta Frame", 500, 0, 500);
  h1tdiffracevt->SetMarkerStyle(20);
  fSession->SetEvents(nEvents);
  DAcq *acq = fSession->GetDataAcquisition();
  Int_t currentEvtNb = fSession->GetCurrentEventNumber();
  ToggleVerbosity();
  while ( currentEvtNb<nEvents) { // loop on events
    fSession->NextRawEvent();
    currentEvtNb = fSession->GetCurrentEventNumber();
    double diff = acq->GetFrameAt(4)-acq->GetFrameAt(0);
    // printf(" event %5d, frameRef %d, frameDUT %d, diff %d\n", currentEvtNb, acq->GetFrameAt(0), acq->GetFrameAt(4), diff);
    hptdiffevt->Fill( currentEvtNb/1000, diff);
  } // end loop on events
  for (size_t i = 1; i <= 500; i++) {
    printf( "bin %d: %f - %d = %f\n", i, hptdiffevt->GetBinContent(i), (int)(hptdiffevt->GetBinContent(i)), hptdiffevt->GetBinContent(i)-(int)(hptdiffevt->GetBinContent(i)));
    h1tdiffracevt->SetBinContent( i, hptdiffevt->GetBinContent(i)-(int)(hptdiffevt->GetBinContent(i)) );
  }
  // Display
  cdisplayuser->cd(1);
  hptdiffevt->Draw("P");
  cdisplayuser->cd(2);
  h1tdiffracevt->Draw("P");
  cdisplayuser->Update();
  // Save the canvas
  TFile outfile(TString::Format("run%d_user.root", fSession->GetRunNumber()), "RECREATE");
  cdisplayuser->Write();
  h1tdiffracevt->Write();
  hptdiffevt->Write();
  outfile.Close();
  // cdisplayuser->SaveAs( TString::Format("run%d_%d_user.root", fSession->GetRunNumber(), nEvents) );

  // ------------------------------------------------------
  // Example to build two maps of pixel signals
  //  - one map is the average signal received per pixel and per event
  //  - the other maps pixels with signal higher than a given threshold
  //   no clustering is involved here BUT note that DisplayNoise() is called
  // JB 2018/05/02

/*
  //-- Init
  Int_t iPlane=1;
  Float_t threshold = 400;

  DTracker *tTracker  =  fSession->GetTracker();
  DPlane   *tPlane = tTracker->GetPlane(iPlane);


  //-- Book histograms

  TCanvas *cdisplayraw = new TCanvas("cdisplayraw", "Inspect Raw Data", 5, 5, 900, 700);
  cdisplayraw->UseCurrentStyle();
  cdisplayraw->Divide(2,1);

  Char_t name[50], title[100];
  sprintf( name, "hrawdatapl%d", iPlane);
  sprintf( title, "Average signal map of plane (%d) %s;col index;row index", iPlane, tPlane->GetPlanePurpose());
  TH2F *hRawData = new TH2F(name, title, tPlane->GetStripsNu(), 0, tPlane->GetStripsNu(), tPlane->GetStripsNv(), 0, tPlane->GetStripsNv());
  hRawData->SetStats(kFALSE);

  sprintf( name, "hhitmapipl%d", iPlane);
  sprintf( title, "Map of fired pixels plane (%d) %s;col index;row index", iPlane, tPlane->GetPlanePurpose());
  TH2F *hHitMapIndex = new TH2F(name, title, tPlane->GetStripsNu(), 0, tPlane->GetStripsNu(),
			  tPlane->GetStripsNv(), 0, tPlane->GetStripsNv());
  hHitMapIndex->SetStats(kFALSE);


  //-- Init noise
  DisplayNoise();
  fSession->SetEvents(nEvents);

  //-- start the loop on event nb to be read
  Int_t iCol, iRow;
  Int_t currentEvtNb = fSession->GetCurrentEventNumber();

  ToggleVerbosity();
  while ( currentEvtNb<nEvents) { // loop on events
    fSession->NextRawEvent();
    tTracker->Update();

    for( Int_t iStrip=0; iStrip<tPlane->GetStripsN(); iStrip++) { // loop on strips
      iCol = iStrip%tPlane->GetStripsNu();
      iRow = iStrip/tPlane->GetStripsNu();
      hRawData->Fill( iCol+1, iRow+1, tPlane->GetPulseHeight(iStrip));
      if( tPlane->GetPulseHeight(iStrip)>threshold ) hHitMapIndex->Fill( iCol+1, iRow+1, 1);
    } //end loop on strips

    currentEvtNb = fSession->GetCurrentEventNumber();
  } // end loop on events

  // Averaging
  hRawData->Scale(1./nEvents);

  // Display
  cdisplayraw->cd(1);
  hRawData->Draw("colz");
  cdisplayraw->cd(1);
  hHitMapIndex->DrawCopy("colz");
  cdisplayraw->Update();

  // Save the canvas
  cdisplayraw->SaveAs( TString::Format("run%d_%ds.root", fSession->GetRunNumber(), nEvents) );
*/


}


//______________________________________________________________________________
//
void MRaw::DisplayCumulatedMonteCarlo2D( Int_t nEvents, Bool_t ifDrawTrack)
{
  // Display the monte carlo position for each plane cumulated over the requested number of events
  // Call by gTAF->GetRaw()->DisplayCumulatedMonteCarlo2D()
  // Display Hit Monte Carlo vs reconstructed hit from pixels.
  // Display Hit Monte Carlo vs track impact in the plane.
  // Last Modified LC, 2014/12/16 : Cleaning code + New Monte Carlo Method + Removing write in root files

  if( !(fSession->GetSetup()->GetTrackerPar().HitMonteCarlo) ) {
    std::cout<<"Hit Monte Carlo Disable. Set HitMonteCarlo:1 in the config file"<<std::endl;
    return;
  }

  fSession->SetEvents(nEvents);

  Int_t nHitsReconstructed = 0;

  TCanvas *cCumulMonteCarlo;
  TObject* g = gROOT->FindObject("cCumulMonteCarlo") ;
  if (g) {
    cCumulMonteCarlo = (TCanvas*)g;
  }
  else {
    cCumulMonteCarlo = new TCanvas("cCumulMonteCarlo", "Hits Monte Carlo vs Track intersections", 5, 5,800,700);
  }
  cCumulMonteCarlo->Clear();
  cCumulMonteCarlo->UseCurrentStyle();
  TPaveLabel* label = new TPaveLabel();
  Char_t canvasTitle[200];
  sprintf(canvasTitle, "Run %d, cumul over %d events", fSession->GetRunNumber(), nEvents);
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad = new TPad("pad","",0.,0.,1.,0.965);
  pad->Draw();

  DTracker *tTracker  =  fSession->GetTracker();
  DLadder* aLadder;
  DPlane* tPlane;
  DTrack *aTrack;
  DHit *aHit;

  Int_t nPlanes = tTracker->GetPlanesN();
  pad->Divide( (Int_t)ceil(nPlanes/4.), (nPlanes>4)?4:1);  // LC 2012/01/04. 4 lines instead of 2.

  Int_t nLadders = tTracker->GetNumberOfLadders();
  for(Int_t iterLadder=1 ; iterLadder<nLadders ; ++iterLadder) {
    aLadder = tTracker->GetLadder(iterLadder);
    aLadder->UpdateAlignment();
  }

  // Get the first DUT among the planes and remember it,
  //  it considered as the one providing the coordinate frame for the geomatrix
  // If no DUT (status=3), set to first plane
  // JB 2012/0820
  DPlane* DUTplane = tTracker->GetPlane(1);
  for( Int_t iPlane=2; iPlane<=nPlanes; iPlane++) { // loop on planes
    tPlane = tTracker->GetPlane(iPlane);
    if( tPlane->GetStatus()==3 ) {
      DUTplane = tPlane;
      break;
    }
  }
  // geomatrix coordinates and box
  Int_t geomatrix=3;
  DR3 gUVWmin( fSession->GetSetup()->GetAnalysisPar().Umin[0][geomatrix], fSession->GetSetup()->GetAnalysisPar().Vmin[0][geomatrix], DUTplane->GetPosition()(2));
  DR3 gUVWmax( fSession->GetSetup()->GetAnalysisPar().Umax[0][geomatrix], fSession->GetSetup()->GetAnalysisPar().Vmax[0][geomatrix], DUTplane->GetPosition()(2));
  // Transform those DUTplane frame coordinates into the telescope frame
  DR3 gXYZmin = DUTplane->PlaneToTracker(gUVWmin);
  DR3 gXYZmax = DUTplane->PlaneToTracker(gUVWmax);
  printf("Geomatrix: U-range %.0f, %.0f, V-range %.0f, %.0f\n", gUVWmin(0), gUVWmax(0), gUVWmin(1), gUVWmax(1));
  printf("Geomatrix: X-range %.0f, %.0f, Y-range %.0f, %.0f\n", gXYZmin(0), gXYZmax(0), gXYZmin(1), gXYZmax(1));
  cout << " DUT plane is got as plane " << DUTplane->GetPlaneNumber() << endl;

  TBox **geomBox = new TBox*[nPlanes];
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    geomBox[iPlane-1] = new TBox( gXYZmin(0), gXYZmin(1), gXYZmax(0), gXYZmax(1) );
    geomBox[iPlane-1]->SetFillStyle(0);
    geomBox[iPlane-1]->SetLineStyle(1);
    geomBox[iPlane-1]->SetLineWidth(2);
    geomBox[iPlane-1]->SetLineColor(1);
  } // end loop on planes

  // Determine extrema of planes position in telescope frame
  Double_t xmin=1e6, xmax=-1e6;
  Double_t ymin=1e6, ymax=-1e6;
  Double_t planeBox[2][2] = {{0,0},{0,0}};
  TBox **geomPlaneBox = new TBox*[nPlanes];
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    tPlane = tTracker->GetPlane(iPlane);
    // bottom left corner
    DR3 posInPlane, posBLInTracker, posURInTracker;
    posInPlane.SetValue( -tPlane->GetStripsNu() * tPlane->GetStripPitch()(0) / 2.
                        ,-tPlane->GetStripsNv() * tPlane->GetStripPitch()(1) / 2.
                        ,0.);
    posBLInTracker = tPlane->PlaneToTracker( posInPlane);
    if( posBLInTracker(0)<xmin ) xmin = posBLInTracker(0);
    if( posBLInTracker(1)<ymin ) ymin = posBLInTracker(1);
    if( posBLInTracker(0)>xmax ) xmax = posBLInTracker(0);
    if( posBLInTracker(1)>ymax ) ymax = posBLInTracker(1);
    // upper right corner
    posInPlane.SetValue( +tPlane->GetStripsNu() * tPlane->GetStripPitch()(0) / 2.
                        ,+tPlane->GetStripsNv() * tPlane->GetStripPitch()(1) / 2.
                        ,0.);
    posURInTracker = tPlane->PlaneToTracker( posInPlane);
    if( posURInTracker(0)<xmin ) xmin = posURInTracker(0);
    if( posURInTracker(1)<ymin ) ymin = posURInTracker(1);
    if( posURInTracker(0)>xmax ) xmax = posURInTracker(0);
    if( posURInTracker(1)>ymax ) ymax = posURInTracker(1);
    planeBox[0][0] = TMath::Min( posBLInTracker(0), posURInTracker(0));
    planeBox[0][1] = TMath::Max( posBLInTracker(0), posURInTracker(0));
    planeBox[1][0] = TMath::Min( posBLInTracker(1), posURInTracker(1));
    planeBox[1][1] = TMath::Max( posBLInTracker(1), posURInTracker(1));
    geomPlaneBox[iPlane-1] = new TBox( planeBox[0][0], planeBox[1][0], planeBox[0][1], planeBox[1][1]);
    geomPlaneBox[iPlane-1]->SetFillStyle(0);
  } // end loop on planes

  TH2F **hMonteCarloMap = new TH2F*[nPlanes];
  TH2F **hTrackMap = new TH2F*[nPlanes]; // JB 2011/11/02
  TH2F **hHitMap = new TH2F*[nPlanes];

  Char_t name[50], title[100];
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    tPlane = tTracker->GetPlane(iPlane);

    // -- Histo with microns
    sprintf( name, "hmontecarlomappl%d", iPlane);
    sprintf( title, "Monte Carlo map of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    // check if the histo is existing or not
    hMonteCarloMap[iPlane-1] = new TH2F(name, title, 2000, xmin, xmax, 2000, ymin, ymax);
    hMonteCarloMap[iPlane-1]->SetMarkerStyle(20);
    hMonteCarloMap[iPlane-1]->SetMarkerSize(.2);
    hMonteCarloMap[iPlane-1]->SetMarkerColor(1);
    hMonteCarloMap[iPlane-1]->SetStats(kFALSE);
    //printf( "MRaw::DisplayRawData created %s histo with %dx%d pixels\n", name, tPlane->GetStripsNu(), tPlane->GetStripsNv());

   sprintf( name, "hhitmappl%d", iPlane);
   sprintf( title, "Hit map of plane %d - %s", iPlane, tPlane->GetPlanePurpose());

    hHitMap[iPlane-1] =  new TH2F(name, title, 2000, xmin, xmax, 2000, ymin, ymax);
    hHitMap[iPlane-1]->SetMarkerStyle(24);
    hHitMap[iPlane-1]->SetMarkerSize(.2);
    hHitMap[iPlane-1]->SetMarkerColor(3);
    hHitMap[iPlane-1]->SetStats(kFALSE);

    // -- Histo for tracks with microns, JB 2011/11/02
    sprintf( name, "htrackmappl%d", iPlane);
    sprintf( title, "Track map of plane (%d) %s", iPlane, tPlane->GetPlanePurpose());
    hTrackMap[iPlane-1] = new TH2F( *(hMonteCarloMap[iPlane-1])); // JB 2009/09/09
    //name, title, hHitMap[iPlane-1]->GetNbinsX(), hHitMap[iPlane-1]->GetNbinsX(), tPlane->GetStripPitch()(0)*tPlane->GetStripsNu()/2., 100, -tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2., tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2.);
    hTrackMap[iPlane-1]->SetName( name);
    hTrackMap[iPlane-1]->SetTitle( title);
    hTrackMap[iPlane-1]->SetMarkerStyle(24);
    hTrackMap[iPlane-1]->SetMarkerSize(.2);
    hTrackMap[iPlane-1]->SetMarkerColor(65);
    hTrackMap[iPlane-1]->SetStats(kFALSE);
  }


// ------ Loop on events and planes ---------//

  //Loop over the requested number of events
  for( Int_t iEvt=0; iEvt < nEvents; iEvt++) {

    if( !(fSession->NextRawEvent()) ) break; // JB 2009/06/26

    tTracker->Update();

    for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes

      tPlane = tTracker->GetPlane(iPlane);
      Int_t hitsNumber = tPlane->GetHitsN();

// --------- Hits and Hits MonteCarlo -------//

      if( hitsNumber>0 ) {
        nHitsReconstructed += hitsNumber;

        for( Int_t iMonteCarlo=1; iMonteCarlo<hitsNumber; iMonteCarlo++) { //loop on hits (starts at 1 !!)

             aHit = (DHit*)tPlane->GetHit( iMonteCarlo );
             // Hit Monte Carlo in tracker frame
             DR3 trackerCoord = aHit->GetMonteCarloPosition();
             // Hit Monte Carlo in Plane frame
             DR3 planeCoord   = tPlane->TrackerToPlane( trackerCoord );
             // Fill Hit Monte Carlo Map
             hMonteCarloMap[iPlane-1]->Fill( planeCoord(0), planeCoord(1), 1 );

             hHitMap[iPlane-1]->Fill( aHit->GetPositionUhit(), aHit->GetPositionVhit(), 1); // weight could be aHit->GetClusterPulseSum()

             if(fVerbose) printf("MRaw::DisplayCumulHits2D  pl %d, hit[%d=(%d,%d)=(%f,%f)]%f, mult=%d\n", iPlane, iMonteCarlo, aHit->GetIndexSeed()%tPlane->GetStripsNu(), aHit->GetIndexSeed()/tPlane->GetStripsNu(), tPlane->PlaneToTracker(*(aHit->GetPosition()))(0), tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), aHit->GetClusterPulseSum(), aHit->GetStripsInCluster());
        } //end loop on hits
      }

// ------ Tracks ------ //

      if( tTracker->GetTracksN()>0 ) {
        for( Int_t iTrack=1; iTrack<=tTracker->GetTracksN(); iTrack++ ) { // loop on tracks, JB 2011/11/02
          aTrack = tTracker->GetTrack(iTrack);
          DR3 posInPlane = aTrack->Intersection(tPlane);
          hTrackMap[iPlane-1]->Fill( posInPlane(0), posInPlane(1), 1);
          if(fVerbose) printf("MRaw::DisplayCumulHits2D  pl %d, track[%d=(%f,%f)\n", iPlane, iTrack, tPlane->PlaneToTracker(posInPlane)(0), tPlane->PlaneToTracker(posInPlane)(1));
        } // end loop on tracks
      } // end if

    } //end loop on planes

  } // End loop on events

  fSession->GetDataAcquisition()->PrintStatistics();
  tTracker->PrintStatistics();

  // Now display
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    pad->cd(iPlane);
    if( ifDrawTrack) {
      hTrackMap[iPlane-1]->DrawCopy(); // JB 2011/11/02
      hMonteCarloMap[iPlane-1]->DrawCopy("same");
    }
    else {
      hMonteCarloMap[iPlane-1]->DrawCopy("colz");
    }
    geomPlaneBox[iPlane-1]->Draw("l");
    //geomBox[iPlane-1]->Draw("l");
    cout << "Plane "<<iPlane<<" has seen "<<hMonteCarloMap[iPlane-1]->GetEntries()<<" hits Monte Carlo."<< endl;
  } // end for

  cCumulMonteCarlo->Update();

  TCanvas *ccumulhit2;
  g = gROOT->FindObject("ccumulhit2") ;
  if (g) {
    ccumulhit2 = (TCanvas*)g;
  }
  else {
    ccumulhit2 = new TCanvas("ccumulhit2", "Hit Monte Carlo vs Hit TAF", 5, 5,800,700);
  }
  ccumulhit2->Clear();
  ccumulhit2->UseCurrentStyle();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad2 = new TPad("pad2","",0.,0.,1.,0.965);
  pad2->Draw();
  pad2->Divide( (Int_t)ceil(nPlanes/4.), (nPlanes>1)?4:1); // LC 2012/01/04. 4 lines instead of 2.

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    pad2->cd(iPlane);
    if(ifDrawTrack) {
      hHitMap[iPlane-1]->DrawCopy();
      hMonteCarloMap[iPlane-1]->DrawCopy("same");
    //hHitPixMult[iPlane-1]->Delete();
    }
    else {
       hMonteCarloMap[iPlane-1]->DrawCopy("colz");
    }
  }
  ccumulhit2->Update();

  // Save canvas and histos
  //cd to result dir
  // added JB 2011/03/14
  /*Char_t rootFile[300];
  sprintf(rootFile,"%sHitMap_run%d.root",fSession->GetResultDirName().Data(),fSession->GetRunNumber());
  sprintf(rootFile,"%s", fTool.LocalizeDirName( rootFile)); // JB 2011/07/07
  cout << "\n-- Saving histos and canvas into " << rootFile << endl;
  TFile fRoot(rootFile,"RECREATE");
  */
  /*
  cCumulMonteCarlo->Write();
  ccumulhit2->Write();
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    hMonteCarloMap[iPlane-1]->Write();
    hTrackMap[iPlane-1]->Write(); // JB 2011/11/02
  }

  fRoot.Close();
  */

}



//______________________________________________________________________________
//
void MRaw::DisplayHistory( Int_t nEvents, Int_t nCumul)
{
  // Display event-wise information per event number
  //  History is done over nEvents events
  //  with averages computed every nCumul(<<nEvents) events
  //
  // JB 2014/06/02

  // In case nEvents<nCumul force a realistic situation
  if ( nEvents<nCumul) {
    nCumul = TMath::Max(1, nEvents/100);
  }
  Int_t nBins = nEvents/nCumul;
  Int_t firstEvent = fSession->GetCurrentEventNumber();

  fSession->SetEvents(nEvents);

  DTracker *tTracker  =  fSession->GetTracker();
  Int_t nPlanes = tTracker->GetPlanesN();
  DPlane* tPlane;
  DHit* aHit;
  DTrack* aTrack;

  // ---- Booking

  TProfile **hpHitsPerEvent = new TProfile*[nPlanes];
  TProfile **hpClusterMult = new TProfile*[nPlanes];
  TObject* g;
  Char_t name[50], title[100];
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    tPlane = tTracker->GetPlane(iPlane);

    // Nb of hits per event
    sprintf( name, "pHitsPerEventPl%d", iPlane);
    sprintf( title, "# hits per event, plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    g = gROOT->FindObject(name) ;
    if (g) {
      hpHitsPerEvent[iPlane-1] = (TProfile*)g;
    }
    else {
      hpHitsPerEvent[iPlane-1] = new TProfile(name, title, nBins, firstEvent, (Double_t)(firstEvent+nEvents), 0., 0.);
      hpHitsPerEvent[iPlane-1]->SetXTitle("Event number");
      hpHitsPerEvent[iPlane-1]->SetYTitle("# hits per event");
      hpHitsPerEvent[iPlane-1]->SetMarkerStyle(20);
      hpHitsPerEvent[iPlane-1]->SetMarkerSize(0.8);
      hpHitsPerEvent[iPlane-1]->SetStats(0);
    }

    // Cluster multiplicity
    sprintf( name, "pClusterMultPl%d", iPlane);
    sprintf( title, "Cluster multiplicity per event, plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    g = gROOT->FindObject(name) ;
    if (g) {
      hpClusterMult[iPlane-1] = (TProfile*)g;
    }
    else {
      hpClusterMult[iPlane-1] = new TProfile(name, title, nBins, firstEvent, (Double_t)(firstEvent+nEvents), 0., 0.);
      hpClusterMult[iPlane-1]->SetXTitle("Event number");
      hpClusterMult[iPlane-1]->SetYTitle("# pixels per cluster");
      hpClusterMult[iPlane-1]->SetMarkerStyle(20);
      hpClusterMult[iPlane-1]->SetMarkerSize(0.8);
      hpClusterMult[iPlane-1]->SetStats(0);
    }

  }  // end Loop on planes

  // Nb of tracks per event
  TProfile *hpTracksPerEvent;
  sprintf( name, "pTracksPerEvent");
  sprintf( title, "# tracks per event");
  g = gROOT->FindObject(name) ;
  if (g) {
    hpTracksPerEvent = (TProfile*)g;
  }
  else {
    hpTracksPerEvent = new TProfile(name, title, nBins, firstEvent, (Double_t)(firstEvent+nEvents), 0., 0.);
    hpTracksPerEvent->SetXTitle("Event number");
    hpTracksPerEvent->SetYTitle("# tracks per event");
    hpTracksPerEvent->SetMarkerStyle(21);
    hpTracksPerEvent->SetMarkerSize(0.8);
    hpTracksPerEvent->SetStats(0);
  }

  // Track multiplicity
  TProfile *hpTrackMult;
  sprintf( name, "pTrackMult");
  sprintf( title, "Track multiplicity");
  g = gROOT->FindObject(name) ;
  if (g) {
    hpTrackMult = (TProfile*)g;
  }
  else {
    hpTrackMult = new TProfile(name, title, nBins, firstEvent, (Double_t)(firstEvent+nEvents), 0., 0.);
    hpTrackMult->SetXTitle("Event number");
    hpTrackMult->SetYTitle("# hits per track");
    hpTrackMult->SetMarkerStyle(21);
    hpTrackMult->SetMarkerSize(0.8);
    hpTrackMult->SetStats(0);
  }



  // ---- Event loop

  //Loop over the requested number of events
  for( Int_t iEvt=0; iEvt < nEvents; iEvt++) {
    if( !(fSession->NextRawEvent()) ) break;
    tTracker->Update();

    for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
      tPlane = tTracker->GetPlane(iPlane);

      hpHitsPerEvent[iPlane-1]->Fill( iEvt, tPlane->GetHitsN());

      //cout << "CumulateHits2D:: plane " << tPlane->GetPlaneNumber() << " has " << tPlane->GetHitsN() << endl;
      if( tPlane->GetHitsN()>0 ) {
        for( Int_t iHit=1; iHit<=tPlane->GetHitsN(); iHit++) { //loop on hits (starts at 1 !!)
          aHit = (DHit*)tPlane->GetHit( iHit);
          //printf("Getting seed index for hit %d (address %x) at plane %d\n", iHit, aHit, iPlane);

          hpClusterMult[iPlane-1]->Fill( iEvt, aHit->GetStripsInCluster());
        } //end loop on hits
      }

    } //end loop on planes


    hpTracksPerEvent->Fill( iEvt, tTracker->GetTracksN());

    if( tTracker->GetTracksN()>0 ) {
      for( Int_t iTrack=1; iTrack<=tTracker->GetTracksN(); iTrack++ ) { // loop on tracks
        aTrack = tTracker->GetTrack(iTrack);
        hpTrackMult->Fill( iEvt, aTrack->GetHitsNumber());
      } // end loop on tracks
    }


  } // END LOOP ON EVENTS

  fSession->GetDataAcquisition()->PrintStatistics();
  tTracker->PrintStatistics();



  // ---- Display

  TCanvas *chistnhits;
  g = gROOT->FindObject("chistnhits") ;
  if (g) {
    chistnhits = (TCanvas*)g;
  }
  else {
    chistnhits = new TCanvas("chistnhits", "History of # hits per event", 5, 5,800,700);
  }
  chistnhits->Clear();
  chistnhits->UseCurrentStyle();
  if( nPlanes>6 ) {
    chistnhits->Divide( (Int_t)ceil(nPlanes/4.), (nPlanes>4)?4:nPlanes);
  }
  else {
    chistnhits->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);
  }

  TCanvas *chistcmult;
  g = gROOT->FindObject("chistcmult") ;
  if (g) {
    chistcmult = (TCanvas*)g;
  }
  else {
    chistcmult = new TCanvas("chistcmult", "History of cluster multiplicity", 5, 5,800,700);
  }
  chistcmult->Clear();
  chistcmult->UseCurrentStyle();
  if( nPlanes>6 ) {
    chistcmult->Divide( (Int_t)ceil(nPlanes/4.), (nPlanes>4)?4:nPlanes);
  }
  else {
    chistcmult->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);
  }

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // Loop on planes

    chistnhits->cd(iPlane);
    hpHitsPerEvent[iPlane-1]->Draw();

    chistcmult->cd(iPlane);
    hpClusterMult[iPlane-1]->Draw();

  } // end Loop on planes

  chistnhits->Update();
  chistcmult->Update();


  TCanvas *chisttrack;
  g = gROOT->FindObject("chisttrack") ;
  if (g) {
    chisttrack = (TCanvas*)g;
  }
  else {
    chisttrack = new TCanvas("chisttrack", "History on tracks", 5, 5,800,700);
  }
  chisttrack->Clear();
  chisttrack->UseCurrentStyle();
  chisttrack->Divide( 1, 2);

  chisttrack->cd(1);
  hpTracksPerEvent->Draw();
  chisttrack->cd(2);
  hpTrackMult->Draw();



  // ---- Saving

  Char_t rootFile[300];
  sprintf(rootFile,"%sHistory_run%d.root",fSession->GetResultDirName().Data(),fSession->GetRunNumber());
  sprintf(rootFile,"%s", fTool.LocalizeDirName( rootFile)); // JB 2011/07/07
  cout << "\n-- Saving histos and canvas into " << rootFile << endl;
  TFile fRoot(rootFile,"RECREATE");
  chistnhits->Write();
  chistcmult->Write();
  chisttrack->Write();
  hpTracksPerEvent->Write();
  hpTrackMult->Write();
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    hpHitsPerEvent[iPlane-1]->Write();
    hpClusterMult[iPlane-1]->Write();
  }
  fRoot.Close();

}

//______________________________________________________________________________
//
void MRaw::DisplayImage( Int_t nEvents, Int_t planeToPlot, TString data, TString format, Bool_t displayImage, Bool_t saveImage, Float_t minValue, Float_t NBins, Bool_t greyScale, TString flip)    {
  // Display an image of the matrix cumulated over nEvents.
  // If data="raw", plot pixel raw data
  // If data="hit", plot hit
  //
  // Save images (one per existing planes) in the given format,
  //  could be gif, jpg, png, tiff
  //
  // JB 2014/06/02
  // Last modified: JH 2016 May the 4th (be with you) : Display/Save images with the size corresponding to the matrix. Data type used. Addition of Greyscale mode for X-Rays. Addition of possibility to flip the results

//  gROOT->SetBatch();// Must be set like that to prevent images to be resized to the scrren's resolution =)
//  cout<<"Batch Mode Activated !"<<endl;
  cout<<"*********************************************************"<<endl;
  cout<<"******             Generating Images               ******"<<endl;

  if (data=="hit"||data=="hits")    {
    cout<<"******                   HIT MAP                   ******"<<endl;
  }
  else    {
    cout<<"******                  RAW DATA                   ******"<<endl;
    cout<<"******     (IF HITS WANTED : CHECK PARAMETERS)     ******"<<endl;
  }

  if (flip=="X"||flip=="Y"||flip=="XY")    {
    if (flip=="X"||flip=="Y")    {
      cout<<"******                   FLIP "<<flip<<"                    ******"<<endl;
    }
    else    {
      cout<<"******                   FLIP "<<flip<<"                   ******"<<endl;
    }
  }
  else    {
    //NOTHING
  }
  cout<<"*********************************************************"<<endl;
  fSession->SetEvents(nEvents);

  DTracker *tTracker  =  fSession->GetTracker();
  Int_t nPlanes = tTracker->GetPlanesN();
  DPlane* tPlane;

  // Canvas for Raw Data map
  TCanvas **cimage = new TCanvas*[nPlanes];
  TH2D **hRDImage = new TH2D*[nPlanes];
  TImage **image = new TImage*[nPlanes];
  TObject* g;
  Double_t nBinsX = 0, nBinsY = 0;
  Char_t name[50], title[300];
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // Loop on planes

    if( iPlane != planeToPlot ) continue;

    tPlane = tTracker->GetPlane(iPlane);
    nBinsX = tPlane->GetStripsNu();
    nBinsY = tPlane->GetStripsNv();

    // Canvas
    sprintf(name, "cimagepl%d", iPlane);
    sprintf(title,"Image plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    g = gROOT->FindObject(name) ;
    if (g) {
      cimage[iPlane-1] = (TCanvas*)g;
    }
    else {
      //cimage[iPlane-1] = new TCanvas(name, title, 5+20*(iPlane-1), 5+20*(iPlane-1), 700, 700*(Float_t)nBinsY/nBinsX);
      if (data=="hit"||data=="hits")    {
        cimage[iPlane-1] = new TCanvas(name, title, NBins*nBinsX+4, NBins*nBinsY+28);
        cimage[iPlane-1]->SetWindowSize(NBins*nBinsX+4, NBins*nBinsY+28);
      }
      else    {
        cimage[iPlane-1] = new TCanvas(name, title, NBins*nBinsX+4, NBins*nBinsY+28);
        cimage[iPlane-1]->SetWindowSize(NBins*nBinsX+4, NBins*nBinsY+28);
      }
      //+4 and +28 seems to be a systematic offset when testing the code. It is here to have the right values... Need more data to confirm these values
      cimage[iPlane-1]->SetFixedAspectRatio();
    }
    //cimage[iPlane-1]->Clear();
    //cimage[iPlane-1]->UseCurrentStyle();

    // Histo with raw data
    sprintf(name, "hrdimagepl%d", iPlane);
    sprintf(title,"Image raw data plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    g = gROOT->FindObject(name);
    if (g) {
      hRDImage[iPlane-1] = (TH2D*)g;
    }
    else {
      if (data=="hit"||data=="hits")    {
        hRDImage[iPlane-1] = new TH2D(name, title, NBins*nBinsX, -tPlane->GetStripPitch()(0)*tPlane->GetStripsNu()/2., tPlane->GetStripPitch()(0)*tPlane->GetStripsNu()/2., NBins*nBinsY, -tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2., tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2.);
      }
      else    {
        hRDImage[iPlane-1] = new TH2D(name, title, NBins*nBinsX, 0, nBinsX, NBins*nBinsY, 0, nBinsY);
      }
      for ( Int_t iX=1; iX<=NBins*nBinsX; iX++) {
        for ( Int_t iY=1; iY<=NBins*nBinsY; iY++) {
          hRDImage[iPlane-1]->SetBinContent( iX, iY, 0);
        }
      }
    }
    hRDImage[iPlane-1]->SetStats(kFALSE);
    //hRDImage[iPlane-1]->Reset();

    // Image
    sprintf( name, "imagepl%d", iPlane);
    g = gROOT->FindObject(name) ;
    if (g) {
      image[iPlane-1] = (TImage*)g;
    }
    else {
      image[iPlane-1] = TImage::Create();
      image[iPlane-1]->SetName(name);
    }

  }  // end Loop on planes

  //Loop over the requested number of events
  vector<DPixel*> *aList;
  for( Int_t iEvt=0; iEvt < nEvents; iEvt++) {
    if( !fSession->NextRawEvent() ) break; // Stop when no more events to read, JB 2011/07/08
    tTracker->Update();

    if (data=="hit"||data=="hits")    {//HIT IMAGE
      DHit *aHit;
      Int_t iCol, iRow;
      for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
        if( iPlane != planeToPlot ) continue;

        tPlane = tTracker->GetPlane(iPlane);

        if( tPlane->GetHitsN()>0 ) { // if some hits present
          for( Int_t iHit=1; iHit<=tPlane->GetHitsN(); iHit++) { //loop on hits (starts at 1 !!)
            aHit = (DHit*)tPlane->GetHit( iHit);
            iCol = aHit->GetIndexSeed()%tPlane->GetStripsNu();
            iRow = aHit->GetIndexSeed()/tPlane->GetStripsNu();
            if (flip=="X")    {
              hRDImage[iPlane-1]->Fill( -tPlane->PlaneToTracker(*(aHit->GetPosition()))(0), tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), 1);
            }

            else if (flip=="Y")    {
              hRDImage[iPlane-1]->Fill( tPlane->PlaneToTracker(*(aHit->GetPosition()))(0), -tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), 1);
            }

            else if (flip=="XY")    {
              hRDImage[iPlane-1]->Fill( -tPlane->PlaneToTracker(*(aHit->GetPosition()))(0), -tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), 1);
            }

            else    {
              hRDImage[iPlane-1]->Fill( tPlane->PlaneToTracker(*(aHit->GetPosition()))(0), tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), 1);
            }
            //hRDImage[iPlane-1]->Fill(iCol, iRow, 1);
          }
        } // end if some hits present
      } //end loop on planes
    } // Raw

    else    {//RAW DATA
      //    DHit *aHit;
      Double_t iCol, iRow, iVal;
      for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
        if( iPlane != planeToPlot ) continue;

        tPlane = tTracker->GetPlane(iPlane);

        if ( tPlane->GetReadout() >= 100){ // if sparsified readout

          aList = tPlane->GetListOfPixels();

          for( Int_t iPix=0; iPix<(Int_t)aList->size(); iPix++) { // loop on fired pixels
            iCol = aList->at(iPix)->GetPixelColumn();
            iRow = aList->at(iPix)->GetPixelLine();
            iVal = aList->at(iPix)->GetPulseHeight();
            if( aList->at(iPix)->GetPulseHeight()>minValue ) {
              //                          hRDImage[iPlane-1]->SetBinContent(iCol,iRow, hRDImage[iPlane-1]->GetBinContent(iCol,iRow) + aList->at(iPix)->GetPulseHeight());
              if (flip=="X")    {
                hRDImage[iPlane-1]->Fill(nBinsX-iCol, iRow, iVal);
              }
              else if (flip=="Y")    {
                hRDImage[iPlane-1]->Fill(iCol, nBinsY-iRow, iVal);
              }
              else if (flip=="XY")    {
                hRDImage[iPlane-1]->Fill(nBinsX-iCol, nBinsY-iRow, iVal);
              }
              else    {
                hRDImage[iPlane-1]->Fill(iCol, iRow, iVal);
              }
            }
          } //end loop on fired pixels
        } //end of sparsified readout

        else if ( tPlane->GetReadout() < 100 && tPlane->GetReadout() > 0 ){ // non-sparsified readout

          aList = tPlane->GetListOfPixels();
          for( Int_t iPix=0; iPix<(Int_t)aList->size(); iPix++) { // loop on fired pixels
            iCol = aList->at(iPix)->GetPixelColumn();
            iRow = aList->at(iPix)->GetPixelLine();
            iVal = aList->at(iPix)->GetPulseHeight();
            if( aList->at(iPix)->GetPulseHeight()>minValue ) {
              if (flip=="X")    {
                hRDImage[iPlane-1]->Fill(nBinsX-iCol, iRow, iVal);
              }
              else if (flip=="Y")    {
                hRDImage[iPlane-1]->Fill(iCol, nBinsY-iRow, iVal);
              }
              else if (flip=="XY")    {
                hRDImage[iPlane-1]->Fill(nBinsX-iCol, nBinsY-iRow, iVal);
              }
              else    {
                hRDImage[iPlane-1]->Fill(iCol, iRow, iVal);
              }
            }
          } //end loop on strips
        } //end of non-sparsified readout
      } //end of loop on planes
    } //End hits Image
  }

  //fSession->GetDataAcquisition()->PrintStatistics();
  //tTracker->PrintStatistics();

  for ( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {

    if( iPlane != planeToPlot ) continue;

    cimage[iPlane-1]->cd();

    hRDImage[iPlane-1]->SetTitle("");
    hRDImage[iPlane-1]->GetYaxis()->SetTitle("");
    hRDImage[iPlane-1]->GetYaxis()->SetTitleSize(0.0);
    hRDImage[iPlane-1]->GetYaxis()->SetTitleSize(0.0);
    hRDImage[iPlane-1]->GetYaxis()->SetTickLength(0.0);
    hRDImage[iPlane-1]->GetYaxis()->SetTitleOffset(1.5);
    hRDImage[iPlane-1]->GetYaxis()->SetLabelSize(0.0);
    hRDImage[iPlane-1]->GetXaxis()->TAxis::SetTickLength(0.0);
    hRDImage[iPlane-1]->GetXaxis()->SetTitle("");
    hRDImage[iPlane-1]->GetXaxis()->SetTitleSize(0.0);
    hRDImage[iPlane-1]->GetXaxis()->SetLabelSize(0.0);
    hRDImage[iPlane-1]->GetZaxis()->SetLabelSize(0.0);//Palette Label size
    hRDImage[iPlane-1]->GetZaxis()->Delete();//Palette Label size
    gStyle->SetTitleFontSize(0);
    gStyle->SetOptStat(0);
    gPad->SetMargin(0,0,0,0);

    if (greyScale)    {
      gStyle->SetPalette(52);//kGreyScale
    }
    else {
      gStyle->SetPalette(1);
    }

    gStyle->SetFrameBorderSize(0);
    //gPad->SetLogz();
    hRDImage[iPlane-1]->Draw("COL2");
    /*
     // Generate images, display and save
     cimage[iPlane-1]->cd();
     //hRDImage[iPlane-1]->Draw("COL2");
     sprintf( title, "Results/%d/image_Pl%d_%s%s_%devents_Binning%.1f.%s", fSession->GetRunNumber(), iPlane, data.Data(), flip.Data(), nEvents, NBins, format.Data());
     cimage[iPlane-1]->Print(title);
     printf("\n ==> Image saved in %s\n", title);
     sprintf( title, "Results/%d/image_Pl%d_%s%s_%devents_Binning%.1f.txt", fSession->GetRunNumber(), iPlane, data.Data(), flip.Data(), nEvents, NBins);
     printf("\n ==> Image saved in text format %s\n", title);
     std::ofstream file_txt(title);
     TString command;
     command = long(nBinsX) + TString("   ") + long(nBinsY) + TString("   ") + long(nEvents);
     file_txt << command.Data() << std::endl;
     for(int ix=0;ix<nBinsX;ix++) {
     for(int iy=0;iy<nBinsY;iy++) {
     command = long(ix) + TString("   ") + long(iy) + TString("   ") + long(hRDImage[iPlane-1]->GetBinContent(ix+1,iy+1));
     file_txt << command.Data() << std::endl;
     }
     }
     file_txt.close();*/
  }
  cimage[planeToPlot-1]->cd();
  if (displayImage) {
    hRDImage[planeToPlot-1]->Draw("COL2");
    cout << "DISPLAY DONE" << endl;
  }
  if (saveImage) {
    sprintf( title, "Results/%d/image_Pl%d_%s%s_%devents_Binning%.1f.%s", fSession->GetRunNumber(), planeToPlot, data.Data(), flip.Data(), nEvents, NBins, format.Data());
    if( format=="tif" || format=="tiff" ) {
      unsigned int nBits = 8;
      int maxVal = pow( 2, nBits) - 1;
      double maxBinContent = hRDImage[planeToPlot-1]->GetMaximum();
      double scale = 1.;
//      if ( maxBinContent > maxVal ) {
        scale = maxVal / maxBinContent;
//      }
      printf( "maxVal = %d,  maxContent = %.1f, scale = %.1f\n", maxVal, maxBinContent, scale);
      unsigned int nPixelsX = hRDImage[planeToPlot-1]->GetNbinsX();
      unsigned int nPixelsY = hRDImage[planeToPlot-1]->GetNbinsY();
      unsigned char pixelValues[ nPixelsX * nPixelsY];
      unsigned int iPix = 0;
      double val;
//      for( unsigned int iy=nPixelsY-1; iy>-1; iy-- ) {
      for( unsigned int iy=0; iy<nPixelsY; iy++ ) {
        for( unsigned int ix=0; ix<nPixelsX; ix++ ) {
          val = hRDImage[planeToPlot-1]->GetBinContent( ix, iy) * scale;
          pixelValues[iPix] = (unsigned char)val; // (maxVal-val)
//          printf( " pix %d (%d,%d) = %.1f -(scale)-> %.1f -> %d\n", iPix, ix, iy, hRDImage[planeToPlot-1]->GetBinContent( ix, iy), val, pixelValues[iPix]);
          iPix++;
        }
      }
      save_tiff_file( pixelValues, hRDImage[planeToPlot-1]->GetNbinsX(), hRDImage[planeToPlot-1]->GetNbinsY(), 0.00207, nBits, title);
    }
    else {
      cimage[planeToPlot-1]->Print(title);
    }
    printf("\n ==> Image saved in %s\n", title);
  }

}


/*void MRaw::DisplayImage( Int_t nEvents, TString data, TString format, Float_t minValue, Float_t NBins, Bool_t greyScale, TString flip)	{
    // Display an image of the matrix cumulated over nEvents.
    // If data="raw", plot pixel raw data
    // If data="hit", plot hit
    //
    // Save images (one per existing planes) in the given format,
    //  could be gif, jpg, png, tiff
    //
    // JB 2014/06/02
    // Last modified: JH 2016 May the 4th (be with you) : Display/Save images with the size corresponding to the matrix. Data type used. Addition of Greyscale mode for X-Rays. Addition of possibility to flip the results



    gROOT->SetBatch();// Must be set like that to prevent images to be resized to the scrren's resolution =)
    cout<<"Batch Mode Activated !"<<endl;
    cout<<"*********************************************************"<<endl;
    cout<<"******             Generating Images               ******"<<endl;

    if (data=="hit"||data=="hits")	{
      cout<<"******                   HIT MAP                   ******"<<endl;
    }
    else	{
      cout<<"******                  RAW DATA                   ******"<<endl;
      cout<<"******     (IF HITS WANTED : CHECK PARAMETERS)     ******"<<endl;
    }

    if (flip=="X"||flip=="Y"||flip=="XY")	{
      if (flip=="X"||flip=="Y")	{
	cout<<"******                   FLIP "<<flip<<"                    ******"<<endl;
      }
      else	{
	cout<<"******                   FLIP "<<flip<<"                   ******"<<endl;
      }
    }
    else	{
      //NOTHING
    }
    cout<<"*********************************************************"<<endl;
    fSession->SetEvents(nEvents);

    DTracker *tTracker  =  fSession->GetTracker();
    Int_t nPlanes = tTracker->GetPlanesN();
    DPlane* tPlane;

    // Canvas for Raw Data map
    TCanvas **cimage = new TCanvas*[nPlanes];
    TH2D **hRDImage = new TH2D*[nPlanes];
    TImage **image = new TImage*[nPlanes];
    TObject* g;
    Double_t nBinsX = 0, nBinsY = 0;
    Char_t name[50], title[100];
    for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // Loop on planes
        tPlane = tTracker->GetPlane(iPlane);
        nBinsX = tPlane->GetStripsNu();
        nBinsY = tPlane->GetStripsNv();

        // Canvas
        sprintf(name, "cimagepl%d", iPlane);
        sprintf(title,"Image plane %d - %s", iPlane, tPlane->GetPlanePurpose());
        g = gROOT->FindObject(name) ;
        if (g) {
            cimage[iPlane-1] = (TCanvas*)g;
        }
        else {
            //cimage[iPlane-1] = new TCanvas(name, title, 5+20*(iPlane-1), 5+20*(iPlane-1), 700, 700*(Float_t)nBinsY/nBinsX);
	    if (data=="hit"||data=="hits")	{
	      cimage[iPlane-1] = new TCanvas(name, title, NBins*nBinsX+4, NBins*nBinsY+28);
	      cimage[iPlane-1]->SetWindowSize(NBins*nBinsX+4, NBins*nBinsY+28);
	    }
	    else	{
	      cimage[iPlane-1] = new TCanvas(name, title, NBins*nBinsX+4, NBins*nBinsY+28);
	      cimage[iPlane-1]->SetWindowSize(NBins*nBinsX+4, NBins*nBinsY+28);
	    }
            //+4 and +28 seems to be a systematic offset when testing the code. It is here to have the right values... Need more data to confirm these values
            cimage[iPlane-1]->SetFixedAspectRatio();
        }
        //cimage[iPlane-1]->Clear();
        //cimage[iPlane-1]->UseCurrentStyle();

        // Histo with raw data
        sprintf(name, "hrdimagepl%d", iPlane);
        sprintf(title,"Image raw data plane %d - %s", iPlane, tPlane->GetPlanePurpose());
        g = gROOT->FindObject(name);
        if (g) {
            hRDImage[iPlane-1] = (TH2D*)g;
        }
        else {
	    if (data=="hit"||data=="hits")	{
	    hRDImage[iPlane-1] = new TH2D(name, title, NBins*nBinsX, -tPlane->GetStripPitch()(0)*tPlane->GetStripsNu()/2., tPlane->GetStripPitch()(0)*tPlane->GetStripsNu()/2., NBins*nBinsY, -tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2., tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2.);
	    }
	    else	{
	    hRDImage[iPlane-1] = new TH2D(name, title, NBins*nBinsX, 0, nBinsX, NBins*nBinsY, 0, nBinsY);
	    }
	    for ( Int_t iX=1; iX<=NBins*nBinsX; iX++) {
                for ( Int_t iY=1; iY<=NBins*nBinsY; iY++) {
                    hRDImage[iPlane-1]->SetBinContent( iX, iY, 0);
                }
            }
        }
        hRDImage[iPlane-1]->SetStats(kFALSE);
        //hRDImage[iPlane-1]->Reset();

        // Image
        sprintf( name, "imagepl%d", iPlane);
        g = gROOT->FindObject(name) ;
        if (g) {
            image[iPlane-1] = (TImage*)g;
        }
        else {
            image[iPlane-1] = TImage::Create();
            image[iPlane-1]->SetName(name);
        }

    }  // end Loop on planes

    //Loop over the requested number of events
    vector<DPixel*> *aList;
    for( Int_t iEvt=0; iEvt < nEvents; iEvt++) {
        if( !fSession->NextRawEvent() ) break; // Stop when no more events to read, JB 2011/07/08
        tTracker->Update();

        if (data=="hit"||data=="hits")    {//HIT IMAGE
            DHit *aHit;
            Int_t iCol, iRow;
            for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
                tPlane = tTracker->GetPlane(iPlane);

                if( tPlane->GetHitsN()>0 ) { // if some hits present
                    for( Int_t iHit=1; iHit<=tPlane->GetHitsN(); iHit++) { //loop on hits (starts at 1 !!)
                        aHit = (DHit*)tPlane->GetHit( iHit);
                        iCol = aHit->GetIndexSeed()%tPlane->GetStripsNu();
                        iRow = aHit->GetIndexSeed()/tPlane->GetStripsNu();
			if (flip=="X")	{
			  hRDImage[iPlane-1]->Fill( -tPlane->PlaneToTracker(*(aHit->GetPosition()))(0), tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), 1);
			}

			else if (flip=="Y")	{
			  hRDImage[iPlane-1]->Fill( tPlane->PlaneToTracker(*(aHit->GetPosition()))(0), -tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), 1);
			}

			else if (flip=="XY")	{
			  hRDImage[iPlane-1]->Fill( -tPlane->PlaneToTracker(*(aHit->GetPosition()))(0), -tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), 1);
			}

			else	{
                        hRDImage[iPlane-1]->Fill( tPlane->PlaneToTracker(*(aHit->GetPosition()))(0), tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), 1);
			}
                        //hRDImage[iPlane-1]->Fill(iCol, iRow, 1);
                    }
                } // end if some hits present
            } //end loop on planes
	} // Raw

        else    {//RAW DATA
	  //    DHit *aHit;
            Double_t iCol, iRow, iVal;
            for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
                tPlane = tTracker->GetPlane(iPlane);

                if ( tPlane->GetReadout() >= 100){ // if sparsified readout

                    aList = tPlane->GetListOfPixels();

                    for( Int_t iPix=0; iPix<(Int_t)aList->size(); iPix++) { // loop on fired pixels
                        iCol = aList->at(iPix)->GetPixelColumn();
                        iRow = aList->at(iPix)->GetPixelLine();
                        iVal = aList->at(iPix)->GetPulseHeight();
                        if( aList->at(iPix)->GetPulseHeight()>minValue ) {
  //                          hRDImage[iPlane-1]->SetBinContent(iCol,iRow, hRDImage[iPlane-1]->GetBinContent(iCol,iRow) + aList->at(iPix)->GetPulseHeight());
			  if (flip=="X")	{
			    hRDImage[iPlane-1]->Fill(nBinsX-iCol, iRow, iVal);
			  }
			  else if (flip=="Y")	{
			    hRDImage[iPlane-1]->Fill(iCol, nBinsY-iRow, iVal);
			  }
			  else if (flip=="XY")	{
			    hRDImage[iPlane-1]->Fill(nBinsX-iCol, nBinsY-iRow, iVal);
			  }
			  else	{
                            hRDImage[iPlane-1]->Fill(iCol, iRow, iVal);
			  }
                        }
                    } //end loop on fired pixels
                } //end of sparsified readout

                else if ( tPlane->GetReadout() < 100 && tPlane->GetReadout() > 0 ){ // non-sparsified readout

                    aList = tPlane->GetListOfPixels();
                    for( Int_t iPix=0; iPix<(Int_t)aList->size(); iPix++) { // loop on fired pixels
                        iCol = aList->at(iPix)->GetPixelColumn();
                        iRow = aList->at(iPix)->GetPixelLine();
                        iVal = aList->at(iPix)->GetPulseHeight();
                        if( aList->at(iPix)->GetPulseHeight()>minValue ) {
			  if (flip=="X")	{
			    hRDImage[iPlane-1]->Fill(nBinsX-iCol, iRow, iVal);
			  }
			  else if (flip=="Y")	{
			    hRDImage[iPlane-1]->Fill(iCol, nBinsY-iRow, iVal);
			  }
			  else if (flip=="XY")	{
			    hRDImage[iPlane-1]->Fill(nBinsX-iCol, nBinsY-iRow, iVal);
			  }
			  else	{
                            hRDImage[iPlane-1]->Fill(iCol, iRow, iVal);
			  }
                        }
                    } //end loop on strips
                } //end of non-sparsified readout
            } //end of loop on planes
        } //End hits Image
    }

    //fSession->GetDataAcquisition()->PrintStatistics();
    //tTracker->PrintStatistics();

    for ( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {


      cimage[iPlane-1]->cd();

      hRDImage[iPlane-1]->SetTitle("");
      hRDImage[iPlane-1]->GetYaxis()->SetTitle("");
      hRDImage[iPlane-1]->GetYaxis()->SetTitleSize(0.0);
      hRDImage[iPlane-1]->GetYaxis()->SetTitleSize(0.0);
      hRDImage[iPlane-1]->GetYaxis()->SetTickLength(0.0);
      hRDImage[iPlane-1]->GetYaxis()->SetTitleOffset(1.5);
      hRDImage[iPlane-1]->GetYaxis()->SetLabelSize(0.0);
      hRDImage[iPlane-1]->GetXaxis()->TAxis::SetTickLength(0.0);
      hRDImage[iPlane-1]->GetXaxis()->SetTitle("");
      hRDImage[iPlane-1]->GetXaxis()->SetTitleSize(0.0);
      hRDImage[iPlane-1]->GetXaxis()->SetLabelSize(0.0);
      hRDImage[iPlane-1]->GetZaxis()->SetLabelSize(0.0);//Palette Label size
      hRDImage[iPlane-1]->GetZaxis()->Delete();//Palette Label size
      gStyle->SetTitleFontSize(0);
      gStyle->SetOptStat(0);
      gPad->SetMargin(0,0,0,0);

      if (greyScale)    {
	  gStyle->SetPalette(52);//kGreyScale
      }
      else {
	  gStyle->SetPalette(1);
      }

      gStyle->SetFrameBorderSize(0);
      //gPad->SetLogz();
      hRDImage[iPlane-1]->Draw("COL2");

      // Generate images, display and save
      cimage[iPlane-1]->cd();
      //hRDImage[iPlane-1]->Draw("COL2");
      sprintf( title, "Results/%d/image_Pl%d_%s%s_%devents_Binning%.1f.%s", fSession->GetRunNumber(), iPlane, data.Data(), flip.Data(), nEvents, NBins, format.Data());
      cimage[iPlane-1]->Print(title);
      printf("\n ==> Image saved in %s\n", title);
      sprintf( title, "Results/%d/image_Pl%d_%s%s_%devents_Binning%.1f.txt", fSession->GetRunNumber(), iPlane, data.Data(), flip.Data(), nEvents, NBins);
      printf("\n ==> Image saved in text format %s\n", title);
      std::ofstream file_txt(title);
      TString command;
      command = long(nBinsX) + TString("   ") + long(nBinsY) + TString("   ") + long(nEvents);
      file_txt << command.Data() << std::endl;
      for(int ix=0;ix<nBinsX;ix++) {
	  for(int iy=0;iy<nBinsY;iy++) {
	      command = long(ix) + TString("   ") + long(iy) + TString("   ") + long(hRDImage[iPlane-1]->GetBinContent(ix+1,iy+1));
	      file_txt << command.Data() << std::endl;
	  }
      }
      file_txt.close();
    }
}*/

//______________________________________________________________________________
//
void MRaw::StudyTrackMultiplicity( TH1F *hNTracksPerPlanes, TH1F *hNHitsPerTrack, Int_t nEvents){

  // Evaluate the detection efficiency of each plane from the distribution
  //  of the nb of tracks reconstructed in each plane (hNTracksPerPlanes).
  //
  // The check the compatibility with the distribution of the nb of hits
  //  per track.
  //
  // Created JB 2014/11/23

  DTracker *tTracker  =  (DTracker*)fSession->GetTracker();
  Int_t nPlanes = tTracker->GetPlanesN();
  Int_t nPlanesInTracking = nPlanes-tTracker->GetNDUT();

  // Test there is enough planes and statistics to compute meaningful info
  if( nPlanesInTracking<2 || hNTracksPerPlanes->GetEntries()<10) {
    return;
  }
  printf("StudyTrackMultiplicity: #plane used in tracking min=%d, max=%d.\n", tTracker->GetRequiredHits(), nPlanesInTracking);

  //Double_t *efficiencyPlane   = new Double_t[nPlanes];
  //Double_t *trackMultiplicity = new Double_t[nPlanes+1];
  Double_t efficiencyPlane[nPlanesInTracking];
  Double_t trackMultiplicity[nPlanesInTracking+1];
  Int_t planeIDList[nPlanesInTracking];

  // Get the det. eff. for each plane
  // !!! This computation cannot be accurate because it assumes that
  //  all track multiplicities are kept, while there is a minimum !!!
  Int_t planeNb = 0;
  for (Int_t iBin=1; iBin<=hNTracksPerPlanes->GetNbinsX(); iBin++) {
    Int_t planeID = hNTracksPerPlanes->GetBinCenter(iBin);
    if( 1<=planeID && planeID<=nPlanes && tTracker->GetPlane(planeID)->GetStatus()!=3 ) {
      efficiencyPlane[planeNb] = hNTracksPerPlanes->GetBinContent(iBin)/nEvents;
      planeIDList[planeNb] = planeID;
      printf("StudyTrackMultiplicity: plane[%d] ID=%d, has %.0f tracks over %d total tracks => det. eff = %.1f\%\n", planeNb, planeID, hNTracksPerPlanes->GetBinContent(iBin), nEvents, efficiencyPlane[planeNb]*100.);
      planeNb++;
    }
  }

  // Compute the expected track multiplicity from the efficiencies
  fTool.TrackMultiplicity( nPlanesInTracking, efficiencyPlane, trackMultiplicity);

  // Formulae giving the track multiplicity
  //  assuming all planes have same detection efficiency
  TF1* fNhitsPerTrack = new TF1("fnhitspertrack","pow([1],x)*pow(1-[1],[0]-x)*TMath::Factorial([0])/TMath::Factorial(x)/TMath::Factorial([0]-x)",0, nPlanes);
  fNhitsPerTrack->SetParameters( nPlanesInTracking, .84);
  fNhitsPerTrack->SetNpx( nPlanes+1);
  fNhitsPerTrack->SetLineColor(2);
  fNhitsPerTrack->SetMarkerColor(2);
  fNhitsPerTrack->SetMarkerStyle(24);
  // Histogram related to previous function
  TH1F *hPredictedMultiplicitySingle = new TH1F( *hNHitsPerTrack);
  hPredictedMultiplicitySingle->SetName("hpredictedmultiplicitysingle");
  hPredictedMultiplicitySingle->SetTitle("Predicted track multiplicity with single efficiency");
  hPredictedMultiplicitySingle->SetMarkerStyle(24);
  hPredictedMultiplicitySingle->SetMarkerColor(2);



  // Book some histos and display

  TH1F *hDetEfficiency = new TH1F( "hdetefficiency", "Plane detection efficiency; plane id; det. efficiency \%", nPlanesInTracking, 0.5, nPlanesInTracking+.5);
  hDetEfficiency->SetStats(0);
  hDetEfficiency->SetMinimum(0.);
  hDetEfficiency->SetMaximum(110.);
  hDetEfficiency->SetMarkerStyle(20);

  // copy initial multiplicity distribution and normalize it
  TH1F *hTrackMultiplicity = (TH1F*)hNHitsPerTrack->Clone("htrackmultiplicity");
  hTrackMultiplicity->Scale( 1./hNHitsPerTrack->GetEntries() );

  TH1F *hPredictedMultiplicity = new TH1F( *hNHitsPerTrack);
  hPredictedMultiplicity->SetName("hpredictedmultiplicity");
  hPredictedMultiplicity->SetTitle("Predicted track multiplicity");
  hPredictedMultiplicity->SetMarkerStyle(20);

  for (Int_t iPlane=0; iPlane<=nPlanesInTracking; iPlane++) {
    if(iPlane<nPlanesInTracking) {
      hDetEfficiency->SetBinContent( planeIDList[iPlane], efficiencyPlane[iPlane]*100.);
    }
    hPredictedMultiplicity->SetBinContent( iPlane+1, trackMultiplicity[iPlane]);
    printf("StudyTrackMultiplicity: tracks with multiplicity %d = %.1e expected %.1e (%.1e)\n", iPlane, hTrackMultiplicity->GetBinContent(iPlane+1), trackMultiplicity[iPlane], fNhitsPerTrack->Eval(iPlane));
  }

  TCanvas *ctrackmult;
  TObject* g = gROOT->FindObject("ctrackmult") ;
  if (g) {
    ctrackmult = (TCanvas*)g;
  }
  else {
    ctrackmult = new TCanvas("ctrackmult", "Track multiplicity styudy", 100, 100,900,600);
  }
  ctrackmult->Clear();
  ctrackmult->Divide(2, 1);

  ctrackmult->cd(1);
  hDetEfficiency->Draw("p");
  gPad->SetGridy();

  ctrackmult->cd(2);
  hTrackMultiplicity->Draw("");
  gStyle->SetOptFit(1);
  fNhitsPerTrack->SetRange( tTracker->GetRequiredHits(), nPlanesInTracking);
  fNhitsPerTrack->FixParameter(0, nPlanesInTracking);
  fNhitsPerTrack->SetParLimits( 1, 0.01, .99);
  hTrackMultiplicity->Fit( fNhitsPerTrack, "NR");
  Double_t estimatedSingleDetEff     = fNhitsPerTrack->GetParameter(1);
  Double_t estimatedSingleDetEff_err = fNhitsPerTrack->GetParError(1);
  printf("StudyTrackMultiplicity: estimated det.eff. = %.2f +/- %.2f \%.\n", (estimatedSingleDetEff*100.), (estimatedSingleDetEff_err*100.));
  for (Int_t iPlane=0; iPlane<=nPlanesInTracking; iPlane++) {
    hPredictedMultiplicitySingle->SetBinContent( iPlane+1, fNhitsPerTrack->Eval(iPlane));
  }
  hPredictedMultiplicity->Draw("psame");
  hPredictedMultiplicitySingle->Draw("psame");
  TLegend *leg = new TLegend( 0.18,.75,.50,.90,"");
  leg->SetTextSize(.03);
  leg->SetFillColor(0);
  leg->AddEntry( hTrackMultiplicity, "Data", "l");
  leg->AddEntry( hPredictedMultiplicitySingle, "Estim. / same det.eff.", "p");
  leg->AddEntry( hPredictedMultiplicity, "Estim. / det.eff. per plane", "p");
  leg->Draw();

  return;

}

//______________________________________________________________________________
//
void MRaw::StudyTrackingEfficiency(Int_t nEvents, Double_t nHitsMax, Double_t Xmin, Double_t Xmax, Double_t Ymin, Double_t Ymax){
  // Created by QL, 2015/04/10
  // Semi-quantitative estimation of tracking efficiency.
  // This function produces the distribution of the ratio of #tracks/#hits per event for each plane in telescope.
  // This ratio should be a lower limit of the tracking efficiency in the case of a significant number of tracks/event.
  // nHitsMax is used to define the upper limit of nHits in each plane.
  // Xmin, Xmax, Ymin, Ymax: window in global reference frame. The default value is a very big window to include all the sensors.
  //
  fSession->SetEvents(nEvents);

  DTracker *tTracker  =  (DTracker*)fSession->GetTracker();
  Int_t nPlanes = tTracker->GetPlanesN();
  //Int_t nPlanesInTracking = nPlanes-tTracker->GetNDUT(); // If DUT Readout is set to 0, this plane is counted in GetPlanesN() but not in GetNDUT()
  Int_t nPlanesInTracking = 0;

  //---------- Calculate nPlanesInTracking ----------------
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    DPlane *tPlane = tTracker->GetPlane(iPlane);
    if(tPlane->GetStatus() <= 2){
      nPlanesInTracking++;
    }
  }//End Plane Loop
  //-------------------------------------------------------

  // Test there is enough planes and statistics to compute meaningful info
  if( nPlanesInTracking<2 ) {
    return;
  }
  printf("StudyTrackingEfficiency: #plane used in tracking min=%d, max=%d.\n", tTracker->GetRequiredHits(), nPlanesInTracking);
  Double_t NHitsInPlMax = fSession->GetSetup()->GetTrackerPar().HitsInPlaneMaximum;
  Int_t NBinsx = 21;
  Double_t xlow = -0.025;
  Double_t xup = 1.025;
  Double_t ylow = 0;
  Double_t yup;
  if(nHitsMax<=0)
       yup = NHitsInPlMax/5; // Set Maximum NHits from conf.
  else
       yup = nHitsMax;
  Int_t NBinsy = 20;//(yup-ylow > 400)?(yup-ylow)/20:100; // To be optimised
  // TH1F *h1TracksPerEvent = new TH1F("h1TracksPerEvent","Reconstructed Tracks per event", NBins, xlow, xup);
  // h1TracksPerEvent->SetXTitle("# tracks / Event");

  //============= Init Historgrams for each plane ==============
  TH2F **h2TrackingEffPerEvent = new TH2F*[nPlanes];
  Int_t *nTracksInPl = new Int_t[nPlanes];
  // 2D Hits and Tracks Map in Plane Local coordinates.
  TH2F **hHitMap = new TH2F*[nPlanes];
  TH2F **hTrackMap = new TH2F*[nPlanes];
  TH2F **hTrackHitMap = new TH2F*[nPlanes];
  TProfile **pNtracksOverNhits_VS_Nhits = new TProfile*[nPlanes];
  // This profile is similar to h2TrackingEffPerEvent[iPlane-1]->ProfileY()

  Char_t name[50], title[100];
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    DPlane *tPlane = tTracker->GetPlane(iPlane);
    // Nb of hits per event
    sprintf( name, "h2TrackingEffPerEventPl%d", iPlane);
    sprintf( title, "Tracking Efficiency per event, plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    h2TrackingEffPerEvent[iPlane-1] = new TH2F(name, title, NBinsx, xlow, xup, NBinsy, ylow, yup);
    h2TrackingEffPerEvent[iPlane-1]->SetXTitle("NTracks/NHits per Event");
    h2TrackingEffPerEvent[iPlane-1]->SetYTitle("NHits");
    h2TrackingEffPerEvent[iPlane-1]->SetTitleSize(0.04,"XYZ");
    h2TrackingEffPerEvent[iPlane-1]->SetLabelSize(0.04,"XYZ");
    nTracksInPl[iPlane-1] = 0; // Init elements in nTracksInPl
    // -- Profile for Ntracks/Nhits VS Nhits
    sprintf( name, "pNtracksOverNhits_vs_NhitsPl%d", iPlane);
    sprintf( title, "Ntracks/Nhits VS Nhits, plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    pNtracksOverNhits_VS_Nhits[iPlane-1] = new TProfile(name, title, NBinsy, ylow, yup, xlow, xup); // exchange x and y
    pNtracksOverNhits_VS_Nhits[iPlane-1]->SetXTitle("NHits");
    pNtracksOverNhits_VS_Nhits[iPlane-1]->SetYTitle("NTracks/NHits per Event");
    pNtracksOverNhits_VS_Nhits[iPlane-1]->SetTitleSize(0.04,"XYZ");
    pNtracksOverNhits_VS_Nhits[iPlane-1]->SetLabelSize(0.04,"XYZ");
    pNtracksOverNhits_VS_Nhits[iPlane-1]->SetLineColor(kBlue);
    pNtracksOverNhits_VS_Nhits[iPlane-1]->SetLineWidth(2.);

    // -- Histo for hits with microns in plane frame
    sprintf( name, "hhitmappl%d", iPlane);
    sprintf( title, "Hit map of plane (%d) %s", iPlane, tPlane->GetPlanePurpose());
    //hHitMap[iPlane-1] = new TH2F(name, title, 100, -tPlane->GetStripPitch()(0)*tPlane->GetStripsNu()/2., tPlane->GetStripPitch()(0)*tPlane->GetStripsNu()/2., 100, -tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2., tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2.);
    hHitMap[iPlane-1] = new TH2F(name, title, tPlane->GetStripsNu()/50, 0, tPlane->GetStripsNu(), tPlane->GetStripsNv()/50, 0, tPlane->GetStripsNv());
    hHitMap[iPlane-1]->SetMarkerStyle(20);
    hHitMap[iPlane-1]->SetMarkerSize(.6);
    hHitMap[iPlane-1]->SetMarkerColor(1);
    hHitMap[iPlane-1]->SetStats(kFALSE);
    // -- Histo for tracks with microns in plane frame
    sprintf( name, "htrackmappl%d", iPlane);
    sprintf( title, "Track(Matched Hit) map of plane (%d) %s", iPlane, tPlane->GetPlanePurpose());
    hTrackMap[iPlane-1] = new TH2F( *(hHitMap[iPlane-1])); // JB 2009/09/09
    hTrackMap[iPlane-1]->SetName( name);
    hTrackMap[iPlane-1]->SetTitle( title);
    hTrackMap[iPlane-1]->SetMarkerStyle(24);
    hTrackMap[iPlane-1]->SetMarkerSize(2.);
    hTrackMap[iPlane-1]->SetStats(kFALSE);
    // -- Histo for tracks with microns in plane frame
    sprintf( name, "Tracks/HitsMAPpl%d", iPlane);
    sprintf( title, "Tracks/Hits map of plane (%d) %s", iPlane, tPlane->GetPlanePurpose());
    hTrackHitMap[iPlane-1] = new TH2F( *(hHitMap[iPlane-1]));
    hTrackHitMap[iPlane-1]->SetName( name);
    hTrackHitMap[iPlane-1]->SetTitle( title);
    hTrackHitMap[iPlane-1]->SetNdivisions(505); // SetNdivisions for X axis
  }
  //=====================================================

  // ============= Event Loop ==========
  DTrack *aTrack;
  DPlane *aPlane;
  DHit   *aHit;
  Double_t col;
  Double_t lin;

  for(Int_t iEvt = 0; iEvt < nEvents; iEvt++){
    if( !(fSession->NextRawEvent()) ) break; // JB 2009/06/26

    // === Analysis ====
    tTracker->Update();
    Int_t nTracks = tTracker->GetTracksN();
    //    h1TracksPerEvent->Fill(nTracks);

    //    if(nTracks > 0){
    for( Int_t iTrack=1; iTrack<=nTracks; iTrack++) { // loop on tracks
      aTrack = (DTrack*)tTracker->GetTrack( iTrack);
      if(aTrack->GetHitsNumber()>0)
	for( Int_t iHit=0; iHit<aTrack->GetHitsNumber(); iHit++ ) {
	  aHit   = (DHit*)aTrack->GetHit(iHit);
	  aPlane = (DPlane*)aHit->GetPlane();
	  // hTrackMap[aPlane->GetPlaneNumber()-1]->Fill(aPlane->Intersection(aTrack)(0), aPlane->Intersection(aTrack)(1), aTrack->GetNumber()); // Check if it is 1 ?
	  //cout << aTrack->GetNumber() << endl << aTrack->GetHitsNumber() << endl;
	  //hTrackMap[aPlane->GetPlaneNumber()-1]->Fill(aHit->GetPositionUhit(), aHit->GetPositionVhit(),1);
	  DR3 positionGlobal = aPlane->PlaneToTracker(*aHit->GetPosition());
	  if(positionGlobal(0) < Xmax && positionGlobal(0) > Xmin && positionGlobal(1) < Ymax && positionGlobal(1) > Ymin){
	       nTracksInPl[aPlane->GetPlaneNumber()-1] += 1;
	       aPlane->ComputeStripPosition_UVToColRow(aHit->GetPositionUhit(), aHit->GetPositionVhit(), col, lin);
	       hTrackMap[aPlane->GetPlaneNumber()-1]->Fill(col,lin);
	  }
	}
    }

    for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
      DPlane *tPlane = tTracker->GetPlane(iPlane);
      Int_t nHits = tPlane->GetHitsN();
      Int_t nHitsInTriggerRegion = 0;
      if(nHits > 0){
	//if(nTracksInPl[iPlane-1]>0){
	//   if(iEvt % 500 == 5)
	//     cout<< "Pl" << iPlane << ": NTracksPl/NTracks = " << nTracksInPl[iPlane-1] << "/" << nTracks << " " << "NHits = " << nHits << " NTracksPl/NHits = " << (Float_t)nTracksInPl[iPlane-1] / nHits << endl;
	//	if(nTracksInPl[iPlane-1]>0)
	for(Int_t iHit=1; iHit<=nHits; iHit++){
	  aHit = (DHit*)tPlane->GetHit(iHit);
	  //hHitMap[iPlane-1]->Fill(aHit->GetPositionUhit(),aHit->GetPositionVhit(), 1);
	  DR3 positionGlobal = tPlane->PlaneToTracker(*aHit->GetPosition());
	  if(positionGlobal(0) < Xmax && positionGlobal(0) > Xmin && positionGlobal(1) < Ymax && positionGlobal(1) > Ymin){
	       tPlane->ComputeStripPosition_UVToColRow(aHit->GetPositionUhit(), aHit->GetPositionVhit(), col, lin);
	       hHitMap[iPlane-1]->Fill(col,lin);
	       nHitsInTriggerRegion++;
	  }
	}
	if(nHitsInTriggerRegion > 0){
	     //h2TrackingEffPerEvent[iPlane-1]->Fill( (Float_t)nTracksInPl[iPlane-1] / nHits, nHits );
	     //pNtracksOverNhits_VS_Nhits[iPlane-1]->Fill(nHits, (Float_t)nTracksInPl[iPlane-1] / nHits );
	     h2TrackingEffPerEvent[iPlane-1]->Fill( (Float_t)nTracksInPl[iPlane-1] / nHitsInTriggerRegion, nHitsInTriggerRegion );
	     pNtracksOverNhits_VS_Nhits[iPlane-1]->Fill(nHitsInTriggerRegion, (Float_t)nTracksInPl[iPlane-1] / nHitsInTriggerRegion );
	}
      }//close for if
      nTracksInPl[iPlane-1] = 0; // Init elements in NTracksInPl for next event
    }//End Plane Loop
    //   } End if(nTracks > 0)
  }
  //============== End Event Loop =========

  //============== Show Plots ===============
  Int_t Index_c; // Index for canvas->cd() loop
  //Int_t nPlanesInTracking = 0; // Number of Telescope planes
  TCanvas *cTrackEff;
  TObject* g = gROOT->FindObject("cTrackEff") ;
  if (g) {
    cTrackEff = (TCanvas*)g;
  }
  else {
    cTrackEff = new TCanvas("cTrackEff", "NHits VS NTracks/NHits", 100, 100,900,600);
  }
  cTrackEff->Clear();
  cTrackEff->Divide((Int_t)((nPlanesInTracking+1)/2.),(nPlanesInTracking>1)?2:1); //nPlanesInTracking could be an odd number.
  Index_c = 1;
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    DPlane *tPlane = tTracker->GetPlane(iPlane);
    if(tPlane->GetStatus() <= 2){
      cTrackEff->cd(Index_c);
      h2TrackingEffPerEvent[iPlane-1]->Draw("colz");
      Index_c++;
    }
  }//End Plane Loop
  //nPlanesInTracking = Index_c - 1;

  TCanvas *cTrackEff1;
  TObject* g1 = gROOT->FindObject("cTrackEff1") ;
  if (g1) {
    cTrackEff1 = (TCanvas*)g1;
  }
  else {
    cTrackEff1 = new TCanvas("cTrackEff1", "NTracks/NHits VS NHits", 150, 150,900,600);
  }
  cTrackEff1->Clear();
  cTrackEff1->Divide((Int_t)((nPlanesInTracking+1)/2.),(nPlanesInTracking>1)?2:1);
  Index_c = 1;
  Float_t histo_YMin = 1.0;
  Float_t histo_YMax = 0;

  // ----- Loop all the histograms to set the same range in Y.
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    DPlane *tPlane = tTracker->GetPlane(iPlane);
    if(tPlane->GetStatus() <= 2){
	 Int_t Bin_max = pNtracksOverNhits_VS_Nhits[iPlane-1]->GetMaximumBin();
	 Int_t Bin_min = pNtracksOverNhits_VS_Nhits[iPlane-1]->GetMinimumBin(); // Not useful because of empty bins.
	 Float_t RealMinimum = 1.0;
	 //--------Get the real MinimumBin fo pNtracksOverNhits_VS_Nhits ------------
	 for(Int_t iBin = 1; iBin <= pNtracksOverNhits_VS_Nhits[iPlane-1]->GetNbinsX(); iBin++){
	      Float_t BinContent = pNtracksOverNhits_VS_Nhits[iPlane-1]->GetBinContent(iBin);
	      if(RealMinimum > BinContent && BinContent > 0.0){
		   RealMinimum = BinContent;
		   Bin_min = iBin;
	      }
	 }
	 histo_YMax = TMath::Max(histo_YMax, (Float_t)(pNtracksOverNhits_VS_Nhits[iPlane-1]->GetBinContent(Bin_max) + pNtracksOverNhits_VS_Nhits[iPlane-1]->GetBinError(Bin_max)));
	 histo_YMin = TMath::Min(histo_YMin, (Float_t)(pNtracksOverNhits_VS_Nhits[iPlane-1]->GetBinContent(Bin_min) - pNtracksOverNhits_VS_Nhits[iPlane-1]->GetBinError(Bin_min)));
    }
  }//End Plane Loop
  histo_YMax = TMath::Min(histo_YMax, (Float_t)1.0);
  histo_YMin = TMath::Max(histo_YMin, (Float_t)0.0);

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    DPlane *tPlane = tTracker->GetPlane(iPlane);
    if(tPlane->GetStatus() <= 2){
      cTrackEff1->cd(Index_c);
      pNtracksOverNhits_VS_Nhits[iPlane-1]->SetMaximum(histo_YMax);
      pNtracksOverNhits_VS_Nhits[iPlane-1]->SetMinimum(histo_YMin);
      pNtracksOverNhits_VS_Nhits[iPlane-1]->Draw();
      Index_c++;
    }
  }//End Plane Loop

  TCanvas *cTrackEff2;
  TObject* g2 = gROOT->FindObject("cTrackEff2") ;
  if (g2) {
    cTrackEff2 = (TCanvas*)g2;
  }
  else {
    cTrackEff2 = new TCanvas("cTrackEff2", "NTracks/NHits Study", 150, 150,900,600);
  }
  cTrackEff2->Clear();
  cTrackEff2->Divide((Int_t)((nPlanesInTracking+1)/2.),(nPlanesInTracking>1)?2:1);
  Index_c = 1;
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    DPlane *tPlane = tTracker->GetPlane(iPlane);
    if(tPlane->GetStatus() <= 2){
      cTrackEff2->cd(Index_c);
      h2TrackingEffPerEvent[iPlane-1]->ProjectionX()->Draw();
      Index_c++;
    }
  }//End Plane Loop

  TCanvas *cTrackEff3;
  TObject* g3 = gROOT->FindObject("cTrackEff3") ;
  if (g3) {
    cTrackEff3 = (TCanvas*)g3;
  }
  else {
    cTrackEff3 = new TCanvas("cTrackEff3", "NTracks/NHits in bins of columns and lines", 200, 200,900,600);
  }
  cTrackEff3->Clear();
  cTrackEff3->Divide((Int_t)((nPlanesInTracking+1)/2.),(nPlanesInTracking>1)?2:1);
  Index_c = 1;
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    DPlane *tPlane = tTracker->GetPlane(iPlane);
    if(tPlane->GetStatus() <= 2){
      cTrackEff3->cd(Index_c);
      //hHitMap[iPlane-1]->Draw();
      //hTrackMap[iPlane-1]->Draw("same");
      //if( hTrackHitMap[iPlane-1]->Divide(hTrackMap[iPlane-1], hHitMap[iPlane-1]) ){
      	//hTrackHitMap[iPlane-1]->Rebin2D(4,4);
      	hTrackHitMap[iPlane-1]->Draw("colz");
      //}
      Index_c++;
    }
  }//End Plane Loop

  TCanvas *cTrackEff4;
  TObject* g4 = gROOT->FindObject("cTrackEff4") ;
  if (g4) {
    cTrackEff4 = (TCanvas*)g4;
  }
  else {
    cTrackEff4 = new TCanvas("cTrackEff4", "Mean NTracks/NHits VS PlaneNumber", 200, 200,900,600);
  }
  cTrackEff4->Clear();
  TH1F * h1NtracksOverNhits_vs_plane = new TH1F("h1NtracksOverNhits_vs_plane","Mean nTracks/nHits for Telescope Planes", nPlanesInTracking, 0.5, nPlanesInTracking + 0.5);
  h1NtracksOverNhits_vs_plane->SetLineColor(kBlue);
  h1NtracksOverNhits_vs_plane->SetXTitle("Plane Number");
  h1NtracksOverNhits_vs_plane->SetYTitle("Mean Ntracks/Nhits");
  h1NtracksOverNhits_vs_plane->SetStats(kFALSE);
  h1NtracksOverNhits_vs_plane->SetTitleSize(0.04,"XYZ");
  h1NtracksOverNhits_vs_plane->SetLabelSize(0.04,"XYZ");

  Int_t iTelPlane = 1;
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    DPlane *tPlane = tTracker->GetPlane(iPlane);
    if(tPlane->GetStatus() <= 2){
	 Double_t mean = h2TrackingEffPerEvent[iPlane-1]->ProjectionX()->GetMean();
         Double_t RMS = h2TrackingEffPerEvent[iPlane-1]->ProjectionX()->GetRMS();
         h1NtracksOverNhits_vs_plane->SetBinContent(iTelPlane,mean);
         h1NtracksOverNhits_vs_plane->SetBinError(iTelPlane,RMS);
         TString label = "";
         label += iPlane;
         h1NtracksOverNhits_vs_plane->GetXaxis()->SetBinLabel(iTelPlane,label);
	 iTelPlane++;
    }
  }//End Plane Loop
  h1NtracksOverNhits_vs_plane->Draw();


  //============= Save Histograms ======================
  Char_t rootFile[300];
  sprintf(rootFile,"%s/TrackingEffStudy_run%d.root",fSession->GetResultDirName().Data(),fSession->GetRunNumber());
  sprintf(rootFile,"%s", fTool.LocalizeDirName( rootFile));
  cout << "\n-- Saving histos and canvas into " << rootFile << endl;
  TFile fRoot(rootFile,"RECREATE");
  for(Int_t iPlane=1; iPlane<=nPlanes; iPlane++){ // loop on planes
    DPlane * tPlane  = tTracker->GetPlane(iPlane);
    if(tPlane->GetStatus() <= 2){
      h2TrackingEffPerEvent[iPlane-1]->Write();
      pNtracksOverNhits_VS_Nhits[iPlane-1]->Write();
      hTrackHitMap[iPlane-1]->Write();
    };
  }//End Plane loop
  cTrackEff->Write();
  cTrackEff1->Write();
  cTrackEff2->Write();
  cTrackEff3->Write();
  h1NtracksOverNhits_vs_plane->Write();
  fRoot.Close();
  //===================================================

  return;
}

//______________________________________________________________________________
//
void MRaw::StudyMultipleScattering( Int_t nEvents, Int_t set0plane0, Int_t set0plane1, Int_t set1plane0, Int_t set1plane1 )
{
  // Study multiple scattering by building for each tracks the residual between
  //  the track slopes obtained from 2 fits using 2 set of 2 points.
  // The two sets are defined by indexes:
  //  - first set: set0plane0, set0plane1
  //  - second set: set1plane0, set1plane1
  //
  // PAY ATTENTION: these planes should not be identified as DUT (status: 3) !
  //
  // Uses subtrack which are track refit from a full track (see DSetup),
  //  if no subtrack setup was required, use normal full tracks.
  //
  // JB, 2015/08/21

  fSession->SetEvents(nEvents);

  DTracker *tTracker  =  (DTracker*)fSession->GetTracker();

  // Find subsets
  Int_t *planeSetIds[2];
  planeSetIds[0] = new Int_t[2];
  planeSetIds[0][0] = set0plane0;
  planeSetIds[0][1] = set0plane1;
  planeSetIds[1] = new Int_t[2];
  planeSetIds[1][0] = set1plane0;
  planeSetIds[1][1] = set1plane1;
  printf( "Will use 2 sets of planes: (%d-%d) and (%d-%d)\n", planeSetIds[0][0], planeSetIds[0][1], planeSetIds[1][0], planeSetIds[1][1] );
  DPlane *aPlane;

//  fSubTrackPlanesN = 2;
//  fSubTrackPlaneIds[0] = dutPlaneNb-1;
//  fSubTrackPlaneIds[1] = dutPlaneNb;

  // Book histograms
  Char_t name[50], title[100];
  TH1F *hSlopeResidualX = new TH1F("hsloperesidualx", "Slope residual in X", 100, -0.002, 0.002);
  TH1F *hSlopeResidualY = new TH1F("hsloperesidualy", "Slope residual in Y", 100, -0.002, 0.002);
  sprintf( title, "Distance between 2 frist planes %d-%d", planeSetIds[0][0], planeSetIds[0][1]);
  TH1F *hdistance[2];
  TH1F *hSetTrackSlopeX[2];
  TH1F *hSetTrackSlopeY[2];
  for (Int_t iset=0; iset<2; iset++) {
    sprintf( name, "hsettrackslopex%d", iset);
    sprintf( title, "Track slopes X=f(Z) for set %d;slope [rad]", iset);
    hSetTrackSlopeX[iset] = new TH1F( name, title, 100, -0.005, 0.005);
    hSetTrackSlopeX[iset]->SetLineColor(1+iset);
    sprintf( name, "hsettrackslopey%d", iset);
    sprintf( title, "Track slopes Y=f(Z) for set %d;slope [rad]", iset);
    hSetTrackSlopeY[iset] = new TH1F( name, title, 100, -0.005, 0.005);
    hSetTrackSlopeY[iset]->SetLineColor(1+iset);
    sprintf( name, "hdistance%d", iset);
    sprintf( title, "Distance between planes %d-%d of set %d;distance  [#mum]", planeSetIds[iset][0], planeSetIds[iset][1], iset);
    hdistance[iset] = new TH1F( name, title, 100, 0., 0.);

  }

  // Canvas for display
  TCanvas *cmultscat;
  TObject* g = gROOT->FindObject("cmultscat") ;
  if (g) {
    cmultscat = (TCanvas*)g;
  }
  else {
    cmultscat = new TCanvas("cmultscat", "Multiple scattering study", 5, 5,800,700);
  }
  cmultscat->Clear();
  cmultscat->UseCurrentStyle();
  TPaveLabel* label = new TPaveLabel();
  Char_t canvasTitle[200];
  TPad *pad = new TPad("pad","",0.,0.,1.,0.965);
  pad->Draw();
  pad->Divide( 2, 3);

  sprintf(canvasTitle, "Run %d, cumul over %d events", fSession->GetRunNumber(), nEvents);
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);


  // === Event Loop ====

  DTrack *aTrack, *aSubTrack;
  DHit  **aHitList;
  DPrecAlign *hitAlignment;
  DR3 trackSlope, subtrackSlope[2], position;
  Int_t selectedTracks = 0;
  Double_t *zpos[2];
  zpos[0] = new Double_t[2];
  zpos[1] = new Double_t[2];

  //Loop over the requested number of events
  for( Int_t iEvt=0; iEvt < nEvents; iEvt++) {
    if( !(fSession->NextRawEvent()) ) break; // JB 2009/06/26

    // === Analysis ====
    tTracker->SetPlanesForSubTrack(0, NULL);
    tTracker->Update();
    Int_t nTracks = tTracker->GetTracksN();
    // =================

    if(fDebugRaw) cout << "evt " << iEvt << ": " << nTracks << " tracks to display" << endl;
    for( Int_t iTrack=1; iTrack<=nTracks; iTrack++) { // loop on tracks
      aTrack = (DTrack*)tTracker->GetTrack( iTrack);
      trackSlope = aTrack->GetLinearFit().GetSlopeZ();
      aHitList = (DHit**)aTrack->GetHitArray();
//      printf( "evt %6d trk %2d - %d hits, slopes( %.2e, %.2e)\n", iEvt, aTrack->GetNumber(), aTrack->GetHitsNumber(), trackSlope(0), trackSlope(1));

      // check the expected planes are used in the track
      Int_t foundPlanes = 0;
//      printf( "    planes:");
      for ( Int_t iHit=0; iHit<aTrack->GetHitsNumber(); iHit++) {
//        printf( "      testing pl id %d\n", aHitList[iHit]->GetPlane()->GetPlaneNumber());
        for (Int_t iset=0; iset<2; iset++) {
          for (Int_t iPlane=0; iPlane<2; iPlane++) {
            if( aHitList[iHit]->GetPlane()->GetPlaneNumber() == planeSetIds[iset][iPlane] ) {
              foundPlanes++;
              hitAlignment = aHitList[iHit]->GetPlane()->GetPrecAlignment();
              position = hitAlignment->TransformHitToTracker( *(aHitList[iHit]->GetPosition()) );
              zpos[iset][iPlane] = position(2);
//              printf( "     - found plane %d of set %d at z=%.2f\n", iPlane, iset, zpos[iset][iPlane]);
            }
          }
        }
      }
//      printf( " -> found = %d\n", foundPlanes);

      // Fit successively with the two sets
      if( foundPlanes == 4 ) {
//        tTracker->SetDebug(2);
        Int_t subTrackFitSuccess = 0;
        for (Int_t iset=0; iset<2; iset++) {
          tTracker->SetPlanesForSubTrack( 2, planeSetIds[iset]);
          tTracker->MakeSubTrack( aTrack, aHitList, aTrack->GetHitsNumber());
          subTrackFitSuccess++;
          aSubTrack = (DTrack*)(tTracker->GetSubTrack( aTrack->GetNumber()));
          subtrackSlope[iset] = aSubTrack->GetLinearFit().GetSlopeZ();
//          printf( "    set %d ( %.2e, %.2e) dist=%.2f\n", iset, subtrackSlope[iset](0), subtrackSlope[iset](1), zpos[iset][1]-zpos[iset][0]);
          hSetTrackSlopeX[iset]->Fill( subtrackSlope[iset](0));
          hSetTrackSlopeY[iset]->Fill( subtrackSlope[iset](1));
          hdistance[iset]->Fill(fabs(zpos[iset][1]-zpos[iset][0]));
        }
//        tTracker->SetDebug(0);

        if ( subTrackFitSuccess==2 ) {
          selectedTracks++;
          hSlopeResidualX->Fill( subtrackSlope[1](0) - subtrackSlope[0](0) );
          hSlopeResidualY->Fill( subtrackSlope[1](1) - subtrackSlope[0](1) );
        }
      }

    } //end loop on tracks

  } // END LOOP ON EVENTS
  printf("\n--> selected tracks for multiple scattering study: %d\n", selectedTracks);
  fSession->GetDataAcquisition()->PrintStatistics();
  tTracker->PrintStatistics();

  // Compute some expectation
  // Get first scattering
  aPlane = tTracker->GetPlane(planeSetIds[0][1]);
  DR3 firstPosition = aPlane->GetPosition();
  printf( "  first plane %d, z-position %.0f\n", planeSetIds[0][1], position(2) );
  Double_t expectedDeviation = aPlane->GetSigmaThetaMS()*aPlane->GetSigmaThetaMS();
  // Add last scattering if plane different from first
  DR3 lastPosition;
  if( planeSetIds[0][1] !=planeSetIds[1][0] ) {
    aPlane = tTracker->GetPlane(planeSetIds[1][0]);
    lastPosition = aPlane->GetPosition();
    printf( "  last plane %d, z-position %.0f\n", planeSetIds[1][0], position(2) );
    expectedDeviation += aPlane->GetSigmaThetaMS()*aPlane->GetSigmaThetaMS();
  } else {
    lastPosition = firstPosition;
  }
  // Search any intermediate scattering
  for (Int_t iPlane=0; iPlane<tTracker->GetPlanesN(); iPlane++) {
    aPlane = tTracker->GetPlane( iPlane+1);
    position = aPlane->GetPosition();
    printf( "  Plane %d, z-position %.0f within [%.0f, %.0f]? test=%d\n", iPlane+1, position(2), firstPosition(2), lastPosition(2), ( firstPosition(2)<position(2) && position(2)<lastPosition(2) ) );
    if ( firstPosition(2)<position(2) && position(2)<lastPosition(2) ) {
      expectedDeviation += aPlane->GetSigmaThetaMS()*aPlane->GetSigmaThetaMS();
    }
  }
  // the factor 1/sqrt(2) is because we measure angle deviations wrt fixed frame coordinates, not "in plane"
  expectedDeviation = sqrt(expectedDeviation/2.);
  printf(" expected angular deviation from scattering in X or Y planes: %f\n", expectedDeviation);

  // === Display ====

  if( selectedTracks>0 ) {

    pad->cd(1);
    if( hSetTrackSlopeX[0]->GetEntries()>0 ) hSetTrackSlopeX[0]->DrawCopy();
    if( hSetTrackSlopeX[1]->GetEntries()>0 ) hSetTrackSlopeX[1]->DrawCopy("same");
    pad->cd(2);
    if( hSetTrackSlopeY[0]->GetEntries()>0 ) hSetTrackSlopeY[0]->DrawCopy();
    if( hSetTrackSlopeY[1]->GetEntries()>0 ) hSetTrackSlopeY[1]->DrawCopy("same");
    pad->cd(3);
    if( hSlopeResidualX->GetEntries()>0 ) hSlopeResidualX->DrawCopy();
    pad->cd(4);
    if( hSlopeResidualY->GetEntries()>0 ) hSlopeResidualY->DrawCopy();
    pad->cd(5);
    if( hdistance[0]->GetEntries()>0 ) hdistance[0]->DrawCopy();
    pad->cd(6);
    if( hdistance[1]->GetEntries()>0 ) hdistance[1]->DrawCopy();

    cmultscat->Update();

  }

  // =================
  // Save canvas and histos
  //cd to result dir
  // added JB 2012/05/11
  Char_t rootFile[300];
  sprintf(rootFile,"%sMultscat_planes%d%d%d%d_run%d.root",fSession->GetResultDirName().Data(),planeSetIds[0][0],planeSetIds[0][1],planeSetIds[1][0],planeSetIds[1][1],fSession->GetRunNumber());
  sprintf(rootFile,"%s", fTool.LocalizeDirName( rootFile));
  cout << "\n-- Saving histos and canvas into " << rootFile << endl;
  TFile fRoot(rootFile,"RECREATE");
  cmultscat->Write();
  hSlopeResidualX->Write();
  hSlopeResidualY->Write();
  for (Int_t iset=0; iset<2; iset++) {
    hSetTrackSlopeX[iset]->Write();
    hSetTrackSlopeY[iset]->Write();
    hdistance[iset]->Write();
  }
  fRoot.Close();


  // Delete histo in case the method is called again
  hSetTrackSlopeX[0]->Delete();
  hSetTrackSlopeX[1]->Delete();
  hSetTrackSlopeY[0]->Delete();
  hSetTrackSlopeY[1]->Delete();
  hdistance[0]->Delete();
  hdistance[1]->Delete();
  hSlopeResidualX->Delete();
  hSlopeResidualY->Delete();


}

//______________________________________________________________________________
//
void MRaw::StudyBeamProfile(Int_t nEvents, Bool_t ifFit2DGaus, Double_t Xmin, Double_t Xmax, Double_t Ymin, Double_t Ymax){
  // Created by QL, 2015/10/26
  //
  // This function produces a TTree which contains event number, plane number, hit position and hit multiplicity.
  // All the related THistograms could be generated from the TTree to save time on hit reconstruction and to simplify the event loop.
  // TODO:
  //      1. One can check the content of the TTree in a file, such that the function could only process the events which are noted stored in the TTree before.
  //      2. Most of the histograms could be generated from this TTree by a common TTree2Histogram function which will simplify the code and make it more readable.
  // Note:
  //      TTree and TH1 must be in the global file directory to show plots.
  //gStyle->SetFillStyle(3001);
  fSession->SetEvents(nEvents);
  DAcq *acquisition = fSession->GetDataAcquisition();

  DTracker *tTracker  =  (DTracker*)fSession->GetTracker();
  Int_t nPlanes = tTracker->GetPlanesN();
  //Int_t nPlanesInTracking = nPlanes-tTracker->GetNDUT(); // If DUT Readout is set to 0, this plane is counted in GetPlanesN() but not in GetNDUT()

  printf("StudyBeamProfile: \n");

  // ----- File directory for TTree -------------------
  TFile * previousDir = (TFile*)gDirectory->GetFile();
  //Char_t rootFile[300];
  //sprintf(rootFile,"%s/BeamProfile_run%d.root",fSession->GetResultDirName().Data(),fSession->GetRunNumber());
  //sprintf(rootFile,"%s", fTool.LocalizeDirName( rootFile));
  //TFile fRoot(rootFile,"RECREATE");
  // --------------------------------------------------
  // ---------------- Prepare TTree -------------------
  TTree * mRawTree = new TTree("mRawTree", "Data Summary for RawData");
  Float_t x, y;
  Int_t plN = 0;
  Int_t evtNb = 0;
  Int_t nPixels = 0;
  Int_t nHits = 0;
  mRawTree->Branch("evtNb", &evtNb, "evtNb/I");
  mRawTree->Branch("plN", &plN, "plN/I");
  mRawTree->Branch("x", &x, "x/F");
  mRawTree->Branch("y", &y, "y/F");
  mRawTree->Branch("nPixels", &nPixels, "nPixels/I");
  //mRawTree->Branch("nHits", &nHits, "nHits/I"); // In this simple Hit info based TTree, this way to fil nHits is wrong

  // ============= Event Loop ==========
  //DTrack *aTrack = NULL;
  //DPlane *aPlane = NULL;
  DHit   *aHit = NULL;
  //Double_t col;
  //Double_t lin;

  for(Int_t iEvt = 0; iEvt < nEvents; iEvt++){
    if( !(fSession->NextRawEvent()) ) break; // JB 2009/06/26

    // === Analysis ====
    tTracker->Update();
    evtNb = acquisition->GetRealEventNumber(); // Fill TTree variable;
    //evtNb = iEvt; // Fill TTree variable;

    for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
      DPlane *tPlane = tTracker->GetPlane(iPlane);
      nHits = tPlane->GetHitsN();// nHits in one plane
      Int_t nHitsInTriggerRegion = 0;
      plN = iPlane; // Fill TTree variable

      if(nHits > 0){
	//if(nTracksInPl[iPlane-1]>0){
	//   if(iEvt % 500 == 5)
	//     cout<< "Pl" << iPlane << ": NTracksPl/NTracks = " << nTracksInPl[iPlane-1] << "/" << nTracks << " " << "NHits = " << nHits << " NTracksPl/NHits = " << (Float_t)nTracksInPl[iPlane-1] / nHits << endl;
	//	if(nTracksInPl[iPlane-1]>0)
	for(Int_t iHit=1; iHit<=nHits; iHit++){
	  aHit = (DHit*)tPlane->GetHit(iHit);
	  //hHitMap[iPlane-1]->Fill(aHit->GetPositionUhit(),aHit->GetPositionVhit(), 1);
	  DR3 positionGlobal = tPlane->PlaneToTracker(*aHit->GetPosition());
	  if(positionGlobal(0) < Xmax && positionGlobal(0) > Xmin && positionGlobal(1) < Ymax && positionGlobal(1) > Ymin){
	       //tPlane->ComputeStripPosition_UVToColRow(aHit->GetPositionUhit(), aHit->GetPositionVhit(), col, lin);
	       nHitsInTriggerRegion++;
	       x = positionGlobal(0);
	       y = positionGlobal(1);
	       nPixels = aHit->GetStripsInCluster();
	       mRawTree->Fill(); // Unknow issues happend when Results/RunNumber/etaRunNumber.root is not closed properly. Delete that file and run TAF agian.!
	  }//close for if positionGlobal in Range
	} // End Hit Loop
      }// close for if
    }// End Plane Loop
  }
  //============== End Event Loop =========

  //========== Data Consumption ========
  //---------- Generate Histograms ----------
  TH1F **hNHitsVSEventN = new TH1F*[nPlanes]; // QL 2015/10/23
  TH1F **hHitPixMult = new TH1F*[nPlanes];
  TH1F **hNHitsPerEvent = new TH1F*[nPlanes];
  TH2F **hHitMap = new TH2F*[nPlanes];

  Char_t name[50], title[100], selection[100];
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    DPlane* tPlane = tTracker->GetPlane(iPlane);

    // -- HitMap with 2D Gaus Fit
    // QL 2015/11/03
    sprintf( name, "hHitMapPl%d", iPlane);
    sprintf( title, "Hit map for plane %d - %s - %s", iPlane, tPlane->GetPlaneName(), tPlane->GetPlanePurpose());
    hHitMap[iPlane-1] = new TH2F(name,title, 300,-15000,15000,300,-15000,15000);
    hHitMap[iPlane-1]->SetXTitle("X");
    hHitMap[iPlane-1]->SetYTitle("Y");
    sprintf( selection, "plN==%d", iPlane);
    mRawTree->Project(name,"y:x", selection);

    // -- #Hits VS #Event
    // QL 2015/10/23
    sprintf( name, "hNHitsVSEventNPl%d", iPlane);
    sprintf( title, "Number of Clusters VS Event Number for plane %d - %s - %s", iPlane, tPlane->GetPlaneName(), tPlane->GetPlanePurpose());
    hNHitsVSEventN[iPlane-1] = new TH1F(name, title, nEvents, 0, nEvents );
    hNHitsVSEventN[iPlane-1]->SetXTitle("Event Number");
    sprintf( selection, "plN==%d", iPlane);
    mRawTree->Project(name,"evtNb", selection);

    // -- Pixel multiplicity in hits
    Int_t nPixMax = fSession->GetSetup()->GetPlanePar(iPlane).MaxNStrips;
    sprintf( name, "hhitpixpl%d", iPlane);
    sprintf( title, "Pixel multiplicity in hits of plane %d - %s - %s", iPlane, tPlane->GetPlaneName(), tPlane->GetPlanePurpose());
    if( nPixMax==0 ) {
      hHitPixMult[iPlane-1] = new TH1F(name, title, 100, 0, 0);
    }
    else {
      hHitPixMult[iPlane-1] = new TH1F(name, title, TMath::Max(nPixMax,500)+1, -.5, TMath::Max(nPixMax,500)+0.5); // Set a large nPixels range for histograms
    }
    hHitPixMult[iPlane-1]->SetXTitle("# pixels in hit");
    sprintf( selection, "plN==%d", iPlane);
    mRawTree->Project(name,"nPixels", selection);

    // -- Number of hits per event
    sprintf( name, "hnhitspereventpl%d", iPlane);
    sprintf( title, "Number of hits per event of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    //hNHitsPerEvent[iPlane-1] = new TH1F(name, title, 100, 0, 0);
    Double_t NHitsInPlMax = fSession->GetSetup()->GetTrackerPar().HitsInPlaneMaximum;
    hNHitsPerEvent[iPlane-1] = new TH1F(name, title, NHitsInPlMax+1, -0.5, NHitsInPlMax+0.5); // QL 2015/10/23, High trigger rate
    hNHitsPerEvent[iPlane-1]->SetXTitle("# hits per event");
    sprintf( selection, "plN==%d", iPlane);
    //mRawTree->Project(name,"nHits", selection); It's Wrong!
    // Fill the nHits of each plane
    for (Int_t i = 0; i < nEvents; i++){
	hNHitsPerEvent[iPlane-1]->Fill(hNHitsVSEventN[iPlane-1]->GetBinContent(i+1));
    }// END for filling nHits histogram from hNHitsPerEvent histograms
  }// END Plane Loop

  TF2 *f_2dGaus = new TF2("f_2dGaus","[0]*TMath::Gaus(x,[1],[2])*TMath::Gaus(y,[3],[4])",-15000,15000,-15000,15000);
  f_2dGaus->SetParNames("Normalization","meanX","sigmaX","meanY","sigmaY");
  f_2dGaus->SetLineColor(kRed);

  //============== Show Plots ===============
  Int_t Index_c; // Index for canvas->cd() loop
  //------------------------------------------
  TCanvas *cBeamProfile_hitMap;
  TObject* g_hitMap = gROOT->FindObject("cBeamProfile_hitMap") ;
  if (g_hitMap) {
    cBeamProfile_hitMap = (TCanvas*)g_hitMap;
  }
  else {
    cBeamProfile_hitMap = new TCanvas("cBeamProfile_hitMap", "Hit Map", 100, 100,900,600);
  }
  cBeamProfile_hitMap->Clear();
  cBeamProfile_hitMap->Divide((Int_t)((nPlanes+1)/2.),(nPlanes>1)?2:1); //nPlanesInTracking could be an odd number.
  Index_c = 1;
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    DPlane *tPlane = tTracker->GetPlane(iPlane);
    if(tPlane->GetStatus() <= 2){
      cBeamProfile_hitMap->cd(Index_c);
      hHitMap[iPlane-1]->Draw("colz");
      if(ifFit2DGaus){
	   Float_t meanX  = hHitMap[iPlane-1]->GetMean(1);
           Float_t sigmaX = hHitMap[iPlane-1]->GetRMS(1);
           Float_t meanY  = hHitMap[iPlane-1]->GetMean(2);
           Float_t sigmaY = hHitMap[iPlane-1]->GetRMS(2);
           f_2dGaus->SetParameters(400,meanX,sigmaX,meanY,sigmaY);
           if(!hHitMap[iPlane-1]->Fit("f_2dGaus")){
               hHitMap[iPlane-1]->Draw("surf2z");
           }
      }
      //TPaveStats *ps1 = (TPaveStats*)hHitMap[iPlane-1]->GetListOfFunctions()->FindObject("stats");
      //ps1->SetFillStyle(3001); // transparent stats panel
      //ps1->SetY1NDC(0.6);
      //ps1->SetX1NDC(0.7);
      Index_c++;
    }
  }//End Plane Loop
  //------------------------------------------
  TCanvas *cBeamProfile_nHitsVSevtN;
  TObject* g_nHitsVSevtN = gROOT->FindObject("cBeamProfile_nHitsVSevtN") ;
  if (g_nHitsVSevtN) {
    cBeamProfile_nHitsVSevtN = (TCanvas*)g_nHitsVSevtN;
  }
  else {
    cBeamProfile_nHitsVSevtN = new TCanvas("cBeamProfile_nHitsVSevtN", "NHits VS Event number", 100, 100,900,600);
  }
  cBeamProfile_nHitsVSevtN->Clear();
  cBeamProfile_nHitsVSevtN->Divide((Int_t)((nPlanes+1)/2.),(nPlanes>1)?2:1); //nPlanesInTracking could be an odd number.
  Index_c = 1;
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    DPlane *tPlane = tTracker->GetPlane(iPlane);
    if(tPlane->GetStatus() <= 2){
      cBeamProfile_nHitsVSevtN->cd(Index_c);
      hNHitsVSEventN[iPlane-1]->Draw();
      Index_c++;
    }
  }//End Plane Loop
  //------------------------------------------
  TCanvas *cBeamProfile_nClusters;
  TObject* g_nClusters = gROOT->FindObject("cBeamProfile_nClusters") ;
  if (g_nClusters) {
    cBeamProfile_nClusters = (TCanvas*)g_nClusters;
  }
  else {
    cBeamProfile_nClusters = new TCanvas("cBeamProfile_nClusters", "Number of Clusters per event", 100, 100,900,600);
  }
  cBeamProfile_nClusters->Clear();
  cBeamProfile_nClusters->Divide((Int_t)((nPlanes+1)/2.),(nPlanes>1)?2:1); //nPlanesInTracking could be an odd number.
  Index_c = 1;
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    DPlane *tPlane = tTracker->GetPlane(iPlane);
    if(tPlane->GetStatus() <= 2){
      cBeamProfile_nClusters->cd(Index_c);
      hNHitsPerEvent[iPlane-1]->Draw();
      hNHitsPerEvent[iPlane-1]->Fit("gaus");
      Double_t mean = hNHitsPerEvent[iPlane-1]->GetMean();
      Double_t rms = TMath::Abs(hNHitsPerEvent[iPlane-1]->GetRMS());
      hNHitsPerEvent[iPlane-1]->GetXaxis()->SetRangeUser(mean-rms*5.5,TMath::Max(mean+rms*5.5,10.));
      //hNHitsPerEvent[iPlane-1]->GetFunction("gaus")->SetLineColor(kRed);
      Index_c++;
    }
  }//End Plane Loop
  //------------------------------------------
  TCanvas *cBeamProfile_hitPixMult;
  TObject* g_hitPixMult = gROOT->FindObject("cBeamProfile_hitPixMult") ;
  if (g_hitPixMult) {
    cBeamProfile_hitPixMult = (TCanvas*)g_hitPixMult;
  }
  else {
    cBeamProfile_hitPixMult = new TCanvas("cBeamProfile_hitPixMult", "Pixel Multiplicity", 100, 100,900,600);
  }
  cBeamProfile_hitPixMult->Clear();
  cBeamProfile_hitPixMult->Divide((Int_t)((nPlanes+1)/2.),(nPlanes>1)?2:1); //nPlanesInTracking could be an odd number.
  Index_c = 1;
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    DPlane *tPlane = tTracker->GetPlane(iPlane);
    if(tPlane->GetStatus() <= 2){
      cBeamProfile_hitPixMult->cd(Index_c);

      Double_t mean = hHitPixMult[iPlane-1]->GetMean();
      Double_t rms  = TMath::Abs(hHitPixMult[iPlane-1]->GetRMS());
      hHitPixMult[iPlane-1]->Draw();
      hHitPixMult[iPlane-1]->GetXaxis()->SetRangeUser(mean-rms*5.5,TMath::Max(mean+rms*5.5,10.));
      Index_c++;
    }
  }//End Plane Loop

  //============= Save to File ======================
  Char_t rootFile[300];
  sprintf(rootFile,"%s/BeamProfile_run%d.root",fSession->GetResultDirName().Data(),fSession->GetRunNumber());
  sprintf(rootFile,"%s", fTool.LocalizeDirName( rootFile));
  TFile fRoot(rootFile,"RECREATE");
  cout << "\n-- Saving histos and canvas into " << rootFile << endl;
  for(Int_t iPlane=1; iPlane<=nPlanes; iPlane++){ // loop on planes
    DPlane * tPlane  = tTracker->GetPlane(iPlane);
    if(tPlane->GetStatus() <= 2){
	 hNHitsVSEventN[iPlane-1]->Write();
	 hNHitsPerEvent[iPlane-1]->Write();
	 hHitPixMult[iPlane-1]->Write();
	 hHitMap[iPlane-1]->Write();
    };
  }//End Plane loop
  cBeamProfile_nHitsVSevtN->Write();
  cBeamProfile_hitPixMult->Write();
  cBeamProfile_nClusters->Write();
  cBeamProfile_hitMap->Write();
  mRawTree->CloneTree()->Write();// QL, 2015/10/26 Save a TTree in another file domain.
  //mRawTree->Write();
  fRoot.Close();
  previousDir->cd();
  //===================================================

  return;
}
//______________________________________________________________________________
//
void MRaw::XrayAnalysis( Int_t nEvents,
                        Bool_t ProduceTree,
                        Bool_t ifDrawTrack,
                        Bool_t Define_Range,
                        int bins,
                        double Xmin, double Xmax,
                        double Ymin, double Ymax,
                        Float_t HighestPeakPositionEv, Float_t MinSpectrum, Float_t MaxSpectrum,
                        Bool_t readNormFromFile,
                        Bool_t normalizeADCspectrum,
                        Float_t cutLimit,
                        Int_t fitXray)//fitXray: 0==no fitting, 1==55Fe, 2==Cr, 3==Cu, 4==Mo
{



  // Analysis specific for X-ray data
  // Call by gTAF->GetRaw()->XrayAnalysis()
  // JH, 2016/07/20 originally copied from DisplayCumulatedHits2D
  //JH, 4/4/2018 => Added fitting of hhitseedq with multiple parameters


  if(ProduceTree) {
    fSession->MakeTree();
    //fSession->SetFillLevel( fillLevel); // JB 2011/07/21
    fSession->SetFillLevel(1); // JB 2011/07/21
    //fSession->GetTracker()->SetAlignmentStatus(2);  // 2 = all planes in telescope used for tracking, for NOW JB
    if( fSession->GetTracker()->GetAlignmentStatus()==1 ) {
      printf("\nDSFProduction: -*- WARNING -*- only fixed (or primary) reference planes will be used for tracking!\n");
      printf("                                use gTAF->SetAlignStatus(2) to modify.\n");
    }
    else {
      printf("\nDSFProduction: both fixed and variable reference planes will be used for tracking.\n");
    }
  }

  fSession->SetEvents(nEvents);

  Int_t nHitsReconstructed = 0;
  Float_t normFromFile=0;

  if (readNormFromFile==kTRUE)	{
    ifstream normTempFileIn;
    normTempFileIn.open("Scripts/normtemp.txt");
    string normStr;
    getline(normTempFileIn, normStr);
    normTempFileIn.close();
    normFromFile=::atof(normStr.c_str());
    cout<<"norm Str : "<<normStr<<endl;
  }
  else {

  }

  TCanvas *ccumulhit;
  TObject* g = gROOT->FindObject("ccumulhit") ;
  if (g) {
    ccumulhit = (TCanvas*)g;
  }
  else {
    ccumulhit = new TCanvas("ccumulhit", "Cumulate Hits", 5, 5,800,700);
  }
  ccumulhit->Clear();
  ccumulhit->UseCurrentStyle();
  TPaveLabel* label = new TPaveLabel();
  Char_t canvasTitle[200];
  sprintf(canvasTitle, "Run %d, cumul over %d events", fSession->GetRunNumber(), nEvents);
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad = new TPad("pad","",0.,0.,1.,0.965);
  pad->Draw();

  DTracker *tTracker  =  fSession->GetTracker();
  DPlane* tPlane = NULL;
  DTrack *aTrack = NULL;
  DHit *aHit = NULL;
  //DPixel *aPixel;
  Double_t seedSN;
  //Double_t seedN;

  Int_t nPlanes = tTracker->GetPlanesN();
  if( nPlanes>6 ) {
    pad->Divide( (Int_t)ceil(nPlanes/4.), (nPlanes>4)?4:nPlanes);  // LC 4 lines instead of 2.
  }
  else {
    pad->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);
  }

  // Get the first DUT among the planes and remember it,
  //  it considered as the one providing the coordinate frame for the geomatrix
  // If no DUT (status=3), set to first plane
  // JB 2012/0820
  DPlane* DUTplane = tTracker->GetPlane(1);
  for( Int_t iPlane=2; iPlane<=nPlanes; iPlane++) { // loop on planes
    tPlane = tTracker->GetPlane(iPlane);
    if( tPlane->GetStatus()==3 ) {
      DUTplane = tPlane;
      break;
    }
  }
  // geomatrix coordinates and box
  Int_t geomatrix=3;
  DR3 gUVWmin( fSession->GetSetup()->GetAnalysisPar().Umin[0][geomatrix], fSession->GetSetup()->GetAnalysisPar().Vmin[0][geomatrix], DUTplane->GetPosition()(2));
  DR3 gUVWmax( fSession->GetSetup()->GetAnalysisPar().Umax[0][geomatrix], fSession->GetSetup()->GetAnalysisPar().Vmax[0][geomatrix], DUTplane->GetPosition()(2));
  // Transform those DUTplane frame coordinates into the telescope frame
  DR3 gXYZmin = DUTplane->PlaneToTracker(gUVWmin);
  DR3 gXYZmax = DUTplane->PlaneToTracker(gUVWmax);
  printf("Geomatrix: U-range %.0f, %.0f, V-range %.0f, %.0f\n", gUVWmin(0), gUVWmax(0), gUVWmin(1), gUVWmax(1));
  printf("Geomatrix: X-range %.0f, %.0f, Y-range %.0f, %.0f\n", gXYZmin(0), gXYZmax(0), gXYZmin(1), gXYZmax(1));
  cout << " DUT plane is got as plane " << DUTplane->GetPlaneNumber() << endl;

  TBox **geomBox = new TBox*[nPlanes];
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    geomBox[iPlane-1] = new TBox( gXYZmin(0), gXYZmin(1), gXYZmax(0), gXYZmax(1) );
    geomBox[iPlane-1]->SetFillStyle(0);
    geomBox[iPlane-1]->SetLineStyle(1);
    geomBox[iPlane-1]->SetLineWidth(2);
    geomBox[iPlane-1]->SetLineColor(1);
  } // end loop on planes

  // Determine extrema of planes position in telescope frame
  Double_t xmin=1e6, xmax=-1e6;
  Double_t ymin=1e6, ymax=-1e6;
  Double_t planeBox[2][2] = {{0,0},{0,0}};
  TBox **geomPlaneBox = new TBox*[nPlanes];
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    tPlane = tTracker->GetPlane(iPlane);
    // bottom left corner
    DR3 posInPlane, posBLInTracker, posURInTracker;
    posInPlane.SetValue( -tPlane->GetStripsNu() * tPlane->GetStripPitch()(0) / 2.
                        ,-tPlane->GetStripsNv() * tPlane->GetStripPitch()(1) / 2.
                        ,0.);
    posBLInTracker = tPlane->PlaneToTracker( posInPlane);
    if( posBLInTracker(0)<xmin ) xmin = posBLInTracker(0);
    if( posBLInTracker(1)<ymin ) ymin = posBLInTracker(1);
    if( posBLInTracker(0)>xmax ) xmax = posBLInTracker(0);
    if( posBLInTracker(1)>ymax ) ymax = posBLInTracker(1);
    // upper right corner
    posInPlane.SetValue( +tPlane->GetStripsNu() * tPlane->GetStripPitch()(0) / 2.
                        ,+tPlane->GetStripsNv() * tPlane->GetStripPitch()(1) / 2.
                        ,0.);
    posURInTracker = tPlane->PlaneToTracker( posInPlane);
    if( posURInTracker(0)<xmin ) xmin = posURInTracker(0);
    if( posURInTracker(1)<ymin ) ymin = posURInTracker(1);
    if( posURInTracker(0)>xmax ) xmax = posURInTracker(0);
    if( posURInTracker(1)>ymax ) ymax = posURInTracker(1);
    planeBox[0][0] = TMath::Min( posBLInTracker(0), posURInTracker(0));
    planeBox[0][1] = TMath::Max( posBLInTracker(0), posURInTracker(0));
    planeBox[1][0] = TMath::Min( posBLInTracker(1), posURInTracker(1));
    planeBox[1][1] = TMath::Max( posBLInTracker(1), posURInTracker(1));
    geomPlaneBox[iPlane-1] = new TBox( planeBox[0][0], planeBox[1][0], planeBox[0][1], planeBox[1][1]);
    geomPlaneBox[iPlane-1]->SetFillStyle(0);
  } // end loop on planes

  int NbinsX = tPlane->GetStripsNu();
  int NbinsY = tPlane->GetStripsNv();
  if(Define_Range) {
    xmin  = Xmin;
    xmax  = Xmax;
    ymin  = Ymin;
    ymax  = Ymax;
    NbinsX = bins;
    NbinsY = bins;
  }

  TH2F **hHitMap = new TH2F*[nPlanes];
  TH2F **hTrackMap = new TH2F*[nPlanes]; // JB 2011/11/02
  TH1F **hHitPixMult = new TH1F*[nPlanes];
  TH1F **hNHitsPerEvent = new TH1F*[nPlanes];
  TH2F **hNHitsPerEventCorr = new TH2F*[nPlanes*(nPlanes - 1)/2];
  TH1F **hHitSeedSN = new TH1F*[nPlanes]; // JB 2012/08/17
  TH1F **hHitNeighbourSN = new TH1F*[nPlanes];
  TH2F **hHitSeedVsNeighbourSN = new TH2F*[nPlanes];
  TH1F **hHitCharge = new TH1F*[nPlanes];
  TH1F **hHitSeedCharge = new TH1F*[nPlanes]; // JB 2013/10/08
  TH1F **hHitSeedCharge_CUT   = new TH1F*[nPlanes]; // JH 2016/10/12
  TH1F **hHitChargeRatio   = new TH1F*[nPlanes]; // JH 2016/10/12
  TH1F **hHitNeighbourCharge = new TH1F*[nPlanes];
  TH2F **hHitSeedVsNeighbourCharge = new TH2F*[nPlanes];
  TH2F **hHitSeedVsNeighbourCharge_cut = new TH2F*[nPlanes];
  TH2F **hHitSeedVsNeighbourCharge_cutseed = new TH2F*[nPlanes];
  TH2F **hHitSeedSNVsSeedCharge = new TH2F*[nPlanes];
  TH1F **hHitTimeStamp = new TH1F*[nPlanes]; // JB 2015/05/25
  TH1F **hNHitsVSEventN = new TH1F*[nPlanes]; // QL 2015/10/23
  TCanvas **cHitProperties = new TCanvas*[nPlanes]; // JB 2013/10/30
  TCanvas **cHitProperties2 = new TCanvas*[nPlanes]; // JH 2016/10/12
  TCanvas **cHitSeedChargeSum = new TCanvas*[nPlanes]; // JH 2016/10/12
  TCanvas **cHitChargeRatio = new TCanvas*[nPlanes]; // JH 2016/10/12
  Char_t name[50], title[100];
  int counter_planes_pairs = 0;
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    tPlane = tTracker->GetPlane(iPlane);

    // -- Histo with microns
    sprintf( name, "hhitmappl%d", iPlane);
    sprintf( title, "Hit map of plane %d - %s;X (#mum);Y (#mum)", iPlane, tPlane->GetPlanePurpose());
    // check if the histo is existing or not
    hHitMap[iPlane-1] = new TH2F(name, title, NbinsX, xmin, xmax, NbinsY, ymin, ymax);
    hHitMap[iPlane-1]->SetMarkerStyle(20);
    hHitMap[iPlane-1]->SetMarkerSize(.1);
    hHitMap[iPlane-1]->SetMarkerColor(1);
    hHitMap[iPlane-1]->SetStats(kFALSE);
    //printf( "MRaw::DisplayRawData created %s histo with %dx%d pixels\n", name, tPlane->GetStripsNu(), tPlane->GetStripsNv());

    // -- Histo for tracks with microns, JB 2011/11/02
    sprintf( name, "htrackmappl%d", iPlane);
    sprintf( title, "Track map of plane (%d) %s;X (#mum);Y (#mum)", iPlane, tPlane->GetPlanePurpose());
    hTrackMap[iPlane-1] = new TH2F( *(hHitMap[iPlane-1])); // JB 2009/09/09
    //name, title, hHitMap[iPlane-1]->GetNbinsX(), hHitMap[iPlane-1]->GetNbinsX(), tPlane->GetStripPitch()(0)*tPlane->GetStripsNu()/2., 100, -tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2., tPlane->GetStripPitch()(1)*tPlane->GetStripsNv()/2.);
    hTrackMap[iPlane-1]->SetName( name);
    hTrackMap[iPlane-1]->SetTitle( title);
    hTrackMap[iPlane-1]->SetMarkerStyle(24);
    hTrackMap[iPlane-1]->SetMarkerSize(.2);
    hTrackMap[iPlane-1]->SetMarkerColor(2);
    hTrackMap[iPlane-1]->SetStats(kFALSE);

    // -- Pixel multiplicity in hits
    Int_t nPixMax = fSession->GetSetup()->GetPlanePar(iPlane).MaxNStrips;
    sprintf( name, "hhitpixpl%d", iPlane);
    sprintf( title, "Pixel multiplicity in hits of plane %d - %s - %s", iPlane, tPlane->GetPlaneName(), tPlane->GetPlanePurpose());
    if( nPixMax==0 ) {
      hHitPixMult[iPlane-1] = new TH1F(name, title, 25, 0, 0);
    }
    else {
      hHitPixMult[iPlane-1] = new TH1F(name, title, nPixMax+1, -.5, nPixMax+0.5);
    }
    hHitPixMult[iPlane-1]->SetXTitle("# pixels in hit");

    // -- Pixel time stamp
    // JB 2015/05/25
    sprintf( name, "hhittimepl%d", iPlane);
    sprintf( title, "Hit time stamp of plane %d - %s - %s; Time stamp", iPlane, tPlane->GetPlaneName(), tPlane->GetPlanePurpose());
    hHitTimeStamp[iPlane-1] = new TH1F(name, title, 100, 0, 0);

    // -- #Hits VS #Event
    // QL 2015/10/23
    sprintf( name, "hNHitsVSEventNPl%d", iPlane);
    sprintf( title, "Number of Clusters VS Event Number for plane %d - %s - %s", iPlane, tPlane->GetPlaneName(), tPlane->GetPlanePurpose());
    hNHitsVSEventN[iPlane-1] = new TH1F(name, title, nEvents, 0, nEvents );
    hNHitsVSEventN[iPlane-1]->SetXTitle("Event Number");

    // -- Number of hits per event
    sprintf( name, "hnhitspereventpl%d", iPlane);
    sprintf( title, "Number of hits per event of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    //hNHitsPerEvent[iPlane-1] = new TH1F(name, title, 100, 0, 0);
    hNHitsPerEvent[iPlane-1] = new TH1F(name, title, 751, -0.5, 750.5); // QL 2015/10/23, High trigger rate
    hNHitsPerEvent[iPlane-1]->SetXTitle("# hits per event");

    // -- Analog hit properties
    sprintf( name, "hhitseedsn%d", iPlane);
    sprintf( title, "Hit S/N for seed pixel of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitSeedSN[iPlane-1] = new TH1F(name, title, 200, 0, 0);
    hHitSeedSN[iPlane-1]->SetXTitle("S/N");
    sprintf( name, "hhitneighboursn%d", iPlane);
    sprintf( title, "Hit S/N for neighbour pixels of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitNeighbourSN[iPlane-1] = new TH1F(name, title, 100, 0, 0);
    hHitNeighbourSN[iPlane-1]->SetXTitle("S/N");
    sprintf( name, "hhitseedneighbourgsn%d", iPlane);
    sprintf( title, "Hit S/N for neighbour pixels VS seed of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitSeedVsNeighbourSN[iPlane-1] = new TH2F(name, title, 200, 0, 0, 50, 0, 0);
    hHitSeedVsNeighbourSN[iPlane-1]->SetXTitle("seed S/N");
    hHitSeedVsNeighbourSN[iPlane-1]->SetYTitle("neighbourgs S/N");
    sprintf( name, "hhitcharge%d", iPlane);
    sprintf( title, "Hit charge of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitCharge[iPlane-1] = new TH1F(name, title, 16001, 0., 16000.);
    hHitCharge[iPlane-1]->SetXTitle("ADC unit");
    // JB 2013/10/08

    sprintf( name, "hhitseedqcut%d", iPlane);
    sprintf( title, "Seed pixel charge cut at %f of plane %d - %s; seed charge (ADCu); counts", cutLimit,iPlane, tPlane->GetPlanePurpose());
    hHitSeedCharge_CUT[iPlane-1] = new TH1F(name, title, 16001, 0, 16000);


    sprintf( name, "hhitqratio%d", iPlane);
    sprintf( title, "Ratio of charges in the seed in the cluster of plane %d - %s; Ratio; counts", iPlane, tPlane->GetPlanePurpose());
    hHitChargeRatio[iPlane-1] = new TH1F(name, title, 100, 0, 1);




    sprintf( name, "hhitseedq%d", iPlane);
    if (readNormFromFile==kFALSE)	{
      sprintf( title, "Seed pixel charge of plane %d - %s; seed charge (ADCu); counts", iPlane, tPlane->GetPlanePurpose());
      if (normalizeADCspectrum == kTRUE)	{
        ifstream binningTempFileIn;
        binningTempFileIn.open("Scripts/binningtemp.txt");
        string binningStr;
        getline(binningTempFileIn, binningStr);
        binningTempFileIn.close();
        Int_t BinToApply=::atof(binningStr.c_str());
        cout<<"norm Str : "<<BinToApply<<endl;
        hHitSeedCharge[iPlane-1] = new TH1F(name, title, BinToApply, 0, BinToApply);

      }
      else {
        hHitSeedCharge[iPlane-1] = new TH1F(name, title, 16001, 0, 16000);
      }
    }
    else {
      sprintf( title, "Seed pixel charge of plane %d - %s; energy (eV); counts/10 eV", iPlane, tPlane->GetPlanePurpose());
      hHitSeedCharge[iPlane-1] = new TH1F(name, title, 700, 0., 7000.);
    }



    sprintf( name, "hhitneighbourq%d", iPlane);
    sprintf( title, "Neighbour pixels charge of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitNeighbourCharge[iPlane-1] = new TH1F(name, title, 16001, 0, 16000);
    hHitNeighbourCharge[iPlane-1]->SetXTitle("charge (ADCu)");
    sprintf( name, "hhitseedneighbourgq%d", iPlane);
    sprintf( title, "Neighbour pixels VS seed charge of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitSeedVsNeighbourCharge[iPlane-1] = new TH2F(name, title, 16001, 0, 16000, 16001, -8000, 8000);
    hHitSeedVsNeighbourCharge[iPlane-1]->SetXTitle("seed charge (ADCu)");
    hHitSeedVsNeighbourCharge[iPlane-1]->SetYTitle("neighbourgs charge (ADCu)");
    sprintf( name, "hhitseedneighbourgq_cut%d", iPlane);
    sprintf( title, "Neighbour pixels VS seed charge of plane %d - %s cut", iPlane, tPlane->GetPlanePurpose());
    hHitSeedVsNeighbourCharge_cut[iPlane-1] = new TH2F(name, title, 100, 0, 0, 50, 0, 0);
    hHitSeedVsNeighbourCharge_cut[iPlane-1]->SetXTitle("seed charge (ADCu)");
    hHitSeedVsNeighbourCharge_cut[iPlane-1]->SetYTitle("neighbourgs charge (ADCu)");
    sprintf( name, "hhitseedneighbourgq_cutseed%d", iPlane);
    sprintf( title, "Neighbour pixels VS seed charge of plane %d - %s cutseed", iPlane, tPlane->GetPlanePurpose());
    hHitSeedVsNeighbourCharge_cutseed[iPlane-1] = new TH2F(name, title, 16001, 0, 16000, 16001, 0, 16000);
    hHitSeedVsNeighbourCharge_cutseed[iPlane-1]->SetXTitle("seed charge (ADCu)");
    hHitSeedVsNeighbourCharge_cutseed[iPlane-1]->SetYTitle("neighbourgs charge (ADCu)");
    sprintf( name, "hhitseedsnseedq%d", iPlane);
    sprintf( title, "Seed pixel S/N VS seed charge of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitSeedSNVsSeedCharge[iPlane-1] = new TH2F(name, title, 100, 0, 0, 100, 0, 0);
    hHitSeedSNVsSeedCharge[iPlane-1]->SetXTitle("seed charge (ADCu)");
    hHitSeedSNVsSeedCharge[iPlane-1]->SetYTitle("seed S/N");

    for( Int_t iPlane2=1; iPlane2<=nPlanes; iPlane2++) {
      if(iPlane >= iPlane2) continue;
      DPlane* tPlane2 = tTracker->GetPlane(iPlane2);

      // -- Correlations among the planes of number of hits per event
      sprintf( name, "hnhitspereventForpl%d_vs_pl%d", iPlane, iPlane2);
      sprintf( title, "Number of hits per event of plane %d (%s) vs %d (%s)", iPlane, tPlane->GetPlanePurpose(),iPlane2, tPlane2->GetPlanePurpose());
      hNHitsPerEventCorr[counter_planes_pairs] = new TH2F(name, title,
                                                          hNHitsPerEvent[iPlane-1]->GetXaxis()->GetNbins(),
                                                          hNHitsPerEvent[iPlane-1]->GetXaxis()->GetXmin(),
                                                          hNHitsPerEvent[iPlane-1]->GetXaxis()->GetXmax(),
                                                          hNHitsPerEvent[iPlane-1]->GetXaxis()->GetNbins(),
                                                          hNHitsPerEvent[iPlane-1]->GetXaxis()->GetXmin(),
                                                          hNHitsPerEvent[iPlane-1]->GetXaxis()->GetXmax());
      sprintf( name, "# hits per event pl %d", iPlane);
      hNHitsPerEventCorr[counter_planes_pairs]->SetXTitle(name);
      sprintf( name, "# hits per event pl %d", iPlane2);
      hNHitsPerEventCorr[counter_planes_pairs]->SetYTitle(name);
      counter_planes_pairs++;
    }
  }

  //Loop over the requested number of events
  for( Int_t iEvt=0; iEvt < nEvents; iEvt++) {
    if( !(fSession->NextRawEvent()) ) break; // JB 2009/06/26
    tTracker->Update();

    counter_planes_pairs = 0;
    for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
      tPlane = tTracker->GetPlane(iPlane);

      //cout << "CumulateHits2D:: plane " << tPlane->GetPlaneNumber() << " has " << tPlane->GetHitsN() << endl;
      hNHitsPerEvent[iPlane-1]->Fill( tPlane->GetHitsN() );

      for( Int_t iPlane2=1; iPlane2<=nPlanes; iPlane2++) {
        if(iPlane >= iPlane2) continue;
        DPlane* tPlane2 = tTracker->GetPlane(iPlane2);
        hNHitsPerEventCorr[counter_planes_pairs]->Fill(tPlane->GetHitsN(),tPlane2->GetHitsN());
        counter_planes_pairs++;
      }


      if( tPlane->GetHitsN()>0 ) {
        nHitsReconstructed += tPlane->GetHitsN();
        //hNHitsPerEvent[iPlane-1]->Fill( tPlane->GetHitsN() );
        for( Int_t iHit=1; iHit<=tPlane->GetHitsN(); iHit++) { //loop on hits (starts at 1 !!)
          aHit = (DHit*)tPlane->GetHit( iHit);

          //printf("Getting seed index for hit %d (address %x) at plane %d\n", iHit, aHit, iPlane);
          hHitMap[iPlane-1]->Fill( tPlane->PlaneToTracker(*(aHit->GetPosition()))(0), tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), 1); // weight could be aHit->GetClusterPulseSum()

          // Filling hit properties depends on analysis mode
          // JB 2012/08/17
          //if( aHit->GetFound() )
          //printf("  hit %d at plane %d: #strips=%d, timestamp=%d\n", iHit, iPlane, aHit->GetStripsInCluster(), aHit->GetTimestamp());
          hHitPixMult[iPlane-1]->Fill( aHit->GetStripsInCluster()); // JB 2011/07/26
          hHitTimeStamp[iPlane-1]->Fill( aHit->GetTimestamp()); // JB 2015/05/25
          hNHitsVSEventN[iPlane-1]->Fill(iEvt);
          if( tPlane->GetAnalysisMode()<3 ) { // analog output
            //seedN = aHit->GetNoise(0);
            seedSN = aHit->GetSNseed(); // JB, 2013/11/08
            // computation below useless since new GetSNseed() method
            //            if( seedN>1.e-5 ) {
            //              seedSN = aHit->GetPulseHeight(0)/seedN;
            //            }
            //            else {
            //              seedSN = 0.;
            //            }
            hHitSeedSN[iPlane-1]->Fill( seedSN);
            hHitNeighbourSN[iPlane-1]->Fill( aHit->GetSNneighbour() );
            hHitSeedVsNeighbourSN[iPlane-1]->Fill( seedSN, aHit->GetSNneighbour() );
            hHitCharge[iPlane-1]->Fill( aHit->GetClusterPulseSum() );


            if (readNormFromFile==kFALSE) {
              hHitSeedCharge[iPlane-1]->Fill( aHit->GetPulseHeight(0));
            }
            else {
              hHitSeedCharge[iPlane-1]->Fill( aHit->GetPulseHeight(0)*normFromFile);
            }


            if ((aHit->GetClusterPulseSum()!=0) && (aHit->GetPulseHeight(0)>0))	{ // =====================ICI=========
              Float_t chargeRatioValue=aHit->GetPulseHeight(0)/aHit->GetClusterPulseSum();
              hHitChargeRatio[iPlane-1]->Fill(chargeRatioValue);
              if (chargeRatioValue>cutLimit)	{
                hHitSeedCharge_CUT[iPlane-1]->Fill( aHit->GetPulseHeight(0));
                hHitSeedVsNeighbourCharge_cutseed[iPlane-1]->Fill( aHit->GetPulseHeight(0), aHit->GetClusterAreaPulseSum());
              }
            }


            hHitNeighbourCharge[iPlane-1]->Fill( aHit->GetClusterAreaPulseSum());
            hHitSeedVsNeighbourCharge[iPlane-1]->Fill( aHit->GetPulseHeight(0), aHit->GetClusterAreaPulseSum());
            if ( aHit->GetPulseHeight(0) + aHit->GetClusterAreaPulseSum()>=170)	{
              hHitSeedVsNeighbourCharge_cut[iPlane-1]->Fill( aHit->GetPulseHeight(0), aHit->GetClusterAreaPulseSum());
            }
            hHitSeedSNVsSeedCharge[iPlane-1]->Fill( aHit->GetPulseHeight(0), seedSN);
          }
          if(fVerbose) printf("MRaw::DisplayCumulHits2D  pl %d, hit[%d=(%d,%d)=(%f,%f)]%f, mult=%d\n", iPlane, iHit, aHit->GetIndexSeed()%tPlane->GetStripsNu(), aHit->GetIndexSeed()/tPlane->GetStripsNu(), tPlane->PlaneToTracker(*(aHit->GetPosition()))(0), tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), aHit->GetClusterPulseSum(), aHit->GetStripsInCluster());
        } //end loop on hits
      }

      if( tTracker->GetTracksN()>0 ) {
        for( Int_t iTrack=1; iTrack<=tTracker->GetTracksN(); iTrack++ ) { // loop on tracks, JB 2011/11/02
          aTrack = tTracker->GetTrack(iTrack);
          DR3 posInPlane = aTrack->Intersection(tPlane);
          hTrackMap[iPlane-1]->Fill( tPlane->PlaneToTracker(posInPlane)(0), tPlane->PlaneToTracker(posInPlane)(1), 1);
          if(fVerbose) printf("MRaw::DisplayCumulHits2D  pl %d, track[%d=(%f,%f)\n", iPlane, iTrack, tPlane->PlaneToTracker(posInPlane)(0), tPlane->PlaneToTracker(posInPlane)(1));
        } // end loop on tracks
      }

    } //end loop on planes

    if(ProduceTree) fSession->FillTree();

  } // END LOOP ON EVENTS

  fSession->GetDataAcquisition()->PrintStatistics();
  tTracker->PrintStatistics();

  if(ProduceTree) {
    fSession->Finish();

    Char_t New_File_Name[1000];
    Char_t Old_File_Name[1000];
    sprintf(Old_File_Name,"%s/%s",(const char*)fSession->GetSummaryFilePath(),fSession->GetSummaryFileName().Data());
    sprintf(Old_File_Name,"%s", fTool.LocalizeDirName( Old_File_Name)); // JB 2011/07/07
    sprintf(New_File_Name,"%srun%d_0%d.root",(const char*)fSession->GetSummaryFilePath(),fSession->GetRunNumber(),GetFileNumber()+1);
    sprintf(New_File_Name,"%s", fTool.LocalizeDirName( New_File_Name)); // JB 2011/07/07

    gSystem->Rename(Old_File_Name,New_File_Name);
    cout<< fSession->GetSummaryFileName().Data()  <<" had been renamed : "<< New_File_Name <<endl;
    ofstream logfile;
    logfile.open("DSFProd.log",ios::app);
    logfile<< fSession->GetSummaryFileName().Data()  <<" had been renamed : "<< New_File_Name <<endl;
    logfile.close();
  }

  // Now display
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    pad->cd(iPlane);
    pad->cd(iPlane)->SetTickx(1);
    pad->cd(iPlane)->SetTicky(1);
    if( ifDrawTrack && tTracker->GetTracksMaximum() > 0) {
      hTrackMap[iPlane-1]->DrawCopy(); // JB 2011/11/02
      hHitMap[iPlane-1]->DrawCopy("same");
    }
    else {
      hHitMap[iPlane-1]->DrawCopy("colz");
    }
    geomPlaneBox[iPlane-1]->Draw("l");
    geomBox[iPlane-1]->Draw("l");
    cout << "Plane "<<iPlane<<" has seen "<<hHitMap[iPlane-1]->GetEntries()<<" hits."<< endl;
  }
  ccumulhit->Update();

  // this canvas is intended mainly for binary outputs
  TCanvas *ccumulhit2;
  g = gROOT->FindObject("ccumulhit2") ;
  if (g) {
    ccumulhit2 = (TCanvas*)g;
  }
  else {
    ccumulhit2 = new TCanvas("ccumulhit2", "Pixel multiplicitiy in hits", 5, 5,800,700);
  }
  ccumulhit2->Clear();
  ccumulhit2->UseCurrentStyle();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad2 = new TPad("pad2","",0.,0.,1.,0.965);
  pad2->Draw();
  pad2->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    if( 0 < tTracker->GetPlane(iPlane)->GetAnalysisMode()  ) {
      pad2->cd(iPlane);
      hHitPixMult[iPlane-1]->DrawCopy();
      //hHitPixMult[iPlane-1]->Delete();
    }
  }
  ccumulhit2->Update();

  // Time stamp of hits
  // JB 2015/05/25
  TCanvas *ccumulhit21;
  g = gROOT->FindObject("ccumulhit21") ;
  if (g) {
    ccumulhit21 = (TCanvas*)g;
  }
  else {
    ccumulhit21 = new TCanvas("ccumulhit21", "Hit time stamp", 5, 5,800,700);
  }
  ccumulhit21->Clear();
  ccumulhit21->UseCurrentStyle();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad21 = new TPad("pad21","",0.,0.,1.,0.965);
  pad21->Draw();
  pad21->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    if( 0 < tTracker->GetPlane(iPlane)->GetAnalysisMode()  ) {
      pad21->cd(iPlane);
      if( hHitTimeStamp[iPlane-1]->GetEntries()>0 ) hHitTimeStamp[iPlane-1]->DrawCopy();
    }
  }
  ccumulhit21->Update();

  // # Hits VS Event Number
  // QL 2015/10/23
  TCanvas *ccumulhit22;
  g = gROOT->FindObject("ccumulhit22") ;
  if (g) {
    ccumulhit22 = (TCanvas*)g;
  }
  else {
    ccumulhit22 = new TCanvas("ccumulhit22", "# hits VS event number", 5, 5,800,700);
  }
  ccumulhit22->Clear();
  ccumulhit22->UseCurrentStyle();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad22 = new TPad("pad22","",0.,0.,1.,0.965);
  pad22->Draw();
  pad22->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    if( 0 < tTracker->GetPlane(iPlane)->GetAnalysisMode()  ) {
      pad22->cd(iPlane);
      if( hNHitsVSEventN[iPlane-1]->GetEntries()>0 ) hNHitsVSEventN[iPlane-1]->DrawCopy();
    }
  }
  ccumulhit22->Update();

  // Number of hits per event
  double MaxAxisRanges = -1.0e+20;
  TCanvas *ccumulhit3;
  g = gROOT->FindObject("ccumulhit3") ;
  if (g) {
    ccumulhit3 = (TCanvas*)g;
  }
  else {
    ccumulhit3 = new TCanvas("ccumulhit3", "Nb of hits per event", 5, 5,800,700);
  }
  ccumulhit3->Clear();
  ccumulhit3->UseCurrentStyle();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad3 = new TPad("pad3","",0.,0.,1.,0.965);
  pad3->Draw();
  pad3->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    if( 0 < tTracker->GetPlane(iPlane)->GetAnalysisMode()  ) {
      pad3->cd(iPlane);

      double MyMaxRange = -999.0;
      for(int i=0;i<hNHitsPerEvent[iPlane-1]->GetXaxis()->GetNbins();i++) {
        int index = hNHitsPerEvent[iPlane-1]->GetXaxis()->GetNbins() - i;
        if(hNHitsPerEvent[iPlane-1]->GetBinContent(index) > 0.0) {
          MyMaxRange  = hNHitsPerEvent[iPlane-1]->GetBinCenter(index);
          MyMaxRange += 0.5*hNHitsPerEvent[iPlane-1]->GetBinWidth(index);
          break;
        }
      }
      MyMaxRange += 0.10*(MyMaxRange - hNHitsPerEvent[iPlane-1]->GetXaxis()->GetXmin());
      if(MaxAxisRanges < MyMaxRange) MaxAxisRanges = MyMaxRange;
      hNHitsPerEvent[iPlane-1]->SetAxisRange(hNHitsPerEvent[iPlane-1]->GetXaxis()->GetXmin(),
                                             MyMaxRange,
                                             "X");
      hNHitsPerEvent[iPlane-1]->DrawCopy();
    }
  }
  ccumulhit3->Update();

  TLatex* latex = new TLatex();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.06);
  latex->SetTextColor(kRed);
  TLine* CorrLines[(nPlanes*(nPlanes - 1)/2)];
  int NcanvasX = 3;
  int NcanvasY = 2;
  int Ncanvas = (nPlanes*(nPlanes - 1)/2)/(NcanvasX*NcanvasY);
  int rest    = (nPlanes*(nPlanes - 1)/2) - (NcanvasX*NcanvasY)*Ncanvas;
  if(rest > 0) Ncanvas++;
  const int RealNcanvas(Ncanvas);
  TCanvas *ccumulhit3_2[RealNcanvas];
  int counter_plots = 0;
  for(int i=0;i<RealNcanvas;i++) {

    TString TheName = TString("ccumulhit3_2_") + long(i+1);

    g = gROOT->FindObject(TheName.Data()) ;
    if (g) {
      ccumulhit3_2[i] = (TCanvas*)g;
    }
    else {
      ccumulhit3_2[i] = new TCanvas(TheName.Data(), "Correlation among the planes of Nb of hits per event", 5, 5,800,700);
    }
    ccumulhit3_2[i]->Divide(NcanvasX,NcanvasY);
    for(int ipad=0;ipad<NcanvasX*NcanvasY;ipad++) {
      ccumulhit3_2[i]->cd(ipad+1);
      counter_plots++;
      if(counter_plots > (nPlanes*(nPlanes - 1)/2)) continue;

      ccumulhit3_2[i]->cd(ipad+1)->SetFillColor(10);
      ccumulhit3_2[i]->cd(ipad+1)->SetTickx(1);
      ccumulhit3_2[i]->cd(ipad+1)->SetTicky(1);
      ccumulhit3_2[i]->cd(ipad+1)->SetLeftMargin(0.20);
      ccumulhit3_2[i]->cd(ipad+1)->SetBottomMargin(0.20);
      ccumulhit3_2[i]->cd(ipad+1)->SetRightMargin(0.20);
      ccumulhit3_2[i]->cd(ipad+1)->SetLogz(1);
      hNHitsPerEventCorr[counter_plots-1]->SetAxisRange(hNHitsPerEventCorr[counter_plots-1]->GetXaxis()->GetXmin(),
                                                        MaxAxisRanges,"X");
      hNHitsPerEventCorr[counter_plots-1]->SetAxisRange(hNHitsPerEventCorr[counter_plots-1]->GetYaxis()->GetXmin(),
                                                        MaxAxisRanges,"Y");
      hNHitsPerEventCorr[counter_plots-1]->Draw("colz");

      CorrLines[counter_plots-1] = new TLine(hNHitsPerEventCorr[counter_plots-1]->GetXaxis()->GetXmin(),
                                             hNHitsPerEventCorr[counter_plots-1]->GetYaxis()->GetXmin(),
                                             MaxAxisRanges,
                                             MaxAxisRanges);
      CorrLines[counter_plots-1]->SetLineColor(2);
      CorrLines[counter_plots-1]->SetLineWidth(3);
      CorrLines[counter_plots-1]->SetLineStyle(2);
      CorrLines[counter_plots-1]->Draw();
      //GGG

      double Xpos,Ypos;
      Xpos  = hNHitsPerEventCorr[counter_plots-1]->GetXaxis()->GetXmin();
      Xpos += 0.20*(MaxAxisRanges - hNHitsPerEventCorr[counter_plots-1]->GetXaxis()->GetXmin());
      Ypos  = MaxAxisRanges;
      Ypos -= 0.05*(MaxAxisRanges - hNHitsPerEventCorr[counter_plots-1]->GetYaxis()->GetXmin());

      char yyy[300];
      sprintf(yyy,"correlation = %.3f",hNHitsPerEventCorr[counter_plots-1]->GetCorrelationFactor());
      latex->DrawLatex(Xpos,Ypos,yyy);
    }
  }


  // For analog output sensors
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    tPlane = tTracker->GetPlane(iPlane);
    if( 0 < tPlane->GetAnalysisMode() && tPlane->GetAnalysisMode() <= 2 ) {

      sprintf( name, "chitpl%d", iPlane);
      sprintf( title, "Hit properties of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
      g = gROOT->FindObject(name) ;
      if (g) {
        cHitProperties[iPlane-1] = (TCanvas*)g;
      }
      else {
        cHitProperties[iPlane-1] = new TCanvas(name, title, 5, 5+5*(iPlane-1),600,500);
      }
      cHitProperties[iPlane-1]->Clear();
      cHitProperties[iPlane-1]->UseCurrentStyle();
      cHitProperties[iPlane-1]->Divide(3,3);

      cHitProperties[iPlane-1]->cd(1);
      if( hHitSeedSN[iPlane-1]->GetEntries()>0 ) hHitSeedSN[iPlane-1]->DrawCopy();
      cHitProperties[iPlane-1]->cd(2);
      if( hHitNeighbourSN[iPlane-1]->GetEntries()>0 ) hHitNeighbourSN[iPlane-1]->DrawCopy();
      cHitProperties[iPlane-1]->cd(3);
      if( hHitSeedVsNeighbourSN[iPlane-1]->GetEntries()>0 ) hHitSeedVsNeighbourSN[iPlane-1]->DrawCopy("colz");
      cHitProperties[iPlane-1]->cd(4);
      if( hHitCharge[iPlane-1]->GetEntries()>0 ) hHitCharge[iPlane-1]->DrawCopy();
      cHitProperties[iPlane-1]->cd(5);
      if( hHitSeedCharge[iPlane-1]->GetEntries()>0 )  hHitSeedCharge[iPlane-1]->DrawCopy();
      cHitProperties[iPlane-1]->cd(6);
      if( hHitNeighbourCharge[iPlane-1]->GetEntries()>0 )  hHitNeighbourCharge[iPlane-1]->DrawCopy();
      cHitProperties[iPlane-1]->cd(7);
      if( hHitSeedVsNeighbourCharge[iPlane-1]->GetEntries()>0 )  hHitSeedVsNeighbourCharge[iPlane-1]->DrawCopy("colz");
      cHitProperties[iPlane-1]->cd(8);
      if( hHitSeedSNVsSeedCharge[iPlane-1]->GetEntries()>0 )
        hHitSeedSNVsSeedCharge[iPlane-1]->DrawCopy("colz");
      cHitProperties[iPlane-1]->cd(9);
      hNHitsPerEvent[iPlane-1]->DrawCopy();
    }
  }




  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    tPlane = tTracker->GetPlane(iPlane);
    if( 0 < tPlane->GetAnalysisMode() && tPlane->GetAnalysisMode() <= 2 ) {

      sprintf( name, "chit2pl%d", iPlane);
      sprintf( title, "Hit properties 2 of plane %d - %s - Ratio Seed and total", iPlane, tPlane->GetPlanePurpose());
      g = gROOT->FindObject(name) ;
      if (g) {
        cHitProperties2[iPlane-1] = (TCanvas*)g;
      }
      else {
        cHitProperties2[iPlane-1] = new TCanvas(name, title, 5, 5+5*(iPlane-1),600,500);
      }
      cHitProperties2[iPlane-1]->Clear();
      cHitProperties2[iPlane-1]->UseCurrentStyle();
      cHitProperties2[iPlane-1]->Divide(2,2);

      cHitProperties2[iPlane-1]->cd(1);
      if( hHitChargeRatio[iPlane-1]->GetEntries()>0 ) hHitChargeRatio[iPlane-1]->DrawCopy();
      TLine* limitLine = new TLine(cutLimit, 0, cutLimit ,hHitChargeRatio[iPlane-1]->GetMaximum());
      limitLine->SetLineStyle(2);
      limitLine->SetLineColor(2);
      limitLine->Draw("same");


      cHitProperties2[iPlane-1]->cd(2);
      if( hHitSeedCharge[iPlane-1]->GetEntries()>0 ) hHitSeedCharge[iPlane-1]->DrawCopy();
      cHitProperties2[iPlane-1]->cd(3);
      if( hHitSeedCharge_CUT[iPlane-1]->GetEntries()>0 ) hHitSeedCharge_CUT[iPlane-1]->DrawCopy();
      cHitProperties2[iPlane-1]->cd(4);
      if( hHitCharge[iPlane-1]->GetEntries()>0 )  {
        hHitSeedCharge[iPlane-1]->DrawCopy();
        hHitSeedCharge_CUT[iPlane-1]->SetFillColor(2);
        hHitSeedCharge_CUT[iPlane-1]->DrawClone("same");
      }

    }
  }

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    tPlane = tTracker->GetPlane(iPlane);
    if( 0 < tPlane->GetAnalysisMode() && tPlane->GetAnalysisMode() <= 2 ) {

      sprintf( name, "chitqtotl%d", iPlane);
      sprintf( title, "Charge planes %d - %s", iPlane, tPlane->GetPlanePurpose());
      g = gROOT->FindObject(name) ;
      if (g) {
        cHitSeedChargeSum[iPlane-1] = (TCanvas*)g;
      }
      else {
        cHitSeedChargeSum[iPlane-1] = new TCanvas(name, title, 5, 5+5*(iPlane-1),600,500);
      }
      cHitSeedChargeSum[iPlane-1]->Clear();
      cHitSeedChargeSum[iPlane-1]->UseCurrentStyle();
      if( hHitSeedCharge[iPlane-1]->GetEntries()>0 ){
        hHitSeedCharge[iPlane-1]->DrawCopy();
        hHitSeedCharge_CUT[iPlane-1]->SetFillColor(2);
        hHitSeedCharge_CUT[iPlane-1]->DrawCopy("same");
      }
    }
  }

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    tPlane = tTracker->GetPlane(iPlane);
    if( 0 < tPlane->GetAnalysisMode() && tPlane->GetAnalysisMode() <= 2 ) {

      sprintf( name, "chitqratio%d", iPlane);
      sprintf( title, "Charge Ratio in planes %d - %s", iPlane, tPlane->GetPlanePurpose());
      g = gROOT->FindObject(name) ;
      if (g) {
        cHitChargeRatio[iPlane-1] = (TCanvas*)g;
      }
      else {
        cHitChargeRatio[iPlane-1] = new TCanvas(name, title, 5, 5+5*(iPlane-1),600,500);
      }
      cHitChargeRatio[iPlane-1]->Clear();
      cHitChargeRatio[iPlane-1]->UseCurrentStyle();
      if( hHitChargeRatio[iPlane-1]->GetEntries()>0 ) hHitChargeRatio[iPlane-1]->DrawCopy();
      TLine* limitLine = new TLine(cutLimit, 0, cutLimit ,hHitChargeRatio[iPlane-1]->GetMaximum());
      limitLine->SetLineStyle(2);
      limitLine->SetLineColor(2);
      limitLine->Draw("same");
    }
  }




  Float_t norm;
  Float_t Binning;

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    if (HighestPeakPositionEv!=0)	{

      //Il faut ajouter un nouveau canvas temporaire pour celui la
      TH1F* htmp;
      TCanvas *ctmp=new TCanvas("ctmp","ctmp",10,10,500,400);
      ctmp->cd();
      htmp=(TH1F*)hHitSeedCharge[iPlane-1]->DrawClone();
      int binmax = htmp->GetXaxis()->GetXmax();
      Binning=binmax;
      float cutlimit=0.1*binmax;//Cut at 10% of the max bin
      htmp->GetXaxis()->SetRangeUser(cutlimit, binmax);

      TSpectrum *s=new TSpectrum();

      Int_t nfound = s->Search(htmp,2,"",0.5);
      Float_t *xpeaks = (Float_t*)s->GetPositionX();//Find any peak position
      Float_t xp = xpeaks[0];

      norm=HighestPeakPositionEv/xp;
      ctmp->Close();

      TCanvas *ctmp2=new TCanvas("ctmp2","ctmp2",10,10,500,400);
      ctmp2->cd();
      TH1F* htmp2;
      htmp2=(TH1F*)hHitSeedCharge[iPlane-1]->DrawClone();
      Double_t newXmin= norm*htmp2->GetXaxis()->GetXmin();
      Double_t newXmax= norm*htmp2->GetXaxis()->GetXmax();
      Double_t nBinsh3=htmp2->GetXaxis()->GetNbins();
      htmp2->SetBins(nBinsh3, newXmin, newXmax);
      htmp2->GetXaxis()->SetRangeUser(MinSpectrum,MaxSpectrum);// From 1keV to 7keV
      htmp2->Draw();
      ctmp2->Close();
      cHitProperties[iPlane-1]->cd(5);
      hHitSeedCharge[iPlane-1]=htmp2;
      hHitSeedCharge[iPlane-1]->GetXaxis()->SetTitle("Seed Charge (eV)");
      hHitSeedCharge[iPlane-1]->GetYaxis()->SetTitleSize(0.05);
      hHitSeedCharge[iPlane-1]->GetXaxis()->SetTitleSize(0.05);
      hHitSeedCharge[iPlane-1]->DrawCopy();

    }
    else {
      TH1F* htmp;
      TCanvas *ctmp=new TCanvas("ctmp","ctmp",10,10,500,400);
      ctmp->cd();
      htmp=(TH1F*)hHitSeedCharge[iPlane-1]->DrawClone();
      int binmax = htmp->GetXaxis()->GetXmax();
      Binning=binmax;
      float cutlimit=0.1*binmax;//Cut at 10% of the max bin
      htmp->GetXaxis()->SetRangeUser(cutlimit, binmax);

      TSpectrum *s=new TSpectrum();

      Int_t nfound = s->Search(htmp,2,"",0.5);
      Float_t *xpeaks = (Float_t*)s->GetPositionX();//Find any peak position
      Float_t xp = xpeaks[0];

      norm=5900/xp;
      ctmp->Close();
    }
  }

  if (readNormFromFile==kFALSE) {
    ofstream normTempFile;
    normTempFile.open("Scripts/normtemp.txt");
    normTempFile<<norm;
    normTempFile.close();
  }

  if (normalizeADCspectrum==kFALSE && readNormFromFile==kFALSE) {
    ofstream BinningTempFile;
    BinningTempFile.open("Scripts/binningtemp.txt");
    BinningTempFile<<Binning;
    BinningTempFile.close();
  }


  // Fitting hhitseedq
  TH1F** hHitSeedChargeFit=new TH1F*[nPlanes];
  TH1F** hHitSeedChargeFitNoBg=new TH1F*[nPlanes];
  TH1F** hb=new TH1F*[nPlanes];
  TF1* Fit_func [nPlanes];
  TF1* Fit_func_Kalpha [nPlanes];
  TF1* Fit_func_Kbeta [nPlanes];

  TF1* Fit_func_NoBg [nPlanes];
  TF1* Fit_func_Kalpha_NoBg [nPlanes];
  TF1* Fit_func_Kbeta_NoBg [nPlanes];



  TH1F** hHitSeedChargeBifFit=new TH1F*[nPlanes];
  TH1F** hHitSeedChargeBifFitNoBg=new TH1F*[nPlanes];

  TF1* BifFit_func [nPlanes];
  TF1* BifFit_func_Kalpha [nPlanes];
  TF1* BifFit_func_Kbeta [nPlanes];

  TF1* BifFit_func_NoBg [nPlanes];
  TF1* BifFit_func_Kalpha_NoBg [nPlanes];
  TF1* BifFit_func_Kbeta_NoBg [nPlanes];




  TSpectrum* spectrum [nPlanes];


  double Rfit_keV[2];
  double Rfit_ADC[2];
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++)
  {
    hHitSeedChargeFit[iPlane-1]=(TH1F*)hHitSeedCharge[iPlane-1]->Clone();
    hHitSeedChargeFit[iPlane-1]->SetName("hhitseedq1_fit");

    hHitSeedChargeFitNoBg[iPlane-1]=(TH1F*)hHitSeedCharge[iPlane-1]->Clone();
    hHitSeedChargeFitNoBg[iPlane-1]->SetName("hhitseedq1_fit_no_bg");

    hHitSeedChargeBifFit[iPlane-1]=(TH1F*)hHitSeedCharge[iPlane-1]->Clone();
    hHitSeedChargeBifFit[iPlane-1]->SetName("hhitseedq1_bifurcated_fit");

    hHitSeedChargeBifFitNoBg[iPlane-1]=(TH1F*)hHitSeedCharge[iPlane-1]->Clone();
    hHitSeedChargeBifFitNoBg[iPlane-1]->SetName("hhitseedq1_bifuracted_fit_no_bg");

    if (fitXray!=0)
    {
      double Peak_position = -999;
      double Peak_Height   = -1.0e+20;
      double Xmin_peak_search = 500.0;


      spectrum[iPlane-1] = new TSpectrum();
      hb[iPlane-1] = (TH1F*)spectrum[iPlane-1]->Background(hHitSeedChargeFit[iPlane-1],70);
      hb[iPlane-1]->SetLineColor(kBlack);
      hb[iPlane-1]->SetLineWidth(2);
      hb[iPlane-1]->SetLineStyle(2);

      for(int j=0;j<hHitSeedChargeFit[iPlane-1]->GetXaxis()->GetNbins();j++)
      {
        double v,e,vb,eb,vorig;
        vb           = hb[iPlane-1]->GetBinContent(j+1);
        eb           = sqrt(hb[iPlane-1]->GetBinContent(j+1));
        v            = hHitSeedChargeFitNoBg[iPlane-1]->GetBinContent(j+1);
        vorig     = hHitSeedChargeFitNoBg[iPlane-1]->GetBinContent(j+1);
        e            = sqrt(hHitSeedChargeFitNoBg[iPlane-1]->GetBinContent(j+1));

        v           -= vb;
        e            = sqrt(pow(e,2) + pow(eb,2));
        if(hHitSeedChargeFitNoBg[iPlane-1]->FindBin(Peak_position) == j+1) Peak_Height -= vb;




        hb[iPlane-1]->SetBinContent(j+1,vb);
        hHitSeedChargeFit[iPlane-1]->SetBinContent(j+1,vorig);

        hHitSeedChargeFitNoBg[iPlane-1]->SetBinContent(j+1,v);
        hHitSeedChargeFitNoBg[iPlane-1]->SetBinError(j+1,  e);

        hHitSeedChargeBifFit[iPlane-1]->SetBinContent(j+1,vorig);

        hHitSeedChargeBifFitNoBg[iPlane-1]->SetBinContent(j+1,v);
        hHitSeedChargeBifFitNoBg[iPlane-1]->SetBinError(j+1,  e);


      }



      // ===============================================
      // Looking for the calibration peak
      // ===============================================
      for(int j=0;j<hHitSeedChargeBifFitNoBg[iPlane-1]->GetXaxis()->GetNbins();j++)
      {
        double c = hHitSeedChargeBifFitNoBg[iPlane-1]->GetBinCenter(j+1);
        double v = hHitSeedChargeBifFitNoBg[iPlane-1]->GetBinContent(j+1);
        if(c < Xmin_peak_search) continue;

        if(Peak_Height < v)
        {
          Peak_Height   = v;
          Peak_position = c;
        }
      }
      cout <<"Peak height: " <<Peak_Height << ", Peak position: "<< Peak_position <<endl;




      Double_t calibrationEnergy=-1.;//keV
      Double_t secondPeak=-1.;//keV




      if (fitXray==1)// 55Fe
      {
        calibrationEnergy=5.89502;//keV
        secondPeak=6.49045;
        //Limits for the fit in energy
        Rfit_keV[0] = 4.9; //keV
        Rfit_keV[1] = 7.0; //keV
        //Conversion of the limits in ADC units
        Rfit_ADC[0] = Rfit_keV[0]*(Peak_position/calibrationEnergy);
        Rfit_ADC[1] = Rfit_keV[1]*(Peak_position/calibrationEnergy);
      }

      else if (fitXray==2)// Cr
      {
        calibrationEnergy=5.41472;//keV
        secondPeak=2.*calibrationEnergy;
        //Limits for the fit in energy
        Rfit_keV[0] = 4.0; //keV
        Rfit_keV[1] = 13.0; //keV
        //Conversion of the limits in ADC units
        Rfit_ADC[0] = Rfit_keV[0]*(Peak_position/calibrationEnergy);
        Rfit_ADC[1] = Rfit_keV[1]*(Peak_position/calibrationEnergy);
      }

      else if (fitXray==3)// Cu
      {
        calibrationEnergy=8.04778;//keV
        secondPeak=2.*calibrationEnergy;
        //Limits for the fit in energy
        Rfit_keV[0] = 7.0; //keV
        Rfit_keV[1] = 18.0; //keV
        //Conversion of the limits in ADC units
        Rfit_ADC[0] = Rfit_keV[0]*(Peak_position/calibrationEnergy);
        Rfit_ADC[1] = Rfit_keV[1]*(Peak_position/calibrationEnergy);
      }

      else if (fitXray==4)// Mo
      {
        calibrationEnergy=17.47934;//keV
        secondPeak=19.9; //Arbitrary second peak energy in the ADC range below
        //Limits for the fit in energy
        Rfit_keV[0] = 14.0; //keV
        Rfit_keV[1] = 20.0; //keV
        //Conversion of the limits in ADC units
        Rfit_ADC[0] = Rfit_keV[0]*(Peak_position/calibrationEnergy);
        Rfit_ADC[1] = Rfit_keV[1]*(Peak_position/calibrationEnergy);
      }

      double left_sigma  = -999;
      double right_sigma = -999;
      int SomeN = 0.0;
      for(int j=0;j<hHitSeedChargeFit[iPlane-1]->GetXaxis()->GetNbins()-1;j++)
      {
        double x1 = hHitSeedChargeFit[iPlane-1]->GetBinCenter(j+1);
        double x2 = hHitSeedChargeFit[iPlane-1]->GetBinCenter(j+2);
        double y1 = hHitSeedChargeFit[iPlane-1]->GetBinContent(j+1);
        double y2 = hHitSeedChargeFit[iPlane-1]->GetBinContent(j+2);
        if(x1 < Rfit_ADC[0]) continue;
        if(x2 > Rfit_ADC[1]) continue;

        SomeN += y1;

        if(y1 <= Peak_Height*0.5 && y2 > Peak_Height*0.5 && left_sigma  < 0.0) left_sigma  = 0.5*(x1+x2);
        if(y1 >= Peak_Height*0.5 && y2 < Peak_Height*0.5 && right_sigma < 0.0) right_sigma = 0.5*(x1+x2);
      }
      cout<<"ls: "<<left_sigma<<"rs: "<<right_sigma<<endl;
      double MyMean  = Peak_position;
      cout<<Peak_position<<endl;
      double MySigma = 0.5*(right_sigma - left_sigma);
      double MyAlpha = 1.5;
      double Myn     = 10.0;
      double MyN1    = SomeN*0.9;
      double MyN2    = SomeN*0.1;
      double MyN     = SomeN;
      double MyFrac  = 0.1;

      double RMyMean[2];
      double RMySigma[2];
      double RMyAlpha[2];
      double RMyn[2];
      double RMyN1[2];
      double RMyN2[2];
      double RMyN[2];
      double RMyFrac[2];
      RMyMean[0]  = Rfit_ADC[0];
      RMyMean[1]  = Rfit_ADC[1];
      RMySigma[0] = MySigma*1.0e-5;
      RMySigma[1] = MySigma*1.0e+1;
      RMyAlpha[0] = 0.0;
      RMyAlpha[1] = 5.0;
      RMyn[0]     = 1.0;
      RMyn[1]     = 20.0;
      RMyN1[0]    = 0.0;
      RMyN1[1]    = 1.0e+3*(MyN1+MyN2);
      RMyN2[0]    = 0.0;
      RMyN2[1]    = 1.0e+3*(MyN1+MyN2);
      RMyN[0]     = 0.0;
      RMyN[1]     = 1.0e+3*MyN;
      RMyFrac[0]  = 0.0;
      RMyFrac[1]  = 1.0;

      cout << endl;
      cout  << " Peak Position at " << Peak_position << " ADCu, with height " << Peak_Height << ". Fit range (" << Rfit_keV[0] << "," << Rfit_keV[1] << ") keV => (" << Rfit_ADC[0] << "," << Rfit_ADC[1] << ")" << endl;
      cout<< "Nb of entries in range with background: "<<hHitSeedChargeFit[iPlane-1]->Integral(RMyMean[0],RMyMean[1])<<endl;
      cout << "Initial values of fit parameters:" << endl;
      cout << "mean  = " << MyMean  << "  (" << RMyMean[0]  << "," << RMyMean[1]  << ") ADCu" << endl;
      cout << "sigma = " << MySigma << "  (" << RMySigma[0] << "," << RMySigma[1] << ") ADCu" << endl;
      cout << "alpha = " << MyAlpha << "  (" << RMyAlpha[0] << "," << RMyAlpha[1] << ")"      << endl;
      cout << "n     = " << Myn     << "  (" << RMyn[0]     << "," << RMyn[1]     << ")"      << endl;
      // TF1 * f = new TF1("f",fptr,&MyFunction::Evaluate,0,1,npar,"MyFunction","Evaluate");   // create TF1 class.



      Char_t fitResultsFileName[300];
      sprintf(fitResultsFileName,"%sFitResults_%d.txt",fSession->GetResultDirName().Data(),fSession->GetRunNumber());
      sprintf(fitResultsFileName,"%s", fTool.LocalizeDirName(fitResultsFileName)); // JB 2011/07/07
      ofstream fitResultsFile;
      fitResultsFile.open(fitResultsFileName);

      if (fitXray==1){fitResultsFile<<"================ 55Fe FITS ================"<<"\n";}
      else if (fitXray==2){fitResultsFile<<"================= Cr FITS ================="<<"\n";}
      else if (fitXray==3){fitResultsFile<<"================= Cu FITS ================="<<"\n";}
      else if (fitXray==4){fitResultsFile<<"================= Mo FITS ================="<<"\n";}




      /*==========================================================================================================================
       ============================================================================================================================
       ================================================== BIFURCATED FIT OF 55Fe ==================================================
       ============================================================================================================================
       ==========================================================================================================================*/



      BifFit_func[iPlane-1] = new TF1("BiffitCB", &fTool, &DGlobalTools::fpeaksBifurcatedCB55Fe, Rfit_ADC[0], Rfit_ADC[1], 12, "DGlobalTools", "fpeaksBifurcatedCB55Fe");


      //BifFit_func[iPlane-1] = new TF1("fitCB", fTool.fpeaksBifurcatedCB, Rfit_ADC[0], Rfit_ADC[1], 7);
      if (fitXray==1)
      {
        BifFit_func[iPlane-1]->SetParName(0,"#mu K#alpha (ADCu)");
        BifFit_func[iPlane-1]->SetParName(1,"#mu K#beta (ADCu)");
        BifFit_func[iPlane-1]->SetParName(2,"#sigma_{K#alphaLEFT} (ADCu)");
        BifFit_func[iPlane-1]->SetParName(3,"#sigma_{K#betaLEFT} (ADCu)");
        BifFit_func[iPlane-1]->SetParName(4,"#sigma_{K#alphaRIGHT} (ADCu)");
        BifFit_func[iPlane-1]->SetParName(5,"#sigma_{K#betaLEFT} (ADCu)");
        BifFit_func[iPlane-1]->SetParName(6,"#alpha K#alpha");
        BifFit_func[iPlane-1]->SetParName(7,"#alpha K#beta");
        BifFit_func[iPlane-1]->SetParName(8,"n K#alpha");
        BifFit_func[iPlane-1]->SetParName(9,"n K#beta");
        BifFit_func[iPlane-1]->SetParName(10,"N_{K#alpha}");
        BifFit_func[iPlane-1]->SetParName(11,"N_{K#beta}");
      }
      else // Changing names for other sources than 55Fe => Harmonics
      {
        BifFit_func[iPlane-1]->SetParName(0,"#mu K#alpha (ADCu)");
        BifFit_func[iPlane-1]->SetParName(1,"2 x #mu K#alpha (ADCu)");
        BifFit_func[iPlane-1]->SetParName(2,"#sigma_{K#alphaLEFT} (ADCu)");
        BifFit_func[iPlane-1]->SetParName(3,"#sigma_{2xK#alphaLEFT} (ADCu)");
        BifFit_func[iPlane-1]->SetParName(4,"#sigma_{K#alphaRIGHT} (ADCu)");
        BifFit_func[iPlane-1]->SetParName(5,"#sigma_{2xK#alphaLEFT} (ADCu)");
        BifFit_func[iPlane-1]->SetParName(6,"#alpha K#alpha");
        BifFit_func[iPlane-1]->SetParName(7,"#alpha 2xK#alpha");
        BifFit_func[iPlane-1]->SetParName(8,"n K#alpha");
        BifFit_func[iPlane-1]->SetParName(9,"n 2xK#alpha");
        BifFit_func[iPlane-1]->SetParName(10,"N_{K#alpha}");
        BifFit_func[iPlane-1]->SetParName(11,"N_{2xK#alpha}");
      }



      BifFit_func[iPlane-1]->SetParameter(0,MyMean);
      BifFit_func[iPlane-1]->SetParameter(1,MyMean*(secondPeak/calibrationEnergy));
      BifFit_func[iPlane-1]->SetParameter(2,MySigma);
      BifFit_func[iPlane-1]->SetParameter(3,MySigma);
      BifFit_func[iPlane-1]->SetParameter(4,MySigma);
      BifFit_func[iPlane-1]->SetParameter(5,MySigma);
      BifFit_func[iPlane-1]->SetParameter(6,MyAlpha);
      BifFit_func[iPlane-1]->SetParameter(7,MyAlpha);
      BifFit_func[iPlane-1]->SetParameter(8,Myn);
      BifFit_func[iPlane-1]->SetParameter(9,Myn);
      BifFit_func[iPlane-1]->SetParameter(10,MyN1);
      if (fitXray==4) //Forcing parameters for Mo where there is no second peak
      {
        BifFit_func[iPlane-1]->FixParameter(11,0.0);
      }
      else
      {
        BifFit_func[iPlane-1]->SetParameter(11,MyN2);
      }

      BifFit_func[iPlane-1]->SetParLimits(0,RMyMean[0],RMyMean[1]);
      BifFit_func[iPlane-1]->SetParLimits(1,RMyMean[0],RMyMean[1]);
      BifFit_func[iPlane-1]->SetParLimits(2,RMySigma[0],RMySigma[1]);
      BifFit_func[iPlane-1]->SetParLimits(3,RMySigma[0],RMySigma[1]);
      BifFit_func[iPlane-1]->SetParLimits(4,RMySigma[0],RMySigma[1]);
      BifFit_func[iPlane-1]->SetParLimits(5,RMySigma[0],RMySigma[1]);
      BifFit_func[iPlane-1]->SetParLimits(6,RMyAlpha[0],RMyAlpha[1]);
      BifFit_func[iPlane-1]->SetParLimits(7,RMyAlpha[0],RMyAlpha[1]);
      BifFit_func[iPlane-1]->SetParLimits(8,RMyn[0],RMyn[1]);
      BifFit_func[iPlane-1]->SetParLimits(9,RMyn[0],RMyn[1]);
      BifFit_func[iPlane-1]->SetParLimits(10,RMyN1[0],RMyN1[1]);
      BifFit_func[iPlane-1]->SetParLimits(11,RMyN2[0],RMyN2[1]);

      BifFit_func[iPlane-1]->SetLineColor(kBlue);
      BifFit_func[iPlane-1]->SetLineWidth(2);
      BifFit_func[iPlane-1]->SetNpx(10000);

      hHitSeedChargeBifFit[iPlane-1]->Fit(BifFit_func[iPlane-1],"RQ");



      cout<<"\n================ BIFURCATED FIT WITH BACKGROUND ===================="<<endl;
      cout<<BifFit_func[iPlane-1]->GetParName(0)<<": "<<BifFit_func[iPlane-1]->GetParameter(0)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(0)<<endl;
      cout<<BifFit_func[iPlane-1]->GetParName(1)<<": "<<BifFit_func[iPlane-1]->GetParameter(1)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(1)<<endl;
      cout<<BifFit_func[iPlane-1]->GetParName(2)<<": "<<BifFit_func[iPlane-1]->GetParameter(2)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(2)<<endl;
      cout<<BifFit_func[iPlane-1]->GetParName(3)<<": "<<BifFit_func[iPlane-1]->GetParameter(3)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(3)<<endl;
      cout<<BifFit_func[iPlane-1]->GetParName(4)<<": "<<BifFit_func[iPlane-1]->GetParameter(4)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(4)<<endl;
      cout<<BifFit_func[iPlane-1]->GetParName(5)<<": "<<BifFit_func[iPlane-1]->GetParameter(5)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(5)<<endl;
      cout<<BifFit_func[iPlane-1]->GetParName(6)<<": "<<BifFit_func[iPlane-1]->GetParameter(6)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(6)<<endl;
      cout<<"Energy Resolution at "<<calibrationEnergy<<" keV: "<<0.5*(BifFit_func[iPlane-1]->GetParameter(2)+BifFit_func[iPlane-1]->GetParameter(4))*2.355*calibrationEnergy*1000./BifFit_func[iPlane-1]->GetParameter(0) <<" eV"<<endl;
      cout<<"Energy Resolution at "<<secondPeak<<" keV: "<<0.5*(BifFit_func[iPlane-1]->GetParameter(3)+BifFit_func[iPlane-1]->GetParameter(5))*2.355*secondPeak*1000./BifFit_func[iPlane-1]->GetParameter(1) <<" eV"<<endl;
      cout<<BifFit_func[iPlane-1]->GetParName(7)<<": "<<BifFit_func[iPlane-1]->GetParameter(7)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(7)<<endl;
      cout<<BifFit_func[iPlane-1]->GetParName(8)<<": "<<BifFit_func[iPlane-1]->GetParameter(8)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(8)<<endl;
      cout<<BifFit_func[iPlane-1]->GetParName(9)<<": "<<BifFit_func[iPlane-1]->GetParameter(9)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(9)<<endl;
      cout<<BifFit_func[iPlane-1]->GetParName(10)<<": "<<BifFit_func[iPlane-1]->GetParameter(10)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(10)<<endl;
      cout<<BifFit_func[iPlane-1]->GetParName(11)<<": "<<BifFit_func[iPlane-1]->GetParameter(11)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(11)<<endl;
      cout<<"Integral between "<<Rfit_keV[0]<<" and "<<Rfit_keV[1]<<" keV: "<<hHitSeedChargeBifFit[iPlane-1]->Integral(BifFit_func[iPlane-1]->GetParameter(0)*Rfit_keV[0]/calibrationEnergy, BifFit_func[iPlane-1]->GetParameter(0)*Rfit_keV[1]/calibrationEnergy)<<endl;


      fitResultsFile<<"================ BIFURCATED FIT WITH BACKGROUND ===================="<<"\n";
      fitResultsFile<<BifFit_func[iPlane-1]->GetParName(0)<<": "<<BifFit_func[iPlane-1]->GetParameter(0)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(0)<<"\n";
      fitResultsFile<<BifFit_func[iPlane-1]->GetParName(1)<<": "<<BifFit_func[iPlane-1]->GetParameter(1)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(1)<<"\n";
      fitResultsFile<<BifFit_func[iPlane-1]->GetParName(2)<<": "<<BifFit_func[iPlane-1]->GetParameter(2)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(2)<<"\n";
      fitResultsFile<<BifFit_func[iPlane-1]->GetParName(3)<<": "<<BifFit_func[iPlane-1]->GetParameter(3)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(3)<<"\n";
      fitResultsFile<<BifFit_func[iPlane-1]->GetParName(4)<<": "<<BifFit_func[iPlane-1]->GetParameter(4)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(4)<<"\n";
      fitResultsFile<<BifFit_func[iPlane-1]->GetParName(5)<<": "<<BifFit_func[iPlane-1]->GetParameter(5)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(5)<<"\n";
      fitResultsFile<<BifFit_func[iPlane-1]->GetParName(6)<<": "<<BifFit_func[iPlane-1]->GetParameter(6)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(6)<<"\n";
      fitResultsFile<<"Energy Resolution at "<<calibrationEnergy<<" keV: "<<0.5*(BifFit_func[iPlane-1]->GetParameter(2)+BifFit_func[iPlane-1]->GetParameter(4))*2.355*calibrationEnergy*1000./BifFit_func[iPlane-1]->GetParameter(0) <<" eV"<<"\n";
      fitResultsFile<<"Energy Resolution at "<<secondPeak<<" keV: "<<0.5*(BifFit_func[iPlane-1]->GetParameter(3)+BifFit_func[iPlane-1]->GetParameter(5))*2.355*secondPeak*1000./BifFit_func[iPlane-1]->GetParameter(1) <<" eV"<<"\n";
      fitResultsFile<<BifFit_func[iPlane-1]->GetParName(7)<<": "<<BifFit_func[iPlane-1]->GetParameter(7)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(7)<<"\n";
      fitResultsFile<<BifFit_func[iPlane-1]->GetParName(8)<<": "<<BifFit_func[iPlane-1]->GetParameter(8)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(8)<<"\n";
      fitResultsFile<<BifFit_func[iPlane-1]->GetParName(9)<<": "<<BifFit_func[iPlane-1]->GetParameter(9)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(9)<<"\n";
      fitResultsFile<<BifFit_func[iPlane-1]->GetParName(10)<<": "<<BifFit_func[iPlane-1]->GetParameter(10)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(10)<<"\n";
      fitResultsFile<<BifFit_func[iPlane-1]->GetParName(11)<<": "<<BifFit_func[iPlane-1]->GetParameter(11)<<" +/- "<<BifFit_func[iPlane-1]->GetParError(11)<<"\n";
      fitResultsFile<<"Integral between "<<Rfit_keV[0]<<" and "<<Rfit_keV[1]<<" keV: "<<hHitSeedChargeBifFit[iPlane-1]->Integral(BifFit_func[iPlane-1]->GetParameter(0)*Rfit_keV[0]/calibrationEnergy, BifFit_func[iPlane-1]->GetParameter(0)*Rfit_keV[1]/calibrationEnergy)<<"\n";


      /*===================================================================================
       ================================ FIT WITHOUT BACKGROUND =============================
       =====================================================================================*/
      BifFit_func_NoBg[iPlane-1] = new TF1("BiffitCB_nobg", &fTool, &DGlobalTools::fpeaksBifurcatedCB55Fe, Rfit_ADC[0], Rfit_ADC[1], 12, "DGlobalTools", "fpeaksBifurcatedCB55Fe");
      //BifFit_func_NoBg[iPlane-1] = new TF1("fitCB", fTool.fpeaksBifurcatedCB, Rfit_ADC[0], Rfit_ADC[1], 7);
      if (fitXray==1)
      {
        BifFit_func_NoBg[iPlane-1]->SetParName(0,"#mu K#alpha (ADCu)");
        BifFit_func_NoBg[iPlane-1]->SetParName(1,"#mu K#beta (ADCu)");
        BifFit_func_NoBg[iPlane-1]->SetParName(2,"#sigma_{K#alphaLEFT} (ADCu)");
        BifFit_func_NoBg[iPlane-1]->SetParName(3,"#sigma_{K#betaLEFT} (ADCu)");
        BifFit_func_NoBg[iPlane-1]->SetParName(4,"#sigma_{K#alphaRIGHT} (ADCu)");
        BifFit_func_NoBg[iPlane-1]->SetParName(5,"#sigma_{K#betaLEFT} (ADCu)");
        BifFit_func_NoBg[iPlane-1]->SetParName(6,"#alpha K#alpha");
        BifFit_func_NoBg[iPlane-1]->SetParName(7,"#alpha K#beta");
        BifFit_func_NoBg[iPlane-1]->SetParName(8,"n K#alpha");
        BifFit_func_NoBg[iPlane-1]->SetParName(9,"n K#beta");
        BifFit_func_NoBg[iPlane-1]->SetParName(10,"N_{K#alpha}");
        BifFit_func_NoBg[iPlane-1]->SetParName(11,"N_{K#beta}");
      }
      else // Changing names for other sources than 55Fe => Harmonics
      {
        BifFit_func_NoBg[iPlane-1]->SetParName(0,"#mu K#alpha (ADCu)");
        BifFit_func_NoBg[iPlane-1]->SetParName(1,"2 x #mu K#alpha (ADCu)");
        BifFit_func_NoBg[iPlane-1]->SetParName(2,"#sigma_{K#alphaLEFT} (ADCu)");
        BifFit_func_NoBg[iPlane-1]->SetParName(3,"#sigma_{2xK#alphaLEFT} (ADCu)");
        BifFit_func_NoBg[iPlane-1]->SetParName(4,"#sigma_{K#alphaRIGHT} (ADCu)");
        BifFit_func_NoBg[iPlane-1]->SetParName(5,"#sigma_{2xK#alphaLEFT} (ADCu)");
        BifFit_func_NoBg[iPlane-1]->SetParName(6,"#alpha K#alpha");
        BifFit_func_NoBg[iPlane-1]->SetParName(7,"#alpha 2xK#alpha");
        BifFit_func_NoBg[iPlane-1]->SetParName(8,"n K#alpha");
        BifFit_func_NoBg[iPlane-1]->SetParName(9,"n 2xK#alpha");
        BifFit_func_NoBg[iPlane-1]->SetParName(10,"N_{K#alpha}");
        BifFit_func_NoBg[iPlane-1]->SetParName(11,"N_{2xK#alpha}");
      }



      BifFit_func_NoBg[iPlane-1]->SetParameter(0,MyMean);
      BifFit_func_NoBg[iPlane-1]->SetParameter(1,MyMean*(secondPeak/calibrationEnergy));
      BifFit_func_NoBg[iPlane-1]->SetParameter(2,MySigma);
      BifFit_func_NoBg[iPlane-1]->SetParameter(3,MySigma);
      BifFit_func_NoBg[iPlane-1]->SetParameter(4,MySigma);
      BifFit_func_NoBg[iPlane-1]->SetParameter(5,MySigma);
      BifFit_func_NoBg[iPlane-1]->SetParameter(6,MyAlpha);
      BifFit_func_NoBg[iPlane-1]->SetParameter(7,MyAlpha);
      BifFit_func_NoBg[iPlane-1]->SetParameter(8,Myn);
      BifFit_func_NoBg[iPlane-1]->SetParameter(9,Myn);
      BifFit_func_NoBg[iPlane-1]->SetParameter(10,MyN1);
      if (fitXray==4) //Forcing parameters for Mo where there is no second peak
      {
        BifFit_func_NoBg[iPlane-1]->FixParameter(11,0.0);
      }
      else
      {
        BifFit_func_NoBg[iPlane-1]->SetParameter(11,MyN2);
      }

      BifFit_func_NoBg[iPlane-1]->SetParLimits(0,RMyMean[0],RMyMean[1]);
      BifFit_func_NoBg[iPlane-1]->SetParLimits(1,RMyMean[0],RMyMean[1]);
      BifFit_func_NoBg[iPlane-1]->SetParLimits(2,RMySigma[0],RMySigma[1]);
      BifFit_func_NoBg[iPlane-1]->SetParLimits(3,RMySigma[0],RMySigma[1]);
      BifFit_func_NoBg[iPlane-1]->SetParLimits(4,RMySigma[0],RMySigma[1]);
      BifFit_func_NoBg[iPlane-1]->SetParLimits(5,RMySigma[0],RMySigma[1]);
      BifFit_func_NoBg[iPlane-1]->SetParLimits(6,RMyAlpha[0],RMyAlpha[1]);
      BifFit_func_NoBg[iPlane-1]->SetParLimits(7,RMyAlpha[0],RMyAlpha[1]);
      BifFit_func_NoBg[iPlane-1]->SetParLimits(8,RMyn[0],RMyn[1]);
      BifFit_func_NoBg[iPlane-1]->SetParLimits(9,RMyn[0],RMyn[1]);
      BifFit_func_NoBg[iPlane-1]->SetParLimits(10,RMyN1[0],RMyN1[1]);
      BifFit_func_NoBg[iPlane-1]->SetParLimits(11,RMyN2[0],RMyN2[1]);

      BifFit_func_NoBg[iPlane-1]->SetLineColor(kBlue);
      BifFit_func_NoBg[iPlane-1]->SetLineWidth(2);
      BifFit_func_NoBg[iPlane-1]->SetNpx(10000);

      hHitSeedChargeBifFitNoBg[iPlane-1]->Fit(BifFit_func_NoBg[iPlane-1],"RQ");



      cout<<"\n================ BIFURCATED FIT WITHOUT BACKGROUND ===================="<<endl;
      cout<<BifFit_func_NoBg[iPlane-1]->GetParName(0)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(0)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(0)<<endl;
      cout<<BifFit_func_NoBg[iPlane-1]->GetParName(1)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(1)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(1)<<endl;
      cout<<BifFit_func_NoBg[iPlane-1]->GetParName(2)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(2)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(2)<<endl;
      cout<<BifFit_func_NoBg[iPlane-1]->GetParName(3)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(3)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(3)<<endl;
      cout<<BifFit_func_NoBg[iPlane-1]->GetParName(4)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(4)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(4)<<endl;
      cout<<BifFit_func_NoBg[iPlane-1]->GetParName(5)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(5)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(5)<<endl;
      cout<<BifFit_func_NoBg[iPlane-1]->GetParName(6)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(6)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(6)<<endl;
      cout<<"Energy Resolution at "<<calibrationEnergy<<" keV: "<<0.5*(BifFit_func_NoBg[iPlane-1]->GetParameter(2)+BifFit_func_NoBg[iPlane-1]->GetParameter(4))*2.355*calibrationEnergy*1000./BifFit_func_NoBg[iPlane-1]->GetParameter(0) <<" eV"<<endl;
      cout<<"Energy Resolution at "<<secondPeak<<" keV: "<<0.5*(BifFit_func_NoBg[iPlane-1]->GetParameter(3)+BifFit_func_NoBg[iPlane-1]->GetParameter(5))*2.355*secondPeak*1000./BifFit_func_NoBg[iPlane-1]->GetParameter(1) <<" eV"<<endl;
      cout<<BifFit_func_NoBg[iPlane-1]->GetParName(7)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(7)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(7)<<endl;
      cout<<BifFit_func_NoBg[iPlane-1]->GetParName(8)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(8)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(8)<<endl;
      cout<<BifFit_func_NoBg[iPlane-1]->GetParName(9)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(9)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(9)<<endl;
      cout<<BifFit_func_NoBg[iPlane-1]->GetParName(10)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(10)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(10)<<endl;
      cout<<BifFit_func_NoBg[iPlane-1]->GetParName(11)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(11)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(11)<<endl;
      cout<<"Integral between "<<Rfit_keV[0]<<" and "<<Rfit_keV[1]<<" keV: "<<hHitSeedChargeBifFitNoBg[iPlane-1]->Integral(BifFit_func_NoBg[iPlane-1]->GetParameter(0)*Rfit_keV[0]/calibrationEnergy, BifFit_func_NoBg[iPlane-1]->GetParameter(0)*Rfit_keV[1]/calibrationEnergy)<<endl;

      fitResultsFile<<"\n================ BIFURCATED FIT WITHOUT BACKGROUND ===================="<<endl;
      fitResultsFile<<BifFit_func_NoBg[iPlane-1]->GetParName(0)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(0)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(0)<<"\n";
      fitResultsFile<<BifFit_func_NoBg[iPlane-1]->GetParName(1)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(1)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(1)<<"\n";
      fitResultsFile<<BifFit_func_NoBg[iPlane-1]->GetParName(2)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(2)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(2)<<"\n";
      fitResultsFile<<BifFit_func_NoBg[iPlane-1]->GetParName(3)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(3)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(3)<<"\n";
      fitResultsFile<<BifFit_func_NoBg[iPlane-1]->GetParName(4)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(4)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(4)<<"\n";
      fitResultsFile<<BifFit_func_NoBg[iPlane-1]->GetParName(5)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(5)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(5)<<"\n";
      fitResultsFile<<BifFit_func_NoBg[iPlane-1]->GetParName(6)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(6)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(6)<<"\n";
      fitResultsFile<<"Energy Resolution at "<<calibrationEnergy<<" keV: "<<0.5*(BifFit_func_NoBg[iPlane-1]->GetParameter(2)+BifFit_func_NoBg[iPlane-1]->GetParameter(4))*2.355*calibrationEnergy*1000./BifFit_func_NoBg[iPlane-1]->GetParameter(0) <<" eV"<<"\n";
      fitResultsFile<<"Energy Resolution at "<<secondPeak<<" keV: "<<0.5*(BifFit_func_NoBg[iPlane-1]->GetParameter(3)+BifFit_func_NoBg[iPlane-1]->GetParameter(5))*2.355*secondPeak*1000./BifFit_func_NoBg[iPlane-1]->GetParameter(1) <<" eV"<<"\n";
      fitResultsFile<<BifFit_func_NoBg[iPlane-1]->GetParName(7)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(7)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(7)<<"\n";
      fitResultsFile<<BifFit_func_NoBg[iPlane-1]->GetParName(8)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(8)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(8)<<"\n";
      fitResultsFile<<BifFit_func_NoBg[iPlane-1]->GetParName(9)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(9)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(9)<<"\n";
      fitResultsFile<<BifFit_func_NoBg[iPlane-1]->GetParName(10)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(10)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(10)<<"\n";
      fitResultsFile<<BifFit_func_NoBg[iPlane-1]->GetParName(11)<<": "<<BifFit_func_NoBg[iPlane-1]->GetParameter(11)<<" +/- "<<BifFit_func_NoBg[iPlane-1]->GetParError(11)<<"\n";
      fitResultsFile<<"Integral between "<<Rfit_keV[0]<<" and "<<Rfit_keV[1]<<" keV: "<<hHitSeedChargeBifFitNoBg[iPlane-1]->Integral(BifFit_func_NoBg[iPlane-1]->GetParameter(0)*Rfit_keV[0]/calibrationEnergy, BifFit_func_NoBg[iPlane-1]->GetParameter(0)*Rfit_keV[1]/calibrationEnergy)<<"\n";
      /*==========================================================================================================================
       ============================================================================================================================
       ==================================================== STANDARD FIT OF 55Fe ==================================================
       ============================================================================================================================
       ==========================================================================================================================*/


      Fit_func[iPlane-1] = new TF1("fitCB", &fTool, &DGlobalTools::fpeaksCB55Fe, Rfit_ADC[0], Rfit_ADC[1], 10, "DGlobalTools", "fpeaksCB55Fe");
      //Fit_func[iPlane-1] = new TF1("fitCB", fTool.fpeaksBifurcatedCB, Rfit_ADC[0], Rfit_ADC[1], 7);

      if (fitXray==1)
      {
        Fit_func[iPlane-1]->SetParName(0,"#mu K#alpha (ADCu)");
        Fit_func[iPlane-1]->SetParName(1,"#mu K#beta (ADCu)");
        Fit_func[iPlane-1]->SetParName(2,"#sigma_{K#alpha} (ADCu)");
        Fit_func[iPlane-1]->SetParName(3,"#sigma_{K#beta} (ADCu)");
        Fit_func[iPlane-1]->SetParName(4,"#alpha K#alpha");
        Fit_func[iPlane-1]->SetParName(5,"#alpha K#beta");
        Fit_func[iPlane-1]->SetParName(6,"n K#alpha");
        Fit_func[iPlane-1]->SetParName(7,"n K#beta");
        Fit_func[iPlane-1]->SetParName(8,"N_{K#alpha}");
        Fit_func[iPlane-1]->SetParName(9,"N_{K#beta}");
      }
      else
      {
        Fit_func[iPlane-1]->SetParName(0,"#mu K#alpha (ADCu)");
        Fit_func[iPlane-1]->SetParName(1,"#mu 2xK#alpha (ADCu)");
        Fit_func[iPlane-1]->SetParName(2,"#sigma_{K#alpha} (ADCu)");
        Fit_func[iPlane-1]->SetParName(3,"#sigma_{2xK#alpha} (ADCu)");
        Fit_func[iPlane-1]->SetParName(4,"#alpha K#alpha");
        Fit_func[iPlane-1]->SetParName(5,"#alpha 2xK#alpha");
        Fit_func[iPlane-1]->SetParName(6,"n K#alpha");
        Fit_func[iPlane-1]->SetParName(7,"n 2xK#alpha");
        Fit_func[iPlane-1]->SetParName(8,"N_{K#alpha}");
        Fit_func[iPlane-1]->SetParName(9,"N_{2xK#alpha}");
      }




      Fit_func[iPlane-1]->SetParameter(0,MyMean);
      Fit_func[iPlane-1]->SetParameter(1,MyMean*(secondPeak/calibrationEnergy));
      Fit_func[iPlane-1]->SetParameter(2,MySigma);
      Fit_func[iPlane-1]->SetParameter(3,MySigma);
      Fit_func[iPlane-1]->SetParameter(4,MyAlpha);
      Fit_func[iPlane-1]->SetParameter(5,MyAlpha);
      Fit_func[iPlane-1]->SetParameter(6,Myn);
      Fit_func[iPlane-1]->SetParameter(7,Myn);
      Fit_func[iPlane-1]->SetParameter(8,MyN1);
      if (fitXray==4) //Forcing parameters for Mo where there is no second peak
      {
        Fit_func[iPlane-1]->FixParameter(9,0.0);
      }
      else
      {
        Fit_func[iPlane-1]->SetParameter(9,MyN2);
      }

      Fit_func[iPlane-1]->SetParLimits(0,RMyMean[0],RMyMean[1]);
      Fit_func[iPlane-1]->SetParLimits(1,RMyMean[0],RMyMean[1]);
      Fit_func[iPlane-1]->SetParLimits(2,RMySigma[0],RMySigma[1]);
      Fit_func[iPlane-1]->SetParLimits(3,RMySigma[0],RMySigma[1]);
      Fit_func[iPlane-1]->SetParLimits(4,RMyAlpha[0],RMyAlpha[1]);
      Fit_func[iPlane-1]->SetParLimits(5,RMyAlpha[0],RMyAlpha[1]);
      Fit_func[iPlane-1]->SetParLimits(6,RMyn[0],RMyn[1]);
      Fit_func[iPlane-1]->SetParLimits(7,RMyn[0],RMyn[1]);
      Fit_func[iPlane-1]->SetParLimits(8,RMyN1[0],RMyN1[1]);
      Fit_func[iPlane-1]->SetParLimits(9,RMyN2[0],RMyN2[1]);

      Fit_func[iPlane-1]->SetLineColor(kBlue);
      Fit_func[iPlane-1]->SetLineWidth(2);
      Fit_func[iPlane-1]->SetNpx(10000);

      hHitSeedChargeFit[iPlane-1]->Fit(Fit_func[iPlane-1],"RQ");



      cout<<"\n================ WITH BACKGROUND ===================="<<endl;
      cout<<Fit_func[iPlane-1]->GetParName(0)<<": "<<Fit_func[iPlane-1]->GetParameter(0)<<" +/- "<<Fit_func[iPlane-1]->GetParError(0)<<endl;
      cout<<Fit_func[iPlane-1]->GetParName(1)<<": "<<Fit_func[iPlane-1]->GetParameter(1)<<" +/- "<<Fit_func[iPlane-1]->GetParError(1)<<endl;
      cout<<Fit_func[iPlane-1]->GetParName(2)<<": "<<Fit_func[iPlane-1]->GetParameter(2)<<" +/- "<<Fit_func[iPlane-1]->GetParError(2)<<endl;
      cout<<Fit_func[iPlane-1]->GetParName(3)<<": "<<Fit_func[iPlane-1]->GetParameter(3)<<" +/- "<<Fit_func[iPlane-1]->GetParError(3)<<endl;
      cout<<"Energy Resolution at "<<calibrationEnergy<<" keV: "<<Fit_func[iPlane-1]->GetParameter(2)*2.355*calibrationEnergy*1000./Fit_func[iPlane-1]->GetParameter(0) <<" eV"<<endl;
      cout<<"Energy Resolution at "<<secondPeak<<" keV: "<<Fit_func[iPlane-1]->GetParameter(3)*2.355*secondPeak*1000./Fit_func[iPlane-1]->GetParameter(1) <<" eV"<<endl;
      cout<<Fit_func[iPlane-1]->GetParName(4)<<": "<<Fit_func[iPlane-1]->GetParameter(4)<<" +/- "<<Fit_func[iPlane-1]->GetParError(4)<<endl;
      cout<<Fit_func[iPlane-1]->GetParName(5)<<": "<<Fit_func[iPlane-1]->GetParameter(5)<<" +/- "<<Fit_func[iPlane-1]->GetParError(5)<<endl;
      cout<<Fit_func[iPlane-1]->GetParName(6)<<": "<<Fit_func[iPlane-1]->GetParameter(6)<<" +/- "<<Fit_func[iPlane-1]->GetParError(6)<<endl;
      cout<<Fit_func[iPlane-1]->GetParName(7)<<": "<<Fit_func[iPlane-1]->GetParameter(7)<<" +/- "<<Fit_func[iPlane-1]->GetParError(7)<<endl;
      cout<<Fit_func[iPlane-1]->GetParName(8)<<": "<<Fit_func[iPlane-1]->GetParameter(8)<<" +/- "<<Fit_func[iPlane-1]->GetParError(8)<<endl;
      cout<<Fit_func[iPlane-1]->GetParName(9)<<": "<<Fit_func[iPlane-1]->GetParameter(9)<<" +/- "<<Fit_func[iPlane-1]->GetParError(9)<<endl;
      cout<<"Integral between "<<Rfit_keV[0]<<" and "<<Rfit_keV[1]<<" keV: "<<hHitSeedChargeFit[iPlane-1]->Integral(Fit_func[iPlane-1]->GetParameter(0)*Rfit_keV[0]/calibrationEnergy, Fit_func[iPlane-1]->GetParameter(0)*Rfit_keV[1]/calibrationEnergy)<<endl;

      fitResultsFile<<"\n================ WITH BACKGROUND ===================="<<endl;
      fitResultsFile<<Fit_func[iPlane-1]->GetParName(0)<<": "<<Fit_func[iPlane-1]->GetParameter(0)<<" +/- "<<Fit_func[iPlane-1]->GetParError(0)<<"\n";
      fitResultsFile<<Fit_func[iPlane-1]->GetParName(1)<<": "<<Fit_func[iPlane-1]->GetParameter(1)<<" +/- "<<Fit_func[iPlane-1]->GetParError(1)<<"\n";
      fitResultsFile<<Fit_func[iPlane-1]->GetParName(2)<<": "<<Fit_func[iPlane-1]->GetParameter(2)<<" +/- "<<Fit_func[iPlane-1]->GetParError(2)<<"\n";
      fitResultsFile<<Fit_func[iPlane-1]->GetParName(3)<<": "<<Fit_func[iPlane-1]->GetParameter(3)<<" +/- "<<Fit_func[iPlane-1]->GetParError(3)<<"\n";
      fitResultsFile<<"Energy Resolution at "<<calibrationEnergy<<" keV: "<<Fit_func[iPlane-1]->GetParameter(2)*2.355*calibrationEnergy*1000./Fit_func[iPlane-1]->GetParameter(0) <<" eV"<<"\n";
      fitResultsFile<<"Energy Resolution at "<<secondPeak<<" keV: "<<Fit_func[iPlane-1]->GetParameter(3)*2.355*secondPeak*1000./Fit_func[iPlane-1]->GetParameter(1) <<" eV"<<"\n";
      fitResultsFile<<Fit_func[iPlane-1]->GetParName(4)<<": "<<Fit_func[iPlane-1]->GetParameter(4)<<" +/- "<<Fit_func[iPlane-1]->GetParError(4)<<"\n";
      fitResultsFile<<Fit_func[iPlane-1]->GetParName(5)<<": "<<Fit_func[iPlane-1]->GetParameter(5)<<" +/- "<<Fit_func[iPlane-1]->GetParError(5)<<"\n";
      fitResultsFile<<Fit_func[iPlane-1]->GetParName(6)<<": "<<Fit_func[iPlane-1]->GetParameter(6)<<" +/- "<<Fit_func[iPlane-1]->GetParError(6)<<"\n";
      fitResultsFile<<Fit_func[iPlane-1]->GetParName(7)<<": "<<Fit_func[iPlane-1]->GetParameter(7)<<" +/- "<<Fit_func[iPlane-1]->GetParError(7)<<"\n";
      fitResultsFile<<Fit_func[iPlane-1]->GetParName(8)<<": "<<Fit_func[iPlane-1]->GetParameter(8)<<" +/- "<<Fit_func[iPlane-1]->GetParError(8)<<"\n";
      fitResultsFile<<Fit_func[iPlane-1]->GetParName(9)<<": "<<Fit_func[iPlane-1]->GetParameter(9)<<" +/- "<<Fit_func[iPlane-1]->GetParError(9)<<"\n";
      fitResultsFile<<"Integral between "<<Rfit_keV[0]<<" and "<<Rfit_keV[1]<<" keV: "<<hHitSeedChargeFit[iPlane-1]->Integral(Fit_func[iPlane-1]->GetParameter(0)*Rfit_keV[0]/calibrationEnergy, Fit_func[iPlane-1]->GetParameter(0)*Rfit_keV[1]/calibrationEnergy)<<"\n";

      /*===================================================================================
       ================================ FIT WITHOUT BACKGROUND =============================
       =====================================================================================*/
      Fit_func_NoBg[iPlane-1] = new TF1("fitCB_nobg", &fTool, &DGlobalTools::fpeaksCB55Fe, Rfit_ADC[0], Rfit_ADC[1], 10, "DGlobalTools", "fpeaksCB55Fe");
      //Fit_func_NoBg[iPlane-1] = new TF1("fitCB", fTool.fpeaksBifurcatedCB, Rfit_ADC[0], Rfit_ADC[1], 7);
      if (fitXray==1)
      {
        Fit_func_NoBg[iPlane-1]->SetParName(0,"#mu K#alpha (ADCu)");
        Fit_func_NoBg[iPlane-1]->SetParName(1,"#mu K#beta (ADCu)");
        Fit_func_NoBg[iPlane-1]->SetParName(2,"#sigma_{K#alpha} (ADCu)");
        Fit_func_NoBg[iPlane-1]->SetParName(3,"#sigma_{K#beta} (ADCu)");
        Fit_func_NoBg[iPlane-1]->SetParName(4,"#alpha K#alpha");
        Fit_func_NoBg[iPlane-1]->SetParName(5,"#alpha K#beta");
        Fit_func_NoBg[iPlane-1]->SetParName(6,"n K#alpha");
        Fit_func_NoBg[iPlane-1]->SetParName(7,"n K#beta");
        Fit_func_NoBg[iPlane-1]->SetParName(8,"N_{K#alpha}");
        Fit_func_NoBg[iPlane-1]->SetParName(9,"N_{K#beta}");
      }
      else
      {
        Fit_func_NoBg[iPlane-1]->SetParName(0,"#mu K#alpha (ADCu)");
        Fit_func_NoBg[iPlane-1]->SetParName(1,"#mu 2xK#alpha (ADCu)");
        Fit_func_NoBg[iPlane-1]->SetParName(2,"#sigma_{K#alpha} (ADCu)");
        Fit_func_NoBg[iPlane-1]->SetParName(3,"#sigma_{2xK#alpha} (ADCu)");
        Fit_func_NoBg[iPlane-1]->SetParName(4,"#alpha K#alpha");
        Fit_func_NoBg[iPlane-1]->SetParName(5,"#alpha 2xK#alpha");
        Fit_func_NoBg[iPlane-1]->SetParName(6,"n K#alpha");
        Fit_func_NoBg[iPlane-1]->SetParName(7,"n 2xK#alpha");
        Fit_func_NoBg[iPlane-1]->SetParName(8,"N_{K#alpha}");
        Fit_func_NoBg[iPlane-1]->SetParName(9,"N_{2xK#alpha}");
      }


      Fit_func_NoBg[iPlane-1]->SetParameter(0,MyMean);
      Fit_func_NoBg[iPlane-1]->SetParameter(1,MyMean*(secondPeak/calibrationEnergy));
      Fit_func_NoBg[iPlane-1]->SetParameter(2,MySigma);
      Fit_func_NoBg[iPlane-1]->SetParameter(3,MySigma);
      Fit_func_NoBg[iPlane-1]->SetParameter(4,MyAlpha);
      Fit_func_NoBg[iPlane-1]->SetParameter(5,MyAlpha);
      Fit_func_NoBg[iPlane-1]->SetParameter(6,Myn);
      Fit_func_NoBg[iPlane-1]->SetParameter(7,Myn);
      Fit_func_NoBg[iPlane-1]->SetParameter(8,MyN1);
      if (fitXray==4) //Forcing parameters for Mo where there is no second peak
      {
        Fit_func_NoBg[iPlane-1]->FixParameter(9,0.0);
      }
      else
      {
        Fit_func_NoBg[iPlane-1]->SetParameter(9,MyN2);
      }

      Fit_func_NoBg[iPlane-1]->SetParLimits(0,RMyMean[0],RMyMean[1]);
      Fit_func_NoBg[iPlane-1]->SetParLimits(1,RMyMean[0],RMyMean[1]);
      Fit_func_NoBg[iPlane-1]->SetParLimits(2,RMySigma[0],RMySigma[1]);
      Fit_func_NoBg[iPlane-1]->SetParLimits(3,RMySigma[0],RMySigma[1]);
      Fit_func_NoBg[iPlane-1]->SetParLimits(4,RMyAlpha[0],RMyAlpha[1]);
      Fit_func_NoBg[iPlane-1]->SetParLimits(5,RMyAlpha[0],RMyAlpha[1]);
      Fit_func_NoBg[iPlane-1]->SetParLimits(6,RMyn[0],RMyn[1]);
      Fit_func_NoBg[iPlane-1]->SetParLimits(7,RMyn[0],RMyn[1]);
      Fit_func_NoBg[iPlane-1]->SetParLimits(8,RMyN1[0],RMyN1[1]);
      Fit_func_NoBg[iPlane-1]->SetParLimits(9,RMyN2[0],RMyN2[1]);

      Fit_func_NoBg[iPlane-1]->SetLineColor(kBlue);
      Fit_func_NoBg[iPlane-1]->SetLineWidth(2);
      Fit_func_NoBg[iPlane-1]->SetNpx(10000);

      hHitSeedChargeFitNoBg[iPlane-1]->Fit(Fit_func_NoBg[iPlane-1],"RQ");




      cout<<"\n================ WITHOUT BACKGROUND ===================="<<endl;
      cout<<Fit_func_NoBg[iPlane-1]->GetParName(0)<<": "<<Fit_func_NoBg[iPlane-1]->GetParameter(0)<<" +/- "<<Fit_func_NoBg[iPlane-1]->GetParError(0)<<endl;
      cout<<Fit_func_NoBg[iPlane-1]->GetParName(1)<<": "<<Fit_func_NoBg[iPlane-1]->GetParameter(1)<<" +/- "<<Fit_func_NoBg[iPlane-1]->GetParError(1)<<endl;
      cout<<Fit_func_NoBg[iPlane-1]->GetParName(2)<<": "<<Fit_func_NoBg[iPlane-1]->GetParameter(2)<<" +/- "<<Fit_func_NoBg[iPlane-1]->GetParError(2)<<endl;
      cout<<Fit_func_NoBg[iPlane-1]->GetParName(3)<<": "<<Fit_func_NoBg[iPlane-1]->GetParameter(3)<<" +/- "<<Fit_func_NoBg[iPlane-1]->GetParError(3)<<endl;
      cout<<"Energy Resolution at "<<calibrationEnergy<<" keV: "<<Fit_func_NoBg[iPlane-1]->GetParameter(2)*2.355*calibrationEnergy*1000./Fit_func_NoBg[iPlane-1]->GetParameter(0) <<" eV"<<endl;
      cout<<"Energy Resolution at "<<secondPeak<<" keV: "<<Fit_func_NoBg[iPlane-1]->GetParameter(3)*2.355*secondPeak*1000./Fit_func_NoBg[iPlane-1]->GetParameter(1) <<" eV"<<endl;
      cout<<Fit_func_NoBg[iPlane-1]->GetParName(4)<<": "<<Fit_func_NoBg[iPlane-1]->GetParameter(4)<<" +/- "<<Fit_func_NoBg[iPlane-1]->GetParError(4)<<endl;
      cout<<Fit_func_NoBg[iPlane-1]->GetParName(5)<<": "<<Fit_func_NoBg[iPlane-1]->GetParameter(5)<<" +/- "<<Fit_func_NoBg[iPlane-1]->GetParError(5)<<endl;
      cout<<Fit_func_NoBg[iPlane-1]->GetParName(6)<<": "<<Fit_func_NoBg[iPlane-1]->GetParameter(6)<<" +/- "<<Fit_func_NoBg[iPlane-1]->GetParError(6)<<endl;
      cout<<Fit_func_NoBg[iPlane-1]->GetParName(7)<<": "<<Fit_func_NoBg[iPlane-1]->GetParameter(7)<<" +/- "<<Fit_func_NoBg[iPlane-1]->GetParError(7)<<endl;
      cout<<Fit_func_NoBg[iPlane-1]->GetParName(8)<<": "<<Fit_func_NoBg[iPlane-1]->GetParameter(8)<<" +/- "<<Fit_func_NoBg[iPlane-1]->GetParError(8)<<endl;
      cout<<Fit_func_NoBg[iPlane-1]->GetParName(9)<<": "<<Fit_func_NoBg[iPlane-1]->GetParameter(9)<<" +/- "<<Fit_func_NoBg[iPlane-1]->GetParError(9)<<endl;
      cout<<"Integral between "<<Rfit_keV[0]<<" and "<<Rfit_keV[1]<<" keV: "<<hHitSeedChargeFitNoBg[iPlane-1]->Integral(BifFit_func[iPlane-1]->GetParameter(0)*Rfit_keV[0]/calibrationEnergy, BifFit_func[iPlane-1]->GetParameter(0)*Rfit_keV[1]/calibrationEnergy)<<endl;

      fitResultsFile<<"\n================ WITHOUT BACKGROUND ===================="<<endl;
      fitResultsFile<<Fit_func_NoBg[iPlane-1]->GetParName(0)<<": "<<Fit_func_NoBg[iPlane-1]->GetParameter(0)<<" +/- "<<Fit_func_NoBg[iPlane-1]->GetParError(0)<<"\n";
      fitResultsFile<<Fit_func_NoBg[iPlane-1]->GetParName(1)<<": "<<Fit_func_NoBg[iPlane-1]->GetParameter(1)<<" +/- "<<Fit_func_NoBg[iPlane-1]->GetParError(1)<<"\n";
      fitResultsFile<<Fit_func_NoBg[iPlane-1]->GetParName(2)<<": "<<Fit_func_NoBg[iPlane-1]->GetParameter(2)<<" +/- "<<Fit_func_NoBg[iPlane-1]->GetParError(2)<<"\n";
      fitResultsFile<<Fit_func_NoBg[iPlane-1]->GetParName(3)<<": "<<Fit_func_NoBg[iPlane-1]->GetParameter(3)<<" +/- "<<Fit_func_NoBg[iPlane-1]->GetParError(3)<<"\n";
      fitResultsFile<<"Energy Resolution at "<<calibrationEnergy<<" keV: "<<Fit_func_NoBg[iPlane-1]->GetParameter(2)*2.355*calibrationEnergy*1000./Fit_func_NoBg[iPlane-1]->GetParameter(0) <<" eV"<<"\n";
      fitResultsFile<<"Energy Resolution at "<<secondPeak<<" keV: "<<Fit_func_NoBg[iPlane-1]->GetParameter(3)*2.355*secondPeak*1000./Fit_func_NoBg[iPlane-1]->GetParameter(1) <<" eV"<<"\n";
      fitResultsFile<<Fit_func_NoBg[iPlane-1]->GetParName(4)<<": "<<Fit_func_NoBg[iPlane-1]->GetParameter(4)<<" +/- "<<Fit_func_NoBg[iPlane-1]->GetParError(4)<<"\n";
      fitResultsFile<<Fit_func_NoBg[iPlane-1]->GetParName(5)<<": "<<Fit_func_NoBg[iPlane-1]->GetParameter(5)<<" +/- "<<Fit_func_NoBg[iPlane-1]->GetParError(5)<<"\n";
      fitResultsFile<<Fit_func_NoBg[iPlane-1]->GetParName(6)<<": "<<Fit_func_NoBg[iPlane-1]->GetParameter(6)<<" +/- "<<Fit_func_NoBg[iPlane-1]->GetParError(6)<<"\n";
      fitResultsFile<<Fit_func_NoBg[iPlane-1]->GetParName(7)<<": "<<Fit_func_NoBg[iPlane-1]->GetParameter(7)<<" +/- "<<Fit_func_NoBg[iPlane-1]->GetParError(7)<<"\n";
      fitResultsFile<<Fit_func_NoBg[iPlane-1]->GetParName(8)<<": "<<Fit_func_NoBg[iPlane-1]->GetParameter(8)<<" +/- "<<Fit_func_NoBg[iPlane-1]->GetParError(8)<<"\n";
      fitResultsFile<<Fit_func_NoBg[iPlane-1]->GetParName(9)<<": "<<Fit_func_NoBg[iPlane-1]->GetParameter(9)<<" +/- "<<Fit_func_NoBg[iPlane-1]->GetParError(9)<<"\n";
      fitResultsFile<<"Integral between "<<Rfit_keV[0]<<" and "<<Rfit_keV[1]<<" keV: "<<hHitSeedChargeFitNoBg[iPlane-1]->Integral(Fit_func_NoBg[iPlane-1]->GetParameter(0)*Rfit_keV[0]/calibrationEnergy, Fit_func_NoBg[iPlane-1]->GetParameter(0)*Rfit_keV[1]/calibrationEnergy)<<"\n";



      fitResultsFile.close();













    }

  }











  // Save canvas and histos
  //cd to result dir
  // added JB 2011/03/14


  Char_t rootFile[300];
  sprintf(rootFile,"%sHitMap_run%d.root",fSession->GetResultDirName().Data(),fSession->GetRunNumber());
  sprintf(rootFile,"%s", fTool.LocalizeDirName( rootFile)); // JB 2011/07/07
  cout << "\n-- Saving histos and canvas into " << rootFile << endl;
  TFile fRoot(rootFile,"RECREATE");
  ccumulhit->Write();
  ccumulhit2->Write();
  ccumulhit22->Write(); // QL 2015/10/23
  ccumulhit3->Write();
  for(int i=0;i<RealNcanvas;i++) ccumulhit3_2[i]->Write();

  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    tPlane = tTracker->GetPlane(iPlane);
    if( 0 < tPlane->GetAnalysisMode()  ) {
      hHitMap[iPlane-1]->Write();
      hTrackMap[iPlane-1]->Write(); // JB 2011/11/02
      hHitPixMult[iPlane-1]->Write();
      hHitTimeStamp[iPlane-1]->Write();
      hNHitsVSEventN[iPlane-1]->Write(); // QL 2015/10/23
      hNHitsPerEvent[iPlane-1]->Write();
      if( tPlane->GetAnalysisMode() <= 2 ) {
        hHitSeedSN[iPlane-1]->Write();
        hHitNeighbourSN[iPlane-1]->Write();
        hHitSeedVsNeighbourSN[iPlane-1]->Write();
        hHitCharge[iPlane-1]->Write();
        hHitSeedCharge[iPlane-1]->Write();
        if (fitXray!=0)
        {
          hb[iPlane-1]->Write();
          Fit_func[iPlane-1]->Write();
          Fit_func_NoBg[iPlane-1]->Write();
          hHitSeedChargeFit[iPlane-1]->Write();
          hHitSeedChargeFitNoBg[iPlane-1]->Write();;
          BifFit_func[iPlane-1]->Write();
          BifFit_func_NoBg[iPlane-1]->Write();
          hHitSeedChargeBifFit[iPlane-1]->Write();
          hHitSeedChargeBifFitNoBg[iPlane-1]->Write();
        }
        hHitSeedCharge_CUT[iPlane-1]->Write();
        hHitNeighbourCharge[iPlane-1]->Write();
        hHitSeedVsNeighbourCharge[iPlane-1]->Write();
        hHitSeedVsNeighbourCharge_cut[iPlane-1]->Write();
        hHitSeedVsNeighbourCharge_cutseed[iPlane-1]->Write();
        hHitSeedSNVsSeedCharge[iPlane-1]->Write();
        cHitProperties[iPlane-1]->Write();
        cHitProperties2[iPlane-1]->Write();
        hHitChargeRatio[iPlane-1]->Write();
        cHitSeedChargeSum[iPlane-1]->Write();
        cHitChargeRatio[iPlane-1]->Write();
      }
    }
  }
  for(int ipair = 0; ipair<(nPlanes*(nPlanes - 1)/2); ipair++) hNHitsPerEventCorr[ipair]->Write();

  fRoot.Close();

}
//______________________________________________________________________________
//
void MRaw::SeedCuts(Int_t nEvents)
{
  // Analysis specific for X-ray data
  // Call by gTAF->GetRaw()->XrayAnalysis()
  // JH, 2016/07/20 originally copied from DisplayCumulatedHits2D




  fSession->SetEvents(nEvents);

  Int_t nHitsReconstructed = 0;



  DTracker *tTracker  =  fSession->GetTracker();
  DPlane* tPlane = NULL;
  DTrack *aTrack = NULL;
  DHit *aHit = NULL;
  //DPixel *aPixel;
  Double_t seedSN;
  //Double_t seedN;

  Int_t nPlanes = tTracker->GetPlanesN();
  Int_t nCutSteps= 21;
  Int_t nCutseedSteps= 10;


  // Get the first DUT among the planes and remember it,
  //  it considered as the one providing the coordinate frame for the geomatrix
  // If no DUT (status=3), set to first plane
  // JB 2012/0820
  /*  DPlane* DUTplane = tTracker->GetPlane(1);
   for( Int_t iPlane=2; iPlane<=nPlanes; iPlane++) { // loop on planes
   tPlane = tTracker->GetPlane(iPlane);
   if( tPlane->GetStatus()==3 ) {
   DUTplane = tPlane;
   break;
   }
   }  */


  TH1F **hHitSeedCharge_cut  = new TH1F*[nPlanes*100*100];
  TH2F **hHitSeedVsNeighbourCharge_cut = new TH2F*[nPlanes*100*100];
  TH2F **hHitMap_cut = new TH2F*[nPlanes*100*100];





  Char_t name[100], title[200];

  //         sprintf( name, "hhitmap");
  //            sprintf( title, "bidon");
  //  hHitMap_cut[0] = new TH2F("bidon", "bidon", NbinsX, xmin, xmax, NbinsY, ymin, ymax);
  // TH2F *hHitMap_cut0 = new TH2F("bidon", "bidon", NbinsX,-50,50,NbinsY,-50,50);
  // TH1F *h = new TH1F("h", "bidon", 100, -50, 50);


  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    tPlane = tTracker->GetPlane(iPlane);
    int NbinsX = tPlane->GetStripsNu();
    int NbinsY = tPlane->GetStripsNv();
    Double_t xmin=-NbinsX*tPlane->GetStripPitch()(0) / 2., xmax=NbinsX*tPlane->GetStripPitch()(0) / 2.;
    Double_t ymin=-NbinsY*tPlane->GetStripPitch()(1) / 2., ymax=NbinsY*tPlane->GetStripPitch()(1) / 2.;





    for (Int_t cut=0; cut<=20; cut++)    {
      for (Int_t cutseed=0; cutseed<=10; cutseed ++) {
        cout<<"Cutseed = "<< float(cutseed)/10 <<", cut = "<<cut*10<<endl;
        sprintf( name, "hhitseedq_cut%d_cutseed%d_pl%d", cut*10, cutseed*10, iPlane);
        //cout<<name<<endl;
        sprintf( title, "Spectrum for seed + cluster > %d, seed/neigh > %2f of plane %d - %s; seed charge (ADCu); counts", cut*10, float(cutseed)/10, iPlane, tPlane->GetPlanePurpose());
        hHitSeedCharge_cut[(iPlane-1)+(nCutSteps*cut)+(nCutseedSteps*cutseed)] = new TH1F(name, title, 3000, 0, 3000);

        sprintf( name, "hhitseedneighbourgq_cut%d_cutseed%d_pl%d", cut*10, cutseed*10, iPlane);
        //cout<<name<<endl;
        sprintf( title, "Neighbour pixels VS seed charge of plane %d - %s seed + cluster > %d, seed/neig > %2f", iPlane, tPlane->GetPlanePurpose(), cut*10, float(cutseed)/10);
        hHitSeedVsNeighbourCharge_cut[(iPlane-1)+(nCutSteps*cut)+(nCutseedSteps*cutseed)] = new TH2F(name, title, 100, 0, 0, 50, 0, 0);
        hHitSeedVsNeighbourCharge_cut[(iPlane-1)+(nCutSteps*cut)+(nCutseedSteps*cutseed)] ->SetXTitle("seed charge (ADCu)");
        hHitSeedVsNeighbourCharge_cut[(iPlane-1)+(nCutSteps*cut)+(nCutseedSteps*cutseed)] ->SetYTitle("neighbourgs charge (ADCu)");


        sprintf( name, "hhitmap_cut%d_cutseed%d_pl%d", cut*10, cutseed*10, iPlane);
        sprintf( title, "Hit map of plane %d - %s;X (#mum);Y (#mum)  seed + cluster > %d, seed/neig > %2f", iPlane, tPlane->GetPlanePurpose(),cut*10, float(cutseed)/10);

        if ((iPlane-1)+(nCutSteps*cut)+(nCutseedSteps*cutseed)< nPlanes*100*100)                 hHitMap_cut[(iPlane-1)+(nCutSteps*cut)+(nCutseedSteps*cutseed)] = new TH2F(name, title, NbinsX, xmin, xmax, NbinsY, ymin, ymax);
        else cout<<  (iPlane-1)+(nCutSteps*cut)+(nCutseedSteps*cutseed) <<" ------ "<<nPlanes*100*100 <<endl;
        hHitMap_cut[(iPlane-1)+(nCutSteps*cut)+(nCutseedSteps*cutseed)]->SetMarkerStyle(20);
        hHitMap_cut[(iPlane-1)+(nCutSteps*cut)+(nCutseedSteps*cutseed)]->SetMarkerSize(.1);
        hHitMap_cut[(iPlane-1)+(nCutSteps*cut)+(nCutseedSteps*cutseed)]->SetMarkerColor(1);
        hHitMap_cut[(iPlane-1)+(nCutSteps*cut)+(nCutseedSteps*cutseed)]->SetStats(kFALSE);

      }
    }
  }




  //Loop over the requested number of events
  for( Int_t iEvt=0; iEvt < nEvents; iEvt++) {
    if( !(fSession->NextRawEvent()) ) break; // JB 2009/06/26
    tTracker->Update();


    for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
      tPlane = tTracker->GetPlane(iPlane);

      if( tPlane->GetHitsN()>0 ) {
        nHitsReconstructed += tPlane->GetHitsN();

        for( Int_t iHit=1; iHit<=tPlane->GetHitsN(); iHit++) { //loop on hits (starts at 1 !!)
          aHit = (DHit*)tPlane->GetHit( iHit);




          if( tPlane->GetAnalysisMode()<3 ) { // analog output



            if ((aHit->GetClusterPulseSum()!=0) && (aHit->GetPulseHeight(0)>0))	{ // =====================ICI=========
              Float_t chargeRatioValue=aHit->GetPulseHeight(0)/aHit->GetClusterPulseSum();

              for (Int_t cut=0; cut<=20; cut++)    {
                for (Int_t cutseed=0; cutseed<=10; cutseed ++) {

                  if ((aHit->GetPulseHeight(0) + aHit->GetClusterAreaPulseSum()>=cut*10) && (chargeRatioValue>=Float_t(cutseed)/10))	{
                    hHitSeedCharge_cut[(iPlane-1)+(nCutSteps*cut)+(nCutseedSteps*cutseed)] ->Fill( aHit->GetPulseHeight(0));
                    hHitSeedVsNeighbourCharge_cut[(iPlane-1)+(nCutSteps*cut)+(nCutseedSteps*cutseed)] ->Fill( aHit->GetPulseHeight(0), aHit->GetClusterAreaPulseSum());
                    hHitMap_cut[(iPlane-1)+(nCutSteps*cut)+(nCutseedSteps*cutseed)]->Fill( tPlane->PlaneToTracker(*(aHit->GetPosition()))(0), tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), 1);
                  }
                }
              }
            }
          }
        }
      }  //end loop on hits
    } //end loop on planes
  } // END LOOP ON EVENTS



  fSession->GetDataAcquisition()->PrintStatistics();
  tTracker->PrintStatistics();



  // Save canvas and histos
  //cd to result dir
  // added JB 2011/03/14
  Char_t rootFile[300];
  sprintf(rootFile,"%sSeedCuts_run%d.root",fSession->GetResultDirName().Data(),fSession->GetRunNumber());
  sprintf(rootFile,"%s", fTool.LocalizeDirName( rootFile)); // JB 2011/07/07
  cout << "\n-- Saving histos and canvas into " << rootFile << endl;
  TFile fRoot(rootFile,"RECREATE");


  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    tPlane = tTracker->GetPlane(iPlane);
    if( 0 < tPlane->GetAnalysisMode()  ) {
      if( tPlane->GetAnalysisMode() <= 2 ) {

        for (Int_t cut=0; cut<=20; cut++)    {
          for (Int_t cutseed=0; cutseed<=10; cutseed ++) {
            hHitSeedVsNeighbourCharge_cut[(iPlane-1)+nCutSteps*cut+nCutseedSteps*cutseed] ->Write();//max 200 en 10 par 10
            hHitSeedCharge_cut[(iPlane-1)+nCutSteps*cut+nCutseedSteps*cutseed] ->Write();
            hHitMap_cut[(iPlane-1)+nCutSteps*cut+nCutseedSteps*cutseed]->Write();
          }
        }


      }
    }
  }


  fRoot.Close();

}


//______________________________________________________________________________
//
//*****************************************************************************
double CBfunction(double *x, double *par) {
  // Crystal ball function (https://en.wikipedia.org/wiki/Crystal_Ball_function)

  double mean     = par[0];
  double sigma    = par[1];
  double alpha    = par[2];
  double n        = par[3];
  double norm     = par[4];
  double absalpha = TMath::Abs(alpha);
  double frac     = (x[0]-mean)/sigma;
  double A        = pow(n/absalpha,n)*exp(-pow(absalpha,2)/2);
  double B        = (n/absalpha)-absalpha;

  double C        = sigma*(n/absalpha)*(1./(n-1))*exp(-0.5*pow(absalpha,2));
  double D        = sigma*sqrt(0.5*TMath::Pi())*(TMath::Erf(absalpha/sqrt(2)) + 1);
  double N        = norm/(C+D);

  double result = 0.0;
  if(frac > -alpha) result = exp(-0.5*pow(frac,2));
  else              result = A*pow(B-frac,-n);
  result *= N;

  return result;

}
//*****************************************************************************
void MRaw::BuildPixelGainMap( Int_t nEvents, Double_t minfit, Double_t maxfit, Double_t maxcharge )
{
  // Fit the seed charge distribution for all individual pixels of 1st plane
  //    => spectrum is expected to come from a monochromatic source!
  //    => cristal function is used for the fit
  //    => the peak positions and widths are stored
  // The correction factor is computed for each pixel as = peak position / average peak position
  // A 2D map of the correction factors is created and saved
  //
  // Inputs:
  //  o nevents = nb of events to analyse
  //  o minfit, maxfit = range for the fit of the spectrum
  //  o maxcharge = range (from 0) for the seed charge distribution (bin width = 1)
  //
  // Outputs:
  //  o PixelSpectrum_runXXXX.root with all plots
  //  o PixelGain_runXXXX.root with 2d map of correction factors
  //
  // JB 2018/07/04

  Int_t planeID = 1;

  DTracker *tTracker  =  fSession->GetTracker();
  DPlane   *tPlane = tTracker->GetPlane(planeID);
  DHit     *aHit;

  Int_t npixels  = tPlane->GetStripsN();
  Int_t ncolumns = tPlane->GetStripsNu();
  Int_t nrows = tPlane->GetStripsNv();

  // For seed charge distributions
  TH1F **hHitSeedCharge = new TH1F*[npixels];
  Char_t name[100], title[300];
  for( int ipix=0; ipix<npixels; ipix++ ) {
    sprintf( name, "hhitseedq%dpl%d", ipix, planeID);
    sprintf( title, "Seed pixel charge for pixel %d - plane %d", ipix, planeID);
    hHitSeedCharge[ipix] = new TH1F(name, title, maxcharge, 0, maxcharge);
    hHitSeedCharge[ipix]->SetXTitle("charge (ADCu)");
  }
  sprintf( name, "hhitseedqallpl%d", planeID);
  sprintf( title, "Seed pixel charge for all pixels - plane %d - %s", planeID, tPlane->GetPlanePurpose());
  TH1F *hHitSeedChargeAll = new TH1F( name, title, maxcharge, 0, maxcharge);

  // For fit & statistics
  double *means = new double[npixels];
  double *sigmas = new double[npixels];
  TF1 *ffit = new TF1("ffit",CBfunction, minfit, maxfit, 5);
  ffit->SetParameters( (maxfit+minfit)/2, (maxfit-minfit)/4, 1., 1., 1.);
  ffit->SetParNames( "mean", "#sigma", "#alpha", "n", "norm");
  sprintf( name, "hmeanspl%d", planeID);
  sprintf( title, "Distribution of calib peak positions - plane %d;peak position (ADCu)", planeID);
  TH1F *hmeans = new TH1F( name, title, 4*(maxfit*1.3-minfit*.7), minfit*.7, maxfit*1.3);
  sprintf( name, "hsigmaspl%d", planeID);
  sprintf( title, "Distribution of std deviations - plane %d;std deviation (ADCu)", planeID);
  TH1F *hsigmas = new TH1F( name, title, 200, 0, 100);
  sprintf( name, "hpixelgainpl%d", planeID);
  sprintf( title, "Map of individual pixel correction factors - plane %d;row index;column index", planeID);
  TH2F *hpixelgain = new TH2F( name, title, ncolumns, 1, ncolumns+1, nrows, 1, nrows+1);


//  cout << "BuildPixelGainMap:: init done" << endl;

  // ================
  // Loop over events
  fSession->SetEvents(nEvents);

  for( Int_t iEvt=0; iEvt < nEvents; iEvt++) { // loop on events
    fSession->NextRawEvent();
    tTracker->Update();
    //    cout << "BuildPixelGainMap:: event " << iEvt << endl;

    if( tPlane->GetHitsN()>0 ) { // If there are some hits
      for( Int_t iHit=1; iHit<=tPlane->GetHitsN(); iHit++) { //loop on hits (starts at 1 !!)
        aHit = (DHit*)tPlane->GetHit( iHit);
        //printf("GettaHit->GetIndexSeed()ing seed index for hit %d (address %x) at plane %d: ", iHit, aHit, iPlane);
        //printf( "%d\n", aHit->GetIndexSeed());

        hHitSeedChargeAll->Fill( aHit->GetPulseHeight(0));

        int ipix = (int)(aHit->GetIndexSeed());
        if( 0<=ipix && ipix<npixels ) {
          hHitSeedCharge[ipix]->Fill( aHit->GetPulseHeight(0));
        }

      } //end loop on hits
    } // end If there are some hits

  } // end loop on events
  fSession->GetDataAcquisition()->PrintStatistics();
  tTracker->PrintStatistics();


  // ================
  // Fit histos for all pixels and individual pixels

  ffit->SetRange( minfit, maxfit);
  ffit->SetParameters( (maxfit+minfit)/2, (maxfit-minfit)/4, 1., 1., 1.);
  ffit->SetParLimits( 0, minfit, maxfit);
  ffit->SetParLimits( 1, 0, maxfit-minfit);
  ffit->SetParLimits( 2, 0.5, 1.5);
  ffit->SetParLimits( 3, 0.1, 2.);
  ffit->SetParLimits( 4, 0., 1.e8);
  hHitSeedChargeAll->Fit( ffit, "QR");
  double averageMean = ffit->GetParameter(0);
  double averageSigma = ffit->GetParameter(1);
  printf( "   average estimate: mean = %.0f, std-dev = %.1f, alpha = %.3f, n = %.3f\n", averageMean, averageSigma, ffit->GetParameter(2), ffit->GetParameter(3));


  for ( int ipix=0; ipix<npixels; ipix++ ) {
    cout << "getting histo nb " << ipix;
    cout << " with " << hHitSeedCharge[ipix]->GetEntries() << " entries" << endl;
    ffit->SetRange( minfit, maxfit);
    ffit->SetParameters( (maxfit+minfit)/2, (maxfit-minfit)/4, 1., 1., 1.);
    ffit->SetParLimits( 0, minfit, maxfit);
    ffit->SetParLimits( 1, 0, maxfit-minfit);
    ffit->SetParLimits( 2, 0.5, 1.5);
    ffit->SetParLimits( 3, 0.1, 2.);
    ffit->SetParLimits( 4, 0., 1.e8);
    hHitSeedCharge[ipix]->Fit( ffit, "QR");
    means[ipix] = ffit->GetParameter(0);
    sigmas[ipix] = ffit->GetParameter(1);
    printf( "   first estimate: mean = %.0f, std-dev = %.1f, alpha = %.3f, n = %.3f\n", means[ipix], sigmas[ipix], ffit->GetParameter(2), ffit->GetParameter(3));
    hpixelgain->SetBinContent( ipix%ncolumns+1, ipix/ncolumns+1, means[ipix]);

    // Uncomment the following for a second fit
    /*    ffit->SetRange( means[ipix]-6*sigmas[ipix], means[ipix]+3*sigmas[ipix]);
     ffit->SetParLimits( 0, minfit, maxfit);
     ffit->SetParLimits( 1, 0, maxfit-minfit);
     ffit->SetParLimits( 2, 0, 2.);
     ffit->SetParLimits( 3, 0, 2.);
     ffit->SetParLimits( 4, 0., 1.e8);
     histo->Fit( ffit, "QR");
     means[ipix] = ffit->GetParameter(0);
     sigmas[ipix] = ffit->GetParameter(1);
     printf( "   second estimation mean = %.0f, std-dev = %.1f, alpha = %.3f, n = %.3f\n", means[ipix], sigmas[ipix], ffit->GetParameter(2), ffit->GetParameter(3));*/

    hmeans->Fill( means[ipix]);
    hsigmas->Fill( sigmas[ipix]);
  }

  hpixelgain->Scale(1./averageMean);


  // ================
  // Display

  TCanvas *c1 = new TCanvas("c1", "Distribution per pixels", 600, 800);
  c1->Divide(1,3);
  c1->cd(1);
  hHitSeedChargeAll->Draw();
  c1->cd(2);
  hmeans->Draw();
  hmeans->Fit("gaus");
  c1->cd(3);
  hsigmas->Draw();
  hsigmas->Fit("gaus");

  TCanvas *c2 = new TCanvas("c2", "Distribution of gain per pixels", 100, 100, 600, 600);
  hpixelgain->Draw("colz");

  TCanvas *c3= new TCanvas("c3", "Distributions for individual pixels", 200, 200, 600, 600);
  Int_t randpix=0;
  for ( int ipix=0; ipix<TMath::Min(npixels,50); ipix++ ) {
    if( npixels>50 ) {
      randpix = (Int_t)(gRandom->Uniform(0,npixels));
    } else {
      randpix = ipix;
    }
    hHitSeedCharge[randpix]->Draw( (ipix==0?"":"same") );
  }

  // ================
  // Save canvas and histos
  //cd to result dir
  // added JB 2011/03/14
  Char_t rootFile[300];
  sprintf(rootFile,"%sPixelSpectrum_run%d.root",fSession->GetResultDirName().Data(),fSession->GetRunNumber());
  sprintf(rootFile,"%s", fTool.LocalizeDirName( rootFile));
  cout << "\n-- Saving histos and canvas into " << rootFile << endl;
  TFile fRoot(rootFile,"RECREATE");
  c1->Write();
  c2->Write();
  c3->Write();
  for( int ipix=0; ipix<npixels; ipix++ ) {
    hHitSeedCharge[ipix]->Write();
  }
  hHitSeedChargeAll->Write();
  hmeans->Write();
  hsigmas->Write();
  hpixelgain->Write();
  fRoot.Close();

  sprintf(rootFile,"%sPixelGain_run%d.root",fSession->GetResultDirName().Data(),fSession->GetRunNumber());
  sprintf(rootFile,"%s", fTool.LocalizeDirName( rootFile));
  cout << "\n-- Saving gain map into " << rootFile << endl;
  TFile fRoot2(rootFile,"UPDATE"); // note the file is updated, old maps are still there!
  hmeans->Write();
  hpixelgain->Write();
  fRoot2.Close();

}


//______________________________________________________________________________
//
void MRaw::BetaSourceMultiFrameAnalysis(int     aPlane,
					int     nFrames,
					Int_t   nEvents,
					Bool_t  ProduceTree,
					int     Npixel_TimeSpaceCut,
					double  SAveN_cutFit,
					int Fiducial_col_min,
					int Fiducial_col_max,
					int Fiducial_row_min,
					int Fiducial_row_max)
{

  // Analysis specific for beta-source data in multiframe format
  // Before running this procedure you will need to call the function
  // gTAF->GetRaw()->SetMultiFrame( aPlane, nFrames ), to set the plane
  // number (aPlane) and the number of events (nFrames) which are taken
  // consecutively.
  // AP, 2016/08/19 creation of this function

  double SOAveNCut   = 3.0;
  int BinSize_in_DAC = 3;
  int MaxDAC         = 10000;
  double MaxSON      = 300.0;

  fSession->SetEvents(nEvents);

  DTracker* tTracker  =  fSession->GetTracker();
  const Int_t nPlanes(tTracker->GetPlanesN());
  DPlane*   tPlane    = NULL;
  DHit*     aHit      = NULL;

  if(aPlane < 1 || aPlane > nPlanes) {
    cout << endl;
    cout << "MRaw::BetaSourceMultiFrameAnalysis:  ERROR, specified plane " << aPlane << " is not between 1 and " << nPlanes << endl;
    cout << endl;
    assert(false);
  }
  if(nFrames < 3) {
    cout << endl;
    cout << "MRaw::BetaSourceMultiFrameAnalysis:  ERROR, nFrames " << nFrames << " is smaller than 3. Check that the number you specify is consistent with data taking configuration." << endl;
    cout << endl;
    assert(false);
  }

  SetMultiFrame(aPlane,nFrames);
  tPlane = tTracker->GetPlane(aPlane);

  int    Ngoodpixels = 0;
  double AveNoise    = 0.0;
  TH2F* h_Noise2D = NULL;
  h_Noise2D = tPlane->GetHNoise();
  if(h_Noise2D) {
    for(int ix=0;ix<h_Noise2D->GetXaxis()->GetNbins();ix++) {
      for(int iy=0;iy<h_Noise2D->GetYaxis()->GetNbins();iy++) {
        double val = h_Noise2D->GetBinContent(ix+1,iy+1);
        if(val < 1.0e-6) continue;

        AveNoise += val;
        Ngoodpixels++;
      }
    }
    if(Ngoodpixels > 0) AveNoise /= Ngoodpixels;
  }
  else {
    cout << endl;
    cout << "MRaw::BetaSourceMultiFrameAnalysis:  Noise 2D distribution histogram is not initialized. Need it for beta-analysis. Exiting now!!!" << endl;
    cout << endl;
    assert(false);
  }

  cout << endl;
  cout << "MRaw::BetaSourceMultiFrameAnalysis:  Average noise is " << AveNoise << endl;
  cout << endl;

  if(ProduceTree) {
    fSession->MakeTree();
    fSession->SetFillLevel(1); // JB 2011/07/21
    if( fSession->GetTracker()->GetAlignmentStatus()==1 ) {
      printf("\nDSFProduction: -*- WARNING -*- only fixed (or primary) reference planes will be used for tracking!\n");
      printf("                                use gTAF->SetAlignStatus(2) to modify.\n");
    }
    else {
      printf("\nDSFProduction: both fixed and variable reference planes will be used for tracking.\n");
    }
  }

  int Rfiducial_col[2];
  int Rfiducial_row[2];
  if((Fiducial_col_min < 0 || Fiducial_col_max < 0) || (Fiducial_col_min > Fiducial_col_max) ) {
    Rfiducial_col[0] = 2;
    Rfiducial_col[1] = tPlane->GetStripsNu() - 2;
  }
  else {
    Rfiducial_col[0] = Fiducial_col_min;
    Rfiducial_col[1] = Fiducial_col_max;
  }
  if((Fiducial_row_min < 0 || Fiducial_row_max < 0) || (Fiducial_row_min > Fiducial_row_max) ) {
    Rfiducial_row[0] = 2;
    Rfiducial_row[1] = tPlane->GetStripsNv() - 2;
  }
  else {
    Rfiducial_row[0] = Fiducial_row_min;
    Rfiducial_row[1] = Fiducial_row_max;
  }

  //Booking histograms
  double Ineffic_Threshold = 0.5; //%
  TObject* g = NULL;
  TH2F* h_HitMap_vs_ColRow;
  TH1F* h_HitMap_vs_TS;
  TH1F* h_NHitsEvent;
  TH1F* h_MinDistEvent;
  TH1F* h_Seed_SON;
  TH1F* h_Seed_SON_TimeSpaceCut;
  TH1F* h_Seed_SOAveN;
  TH1F* h_Seed_SOAveN_TimeSpaceCut;
  TH1F* h_Seed_SOAveN_TimeSpaceCut_v2;
  TH1F* h_EfficTimeSpaceCut_vs_SOAveN;
  TH2F* h_Seed_SOAveN_vs_Max3NeighboursCharge;
  TH2F* h_Seed_SOAveN_vs_Max3NeighboursCharge_TimeSpaceCut;
  TH1F* h_Ineffic_vs_SeedSON_cut;
  TH1F* h_Seed_Charge_TimeSpaceCut;
  TH1F* h_ClusterCharge_TimeSpaceCut;
  int RSeedSONCuts[2];
  RSeedSONCuts[0] = 5;
  RSeedSONCuts[1] = 12;
  const int NSeedSONCuts(RSeedSONCuts[1] - RSeedSONCuts[0] + 1);
  TH1F **h_ClusterPseudoMult = new TH1F*[NSeedSONCuts];

  g = gROOT->FindObject("h_HitMap_vs_ColRow") ;
  if(g) h_HitMap_vs_ColRow = (TH2F*)g;
  else {
    //int Ncols = Rfiducial_col[1] - Rfiducial_col[0] + 1;
    //int Nrows = Rfiducial_row[1] - Rfiducial_row[0] + 1;
    h_HitMap_vs_ColRow = new TH2F("h_HitMap_vs_ColRow",
				  "Seed pixels Hit Map in Col vs Row",
				  tPlane->GetStripsNu(),-0.5,tPlane->GetStripsNu()-0.5,
				  tPlane->GetStripsNv(),-0.5,tPlane->GetStripsNv()-0.5);
    h_HitMap_vs_ColRow->SetXTitle("col");
    h_HitMap_vs_ColRow->GetXaxis()->CenterTitle(true);
    h_HitMap_vs_ColRow->SetYTitle("row");
    h_HitMap_vs_ColRow->GetYaxis()->CenterTitle(true);
    h_HitMap_vs_ColRow->SetZTitle("hits");
    h_HitMap_vs_ColRow->GetZaxis()->CenterTitle(true);
    h_HitMap_vs_ColRow->SetStats(false);
  }

  g = gROOT->FindObject("h_HitMap_vs_TS") ;
  if(g) h_HitMap_vs_TS = (TH1F*)g;
  else {
    h_HitMap_vs_TS = new TH1F("h_HitMap_vs_TS",
			      "Seed pixels Time-Stamp",
			      nFrames+2,-0.5,nFrames+1+0.5);
    h_HitMap_vs_TS->SetXTitle("Time-Stamp");
    h_HitMap_vs_TS->GetXaxis()->CenterTitle(true);
    h_HitMap_vs_TS->SetYTitle("hits");
    h_HitMap_vs_TS->GetYaxis()->CenterTitle(true);
    h_HitMap_vs_TS->SetLineColor(kBlue);
    h_HitMap_vs_TS->SetLineWidth(2);
  }

  g = gROOT->FindObject("h_NHitsEvent") ;
  if(g) h_NHitsEvent = (TH1F*)g;
  else {
    int Nbins = 120;
    double R[2];
    R[0] = -0.5;
    R[1] = Nbins+0.5;
    Nbins++;
    h_NHitsEvent = new TH1F("h_NHitsEvent",
			    "Number of hits in event",
			    Nbins,R[0],R[1]);
    h_NHitsEvent->SetXTitle("# hits");
    h_NHitsEvent->GetXaxis()->CenterTitle(true);
    h_NHitsEvent->SetYTitle("Events");
    h_NHitsEvent->GetYaxis()->CenterTitle(true);
    h_NHitsEvent->SetLineColor(kBlue);
    h_NHitsEvent->SetLineWidth(2);
  }

  g = gROOT->FindObject("h_MinDistEvent") ;
  if(g) h_MinDistEvent = (TH1F*)g;
  else {
    h_MinDistEvent = new TH1F("h_MinDistEvent",
			      "Min distance between hits in event",
			      100,0.0,sqrt(pow(tPlane->GetStripsNu(),2) + pow(tPlane->GetStripsNv(),2)));
    h_MinDistEvent->SetXTitle("min distance");
    h_MinDistEvent->GetXaxis()->CenterTitle(true);
    h_MinDistEvent->SetYTitle("Events");
    h_MinDistEvent->GetYaxis()->CenterTitle(true);
    h_MinDistEvent->SetLineColor(kBlue);
    h_MinDistEvent->SetLineWidth(2);
  }

  g = gROOT->FindObject("h_Seed_SON") ;
  if(g) h_Seed_SON = (TH1F*)g;
  else {
    double R_SON[2];
    R_SON[0] = 0.0;
    R_SON[1] = MaxDAC/AveNoise;
    int Nbins_SON = MaxDAC/BinSize_in_DAC;

    h_Seed_SON = new TH1F("h_Seed_SON",
			  "Seed pixel S/N",
			  Nbins_SON,
			  R_SON[0],R_SON[1]);
    h_Seed_SON->SetXTitle("Seed S/N");
    h_Seed_SON->GetXaxis()->CenterTitle(true);
    h_Seed_SON->SetYTitle("hits");
    h_Seed_SON->GetYaxis()->CenterTitle(true);
    h_Seed_SON->SetLineColor(kRed);
    h_Seed_SON->SetLineWidth(2);
  }

  g = gROOT->FindObject("h_Seed_SON_TimeSpaceCut") ;
  if(g) h_Seed_SON_TimeSpaceCut = (TH1F*)g;
  else {
    h_Seed_SON_TimeSpaceCut = new TH1F("h_Seed_SON_TimeSpaceCut",
				       "Seed pixel S/N after Temporal-spatial veto",
				       h_Seed_SON->GetXaxis()->GetNbins(),
				       h_Seed_SON->GetXaxis()->GetXmin(),
				       h_Seed_SON->GetXaxis()->GetXmax());
    h_Seed_SON_TimeSpaceCut->SetXTitle("Seed S/N");
    h_Seed_SON_TimeSpaceCut->GetXaxis()->CenterTitle(true);
    h_Seed_SON_TimeSpaceCut->SetYTitle("hits");
    h_Seed_SON_TimeSpaceCut->GetYaxis()->CenterTitle(true);
    h_Seed_SON_TimeSpaceCut->SetLineColor(kBlue);
    h_Seed_SON_TimeSpaceCut->SetLineWidth(2);
  }

  g = gROOT->FindObject("h_Seed_SOAveN") ;
  if(g) h_Seed_SOAveN = (TH1F*)g;
  else {
    h_Seed_SOAveN = new TH1F("h_Seed_SOAveN",
			     "Seed pixel S/<N>",
			     h_Seed_SON->GetXaxis()->GetNbins(),
			     h_Seed_SON->GetXaxis()->GetXmin(),
			     h_Seed_SON->GetXaxis()->GetXmax());
    h_Seed_SOAveN->SetXTitle("Seed S/<N>");
    h_Seed_SOAveN->GetXaxis()->CenterTitle(true);
    h_Seed_SOAveN->SetYTitle("hits");
    h_Seed_SOAveN->GetYaxis()->CenterTitle(true);
    h_Seed_SOAveN->SetLineColor(kRed);
    h_Seed_SOAveN->SetLineWidth(2);
  }

  g = gROOT->FindObject("h_Seed_SOAveN_TimeSpaceCut") ;
  if(g) h_Seed_SOAveN_TimeSpaceCut = (TH1F*)g;
  else {
    h_Seed_SOAveN_TimeSpaceCut = new TH1F("h_Seed_SOAveN_TimeSpaceCut",
					  "Seed pixel S/<N> after Temporal-spatial veto",
					  h_Seed_SON->GetXaxis()->GetNbins(),
					  h_Seed_SON->GetXaxis()->GetXmin(),
					  h_Seed_SON->GetXaxis()->GetXmax());
    h_Seed_SOAveN_TimeSpaceCut->SetXTitle("Seed S/<N>");
    h_Seed_SOAveN_TimeSpaceCut->GetXaxis()->CenterTitle(true);
    h_Seed_SOAveN_TimeSpaceCut->SetYTitle("hits");
    h_Seed_SOAveN_TimeSpaceCut->GetYaxis()->CenterTitle(true);
    h_Seed_SOAveN_TimeSpaceCut->SetLineColor(kBlue);
    h_Seed_SOAveN_TimeSpaceCut->SetLineWidth(2);
  }

  g = gROOT->FindObject("h_Seed_SOAveN_TimeSpaceCut_v2") ;
  if(g) h_Seed_SOAveN_TimeSpaceCut_v2 = (TH1F*)g;
  else {
    h_Seed_SOAveN_TimeSpaceCut_v2 = new TH1F("h_Seed_SOAveN_TimeSpaceCut_v2",
					     "Seed pixel S/<N> after Temporal-spatial veto",
					     h_Seed_SON->GetXaxis()->GetNbins(),
					     h_Seed_SON->GetXaxis()->GetXmin(),
					     h_Seed_SON->GetXaxis()->GetXmax());
    h_Seed_SOAveN_TimeSpaceCut_v2->SetXTitle("Seed S/<N>");
    h_Seed_SOAveN_TimeSpaceCut_v2->GetXaxis()->CenterTitle(true);
    h_Seed_SOAveN_TimeSpaceCut_v2->SetYTitle("hits");
    h_Seed_SOAveN_TimeSpaceCut_v2->GetYaxis()->CenterTitle(true);
    h_Seed_SOAveN_TimeSpaceCut_v2->SetLineColor(kBlue);
    h_Seed_SOAveN_TimeSpaceCut_v2->SetLineWidth(2);
  }

  g = gROOT->FindObject("h_EfficTimeSpaceCut_vs_SOAveN");
  if(g) h_EfficTimeSpaceCut_vs_SOAveN = (TH1F*)g;
  else {
    h_EfficTimeSpaceCut_vs_SOAveN = new TH1F("h_EfficTimeSpaceCut_vs_SOAveN",
					     "Seed pixel S/<N>",
					     h_Seed_SON->GetXaxis()->GetNbins(),
					     h_Seed_SON->GetXaxis()->GetXmin(),
					     h_Seed_SON->GetXaxis()->GetXmax());
    h_EfficTimeSpaceCut_vs_SOAveN->SetXTitle("Seed S/<N>");
    h_EfficTimeSpaceCut_vs_SOAveN->GetXaxis()->CenterTitle(true);
    h_EfficTimeSpaceCut_vs_SOAveN->SetYTitle("Efficiency (%)");
    h_EfficTimeSpaceCut_vs_SOAveN->GetYaxis()->CenterTitle(true);
    h_EfficTimeSpaceCut_vs_SOAveN->SetLineColor(kBlue);
    h_EfficTimeSpaceCut_vs_SOAveN->SetLineWidth(2);
  }

  g = gROOT->FindObject("h_Seed_SOAveN_vs_Max3NeighboursCharge");
  if(g) h_Seed_SOAveN_vs_Max3NeighboursCharge = (TH2F*)g;
  else {
    int R_Max3NeighboursCharge[2];
    R_Max3NeighboursCharge[0] = -5000;
    R_Max3NeighboursCharge[1] = +5000;
    int Nbins_Max3NeighboursCharge  = R_Max3NeighboursCharge[1] - R_Max3NeighboursCharge[0];
    Nbins_Max3NeighboursCharge     /= BinSize_in_DAC;

    h_Seed_SOAveN_vs_Max3NeighboursCharge = new TH2F("h_Seed_SOAveN_vs_Max3NeighboursCharge",
					             "Seed pixel S/<N> vs Charge of 3 Neighbours with maximum charge",
						     h_Seed_SON->GetXaxis()->GetNbins(),
						     h_Seed_SON->GetXaxis()->GetXmin(),
						     h_Seed_SON->GetXaxis()->GetXmax(),
						     Nbins_Max3NeighboursCharge,
						     R_Max3NeighboursCharge[0],
						     R_Max3NeighboursCharge[1]);
    h_Seed_SOAveN_vs_Max3NeighboursCharge->SetXTitle("Seed S/<N>");
    h_Seed_SOAveN_vs_Max3NeighboursCharge->GetXaxis()->CenterTitle(true);
    h_Seed_SOAveN_vs_Max3NeighboursCharge->SetYTitle("Charge of 3 Neighbours (ADCu)");
    h_Seed_SOAveN_vs_Max3NeighboursCharge->GetYaxis()->CenterTitle(true);
    h_Seed_SOAveN_vs_Max3NeighboursCharge->SetZTitle("hits");
    h_Seed_SOAveN_vs_Max3NeighboursCharge->GetZaxis()->CenterTitle(true);
  }

  g = gROOT->FindObject("h_Seed_SOAveN_vs_Max3NeighboursCharge_TimeSpaceCut");
  if(g) h_Seed_SOAveN_vs_Max3NeighboursCharge_TimeSpaceCut = (TH2F*)g;
  else {
    h_Seed_SOAveN_vs_Max3NeighboursCharge_TimeSpaceCut = new TH2F("h_Seed_SOAveN_vs_Max3NeighboursCharge_TimeSpaceCut",
								  "Seed pixel S/<N> vs Charge of 3 Neighbours with maximum charge after Temporal-spatial veto",
								  h_Seed_SOAveN_vs_Max3NeighboursCharge->GetXaxis()->GetNbins(),
								  h_Seed_SOAveN_vs_Max3NeighboursCharge->GetXaxis()->GetXmin(),
								  h_Seed_SOAveN_vs_Max3NeighboursCharge->GetXaxis()->GetXmax(),
								  h_Seed_SOAveN_vs_Max3NeighboursCharge->GetYaxis()->GetNbins(),
								  h_Seed_SOAveN_vs_Max3NeighboursCharge->GetYaxis()->GetXmin(),
								  h_Seed_SOAveN_vs_Max3NeighboursCharge->GetYaxis()->GetXmax());
    h_Seed_SOAveN_vs_Max3NeighboursCharge_TimeSpaceCut->SetXTitle("Seed S/<N>");
    h_Seed_SOAveN_vs_Max3NeighboursCharge_TimeSpaceCut->GetXaxis()->CenterTitle(true);
    h_Seed_SOAveN_vs_Max3NeighboursCharge_TimeSpaceCut->SetYTitle("Charge of 3 Neighbours (ADCu)");
    h_Seed_SOAveN_vs_Max3NeighboursCharge_TimeSpaceCut->GetYaxis()->CenterTitle(true);
    h_Seed_SOAveN_vs_Max3NeighboursCharge_TimeSpaceCut->SetZTitle("hits");
    h_Seed_SOAveN_vs_Max3NeighboursCharge_TimeSpaceCut->GetZaxis()->CenterTitle(true);
  }

  g = gROOT->FindObject("h_Ineffic_vs_SeedSON_cut");
  if(g) h_Ineffic_vs_SeedSON_cut = (TH1F*)g;
  else {
    int Nbins_Ineffic_vs_SeedSON_cut = 0;
    double R_Ineffic_vs_SeedSON_cut[2];
    R_Ineffic_vs_SeedSON_cut[0] =  0.0;
    R_Ineffic_vs_SeedSON_cut[1] = 30.0;
    double Max_temp = -999.9;
    for(int i=0;i<h_Seed_SON->GetXaxis()->GetNbins();i++) {
      double c = h_Seed_SON->GetBinCenter(i+1);
      if(c > R_Ineffic_vs_SeedSON_cut[1]) break;

      Nbins_Ineffic_vs_SeedSON_cut++;
      Max_temp = c;

    }
    if(TMath::Abs(R_Ineffic_vs_SeedSON_cut[1] - Max_temp) > 1.0e-6) R_Ineffic_vs_SeedSON_cut[1] = Max_temp;

    h_Ineffic_vs_SeedSON_cut = new TH1F("h_Ineffic_vs_SeedSON_cut",
					"Inifficiency vs Seed pixel S/<N> cut",
					Nbins_Ineffic_vs_SeedSON_cut,R_Ineffic_vs_SeedSON_cut[0],R_Ineffic_vs_SeedSON_cut[1]);
    h_Ineffic_vs_SeedSON_cut->SetXTitle("Seed S/<N> cut");
    h_Ineffic_vs_SeedSON_cut->GetXaxis()->CenterTitle(true);
    h_Ineffic_vs_SeedSON_cut->SetYTitle("Inefficiency (%)");
    h_Ineffic_vs_SeedSON_cut->GetYaxis()->CenterTitle(true);
    h_Ineffic_vs_SeedSON_cut->SetLineColor(kBlue);
    h_Ineffic_vs_SeedSON_cut->SetLineWidth(2);
  }

  g = gROOT->FindObject("h_Seed_Charge_TimeSpaceCut") ;
  if(g) h_Seed_Charge_TimeSpaceCut = (TH1F*)g;
  else {
    h_Seed_Charge_TimeSpaceCut = new TH1F("h_Seed_Charge_TimeSpaceCut",
					  "Seed pixel charge after Temporal-spatial veto",
					  MaxDAC/BinSize_in_DAC,
					  0.0,
					  MaxDAC);
    h_Seed_Charge_TimeSpaceCut->SetXTitle("Seed pixel charge (ADCu)");
    h_Seed_Charge_TimeSpaceCut->GetXaxis()->CenterTitle(true);
    h_Seed_Charge_TimeSpaceCut->SetYTitle("hits");
    h_Seed_Charge_TimeSpaceCut->GetYaxis()->CenterTitle(true);
    h_Seed_Charge_TimeSpaceCut->SetLineColor(kBlue);
    h_Seed_Charge_TimeSpaceCut->SetLineWidth(2);
  }

  g = gROOT->FindObject("h_ClusterCharge_TimeSpaceCut") ;
  if(g) h_ClusterCharge_TimeSpaceCut = (TH1F*)g;
  else {
    h_ClusterCharge_TimeSpaceCut = new TH1F("h_ClusterCharge_TimeSpaceCut",
					    "Cluster charge after Temporal-spatial veto",
					    MaxDAC/BinSize_in_DAC,
					    0.0,
					    MaxDAC);
    h_ClusterCharge_TimeSpaceCut->SetXTitle("Cluster charge (ADCu)");
    h_ClusterCharge_TimeSpaceCut->GetXaxis()->CenterTitle(true);
    h_ClusterCharge_TimeSpaceCut->SetYTitle("hits");
    h_ClusterCharge_TimeSpaceCut->GetYaxis()->CenterTitle(true);
    h_ClusterCharge_TimeSpaceCut->SetLineColor(kBlue);
    h_ClusterCharge_TimeSpaceCut->SetLineWidth(2);
  }

  int Nbins_mult = 40;
  double R_mult[2];
  R_mult[0] = -0.5;
  R_mult[1] = Nbins_mult+0.5;
  Nbins_mult++;
  for(int i=0;i<NSeedSONCuts;i++) {
    int TheCut = RSeedSONCuts[0] + i;
    TString HistName  = TString("h_ClusterPseudoMult_cut") + long(i+1);
    TString HistTitle = TString("Cluster pixel multiplicity for S/<N> cut ") + long(TheCut);
    g = gROOT->FindObject(HistName.Data()) ;
    if(g) h_ClusterPseudoMult[i] = (TH1F*)g;
    else {
      h_ClusterPseudoMult[i] = new TH1F(HistName.Data(),
					HistTitle.Data(),
					Nbins_mult,R_mult[0],R_mult[1]);
      h_ClusterPseudoMult[i]->SetXTitle("Cluster charge (ADCu)");
      h_ClusterPseudoMult[i]->GetXaxis()->CenterTitle(true);
      h_ClusterPseudoMult[i]->SetYTitle("hits");
      h_ClusterPseudoMult[i]->GetYaxis()->CenterTitle(true);
      h_ClusterPseudoMult[i]->SetLineColor(kBlue);
      h_ClusterPseudoMult[i]->SetLineWidth(2);
    }
  }



  //Loop over the requested number of events
  for( Int_t iEvt=0; iEvt < nEvents; iEvt++) {
    if( !(fSession->NextRawEvent()) ) break; // JB 2009/06/26
    tTracker->Update();

    //tPlane = tTracker->GetPlane(aPlane);
    vector<DPixel*> *aList = tPlane->GetListOfPixels();

    //cout << "evt = " << iEvt+1 << "  " << tPlane->GetHitsN() << endl;
    h_NHitsEvent->Fill(tPlane->GetHitsN());

    std::vector<int>  ListOfAllHits;
    std::vector<bool> ListOfGoodHits;
    std::vector<double> ListOfSON;
    std::vector<double> ListOfSOAveN;
    std::vector<double> ListOfMax3NeighboursCharge;
    ListOfAllHits.clear();
    ListOfGoodHits.clear();
    ListOfMax3NeighboursCharge.clear();
    ListOfSON.clear();
    ListOfSOAveN.clear();

    for( Int_t iHit=1; iHit<=tPlane->GetHitsN(); iHit++) { //loop on hits (starts at 1 !!)
      aHit = (DHit*)tPlane->GetHit(iHit);
      int col_seed = aList->at(aHit->GetIndexInOriginalList(0))->GetPixelColumn();
      int row_seed = aList->at(aHit->GetIndexInOriginalList(0))->GetPixelLine();
      int ts_seed  = aList->at(aHit->GetIndexInOriginalList(0))->GetTimestamp();

      if(col_seed < Rfiducial_col[0] || col_seed > Rfiducial_col[1]) continue;
      if(row_seed < Rfiducial_row[0] || row_seed > Rfiducial_row[1]) continue;
      if(ts_seed == 0 || ts_seed == nFrames-1) continue;

      int idx_seed      = -999;
      double Max_charge = -1.0e+20;
      for(int ipixInHit=0; ipixInHit < aHit->GetStripsInCluster(); ipixInHit++) { //Loop over pixels of this hit
	if(Max_charge < aList->at(aHit->GetIndexInOriginalList(ipixInHit))->GetPulseHeight()) {
	  Max_charge = aList->at(aHit->GetIndexInOriginalList(ipixInHit))->GetPulseHeight();
	  idx_seed = ipixInHit;
	}
      }  //end Loop over pixels of this hit
      double SeedSOAveN = aList->at(aHit->GetIndexInOriginalList(idx_seed))->GetPulseHeight()/AveNoise;

      if(SeedSOAveN < SOAveNCut) continue;

      ListOfAllHits.push_back(iHit);
      ListOfGoodHits.push_back(true);
    } //end loop on hits

    double MinDist_TS[nFrames];
    for(int its=0;its<nFrames;its++) MinDist_TS[its] = 1.0e+20;
    for(Int_t iHitFidu=0; iHitFidu < int(ListOfAllHits.size()); iHitFidu++) { //loop on fiducial hits
      int iHit = ListOfAllHits[iHitFidu];
      aHit = (DHit*)tPlane->GetHit(iHit);

      int col_seed = aList->at(aHit->GetIndexInOriginalList(0))->GetPixelColumn();
      int row_seed = aList->at(aHit->GetIndexInOriginalList(0))->GetPixelLine();
      int ts_seed  = aList->at(aHit->GetIndexInOriginalList(0))->GetTimestamp();

      h_HitMap_vs_ColRow->Fill(col_seed,row_seed);
      h_HitMap_vs_TS->Fill(ts_seed);

      //Claculate the charge of the 3 neighbour pixels with the highest charge
      int idx_seed      = -999;
      double Max_charge = -1.0e+20;
      double Max3NeighboursCharge = 0.0;
      std::vector<double> PixelChargeList;
      PixelChargeList.clear();
      for(int ipixInHit=0; ipixInHit < aHit->GetStripsInCluster(); ipixInHit++) { //Loop over pixels of this hit
	//Exclude seed pixel
	if(Max_charge < aList->at(aHit->GetIndexInOriginalList(ipixInHit))->GetPulseHeight()) {
	  Max_charge = aList->at(aHit->GetIndexInOriginalList(ipixInHit))->GetPulseHeight();
	  idx_seed = ipixInHit;
	}

	if(ipixInHit == 0) continue;
	PixelChargeList.push_back(aList->at(aHit->GetIndexInOriginalList(ipixInHit))->GetPulseHeight());
      }  //end Loop over pixels of this hit

      double SeedSON    = aList->at(aHit->GetIndexInOriginalList(idx_seed))->GetPulseHeightToNoise();
      double SeedSOAveN = aList->at(aHit->GetIndexInOriginalList(idx_seed))->GetPulseHeight()/AveNoise;
      h_Seed_SON->Fill(SeedSON);
      h_Seed_SOAveN->Fill(SeedSOAveN);

      ListOfSON.push_back(SeedSON);
      ListOfSOAveN.push_back(SeedSOAveN);

      //Order pixel charge list from highest to lowest
      for(int iii=2;iii<=int(PixelChargeList.size());iii++) {
        for(int jjj=0;jjj<=int(PixelChargeList.size())-iii;jjj++) {
	  int Ch_jjj   = PixelChargeList[jjj];
          int Ch_jjjp1 = PixelChargeList[jjj+1];

	  if(Ch_jjj < Ch_jjjp1) {
	    int  Ch_aux            = PixelChargeList[jjj];
	    PixelChargeList[jjj]   = PixelChargeList[jjj+1];
	    PixelChargeList[jjj+1] = Ch_aux;
	  }
        }
      }
      int Npixel_loop = PixelChargeList.size();
      if(Npixel_loop > 3) Npixel_loop = 3;
      for(int iii=0;iii<Npixel_loop;iii++) Max3NeighboursCharge += PixelChargeList[iii];
      PixelChargeList.clear();
      ListOfMax3NeighboursCharge.push_back(Max3NeighboursCharge);

      h_Seed_SOAveN_vs_Max3NeighboursCharge->Fill(SeedSOAveN,Max3NeighboursCharge);

      //Calculate the minimum distance between two cluster for the same time-stamp
      for(Int_t iHitFidu2=0; iHitFidu2 < int(ListOfAllHits.size()); iHitFidu2++) { //second loop on fiducial hits
	if(iHitFidu == iHitFidu2) continue;
	int iHit2 = ListOfAllHits[iHitFidu2];
	DHit* aHit2 = (DHit*)tPlane->GetHit(iHit2);

	int col_seed2 = aList->at(aHit2->GetIndexInOriginalList(0))->GetPixelColumn();
        int row_seed2 = aList->at(aHit2->GetIndexInOriginalList(0))->GetPixelLine();
        int ts_seed2  = aList->at(aHit2->GetIndexInOriginalList(0))->GetTimestamp();
	if(ts_seed != ts_seed2) continue;

	double distance = sqrt(pow(col_seed - col_seed2,2) + pow(row_seed - row_seed2,2));
	if(MinDist_TS[ts_seed] > distance) MinDist_TS[ts_seed] = distance;
      } //end second loop on fiducial hits

      //if(!ListOfGoodHits[iHitFidu]) continue;

      for(Int_t iHitFidu2=0; iHitFidu2 < int(ListOfAllHits.size()); iHitFidu2++) { //third loop on fiducial hits
	if(iHitFidu == iHitFidu2) continue;
	//if(!ListOfGoodHits[iHitFidu2]) continue;

	int iHit2 = ListOfAllHits[iHitFidu2];
	DHit* aHit2 = (DHit*)tPlane->GetHit(iHit2);

	int col_seed2 = aList->at(aHit2->GetIndexInOriginalList(0))->GetPixelColumn();
        int row_seed2 = aList->at(aHit2->GetIndexInOriginalList(0))->GetPixelLine();
        int ts_seed2  = aList->at(aHit2->GetIndexInOriginalList(0))->GetTimestamp();

	if(abs(ts_seed - ts_seed2) != 1) continue;

	int delta_col = col_seed - col_seed2;
	int delta_row = row_seed - row_seed2;

	if(abs(delta_row) < Npixel_TimeSpaceCut && abs(delta_col) < Npixel_TimeSpaceCut) {
	  ListOfGoodHits[iHitFidu]  = false;
	  ListOfGoodHits[iHitFidu2] = false;
	}

      } //end third loop on fiducial hits

    } //end loop on fiducial hits
    std::vector<int> VetoedTS;
    double MinDist = 1.0e+20;
    for(int its=0;its<nFrames;its++) {
      if(MinDist_TS[its] < 5.0*sqrt(2.0)) VetoedTS.push_back(its);
      if(MinDist > MinDist_TS[its]) MinDist = MinDist_TS[its];
    }
    if(MinDist < 1.0e+20) h_MinDistEvent->Fill(MinDist);

    //Now veto all the Time-stamps for which the minimum distance between clusters was too small
    for(Int_t iHitFidu=0; iHitFidu < int(ListOfAllHits.size()); iHitFidu++) { //loop on good hits
      if(!ListOfGoodHits[iHitFidu]) continue;

      int iHit = ListOfAllHits[iHitFidu];
      aHit = (DHit*)tPlane->GetHit(iHit);
      int ts_seed  = aList->at(aHit->GetIndexInOriginalList(0))->GetTimestamp();

      for(int itsveto=0;itsveto<int(VetoedTS.size());itsveto++) {
	if(VetoedTS[itsveto] == ts_seed) {
	  ListOfGoodHits[iHitFidu] = false;
	  break;
	}
      }

    } //end loop on good hits

    //Now loop over the good hits and proceed to fill the histograms
    for(Int_t iHitFidu=0; iHitFidu < int(ListOfAllHits.size()); iHitFidu++) { //loop on good hits
      if(!ListOfGoodHits[iHitFidu]) continue;

      int iHit = ListOfAllHits[iHitFidu];
      aHit = (DHit*)tPlane->GetHit(iHit);

      //double SeedSON    = aList->at(aHit->GetIndexInOriginalList(0))->GetPulseHeightToNoise();
      //double SeedSOAveN = aList->at(aHit->GetIndexInOriginalList(0))->GetPulseHeight()/AveNoise;

      h_Seed_SON_TimeSpaceCut->Fill(ListOfSON[iHitFidu]);
      h_Seed_SOAveN_TimeSpaceCut->Fill(ListOfSOAveN[iHitFidu]);
      h_Seed_SOAveN_TimeSpaceCut_v2->Fill(ListOfSOAveN[iHitFidu]);

      h_Seed_Charge_TimeSpaceCut->Fill(aList->at(aHit->GetIndexInOriginalList(0))->GetPulseHeight());
      h_ClusterCharge_TimeSpaceCut->Fill(aHit->GetClusterPulseSum());

      h_Seed_SOAveN_vs_Max3NeighboursCharge_TimeSpaceCut->Fill(ListOfSOAveN[iHitFidu],ListOfMax3NeighboursCharge[iHitFidu]);

      //Claculate the charge of the 3 neighbour pixels with the highest charge
      int ClusterPixelMult[NSeedSONCuts];
      for(int isncut=0;isncut<NSeedSONCuts;isncut++) ClusterPixelMult[isncut] = 0;
      for(int ipixInHit=0; ipixInHit < aHit->GetStripsInCluster(); ipixInHit++) { //Loop over pixels of this hit
	double SN = aList->at(aHit->GetIndexInOriginalList(ipixInHit))->GetPulseHeightToNoise();
        if(AveNoise > 0.0) SN = aList->at(aHit->GetIndexInOriginalList(ipixInHit))->GetPulseHeight()/AveNoise;

	for(int isncut=0;isncut<NSeedSONCuts;isncut++) {
          int TheCut = RSeedSONCuts[0] + isncut;
	  if(ipixInHit==0) {
	    if(TheCut < SAveN_cutFit) TheCut = SAveN_cutFit;
	  }
	  if(SN >= TheCut) ClusterPixelMult[isncut]++;
	}

      }  //end Loop over pixels of this hit
      for(int isncut=0;isncut<NSeedSONCuts;isncut++) {
	if(ClusterPixelMult[isncut] > 0) h_ClusterPseudoMult[isncut]->Fill(ClusterPixelMult[isncut]);
      }

    } //end loop on good hits

    ListOfAllHits.clear();
    ListOfGoodHits.clear();
    ListOfSON.clear();
    ListOfSOAveN.clear();
    ListOfMax3NeighboursCharge.clear();
    VetoedTS.clear();

    if(ProduceTree) fSession->FillTree();

  } // END LOOP ON EVENTS

  double REfficTimeSpaceCut[2];
  REfficTimeSpaceCut[0] = +1.0e+20;
  REfficTimeSpaceCut[1] = -1.0e+20;
  for(int i=0;i<h_Seed_SOAveN->GetXaxis()->GetNbins();i++) {
    double Num = h_Seed_SOAveN_TimeSpaceCut->GetBinContent(i+1);
    double Den = h_Seed_SOAveN->GetBinContent(i+1);

    double effic,err_effic;
    if(Den > 25) {
      effic     = Num/Den;
      err_effic = sqrt(effic*(1.0 - effic)/Den);

      effic     *= 100.0;
      err_effic *= 100.0;
    }
    else {
      effic     = -10.0;
      err_effic =  1.0e-10;
    }

    if(effic >= 0) {
      if(REfficTimeSpaceCut[0] > effic - err_effic) REfficTimeSpaceCut[0] = effic - err_effic;
      if(REfficTimeSpaceCut[1] < effic + err_effic) REfficTimeSpaceCut[1] = effic + err_effic;
    }

    h_EfficTimeSpaceCut_vs_SOAveN->SetBinContent(i+1,effic);
    h_EfficTimeSpaceCut_vs_SOAveN->SetBinError(i+1,err_effic);
  }
  double porcent,delta;
  porcent = 0.10;
  delta = REfficTimeSpaceCut[1] - REfficTimeSpaceCut[0];
  REfficTimeSpaceCut[0] -= porcent*delta;
  REfficTimeSpaceCut[1] += porcent*delta;
  if(REfficTimeSpaceCut[0] < 0.0) REfficTimeSpaceCut[0] = 0.0;
  h_EfficTimeSpaceCut_vs_SOAveN->SetMinimum(REfficTimeSpaceCut[0]);
  h_EfficTimeSpaceCut_vs_SOAveN->SetMaximum(REfficTimeSpaceCut[1]);


  fSession->GetDataAcquisition()->PrintStatistics();
  tTracker->PrintStatistics();

  if(ProduceTree) {
    fSession->Finish();

    Char_t New_File_Name[1000];
    Char_t Old_File_Name[1000];
    sprintf(Old_File_Name,"%s/%s",(const char*)fSession->GetSummaryFilePath(),fSession->GetSummaryFileName().Data());
    sprintf(Old_File_Name,"%s", fTool.LocalizeDirName( Old_File_Name)); // JB 2011/07/07
    sprintf(New_File_Name,"%srun%d_0%d.root",(const char*)fSession->GetSummaryFilePath(),fSession->GetRunNumber(),GetFileNumber()+1);
    sprintf(New_File_Name,"%s", fTool.LocalizeDirName( New_File_Name)); // JB 2011/07/07

    gSystem->Rename(Old_File_Name,New_File_Name);
    cout<< fSession->GetSummaryFileName().Data()  <<" had been renamed : "<< New_File_Name <<endl;
    ofstream logfile;
    logfile.open("DSFProd.log",ios::app);
    logfile<< fSession->GetSummaryFileName().Data()  <<" had been renamed : "<< New_File_Name <<endl;
    logfile.close();
  }


  TCanvas *cbeta1 = NULL;
  g = gROOT->FindObject("cbeta1") ;
  if(g) cbeta1 = (TCanvas*)g;
  cbeta1 = new TCanvas("cbeta1","Cumulate Hits", 5, 5,800,700);
  cbeta1->Clear();
  cbeta1->UseCurrentStyle();
  cbeta1->cd();
  TPaveLabel* label = new TPaveLabel();
  Char_t canvasTitle[200];
  sprintf(canvasTitle, "Run %d, cumul over %d events", fSession->GetRunNumber(), nEvents);
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad1 = new TPad("pad1","",0.,0.,1.,0.965);
  pad1->Draw();
  pad1->Divide(2,2);
  pad1->cd(1);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetBottomMargin(0.20);
  gPad->SetRightMargin(0.15);
  TLine* h1_colrow = new TLine(Rfiducial_col[0]-0.5,Rfiducial_row[0]-0.5,Rfiducial_col[1]+0.5,Rfiducial_row[0]-0.5);
  h1_colrow->SetLineColor(kRed);
  h1_colrow->SetLineWidth(2);
  h1_colrow->SetLineStyle(2);
  TLine* h2_colrow = new TLine(Rfiducial_col[0]-0.5,Rfiducial_row[1]+0.5,Rfiducial_col[1]+0.5,Rfiducial_row[1]+0.5);
  h2_colrow->SetLineColor(kRed);
  h2_colrow->SetLineWidth(2);
  h2_colrow->SetLineStyle(2);
  TLine* v1_colrow = new TLine(Rfiducial_col[0]-0.5,Rfiducial_row[0]-0.5,Rfiducial_col[0]-0.5,Rfiducial_row[1]+0.5);
  v1_colrow->SetLineColor(kRed);
  v1_colrow->SetLineWidth(2);
  v1_colrow->SetLineStyle(2);
  TLine* v2_colrow = new TLine(Rfiducial_col[1]+0.5,Rfiducial_row[0]-0.5,Rfiducial_col[1]+0.5,Rfiducial_row[1]+0.5);
  v2_colrow->SetLineColor(kRed);
  v2_colrow->SetLineWidth(2);
  v2_colrow->SetLineStyle(2);
  h_HitMap_vs_ColRow->Draw("colz");
  h1_colrow->Draw();
  h2_colrow->Draw();
  v1_colrow->Draw();
  v2_colrow->Draw();
  pad1->cd(2);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetBottomMargin(0.20);
  h_HitMap_vs_TS->Draw();
  pad1->cd(3);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetBottomMargin(0.20);
  h_NHitsEvent->Draw();
  pad1->cd(4);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetBottomMargin(0.20);
  h_MinDistEvent->Draw();
  cbeta1->Update();

  double Max_SON = -1.0e+20;
  for(int i=0;i<h_Seed_SON->GetXaxis()->GetNbins();i++) {
    double c = h_Seed_SON->GetBinCenter(i+1);
    double v = h_Seed_SON->GetBinContent(i+1);

    if(c < SAveN_cutFit) continue;
    if(Max_SON < v) Max_SON = v;
  }
  porcent = 0.10;
  h_Seed_SON->SetMaximum(Max_SON*(1.0+porcent));

  Max_SON = -1.0e+20;
  for(int i=0;i<h_Seed_SON_TimeSpaceCut->GetXaxis()->GetNbins();i++) {
    double c = h_Seed_SON_TimeSpaceCut->GetBinCenter(i+1);
    double v = h_Seed_SON_TimeSpaceCut->GetBinContent(i+1);

    if(c < SAveN_cutFit) continue;
    if(Max_SON < v) Max_SON = v;
  }
  porcent = 0.10;
  h_Seed_SON_TimeSpaceCut->SetMaximum(Max_SON*(1.0+porcent));

  Max_SON = -1.0e+20;
  for(int i=0;i<h_Seed_SOAveN->GetXaxis()->GetNbins();i++) {
    double c = h_Seed_SOAveN->GetBinCenter(i+1);
    double v = h_Seed_SOAveN->GetBinContent(i+1);

    if(c < SAveN_cutFit) continue;
    if(Max_SON < v) Max_SON = v;
  }
  porcent = 0.10;
  h_Seed_SOAveN->SetMaximum(Max_SON*(1.0+porcent));

  Max_SON = -1.0e+20;
  for(int i=0;i<h_Seed_SOAveN_TimeSpaceCut->GetXaxis()->GetNbins();i++) {
    double c = h_Seed_SOAveN_TimeSpaceCut->GetBinCenter(i+1);
    double v = h_Seed_SOAveN_TimeSpaceCut->GetBinContent(i+1);

    if(c < SAveN_cutFit) continue;
    if(Max_SON < v) Max_SON = v;
  }
  porcent = 0.10;
  h_Seed_SOAveN_TimeSpaceCut->SetMaximum(Max_SON*(1.0+porcent));

  h_Seed_SOAveN_TimeSpaceCut_v2->Sumw2();
  Max_SON = -1.0e+20;
  for(int i=0;i<h_Seed_SOAveN_TimeSpaceCut_v2->GetXaxis()->GetNbins();i++) {
    double c = h_Seed_SOAveN_TimeSpaceCut_v2->GetBinCenter(i+1);
    double v = h_Seed_SOAveN_TimeSpaceCut_v2->GetBinContent(i+1);
    double e = h_Seed_SOAveN_TimeSpaceCut_v2->GetBinError(i+1);

    if(c < SAveN_cutFit) continue;
    if(Max_SON < v+e) Max_SON = v+e;
  }
  porcent = 0.10;
  h_Seed_SOAveN_TimeSpaceCut_v2->SetMaximum(Max_SON*(1.0+porcent));

  TLine* lv_SON = new TLine(SAveN_cutFit,h_Seed_SON->GetMinimum(),
                            SAveN_cutFit,h_Seed_SON->GetMaximum());
  lv_SON->SetLineColor(kGreen+2);
  lv_SON->SetLineWidth(2);
  lv_SON->SetLineStyle(2);
  TLine* lv_SON_TimeSpaceCut = new TLine(SAveN_cutFit,h_Seed_SON_TimeSpaceCut->GetMinimum(),
                                         SAveN_cutFit,h_Seed_SON_TimeSpaceCut->GetMaximum());
  lv_SON_TimeSpaceCut->SetLineColor(kGreen+2);
  lv_SON_TimeSpaceCut->SetLineWidth(2);
  lv_SON_TimeSpaceCut->SetLineStyle(2);
  TLine* lv_SOAveN = new TLine(SAveN_cutFit,h_Seed_SOAveN->GetMinimum(),
                               SAveN_cutFit,h_Seed_SOAveN->GetMaximum());
  lv_SOAveN->SetLineColor(kGreen+2);
  lv_SOAveN->SetLineWidth(2);
  lv_SOAveN->SetLineStyle(2);
  TLine* lv_SOAveN_TimeSpaceCut = new TLine(SAveN_cutFit,h_Seed_SOAveN_TimeSpaceCut->GetMinimum(),
                                            SAveN_cutFit,h_Seed_SOAveN_TimeSpaceCut->GetMaximum());
  lv_SOAveN_TimeSpaceCut->SetLineColor(kGreen+2);
  lv_SOAveN_TimeSpaceCut->SetLineWidth(2);
  lv_SOAveN_TimeSpaceCut->SetLineStyle(2);
  TLine* lv_SOAveN_TimeSpaceCut_v2 = new TLine(SAveN_cutFit,h_Seed_SOAveN_TimeSpaceCut_v2->GetMinimum(),
                                               SAveN_cutFit,h_Seed_SOAveN_TimeSpaceCut_v2->GetMaximum());
  lv_SOAveN_TimeSpaceCut_v2->SetLineColor(kGreen+2);
  lv_SOAveN_TimeSpaceCut_v2->SetLineWidth(2);
  lv_SOAveN_TimeSpaceCut_v2->SetLineStyle(2);

  TCanvas *cbeta2 = NULL;
  g = gROOT->FindObject("cbeta2") ;
  if(g) cbeta2 = (TCanvas*)g;
  cbeta2 = new TCanvas("cbeta2","Cumulate Hits", 5, 5,800,700);
  cbeta2->Clear();
  cbeta2->UseCurrentStyle();
  cbeta2->cd();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad2 = new TPad("pad2","",0.,0.,1.,0.965);
  pad2->Draw();
  pad2->Divide(3,3);
  pad2->cd(1);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetBottomMargin(0.20);
  gPad->SetRightMargin(0.15);
  h_Seed_SON->SetAxisRange(0.0,MaxSON,"X");
  h_Seed_SON_TimeSpaceCut->SetAxisRange(0.0,MaxSON,"X");
  h_Seed_SON->Draw();
  h_Seed_SON_TimeSpaceCut->Draw("same");
  lv_SON->Draw();
  pad2->cd(2);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetBottomMargin(0.20);
  h_Seed_SOAveN->SetAxisRange(0.0,MaxSON,"X");
  h_Seed_SOAveN_TimeSpaceCut->SetAxisRange(0.0,MaxSON,"X");
  h_Seed_SOAveN->Draw();
  h_Seed_SOAveN_TimeSpaceCut->Draw("same");
  lv_SOAveN->Draw();
  pad2->cd(3);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetBottomMargin(0.20);
  h_EfficTimeSpaceCut_vs_SOAveN->SetAxisRange(0.0,MaxSON,"X");
  h_EfficTimeSpaceCut_vs_SOAveN->SetStats(false);
  h_EfficTimeSpaceCut_vs_SOAveN->Draw();
  pad2->cd(4);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetBottomMargin(0.20);
  gPad->SetLogz(1);
  h_Seed_SOAveN_vs_Max3NeighboursCharge->SetAxisRange(0.0,MaxSON,"X");
  h_Seed_SOAveN_vs_Max3NeighboursCharge->SetAxisRange(-0.5*MaxSON*AveNoise,3.0*MaxSON*AveNoise,"Y");
  h_Seed_SOAveN_vs_Max3NeighboursCharge->Draw("colz");
  pad2->cd(7);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetBottomMargin(0.20);
  gPad->SetLogz(1);
  h_Seed_SOAveN_vs_Max3NeighboursCharge_TimeSpaceCut->SetAxisRange(0.0,MaxSON,"X");
  h_Seed_SOAveN_vs_Max3NeighboursCharge_TimeSpaceCut->SetAxisRange(-0.5*MaxSON*AveNoise,3.0*MaxSON*AveNoise,"Y");
  h_Seed_SOAveN_vs_Max3NeighboursCharge_TimeSpaceCut->Draw("colz");
  pad2->cd(5);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetBottomMargin(0.20);
  gPad->SetLogy(1);
  h_Seed_Charge_TimeSpaceCut->SetAxisRange(0.0,2.0*MaxSON*AveNoise,"X");
  h_Seed_Charge_TimeSpaceCut->Draw();
  pad2->cd(6);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetBottomMargin(0.20);
  gPad->SetLogy(1);
  h_ClusterCharge_TimeSpaceCut->SetAxisRange(0.0,4.0*MaxSON*AveNoise,"X");
  h_ClusterCharge_TimeSpaceCut->Draw();
  cbeta2->Update();

  TCanvas *cbeta3 = NULL;
  g = gROOT->FindObject("cbeta3") ;
  if(g) cbeta3 = (TCanvas*)g;
  cbeta3 = new TCanvas("cbeta3","Cumulate Hits", 5, 5,800,700);
  cbeta3->Clear();
  cbeta3->UseCurrentStyle();
  cbeta3->cd();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad3 = new TPad("pad3","",0.,0.,1.,0.965);
  pad3->SetFillColor(10);
  pad3->SetFrameFillColor(10);
  pad3->SetTickx(1);
  pad3->SetTicky(1);
  pad3->SetBottomMargin(0.20);
  pad3->Draw();
  pad3->cd();
  TF1* f_landau = new TF1("f_landau","[2]*TMath::Landau(x,[0],[1])",SAveN_cutFit,MaxSON);
  f_landau->SetLineColor(2);
  f_landau->SetParName(0,"MPV");
  f_landau->SetParName(1,"#sigma");
  f_landau->SetParName(2,"Constant");
  TF1* f_landau_ext = new TF1("f_landau_ext","[2]*TMath::Landau(x,[0],[1])",h_Seed_SOAveN_TimeSpaceCut_v2->GetXaxis()->GetXmin(),MaxSON);
  f_landau_ext->SetLineColor(2);
  f_landau_ext->SetLineStyle(2);
  f_landau_ext->SetParName(0,"MPV");
  f_landau_ext->SetParName(1,"#sigma");
  f_landau_ext->SetParName(2,"Constant");
  double Init_MPV   = -999.0;
  double Maximum = -1.0e+20;
  double Init_Sigma = 0.20*(h_Seed_SOAveN_TimeSpaceCut_v2->GetXaxis()->GetXmax() - SAveN_cutFit);
  for(int i=0;i<h_Seed_SOAveN_TimeSpaceCut_v2->GetXaxis()->GetNbins();i++) {
    double c,v;
    v = h_Seed_SOAveN_TimeSpaceCut_v2->GetBinContent(i+1);
    c = h_Seed_SOAveN_TimeSpaceCut_v2->GetBinCenter(i+1);
    if(c < SAveN_cutFit) continue;
    if(Maximum < v) {
      Maximum  = v;
      Init_MPV = c;
    }
  }

  double l_pos = -999.0;
  double r_pos = -999.0;
  for(int i=0;i<h_Seed_SOAveN_TimeSpaceCut_v2->GetXaxis()->GetNbins()-1;i++) {
    double c = h_Seed_SOAveN_TimeSpaceCut_v2->GetBinCenter(i+1);
    if(c < SAveN_cutFit) continue;

    int index = h_Seed_SOAveN_TimeSpaceCut_v2->GetXaxis()->GetNbins() - (i+1);
    int idx_lr_1 = i+1;
    int idx_lr_2 = i+2;
    int idx_rl_1 = index+1;
    int idx_rl_2 = index;

    if(h_Seed_SOAveN_TimeSpaceCut_v2->GetBinContent(idx_lr_1) <= 0.5*Maximum &&
       h_Seed_SOAveN_TimeSpaceCut_v2->GetBinContent(idx_lr_2) >  0.5*Maximum &&
       l_pos == -999.0) {
      double x1 = h_Seed_SOAveN_TimeSpaceCut_v2->GetBinCenter(idx_lr_1);
      double x2 = h_Seed_SOAveN_TimeSpaceCut_v2->GetBinCenter(idx_lr_2);
      double y1 = h_Seed_SOAveN_TimeSpaceCut_v2->GetBinContent(idx_lr_1);
      double y2 = h_Seed_SOAveN_TimeSpaceCut_v2->GetBinContent(idx_lr_2);
      double a  = (y2-y1)/(x2-x1);
      double b  = y2 - a*x2;
      if(TMath::Abs(a) > 1.0e-6) l_pos = (0.5*Maximum - b)/a;
      else                       l_pos = 999.0;
    }

    if(h_Seed_SOAveN_TimeSpaceCut_v2->GetBinContent(idx_rl_1) <= 0.5*Maximum &&
       h_Seed_SOAveN_TimeSpaceCut_v2->GetBinContent(idx_rl_2) >  0.5*Maximum &&
       r_pos == -999.0) {
      double x1 = h_Seed_SOAveN_TimeSpaceCut_v2->GetBinCenter(idx_rl_1);
      double x2 = h_Seed_SOAveN_TimeSpaceCut_v2->GetBinCenter(idx_rl_2);
      double y1 = h_Seed_SOAveN_TimeSpaceCut_v2->GetBinContent(idx_rl_1);
      double y2 = h_Seed_SOAveN_TimeSpaceCut_v2->GetBinContent(idx_rl_2);
      double a  = (y2-y1)/(x2-x1);
      double b  = y2 - a*x2;
      if(TMath::Abs(a) > 1.0e-6) r_pos = (0.5*Maximum - b)/a;
      else                       r_pos = 999.0;
    }
  }
  if(l_pos == 999.0 || r_pos == 999.0) {
    l_pos = 0.0;
    r_pos = 2.0*h_Seed_SOAveN_TimeSpaceCut_v2->GetRMS();
  }
  Init_Sigma = 0.5*TMath::Abs(r_pos-l_pos);

  f_landau->SetParameter(0,Init_MPV);
  f_landau->SetParameter(1,Init_Sigma);
  f_landau->SetParameter(2,Maximum);
  f_landau->SetParLimits(0,SAveN_cutFit,h_Seed_SOAveN_TimeSpaceCut_v2->GetXaxis()->GetXmax());
  f_landau->SetParLimits(1,0.0,10.0*Init_Sigma);
  f_landau->SetParLimits(2,0.01*Maximum,10.0*Maximum);
  h_Seed_SOAveN_TimeSpaceCut_v2->Fit(f_landau,"R");
  f_landau_ext->SetParameter(0,f_landau->GetParameter(0));
  f_landau_ext->SetParameter(1,f_landau->GetParameter(1));
  f_landau_ext->SetParameter(2,f_landau->GetParameter(2));
  h_Seed_SOAveN_TimeSpaceCut_v2->SetAxisRange(0.0,MaxSON,"X");
  h_Seed_SOAveN_TimeSpaceCut_v2->Draw();
  f_landau_ext->Draw("same");
  lv_SOAveN_TimeSpaceCut_v2->Draw();
  cbeta3->Update();

  TCanvas *cbeta4 = NULL;
  g = gROOT->FindObject("cbeta4") ;
  if(g) cbeta4 = (TCanvas*)g;
  cbeta4 = new TCanvas("cbeta4","Cumulate Hits", 5, 5,800,700);
  cbeta4->Clear();
  cbeta4->UseCurrentStyle();
  cbeta4->cd();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad4 = new TPad("pad4","",0.,0.,1.,0.965);
  pad4->SetFillColor(10);
  pad4->SetFrameFillColor(10);
  pad4->SetTickx(1);
  pad4->SetTicky(1);
  pad4->SetBottomMargin(0.20);
  pad4->Draw();
  pad4->cd();

  int Nevts_above_cut = 0;
  for(int i=0;i<h_Seed_SOAveN_TimeSpaceCut_v2->GetXaxis()->GetNbins();i++) {
    double c = h_Seed_SOAveN_TimeSpaceCut_v2->GetBinCenter(i+1);
    if(c > SAveN_cutFit) Nevts_above_cut += h_Seed_SOAveN_TimeSpaceCut_v2->GetBinContent(i+1);
  }
  double Integral_above_cut = f_landau_ext->Integral(SAveN_cutFit,h_Seed_SOAveN_TimeSpaceCut_v2->GetXaxis()->GetXmax());
  double Integral_all       = f_landau_ext->Integral(h_Seed_SOAveN_TimeSpaceCut_v2->GetXaxis()->GetXmin(),h_Seed_SOAveN_TimeSpaceCut_v2->GetXaxis()->GetXmax());
  double Nevts_all          = Nevts_above_cut*(Integral_all/Integral_above_cut);

  //cout << "Integral_above_cut = " << Integral_above_cut << endl;
  //cout << "Nevts_above_cut    = " << Nevts_above_cut    << endl;
  //cout << "Integral_all       = " << Integral_all       << endl;
  //cout << "Nevts_all          = " << Nevts_all          << endl;

  double R_Ineffic_MaxMin[2];
  R_Ineffic_MaxMin[0] = +1.0e+20;
  R_Ineffic_MaxMin[1] = -1.0e+20;
  for(int i=0;i<h_Ineffic_vs_SeedSON_cut->GetXaxis()->GetNbins();i++) {
    double c = h_Ineffic_vs_SeedSON_cut->GetBinCenter(i+1);
    double Num  = f_landau_ext->Integral(c,h_Seed_SOAveN_TimeSpaceCut_v2->GetXaxis()->GetXmax());
    Num        *= (Nevts_above_cut/Integral_above_cut);

    double effic,effic_err;
    if(Num > 25.0 && Nevts_all > 25) {
      effic     = Num/Nevts_all;
      effic_err = sqrt(effic*(1.0 - effic)/Nevts_all);
      effic     = 1.0 - effic;
    }
    else {
      effic     = -1.0;
      effic_err =  1.0e-10;
    }

    effic     *= 100.0;
    effic_err *= 100.0;

    if(effic > -100.0) {
      if(R_Ineffic_MaxMin[0] > effic - effic_err)  R_Ineffic_MaxMin[0] = effic - effic_err;
      if(R_Ineffic_MaxMin[1] < effic + effic_err)  R_Ineffic_MaxMin[1] = effic + effic_err;
    }

    //cout << "i = " << i+1 << " c = " << c << ", Num =  " << Num << ", ineffic = " << effic << " +/- " << effic_err << " %" << endl;

    h_Ineffic_vs_SeedSON_cut->SetBinContent(i+1,effic);
    h_Ineffic_vs_SeedSON_cut->SetBinError(i+1,effic_err);
  }
  if(R_Ineffic_MaxMin[0] > 100.0 && R_Ineffic_MaxMin[1] < 100.0) {
    R_Ineffic_MaxMin[0] =   0.0;
    R_Ineffic_MaxMin[1] = 100.0;
  }
  else {
    porcent = 0.10;
    delta   = R_Ineffic_MaxMin[1] - R_Ineffic_MaxMin[0];
    R_Ineffic_MaxMin[0] -= porcent*delta;
    R_Ineffic_MaxMin[1] += porcent*delta;
    if(R_Ineffic_MaxMin[0] < -50.0) R_Ineffic_MaxMin[0] = 0.0;
  }
  h_Ineffic_vs_SeedSON_cut->SetMinimum(R_Ineffic_MaxMin[0]);
  h_Ineffic_vs_SeedSON_cut->SetMaximum(R_Ineffic_MaxMin[1]);
  h_Ineffic_vs_SeedSON_cut->SetStats(false);
  h_Ineffic_vs_SeedSON_cut->Draw();

  TLine* l_Ineffic_h = new TLine(h_Ineffic_vs_SeedSON_cut->GetXaxis()->GetXmin(),0.0,h_Ineffic_vs_SeedSON_cut->GetXaxis()->GetXmax(),0.0);
  l_Ineffic_h->SetLineColor(1);
  l_Ineffic_h->SetLineWidth(1);
  l_Ineffic_h->SetLineStyle(2);
  l_Ineffic_h->Draw();

  double Pos_Ineffic_Threshold = -999.0;
  if(h_Ineffic_vs_SeedSON_cut->GetBinContent(1)                                                < Ineffic_Threshold &&
     h_Ineffic_vs_SeedSON_cut->GetBinContent(h_Ineffic_vs_SeedSON_cut->GetXaxis()->GetNbins()) > Ineffic_Threshold) {

    for(int i=0;i<h_Ineffic_vs_SeedSON_cut->GetXaxis()->GetNbins()-1;i++) {
      double y1 = h_Ineffic_vs_SeedSON_cut->GetBinContent(i+1);
      double y2 = h_Ineffic_vs_SeedSON_cut->GetBinContent(i+2);

      if(y1 <= Ineffic_Threshold && y2 > Ineffic_Threshold) {
	double x1 = h_Ineffic_vs_SeedSON_cut->GetBinCenter(i+1);
        double x2 = h_Ineffic_vs_SeedSON_cut->GetBinCenter(i+2);
	double a  = (y2-y1)/(x2-x1);
	double b  = y2 - a*x2;
	if(TMath::Abs(a) < 1.0e-6) Pos_Ineffic_Threshold = 0.5*(x1+x2);
	else                       Pos_Ineffic_Threshold = (Ineffic_Threshold - b)/a;
      }
    }
  }
  TLine* l_Ineffic_thrPos1 = new TLine(h_Ineffic_vs_SeedSON_cut->GetXaxis()->GetXmin(),Ineffic_Threshold,Pos_Ineffic_Threshold,Ineffic_Threshold);
  l_Ineffic_thrPos1->SetLineColor(kRed);
  l_Ineffic_thrPos1->SetLineWidth(2);
  l_Ineffic_thrPos1->SetLineStyle(2);
  TLine* l_Ineffic_thrPos2 = new TLine(Pos_Ineffic_Threshold,h_Ineffic_vs_SeedSON_cut->GetMinimum(),Pos_Ineffic_Threshold,Ineffic_Threshold);
  l_Ineffic_thrPos2->SetLineColor(kRed);
  l_Ineffic_thrPos2->SetLineWidth(2);
  l_Ineffic_thrPos2->SetLineStyle(2);

  if(Pos_Ineffic_Threshold > -999.0) {
    l_Ineffic_thrPos1->Draw();
    l_Ineffic_thrPos2->Draw();
  }

  cbeta4->Update();

  TCanvas *cbeta5 = NULL;
  g = gROOT->FindObject("cbeta5") ;
  if(g) cbeta5 = (TCanvas*)g;
  cbeta5 = new TCanvas("cbeta5","Cumulate Hits", 5, 5,800,700);
  cbeta5->Clear();
  cbeta5->UseCurrentStyle();
  cbeta5->cd();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad5 = new TPad("pad5","",0.,0.,1.,0.965);
  pad5->Draw();
  pad5->Divide(3,3);
  for(int i=0;i<NSeedSONCuts;i++) {
    pad5->cd(i+1);
    gPad->SetFillColor(10);
    gPad->SetFrameFillColor(10);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    gPad->SetBottomMargin(0.20);
    gPad->SetRightMargin(0.15);
    h_ClusterPseudoMult[i]->Draw();
  }
  cbeta5->Update();


  // Save canvas and histos
  //cd to result dir
  Char_t rootFile[300];
  sprintf(rootFile,"%sBetaAnalysis_run%d.root",fSession->GetResultDirName().Data(),fSession->GetRunNumber());
  sprintf(rootFile,"%s", fTool.LocalizeDirName( rootFile)); // JB 2011/07/07
  cout << "\n-- Saving histos and canvas into " << rootFile << endl;
  TFile fRoot(rootFile,"RECREATE");
  cbeta1->Write();
  cbeta2->Write();
  cbeta3->Write();
  cbeta4->Write();
  cbeta5->Write();

  h_HitMap_vs_ColRow->Write();
  h_HitMap_vs_TS->Write();
  h_NHitsEvent->Write();
  h_MinDistEvent->Write();
  h_Seed_SON->Write();
  h_Seed_SON_TimeSpaceCut->Write();
  h_Seed_SOAveN->Write();
  h_Seed_SOAveN_TimeSpaceCut_v2->Write();
  h_EfficTimeSpaceCut_vs_SOAveN->Write();
  h_Seed_SOAveN_vs_Max3NeighboursCharge->Write();
  h_Seed_SOAveN_vs_Max3NeighboursCharge_TimeSpaceCut->Write();
  h_Ineffic_vs_SeedSON_cut->Write();
  h_Seed_Charge_TimeSpaceCut->Write();
  h_ClusterCharge_TimeSpaceCut->Write();
  for(int i=0;i<NSeedSONCuts;i++) h_ClusterPseudoMult[i]->Write();
  fRoot.Close();

  return;

}
//______________________________________________________________________________
//
Int_t MRaw::GetFileNumber()
{
  // Returns the last DSF file number created or 0 if no file.
  // Try 100 file numbers before stopping.
  // Modified: JB 2011/07/07 to localize path names
  // Modified: JB 2012/11/21 to find files event if low numbers do no exist

  Char_t New_File_Name[1000];
  Int_t FileNumber = 1;
  Int_t missedFiles = 0;
  Int_t Returned_FileNumber = 0;
  int exist = 1;
  while(exist) {
    sprintf(New_File_Name,"%srun%d_0%d.root",(const char*)fSession->GetSummaryFilePath(),fSession->GetRunNumber(),FileNumber);
    sprintf(New_File_Name,"%s", fTool.LocalizeDirName( New_File_Name)); // JB 2011/07/07
    if (!gSystem->AccessPathName(New_File_Name)){ missedFiles=-1; FileNumber++; }
    else if( 0<=missedFiles && missedFiles<100 ) { missedFiles++; FileNumber++; }
    else {exist=0;}

  };

  // Check if the user asked for a specific number
  if(!fUserFileNumber) { Returned_FileNumber = FileNumber-1;}
  else { Returned_FileNumber=TMath::Min(FileNumber-1,fUserFileNumber);}

  // Check the file exists
  sprintf(New_File_Name,"%srun%d_0%d.root",(const char*)fSession->GetSummaryFilePath(),fSession->GetRunNumber(),Returned_FileNumber);
  sprintf(New_File_Name,"%s", fTool.LocalizeDirName( New_File_Name));
  if (gSystem->AccessPathName(New_File_Name)){
    //Info( "GetFileNumber", "WARNING: file %s does not exist!", New_File_Name);
    Returned_FileNumber=0;
  }

  return Returned_FileNumber;
}
//______________________________________________________________________________
//
void MRaw::DisplayBeast()
{
  TH2F *h_2DUVPosLadder1Module1;
  TH2F *h_2DUVPosLadder1Module2;
  TH2F *h_2DUVPosLadder2Module1;
  TH2F *h_2DUVPosLadder2Module2;
  double Pitch; //mm
  double NCols;
  double NRows ;
  double Surface; //cm2
  int NSensors;
  double TheSize;
  float _NoiseOccupancy;

  TheSize = 0.05;
  Pitch = 18.4*1.0e-3; //mm
  NCols = 1152;
  NRows =  576;
  float DClearance = 500*1.0e-3; //mm
  float WSensor = NCols*Pitch; //mm
  float HSensor = NRows*Pitch; //mm
  //Histogram definition of 2D Position of U and V in Ladder referentiel
  // int Nbins_LadderU = NSensors*NCols;
  // int Nbins_LadderV = NSensors*NRows;
  int Nbins_LadderU = 100;
  int Nbins_LadderV = 100;
  double R_LadderU[2];
  // R_LadderU[0] = -(WSensor*3+DClearance*2.5)*1.05;
  R_LadderU[0] =-125.0;
  // R_LadderU[1] = (WSensor*3+DClearance*2.5)*1.05;
  R_LadderU[1] = 15.;
  // R_LadderU[1] = 0.;
  double R_LadderV[2];
  // R_LadderV[0] = -HSensor*0.5*1.05;
  R_LadderV[0] = -6.;
  // R_LadderV[1] = HSensor*0.5*1.05;
  R_LadderV[1] = 6.;

  h_2DUVPosLadder1Module1 = new TH2F("h_2DUVPosLadder1Module1",
             "U Vs V position in Ladder 1 Module 1",
             Nbins_LadderU,R_LadderU[0],R_LadderU[1],
             Nbins_LadderV,R_LadderV[0],R_LadderV[1]);
  h_2DUVPosLadder1Module1->SetXTitle("U_{Ladder} (mm)");
  h_2DUVPosLadder1Module1->GetXaxis()->CenterTitle(true);
  h_2DUVPosLadder1Module1->SetYTitle("V_{Ladder} (mm)");
  h_2DUVPosLadder1Module1->GetYaxis()->CenterTitle(true);
  h_2DUVPosLadder1Module1->SetZTitle("Sim Hits");
  h_2DUVPosLadder1Module1->GetZaxis()->CenterTitle(true);
  h_2DUVPosLadder1Module1->SetLineColor(4);
  h_2DUVPosLadder1Module1->SetLineWidth(2);
  h_2DUVPosLadder1Module1->SetMinimum(1.0e-1);
  h_2DUVPosLadder1Module1->GetXaxis()->SetTitleSize(TheSize);
  h_2DUVPosLadder1Module1->GetXaxis()->SetLabelSize(TheSize);
  h_2DUVPosLadder1Module1->GetYaxis()->SetTitleSize(TheSize);
  h_2DUVPosLadder1Module1->GetYaxis()->SetLabelSize(TheSize);
  h_2DUVPosLadder1Module1->GetZaxis()->SetTitleSize(TheSize);
  h_2DUVPosLadder1Module1->GetZaxis()->SetLabelSize(TheSize);
  // h_2DUVPosLadder1Module1->SetStats(false);
  h_2DUVPosLadder1Module1->SetMarkerStyle(5);
  // h_2DUVPosLadder1Module1->SetMarkerSize(5);

  h_2DUVPosLadder2Module1 = new TH2F("h_2DUVPosLadder2Module1",
             "U Vs V position in Ladder 2 Module 1",
             Nbins_LadderU,R_LadderU[0],R_LadderU[1],
             Nbins_LadderV,R_LadderV[0],R_LadderV[1]);
  h_2DUVPosLadder2Module1->SetXTitle("U_{Ladder} (mm)");
  h_2DUVPosLadder2Module1->GetXaxis()->CenterTitle(true);
  h_2DUVPosLadder2Module1->SetYTitle("V_{Ladder} (mm)");
  h_2DUVPosLadder2Module1->GetYaxis()->CenterTitle(true);
  h_2DUVPosLadder2Module1->SetZTitle("Sim Hits");
  h_2DUVPosLadder2Module1->GetZaxis()->CenterTitle(true);
  h_2DUVPosLadder2Module1->SetLineColor(4);
  h_2DUVPosLadder2Module1->SetLineWidth(2);
  h_2DUVPosLadder2Module1->SetMinimum(1.0e-1);
  h_2DUVPosLadder2Module1->GetXaxis()->SetTitleSize(TheSize);
  h_2DUVPosLadder2Module1->GetXaxis()->SetLabelSize(TheSize);
  h_2DUVPosLadder2Module1->GetYaxis()->SetTitleSize(TheSize);
  h_2DUVPosLadder2Module1->GetYaxis()->SetLabelSize(TheSize);
  h_2DUVPosLadder2Module1->GetZaxis()->SetTitleSize(TheSize);
  h_2DUVPosLadder2Module1->GetZaxis()->SetLabelSize(TheSize);
  // h_2DUVPosLadder2Module1->SetStats(false);
  h_2DUVPosLadder2Module1->SetMarkerStyle(5);
 //  h_2DUVPosLadder2Module1->SetMarkerSize(5);

  h_2DUVPosLadder1Module2 = new TH2F("h_2DUVPosLadder1Module2",
             "U Vs V position in Ladder 1 Module 2",
             Nbins_LadderU,R_LadderU[0],R_LadderU[1],
             Nbins_LadderV,R_LadderV[0],R_LadderV[1]);
  h_2DUVPosLadder1Module2->SetXTitle("U_{Ladder} (mm)");
  h_2DUVPosLadder1Module2->GetXaxis()->CenterTitle(true);
  h_2DUVPosLadder1Module2->SetYTitle("V_{Ladder} (mm)");
  h_2DUVPosLadder1Module2->GetYaxis()->CenterTitle(true);
  h_2DUVPosLadder1Module2->SetZTitle("Sim Hits");
  h_2DUVPosLadder1Module2->GetZaxis()->CenterTitle(true);
  h_2DUVPosLadder1Module2->SetLineColor(4);
  h_2DUVPosLadder1Module2->SetLineWidth(2);
  h_2DUVPosLadder1Module2->SetMinimum(1.0e-1);
  h_2DUVPosLadder1Module2->GetXaxis()->SetTitleSize(TheSize);
  h_2DUVPosLadder1Module2->GetXaxis()->SetLabelSize(TheSize);
  h_2DUVPosLadder1Module2->GetYaxis()->SetTitleSize(TheSize);
  h_2DUVPosLadder1Module2->GetYaxis()->SetLabelSize(TheSize);
  h_2DUVPosLadder1Module2->GetZaxis()->SetTitleSize(TheSize);
  h_2DUVPosLadder1Module2->GetZaxis()->SetLabelSize(TheSize);
  // h_2DUVPosLadder1Module2->SetStats(false);
  h_2DUVPosLadder1Module2->SetMarkerStyle(5);
  // h_2DUVPosLadder1Module2->SetMarkerSize(5);

  h_2DUVPosLadder2Module2 = new TH2F("h_2DUVPosLadder2Module2",
             "U Vs V position in Ladder 2 Module 2",
             Nbins_LadderU,R_LadderU[0],R_LadderU[1],
             Nbins_LadderV,R_LadderV[0],R_LadderV[1]);
  h_2DUVPosLadder2Module2->SetXTitle("U_{Ladder} (mm)");
  h_2DUVPosLadder2Module2->GetXaxis()->CenterTitle(true);
  h_2DUVPosLadder2Module2->SetYTitle("V_{Ladder} (mm)");
  h_2DUVPosLadder2Module2->GetYaxis()->CenterTitle(true);
  h_2DUVPosLadder2Module2->SetZTitle("Sim Hits");
  h_2DUVPosLadder2Module2->GetZaxis()->CenterTitle(true);
  h_2DUVPosLadder2Module2->SetLineColor(4);
  h_2DUVPosLadder2Module2->SetLineWidth(2);
  h_2DUVPosLadder2Module2->SetMinimum(1.0e-1);
  h_2DUVPosLadder2Module2->GetXaxis()->SetTitleSize(TheSize);
  h_2DUVPosLadder2Module2->GetXaxis()->SetLabelSize(TheSize);
  h_2DUVPosLadder2Module2->GetYaxis()->SetTitleSize(TheSize);
  h_2DUVPosLadder2Module2->GetYaxis()->SetLabelSize(TheSize);
  h_2DUVPosLadder2Module2->GetZaxis()->SetTitleSize(TheSize);
  h_2DUVPosLadder2Module2->GetZaxis()->SetLabelSize(TheSize);
  // h_2DUVPosLadder2Module2->SetStats(false);
  h_2DUVPosLadder2Module2->SetMarkerStyle(5);
  // h_2DUVPosLadder2Module2->SetMarkerSize(5);

  int PlaneIdx;
  int LadderNumber;



  //////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////

  fSession->SetEvents(50000);
  DTracker *tTracker     = fSession->GetTracker();

  DHit*   aHit      = NULL;
  DPlane *aPlane = NULL;
  DLadder *aLadder = NULL;

  // Float_t tPlaneResolution=fc->GetTrackerPar().Resolution;
  DR3 *hitPositionPlane; // usefull for temporary computation check
  DR3 hitPositionLadder;
  DR3 hitPositionLab;
  LadderNumber=0;



  int nHitsEvt=0;
  double RLabFrame;
  DPrecAlign *Dprec;


    DBeaster *aBeaster;
    aBeaster=tTracker->GetBeaster();
    aBeaster->ListOfBeastRecoParts_L1.clear();
    aBeaster->ListOfBeastRecoParts_L2.clear();
    fSession->NextRawEvent();
    tTracker->Update();

///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
DEventMC* MCInfoHolder = tTracker->GetMCInfoHolder();


///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
    std::cout << "Beaster RecoParts On L1 = " << aBeaster->ListOfBeastRecoParts_L1.size()<<'\n';
    std::cout << "Beaster RecoParts On L2 = " << aBeaster->ListOfBeastRecoParts_L2.size()<<'\n';
    Int_t nPlanes = fSession->GetTracker()->GetPlanesN();
    for( int iPlane=0; iPlane< nPlanes; iPlane++ ) { // loop on planes

      aPlane=tTracker->GetPlane(iPlane+1);

      // std::cout << "******/////******/////** Hitsn = " << aPlane->GetHitsN()<<'\n';
      if(aPlane->GetHitsN()==0) continue;
        for( Int_t iHit=1; iHit <= aPlane->GetHitsN(); iHit++ ) { // loop on plane hits

          aHit = aPlane->GetHit(iHit);
          if(aHit->GetStripsInCluster()<1) continue;
          if(aHit->GetMCHitID()<0) continue;
          LadderNumber=aPlane->GetLadderNumber();
          hitPositionPlane=aHit->GetPosition();
          Dprec=aPlane->GetPrecAlignment();
          hitPositionLab=Dprec->TransformHitToTracker(*hitPositionPlane);
          aLadder=tTracker->GetLadder(LadderNumber);

            if (LadderNumber==1) {
              PlaneIdx=iPlane+1;
              hitPositionLadder=aLadder->PlaneToLadder(*hitPositionPlane,PlaneIdx);
              if (iPlane+1<7 ){
                 h_2DUVPosLadder1Module1->Fill((hitPositionLadder)(0)/1000,(hitPositionLadder)(1)/1000);
              }else{
                 h_2DUVPosLadder1Module2->Fill((hitPositionLadder)(0)/1000,(hitPositionLadder)(1)/1000);
              }
            }else{
              PlaneIdx=iPlane+1-12;
              hitPositionLadder=aLadder->PlaneToLadder(*hitPositionPlane,PlaneIdx);
              if (iPlane+1<19){
                 h_2DUVPosLadder2Module1->Fill((hitPositionLadder)(0)/1000,(hitPositionLadder)(1)/1000);
              }else{
                 h_2DUVPosLadder2Module2->Fill((hitPositionLadder)(0)/1000,(hitPositionLadder)(1)/1000);
               }
            }
        } // end loop on plane hits
    } // end loop on planes

    TCanvas *cMonitor = new TCanvas("cMonitor","cMonitor");
    cMonitor->Print("Results/26999/Plots/Monitoring.pdf[");
    cMonitor->Clear();
    cMonitor->Divide(1,2);
    cMonitor->cd(1);
    h_2DUVPosLadder1Module1->Draw("COL Z");
    cMonitor->cd(2);
    h_2DUVPosLadder1Module2->Draw("COL Z");
    cMonitor->Print("Results/26999/Plots/Monitoring.pdf");
    cMonitor->Clear();
    cMonitor->Divide(1,2);
    cMonitor->cd(1);
    h_2DUVPosLadder2Module1->Draw("COL Z");
    cMonitor->cd(2);
    h_2DUVPosLadder2Module2->Draw("COL Z");
    cMonitor->Print("Results/26999/Plots/Monitoring.pdf");
    cMonitor->Print("Results/26999/Plots/Monitoring.pdf]");

      h_2DUVPosLadder1Module1->Reset();
      h_2DUVPosLadder1Module2->Reset();
      h_2DUVPosLadder2Module1->Reset();
      h_2DUVPosLadder2Module2->Reset();

 return;
}
//______________________________________________________________________________
//

void MRaw::DisplayCumulatedBeast(int MaxEvt)
{


  TH1F *h_NbPartvsFrame;
  TH1F *h_NbHitvsFrame;
  TH1F *h_HitOnSensor;
  TH1F *h_PixelOnSensor;
  TH1F *h_NbPixelsVsHit;
  TH1F* h_Occupancies;
  TH1F* h_Rates;
  TH1F* h_BkgType;
  TH2F* h_VtxZR;
  TH2F* h_VtxXY;
  TH1F *PartCat_Ladder2;
  TH1F *PartCat_Ladder1;
  TH2F* h_HitPos_L1;
  TH2F* h_HitPos_L2;
  TH2F *h_hitOnSensorFrame;
  TH2F *h_hitOnLadderFrame;
  TH2F *h_hitOnLabFrameZY;
  TH2F *h_hitOnLabFrameZX;
  TH2F *h_hitOnLabFrameZR;
  TH2F *h_hitOnLabFrameXY;
  TH2F *h_HitPosOnSensor[24];
  TH2F *h_2DUVPosLadder1Module1;
  TH2F *h_2DUVPosLadder1Module2;
  TH2F *h_2DUVPosLadder2Module1;
  TH2F *h_2DUVPosLadder2Module2;
  TH1F *h_BeastRecoPartClassType_L1;
  TH1F *h_BeastRecoPartClassType_L2;
  float HitOnSensor[25];
  float PixelOnSensor[25];
  float MCHitOnSensor[25];
  float MCPixelOnSensor[25];
  bool IsDef=false;


    double Pitch; //mm
    double NCols;
    double NRows ;
    double Surface; //cm2
    int NSensors;
    double TheSize;
    float _NoiseOccupancy;
    int AlejandroHitsCumulated;
    int FrameNb;
    int nHitsAllEvt;

        _NoiseOccupancy  = 1.0e-6;
       Pitch = 18.4*1.0e-3; //mm
       NCols = 1152;
       NRows =  576;
       Surface = NCols*NRows*pow(Pitch,2)*1.0e-2; //cm2
       NSensors=24;
       TheSize = 0.03;

      PartCat_Ladder1 = new TH1F("Particle Categorie Ladder 1","Particle Pattern Ladder 1",4,1.,5.);
      //PartCat_Ladder1->SetXTitle("Pattern");
      PartCat_Ladder1->GetXaxis()->CenterTitle(true);
      PartCat_Ladder1->SetYTitle("");
      PartCat_Ladder1->GetYaxis()->CenterTitle(true);
      PartCat_Ladder1->SetLineColor(4);
      PartCat_Ladder1->SetLineWidth(2);
      PartCat_Ladder1->GetYaxis()->SetTitleSize(TheSize);
      PartCat_Ladder1->GetYaxis()->SetLabelSize(TheSize);
      PartCat_Ladder1->GetXaxis()->SetTitleSize(TheSize);
      PartCat_Ladder1->GetXaxis()->SetLabelSize(TheSize);
      // PartCat_Ladder1->SetStats(false);

      PartCat_Ladder2 = new TH1F("Particle Categorie Ladder 2","Particle Pattern Ladder 2",4,1.,5.);
      //PartCat_Ladder2->SetXTitle("Pattern");
      PartCat_Ladder2->GetXaxis()->CenterTitle(true);
      PartCat_Ladder2->SetYTitle("");
      PartCat_Ladder2->GetYaxis()->CenterTitle(true);
      PartCat_Ladder2->SetLineColor(4);
      PartCat_Ladder2->SetLineWidth(2);
      PartCat_Ladder2->GetYaxis()->SetTitleSize(TheSize);
      const char *LabelNameCat[4]={"One Side ","Two Sides ","OSL","TSL"};
      for (size_t ilabel = 1; ilabel < 5; ilabel++) {
        PartCat_Ladder1->GetXaxis()->SetBinLabel(ilabel,LabelNameCat[ilabel-1]);
        PartCat_Ladder2->GetXaxis()->SetBinLabel(ilabel,LabelNameCat[ilabel-1]);
      }


      h_VtxZR= new TH2F("h_VtxZR","Vtx Prod RvsZ ",500,-600.,600.,500,0.,140.);
      h_VtxZR->SetXTitle("Z[mm]");
      h_VtxZR->GetXaxis()->CenterTitle(true);
      h_VtxZR->SetYTitle("R[mm]");
      h_VtxZR->GetYaxis()->CenterTitle(true);
      h_VtxZR->SetLineColor(4);
      h_VtxZR->SetLineWidth(2);
      h_VtxZR->GetYaxis()->SetTitleSize(TheSize);
      h_VtxZR->GetYaxis()->SetLabelSize(TheSize);
      h_VtxZR->GetXaxis()->SetTitleSize(TheSize);
      h_VtxZR->GetXaxis()->SetLabelSize(TheSize);
      h_VtxZR->SetStats(false);

      h_VtxXY= new TH2F("h_VtxXY","Vtx Prod YvsX ",500,-100.,100.,500,-100.,100.);
      h_VtxXY->SetXTitle("X[mm]");
      h_VtxXY->GetXaxis()->CenterTitle(true);
      h_VtxXY->SetYTitle("Y[mm]");
      h_VtxXY->GetYaxis()->CenterTitle(true);
      h_VtxXY->SetLineColor(4);
      h_VtxXY->SetLineWidth(2);
      h_VtxXY->GetYaxis()->SetTitleSize(TheSize);
      h_VtxXY->GetYaxis()->SetLabelSize(TheSize);
      h_VtxXY->GetXaxis()->SetTitleSize(TheSize);
      h_VtxXY->GetXaxis()->SetLabelSize(TheSize);
      h_VtxXY->SetStats(false);

      h_BkgType= new TH1F("h_BkgType","Bkg Type",9,1.,10.);
      //h_BkgType->SetXTitle("BkgType");
      h_BkgType->GetXaxis()->CenterTitle(true);
      h_BkgType->SetYTitle("");
      h_BkgType->GetYaxis()->CenterTitle(true);
      h_BkgType->SetLineColor(4);
      h_BkgType->SetLineWidth(2);
      h_BkgType->GetYaxis()->SetTitleSize(TheSize);
      h_BkgType->GetYaxis()->SetLabelSize(TheSize);
      h_BkgType->GetXaxis()->SetTitleSize(TheSize);
      h_BkgType->GetXaxis()->SetLabelSize(TheSize);
      // h_BkgType->SetStats(false);

      const char *LabelNameBkg[10]={"Touschek HER","Touschek LER","Beam Gas HER","Beam Gas LER","RBB HER","RBB LER","Synchrotron HER","Synchrotron LER","2-Photons"};
      for (size_t ilabel = 1; ilabel < 10; ilabel++) {
      h_BkgType->GetXaxis()->SetBinLabel(ilabel,LabelNameBkg[ilabel-1]);
      }
      h_NbHitvsFrame= new TH1F("h_NbHitvsFrame","Nb Hits par Frame",500,0.,50000.);
      h_NbHitvsFrame->SetXTitle("Frame Number");
      h_NbHitvsFrame->GetXaxis()->CenterTitle(true);
      h_NbHitvsFrame->SetYTitle("Hits");
      h_NbHitvsFrame->GetYaxis()->CenterTitle(true);
      h_NbHitvsFrame->SetLineColor(4);
      h_NbHitvsFrame->SetLineWidth(2);
      h_NbHitvsFrame->GetYaxis()->SetTitleSize(TheSize);
      h_NbHitvsFrame->GetYaxis()->SetLabelSize(TheSize);
      h_NbHitvsFrame->GetXaxis()->SetTitleSize(TheSize);
      h_NbHitvsFrame->GetXaxis()->SetLabelSize(TheSize);
      h_NbHitvsFrame->SetStats(false);

      h_NbPartvsFrame= new TH1F("h_NbPartvsFrame","Nb Particules par Frame",500,1.,50000.);
      h_NbPartvsFrame->SetXTitle("Frame Number");
      h_NbPartvsFrame->GetXaxis()->CenterTitle(true);
      h_NbPartvsFrame->SetYTitle("Particles");
      h_NbPartvsFrame->GetYaxis()->CenterTitle(true);
      h_NbPartvsFrame->SetLineColor(4);
      h_NbPartvsFrame->SetLineWidth(2);
      h_NbPartvsFrame->GetYaxis()->SetTitleSize(TheSize);
      h_NbPartvsFrame->GetYaxis()->SetLabelSize(TheSize);
      h_NbPartvsFrame->GetXaxis()->SetTitleSize(TheSize);
      h_NbPartvsFrame->GetXaxis()->SetLabelSize(TheSize);
      h_NbPartvsFrame->SetStats(false);

      h_NbPixelsVsHit= new TH1F("h_NbPixelsVsHit","Nb Pixels vs Hit",20,0.,0.);
      h_NbPixelsVsHit->SetXTitle("Pixel #");
      h_NbPixelsVsHit->GetXaxis()->CenterTitle(true);
      h_NbPixelsVsHit->SetYTitle("");
      h_NbPixelsVsHit->GetYaxis()->CenterTitle(true);
      h_NbPixelsVsHit->SetLineColor(4);
      h_NbPixelsVsHit->SetLineWidth(2);
      h_NbPixelsVsHit->GetYaxis()->SetTitleSize(TheSize);
      h_NbPixelsVsHit->GetYaxis()->SetLabelSize(TheSize);
      h_NbPixelsVsHit->GetXaxis()->SetTitleSize(TheSize);
      h_NbPixelsVsHit->GetXaxis()->SetLabelSize(TheSize);
      // h_NbPixelsVsHit->SetStats(false);

      h_Occupancies = new TH1F("h_Occupancies",
    			   "Occupancies on sensors",
    			   NSensors+1,-0.5,NSensors+1+0.5);
      h_Occupancies->SetXTitle("");
      h_Occupancies->GetXaxis()->CenterTitle(true);
      h_Occupancies->SetYTitle("Occupancy");
      h_Occupancies->GetYaxis()->CenterTitle(true);
      h_Occupancies->SetLineColor(4);
      h_Occupancies->SetLineWidth(2);
      h_Occupancies->GetYaxis()->SetTitleSize(TheSize);
      h_Occupancies->GetYaxis()->SetLabelSize(TheSize);
      h_Occupancies->GetXaxis()->SetTitleSize(TheSize);
      h_Occupancies->GetXaxis()->SetLabelSize(TheSize);
      h_Occupancies->SetStats(false);
      // h_Occupancies->SetMinimum(1.0e-7);
      // h_Occupancies->SetMaximum(1.0e-4);
      for(int iSensor=0;iSensor<NSensors+1;iSensor++) {
        TString Name;
        if(iSensor+1 <= NSensors) Name = TString("Sen-") + long(iSensor);
        else                      Name = TString("Ave.");
        h_Occupancies->GetXaxis()->SetBinLabel(iSensor+1,Name.Data());
      }

      h_Rates = new TH1F("h_Rates",
    		     "Rates on sensors",
    		     NSensors+1,-0.5,NSensors+1+0.5);
      h_Rates->SetTitleSize(0.01);
      h_Rates->SetXTitle("");
      h_Rates->GetXaxis()->CenterTitle(true);
      h_Rates->SetYTitle("");
      h_Rates->GetYaxis()->CenterTitle(true);
      h_Rates->SetLineColor(4);
      h_Rates->SetLineWidth(2);
      h_Rates->GetYaxis()->SetTitleSize(TheSize);
      h_Rates->GetYaxis()->SetLabelSize(TheSize);
      h_Rates->GetXaxis()->SetTitleSize(TheSize);
      h_Rates->GetXaxis()->SetLabelSize(TheSize);
      h_Rates->SetStats(false);
      for(int iSensor=0;iSensor<NSensors+1;iSensor++) {
        TString Name;
        if(iSensor+1 <= NSensors) Name = TString("Sen-") + long(iSensor);
        else                      Name = TString("Ave.");
        h_Rates->GetXaxis()->SetBinLabel(iSensor+1,Name.Data());
      }


      //Histo Hit Occupancies on different sensor
      TString HistName,HistTitle;
      h_HitOnSensor = new TH1F("h_HitOnSensor",
             "Hits on sensors",
             NSensors+1,-0.5,NSensors+1+0.5);
      h_HitOnSensor->SetXTitle("");
      h_HitOnSensor->GetXaxis()->CenterTitle(true);
      h_HitOnSensor->SetYTitle("#Hits");
      h_HitOnSensor->GetYaxis()->CenterTitle(true);
      h_HitOnSensor->SetLineColor(4);
      h_HitOnSensor->SetLineWidth(2);
      h_HitOnSensor->GetYaxis()->SetTitleSize(TheSize);
      h_HitOnSensor->GetYaxis()->SetLabelSize(TheSize);
      h_HitOnSensor->GetXaxis()->SetTitleSize(TheSize);
      h_HitOnSensor->GetXaxis()->SetLabelSize(TheSize);

      h_PixelOnSensor = new TH1F("h_PixelOnSensor",
             "Pixels on sensors",
             NSensors+1,-0.5,NSensors+1+0.5);
      h_PixelOnSensor->SetXTitle("");
      h_PixelOnSensor->GetXaxis()->CenterTitle(true);
      h_PixelOnSensor->SetYTitle("#Hits");
      h_PixelOnSensor->GetYaxis()->CenterTitle(true);
      h_PixelOnSensor->SetLineColor(4);
      h_PixelOnSensor->SetLineWidth(2);
      h_PixelOnSensor->GetYaxis()->SetTitleSize(TheSize);
      h_PixelOnSensor->GetYaxis()->SetLabelSize(TheSize);
      h_PixelOnSensor->GetXaxis()->SetTitleSize(TheSize);
      h_PixelOnSensor->GetXaxis()->SetLabelSize(TheSize);
      // h_PixelOnSensor->SetStats(false);
      // h_PixelOnSensor->SetMinimum(1.0e-7);
      // h_PixelOnSensor->SetMaximum(1.0e-4);
      for(int iSensor=0;iSensor<NSensors+1;iSensor++) {
        TString Name;
        if(iSensor+1 <= NSensors) Name = TString("Sen-") + long(iSensor);
        else                      Name = TString("Ave.");
        h_HitOnSensor->GetXaxis()->SetBinLabel(iSensor+1,Name.Data());
        h_PixelOnSensor->GetXaxis()->SetBinLabel(iSensor+1,Name.Data());
      }

        h_HitPos_L1=new TH2F("HitPos L1","HitPos L1",1000,0.,0.,1000,0.,0.);
        h_HitPos_L1->SetXTitle("U[mm]");
        h_HitPos_L1->GetXaxis()->CenterTitle(true);
        h_HitPos_L1->SetYTitle("V[mm]");
        h_HitPos_L1->GetYaxis()->CenterTitle(true);
        h_HitPos_L1->SetLineColor(4);
        h_HitPos_L1->SetLineWidth(2);
        h_HitPos_L1->GetXaxis()->SetTitleSize(TheSize);
        h_HitPos_L1->GetXaxis()->SetLabelSize(TheSize);
        h_HitPos_L1->GetYaxis()->SetTitleSize(TheSize);
        h_HitPos_L1->GetYaxis()->SetLabelSize(TheSize);
        // h_HitPos_L1->SetStats(false);
        h_HitPos_L2=new TH2F("HitPosL2","HitPosL2",1000,0.,0.,1000,0.,0.);
        h_HitPos_L2->SetXTitle("U[mm]");
        h_HitPos_L2->GetXaxis()->CenterTitle(true);
        h_HitPos_L2->SetYTitle("V[mm]");
        h_HitPos_L2->GetYaxis()->CenterTitle(true);
        h_HitPos_L2->SetLineColor(4);
        h_HitPos_L2->SetLineWidth(2);
        h_HitPos_L2->GetXaxis()->SetTitleSize(TheSize);
        h_HitPos_L2->GetXaxis()->SetLabelSize(TheSize);
        h_HitPos_L2->GetYaxis()->SetTitleSize(TheSize);
        h_HitPos_L2->GetYaxis()->SetLabelSize(TheSize);
        // h_HitPos_L2->SetStats(false);
        //Histo Hit Occupancies on different sensor
  ////////////////////////////////////////////////////////
  ////////  Histo For Cumulated Display /////////////////
  //////////////////////////////////////////////////////
      h_hitOnSensorFrame=new TH2F("h_hitOnSensorFrame","Sensor frame",200,-100.,100.,200,-100.,100.);
      h_hitOnSensorFrame->SetXTitle("U[mm]");
      h_hitOnSensorFrame->GetXaxis()->CenterTitle(true);
      h_hitOnSensorFrame->SetYTitle("V[mm]");
      h_hitOnSensorFrame->GetYaxis()->CenterTitle(true);
      h_hitOnSensorFrame->SetLineColor(4);
      h_hitOnSensorFrame->SetLineWidth(2);
      h_hitOnSensorFrame->GetXaxis()->SetTitleSize(TheSize);
      h_hitOnSensorFrame->GetXaxis()->SetLabelSize(TheSize);
      h_hitOnSensorFrame->GetYaxis()->SetTitleSize(TheSize);
      h_hitOnSensorFrame->GetYaxis()->SetLabelSize(TheSize);
      h_hitOnSensorFrame->SetStats(false);
      // h_hitOnSensorFrame;

      h_hitOnLadderFrame=new TH2F("h_hitOnLadderFrame","Ladder frame",200,-120.,20.,200,-50.,50.);
      h_hitOnLadderFrame->SetXTitle("U[mm]");
      h_hitOnLadderFrame->GetXaxis()->CenterTitle(true);
      h_hitOnLadderFrame->SetYTitle("V[mm]");
      h_hitOnLadderFrame->GetYaxis()->CenterTitle(true);
      h_hitOnLadderFrame->SetLineColor(4);
      h_hitOnLadderFrame->SetLineWidth(2);
      h_hitOnLadderFrame->GetXaxis()->SetTitleSize(TheSize);
      h_hitOnLadderFrame->GetXaxis()->SetLabelSize(TheSize);
      h_hitOnLadderFrame->GetYaxis()->SetTitleSize(TheSize);
      h_hitOnLadderFrame->GetYaxis()->SetLabelSize(TheSize);
      h_hitOnLadderFrame->SetStats(false);

      h_hitOnLabFrameZY=new TH2F("h_hitOnLabFrameZY","Lab frame",200,-100.,200.,200,-80.,60.);
      h_hitOnLabFrameZY->SetXTitle("Z[mm]");
      h_hitOnLabFrameZY->GetXaxis()->CenterTitle(true);
      h_hitOnLabFrameZY->SetYTitle("Y[mm]");
      h_hitOnLabFrameZY->GetYaxis()->CenterTitle(true);
      h_hitOnLabFrameZY->SetLineColor(4);
      h_hitOnLabFrameZY->SetLineWidth(2);
      h_hitOnLabFrameZY->GetXaxis()->SetTitleSize(TheSize);
      h_hitOnLabFrameZY->GetXaxis()->SetLabelSize(TheSize);
      h_hitOnLabFrameZY->GetYaxis()->SetTitleSize(TheSize);
      h_hitOnLabFrameZY->GetYaxis()->SetLabelSize(TheSize);
      h_hitOnLabFrameZY->SetStats(false);

      h_hitOnLabFrameZX=new TH2F("h_hitOnLabFrameZX","Lab frame",200,-150.,200.,200,-100.,-30.);
      h_hitOnLabFrameZX->SetXTitle("Z[mm]");
      h_hitOnLabFrameZX->GetXaxis()->CenterTitle(true);
      h_hitOnLabFrameZX->SetYTitle("X[mm]");
      h_hitOnLabFrameZX->GetYaxis()->CenterTitle(true);
      h_hitOnLabFrameZX->SetLineColor(4);
      h_hitOnLabFrameZX->SetLineWidth(2);
      h_hitOnLabFrameZX->GetXaxis()->SetTitleSize(TheSize);
      h_hitOnLabFrameZX->GetXaxis()->SetLabelSize(TheSize);
      h_hitOnLabFrameZX->GetYaxis()->SetTitleSize(TheSize);
      h_hitOnLabFrameZX->GetYaxis()->SetLabelSize(TheSize);
      h_hitOnLabFrameZX->SetStats(false);

      h_hitOnLabFrameZR=new TH2F("h_hitOnLabFrameZR","Lab frame",200,-100.,200.,200,0.,200.);
      h_hitOnLabFrameZR->SetXTitle("Z[mm]");
      h_hitOnLabFrameZR->GetXaxis()->CenterTitle(true);
      h_hitOnLabFrameZR->SetYTitle("R[mm]");
      h_hitOnLabFrameZR->GetYaxis()->CenterTitle(true);
      h_hitOnLabFrameZR->SetLineColor(4);
      h_hitOnLabFrameZR->SetLineWidth(2);
      h_hitOnLabFrameZR->GetXaxis()->SetTitleSize(TheSize);
      h_hitOnLabFrameZR->GetXaxis()->SetLabelSize(TheSize);
      h_hitOnLabFrameZR->GetYaxis()->SetTitleSize(TheSize);
      h_hitOnLabFrameZR->GetYaxis()->SetLabelSize(TheSize);
      h_hitOnLabFrameZR->SetStats(false);

      h_hitOnLabFrameXY=new TH2F("h_hitOnLabFrameXY","Lab frame",200,-100.,-30.,200,-100.,100.);
      h_hitOnLabFrameXY->SetXTitle("X[mm]");
      h_hitOnLabFrameXY->GetXaxis()->CenterTitle(true);
      h_hitOnLabFrameXY->SetYTitle("Y[mm]");
      h_hitOnLabFrameXY->GetYaxis()->CenterTitle(true);
      h_hitOnLabFrameXY->SetLineColor(4);
      h_hitOnLabFrameXY->SetLineWidth(2);
      h_hitOnLabFrameXY->GetXaxis()->SetTitleSize(TheSize);
      h_hitOnLabFrameXY->GetXaxis()->SetLabelSize(TheSize);
      h_hitOnLabFrameXY->GetYaxis()->SetTitleSize(TheSize);
      h_hitOnLabFrameXY->GetYaxis()->SetLabelSize(TheSize);
      h_hitOnLabFrameXY->SetStats(false);
      //Histo Hit Occupancies on different sensor

      // TString HistName,HistTitle;
      for(int i=0;i<NSensors;i++){
        HistName  = TString("h_HitOnSensor") + long(i+1);
        HistTitle = TString("Sensor = ") + long(i+1) ;
        h_HitPosOnSensor[i] = new TH2F(HistName.Data(),HistTitle.Data(),200,-100.,200.,200,-50.,50.);
        h_HitPosOnSensor[i]->SetXTitle("U[mm]");
        h_HitPosOnSensor[i]->GetXaxis()->CenterTitle(true);
        h_HitPosOnSensor[i]->SetYTitle("Lin");
        h_HitPosOnSensor[i]->GetYaxis()->CenterTitle(true);
        // h_HitPosOnSensor[i]->SetZTitle("V[mm]");
        // h_HitPosOnSensor[i]->GetZaxis()->CenterTitle(true);
        h_HitPosOnSensor[i]->SetLineColor(4);
        h_HitPosOnSensor[i]->SetLineWidth(2);
        h_HitPosOnSensor[i]->SetMinimum(1.0e-3);
        h_HitPosOnSensor[i]->GetXaxis()->SetTitleSize(TheSize);
        h_HitPosOnSensor[i]->GetXaxis()->SetLabelSize(TheSize);
        h_HitPosOnSensor[i]->GetYaxis()->SetTitleSize(TheSize);
        h_HitPosOnSensor[i]->GetYaxis()->SetLabelSize(TheSize);
        // h_HitPosOnSensor[i]->GetZaxis()->SetTitleSize(TheSize);
        // h_HitPosOnSensor[i]->GetZaxis()->SetLabelSize(TheSize);
        h_HitPosOnSensor[i]->SetStats(false);
       }
      TheSize = 0.05;
      Pitch = 18.4*1.0e-3; //mm
      NCols = 1152;
      NRows =  576;
      float DClearance = 500*1.0e-3; //mm
      float WSensor = NCols*Pitch; //mm
      float HSensor = NRows*Pitch; //mm
      //Histogram definition of 2D Position of U and V in Ladder referentiel
      // int Nbins_LadderU = NSensors*NCols;
      // int Nbins_LadderV = NSensors*NRows;
      int Nbins_LadderU = 100;
      int Nbins_LadderV = 100;
      double R_LadderU[2];
      // R_LadderU[0] = -(WSensor*3+DClearance*2.5)*1.05;
      R_LadderU[0] =-125.0;
      // R_LadderU[1] = (WSensor*3+DClearance*2.5)*1.05;
      R_LadderU[1] = 15.;
      // R_LadderU[1] = 0.;
      double R_LadderV[2];
      // R_LadderV[0] = -HSensor*0.5*1.05;
      R_LadderV[0] = -6.;
      // R_LadderV[1] = HSensor*0.5*1.05;
      R_LadderV[1] = 6.;

      h_2DUVPosLadder1Module1 = new TH2F("h_2DUVPosLadder1Module1",
                 "U Vs V position in Ladder 1 Module 1",
                 Nbins_LadderU,R_LadderU[0],R_LadderU[1],
                 Nbins_LadderV,R_LadderV[0],R_LadderV[1]);
      h_2DUVPosLadder1Module1->SetXTitle("U_{Ladder} (mm)");
      h_2DUVPosLadder1Module1->GetXaxis()->CenterTitle(true);
      h_2DUVPosLadder1Module1->SetYTitle("V_{Ladder} (mm)");
      h_2DUVPosLadder1Module1->GetYaxis()->CenterTitle(true);
      h_2DUVPosLadder1Module1->SetZTitle("Sim Hits");
      h_2DUVPosLadder1Module1->GetZaxis()->CenterTitle(true);
      h_2DUVPosLadder1Module1->SetLineColor(4);
      h_2DUVPosLadder1Module1->SetLineWidth(2);
      h_2DUVPosLadder1Module1->SetMinimum(1.0e-1);
      h_2DUVPosLadder1Module1->GetXaxis()->SetTitleSize(TheSize);
      h_2DUVPosLadder1Module1->GetXaxis()->SetLabelSize(TheSize);
      h_2DUVPosLadder1Module1->GetYaxis()->SetTitleSize(TheSize);
      h_2DUVPosLadder1Module1->GetYaxis()->SetLabelSize(TheSize);
      h_2DUVPosLadder1Module1->GetZaxis()->SetTitleSize(TheSize);
      h_2DUVPosLadder1Module1->GetZaxis()->SetLabelSize(TheSize);
      // h_2DUVPosLadder1Module1->SetStats(false);
      h_2DUVPosLadder1Module1->SetMarkerStyle(5);
      // h_2DUVPosLadder1Module1->SetMarkerSize(5);

      h_2DUVPosLadder2Module1 = new TH2F("h_2DUVPosLadder2Module1",
                 "U Vs V position in Ladder 2 Module 1",
                 Nbins_LadderU,R_LadderU[0],R_LadderU[1],
                 Nbins_LadderV,R_LadderV[0],R_LadderV[1]);
      h_2DUVPosLadder2Module1->SetXTitle("U_{Ladder} (mm)");
      h_2DUVPosLadder2Module1->GetXaxis()->CenterTitle(true);
      h_2DUVPosLadder2Module1->SetYTitle("V_{Ladder} (mm)");
      h_2DUVPosLadder2Module1->GetYaxis()->CenterTitle(true);
      h_2DUVPosLadder2Module1->SetZTitle("Sim Hits");
      h_2DUVPosLadder2Module1->GetZaxis()->CenterTitle(true);
      h_2DUVPosLadder2Module1->SetLineColor(4);
      h_2DUVPosLadder2Module1->SetLineWidth(2);
      h_2DUVPosLadder2Module1->SetMinimum(1.0e-1);
      h_2DUVPosLadder2Module1->GetXaxis()->SetTitleSize(TheSize);
      h_2DUVPosLadder2Module1->GetXaxis()->SetLabelSize(TheSize);
      h_2DUVPosLadder2Module1->GetYaxis()->SetTitleSize(TheSize);
      h_2DUVPosLadder2Module1->GetYaxis()->SetLabelSize(TheSize);
      h_2DUVPosLadder2Module1->GetZaxis()->SetTitleSize(TheSize);
      h_2DUVPosLadder2Module1->GetZaxis()->SetLabelSize(TheSize);
      // h_2DUVPosLadder2Module1->SetStats(false);
      h_2DUVPosLadder2Module1->SetMarkerStyle(5);
     //  h_2DUVPosLadder2Module1->SetMarkerSize(5);

      h_2DUVPosLadder1Module2 = new TH2F("h_2DUVPosLadder1Module2",
                 "U Vs V position in Ladder 1 Module 2",
                 Nbins_LadderU,R_LadderU[0],R_LadderU[1],
                 Nbins_LadderV,R_LadderV[0],R_LadderV[1]);
      h_2DUVPosLadder1Module2->SetXTitle("U_{Ladder} (mm)");
      h_2DUVPosLadder1Module2->GetXaxis()->CenterTitle(true);
      h_2DUVPosLadder1Module2->SetYTitle("V_{Ladder} (mm)");
      h_2DUVPosLadder1Module2->GetYaxis()->CenterTitle(true);
      h_2DUVPosLadder1Module2->SetZTitle("Sim Hits");
      h_2DUVPosLadder1Module2->GetZaxis()->CenterTitle(true);
      h_2DUVPosLadder1Module2->SetLineColor(4);
      h_2DUVPosLadder1Module2->SetLineWidth(2);
      h_2DUVPosLadder1Module2->SetMinimum(1.0e-1);
      h_2DUVPosLadder1Module2->GetXaxis()->SetTitleSize(TheSize);
      h_2DUVPosLadder1Module2->GetXaxis()->SetLabelSize(TheSize);
      h_2DUVPosLadder1Module2->GetYaxis()->SetTitleSize(TheSize);
      h_2DUVPosLadder1Module2->GetYaxis()->SetLabelSize(TheSize);
      h_2DUVPosLadder1Module2->GetZaxis()->SetTitleSize(TheSize);
      h_2DUVPosLadder1Module2->GetZaxis()->SetLabelSize(TheSize);
      // h_2DUVPosLadder1Module2->SetStats(false);
      h_2DUVPosLadder1Module2->SetMarkerStyle(5);
      // h_2DUVPosLadder1Module2->SetMarkerSize(5);

      h_2DUVPosLadder2Module2 = new TH2F("h_2DUVPosLadder2Module2",
                 "U Vs V position in Ladder 2 Module 2",
                 Nbins_LadderU,R_LadderU[0],R_LadderU[1],
                 Nbins_LadderV,R_LadderV[0],R_LadderV[1]);
      h_2DUVPosLadder2Module2->SetXTitle("U_{Ladder} (mm)");
      h_2DUVPosLadder2Module2->GetXaxis()->CenterTitle(true);
      h_2DUVPosLadder2Module2->SetYTitle("V_{Ladder} (mm)");
      h_2DUVPosLadder2Module2->GetYaxis()->CenterTitle(true);
      h_2DUVPosLadder2Module2->SetZTitle("Sim Hits");
      h_2DUVPosLadder2Module2->GetZaxis()->CenterTitle(true);
      h_2DUVPosLadder2Module2->SetLineColor(4);
      h_2DUVPosLadder2Module2->SetLineWidth(2);
      h_2DUVPosLadder2Module2->SetMinimum(1.0e-1);
      h_2DUVPosLadder2Module2->GetXaxis()->SetTitleSize(TheSize);
      h_2DUVPosLadder2Module2->GetXaxis()->SetLabelSize(TheSize);
      h_2DUVPosLadder2Module2->GetYaxis()->SetTitleSize(TheSize);
      h_2DUVPosLadder2Module2->GetYaxis()->SetLabelSize(TheSize);
      h_2DUVPosLadder2Module2->GetZaxis()->SetTitleSize(TheSize);
      h_2DUVPosLadder2Module2->GetZaxis()->SetLabelSize(TheSize);
      // h_2DUVPosLadder2Module2->SetStats(false);
      h_2DUVPosLadder2Module2->SetMarkerStyle(5);
      // h_2DUVPosLadder2Module2->SetMarkerSize(5);

      h_BeastRecoPartClassType_L1 = new TH1F("h_BeastRecoPartClassType_L1",
             "Reco Parts Class type ",
             4,1,5);
      h_BeastRecoPartClassType_L1->SetXTitle("");
      h_BeastRecoPartClassType_L1->GetXaxis()->CenterTitle(true);
      h_BeastRecoPartClassType_L1->SetYTitle("#Parts");
      h_BeastRecoPartClassType_L1->GetYaxis()->CenterTitle(true);
      h_BeastRecoPartClassType_L1->SetLineColor(4);
      h_BeastRecoPartClassType_L1->SetLineWidth(2);
      h_BeastRecoPartClassType_L1->GetYaxis()->SetTitleSize(TheSize);
      h_BeastRecoPartClassType_L1->GetYaxis()->SetLabelSize(TheSize);
      h_BeastRecoPartClassType_L1->GetXaxis()->SetTitleSize(TheSize);
      h_BeastRecoPartClassType_L1->GetXaxis()->SetLabelSize(TheSize);
      h_BeastRecoPartClassType_L1->SetMinimum(0.);
      h_BeastRecoPartClassType_L2 = new TH1F("h_BeastRecoPartClassType_L2",
             "Reco Parts Class type ",
             4,1,5);
      h_BeastRecoPartClassType_L2->SetXTitle("");
      h_BeastRecoPartClassType_L2->GetXaxis()->CenterTitle(true);
      h_BeastRecoPartClassType_L2->SetYTitle("#Parts");
      h_BeastRecoPartClassType_L2->GetYaxis()->CenterTitle(true);
      h_BeastRecoPartClassType_L2->SetLineColor(4);
      h_BeastRecoPartClassType_L2->SetLineWidth(2);
      h_BeastRecoPartClassType_L2->GetYaxis()->SetTitleSize(TheSize);
      h_BeastRecoPartClassType_L2->GetYaxis()->SetLabelSize(TheSize);
      h_BeastRecoPartClassType_L2->GetXaxis()->SetTitleSize(TheSize);
      h_BeastRecoPartClassType_L2->GetXaxis()->SetLabelSize(TheSize);
      h_BeastRecoPartClassType_L2->SetMinimum(0.);
        // const char *LabelNameCat[4]={"One Side ","Two Sides ","OSL","TSL"};
        for(int i=1;i<5;i++){
          h_BeastRecoPartClassType_L1->GetXaxis()->SetBinLabel(i,LabelNameCat[i-1]);
          h_BeastRecoPartClassType_L2->GetXaxis()->SetBinLabel(i,LabelNameCat[i-1]);
        }

  for (int i=0; i<25; i++) {
    HitOnSensor[i]=0;
    PixelOnSensor[i]=0;
    MCHitOnSensor[i]=0;
    MCPixelOnSensor[i]=0;
  }
  AlejandroHitsCumulated=0;
  FrameNb=1;
  nHitsAllEvt=0;

  int LadderIdTest;
  int TestPartNB=0;
  int NbOS_L1;
  int NbTS_L1;
  int NbOSL_L1;
  int NbTSL_L1;
  int NbOS_L2;
  int NbTS_L2;
  int NbOSL_L2;
  int NbTSL_L2;
  int HitNumberLadder_1;
  int HitNumberModule_11;
  int HitNumberModule_12;
  int HitNumberLadder_2;
  int HitNumberModule_21;
  int HitNumberModule_22;
  int PlaneIdx;
  int LadderNumber;
  bool Module1;
  bool Module2;
  bool Ladder1;
  bool Ladder2;
  bool MoreThanTwoHits;

  int iHitSensorId;
  int iHitNpixels;
  int HitIdx;

    fSession->SetEvents(MaxEvt);
    DTracker *tTracker     = fSession->GetTracker();
    DHit*   aHit      = NULL;
    DPlane *aPlane = NULL;
    DLadder *aLadder = NULL;

    DR3 *hitPositionPlane;
    DR3 hitPositionLadder;
    DR3 hitPositionLab;
    LadderNumber=0;

    HitNumberLadder_1=0;
    HitNumberModule_11=0;
    HitNumberModule_12=0;
    HitNumberLadder_2=0;
    HitNumberModule_21=0;
    HitNumberModule_22=0;

    NbOS_L1=0;
    NbTS_L1=0;
    NbOSL_L1=0;
    NbTSL_L1=0;
    NbOS_L2=0;
    NbTS_L2=0;
    NbOSL_L2=0;
    NbTSL_L2=0;
///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
    int nHitsEvt=0;
    double RLabFrame;
    DPrecAlign *Dprec;

    DBeaster *aBeaster;
    aBeaster=tTracker->GetBeaster();
    aBeaster->ListOfBeastRecoParts_L1.clear();
    aBeaster->ListOfBeastRecoParts_L2.clear();
    DEventMC* MCInfoHolder = tTracker->GetMCInfoHolder();

    for (size_t ievt = 0; ievt < MaxEvt; ievt++) {
      // std::cout << "NEW EVENT" << '\n';
      std::cout << "Frame n° " <<ievt +1<<'\n';
      if( !(fSession->NextRawEvent()) ) break;
      tTracker->Update();

///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
      // std::cout << "Beaster HitsOnM1 = " <<aBeaster->ListOfBeastHitsOnM1.size() <<'\n';
      // std::cout << "Beaster HitsOnM2 = " <<aBeaster->ListOfBeastHitsOnM2.size()<<'\n';
      std::cout << "Beaster RecoParts On L1 = " << aBeaster->ListOfBeastRecoParts_L1.size()<<'\n';
      std::cout << "Beaster RecoParts On L2 = " << aBeaster->ListOfBeastRecoParts_L2.size()<<'\n';

      int NbBeastRecoPart_L1=aBeaster->ListOfBeastRecoParts_L1.size();
      int NbBeastRecoPart_L2=aBeaster->ListOfBeastRecoParts_L2.size();
      // std::cout << "Reco Beaster" << '\n';
      for (int ipart = 0; ipart < NbBeastRecoPart_L1; ipart++) {
        int BeastRecoPartClassType= aBeaster->ListOfBeastRecoParts_L1[ipart].ClassType;
        // std::cout << "***********ClassType on L1 " << BeastRecoPartClassType <<'\n';
        h_BeastRecoPartClassType_L1->Fill(BeastRecoPartClassType);
      }
      for (int ipart = 0; ipart < NbBeastRecoPart_L2; ipart++) {
        int BeastRecoPartClassType= aBeaster->ListOfBeastRecoParts_L2[ipart].ClassType;
        // std::cout << "***********ClassType on L2 " << BeastRecoPartClassType <<'\n';
        h_BeastRecoPartClassType_L2->Fill(BeastRecoPartClassType);
      }
      // std::cout << "Reco Beaster END" << '\n';
///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

      Int_t nPlanes = fSession->GetTracker()->GetPlanesN();
      for( int iPlane=0; iPlane< nPlanes; iPlane++ ) { // loop on planes

        aPlane=tTracker->GetPlane(iPlane+1);
        std::cout << "aPlane = " << iPlane <<'\n';
        if(aPlane->GetHitsN()==0) continue;
          for( Int_t iHit=1; iHit <= aPlane->GetHitsN(); iHit++ ) { // loop on plane hits
            aHit = aPlane->GetHit(iHit);
            // if(aHit->GetStripsInCluster()<=1) continue;
            // std::cout << "aHit" << '\n';
            if(aHit->GetMCHitID()<0) continue;
            nHitsEvt++;
            nHitsAllEvt++;
            //////////////////  Occupancy/Rate  /////////////////////
            HitOnSensor[iPlane]+=1.;
            PixelOnSensor[iHitSensorId]+=iHitNpixels;
            ////////////////////////////////////////////////////////

            //////////////////  Check Geometry   ///////////////////
            // std::cout << "CheckGeometry IN" << '\n';

            hitPositionPlane=aHit->GetPosition();
            Dprec=aPlane->GetPrecAlignment();
            hitPositionLab=Dprec->TransformHitToTracker(*hitPositionPlane);
            //  if(iPlane<12){
            h_hitOnLabFrameZY->Fill((hitPositionLab)(2)/1000,(hitPositionLab)(1)/1000);
            h_hitOnLabFrameZX->Fill((hitPositionLab)(2)/1000,(hitPositionLab)(0)/1000);
            h_hitOnLabFrameXY->Fill((hitPositionLab)(0)/1000,(hitPositionLab)(1)/1000);
            RLabFrame=sqrt(pow((hitPositionLab)(0),2)+pow((hitPositionLab)(1),2));
            h_hitOnLabFrameZR->Fill((hitPositionLab)(2)/1000,RLabFrame/1000);
            h_HitPosOnSensor[iPlane]->Fill((hitPositionLab)(2)/1000,(hitPositionLab)(1)/1000);
            ////////////////////////////////////////////////////////////

            ////////////////////  Hit Position on Ladder Frame   ////////////////////////
            std::cout << "LADDER" << '\n';
            LadderNumber=aPlane->GetLadderNumber();
            aLadder=tTracker->GetLadder(LadderNumber);
              h_hitOnSensorFrame->Fill((*hitPositionPlane)(0)/1000,(*hitPositionPlane)(1)/1000);
              if (LadderNumber==1) {
                PlaneIdx=iPlane+1;
                hitPositionLadder=aLadder->PlaneToLadder(*hitPositionPlane,PlaneIdx);
                if (iPlane+1<7 ){
                   h_2DUVPosLadder1Module1->Fill((hitPositionLadder)(0)/1000,(hitPositionLadder)(1)/1000);
                }else{
                   h_2DUVPosLadder1Module2->Fill((hitPositionLadder)(0)/1000,(hitPositionLadder)(1)/1000);
                }
              }else{
                PlaneIdx=iPlane+1-12;
                hitPositionLadder=aLadder->PlaneToLadder(*hitPositionPlane,PlaneIdx);
                if (iPlane+1<19){
                   h_2DUVPosLadder2Module1->Fill((hitPositionLadder)(0)/1000,(hitPositionLadder)(1)/1000);
                }else{
                   h_2DUVPosLadder2Module2->Fill((hitPositionLadder)(0)/1000,(hitPositionLadder)(1)/1000);
                 }
              }
            ////////////////////////////////////////////////////////////////////////////
          // }//End Selected plane
          } // end loop on plane hits
      } // end loop on planes
      // std::cout << "NHits in evt = "<< nHitsEvt << '\n';

///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
      std::cout << "END CHECK GEOMETRY" << '\n';
      if(MCInfoHolder){
          // int AlejandroNhitsEvt=0;
          for (size_t ihit = 0; ihit < MCInfoHolder->GetNSimHits(); ihit++) {
            if (MCInfoHolder->GetASimHit(ihit).Npixels==0) continue;
              // AlejandroNhitsEvt++;
              AlejandroHitsCumulated++;
          }

          // // std::cout << "AlejandroNhitsEvt = " << AlejandroNhitsEvt<<'\n';
          // FrameNb++;
          // std::cout << "MCInfoHolder" << '\n';
        int NbMcTruthHits=0;
        for(int ipart=0;ipart<MCInfoHolder->GetNSimParticles();ipart++) {
          TestPartNB++;
          h_BkgType->Fill(MCInfoHolder->GetASimParticle(ipart).BKGType);

          h_VtxZR->Fill(MCInfoHolder->GetASimParticle(ipart).ProdVtx.Z(),
                          sqrt(
                                pow(MCInfoHolder->GetASimParticle(ipart).ProdVtx.X(),2)
                                +pow(MCInfoHolder->GetASimParticle(ipart).ProdVtx.Y(),2)
                              )
                        );
          h_VtxXY->Fill(MCInfoHolder->GetASimParticle(ipart).ProdVtx.X(),MCInfoHolder->GetASimParticle(ipart).ProdVtx.Y());
          int FirstHitIdx = MCInfoHolder->GetASimParticle(ipart).FirstHitIdx;

///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
          int NbHitsTest=MCInfoHolder->GetASimParticle(ipart).NHits;
          if(NbHitsTest==0) continue;
          h_NbPartvsFrame->Fill(ievt);
          Module1=false;
          Module2=false;
          Ladder1=false;
          Ladder2=false;
          LadderIdTest=0;
          MoreThanTwoHits=false;
          for(int ihit=0;ihit<MCInfoHolder->GetASimParticle(ipart).NHits;ihit++) {
            NbMcTruthHits++;
            iHitNpixels=MCInfoHolder->GetASimHit(FirstHitIdx + ihit).Npixels;
            if(iHitNpixels==0) continue;
            h_NbHitvsFrame->Fill(ievt);
            iHitSensorId=MCInfoHolder->GetASimHit(FirstHitIdx + ihit).sensorID;
            if (iHitSensorId<12) {
              h_HitPos_L1->Fill(MCInfoHolder->GetASimHit(FirstHitIdx + ihit).PosRecoAveUVLaddermm.X(),MCInfoHolder->GetASimHit(FirstHitIdx + ihit).PosRecoAveUVLaddermm.Y());
            }else{
              h_HitPos_L2->Fill(MCInfoHolder->GetASimHit(FirstHitIdx + ihit).PosRecoAveUVLaddermm.X(),MCInfoHolder->GetASimHit(FirstHitIdx + ihit).PosRecoAveUVLaddermm.Y());
            }
            HitIdx++;
            ////////////////// MCtruth Occupancy/Rate  /////////////////////
            MCHitOnSensor[iHitSensorId]+=1.;
            MCPixelOnSensor[iHitSensorId]=PixelOnSensor[iHitSensorId]+iHitNpixels;
            ////////////////////////////////////////////////////////////////
            // if (iHitNpixels>1) {
              h_NbPixelsVsHit->Fill(iHitNpixels);
            // }
            // if(iHitSensorId==0) std::cout << "*********************SENSOR 0 FOUND " << '\n';
            // if(iHitSensorId==24) std::cout << "*********************SENSOR 24 FOUND " << '\n';
            if (NbHitsTest>2) {MoreThanTwoHits=true;}
            if (iHitSensorId<12) {
              Ladder1=true;
              HitNumberLadder_1++;
              if (iHitSensorId<6) {
                Module1=true;
                HitNumberModule_11++;
              }else{ //end IF Ladder 1 Module 1
                Module2=true;
                HitNumberModule_12++;
              }//end IF Ladder 1 Module 2
            }else{ //End if Ladder 1
              Ladder2=true;
              HitNumberLadder_2++;
              if (iHitSensorId<18) {
                Module1=true;
                HitNumberModule_21++;
              }else{ //end IF Ladder 2 Module 1
                Module2=true;
                HitNumberModule_22++;
              } //end IF Ladder 2 Module 2
            }//End if Ladder 2
          } // End loop on Hits

          if(Module1==true && Module2==true){
            if(MoreThanTwoHits==true){ //TWO SIDES LOOPER ELECTRON
                if(Ladder1 && !(Ladder2)){
                  NbTSL_L1++;
                  std::cout << "******** MC TSl L1" << '\n';
                  PartCat_Ladder1->Fill(4.);
                }
                if(Ladder2 && !(Ladder1)){
                  NbTSL_L2++;
                  std::cout << "******** MC TSL L2" << '\n';
                  PartCat_Ladder2->Fill(4.);
                }
              }else{ //TWO SIDES ELECTRON
                if(Ladder1 && !(Ladder2)){
                  NbTS_L1++;
                  std::cout << "******** MC TS L1" << '\n';
                  PartCat_Ladder1->Fill(2.);
                }
                if(Ladder2 && !(Ladder1)){
                  NbTS_L2++;
                  std::cout << "******** MC TS L2" << '\n';
                  PartCat_Ladder2->Fill(2.);
                }
              }
          }else if((Module1==true && Module2==false)||(Module1==false && Module2==true)){

            if(NbHitsTest>1){ //ONE SIDE LOOPER ELECTRON
              if(Ladder1 && !(Ladder2)){
                NbOSL_L1++;
                PartCat_Ladder1->Fill(3.);
                std::cout << "******** MC OSL L1" << '\n';
              }
              if(Ladder2 && !(Ladder1)){
                NbOSL_L2++;
                std::cout << "******** MC OSL L2" << '\n';
                PartCat_Ladder2->Fill(3.);
              }
            }else if(NbHitsTest==1){ //ONE SIDE ELECTRON
              if(Ladder1 && !(Ladder2)){
                NbOS_L1++;
                std::cout << "******** MC OS L1" << '\n';
                PartCat_Ladder1->Fill(1.);
              }
              if(Ladder2 && !(Ladder1)){
                NbOS_L2++;
                std::cout << "******** MC OS L2" << '\n';
                PartCat_Ladder2->Fill(1.);
              }
            }
          }
        } // End Loop on parts
        // std::cout << "MCInfoHolder Nb Hits = " <<NbMcTruthHits <<'\n';
      } //End If MC parts
///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
  }
  std::cout << "AlejandroHitsCumulated = " <<AlejandroHitsCumulated <<'\n';

  if (MCInfoHolder) {

    for (int iSensor = 0; iSensor < NSensors; iSensor++) {
    h_PixelOnSensor->SetBinContent(iSensor+1,MCPixelOnSensor[iSensor]);
    h_HitOnSensor->SetBinContent(iSensor+1,MCHitOnSensor[iSensor]);
    h_Occupancies->SetBinContent(iSensor+1,float(HitOnSensor[iSensor])/50000);
    h_Rates->SetBinContent(iSensor+1,float(PixelOnSensor[iSensor])/(50000*663552));
    }

    TCanvas *CMcTruth = new TCanvas("CMcTruth","CMcTruth");
    CMcTruth->Print("Results/26999/Plots/MCTruth.pdf[");
    CMcTruth->Clear();
    h_BkgType->Draw();
    CMcTruth->Print("Results/26999/Plots/MCTruth.pdf");
    CMcTruth->Clear();
    PartCat_Ladder1->SetMinimum(0.);
    PartCat_Ladder1->Draw();
    CMcTruth->Print("Results/26999/Plots/MCTruth.pdf");
    CMcTruth->Clear();
    PartCat_Ladder2->SetMinimum(0.);
    PartCat_Ladder2->Draw();
    CMcTruth->Print("Results/26999/Plots/MCTruth.pdf");
    CMcTruth->Clear();
    CMcTruth->Divide(1,2);
    CMcTruth->cd(1);
    h_VtxZR->Draw("COL Z");
    CMcTruth->cd(2);
    h_VtxXY->Draw("COL Z");
    CMcTruth->Print("Results/26999/Plots/MCTruth.pdf");
    CMcTruth->Clear();
    h_HitOnSensor->Draw();
    CMcTruth->Print("Results/26999/Plots/MCTruth.pdf");
    CMcTruth->Clear();
    h_PixelOnSensor->Draw();
    CMcTruth->Print("Results/26999/Plots/MCTruth.pdf");
    CMcTruth->Clear();
    h_Occupancies->Draw();
    CMcTruth->Print("Results/26999/Plots/MCTruth.pdf");
    CMcTruth->Clear();
    h_Rates->Draw();
    CMcTruth->Print("Results/26999/Plots/MCTruth.pdf");
    CMcTruth->Print("Results/26999/Plots/MCTruth.pdf]");
    h_Occupancies->Reset();
    h_PixelOnSensor->Reset();
    h_HitOnSensor->Reset();
    h_Rates->Reset();


    //

    // // cMonitor->Print("Results/26999/Plots/Monitoring.pdf]");
  }

  TCanvas *c = new TCanvas("c","c");
  c->Print("Results/26999/Plots/GeometryCheck.pdf[");
  c->Clear();
  h_hitOnLabFrameZR->Draw();
  c->Print("Results/26999/Plots/GeometryCheck.pdf");
  c->Clear();
  h_hitOnLabFrameZY->Draw("COL Z");
  c->Print("Results/26999/Plots/GeometryCheck.pdf");
  c->Clear();
  h_hitOnLabFrameXY->Draw("COL Z");
  c->Print("Results/26999/Plots/GeometryCheck.pdf");
  c->Clear();
  h_hitOnLabFrameZX->Draw("COL Z");
  c->Print("Results/26999/Plots/GeometryCheck.pdf");

  c->Clear();
  c->Divide(1,4);
  c->cd(1);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.18);
  gPad->SetRightMargin(0.15);
  h_2DUVPosLadder1Module1->Draw("colz");
  c->cd(2);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.18);
  gPad->SetRightMargin(0.15);
  h_2DUVPosLadder1Module2->Draw("colz");
  c->cd(3);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.18);
  gPad->SetRightMargin(0.15);
  h_2DUVPosLadder2Module1->Draw("colz");
  c->cd(4);
  gPad->SetFillColor(10);
  gPad->SetFrameFillColor(10);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.18);
  gPad->SetRightMargin(0.15);
  h_2DUVPosLadder2Module2->Draw("colz");
  c->Print("Results/26999/Plots/GeometryCheck.pdf");
  c->Print("Results/26999/Plots/GeometryCheck.pdf]");

  TCanvas *cMonitor = new TCanvas("cMonitor","cMonitor");
  cMonitor->Print("Results/26999/Plots/Monitoring.pdf[");

  cMonitor->Clear();
  h_NbPartvsFrame->SetAxisRange(0., MaxEvt,"X");
  h_NbPartvsFrame->Draw();
  cMonitor->Print("Results/26999/Plots/Monitoring.pdf");
  cMonitor->Clear();
  h_NbHitvsFrame->SetAxisRange(0., MaxEvt,"X");
  h_NbHitvsFrame->Draw();
  cMonitor->Print("Results/26999/Plots/Monitoring.pdf");
  cMonitor->Clear();
  h_NbPixelsVsHit->Draw();
  cMonitor->Print("Results/26999/Plots/Monitoring.pdf");
  cMonitor->Clear();
  cMonitor->Divide(1,2);
  cMonitor->cd(1);
  h_HitPos_L1->Draw("COL Z");
  cMonitor->cd(2);
  h_HitPos_L2->Draw("COL Z");
  cMonitor->Print("Results/26999/Plots/Monitoring.pdf");
  for (int iSensor = 0; iSensor < NSensors; iSensor++) {
    h_PixelOnSensor->SetBinContent(iSensor+1,PixelOnSensor[iSensor]);
    h_HitOnSensor->SetBinContent(iSensor+1,HitOnSensor[iSensor]);
    h_Occupancies->SetBinContent(iSensor+1,float(HitOnSensor[iSensor])/50000);
    h_Rates->SetBinContent(iSensor+1,float(PixelOnSensor[iSensor])/(50000*663552));
  }

  cMonitor->Clear();
  h_HitOnSensor->Draw();
  cMonitor->Print("Results/26999/Plots/Monitoring.pdf");
  cMonitor->Clear();
  h_PixelOnSensor->Draw();
  cMonitor->Print("Results/26999/Plots/Monitoring.pdf");
  cMonitor->Clear();
  h_Occupancies->Draw();
  cMonitor->Print("Results/26999/Plots/Monitoring.pdf");
  cMonitor->Clear();
  h_Rates->Draw();
  cMonitor->Print("Results/26999/Plots/Monitoring.pdf");
  cMonitor->Print("Results/26999/Plots/Monitoring.pdf]");

  TCanvas *cOffLine = new TCanvas("cOffLine","cOffLine");
  cOffLine->Print("Results/26999/Plots/OffLine_Analysis.pdf[");
  cOffLine->Clear();
  h_BeastRecoPartClassType_L1->Draw();
  cOffLine->Print("Results/26999/Plots/OffLine_Analysis.pdf");
  cOffLine->Clear();
  h_BeastRecoPartClassType_L2->Draw();
  cOffLine->Print("Results/26999/Plots/OffLine_Analysis.pdf");
  cOffLine->Print("Results/26999/Plots/OffLine_Analysis.pdf]");

  return;
}
//______________________________________________________________________________
//
void  MRaw::BeastStatStudy(){

  double U1;
  double U2;
  double dU;

  double V1;
  double V2;
  double dV;

  double dR;

  double dRminSamePartSameModule;
  double dRminSamePartDiffModule;
  double dRminDiffPartSameModule;
  double dRminDiffPartDiffModule;

  std::vector<double> dRSamePartSameModuleL1;
  std::vector<double> dRSamePartDiffModuleL1;
  std::vector<double> dRDiffPartSameModuleL1;
  std::vector<double> dRDiffPartDiffModuleL1;

  std::vector<double> dRSamePartSameModuleL2;
  std::vector<double> dRSamePartDiffModuleL2;
  std::vector<double> dRDiffPartSameModuleL2;
  std::vector<double> dRDiffPartDiffModuleL2;

  TH1D *h_dRSamePartSameModuleL1;
  TH1D *h_dRSamePartDiffModuleL1;
  TH1D *h_dRDiffPartSameModuleL1;
  TH1D *h_dRDiffPartDiffModuleL1;

  TH1D *h_dRSamePartSameModuleL2;
  TH1D *h_dRSamePartDiffModuleL2;
  TH1D *h_dRDiffPartSameModuleL2;
  TH1D *h_dRDiffPartDiffModuleL2;

  int iHitIdx;
  int iFirstHitIdx;
  int iSensorId;
  int iHitModuleId;
  int iHitLadderId;

  int jHitIdx;
  int jFirstHitIdx;
  int jSensorId;
  int jHitModuleId;
  int jHitLadderId;
  int MaxEvt=10000;
  fSession->SetEvents(MaxEvt);
  DTracker *tTracker     = fSession->GetTracker();
  DEventMC* MCInfoHolder = tTracker->GetMCInfoHolder();
  for (size_t ievt = 0; ievt < MaxEvt; ievt++) {
      // std::cout << "NEW EVENT" << '\n';
      std::cout << "Frame n° " <<ievt +1<<'\n';
      if( !(fSession->NextRawEvent()) ) break;
      tTracker->Update();
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
      for(int ipart=0;ipart<MCInfoHolder->GetNSimParticles();ipart++) {
        iFirstHitIdx = MCInfoHolder->GetASimParticle(ipart).FirstHitIdx;
        for(int ihit=0;ihit<MCInfoHolder->GetASimParticle(ipart).NHits;ihit++){
          iHitIdx=iFirstHitIdx+ihit;
          // std::cout << "********************" << '\n';
          iSensorId=MCInfoHolder->GetASimHit(iHitIdx).sensorID;
          iHitLadderId=GetBeastLadderId(iSensorId);
          // std::cout << "iHitLadderId = "<< iHitLadderId << '\n';
          iHitModuleId=GetBeastModuleId(iSensorId);
          // std::cout << " iHitModuleId= "<< iHitModuleId << '\n';
          dRminSamePartSameModule=3000.;
          dRminSamePartDiffModule=3000.;
          dRminDiffPartSameModule=3000.;
          dRminDiffPartDiffModule=3000.;

          for(int jpart=0;jpart<MCInfoHolder->GetNSimParticles();jpart++) {
            if(ipart>jpart) continue;
             jFirstHitIdx = MCInfoHolder->GetASimParticle(jpart).FirstHitIdx;
            for(int jhit=0;jhit<MCInfoHolder->GetASimParticle(jpart).NHits;jhit++){
              jHitIdx=jFirstHitIdx+jhit;
              jSensorId=MCInfoHolder->GetASimHit(jHitIdx).sensorID;
              jHitLadderId=GetBeastLadderId(jSensorId);
              // std::cout << "jHitLadderId = "<< jHitLadderId << '\n';
              jHitModuleId=GetBeastModuleId(jSensorId);
              // std::cout << " jHitModuleId= "<< jHitModuleId << '\n';

              U1=MCInfoHolder->GetASimHit(iHitIdx).PosRecoAveUVLaddermm.X();
              U2=MCInfoHolder->GetASimHit(jHitIdx).PosRecoAveUVLaddermm.X();
              dU=abs(U1-U2);
              V1=MCInfoHolder->GetASimHit(iHitIdx).PosRecoAveUVLaddermm.Y();
              V2=MCInfoHolder->GetASimHit(jHitIdx).PosRecoAveUVLaddermm.Y();
              dV=abs(V1-V2);
              dR=sqrt( pow(dU,2) + pow(dV,2) );

              if(iHitLadderId != jHitLadderId) continue;
              // std::cout << "**** ihitLadder = jHitLadder" << '\n';
              if (ipart==jpart) {
                // std::cout << "**** SAME PART" << '\n';
                if(iHitIdx==jHitIdx) continue;
                if(iHitModuleId == jHitModuleId){
                  // std::cout << "SAME MODULE" << '\n';
                  // std::cout << "dR = "<<dR << '\n';
                  if(dR<dRminSamePartSameModule) dRminSamePartSameModule=dR;
                  // std::cout << "dRmin = " << dRminSamePartSameModule <<'\n';
                }
                if(iHitModuleId != jHitModuleId){
                  // std::cout << "DIFF MoDULE" << '\n';
                  // std::cout << "dR = "<<dR << '\n';
                  if(dR<dRminSamePartDiffModule) dRminSamePartDiffModule=dR;
                  // std::cout << "dRmin = " << dRminSamePartDiffModule <<'\n';
                }
              }//End If ipart=jpart
              if (ipart != jpart) {
                // std::cout << "**** DIFF PART" << '\n';
                if(iHitModuleId == jHitModuleId){
                  // std::cout << "SAME MODULE" << '\n';
                  // std::cout << "dR = "<<dR << '\n';
                  if(dR<dRminDiffPartSameModule) dRminDiffPartSameModule=dR;
                  // std::cout << "dRmin = " << dRminDiffPartSameModule <<'\n';
                }
                if(iHitModuleId != jHitModuleId){
                  // std::cout << "DIFF MoDULE" << '\n';
                  // std::cout << "dR = "<<dR << '\n';
                  if(dR<dRminDiffPartDiffModule) dRminDiffPartDiffModule=dR;
                  // std::cout << "dRmin = " << dRminDiffPartDiffModule <<'\n';
                }
              }
              }//End loop on jhit

            }//End loop on jpart
            if (dRminSamePartSameModule != 3000.) {
              // std::cout << "Less Than 3000" << '\n';
              if(iHitLadderId==1){
                if ( std::find(dRSamePartSameModuleL1.begin(), dRSamePartSameModuleL1.end(), dRminSamePartSameModule) == dRSamePartSameModuleL1.end() ){
                  // std::cout << "PUSHBACK " << '\n';
                  dRSamePartSameModuleL1.push_back(dRminSamePartSameModule);
                }
              }
              if(iHitLadderId==2){
                if ( std::find(dRSamePartSameModuleL2.begin(), dRSamePartSameModuleL2.end(), dRminSamePartSameModule) == dRSamePartSameModuleL2.end() ){
                  // std::cout << "PUSHBACK " << '\n';
                  dRSamePartSameModuleL2.push_back(dRminSamePartSameModule);
                }
              }
            }

            if (dRminSamePartDiffModule != 3000.) {
              // std::cout << "Less Than 3000" << '\n';
              if(iHitLadderId==1){
                if ( std::find(dRSamePartDiffModuleL1.begin(), dRSamePartDiffModuleL1.end(), dRminSamePartDiffModule) == dRSamePartDiffModuleL1.end() ){
                  // std::cout << "PUSHBACK " << '\n';
                  dRSamePartDiffModuleL1.push_back(dRminSamePartDiffModule);
                }
              }
              if(iHitLadderId==2){
                if ( std::find(dRSamePartDiffModuleL2.begin(), dRSamePartDiffModuleL2.end(), dRminSamePartDiffModule) == dRSamePartDiffModuleL2.end() ){
                  // std::cout << "PUSHBACK " << '\n';
                  dRSamePartDiffModuleL2.push_back(dRminSamePartDiffModule);
                }
              }
            }

            if (dRminDiffPartSameModule != 3000.) {
              // std::cout << "Less Than 3000" << '\n';
              if(iHitLadderId==1){
                if ( std::find(dRDiffPartSameModuleL1.begin(), dRDiffPartSameModuleL1.end(), dRminDiffPartSameModule) == dRDiffPartSameModuleL1.end() ){
                  // std::cout << "PUSHBACK " << '\n';
                  dRDiffPartSameModuleL1.push_back(dRminDiffPartSameModule);
                }
              }
              if(iHitLadderId==2){
                if ( std::find(dRDiffPartSameModuleL2.begin(), dRDiffPartSameModuleL2.end(), dRminDiffPartSameModule) == dRDiffPartSameModuleL2.end() ){
                  // std::cout << "PUSHBACK " << '\n';
                  dRDiffPartSameModuleL2.push_back(dRminDiffPartSameModule);
                }
              }
            }

            if (dRminDiffPartDiffModule != 3000.) {
              // std::cout << "Less Than 3000" << '\n';
              if(iHitLadderId==1){
                if ( std::find(dRDiffPartDiffModuleL1.begin(), dRDiffPartDiffModuleL1.end(), dRminDiffPartDiffModule) == dRDiffPartDiffModuleL1.end() ){
                  // std::cout << "PUSHBACK " << '\n';
                  dRDiffPartDiffModuleL1.push_back(dRminDiffPartDiffModule);
                }
              }
              if(iHitLadderId==2){
                if ( std::find(dRDiffPartDiffModuleL2.begin(), dRDiffPartDiffModuleL2.end(), dRminDiffPartDiffModule) == dRDiffPartDiffModuleL2.end() ){
                  // std::cout << "PUSHBACK " << '\n';
                  dRDiffPartDiffModuleL2.push_back(dRminDiffPartDiffModule);
                }
              }
            }
            // std::cout << "+++++++++++++++++++++++++++++++++++++" << '\n';
          }//End loop on ihit
        }//End loop on ipart


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
      // for(int ipart=0;ipart<MCInfoHolder->GetNSimParticles();ipart++) {
      //   int FirstHitIdx = MCInfoHolder->GetASimParticle(ipart).FirstHitIdx;
      //   int NbHitsTest=MCInfoHolder->GetASimParticle(ipart).NHits;
      //   if(NbHitsTest==0) continue;
      //   Module1=false;
      //   Module2=false;
      //   Ladder1=false;
      //   Ladder2=false;
      //   LadderIdTest=0;
      //   MoreThanTwoHits=false;
      //   for(int ihit=0;ihit<MCInfoHolder->GetASimParticle(ipart).NHits;ihit++){
      //     iHitNpixels=MCInfoHolder->GetASimHit(FirstHitIdx + ihit).Npixels;
      //       if(iHitNpixels==0) continue;
      //
      //       if (NbHitsTest>2) {MoreThanTwoHits=true;}
      //       if (iHitSensorId<12) {
      //         Ladder1=true;
      //         HitNumberLadder_1++;
      //         if (iHitSensorId<6) {
      //           Module1=true;
      //         }else{ //end IF Ladder 1 Module 1
      //           Module2=true;
      //         }//end IF Ladder 1 Module 2
      //       }else{ //End if Ladder 1
      //         Ladder2=true;
      //         if (iHitSensorId<18) {
      //           Module1=true;
      //         }else{ //end IF Ladder 2 Module 1
      //           Module2=true;
      //         } //end IF Ladder 2 Module 2
      //       }//End if Ladder 2
      //   }//END Loop on Hits
      //
      //   /////////////////////////////////////////////////////////////
      //   /////////////////////////////////////////////////////////////
      //   if(Module1==true && Module2==true){
      //     if(MoreThanTwoHits==true){ //TWO SIDES LOOPER ELECTRON
      //         if(Ladder1 && !(Ladder2)){
      //           NbTSL_L1++;
      //           // std::cout << "******** MC TSl L1" << '\n';
      //           PartCat_Ladder1->Fill(4.);
      //         }
      //         if(Ladder2 && !(Ladder1)){
      //           NbTSL_L2++;
      //           // std::cout << "******** MC TSL L2" << '\n';
      //           PartCat_Ladder2->Fill(4.);
      //         }
      //       }else{ //TWO SIDES ELECTRON
      //         if(Ladder1 && !(Ladder2)){
      //           NbTS_L1++;
      //           // std::cout << "******** MC TS L1" << '\n';
      //           PartCat_Ladder1->Fill(2.);
      //         }
      //         if(Ladder2 && !(Ladder1)){
      //           NbTS_L2++;
      //           // std::cout << "******** MC TS L2" << '\n';
      //           PartCat_Ladder2->Fill(2.);
      //         }
      //       }
      //   }else if((Module1==true && Module2==false)||(Module1==false && Module2==true)){
      //
      //     if(NbHitsTest>1){ //ONE SIDE LOOPER ELECTRON
      //       if(Ladder1 && !(Ladder2)){
      //         NbOSL_L1++;
      //         PartCat_Ladder1->Fill(3.);
      //         // std::cout << "******** MC OSL L1" << '\n';
      //       }
      //       if(Ladder2 && !(Ladder1)){
      //         NbOSL_L2++;
      //         // std::cout << "******** MC OSL L2" << '\n';
      //         PartCat_Ladder2->Fill(3.);
      //       }
      //     }else if(NbHitsTest==1){ //ONE SIDE ELECTRON
      //       if(Ladder1 && !(Ladder2)){
      //         NbOS_L1++;
      //         // std::cout << "******** MC OS L1" << '\n';
      //         PartCat_Ladder1->Fill(1.);
      //       }
      //       if(Ladder2 && !(Ladder1)){
      //         NbOS_L2++;
      //         // std::cout << "******** MC OS L2" << '\n';
      //         PartCat_Ladder2->Fill(1.);
      //       }
      //     }
      //   }
      //   //////////////////////////////////////////////////////////////////////
      //   //////////////////////////////////////////////////////////////////////
      //
      // }//END Loop on Parts
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
  }//END Loop on Evt

  h_dRSamePartSameModuleL1=new TH1D("","Ladder1 2D distance: Same Particule Same Module ",100,0.,140.);
  h_dRSamePartSameModuleL1->SetXTitle("dR[mm]");
  h_dRSamePartSameModuleL1->SetLineColor(3);

  h_dRSamePartDiffModuleL1=new TH1D("","Ladder1 2D distance: Same Particule Different Module",100,0.,140.);
  h_dRSamePartDiffModuleL1->SetXTitle("dR[mm]");
  h_dRSamePartDiffModuleL1->SetLineColor(3);

  h_dRDiffPartSameModuleL1=new TH1D("","Ladder1 2D distance: Different Particule Same Module",100,0.,140.);
  h_dRDiffPartSameModuleL1->SetXTitle("dR[mm]");
  h_dRDiffPartSameModuleL1->SetLineColor(2);

  h_dRDiffPartDiffModuleL1=new TH1D("","Ladder1 2D distance: Different Particule Different Module",100,0.,140.);
  h_dRDiffPartDiffModuleL1->SetXTitle("dR[mm]");
  h_dRDiffPartDiffModuleL1->SetLineColor(2);

  h_dRSamePartSameModuleL2=new TH1D("","Ladder2 2D distance: Same Particule Same Module ",100,0.,140.);
  h_dRSamePartSameModuleL2->SetXTitle("dR[mm]");
  h_dRSamePartSameModuleL2->SetLineColor(3);

  h_dRSamePartDiffModuleL2=new TH1D("","Ladder2 2D distance: Same Particule Different Module",100,0.,140.);
  h_dRSamePartDiffModuleL2->SetXTitle("dR[mm]");
  h_dRSamePartDiffModuleL2->SetLineColor(3);

  h_dRDiffPartSameModuleL2=new TH1D("","Ladder2 2D distance: Different Particule Same Module",100,0.,140.);
  h_dRDiffPartSameModuleL2->SetXTitle("dR[mm]");
  h_dRDiffPartSameModuleL2->SetLineColor(2);

  h_dRDiffPartDiffModuleL2=new TH1D("","Ladder2 2D distance: Different Particule Different Module",100,0.,140.);
  h_dRDiffPartDiffModuleL2->SetXTitle("dR[mm]");
  h_dRDiffPartDiffModuleL2->SetLineColor(2);

  for(int i=0;i<dRSamePartSameModuleL1.size();i++){
    h_dRSamePartSameModuleL1->Fill(dRSamePartSameModuleL1[i]);
  }
  for(int i=0;i<dRSamePartDiffModuleL1.size();i++){
    h_dRSamePartDiffModuleL1->Fill(dRSamePartDiffModuleL1[i]);
  }
  for(int i=0;i<dRDiffPartSameModuleL1.size();i++){
    h_dRDiffPartSameModuleL1->Fill(dRDiffPartSameModuleL1[i]);
  }
  for(int i=0;i<dRDiffPartDiffModuleL1.size();i++){
    h_dRDiffPartDiffModuleL1->Fill(dRDiffPartDiffModuleL1[i]);
  }

  for(int i=0;i<dRSamePartSameModuleL2.size();i++){
    h_dRSamePartSameModuleL2->Fill(dRSamePartSameModuleL2[i]);
  }
  for(int i=0;i<dRSamePartDiffModuleL2.size();i++){
    h_dRSamePartDiffModuleL2->Fill(dRSamePartDiffModuleL2[i]);
  }
  for(int i=0;i<dRDiffPartSameModuleL2.size();i++){
    h_dRDiffPartSameModuleL2->Fill(dRDiffPartSameModuleL2[i]);
  }
  for(int i=0;i<dRDiffPartDiffModuleL2.size();i++){
    h_dRDiffPartDiffModuleL2->Fill(dRDiffPartDiffModuleL2[i]);
  }
  h_dRSamePartSameModuleL1->Scale(1/h_dRSamePartSameModuleL1->Integral("width"));
  h_dRDiffPartSameModuleL1->Scale(1/h_dRDiffPartSameModuleL1->Integral("width"));

  h_dRSamePartDiffModuleL1->Scale(1/h_dRSamePartDiffModuleL1->Integral("width"));
  h_dRDiffPartDiffModuleL1->Scale(1/h_dRDiffPartDiffModuleL1->Integral("width"));

  h_dRSamePartSameModuleL2->Scale(1/h_dRSamePartSameModuleL2->Integral("width"));
  h_dRDiffPartSameModuleL2->Scale(1/h_dRDiffPartSameModuleL2->Integral("width"));

  h_dRSamePartDiffModuleL2->Scale(1/h_dRSamePartDiffModuleL2->Integral("width"));
  h_dRDiffPartDiffModuleL2->Scale(1/h_dRDiffPartDiffModuleL2->Integral("width"));

  TCanvas *c = new TCanvas("c","c");
  c->Print("Results/26999/Plots/BeastStatStudy.pdf[");
  c->Clear();
  h_dRSamePartSameModuleL1->Draw("");
  h_dRDiffPartSameModuleL1->Draw("SAME");
  c->Print("Results/26999/Plots/BeastStatStudy.pdf");
  c->Clear();
  h_dRSamePartDiffModuleL1->Draw("");
  h_dRDiffPartDiffModuleL1->Draw("SAME");
  c->Print("Results/26999/Plots/BeastStatStudy.pdf");

  c->Clear();
  h_dRSamePartSameModuleL2->Draw("");
  h_dRDiffPartSameModuleL2->Draw("SAME");
  c->Print("Results/26999/Plots/BeastStatStudy.pdf");
  c->Clear();
  h_dRSamePartDiffModuleL2->Draw("");
  h_dRDiffPartDiffModuleL2->Draw("SAME");
  c->Print("Results/26999/Plots/BeastStatStudy.pdf");
  c->Print("Results/26999/Plots/BeastStatStudy.pdf]");
  return;
}
//______________________________________________________________________________
//
int  MRaw::GetBeastLadderId(int SensorId){
  if (SensorId<12) return 1;
  else             return 2;

}
//______________________________________________________________________________
//
int  MRaw::GetBeastModuleId(int SensorId){
    if (SensorId<12) {
      if (SensorId<6) {
        return 1;
      }else{ //end IF Ladder 1 Module 1
        return 2;
      }//end IF Ladder 1 Module 2
    }else{ //End if Ladder 1
      if (SensorId<18) {
        return 1;
      }else{ //end IF Ladder 2 Module 1
        return 2;
      } //end IF Ladder 2 Module 2
    }//End if Ladder 2
}
//______________________________________________________________________________
//
void MRaw::BeastCheckPosition(){
      int MaxEvt=100000;
      fSession->SetEvents(MaxEvt);
      DTracker *tTracker     = fSession->GetTracker();
      DHit*   aHit      = NULL;
      DPlane *aPlane = NULL;
      DPrecAlign *Dprec;
      int McHitId;
      DR3 hitPositionLab;
      DR3 *hitPositionPlane;
      DEventMC* MCInfoHolder = tTracker->GetMCInfoHolder();
      fSession->NextRawEvent();
      tTracker->Update();
      Int_t nPlanes = fSession->GetTracker()->GetPlanesN();
      for( int iPlane=0; iPlane< nPlanes; iPlane++ ) {
        aPlane=tTracker->GetPlane(iPlane+1);
        if(aPlane->GetHitsN()==0) continue;
        for( Int_t iHit=1; iHit <= aPlane->GetHitsN(); iHit++ ) {
          aHit = aPlane->GetHit(iHit);
          if(aHit->GetMCHitID()<0) continue;
          /////////////////////////////////////////////////////////////////////
          ////////////////// POSITION IN LAB FRAME BY TAF /////////////////////
          hitPositionPlane=aHit->GetPosition();
          Dprec=aPlane->GetPrecAlignment();
          hitPositionLab=Dprec->TransformHitToTracker(*hitPositionPlane);
          /////////////////////////////////////////////////////////////////////
          ///////////////// POSITION IN LAB FRAME BY McInfoHolder /////////////////////
          McHitId= aHit->GetMCHitID();
          if(MCInfoHolder->GetASimHit(McHitId).ThetaLoc >=15) continue;
          std::cout << "+++++++++++++++++ New Hit on Plane:"<<iPlane+1 <<"++++++++++++++++++++++++" << '\n';
          std::cout << "X in TAF = "<<  (hitPositionLab)(0)/1000<<" X in McInfoHolder = " << MCInfoHolder->GetASimHit(McHitId).PosInXYZmm.X()<<'\n';
          std::cout << "----------- X Offset = " <<abs(((hitPositionLab)(0)/1000) - MCInfoHolder->GetASimHit(McHitId).PosInXYZmm.X())<<'\n';
          std::cout << "Y in TAF = "<< (hitPositionLab)(1)/1000<<" Y in McInfoHolder = " << MCInfoHolder->GetASimHit(McHitId).PosInXYZmm.Y()<<'\n';
          std::cout << "----------- Y Offset = " <<abs(((hitPositionLab)(1)/1000) - MCInfoHolder->GetASimHit(McHitId).PosInXYZmm.Y())<<'\n';
          std::cout << "Z in TAF = "<< (hitPositionLab)(2)/1000<<" Z in McInfoHolder = " << MCInfoHolder->GetASimHit(McHitId).PosInXYZmm.Z()<<'\n';
          std::cout << "----------- Z Offset = " <<abs(((hitPositionLab)(2)/1000) - MCInfoHolder->GetASimHit(McHitId).PosInXYZmm.Z())<<'\n';

        }
      }

  return;
}
//______________________________________________________________________________
//
#ifdef USETMVA
void MRaw::FillnTupleForTMVA(int    nEvents,
			     double my_theta_mean,
			     double my_phi_mean)
{

  //Function to fill up tree for cluster shape TMVA training to discriminate high theta angle hits

  //Tree variables
  //Data-taking config variables
  float  Conf_ThetaMean    = my_theta_mean;
  float  Conf_PhiMean      = my_phi_mean;
  float  Conf_PhiMeanModPi = my_phi_mean;
  if(Conf_PhiMeanModPi > TMath::Pi())      Conf_PhiMeanModPi -= TMath::Pi();
  else if(Conf_PhiMeanModPi > TMath::Pi()) Conf_PhiMeanModPi += TMath::Pi();

  //Hit reconstruction variables
  float  Hit_NormMinStd;
  float  Hit_NormMaxStd;
  float  Hit_TotChargeADC;
  float  Hit_SeedChargeADC;
  float  Hit_MainAxisPhi;

  //Output tree
  TTree* TMVAtree = new TTree("TMVAtree","TMVA tree");
  //Data-taking config variables
  TMVAtree->Branch("Conf_ThetaMean",    &Conf_ThetaMean,     "Conf_ThetaMean/F");
  TMVAtree->Branch("Conf_PhiMean",      &Conf_PhiMean,       "Conf_PhiMean/F");
  TMVAtree->Branch("Conf_PhiMeanModPi", &Conf_PhiMeanModPi,  "Conf_PhiMeanModPi/F");
  //Hit reconstruction variables
  TMVAtree->Branch("Hit_NormMinStd",    &Hit_NormMinStd,     "Hit_NormMinStd/F");
  TMVAtree->Branch("Hit_NormMaxStd",    &Hit_NormMaxStd,     "Hit_NormMaxStd/F");
  TMVAtree->Branch("Hit_TotChargeADC",  &Hit_TotChargeADC,   "Hit_TotChargeADC/F");
  TMVAtree->Branch("Hit_SeedChargeADC", &Hit_SeedChargeADC,  "Hit_SeedChargeADC/F");
  TMVAtree->Branch("Hit_MainAxisPhi",   &Hit_MainAxisPhi,    "Hit_MainAxisPhi/F");

  fSession->SetEvents(nEvents);

  DTracker *tTracker = fSession->GetTracker();
  Int_t nPlanes  = tTracker->GetPlanesN();
  DPlane* tPlane = NULL;
  DHit *aHit     = NULL;

  Int_t nHitsReconstructed = 0;

  TH2F **hHitMap                    = new TH2F*[nPlanes];
  TH1F **hHitPixMult                = new TH1F*[nPlanes];
  TH1F **hNHitsPerEvent             = new TH1F*[nPlanes];
  TH1F **hHitSeedSN                 = new TH1F*[nPlanes];
  TH1F **hHitNeighbourSN            = new TH1F*[nPlanes];
  TH2F **hHitSeedVsNeighbourSN      = new TH2F*[nPlanes];
  TH1F **hHitCharge                 = new TH1F*[nPlanes];
  TH1F **hHitSeedCharge             = new TH1F*[nPlanes];
  TH1F **hHitNeighbourCharge        = new TH1F*[nPlanes];
  TH2F **hHitSeedVsNeighbourCharge  = new TH2F*[nPlanes];
  TH2F **hHitSeedSNVsSeedCharge     = new TH2F*[nPlanes];
  TH2F **hHitColRowLength           = new TH2F*[nPlanes];
  TH1F **hFracChargeOut7x7Window    = new TH1F*[nPlanes];

  TH1F **hHitNormMinStd             = new TH1F*[nPlanes];
  TH1F **hHitNormMaxStd             = new TH1F*[nPlanes];
  TH2F **hHitNormMinStdVsNormMaxStd = new TH2F*[nPlanes];
  TH1F **hMainAxisPhi               = new TH1F*[nPlanes];

  TCanvas **cHitProperties = new TCanvas*[nPlanes];
  TCanvas **cMCAProperties = new TCanvas*[nPlanes];
  Char_t name[50], title[100];
  int counter_planes_pairs = 0;
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    tPlane = tTracker->GetPlane(iPlane);

    int Ntimes = 2;
    int NbinsU = tPlane->GetStripsNu()*Ntimes;
    double RU[2];
    RU[0] = -0.5*tPlane->GetStripsNu()*tPlane->GetStripPitch()(0);
    RU[1] = +0.5*tPlane->GetStripsNu()*tPlane->GetStripPitch()(0);
    int NbinsV = tPlane->GetStripsNv()*Ntimes;
    double RV[2];
    RV[0] = -0.5*tPlane->GetStripsNv()*tPlane->GetStripPitch()(1);
    RV[1] = +0.5*tPlane->GetStripsNv()*tPlane->GetStripPitch()(1);

    // -- Histo with microns
    sprintf( name, "hhitmappl%d", iPlane);
    sprintf( title, "Hit map of plane %d - %s;X (#mum);Y (#mum)", iPlane, tPlane->GetPlanePurpose());
    // check if the histo is existing or not
    hHitMap[iPlane-1] = new TH2F(name, title,
				 NbinsU, RU[0], RU[1],
				 NbinsV, RV[0], RV[1]);
    hHitMap[iPlane-1]->SetMarkerStyle(20);
    hHitMap[iPlane-1]->SetMarkerSize(.1);
    hHitMap[iPlane-1]->SetMarkerColor(1);
    hHitMap[iPlane-1]->SetStats(kFALSE);
    hHitMap[iPlane-1]->GetXaxis()->CenterTitle(true);
    hHitMap[iPlane-1]->GetYaxis()->CenterTitle(true);

    // -- Pixel multiplicity in hits
    Int_t nPixMax = fSession->GetSetup()->GetPlanePar(iPlane).MaxNStrips;
    sprintf( name, "hhitpixpl%d", iPlane);
    sprintf( title, "Pixel multiplicity in hits of plane %d - %s - %s", iPlane, tPlane->GetPlaneName(), tPlane->GetPlanePurpose());
    if(nPixMax == 0) hHitPixMult[iPlane-1] = new TH1F(name, title, 25, 0, 0);
    else             hHitPixMult[iPlane-1] = new TH1F(name, title, nPixMax+1, -.5, nPixMax+0.5);
    hHitPixMult[iPlane-1]->SetXTitle("# pixels in hit");
    hHitPixMult[iPlane-1]->GetXaxis()->CenterTitle(true);

    // -- Number of hits per event
    sprintf( name, "hnhitspereventpl%d", iPlane);
    sprintf( title, "Number of hits per event of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    //hNHitsPerEvent[iPlane-1] = new TH1F(name, title, 100, 0, 0);
    hNHitsPerEvent[iPlane-1] = new TH1F(name, title, 751, -0.5, 750.5); // QL 2015/10/23, High trigger rate
    hNHitsPerEvent[iPlane-1]->SetXTitle("# hits per event");
    hNHitsPerEvent[iPlane-1]->GetXaxis()->CenterTitle(true);

    // -- Analog hit properties
    sprintf( name, "hhitseedsn%d", iPlane);
    sprintf( title, "Hit S/N for seed pixel of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitSeedSN[iPlane-1] = new TH1F(name, title, 200, 0, 0);
    hHitSeedSN[iPlane-1]->SetXTitle("S/N");
    hHitSeedSN[iPlane-1]->GetXaxis()->CenterTitle(true);

    sprintf( name, "hhitneighboursn%d", iPlane);
    sprintf( title, "Hit S/N for neighbour pixels of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitNeighbourSN[iPlane-1] = new TH1F(name, title, 100, 0, 0);
    hHitNeighbourSN[iPlane-1]->SetXTitle("S/N");
    hHitNeighbourSN[iPlane-1]->GetXaxis()->CenterTitle(true);

    sprintf( name, "hhitseedneighbourgsn%d", iPlane);
    sprintf( title, "Hit S/N for neighbour pixels VS seed of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitSeedVsNeighbourSN[iPlane-1] = new TH2F(name, title, 200, 0, 0, 50, 0, 0);
    hHitSeedVsNeighbourSN[iPlane-1]->SetXTitle("seed S/N");
    hHitSeedVsNeighbourSN[iPlane-1]->GetXaxis()->CenterTitle(true);
    hHitSeedVsNeighbourSN[iPlane-1]->SetYTitle("neighbourgs S/N");
    hHitSeedVsNeighbourSN[iPlane-1]->GetYaxis()->CenterTitle(true);

    sprintf( name, "hhitcharge%d", iPlane);
    sprintf( title, "Hit charge of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitCharge[iPlane-1] = new TH1F(name, title, 200, 0, 0);
    hHitCharge[iPlane-1]->SetXTitle("ADC unit");
    hHitCharge[iPlane-1]->GetXaxis()->CenterTitle(true);

    sprintf( name, "hhitseedq%d", iPlane);
    sprintf( title, "Seed pixel charge of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitSeedCharge[iPlane-1] = new TH1F(name, title, 200, 0, 0);
    hHitSeedCharge[iPlane-1]->SetXTitle("charge (ADCu)");
    hHitSeedCharge[iPlane-1]->GetXaxis()->CenterTitle(true);

    sprintf( name, "hhitneighbourq%d", iPlane);
    sprintf( title, "Neighbour pixels charge of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitNeighbourCharge[iPlane-1] = new TH1F(name, title, 100, 0, 0);
    hHitNeighbourCharge[iPlane-1]->SetXTitle("charge (ADCu)");
    hHitNeighbourCharge[iPlane-1]->GetXaxis()->CenterTitle(true);

    sprintf( name, "hhitseedneighbourgq%d", iPlane);
    sprintf( title, "Neighbour pixels VS seed charge of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitSeedVsNeighbourCharge[iPlane-1] = new TH2F(name, title, 100, 0, 0, 50, 0, 0);
    hHitSeedVsNeighbourCharge[iPlane-1]->SetXTitle("seed charge (ADCu)");
    hHitSeedVsNeighbourCharge[iPlane-1]->GetXaxis()->CenterTitle(true);
    hHitSeedVsNeighbourCharge[iPlane-1]->SetYTitle("neighbourgs charge (ADCu)");
    hHitSeedVsNeighbourCharge[iPlane-1]->GetYaxis()->CenterTitle(true);

    sprintf( name, "hhitseedsnseedq%d", iPlane);
    sprintf( title, "Seed pixel S/N VS seed charge of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitSeedSNVsSeedCharge[iPlane-1] = new TH2F(name, title, 100, 0, 0, 100, 0, 0);
    hHitSeedSNVsSeedCharge[iPlane-1]->SetXTitle("seed charge (ADCu)");
    hHitSeedSNVsSeedCharge[iPlane-1]->GetXaxis()->CenterTitle(true);
    hHitSeedSNVsSeedCharge[iPlane-1]->SetYTitle("seed S/N");
    hHitSeedSNVsSeedCharge[iPlane-1]->GetYaxis()->CenterTitle(true);

    int NtimesRMS = 20;
    double MaxPitch = TMath::Max(tPlane->GetStripPitch()(0),tPlane->GetStripPitch()(1));
    double RMSmax   = MaxPitch*NtimesRMS;
    double RMS_step = MaxPitch/15;
    int NbinsRMS = int(RMSmax/RMS_step);

    sprintf( name, "hHitNormMinStd_%d", iPlane);
    sprintf( title, "Hit MCA RMS_{min} of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitNormMinStd[iPlane-1] = new TH1F(name, title, NbinsRMS, 0, RMSmax);
    hHitNormMinStd[iPlane-1]->SetXTitle("RMS_{min} (#mum)");
    hHitNormMinStd[iPlane-1]->GetXaxis()->CenterTitle(true);

    sprintf( name, "hHitNormMaxStd_%d", iPlane);
    sprintf( title, "Hit MCA RMS_{max} of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitNormMaxStd[iPlane-1] = new TH1F(name, title, NbinsRMS, 0, RMSmax);
    hHitNormMaxStd[iPlane-1]->SetXTitle("RMS_{max} (#mum)");
    hHitNormMaxStd[iPlane-1]->GetXaxis()->CenterTitle(true);

    sprintf( name, "hHitNormMinStdVsNormMaxStd_%d", iPlane);
    sprintf( title, "Hit MCA RMS_{min} vs RMS_{max} of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitNormMinStdVsNormMaxStd[iPlane-1] = new TH2F(name, title, NbinsRMS, 0, RMSmax,NbinsRMS, 0, RMSmax);
    hHitNormMinStdVsNormMaxStd[iPlane-1]->SetXTitle("RMS_{max} (#mum)");
    hHitNormMinStdVsNormMaxStd[iPlane-1]->GetXaxis()->CenterTitle(true);
    hHitNormMinStdVsNormMaxStd[iPlane-1]->SetYTitle("RMS_{min} (#mum)");
    hHitNormMinStdVsNormMaxStd[iPlane-1]->GetYaxis()->CenterTitle(true);
    hHitNormMinStdVsNormMaxStd[iPlane-1]->SetStats(false);

    double Step_phi = 5.0; //deg
    double R_phi[2];
    R_phi[0] =   0.0;
    R_phi[1] = 180.0;
    int Nbins_Phi = int((R_phi[1] - R_phi[0])/Step_phi);

    sprintf( name, "hMainAxisPhi_%d", iPlane);
    sprintf( title, "Hit MCA main axis #phi of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hMainAxisPhi[iPlane-1] = new TH1F(name, title, Nbins_Phi, R_phi[0], R_phi[1]);
    hMainAxisPhi[iPlane-1]->SetXTitle("MCA #phi (deg)");

    int NMaxColsPlot = 20;
    double R_ColRowLength[2];
    R_ColRowLength[0] =  -0.5;
    R_ColRowLength[1] =  NMaxColsPlot + 0.5;
    sprintf( name, "hHitColRowLength_%d", iPlane);
    sprintf( title, "Hit Col/Row Length of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hHitColRowLength[iPlane-1] = new TH2F(name, title,
					  NMaxColsPlot+1,R_ColRowLength[0],R_ColRowLength[1],
					  NMaxColsPlot+1,R_ColRowLength[0],R_ColRowLength[1]);
    hHitColRowLength[iPlane-1]->SetXTitle("Col Lenth");
    hHitColRowLength[iPlane-1]->SetYTitle("Row Lenth");

    sprintf( name, "hFracChargeOut7x7Window_%d", iPlane);
    sprintf( title, "Hit Fraction of charge outside 7x7 window of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    hFracChargeOut7x7Window[iPlane-1] = new TH1F(name, title, 40,0.0,100.0);
    hFracChargeOut7x7Window[iPlane-1]->SetXTitle("Frac Chatge out 7x7 Window (%)");
  }

  int Fiducial_Region  = 2;
  int TheNEvts_counter = 0;
  //Loop over the requested number of events
  std::vector<DPixel*> *aList;
  for( Int_t iEvt=0; iEvt < nEvents; iEvt++) {
    if( !(fSession->NextRawEvent()) ) break;
    tTracker->Update();

    for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
      tPlane = tTracker->GetPlane(iPlane);

      int NGoodHits = 0;

      for( Int_t iHit=1; iHit<=tPlane->GetHitsN(); iHit++) { //loop on hits (starts at 1 !!)
	DHit* aHit = (DHit*)tPlane->GetHit(iHit);
	aHit->DoMCA();
	aHit->CalcHitLimits();
	Hit_NormMinStd    = aHit->GetNormMinStd();
	Hit_NormMaxStd    = aHit->GetNormMaxStd();
	Hit_MainAxisPhi   = aHit->GetMainAxisPhi();
	Hit_TotChargeADC  = aHit->GetClusterPulseSum();
	Hit_SeedChargeADC = aHit->GetPulseHeight(0);

	//Some Cuts
	if(aHit->GetColMin() <= Fiducial_Region-1) continue;
	if(aHit->GetColMax() >= tPlane->GetStripsNu() - (Fiducial_Region-1)) continue;
	if(aHit->GetRowMin() <= Fiducial_Region-1) continue;
	if(aHit->GetRowMax() >= tPlane->GetStripsNv() - (Fiducial_Region-1)) continue;
	if(aHit->GetStripsInCluster() >= 200) continue;
	if(Hit_NormMaxStd == -1000) continue;

	//Now fill some histos
	DR3* Pos = aHit->GetPosition();
	hHitMap[iPlane-1]->Fill((*Pos)(0),(*Pos)(1));

	hHitPixMult[iPlane-1]->Fill(aHit->GetStripsInCluster());

	hHitColRowLength[iPlane-1]->Fill(aHit->GetColMax() - aHit->GetColMin() + 1,aHit->GetRowMax() - aHit->GetRowMin() + 1);

	double Charge_Outside_7x7_window = 0.0;
	int col_seed = -1,row_seed = -1;
	for(int ipix=0;ipix < aHit->GetStripsInCluster();ipix++) {
	  int col,row;
	  double charge;
	  if(tPlane->GetReadout() > 100) {
	    vector<DPixel*> *aList = tPlane->GetListOfPixels();
	    col    = aList->at(aHit->GetIndexInOriginalList(ipix))->GetPixelColumn();
	    row    = aList->at(aHit->GetIndexInOriginalList(ipix))->GetPixelLine();
	    charge = aList->at(aHit->GetIndexInOriginalList(ipix))->GetPulseHeight();
	  }
	  else {
	    int idex_pixel   = aHit->GetIndex(ipix);
	    col    = idex_pixel%tPlane->GetStripsNu();
	    row    = idex_pixel/tPlane->GetStripsNu();
	    charge = tPlane->GetStrip(idex_pixel)->GetPulseHeight();
	  }

	  if(ipix == 0) {
	    col_seed = col;
	    row_seed = row;
	    continue;
	  }

	  if(abs(col_seed - col) <= 3 && abs(row_seed - row) <= 3) continue;
	  Charge_Outside_7x7_window += charge;
	}

	if(tPlane->GetAnalysisMode() < 3) { // analog output
	  hHitSeedSN[iPlane-1]->Fill(aHit->GetSNseed());
	  hHitNeighbourSN[iPlane-1]->Fill(aHit->GetSNneighbour());
	  hHitSeedVsNeighbourSN[iPlane-1]->Fill(aHit->GetSNseed(),
						aHit->GetSNneighbour());

	  hHitCharge[iPlane-1]->Fill(aHit->GetClusterPulseSum());
	  hHitSeedCharge[iPlane-1]->Fill(aHit->GetPulseHeight(0));
	  hHitNeighbourCharge[iPlane-1]->Fill(aHit->GetClusterAreaPulseSum());
	  hHitSeedVsNeighbourCharge[iPlane-1]->Fill(aHit->GetPulseHeight(0),
						    aHit->GetClusterAreaPulseSum());

	  hHitSeedSNVsSeedCharge[iPlane-1]->Fill(aHit->GetPulseHeight(0),
						 aHit->GetSNseed());
	}

	hHitNormMinStd[iPlane-1]->Fill(Hit_NormMinStd);
	hHitNormMaxStd[iPlane-1]->Fill(Hit_NormMaxStd);
	hHitNormMinStdVsNormMaxStd[iPlane-1]->Fill(Hit_NormMaxStd,Hit_NormMinStd);
	hMainAxisPhi[iPlane-1]->Fill(Hit_MainAxisPhi*180.0/TMath::Pi());
	hFracChargeOut7x7Window[iPlane-1]->Fill(100*Charge_Outside_7x7_window/aHit->GetClusterPulseSum());

	TMVAtree->Fill();
	NGoodHits++;
      } //end loop on hits

      hNHitsPerEvent[iPlane-1]->Fill(NGoodHits);
      nHitsReconstructed += NGoodHits;

    } //end of loop on planes
  } // END LOOP ON EVENTS

  fSession->GetDataAcquisition()->PrintStatistics();
  tTracker->PrintStatistics();

  double MaxAxisRanges = -1.0e+20;

  TCanvas *ccumulhit;
  TObject* g = gROOT->FindObject("ccumulhit") ;
  if(g) ccumulhit = (TCanvas*)g;
  else  ccumulhit = new TCanvas("ccumulhit", "Cumulate Hits", 5, 5,800,700);
  ccumulhit->Clear();
  ccumulhit->UseCurrentStyle();
  TPaveLabel* label = new TPaveLabel();
  Char_t canvasTitle[200];
  sprintf(canvasTitle, "Run %d, cumul over %d events", fSession->GetRunNumber(), nEvents);
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad = new TPad("pad","",0.,0.,1.,0.965);
  pad->Draw();
  if(nPlanes > 6) pad->Divide( (Int_t)ceil(nPlanes/4.), (nPlanes>4)?4:nPlanes);  // LC 4 lines instead of 2.
  else            pad->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);
  for(Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    pad->cd(iPlane);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    gPad->SetBottomMargin(0.20);
    gPad->SetLeftMargin(0.15);
    gPad->SetRightMargin(0.15);

    hHitMap[iPlane-1]->DrawCopy("colz");
  }
  ccumulhit->Update();

  // this canvas is intended mainly for binary outputs
  TCanvas *ccumulhit2;
  g = gROOT->FindObject("ccumulhit2") ;
  if(g) ccumulhit2 = (TCanvas*)g;
  else  ccumulhit2 = new TCanvas("ccumulhit2", "Pixel multiplicitiy in hits", 5, 5,800,700);
  ccumulhit2->Clear();
  ccumulhit2->UseCurrentStyle();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad2 = new TPad("pad2","",0.,0.,1.,0.965);
  pad2->Draw();
  pad2->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);
  for(Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    pad2->cd(iPlane);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    gPad->SetBottomMargin(0.20);
    gPad->SetLeftMargin(0.15);

    double MyMaxRange = -999.0;
    for(int i=0;i<hHitPixMult[iPlane-1]->GetXaxis()->GetNbins();i++) {
      int index = hHitPixMult[iPlane-1]->GetXaxis()->GetNbins() - i;
      if(hHitPixMult[iPlane-1]->GetBinContent(index) > 0.0) {
	MyMaxRange  = hHitPixMult[iPlane-1]->GetBinCenter(index);
	MyMaxRange += 0.5*hHitPixMult[iPlane-1]->GetBinWidth(index);
	break;
      }
    }
    MyMaxRange += 0.10*(MyMaxRange - hHitPixMult[iPlane-1]->GetXaxis()->GetXmin());
    if(MaxAxisRanges < MyMaxRange) MaxAxisRanges = MyMaxRange;
    hHitPixMult[iPlane-1]->SetAxisRange(hHitPixMult[iPlane-1]->GetXaxis()->GetXmin(),
					MyMaxRange,
					"X");

    hHitPixMult[iPlane-1]->DrawCopy();
  }
  ccumulhit2->Update();

  // Number of hits per event
  TCanvas *ccumulhit3;
  g = gROOT->FindObject("ccumulhit3") ;
  if(g) ccumulhit3 = (TCanvas*)g;
  else  ccumulhit3 = new TCanvas("ccumulhit3", "Nb of hits per event", 5, 5,800,700);
  ccumulhit3->Clear();
  ccumulhit3->UseCurrentStyle();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad3 = new TPad("pad3","",0.,0.,1.,0.965);
  pad3->Draw();
  pad3->Divide( (Int_t)ceil(nPlanes/2.), (nPlanes>1)?2:1);
  for(Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    pad3->cd(iPlane);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    gPad->SetBottomMargin(0.20);
    gPad->SetLeftMargin(0.15);

    double MyMaxRange = -999.0;
    for(int i=0;i<hNHitsPerEvent[iPlane-1]->GetXaxis()->GetNbins();i++) {
      int index = hNHitsPerEvent[iPlane-1]->GetXaxis()->GetNbins() - i;
      if(hNHitsPerEvent[iPlane-1]->GetBinContent(index) > 0.0) {
	MyMaxRange  = hNHitsPerEvent[iPlane-1]->GetBinCenter(index);
	MyMaxRange += 0.5*hNHitsPerEvent[iPlane-1]->GetBinWidth(index);
	break;
      }
    }
    MyMaxRange += 0.10*(MyMaxRange - hNHitsPerEvent[iPlane-1]->GetXaxis()->GetXmin());
    if(MaxAxisRanges < MyMaxRange) MaxAxisRanges = MyMaxRange;
    hNHitsPerEvent[iPlane-1]->SetAxisRange(hNHitsPerEvent[iPlane-1]->GetXaxis()->GetXmin(),
					   MyMaxRange,
					   "X");
    hNHitsPerEvent[iPlane-1]->DrawCopy();
  }
  ccumulhit3->Update();

  // For analog output sensors
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    tPlane = tTracker->GetPlane(iPlane);
    if(0 < tPlane->GetAnalysisMode() && tPlane->GetAnalysisMode() <= 2) {
      sprintf( name, "chitpl%d", iPlane);
      sprintf( title, "Hit properties of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
      g = gROOT->FindObject(name);
      if(g) cHitProperties[iPlane-1] = (TCanvas*)g;
      else  cHitProperties[iPlane-1] = new TCanvas(name, title, 5, 5+5*(iPlane-1),600,500);

      cHitProperties[iPlane-1]->Clear();
      cHitProperties[iPlane-1]->UseCurrentStyle();
      cHitProperties[iPlane-1]->Divide(3,3);

      cHitProperties[iPlane-1]->cd(1);
      gPad->SetTickx(1);
      gPad->SetTicky(1);
      gPad->SetBottomMargin(0.20);
      gPad->SetLeftMargin(0.15);
      if(hHitSeedSN[iPlane-1]->GetEntries() > 0) hHitSeedSN[iPlane-1]->DrawCopy();

      cHitProperties[iPlane-1]->cd(2);
      gPad->SetTickx(1);
      gPad->SetTicky(1);
      gPad->SetBottomMargin(0.20);
      gPad->SetLeftMargin(0.15);
      if(hHitNeighbourSN[iPlane-1]->GetEntries() > 0) hHitNeighbourSN[iPlane-1]->DrawCopy();

      cHitProperties[iPlane-1]->cd(3);
      gPad->SetTickx(1);
      gPad->SetTicky(1);
      gPad->SetBottomMargin(0.20);
      gPad->SetLeftMargin(0.15);
      gPad->SetRightMargin(0.20);
      if(hHitSeedVsNeighbourSN[iPlane-1]->GetEntries() > 0) hHitSeedVsNeighbourSN[iPlane-1]->DrawCopy("colz");

      cHitProperties[iPlane-1]->cd(4);
      gPad->SetTickx(1);
      gPad->SetTicky(1);
      gPad->SetBottomMargin(0.20);
      gPad->SetLeftMargin(0.15);
      if(hHitCharge[iPlane-1]->GetEntries() > 0) hHitCharge[iPlane-1]->DrawCopy();

      cHitProperties[iPlane-1]->cd(5);
      gPad->SetTickx(1);
      gPad->SetTicky(1);
      gPad->SetBottomMargin(0.20);
      gPad->SetLeftMargin(0.15);
      if(hHitSeedCharge[iPlane-1]->GetEntries() > 0)  hHitSeedCharge[iPlane-1]->DrawCopy();

      cHitProperties[iPlane-1]->cd(6);
      gPad->SetTickx(1);
      gPad->SetTicky(1);
      gPad->SetBottomMargin(0.20);
      gPad->SetLeftMargin(0.15);
      if(hHitNeighbourCharge[iPlane-1]->GetEntries() > 0)  hHitNeighbourCharge[iPlane-1]->DrawCopy();

      cHitProperties[iPlane-1]->cd(7);
      gPad->SetTickx(1);
      gPad->SetTicky(1);
      gPad->SetBottomMargin(0.20);
      gPad->SetLeftMargin(0.15);
      gPad->SetRightMargin(0.20);
      if(hHitSeedVsNeighbourCharge[iPlane-1]->GetEntries() > 0)  hHitSeedVsNeighbourCharge[iPlane-1]->DrawCopy("colz");

      cHitProperties[iPlane-1]->cd(8);
      gPad->SetTickx(1);
      gPad->SetTicky(1);
      gPad->SetBottomMargin(0.20);
      gPad->SetLeftMargin(0.15);
      gPad->SetRightMargin(0.20);
      if( hHitSeedSNVsSeedCharge[iPlane-1]->GetEntries() > 0) hHitSeedSNVsSeedCharge[iPlane-1]->DrawCopy("colz");

      cHitProperties[iPlane-1]->cd(9);
      gPad->SetTickx(1);
      gPad->SetTicky(1);
      gPad->SetBottomMargin(0.20);
      gPad->SetLeftMargin(0.15);
      hNHitsPerEvent[iPlane-1]->DrawCopy();
    }

    sprintf( name, "cMCApl%d", iPlane);
    sprintf( title, "Hit MCA properties of plane %d - %s", iPlane, tPlane->GetPlanePurpose());
    g = gROOT->FindObject(name);
    if(g) cMCAProperties[iPlane-1] = (TCanvas*)g;
    else  cMCAProperties[iPlane-1] = new TCanvas(name, title, 5, 5+5*(iPlane-1),600,500);

    double Frac_limit = 0.99;

    double MyMaxRangeRMSmax = -999.0;
    for(int i=0;i<hHitNormMaxStd[iPlane-1]->GetXaxis()->GetNbins();i++) {
      int index = hHitNormMaxStd[iPlane-1]->GetXaxis()->GetNbins() - i;
      double Frac  = hHitNormMaxStd[iPlane-1]->Integral(1,index);
      Frac        /= hHitNormMaxStd[iPlane-1]->Integral();
      if(Frac < Frac_limit) {
	MyMaxRangeRMSmax  = hHitNormMaxStd[iPlane-1]->GetBinCenter(index);
	MyMaxRangeRMSmax += 0.5*hHitNormMaxStd[iPlane-1]->GetBinWidth(index);
	break;
      }
    }
    MyMaxRangeRMSmax += 0.10*(MyMaxRangeRMSmax - hHitNormMaxStd[iPlane-1]->GetXaxis()->GetXmin());
    hHitNormMaxStd[iPlane-1]->SetAxisRange(hHitNormMaxStd[iPlane-1]->GetXaxis()->GetXmin(),
					   MyMaxRangeRMSmax,
					   "X");

    double MyMaxRangeRMSmin = -999.0;
    for(int i=0;i<hHitNormMinStd[iPlane-1]->GetXaxis()->GetNbins();i++) {
      int index = hHitNormMinStd[iPlane-1]->GetXaxis()->GetNbins() - i;
      double Frac  = hHitNormMinStd[iPlane-1]->Integral(1,index);
      Frac        /= hHitNormMinStd[iPlane-1]->Integral();
      if(Frac < Frac_limit) {
	MyMaxRangeRMSmin  = hHitNormMinStd[iPlane-1]->GetBinCenter(index);
	MyMaxRangeRMSmin += 0.5*hHitNormMinStd[iPlane-1]->GetBinWidth(index);
	break;
      }
    }
    MyMaxRangeRMSmin += 0.10*(MyMaxRangeRMSmin - hHitNormMinStd[iPlane-1]->GetXaxis()->GetXmin());
    hHitNormMinStd[iPlane-1]->SetAxisRange(hHitNormMinStd[iPlane-1]->GetXaxis()->GetXmin(),
					   //MyMaxRangeRMSmin,
					   MyMaxRangeRMSmax,
					   "X");

    hHitNormMinStdVsNormMaxStd[iPlane-1]->SetAxisRange(hHitNormMinStd[iPlane-1]->GetXaxis()->GetXmin(),
						       //MyMaxRangeRMSmin,
						       MyMaxRangeRMSmax,
						       "X");
    hHitNormMinStdVsNormMaxStd[iPlane-1]->SetAxisRange(hHitNormMaxStd[iPlane-1]->GetXaxis()->GetXmin(),
						       MyMaxRangeRMSmax,
						       "Y");

    cMCAProperties[iPlane-1]->Clear();
    cMCAProperties[iPlane-1]->UseCurrentStyle();
    cMCAProperties[iPlane-1]->Divide(2,3);
    cMCAProperties[iPlane-1]->cd(1);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    gPad->SetBottomMargin(0.20);
    gPad->SetLeftMargin(0.15);
    hHitNormMinStd[iPlane-1]->Draw();
    cMCAProperties[iPlane-1]->cd(2);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    gPad->SetBottomMargin(0.20);
    gPad->SetLeftMargin(0.15);
    hHitNormMaxStd[iPlane-1]->Draw();
    cMCAProperties[iPlane-1]->cd(3);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    gPad->SetBottomMargin(0.20);
    gPad->SetLeftMargin(0.18);
    gPad->SetRightMargin(0.15);
    hHitNormMinStdVsNormMaxStd[iPlane-1]->Draw("colz");
    cMCAProperties[iPlane-1]->cd(4);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    gPad->SetBottomMargin(0.20);
    gPad->SetLeftMargin(0.15);
    hMainAxisPhi[iPlane-1]->Draw();
    cMCAProperties[iPlane-1]->cd(5);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    gPad->SetBottomMargin(0.20);
    gPad->SetLeftMargin(0.15);
    gPad->SetRightMargin(0.18);
    int MaxClusSize = 7;
    TLine* l1 = new TLine(0.5,MaxClusSize+0.5,MaxClusSize+0.5,MaxClusSize+0.5);
    l1->SetLineColor(kRed);
    l1->SetLineWidth(2);
    l1->SetLineStyle(2);
    TLine* l2 = new TLine(MaxClusSize+0.5,0.5,MaxClusSize+0.5,MaxClusSize+0.5);
    l2->SetLineColor(kRed);
    l2->SetLineWidth(2);
    l2->SetLineStyle(2);
    hHitColRowLength[iPlane-1]->SetStats(false);
    hHitColRowLength[iPlane-1]->Draw("colz");
    l1->Draw();
    l2->Draw();
    cMCAProperties[iPlane-1]->cd(6);
    gPad->SetTickx(1);
    gPad->SetTicky(1);
    gPad->SetBottomMargin(0.20);
    gPad->SetLeftMargin(0.15);
    gPad->SetLogy(1);
    hFracChargeOut7x7Window[iPlane-1]->Draw();
  }

  // Save canvas and histos
  Char_t rootFile[300];
  sprintf(rootFile,"%sTMVAData_run%d.root",fSession->GetResultDirName().Data(),fSession->GetRunNumber());
  sprintf(rootFile,"%s", fTool.LocalizeDirName( rootFile)); // JB 2011/07/07
  cout << "\n-- Saving histos and canvas into " << rootFile << endl;
  TFile fRoot(rootFile,"RECREATE");
  ccumulhit->Write();
  ccumulhit2->Write();
  ccumulhit3->Write();

  for(Int_t iPlane=1; iPlane<=nPlanes; iPlane++) {
    tPlane = tTracker->GetPlane(iPlane);
    if(0 < tPlane->GetAnalysisMode()) {
      hHitMap[iPlane-1]->Write();
      hHitPixMult[iPlane-1]->Write();
      hNHitsPerEvent[iPlane-1]->Write();

      if(tPlane->GetAnalysisMode() <= 2) {
	hHitSeedSN[iPlane-1]->Write();
	hHitNeighbourSN[iPlane-1]->Write();
	hHitSeedVsNeighbourSN[iPlane-1]->Write();
	hHitCharge[iPlane-1]->Write();
	hHitSeedCharge[iPlane-1]->Write();
	hHitNeighbourCharge[iPlane-1]->Write();
	hHitSeedVsNeighbourCharge[iPlane-1]->Write();
	hHitSeedSNVsSeedCharge[iPlane-1]->Write();
	cHitProperties[iPlane-1]->Write();
      }

      hHitNormMinStd[iPlane-1]->Write();
      hHitNormMaxStd[iPlane-1]->Write();
      hHitNormMinStdVsNormMaxStd[iPlane-1]->Write();
      hMainAxisPhi[iPlane-1]->Write();
      hHitColRowLength[iPlane-1]->Write();
      hFracChargeOut7x7Window[iPlane-1]->Write();
      cMCAProperties[iPlane-1]->Write();

    }
  }
  TMVAtree->Write();
  fRoot.Close();

  return;

}
//______________________________________________________________________________
//
void  MRaw::TrainTMVA(TString myMethodList,
		      TString fname,
		      TString TreeName,
		      TString OutputFile,
		      int Nevts_training,
		      int Nevts_testing)
{


  // This loads the library
  TMVA::Tools::Instance();

  std::map<std::string,int> Use;

  // --- Mutidimensional likelihood and Nearest-Neighbour methods
  Use["PDERS"]           = 0;
  Use["PDEFoam"]         = 0;
  Use["KNN"]             = 0;
  //
  // --- Linear Discriminant Analysis
  Use["LD"]		        = 1;
  //
  // --- Function Discriminant analysis
  Use["FDA_GA"]          = 0;
  Use["FDA_MC"]          = 0;
  Use["FDA_MT"]          = 0;
  Use["FDA_GAMT"]        = 0;
  //
  // --- Neural Network
  Use["MLP"]             = 1;
  //
  // --- Support Vector Machine
  Use["SVM"]             = 0;
  //
  // --- Boosted Decision Trees
  Use["BDT"]             = 0;
  Use["BDTG"]            = 1;
  // ---------------------------------------------------------------

  cout << endl;
  cout << "==> Start TMVARegression" << endl;
  // Select methods (don't look at this code - not of interest)
  if(myMethodList != "") {
    for(std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;
    vector<TString> mlist = gTools().SplitString( myMethodList, ',' );
    for(UInt_t i=0; i<mlist.size(); i++) {
      std::string regMethod(mlist[i]);

      if(Use.find(regMethod) == Use.end()) {
	cout << "Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << endl;
	for(std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) cout << it->first << " ";
	cout << endl;

	return;
      }
      Use[regMethod] = 1;
    }
  }

  TString  OutFileName = OutputFile + TString(".root");

  if(Nevts_training > 0) {
    // Default MVA methods to be trained + tested

    // --------------------------------------------------------------------------------------------------
    // --- Here the preparation phase begins

    // Create a new root output file
    TFile* outputFile = TFile::Open( OutFileName.Data(), "RECREATE" );

    // Create the factory object. Later you can choose the methods
    // whose performance you'd like to investigate. The factory will
    // then run the performance analysis for you.
    //
    // The first argument is the base of the name of all the
    // weightfiles in the directory weight/
    //
    // The second argument is the output file for the training results
    // All TMVA output can be suppressed by removing the "!" (not) in
    // front of the "Silent" argument in the option string
    TMVA::Factory *factory = new TMVA::Factory( "TMVARegression", outputFile,
                                                "!V:!Silent:Color:DrawProgressBar" );

    // Define the input variables that shall be used for the MVA training
    // note that you may also use variable expressions, such as: "3*var1/var2*abs(var3)"
    // [all types of expressions that can also be parsed by TTree::Draw( "expression" )]
    // factory->AddVariable( "ST_TMVA_ThetaIncidentAngle","ThetaIncidentAngle", 'F' );

    factory->AddVariable( "Hit_NormMinStd",    "Hit_NormMinStd",    "#mum", 'F' );
    factory->AddVariable( "Hit_NormMaxStd",    "Hit_NormMaxStd",    "#mum", 'F' );
    factory->AddVariable( "Hit_TotChargeADC",  "Hit_TotChargeADC",  "ADC",  'F' );
    factory->AddVariable( "Hit_SeedChargeADC", "Hit_SeedChargeADC", "ADC",  'F' );

    //   factory->AddVariable( "", "", "units", 'F' );
    //  factory->AddVariable( "var2", "Variable 2", "units", 'F' );

    // You can add so-called "Spectator variables", which are not used in the MVA training,
    // but will appear in the final "TestTree" produced by TMVA. This TestTree will contain the
    // input variables, the response values of all trained MVAs, and the spectator variables
    // factory->AddSpectator( "STparticles_ThetaIncidentAngle",  "STparticles_ThetaIncidentAngle", "units", 'F' );
    //   factory->AddSpectator( "ST_TMVA_PhiIncidentAngle",  "STparticles_PhiIncidentAngle", "units", 'F' );
    factory->AddSpectator( "Conf_PhiMean",       "Conf_PhiMean",      "deg", 'F' );
    factory->AddSpectator( "Conf_PhiMeanModPi",  "Conf_PhiMeanModPi", "deg", 'F' );
    factory->AddSpectator( "Hit_MainAxisPhi",    "Hit_MainAxisPhi",   "deg", 'F' );

    //factory->AddSpectator( "spec1:=var1*2",  "Spectator 1", "units", 'F' );
    //factory->AddSpectator( "spec2:=var1*3",  "Spectator 2", "units", 'F' );

    // Add the variable carrying the regression target
    factory->AddTarget( "Conf_ThetaMean" );
    //factory->AddTarget( "Conf_PhiMean" );

    // --------------------------------------------------------------------------------------------------

    // global event weights per tree (see below for setting event-wise weights)
    Double_t regWeight  = 1.0;

    std::vector<TString> MyFileList;
    MyFileList.clear();
    if(fname.EndsWith(".root")) MyFileList.push_back(fname);
    else if(fname.EndsWith(".txt")) {
      char ccc[300];
      ifstream fp(fname.Data());
      while(fp >> ccc) MyFileList.push_back(TString(ccc));
      fp.close();
    }
    else {
      cout << endl;
      cout << "MRaw::TrainTMVA::ERROR  intput data files doesn't end either with .root or .txt. Check your inputs. Exiting now!!!" << endl;
      cout << endl;
    }

    const int Nfiles(MyFileList.size());
    TTree *regTree[Nfiles];
    for(int ifile=0;ifile<Nfiles;ifile++) {
      TFile *input(0);
      if(!gSystem->AccessPathName(MyFileList[ifile].Data())) input = TFile::Open(MyFileList[ifile].Data()); // check if file in local directory exists
      if(!input) {
        cout << "ERROR: could not open data file " << MyFileList[ifile].Data() << endl;
        exit(1);
      }
      cout << "--- TMVARegression           : Using input file: " << input->GetName() << endl;

      regTree[ifile] = (TTree*)input->Get(TreeName.Data());

      factory->AddRegressionTree(regTree[ifile], regWeight);
    }

    // This would set individual event weights (the variables defined in the
    // expression need to exist in the original TTree)
    double Theta_Cut       = 45.0;
    double Frac_High_theta = 0.9;

    if(Theta_Cut < 0.0 || Theta_Cut > 180.0) {
      cout << endl;
      cout << "Theta cut value " << Theta_Cut << ", is outside (0,180.0) range. Exiting now!!!" << endl;
      cout << endl;
      assert(false);
    }
    if(Frac_High_theta < 0.0 || Frac_High_theta > 1.0) {
      cout << endl;
      cout << "Weight for high theta, Frac = " << Frac_High_theta << ", is outside (0,1) range. Exiting now!!!" << endl;
      cout << endl;
      assert(false);
    }
    char ytitle[100];
    sprintf(ytitle,"%.2f",1.0 - Frac_High_theta);
    TString WeightExpression  = TString(ytitle) + TString("*(Conf_ThetaMean <= ");
    sprintf(ytitle,"%.2f",Theta_Cut);
    WeightExpression         += TString(ytitle) + TString(") + (Conf_ThetaMean > ") + TString(ytitle) + TString(")*");
    sprintf(ytitle,"%.2f",Frac_High_theta);
    WeightExpression         += TString(ytitle);

    cout << endl;
    cout << "Weight Expression for regression: " << WeightExpression.Data() << endl;
    cout << endl;
    //factory->SetWeightExpression( "0.1+(Conf_ThetaMean > 45.0)*0.9", "Regression" );
    factory->SetWeightExpression(WeightExpression.Data(), "Regression" );
    //factory->SetWeightExpression( "var1", "Regression" );

    // Apply additional cuts on the signal and background samples (can be different)
    //TCut mycut = ""; // for example: TCut mycut = "abs(var1)<0.5 && abs(var2-0.5)<1";
    //TCut mycut = "STparticles_ThetaIncidentAngle>0.8"; // for example: TCut mycut = "abs(var1)<0.5 && abs(var2-0.5)<1";
    TCut mycut = "Hit_NormMaxStd < 10000"; // for example: TCut mycut = "abs(var1)<0.5 && abs(var2-0.5)<1";

    // tell the factory to use all remaining events in the trees after training for testing:
    TString TrainingMethodPre = TString("nTrain_Regression=") + long(Nevts_training) + TString(":nTest_Regression=") + long(Nevts_testing) + TString(":SplitMode=Random:NormMode=NumEvents:!V");
    cout << endl;
    cout << "Training and Testing config: " << TrainingMethodPre.Data() << endl;
    cout << endl;
    factory->PrepareTrainingAndTestTree(mycut,
				        TrainingMethodPre.Data());

    // ---- Book MVA methods
    //
    // please lookup the various method configuration options in the corresponding cxx files, eg:
    // src/MethoCuts.cxx, etc, or here: http://tmva.sourceforge.net/optionRef.html
    // it is possible to preset ranges in the option string in which the cut optimisation should be done:
    // "...:CutRangeMin[2]=-1:CutRangeMax[2]=1"...", where [2] is the third input variable

    // PDE - RS method
    if(Use["PDERS"])   factory->BookMethod( TMVA::Types::kPDERS,
                                            "PDERS",
                                            "!H:!V:NormTree=T:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=40:NEventsMax=60:VarTransform=None" );

    if(Use["PDEFoam"]) factory->BookMethod( TMVA::Types::kPDEFoam,
                                            "PDEFoam",
			                    "!H:!V:MultiTargetRegression=F:TargetSelection=Mpv:TailCut=0.001:VolFrac=0.0666:nActiveCells=500:nSampl=2000:nBin=5:Compress=T:Kernel=None:Nmin=10:VarTransform=None" );

    // K-Nearest Neighbour classifier (KNN)
    if(Use["KNN"])     factory->BookMethod( TMVA::Types::kKNN,
                                            "KNN",
	  				    "nkNN=20:ScaleFrac=0.8:SigmaFact=1.0:Kernel=Gaus:UseKernel=F:UseWeight=T:!Trim" );

    // Linear discriminant
    if(Use["LD"])      factory->BookMethod( TMVA::Types::kLD,
                                            "LD",
				  	    "!H:!V:VarTransform=None" );

    // Function discrimination analysis (FDA) -- test of various fitters - the recommended one is Minuit (or GA or SA)
    if(Use["FDA_MC"])  factory->BookMethod( TMVA::Types::kFDA,
                                            "FDA_MC",
					    "!H:!V:Formula=(0)+(1)*x0+(2)*x1:ParRanges=(-100,100);(-100,100);(-100,100):FitMethod=MC:SampleSize=100000:Sigma=0.1:VarTransform=D" );

    // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options) .. the formula of this example is good for parabolass
    if(Use["FDA_GA"])  factory->BookMethod( TMVA::Types::kFDA,
                                            "FDA_GA",
					    "!H:!V:Formula=(0)+(1)*x0+(2)*x1:ParRanges=(-100,100);(-100,100);(-100,100):FitMethod=GA:PopSize=100:Cycles=3:Steps=30:Trim=True:SaveBestGen=1:VarTransform=Norm" );

    if(Use["FDA_MT"])  factory->BookMethod( TMVA::Types::kFDA,
                                            "FDA_MT",
					    "!H:!V:Formula=(0)+(1)*x0+(2)*x1:ParRanges=(-100,100);(-100,100);(-100,100);(-10,10):FitMethod=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=2:UseImprove:UseMinos:SetBatch" );

    if(Use["FDA_GAMT"])  factory->BookMethod( TMVA::Types::kFDA,
                                              "FDA_GAMT",
					      "!H:!V:Formula=(0)+(1)*x0+(2)*x1:ParRanges=(-100,100);(-100,100);(-100,100):FitMethod=GA:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:Cycles=1:PopSize=5:Steps=5:Trim" );

    // Neural network (MLP)
    if(Use["MLP"])       factory->BookMethod( TMVA::Types::kMLP,
                                              "MLP",
					      "!H:!V:VarTransform=Norm:NeuronType=tanh:NCycles=20000:HiddenLayers=N+10:TestRate=6:TrainingMethod=BFGS:Sampling=0.3:SamplingEpoch=0.8:ConvergenceImprove=1e-6:ConvergenceTests=15:!UseRegulator" );

    // Support Vector Machine
    if(Use["SVM"])        factory->BookMethod( TMVA::Types::kSVM, "SVM", "Gamma=0.25:Tol=0.001:VarTransform=Norm" );

    // Boosted Decision Trees
    if(Use["BDT"])        factory->BookMethod( TMVA::Types::kBDT,
                                               "BDT",
                                               "!H:!V:NTrees=100:MinNodeSize=1.0%:BoostType=AdaBoostR2:SeparationType=RegressionVariance:nCuts=20:PruneMethod=CostComplexity:PruneStrength=30" );

    if(Use["BDTG"])       factory->BookMethod( TMVA::Types::kBDT,
                                               "BDTG",
					       "!H:!V:NTrees=2000::BoostType=Grad:Shrinkage=0.1:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=3:MaxDepth=4" );

    // --------------------------------------------------------------------------------------------------

    // ---- Now you can tell the factory to train, test, and evaluate the MVAs

    // Train MVAs using the set of training events
    factory->TrainAllMethods();

    // ---- Evaluate all MVAs using the set of test events
    factory->TestAllMethods();

    // ----- Evaluate and compare performance of all configured MVAs
    factory->EvaluateAllMethods();

    // --------------------------------------------------------------

    // Save the output
    outputFile->Close();

    TString OutWeightFileName;
    if(Use["MLP"]) {
      TString command;
      OutWeightFileName = OutputFile + TString(".weights.xml");
      command = TString("mv  weights/TMVARegression_MLP.weights.xml   ") + OutWeightFileName;
      gSystem->Exec(command.Data());
    }

    std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
    if(Use["MLP"]) std::cout << "==> Wrote weights file: " << OutWeightFileName.Data() << std::endl;
    std::cout << "==> TMVARegression is done!" << std::endl;

    delete factory;


  }

  // Launch the GUI for the root macros
  if (!gROOT->IsBatch()) {
    TFile *file(0);
    if(!gSystem->AccessPathName( OutFileName.Data() )) file = TFile::Open( OutFileName.Data() ); // check if file in local directory exists
    if(!file) {
      cout << "ERROR: could not open data file " << OutFileName.Data() << endl;
      exit(1);
    }

    TMVARegGui(OutFileName.Data());
  }

  //Do some plots with the regression output
  float theta_Step_in = 1.0; //deg
  float R_theta_in[2];
  R_theta_in[0] =  0.0; //deg
  R_theta_in[1] = 90.0; //deg
  const int Nbins_theta_in(int((R_theta_in[1] - R_theta_in[0])/theta_Step_in));

  float theta_Step_out = 5.0; //deg
  float R_theta_out[2];
  R_theta_out[0] =  0.0; //deg
  R_theta_out[1] = 90.0; //deg
  int Nbins_theta_out = int((R_theta_out[1] - R_theta_out[0])/theta_Step_out);

  float theta_Step_outdel = 5.0; //deg
  float R_theta_outdel[2];
  R_theta_outdel[0] = -90.0; //deg
  R_theta_outdel[1] = +90.0; //deg
  int Nbins_theta_outdel = int((R_theta_outdel[1] - R_theta_outdel[0])/theta_Step_outdel);

  TH2F* h_ThetaIncident_vs_MLPregressionOut = new TH2F("h_ThetaIncident_vs_MLPregressionOut",
                                                       "MLP reg. output vs #theta incident",
						       Nbins_theta_in, R_theta_in[0], R_theta_in[1],
						       Nbins_theta_out,R_theta_out[0],R_theta_out[1]);
  h_ThetaIncident_vs_MLPregressionOut->SetXTitle("#theta_{inc} (deg)");
  h_ThetaIncident_vs_MLPregressionOut->SetYTitle("MLP-out (deg)");
  h_ThetaIncident_vs_MLPregressionOut->SetStats(false);

  TH2F* h_ThetaIncident_vs_DeltaMLPregressionOut = new TH2F("h_ThetaIncident_vs_DeltaMLPregressionOut",
							    "(MLP reg. output - #theta incident) vs #theta incident",
							    Nbins_theta_in,R_theta_in[0],R_theta_in[1],
							    Nbins_theta_outdel,R_theta_outdel[0],R_theta_outdel[1]);
  h_ThetaIncident_vs_DeltaMLPregressionOut->SetXTitle("#theta_{inc} (deg)");
  h_ThetaIncident_vs_DeltaMLPregressionOut->SetYTitle("(MLP-out - #theta_{inc}) (deg)");
  h_ThetaIncident_vs_DeltaMLPregressionOut->SetStats(false);

  TH1F* h_ThetaIncident_vs_MeanMLPregressionOut = new TH1F("h_ThetaIncident_vs_MeanMLPregressionOut",
							   "MLP reg. output profile vs #theta incident",
							   Nbins_theta_in, R_theta_in[0], R_theta_in[1]);
  h_ThetaIncident_vs_MeanMLPregressionOut->SetXTitle("#theta_{inc} (deg)");
  h_ThetaIncident_vs_MeanMLPregressionOut->SetYTitle("MLP-out (deg)");
  h_ThetaIncident_vs_MeanMLPregressionOut->SetStats(false);
  h_ThetaIncident_vs_MeanMLPregressionOut->SetMinimum(h_ThetaIncident_vs_MLPregressionOut->GetYaxis()->GetXmin());
  h_ThetaIncident_vs_MeanMLPregressionOut->SetMaximum(h_ThetaIncident_vs_MLPregressionOut->GetYaxis()->GetXmax());
  h_ThetaIncident_vs_MeanMLPregressionOut->SetLineColor(kBlue);
  h_ThetaIncident_vs_MeanMLPregressionOut->SetLineWidth(2);

  TH1F* h_ThetaIncident_vs_MeanDeltaMLPregressionOut = new TH1F("h_ThetaIncident_vs_MeanDeltaMLPregressionOut",
								"(MLP reg. output - #theta incident) profile vs #theta incident",
							        Nbins_theta_in,R_theta_in[0],R_theta_in[1]);
  h_ThetaIncident_vs_MeanDeltaMLPregressionOut->SetXTitle("#theta_{inc} (deg)");
  h_ThetaIncident_vs_MeanDeltaMLPregressionOut->SetYTitle("(MLP-out - #theta_{inc}) (deg)");
  h_ThetaIncident_vs_MeanDeltaMLPregressionOut->SetStats(false);
  h_ThetaIncident_vs_MeanDeltaMLPregressionOut->SetMinimum(h_ThetaIncident_vs_DeltaMLPregressionOut->GetYaxis()->GetXmin());
  h_ThetaIncident_vs_MeanDeltaMLPregressionOut->SetMaximum(h_ThetaIncident_vs_DeltaMLPregressionOut->GetYaxis()->GetXmax());
  h_ThetaIncident_vs_MeanDeltaMLPregressionOut->SetLineColor(kBlue);
  h_ThetaIncident_vs_MeanDeltaMLPregressionOut->SetLineWidth(2);

  double MLPOutMean[Nbins_theta_in];
  double MLPOutRMS[Nbins_theta_in];
  double DeltaMLPOutMean[Nbins_theta_in];
  int    Nhits[Nbins_theta_in];
  for(int i=0;i<Nbins_theta_in;i++) MLPOutMean[i] = DeltaMLPOutMean[i] = MLPOutRMS[i] = Nhits[i] = 0;

  std::vector<TString> FileList;
  FileList.clear();
  if(fname.EndsWith(".root")) FileList.push_back(fname);
  else if(fname.EndsWith(".txt")) {
    char ccc[300];
    ifstream fp(fname.Data());
    while(fp >> ccc) FileList.push_back(TString(ccc));
    fp.close();
  }

  float  Hit_NormMinStd;
  float  Hit_NormMaxStd;
  float  Hit_TotChargeADC;
  float  Hit_SeedChargeADC;
  float  Hit_MainAxisPhi;
  float  Conf_PhiMean;
  float  Conf_PhiMeanModPi;
  float  Conf_ThetaMean;

  TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );

  reader->AddVariable( "Hit_NormMinStd",     &Hit_NormMinStd);
  reader->AddVariable( "Hit_NormMaxStd",     &Hit_NormMaxStd);
  reader->AddVariable( "Hit_TotChargeADC",   &Hit_TotChargeADC);
  reader->AddVariable( "Hit_SeedChargeADC",  &Hit_SeedChargeADC);

  reader->AddSpectator( "Conf_PhiMean",      &Conf_PhiMean);
  reader->AddSpectator( "Conf_PhiMeanModPi", &Conf_PhiMeanModPi);
  reader->AddSpectator( "Hit_MainAxisPhi",   &Hit_MainAxisPhi);

  // Book method(s)
  TString dir    = "weights/";
  TString prefix = "TMVARegression";
  std::vector<TString> MethodList;
  MethodList.clear();
  for(std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
    if(it->second) {
      TString methodName = it->first + " method";
      MethodList.push_back(methodName);
      TString weightfile = dir + prefix + "_" + TString(it->first) + ".weights.xml";
      if(TString(it->first) == TString("MLP")) weightfile = OutputFile + TString(".weights.xml");
      reader->BookMVA( methodName, weightfile );
    }
  }

  for(int ifile=0;ifile<int(FileList.size());ifile++) {
    TFile *input(0);
    if(!gSystem->AccessPathName(FileList[ifile].Data())) input = TFile::Open(FileList[ifile].Data());
    if(!input) {
      cout << "ERROR: could not open data file " << FileList[ifile].Data() << endl;
      exit(1);
    }
    cout << "--- Looking at file " << input->GetName() << endl;

    TTree* regTree = (TTree*)input->Get(TreeName.Data());

    regTree->SetBranchAddress("Hit_NormMinStd",    &Hit_NormMinStd);
    regTree->SetBranchAddress("Hit_NormMaxStd",    &Hit_NormMaxStd);
    regTree->SetBranchAddress("Hit_TotChargeADC",  &Hit_TotChargeADC);
    regTree->SetBranchAddress("Hit_SeedChargeADC", &Hit_SeedChargeADC);
    regTree->SetBranchAddress("Hit_MainAxisPhi",   &Hit_MainAxisPhi);
    regTree->SetBranchAddress("Conf_PhiMean",      &Conf_PhiMean);
    regTree->SetBranchAddress("Conf_PhiMeanModPi", &Conf_PhiMeanModPi);
    regTree->SetBranchAddress("Conf_ThetaMean",    &Conf_ThetaMean);

    Long64_t PrintFreq = 1000;
    for(Long64_t ievt=0; ievt<regTree->GetEntries();ievt++) {
      if(ievt%PrintFreq == 0) std::cout << "--- ... Processing entry: " << ievt << " of file " << input->GetName() << std::endl;
      regTree->GetEntry(ievt);

      for(int imethod=0;imethod<int(MethodList.size());imethod++) {
	if(!MethodList[imethod].BeginsWith("MLP")) continue;

	float regOutput = (reader->EvaluateRegression( MethodList[imethod].Data() ))[0];

	h_ThetaIncident_vs_MLPregressionOut->Fill(Conf_ThetaMean,regOutput);
	h_ThetaIncident_vs_DeltaMLPregressionOut->Fill(Conf_ThetaMean,regOutput - Conf_ThetaMean);


	int index = h_ThetaIncident_vs_MLPregressionOut->GetXaxis()->FindBin(Conf_ThetaMean) - 1;
	MLPOutMean[index]       += regOutput;
	DeltaMLPOutMean[index]  += regOutput - Conf_ThetaMean;
	MLPOutRMS[index]        += pow(regOutput,2);
	Nhits[index]++;

	if(MethodList[imethod].BeginsWith("MLP")) break;
      }
    }

  }
  delete reader;

  for(int i=0;i<Nbins_theta_in;i++) {
    if(Nhits[i] > 25) {
      MLPOutMean[i]      /= Nhits[i];
      DeltaMLPOutMean[i] /= Nhits[i];
      MLPOutRMS[i]       /= Nhits[i];
      MLPOutRMS[i]       -= pow(MLPOutMean[i],2);
      MLPOutRMS[i]        = sqrt(MLPOutRMS[i]);
    }
    else {
      MLPOutMean[i]      = -1000.0;
      DeltaMLPOutMean[i] = -1000.0;
      MLPOutRMS[i]       = 1.0e-6;
    }

    h_ThetaIncident_vs_MeanMLPregressionOut->SetBinContent(i+1,MLPOutMean[i]);
    h_ThetaIncident_vs_MeanMLPregressionOut->SetBinError(i+1,MLPOutRMS[i]);

    h_ThetaIncident_vs_MeanDeltaMLPregressionOut->SetBinContent(i+1,DeltaMLPOutMean[i]);
    h_ThetaIncident_vs_MeanDeltaMLPregressionOut->SetBinError(i+1,MLPOutRMS[i]);
  }

  // Canvas for Raw Data map
  TCanvas *MLPRegressionOutput;
  TObject* g = gROOT->FindObject("MLPRegressionOutput") ;
  if(g) MLPRegressionOutput = (TCanvas*)g;
  else  MLPRegressionOutput = new TCanvas("MLPRegressionOutput", "MLP Regression output", 5, 5,800,700);
  MLPRegressionOutput->Clear();
  MLPRegressionOutput->UseCurrentStyle();
  TPaveLabel* label = new TPaveLabel();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,"MLP Regression output");
  TPad *pad = new TPad("pad","",0.,0.,1.,0.965);
  pad->Draw();
  pad->Divide(2,2);
  pad->cd(1);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetLeftMargin(0.18);
  gPad->SetBottomMargin(0.18);
  TLine* l1 = new TLine(h_ThetaIncident_vs_MLPregressionOut->GetXaxis()->GetXmin(),
			h_ThetaIncident_vs_MLPregressionOut->GetXaxis()->GetXmin(),
                        h_ThetaIncident_vs_MLPregressionOut->GetXaxis()->GetXmax(),
			h_ThetaIncident_vs_MLPregressionOut->GetXaxis()->GetXmax());
  l1->SetLineColor(kRed);
  l1->SetLineWidth(2);
  l1->SetLineStyle(2);
  h_ThetaIncident_vs_MLPregressionOut->Draw("colz");
  l1->Draw();
  pad->cd(2);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetLeftMargin(0.18);
  gPad->SetBottomMargin(0.18);
  TLine* l2 = new TLine(h_ThetaIncident_vs_MLPregressionOut->GetXaxis()->GetXmin(),
			0.0,
                        h_ThetaIncident_vs_MLPregressionOut->GetXaxis()->GetXmax(),
			0.0);
  l2->SetLineColor(kBlack);
  l2->SetLineWidth(1);
  l2->SetLineStyle(2);
  h_ThetaIncident_vs_DeltaMLPregressionOut->Draw("colz");
  l2->Draw();
  pad->cd(3);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetLeftMargin(0.18);
  gPad->SetBottomMargin(0.18);
  h_ThetaIncident_vs_MeanMLPregressionOut->Draw();
  l1->Draw();
  pad->cd(4);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gPad->SetLeftMargin(0.18);
  gPad->SetBottomMargin(0.18);
  h_ThetaIncident_vs_MeanDeltaMLPregressionOut->Draw();
  l2->Draw();

  // Save canvas and histos
  TFile fRoot(OutFileName.Data(),"UPDATE");
  h_ThetaIncident_vs_MLPregressionOut->Write();
  h_ThetaIncident_vs_DeltaMLPregressionOut->Write();
  MLPRegressionOutput->Write();
  fRoot.Close();

  std::cout <<" END  !!!"<< std::endl;

  return;

}
#endif // USETMVA
//______________________________________________________________________________
//
//
//______________________________________________________________________________
void MRaw::SitrineoByEvent( Int_t lastPlaneOfFirstTracker)
{
  // Display various info for Sitrineo tracker event by event
  //
  // Argument lastPlaneOfFirsttracker means:
  //  all planes with ID <= lastPlaneOfFirsttracker are included in first tracker
  //  all planes with ID > lastPlaneOfFirsttracker are included in second tracker
  //
  // JB, 2019/03/10

  fSession->SetEvents(3);

// ==================================
// == Plane, hit and track info

  DTracker *tTracker  =  fSession->GetTracker();
  DPlane* tPlane;
  DHit* aHit;
  DTrack* aTrack;

  int    nPlanes = tTracker->GetPlanesN();
  int    NGoodPlanesInFirstTracker = 0;
  int    NGoodPlanesInSecondTracker = 0;
  bool   PlotPlane[nPlanes];
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    tPlane = tTracker->GetPlane(iPlane);
    if(tPlane->GetReadout() > 0) PlotPlane[iPlane-1] = true;
    else                         PlotPlane[iPlane-1] = false;
    if(PlotPlane[iPlane-1] && iPlane<=lastPlaneOfFirstTracker) {
      NGoodPlanesInFirstTracker++;
    }
    else {
      NGoodPlanesInSecondTracker++;
    }
  } // end loop on Planes


  // ==================================
  // == Canvas and histo info

  TObject* gtmp;
  Char_t name[50], title[1000], canvasTitle[200];

  // Canvas for rawdata
  TCanvas *cdisplayraw;
  gtmp = gROOT->FindObject("cdisplayraw") ;
  if (gtmp) {
    cdisplayraw = (TCanvas*)gtmp;
  }
  else {
    cdisplayraw = new TCanvas("cdisplayraw", "Inspect Raw Data", 5, 5, 800, 700);
  }
  cdisplayraw->Clear();
  cdisplayraw->UseCurrentStyle();
  cdisplayraw->cd();
  TPaveLabel* label = new TPaveLabel();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,"");
  cdisplayraw->cd();
  TPad *padraw = new TPad("pad","",0.,0.,1.,0.965);
  padraw->Draw();
  padraw->Divide( TMath::Max(NGoodPlanesInFirstTracker, NGoodPlanesInSecondTracker), 2);

  // Canvas for hits
  TCanvas *cdisplayhit;
  gtmp = gROOT->FindObject("cdisplayhit") ;
  if (gtmp) {
    cdisplayhit = (TCanvas*)gtmp;
  }
  else {
    cdisplayhit = new TCanvas("cdisplayhit", "Inspect Hits", 600, 150, 800, 700);
  }
  cdisplayhit->Clear();
  cdisplayhit->UseCurrentStyle();
  TPad *padhit = new TPad("pad","",0.,0.,1.,0.965);
  padhit->Draw();
  padhit->Divide( TMath::Max(NGoodPlanesInFirstTracker, NGoodPlanesInSecondTracker), 2);

  // Canvas for tracks
  TCanvas *cdisplaytrack;
  gtmp = gROOT->FindObject("cdisplaytrack") ;
  if (gtmp) {
    cdisplaytrack = (TCanvas*)gtmp;
  }
  else {
    cdisplaytrack = new TCanvas("cdisplaytrack", "Inspect Tracks", 1200, 300 ,800, 700);
  }
  cdisplaytrack->Clear();
  cdisplaytrack->UseCurrentStyle();
  TPad *padtrack = new TPad("padtrack","",0.,0.,1.,0.965);
  padtrack->Draw();
  padtrack->Divide( 2, 2);


  const int Ncolors(10);
  int Colors_tmp[Ncolors];
  Colors_tmp[0] = kBlack;
  Colors_tmp[1] = kGreen+2;
  Colors_tmp[2] = kBlue;
  Colors_tmp[3] = kGray+2;
  Colors_tmp[4] = kOrange-3;
  Colors_tmp[5] = kCyan;
  Colors_tmp[6] = kMagenta;
  Colors_tmp[7] = kYellow;
  Colors_tmp[8] = kOrange;
  Colors_tmp[9] = kTeal+1;

  TLatex* latex = new TLatex();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.04);
  latex->SetTextColor(kBlue);

  TLatex* latex2 = new TLatex();
  latex2->SetTextAlign(12);
  latex2->SetTextSize(0.06);
  latex2->SetTextColor(kBlue);

  std::vector<std::vector<double> > _HitPositions_U        ;  _HitPositions_U        .resize(nPlanes);
  std::vector<std::vector<double> > _HitPositions_V        ;  _HitPositions_V        .resize(nPlanes);
  std::vector<std::vector<int> >    _HitColors             ;  _HitColors             .resize(nPlanes);
  std::vector<std::vector<int> >    _HitTS                 ;  _HitTS                 .resize(nPlanes);

  std::vector<std::vector<double> > _PixelsInHitPositions_U;  _PixelsInHitPositions_U.resize(nPlanes);
  std::vector<std::vector<double> > _PixelsInHitPositions_V;  _PixelsInHitPositions_V.resize(nPlanes);
  std::vector<std::vector<int> >    _PixelsInHitColors     ;  _PixelsInHitColors     .resize(nPlanes);
  std::vector<std::vector<int> >    _PixelsInHitTS         ;  _PixelsInHitTS         .resize(nPlanes);

  std::vector<std::vector<double> > _TrkPositions_U        ;  _TrkPositions_U        .resize(nPlanes);
  std::vector<std::vector<double> > _TrkPositions_V        ;  _TrkPositions_V        .resize(nPlanes);
  std::vector<std::vector<int> >    _TrkIdx                ;  _TrkIdx                .resize(nPlanes);
  // NOTE (Antonin) :
  //    set up the size of the 2D arrays  (nPlanes(fixed nb) x nTracks(changing from evt to evt...) )
  //    One resizes with the number of planes and push_back as needed as far as tracks are concerned

  // Rawdata histograms
  TH2F **hRawData = new TH2F*[nPlanes];
  TH2F **hTrackMapIndex = new TH2F*[nPlanes];
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    if(!PlotPlane[iPlane-1]) continue;
    tPlane = tTracker->GetPlane(iPlane);

    _HitPositions_U[iPlane-1].clear();
    _HitPositions_V[iPlane-1].clear();
    _HitColors[iPlane-1].clear();
    _HitTS[iPlane-1].clear();

    _PixelsInHitPositions_U[iPlane-1].clear();
    _PixelsInHitPositions_V[iPlane-1].clear();
    _PixelsInHitColors[iPlane-1].clear();
    _PixelsInHitTS[iPlane-1].clear();

    _TrkPositions_U[iPlane-1].clear();
    _TrkPositions_V[iPlane-1].clear();
    _TrkIdx[iPlane-1].clear();

    double R_RawData_U[2];
    int Nbins_RawData_U;
    R_RawData_U[0] = - 0.5;
    R_RawData_U[1] = tPlane->GetStripsNu() + 0.5;
    Nbins_RawData_U = int(R_RawData_U[1] - R_RawData_U[0])*NbinsReductionFactor;
    double R_RawData_V[2];
    int Nbins_RawData_V;
    R_RawData_V[0] = - 0.5;
    R_RawData_V[1] = tPlane->GetStripsNv() + 0.5;
    Nbins_RawData_V = int(R_RawData_V[1] - R_RawData_V[0])*NbinsReductionFactor;

    sprintf( name, "hrawdatapl%d", iPlane);
    sprintf( title, "Raw data of plane (%d) %s", iPlane, tPlane->GetPlanePurpose());
    gtmp = NULL;
    gtmp = gROOT->FindObject(name);
    if(gtmp) {
       hRawData[iPlane-1] = (TH2F*)gtmp;
    }
    else {
      hRawData[iPlane-1] = new TH2F(name, title,
                                    Nbins_RawData_U,R_RawData_U[0],R_RawData_U[1],
                                    Nbins_RawData_V,R_RawData_V[0],R_RawData_V[1]);
      hRawData[iPlane-1]->SetMarkerStyle(20);
      hRawData[iPlane-1]->SetMarkerSize(.4);
      hRawData[iPlane-1]->SetMarkerColor(1);
      hRawData[iPlane-1]->SetStats(kFALSE);
    }

    sprintf( name, "htrackmapipl%d", iPlane);
    sprintf( title, "Track map of plane (%d) %s", iPlane, tPlane->GetPlanePurpose());
    gtmp = NULL;
    gtmp = gROOT->FindObject(name);
    if(gtmp) {
      hTrackMapIndex[iPlane-1] = (TH2F*)gtmp;
    }
    else {
      hTrackMapIndex[iPlane-1] = new TH2F(name, title,
                                          Nbins_RawData_U,R_RawData_U[0],R_RawData_U[1],
                                          Nbins_RawData_V,R_RawData_V[0],R_RawData_V[1]);
      hTrackMapIndex[iPlane-1]->SetMarkerStyle(28);
      hTrackMapIndex[iPlane-1]->SetMarkerSize(2.0);
      hTrackMapIndex[iPlane-1]->SetMarkerColor(4);
      hTrackMapIndex[iPlane-1]->SetLineColor(4);
      hTrackMapIndex[iPlane-1]->SetLineWidth(3);
      hTrackMapIndex[iPlane-1]->SetStats(kFALSE);
    }

  } // end loop on planes


  // Hit histrograms
  TH2F **hHitMapTel = new TH2F*[nPlanes];
  TH2F **hHitFoundMapTel = new TH2F*[nPlanes];
  TH2F **hTrackMapTel = new TH2F*[nPlanes];

  // Float_t *geomUmin = new Float_t[nPlanes];
  // Float_t *geomUmax = new Float_t[nPlanes];
  // Float_t *geomVmin = new Float_t[nPlanes];
  // Float_t *geomVmax = new Float_t[nPlanes];
  // Int_t *geomBinUmin = new Int_t[nPlanes];
  // Int_t *geomBinUmax = new Int_t[nPlanes];
  // Int_t *geomBinVmin = new Int_t[nPlanes];
  // Int_t *geomBinVmax = new Int_t[nPlanes];
  // Int_t *centerUbin = new Int_t[nPlanes];
  // Int_t *centerVbin = new Int_t[nPlanes];

  // Determine extrema of planes position in telescope frame
  Double_t xmin=1e6, xmax=-1e6;
  Double_t ymin=1e6, ymax=-1e6;
  Double_t zmin=1e6, zmax=-1e6;
  Double_t planeBox[2][2] = {{0,0},{0,0}};
  TBox **geomPlaneBox = new TBox*[nPlanes];
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    if(!PlotPlane[iPlane-1]) continue;
    tPlane = tTracker->GetPlane(iPlane);
    DR3 posInPlane, posBLInTracker, posURInTracker;

   // bottom left corner
    posInPlane.SetValue( -tPlane->GetStripsNu() * tPlane->GetStripPitch()(0) / 2.
			,-tPlane->GetStripsNv() * tPlane->GetStripPitch()(1) / 2.
			,0.);
    posBLInTracker = tPlane->PlaneToTracker( posInPlane);
    if( posBLInTracker(0)<xmin ) xmin = posBLInTracker(0);
    if( posBLInTracker(1)<ymin ) ymin = posBLInTracker(1);
    if( posBLInTracker(0)>xmax ) xmax = posBLInTracker(0);
    if( posBLInTracker(1)>ymax ) ymax = posBLInTracker(1);
   // upper right corner
    posInPlane.SetValue( +tPlane->GetStripsNu() * tPlane->GetStripPitch()(0) / 2.
			,+tPlane->GetStripsNv() * tPlane->GetStripPitch()(1) / 2.
			,0.);
    posURInTracker = tPlane->PlaneToTracker( posInPlane);
    if( posURInTracker(0)<xmin ) xmin = posURInTracker(0);
    if( posURInTracker(1)<ymin ) ymin = posURInTracker(1);
    if( posURInTracker(0)>xmax ) xmax = posURInTracker(0);
    if( posURInTracker(1)>ymax ) ymax = posURInTracker(1);
    // final box
    planeBox[0][0] = TMath::Min( posBLInTracker(0), posURInTracker(0));
    planeBox[0][1] = TMath::Max( posBLInTracker(0), posURInTracker(0));
    planeBox[1][0] = TMath::Min( posBLInTracker(1), posURInTracker(1));
    planeBox[1][1] = TMath::Max( posBLInTracker(1), posURInTracker(1));
    geomPlaneBox[iPlane-1] = new TBox( planeBox[0][0], planeBox[1][0], planeBox[0][1], planeBox[1][1]);
    geomPlaneBox[iPlane-1]->SetFillStyle(0);
    // Z extrema
    if( tPlane->GetPosition()(2)<zmin) zmin = tPlane->GetPosition()(2);
    if( tPlane->GetPosition()(2)>zmax) zmax = tPlane->GetPosition()(2);

  } // end loop on planes

  // Create the histograms
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    if(!PlotPlane[iPlane-1]) continue;
    tPlane = tTracker->GetPlane(iPlane);

    // -- Histo for hits with microns in telescope frame
    sprintf( name, "hhitmaptelpl%d", iPlane);
    sprintf( title, "Hit map of plane (%d) %s (tel frame)", iPlane, tPlane->GetPlanePurpose());
    gtmp = NULL;
    gtmp = gROOT->FindObject(name);
    if(gtmp) {
      hHitMapTel[iPlane-1] = (TH2F*)gtmp;
    }
    else {
      hHitMapTel[iPlane-1] = new TH2F(name, title, 100, xmin, xmax, 100, ymin, ymax);
      hHitMapTel[iPlane-1]->SetMarkerStyle(20);
      hHitMapTel[iPlane-1]->SetMarkerSize(.6);
      hHitMapTel[iPlane-1]->SetMarkerColor(1);
      hHitMapTel[iPlane-1]->SetStats(kFALSE);
      //printf( "MRaw::DisplayRawData created %s histo with %dx%d pixels\n", name, tPlane->GetStripsNu(), tPlane->GetStripsNv());
    }

    // -- Histo for found (associated with tracks) hits with microns in telescope frame
    sprintf( name, "hhitfoundmaptelpl%d", iPlane);
    sprintf( title, "Found hit map of plane (%d) %s (tel frame)", iPlane, tPlane->GetPlanePurpose());
    gtmp = NULL;
    gtmp = gROOT->FindObject(name);
    if(gtmp) {
      hHitFoundMapTel[iPlane-1] = (TH2F*)gtmp;
    }
    else {
      hHitFoundMapTel[iPlane-1] = new TH2F( *(hHitMapTel[iPlane-1]));
      hHitFoundMapTel[iPlane-1]->SetName( name);
      hHitFoundMapTel[iPlane-1]->SetTitle( title);
      hHitFoundMapTel[iPlane-1]->SetMarkerStyle(2);
      hHitFoundMapTel[iPlane-1]->SetMarkerSize(1.);
      hHitFoundMapTel[iPlane-1]->SetMarkerColor(2);
      hHitFoundMapTel[iPlane-1]->SetStats(kFALSE);
      //printf( "MRaw::DisplayRawData created %s histo with %dx%d pixels\n", name, tPlane->GetStripsNu(), tPlane->GetStripsNv());
    }

    // -- Histo for tracks with microns in telescope frame
    sprintf( name, "htrackmaptelpl%d", iPlane);
    sprintf( title, "Track map of plane (%d) %s (tel frame)", iPlane, tPlane->GetPlanePurpose());
    gtmp = NULL;
    gtmp = gROOT->FindObject(name);
    if(gtmp) {
      hTrackMapTel[iPlane-1] = (TH2F*)gtmp;
    }
    else {
      hTrackMapTel[iPlane-1] = new TH2F( *(hHitMapTel[iPlane-1]));
      hTrackMapTel[iPlane-1]->SetName( name);
      hTrackMapTel[iPlane-1]->SetTitle( title);
      hTrackMapTel[iPlane-1]->SetMarkerStyle(24);
      hTrackMapTel[iPlane-1]->SetMarkerSize(2.);
      hTrackMapTel[iPlane-1]->SetStats(kFALSE);
    }

    // change the color of the track pos wrt tracker index
    if( iPlane < lastPlaneOfFirstTracker ) {
      hTrackMapTel[iPlane-1]->SetMarkerColor(3);
    }
    else {
      hTrackMapTel[iPlane-1]->SetMarkerColor(4);
    }

  } // end loop on planes

  // Track histograms
  TH2F *hTrackMapXY = new TH2F( "htrackmapxy", "Tracks in XY", 100, xmin, xmax, 100, ymin, ymax);
  hTrackMapXY->SetXTitle("X (#mum)");
  hTrackMapXY->SetYTitle("Y (#mum)");
  TH2F *hTrackMapXZ = new TH2F( "htrackmapxz", "Tracks in XZ", 100, xmin, xmax, 100, zmin, zmax);
  hTrackMapXZ->SetXTitle("X (#mum)");
  hTrackMapXZ->SetYTitle("Z (#mum)");
  TH2F *hTrackMapYZ = new TH2F( "htrackmapyz", "Tracks in YZ", 100, ymin, ymax, 100, zmin, zmax);
  hTrackMapYZ->SetXTitle("Y (#mum)");
  hTrackMapYZ->SetYTitle("Z (#mum)");
  TH3F *hTrackMapXYZ = new TH3F( "htrackmapxyz", "Tracks in XYZ", 100, xmin, xmax, 100, ymin, ymax, 100, zmin, zmax);
  hTrackMapXYZ->SetXTitle("X (#mum)");
  hTrackMapXYZ->SetYTitle("Y (#mum)");
  hTrackMapXYZ->SetZTitle("Z (#mum)");




  // ==================================
  // == Event analysis

  fSession->NextRawEvent();
  tTracker->Update();
  Int_t nPairs;
  trackpair_t pairList[100];
  SitrineoAnalysis( lastPlaneOfFirstTracker, nPairs, pairList);
  cout << "Sitrineo: found " << nPairs << " pairs of tracks." << endl;

  sprintf(canvasTitle, "Run %d Event %d", fSession->GetRunNumber(), fSession->GetCurrentEventNumber()-1);
  cdisplayraw->cd();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  cdisplayhit->cd();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  cdisplaytrack->cd();
  sprintf(canvasTitle, "Run %d Event %d, %d tracks", fSession->GetRunNumber(), fSession->GetCurrentEventNumber(), tTracker->GetTracksN());
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);



  // ==================================
  // == Fill & display histos

  Int_t iCol, iRow; // start at 0, so for Setting Bin, use iCol/iRow +1
  int GoodPlaneCounter = 0;
  for( Int_t iPlane=1; iPlane<=nPlanes; iPlane++) { // loop on planes
    if(!PlotPlane[iPlane-1]) continue;
    GoodPlaneCounter++;

    tPlane = tTracker->GetPlane(iPlane);

    std::vector<DPixel*> *aList = tPlane->GetListOfPixels();
    if(fVerbose) cout << "Plane " << iPlane << " has " << aList->size() << " fired pixels" << endl;
    for( Int_t iPix=0; iPix<(Int_t)aList->size(); iPix++) { // loop on fired pixels
      iCol = aList->at(iPix)->GetPixelColumn();
      iRow = aList->at(iPix)->GetPixelLine();
      if( aList->at(iPix)->GetPulseHeight()>0.5 ) {
        int idx_U = hRawData[iPlane-1]->GetXaxis()->FindBin(iCol);
        int idx_V = hRawData[iPlane-1]->GetYaxis()->FindBin(iRow);
        hRawData[iPlane-1]->SetBinContent(idx_U,idx_V, aList->at(iPix)->GetPulseHeight());
      }
      //printf("MRaw::DisplayRawData  pl %d, strip[%d(%d,%d)]=%f\n", iPlane, iStrip, iStrip%tPlane->GetStripsNu(), iStrip/tPlane->GetStripsNu(), tPlane->GetRawValue(iStrip));
    } //end loop on fired pixels

    if(fVerbose) cout << "Plane " << iPlane << " has " << tPlane->GetHitsN() << " hits reconstructed" << endl;
    int color_counter = 0;
    for( Int_t iHit=1; iHit<=tPlane->GetHitsN(); iHit++) { //loop on hits (starts at 1 !!)
      aHit = (DHit*)tPlane->GetHit( iHit);
//        printf("Getting seed index for hit %d (address %x) at plane %d\n", iHit, aHit, iPlane);

      double u = aHit->GetPositionUhit();
      double v = aHit->GetPositionVhit();
      double TheCol,TheRow;
      tPlane->ComputeStripPosition_UVToColRow(u,v,TheCol,TheRow);
      iCol = TheCol;
      iRow = TheRow;
      int Npixels_in_Hit = aHit->GetStripsInCluster();
//        printf(" hit %d, (col,row)=(%3d, %3d), (u,v)=(%.1f, %.1f), Npixels=%d, testMult=%d\n", iHit, (int)TheCol, (int)TheRow, u, v, Npixels_in_Hit, testMult);
      int TS = 0;
      if(tPlane->GetReadout() > 100) aHit->GetPSeed()->GetTimestamp();
      _HitPositions_U[iPlane-1].push_back(TheCol);
      _HitPositions_V[iPlane-1].push_back(TheRow);
      _HitTS[iPlane-1].push_back(aHit->GetTimestamp());
      _HitColors[iPlane-1].push_back(Colors_tmp[color_counter]);

      vector<DPixel*> *aList = tPlane->GetListOfPixels();
      int idex_pixel;
      double Pixel_col;
      double Pixel_row;
      //          if(fVerbose) cout << "Plane " << iPlane << " has " << aList->size() << " fired pixels" << endl;

      for(int ipixInHit=0;ipixInHit < Npixels_in_Hit;ipixInHit++) {
        if( aHit->GetPSeed()==NULL ) { // Hit built with DStrip object
          idex_pixel = aHit->GetIndex(ipixInHit) - 1;
          Pixel_col = idex_pixel%tPlane->GetStripsNu();
          Pixel_row = idex_pixel/tPlane->GetStripsNu();
        }
        else { // Hit built with DPixel object
          idex_pixel   = aHit->GetIndexInOriginalList(ipixInHit);
          Pixel_col = aList->at(idex_pixel)->GetPixelColumn();
          Pixel_row = aList->at(idex_pixel)->GetPixelLine();
        }
        //            printf("    pix %d, index = %d, (col,row)=(%3d, %3d)\n", ipixInHit, idex_pixel, Pixel_col, Pixel_row );

        _PixelsInHitPositions_U[iPlane-1].push_back(Pixel_col);
        _PixelsInHitPositions_V[iPlane-1].push_back(Pixel_row);
        _PixelsInHitTS[iPlane-1].push_back(aHit->GetTimestamp());
        _PixelsInHitColors[iPlane-1].push_back(Colors_tmp[color_counter]);
      }

      color_counter++;
      if(color_counter == Ncolors) color_counter = 0;

      //printf("Getting seed index for hit %d (address %x) at plane %d\n", iHit, aHit, iPlane);
      hHitMapTel[iPlane-1]->Fill( tPlane->PlaneToTracker(*(aHit->GetPosition()))(0), tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), 1);
      if( aHit->GetFound() ) hHitFoundMapTel[iPlane-1]->Fill( tPlane->PlaneToTracker(*(aHit->GetPosition()))(0), tPlane->PlaneToTracker(*(aHit->GetPosition()))(1), 1);

      if(fVerbose) printf("MRaw::SitrineoByEvent  pl %d, hit[%d=(%d,%d)=(%f,%f)]%f\n", iPlane, iHit, aHit->GetIndexSeed()%tPlane->GetStripsNu(), aHit->GetIndexSeed()/tPlane->GetStripsNu(), aHit->GetPositionUhit(), aHit->GetPositionVhit(), aHit->GetClusterPulseSum());

    } //end loop on hits


    if(fVerbose) cout << "Tracker has " << tTracker->GetTracksN() << " tracks reconstructed" << endl;
    for( Int_t iTrack=1; iTrack<=tTracker->GetTracksN(); iTrack++ ) { // loop on tracks
      aTrack = tTracker->GetTrack(iTrack);
      double u = tPlane->Intersection(aTrack)(0);
      double v = tPlane->Intersection(aTrack)(1);
      double TheCol,TheRow;
      tPlane->ComputeStripPosition_UVToColRow(u,v,TheCol,TheRow);

      if((TheCol >= hTrackMapIndex[iPlane-1]->GetXaxis()->GetXmin() && TheCol <= hTrackMapIndex[iPlane-1]->GetXaxis()->GetXmax()) &&
         (TheRow >= hTrackMapIndex[iPlane-1]->GetYaxis()->GetXmin() && TheRow <= hTrackMapIndex[iPlane-1]->GetYaxis()->GetXmax())) {
        hTrackMapIndex[iPlane-1]->Fill(TheCol, TheRow, 1);

        _TrkPositions_U[iPlane-1].push_back(TheCol);
        _TrkPositions_V[iPlane-1].push_back(TheRow);
        _TrkIdx[iPlane-1].push_back(iTrack);
      }

      DR3 posInPlane = aTrack->Intersection(tPlane);
      hTrackMapTel[iPlane-1]->Fill( tPlane->PlaneToTracker(posInPlane)(0), tPlane->PlaneToTracker(posInPlane)(1), aTrack->GetNumber());

      if(fVerbose) printf("MRaw::DisplayHits2D  pl %d, track[%d] = (%.1f,%.1f) in plane, (%.1f, %.1f) in telescope.\n", iPlane, iTrack, tPlane->Intersection(aTrack)(0), tPlane->Intersection(aTrack)(1), tPlane->PlaneToTracker(posInPlane)(0), tPlane->PlaneToTracker(posInPlane)(1));

    } // end loop on tracks


    // == Display

    // display for rawdata
    cdisplayraw->cd();

    double R_tmp_U[2];
    double R_tmp_V[2];
    R_tmp_U[0] = hRawData[iPlane-1]->GetXaxis()->GetXmin();
    R_tmp_U[1] = hRawData[iPlane-1]->GetXaxis()->GetXmax();
    R_tmp_V[0] = hRawData[iPlane-1]->GetYaxis()->GetXmin();
    R_tmp_V[1] = hRawData[iPlane-1]->GetYaxis()->GetXmax();

    padraw->cd(GoodPlaneCounter);
    padraw->cd(GoodPlaneCounter)->SetGridx(1);
    padraw->cd(GoodPlaneCounter)->SetGridy(1);
    padraw->cd(GoodPlaneCounter)->SetTickx(1);
    padraw->cd(GoodPlaneCounter)->SetTicky(1);
    hRawData[iPlane-1]->DrawCopy("colz");
    hRawData[iPlane-1]->Delete();

    //Plot positions of pixels beloning to the rec hits:
    for(int iPixelInHits=0;iPixelInHits<int(_PixelsInHitPositions_U[iPlane-1].size());iPixelInHits++) {
      if((_PixelsInHitPositions_U[iPlane-1][iPixelInHits] >= R_tmp_U[0] &&
          _PixelsInHitPositions_U[iPlane-1][iPixelInHits] <= R_tmp_U[1])
         &&
         (_PixelsInHitPositions_V[iPlane-1][iPixelInHits] >= R_tmp_V[0] &&
          _PixelsInHitPositions_V[iPlane-1][iPixelInHits] <= R_tmp_V[1])
         ) {
        latex->SetTextColor(_PixelsInHitColors[iPlane-1][iPixelInHits]);
        latex->DrawLatex(_PixelsInHitPositions_U[iPlane-1][iPixelInHits],
                         _PixelsInHitPositions_V[iPlane-1][iPixelInHits],
                         "X");
      }
    }

    //Plot positions of rec hits:
    //latex->SetTextColor(kBlue);
    for(int iHits=0;iHits<int(_HitPositions_U[iPlane-1].size());iHits++) {
      if((_HitPositions_U[iPlane-1][iHits] >= R_tmp_U[0] &&
          _HitPositions_U[iPlane-1][iHits] <= R_tmp_U[1])
         &&
         (_HitPositions_V[iPlane-1][iHits] >= R_tmp_V[0] &&
          _HitPositions_V[iPlane-1][iHits] <= R_tmp_V[1])
         ) {
        latex->SetTextColor(_HitColors[iPlane-1][iHits]);
        latex->DrawLatex(_HitPositions_U[iPlane-1][iHits],
                         _HitPositions_V[iPlane-1][iHits],
                         "O");
      }
    }


    for(int iTrk=0;iTrk<int(_TrkPositions_U[iPlane-1].size());iTrk++) {
      if((_TrkPositions_U[iPlane-1][iTrk] >= R_tmp_U[0] &&
        _TrkPositions_U[iPlane-1][iTrk] <= R_tmp_U[1])
        &&
        (_TrkPositions_V[iPlane-1][iTrk] >= R_tmp_V[0] &&
          _TrkPositions_V[iPlane-1][iTrk] <= R_tmp_V[1])
        ) {
          double shift = 2.0*(tPlane->GetStripsNv()/(tPlane->GetStripsNv()*NbinsReductionFactor));
          //double shift = 2.0*hHitMapIndex[iPlane-1]->GetYaxis()->GetBinWidth(1);
          char ttt[300];
          sprintf(ttt,"%d",_TrkIdx[iPlane-1][iTrk]);
          latex2->DrawLatex(_TrkPositions_U[iPlane-1][iTrk],
            //_TrkPositions_V[iPlane-1][iTrk] + shift,
            _TrkPositions_V[iPlane-1][iTrk],
            ttt);
      }
    }

    cdisplayraw->Update();


    // display for hits
    cdisplayhit->cd();

    padhit->cd(GoodPlaneCounter);
    hHitMapTel[iPlane-1]->DrawCopy("P");
    hHitFoundMapTel[iPlane-1]->DrawCopy("Psame");
    hTrackMapTel[iPlane-1]->DrawCopy("TextSame");
    geomPlaneBox[iPlane-1]->Draw("l");
    hHitMapTel[iPlane-1]->Delete();
    hHitFoundMapTel[iPlane-1]->Delete();
    hTrackMapTel[iPlane-1]->Delete();

    cdisplayhit->Update();


  } //end loop on planes

  // display for tracks
  padtrack->cd(1);
  hTrackMapXY->DrawCopy();
  padtrack->cd(2);
  hTrackMapXZ->DrawCopy();
  padtrack->cd(3);
  hTrackMapYZ->DrawCopy();
  padtrack->cd(4);
  hTrackMapXYZ->DrawCopy();

  TLine   aLine; // solid line from first to last plane of the track
  TLine   bLine; // dotted line over the whole tracker dimension
  aLine.SetLineStyle(1);
  aLine.SetLineColor(1);
  bLine.SetLineStyle(2);
  bLine.SetLineColor(1);
  Double_t x[2], y[2], z[2];
  cout << "--> " << tTracker->GetTracksN() << " tracks to display" << endl;
  for( Int_t iTrack=1; iTrack<=tTracker->GetTracksN(); iTrack++) { // loop on tracks
    aTrack = (DTrack*)tTracker->GetTrack( iTrack);

    bLine.SetLineColor( bLine.GetLineColor()+1);
    aLine.SetLineColor( aLine.GetLineColor()+1);

    DR3 trackSlope = aTrack->GetLinearFit().GetSlopeZ();
    DR3 trackOrigin = aTrack->GetLinearFit().GetOrigin();

    z[0] = zmin;
    z[1] = zmax;
    x[0] = trackOrigin(0)+trackSlope(0)*z[0];
    x[1] = trackOrigin(0)+trackSlope(0)*z[1];
    y[0] = trackOrigin(1)+trackSlope(1)*z[0];
    y[1] = trackOrigin(1)+trackSlope(1)*z[1];

    if(fVerbose) printf(" track %d: x[%.1f, %.1f], y[%.1f, %.1f], z[%.1f, %.1f]\n", aTrack->GetNumber(), x[0], x[1], y[0], y[1], z[0], z[1]);

    // XY plane
    padtrack->cd(1);
    bLine.DrawLine( x[0], y[0], x[1], y[1] );
    // XZ plane
    padtrack->cd(2);
    bLine.DrawLine( x[0], z[0], x[1], z[1]);
    // YZ plane
    padtrack->cd(3);
    bLine.DrawLine( y[0], z[0], y[1], z[1]);

    z[0] = zmin;
    z[1] = zmax;
    x[0] = trackOrigin(0)+trackSlope(0)*z[0];
    x[1] = trackOrigin(0)+trackSlope(0)*z[1];
    y[0] = trackOrigin(1)+trackSlope(1)*z[0];
    y[1] = trackOrigin(1)+trackSlope(1)*z[1];
    if(fVerbose) printf("      : x[%.1f, %.1f], y[%.1f, %.1f], z[%.1f, %.1f]\n", x[0], x[1], y[0], y[1], z[0], z[1]);

    // XY plane
    padtrack->cd(1);
    aLine.DrawLine( x[0], y[0], x[1], y[1] );
    // XZ plane
    padtrack->cd(2);
    aLine.DrawLine( x[0], z[0], x[1], z[1]);
    // YZ plane
    padtrack->cd(3);
    aLine.DrawLine( y[0], z[0], y[1], z[1]);
    // XYZ 3Dplane
    padtrack->cd(4);
    TPolyLine3D *line3D = new TPolyLine3D( 2, x, y, z);
    line3D->SetLineColor(aLine.GetLineColor());
    line3D->SetLineWidth(2); // removing warning: from '1.5' to '2' (has to be 'short') BH 2013/08/20
    line3D->Draw();

  } //end loop on tracks

  hTrackMapXY->Delete();
  hTrackMapXZ->Delete();
  hTrackMapYZ->Delete();
  hTrackMapXYZ->Delete();

  cdisplaytrack->Update();

  //===============
  //== End
  cout << "\n Event number : " << fSession->GetCurrentEventNumber()-1<<" " << endl;


}
//
//______________________________________________________________________________
void MRaw::SitrineoAnalysis( Int_t lastPlaneOfFirstTracker, Int_t &nPairs, trackpair_t* pairList)
{
  //Analyse tracks in Sitrineo tracker to evaluate momentum
  //
  // Argument lastPlaneOfFirsttracker means:
  //  all planes with ID <= lastPlaneOfFirsttracker are included in first tracker
  //  all planes with ID > lastPlaneOfFirsttracker are included in second tracker
  //
  //  2019/03/10, Adele Perus, Romain Schotter

  DTracker *tTracker  =  fSession->GetTracker();
  DPlane* aPlane;
  DHit* aHit;
  DTrack *track1, *track2;
  DR3 slope1, slope2;
  trackpair_t aPair;

  // Loop on all tracks and pair them
  nPairs = 0;
  for( Int_t iTrack=1; iTrack<=tTracker->GetTracksN(); iTrack++) { // loop on 1st tracks // recup t from 1st tracker
    track1 = (DTrack*)tTracker->GetTrack( iTrack);
    int plane1 = track1->GetHit(0)->GetPlane()->GetPlaneNumber();
    slope1 = track1->GetLinearFit().GetSlopeZ();

    for( Int_t jTrack=iTrack+1; jTrack<=tTracker->GetTracksN(); jTrack++) { // loop on 2nd tracks
      track2 = (DTrack*)tTracker->GetTrack( jTrack);
      int plane2 = track2->GetHit(0)->GetPlane()->GetPlaneNumber(); 
      if( plane2 < plane1){     
	cout << "First Track " << plane1 << endl ;  
        cout << "Second Track " << plane2 << endl ;    
      	slope2 = track2->GetLinearFit().GetSlopeZ();

      	pairList[nPairs].firstTrackID = iTrack;
      	pairList[nPairs].secondTrackID = jTrack;
      	pairList[nPairs].slope1 = slope1(0);
      	pairList[nPairs].slope2 = slope2(0);
      	double angle = acos( ( (pairList[nPairs].slope2)*(pairList[nPairs].slope1) + 1 )/(sqrt(1+pow(pairList[nPairs].slope2,2) ) * sqrt(1+pow(pairList[nPairs].slope2,2) ) ) ) ;
	cout << " Angle : " << angle << endl ;
	double momentum = 0.3*0.2*20/angle ;
	pairList[nPairs].momentumXY = momentum ;
	cout << " Momentum : " << momentum << endl ;
      	nPairs++;
      }
    } // end loop on 2nd tracks

  } // end loop on 1st tracks

  if(fVerbose) {
    printf( "MRaw::Sitrineo: found %d pairs:\n", nPairs);
    for (size_t ip = 0; ip < nPairs; ip++) {
      printf( "  %d: track1=%d + track2=%d, slope1=%.2e, slope2=%.2e, momentum=%.2e\n", ip, pairList[nPairs].firstTrackID, pairList[nPairs].secondTrackID, pairList[nPairs].slope1, pairList[nPairs].slope2, pairList[nPairs].momentumXY);
    }
  }

}
