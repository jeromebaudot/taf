/////////////////////////////////////////////////////////////
// Class Description of VMEBoardReader                     //
//                                                         //
// Dedicated to decode ASCII files 
//  from Eleuterio Spiriti VME-based DAQ
// 
// Originally written by Christian Finck
//  The fread, fseek methods originally used do not work well
//  The have been replaced with fscan 
// 
// Expects one file per channel, one channel per sensor
//                                                         //
/////////////////////////////////////////////////////////////
//
// created JB, 2014/05/12

#include "VMEBoardReader.h"

/*!
  class VMEBoardReader 
  \brief Reader action for binary files M26. **
   version for ascii files in revision 805
*/

ClassImp(VMEBoardReader);

const unsigned int VMEBoardReader::fgKeyHeader[] = {0x80018002, 0x80018003, 0x80018004, 0x80018005, 0x80018006, 0x80018007, 0x80018008, 0x80018009};
const int  VMEBoardReader::fgkHeaderSize = 5; 
const unsigned int VMEBoardReader::fgkTailHeader = 0xaaaaaaaa;


//------------------------------------------+-----------------------------------
//! Default constructor.
VMEBoardReader::VMEBoardReader(int boardNumber, const char* pathName, const char* prefixName, const char* suffixName, int runNumber, int numberOfSensors, int numberOfRows) {

  fDisplay = false;
  fDebugLevel = 0;
  
  fVetoOverflow = true; // if true, reject events with overflow
  
  fBoardNumber = boardNumber;
  fPathName = pathName;
  fPrefixName = prefixName;
  fSuffixName = suffixName;
  fRunNumber = runNumber;
  fNumberOfSensors = numberOfSensors;
  fNumberOfRows = numberOfRows;

  cout << "*****************************************" << endl;
  cout << "    < VMEBoardReader constructor >      " << endl; 
  cout << "*****************************************" << endl;  
  cout << "Creating a VMEBoardReader" << endl;
  cout << " * for board : " << fBoardNumber << endl;
  cout << " * for runNumber : " << fRunNumber << endl;
  cout << " * nb of sensors : " << fNumberOfSensors << endl;
  cout << " * input file like : " << fPrefixName << "xxx" << fSuffixName << endl;
  cout << " * with vetoOverflow : " << fVetoOverflow << endl;
  

  int size = fNumberOfSensors*sizeof(MI26_FrameRaw);
  printf(" * Frame size : %d\n", size);
  fData = new unsigned int[size];
  fDataBackup = new unsigned int[size];
  fBackupSize = new int[fNumberOfSensors];
  for (int iSensor=0; iSensor<fNumberOfSensors; iSensor++) {
    fBackupSize[iSensor] = 0;
  }
  
  fEventNumber = 0;
  fCurrentTriggerCnt = 0;
  fEventsOverflow = 0;
  fFramesReadFromFile = 0;
  fCurrentEvent = NULL;
  
  Open();
  
}

//------------------------------------------+-----------------------------------
//! Destructor.
VMEBoardReader::~VMEBoardReader()
{
   delete fData;
}

//------------------------------------------+-----------------------------------
//! Setup all histograms.
void VMEBoardReader::CreateHistogram()
{
  
#ifdef withROOT
  
   //DeleteHistogram();
   for (int i = 0; i < fNumberOfSensors; ++i) {
     switch (fNumberOfRows) {
         
       case 576: // MIMOSA 26
         fNumberOfColumns = 1152;
         break;
         
       default:
       case 928: // MIMOSA 28
         fNumberOfColumns = 960;
         break;
     };
		 fpHisPixelMap[i] = new TH2F(Form("vtPixelMap%d", i+1), Form("Vertex - pixel map for sensor %d", i+1), 
                                 fNumberOfRows, 0, fNumberOfRows, 
                                 fNumberOfColumns, 0, fNumberOfColumns);
		 fpHisPixelMap[i]->SetStats(kFALSE);
     fpHisRateMap[i] = new TH1F(Form("vtRateMap%d", i+1), Form("Vertex - rate per line for sensor %d", i+1), 
                                fNumberOfColumns, 0, fNumberOfColumns);
     
     fpHisRateMapQ[i] = new TH1F(Form("vtRateMapQ%d", i+1), Form("Vertex - rate per quadrant for sensor %d", i+1), 
                                 10, 0, 5);
     
     fpHisEvtLength[i] = new TH1F(Form("vtEvtLength%d", i+1), Form("Vertex - event length sensor %d", i+1), 
                                  1000, 0, 1000);
   }
 
#endif  
}

