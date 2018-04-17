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
#ifndef MKalmanFilter_h
#define MKalmanFilter_h

#include "TMath.h"
#include <TMatrixD.h>
#include <TMatrixDSym.h>

#include <algorithm>
#include <vector>

class MKalmanFilter {
public:
   /// constructor(s)
   MKalmanFilter(Double_t mass = 0, Double_t energy = 0, Float_t charge = 1);

   /// destructor
   ~MKalmanFilter();

   /// Reset for next track
   void reset();

   /// Results
   const Double_t* getPar()const{return fPar.GetMatrixArray();}
   const Double_t* getCov()const{return fCov.GetMatrixArray();}

   Double_t getZ()const{return fZ;}
   Bool_t   isSmootherEnabled() const{return fSmootherEnabled;}
   void initTelescopeTracking();
   /// setting
   void setZ(const Double_t z){fZ = z;}
   void setSmootherEnabled(const Bool_t enabled){fSmootherEnabled = enabled;}
   //void setLayerX(Double_t x){fLayerX = x;}
   //void setLayerXOverX0(Double_t xOverX0){fLayerXOverX0 = xOverX0;}
   void setBeam(const Double_t mass, const Double_t energy, const Float_t charge=1.){fMass = mass;fEnergy=energy;fCharge = charge;}

   void initKalZParCov(const Double_t z, const Double_t* dataPar, const Double_t * dataCov){
	fZ = z;
	fZ0 = z;
	fPar.SetMatrixArray(dataPar);
        fCov.SetMatrixArray(dataCov);
   }
   void setPar(const Double_t* data){fPar.SetMatrixArray(data);}
   void setCov(const Double_t* data){fCov.SetMatrixArray(data);}
   void setHmatrix(const Double_t* data){fHmatrix.SetMatrixArray(data);}
   void setFmatrix(const Double_t* data){fFmatrix.SetMatrixArray(data);}
   void setMvector(const Double_t* data){fMvector.SetMatrixArray(data);}
   void setVmatrix(const Double_t* data){fVmatrix.SetMatrixArray(data);}
   void setQmatrix(const Double_t* data){fQmatrix.SetMatrixArray(data);}

   /// KF init, call initKalZParCov();
   //setZ
   //setPar
   //setCov

   /// KF update
   //setZ
   //setMvector
   //setVmatrix
   // forward filter
   void update(const Double_t z, const Double_t* meas, const Double_t* measCov=NULL);//
   void updateAlt(const Double_t z, const Double_t* meas, const Double_t* measCov=NULL);// Alternative Kalman Filter, uesfull to keep fCov symmetric
   // backward filter
   void updateBack(const Double_t z, const Double_t* meas, const Double_t* measCov=NULL);//
   
   /// KF predict/propagate
   void prepareFmatrix(const Double_t dz){fFmatrix(0,1) = fFmatrix(2,3) = dz;}
   void prepareQmatrix(const Double_t x, const Double_t xOverX0);
   // forward filter
   void propagateTo(const Double_t z, const Double_t x, const Double_t xOverX0);
   // backward filter
   void propagateBackTo(const Double_t z, const Double_t x, const Double_t xOverX0);

   /// KF smoother
   Bool_t smoothTo(const Double_t z);
   void clearSmoother();
   void prepare_propagate();
   void prepare_afterPropagate();
   void prepare_beforeUpdate();
   void prepare_afterUpdate();

   /// ChiSquare in filtering
   Double_t getChiSquare()const{return fChiSquare;}


   /// tools
   void checkCov(TMatrixDSym &m)const;
   void print()const;
   void setDebugLevel(const Int_t l){fDebugLevel = l;}

//   void makeMatrixSymmetric(TMatrixD &m){
//	TMatrixD tmpMt(TMatrixD::kTransposed,m);
//        m = 0.5*(m + tmpMt);
//}

protected:

   TMatrixD fPar; // State/Parameter vector: ResizeTo(n, 1) latter {x, tanX, y, tanY};
   TMatrixDSym fCov; // Covariance matrix of fPar

