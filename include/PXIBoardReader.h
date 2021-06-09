#ifndef _PXIBoardReader_included_
#define _PXIBoardReader_included_

#include "Riostream.h"
#include "TObject.h"
#include "TH1.h"
//#include <iostream>
#include <fstream>
#include <vector>
#include "DGlobalTools.h" // to have fTool has a data member
using namespace std;


// --------------------------------------------------------------------------------------

class PXIPixel : public TObject {

  // Container for a simple pixel
  // JB, 2009/08/13

 private:

  int Input;
  int Value;
  int LineNumber;
  int ColumnNumber;

 public:

  PXIPixel() {  Input = 0; Value = 0; LineNumber = 0; ColumnNumber = 0;}
  PXIPixel( int input, int value, int line, int col) {  Input = input; Value = value; LineNumber = line; ColumnNumber = col;}
  virtual ~PXIPixel() {;}
  int      GetInput()                 { return Input; }
  int      GetValue()                 { return Value; }
  int      GetLineNumber()            { return LineNumber; }
  int      GetColumnNumber()          { return ColumnNumber; }

  ClassDef(PXIPixel,1)
};

// --------------------------------------------------------------------------------------

class PXIEvent : public TObject {

 private:

  int  EventNumber;
  int  BoardNumber;  
  int  RunNumber;
 std::vector<PXIPixel> *ListOfPixels;
 std::vector<int>      *ListOfTriggerPos; //JB 2010/06/16
 std::vector<int>      *ListOfFrames; //JB 2010/06/16

 public:

  PXIEvent() {;}
  PXIEvent( int EventNumber, int boardNumber, int runNumber, vector<PXIPixel> *ListOfPixels, vector<int> *ListOfTriggerPos, vector<int> *ListOfFrames);
 ~PXIEvent();

  int               GetEventNumber()             { return EventNumber; }
  int               GetBoardNumber()             { return BoardNumber; }
  int               GetRunNumber()               { return RunNumber; }
  int               GetNumberOfPixels()          { return ListOfPixels->size(); }
 std::vector<PXIPixel> *GetPixels()                  { return ListOfPixels;}
  PXIPixel         *GetPixelAt( int index)       { return &(ListOfPixels->at(index)); }
  int               GetNumberOfFrames()          { return ListOfFrames->size(); } //JB 2010/06/16
 std::vector<int>      *GetFrames()                  { return ListOfFrames;} //JB 2010/06/16
  int               GetNumberOfTriggers()        { return ListOfTriggerPos->size(); } //JB 2010/06/16
 std::vector<int>      *GetTriggers()                { return ListOfTriggerPos;} //JB 2010/06/16
	
  ClassDef(PXIEvent,1)
};

// --------------------------------------------------------------------------------------

class PXIBoardReader : public TObject {

 private:

  int               DebugLevel;
  DGlobalTools      fTool;                       // JB 2011/07/18

  int               BoardNumber;
  int               NSensors;
  int               TriggerMode;
  int		    TriggerLineOffset;
  int		    FramesPostTrigger;
  int               NextFirstLineToKeep;
  int               NextFirstFrame;
  int               FirstLineToKeep;
  int               FirstFrame;
  int               LastLineToKeep;
  int               LastFrame;
  int               NumberOfLines;
  int               RunNumber;
  int               EventsPerFile;
  int               FramesReadFromFile;
  
  bool              ReadingEvent;
  bool              ReadTwice;
  bool              Overflow; // JB 2009/09/08
  bool              EventReady;
  PXIEvent         *CurrentEvent;
  int               CurrentEventNumber;
 std::vector<PXIPixel>  ListOfPixels;
 std::vector<int>       ListOfTriggerPos; //JB 2010/06/16
 std::vector<int>       ListOfFrames; //JB 2010/06/16

  int               EventsCount; // JB 2009/09/09
  int               EventsOverflow; // JB 2009/09/09
  int               EventsAborted; // JB 2009/09/16
  int               FramesReadTwice; // JB 2009/10/14
  int               NStatesInBlock[100]; // up to 100 blocks
  int               NStatesInLine; // up to 100 blocks
  TH1S             *h1BlockOccupancy; // JB 2009/09/10
  TH1S             *h1LineOccupancy; // JB 2009/09/10

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

  void         AddPixel( int input, int value, int aLine, int aColumn);

  public:

  PXIBoardReader() {;}
  PXIBoardReader( int boardNumber, const char *configFileName, int triggerMode, int endian=0);
  ~PXIBoardReader();
  
  bool         ReadConfiguration();
  void         SetDebugLevel( int level)              { DebugLevel = level; cout << "PXIBoardReader " << BoardNumber << " debug updated to " << DebugLevel << endl;}
  void         SetEndiannes( int endian)              { Endianness = endian; }
  void         AddFileList( const char *prefixFileName, int firstIndex, int endIndex,const char *suffixFileName);
  bool         OpenNextFile();
  bool         GetNextDaqEvent();
  bool         DecodeFrame(int iSensor);
  bool         HasData();
  void         SkipNextEvent();
  int          GetBoardNumber()                       { return BoardNumber; }
  int          GetNSensors()                          { return NSensors; }
  int          GetTriggerMode()                       { return TriggerMode; }
  int          GetRunNumber()                         { return RunNumber; }
  PXIEvent*    GetEvent()                             { return CurrentEvent; }
  int          GetEventNumber()                       { return CurrentEventNumber;}
  char*        GetConfigFileName()                    { return ConfigFileName;}
  char*        GetInputFileName()                     { return InputFileName;}
  char*        GetSuffixFileName()                    { return SuffixFileName;}
  char*        GetPrefixFileName()                    { return PrefixFileName;}
  void         Close();
  void         PrintStatistics(ostream &stream=cout); // JB 2010/08/26 // SS 2011/12/14

  unsigned int SwapEndian( unsigned int data);

  ClassDef(PXIBoardReader,2);  
};

# endif
