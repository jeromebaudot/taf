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
BoardReaderIHEP::BoardReaderIHEP(int boardNumber, int triggerMode, int eventBuildingMode) {

  vi_Verbose = 12;
  vi_Verbose = 0;

  N_COLUMN     = 960;
  N_ROW        = 928;
  N_BANK       = 4;
  N_BANKCOLUMN = N_COLUMN / N_BANK;

  BYTE  = 1; // 1Byte  = 8bits
  WORD  = 2; // 1Word  = 2Byte = 16bits (Mi28 default)
  DWORD = 4; // 1DWord = 2Word = 32bits

  M_LADDER_HEADER  = 0xCCCCCC   ; // Ladder Header  Marker 32 bits : 0xCCCCCCXX, Check the Big 24 bits
  M_LADDER_CHIP    = 0x3333333  ; // Ladder Trailer Marker 32 bits : 0x3333333X, Check the Big 28 bits
  M_LADDER_TRAILER = 0x99999999 ; // Ladder Trailer Marker 32 bits : 0xCCCCCCCC, Check all the 32 bits

  vi_Ladder_Header       = 0;
  vi_Ladder_Header_1     = 0;
  vi_Ladder_DataLength   = 0;
  vi_Ladder_Trigger      = 0;
  vi_Ladder_FrameCounter = 0;
  vi_Ladder_Chip         = 0;
  vi_Ladder_Chip_1       = 0;
  vi_Ladder_Trailer      = 0;
  vi_ID_Ladder           = 0;
  vi_ID_Ladder_Chip      = 0;
  vi_N_Ladder_Chip       = 0;

  M_CHIP_HEADER  = 0x12345678; // hex=1234 5678
  M_CHIP_TRAILER = 0xaaaaaaaa; // hex=aaaa aaaa

  /* Signification of the bits in the Status/Line word */
  M_CHIP_N_STATE         = 15    ; // Status/Line, 0-3, number of States
  M_CHIP_ADDRESS_LINE    = 16368 ; // Status/Line, 4-13, address of the line
  M_CHIP_FLAG_RESIDUAL_1 = 16384 ; // Status/Line, 14, 1 bit zero
  M_CHIP_FLAG_OVERFLOW   = 32768 ; // Status/Line, 15, overflow

  /* Signification of the bits in the State word */
  M_CHIP_N_PIXEL         = 3     ; // State, 0-1, number of Pixels
  M_CHIP_ADDRESS_COLUMN  = 4092  ; // State, 2-11, address of the column
  M_CHIP_FLAG_RESIDUAL_2 = 61440 ; // State, 12-15, 4 bits zero

  /* Values for a frame */
  vi_DataRaw              = 0;
  vi_Chip_Header          = 0;
  vi_Chip_FrameCounter    = 0;
  vi_Chip_DataLength      = 0;
  vi_Chip_DataLength_1    = 0;
  vi_Chip_DataLength_2    = 0;
  vi_Chip_Status          = 0;
  vi_Chip_N_State         = 0;
  vi_Chip_Address_Line    = 0;
  vi_Chip_Flag_Residual_1 = 0;
  vi_Chip_Flag_OverFlow   = 0;
  vi_Chip_State           = 0;
  vi_Chip_N_Pixel         = 0;
  vi_Chip_Address_Column  = 0;
  vi_Chip_Flag_Residual_2 = 0;
  vi_Chip_Trailer         = 0;
  vi_Column_Temp          = 0;

  /* Pointers */
  vi_Pointer_Data              = 0;
  vi_Pointer_Ladder_DataLength = 0;
  vi_Pointer_Chip_DataLength   = 0;

  vi_N_Frame_Good = 0;
  vi_N_Frame_Bad  = 0;

  vd_N_PixelTotal = 0;
  vd_N_PixelBankA = 0;
  vd_N_PixelBankB = 0;
  vd_N_PixelBankC = 0;
  vd_N_PixelBankD = 0;

  fBoardNumber = boardNumber;

  cout << "*****************************************" << endl;
  cout << "    < BoardReaderIHEP constructor >      " << endl;
  cout << "*****************************************" << endl;
  cout << "Creating a BoardReaderIHEP" << endl;
  cout << " * for board : " << fBoardNumber << endl;
  cout << " * for sensor type : MIMOSA-28" << endl;

  cout << " INFO BoardReaderIHEP the Data format  is                : "
  << std::setw(15) << "hex"
  << std::setw(15) << "dec"
  << std::setw(15) << "Pointer" << endl;

  fEventNumber = 0;
  fCurrentEvent = NULL;
  fCurrentTriggerNumber = 0;

}

//------------------------------------------+-----------------------------------
BoardReaderIHEP::~BoardReaderIHEP()
{

//  delete fCurrentEvent;
//  delete fInputFileName;

}

// --------------------------------------------------------------------------------------

