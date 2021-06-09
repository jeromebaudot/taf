
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


#ifndef EUDET_FRIO_SET_C
#define EUDET_FRIO_SET_C


  

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      : Set board conf fields
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 09/08/2010
Doc date  : 09/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FSetBoardConf ( SInt32 BoardId, EFRIO__TBoardConf* PtSrc ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  err_retnull ( PtSrc, (ERR_OUT,"Abort : PtSrc == NULL") );
  
  EFRIO__VGContext.ABoardsConf[BoardId] = *PtSrc;
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      : Set board conf fields
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 10/08/2010
Doc date  : 10/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

// 10/08/2010

SInt32 EFRIO__FSetBoardConfBoardId ( SInt32 BoardId ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].BoardId = BoardId;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfAsicName ( SInt32 BoardId, char* AsicName ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
    
  snprintf ( EFRIO__VGContext.ABoardsConf[BoardId].AsicName, GLB_CMT_SZ - 1, "%s", AsicName );
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfAsicNb ( SInt32 BoardId, SInt32 AsicNb ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].AsicNb = AsicNb;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfReadoutMode ( SInt32 BoardId, SInt32 ReadoutMode ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].ReadoutMode = ReadoutMode;
  
  err_retok (( ERR_OUT, "" ));
}

// 11/08/2010

SInt32 EFRIO__FSetBoardConfEmuleChannels ( SInt32 BoardId, SInt8 EmuleChannels ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].EmuleChannels = EmuleChannels;
  
  err_retok (( ERR_OUT, "" ));
}


// 10/08/2010

SInt32 EFRIO__FSetBoardConfDataClkFrequency ( SInt32 BoardId, float DataClkFrequency ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].DataClkFrequency = DataClkFrequency;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfDmaHostSz ( SInt32 BoardId, UInt32 DmaHostSz ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].DmaHostSz = DmaHostSz;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfFrameNbPerAcq ( SInt32 BoardId, SInt32 FrameNbPerAcq ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].FrameNbPerAcq = FrameNbPerAcq;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfEnableExtraChannel ( SInt32 BoardId, SInt8 EnableExtraChannel ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].EnableExtraChannel = EnableExtraChannel;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfAcqNbPerTrig ( SInt32 BoardId, SInt32 AcqNbPerTrig ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].AcqNbPerTrig = AcqNbPerTrig;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfTriggerMode ( SInt32 BoardId, SInt8 TriggerMode ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].TriggerMode = TriggerMode;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfTriggerDetectTimeWindow ( SInt32 BoardId, UInt32 TriggerDetectTimeWindow ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].TriggerDetectTimeWindow = TriggerDetectTimeWindow;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfTriggerDetectOccurNb ( SInt32 BoardId, UInt32 TriggerDetectOccurNb ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].TriggerDetectOccurNb = TriggerDetectOccurNb;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfTimeStampRes ( SInt32 BoardId, UInt32 TimeStampRes ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].TimeStampRes = TimeStampRes;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfEnableTimeStamping ( SInt32 BoardId, SInt8 EnableTimeStamping ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].EnableTimeStamping = EnableTimeStamping;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfEnableTrigCnt ( SInt32 BoardId, SInt8 EnableTrigCnt ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].EnableTrigCnt = EnableTrigCnt;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfTagEventsStoredByDUT ( SInt32 BoardId, SInt8 TagEventsStoredByDUT ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].TagEventsStoredByDUT = TagEventsStoredByDUT;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfReadTluTrigCntEachNTrig ( SInt32 BoardId, UInt32 ReadTluTrigCntEachNTrig ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].ReadTluTrigCntEachNTrig = ReadTluTrigCntEachNTrig;
  
  err_retok (( ERR_OUT, "" ));
}




/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      : Set board status fields
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 10/08/2010
Doc date  : 10/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

// 10/08/2010

SInt32 EFRIO__FSetBoardStatusBoardId ( SInt32 BoardId ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsStatus[BoardId].BoardId = BoardId;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardStatusBoardPresent ( SInt32 BoardId, SInt8 BoardPresent ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsStatus[BoardId].BoardPresent = BoardPresent;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardStatusFwLoaded ( SInt32 BoardId, SInt8 FwLoaded ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsStatus[BoardId].FwLoaded = FwLoaded;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardStatusConfDone ( SInt32 BoardId, SInt8 ConfDone ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsStatus[BoardId].ConfDone = ConfDone;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardStatusStatusCode ( SInt32 BoardId, SInt32 StatusCode ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsStatus[BoardId].StatusCode = StatusCode;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardStatusStatusStr ( SInt32 BoardId, char* StatusStr ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  err_retnull ( StatusStr, (ERR_OUT,"Abort : StatusStr == NULL") );
  
  snprintf ( EFRIO__VGContext.ABoardsStatus[BoardId].StatusStr, GLB_CMT_SZ - 1, "%s", StatusStr );
    
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardStatusErrorMsgList ( SInt32 BoardId, char* ErrorMsg, SInt32 Index ) {
  
  SInt32 VMsgLen;
  EFRIO__TBoardStatus* VPtStatus;
  
  EFRIO__CHK_BOARD_ID (BoardId);

  err_retnull ( ErrorMsg, (ERR_OUT,"Abort : ErrorMsg == NULL") );
  
  VPtStatus = &EFRIO__VGContext.ABoardsStatus[BoardId];
  
  // Check index limits
  
  if ( (Index < 0) || (Index >= EFRIO__ERROR_MSG_LIST_MAX_NB ) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Index=%d out of range [0..%d]", Index, EFRIO__ERROR_MSG_LIST_MAX_NB-1 ) );
  }
  
  // Get message length
  
  VMsgLen = strlen ( ErrorMsg );
  
  // Free item if already allocated
  
  if ( VPtStatus->ErrorMsgList[Index] != NULL ) {
    free ( VPtStatus->ErrorMsgList[Index] );
  }
  
  // Allocate memory for message
  
  VPtStatus->ErrorMsgList[Index] = (char*) malloc ( VMsgLen + 1 );
  
  err_retnull ( VPtStatus->ErrorMsgList[Index], (ERR_OUT,"Abort : Allocation of message [%d] failed !", Index) );
  
  // Copy message

  snprintf ( VPtStatus->ErrorMsgList[Index], VMsgLen, "%s", ErrorMsg );
    
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardStatusLastErrorMsg ( SInt32 BoardId, char* LastErrorMsg ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);

  err_retnull ( LastErrorMsg, (ERR_OUT,"Abort : LastErrorMsg == NULL") );
  
  snprintf ( EFRIO__VGContext.ABoardsStatus[BoardId].LastErrorMsg, GLB_CMT_SZ - 1, "%s", LastErrorMsg );  
  
  err_retok (( ERR_OUT, "" ));
}

// 21/12/2010

SInt32 EFRIO__FSetCmdRun ( SInt32 Cmd ) {
  
  if ( Cmd == 0 ) {
    if ( EFRIO__VGContext.RunCont.CmdRun == 1 ) {
      EFRIO__VGContext.RunCont.CmdRun = 0;
    }
  }
  
  else {
    EFRIO__VGContext.RunCont.CmdRun = Cmd;
  }

  return (0);
}

#endif
