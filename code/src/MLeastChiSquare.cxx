/***************************************************************************//**
 * SVN File:    $Id$
 *
 * Project:     Pandax DAQ
 *
 * @class:      MLeastChiSquare
 *
 * @brief:      
 *
 * Description: Least Chi-square fitting
 *
 *
 * @createdby:  LIU Qingyuan <liuqingyuan678@gmail.com> at 2016-06-01 14:08:51
 * @copyright:  (c)2016 HEPG - Shandong University. All Rights Reserved.
 *
 * @lastchange: $Revision$
 *              $Author$
 *              $Date$
 *
 *******************************************************************************/

/// project header files
#include "MLeastChiSquare.h"

/// system header files

using namespace std;


///
/// constructor(s)
///_____________________________________________________________________________
MLeastChiSquare::MLeastChiSquare() {

     
     fPar.ResizeTo(2,1);
     fCov.ResizeTo(2,2);
}


///
/// destructor
///_____________________________________________________________________________
MLeastChiSquare::~MLeastChiSquare() {

}

///
///_____________________________________________________________________________
void MLeastChiSquare::PrepareMatrices(){
     
     size_t nMeas = fMeasArray.size();
     size_t nSigma = fSigmaArray.size();
     size_t nZ = fZArray.size();

     size_t nSigmaMS = fSigmaMSArray.size();
     size_t nZScatter = fZScatterArray.size();

     if(nMeas == 0){
	  printf("ERROR: No measurements!\n");
	  exit(0);
     }
     if(nMeas!=nSigma || nMeas != nZ){
	  printf("ERROR: Please check measurements related inputs. Array size is not equal!\n");
	  exit(0);
     }
     if(nZScatter!=nSigmaMS){
	  printf("ERROR: Please check scattering layers inputs. Array size is not equal!\n");
	  exit(0);
     }
     /// order check of zPosition. The codes below are possible for C++11
     //if( !std::is_sorted(fZArray.begin(), fZArray.end()) ||
     //          !std::is_sorted(fZScatterArray.begin(), fZScatterArray.end())){
     //     printf("ERROR: z positions are not sorted!\n");
     //     exit(0);
     //}

     //Resize matrices
     fA.ResizeTo(nMeas, 2);
     fMeas.ResizeTo(nMeas,1);
     fMeasCov.ResizeTo(nMeas, nMeas);
     
     //Init matrices
     for(unsigned i = 0; i < nMeas; i++){
	  fA(i,0) = 1;
	  fA(i,1) = fZArray.at(i);
	  fMeas(i,0) = fMeasArray.at(i);
	  fMeasCov(i,i) = pow(fSigmaArray.at(i),2);
     }
     // top-right triangle of fMeasCov
     for(unsigned i = 0; i<nMeas; i++){
	  for(unsigned j = i; j<nMeas; j++){
	       for(unsigned l =0; l < nZScatter; l++){
		    if((fZScatterArray)[l]<(fZArray)[i])
		      fMeasCov(i,j) += ((fZArray)[i] - (fZScatterArray)[l])*((fZArray)[j]-(fZScatterArray)[l])*pow(fSigmaMSArray.at(l),2);
	       }
	       if(i!=j) fMeasCov(j,i) = fMeasCov(i,j);
	  }
     }
     
}

///
///_____________________________________________________________________________
void MLeastChiSquare::Estimate(){

     PrepareMatrices();
     TMatrixDSym tMeasCovInv(fMeasCov);
     tMeasCovInv.Invert();
     TMatrixDSym tmpAtViA(tMeasCovInv);
     tmpAtViA.SimilarityT(fA); 
     fCov = tmpAtViA.Invert();

     TMatrixD tmpAt(TMatrixD::kTransposed, fA);
     fPar = fCov*tmpAt*tMeasCovInv*fMeas;

}

///
///_____________________________________________________________________________
Double_t MLeastChiSquare::GetSigmaAtZ(const Double_t &z) const{

     Double_t result = 0;
     Double_t covX0 = fCov(0,0);
     Double_t covSlope = fCov(1,1);
     Double_t covX0Slope = fCov(0,1);
     result = covX0 + z*z*covSlope  + 2 * z * covX0Slope;

     return sqrt(result);

}

///
///_____________________________________________________________________________
void MLeastChiSquare::Print() const {
     printf("fA ");
     fA.Print();
     printf("fMeas ");
     fMeas.Print();
     printf("fMeasCov ");
     fMeasCov.Print();
     printf("fPar ");
     fPar.Print();
     printf("fCov ");
     fCov.Print();
}
