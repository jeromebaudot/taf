// @(#)MAlign.h  $:$Id: MAlign.h v.1 2005/10/02 18:03:46 sha Exp $
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
#include "DSetup.h"

#include "TStyle.h"
#include "TVectorD.h"
#include "MAlignment.h"
#include <sstream>
#include "DMiniVector.h"

using namespace std;

void FCNAlignMimosa(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag);
void FCNAlignMimosaLadder(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag);
void FCNAlignMimosaLadder2(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag);
//void FCNAlignMimosaMV(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag);

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
  TCanvas* fCanvasChiSquare;

  TH1F* fDu ;
  TH1F* fDv ;
  TH1F* fDw;
  TH1F* fDdu ;
  TH1F* fDdv ;
  TH1F* fDdw;

  TH1F* fDx; // qyliu: ladder
  TH1F* fDy;
  TH1F* fDz;

  TH1F* fChiSquareX;
  TH1F* fChiSquareY;
  TH1F* fChiSquareZ;
  TH1F* fChiSquareTiltX;
  TH1F* fChiSquareTiltY;

  TGraph fGraph[9]; //qyliu: changed from [6] for ladder
  TF1* fMyfit;
  MAlignment* fAlignment;
  DPrecAlign* fAlignement0;
  //DPrecAlign** myPrecAlignments;
  std::vector<DLadder*> fLadders;
  DLadder* ladderToAlign;
  DLadder* ladder0;
  Int_t aChi2Limit;
  ofstream file;
  ofstream trackIndexFile;

  // Minimization tool :
  Bool_t  printResiduals;
  TString algoName;
  TString minimizerName;
  Int_t printLevel;

  TGraph** alignParametersIterations;
  TCanvas** alignParamsCanvas;

  static MimosaAlignAnalysis* fgInstance;


 public:

  MimosaAlignAnalysis( DSession *aSession);
  virtual  ~MimosaAlignAnalysis();
  void        SetDebug(Int_t aDebug) { AlignDebug = aDebug;     cout << "MAlign debug updated to " << AlignDebug << endl;
} // JB 2011/06/18

  void        fcn(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag);
  void        fcnLadder(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag);
  void        fcnLadder2(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag);

  //void        fcnMiniVectors(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag);  // LC 2014/12/23
  void        SetTrackGeoLimits(Double_t xmin, Double_t xmax, Double_t ymin, Double_t ymax); // JB 2013/06/10
  Bool_t      CheckTrackInGeoLimits( DTrack *aTrack); // JB 2013/06/11

  void        SetTrackChi2Limit( Double_t aChi2Limit); // JB 2013/07/14

  //DPrecAlign* AlignMimosa(TVectorD aGetParamVect, TFile* aCorfile, int save_result, const char* MimosaResultDir);
  DPrecAlign* AlignMimosa(DPrecAlign* initAlignment, TFile* aCorFile, const char* MimosaResultDir, Double_t aDistance); // new parameter, JB 2012/05/11
  DPrecAlign* AlignPrecMimosa(DPrecAlign* initAlignment, Bool_t modeAuto, Double_t aDistance=150.); // LC 2012/09/06.
  //DPrecAlign* AlignMimosaMV( Bool_t modeAuto ); // LC 2013/09/05.
  //DPrecAlign* AlignMimosaMV(DLadder* myLadderToAlign, DPrecAlign* initAlignment, DPrecAlign* fAlignmentLadder0, Bool_t modeAuto, Int_t chi2Limit);

  void        FitInterface( Bool_t modeAuto, const Double_t& aDistance, const Double_t& aSlopeMax);
  void        MakeFit(TString minimizerName, TString algoName, Int_t maxCall, Int_t maxIter, Double_t tolerance, Int_t printLevel, Int_t strategy);
  void        DrawResiduals();
  Double_t    ComputeTotalChiSquareMV(const Double_t* par);
  Int_t       PrintMinimizationChoice();
  std::string PrintMenu();
  std::string ShowAndSetParameters();
  Int_t       SetPrintLevel();

  DPrecAlign* AlignPrecMimosaLadder(DLadder* myLadder, Bool_t modeAuto, Double_t aDistance=150.);
  DPrecAlign* AlignPrecMimosaLadder2(DLadder* myLadder, Bool_t modeAuto, Double_t aDistance=150.);

  //std::vector<Double_t>& AlignMimosaAssociationMV(DLadder* myLadderToAlign, DPrecAlign* initAlignment, DPrecAlign* fAlignmentLadder0, Bool_t modeAuto, Int_t chi2Limit); // LC 2014/02/11

  //void        cutData(Int_t nSigmas, Double_t* par); // LC 2014/02/10
  void        AlignTracker(const Double_t tiniBoundU,
			   const Double_t tiniBoundV,
			   Int_t nAlignEvents=4000,
			   Int_t nAdditionalEvents=2000,
			   bool  UseAllHits = false); // U,V bounds added, JB 2013/06/10, # events added by JB, 2007 June
                                                      // AP 2015/06/10: added bool parameter (UseAllHits) to decide if doing alignment with all hits or the closest one.
  void        AlignTracker(const Double_t tiniBound=1000., Int_t nAlignEvents=4000, Int_t nAdditionalEvents=2000, bool UseAllHits = false) { AlignTracker( tiniBound, tiniBound, nAlignEvents, nAdditionalEvents, UseAllHits); }
  void        AlignTrackerMinuit(Bool_t modeAuto=1,
				 const Double_t tiniBound=480.,
				 Int_t nAlignEvents=5000,
				 Int_t nAlignHitsInPlane=800,
				 Int_t nAdditionalEvents=2000,
				 Double_t chi2Limit=0,
				 bool UseAllHits = true); // LC 2012/09/06  // LC 2012/10/08 chiLimit to select track with good chi2.
                                                          // AP 2015/06/08 : Added bool parameter (UseAllHits) to decide if doing alignment with all hits or with closest one.
  void        AlignTrackerMinuitLadder(Bool_t modeAuto =1, const Double_t tiniBound=480., Int_t nAlignEvents=5000, Int_t nAlignHitsInPlane=4000, Int_t nAdditionalEvents=2000, Double_t chi2Limit=0); // LC 2013/01/16.
  //void        AlignLadder(Bool_t modeAuto=1, const Double_t tiniBound=480., Int_t nAlignEvents=5000, Int_t nAlignHitsInPlane=4000, Int_t nAdditionalEvents=2000, Int_t ladderFace=1, Double_t chi2Limit=0.); // OUTDATED, LC: 2014/12/18
  void        AlignLadderMV(Bool_t modeAuto=1, const Double_t tiniBound=480., const Double_t boundSlopes=100., Int_t nAlignEvents=5000, Int_t nGoodTracks=1000, Int_t nAdditionalEvents=2000, Int_t chi2Limit=40, Int_t mode=0); // LC 2013/09/30.
  void        AlignTrackerGlobal(const Int_t refPlane=1, const Int_t nEvents=4000, const Int_t nIterations=10, const Bool_t alignConstrainsts=true, const Bool_t trackConstrainsts=true, const Bool_t multipleScatteringFit=true);  // LC & LiuQ 2015/02/06
  void        AlignTrackerMillepede(Int_t nAlignEvents); // LC 24/12/2012.
  void        UpdateConfAlignUVW(fstream &fileIn, fstream &fileOut, DPlane* SecPlane);
  void        UpdateConfAlign2D(Int_t nSecPlanes, DPlane** SecPlane);
  void        Gener(Double_t* xp, Double_t* yp, Double_t& aX, Double_t& bX, Double_t& aY, Double_t& bY, Double_t* sigX, Double_t* disX, Double_t* sigY, Double_t* disY, Double_t* z, Double_t* phi);
  void        AssociateMiniVectors( Double_t boundDistance, Double_t boundSlopes, Int_t mode); // LC 2014/12/20.
  void        SplitString(const string& s, char c, vector<string>& v);

  void        FillMiniVectorsResiduals( std::vector<MiniVector*> MV0, std::vector<MiniVector*> MV1 );   // LC 2015/06/04
  void        FillMiniVectorsResidualsMC( std::vector<MiniVector*> MV0, std::vector<MiniVector*> MV1 ); // LC 2015/06/04
  void        StudyDeformation(const Float_t tiniBound=1000, Int_t nEvents=2000, Bool_t fitMode=0, Float_t minU=0., Float_t maxU=0., Float_t minV=0., Float_t maxV=0.);// Method to study shape of a plane with possibility of fitting the shape with a Legendre polynomials : BB 2014/05/20, updated JB 2015/10/31


  // Convert string to type T :: usage StringToNumber<Type> ( String );
  template <typename T>
  T StringToNumber ( const string &Text )
  {
    istringstream ss(Text);
    T result;
    return ss >> result ? result : 0;
  }

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
