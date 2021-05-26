
/**
* ----------------------------------------------------------------------------------
* \file              X:\prj\win\mimosis_1\run_read_light_bt\com\errors_light.c
* \brief Goal      : Errors messages library - Functions
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


#ifndef ERR_L_C
#define ERR_L_C


#include <stdio.h>
#include <stdlib.h>

#include <stdint.h> // types
#include <string.h>
#include <errno.h>


/* ========================================= */
/* Conditional compilation for DAQ sources   */
/* ========================================= */

#define ROOT_ROOT // Disable code parts which can't compile under ROOT
#define CC_UNIX   // Specify operating system

//---------------------------------------------------------------------------------
// Conditionnal compilation switches
// ---------------------------------------------------------------------------------


#undef  CC_APP_GUI_CPP       // Disables C++ Builder GUI application => Memo for error and general messagess logging

#define CC_MSIS1_BDF_LIGHT   // Reduces functions, class integrated from library X:/lib/com/maps/msis1/Data
                             // in oder to minimize the risk of compilation warnings / errors

#define CC_NOT_CPP_BUILDER   // Removes items (cst, functions, etc ... ) only available under C++ Builder



#undef CC_INTERFACE_FILE     // To get variables created in this module


// #define APP_ERR_MSG_MACROS_NOT_SUPPORTED  // To be defined in case of compilation errors on errors and general messages libs
                                             // ..\..\com\errors_light.c, errors_light.h
                                             // ..\..\com\msg_light.c, msg_light.h
                                                 

#undef CC_APP_ERR_LOG_VIA_PRINTF             // If enabled => Error log message are automatically printed in console via printf () function
                                             // Rq : The sames messages are written in MIS1__TBtRunRead_DEF_ERR_LOG_FILE

#define CC_APP_MSG_LOG_VIA_PRINTF            // If enabled => General message log are automatically  printed in console via printf () function
                                             // Rq : The sames messages are written in MIS1__TBtRunRead_DEF_MSG_LOG_FILE

//

/* ========================== */
/* Application constants      */
/* ========================== */

#define MIS1__TBtRunRead_DEF_ERR_LOG_FILE "./Results/err_MIS1__TBtRunRead.txt"
#define MIS1__TBtRunRead_DEF_MSG_LOG_FILE "./Results/msg_MIS1__TBtRunRead.txt"
/* ========================================= */
/* Includes DAQ source files                 */
/* ========================================= */

// C4Pi test team general lib interface

#include "c4pi_test_team.h"
#include "globals_root.def"
#include "types.def"

// C4Pi test team general errors and general messages lib interface

#include "errors_light.h"
#include "msg_light.h"

// C4Pi test team Mimosis 1 data lib interface


#include "msis1_data.def"
#include "msis1_data.typ"
#include "msis1_data_exp.h"

// C4Pi test team general errors and general messages lib
// C code inclusion in application
//
// Remark : Ugly piggy way, but fast way and no time to do better ;-)
// anyway if someone wants to do better => please do
#ifdef APP_ERR_MSG_MACROS_NOT_SUPPORTED


SInt32 ERR_FEnableLog ( SInt8 Enable ) {
}


SInt32 ERR_EnableLog ( SInt8 Enable ) {
  return (0);
}

/* 07/04/2007 */

SInt32 ERR_FGetFileLogEnabled () {
  return (0);
}

/* 07/04/2007 */

SInt32 ERR_FGetUserLogEnabled () {
  return (0);
}


SInt32 ERR_FSetLogFilePath ( char* LogFilePath ) {
  return (0);
}

char* ERR_FGetLogFilePath () {
  return ( ERR_VGMsgNotAvailable );
}

SInt32 ERR_FBegin ( SInt8 Enable, char* FilePath ) {
  return (0);
}


SInt32 ERR_FEnd () {
  return (0);
}


SInt32 ERR_FStopLog ( SInt8 Stop ) {
  return (0);
}

/* 11/06/2005 */

SInt32 ERR_FSetFileLogLevel ( SInt8 LogLevel ) {
  return (0);
}

/* 07/04/2007 */

SInt8 ERR_FGetFileLogLevel () {
  return (0);
}

/* 07/04/2007 */

char* ERR_FLogLevel2Str ( SInt8 Lvl ) {

  return ( ERR_VGMsgNotAvailable );
}


