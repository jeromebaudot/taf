#ifndef _TNTBoardReader_included_
#define _TNTBoardReader_included_

#include "Riostream.h"
#include "TObject.h"
#include "TMath.h"
//#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include "DGlobalTools.h" // to have fTool has a data member


using namespace std;


// --------------------------------------------------------------------------------------

class TNTPixel : public TObject {

  // Container for a simple pixel
  // only 3 values
  // JB, 2008/09/27

 private:

  int Input;
  int Value;
  int Index;

 public:

  TNTPixel() {  Input = 0; Value = 0; Index = 0; }
  TNTPixel( int input, int value, int index) {  Input = input; Value = value; Index = index; }
  virtual ~TNTPixel() {;}
  int      GetInput()                 { return Input; }
  int      GetValue()                 { return Value; }
  int      GetIndex()                 { return Index; }

  ClassDef(TNTPixel,1)
};

// --------------------------------------------------------------------------------------

class TNTEvent : public TObject {

 private:

  int  EventNumber;
  int  BoardNumber;  
 std::vector<int>      *ListOfTriggers;
 std::vector<int>      *ListOfTimestamps; //RDM150509
 std::vector<int>      *ListOfFrames;
 std::vector<TNTPixel> *ListOfPixels;

 public:

  TNTEvent() {;}
  TNTEvent( int currentEventNumber, int boardNumber, vector<int> *ListOfTriggers, vector<int> *ListOfTimestamps, vector<int> *ListOfFrames, vector<TNTPixel> *ListOfPixels); //RDM150509
 ~TNTEvent();

  int               GetEventNumber()             { return EventNumber; }
  int               GetBoardNumber()             { return BoardNumber; }
  int               GetNumberOfTriggers()        { return ListOfTriggers->size(); }
  int               GetNumberOfTimestamps()      { return ListOfTimestamps->size(); }//RDM150509
 std::vector<int>      *GetTriggers()                { return ListOfTriggers;}
 std::vector<int>      *GetTimestamps()              { return ListOfTimestamps;} //RDM150509
  int               GetTriggerAt( int index)     { return ListOfTriggers->at(index);}
  int               GetTimestampAt( int index)   { return ListOfTimestamps->at(index);}//RDM150509
  int               GetNumberOfFrames()          { return ListOfFrames->size(); }
 std::vector<int>      *GetFrames()                  { return ListOfFrames;} //JB 2010/06/16
  int               GetFrameAt( int index)       { return ListOfFrames->at(index);}
  int               GetNumberOfPixels()          { return ListOfPixels->size(); }
 std::vector<TNTPixel> *GetPixels()                  { return ListOfPixels;}
  TNTPixel         *GetPixelAt( int index)       { return &(ListOfPixels->at(index)); }

  ClassDef(TNTEvent,1)
};

// --------------------------------------------------------------------------------------

class TNTBoardReader : public TObject {

 private:

  int               DebugLevel;
  DGlobalTools      fTool;                       // JB 2011/07/18

  int               BoardNumber;  
  bool              ReadingEvent;
  Int_t             TriggerMode; // JB 2009/09/28
  TNTEvent         *CurrentEvent;
  int               CurrentEventNumber;
  int               CurrentTriggerNumber; // Jb 2009/09/28
  int               CurrentFrameNumber;
  int               CurrentTimestamp; //JB 2009/05/21
  int		    TimeLimitBetweenTwoTriggers; // JB 2009/05/21
 std::vector<int>       ListOfTriggers;
 std::vector<int>       ListOfTimestamps;//RDM150509
  int               maxNumberOfTriggersPerEvent;
 std::vector<int>       ListOfFrames;
 std::vector<TNTPixel>  ListOfPixels;

  ifstream          RawFileStream;
  char             *InputFileName;
  char*             PrefixFileName;
  char*             SuffixFileName;
  int               CurrentFileNumber;
  int               NumberOfFiles;
  bool              NoMoreFile; // JB 2010/10/13
  size_t            SizeOfWord;
  size_t            SizeOfDataBuffer;
  unsigned int     *Data;
  int               CurrentWordIndex;
  int               Endianness;       // 0= do not swap bytes, 1= swap bytes
  int               NumberOfBitsValue; // 13 by default
  bool              SignedValues; // JB 2010/08/26
  int 		    NewFormat; // 30/10/09 NCS

  // Masks for binary decoding, JB 2010/09/26
  int               MaskForValue;
  int               ShiftForValue;
  int               MaskForIndex;
  int               MaskForAddress;
  int               MaxSignedValue;
  
  // For statistics, JB 2010/09/26
  int               TotalNEvents; // JB 2010/08/26
  int               TotalNTriggers; // JB 2010/08/26
  int               TotalNFaultyTriggers; // JB 2010/08/26
  int               TotalNFrames; // JB 2010/08/26
  int               TotalNPixels; // JB 2010/08/26
  int               TotalNErrors; // JB 2010/08/26
  int               TotalOverflows; // JB 2010/10/06

  bool         GetNextWord();
  void         AddPixel( int input, int value, int index);
  
  int eventnumber; // NCS 19/11/2009

  public:

  TNTBoardReader() {;}
  TNTBoardReader( int boardNumber, int sizeOfWord=1024, int endian=0, int timelimit=0, int triggermode=0, int numberOfBits=12, int newformat=1);
  ~TNTBoardReader();
  void         SetDebugLevel( int level)              { DebugLevel = level; cout << "TNTBoardReader " << BoardNumber << " debug updated to " << DebugLevel << endl;}
  void         SetEndiannes( int endian)              { Endianness = endian; }
  bool         AddFile(char *inputFileName);
  bool         AddFileList(const char *prefixFileName, int startIndex, int endIndex, const char *suffixFileName);
  bool         HasData();
  void         SkipNextEvent(); // JB 2009/05/26
  int          GetBoardNumber()                       { return BoardNumber; }
  TNTEvent*    GetEvent()                             { return CurrentEvent; }
  int          GetEventNumber()                       { return CurrentEventNumber;}
  char*        GetInputFileName()                     { return InputFileName;}  //RDM300509
  char*        GetSuffixFileName()                    { return SuffixFileName;} //RDM300509
  char*        GetPrefixFileName()                    { return PrefixFileName;} //RDM300509
  void         Close();
  unsigned int SwapEndian( unsigned int data);
  void         PrintStatistics(ostream &stream=cout); // JB 2010/08/26 // SS 2011/12/06

  ClassDef(TNTBoardReader,1);  
};

# endif
