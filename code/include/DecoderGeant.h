#ifndef DECODERGeant_H
#define DECODERGeant_H

/* $Id: DecoderGeant.h 0 2014 fb $ */

///////////////////////////////////////////////////////////////////////////////
///
/// This class provides access to MIMOSA 18 in raw data.
///
///////////////////////////////////////////////////////////////////////////////

#include <TObject.h>
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

class DecoderGeant: public TObject {
  public :
    DecoderGeant(Int_t aBoardNumber, Int_t aRunNumber);
    DecoderGeant(const DecoderGeant& rstream);
    DecoderGeant& operator=(const DecoderGeant& rstream);
    ~DecoderGeant() {};

    void    SetInputFile(const char *filename="data.txt"){
      fFileInput = fopen(filename,"r");
      printf("your file is ok!! \n"); };

    Int_t  Get_Nevent(); // n of total event
    Int_t Get_NHitPixel() {return fIndex.size();};
    Int_t GetRow(Int_t iPix) {return fRow[iPix];};
    Int_t Get_NRow() {return fRowPerChip;};
    Int_t GetCol(Int_t iPix) {return fCol[iPix];};
    Int_t Get_NCol() {return fColPerChip;};
    Double_t GetAmp(Int_t iPix) {return fAmp[iPix];};
    Int_t GetIndex(Int_t iPix) {return fIndex[iPix];};
    
    Bool_t ReadData();
    
    Int_t  Get_EvNumber(){return fEvCounterCDH;};// current event number
    
    
    void   SetDebugLevel( int aLevel) { fDebugLevel = aLevel; };
    void   PrintStatistics(ostream &stream);
    Bool_t ReadNextLine();


  private:
    
    Int_t fRowPerChip;
    Int_t fColPerChip;
    void NewEvent();
    
    Int_t fNEvent;
    FILE *fFileInput;
    UInt_t fDataChar;
    Int_t fEvCounterCDH; // counter read in CDH
    Int_t fNHitPixel;
    Int_t fRowtmp[3];
    Int_t fColtmp[3];
    Double_t fAmptmp[3];
    
    Int_t CalculateIndex(int ipixel);
    Bool_t ReadCDH();

    std::vector<int> fRow;
    std::vector<int> fCol;
    std::vector<int> fIndex;
    std::vector<double> fAmp;
    
    Int_t   fBoardNumber;
    Int_t   fRunNumber;
    Bool_t  fFlag;

    Int_t   fDebugLevel;       // debug level
    
    ClassDef(DecoderGeant, 0) // class for reading MIMOSA raw digits

};

#endif