//------------------------------------------+-----------------------------------
int VMEBoardReader::Open()
{
  // Open all ascii files, one file per channel/sensor.
  // Return false if one file cannot be opened, true otherwise.
  //

  
  char inputFileName[500];
   
	  
   bool valid = true;
   bool isOk  = false;

   for (int i = 0; i < fNumberOfSensors; ++i) {
	  
	  // Close any previous open file
	   //fclose(fRawFileFAdc[i]); // crash on some architectures

		 sprintf( inputFileName, "%s/%s%d%s", fPathName, fPrefixName, i, fSuffixName);
     fRawFileFAdc[i] = fopen(inputFileName, "r");
		 if( !fRawFileFAdc[i] ) { // end the reading if file opening failed
			cout << endl << "VMEBoardReader::Open(), cannot open file " << inputFileName << endl;
			isOk = false;
		 } else {
			isOk = true;
       cout << endl << "VMEBoardReader::Open(), file " << inputFileName << " opened" << endl;
		 }
	  
	  valid &= isOk;
   }

  return valid;
}


//------------------------------------------+-----------------------------------

bool VMEBoardReader::HasData()
{
  
  // Try to built the next event:
  //  1) read the raw data
  //  2) decode them
  //
  // This method is the one to call from the outside
  
  if (fDebugLevel) printf("VMEBoardReader::HasData reading event %d\n", fEventNumber);
  
  fCurrentTriggerCnt = 0;
  if( fCurrentEvent!=NULL) {
    delete fCurrentEvent;
    fCurrentEvent = NULL; 
  }
  ListOfPixels.clear();
  ListOfTriggers.clear();
  ListOfFrames.clear();
  ListOfLineOverflow.clear();
  
  if (FetchEvent()) {
    if (fDebugLevel) printf("VMEBoardReader::HasData start decoding event\n");
    if( DecodeFrame() ) {
      if (fDebugLevel) printf("VMEBoardReader::HasData new event %d will be generated with %lu triggers\n", fEventNumber, ListOfTriggers.size());
      fCurrentEvent = new BoardReaderEvent( fEventNumber, fBoardNumber, fRunNumber, &ListOfPixels);
      fCurrentEvent->SetListOfTriggers( &ListOfTriggers);
      fCurrentEvent->SetListOfFrames( &ListOfFrames);
    };
    return true;
  }
  
  else {
    if (fDebugLevel) printf("VMEBoardReader::HasData wrong event building...stopping!\n");
    return false; 
  }
  
}

//------------------------------------------+-----------------------------------
bool VMEBoardReader::FetchEvent()
{
  // Read the necessary frames for all sensors from the raw data files.
  // The data are stored in fData array for further decoding.
  // It means, a first frame is read (or restored if already read)
  //  and then subsequent frames are read and associated if related
  //  to the same trigger.
  
  
  // ----- First loop on sensor to build the current frame
  
  fIndex = 0; // index of current built event
  fBackupIndex = 0; //index of backup data for current event
  fEventSize = 0;
  //cout << "VMEBoardReader::FetchEvent declaring temporary backup of size " << fNumberOfSensors*sizeof(MI26_FrameRaw) << endl;
  unsigned int *tempDataBackup = new unsigned int[fNumberOfSensors*sizeof(MI26_FrameRaw)];
  int tempBackupIndex = 0; // index of data to backup for next event
  
  for (int iSensor = 0; iSensor < fNumberOfSensors; ++iSensor) { // Loop on all sensors
	  
    if (fDebugLevel) printf("VMEBoardReader::FetchEvent fetching frames for event %d sensor %d\n", fEventNumber, iSensor);
    
    // Get the first frame
	  MI26_FrameRaw* data = new MI26_FrameRaw;
    if ( fBackupSize[iSensor]!=0 ) { // if already read, restore
      GetBackupData( iSensor, data);
    }
    else { // read a new one
      if (!GetStart(iSensor)) return false;    
      GetFrame(iSensor, data);
    }
	  unsigned int trigger = data->TriggerCnt;
    if (fDebugLevel>2) printf("VMEBoardReader::FetchEvent found trigger %d in first frame for sensor %d\n", trigger, iSensor);
    ListOfFrames.push_back( data->FrameCnt);
    
    // Get subsequent frames
	  GetNextFrames(iSensor, trigger, tempDataBackup, tempBackupIndex);
    
	  delete data;
    if (fDebugLevel>2) printf("VMEBoardReader::FetchEvent end data for trigger %d and  sensor %d\n", trigger, iSensor);
    
  }  // end loop on all sensors
  
  if(fDebugLevel>4) 
	  for (int i = 0; i < fEventSize; ++i) 
      printf("Data %x\n", fData[i]);
  
  if (fDebugLevel>2) printf("VMEBoardReader::FetchEvent end event %d with size %d words\n", fEventNumber, fEventSize);
  fEventNumber++;
  
  
  // ----- Second loop on sensors to build the current frame
  
  if (fDebugLevel) printf("VMEBoardReader::FetchEvent transfering %d backup words for next event\n", tempBackupIndex);
  
  for (int iword = 0; iword < tempBackupIndex; iword++) {
    fDataBackup[iword] = tempDataBackup[iword];
  }
  
  
  return true;
}

