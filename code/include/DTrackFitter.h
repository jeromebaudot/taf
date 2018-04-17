// Author: L. Cousin && Liu Q

#ifndef _DTrackFitter_included_
#define _DTrackFitter_included_

  /////////////////////////////////////////////////////////////
  //                                                         //
  //  Track fiiting class           //
  //                                                         //
  //                                                         //   
  /////////////////////////////////////////////////////////////

#include "Riostream.h"
#include "TMath.h"
#include "TMatrixD.h"
#include "TVectorD.h"
//#include "TMatrixDLazy.h"
#include "TVectorD.h"
#include "TDecompLU.h"
#include "TDecompSVD.h"

#include "DPrecAlign.h"
#include "DTrack.h"
#include "DPlane.h"

class DTrackFitter : public TObject {

  // Track Fitting  : Least Square Fit with matrix form. // LC 2015/06/24
  private:

  Bool_t    _multipleScatteringFit;

  Int_t     _hitNumber;
  Int_t     _localAxisNumber;
  Int_t     _trackParametersNumber;

  DHit**    _fHitList;

  TMatrixD* _derivativeTrackMatrix;
  
  TMatrixD* _covarianceMatrix;
  TMatrixD* _covarianceMatrixMS;
  
  TMatrixD*  _inverseCovarianceMatrix;

  TMatrixD* _finalMatrix;
  TMatrixD* _inverseFinalMatrix;

  TVectorD* _residualsVector;
  TVectorD* _residualsVectorMS; 

  TVectorD* _trackParameters;
  TVectorD* _trackParametersCorrections;

  TVectorD* _finalVector;
  
  std::map<Int_t, DPrecAlign*> _map_DPrecAlign;
  std::map<Int_t, DPlane*>     _map_Planes;

  std::vector<DR3> _trackExtrapolation;
  std::vector<Double_t> _thetaMS;

  DR3 _initialTrackParameters;
  DR3 _initialTrackOrigin;

  public:

  Double_t ComputeResidualDerivative_U_AboutTrackDirectionX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint);
  Double_t ComputeResidualDerivative_U_AboutTrackDirectionY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint);

  Double_t ComputeResidualDerivative_V_AboutTrackDirectionX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint);
  Double_t ComputeResidualDerivative_V_AboutTrackDirectionY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint);

  Double_t ComputeResidualDerivative_U_AboutTrackPointX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY);
  Double_t ComputeResidualDerivative_U_AboutTrackPointY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY);
  Double_t ComputeResidualDerivative_U_AboutTrackPointZ(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY);

  Double_t ComputeResidualDerivative_V_AboutTrackPointX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY);
  Double_t ComputeResidualDerivative_V_AboutTrackPointY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY);
  Double_t ComputeResidualDerivative_V_AboutTrackPointZ(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY);

  Double_t ComputeTrackIntersectionU(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY,  DR3 trackPoint);
  Double_t ComputeTrackIntersectionV(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY,  DR3 trackPoint);

  void DeleteMatricesAndVectors();

  DTrackFitter(Int_t trackParameters, Int_t hitNumber, Int_t localAxisNumber, Bool_t ifMultipleScattering, DHit** hitList, DR3 initialTrackParameters, DR3 initialTrackOrigin);
  ~DTrackFitter();

  void SetCovarianceMatrixToIdentity();

  void SetCovarianceMatrixMS();
  void ComputeResidualsMS();

  void ProcessHits();             // Fill Vectors and Matrices.

  void ComputeTrackParameters();  // Matrix Inversion included.
  
  void RedefineTrackParameters();

  void PrintTrackParameters();
  
  Double_t ComputeDistance(DR3 firstPlane, DR3 lastPlane);

  DR3 GetTrackDirections();
  DR3 GetTrackOrigin();

};

#endif
