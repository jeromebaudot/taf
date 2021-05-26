
/**
* ----------------------------------------------------------------------------------
* \file              X:\prj\win\mimosis_1\run_read_light_bt\com\errors_light.h
* \brief Goal      : Errors messages library - Constants, types, functions proto, ...
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


#ifndef ERR_L_H
#define ERR_L_H


#include <stdio.h>
#include <stdlib.h>
#include <errno.h> // GC 19/05/2021 :

#include "var_decl_def.h"


#ifdef APP_ERR_MSG_MACROS_NOT_SUPPORTED

// -----------------------------------------------------------------------
// Constants for errors messages
// -----------------------------------------------------------------------


#define ERR_LOG_LVL_NONE             0
#define ERR_LOG_LVL_ALL              1
#define ERR_LOG_LVL_WARINGS_ERRORS   2
#define ERR_LOG_LVL_WARNINGS_ERRORS  2
#define ERR_LOG_LVL_ERRORS           3


// -----------------------------------------------------------------------
// Errors messages "conversion" lowercase  to uppercase
// -----------------------------------------------------------------------

#define err_trace(Msg)                  ERR_TRACE(Msg)
#define err_tracel(Lvl,Msg)             ERR_TRACE(Msg)
#define err_warning(Msg)                ERR_WARNING(Msg)
#define err_error(Msg)                  ERR_ERROR(Msg)

#define err_ret(Code,MsgOk,MsgFail)     ERR_RET(Code,MsgOk,MsgFail)
#define err_retok(MsgOk)                ERR_RET_OK(MsgOk)
#define err_retval(Code,MsgOk)          ERR_RET_VAL(Code,MsgOk)
#define err_retfail(Code,MsgFail)       ERR_RET_FAIL(Code,MsgFail)
#define err_retfailnull(Code,MsgFail)   ERR_RET_FAIL_NULL(Code,MsgFail)
#define err_retnull(Ptr,MsgFail)        ERR_RET_NULL(Ptr,MsgFail)


// -----------------------------------------------------------------------
// Macro for errors messages
// -----------------------------------------------------------------------

// Because __FUNCTION__ does not exists under C++ Builder (Windows)

#ifdef CC_APP_GUI_CPP
  #define __FUNCTION__ __FUNC__
#endif


/*

#define ERR_TRACE(Msg) { sprintf ( ERR_OUT, "T : file %s function ? line %.4d ", __FILE__, __LINE__ ) ; }

#define ERR_WARNING(Msg) { sprintf ( ERR_OUT, "W : file %s function ? line %.4d ", __FILE__, __LINE__ ) ; }

#define ERR_ERROR(Msg) { sprintf ( ERR_OUT, "E : file %s function ? line %.4d ", __FILE__, __LINE__ ) ; }

*/


#define ERR_TRACE(Msg) { sprintf ( ERR_OUT, "T : file %s function %s line %.4d ", __FILE__, __FUNCTION__, __LINE__ ) ; }

#define ERR_WARNING(Msg) { sprintf ( ERR_OUT, "W : file %s function %s line %.4d ", __FILE__, __FUNCTION__, __LINE__ ) ; }

#define ERR_ERROR(Msg) { sprintf ( ERR_OUT, "E : file %s function %s line %.4d ", __FILE__, __FUNCTION__, __LINE__ ) ; }



#define ERR_RET(Code,MsgOk,MsgFail) { \
 if ((Code)<0) ERR_ERROR (MsgFail) \
    else ERR_TRACE (MsgOk) \
    return (Code); \
  }


#define ERR_RET_FAIL(Code,MsgFail) { \
 if ((SInt32)(Code)<0) { ERR_ERROR (MsgFail) return (Code); } \
  }



#define ERR_RET_FAIL_NULL(Code,MsgFail) { \
if ((SInt32)(Code)<0) { ERR_ERROR (MsgFail) return (NULL); } \
  }



#define ERR_RET_NULL(Ptr,MsgFail) { \
if ((void*)(Ptr)==NULL) { ERR_ERROR (MsgFail) return (-1); } \
  }




#define ERR_RET_OK(MsgOk) { \
  if ( ERR_VGFastErrRetOkNoLog == 0 ) ERR_TRACE (MsgOk); \
    return (0); \
  }


#define ERR_RET_VAL(Code,MsgOk) { \
ERR_TRACE (MsgOk) \
    return (Code); \
  }



// -----------------------------------------------------------------------
// Types definition
// -----------------------------------------------------------------------


typedef SInt32 (*ERR_TUserErrorFunc) ( char Type, char* ErrLocation, char* ErrUserMsg, char* FullMsg );
  

