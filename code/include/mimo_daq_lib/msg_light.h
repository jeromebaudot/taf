
/**
* ----------------------------------------------------------------------------------
* \file              X:\prj\win\mimosis_1\run_read_light_bt\com\msg_light.h
* \brief Goal      : General messages library - Constants, types, functions proto, ...
* \brief
* \brief
* \version         : 1.0
* \date Prj date   : 12/05/2021
* \date File date  : 12/05/2021
* \date Doc date   : 12/05/2021
* \author          : Gilles CLAUS
* \author          : gilles.claus@iphc.cnrs.fr
* \author          : CNRS - IN2P3 - IPHC 23 Rue du Loess 67037 STRASBOURG
*
* Remark : None
*
* ----------------------------------------------------------------------------------
* License   : GNU General Public License
*
* ----------------------------------------------------------------------------------
*/


#ifndef MSG_L_H
#define MSG_L_H


#include <stdio.h>
#include <stdlib.h>


// #include "..\..\com\var_decl_def.h" // For selection between variable creation / export via  CC
                                    // #undef CC_INTERFACE_FILE  => Creates variables and initializes them
                                    // #define CC_INTERFACE_FILE => Export variables only


#include "var_decl_def.h"


#ifdef APP_ERR_MSG_MACROS_NOT_SUPPORTED


// -----------------------------------------------------------------------
// Constants for genral messages
// -----------------------------------------------------------------------

#define MSG_CMT_SZ 10000 // 1024 until 23/05/2019

#define MSG_CHAN_NB   2
#define MSG_CHAN_MSG  0
#define MSG_CHAN_RMSG 1


// -----------------------------------------------------------------------
// General messages "conversion" lowercase  to uppercase
// -----------------------------------------------------------------------


#define MSG_PRINTF(Msg) { sprintf Msg;}

#define msg(Msg) MSG_PRINTF(Msg)

#define MSG_PRINTF_LVL(Msg,Lvl) { sprintf Msg; }

#define msgl(Msg,Lvl) MSG_PRINTF_LVL(Msg,Lvl)


// -----------------------------------------------------------------------
// Types definition
// -----------------------------------------------------------------------


typedef SInt32 (*TUserMsgFunc) ( char* Msg );


// -----------------------------------------------------------------------
// Variables for errors messages
// -----------------------------------------------------------------------


// VAR_DCL_INIT    ( EXTERN, VAR_STATIC, char*        MSG_OUT                                      , MSG_VGAStrMsg[MSG_CHAN_MSG] );

  VAR_DCL_INIT ( EXTERN, VAR_STATIC, char MSG_OUT[GLB_CMT_SZ] , "");

  VAR_DCL_INIT ( EXTERN, VAR_STATIC, char MSG_VGMsgNotAvailable[GLB_CMT_SZ] , "General messages loging not fully available");

#else

// -----------------------------------------------------------------------
// Compilation directives
// -----------------------------------------------------------------------



// -----------------------------------------------------------------------
// Macro definition
// -----------------------------------------------------------------------





// -----------------------------------------------------------------------
// Macro for genral messages
// -----------------------------------------------------------------------

#define MSG_CMT_SZ 10000 // 1024 until 23/05/2019

#define MSG_CHAN_NB   2
#define MSG_CHAN_MSG  0
#define MSG_CHAN_RMSG 1


/* Must be after variables definition, instead CINT will be unable to handle these macros */

#define MSG_VGLogFile     MSG_VGALogFile[MSG_CHAN_MSG]
#define MSG_VGLogClosed   MSG_VGALogClosed[MSG_CHAN_MSG]
#define MSG_VGDontLog     MSG_VGADontLog[MSG_CHAN_MSG]
#define MSG_VGMsgCnt      MSG_VGAMsgCnt[MSG_CHAN_MSG]
#define MSG_VGLogPath     MSG_VGALogPath[MSG_CHAN_MSG]

#define RMSG_VGLogFile    MSG_VGALogFile[MSG_CHAN_RMSG]
#define RMSG_VGLogClosed  MSG_VGALogClosed[MSG_CHAN_RMSG]
#define RMSG_VGMsgCnt     MSG_VGAMsgCnt[MSG_CHAN_RMSG]
#define RMSG_VGLogPath    MSG_VGALogPath[MSG_CHAN_RMSG]

