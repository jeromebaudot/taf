
/*******************************************************************************
File      : x:\lib\win\eudet_frio\eudet_frio.c
Goal      : Functions of flex rio board library for EUDET Mimosa 26 DAQ
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


#ifndef EUDET_FRIO_PRINT_C
#define EUDET_FRIO_PRINT_C



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : char* EFRIO__FTluTrigger2Str ( UInt32 Trig, char* DestStr, SInt32 MaxDestSz )
:
Goal      : Convert TLU trigger info record to string for print or display
:
Inputs    : Trig      - Source trigger record ( it's only a W32 )
: DestStr   - Destination string
: MaxDestSz - Destination string size
:
Ouputs    : The trigger as a string in an human readable format
:
Globals   :
:
Remark    : If DestStr = NULL or is too small, a pointer to static local variable is
: returned. But please do a copy of the string, because if you use via the
: pointer, string content may / will change at next function call !
:
Level     :
Date      : 06/11/2010
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

char* EFRIO__FTluTrigger2Str ( UInt32 Trig, char* DestStr, SInt32 MaxDestSz )
{
  static char VStr [30];
  EFRIO__TTluTrigger VTrig;
  
  VTrig.W32 = Trig;
  
  // Convert in string
  
  sprintf ( VStr, "F%.4d - T%.4d", VTrig.F.FrameIdInAcq, VTrig.F.TrigCnt );
  
  // Fill DestStr param if possible
  
  if ( (DestStr != NULL) && (MaxDestSz >= 30) ) {
    sprintf (DestStr, "%s", VStr );
    return (DestStr);
  }
  
  // Return local var if no valid DestStr
  
  return (VStr);
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : char* EFRIO__FTimeStamp2Str ( UInt32 Ts, char* DestStr, SInt32 MaxDestSz )
:
Goal      : Convert Flex RIO trigger / ime stamp info record to string for print or display
:
Inputs    : Ts        - Source time stamp record ( it's only a W32 )
: DestStr   - Destination string
: MaxDestSz - Destination string size
:
Ouputs    : The trigger / timestamp as a string in an human readable format
:
Globals   :
:
Remark    : If DestStr = NULL or is too small, a pointer to static local variable is
: returned. But please do a copy of the string, because if you use via the
: pointer, string content may / will change at next function call !
:
Level     :
Date      : 06/11/2010
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

char* EFRIO__FTimeStamp2Str ( UInt32 Ts, char* DestStr, SInt32 MaxDestSz )
{
  static char VStr [30];
  EFRIO__TFlexRioTimeStamp1 VTs;
  
  VTs.W32 = Ts;
  
  // Convert in string
  
  sprintf ( VStr, "F%.4d - L%.4d", VTs.F.Mi26Frame, VTs.F.Mi26Line );
  
  // Fill DestStr param if possible
  
  if ( (DestStr != NULL) && (MaxDestSz >= 30) ) {
    sprintf ( DestStr, "%s", VStr );
    return (DestStr);
  }
  
  // Return local var if no valid DestStr
  
  return (VStr);
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FPrintRunContRec ( EFRIO__TRunCont* PtRec )
          :
Goal      : Print run context record in log file
          :
Inputs    : PtRec - Pointer on the record
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if PtRec = NULL
          :
Globals   :
          :
Remark    :
          :
Level     :
Date      : 09/08/2010
Rev       : 21/02/2011
          : - Print new fields ParDaqVersion, ParMapsName
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FPrintRunContRec ( EFRIO__TRunCont* PtRec ) {

  err_retnull ( PtRec, (ERR_OUT,"Abort : PtRec == NULL") );

  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "Run context record" ));
  msg (( MSG_OUT, "============================================================" ));
//  msg (( MSG_OUT, "ParDaqVersion             = %.4d", PtRec->ParDaqVersion ));
//  msg (( MSG_OUT, "ParMapsName               = %.4d", PtRec->ParMapsName ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "ParMi26Nb                 = %.4d", PtRec->ParMi26Nb ));
  msg (( MSG_OUT, "ParFrameNbPerAcq          = %.4d", PtRec->ParFrameNbPerAcq ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "ParRunNo                  = %.4d", PtRec->ParRunNo ));
  msg (( MSG_OUT, "ParTotEvNb                = %.4d", PtRec->ParTotEvNb ));
  msg (( MSG_OUT, "ParEvNbPerFile            = %.4d", PtRec->ParEvNbPerFile ));
  msg (( MSG_OUT, "ParDataTransferMode       = %.4d", PtRec->ParDataTransferMode ));
  msg (( MSG_OUT, "ParTrigMode               = %.4d", PtRec->ParTrigMode ));
  msg (( MSG_OUT, "ParSaveOnDisk             = %.4d", PtRec->ParSaveOnDisk ));
  msg (( MSG_OUT, "ParSendOnEth              = %.4d", PtRec->ParSendOnEth ));
  msg (( MSG_OUT, "ParSendOnEthPCent         = %.4d", PtRec->ParSendOnEthPCent ));
  msg (( MSG_OUT, "ParDestDir                = %s"  , PtRec->ParDestDir ));
  msg (( MSG_OUT, "ParFileNamePrefix         = %s"  , PtRec->ParFileNamePrefix ));  
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "ParJtagFileName           = %s ", PtRec->ParJtagFileName ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));  
//  msg (( MSG_OUT, "InfMi26FrameSzFromFlexRio = %.4d", PtRec->InfMi26FrameSzFromFlexRio ));
  msg (( MSG_OUT, "InfZsFFrameRawBuffSz      = %.4d", PtRec->InfZsFFrameRawBuffSz ));
  msg (( MSG_OUT, "InfFrameBuffSz            = %.4d", PtRec->InfFrameBuffSz ));
  msg (( MSG_OUT, "InfConfFileName           = %s"  , PtRec->InfConfFileName        ));
  msg (( MSG_OUT, "InfDataFileName           = %s"  , PtRec->InfDataFileName        ));
  msg (( MSG_OUT, "InfSaveDataOnDiskRunning  = %.4d", PtRec->InfSaveDataOnDiskRunning ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "CmdRun                    = %.4d", PtRec->CmdRun           ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "ResAcqCnt                 = %.4d", PtRec->ResAcqCnt           ));
  msg (( MSG_OUT, "ResFrameCnt               = %.4d", PtRec->ResFrameCnt         ));
  msg (( MSG_OUT, "ResEventCnt               = %.4d", PtRec->ResEventCnt         ));
  msg (( MSG_OUT, "ResDataRateMBytesPerSec   = %.3f", PtRec->ResDataRateMBytesPerSec ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "PtZsFFrameRaw             = %.8x", PtRec->PtZsFFrameRaw ));
  msg (( MSG_OUT, "PtFrame                   = %.8x", PtRec->PtFrame ));
  msg (( MSG_OUT, "============================================================" ));

  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FPrintRunCont ()
          :
Goal      : Print lib run context record in log file
          :
Inputs    : None
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if &EFRIO__VGContext.RunCont = NULL => But it's not possible 
          :
Globals   :
          :
Remark    : Call EFRIO__FPrintRunContRec (&EFRIO__VGContext.RunCont)
          :
Level     :
Date      : 09/08/2010
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FPrintRunCont () {

  return ( EFRIO__FPrintRunContRec (&EFRIO__VGContext.RunCont) ); 
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FPrintBoardConfRec ( EFRIO__TBoardConf* PtRec )
          :
Goal      : Print board conf record in log file
          :
Inputs    : PtRec - Pointer on the record
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if PtRec = NULL
          :
Globals   :
          :
Remark    :
          :
Level     :
Date      : 09/08/2010
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */
  