/* 07/04/2007 */

char* ERR_FGetFileLogLevelStr () {

  return ( ERR_VGMsgNotAvailable );
}


/* 11/06/2005 */

SInt32 ERR_FSetUserLogLevel ( SInt8 LogLevel ) {

  return (0);
}

/* 07/04/2007 */

SInt8 ERR_FGetUserLogLevel () {

  return (0);
}

/* 07/04/2007 */

char* ERR_FGetUserLogLevelStr () {
  
  return ( ERR_VGMsgNotAvailable );
}


/* 07/04/2007 */

char* ERR_FGetErrLogConfStr () {

  return ( ERR_VGMsgNotAvailable );
}


/* 11/06/2005 */

SInt32 ERR_FUserStopLog ( SInt8 Stop ) {

  return (0);
}

/* 10/06/2005 */

SInt32 ERR_FSetUserErrorFunc ( ERR_TUserErrorFunc Func ) {

  return (0);
}

// 25 05 18 MS added counter to the message system for debugging for mimosis0

SInt32 ERR_FGenError ( char MsgType ) {

  return (0);
}



char* ERR_FGetLastErrMsg () {
  
  return (ERR_OUT);
  
}


// GC - 20/02/2020

char* ERR_FGetLastErrMsgLong () {

  return (ERR_OUT);
  
}


// GC - 20/02/2020

char* ERR_FGetLastErrMsgShort () {
  
  return (ERR_OUT);
  
}

// Returns trace messages flag an reset it
// Mainly for interface / LabVIEW => Testing errors messages presence by polling
// GC 06/04/2021

SInt8 ERR_FIsTraceMsg () {

  return (0);  
}


// Returns warning messages flag an reset it
// Mainly for interface / LabVIEW => Testing errors messages presence by polling
// GC 06/04/2021

SInt8 ERR_FIsWarnMsg () {

  return (0);
}


// Returns errors messages flag an reset it
// Mainly for interface / LabVIEW => Testing errors messages presence by polling
// GC 06/04/2021

SInt8 ERR_FIsErrMsg () {


  return (0);
}


// Returns messages flag an reset it
// Mainly for interface / LabVIEW => Testing errors messages presence by polling
//
// ErrType
// - 0 => No error <=> Do nothing
// - 1 => Trace
// - 2 => Warnings
// - 3 => Errors
// - 4 => Warning + Errors
// - 5 => Any => Trace or Warning or Error
//

// GC 06/04/2021
//
// ErrType
// - 0 => No error <=> Do nothing
// - 1 => Trace
// - 2 => Warning
// - 3 => Error
// - 4 => Any => Trace or Warning or Error

// GC 14/04/2021 
// WARNING List of ErrType modified
// 

SInt8 ERR_FIsMsg ( SInt8 ErrType ) {
   
  return (0);
}




// Emul error messaget
// Mainly for interface / LabVIEW => Testing errors messages presence by polling
// ErrType
// - 0 => No error <=> Do nothing
// - 1 => Trace
// - 2 => Warning
// - 3 => Error
//
// GC 06/04/2021



SInt8 ERR_FEmulErr ( UInt8 ErrType, char* Msg) {


  return (0);
}



void FPrint ( void )
{
}


// 01/03/2012

SInt32 ERR_FGetEnumSz () {


  return (0);
}



//=========================================================================================


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 ERR_FPrintLvErrMsg ( SInt8 LvIsError, SInt32 LvErrCode, char* LvMsg, char* UserErrMsg, char* UserOkMsg, SInt32 UserErrLvl )
*          :
* \brief   : Prints an error message from LabVIEW (LV error cluster) to error log file
*          :
*          :
* \param   : LvIsError - Connecter to LV boolean of error cluster via cat bool to int
*          :             0 => No error, > 0 => error
*          :
* \param   : LvErrCode - Error code = LV SInt32 of error cluster
*          :
* \param   : LvMsg     - Text message from LV (char*) = LV text of error cluster
*          :
* \param   : UserErrMsg - Optional error msg from user (char*), printed in case LvIsError > 0
*          :
* \param   : UserErrOk  - Optional ok message from user (char*), printed in case LvIsError = 0
*          :
* \param   : UserErrLvl - Optional error level from user (SInt32), 0 = Not set / No error, 1 = Warning, 2 = Error
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : To give acces to this function in a DLL you MUST add its prototype in my_dll.cpp before error lib files inclusion
* \warning : add => extern "C" __declspec(dllexport) SInt32 ERR_FPrintLvErrMsg ( SInt8 LvIsError, SInt32 LvErrCode, char* LvMsg, char* UserMsg, SInt32 UserErrLvl );
* \warning :
* \warning :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 26/04/2019
* \date    : Doc date  : 26/04/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 ERR_FPrintLvErrMsg ( SInt8 LvIsError, SInt32 LvErrCode, char* LvMsg, char* UserErrMsg, char* UserOkMsg, SInt32 UserErrLvl ) {
  

  return (0);
}




