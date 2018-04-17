/////////////////////////////////////////////////////////////
// Class Description of BoardReaderEUDAQ
//                                      
// Dedicated to decode output files from the EUDAQ system 
//  
// 
// Based on ...
//                                      
/////////////////////////////////////////////////////////////
//
// created JB, 2016/08/17

#include "BoardReaderEUDAQ.h"

ClassImp(BoardReaderEUDAQ);

//------------------------------------------+-----------------------------------
BoardReaderEUDAQ::BoardReaderEUDAQ(int boardNumber, int runNumber, int sensorType, int numberOfSensors, int triggerMode, int eventBuildingMode) {

  fDebugLevel = 0;
  
  fBoardNumber = boardNumber;
  fRunNumber = runNumber;
  fSensorType = sensorType;
  fNumberOfSensors = numberOfSensors;

  cout << "*****************************************" << endl;
  cout << "    < BoardReaderEUDAQ constructor >      " << endl; 
  cout << "*****************************************" << endl;  
  cout << "Creating a BoardReaderEUDAQ" << endl;
  cout << " * for board : " << fBoardNumber << endl;
  cout << " * for runNumber : " << fRunNumber << endl;
  cout << " * for sensor type : " << fSensorType << endl;
  cout << " * nb of sensors : " << fNumberOfSensors << endl;  

  fListOfInputFileNames.clear();
//  fNumberOfFiles = 0;
  fCurrentFileNumber = -1;

  fBufferRead = 0;
  fEventNumber = 0;
  fCurrentEvent = NULL;
  
}

//------------------------------------------+-----------------------------------
BoardReaderEUDAQ::~BoardReaderEUDAQ()
{
  delete fCurrentEvent;
  delete fData;
  delete fInputFileName;

}

// --------------------------------------------------------------------------------------

bool BoardReaderEUDAQ::AddFile(char *fileName) {
  
//  fCurrentFileNumber = 0;
  char inputFileName[500] = fileName;
  sprintf(inputFileName,"%s", fTool.LocalizeDirName( inputFileName));
  fRawFileStream.open( inputFileName);
  if( fRawFileStream.fail() ) {
    cout << endl << "ERROR BoardReaderEUDAQ " << fBoardNumber << " file " << inputFileName << " does not exist!" << endl;
    return false;
  }
  fListOfInputFileNames.push_back( inputFileName);
  cout << "  --> BoardReaderEUDAQ " << fBoardNumber << " New file " << fListOfInputFileNames.back() << ", total of " << fListOfInputFileNames.size() << " files." << endl;
  
  fCurrentInputFileName = fListOfInputFileNames.begin();

  return true;
}

// --------------------------------------------------------------------------------------
bool BoardReaderEUDAQ::AddFileList(const char *prefixFileName, int startIndex, int endIndex, const char *suffixFileName) {
  
  // Build the list of rawdata files from start index to endindex
  
  char inputFileName[500];
  
  if(fDebugLevel>0) cout <<  "Indices: start = " << startIndex << ", end = " << endIndex << endl;
  cout << "BoardReaderEUDAQ " << BoardNumber << " adding " << fNumberOfFiles << " files like " << prefixFileName << "*" << suffixFileName << endl;
  for(int fileNumber=0; fileNumber<=endIndex-startIndex; fileNumber++){
    sprintf( inputFileName, "%s%d%s", prefixFileName, fileNumber+startIndex, suffixFileName);
    sprintf( inputFileName,"%s", fTool.LocalizeDirName( inputFileName));
    if(DebugLevel>0) cout << "  checking file " << inputFileName << endl;
    fRawFileStream.open( inputFileName);
    if( fRawFileStream.fail() ) {
      cout << endl << "ERROR BoardReaderEUDAQ " << fBoardNumber << " file " << inputFileName << " does not exist!!" << endl;
      fRawFileStream.close();
      return false;
    }
    else{
      fListOfInputFileNames.push_back( inputFileName);
      cout << "  --> BoardReaderEUDAQ " << fBoardNumber << " New file " << fListOfInputFileNames.back() << ", total of " << fListOfInputFileNames.size() << " files." << endl;
      fRawFileStream.close();
    }
  }
  //OpenRawFile(ListOfInputFileNames[0]); //reopens the very first file
  //   RawFileStream.clear();

  fCurrentInputFileName = fListOfInputFileNames.begin();
  
  return true;
}