SInt32 EFRIO__FPrintBoardConfRec ( EFRIO__TBoardConf* PtRec ) {

  err_retnull ( PtRec, (ERR_OUT,"Abort : PtRec == NULL") );

  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "Board conf record" ));
  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "BoardId                 = %.4d", PtRec->BoardId ));
  msg (( MSG_OUT, "AsicName                = %s", PtRec->AsicName ));
  msg (( MSG_OUT, "AsicNb                  = %.4d", PtRec->AsicNb ));
  msg (( MSG_OUT, "ReadoutMode             = %.4d", PtRec->ReadoutMode ));
  msg (( MSG_OUT, "DataClkFrequency        = %2.f", PtRec->DataClkFrequency ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "DmaHostSz               = %.4d", PtRec->DmaHostSz ));
  msg (( MSG_OUT, "FrameNbPerAcq           = %.4d", PtRec->FrameNbPerAcq ));
  msg (( MSG_OUT, "EnableExtraChannel      = %.4d", PtRec->EnableExtraChannel ));
  msg (( MSG_OUT, "AcqNbPerTrig            = %.4d", PtRec->AcqNbPerTrig ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "TriggerMode             = %.4d", PtRec->TriggerMode ));
  msg (( MSG_OUT, "TriggerDetectTimeWindow = %.4d", PtRec->TriggerDetectTimeWindow ));
  msg (( MSG_OUT, "TriggerDetectOccurNb    = %.4d", PtRec->TriggerDetectOccurNb ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "TimeStampRes            = %.4d", PtRec->TimeStampRes ));
  msg (( MSG_OUT, "EnableTimeStamping      = %.4d", PtRec->EnableTimeStamping ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "EnableTrigCnt           = %.4d", PtRec->EnableTrigCnt ));
  msg (( MSG_OUT, "TagEventsStoredByDUT    = %.4d", PtRec->TagEventsStoredByDUT ));
  msg (( MSG_OUT, "ReadTluTrigCntEachNTrig = %.4d", PtRec->ReadTluTrigCntEachNTrig ));
  msg (( MSG_OUT, "============================================================" ));
  

  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FPrintBoardConf ( SInt32 BoardId )
          :
Goal      : Print lib board conf record of BoardId in log file
          :
Inputs    : BoardId - Board identifier
          : 
Ouputs    : The function returns
          :  0 if ok
          : -1 if BoardId is not valid
          :
Globals   :
          :
Remark    : Call EFRIO__FPrintBoardConfRec ( &EFRIO__VGContext.ABoardsConf[BoardId] )
          :
Level     :
Date      : 09/08/2010
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FPrintBoardConf ( SInt32 BoardId ) {

  EFRIO__CHK_BOARD_ID (BoardId);

  return ( EFRIO__FPrintBoardConfRec ( &EFRIO__VGContext.ABoardsConf[BoardId] ) ); 
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FPrintBoardStatusRec ( EFRIO__TBoardStatus* PtRec )
          :
Goal      : Print board status record in log file
          :
Inputs    : PtRec - Pointer on the record
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if PtRec = NULL
          :
Globals   :
          :
Remark    :
          :
Level     :
Date      : 09/08/2010
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FPrintBoardStatusRec ( EFRIO__TBoardStatus* PtRec ) {

  err_retnull ( PtRec, (ERR_OUT,"Abort : PtRec == NULL") );
  
  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "Board status record" ));
  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "BoardId                    = %.4d", PtRec->BoardId ));
  msg (( MSG_OUT, "BoardPresent               = %.4d", PtRec->BoardPresent ));
  msg (( MSG_OUT, "FwLoaded                   = %.4d", PtRec->FwLoaded ));
  msg (( MSG_OUT, "ConfDone                   = %.4d", PtRec->ConfDone ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "StatusCode                 = %.4d", PtRec->StatusCode ));
  msg (( MSG_OUT, "StatusStr                  = %.4d", PtRec->StatusStr ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "ErrorMsgList               = %s", PtRec->ErrorMsgList ));
  msg (( MSG_OUT, "LastErrorMsg               = %s", PtRec->LastErrorMsg ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "RegDmaHostSz               = %.4d", PtRec->RegDmaHostSz ));
  msg (( MSG_OUT, "RegFrameNbPerAcq           = %.4d", PtRec->RegFrameNbPerAcq ));
  msg (( MSG_OUT, "RegEnableExtraChannel      = %.4d", PtRec->RegEnableExtraChannel ));
  msg (( MSG_OUT, "RegAcqNbPerTrig            = %.4d", PtRec->RegAcqNbPerTrig ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "RegTriggerMode             = %.4d", PtRec->RegTriggerMode ));
  msg (( MSG_OUT, "RegTriggerDetectTimeWindow = %.4d", PtRec->RegTriggerDetectTimeWindow ));
  msg (( MSG_OUT, "RegTriggerDetectOccurNb    = %.4d", PtRec->RegTriggerDetectOccurNb ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "RegTimeStampRes            = %.4d", PtRec->RegTimeStampRes ));
  msg (( MSG_OUT, "RegEnableTimeStamping      = %.4d", PtRec->RegEnableTimeStamping ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "RegEnableTrigCnt           = %.4d", PtRec->RegEnableTrigCnt ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "RegTagEventsStoredByDUT    = %.4d", PtRec->RegTagEventsStoredByDUT ));
  msg (( MSG_OUT, "RegReadTluTrigCntEachNTrig = %.4d", PtRec->RegReadTluTrigCntEachNTrig ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "RegTimeStamp               = %.4d", PtRec->RegTimeStamp ));
  msg (( MSG_OUT, "RegTrigCnt                 = %.4d", PtRec->RegTrigCnt ));
  msg (( MSG_OUT, "RegTluTrigCnt              = %.4d", PtRec->RegTluTrigCnt ));
  msg (( MSG_OUT, "============================================================" ));
  
  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FPrintBoardStatus ( SInt32 BoardId )
          :
Goal      : Print lib board status record of BoardId in log file
          :
Inputs    : BoardId - Board identifier
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if BoardId is not valid
          :
Globals   :
          :
Remark    : Call EFRIO__FPrintBoardStatusRec ( &EFRIO__VGContext.ABoardsStatus[BoardId] )
          :
