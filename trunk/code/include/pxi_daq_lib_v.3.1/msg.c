
/*******************************************************************************
File      : x:\lib\com\msg\msg.c
Goal      : Implement user messages print ( screen ) or log ( file ) functions.
          : It's usefull to send debug messages.
          :
Remark    : The way it works is controlled by 3 globals variables
          : MSG_VGLogClosed = 0 ( disable ) / 1 ( enable ) user messages
          : MSG_VGLogPath = '/msg/msg.txt' = path of messages logfile
          : MSG_VGLogFile = NULL ( output = file ) / stdout ( use stdout )
          : There is the same variables with RMSG_ instead of MSG_ they are
          : used for on-line remote monitoring messages.
          : The default state off these variable is set here, but you can
          : overwrite it at the beginning of a program
Prj date  : 2000 - 2002
File date :
Doc date  : 23/11/2002
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

#ifndef MSG_C
#define MSG_C


SInt32 MSG_FGenEnableLog ( SInt8 Chan, SInt8 Enable ) {
  MSG_VGALogClosed[Chan]  = Enable;
  MSG_VGALogEnabled[Chan] = Enable;
  return (0);
}

/* 07/04/2007 */

SInt8 MSG_FGenGetLogEnabled ( SInt8 Chan ) {
  return (MSG_VGALogEnabled[Chan]);
}


SInt32 MSG_FGenSetLogFilePath ( SInt8 Chan, char* LogFilePath ) {
  sprintf ( MSG_VGALogPath[Chan], "%s", LogFilePath );
  return (0);
}

char* MSG_FGenGetLogFilePath ( SInt8 Chan ) {
  return ( MSG_VGALogPath[Chan] );
}

SInt32 MSG_FGenStopLog ( SInt8 Chan, SInt8 Stop ) {
  MSG_VGADontLog[Chan] = Stop;
  return (0);
}


SInt32 MSG_FGenBegin ( SInt8 Chan, SInt8 Enable, char* FilePath ) {
  
  MSG_FGenEnableLog      ( Chan, Enable   );
  MSG_FGenSetLogFilePath ( Chan, FilePath );

  return (0);
}

SInt32 MSG_FBegin ( SInt8 Enable, char* FilePath ) {
  
  MSG_FGenEnableLog      ( MSG_CHAN_MSG, Enable   );
  MSG_FGenSetLogFilePath ( MSG_CHAN_MSG, FilePath );

  return (0);
}


SInt32 MSG_FEnd () {
    
  return (0);
}


SInt32 MSG_FEnableLog ( SInt8 Enable ) {
  return ( MSG_FGenEnableLog ( MSG_CHAN_MSG, Enable) );
}

SInt32 MSG_EnableLog ( SInt8 Enable ) {
  return ( MSG_FGenEnableLog ( MSG_CHAN_MSG, Enable) );
}

SInt8  MSG_FGetLogEnabled () {
  return ( MSG_FGenGetLogEnabled ( MSG_CHAN_MSG ) );
}

SInt32 MSG_FSetLogFilePath ( char* LogFilePath ) {
  return ( MSG_FGenSetLogFilePath ( MSG_CHAN_MSG, LogFilePath ) );
}

/* 11/06/2005 */

SInt32 MSG_FSetFileLogLevel ( SInt8 Level ) {
  
  MSG_VGFileLogLevel = Level;
  
  return (0);
}

/* 07/04/2007 */

SInt8 MSG_GSetFileLogLevel () {
  return (MSG_VGFileLogLevel);
}



/* 11/06/2005 */

SInt32 MSG_FSetUserLogLevel ( SInt8 Level ) {

  MSG_VGUserLogLevel = Level;

  return (0);
}

/* 07/04/2007 */

SInt8 MSG_FGetUserLogLevel () {
  return (MSG_VGUserLogLevel);
}

char* MSG_FGetLogFilePath ( void ) {
  return ( MSG_FGenGetLogFilePath ( MSG_CHAN_MSG ) );
}


