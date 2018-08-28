// @(#)maf/dtools:$Name:  $:$Id:DSession.h  v.1 2005/10/02 18:03:46 sha Exp $
// Author   : ?

#ifndef _DSession_included_
#define _DSession_included_


  ////////////////////////////////////////////////////////////
  // Class Description of DSession                          // 
  //                                                        //
  // + frame for analysis of raw event data                 //
  // + dumps data on standard output                        //
  // + fills events for storing in .root file               //
  //                                                        //
  ////////////////////////////////////////////////////////////

// Root classes
#include "TObject.h"
#include "TString.h"
#include "TTree.h"
#include "TBranch.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TStopwatch.h"
#include "TSystem.h"
#include "DEvent.h"   // has to be included, because of return from inline functions
#include "DGlobalTools.h" // to have fTool has a data member

using namespace std;

class DSetup;         // forwards
class DAcq;
//class DReader;
class DTracker;

class DSession : public TObject {

 private:
  static DSession* fgInstance;
  Int_t          fDebugSession;                // session debug flag
  Int_t          fPlaneToScan;
  Int_t          fStatus;                      // Status, 0 = initalizing, >= 1 aligning, running
  Int_t          fEventsToDo;                  // number of events to read from tape/file
  Int_t          fCurrentEventNumber;	       // actual event number // VR 2014/07/13 renamed
  Int_t          fRunNumber;                   // the run number
  TString        fConfigPath;                  // the path to the directory that contains telescope configuration files 
  TString        fConfigFileName;              // name of the configuration file
  TString        fRawSourcePath;               // path to the raw data source  (input)
  TString        fSummaryFilePath;             // path to the summary file (output)
  TString        fSummaryFileName;             // name of the data summary file
  TString        fSummaryFilePathAndName;      // name for the .root file (DSF)
  TString        fSummaryFileTitle;            // title for  the .root file (DSF)
  TString        fResultDir;                   // name of result dir
  TString        fResultsDirParent;
  TString        fOutputFilesPrefix;           // Prefix for Output Files
  TString        fOutputFilesSuffix;           // Suffix for Output Files
  //TString        fWeightFileName;	       // name of the file containings eta parameters, JB 2011/04/12
  //TFile         *fWeightFile;                  // pointer to .root file, contains weights for accurate hit measurements

  Int_t          fPlaneNumber;                 // ... a specific planenumber to look at
  DTracker      *fTracker;                     // pointer to the tracker
  DSetup        *fc;                           // pointer to configuration
  DAcq          *fAcq;                         // pointer to Acquisition
/*   DReader       *fReader;                      // pointer to Run Reader */
  DEvent        *fEvent;                       // pointer to an Event
  TTree         *fEventTree;                   // pointer to the Tree 
  Int_t          fFillLevel;                   // controls amount of info stored in Tree, JB 2011/07/21
  Int_t          fEventBuildingMode;           // To switch externally EventBuildingMode. SS 2011.11.14
  
  Double_t    fTrackLimitsForAlignX[2];        // min-max in X for tracks used in alignment procedure, JB 2013/06/10
  Double_t    fTrackLimitsForAlignY[2];        // min-max in Y for tracks used in alignment procedure, JB 2013/06/10
  Double_t    fTrackChi2LimitForAlign;         // upper chi2 limit in alignment procedure, JB 2013/07/14
  
  TStopwatch     fWatch;                       // to monitor event processing time JB, 2008/08/08
  TString        fBeamTime;
  
  TFile         *fSummaryFile;                // pointer to data summary file
  DGlobalTools   fTool;                       // JB 2011/07/18
  
  Bool_t        fDaqAbleToGoToAspecificEvent; // Is the DAQ able to go to a specific event ? // VR 2014/07/13

 public:
  DSession();
  DSession(const Int_t num);
  DSession(Option_t*);

  void           MakeTree();
  Bool_t         NextRawEvent( Int_t aTrigger=-1); // get next event from run
  Int_t          GoToEvent(Int_t anEvent);      // Specific method to ask the DAQ for a given event // VR 2014/07/13
  Int_t          GoToNextEvent(void);           // Specific method to ask the DAQ for a given event // VR 2014/07/13
  void           ResetDaq();                    // Restart event reading from the beginning, JB 2015/03/02
  void           Loop();                 // loop over events and fill .root
  void           SetPlaneToScan(Int_t aPlnb) {fPlaneToScan = aPlnb ;}         
  void           Scan() ;                // loop and plot plane data  
  void           Finish();
  void           InitSession();						    
  void           FillTree();

