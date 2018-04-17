// @(#)maf/dtools:$Name:  $:$Id: DLine.cxx,v.1 2005/10/02 18:03:46 sha Exp $
// Author :  Dirk Meier     98/01/09

  //////////////////////////////////////////////////////////////////////////////////////////////
  //                                                                                          //
  // Class Description of DLine                                                               // 
  //                                                                                          //
  // a Line object is defined by its                                                          //
  //                  origin     = (x_0,y_0,z_0),                                             //
  //                  direction  = (dx,dy,dz),                                                //
  // and              length.                                                                 //
  // Points on the line at r_i are given as a function                                        //
  // of the parameter beta. beta has no dimension.                                            //
  // r_i(beta) = origin_i + beta * direction_i                                                //
  // If one wants the pair (x,y) as a function of z along (0,0,dz) then beta = z/dz           //
  //    and r_1(beta) = x_0 + z * dx/dz                                                       //
  //        r_2(beta) = y_0 + z * dy/dz                                                       //
  //        r_3(beta) = z_0 + z * 1                                                           //  
  // In case one needs pair (x,y,z) as a function of l along (dx,dy,dz) then beta' = l/dl     //
  //    and r_1(beta') = x_0 + l * dx/dl                                                      //
  //        r_2(beta') = y_0 + l * dy/dl                                                      //
  //        r_3(beta') = z_0 + l * dz/dl                                                      //
  // with the relation beta^2 = beta'^2 * (1-x^2/l^2-y^2/l^2) / (1-dx^2/dl^2-dy^2/dl^2)       //
  //                                                                                          //
  //////////////////////////////////////////////////////////////////////////////////////////////


//*-- Modified :  
//*-- Copyright:  RD42
//*-- Author   :  Dirk Meier   98/01/09
//*KEEP,CopyRight.

// Last Modified: JB 2011/07/25 destructor

/************************************************************************
 * Copyright(c) 1997, DiamondTracking, RD42@cern.ch
 ***********************************************************************/
//*KEND.

//*KEEP,DLine.
#include "DLine.h"
//*KEEP,DR3.
#include "DR3.h"
//*KEND.

ClassImp(DLine) // Description of a Line

//______________________________________________________________________________
//  

DLine::DLine(){ 
  fOrigin    = new DR3();
  fDirection = new DR3();
  fSlope     = new DR3();
  fLength    = 0.0;
}

//______________________________________________________________________________
//  

DLine::DLine(DR3 &aOrigin, DR3 &aDirection, Float_t aLength){
  fOrigin    = new DR3(aOrigin);
  fDirection = new DR3(aDirection);
  fSlope     = new DR3();
  fLength    = aLength;
}

//______________________________________________________________________________
//  

DLine::~DLine(){
  
  // delete instruction modified to avoid crash, JB 2011/07/25
  delete fOrigin;
  delete fDirection;
  delete fSlope;
}

//______________________________________________________________________________
//  

void DLine::SetValue(const DR3& aOrigin, 
		     const DR3& aDirection, 
		     const DR3& aSlope,
		     const Float_t aLength){
  *fOrigin    = aOrigin;
  *fDirection = aDirection;
  *fSlope     = aSlope;
  fLength     = aLength;
}

//______________________________________________________________________________
//  

void DLine::Zero(){
  fOrigin->Zero();
  fDirection->Zero();
  fSlope->Zero();
  fLength = 0;
}

//______________________________________________________________________________
//  

DR3 DLine::GetPoint(Float_t beta){
  DR3 result;
  result = (*fDirection) * beta;
  return result += *fOrigin; 
}

//______________________________________________________________________________
//   

DR3 DLine::GetIntersectZ(Float_t aZvalue){
  Float_t tDz;
  DR3     tZero;
  tDz = GetDirection()(2);
  if (tDz != 0.)
    return GetPoint(aZvalue/tDz);
  else
    return tZero;
}

//______________________________________________________________________________
//  

Float_t DLine::Distance(DR3 &p){
  // distance d = sqrt(n_k n_k) between point p_k and line G
  // with G is set of r_k :  r_k = a_k x + b_k ; for all x element R and k = 1..3
  // ... now n_i pops out :
  //        n_i = (1_ik - a_i a_k / a^2) (p_k - b_k)    ; 1_ik is kronecker's thing
  // ... which delivers distance d: (note a,b,c,p are 3-Vectors)
  //        d = sqrt(c^2 - 2 (a*c)^2 / a^2 + (a*c)^2)   with c = p - b, 
  // * is the scalar vector product
  // and wire it in:

  DR3  c;
  Float_t d;

  c.SetDifference(p, *fOrigin);
  d = c.InnerProduct(*fDirection);
  
  return sqrt(c.InnerProduct(c) - 
	      2 * d * d / fDirection->InnerProduct(*fDirection) + 
	      d * d);
}











