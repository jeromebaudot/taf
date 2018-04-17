 
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


// 06/11/2010 -> 21

// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, char* DIU_FGetVersion ();)
// FHEAD ( SInt32 REF_FHello ();)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FCpyAcq ( UInt8* PtDest, SInt32 MaxDestSz, SInt32 AcqSz );)


FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FBegin    ( SInt8 ErrorLogLvl, char* ErrorLogFile, SInt8 MsgLogLvl, char* MsgLogFile );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FBeginExt ( SInt8 MapsName, SInt8 ErrorLogLvl, char* ErrorLogFile, SInt8 MsgLogLvl, char* MsgLogFile );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FEnd ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt32* EFRIO__FTrfData ( SInt8 CmdGetPtrCpyAlloc, UInt32 AllocW32Sz, UInt32* PtSrcW32, UInt32 SrcW32Sz );)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FBuildFrameListFromAcq ( SInt32 FrameNb, void* PtAcqData, EFRIO__TFrameList* PtList );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FEmuleBegin ( SInt8 RunInLabview );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FEmuleEnd ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt32* EFRIO__FEmuleReadDRam ( SInt8 Cmd );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSetFrameIdInTriggerRec ( SInt32 FrameId, SInt32 TrigNb, EFRIO__TTriggerRec* PtRec );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FConfRun ( SInt8 Mi26Nb, SInt32 RunNo, SInt32 TotEvNb, SInt32 EvNbPerFile, SInt32 FrameNbPerAcq, SInt8 DataTransferMode, SInt8 TrigMode, char* DestDir, char* FileNamePrefix, SInt8 SaveToDisk, SInt8 SendOnEth, SInt8 SendOnEthPCent, char* JtagFileName );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FConfRunExt ( SInt8 MapsName, SInt8 MapsNb, SInt32 RunNo, SInt32 TotEvNb, SInt32 EvNbPerFile, SInt32 FrameNbPerAcq, SInt8 DataTransferMode, SInt8 TrigMode, char* DestDir, char* FileNamePrefix, SInt8 SaveToDisk, SInt8 SendOnEth, SInt8 SendOnEthPCent, char* JtagFileName );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FStartSavingOnFile ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSaveAcqOnFile ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FStopSavingOnFile  ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, char*  EFRIO__FTluTrigger2Str ( UInt32 Trig, char* DestStr, SInt32 MaxDestSz );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, char*  EFRIO__FTimeStamp2Str  ( UInt32 Ts, char* DestStr, SInt32 MaxDestSz );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__MI26_FChkAcqIphcMode ( SInt32 PrevErrCnt, SInt8 Verbose );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt32 EFRIO__MI26_FChkAcqEudetMode ( SInt32 PrevErrCnt, SInt8 Verbose );)
  
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__MI26_FFRioAcqDeserDataIphcMode1Mi26   ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__MI26_FFRioAcqDeserDataIphcMode6Mi26   ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet1Mode1Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet1Mode6Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet2Mode1Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet2Mode6Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet3Mode6Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet3Mode1Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__MI26_FFRioAcqDeserDataMi26            ( SInt8 Mi26Nb, SInt32 BoardId, UInt32* PtSrcW32AsPt, UInt32 PtSrcW32AsInt, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, UInt32 WaitMsAtEnd, SInt8 DataConvertMode, SInt8 TriggerHandlingMode, SInt16 EmuleMode );)


FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FJtagLoadFile ( char* FileName );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FJtagReset ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FJtagLoadChip ();)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FJtagResetAndLoadChip ( );)


FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FJtagStartChip ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FHwStartChip ( SInt32 SpareS32Par );)


FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__MI26_FJtagLoadFile ( char* FileName );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__MI26_FJtagLoadDefFile ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__MI26_FJtagReset ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__MI26_FJtagLoadChip ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__MI26_FJtagStartChip ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__MI26_FHwStartChip ( SInt32 SpareS32Par );)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FTestOnDataStartStop ( SInt8 StartAndTestType, SInt8 MapsNbToTest, SInt8 PrintLvl );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FTestOnDataResetErrCnt ();)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FRestartDaqWaitOnDutDaq ( SInt32 TimeBeforeWaitMs, SInt8 WaitOnDutDaq, SInt32 TimeToWaitOnDutDaqMs );)



#ifndef APP_DLL_IMPORT_EXPORT
  #ifndef EUDET_FRIO_H
    #define EUDET_FRIO_H
    #endif
#endif


#endif
