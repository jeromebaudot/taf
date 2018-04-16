
/*******************************************************************************
File      : x:\lib\win\eudet_frio\eudet_frio_usr.typ
Goal      : Types definition of USER PART of flex rio board library for EUDET Mimosa 26 DAQ
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


#ifndef EUDET_FRIO_USR_TYP
#define EUDET_FRIO_USR_TYP



// 03/02/2011

typedef struct {
  
  SInt32 TotMsgSz;
  SInt32 HeaderSz;
  SInt8  MsgCode;
  char   StrMsgCode[EFRIO__USR_RUN_CTRL_MSG_CODE_STR_MAX_SZ];
  
  UInt8  Data;
  
} EFRIO__USR_TRunCtrlMsgHeader;



// 16/02/2011

typedef struct {

  EFRIO__TRunCont RunCont;
  
} EFRIO__USR_TRunCtrlRunStatusAns;


/* ============================================== */
/* Lib context record                             */
/* ---------------------------------------------- */
/* This record contains all lib global variables  */
/* ---------------------------------------------- */
/* Date      : dd/mm/2010                         */
/* Doc date  : dd/mm/2010                         */
/* Author    :                                    */
/* E-mail    :                                    */
/* Labo      :                                    */
/* ============================================== */


typedef struct {
  
  SInt8 InfInitDone; // Lib iit done or not
  
  char  LastMsgToGui[GLB_CMT_SZ]; // Last message to GUI  

// Set in comment #ifdef EFRIO__INCLUDE_RUN_CTRL on 13/06/2013
// Because otherwise the data plot  application doesn't compile
// Dont' remember why this CC had been added ? 

// #ifdef EFRIO__INCLUDE_RUN_CTRL

  char   ParRunCtrlIPAddr[GLB_FILE_PATH_SZ];
  SInt32 ParRunCtrlPort;
  SInt32 ParDaqPort;
  SInt8  ParCrIndexFile; // Create index file for synchro with DUT DAQ - 26/03/2013
  SInt32 ParReadoutMode; // Reserved for future use (data demux hard coded / free nb of Mimosa) - 26/03/2013
  
                                          // 01/03/2013
  SInt8  ParDisableRunCtrlMsgProcessing;  // Disable RC requests processing display only request in GUI if ParEnableRunCtrlMsgDisplay = 1
                                          // used to test a run ctrl application
  SInt8  ParEnableRunCtrlMsgDisplay;      // Enable RC requests display in GUI 
  
  SInt32 InfRunCtrlSenderId;
  SInt32 InfRunCtrlReceiverId;  
  DWORD  InfThreadRespondRunCtrlId;
  HANDLE InfThreadRespondRunCtrlHnd;
  HANDLE InfEventRespondRunCtrlHnd;
  SInt32 InfFOnConfByRunCtrlRetCode;
  SInt32 InfFOnStartRunByRunCtrlRetCode;
  SInt32 InfFOnGetRunStatusByRunCtrl;
  SInt32 InfFOnStopRunByRunCtrl;

  SInt32 ResRunCtrlMsgReady;              // Indicates that a Run Ctrl message has been received and can be read from ResRunCtrlMsgStr
  char   ResRunCtrlMsgStr[GLB_CMT_SZ];    // Last Run Ctrl message
  
  
// #endif


} EFRIO__USR_TContext;


#endif
