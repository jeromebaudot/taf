// @(#)maf/maf:$Name:  $:$Id: MHist.h,v.1 2005/10/02 18:03:46 sha Exp $
// Author: A. Shabetai 

#ifndef _MHist_included_
#define _MHist_included_


  /////////////////////////////////////////////////////////////
  //                                                         //
  //  Contains all analysis  histograms and canvas           //
  //  (those which are filled by MimosaPro())                //
  //                                                         //   
  ////////////////////////////////////////////////////////////


#include "TH3F.h"
#include "TCanvas.h"
#include "TText.h"
#include "TGraphErrors.h"
#include "TObject.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "Riostream.h"
#include "TRefArray.h"

#include "TSystem.h"
#include "TInterpreter.h"

#include "TControlBar.h"

//---ADC
#include "DSetup.h"
#include "DSession.h"
#include "DTracker.h"
#include "DPlane.h"
#include "DAlign.h"
#include "DHit.h"
//#include "MAnalysis.h"
//---ADC
//class MimosaAnalysis;
class DSetup;         // forwards
class DSession;         // forwards
//---ADC
//---ADC

class MHist : public TObject { 

  public:
  
  MHist();
  virtual ~MHist();
  
  TObject**  Zero(TObject** ptr,int  size=0) {for(int ind=0;ind<size;ind++){ if(!ptr[ind]) ptr[ind]=0;}; ptr=0;return ptr;};
  TCanvas**  Zero(TCanvas** ptr,int  size=0) {for(int ind=0;ind<size;ind++){ if(!ptr[ind]) ptr[ind]=0;}; ptr=0;return ptr;}; //
  TProfile**  Zero(TProfile** ptr,int  size=0) {for(int ind=0;ind<size;ind++){ if(!ptr[ind]) ptr[ind]=0;}; ptr=0;return ptr;};
  TProfile2D**  Zero(TProfile2D** ptr,int  size=0) {for(int ind=0;ind<size;ind++){ if(!ptr[ind]) ptr[ind]=0;}; ptr=0;return ptr;};
  TH1F**  Zero(TH1F** ptr,int  size=0) {for(int ind=0;ind<size;ind++){ if(!ptr[ind]) ptr[ind]=0;}; ptr=0;return ptr;};
  TH2F**  Zero(TH2F** ptr,int  size=0) {for(int ind=0;ind<size;ind++){ if(!ptr[ind]) ptr[ind]=0;}; ptr=0;return ptr;};
  TH3F**  Zero(TH3F** ptr,int  size=0) {for(int ind=0;ind<size;ind++){ if(!ptr[ind]) ptr[ind]=0;}; ptr=0;return ptr;};

  TObject*  Zero(TObject* ptr)  { dir->cd(); if(ptr) {delete ptr;ptr=0;}return ptr; };
  TCanvas*  Zero(TCanvas* ptr) { dir->cd(); if(ptr) {delete ptr;ptr=0;}return ptr; };
  TProfile*  Zero(TProfile* ptr){ dir->cd(); if(ptr) {delete ptr;ptr=0;}return ptr; }; 
  TProfile2D*  Zero(TProfile2D* ptr){ dir->cd(); if(ptr) {delete ptr;ptr=0;}return ptr; }; 
  TH1F*  Zero(TH1F* ptr) { dir->cd(); if(ptr) {delete ptr;ptr=0;}return ptr;};      
  TH2F*  Zero(TH2F* ptr)  { dir->cd(); if(ptr) {delete ptr;ptr=0;}return ptr;};
  TH3F*  Zero(TH3F* ptr)  { dir->cd(); if(ptr) {delete ptr;ptr=0;}return ptr;};
  TGraphErrors*  Zero(TGraphErrors* ptr) { dir->cd(); if(ptr) {delete ptr;ptr=0;}return ptr; };

  TH1F* AutoZoom(TH1F* H,Option_t* aType="all", Int_t EntryMin=0);
  TH2F* AutoZoom(TH2F* H,Option_t* aType="all", Int_t EntryMin=0);
  void Dir();
  void Clear(Option_t* /*option*/ = "");
  void BookingHistograms(Int_t RunNumber, Float_t PixelSizeU, Float_t PixelSizeV, Int_t NofPixelInRaw, Int_t NofPixelInColumn, Float_t TrackToHitDistanceLimit, DSetup *aSetup, Bool_t ifReferenceTrack=kFALSE,
			 Bool_t ifCalibration=kFALSE, Bool_t ifLaserSpot=kFALSE, Bool_t ifMiniVector=kFALSE, Bool_t ifVertex=kFALSE, Bool_t ifFake=kFALSE, Bool_t ifImaging=kTRUE);
  MHist* theHist;  