// -----------------------------------------------------------------------
// Variables for errors messages
// -----------------------------------------------------------------------


  // VAR_DCL      ( EXTERN, VAR_STATIC, char ERR_OUT[GLB_CMT_SZ]     );

  VAR_DCL_INIT ( EXTERN, VAR_STATIC, char ERR_OUT[GLB_CMT_SZ] , "");

  VAR_DCL_INIT ( EXTERN, VAR_STATIC, SInt8 ERR_VGFastErrRetOkNoLog, 0);

  VAR_DCL_INIT ( EXTERN, VAR_STATIC, char ERR_VGMsgNotAvailable[GLB_CMT_SZ] , "Errors loging not fully available");





#else





// #include "..\..\com\var_decl_def.h" // For selection between variable creation / export via  CC
                                    // #undef CC_INTERFACE_FILE  => Creates variables and initializes them
                                    // #define CC_INTERFACE_FILE => Export variables only




// -----------------------------------------------------------------------
// Compilation directives
// -----------------------------------------------------------------------



// -----------------------------------------------------------------------
// Macro definition
// -----------------------------------------------------------------------


// Because __FUNCTION__ does not exists under C++ Builder (Windows)

#ifdef CC_APP_GUI_CPP
  #define __FUNCTION__ __FUNC__
#endif



// -----------------------------------------------------------------------
// Macro for errors messages
// -----------------------------------------------------------------------


#define ERR_LOG_LVL_NONE             0
#define ERR_LOG_LVL_ALL              1
#define ERR_LOG_LVL_WARINGS_ERRORS   2
#define ERR_LOG_LVL_WARNINGS_ERRORS  2
#define ERR_LOG_LVL_ERRORS           3




#define ERR_USR_MSG_SZ 256
#define ERR_SYS_MSG_SZ 256
#define ERR_TOT_MSG_SZ (ERR_USR_MSG_SZ + ERR_SYS_MSG_SZ)


/* 07/04/2007 ERR_VGStrUserMsg identifier replaced by ERR_OUT because of ROOT CINT macros limitations */
/* #define ERR_OUT ERR_VGStrUserMsg */


/* 07/04/2007 - Use a local variable in each function because __FUNCTION__ IS NOT handled by CNT ... */
/* __LINE__ is replaced by 0 because __LINE__ because ... __LINE__ is not handled file by file ...  */

#ifdef APP_ROOT
  #define ERR_LOCATION(MsgType)  {sprintf ( ERR_VGStrLocationMsg, "%.4d #%c - %-35s - %-25s - %.4d = ", ERR_VGMsgCnt++, MsgType, __FILE__,VFuncName, 0 /* __LINE__ */ );}
#else
  #define ERR_LOCATION(MsgType)  {sprintf ( ERR_VGStrLocationMsg, "%.4d #%c - %-35s - %-25s - %.4d = ", ERR_VGMsgCnt++, MsgType, __FILE__,__FUNCTION__,__LINE__ );}
#endif



#define ERR_WARNING(Msg) { sprintf Msg; ERR_LOCATION ('W'); ERR_FGenError ('W'); }

#define ERR_ERROR(Msg) { sprintf Msg; ERR_LOCATION ('E'); ERR_FGenError ('E'); }

#define ERR_TRACE(Msg) { sprintf Msg; ERR_LOCATION ('T'); ERR_FGenError ('T'); }


#define ERR_RET(Code,MsgOk,MsgFail) { \
 if ((Code)<0) ERR_ERROR (MsgFail) \
    else ERR_TRACE (MsgOk) \
    return (Code); \
  }


#define ERR_RET_FAIL(Code,MsgFail) { \
 if ((SInt32)(Code)<0) { ERR_ERROR (MsgFail) return (Code); } \
  }



#define ERR_RET_FAIL_NULL(Code,MsgFail) { \
if ((SInt32)(Code)<0) { ERR_ERROR (MsgFail) return (NULL); } \
  }



#define ERR_RET_NULL(Ptr,MsgFail) { \
if ((void*)(Ptr)==NULL) { ERR_ERROR (MsgFail) return (-1); } \
  }




#define ERR_RET_OK(MsgOk) { \
  if ( ERR_VGFastErrRetOkNoLog == 0 ) ERR_TRACE (MsgOk); \
    return (0); \
  }



#define ERR_RET_VAL(Code,MsgOk) { \
ERR_TRACE (MsgOk) \
    return (Code); \
  }



#define err_trace(Msg)                  ERR_TRACE(Msg)
#define err_tracel(Lvl,Msg)             ERR_TRACE(Msg)
#define err_warning(Msg)                ERR_WARNING(Msg)
#define err_error(Msg)                  ERR_ERROR(Msg)