#else // Of #ifdef APP_ERR_MSG_MACROS_NOT_SUPPORTED

// -----------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------


#ifdef CC_APP_OS9

char*  strerror ( SInt32 errno ) {
  
  static char VMsgStr[ERR_TOT_MSG_SZ+1];

  sprintf ( VMsgStr, "Not avalibale under OS9" );
  
  return (VMsgStr);
}

#endif

#ifndef CC_APP_WINDOWS


char *_strerror ( char* UsrStr ) {
  
  static char VMsgStr[ERR_TOT_MSG_SZ+1];

  #ifdef APP_ROOT
    sprintf ( VMsgStr, "%s : No errno str because not supported by ROOT", UsrStr );
  #else
    strncpy ( VMsgStr, UsrStr, ERR_USR_MSG_SZ-10 );
    strcat  ( VMsgStr, " : " );
    strncat ( VMsgStr, strerror ( errno ), ERR_SYS_MSG_SZ-10 );  
  #endif

  
  return (VMsgStr);
}


#endif

SInt32 ERR_FEnableLog ( SInt8 Enable ) {

  ERR_VGFileLogEnable = Enable;
  
  ERR_VGUserLogEnable = Enable;
  
  if (Enable){
    
    if ( ERR_VGLogClosed && (ERR_VGLogFile != stdout) && (ERR_VGLogFile != stderr) ) {
      ERR_VGLogClosed = 0;
      
      //
      // 29/10/10 => Don't open log file in append mode but overwrite it
      //
      // if (( ERR_VGLogFile = fopen ( ERR_VGLogPath, "a" ) ) == NULL ) { ERR_VGLogFile = fopen ( ERR_VGLogPath, "w" ); }
      
      // 02/06/12 => Enable again open in append mode for strip analysis LIB / DLL debug
      // if (( ERR_VGLogFile = fopen ( ERR_VGLogPath, "a" ) ) == NULL ) { ERR_VGLogFile = fopen ( ERR_VGLogPath, "w" ); }
      

      ERR_VGLogFile = fopen ( ERR_VGLogPath, "w" ); 
    }
  }
  
  // 11 04 2019 MS try to remove the closing of the file
  
/*	else{
		if (ERR_VGLogClosed == 0){
			fclose(ERR_VGLogFile);
			ERR_VGLogClosed = 1;
			ERR_VGLogFile = NULL;
		}
		
	}*/ 
  
  
  return (0);
}

SInt32 ERR_EnableLog ( SInt8 Enable ) {
  return ( ERR_FEnableLog ( Enable) );
}

/* 07/04/2007 */

SInt32 ERR_FGetFileLogEnabled () {
  return ( ERR_VGFileLogEnable );
}

/* 07/04/2007 */

SInt32 ERR_FGetUserLogEnabled () {
  return ( ERR_VGUserLogEnable );
}


SInt32 ERR_FSetLogFilePath ( char* LogFilePath ) {
  sprintf ( ERR_VGLogPath, "%s", LogFilePath );
  return (0);
}

char* ERR_FGetLogFilePath () {
  return ( ERR_VGLogPath );
}

