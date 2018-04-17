#ifndef DecoderM18_fb_H
#define DecoderM18_fb_H

/* $Id: DecoderM18_fb.h 0 2014 fb $ */

///////////////////////////////////////////////////////////////////////////////
///
/// This class provides access to MIMOSA 18 in raw data.
///
///////////////////////////////////////////////////////////////////////////////

#include "Riostream.h"
#include "TObject.h"
#include "BoardReader.h"

class DecoderM18_fb: public TObject {
  public :
    DecoderM18_fb( Int_t aBoardNumber, Int_t aRunNumber, Int_t aSensorNumber);
    DecoderM18_fb(const DecoderM18_fb& rstream);
    DecoderM18_fb& operator=(const DecoderM18_fb& rstream);
    ~DecoderM18_fb() {};

    void   SetDebugLevel( int aLevel) { fDebugLevel = aLevel; }
    BoardReaderEvent*   GetEvent() { return fCurrentEvent; }
    void   PrintStatistics(ostream &stream);
    Bool_t HasData();
    Bool_t ReadCDH();
    Int_t  Get_NeventTOT(){return fCount;}; // n of total event
    Int_t  Get_Nevent(); // n of total event
    
    Int_t  Get_Event_Info();  // info on the n evt
    Int_t  Read_Evento(Int_t evt);  // info on the n evt
    Bool_t  GetPixeltrs(unsigned int WORD, Int_t FCOL, Int_t  FROW, Int_t FAMP, Int_t FEVENT);
    UInt_t GetEventNumber(){ return fEvent2;};
    UInt_t GetRow(){   return frow;};
    UInt_t GetCol(){   return fcol;};
    UInt_t GetAmp(){   return famp;};
    UInt_t GetPixel() {
        for(Int_t fb=0; fb <=1000; fb++)
            {
                //if(PIXEL[fb]!=0x2bc){
                    UInt_t word_simple=0;
                    word_simple=PIXEL[fb];
                    printf("CURRENT PIXEL :--->  %x \n",word_simple);
                    //Trasf(word_simple);
                    //}
            }
        return 0;
    }
    UInt_t GetPixel_i(Int_t i)      { return PIXEL[i];}
    Int_t GetPixel_i_index(Int_t i);
    Int_t GetPixel_i_row(Int_t i)   {  return PIXEL[i] & 0x000000FF; }
    Int_t GetPixel_i_col(Int_t i)   {  return (PIXEL[i] & 0x0000FF00) >> 8; }
    Int_t GetPixel_i_value(Int_t i) {  return (PIXEL[i] & 0xFFFF0000) >> 16; }

    
    
    void Trasf(UInt_t parola){
        Int_t froww =  parola & 0x000000FF;
        Int_t fcolw = (parola & 0x0000FF00) >> 8;
        Int_t fampw = (parola & 0xFFFF0000) >> 16;
        printf("...pixel info: -------------------->  %d \n", froww);
        printf("...pixel info: -------------------->  %d \n", fcolw);
        printf("...pixel info: -------------------->  %d \n", fampw);
        }
    Int_t GetIndexMax(){  return Index_Max;};
    //UInt_t GetRowF(){   return froww;};
    //UInt_t GetColF(){   return fcolw;};
    //UInt_t GetAmpF(){   return fampw;};
    
    UInt_t GetPixel1(){   return PIXEL[0];}
    UInt_t GetPixel2(){   return PIXEL[1];}
    UInt_t GetPixel3(){   return PIXEL[2];}

    //UInt_t  ReadCDH();
    //Bool_t  ReadData();
    Bool_t  ReadData();
    
    void    SetInputFile(const char *filename="data.txt"){
      fFileInput = NULL;
      fFileInput = fopen(filename,"rb");
      if( fFileInput && ReadCDH() ) printf("your file %s is ok!! .... pay attention--> read option: rb \n", filename);
      else printf("ERROR cannot open file %s !!\n", filename);};

