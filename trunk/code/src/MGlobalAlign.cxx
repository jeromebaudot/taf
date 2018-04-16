// Class for global alignment method.
// Started 2015/02/06. Work in Progress...
#include <cmath>
#include "DR3.h"
#include "MGlobalAlign.h"
#include "DHit.h"
#include "DLine.h"
#include <iomanip>

/*

//--------- ABOUT RESIDUALS and DERIVATIVES -------//

  Rotation Matrix :

  Double_t A = cos(angle_x);
  Double_t B = sin(angle_x);
  Double_t C = cos(angle_y);
  Double_t D = sin(angle_y);
  Double_t E = cos(angle_z);
  Double_t F = sin(angle_z);

  Double_t AD = A*D;
  Double_t BD = B*D;

  // Rotation X,Y,Z :
  _rotmat[0][0] = C*E;
  _rotmat[0][1] = BD*E - A*F;
  _rotmat[0][2] = AD*E + B*F;
  _rotmat[1][0] = C*F;
  _rotmat[1][1] = BD*F + A*E;
  _rotmat[1][2] = AD*F - B*E;
  _rotmat[2][0] = -D;
  _rotmat[2][1] = B*C;
  _rotmat[2][2] = A*C;

  _rotmat[0][0] = Mat[0] = cos(angleY)*cos(angleZ);
  _rotmat[0][1] = Mat[1] = sin(angleX)*sin(angleY)*cos(angleZ) - cos(angleX)*sin(angleZ);
  _rotmat[0][2] = Mat[2] = cos(angleX)*sin(angleY)*cos(angleZ) + sin(angleX)*sin(angleZ);
  _rotmat[1][0] = Mat[3] = cos(angleY)*sin(angleZ);
  _rotmat[1][1] = Mat[4] = sin(angleX)*sin(angleY)*sin(angleZ) + cos(angleX)*cos(angleZ);
  _rotmat[1][2] = Mat[5] = cos(angleX)*sin(angleY)*sin(angleZ) - sin(angleX)*cos(angleZ);
  _rotmat[2][0] = Mat[6] = -sin(angleY);
  _rotmat[2][1] = Mat[7] = sin(angleX)*cos(angleY);
  _rotmat[2][2] = Mat[8] = cos(angleX)*cos(angleY);

  Residual_U = Measure_U - Intersection_U
  Residual_V = Measure_V - Intersection_V

  // Expression in compact form :
  
  R = Rotation Matrix
  
  Intersection_U = (x0-Tx-d*F)*R00+(y0-Ty-e*F)*R01+(z0-Tz-F)*R02
  Intersection_V = (x0-Tx-d*F)*R10+(y0-Ty-e*F)*R11+(z0-Tz-F)*R12

  F = ( R20*(x0-Tx)+R21*(y0-Ty)+R22*(z0-Tz) )/( d*R20+e*R21+R22 )

  Intersection U :

  (X0-TrX-e*( rotMat[6]*(X0-TrX)+rotMat[7]*(Y0-TrY)+rotMat[8]*(Z0-TrZ) )/( e*rotMat[6]+d*rotMat[7]+rotMat[8] ) )*rotMat[0]+(Y0-TrY-d*( rotMat[6]*(X0-TrX)+rotMat[7]*(Y0-TrY)+rotMat[8]*(Z0-TrZ) )/( e*rotMat[6]+d*rotMat[7]+rotMat[8] ) )*rotMat[1]+(Z0-TrZ-( rotMat[6]*(X0-TrX)+rotMat[7]*(Y0-TrY)+rotMat[8]*(Z0-TrZ) )/( e*rotMat[6]+d*rotMat[7]+rotMat[8] ))*rotMat[2];

  Intersection V :

  (X0-TrX-e*( rotMat[6]*(X0-TrX)+rotMat[7]*(Y0-TrY)+rotMat[8]*(Z0-TrZ) )/( e*rotMat[6]+d*rotMat[7]+rotMat[8] ))*rotMat[3]+(Y0-TrY-d*( rotMat[6]*(X0-TrX)+rotMat[7]*(Y0-TrY)+rotMat[8]*(Z0-TrZ) )/( e*rotMat[6]+d*rotMat[7]+rotMat[8] ))*rotMat[4]+(Z0-TrZ-( rotMat[6]*(X0-TrX)+rotMat[7]*(Y0-TrY)+rotMat[8]*(Z0-TrZ) )/( e*rotMat[6]+d*rotMat[7]+rotMat[8] ))*rotMat[5];

*/

MimosaGlobalAlign::MimosaGlobalAlign(const Int_t& planeNumber, const Int_t& DoF, const Int_t& NbTrackPrameters, const Int_t& axisNumberInPlane, const Double_t& bound, const Bool_t alignConstrainsts, const Bool_t trackConstrainsts, const Bool_t multipleScattering)

{ 

  _debugLevel            = 0;
  
  _trackParametersConstrainsts = trackConstrainsts;
  _alignParametersConstrainsts = alignConstrainsts;
  _multipleScatteringFit       = multipleScattering;

  _planeNumber           = planeNumber;
  _axisNumber            = axisNumberInPlane;
  _trackParametersNumber = NbTrackPrameters;
  _degreeOfFreedom       = DoF;

  std::cout<<_planeNumber*_axisNumber<<"  "<<_trackParametersNumber<<"  "<<_degreeOfFreedom*_planeNumber<<std::endl;

  _alignmentVector            = new TVectorD(_planeNumber*_degreeOfFreedom);

  _covarianceMatrix           = new TMatrixD(_planeNumber*_axisNumber, _planeNumber*_axisNumber);

  _covarianceMatrixMS         = new TMatrixD(_planeNumber*_axisNumber, _planeNumber*_axisNumber);

  _inverseCovarianceMatrix    = new TMatrixD(_planeNumber*_axisNumber, _planeNumber*_axisNumber);

  _residualsVector            = new TVectorD(_planeNumber*_axisNumber);

  _residualsVectorMS          = new TVectorD(_planeNumber*_axisNumber);

  _derivativesTracksMatrix    = new TMatrixD(_planeNumber*_axisNumber, _trackParametersNumber);

  _derivativesAlignmentMatrix = new TMatrixD(_planeNumber*_axisNumber, _degreeOfFreedom*_planeNumber);

  _saveDerivativesAlignmentMatrix = new TMatrixD(_planeNumber*_axisNumber, _degreeOfFreedom*_planeNumber);
  
  _saveDerivativesTracksMatrix = new TMatrixD(_planeNumber*_axisNumber, _trackParametersNumber);

  _saveTransposeDerivativesAlignmentMatrix = new TMatrixD(_degreeOfFreedom*_planeNumber, _planeNumber*_axisNumber);
  
  _saveTransposeDerivativesTracksMatrix = new TMatrixD( _trackParametersNumber, _planeNumber*_axisNumber );

  _identity                   = new TMatrixD(_planeNumber*_axisNumber, _planeNumber*_axisNumber);

  _finalMatrix      = new TMatrixD(_degreeOfFreedom*_planeNumber, _degreeOfFreedom*_planeNumber);
  
  _finalInverseMatrix = new TMatrixD(_degreeOfFreedom*_planeNumber, _degreeOfFreedom*_planeNumber);

  _finalVector                = new TVectorD(_degreeOfFreedom*_planeNumber);

  _alignmentCorrections       = new TVectorD(_degreeOfFreedom*_planeNumber);

  _trackParameters            = new TVectorD(_trackParametersNumber);

  //trackParametersCorrections = new TVectorD(NbTrackParameters*NbTracks); // TO BE IMPLEMENTED AFTER :)

  _covarianceMatrixAlignementConstrainsts = new TMatrixD(_degreeOfFreedom*_planeNumber, _degreeOfFreedom*_planeNumber);

  _inverseCovMatAlignConstraints = new TMatrixD(_degreeOfFreedom*_planeNumber, _degreeOfFreedom*_planeNumber);
 
  _identityMatrix = new TMatrixD(_degreeOfFreedom*_planeNumber, _degreeOfFreedom*_planeNumber);
  
  _vectorAlignmentConstraints = new TVectorD(_degreeOfFreedom*_planeNumber);

  _covarianceMatrixTrackConstraints = new TMatrixD(_trackParametersNumber, _trackParametersNumber);
  
  _inverseCovMatTrackConstraints = new TMatrixD(_trackParametersNumber, _trackParametersNumber);
  
  _identidyMatrixTrack = new TMatrixD(_trackParametersNumber, _trackParametersNumber);
  
  _vectorTrackConstraints = new TVectorD(_trackParametersNumber); 

  _saveFinalMatrix = new TMatrixD(_degreeOfFreedom*_planeNumber, _degreeOfFreedom*_planeNumber); 
  
  _matrixU = new TMatrixD(_degreeOfFreedom*_planeNumber, _degreeOfFreedom*_planeNumber);
   
  _matrixV = new TMatrixD(_degreeOfFreedom*_planeNumber, _degreeOfFreedom*_planeNumber);
   
  _singularValues = new TVectorD(_degreeOfFreedom*_planeNumber);
 
  _singularMatrix = new TMatrixD(_degreeOfFreedom*_planeNumber, _degreeOfFreedom*_planeNumber);
  
  _indexResiduals       = 0;
  _indexAlignmentParams = 0;
  _indexTrackParams     = 0;

  _mapParameterName[0] = "Translation X";
  _mapParameterName[1] = "Translation Y";
  _mapParameterName[2] = "Translation Z";
  _mapParameterName[3] = "Rotation X";
  _mapParameterName[4] = "Rotation Y";
  _mapParameterName[5] = "Rotation Z";


   canvasResiduals = new TCanvas*[_planeNumber+1];
   residualsU = new TH1F*[_planeNumber];
   residualsV = new TH1F*[_planeNumber];
 
   _bound = bound; 
/*
   for(Int_t planeIndex=0 ; planeIndex<_planeNumber ; planeIndex++) {

     TString name = "canvasResiduals_Pl";
     name += planeIndex;

     canvasResiduals[planeIndex] = new TCanvas(name.Data(),"Residuals",410,10,750,500);
     canvasResiduals[planeIndex]->Divide(2);
     canvasResiduals[planeIndex]->Update();

     TString nameDU = "du_pl";
     nameDU += planeIndex;

     TString nameDV = "dv_pl";
     nameDV += planeIndex;

     residualsU[planeIndex] = new TH1F(nameDU.Data(),"Delta U", 200, -2.*bound, +2.*bound); 
     residualsV[planeIndex] = new TH1F(nameDV.Data(),"Delta V", 200, -2.*bound, +2.*bound); 
   }
   
   canvasResiduals[_planeNumber] = new TCanvas("Track Parameters","Residuals",410,10,750,500);
   canvasResiduals[_planeNumber]->Divide(2);
   canvasResiduals[_planeNumber]->Update();
   
   trackParameterX = new TH1F("Track Parameter X","Track Parameter X", 200, -0.01, +0.01);
   trackParameterY = new TH1F("Track Parameter Y","Track Parameter Y", 200, -0.01, +0.01);
*/   
}

MimosaGlobalAlign::~MimosaGlobalAlign()
{

  _alignmentVector->Clear();
  _covarianceMatrix->Clear();
  _covarianceMatrixMS->Clear();
  _inverseCovarianceMatrix->Clear();
  _residualsVector->Clear();
  _residualsVectorMS->Clear();
  _derivativesTracksMatrix->Clear();
  _derivativesAlignmentMatrix->Clear();
  _finalMatrix->Clear();
  _finalInverseMatrix->Clear();
  _finalVector->Clear();
  _alignmentCorrections->Clear();
  _covarianceMatrixAlignementConstrainsts->Clear();
  _inverseCovMatAlignConstraints->Clear();
  _identityMatrix->Clear();
  _vectorAlignmentConstraints->Clear();
  _covarianceMatrixTrackConstraints->Clear();
  _inverseCovMatTrackConstraints->Clear();
  _identidyMatrixTrack->Clear();
  _vectorTrackConstraints->Clear();
  _trackParameters->Clear();
  _saveFinalMatrix->Clear();
  _matrixU->Clear();
  _matrixV->Clear();
  _singularValues->Clear();
  _singularMatrix->Clear();
  _saveDerivativesAlignmentMatrix->Clear();
  _saveDerivativesTracksMatrix->Clear();
  _saveTransposeDerivativesAlignmentMatrix->Clear();
  _saveTransposeDerivativesTracksMatrix->Clear();

  delete _alignmentVector;
  delete _covarianceMatrix;
  delete _covarianceMatrixMS;
  delete _inverseCovarianceMatrix;
  delete _residualsVector;
  delete _residualsVectorMS;
  delete _derivativesTracksMatrix;
  delete _derivativesAlignmentMatrix;
  delete _finalMatrix;
  delete _finalInverseMatrix;
  delete _finalVector;
  delete _alignmentCorrections;
  delete _covarianceMatrixAlignementConstrainsts;
  delete _inverseCovMatAlignConstraints;
  delete _identityMatrix;
  delete _vectorAlignmentConstraints;
  delete _covarianceMatrixTrackConstraints;
  delete _inverseCovMatTrackConstraints;
  delete _identidyMatrixTrack;
  delete _vectorTrackConstraints;
  delete _trackParameters;
  delete _saveFinalMatrix;
  delete _matrixU;
  delete _matrixV;
  delete _singularValues;
  delete _singularMatrix;
  delete _saveDerivativesAlignmentMatrix;
  delete _saveDerivativesTracksMatrix;
  delete _saveTransposeDerivativesAlignmentMatrix;
  delete _saveTransposeDerivativesTracksMatrix;

  for(Int_t planeIndex=0 ; planeIndex<_planeNumber ; planeIndex++) {
    delete canvasResiduals[planeIndex];
    delete residualsU[planeIndex]; 
    delete residualsV[planeIndex]; 
  }

  delete canvasResiduals[_planeNumber];

  delete canvasResiduals;
  delete residualsU;
  delete residualsV; 

  delete trackParameterX;
  delete trackParameterY; 

}
/*

// Expresion sans explicitation des angles :
U = (trackPoint(0)-translations(0)-trackDirectionX*F)*rotMat[0]+(trackPoint(1)-translations(1)-trackDirectionY*F)*rotMat[1]+(trackPoint(2)-translations(2)-F)*rotMat[2]
F = ( rotMat[6]*(trackPoint(0)-translations(0))+rotMat[7]*(trackPoint(1)-translations(1))+rotMat[8]*(trackPoint(2)-translations(2)) )/( trackDirectionX*rotMat[6]+trackDirectionY*rotMat[7]+rotMat[8] )
U = (trackPoint(0)-translations(0)-trackDirectionX*( rotMat[6]*(trackPoint(0)-translations(0))+rotMat[7]*(trackPoint(1)-translations(1))+rotMat[8]*(trackPoint(2)-translations(2)) )/( trackDirectionX*rotMat[6]+trackDirectionY*rotMat[7]+rotMat[8] ) )*rotMat[0]+(trackPoint(1)-translations(1)-trackDirectionY*( rotMat[6]*(trackPoint(0)-translations(0))+rotMat[7]*(trackPoint(1)-translations(1))+rotMat[8]*(trackPoint(2)-translations(2)) )/( trackDirectionX*rotMat[6]+trackDirectionY*rotMat[7]+rotMat[8] ) )*rotMat[1]+(trackPoint(2)-translations(2)-( rotMat[6]*(trackPoint(0)-translations(0))+rotMat[7]*(trackPoint(1)-translations(1))+rotMat[8]*(trackPoint(2)-translations(2)) )/( trackDirectionX*rotMat[6]+trackDirectionY*rotMat[7]+rotMat[8] ))*rotMat[2]

*/

