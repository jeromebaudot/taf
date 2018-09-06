/////////////////////////////////////////////////////////////
// Class Description of BoardReaderIHEP
//                                      
// Dedicated to decode output files from the IHEP acquisition system
//  dedicated to MIMOSA-28 sensors
// 
// This class wraps up the original code of Mingyi DONG, IHEP, 2018/02/11
//                                      
/////////////////////////////////////////////////////////////
//
// created JB, 2018/06/03

#include "BoardReaderIHEP.h"

ClassImp(BoardReaderIHEP);

//------------------------------------------+-----------------------------------
BoardReaderIHEP::BoardReaderIHEP(int boardNumber, int runNumber, int numberOfSensors, int triggerMode, int eventBuildingMode) {

  vi_Verbose = 12;
  fDebugLevel = 0;
  
  fBoardNumber = boardNumber;
  fRunNumber = runNumber;
  fNumberOfSensors = numberOfSensors;

  cout << "*****************************************" << endl;
  cout << "    < BoardReaderIHEP constructor >      " << endl;
  cout << "*****************************************" << endl;  
  cout << "Creating a BoardReaderIHEP" << endl;
  cout << " * for board : " << fBoardNumber << endl;
  cout << " * for runNumber : " << fRunNumber << endl;
  cout << " * for sensor type : MIMOSA-28" << endl;
  cout << " * nb of sensors : " << fNumberOfSensors << endl;  

  cout << " * the Data format  is                : "
  << std::setw(15) << "hex"
  << std::setw(15) << "dec"
  << std::setw(15) << "Pointer" << endl;

  fNumberOfFiles = 0;
  fCurrentFileNumber = -1;

  fEventNumber = 0;
  fCurrentEvent = NULL;
//  ifs_DataRaw = NULL;
  
}

//------------------------------------------+-----------------------------------
BoardReaderIHEP::~BoardReaderIHEP()
{

//  delete fCurrentEvent;
//  delete fInputFileName;

}

// --------------------------------------------------------------------------------------

bool BoardReaderIHEP::AddFile(const char *fileName) {
  
//  fNumberOfFiles++;
//  fCurrentFileNumber = 0;
//  char inputFileName[500] = fileName;
////  sprintf(inputFileName,"%s", fTool.LocalizeDirName( inputFileName));
//  fRawFileStream.open( inputFileName);
//  if( fRawFileStream.fail() ) {
//    cout << endl << "ERROR BoardReaderIHEP " << fBoardNumber << " file " << inputFileName << " does not exist!" << endl;
//    return false;
//  }
//  fListOfInputFileNames.push_back( inputFileName);
//  cout << "  --> BoardReaderIHEP " << fBoardNumber << " New file " << fListOfInputFileNames.back() << ", total of " << fListOfInputFileNames.size() << " files." << endl;
//  
//  fCurrentInputFileName = fListOfInputFileNames.begin();
//
//  ifs_DataRaw.close();
//  ifs_DataRaw.clear();
//  ifs_DataRaw.open( fileName, ios::in|ios::binary);
//  if (!ifs_DataRaw.good())
//  {
//    cout << "ERROR : Mi28DecodeLadderDataToRoot(), Failed to open input file: " << fileName << endl;
//    return false;
//  }
//  else
//  {
//    cout << "INFO : Mi28DecodeLadderDataToRoot(), Successfully open input file: " << vs_DataRaw << endl;
//  }
//
//  /* Get the size of the binary file */
//  ifs_DataRaw.seekg(0, ios::beg);
//  vi_Pointer_FileBegin = ifs_DataRaw.tellg();
//  ifs_DataRaw.seekg(0, ios::end);
//  vi_Pointer_FileEnd = ifs_DataRaw.tellg();
//  vi_N_FileSize = vi_Pointer_FileEnd - vi_Pointer_FileBegin;
//  ifs_DataRaw.seekg(0, ios::beg);
//  cout << "INFO : Mi28DecodeLadderDataToRoot(), The input file size is : " << vi_N_FileSize << endl;
//  cout << "------------------------------------------------------"   << endl;
//  
  return true;
}

// --------------------------------------------------------------------------------------
//bool BoardReaderIHEP::AddFileList(const char *prefixFileName, int startIndex, int endIndex, const char *suffixFileName) {
//  
//  // Build the list of rawdata files from start index to endindex
//  
//  char inputFileName[500];
//  
//  if(fDebugLevel>0) cout <<  "Indices: start = " << startIndex << ", end = " << endIndex << endl;
//  cout << "BoardReaderIHEP " << BoardNumber << " adding " << fNumberOfFiles << " files like " << prefixFileName << "*" << suffixFileName << endl;
//  for(int fileNumber=0; fileNumber<=endIndex-startIndex; fileNumber++){
//    sprintf( inputFileName, "%s%d%s", prefixFileName, fileNumber+startIndex, suffixFileName);
////    sprintf( inputFileName,"%s", fTool.LocalizeDirName( inputFileName));
//    if(fDebugLevel>0) cout << "  checking file " << inputFileName << endl;
//    fRawFileStream.open( inputFileName);
//    if( fRawFileStream.fail() ) {
//      cout << endl << "ERROR BoardReaderIHEP " << fBoardNumber << " file " << inputFileName << " does not exist!!" << endl;
//      fRawFileStream.close();
//      return false;
//    }
//    else{
//      fListOfInputFileNames.push_back( inputFileName);
//      cout << "  --> BoardReaderIHEP " << fBoardNumber << " New file " << fListOfInputFileNames.back() << ", total of " << fListOfInputFileNames.size() << " files." << endl;
//      fRawFileStream.close();
//    }
//  }
//  //OpenRawFile(ListOfInputFileNames[0]); //reopens the very first file
//  //   RawFileStream.clear();
//
//  fCurrentInputFileName = fListOfInputFileNames.begin();
//  
//}

