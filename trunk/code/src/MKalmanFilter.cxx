/***************************************************************************//**
 * SVN File:    $Id$
 *
 * Project:     Kalman Filter & Smoother
 *
 * @class:      MKalmanFilter
 *
 * @brief:      
 *
 * Description:	A Kalman filter and smoother implementation
 *
 *
 * @createdby:  LIU Qingyuan <liuqingyuan678@gmail.com> at 2016-05-20 11:19:00
 * @copyright:  (c)2016 IPHC & HEPG - Shandong University. All Rights Reserved.
 *
 * @lastchange: $Revision$
 *              $Author$
 *              $Date$
 *
 *******************************************************************************/

/// project header files
#include "MKalmanFilter.h"

/// system header files
using namespace std;


///
/// constructor(s)
///_____________________________________________________________________________
MKalmanFilter::MKalmanFilter(Double_t mass, Double_t energy, Float_t charge){

     fMass = mass;
     fEnergy = energy;
     fCharge = charge;
     fZ = 0;
     fChiSquare = 0;

     fDebugLevel = 0;
     //fLayerXOverX0 = 0;
     //fLayerX = 0;
     fSmootherEnabled = kFALSE;
     fSmoothed = kFALSE;
     fUpdatedBeforePre = kFALSE;
     initTelescopeTracking();
}

///
/// destructor
///_____________________________________________________________________________
MKalmanFilter::~MKalmanFilter() {

     clearSmoother();
}

void MKalmanFilter::reset(){

     clearSmoother();
     fSmoothed = kFALSE;
     fUpdatedBeforePre = kFALSE;
     fChiSquare = 0;

}

void MKalmanFilter::initTelescopeTracking(){

     fPar.ResizeTo(4,1); // x, tanX, y, tanY
     fCov.ResizeTo(4,4);
     fHmatrix.ResizeTo(2,4);
     fFmatrix.ResizeTo(4,4);
     fMvector.ResizeTo(2,1);
     fVmatrix.ResizeTo(2,2);
     fQmatrix.ResizeTo(4,4);
     fKmatrix.ResizeTo(4,2);
     fResidual.ResizeTo(2,1);

     Double_t dataHmatrix[8] = {
	  1,0,0,0,
	  0,0,1,0};
     setHmatrix(dataHmatrix);

     Double_t dataFmatrix[16] = {
	  1,   2, 0,   0,
	  0,   1, 0,   0,
	  0,   0, 1,   2,
	  0,   0, 0,   1};
     setFmatrix(dataFmatrix);
}

void MKalmanFilter::update(const Double_t z, const Double_t* meas, const Double_t* measCov){

     setZ(z);
     setMvector(meas);
     if (measCov != NULL)
	  setVmatrix(measCov);

     prepare_beforeUpdate();

     // K_k Calulation
     TMatrixD tmpHt(TMatrixD::kTransposed,fHmatrix);
     //TMatrixD tmpCovHt(4,2);
     //tmpCovHt =fCov * tmpHt;
     TMatrixD tmpCovHt(fCov, TMatrixD::kMult ,tmpHt);

     TMatrixDSym tmpHCovHt(fCov);
     tmpHCovHt.Similarity(fHmatrix);
     TMatrixD tmpInvTerm(tmpHCovHt);
     tmpInvTerm += fVmatrix;
     //TMatrixD tmpInvTerm(tmpHCovHt, TMatrixD::kPlus, fVmatrix); // doesn't work. Why?
     tmpInvTerm.Invert();

     fKmatrix = tmpCovHt * tmpInvTerm;

     // Update fPar,  the state vector
     //TMatrixD tmpKH(4,4);
     //tmpKH = fKmatrix * fHmatrix;
     TMatrixD tmpKH(fKmatrix, TMatrixD::kMult, fHmatrix);
     fPar += fKmatrix * fMvector - tmpKH*fPar;

     // Update fCov, Covariance matrix
     TMatrixD covResult = fCov - tmpKH * fCov;
     fCov.SetMatrixArray(covResult.GetMatrixArray());
     // Make sure fCov is symmetric
     //checkCov(fCov);
     prepare_afterUpdate();

     ///======== Debug =======
     if(fDebugLevel > 0){
	  printf("DEBUG: Updated Cov:");
	  fCov.Print();
     }
     if(fDebugLevel > 1){
	  Printf("DEBUG: ");
     }
}