Level     :
Date      : 09/08/2010
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FPrintBoardStatus ( SInt32 BoardId ) {

  EFRIO__CHK_BOARD_ID (BoardId);
    
  return ( EFRIO__FPrintBoardStatusRec ( &EFRIO__VGContext.ABoardsStatus[BoardId] ) );
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FPrintAcqEmulRec ( EFRIO__TAcqEmul* PtRec )
          :
Goal      : Print acquisition emulation context record in log file
          :
Inputs    : PtRec - Pointer on the record
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if PtRec = NULL
          :
Globals   :
          :
Remark    :
          :
Level     :
Date      : 31/10/2010
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FPrintAcqEmulRec ( EFRIO__TAcqEmul* PtRec ) {
  
  err_retnull ( PtRec, (ERR_OUT,"Abort : PtRec == NULL") );

  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "Acquistion emulation record" ));
  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "ParAcqCycleMs       = %.4d", PtRec->ParAcqCycleMs ));
  msg (( MSG_OUT, "ParEmuleDRamReadMs  = %.4d", PtRec->ParEmuleDRamReadMs ));
  msg (( MSG_OUT, "ParEmuleFunctNo     = %.4d", PtRec->ParEmuleFunctNo ));
  msg (( MSG_OUT, "InfEmuleFuncCmt     = %s"  , PtRec->InfEmuleFuncCmt ));
  msg (( MSG_OUT, "ParRandomDataSz     = %d"  , PtRec->ParRandomDataSz ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "H [0]=%.8x [1]=%.8x [2]=%.8x [3]=%.8x [4]=%.8x [5]=%.8x", PtRec->ParAHeader[0] , PtRec->ParAHeader[1] , PtRec->ParAHeader[2] , PtRec->ParAHeader[3] , PtRec->ParAHeader[4] , PtRec->ParAHeader[5] ));
  msg (( MSG_OUT, "T [0]=%.8x [1]=%.8x [2]=%.8x [3]=%.8x [4]=%.8x [5]=%.8x", PtRec->ParATrailer[0], PtRec->ParATrailer[1], PtRec->ParATrailer[2], PtRec->ParATrailer[3], PtRec->ParATrailer[4], PtRec->ParATrailer[5] ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "ParTrigNbPerFrame            = %.4d", PtRec->ParTrigNbPerFrame ));
  msg (( MSG_OUT, "ParTrigOnOneFrameOverN       = %.4d", PtRec->ParTrigOnOneFrameOverN ));
  msg (( MSG_OUT, "ParTrigOnNConsecutiveFrames  = %.4d", PtRec->ParTrigOnNConsecutiveFrames ));
  msg (( MSG_OUT, "Trig [0]=%.4d [1]=%.4d [2]=%.4d [Last]=%.4d", PtRec->ParATrig[0], PtRec->ParATrig[1], PtRec->ParATrig[2], PtRec->ParATrig[3] ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "InfDRamSzMb         = %.4d", PtRec->InfDRamSzMb ));
  msg (( MSG_OUT, "InfDRamSz           = %.4d", PtRec->InfDRamSz ));
  msg (( MSG_OUT, "InfDRamPtr          = %.8x", PtRec->InfDRamPtr ));
  msg (( MSG_OUT, "InfExtraChan        = %.4d", PtRec->InfExtraChan ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "ResAcqFunctRetCode  = %.4d", PtRec->ResAcqFunctRetCode ));
  msg (( MSG_OUT, "ResAcqCnt           = %.4d", PtRec->ResAcqCnt ));
  msg (( MSG_OUT, "ResEvCnt            = %.4d", PtRec->ResEvCnt ));
  msg (( MSG_OUT, "============================================================" ));
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FPrintAcqEmul ()
          : 
Goal      : Print lib acquisition emulation context record in log file
          :      
Inputs    : None
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if &EFRIO__VGContext.AcqEmul = NULL => But it's not possible
          :
Globals   :
          :
Remark    : Call EFRIO__FPrintAcqEmulRec ( &EFRIO__VGContext.AcqEmul )
          :
Level     :
Date      : 06/11/2010
Doc date  : 06/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FPrintAcqEmul () {
  
  return ( EFRIO__FPrintAcqEmulRec ( &EFRIO__VGContext.AcqEmul ) );
}
  

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FPrintFrCheckRec ( EFRIO_TFrCheck* PtRec )
          :
Goal      : Print frame check context record in log file
          :
Inputs    : PtRec - Pointer on the record
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if PtRec = NULL
          :
Globals   :
          :
Remark    :
          :
Level     :
Date      : 31/10/2010
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__FPrintFrCheckRec ( EFRIO__TFrCheck* PtRec ) {
  
  err_retnull ( PtRec, (ERR_OUT,"Abort : PtRec == NULL") );
  
  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "Frames check record" ));
  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "ParAcqNo        = %.4d", PtRec->ParAcqNo ));
  msg (( MSG_OUT, "ParFrNo         = %.4d", PtRec->ParFrNo ));
  msg (( MSG_OUT, "ParChkMode      = %.4d", PtRec->ParChkMode ));
  msg (( MSG_OUT, "ParFrPrintLevel = %.4d", PtRec->ParFrPrintLevel ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "InfMi26Nb     = %.4d", PtRec->InfMi26Nb ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "H  [0]=%.8x [1]=%.8x [2]=%.8x [3]=%.8x [4]=%.8x [5]=%.8x", PtRec->ResAHeader[0]    , PtRec->ResAHeader[1]    , PtRec->ResAHeader[2]    , PtRec->ResAHeader[3]    , PtRec->ResAHeader[4]    , PtRec->ResAHeader[5]     ));
  msg (( MSG_OUT, "T  [0]=%.8x [1]=%.8x [2]=%.8x [3]=%.8x [4]=%.8x [5]=%.8x", PtRec->ResATrailer[0]   , PtRec->ResATrailer[1]   , PtRec->ResATrailer[2]   , PtRec->ResATrailer[3]   , PtRec->ResATrailer[4]   , PtRec->ResATrailer[5]    ));
  msg (( MSG_OUT, "FC [0]=%.8d [1]=%.8d [2]=%.8d [3]=%.8d [4]=%.8d [5]=%.8d", PtRec->ResAFrameCnt[0]  , PtRec->ResAFrameCnt[1]  , PtRec->ResAFrameCnt[2]  , PtRec->ResAFrameCnt[3]  , PtRec->ResAFrameCnt[4]  , PtRec->ResAFrameCnt[5]   ));
  msg (( MSG_OUT, "DL [0]=%.8d [1]=%.8d [2]=%.8d [3]=%.8d [4]=%.8d [5]=%.8d", PtRec->ResADataLenght[0], PtRec->ResADataLenght[1], PtRec->ResADataLenght[2], PtRec->ResADataLenght[3], PtRec->ResADataLenght[4], PtRec->ResADataLenght[5] ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "ResTrigNb      = %.4d", PtRec->ResTrigNb ));
  msg (( MSG_OUT, "TRG [0]=%.8d [1]=%.8d [3]=%.8d [Last]=%.8d", PtRec->ResATrig[0], PtRec->ResATrig[1], PtRec->ResATrig[2], PtRec->ResATrig[3] ));
  msg (( MSG_OUT, "============================================================" ));
  
  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FPrintFrCheck ()
          :
Goal      : Print lib frame check context record in log file
          :
Inputs    : None
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if &EFRIO__VGContext.FrCheck = NULL => But it's not possible
          :
Globals   :
          :
Remark    : Call EFRIO__FPrintFrCheckRec ( &EFRIO__VGContext.FrCheck )
          :
