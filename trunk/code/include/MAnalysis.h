// @(#)maf/maf:$Name:  $:$Id: MAnalysis.h v.1 2005/10/02 18:03:46 sha Exp $
// Author: A. Shabetai 

#ifndef _MimosaAnalysis_included_
#define _MimosaAnalysis_included_

// Choose your environment
//#define LINUX
#define MAC
//#define WINDOWS

  /////////////////////////////////////////////////////////////
  //                                                         //
  //  TAF - Main header file                                 //
  //  Defines all user accessible analysis methods            //
  //                                                         //   
  ////////////////////////////////////////////////////////////



#include "Riostream.h"
#include "TControlBar.h"
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "DEvent.h"
#include "DPrecAlign.h"
#include "DSession.h"
#include "TPaveLabel.h"
#include "TProfile.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TF1.h"
#include "TStopwatch.h"
#include "TVectorD.h"
#include "TVector3.h"
#include "TGraph.h"
#include "TGraph2D.h"

#include "TError.h"
#include "TEnv.h"  
#include "TInterpreter.h"  

#include "DGlobalTools.h" // to have fTool has a data member

// pre 
#include "DLadder.h" // JB 2014/02/10
#ifdef UseROOFIT
#include "DXRay2DPdf.h" // AP 2014/12/08
#endif
#include "DTracker.h"
#include "DPlane.h"
#include "DAlign.h"
#include "DHit.h"
#include "DR3.h"


// post
#include "TPad.h"
#include "TFrame.h"
#include "TText.h"
#include "TPaveStats.h"
#include "TPaveText.h"
#include "TPave.h"
#include "TLatex.h"
#include "TLegend.h"

//Results store
#include "TNtuple.h"

// align Mimosa 
#include "MAlign.h"
#include "MHist.h"

// MRaw/x
#include "MRaw.h"
#include "MRax.h"

//---ADC
#include "DSetup.h"
//---ADC

class MRaw;
class MRax;

//---ADC
class DSetup;         // forwards
//---ADC

class DR3;

class MimosaAnalysis : public MHist {

  //====================
 private:
  //====================
  static MimosaAnalysis* fgInstance; // global pointer to the TAF object
#ifdef MAC
  MimosaAnalysis *gTAF;// for MAC
#endif
  
  // ------------------------
  // Files and directories
  // ------------------------
  TFile *fileInputTree;
  ofstream outFileGoodEvt; //!
  ofstream outFileBadEvt; //!
  ofstream csvfile; //!
  TFile* theCorFile; //!
  TString DTDIR; // set from the environment variable DTDIR = TAF installation directory
  TString fLaunchedDirectory; // directory where taf has been launched // VR 2014/06/30
  TString fWorkingDirectory; // directory where taf will work, choose in MimosaAnalysis::MimosaAnalysis() // VR 2014/06/30
  //TString ROOTDIR; // removed by JB 2011/04/12
  TString ResultDirName; // JB 2011/04/12
  DGlobalTools   fTool;                       // JB 2011/07/18
  char RootFile[300]; //name of the input DSF file

  // ------------------------
  // Input tree
  // ------------------------
  // JB 2010/08/30
  TTree *t;
  DEvent *Evt;
  TBranch *branch;

  // ------------------------
  // Parameters for sensor, alignment, geometry, analysis
  // ------------------------
  
  DSession   *fSession; // JB 2011/07/21 to get rid of global var tSession
  Double_t trU, trV, trW; // alignment
  Double_t theta[3];
  DPrecAlign *alignement;

  Float_t calibration;
  Float_t NoiseScope;
	
  Int_t MimosaType;
  Int_t RunNumber ;
  Int_t ThePlaneNumber; 
  Int_t ThePlaneNumber2; // JB 2011/10/30
  Int_t TheLadderNumber; // JB 2014/01/10
  Int_t Nevt ,User_MaxEvt;

  Int_t ThesubmatrixNumber; 
  Int_t ThesubmatrixNumber2; // JB 2015/12/15 

  Float_t MimosaSizeU, MimosaSizeV;
  Float_t PixelSize;
  Float_t PixelSizeU;
  Float_t PixelSizeV;
  Int_t Matrixtype;
  Int_t NofPixelInColumn;
  Int_t NofPixelInRaw ;
  Int_t NofPixels ;
  Float_t FirstPixelShiftU ,FirstPixelShiftV;
  Int_t Thegeomatrix;
  Int_t Thegeomatrix2; // JB 2015/12/15
  //  Float_t geomUmin, geomUmax;
  //  Float_t geomVmin, geomVmax;
  Float_t geomUmin2, geomUmax2;
  Float_t geomVmin2, geomVmax2;
  Float_t exgeomUmin, exgeomUmax; // JB 2013/06/22
  Float_t exgeomVmin, exgeomVmax;
  
  Int_t UserFlag; // parameter transmitted through config file, free for user, JB 2013/07/17

