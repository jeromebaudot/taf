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
// vetoPixel mechanism introduced, JB, 2020/02/17

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

  fVetoOverflow = false; // if true, reject events with overflow

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
  // delete fData;
}

//------------------------------------------+-----------------------------------
int VMEBoardReader::Open()
{
  // Open all ascii files, one file per channel/sensor.
  // Return false if one file cannot be opened, true otherwise.
  // Assume various option for the datapath (w or wo the run number in)
  //
  //
  // Upgraded, JB 2020/02/17

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

    // Try with a first file format
    inputFileName = Form("%s/%s%d%s", fPathName.Data(), fPrefixName.Data(), i, fSuffixName.Data());
    fRawFileAscii[i].open(inputFileName.Data());
    if( fRawFileAscii[i].fail() ) { // end the reading if file opening failed

     // try again by including the run number in the directory path assuming format runXXXX where XXXX=run number
     if( fRunNumber<100 ) { inputFileName = Form("%s/run00%d/%s%d%s", fPathName.Data(), fRunNumber, fPrefixName.Data(), i, fSuffixName.Data()); }
     else if( fRunNumber<1000 ) {  inputFileName = Form("%s/run0%d/%s%d%s", fPathName.Data(), fRunNumber, fPrefixName.Data(), i, fSuffixName.Data()); } 
     else  { inputFileName = Form("%s/run%d/%s%d%s", fPathName.Data(), fRunNumber, fPrefixName.Data(), i, fSuffixName.Data()); }
     fRawFileAscii[i].open(inputFileName.Data());
     if( fRawFileAscii[i].fail() ) { // end the reading if file opening failed
 
     // try again by including the run number in the directory path assuming
      inputFileName = Form("%s/%d/%s%d%s", fPathName.Data(), fRunNumber, fPrefixName.Data(), i, fSuffixName.Data());
      fRawFileAscii[i].open(inputFileName.Data());
      if( fRawFileAscii[i].fail() ) { // end the reading if file opening failed
       cout << endl << "VMEBoardReader::Open(), cannot open file " << inputFileName << endl;
       isOk = false;
      }
     }
    }

    cout << endl << "VMEBoardReader::Open(), file " << inputFileName << " opened" << endl;
    isOk = true;
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

   for (Int_t i = 0; i < GetSensorsN(); ++i)
	  fRawFileAscii[i].close();
}

// --------------------------------------------------------------------------------------

void VMEBoardReader::SetVetoPixel( int noiseRun) {

  // Select the required function to veto the pixel depending on the run number.
  // JB 2020/02/07

  if( fDebugLevel) printf( "  VMEBoardReader board %d::SetVetoPixel with noise run number %d\n", fBoardNumber, noiseRun);
  fTool.SetVetoPixel( noiseRun);

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

  if ( Process() ) {
    if (fDebugLevel) printf("VMEBoardReader::HasData new event %d will be generated with %lu pixels, %lu frames and %lu triggers\n", fEventNumber, ListOfPixels.size(), ListOfFrames.size(), ListOfTriggers.size());
    fCurrentEvent = new BoardReaderEvent( fEventNumber, fBoardNumber, fRunNumber, &ListOfPixels);
    fCurrentEvent->SetListOfTriggers( &ListOfTriggers);
    fCurrentEvent->SetListOfFrames( &ListOfFrames);
    return true;
  }

  else {
    if (fDebugLevel) printf("VMEBoardReader::HasData wrong event building...stopping!\n");
    return false;
  }

}

//------------------------------------------+-----------------------------------
Bool_t VMEBoardReader::Process()
{
  // Actually process the analysis of one event
  //
  // Upgraded, JB 2019/11/07

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
         // SetBit(kEof);
         // SetBitAllDataOut(kEof);
      }
   }

   delete data;


   if (neof)
      return true;
   else
      return false;

}

// --------------------------------------------------------------------------------------
Bool_t VMEBoardReader::GetSensorEvent(Int_t iSensor)
{
  // Upgraded, JB 2019/11/06

  if (fDebugLevel>2) printf("Getting sensor %d event\n", iSensor);

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
         printf("  tito %d %08x\n", iSensor, dataPrev);
      }

      dataPrev = data;

      if (line.Contains(key)) {
         if(GetDebugLevel() > 1)
            printf("  sensor header %s %d\n", tmp, (int) fRawFileAscii[iSensor].tellg()/9+1);

         // fpHisSensorKey[iSensor]->Fill(1);

         fDataEvent[fIndex++] = GetKeyHeader(iSensor);

         fRawFileAscii[iSensor] >> tmp;
         sscanf(tmp, "%x", &fEventNumber);
         fDataEvent[fIndex++] = fEventNumber;

         // trigger
         fRawFileAscii[iSensor] >> tmp;
         sscanf(tmp, "%x", &fTriggerNumber);
         fDataEvent[fIndex++] = fTriggerNumber;

         if(GetDebugLevel() > 2)
            printf("  sensor %d: trig %d evt %d\n", iSensor, fTriggerNumber, fEventNumber);

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

   if(GetDebugLevel() > 2) printf(" Sensor %d, event size %d:\n", iSensor, fEventSize);
   if(GetDebugLevel() > 3) {
      for (Int_t i = 0; i < fEventSize; ++i)
         printf("    Data %08x\n", fDataEvent[i]);
      printf("\n");
   }

   return true;
}