bool BoardReaderIHEP::AddFile(const char *fileName) {
  // Adapted from method sent on 2018/08/14

  /* Open the input Raw Data file, Data getting from FPGA, default txt in 18 32bit-words */
  ifs_DataRaw.close();
  ifs_DataRaw.clear();
  ifs_DataRaw.open( fileName, ios::in|ios::binary);
  if (!ifs_DataRaw.good())
  {
    cout << "ERROR : Mi28DecodeLadderDataToRoot(), Failed to open input file: " << fileName << endl;
    return false;
  }
  else
  {
    cout << "INFO : BoardReaderIHEP::AddFile, Successfully open input file: " << fileName << endl;
  }
  /* Get the size of the binary file */
  ifs_DataRaw.seekg(0, ios::beg);
  vi_Pointer_FileBegin = ifs_DataRaw.tellg();
  ifs_DataRaw.seekg(0, ios::end);
  vi_Pointer_FileEnd = ifs_DataRaw.tellg();
  vi_N_FileSize = vi_Pointer_FileEnd - vi_Pointer_FileBegin;
  ifs_DataRaw.seekg(0, ios::beg);
  cout << "INFO : Mi28DecodeLadderDataToRoot(), The input file size is : " << vi_N_FileSize << endl;
  cout << "------------------------------------------------------"   << endl;

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
  // JB, 2018/07/16

  // -+-+- Initialization

  bool eventOK   = true;    // we start with a good event by default
  fCurrentEvent   = 0;       // Allow to know wether data are correct
  ListOfTriggers.clear(); // and clear the vectors
  ListOfTimestamps.clear();
  ListOfFrames.clear();
  ListOfPixels.clear();

 if(vi_Verbose>0) {
   cout << endl ;
   cout << "BoardReaderIHEP::HasData() BoardNumber " << fBoardNumber << " IMGBoardReader::HasData() - currentEvent " << fEventNumber << " currentTrigger " << fCurrentTriggerNumber << endl;
 }

 if( GetNextEvent() ) {

   if( vi_Verbose>1 ) PrintEventHeader();

   if( DecodeFrame() ) {
     fCurrentEvent = new BoardReaderEvent( fEventNumber, fBoardNumber, 0, &ListOfPixels); // run number set to 0 for now
     fCurrentEvent->SetListOfTriggers( &ListOfTriggers);
     fCurrentEvent->SetListOfFrames( &ListOfFrames);
     if( vi_Verbose>1 ) cout << " BoardNumber " << fBoardNumber << " create new event " << fEventNumber << " with " << ListOfPixels.size() << " pixels" << " from " << ListOfTriggers.size() << " and " << ListOfFrames.size() << "frames." << endl;
     fEventNumber++;
   } // decoding frame was OK
   else {
     eventOK = false;
     cout<<"BoardReaderIHEP::HasData() - Can't decode frame !"<<endl;
   }

 } // getting next buffer was OK
 else{
   eventOK = false;
   cout<<"BoardReaderIHEP::HasData() - Can't get next frame !"<<endl;
 }

 // End
 // check there are still data to read
 return eventOK;
}