// Compute Intersection U :

Double_t MimosaGlobalAlign::ComputeTrackIntersectionU(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY,  DR3 trackPoint)
{

  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  Double_t* rotMat = alignParameters->GetRotationMatrix();
 
  Double_t intersectionU = (trackPoint(0)-translations(0)-trackDirectionX*( rotMat[6]*(trackPoint(0)-translations(0))+rotMat[7]*(trackPoint(1)-translations(1))+rotMat[8]*(trackPoint(2)-translations(2)) )/( trackDirectionX*rotMat[6]+trackDirectionY*rotMat[7]+rotMat[8] ) )*rotMat[0]+(trackPoint(1)-translations(1)-trackDirectionY*( rotMat[6]*(trackPoint(0)-translations(0))+rotMat[7]*(trackPoint(1)-translations(1))+rotMat[8]*(trackPoint(2)-translations(2)) )/( trackDirectionX*rotMat[6]+trackDirectionY*rotMat[7]+rotMat[8] ) )*rotMat[1]+(trackPoint(2)-translations(2)-( rotMat[6]*(trackPoint(0)-translations(0))+rotMat[7]*(trackPoint(1)-translations(1))+rotMat[8]*(trackPoint(2)-translations(2)) )/( trackDirectionX*rotMat[6]+trackDirectionY*rotMat[7]+rotMat[8] ))*rotMat[2];

  //Double_t intersectionU_2 = (trackPoint(0)-translations(0)-trackDirectionX*( (-1)*sin(rotations(1))*(trackPoint(0)-translations(0))+sin(rotations(2))*cos(rotations(1))*(trackPoint(1)-translations(1))+cos(rotations(2))*cos(rotations(1))*(trackPoint(2)-translations(2)) )/( trackDirectionX*(-1)*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)) ) )*cos(rotations(1))*cos(rotations(0))+(trackPoint(1)-translations(1)-trackDirectionY*( (-1)*sin(rotations(1))*(trackPoint(0)-translations(0))+sin(rotations(2))*cos(rotations(1))*(trackPoint(1)-translations(1))+cos(rotations(2))*cos(rotations(1))*(trackPoint(2)-translations(2)) )/( trackDirectionX*(-1)*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)) ) )*(sin(rotations(2))*sin(rotations(1))*cos(rotations(0)) - cos(rotations(2))*sin(rotations(0)))+(trackPoint(2)-translations(2)-( (-1)*sin(rotations(1))*(trackPoint(0)-translations(0))+sin(rotations(2))*cos(rotations(1))*(trackPoint(1)-translations(1))+cos(rotations(2))*cos(rotations(1))*(trackPoint(2)-translations(2)) )/( trackDirectionX*(-1)*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)) ))*(cos(rotations(2))*sin(rotations(1))*cos(rotations(0)) + sin(rotations(2))*sin(rotations(0)));


  return intersectionU;

}

// Definition of all the derivatives of the residuals in local frame.

// For U direction :

Double_t MimosaGlobalAlign::ComputeResidualDerivative_U_AboutTranslationX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();
  
  // Derivee par rapport a translations(0) :
  //Double_t oldDerivative = -( rotMat[0]*((trackDirectionX*rotMat[6])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6])-1)+(rotMat[2]*rotMat[6])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6])+(trackDirectionY*rotMat[1]*rotMat[6])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]) );

  //Double_t newDerivative = -(sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) - (cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) + (2*cos(rotations(1))*cos(rotations(1))*sin(rotations(2)) - sin(rotations(2)))*cos(rotations(0)))*trackDirectionX - (2*cos(rotations(2))*cos(rotations(1))*cos(rotations(1)) - cos(rotations(2)))*cos(rotations(0)))/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1)));

  Double_t newDerivative = -(trackDirectionX*sin(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) + 1.0)*cos(rotations(1))*cos(rotations(0)) - (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*trackDirectionY*sin(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - (cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*sin(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)));

 
 //if(_debugLevel>0) std::cout<<"Old Derivative U --> TrX = "<<oldDerivative<<" New = "<<newDerivative<<std::endl; 

 return -newDerivative;

}

Double_t MimosaGlobalAlign::ComputeResidualDerivative_U_AboutTranslationY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();
  
  // Derivee par rapport a translations(1) :
  //Double_t oldDerivative = -( rotMat[1]*((trackDirectionY*rotMat[7])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6])-1)+(rotMat[2]*rotMat[7])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6])+(trackDirectionX*rotMat[0]*rotMat[7])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]) );

  //Double_t newDerivative = -((cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - (2*sin(rotations(2))*sin(rotations(1))*sin(rotations(1)) - sin(rotations(2)))*cos(rotations(0)))*trackDirectionY - cos(rotations(1))*sin(rotations(0)))/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1)));
 
  //if(_debugLevel>0) std::cout<<"Old Derivative U --> TrY = "<<oldDerivative<<" New = "<<newDerivative<<std::endl;

  Double_t newDerivative = trackDirectionX*cos(rotations(1))*cos(rotations(1))*cos(rotations(0))*sin(rotations(2))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) + (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*(trackDirectionY*cos(rotations(1))*sin(rotations(2))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - 1.0) + (cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*cos(rotations(1))*sin(rotations(2))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)));
 

  return -newDerivative;

}

Double_t MimosaGlobalAlign::ComputeResidualDerivative_U_AboutTranslationZ(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();
  
  // Derivee par rapport a translations(2) :
  //Double_t oldDerivative = -( rotMat[2]*(rotMat[8]/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6])-1)+(trackDirectionY*rotMat[1]*rotMat[8])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6])+(trackDirectionX*rotMat[0]*rotMat[8])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]) );

  //Double_t newDerivative = -(trackDirectionX*cos(rotations(1))*sin(rotations(0)) - (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + (2*cos(rotations(2))*sin(rotations(1))*sin(rotations(1)) - cos(rotations(2)))*cos(rotations(0)))*trackDirectionY)/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1)));
 
  //if(_debugLevel>0) std::cout<<"Old Derivative U --> TrZ = "<<oldDerivative<<" New = "<<newDerivative<<std::endl;

  Double_t newDerivative = trackDirectionX*cos(rotations(2))*cos(rotations(1))*cos(rotations(1))*cos(rotations(0))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) + (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*trackDirectionY*cos(rotations(2))*cos(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) + (cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*(cos(rotations(2))*cos(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - 1.0);
 

  return -newDerivative;
 
}

Double_t MimosaGlobalAlign::ComputeResidualDerivative_U_AboutTrackDirectionX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  // Derivee par rapport a d :
  //Double_t oldDerivative =  -( rotMat[0]*((trackDirectionX*rotMat[6]*((trackPoint(2)-translations(2))*rotMat[8]+(trackPoint(1)-translations(1))*rotMat[7]+(trackPoint(0)-translations(0))*rotMat[6]))/pow((rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]),2)-((trackPoint(2)-translations(2))*rotMat[8]+(trackPoint(1)-translations(1))*rotMat[7]+(trackPoint(0)-translations(0))*rotMat[6])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]))+(trackDirectionY*rotMat[1]*rotMat[6]*((trackPoint(2)-translations(2))*rotMat[8]+(trackPoint(1)-translations(1))*rotMat[7]+(trackPoint(0)-translations(0))*rotMat[6]))/pow((rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]),2)-(rotMat[2]*rotMat[6]*(-(trackPoint(2)-translations(2))*rotMat[8]-(trackPoint(1)-translations(1))*rotMat[7]-(trackPoint(0)-translations(0))*rotMat[6]))/pow((rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]), 2) );

  //Double_t newDerivative =  ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionY*cos(rotations(1))*cos(rotations(1))*cos(rotations(0))*sin(rotations(2))/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))), 2) - (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX*cos(rotations(1))*sin(rotations(2))/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))), 2) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1)))) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*cos(rotations(1))*sin(rotations(2))/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))), 2);
  
  //if(_debugLevel>0) std::cout<<"Old Derivative U --> d = "<<oldDerivative<<" New = "<<newDerivative<<std::endl;
  
  // Old def : X<-->Y
  //Double_t newDerivative = -((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX*cos(rotations(1))*cos(rotations(1))*cos(rotations(0))*sin(rotations(2))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) - (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionY*cos(rotations(1))*sin(rotations(2))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)))) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*cos(rotations(1))*sin(rotations(2))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2);

   // New Definition :
   Double_t newDerivative = (((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX*sin(rotations(1))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))))*cos(rotations(1))*cos(rotations(0)) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*trackDirectionY*sin(rotations(1))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*sin(rotations(1))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2);
 
  return -newDerivative;

}

Double_t MimosaGlobalAlign::ComputeResidualDerivative_U_AboutTrackDirectionY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  // Derivee par rapport a e :
  
  //Double_t oldDerivative = -( rotMat[1]*((trackDirectionY*rotMat[7]*((trackPoint(2)-translations(2))*rotMat[8]+(trackPoint(1)-translations(1))*rotMat[7]+(trackPoint(0)-translations(0))*rotMat[6]))/pow((rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]),2)-((trackPoint(2)-translations(2))*rotMat[8]+(trackPoint(1)-translations(1))*rotMat[7]+(trackPoint(0)-translations(0))*rotMat[6])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]))+(trackDirectionX*rotMat[0]*rotMat[7]*((trackPoint(2)-translations(2))*rotMat[8]+(trackPoint(1)-translations(1))*rotMat[7]+(trackPoint(0)-translations(0))*rotMat[6]))/pow((rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]),2)-(rotMat[2]*rotMat[7]*(-(trackPoint(2)-translations(2))*rotMat[8]-(trackPoint(1)-translations(1))*rotMat[7]-(trackPoint(0)-translations(0))*rotMat[6]))/pow((rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]),2) );

  //Double_t newDerivative = -(((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionY*sin(rotations(1))/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))), 2) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))))*cos(rotations(1))*cos(rotations(0)) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*trackDirectionX*sin(rotations(1))/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))), 2) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*sin(rotations(1))/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))), 2); 

  //if(_debugLevel>0) std::cout<<"Old Derivative U --> e = "<<oldDerivative<<" New = "<<newDerivative<<std::endl;

  // Old Definition X<-->Y :
  //Double_t newDerivative = (((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX*sin(rotations(1))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))))*cos(rotations(1))*cos(rotations(0)) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*trackDirectionY*sin(rotations(1))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*sin(rotations(1))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2);
   
   // New Definition :
   Double_t newDerivative = -((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX*cos(rotations(1))*cos(rotations(1))*cos(rotations(0))*sin(rotations(2))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) - (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionY*cos(rotations(1))*sin(rotations(2))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)))) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*cos(rotations(1))*sin(rotations(2))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2);

  return -newDerivative;
}

Double_t MimosaGlobalAlign::ComputeResidualDerivative_U_AboutTrackPointX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  // Derivee par rapport a trackPoint(0) :
  //Double_t oldDerivative = -( rotMat[0]*(1-(trackDirectionX*rotMat[6])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]))-(rotMat[2]*rotMat[6])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6])-(trackDirectionY*rotMat[1]*rotMat[6])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]) );

  //Double_t newDerivative = (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) - (cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - (2*sin(rotations(2))*sin(rotations(1))*sin(rotations(1)) - sin(rotations(2)))*cos(rotations(0)))*trackDirectionX + (2*cos(rotations(2))*sin(rotations(1))*sin(rotations(1)) - cos(rotations(2)))*cos(rotations(0)))/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1)));

  //if(_debugLevel>0) std::cout<<"Old Derivative U --> X0 = "<<oldDerivative<<" New = "<<newDerivative<<std::endl;

  Double_t newDerivative = (trackDirectionX*sin(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) + 1.0)*cos(rotations(1))*cos(rotations(0)) + (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*trackDirectionY*sin(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) + (cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*sin(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)));
 
  return -newDerivative;

}

Double_t MimosaGlobalAlign::ComputeResidualDerivative_U_AboutTrackPointY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  // Derivee par rapport a trackPoint(1) :
  //Double_t oldDerivative = -( rotMat[1]*(1-(trackDirectionY*rotMat[7])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]))-(rotMat[2]*rotMat[7])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6])-(trackDirectionX*rotMat[0]*rotMat[7])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]) );

  //Double_t newDerivative = ((cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) + (2*cos(rotations(1))*cos(rotations(1))*sin(rotations(2)) - sin(rotations(2)))*cos(rotations(0)))*trackDirectionY - cos(rotations(1))*sin(rotations(0)))/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1)));

  //if(_debugLevel>0) std::cout<<"Old Derivative U --> Y0 = "<<oldDerivative<<" New = "<<newDerivative<<std::endl;

  Double_t newDerivative = -trackDirectionX*cos(rotations(1))*cos(rotations(1))*cos(rotations(0))*sin(rotations(2))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*(trackDirectionY*cos(rotations(1))*sin(rotations(2))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - 1.0) - (cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*cos(rotations(1))*sin(rotations(2))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)));
 
  return -newDerivative;

}

