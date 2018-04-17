#include <cmath>
#include "DR3.h"
#include "MGlobalAlign.h"
#include "DHit.h"
#include "DLine.h"
#include <iomanip>
#include "DTrackFitter.h"
// DTrackFitter Class : LC 2015/06/24

//_____________________________________________________________________________
//

DTrackFitter::DTrackFitter(Int_t trackParamNumber, Int_t hitNumber, Int_t localAxisNumber, Bool_t ifMultipleScattering, DHit** hitList, DR3 initialTrackParameters, DR3 initialTrackOrigin)
{

  _hitNumber             = hitNumber;
  _localAxisNumber       = localAxisNumber;
  _trackParametersNumber = trackParamNumber;
  _multipleScatteringFit = ifMultipleScattering;

  _fHitList               = hitList;
  _initialTrackParameters = initialTrackParameters;
  _initialTrackOrigin     = initialTrackOrigin;
  
  Int_t _dimResiduals = _hitNumber*_localAxisNumber;

  // Define Matrices :
  _derivativeTrackMatrix   = new TMatrixD(_dimResiduals, _trackParametersNumber);
  _covarianceMatrix        = new TMatrixD(_dimResiduals, _dimResiduals);
  _covarianceMatrixMS      = new TMatrixD(_dimResiduals, _dimResiduals);
  _inverseCovarianceMatrix = new TMatrixD(_dimResiduals, _dimResiduals);
  _finalMatrix             = new TMatrixD(_trackParametersNumber, _trackParametersNumber);
  _inverseFinalMatrix      = new TMatrixD(_trackParametersNumber, _trackParametersNumber);

  // Define Vectors :
  _residualsVector            = new TVectorD(_dimResiduals);
  _residualsVectorMS          = new TVectorD(_dimResiduals); 
  _trackParameters            = new TVectorD(_trackParametersNumber);
  _trackParametersCorrections = new TVectorD(_trackParametersNumber);
  _finalVector                = new TVectorD(_dimResiduals);

  for(Int_t i=0 ; i<_trackParametersNumber ; ++i) {
  
    if(i==0 || i==1) (*_trackParameters)[i] = _initialTrackParameters(i);
    else if(i>1) (*_trackParameters)[i] =   _initialTrackOrigin(i-2);
  }

}

//_____________________________________________________________________________
//

DTrackFitter::~DTrackFitter()
{
  _derivativeTrackMatrix->Zero();
  _covarianceMatrix->Zero();
  _covarianceMatrixMS->Zero();
  _inverseCovarianceMatrix->Zero();
  _finalMatrix->Zero();
  _inverseFinalMatrix->Zero();

  _residualsVector->Zero();
  _residualsVectorMS->Zero();
  _trackParameters->Zero();
  _trackParametersCorrections->Zero();
  _finalVector->Zero();
 
  delete _derivativeTrackMatrix;
  delete _covarianceMatrix;
  delete _covarianceMatrixMS;
  delete _inverseCovarianceMatrix;
  delete _finalMatrix;
  delete _inverseFinalMatrix;

  delete _residualsVector;
  delete _residualsVectorMS;
  delete _trackParameters;
  delete _trackParametersCorrections;
  delete _finalVector;
 
}

// Compute Intersection U :
Double_t DTrackFitter::ComputeTrackIntersectionU(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY,  DR3 trackPoint)
{

  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  Double_t* rotMat = alignParameters->GetRotationMatrix();
 
  Double_t intersectionU = (trackPoint(0)-translations(0)-trackDirectionX*( rotMat[6]*(trackPoint(0)-translations(0))+rotMat[7]*(trackPoint(1)-translations(1))+rotMat[8]*(trackPoint(2)-translations(2)) )/( trackDirectionX*rotMat[6]+trackDirectionY*rotMat[7]+rotMat[8] ) )*rotMat[0]+(trackPoint(1)-translations(1)-trackDirectionY*( rotMat[6]*(trackPoint(0)-translations(0))+rotMat[7]*(trackPoint(1)-translations(1))+rotMat[8]*(trackPoint(2)-translations(2)) )/( trackDirectionX*rotMat[6]+trackDirectionY*rotMat[7]+rotMat[8] ) )*rotMat[1]+(trackPoint(2)-translations(2)-( rotMat[6]*(trackPoint(0)-translations(0))+rotMat[7]*(trackPoint(1)-translations(1))+rotMat[8]*(trackPoint(2)-translations(2)) )/( trackDirectionX*rotMat[6]+trackDirectionY*rotMat[7]+rotMat[8] ))*rotMat[2];

  return intersectionU;

}