// --------------------------------------------------------------------------------------
bool BoardReaderIHEP::LookUpRawFile() {
  
  // Close the current rawdata file if any
  // and try to open the next rawdata file.
  
//  if( !(fListOfInputFileNames.empty()) ) {
//    
//    if( fCurrentInputFileName != fListOfInputFileNames.end() ) {
//
//      if( fRawFileStream.is_open() ) { // file already open, close and update iterator
//        CloseRawFile();
//        fCurrentInputFileName++;
//      }
//
//      if(fDebugLevel) cout << "  --> BoardReaderIHEP " << fBoardNumber << " Next file to read " << fCurrentInputFileName-fListOfInputFileNames.size() << " over " << fListOfInputFileNames.size() << " closing and opening." << endl;
//      return OpenRawFile( *fCurrentInputFileName );
//      
//    }
//  
//    else { // no more file, end the reading
//      cout << "  --> BoardReaderIHEP " << fBoardNumber << ": all " << fListOfInputFileNames.size() << " files read, closing!" << endl;
////      fNoMoreFile = true;
//      return false;
//    }
//    
//  }
//     
//  else { // no file at all
//    cout << "ERROR: BoardReaderIHEP NO RAW DATA FILE WAS ASSOCIATED WITH BOARD " << fBoardNumber << ", STOPPING!" << endl << endl;
//    return false;
//  }
  return true;

}

// --------------------------------------------------------------------------------------

bool BoardReaderIHEP::HasData( ) {
  
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
  
  bool eventOK   = true;    // we start with a good event by default
//  CurrentEvent   = 0;       // Allow to know wether data are correct
//  ListOfTriggers.clear(); // and clear the vectors
//  ListOfTimestamps.clear();
//  ListOfFrames.clear();
//  ListOfPixels.clear();
//  
//  if(fDebugLevel>0) {
//    cout << endl ;
//    cout << "BoardReaderIHEP::HasData() BoardNumber " << BoardNumber << " IMGBoardReader::HasData() - readingEvent " << ReadingEvent << " currentEvent " << CurrentEventNumber << " currentTrigger " << CurrentTriggerNumber << " currentFrame " <<	CurrentFrameNumber << endl;
//  }
//  
//  if( GetNextEvent() ) {
//    
//    eventOK = SetBufferPointers();
//    if( fDebugLevel>1 ) PrintEventHeader();
//    
//    if( eventOK) {
//      GetTriggerData(); // JB 2012/08/18, called prior GetInputData since 2013/06/20
//      GetInputData();
//      
//      CurrentEvent = new IMGEvent( EventHeader->EvNo, BoardNumber, &ListOfTriggers, &ListOfTimestamps, &ListOfFrames, &ListOfPixels);
//      if( fDebugLevel>1 ) cout << " BoardNumber " << BoardNumber << " create new event " << EventHeader->EvNo << " with " << ListOfPixels.size() << " pixels." << endl;
//      EventsCount++;
//      eventDone = true;
//    }
//    else{
//      cout<<"BoardReaderIHEP::HasData() - Event is not ok!"<<endl;
//    }
//    
//  } // getting next buffer was OK
//  else{
//    cout<<"BoardReaderIHEP::HasData() - Can't get next buffer (# " << BuffersRead <<")!"<<endl;
//  }
//  
//  
//  
//  // End
//  // check there are still data to read
//  return eventOK && !NoMoreFile;
  return eventOK;
}

