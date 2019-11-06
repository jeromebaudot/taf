/////////////////////////////////////////////////////////////
// Class Description of VMEBoardReader                     //
//                                                         //
// Dedicated to decode ASCII files
//  from Eleuterio Spiriti FPGA-based (Moser board) DAQ
//
// Originally written by Christian Finck
//  The fread, fseek methods originally used do not work well
//  The have been replaced with fscan
//  Based on Christian's classes: TAVTactVmeReader & TAVTactBaseRaw
//
// Expects one file per channel, one channel per sensor
//  3 frames per event
//                                                         //
/////////////////////////////////////////////////////////////
//
// created JB, 2014/05/12
// completely renewed JB, 2019/07/13

#include "VMEBoardReader.h"

/*!
  class VMEBoardReader
  \brief Reader action for binary files M26. **
   version for ascii files in revision 805
*/

ClassImp(VMEBoardReader);

const UInt_t VMEBoardReader::fgkKeyHeader[]      = {0x80008000, 0x80018001, 0x80028002, 0x80038003};
const Int_t  VMEBoardReader::fgkFrameHeaderSize  = 6;
const Int_t  VMEBoardReader::fgkLineWidth        = 9;
const UInt_t VMEBoardReader::fgkFrameHeader      = 0x80088007;
const UInt_t VMEBoardReader::fgkFrameTail        = 0xaaa8aaa7;
const UInt_t VMEBoardReader::fgkKeyTail[]        = {0x8bb08bb0, 0x8bb18bb1, 0x8bb28bb2, 0x8bb38bb3};


//------------------------------------------+-----------------------------------
//! Default constructor.
VMEBoardReader::VMEBoardReader(int boardNumber, TString pathName, TString prefixName, TString suffixName, int runNumber, int numberOfSensors, int numberOfRows) {

  // Upgraded, JB 2019/07/13

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
  switch (fNumberOfRows) {
    case 576: // MIMOSA 26
      fNumberOfColumns = 1152;
      break;
    default:
    case 928: // MIMOSA 28
      fNumberOfColumns = 960;
      break;
  };


  cout << "*****************************************" << endl;
  cout << "    < VMEBoardReader constructor >      " << endl;
  cout << "*****************************************" << endl;
  cout << "Creating a VMEBoardReader" << endl;
  cout << " * for board : " << fBoardNumber << endl;
  cout << " * for runNumber : " << fRunNumber << endl;
  cout << " * nb of sensors : " << fNumberOfSensors << endl;
  cout << " * input file like : " << fPrefixName << "xxx" << fSuffixName << endl;
  cout << " * with vetoOverflow : " << fVetoOverflow << endl;

  SetTitle("TAVTactVmeReader - reader for VME reader");
  fBaseName ="data_FPGA_Mouser993P0160_V1_ch";

  Int_t size = (sizeof(MI26_FrameRaw)/4)*3 + 3; // 3 frame per event and 3 header word for each sensor
  printf(" * Event size : %d\n", size);
  fDataEvent = new UInt_t[size];

  size = fNumberOfSensors*sizeof(MI26_FrameRaw)*4;
  printf(" * Frame size : %d\n", size);
  fData.resize(size);

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
  delete [] fDataEvent;
  delete fData;
}

//------------------------------------------+-----------------------------------
int VMEBoardReader::Open()
{
  // Open all ascii files, one file per channel/sensor.
  // Return false if one file cannot be opened, true otherwise.
  //
  // Upgraded, JB 2019/11/06

  TString inputFileName;

  Bool_t valid = true;
  Bool_t isOk  = false;

  for (Int_t i = 0; i < fNumberOfSensors; ++i) {

   // Close any previous open file
   if( fRawFileAscii[i].is_open() && !fRawFileAscii[i].eof()) {
    isOk = true;
   } else {
    fRawFileAscii[i].close();
//		 inputFileName = Form("%s/%s%04d/800%d_%s%d%s", fPrefixName.Data(), fgDefaultFolderName.Data(), fRunNumber, i, fBaseName.Data(), i, fgDefaultExtName.Data());
    inputFileName = Form("%s/%s%d%s", fPathName.Data(), fPrefixName.Data(), i, fSuffixName.Data());

    fRawFileAscii[i].open(inputFileName.Data());
    if( fRawFileAscii[i].fail() ) { // end the reading if file opening failed
     cout << endl << "VMEBoardReader::Open(), cannot open file " << inputFileName << endl;
     isOk = false;
    } else {
     cout << endl << "VMEBoardReader::Open(), file " << inputFileName << " opened" << endl;
     isOk = true;
    }
   }
   valid &= isOk;

 } // end loop on sensors

  return valid;

}