// private method
// --------------------------------------------------------------------------------------
bool VMEBoardReader::GetStart(int iSensor) 
{
  // Test if first word of the frame matches sensor id,
  // return false if not.
  
  unsigned int key  = GetKeyHeader(iSensor);

  do {
//	  fread(tmp, sizeof(unsigned int), 1, fRawFileFAdc[iSensor]);
    fscanf( fRawFileFAdc[iSensor], "%x", &(fData[fIndex]));
    if (fDebugLevel>3) printf("VMEBoardReader::GetStart first header word %x at index %d (expedted key=%x) for sensor %d\n", fData[fIndex], fIndex, key, iSensor);
	  if (fData[fIndex] == key) {
      if (fDebugLevel>1) printf("VMEBoardReader::GetStart found starting key %x for sensor %d\n", key, iSensor);
      fIndex++;
      return true;
	  }
  } while (!feof(fRawFileFAdc[iSensor]));
  
  return false;
}

// --------------------------------------------------------------------------------------
void VMEBoardReader::GetNextFrames(int iSensor, unsigned int trigger, unsigned int *dataBackup, int &backupIndex) 
{
  
  // Read additional frames with respect to a first one define by the given trigger.
  // When the newly read frame does not match the given trigger,
  //  backup the data of this frame for the next event.
  
  if (fDebugLevel>2) printf("VMEBoardReader::GetNextFrames Getting a potential next frame for sensor %d, current index %d\n", iSensor, fIndex);
    
  MI26_FrameRaw* data = new MI26_FrameRaw;
  bool sameTrigger = true;
  int index = 0;
  do {
	  index = fIndex;
	  if( GetStart(iSensor) ) {
      GetFrame(iSensor, data);
      if (fDebugLevel>3) printf(" testing trigger %d from new frame / current trigger %d for sensor %d\n", data->TriggerCnt, trigger, iSensor);
      if (data->TriggerCnt != trigger) {
        sameTrigger = false;
        if (fDebugLevel>2) printf("VMEBoardReader::GetNextFrames the new frame does not match current trigger %d and is not used for sensor %d\n File pointer reader is rewinded by 1 frame.\n", trigger, iSensor);
        break;
      }
      ListOfFrames.push_back( data->FrameCnt);
      if (fDebugLevel>2) printf("VMEBoardReader::GetNextFrames new frame added to current trigger %d for sensor %d\n", trigger, iSensor);
    }
    else break;
  } while(sameTrigger);
  
  // At this step, two options:
  // 1) the last frame does not match the current event (sameTrigger == false)
  //    -> The already read data shall be backup to avoid reading it again !
  // 2) there is no more frame to read
  //    -> Indicate there are no backup data
  
  if ( !sameTrigger) {
    //fseek(fRawFileFAdc[iSensor], -2*sizeof(MI26_FrameRaw), SEEK_CUR);
    fBackupSize[iSensor] = fIndex - index;
    for (int iword=0; iword<fBackupSize[iSensor]; iword++) {
      dataBackup[backupIndex++] = fData[index+iword];
    } 
    if (fDebugLevel>2) printf( "VMEBoardReader::GetNextFrames ===> Data were back-up over %d words with header %x, trigger %x, frame %x, for sensor %d\n", fBackupSize[iSensor], fData[index], fData[index+1], fData[index+3], iSensor);
  }
  
  else {
    fBackupSize[iSensor] = 0;
    if (fDebugLevel) printf( "VMEBoardReader::GetNextFrames There is no more frame to read for sensor %d!\n", iSensor);
  }
  
  fIndex = index;
  fEventSize = fIndex;
  if (fDebugLevel>2) printf("VMEBoardReader::GetNextFrames event size is now %d for sensor %d\n", fEventSize, iSensor);
  
  delete data;
  
}