  // ------------------------
  // flags
  // ------------------------
  Int_t UsedMacro[50]; // to be modified 
  Int_t MimoDebug;
  Int_t fWriteHistos;
  Int_t fUserFileNumber;
  Bool_t fInitDone;
  Bool_t fMimosaDone; // for general purpose histo JB 2010/09/10
  Bool_t fMimosaProDone;
  Bool_t fMimosaFakerateDone; // JB 2010/07/26
  Bool_t fMimosaCalibrationDone; // JB 2010/07/27
  Bool_t fMimosaMiniVectorsDone; // JB 2010/08/30
  Bool_t fMimosaPro2PlanesDone; // JB 2013/05/01
  Bool_t fMimosaImagingDone; // JH 2014/06/16
  Bool_t fClearDone;
  // The following flags indicate the purpose of the analysis
  // JB 2014/01/11
  Bool_t fIfReferenceTrack;
  Bool_t fIfCalibration;
  Bool_t fIfImaging;
  Bool_t fIfLaserSpot;
  Bool_t fIfMiniVector;
  Bool_t fIfVertex;
  Bool_t fIfFake;
  Bool_t fIfBinary; // for binary output sensor, JB 2014/03/15
  Bool_t fIfMCGeneration; //for MC generation, AP 2015/04/02

  // ------------------------
  // Hot pixel map 
  // ------------------------
  Int_t TheUsePixelMap; // use pixel map or not
  Int_t Option_read_Pixel_map; // Hot hit map read=1/write=0 option
  TH2F *h2HotPixelMap; // pointer to the histogram containing pixel hit rate
  Char_t HotPixelFileName[40];
  TFile *HotPixelFile; // pointer to file containing the previous histogram
  
  // ------------------------
  // cuts
  // ------------------------
  Float_t  TrackChi2Limit; 
  Int_t    MinHitsPerTrack; // JB 2013/06/22
  Int_t    MaxNbOfTracksInGeom; // JB 2012/08/30
  Int_t    GeoMatrixForTrackCut; // JB 2012/08/30
  Int_t    CUT_MaxNbOfHits;
  Int_t    CUT_MinNbOfHits;
  Float_t  TrackToHitDistanceLimit;
  Float_t  TrackToHitDistanceLimit2; // JB 2015/12/15
  Float_t  CUT_S2N_seed; 
  Float_t  CUT_S2N_neighbour ;
  Float_t  CUT_Q_seed; // JB 2013/11/08
  Float_t  CUT_Q_cluster; // JB 2014/01/21
  Float_t  CUT_MinQ_neighbour; // JB 2010/07/27
  Float_t  CUT_MaxQ_neighbour; // JB 2013/11/08
  Double_t CUT_MaxHitRatePerPixel;
  Double_t CUT_MinHitRatePerPixel;
  Int_t    CUT_MinSeedIndex; // JB 2013/08/21
  Int_t    CUT_MaxSeedIndex; // JB 2013/08/21
  Int_t    CUT_MinSeedCol; // JB 2013/08/22
  Int_t    CUT_MaxSeedCol; // JB 2013/08/22
  Int_t    CUT_MinSeedRow; // JB 2013/08/22
  Int_t    CUT_MaxSeedRow; // JB 2013/08/22

  // ------------------------
  // counters & efficiency
  // JB 2011/11/04
  // ------------------------
  Double_t MimosaEfficiency;
  Double_t MimosaEfficiency_ERR;

  Int_t    NofClMatchTrack; // # good tracks in the DUT WITH a matched hit
  Int_t    NtrkInMimo; // Number of tracks in the MIMOSA aperture
  
  Int_t    NeventRangeForEfficiency; //binning for the efficiency during the run
  Int_t    temp_maxarray;
  Int_t    ievt_array; // counter of bunches
  Float_t  *temp_Efficiency_array;
  Float_t  *temp_NofClMatchTrack;
  Float_t  *temp_NtrkInMimo;

  // ------------------------
  // 
  // ------------------------
    
  Float_t* ResolutionPoint; // better in MHIst ??


  // ------------------------
  // pixel data inside cluster, JB 2010/06/03
  // ------------------------
  Int_t NofPixelsInCluster;
  Float_t* qonly;
  Float_t* snonly;
  Float_t* noise;
  Float_t* UofPix;
  Float_t* VofPix;
  Int_t *LineInCluster;
  Int_t *ColumnInCluster;
  Int_t* IndexofPix;
  Int_t* IndexInCluster;
  Int_t LineSizeOfCluster; // JB 2014/03/31
  Int_t ColumnSizeOfCluster;
  
  typedef enum {
    one_pix = 1, 
    two_pix_row, 
    two_pix_col, 
    three_pix_L, 
    three_pix_row, 
    three_pix_col, 
    four_pix_square, 
    four_pix_L_row, 
    four_pix_L_col, 
    four_others,
    more_than_four,
    five_pix_squarerow,
    five_pix_squarecol,
    five_others,
    six_pix_3col2row,
    six_pix_2col3row,
    six_others,
    more_than_six
  } clusterTypeDef;
  clusterTypeDef ClusterGeometricalType;
  clusterTypeDef ClusterGeometricalTypeBeyond4;

  Int_t IndexOfMaxPixel; // index of the pixel with the highest charge (in qonly array)

  Float_t snOptimal; // Largest S/N obtained by summing q ordered pixel
  Int_t optimalSize; // number of pixels in the largest S/N

