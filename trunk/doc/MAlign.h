// @(#)maf/maf:$Name:  $:$Id: MAlign.h v.1 2005/10/02 18:03:46 sha Exp $
// Author: A. Shabetai 

#ifndef _MimosaAlignAnalysis_included_
#define _MimosaAlignAnalysis_included_


  /////////////////////////////////////////////////////////////
  //                                                         //
  //  Mimosa chip and ref. system alignement class           //
  //                                                         //
  //                                                         //   
  ////////////////////////////////////////////////////////////

#include "Riostream.h"

//align Mimosa 
#include "TMinuit.h"
#include "TPostScript.h"
#include "DPrecAlign.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TF2.h"
#include "TGraph.h"
#include "TSystem.h"
#include "TLegend.h"

//align tracker
#include "DTrack.h"
#include "DTracker.h"
#include "TTree.h"
#include "DPlane.h"
#include "DSession.h"
#include "TPaveLabel.h"
#include "TText.h"
#include "DAlign.h"

#include "TStyle.h"
#include "TVectorD.h"
#include "MAlignment.h"

void FCNAlignMimosa(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag);
void FCNAlignMimosaLadder(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag);
void FCNAlignMimosaMV(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag);
void FCNAlignMimosaMV2(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag);

class TFile;

class MimosaAlignAnalysis : public TObject {
 private: 

  Int_t AlignDebug;
  DSession*   fSession; // JB 2011/07/21 to get rid of global var
  DPrecAlign* fAlignement;  
  DPrecAlign* fGeom;
  Double_t    fLimitsX[2]; // min-max in X for tracks, JB 2013/06/10
  Double_t    fLimitsY[2]; // min-max in Y for tracks, JB 2013/06/10
  Double_t    fChi2Limit; // JB 2013/07/14
  TCanvas* fCnv ;
  TCanvas* fCcr ;    
  TH1F* fDu ;
  TH1F* fDv ;
  TH1F* fDw;
  TH1F* fDdu ;
  TH1F* fDdv ;
  TH1F* fDdw;
  TGraph fGraph[6];
  TF1* fMyfit;
  MAlignment* fAlignment;
  DPrecAlign* fAlignement0;
  //DPrecAlign** myPrecAlignments;
  DLadder* ladderToAlign; 
  DLadder* ladder0;
  Int_t aChi2Limit;

  static MimosaAlignAnalysis* fgInstance;


 public:
  
  MimosaAlignAnalysis( DSession *aSession);
  virtual  ~MimosaAlignAnalysis() {delete fAlignment;} ;
  void        SetDebug(Int_t aDebug) { AlignDebug = aDebug;     cout << "MAlign debug updated to " << AlignDebug << endl;
} // JB 2011/06/18
  
  void        fcn(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag);
  void        fcnLadder(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag);
  void        fcnMiniVectors(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag);
  void        fcnMiniVectors2(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag);
  void        SetTrackGeoLimits(Double_t xmin, Double_t xmax, Double_t ymin, Double_t ymax); // JB 2013/06/10
  Bool_t      CheckTrackInGeoLimits( DTrack *aTrack); // JB 2013/06/11
  
  void        SetTrackChi2Limit( Double_t aChi2Limit); // JB 2013/07/14
  
  //DPrecAlign* AlignMimosa(TVectorD aGetParamVect, TFile* aCorfile, int save_result, const char* MimosaResultDir);
  DPrecAlign* AlignMimosa(DPrecAlign* initAlignment, TFile* aCorFile, const char* MimosaResultDir, Double_t aDistance); // new parameter, JB 2012/05/11
  DPrecAlign* AlignPrecMimosa(DPrecAlign* initAlignment, Bool_t modeAuto, Double_t aDistance=150.); // LC 2012/09/06.
  DPrecAlign* AlignMimosaMV(DLadder* myLadderToAlign, DPrecAlign* initAlignment, DPrecAlign* fAlignmentLadder, Bool_t modeAuto, Int_t chi2Limit); // LC 2013/09/05.
  DPrecAlign* AlignMimosaMV2(DLadder* myLadder0, DLadder* myLadderToAlign, DPrecAlign* initAlignment, DPrecAlign* fAlignmentLadder, Bool_t modeAuto, Int_t chi2Limit); // LC 2013/11/15.
  DPrecAlign* AlignPrecMimosaLadder(DLadder* myLadder, Bool_t modeAuto, Double_t aDistance=150.);
  void        AlignTracker(const Float_t tiniBoundU, const Float_t tiniBoundV, Int_t nAlignEvents=4000, Int_t nAdditionalEvents=2000); // U,V bounds added, JB 2013/06/10, # events added by JB, 2007 June    
  void        AlignTracker(const Float_t tiniBound=1000., Int_t nAlignEvents=4000, Int_t nAdditionalEvents=2000) { AlignTracker( tiniBound, tiniBound, nAlignEvents, nAdditionalEvents); }
  void        AlignTrackerMinuit(Bool_t modeAuto=1, const Float_t tiniBound=480., Int_t nAlignEvents=5000, Int_t nAlignHitsInPlane=800, Int_t nAdditionalEvents=2000, Double_t chi2Limit=0); // LC 2012/09/06  // LC 2012/10/08 chiLimit to select track with good chi2.
  void        AlignTrackerMinuitLadder(Bool_t modeAuto =1, const Float_t tiniBound=480., Int_t nAlignEvents=5000, Int_t nAlignHitsInPlane=4000, Int_t nAdditionalEvents=2000, Double_t chi2Limit=0); // LC 2013/01/16.
  void        AlignLadder(Bool_t modeAuto=1, const Float_t tiniBound=480., Int_t nAlignEvents=5000, Int_t nAlignHitsInPlane=4000, Int_t nAdditionalEvents=2000, Int_t ladderFace=1, Double_t chi2Limit=0.);
  void        AlignLadderMV(Bool_t modeAuto=1, const Float_t tiniBound=480., Int_t nAlignEvents=5000, Int_t nAdditionalEvents=2000, Int_t chi2Limit=40); // LC 2013/09/30.
  void        AlignLadderMV2(Bool_t modeAuto=1, const Float_t tiniBound=480, Int_t nAlignEvents=5000, Int_t nAdditionalEvents=2000, Int_t chi2Limit=40); // LC 2013/11/15.  
  void        AlignTrackerMillepede(Int_t nAlignEvents); // LC 24/12/2012.
  void        UpdateConfAlignUVW(fstream &fileIn, fstream &fileOut, DPlane* SecPlane);
  void        UpdateConfAlign2D(Int_t nSecPlanes, DPlane** SecPlane);
  void        Gener(Double_t* xp, Double_t* yp, Double_t& aX, Double_t& bX, Double_t& aY, Double_t& bY, Double_t* sigX, Double_t* disX, Double_t* sigY, Double_t* disY, Double_t* z, Double_t* phi);
  
  static  MimosaAlignAnalysis*& Instance( DSession *aSession) { 
    // Modified: JB 2011/07/21 to pass session pointer
    if (!fgInstance) 
    {
      cout << "MimosaAlignAnalysis::Instance: No current Mimosa Alignement session detected! Creating a new one..." << endl; 
      fgInstance = new MimosaAlignAnalysis( aSession);  ;
    } 
    return fgInstance; 
  }
  
  static  MimosaAlignAnalysis*& Instance() { 
    // Modified: JB 2011/07/21 to pass session pointer
    return fgInstance; 
  }

  ClassDef(MimosaAlignAnalysis,1)
    
};
    
#endif
    