   TMatrixD fHmatrix; // Measurement matrix H
   TMatrixD fFmatrix; // Propagation function F
   TMatrixD fMvector; // Measurement vector
   TMatrixDSym fVmatrix; // Covariance matrix of fMvector
   
   TMatrixDSym fQmatrix; // Covariance matrix of the propagation noise(MCS, Energy loss)
   TMatrixD fKmatrix; // Kalman gain

   TMatrixD fResidual; // residual = fMvector - fHmatrix * fPar;
   Double_t fChiSquare;

   Double_t fZ; // current telescope plane position
   Double_t fZ0; // the first z, which is used to calculate Qmatrix(MS)!!
   //Double_t fLayerX; // thickness (mm)
   //Double_t fLayerXOverX0; // thickness in radiation length (mm)

   ///Beam
   Double_t fMass; // GeV
   Double_t fEnergy; // GeV
   Float_t  fCharge; // charge number

   /// Smoothing related
   Bool_t fSmootherEnabled;
   Bool_t fSmoothed;
   Bool_t fUpdatedBeforePre;// updated before prediction

   // push_back in KF update
   std::vector<TMatrixDSym> fCovPreArr;  // predicted
   std::vector<TMatrixD>    fParPreArr;  // 
   std::vector<TMatrixDSym> fCovArr;     // filtered
   std::vector<TMatrixD>    fParArr;     // 
   std::vector<Double_t>    fZArr;       // fZ(Used as plane index) array '
   // push_back in KF propagateTo
   std::vector<TMatrixD>    fFmatrixArr; // propagation matrices
   ///iterators
   std::vector<Double_t>::iterator    itZArr     ; 
   std::vector<TMatrixDSym>::iterator itCovPreArr; 
   std::vector<TMatrixDSym>::iterator itCovArr   ; 
   std::vector<TMatrixD>::iterator    itParPreArr; 
   std::vector<TMatrixD>::iterator    itParArr   ; 
   std::vector<TMatrixD>::iterator    itFmatrixArr;

   ///Tool
   Int_t fDebugLevel;
//private:




};

inline void MKalmanFilter::prepare_propagate(){
     if(fSmootherEnabled){
          if (!fSmoothed)
     	  fFmatrixArr.push_back(fFmatrix);
  	}
}

inline void MKalmanFilter::prepare_afterPropagate(){
     if(fSmootherEnabled){
          if (!fUpdatedBeforePre && !fSmoothed){
	       prepare_beforeUpdate();
	       prepare_afterUpdate();
	  }
     }
     fUpdatedBeforePre = kFALSE;
}

inline void MKalmanFilter::prepare_beforeUpdate(){
     if(fSmootherEnabled){
          if (fSmoothed){
     	  printf("ERROR: Smoother Already exists! No more updates acceptable!\n");
     	  exit(0);
          }
          fZArr.push_back(fZ);
          fCovPreArr.push_back(fCov);
          fParPreArr.push_back(fPar);
     }
}

inline void MKalmanFilter::prepare_afterUpdate(){
     if(fSmootherEnabled){
          fCovArr.push_back(fCov);
          fParArr.push_back(fPar);
     }
     TMatrixDSym tmpHCHt(fCov);
     tmpHCHt.Similarity(fHmatrix);
     TMatrixDSym ResidErr(fVmatrix,TMatrixDSym::kMinus, tmpHCHt);
     TMatrixDSym ResidErrInv(TMatrixDSym::kInverted, ResidErr);
     TMatrixD Increment(1,1);
     Increment = ResidErrInv.SimilarityT(fResidual);
     fChiSquare += Increment(0,0);
     fUpdatedBeforePre = kTRUE;
}

/// tools
inline void MKalmanFilter::checkCov(TMatrixDSym &m)const{
     for(int i = 0; i< 4; i++){
          for(int j =0; j < i; j++){
               m(j,i) = m(i,j);
          }
          m(i,i) = TMath::Abs(m(i,i));
     }
}
#endif //~ MKalmanFilter_h