 protected:
  TDirectory *saved; 
  TDirectory *dir;  
  
  Char_t titre[150]; 
  Char_t nom[50]; 

  Int_t   MaxNofPixelsInCluster; 
  Int_t   MinNofPixelsInCluster; // JB 2013/09/12
  static const int MaxNofPixelsInClusterMax=361; // a nodif 

  TH1F *hdummy ;
  
  
  //------------------------------------------------------------------------------
  //----- general control
  //------------------------------------------------------------------------------

  TH1F *selection ;
  TText *textLabel[9];

  TCanvas *MainCanvas;

  Float_t geomUmin, geomUmax;
  Float_t geomVmin, geomVmax;

  //------------------------------------------------------------------------------ 
  // Charges, noise, S/N, pixel multiplicity plots for SELECTED clusters
  //------------------------------------------------------------------------------ 

  // seed
  TH1F *hChargeInSeed ;
  TH1F *hRealTrackNoise; 
  TH1F *hsn  ;
  TH1F *hSNReal;

  // 2nd higest S/N pixel
  TH1F *hS2N2ndRH ;
  TH1F *hSNNReal;
  TH1F *hind ;

  // all pixels
  TH1F *hChargeInCluster; 
  TH1F* hqcn[MaxNofPixelsInClusterMax] ; 
  TH1F* hqcngeom[MaxNofPixelsInClusterMax] ; 
  TH1F* hindivq[MaxNofPixelsInClusterMax] ; 
  TH1F* hsnn[MaxNofPixelsInClusterMax] ;
  
  TH1F *hChargeOrder1 ;
  TH1F *hChargeOrder2 ;
  TH1F *hChargeOrder3 ;
  TH1F *hChargeOrder4;
  TH1F *hChargeOrder5 ;
  TH1F *hChargeOrder6 ;
  TH1F *hChargeOrder7;
  TH1F *hChargeOrder8 ;
  TH1F *hChargeOrder9;

  TH2F *hChargeCor_1_2; 
  TH2F *hChargeCor_1_3 ;
  TH2F *hChargeCor_1_4 ;
  TH2F *hChargeCor_2_3 ;

  TH1F *hChargeRap_1_over_2;
  TH1F *hChargeRap_1_over_3;
  TH1F *hChargeRap_1_over_4;
  TH1F *hChargeRap_2_over_3 ;

  TH1F *hQofPix3x3 ;
  TH1F *hQ3x34 ;
  TH1F *hQ3x35 ;
  TH1F *hQ3x345 ;
  TH1F *hQ3x327 ;
  TH1F *hQ3x345r;
  TH1F *hQ3x327r ;

  // cluster wise 
  TH1F *hsnc;
  TH1F *hChargeSum_4 ;
  TH1F *hqc_c   ;
  TH1F *hqc    ;
  TH1F *hChargeInCross ;
  TH1F *hChargeInX ;
  TH1F *hChargeInCrown1 ;
  TH1F *hChargeInCrown2 ;
  TH1F *hqcel  ;
  TH1F *hsnc1  ;
  TH1F *hoptimalsize;
  TH1F *hSNneighbour;
  TH2F *hSNseedvsSNneighbour; // JB 2013/11/07
  TH2F *hQseedvsQcluster; // JB 2014/05/22
  TH2F *hQseedvsQneighbour; // JB 2013/11/08
  TH2F *hSNseedvsQcluster; // JB 2014/05/22
  TH1F *hnpix_c  ;
  TH1F *hnpixCumul_c;

  static const Int_t jpixmax=8;
  TH1F *hsn_pix_0[jpixmax] ;
  TH1F *hsn_pix_1[jpixmax] ;
  TH2F *hsn_seed_vs_pix_0[jpixmax];
  TH2F *hsn_seed_vs_pix_1[jpixmax];

  // canvas
  TCanvas *cClusterCharge;

  //------------------------------------------------------------------------------
  //--- Charges for calibration peak
  //------------------------------------------------------------------------------

  TH1F* hqSeedCalibration;
  TH1F* hqNeighbourCalibration;
  TH2F* hqSeedVsNeighbourCalibration;

  TCanvas *cCalibration;
  TCanvas* Calib_distr1;
  TCanvas* Calib_distr2;
  TCanvas* Calib_distr3;