SInt32 ERR_FBegin ( SInt8 Enable, char* FilePath ) {
  
  // 30/11/2019 begin :
  // Upgrade in order to
  // - get one couple of log files per DLL instance loaded by application ( a suffix 0, 1, is added to log files names)
  // - keep one single couple of log files for the application itself, each time a library (compiled by src inclusion)
  // defines a new log file, it is ignored if one is laready opened => the first log file name configured is used
  //
  // DLL compilation         => DO NOT define APP_SAME_LOG_FILES_NAMES
  // Application compilation => Define APP_SAME_LOG_FILES_NAMES
  
    
  #ifndef APP_SAME_LOG_FILES_NAMES
  
    ERR_VGLogClosed = 1;

    if ( ERR_VGLogFile != NULL ) {
      fflush (ERR_VGLogFile);
      fclose (ERR_VGLogFile);
    }

  #endif
  
  // 30/11/2019 end modif
  
  ERR_FSetLogFilePath ( FilePath );
  ERR_FEnableLog ( Enable );
    
  return (0);
}


SInt32 ERR_FEnd () {
	if (ERR_VGLogClosed == 0){
		fclose(ERR_VGLogFile);
		ERR_VGLogClosed = 1;
	}
  return (0);
}


SInt32 ERR_FStopLog ( SInt8 Stop ) {
  ERR_VGFileDontLog = Stop;
  return (0);
}

/* 11/06/2005 */

SInt32 ERR_FSetFileLogLevel ( SInt8 LogLevel ) {
  ERR_VGFileLogLevel = LogLevel;
  return (0);
}

/* 07/04/2007 */

SInt8 ERR_FGetFileLogLevel () {
  return ( ERR_VGFileLogLevel );
}

/* 07/04/2007 */

char* ERR_FLogLevel2Str ( SInt8 Lvl ) {
  
  static char VStr[GLB_CMT_SZ+1];
  
  switch ( Lvl ) {
    
    case ERR_LOG_LVL_NONE : {
      sprintf ( VStr, "ERR_LOG_LVL_NONE" );
      break; }
    
    case ERR_LOG_LVL_ALL : {
      sprintf ( VStr, "ERR_LOG_LVL_ALL" );
      break; }
    
    case ERR_LOG_LVL_WARINGS_ERRORS : {
      sprintf ( VStr, "ERR_LOG_LVL_WARINGS_ERRORS" );
      break; }

    case ERR_LOG_LVL_ERRORS : {
      sprintf ( VStr, "ERR_LOG_LVL_ERRORS" );
      break; }
    
    default : {
      sprintf ( VStr, "Unknow error level = %d",  Lvl );
      break; }
    
  }
  
  return ( VStr );
}


/* 07/04/2007 */

char* ERR_FGetFileLogLevelStr () {
  
  return (  ERR_FLogLevel2Str ( ERR_VGFileLogLevel ) );
}


/* 11/06/2005 */

SInt32 ERR_FSetUserLogLevel ( SInt8 LogLevel ) {
  ERR_VGUserLogLevel = LogLevel;
  return (0);
}

/* 07/04/2007 */

SInt8 ERR_FGetUserLogLevel () {
  return ( ERR_VGUserLogLevel );
}

/* 07/04/2007 */

char* ERR_FGetUserLogLevelStr () {
  
  return (  ERR_FLogLevel2Str ( ERR_VGUserLogLevel ) );
}


/* 07/04/2007 */

char* ERR_FGetErrLogConfStr () {

  static char VStr[GLB_CMT_SZ+1];
  
  sprintf ( VStr, "Error log configuration \n\n - File log enabled = %d \n - File log level = %s \n - File stop log =%d \n - Log file name = %s \n - User log enabled = %d \n - User log level = %s \n - User stop log =%d \n\n", ERR_VGFileLogEnable, ERR_FGetFileLogLevelStr (), ERR_VGFileDontLog, ERR_VGLogPath, ERR_VGUserLogEnable, ERR_FGetUserLogLevelStr (), ERR_VGUserDontLog );
  
  return (VStr);
}


/* 11/06/2005 */

SInt32 ERR_FUserStopLog ( SInt8 Stop ) {
  ERR_VGUserDontLog = Stop;
  return (0);
}

/* 10/06/2005 */

SInt32 ERR_FSetUserErrorFunc ( ERR_TUserErrorFunc Func ) {

  ERR_VGUserErrorFunc = Func;

  return (0);
}

// 25 05 18 MS added counter to the message system for debugging for mimosis0