Level     :
Date      : 06/11/2010
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__FPrintFrCheck () {
  
  return ( EFRIO__FPrintFrCheckRec ( &EFRIO__VGContext.FrCheck ) );
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FPrintFrameData ( EFRIO__TFrame* PtRec, SInt8 PrintLevel )
          :
Goal      : print one frame content in log file
          :
Inputs    : PtRec       - Pointer on the record
          :
          : PrintLevel  - 0   -> Print nothing
          :             - > 0 -> print state list for each line
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if PtRec = NULL
          :
Globals   :
          :
Remark    :
          :
Level     :
Date      : 22/12/2010
Rev       : 30/12/2010
          : - Add handling of N Mimosa 26
          :
Doc date  : 22/12/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FPrintFrameData ( EFRIO__TFrame* PtRec, SInt8 PrintLevel ) {
  
  EFRIO__TFrameHeader* VPtHead;
  EFRIO__TFrameData*   VPtData;
  
  UInt16               VOneMapsSzW16;
  UInt16               VDataW16Length;
  UInt16               VLastW16;
  SInt32               ViSrcW16;
  UInt16*              VPtSrcW16;
  MI26__TStatesLine    VStatesLine;
  MI26__TState         VState;
  SInt16               ViMi26;
  SInt16               ViStatesLine;
  SInt8                ViState;
  SInt8                VStatesNbPerLine;
  char                 VStrState[255];
  char                 VStrLine[255];
  
  
  
  err_retnull ( PtRec, (ERR_OUT,"PtRec == NULL !") );
  
  if ( PrintLevel == 0 ) {
    return (0);
  }
  
  // -------------------------------------
  // Init pointers on TFrame sub records
  // -------------------------------------
  
  VPtHead = &PtRec->Header;
  VPtData = &PtRec->Data;
    
  VOneMapsSzW16 = VPtData->OneMapsSz / 2;


  for ( ViMi26=0; ViMi26 < VPtHead->MapsNb ; ViMi26++ ) {

    VPtSrcW16       = (UInt16*) ( (UInt16*) VPtData->ADataW32 + ( ViMi26  * VOneMapsSzW16 ) );
    VDataW16Length  = VPtHead->AMapsDataLength[ViMi26] / 2;
    ViSrcW16        = 0;
    
    msg (( MSG_OUT, "===================================================================================" ));
    msg (( MSG_OUT, "                                  Mimosa 26 No %2d                                 ", ViMi26 ));
    msg (( MSG_OUT, "===================================================================================" ));
    
    
    if ( VDataW16Length != 0 ) {
      
      // -------------------------------------------------------------------------------------------------
      // Odd W16 nb handling !
      //
      // It can seem strange that this can be done by processing one W16 less than total data length in all
      // cases, this is due to data processing method used in loop, read explanation below if needed.
      // -------------------------------------------------------------------------------------------------
      // If the total W16 number is odd, Mi26 add one more bad W16 to get an even W16 number.
      // This bad W16 will be seen as a StatesLine field followed by NO state because it is the last W16.
      // Therefore if at the beginning of the while loop there is only one W16 to process, this W16 is the
      // bad one, because it is a StateLines field followed by no states. In others words, if the index of
      // the W16 at the beginning of loop is the index of last W16 this W16 is the bad one which must be
      // rejected, we must not enter the loop. In normal case, even W16 number, after processing of last
      // state of last line the index of W16 equal W16 number, therefore is > of index of last W16, and
      // we don't enter the loop.
      
      VLastW16 = VDataW16Length - 1;
      
      ViStatesLine  = 0;
      
      while ( ViSrcW16 < VLastW16 ) { // Odd W16 nb handling => Don't process last W16

        // Copy StatesLine field
        
        VStatesLine.W16  = VPtSrcW16[ViSrcW16];
        VStatesNbPerLine = VStatesLine.F.StateNb;
        
        sprintf ( VStrLine, "Mi26 %2d Line %4d - %d state(s) - %d Ovf : ", ViMi26, VStatesLine.F.LineAddr, VStatesLine.F.StateNb, VStatesLine.F.Ovf );
        
        if ( (PrintLevel != 0) ) {
          msg (( MSG_OUT, "%s", VStrLine ));
        }
        
        ++ViSrcW16;
        
        // Copy states
        
        for ( ViState=0; ViState < VStatesNbPerLine; ViState++ ) {
          VState.W16 = VPtSrcW16[ViSrcW16];
          
          sprintf ( VStrState, "[Col %4d - %1d pixel(s)] ", VState.F.ColAddr, VState.F.HitNb + 1 );
          // strcat  ( VStrLine , VStrState );

          if ( (PrintLevel != 0) ) {
            msg (( MSG_OUT, "%s", VStrState ));
          }
        
          ++ViSrcW16;
        }
        
        // if ( (PrintLevel != 0) ) {
        //   msg (( MSG_OUT, "%s", VStrLine ));
        // }
        
        ++ViStatesLine;          
                
                
      } // End while
      
    } // End if ( VDataW16Length != 0 )
    
        
  } // End for ( ViMi26 )

}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FPrintFrameRec ( EFRIO__TFrame* PtRec, SInt8 PrintLevel )
          :
Goal      : print one frame content in log file
          :
Inputs    : PtRec       - Pointer on the record
          :
          : PrintLevel  - 0 -> Print nothing
          :             - 1 -> Print AcqId & FrId
          :             - 2 -> Print AcqId, FrId ... Mi26 header, trailer ... + Trig nb
          :             - 3 -> Print AcqId, FrId ... Mi26 header, trailer ... + Trig nb + Data
          :             - 4 -> Print trigger list
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if PtRec = NULL
          :
Globals   :
          :
Remark    :
          :
Level     :
Date      : 06/11/2010
Rev       : 21/02/2011
          : - Print new field DaqVersion
          : 23/06/2013
          : - Print header, frame cnt, data sz, header of MAPS No 6 (debug time-stamping)
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FPrintFrameRec ( EFRIO__TFrame* PtRec, SInt8 PrintLevel ) {
  
  EFRIO__TFrameHeader* VPtHead;
  EFRIO__TFrameData*   VPtData;
  EFRIO__TTriggerRec*  VPtTrig;
  SInt16               ViTrig;
  char                 VStrTrig[30];
  char                 VStrTs  [30];
  
  
  err_retnull ( PtRec, (ERR_OUT,"PtRec == NULL !") );
  
  if ( PrintLevel == 0 ) {
    return (0);
  }
  
  // -------------------------------------
  // Init pointers on TFrame sub records
  // -------------------------------------
  
  VPtHead = &PtRec->Header;
  VPtData = &PtRec->Data;
  
  // Trigger record follows the data record which has a VARIABLE size
    
  VPtTrig = (EFRIO__TTriggerRec*) ( (UInt8*) PtRec + PtRec->TrigRecOffset );

  // -------------------------------------
  // Print TFrame fields
  // -------------------------------------

  if ( PrintLevel == 1 ) {
    msg (( MSG_OUT, "==============================================" ));
    msg (( MSG_OUT, "AcqId = %.4d - FrameIdInAcq = %.4d - TrigNb = %.4d  [D]", VPtHead->AcqId, VPtHead->FrameIdInAcq, VPtTrig->TrigNb ));
    msg (( MSG_OUT, "Trigger line [0]=%.4d [1]=%.4d [2]=%.4d ", VPtHead->AMapsTrigInfo[0] & 0x3FF, VPtHead->AMapsTrigInfo[1] & 0x3FF, VPtHead->AMapsTrigInfo[2] & 0x3FF));
    msg (( MSG_OUT, "Trigger TS   [0]=%.4d [1]=%.4d [2]=%.4d ", VPtHead->AMapsTrigInfo[0] >> 10, VPtHead->AMapsTrigInfo[1] >> 10, VPtHead->AMapsTrigInfo[2]  >> 10));
  
    for ( ViTrig=0; ViTrig < VPtTrig->TrigNb; ViTrig++ ) {
      EFRIO__FTluTrigger2Str ( VPtTrig->ATrig[(2 * ViTrig)]    , VStrTrig, 30 /* MaxDestSz */ );
      EFRIO__FTimeStamp2Str  ( VPtTrig->ATrig[(2 * ViTrig) + 1], VStrTs  , 30 /* MaxDestSz */ );
      
      msg (( MSG_OUT, "T.[%.3d] Trig = %s - Ts = %s", ViTrig, VStrTrig, VStrTs ));
    }
    
    msg (( MSG_OUT, "" ));
    
    return (0);
  }
  

  msg (( MSG_OUT, "==============================================" ));
#ifdef EFRIO__FRAME_TAGS_ENABLE
  msg (( MSG_OUT, "Tag            = %.8X [H]", PtRec->Tag ));
#endif
  msg (( MSG_OUT, "DaqVersion     = %.4d [D]", PtRec->DaqVersion ));
  msg (( MSG_OUT, "TotSz          = %.4d [D]", PtRec->TotSz ));
  msg (( MSG_OUT, "TrigRecOffset  = %.4d [D]", PtRec->TrigRecOffset ));
  msg (( MSG_OUT, "----------------------------------------------" ));
#ifdef EFRIO__FRAME_TAGS_ENABLE
  msg (( MSG_OUT, "H.Tag          = %.8X [H]", VPtHead->Tag ));
#endif
  msg (( MSG_OUT, "H.AcqStatus    = %.4d [D]", VPtHead->AcqStatus ));
  msg (( MSG_OUT, "H.TrigStatus   = %.4d [D]", VPtHead->TrigStatus ));  
  msg (( MSG_OUT, "H.AcqId        = %.4d [D]", VPtHead->AcqId ));
  msg (( MSG_OUT, "H.FrameIdInAcq = %.4d [D]", VPtHead->FrameIdInAcq ));
  msg (( MSG_OUT, "H.MapsName     = %.4d [D]", VPtHead->MapsName ));
  msg (( MSG_OUT, "H.MapsNb       = %.4d [D]", VPtHead->MapsNb ));
  msg (( MSG_OUT, "----------------------------------------------" ));
  msg (( MSG_OUT, "H.Header   [0]=%.8X [1]=%.8X [2]=%.8X [3]=%.8X [4]=%.8X [5]=%.8X [6]=%.8X [7]=%.8X"      , VPtHead->AMapsHeader[0]    , VPtHead->AMapsHeader[1]    , VPtHead->AMapsHeader[2]    , VPtHead->AMapsHeader[3]    , VPtHead->AMapsHeader[4]    , VPtHead->AMapsHeader[5], VPtHead->AMapsHeader[6], VPtHead->AMapsHeader[7] ));
  msg (( MSG_OUT, "H.FrCnt    [0]=%8d [1]=%8d [2]=%8d [3]=%8d [4]=%8d [5]=%8d [6]=%8d [7]=%8d", VPtHead->AMapsFrameCnt[0]  , VPtHead->AMapsFrameCnt[1]  , VPtHead->AMapsFrameCnt[2]  , VPtHead->AMapsFrameCnt[3]  , VPtHead->AMapsFrameCnt[4]  , VPtHead->AMapsFrameCnt[5], VPtHead->AMapsFrameCnt[6], VPtHead->AMapsFrameCnt[7] ));
  msg (( MSG_OUT, "H.DataSz   [0]=%8d [1]=%8d [2]=%8d [3]=%8d [4]=%8d [5]=%8d [6]=%8d [7]=%8d", VPtHead->AMapsDataLength[0], VPtHead->AMapsDataLength[1], VPtHead->AMapsDataLength[2], VPtHead->AMapsDataLength[3], VPtHead->AMapsDataLength[4], VPtHead->AMapsDataLength[5], VPtHead->AMapsDataLength[6], VPtHead->AMapsDataLength[7] ));
  msg (( MSG_OUT, "H.Trailer  [0]=%.8X [1]=%.8X [2]=%.8X [3]=%.8X [4]=%.8X [5]=%.8X [6]=%.8X [7]=%.8X"      , VPtHead->AMapsTrailer[0]   , VPtHead->AMapsTrailer[1]   , VPtHead->AMapsTrailer[2]   , VPtHead->AMapsTrailer[3]   , VPtHead->AMapsTrailer[4]   , VPtHead->AMapsTrailer[5], VPtHead->AMapsTrailer[6], VPtHead->AMapsTrailer[7] ));
  msg (( MSG_OUT, "----------------------------------------------" ));
  msg (( MSG_OUT, "H.TriggerNb    = %.4d [D]", VPtHead->TriggerNb ));
  // msg (( MSG_OUT, "H.TrigInfo [0]=%.8X [1]=%.8X [2]=%.8X ", VPtHead->AMapsTrigInfo[0], VPtHead->AMapsTrigInfo[1], VPtHead->AMapsTrigInfo[2] ));
  msg (( MSG_OUT, "H.TrigInfo line  [0]=%.4d [1]=%.4d [2]=%.4d ", VPtHead->AMapsTrigInfo[0] & 0x3FF, VPtHead->AMapsTrigInfo[1] & 0x3FF, VPtHead->AMapsTrigInfo[2] & 0x3FF));
  msg (( MSG_OUT, "H.TrigInfo TS    [0]=%.4d [1]=%.4d [2]=%.4d ", VPtHead->AMapsTrigInfo[0] >> 10, VPtHead->AMapsTrigInfo[1] >> 10, VPtHead->AMapsTrigInfo[2]  >> 10));
  msg (( MSG_OUT, "----------------------------------------------" ));
#ifdef EFRIO__FRAME_TAGS_ENABLE
  msg (( MSG_OUT, "D.Tag          = %.8X [H]", VPtData->Tag        ));
#endif
  msg (( MSG_OUT, "D.TotSz        = %.4d  [D]", VPtData->TotSz      ));
  msg (( MSG_OUT, "D.OneMapsSz    = %.4d  [D]", VPtData->OneMapsSz  ));

  if ( PrintLevel == 3 ) {
    EFRIO__FPrintFrameData ( PtRec, PrintLevel );
  }

  if ( PrintLevel < 4 ) {
    msg (( MSG_OUT, "===================================================================================" ));
    return (0);
  }
  
  msg (( MSG_OUT, "----------------------------------------------" ));
#ifdef EFRIO__FRAME_TAGS_ENABLE
  msg (( MSG_OUT, "T.Tag          = %X   [H]", VPtTrig->Tag        ));
#endif
  msg (( MSG_OUT, "T.TotSz        = %.4d  [D]", VPtTrig->TotSz      ));
  msg (( MSG_OUT, "T.TrigNb       = %.4d  [D]", VPtTrig->TrigNb     ));
  msg (( MSG_OUT, "T.TrigType     = %d   [D]", VPtTrig->TrigType   ));
  

  for ( ViTrig=0; ViTrig < VPtTrig->TrigNb; ViTrig++ ) {
    EFRIO__FTluTrigger2Str ( VPtTrig->ATrig[(2 * ViTrig)]    , VStrTrig, 30 /* MaxDestSz */ );
    EFRIO__FTimeStamp2Str  ( VPtTrig->ATrig[(2 * ViTrig) + 1], VStrTs  , 30 /* MaxDestSz */ );
    
    msg (( MSG_OUT, "T.[%.3d] Trig = %s - Ts = %s", ViTrig, VStrTrig, VStrTs ));
  }


  msg (( MSG_OUT, "" ));
  msg (( MSG_OUT, "" ));
  
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FPrintFrameDataFsbb0 ( EFRIO__TFrame* PtRec, SInt8 PrintLevel )
          :
Goal      : print one frame content in log file
          :
Inputs    : PtRec       - Pointer on the record
          :
          : PrintLevel  - 0   -> Print nothing
          :             - > 0 -> print state list for each line
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if PtRec = NULL
          :
Globals   :
          :
Remark    :
          :
Level     :
Date      : 22/12/2010
Rev       : 30/12/2010
          : - Add handling of N Mimosa 26
          :
Doc date  : 22/12/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */
#ifdef EFRIO_FSBB0
SInt32 EFRIO__FPrintFrameDataFsbb0 ( EFRIO__TFrame* PtRec, SInt8 PrintLevel ) {
  
  SInt16               ViFsbb0;
 EFRIO__TFrameHeader* VPtHead;

  FSBB0__TZsFFrame     VZsFFrame;
  FSBB0__TMatDiscriBit VMatBit;
  SInt32               VOvfCnt;
  
  
  err_retnull ( PtRec, (ERR_OUT,"PtRec == NULL !") );
  
  if ( PrintLevel == 0 ) {
    return (0);
  }
   VPtHead = &PtRec->Header;

  memset ( &VZsFFrame, 0, sizeof (VZsFFrame) );
  memset ( &VMatBit  , 0, sizeof (VMatBit) );

  for ( ViFsbb0=0; ViFsbb0 < VPtHead->MapsNb ; ViFsbb0++ ) {
    
    msg (( MSG_OUT, "===================================================================================" ));
    msg (( MSG_OUT, "                                  Fsbb0     No %2d                                 ", ViFsbb0 ));
    msg (( MSG_OUT, "===================================================================================" ));
	
	EFRIO__FSBB0_FConvEfrioFrameToZsFFrame ( PtRec /* Src */, ViFsbb0 /* MapsId */, &VZsFFrame, 0 /* PrintLvl */ );

	EFRIO__FSBB0_FConvZsFFrameToMatDiscriBit ( &VZsFFrame /* PtSrc */, &VMatBit /* PtDest */ , &VOvfCnt /* PtOvfCnt */, 0 /* PrintLvl */ );

	EFRIO__FSBB0_FMatDiscriPrintHit ( "" /* CmtStrTitle */, ViFsbb0 /* CmtSInt8MapsId */, &VMatBit );
  } // End for ( ViFsbb0 )
  return (0);

} /* end EFRIO__FPrintFrameDataFsbb0  */
#endif








/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FPrintFrameRecFSbb0 ( EFRIO__TFrame* PtRec, SInt8 PrintLevel )
          :
Goal      : print one frame content in log file
          :
Inputs    : PtRec       - Pointer on the record
          :
          : PrintLevel  - 0 -> Print nothing
          :             - 1 -> Print AcqId & FrId
          :             - 2 -> Print AcqId, FrId ... Mi26 header, trailer ... + Trig nb
          :             - 3 -> Print AcqId, FrId ... Mi26 header, trailer ... + Trig nb + Data
          :             - 4 -> Print trigger list
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if PtRec = NULL
          :
Globals   :
          :
Remark    : copy of function  SInt32 EFRIO__FPrintFrameRec ( EFRIO__TFrame* PtRec, SInt8 PrintLevel ) from GC
          :
Level     :
Date      : 09/10/2014
Rev       : 
Doc date  : 09/10/2014
Author    : Matthieu SPecht
E-mail    : matthieu.specht@iphc.cnrs.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */
#ifdef EFRIO_FSBB0

SInt32 EFRIO__FPrintFrameRecFsbb0 ( EFRIO__TFrame* PtRec, SInt8 PrintLevel ) {
  
  EFRIO__TFrameHeader* VPtHead;
  EFRIO__TFrameData*   VPtData;
  EFRIO__TTriggerRec*  VPtTrig;
  SInt16               ViTrig;
  char                 VStrTrig[30];
  char                 VStrTs  [30];
   
  err_retnull ( PtRec, (ERR_OUT,"PtRec == NULL !") );
  
  if ( PrintLevel == 0 ) {
    return (0);
  }
  
  // -------------------------------------
  // Init pointers on TFrame sub records
  // -------------------------------------
  
  VPtHead = &PtRec->Header;
  VPtData = &PtRec->Data;
  
  // Trigger record follows the data record which has a VARIABLE size
    
  VPtTrig = (EFRIO__TTriggerRec*) ( (UInt8*) PtRec + PtRec->TrigRecOffset );

  // -------------------------------------
  // Print TFrame fields
  // -------------------------------------

  if ( PrintLevel == 1 ) {
    msg (( MSG_OUT, "==============================================" ));
    msg (( MSG_OUT, "AcqId = %.4d - FrameIdInAcq = %.4d - TrigNb = %.4d  [D]", VPtHead->AcqId, VPtHead->FrameIdInAcq, VPtTrig->TrigNb ));
    msg (( MSG_OUT, "Trigger line [0]=%.4d [1]=%.4d [2]=%.4d ", VPtHead->AMapsTrigInfo[0] & 0x3FF, VPtHead->AMapsTrigInfo[1] & 0x3FF, VPtHead->AMapsTrigInfo[2] & 0x3FF));
    msg (( MSG_OUT, "Trigger TS   [0]=%.4d [1]=%.4d [2]=%.4d ", VPtHead->AMapsTrigInfo[0] >> 10, VPtHead->AMapsTrigInfo[1] >> 10, VPtHead->AMapsTrigInfo[2]  >> 10));
  
    for ( ViTrig=0; ViTrig < VPtTrig->TrigNb; ViTrig++ ) {
      EFRIO__FTluTrigger2Str ( VPtTrig->ATrig[(2 * ViTrig)]    , VStrTrig, 30 /* MaxDestSz */ );
      EFRIO__FTimeStamp2Str  ( VPtTrig->ATrig[(2 * ViTrig) + 1], VStrTs  , 30 /* MaxDestSz */ );
      
      msg (( MSG_OUT, "T.[%.3d] Trig = %s - Ts = %s", ViTrig, VStrTrig, VStrTs ));
    }
    
    msg (( MSG_OUT, "" ));
    
    return (0);
  }
  

  msg (( MSG_OUT, "==============================================" ));
#ifdef EFRIO__FRAME_TAGS_ENABLE
  msg (( MSG_OUT, "Tag            = %.8X [H]", PtRec->Tag ));
#endif
  msg (( MSG_OUT, "DaqVersion     = %.4d [D]", PtRec->DaqVersion ));
  msg (( MSG_OUT, "TotSz          = %.4d [D]", PtRec->TotSz ));
  msg (( MSG_OUT, "TrigRecOffset  = %.4d [D]", PtRec->TrigRecOffset ));
  msg (( MSG_OUT, "----------------------------------------------" ));
#ifdef EFRIO__FRAME_TAGS_ENABLE
  msg (( MSG_OUT, "H.Tag          = %.8X [H]", VPtHead->Tag ));
#endif
  msg (( MSG_OUT, "H.AcqStatus    = %.4d [D]", VPtHead->AcqStatus ));
  msg (( MSG_OUT, "H.TrigStatus   = %.4d [D]", VPtHead->TrigStatus ));  
  msg (( MSG_OUT, "H.AcqId        = %.4d [D]", VPtHead->AcqId ));
  msg (( MSG_OUT, "H.FrameIdInAcq = %.4d [D]", VPtHead->FrameIdInAcq ));
  msg (( MSG_OUT, "H.MapsName     = %.4d [D]", VPtHead->MapsName ));
  msg (( MSG_OUT, "H.MapsNb       = %.4d [D]", VPtHead->MapsNb ));
  msg (( MSG_OUT, "----------------------------------------------" ));
  msg (( MSG_OUT, "H.Header   [0]=%.8X [1]=%.8X [2]=%.8X [3]=%.8X [4]=%.8X [5]=%.8X [6]=%.8X [7]=%.8X"      , VPtHead->AMapsHeader[0]    , VPtHead->AMapsHeader[1]    , VPtHead->AMapsHeader[2]    , VPtHead->AMapsHeader[3]    , VPtHead->AMapsHeader[4]    , VPtHead->AMapsHeader[5], VPtHead->AMapsHeader[6], VPtHead->AMapsHeader[7] ));
  msg (( MSG_OUT, "H.FrCnt    [0]=%8d [1]=%8d [2]=%8d [3]=%8d [4]=%8d [5]=%8d [6]=%8d [7]=%8d", VPtHead->AMapsFrameCnt[0]  , VPtHead->AMapsFrameCnt[1]  , VPtHead->AMapsFrameCnt[2]  , VPtHead->AMapsFrameCnt[3]  , VPtHead->AMapsFrameCnt[4]  , VPtHead->AMapsFrameCnt[5], VPtHead->AMapsFrameCnt[6], VPtHead->AMapsFrameCnt[7] ));
  msg (( MSG_OUT, "H.DataSz   [0]=%8d [1]=%8d [2]=%8d [3]=%8d [4]=%8d [5]=%8d [6]=%8d [7]=%8d", VPtHead->AMapsDataLength[0], VPtHead->AMapsDataLength[1], VPtHead->AMapsDataLength[2], VPtHead->AMapsDataLength[3], VPtHead->AMapsDataLength[4], VPtHead->AMapsDataLength[5], VPtHead->AMapsDataLength[6], VPtHead->AMapsDataLength[7] ));
  msg (( MSG_OUT, "H.Trailer  [0]=%.8X [1]=%.8X [2]=%.8X [3]=%.8X [4]=%.8X [5]=%.8X [6]=%.8X [7]=%.8X"      , VPtHead->AMapsTrailer[0]   , VPtHead->AMapsTrailer[1]   , VPtHead->AMapsTrailer[2]   , VPtHead->AMapsTrailer[3]   , VPtHead->AMapsTrailer[4]   , VPtHead->AMapsTrailer[5], VPtHead->AMapsTrailer[6], VPtHead->AMapsTrailer[7] ));
  msg (( MSG_OUT, "----------------------------------------------" ));
  msg (( MSG_OUT, "H.TriggerNb    = %.4d [D]", VPtHead->TriggerNb ));
  // msg (( MSG_OUT, "H.TrigInfo [0]=%.8X [1]=%.8X [2]=%.8X ", VPtHead->AMapsTrigInfo[0], VPtHead->AMapsTrigInfo[1], VPtHead->AMapsTrigInfo[2] ));
  msg (( MSG_OUT, "H.TrigInfo line  [0]=%.4d [1]=%.4d [2]=%.4d ", VPtHead->AMapsTrigInfo[0] & 0x3FF, VPtHead->AMapsTrigInfo[1] & 0x3FF, VPtHead->AMapsTrigInfo[2] & 0x3FF));
  msg (( MSG_OUT, "H.TrigInfo TS    [0]=%.4d [1]=%.4d [2]=%.4d ", VPtHead->AMapsTrigInfo[0] >> 10, VPtHead->AMapsTrigInfo[1] >> 10, VPtHead->AMapsTrigInfo[2]  >> 10));
  msg (( MSG_OUT, "----------------------------------------------" ));
#ifdef EFRIO__FRAME_TAGS_ENABLE
  msg (( MSG_OUT, "D.Tag          = %.8X [H]", VPtData->Tag        ));
#endif
  msg (( MSG_OUT, "D.TotSz        = %.4d  [D]", VPtData->TotSz      ));
  msg (( MSG_OUT, "D.OneMapsSz    = %.4d  [D]", VPtData->OneMapsSz  ));

  if ( PrintLevel == 3 ) {
    EFRIO__FPrintFrameDataFsbb0 ( PtRec, PrintLevel );
  }

  if ( PrintLevel < 4 ) {
    msg (( MSG_OUT, "===================================================================================" ));
    return (0);
  }
  
  msg (( MSG_OUT, "----------------------------------------------" ));
#ifdef EFRIO__FRAME_TAGS_ENABLE
  msg (( MSG_OUT, "T.Tag          = %X   [H]", VPtTrig->Tag        ));
#endif
  msg (( MSG_OUT, "T.TotSz        = %.4d  [D]", VPtTrig->TotSz      ));
  msg (( MSG_OUT, "T.TrigNb       = %.4d  [D]", VPtTrig->TrigNb     ));
  msg (( MSG_OUT, "T.TrigType     = %d   [D]", VPtTrig->TrigType   ));
 
  for ( ViTrig=0; ViTrig < VPtTrig->TrigNb; ViTrig++ ) {
    EFRIO__FTluTrigger2Str ( VPtTrig->ATrig[(2 * ViTrig)]    , VStrTrig, 30 /* MaxDestSz */ );
    EFRIO__FTimeStamp2Str  ( VPtTrig->ATrig[(2 * ViTrig) + 1], VStrTs  , 30 /* MaxDestSz */ );
    
    msg (( MSG_OUT, "T.[%.3d] Trig = %s - Ts = %s", ViTrig, VStrTrig, VStrTs ));
  }

  msg (( MSG_OUT, "" ));
  msg (( MSG_OUT, "" ));
  
}
#endif


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FPrintMonContRec ( EFRIO__TMon* PtRec )
          :
Goal      : Print monitor context record in log file
          :
Inputs    : PtRec - Pointer on the record
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if PtRec = NULL
          :
Globals   :
          :
Remark    :
          :
Level     :
Date      : 15/02/2011
Doc date  : 15/02/2011
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FPrintMonContRec ( EFRIO__TMon* PtRec ) {
  
  err_retnull ( PtRec, (ERR_OUT,"Abort : PtRec == NULL") );
  
  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "Monitor context record" ));
  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "InfFrameNbToSend     = %.4d", PtRec->InfFrameNbToSend ));
  msg (( MSG_OUT, "InfSzToSend          = %.4d", PtRec->InfSzToSend ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "============================================================" ));


  err_retok (( ERR_OUT, "" ));
}
  
