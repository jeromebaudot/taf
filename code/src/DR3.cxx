// @(#)maf/dtools:$Name:  $:$Id: DR3.cxx v.2 2005/10/02 18:03:46 sha Exp $
//*-- Author   :  Dirk Meier   98/01/09

  /////////////////////////////////////////////////////////////////////////
  // Class Description of DR3                                            //
  //                                                                     //
  // this is a basic vector class, restricted to 3 dimensional vectors   //
  // could be replaced later by more general class                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////


//*-- Modified :  
// Last Modified: VR 2014/06/29 Add Convert2DoubleArray() and ComputeWithSlopeAndDistance methods and  SetValue(const Double_t)

//*-- Copyright:  RD42


//*KEEP,CopyRight.
/************************************************************************
 * Copyright(c) 1997, DiamonTracking
 ***********************************************************************/

#include "Riostream.h"

//*KEEP,DR3.
#include "DR3.h"

//*KEND.

ClassImp(DR3) // Description of a Vector in 3 dim. space, R^3

//______________________________________________________________________________
//  
DR3::DR3()
{
  fCoordinate = new Double_t[3];
}

//______________________________________________________________________________
//  
DR3::DR3(const DR3 &aR3)
{
  fCoordinate = new Double_t[3];
  copy(aR3);
}

//______________________________________________________________________________
//  
DR3::~DR3()
{ 
  delete [] fCoordinate;
}

//______________________________________________________________________________
//  

void DR3::copy(const DR3 &aR3)
{
  fCoordinate[0] = aR3(0);
  fCoordinate[1] = aR3(1);
  fCoordinate[2] = aR3(2);
}

//______________________________________________________________________________
//  
DR3::DR3(Double_t aX, Double_t aY, Double_t aZ)
{
  fCoordinate = new Double_t[3];
  fCoordinate[0] = aX;
  fCoordinate[1] = aY;
  fCoordinate[2] = aZ;
}

//______________________________________________________________________________
//  
void  DR3::Convert2DoubleArray(Double_t *vector3)
{
  vector3[0]=fCoordinate[0];
  vector3[1]=fCoordinate[1];
  vector3[2]=fCoordinate[2];
  
}
//______________________________________________________________________________
//  
void DR3::Print(const Option_t*) const
{ 
  printf("DR3: %f, %f, %f\n",fCoordinate[0],fCoordinate[1],fCoordinate[2]);
  
}

//______________________________________________________________________________
//  
void DR3::Zero()
{
  fCoordinate[0] = 0.;
  fCoordinate[1] = 0.;
  fCoordinate[2] = 0.;
}

//______________________________________________________________________________
//  
Double_t DR3::Length()
{
  return sqrt(fCoordinate[0] * fCoordinate[0] +
	      fCoordinate[1] * fCoordinate[1] +
	      fCoordinate[2] * fCoordinate[2]);
}

//______________________________________________________________________________
//  
void DR3::SetDifference(DR3 &a, DR3 &b)
{
  fCoordinate[0] = a(0) - b(0);
  fCoordinate[1] = a(1) - b(1);
  fCoordinate[2] = a(2) - b(2);
}

//______________________________________________________________________________
//  
Double_t DR3::InnerProduct(DR3 &a)
{
  return a(0) * fCoordinate[0] + a(1) * fCoordinate[1] + a(2) * fCoordinate[2];
}

//______________________________________________________________________________
//  
void DR3::SetScale(DR3 &a, const Double_t s)
{
  fCoordinate[0] = a(0) * s;
  fCoordinate[1] = a(1) * s;
  fCoordinate[2] = a(2) * s;
}

//______________________________________________________________________________
//  
void DR3::SetBias(DR3 &a, const Double_t b)
{
  fCoordinate[0] = a(0) + b;
  fCoordinate[1] = a(1) + b;
  fCoordinate[2] = a(2) + b;
}

//______________________________________________________________________________
//  
void DR3::SetValue(Double_t u, Double_t v, Double_t w)
{
  fCoordinate[0] = u;
  fCoordinate[1] = v;
  fCoordinate[2] = w;
}

//______________________________________________________________________________
//  
void DR3::SetValue(const Float_t* value)
{
  fCoordinate[0] = value[0];
  fCoordinate[1] = value[1];
  fCoordinate[2] = value[2];
}

//______________________________________________________________________________
//  
void DR3::SetValue(const Double_t* value)
{
  //VR 2014/06/29
  fCoordinate[0] = (Double_t)value[0];
  fCoordinate[1] = (Double_t)value[1];
  fCoordinate[2] = (Double_t)value[2];
}

//______________________________________________________________________________
//  
void DR3::SetValue(const DR3 &a){
  fCoordinate[0] = a(0);
  fCoordinate[1] = a(1);
  fCoordinate[2] = a(2);
}

//______________________________________________________________________________
//  
Double_t& DR3::operator()(Int_t aIndex)
{
  return (Double_t&)((*(const DR3 *)this)(aIndex));
  //return fCoordinate[aIndex];
}

//______________________________________________________________________________
//  
Double_t& DR3::operator()(Int_t aIndex) const
{
  return fCoordinate[aIndex];
}

//______________________________________________________________________________
//  
DR3& DR3::operator-=(const DR3 &rhs)
{
  fCoordinate[0] -= rhs(0);
  fCoordinate[1] -= rhs(1);
  fCoordinate[2] -= rhs(2);
  return *this;
}

//______________________________________________________________________________
//  
DR3& DR3::operator+=(const DR3 &rhs)
{
  fCoordinate[0] += rhs(0);
  fCoordinate[1] += rhs(1);
  fCoordinate[2] += rhs(2);
  return *this;
}

//______________________________________________________________________________
//  
DR3& DR3::operator/=(const Double_t s)
{
  if (s != 0.0) {
    fCoordinate[0] /= s;
    fCoordinate[1] /= s;
    fCoordinate[2] /= s;
  }
  return *this;
}

//______________________________________________________________________________
//  
DR3& DR3::operator*=(const Double_t s)
{
  fCoordinate[0] *= s;
  fCoordinate[1] *= s;
  fCoordinate[2] *= s;

  return *this;
}

//______________________________________________________________________________
//  
DR3& DR3::operator=(const DR3& rhs)
{
  SetValue(rhs);
  return *this;
}

//______________________________________________________________________________
//  
DR3 DR3::operator*(const Double_t rhs)
{
  DR3 result(*this);
  result *= rhs;
  return result;
}

//______________________________________________________________________________
//  
DR3 DR3::operator/(const Double_t rhs)
{
  DR3 result(*this);
  result /= rhs;
  return result;
}

//______________________________________________________________________________
//  
DR3 DR3::operator+(const DR3& rhs)
{
  DR3 result(*this);
  result += rhs;
  return result;
}

//______________________________________________________________________________
//  
DR3 DR3::operator-(const DR3& rhs)
{
  DR3 result(*this);
  result -= rhs;
  return result;
}

//______________________________________________________________________________
// 
DR3	DR3::ComputeWithSlopeAndDistance(const DR3& slope, const Double_t& distance)
{
  // Compute a point position from a start point, a slope, and a distance
  DR3 result;
  result(0) = (*(const DR3 *)this)(0) + slope(0)*distance;
  result(1) = (*(const DR3 *)this)(1) + slope(1)*distance;
  result(2) = (*(const DR3 *)this)(2) + slope(2)*distance;
  
  return result;
}
