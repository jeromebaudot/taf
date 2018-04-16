//**********************************************************************************//
//Autor: Daniel Cuesta
//Purpose of Class :
//    Holds all the modules needed to define and work with Helix 
//***********************************************************************************//
//                      /!\ /!\ /!\ WORK IN PROGRESS/!\/!\ /!\*
//**********************************************************************************//
#include "DHelix.h"
#include "DR3.h"

ClassImp(DHelix) // Description of a Hit

DHelix::DHelix(){
  fTransverseMomentum = -999;
  fDrho               = -999;
  fPhi0               = -999;
  fDz                 = -999;
  fTanLambda          = -999;
  fPhi                = -999;
  fMagneticField      = -999;
}
//===========================================================================================================
DHelix::DHelix(DR3 PivotPosition,double TransverseMomentum,double phi0,double drho,double dz,double TanLambda,double MagneticField)
{
  fTransverseMomentum = TransverseMomentum;
  fPhi0               = phi0;
  fDrho               = drho;
  fDz                 = dz;
  fTanLambda          = TanLambda;
  fMagneticField      = MagneticField;
  fPivotPosition      = PivotPosition;
  return;
}
DHelix::~DHelix(){
  return;
}
//===========================================================================================================
DR3 DHelix::GetHelixPosition(double phi)
{
  double alpha = 1./(0.3*fMagneticField);
  // double alpha = 1./(3e8*fMagneticField);
  double k     = -1./fTransverseMomentum;

  (fHelixPosition)(0)= ((fDrho*cos(fPhi0))+(alpha/k)*(cos(fPhi0)-cos(fPhi0+phi)));
  (fHelixPosition)(1)= ((fDrho*sin(fPhi0))+(alpha/k)*(sin(fPhi0)-sin(fPhi0+phi)));
  (fHelixPosition)(2)= (fDz-(alpha/k)*(fTanLambda*phi));

return fHelixPosition;
}
//===========================================================================================================
void DHelix::SetAllParameters(Double_t *par)
{
  fTransverseMomentum = 1./par[0];
  fPhi0               = par[1];
  fTanLambda          = par[2];
  fDrho               = par[3];
  fDz                 = par[4];

  return;
}
//===========================================================================================================
void DHelix::SetAllParameters(double TransverseMomentum,double phi0,double drho,double dz,double TanLambda) 
{
  fTransverseMomentum = TransverseMomentum;
  fPhi0               = phi0;
  fTanLambda          = drho;
  fDrho               = dz;
  fDz                 = TanLambda;

  return;
}
//===========================================================================================================