// --------------------------------------------------------------------------------------
void VMEBoardReader::GetFrame(int iSensor, MI26_FrameRaw* data)
{
  
  if (fDebugLevel>2) printf("Getting a new frame for sensor %d, current index %d\n", iSensor, fIndex);

    // 4 more words of frame header
    fscanf( fRawFileFAdc[iSensor], "%x", &(fData[fIndex++]));
    if (fDebugLevel>3) printf("new header word %x at index %d for sensor %d\n", fData[fIndex-1], fIndex-1, iSensor);
    fscanf( fRawFileFAdc[iSensor], "%x", &(fData[fIndex++]));
    if (fDebugLevel>3) printf("new header word %x at index %d for sensor %d\n", fData[fIndex-1], fIndex-1, iSensor);
    fscanf( fRawFileFAdc[iSensor], "%x", &(fData[fIndex++]));
    if (fDebugLevel>3) printf("new header word %x at index %d for sensor %d\n", fData[fIndex-1], fIndex-1, iSensor);
    fscanf( fRawFileFAdc[iSensor], "%x", &(fData[fIndex++]));
    if (fDebugLevel>3) printf("new header word %x at index %d for sensor %d\n", fData[fIndex-1], fIndex-1, iSensor);
  
  memcpy(data, &fData[fIndex-GetHeaderSize()], GetHeaderSize()*sizeof(unsigned int));

  unsigned int dataLength  =  ((data->DataLength & 0xFFFF0000)>>16);
  if (fDebugLevel>2) printf("datalength is %d words from %x for sensor %d\n", dataLength, data->DataLength, iSensor);
  
  
  //fread(&fData[fIndex], sizeof(int), dataLength, fRawFileFAdc[iSensor]);
  for (unsigned int idata=0; idata<dataLength; idata++) {
    fscanf( fRawFileFAdc[iSensor], "%x", &(fData[fIndex++]));
    if (fDebugLevel>3) printf("new data word %x at index %d for sensor %d\n", fData[fIndex-1], fIndex-1, iSensor);
  }
  
  fscanf( fRawFileFAdc[iSensor], "%x", &(fData[fIndex++]));
  if (fDebugLevel>3) printf("new trailer word %x at index %d for sensor %d\n", fData[fIndex-1], fIndex-1, iSensor);

  if( fData[fIndex-1] != GetTailHeader() ) {
    printf("VMEBoardReader::GetFrame frame probably incomplete, wrong trailer %x instead of %x!!!\n", fData[fIndex-1], GetTailHeader());
  }
  else if (fDebugLevel>2) printf("VMEBoardReader::GetFrame got a new frame for sensor %d: new index = %d, trailer =%x\n", iSensor, fIndex, fData[fIndex-1]);

  fFramesReadFromFile++;
  
}

// --------------------------------------------------------------------------------------
void VMEBoardReader::GetBackupData(int iSensor, MI26_FrameRaw* data)
{
  
  if (fDebugLevel>2) printf("Getting backup data (%d words) from frame already read for sensor %d, current index %d\n", fBackupSize[iSensor], iSensor, fIndex);
  
  // copy the backup words into the data buffer
  for (int iword=0; iword<fBackupSize[iSensor]; iword++) {
    fData[fIndex++] = fDataBackup[fBackupIndex];
    if( iword==GetHeaderSize()-1) memcpy(data, &fData[fIndex-GetHeaderSize()], GetHeaderSize()*sizeof(unsigned int));
    fDataBackup[fBackupIndex++] = 0;
    if (fDebugLevel>3) printf("backup word[%d]=%x copied to index %d for sensor %d\n", fBackupIndex-1, fData[fIndex-1], fIndex-1, iSensor);
  }
  
  if (fDebugLevel>2) printf("VMEBoardReader::GetBackupData first frame for sensor %d: header %x, trigger %x frame %x dataLength %x\n", iSensor, data->Header, data->TriggerCnt, data->FrameCnt, data->DataLength);
   
}