//------------------------------------------+-----------------------------------
bool BoardReaderIHEP::GetNextEvent()
{
  // Read the necessary frames for all sensors from the raw data files.
  // The data are stored in fData array for further decoding.
  // It means, a first frame is read (or restored if already read)
  //  and then subsequent frames are read and associated if related
  //  to the same trigger.

  // If rawdata are good, decode
//  if (ifs_DataRaw.tellg() != vi_Pointer_FileEnd)
//  {
//    if (ifs_DataRaw.good())
//    {
//      if (ifs_DataRaw.tellg() < vi_Pointer_FileEnd)
//      {
//
//        /* Check file size */
//        /* The minimum case : Ladder_Header/Ladder_FrameCounter/Ladder_DataLength/Ladder_Trailer */
//        vi_Pointer_Data = ifs_DataRaw.tellg();
//        if ((vi_Pointer_Data + 4*DWORD) <= vi_Pointer_FileEnd)
//        {
//          cout << "  ERROR : Mi28DecodeLadderDataToRoot(), file is incomplete at !! Ladder_Header !!, STOP DECODING!" << endl;
//        }
//        // enough info to decode the event
//        else {
//          return DecodeFrame();
//        }
//        
//      } /* End of 'if (ifs_DataRaw.tellg() < vi_Pointer_FileEnd)' */
//    } /* End of 'if (ifs_DataRaw.good())' */
//  } /* End of 'if (!ifs_DataRaw.eof())' */
//  
//  // If here, it means rawdata are no good, try the next file
//  return LookUpRawFile();

  return true;
  
}


// --------------------------------------------------------------------------------------
void BoardReaderIHEP::AddPixel( int iSensor, int value, int aLine, int aColumn)
{
   // Add a pixel to the vector of pixels
   // require the following info
   // - input = number of the sensors (start at 1 !!!!!)
   // - value = analog value of this pixel
   // - line & column = position of the pixel in the matrix
   
//  if (fDebugLevel>2) printf("BoardReaderIHEP::Addpixel adding pixel for sensor %d with value %d line %d row %d\n", iSensor, value, aLine, aColumn);
//
//  ListOfPixels.push_back( BoardReaderPixel( iSensor+1, value, aLine, aColumn, 0) );

}

