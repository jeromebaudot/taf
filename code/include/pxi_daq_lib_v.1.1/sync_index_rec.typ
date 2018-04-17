/* ============== */
/* 10/07/2012     */
/* -------------- */
/* Doc 18/07/2012 */
/* ============== */

#ifndef SYNC_INDEX_REC_TYP
#define SYNC_INDEX_REC_TYP

#define CC_UNIX   // Specify operating system
#include "types.typ"

typedef struct {

  // The fields needed to synchronize DUT & Telescope run are marked with a *
  // The other fields are extra information, which may be use to make cross-check

  UInt32 Version; // Record version

  // DUT events information

  UInt32 DutEvId;            // * Event identifier from 0 to NbMaxEventsInRun - 1
  UInt32 DutEvTag;           // Event tag = Mi26 sync signal pulses count = Mi26 frames counter
  UInt32 DutTrigLine;        // Line read by DAQ when trigger has occurred
  UInt32 DutTrigFrame;       // Frame read by DAQ when trigger has occurred
  UInt32 DutTrigNbTot;       // Total number of triggers seen by DAQ since beginning of run
  UInt32 DutTrigNbAccepted;  // Number of triggers accepted by DAQ since beginning of run = number of events taken by DAQ
  UInt32 DutSpareU32;        // Reserved

  // Telescope information

  UInt32 TelBegEvFrId;       // * First frame corresponding to DUT event

                             // Frame which contains trigger info
                             //
  UInt32 TelTrigFrId;        // Frame id since begining of run => 0 .. NbMaxFramesInRun - 1
  UInt32 TelTrigAcqId;       // Acquisition id since begining of run
  UInt32 TelTrigFrIdInAcq;   // Frame id in current acquisition => 0 .. NbMaxFramesPerAcq
  UInt32 TelTrigEvTag;       // Event tag = Mi26 frames counter

                             // Three first triggers of the frame (TelTrigFrId)
                             // Unit is Mi26 clock TS => divide by 16 to get trigger line
                             //
  UInt32 TelTrig0;           // First trigger
  UInt32 TelTrig1;           // Second trigger
  UInt32 TelTrig2;           // Third trigger

  UInt32 TelFrTrigNb;        // Total number of trigger of the frame (TelTrigFrId)

  UInt32 TelTrigSpareU32;    // Reserved

  UInt32 TelEndEvFrId;       // * Last frame corresponding to DUT event

} APP__TSyncIndexRec;

#endif

