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

    void    SetInputFile(const char *filename="data.txt"){
      fFileInput = fopen(filename,"rb");
      if( fFileInput) {
        printf("your file %s is ok!! .... pay attention--> read option: rb \n", filename); 
      }
      else {
        printf("WARNING: your file %s cannot be opened!!\n", filename);         
      }
    };

    Int_t Get_Nevent(); // n of total event
    Int_t Get_NHitPixel() {return fIndex.size();}; 
    Int_t GetRow(Int_t iPix) {return fIndex[iPix]/kRowPerChip;};
    Int_t GetCol(Int_t iPix) {return fIndex[iPix]%kRowPerChip;};
    Int_t GetAmp(Int_t iPix) {return fAmp[iPix];};
    Int_t GetIndex(Int_t iPix) {return fIndex[iPix];};
    Int_t GetOMKDTransition() {return kOMKDTransitionRead;};
    Int_t GetStopPointerTransition() {return kStopPointerTransitionRead;};
  
    Bool_t ReadData( );

    Int_t  Get_EvNumber(){return fEvCounterCDH;};// current event number

  void   SetShift( Int_t aShift);
  void   SetOffset( Int_t aOffset);
  void   SetMulFactor( Int_t aFactor);
  void   SetTrailer( UInt_t aTrailer);
  void   SetOMKDTransition( Int_t aTransition ) { kOMKDTransitionSet = aTransition; };
  void   SetStopPointerTransition( Int_t bTransition ) { kStopPointerTransitionSet = bTransition; };
  
    void   SetDebugLevel( int aLevel) { fDebugLevel = aLevel; };
    void   PrintStatistics(ostream &stream);
    
 private:

    enum {kRowPerChip=256};
    enum {kColPerChip=256};
//    enum {kShift=3};
//    enum {kToBeSutractedFromAmp=32768};
//    enum {kTrailer=0xfafafafa};

    Int_t kShift;
    Int_t kToBeSutractedFromAmp;
    Int_t kAmpMultFactor;
    UInt_t kTrailer;
    Int_t kOMKDTransitionRead;
    Int_t kOMKDTransitionSet;
    Int_t kStopPointerTransitionRead;
    Int_t kStopPointerTransitionSet;
  
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
