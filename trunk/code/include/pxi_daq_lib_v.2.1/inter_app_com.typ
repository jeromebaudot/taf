
/*******************************************************************************
File      : x:\lib\com\inter_app_com\inter_app_com.typ
Goal      : Types definition of inter applications communication library.
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


#ifndef INTER_APP_COM_TYP
#define INTER_APP_COM_TYP



/* ============== */
/*  */
/* ============== */

// -------------------------------------------------------
// Commands RMP
// -------------------------------------------------------

// Ask run status

typedef struct {
  
  SInt32 ParRunNo;
  SInt32 ParEventNb;
  char   ParRunDir[GLB_FILE_PATH_SZ];
  
  SInt8		ResRunning;
  SInt32 ResCurEvNb;

}

#ifdef ROOT_SCRIPT
  RS__IAC__RMP_TRunStatusAnswer;
#else
  IAC__RMP_TRunStatusAnswer;
#endif


typedef SInt32 (*IAC__RMP_TFuncRunStatusAnswer ) ( COM_TCommand* PtCmd, IAC__RMP_TRunStatusAnswer* PtAns );


// Ask run status 2

typedef struct {
  
  SInt32 ParRunNo;
  SInt32 ParEventNb;
  char   ParRunDir[GLB_FILE_PATH_SZ];
  
  SInt8		ResRunning;
  SInt32 ResCurEvNb;
  
  char   ResPadding[157];
  
} IAC__RMP_TRunStatusAnswer2;  


typedef SInt32 (*IAC__RMP_TFuncRunStatusAnswer2 ) ( COM_TCommand* PtCmd, IAC__RMP_TRunStatusAnswer2* PtAns );



// Ask event


typedef struct {
  
  SInt32 EvNb;
  SInt32 TestTag;
  
} IAC__RMP_TAskEventPar;

typedef struct {
  
  SInt32 CmdEvNbRequested;
  
  SInt32	ResEvNbAvailable;
  SInt32 AResEvSz				[IAC__RMP_MAX_EV_NB_REQUEST];
  SInt32 AResEvOffset[IAC__RMP_MAX_EV_NB_REQUEST];
    
}

#ifdef ROOT_SCRIPT
  RS__IAC__RMP_TAskEventAnswer;
#else
  IAC__RMP_TAskEventAnswer;
#endif

typedef SInt32 (*IAC__RMP_TFuncAskEventAnswer ) ( COM_TCommand* PtCmd, IAC__RMP_TAskEventAnswer* PtAns );


// -------------------------------------------------------
// Socket handling
// -------------------------------------------------------


#ifdef ROOT_ROOT

  typedef void* (*IAC__TFSockReceiverThread) ( void* lpParam );
  
#else

  typedef DWORD WINAPI (*IAC__TFSockReceiverThread) ( LPVOID lpParam );
  typedef DWORD WINAPI (*IAC__TFSockSenderThread) ( LPVOID lpParam );

#endif


// typedef DWORD WINAPI (*IAC__TFSockSenderThread) ( LPVOID lpParam ); // moved on 26/01/09

typedef SInt32 (*IAC__TFSockUsrHandleRecMsg) (UInt8* PtMsg, UInt32 MsgSz, UInt8* PtUsrData, UInt32 UsrDataSz );

typedef SInt32 (*IAC__TFSockFuncCheckSum)   ( UInt8* PtData, UInt32 DataSz );

typedef struct {
  
  UInt32 StartTag;
  UInt8  Zero;
  UInt32 TotMsgSz;
  UInt32 CheckSum;
  
} IAC__TSockMsgHeader;


typedef struct {
  
  UInt32 StopTag;
  UInt8  Zero;
  
} IAC__TSockMsgTrailer;



#ifndef ROOT_ROOT

typedef struct {
  
  UInt8   Id;
  UInt8   Used;
  UInt8   Installed;
  UInt8   Started;
  
  UInt16  CstHeaderSz;
  UInt16  CstTrailerSz;
  
  IAC__TSockMsgHeader  MsgHeader;
  IAC__TSockMsgTrailer MsgTrailer;
  
  char    ParDestHost[IAC__SOCK_MAX_STR_HOST_SZ];
  UInt32  ParDestPort;
  UInt8   ParLongMsg;
  UInt32  ParAckTimeOutMs;
  UInt8   ParRetryNb;
  UInt8*  ParPtMsg;
  UInt32  ParMsgSz;
  IAC__TFSockFuncCheckSum ParFuncCheckSum;
  
  UInt8*  PtBuffSendShortMsg;
  UInt8   BuffRecAck[IAC__SOCK_ACK_SZ];
  
  
  IAC__TFSockSenderThread ThreadFunc;
  
  SOCKET             Socket;
  struct sockaddr_in LocAddr;
  struct sockaddr    DestAddr;
  
  
  
  HANDLE                 ThreadHnd;
  DWORD                  ThreadId;
  HANDLE                 EvNewMsgHnd; // New message to send        
  HANDLE                 EvAckMsgHnd; // Acknowledge received from message receiver
  
  SInt32 SendStatus;
  
} IAC__TSockSenderPort;


