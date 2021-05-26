#ifndef _BoardReaderMIMOSIS_included_
#define _BoardReaderMIMOSIS_included_

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
class BoardReaderMIMOSIS : public TObject  {

private:

  int             fDebugLevel;
  DGlobalTools    fTool;

  int             fBoardNumber;
  int             fNSensors;
  int             fTriggerMode;
  int             fEventBuildingMode;
  bool            fVetoOverflow;
  int             fEndianness;       // 0= do not swap bytes, 1= swap bytes

  int             fSizeOfHeader;
  int             fSizeOfTrailer;
  std::ifstream   fRawFileStream;
  std::vector<string> fListInputFileNames;
  size_t          fCurrentFileNumber;
  bool            fNoMoreFile;

  int             fCurrentTriggerNumber;
  int             fCurrentEventNumber;
  int             fTriggerCount;
  int             fFrameCount;
  int             fNEventsWithOverflow;
  BoardReaderEvent *fCurrentEvent;
  std::vector<BoardReaderPixel> fListOfPixels;
  std::vector<int>       fListOfTriggers;
  std::vector<int>       fListOfTimestamps;
  std::vector<int>       fListOfFrames;


  bool  LookUpRawFile();
  bool  CloseRawFile();
  bool  OpenRawFile();
  bool  DecodeNextEvent();
  void  AddPixel( int iSensor, int value, int aLine, int aColumn, int aTime=0);

  // ==> PROBABLY MORE PRIVATE METHODS are needed <==

public:

    int test();
  BoardReaderMIMOSIS(int boardNumber, int nSensors=1, int triggerMode=0, int eventBuildingMode=0, int headerSize=0, int trailerSize=0, int endianness=0);
  ~BoardReaderMIMOSIS();

  void  SetDebugLevel( int aLevel) { fDebugLevel = aLevel; cout << "BoardReaderMIMOSIS " << fBoardNumber << " debug updated to " << fDebugLevel << endl; }
  bool  AddFileList(std::string prefixFileName, int startIndex, int endIndex, std::string suffixFileName);
  void  SetVetoPixel( int noiseRun=0);
  bool  HasData();
  void  SkipNextEvent();
  int   GetBoardNumber() { return fBoardNumber; }
  int   GetNumberOfSensors() { return fNSensors; }
  int   GetTriggerMode() { return fTriggerMode; }
  int   GetEventBuildingMode() { return fEventBuildingMode; }
  int   GetEventNumber() { return fCurrentEventNumber; }
  int   GetNofEventsWithOverflow() { return fNEventsWithOverflow; }
  BoardReaderEvent*   GetEvent() { return fCurrentEvent; }
  void  PrintEventHeader();
  void  PrintStatistics(ostream &stream);

};

#endif
