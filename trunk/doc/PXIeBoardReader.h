#ifndef _PXIeBoardReader_included_
#define _PXIeBoardReader_included_


#include "Riostream.h"
#include "TObject.h"
#include "TH1.h"
#include "TH1F.h"
#include "TTree.h"
#include "TFile.h"
//#include <iostream>
#include <fstream>
#include <vector>
#include "DGlobalTools.h" // to have fTool has a data member
//using namespace std;

// --------------------------------------------------------------------------------------

class PXIePixel : public TObject {

  // Container for a simple pixel
  // JB, 2009/08/13

 private:

  int Input;
  int Value;
  int LineNumber;
  int ColumnNumber;

 public:

  PXIePixel() {  Input = 0; Value = 0; LineNumber = 0; ColumnNumber = 0;}
  PXIePixel( int input, int value, int line, int col) {  Input = input; Value = value; LineNumber = line; ColumnNumber = col;}
  virtual ~PXIePixel() {;}
  int      GetInput()                 { return Input; }
  int      GetValue()                 { return Value; }
  int      GetLineNumber()            { return LineNumber; }
  int      GetColumnNumber()          { return ColumnNumber; }

  ClassDef(PXIePixel,1)
};

// --------------------------------------------------------------------------------------

class PXIeEvent : public TObject {

 private:

  int  EventNumber;
  int  BoardNumber;  
  int  RunNumber;
  vector<PXIePixel> *ListOfPixels;
  vector<int>       *ListOfTriggerPos; //JB 2010/06/16
  vector<int>       *ListOfFrames; //JB 2010/06/16
  vector<int>       *ListOfLineOverflow; /// MG 2012/02/15
  vector<int>  *ListOfTimestamps; // JB 2012/05/04

 public:

  PXIeEvent() {;}
///  PXIeEvent( int EventNumber, int boardNumber, int runNumber, vector<PXIePixel> *ListOfPixels, vector<int> *ListOfTriggerPos, vector<int> *ListOfFrames);
  PXIeEvent( int EventNumber, int boardNumber, int runNumber, vector<PXIePixel> *ListOfPixels, vector<int> *ListOfTriggerPos, vector<int> *ListOfFrames, vector<int> *ListOfTimestamps, vector<int> *ListOfLineOverflow); //MG 2012/03/14, JB 2012/05/04
 ~PXIeEvent();

  int                GetEventNumber()             { return EventNumber; }
  int                GetBoardNumber()             { return BoardNumber; }
  int                GetRunNumber()               { return RunNumber; }
  int                GetNumberOfPixels()          { return ListOfPixels->size(); }
  vector<PXIePixel> *GetPixels()                  { return ListOfPixels;}
  PXIePixel         *GetPixelAt( int index)       { return &(ListOfPixels->at(index)); }
  int                GetNumberOfFrames()          { return ListOfFrames->size(); } //JB 2010/06/16
  vector<int>       *GetFrames()                  { return ListOfFrames;} //JB 2010/06/16
  int                GetNumberOfTriggers()        { return ListOfTriggerPos->size(); } //JB 2010/06/16
  vector<int>       *GetTriggers()                { return ListOfTriggerPos;} //JB 2010/06/16
  vector<int>       *GetLineOverflow()            { return ListOfLineOverflow;} // MG 2012/02/15
  int                GetNumberOfLineOverflow ( int module ) { return ListOfLineOverflow[module].size();} // MG 2012/02/15
  int               GetNumberOfTimestamps()       { return ListOfTimestamps->size(); }// JB 2012/05/04
  int          GetTimestampAt( int index)    { return ListOfTimestamps->at(index);}
  vector<int> *GetTimestamps()               { return ListOfTimestamps;}

  ClassDef(PXIeEvent,2)
};

// --------------------------------------------------------------------------------------

class PXIeBoardReader : public TObject {

 private:

  int               DebugLevel;
  DGlobalTools      fTool;                       // JB 2011/07/18

  int               BoardNumber;
  int               NSensors;
  int               TriggerMode;
  int               EventBuildingMode;
  int               FramesToSkipAtEndOfAcqForTrig; // JB 2011/06/30
  int		            TriggerLineOffset;
  int		            FramesPostTrigger;
  int               NextFirstLineToKeep;
  int               NextFirstFrame;
  int               NextLastLineToKeep; // JB 2011/03/14
  int               FirstLineToKeep;
  int               FirstFrame;
  int               LastLineToKeep;
  int               LastFrame;
  int               NumberOfLines;
  int               RunNumber;
  int               EventsPerFile;
  int               FramesReadFromFile;
  int               AcqsReadFromFile; // JB 2011/03/14
  int               NFramesPerAcq; // JB 2011/03/14
  int               Nacquisitions; // JB 2011/03/14
  int               PeriodsVetoOff; // JB 2012/02/23
  int               OriginAcq; // JB 2012/05/04
  int               RunTimeInSec; // JB 2012/05/12
  
  vector<int>       ListOfDUTSensors; // JB 2014/02/12
  