    UInt_t   GetEventCounterCDH() {return fEventCounterCDH;};    // get last read event counter value from CDH
    UInt_t   GetWordCounterCDH() {return fWordCounterCDH;};     // get last read word counter value from CDH
    Bool_t   IsLastEvent() {return fLastEvt;};
    Bool_t   IsRunWithTrigger() {return fTrigger;};
    Int_t    GetMimosaModRead() {return fModeRead;};
    UInt_t   GetMimosaFrameType() {return fMimosaMode;};
    Int_t    GetChipNumber() {return fChipNbr;};   
    Int_t    GetNbrChipActive() {return fNbrChip;};            // get number of chip in data-taking, value from CDH 
    UInt_t   GetEventCounterLH() {return fEventCounterLH;};    // get last read event counter value from CDH
    UInt_t   GetWordCounterLH()  {return fWordCounterLH;};     // get last read word counter value from CDH
    UInt_t   GetExternalTriggerCounter()  {return fTriggerExternalCounter;};
    Bool_t   IsChipActive(Int_t key){
              if(key<0 || key>3){
                printf("IsChipActive: you asked for no existing chip\n");
                return 0;}
              else{
                return fChipKey[key];} };
    Short_t  GetADCCounts(Int_t row, Int_t col){
              if(row<0 || row>63 || col<0 || col>15){
                printf("GetADCCounts1: you asked for no existing pixel\n"); 
                return -1;}
              else{
                return fDataFrame[row][col];} };
    Short_t  GetADCCounts2(Int_t row, Int_t col){
              if(row<0 || row>63 || col<0 || col>15){
                printf("GetADCCounts2: you asked for no existing pixel\n");
                return -1;}
              else{
                 return fDataFrame2[row][col];} };
    Short_t  GetADCCounts3(Int_t row, Int_t col){
              if(row<0 || row>63 || col<0 || col>15){
                printf("GetADCCounts3: you asked for no existing pixel\n");
                return -1;}
              else{
                 return fDataFrame3[row][col];} };

    
    
    
    
    enum {kDDLsNumber = 20};      // number of DDLs  
    enum {kChipsPerDDL = 4};   // number of chips in each DDL
    enum {kRowPerChip = 64};   // number of rows per chip
    enum {kColPerChip = 16};   //number of columns per chip

  private :
    
    Int_t   fDebugLevel;       // debug level

    Int_t   fBoardNumber;
    Int_t   fRunNumber;
    Int_t   fSensorNumber;

    BoardReaderEvent *fCurrentEvent;
    std::vector<BoardReaderPixel> ListOfPixels;

    //Bool_t      ReadNextInt();
    UInt_t      ReadNextInt();
    
    void        NewEvent();
    FILE        *fFileInput;
    
    UInt_t      fDataChar1;
    UInt_t      fDataChar2;
    UInt_t      fDataChar3;
    UInt_t      fDataChar4; // temps part of a 32bit word
    UInt_t      fEventCounterCDH;   // event number read by CDH
    UInt_t      fWordCounterCDH;   // number of words of the event from CDH
    UInt_t      fTriggerExternalCounter;
    Bool_t      fLastEvt;   // check if it is the last event
    Bool_t      fTrigger;
    Bool_t      fActiveChip[kChipsPerDDL];
    Bool_t      fChipKey[kChipsPerDDL];

    Int_t       fModeRead;
    UInt_t      fMimosaMode;
    Int_t       fChipNbr;
    Int_t       fNbrChip;
    UInt_t      fEventCounterLH;   // event number read by LH
    UInt_t      fWordCounterLH;   // number of words of the event from LH

    Short_t     fDataFrame[kRowPerChip][kColPerChip];
    Short_t     fDataFrame2[kRowPerChip][kColPerChip];
    Short_t     fDataFrame3[kRowPerChip][kColPerChip];
    Int_t       fEvent; //  event nb read from header
    Int_t       fEvent2; // event nb read from header 
    Int_t       fCount; //  count of events read
    Int_t       fEvt; //  number of evt ? set to 0 and unchanged!
    Int_t       fjump; //  jump
    Int_t       fcol; //
    Int_t       frow; //
    Int_t       famp; //
    Int_t       Index_Max; //
    
    UInt_t      PIXEL[1000];
    
    
    ClassDef(DecoderM18_fb, 0) // class for reading MIMOSA raw digits
};

#endif