Double_t MimosaGlobalAlign::ComputeResidualDerivative_U_AboutTrackPointZ(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  // Derivee par rapport a trackPoint(2) :
  //Double_t oldDerivative = -( rotMat[2]*(1-rotMat[8]/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]))-(trackDirectionY*rotMat[1]*rotMat[8])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6])-(trackDirectionX*rotMat[0]*rotMat[8])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]) );

  //Double_t newDerivative = (trackDirectionX*cos(rotations(1))*sin(rotations(0)) - (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) - (2*cos(rotations(2))*cos(rotations(1))*cos(rotations(1)) - cos(rotations(2)))*cos(rotations(0)))*trackDirectionY)/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1)));

  //if(_debugLevel>0) std::cout<<"Old Derivative U --> Z0 = "<<oldDerivative<<" New = "<<newDerivative<<std::endl;

  Double_t newDerivative = -trackDirectionX*cos(rotations(2))*cos(rotations(1))*cos(rotations(1))*cos(rotations(0))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*trackDirectionY*cos(rotations(2))*cos(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - (cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*(cos(rotations(2))*cos(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - 1.0);
 

  return -newDerivative;

}

Double_t MimosaGlobalAlign::ComputeResidualDerivative_U_AboutRotationX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  // Derivee Ru = U(hit)-U(extraolation) par rapport a rotations(2) :
  
  //Double_t oldDerivative =  -(((trackPoint(2)-translations(2))*sin(rotations(2))*cos(rotations(1))-(trackPoint(1)-translations(1))*cos(rotations(2))*cos(rotations(1)))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)))-((trackDirectionY*cos(rotations(2))*cos(rotations(1))-sin(rotations(2))*cos(rotations(1)))*((trackPoint(0)-translations(0))*sin(rotations(1))-(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))-(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1))))/pow((-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1))),2))*(sin(rotations(2))*sin(rotations(0))+cos(rotations(2))*sin(rotations(1))*cos(rotations(0)))-(-(trackDirectionY*(-(trackPoint(0)-translations(0))*sin(rotations(1))+(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))+(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1))))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)))+trackPoint(1)-translations(1))*(sin(rotations(2))*sin(rotations(0))+cos(rotations(2))*sin(rotations(1))*cos(rotations(0)))-(((trackPoint(0)-translations(0))*sin(rotations(1))-(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))-(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1)))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)))+trackPoint(2)-translations(2))*(cos(rotations(2))*sin(rotations(0))-sin(rotations(2))*sin(rotations(1))*cos(rotations(0)))-((trackDirectionY*(trackDirectionY*cos(rotations(2))*cos(rotations(1))-sin(rotations(2))*cos(rotations(1)))*(-(trackPoint(0)-translations(0))*sin(rotations(1))+(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))+(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1))))/pow((-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1))),2)-(trackDirectionY*((trackPoint(1)-translations(1))*cos(rotations(2))*cos(rotations(1))-(trackPoint(2)-translations(2))*sin(rotations(2))*cos(rotations(1))))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1))))*(sin(rotations(2))*sin(rotations(1))*cos(rotations(0))-cos(rotations(2))*sin(rotations(0)))-cos(rotations(1))*((trackDirectionX*(trackDirectionY*cos(rotations(2))*cos(rotations(1))-sin(rotations(2))*cos(rotations(1)))*(-(trackPoint(0)-translations(0))*sin(rotations(1))+(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))+(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1))))/pow((-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1))),2)-(trackDirectionX*((trackPoint(1)-translations(1))*cos(rotations(2))*cos(rotations(1))-(trackPoint(2)-translations(2))*sin(rotations(2))*cos(rotations(1))))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1))))*cos(rotations(0)); 

  //Double_t newDerivative =  (((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionX*cos(rotations(2))*cos(rotations(1)) - cos(rotations(1))*sin(rotations(2)))*trackDirectionY/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))), 2) - ((translations(1) - trackPoint(1))*cos(rotations(2))*cos(rotations(1)) - (translations(2) - trackPoint(2))*cos(rotations(1))*sin(rotations(2)))*trackDirectionY/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))))*cos(rotations(1))*cos(rotations(0)) - (cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*(translations(1) - trackPoint(1) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1)))) + (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*(translations(2) - trackPoint(2) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1)))) - (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionX*cos(rotations(2))*cos(rotations(1)) - cos(rotations(1))*sin(rotations(2)))*trackDirectionX/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))), 2) - ((translations(1) - trackPoint(1))*cos(rotations(2))*cos(rotations(1)) - (translations(2) - trackPoint(2))*cos(rotations(1))*sin(rotations(2)))*trackDirectionX/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1)))) - (cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionX*cos(rotations(2))*cos(rotations(1)) - cos(rotations(1))*sin(rotations(2)))/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))), 2) - ((translations(1) - trackPoint(1))*cos(rotations(2))*cos(rotations(1)) - (translations(2) - trackPoint(2))*cos(rotations(1))*sin(rotations(2)))/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))));

  //if(_debugLevel>0) std::cout<<"Old Derivative U --> angleX = "<<oldDerivative<<" New = "<<newDerivative<<std::endl;

  Double_t newDerivative  = -(((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionY*cos(rotations(2))*cos(rotations(1)) - cos(rotations(1))*sin(rotations(2)))*trackDirectionX/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) - ((translations(1) - trackPoint(1))*cos(rotations(2))*cos(rotations(1)) - (translations(2) - trackPoint(2))*cos(rotations(1))*sin(rotations(2)))*trackDirectionX/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))))*cos(rotations(1))*cos(rotations(0)) - (cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*(translations(1) - trackPoint(1) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionY/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)))) + (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*(translations(2) - trackPoint(2) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)))) - (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionY*cos(rotations(2))*cos(rotations(1)) - cos(rotations(1))*sin(rotations(2)))*trackDirectionY/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) - ((translations(1) - trackPoint(1))*cos(rotations(2))*cos(rotations(1)) - (translations(2) - trackPoint(2))*cos(rotations(1))*sin(rotations(2)))*trackDirectionY/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)))) - (cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionY*cos(rotations(2))*cos(rotations(1)) - cos(rotations(1))*sin(rotations(2)))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) - ((translations(1) - trackPoint(1))*cos(rotations(2))*cos(rotations(1)) - (translations(2) - trackPoint(2))*cos(rotations(1))*sin(rotations(2)))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))));
  
  return -newDerivative;

}

Double_t MimosaGlobalAlign::ComputeResidualDerivative_U_AboutRotationY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  // Derivee Ru = U(hit)-U(extraolation) par rapport a rotations(1) :
  //Double_t oldDerivative = -(((trackPoint(1)-translations(1))*sin(rotations(2))*sin(rotations(1))+(trackPoint(2)-translations(2))*cos(rotations(2))*sin(rotations(1))+(trackPoint(0)-translations(0))*cos(rotations(1)))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)))-(((trackPoint(0)-translations(0))*sin(rotations(1))-(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))-(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1)))*(-trackDirectionY*sin(rotations(2))*sin(rotations(1))-cos(rotations(2))*sin(rotations(1))-trackDirectionX*cos(rotations(1))))/pow((-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1))),2))*(sin(rotations(2))*sin(rotations(0))+cos(rotations(2))*sin(rotations(1))*cos(rotations(0)))-((trackDirectionY*(-(trackPoint(0)-translations(0))*sin(rotations(1))+(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))+(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1)))*(-trackDirectionY*sin(rotations(2))*sin(rotations(1))-cos(rotations(2))*sin(rotations(1))-trackDirectionX*cos(rotations(1))))/pow((-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1))),2)-(trackDirectionY*(-(trackPoint(1)-translations(1))*sin(rotations(2))*sin(rotations(1))-(trackPoint(2)-translations(2))*cos(rotations(2))*sin(rotations(1))-(trackPoint(0)-translations(0))*cos(rotations(1))))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1))))*(sin(rotations(2))*sin(rotations(1))*cos(rotations(0))-cos(rotations(2))*sin(rotations(0)))-cos(rotations(1))*((trackDirectionX*(-(trackPoint(0)-translations(0))*sin(rotations(1))+(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))+(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1)))*(-trackDirectionY*sin(rotations(2))*sin(rotations(1))-cos(rotations(2))*sin(rotations(1))-trackDirectionX*cos(rotations(1))))/pow((-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1))),2)-(trackDirectionX*(-(trackPoint(1)-translations(1))*sin(rotations(2))*sin(rotations(1))-(trackPoint(2)-translations(2))*cos(rotations(2))*sin(rotations(1))-(trackPoint(0)-translations(0))*cos(rotations(1))))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1))))*cos(rotations(0))-cos(rotations(2))*cos(rotations(1))*(((trackPoint(0)-translations(0))*sin(rotations(1))-(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))-(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1)))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)))+trackPoint(2)-translations(2))*cos(rotations(0))-sin(rotations(2))*cos(rotations(1))*(-(trackDirectionY*(-(trackPoint(0)-translations(0))*sin(rotations(1))+(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))+(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1))))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)))+trackPoint(1)-translations(1))*cos(rotations(0))+sin(rotations(1))*(-(trackDirectionX*(-(trackPoint(0)-translations(0))*sin(rotations(1))+(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))+(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1))))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)))+trackPoint(0)-translations(0))*cos(rotations(0));

  //Double_t newDerivative =  -(translations(2) - trackPoint(2) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))))*cos(rotations(2))*cos(rotations(1))*cos(rotations(0)) - (translations(1) - trackPoint(1) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))))*cos(rotations(1))*cos(rotations(0))*sin(rotations(2)) + (((translations(2) - trackPoint(2))*cos(rotations(2))*sin(rotations(1)) + (translations(1) - trackPoint(1))*sin(rotations(2))*sin(rotations(1)) + (translations(0) - trackPoint(0))*cos(rotations(1)))*trackDirectionY/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionX*sin(rotations(2))*sin(rotations(1)) + trackDirectionY*cos(rotations(1)) + cos(rotations(2))*sin(rotations(1)))*trackDirectionY/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))), 2))*cos(rotations(1))*cos(rotations(0)) - (translations(0) - trackPoint(0) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionY/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))))*cos(rotations(0))*sin(rotations(1)) - (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*sin(rotations(1)) + (translations(1) - trackPoint(1))*sin(rotations(2))*sin(rotations(1)) + (translations(0) - trackPoint(0))*cos(rotations(1)))*trackDirectionX/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionX*sin(rotations(2))*sin(rotations(1)) + trackDirectionY*cos(rotations(1)) + cos(rotations(2))*sin(rotations(1)))*trackDirectionX/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))), 2)) - (cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*sin(rotations(1)) + (translations(1) - trackPoint(1))*sin(rotations(2))*sin(rotations(1)) + (translations(0) - trackPoint(0))*cos(rotations(1)))/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionX*sin(rotations(2))*sin(rotations(1)) + trackDirectionY*cos(rotations(1)) + cos(rotations(2))*sin(rotations(1)))/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))),2));

  //if(_debugLevel>0) std::cout<<"Old Derivative U --> AngleY = "<<oldDerivative<<" New = "<<newDerivative<<std::endl;

  Double_t newDerivative = -(translations(2) - trackPoint(2) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))))*cos(rotations(2))*cos(rotations(1))*cos(rotations(0)) - (translations(1) - trackPoint(1) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionY/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))))*cos(rotations(1))*cos(rotations(0))*sin(rotations(2)) - (((translations(2) - trackPoint(2))*cos(rotations(2))*sin(rotations(1)) + (translations(1) - trackPoint(1))*sin(rotations(2))*sin(rotations(1)) + (translations(0) - trackPoint(0))*cos(rotations(1)))*trackDirectionX/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionY*sin(rotations(2))*sin(rotations(1)) + trackDirectionX*cos(rotations(1)) + cos(rotations(2))*sin(rotations(1)))*trackDirectionX/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))),2 ) )*cos(rotations(1))*cos(rotations(0)) + (translations(0) - trackPoint(0) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))))*cos(rotations(0))*sin(rotations(1)) - (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*sin(rotations(1)) + (translations(1) - trackPoint(1))*sin(rotations(2))*sin(rotations(1)) + (translations(0) - trackPoint(0))*cos(rotations(1)))*trackDirectionY/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionY*sin(rotations(2))*sin(rotations(1)) + trackDirectionX*cos(rotations(1)) + cos(rotations(2))*sin(rotations(1)))*trackDirectionY/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) ) - (cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*sin(rotations(1)) + (translations(1) - trackPoint(1))*sin(rotations(2))*sin(rotations(1)) + (translations(0) - trackPoint(0))*cos(rotations(1)))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionY*sin(rotations(2))*sin(rotations(1)) + trackDirectionX*cos(rotations(1)) + cos(rotations(2))*sin(rotations(1)))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) );

  return -newDerivative;

}

Double_t MimosaGlobalAlign::ComputeResidualDerivative_U_AboutRotationZ(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  // Derivee Ru = U(hit)-U(extraolation) par rapport a rotations(0) :
  //Double_t oldDerivative = -(-(trackDirectionY*(-(trackPoint(0)-translations(0))*sin(rotations(1))+(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))+(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1))))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)))+trackPoint(1)-translations(1))*(-sin(rotations(2))*sin(rotations(1))*sin(rotations(0))-cos(rotations(2))*cos(rotations(0)))-(((trackPoint(0)-translations(0))*sin(rotations(1))-(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))-(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1)))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)))+trackPoint(2)-translations(2))*(sin(rotations(2))*cos(rotations(0))-cos(rotations(2))*sin(rotations(1))*sin(rotations(0)))+cos(rotations(1))*(-(trackDirectionX*(-(trackPoint(0)-translations(0))*sin(rotations(1))+(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))+(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1))))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)))+trackPoint(0)-translations(0))*sin(rotations(0));
 
  //Double_t newDerivative = -(translations(0) - trackPoint(0) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionY/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))))*cos(rotations(1))*sin(rotations(0)) + (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*(translations(1) - trackPoint(1) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1)))) + (cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*(translations(2) - trackPoint(2) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))));

  //if(_debugLevel>0) std::cout<<"Old Derivative U --> angleZ = "<<oldDerivative<<" New = "<<newDerivative<<std::endl;

  Double_t newDerivative = (translations(0) - trackPoint(0) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))))*cos(rotations(1))*sin(rotations(0)) + (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*(translations(1) - trackPoint(1) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)))) + (cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*(translations(2) - trackPoint(2) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))));

  return -newDerivative;

}