  bool              ReadingEvent;
  bool              ReadTwice;
  bool              Overflow; // JB 2009/09/08
  bool              VetoOverflow; // JB 2010/07/06
  bool              VetoEvent; // JB 2012/02/23
  bool              EventReady;
  bool              StopIfRequiredFrameNotFound; // JB 2012/09/04
  bool              NoMoreData; // JB 2012/09/04
  PXIeEvent         *CurrentEvent;
  int               CurrentEventNumber;
  vector<PXIePixel> ListOfPixels;
  vector<int>       ListOfTriggerPos; //JB 2010/06/16
  vector<int>       ListOfNextTriggerPos; //JB 2011/07/18
  vector<int>       ListOfFrames; //JB 2010/06/16
  vector<int>       *ListOfLineOverflow; // MG 2012/02/15
  vector<int>       ListOfTimestamps; // JB 2012/05/04
  vector<int>       ListOfNextTimestamps; // JB 2012/05/04

  int               EventsCount; // JB 2009/09/09
  int               EventsOverflow; // JB 2009/09/09
  int               EventsAborted; // JB 2009/09/16
  int               EventsMissed; // JB 2012/09/04
  int               EventsWithBadTrigger; // JB 2012/09/05
  int               FramesReadTwice; // JB 2009/10/14
  int               TriggerCount; // JB 2011/06/18
  int               TriggerInvalidCount; // JB 2012/09/25
  int               NStatesInBlock[100]; // up to 100 blocks
  int               NStatesInLine; // up to 100 blocks
  int		   *overflowSensor; // JB 2011/11/02
  TH1S             *h1BlockOccupancy; // JB 2009/09/10
  TH1S             *h1LineOccupancy; // JB 2009/09/10
  TH1F             **hLineOverFlowDist; //AP 2014/06/03

  ifstream          RawFileStream;
  char             *InputFileName;
  char*             PrefixFileName;
  char*             SuffixFileName;
  
  FILE             *ConfigFile;
  char              ConfigFileName[300];
  int               CurrentFileNumber;
  int               NumberOfFiles;

  size_t            SizeOfDaqEvent;
  unsigned int     *Data;
  int               Endianness;       // 0= do not swap bytes, 1= swap bytes

  struct header{
    int    triggerNb;
    int    frameNb;
    int    acqId;
    int    frameId;
    int    triggersInFrame;
    int    triggerPos;
    int    timestamp;
  };
  TTree            *treeHeaders;
  TFile            *fileHeaders;

  void    AddPixel( int input, int value, int aLine, int aColumn);
  int     ComputeTimestamp( int aAcqId, int aFrameId, int aLineNumber);
  bool    OpenFileHeaders();
  
  public:

  int          test(char *path, int runNumber);
  PXIeBoardReader() {;}
  PXIeBoardReader( int boardNumber, char *configFileName, int runNumber, int triggerMode, int eventBuildingMode=0, bool vetoOverflow=true, int endian=0, int numberOfRows=576);
  ~PXIeBoardReader();
  
  bool         ReadConfiguration();
  void         SetDebugLevel( int level)              { DebugLevel = level; cout << "PXIeBoardReader " << BoardNumber << " debug updated to " << DebugLevel << endl;}
  void         SetEndiannes( int endian)              { Endianness = endian; }
  void         AddFileList(char *prefixFileName, int firstIndex, int endIndex, char *suffixFileName);
  //bool         OpenNextFile();
  void         AddDUTSensor( int aSensorId); // JB 2014/02/12

  bool         GetNextDaqEvent();
  bool         ResetDaqEvent( int newAcqNo, int newFrNo); // JB 2012/07/10
  //bool         DecodeFrame(int iSensor); // obsolete, replaced by the 5 following methods
  bool         ReadTriggersFromFrame(); // JB 2011/03/14
  bool         CheckPixelToAdd( Int_t aSensorId, Int_t aFrame, Int_t aLine, Int_t aColumn); // JB 2014/02/11
  bool         IsDUTSensor( int aSensorId); // JB 2014/02/12
  bool         CheckTriggerValidity(); // JB 2012/07/10
  bool         GetTriggerList(); // JB 2012/09/25
  void	       StartEventReading(); // JB 2011/03/14
  void         DefineEventEnd(); // JB 2011/03/14
  void         CheckSecondReading(); // JB 2011/03/14
  bool         ReadPixelsFromFrame(); // JB 2011/03/14
  bool         HasData();
  bool         HasData( int acqNo, int frNo); // JB 2012/07/10
  bool         HasData( int aTriggerNb); // JB 2012/07/10
  void         SkipNextEvent();
  int          GetBoardNumber()                       { return BoardNumber; }
  int          GetNSensors()                          { return NSensors; }
  int          GetTriggerMode()                       { return TriggerMode; }
  int          GetRunNumber()                         { return RunNumber; }
  PXIeEvent*   GetEvent()                             { return CurrentEvent; }
  int          GetEventNumber()                       { return CurrentEventNumber;}
  char*        GetConfigFileName()                    { return ConfigFileName;}
  char*        GetInputFileName()                     { return InputFileName;}
  char*        GetSuffixFileName()                    { return SuffixFileName;}
  char*        GetPrefixFileName()                    { return PrefixFileName;}
  void         Close();
  void         PrintStatistics(ostream &stream=cout); // JB 2010/08/26 // SS 2011/12/14
  void         SetVetoPixel( int noiseRun); // JB 2012/03/11
  void         DumpEventHeaders( int nFrames); // JB 2012/05/03
  
  unsigned int SwapEndian( unsigned int data);

  ClassDef(PXIeBoardReader,3);  
};

# endif
