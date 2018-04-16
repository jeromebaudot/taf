
/*******************************************************************************
File      : x:\lib\win\eudet_frio\eudet_frio_usr.c
Goal      : Functions of USER PART of flex rio board library for EUDET Mimosa 26 DAQ
Prj date  : 05/08/2010
File date : 11/11/2010
Doc date  : 11/11/2010
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


#ifndef EUDET_FRIO_USR_C
#define EUDET_FRIO_USR_C




/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
          :
Goal      :
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
Date      : 11/11/2010
Rev       : 01/03/2013
          : - Add fields of EFRIO__USR_TContext and set their default values
          : -- ParDisableRunCtrlMsgProcessing
          : -- ParEnableRunCtrlMsgDisplay
          : -- ResRunCtrlMsgReady
          : -- ResRunCtrlMsgStr
Doc date  : 11/11/2010
Author    : 
E-mail    : 
Labo      :  */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO_USR__FBegin () {
  
  EFRIO__USR_TContext* VPtCont = &EFRIO__USR_VGContext;
  char* VPtLastMsgToGui        = VPtCont->LastMsgToGui;
    
    
  err_trace (( ERR_OUT, "" ));

  // Reset last message to GUI
  // Fill with 0 to be sure to have 0 at end of string in case string is truncated by snprintf
  
  memset ( VPtLastMsgToGui, 0, GLB_CMT_SZ ); 

#ifdef EFRIO__INCLUDE_RUN_CTRL

  // 01/03/2013
  
  VPtCont->ParDisableRunCtrlMsgProcessing = 0;
  VPtCont->ParEnableRunCtrlMsgDisplay     = 0;
  VPtCont->ResRunCtrlMsgReady             = 0;
  sprintf ( VPtCont->ResRunCtrlMsgStr, "" );
  

  // Create event & thread to respond to run ctrl requests
  
  // Event
  
  VPtCont->InfEventRespondRunCtrlHnd = CreateEvent (
    NULL    /*  LPSECURITY_ATTRIBUTES */,
    FALSE   /* Manual Reset */,
    FALSE   /* Initial State */,
    NULL    /* Name STring */ );
    
  if ( VPtCont->InfEventRespondRunCtrlHnd == NULL ) {
    err_retfail ( -1, (ERR_OUT,"Create Event respond run ctrl failed !  - LastError=%d", GetLastError () ) );
  }

  // Thread

  VPtCont->InfThreadRespondRunCtrlHnd = CreateThread( NULL, NULL, EFRIO_USR__FThreadRespondRunCtrl, NULL, NULL, &VPtCont->InfThreadRespondRunCtrlId );
    
  if ( VPtCont->InfThreadRespondRunCtrlHnd == NULL ) {
    err_retfail ( -1, (ERR_OUT,"Create thread respond run ctrl failed !  - LastError=%d", GetLastError () ) );
  }
  
#endif
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
          :
Goal      :
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
Date      : 11/11/2010
Doc date  : 11/11/2010
Author    : 
E-mail    : 
Labo      :  */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO_USR__FEnd () {

  EFRIO__USR_TContext* VPtCont = &EFRIO__USR_VGContext;
  
#ifdef EFRIO__INCLUDE_RUN_CTRL

  // Kill respond run ctrl thread

  CloseHandle ( VPtCont->InfEventRespondRunCtrlHnd );
  
  TerminateThread ( EFRIO_USR__FThreadRespondRunCtrl, 0 /* Thread exit code */ );

#endif

  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO_USR__FSetLineCmdPar ( char** CmdLinePar )
          :
Goal      : This function 
          : 
          :
Inputs    : None
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   : 
          :
Remark    : 
          : 
          :
Level     :
Date      : 26/01/2011
Doc date  : 26/01/2011
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO_USR__FGetLineCmdPar () {

  SInt8  Vi;
  SInt32 VParamCnt;


  VParamCnt = ParamCount();

  msg (( MSG_OUT, "Line cmd param nb = %d", VParamCnt    ));
  
  for ( Vi=0; Vi < (VParamCnt + 1); Vi++ ) {
    msg (( MSG_OUT, "Par[%.2d] = %s", Vi, ParamStr (Vi) ));
  }
      
  err_retval ( VParamCnt, ( ERR_OUT, "Line cmd param nb = %d", VParamCnt ) );
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
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
Date      : 03/02/2011
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */


#ifdef EFRIO__INCLUDE_RUN_CTRL

SInt32 EFRIO_USR__FHandleRunCtrlMsg (UInt8* PtMsg, UInt32 MsgSz, UInt8* PtUsrData, UInt32 UsrDataSz ) {
  
  void*  VPtMsg;
  EFRIO__USR_TRunCtrlMsgHeader* VPtRunCtrlMsg;
  EFRIO__TRunCont*              VPtRunCont;
  EFRIO__USR_TContext*          VPtUsrCont = &EFRIO__USR_VGContext;
  
  if ( MsgSz <= 0 ) {
    err_error (( ERR_OUT, "Msg received with MsgSz=%d <= 0", MsgSz ));
  }
      
  VPtMsg = malloc ( UsrDataSz );
  
  if ( VPtMsg == NULL ) {
    err_error (( ERR_OUT, "Allocation of reception buffer failed !" ));
    return (-1);
  }
  
  VPtRunCtrlMsg = (EFRIO__USR_TRunCtrlMsgHeader*) VPtMsg;
    
  memcpy ( VPtMsg, PtUsrData, UsrDataSz );
    
  switch ( VPtRunCtrlMsg->MsgCode ) {
    
    case EFRIO__USR_RUN_CTRL_MSG_CONF : {
      VPtRunCont = (EFRIO__TRunCont*) &(VPtRunCtrlMsg->Data);
      VPtUsrCont->InfFOnConfByRunCtrlRetCode = EFRIO_USR__FOnConfByRunCtrl ( VPtRunCont );
      break;
    }
        
    case EFRIO__USR_RUN_CTRL_MSG_START_RUN : {
      VPtUsrCont->InfFOnStartRunByRunCtrlRetCode = EFRIO_USR__FOnStartRunByRunCtrl ();
      break; }    
        
    case EFRIO__USR_RUN_CTRL_MSG_GET_RUN_STATUS_RQ : {
      VPtUsrCont->InfFOnGetRunStatusByRunCtrl = EFRIO_USR__FOnGetRunStatusByRunCtrl ();
      break; }

    case EFRIO__USR_RUN_CTRL_MSG_STOP_RUN : {
      VPtUsrCont->InfFOnStopRunByRunCtrl = EFRIO_USR__FOnStopRunByRunCtrl ();
      break; }
    
  }
  
  free ( VPtMsg );

 err_retok (( ERR_OUT, "" ));

}

#endif

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
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
Date      : 22/02/2011
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

#ifdef EFRIO__INCLUDE_RUN_CTRL

DWORD WINAPI EFRIO_USR__FThreadRespondRunCtrl ( LPVOID lpParam ) {
  
  EFRIO__USR_TContext*             VPtUsrCont = &EFRIO__USR_VGContext;
  EFRIO__TRunCont*                 VPtRunCont = &EFRIO__VGContext.RunCont;
  void*                            VPtMsg;
  EFRIO__USR_TRunCtrlMsgHeader*    VPtRunCtrlMsg;
  EFRIO__USR_TRunCtrlRunStatusAns* VPtRunStatusAns;
  SInt32                           VTotMsgSz;

  SInt32 VSendStatus;


  while (1) {
    
    if ( VPtUsrCont->InfEventRespondRunCtrlHnd != NULL ) {
      WaitForSingleObject ( VPtUsrCont->InfEventRespondRunCtrlHnd, INFINITE /* dwTimeout */ );
    }
    
    Sleep (100);
    
    // msg (( MSG_OUT, "Run ctrl request received" ));

    // -----------------------------------------------------------------------
    // Allocate message bloc
    // -----------------------------------------------------------------------
    
    VTotMsgSz = sizeof (EFRIO__USR_TRunCtrlMsgHeader) + sizeof (EFRIO__USR_TRunCtrlRunStatusAns);
        
    VPtMsg = malloc ( VTotMsgSz );
    
    err_retnull ( VPtMsg, (ERR_OUT,"Allocation of buffer failed !") );
    
    
    VPtRunCtrlMsg   = (EFRIO__USR_TRunCtrlMsgHeader*) VPtMsg;
    VPtRunStatusAns = (EFRIO__USR_TRunCtrlRunStatusAns*) &(VPtRunCtrlMsg->Data);
    
    
    // -----------------------------------------------------------------------
    // Fill answer record
    // -----------------------------------------------------------------------
    
    VPtRunStatusAns->RunCont = *VPtRunCont;
    
    // -----------------------------------------------------------------------
    // Prepare message
    // -----------------------------------------------------------------------
    
    
    VPtRunCtrlMsg->TotMsgSz   = VTotMsgSz;
    VPtRunCtrlMsg->HeaderSz   = sizeof (EFRIO__USR_TRunCtrlMsgHeader);
    VPtRunCtrlMsg->MsgCode    = EFRIO__USR_RUN_CTRL_MSG_GET_RUN_STATUS_ANS;
    snprintf ( VPtRunCtrlMsg->StrMsgCode, EFRIO__USR_RUN_CTRL_MSG_CODE_STR_MAX_SZ - 1, "%s", "EFRIO__USR_RUN_CTRL_MSG_GET_RUN_STATUS_ANS" );
    
    
    VSendStatus = IAC__FSockSendShortMsg ( VPtUsrCont->InfRunCtrlSenderId, (UInt8*) VPtMsg /* PtMsg */, VPtRunCtrlMsg->TotMsgSz, NULL /* Caller  */  );
    
    
    if ( VSendStatus < 0 ) {
      err_error (( ERR_OUT, "Sending response to run status request failed !" ));
    }
    
    // Freee message bloc
    
    free ( VPtMsg );


  }
  
  
  return (0);
}

#endif


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO_USR__FOnAppStart ()
:
Goal      : This function is called when application is started. It will do all library
: initializations.
:
Inputs    : None
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   : Write to EFRIO__USR_VGContext.LastMsgToGui.
:
Remark    : It can use command line arguments to select operating modes, debug level, etc ...
: Update the global variable GUI message -> EFRIO__USR_VGContext.LastMsgToGui
:
Level     :
Date      : 21/12/2010
Doc date  : 21/12/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO_USR__FOnAppStart () {
  
  SInt32 VRet;
  char*  VPtLastMsgToGui = EFRIO__USR_VGContext.LastMsgToGui;
  
  // Variables to set error & messages log files parameters
  
  SInt8 VErrFileLogLvl;
  char  VErrLogFilePath[GLB_FILE_PATH_SZ];
  SInt8 VMsgFileLogLvl;
  char  VMsgLogFilePath[GLB_FILE_PATH_SZ];
  
  SInt32 VMyVar = 10; // For error & general messages demo
  
  
  // -----------------------------------------------------------------------
  // Default values for error & messages log files are hard coded here
  // But they can be overwritten by parameters from command line for example
  // -----------------------------------------------------------------------
  
  VErrFileLogLvl = ERR_LOG_LVL_ERRORS; // ERR_LOG_LVL_NONE, ERR_LOG_LVL_ALL, ERR_LOG_LVL_WARNINGS_ERRORS, ERR_LOG_LVL_ERRORS
  
  sprintf ( VErrLogFilePath, "%s", "x:\\log\\err_eudet_dll.txt" );
  
  VMsgFileLogLvl = 127;
  
  sprintf ( VMsgLogFilePath, "%s", "x:\\log\\msg_eudet_dll.txt" );
  
  // -----------------------------------------------------------------------
  // Overwrite here variables to conf error & messages log files parameters
  // Values from command line arguments can be use
  // -----------------------------------------------------------------------
  
  // VErrFileLogLvl =  // ERR_LOG_LVL_NONE, ERR_LOG_LVL_ALL, ERR_LOG_LVL_WARNINGS_ERRORS, ERR_LOG_LVL_ERRORS
  
  // sprintf ( VErrLogFilePath, "%s", "" );
  
  // VMsgFileLogLvl
  
  // sprintf ( VMsgLogFilePath, "%s", "" );
  
  
  // -----------------------------------------------------------------------
  // Flex rio lib init
  // -----------------------------------------------------------------------
  
  // Call init function
  
  VRet = EFRIO__FBegin ( VErrFileLogLvl, VErrLogFilePath, VMsgFileLogLvl, VMsgLogFilePath );
  
  // Copy error message from err lib to last GUI message - user can write something else if more useful
  
  if ( VRet < 0 ) {
    snprintf ( VPtLastMsgToGui, GLB_CMT_SZ, "%s", ERR_FGetLastErrMsg () );
    return (VRet);
  }
  
  // Set last GUI message to "OK"
  
  snprintf ( VPtLastMsgToGui, GLB_CMT_SZ-1, "OK - General init done :-)" );
  
  // -----------------------------------------------------------------------
  // User part of flex rio lib init
  // -----------------------------------------------------------------------
  
  // Call init function
  
  VRet = EFRIO_USR__FBegin ();
  
  // Copy error message from err lib to last GUI message - user can write something else if more useful
  
  if ( VRet < 0 ) {
    snprintf ( VPtLastMsgToGui, GLB_CMT_SZ, "%s", ERR_FGetLastErrMsg () );
    return (VRet);
  }
  
  // Set last GUI message to "OK"
  
  snprintf ( VPtLastMsgToGui, GLB_CMT_SZ-1, "OK - General & user init done :-)" );
  
  
  // -----------------------------------------------------------------------
  // Error messages demo
  // -----------------------------------------------------------------------
  
  err_trace   (( ERR_OUT, "This is a trace message from DLL   - VMyVar=%d", VMyVar ));
  err_warning (( ERR_OUT, "This is a warning message from DLL - VMyVar=%d", VMyVar ));
  err_error   (( ERR_OUT, "This is an error message from DLL  - VMyVar=%d", VMyVar ));
  
  // -----------------------------------------------------------------------
  // General messages demo
  // -----------------------------------------------------------------------
  
  msg  (( MSG_OUT, "This is a general message from DLL - with default LogLvl = 1 - VMyVar=%d", VMyVar ));
  msgl (( MSG_OUT, "This is a general message from DLL - with LogLvl = 2         - VMyVar=%d", VMyVar ), 2 );  
  
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO_USR__FOnAppStartByRunCtrlExt ( SInt8 MapsName, char* RunCtrlIPAddr, SInt32 RunCtrlPort, SInt32 DaqPort )
          :
Goal      : This function is called when application is started. It will do all library
          : initializations.
          :
Inputs    : None
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   : Write to EFRIO__USR_VGContext.LastMsgToGui.
          :
Remark    : It can use command line arguments to select operating modes, debug level, etc ...
          : Update the global variable GUI message -> EFRIO__USR_VGContext.LastMsgToGui
          :
Level     :
Date      : 03/02/2011
          :
Rev       : 25/05/2011
          : - Renamed in EFRIO_USR__FOnAppStartByRunCtrlExt
          : - Add parameter MapsName
          : - Create a new EFRIO_USR__FOnAppStartByRunCtrl (...) which call EFRIO_USR__FOnAppStartByRunCtrlExt ( ASIC__MI26, ... )
          :   This will keep compatibility with previous sw (EUDET), because EFRIO_USR__FOnAppStartByRunCtrl call
          :   will configure Mimosa 26 as ASIC as it was done for first version of EUDET library.
          :
          : 20/03/2013
          : - Add parameters ErrLogLvl and MsgLogLvl
          :
          : 26/03/2013
          : - Add parameters CrIndexFile and ReadoutMode
          :
Doc date  : 03/02/2011
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


#ifndef EFRIO__INCLUDE_RUN_CTRL

  SInt32 EFRIO_USR__FOnAppStartByRunCtrlExt ( SInt8 MapsName, char* RunCtrlIPAddr, SInt32 RunCtrlPort, SInt32 DaqPort, SInt8 CrIndexFile, SInt32 ReadoutMode, SInt8 ErrLogLvl, SInt8 MsgLogLvl )  {
    
    err_retfail ( -1, (ERR_OUT,"Function no available => EFRIO__INCLUDE_RUN_CTRL not define !") );
  
  }

#else

  SInt32 EFRIO_USR__FOnAppStartByRunCtrlExt ( SInt8 MapsName, char* RunCtrlIPAddr, SInt32 RunCtrlPort, SInt32 DaqPort, SInt8 CrIndexFile, SInt32 ReadoutMode, SInt8 ErrLogLvl, SInt8 MsgLogLvl )  {
    
    SInt32 VRet;
    char*  VPtLastMsgToGui             = EFRIO__USR_VGContext.LastMsgToGui;
    EFRIO__USR_TContext* VPtUsrContext = &EFRIO__USR_VGContext;
    
    // Variables to set error & messages log files parameters
    
    SInt8 VErrFileLogLvl;
    char  VErrLogFilePath[GLB_FILE_PATH_SZ];
    SInt8 VMsgFileLogLvl;
    char  VMsgLogFilePath[GLB_FILE_PATH_SZ];
    
    SInt32 VMyVar = 10; // For error & general messages demo
    
    
    // Param settings - Moved here on 14/06/13

    sprintf ( VPtUsrContext->ParRunCtrlIPAddr, "%s", RunCtrlIPAddr );
    
    VPtUsrContext->ParRunCtrlPort = RunCtrlPort;
    VPtUsrContext->ParDaqPort     = DaqPort;
    VPtUsrContext->ParCrIndexFile = CrIndexFile;
    VPtUsrContext->ParReadoutMode = ReadoutMode;
    

    // -----------------------------------------------------------------------
    // Default values for error & messages log files are hard coded here
    // But they can be overwritten by parameters from command line for example
    // -----------------------------------------------------------------------
    
    VErrFileLogLvl = ErrLogLvl; // ERR_LOG_LVL_ALL; // 0 = ERR_LOG_LVL_NONE, 1 = ERR_LOG_LVL_ALL, 2 = ERR_LOG_LVL_WARNINGS_ERRORS, 3 = ERR_LOG_LVL_ERRORS
    
    sprintf ( VErrLogFilePath, "%s", "x:\\log\\err_eudet_dll.txt" );
    
    VMsgFileLogLvl = MsgLogLvl; // 127;
    
    sprintf ( VMsgLogFilePath, "%s", "x:\\log\\msg_eudet_dll.txt" );
    
    // -----------------------------------------------------------------------
    // Overwrite here variables to conf error & messages log files parameters
    // Values from command line arguments can be use
    // -----------------------------------------------------------------------
    
    // VErrFileLogLvl =  // ERR_LOG_LVL_NONE, ERR_LOG_LVL_ALL, ERR_LOG_LVL_WARNINGS_ERRORS, ERR_LOG_LVL_ERRORS
    
    // sprintf ( VErrLogFilePath, "%s", "" );
    
    // VMsgFileLogLvl
    
    // sprintf ( VMsgLogFilePath, "%s", "" );
    
    
    // -----------------------------------------------------------------------
    // Flex rio lib init
    // -----------------------------------------------------------------------
    
    // Call init function
    // 25/05/2011 => Replace EFRIO__FBegin call by EFRIO__FBeginExt in order to handle MapsName
        
    VRet = EFRIO__FBeginExt ( MapsName, VErrFileLogLvl, VErrLogFilePath, VMsgFileLogLvl, VMsgLogFilePath );
    
    // Copy error message from err lib to last GUI message - user can write something else if more useful
    
    if ( VRet < 0 ) {
      snprintf ( VPtLastMsgToGui, GLB_CMT_SZ, "%s", ERR_FGetLastErrMsg () );
      return (VRet);
    }
    
    // Set last GUI message to "OK"
    
    snprintf ( VPtLastMsgToGui, GLB_CMT_SZ-1, "OK - General init done :-)" );

    // -----------------------------------------------------------------------
    // Run control handling initialization
    // -----------------------------------------------------------------------
      
    // Before 14/06/13 => Moved at beginning of function 

/*
    sprintf ( VPtUsrContext->ParRunCtrlIPAddr, "%s", RunCtrlIPAddr );
    
    VPtUsrContext->ParRunCtrlPort = RunCtrlPort;
    VPtUsrContext->ParDaqPort     = DaqPort;
    VPtUsrContext->ParCrIndexFile = CrIndexFile;
    VPtUsrContext->ParReadoutMode = ReadoutMode;
*/    
        
    // Conf sender
    
    VPtUsrContext->InfRunCtrlSenderId  = IAC__FSockInstallSenderPort ( VPtUsrContext->ParRunCtrlIPAddr /* DestHost */, VPtUsrContext->ParDaqPort /* DestPort */, 0 /* LongMsg */,  10000 /* AckTimeOutMs */, 5 /* RetryNb */, NULL /* APP_FCheckSum */ /* FuncCheckSum */ );
    
    // Start sender
        
    IAC__FSockStartSenderPort ( VPtUsrContext->InfRunCtrlSenderId );
    
    // Conf receiver
    
    VPtUsrContext->InfRunCtrlReceiverId = IAC__FSockInstallReceiverPort     ( VPtUsrContext->ParRunCtrlPort /* RecPort */, 0 /* LongMsg */, EFRIO_USR__FHandleRunCtrlMsg /* IAC__TFSockUsrHandleRecMsg */, NULL /* APP_FCheckSum */ /* FuncCheckSum */ );
    
    // Start receiver

    IAC__FSockStartReceiverPort ( VPtUsrContext->InfRunCtrlReceiverId );


    // -----------------------------------------------------------------------
    // User part of flex rio lib init
    // -----------------------------------------------------------------------
    
    // Call init function
    
    VRet = EFRIO_USR__FBegin ();
    
    // Copy error message from err lib to last GUI message - user can write something else if more useful
    
    if ( VRet < 0 ) {
      snprintf ( VPtLastMsgToGui, GLB_CMT_SZ, "%s", ERR_FGetLastErrMsg () );
      return (VRet);
    }
    
    // Set last GUI message to "OK"
    
    snprintf ( VPtLastMsgToGui, GLB_CMT_SZ-1, "OK - General & user init done :-)" );
    
    
    // -----------------------------------------------------------------------
    // Error messages demo
    // -----------------------------------------------------------------------
    
    err_trace   (( ERR_OUT, "This is a trace message from DLL   - VMyVar=%d", VMyVar ));
    err_warning (( ERR_OUT, "This is a warning message from DLL - VMyVar=%d", VMyVar ));
    err_error   (( ERR_OUT, "This is an error message from DLL  - VMyVar=%d", VMyVar ));
    
    // -----------------------------------------------------------------------
    // General messages demo
    // -----------------------------------------------------------------------
    
    msg  (( MSG_OUT, "This is a general message from DLL - with default LogLvl = 1 - VMyVar=%d", VMyVar ));
    msgl (( MSG_OUT, "This is a general message from DLL - with LogLvl = 2         - VMyVar=%d", VMyVar ), 2 );
    
    
    msg (( MSG_OUT, "RunCtrlIPAddr=%s, RunCtrlPort=%d, DaqPort=%d, CrIndexFile=%d, ReadoutMode=%d, ParCrIndexFile=%d", RunCtrlIPAddr, RunCtrlPort, DaqPort, CrIndexFile, ReadoutMode, VPtUsrContext->ParCrIndexFile ));
      
    
    err_retok (( ERR_OUT, "" ));
  }
  

#endif



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO_USR__FOnAppStartByRunCtrl ( char* RunCtrlIPAddr, SInt32 RunCtrlPort, SInt32 DaqPort )
          :
Goal      : This function is called when application is started. It will do all library
          : initializations.
          :
Inputs    : None
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   : Write to EFRIO__USR_VGContext.LastMsgToGui.
          :
Remark    : It can use command line arguments to select operating modes, debug level, etc ...
          : Update the global variable GUI message -> EFRIO__USR_VGContext.LastMsgToGui
          :
Level     :
Date      : 03/02/2011
          :
Rev       : 25/05/2011
          : - Body of function is " empty " => it has been moved in EFRIO_USR__FOnAppStartByRunCtrlExt (...)
          : - Call EFRIO_USR__FOnAppStartByRunCtrlExt ( ASIC__MI26, ... )
          : - EFRIO_USR__FOnAppStartByRunCtrlExt (...) handles MapsName parameter ( Mi26 / Ultimate / ... )
          :   EFRIO_USR__FOnAppStartByRunCtrl still configure Mimosa 26 as it was done in first version of lib.
          :
          :
Doc date  : 03/02/2011
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


#ifndef EFRIO__INCLUDE_RUN_CTRL

SInt32 EFRIO_USR__FOnAppStartByRunCtrl ( char* RunCtrlIPAddr, SInt32 RunCtrlPort, SInt32 DaqPort, SInt8 CrIndexFile, SInt32 ReadoutMode )  {
  
  err_retfail ( -1, (ERR_OUT,"Function no available => EFRIO__INCLUDE_RUN_CTRL not define !") );
  
}

#else

SInt32 EFRIO_USR__FOnAppStartByRunCtrl ( char* RunCtrlIPAddr, SInt32 RunCtrlPort, SInt32 DaqPort, SInt8 CrIndexFile, SInt32 ReadoutMode )  {
  
  return ( EFRIO_USR__FOnAppStartByRunCtrlExt ( ASIC__MI26, RunCtrlIPAddr, RunCtrlPort, DaqPort, CrIndexFile, ReadoutMode, ERR_LOG_LVL_ERRORS /* ErrLogLvl */, 127 /* MsgLogLvl */ ) );
  
// Before 14/06/13  return ( EFRIO_USR__FOnAppStartByRunCtrlExt ( ASIC__MI26, RunCtrlIPAddr, RunCtrlPort, DaqPort, 0 /* CrIndexFile */, 0 /* ReadoutMode */, ERR_LOG_LVL_ERRORS /* ErrLogLvl */, 127 /* MsgLogLvl */ ) );
}
   
#endif


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO_USR__FOnConfExt ( SInt8 MapsName )
          :
Goal      : This function is called by the EUDET event method which configure run parameters
          : It will do all run initializations, including Mi26 configuration by JTAG. 
          :
          !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
          : WARNING - THIS FUNCTION IS NOT USED ON IPHC DAQ, IT'S ONLY USED ON EUDET DEMO !!!
          !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!          
          :
Inputs    : None
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   : Write to EFRIO__USR_VGContext.LastMsgToGui.
          :
Remark    : Update the global variable GUI message -> EFRIO__USR_VGContext.LastMsgToGui
          :
Level     :
Date      : 21/12/2010
Rev       : 12/03/2011
          : - Exit if called while DAQ is taking data
          :
          : 12/05/2011
          : - Implement handling of different MAPS ( Mi26, Ultimate, ... )
          : -> Replace call to EFRIO__FConfRun by EFRIO__FConfRunExt ( new param MapsName )
          : -> Move the call to EFRIO__FConfRunExt BEFORE JTAG functions call because they
          :    use the ParMapsName field of run context record.
          : -> Replace Mi26 JTAG functions by generic ones ( handling Mi26, Ult1, ... )
          :
          : 23/05/2011 
          : -> Configure Ultimate 1 for test purpose ( => Ult1 FW debug without run ctrl )
          :
          : 25/05/2011
          : - Renamed in EFRIO_USR__FOnConfExt
          : - Add parameter MapsName
          : - Create a new EFRIO_USR__FOnConf (...) which call EFRIO_USR__FOnConfExt ( ASIC__MI26, ... )
          :   This will keep compatibility with previous sw (EUDET), because EFRIO_USR__FOnConf call
          :   will configure Mimosa 26 as ASIC as it was done for first version of EUDET library.
          :
          : 29/10/2013
          : - ONLY on version V3.0 defined by CC => EFRIO__V30 in DLL cpp file
          :   Clock distrib board configuration and stop clock 
          :
          : 28/11/2013
          : - Bug fix on clock distrib board configuration (29/10/13) configuration request
          :   was not executed, and therefore the board was not configured
          :
Doc date  : 21/12/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO_USR__FOnConfExt ( SInt8 MapsName ) {
  
  EFRIO__TContext*     VPtCont  = &EFRIO__VGContext;
  char* VPtLastMsgToGui = EFRIO__USR_VGContext.LastMsgToGui;

  SInt32 VRet; // Variable to store error code of functions called
  
  // JTAG file
  
  char   VJtagConfFile[GLB_FILE_PATH_SZ]; // JTAG master conf file ( *.mcf )
  
  // Run parameters
  
  SInt8  VMapsName;       // Name of the MAPS ( ASIC__MI26, ASIC__ULT1, ... )
  SInt8  VMapsNb;         // Number of MAPS
  SInt32 VRunNo;          // No of run
  SInt32 VTotEvNb;        // Total event number of run
  SInt32 VEvNbPerFile;    // Number of events per file
  SInt32 VFrNbPerAcq;     // Number of frames per acquisition
  SInt8  VDataTrfMode;    // Data transfer mode IPHC, EUDET 1, 2, 3
  
  char   VDestDir[GLB_FILE_PATH_SZ];        // Destination directory forrun file
  char   VFileNamePrefix[GLB_FILE_PATH_SZ]; // Run file name prefix, eg "RUN_666" => prefix  = RUN_
  
  SInt8  VSaveOnDisk;     // If > 0 => save data on disk
                          // 1 => use multi threading in TCStreamFile class
                          // 2 => don't use multi threading in TCStreamFile class
  
  SInt8  VSendOnEth;      // If > 0 send data on Ethernet
  SInt8  VSendOnEthPCent; // % of event send on Ethernet



  // -----------------------------------------------------------------------
  // Debug message
  // -----------------------------------------------------------------------
  
  msg (( MSG_OUT, "EFRIO_USR__FOnConf (...) called" ));
  
  // -----------------------------------------------------------------------
  // Reject cmd if run is already running
  // -----------------------------------------------------------------------
  
  if ( VPtCont->RunCont.CmdRun != -1 ) {
    err_retfail ( -1, (ERR_OUT,"Conf run cmd received - Abort => Because DAQ is already runnig !" ) );
  }
  
  // -----------------------------------------------------------------------
  // Set default value to JTAG configuration file
  // -----------------------------------------------------------------------
  
  
  switch ( MapsName ) {
    
    case ASIC__MI26 : {
      sprintf ( VJtagConfFile, "%s", "C:\\ccmos_sctrl\\MIMOSA26_JTAG\\config_files\\daq_test_2x80MHz_1_chip.mcf" );
      //  sprintf ( VJtagConfFile, "%s", "C:\\ccmos_sctrl\\MIMOSA26_JTAG\\config_files\\daq_test_2x80MHz_6_chip.mcf" );
      break; }
    
    case ASIC__ULT1 : {
      sprintf ( VJtagConfFile, "%s", "C:\\ccmos_sctrl\\MIMOSA28_JTAG\\config_files\\MI28_2x160MHz.mcf" );
      break; }
    
    default : {
      err_retfail ( -1, (ERR_OUT,"Unknown MAPS name = %d => Abort", MapsName) );
      break; }
    
  }
    
  
  // -----------------------------------------------------------------------
  // Set default values to run parameters
  // -----------------------------------------------------------------------
  
//  VMapsName       = ASIC__MI26;
//  VMapsName       = ASIC__ULT1; // 23/05/2011 => Ultimate fw debug
  VMapsName       = MapsName; // 25/05/11 => Use MapsName parameter
  VMapsNb         =     6;
  VRunNo          =   777;
  VTotEvNb        = 10000;
  VEvNbPerFile    =  1000;
  
  switch ( MapsName ) {
    
    case ASIC__MI26 : {
      VFrNbPerAcq     =  1800;
      break; }
    
    case ASIC__ULT1 : {
      VFrNbPerAcq     =  400;
      break; }
    case ASIC__FSBB0 : {
      VFrNbPerAcq     =  200;
      break; }    
    default : {
      err_retfail ( -1, (ERR_OUT,"Unknown MAPS name = %d => Abort", MapsName) );
      break; }
    
  }
  
  
  //  VDataTrfMode    = EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG;
  VDataTrfMode    = EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES;
  
  sprintf ( VDestDir       , "%s", "c:\\data" );
  sprintf ( VFileNamePrefix, "%s", "RUN_"       );
  
  VSaveOnDisk     = 0; // 0 = dont save, 1 = save multithreading, 2 = save NO multithreading
  VSendOnEth      = 1; 
  VSendOnEthPCent = 20;
  
  // -----------------------------------------------------------------------
  // Overwrite JTAG configuration file with name provided by EUDET DAQ
  // -----------------------------------------------------------------------
  
  // sprintf ( VJtagConfFile, "%s", "" );
  
  // -----------------------------------------------------------------------
  // Overwrite here run parameters with values provided by EUDET DAQ
  // -----------------------------------------------------------------------

  // VMi26Nb         =    
  // VRunNo          =   
  // VTotEvNb        = 
  // VEvNbPerFile    =  
  // VFrNbPerAcq     =  
  // VDataTrfMode    =  
  
  // sprintf ( VDestDir       , "%s", "" );
  // sprintf ( VFileNamePrefix, "%s", "" );
  
  // VSaveOnDisk     = 
  // VSendOnEth      = 
  // VSendOnEthPCent = 
  
  // -----------------------------------------------------------------------
  // Call conf run function
  // -----------------------------------------------------------------------
  // Moved here on 12/05/2011
  // -----------------------------------------------------------------------
  
  // Call DLL run configuration function with parameters get from GUI
  
  VRet = EFRIO__FConfRunExt (
    VMapsName,
    VMapsNb,
    VRunNo,
    VTotEvNb,
    VEvNbPerFile,
    VFrNbPerAcq,
    VDataTrfMode,
    0 /* TrigMode */,
    VDestDir,
    VFileNamePrefix,
    VSaveOnDisk,
    VSendOnEth,
    VSendOnEthPCent,
    VJtagConfFile );
    
  // -----------------------------------------------------------------------
  // Print run parameters in messages log file x:\log\msg_eudet_dll.txt
  // -----------------------------------------------------------------------
  
  EFRIO__FPrintRunCont ();
  
  // Copy error message from err lib to last GUI message - user can write something else if more useful
  
  if ( VRet < 0 ) {
    snprintf ( VPtLastMsgToGui, GLB_CMT_SZ, "%s", ERR_FGetLastErrMsg () );
    return (VRet);
  }


  // -----------------------------------------------------------------------
  // Configure clock distrib board and stop clock
  // -----------------------------------------------------------------------
  // 29/10/13 : Implementation, but not tested in detail -> Hidden bug
  // 28/11/13 : Add SPICD_FStartCom () call because otherwise the
  //            SPICD_FSendConf () is not exectuted => board not configured
  // -----------------------------------------------------------------------
  
  #ifdef EFRIO__V30
    msg (( MSG_OUT, "$$$ Clock distrib board config cmd sent" ));
    SPICD_FStartCom (); // 28/11/13 Bug fix : SPICD_FStartCom () call needed otherwise SPICD_FSendConf () is not executed
    SPICD_FSendConf ();
    Sleep ( 1000 );     // Delay 1000 ms to be sure that board has been configured
    PPO_FOutD7 ( 0 /* Id */, 1 /* State */ ); // Stop clock
  #endif

  // -----------------------------------------------------------------------
  // Print JTAG file name in messages log file x:\log\msg_eudet_dll.txt
  // -----------------------------------------------------------------------
  
  msg (( MSG_OUT, "JTAG configuration file = %s", VJtagConfFile ));
  
  // -----------------------------------------------------------------------
  // Load JTAG conf file, Reset & send parameters to Mi26
  // -----------------------------------------------------------------------
    
  // Load file
    
  VRet = EFRIO__FJtagLoadFile ( VJtagConfFile );
  
  // Copy error message from err lib to last GUI message - user can write something else if more useful
  
  if ( VRet < 0 ) {
    snprintf ( VPtLastMsgToGui, GLB_CMT_SZ, "%s", ERR_FGetLastErrMsg () );
    return (VRet);
  }

  // Reset Mi 26

  VRet = EFRIO__FJtagReset ();
  
  // Copy error message from err lib to last GUI message - user can write something else if more useful
  
  if ( VRet < 0 ) {
    snprintf ( VPtLastMsgToGui, GLB_CMT_SZ, "%s", ERR_FGetLastErrMsg () );
    return (VRet);
  }
  
  // Send parameters to Mimosa 26 registers
  // If read back registers <> values sent => an error is generated => VRet < 0 
  
  VRet =  EFRIO__FJtagLoadChip ();
  
  // Copy error message from err lib to last GUI message - user can write something else if more useful
  
  if ( VRet < 0 ) {
    snprintf ( VPtLastMsgToGui, GLB_CMT_SZ, "%s", ERR_FGetLastErrMsg () );
    return (VRet);
  }
  
  // Set last GUI message to "OK"
  
  snprintf ( VPtLastMsgToGui, GLB_CMT_SZ-1, "OK - JTAG parameters loaded to Mi26 :-)" );


  // -----------------------------------------------------------------------
  // Call conf run function
  // -----------------------------------------------------------------------
  // Was here before 12/05/2011
  // Moved before JTAG functions call on 12/05/2011
  // because they need to know which MAPS is used ( Run context -> ParMapsName )
  // -----------------------------------------------------------------------
  

  
  // Set last GUI message to "OK"
  
  snprintf ( VPtLastMsgToGui, GLB_CMT_SZ-1, "OK - Conf run done :-)" );  

  err_retok (( ERR_OUT, "" ));
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO_USR__FOnConf ()
          :
Goal      : This function is called by the EUDET event method which configure run parameters
          : It will do all run initializations, including Mi26 configuration by JTAG.
          :
Inputs    : None
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   : Write to EFRIO__USR_VGContext.LastMsgToGui.
          :
Remark    : Update the global variable GUI message -> EFRIO__USR_VGContext.LastMsgToGui
          :
Level     :
Date      : 21/12/2010
Rev       : 12/03/2011
          : - Exit if called while DAQ is taking data
          :
          : 12/05/2011
          : - Implement handling of different MAPS ( Mi26, Ultimate, ... )
          : -> Replace call to EFRIO__FConfRun by EFRIO__FConfRunExt ( new param MapsName )
          : -> Move the call to EFRIO__FConfRunExt BEFORE JTAG functions call because they
          :    use the ParMapsName field of run context record.
          : -> Replace Mi26 JTAG functions by generic ones ( handling Mi26, Ult1, ... )
          :
          : 23/05/2011
          : -> Configure Ultimate 1 for test purpose ( => Ult1 FW debug without run ctrl )
          :
          : 25/05/2011
          : - Body of function is " empty " => it has been moved in EFRIO_USR__FOnConfExt (...)
          : - Call EFRIO_USR__FOnConfExt ( ASIC__MI26, ... )
          : - EFRIO_USR__FOnConfExt (...) handles MapsName parameter ( Mi26 / Ultimate / ... )
          :   EFRIO_USR__FOnConf still configure Mimosa 26 as it was done in first version of lib.
          :
Doc date  : 21/12/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO_USR__FOnConf () {

  return ( EFRIO_USR__FOnConfExt ( ASIC__FSBB0 ) );
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO_USR__FOnConfByRunCtrl ( EFRIO__TRunCont* PtRunCont )
          :
Goal      : This function is called by the EUDET event method which configure run parameters
          : It will do all run initializations, including Mi26 configuration by JTAG.
          :
Inputs    : None
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   : Write to EFRIO__USR_VGContext.LastMsgToGui.
          :
Remark    : Update the global variable GUI message -> EFRIO__USR_VGContext.LastMsgToGui
          :
Level     :
Date      : 03/02/2011
Rev       : 12/03/2011
          : - Exit if called while DAQ is taking data
          :
          : 12/05/2011
          : - Implement handling of different MAPS ( Mi26, Ultimate, ... )
          : -> Replace call to EFRIO__FConfRun by EFRIO__FConfRunExt ( new param MapsName )
          : -> Move the call to EFRIO__FConfRunExt BEFORE JTAG functions call because they
          :    use the ParMapsName field of run context record.
          : -> Replace Mi26 JTAG functions by generic ones ( handling Mi26, Ult1, ... )
          :
          : 01/03/2013
          : - Handle EFRIO__USR_VGContext.DisableRunCtrlMsgProcessing
          :
          : 28/11/2013
          : - ONLY on version V3.0 defined by CC => EFRIO__V30 in DLL cpp file
          :   Clock distrib board configuration and stop clock
          :   Copy of code from EFRIO_USR__FOnConfExt () 29/10/13 here, because it has been
          :   first implemented in EFRIO_USR__FOnConfExt () - EUDET demo sw - and I have
          :   forgotten to implement it here. A bug on board configuration has also been
          :   solved, see EFRIO_USR__FOnConfExt () text header.
          :
Doc date  : 03/02/2011
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


#ifndef EFRIO__INCLUDE_RUN_CTRL

SInt32 EFRIO_USR__FOnConfByRunCtrl ( EFRIO__TRunCont* PtRunCont )  {
  
  err_retfail ( -1, (ERR_OUT,"Function no available => EFRIO__INCLUDE_RUN_CTRL not define !") );
    
}

#else

SInt32 EFRIO_USR__FOnConfByRunCtrl ( EFRIO__TRunCont* PtRunCont ) {
  

  EFRIO__USR_TContext*  VPtUsrCont      = &EFRIO__USR_VGContext;
  EFRIO__TContext*      VPtCont         = &EFRIO__VGContext;
  char*                 VPtLastMsgToGui = EFRIO__USR_VGContext.LastMsgToGui;
  SInt8                 VMaxTryToConfMaps;
  SInt8                 VCptTryToConfMaps;
  SInt32                VTimeMsBetweenTryToConfMaps;
  SInt32                VRet; // Variable to store error code of functions called
 


  // -----------------------------------------------------------------------
  // Debug message
  // -----------------------------------------------------------------------
  
  VMaxTryToConfMaps           = 10; // Nb max tentatives to configure MAPS
  VTimeMsBetweenTryToConfMaps = 1000;
  
  msg (( MSG_OUT, "EFRIO_USR__FOnConfByRunCtrl (...) called - Try to conf MAPS %d times", VMaxTryToConfMaps ));

  // -----------------------------------------------------------------------
  // Display command if display is enabled
  // Reject command if commands processing is disabled
  // -----------------------------------------------------------------------

  if ( VPtUsrCont->ParEnableRunCtrlMsgDisplay == 1 ) {
    
    if ( PtRunCont != NULL ) {
      sprintf ( VPtUsrCont->ResRunCtrlMsgStr, "Run Ctrl command => Conf run : Conf param in msg.txt" );
      VPtUsrCont->ResRunCtrlMsgReady = 1;
      EFRIO__FPrintRunContRec ( PtRunCont );
    }
    
    else {
      sprintf ( VPtUsrCont->ResRunCtrlMsgStr, "Run Ctrl command => Conf run : No conf param (ptr NULL) !" );
      VPtUsrCont->ResRunCtrlMsgReady = 1;
    }
    
  }
    
  if ( VPtUsrCont->ParDisableRunCtrlMsgProcessing == 1 ) {
    err_retok (( ERR_OUT, "Exit => Run Ctrl cmd processing disabled !" ));
  }

  // -----------------------------------------------------------------------
  // Reject command if run is already running
  // -----------------------------------------------------------------------
  
  if ( VPtCont->RunCont.CmdRun != -1 ) {
    err_retfail ( -1, (ERR_OUT,"Conf run cmd received - Abort => Because DAQ is already runnig !" ) );
  }
  
  // -----------------------------------------------------------------------
  // Test run context pointer
  // -----------------------------------------------------------------------
  
  err_retnull ( PtRunCont, (ERR_OUT,"PtRunCont == NULL") );
  

  // -----------------------------------------------------------------------
  // Call conf run function
  // -----------------------------------------------------------------------
  // Moved here on 12/05/2011
  // -----------------------------------------------------------------------
  
  // -----------------------------------------------------------------------
  // Call conf run function
  // -----------------------------------------------------------------------
  
  // Call DLL run configuration function with parameters get from GUI
  
  msg (( MSG_OUT, "Call EFRIO__FConfRun" ));
  
  VRet = EFRIO__FConfRunExt (
    PtRunCont->ParMapsName,
    PtRunCont->ParMi26Nb,
    PtRunCont->ParRunNo,
    PtRunCont->ParTotEvNb,
    PtRunCont->ParEvNbPerFile,
    PtRunCont->ParFrameNbPerAcq,
    PtRunCont->ParDataTransferMode,
    0 /* TrigMode */,
    PtRunCont->ParDestDir,
    PtRunCont->ParFileNamePrefix,
    PtRunCont->ParSaveOnDisk,
    PtRunCont->ParSendOnEth,
    PtRunCont->ParSendOnEthPCent,
    PtRunCont->ParJtagFileName );
    
  // -----------------------------------------------------------------------
  // Print run parameters in messages log file x:\log\msg_eudet_dll.txt
  // -----------------------------------------------------------------------
  
  msg (( MSG_OUT, "$CALL => EFRIO_USR__FOnConfByRunCtrl ()" )); // 01/03/2013
  
  // EFRIO__FPrintRunCont ();
  
  
  // Copy error message from err lib to last GUI message - user can write something else if more useful
  
  if ( VRet < 0 ) {
    snprintf ( VPtLastMsgToGui, GLB_CMT_SZ, "%s", ERR_FGetLastErrMsg () );
    return (VRet);
  }
  
  // -----------------------------------------------------------------------
  // Configure clock distrib board and stop clock
  // Copy code from EFRIO_USR__FOnConfExt () 29/10/13 here
  // -----------------------------------------------------------------------
  // 29/10/13 : Implementation, but not tested in detail -> Hidden bug
  // 28/11/13 : Add SPICD_FStartCom () call because otherwise the
  //            SPICD_FSendConf () is not exectuted => board not configured
  // -----------------------------------------------------------------------
   
  #ifdef EFRIO__V30
    msg (( MSG_OUT, "$$$ Clock distrib board config cmd sent" ));
    SPICD_FStartCom (); // 28/11/13 Bug fix : SPICD_FStartCom () call needed otherwise SPICD_FSendConf () is not executed
    SPICD_FSendConf ();
    Sleep ( 1000 );     // Delay 1000 ms to be sure that board has been configured
    PPO_FOutD7 ( 0 /* Id */, 1 /* State */ ); // Stop clock
  #endif
  
  
  
  // -----------------------------------------------------------------------
  // Print JTAG file name in messages log file x:\log\msg_eudet_dll.txt
  // -----------------------------------------------------------------------
  
  msg (( MSG_OUT, "RC => JTAG configuration file = %s", PtRunCont->ParJtagFileName ));
  
  // -----------------------------------------------------------------------
  // Load JTAG conf file, Reset & send parameters to Mi26
  // -----------------------------------------------------------------------
    
  // Load file
  
  VRet = EFRIO__FJtagLoadFile ( PtRunCont->ParJtagFileName );
    
  // Copy error message from err lib to last GUI message - user can write something else if more useful
  
  if ( VRet < 0 ) {
    snprintf ( VPtLastMsgToGui, GLB_CMT_SZ, "%s", ERR_FGetLastErrMsg () );
    return (VRet);
  }
      
  // Try to Reset & Configure MAPS - Before 30/04/13 it was done one time, there was no retry
   
  for ( VCptTryToConfMaps=0; VCptTryToConfMaps < VMaxTryToConfMaps; VCptTryToConfMaps++ ) {
  
    // Reset Maps
  
    VRet = EFRIO__FJtagReset ();
  
    // Copy error message from err lib to last GUI message - user can write something else if more useful
  
    if ( VRet < 0 ) {
      snprintf ( VPtLastMsgToGui, GLB_CMT_SZ, "%s", ERR_FGetLastErrMsg () );
      return (VRet);
    }
    
    // Send parameters to Maps registers
    // If read back registers <> values sent => retry
    // Tries to oad two times after a reset
  
    VRet =  EFRIO__FJtagLoadChip ();
  
    if ( VRet == 0 ) {
      break; // MAPS loaded & readback OK
    }
  
    VRet =  EFRIO__FJtagLoadChip ();
  
    if ( VRet == 0 ) {
      break; // MAPS loaded & readback OK
    }
      
    // Wait before next try
    
    Sleep ( VTimeMsBetweenTryToConfMaps );

  } // End for ()
  

  // Copy error message from err lib to last GUI message - user can write something else if more useful
  
  if ( VRet < 0 ) {
    snprintf ( VPtLastMsgToGui, GLB_CMT_SZ, "%s", ERR_FGetLastErrMsg () );
    return (VRet);
  }
  

  // Set last GUI message to "OK"

  msg (( MSG_OUT, "Set last GUI message" ));
  
  snprintf ( VPtLastMsgToGui, GLB_CMT_SZ-1, "OK - JTAG parameters loaded to Mi26 :-)" );


  // -----------------------------------------------------------------------
  // Call conf run function
  // -----------------------------------------------------------------------
  // Was here before 12/05/2011
  // Moved before JTAG functions call on 12/05/2011
  // because they need to know which MAPS is used ( Run context -> ParMapsName )
  // -----------------------------------------------------------------------
  

  
  // Set last GUI message to "OK"
  
  snprintf ( VPtLastMsgToGui, GLB_CMT_SZ-1, "OK - Conf run done :-)" );
  
  err_retok (( ERR_OUT, "" ));
}

#endif

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO_USR__FOnStartRun ()
          :
Goal      : This function is called by the EUDET event method which start run.
          :
Inputs    : None
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   : Write to EFRIO__USR_VGContext.LastMsgToGui.
          :
Remark    : Update the global variable GUI message -> EFRIO__USR_VGContext.LastMsgToGui
          :
Level     :
Date      : 21/12/2010
Rev       : 07/03/2001
          : - Add reset & reload Mi26 
          : 12/03/2011
          : - Exit if called while DAQ is taking data
          :
          : 12/05/2011
          : -> Replace Mi26 JTAG functions by generic ones ( handling Mi26, Ult1, ... )                    
          :          
Doc date  : 21/12/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO_USR__FOnStartRun () {
  
  EFRIO__TContext*     VPtCont  = &EFRIO__VGContext;
  char* VPtLastMsgToGui = EFRIO__USR_VGContext.LastMsgToGui;


  // -----------------------------------------------------------------------
  // Debug message
  // -----------------------------------------------------------------------
  
  msg (( MSG_OUT, "EFRIO_USR__FOnStartRun (...) called" ));
  
  // -----------------------------------------------------------------------
  // Reject cmd if run is already running
  // -----------------------------------------------------------------------
  
  if ( VPtCont->RunCont.CmdRun != -1 ) {
    err_retfail ( -1, (ERR_OUT,"Start run cmd received - Abort => Because DAQ is already runnig !" ) );
  }
  
  // ---------------------------------------------------------------------
  // Reset and reload all Mi26 - 07/03/2011
  // ---------------------------------------------------------------------
    
  EFRIO__FJtagReset ();
  
  EFRIO__FJtagLoadChip ();
    
  // ---------------------------------------------------------------------
  // If saving is enabled ( run par ) => create run conf/par & data files
  // ---------------------------------------------------------------------
  
  EFRIO__FStartSavingOnFile ();
  
  // --------------------------------------------------------------------------------------
  // Set the flag CmdRun of record EFRIO__TRunCont which will be polled by DAQ application
  // --------------------------------------------------------------------------------------
  
  EFRIO__FSetCmdRun (1);

  // Set last GUI message to "OK"
  
  snprintf ( VPtLastMsgToGui, GLB_CMT_SZ-1, "OK - Run started :-)" );
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO_USR__FOnStartRunByRunCtrl ( )
:
Goal      : This function is called by the EUDET event method which start run.
:
Inputs    : None
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   : Write to EFRIO__USR_VGContext.LastMsgToGui.
:
Remark    : Update the global variable GUI message -> EFRIO__USR_VGContext.LastMsgToGui
:
Level     :
Date      : 03/02/2011
Rev       : 07/03/2001
          : - Add reset & reload Mi26
          : 12/03/2011
          : - Exit if called while DAQ is taking data
          :
          : 12/05/2011
          : -> Replace Mi26 JTAG functions by generic ones ( handling Mi26, Ult1, ... )
          :
          : 16/07/2012
          : -> Removed JTAB access to implement Mi32 -> Mi26 BT run control
          :
          : 01/03/2013
          : - Handle EFRIO__USR_VGContext.DisableRunCtrlMsgProcessing
          :
Doc date  : 03/02/2011
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


#ifndef EFRIO__INCLUDE_RUN_CTRL

SInt32 EFRIO_USR__FOnStartRunByRunCtrl ()  {
  
  err_retfail ( -1, (ERR_OUT,"Function no available => EFRIO__INCLUDE_RUN_CTRL not define !") );
    
}

#else
  
SInt32 EFRIO_USR__FOnStartRunByRunCtrl () {
  
  EFRIO__USR_TContext*  VPtUsrCont = &EFRIO__USR_VGContext;  
  EFRIO__TContext*      VPtCont  = &EFRIO__VGContext;  
  char*                 VPtLastMsgToGui = EFRIO__USR_VGContext.LastMsgToGui;

  // -----------------------------------------------------------------------
  // Debug message
  // -----------------------------------------------------------------------
  
  msg (( MSG_OUT, "$CALL => EFRIO_USR__FOnStartRunByRunCtrl ()" )); // 01/03/2013

  // -----------------------------------------------------------------------
  // Display command if display is enabled
  // Reject command if commands processing is disabled
  // -----------------------------------------------------------------------
  
  if ( VPtUsrCont->ParEnableRunCtrlMsgDisplay == 1 ) {
    sprintf ( VPtUsrCont->ResRunCtrlMsgStr, "Run Ctrl command => Start run" );
    VPtUsrCont->ResRunCtrlMsgReady = 1;
  }
  
  if ( VPtUsrCont->ParDisableRunCtrlMsgProcessing == 1 ) {
    err_retok (( ERR_OUT, "Exit => Run Ctrl cmd processing disabled !" ));
  }
  

  // -----------------------------------------------------------------------
  // Reject command if conf run has failed - 30/04/13
  // -----------------------------------------------------------------------
  
  if ( VPtUsrCont->InfFOnConfByRunCtrlRetCode < 0 ) {
    err_retfail ( -1, (ERR_OUT,"Start run cmd received - Abort => Because Conf run has failed !" ) );
  }

  // -----------------------------------------------------------------------
  // Reject command if run is already running
  // -----------------------------------------------------------------------
  
  if ( VPtCont->RunCont.CmdRun != -1 ) {
    err_retfail ( -1, (ERR_OUT,"Start run cmd received - Abort => Because DAQ is already runnig !" ) );
  }

  // ---------------------------------------------------------------------
  // Reset and reload all Mi26 - 07/03/2011
  // ---------------------------------------------------------------------
  
  // JTAG access removed on 16/07/2012 in order to implement Mi32A -> Mi26 DAQ BT run control
  // JTAG reset & conf already done in EFRIO_USR__FOnConfByRunCtrl
  //
  // EFRIO__FJtagReset ();
  //
  // EFRIO__FJtagLoadChip ();
  
  // ---------------------------------------------------------------------
  // If saving is enabled ( run par ) => create run conf/par & data files
  // ---------------------------------------------------------------------
  
  EFRIO__FStartSavingOnFile ();
  
  // --------------------------------------------------------------------------------------
  // Set the flag CmdRun of record EFRIO__TRunCont which will be polled by DAQ application
  // --------------------------------------------------------------------------------------
  
  EFRIO__FSetCmdRun (1);
  
  // Set last GUI message to "OK"
  
  snprintf ( VPtLastMsgToGui, GLB_CMT_SZ-1, "OK - Run started :-)" );
  
  err_retok (( ERR_OUT, "" ));
}

#endif

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO_USR__FOnGetRunStatus ()
          :
Goal      : This function is called by the EUDET event method which request run status.
          :
Inputs    : None
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   : Write to EFRIO__USR_VGContext.LastMsgToGui.
          :
Remark    : Update the global variable GUI message -> EFRIO__USR_VGContext.LastMsgToGui
          :
Level     :
Date      : 21/12/2010
Doc date  : 21/12/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO_USR__FOnGetRunStatus () {
  
  char* VPtLastMsgToGui       = EFRIO__USR_VGContext.LastMsgToGui;
  EFRIO__TRunCont* VPtRunCont = &EFRIO__VGContext.RunCont;
  
  // Build last GUI message to with information about run status
  
  snprintf ( VPtLastMsgToGui, GLB_CMT_SZ-1, "Acquisitions Cnt = %d - Frames cnt = %d - Events cnt = %d", VPtRunCont->ResAcqCnt, VPtRunCont->ResFrameCnt, VPtRunCont->ResEventCnt );
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO_USR__FOnGetRunStatusByRunCtrl ( )
:
Goal      : This function is called by the EUDET event method which request run status.
:
Inputs    : None
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   : Write to EFRIO__USR_VGContext.LastMsgToGui.
:
Remark    : Update the global variable GUI message -> EFRIO__USR_VGContext.LastMsgToGui
:
Level     :
Date      : 03/02/2011
          :
          : 01/03/2013
          : - Handle EFRIO__USR_VGContext.DisableRunCtrlMsgProcessing
          :
Doc date  : 03/02/2011
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


#ifndef EFRIO__INCLUDE_RUN_CTRL

SInt32 EFRIO_USR__FOnGetRunStatusByRunCtrl () {
  
  err_retfail ( -1, (ERR_OUT,"Function no available => EFRIO__INCLUDE_RUN_CTRL not define !") );
  
}

#else

SInt32 EFRIO_USR__FOnGetRunStatusByRunCtrl () {
  
   
  EFRIO__USR_TContext*  VPtUsrContext    = &EFRIO__USR_VGContext;  
  char*                 VPtLastMsgToGui  = EFRIO__USR_VGContext.LastMsgToGui;
  EFRIO__TRunCont*      VPtRunCont       = &EFRIO__VGContext.RunCont;
  
  
  
  
  // msg (( MSG_OUT, "$CALL => EFRIO_USR__FOnGetRunStatusByRunCtrl ()" )); // 01/03/2013
  // msg (( MSG_OUT, "$CALL => EFRIO_USR__FOnGetRunStatusByRunCtrl () - JTAG=%s", VPtRunCont->ParJtagFileName ));
  
  
  // -----------------------------------------------------------------------
  // Display command if display is enabled
  // DON'T Reject command if commands processing is disabled => Because HW is not needed to get run status ( AcqNb=0, FrNb=0, EvNb=0 ;-))
  // -----------------------------------------------------------------------
  
  if (VPtUsrContext->ParEnableRunCtrlMsgDisplay == 1 ) {
    sprintf ( VPtUsrContext->ResRunCtrlMsgStr, "Run Ctrl command => Get run status" );
    VPtUsrContext->ResRunCtrlMsgReady = 1;
  }
  
//  if (VPtUsrContext->ParDisableRunCtrlMsgProcessing == 1 ) {
//    err_retok (( ERR_OUT, "Exit => Run Ctrl cmd processing disabled !" ));
//  }
  
  
  // Build last GUI message to with information about run status
  
  snprintf ( VPtLastMsgToGui, GLB_CMT_SZ-1, "Acquisitions Cnt = %d - Frames cnt = %d - Events cnt = %d", VPtRunCont->ResAcqCnt, VPtRunCont->ResFrameCnt, VPtRunCont->ResEventCnt );
  
  // msg (( MSG_OUT, "%s", VPtLastMsgToGui ));


  if ( PulseEvent ( VPtUsrContext->InfEventRespondRunCtrlHnd ) != TRUE ) {
    err_retfail ( -1, (ERR_OUT,"PulseEvent respond run ctrl failed !  - LastError=%d", GetLastError () ) );
  }

  err_retok (( ERR_OUT, "" ));
}

#endif





/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO_USR__FOnStopRun ()
          :
Goal      : This function is called by the EUDET event method which stop run.
          :
Inputs    : None
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   : Write to EFRIO__USR_VGContext.LastMsgToGui.
          :
Remark    : Update the global variable GUI message -> EFRIO__USR_VGContext.LastMsgToGui
          :
Level     :
Date      : 21/12/2010
Doc date  : 21/12/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO_USR__FOnStopRun () {

  char* VPtLastMsgToGui = EFRIO__USR_VGContext.LastMsgToGui;


  // ----------------------------------------------
  // Stop saving on file ( if enabled in run par )
  // ----------------------------------------------
  
  EFRIO__FStopSavingOnFile ();
    
  // ----------------------------------------------------------------------------------------
  // Reset the flag CmdRun of record EFRIO__TRunCont which will be polled by DAQ application
  // ----------------------------------------------------------------------------------------
  
  EFRIO__FSetCmdRun (-1);

  // Set last GUI message to "OK"
  
  snprintf ( VPtLastMsgToGui, GLB_CMT_SZ-1, "OK - Run stopped :-)" );
  
  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO_USR__FOnStopRunByRunCtrl ( )
:
Goal      : This function is called by the EUDET event method which stop run.
:
Inputs    : None
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   : Write to EFRIO__USR_VGContext.LastMsgToGui.
:
Remark    : Update the global variable GUI message -> EFRIO__USR_VGContext.LastMsgToGui
:
Level     :
Date      : 03/02/2011
Rev       : 14/07/2012
          : - Reset Mimosa 26/28 for synchro between Telescope DAQ / DUT DAQ
          :   because the Mimosa frame cnt is used to tag events. Therefore it must
          :   stay to 0 until the DUT DAQ has resetted his Sync cnt => Reset Mimosa at end of run.
          :   It is done by setting TRSTB to 0 by a direct access to // port (No JTAG COM)
          :   The line TRSTB = D0 is set to 0 and keep in this state until JTAG SW will be
          :   launched by DAQ application.
          :
          : 01/03/2013
          : - Handle EFRIO__USR_VGContext.DisableRunCtrlMsgProcessing
          :
Doc date  : 03/02/2011
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

#ifndef EFRIO__INCLUDE_RUN_CTRL

SInt32 EFRIO_USR__FOnStopRunByRunCtrl () {
  
  err_retfail ( -1, (ERR_OUT,"Function no available => EFRIO__INCLUDE_RUN_CTRL not define !") );
  
}

#else

SInt32 EFRIO_USR__FOnStopRunByRunCtrl () {
  
  
  EFRIO__USR_TContext* VPtUsrCont      = &EFRIO__USR_VGContext; 
  char*                VPtLastMsgToGui = EFRIO__USR_VGContext.LastMsgToGui;
  
  // -----------------------------------------------------------------------
  // Debug message
  // -----------------------------------------------------------------------
  
  msg (( MSG_OUT, "$CALL => EFRIO_USR__FOnStopRunByRunCtrl ()" )); // 01/03/2013
  

  // -----------------------------------------------------------------------
  // Display command if display is enabled
  // Reject command if commands processing is disabled
  // -----------------------------------------------------------------------
  
  if ( VPtUsrCont->ParEnableRunCtrlMsgDisplay == 1 ) {
    sprintf ( VPtUsrCont->ResRunCtrlMsgStr, "Run Ctrl command => Stop run" );
    VPtUsrCont->ResRunCtrlMsgReady = 1;
  }
  
  if ( VPtUsrCont->ParDisableRunCtrlMsgProcessing == 1 ) {
    err_retok (( ERR_OUT, "Exit => Run Ctrl cmd processing disabled !" ));
  }
  

  // ----------------------------------------------
  // Stop saving on file ( if enabled in run par )
  // ----------------------------------------------
  
  EFRIO__FStopSavingOnFile ();
    
  // ----------------------------------------------------------------------------------------
  // Reset the flag CmdRun of record EFRIO__TRunCont which will be polled by DAQ application
  // ----------------------------------------------------------------------------------------
  
  EFRIO__FSetCmdRun (-1);
  
  // ----------------------------------------------
  // Reset Mimosa (26/28) => Rev 14/07/2012
  // ----------------------------------------------
  
  // Warning => Must wait until the stop run cmd has been seen by LabVIEW before performing reset
  // otherwise the run will not be terminated in a propoer way and LabVIEW will not go back to "main loop"
  
  Sleep ( 2000 ); // Now an arbitrary delay is used (value to be confirmed) - A better way should be found later
  
  msg (( MSG_OUT, "EFRIO_USR__FOnStopRunByRunCtrl () => Reset Mimosa (Set TRSTB = 0)" ));
  
  PPO_FOutD0 ( 0 /* Id */, 0 /* State */ );
  
  // Set last GUI message to "OK"
  
  snprintf ( VPtLastMsgToGui, GLB_CMT_SZ-1, "OK - Run stopped :-)" );
  
  err_retok (( ERR_OUT, "" ));
}

#endif

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO_USR__FOnAppExit ()
          :
Goal      : This function is called when application is stoped.
          : It will free memory and so on ...
          :
Inputs    : None
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   : Write to EFRIO__USR_VGContext.LastMsgToGui.
          :
Remark    : Update the global variable GUI message -> EFRIO__USR_VGContext.LastMsgToGui
          :
Level     :
Date      : 21/12/2010
Doc date  : 21/12/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO_USR__FOnAppExit () {

  char* VPtLastMsgToGui = EFRIO__USR_VGContext.LastMsgToGui;


  // Call lib user terminate function

  EFRIO_USR__FEnd ();

  // Call lib general terminate function
  
  EFRIO__FEnd ();
  

  // Set last GUI message to "OK"
  
  snprintf ( VPtLastMsgToGui, GLB_CMT_SZ-1, "OK - Application exit - Do nothing now :-)" );
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO_USR__FOnAppExitByRunCtrl ()
:
Goal      : This function is called when application is stoped.
: It will free memory and so on ...
:
Inputs    : None
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   : Write to EFRIO__USR_VGContext.LastMsgToGui.
:
Remark    : Update the global variable GUI message -> EFRIO__USR_VGContext.LastMsgToGui
:
Level     :
Date      : 03/02/2011
Rev       : 15/02/2011
          : - Add stop receiver port by a call to IAC__FSockStopReceiverPort
Doc date  : 03/02/2011
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

#ifndef EFRIO__INCLUDE_RUN_CTRL

SInt32 EFRIO_USR__FOnAppExitByRunCtrl () {
  
  err_retfail ( -1, (ERR_OUT,"Function no available => EFRIO__INCLUDE_RUN_CTRL not define !") );
  
}

#else

SInt32 EFRIO_USR__FOnAppExitByRunCtrl () {
  
  EFRIO__USR_TContext* VPtUsrContext = &EFRIO__USR_VGContext;
  char* VPtLastMsgToGui = EFRIO__USR_VGContext.LastMsgToGui;

  
  // Stop sender

  err_error (( ERR_OUT, "1" ));
  
  IAC__FSockStopSenderPort ( VPtUsrContext->InfRunCtrlSenderId );

  err_error (( ERR_OUT, "2" ));
  
  IAC__FSockUnInstallSenderPort ( VPtUsrContext->InfRunCtrlSenderId ); // 10/03/2011

  // Stop receiver

  err_error (( ERR_OUT, "3" ));
  
  IAC__FSockStopReceiverPort ( VPtUsrContext->InfRunCtrlReceiverId );

  err_error (( ERR_OUT, "4" ));
  
  IAC__FSockUnInstallReceiverPort ( VPtUsrContext->InfRunCtrlReceiverId ); // 10/03/2011

  // Call lib user terminate function

  err_error (( ERR_OUT, "5" ));
  
  EFRIO_USR__FEnd ();
  
  // Call lib general terminate function

  err_error (( ERR_OUT, "6" ));
  
  EFRIO__FEnd ();
  
  err_error (( ERR_OUT, "7" ));
  
  // Set last GUI message to "OK"
  
  snprintf ( VPtLastMsgToGui, GLB_CMT_SZ-1, "OK - Application exit - Do nothing now :-)" );
  
  err_retok (( ERR_OUT, "" ));
}

#endif


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO_USR__FGetAcqData ()
          :
Goal      : Function which send data of current acquisition to EUDET DAQ.
          :
Inputs    :
          :
Ouputs    :
          :
Globals   : Read EFRIO__VGContext fields.
          :
Remark    :
          :
Level     :
Date      : 22/12/2010
Doc date  : 22/12/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      :  IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO_USR__FGetAcqData () {
  
  EFRIO__TFrameList* VPtFrameList = EFRIO__VGContext.AAcqFrameList;
  EFRIO__TFrame*     VPtFrameBuff = EFRIO__VGContext.RunCont.PtFrame;
  SInt32             VTotAcqSz    = EFRIO__VGContext.RunCont.ResAcqFunctRetCode;


  // ------------------------------------------------------------------------------------------------
  // Via VPtFrameList you can access at current acquisition frame by frame via an array of pointers
  // ------------------------------------------------------------------------------------------------
  // The EFRIO__TFrameList type has the following fields
  //
  // SInt32 TotFrameNb;                                       // Total frames nb in list
  // EFRIO__TFrame* AFramePtr[EFRIO__MAX_FRAME_NB_PER_ACQ];   // Array of pointers on each frame
  // UInt32 AFrameSz[EFRIO__MAX_FRAME_NB_PER_ACQ];            // Array of frames size
  
  // ------------------------------------------------------------------------------------------------
  // Via VPtFrameBuff you can access at current acquisition seen as a single memory block, the size
  // of the bloc ( in bytes ) is given by VTotAcqSz which is a copy of the field ResAcqFunctRetCode
  // of EFRIO__TRunCont.
  // ------------------------------------------------------------------------------------------------
  
  return (0);  
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : char* EFRIO_USR_FGetLastMsgToGui ()
          :
Goal      :
          :
Inputs    : None
          :
Ouputs    : The function returns the last message (char*) for GUI
          :
Globals   : Read from EFRIO__USR_VGContext.LastMsgToGui.
          :
Remark    : 
          :
Level     :
Date      : 21/12/2010
Doc date  : 21/12/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

char* EFRIO_USR__FGetLastMsgToGui () {
  
  char* VPtLastMsgToGui = EFRIO__USR_VGContext.LastMsgToGui;
  
  return (VPtLastMsgToGui);
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
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
Date      : 01/03/2013
Doc date  : 01/03/2013
Author    :
E-mail    :
Labo      :  */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO_USR__FSetRunCtrlMsgHandlingOptions ( SInt8 DisableRunCtrlMsgProcessing, SInt8 EnableRunCtrlMsgDisplay ) {
  
  EFRIO__USR_TContext* VPtUsrCont = &EFRIO__USR_VGContext;
    
  VPtUsrCont->ParDisableRunCtrlMsgProcessing = DisableRunCtrlMsgProcessing;
  VPtUsrCont->ParEnableRunCtrlMsgDisplay     = EnableRunCtrlMsgDisplay;
  
}

  
/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
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
Date      : 01/03/2013
Doc date  : 01/03/2013
Author    :
E-mail    :
Labo      :  */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO_USR__FIsRunCtrlMsgReady () {
  
  EFRIO__USR_TContext* VPtUsrCont = &EFRIO__USR_VGContext;
   
  if ( VPtUsrCont->ResRunCtrlMsgReady == 1 ) {
    VPtUsrCont->ResRunCtrlMsgReady = 0;
    return (1);
  }
  
  return (0);  
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
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
Date      : 01/03/2013
Doc date  : 01/03/2013
Author    :
E-mail    :
Labo      :  */
/* =================================================================================== */
/* DOC_FUNC_END */

char* EFRIO_USR__FGetCtrlMsg () {
  
  EFRIO__USR_TContext* VPtUsrCont = &EFRIO__USR_VGContext;
  
  
  return (VPtUsrCont->ResRunCtrlMsgStr); 
}

  

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
          :
Goal      :
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
Date      : 11/11/2010
Doc date  : 11/11/2010
Author    :
E-mail    :
Labo      :  */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO_USR_FPriv () {
    
  err_retok (( ERR_OUT, "" ));
}



#endif
