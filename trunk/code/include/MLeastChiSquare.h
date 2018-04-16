/***************************************************************************//**
 * SVN File:    $Id$
 *
 * Project:     PandaX DAQ
 *
 * @class:      MLeastChiSquare
 *
 * @brief:      
 *
 * Description:	Least Chi-square fitting
 *
 *
 * @createdby:  LIU Qingyuan <liuqingyuan678@gmail.com> at 2016-06-01 14:07:38
 * @copyright:  (c)2016 HEPG - Shandong University. All Rights Reserved.
 *
 * @lastchange: $Revision$
 *              $Author$
 *              $Date$
 *
 *******************************************************************************/
#ifndef MLeastChiSquare_h
#define MLeastChiSquare_h

#include "TMath.h"
#include <TMatrixD.h>
#include <TMatrixDSym.h>

#include <algorithm>
#include <vector>



class MLeastChiSquare {
public:
   /// constructor(s)
   MLeastChiSquare();

   /// destructor
   ~MLeastChiSquare();

   /// Results
   Double_t* GetPar(){return fPar.GetMatrixArray();}
   Double_t* GetCov(){return fCov.GetMatrixArray();}

   /// Inputs
   std::vector<Double_t>* GetPtrMeasArray() {return &fMeasArray;} 
   std::vector<Double_t>* GetPtrSigmaArray()  {return &fSigmaArray;}
   std::vector<Double_t>* GetPtrZArray()  {return &fZArray;}
   std::vector<Double_t>* GetPtrSigmaMSArray()  {return &fSigmaMSArray;}
   std::vector<Double_t>* GetPtrZScatterArray()  {return &fZScatterArray;}

   void     Estimate();
   Double_t GetSigmaAtZ(const Double_t &z) const; // calculate the uncertainty of the extrapolated track postition at z
   void     Print() const; 

protected:

   //Int_t    nHits; // number of hits, also used to define fA dimension
   /// Inputs
   std::vector<Double_t> fMeasArray;     // dimension should be n
   std::vector<Double_t> fSigmaArray;    // measurements uncertainties
   std::vector<Double_t> fZArray;        // z position of detecting layers
   std::vector<Double_t> fSigmaMSArray;  // multiple scattering
   std::vector<Double_t> fZScatterArray; // z position of scattering layers

   ///Matrices
   TMatrixD fPar; // x =  x_0 + k * z, fpar = {x_0,  slopeX}^T with dimension (2,1)
   TMatrixDSym fCov; // Covariance matrix of fPar, dimension(2,2);
   TMatrixD fA; // Auxiliary matrix with dimension (n, 2)
                // 1, z1, 1, z2, 1, z3 ..., 1, zn
   TMatrixD fMeas; // Measurements fMeas = {y1, y2, ..., ynHits}^T with dimentsion (n, 1)
   TMatrixDSym fMeasCov;  // Covariance matrix of fMeas with dimension (n,n)


   void     PrepareMatrices();
//private:


};

#endif //~ MLeastChiSquare_h
