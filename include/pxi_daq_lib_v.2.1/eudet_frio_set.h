 
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


#ifndef EUDET_FRIO_SET_H

#include "func_header.def"


// 06/11/2010 -> 27


// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, char* DIU_FGetVersion ();)
// FHEAD ( SInt32 REF_FHello ();)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardConf ( SInt32 BoardId, EFRIO__TBoardConf* PtSrc );)
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
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardStatusBoardId ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardStatusBoardPresent ( SInt32 BoardId, SInt8 BoardPresent );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardStatusFwLoaded ( SInt32 BoardId, SInt8 FwLoaded );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardStatusConfDone ( SInt32 BoardId, SInt8 ConfDone );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardStatusStatusCode ( SInt32 BoardId, SInt32 StatusCode );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardStatusStatusStr ( SInt32 BoardId, char* StatusStr );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardStatusErrorMsgList ( SInt32 BoardId, char* ErrorMsg, SInt32 Index );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetBoardStatusLastErrorMsg ( SInt32 BoardId, char* LastErrorMsg );)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetCmdRun ( SInt32 Cmd );)


#ifndef APP_DLL_IMPORT_EXPORT
  #ifndef EUDET_FRIO_SET_H
    #define EUDET_FRIO_SET_H
    #endif
#endif


#endif