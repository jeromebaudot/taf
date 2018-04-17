/////////////////////////////////////////////////////////////
// Class Description of IMGBoardReader                     //
//                                                         //
// Read raw data file from Imager Board,
// which corresponds to:
//  - non sparsified data,
//  - "analogue" data, that is encoded on a number of bits,
//  - "data" of a pixel includes usually 2 frames for CDS 
//    but a specific mode allows for more than 2 frames/data.
//
// See constructor IMGBoardReader for the list of options.
//  !!! some parameters are hardcoded in this constructor !!!
//
// The file format is described in the documents by Gilles Clauss:
//  mi32A_pxia_pxid_beam_test_file_format__template_files_v1.0.txt 
//  das_data_format_d7.txt
//
// Each event has a fixed size with a header and a data part.
// The data part is subdivided in inputs (=sensors, a priori), 
//  all with an equivalent lenght. Each input contains a number 
//  of rawdata values of a given bit size (NumberOfBitsValue).
// Then, there are two possibilities to extract a channel (=pixels) signal,
//  which is a SignificantBits bits word:
//  1) more than one rawdata value is needed,
//  2) a single rawdata value contains several channel signal
//  corresponds to a N-bits value.
// 
// Variables defining the class behavior (set by the constructor)
//  - NbOfInputs
//  - SizeOfEvent
//  - ifStripTelescope = 
//    If true very specific format for 8 planes of silicon strips.
//    This  format is HARD-CODED.
//  - ifMultiFrame = 
//  - First / LastTriggerChannel = limits where to look for trigger info in data
//  - First / LastExcludedChannel = limits of channel indexes not taken into account
//  - triggerLowThreshold/triggerHighThreshold = define interaval to identify a trigger
//
//
// Methods to interact externally with the class
//  - HasData() =
//    To be called for each event, get and decode the data
//    corresponding to next event. Update the IMGEvent object.
//  - SkipNextEvent = jump an event, ignoring its data.
//  - AddFile/AddFileList =
//    Add file names to the list of binary files to be considered for the readout
//
// Methods internal to the class
//  - IMGBoardReader = constructor, sets almost all control variables
//  - LookUpRawFile, OpenRawFile, CloseRawFile = raw data file management
//  - GetNextBuffer = load a full event in the memory (= Data buffer)
//  - SetBufferPointers = set the pointers to the different part of
//      the Data buffer (Header, inpu0, input1, ..., Trailer) for an event
//  - BuildValue( anAddress, wordSize) = return the content of the Data buffer
//      at anAddress and with length wordSize.
//  - GetInputData = extract the rawdata value(s) for each channel (=pixel or strip)
//    of each inputs and call the AddPixel method for it.
//  - AddPixel = build the channel signal associated with the given rawdata value. 
//
//                                                         //
/////////////////////////////////////////////////////////////
//
// JB, 2012/06/
// Last modified: SS 2012/08/01 number of bug fixes
// Last modified: JB 2012/08/18 upgraded management of multi-files and eof
// Last modified: JB 2012/08/18 trigger info added to event
// Last modified: SS 2012/08/21 new CDS in AddPixel
// Last modified: JB 2012/08/22,23 strip-telescope specific treatments
// Last modified: JB 2013/06/16-20 multi-frame specific treatments
// Last modified: JB 2013/07/19 AddPixel
// Last modified: JB 2014/01/28 IMGBoardReader
// Last modified: JB 2016/08/17 GetInputData
// Last modified: JB 2016/09/20 IMGBoardReader, GetInputData
// Last modified: JB 2017/03/03 IMGBoardReader, GetInputData
// Last modified: JB 2017/11/20 IMGBoardReader, SetZeroSuppression, AddPixel
// Last modified: JB 2018/03/18 SetBufferPointers

#include "IMGBoardReader.h"

ClassImp(IMGBoardReader)
ClassImp(IMGEvent)

// --------------------------------------------------------------------------------------