SInt32 ERR_FGenError ( char MsgType ) {

  static char VErrMsg[ERR_TOT_MSG_SZ];
  static SInt32 VErrPrintCnt = 0;//
  
  
  // 03/11/2020 - No log if called in class constructor
  
  #ifndef CC_FORCE_LOG_IN_CONSTRUCTORS
  
    if ( ERR_VGConstructorNoLog ) {
      return (0);
    }
  
  #endif
  
  ERR_VGLastErrMsg = VErrMsg;

  /* ERR_VGLogLevel */
  /* == ERR_LOG_LVL_NONE           => No log            */
  /* == ERR_LOG_LVL_ALL            => Log 'T', 'W', 'E' */
  /* == ERR_LOG_LVL_WARINGS_ERRORS => Log 'W', 'E'      */
  /* == ERR_LOG_LVL_ERRORS         => Log 'E'           */


  if ( (ERR_VGFileLogLevel == ERR_LOG_LVL_NONE) && (ERR_VGUserLogLevel == ERR_LOG_LVL_NONE) ) {
    return (0);
  }


  // 06/04/2021
  
  switch ( MsgType ) {
    
    case 'T' : {
      ERR_VGIsTraceMsg = 1;
      break; }

    case 'W' : {
      ERR_VGIsWarnMsg = 1;
      break; }
    
    case 'E' : {
      ERR_VGIsErrMsg = 1;
      break; }
    
    
  }


  sprintf ( VErrMsg, "%d\\%s%s \n", VErrPrintCnt, ERR_VGStrLocationMsg , ERR_OUT );
  
  VErrPrintCnt++; //

  /* File log handling */

  while (1) {

    if ( ERR_VGFileLogLevel == ERR_LOG_LVL_NONE ) {
      break;
    }

    if ( (ERR_VGFileLogLevel == ERR_LOG_LVL_ERRORS) && (MsgType != 'E') ) {
      break;
    }

    if ( (ERR_VGFileLogLevel == ERR_LOG_LVL_WARINGS_ERRORS) && (MsgType == 'T') ) {
      break;
    }

    /* Write error message to file */

		//
		// 09/04/2019 MS
		//
		// moved the opening of the log file to the ERR_FEnableLog function
		//
		// 10/04/2019 MS
		//
		// enabled again to ensure compatibility with old software 
    //
    
    if ( ERR_VGLogClosed && (ERR_VGLogFile != stdout) && (ERR_VGLogFile != stderr) ) {
      
      ERR_VGLogClosed = 0;
      
      //
      // 29/10/10 => Don't open log file in append mode but overwrite it
      //
      // if (( ERR_VGLogFile = fopen ( ERR_VGLogPath, "a" ) ) == NULL ) { ERR_VGLogFile = fopen ( ERR_VGLogPath, "w" ); }
      
      // 02/06/12 => Enable again open in append mode for strip analysis LIB / DLL debug
      //
      // if (( ERR_VGLogFile = fopen ( ERR_VGLogPath, "a" ) ) == NULL ) { ERR_VGLogFile = fopen ( ERR_VGLogPath, "w" ); }
      

      ERR_VGLogFile = fopen ( ERR_VGLogPath, "w" ); 
    }

    if ( (ERR_VGFileDontLog == 0) && (ERR_VGLogFile != NULL) ) {
      fprintf ( ERR_VGLogFile, VErrMsg );
      fflush  ( ERR_VGLogFile );
    }

    break;
  }

  /* User log handling */

  while (1) {

    if ( ERR_VGUserLogLevel == ERR_LOG_LVL_NONE ) {
      break;
    }

    if ( (ERR_VGUserLogLevel == ERR_LOG_LVL_ERRORS) && (MsgType != 'E') ) {
      break;
    }

    if ( (ERR_VGUserLogLevel == ERR_LOG_LVL_WARINGS_ERRORS) && (MsgType == 'T') ) {
      break;
    }

    /* Call user error function */

    if ( (ERR_VGUserDontLog == 0) && (ERR_VGUserErrorFunc != NULL) ) {
      ERR_VGUserErrorFunc ( MsgType, ERR_VGStrLocationMsg , ERR_OUT, VErrMsg );
    }

    break;
  }






  return (0);
}



char* ERR_FGetLastErrMsg () {
  
  return (ERR_VGLastErrMsg);
  
}


// GC - 20/02/2020