  //------------------------------------------------------------------------------
  //----- Comparebadgoodraw()
  //------------------------------------------------------------------------------
  TH1F *hraw1badone ;
  TH1F *hraw1goodone ;
  TH2F *hraw1goodone_time ;
  TH2F *hraw1badone_time ;
  TH2F *hraw1goodoneNoise_time;
  TH2F *hraw1badoneNoise_time;
  TH2F *hRealTrackNoise_time ;

  TH1F *hraw1PFrfr1GOOD;
  TH1F *hraw1PFrfr1BAD;
  TH2F *hraw1PFrfr1GOOD_time;
  TH2F *hraw1PFrfr1BAD_time;
  TH1F *hraw1PFrfr2GOOD;
  TH1F *hraw1PFrfr2BAD;
  TH2F *hraw1PFrfr2GOOD_time;
  TH2F *hraw1PFrfr2BAD_time;
  TH2F *hraw1NoiseGOOD_time;
  TH2F *hraw1NoiseBAD_time;

  TH1F *hraw1Noise;
  TH1F *hraw1Pedestal;
  TH1F *hraw1CDS;
  TH1F *hraw1Signal;
  TH2F *hraw1Noise_time;
  TH2F *hraw1Pedestal_time;
  TH2F *hraw1CDS_time;
  TH2F *hraw1Signal_time;


  //------------------------------------------------------------------------------
  //--- efficiency
  // MG 2010/06/04
  //------------------------------------------------------------------------------
  
  TH2F *effimap;
  TH2F *goodtracks;
  TH2F *TrkInMimo;
  
  //------------------------------------------------------------------------------
  //--- hit position
  //------------------------------------------------------------------------------

  TCanvas *cHitMap;
  TCanvas *cPosStudy1;
  TCanvas *cPosStudy2;

  //------------------------------------------------------------------------------
  //--- ?
  //------------------------------------------------------------------------------

    TH2F *hseedQvsS2NAll;
    TH2F *hseedQvsS2NGood ;
    TH1F *hQseedAll ;
    TH1F *hIndex2x2  ;
    TH1F *hnhit ;
    TH2F *vec   ;
    TH2F *tudv  ;
    TH2F *tvdu  ;
    TH2F *hudv   ;
    TH2F *hvdu ;
    TH1F *tuhu1 ;
    TH1F *tvhv1  ;
    TH2F *tuhu   ;
    TH2F *tvhv ;
    TH2F *tuhv  ;
    TH2F *tvhu  ;

    TH2F *hAllHvvsAllTv ;
    TH2F *hAllHuvsAllTv ;
    TH2F *hAllHvvsAllTu ;
    TH2F *hAllHuvsAllTu  ;

    TH2F *huCG5tu  ;
    TH2F *hvCG5tv  ;
    TH2F *huCGtu   ;
    TH2F *hvCGtv ;
    TH2F *huCGtuInPix ;
    TH2F *hvCGtvInPix;
    TH2F *huCGtuInPix5 ;
    TH2F *hvCGtvInPix5 ;
    TH2F *htuvInPix ;
    TProfile *ProfUCG ;
    TProfile *ProfVCG ;
    TProfile *ProfACGn;
    TH2F *hUeta3TuInPix;
    TH2F *hVeta3TvInPix ;
    TH2F *hUcorTuInPix ;
    TH2F *hVcorTvInPix ;
    TH2F *huCGtuInPix4;
    TH2F *huCG2x2tuInPix ;
    TH2F *hvCG2x2tvInPix; 
    TH1F *hEta2x2tu1L ;
    TH1F *hEta2x2tv1L ;
    TH2F *hEta2x2tu2L ;
    TH2F *hEta2x2tv2L ;
    TH1F *hCG2x2tu1L;
    TH1F *hCG2x2tv1L ;

    TH1F *hEta2x2 ;
    TH1F *hEta2x2U;
    TH1F *hEta2x2V  ;
    TH1F *hEta2x2m ;

    TH1F *hEta2x2UL  ;
    TH1F *hEta2x2VL;