// --------------------------------------------------------------------------------------
Bool_t VMEBoardReader::GetFrame(int iSensor, MI26_FrameRaw* data)
{
  // Updated, JB 2019/11/07

  if (fDebugLevel>1) printf("Getting a new frame for sensor %d, current index %d / event size %d, nb of frames read so far: %d\n", iSensor, fIndex, fEventSize, fFramesReadFromFile);

  Char_t tmp[255];
  fDataSize = 0;

  if (fIndex >= fEventSize -2) return false;

  // find header
  do {
     if (fDataEvent[fIndex] == GetFrameHeader()) {
        fData[fDataSize++] = fDataEvent[fIndex];
        break;
     }
  } while (fIndex++ < fEventSize);
  if (fDebugLevel>2) printf("  frame header %x found at index %d over event size %d\n", GetFrameHeader(), fIndex, fEventSize);

  if (fIndex >= fEventSize -2) return false;

  fIndex++;

  // find trailer
  UInt_t key1  =  GetFrameTail() & 0xFFFF;
  UInt_t key2  = (GetFrameTail() & 0xFFFF0000) >> 16;

  do {
    fData[fDataSize++] = fDataEvent[fIndex];
    if (( (fDataEvent[fIndex] & 0xFFFF) == key1) || ( (fDataEvent[fIndex] & 0xFFFF0000) >> 16) == key2) {
    // if( fDataEvent[fIndex] == GetKeyTail(iSensor) ) {
      break;
    }
  } while (fIndex++ < fEventSize);
  if (fDebugLevel>2) printf("  frame trailer %x found at index %d over event size %d\n", GetKeyTail(iSensor), fIndex, fEventSize);

  memcpy(data, &fData[0], sizeof(MI26_FrameRaw));
  // FillHistoFrame(iSensor, data);

  fDataSize -= fgkFrameHeaderSize; // removing header

  if(GetDebugLevel()>2) {
    printf( " For sensor %d:\n", iSensor);
     for (Int_t i = 0; i < fDataSize+fgkFrameHeaderSize; ++i)
        printf("   Data %08x\n", fData[i]);
     printf("\n");
  }

  if (fIndex >= fEventSize -2) return false;

  fFramesReadFromFile++;
  if (fDebugLevel>1) printf("  => New frame %d OK for sensor %d with data size %d\n", fFramesReadFromFile, iSensor, fDataSize);
  return true;

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
   // Check if there is a veto on certain pixels
   //
   // Upgraded, JB 2020/02/17

   bool add = fTool.VetoPixel==NULL || !fTool.VetoPixel( iSensor, aLine, aColumn);

   if (fDebugLevel>2) printf("VMEBoardReader::Addpixel adding pixel for sensor %d with value %d line %d row %d, veto-add=%d\n", iSensor, value, aLine, aColumn, add);

   if( !add ) return;

   if ( fTool.VetoPixel==NULL || !fTool.VetoPixel( iSensor, aLine, aColumn) ) {
    ListOfPixels.push_back( BoardReaderPixel( iSensor+1, value, aLine, aColumn, 0) );
   }

 #ifdef withROOT
   // if (ValidHistogram()) {
	 //  if (TAVTparConf::IsMapHistOn())
		//  fpHisPixelMap[iSensor]->Fill(aLine, aColumn);
   //
	 //  fpHisRateMap[iSensor]->Fill(aColumn);
   //
	 //  for (Int_t i = 0; i < 4; ++i) {
		//  if (aColumn >= 258*i && aColumn < (i+1)*258)
		// 	fpHisRateMapQ[iSensor]->Fill(i+1);
	 //  }
   // }
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
Bool_t VMEBoardReader::DecodeFrame(Int_t iSensor, MI26_FrameRaw *frame)
{
  // Read the information of a frame for a given sensor
  // We use extensively the structure definined by Gille Clauss
  //
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
   // Upgraded, JB 2019/07/13

  if (fDebugLevel>1) printf("VMEBoardReader::DecodeFrame decoding event %d with size %d, sensor %d\n", fEventNumber, fEventSize, iSensor);

   // TAVTntuRaw*  pNtuRaw = (TAVTntuRaw*)  fpNtuRaw->Object();
   // TAVTparConf* pConfig = (TAVTparConf*) fpConfig->Object();
   // TAVTparGeo*  pGeoPar = (TAVTparGeo*)  fpGeoMap->Object();
   // TAVTparMap*  pParMap = (TAVTparMap*) fpParMap->Object();

   Int_t dataLength    = ((frame->DataLength & 0xFFFF0000)>>16);
   // if (dataLength != fDataSize) fpHisFrameErrors[iSensor]->Fill(3);
   //if (dataLength > MI26__FFRAME_RAW_MAX_W16) return false;

   Int_t dataSize = fDataSize*2;


   UShort_t *frameData = (UShort_t*)frame->ADataW16;
   dataLength         *= 2; // go to short

   // if (ValidHistogram()) {
   //    fpHisEvtLength[iSensor]->Fill(frame->TriggerCnt % 10000, fDataSize);
   //    if (frame->TriggerCnt % 10000 == 0) fpHisEvtLength[iSensor]->Reset();
   // }

   if (iSensor == -1) {
      Warning("DecodeFrame()", "Wrong header key %x\n", frame->Header);
      return false;
   }

   // Some Statistics, only for first sensor
   if (iSensor == 0) {
     ListOfFrames.push_back( frame->FrameCnt);
     ListOfTriggers.push_back( frame->TriggerCnt);
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
               AddPixel(iSensor, 1, lineStatus->F.LineAddr, state->F.ColAddr+iPixel);
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

  if( fDebugLevel>1 ) printf("VMEBoardReader::DecodeFrame  --> Current frame %d for sensor %d has %d pixels.\n", frame->FrameCnt, iSensor, localNbOfPixels);

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
