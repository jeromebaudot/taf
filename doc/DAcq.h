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

// ROOT classes
#include "TObject.h"
#include "TArrayD.h"   // necessary for ? reason
#include "TArrayS.h"   // also necessary
#include "Riostream.h"
#include "TBits.h"

#include "DGlobalTools.h"
#include "DSetup.h"
#include "DPixel.h"
#include "DMonteCarlo.h"
#include "TNTBoardReader.h"
#include "PXIBoardReader.h"
#include "PXIeBoardReader.h"
#include "GIGBoardReader.h"
#include "IMGBoardReader.h"
#include "BoardReader.h"
#include "VMEBoardReader.h"
#include "AliMIMOSA22RawStreamVASingle.h"
#include "DecoderM18.h"


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
      AliMIMOSA22RawStreamVASingle  **fALI22; // pointer to ALI22 boards, JB 2014/05/14
      DecoderM18      **fM18;              // pointer to DecoderM18 boards, JB 2014/05/25, then 2014/08/26
      Int_t          ***fRawData;          // pointer to Raw Values
      vector<DPixel*>  *fListOfPixels;     // pointer to list of hit pixel
      vector<DMonteCarlo*> *fListOfMonteCarlo; // pointer to list of hit montecarlo
      //vector<Int_t>   *fListOfPixels;    // list of hit pixel index
      Int_t             fTriggersN;        // number of triggers in the event, JB 2009/05/22
      Int_t             fFramesN;          // number of frames in the event, JB 2009/05/22
      Int_t             fTimestampsN;      // number of timestamps in the event, JB 2009/05/26
      Int_t            *fLineOverflowN;	   // overflow line counter, MG 2012/02/15
      vector<int>      *ListOfTriggers;    // list of triggers JB 2010/06/16
      vector<int>      *ListOfFrames;      // list of frames JB 2010/06/16
      vector<int>      *ListOfTimestamps;  // list of timestamps JB 2010/06/16
      vector<int>      *ListOfLineOverflow; // line overflow vector per sensor, MG 2012/02/15
      Int_t         ****fMatchingPlane;   // plane matching the input
      Int_t         ****fIndexShift;      // index shift to add
      vector<int>    ***fInputSegments;    // limits of segments for this input, JB 2013/08/14
      Int_t             fMaxSegments;    // max nb of segements allowed for an input
  
      Int_t             fEventNumber;      //  Number of the event according to DSession, JB 2009/05/26
      Int_t             fRealEventNumber;  //  Number of the event writen in the board
      Int_t             fEventsMissed;         // Number of events missed for synchronization
      Int_t             fRunNumber;        //  Run Number the event is in

      // Data to synchronized two boards, JB 2012/07/19
      Char_t           *fSynchroFileName;
      unsigned char    *fSynchroInfo;
      Int_t             fNbSynchroInfo;

  public:
      DAcq();                                 
      DAcq(DSetup& c);                                 
      ~DAcq();
      TBits*           NextEvent( Int_t eventNumber, Int_t aTrigger=-1); // actually read the data from raw file!, JB 2009/05/26, 2012/07/10
      Int_t*           GetRawData( Int_t mdt, Int_t mdl, Int_t input); // get the raw data buffer
      void             GetMatchingPlaneAndShift( Int_t mdt, Int_t mdl, Int_t input, Int_t channel, Int_t &aPlane, Int_t &aShift);  // get the plane and shift matching the input and channel
      vector<DPixel*> *GetListOfPixels( Int_t aPlaneNumber) { return &fListOfPixels[aPlaneNumber-1]; }// get the hit pixel list for a given plane
      //vector<Int_t>   *GetListOfPixels( Int_t aPlaneNumber) { return &fListOfPixels[aPlaneNumber]; }// get the hit pixel index list for a given plane
      vector<DMonteCarlo*> *GetListOfMonteCarlo( Int_t aPlaneNumber) { return &fListOfMonteCarlo[aPlaneNumber-1]; }// get the hit monte carlo list for a given plane

      Bool_t           DumpHexToTerm();     // performs a hexadecimal dump of data
                                          // without any interpretation of data
      Int_t            GetEventNumber()     const { return fEventNumber; } // Number of the event according to Dsession, JB 2009/05/26
      Int_t            GetRealEventNumber() const { return fRealEventNumber; } // Number of the event in the raw data file
      Int_t            GetRunNumber()       const { return fRunNumber; }   // Run Number the event is in

      vector<int>     *GetTriggers()                { return ListOfTriggers;}
      vector<int>     *GetFrames()                  { return ListOfFrames;}
      vector<int>     *GetTimestamps()              { return ListOfTimestamps;}
      vector<int>     *GetLineOverflow()            { return ListOfLineOverflow;}      
      Int_t            GetNumberOfTriggers()        { return fTriggersN;}
      Int_t            GetNumberOfFrames()          { return fFramesN;}
      Int_t            GetNumberOfTimestamps()      { return fTimestampsN;}
      Int_t            GetTriggerAt( int index)     { return ListOfTriggers->at(index); }
      Int_t            GetFrameAt( int index)       { return ListOfFrames->at(index); }
      Int_t            GetTimestampAt( int index)   { return ListOfTimestamps->at(index); }
  
      // Methods to synchronized two boards, JB 2012/07/19
      Bool_t           InitSynchroInfo( );
      Bool_t           GetSynchroInfo( int anEventId, int &anAcqId, int &aFrameId);
      
      DSetup&          GetSetup()                            { return *fc; }
      void             SetDebug(Int_t aDebug);
      Int_t            GetDebug()                            { return fDebugAcq;}

      void             PrintStatistics(ostream &stream=cout); // JB 2009/09/09 //SS 2011/12/14
      void             DumpSynchroInfo( Int_t nEvents=-1); // JB 2013/08/20

      ClassDef(DAcq,3)                    // Data Acquisition 
};
 
#endif


