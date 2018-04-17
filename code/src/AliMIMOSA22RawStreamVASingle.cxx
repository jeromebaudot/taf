///////////////////////////////////////////////////////////////////////////////
///
/// This class provides access to MIMOSA22THRB digits in raw data.
/// Developer: Ilaria Aimo, Cristina Bedda, Paola La Rocca  
/// Contact: paola.larocca@ct.infn.it
//////////////////////////////////////////////////////////////////////////////

#include "AliMIMOSA22RawStreamVASingle.h"

ClassImp(AliMIMOSA22RawStreamVASingle)

//__________________________________________________________________________
AliMIMOSA22RawStreamVASingle::AliMIMOSA22RawStreamVASingle() :
	fFileInput(0x0),kNFrames(21),fNFrame(0),fDataChar(0),fEventCounterCDH(0),fcountEv(0),fStart(0)
{
  fFrame = new std::vector<UInt_t>[kNFrames];
	for(Int_t i=0;i<kChipsPerDDL;i++) {fActiveChip[i]=kFALSE;fChipKey[i]=kFALSE;}
	for(Int_t i=0;i<kRowPerChip;i++){
		for(Int_t j=0;j<kColPerChip;j++)  fDataFrame[i][j]=-1;}
	NewEvent();
}
//__________________________________________________________________________
AliMIMOSA22RawStreamVASingle::AliMIMOSA22RawStreamVASingle(const AliMIMOSA22RawStreamVASingle& rstream) :
  TObject(rstream),fFileInput(0x0),kNFrames(21),fNFrame(0),fDataChar(0),fEventCounterCDH(0),fcountEv(0),fStart(0)
{
	// copy constructor
	printf("Copy constructor should not be used.\n");
}
//__________________________________________________________________________
AliMIMOSA22RawStreamVASingle& AliMIMOSA22RawStreamVASingle::operator=(const AliMIMOSA22RawStreamVASingle& rstream) 
{
	// assignment operator
	if (this!=&rstream) {}
	printf("Assignment opertator should not be used.\n");
	return *this;
}
//__________________________________________________________________________

Bool_t AliMIMOSA22RawStreamVASingle::ReadCDH() 
{
// read the next event
// returns kFALSE if there is no event left
	do{
		if(!ReadNextInt())return kFALSE;
	} while (fDataChar != 0xFAFAFAFA);

	NewEvent();
	fcountEv++;
	if(!ReadNextInt())return kFALSE;
	fEventCounterCDH = fDataChar & 0xFFFFFF;
	// printf("\n Trigger number %08X \n", fEventCounterCDH); //CHECK
	if(fcountEv!=fEventCounterCDH) {
		cout<<"Number of events != trigger"<<endl;
		return kFALSE; }
	// cout<<(bitset<4>)word<<endl;
	return kTRUE;
}
//__________________________________________________________________________
Bool_t AliMIMOSA22RawStreamVASingle::ReadNextInt() 
{
// reads next 32 bit into fDataChar 
	if(!fFileInput){
		printf("AliMIMO22 %d: Error No Input File\n", fBoardNumber);
		return kFALSE; }

	if(fread(&fDataChar,4,1,fFileInput))  return kTRUE;
  else return kFALSE;
}
//__________________________________________________________________________
Int_t AliMIMOSA22RawStreamVASingle::GetStart() 
{
// Look for the last word of the first frame
	fStart=0;

	for (int i = 0; i < kWordsPerFrame*(kNFrames-1); i++){
		if((fData[i] >> 16)!=0x0001) fStart++;
		else {
		//cout<<"Last word of the first frame " << fStart<<endl;
		return fStart;}
	}
    
  return 0;
}
//__________________________________________________________________________
Bool_t AliMIMOSA22RawStreamVASingle::ReadData() 
{
// Reads all the words contained in a single event 
// and fill the vector fFrame

	if(!fFileInput){
		printf("AliMIMO22 %d: Error No Input File\n", fBoardNumber);
		return kFALSE; }

	for (int i = 0; i < kWordsPerFrame*(kNFrames-1); i++){
		if(!ReadNextInt())return kFALSE;
		fData.push_back(fDataChar);
		//printf("%08X \n",fDataChar); //CHECK
	}
	GetStart();

	int tmpcnt = 0;
	for(int i = 0; i < kNFrames; i++){
		//First frame (Incomplete frame = 0)
		if(i == 0) {
			for(int j=0; j<(kWordsPerFrame-1-fStart); j++){
				UInt_t word = 0x10000000; //Set this value for missing pixel info
				fFrame[i].push_back(word);
			}
			for(int j=(kWordsPerFrame-1-fStart); j < kWordsPerFrame; j++){
				fFrame[i].push_back(fData[tmpcnt]);
				tmpcnt++;
			}
		}
		//Complete frames 1-9
		if(i > 0 && (i < (kNFrames-1))) {
			for(int j=0; j<kWordsPerFrame; j++){
				fFrame[i].push_back(fData[tmpcnt]);
				tmpcnt++;
			}
		}
		//Last frame (Incomplete frame = 10)
		if(i == (kNFrames-1)) {
			for(int j=0; j<(kWordsPerFrame-1-fStart); j++){
				fFrame[i].push_back(fData[tmpcnt]);
				tmpcnt++;
			}
			for(int j=(kWordsPerFrame-1-fStart); j < kWordsPerFrame; j++){
				UInt_t word =  0x10000000; //Set this value for missing pixel info
				fFrame[i].push_back(word);	
			}
		}
  } //End loop on frames
	return 1;
}	
//__________________________________________________________________________
void AliMIMOSA22RawStreamVASingle::NewEvent() 
{
// call this to reset flags for a new event
	fEventCounterCDH = 0;
	fData.erase(fData.begin(),fData.end()); //Erase elements of fData for a new event
	for(int i = 0; i < kNFrames; i++) fFrame[i].erase(fFrame[i].begin(),fFrame[i].end()); //Erase elements of fFrame[i] for a new event
}