//------------------------------------------+-----------------------------------
bool BoardReaderIHEP::GetNextEvent()
{
  //

  // If rawdata are good, decode
  bool ready = false;

  while (ifs_DataRaw.tellg() != vi_Pointer_FileEnd && ready != true)
  {
  /* Input raw binary data */
    if (ifs_DataRaw.good())
    {
      /* Check file size */
      /* The minimum case : Ladder_Header/Ladder_FrameCounter/Ladder_DataLength/Ladder_Trailer */
      vi_Pointer_Data = ifs_DataRaw.tellg();
      if ((vi_Pointer_Data + 4*DWORD) > vi_Pointer_FileEnd)
      {
        cout << "  ERROR : Mi28DecodeLadderDataToRoot(), file is incomplete at !! Ladder_Header !!, STOP DECODING!" << endl;
        break;
      }

      /* ------------------------------------------------------ */
      /* Ladder_Header */
      ifs_DataRaw.read((char *)&vi_Ladder_Header, DWORD);
      vi_Pointer_Data    = ifs_DataRaw.tellg();
      vi_Ladder_Header_1 = SwitchDWordWords(vi_Ladder_Header);
      vi_ID_Ladder       = vi_Ladder_Header_1 & 0xFF;
      vi_Ladder_Header   = vi_Ladder_Header_1 >> 8;
      if (vi_Ladder_Header != M_LADDER_HEADER)
      {
        if (vi_Verbose < 11)
        {
          cout << "  ERROR : Mi28DecodeLadderDataToRoot(), find a bad Ladder_Header : "
               << std::setw(15) << std::setbase(16) << vi_Ladder_Header << "  VS  "
               << std::setw(11) << std::setbase(16) << M_LADDER_HEADER  << "\t@\t"
               << std::setw(15) << std::setbase(10) << vi_Pointer_Data  << endl;
        }

        /* Pass the first BYTE, and Continue to Check Ladder_Header */
        ifs_DataRaw.seekg(BYTE - DWORD, ios::cur); /* Return the Pointer */
        continue; /* Continue to Check Ladder_Header */
      }
      else
      {
        if (vi_Verbose < 6)
        {
          cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Ladder_Header is               : "
               << std::setw(15) << std::setbase(16) << vi_Ladder_Header_1
               << std::setw(15) << std::setbase(10) << vi_Ladder_Header_1
               << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
        }
      }

      /* ------------------------------------------------------ */

       /* ------------------------------------------------------ */
      /* Ladder_Trigger */
      ifs_DataRaw.read((char *)&vi_Ladder_Trigger, DWORD);
      vi_Pointer_Data        = ifs_DataRaw.tellg();
      vi_Ladder_Trigger = SwitchDWordWords(vi_Ladder_Trigger);
      if (vi_Verbose < 6)
      {
        cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Ladder_Trigger is         : "
             << std::setw(15) << std::setbase(16) << vi_Ladder_Trigger
             << std::setw(15) << std::setbase(10) << vi_Ladder_Trigger
             << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
      }

      /* Ladder_FrameCounter */
      ifs_DataRaw.read((char *)&vi_Ladder_FrameCounter, DWORD);
      vi_Pointer_Data        = ifs_DataRaw.tellg();
      vi_Ladder_FrameCounter = SwitchDWordWords(vi_Ladder_FrameCounter);
      if (vi_Verbose < 6)
      {
        cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Ladder_FrameCounter is         : "
             << std::setw(15) << std::setbase(16) << vi_Ladder_FrameCounter
             << std::setw(15) << std::setbase(10) << vi_Ladder_FrameCounter
             << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
      }

      /* ------------------------------------------------------ */
      /* Ladder_DataLength */
      ifs_DataRaw.read((char *)&vi_Ladder_DataLength, DWORD);
      vi_Pointer_Data      = ifs_DataRaw.tellg();
      //vi_Ladder_DataLength = SwitchDWordWords(vi_Ladder_DataLength);
      vi_Pointer_Ladder_DataLength = vi_Pointer_Data;
      if (vi_Verbose < 6)
      {
        cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Ladder_DataLength is           : "
             << std::setw(15) << std::setbase(16) << vi_Ladder_DataLength
             << std::setw(15) << std::setbase(10) << vi_Ladder_DataLength
             << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
      }

      /* Check the the file size */
      if ((vi_Pointer_Data + vi_Ladder_DataLength*WORD + DWORD) > vi_Pointer_FileEnd)
      {
        cout << "  ERROR : Mi28DecodeLadderDataToRoot(), file is incomplete at !! Ladder_Trailer !!, STOP DECODING!" << endl;
        vi_N_Frame_Bad++;
        break;
      }

      /* ------------------------------------------------------ */
      /* Ladder_Trailer */
      ifs_DataRaw.seekg(vi_Ladder_DataLength*WORD, ios::cur);
      ifs_DataRaw.read((char *)&vi_Ladder_Trailer, DWORD);
      vi_Pointer_Data = ifs_DataRaw.tellg();
      ifs_DataRaw.seekg(-vi_Ladder_DataLength*WORD - DWORD, ios::cur); /* Return the Pointer */
      vi_Ladder_Trailer = SwitchDWordWords(vi_Ladder_Trailer);
      if (vi_Ladder_Trailer != M_LADDER_TRAILER)
      {
        if (vi_Verbose < 11)
        {
          cout << "  ERROR : Mi28DecodeLadderDataToRoot(), find a bad Checking Ladder_Trailer : "
               << std::setw(15) << std::setbase(16) << vi_Ladder_Trailer << "  VS  "
               << std::setw(11) << std::setbase(16) << M_LADDER_TRAILER  << "\t@\t"
               << std::setw(15) << std::setbase(10) << vi_Pointer_Data   << endl;
        }

        /* Return to Pointer Ladder_DataLength, Continue a new Ladder_Frame */
        ifs_DataRaw.seekg(BYTE - 3*DWORD, ios::cur); /* Return the Pointer */
        continue; /* Continue to Check Ladder_Header */
      }
      else
      {
        ready = true;
        if (vi_Verbose < 6)
        {
          cout << " BoardReaderIHEP::GetNextEvent ready to decode frame" << endl;
          cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Checking Ladder_Trailer is     : "
               << std::setw(15) << std::setbase(16) << vi_Ladder_Trailer
               << std::setw(15) << std::setbase(10) << vi_Ladder_Trailer
               << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
        }
      }

    } // end of if ifs_DataRaw.good()

  } // end of while ifs_DataRaw.tellg() != vi_Pointer_FileEnd

  return ready;

}


// --------------------------------------------------------------------------------------
void BoardReaderIHEP::AddPixel( int iSensor, int value, int aLine, int aColumn)
{
   // Add a pixel to the vector of pixels
   // require the following info
   // - input = number of the sensors (start at 1 !!!!!)
   // - value = analog value of this pixel
   // - line & column = position of the pixel in the matrix

  if (vi_Verbose>2) printf("BoardReaderIHEP::Addpixel adding pixel for sensor %d with value %d line %d row %d\n", iSensor, value, aLine, aColumn);

  ListOfPixels.push_back( BoardReaderPixel( iSensor+1, value, aLine, aColumn, 0) );

}