// --------------------------------------------------------------------------------------
bool BoardReaderIHEP::DecodeFrame()
{
   // Read the information of a frame for a given sensor
   // We use extensively the structure definined by Gille Clauss
   
  if (fDebugLevel) printf("BoardReaderIHEP::DecodeFrame decoding event %d\n", fEventNumber);
  
  bool frameOK = true;
//
//  /* ------------------------------------------------------ */
//  /* Ladder_Header */
//  ifs_DataRaw.read((char *)&vi_Ladder_Header, DWORD);
//  vi_Pointer_Data    = ifs_DataRaw.tellg();
//  vi_Ladder_Header_1 = SwitchDWordBytes2(vi_Ladder_Header);
//  vi_ID_Ladder       = vi_Ladder_Header_1 & 0xFF;
//  vi_Ladder_Header   = vi_Ladder_Header_1 >> 8;
//  if (vi_Ladder_Header != M_LADDER_HEADER)
//  {
//    if (vi_Verbose < 11)
//    {
//      cout << "  ERROR : Mi28DecodeLadderDataToRoot(), find a bad Ladder_Header : "
//      << std::setw(15) << std::setbase(16) << vi_Ladder_Header << "  VS  "
//      << std::setw(11) << std::setbase(16) << M_LADDER_HEADER  << "\t@\t"
//      << std::setw(15) << std::setbase(10) << vi_Pointer_Data  << endl;
//    }
//    
//    /* Pass the first BYTE, and Continue to Check Ladder_Header */
//    ifs_DataRaw.seekg(BYTE - DWORD, ios::cur); /* Return the Pointer */
//    continue; /* Continue to Check Ladder_Header */
//  }
//  else
//  {
//    if (vi_Verbose < 6)
//    {
//      cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Ladder_Header is               : "
//      << std::setw(15) << std::setbase(16) << vi_Ladder_Header_1
//      << std::setw(15) << std::setbase(10) << vi_Ladder_Header_1
//      << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
//    }
//  }
//  
//  /* ------------------------------------------------------ */
//  /* Ladder_FrameCounter */
//  ifs_DataRaw.read((char *)&vi_Ladder_FrameCounter, DWORD);
//  vi_Pointer_Data        = ifs_DataRaw.tellg();
//  vi_Ladder_FrameCounter = SwitchDWordBytes2(vi_Ladder_FrameCounter);
//  if (vi_Verbose < 6)
//  {
//    cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Ladder_FrameCounter is         : "
//    << std::setw(15) << std::setbase(16) << vi_Ladder_FrameCounter
//    << std::setw(15) << std::setbase(10) << vi_Ladder_FrameCounter
//    << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
//  }
//  
//  /* ------------------------------------------------------ */
//  /* Ladder_DataLength */
//  ifs_DataRaw.read((char *)&vi_Ladder_DataLength, DWORD);
//  vi_Pointer_Data      = ifs_DataRaw.tellg();
//  vi_Ladder_DataLength = SwitchDWordBytes2(vi_Ladder_DataLength);
//  vi_Pointer_Ladder_DataLength = vi_Pointer_Data;
//  if (vi_Verbose < 6)
//  {
//    cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Ladder_DataLength is           : "
//    << std::setw(15) << std::setbase(16) << vi_Ladder_DataLength
//    << std::setw(15) << std::setbase(10) << vi_Ladder_DataLength
//    << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
//  }
//  
//  /* Check the file size */
//  if ((vi_Pointer_Data + vi_Ladder_DataLength*WORD + DWORD) > vi_Pointer_FileEnd)
//  {
//    cout << "  ERROR : Mi28DecodeLadderDataToRoot(), file is incomplete at !! Ladder_Trailer !!, STOP DECODING!" << endl;
//    vi_N_Frame_Bad++;
//    break;
//  }
//  
//  /* ------------------------------------------------------ */
//  /* Ladder_Trailer */
//  ifs_DataRaw.seekg(vi_Ladder_DataLength*WORD, ios::cur);
//  ifs_DataRaw.read((char *)&vi_Ladder_Trailer, DWORD);
//  vi_Pointer_Data = ifs_DataRaw.tellg();
//  ifs_DataRaw.seekg(-vi_Ladder_DataLength*WORD - DWORD, ios::cur); /* Return the Pointer */
//  vi_Ladder_Trailer = SwitchDWordBytes2(vi_Ladder_Trailer);
//  if (vi_Ladder_Trailer != M_LADDER_TRAILER)
//  {
//    if (vi_Verbose < 11)
//    {
//      cout << "  ERROR : Mi28DecodeLadderDataToRoot(), find a bad Checking Ladder_Trailer : "
//      << std::setw(15) << std::setbase(16) << vi_Ladder_Trailer << "  VS  "
//      << std::setw(11) << std::setbase(16) << M_LADDER_TRAILER  << "\t@\t"
//      << std::setw(15) << std::setbase(10) << vi_Pointer_Data   << endl;
//    }
//    
//    /* Return to Pointer Ladder_DataLength, Continue a new Ladder_Frame */
//    ifs_DataRaw.seekg(BYTE - 3*DWORD, ios::cur); /* Return the Pointer */
//    continue; /* Continue to Check Ladder_Header */
//  }
//  else
//  {
//    if (vi_Verbose < 6)
//    {
//      cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Checking Ladder_Trailer is     : "
//      << std::setw(15) << std::setbase(16) << vi_Ladder_Trailer
//      << std::setw(15) << std::setbase(10) << vi_Ladder_Trailer
//      << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
//    }
//  }
//  
//  /* ------------------------------------------------------ */
//  /* Recycling decode the data for less than 10 chips */
//  /* ------------------------------------------------------ */
//  vi_N_Ladder_Chip = 0;
//  while (ifs_DataRaw.tellg() < (vi_Pointer_Ladder_DataLength + vi_Ladder_DataLength*WORD))
//  {
//    /* ------------------------------------------------------ */
//    /* Ladder_Chip */
//    ifs_DataRaw.read((char *)&vi_Ladder_Chip, DWORD);
//    vi_Pointer_Data   = ifs_DataRaw.tellg();
//    vi_Ladder_Chip_1  = SwitchDWordBytes2(vi_Ladder_Chip);
//    vi_ID_Ladder_Chip = vi_Ladder_Chip_1 & 0xF;
//    vi_Ladder_Chip    = vi_Ladder_Chip_1 >> 4;
//    if (vi_Ladder_Chip != M_LADDER_CHIP)
//    {
//      if (vi_Verbose < 11)
//      {
//        cout << "  ERROR : Mi28DecodeLadderDataToRoot(), find a bad Ladder_Chip : "
//        << std::setw(15) << std::setbase(16) << vi_Ladder_Chip  << "  VS  "
//        << std::setw(11) << std::setbase(16) << M_LADDER_CHIP   << "\t@\t"
//        << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
//      }
//      
//      /* Return to Pointer Ladder_Chip, Continue a new Ladder_Chip */
//      ifs_DataRaw.seekg(BYTE - DWORD, ios::cur); /* Return the Pointer */
//      continue; /* Continue to Check Ladder_Chip */
//    }
//    else
//    {
//      if (vi_Verbose < 6)
//      {
//        cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Ladder_Chip is                 : "
//        << std::setw(15) << std::setbase(16) << vi_Ladder_Chip_1
//        << std::setw(15) << std::setbase(10) << vi_Ladder_Chip_1
//        << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
//      }
//    }
//    
//    /* ------------------------------------------------------ */
//    /* Chip_Header */
//    ifs_DataRaw.read((char *)&vi_Chip_Header, DWORD);
//    vi_Pointer_Data = ifs_DataRaw.tellg();
//    vi_Chip_Header  = SwitchDWordBytes2(vi_Chip_Header);
//    if (vi_Chip_Header != M_CHIP_HEADER)
//    {
//      if (vi_Verbose < 11)
//      {
//        cout << "  ERROR : Mi28DecodeLadderDataToRoot(), find a bad Chip_Header : "
//        << std::setw(15) << std::setbase(16) << vi_Chip_Header   << "  VS  "
//        << std::setw(11) << std::setbase(16) << M_CHIP_HEADER    << "\t@\t"
//        << std::setw(15) << std::setbase(10) << vi_Pointer_Data  << endl;
//      }
//      
//      /* Return to Pointer Ladder_Chip, Continue a new Ladder_Chip */
//      ifs_DataRaw.seekg(BYTE - 2*DWORD, ios::cur); /* Return the Pointer */
//      continue; /* Continue to Check Ladder_Chip */
//    }
//    else
//    {
//      if (vi_Verbose < 6)
//      {
//        cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Chip_Header is                 : "
//        << std::setw(15) << std::setbase(16) << vi_Chip_Header
//        << std::setw(15) << std::setbase(10) << vi_Chip_Header
//        << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
//      }
//    }
//    
//    /* ------------------------------------------------------ */
//    /* Chip_FrameCounter */
//    ifs_DataRaw.read((char *)&vi_Chip_FrameCounter, DWORD);
//    vi_Pointer_Data      = ifs_DataRaw.tellg();
//    vi_Chip_FrameCounter = SwitchDWordBytes2(vi_Chip_FrameCounter);
//    if (vi_Verbose < 6)
//    {
//      cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Chip_FrameCounter is           : "
//      << std::setw(15) << std::setbase(16) << vi_Chip_FrameCounter
//      << std::setw(15) << std::setbase(10) << vi_Chip_FrameCounter
//      << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
//    }
//    
//    /* ------------------------------------------------------ */
//    /* Chip_DataLength */
//    ifs_DataRaw.read((char *)&vi_Chip_DataLength_1, WORD);
//    ifs_DataRaw.read((char *)&vi_Chip_DataLength_2, WORD);
//    vi_Chip_DataLength_1       = SwitchWordBytes(vi_Chip_DataLength_1);
//    vi_Chip_DataLength_2       = SwitchWordBytes(vi_Chip_DataLength_2);
//    vi_Chip_DataLength         = vi_Chip_DataLength_1 + vi_Chip_DataLength_2;
//    vi_Pointer_Data = ifs_DataRaw.tellg();
//    vi_Pointer_Chip_DataLength = vi_Pointer_Data;
//    if (vi_Verbose < 6)
//    {
//      cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Chip_DataLength is             : "
//      << std::setw(15) << std::setbase(16) << vi_Chip_DataLength
//      << std::setw(15) << std::setbase(10) << vi_Chip_DataLength
//      << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
//    }
//    
//    /* Check the the file size */
//    if ((vi_Chip_DataLength*WORD + DWORD) > vi_Ladder_DataLength*WORD)
//    {
//      cout << "  ERROR : Mi28DecodeLadderDataToRoot(), file frame is incomplete at !! Chip_Trailer !!, STOP DECODING!" << endl;
//      vi_N_Frame_Bad++;
//      break;
//    }
//    
//    /* ------------------------------------------------------ */
//    /* Chip_Trailer */
//    ifs_DataRaw.seekg(vi_Chip_DataLength*WORD, ios::cur);
//    ifs_DataRaw.read((char *)&vi_Chip_Trailer, DWORD);
//    vi_Pointer_Data = ifs_DataRaw.tellg();
//    ifs_DataRaw.seekg(-vi_Chip_DataLength*WORD - DWORD, ios::cur); /* Return the Pointer */
//    vi_Chip_Trailer = SwitchDWordBytes2(vi_Chip_Trailer);
//    if (vi_Chip_Trailer != M_CHIP_TRAILER)
//    {
//      if (vi_Verbose < 11)
//      {
//        cout << "  ERROR : Mi28DecodeLadderDataToRoot(), find a bad Checking Chip_Trailer : "
//        << std::setw(15) << std::setbase(16) << vi_Chip_Trailer << "  VS  "
//        << std::setw(11) << std::setbase(16) << M_CHIP_TRAILER  << "\t@\t"
//        << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
//      }
//      
//      /* Return to Pointer Ladder_Chip, Continue a new Ladder_Chip */
//      ifs_DataRaw.seekg(BYTE - 4*DWORD, ios::cur); /* Return the Pointer */
//      continue; /* Continue to Check Ladder_Chip */
//    }
//    else
//    {
//      if (vi_Verbose < 6)
//      {
//        cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Checking Chip_Trailer is       : "
//        << std::setw(15) << std::setbase(16) << vi_Chip_Trailer
//        << std::setw(15) << std::setbase(10) << vi_Chip_Trailer
//        << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
//      }
//    }
//    
//    /* ------------------------------------------------------ */
//    /*                   Deal with useful data                */
//    /* ------------------------------------------------------ */
//    while (ifs_DataRaw.tellg() < (vi_Pointer_Chip_DataLength + vi_Chip_DataLength*WORD))
//    {
//      /* Chip_Status */
//      ifs_DataRaw.read((char *)&vi_DataRaw, WORD);
//      vi_Pointer_Data = ifs_DataRaw.tellg();
//      
//      vi_Chip_Status          = SwitchWordBytes(vi_DataRaw);
//      vi_Chip_N_State         = vi_Chip_Status  & M_CHIP_N_STATE;
//      vi_Chip_Address_Line    = (vi_Chip_Status & M_CHIP_ADDRESS_LINE)    >> 4;
//      vi_Chip_Flag_Residual_1 = (vi_Chip_Status & M_CHIP_FLAG_RESIDUAL_1) >> 14;
//      vi_Chip_Flag_OverFlow   = (vi_Chip_Status & M_CHIP_FLAG_OVERFLOW)   >> 15;
//      
//      if (vi_Verbose < 3)
//      {
//        vi_Pointer_Data = ifs_DataRaw.tellp();
//        cout << "  SSSSS" << endl;
//        cout << "  INFO : Status / Pointer_Status / N_State / Address_Line   is : "
//        << std::setw(12) << std::setbase(16) << vi_Chip_Status
//        << std::setw(12) << std::setbase(10) << vi_Pointer_Data
//        << std::setw(12) << std::setbase(10) << vi_Chip_N_State
//        << std::setw(12) << std::setbase(10) << vi_Chip_Address_Line   << endl;
//      }
//      
//      /* Check the status word */
//      if ((vi_Chip_Address_Line>N_ROW) || (vi_Chip_Address_Line<0))
//      {
//        if (vi_Verbose < 11)
//        {
//          cout << "  ERROR : Mi28DecodeLadderDataToRoot(), Line counter is wrong, which is "
//          << std::setbase(10) << vi_Chip_Address_Line << " VS "
//          << std::setbase(10) << N_ROW << endl;
//        }
//        vi_N_Frame_Bad++;
//        break;
//      }
//      if (vi_Chip_Flag_Residual_1 != 0)
//      {
//        if (vi_Verbose < 4)
//        {
//          cout << "  Warning : Mi28DecodeLadderDataToRoot(), Status is wrong! The status residual bit should be 0, not should be "
//          << std::setbase(10) << vi_Chip_Flag_Residual_1 << endl;
//        }
//      }
//      if (vi_Chip_Flag_OverFlow == 1)
//      {
//        if (vi_Verbose < 4)
//        {
//          cout << "  Warning : Mi28DecodeLadderDataToRoot(), The status is overflow, the bit is "
//          << std::setbase(10) << vi_Chip_Flag_OverFlow << " @ line "
//          << std::setbase(10) << vi_Chip_Address_Line  << endl;
//        }
//      }
//      
//      /* Skip the last data word, if not, will lead to read more word for this frame */
//      //if ((vi_Pointer_Data + vi_Chip_N_State) >= (vi_Pointer_Chip_DataLength + vi_Chip_DataLength*WORD))
//      if (vi_Pointer_Data == (vi_Pointer_Chip_DataLength + vi_Chip_DataLength*WORD))
//      {
//        if (vi_Verbose < 11)
//        {
//          cout << "  Warning : Mi28DecodeLadderDataToRoot(), The useful word exceeds the data length, skip this word which is @ "
//          << std::setbase(10) << vi_Pointer_Data    << " + "
//          << std::setbase(10) << vi_Chip_N_State    << " VS "
//          << std::setbase(10) << (vi_Pointer_Chip_DataLength + vi_Chip_DataLength*WORD)
//          << endl;
//        }
//        vi_N_Frame_Bad++;
//        break;
//      }
//      
//      for (unsigned long int iState=0; iState<vi_Chip_N_State; iState++)
//      {
//        /* Chip_State */
//        ifs_DataRaw.read((char *)&vi_DataRaw, WORD);
//        vi_Pointer_Data = ifs_DataRaw.tellp();
//        
//        vi_Chip_State           = SwitchWordBytes(vi_DataRaw);
//        vi_Chip_N_Pixel         = vi_Chip_State  & M_CHIP_N_PIXEL;
//        vi_Chip_Address_Column  = (vi_Chip_State & M_CHIP_ADDRESS_COLUMN)  >> 2;
//        vi_Chip_Flag_Residual_2 = (vi_Chip_State & M_CHIP_FLAG_RESIDUAL_2) >> 12;
//        
//        if (vi_Verbose < 3)
//        {
//          cout << "  INFO : State  / Pointer_State  / N_Pixel / Address_Column is : "
//          << std::setw(12) << std::setbase(16) << vi_Chip_State
//          << std::setw(12) << std::setbase(10) << vi_Pointer_Data
//          << std::setw(12) << std::setbase(10) << vi_Chip_N_Pixel
//          << std::setw(12) << std::setbase(10) << vi_Chip_Address_Column << endl;
//        }
//        
//        /* Check the state word */
//        if ((vi_Chip_Address_Column>N_COLUMN) || (vi_Chip_Address_Column<0))
//        {
//          if (vi_Verbose < 11)
//          {
//            cout << "  ERROR : Mi28DecodeLadderDataToRoot(), Column counter is wrong, which is "
//            << std::setbase(10) << vi_Chip_Address_Column << " VS "
//            << std::setbase(10) << N_COLUMN << endl;
//          }
//          vi_N_Frame_Bad++;
//          break;
//        }
//        if (vi_Chip_Flag_Residual_2 != 0)
//        {
//          if (vi_Verbose < 4)
//          {
//            cout << "  Warning : Mi28DecodeLadderDataToRoot(), state is wrong! The state residual bits should be 0, not should be "
//            << std::setbase(10) << vi_Chip_Flag_Residual_2 << endl;
//          }
//        }
//        
//        /* +1 means that vi_Chip_N_Pixel is the number of pixels after the first column */
//        for (unsigned long int iPixel=0; iPixel<vi_Chip_N_Pixel+1; iPixel++)
//        {
//          vi_Column_Temp = vi_Chip_Address_Column+iPixel;
//          
//          vi_Tree_N_Ladder_Frame         = vi_N_Frame_Good + 1;
//          vi_Tree_N_Ladder_Chip          = 2;
//          vi_Tree_ID_Ladder              = vi_ID_Ladder;
//          vi_Tree_ID_Ladder_FrameCounter = vi_Ladder_FrameCounter;
//          vi_Tree_ID_Ladder_Chip         = vi_ID_Ladder_Chip;
//          vi_Tree_ID_Chip_FrameCounter   = vi_Chip_FrameCounter;
//          vi_Tree_ID_Chip_Column         = vi_Column_Temp;
//          vi_Tree_ID_Chip_Row            = vi_Chip_Address_Line;
//          
//          Tree_FiredPixel->Fill();
//          
//          vd_N_PixelTotal++;
//          
//          if ((vi_Column_Temp >= N_BANKCOLUMN*0) && (vi_Column_Temp < N_BANKCOLUMN*1))
//          {
//            vd_N_PixelBankA++;
//          }
//          if ((vi_Column_Temp >= N_BANKCOLUMN*1) && (vi_Column_Temp < N_BANKCOLUMN*2))
//          {
//            vd_N_PixelBankB++;
//          }
//          if ((vi_Column_Temp >= N_BANKCOLUMN*2) && (vi_Column_Temp < N_BANKCOLUMN*3))
//          {
//            vd_N_PixelBankC++;
//          }
//          if ((vi_Column_Temp >= N_BANKCOLUMN*3) && (vi_Column_Temp < N_BANKCOLUMN*4))
//          {
//            vd_N_PixelBankD++;
//          }
//          
//          if (fDebugLevel>2) printf("BoardReaderIHEP::Addpixel adding pixel for sensor %d with value %d line %d row %d\n", iSensor, value, aLine, aColumn);
//          
//          ListOfPixels.push_back( BoardReaderPixel( iSensor+1, value, aLine, aColumn, 0) );
//
//          
//        }
//        
//      } /* End of 'for (int iState=0; iState<vi_Chip_N_State; iState++)' */
//    } /* End of 'while ((ifs_DataRaw.tellg() - vi_Pointer_Chip_DataLength) < vi_Chip_DataLength)' */
//    
//    /* ------------------------------------------------------ */
//    /* Chip_Trailer */
//    ifs_DataRaw.read((char *)&vi_Chip_Trailer, DWORD);
//    vi_Pointer_Data = ifs_DataRaw.tellg();
//    vi_Chip_Trailer = SwitchDWordBytes2(vi_Chip_Trailer);
//    if (vi_Chip_Trailer != M_CHIP_TRAILER)
//    {
//      if (vi_Verbose < 11)
//      {
//        cout << "  ERROR : Mi28DecodeLadderDataToRoot(), find a bad Scanning Chip_Trailer  : "
//        << std::setw(15) << std::setbase(16) << vi_Chip_Trailer << "  VS  "
//        << std::setw(11) << std::setbase(16) << M_CHIP_TRAILER  << "\t@\t"
//        << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
//      }
//      break;
//    }
//    else
//    {
//      if (vi_Verbose < 6)
//      {
//        cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Scanning Chip_Trailer is       : "
//        << std::setw(15) << std::setbase(16) << vi_Chip_Trailer
//        << std::setw(15) << std::setbase(10) << vi_Chip_Trailer
//        << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
//      }
//    }
//    vi_N_Ladder_Chip++;
//  } /* End of 'while ((ifs_DataRaw.tellg() - vi_Pointer_Ladder_DataLength) < vi_Ladder_DataLength)' */
//  
//  /* ------------------------------------------------------ */
//  /* Ladder_Trailer */
//  ifs_DataRaw.read((char *)&vi_Ladder_Trailer, DWORD);
//  vi_Pointer_Data = ifs_DataRaw.tellg();
//  vi_Ladder_Trailer = SwitchDWordBytes2(vi_Ladder_Trailer);
//  if (vi_Ladder_Trailer != M_LADDER_TRAILER)
//  {
//    if (vi_Verbose < 11)
//    {
//      cout << "  ERROR : Mi28DecodeLadderDataToRoot(), find a bad Scannin Ladder_Trailer : "
//      << std::setw(15) << std::setbase(16) << vi_Ladder_Trailer << "  VS  "
//      << std::setw(11) << std::setbase(16) << M_LADDER_TRAILER  << "\t@\t"
//      << std::setw(15) << std::setbase(10) << vi_Pointer_Data   << endl;
//    }
//    break;
//  }
//  else
//  {
//    if (vi_Verbose < 6)
//    {
//      cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Scanning Ladder_Trailer is     : "
//      << std::setw(15) << std::setbase(16) << vi_Ladder_Trailer
//      << std::setw(15) << std::setbase(10) << vi_Ladder_Trailer
//      << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
//    }
//  }
//  vi_N_Frame_Good++;
//  if (vi_Verbose < 11)
//  {
//    cout << "  SUCCESS : Mi28DecodeLadderDataToRoot(), Finish Good Ladder Frame " << vi_N_Frame_Good << " with Chips = " << vi_N_Ladder_Chip
//    << " @ "  << std::setw(15) << std::setbase(10) << ifs_DataRaw.tellg()
//    << " VS " << std::setw(15) << std::setbase(10) << vi_Pointer_FileEnd \
//    << endl;
//  }
  
  return frameOK;
}

