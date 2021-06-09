#ifndef _DLadder_included_
#define _DLadder_included_

#include <math.h>
#include <vector>

// ROOT classes
#include "TMath.h"
#include "DR3.h"
#include "DSession.h"
#include "TString.h"
#include "DPrecAlign.h"
#include "DPixel.h"
#include "DMiniVector.h"
#include "TH1F.h"
#include "TH2F.h"

// Last Modified: LC 2014/12/18, Big code cleaning. Now custom ladders by default.
// Last Modified: LC 2014/12/18, Now use only DPrecAlign to recompute ladder+planes parameters.
// Last Modified: Lc 2014/12/19, Saving Mini-vectors in Dladder. With new class MiniVector.

class DSetup;
class DTrack;
class DTracker;
class DAcq;
class DAcqModule;
class DInp;
class DHit;
class DPrecAlign;
class DAlign;
class DCut;
class DR3;
class DSession;

class DLadder : public TObject {

 private:

  DSession*                 fSession;
  DAcq*                     fAcq;                      // pointer to Data Acquisition
  DSetup*                   fc;                        // pointer to Configuration
  DTracker*                 fTracker;                  // pointer to the Tracker

  TString                   ladderType;            
  Int_t                     ladderID;           // ID of the ladder 
  Int_t                     Status;             // Ladder status
  Int_t                     numberOfSensors;    // Number of sensors in the ladder
  Double_t                  separatorLengthU;

  std::map<Int_t, DPlane*>  planeList;          // map of the DPlane in the Ladder.
  
  //std::map<Int_t, DR3>      planePosition;    
  //std::map<Int_t, DR3>      planeRotation;

  std::map<Int_t, DR3>      planeRelativePosition;      // relative shift wrt ladder
  std::map<Int_t, DR3>      planeRelativeRotation;      // relative rotation wrt ladder
  std::map<Int_t, DPrecAlign*> planeRelativePrecAlign;  // relative alignment wrt ladder

  std::vector<MiniVector*>   _miniVectors;              // Vector of mini-vectors. (See class MiniVector). One event.
  std::vector<MiniVector*>   _associatedMiniVectors;    // List of associated mini-vectors. Sum on all events.

  DR3                       fTilt;                      // Ladder tilts
  DR3                       fPosition;                  // Ladder position (center position)
  
  DPrecAlign*               fPrecAlign;                 // DPrecAlign = position + tilts of the ladder.

  Int_t                     fDebugLadder;

  Bool_t                    fIfAssociated;

  TCanvas*                  fCanvas;
  TH1F*                     fhDU;
  TH1F*                     fhDV;
  TH1F*                     fhDW;
  
  TH2F*                     fhDU_U;
  TH2F*                     fhDV_V;
  TH2F*                     fhDU_V;
  TH2F*                     fhDV_U;
  TH2F*                     fhDU_W;
  TH2F*                     fhDV_W;
  TH2F*                     fhDW_U;
  TH2F*                     fhDW_V;
  TH2F*                     fhDW_W;


  TH1F*                     fhSlopeX;
  TH1F*                     fhSlopeY;
  TH1F*                     fhDiffSlopeX;
  
  TProfile*                 myProfileX;
  TProfile*                 myProfileY;

  TCanvas*                  fCanvasMC;
  TH1F*                     fResMC_U;
  TH1F*                     fResMC_V;
  TH1F*                     fResMC_W;

 public:
  
  DLadder();
  DLadder(DTracker& aTracker, TString ladderName, Int_t aLadderID, Int_t aStatus, Double_t sensorLengthX, Double_t sensorSeparator, Double_t distanceFromFoam, Double_t rotationX, Double_t rotationY, Double_t rotationZ);
  
  ~DLadder();
  
  Int_t         GetLadderID()                    { return ladderID; }
  
//  DR3           GetPlanePosition(Int_t sensorID) { return planePosition[sensorID]; }
//  DR3           GetPlaneRotation(Int_t sensorID) { return planeRotation[sensorID]; }
  DPlane*       GetPlane(Int_t sensorID)         { return planeList[sensorID]; }
 
  Int_t         GetNumberOfPlanes()               { return planeList.size(); }
  Int_t         GetFirstPlane()                   { std::map<Int_t, DPlane*>::iterator iterPlane = planeList.begin(); return iterPlane->first; } 
  DR3           GetLadderPosition()               { return fPosition; }
  DR3           GetLadderRotation()               { return fTilt; }
  
  Double_t      GetSeparatorTotalLentgh()         { return separatorLengthU*(planeList.size()/2-1); }

  Int_t         GetStatus()                       { return Status; }

  void          UpdateAlignment();
  
  void          GetRelativePosition( Int_t aPlaneNumber, DR3* aTilt, DR3* aShift); // JB 2014/02/10
  
  void          AlignLadder(std::vector<Double_t>& DataTemp, const Int_t ladderNumber, const Int_t planeNumber);
  
  DR3           PlaneToLadder(DR3& aPlaneCoordinate, Int_t aPlaneNumber); // JB 2014/02/17
  void          MakeMiniVectors();

  DR3           ComputeDirection(DR3& positionLeft, DR3& positionRight);

  void          AddPlane(DPlane* aPlane, Int_t planeID);
  void          SetLadderType(TString aType)      { ladderType = aType;};

  TString       GetLadderType()                   { return ladderType; }

  DTracker*     GetTracker()                      { return fTracker;  }
  DPrecAlign*   GetPrecAlign()                    { return fPrecAlign; }
  DPrecAlign*   GetRelativePlaneAlign(Int_t planeIndex) { return planeRelativePrecAlign[planeIndex]; } // planeIndex start from 1 (first plane in the ladder) to N.

  void          AddMiniVector(MiniVector* myNewMiniVector);
  void          RemoveLastMiniVector();
  Int_t         GetMiniVectorsSize()              { return _miniVectors.size(); }
  void          ResetMiniVectors();
  void          ResetMiniVectorsList(std::vector<Int_t> listOfDelete );
  std::vector<MiniVector*>&  GetMiniVectors()     { return _miniVectors; }

  Bool_t        IfAssociated()                    { return fIfAssociated; }
  void          SetAssociated()                   { fIfAssociated = 1; }
  Int_t         GetAssociatedMiniVectorsSize()    { return _associatedMiniVectors.size(); }
  void          AddAssociatedMiniVector(MiniVector* myAssociatedMV);
  void          ResetAssociatedMiniVectors();
  std::vector<MiniVector*>&  GetAssociatedMiniVectors()  { return _associatedMiniVectors; }

  void          Display(const Int_t planeNumber);
  void          PrepareDisplay();
  
  //void          FillResidus(std::vector<Double_t>& Data, const Int_t planeNumber, DR3& dxdydz);  // LC 2014/12/18 useless
  void          FillResidus(std::vector<Double_t>& Data, const Int_t planeNumber);
  void          FillResidusMC(std::vector<Double_t>& Res, const Int_t planeNumber);

  ClassDef(DLadder,3)                      // Detector Ladder

};

#endif
