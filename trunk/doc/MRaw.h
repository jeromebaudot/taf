// @(#)maf/maf:$Name:  $:$Id: MRaw.h,v.1 2005/10/02 18:03:46 sha Exp $
// Author: A. Shabetai 

#ifndef _MRaw_included_
#define _MRaw_included_

  /////////////////////////////////////////////////////////////
  //                                                         //
  //  Describes all Raw data analysis methods                //
  //                                                         //   
  ////////////////////////////////////////////////////////////

#include "TObject.h"
#include "DTracker.h"
#include "DSession.h"
#include "Riostream.h"
#include "DPlane.h"
#include "TStyle.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "DHit.h"
#include "DStrip.h"
#include "TPolyMarker.h"
#include "TF1.h"
#include "TText.h"
#include <cmath>
#include "TControlBar.h"
#include "MAnalysis.h"
#include "DPixel.h"
#include "DMonteCarlo.h"
#include "DAcq.h"
#include "DGlobalTools.h" // to have fTool has a data member

class MRaw : public TObject {

 private:

 static MRaw* fgInstanceRaw;

  //-------- Control parameters
 Bool_t         fVerbose;
 Int_t          fDebugRaw;     // JB 2014/05/29
 DGlobalTools   fTool;         // JB 2011/07/18 to use global tools
 DSession      *fSession;      // JB 2011/07/21 to get rid of global var tSession

  Int_t MultiFramePlane; // JB 2013/07/18
  Int_t MultiFrameNFrames;

 TH1F *hnnois1;
 TH1F *hraw1;
 TH1F *hson1;
 TH1F *hson;
 TH1F *hpu;
 TH1F *hpu1;
 TH1F *hnoisd;
 TH1F *hped;
 TH1F *hpedd;
 TH1F *raww1;
 TH2F *raw1;
 TH2F *raw2;
 TH2F *raww;
 TH2F *hit0;
 TH2F *hit;
 TH2F *hnnois;
 TH2F *hNoisyChannels;

  
 DTracker *tTracker;
 Bool_t startMIMOSA;
 Bool_t startRS;
 Bool_t showit;
  
 TH1F* hson_iterated;
 TH1F*  hson_iterated2;  //ADAPTED FOR MI5
 TH1F*  hson_iterated3;  //ADAPTED FOR MI5 
 TH1F*  hson_iterated4 ;  //ADAPTED FOR MI5 
 TH1F*  hson_substract2_3;  //ADAPTED FOR MI5 
 TH1F*  hson_substract3_4; 
 TH2F*  hhitmap;

 Int_t   nh;
 TCanvas *cnv;

 //-------iterated histogram setting:
 Int_t iterated_bin_number;
 Float_t iterated_xmin_number;
 Float_t iterated_xmax_number;

 TControlBar*  bar3;
 Bool_t fInitDone;
 Bool_t fMDisplayDone;

