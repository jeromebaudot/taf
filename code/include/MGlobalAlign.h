// Author: L. Cousin && Liu Q

#ifndef _MimosaGlobalAlign_included_
#define _MimosaGlobalAlign_included_

  /////////////////////////////////////////////////////////////
  //                                                         //
  //  Mimosa chip and ref. global alignement class           //
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

class MimosaGlobalAlign : public TObject {
   
   private:

   Int_t _debugLevel;
 
   Bool_t _trackParametersConstrainsts;
   Bool_t _alignParametersConstrainsts;
   Bool_t _multipleScatteringFit;

   Double_t _bound;

   std::map<Int_t, DPrecAlign*> _map_DPrecAlign;
   std::map<Int_t, DPlane*>     _map_Planes;
   std::map<Int_t, Int_t>       _map_PlanesIndex;
   
   std::vector<Int_t>            _hitsPerPlane;
    
   std::map<Int_t, TString>     _mapParameterName;

   Int_t _indexResiduals;
   Int_t _indexAlignmentParams;
   Int_t _indexTrackParams;

   Int_t _planeNumber;
   Int_t _axisNumber;
   Int_t _trackParametersNumber;
   Int_t _degreeOfFreedom;

   Double_t _fixedTrackDirectionX;
   Double_t _fixedTrackDirectionY;
   Double_t _sigmaTrackDirectionX;
   Double_t _sigmaTrackDirectionY;
   Double_t _sigmaTrackOriginX;
   Double_t _sigmaTrackOriginY;
   Double_t _sigmaTrackOriginZ;

   TVectorD* _alignmentVector;
   TMatrixD* _covarianceMatrix;
   TMatrixD* _covarianceMatrixMS;
   TMatrixD* _inverseCovarianceMatrix;

   TVectorD* _residualsVector;
   TVectorD* _residualsVectorMS;
   
   TMatrixD* _derivativesAlignmentMatrix;
   TMatrixD* _derivativesTracksMatrix;

   TMatrixD* _saveDerivativesAlignmentMatrix;
   TMatrixD* _saveDerivativesTracksMatrix;

   TMatrixD* _saveTransposeDerivativesAlignmentMatrix;
   TMatrixD* _saveTransposeDerivativesTracksMatrix;

   TMatrixD* _matrixB;
   TMatrixD* _matrixI;
   TMatrixD* _matrixG;

   TMatrixD* _identity;

   TMatrixD* _finalMatrix;
   TVectorD* _finalVector;
   TMatrixD* _finalInverseMatrix;

   TVectorD* _alignmentCorrections;
   //TVectorD* _trackParametersCorrections; COMMENTED. TO BE UPDATED :)
   TVectorD* _trackParameters;

   TMatrixD* _covarianceMatrixAlignementConstrainsts;
   TMatrixD* _inverseCovMatAlignConstraints;
   TMatrixD* _identityMatrix;
   TVectorD* _vectorAlignmentConstraints;

   TMatrixD* _covarianceMatrixTrackConstraints;
   TMatrixD* _inverseCovMatTrackConstraints;
   TMatrixD* _identidyMatrixTrack;
   TVectorD* _vectorTrackConstraints;

   TMatrixD* _saveFinalMatrix;

   TMatrixD* _matrixU; // for SVD decomp
   TMatrixD* _matrixV; // for SVD decomp
   TVectorD* _singularValues; // for SVD decomp
   TMatrixD* _singularMatrix; // for SVD decomp

   TCanvas** canvasResiduals;
   TH1F**    residualsU;
   TH1F**    residualsV;
   TH1F*     trackParameterX;
   TH1F*     trackParameterY;

   std::vector<DR3> _trackExtrapolation;
   std::vector<Double_t> _thetaMS;

   public:
  
   MimosaGlobalAlign(const Int_t& planeNumber, const Int_t& DoF, const Int_t& NbTrackPrameters, const Int_t& axisNumberInPlane, const Double_t& bound, const Bool_t alignConstrainsts, const Bool_t trackConstrainsts, const Bool_t _multipleScatteringFit);
   virtual ~MimosaGlobalAlign();

   void SetInitialAlignementParameters(std::map<const Int_t, DPlane*>& planeList, const Double_t& ResoAlignParamTr, const Double_t& ResoAlignParamRot);
   void SetDisplay();
   void ProcessTrack(DTrack*);

