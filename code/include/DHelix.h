// Author   :  Daniel Cuesta

#ifndef _DHelix_included_
#define _DHelix_included_

  ////////////////////////////////////////////////////////////
  //                                                        //
  // Class Description of DHit                              //
  //                                                        //
  ////////////////////////////////////////////////////////////

#include "Riostream.h"
#include <math.h>
#include <vector>

// ROOT classes
#include "TObject.h"
#include "TArrayF.h"
#include "TAxis.h"
#include "DPixel.h"
#include "DR3.h"
//#include "DPlane.h"

// class DTrack;                  // forward declarations
// class DStrip;
// class DPlane;
// class DCut;
class DR3;
class DHelix : public TObject {
private:
  Double_t         fTransverseMomentum;
  Double_t         fPhi0;
  Double_t         fDrho;
  Double_t         fDz;
  Double_t         fTanLambda;
  Double_t         fPhi;
  Double_t         fMagneticField;
  DR3              fHelixPosition;
  DR3              fPivotPosition;
public:
  DHelix();
  DHelix(DR3 PivotPosition,double TransverseMomentum,double phi0,double drho,double dz,double TanLambda,double MagneticFiel);
  ~DHelix();
  DR3              GetHelixPosition(double phi);
  // DR3              GetPivotPosition()                     {return fPivotPosition;}
  Double_t           GetTransverseMomentum()                {return fTransverseMomentum;}
  Double_t           GetMagneticField()                     {return fMagneticField;}
  Double_t           GetPhi0()                              {return fPhi0;}
  Double_t           Getdrho()                              {return fDrho;}
  Double_t           Getdz()                                {return fDz;}
  Double_t           GetTanLambda()                         {return fTanLambda;}
  Double_t           GetPhi()                               {return fPhi;}

  void              SetPivotPosition(DR3 NewPivotPosition) {fPivotPosition=NewPivotPosition;}
  void             SetAllParameters(double TransverseMomentum,double phi0,double drho,double dz,double TanLambda);
  void        SetAllParameters(Double_t *par);
  void             SetTransverseMomentum(double SetValue) {fTransverseMomentum=SetValue;}
  void             SetMagneticField(double SetValue)      {fMagneticField=SetValue;}
  void             SetPhi0(double SetValue)               {fPhi0=SetValue;}
  void             Setdrho(double SetValue)               {fDrho=SetValue;}
  void             Setdz(double SetValue)                 {fDz=SetValue;}
  void             SetTanLambda(double SetValue)          {fTanLambda=SetValue;}
  void             SetPhi(double SetValue)                {fPhi=SetValue;}
  ClassDef(DHelix,1)
};
#endif
