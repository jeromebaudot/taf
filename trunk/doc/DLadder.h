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
#include "DMonteCarlo.h"
#include "TH1F.h"
#include "TH2F.h"

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

  //vector<DPixel*>           *fListOfPixels;         // pointer to list of hit pixel
  //Int_t                     fPixelsN;                  // number of hit pixels
  //DHit                      **fHit;                      //! list of pointers to hits
  //Int_t                     fHitsN;                    // number of hits found in plane
  
  TString                   ladderType;            

  std::map<Int_t, DPlane*>  planeList;         // map of the DPlane in the Ladder.
  std::map<Int_t, DPlane*>  trackerPlaneList;  // map of the DPlane in the tracker for this Ladder, JB 2014/02/10
  Int_t                     ladderID;           // ID of the ladder; 
  Int_t                     Status;
  
  Int_t                     numberOfSensors;    // Number of sensors in the ladder;
  
  std::map<Int_t, DR3>      planePosition;
  std::map<Int_t, DR3>      planeRotation;   // /!\ For Salat : rotation of planes are different /!\

  std::map<Int_t, DR3>      planeRelativePosition; // relative shift wrt ladder
  std::map<Int_t, DR3>      planeRelativeRotation; // relative rotation wrt ladder
  std::map<Int_t, DPrecAlign> planeRelativePrecAlign; // relative alignment wrt ladder

  DR3                       ladderInitialRotations;

  std::vector<DTrack*>       trackVector;
  std::vector<DR3>           centerVector;
  std::vector<DR3>           dxdydzVector;
  std::vector<DR3>           centerLadderVector;
  
  //std::map<Int_t, std::vector<DTrack> > miniVectors;

  DR3                       ladderPosition;
  DR3                       ladderRotation;

  Float_t                   fSensorLengthX;
  Float_t                   fSensorSeparator;
  Float_t                   fDistanceFromFoam;

  DR3*                      fTilt;
  DR3*                      fPosition;

  
  DPrecAlign*               fPrecAlign;

  Int_t                     fDebugLadder;

  TCanvas*                  fCanvas;
  TH1F*                     fhDU;
  TH1F*                     fhDV;
  TH1F*                     fhDW;
  
  TH2F*                     fhDU_U;
  TH2F*                     fhDV_V;
  TH2F*                     fhDU_V;
  TH2F*                     fhDV_U;

  TH1F*                     fhSlopeX;
  TH1F*                     fhSlopeY;
  TH1F*                     fhDiffSlopeX;
  
  TProfile*                 myProfileX;
  TProfile*                 myProfileY;


 public:
  
  DLadder();
  DLadder(DTracker& aTracker, TString ladderName, Int_t aLadderID, Int_t aStatus, Float_t sensorLengthX, Float_t sensorSeparator, Float_t distanceFromFoam, Float_t rotationX, Float_t rotationY, Float_t rotationZ);
  
  ~DLadder();
  
  DR3           GetPlanePosition(Int_t sensorID) { return planePosition[sensorID]; }
  DR3           GetPlaneRotation(Int_t sensorID) { return planeRotation[sensorID]; }
  DPlane*       GetPlane(Int_t sensorID)         { return planeList[sensorID]; }
  DPlane*       GetTrackerPlane(Int_t planeID)   { return trackerPlaneList[planeID]; } // JB 2014/02/10

  Int_t         GetNumberOfPlanes()               { return planeList.size(); }

  DR3           GetLadderPosition()               { return ladderPosition; }
  DR3           GetLadderRotation()               { return ladderRotation; }

  Int_t         GetStatus()                       { return Status; }

  void          SetSensorLengthX(Float_t sensorLengthX) { fSensorLengthX = sensorLengthX; }
  void          SetSensorSeparator(Float_t sensorSeparator) { fSensorSeparator = sensorSeparator; }
  void          SetDistanceFromFoam(Float_t distanceFromFoam) { fDistanceFromFoam = distanceFromFoam; }

  void          SetRotation(Float_t rotationX, Float_t rotationY, Float_t rotationZ);

  void          UpdateAlignment();
  void          GetRelativePosition( Int_t aPlaneNumber, DR3* aTilt, DR3* aShift); // JB 2014/02/10
  
  void          AlignLadder(std::vector<Float_t>& DataTemp, const Int_t ladderNumber, const Int_t planeNumber);
  DR3           PlaneToLadder(DR3& aPlaneCoordinate, Int_t aPlaneNumber); // JB 2014/02/17

  void          MakeMiniVectors(Int_t planeNumber);
 
  void          MakeTrack(DR3& positionLeft, DR3& positionRight, DTrack* myTrack);
  void          MakeTrack2(DHit* myHitLeft, DHit* myHitRight, DTrack* myTrack, Int_t& number);
  void          MakeTrack4(DHit* myHitLeft, DHit* myHitRight, DHit* myHitLeft2, DHit* myHitRight2, DTrack* myTrack, Int_t& number);
  
  
  std::vector<DTrack*>&  GetMiniVectors()         { return trackVector; }
  std::vector<DR3>&      GetDxDyDz()              { return dxdydzVector; }
  std::vector<DR3>&      GetCenterLadderPosition() { return centerLadderVector; }

  void          AddPlane(DPlane* aPlane, Int_t planeID);
  void          SetLadderType(TString aType)      { ladderType = aType;};

  TString       GetPlaneType()                    { return ladderType; }
  TString       GetLadderType()                    { return ladderType; }

  DTracker      *GetTracker()                     { return fTracker;  }
  DPrecAlign*   GetPrecAlign()                    { return fPrecAlign; }

  void          Display(const Int_t planeNumber);
  void          PrepareDisplay();
  void          FillResidus(std::vector<Float_t>& Data, const Int_t planeNumber, DR3& dxdydz);
  void          FillResidus(std::vector<Float_t>& Data, const Int_t planeNumber);

  double        RotationCoordX(double x, double y, double z, double angleX, double angleY, double angleZ);
  double        RotationCoordY(double x, double y, double z, double angleX, double angleY, double angleZ);
  double        RotationCoordZ(double x, double y, double z, double angleX, double angleY, double angleZ);

  ClassDef(DLadder,3)                      // Detector Ladder

};

#endif
