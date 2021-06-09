
/* $Id: DecoderM18.h 0 2012-11-14 15:26:05Z valtini $ */

///////////////////////////////////////////////////////////////////////////////
///
/// This class provides access to MIMOSA digits in raw data.
/// Developer: fb
//////////////////////////////////////////////////////////////////////////////

#include "DecoderM18.h"
#include "TMath.h"

ClassImp(DecoderM18)

// #############################################################################

DecoderM18::DecoderM18( Int_t aBoardNumber, Int_t aRunNumber, Int_t aSensorNumber) :
  kShift(3),
  kToBeSutractedFromAmp(32768),
  kAmpMultFactor(1.),
  kTrailer(0xfafafafa),
  kOMKDTransitionRead(0),
  kOMKDTransitionSet(0),
  kStopPointerTransitionRead(0),
  kStopPointerTransitionSet(0),
  fNEvent(0),
  fFileInput(0x0),
  fDataChar(0),
  fcountEv(-1),
  fEvCounterCDH(-1),
  fIndex(0x0),
  fAmp(0x0),
  fBoardNumber(0),
  fRunNumber(0),
  fSensorNumber(0),
  fDebugLevel(0)
{
    
  fBoardNumber = aBoardNumber;
  fRunNumber = aRunNumber;
  fSensorNumber = aSensorNumber;
 
}
// #############################################################################

DecoderM18::DecoderM18(const DecoderM18& rstream) :
TObject(rstream),
  kShift(3),
  kToBeSutractedFromAmp(32768),
  kAmpMultFactor(1.),
  kTrailer(0xfafafafa),
  kOMKDTransitionRead(0),
  kOMKDTransitionSet(0),
  kStopPointerTransitionRead(0),
  kStopPointerTransitionSet(0),
  fNEvent(0),
  fFileInput(0x0),
  fDataChar(0),
  fcountEv(-1),
  fEvCounterCDH(-1),
  fIndex(0x0),
  fAmp(0x0),
  fBoardNumber(0),
  fRunNumber(0),
  fSensorNumber(0),
  fDebugLevel(0)
{
     
  // copy constructor
  printf("Copy constructor should not be used.\n");
}

// #############################################################################

DecoderM18& DecoderM18::operator=(const DecoderM18& rstream) {
  // assignment operator
  if (this!=&rstream) {}
  printf("Assignment opertator should not be used.\n");
  return *this;
}

// #############################################################################
void DecoderM18::SetShift( Int_t aShift){

  kShift = aShift;
  printf("DecoderM18 [%d]: Shift set to %d\n", fBoardNumber, kShift);
  
}

// #############################################################################
void DecoderM18::SetOffset( Int_t aOffset){
  
  kToBeSutractedFromAmp = aOffset;
  printf("DecoderM18 [%d]: Offset set to %d\n", fBoardNumber, kToBeSutractedFromAmp);
  
}

// #############################################################################
void DecoderM18::SetMulFactor( Int_t aFactor){
  
  kAmpMultFactor = aFactor;
  printf("DecoderM18 [%d]: MultFactor set to %d\n", fBoardNumber, kAmpMultFactor);
  
}

// #############################################################################
void DecoderM18::SetTrailer( UInt_t aTrailer){
  
  kTrailer = aTrailer;
  printf("DecoderM18 [%d]: Trailer set to %x\n", fBoardNumber, kTrailer);
  
}

// #############################################################################
Bool_t DecoderM18::ReadNextInt() {

  if(!fFileInput){ printf("DecoderM18 [%d]: Error No Input File\n", fBoardNumber); return 0; }
  
  fread(&fDataChar,1,4,fFileInput);
   
  if( feof(fFileInput) ) {
    printf("DecoderM18 [%d]: No more data in file! -> STOPPING\n", fBoardNumber);
    return kFALSE;
  }
    
 return kTRUE;
}


// #############################################################################
Int_t DecoderM18::Get_Nevent() {
  
  if(!fFileInput){ printf("DecoderM18 [%d]: Error No Input File\n", fBoardNumber); return -1; }

  Long_t position;
  position = ftell(fFileInput); //save current position 

  fseek(fFileInput,0,SEEK_SET); // go to beginning of the file

  fNEvent=0;
  while(!feof(fFileInput))
    {
      if(!ReadNextInt()){
	cout << "No 32 bit words to read!" << endl;
	return -1;
      }
      if(fDataChar == kTrailer){fNEvent++;}
    }


  fseek(fFileInput,position,SEEK_SET); //go back to initial position

  return fNEvent;



}