  Float_t MinUofPix; // position of lower left pixel
  Float_t MinVofPix;

  Float_t TotalCharge; // all the pixels in the cluster
  Float_t ChargeAroundSeed; // all the pixels except the seed
  Float_t ChargeInCross; // the 4 pixels direct neighbours of the seed
  Float_t ChargeInX; // the 4 pixels in the corner of the seed
  Float_t ChargeInCrown1; // the 16 pixels after the first neighbours of the seed
  Float_t ChargeInCrown2; // the 24 pixels next to next neighbours of the seed

  Float_t TotalCharge2x2; // Cluster charge limited to 2x2 sub-cluster 
  Float_t ExternalCharge2x2; // 2x2 cluster charge without the seed (Qof3x3[0])
  Float_t Qof2x2[4];
  Float_t UofPix2x2[4];
  Float_t VofPix2x2[4];
  Int_t IndexU2x2[4];
  Int_t IndexV2x2[4];
  Short_t IndexOfCluster2x2;

  Float_t TotalCharge3x3; // Cluster charge limited to 3x3 sub-cluster 
  Float_t Qof3x3[9];
  Float_t UofPix3x3[9];
  Float_t VofPix3x3[9];
  Int_t IndexofPix3x3[9];

  Float_t TotalCharge5x5; // Cluster charge limited to 5x5 sub-cluster
  Float_t Qof5x5[25];
  Float_t UofPix5x5[25];
  Float_t VofPix5x5[25];
  Int_t IndexofPix5x5[25];

  Float_t* qqordered; // charge ordered pixels
  Float_t* nqordered;
  Float_t* snqordered;
  Int_t* IndexofPixqordered;
  Float_t* UofPixqordered;
  Float_t* VofPixqordered;
  Int_t *LineInClusterqordered;
  Int_t *ColumnInClusterqordered;
	
  Float_t* q; // cumulated q,n,sn with charge ordered pixels
  Float_t* n;
  Float_t* sn;

  Float_t* qsnordered; // sn ordered pixels
  Float_t* nsnordered;
  Float_t* snsnordered;
  Int_t* IndexofPixsnordered;
  Float_t* UofPixsnordered;
  Float_t* VofPixsnordered;
  Int_t *LineInClustersnordered;
  Int_t *ColumnInClustersnordered;

  Float_t* qsumsn; // cumulated q,n,sn with s/n ordered pixels
  Float_t* nsumsn;
  Float_t* snsumsn;

  Float_t* qspiral; // geometry ordered pixels
  Float_t* nspiral;
  Float_t* snspiral;
  Int_t* IndexofPixspiral;
  Float_t* UofPixspiral;
  Float_t* VofPixspiral;
  Int_t *LineInClusterspiral;
  Int_t *ColumnInClusterspiral;

  Float_t* qsumspiral; // cumulated q,n,sn with s/n ordered pixels
  Float_t* nsumspiral;
  Float_t* snsumspiral;

  // ------------------------
  // Track information, JB 2010/06/03
  // ------------------------
  Float_t TrackToHitDistance;
  Float_t TrackToHit2ndDistance;
  Float_t chi2;
  Float_t tu; // track position in plane
  Float_t tv;
  Float_t tw;
  Float_t tdu; // track slope in plane coord.
  Float_t tdv;
  Float_t tk1; // index of nearest strip to seed
  Float_t tx; // track position in telescope
  Float_t ty;
  Float_t tz;
  Float_t tdx;  // track slope in tracker coord.
  Float_t tdy;

  // ------------------------
  // MiniVector information, JB 2010/08/30
  // ------------------------
  DR3      trackPosUVW[2];
  DR3      trackPosXYZ[2];
  DR3      trackMeanPosXYZ;
  Double_t trackAngleXZ;
  Double_t trackAngleYZ;
	
  DR3      hitPosUVW[2];
  DR3      hitPosXYZ[2];

  DR3      vectorPosXYZ;
  Double_t vectorSlopeXZ;
  Double_t vectorSlopeYZ;
  Double_t vectorAngleXZ;
  Double_t vectorAngleYZ;

  // ------------------------
  // cluster positions
  // ------------------------
  Int_t    hitCounterPos; // JB 2014/01/10
  
  Float_t hX, hY, hZ; // position in tracker frame, JB 2010/08/30
  Float_t hU; // position in plane frame
  Float_t hV;
  Float_t hW;
  Float_t hUdigital; // seed or digital position
  Float_t hVdigital;
  Float_t UCG2x2; // CoG 2x2 ??
  Float_t VCG2x2;
  Float_t UofHitCG3; // CoG using only 3x3 pixels
  Float_t VofHitCG3;
  Float_t UofHitCG5; // CoG using only 5x5 pixels
  Float_t VofHitCG5;
  Float_t UofHitCG; // CoG using all pixels
  Float_t VofHitCG;
  Float_t UCGcorr; // CoG corrected by correlation plot
  Float_t VCGcorr;
  Float_t UofHitEta3; // Eta algo. 
  Float_t VofHitEta3;
  Float_t Eta2x2U;
  Float_t Eta2x2V;
  Float_t UofHitEta2x2;
  Float_t VofHitEta2x2;
  Float_t UofHitEta2x2_new;
  Float_t VofHitEta2x2_new;
  Float_t UofHitEta5x5_new;
  Float_t VofHitEta5x5_new;
  Float_t UofHitEta2x2_newR;
  Float_t VofHitEta2x2_newR;
  Float_t UofHitEta5x5_newR;
  Float_t VofHitEta5x5_newR;
  Double_t Uaht;
  Double_t Vaht;