    TH1F *hdCGEtaU ;
    TH1F *hdCGEtaV   ;
    TH1F *hdCGDigU;
    TH1F *hdCGDigV;
    TH2F *hdCGDigUV;
    TH1F *htmp5;
    TH2F *hEta2x2tu2 ;
    TH2F *hEta2x2tv2 ;
    TH2F *hTHCorr2 ;
    TH1F *hDifCorU ;
    TH1F *hDifCorV ;
    TH1F *hAllHuvsAllTu2 ;
    TH1F *hAllHvvsAllTv2  ;
    TH1F *hAlignHuTu ; //JB 2010/03/12
    TH1F *hAlignHvTv ; //JB 2010/03/12
    TH2F *hAlignHuTuvsTv; //JB 2010/03/12
    TH2F *hAlignHvTvvsTu; //JB 2010/03/12
    TH2F *hAlignHuTuvsTu; //JB 2013/07/16
    TH2F *hAlignHvTvvsTv; //JB 2013/07/16
    TH2F *hAlignHuTuvsHv; //JB 2010/03/12
    TH2F *hAlignHvTvvsHu; //JB 2010/03/12
    TH1F *hAllHvvsAllTv3  ;
    TH2F *h2dallhits   ;
    TH2F *h2dgoodhits   ;
    TH2F *h2dmatchedhits   ;
    TH2F *h2DpictureMatched   ; // JB 2014/01/10
    TH2F *hEta2x2vsInd ;  
    TH2F *hChargeVsPosition; 
    TH1F *hChargeVsDistance; // clm 2013/07/16 
    TH2F *hNorm ;
    TH2F *hAllHitsInPixel; 
    TH1F *h1RmsOnTheta; // JH 2014/06/16
    TH2F *h2RmsOnThetaScanVsThetaCut; // JH 2014/07/30
    TH1F *h1ProjectionOnX; // JH 2014/06/17
    TH1F *h1ProjectionOnY; // JH 2014/06/19
    TH1F *h1ProjectionOnXMult[4]; // JH 2014/06/19
    TH1F *h1ProjectionOnYMult[4]; // JH 2014/06/19
    TH1F *h1NumberOfHitsMult; // JH 2014/06/25
    TH2F *h2GoodHitsMult[4]; // JH 2014/06/25
    TH1F *h1Sigma; // JH 2014/06/19
    TH2F *hdCGDigUVMult[6]; // JH 2014/07/22
    TH1F *h1SigmaSlices; // JH 2014/07/29
    TH1F *h1SigmaSlices_Pull; // JH 2014/07/29


    TH2F *huv   ;
    TH2F *hxy   ;
    TH2F *huvBad  ;
    TH2F *hxyBad  ;
    TH2F *tuv    ;
    TH2F *tuv1  ;

    TH1F *hhu ;   
    TH1F *hhv ;
    TH1F *hhx ;   
    TH1F *hhy ;
    TH1F *hhuS  ;
    TH1F *hhvS ;  
    TH1F *htu;    
    TH1F *htv  ;
    //---ab 
    TH2F *htuhtv  ;
    TH1F *hGOODqcel ;

    TH1F *hgoodSeedPixel;
    TH2F *h2DgoodSeedPixel ;
    TH1F *hSeedBetweenDist ;
    TH1F *hEta3DigU ;
    TH1F *hEta3DigV ;
    TH1F *hqc_nc   ;

    TH1F *hnpix  ;
    TH1F *hnpix_nc;

    TH2F *FalseHitMap; 
    TH1F *hClusterChargeProfile;
    TH1F *hClusterChargeNorm ;
    TH1F *etal[2] ;
    TH1F *etal1[2] ;
    TH1F *vecu[25];
    TH1F *vecv[25]    ;
    TH1F *hChargeInSeedInGroup[64];
    TH1F *DuvCG ;
    TH1F *duvall ;

    TH1F *hCDSvar ;
    TH2F *CDSVarvsTime; 
    TH1F *dtime ;

  //------------------------------------------------------------------------------
  //----- Spatial Resolution
  //------------------------------------------------------------------------------
    TH1F *hAllHuvsAllTu1;  
    TH1F *hAllHvvsAllTv1 ;
    TH1F *huCGtu1 ;
    TH1F *hvCGtv1   ;
    TH1F *huCGtu2 ;
    TH1F *hvCGtv2   ;
    TH1F *hCG2x2tu1;
    TH1F *hCG2x2tv1 ;
    TH1F *hCG5URes ;
    TH1F *hCG5VRes   ;
    TH1F *hTuHuCorr;
    TH1F *hTvHvCorr;
    TH1F *hEta2x2tu1;
    TH1F *hEta2x2tv1 ;
    TH1F *hEtaURes;
    TH1F *hEtaVRes ;
    TH1F *hEta3URes ;
    TH1F *hEta3VRes ;
    TH1F *hEtaU_2x2Res;
    TH1F *hEtaV_2x2Res;
    TH1F *hEtaU_5x5Res;
    TH1F *hEtaV_5x5Res;
    TH1F *hAHTURes;
    TH1F *hAHTVRes;

