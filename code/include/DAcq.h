// @(#)maf/dtools:$Name:  $:$Id: DAcq.h,v.2 2005/10/02 18:03:46 sha Exp $
// Author: Dirk Meier   97/12/06 

#ifndef _DAcq_included_
#define _DAcq_included_


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// DAcq                                                                 //
//                                                                      //
// Data Acquisition class                                               //
//                             .                                        //
//////////////////////////////////////////////////////////////////////////

#include <vector>
#include <map>
#include <ctime>

// ROOT classes
#include "TObject.h"
#include "TArrayD.h"   // necessary for ? reason
#include "TArrayS.h"   // also necessary
#include "Riostream.h"
#include "TBits.h"
#include "TVector2.h"
#include "TVector3.h"

#include "DGlobalTools.h"
#include "DSetup.h"
#include "DPixel.h"
//#include "DMonteCarlo.h"
#include "TNTBoardReader.h"
#include "PXIBoardReader.h"
#include "PXIeBoardReader.h"
#include "GIGBoardReader.h"
#include "IMGBoardReader.h"
#include "BoardReader.h"
#include "VMEBoardReader.h"
#include "MCBoardReader.h"
#include "AliMIMOSA22RawStreamVASingle.h"
#include "DecoderM18.h"
#include "DecoderGeant.h"
#include "DEventMC.h"
#include "BoardReaderIHEP.h"
#include "sup_exp.typ" // for time reference information

class DAcq : public TObject {

  private:

      DSetup           *fc;                // pointer to the configuration
      DGlobalTools      fTool;             // JB 2013/08/20

      Int_t             fDebugAcq;         // Acq debug flag
      Int_t             fModuleTypes;      //  number of Acquisition Module Types

      TNTBoardReader  **fTNT;              // pointer to TNT boards
      PXIBoardReader  **fPXI;              // pointer to PXI boards
      PXIeBoardReader **fPXIe;             // pointer to PXIexpress boards
      GIGBoardReader  **fGIG;              // pointer to GIG boards, JB 2012/04/25
      IMGBoardReader  **fIMG;              // pointer to IMG boards, JB 2012/07/22
      VMEBoardReader  **fVME;              // pointer to VME boards, JB 2014/05/13
      MCBoardReader   **fMC;               // pointer to MC boards, AP 2016/04/15
      AliMIMOSA22RawStreamVASingle  **fALI22; // pointer to ALI22 boards, JB 2014/05/14
      DecoderM18      **fM18;              // pointer to DecoderM18 boards, JB 2014/05/25, then 2014/08/26
      DecoderGeant    **fGeant;            // pointer to DecoderGeant
      BoardReaderIHEP **fIHEP;             // pointer to IHEP boards, JB 2018/06/03
      Int_t          ***fRawData;          // pointer to Raw Values
      std::vector<DPixel*>  *fListOfPixels;     // pointer to list of hit pixel
      //std::vector<DMonteCarlo*> *fListOfMonteCarlo; // pointer to list of hit montecarlo
      //std::vector<Int_t>   *fListOfPixels;    // list of hit pixel index
      Int_t             fTriggersN;        // number of triggers in the event, JB 2009/05/22
      Int_t             fFramesN;          // number of frames in the event, JB 2009/05/22
      Int_t             fTimestampsN;      // number of timestamps in the event, JB 2009/05/26
      Int_t            *fLineOverflowN;	   // overflow line counter, MG 2012/02/15
      std::vector<int>      *ListOfTriggers;    // list of triggers JB 2010/06/16
      std::vector<int>      *ListOfFrames;      // list of frames JB 2010/06/16
      std::vector<int>      *ListOfTimestamps;  // list of timestamps JB 2010/06/16
      std::vector<int>      *ListOfLineOverflow; // line overflow vector per sensor, MG 2012/02/15
      Int_t         ****fMatchingPlane;   // plane matching the input
      Int_t         ****fIndexShift;      // index shift to add
      std::vector<int>    ***fInputSegments;    // limits of segments for this input, JB 2013/08/14
      Int_t             fMaxSegments;      // max nb of segements allowed for an input
      Bool_t          **fUseTimestamp;     // flag for timestamp usage, JB 2015/05/26
  
      Int_t             fEventNumber;      //  Number of the event according to DSession, JB 2009/05/26
      Int_t             fRealEventNumber;  //  Number of the event writen in the board
      Int_t             fEventsMissed;     // Number of events missed for synchronization
      Int_t             fEventsDataNotOK;  // Number of events with wrong data
      Int_t             fEventsModuleNotOK;// Number of events with pb in module
      Int_t             fRunNumber;        //  Run Number the event is in

      // Data to synchronize two PXIe boards, JB 2012/07/19
      Char_t           *fSynchroFileName;
      unsigned char    *fSynchroInfo;
      Int_t             fNbSynchroInfo;
  
      // Data to synchronize two M18Decoders, JB 2015/03/27
      Int_t             fSynchroFirstM18Decoder;
      Int_t             fSynchroOMKDTransition;
      Int_t             fSynchroStopPointerTransition;
  