  // ------------------------
  // used in getmieta and create new eta
  // ------------------------
  Int_t NBins2x2;
  Float_t* Edges2x2;     //!
  Float_t* Contents2x2U; //!
  Float_t* Contents2x2V; //!
  Float_t* Edges3x3;     //!             
  Int_t NBins3x3;
  Float_t* Contents3x3U; //!  
  Float_t* Contents3x3V; //!
  static const Int_t PolDeg = 7;
  Float_t FitParEta3U[PolDeg];
  Float_t FitParEta3V[PolDeg];
  Float_t FitParamU[PolDeg];
  Float_t FitParamV[PolDeg];
    
	
  // ------------------------
  // For cluster shape studies, cdritsa & JB 2010/04/13
  // ------------------------
  Int_t    hitCounter;

  Int_t    nThresholds; // <10
  Float_t  thresholds[10];
  Float_t  snrThresholds[10];
  Int_t    clusterMultiplicity[10];
  Int_t    clusterMultiplicityCounter[10];
  Float_t  multV[10];
  Float_t  multU[10];
  Float_t  mult[10];
  Float_t  formFactor[10];

  Double_t ChargeTimesPositionU[50];
  Double_t ChargeTimesPositionV[50];
  Double_t ChargeTimesPositionUmean[50];
  Double_t ChargeTimesPositionVmean[50];

  Double_t ChargeInCluster[50];
  Double_t sumChargeTimesPositionU[50];
  Double_t sumChargeTimesPositionV[50];
  Double_t sumChargeTimesPositionUmean[50];
  Double_t sumChargeTimesPositionVmean[50];
  
  Double_t RMSV;
  Double_t RMSU;
  Double_t RMSVmean;      
  Double_t RMSUmean;      
  // end for cluster shape
  
  int NPages;
  
  int NTracksPerEventInSensor;
  double AverageEffic[2];

  Int_t   GetFileNumber(); 
  void    GetParameters();
  void    GetAnalysisGoal(); // JB 2014/01/17
  void    ComputePixelPosition(Int_t col, Int_t lin, Float_t &u, Float_t &v); // JB 2012/11/21
  void    ComputePixelPosition_UVToColRow( Double_t u, Double_t v, double &col, double &lin); // JH 2014/07/30
  void    PrepareOnlineDisplay(); // JB 2010/08/30
  void    BookingHistograms() { MHist::BookingHistograms(RunNumber, PixelSizeU, PixelSizeV, NofPixelInRaw, NofPixelInColumn, TrackToHitDistanceLimit, fSession->GetSetup(), fIfReferenceTrack, fIfCalibration, fIfLaserSpot, fIfMiniVector, fIfVertex, fIfFake, fIfImaging);};
  void    CreateNewEta();
  Bool_t  TrackInMimo( Int_t aGeoMatrix, Float_t tuVal, Float_t tvVal, Int_t aSubMatrix=0);
  //void Zero(TObject** ptr,int  size=0) {/*for(int ind=0;ind<size;ind++){ if(!ptr) ptr=0;}*/};
  void    Zero(TObject* ptr) { if(ptr) delete ptr; if(!ptr) ptr=0;};      
  const char*   CreateGlobalResultDir();
  void    PreparePost();
  Int_t   CorStatus; //corpar file status.
  void    SetCalibration(Float_t acalibration=1)    { calibration  = acalibration;}
  void    CreateConfig(int RunNumberBigin, int NumberOfFilesPerRun=0, int RunNumberEnd=0, int RunNumberStep=1 );
  void    CreateLinks(int RunNumberBigin,  int NumberOfFilesPerRun=0, int RunNumberEnd=0, int RunNumberStep=1 );
  Int_t   OpenInputFile(); // JB 2010/09/30

  // ************************************
  // Analysis functions
  
  void GoodHit_Fill(DAuthenticHit *ahit); // JB 2010/10/06

  void ClusterCharges_init(); // JB 2010/06/03
  void ClusterCharges_compute(DAuthenticHit *thehit);
  void ClusterCharges_fill(DAuthenticHit *thehit, Int_t ievt);
  void ClusterCharges_end();
	
  void ClusterPosition_init(); // JB 2014/01/10
  void ClusterPosition_compute(DAuthenticHit *thehit, DPrecAlign *align); // align added, JB 2013/05/01
  void ClusterPosition_cog(DAuthenticHit *thehit);
  void ClusterPosition_eta(DAuthenticHit *thehit);
  void ClusterPosition_aht(DAuthenticHit *thehit);
  void ClusterPosition_fill(DAuthenticHit *thehit);
  void ClusterPosition_end(); // JB 2014/01/10

