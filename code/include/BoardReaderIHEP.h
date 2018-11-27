#ifndef _BoardReaderIHEP_included_
#define _BoardReaderIHEP_included_

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <set>
#include <cstdlib>
#include <cmath>
#include <cfloat>
#include <time.h>
#include <climits>

#include "BoardReader.h"
#include "Riostream.h"
#include "TObject.h"
#include "DGlobalTools.h" // to have fTool has a data member
//using namespace std;

//##############################################################################
class BoardReaderIHEP : public TObject  {

private:

  int             fBoardNumber;
  int             fTriggerMode;
  int             fEventBuildingMode;
  int             fEventNumber;
  int             fCurrentTriggerNumber;
  BoardReaderEvent *fCurrentEvent;
  std::vector<BoardReaderPixel> ListOfPixels;
  std::vector<int>       ListOfTriggers;
  std::vector<int>       ListOfTimestamps;
  std::vector<int>       ListOfFrames;

  // management of event reading status
  bool            fVetoOverflow;   // control veto for event with overflow
  bool            fOverflow;       // flag to indicate overflow
  int             fEventsOverflow; // count total events with overflow
  int             fFramesReadFromFile; // count total # frames read

  int i_Trigger;
  int Trigger_Framcount[200000][10];
  int Trigger[6];

  int iFile;

  /* Verbosity : >3-Little information */
  /* 0/1     - Error, Status, Warning */
  /* 0/1/2   - Error, Status, Warning */
  /* 0/1/2/3 - Error, Warning */
  /* >3      - Error */
  int vi_Verbose; /* Recommend-4; Little-6; Less-10 */

  /* Stream to input Raw Data file */
  fstream ifs_DataRaw;
  unsigned long int vi_Pointer_FileBegin;
  unsigned long int vi_Pointer_FileEnd;
  unsigned long int vi_N_FileSize;

  /* Chip Parameters */
  unsigned long int N_COLUMN;
  unsigned long int N_ROW;
  unsigned long int N_BANK;
  unsigned long int N_BANKCOLUMN;

  /* Read binary data */
  unsigned long int BYTE; // 1Byte  = 8bits
  unsigned long int WORD; // 1Word  = 2Byte = 16bits (Mi28 default)
  unsigned long int DWORD; // 1DWord = 2Word = 32bits

  /* Marker for a Ladder */
  unsigned long int M_LADDER_HEADER;
  unsigned long int M_LADDER_CHIP; // Ladder Trailer Marker 32 bits : 0x3333333X, Check the Big 28 bits
  unsigned long int M_LADDER_TRAILER; // Ladder Trailer Marker 32 bits : 0xCCCCCCCC, Check all the 32 bits
  unsigned long int M_LADDER_TriggerHeader; // Ladder Trigger Header  Marker 32 bits : 0x55555555, Check all the 32 bits
  unsigned long int M_LADDER_TriggerTRAILER; // Ladder Trigger Trailer Marker 32 bits : 0x66666666, Check all the 32 bits

  /* Values for a Pack */
  unsigned long int vi_Trigger_Header;
  unsigned long int vi_Trigger_ID;
  unsigned long int vi_Pack_Length;
  unsigned long int vi_Pack_State;
  unsigned long int vi_Trigger_Trailer;

  /* Values for a Ladder */
  unsigned long int vi_Ladder_Header;
  unsigned long int vi_Ladder_Header_1;
  unsigned long int vi_Ladder_DataLength;
  unsigned long int vi_Ladder_Trigger;
  unsigned long int vi_Ladder_FrameCounter;
  unsigned long int vi_Ladder_Chip;
  unsigned long int vi_Ladder_Chip_1;
  unsigned long int vi_Ladder_Trailer;
  unsigned long int vi_ID_Ladder;
  unsigned long int vi_ID_Ladder_Chip;
  unsigned long int vi_N_Ladder_Chip;

