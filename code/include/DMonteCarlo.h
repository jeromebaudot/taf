#ifndef _DMonteCarlo_included_
#define _DMonteCarlo_included_

#include <math.h>

// ROOT classes
#include "TObject.h"
#include "DR3.h"

class DSetup;
class DPlane;
class TH1F;

class DMonteCarlo : public TObject {

 public:

  DMonteCarlo();                                 
  //DMonteCarlo( Int_t aNumber, const Int_t aIndex, Double_t aValue);
  DMonteCarlo( Int_t aNumber, Double_t aMC_X, Double_t aMC_Y, Double_t aMC_Z, Double_t aValue);
  //DPixel( Int_t aNumber, const Int_t aIndex, Double_t aValue, Int_t aLine, Int_t aColumn, DR3 aPosition, DR3 aSize);
  ~DMonteCarlo();
  
  void               SetPlanenumber(Int_t aNumber) { fPlaneNumber = aNumber; }
  
  void               SetMonteCarloX(Int_t aMonteCarloX) { fMonteCarloX = aMonteCarloX; }
  void               SetMonteCarloY(Int_t aMonteCarloY) { fMonteCarloY = aMonteCarloY; }
  void               SetMonteCarloZ(Int_t aMonteCarloZ) { fMonteCarloZ = aMonteCarloZ; }
  void               SetPosition( DR3 aPosition) { fPosition = aPosition; }
  
  void               SetFound(Bool_t b) { fFound = b; }
  Bool_t             Found() { return  fFound; }
  
  Double_t           GetMonteCarloX() { return fMonteCarloX; }
  Double_t           GetMonteCarloY() { return fMonteCarloY; }
  Double_t           GetMonteCarloZ() { return fMonteCarloZ; }
  Int_t              GetPlaneNumber() { return fPlaneNumber; }

 private:

  Int_t              fPlaneNumber;
  
  Double_t           fMonteCarloX;
  Double_t           fMonteCarloY;
  Double_t           fMonteCarloZ;

  DR3                fPosition; 
  Double_t           fRawValue;
  Bool_t             fFound; 
  Int_t              fDebugMonteCarlo;

  ClassDef(DMonteCarlo,1)                                 // Pixel or Pixel of a Detector Plane

};

#endif


