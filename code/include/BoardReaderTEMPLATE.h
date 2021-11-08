#ifndef _BoardReaderTEMPLATE_included_
#define _BoardReaderTEMPLATE_included_

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

#include "TH1.h"
#include "TH1F.h"

#include "TTree.h"
#include "TFile.h"

#include "BoardReader.h"
#include "Riostream.h"
#include "TObject.h"
#include "DGlobalTools.h" // to have fTool has a data member

//#include "mimo_daq_lib/mimo_daq_lib.h"
//#include "mimo_daq_lib/mimo_daq_lib.c"

//using namespace std;

//##############################################################################
class BoardReaderTEMPLATE : public TObject  {

private:

  int             fDebugLevel;
  DGlobalTools    fTool;

  int             fBoardNumber;
  int             fRunNumber;
  int             fNSensors;
  int             fTriggerMode;
  int             fEventBuildingMode;
  bool            fVetoOverflow;
  int             fEndianness;       // 0= do not swap bytes, 1= swap bytes

  std::ifstream   fRawFileStream;
  std::vector<string> fListInputFileNames;
  size_t          fCurrentFileNumber;
  bool            fNoMoreFile;
  bool            fisfirstAcq;
  int             fnbFrPerAcq;

  int             fCurrentTriggerNumber;
  int             fCurrentEventNumber;

  int             fReachEndOfRun;

  int             fCurrentAcqNumber;
  int             fCurrentFrameNumber;
  int             fTriggerCount;
  int             fFrameCount;
  int             fBadDecFrameCounter ; // ZE 2021/06/04 - Counter for bad decoded frames
  int             fNEventsWithOverflow;
  BoardReaderEvent   *fCurrentEvent;

  std::vector<BoardReaderPixel> fListOfPixels;
  std::vector<int>       fListOfTriggers;
  std::vector<int>       fListOfTimestamps;
  std::vector<int>       fListOfFrames;

  std::vector<int>       fListOfTriggerPos; //JB 2010/06/16
  std::vector<int>       fListOfNextTriggerPos; //JB 2011/07/18
  std::vector<int>       *fListOfLineOverflow; // MG 2012/02/15
  std::vector<int>       fListOfNextTimestamps; // JB 2012/05/04


  bool  DecodeNextEvent();
  bool  DecodeFrame();
//  bool  DecodeFrame(MIS1__BT_FBtAcqW16AAlloc*, MIS1__TBtAcqRawRec*, int moduleID, int frameID,
//  UInt8 MeasExecTime, UInt8 PrintLvl); // ZE 2021/06/02
  void  AddPixel( int iSensor, int value, int aLine, int aColumn, int aTime=0);


public:

    int test();
    BoardReaderTEMPLATE(int boardNumber, char *dataPath, int runNumber, int nSensors=1, int triggerMode=0, int eventBuildingMode=0, int endianness=0);
  ~BoardReaderTEMPLATE();

  void  SetDebugLevel( int aLevel) { fDebugLevel = aLevel; cout << "BoardReaderTEMPLATE " << fBoardNumber << " debug updated to " << fDebugLevel << endl; }
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