// --------------------------------------------------------------------------------------
bool BoardReaderEUDAQ::LookUpRawFile() {
  
  // Close the current rawdata file if any
  // and try to open the next rawdata file.
  
  if( !(fListOfInputFileNames.empty()) ) {
    
    if( fCurrentInputFileName != fListOfInputFileNames.end() ) {

      if( fRawFileStream.is_open() ) { // file already open, close and update iterator
        CloseRawFile();
        fCurrentInputFileName++;
      }

      if(DebugLevel) cout << "  --> BoardReaderEUDAQ " << fBoardNumber << " Next file to read " << fCurrentInputFileName-fListOfInputFileNames.size() << " over " << fListOfInputFileNames.size() << " closing and opening." << endl;
      return OpenRawFile( *fCurrentInputFileName );
      
    }
  
    else { // no more file, end the reading
      cout << "  --> BoardReaderEUDAQ " << fBoardNumber << ": all " << fListOfInputFileNames.size() << " files read, closing!" << endl;
//      fNoMoreFile = true;
      return false;
    }
    
  }
     
  else { // no file at all
    cout << "ERROR: BoardReaderEUDAQ NO RAW DATA FILE WAS ASSOCIATED WITH BOARD " << fBoardNumber << ", STOPPING!" << endl << endl;
    return false;
  }

}

// --------------------------------------------------------------------------------------
bool BoardReaderEUDAQ::OpenRawFile( const char *fileName) {
  
  // Open a rawdata file
  
  fRawFileStream.open( fileName);
  bool b = fRawFileStream.fail();
  if (b == 0)  {
    BuffersRead = 0;
    cout << " -+-+- INFO BoardReaderEUDAQ " << fBoardNumber << ": File " << fileName << " opened." << endl;
  }
  else {
    cout << " -/-/- INFO BoardReaderEUDAQ " << fBoardNumber << ": File " << fileName << " not opened, rc = " << b << "." << endl;
  }
  return !b;
  
}

// --------------------------------------------------------------------------------------
bool BoardReaderEUDAQ::CloseRawFile() {

  // Closes a rawdata file
  
  fRawFileStream.close();
  bool b = fRawFileStream.fail();
  if (b == 0)
    cout << " -+-+- INFO BoardReaderEUDAQ " << fBoardNumber << ": File " << *fCurrentInputFileName << " closed " << endl;
  else
    cout << " -/-/- INFO BoardReaderEUDAQ " << fBoardNumber << ":  File " << *fCurrentInputFileName << " not closed, rc = " << b << "(eof="<< fRawFileStream.eof() << ", bad="<< fRawFileStream.bad() <<"." << endl;
  return b;
  
}

// --------------------------------------------------------------------------------------

bool BoardReaderEUDAQ::FetchEvent( ) {
  
  // Try to get the next word index from the Data buffer
  // either by reading in the current Data buffer
  // either by getting a new Data buffer from file and reading first word
  //
  // Change the bit order according to the endianess.
  //
  // SizeOfEvent corresponds typically to one event
  //
  //
  // return "false" if nothing to read, "true" otherwise
  //
  // JB, 2012/06/
  // Modified: JB, 2012/08/18 test the nb of events read
  
  bool readSuccess = false;
  
  fEventSize = 1024; // random value for now, might be replaced by method to evaluate event size
  
  // If we have read the full current Data buffer alreay,
  // try to get a new Data buffer
  //  either from the current file or a new one
  if ( !( RawFileStream.eof() ) || LookUpRawFile() ) {
    
    // Now we can get the next data buffer
    RawFileStream.read(reinterpret_cast<char *> ( &Data[0] ), sizeof(char) * fEventSize);
    if(DebugLevel>2) cout << "  BoardReaderEUDAQ " << BoardNumber << ": Got new data buffer " << BuffersRead << " with gcount=" << RawFileStream.gcount() << " bytes, SizeOfEvent=" << fEventSize << endl;
    readSuccess = ( (size_t)RawFileStream.gcount() == fEventSize );
    BuffersRead++;
  }
  return readSuccess;
  
}

