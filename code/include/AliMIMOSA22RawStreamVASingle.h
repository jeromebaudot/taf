#ifndef ALIMIMOSA22RAWSTREAMVASINGLE_H
#define ALIMIMOSA22RAWSTREAMVASINGLE_H

///////////////////////////////////////////////////////////////////////////////
///
/// This class provides access to MIMOSA22THRB digits in raw data.
///
///////////////////////////////////////////////////////////////////////////////


#include "Riostream.h"
#include "TObject.h"
//#include <iostream>
#include <vector>
#include <fstream>
#include <bitset>
#include "BoardReader.h"

class AliMIMOSA22RawStreamVASingle: public TObject {

	public :
		AliMIMOSA22RawStreamVASingle();
    AliMIMOSA22RawStreamVASingle(const AliMIMOSA22RawStreamVASingle& rstream);
    AliMIMOSA22RawStreamVASingle& operator=(const AliMIMOSA22RawStreamVASingle& rstream);
    ~AliMIMOSA22RawStreamVASingle() {};

    void	SetInputFile(const char *filename="FIFOdata_M22.dat"){
						if ((fFileInput = fopen(filename, "rb")) == NULL)
							cout << "Could not open specified file: " << filename << endl;
						else
							cout << "File " << filename << " opened successfully" << endl; };

    void    SetFrame(int nframe){ 
	      			fNFrame=nframe; };

    void    SetDebugLevel( int aLevel) { fDebugLevel = aLevel; }
    BoardReaderEvent*   GetEvent() { return fCurrentEvent; }
    void    PrintStatistics(ostream &stream);
    Int_t     GetCDHEventCounter() { return fEventCounterCDH; }

    Bool_t  HasData();
  
    Bool_t  ReadFrame();

    Bool_t  ReadCDH();	

    Bool_t  ReadData();
              
    Short_t GetADCCounts(Int_t row, Int_t col){
            	if(row<0 || row>=kRowPerChip || col<0 || col>=kColPerChip){
             		printf("GetADCCounts: you asked for no existing pixel\n"); 
             		return -1;}
            	else {
								if(col < 8) return 0; 	// First 8 columns set to 0
								else return fDataFrame[row][col]; } };

  
    enum 	{kDDLsNumber = 1};		// number of DDLs  
    enum	{kChipsPerDDL = 1};		// number of chips in each DDL
    enum 	{kRowPerChip = 64};		// number of rows per chip
    enum 	{kColPerChip = 64};		//number of columns per chip
//    enum 	{kNFrames = 21};			//number of frames, a parameter since 2014/08/26 
    enum 	{kWordsPerFrame = (kRowPerChip*kColPerChip)/16};		//Number of words in one frame
    void SetNFrames( Int_t aNbOfFrames ) { kNFrames = aNbOfFrames; } // JB 2014/08/26  
  

  private :

    Int_t   fDebugLevel;       // debug level

  	FILE		*fFileInput;
    
    void		NewEvent();
    Bool_t	ReadNextInt();
    Int_t 	GetStart();

    Int_t   fBoardNumber;
    Int_t   fRunNumber;
    Int_t   kNFrames;             // Nb of frames per channel, JB 2014/08/26
    Int_t		fNFrame;							// frame (from 0 to kNFrames-1)
    UInt_t 	fDataChar;
    UInt_t  fEventCounterCDH;			// event number read by CDH
    UInt_t 	fcountEv;
    Int_t 	fStart;								// word before we start to read 

    Short_t	fDataFrame[kRowPerChip][kColPerChip];
    Bool_t	fActiveChip[kChipsPerDDL];
    Bool_t	fChipKey[kChipsPerDDL];

    std::vector<UInt_t> fData; 
    std::vector<UInt_t> *fFrame;

    BoardReaderEvent *fCurrentEvent;
    std::vector<BoardReaderPixel> ListOfPixels;

    ClassDef(AliMIMOSA22RawStreamVASingle, 1)		// class for reading MIMOSA22THRB raw digits
};

#endif