/*
// V Definition :
V = (trackPoint(0)-translations(0)-trackDirectionX*F)*rotMat[3]+(trackPoint(1)-translations(1)-trackDirectionY*F)*rotMat[4]+(trackPoint(2)-translations(2)-F)*rotMat[5]
F = ( rotMat[6]*(trackPoint(0)-translations(0))+rotMat[7]*(trackPoint(1)-translations(1))+rotMat[8]*(trackPoint(2)-translations(2)) )/( trackDirectionX*rotMat[6]+trackDirectionY*rotMat[7]+rotMat[8] )
F = ( -sin(rotations(1))*(trackPoint(0)-translations(0)) + sin(rotations(2))*cos(rotations(1))*(trackPoint(1)-translations(1)) + cos(rotations(2))*cos(rotations(1))*(trackPoint(2)-translations(2)) )/( -sin(rotations(1))*d + sin(rotations(2))*cos(rotations(1))*e + cos(rotations(2))*cos(rotations(1)) )
V = (trackPoint(0)-translations(0)-trackDirectionX*( rotMat[6]*(trackPoint(0)-translations(0))+rotMat[7]*(trackPoint(1)-translations(1))+rotMat[8]*(trackPoint(2)-translations(2)) )/( trackDirectionX*rotMat[6]+trackDirectionY*rotMat[7]+rotMat[8] ))*rotMat[3]+(trackPoint(1)-translations(1)-trackDirectionY*( rotMat[6]*(trackPoint(0)-translations(0))+rotMat[7]*(trackPoint(1)-translations(1))+rotMat[8]*(trackPoint(2)-translations(2)) )/( trackDirectionX*rotMat[6]+trackDirectionY*rotMat[7]+rotMat[8] ))*rotMat[4]+(trackPoint(2)-translations(2)-( rotMat[6]*(trackPoint(0)-translations(0))+rotMat[7]*(trackPoint(1)-translations(1))+rotMat[8]*(trackPoint(2)-translations(2)) )/( trackDirectionX*rotMat[6]+trackDirectionY*rotMat[7]+rotMat[8] ))*rotMat[5]
V = ( trackPoint(0)-translations(0)-trackDirectionX*( -sin(rotations(1))*(trackPoint(0)-translations(0)) + sin(rotations(2))*cos(rotations(1))*(trackPoint(1)-translations(1)) + cos(rotations(2))*cos(rotations(1))*(trackPoint(2)-translations(2)) )/( -sin(rotations(1))*d + sin(rotations(2))*cos(rotations(1))*e + cos(rotations(2))*cos(rotations(1)) ) )*(cos(rotations(1))*sin(rotations(0))) + ( trackPoint(1)-translations(1)-trackDirectionY*( -sin(rotations(1))*(trackPoint(0)-translations(0)) + sin(rotations(2))*cos(rotations(1))*(trackPoint(1)-translations(1)) + cos(rotations(2))*cos(rotations(1))*(trackPoint(2)-translations(2)) )/( -sin(rotations(1))*d + sin(rotations(2))*cos(rotations(1))*e + cos(rotations(2))*cos(rotations(1)) ) )*( sin(rotations(2))*sin(rotations(1))*sin(rotations(0))+cos(rotations(2))*cos(rotations(0)) ) + ( trackPoint(2)-translations(2)-( -sin(rotations(1))*(trackPoint(0)-translations(0)) + sin(rotations(2))*cos(rotations(1))*(trackPoint(1)-translations(1)) + cos(rotations(2))*cos(rotations(1))*(trackPoint(2)-translations(2)) )/( -sin(rotations(1))*d + sin(rotations(2))*cos(rotations(1))*e + cos(rotations(2))*cos(rotations(1)) ) )*( cos(rotations(2))*sin(rotations(1))*sin(rotations(0))-sin(rotations(2))*cos(rotations(0))  )
*/

// Compute Intersection V :

Double_t MimosaGlobalAlign::ComputeTrackIntersectionV(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY,  DR3 trackPoint)
{

  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  Double_t* rotMat = alignParameters->GetRotationMatrix();
 
  Double_t intersectionV = (trackPoint(0)-translations(0)-trackDirectionX*( rotMat[6]*(trackPoint(0)-translations(0))+rotMat[7]*(trackPoint(1)-translations(1))+rotMat[8]*(trackPoint(2)-translations(2)) )/( trackDirectionX*rotMat[6]+trackDirectionY*rotMat[7]+rotMat[8] ))*rotMat[3]+(trackPoint(1)-translations(1)-trackDirectionY*( rotMat[6]*(trackPoint(0)-translations(0))+rotMat[7]*(trackPoint(1)-translations(1))+rotMat[8]*(trackPoint(2)-translations(2)) )/( trackDirectionX*rotMat[6]+trackDirectionY*rotMat[7]+rotMat[8] ))*rotMat[4]+(trackPoint(2)-translations(2)-( rotMat[6]*(trackPoint(0)-translations(0))+rotMat[7]*(trackPoint(1)-translations(1))+rotMat[8]*(trackPoint(2)-translations(2)) )/( trackDirectionX*rotMat[6]+trackDirectionY*rotMat[7]+rotMat[8] ))*rotMat[5];

  //Double_t F = ( rotMat[6]*(trackPoint(0)-translations(0))+rotMat[7]*(trackPoint(1)-translations(1))+rotMat[8]*(trackPoint(2)-translations(2)) )/( trackDirectionX*rotMat[6]+trackDirectionY*rotMat[7]+rotMat[8] );

  //Double_t intersectionV = (trackPoint(0)-translations(0)-trackDirectionX*F)*rotMat[3]+(trackPoint(1)-translations(1)-trackDirectionY*F)*rotMat[4]+(trackPoint(2)-translations(2)-F)*rotMat[5];

  //Double_t intersectionV_2 = (trackPoint(0)-translations(0)-trackDirectionX*( (-1.)*sin(rotations(1))*(trackPoint(0)-translations(0))+sin(rotations(2))*cos(rotations(1))*(trackPoint(1)-translations(1))+cos(rotations(2))*cos(rotations(1))*(trackPoint(2)-translations(2)) )/( trackDirectionX*(-1.)*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)) ))*cos(rotations(1))*sin(rotations(0))+(trackPoint(1)-translations(1)-trackDirectionY*( (-1.)*sin(rotations(1))*(trackPoint(0)-translations(0))+sin(rotations(2))*cos(rotations(1))*(trackPoint(1)-translations(1))+cos(rotations(2))*cos(rotations(1))*(trackPoint(2)-translations(2)) )/( trackDirectionX*(-1.)*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)) ))*(sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))+(trackPoint(2)-translations(2)-( (-1.)*sin(rotations(1))*(trackPoint(0)-translations(0))+sin(rotations(2))*cos(rotations(1))*(trackPoint(1)-translations(1))+cos(rotations(2))*cos(rotations(1))*(trackPoint(2)-translations(2)) )/( trackDirectionX*(-1.)*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)) ))*(cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - sin(rotations(2))*cos(rotations(0)));
 

  return intersectionV;

}

// Derivatives for V direction :

Double_t MimosaGlobalAlign::ComputeResidualDerivative_V_AboutTranslationX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  // Derivee par rapport a translations(0) :
  //return -( rotMat[3]*((trackDirectionX*rotMat[6])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6])-1)+(rotMat[5]*rotMat[6])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6])+(trackDirectionY*rotMat[4]*rotMat[6])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]) );

  //return  (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - (cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) - (2*cos(rotations(1))*cos(rotations(1))*sin(rotations(2)) - sin(rotations(2)))*sin(rotations(0)))*trackDirectionX + (2*cos(rotations(2))*cos(rotations(1))*cos(rotations(1)) - cos(rotations(2)))*sin(rotations(0)))/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1)));

  Double_t newDerivative = -(trackDirectionX*sin(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) + 1.0)*cos(rotations(1))*sin(rotations(0)) - (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*trackDirectionY*sin(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - (cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*sin(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)));
  
  return -newDerivative;

}

Double_t MimosaGlobalAlign::ComputeResidualDerivative_V_AboutTranslationY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  // Derivee par rapport a translations(1) :
  //return -( rotMat[4]*((trackDirectionY*rotMat[7])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6])-1)+(rotMat[5]*rotMat[7])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6])+(trackDirectionX*rotMat[3]*rotMat[7])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]) );

  //return  ((cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + (2*sin(rotations(2))*sin(rotations(1))*sin(rotations(1)) - sin(rotations(2)))*sin(rotations(0)))*trackDirectionY - cos(rotations(1))*cos(rotations(0)))/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1)));

  Double_t newDerivative = trackDirectionX*cos(rotations(1))*cos(rotations(1))*sin(rotations(2))*sin(rotations(0))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) + (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*(trackDirectionY*cos(rotations(1))*sin(rotations(2))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - 1.0) + (cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*cos(rotations(1))*sin(rotations(2))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)));
 
  return -newDerivative;

}

Double_t MimosaGlobalAlign::ComputeResidualDerivative_V_AboutTranslationZ(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  // Derivee par rapport a translations(2) :
  //return -( rotMat[5]*(rotMat[8]/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6])-1)+(trackDirectionY*rotMat[4]*rotMat[8])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6])+(trackDirectionX*rotMat[3]*rotMat[8])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]) );

//  return  (trackDirectionX*cos(rotations(1))*cos(rotations(0)) - (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - (2*cos(rotations(2))*sin(rotations(1))*sin(rotations(1)) - cos(rotations(2)))*sin(rotations(0)))*trackDirectionY)/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1)));

  Double_t newDerivative = trackDirectionX*cos(rotations(2))*cos(rotations(1))*cos(rotations(1))*sin(rotations(0))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) + (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*trackDirectionY*cos(rotations(2))*cos(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) + (cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*(cos(rotations(2))*cos(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - 1.0);

  return -newDerivative;

}

Double_t MimosaGlobalAlign::ComputeResidualDerivative_V_AboutTrackDirectionX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  // Derivee par rapport a d :
  //return -( rotMat[3]*((trackDirectionX*rotMat[6]*((trackPoint(2)-translations(2))*rotMat[8]+(trackPoint(1)-translations(1))*rotMat[7]+(trackPoint(0)-translations(0))*rotMat[6]))/pow((rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]),2)-((trackPoint(2)-translations(2))*rotMat[8]+(trackPoint(1)-translations(1))*rotMat[7]+(trackPoint(0)-translations(0))*rotMat[6])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]))+(trackDirectionY*rotMat[4]*rotMat[6]*((trackPoint(2)-translations(2))*rotMat[8]+(trackPoint(1)-translations(1))*rotMat[7]+(trackPoint(0)-translations(0))*rotMat[6]))/pow((rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]),2)-(rotMat[5]*rotMat[6]*(-(trackPoint(2)-translations(2))*rotMat[8]-(trackPoint(1)-translations(1))*rotMat[7]-(trackPoint(0)-translations(0))*rotMat[6]))/pow((rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]),2) );

 //return  ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionY*cos(rotations(1))*cos(rotations(1))*sin(rotations(2))*sin(rotations(0))/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))), 2) - (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX*cos(rotations(1))*sin(rotations(2))/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))), 2) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1)))) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*cos(rotations(1))*sin(rotations(2))/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))), 2);

  // Old Derivative : X<-->Y :
  //Double_t newDerivative = -((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX*cos(rotations(1))*cos(rotations(1))*sin(rotations(2))*sin(rotations(0))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) - (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionY*cos(rotations(1))*sin(rotations(2))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)))) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*cos(rotations(1))*sin(rotations(2))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2);
  
  // New Derivative :
  Double_t newDerivative = (((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX*sin(rotations(1))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))))*cos(rotations(1))*sin(rotations(0)) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*trackDirectionY*sin(rotations(1))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*sin(rotations(1))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2);

  return -newDerivative;

}

Double_t MimosaGlobalAlign::ComputeResidualDerivative_V_AboutTrackDirectionY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  // Derivee par rapport a e :
  //return -( rotMat[4]*((trackDirectionY*rotMat[7]*((trackPoint(2)-translations(2))*rotMat[8]+(trackPoint(1)-translations(1))*rotMat[7]+(trackPoint(0)-translations(0))*rotMat[6]))/pow((rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]),2)-((trackPoint(2)-translations(2))*rotMat[8]+(trackPoint(1)-translations(1))*rotMat[7]+(trackPoint(0)-translations(0))*rotMat[6])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]))+(trackDirectionX*rotMat[3]*rotMat[7]*((trackPoint(2)-translations(2))*rotMat[8]+(trackPoint(1)-translations(1))*rotMat[7]+(trackPoint(0)-translations(0))*rotMat[6]))/pow((rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]),2)-(rotMat[5]*rotMat[7]*(-(trackPoint(2)-translations(2))*rotMat[8]-(trackPoint(1)-translations(1))*rotMat[7]-(trackPoint(0)-translations(0))*rotMat[6]))/pow((rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]),2) );
 
  //return  -(((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionY*sin(rotations(1))/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))), 2) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))))*cos(rotations(1))*sin(rotations(0)) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*trackDirectionX*sin(rotations(1))/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))), 2) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*sin(rotations(1))/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))), 2);

  // Old Derivative X<-->Y :
  //Double_t newDerivative = (((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX*sin(rotations(1))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))))*cos(rotations(1))*sin(rotations(0)) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*trackDirectionY*sin(rotations(1))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) + ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*sin(rotations(1))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2);

  // New Derivative :
  Double_t newDerivative = -((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX*cos(rotations(1))*cos(rotations(1))*sin(rotations(2))*sin(rotations(0))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) - (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionY*cos(rotations(1))*sin(rotations(2))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)))) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*cos(rotations(1))*sin(rotations(2))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2);

  return -newDerivative;

}

Double_t MimosaGlobalAlign::ComputeResidualDerivative_V_AboutTrackPointX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  // Derivee par rapport a trackPoint(0) :
  //return -( rotMat[3]*(1-(trackDirectionX*rotMat[6])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]))-(rotMat[5]*rotMat[6])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6])-(trackDirectionY*rotMat[4]*rotMat[6])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]) );

  //return  -(cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - (cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + (2*sin(rotations(2))*sin(rotations(1))*sin(rotations(1)) - sin(rotations(2)))*sin(rotations(0)))*trackDirectionX - (2*cos(rotations(2))*sin(rotations(1))*sin(rotations(1)) - cos(rotations(2)))*sin(rotations(0)))/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1)));

  Double_t newDerivative = (trackDirectionX*sin(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) + 1.0)*cos(rotations(1))*sin(rotations(0)) + (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*trackDirectionY*sin(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) + (cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*sin(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))); 

  return -newDerivative;

}

Double_t MimosaGlobalAlign::ComputeResidualDerivative_V_AboutTrackPointY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  // Derivee par rapport a trackPoint(1) :
  //return -( rotMat[4]*(1-(trackDirectionY*rotMat[7])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]))-(rotMat[5]*rotMat[7])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6])-(trackDirectionX*rotMat[3]*rotMat[7])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]) );

  //return  -((cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) - (2*cos(rotations(1))*cos(rotations(1))*sin(rotations(2)) - sin(rotations(2)))*sin(rotations(0)))*trackDirectionY - cos(rotations(1))*cos(rotations(0)))/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1)));

  Double_t newDerivative = -trackDirectionX*cos(rotations(1))*cos(rotations(1))*sin(rotations(2))*sin(rotations(0))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*(trackDirectionY*cos(rotations(1))*sin(rotations(2))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - 1.0) - (cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*cos(rotations(1))*sin(rotations(2))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)));

  return -newDerivative;

}

