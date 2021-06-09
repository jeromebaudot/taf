//  Author   :  L. Cousin 2014/12/19
//  Save, recompute and project mini-vectors

#ifndef _MiniVector_included_
#define _MiniVector_included_

// ROOT classes
#include "TObject.h"
#include "TClass.h"
#include "TList.h"
#include "DR3.h"

#include "DPrecAlign.h"

class MiniVector : public TObject {
 
 public:
  
  MiniVector() {;};
  MiniVector(DPrecAlign* ladderDPrecAlign, DR3& hitLeftPosition, DR3& hitRightPosition, DPrecAlign* alignPlaneLeft, DPrecAlign* alignPlaneRight, DR3& centerPosition, DR3& direction, DR3 planeFrameLeft, DR3 planeFrameRight);         // In tracker frame.

  virtual ~MiniVector(){};
  
  void SetMonteCarloMiniVector(DR3& hitLeftPositionMC, DR3& hitRightPositionMC, DR3& centerPositionMC, DR3& directionMC, DR3 planeFrameLeftMC, DR3 planeFrameRightMC );

  void SetLadderDPrecAlign(DPrecAlign* ladderDPrecAlign) { _ladderDPrecAlign = ladderDPrecAlign; };
  void SetHitLeft(DR3& hitLeftPosition);
  void SetHitRight(DR3& hitRightPosition);
  void SetCenter(DR3& centerPosition);
  void SetDirection(DR3& direction);
  //void SetDPrecAlign(DPrecAlign* myNewDPrecAlign)        { _ladderDPrecAlign = myNewDPrecAlign; };
 
  void SetHitLeftMC(DR3& hitLeftPositionMC);
  void SetHitRightMC(DR3& hitRightPositionMC);
  void SetCenterMC(DR3& centerPositionMC);
  void SetDirectionMC(DR3& directionMC);

  void PrintMiniVector();

  DR3 CalculateMiniVectorIntersectionTracker(DPrecAlign* myDPrecAlign);  // Compute intersection with a DPrecALign object.
  DR3 CalculateMiniVectorIntersectionLadder(DPrecAlign* myDPrecAlign);   // Compute intersection with a DprecAlign in the frame of these DPrecAlign.

  DR3 CalculateMiniVectorIntersectionTrackerMC(DPrecAlign* myDPrecAlign);
  DR3 CalculateMiniVectorIntersectionLadderMC(DPrecAlign* myDPrecAlign);

  DR3  GetHitLeftPlanePosition();
  DR3  GetHitRightPlanePosition();
  DR3  GetMiniVectorLadderCenter();

  DR3  GetHitLeftPlanePositionMC();
  DR3  GetHitRightPlanePositionMC();
  DR3  GetMiniVectorLadderCenterMC();

  DR3  GetRealLeftPlaneFrame()   { return _planeFrameLeft; }; 
  DR3  GetRealRightPlaneFrame()  { return _planeFrameRight; };

  DR3  GetRealLeftPlaneFrameMC()   { return _planeFrameLeftMC; }; 
  DR3  GetRealRightPlaneFrameMC()  { return _planeFrameRightMC; };

  DR3  GetHitLeftTrackerPosition();                                      // Re-Compute hitLeft in function of _ladderDprecAlign;
  DR3  GetHitRightTrackerPosition();                                     // Re-Compute hitRight in function of _ladderDprecAlign;
  DR3  GetMiniVectorTrackerCenter();                                     // Re-Compute MV center in function of _ladderDprecAlign;
  DR3  GetMiniVectorDirection();                                         // Re-Compute MV direction in function of _ladderDprecAlign;
  DR3  GetMiniVectorOrigin();

  DR3  GetHitLeftTrackerPositionMC();                                      // Re-Compute hitLeft in function of _ladderDprecAlign;
  DR3  GetHitRightTrackerPositionMC();                                     // Re-Compute hitRight in function of _ladderDprecAlign;
  DR3  GetMiniVectorTrackerCenterMC();                                     // Re-Compute MV center in function of _ladderDprecAlign;
  DR3  GetMiniVectorDirectionMC();                                         // Re-Compute MV direction in function of _ladderDprecAlign;
  DR3  GetMiniVectorOriginMC();

  DPrecAlign* GetDPrecAlignLeft()  { return _planesDPrecAlign.first; };
  DPrecAlign* GetDPrecAlignRight() { return _planesDPrecAlign.second; };

  Bool_t IfMonteCarlo() { return monteCarloMiniVector; };

 private:

  Bool_t monteCarloMiniVector;
  
  std::pair<DPrecAlign*, DPrecAlign*> _planesDPrecAlign;
  
  std::vector<Double_t> _hitLeftPosition;              // In the corresponding plane
  std::vector<Double_t> _hitRightPosition;             // In the corresponding plane

  std::vector<Double_t> _miniVectorCenterPosition;     // In ladder frame
  std::vector<Double_t> _miniVectorDirection;          // In tracker frame
  std::vector<Double_t> _miniVectorOrigin;

  DPrecAlign*           _ladderDPrecAlign;             // ladder DPrecAlign

  DR3                   _planeFrameLeft;
  DR3                   _planeFrameRight;

  DR3                   _planeFrameLeftMC;
  DR3                   _planeFrameRightMC;

  std::vector<Double_t> _hitLeftPositionMC;
  std::vector<Double_t> _hitRightPositionMC;

  std::vector<Double_t> _miniVectorCenterPositionMC;     // In ladder frame
  std::vector<Double_t> _miniVectorDirectionMC;          // In tracker frame
  std::vector<Double_t> _miniVectorOriginMC;

 ClassDef(MiniVector,1) 

};

#endif