/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FPrintMonCont ()
          :
Goal      : Print lib monitor context record in log file
          :
Inputs    : None
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if &EFRIO__VGContext.RunCont = NULL => But it's not possible
          :
Globals   :
          :
Remark    : Call EFRIO__FPrintRunContRec (&EFRIO__VGContext.RunCont)
          :
Level     :
Date      : 15/02/2011
Doc date  : 15/02/2011
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FPrintMoniCont () {
  
  return ( EFRIO__FPrintMonContRec (&EFRIO__VGContext.MonCont) );
}

  
/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FPrintTestOnDataContRec ( EFRIO__TTestOnDataCont* PtRec, char* Msg )
:
Goal      : Print monitor context record in log file
:
Inputs    : PtRec - Pointer on the record
:
Ouputs    : The function returns
:  0 if ok
: -1 if PtRec = NULL
:
Globals   :
:
Remark    :
:
Level     :
Date      : 15/02/2011
Doc date  : 15/02/2011
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */  

SInt32 EFRIO__FPrintTestOnDataContRec ( EFRIO__TTestOnDataCont* PtRec, char* Msg ) {

  SInt8 ViMaps;

  err_retnull ( PtRec, (ERR_OUT,"Abort : PtRec == NULL") );
  
  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "Test on data context record => Print only relevant fields !" ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "Message : %s", Msg ));
  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "ParModeEnable = %d", PtRec->ParModeEnable ));
  msg (( MSG_OUT, "ParResetCnt   = %d", PtRec->ParResetCnt   ));
  msg (( MSG_OUT, "ParPrintLvl   = %d", PtRec->ParResetCnt   ));
  msg (( MSG_OUT, "ParMapsNb     = %d", PtRec->ParMapsNb     ));

  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  
  for ( ViMaps=0; ViMaps < PtRec->ParMapsNb; ViMaps++ ) {
    msg (( MSG_OUT, "ParAMapsHeaderRef[%d]        = %.8x", ViMaps, PtRec->ParAMapsHeaderRef[ViMaps] ));
  }

  msg (( MSG_OUT, "------------------------------------------------------------" ));
  
  for ( ViMaps=0; ViMaps < PtRec->ParMapsNb; ViMaps++ ) {
    msg (( MSG_OUT, "ParAMapsTrailerRef[%d]       = %8x", ViMaps, PtRec->ParAMapsTrailerRef[ViMaps] ));
  }

  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  
  for ( ViMaps=0; ViMaps < PtRec->ParMapsNb; ViMaps++ ) {
    msg (( MSG_OUT, "ResAMapsErrCnt[%d]           = %d", ViMaps, PtRec->ResAMapsErrCnt[ViMaps] ));
  }
  

  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  
  for ( ViMaps=0; ViMaps < PtRec->ParMapsNb; ViMaps++ ) {
    msg (( MSG_OUT, "ResAMapsHeaderErrCnt[%d]     = %d", ViMaps, PtRec->ResAMapsHeaderErrCnt[ViMaps] ));
  }
  
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  
  for ( ViMaps=0; ViMaps < PtRec->ParMapsNb; ViMaps++ ) {
    msg (( MSG_OUT, "ResAMapsFrameCntErrCnt[%d]   = %d", ViMaps, PtRec->ResAMapsFrameCntErrCnt[ViMaps] ));
  }
  
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  
  for ( ViMaps=0; ViMaps < PtRec->ParMapsNb; ViMaps++ ) {
    msg (( MSG_OUT, "ResAMapsDataLengthErrCnt[%d] = %d", ViMaps, PtRec->ResAMapsDataLengthErrCnt[ViMaps] ));
  }

  msg (( MSG_OUT, "------------------------------------------------------------" ));
  
  for ( ViMaps=0; ViMaps < PtRec->ParMapsNb; ViMaps++ ) {
    msg (( MSG_OUT, "ResAMapsTrailerErrCnt[%d]    = %d", ViMaps, PtRec->ResAMapsTrailerErrCnt[ViMaps] ));
  }

  msg (( MSG_OUT, "------------------------------------------------------------" ));
  
  for ( ViMaps=0; ViMaps < PtRec->ParMapsNb; ViMaps++ ) {
    msg (( MSG_OUT, "ResAMapsMatrixErrCnt[%d]     = %d", ViMaps, PtRec->ResAMapsMatrixErrCnt[ViMaps] ));
  }
  
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  
  
  msg (( MSG_OUT, "ResTotErrCnt      = %.4d", PtRec->ResTotErrCnt ));
  msg (( MSG_OUT, "ResFrameNbWithErr = %.4d", PtRec->ResFrameNbWithErr ));
  msg (( MSG_OUT, "============================================================" ));
  
  
  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FPrintTestOnDataCont ( char* Msg )
