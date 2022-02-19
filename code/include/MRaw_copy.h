// @(#)MRaw.h  $:$Id: MRaw.h,v.1 2005/10/02 18:03:46 sha Exp $
// Author: A. Shabetai

#ifndef _MRaw_included_
#define _MRaw_included_

  /////////////////////////////////////////////////////////////
  //                                                         //
  //  Describes all Raw data analysis methods                //
  //                                                         //
  /////////////////////////////////////////////////////////////

#include "TObject.h"
#include "DTracker.h"
#include "DSession.h"
#include "Riostream.h"
#include "DPlane.h"
#include "TStyle.h"
#include "TTree.h"
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
#include "DBeaster.h"
//#include "DMonteCarlo.h"
#include "DAcq.h"
#include "DGlobalTools.h" // to have fTool has a data member
#include "tiffio.h"

class MRaw : public TObject {

 private:

 static MRaw* fgInstanceRaw;

  //-------- Control parameters
 Bool_t         fVerbose;
 Int_t          fDebugRaw;     // JB 2014/05/29
 DGlobalTools   fTool;         // JB 2011/07/18 to use global tools
 DSession      *fSession;      // JB 2011/07/21 to get rid of global var tSession

  Float_t NbinsReductionFactor; // JB 2014/12/08
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

 Int_t fUserFileNumber;
 Int_t   GetFileNumber();

 // Sitrineo
 struct sitrihitpair_t {
   int plane1;
   int hitid1;
   double x1;
   double y1;
   double z1;
   int plane2;
   int hitid2;
   double x2;
   double y2;
   double z2;
   double deltaX;
   double deltaY;
   double deltaZ;
   double slopeX;
   double slopeY;
 };
 struct sitritrack_t {
     int id;
     bool bckup_flag;
     int firstTrackID;
     int secondTrackID;
     int plane[4];
     int hit[4];
     double secondtrack_DX ;
     double secondtrack_DY ;
     double secondtrack_DZ ;
     double secondtrack_slopeX;
     double secondtrack_slopeY;
     double firsttrack_DX ;
     double firsttrack_DY ;
     double firsttrack_DZ ;
     double firsttrack_slopeX;
     double firsttrack_slopeY;
     double Dslope_X;
     double Dslope_Y;
     double momentum_X;
     double momentum_Y;
     double fit_origin_X;
     double fit_origin_Y;
     double fit_slope_X;
     double fit_slope_Y;
     double fit_momentum;
     double fit_chi2X;
     double fit_chi2Y;
 };


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
 void DisplayCumulatedSubtracks( Int_t nEvents=1000, Int_t dutPlaneNb=-1); //JB 2014/12/23
 void DisplayCumulatedHits2D(Int_t nEvents=1000,
			     Bool_t ifDrawTrack=kTRUE,
			     Bool_t Define_Range=kFALSE,
			     int bins = 100,
			     double Xmin = -1.0e10,
			     double Xmax = -1.0e10,
			     double Ymin = -1.0e10,
			     double Ymax = -1.0e10,
           Bool_t Define_QRange=kFALSE,
           int qbins = 1000,
           double qmax = 1000);
 void DisplayLadderCumulatedHits2D( Int_t nEvents=1000,
				    Bool_t ifDrawTrack=kTRUE,
				    Bool_t Define_Range=kTRUE,
				    Int_t bins=100,
				    Double_t Xmin=-1.0e-10,
				    Double_t Xmax=-1.0e-10,
				    Double_t Ymin=-1.0e-10,
				    Double_t Ymax=-1.0e-10); // LC 2015/02/17
/////////////////////////////////////////
/////// Methods For Beast ///////////////
void DisplayBeast();
void DisplayCumulatedBeast(int MaxEvt);
void BeastStatStudy();
int  GetBeastModuleId(int SensorId);
int  GetBeastLadderId(int SensorId);
void BeastCheckPosition();
/////////////////////////////////////////