typedef struct {
  
  UInt8  Id;
  UInt8  Used;
  UInt8  Installed;
  UInt8  Started;
  
  UInt8  MsgReady;
  
  UInt16  CstHeaderSz;
  UInt16  CstTrailerSz;
  
  IAC__TSockMsgHeader  MsgHeader;
  IAC__TSockMsgTrailer MsgTrailer;
  
  
  UInt32 ParPort;
  SInt8  ParLongMsg; // 0 short message - 1 long message
  IAC__TFSockUsrHandleRecMsg ParUsrFunc;
  IAC__TFSockFuncCheckSum ParFuncCheckSum;
  
  SOCKET  LSocket;  // Listen socket
  SOCKET  ASocket;  // Accept socket
  struct sockaddr_in LocAddr;

  
  IAC__TFSockReceiverThread  ThreadFunc;
  HANDLE                     ThreadHnd;
  DWORD                      ThreadId;
  
  UInt8* PtMsg;
  UInt32 MaxMsgSz;
  UInt32 MsgSz;
  

  SInt32 DbgDisableReception;

  // Fields to removr later 

  UInt8  Run; // ???
  SOCKET  Socket; // ???
  SOCKET  Accept; // ???
  
} IAC__TSockReceiverPort;


typedef struct {

  SInt8                InitDone;
  
  SInt32               SenderPortNb;
  IAC__TSockSenderPort ASenderPort[IAC__SOCK_MAX_SENDER_PORT_NB];
  
  SInt32                 ReceiverPortNb;
  IAC__TSockReceiverPort AReceiverPort[IAC__SOCK_MAX_REC_PORT_NB];

} IAC__TSockContext;

#endif

// -------------------------------------------------------
// IAC__TWho => Record which identifies sender / receiver
// -------------------------------------------------------

typedef struct {
  
  char   PcName[IAC__TWHO_MAX_NAME_SZ];
  char   ApplicationName[IAC__TWHO_MAX_NAME_SZ];
  SInt16 ApplicationInstance;
  char   ApplicationCommonDir[GLB_FILE_PATH_SZ];
  
} IAC__TWho;


// -------------------------------------------------------
// IAC_TMsgHeader => Message header
// -------------------------------------------------------

typedef struct {

  char   MsgIdStr[IAC__MSG_ID_STR_SZ];
  SInt32 MsgTotSz;
  SInt32 MsgDataSz;
  
  SInt8  Priority;
  SInt8  Type;
  SInt16 SubType;
  
  IAC__TWho From;
  IAC__TWho To;
  
} IAC_TMsgHeader;

// -------------------------------------------------------
// IAC__TMsg => Message
// -------------------------------------------------------

typedef struct {

  IAC_TMsgHeader Header;
  UInt8          Data;
    
} IAC__TMsg;


// -------------------------------------------------------
// IAC__TRmpContext => RMP
// -------------------------------------------------------

typedef struct {

  SInt8 CmdComPcObjId;
  SInt8 CmdComUcObjId;

  UInt16 CmdGetCmdResLoopPeriod;
  SInt32 CmdGetCmdResTryNb;

  SInt8 DataComPcObjId;
  SInt8 DataComUcObjId;
  
  IAC__RMP_TFuncRunStatusAnswer  FuncRunStatusAnswer;
  IAC__RMP_TFuncRunStatusAnswer2 FuncRunStatusAnswer2;
  IAC__RMP_TFuncAskEventAnswer 	 FuncAskEventAnswer;
  
} IAC__TRmpContext;

// -------------------------------------------------------
// IAC__TContext => Lib context
// -------------------------------------------------------

typedef struct {
  
  SInt8 InitDone;
  
  IAC__TRmpContext  Rmp;
  
  #ifndef ROOT_ROOT
    IAC__TSockContext Sock;
  #endif
  
} IAC__TContext;


#endif