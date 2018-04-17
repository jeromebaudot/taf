#ifndef _GIGBoardReader_included_
#define _GIGBoardReader_included_


#include "Riostream.h"
#include "TObject.h"
#include "TH1.h"
#include <fstream>
#include <vector>
#include "DGlobalTools.h" // to have fTool has a data member


// --------------------------------------------------------------------------------------

class GIGPixel : public TObject {

  // Container for a simple pixel
  // JB, 2009/08/13

 private:

  int Input;
  int Value;
  int LineNumber;
  int ColumnNumber;
/*  double monteCarloX;
  double monteCarloY;
  double monteCarloZ;
*/

 public:

  GIGPixel() {  Input = 0; Value = 0; LineNumber = 0; ColumnNumber = 0; /*monteCarloX = 0; monteCarloY = 0; monteCarloZ = 0;*/ }
  GIGPixel( int input, int value, int line, int col/*, double XmonteCarlo, double YmonteCarlo, double ZmonteCarlo*/) {  Input = input; Value = value; LineNumber = line; ColumnNumber = col; /*monteCarloX = XmonteCarlo; monteCarloY = YmonteCarlo; monteCarloZ = ZmonteCarlo;*/ }
  virtual ~GIGPixel() {;}
  int      GetInput()                 { return Input; }
  int      GetValue()                 { return Value; }
  int      GetLineNumber()            { return LineNumber; }
  int      GetColumnNumber()          { return ColumnNumber; }
/*  double   GetMonteCarloX()           { return monteCarloX; }
  double   GetMonteCarloY()           { return monteCarloY; }
  double   GetMonteCarloZ()           { return monteCarloZ; }
*/
  ClassDef(GIGPixel,1)
};

// --------------------------------------------------------------------------------------


class GIGMonteCarlo : public TObject {

  // Container for the monte carlo
  // LC 2012/10/17

 private:

  int Input;
  int Value;
  double monteCarloX;
  double monteCarloY;
  double monteCarloZ;

 public:

  GIGMonteCarlo() {  Input = 0; Value = 0; monteCarloX = 0; monteCarloY = 0; monteCarloZ = 0; }
  GIGMonteCarlo( int input, int value, double XmonteCarlo, double YmonteCarlo, double ZmonteCarlo) {  Input = input; Value = value; monteCarloX = XmonteCarlo; monteCarloY = YmonteCarlo; monteCarloZ = ZmonteCarlo; }
  virtual ~GIGMonteCarlo() {;}
  int      GetInput()                 { return Input; }
  int      GetValue()                 { return Value; }
  double   GetMonteCarloX()           { return monteCarloX; }
  double   GetMonteCarloY()           { return monteCarloY; }
  double   GetMonteCarloZ()           { return monteCarloZ; }

  ClassDef(GIGMonteCarlo,1)
};


class GIGEvent : public TObject {

 private:

  int  EventNumber;
  int  BoardNumber;  
  vector<GIGPixel>      *ListOfPixels;
  vector<GIGMonteCarlo> *ListOfMonteCarlo;

 public:

  GIGEvent() {;}
  GIGEvent( int EventNumber, int boardNumber, vector<GIGPixel> *listOfPixels, vector<GIGMonteCarlo> *listOfMonteCarlo);
 ~GIGEvent();

  int               GetEventNumber()             { return EventNumber; }
  int               GetBoardNumber()             { return BoardNumber; }
  int               GetNumberOfPixels()          { return ListOfPixels->size(); }
  int               GetNumberOfMonteCarlo()      { return ListOfMonteCarlo->size(); }
  vector<GIGPixel> *GetPixels()                  { return ListOfPixels;}
  GIGPixel         *GetPixelAt( int index)       { return &(ListOfPixels->at(index)); }
  vector<GIGMonteCarlo> *GetMonteCarlo()                  { return ListOfMonteCarlo;}
  GIGMonteCarlo         *GetMonteCarloAt( int index)      { return &(ListOfMonteCarlo->at(index)); }
  

  ClassDef(GIGEvent,1)
};

// --------------------------------------------------------------------------------------

class GIGBoardReader : public TObject {

 private:

  int               DebugLevel;
  DGlobalTools      fTool;

  int               BoardNumber;
  int               NSensors;

  bool              ReadingOK;
  bool              EventReady;
  bool              EventStarted;
  GIGEvent         *CurrentEvent;
  int               CurrentEventID;
  int               NewEventID;
  vector<GIGPixel>  ListOfPixels;
  vector<GIGMonteCarlo> ListOfMonteCarlo;

  int               EventsCount;
  int               AveragePixelCountTotal;
  int               AveragePixelCountTotalTemp;
  int              *AveragePixelCount;
  int              *AveragePixelCountTemp;

  ifstream          RawFileStream;
  char             *InputFileName;
  char*             PrefixFileName;
  char*             SuffixFileName;
  int               CurrentFileNumber;
  int               NumberOfFiles;

  int               bufferPlane;

  void              AddPixel( int input, int value, int aLine, int aColumn, double coordX, double CoordY, double CoordZ);
  void              GenerateNewEvent();

  public:

  GIGBoardReader() {;}
  GIGBoardReader( int boardNumber, int nSensors);
  ~GIGBoardReader();
  
  void         SetDebugLevel( int level)              { DebugLevel = level; cout << "GIGBoardReader " << BoardNumber << " debug updated to " << DebugLevel << endl;}
  //void         AddFileList(char *prefixFileName, int firstIndex, int endIndex, char *suffixFileName);
  bool         AddFile(char *fileName);
  void         ReadLine();
  bool         HasData();
  void         SkipNextEvent();
  int          GetBoardNumber()                       { return BoardNumber; }
  int          GetNSensors()                          { return NSensors; }
  GIGEvent*    GetEvent()                             { return CurrentEvent; }
  int          GetEventNumber()                       { return EventsCount;}
  char*        GetInputFileName()                     { return InputFileName;}
  char*        GetSuffixFileName()                    { return SuffixFileName;}
  char*        GetPrefixFileName()                    { return PrefixFileName;}
  void         Close();
  void         PrintStatistics(ostream &stream=cout);

  ClassDef(GIGBoardReader,1);  
};

# endif
