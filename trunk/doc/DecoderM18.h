#ifndef DECODERM18_H
#define DECODERM18_H

/* $Id: DecoderM18.h 0 2014 fb $ */

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

class DecoderM18: public TObject {
  public :
    DecoderM18(Int_t aBoardNumber, Int_t aRunNumber, Int_t aSensorNumber);
    DecoderM18(const DecoderM18& rstream);
    DecoderM18& operator=(const DecoderM18& rstream);
    ~DecoderM18() {};

    void    SetInputFile(char *filename="data.txt"){
      fFileInput = fopen(filename,"rb");
      printf("your file is ok!! .... pay attention--> read option: rb \n"); };

    Int_t  Get_Nevent(); // n of total event
    Int_t Get_NHitPixel() {return fIndex.size();}; 
    Int_t GetRow(Int_t iPix) {return fIndex[iPix]/kRowPerChip;};
    Int_t GetCol(Int_t iPix) {return fIndex[iPix]%kRowPerChip;};
    Int_t GetAmp(Int_t iPix) {return fAmp[iPix];};
    Int_t GetIndex(Int_t iPix) {return fIndex[iPix];};

    Bool_t ReadData();

    Int_t  Get_EvNumber(){return fEvCounterCDH;};// current event number


    void   SetDebugLevel( int aLevel) { fDebugLevel = aLevel; };
    void   PrintStatistics(ostream &stream);
    
 private:

    enum {kRowPerChip=256};
    enum {kColPerChip=256};
    enum {kShift=3};
    enum {kToBeSutractedFromAmp=32768};

    Int_t CalculateIndex();
    void NewEvent();
    Bool_t ReadNextInt(); 
    Bool_t ReadCDH();

    Int_t fNEvent;
    FILE *fFileInput;
    UInt_t fDataChar;
    Int_t fcountEv; //counter incremented when reading CDH
    Int_t fEvCounterCDH; // counter read in CDH


    std::vector<int> fIndex;
    std::vector<int> fAmp;


    Int_t   fBoardNumber;
    Int_t   fRunNumber;
    Int_t   fSensorNumber;


    Int_t   fDebugLevel;       // debug level

    ClassDef(DecoderM18, 0) // class for reading MIMOSA raw digits

};

#endif