// --------------------------------------------------------------------------------------
bool BoardReaderIHEP::DecodeFrame()
{

  if (vi_Verbose) printf("BoardReaderIHEP::DecodeFrame decoding event %d\n", fEventNumber);

  bool frameOK = false;

  /* ------------------------------------------------------ */
  /* Recycling decode the data for less than 10 chips */
  /* ------------------------------------------------------ */
  vi_N_Ladder_Chip = 0;
  while (ifs_DataRaw.tellg() < (vi_Pointer_Ladder_DataLength + vi_Ladder_DataLength*WORD) && frameOK!=true)
  {
    /* ------------------------------------------------------ */
    /* Ladder_Chip */
    ifs_DataRaw.read((char *)&vi_Ladder_Chip, DWORD);
    vi_Pointer_Data   = ifs_DataRaw.tellg();
    vi_Ladder_Chip_1  = SwitchDWordWords(vi_Ladder_Chip);
    vi_ID_Ladder_Chip = vi_Ladder_Chip_1 & 0xF;
    vi_Ladder_Chip    = vi_Ladder_Chip_1 >> 4;
    if (vi_Ladder_Chip != M_LADDER_CHIP)
    {
      if (vi_Verbose < 11)
      {
        cout << "  ERROR : Mi28DecodeLadderDataToRoot(), find a bad Ladder_Chip : "
        << std::setw(15) << std::setbase(16) << vi_Ladder_Chip  << "  VS  "
        << std::setw(11) << std::setbase(16) << M_LADDER_CHIP   << "\t@\t"
        << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
      }

      /* Return to Pointer Ladder_Chip, Continue a new Ladder_Chip */
      ifs_DataRaw.seekg(BYTE - DWORD, ios::cur); /* Return the Pointer */
      continue; /* Continue to Check Ladder_Chip */
    }
    else
    {
      if (vi_Verbose < 6)
      {
        cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Ladder_Chip is                 : "
        << std::setw(15) << std::setbase(16) << vi_Ladder_Chip_1
        << std::setw(15) << std::setbase(10) << vi_Ladder_Chip_1
        << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
      }
    }

    /* ------------------------------------------------------ */
    /* Chip_Header */
    ifs_DataRaw.read((char *)&vi_Chip_Header, DWORD);
    vi_Pointer_Data = ifs_DataRaw.tellg();
    vi_Chip_Header  = SwitchDWordWords(vi_Chip_Header);
    if (vi_Chip_Header != M_CHIP_HEADER)
    {
      if (vi_Verbose < 11)
      {
        cout << "  ERROR : Mi28DecodeLadderDataToRoot(), find a bad Chip_Header : "
        << std::setw(15) << std::setbase(16) << vi_Chip_Header   << "  VS  "
        << std::setw(11) << std::setbase(16) << M_CHIP_HEADER    << "\t@\t"
        << std::setw(15) << std::setbase(10) << vi_Pointer_Data  << endl;
      }

      /* Return to Pointer Ladder_Chip, Continue a new Ladder_Chip */
      ifs_DataRaw.seekg(BYTE - 2*DWORD, ios::cur); /* Return the Pointer */
      continue; /* Continue to Check Ladder_Chip */
    }
    else
    {
      if (vi_Verbose < 6)
      {
        cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Chip_Header is                 : "
        << std::setw(15) << std::setbase(16) << vi_Chip_Header
        << std::setw(15) << std::setbase(10) << vi_Chip_Header
        << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
      }
    }

    /* ------------------------------------------------------ */
    /* Chip_FrameCounter */
    ifs_DataRaw.read((char *)&vi_Chip_FrameCounter, DWORD);
    vi_Pointer_Data      = ifs_DataRaw.tellg();
    vi_Chip_FrameCounter = SwitchDWordWords(vi_Chip_FrameCounter);
    if (vi_Verbose < 6)
    {
      cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Chip_FrameCounter is           : "
      << std::setw(15) << std::setbase(16) << vi_Chip_FrameCounter
      << std::setw(15) << std::setbase(10) << vi_Chip_FrameCounter
      << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
    }

    /* ------------------------------------------------------ */
    /* Chip_DataLength */
    ifs_DataRaw.read((char *)&vi_Chip_DataLength_1, WORD);
    ifs_DataRaw.read((char *)&vi_Chip_DataLength_2, WORD);
    vi_Chip_DataLength_1       = SwitchWordBytes(vi_Chip_DataLength_1);
    vi_Chip_DataLength_2       = SwitchWordBytes(vi_Chip_DataLength_2);
    vi_Chip_DataLength         = vi_Chip_DataLength_1 + vi_Chip_DataLength_2;
    vi_Pointer_Data = ifs_DataRaw.tellg();
    vi_Pointer_Chip_DataLength = vi_Pointer_Data;
    if (vi_Verbose < 6)
    {
      cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Chip_DataLength is             : "
      << std::setw(15) << std::setbase(16) << vi_Chip_DataLength
      << std::setw(15) << std::setbase(10) << vi_Chip_DataLength
      << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
    }

    /* Check the the file size */
    if ((vi_Chip_DataLength*WORD + DWORD) > vi_Ladder_DataLength*WORD)
    {
      cout << "  ERROR : Mi28DecodeLadderDataToRoot(), file frame is incomplete at !! Chip_Trailer !!, STOP DECODING!" << endl;
      vi_N_Frame_Bad++;
      break;
    }

    /* ------------------------------------------------------ */
    /* Chip_Trailer */
    ifs_DataRaw.seekg(vi_Chip_DataLength*WORD, ios::cur);
    ifs_DataRaw.read((char *)&vi_Chip_Trailer, DWORD);
    vi_Pointer_Data = ifs_DataRaw.tellg();
    ifs_DataRaw.seekg(-vi_Chip_DataLength*WORD - DWORD, ios::cur); /* Return the Pointer */
    vi_Chip_Trailer = SwitchDWordWords(vi_Chip_Trailer);
    if (vi_Chip_Trailer != M_CHIP_TRAILER)
    {
      if (vi_Verbose < 11)
      {
        cout << "  ERROR : Mi28DecodeLadderDataToRoot(), find a bad Checking Chip_Trailer : "
        << std::setw(15) << std::setbase(16) << vi_Chip_Trailer << "  VS  "
        << std::setw(11) << std::setbase(16) << M_CHIP_TRAILER  << "\t@\t"
        << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
      }

      /* Return to Pointer Ladder_Chip, Continue a new Ladder_Chip */
      ifs_DataRaw.seekg(BYTE - 4*DWORD, ios::cur); /* Return the Pointer */
      continue; /* Continue to Check Ladder_Chip */
    }
    else
    {
      if (vi_Verbose < 6)
      {
        cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Checking Chip_Trailer is       : "
        << std::setw(15) << std::setbase(16) << vi_Chip_Trailer
        << std::setw(15) << std::setbase(10) << vi_Chip_Trailer
        << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
      }
    }

    /* ------------------------------------------------------ */
    /*                   Deal with useful data                */
    /* ------------------------------------------------------ */
    while (ifs_DataRaw.tellg() < (vi_Pointer_Chip_DataLength + vi_Chip_DataLength*WORD))
    {
      /* Chip_Status */
      ifs_DataRaw.read((char *)&vi_DataRaw, WORD);
      vi_Pointer_Data = ifs_DataRaw.tellg();

      vi_Chip_Status          = SwitchWordBytes(vi_DataRaw);
      vi_Chip_N_State         = vi_Chip_Status  & M_CHIP_N_STATE;
      vi_Chip_Address_Line    = (vi_Chip_Status & M_CHIP_ADDRESS_LINE)    >> 4;
      vi_Chip_Flag_Residual_1 = (vi_Chip_Status & M_CHIP_FLAG_RESIDUAL_1) >> 14;
      vi_Chip_Flag_OverFlow   = (vi_Chip_Status & M_CHIP_FLAG_OVERFLOW)   >> 15;

      if (vi_Verbose < 3)
      {
        vi_Pointer_Data = ifs_DataRaw.tellp();
        cout << "  SSSSS" << endl;
        cout << "  INFO : Status / Pointer_Status / N_State / Address_Line   is : "
        << std::setw(12) << std::setbase(16) << vi_Chip_Status
        << std::setw(12) << std::setbase(10) << vi_Pointer_Data
        << std::setw(12) << std::setbase(10) << vi_Chip_N_State
        << std::setw(12) << std::setbase(10) << vi_Chip_Address_Line   << endl;
      }

      /* Check the status word */
      if ((vi_Chip_Address_Line>N_ROW) || (vi_Chip_Address_Line<0))
      {
        if (vi_Verbose < 11)
        {
          cout << "  ERROR : Mi28DecodeLadderDataToRoot(), Line counter is wrong, which is "
          << std::setbase(10) << vi_Chip_Address_Line << " VS "
          << std::setbase(10) << N_ROW << endl;
        }
        vi_N_Frame_Bad++;
        break;
      }
      if (vi_Chip_Flag_Residual_1 != 0)
      {
        if (vi_Verbose < 4)
        {
          cout << "  Warning : Mi28DecodeLadderDataToRoot(), Status is wrong! The status residual bit should be 0, not should be "
          << std::setbase(10) << vi_Chip_Flag_Residual_1 << endl;
        }
      }
      if (vi_Chip_Flag_OverFlow == 1)
      {
        if (vi_Verbose < 4)
        {
          cout << "  Warning : Mi28DecodeLadderDataToRoot(), The status is overflow, the bit is "
          << std::setbase(10) << vi_Chip_Flag_OverFlow << " @ line "
          << std::setbase(10) << vi_Chip_Address_Line  << endl;
        }
      }
      /* Skip the last data word, if not, will lead to read more word for this frame */
      //if ((vi_Pointer_Data + vi_Chip_N_State) >= (vi_Pointer_Chip_DataLength + vi_Chip_DataLength*WORD))
      if (vi_Pointer_Data == (vi_Pointer_Chip_DataLength + vi_Chip_DataLength*WORD))
      {
        if (vi_Verbose < 11)
        {
          cout << "  Warning : Mi28DecodeLadderDataToRoot(), The useful word exceeds the data length, skip this word which is @ "
          << std::setbase(10) << vi_Pointer_Data    << " + "
          << std::setbase(10) << vi_Chip_N_State    << " VS "
          << std::setbase(10) << (vi_Pointer_Chip_DataLength + vi_Chip_DataLength*WORD)
          << endl;
        }
        vi_N_Frame_Bad++;
        break;
      }

      for (unsigned long int iState=0; iState<vi_Chip_N_State; iState++)
      {
        /* Chip_State */
        ifs_DataRaw.read((char *)&vi_DataRaw, WORD);
        vi_Pointer_Data = ifs_DataRaw.tellp();

        vi_Chip_State           = SwitchWordBytes(vi_DataRaw);
        vi_Chip_N_Pixel         = vi_Chip_State  & M_CHIP_N_PIXEL;
        vi_Chip_Address_Column  = (vi_Chip_State & M_CHIP_ADDRESS_COLUMN)  >> 2;
        vi_Chip_Flag_Residual_2 = (vi_Chip_State & M_CHIP_FLAG_RESIDUAL_2) >> 12;

        if (vi_Verbose < 3)
        {
          cout << "  INFO : State  / Pointer_State  / N_Pixel / Address_Column is : "
          << std::setw(12) << std::setbase(16) << vi_Chip_State
          << std::setw(12) << std::setbase(10) << vi_Pointer_Data
          << std::setw(12) << std::setbase(10) << vi_Chip_N_Pixel
          << std::setw(12) << std::setbase(10) << vi_Chip_Address_Column << endl;
        }

        /* Check the state word */
        if ((vi_Chip_Address_Column>N_COLUMN) || (vi_Chip_Address_Column<0))
        {
          if (vi_Verbose < 11)
          {
            cout << "  ERROR : Mi28DecodeLadderDataToRoot(), Column counter is wrong, which is "
            << std::setbase(10) << vi_Chip_Address_Column << " VS "
            << std::setbase(10) << N_COLUMN << endl;
          }
          vi_N_Frame_Bad++;
          break;
        }
        if (vi_Chip_Flag_Residual_2 != 0)
        {
          if (vi_Verbose < 4)
          {
            cout << "  Warning : Mi28DecodeLadderDataToRoot(), state is wrong! The state residual bits should be 0, not should be "
            << std::setbase(10) << vi_Chip_Flag_Residual_2 << endl;
          }
        }

        /* +1 means that vi_Chip_N_Pixel is the number of pixels after the first column */
        for (unsigned long int iPixel=0; iPixel<vi_Chip_N_Pixel+1; iPixel++)
        {

          AddPixel( vi_ID_Ladder_Chip+vi_ID_Ladder, 1, vi_Chip_Address_Line, vi_Column_Temp);
          // vi_Column_Temp = vi_Chip_Address_Column+iPixel;
          // vi_Tree_N_Trigger              = vi_Ladder_Trigger;
          // vi_Tree_N_Ladder_Frame         = vi_N_Frame_Good + 1;
          // vi_Tree_N_Ladder_Chip          = 2;
          // vi_Tree_ID_Ladder              = vi_ID_Ladder;
          // vi_Tree_ID_Ladder_FrameCounter = vi_Ladder_FrameCounter;
          // vi_Tree_ID_Ladder_Chip         = vi_ID_Ladder_Chip;
          // vi_Tree_ID_Chip_FrameCounter   = vi_Chip_FrameCounter;
          // vi_Tree_ID_Chip_Column         = vi_Column_Temp;
          // vi_Tree_ID_Chip_Row            = vi_Chip_Address_Line;
          //
          // Tree_FiredPixel->Fill();

          vd_N_PixelTotal++;

          if ((vi_Column_Temp >= N_BANKCOLUMN*0) && (vi_Column_Temp < N_BANKCOLUMN*1))
          {
            vd_N_PixelBankA++;
          }
          if ((vi_Column_Temp >= N_BANKCOLUMN*1) && (vi_Column_Temp < N_BANKCOLUMN*2))
          {
            vd_N_PixelBankB++;
          }
          if ((vi_Column_Temp >= N_BANKCOLUMN*2) && (vi_Column_Temp < N_BANKCOLUMN*3))
          {
            vd_N_PixelBankC++;
          }
          if ((vi_Column_Temp >= N_BANKCOLUMN*3) && (vi_Column_Temp < N_BANKCOLUMN*4))
          {
            vd_N_PixelBankD++;
          }
        } // End of for (unsigned long int iPixel

      } // End of for (int iState=0; iState<vi_Chip_N_State; iState++)

    } // End of 'while ((ifs_DataRaw.tellg() - vi_Pointer_Chip_DataLength) < vi_Chip_DataLength)'

    /* ------------------------------------------------------ */
    /* Chip_Trailer */
    ifs_DataRaw.read((char *)&vi_Chip_Trailer, DWORD);
    vi_Pointer_Data = ifs_DataRaw.tellg();
    vi_Chip_Trailer = SwitchDWordWords(vi_Chip_Trailer);
    if (vi_Chip_Trailer != M_CHIP_TRAILER)
    {
      if (vi_Verbose < 11)
      {
        cout << "  ERROR : Mi28DecodeLadderDataToRoot(), find a bad Scanning Chip_Trailer  : "
        << std::setw(15) << std::setbase(16) << vi_Chip_Trailer << "  VS  "
        << std::setw(11) << std::setbase(16) << M_CHIP_TRAILER  << "\t@\t"
        << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
      }
      break;
    }
    else
    {
      if (vi_Verbose < 6)
      {
        cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Scanning Chip_Trailer is       : "
        << std::setw(15) << std::setbase(16) << vi_Chip_Trailer
        << std::setw(15) << std::setbase(10) << vi_Chip_Trailer
        << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
      }
    }
    vi_N_Ladder_Chip++;

    /* ------------------------------------------------------ */
    /* Ladder_Trailer */
    ifs_DataRaw.read((char *)&vi_Ladder_Trailer, DWORD);
    vi_Pointer_Data = ifs_DataRaw.tellg();
    vi_Ladder_Trailer = SwitchDWordWords(vi_Ladder_Trailer);
    if (vi_Ladder_Trailer != M_LADDER_TRAILER)
    {
      if (vi_Verbose < 11)
      {
        cout << "  ERROR : Mi28DecodeLadderDataToRoot(), find a bad Scannin Ladder_Trailer : "
        << std::setw(15) << std::setbase(16) << vi_Ladder_Trailer << "  VS  "
        << std::setw(11) << std::setbase(16) << M_LADDER_TRAILER  << "\t@\t"
        << std::setw(15) << std::setbase(10) << vi_Pointer_Data   << endl;
      }
      break;
    }
    else
    {
      if (vi_Verbose < 6)
      {
        cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Scanning Ladder_Trailer is     : "
        << std::setw(15) << std::setbase(16) << vi_Ladder_Trailer
        << std::setw(15) << std::setbase(10) << vi_Ladder_Trailer
        << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
      }
    }

    frameOK = true;
    vi_N_Frame_Good++;
    if (vi_Verbose < 11)
    {
      cout << "  SUCCESS : Mi28DecodeLadderDataToRoot(), Finish Good Ladder Frame " << vi_N_Frame_Good << " with Chips = " << vi_N_Ladder_Chip
      << " @ "  << std::setw(15) << std::setbase(10) << ifs_DataRaw.tellg()
      << " VS " << std::setw(15) << std::setbase(10) << vi_Pointer_FileEnd \
      << endl;
    }

  } // End of while (ifs_DataRaw.tellg() < vi_Pointer_FileEnd + vi_Ladder_DataLength*WORD)

  return frameOK;
}

