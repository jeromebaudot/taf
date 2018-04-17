#ifndef _BoardReader_included_
#define _BoardReader_included_


#include "Riostream.h"
#include "TObject.h"
#include <vector>
//using namespace std;

// --------------------------------------------------------------------------------------

class BoardReaderPixel : public TObject {
    
  // Container for a simple pixel
  // JB, 2014/05/12

 private:

  int Input;
  int Value;
  int Index;
  int LineNumber;
  int ColumnNumber;
  int TimeStamp;

 public:

  BoardReaderPixel() {  Input = 0; Value = 0; Index = 0; LineNumber = 0; ColumnNumber = 0; TimeStamp = 0;}
  BoardReaderPixel( int input, int value, int index) {  Input = input; Value = value; Index = index; LineNumber = 0; ColumnNumber = 0; TimeStamp = 0;}
  BoardReaderPixel( int input, int value, int line, int col) {  Input = input; Value = value; Index = 0; LineNumber = line; ColumnNumber = col; TimeStamp = 0;}
  virtual ~BoardReaderPixel() {;}
  int      GetInput()                 { return Input; }
  int      GetValue()                 { return Value; }
  int      GetIndex()                 { return Index; }
  int      GetLineNumber()            { return LineNumber; }
  int      GetColumnNumber()          { return ColumnNumber; }
  int      GetTimeStamp()             { return TimeStamp; }
  void     SetTimeStamp( int timeStamp) { TimeStamp = timeStamp; }
  
  ClassDef(BoardReaderPixel,1)
};

// --------------------------------------------------------------------------------------

class BoardReaderEvent : public TObject {

  // A simple event
  // JB, 2014/05/12
  
 private:

  int  EventNumber;
  int  BoardNumber;  
  int  RunNumber;
  vector<BoardReaderPixel> *ListOfPixels;
  vector<int>       *ListOfTriggers;
  vector<int>       *ListOfFrames;
  vector<int>       *ListOfTimeStamps;

 public:

  BoardReaderEvent() {;}
  BoardReaderEvent( int eventNumber, int boardNumber, int runNumber) ;
  BoardReaderEvent( int eventNumber, int boardNumber, int runNumber, vector<BoardReaderPixel> *aListOfPixels);
  ~BoardReaderEvent();

  int                GetEventNumber()             { return EventNumber; }
  int                GetBoardNumber()             { return BoardNumber; }
  int                GetRunNumber()               { return RunNumber; }
  int                GetNumberOfPixels()          { return ListOfPixels->size(); }
  vector<BoardReaderPixel> *GetPixels()           { return ListOfPixels;}
  BoardReaderPixel         *GetPixelAt( int index)       { return &(ListOfPixels->at(index)); }
  int                GetNumberOfFrames()          { return ListOfFrames->size(); }
  vector<int>       *GetFrames()                  { return ListOfFrames;}
  int                GetNumberOfTriggers()        { return ListOfTriggers->size(); }
  vector<int>       *GetTriggers()                { return ListOfTriggers;}
  int                GetNumberOfTimestamps()      { return ListOfTimeStamps->size(); }
  int                GetTimestampAt( int index)   { return ListOfTimeStamps->at(index);}
  vector<int>       *GetTimestamps()              { return ListOfTimeStamps;}

  void               SetRunNumber( int aRunNumber ) { RunNumber = aRunNumber; }
  void               SetListOfTriggers( vector<int> *aListOfTriggers ) { ListOfTriggers = aListOfTriggers; }
  void               SetListOfFrames( vector<int> *aListOfFrames ) { ListOfFrames = aListOfFrames; }
  void               SetListOfTimeStamps( vector<int> *aListOfTimeStamps ) { ListOfTimeStamps = aListOfTimeStamps; }

  
  ClassDef(BoardReaderEvent,1)
};

# endif