IMGBoardReader::IMGBoardReader( int boardNumber, int nInputs, int *nChannels, int sizeOfEvent, int eventsInFile, int headerSize, int trailerSize, int *numberOfBits, int *sigBits, int endian, int triggermode, int daqmode, int Ncolumns, int Nframes) {

  //  numberOfBits = size of the words (= rawdata values) to read from inputs
  //                 it may corresponds to one or more channels
  //    If numberOfBits<0, build signed signals for channels, unsigned otherwise.
  //
  //  sigBits = size of the word containing one channel signal 
  //            usually it encompasses the signals on two succesive frames
  //    If sigBits<0, do CDS (frame2-frame1) to build the final channel signal.
  // 
  //  daqmode = 0 standard mode (1 rawdata value = 2 frames), 
  //          = 1 strip-telescope mode (8 1st planes=strips)
  //          = 2 each rawdata value corresponds to more than 2 frames
  //            -> ifMultiFrame contains the number of rawdata values
  //               associated with one channel
  //		   Note that if CDS is required, 5 frames means 5 times frame0+frame1 
  //			and nb of columns in sensor needs to be known
  //          = 3 all frame1 pixel values comes first, then all frame 2 values 
  //
  //  strip-telescope mode :
  //   map a single input to 4 interleaved inputs 
  //    according to description in
  //    das_data_format_d7.txt by G.Clauss
  //
  //
  // JB, 2012/07/16
  // Modified: JB 2012/08/18 take into account nb of events in file
  // Modified: JB 2012/08/19 to allow different setup for each input
  // Modified: JB 2012/08/22 specific treatment when strip-telescope data
  // Modified: JB 2012/08/23 new argument daqmode to select strip-tel or not
  // Modified: JB 2013/06/16 daqmode 2 with new ifMultiFrame variable
  // Modified: JB 2014/01/28 manage case with or without trigger for multiFrame mode
  // Modified: JB 2014/11/20 new daqmode=3 for Maciej's DAQ (ifSplitFrames)
  // Modified: JB 2016/09/19 NValuesToJumpPerChannel updated to handle both CDS/non-CDS cases
  // Modified: JB 2017/03/03 new daqmode=4 to handle 16 parallel inputs DAQ
  // Modified: JB 2018/03/19 trailerSize introduced
  
  cout << "Creating IMGBoardReader for board " << boardNumber << " with " << nInputs << " inputs of " << nChannels[0] << " channels (for " << Ncolumns << " columns), event is " << sizeOfEvent << " bytes, " << eventsInFile << " events per file, header is " << headerSize << " bytes, each value has " << numberOfBits[0] << " bits among which " << sigBits[0] << " are significant and endianess is " << endian << ", trigger mode is " << triggermode << ", daq mode is " << daqmode << "." <<  endl;
  
  BoardNumber        = boardNumber;
  ifStripTelescope = false;
  SizeOfTrailer = trailerSize;
  ifMultiFrame = 0;
  ifSplitFrames = 0;
  if16outputs = 0;
  if( daqmode==1 ) {  // daq mode for strip telescope
    ifStripTelescope = true; 
    SizeOfTrailer = 4;
  }
  else if( daqmode==2 ) {  // daq mode with multiFrame, JB 2013/06/16
    ifMultiFrame = Nframes;
  }
  else if( daqmode==3 ) {  // daq mode for all frame1 values before all frame2 values
    ifSplitFrames = 1;
  }
  else if( daqmode==4 ) {
    if16outputs = 1;
    cout << "IMGBoardReader board: 16 parallel outputs DAQ mode detected!" << endl;
  }

  if( ifStripTelescope ) {
    // when strip telescope is read, the 4 first input from the config
    //  are to be turned into 1 physical input (hence -3)
    NbOfInputs         = nInputs; // was nInputs-3
    cout << "IMGBoardReader board " << boardNumber << ": strip-telescope configuration -> consider 4 imbricated channels  per value (..., i i i i, i+1, i+1, i+1, i+1, ...) for 2 first inputs!" << endl;
  }
  else {
    NbOfInputs         = nInputs;
  }

  if( ifMultiFrame ) {  // JB 2013/06/19
    triggerLowThreshold = 5000;
    triggerHighThreshold = 100000;
  }
  else {
    triggerLowThreshold = 0;
    triggerHighThreshold = 0;
  }

  SizeOfEvent        = sizeOfEvent;
  EventsInFile       = eventsInFile; // JB 2012/08/18
  SizeOfHeader       = headerSize;
  Data               = new char[SizeOfEvent];
  Endianness         = endian;
  IfZeroSupress      = 0;
  
  SetNumberOfColumns(Ncolumns);
  
  // Setup input parameters
  // JB 2012/08/19
  NbOfChannels       = new int[NbOfInputs];
  NumberOfBitsValue  = new int[NbOfInputs];
  SignificantBits    = new int[NbOfInputs];
  SignedValues       = new bool[NbOfInputs];
  InputDataAdress    = new int[NbOfInputs];
  MaxSignedValue     = new unsigned int[NbOfInputs];
  SizeOfInputData    = new size_t[NbOfInputs];
  SizeOfValue        = new size_t[NbOfInputs];
  NChannelsPerValue  = new int[NbOfInputs];
  NValuesToRead      = new int[NbOfInputs];
  WithCDS            = new bool[NbOfInputs];
  NValuesPerChannel  = new int[NbOfInputs]; // JB 2013/06/16
  NValuesToJumpPerChannel = new int[NbOfInputs];
  FirstTriggerChannel = new int[NbOfInputs];
  LastTriggerChannel = new int[NbOfInputs];
  FirstExcludedChannel = new int[NbOfInputs];
  LastExcludedChannel = new int[NbOfInputs];
                       
  for( int iInput=0; iInput<NbOfInputs; iInput++ ) { // loop on inputs

    if( ifStripTelescope && iInput<2 ) { // if strip telescope, use specific settings for the first input
      // one DAQ-input matches 8 planes of 256 strips
      //                    or 4 inputs of 2x256 channels
      // one value of 32 bits include 4 positive values of 8 bits
      // JB 2012/08/22, revised 2013/08/14
      NbOfChannels[iInput]       = nChannels[iInput];
      SignedValues[iInput]       = false;
      WithCDS[iInput]            = false;
      NumberOfBitsValue[iInput]  = 32;
      SignificantBits[iInput]    = 8;
      MaxSignedValue[iInput]     = (unsigned int)pow(2.,SignificantBits[iInput]);    
    } // end if strip telescope

    else { // other inputs use general settings
      NbOfChannels[iInput]       = nChannels[iInput];
      SignedValues[iInput]       = numberOfBits[iInput]<0; // if negative, we have signed values
      WithCDS[iInput]            = sigBits[iInput]<0; // if negative, CDS is required
      NumberOfBitsValue[iInput]  = (int)TMath::Abs(numberOfBits[iInput]);
      SignificantBits[iInput]    = (int)TMath::Abs(sigBits[iInput]);
      if (SignedValues[iInput]) { // max values depends on signed or unsigned, JB 2014/11/20
        MaxSignedValue[iInput]     = (unsigned int)pow(2.,SignificantBits[iInput]/2);
      }
      else {
        MaxSignedValue[iInput]     = (unsigned int)pow(2.,SignificantBits[iInput]);
      }
      
    }

    SizeOfValue[iInput]       = NumberOfBitsValue[iInput]/8/sizeof(char); 
    NChannelsPerValue[iInput] = NumberOfBitsValue[iInput]/SignificantBits[iInput];

    FirstTriggerChannel[iInput] = 9999999;
    LastTriggerChannel[iInput] = -9999999;    
    FirstExcludedChannel[iInput] = 9999999;
    LastExcludedChannel[iInput] = -9999999;
    if( ifMultiFrame ) {
      //cout << "NumberOfColumns = " << NumberOfColumns << ", ifMultiFrame = " << ifMultiFrame << endl;
      NValuesPerChannel[iInput]       = ifMultiFrame;
      if( WithCDS[iInput] ) { // if CDS needed, data are read by column hence Jump one column
        NValuesToJumpPerChannel[iInput] = NumberOfColumns; // 64 for MIMOSA-34, 128 for PIPPER-2;
      }
      else { // if no CDS, jump a full frame
        NValuesToJumpPerChannel[iInput] = NbOfChannels[iInput]/NChannelsPerValue[iInput]; // MIMOSA-22-SX
      }
      if( triggermode > 0 ) { // manage case with or without trigger, JB 2014/01/28
        FirstTriggerChannel[iInput] = NumberOfColumns; // 64 for MIMOSA-34, 128 for PIPPER-2;
        LastTriggerChannel[iInput] = FirstTriggerChannel[iInput]+NumberOfColumns-1;
        FirstExcludedChannel[iInput] = 0; // 0
        LastExcludedChannel[iInput] = 4*NumberOfColumns-1; // 4*64-1
      }
    }
    else {
      NValuesPerChannel[iInput] = 1;
      NValuesToJumpPerChannel[iInput] = 1;
    }
    
//    SizeOfValue[iInput]       = NumberOfBitsValue[iInput]/8/sizeof(char); 
//    NChannelsPerValue[iInput] = NumberOfBitsValue[iInput]/SignificantBits[iInput];
    NValuesToRead[iInput]     = NbOfChannels[iInput]*NValuesPerChannel[iInput]/NChannelsPerValue[iInput];
    SizeOfInputData[iInput]   = NValuesToRead[iInput]*SizeOfValue[iInput];
    //SizeOfInputData[iInput]    = NbOfChannels[iInput]*NValuesPerChannel[iInput]*SignificantBits[iInput]/8/sizeof(char);

    
    cout << "  - Input " << iInput << ": " << NbOfChannels[iInput] << " channels with " << SignificantBits[iInput] << " bits, in " << NValuesToRead[iInput] << " values with " << NumberOfBitsValue[iInput] << " bits ( " << SizeOfValue[iInput] << " Bytes) so " << NChannelsPerValue[iInput] << " channel per value AND " << NValuesPerChannel[iInput] << " values per channel (distant by " << NValuesToJumpPerChannel[iInput] << " values), signed? = " << SignedValues[iInput] << ", max value is " << MaxSignedValue[iInput] << ", total input size is " << SizeOfInputData[iInput] << " Bytes, CDS needed? = " << WithCDS[iInput] << "." << endl;
    if( ifMultiFrame ) {
      cout << "     ! multi-frame configuration ( " << ifMultiFrame << " frames/RawEvent ) " << endl;
      cout << "         -> CDS is " << (WithCDS[iInput]?"":"NOT") << " required, # values to jump per channel = " << NValuesToJumpPerChannel[iInput] << endl;    
      cout << "         -> search for trigger info between channels " << FirstTriggerChannel[iInput] << " and " << LastTriggerChannel[iInput] << " in a value range from " << triggerLowThreshold << " to " << triggerHighThreshold << "!" << endl;    
    }
    
  } // end loop on inputs
  

  BuffersRead        = 0; // JB 2012/08/18
  NumberOfFiles      = 0;
  CurrentFileNumber  = 0;
  NoMoreFile         = false;
  
  ReadingEvent       = false; 
  CurrentEvent       = 0; // Allow to know wether data are correct, JB 2009/05/26
  CurrentEventNumber = 0;
  CurrentTriggerNumber = 0;
  CurrentFrameNumber = 0;
  CurrentTimestamp   = 0;
  TriggerMode        = triggermode; // define limits of events: 0=with trigger, 1=with frames


  // Initialise statistics
  EventsCount = 0;
  FramesRead  = 0;

}

// --------------------------------------------------------------------------------------

IMGBoardReader::~IMGBoardReader() {

  // JB, 2008/09/27
  delete CurrentEvent;
  delete InputFileName;
  delete Data;
  ListOfTriggers.clear();
  ListOfTimestamps.clear();
  ListOfFrames.clear();
  ListOfPixels.clear();

  delete[] NbOfChannels;
  delete[] NumberOfBitsValue;
  delete[] SignificantBits;
  delete[] SignedValues;
  delete[] InputDataAdress;
  delete[] MaxSignedValue;
  delete[] SizeOfInputData;
  delete[] SizeOfValue;
  delete[] NChannelsPerValue;
  delete[] NValuesToRead;
  delete[] WithCDS;
  delete[] NValuesPerChannel;
  delete[] NValuesToJumpPerChannel;
  delete[] FirstTriggerChannel;
  delete[] LastTriggerChannel;
  delete[] FirstExcludedChannel;
  delete[] LastExcludedChannel;

}