// --------------------------------------------------------------------------------------

void BoardReaderIHEP::PrintEventHeader() {

  // Print Event Header

}

// --------------------------------------------------------------------------------------

void BoardReaderIHEP::PrintStatistics(ostream &stream) {

  // Print statistics on the events read by this board
  //
  //

 stream << "***********************************************" << endl;
 stream << " Board IHEP " << fBoardNumber << " found:" << endl;
 // stream << fCurrentTriggerCnt << " triggers." << endl;
 stream << fEventNumber << " events in total," << endl;
 // stream << fEventsOverflow << " events with an overflow (veto overflow " << fVetoOverflow << ")." << endl;
 // stream << fFramesReadFromFile<< " frames read overall." << endl;
 stream << "***********************************************" << endl;

 double vd_FakeRateTotal = vd_N_PixelTotal / N_COLUMN  / N_ROW / vi_N_Frame_Good;
 double vd_FakeRateBankA = vd_N_PixelBankA / N_BANKCOLUMN / N_ROW / vi_N_Frame_Good;
 double vd_FakeRateBankB = vd_N_PixelBankB / N_BANKCOLUMN / N_ROW / vi_N_Frame_Good;
 double vd_FakeRateBankC = vd_N_PixelBankC / N_BANKCOLUMN / N_ROW / vi_N_Frame_Good;
 double vd_FakeRateBankD = vd_N_PixelBankD / N_BANKCOLUMN / N_ROW / vi_N_Frame_Good;

 cout << "------------------------------------------------------"   << endl;
 cout << resetiosflags(ios::scientific);
 cout << "INFO : Mi28DecodeLadderDataToRoot(), total good frames are  " << setw(9) << vi_N_Frame_Good << endl;
 cout << "INFO : Mi28DecodeLadderDataToRoot(), total bad  frames are  " << setw(9) << vi_N_Frame_Bad  << endl;
 cout << endl;
 cout << "------------------------------------------------------" << endl;
 cout << "INFO : Mi28DecodeLadderDataToRoot(), total hit  pixels are  " << setw(9) << right << vd_N_PixelTotal / vi_N_Frame_Good << "\tvs\t" << N_COLUMN *N_ROW << endl;
 cout << endl;
 cout << "INFO : Mi28DecodeLadderDataToRoot(), BankA hit  pixels are  " << setw(9) << right << vd_N_PixelBankA / vi_N_Frame_Good << "\tvs\t" << N_BANKCOLUMN*N_ROW << endl;
 cout << "INFO : Mi28DecodeLadderDataToRoot(), BankB hit  pixels are  " << setw(9) << right << vd_N_PixelBankB / vi_N_Frame_Good << "\tvs\t" << N_BANKCOLUMN*N_ROW << endl;
 cout << "INFO : Mi28DecodeLadderDataToRoot(), BankC hit  pixels are  " << setw(9) << right << vd_N_PixelBankC / vi_N_Frame_Good << "\tvs\t" << N_BANKCOLUMN*N_ROW << endl;
 cout << "INFO : Mi28DecodeLadderDataToRoot(), BankD hit  pixels are  " << setw(9) << right << vd_N_PixelBankD / vi_N_Frame_Good << "\tvs\t" << N_BANKCOLUMN*N_ROW << endl;
 cout << "------------------------------------------------------" << endl;
 cout << "INFO : Mi28DecodeLadderDataToRoot(), total hit fake rate is " << setw(9) << setprecision(6) << scientific << vd_FakeRateTotal << endl;
 cout << endl;
 cout << "INFO : Mi28DecodeLadderDataToRoot(), BankA hit fake rate is " << setw(9) << setprecision(6) << scientific << vd_FakeRateBankA << endl;
 cout << "INFO : Mi28DecodeLadderDataToRoot(), BankB hit fake rate is " << setw(9) << setprecision(6) << scientific << vd_FakeRateBankB << endl;
 cout << "INFO : Mi28DecodeLadderDataToRoot(), BankC hit fake rate is " << setw(9) << setprecision(6) << scientific << vd_FakeRateBankC << endl;
 cout << "INFO : Mi28DecodeLadderDataToRoot(), BankD hit fake rate is " << setw(9) << setprecision(6) << scientific << vd_FakeRateBankD << endl;
 cout << resetiosflags(ios::scientific);

}

