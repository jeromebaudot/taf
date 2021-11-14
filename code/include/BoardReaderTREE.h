#ifndef _BoardReaderTREE_included_
#define _BoardReaderTREE_included_

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
#include "CE65Event.h"
#include "Riostream.h"
#include "TObject.h"
#include "DGlobalTools.h" // to have fTool has a data member

//using namespace std;

//##############################################################################
class BoardReaderTREE : public TObject  {

private:

  int             fDebugLevel;
  DGlobalTools    fTool;

  int             fBoardNumber;
  int             fRunNumber;
  int             fNSensors;
  int             fNcolumns;
  int             fNrows;
  int             fTriggerMode;
  int             fEventBuildingMode;
  int             fNbOfBits;

  TFile          *fRawFile;
  TTree          *fRawTree;
  Long64_t        fNentries;

  Long64_t        fRawEventNumber;
  int             fFramesPerEvent;
  std::vector<ce65_frame> *fRawFrame = 0;

  int             fCurrentEventNumber;
  int             fCurrentTriggerNumber;
  int             fTriggerCount;
  int             fFrameCount;
  int             fBadEventCount;

  BoardReaderEvent   *fCurrentEvent;

  std::vector<BoardReaderPixel> fListOfPixels;
  std::vector<int>       fListOfTriggers;
  std::vector<int>       fListOfFrames;


  bool  DecodeNextEvent();
  bool  DecodeFrame();


public:

  int test();
  BoardReaderTREE(int boardNumber, int nSensors=1, int nColumns=64, int nRows=32, int triggerMode=0, int eventBuildingMode=0, int nBits=16);
  ~BoardReaderTREE();

  void  SetDebugLevel( int aLevel) { fDebugLevel = aLevel; cout << "BoardReaderTREE " << fBoardNumber << " debug updated to " << fDebugLevel << endl; }
  bool  AddFile(const char *inputFileName);
  bool  HasData();
  void  SkipNextEvent();
  int   GetBoardNumber() { return fBoardNumber; }
  int   GetNumberOfSensors() { return fNSensors; }
  int   GetTriggerMode() { return fTriggerMode; }
  int   GetEventBuildingMode() { return fEventBuildingMode; }
  int   GetEventNumber() { return fCurrentEventNumber; }
  int   GetNofBadEvents() { return fBadEventCount; }
  BoardReaderEvent*   GetEvent() { return fCurrentEvent; }
  void  PrintStatistics(ostream &stream);

};

#endif