  void TrackParameters_init() {;} // JB 2010/07/23
  void TrackParameters_compute(DTransparentPlane *atrack, DPrecAlign *align);
  void TrackParameters_allFill(DTransparentPlane *atrack, Int_t ievt);
  void TrackParameters_goodFill(DTransparentPlane *atrack, Int_t ievt);
  void TrackHitPosition_fill(DAuthenticHit *thehit, DTransparentPlane *atrack, Int_t ievt);
  void TrackParameters_end(); // JB 2014/01/17

  void ClusterShape_init();  // JB 2010/04/13
  void ClusterShape_fill(DAuthenticHit *thehit);
  void ClusterShape_end();

  void RefPlane_init(){;}
  void RefPlane_fill(); // JB 2010/05/27
  void RefPlane_end(){;}
  
  void BinarySpecific_init(){;}  // JB 2010/06/03
  void BinarySpecific_HitsOnly_fill(DAuthenticHit *thehit);
  void BinarySpecific_fill(DAuthenticHit *thehit);
  void BinarySpecific_end();
		
  // FakeRate_compute and _fill are kept for consistency
  // but there are so far useless because _end uses histos 
  // filled by 
  // JB 2010/07/22
  void FakeRate_init(){;}
  void FakeRate_compute() {;}
  void FakeRate_fill() {;}
  void FakeRate_end( Int_t rateNormalisation);
	
  // MiniVector analysis
  // JB 2010/08/30 (from NCS 2009/10)
  void MiniVector_init(){;}
  void MiniVector_compute();
  void MiniVector_fill( DAuthenticHit *hit1, DAuthenticHit *hit2);

  //Proba of fake association with inefficient events
  void Inefficent_end();

  // Efficiency computation
  // JB 2011/11/04 (from AB)
  void Efficiency_init();
  void Efficiency_end( Int_t eventsRead);
  
  // Hot pixel removing
  void HotPixel_init( Int_t useMap);
  Int_t HotPixel_test( Int_t aPixelIndex);
  void HotPixel_end( Int_t eventsRead);
  
  // Check what crown the pixel belongs to //clm 2013/01/23
  Bool_t IsPixelIn1stCrown(Int_t lin, Int_t col);
  Bool_t IsPixelIn2ndCrown(Int_t lin, Int_t col);
  
  // Imaging analysis
  Double_t NOfHitsMult[5];
  void ProjectionImaging_init();
  void ProjectionImaging_Fill( DAuthenticHit *thehit, Double_t angle );  
  void ProjectionImaging_end( Int_t numberOfHits );
  
  //====================
 public:
  //====================
  MimosaAnalysis();
  virtual  ~MimosaAnalysis() {;}
  Bool_t CheckIfDone(const Option_t* Option); 
 
  static  MimosaAnalysis*& Instance() { 
    if (!fgInstance) 
      {
        //cout<<"No current TAF session detected! Creatung a new one..."<<endl;  
	fgInstance = new  MimosaAnalysis();  
      } 
    return fgInstance; 
  }

  void MergingPDFFiles(void);

  // ************************************
  // Internal tool methods
  
  void       GetMiEta();
  // void       StoreEta() ;  // replaced by CreateNewEta, JB 2011/06/19
  void       GetAlignment(); // JB 2011/06/19
  Int_t      GetHitMapReadOpt()                  { return Option_read_Pixel_map;}
  //void       SettHitMapReadOpt(Int_t anOpt)      {Option_read_Pixel_map = anOpt;}
  Int_t      GetDebug()                          { return MimoDebug;}
  Int_t      GetRunNumber()                      { return RunNumber;}
  DSession  *GefSession()                        { return fSession;}
  void       SetRunNumber(Int_t aRunNumber)      { RunNumber  = aRunNumber;}
  void       InitRunNumber()                     { RunNumber = fSession->GetRunNumber();}
  Int_t      GetPlaneNumber()                    { return ThePlaneNumber;}
  void       SetPlaneNumber(Int_t aPlaneNumber);
  void       SetLadderNumber(Int_t aNumber); // JB 2014/01/10
  void       InitPlaneNumber()                   { ThePlaneNumber = fSession->GetPlaneNumber(); }
  Int_t      GetMimosaType()                     { return MimosaType;}
  void       SetMimosaType(Int_t aMimosaType)    { MimosaType  = aMimosaType;}
  Int_t      GetAlignStatus()                    { return fSession->GetTracker()->GetAlignmentStatus(); }
  void       SetDebug(Int_t aMimoDebug);
  void       SetAlignStatus(Int_t aStatus)       { fSession->GetTracker()->SetAlignmentStatus( aStatus); } // See DTracker

  Double_t   GetCUT_MaxHitRatePerPixel()               { return CUT_MaxHitRatePerPixel;}
  Double_t   GetCUT_MinHitRatePerPixel()               { return CUT_MinHitRatePerPixel;}
  void       SetCUT_MaxHitRatePerPixel(Double_t aRate) { CUT_MaxHitRatePerPixel = aRate;}
  void       SetCUT_MinHitRatePerPixel(Double_t aRate) { CUT_MinHitRatePerPixel = aRate;}
  