// --------------------------------------------------------------------------------------

bool BoardReaderEUDAQ::SetBufferPointers( ) {
  
  // Set the pointers to the differrent elements inside the event buffer
  //  event header, each input and event trailer
  //
  // JB, 201
  
  if( Data==NULL) {
    cout << "WARNING in BoardReaderEUDAQ board " << BoardNumber << ", event pointer is null!" << endl;
    return false;
  }
  
  EventHeader = (TEventHeader*)Data;
  TriggerLine = EventHeader->VFasCnt[0];
  for( int iInput=0; iInput<NbOfInputs; iInput++ ) {
    if( iInput==0 ) {
      InputDataAdress[iInput] = SizeOfHeader;
    }
    else {
      InputDataAdress[iInput] = InputDataAdress[iInput-1] + SizeOfInputData[iInput-1];
    }
    if( DebugLevel>1 ) cout << "   shift for input " << iInput << " is " << InputDataAdress[iInput] << " bytes." << endl;
  }
  EventTrailer = BuildValue( fEventSize - fEventTrailerSize, 4);
  if( SizeOfTrailer>4 ) {
    EventTrailer = BuildValue( fEventSize - fEventTrailerSize + 4, 4);
  }
  
  FramesRead++;
  
  if( DebugLevel>1 ) cout << " SetBufferPointers for IMGBoard " << BoardNumber << " done, trailer is " << hex << EventTrailer << dec << endl;
  
  EventTrailer=0x89abcdef;
  return (EventTrailer==(int)0x89abcdef);
}

// --------------------------------------------------------------------------------------

bool BoardReaderEUDAQ::HasData( ) {
  
  // Try to find the data for the next event in the file
  // loop over the raw data file and decode each word in it
  //
  // If something goes wrong with an unexpected word,
  //  a flag (eventOK) is used to return "false"
  // Otherwise returns "true"
  //
  // JB, 2012/07/16
  // Modified: JB 2012/08/18 to store trigger info
  
  // -+-+- Initialization
  
  bool eventDone = false;   // we start with event incomplete
  bool eventOK   = true;    // we start with a good event by default
  CurrentEvent   = 0;       // Allow to know wether data are correct
  ListOfTriggers.clear(); // and clear the vectors
  ListOfTimestamps.clear();
  ListOfFrames.clear();
  ListOfPixels.clear();
  
  if(DebugLevel>0) {
    cout << endl ;
    cout << "BoardReaderEUDAQ::HasData() BoardNumber " << BoardNumber << " IMGBoardReader::HasData() - readingEvent " << ReadingEvent << " currentEvent " << CurrentEventNumber << " currentTrigger " << CurrentTriggerNumber << " currentFrame " <<	CurrentFrameNumber << endl;
  }
  
  if( GetNextEvent() ) {
    
    eventOK = SetBufferPointers();
    if( DebugLevel>1 ) PrintEventHeader();
    
    if( eventOK) {
      GetTriggerData(); // JB 2012/08/18, called prior GetInputData since 2013/06/20
      GetInputData();
      
      CurrentEvent = new IMGEvent( EventHeader->EvNo, BoardNumber, &ListOfTriggers, &ListOfTimestamps, &ListOfFrames, &ListOfPixels);
      if( DebugLevel>1 ) cout << " BoardNumber " << BoardNumber << " create new event " << EventHeader->EvNo << " with " << ListOfPixels.size() << " pixels." << endl;
      EventsCount++;
      eventDone = true;
    }
    else{
      cout<<"BoardReaderEUDAQ::HasData() - Event is not ok!"<<endl;
    }
    
  } // getting next buffer was OK
  else{
    cout<<"BoardReaderEUDAQ::HasData() - Can't get next buffer (# " << BuffersRead <<")!"<<endl;
  }
  
  
  
  // End
  // check there are still data to read
  return eventOK && !NoMoreFile;
  
}

