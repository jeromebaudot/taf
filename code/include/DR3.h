// @(#)maf/dtools:$Name:  $:$Id: DR3.h,v.2 2005/10/02 18:03:46 sha Exp $
//*-- Author   :  Dirk Meier   98/01/09

#ifndef _DR3_included_
#define _DR3_included_

  /////////////////////////////////////////////////////////////////////////
  // Class Description of DR3                                            //
  //                                                                     //
  // this is a basic vector class, restricted to 3 dimensional vectors   //
  // could be replaced later by more general class                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////



#include <math.h>

// ROOT classes
#include "TObject.h"


class DR3 : public TObject {

 private:
  Double_t           *fCoordinate;              //[3] coordinate of the vector
  void               copy(const DR3 &aR3);     // copies coordinate
  
 public:
  DR3();                                 
  DR3(Double_t aX, Double_t aY, Double_t aZ);
  DR3(const DR3& aR3);
  ~DR3();

  virtual void       Print(const Option_t* ="") const ;
  Double_t            Length();
  Double_t            InnerProduct(DR3 &rhs);
  Double_t&           operator()(Int_t aIndex);
  Double_t&           operator()(Int_t aIndex) const;
  DR3&               operator-=(const DR3 &rhs);
  DR3&               operator+=(const DR3 &rhs);
  DR3&               operator/=(const Double_t s);
  DR3&               operator*=(const Double_t s);
  DR3&               operator=(const DR3& rhs);
  DR3                operator*(const Double_t rhs);
  DR3                operator/(const Double_t rhs);
  DR3                operator+(const DR3& rhs);
  DR3                operator-(const DR3& rhs);
  DR3		     ComputeWithSlopeAndDistance(const DR3& slope, const Double_t& distance);//VR 2014/06/29
  void		     Convert2DoubleArray(Double_t *vector3); //VR 2014/06/29
  void               SetDifference(DR3 &v1, DR3 &v2);
  void               SetScale(DR3& vector, const Double_t s);
  void               SetBias(DR3& vector, const Double_t b);
  void               SetValue(Double_t u, Double_t v, Double_t w);
  void               SetValue(const Double_t* value);
  void               SetValue(const Float_t* value);
  void               SetValue(const DR3 &vector);
  void               Zero();

  ClassDef(DR3,2)   // Describes DR3

};

#endif