Double_t MimosaGlobalAlign::ComputeResidualDerivative_V_AboutTrackPointZ(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  // Derivee par rapport a trackPoint(2) :
  //return -( rotMat[5]*(1-rotMat[8]/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]))-(trackDirectionY*rotMat[4]*rotMat[8])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6])-(trackDirectionX*rotMat[3]*rotMat[8])/(rotMat[8]+trackDirectionY*rotMat[7]+trackDirectionX*rotMat[6]) );

  //return -(trackDirectionX*cos(rotations(1))*cos(rotations(0)) - (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) + (2*cos(rotations(2))*cos(rotations(1))*cos(rotations(1)) - cos(rotations(2)))*sin(rotations(0)))*trackDirectionY)/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1)));
   
  Double_t newDerivative = -trackDirectionX*cos(rotations(2))*cos(rotations(1))*cos(rotations(1))*sin(rotations(0))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*trackDirectionY*cos(rotations(2))*cos(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - (cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*(cos(rotations(2))*cos(rotations(1))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - 1.0);

  return -newDerivative;

}

Double_t MimosaGlobalAlign::ComputeResidualDerivative_V_AboutRotationX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  // Derivee par rapport a rotations(2) :
  //return -((trackDirectionY*(trackDirectionY*cos(rotations(2))*cos(rotations(1))-sin(rotations(2))*cos(rotations(1)))*(-(trackPoint(0)-translations(0))*sin(rotations(1))+(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))+(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1))))/pow((-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1))),2)-(trackDirectionY*((trackPoint(1)-translations(1))*cos(rotations(2))*cos(rotations(1))-(trackPoint(2)-translations(2))*sin(rotations(2))*cos(rotations(1))))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1))))*(sin(rotations(2))*sin(rotations(1))*sin(rotations(0))+cos(rotations(2))*cos(rotations(0)))-(((trackPoint(0)-translations(0))*sin(rotations(1))-(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))-(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1)))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)))+trackPoint(2)-translations(2))*(-sin(rotations(2))*sin(rotations(1))*sin(rotations(0))-cos(rotations(2))*cos(rotations(0)))-(((trackPoint(2)-translations(2))*sin(rotations(2))*cos(rotations(1))-(trackPoint(1)-translations(1))*cos(rotations(2))*cos(rotations(1)))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)))-((trackDirectionY*cos(rotations(2))*cos(rotations(1))-sin(rotations(2))*cos(rotations(1)))*((trackPoint(0)-translations(0))*sin(rotations(1))-(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))-(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1))))/pow((-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1))),2))*(cos(rotations(2))*sin(rotations(1))*sin(rotations(0))-sin(rotations(2))*cos(rotations(0)))-(-(trackDirectionY*(-(trackPoint(0)-translations(0))*sin(rotations(1))+(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))+(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1))))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)))+trackPoint(1)-translations(1))*(cos(rotations(2))*sin(rotations(1))*sin(rotations(0))-sin(rotations(2))*cos(rotations(0)))-cos(rotations(1))*((trackDirectionX*(trackDirectionY*cos(rotations(2))*cos(rotations(1))-sin(rotations(2))*cos(rotations(1)))*(-(trackPoint(0)-translations(0))*sin(rotations(1))+(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))+(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1))))/pow((-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1))),2)-(trackDirectionX*((trackPoint(1)-translations(1))*cos(rotations(2))*cos(rotations(1))-(trackPoint(2)-translations(2))*sin(rotations(2))*cos(rotations(1))))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1))))*sin(rotations(0));

  //return  (((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionX*cos(rotations(2))*cos(rotations(1)) - cos(rotations(1))*sin(rotations(2)))*trackDirectionY/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))), 2) - ((translations(1) - trackPoint(1))*cos(rotations(2))*cos(rotations(1)) - (translations(2) - trackPoint(2))*cos(rotations(1))*sin(rotations(2)))*trackDirectionY/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))))*cos(rotations(1))*sin(rotations(0)) - (cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*(translations(1) - trackPoint(1) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1)))) + (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*(translations(2) - trackPoint(2) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1)))) - (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionX*cos(rotations(2))*cos(rotations(1)) - cos(rotations(1))*sin(rotations(2)))*trackDirectionX/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))), 2) - ((translations(1) - trackPoint(1))*cos(rotations(2))*cos(rotations(1)) - (translations(2) - trackPoint(2))*cos(rotations(1))*sin(rotations(2)))*trackDirectionX/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1)))) - (cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionX*cos(rotations(2))*cos(rotations(1)) - cos(rotations(1))*sin(rotations(2)))/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))), 2) - ((translations(1) - trackPoint(1))*cos(rotations(2))*cos(rotations(1)) - (translations(2) - trackPoint(2))*cos(rotations(1))*sin(rotations(2)))/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))));

  Double_t newDerivative = -(((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionY*cos(rotations(2))*cos(rotations(1)) - cos(rotations(1))*sin(rotations(2)))*trackDirectionX/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) - ((translations(1) - trackPoint(1))*cos(rotations(2))*cos(rotations(1)) - (translations(2) - trackPoint(2))*cos(rotations(1))*sin(rotations(2)))*trackDirectionX/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))))*cos(rotations(1))*sin(rotations(0)) - (cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*(translations(1) - trackPoint(1) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionY/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)))) + (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*(translations(2) - trackPoint(2) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)))) - (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionY*cos(rotations(2))*cos(rotations(1)) - cos(rotations(1))*sin(rotations(2)))*trackDirectionY/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) - ((translations(1) - trackPoint(1))*cos(rotations(2))*cos(rotations(1)) - (translations(2) - trackPoint(2))*cos(rotations(1))*sin(rotations(2)))*trackDirectionY/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)))) - (cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionY*cos(rotations(2))*cos(rotations(1)) - cos(rotations(1))*sin(rotations(2)))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) - ((translations(1) - trackPoint(1))*cos(rotations(2))*cos(rotations(1)) - (translations(2) - trackPoint(2))*cos(rotations(1))*sin(rotations(2)))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)))); 

  return -newDerivative;

}

Double_t MimosaGlobalAlign::ComputeResidualDerivative_V_AboutRotationY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  // Derivee par rapport a rotations(1) :
  //return -((trackDirectionY*(-(trackPoint(0)-translations(0))*sin(rotations(1))+(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))+(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1)))*(-trackDirectionY*sin(rotations(2))*sin(rotations(1))-cos(rotations(2))*sin(rotations(1))-trackDirectionX*cos(rotations(1))))/pow((-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1))),2)-(trackDirectionY*(-(trackPoint(1)-translations(1))*sin(rotations(2))*sin(rotations(1))-(trackPoint(2)-translations(2))*cos(rotations(2))*sin(rotations(1))-(trackPoint(0)-translations(0))*cos(rotations(1))))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1))))*(sin(rotations(2))*sin(rotations(1))*sin(rotations(0))+cos(rotations(2))*cos(rotations(0)))-(((trackPoint(1)-translations(1))*sin(rotations(2))*sin(rotations(1))+(trackPoint(2)-translations(2))*cos(rotations(2))*sin(rotations(1))+(trackPoint(0)-translations(0))*cos(rotations(1)))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)))-(((trackPoint(0)-translations(0))*sin(rotations(1))-(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))-(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1)))*(-trackDirectionY*sin(rotations(2))*sin(rotations(1))-cos(rotations(2))*sin(rotations(1))-trackDirectionX*cos(rotations(1))))/pow((-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1))),2))*(cos(rotations(2))*sin(rotations(1))*sin(rotations(0))-sin(rotations(2))*cos(rotations(0)))-cos(rotations(1))*((trackDirectionX*(-(trackPoint(0)-translations(0))*sin(rotations(1))+(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))+(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1)))*(-trackDirectionY*sin(rotations(2))*sin(rotations(1))-cos(rotations(2))*sin(rotations(1))-trackDirectionX*cos(rotations(1))))/pow((-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1))),2)-(trackDirectionX*(-(trackPoint(1)-translations(1))*sin(rotations(2))*sin(rotations(1))-(trackPoint(2)-translations(2))*cos(rotations(2))*sin(rotations(1))-(trackPoint(0)-translations(0))*cos(rotations(1))))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1))))*sin(rotations(0))-cos(rotations(2))*cos(rotations(1))*(((trackPoint(0)-translations(0))*sin(rotations(1))-(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))-(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1)))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)))+trackPoint(2)-translations(2))*sin(rotations(0))-sin(rotations(2))*cos(rotations(1))*(-(trackDirectionY*(-(trackPoint(0)-translations(0))*sin(rotations(1))+(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))+(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1))))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)))+trackPoint(1)-translations(1))*sin(rotations(0))+sin(rotations(1))*(-(trackDirectionX*(-(trackPoint(0)-translations(0))*sin(rotations(1))+(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))+(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1))))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)))+trackPoint(0)-translations(0))*sin(rotations(0));

  //return  -(translations(2) - trackPoint(2) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))))*cos(rotations(2))*cos(rotations(1))*sin(rotations(0)) - (translations(1) - trackPoint(1) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))))*cos(rotations(1))*sin(rotations(2))*sin(rotations(0)) + (((translations(2) - trackPoint(2))*cos(rotations(2))*sin(rotations(1)) + (translations(1) - trackPoint(1))*sin(rotations(2))*sin(rotations(1)) + (translations(0) - trackPoint(0))*cos(rotations(1)))*trackDirectionY/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionX*sin(rotations(2))*sin(rotations(1)) + trackDirectionY*cos(rotations(1)) + cos(rotations(2))*sin(rotations(1)))*trackDirectionY/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))), 2))*cos(rotations(1))*sin(rotations(0)) - (translations(0) - trackPoint(0) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionY/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))))*sin(rotations(1))*sin(rotations(0)) - (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*sin(rotations(1)) + (translations(1) - trackPoint(1))*sin(rotations(2))*sin(rotations(1)) + (translations(0) - trackPoint(0))*cos(rotations(1)))*trackDirectionX/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionX*sin(rotations(2))*sin(rotations(1)) + trackDirectionY*cos(rotations(1)) + cos(rotations(2))*sin(rotations(1)))*trackDirectionX/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))), 2)) - (cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*sin(rotations(1)) + (translations(1) - trackPoint(1))*sin(rotations(2))*sin(rotations(1)) + (translations(0) - trackPoint(0))*cos(rotations(1)))/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionX*sin(rotations(2))*sin(rotations(1)) + trackDirectionY*cos(rotations(1)) + cos(rotations(2))*sin(rotations(1)))/pow( (trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))), 2) );

  Double_t newDerivative = -(translations(2) - trackPoint(2) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))))*cos(rotations(2))*cos(rotations(1))*sin(rotations(0)) - (translations(1) - trackPoint(1) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionY/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))))*cos(rotations(1))*sin(rotations(2))*sin(rotations(0)) - (((translations(2) - trackPoint(2))*cos(rotations(2))*sin(rotations(1)) + (translations(1) - trackPoint(1))*sin(rotations(2))*sin(rotations(1)) + (translations(0) - trackPoint(0))*cos(rotations(1)))*trackDirectionX/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionY*sin(rotations(2))*sin(rotations(1)) + trackDirectionX*cos(rotations(1)) + cos(rotations(2))*sin(rotations(1)))*trackDirectionX/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) )*cos(rotations(1))*sin(rotations(0)) + (translations(0) - trackPoint(0) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))))*sin(rotations(1))*sin(rotations(0)) - (sin(rotations(2))*sin(rotations(1))*sin(rotations(0)) + cos(rotations(2))*cos(rotations(0)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*sin(rotations(1)) + (translations(1) - trackPoint(1))*sin(rotations(2))*sin(rotations(1)) + (translations(0) - trackPoint(0))*cos(rotations(1)))*trackDirectionY/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionY*sin(rotations(2))*sin(rotations(1)) + trackDirectionX*cos(rotations(1)) + cos(rotations(2))*sin(rotations(1)))*trackDirectionY/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) ) - (cos(rotations(2))*sin(rotations(1))*sin(rotations(0)) - cos(rotations(0))*sin(rotations(2)))*(((translations(2) - trackPoint(2))*cos(rotations(2))*sin(rotations(1)) + (translations(1) - trackPoint(1))*sin(rotations(2))*sin(rotations(1)) + (translations(0) - trackPoint(0))*cos(rotations(1)))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*(trackDirectionY*sin(rotations(2))*sin(rotations(1)) + trackDirectionX*cos(rotations(1)) + cos(rotations(2))*sin(rotations(1)))/pow( (trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))), 2) );

  return -newDerivative;

}

Double_t MimosaGlobalAlign::ComputeResidualDerivative_V_AboutRotationZ(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint)
{
  DPrecAlign* alignParameters =  _map_DPrecAlign[planeIndex];
  DR3 translations = alignParameters->GetTranslationsDR3();
  DR3 rotations    = alignParameters->GetRotationsDR3();
  //Double_t* rotMat = alignParameters->GetRotationMatrix();

  // Derivee par rapport a rotations(0) :
  // return -(((trackPoint(0)-translations(0))*sin(rotations(1))-(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))-(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1)))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)))+trackPoint(2)-translations(2))*(sin(rotations(2))*sin(rotations(0))+cos(rotations(2))*sin(rotations(1))*cos(rotations(0)))-(-(trackDirectionY*(-(trackPoint(0)-translations(0))*sin(rotations(1))+(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))+(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1))))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)))+trackPoint(1)-translations(1))*(sin(rotations(2))*sin(rotations(1))*cos(rotations(0))-cos(rotations(2))*sin(rotations(0)))-cos(rotations(1))*(-(trackDirectionX*(-(trackPoint(0)-translations(0))*sin(rotations(1))+(trackPoint(1)-translations(1))*sin(rotations(2))*cos(rotations(1))+(trackPoint(2)-translations(2))*cos(rotations(2))*cos(rotations(1))))/(-trackDirectionX*sin(rotations(1))+trackDirectionY*sin(rotations(2))*cos(rotations(1))+cos(rotations(2))*cos(rotations(1)))+trackPoint(0)-translations(0))*cos(rotations(0));

  //return (translations(0) - trackPoint(0) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionY/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))))*cos(rotations(1))*cos(rotations(0)) - (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*(translations(1) - trackPoint(1) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1)))) - (cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*(translations(2) - trackPoint(2) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionX*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionY*sin(rotations(1))));  

  Double_t newDerivative = -(translations(0) - trackPoint(0) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionX/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))))*cos(rotations(1))*cos(rotations(0)) - (cos(rotations(0))*sin(rotations(2))*sin(rotations(1)) - cos(rotations(2))*sin(rotations(0)))*(translations(1) - trackPoint(1) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))*trackDirectionY/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1)))) - (cos(rotations(2))*cos(rotations(0))*sin(rotations(1)) + sin(rotations(2))*sin(rotations(0)))*(translations(2) - trackPoint(2) - ((translations(2) - trackPoint(2))*cos(rotations(2))*cos(rotations(1)) + (translations(1) - trackPoint(1))*cos(rotations(1))*sin(rotations(2)) - (translations(0) - trackPoint(0))*sin(rotations(1)))/(trackDirectionY*cos(rotations(1))*sin(rotations(2)) + cos(rotations(2))*cos(rotations(1)) - trackDirectionX*sin(rotations(1))));

  return -newDerivative;

}