 Bool_t CheckIfDone(const Option_t* Option); 
 TH1F*  Zero(TH1F* ptr) { if(ptr) {delete ptr;ptr=0;}return ptr;};     
 public:
 MRaw( DSession *aSession);
 virtual ~MRaw(){;}
 void InitScan(Int_t Events2Scan = 400,Float_t SignalOverNoiseCut = 5); // Accumulate certain amount of events for scanning
 void MimosaDisplay(Int_t NEVENT = 1); // event display. 
 void RSDisplay(); //Display Ref.system events 
 void InspectScan(); // inspects events 
 void Inspectfake(); //inspects fakes 
 void MimosaJump(Int_t Njump =10); // jumps events
 void PrepareRaw();
 void Clear(const Option_t* /*option*/ = "");
  void InspectRawChannels( Int_t nEvents=1000, Int_t thePlaneNumber=1, Int_t firstChannel=0, Int_t nChannels=1, Int_t maxAutoCorr=20); // JB 2012/09/20
 void DisplayRawData( Float_t nChanToDisplay = .5);
 void DisplayRawData2D(Float_t minSN=-65536., Bool_t withHits=kTRUE, Bool_t withTracks=kTRUE, Int_t triggerNb=-1, Int_t clusterMultiplicity=-1,
		       double Rminu = -1.0e10,
		       double Rmaxu = -1.0e10,
		       double Rminv = -1.0e10,
		       double Rmaxv = -1.0e10);
 void DisplayHits2D( Int_t ifIndex=2, Int_t ifTrack=1, Int_t geomatrix=0); //JB 2012/04/02
 void DisplayTracks(); // JB 2011/07/26
 void DisplayCumulatedTracks( Int_t nEvents=1000);
 void DisplayCumulatedHits2D(Int_t nEvents=1000,
			     Bool_t ifDrawTrack=kTRUE,
			     Bool_t Define_Range=kFALSE,
			     int bins = 100,
			     double Xmin = -1.0e10, 
			     double Xmax = -1.0e10, 
			     double Ymin = -1.0e10, 
			     double Ymax = -1.0e10);
 void DisplayCumulatedRawData2D(Int_t nEvents=500, Float_t minSN=-65536., Float_t occurence_min=.001,
				Float_t occurence_max=1.,
				Int_t nOccurences=20,
				Float_t minOccurence=0., 
				Bool_t storeOccurence=kFALSE); //RDM280509, JB 2011/03/14
 void DisplayCumulatedOverflow( Int_t nEvents=500); //MG 2012/03/14
 void DisplayNoise(  Int_t thePlaneNumber=-1, Float_t calibFactor=-1., Bool_t ifPixelHistory=kFALSE);
 void SkipEvent( Int_t nEventsToSkip);
 void DisplaySpectrum(Int_t nEvents, Int_t minSN=3, Int_t minValue=0, Int_t maxValue=4096); //26/05/09 YV
 void DisplayCumulatedClusters(Int_t planeNumber, Int_t nEvents); //JB 2011/02/07
 void LaserStudy( Int_t Xpixel=125, Int_t Ypixel=125, Int_t nPixels=50, Int_t nEvents=1000, Int_t cycleLength=20, Int_t nSpots=10); //JB+MB 2011/02/25
 void VertexStudy( Int_t nEvents=1000, Double_t lineX=0., Double_t lineY=0., Double_t beamTilt=0.); //JB 2011/07/26
 void DisplayGeometry(); // JB 2011/11/01
  void MakeLadderGeometry( Int_t *planeList, Int_t numberOfPlanes, Int_t aRefPlaneNumber); // JB 2014/02/12
 void FakeRateBinaryFromRawData( Int_t nEvents, Int_t aPLaneNumber, Int_t maxPixPerEvent = 100, Double_t maxRateForTruncation=0.01, Int_t lowestCol=0, Int_t highestCol=0, Int_t lowestRow=0, Int_t highestRow=0); // JB 2011/11/22
  void DumpEventHeader( Int_t nEvents); // JB 2012/05/03
  void UserPlot( Int_t nEvents=1000 ); // JB 2009/09/08
  void ToggleVerbosity(); // JB 2011/02/09
  void DisplayCumulatedMonteCarlo2D( Int_t nEvents=1000, Bool_t ifDrawTrack=kTRUE); // LC 2012/10/19
  void SetMultiFrame( Int_t aPlane, Int_t nFrames ); // JB 2013/07/18
  void DisplayHistory( Int_t nEvents=50000, Int_t nCumul=500);  // JB 2014/05/27
  void SetDebugLevel( Int_t aLevel) { fDebugRaw = aLevel; }; // JB 2014/05/29
  void DisplayImage( Int_t nEvents=1, Char_t *data="raw", Char_t *format="gif", Float_t minValue=-65536.);
  
 using TObject::Clear;

 static  MRaw*& InstanceRaw( DSession *aSession) { 
   // Modified: JB 2011/07/21 to pass session pointer
   if (!fgInstanceRaw) 
     {
       cout<<"No current Mimosa Raw session detected! Creating a new one..."<<endl; 
       fgInstanceRaw = new MRaw( aSession);
     } 
   return fgInstanceRaw; 
 }
 


ClassDef(MRaw,1) //Mimosa Raw studies

};

          
#endif
