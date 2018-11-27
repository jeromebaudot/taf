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

  vi_Verbose = 100;

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
  M_LADDER_TriggerHeader  = 0xDDDDDDDD ; // Ladder Trigger Header  Marker 32 bits : 0x55555555, Check all the 32 bits
  M_LADDER_TriggerTRAILER = 0x66666666 ; // Ladder Trigger Trailer Marker 32 bits : 0x66666666, Check all the 32 bits

  vi_Trigger_Header      = 0;
  vi_Trigger_ID          = 0;
  vi_Pack_Length         = 0;
  vi_Pack_State          = 0;
  vi_Trigger_Trailer     = 0;
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
  vi_Pointer_Pack_DataLength   = 0;
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
  fEventBuildingMode = eventBuildingMode;

  cout << "*****************************************" << endl;
  cout << "    < BoardReaderIHEP constructor >      " << endl;
  cout << "*****************************************" << endl;
  cout << "Creating a BoardReaderIHEP" << endl;
  cout << " * for board : " << fBoardNumber << endl;
  cout << " * for sensor type : MIMOSA-28" << endl;
  cout << " * with eventBuildingMode : " << fEventBuildingMode << endl;

  cout << " INFO BoardReaderIHEP the Data format  is                : "
  << std::setw(15) << "hex"
  << std::setw(15) << "dec"
  << std::setw(15) << "Pointer" << endl;

  fEventNumber = 0;
  fCurrentEvent = NULL;
  i_Trigger=0;
  for (int it=0; it<6; it++) { Trigger[it] = 0; }
  memset(Trigger_Framcount, 0, sizeof(Trigger_Framcount));

  iFile = 0;

  cout << "    < BoardReaderIHEP constructor DONE >      " << endl;
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
  // Modified, JB, 2018/10/08

  // -+-+- Initialization

  bool eventOK   = true;    // we start with a good event by default
  fCurrentEvent   = 0;       // Allow to know wether data are correct
  ListOfTriggers.clear(); // and clear the vectors
  ListOfTimestamps.clear();
  ListOfFrames.clear();
  ListOfPixels.clear();

 if(vi_Verbose<10) {
   cout << endl ;
   cout << "BoardReaderIHEP::HasData() BoardNumber " << fBoardNumber << " IMGBoardReader::HasData() - currentEvent " << fEventNumber << " currentTrigger " << i_Trigger << endl;
 }

 if( DecodeNextEvent() ) {

   if( vi_Verbose<10 ) PrintEventHeader();

   fCurrentEvent = new BoardReaderEvent( fEventNumber, fBoardNumber, 0, &ListOfPixels); // run number set to 0 for now
   fCurrentEvent->SetListOfTriggers( &ListOfTriggers);
   fCurrentEvent->SetListOfFrames( &ListOfFrames);
   if( vi_Verbose<10 ) cout << " BoardNumber " << fBoardNumber << " create new event " << fEventNumber << " with " << ListOfPixels.size() << " pixels" << " from " << ListOfTriggers.size() << " and " << ListOfFrames.size() << " frames." << endl;
   fEventNumber++;

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
bool BoardReaderIHEP::DecodeNextEvent()
{
  // Set buffers to the next event
  //
  // Code extracted from Mi28DecodeLadderDataToRoot.cc
  //  provided IHEP group 2018/06
  //  Author: juxd, juxd@ihep.ac.cn
  //  Updated 2018/10/12

  // Decoding status
  bool ready = false;
  int timestamp = 0; // it will be 1 or 2
  int frameID2setTime = 0;
  int ladderID2setTime = 0;
  int chipID2setTime = 0;


  if (vi_Verbose < 3) {
    cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Data format  is                : "
    << std::setw(15) << "hex"
    << std::setw(15) << "dec"
    << std::setw(15) << "Pointer" << endl;
    cout << "......................................................................................................................" << endl;
  }

  /* ------------------------------------------------------ */
  /* DWORD Data format */
  /* Hex in VIM                    : 5678 1234 */
  /* Hex in stream                 : 3412 7856 */
  /* Hex after SwitchDWordBytes2() : 1234 5678 */
  //while (!ifs_DataRaw.eof())
  while (ifs_DataRaw.tellg() != vi_Pointer_FileEnd && !ready)
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
        /* Trigger_Header*/
        ifs_DataRaw.read((char *)&vi_Trigger_Header, DWORD);
        vi_Pointer_Data         = ifs_DataRaw.tellg();
        vi_Trigger_Header = SwitchDWordWords(vi_Trigger_Header);
        if (vi_Trigger_Header != M_LADDER_TriggerHeader)
        {
          if (vi_Verbose < 11)
          {
            cout << "  ERROR : Mi28DecodeLadderDataToRoot(), find a bad Checking Trigger_Header : "
                 << std::setw(15) << std::setbase(16) << vi_Trigger_Header << "  VS  "
                 << std::setw(11) << std::setbase(16) << M_LADDER_TriggerHeader  << "\t@\t"
                 << std::setw(15) << std::setbase(10) << vi_Pointer_Data   << endl;
          }
          ifs_DataRaw.seekg(BYTE -DWORD, ios::cur); /* Return the Pointer */
         continue; /* Continue to Check Trigger_Header */
         }
         else
        {
          if (vi_Verbose < 6)
          {
            cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Checking Trigger_Header is     : "
                 << std::setw(15) << std::setbase(16) << vi_Trigger_Header
                 << std::setw(15) << std::setbase(10) << vi_Trigger_Header
                 << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
          }
        }
        /*-------------------------------------------------------*/
        /* Trigger ID */
        ifs_DataRaw.read((char *)&vi_Trigger_ID, DWORD);
        vi_Pointer_Data        = ifs_DataRaw.tellg();
        vi_Trigger_ID = SwitchDWordWords(vi_Trigger_ID);
        if (vi_Verbose < 6)
        {
          cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Trigger_ID is         : "
               << std::setw(15) << std::setbase(16) << vi_Trigger_ID
               << std::setw(15) << std::setbase(10) << vi_Trigger_ID
               << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
        }
        /*--------------------------------------------------------*/
        /*PackLength*/
        ifs_DataRaw.read((char *)&vi_Pack_Length, DWORD);
        vi_Pointer_Data      = ifs_DataRaw.tellg();
        vi_Pack_Length = SwitchDWordWords(vi_Pack_Length);
        vi_Pointer_Pack_DataLength = vi_Pointer_Data;
        if (vi_Verbose < 6)
        {
          cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Pack_Length is           : "
               << std::setw(15) << std::setbase(16) << vi_Pack_Length
               << std::setw(15) << std::setbase(10) << vi_Pack_Length
               << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
        }


        /* Check the the file size */
        if ((vi_Pointer_Data + vi_Pack_Length*BYTE + DWORD) > vi_Pointer_FileEnd)
        {
          cout << "  ERROR : Mi28DecodeLadderDataToRoot(), file is incomplete at !! Trigger Trailer !!, STOP DECODING!" << endl;
          vi_N_Frame_Bad++;
          break;
        }
        /*-----------------------------------------------------------*/
        /* Pack State*/
          ifs_DataRaw.seekg(WORD, ios::cur);
        /*------------------------------------------------------------*/
         /* ------------------------------------------------------ */
        /*-------------------------------------------------------*/
        /* Trigger Trailer*/
        ifs_DataRaw.seekg(vi_Pack_Length*BYTE, ios::cur);
        ifs_DataRaw.read((char *)&vi_Trigger_Trailer, DWORD);
        vi_Pointer_Data = ifs_DataRaw.tellg();
        ifs_DataRaw.seekg(-vi_Pack_Length*BYTE - DWORD, ios::cur); /* Return the Pointer */
        vi_Trigger_Trailer = SwitchDWordWords(vi_Trigger_Trailer);
        if (vi_Trigger_Trailer != M_LADDER_TriggerTRAILER)
        {
          if (vi_Verbose < 11)
          {
            cout << "  ERROR : Mi28DecodeLadderDataToRoot(), find a bad Checking Trigger_Trailer : "
                 << std::setw(15) << std::setbase(16) << vi_Trigger_Trailer << "  VS  "
                 << std::setw(11) << std::setbase(16) << M_LADDER_TriggerTRAILER  << "\t@\t"
                 << std::setw(15) << std::setbase(10) << vi_Pointer_Data   << endl;
          }

          /* Return to Pointer Ladder_DataLength, Continue a new Ladder_Frame */
          ifs_DataRaw.seekg(BYTE - 3*DWORD, ios::cur); /* Return the Pointer */
          continue; /* Continue to Check Trigger_Header */
        }
        else
        {
          if (vi_Verbose < 6)
          {
            cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Checking Trigger_Trailer is     : "
                 << std::setw(15) << std::setbase(16) << vi_Trigger_Trailer
                 << std::setw(15) << std::setbase(10) << vi_Trigger_Trailer
                 << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
          }
        }
        /*-----------------------------------------------------------*/
        /* Recycling decode the data for less than 5 Ladder */
        /*-----------------------------------------------------------*/
        int  vi_N_Ladder      = 0;
        int vi_N_Ladder_Chip = 0;
        while (ifs_DataRaw.tellg() < (vi_Pointer_Pack_DataLength + vi_Pack_Length*BYTE))
        {
          /*-----------------------------------------------------------*/
          /* Pack State*/

          /*------------------------------------------------------------*/
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

          /* ------------------------------------------------------ */

          /* ------------------------------------------------------ */
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

          // if the timestamp has not be set yet,
          // => start with 1
          // => new trigger
          if( timestamp==0 ) {
            ListOfTriggers.push_back( vi_Ladder_FrameCounter);
            timestamp = 1;
            frameID2setTime = vi_Ladder_FrameCounter;
            ladderID2setTime = vi_ID_Ladder;
          }
          // if the ladder ID is different,
          // => this is the 1st timestamp of a new ladder
          else if( ladderID2setTime!=vi_ID_Ladder ) {
            timestamp = 1;
            frameID2setTime = vi_Ladder_FrameCounter;
            ladderID2setTime = vi_ID_Ladder;
          }
          // if the timestamp is already 1 for the same ladder.
          // => this is the second frame
          else if( timestamp==1 && ladderID2setTime==vi_ID_Ladder ){
            timestamp = 2;
          }
          ListOfFrames.push_back(vi_Ladder_FrameCounter);


          /* ------------------------------------------------------ */
          /* Ladder_DataLength */
          ifs_DataRaw.read((char *)&vi_Ladder_DataLength, DWORD);
          vi_Pointer_Data      = ifs_DataRaw.tellg();
          vi_Ladder_DataLength = SwitchDWordWords(vi_Ladder_DataLength);
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
            ifs_DataRaw.seekg(BYTE - 4*DWORD, ios::cur); /* Return the Pointer */
            continue; /* Continue to Check Ladder_Header */
          }
          else
          {
            if (vi_Verbose < 6)
            {
              cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Checking Ladder_Trailer is     : "
              << std::setw(15) << std::setbase(16) << vi_Ladder_Trailer
              << std::setw(15) << std::setbase(10) << vi_Ladder_Trailer
              << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
            }
          }

          /* ------------------------------------------------------ */
          /* Recycling decode the data for less than 10 chips */
          /* ------------------------------------------------------ */

          while (ifs_DataRaw.tellg() < (vi_Pointer_Ladder_DataLength + vi_Ladder_DataLength*WORD))
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

              /* Skip the last data word, if not, will lead to read more one word for this frame */
              if ((vi_Pointer_Data + vi_Chip_N_State*WORD) > (vi_Pointer_Chip_DataLength + vi_Chip_DataLength*WORD))
              {
                unsigned long int Checking_Stata=0;
                unsigned long int M_AAAA=0xAAAA;
                for(int i_word=0;i_word<100;i_word++)
                {
                  ifs_DataRaw.read((char *)&Checking_Stata, WORD);
                  Checking_Stata=SwitchWordBytes(Checking_Stata);
                  if(Checking_Stata == M_AAAA)
                  {
                      // cout << "  Warning : Mi28DecodeLadderDataToRoot(), The useful word exceeds the data length, skip this Stata which is @ "
                      // << std::setbase(10) << vi_Pointer_Data    << " + "
                      // << std::setbase(10) << vi_Chip_N_State    << " + "<< std::setbase(10) <<i_word<<" VS "
                      // << std::setbase(10) << (vi_Pointer_Chip_DataLength + vi_Chip_DataLength*WORD)
                      // << endl;
                    ifs_DataRaw.seekg( - WORD, ios::cur);
                    break;
                  }
                }
                vi_N_Frame_Bad++;
                break;
              }
              /* Skip the last data word, if not, will lead to read more word for this frame */
              if (vi_Pointer_Data == (vi_Pointer_Chip_DataLength + vi_Chip_DataLength*WORD)&&vi_Chip_N_State==0)
              {
                if (vi_Verbose < 11)
                {
                  cout << "  Warning : Mi28DecodeLadderDataToRoot(), The useful word  exceeds the data length, skip this word which is @ "
                  << std::setbase(10) << vi_Pointer_Data    << " + "
                  << std::setbase(10) << vi_Chip_N_State    << " + "<< std::setbase(10) <<0<<" VS "
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
                  vi_Column_Temp = vi_Chip_Address_Column+iPixel;
                  vd_N_PixelTotal++;

                  // AddPixel( vi_ID_Ladder_Chip+(vi_ID_Ladder-1)*2, 1, vi_Chip_Address_Line, vi_Column_Temp, timestamp);
                  // if (vi_Verbose < 7) {
                  //   printf( "  Adding pixel from ladder %d, Chip %d -> input %d, line %d column %d, timestamp %d\n", vi_ID_Ladder, vi_ID_Ladder_Chip, vi_ID_Ladder_Chip+(vi_ID_Ladder-1)*2, vi_Chip_Address_Line, vi_Column_Temp, timestamp);
                  // }

                  int input;
                  switch ( fEventBuildingMode ) {
                    case 1:
                      input = vi_ID_Ladder_Chip+(vi_ID_Ladder-1)*10;
                      break;

                    case 2:
                        input = vi_ID_Ladder;
                        break;

                    default:
                    input = vi_ID_Ladder_Chip+(vi_ID_Ladder-1)*2;
                  }
                  AddPixel( input, 1, vi_Chip_Address_Line, vi_Column_Temp, timestamp);
                  if (vi_Verbose < 7) {
                    printf( "  Adding pixel from ladder %d, Chip %d -> input %d, line %d column %d, timestamp %d\n", vi_ID_Ladder, vi_ID_Ladder_Chip, input, vi_Chip_Address_Line, vi_Column_Temp, timestamp);
                  }

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
                } // End of 'for (unsigned long int iPixel=0; iPixel<vi_Chip_N_Pixel+1; iPixel++)'

              } /* End of 'for (int iState=0; iState<vi_Chip_N_State; iState++)' */
            } /* End of 'while ((ifs_DataRaw.tellg() - vi_Pointer_Chip_DataLength) < vi_Chip_DataLength)' */

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
          } /* End of 'while ((ifs_DataRaw.tellg() - vi_Pointer_Ladder_DataLength) < vi_Ladder_DataLength)' */

          /* ------------------------------------------------------ */
          /* Ladder_Trailer */
          ifs_DataRaw.read((char *)&vi_Ladder_Trailer, DWORD);
          vi_Pointer_Data = ifs_DataRaw.tellg();
          vi_Ladder_Trailer = SwitchDWordWords(vi_Ladder_Trailer);
          if (vi_Ladder_Trailer != M_LADDER_TRAILER)
          {
            // if (vi_Verbose < 11)
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
          if (vi_Verbose < 11)
          {
            cout << "  SUCCESS : Mi28DecodeLadderDataToRoot(), Finish Good Ladder Frame " << vi_N_Frame_Good << " with Chips = " << vi_N_Ladder_Chip
            << " @ "  << std::setw(15) << std::setbase(10) << ifs_DataRaw.tellg()
            << " VS " << std::setw(15) << std::setbase(10) << vi_Pointer_FileEnd
            << endl;
          }

          vi_N_Ladder++;
        }/* End of 'while (ifs_DataRaw.tellg() < (vi_Pointer_Ladder_PackLength + vi_Ladder_PackLength*WORD))'*/

        /* ------------------------------------------------------ */
        /* Trigger_Trailer */
        ifs_DataRaw.read((char *)&vi_Trigger_Trailer, DWORD);
        vi_Pointer_Data = ifs_DataRaw.tellg();
        vi_Trigger_Trailer = SwitchDWordWords(vi_Trigger_Trailer);
        if (vi_Trigger_Trailer != M_LADDER_TriggerTRAILER)
        {
          //if (vi_Verbose < 11)
          {
            cout << "  ERROR : Mi28DecodeLadderDataToRoot(), find a bad Checking Trigger_Trailer : "
                 << std::setw(15) << std::setbase(16) << vi_Trigger_Trailer << "  VS  "
                 << std::setw(11) << std::setbase(16) << M_LADDER_TriggerTRAILER  << "\t@\t"
                 << std::setw(15) << std::setbase(10) << vi_Pointer_Data   << endl;
          }

         break;
        }
        else
        {
          if (vi_Verbose < 6)
          {
            cout << "  INFO : Mi28DecodeLadderDataToRoot(), the Checking Trigger_Trailer is     : "
                 << std::setw(15) << std::setbase(16) << vi_Trigger_Trailer
                 << std::setw(15) << std::setbase(10) << vi_Trigger_Trailer
                 << std::setw(15) << std::setbase(10) << vi_Pointer_Data << endl;
          }
         vi_N_Frame_Good++;
         ready = true;
        }
       /*---------------------------------------------------------------*/

        if (vi_Verbose < 11)
        {
          cout << "  SUCCESS : Mi28DecodeLadderDataToRoot(), Finish Good Ladder Frame " << vi_N_Frame_Good << " with Chips = " << vi_N_Ladder
               << " @ "  << std::setw(15) << std::setbase(10) << ifs_DataRaw.tellg()
               << " VS " << std::setw(15) << std::setbase(10) << vi_Pointer_FileEnd \
               << endl;
        }

    } /* End of 'if (ifs_DataRaw.good())' */
  } /* End of 'while (!ifs_DataRaw.eof())' */


  return ready;

}


// --------------------------------------------------------------------------------------
void BoardReaderIHEP::AddPixel( int iSensor, int value, int aLine, int aColumn, int aTime)
{
   // Add a pixel to the vector of pixels
   // require the following info
   // - input = number of the sensors
   // - value = analog value of this pixel
   // - line & column = position of the pixel in the matrix
   // - time = 1 or 2, indicate whether this is 1st or 2nd frame wrt trigger

//  if (vi_Verbose<2) printf("BoardReaderIHEP::Addpixel adding pixel for sensor %d with value %d line %d row %d\n", iSensor, value, aLine, aColumn, aTime);

  ListOfPixels.push_back( BoardReaderPixel( iSensor, value, aLine, aColumn, aTime) );

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