void MimosaGlobalAlign::FixParameter(Int_t planeIndex, Int_t parameterIndex, Double_t sigma)
{
  // If sigma --> +inf the parameter is free.
  // If sigma -->  the parameter is constrainst with resolution +/- sigma

  (*_covarianceMatrixAlignementConstrainsts)[(planeIndex-1)*_degreeOfFreedom+parameterIndex][(planeIndex-1)*_degreeOfFreedom+parameterIndex] = sigma*sigma;
  
  std::cout<<"Parameter "<<_mapParameterName[parameterIndex]<<" of plane number "<<planeIndex<<" fixed with resolution "<<sqrt((*_covarianceMatrixAlignementConstrainsts)[(planeIndex-1)*_degreeOfFreedom+parameterIndex][(planeIndex-1)*_degreeOfFreedom+parameterIndex])<<std::endl;

  //(*_vectorAlignmentConstraints).Print();
  //(*_covarianceMatrixAlignementConstrainsts).Print();

}

void MimosaGlobalAlign::FixPlane(Int_t planeIndex, Double_t sigma)
{
  // If sigma --> +inf the parameter is free.
  // If sigma -->  the parameter is constrainst with resolution +/- sigma

  for(Int_t j=0 ; j<6 ; ++j) (*_covarianceMatrixAlignementConstrainsts)[(planeIndex-1)*_degreeOfFreedom+j][(planeIndex-1)*_degreeOfFreedom+j] = sigma*sigma;
 
  std::cout<<"Plane number "<<planeIndex<<" fixed with resolution "<<sqrt((*_covarianceMatrixAlignementConstrainsts)[(planeIndex-1)*_degreeOfFreedom][(planeIndex-1)*_degreeOfFreedom])<<std::endl;

}

void MimosaGlobalAlign::SetConstraintValueOnParameter(Int_t planeIndex, Int_t parameterIndex, Double_t value)
{

  (*_vectorAlignmentConstraints)[(planeIndex-1)*_degreeOfFreedom+parameterIndex] = (*_alignmentVector)[(planeIndex-1)*_degreeOfFreedom+parameterIndex] - value;
  
  std::cout<<"Parameter "<<_mapParameterName[parameterIndex]<<" of plane number "<<planeIndex<<" set to value "<<value<<"("<<(*_alignmentVector)[(planeIndex-1)*_degreeOfFreedom+parameterIndex]<<")"<<" with resolution "<<sqrt((*_covarianceMatrixAlignementConstrainsts)[(planeIndex-1)*_degreeOfFreedom+parameterIndex][(planeIndex-1)*_degreeOfFreedom+parameterIndex])<<std::endl;
  
  //(*_vectorAlignmentConstraints).Print();
  //(*_covarianceMatrixAlignementConstrainsts).Print();
}

void MimosaGlobalAlign::SetInitialAlignementParameters(std::map<const Int_t, DPlane*>& planeList, const Double_t& ResoAlignParamTr, const Double_t& ResoAlignParamRot)
{

  SetCovMatrixAlignConstraintToIdentity();
  _hitsPerPlane.clear();

  // Filling the initial alignment paramters.
  std::map<const Int_t, DPlane*>::iterator it = planeList.begin();
  for( Int_t i=0 ; i<_planeNumber ; ++i) {

    DPlane* aPlane = it->second;
    DPrecAlign* alignParameters = aPlane->GetPrecAlignment();

    _map_DPrecAlign[i] = alignParameters;
    _map_Planes[i] = aPlane;
    _map_PlanesIndex[aPlane->GetPlaneNumber()] = i+1;
    
    std::cout<<" Pl"<<aPlane->GetPlaneNumber()<<" Mapped to Pl"<<i+1<<std::endl;
    
    _hitsPerPlane.push_back(0);

    DR3 translationPlane = alignParameters->GetTranslationsDR3();
    DR3 rotationsPlane   = alignParameters->GetRotationsDR3();

    translationPlane.Print();
    rotationsPlane.Print();

    // Fill initial alignment parameters :    
    for(Int_t indexTr=0 ; indexTr<_degreeOfFreedom/2. ; ++indexTr) (*_alignmentVector)( i*_degreeOfFreedom+indexTr ) = translationPlane(indexTr);
    for(Int_t indexRot=0 ; indexRot<_degreeOfFreedom/2. ; ++indexRot) (*_alignmentVector)( (i+1)*_degreeOfFreedom-(indexRot+1) ) = rotationsPlane(indexRot); 

    for(Int_t paramIndex=0 ; paramIndex<_degreeOfFreedom ; ++paramIndex) {
      
      if(paramIndex<_degreeOfFreedom/2) {                           // TRANSLATION
        SetConstraintValueOnParameter(i+1, paramIndex, translationPlane(paramIndex) );
        FixParameter(i+1, paramIndex, ResoAlignParamTr);
      }  
      else if(paramIndex>=_degreeOfFreedom/2 && paramIndex<_degreeOfFreedom) {     // ROTATIONS
        SetConstraintValueOnParameter(i+1, paramIndex, rotationsPlane( _degreeOfFreedom - (paramIndex+1) ) ); // BUG ICI :)
        FixParameter(i+1, paramIndex, ResoAlignParamRot);     
      }
    
    }
    
    for(Int_t j=0 ; j<_degreeOfFreedom ; ++j) (*_identityMatrix)[ i*_degreeOfFreedom+j][ i*_degreeOfFreedom+j] = 1.;
    
    ++it;

  }
  
  SetDisplay();

  _alignmentVector->Print();
  //(*_vectorAlignmentConstraints).Print();
  //(*_covarianceMatrixAlignementConstrainsts).Print();

}

void MimosaGlobalAlign::SetDisplay() {

  for(Int_t planeIndex=0 ; planeIndex<_planeNumber ; planeIndex++) {

    TString name = "canvasResiduals_Pl";
    name += _map_Planes[planeIndex]->GetPlaneNumber();

    canvasResiduals[planeIndex] = new TCanvas(name.Data(),"Residuals",410,10,750,500);
    canvasResiduals[planeIndex]->Divide(2);
    canvasResiduals[planeIndex]->Update();

    TString nameDU = "du_pl";
    nameDU += _map_Planes[planeIndex]->GetPlaneNumber();

    TString nameDV = "dv_pl";
    nameDV += _map_Planes[planeIndex]->GetPlaneNumber();
    
    residualsU[planeIndex] = new TH1F(nameDU.Data(),"Delta U", 200, -2.*_bound, +2.*_bound); 
    residualsV[planeIndex] = new TH1F(nameDV.Data(),"Delta V", 200, -2.*_bound, +2.*_bound); 

  }
 
  canvasResiduals[_planeNumber] = new TCanvas("Track Parameters","Residuals",410,10,750,500);
  canvasResiduals[_planeNumber]->Divide(2);
  canvasResiduals[_planeNumber]->Update();
  
  trackParameterX = new TH1F("Track Parameter X","Track Parameter X", 200, -0.01, +0.01);
  trackParameterY = new TH1F("Track Parameter Y","Track Parameter Y", 200, -0.01, +0.01);

}

void MimosaGlobalAlign::SetTrackParameters(DR3 trackDirection, DR3 trackOrigin)
{
  for(Int_t i=0; i<_trackParametersNumber ; ++i) { 

    if(i<2) (*_trackParameters)[i] = trackDirection(i);
    else if(i>=2) (*_trackParameters)[i] = trackOrigin(i-2);
  }

}

void MimosaGlobalAlign::FixTrackParameters(Double_t directionX, Double_t directionY, Double_t sigmaX, Double_t sigmaY)
{
  for(Int_t i=0 ; i<2 ; ++i) {
    if(i==0) {
      (*_covarianceMatrixTrackConstraints)[i][i] = sigmaX*sigmaX;
      (*_vectorTrackConstraints)[i] = (*_trackParameters)[i] - directionX;
    }
    else if(i==1) {
     (*_covarianceMatrixTrackConstraints)[i][i] = sigmaY*sigmaY;
     (*_vectorTrackConstraints)[i] =  (*_trackParameters)[i] - directionY;
    }
  }
}


void MimosaGlobalAlign::FixTrackOrigin( DR3 fixedTrackOrigin, DR3 sigmaOrigin)
{

  for(Int_t i=2 ; i<_trackParametersNumber ; ++i) {
    
    (*_covarianceMatrixTrackConstraints)[i][i] = sigmaOrigin(i-2)*sigmaOrigin(i-2);
    (*_vectorTrackConstraints)[i] = (*_trackParameters)[i] - fixedTrackOrigin(i-2);
    //std::cout<<(*_trackParameters)[i] - fixedTrackOrigin(i-2)<<std::endl;
  }

}


void MimosaGlobalAlign::SetCovarianceMatrixToIdentity()
{

  for(Int_t i=0 ; i<_planeNumber*_axisNumber ; ++i) {
    (*_covarianceMatrix)[i][i] = 1.; 
    (*_identity)[i][i] = 1.;
  }
}

void MimosaGlobalAlign::SetCovarianceMatrixMS()
{

  // Var(U_k) = sigma_k^2 + Sum{ (x_k-x_j)^2 theta_0^2 } sum on j<k
  // Cov(U_k,U_l) = Sum{ (x_k-x_j)(x_l-x_j) theta_0^2  } sum on j<min(k,l)
 
  // --> Stocker les infos dans une map ? ou un vector ? std::map<Int_t, std::vector<Double_t> > _planeInformations;
  // --> 3 vector : std::vector<Double_t> _thetaMS std::vector<Double_t> trackExtrapolation
 
  /* 
  // Variances :
  for(Int_t i=1 ; i<(_planeNumber) ; ++i) {
    
    for(Int_t k=0 ; k<i ; ++k) {
      
      _covarianceMatrixMS[i][i] = pow( (hitPositions[i]-trackExtrapolation[k]) * _thetaMS[k] , 2);
    }
  }
  */

  // Covariances :
  for(Int_t i=0 ; i<_planeNumber*_axisNumber ; i+=2) {

    for(Int_t j=0 ; j<_planeNumber*_axisNumber ; j+=2) {
      
      for(Int_t k=0 ; k<_planeNumber*_axisNumber ; k+=2) { 
  
        if( k<std::min(i,j) ) (*_covarianceMatrixMS)[i][j] += ComputeDistance(_trackExtrapolation[i/2], _trackExtrapolation[k/2]) * ComputeDistance(_trackExtrapolation[j/2], _trackExtrapolation[k/2]) * _thetaMS[k/2] * _thetaMS[k/2];
        //std::cout<<ComputeDistance(_trackExtrapolation[i/2], _trackExtrapolation[k/2]) <<std::endl;
      }

      Double_t covValue = (*_covarianceMatrixMS)[i][j];
      
      if(_debugLevel>0) std::cout<<"i/j = "<<i<<" "<<j<<" cov[i][j] = "<<covValue<<std::endl;

      (*_covarianceMatrixMS)[i+1][j+1] = covValue; 
      //(*_covarianceMatrixMS)[j][i]     += (*_covarianceMatrix)[i][j];
      //(*_covarianceMatrixMS)[j+1][i+1] += (*_covarianceMatrix)[i][j];

    }
  }

  if(_debugLevel>0) _covarianceMatrixMS->Print();
  
  *_covarianceMatrix += *_covarianceMatrixMS; 

}

void MimosaGlobalAlign::ComputeResidualsMS()
{
  // _planeNumber*_axisNumber = dimension

  for(Int_t i=0 ; i<_planeNumber*_axisNumber ; i+=2) {

    for(Int_t k=0 ; k<i ; k+=2) (*_residualsVectorMS)[i] += ComputeDistance(_trackExtrapolation[i/2], _trackExtrapolation[k/2]) * _thetaMS[k/2];

    Double_t resValue = (*_residualsVectorMS)[i];
    (*_residualsVectorMS)[i+1] = resValue;
  }

/*  if(_debugLevel>0)*/ _residualsVectorMS->Print();

  *_residualsVector += *_residualsVectorMS;
}

Double_t MimosaGlobalAlign::ComputeDistance(DR3 firstPlane, DR3 lastPlane)
{
 
 return sqrt( pow( (lastPlane(0)-firstPlane(0)), 2) +   pow( (lastPlane(1)-firstPlane(1)), 2) +  pow( (lastPlane(2)-firstPlane(2)), 2) );
   
}

void MimosaGlobalAlign::SetCovMatrixAlignConstraintToIdentity()
{

  for(Int_t i=0 ; i<_planeNumber*_degreeOfFreedom ; ++i) (*_covarianceMatrixAlignementConstrainsts)[i][i] = 1.;

}

void MimosaGlobalAlign::SetIdentityForTrackConstraint()
{
  for(Int_t i=0 ; i<_trackParametersNumber ; ++i) (*_identidyMatrixTrack)[i][i] = 1.;
}