      // Data to obtain external time references, JB 2018/02/11
      Char_t           *fTimeRefFileName;
      SEXP_TTsRec      *fTimeRefInfo;
      Int_t             fNbTimeRefInfo;
      Int_t             fCurrentTimeRefInfo;
      Int_t             fEventReferenceTime;
      Int_t             fEventTime;

      Bool_t            fIfMonteCarlo;     // LC 2014/12/15 : If MonteCarlo Info in DPixel fIfMonteCarlo=1 else fIfMonteCarlo=0
      
      Bool_t            fIsMCBoardReader;  // AP 2016/07/27   bool to specify if reading data with MCBoardReader
      DEventMC*         MCInfoHolder;      // AP 2016/04/21   Object with all the MC information. i.e. the full list of particles, hits and pixels (both from physics and noise)

  public:
      DAcq();                                 
      DAcq(DSetup& c);                                 
      ~DAcq();
      TBits*           NextEvent( Int_t eventNumber, Int_t aTrigger=-1); // actually read the data from raw file!, JB 2009/05/26, 2012/07/10
      void             Reset();                                          // Restart event reading at 0, JB 2015/03/02
      Int_t*           GetRawData( Int_t mdt, Int_t mdl, Int_t input);   // get the raw data buffer
      void             GetMatchingPlaneAndShift( Int_t mdt, Int_t mdl, Int_t input, Int_t channel, Int_t &aPlane, Int_t &aShift);  // get the plane and shift matching the input and channel
      std::vector<DPixel*> *GetListOfPixels( Int_t aPlaneNumber) { return &fListOfPixels[aPlaneNumber-1]; }// get the hit pixel list for a given plane
      //std::vector<Int_t>   *GetListOfPixels( Int_t aPlaneNumber) { return &fListOfPixels[aPlaneNumber]; }// get the hit pixel index list for a given plane
      //std::vector<DMonteCarlo*> *GetListOfMonteCarlo( Int_t aPlaneNumber) { return &fListOfMonteCarlo[aPlaneNumber-1]; }// get the hit monte carlo list for a given plane
      Bool_t           GetUsageTimestamp( Int_t mdt, Int_t mdl) { return fUseTimestamp[mdt-1][mdl-1];} // JB 2015/05/26
  
      Bool_t           DumpHexToTerm();     // performs a hexadecimal dump of data
                                          // without any interpretation of data
      Int_t            GetEventNumber()     const { return fEventNumber; } // Number of the event according to Dsession, JB 2009/05/26
      Int_t            GetRealEventNumber() const { return fRealEventNumber; } // Number of the event in the raw data file
      Int_t            GetRunNumber()       const { return fRunNumber; }   // Run Number the event is in

      std::vector<int>     *GetTriggers()                { return ListOfTriggers;}
      std::vector<int>     *GetFrames()                  { return ListOfFrames;}
      std::vector<int>     *GetTimestamps()              { return ListOfTimestamps;}
      std::vector<int>     *GetLineOverflow()            { return ListOfLineOverflow;}      
      Int_t            GetNumberOfTriggers()        { return fTriggersN;}
      Int_t            GetNumberOfFrames()          { return fFramesN;}
      Int_t            GetNumberOfTimestamps()      { return fTimestampsN;}
      Int_t            GetTriggerAt( int index)     { return ListOfTriggers->at(index); }
      Int_t            GetFrameAt( int index)       { return ListOfFrames->at(index); }
      Int_t            GetTimestampAt( int index)   { return ListOfTimestamps->at(index); }
  
      // Methods to synchronized two boards, JB 2012/07/19
      Bool_t           InitSynchroInfo( );
      Bool_t           GetSynchroInfo( int anEventId, int &anAcqId, int &aFrameId);
      
      // Methods to obtain an outside time reference , JB 2018/02/11
      Bool_t           InitTimeRefInfo( );
      Bool_t           GetTimeRef( int index, int &recordID, int &cycleID, int &rtcTime, int &ntpTime);
      Int_t            GetEventTime()               { return fEventTime; } // JB 2018/02/12
  
      DSetup&          GetSetup()                            { return *fc; }
      void             SetDebug(Int_t aDebug);
      Int_t            GetDebug()                            { return fDebugAcq;}

      void             PrintStatistics(ostream &stream=cout); // JB 2009/09/09 //SS 2011/12/14
      void             DumpSynchroInfo( Int_t nEvents=-1); // JB 2013/08/20

      Bool_t           IfMonteCarlo()               { return fIfMonteCarlo; }  // LC 2014/12/15 : Test to include MonteCarlo Infos
      
      Bool_t           GetIfMCBoardReader()         { return fIsMCBoardReader; }  // AP 2016/07/27 : Function to get the if reading data with MCBoardReader
      DEventMC*        GetMCInfoHolder()            { return MCInfoHolder;  }     // AP 2016/04/21 : Function to get the MCInfoHolder
      
      ClassDef(DAcq,3)                    // Data Acquisition 
};
 
#endif


