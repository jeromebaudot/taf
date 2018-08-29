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
  int             fRunNumber;
  int             fNumberOfSensors;
  int             fTriggerMode;
  int             fEventBuildingMode;
  int             fEventNumber;
  BoardReaderEvent *fCurrentEvent;
  
  // management of event reading status
  bool            fVetoOverflow;   // control veto for event with overflow
  bool            fOverflow;       // flag to indicate overflow
  int             fEventsOverflow; // count total events with overflow
  int             fFramesReadFromFile; // count total # frames read
     
  
  /* Verbosity : >3-Little information */
  /* 0/1     - Error, Status, Warning */
  /* 0/1/2   - Error, Status, Warning */
  /* 0/1/2/3 - Error, Warning */
  /* >3      - Error */
  int vi_Verbose; /* Recommend-4; Little-6; Less-10 */
  int fDebugLevel;
  
  /* Stream to input Raw Data file */
  fstream ifs_DataRaw;
  unsigned long int vi_Pointer_FileBegin;
  unsigned long int vi_Pointer_FileEnd;
  unsigned long int vi_N_FileSize;
  char*             PrefixFileName;
  char*             SuffixFileName;
  int               fCurrentFileNumber;
  int               fNumberOfFiles;
  bool              NoMoreFile;

#if 0

  /* Chip Parameters */
//  const unsigned long int N_COLUMN     = 960;
//  const unsigned long int N_ROW        = 928;
//  const unsigned long int N_BANK       = 4;
//  const unsigned long int N_BANKCOLUMN = N_COLUMN / N_BANK;
  
  /* Read binary data */
//  const unsigned long int BYTE  = 1; // 1Byte  = 8bits
//  const unsigned long int WORD  = 2; // 1Word  = 2Byte = 16bits (Mi28 default)
//  const unsigned long int DWORD = 4; // 1DWord = 2Word = 32bits
  
  /* ------------------------------------------------------ */
  /* Marker for a Ladder */