//------------------------------------------+-----------------------------------

bool BoardReaderEUDAQ::HasData()
{
  
  // Try to built the next event:
  //  1) read the raw data
  //  2) decode them
  //
  // This method is the one to call from the outside
  
  if (fDebugLevel) printf("BoardReaderEUDAQ::HasData reading event %d\n", fEventNumber);
  
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
    if (fDebugLevel) printf("BoardReaderEUDAQ::HasData start decoding event\n");
    if( DecodeFrame() ) {
      if (fDebugLevel) printf("BoardReaderEUDAQ::HasData new event %d will be generated with %d triggers\n", fEventNumber, ListOfTriggers.size());      
      fCurrentEvent = new BoardReaderEvent( fEventNumber, fBoardNumber, fRunNumber, &ListOfPixels);
      fCurrentEvent->SetListOfTriggers( &ListOfTriggers);
      fCurrentEvent->SetListOfFrames( &ListOfFrames);
    };
    return true;
  }
  
  else {
    if (fDebugLevel) printf("BoardReaderEUDAQ::HasData wrong event building...stopping!\n");
    return false; 
  }
  
}

//------------------------------------------+-----------------------------------
bool BoardReaderEUDAQ::FetchEvent()
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
  //cout << "BoardReaderEUDAQ::FetchEvent declaring temporary backup of size " << fNumberOfSensors*sizeof(MI26_FrameRaw) << endl;
  unsigned int *tempDataBackup = new unsigned int[fNumberOfSensors*sizeof(MI26_FrameRaw)];
  int tempBackupIndex = 0; // index of data to backup for next event
  
  for (int iSensor = 0; iSensor < fNumberOfSensors; ++iSensor) { // Loop on all sensors
	  
    if (fDebugLevel) printf("BoardReaderEUDAQ::FetchEvent fetching frames for event %d sensor %d\n", fEventNumber, iSensor);
    
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
    if (fDebugLevel>2) printf("BoardReaderEUDAQ::FetchEvent found trigger %d in first frame for sensor %d\n", trigger, iSensor);
    ListOfFrames.push_back( data->FrameCnt);
    
    // Get subsequent frames
	  GetNextFrames(iSensor, trigger, tempDataBackup, tempBackupIndex);
    
	  delete data;
    if (fDebugLevel>2) printf("BoardReaderEUDAQ::FetchEvent end data for trigger %d and  sensor %d\n", trigger, iSensor);
    
  }  // end loop on all sensors
  
  if(fDebugLevel>4) 
	  for (int i = 0; i < fEventSize; ++i) 
      printf("Data %x\n", fData[i]);
  
  if (fDebugLevel>2) printf("BoardReaderEUDAQ::FetchEvent end event %d with size %d words\n", fEventNumber, fEventSize);
  fEventNumber++;
  
  
  // ----- Second loop on sensors to build the current frame
  
  if (fDebugLevel) printf("BoardReaderEUDAQ::FetchEvent transfering %d backup words for next event\n", tempBackupIndex);
  
  for (int iword = 0; iword < tempBackupIndex; iword++) {
    fDataBackup[iword] = tempDataBackup[iword];
  }
  
  
  return true;
}