// --------------------------------------------------------------------------------------

void IMGBoardReader::SetZeroSuppression(int aThreshold) {

  // Used to set zero suppression and define the threshold
  //
  // JB 2017/11/20

  IfZeroSupress = 1;
  ZeroThreshold = aThreshold;
  
  cout << endl << " ==> ZERO SUPPRESSION used with threshold " << ZeroThreshold << endl;
  
}

// --------------------------------------------------------------------------------------

bool IMGBoardReader::AddFile(char *fileName) {

  // JB, 2008/09/27

  NumberOfFiles++;
  CurrentFileNumber = 0;
  InputFileName = fileName;
#ifndef STANDALONE
  sprintf(InputFileName,"%s", fTool.LocalizeDirName( InputFileName)); // JB 2011/07/07
#endif
  RawFileStream.open( InputFileName);
  if( RawFileStream.fail() ) {
    cout << endl << "ERROR IMGBoardReader " << BoardNumber << " file " << InputFileName << " does not exist!" << endl;
    return false;
  }
  cout << endl << "  --> IMGBoardReader " << BoardNumber << " New file " << InputFileName << endl;  

  return true;
}

// --------------------------------------------------------------------------------------
bool IMGBoardReader::AddFileList(const char *prefixFileName, int startIndex, int endIndex, const char *suffixFileName) {

  // Store the inputs to be able to read several files one after the other
  // JB, 2008/10/8
  // Modified: JB 2010/10/05 to allow a first index file not 1 and read file#>10
  // Modified: SS 2012/08/01 really add several files
  // Modified: JB 2012/08/18 consider endIndex as inclusive
  // Modified: JB 2013/10/04 Fix behavior of NumberOfFiles and CurrentFileNumber
  // Modified: JB 2017/11/20 allows to find files without index at second try

  bool rc = true;
  int nbFilesOK = 0;
  
  if(DebugLevel>0) cout <<  "Indices: start = " << startIndex << ", end = " << endIndex << endl;
  NumberOfFiles = endIndex-startIndex+1; // Better way to handle non-0 start index, JB 2013/10/04
  CurrentFileNumber = 0;
  SuffixFileName = new char[10];
  PrefixFileName = new char[300];
  ListOfInputFileNames = new char*[endIndex-startIndex+1];
  sprintf( PrefixFileName, "%s", prefixFileName);
  sprintf( SuffixFileName, "%s", suffixFileName);
  /*if(DebugLevel>0)*/ cout << "IMGBoardReader " << BoardNumber << " adding " << NumberOfFiles << " files like " << prefixFileName << "*" << SuffixFileName << endl;

  // Try filenames using an index
  for(int FileNumberToAdd=0; FileNumberToAdd<=endIndex-startIndex;FileNumberToAdd++){
  	InputFileName = new char[300];
  	sprintf( InputFileName, "%s%d%s", PrefixFileName, FileNumberToAdd+startIndex, SuffixFileName);
#ifndef STANDALONE
  	sprintf(InputFileName,"%s", fTool.LocalizeDirName( InputFileName));
#endif
    if(DebugLevel>0) cout << "  trying file " << InputFileName << endl;
  	RawFileStream.open( InputFileName);
  	if( RawFileStream.fail() ) {
  		cout << endl << "ERROR IMGBoardReader " << BoardNumber << " file " << InputFileName << " does not exist!!" << endl;
      RawFileStream.close();
  		rc = false;
  	}
  	else{
      nbFilesOK++;
  		ListOfInputFileNames[FileNumberToAdd]=InputFileName;
  		cout << "  --> IMGBoardReader " << BoardNumber << " New file " << ListOfInputFileNames[FileNumberToAdd] << ", total of " << FileNumberToAdd+1 << " files." << endl;
      RawFileStream.close();
    }
  }
  cout << "#files OK = " << nbFilesOK << " out of " << NumberOfFiles << endl;

  // If there is only one file, try a filename without an index (JB 2017/04/05)
  if ( !rc && NumberOfFiles == 1 ) {
    sprintf( InputFileName, "%s%s", PrefixFileName, SuffixFileName);
#ifndef STANDALONE
    sprintf(InputFileName,"%s", fTool.LocalizeDirName( InputFileName));
#endif
    /*if(DebugLevel>0)*/ cout << "  trying file " << InputFileName << endl;
    RawFileStream.open( InputFileName);
    if( RawFileStream.fail() ) {
      cout << endl << "ERROR IMGBoardReader " << BoardNumber << " file " << InputFileName << " does not exist!!" << endl;
      RawFileStream.close();
      rc = false;
    }
    else{
      ListOfInputFileNames[0]=InputFileName;
      cout << "  --> IMGBoardReader " << BoardNumber << " New file " << ListOfInputFileNames[0] << ", total of " << 1 << " files." << endl;
      RawFileStream.close();
      rc = true;
    }
  }
  
  // if at least some files are OK, JB 2017/11/20
  else if ( !rc && nbFilesOK > 0 ) {
    rc = true;
  }
  
  if( rc)  OpenRawFile(ListOfInputFileNames[0]); //reopens the very first file
//   RawFileStream.clear();
  return rc;
}

// --------------------------------------------------------------------------------------
bool IMGBoardReader::LookUpRawFile() {
  
  // Try to open the next rawdata file.
  // The file name is decided upon
  
  if( CurrentFileNumber < NumberOfFiles-1) {
    
    

//      if( CurrentFileNumber ) { // only close if not first file
      CloseRawFile();
      CurrentFileNumber++;
//     }
    if(DebugLevel) cout << "  --> IMGBoardReader " << BoardNumber << " New file to read " << CurrentFileNumber+1 << " < " << NumberOfFiles << " closing and opening." << endl;
    InputFileName = ListOfInputFileNames[CurrentFileNumber];
    return OpenRawFile( InputFileName);

/*
    if( CurrentFileNumber<10 ) {
      sprintf( InputFileName, "%s000%d%s", PrefixFileName, CurrentFileNumber, SuffixFileName);
    }
    else if( CurrentFileNumber<100 ) {
      sprintf( InputFileName, "%s00%d%s", PrefixFileName, CurrentFileNumber, SuffixFileName);
    }
    else if( CurrentFileNumber<1000 ) {
      sprintf( InputFileName, "%s0%d%s", PrefixFileName, CurrentFileNumber, SuffixFileName);
    }
    else {
      sprintf( InputFileName, "%s%d%s", PrefixFileName, CurrentFileNumber, SuffixFileName);
    }
    sprintf(InputFileName,"%s", fTool.LocalizeDirName( InputFileName)); // JB 2011/07/07
 */
//     RawFileStream.open( InputFileName); //SS 2012.08.09 - commented out as it repeats OpenRawFile()
//     return true;

  } // end if( CurrentFileNumber < NumberOfFiles)
  
  else if ( NumberOfFiles==0 ) {
    cout << "ERROR: IMGBoardReader NO RAW DATA FILE WAS ASSOCIATED WITH BOARD " << BoardNumber << ", STOPPING!" << endl << endl;
    return false;
  }
  else { // Otherwise no more file, end the reading    
    cout << "  --> IMGBoardReader " << BoardNumber << ": No more files to read " << CurrentFileNumber+1 << " >= " << NumberOfFiles << " closing!" << endl;
    CloseRawFile();
    NoMoreFile = true;
    return false;
  }

}

// --------------------------------------------------------------------------------------
bool IMGBoardReader::OpenRawFile( const char *fileName) {
  
  // Open a File of a Run 
  
  RawFileStream.open( fileName);
  bool b = RawFileStream.fail();
  if (b == 0)  {
    BuffersRead = 0;
    cout << " -+-+- INFO IMGBoardReader " << BoardNumber << ": File " << fileName << " opened." << endl;
  }
  else {
    cout << " -/-/- INFO IMGBoardReader " << BoardNumber << ": File " << fileName << " not opened, rc = " << b << "." << endl;
  }
  return !b;
  
}