#define err_ret(Code,MsgOk,MsgFail)     ERR_RET(Code,MsgOk,MsgFail)
#define err_retok(MsgOk)                ERR_RET_OK(MsgOk)
#define err_retval(Code,MsgOk)          ERR_RET_VAL(Code,MsgOk)
#define err_retfail(Code,MsgFail)       ERR_RET_FAIL(Code,MsgFail)
#define err_retfailnull(Code,MsgFail)   ERR_RET_FAIL_NULL(Code,MsgFail)
#define err_retnull(Ptr,MsgFail)        ERR_RET_NULL(Ptr,MsgFail)




// -----------------------------------------------------------------------
// Types definition
// -----------------------------------------------------------------------


typedef SInt32 (*ERR_TUserErrorFunc) ( char Type, char* ErrLocation, char* ErrUserMsg, char* FullMsg );

// -----------------------------------------------------------------------
// Variables definition
// -----------------------------------------------------------------------


/* 07/04/2007 - New macros VAR_DCL and VAR_DCL_INIT for variable declaration to solve a ROOT CINT limitation on macros               */
/* CINT doesn't handle macros like #define EMPTY_MACRO ... EMPTY_MACRO IS NOT replaced by empty space BUT LET as si = not replaced   */

VAR_DCL_INIT ( EXTERN, VAR_STATIC, SInt8              ERR_VGLogClosed                      , 1);

VAR_DCL_INIT ( EXTERN, VAR_STATIC, SInt8              ERR_VGFileDontLog                    , 0);
VAR_DCL_INIT ( EXTERN, VAR_STATIC, SInt8              ERR_VGUserDontLog                    , 0);
VAR_DCL_INIT ( EXTERN, VAR_STATIC, SInt8              ERR_VGFileLogEnable                  , 1);
VAR_DCL_INIT ( EXTERN, VAR_STATIC, SInt8              ERR_VGUserLogEnable                  , 0);
VAR_DCL_INIT ( EXTERN, VAR_STATIC, SInt8              ERR_VGFileLogLevel                   , ERR_LOG_LVL_ALL);
VAR_DCL_INIT ( EXTERN, VAR_STATIC, SInt8              ERR_VGUserLogLevel                   , ERR_LOG_LVL_ALL);


VAR_DCL_INIT ( EXTERN, VAR_STATIC, SInt8              ERR_VGConstructorNoLog               , 0); // 03/11/2020, used for error AND msg lib


// VAR_DCL_INIT ( EXTERN, VAR_STATIC, char               ERR_VGLogPath[GLB_FILE_PATH_SZ]      , "c:\\tmp\\errors.txt"); // Before 30/11/2019

VAR_DCL_INIT ( EXTERN, VAR_STATIC, char               ERR_VGLogPath[GLB_FILE_PATH_SZ]      , "./default_err.txt"); // Since 30/11/2019

VAR_DCL_INIT ( EXTERN, VAR_STATIC, FILE               *ERR_VGLogFile                       , NULL);
VAR_DCL_INIT ( EXTERN, VAR_STATIC, FILE               *ERR_VGTmpLogFile                    , NULL);
VAR_DCL_INIT ( EXTERN, VAR_STATIC, SInt32             ERR_VGMsgCnt                         , 0);
VAR_DCL_INIT ( EXTERN, VAR_STATIC, ERR_TUserErrorFunc ERR_VGUserErrorFunc                  , NULL);
VAR_DCL      ( EXTERN, VAR_STATIC, char               ERR_VGStrLocationMsg[ERR_TOT_MSG_SZ] );

/* 07/04/2007 - ERR_VGStrUserMsg identifier replaced by ERR_OUT because of ROOT CINT macros limitations */

VAR_DCL      ( EXTERN, VAR_STATIC, char               ERR_OUT[ERR_TOT_MSG_SZ]     );

VAR_DCL      ( EXTERN, VAR_STATIC, char   VGOut[255] );

// 21/12/2010

VAR_DCL_INIT ( EXTERN, VAR_STATIC, char *ERR_VGLastErrMsg, "NULL");

// 26/02/2020

VAR_DCL_INIT ( EXTERN, VAR_STATIC, SInt8 ERR_VGFastErrRetOkNoLog, 0);


// Flags to indicate that there is a trace, warnning, error message available
// mainly for interface / LabVIEW => Testing errors messages presence by polling
//
// 06/04/2021


VAR_DCL_INIT ( EXTERN, VAR_STATIC, SInt8 ERR_VGIsTraceMsg, 0);
VAR_DCL_INIT ( EXTERN, VAR_STATIC, SInt8 ERR_VGIsWarnMsg, 0);
VAR_DCL_INIT ( EXTERN, VAR_STATIC, SInt8 ERR_VGIsErrMsg, 0);



#endif // End of #ifdef APP_ERR_MSG_MACROS_NOT_SUPPORTED


#endif
