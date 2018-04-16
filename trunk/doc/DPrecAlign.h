// @(#)maf/dtools:$Name:  $:$Id: DPrecAlign.h,v.1 2005/10/02 18:03:46 sha Exp $
//  Author   :  G.Orazi 1/2/2000

#ifndef _DPrecAlign_included_
#define _DPrecAlign_included_


// ROOT classes
#include "TObject.h"
#include "TClass.h"
#include "TList.h"
#include "DR3.h"

class DataPoints : public TObject {
 public:
  DataPoints() {;}
  DataPoints(Float_t auh,Float_t avh,Float_t atx,Float_t aty,Float_t atz,Float_t atdx, Float_t atdy);
  virtual ~DataPoints();
  void Set(Float_t auh,Float_t avh,Float_t atx,Float_t aty,Float_t atz,Float_t atdx, Float_t atdy);
  void PrintData();

  DR3 GetHitPosition();
  DR3 GetTrackOrigin();
  DR3 GetTrackDirection();
 private:
  Double_t _uh; // point in local system (hit position)
  Double_t _vh;
 
  Double_t _tx;  //  -
  Double_t _ty;  //   | Track origin
  Double_t _tz;  //  - 
  Double_t _tdx; //  \  Track Direction
  Double_t _tdy; //  /

 ClassDef(DataPoints,1) 
};

//______________________________________________________________________________
//  

class DPrecAlign : public TObject  {
 private: 
  Int_t        fDebugDPrecAlign;  // DPrecAlign debug flag
  Double_t _alpha[3][3] ; // rotation around z
  Double_t _beta[3][3] ;  // rotation around y'
  Double_t _gamma[3][3] ; // rotation around x"
  Double_t _rotmat[3][3] ;//  gamma*beta*alpha
  Double_t _ahpla[3][3] ; // inverse rotation around z
  Double_t _ateb[3][3] ;  // inverse rotation around y'
  Double_t _ammag[3][3] ; // inverse rotation around x"
  Double_t _tormat[3][3] ;//  inverse rotation matrix
  Double_t _Acoeff;            // 
  Double_t _Bcoeff;            //  Parameters of the rotated plane
  Double_t _Ccoeff;            //   (name changed, JB 2011/10/27)
  Double_t _fTh[3] ;      // Rotations  0=z 1=y 2=x   R(tracker->Plan)=Rz.Ry.Rx 
  Double_t _fTr[3] ;      // Translations 
  TList   _data;       // List of the points: u,v and x,y,z,dx/dz,dy/dz
  TList   _data1;

  Double_t _xh;          //
  Double_t _yh;          //  track point in xyz system at plane
  Double_t _zh;          // 
  Double_t _initialRotations[3];
  Double_t _initialPosition[3];

public:
  DPrecAlign();                                 
  DPrecAlign(const DPrecAlign &aobj);                                 
  virtual  ~DPrecAlign();
 
  void     NewData(Float_t auh,Float_t avh,Float_t atx,Float_t aty,Float_t atz, Float_t atdx, Float_t atdy);
  void     NewData(DataPoints* myDataPoint);
  void     NewData1(DataPoints* myDataPoint);
  void     ListDataPoints() ;
  Int_t    DataSize(); // LC 2012/09/06.
  Int_t    DataSize1(); // LC 2013/09/10
  void     ResetDataPoints() {_data.Delete();} // SS, 2012/09/05
  void     ResetDataPoints1() {_data1.Delete();}
  
  void     CopyAlignment( DPrecAlign *anAlign); // JB 2014/02/17
  void     SetRotations(Double_t th0,Double_t th1,Double_t th2 ) ;
  void     SetRotations(DR3 tilt ) ; // JB 2010/11/25
  void     ComputeRotationMatrix() ;
  void     ComputeTorationMatrix() ; // JB 2010/01/03
  void     SetTranslation(Double_t t0,Double_t t1,Double_t t2 ) ;
  void     SetTranslation(DR3 tr ) ; // JB 2010/11/25
  void     CalculatePlane();
  void     CalculateIntersection(DataPoints* aDataPoint);
  //DR3&     RotateMV(DataMiniVectors* aMiniVector);
  void     PrintAll();
  void     PrintRotationMatrix();
  void     PrintTranslations();
  DR3      RotateToPlane(DR3 xxx) ;
  DR3      RotateToTracker( Double_t u, Double_t v, Double_t w) ;
  DR3      RotateToTracker(DR3 uuu) ;
  DR3      TransformHitToTracker(DR3 uuu); // JB 2010/01/03
  DR3      TransformHitToPlane(DR3 xxx); // JB 2010/11/25
  DR3      TransformTrackToPlane();
  DR3      TransformTrackToPlane( Double_t tx, Double_t ty, Double_t tz, Double_t tdx, Double_t tdy);
  void     SetTrackPosition( DR3 xxx); // JB 2010/11/25
  DR3      GetTrackPosition();
  void     ConvoluteRotation( Double_t tiltW); // JB 2010/11/25
  void     ConvoluteRotation( DR3 aTilt); // JB 2014/02/16
  void     ConvoluteUVWAlignment( Double_t u, Double_t v, Double_t tiltW); // JB 2010/11/25
  void     ConvoluteAlignment( DPrecAlign *refAlignment); // JB 2010/11/25
  void     DecomposeRotations(); // JB 2011/04/04

  Double_t*   GetRotations()    {return _fTh ;}
  Double_t*   GetTranslation()  {return _fTr ;}
  Double_t*   GetRotationMatrix()    {return (Double_t*)_rotmat ;}
  Double_t*   GetTorationMatrix()    {return (Double_t*)_tormat ;}
  TList*      GetDataPoints()   {return &_data ;}
  TList*      GetDataPoints1()   {return &_data1 ;}
  Double_t*   GetInitialRotations() {return _initialRotations;} 
  void        SetInitialRotations();
  
  Double_t*   GetInitialPosition() {return _initialPosition;}
  void        SetInitialPosition();

  void  SetDebug(Int_t aDebug){fDebugDPrecAlign = aDebug;}
  Int_t GetDebug()    { return fDebugDPrecAlign;}


  ClassDef(DPrecAlign,2) // DPrecAlign

};

#endif