  void       InitMimosaType()                    {  MimosaType = (Int_t)(RunNumber/1000.); if(RunNumber==2110)  MimosaType=4; else if(RunNumber<1000) MimosaType = 99;} // JB 2012/05/04 deal with RunNumber<1000 case

  void       SetFileNumber(Int_t aUserFileNumber=0) {fUserFileNumber = aUserFileNumber;};
  void       SetDSFFile(const Char_t *aFileName) {sprintf( RootFile, "%s", aFileName);}; // JB 2013/09/19
  DPrecAlign*  AlignMimosa( Int_t aDistance);
  //DPrecAlign*  AlignPrecMimosa(DPrecAlign* initAlignment, Bool_t modeAuto); // LC 2012/09/06.
  //TFile*     InitCorPar(Int_t aRun, Int_t aPlane, Int_t &aStatus);
  //void       InitCorPar(Int_t aRun, Int_t aPlane); // Jb 2009/05/18
  void       InitCorPar(Int_t aRun, Int_t aPlane, const char* objType="plane"); // JB 2014/02/10
  Bool_t     IsPlaneInLadder(Int_t aPlaneNumber, Int_t aLadderNumber); // JB 2014/02/10

  int        ResultsStrore();

  Float_t    GetCalibration()                    {return calibration;};
  
  //prepare 
 void       PrepareRun(int RunNumberBigin, Option_t* aType,int NumberOfFilesPerRun=0, int RunNumberEnd=0, int RunNumberStep=1 );


  Float_t   GetMimosaEfficiency()                {return MimosaEfficiency;}
  Float_t   GetMimosaEfficiency_ERR()            {return MimosaEfficiency_ERR;}

  Int_t     GetNtrkInMimo()                      {return NtrkInMimo;} // Number of tracks in the MIMOSA aperture

    
  TFile*    GetCorParFile() {
    if (!theCorFile) { InitCorPar(GetRunNumber(),GetPlaneNumber()); }
    return theCorFile;
  }    


  //----ADC   
  Float_t tVect[100]; //n res
  // Int_t       *fChannel;                  //! pointer to Channels 
  //Int_t&       GetChannel(Int_t aSk)              { return  fChannel[aSk-1];     }
  Float_t* GettVect()  {return tVect;} ; //n res
  //----ADC

  //MC simulation parameters
  bool UsingTrackerResolution;
  int  Bins_GlobalResolution;
  double DUT_residualU_CGDSF[2];
  double DUT_residualV_CGDSF[2];
  double DUT_resolutionU_CGDSF[2];
  double DUT_resolutionV_CGDSF[2];
  double Tel_resolutionU_CGDSF[2];
  double Tel_resolutionV_CGDSF[2];
  double From_mu_to_mm;
  int _PrintFreq;
  std::vector<int>   _PlaneList; //Plane indexes in orderer by Z coordinate, from lower to higher.
  std::vector<float> _PlaneListZ;
  std::vector<float> _PlaneThickness;
  std::vector<TString> _PlaneMaterial;
  std::vector<float> _RU_min;
  std::vector<float> _RU_max;
  std::vector<float> _RV_min;
  std::vector<float> _RV_max;
  double MyRX[2];
  double MyRY[2];
  double MyRZ[2];
  double DUTGeomMatrixX[2];
  double DUTGeomMatrixY[2];
  double RGlobalResolution[2];
  Color_t color[500];
  double Nsigma;
  int *TracksPerEvt;
  int *NRectracks;

  // ************************************
  // Arrays for eta correction
  
  TArrayF Liste_CoGU;
  TArrayF Liste_CoGV;
  Int_t nListe_CoG;
  TArrayF READListe_CoGU;
  TArrayF READListe_CoGV;
  Int_t READnListe_CoG;

  TArrayF Liste_CoGU_eta2x2;
  TArrayF Liste_CoGV_eta2x2;
  Int_t nListe_CoG_eta2x2;
  TArrayF READListe_CoGU_eta2x2;
  TArrayF READListe_CoGV_eta2x2;
  Int_t READnListe_CoG_eta2x2;

  TArrayF Liste_CoGU_eta5x5;
  TArrayF Liste_CoGV_eta5x5;
  Int_t nListe_CoG_eta5x5;
  TArrayF READListe_CoGU_eta5x5;
  TArrayF READListe_CoGV_eta5x5;
  Int_t READnListe_CoG_eta5x5;

  //  Int_t GetREADnListe_CoG()                    { return READnListe_CoG;                  }
  //  void  SetREADnListe_CoG(Int_t aREADnListe_CoG)    { READnListe_CoG  = aREADnListe_CoG;           }


  // ************************************
  // Comonly used commands

  void       CDHist() { if(!CheckIfDone("mimosapro")) {return;} else { MHist::Dir();}}
  void       Help();
  
  void       InitSession(const Int_t TheRun,const unsigned int ThePlane=0, Int_t EventBuildingMode=-1,TString ConfigFile="", TString ConfigDir=""); 
  //EventBuildingMode key by default is at -1 meaning that it will not override the value taken in DSetup::ReadConfiguration(), SS 2011/11/14
  // VR 2014/06/30 Add optional args
  