//__________________________________________________________________________
Bool_t AliMIMOSA22RawStreamVASingle::ReadFrame() 
{
// Decoding data
	for(Int_t i=0;i<kRowPerChip;i++){
		for(Int_t j=0;j<kColPerChip;j++)		fDataFrame[i][j]=-1;
	}
	if(fNFrame<0 || fNFrame>=kNFrames){
		printf("Frame = %d: error no existing frame\n",fNFrame);
		return kFALSE;
	}
 
	Int_t tmpcnt = 0;
	for(Short_t row=0; row<kRowPerChip;row=row+2){
		for(Short_t i=0; i<8; i++){
			UInt_t word = fFrame[fNFrame][tmpcnt];
			UShort_t tmpbits = 0x0000;
			for(Short_t col=7; col<kColPerChip;col=col+8){
				if(word & 0x10000000){ 
					fDataFrame[row][col-i]   = -1; //Data for missing pixel info
					fDataFrame[row+1][col-i] = -1; //Data for missing pixel info
				}
				else{
					fDataFrame[row][col-i]   = (word >> tmpbits) & 0x00000001;
					fDataFrame[row+1][col-i] = (word >> (tmpbits+8)) & 0x00000001;
				}
			tmpbits++;
		}
		tmpcnt++;
	}
 }
 
return kTRUE;
}


//__________________________________________________________________________
Bool_t AliMIMOSA22RawStreamVASingle::HasData() 
{
  
  // Modified by JB 2015/05/25 transmit frame # as timestamp
  
  if (fDebugLevel) printf("ALI22::HasData reading event %d\n", fcountEv);

  ListOfPixels.clear();

  Short_t value;
  
  if( !ReadCDH() ) {
    return false;
  }

  if( !ReadData() ) {
    return false;
  }
  
  for(Int_t iframe=0; iframe<kNFrames; iframe++){  // loop on frames
    
    // cout<<iframe<<endl;	
    SetFrame(iframe);
    ReadFrame();
    
    for(Int_t irow=0;irow<kRowPerChip;irow++){
      for(Int_t icol=0;icol<kColPerChip;icol++) {
        value = GetADCCounts(irow,icol);
        // for now, set -1 values to 0, JB 2014/05/14
        if( value<0 ) value = 0;
        if (fDebugLevel>2) printf("Ali22::HasData adding pixel with value %d row %d col %d\n", value, irow, icol);
        //ListOfPixels.push_back( BoardReaderPixel( 1, value, irow, icol) );
        ListOfPixels.push_back( BoardReaderPixel( 1, value, irow, icol, iframe) );
      }
    }
    
  } // end loop on frames

  if (fDebugLevel) printf("Ali22::HasData new event %d will be generated with %lu pixels, trigger nb %d\n", fcountEv, ListOfPixels.size(), fEventCounterCDH);      
  fCurrentEvent = new BoardReaderEvent( fcountEv, fBoardNumber, fRunNumber, &ListOfPixels);

  return true;
      
}


//__________________________________________________________________________
void AliMIMOSA22RawStreamVASingle::PrintStatistics(ostream &stream) 
{
  
  // Print statistics on the events read by this board
  //
  // JB, 2014/05/14
  
  stream << "***********************************************" << endl;
  stream << " Board ALI22 " << fBoardNumber << " found:" << endl;
  stream << fEventCounterCDH << " triggers." << endl;
  stream << fcountEv << " events in total," << endl;
  stream << "***********************************************" << endl;
  

}
