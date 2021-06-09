// @(#)maf/dtools:$Name:  $:$Id: DPrecAlign.h,v.1 2005/10/02 18:03:46 sha Exp $
//  Author   :  G.Orazi 1/2/2000
//  LastModified: LC, 2014/12/20 : Now mini-vectors saved in DLadder with MiniVector Objects.
//  LastModified: LC, New DPrecAlign Mecanism with good matrices definitions. (Before it was the transpose matrices)
//  LastModified: AP, 2015/03/09: Adding spatial resolution to DataPoints class

#ifndef _DPrecAlign_included_
#define _DPrecAlign_included_

// ROOT classes
#include "TObject.h"
#include "TClass.h"
#include "TList.h"
#include "TBuffer.h"
#include "DR3.h"
#include "DGlobalTools.h"
#include "DTrack.h"

class DataPoints : public TObject {
 public:
  DataPoints() {;}
  DataPoints(Double_t auh,Double_t avh,
	     Double_t aresU,Double_t aresV,
	     Double_t atx,Double_t aty,Double_t atz,Double_t atdx, Double_t atdy);
  virtual ~DataPoints();
  void Set(Double_t auh,Double_t avh,
	   Double_t aresU,Double_t aresV,
	   Double_t atx,Double_t aty,Double_t atz,Double_t atdx, Double_t atdy);
  void PrintData();

  DR3 GetHitPosition();
  DR3 GetHitResolution();
  DR3 GetTrackOrigin();
  DR3 GetTrackDirection();

 private:

  Double_t _uh; // point in local system (hit position)
  Double_t _vh;
  
  //AP, 2015/03/09. Adding spatial resolution to DataPoint
  Double_t _resU; // spatial point resolution
  Double_t _resV;
 
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
  Int_t    _fIfDeformation;            // Flag to model or not deformation, JB 2015/10/31
  Double_t _fUDeformationCoef[8];     // Coeff. for U-deformation model, JB 2015/10/31
  Double_t _fVDeformationCoef[8];     // Coeff. for V-deformation model, JB 2015/10/31

  
  TList   _data;       // List of the points: u,v and x,y,z,dx/dz,dy/dz
  TList   _data1;
  
  Double_t _xh;          //
  Double_t _yh;          //  track point in xyz system at plane
  Double_t _zh;          // 
  Double_t _xtd;         //
  Double_t _ytd;         //  track slope in xyz system at plane, JB 2015/10/31
  Double_t _ztd;         // 
  Double_t _initialRotations[3];
  Double_t _initialPosition[3];

  DGlobalTools  fTool;                    // JB 2015/10/31

  Int_t DPrecAlignMethod;

public:
  DPrecAlign();
  DPrecAlign(Int_t method);
  DPrecAlign(const DPrecAlign &aobj);                                 
  virtual  ~DPrecAlign();
 
  void     NewData(Double_t auh,Double_t avh,
		   Double_t aresU,Double_t aresV,
		   Double_t atx,Double_t aty,Double_t atz, Double_t atdx, Double_t atdy);
  void     NewData(DataPoints* myDataPoint);
  void     NewData1(DataPoints* myDataPoint);
  void     RemoveData(DataPoints* myDataPoint);  // LC 2013/10/02
  void     RemoveData1(DataPoints* myDataPoint); // LC 2013/10/02
  
  void     ListDataPoints();
  Int_t    DataSize(); // LC 2012/09/06.
  Int_t    DataSize1(); // LC 2013/09/10
  
  void     ResetDataPoints()  {_data.Delete();} // SS, 2012/09/05
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
  void     SetDeformation( Double_t *coeffU, Double_t *coeffV=NULL); // JB 2015/10/31
  
  void     PrintAll();
  void     PrintRotationMatrix();
  void     PrintTorMatrix();
  void     PrintTranslations();
  void     PrintRotTimeInv();
  void     PrintDeformations();
  DR3      RotateToPlane(DR3 xxx) ;
  DR3      RotateToTracker( Double_t u, Double_t v, Double_t w) ;
  DR3      RotateToTracker(DR3 uuu) ;
  DR3      TransformHitToTracker(DR3 uuu); // JB 2010/01/03
  DR3      TransformHitToPlane(DR3 xxx); // JB 2010/11/25
  DR3      TransformTrackToPlane();
  DR3      TransformTrackToPlane( Double_t tx, Double_t ty, Double_t tz, Double_t tdx, Double_t tdy);
  void     SetTrackPosition( DR3 xxx); // JB 2010/11/25
  DR3      GetTrackPosition();
  DR3      GetTrackSlope();
  void     ConvoluteRotation( Double_t tiltW); // JB 2010/11/25
  void     ConvoluteRotation( DR3 aTilt); // JB 2014/02/16
  void     ConvoluteUVWAlignment( Double_t u, Double_t v, Double_t tiltW); // JB 2010/11/25
  void     ConvoluteAlignment( DPrecAlign *refAlignment); // JB 2010/11/25
  void     DecomposeRotations(); // JB 2011/04/04
  void     DeconvoluteAlignment( DPrecAlign *refAlignment); // JB 2014/11/24
  DR3      DeformedLocalPoint(DR3& aPlaneCoordinate); // JB 2015/10/31
  
  DR3      GetCoeffs() { return DR3(_Acoeff, _Bcoeff, _Ccoeff); } // LC 2014/12/20 
  Double_t*   GetRotations()    {return _fTh ;}
  Double_t*   GetTranslation()  {return _fTr ;}
  DR3         GetRotationsDR3() { return DR3(_fTh[0],_fTh[1],_fTh[2]); }
  DR3         GetTranslationsDR3() {return DR3(_fTr[0], _fTr[1], _fTr[2]); }    
  Double_t*   GetRotationMatrix()    {return &_rotmat[0][0] ;}
  Double_t*   GetTorationMatrix()    {return &_tormat[0][0] ;}
  
  TList*      GetDataPoints()   {return &_data ;}
  TList*      GetDataPoints1()   {return &_data1 ;}
  
  Double_t*   GetInitialRotations() {return _initialRotations;} 
  void        SetInitialRotations();
  
  Double_t*   GetInitialPosition() {return _initialPosition;}
  void        SetInitialPosition();

  void  SetDebug(Int_t aDebug){fDebugDPrecAlign = aDebug;}
  Int_t GetDebug()    { return fDebugDPrecAlign;}

  void        SetDPrecAlignMethod(Int_t method) { DPrecAlignMethod=method; } // LC 2015/01/31
  Int_t       GetDPrecAlignMethod() { return DPrecAlignMethod; }             // LC 2015/01/31

  ClassDef(DPrecAlign,2) // DPrecAlign
};

#endif