:
Goal      : Print lib monitor context record in log file
:
Inputs    : None
:
Ouputs    : The function returns
:  0 if ok
: -1 if &EFRIO__VGContext.RunCont = NULL => But it's not possible
:
Globals   :
:
Remark    : Call EFRIO__FPrintRunContRec (&EFRIO__VGContext.RunCont)
:
Level     :
Date      : 29/04/2011
Doc date  : 29/04/2011
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FPrintTestOnDataCont ( char* Msg )  {
  
  return ( EFRIO__FPrintTestOnDataContRec (&EFRIO__VGContext.TestOnDataCont, Msg) );
}

  

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FPrintMsg (
          :  SInt32 DummyS32In, SInt8 PrintAsMsg, SInt8 PrintAsTrace, SInt8 PrintAsWarning,
          :  SInt8 PrintAsError, char* Msg )
          :
Goal      : Print messages in msg or error log file from LabView -> A Vi encapsulets this function.
          :
Inputs    : DummyS32In      - Dummy value used under Labview to easily " chain " function execution.
          :
          :                   To execute it after a Vi call, connect any output of this Vi to the
          :                   DummyS32In pin and it will be automatically called after Vi end.
          :                   To execute it before a Vi call, if this Vi has an integer parameter
          :                   cut the wire and insert DummyS32In input and function output on it.
          :
          :                 Printing mode flags  
          :
          : PrintAsMsg      - If 1 -> Print in messages log file
          : PrintAsTrace    - If 1 -> Print in errors log file as a trace message
          : PrintAsWarning  - If 1 -> Print in errors log file as a warning message
          : PrintAsError    - If 1 -> Print in errors log file as an error message
          :
          : Msg             - Message to print ( string )
          :