//  const unsigned long int M_LADDER_HEADER  = 0xCCCCCC   ; /* Ladder Header  Marker 32 bits : 0xCCCCCCXX, Check the Big 24 bits */
//  const unsigned long int M_LADDER_CHIP    = 0x3333333  ; /* Ladder Trailer Marker 32 bits : 0x3333333X, Check the Big 28 bits */
//  const unsigned long int M_LADDER_TRAILER = 0x99999999 ; /* Ladder Trailer Marker 32 bits : 0xCCCCCCCC, Check all the 32 bits */
  
  /* Values for a Ladder */
  unsigned long int vi_Ladder_Header       = 0;
  unsigned long int vi_Ladder_Header_1     = 0;
  unsigned long int vi_Ladder_DataLength   = 0;
  unsigned long int vi_Ladder_FrameCounter = 0;
  unsigned long int vi_Ladder_Chip         = 0;
  unsigned long int vi_Ladder_Chip_1       = 0;
  unsigned long int vi_Ladder_Trailer      = 0;
  unsigned long int vi_ID_Ladder           = 0;
  unsigned long int vi_ID_Ladder_Chip      = 0;
  unsigned long int vi_N_Ladder_Chip       = 0;
  
  /* ------------------------------------------------------ */
  /* Marker for a Chip */
  /* const value for data length */
  //const int CHIP_LENGTH00 = 918  ; /* Mode00,  80MHz , DO1     , 459  + 459  = 918  */
  //const int CHIP_LENGTH01 = 1836 ; /* Mode01,  80MHz , DO0 DO1 , 918  + 918  = 1836 */
  //const int CHIP_LENGTH10 = 1844 ; /* Mode10, 160MHz , DO1     , 922  + 922  = 1844 */
  //const int CHIP_LENGTH11 = 3700 ; /* Mode11, 160MHz , DO0 DO1 , 1850 + 1850 = 3700 */
  
  /* Header and Trailer */
  const unsigned long int M_CHIP_HEADER  = 0x12345678; // hex=1234 5678
  const unsigned long int M_CHIP_TRAILER = 0xaaaaaaaa; // hex=aaaa aaaa
  
  /* Signification of the bits in the Status/Line word */
  const unsigned long int M_CHIP_N_STATE         = 15    ; // Status/Line, 0-3, number of States
  const unsigned long int M_CHIP_ADDRESS_LINE    = 16368 ; // Status/Line, 4-13, address of the line
  const unsigned long int M_CHIP_FLAG_RESIDUAL_1 = 16384 ; // Status/Line, 14, 1 bit zero
  const unsigned long int M_CHIP_FLAG_OVERFLOW   = 32768 ; // Status/Line, 15, overflow
  
  /* Signification of the bits in the State word */
  const unsigned long int M_CHIP_N_PIXEL         = 3     ; // State, 0-1, number of Pixels
  const unsigned long int M_CHIP_ADDRESS_COLUMN  = 4092  ; // State, 2-11, address of the column
  const unsigned long int M_CHIP_FLAG_RESIDUAL_2 = 61440 ; // State, 12-15, 4 bits zero
  
  /* Values for a frame */
  unsigned long int vi_DataRaw              = 0;
  unsigned long int vi_Chip_Header          = 0;
  unsigned long int vi_Chip_FrameCounter    = 0;
  unsigned long int vi_Chip_DataLength      = 0;
  unsigned long int vi_Chip_DataLength_1    = 0;
  unsigned long int vi_Chip_DataLength_2    = 0;
  unsigned long int vi_Chip_Status          = 0;
  unsigned long int vi_Chip_N_State         = 0;
  unsigned long int vi_Chip_Address_Line    = 0;
  unsigned long int vi_Chip_Flag_Residual_1 = 0;
  unsigned long int vi_Chip_Flag_OverFlow   = 0;
  unsigned long int vi_Chip_State           = 0;
  unsigned long int vi_Chip_N_Pixel         = 0;
  unsigned long int vi_Chip_Address_Column  = 0;
  unsigned long int vi_Chip_Flag_Residual_2 = 0;
  unsigned long int vi_Chip_Trailer         = 0;
  unsigned long int vi_Column_Temp          = 0;
  
  /* Pointers */
  unsigned long int vi_Pointer_Data              = 0;
  unsigned long int vi_Pointer_Ladder_DataLength = 0;
  unsigned long int vi_Pointer_Chip_DataLength   = 0;
  
  unsigned long int vi_N_Frame_Good = 0;
  unsigned long int vi_N_Frame_Bad  = 0;
  
  double vd_N_PixelTotal = 0;
  double vd_N_PixelBankA = 0;
  double vd_N_PixelBankB = 0;
  double vd_N_PixelBankC = 0;
  double vd_N_PixelBankD = 0;

#endif
  
  bool  LookUpRawFile();
  bool  GetNextEvent();
  bool  DecodeFrame();
  void  AddPixel( int iSensor, int value, int aLine, int aColumn);
  unsigned long int SwitchWordBytes(unsigned long int fi_Word);
  unsigned long int SwitchDWordBytes(unsigned long int fi_DWord);
  unsigned long int SwitchDWordBytes2(unsigned long int fi_DWord);
  unsigned long int SwitchDWordWords(unsigned long int fi_DWord);

   
public:

  BoardReaderIHEP(int boardNumber, int runNumber, int numberOfSensors, int triggerMode=0, int eventBuildingMode=0);
  ~BoardReaderIHEP();

  void  SetDebugLevel( int aLevel) { vi_Verbose = aLevel; }
  bool  AddFile(const char *inputFileName);
//  void  AddFileList(const char *prefixFileName, int firstIndex, int endIndex, const char *suffixFileName);
  bool  HasData();
  int   GetBoardNumber() { return fBoardNumber; }
  int   GetEventNumber() { return fEventNumber; }
  BoardReaderEvent*   GetEvent() { return fCurrentEvent; }
  void  PrintEventHeader();
  void  PrintStatistics(ostream &stream);

  
  ClassDef(BoardReaderIHEP,0)
};

#endif