// --------------------------------------------------------------------------------------
bool IMGBoardReader::CloseRawFile() {
  // Closes a File of a Run 
  
  RawFileStream.close();
  bool b = RawFileStream.fail();
  if (b == 0) 
    cout << " -+-+- INFO IMGBoardReader " << BoardNumber << ": File " << CurrentFileNumber << " closed " << endl;
  else
    cout << " -/-/- INFO IMGBoardReader " << BoardNumber << ":  File " << CurrentFileNumber << " not closed, rc = " << b << "(eof="<< RawFileStream.eof() << ", bad="<< RawFileStream.bad() <<"." << endl;
  return b;
  
}

// --------------------------------------------------------------------------------------

bool IMGBoardReader::GetNextBuffer( ) {

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
  
  // If we have read the full current Data buffer alreay,
  // try to get a new Data buffer
  //  either from the current file or a new one
  if ( !( BuffersRead>=EventsInFile || RawFileStream.eof() ) || LookUpRawFile() ) {
    
    // Now we can get the next data buffer
    RawFileStream.read(reinterpret_cast<char *> ( &Data[0] ), sizeof(char) * SizeOfEvent);
    if(DebugLevel>2) cout << "  IMGBoardReader " << BoardNumber << ": Got new data buffer " << BuffersRead << " with gcount=" << RawFileStream.gcount() << " bytes, SizeOfEvent=" << SizeOfEvent << endl;
    readSuccess = ( (size_t)RawFileStream.gcount() == SizeOfEvent );
    BuffersRead++;
  }
  return readSuccess;

}

// --------------------------------------------------------------------------------------