void MimosaGlobalAlign::ProcessTrack(DTrack* aTrack)
{

  // Clear old matrices :
  _residualsVector->Zero();
  _residualsVectorMS->Zero();
  _derivativesTracksMatrix->Zero();
  _derivativesAlignmentMatrix->Zero();
  _covarianceMatrix->Zero();
  _covarianceMatrixMS->Zero();
  _inverseCovarianceMatrix->Zero();
  _covarianceMatrixTrackConstraints->Zero();
  _inverseCovMatTrackConstraints->Zero();
  _identidyMatrixTrack->Zero();
  _vectorTrackConstraints->Zero();
  _saveDerivativesTracksMatrix->Zero();
  _saveTransposeDerivativesTracksMatrix->Zero();
  _saveDerivativesAlignmentMatrix->Zero();
  _saveTransposeDerivativesAlignmentMatrix->Zero(); 

  _trackExtrapolation.clear();
  _thetaMS.clear();

  SetCovarianceMatrixToIdentity();
  SetIdentityForTrackConstraint();
  
  // Get track parameters : 
  DR3 trackOrigin = aTrack->GetLinearFit().GetOrigin();
  DR3 trackDirection = aTrack->GetLinearFit().GetSlopeZ();
  
  Int_t hitNumber = aTrack->GetHitsNumber();
  if(hitNumber==0) return;

  FillTrackParametersPlots( trackDirection );

  //std::cout<<"Hits in Track = "<<hitNumber<<std::endl;
 
  SetTrackParameters(trackDirection, trackOrigin);

  FixTrackParameters(_fixedTrackDirectionX, _fixedTrackDirectionY, _sigmaTrackDirectionX, _sigmaTrackDirectionY);  // Numbers set in MAlign::AlignTrackerGlobal
 
  // For Track Origin :
  _sigmaTrackOriginX = 1.*_sigmaTrackDirectionX*1e4;
  _sigmaTrackOriginY = 1.*_sigmaTrackDirectionY*1e4; 
  _sigmaTrackOriginZ = 0.;
  DR3 sigmaOrigin(_sigmaTrackOriginX, _sigmaTrackOriginY, _sigmaTrackOriginZ);
  FixTrackOrigin( trackOrigin, sigmaOrigin);
  
  // Get Hit List of the track :
  DHit** hitList = aTrack->GetHitArray();

  if(_multipleScatteringFit==true) {

    for(Int_t i=0 ; i<_planeNumber ; ++i) {
      _trackExtrapolation.push_back(DR3(0.,0.,0.));
      _thetaMS.push_back(0.);
    }

    for(Int_t iterHit=0 ; iterHit<hitNumber ; ++iterHit) {
 
      DPlane* myPlane  = hitList[iterHit]->GetPlane();
      Int_t planeIndex = _map_PlanesIndex[myPlane->GetPlaneNumber()];

      DR3 extrapolationPlaneFrame = myPlane->Intersection(aTrack);
      _trackExtrapolation[planeIndex-1] = myPlane->PlaneToTracker( extrapolationPlaneFrame );
      _thetaMS[planeIndex-1]            = myPlane->GetSigmaThetaMS();
      std::cout<<"plane Index = "<<planeIndex<<" MS angle theta = "<<myPlane->GetSigmaThetaMS()<<std::endl; 
   
    } // end for hits
  } // if MS fit

  // Hits Loop :
  for(Int_t iterHit=0 ; iterHit<hitNumber ; ++iterHit) {

    DR3 hitPosition = DR3( hitList[iterHit]->GetPositionUhitCG(), hitList[iterHit]->GetPositionVhitCG() , 0.);
    
    DPlane* aPlane = hitList[iterHit]->GetPlane();
    Int_t planeIndex = _map_PlanesIndex[aPlane->GetPlaneNumber()];
    //std::cout<<"Hit in Pl"<<aPlane->GetPlaneNumber()<<" plane mapped to index "<<planeIndex<<std::endl;

    _hitsPerPlane[planeIndex-1]++; // To count hits per plane. If 0 hit => singular matrix. So need to set readout mode = 0 for this plane.

    DR3 planeTrackIntersection = aPlane->Intersection(aTrack);
    
    if(_debugLevel>0) {
      // Compute Track-Plane intersection with DPrecAlign mecanism :
      std::cout<<"planeTrackIntersection Method DPrecAlign : "<<std::endl;
      planeTrackIntersection.Print();
    
      DR3 intersectionFullComputedByFullExpression(0.,0.,0.);
      intersectionFullComputedByFullExpression(0) = ComputeTrackIntersectionU(planeIndex-1, trackDirection(0), trackDirection(1), trackOrigin);
      intersectionFullComputedByFullExpression(1) = ComputeTrackIntersectionV(planeIndex-1, trackDirection(0), trackDirection(1), trackOrigin); 
    
      // Compute Track-Plane intersection with full expression : 
      std::cout<<"planeTrackIntersection Method By Full Expression : "<<std::endl;
      intersectionFullComputedByFullExpression.Print();
    }

    DR3 Residuals = hitPosition-planeTrackIntersection;

    if(_debugLevel>0) {
      std::cout<<"Residuals = "<<std::endl;
      Residuals.Print();
    }

    Int_t resolutionU = hitList[iterHit]->GetResolutionUhit();
    Int_t resolutionV = hitList[iterHit]->GetResolutionVhit();

    //std::cout<<"ResolutionU = "<<resolutionU<<"  ResolutionV = "<<resolutionV<<std::endl;

    _indexResiduals = (planeIndex-1)*_axisNumber;
    _indexAlignmentParams = (planeIndex-1)*_degreeOfFreedom;
    // Filling Temp matrices ...

    (*_residualsVector)(_indexResiduals) = Residuals(0);
    (*_derivativesTracksMatrix)[_indexResiduals][0] = ComputeResidualDerivative_U_AboutTrackDirectionX(planeIndex-1, trackDirection(0), trackDirection(1), trackOrigin);
    (*_derivativesTracksMatrix)[_indexResiduals][1] = ComputeResidualDerivative_U_AboutTrackDirectionY(planeIndex-1, trackDirection(0), trackDirection(1), trackOrigin);

    // For track origin point :
    (*_derivativesTracksMatrix)[_indexResiduals][2] = ComputeResidualDerivative_U_AboutTrackPointX(planeIndex-1, trackDirection(0), trackDirection(1) );
    (*_derivativesTracksMatrix)[_indexResiduals][3] = ComputeResidualDerivative_U_AboutTrackPointY(planeIndex-1, trackDirection(0), trackDirection(1) );
    //(*_derivativesTracksMatrix)[_indexResiduals][4] = ComputeResidualDerivative_U_AboutTrackPointZ(planeIndex-1, trackDirection(0), trackDirection(1) ); // trackOrigin in Z=0 by default.
    
    //(*_derivativesTracksMatrix)[_indexResiduals][2] = 0.;
    
    (*_derivativesAlignmentMatrix)[_indexResiduals][_indexAlignmentParams+0] = ComputeResidualDerivative_U_AboutTranslationX(planeIndex-1, trackDirection(0), trackDirection(1) ); 
    (*_derivativesAlignmentMatrix)[_indexResiduals][_indexAlignmentParams+1] = ComputeResidualDerivative_U_AboutTranslationY(planeIndex-1, trackDirection(0), trackDirection(1) );
    (*_derivativesAlignmentMatrix)[_indexResiduals][_indexAlignmentParams+2] = ComputeResidualDerivative_U_AboutTranslationZ(planeIndex-1, trackDirection(0), trackDirection(1) );  
    (*_derivativesAlignmentMatrix)[_indexResiduals][_indexAlignmentParams+3] = ComputeResidualDerivative_U_AboutRotationX(planeIndex-1, trackDirection(0), trackDirection(1), trackOrigin); 
    (*_derivativesAlignmentMatrix)[_indexResiduals][_indexAlignmentParams+4] = ComputeResidualDerivative_U_AboutRotationY(planeIndex-1, trackDirection(0), trackDirection(1), trackOrigin);
    (*_derivativesAlignmentMatrix)[_indexResiduals][_indexAlignmentParams+5] = ComputeResidualDerivative_U_AboutRotationZ(planeIndex-1, trackDirection(0), trackDirection(1), trackOrigin);
    
    //_derivativesTracksMatrix->Print();
    (*_covarianceMatrix)[_indexResiduals][_indexResiduals] = resolutionU*resolutionU;      // Filling covariance matrix (planeIndex-1, axis U) 

    _indexResiduals++;
    
    (*_residualsVector)[_indexResiduals] = Residuals(1);                                  // Fillinf Residual vector (planeIndex-1, axis V)
    (*_derivativesTracksMatrix)[_indexResiduals][0] = ComputeResidualDerivative_V_AboutTrackDirectionX(planeIndex-1, trackDirection(0), trackDirection(1), trackOrigin);
    (*_derivativesTracksMatrix)[_indexResiduals][1] = ComputeResidualDerivative_V_AboutTrackDirectionY(planeIndex-1, trackDirection(0), trackDirection(1), trackOrigin);

    // Fot track origin point : not necessary
    (*_derivativesTracksMatrix)[_indexResiduals][2] = ComputeResidualDerivative_V_AboutTrackPointX(planeIndex-1, trackDirection(0), trackDirection(1) );
    (*_derivativesTracksMatrix)[_indexResiduals][3] = ComputeResidualDerivative_V_AboutTrackPointY(planeIndex-1, trackDirection(0), trackDirection(1) );
    //(*_derivativesTracksMatrix)[_indexResiduals][4] = ComputeResidualDerivative_V_AboutTrackPointZ(planeIndex-1, trackDirection(0), trackDirection(1) );
    
    //(*_derivativesTracksMatrix)[_indexResiduals][2] = 0;
    
    (*_derivativesAlignmentMatrix)[_indexResiduals][_indexAlignmentParams+0] = ComputeResidualDerivative_V_AboutTranslationX(planeIndex-1, trackDirection(0), trackDirection(1) ); 
    (*_derivativesAlignmentMatrix)[_indexResiduals][_indexAlignmentParams+1] = ComputeResidualDerivative_V_AboutTranslationY(planeIndex-1, trackDirection(0), trackDirection(1) );
    (*_derivativesAlignmentMatrix)[_indexResiduals][_indexAlignmentParams+2] = ComputeResidualDerivative_V_AboutTranslationZ(planeIndex-1, trackDirection(0), trackDirection(1) );
    (*_derivativesAlignmentMatrix)[_indexResiduals][_indexAlignmentParams+3] = ComputeResidualDerivative_V_AboutRotationX(planeIndex-1, trackDirection(0), trackDirection(1), trackOrigin); 
    (*_derivativesAlignmentMatrix)[_indexResiduals][_indexAlignmentParams+4] = ComputeResidualDerivative_V_AboutRotationY(planeIndex-1, trackDirection(0), trackDirection(1), trackOrigin);
    (*_derivativesAlignmentMatrix)[_indexResiduals][_indexAlignmentParams+5] = ComputeResidualDerivative_V_AboutRotationZ(planeIndex-1, trackDirection(0), trackDirection(1), trackOrigin);
    
    (*_covarianceMatrix)[_indexResiduals][_indexResiduals] = resolutionV*resolutionV;      // Filling covariance Matrix (planeIndex-1, axis V)

  } // end loop on hits

  FillResidualsPlots();

  if(_multipleScatteringFit==true) {
    SetCovarianceMatrixMS(); 
    ComputeResidualsMS();
    //_covarianceMatrix->Print(); 
  }
   
  //_residualsVector->Print();
  //_derivativesTracksMatrix->Print();
  //_derivativesAlignmentMatrix->Print();
  //_covarianceMatrix->Print();

  FillMatrixB();
  FillFinalGlobalMatrix();
  FillGlobalFinalVector();

  _matrixB->Clear();
  _matrixI->Clear();
  _matrixG->Clear();
  delete _matrixB;
  delete _matrixI;
  delete _matrixG;

}

void MimosaGlobalAlign::FillResidualsPlots()
{

  for(Int_t planeIndex=0 ; planeIndex<_planeNumber ; ++planeIndex) {
  
     Int_t indexU = (planeIndex)*_axisNumber;
     Int_t indexV = (planeIndex)*_axisNumber + 1; 
 
     if(_debugLevel>0) {
       std::cout<<" Index U/V = ("<<indexU<<","<<indexV<<")"<<std::endl;
       std::cout<<" Residuals Values U/V = ("<<Double_t( (*_residualsVector)[indexU])<<","<<Double_t( (*_residualsVector)[indexV])<<")"<<std::endl;
     }

     residualsU[planeIndex]->Fill( Double_t( (*_residualsVector)[indexU]) );
     residualsV[planeIndex]->Fill( Double_t( (*_residualsVector)[indexV]) );
 
  }

}

void MimosaGlobalAlign::FillTrackParametersPlots( DR3 trackParameters )
{

  trackParameterX->Fill( trackParameters(0) );
  trackParameterY->Fill( trackParameters(1) ); 

}

void MimosaGlobalAlign::PrintResiduals()
{

  for(Int_t planeIndex=0 ; planeIndex<_planeNumber ; ++planeIndex) {
    
    canvasResiduals[planeIndex]->cd();
    canvasResiduals[planeIndex]->cd(1);
    residualsU[planeIndex]->Draw();
    residualsU[planeIndex]->Fit("gaus");
    canvasResiduals[planeIndex]->cd(2);
    residualsV[planeIndex]->Draw();
    residualsV[planeIndex]->Fit("gaus");

    canvasResiduals[planeIndex]->Modified();
    canvasResiduals[planeIndex]->Update();
  }

  canvasResiduals[_planeNumber]->cd();
  canvasResiduals[_planeNumber]->cd(1);
  trackParameterX->Draw();
  trackParameterX->Fit("gaus");
  canvasResiduals[_planeNumber]->cd(2);
  trackParameterY->Draw();
  trackParameterY->Fit("gaus");
  canvasResiduals[_planeNumber]->Modified();
  canvasResiduals[_planeNumber]->Update();

}

void MimosaGlobalAlign::FillMatrixB()
{

  // Filling B matrix for one track :
  // MatrixG contain the term to fix track parameter.
  //
  // We add a new chi2 defined for each track to fix track parameter with a certain value and a certain resolution.
  //
  // We introduce a new residual : R(pi) = pi - pi(fixed) and a new cov matrix W (diag if no correlation between track parameters)
  // the chi2 is : R(pi)T . W-1 . R(pi)
  //
  // After some algebra we have a new definition for final matrix and final vector.
  //
  // Here is the definition of the solution for each track :
  // (dR(a,pi)/da)T . matrixB . dR(a,pi)/da
  //
  // A = dR(a,pi)/dpi
  // F = dR(pi)/dpi (derivative of the new residual with track parameters) = Id if no correlation between track parameters.
  // V = cov matrix of residuals for track fitting
  //
  // matrixB = V-1 - V-1 . A . (A^T . V-1 . A + F^T . W-1 . F)^-1 . A^T . V^-1


  _matrixB = new TMatrixD(_planeNumber*_axisNumber, _planeNumber*_axisNumber);
  _matrixI = new TMatrixD(_trackParametersNumber, _trackParametersNumber);
  _matrixG = new TMatrixD(_trackParametersNumber, _planeNumber*_axisNumber); 
   
  *_inverseCovarianceMatrix = _covarianceMatrix->Invert();

  TMatrixD transposeResTracksConstraintDerivatices = _identidyMatrixTrack->T();
  _identidyMatrixTrack->T(); // return to not transpose matrix. But for identify, it's the same. 
 
  
  if(_trackParametersConstrainsts==true) {
    //_covarianceMatrixTrackConstraints->Print();
    *_inverseCovMatTrackConstraints = _covarianceMatrixTrackConstraints->Invert();
  }

  *_saveDerivativesTracksMatrix = *_derivativesTracksMatrix; 
  *_saveTransposeDerivativesTracksMatrix = _derivativesTracksMatrix->T();
  _derivativesTracksMatrix->T(); // restaure the matrix (AT)T = A
 
  // Print Matrices To check the operations :
  if(_debugLevel>0) {

    std::cout<<"A = "<<std::endl;
    _saveDerivativesTracksMatrix->Print();

    std::cout<<"A^T = "<<std::endl;
    _saveTransposeDerivativesTracksMatrix->Print();

    std::cout<<"V^-1 = "<<std::endl;
    _inverseCovarianceMatrix->Print();

    if(_trackParametersConstrainsts==true) {
      
      std::cout<<"F = Identity = "<<std::endl;
      _identidyMatrixTrack->Print();

      std::cout<<"F^T = "<<std::endl;
      transposeResTracksConstraintDerivatices.Print();

      std::cout<<"V_{pi}^-{1} = inverse cov matrix for track parameter resolution ="<<std::endl;
      _inverseCovMatTrackConstraints->Print();
    } // end if track constrainsts

  } // en dif debug

  //TMatrixD tempMatrix(_trackParametersNumber, _trackParametersNumber);

  //std::cout<<"test-2 "<<_trackParametersConstrainsts<<std::endl;
   
  // Definition with track constrainst or not
  if(_trackParametersConstrainsts==true) {
    *_matrixI = *_saveTransposeDerivativesTracksMatrix * *_inverseCovarianceMatrix * *_saveDerivativesTracksMatrix + transposeResTracksConstraintDerivatices * *_inverseCovMatTrackConstraints * *_identidyMatrixTrack;
  }
  else if(_trackParametersConstrainsts==false) {
    *_matrixI = *_saveTransposeDerivativesTracksMatrix * *_inverseCovarianceMatrix * *_saveDerivativesTracksMatrix; /*+ transposeResTracksConstraintDerivatices * *_inverseCovMatTrackConstraints * *_identidyMatrixTrack;*/
  }
  
  //std::cout<<"test-1 "<<_trackParametersConstrainsts<<std::endl;

  // Remember : (AT)T = A
  
  if(_debugLevel>0) {

    std::cout<<"TempMatrix = "<<std::endl;
    _matrixI->Print();

  }
  
  // Inverse : 
  //_matrixI->Invert();

  TDecompLU lu(*_matrixI);
  lu.SetTol(1e-50);
  lu.Invert(*_matrixI);

  if(_debugLevel>0) {
  
    std::cout<<"Inverse TempMatrix = "<<std::endl;
    _matrixI->Print();
  }

  *_matrixG = *_matrixI * *_saveTransposeDerivativesTracksMatrix * *_inverseCovarianceMatrix;

  if(_debugLevel>0) {

    std::cout<<"G = "<<std::endl;
    _matrixG->Print();
  }

  if(_trackParametersConstrainsts==true) { 
    *_matrixB = ( *_identity - *_saveDerivativesTracksMatrix * *_matrixG ).T() * *_inverseCovarianceMatrix; 
  }
  else if(_trackParametersConstrainsts==false) {
    *_matrixB = *_inverseCovarianceMatrix - *_inverseCovarianceMatrix * *_saveDerivativesTracksMatrix * *_matrixG;  
  }
      
  if(_debugLevel>0) {
  
    std::cout<<"Identity = "<<std::endl;
    _identity->Print();

    std::cout<<"A = "<<std::endl;
    _saveDerivativesTracksMatrix->Print();

    std::cout<<"V-1 = "<<std::endl;
    _inverseCovarianceMatrix->Print();

    std::cout<<"Matrix B = "<<std::endl;
    _matrixB->Print();
  }

}

