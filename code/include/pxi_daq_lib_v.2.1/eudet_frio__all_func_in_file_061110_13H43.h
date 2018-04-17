 
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


#ifndef EUDET_FRIO_H

#include "func_header.def"


// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, char* DIU_FGetVersion ();)
// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, ;)
// FHEAD ( SInt32 REF_FHello ();)

FHEAD ( SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet3Mode1Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt8 AcqStatus, SInt16 TrigStatus, SInt8 DataConvertMode );)
  

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32  EFRIO__FBegin ( SInt8 ErrorLogLvl, char* ErrorLogFile, SInt8 MsgLogLvl, char* MsgLogFile );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32  EFRIO__FEnd ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32  EFRIO__FPrintMsg ( SInt32 DummyS32In, SInt8 PrintAsMsg, SInt8 PrintAsTrace, SInt8 PrintAsWarning, SInt8 PrintAsError, char* Msg );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt32* EFRIO__FTrfData ( SInt8 CmdGetPtrCpyAlloc, UInt32 AllocW32Sz, UInt32* PtSrcW32, UInt32 SrcW32Sz );)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FDbgMeasRamCpyTime ( SInt8 Cmd, SInt32 SzMb );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FDbgCheckTrigAlgo  ( SInt8 Cmd, SInt8 TriggerNb, SInt8 AddFrameBefore, SInt8 AddFrameAfter );)

// Param add on 30/10/10
// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32  EFRIO__FConfRun ( SInt8 Mi26Nb, SInt32 RunNo, SInt32 TotEvNb, SInt32 EvNbPerFile, SInt32 FrameNbPerAcq, SInt8 DataTransferMode, char* DestDir, char* FileNamePrefix );)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32  EFRIO__FConfRun ( SInt8 Mi26Nb, SInt32 RunNo, SInt32 TotEvNb, SInt32 EvNbPerFile, SInt32 FrameNbPerAcq, SInt8 DataTransferMode, char* DestDir, char* FileNamePrefix, SInt8 SaveToDisk, SInt8 SendOnEth, SInt8 SendOnEthPCent );)



FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32  EFRIO__FPrintRunContRec ( EFRIO__TRunCont* PtRec );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32  EFRIO__FPrintRunCont ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32  EFRIO__FPrintBoardConfRec ( EFRIO__TBoardConf* PtRec );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32  EFRIO__FPrintBoardConf ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32  EFRIO__FPrintBoardStatusRec ( EFRIO__TBoardStatus* PtRec );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32  EFRIO__FPrintBoardStatus ( SInt32 BoardId ) ;)

// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt32* EFRIO__FGetZsFFrameRawFields ( SInt32 AcqId, SInt8 Mi26Id, SInt32 FrameIdInAcq, UInt32* PtDest, SInt8 DestW32Sz );)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt32* EFRIO__FGetFrameFields ( SInt32 AcqId, SInt8 Mi26Id, SInt32 FrameIdInAcq, UInt32* PtDest, SInt8 DestW32Sz );)


FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardConf ( SInt32 BoardId, EFRIO__TBoardConf* PtSrc  );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetBoardConf ( SInt32 BoardId, EFRIO__TBoardConf* PtDest );)


FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__MI26_FFRioAcqDeserDataMi26 ( SInt8 Mi26Nb, SInt32 BoardId, UInt32* PtSrcW32AsPt, UInt32 PtSrcW32AsInt, SInt32 EltNb, SInt8 AcqStatus, SInt16 TrigStatus, UInt32 WaitMsAtEnd, SInt8 DataConvertMode, SInt8 TriggerHandlingMode, SInt16 EmuleMode );)

// Board conf fields access functions

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardConfBoardId ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardConfAsicName ( SInt32 BoardId, char* AsicName );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardConfAsicNb ( SInt32 BoardId, SInt32 AsicNb );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardConfReadoutMode ( SInt32 BoardId, SInt32 ReadoutMode );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardConfEmuleChannels ( SInt32 BoardId, SInt8 EmuleChannels );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardConfDataClkFrequency ( SInt32 BoardId, float DataClkFrequency );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardConfDmaHostSz ( SInt32 BoardId, UInt32 DmaHostSz );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardConfFrameNbPerAcq ( SInt32 BoardId, SInt32 FrameNbPerAcq );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardConfEnableExtraChannel ( SInt32 BoardId, SInt8 EnableExtraChannel );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardConfAcqNbPerTrig ( SInt32 BoardId, SInt32 AcqNbPerTrig );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardConfTriggerMode ( SInt32 BoardId, SInt8 TriggerMode );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardConfTriggerDetectTimeWindow ( SInt32 BoardId, UInt32 TriggerDetectTimeWindow );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardConfTriggerDetectOccurNb ( SInt32 BoardId, UInt32 TriggerDetectOccurNb );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardConfTimeStampRes ( SInt32 BoardId, UInt32 TimeStampRes );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardConfEnableTimeStamping ( SInt32 BoardId, SInt8 EnableTimeStamping );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardConfEnableTrigCnt ( SInt32 BoardId, SInt8 EnableTrigCnt );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardConfTagEventsStoredByDUT ( SInt32 BoardId, SInt8 TagEventsStoredByDUT );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardConfReadTluTrigCntEachNTrig ( SInt32 BoardId, UInt32 ReadTluTrigCntEachNTrig );)

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

// Board status fields access functions

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardStatusBoardId ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardStatusBoardPresent ( SInt32 BoardId, SInt8 BoardPresent );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardStatusFwLoaded ( SInt32 BoardId, SInt8 FwLoaded );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardStatusConfDone ( SInt32 BoardId, SInt8 ConfDone );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardStatusStatusCode ( SInt32 BoardId, SInt32 StatusCode );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardStatusStatusStr ( SInt32 BoardId, char* StatusStr );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardStatusErrorMsgList ( SInt32 BoardId, char* ErrorMsg, SInt32 Index );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardStatusLastErrorMsg ( SInt32 BoardId, char* LastErrorMsg );)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetBoardStatusBoardId ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt8  EFRIO__FGetBoardStatusBoardPresent ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt8 EFRIO__FGetBoardStatusFwLoaded ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt8 EFRIO__FGetBoardStatusConfDone ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FGetBoardStatusStatusCode ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, char* EFRIO__FGetBoardStatusStatusStr ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, char* EFRIO__FGetBoardStatusErrorMsgList ( SInt32 BoardId, SInt32 Index );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, char* EFRIO__FGetBoardStatusLastErrorMsg ( SInt32 BoardId );)


#ifndef APP_DLL_IMPORT_EXPORT
  #ifndef EUDET_FRIO_H
    #define EUDET_FRIO_H
    #endif
#endif


#endif