char* ERR_FGetLastErrMsgLong () {
  
  return (ERR_VGLastErrMsg);
  
}


// GC - 20/02/2020

char* ERR_FGetLastErrMsgShort () {
  
  return (ERR_OUT);
  
}

// Returns trace messages flag an reset it
// Mainly for interface / LabVIEW => Testing errors messages presence by polling
// GC 06/04/2021

SInt8 ERR_FIsTraceMsg () {
  
  if ( ERR_VGIsTraceMsg ) {
    ERR_VGIsTraceMsg = 0;
    return (1);
  }
  
  return (0);  
}


// Returns warning messages flag an reset it
// Mainly for interface / LabVIEW => Testing errors messages presence by polling
// GC 06/04/2021

SInt8 ERR_FIsWarnMsg () {
  
  if ( ERR_VGIsWarnMsg ) {
    ERR_VGIsWarnMsg = 0;
    return (1);
  }
  
  return (0);
}


// Returns errors messages flag an reset it
// Mainly for interface / LabVIEW => Testing errors messages presence by polling
// GC 06/04/2021

SInt8 ERR_FIsErrMsg () {
  
  if ( ERR_VGIsErrMsg ) {
    ERR_VGIsErrMsg = 0;
    return (1);
  }
  
  return (0);
}


// Returns messages flag an reset it
// Mainly for interface / LabVIEW => Testing errors messages presence by polling
//
// ErrType
// - 0 => No error <=> Do nothing
// - 1 => Trace
// - 2 => Warnings
// - 3 => Errors
// - 4 => Warning + Errors
// - 5 => Any => Trace or Warning or Error
//

// GC 06/04/2021
//
// ErrType
// - 0 => No error <=> Do nothing
// - 1 => Trace
// - 2 => Warning
// - 3 => Error
// - 4 => Any => Trace or Warning or Error

// GC 14/04/2021 
// WARNING List of ErrType modified
// 

SInt8 ERR_FIsMsg ( SInt8 ErrType ) {
  
  
  switch ( ErrType ) {
    
    case 0 : {
      // Nothing to do
      return (0);
      break; }
    
    
    case 1 : {
      return ( ERR_FIsTraceMsg () );
      break; }
    
    
    case 2 : {
      return ( ERR_FIsWarnMsg () );
      break; }
    
    
    case 3 : {
      return ( ERR_FIsErrMsg () );
      break; }
    
    
    case 4 : {
      
      if ( ERR_VGIsWarnMsg || ERR_VGIsErrMsg ) {
        ERR_VGIsWarnMsg  = 0;
        ERR_VGIsErrMsg   = 0;
        return (1);
      }
      
      else {
        return (0);
      }
      
      break; }
    
    
    case 5 : {
      
      if ( ERR_VGIsTraceMsg || ERR_VGIsWarnMsg || ERR_VGIsErrMsg ) {
        ERR_VGIsTraceMsg = 0;
        ERR_VGIsWarnMsg  = 0;
        ERR_VGIsErrMsg   = 0;
        return (1);
      }
      
      else {
        return (0);  
      }
            
      break; }
    
    
  }
  
   
  return (0);
}




// Emul error messaget
// Mainly for interface / LabVIEW => Testing errors messages presence by polling
// ErrType
// - 0 => No error <=> Do nothing
// - 1 => Trace
// - 2 => Warning
// - 3 => Error
//
// GC 06/04/2021



SInt8 ERR_FEmulErr ( UInt8 ErrType, char* Msg) {
  

  // Check param
  
  err_retnull (  Msg, (ERR_OUT,"Abort => Msg == NULL") );
  

  // Emul message

  switch ( ErrType ) {
    
    case 0 : {
      // Nothing to do
      break; }
    
    
    case 1 : {
      err_trace (( ERR_OUT, "%s", Msg ));
      break; }
    

    case 2 : {
      err_warning (( ERR_OUT, "%s", Msg ));
      break; }
    
    
    case 3 : {
      err_error (( ERR_OUT, "%s", Msg ));
      break; }    
    
  }


  return (0);
}



void FPrint ( void )
{
  printf ( "%s \n", VGOut );
}


// 01/03/2012