// --------------------------------------------------------------------------------------
void VMEBoardReader::AddPixel( int iSensor, int value, int aLine, int aColumn) 
{
   // Add a pixel to the vector of pixels
   // require the following info
   // - input = number of the sensors (start at 1 !!!!!)
   // - value = analog value of this pixel
   // - line & column = position of the pixel in the matrix
   
  if (fDebugLevel>2) printf("VMEBoardReader::Addpixel adding pixel for sensor %d with value %d line %d row %d\n", iSensor, value, aLine, aColumn);

  ListOfPixels.push_back( BoardReaderPixel( iSensor+1, value, aLine, aColumn, 0) );

#ifdef withROOT
   if (ValidHistogram()) {
		 fpHisPixelMap[iSensor]->Fill(aLine, aColumn);
     fpHisRateMap[iSensor]->Fill(aColumn);
     for (int i = 0; i < 4; ++i) {
       if (aColumn >= 258*i && aColumn < (i+1)*258)
         fpHisRateMapQ[iSensor]->Fill(i+1);
     }
   }
#endif
  
}

// --------------------------------------------------------------------------------------
int VMEBoardReader::GetSensor(unsigned int key)
{
//   key = (key >> 16) & 0xFFFF;
  if (fDebugLevel>2) printf("VMEBoardReader::GetSensor searching for sensor with key %x\n", key);

  for (int i = 0; i < fNumberOfSensors; i++) {
	  if (GetKeyHeader(i) == key) {
      if (fDebugLevel>2) printf("VMEBoardReader::GetSensor found sensor %d\n", i);
      return i;
    }
  }

  if (fDebugLevel>2) printf("VMEBoardReader::GetSensor NOT FOUND!\n");
  return -1;

}

// --------------------------------------------------------------------------------------
bool VMEBoardReader::CheckTriggerCnt(unsigned int trig)
{
  if (fCurrentTriggerCnt == 0) {
	  fCurrentTriggerCnt = trig;
    ListOfTriggers.push_back( trig);
	  return true;
  }
  
  if (fCurrentTriggerCnt != trig)
	  return false;
  
  return true;
}