   Double_t ComputeResidualDerivative_U_AboutTranslationX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY);
   Double_t ComputeResidualDerivative_U_AboutTranslationY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY);
   Double_t ComputeResidualDerivative_U_AboutTranslationZ(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY);
   
   Double_t ComputeResidualDerivative_U_AboutTrackDirectionX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint);
   Double_t ComputeResidualDerivative_U_AboutTrackDirectionY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint);

   Double_t ComputeResidualDerivative_U_AboutTrackPointX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY);
   Double_t ComputeResidualDerivative_U_AboutTrackPointY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY);
   Double_t ComputeResidualDerivative_U_AboutTrackPointZ(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY);

   Double_t ComputeResidualDerivative_U_AboutRotationX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint);
   Double_t ComputeResidualDerivative_U_AboutRotationY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint);
   Double_t ComputeResidualDerivative_U_AboutRotationZ(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint);

   Double_t ComputeResidualDerivative_V_AboutTranslationX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY);
   Double_t ComputeResidualDerivative_V_AboutTranslationY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY);
   Double_t ComputeResidualDerivative_V_AboutTranslationZ(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY);

   Double_t ComputeResidualDerivative_V_AboutTrackDirectionX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint);
   Double_t ComputeResidualDerivative_V_AboutTrackDirectionY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint);

   Double_t ComputeResidualDerivative_V_AboutTrackPointX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY);
   Double_t ComputeResidualDerivative_V_AboutTrackPointY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY);
   Double_t ComputeResidualDerivative_V_AboutTrackPointZ(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY);

   Double_t ComputeResidualDerivative_V_AboutRotationX(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint);
   Double_t ComputeResidualDerivative_V_AboutRotationY(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint);
   Double_t ComputeResidualDerivative_V_AboutRotationZ(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY, DR3 trackPoint);

   Double_t ComputeTrackIntersectionU(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY,  DR3 trackPoint);
   Double_t ComputeTrackIntersectionV(Int_t planeIndex, Double_t trackDirectionX, Double_t trackDirectionY,  DR3 trackPoint);

   void FixPlane(Int_t planeIndex, Double_t sigma);
   void FixParameter(Int_t planeIndex, Int_t parameterIndex, Double_t sigma);
   void SetConstraintValueOnParameter(Int_t planeIndex, Int_t parameterIndex, Double_t value);

   void SetCovarianceMatrixToIdentity();
   void SetCovarianceMatrixMS();

   void ComputeResidualsMS();
   Double_t ComputeDistance(DR3 firstPlane, DR3 lastPlane);

   void SetIdentityForTrackConstraint();
   void SetCovMatrixAlignConstraintToIdentity();

   void FillMatrixB();
   void FillFinalGlobalMatrix();
   void FillGlobalFinalVector();
   
   void InvertFinalMatrix();
   void ComputeAlignmentCorrections();

   void DecomposeSVD_FinalMatrix();

   void FixTrackDirections(Double_t dirX, Double_t dirY) { _fixedTrackDirectionX = dirX; _fixedTrackDirectionY = dirY; };
   void SetTrackDirectionResolution(Double_t sigmaX, Double_t sigmaY) {_sigmaTrackDirectionX = sigmaX; _sigmaTrackDirectionY = sigmaY; }; 
   void SetTrackOriginResolution(Double_t sigmaTrackOriginX, Double_t sigmaTrackOriginY, Double_t sigmaTrackOriginZ) {_sigmaTrackOriginX = sigmaTrackOriginX; _sigmaTrackOriginY = sigmaTrackOriginY; _sigmaTrackOriginZ = sigmaTrackOriginZ; };

   void SetTrackParameters(DR3 trackDirection, DR3 trackOrigin);
   void FixTrackParameters(Double_t directionX, Double_t directionY, Double_t sigmaX, Double_t sigmaY);
   void FixTrackOrigin( DR3 fixedTrackOrigin, DR3 sigmaOrigin);

   void SetNewAlignmentParameters();

   void PrintAlignmentCorrections();
   void PrintNewAlignmentParameters();
   void PrintResults(Int_t iteration);

   void FillResidualsPlots();
   void FillTrackParametersPlots(DR3 trackDirections);
   void PrintResiduals();

   Double_t GetParameterNewValue(Int_t planeNumber, Int_t parameterNumber);

   Bool_t IfAlignConstrainsts() { return _alignParametersConstrainsts; };
   Bool_t IfTrackConstrainsts() { return _trackParametersConstrainsts; };

   void SetDebug(Int_t debug) { _debugLevel=debug; };

   ClassDef(MimosaGlobalAlign,1);

};
#endif