// --------------------------------------------------------------------------------------

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// 3412 -> 1234
unsigned long int BoardReaderIHEP::SwitchWordBytes(unsigned long int fi_Word)
{

  // original method sent on 2018/08/14

  const unsigned long int BYTE_H = 65280; // 1111 1111 0000 0000
  const unsigned long int BYTE_L = 255;   // 0000 0000 1111 1111

  unsigned long int vi_Word_0 = (fi_Word & BYTE_L);
  unsigned long int vi_Word_1 = (fi_Word & BYTE_H) >> 8;
  //unsigned long int vi_Word_2 = (vi_Word_0 << 8) + vi_Word_1;
unsigned long int vi_Word_2 = (vi_Word_1 << 8) + vi_Word_0;
  return vi_Word_2;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// 7856 3412 -> 1234 5678
unsigned long int BoardReaderIHEP::SwitchDWordBytes(unsigned long int fi_DWord)
{

  // original method sent on 2018/08/14

  const unsigned long int WORD_H = 4294901760; // 1111 1111 1111 1111 0000 0000 0000 0000
  const unsigned long int WORD_L = 65535;      // 0000 0000 0000 0000 1111 1111 1111 1111

  unsigned long int vi_DWord_0 = (fi_DWord & WORD_L);
  unsigned long int vi_DWord_1 = (fi_DWord & WORD_H) >> 16;

  vi_DWord_0 = SwitchWordBytes(vi_DWord_0);
  vi_DWord_1 = SwitchWordBytes(vi_DWord_1);

  unsigned long int vi_DWord_2 = (vi_DWord_0 << 16) + vi_DWord_1;

  return vi_DWord_2;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// 7856 3412 -> 5678 1234
unsigned long int BoardReaderIHEP::SwitchDWordBytes2(unsigned long int fi_DWord)
{

  // original method sent on 2018/08/14

  const unsigned long int WORD_H = 4294901760; // 1111 1111 1111 1111 0000 0000 0000 0000
  const unsigned long int WORD_L = 65535;      // 0000 0000 0000 0000 1111 1111 1111 1111

  unsigned long int vi_DWord_0 = (fi_DWord & WORD_L);
  unsigned long int vi_DWord_1 = (fi_DWord & WORD_H) >> 16;

  vi_DWord_0 = SwitchWordBytes(vi_DWord_0);
  vi_DWord_1 = SwitchWordBytes(vi_DWord_1);

  unsigned long int vi_DWord_2 = (vi_DWord_1 << 16) + vi_DWord_0;

  return vi_DWord_2;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// 5678 1234 -> 1234 5678
unsigned long int BoardReaderIHEP::SwitchDWordWords(unsigned long int fi_DWord)
{

// original method sent on 2018/08/14

  const unsigned long int WORD_H = 4294901760; // 1111 1111 1111 1111 0000 0000 0000 0000
  const unsigned long int WORD_L = 65535;      // 0000 0000 0000 0000 1111 1111 1111 1111

  unsigned long int vi_DWord_0 = (fi_DWord & WORD_L);
  unsigned long int vi_DWord_1 = (fi_DWord & WORD_H) >> 16;

  unsigned long int vi_DWord_2 = (vi_DWord_1 << 16) + vi_DWord_0;

  return vi_DWord_2;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......ll_120-4.5-A208
