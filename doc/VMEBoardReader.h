#ifndef _VMEBoardReader_HXX
#define _VMEBoardReader_HXX
/*!
  \file
  \version $Id: VMEBoardReader.hxx,v 1.8 2003/07/08 18:54:19 mueller Exp $
  \brief   Declaration of VMEBoardReader.
*/
/*------------------------------------------+---------------------------------*/

#include "BoardReader.h" // withROOT is define therein
#include <fstream>
#include <iostream>
#include <vector>
//using namespace std;
#include "TH2F.h"
class TH2F;
class TH1F;

#include "mi26types.h"

#define withROOT

//##############################################################################
class VMEBoardReader : public TObject  {
    
public:

  VMEBoardReader(int boardNumber, const char* pathName, const char* prefixName, const char* suffixName, int runNumber, int numberOfSensors, int numberOfRows);
  ~VMEBoardReader();

  void  SetDebugLevel( int aLevel) { fDebugLevel = aLevel; }
  void  SetDisplayOn() { fDisplay = true; CreateHistogram(); }
  bool  HasData();
  bool  ValidHistogram() { return fDisplay; }
  int   GetBoardNumber() { return fBoardNumber; }
  int   GetEventNumber() { return fEventNumber; }
  BoardReaderEvent*   GetEvent() { return fCurrentEvent; }
  void  PrintStatistics(ostream &stream);

  
private:

  int             fDebugLevel;       // debug level
  int             fBoardNumber;
  int             fRunNumber;
  int             fNumberOfSensors;
  int             fNumberOfRows;
  int             fNumberOfColumns;
  const char*           fPrefixName;       // prefix file name
  const char*           fSuffixName;       // suffix file name
  const char*           fPathName;         // base file name
  FILE*           fRawFileFAdc[10];  // file streamm
  
  unsigned int   *fData;             // data array to fill
  int             fIndex;            // index of data array
  int             fEventSize;        // size of the event
  
  bool            fDisplay;          // decide histo display or not
  
  unsigned int    fCurrentTriggerCnt;
  int             fEventNumber;
  
  BoardReaderEvent *fCurrentEvent;
  vector<BoardReaderPixel> ListOfPixels;
  vector<int>       ListOfTriggers;
  vector<int>       ListOfFrames;
  vector<int>       ListOfLineOverflow;

  bool            fVetoOverflow;   // control veto for event with overflow
  bool            fOverflow;       // flag to indicate overflow
  int             fEventsOverflow; // count total events with overflow
  int             fNStatesInLine;  // count total # states
  int             fFramesReadFromFile; // count total # frames read
     
  static const unsigned int  fgKeyHeader[];
  static const int   fgkHeaderSize;
  static const unsigned int  fgkTailHeader;

//  typedef struct {
//	  
//	  unsigned int Header;
//	  unsigned int TriggerCnt;
//	  unsigned int StopIdx;
//	  unsigned int TimeStamp;
//	  
//  } FAdcHeader; 
//  
//  FAdcHeader* fFAdcHeader;
  
//  typedef struct {
//    unsigned int Header;
//    unsigned int TriggerCnt;
//    unsigned int TriggerLine; // ? not sure
//    unsigned int FrameCnt;
//    unsigned int DataLength;
//    unsigned int ADataW16[140]; //data MI26
//    unsigned int Trailer;
//
//  } MI26_FrameRaw;
//  
  unsigned int *fDataBackup; // needed to get previous reading
  int           fBackupIndex; // index of backup data
  int          *fBackupSize; // nb of words back-up per sensor
  
//  typedef union {
//    
//    unsigned int W16;
//    
//    struct {
//      
//      unsigned int StateNb  :  4;
//      unsigned int LineAddr : 11;
//      unsigned int Ovf      :  1;
//      
//    } F;
//    
//  } MI26__TStatesLine;
//
  
//  typedef union {
//    
//    unsigned int W16;
//    
//    struct {
//      
//      unsigned int HitNb   :  2;
//      unsigned int ColAddr : 11;
//      unsigned int NotUsed :  3;
//      
//    } F;
//    
//  } MI26__TState;
//    
  
  int   Open();
  void    CreateHistogram();
  
#ifdef withROOT  
   TH2F*             fpHisPixelMap[8];  // pixel map per sensor
   TH1F*             fpHisRateMap[8];   // pixel map per sensor
   TH1F*             fpHisRateMapQ[8];  // pixel map per sensor quadrant
   TH1F*             fpHisEvtLength[8]; // event data length for each
#endif

  static  unsigned int GetKeyHeader(int idx) { return fgKeyHeader[idx]; }
   static  unsigned int GetTailHeader()         { return fgkTailHeader;    }
   static  int GetHeaderSize()          { return fgkHeaderSize;    }
   
 
   //! Build event
   bool FetchEvent();
	
   //! Get the starting point of each frame
   bool GetStart(int iSensor);
   
   //! Get frame and returns frameRaw
  void   GetFrame(int iSensor, MI26_FrameRaw* data);
  void   GetBackupData(int iSensor, MI26_FrameRaw* data);
   
   //! Get next frames with same trigger
   void   GetNextFrames(int iSensor, unsigned int trigger, unsigned int *dataBackup, int &backupIndex);
	  
   //! Add pixel to list
   void  AddPixel( int input, int value, int aLine, int aColumn);
   
   //! Get Sensor number
   int GetSensor(unsigned int key);
   
   //! Check trigger counts
   bool CheckTriggerCnt(unsigned int trig);
   
   //! decode frame
   bool DecodeFrame();
   
   ClassDef(VMEBoardReader,0)
};

#endif