    TCanvas *cres;
    TCanvas **cresfit; // JB 2009/09/14


    //------------------------------------------------------------------------------ 
    // Fake rate study
    //------------------------------------------------------------------------------ 
    TH1F *hNhitperpixel;
    TH1F *hNhitRateperpixel;
    TH1F *hPixelsPerFakeRate;

    TCanvas *cfake;

    //------------------------------------------------------------------------------ 
    // tracks properties and chi2  
    //------------------------------------------------------------------------------ 

    TH1F *hNTracksPerEvent;
    TH1F *hNTracksPerEventievt;
    TH1F *hNGoodGeomTracksPerEvent;
  
    TH2F *hAllTvTu;
    TH1F *hAllTu ;
    TH1F *hAllTv    ;
    TH2F *hGoodChi2TvTu;
    TH1F *hGoodChi2Tu  ; 
    TH1F *hGoodChi2Tv   ;
    TH1F *hGoodChi2Tx    ;
    TH1F *hGoodChi2Ty    ;
    TH1F *hGoodChi2AngleXZ;
    TH1F *hGoodChi2AngleYZ;

    TH1F *hchi2_c;  
    TH1F *hchi2_nc ;
    TH1F *hchi2 ;

    TH1F *hTrackToClusterMinDistance ; 

    TH1F *hNhitsPerTrack_all;
    TH1F *hNhitsPerTrack_good;
    TH1F *hNhitsPerTrack_matched;

    static const Int_t NRefPlane = 10;
    TH2F *hRef_Tud_vs_Tv[NRefPlane];
    TH2F *hRef_Tud_vs_Tu[NRefPlane];
    TH2F *hRef_Tud_vs_TDOX[NRefPlane];
    TH2F *hRef_Tud_vs_TDOY[NRefPlane];
    TH2F *hRef_Tud_vs_Chi2[NRefPlane];
    TH2F *hRef_Tud_vs_Tu1[NRefPlane];
    TH2F *hRef_Tud_vs_Tk1[NRefPlane];

    Char_t titreref[100] ; 
    Char_t nomRef[50]; 

    //------------------------------------------------------------------------------ 
    // Good/All hits. 
    //------------------------------------------------------------------------------ 
    TH1F *hnhitievt ;
    TH2F *hnahitievt;
    TH1F *hnGOODhit ;
    TH1F *hnGOODhitwhentrack;

    TH1F *hAllS2N ;
    TH1F *hallhitSN ;
    TH1F *hallSNneighbour; 
    TH1F *hgoodSNneighbour;
    TH2F *hgoodSN_vs_SNN ;
    TH2F *hallSN_vs_SNN;

    TH2F *hallSN_minus_hgoodSN_vs_SNN ;
    TH2F *hdistchi2 ;

    //------------------------------------------------------------------------------
    // REAL S/N NEIGHBOURS ----ab25072005
    //------------------------------------------------------------------------------

    TH2F *hSN_vs_SNNReal;

    //------------------------------------------------------------------------------ 
    // Charge by ordered pixels Histos. 
    //------------------------------------------------------------------------------ 
    //-------------
    TH1F *hGOODChargeOrder1 ;
    TH1F *hGOODChargeOrder2 ;
    TH1F *hGOODChargeOrder3;
    TH1F *hGOODChargeOrder4 ;
    TH1F *hGOODChargeOrder5 ;
    TH1F *hGOODChargeOrder6 ;
    TH1F *hGOODChargeOrder7;
    TH1F *hGOODChargeOrder8 ;
    TH1F *hGOODChargeOrder9 ;
    TH2F *hGOODChargeCor_1_2 ;
    TH2F *hGOODChargeCor_1_3;
    TH2F *hGOODChargeCor_1_4;
    TH2F *hGOODChargeCor_2_3 ;
    TH1F *hGOODChargeSum_4 ;
    TH1F *hGOODChargeRap_1_over_2; 
    TH1F *hGOODChargeRap_1_over_3 ;
    TH1F *hGOODChargeRap_1_over_4;
    TH1F *hGOODChargeRap_2_over_3;
   
  //------------------------------------------------------------------------------
  //----- ?
  //------------------------------------------------------------------------------

