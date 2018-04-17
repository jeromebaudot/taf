// @(#)maf/dtools:$Name:  $:$Id: DModule.cxx,v.1 2005/10/02 18:03:46 sha Exp $
// Author :  Dirk Meier     98/03/22


  ////////////////////////////////////////////////////////////////////////////////
  //                                                                            //
  // Class Description of DModule                                               //
  //                                                                            //
  // + DModule, may be used in future (>980322) to replace parts of DAcqModule  //
  //                                                                            //
  ////////////////////////////////////////////////////////////////////////////////


//*-- Modified :  
//*-- Copyright:  RD42
//*-- Author   :  Dirk Meier   980322
//*KEEP,CopyRight.
/************************************************************************
 * Copyright(c) 1997, RD42@CERN, DiamondTracking.
 ***********************************************************************/
//*KEND.

#include "Riostream.h"

// DT classes
//*KEEP,DModule.
#include "DModule.h"

//*KEND.

ClassImp(DModule) // Description of DModule 

//______________________________________________________________________________
//  
DModule::DModule()
{ 
  // DModule default constructor
}

//______________________________________________________________________________
//  
DModule::~DModule()
{ 
  // DModule default destructor
}

//______________________________________________________________________________
//  
DModule::DModule(Int_t aValuesN)
{
  fIsConnected = kFALSE;
  fValuesN     = aValuesN;
  
  fData        = new TArrayF(fValuesN); //Float_t[fValuesN];
}

//______________________________________________________________________________
//  
void DModule::SetCodeTable(const Char_t *aLocation, 
			   Int_t   aCodingValuesN, 
			   Int_t   aCodingBitsN,
			   Int_t   aSplitInputsN,
			   Int_t   aSplitInput) 
{
  fLocation      = new TArrayC(aCodingValuesN); // first create it with new operator
  fLocation->Adopt(aCodingValuesN, aLocation);  // then adopt the *Char_t
  fCodingValuesN = aCodingValuesN;
  fSplitInputsN  = aSplitInputsN;
  fSplitInput    = aSplitInput;
  fCodingBitsN   = aCodingBitsN;
  fIsConnected   = kTRUE;
}

//______________________________________________________________________________
//  
void DModule::Update()
{
  Int_t pp;
  if(fIsConnected)
    for(Int_t tINP = 1; tINP <= fSplitInputsN; tINP++)
      for(Int_t k = 1; k <= fCodingValuesN; k++) {
	pp = ( (k-1) * fSplitInputsN + tINP-1 ) * fCodingBitsN/8;
	//	fData[k-1] = theValue(fLocation->GetArray() + pp, fCodingBitsN/8);
	fData->AddAt(theValue(fLocation->GetArray() + pp, fCodingBitsN/8), k-1);
      }
  else
    printf(" DModule, not connected \n");
}

//______________________________________________________________________________
//  
void DModule::PrintDataToTerm()
{
  if(fIsConnected)
    printf(" DModule, is connected, printing not implemented yet\n");
  else
    printf(" DModule, not connected \n");
}

//______________________________________________________________________________
//  
Int_t DModule::theValue(const Char_t* aBufferPoint, Int_t width)
{
  
  // Converts the contens of buffer to a value
  //  e.g.  *aBufferPoint = (0A 14 FA 3C ... ), 
  // and width = 2  gives value = 2580
  
  Int_t v   = 0;
  Int_t pot = 1;
  for (Int_t k = 1; k <= width; k++){
    v = v + pot * (0x00FF & aBufferPoint[width - k]);
    pot   = 0x100 * pot;
  }
  return v;
}