// private method
// --------------------------------------------------------------------------------------
bool BoardReaderEUDAQ::GetStart(int iSensor) 
{
  // Test if first word of the frame matches sensor id,
  // return false if not.
  
  unsigned int key  = GetKeyHeader(iSensor);

  do {
//	  fread(tmp, sizeof(unsigned int), 1, fRawFileFAdc[iSensor]);
    fscanf( fRawFileFAdc[iSensor], "%x", &(fData[fIndex]));
    if (fDebugLevel>3) printf("BoardReaderEUDAQ::GetStart first header word %x at index %d (expedted key=%x) for sensor %d\n", fData[fIndex], fIndex, key, iSensor);
	  if (fData[fIndex] == key) {
      if (fDebugLevel>1) printf("BoardReaderEUDAQ::GetStart found starting key %x for sensor %d\n", key, iSensor);
      fIndex++;
      return true;
	  }
  } while (!feof(fRawFileFAdc[iSensor]));
  
  return false;
}

// --------------------------------------------------------------------------------------
void BoardReaderEUDAQ::GetNextFrames(int iSensor, unsigned int trigger, unsigned int *dataBackup, int &backupIndex) 
{
  
  // Read additional frames with respect to a first one define by the given trigger.
  // When the newly read frame does not match the given trigger,
  //  backup the data of this frame for the next event.
  
  if (fDebugLevel>2) printf("BoardReaderEUDAQ::GetNextFrames Getting a potential next frame for sensor %d, current index %d\n", iSensor, fIndex);
    
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
        if (fDebugLevel>2) printf("BoardReaderEUDAQ::GetNextFrames the new frame does not match current trigger %d and is not used for sensor %d\n File pointer reader is rewinded by 1 frame.\n", trigger, iSensor);
        break;
      }
      ListOfFrames.push_back( data->FrameCnt);
      if (fDebugLevel>2) printf("BoardReaderEUDAQ::GetNextFrames new frame added to current trigger %d for sensor %d\n", trigger, iSensor);
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
    if (fDebugLevel>2) printf( "BoardReaderEUDAQ::GetNextFrames ===> Data were back-up over %d words with header %x, trigger %x, frame %x, for sensor %d\n", fBackupSize[iSensor], fData[index], fData[index+1], fData[index+3], iSensor);
  }
  
  else {
    fBackupSize[iSensor] = 0;
    if (fDebugLevel) printf( "BoardReaderEUDAQ::GetNextFrames There is no more frame to read for sensor %d!\n", iSensor);
  }
  
  fIndex = index;
  fEventSize = fIndex;
  if (fDebugLevel>2) printf("BoardReaderEUDAQ::GetNextFrames event size is now %d for sensor %d\n", fEventSize, iSensor);
  
  delete data;
  
}

// --------------------------------------------------------------------------------------
void BoardReaderEUDAQ::GetFrame(int iSensor, MI26_FrameRaw* data)
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
  for (int idata=0; idata<dataLength; idata++) {
    fscanf( fRawFileFAdc[iSensor], "%x", &(fData[fIndex++]));
    if (fDebugLevel>3) printf("new data word %x at index %d for sensor %d\n", fData[fIndex-1], fIndex-1, iSensor);
  }
  
  fscanf( fRawFileFAdc[iSensor], "%x", &(fData[fIndex++]));
  if (fDebugLevel>3) printf("new trailer word %x at index %d for sensor %d\n", fData[fIndex-1], fIndex-1, iSensor);

  if( fData[fIndex-1] != GetTailHeader() ) {
    printf("BoardReaderEUDAQ::GetFrame frame probably incomplete, wrong trailer %x instead of %x!!!\n", fData[fIndex-1], GetTailHeader());
  }
  else if (fDebugLevel>2) printf("BoardReaderEUDAQ::GetFrame got a new frame for sensor %d: new index = %d, trailer =%x\n", iSensor, fIndex, fData[fIndex-1]);

  fFramesReadFromFile++;
  
}