  // histograms needed for charge distribution function: 
  TH2F *hS2N2All; 
  TH1F *hS2N2nd ;
  TH2F *hS2N2RH ;
  //Int_t nb=25; Int_t SpreadRange=100; 
  TH1F *hPedestal;

    //----ALL HITS: Signal / noise seed vs Signal / noise in the pixel range [i-j]  
    TH2F *hsnALL_seed_vs_pix_0[jpixmax];
    TH2F *hsnALL_seed_vs_pix_1[jpixmax];

  TH2F *hChargeIntegral1;
  TH2F *hChargeNorm1;
  TH2F *hChargeIntegral2; 
  TH2F *hChargeNorm2 ;
  TH2F *hChargeIntegral3 ;
  TH2F *hChargeNorm3 ;
  TH2F *hChargeIntegral4;
  TH2F *hChargeNorm4 ;
  TH2F *hChargeIntegral;
  TH2F *hChargeIntSmoothed;



    //------------------------------------------------------------------------------
    // Check pixel homogeneity depending on real track position
    //------------------------------------------------------------------------------
    TH2F *hHOM_tu_tv_modulo;
    TH2F *hHOM_ResU_tu;
    TH2F *hHOM_ResV_tv;
    TProfile *ProfHOM_ResU_tu;
    TProfile *ProfHOM_ResV_tv;

    TH2F *hHOM_modUCG_modtu;	    
    TH2F *hHOM_modVCG_modtv; 	    
    TH2F *hHOM_modUeta3_modtu;
    TH2F *hHOM_modVeta3_modtv;
    TH2F *hHOM_modUeta3_realtu;	    
    TH2F *hHOM_modVeta3_realtv; 	    
    TH2F *hHOM_modUCG_realtu;	    
    TH2F *hHOM_modVCG_realtv;	    
    TH2F *hHOM_modUeta3_Eta3U;	    
    TH2F *hHOM_modVeta3_Eta3V;	    
    TH2F *hHOM_modUeta3_modVeta3;  
    TH2F *hHOM_modUCG_modVCG;         
    TH2F *hHOM_modUeta3_modUCG;
    TH2F *hHOM_modVeta3_modVCG;

    TH2F *hHOM_Charge_diodedist_alg;
    TH2F *hHOM_Charge_diodedist_alg_u;//clm
    TH2F *hHOM_Charge_diodedist_alg_v;//clm
    TH2F *hHOM_Charge_diodedist;
    TH2F *hHOM_Charge2_diodedist;
    TH2F *hHOM_Charge4_diodedist;
    TH2F *hHOM_Charge9_diodedist;
    TH2F *hHOM_Charge25_diodedist;
    TH2F *hHOM_Noise_diodedist;

    // start CLM 2013/01/23
  TProfile2D *ProfhGOODCharge_Charge_DiodePosition;
  TProfile2D *ProfhGOODCharge_Charge_DiodePositionSeedQLT300;
  TProfile2D *ProfhGOODCharge_Charge_DiodePositionSeedQGT2000;
  //clm
  TProfile2D *ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow;
  TProfile2D *ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow;
  TProfile2D *ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow;
  TProfile2D *ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow;
  //clm
  TProfile2D *ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow_seed;
  TProfile2D *ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow_seed;
  TProfile2D *ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow_seed;
  TProfile2D *ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow_seed;
  //clm
  TProfile2D *ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow_1stcrown;
  TProfile2D *ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow_1stcrown;
  TProfile2D *ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow_1stcrown;
  TProfile2D *ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow_1stcrown;
  //clm
  TProfile2D *ProfhGOODCharge_Charge_DiodePosition_evencol_evenrow_2ndcrown;
  TProfile2D *ProfhGOODCharge_Charge_DiodePosition_evencol_oddrow_2ndcrown;
  TProfile2D *ProfhGOODCharge_Charge_DiodePosition_oddcol_evenrow_2ndcrown;
  TProfile2D *ProfhGOODCharge_Charge_DiodePosition_oddcol_oddrow_2ndcrown;
    
  TH2F*  hDistVSeedOtherOldCalc;
  TH2F*  hDistVSeedOtherNewCalc;  
  
  TH2F*     h2dCharge_Charge_DiodePosition_Track;
  TH2F*     h2dCharge_Charge_DiodePosition_CluSize;
  
  TH1F*     hNpixInClu;
  TH1F*     hQpixInClu;
  