//------------------------------------------+-----------------------------------
void VMEBoardReader::Close()
{
  //! Close input file.
  //
  // Upgraded, JB 2019/11/06

   for (Int_t i = 0; i < pGeoMap->GetNSensors(); ++i)
	  fRawFileAscii[i].close();
}

//------------------------------------------+-----------------------------------
//! Process
Bool_t VMEBoardReader::Process()
{
  // Upgraded, JB 2019/11/06

   Int_t size = (sizeof(MI26_FrameRaw)/4)*3 + 3;

   MI26_FrameRaw* data = new MI26_FrameRaw;

   // TAVTparGeo*  pGeoMap = (TAVTparGeo*)  fpGeoMap->Object();
   // TAVTparConf* pConfig = (TAVTparConf*) fpConfig->Object();

   Bool_t neof = false;
   // loop over boards
   for (Int_t i = 0; i < GetSensorsN(); ++i) {

      if (GetSensorEvent(i)) {
         neof |= true;
         ResetFrames();

         fIndex    = 0;

         // loop over frame (3 max)
         for (Int_t k = 0; k < 3; ++k) {
            if (GetFrame(i, data))
               DecodeFrame(i, data);
         }

         memset(fDataEvent, 0, size);
      } else {
         SetBit(kEof);
         SetBitAllDataOut(kEof);
      }
   }

   delete data;


   if (neof)
      return true;
   else
      return false;

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
Bool_t VMEBoardReader::GetSensorEvent(Int_t iSensor)
{
   Char_t tmp[255];

   fIndex = 0;
   UInt_t timestamp = 0;
   UInt_t dataPrev = 0;

   // lokking for header
   TString key  = Form("%x", GetKeyHeader(iSensor));
   TString tail = Form("%x", GetKeyTail(iSensor));

   do {
      fRawFileAscii[iSensor] >> tmp;
      TString line = tmp;

      UInt_t data = 0;
      sscanf(tmp, "%x", &data);
//      if (IsPartEqual(data, GetKeyHeader(iSensor)))
//        printf("toto\n");

      if (line.Contains(tail)) {
         printf("tito %d %08x\n", iSensor, dataPrev);
      }

      dataPrev = data;

      if (line.Contains(key)) {
         if(GetDebugLevel(1))
            printf("sensor header %s %d\n", tmp, (int) fRawFileAscii[iSensor].tellg()/9+1);

         // fpHisSensorKey[iSensor]->Fill(1);

         fDataEvent[fIndex++] = GetKeyHeader(iSensor);

         fRawFileAscii[iSensor] >> tmp;
         sscanf(tmp, "%x", &fEventNumber);
         fDataEvent[fIndex++] = fEventNumber;

         // trigger
         fRawFileAscii[iSensor] >> tmp;
         sscanf(tmp, "%x", &fTriggerNumber);
         fDataEvent[fIndex++] = fTriggerNumber;


         if(GetDebugLevel(3))
            printf("sensor %d: %d %d\n", iSensor, fTriggerNumber, fEventNumber);

         // fake time stamp
         fRawFileAscii[iSensor] >> tmp;
         sscanf(tmp, "%x", &timestamp);
         fDataEvent[fIndex++] = timestamp;

         // FillHistoEvt(iSensor);

         fPrevEventNumber[iSensor]   = fEventNumber;
         fPrevTriggerNumber[iSensor] = fTriggerNumber;
         fPrevTimeStamp[iSensor]     = fTimeStamp;
         break;
      }
   } while (!fRawFileAscii[iSensor].eof());

   if (fRawFileAscii[iSensor].eof()) return false;

   // look for trailer
   UInt_t data;

   do {
      fRawFileAscii[iSensor] >> tmp;
      TString line = tmp;
      sscanf(tmp, "%x", &data);
      fDataEvent[fIndex++] = data;

//      if (IsPartEqual(data, GetKeyTail(iSensor)))
//         printf("titi %d %08x\n", iSensor, data);

      if (line.Contains(tail)) {
         // fpHisSensorKey[iSensor]->Fill(2);
         break;
      }

      if (line.Contains(key)) {
         Int_t pos =  (int) fRawFileAscii[iSensor].tellg();
         fRawFileAscii[iSensor].seekg(pos-1);
         fIndex--;
         printf("key found\n");
      }

   } while (!fRawFileAscii[iSensor].eof());

   if (fRawFileAscii[iSensor].eof()) return false;

   fEventSize = fIndex;

   if(GetDebugLevel(3)) {
      for (Int_t i = 0; i < fEventSize; ++i)
         printf("Data %08x\n", fDataEvent[i]);
      printf("\n");
   }

   return true;
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
void VMEBoardReader::ResetFrames()
{
   fFirstFrame = 0;
   fFrameOk = true;
}


// --------------------------------------------------------------------------------------
void VMEBoardReader::AddPixel( Int_t iSensor, Int_t value, Int_t aLine, Int_t aColumn)
{
   // Add a pixel to the vector of pixels
   // require the following info
   // - input = number of the sensors
   // - value = analog value of this pixel
   // - line & column = position of the pixel in the matrix
   //
   // Upgraded, JB 2019/07/13

   if (fDebugLevel>2) printf("VMEBoardReader::Addpixel adding pixel for sensor %d with value %d line %d row %d\n", iSensor, value, aLine, aColumn);

   ListOfPixels.push_back( BoardReaderPixel( iSensor+1, value, aLine, aColumn, 0) );

 #ifdef withROOT
   if (ValidHistogram()) {
	  if (TAVTparConf::IsMapHistOn())
		 fpHisPixelMap[iSensor]->Fill(aLine, aColumn);

	  fpHisRateMap[iSensor]->Fill(aColumn);

	  for (Int_t i = 0; i < 4; ++i) {
		 if (aColumn >= 258*i && aColumn < (i+1)*258)
			fpHisRateMapQ[iSensor]->Fill(i+1);
	  }
   }
 #endif

}


// --------------------------------------------------------------------------------------
int VMEBoardReader::GetSensor(unsigned int key)
{

  // Upgraded, JB 2019/07/13

  if (fDebugLevel>2) printf("VMEBoardReader::GetSensor searching for sensor with key %x\n", key);

  key = (key >> 16) & 0xFFFF;
  for (Int_t i = 0; i <  fNumberOfSensors; ++i) {
     if (fgkKeyHeader[i] == key)
     if (fDebugLevel>2) printf("VMEBoardReader::GetSensor found sensor %d\n", i);
        return i;
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
Bool_t VMEBoardReader::DecodeFrame(Int_t iSensor, MI26_FrameRaw *frame)
{
  // Read the information of a frame for a given sensor
  // We use extensively the structure definined by Gille Clauss
  /*
   Events Words:
   1) Header;
   2) TriggerCnt;
   3) TimeStamp;
   4) FrameCnt;
   5) DataLength;
   6) ADataW16[1140]; //data MI26
   7) Trailer;
   */
   //
   // Upgrade, JB 2019/07/13

  if (fDebugLevel) printf("VMEBoardReader::DecodeFrame decoding event %d with size %d, sensor %d\n", fEventNumber, fEventSize);

   // TAVTntuRaw*  pNtuRaw = (TAVTntuRaw*)  fpNtuRaw->Object();
   // TAVTparConf* pConfig = (TAVTparConf*) fpConfig->Object();
   // TAVTparGeo*  pGeoPar = (TAVTparGeo*)  fpGeoMap->Object();
   // TAVTparMap*  pParMap = (TAVTparMap*) fpParMap->Object();

   Int_t dataLength    = ((frame->DataLength & 0xFFFF0000)>>16);
   // if (dataLength != fDataSize) fpHisFrameErrors[iSensor]->Fill(3);
   if (dataLength > MI26__FFRAME_RAW_MAX_W16) return false;

   Int_t dataSize = fDataSize*2;


   UShort_t *frameData = (UShort_t*)frame->ADataW16;
   dataLength         *= 2; // go to short

   if (ValidHistogram()) {
      fpHisEvtLength[iSensor]->Fill(frame->TriggerCnt % 10000, fDataSize);
      if (frame->TriggerCnt % 10000 == 0) fpHisEvtLength[iSensor]->Reset();
   }

   if (iSensor == -1) {
      Warning("DecodeFrame()", "Wrong header key %x\n", frame->Header);
      return false;
   }

   // -+-+- Pointers AND LOOP to usefull data, i.e. line and states
   MI26__TStatesLine* lineStatus;
   MI26__TState*      state;

   Int_t index = 0;
   int localNbOfPixels = 0;
   while( index < dataLength) { // Loop over usefull data


      if (index >= dataSize) return false;

      // first 16 bits word is the Status/Line
      lineStatus = (MI26__TStatesLine*)frameData;

      if(fDebugLevel>2)
         printf("frame %x %x #state %d Line add %d ovf %d\n", frameData[0], frameData[1], lineStatus->F.StateNb,
                lineStatus->F.LineAddr, lineStatus->F.Ovf);

      frameData += 1; // goto next word
      index += 2;

      if( lineStatus->F.Ovf > 0 ) { // Stop when overflow
         if(fEventsOverflow%1000==0 && fReadingEvent)
            printf("WARNING : overflow while reading %d at frame %d and sensor %d, total overflow number is %d\n",
                   fReadingEvent, frame->FrameCnt, iSensor, fEventsOverflow);
         fOverflow = true;
      }

      if(fDebugLevel>2)
         printf("  line %d, #states %d, overflow %d, reading event  %d\n",
                lineStatus->F.LineAddr, lineStatus->F.StateNb, lineStatus->F.Ovf, fReadingEvent);

      fNStatesInLine = 0;
      // Next words are the states if any
      for( Int_t iState = 0; iState < lineStatus->F.StateNb; ++iState ) { // loop over states found on sensor
         state = (MI26__TState*)frameData;
         frameData += 1; // goto next word
         index++;
         if(fDebugLevel>3)
            printf("                  number of states %d, number of hits %d,\n", lineStatus->F.StateNb, state->F.HitNb+1);

         fNStatesInLine++;

         // A state contains HitNb+1 pixels
         // the first pixel being on the left at the column ColAddr
         for( Int_t iPixel=0; iPixel < state->F.HitNb+1; iPixel++) { // loop on pixels in the state

            if(fDebugLevel>3)
               printf("   line %3d, col %3d\n", lineStatus->F.LineAddr, state->F.ColAddr+iPixel);

            // create a new pixel only if we are reading an event
            // and if the line is in the proper limit
            if (!lineStatus->F.Ovf) {
               Int_t planeId = pParMap->GetPlaneId(iSensor);
               AddPixel(planeId, 1, lineStatus->F.LineAddr, state->F.ColAddr+iPixel);
               localNbOfPixels++;
               if(fDebugLevel>3)
                  printf("sensor %d, line %d, col %d\n", iSensor, lineStatus->F.LineAddr, state->F.ColAddr+iPixel);
            }
         }

         if(fDebugLevel>3)
            printf("                  state %d, #pixels %d, column %d at mem.pos %ld\n",
                   iState, state->F.HitNb+1, state->F.ColAddr, (long int)state);
      } // end loop over states

      if( fDebugLevel>2 ) printf(" --> Current frame %d for sensor %d has %d pixels.\n", frame->FrameCnt, iSensor, localNbOfPixels);

   } // end loop over usefull data

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

///////// OLD METHODS
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