/* 07/04/2007 MSG_VGAStrMsg identifier replaced by MSG_OUT because of ROOT CINT macros limitations */
/* #define MSG_OUT         MSG_VGAStrMsg[MSG_CHAN_MSG] */

#define RMSG_OUT        MSG_VGAStrMsg[MSG_CHAN_RMSG]

#define MSG_PRINTF(Msg) { sprintf Msg; MSG_FGenMsg (MSG_CHAN_MSG,127);}
#define msg(Msg) MSG_PRINTF(Msg)

#define MSG_PRINTF_LVL(Msg,Lvl) { sprintf Msg; MSG_FGenMsg (MSG_CHAN_MSG,Lvl); }
#define msgl(Msg,Lvl) MSG_PRINTF_LVL(Msg,Lvl)


#define RMSG_PRINTF(Msg) { sprintf Msg; MSG_FGenMsg (MSG_CHAN_RMSG,127); }
#define msgr(Msg) RMSG_PRINTF(Msg)
#define rmsg(Msg) RMSG_PRINTF(Msg)





// -----------------------------------------------------------------------
// Types definition
// -----------------------------------------------------------------------

typedef SInt32 (*TUserMsgFunc) ( char* Msg );

// -----------------------------------------------------------------------
// Variables definition
// -----------------------------------------------------------------------



VAR_DCL_INIT_A2 ( EXTERN, VAR_STATIC, FILE*        MSG_VGALogFile[MSG_CHAN_NB]                  , NULL, NULL );
VAR_DCL_INIT_A2 ( EXTERN, VAR_STATIC, SInt8        MSG_VGALogClosed[MSG_CHAN_NB]                , 1, 1);
VAR_DCL_INIT_A2 ( EXTERN, VAR_STATIC, SInt8        MSG_VGALogEnabled[MSG_CHAN_NB]               , 0, 0);
VAR_DCL_INIT_A2 ( EXTERN, VAR_STATIC, SInt8        MSG_VGADontLog[MSG_CHAN_NB]                  , 0, 0);
VAR_DCL_INIT_A2 ( EXTERN, VAR_STATIC, SInt32       MSG_VGAMsgCnt[MSG_CHAN_NB]                   , 0, 0);


VAR_DCL_INIT_A2 ( EXTERN, VAR_STATIC, char         MSG_VGALogPath[MSG_CHAN_NB][GLB_FILE_PATH_SZ], "./default_msg.txt", "./default_rmsg" ); // Since 30/11/2019

VAR_DCL         ( EXTERN, VAR_STATIC, char         MSG_VGAStrMsg[MSG_CHAN_NB][MSG_CMT_SZ] );

VAR_DCL         ( EXTERN, VAR_STATIC, char         MSG_VGAStrMsgLong[MSG_CHAN_NB][MSG_CMT_SZ] ); // GC - 20/02/2020, full message with line No for ERR_FGetLastErrMsg (...)

/* 07/04/2007 Replace macro MSG_OUT by a variable which points to MSG_VGAStrMsg[MSG_CHAN_MSG] because of ROOT CINT macros limitations */


VAR_DCL_INIT    ( EXTERN, VAR_STATIC, char*        MSG_OUT                                      , MSG_VGAStrMsg[MSG_CHAN_MSG] );
VAR_DCL_INIT    ( EXTERN, VAR_STATIC, SInt8        MSG_VGFileLogLevel                           , 0);
VAR_DCL_INIT    ( EXTERN, VAR_STATIC, SInt8        MSG_VGUserLogLevel                           , 0);
VAR_DCL_INIT    ( EXTERN, VAR_STATIC, TUserMsgFunc MSG_VGUserMsgFunc                            , NULL);


// Flags to indicate that there is a message available
// mainly for interface / LabVIEW => Testing errors messages presence by polling
//
// 08/04/2021


VAR_DCL_INIT_A2 ( EXTERN, VAR_STATIC, SInt8  MSG_VGIsMsgAndLevel[MSG_CHAN_NB], 0, 0); // 0 => No msg, > 0 one msg and value indicates msg level (1 to 127)


#endif // End of #ifdef APP_ERR_MSG_MACROS_NOT_SUPPORTED


#endif