// #############################################################################
Bool_t DecoderM18::ReadCDH() {
    
  // Take into account properly the header,
  //  this is necessary to extract the OMKDTransition information.
  // BUT, the read OMKDTransition info is a priori not used for the same file,
  //   because it is expected that the DAQ sw has already used it in the computation
  //   of the column and row of the samples.
  // JB, 2015/03/27
  
  do{
    if(!ReadNextInt()) return kFALSE;
  }
  while(fDataChar != kTrailer);

  NewEvent();

  Int_t NwordJump=0;

  fcountEv++;
 
  if(!ReadNextInt()) return kFALSE;
  fEvCounterCDH = fDataChar;
  if(fcountEv!=fEvCounterCDH) cout << "N ev mismatch!!! counter: " << fcountEv << " read from CDH " << fEvCounterCDH << endl;
  if(!ReadNextInt()) return kFALSE;
  kStopPointerTransitionRead = fDataChar%(kRowPerChip*kColPerChip);
  //if(!ReadNextInt()) return kFALSE; //Dummy info (stop pointer)
  if(!ReadNextInt()) return kFALSE; //Dummy info (wrap around bit)
  if(!ReadNextInt()) return kFALSE; 

  NwordJump=fDataChar; //(OMKDtransition) # of words to skip
  Int_t info[4] = {0, 0, 0, 0};
  for(Int_t i=0; i<NwordJump; i++) {
    if(!ReadNextInt()) return kFALSE;
    info[i] = fDataChar;
    if( i==NwordJump && (fDataChar==0 || kRowPerChip*kColPerChip<=fDataChar) ) printf( "WARNING: DecoderM18 [%d]: crazy OKMD %d %d %d %d\n", fBoardNumber, info[0], info[1], info[2], info[3]);
  }
  kOMKDTransitionRead = info[0]%(kRowPerChip*kColPerChip);
//  printf( "WARNING: DecoderM18 [%d]: OKMD values %d %d %d %d -> (modulo %d) set to %d\n", fBoardNumber, info[0], info[1], info[2], info[3], kRowPerChip*kColPerChip, kOMKDTransitionRead);

 if (fDebugLevel) {
    cout << "DecoderM18 [" << fBoardNumber << "] HEADER:" << endl;
    cout << "   event number = " << fEvCounterCDH << endl;
    cout << "   StopPointerTransitionRead = " << kStopPointerTransitionRead << endl;
    cout << "   NumOMKDTransitions = " << NwordJump << endl;
    cout << "   OMKDTransitionRead = " << kOMKDTransitionRead << endl;
   
   }
    
return kTRUE;
    
}
// #############################################################################
void DecoderM18::NewEvent() {

  fEvCounterCDH=0;
  fIndex.clear();
  fAmp.clear();


}



// #############################################################################
Bool_t DecoderM18::ReadData( ) {

  if(!ReadCDH()) return kFALSE;
  Int_t Amp=0;
  Int_t Index=-1;
  
  if (fDebugLevel) printf("DecoderM18 [%d]::ReadData reading event %d with OMKDTransitionSet=%d\n with StopPointerTransitionSet=%d\n", fBoardNumber, fEvCounterCDH, kOMKDTransitionSet, kStopPointerTransitionSet);

  while(true){
    if(!ReadNextInt()) return kFALSE;
    if(fDataChar != kTrailer){
      Index = CalculateIndex();
      fIndex.push_back( Index );
      Amp = (fDataChar & 0xffff0000) >> 16;
      Amp = (Amp-kToBeSutractedFromAmp)*kAmpMultFactor;
      fAmp.push_back( Amp );
      //cout << " Pixel[" << fIndex.size() << "]: initial hex= " << fDataChar << ", final amp= ", << Amp << ", at index " << Index << endl;
    } 
    else {
      fseek(fFileInput,-4,SEEK_CUR);
      return kTRUE;
    }
  }
 
  if (fDebugLevel) printf("  DecoderM18 %d: event %d (real #=%d) completed with %lu pixels found\n", fBoardNumber, fEvCounterCDH, fcountEv, fIndex.size());
        
return kTRUE;
    
}


// #############################################################################
Int_t DecoderM18::CalculateIndex() {

  // Use the info aOMKDTransitionSet to shift the index computed 
  // from the col & row information read in the binary file.
  // The shift is 0 by default and has no effect.
  // The shift is set externally by the SetOMKDTransition( aTransition) method,
  //  since it is expected to be read from another file.
  // 
  // JB 2015/03/27
  
  Int_t row ,col, index;

  
  // Get the row, col info from the file
  row = fDataChar & 0x000000FF;
  col = (fDataChar & 0x0000FF00) >> 8;

  
  // Compute the index:
  //   if the OMKDTransition info is 0, do not modify basic formula
  //   otherwise shift index by OMKDTransition value
  index =row *kRowPerChip +col;
  if( kOMKDTransitionSet != 0 ) {
    Int_t kFirst = kStopPointerTransitionSet-kOMKDTransitionSet;
    if (index>=kFirst) index=(index+ kFirst)%(kRowPerChip*kColPerChip);
    else  index =(index+ kRowPerChip*kColPerChip+kFirst)%(kRowPerChip*kColPerChip);

//      index = (index+kOMK
  }

  //   if (fDebugLevel)cout << " !!!!!!!!!!!!!!!!check on kOMKD StopPointer   row " << row << ", col " << col << ", initial index " << row *kRowPerChip +col << ",final  index " << index <<" kOMD " <<kOMKDTransitionSet<<"  Stop Pointer "<<kStopPointerTransitionSet << endl;
 
 
  // Apply kShift
  if(index>=kShift) index = index -kShift;
  else index = kRowPerChip*kColPerChip + index - kShift;    
  
  // cout << "row " << row << ", col " << col << ", initial index " << row *kRowPerChip +col << ", final index " << index << endl;
    
  if( index<0 || kRowPerChip*kColPerChip<=index ) 
    printf( "WARNING DecoderM18 [%d]: pb with crazy pixel index %d, initial( row=%d, col=%d, index=%d), OKMD=%d, Shift=%d\n, StopPointer=%d\n", fBoardNumber, index, row, col, row*kRowPerChip+col, kOMKDTransitionSet, kShift, kStopPointerTransitionSet);
  
  return index;
    
}

// #############################################################################
void DecoderM18::PrintStatistics(ostream &stream) 
{
  
  // Print statistics on the events read by this board
  //
  // JB, 2014/05/14
  
  stream << "***********************************************" << endl;
  stream << " Board DecoderM18 " << fBoardNumber << " found:" << endl;
  stream << fEvCounterCDH << " events in total," << endl;
  stream << "***********************************************" << endl;
  
  
}