  // ************************************
  // Raw data reconstruction commands

  void       MakeEta(int NEvt = 10000);
  
  void       SetTrackGeoLimitsForAlign(Double_t xmin, Double_t xmax, Double_t ymin, Double_t ymax) { fSession->SetTrackGeoLimitsForAlign(xmin, xmax, ymin, ymax); } // JB 2013/06/11
  void       SetTrackChi2LimitForAlign( Double_t aLimit) { fSession->SetTrackChi2LimitForAlign( aLimit); } // JB 2013/07/14
  void       AlignTracker(const Double_t tiniBoundU,
			  const Double_t tiniBoundV,
			  Int_t nAlignEvents=4000,
			  Int_t nAdditionalEvents=2000,
			  bool UseAllHits = false); // 2013/06/10.
                                                    // AP 2015/06/10: added bool parameter (UseAllHits) to decide if doing alignement with all hits or the closest one to the track.
  void       AlignTracker(const Double_t tiniBound = 480.,
			  Int_t nAlignEvents=4000,
			  Int_t nAdditionalEvents=2000, 
			  bool UseAllHits = false) { AlignTracker( tiniBound, tiniBound, nAlignEvents, nAdditionalEvents, UseAllHits); }
  void       AlignTrackerMinuit(Bool_t modeAuto=1,
				const Double_t tiniBound=480.,
				Int_t nAlignEvents=5000,
				Int_t nAlignHitsInPlane=4000,
				Int_t nAdditionalEvents=2000,
				Double_t chi2Limit=0.,
				bool UseAllHits = true); // LC 2012/09/06
                                                         // AP 2015/06/08 : adding bool parameter (UseAllHits) to decide if doing alignment with all hits or the closest one
  void       AlignTrackerMinuitLadder(Bool_t modeAuto =1, const Double_t tiniBound=480., Int_t nAlignEvents=5000, Int_t nAlignHitsInPlane=4000, Int_t nAdditionalEvents=2000, Double_t chi2Limit=0.);
  //void       AlignLadder(Bool_t modeAuto=1, const Double_t tiniBound=480., Int_t nAlignEvents=5000, Int_t nAlignHitsInPlane=4000, Int_t nAdditionalEvents=2000, Int_t ladderFace=1, Double_t chi2Limit=0.);  // Commented 2014/12/18
  void        AlignLadderMV(Bool_t modeAuto=1, const Double_t tiniBound=480., const Double_t boundSlopes=100., Int_t nAlignEvents=5000, Int_t nGoodTracks=1000, Int_t nAdditionalEvents=2000, Int_t chi2Limit=40, Int_t mode=0); // LC 2013/09/10
  void       AlignTrackerGlobal(const Int_t refPlane=1, const Int_t nEvents=4000, const Int_t nIterations=10, const Bool_t alignConstrainsts=true, const Bool_t trackConstrainsts=true, const Bool_t multipleScatteringFit=true); // LC && LiuQ 2015/02/06. 
  void       AlignTrackerMillepede(Int_t nAlignEvents=4000);  // LC 2012/12/24.
//  void        Gener(Double_t* xp, Double_t* yp, Double_t& aX, Double_t& bX, Double_t& aY, Double_t& bY, Double_t* sigX, Double_t* disX, Double_t* sigY, Double_t* disY, Double_t* z, Double_t* phi); // LC 2012/01/07

  void       DSFProduction(Int_t NEvt = 500000, Int_t fillLevel=1);
  void       StudyDeformation(const Float_t tiniBound = 480., Int_t nEvents=2000, Bool_t fitAuto=0); // BB 2014/05/20


  // ************************************
  // Analysis commands

  DSession*  GetSession() {return fSession;} // JB 2013/06/23
  MRaw*      GetRaw();
  MRax*      GetRax();
  void       MimosaPro(Int_t MaxEvt, Float_t TrackHitDist, Float_t S2N_seed, Float_t S2N_neighbour, Int_t submatrix, Int_t GeoMatrix, Option_t*  SaveAlign, Int_t UseHitMap=0, Int_t WriteMissedHits=0, Int_t WriteGoodHits=0);
  void       MimosaFakerate(Int_t MaxEvt, Float_t S2N_seed, Float_t S2N_neighbour, Int_t submatrix=0, Int_t GeoMatrix=0);
  void       MimosaCalibration(Int_t MaxEvt, Float_t S2N_seed = 0., Float_t S2N_neighbour = 0., Float_t ChargeAroundSeedCutMax = 5000., Int_t submatrix=0, Int_t GeoMatrix=0);
  void       MimosaMiniVectors(Int_t MaxEvt, Int_t TrackHitDist, Short_t plane0, Float_t S2N_seed0, Float_t S2N_neighbour0, Int_t submatrix0, Int_t GeoMatrix0, Short_t plane1, Float_t S2N_seed1, Float_t S2N_neighbour1, Int_t submatrix1, Int_t GeoMatrix1);
  void       MimosaPro2Planes(Int_t MaxEvt, Int_t TrackHitDist, Short_t plane0, Float_t S2N_seed0, Float_t S2N_neighbour0, Int_t submatrix0, Short_t plane1, Float_t S2N_seed1, Float_t S2N_neighbour1, Int_t submatrix1, Int_t GeoMatrix);
  void       MimosaVertex(Int_t MaxEvt, Short_t plane1, Short_t plane2);
  void       MimosaVertexFinder(Int_t MaxEvt, Int_t submatrix , Int_t GeoMatrix, Float_t chi2Cut);                                                                        
  void       MimosaCluster(Int_t MaxEvt, Float_t S2N_seed = 0., Float_t S2N_neighbour = 0., Int_t submatrix=0, Int_t GeoMatrix=0); // JB 2014/01/17
  void       MimosaProLadder(Int_t MaxEvt, Int_t TrackHitDist, Float_t S2N_seed, Float_t S2N_neighbour  , Int_t submatrix , Int_t GeoMatrix ,Option_t*  SaveAlign,  Int_t UseHitMap, Int_t WriteMissedHits, Int_t WriteGoodHits); // JB 2014/02/1
  