  TProfile2D *ProfhGOODCharge_Charge_DiodePositionSimpDist;
  
  TH3F *hHOM_Charge_diodedist3D;
  TH3F *hHOM_Charge2_diodedist3D;
  TH3F *hHOM_Charge4_diodedist3D;
  TH3F *hHOM_Charge9_diodedist3D;
  TH3F *hHOM_Charge25_diodedist3D;  
    // end CLM 2013/01/23
  
    TProfile *ProfHOM_Charge_diodedist_alg;  // JB 2010/03/11
    TProfile *ProfHOM_Charge_diodedist_alg_v;  //clm 2013/01/23
    TProfile *ProfHOM_Charge_diodedist_alg_u;  //clm 2013/01/23
    TProfile *ProfHOM_Charge_diodedist;
    TProfile *ProfHOM_Charge2_diodedist;
    TProfile *ProfHOM_Charge4_diodedist;
    TProfile *ProfHOM_Charge9_diodedist;
    TProfile *ProfHOM_Charge25_diodedist;

    TH2F *hHOM_SNseed_diodedist;
    TProfile *ProfHOM_SNseed_diodedist;

    TH1F *hHOM_Charge_diodedist_00_10;
    TH1F *hHOM_Charge_diodedist_10_20;
    TH1F *hHOM_Charge_diodedist_20_30;
    TH1F *hHOM_Charge_diodedist_30_40;
    TH1F *hHOM_Charge_diodedist_40_50;
    TH1F *hHOM_Charge_diodedist_50_60;
    TH1F *hHOM_Charge_diodedist_60_70;
    TH1F *hHOM_Charge_diodedist_70_80;
    TH1F *hHOM_Charge_diodedist_80_90;
    TH1F *hHOM_Charge_diodedist_90_inf;

    TH2F *hHOM_DU_Nevent;
    TH2F *hHOM_DV_Nevent;
    TH2F *hHOM_modtu_Nevent;
    TH2F *hHOM_modtv_Nevent;
    
    TH2F *hHOM_modUCG_Nevent;
    TH2F *hHOM_modVCG_Nevent;
 
    TH1F *hHOM_ResEta25_U;
    TH1F *hHOM_ResEta25_V;


    TH2F *hHOM_modUeta2x2_modVeta2x2;
    TH2F *hHOM_modUCG2x2_modVCG2x2;
    TH2F *hHOM_modUeta5x5_modVeta5x5;
    TH2F *hHOM_modUCG5x5_modVCG5x5;


    //--------------------------------------------------------------------------
    // cluster shape study, JB 2010/04/13 based on cdritsa stuff
    //--------------------------------------------------------------------------

    TH2F *hClusterMeanForm;
    TH1D *hprojLForm;
    TH1D *hprojCForm;
    TH1F *hCountPixels[10];
    TH2F *hPixelsOverSNR[10];
    TH2F *hPixelsOverCharge[10];
    TH1D *hprojL[10];
    TH1D *hprojC[10];
    TH2F *Cluster[10];
    TH2F *hMultVsFormFactor[10];
    TH1F *hMultVsFormFactor1D[10]; //clm added back 1D clu mult. 2013.08.25
    TH2F *hChargeCoG_Correl;
    TH2F *hChargeCoG_Correl2;
    TH1F *h_SNRratioL;
    TH1F *hClusterTest1;
    TH1F *hClusterTest2;
    TH1F *hClusterSizeInLines; // JB 2014/03/31
    TH1F *hClusterSizeInColumns;
    TH1F *hChargeDistrInSeed;
    TH1F *hChargeDistrInLine;
    TH1F *hChargeDistrIn3rdLeftNeigh;
    TH1F *hChargeDistrIn2ndLeftNeigh;
    TH1F *hChargeDistrIn1stLeftNeigh;
    TH1F *hChargeDistrIn3rdRightNeigh;
    TH1F *hChargeDistrIn2ndRightNeigh;
    TH1F *hChargeDistrIn1stRightNeigh;
    TH1F *hClusterTypes; // JB 2014/03/31

    TCanvas *ang1;
    TCanvas *ang2;
    TCanvas *ang3;
    TCanvas *ang4;
    TCanvas *ang5;
    TCanvas *ang6; //clm added back 1D clu mult. 2013.08.25
    TCanvas *shape;
    TCanvas *solo;
    TCanvas *projL;
    TCanvas *projC;


