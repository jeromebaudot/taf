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
  Float_t           *fCoordinate;              //[3] coordinate of the vector
  void               copy(const DR3 &aR3);     // copies coordinate
  
 public:
  DR3();                                 
  DR3(Float_t aX, Float_t aY, Float_t aZ);
  DR3(const DR3& aR3);
  ~DR3();

  virtual void       Print(const Option_t* ="") const ;
  Float_t            Length();
  Float_t            InnerProduct(DR3 &rhs);
  Float_t&           operator()(Int_t aIndex);
  Float_t&           operator()(Int_t aIndex) const;
  DR3&               operator-=(const DR3 &rhs);
  DR3&               operator+=(const DR3 &rhs);
  DR3&               operator/=(const Float_t s);
  DR3&               operator*=(const Float_t s);
  DR3&               operator=(const DR3& rhs);
  DR3                operator*(const Float_t rhs);
  DR3                operator/(const Float_t rhs);
  DR3                operator+(const DR3& rhs);
  DR3                operator-(const DR3& rhs);
  DR3		     ComputeWithSlopeAndDistance(const DR3& slope, const Double_t& distance);//VR 2014/06/29
  void		     Convert2DoubleArray(Double_t *vector3); //VR 2014/06/29
  void               SetDifference(DR3 &v1, DR3 &v2);
  void               SetScale(DR3& vector, const Float_t s);
  void               SetBias(DR3& vector, const Float_t b);
  void               SetValue(Float_t u, Float_t v, Float_t w);
  void               SetValue(const Float_t* value);
  void               SetValue(const Double_t* value);
  void               SetValue(const DR3 &vector);
  void               Zero();

  ClassDef(DR3,2)   // Describes DR3

};

#endif