void MKalmanFilter::updateAlt(const Double_t z, const Double_t* meas, const Double_t* measCov){

     setZ(z);
     setMvector(meas);
     if (measCov != NULL)
	  setVmatrix(measCov);
     prepare_beforeUpdate();
     TMatrixDSym tmpVinvs(TMatrixDSym::kInverted, fVmatrix);
     TMatrixDSym tmpHtViH(tmpVinvs);
     tmpHtViH = tmpHtViH.SimilarityT(fHmatrix);

     TMatrixDSym tmpCovInvs(TMatrixDSym::kInverted, fCov);
     // Update fCov, Covariance matrix
     fCov = tmpCovInvs + tmpHtViH;
     fCov.InvertFast();

     // K_k Calulation
     TMatrixD tmpHt(TMatrixD::kTransposed,fHmatrix);
     fKmatrix = fCov * tmpHt * tmpVinvs; 

     // Update fPar,  the state vector
     //TMatrixD tmpKH(4,4);
     //tmpKH = fKmatrix * fHmatrix;
     TMatrixD tmpKH(fKmatrix, TMatrixD::kMult ,fHmatrix);
     fResidual = fMvector - fHmatrix*fPar;
     fPar += fKmatrix * fResidual;
     //fPar += fKmatrix * (fMvector - fHmatrix*fPar);
     prepare_afterUpdate();
}

void MKalmanFilter::updateBack(const Double_t z, const Double_t* meas, const Double_t* measCov){

     setZ(z);
     setMvector(meas);
     if (measCov != NULL)
	  setVmatrix(measCov);
     prepare_beforeUpdate();
     TMatrixDSym tmpVinvs(TMatrixDSym::kInverted, fVmatrix);
     TMatrixDSym tmpHtViH(tmpVinvs);
     tmpHtViH = tmpHtViH.SimilarityT(fHmatrix);

     TMatrixDSym tmpCovInvs(TMatrixDSym::kInverted, fCov);
     // Update fCov, Covariance matrix
     fCov = tmpCovInvs + tmpHtViH;
     fCov.InvertFast();


     /// Optimal Estimation of Dynamic Systems p349 eq 6.32
     // CovPre_N should be very large --> CovPre_N^-1 == 0 (When used for the Forw-Back-filters Smoother)
     // Update fPar,  the state vector
     TMatrixD tmpCoviPar(tmpCovInvs, TMatrixD::kMult, fPar);
     TMatrixD tmpHt(TMatrixD::kTransposed, fHmatrix);
     tmpCoviPar += tmpHt * tmpVinvs * fMvector;
     fPar = fCov * tmpCoviPar;
     //TMatrixD tmpKH(fKmatrix, TMatrixD::kMult ,fHmatrix);
     //fPar += fKmatrix * (fMvector - fHmatrix*fPar);
     prepare_afterUpdate();
}