void MimosaGlobalAlign::FillFinalGlobalMatrix()
{

// This Final Matrix (M) is the inverse of the final covariance matrix.
// It means M->invert() is the finale covariance matrix.
// So the error on correction parameters are just the sqrt of the diagonal elements.

// This method fill the final matrix for each track.
// A term to fix track parameter with a certain value and a certain resolution is included in matrixB.

  *_saveDerivativesAlignmentMatrix = *_derivativesAlignmentMatrix;
  *_saveTransposeDerivativesAlignmentMatrix = _derivativesAlignmentMatrix->T();
  _derivativesAlignmentMatrix->T(); // restaure the matrix (AT)T=A 

  if(_debugLevel>0) {
    
    std::cout<<"Matrix Derivatives Alignment = "<<std::endl;
    _saveDerivativesAlignmentMatrix->Print();

    std::cout<<"Transpose Matrix Derivatives Alignment = "<<std::endl;
    _saveTransposeDerivativesAlignmentMatrix->Print();

    std::cout<<"Matrix Derivatives Alignment = "<<std::endl;
    _derivativesAlignmentMatrix->Print();
  }

  *_finalMatrix += *_saveTransposeDerivativesAlignmentMatrix * *_matrixB * *_saveDerivativesAlignmentMatrix;

  if(_debugLevel>0) {

    std::cout<<"final Matrix = "<<std::endl;
    _finalMatrix->Print();
  }

}

void MimosaGlobalAlign::FillGlobalFinalVector()
{

  // This method fill the final vector for each track.
  // We add a second term to fix the track parameters to a certain value with a certain resolution.
  //
  // This term is equal to : 
  // - (dR(a,pi)/da)T . (F*matrixG)T * W-1 * R(pi0)
  // W-1 --> new cov mat fot track constraint R(pi) --> new residual for track constraint.
 
  if(_debugLevel>0) {

    std::cout<<"Transpose Derivative Alignment Matrix = "<<std::endl;
    _saveTransposeDerivativesAlignmentMatrix->Print();

    std::cout<<"Matrix B = "<<std::endl;
    _matrixB->Print();

    std::cout<<"Residual Vector = "<<std::endl;
    _residualsVector->Print();

    std::cout<<"Inv Cov Matrix for Track Constrainsts Resolution = "<<std::endl;
    _inverseCovMatTrackConstraints->Print();

    std::cout<<"Vector Initial Track Parameters for track Constrainsts = "<<std::endl;
    _vectorTrackConstraints->Print();

  }
 
  //std::cout<<"test0 "<<_trackParametersConstrainsts<<std::endl;
   
  if(_trackParametersConstrainsts==true) { 
    *_finalVector += *_saveTransposeDerivativesAlignmentMatrix * *_matrixB * *_residualsVector;
    *_finalVector -= *_saveTransposeDerivativesAlignmentMatrix * (*_identidyMatrixTrack * *_matrixG).T() * *_inverseCovMatTrackConstraints * *_vectorTrackConstraints;
  }
  else if(_trackParametersConstrainsts==false) {
    *_finalVector +=  *_saveTransposeDerivativesAlignmentMatrix * *_matrixB * *_residualsVector; /*- ( *_saveTransposeDerivativesAlignmentMatrix * _matrixG->T() * _identidyMatrixTrack->T() ) * *_inverseCovMatTrackConstraints * *_vectorTrackConstraints;*/
  }
  
  //std::cout<<"test1 "<<_trackParametersConstrainsts<<std::endl;


  //_derivativesAlignmentMatrix->T(); // Restaure the good matrix : (AT)T = A

  if(_debugLevel>0) {
    
     std::cout<<"Matrix G Transposed = "<<std::endl;
    _matrixG->Print();

    std::cout<<"Identity Matrix for Track constrainsts = "<<std::endl;
    _identidyMatrixTrack->Print();
    
    std::cout<<"finel Vector = "<<std::endl;
    _finalVector->Print();
  }

}

void MimosaGlobalAlign::InvertFinalMatrix()
{

  // Final matrix invertion. Inverse of the final matrix = global covariance matrix.
  // So, this method compute the final covariance matrix.

  // In this method we add alignment parameters constraints.
  // A new chi2 is added = R(a)T . W-1 . R(a)
  // With :
  //  R(a) = a - a(fixed value)
  //  W = covariance matrix of R(a) = resolution on the fixed value
  //  If no correletion between parameters : W is diagonal and dR(a)/da = Identity
  // 
  // After some algebra we have to add the term :
  // (dR(a)/da)T . W-1 . dR(a)/da with a=a0 To the final matrix
  // And the term :
  // dR(a)/da . W-1 . R(a) with a=a0 To the final vector ( see ComputeAlignmentCorrections() method )

  *_inverseCovMatAlignConstraints = _covarianceMatrixAlignementConstrainsts->Invert();

  if(_debugLevel>0) {
    std::cout<<"Inv Cov Matrix for alignement parameters constraints (=resolution) = "<<std::endl;
    _inverseCovMatAlignConstraints->Print();
  }

 // In the case where dRa/da = (*_identityMatrix) (No correletion between alignment parameters) 
  if(_alignParametersConstrainsts==true) {
     
     //(*_identityMatrix).Print();

     *_finalMatrix += _identityMatrix->T() * *_inverseCovMatAlignConstraints * (*_identityMatrix);
     
     //std::cout<<"Inversion withn align params constraints"<<std::endl;
     //(*_covarianceMatrixAlignementConstrainsts).Print();
     //( _identityMatrix->T() * *_inverseCovMatAlignConstraints * (*_identityMatrix) ).Print();
  }

  if(_debugLevel>0) {
    std::cout<<"Identity Matrix Transposed = "<<std::endl;
    _identityMatrix->Print();

    std::cout<<"Final Matrix M = "<<std::endl;
    _finalMatrix->Print();
  }
  
  _identityMatrix->T(); // return to identity matrix.  

  // Final matrix inversion :)
  *_saveFinalMatrix = *_finalMatrix;  // To save the final matrix M. The problem is after _finalMatrix->Invert(), _finalMatrix is the invert, so here we save the finalMatrix.
  (*_saveFinalMatrix).Print();
  
  *_finalInverseMatrix = _finalMatrix->Invert();


  TDecompLU lu(*_saveFinalMatrix);
  lu.SetTol(1e-50);
  lu.Invert(*_finalInverseMatrix);

  if(_debugLevel>0) {
    std::cout<<"Final Inverse Matrix = "<<std::endl;
    _finalInverseMatrix->Print();
  }

}

void MimosaGlobalAlign::DecomposeSVD_FinalMatrix() {

  // SVD decomposition for weak modes spectral analisys
  // final matrix M = U.D.V' with D a diagonal matrix (decomposition in eigen vectors and eigen values)
  //
  // We have : delta a = M-1 * C
  //
  // In our case in our new basis : 
  // delta a = - U * D-1 * U-1            ( V=UT=U-1 orthogonal matrix + M symetric --> real eigen value)  
  //
  // def : delta b = - D-1 * U * C  (new def for cuts ...)
  // 
  // D = diag matrix = singular values ( we define lambda_i = D[i][i] )
  //
  // If lambda_i --> 0 ==> 1/lambda_i --> inf ==> error --> inf ==> weak mode
  // If lambda_i --> inf ==> 1/lambda_i --> 0 ==> no errors = very well constraint mode
  //
  // So we can found the weaks modes and cut them.
  // In this case we can reduce the matrix M cutting the weakest modes and inverse the matrix
  // So we solve the weaks modes problem :)

  TDecompSVD* SVD = new TDecompSVD(*_saveFinalMatrix);

  SVD->Decompose();  // SVD decomposition :)
 
  *_matrixU = SVD->GetU();
  *_matrixV = SVD->GetV();
  *_singularValues = SVD->GetSig();

  for(Int_t i=0 ; i<_planeNumber*_degreeOfFreedom ; ++i ) (*_singularMatrix)[i][i] = (*_singularValues)[i];

  _singularMatrix->Invert();


  if(_debugLevel>0) {
    std::cout<<"Inverse Singular Matrix (weak modes) :"<<std::endl;
    _singularMatrix->Print();
  }
  // TO BE UPDATED :)

}

void MimosaGlobalAlign::ComputeAlignmentCorrections(){

  if(_alignParametersConstrainsts==true) {
    *_alignmentCorrections -= *_finalInverseMatrix * (*_finalVector + _identityMatrix->T() * *_inverseCovMatAlignConstraints * *_vectorAlignmentConstraints);
  }
  else if(_alignParametersConstrainsts==false) {
    *_alignmentCorrections -= *_finalInverseMatrix * (*_finalVector /*+ _identityMatrix->T() * *_inverseCovMatAlignConstraints * *_vectorAlignmentConstraints*/); 
  }

}

void MimosaGlobalAlign::SetNewAlignmentParameters()
{

 TVectorD newAlignmentParameters = (*_alignmentVector+*_alignmentCorrections);

 for(Int_t iterPlane=0 ; iterPlane<_planeNumber ; ++iterPlane) {

   DR3 translations(0., 0., 0.);
   DR3 rotations( 0., 0., 0.);

   for(Int_t iter=0 ; iter<_degreeOfFreedom/2 ; ++iter) { 

     //std::cout<<"plane Index = "<<iterPlane<<" param "<<iter<<" "<<newAlignmentParameters[iterPlane*_degreeOfFreedom+iter]<<"  "<<newAlignmentParameters[iterPlane*_degreeOfFreedom+_degreeOfFreedom/2+iter]<<std::endl;
     translations(iter) = newAlignmentParameters[iterPlane*_degreeOfFreedom+iter];
     //rotations(iter)    = newAlignmentParameters[(iterPlane)*_degreeOfFreedom+_degreeOfFreedom/2+iter]; 
     rotations(iter)    = newAlignmentParameters[(iterPlane+1)*_degreeOfFreedom-(iter+1)]; 
   }
  
   //std::cout<<"DPrecAlignMethod = "<<_map_DPrecAlign[iterPlane]->GetDPrecAlignMethod()<<std::endl; 
   _map_DPrecAlign[iterPlane]->SetTranslation(translations);
   _map_DPrecAlign[iterPlane]->SetRotations(rotations);
   _map_DPrecAlign[iterPlane]->DecomposeRotations();
 } 

}

 void MimosaGlobalAlign::PrintAlignmentCorrections()
{
  _alignmentCorrections->Print();
}

void MimosaGlobalAlign::PrintNewAlignmentParameters()
{
  (*_alignmentVector+*_alignmentCorrections).Print(); 
  
}

void MimosaGlobalAlign::PrintResults(Int_t iteration)
{

  std::cout<<std::endl;

  std::cout<<"||-------------------------------------------------------------------------||"<<std::endl;
  std::cout<<"||-------------------------------- RESULTS --------------------------------||"<<std::endl;
  std::cout<<"||------------------------------ Iteration "<<iteration<<" ------------------------------||"<<std::endl;
  std::cout<<"||-------------------------------------------------------------------------||"<<std::endl;

  for(Int_t i=0 ; i<_planeNumber*_degreeOfFreedom ; ++i) {

    Int_t planeIndex = Int_t(i/_degreeOfFreedom);

    if(i%_degreeOfFreedom==0) std::cout<<std::endl;

    std::cout<<"  Plane "<<std::setw(10)<<planeIndex+1<<std::setw(15)<<_mapParameterName[i-planeIndex*_degreeOfFreedom]<<std::setw(15)<<"     --> Initial Value = "<<std::setw(12)<<(*_alignmentVector)[i]<<std::setw(15)<<"     --> Alignment Correction = "<<std::setw(15)<<(*_alignmentCorrections)[i]<<std::setw(15)<<"  --> New Value = "<<std::setw(15)<<(*_alignmentVector)[i]+(*_alignmentCorrections)[i]<<std::setw(15)<<" Error +/- "<<std::setw(15)<<sqrt((*_finalInverseMatrix)[i][i])<<std::endl;

  }

  std::cout<<std::endl;

  for( Int_t j=0 ; j<int(_hitsPerPlane.size()) ; ++j ) {
    
    if( _hitsPerPlane.size()==0 ) std::cout<<"CAUTION !!! : Plane number "<<j<<" with 0 hit. To get matrix inversion, set his readout to 0"<<std::endl;
  }

}

Double_t MimosaGlobalAlign::GetParameterNewValue(Int_t planeNumber, Int_t parameterNumber)
{
  return (*_alignmentVector)[(planeNumber-1)*_degreeOfFreedom+parameterNumber]+(*_alignmentCorrections)[(planeNumber-1)*_degreeOfFreedom+parameterNumber];
}