  /* ------------------------------------------------------ */
  /* Marker for a Chip */
  /* const value for data length */
  //const int CHIP_LENGTH00 = 918  ; /* Mode00,  80MHz , DO1     , 459  + 459  = 918  */
  //const int CHIP_LENGTH01 = 1836 ; /* Mode01,  80MHz , DO0 DO1 , 918  + 918  = 1836 */
  //const int CHIP_LENGTH10 = 1844 ; /* Mode10, 160MHz , DO1     , 922  + 922  = 1844 */
  //const int CHIP_LENGTH11 = 3700 ; /* Mode11, 160MHz , DO0 DO1 , 1850 + 1850 = 3700 */

  /* Header and Trailer */
  unsigned long int M_CHIP_HEADER; // hex=1234 5678
  unsigned long int M_CHIP_TRAILER; // hex=aaaa aaaa

  /* Signification of the bits in the Status/Line word */
  unsigned long int M_CHIP_N_STATE; // Status/Line, 0-3, number of States
  unsigned long int M_CHIP_ADDRESS_LINE; // Status/Line, 4-13, address of the line
  unsigned long int M_CHIP_FLAG_RESIDUAL_1; // Status/Line, 14, 1 bit zero
  unsigned long int M_CHIP_FLAG_OVERFLOW; // Status/Line, 15, overflow

  /* Signification of the bits in the State word */
  unsigned long int M_CHIP_N_PIXEL; // State, 0-1, number of Pixels
  unsigned long int M_CHIP_ADDRESS_COLUMN; // State, 2-11, address of the column
  unsigned long int M_CHIP_FLAG_RESIDUAL_2; // State, 12-15, 4 bits zero

  /* Values for a frame */
  unsigned long int vi_DataRaw;
  unsigned long int vi_Chip_Header;
  unsigned long int vi_Chip_FrameCounter;
  unsigned long int vi_Chip_DataLength;
  unsigned long int vi_Chip_DataLength_1;
  unsigned long int vi_Chip_DataLength_2;
  unsigned long int vi_Chip_Status;
  unsigned long int vi_Chip_N_State;
  unsigned long int vi_Chip_Address_Line;
  unsigned long int vi_Chip_Flag_Residual_1;
  unsigned long int vi_Chip_Flag_OverFlow;
  unsigned long int vi_Chip_State;
  unsigned long int vi_Chip_N_Pixel;
  unsigned long int vi_Chip_Address_Column;
  unsigned long int vi_Chip_Flag_Residual_2;
  unsigned long int vi_Chip_Trailer;
  unsigned long int vi_Column_Temp;

  /* Pointers */
  unsigned long int vi_Pointer_Data;
  unsigned long int vi_Pointer_Pack_DataLength;
  unsigned long int vi_Pointer_Ladder_DataLength;
  unsigned long int vi_Pointer_Chip_DataLength;

  unsigned long int vi_N_Frame_Good;
  unsigned long int vi_N_Frame_Bad;

  double vd_N_PixelTotal;
  double vd_N_PixelBankA;
  double vd_N_PixelBankB;
  double vd_N_PixelBankC;
  double vd_N_PixelBankD;

  bool  DecodeNextEvent();
  void  AddPixel( int iSensor, int value, int aLine, int aColumn, int aTime=0);
  unsigned long int SwitchWordBytes(unsigned long int fi_Word);
  unsigned long int SwitchDWordBytes(unsigned long int fi_DWord);
  unsigned long int SwitchDWordBytes2(unsigned long int fi_DWord);
  unsigned long int SwitchDWordWords(unsigned long int fi_DWord);


public:

  BoardReaderIHEP(int boardNumber, int triggerMode=0, int eventBuildingMode=0);
  ~BoardReaderIHEP();

  void  SetDebugLevel( int aLevel) { vi_Verbose = abs(12-aLevel*2); cout << "IHEP board debug set to " << vi_Verbose << endl; }
  bool  AddFile(const char *inputFileName);
  bool  HasData();
  int   GetBoardNumber() { return fBoardNumber; }
  int   GetEventNumber() { return fEventNumber; }
  BoardReaderEvent*   GetEvent() { return fCurrentEvent; }
  void  PrintEventHeader();
  void  PrintStatistics(ostream &stream);


  ClassDef(BoardReaderIHEP,0)
};

#endif
