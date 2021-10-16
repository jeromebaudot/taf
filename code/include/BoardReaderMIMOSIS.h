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
class BoardReaderMIMOSIS : public TObject  {

private:

  int             fDebugLevel;
  DGlobalTools    fTool;

  int             fBoardNumber;
  int             fRunNumber;
  int             fNSensors;
  int             fTriggerMode;
  int             fTriggerOffset;
  int             fFramesPerTrigger;
  int             fEventBuildingMode;
  bool            fVetoOverflow;
  int             fEndianness;       // 0= do not swap bytes, 1= swap bytes

  std::ifstream   fRawFileStream;
  std::vector<string> fListInputFileNames;
  size_t          fCurrentFileNumber;
  bool            fNoMoreFile;
  bool            fisfirstAcq;
  bool            fisAcqSafe;     // Boolean variable to test whether the acquisition is safe (i.e not truncated)
                                   // Acquisition is truncated if the number of frames/Acq is not the same
                                   // in all the sensors for a giving acquisition.
                                   
  int             fnbFrPerAcq;
  int             fnbTrg;
  int             fendTrg;

  int             fCurrentTriggerNumber;
  int             fFramesCounterInTrigger;
  int             fCurrentEventNumber;
  
  int             fReachEndOfRun;
  
  int             fCurrentAcqNumber;
  int             fCurrentFrameNumber;
  int             fTriggerCount;
  int             fFrameCount;
  int             fBadDecFrameCounter ; // ZE 2021/06/04 - Counter for bad decoded frames
  int             fTruncatedAcqCounter ; // ZE 2021/06/09 - Counter for Truncated Acquistions (risky)
  int             *NbFiredPixPerFrame; // ZE 2021/10/10 - Number of fired pixels per frame "cumulated"
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
  void  DecodeFrame();
  bool  isAcqSafe();
    
  
//  bool  DecodeFrame(MIS1__BT_FBtAcqW16AAlloc*, MIS1__TBtAcqRawRec*, int moduleID, int frameID,
//  UInt8 MeasExecTime, UInt8 PrintLvl); // ZE 2021/06/02
  void  AddPixel( int iSensor, int value, int aLine, int aColumn, int aTime=0);


public:

    int test();
    BoardReaderMIMOSIS(int boardNumber, char *dataPath, int runNumber, int nSensors=1, int triggerMode=0, int triggeroffset=0, int framesPerTrigger = 1, int eventBuildingMode=0, int endianness=0);
  ~BoardReaderMIMOSIS();

  void  SetDebugLevel( int aLevel) { fDebugLevel = aLevel; cout << "BoardReaderMIMOSIS " << fBoardNumber << " debug updated to " << fDebugLevel << endl; }
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