// --------------------------------------------------------------------------------------
bool VMEBoardReader::DecodeFrame() 
{
   // Read the information of a frame for a given sensor
   // We use extensively the structure definined by Gille Clauss
   
  if (fDebugLevel) printf("VMEBoardReader::DecodeFrame decoding event %d with size %d\n", fEventNumber, fEventSize);
   
   fIndex = 0;
   while (fIndex < fEventSize) { // Loop on the full event data
     
	  MI26_FrameRaw *frame = (MI26_FrameRaw*)(&fData[fIndex]);
	  int iSensor     = GetSensor(frame->Header);
     int localNbOfPixels = 0;
	  if (!CheckTriggerCnt(frame->TriggerCnt)) {
		 printf("VMEBoardReader::DecodeFrame() wrong trigger number %d instead of %d for sensor %d\n",frame->TriggerCnt, fCurrentTriggerCnt, iSensor);
		 return false;
	  }   
    if (fDebugLevel) printf("VMEBoardReader::DecodeFrame frame matches current trigger %d for sensor %d\n", frame->TriggerCnt, iSensor);
	  
	  int dataLength    = ((frame->DataLength & 0xFFFF0000)>>16); 
	  unsigned short *frameData = (unsigned short*)(&fData[fIndex+fgkHeaderSize]);
	  fIndex             += fgkHeaderSize + dataLength + 1;
	  dataLength         *= 2; // go to short
	  
//	  if (ValidHistogram()) {
//		 fpHisEvtLength[iSensor]->Fill(frame->TriggerCnt % 1000, dataLength/2);
//		 if (frame->TriggerCnt % 1000 == 0) fpHisEvtLength[iSensor]->Reset();
//	  }
	  
	  if (iSensor == -1) {
		 printf("VMEBoardReader::DecodeFrame() wrong header key %x\n",frame->Header);
		 return false;
	  }
	  
	  // -+-+- Pointers AND LOOP to usefull data, i.e. line and states
	  MI26__TStatesLine* lineStatus;
	  MI26__TState*      state;
	  
	  int index = 0;
	  while( index < dataLength) { // Loop over usefull data
		 // first 16 bits word is the Status/Line
		 lineStatus = (MI26__TStatesLine*)frameData;
		 
		 if (fDebugLevel > 3)
			printf("frame %x %x #state %d Line add %d ovf %d\n", frameData[0], frameData[1], lineStatus->F.StateNb, lineStatus->F.LineAddr, lineStatus->F.Ovf);
		 
		 frameData += 1; // goto next word
		 index += 2;
		 
		 if( lineStatus->F.Ovf > 0 ) { // Stop when overflow
       if(fEventsOverflow%1000==0) 
			   printf("WARNING : overflow while reading frame %d and sensor %d, total overflow number is %d\n", 
                frame->FrameCnt, iSensor, fEventsOverflow);
       fOverflow = true;
		 }
		 
		 if(fDebugLevel>3) 
			printf("  line %d, #states %d, overflow %d\n", 
				   lineStatus->F.LineAddr, lineStatus->F.StateNb, lineStatus->F.Ovf);
		 
		 fNStatesInLine = 0;
		 // Next words are the states if any
		 for( int iState = 0; iState < lineStatus->F.StateNb; iState++ ) { // loop over states found on sensor
			state = (MI26__TState*)frameData;
			frameData += 1; // goto next word
			//index++;
      if(lineStatus->F.StateNb!=1) index++; // proposed by Ilaria, 2014/08/27
			if(fDebugLevel > 3) 
			   printf("                  number of states %d, number of hits %d,\n", lineStatus->F.StateNb, state->F.HitNb+1);
			
			fNStatesInLine++;
			
			// A state contains HitNb+1 pixels
			// the first pixel being on the left at the column ColAddr
			for( int iPixel=0; iPixel < state->F.HitNb+1; iPixel++) { // loop on pixels in the state
			   
			   if(fDebugLevel > 3) 
				  printf("   line %d, col %d,\n", lineStatus->F.LineAddr, state->F.ColAddr+iPixel);
			   
			   // create a new pixel only if we are reading an event
			   // and if the line is in the proper limit
			   if ( !lineStatus->F.Ovf || !fVetoOverflow ) {
				  AddPixel( iSensor, 1, lineStatus->F.LineAddr, state->F.ColAddr+iPixel); // sensor starts at zero here
           localNbOfPixels++;
//				  if (pDatRaw->GetPixelsN(iSensor) > pConfig->GetAnalysisPar().HitsInPlaneMaximum) return false;
				  
			   }
			}
			
			if(fDebugLevel>2) 
			   printf("                  state %d, #pixels %d, column %d at mem.pos %ld\n", 
					  iState, state->F.HitNb+1, state->F.ColAddr, (long int)state);
		 } // end loop over states
		 
	  } // end loop over usefull data
	  
     if( fDebugLevel>2 ) printf(" --> Current frame %d for sensor %d has %d pixels.\n", frame->FrameCnt, iSensor, localNbOfPixels);
     
   } // end loop on the full event data
  
  // Count event with overflow
  if (fOverflow) {
    fEventsOverflow++;
  }
   
  return true;   
}

// --------------------------------------------------------------------------------------

void VMEBoardReader::PrintStatistics(ostream &stream) {
  
  // Print statistics on the events read by this board
  //
  // JB, 2014/05/13
  
  stream << "***********************************************" << endl;
  stream << " Board VME " << fBoardNumber << " found:" << endl;
  stream << fCurrentTriggerCnt << " triggers." << endl;
  stream << fEventNumber << " events in total," << endl;
  stream << fEventsOverflow << " events with an overflow (veto overflow " << fVetoOverflow << ")." << endl;
  stream << fFramesReadFromFile<< " frames read overall." << endl;
  stream << "***********************************************" << endl;
    
}