bool IMGBoardReader::SetBufferPointers( ) {
  
  // Set the pointers to the differrent buffers,
  //  event header and each input.
  //
  // JB, 2012/07/20
  // Modified: JB 2012/08/19 to allow different setup for each input
  // Modified: JB 2012/08/21 corrected address compuation
  // Modified: JB 2014/11/20 SizeOfTrailer introduced
  // Modified: JB 2018/03/18 Consider no even trailer case
  
  if( Data==NULL) {
    cout << "WARNING in IMGBoardReader board " << BoardNumber << ", event pointer is null!" << endl; 
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

  if( SizeOfTrailer==0 ) {
    EventTrailer = (int)0x89abcdef;
  }
  else if( SizeOfTrailer>4 ) {
    EventTrailer = BuildValue( SizeOfEvent - SizeOfTrailer + 4, 4);    
  }
  else {
    EventTrailer = BuildValue( SizeOfEvent - SizeOfTrailer, 4);
  }
    
  FramesRead++;

  if( DebugLevel>1 ) cout << " SetBufferPointers for IMGBoard " << BoardNumber << " done, trailer is " << hex << EventTrailer << dec << endl;
  
//  EventTrailer = (int)0x89abcdef; // use this to trick the final test....only if you know what you are doing!
  
  return (EventTrailer == (int)0x89abcdef);
  
}

// --------------------------------------------------------------------------------------

void IMGBoardReader::PrintEventHeader( ) {
  
  // Print the buffer information.
  //
  // From Gilles Claus documentation 17/07/2012
  //VFasCnt[0] = Trigger line
  //VFasCnt[1] = Trigger frame = id of the frame on which trigger occurs
  //VFasCnt[2] = Number of trigger received since beginning of run
  //VFasCnt[3] = Number of trigger accepted by DAQ since beginning of run
  //
  //VFasReg[0] = Counter of Mi26 Sync pulse read on current trigger / event
  //VFasReg[1] = Current value of counter of Mi26 Sync pulse since beginning of run
  //VFasReg[2] = Reserved
  //VFasReg[3] = Acquisition state ( number of "Acq lines" acquired => should be 128 )
  //
  //
  // JB, 2012/07/20 (copy of what was in MAF)
  
  if( EventHeader==NULL) {
    cout << "WARNING in IMGBoardReader board " << BoardNumber << ", header pointer is null!" << endl; 
    return;
  }
  
  printf("EvTrig     %10d \n",EventHeader->EvTrig);
  printf("EvNo       %10d \n", EventHeader->EvNo);
  printf("EvPos      %10d \n", EventHeader->EvPos);
  printf("EvTag      %10d \n",EventHeader->EvTag);
  printf("EvDate     %10d \n",EventHeader->EvDate);
  printf("EvTime     %10d \n",EventHeader->EvTime);
  printf("TrigCnt    %10d \n",EventHeader->TrigCnt);
  printf("EvVmeTime  %10d ms \n",EventHeader->EvVmeTime);
  for( int ifas=0; ifas<NB_MAX_VFAS; ifas++) {
    printf("FAS %2d: Trigger position  %4d, Nb of frames skipped  %4d \n", ifas, EventHeader->VFasCnt[ifas], EventHeader->VFasReg[ifas]);
  }    
  printf("EvNetTime  %10d ms \n",EventHeader->EvNetTime);
  printf("MeasNo     %5d \n",EventHeader->MeasNo);
  printf("EvInMeasNo %5d \n",EventHeader->EvInMeasNo);
  
  printf("\nEvent trailer 0x%x \n", EventTrailer);
  
}

// --------------------------------------------------------------------------------------

void IMGBoardReader::GetInputData( int mode) {

  // Naviguate through the rawdatas in order to:
  // If mode = 0 (default)
  //  Build the rawdata(s) for each channel (=pixel or strip) of each inputs.
  //  Create the pixel from these rawdata(s).
  //
  // If mode = 1 
  //  Search for triggers in a specific channel range.
  // 
  // Associating rawdatas to channels takes into account that:
  //  - several rawdatas (=values) may be needed to built a channel signal,
  //  - one rawdata may contain more than one channel signal.
  // 
  //
  //
  // Add to the event list of frames each frame tested.
  //
  // JB, 2012/07/20
  // Modified: JB 2012/08/19 allow to read several channels from one value
  // Modified: JB 2013/06/16 allow to read several values from one channel
  // Modified: JB 2013/06/17 allow to read trigger info from special input
  // Modified: JB 2013/06/20 use a mode, to decide what to do
  // Modified: JB 2013/06/20 also update frame counter
  // Modified: JB 2016/08/17 correction for multiframe mode (compatible with run 34939)
  // Modified: JB 2016/09/20 upgrade of multiframe mode to handle both CDS and non-CDS case

  if( Data==NULL) {
    cout << "WARNING in IMGBoardReader board " << BoardNumber << ", event pointer is null!" << endl;
    return;
  }
  
  if( mode == 0 ) {
    for ( int iFrame=0; iFrame<NValuesPerChannel[0]; iFrame++) { // loop on frames
      ListOfFrames.push_back( iFrame);
      if( WithCDS) ListOfFrames.push_back( iFrame+NValuesPerChannel[0]);
    }
  }
  
  for( int iInput=0; iInput<NbOfInputs; iInput++ ) { // loop on inputs
    
    unsigned int value = 0;
    unsigned int second_value = 0;
    unsigned int rawdata = 0;
    unsigned int second_rawdata = 0;
    unsigned int *rawdatas = new unsigned int[NValuesPerChannel[iInput]];
    int valuesReadForCurrentChannel = 0;
    int address = 0, shifta = 0, shiftb = 0, shiftc = 0;
    char goal[10] = "PIXELS";
    int channel = 0;
    int totalValuesToRead = NValuesToRead[iInput];
    
    // Initialise depending on mode
    if( mode == 1 ) { // trigger search
      sprintf( goal, "%s", "TRIGGERS");
      channel = FirstTriggerChannel[iInput];
      totalValuesToRead = (LastTriggerChannel[iInput]-FirstTriggerChannel[iInput])*NValuesPerChannel[iInput]/NChannelsPerValue[iInput];
    }
    
    // build a mask depending on how many significant bits are required
    unsigned int mask = (unsigned int)pow(2.,SignificantBits[iInput])-1;
    
    if( DebugLevel>2 ) {
      printf( "    GetInputData for %s input %d, with %d values and %d channels per value and %d values per channel; mask is 0x%x\n", goal, iInput, totalValuesToRead, NChannelsPerValue[iInput], NValuesPerChannel[iInput], mask);
    }
    
    for( int iValue=0; iValue<totalValuesToRead; iValue++ ) { // loop on values
      
      // if all the values of the previous channel have been read
      //  increment the channel
      if ( valuesReadForCurrentChannel>=NValuesPerChannel[iInput] ) {
        channel += NChannelsPerValue[iInput]; //was ++, JB 2013/08/14
        valuesReadForCurrentChannel=0;
      }
      
      // Compute the address of the next value to read depending on
      //  the number of values already read for this channel (a)
      //  and the number of channels already read (b)
      // When ifMultiFrame add (c)
      if( ifMultiFrame ) {
        if( WithCDS[iInput] ) { // if CDS ( MIMOSA-32, MIMOSA-34, PIPPER)
          shifta = valuesReadForCurrentChannel*NValuesToJumpPerChannel[iInput]*SizeOfValue[iInput]; // previous computation, before 2014
          // 	 channel/NChannelsPerValue[iInput]*NValuesPerChannel[iInput]*SizeOfValue[iInput]  // new computation, after 2014/01, BUT does not work with run 34939 !
          // for some reason, the computation does not work if not using (channel/j)*j ... even if it looks stupid
          shiftb = (channel/NValuesToJumpPerChannel[iInput])*NValuesToJumpPerChannel[iInput]*NValuesPerChannel[iInput]*SizeOfValue[iInput];
          shiftc = (channel%NValuesToJumpPerChannel[iInput])*SizeOfValue[iInput];
        }
        else { // if no CDS (MIMOSA-22-SX)
          shifta = valuesReadForCurrentChannel*NValuesToJumpPerChannel[iInput]*SizeOfValue[iInput]; // previous computation, before 2014
          //        + channel/NChannelsPerValue[iInput]*NValuesPerChannel[iInput]*SizeOfValue[iInput]  // new computation, after 2014/01, BUT does not work with run 34939 !
          shiftb = channel/NChannelsPerValue[iInput]*1*SizeOfValue[iInput];
          shiftc = 0;
        }
        // 	if( shiftc != 0 ) cout << "shiftc not zero!" << endl;
      } // end if multiframe
      
      else {
        // Note that in this case NValuesPerChannel=1 AND NValuesToJumpPerChannel=1
        shifta = valuesReadForCurrentChannel*NValuesToJumpPerChannel[iInput]*SizeOfValue[iInput];
        shiftb = channel/NChannelsPerValue[iInput]*NValuesPerChannel[iInput]*SizeOfValue[iInput];
        shiftc = 0;
      }
      address = InputDataAdress[iInput] + shifta + shiftb + shiftc;
      
      if( DebugLevel>2 ) printf( "    Getting next value with a shift of %d (=%d+%d+%d) bytes computed for channel %d, currentValue %d, ChannelsPerValue %d, ValuesPerChannel %d, ValuesToJumpPerChannel %d, sizeOfValue %d bytes\n", address-InputDataAdress[iInput], shifta, shiftb, shiftc, channel, valuesReadForCurrentChannel, NChannelsPerValue[iInput], NValuesPerChannel[iInput], NValuesToJumpPerChannel[iInput], (Int_t)SizeOfValue[iInput]);
      
      
      // value is a word of SizeOfValue Bytes
      //  it contains nChannelsPerValue channels
      value = BuildValue( address, (int)SizeOfValue[iInput] );
      
      // If splitFrames option is ON, it means a second value (Frame2)
      //  is to be read at the address = adressOfFrame1 + all frame size
      if ( ifSplitFrames ) {
        second_value = BuildValue( address+totalValuesToRead*SizeOfValue[iInput], (int)SizeOfValue[iInput] );
      }
      
      if( DebugLevel>3 ) {
        printf( "      got value[%d] = 0x%x, nb %d for channels %d to %d\n", iValue, value, valuesReadForCurrentChannel, channel, channel+NChannelsPerValue[iInput]-1);
      }
      
      for( int iChannel=channel; iChannel<channel+NChannelsPerValue[iInput]; iChannel++ ) { // loop on channels contained in a value
        rawdata = value & mask;
        rawdatas[valuesReadForCurrentChannel] = rawdata;
        value   = value >> SignificantBits[iInput];
        if( DebugLevel>3 ) {
          printf( "         channel[%d] = 0x%x (value is now 0x%x) (0-supp on = %d, threshold = %d)\n", iChannel, rawdata, value, IfZeroSupress, ZeroThreshold);
        }
        if( valuesReadForCurrentChannel == NValuesPerChannel[iInput]-1 ) {
          
          if ( mode==1 && FirstTriggerChannel[iInput]<=iChannel && iChannel<=LastTriggerChannel[iInput] ) {
            DecodeTriggerFromData( iInput, iChannel-FirstTriggerChannel[iInput], rawdatas, NValuesPerChannel[iInput]);
          }
          
          else if ( mode==0
                   && !(FirstTriggerChannel[iInput]<=iChannel
                        && iChannel<=LastTriggerChannel[iInput])
                   && !(FirstExcludedChannel[iInput]<=iChannel
                        && iChannel<=LastExcludedChannel[iInput])
                   ) { // standard pixel
            
            if ( ifSplitFrames ) { // JB 2014/11/20
              second_rawdata = second_value & mask;
              rawdatas[valuesReadForCurrentChannel] = rawdata - second_rawdata;
              if( DebugLevel>3 ) {
                printf( "      got second value[%d] = 0x%x, nb %d for channels %d to %d\n", iValue, second_value, valuesReadForCurrentChannel, channel, channel+NChannelsPerValue[iInput]-1);
                printf( "         second channel[%d] = 0x%x\n", iChannel, second_rawdata);
                printf( "         CDS for channel[%d] = 0x%x\n", iChannel, rawdatas[valuesReadForCurrentChannel]);
              }
            }
            
            AddPixel( iInput, rawdatas, NValuesPerChannel[iInput], iChannel);
            
          }
          
        }
      } // end loop on channels contained in a value
      
      valuesReadForCurrentChannel++;
      
    } // end loop on values
    
    delete[] rawdatas; // reduce memory leak, BH 2013/08/21
  } // end loop on inputs
  
}

// --------------------------------------------------------------------------------------

void IMGBoardReader::GetTriggerData( ) {
  
  // Get the information on the trigger position
  //
  //
  // JB, 2012/08/18
  // Modified: JB 2013/06/20 allow to search for trigger in data
  
  ListOfTriggers.push_back( EventHeader->VFasCnt[0] ); // get trigger in header
  
  if( ifMultiFrame ) { // search for trigger in data, JB 2013/06/20
    ListOfTriggersFromData.clear();
    GetInputData( 1); 
    ListOfTriggers.insert( ListOfTriggers.end(), ListOfTriggersFromData.begin(), ListOfTriggersFromData.end());
  }

  if( DebugLevel ) {
    printf(" Total triggers found in event = %d: at lines =", (int)ListOfTriggers.size());
    for ( int iTrig=0; iTrig<(int)ListOfTriggers.size(); iTrig++ ) {
      printf(" %d", ListOfTriggers.at(iTrig) );
    }
    printf("\n");
  }
  
}

// --------------------------------------------------------------------------------------

int IMGBoardReader::DecodeTriggerFromData( int input, int rowNumber, unsigned int *rawdatas, int nFrames) {
    
  // From the rawdata of each frame (more than one if ifMultiframe)
  //  test if the value is in the expected range for a trigger.
  //  If so, add the corresponding line number in the trigger list.
  //
  // JB 2013/06/17
  // Modified: JB 2013/06/23 detect trigger up front, not just value
  
  int nFoundTriggers = 0;
  bool triggerUp = false;
  int triggerRow;
  signed int value;
  //signed int previousValue;
  signed short int *frames = new signed short int[nFrames*2];
  
  // Re-order the frames in a "physical" order,
  //  knowing that 
  for ( int iFrame=0; iFrame<nFrames; iFrame++) { // loop on daq frames
    frames[iFrame]   = rawdatas[iFrame] / MaxSignedValue[input];
    frames[iFrame+nFrames] = rawdatas[iFrame] % MaxSignedValue[input];      
    if( DebugLevel>3 ) printf( "    raw[%d] = 0x%8x -> frame[%d] = 0x%4x, frame[%d] = 0x%4x\n", iFrame, rawdatas[iFrame], iFrame, frames[iFrame], iFrame+nFrames, frames[iFrame+nFrames]);      
  } // end loop on daq frames

  // Search for a trigger in the physical frames.
  // Do not consider two triggers on consecutive lines with same value,
  //  this is a "feature" of the DAQ system and only ONE trigger shall be kept.
  for ( int iFrame=0; iFrame<2*nFrames; iFrame++) { // loop on physical frames
    value = (int)frames[iFrame];
    triggerRow = rowNumber + iFrame*NValuesToJumpPerChannel[input];
    
    if( triggerLowThreshold <= value && value <= triggerHighThreshold ) {
      // Create a new trigger only if
      //  there was none seen in the previous rows (triggerUp would be true),
      //  it is not the same as the one registered in the EventHeader.
      if( !triggerUp && abs(triggerRow-EventHeader->VFasCnt[0])>1 ) {
        triggerUp = true;
        //previousValue = value;
        if( DebugLevel>1 ) printf( "    found trigger nb %d at row %d and frame %d with value 0x%4x or %d\n", nFoundTriggers, triggerRow, iFrame, value, value);
        ListOfTriggersFromData.push_back( triggerRow);
        nFoundTriggers++;
      }
      else if( DebugLevel>1 ) {
        printf("    trigger %d still up at row %d\n", nFoundTriggers, triggerRow);
      }
    }
    else {
      triggerUp = false;
    }
    
  } // end loop on physical frames
  
  delete[] frames; // remove memory leaks, BH 2013/08/21
  return nFoundTriggers;
  
}

// --------------------------------------------------------------------------------------

void IMGBoardReader::AddPixel( int input, unsigned int rawdata, int index) {
  
  // Compute the signal value of the channel (or pixel) from the rawdata value
  // and add the pixel to the vector of pixels
  //
  // THIS VERSION STILL DOES NOT HAVE THE CORRECT POLARIZATION COMPUTATION
  //
  // Requires the following info
  // - input = input number of the board
  // - rawdata = rawdata value containing the two frames embedded (if CDS)
  // - index = index of this pixel on this input
  //
  // JB, 2008/09/27
  // Modified: SS, 2012/08/01 signed frame value
  // Modified: JB, 2012/08/18 CDS or no-CDS possibility
  // Modified: SS, 2012/08/21 correct CDS according to trigger position
  // Modified: JB, 2012/08/22 specific input-index setup when strip-telescope
  // Modified: JB, 2013/08/18 trim 0 when significantBit==1 (binary output)
  // Modified: JB, 2017/11/20 new mode for zero suppression
  
  int trueInput = input;
  int trueIndex = index;

  if( ifStripTelescope ) { // specific settings when strip-telescope read
    // re-shuffle the channel of the first input to inputs 0-4
    // other inputs correspond to input+3
    if( input<2 ) {
      trueIndex = index/4+(index%4)*NbOfChannels[input]; //  was index/4
      trueInput = input; // was index%4
    }
    else {
      trueIndex = index;
      trueInput = input; // was input+3
    }
  }
  
  //unsigned int value;
  signed int value;
  
  if( !WithCDS[input] ) { // if no CDS needed
    value = rawdata;
    if( DebugLevel>3 ) {
      printf( "    input %d, channel %d: raw = 0x%8x, value = 0x%8x or %d\n", input, index, rawdata, value, value);      
    }  
  }
  
  else { // With CDS
    //SS 2012.08.10 frame1 and frame2 should be signed short int to keep values between -32768 and +32678
//    signed short int *frames = new signed short int[NValuesPerChannel[input]];
//    signed int *values = new signed int[NValuesPerChannel[input]];
//    for( int iFrame=0; iFrame<NValuesPerChannel[input]; iFrame++ ) {
//      frames[iFrame] = rawdata >> SignificantBits[input];
//      values[iFrame] = frames[iFrame]-frames[(iFrame>0?iFrame-1:0)];
//      if( DebugLevel>3 ) printf( "      frame[%d] = 0x%x or %d, value = 0x%x or %d, raw = 0x%x\n\n", iFrame, frames[iFrame], frames[iFrame], values[iFrame], values[iFrame], rawdata);      
//    }
    
    signed short int frame1 = rawdata / MaxSignedValue[input];
    signed short int frame2 = rawdata % MaxSignedValue[input];    

    // Decision of polarity
    // from TriggerLine
    // SS 2012/08/21
/*    int TriggerLine2=TriggerLine/5;
    int TriggerLine3=TriggerLine%64;
    int PixelLine=index%64; //here the transformation from index to PixelLine is hardcoded for Mimosa 32 which has 64 lines.
    if (TriggerLine2>63) {
      if (PixelLine>TriggerLine3) {
        value=frame1-frame2;
      }
      else{
        value=frame2-frame1;
      }
    }
    else{
      if (PixelLine>TriggerLine3) {
        value=frame2-frame1;
      }
      else{
        value=frame1-frame2;
      }
    } */
    // CDS using absolute value.
    //  the absolute value is set in DPlane depending on the #events / init
    value = (int)(frame1 - frame2);
    if( DebugLevel>3 ) printf( "    input %d, channel %d: raw = 0x%8x, frame1 = 0x%4x, frame2 = 0x%4x, value = 0x%4x or %d (0-supp on = %d, threshold = %d)\n", input, index, rawdata, frame1, frame2, value, value, IfZeroSupress, ZeroThreshold);

//    if (ifMultiFrame) {
//      value = values[5]-values[4];
//    }
//    else {
//      value = values[1]-values[0];
//    }
//    
//    if( DebugLevel>3 ) printf( "    input %d, channel %d: raw = 0x%x, value = 0x%4x or %d\n", input, index, rawdata, value, value);      
    
  } // end with CDS
  
  // If binary output, create a pixel only if non-zero value, JB 2013/08/18
  //   test if zero suppression is required
  if ( abs(SignificantBits[input])!=1 || value!=0 ) {
    if( IfZeroSupress == 0 || value > ZeroThreshold ) {
      ListOfPixels.push_back( IMGPixel( trueInput, value, trueIndex));
    }
  }
  
}

// --------------------------------------------------------------------------------------

void IMGBoardReader::AddPixel( int input, unsigned int *rawdatas, int nFrames, int index) {
  
  // Compute the signal value of the channel (or pixel) from the rawdata value
  // and add the pixel to the vector of pixels
  //
  // THIS VERSION STILL DOES NOT HAVE THE CORRECT POLARIZATION COMPUTATION
  //
  // Requires the following info
  // - input = input number of the board
  // - rawdatas = array of nFrames rawdata values,
  //              each containing two frames embedded (if CDS)
  // - index = index of this pixel on this input
  //
  // How pixel values are computed if nFrames>1
  //  - if no CDS required, simply associate each rawdata to one frame value,
  //  - if CDS required, re-organize the frames in the following way:
  //    In the case of nFrames = 5 => 2x5=10 frames are contained in rawdatas
  //                                  and 9 CDS values will be build
  //    frame[0] = frame1_rawdatas[0]
  //    frame[1] = frame1_rawdatas[1]
  //    ...
  //    frame[3] = frame1_rawdatas[3]
  //    frame[4] = frame1_rawdatas[4]
  //    frame[5] = frame2_rawdatas[0]
  //    frame[6] = frame2_rawdatas[1]
  //    ...
  //    frame[8] = frame2_rawdatas[3]
  //    frame[9] = frame2_rawdatas[4]
  // and
  //    event 0 = frame[0]-frame[1]
  //    event 1 = frame[1]-frame[2]
  //    ...
  //    event 7 = frame[7]-frame[8]
  //    event 8 = frame[8]-frame[9]
  //
  //
  // JB, 2008/09/27
  // Modified: SS, 2012/08/01 signed frame value
  // Modified: JB, 2012/08/18 CDS or no-CDS possibility
  // Modified: SS, 2012/08/21 correct CDS according to trigger position
  // Modified: JB, 2012/08/22 specific input-index setup when strip-telescope
  // Modified: JB, 2013/06/19 manage multi-frame rawdatas
  // Modified: JB, 2013/07/19 memory leak fixed with delete frames
  // Modified: JB, 2017/03/03 new mode for if16outputs
  // Modified: JB, 2017/11/20 new mode for zero suppression

  int trueInput = input;
  int trueIndex = index;
  
  if( ifStripTelescope ) { // specific settings when strip-telescope read
    // re-shuffle the channels of the first input to input 0-3
    // other inputs correspond to input+3
    if( input<2 ) {
      trueIndex = index/4+(index%4)*NbOfChannels[input]/4; //  was index/4
      trueInput = input; // was index%4
    }
    else {
      trueIndex = index;
      trueInput = input; // was input
    }
  }
  
  if( if16outputs ) { // Maciej's DAQ for MIMOSA-22-SX, 2017/03/03
    // 1U16 => [127,119,111,103,95,...,23,15,7] u=0
    // 2U16 => [126,118,110,102,94,...,22,14,6] u=1
    // .
    // 8U16 => [120,112,104,196,88,...,16, 8,0] u=7
    Int_t u = index/16;
    Int_t b = index%16;
    trueIndex = 7 - (u-1)%8 + 128*((u-1)/8) + b*8;
    //printf( "   trueIndex = %5d (or %5d) from index=%5d with u=%4d and b=%2d\n", trueIndex, 8 - u%8 + b*8 + 128*(u/8), index, u, b);
  }
  
  //unsigned int value;
  signed int value;
  
  if( !WithCDS[input] ) { // if no CDS needed
    for ( int iFrame=0; iFrame<nFrames; iFrame++) { // loop on frames
      value = rawdatas[iFrame];
      // If binary output, create a pixel only if non-zero value, JB 2013/08/18
      //printf( "     (value!=0):%d (sigBit(%d)!=1):%d -> new pixel = %d\n", value!=0, SignificantBits[input], abs(SignificantBits[input])!=1, abs(SignificantBits[input])!=1 || value!=0);
      if ( abs(SignificantBits[input])!=1 || value!=0 ) {
        if( IfZeroSupress == 0 || value > ZeroThreshold ) {
          ListOfPixels.push_back( IMGPixel( trueInput, value, trueIndex));
        }
        if( DebugLevel>3 ) {
          printf( "    input %d(true %d), channel %d(true %d): raw = 0x%8x, value = 0x%8x or %d for frame %d\n", input, trueInput, index, trueIndex, rawdatas[iFrame], value, value, iFrame);      
        }  
      }
    } // end loop on frames
  } 
  
  else { // With CDS
    //SS 2012.08.10 frame1 and frame2 should be signed short int to keep values between -32768 and +32678
    //    signed short int *frames = new signed short int[NValuesPerChannel[input]];
    //    signed int *values = new signed int[NValuesPerChannel[input]];
    //    for( int iFrame=0; iFrame<NValuesPerChannel[input]; iFrame++ ) {
    //      frames[iFrame] = rawdata >> SignificantBits[input];
    //      values[iFrame] = frames[iFrame]-frames[(iFrame>0?iFrame-1:0)];
    //      if( DebugLevel>3 ) printf( "      frame[%d] = 0x%x or %d, value = 0x%x or %d, raw = 0x%x\n\n", iFrame, frames[iFrame], frames[iFrame], values[iFrame], values[iFrame], rawdata);      
    //    }
    
    signed short int *frames = new signed short int[nFrames*2];
    
    // Re-order the frames in a "physical" order,
    //  knowing that 
    for ( int iFrame=0; iFrame<nFrames; iFrame++) { // loop on daq frames
//      frames[iFrame*2]         = rawdatas[iFrame] / MaxSignedValue[input];
//      frames[iFrame*2+1]       = rawdatas[iFrame] % MaxSignedValue[input];
//      if( DebugLevel>3 ) printf( "    raw[%d] = 0x%8x -> frame[%d] = 0x%4x, frame[%d] = 0x%4x\n", iFrame, rawdatas[iFrame], iFrame*2, frames[iFrame*2], iFrame*2+1, frames[iFrame*2+1]);
      frames[iFrame]         = rawdatas[iFrame] / MaxSignedValue[input];
      frames[iFrame+nFrames] = rawdatas[iFrame] % MaxSignedValue[input];
      if( DebugLevel>3 ) printf( "    raw[%d] = 0x%8x -> frame[%d] = 0x%4x, frame[%d] = 0x%4x\n", iFrame, rawdatas[iFrame], iFrame, frames[iFrame], iFrame+nFrames, frames[iFrame+nFrames]);
    } // end loop on daq frames
    
    // Build pixel signal for each physical events following
    //  signal(frame i+1) = frame[i]-frame[i+1] with i<2*nFrames-1
    // CURRENTLY: CDS using absolute value.
    //  the absolute value is set in DPlane depending on the #events / init
    //

//    for ( int iFrame=0; iFrame<nFrames; iFrame++) { // loop on physical frames
    
//      value = (int)(frames[2*iFrame] - frames[2*iFrame+1]);

//      if( DebugLevel>3 ) printf( "    input %d(true %d), channel %d(true %d), timestamp %d: frame1 = 0x%4x, frame2 = 0x%4x, value = 0x%4x or %d\n", input, trueInput, index, trueIndex, iFrame, frames[2*iFrame], frames[2*iFrame+1], value, value);

    for ( int iFrame=0; iFrame<2*nFrames-1; iFrame++) { // loop on physical frames
      value = (int)(frames[iFrame] - frames[iFrame+1]);

      if( DebugLevel>3 ) printf( "    input %d(true %d), channel %d(true %d), timestamp %d: frame1 = 0x%4x, frame2 = 0x%4x, value = 0x%4x or %d (0-supp on = %d, threshold = %d)\n", input, trueInput, index, trueIndex, iFrame, frames[iFrame], frames[iFrame+1], value, value, IfZeroSupress, ZeroThreshold );
      
      /*if( iFrame==4)*/ 
      /*if( 2<iFrame && iFrame<7)*/
      if( IfZeroSupress == 0 || value > ZeroThreshold ) {
        ListOfPixels.push_back( IMGPixel( trueInput, value, trueIndex, iFrame));
      }
      
    } // end loop on physical frames

    delete[] frames; // fix memory leak, JB 2013/07/19
      
  } // end with CDS
  
  
}

// --------------------------------------------------------------------------------------

unsigned int IMGBoardReader::BuildValue( int anAddress, int wordSize) {
  
  // Return a rawdata value build from the data pointer "Data"
  //  at adress anAdress and with size in bytes given by wordSize.
  // Note that words of 1 Byte are read two at a time from the data pointer.
  //
  // Pay attention in case Endianness involved that we swapp halves 
  //  within the full word.
  // So for 32 bits we swap 16 bits packets, for 16 bits, we swapp the 8 bits packets 
  //
  // JB 2012/07/20
  
  unsigned int rawdata = 0;
  unsigned int shifter = 0x01;
  unsigned int multiplyer = 0x100;
  
  if( DebugLevel>3 ) {
    cout << "  building value from address " << anAddress << " over " << wordSize << " bytes";
    printf( " raw = ");
    for(int k = wordSize-1; k >= 0; k-=1){
      printf("0x%2x ", (0xFF & Data[anAddress+k]) );
    }
    printf(" \n");
  }

    //--- to decode one endian
    if( Endianness==1 ) {
      shifter = 0x100;      
      for (int k = wordSize-1; k >= 0; k-=2){
        rawdata += shifter * (0x00FF & Data[anAddress + k-1]);
//        if( DebugLevel>3 ) printf( " endian 1: k-1=%d, data = 0x%2x, shifter = 0x%8x, raw = 0x%8x\n", k-1, Data[anAddress + k-1], shifter, rawdata);
        shifter /= multiplyer;
        rawdata += shifter * (0x00FF & Data[anAddress + k]);
//        if( DebugLevel>3 ) printf( " endian 1: k=%d, raw = 0x%8x, shifter = 0x%8x\n", k, rawdata, shifter);
      }
    }
    //--- to decode the other endian
    else{    
      for (int k = 0; k < wordSize; k+=2){
        rawdata += shifter * (0x00FF & Data[anAddress + k]);
        shifter *= multiplyer;
//        if( DebugLevel>3 ) printf( " endian 1: k+1=%d, data = 0x%2x, shifter = 0x%8x, raw = 0x%8x\n", k+1, Data[anAddress + k+1], shifter, rawdata);
         rawdata += shifter * (0x00FF & Data[anAddress + k+1]);
        shifter *= multiplyer;
//        if( DebugLevel>3 ) printf( " endian 0: k=%d, raw = 0x%8x, shifter = 0x%8x\n", k, rawdata, shifter);
      }
    }
  
  if( DebugLevel>3 ) {
    printf( "   -> built value = ");
    for(int k = wordSize-1; k >= 0; k-=1){
      printf("0x%2x ", 0xFF & (rawdata>>(k*8)) );
    }
    printf(" \n");
  }

  return rawdata;
}

// --------------------------------------------------------------------------------------

void IMGBoardReader::SkipNextEvent() {

  // This method is used to ignore the next event
  //
  // JB, 2009/05/26

  if(DebugLevel) printf("  IMGBoardReader: %d Resetting readout because of bad event\n", BoardNumber);
  ReadingEvent       = false; 
  //CurrentEventNumber = 0;
  //CurrentFrameNumber = 0;
  //CurrentTimestamp   = 0;
  CurrentEvent       = 0; // Allow to know wether data are correct, JB 2009/05/26

}

// --------------------------------------------------------------------------------------

bool IMGBoardReader::HasData( ) {

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

  //bool eventDone = false;   // we start with event incomplete
  bool eventOK   = true;    // we start with a good event by default
  CurrentEvent   = 0;       // Allow to know wether data are correct
  ListOfTriggers.clear(); // and clear the vectors
  ListOfTimestamps.clear();
  ListOfFrames.clear();
  ListOfPixels.clear();

  if(DebugLevel>0) {
    cout << endl ;
    cout << "  BoardNumber " << BoardNumber << " IMGBoardReader::HasData() - readingEvent " << ReadingEvent << " currentEvent " << CurrentEventNumber << " currentTrigger " << CurrentTriggerNumber << " currentFrame " <<	CurrentFrameNumber << endl;
  }

  if( GetNextBuffer() ) {
    
    eventOK = SetBufferPointers();
    if( DebugLevel>1 ) PrintEventHeader();

    if( eventOK) {
      GetTriggerData(); // JB 2012/08/18, called prior GetInputData since 2013/06/20
      GetInputData();
      
      CurrentEvent = new IMGEvent( EventHeader->EvNo, BoardNumber, &ListOfTriggers, &ListOfTimestamps, &ListOfFrames, &ListOfPixels);
      if( DebugLevel>1 ) cout << " BoardNumber " << BoardNumber << " create new event " << EventHeader->EvNo << " with " << ListOfPixels.size() << " pixels." << endl;
      EventsCount++;
      //eventDone = true;
    }
    else{
    	cout<<"IMGBoardReader::HasData() - Event is not ok!"<<endl;
    }
    
  } // getting next buffer was OK
  else{
  	cout<<"IMGBoardReader::HasData() - Can't get next buffer (# " << BuffersRead <<")!"<<endl;
  }
  
  

  // End
  // check there are still data to read
  return eventOK && !NoMoreFile;

}

// --------------------------------------------------------------------------------------

void IMGBoardReader::PrintStatistics(ostream &stream) {

  // Print statistics on the events read by this board
  //
  // JB, 2010/08/26

  stream << "***********************************************" << endl;
  stream << " Board IMG " << BoardNumber << " found:" << endl;
  stream << EventsCount << " events in total," << endl;
  stream << FramesRead << " frames detected while reading an event." << endl;
  stream << "***********************************************" << endl;

}

// --------------------------------------------------------------------------------------

void IMGBoardReader::DumpEventHeaders( int nFrames) {
  
  // Dump the information of acquisition, frame and triggers,
  //  for nFrames frames.
  // The info is printed into a file.
  //
  // This method is intended to be used in standalone mode,
  //  without any actual analysis of the data.
  // Once TAF is launched:
  //  instantiate an object: IMGBoardReader board(0,1,16*64,4212,20000,112,-32,-32,0,0,0);
  //  add the files board.AddFileList("/Users/jeromeb/Data/cmos/mi32/pxie_June2013/32705/RUN_32705_",0,2,".rz")
  //  call the method: board.DumpEventHeaders( 1000);
  //
  // JB, 2013/06/23
  
  Char_t fileName[300];
  sprintf( fileName, "Results/IMG_headers_dump.root");
#ifndef STANDALONE
  sprintf( fileName,"%s", fTool.LocalizeDirName( fileName));
#endif
  FILE *outFile;
  outFile = fopen( fileName, "w");

  for( int framesRead = 0; framesRead<nFrames; framesRead++ ) {
    
    fprintf( outFile, "**********************************" );
    
    if( GetNextBuffer() ) {
      
      if( SetBufferPointers() ) {
        GetTriggerData();
        
        fprintf( outFile, "EvTrig     %10d \n",EventHeader->EvTrig);
        fprintf( outFile, "EvNo       %10d \n", EventHeader->EvNo);
        fprintf( outFile, "EvPos      %10d \n", EventHeader->EvPos);
        fprintf( outFile, "EvTag      %10d \n",EventHeader->EvTag);
        fprintf( outFile, "EvDate     %10d \n",EventHeader->EvDate);
        fprintf( outFile, "EvTime     %10d \n",EventHeader->EvTime);
        fprintf( outFile, "TrigCnt    %10d \n",EventHeader->TrigCnt);
        fprintf( outFile, "EvVmeTime  %10d ms \n",EventHeader->EvVmeTime);
        for( int ifas=0; ifas<NB_MAX_VFAS; ifas++) {
          fprintf( outFile, "FAS %2d: Trigger position  %4d, Nb of frames skipped  %4d \n", ifas, EventHeader->VFasCnt[ifas], EventHeader->VFasReg[ifas]);
        }    
        fprintf( outFile, "EvNetTime  %10d ms \n",EventHeader->EvNetTime);
        fprintf( outFile, "MeasNo     %5d \n",EventHeader->MeasNo);
        fprintf( outFile, "EvInMeasNo %5d \n",EventHeader->EvInMeasNo);
        
        fprintf( outFile, " Total triggers found in event = %d: at lines =", (int)ListOfTriggers.size());
        for ( int iTrig=0; iTrig<(int)ListOfTriggers.size(); iTrig++ ) {
          fprintf( outFile, " %d", ListOfTriggers.at(iTrig) );
        }
        fprintf( outFile, "\n");

      }
      else{
        cout<<"IMGBoardReader::DumpEventHeader() - Event is not ok!"<<endl;
      }
      
    } // getting next buffer was OK
    else{
      cout<<"IMGBoardReader::DumpEventHeader() - Can't get next buffer!"<<endl;
    }
    
  }
  
  fclose( outFile);
  
  cout << endl << "OUTPUT IN " << fileName << endl;
  
}

// --------------------------------------------------------------------------------------

IMGEvent::IMGEvent( int eventNumber, int boardNumber, vector<int> *listOfTriggers, vector<int> *listOfTimestamps, vector<int> *listOfFrames, vector<IMGPixel> *listOfPixels) {

  // JB, 2008/09/27

  EventNumber    = eventNumber;
  BoardNumber    = boardNumber;
  ListOfTriggers = listOfTriggers;
  ListOfTimestamps = listOfTimestamps;
  ListOfFrames   = listOfFrames;
  ListOfPixels   = listOfPixels;

}

// --------------------------------------------------------------------------------------

IMGEvent::~IMGEvent() {

  // crashing for the time bein...?
  // JB, 2008/09/27

//   delete ListOfTriggers;
//   delete ListOfTimestamps; //RDM150509
//   delete ListOfPixels;
//   delete ListOfFrames;

}