/* 11/06/2005 */

SInt32 MSG_FSetUserMsgFunc ( TUserMsgFunc Func ) {
  
  MSG_VGUserMsgFunc = Func;
  
  return (0);
}


/* 07/04/2007 */

char* MSG_FGenGetMsgLogConfStr ( SInt8 Chan ) {
  
  static char VStr[MSG_CMT_SZ+1];
  
  sprintf ( VStr, "Messages log configuration \n\n - Log enabled = %d \n - Stop log = %d \n - File log level = %d \n - Log file name = %s \n - User log level = %d \n\n ", MSG_VGALogEnabled[Chan], MSG_VGADontLog[Chan], MSG_VGFileLogLevel, MSG_VGALogPath[Chan], MSG_VGUserLogLevel );
    
  return (VStr);
}

/* 07/04/2007 */

char* MSG_FGetMsgLogConfStr () {
  return ( MSG_FGenGetMsgLogConfStr (MSG_CHAN_MSG) );
}

SInt32 MSG_FGenMsg ( SInt8 Chan, SInt8 Level ) {

  static char VMsg[256 /* MSG_CMT_SZ  include file not visble from ROOT => must be solved */];

  if ( (MSG_VGFileLogLevel == 0) && (MSG_VGUserLogLevel == 0) ) {
    return (0);
  }
  
  sprintf ( VMsg, "MSG %.7d => %s \n", MSG_VGAMsgCnt[Chan]++, MSG_VGAStrMsg[Chan]);

  if ( (MSG_VGFileLogLevel != 0) && (MSG_VGFileLogLevel >= Level) ) {
  
    if ( MSG_VGALogClosed[Chan] && (MSG_VGALogFile[Chan] != stdout ) ) {
      MSG_VGALogClosed[Chan] = 0;
      
      // 29/10/2010 => Alway overwrite log file
      
      // if ( ( MSG_VGALogFile[Chan] = fopen ( MSG_VGALogPath[Chan], "a" ) ) == NULL ) {
      //   MSG_VGALogFile[Chan] = fopen ( MSG_VGALogPath[Chan], "w" );
      // }
      
      MSG_VGALogFile[Chan] = fopen ( MSG_VGALogPath[Chan], "w" );
      
    }
    
    if ( (MSG_VGADontLog[Chan] == 0) && (MSG_VGALogFile[Chan] != NULL) ) {
      fprintf ( MSG_VGALogFile[Chan], "%s", VMsg );
      fflush  ( MSG_VGALogFile[Chan] );
    }

  } /* End if ( Level >= MSG_VGFileLogLevel ) */
  
  if ( (MSG_VGUserLogLevel != 0) && (MSG_VGUserLogLevel >= Level) ) {
  
    if ( MSG_VGUserMsgFunc != NULL )  {
      MSG_VGUserMsgFunc ( VMsg );
    }
    
  } /* End if ( Level >= MSG_VGUserLogLevel ) */
  

  return (0);
  
/*
  if ( MSG_VGALogClosed[Chan] && (MSG_VGALogFile[Chan] != stdout ) ) {
    MSG_VGALogClosed[Chan] = 0;
    if ( ( MSG_VGALogFile[Chan] = fopen ( MSG_VGALogPath[Chan], "a" ) ) == NULL ) {
      MSG_VGALogFile[Chan] = fopen ( MSG_VGALogPath[Chan], "w" );
    }
  }
  
  if ( (MSG_VGADontLog[Chan] == 0) && (MSG_VGALogFile[Chan] != NULL) ) {
    fprintf ( MSG_VGALogFile[Chan], "MSG %.4d =>", MSG_VGAMsgCnt[Chan]++ );
    fprintf ( MSG_VGALogFile[Chan], MSG_VGAStrMsg[Chan] );
    fprintf ( MSG_VGALogFile[Chan], "\n" );
    fflush  ( MSG_VGALogFile[Chan] );
  }
  
  return (0);
    
*/

}


#endif