 void DisplayCumulatedRawData2D(Int_t nEvents=500, TString str_EvtParsingMode="continuous", Float_t minSN=-65536., Float_t occurence_min=.001,
				Float_t occurence_max=1.,
				Int_t nOccurences=20,
				Float_t minOccurence=0.,
				Bool_t storeOccurence=kFALSE,
				double Colmin = -1.0e10, double Colmax = -1.0e10,
				double Linmin = -1.0e10, double Linmax = -1.0e10); //RDM280509, JB 2011/03/14
 void DisplayCumulatedOverflow( Int_t nEvents=500); //MG 2012/03/14
 void DisplayNoise(  Int_t thePlaneNumber=-1, Float_t calibFactor=-1., Float_t maxAxisNoise=0., Float_t maxAxisPed=0., Bool_t ifPixelHistory=kFALSE);
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
  void SetNbinsReductionFactor( Float_t aFactor); // JB 2014/12/08
  void DisplayHistory( Int_t nEvents=50000, Int_t nCumul=500);  // JB 2014/05/27
  void SetDebugLevel( Int_t aLevel) { fDebugRaw = aLevel; }; // JB 2014/05/29
  void DisplayImage( Int_t nEvents=1, Int_t planeToPlot=4, TString data=TString("raw"), TString format=TString("gif"), Bool_t displayImage=1, Bool_t saveImage=1, Float_t minValue=-65536, Float_t NBins=1., Bool_t greyScale=kTRUE, TString flip=TString("Y"));
  void StudyTrackMultiplicity( TH1F *hNTracksPerPlanes, TH1F *hNHitsPerTrack, Int_t nEvents);
  void StudyTrackingEfficiency(Int_t nEvents,
			       Double_t nHitsMax = -1/*Default: Set default nHitsMax from conf file*/,
			       Double_t Xmin = -1e6, Double_t Xmax = 1e6,
			       Double_t Ymin = -1e6, Double_t Ymax = 1e6); // QL, 2015/04/10
  void StudyMultipleScattering( Int_t nEvents, Int_t set0plane0, Int_t set0plane1, Int_t set1plane0, Int_t set1plane1 ); // JB 2015/08/21
  void StudyBeamProfile(Int_t nEvents = 1000,
	                Bool_t ifFit2DGaus = false, // time consuming
			double Xmin = -1.0e10,
			double Xmax =  1.0e10,
			double Ymin = -1.0e10,
			double Ymax =  1.0e10); // QL, 2015/10/26
  void XrayAnalysis(Int_t nEvents=1000,
                    Bool_t ProduceTree = kFALSE,
                    Bool_t ifDrawTrack=kTRUE,
                    Bool_t Define_Range=kFALSE,
                    int bins = 100,
                    double Xmin = -1.0e10,
                    double Xmax = -1.0e10,
                    double Ymin = -1.0e10,
                    double Ymax = -1.0e10,
                    Float_t HighestPeakPositionEv=0,
                    Float_t MinSpectrum=1000,
                    Float_t MaxSpectrum=7000,
                    Bool_t readNormFromFile=kFALSE,
                    Bool_t normalizeADCspectrum=kFALSE,
                    Float_t cutLimit=0.95,
                    Int_t fitXray=0);

  void SeedCuts(Int_t nEvents=1000);

  void BuildPixelGainMap( Int_t nEvents=100000, Double_t min=850, Double_t max=960, Double_t maxcharge=2500 ); // JB 2018/07/04

  void BetaSourceMultiFrameAnalysis(int     aPlane  = 0,
				    int     nFrames = 0,
                                    Int_t   nEvents=1000,
		                    Bool_t  ProduceTree = kFALSE,
				    int     Npixel_TimeSpaceCut = 5,
				    double  SAveN_cutFit = 12.0,
				    int     Fiducial_col_min = -1,
				    int     Fiducial_col_max = -1,
				    int     Fiducial_row_min = -1,
				    int     Fiducial_row_max = -1);

  void CumulateTxtFrames( Int_t nEvents=1000, Int_t nCumulFrames=100);

//#define USETMVA
#ifdef USETMVA
  void FillnTupleForTMVA(int    nEvents       = 100,
			 double my_theta_mean = 0.0,
			 double my_phi_mean   = 0.0);

  void TrainTMVA(TString myMethodList = "",
		 TString fname        = "",
		 TString TreeName     = "T",
		 TString OutputFile   = "TMVAReg.root",
		 int Nevts_training   = 10000,
		 int Nevts_testing    = 10000);
#endif

  void SitrineoByEvent( Int_t lastPlaneOfFirstTracker=2, Double_t maxX1=20000, Double_t maxY1=20000, Double_t maxSlopeX1=3.2, Double_t maxSlopeY1=3.2, Double_t maxDX2=20000, Double_t maxDY2=20000, Double_t maxSlopeX2=3.2, Double_t maxSlopeY2=3.2);
  void SitrineoCumul( Int_t nEvents=1000, Int_t lastPlaneOfFirstTracker=2, Double_t maxX1=20000, Double_t maxY1=20000, Double_t maxSlopeX1=3.2, Double_t maxSlopeY1=3.2, Double_t maxDX2=20000, Double_t maxDY2=20000, Double_t maxSlopeX2=3.2, Double_t maxSlopeY2=3.2);
  void SitrineoAnalysis( Int_t lastPlaneOfFirstTracker, vector<sitritrack_t> *tracklist);
  int SitrineoAnalysisFromHits( Int_t lastPlaneOfFirstTracker, vector<sitritrack_t> *tracklist, Double_t minX1=-10000, Double_t maxX1=10000, Double_t minY1=-10000, Double_t maxY1=10000, Double_t maxSlope1=30.);
  void SitrineoContinuous( Int_t nEvents=20, Int_t lastPlaneOfFirstTracker=2, Double_t maxX1=20000, Double_t maxY1=20000, Double_t maxSlopeX1=3.2, Double_t maxSlopeY1=3.2, Double_t maxDX2=20000, Double_t maxDY2=20000, Double_t maxSlopeX2=3.2, Double_t maxSlopeY2=3.2);
  void SitrineoAlign( Int_t nEvents=200000, Bool_t order=kTRUE);

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