SInt32 ERR_FGetEnumSz () {

  SInt32 VSz;

  typedef enum EEnumTest { FIRST, PREC, NEXT, LAST } TEnumTest;

  VSz = sizeof ( TEnumTest );
  
  err_trace (( ERR_OUT, "Enum size = %d Bytes", VSz ));
  
  return (VSz);
}



//=========================================================================================


/* DOC_FUNC_BEGIN */
/**
===================================================================================
* \fn      : SInt32 ERR_FPrintLvErrMsg ( SInt8 LvIsError, SInt32 LvErrCode, char* LvMsg, char* UserErrMsg, char* UserOkMsg, SInt32 UserErrLvl )
*          :
* \brief   : Prints an error message from LabVIEW (LV error cluster) to error log file
*          :
*          :
* \param   : LvIsError - Connecter to LV boolean of error cluster via cat bool to int
*          :             0 => No error, > 0 => error
*          :
* \param   : LvErrCode - Error code = LV SInt32 of error cluster
*          :
* \param   : LvMsg     - Text message from LV (char*) = LV text of error cluster
*          :
* \param   : UserErrMsg - Optional error msg from user (char*), printed in case LvIsError > 0
*          :
* \param   : UserErrOk  - Optional ok message from user (char*), printed in case LvIsError = 0
*          :
* \param   : UserErrLvl - Optional error level from user (SInt32), 0 = Not set / No error, 1 = Warning, 2 = Error
*          :
* \return  : Error code
*          :   0 - OK
*          : < 0 - Error
*          :
* \warning : Globals   :
* \warning : Remark    : To give acces to this function in a DLL you MUST add its prototype in my_dll.cpp before error lib files inclusion
* \warning : add => extern "C" __declspec(dllexport) SInt32 ERR_FPrintLvErrMsg ( SInt8 LvIsError, SInt32 LvErrCode, char* LvMsg, char* UserMsg, SInt32 UserErrLvl );
* \warning :
* \warning :
* \warning : Level     :
*          :
* \warning : Items not filled now :
*  todo    :
*          :
*  bug     :
*          :
* \date    : Date      : 26/04/2019
* \date    : Doc date  : 26/04/2019
* \author  : Name      : Gilles CLAUS
* \author  : E-mail    : gilles.claus@iphc.cnrs.fr
* \author  : Labo      : IPHC
*
===================================================================================
*/
/* DOC_FUNC_END */

SInt32 ERR_FPrintLvErrMsg ( SInt8 LvIsError, SInt32 LvErrCode, char* LvMsg, char* UserErrMsg, char* UserOkMsg, SInt32 UserErrLvl ) {
  
  SInt32 VRetError;
  
  
  VRetError = 0;
  
  
  // If no error 
  
  if ( LvIsError == 0 ) {
    err_retok (( ERR_OUT, "$LV: %s", UserOkMsg ));
    return (VRetError);
  }
  
  // If error
  
  // Force UserErrLvl to "error", in case it has not been set, or set incorrectly
  // 0 => Not set or No error
  // 1 => Warning
  // 2 => Error
  
  if ( UserErrLvl == 0 ) {
    UserErrLvl = 2;
  }
  
  switch ( UserErrLvl ) {
    
    // Should never happen here, but who knows ...
    
    case 0 : {
      VRetError = 0;
      err_retok (( ERR_OUT, "$LV: LvMsg = %s - UserMsg = %s", LvMsg, UserErrMsg ));
      break; }
    
    case 1 : {
      VRetError = -1;
      err_warning (( ERR_OUT, "$LV: LvErrCode = %d - LvMsg = %s - UserMsg = %s", LvErrCode, LvMsg, UserErrMsg ));
      break; }
    
    case 2 : {
      VRetError = -1;
      err_error (( ERR_OUT, "$LV: LvErrCode = %d - LvMsg = %s - UserMsg = %s", LvErrCode, LvMsg, UserErrMsg ));
      break; }
    
    default : {
      VRetError = -1;
      err_warning (( ERR_OUT, "$LV: Unknown UserErrLvl = %d <> 0=No,1=Warning,2=Error - LvErrCode = %d - LvMsg = %s - UserMsg = %s", UserErrLvl, LvErrCode, LvMsg, UserErrMsg ));
      break; }
    
  }
  
  return (VRetError);
}




#endif // End of #ifdef APP_ERR_MSG_MACROS_NOT_SUPPORTED



#endif
