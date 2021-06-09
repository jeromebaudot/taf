// @(#)maf/dtools:$Name:  $:$Id:DPixel.cxx  v.3 2005/10/02 18:03:46 sha Exp $
// Author   : JB, 2008/10/28
// Modified: RDM 060809
// Modified: JB 2009/08/17
// Modified: JB 2010/10/05 signal over noise management
// Modified: JB 2013/06/23 Consructors timestamp info added 
// Modified: LC laste 2014 or early 2015 MonteCarlo info management
// Modified: JB 2015/03/25 Constructors with DAQ index
// Modified: AP 2016/04/15 Constructors with Sim-hit index turning on this pixel

  ////////////////////////////////////////////////////////////
  // Class Description of DPixel                            //
  //                                                        //
  //                                                        //
  ////////////////////////////////////////////////////////////

#include "DPixel.h"
#include "DR3.h"

ClassImp(DPixel) // Description of Single Detector DPixel 
//______________________________________________________________________________
//  
DPixel::DPixel()
{ 
  // DPixel default constructor
}

//______________________________________________________________________________
//  

DPixel::~DPixel()
{
  // DPixel default destructor
  _monteCarloInfo.clear(); 
}

//______________________________________________________________________________
//  
DPixel::DPixel(Int_t aPlaneNumber, const Int_t aPixelIndex, Double_t aValue, Int_t aTime)
{
  // Constructor of a Pixel with index identification.
  //
  // Note that the index is understood as a DAQ or a physical index:
  //   - DAQ index = channel nb from DAQ board (BoardReader),
  //   - physical index = related to (col,row) position with
  //         col = index%(#pixelsInRow),  row = index/(#pixelsInRow)
  //
  // Last modified: JB, 2009/05/03
  // Last modified: JB, 2013/06/23 timestamp info added
  // Last modified: JB, 2015/03/25 DAQ index added
  // Last modified: AP, 2016/04/15 Added HitIdx variable to retrive the MC sim-hit turning on this pixel

  fDebugPixel     =  0;

  fPlaneNumber    =  aPlaneNumber;
  fPixelIndex     =  aPixelIndex;
  fDAQIndex       =  aPixelIndex; 
  fRawValue       =  aValue;
  fTimestamp      =  aTime;

  fPixelLine      = 0; // init, TBD later
  fPixelColumn    = 0; // init, TBD later
  fPosition.SetValue(0, 0, 0);
  fSize.SetValue(0, 0, 0);
  
  fPedestal       = 0.;
  fCommonMode     = 0.;
  fNoise          = 1.e-5; 
  fPulseHeight    = fRawValue; // To be modified later when pedestal is known, JB 2009/05/03   
  fFound          = kFALSE;
  fIfMonteCarlo   = 0;
  fMCHitIdx       = -1;

  if(fDebugPixel>1) printf("DPixel: pixel %d from plane %d with value %f built\n", fPixelIndex, fPlaneNumber, fRawValue);

}
//______________________________________________________________________________
//  
DPixel::DPixel(Int_t aPlaneNumber, Int_t aLine, Int_t aColumn, Double_t aValue, Int_t aTime)
{
  // constructor of a Pixel with column and line 
  //
  // Last modified: JB, 2009/08/17
  // Last modified: JB, 2013/06/23 timestamp info added
  // Last modified: JB, 2015/03/25 DAQ index added

  fDebugPixel     = 0;

  fPlaneNumber    = aPlaneNumber;
  fPixelLine      = aLine;
  fPixelColumn    = aColumn;
  fRawValue       = aValue;
  fTimestamp      = aTime;

  fPixelIndex     = -1; // TBD later
  fDAQIndex       = -1; // TBD later 
  fPosition.SetValue(0, 0, 0);
  fSize.SetValue(0, 0, 0);
  
  fPedestal       = 0.;
  fCommonMode     = 0.;
  fNoise          = 1.e-5; 
  fPulseHeight    = fRawValue; // To be modified later when pedestal is known, JB 2009/05/03   
  fFound          = kFALSE;
  fIfMonteCarlo   = 0;

  if(fDebugPixel>1) printf("DPixel: pixel %d from plane %d with value %f built\n", fPixelIndex, fPlaneNumber, fRawValue);

}
//______________________________________________________________________________

Double_t DPixel::GetPulseHeightToNoise(){

  // Note that the method returns the pulseheight if there is no noise available
  //
  // Modified: JB 2010/10/05

  if (fNoise > 0.5) {
    return fPulseHeight / fNoise;
  }
  else  {
    return fPulseHeight;
  }
}
//______________________________________________________________________________
//  
Double_t DPixel::Distance(DPixel &aPixel)
{
  return Distance(aPixel.GetPosition());
}

//______________________________________________________________________________
//  
Double_t DPixel::Distance(const DR3& aPosition)
{
  DR3 result(fPosition);
  result -= aPosition; 
  return result.Length();
}

//______________________________________________________________________________
//
Double_t DPixel::DistanceU(DPixel &aPixel)
{
  return DistanceU(aPixel.GetPosition());
}

//______________________________________________________________________________
//  
Double_t DPixel::DistanceU(const DR3& aPosition)
{
  DR3 result(fPosition);
  result -= aPosition; 
  return result(0);
}

//______________________________________________________________________________
//
Double_t DPixel::DistanceV(DPixel &aPixel)
{
  return DistanceV(aPixel.GetPosition());
}

//______________________________________________________________________________
//  
Double_t DPixel::DistanceV(const DR3& aPosition)
{
  DR3 result(fPosition);
  result -= aPosition; 
  return result(1);
}

//______________________________________________________________________________
//  
void DPixel::SetMonteCarlo( Double_t aMC_X, Double_t aMC_Y, Double_t aMC_Z, Int_t row, Int_t column)
{
  if(fDebugPixel) std::cout<<"Setting MonteCarlo parameters"<<std::endl;

  fIfMonteCarlo   = 1;
  
  if(_monteCarloInfo.size()==0) {
    _monteCarloInfo.push_back(aMC_X);
    _monteCarloInfo.push_back(aMC_Y);
    _monteCarloInfo.push_back(aMC_Z);
    _monteCarloInfo.push_back(row);
    _monteCarloInfo.push_back(column);
  } 
  else {
    _monteCarloInfo[0] = aMC_X;
    _monteCarloInfo[1] = aMC_Y;
    _monteCarloInfo[2] = aMC_Z;
    _monteCarloInfo[3] = row;
    _monteCarloInfo[4] = column;
  }
  
  if(fDebugPixel) printf("DPixel::SetMonteCarlo : Plane %d \n", fPlaneNumber);

}

//______________________________________________________________________________
//  
DR3 DPixel::GetMonteCarloPosition()
{
  if(fIfMonteCarlo==1 && _monteCarloInfo.size()>=3 ) { return DR3( _monteCarloInfo[0], _monteCarloInfo[1], _monteCarloInfo[2] ); }
  else { 
    std::cout<<"Monte Carlo Informations Disable --> To enable these option set HitMonteCarlo: 1 in the config file."<<std::endl;
    return DR3(0,0,0);
  }
}