void MKalmanFilter::prepareQmatrix(const Double_t x, const Double_t xOverX0){

     if (fMass == 0 && fEnergy == 0){
	  printf("ERROR: No beam status! Please setBeam first!\n");
	  exit(0);
     }
     ///Multiple Scattering
     Double_t tanLx = fPar(1,0);
     Double_t tanLy = fPar(3,0);
     Double_t tmp = 1 + tanLx*tanLx + tanLy*tanLy;
     Double_t dLOverdZ = sqrt(tmp);
     Double_t lOverX0 = xOverX0 * dLOverdZ;

     Double_t p = TMath::Sqrt(fEnergy*fEnergy - fMass*fMass);
     Double_t p2 = p*p;
     Double_t beta = p/fEnergy;
     Double_t beta2 = beta*beta;
     Double_t theta2;
     theta2=0.0136*0.0136*fCharge*fCharge/(beta2*p2)*TMath::Abs(lOverX0);
     if (true){
       double lt = 1+0.038*TMath::Log(TMath::Abs(lOverX0));
       if (lt>0) theta2 *= lt*lt;
     }

     //x = 0; // simplify Qmatrix
     Double_t thick = x;
     Double_t thick2 = thick*thick;
     // //fPar = {x, slopeX, y, slopeY}
     // Double_t dataQmatrix[16] = {
     //     thick2/3, thick/2,        0,       0,
     //      thick/2,	  1,        0,       0,
     //      	 0, 	  0, thick2/3, thick/2,
     //      	 0, 	  0,  thick/2,       1};
     // //Double_t dataQmatrix[16] = {
     // //       fZ*fZ,    -fZ,        0,       0,
     // //         -fZ,	  1,        0,       0,
     // //     	 0, 	  0, thick2/3, thick/2,
     // //     	 0, 	  0,  thick/2,       1};
     // //Double_t dataQmatrix[16] = {
     // //    thick2/3, thick/2,        0,       0,
     // //     thick/2,	  1,        0,       0,
     // //     	 0, 	  0, thick2/3, thick/2,
     // //     	 0, 	  0,  thick/2,       1};
     // fQmatrix.SetMatrixArray(dataQmatrix);
     // fQmatrix *= theta2;
     Double_t qLxLx = theta2 * (1 + tanLx*tanLx) * tmp;
     Double_t qLyLy = theta2 * (1 + tanLy*tanLy) * tmp;
     Double_t qLxLy = theta2 * tanLx * tanLy * tmp;

     //// When intercept is chosen in state vector!
     //Double_t dz = -fZ0;
     //Double_t qxx = dz*dz * qLxLx;
     //Double_t qyy = dz*dz * qLyLy;
     //Double_t qxy = dz*dz * qLxLy;
     //Double_t qxLx = dz * qLxLx;
     //Double_t qyLy = dz * qLyLy;
     //Double_t qxLy = dz * qLxLy;
     Double_t qxx = thick2/3 * qLxLx;
     Double_t qyy = thick2/3 * qLyLy;
     Double_t qxy = thick2/3 * qLxLy;
     Double_t qxLx = thick/2 * qLxLx;
     Double_t qyLy = thick/2 * qLyLy;
     Double_t qxLy = thick/2 * qLxLy;
     Double_t qyLx = qxLy;
     Double_t dataQmatrix[16] = {
	  qxx,  qxLx,  qxy,   qxLy,
	  qxLx, qLxLx, qyLx, qLxLy,
	  qxy,  qyLx,  qyy,   qyLy,
	  qxLy, qLxLy, qyLy, qLyLy};
     fQmatrix.SetMatrixArray(dataQmatrix);
}

void MKalmanFilter::propagateTo(const Double_t z, const Double_t x, const Double_t xOverX0){
     Double_t dz = z - fZ;
     fZ = z; 
     prepareFmatrix(dz);
     prepareQmatrix(x, xOverX0);
     prepare_propagate();

     /// fPar prediction
     fPar = fFmatrix * fPar;

     /// fCov prediction
     TMatrixDSym tmpFCFt(fCov);
     tmpFCFt = tmpFCFt.Similarity(fFmatrix);
     fCov = tmpFCFt + fQmatrix;
     // fCov is already symmetric
     //checkCov(fCov);
     prepare_afterPropagate();

}

void MKalmanFilter::propagateBackTo(const Double_t z, const Double_t x, const Double_t xOverX0){
     Double_t dz = fZ - z;
     fZ = z; 
     prepareFmatrix(dz);
     TMatrixD Fi(TMatrixD::kInverted, fFmatrix);

     prepareQmatrix(x, xOverX0);
     prepare_propagate();

     /// fPar prediction
     fPar = Fi * fPar;

     /// fCov prediction
     TMatrixDSym tmpFiCFti(fCov + fQmatrix);
     fCov = tmpFiCFti.Similarity(Fi);
     prepare_afterPropagate();

}