// Compute Intersection V :
Double_t DTrackFitter::ComputeTrackIntersectionV(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY,  DR3 trackPoint)
{

  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  Double_t* rotMat = alignParameters->GetRotationMatrix();
 
  Double_t intersectionV = (trackPoint(0)-translations(0)-trackDirectionX*( rotMat[6]*(trackPoint(0)-translations(0))+rotMat[7]*(trackPoint(1)-translations(1))+rotMat[8]*(trackPoint(2)-translations(2)) )/( trackDirectionX*rotMat[6]+trackDirectionY*rotMat[7]+rotMat[8] ))*rotMat[3]+(trackPoint(1)-translations(1)-trackDirectionY*( rotMat[6]*(trackPoint(0)-translations(0))+rotMat[7]*(trackPoint(1)-translations(1))+rotMat[8]*(trackPoint(2)-translations(2)) )/( trackDirectionX*rotMat[6]+trackDirectionY*rotMat[7]+rotMat[8] ))*rotMat[4]+(trackPoint(2)-translations(2)-( rotMat[6]*(trackPoint(0)-translations(0))+rotMat[7]*(trackPoint(1)-translations(1))+rotMat[8]*(trackPoint(2)-translations(2)) )/( trackDirectionX*rotMat[6]+trackDirectionY*rotMat[7]+rotMat[8] ))*rotMat[5];

  return intersectionV;

}

//_____________________________________________________________________________
//

Double_t DTrackFitter::ComputeResidualDerivative_U_AboutTrackDirectionX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  Double_t newDerivative = (((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX*sin(rotations(1))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))))*cos(rotations(1))*cos(rotations(0)) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*trackDirectionY*sin(rotations(1))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*sin(rotations(1))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2);
 
  return -newDerivative;

}

//_____________________________________________________________________________
//

Double_t DTrackFitter::ComputeResidualDerivative_U_AboutTrackDirectionY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  Double_t newDerivative = -((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX*cos(rotations(1))*cos(rotations(1))*cos(rotations(0))*sin(rotations(2))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) - (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionY*cos(rotations(1))*sin(rotations(2))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)))) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*cos(rotations(1))*sin(rotations(2))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2);

  return -newDerivative;
}

//_____________________________________________________________________________
//

Double_t DTrackFitter::ComputeResidualDerivative_V_AboutTrackDirectionX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  Double_t newDerivative = (((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX*sin(rotations(1))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))))*cos(rotations(1))*sin(rotations(0)) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*trackDirectionY*sin(rotations(1))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*sin(rotations(1))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2);

  return -newDerivative;

}

//_____________________________________________________________________________
//

Double_t DTrackFitter::ComputeResidualDerivative_V_AboutTrackDirectionY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  Double_t newDerivative = -((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX*cos(rotations(1))*cos(rotations(1))*sin(rotations(2))*sin(rotations(0))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) - (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionY*cos(rotations(1))*sin(rotations(2))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)))) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*cos(rotations(1))*sin(rotations(2))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2);

  return -newDerivative;

}

Double_t DTrackFitter::ComputeResidualDerivative_U_AboutTrackPointX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  Double_t newDerivative = (trackDirectionX*sin(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) + 1.0)*cos(rotations(1))*cos(rotations(0)) + (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*trackDirectionY*sin(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) + (cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*sin(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)));
 
  return -newDerivative;

}

Double_t DTrackFitter::ComputeResidualDerivative_U_AboutTrackPointY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  Double_t newDerivative = -trackDirectionX*cos(rotations(1))*cos(rotations(1))*cos(rotations(0))*sin(rotations(2))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*(trackDirectionY*cos(rotations(1))*sin(rotations(2))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - 1.0) - (cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*cos(rotations(1))*sin(rotations(2))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)));
 
  return -newDerivative;

}

