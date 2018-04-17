
/* $Id: DecoderGeant.h 0 2012-11-14 15:26:05Z $ */

///////////////////////////////////////////////////////////////////////////////
///
/// This class provides access to data simulated by GEANT.
/// Developer: plr
//////////////////////////////////////////////////////////////////////////////

#include "DecoderGeant.h"

ClassImp(DecoderGeant)

// #############################################################################

DecoderGeant::DecoderGeant(Int_t aBoardNumber, Int_t aRunNumber) :
fNEvent(0),
fFileInput(0x0),
fDataChar(0),
fEvCounterCDH(-1),
fRow(0x0),
fCol(0x0),
fIndex(0x0),
fAmp(0x0),
fBoardNumber(0),
fRunNumber(0),
fFlag(0),
fDebugLevel(0)
{

  fBoardNumber = aBoardNumber;
  fRunNumber = aRunNumber;

}
// #############################################################################

DecoderGeant::DecoderGeant(const DecoderGeant& rstream) :
TObject(rstream),
  fNEvent(0),
  fFileInput(0x0),
  fDataChar(0),
  fEvCounterCDH(-1),
  fRow(0x0),
  fCol(0x0),
  fIndex(0x0),
  fAmp(0x0),
  fBoardNumber(0),
  fRunNumber(0),
  fFlag(0),
  fDebugLevel(0)
{
    
  // copy constructor
  printf("Copy constructor should not be used.\n");
}

// #############################################################################

DecoderGeant& DecoderGeant::operator=(const DecoderGeant& rstream) {
  // assignment operator
  if (this!=&rstream) {}
  printf("Assignment opertator should not be used.\n");
  return *this;
}

// #############################################################################
void DecoderGeant::NewEvent() {

  fEvCounterCDH=0;
  fIndex.clear();
  fAmp.clear();
	fRow.clear();
	fCol.clear();

}


// #############################################################################
Bool_t DecoderGeant::ReadCDH() {

	if(!fFileInput){ printf("Error No Input File\n"); return kFALSE; }
	fscanf (fFileInput, "%i %i",&fRowPerChip,&fColPerChip);

	fFlag = kTRUE;
	
	return kTRUE;

}

// #############################################################################
Bool_t DecoderGeant::ReadNextLine() {

	if(!fFileInput){ printf("Error No Input File\n"); return kFALSE; }

	if(fscanf (fFileInput, "%i %i %i %i %lf %i %i %lf %i %i %lf", &fEvCounterCDH, &fNHitPixel, &fRowtmp[0], &fColtmp[0], &fAmptmp[0], 
																											 															&fRowtmp[1], &fColtmp[1], &fAmptmp[1], 
																																										&fRowtmp[2], &fColtmp[2], &fAmptmp[2])) return kTRUE;
	else return kFALSE;
}

// #############################################################################
Int_t DecoderGeant::Get_Nevent() {

  if(!fFileInput){ printf("Error No Input File\n"); return -1; }

  Long_t position;
	Bool_t fFlagStatus = fFlag;
	
  position = ftell(fFileInput); //save current position 

  fseek(fFileInput,0,SEEK_SET); // go to beginning of the file
	ReadCDH();

  fNEvent=0;
  while(!feof(fFileInput))
    {
      if(!ReadNextLine()){

				cout << "No new lines to read!" << endl;
				return -1;
      }
      fNEvent++;
    }

  fseek(fFileInput,position,SEEK_SET); //go back to initial position
	fFlag = fFlagStatus;
  return fNEvent;

}

// #############################################################################
Bool_t DecoderGeant::ReadData() {

	NewEvent();

	if(!fFlag) ReadCDH();

  if (fDebugLevel) printf("DecoderGeant::ReadData reading event %d\n", fEvCounterCDH);
	if(!ReadNextLine()) return kFALSE;
	for(int i=0; i<fNHitPixel; i++)	{
				fIndex.push_back(CalculateIndex(i));
				fAmp.push_back(fAmptmp[i]);
				fRow.push_back(fRowtmp[i]);
				fCol.push_back(fColtmp[i]);
	}

  if (fDebugLevel) printf("  event %d (real #=%d) completed with %d pixels found\n", int(fEvCounterCDH), int(fIndex.size()),1);
    
	return kTRUE;
    
}

// #############################################################################
Int_t DecoderGeant::CalculateIndex(int ipixel) {

  Int_t index =fRowtmp[ipixel]*fColPerChip +fColtmp[ipixel];
 
  return index;
    
}

// #############################################################################
void DecoderGeant::PrintStatistics(ostream &stream) 
{
  
  // Print statistics on the events read by this board
  //
  // JB, 2014/05/14
  
  stream << "***********************************************" << endl;
	stream << " Board DecoderGeant " << fBoardNumber << " found:" << endl;
  stream << fEvCounterCDH << " events in total," << endl;
  stream << "***********************************************" << endl;
  
  
}

