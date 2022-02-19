#ifndef _VMEBoardReader_HXX
#define _VMEBoardReader_HXX
/*!
  \file
  \version $Id: VMEBoardReader.hxx,v 1.8 2003/07/08 18:54:19 mueller Exp $
  \brief   Declaration of VMEBoardReader.
*/
/*------------------------------------------+---------------------------------*/

#include "BoardReader.h" // withROOT is define therein
#include <fstream>
#include <iostream>
#include <vector>
#include "TString.h"
#include "TH1F.h"
#include "TH2F.h"
#include "DGlobalTools.h" // to have fTool has a data member
using namespace std;

class TH2F;
class TH1F;

#include "mi26types.h"

#define withROOT

//##############################################################################
class VMEBoardReader : public TObject  {

public:

  VMEBoardReader(int boardNumber, TString pathName, TString prefixName, TString suffixName, int runNumber, int numberOfSensors, int numberOfRows);
  ~VMEBoardReader();

  void  SetDebugLevel( int aLevel) { fDebugLevel = aLevel; }
  void  SetVetoPixel( int noiseRun);
  bool  HasData();
  int   GetDebugLevel() { return fDebugLevel; }
  int   GetBoardNumber() { return fBoardNumber; }
  int   GetEventNumber() { return fEventNumber; }
  Int_t GetSensorsN() const { return fNumberOfSensors; }
  BoardReaderEvent*   GetEvent() { return fCurrentEvent; }
  void  PrintStatistics(ostream &stream);

  //! Histograms
  // void  SetDisplayOn() { fDisplay = true; CreateHistogram(); }
  // bool  ValidHistogram() { return fDisplay; }
  // void CreateHistogram();


private:

  int             fDebugLevel;       // debug level
  DGlobalTools    fTool;                     
  Bool_t          fDisplay;
  int             fBoardNumber;
  Int_t           fRunNumber;           // run number
  int             fNumberOfSensors; // fNSensors
  int             fNumberOfRows;
  int             fNumberOfColumns;
  int             fVetoOverflow;
  TString         fPrefixName;          // prefix folder name
  TString         fSuffixName;       // suffix file name
  TString         fPathName;         // base file name
  ifstream        fRawFileAscii[10]; // file streamm

  Int_t             fIndex;             // Index of what?
  Int_t             fEventSize;         // ?
  Int_t             fDataSize;
  vector<UInt_t>    fData;              // data array to fill
  UInt_t*           fDataEvent;         // data array to fill the whole eevent

  Int_t             fEventNumber;        // number of the event
  Int_t             fPrevEventNumber[8];    // previous number of the event
  Int_t             fTriggerNumber;      // number of the trigger
  Int_t             fPrevTriggerNumber[8];  // previous number of the trigger
  Int_t             fTimeStamp;          // time stamp per frame
  Int_t             fPrevTimeStamp[8];      // time stamp per frame

  Int_t             fFrameCount;         // number of frame
  Int_t             fTriggerNumberFrame; // number of the trigger
  Int_t             fTimeStampFrame;     // time stamp per frame
  Int_t             fFirstFrame;         // first frame flag
  Bool_t            fFrameOk;         // first frame flag
  UInt_t            fCurrentTriggerCnt;
  Bool_t            fReadingEvent;
  Bool_t            fOverflow;
  Int_t             fEventsOverflow;
  Int_t             fNStatesInLine;
  Int_t             fFramesReadFromFile;

  BoardReaderEvent* fCurrentEvent;
  std::vector<BoardReaderPixel> ListOfPixels;
  std::vector<int>       ListOfFrames;
  std::vector<int>       ListOfLineOverflow;
  std::vector<int>       ListOfTriggers;


  // Markers used by the acquisition
  static const UInt_t  fgkKeyHeader[];
  static const Int_t   fgkFrameHeaderSize;
  static const UInt_t  fgkKeyTail[];
  static const Int_t   fgkLineWidth;
  static const UInt_t  fgkFrameHeader;
  static const UInt_t  fgkFrameTail;

  static  UInt_t  GetKeyHeader(Int_t idx)                  { return fgkKeyHeader[idx];   }
  static  Int_t   GetHeaderSize()                          { return fgkFrameHeaderSize;  }
  static  UInt_t  GetKeyTail(Int_t idx)                    { return fgkKeyTail[idx];     }
  static  Int_t   GetLineWidth()                           { return fgkLineWidth;        }
  static  UInt_t  GetFrameHeader()                         { return fgkFrameHeader;      }
  static  UInt_t  GetFrameTail()                           { return fgkFrameTail;        }


  //! File management
  virtual Int_t   Open();
  virtual void    Close();

  //! Processing
  virtual Bool_t  Process();
  Bool_t GetSensorEvent(Int_t iSensor);
  Bool_t GetFrame(Int_t iSensor, MI26_FrameRaw* data);
  Bool_t DecodeFrame(Int_t iSensor, MI26_FrameRaw *frame);
  Bool_t IsPartEqual(UInt_t data, UInt_t key);
  void  ResetFrames();
  void  AddPixel( Int_t input, Int_t value, Int_t aLine, Int_t aColumn);
  Int_t  GetSensor(UInt_t key);

  //! Fill histogram frame
  // void FillHistoFrame(Int_t iSensor, MI26_FrameRaw* data);
  // void FillHistoEvt(Int_t iSensor);


   ClassDef(VMEBoardReader,1)
};

#endif