// --------------------------------------------------------------------------------------

void BoardReaderIHEP::PrintStatistics(ostream &stream) {
  
  // Print statistics on the events read by this board
  //
  // JB, 2014/05/13
  
//  stream << "***********************************************" << endl;
//  stream << " Board IHEP " << fBoardNumber << " found:" << endl;
//  stream << fCurrentTriggerCnt << " triggers." << endl;
//  stream << fEventNumber << " events in total," << endl;
//  stream << fEventsOverflow << " events with an overflow (veto overflow " << fVetoOverflow << ")." << endl;
//  stream << fFramesReadFromFile<< " frames read overall." << endl;
//  stream << "***********************************************" << endl;
//  
//  double vd_FakeRateTotal = vd_N_PixelTotal / N_COLUMN  / N_ROW / vi_N_Frame_Good;
//  double vd_FakeRateBankA = vd_N_PixelBankA / N_BANKCOLUMN / N_ROW / vi_N_Frame_Good;
//  double vd_FakeRateBankB = vd_N_PixelBankB / N_BANKCOLUMN / N_ROW / vi_N_Frame_Good;
//  double vd_FakeRateBankC = vd_N_PixelBankC / N_BANKCOLUMN / N_ROW / vi_N_Frame_Good;
//  double vd_FakeRateBankD = vd_N_PixelBankD / N_BANKCOLUMN / N_ROW / vi_N_Frame_Good;
//  
//  cout << "------------------------------------------------------"   << endl;
//  cout << resetiosflags(ios::scientific);
//  cout << "INFO : Mi28DecodeLadderDataToRoot(), total good frames are  " << setw(9) << vi_N_Frame_Good << endl;
//  cout << "INFO : Mi28DecodeLadderDataToRoot(), total bad  frames are  " << setw(9) << vi_N_Frame_Bad  << endl;
//  cout << endl;
//  cout << "------------------------------------------------------" << endl;
//  cout << "INFO : Mi28DecodeLadderDataToRoot(), total hit  pixels are  " << setw(9) << right << vd_N_PixelTotal / vi_N_Frame_Good << "\tvs\t" << N_COLUMN *N_ROW << endl;
//  cout << endl;
//  cout << "INFO : Mi28DecodeLadderDataToRoot(), BankA hit  pixels are  " << setw(9) << right << vd_N_PixelBankA / vi_N_Frame_Good << "\tvs\t" << N_BANKCOLUMN*N_ROW << endl;
//  cout << "INFO : Mi28DecodeLadderDataToRoot(), BankB hit  pixels are  " << setw(9) << right << vd_N_PixelBankB / vi_N_Frame_Good << "\tvs\t" << N_BANKCOLUMN*N_ROW << endl;
//  cout << "INFO : Mi28DecodeLadderDataToRoot(), BankC hit  pixels are  " << setw(9) << right << vd_N_PixelBankC / vi_N_Frame_Good << "\tvs\t" << N_BANKCOLUMN*N_ROW << endl;
//  cout << "INFO : Mi28DecodeLadderDataToRoot(), BankD hit  pixels are  " << setw(9) << right << vd_N_PixelBankD / vi_N_Frame_Good << "\tvs\t" << N_BANKCOLUMN*N_ROW << endl;
//  cout << "------------------------------------------------------" << endl;
//  cout << "INFO : Mi28DecodeLadderDataToRoot(), total hit fake rate is " << setw(9) << setprecision(6) << scientific << vd_FakeRateTotal << endl;
//  cout << endl;
//  cout << "INFO : Mi28DecodeLadderDataToRoot(), BankA hit fake rate is " << setw(9) << setprecision(6) << scientific << vd_FakeRateBankA << endl;
//  cout << "INFO : Mi28DecodeLadderDataToRoot(), BankB hit fake rate is " << setw(9) << setprecision(6) << scientific << vd_FakeRateBankB << endl;
//  cout << "INFO : Mi28DecodeLadderDataToRoot(), BankC hit fake rate is " << setw(9) << setprecision(6) << scientific << vd_FakeRateBankC << endl;
//  cout << "INFO : Mi28DecodeLadderDataToRoot(), BankD hit fake rate is " << setw(9) << setprecision(6) << scientific << vd_FakeRateBankD << endl;
//  cout << resetiosflags(ios::scientific);

}