  Int_t          GetDebug()                        { return fDebugSession;}
  DEvent        *GetEvent()                        { return  fEvent;      }   
  TTree         *GetEventTree()                    { return  fEventTree;  }
  TFile         *GetSummaryFile()                  { return  fSummaryFile;                 }
  DTracker      *GetTracker()                      { return  fTracker;                     }
  DAcq          *GetDataAcquisition()              { return  fAcq;                         }
  DSetup        *GetSetup()                        { return  fc;                           }
  Int_t          GetStatus()                       { return  fStatus;                      }

  TString        GetSummaryFileName()              { return  fSummaryFileName;             }
  TString        GetSummaryFileTitle()             { return  fSummaryFileTitle;            }
  TString        GetSummaryFilePath()              { return  fSummaryFilePath;             }
  TString        GetRawSourcePath()                { return  fRawSourcePath;               } 
  TString        GetConfigPath()                   { return  fConfigPath;} 
  TString        GetConfigFileName()               { return  fConfigFileName;}//VR 2014/06/29
  TString        GetResultDirName()                { return  fResultDir;                   } // JB 2009/09/14
  TString        GetResultsDirParent()             { return  fResultsDirParent;            }
  TString        GetOutputFilesPrefix()            { return  fOutputFilesPrefix;           }
  TString        GetOutputFilesSuffix()            { return  fOutputFilesSuffix;           }
  //TFile         *GetWeightFile()                    { return fWeightFile;		   } // JB 2011/04/12
  //TString        GetWeightFileName()                { return fWeightFileName;		   } // Jb 2011/04/12
  Int_t          GetRunNumber()                    { return  fRunNumber;                   }
  Int_t          GetPlaneNumber()                  { return  fPlaneNumber;                 }
  Int_t          GetCurrentEventNumber()           { return  fCurrentEventNumber;          } // VR 2014/07/13

  void           SetRunNumber(Int_t aRunNumber)    { fRunNumber  = aRunNumber;             }
  void           SetEvents(Int_t aNumberOfEvents)  { fEventsToDo += aNumberOfEvents;       } 
  void 		 SetEventsToDo(Int_t aNumberOfEventsToDo) { fEventsToDo = aNumberOfEventsToDo;};//VR 2014/06/29
  void           SetStatus(Int_t aStatus)          { fStatus = aStatus;     cout << endl << "The Session status just changed to " << aStatus << endl;
} // JB 2009/07/17
  void           SetFillLevel( Int_t aLevel)       { fFillLevel = aLevel; } // JB 2011/07/21
  void           SetEventBuildingMode (Int_t aEventBuildingMode) {fEventBuildingMode = aEventBuildingMode; } //SS 2011.11.14

  void           SetConfigPath(TString aConfigPath){ fConfigPath = aConfigPath; }
  void           SetConfigFileName(TString aConfigFileName){ fConfigFileName = aConfigFileName; } // VR 2014/06/30
  void           SetResultsDir(TString aPath){ fResultDir = aPath; gSystem->mkdir( fResultDir, 1);} // JB 2011/04/12
  void           SetResultsDirParent(TString aPath){ fResultsDirParent = aPath; gSystem->mkdir( fResultsDirParent, 1);}
  void           SetOutputFilesPrefix(TString aString) {fOutputFilesPrefix = aString;}
  void           SetOutputFilesSuffix(TString aString) {fOutputFilesSuffix = aString;}
  void           SetRawSourcePath(TString aRawSourcePath){ fRawSourcePath = aRawSourcePath; }
  void           SetSummaryFilePath(TString aSummaryFilePath){ fSummaryFilePath = aSummaryFilePath; gSystem->mkdir( fSummaryFilePath, 1); }
  void           SetSummaryFileName(TString aSummaryFileName){ fSummaryFileName = aSummaryFileName; }
  void           SetSummaryFilePathAndName() { fSummaryFilePathAndName = fSummaryFilePath + "/" + fSummaryFileName; }
  //void		 SetWeightFile( TString aFileName); // JB 2011/04/12
  
  void           SetPlaneNumber(Int_t aPlaneNumber){ fPlaneNumber = aPlaneNumber;};  
  void           SetDebug(Int_t aDebug);
  void           SetTrackGeoLimitsForAlign(Double_t xmin, Double_t xmax, Double_t ymin, Double_t ymax); // JB 2013/06/11
  void           GetTrackGeoLimitsForAlign(Double_t &xmin, Double_t &xmax, Double_t &ymin, Double_t &ymax); // JB 2013/06/11
  void           SetTrackChi2LimitForAlign( Double_t aLimit); // JB 2013/07/14
  Double_t       GetTrackChi2LimitForAlign() { return fTrackChi2LimitForAlign; } // JB 2013/07/14

    static  DSession*& Instance() { 
      if (!fgInstance)  fgInstance = new  DSession("options"/*'s'*/); 
      return fgInstance; 
    }
   
  ClassDef(DSession,1)                         // Frame for event loops
};

//R__EXTERN DSession  *tSession;

//#define tSession DSession::Instance() 
    

#endif