Bool_t MKalmanFilter::smoothTo(Double_t z){
     // Smooth to a layer

     if(!fSmootherEnabled){
	  printf("Smoother is not enabled!\n");
	  return kFALSE;
     }

     // At the last plane for tracking, there is no propagation. such that nFmatrix == nCmatrix - 1
     Int_t nFmatrix = fFmatrixArr.size();
     Int_t nCmatrix = fCovArr.size();
     if (nFmatrix < nCmatrix - 1 ){
	  printf("ERROR: At least one Fmatrice is missing!\n");
	  return kFALSE;
     }
     //else if nFmatrix == nCmatrix - 1 // Good!
     else if (nFmatrix == nCmatrix){
	  if (fDebugLevel > 0)
	       printf("WARNING: More predictions than updates! \n");
	  fFmatrixArr.pop_back();// drop the additional Fmatrix which should be used to propagate to DUT without KF::update process there.
     }
     else if (nFmatrix > nCmatrix){
	       printf("ERROR: Smoother is completely puzzlez!\n ");
	       return kFALSE;
     }
     /// check the existence of the index to be smoothed.
     //printf("%f\n", double(fZArr.back()));
     if(std::find(fZArr.begin(), fZArr.end(), z) == fZArr.end()){
	  printf ("ERROR: smoother can't find your detector at %f\n", z);
	  return kFALSE;
     }

     /// Smoothing
     fCov = fCovArr.back();
     fPar = fParArr.back();
     if (z == fZArr.back()){
	  if (fDebugLevel > 1)
	       printf("INFO: The last layer doesn't need smoothing.\n");
	  fSmoothed = kTRUE;
	  return kTRUE;
     }

     //else
     //Obsolete
     //old//while (z != fZArr.back()){

     //old//     fZArr.pop_back();
     //old//     fZ = fZArr.back();
     //old//     TMatrixDSym covS_Kplus1 = fCov; // CovS at K+1 smoothed Cov
     //old//     TMatrixD parS_Kplus1 = fPar; // parS at K+1 smoothed par
     //old//     TMatrixDSym covPre_Kplus1 = fCovPreArr.back(); // CovPre at k+1
     //old//     TMatrixD parPre_Kplus1 = fParPreArr.back(); // parPre at k+1

     //old//     TMatrixD F_K = fFmatrixArr.back(); // F at k

     //old//     fCovArr.pop_back();
     //old//     fParArr.pop_back();
     //old//     TMatrixDSym cov_K = fCovArr.back(); // Cov at k
     //old//     TMatrixD par_K = fParArr.back(); // par at k


     //old//     fFmatrixArr.pop_back();
     //old//     fCovPreArr.pop_back();
     //old//     fParPreArr.pop_back();

     //Avoid pop_back().  smoothTo() could be used more than one time.
     itZArr       = fZArr.end(); itZArr--;
     itCovPreArr  = fCovPreArr.end(); itCovPreArr--;
     itCovArr     = fCovArr.end(); itCovArr--;
     itParPreArr  = fParPreArr.end(); itParPreArr--;
     itParArr     = fParArr.end(); itParArr--;
     itFmatrixArr = fFmatrixArr.end(); itFmatrixArr--;
     while(z != *itZArr){
          itZArr--;
          fZ = *itZArr;
          TMatrixDSym covS_Kplus1 = fCov; // CovS at K+1 smoothed Cov
          TMatrixD parS_Kplus1 = fPar; // parS at K+1 smoothed par
          TMatrixDSym covPre_Kplus1 = *itCovPreArr; // CovPre at k+1
          TMatrixD parPre_Kplus1 = *itParPreArr; // parPre at k+1

          TMatrixD F_K = *itFmatrixArr; // F at k

          itCovArr--;
          itParArr--;
          TMatrixDSym cov_K = *itCovArr; // Cov at k
          TMatrixD par_K = *itParArr; // par at k


          itFmatrixArr--;
          itCovPreArr--;
          itParPreArr--;

	  
	  //---------------------------------
	  TMatrixDSym covPi_K1(TMatrixDSym::kInverted, covPre_Kplus1);
	  TMatrixD Ft_K(TMatrixD::kTransposed, F_K);
	  TMatrixD A_K = cov_K * Ft_K * covPi_K1;

	  covS_Kplus1 = covS_Kplus1 - covPre_Kplus1;
	  //TMatrixD tempCov(fCov);
	  //tempCov = cov_K + covS_Kplus1.Similarity(A_K);
	  //fCov.SetMatrixArray(tempCov.GetMatrixArray());
	  fCov = cov_K + covS_Kplus1.Similarity(A_K);
	  fPar = par_K + A_K * (parS_Kplus1 - parPre_Kplus1); 

     }
     //old//fSmootherEnabled = kFALSE;
     //old//clearSmoother();

     fSmoothed = kTRUE;
     return kTRUE;

}

void MKalmanFilter::clearSmoother(){
   fCovPreArr.clear(); 
   fParPreArr.clear(); 
   fCovArr.clear();    
   fParArr.clear();    
   fZArr.clear();     
   fFmatrixArr.clear();

}

void MKalmanFilter::print()const{

     //fPar.Print();

     printf("Covariance ");
     fCov.Print();
}