Ouputs    : The function always returns the input parameter DummyS32In.
          :
Globals   :
          :
Remark    : Return the value of the input parameter name DummyS32In ( SInt32 )
          : It can be used to insert this function call on an integer datapath under LabView
          :
          : If more than one printing mode flag is enabled therefore the same message will
          : be printed in different ways. 
          :
Level     :
Date      : 09/08/2010
Rev       : 25/10/2010
          : - Implementation -> Empty function before
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FPrintMsg ( SInt32 DummyS32In, SInt8 PrintAsMsg, SInt8 PrintAsTrace, SInt8 PrintAsWarning, SInt8 PrintAsError, char* Msg ) {
  
  if ( PrintAsMsg     ) msg         (( MSG_OUT, "%s", Msg ));
  if ( PrintAsTrace   ) err_trace   (( ERR_OUT, "%s", Msg ));
  if ( PrintAsWarning ) err_warning (( ERR_OUT, "%s", Msg ));
  if ( PrintAsError   ) err_error   (( ERR_OUT, "%s", Msg ));
  
  return (DummyS32In);
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FPrintMsg (
:  SInt32 DummyS32In, SInt8 PrintAsMsg, SInt8 PrintAsTrace, SInt8 PrintAsWarning,
:  SInt8 PrintAsError, char* Msg )
:
Goal      : Print messages in msg or error log file from LabView -> A Vi encapsulets this function.
:
Inputs    : DummyS32In      - Dummy value used under Labview to easily " chain " function execution.
:
:                   To execute it after a Vi call, connect any output of this Vi to the
:                   DummyS32In pin and it will be automatically called after Vi end.
:                   To execute it before a Vi call, if this Vi has an integer parameter
:                   cut the wire and insert DummyS32In input and function output on it.
:
:                 Printing mode flags
:
: PrintAsMsg      - If 1 -> Print in messages log file
: PrintAsTrace    - If 1 -> Print in errors log file as a trace message
: PrintAsWarning  - If 1 -> Print in errors log file as a warning message
: PrintAsError    - If 1 -> Print in errors log file as an error message
:
: Msg             - Message to print ( string )
:
Ouputs    : The function always returns the input parameter DummyS32In.
:
Globals   :
:
Remark    : Return the value of the input parameter name DummyS32In ( SInt32 )
: It can be used to insert this function call on an integer datapath under LabView
:
: If more than one printing mode flag is enabled therefore the same message will
: be printed in different ways.
:
Level     :
Date      : 24/04/2013
Doc date  : 
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

#ifndef CC_UNIX

SInt32 EFRIO__FPrintMsgWithTimeStamp ( SInt32 DummyS32In, SInt8 PrintAsMsg, SInt8 PrintAsTrace, SInt8 PrintAsWarning, SInt8 PrintAsError, char* Msg ) {
  
  TDateTime VDateTime;
  char            VStrMsgDateTime[GLB_CMT_SZ];
  unsigned short  VYear;
  unsigned short  VMonth;
  unsigned short  VDay;
  unsigned short  VHour;
  unsigned short  VMin;
  unsigned short  VSec;
  unsigned short  VMSec;
  
  
  VDateTime = VDateTime.CurrentDateTime();
  
  VDateTime.DecodeDate (  &VYear,  &VMonth,  &VDay );    
  VDateTime.DecodeTime (  &VHour,    &VMin,  &VSec, &VMSec );
    
        
  sprintf ( VStrMsgDateTime, "%.2d/%.2d/%.2d - %.2d:%.2d:%.2d|%.2d : %s", VDay, VMonth, VYear, VHour, VMin, VSec, VMSec, Msg );
  
  if ( PrintAsMsg     ) msg         (( MSG_OUT, "%s", VStrMsgDateTime ));
  if ( PrintAsTrace   ) err_trace   (( ERR_OUT, "%s", VStrMsgDateTime ));
  if ( PrintAsWarning ) err_warning (( ERR_OUT, "%s", VStrMsgDateTime ));
  if ( PrintAsError   ) err_error   (( ERR_OUT, "%s", VStrMsgDateTime ));
  
  return (DummyS32In);
}

#endif

#endif