// --------------------------------------------------------------------------------------
void BoardReaderEUDAQ::GetBackupData(int iSensor, MI26_FrameRaw* data)
{
  
  if (fDebugLevel>2) printf("Getting backup data (%d words) from frame already read for sensor %d, current index %d\n", fBackupSize[iSensor], iSensor, fIndex);
  
  // copy the backup words into the data buffer
  for (int iword=0; iword<fBackupSize[iSensor]; iword++) {
    fData[fIndex++] = fDataBackup[fBackupIndex];
    if( iword==GetHeaderSize()-1) memcpy(data, &fData[fIndex-GetHeaderSize()], GetHeaderSize()*sizeof(unsigned int));
    fDataBackup[fBackupIndex++] = 0;
    if (fDebugLevel>3) printf("backup word[%d]=%x copied to index %d for sensor %d\n", fBackupIndex-1, fData[fIndex-1], fIndex-1, iSensor);
  }
  
  if (fDebugLevel>2) printf("BoardReaderEUDAQ::GetBackupData first frame for sensor %d: header %x, trigger %x frame %x dataLength %x\n", iSensor, data->Header, data->TriggerCnt, data->FrameCnt, data->DataLength);
   
}

// --------------------------------------------------------------------------------------
void BoardReaderEUDAQ::AddPixel( int iSensor, int value, int aLine, int aColumn) 
{
   // Add a pixel to the vector of pixels
   // require the following info
   // - input = number of the sensors (start at 1 !!!!!)
   // - value = analog value of this pixel
   // - line & column = position of the pixel in the matrix
   
  if (fDebugLevel>2) printf("BoardReaderEUDAQ::Addpixel adding pixel for sensor %d with value %d line %d row %d\n", iSensor, value, aLine, aColumn);

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
int BoardReaderEUDAQ::GetSensor(unsigned int key)
{
//   key = (key >> 16) & 0xFFFF;
  if (fDebugLevel>2) printf("BoardReaderEUDAQ::GetSensor searching for sensor with key %x\n", key);

  for (int i = 0; i < fNumberOfSensors; i++) {
	  if (GetKeyHeader(i) == key) {
      if (fDebugLevel>2) printf("BoardReaderEUDAQ::GetSensor found sensor %d\n", i);
      return i;
    }
  }

  if (fDebugLevel>2) printf("BoardReaderEUDAQ::GetSensor NOT FOUND!\n");
  return -1;

}

// --------------------------------------------------------------------------------------
bool BoardReaderEUDAQ::CheckTriggerCnt(unsigned int trig)
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
bool BoardReaderEUDAQ::DecodeFrame() 
{
   // Read the information of a frame for a given sensor
   // We use extensively the structure definined by Gille Clauss
   
  if (fDebugLevel) printf("BoardReaderEUDAQ::DecodeFrame decoding event %d with size %d\n", fEventNumber, fEventSize);
   
   fIndex = 0;
   while (fIndex < fEventSize) { // Loop on the full event data
     
	  MI26_FrameRaw *frame = (MI26_FrameRaw*)(&fData[fIndex]);
	  int iSensor     = GetSensor(frame->Header);
     int localNbOfPixels = 0;
	  if (!CheckTriggerCnt(frame->TriggerCnt)) {
		 printf("BoardReaderEUDAQ::DecodeFrame() wrong trigger number %d instead of %d for sensor %d\n",frame->TriggerCnt, fCurrentTriggerCnt, iSensor);
		 return false;
	  }   
    if (fDebugLevel) printf("BoardReaderEUDAQ::DecodeFrame frame matches current trigger %d for sensor %d\n", frame->TriggerCnt, iSensor);
	  
	  int dataLength    = ((frame->DataLength & 0xFFFF0000)>>16); 
	  unsigned short *frameData = (unsigned short*)(&fData[fIndex+fgkHeaderSize]);
	  fIndex             += fgkHeaderSize + dataLength + 1;
	  dataLength         *= 2; // go to short
	  
//	  if (ValidHistogram()) {
//		 fpHisEvtLength[iSensor]->Fill(frame->TriggerCnt % 1000, dataLength/2);
//		 if (frame->TriggerCnt % 1000 == 0) fpHisEvtLength[iSensor]->Reset();
//	  }
	  
	  if (iSensor == -1) {
		 printf("BoardReaderEUDAQ::DecodeFrame() wrong header key %x\n",frame->Header);
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

void BoardReaderEUDAQ::PrintStatistics(ostream &stream) {
  
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