  void       MimosaImaging(Int_t MaxEvt, Int_t submatrix=0, Int_t GeoMatrix=0, Double_t theta_init=0.0, Int_t Npeak=1, Double_t S=100, Double_t W=100, Bool_t FirstLoop=kTRUE, Bool_t chooseFit=kTRUE); // JH 2014/06/13
  
  void       MimosaGeneration_ToyMC(Int_t MaxEvt   = 10000,
			            Int_t PlaneNumber = 1,
			            Int_t Submatrix = 0,
			            Int_t Geomatrix = 0,
			            bool  DoGaussianMS = true,
			            Int_t seed     = 8292894,
			            bool DoDisplay = false,
			            double XMeanDiv = 0.0,
			            double XRMSDiv  = 1.0e-20,
			            double YMeanDiv = 0.0,
			            double YRMSDiv  = 1.0e-20,
			            bool CalledFromMimosaPro = false,
			            bool verbose   = false);    //AP 2015/03/11
  
  void       MimosaGeneration_LoicG4Simu(Int_t MaxEvt   = 10000,
			                 Int_t PlaneNumber = 1,
					 Int_t Submatrix = 0,
			                 Int_t Geomatrix = 0,
					 Int_t seed     = 8292894,
			                 bool DoDisplay = false,
			                 bool verbose   = false);    //AP 2015/07/08
  
  void       MimosaGeneration_APG4Simu(Int_t MaxEvt   = 10000,
			               Int_t PlaneNumber = 1,
				       Int_t Submatrix = 0,
			               Int_t Geomatrix = 0,
				       Int_t seed     = 8292894,
			               bool DoDisplay = false,
			               bool verbose   = false);    //AP 2015/07/29
  
  void       BookingMC(Int_t PlaneNumber = 1,
		       Int_t Submatrix = 0,
		       Int_t Geomatrix = 0,
		       bool CalledFromMimosaPro = false); //AP 2015/07/29
  void       DoMCDisplay(int NDisplay,bool verbose);      //AP 2015/07/29
  void       DoMCDisplay3D(int NDisplay,bool verbose,
			   const char* file_gdml);        //AP 2015/07/31
  double     DistanceOfPointToPlane(const int PlaneNumber,
                                    DR3 Point);           //AP 2015/08/03
  DR3        ClosestPointInPlaneToPoint(const int PlaneNumber,
                                        DR3 Point);       //AP 2015/08/03

  float      GetTrackDistantToClosestDiode(float tu,
					   float tv);

  // ************************************
  // Plotting functions
  
  void       MimosaResolution();
  void       CheckMCSimulation(bool StandAlone = false);
  void       MimosaClusterCharge( TString fitOption="landau" );
  void       MimosaBinaryplots();
  void       CheckMimosaAlign();
  void       CheckAsymmetry();
  void       InspectEfficiency();
  void       checkRefPlane();
  void       CompareBadGoodRaw();
  void       CheckClusters();
  void       Checkreferencetracks();
  void       pixelcharge();
  void       MimosaSN(); 
  void       ClusterShape(); //cdritsa feb 2008, JB 2010
  void       HitMap(); // JB 2010/10/06
  void       MimosaPixelHomogeneity();
  void       MimosaOptimize();
  void       FakeRate();
  void       Calibration(); // JB 2010/07/27
  TPad*      PlotCalibration(TH1F* h, Int_t manualFitRange=0);
  void       MiniVectors(); // Jb 2010/08/21
  void       UserAnalysis();
  void       SaveGifFile();
  void       Clear(Option_t* /*option = ""*/);
  void       CheckImaging(Double_t theta_init=62.0, Int_t Npeak=1, Double_t S=100, Double_t W=100, Bool_t FirstLoop=kTRUE, Bool_t chooseFit=kTRUE);  // JH 2014/06/16

  // ************************************



  ClassDef(MimosaAnalysis,2) 
    };

//R__EXTERN MimosaAnalysis *gTAF; // JB, for inline compilation


#ifndef MAC 
//#define gTAF MimosaAnalysis::Instance() // to comment for MAC 
#endif


#endif 


