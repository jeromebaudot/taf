// @(#)maf/dtools:$Name:  $:$Id: DAlign.h,v.1 2005/10/02 18:03:46 sha Exp $
// Author: Dirk Meier   97/01/20 

#ifndef _DAlign_included_
#define _DAlign_included_

////////////////////////////////////////////////////////////
//                                                        //
// Class  DAlign                                          //
// book keeping for alignment                             //
//                                                        //
////////////////////////////////////////////////////////////

// ROOT classes
#include "TObject.h"
#include "Riostream.h"
// DT classes
#include "DR3.h"
#include "TH2.h"
#include "TH3.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TFile.h"

class DCut;
class DTrack;

class DAlign : public TObject {
 private:
  Int_t        fDebugAlign;                // Align  debug flag
  Int_t        fPlaneNumber;               // plane nb being aligned
  TCanvas*     fAlignCanvas;
  TCanvas*     fpfxAlignCanvas;            // Added 060314 BB
  TCanvas*     fAlign3DCanvas;             // Added 070414 BB
  TH1F*        fhAlignDU;
  TH1F*        fhAlignDV;
  TH2F*        fhAlignDVDU;
  TH2F*        fhAlignPosUV;
  TH2F*        fhAlignPosVU;
  TH2F*        fhAlignPosUU;
  TH2F*        fhAlignPosVV;
  TH2F*        fhAlignUU;                  // JB 2014/08/26
  TH2F*        fhAlignVV;                  // JB 2014/08/26
  TH2F*        fhAlignPosUUV;              // BB 070414
  TH2F*        fhAlignPosVUV;              // BB 070414
  TProfile*    fpfxAlignPosUV;             // Profile of fhAlignPosUV, BB 050314
  TProfile*    fpfxCorrectedAlignPosUV;    // Profile of fhAlignPosUV corrected by residual/tan(theta), BB 080414
  TProfile*    fpfxAlignPosVU;             // Profile of fhAlignPosVU, BB 050314
  TProfile*    fpfxAlignPosUU;             // Profile of fhAlignPosUU, BB 050314
  TProfile*    fpfxAlignPosVV;             // Profile of fhAlignPosVV, BB 050314
  Bool_t       fEnoughU;
  Bool_t       fEnoughV;
  Bool_t       fEnough2D;
  Int_t        fNumberOfEvents;            // min # events to accumulate
  Double_t     fBound;                     // max distance track-hit
  Double_t     fBoundU;                    // max distance track-hit in U, JB 2013/06/10
  Double_t     fBoundV;                    // max distance track-hit in V, JB 2013/06/10
  Double_t     fLimitsU[2];                // min-max in U for tracks, JB 2013/06/10
  Double_t     fLimitsV[2];                // min-max in V for tracks, JB 2013/06/10
  Double_t     fAlignResolutionU;
  Double_t     fAlignMeanU;
  Double_t     fAlignResolutionV;
  Double_t     fAlignMeanV;
  Double_t     fAlignSum_du;    
  Double_t     fAlignSum_dudu;
  Double_t     fAlignSum_duvt;  
  Double_t     fAlignSum_vt;   
  Double_t     fAlignSum_vtvt; 
  Int_t        fAlignCountu;    
  Double_t     fAlignSum_dv;    
  Double_t     fAlignSum_dvdv;
  Double_t     fAlignSum_dvut;  
  Double_t     fAlignSum_ut;   
  Double_t     fAlignSum_utut; 
  Int_t        fAlignCountv;    
  Double_t     fAlignSum_uvt2;
  Double_t     fAlignSum_dvuduv;
  Int_t        fAlignCount2D;    
  Double_t     fAlignTiltW; // name reaplced from tiltZ to tiltW, JB 2010/11/25
  Double_t     fAlignOffsetU;    
  Double_t     fAlignOffsetV;    
  Double_t     fAlignTiltWError; 
  Double_t     fAlignOffsetUError; 
  Double_t     fAlignOffsetVError; 
  Double_t     fAlignCorrelation;
  DR3          fAlignOffset;

 public:
  DAlign();                                 
  DAlign(Int_t aPlaneNumber, Int_t aDebugLevel=0);                                 
  ~DAlign();
  void         SetDebug(Int_t aDebug){fDebugAlign = aDebug;}
  Int_t        GetDebug()    { return fDebugAlign;}

  void         AccumulateU(DR3 &aTrackPos, Double_t aDistance_U);	
  void         AccumulateV(DR3 &aTrackPos, Double_t aDistance_V);	
  void         Accumulate2D(DR3& aTrackPos, Double_t aDistanceU, Double_t aDistanceV);
  Double_t     GetOffsetU()                 { return fAlignOffsetU; }
  Double_t     GetOffsetV()                 { return fAlignOffsetV; }
  Double_t     GetTiltW()                   { return fAlignTiltW; }
  Double_t     GetResolutionU()             { return fAlignResolutionU; }
  Double_t     GetMeanU()                   { return fAlignMeanU; }
  Double_t     GetResolutionV()             { return fAlignResolutionV; }
  Double_t     GetMeanV()                   { return fAlignMeanV; }
  DR3&         GetOffset()                  { return fAlignOffset; }
  void         Modified();
  Int_t        GetCounts()                  { return TMath::Max(fAlignCountu, fAlignCount2D); }
  Double_t     GetBounding()                { return fBound; }
  Double_t     GetBoundingU()               { return fBoundU; } // JB 2013/06/10
  Double_t     GetBoundingV()               { return fBoundV; } // JB 2013/06/10
  void         ShowCorrection();
  void         ShowStoredResolution();
  void         CreateDisplay( Double_t Umin, Double_t Umax, Double_t Vmin, Double_t Vmax);
  TCanvas*     GetDisplay() { return fAlignCanvas; }
  void         SaveDisplay( Char_t *fileName); // JB 2014/05/06
  void         StoreU(DR3& aTrackPos, Double_t aDistanceU);
  void         StoreV(DR3& aTrackPos, Double_t aDistanceV);
  void         Store2D(DR3& aTrackPos, Double_t aDistanceU, Double_t aDistanceV);
  void         Display();
  void         Display(Int_t fRunNumber); // LC 2012/09/20.
  void         SetBounding(Double_t aBound);
  void         SetBoundings(Double_t aBoundU, Double_t aBoundV); // JB 2013/06/10
  void         SetGeoLimits(Double_t umin, Double_t umax, Double_t vmin, Double_t vmax); // JB 2013/06/10
  void         SetEvents( Int_t nEvents)    { fNumberOfEvents = nEvents; } // JB, 2009/05/25
  Bool_t       EnoughU()                    { return fEnoughU; }
  Bool_t       EnoughV()                    { return fEnoughV; }
  Bool_t       Enough2D()                    { return fEnough2D; }

  TH2F*        GetHistUV()                   { return fhAlignPosUV; }
  TH2F*        GetHistVU()                   { return fhAlignPosVU; }


  ClassDef(DAlign,2)           // book keeping for alignment 
    };

#endif