Double_t DTrackFitter::ComputeResidualDerivative_U_AboutTrackPointZ(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  Double_t newDerivative = -trackDirectionX*cos(rotations(2))*cos(rotations(1))*cos(rotations(1))*cos(rotations(0))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*trackDirectionY*cos(rotations(2))*cos(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - (cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*(cos(rotations(2))*cos(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - 1.0);
 

  return -newDerivative;

}

Double_t DTrackFitter::ComputeResidualDerivative_V_AboutTrackPointX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  Double_t newDerivative = (trackDirectionX*sin(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) + 1.0)*cos(rotations(1))*sin(rotations(0)) + (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*trackDirectionY*sin(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) + (cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*sin(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))); 

  return -newDerivative;

}

Double_t DTrackFitter::ComputeResidualDerivative_V_AboutTrackPointY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  Double_t newDerivative = -trackDirectionX*cos(rotations(1))*cos(rotations(1))*sin(rotations(2))*sin(rotations(0))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*(trackDirectionY*cos(rotations(1))*sin(rotations(2))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - 1.0) - (cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*cos(rotations(1))*sin(rotations(2))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)));

  return -newDerivative;

}

Double_t DTrackFitter::ComputeResidualDerivative_V_AboutTrackPointZ(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  Double_t newDerivative = -trackDirectionX*cos(rotations(2))*cos(rotations(1))*cos(rotations(1))*sin(rotations(0))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*trackDirectionY*cos(rotations(2))*cos(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - (cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*(cos(rotations(2))*cos(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - 1.0);

  return -newDerivative;

}

void DTrackFitter::SetCovarianceMatrixMS()
{

  // Var(U_k) = sigma_k^2 + Sum{ (x_k-x_j)^2 theta_0^2 } sum on j<k
  // Cov(U_k,U_l) = Sum{ (x_k-x_j)(x_l-x_j) theta_0^2  } sum on j<min(k,l)
 
  // --> Stocker les infos dans une map ? ou un vector ? std::map<Int_t, std::vector<Double_t> > _planeInformations;
  // --> 3 vector : std::vector<Double_t> _thetaMS std::vector<Double_t> trackExtrapolation
 
  /* 
  // Variances :
  for(Int_t i=1 ; i<(_hitNumber) ; ++i) {
    
    for(Int_t k=0 ; k<i ; ++k) {
      
      _covarianceMatrixMS[i][i] = pow( (hitPositions[i]-trackExtrapolation[k]) * _thetaMS[k] , 2);
    }
  }
  */

  // Covariances :
  for(Int_t i=0 ; i<_hitNumber*_localAxisNumber ; i+=2) {

    for(Int_t j=0 ; j<_hitNumber*_localAxisNumber ; j+=2) {
      
      for(Int_t k=0 ; k<_hitNumber*_localAxisNumber ; k+=2) { 
  
        if( k<std::min(i,j) ) (*_covarianceMatrixMS)[i][j] += ComputeDistance(_trackExtrapolation[i/2], _trackExtrapolation[k/2]) * ComputeDistance(_trackExtrapolation[j/2], _trackExtrapolation[k/2]) * pow( _thetaMS[k/2], 2);
        //std::cout<<ComputeDistance(_trackExtrapolation[i/2], _trackExtrapolation[k/2]) <<std::endl;
      }

      Double_t covValue = (*_covarianceMatrixMS)[i][j];
      
      //if(_debugLevel>0) std::cout<<"i/j = "<<i<<" "<<j<<" cov[i][j] = "<<covValue<<std::endl;

      (*_covarianceMatrixMS)[i+1][j+1] = covValue; 
      //(*_covarianceMatrixMS)[j][i]     += (*_covarianceMatrix)[i][j];
      //(*_covarianceMatrixMS)[j+1][i+1] += (*_covarianceMatrix)[i][j];

    }
  }

  //if(_debugLevel>0) _covarianceMatrixMS->Print();
  
  *_covarianceMatrix += *_covarianceMatrixMS; 

}

void DTrackFitter::ComputeResidualsMS()
{
  // _hitNumber*_localAxisNumber = dimension

  for(Int_t i=0 ; i<_hitNumber*_localAxisNumber ; i+=2) {

    for(Int_t k=0 ; k<i ; k+=2) (*_residualsVectorMS)[i] += ComputeDistance(_trackExtrapolation[i/2], _trackExtrapolation[k/2]) * _thetaMS[k/2];

    Double_t resValue = (*_residualsVectorMS)[i];
    (*_residualsVectorMS)[i+1] = resValue;
  }

  //if(_debugLevel>0) _residualsVectorMS->Print();

  *_residualsVector += *_residualsVectorMS;
}

Double_t DTrackFitter::ComputeDistance(DR3 firstPlane, DR3 lastPlane)
{
 
 return sqrt( pow( (lastPlane(0)-firstPlane(0)), 2) +   pow( (lastPlane(1)-firstPlane(1)), 2) +  pow( (lastPlane(2)-firstPlane(2)), 2) );
   
}


void DTrackFitter::RedefineTrackParameters()
{

  for(Int_t i=0 ; i<_trackParametersNumber ; ++i) {

    if(i==0 || i==1) _initialTrackParameters(i) = (*_trackParameters)[i]; 
    else if(i>1) _initialTrackOrigin(i-2)       = (*_trackParameters)[i];
  }

}

void DTrackFitter::ProcessHits()
{

  // Set Matrices And Vectors to Zero() :
  _covarianceMatrix->Zero();
  _covarianceMatrixMS->Zero();
  _residualsVector->Zero();
  _residualsVectorMS->Zero();
  _derivativeTrackMatrix->Zero();
  _finalMatrix->Zero(); 
  _inverseFinalMatrix->Zero();
  _trackParametersCorrections->Zero();
  _trackExtrapolation.clear();
  _thetaMS.clear();
  _map_DPrecAlign.clear();
  _map_Planes.clear();

  // Set info to multiple scattering :
  for(Int_t i=0 ; i<_hitNumber ; ++i) {
    _trackExtrapolation.push_back(DR3(0.,0.,0.));
    _thetaMS.push_back(0.);
  }

  for(Int_t iterHit=0 ; iterHit<_hitNumber ; ++iterHit) {
 
    DPlane* myPlane  = _fHitList[iterHit]->GetPlane();
    //Int_t planeIndex = myPlane->GetPlaneNumber();
    DPrecAlign* alignParameters = myPlane->GetPrecAlignment();

    _map_DPrecAlign[iterHit] = alignParameters;
    _map_Planes[iterHit] = myPlane;

    DR3 extrapolationPlaneFrame(0.,0.,0.);

    extrapolationPlaneFrame(0) = ComputeTrackIntersectionU(iterHit, _initialTrackParameters(0), _initialTrackParameters(1), _initialTrackOrigin);
    extrapolationPlaneFrame(1) = ComputeTrackIntersectionV(iterHit, _initialTrackParameters(0), _initialTrackParameters(1), _initialTrackOrigin);
    extrapolationPlaneFrame(2) = 0.;

    _trackExtrapolation[iterHit] = myPlane->PlaneToTracker( extrapolationPlaneFrame );
    _thetaMS[iterHit]            = myPlane->GetSigmaThetaMS();
 
  }

  // Process Hits : 
  
  for(Int_t iterHit=0 ; iterHit<_hitNumber ; ++iterHit) {

    DR3 hitPosition = DR3( _fHitList[iterHit]->GetPositionUhitCG(), _fHitList[iterHit]->GetPositionVhitCG() , 0.);

    Int_t resolutionU = _fHitList[iterHit]->GetResolutionUhit();
    Int_t resolutionV = _fHitList[iterHit]->GetResolutionVhit();

    //DPlane* myPlane  = _fHitList[iterHit]->GetPlane();
    //Int_t planeIndex = myPlane->GetPlaneNumber();

    // Calcule de l'intersection sur le plan iterHit+1 avec trace droite : trackDiractionX/Y = 0 :
    //DR3 Intersection = ComputeIntersection(...);
    DR3 Intersection(0.,0.,0.);

    Intersection(0) = ComputeTrackIntersectionU(iterHit, _initialTrackParameters(0), _initialTrackParameters(1), _initialTrackOrigin);
    Intersection(1) = ComputeTrackIntersectionV(iterHit, _initialTrackParameters(0), _initialTrackParameters(1), _initialTrackOrigin);
    Intersection(2) = 0.;

    (*_residualsVector)[2*iterHit]   = Intersection(0)-hitPosition(0);
    (*_residualsVector)[2*iterHit+1] = Intersection(1)-hitPosition(1);

    (*_covarianceMatrix)[iterHit*2][iterHit*2]     = resolutionU*resolutionU;
    (*_covarianceMatrix)[iterHit*2+1][iterHit*2+1] = resolutionV*resolutionV;

    (*_derivativeTrackMatrix)[2*iterHit][0] = ComputeResidualDerivative_U_AboutTrackDirectionX( iterHit, _initialTrackParameters(0), _initialTrackParameters(1), _initialTrackOrigin);
    (*_derivativeTrackMatrix)[2*iterHit][1] = ComputeResidualDerivative_U_AboutTrackDirectionY( iterHit, _initialTrackParameters(0), _initialTrackParameters(1), _initialTrackOrigin);

    (*_derivativeTrackMatrix)[2*iterHit+1][0] = ComputeResidualDerivative_V_AboutTrackDirectionX( iterHit, _initialTrackParameters(0), _initialTrackParameters(1), _initialTrackOrigin);
    (*_derivativeTrackMatrix)[2*iterHit+1][1] = ComputeResidualDerivative_V_AboutTrackDirectionY( iterHit, _initialTrackParameters(0), _initialTrackParameters(1), _initialTrackOrigin);

    (*_derivativeTrackMatrix)[2*iterHit][2] = ComputeResidualDerivative_U_AboutTrackPointX( iterHit, _initialTrackParameters(0), _initialTrackParameters(1) );
    (*_derivativeTrackMatrix)[2*iterHit][3] = ComputeResidualDerivative_U_AboutTrackPointY( iterHit, _initialTrackParameters(0), _initialTrackParameters(1) );
    //(*_derivativeTrackMatrix)[2*iterHit][4] = ComputeResidualDerivative_U_AboutTrackPointZ( iterHit, _initialTrackParameters(0), _initialTrackParameters(1) );

    (*_derivativeTrackMatrix)[2*iterHit+1][2] = ComputeResidualDerivative_V_AboutTrackPointX( iterHit, _initialTrackParameters(0), _initialTrackParameters(1) );
    (*_derivativeTrackMatrix)[2*iterHit+1][3] = ComputeResidualDerivative_V_AboutTrackPointY( iterHit, _initialTrackParameters(0), _initialTrackParameters(1) );
    //(*_derivativeTrackMatrix)[2*iterHit+1][4] = ComputeResidualDerivative_V_AboutTrackPointZ( iterHit, _initialTrackParameters(0), _initialTrackParameters(1) );


  } // End loop on hits 
 
  if(_multipleScatteringFit==true) {
    SetCovarianceMatrixMS(); 
    ComputeResidualsMS();
  }
  
//  _trackParameters->Print(); // Track parameters before fit  

  ComputeTrackParameters();
/*
  _trackParametersCorrections->Print(); // Coorection to track parameters after track fitting
  _trackParameters->Print();            // New track parameters
*/
  RedefineTrackParameters();

}

void DTrackFitter::ComputeTrackParameters() //FillFinalMatrix()
{
  // Linear developpement : Linear General Least Square : ( H^T * V^-{-1} *  H )^{-1} * H^T * V^{-1} * R 

  TMatrixD transposeDerivativeTrackMatrix = _derivativeTrackMatrix->T();
  _derivativeTrackMatrix->T(); // H^T^T = H

  //*_inverseCovarianceMatrix = _covarianceMatrix->Invert();
  TDecompLU lu0(*_covarianceMatrix);
  lu0.SetTol(1e-50);
  lu0.Invert(*_inverseCovarianceMatrix);


  TMatrixD tempMatrix = transposeDerivativeTrackMatrix * *_inverseCovarianceMatrix * *_derivativeTrackMatrix;
  *_finalMatrix = tempMatrix;

  //_finalMatrix->Print();

  TDecompLU lu(tempMatrix);
  lu.SetTol(1e-50);
  lu.Invert(tempMatrix);

  *_inverseFinalMatrix = tempMatrix;

  *_trackParametersCorrections = *_inverseFinalMatrix * transposeDerivativeTrackMatrix * *_inverseCovarianceMatrix * *_residualsVector;

  *_trackParameters += *_trackParametersCorrections;
}

DR3 DTrackFitter::GetTrackDirections()
{
  DR3 trackParams( (*_trackParameters)[0], (*_trackParameters)[1], 1.); 
  return trackParams;
}

DR3 DTrackFitter::GetTrackOrigin()
{
  DR3 trackOrigin( (*_trackParameters)[2], (*_trackParameters)[3], /*(*_trackParameters)[4]*/0. ); 
  return trackOrigin;
}
