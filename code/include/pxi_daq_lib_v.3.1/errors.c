
/*******************************************************************************
File      : x:\lib\com\errors\errors.c
Goal      : Implement errors messages print ( screen ) or log ( file ) functions.
          : Each function use " errors macros " to log errors and return code.
          :
Remark    : The way it works is controlled by 3 globals variables
          : ERR_VGLogClosed = 0 ( disable ) / 1 ( enable ) errors messages
          : ERR_VGLogPath = '/tmp/errors.txt' = path of error logfile 
          : ERR_VGLogFile = NULL ( output = file ) / stdout ( use stdout )
          : The default state off these variable is set here, but you can
          : overwrite it at the beginning of a program
Prj date  : 2000 - 2002
File date :
Doc date  : 23/11/2002
Remark    : Library exists since 2000, but split in files .def, .typ, .var, .h, .c
          : was done on 20/02/2005.
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
----------------------------------------------------------------------------------
License   : You are free to use this source files for your own development as long
          : as it stays in a public research context. You are not allowed to use it
          : for commercial purpose. You must put this header with laboratory and
          : authors names in all development based on this library.
----------------------------------------------------------------------------------
Labo      : LEPSI */
/*******************************************************************************/


#ifndef ERRORS_C
#define ERRORS_C


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
  
  ERR_VGLogClosed     = Enable;
  ERR_VGFileLogEnable = Enable;
  ERR_VGUserLogEnable = Enable;
      
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
  ERR_FEnableLog ( Enable );
  ERR_FSetLogFilePath ( FilePath );
    
  return (0);
}


SInt32 ERR_FEnd () {
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

SInt32 ERR_FGenError ( char MsgType ) {

  static char VErrMsg[ERR_TOT_MSG_SZ];
  
  ERR_VGLastErrMsg = VErrMsg;

  /* ERR_VGLogLevel */
  /* == ERR_LOG_LVL_NONE           => No log            */
  /* == ERR_LOG_LVL_ALL            => Log 'T', 'W', 'E' */
  /* == ERR_LOG_LVL_WARINGS_ERRORS => Log 'W', 'E'      */
  /* == ERR_LOG_LVL_ERRORS         => Log 'E'           */


  if ( (ERR_VGFileLogLevel == ERR_LOG_LVL_NONE) && (ERR_VGUserLogLevel == ERR_LOG_LVL_NONE) ) {
    return (0);
  }

  sprintf ( VErrMsg, "%s%s \n", ERR_VGStrLocationMsg , ERR_OUT );

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




#endif



