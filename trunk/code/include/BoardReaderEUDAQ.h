#ifndef _BoardReaderEUDAQ_included_
#define _BoardReaderEUDAQ_included_

#include "BoardReader.h"
#include "Riostream.h"
#include "TObject.h"
#include <fstream>
#include <vector>
#include "DGlobalTools.h" // to have fTool has a data member
//using namespace std;

//##############################################################################
class BoardReaderEUDAQ : public TObject  {
    
private:

  int             fDebugLevel;       // debug level

  int             fBoardNumber;
  int             fRunNumber;
  int             fTriggerMode;
  int             fEventBuildingMode;

  int             fSensorType;
  int             fNumberOfSensors;
  int             fNumberOfRows;
  int             fNumberOfColumns;

  
  // management of input files
  ifstream          fRawFileStream;
  char             *fInputFileName;
  std::vector<char*>           fListOfInputFileNames;
  std::vector<char*>::iterator fCurrentInputFileName;
  int               fCurrentFileNumber;
  int               fNumberOfFiles;

  
  // management of data buffer
  unsigned int   *fData;             // data array to fill
  int             fIndex;            // index of data array
  int             fEventSize;        // size of the event = header + data + trailer
  int             fEventHeaderSize;
  int             fEventDataSize;
  int             fEventTrailerSize;
  int             fFiletHeaderSize;
  int             fBufferRead;
  
  
  // stored information for each event
  unsigned int    fCurrentTriggerCnt;
  int             fEventNumber;
  
  BoardReaderEvent *fCurrentEvent;
  vector<BoardReaderPixel> ListOfPixels;
  vector<int>       ListOfTriggers;
  vector<int>       ListOfFrames;
  vector<int>       ListOfLineOverflow;

  
  // management of event reading status
  bool            fVetoOverflow;   // control veto for event with overflow
  bool            fOverflow;       // flag to indicate overflow
  int             fEventsOverflow; // count total events with overflow
  int             fNStatesInLine;  // count total # states
  int             fFramesReadFromFile; // count total # frames read
     

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
  
  
  bool  CloseRawFile();
  bool  OpenRawFile(const char *fileName);
  bool  LookUpRawFile();
  bool  FetchEvent();
	
   //! Get the starting point of each frame
   bool GetStart(int iSensor);
   
   //! Get frame and returns frameRaw
  void   GetFrame(int iSensor, MI26_FrameRaw* data);
  void   GetBackupData(int iSensor, MI26_FrameRaw* data);
   
   void   GetNextFrames(int iSensor, unsigned int trigger, unsigned int *dataBackup, int &backupIndex)
   void  AddPixel( int input, int value, int aLine, int aColumn);
   
   //! Get Sensor number
   int GetSensor(unsigned int key);
   
   //! Check trigger counts
   bool CheckTriggerCnt(unsigned int trig);
   
   //! decode frame
   bool DecodeFrame();

   
public:

  BoardReaderEUDAQ(int boardNumber, int runNumber, int fSensorType, int numberOfSensors, int triggerMode, int eventBuildingMode=0);
  ~BoardReaderEUDAQ();

  void  SetDebugLevel( int aLevel) { fDebugLevel = aLevel; }
  bool  AddFile(const char *inputFileName);
  void  AddFileList(const char *prefixFileName, int firstIndex, int endIndex, const char *suffixFileName);
  void  OpenFile();
  void  CloseFile();
  bool  HasData();
  int   GetBoardNumber() { return fBoardNumber; }
  int   GetEventNumber() { return fEventNumber; }
  BoardReaderEvent*   GetEvent() { return fCurrentEvent; }
  void  PrintEventHeader();
  void  PrintStatistics(ostream &stream);

  
  ClassDef(BoardReaderEUDAQ,0)
};

#endif
