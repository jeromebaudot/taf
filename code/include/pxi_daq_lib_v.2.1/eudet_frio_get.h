 
/*******************************************************************************
File      : x:\lib\win\eudet_frio\eudet_frio.h
Goal      : Functions prototypes of flex rio board library for EUDET Mimosa 26 DAQ
Prj date  : 05/08/2010
File date : 05/08/2010
Doc date  : 05/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
----------------------------------------------------------------------------------
License   : You are free to use this source files for your own development as long
          : as it stays in a public research context. You are not allowed to use it
          : for commercial purpose. You must put this header with laboratory and
          : authors names in all development based on this library.
----------------------------------------------------------------------------------
Labo      : IPHC */
/*******************************************************************************/


#ifndef EUDET_FRIO_GET_H

#include "func_header.def"

// 06/11/2010 -> 29


// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, char* DIU_FGetVersion ();)
// FHEAD ( SInt32 REF_FHello ();)
// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, ;)


FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt32* EFRIO__FGetFrameFields ( SInt32 AcqId, SInt8 Mi26Id, SInt32 FrameIdInAcq, UInt32* PtDest, SInt8 DestW32Sz );)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt32 EFRIO__FGetFrameHeader      ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt32 EFRIO__FGetFrameFrameCnt    ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt32 EFRIO__FGetFrameDataLength  ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt32 EFRIO__FGetFrameTrailer     ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetFrameTrigCnt     ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetFrameTrigStatus  ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt32 EFRIO__FGetFrameMi26Trig0   ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt32 EFRIO__FGetFrameMi26Trig1   ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt32 EFRIO__FGetFrameMi26Trig2   ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt32 EFRIO__FGetFrameTluTrig     ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq, SInt16 TrigNo );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt32 EFRIO__FGetFrameFlexRioTrig ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq, SInt16 TrigNo );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetFrameHitCnt      ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq );)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, float EFRIO__FGetDataRateMBPerSec  ( SInt8 PrintInMsgFile );)


FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, EFRIO__TFrame* EFRIO__FGetFramePt ( SInt32 AcqId, SInt32 FrameIdInAcq );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetBoardConf ( SInt32 BoardId, EFRIO__TBoardConf* PtDest );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetBoardConfBoardId ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, char* EFRIO__FGetBoardConfAsicName ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetBoardConfAsicNb ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetBoardConfReadoutMode ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt8 EFRIO__FGetBoardConfEmuleChannels ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, float EFRIO__FGetBoardConfDataClkFrequency ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt32 EFRIO__FGetBoardConfDmaHostSz ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetBoardConfFrameNbPerAcq ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt8 EFRIO__FGetBoardConfEnableExtraChannel ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetBoardConfAcqNbPerTrig ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt8 EFRIO__FGetBoardConfTriggerMode ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt32 EFRIO__FGetBoardConfTriggerDetectTimeWindow ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt32 EFRIO__FGetBoardConfTriggerDetectOccurNb ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt32 EFRIO__FGetBoardConfTimeStampRes ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt8 EFRIO__FGetBoardConfEnableTimeStamping ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt8 EFRIO__FGetBoardConfEnableTrigCnt ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt8 EFRIO__FGetBoardConfTagEventsStoredByDUT ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt32 EFRIO__FGetBoardConfReadTluTrigCntEachNTrig ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetBoardStatusBoardId ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt8  EFRIO__FGetBoardStatusBoardPresent ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt8 EFRIO__FGetBoardStatusFwLoaded ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt8 EFRIO__FGetBoardStatusConfDone ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetBoardStatusStatusCode ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, char* EFRIO__FGetBoardStatusStatusStr ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, char* EFRIO__FGetBoardStatusErrorMsgList ( SInt32 BoardId, SInt32 Index );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, char* EFRIO__FGetBoardStatusLastErrorMsg ( SInt32 BoardId );)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetCmdRun ();)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetMonInfFrameNbToSend ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetMonInfSzToSend ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetRunContSendOnEth ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetRunContFrameNbPerAcq ();)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetTestOnDataHeaderErrCnt ( SInt8 MapsId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetTestOnDataTotErrCnt ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetTestOnDataErrOnCurrentAcq ();)


FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetFrameFirstStateLineWordStatesNb ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetFrameFirstStateLineWordLineNo   ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetFrameFirstGroupStatesNb         ( SInt32 AcqId, SInt16 Mi26Id, SInt32 FrameIdInAcq );)



#ifndef APP_DLL_IMPORT_EXPORT
  #ifndef EUDET_FRIO_GET_H
    #define EUDET_FRIO_GET_H
    #endif
#endif


#endif