    //------------------------------------------------------------------------------ 
    // Binary output histos.
    //------------------------------------------------------------------------------ 
    TH1F *hBinary_test;
    TH1F *hBinary_NumberOf_1_ALL;
    TH1F *hBinary_NumberOf_1_goodhit;
    TH1F *hBinary_NumberOf_1_submatrix;
    TH1F *hBinary_Nhitperpixel_submatrix;
    TH1F *hBinary_NhitRateperpixel_submatrix;


  //------------------------------------------------------------------------------ 
  // MiniVectors Histograms ; NCS , 2010/01/21
  //------------------------------------------------------------------------------ 
  TH1F* hDiffPosX; // NCS 210110
  TH1F* hDiffPosY;// NCS 210110
  TH1F* hDiffAngleX; // NCS 210110
  TH1F* hDiffAngleY;// NCS 210110
  TH1F* hDiffAngleX11; // JB 2011/11/01
  TH1F* hDiffAngleY11;
  TH1F* hDiffAngleX12; // JB 2011/11/01
  TH1F* hDiffAngleY12;
  TH1F* hDiffAngleX21; // JB 2011/11/01
  TH1F* hDiffAngleY21;
  TH1F* hDiffAngleX22; // JB 2011/11/01
  TH1F* hDiffAngleY22;
  TH1F* hDiffAngleXg1g1; // JB 2011/11/01
  TH1F* hDiffAngleYg1g1;
  TH1F* hxtxPL3 ; // NCS 260110 not CG position only Hu Hv
  TH1F* hytyPL3 ; // NCS 260110 
  TH1F* hxtxPL4 ; // NCS 260110 
  TH1F* hytyPL4 ; // NCS 260110 
  TH1F* hutuPL3 ; // NCS 260110 not CG position only Hu Hv
  TH1F* hvtvPL3 ; // NCS 260110 
  TH1F* hutuPL4 ; // NCS 260110 
  TH1F* hvtvPL4 ; // NCS 260110 
  TH2F* hdiffydiffx; // JB 2011/11/01
  TH2F* hdiffydiffx11; // JB 2011/11/01
  TH2F* hdiffydiffx12; // JB 2011/11/01
  TH2F* hdiffydiffx21; // JB 2011/11/01
  TH2F* hdiffydiffx22; // JB 2011/11/01
  TH2F* hdiffydiffxg1g1; // JB 2011/11/01

  //------------------------------------------------------------------------------ 
  // MimosaVertexFinder Histograms ; LC , 2012/09/06
  //------------------------------------------------------------------------------ 

  TH1F* hVertexPosX;
  TH1F* hVertexPosY;
  TH1F* hVertexPosZ;
  TH1F* hVertexTrackDistance;
  TH2F* hVertexPosXY;
  TH1F* hVertexTrackChi2;

  TCanvas *cVertexFinder;

  TCanvas *cMiniVec;
  TCanvas *cMiniVec1;

  //------------------------------------------------------------------------------ 
  // canvas 
  //------------------------------------------------------------------------------ 
  TCanvas *c2;
  TCanvas *casym;
  TCanvas *ceffi;
  TCanvas *ccomp;
  TCanvas *ccomp2;
  TCanvas *ccomp3;
  TCanvas *c3;
  TCanvas *c4;
  TCanvas *cClusterProperties2;
  TCanvas *cClusterProperties3;
  TCanvas *c4_2;
  TCanvas *c5;
  TCanvas *c6;
  TCanvas *c7;

  static const Int_t numcanvas=4;
  static const Int_t numcanvasSN=6;
  static const Int_t numcanvasOptimize=1;
  TCanvas *cSN[numcanvasSN]; //!
  TCanvas *cRef[numcanvas];   //!   
  TCanvas *cOptimize[numcanvasOptimize]; //!
 
  TCanvas *cM8;


  TCanvas *PixHom;
  TCanvas *PixHom2;
  TCanvas *PixHom3;
  TCanvas *PixHom4;
  TCanvas *PixHom5;
  TCanvas *PixHom6;

  //graphs
  TGraphErrors* greff ;
  TGraphErrors* grnum ;
  TGraphErrors* grevt ;
  TGraphErrors* ChargeSpread;

  TControlBar*  bar2; 


  //------------------------------------------------------------------------------ 
  // User histograms
  //------------------------------------------------------------------------------ 
  TCanvas *cUser;
  TH1F* hUserHitCorrelationLine;
  TH1F* hUserHitCorrelationCol;

  ClassDef(MHist, 2)
};

#endif
