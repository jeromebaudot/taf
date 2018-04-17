
/*******************************************************************************
File      : x:\lib\com\inter_app_com\inter_app_com.h
Goal      : Functions prototypes of inter applications communication library.
Prj date  : 05/12/2007
File date : //200
Doc date  : //200
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
----------------------------------------------------------------------------------
License   : You are free to use this source files for your own development as long
          : as it stays in a public research context. You are not allowed to use it
          : for commercial purpose. You must put this header with laboratory and
          : authors names in all development based on this library.
----------------------------------------------------------------------------------
Labo      : IPHC */
/*******************************************************************************/


#ifndef INTER_APP_COM_H

#include "func_header.def"


// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, char* DIU_FGetVersion ();)
// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, ;)


FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__FBegin ( SInt8 ErrLogLvl, char* ErrLogFilePath );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__FEnd ();)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__RMP_FSetFuncRunStatusAnswer  ( IAC__RMP_TFuncRunStatusAnswer PtFunc );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__RMP_FSetFuncRunStatusAnswer2 ( IAC__RMP_TFuncRunStatusAnswer2 PtFunc );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__RMP_FSetFuncAskEventAnswer   ( IAC__RMP_TFuncAskEventAnswer PtFunc );)


FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__RMP_FCmdAskRunStatus  ( IAC__RMP_TRunStatusAnswer* PtStatus );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__RMP_FCmdAskRunStatus2 ( IAC__RMP_TRunStatusAnswer2* PtStatus );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__RMP_FCmdAskEvent 				 ( IAC__RMP_TAskEventAnswer* PtAns );)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__RMP_FDaqCmdInterpreter ();)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__RMP_FSendEvent ( UInt8* PtEv, SInt32 Sz );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__RMP_FReceiveEvent ( UInt8* PtEv, SInt32 MaxEvSz, SInt32* PtEvSz );)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__RMP_FTestFillEvent ( UInt8* PtEv, SInt32 Sz );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__RMP_FTestCompareEvents ( UInt8* PtEvToCmp, UInt8* PtEvRef, SInt32 Sz );)

#ifndef ROOT_ROOT
	FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__RMP_FTestPrintMemStatusRec ( LPMEMORYSTATUS PtRec );)
	FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__RMP_FTestGetMemStatus ( SInt8 Print, LPMEMORYSTATUS PtRec, SInt32* PtStackAvailSz );)
#endif


FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__FSockInstallSenderPort            ( char* DestHost, UInt32 DestPort, SInt8 LongMsg, UInt32 AckTimeOutMs, UInt8 RetryNb, IAC__TFSockFuncCheckSum FuncCheckSum );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__FSockRebootSenderPort             ( SInt32 ContextId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__FSockUnInstallSenderPort          ( SInt32 SendPortContextId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__FSockStartSenderPort              ( SInt32 ContextId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__FSockStopSenderPort               ( SInt32 ContextId );)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__FSockSendShortMsg ( SInt32 ContextId, UInt8* PtMsg, UInt32 MsgSz, char* Caller );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__FSockSendLongMsg  ( SInt32 ContextId, UInt8* PtMsg, UInt32 MsgSz, char* Caller );)


FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__FSockInstallReceiverPort     ( UInt32 RecPort, SInt8 LongMsg, IAC__TFSockUsrHandleRecMsg FuncUsr, IAC__TFSockFuncCheckSum FuncCheckSum );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__FSockRebootReceiverPort      ( SInt32 ContextId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__FSockUnInstallReceiverPort   ( SInt32 RecPortContextId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__FSockStartReceiverPort       ( SInt32 ContextId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__FSockStopReceiverPort        ( SInt32 ContextId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 IAC__FSockDbgReceiverPortDisableReception ( SInt32 ContextId, SInt8 DisableReception );)

  

// FHEAD ( SInt32 REF_FHello ();)


FHEAD ( SInt32 IAC__PRIV_FSockInit ();)
FHEAD ( struct sockaddr IAC__PRIV_FTcpFormatAdress ( char * Host, u_short Port );)

FHEAD ( SInt32 IAC__PRIV_FSockGetSenderPortContext    ();)
FHEAD ( SInt32 IAC__PRIV_FSockFreeSenderPortContext   ( SInt32 ContextId );)
FHEAD ( SInt32 IAC__PRIV_FChkSenderContextId          ( SInt32 ContextId );)


FHEAD ( SInt32 IAC__PRIV_FSockGetReceiverPortContext ();)
FHEAD ( SInt32 IAC__PRIV_FSockFreeReceiverPortContext ( SInt32 ContextId ) ;)
FHEAD ( SInt32 IAC__PRIV_FChkReceiverContextId ( SInt32 ContextId );)
FHEAD ( SInt32 IAC__PRIV_FSockLookForShortRecMsgOnPortAndProcessIt ( UInt32 RecPortContextId );)
FHEAD ( SInt32 IAC__PRIV_FSockLookForLongRecMsgOnPortAndProcessIt  ( UInt32 RecPortContextId );)

#ifdef ROOT_ROOT
  FHEAD ( void* IAC__PRIV_FSockReceiverThreadFunc ( void* lpParam );)
#else
  FHEAD ( DWORD WINAPI IAC__PRIV_FSockReceiverThreadFunc ( LPVOID lpParam );)
#endif


  FHEAD ( DWORD WINAPI IAC__PRIV_FSockSenderThreadFunc ( LPVOID lpParam );)
  

#ifndef APP_DLL_IMPORT_EXPORT
  #ifndef INTER_APP_COM_H
    #define INTER_APP_COM_H
	#endif
#endif


#endif