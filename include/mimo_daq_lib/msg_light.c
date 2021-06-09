
/**
* ----------------------------------------------------------------------------------
* \file              X:\prj\win\mimosis_1\run_read_light_bt\com\msg_light.c
* \brief Goal      : Genral messages library - Functions
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


#ifndef MSG_L_C
#define MSG_L_C


#include <stdio.h>
#include <stdlib.h>


#ifdef APP_ERR_MSG_MACROS_NOT_SUPPORTED

SInt32 MSG_FGenEnableLog ( SInt8 Chan, SInt8 Enable ) {

  return (0);
}

/* 07/04/2007 */

SInt8 MSG_FGenGetLogEnabled ( SInt8 Chan ) {
  return (0);
}


SInt32 MSG_FGenSetLogFilePath ( SInt8 Chan, char* LogFilePath ) {
  return (0);
}

char* MSG_FGenGetLogFilePath ( SInt8 Chan ) {
  return ( MSG_VGMsgNotAvailable );
}

SInt32 MSG_FGenStopLog ( SInt8 Chan, SInt8 Stop ) {
  return (0);
}


SInt32 MSG_FGenBegin ( SInt8 Chan, SInt8 Enable, char* FilePath ) {

  return (0);
}

SInt32 MSG_FBegin ( SInt8 Enable, char* FilePath ) {

  return (0);
}


SInt32 MSG_FEnd () {

  return (0);
}


SInt32 MSG_FEnableLog ( SInt8 Enable ) {
  return (0 );
}

SInt32 MSG_EnableLog ( SInt8 Enable ) {
  return (0);
}

SInt8  MSG_FGetLogEnabled () {
  return (0);
}

SInt32 MSG_FSetLogFilePath ( char* LogFilePath ) {
  return (0);
}

/* 11/06/2005 */

SInt32 MSG_FSetFileLogLevel ( SInt8 Level ) {
  
  return (0);
}

/* 07/04/2007 */

SInt8 MSG_GSetFileLogLevel () {
  return (0);
}



/* 11/06/2005 */

SInt32 MSG_FSetUserLogLevel ( SInt8 Level ) {

  return (0);
}

/* 07/04/2007 */

SInt8 MSG_FGetUserLogLevel () {
  return (0);
}

char* MSG_FGetLogFilePath ( void ) {
  return ( MSG_VGMsgNotAvailable );
}


/* 11/06/2005 */

SInt32 MSG_FSetUserMsgFunc ( TUserMsgFunc Func ) {

  return (0);
}


/* 07/04/2007 */

char* MSG_FGenGetMsgLogConfStr ( SInt8 Chan ) {

  return ( MSG_VGMsgNotAvailable );
}

/* 07/04/2007 */

char* MSG_FGetMsgLogConfStr () {

  return ( MSG_VGMsgNotAvailable  );
}

SInt32 MSG_FGenMsg ( SInt8 Chan, SInt8 Level ) {

  return (0);

}

// GC - 20/02/2020

char* MSG_FGetLastMsgLong ( UInt16 Chan ) {
  

  return (MSG_OUT);
}

// GC - 20/02/2020

char* MSG_FGetLastMsgShort ( UInt16 Chan ) {


  return (MSG_OUT);
  
}



// Returns messages flag an reset it
// The message level must be <= Level
// Mainly for interface / LabVIEW => Testing errors messages presence by polling
// GC 08/04/2021

SInt8 MSG_FIsMsg ( UInt16 Chan, SInt8 Level ) {

  return (0);
}





#else // Of #ifdef APP_ERR_MSG_MACROS_NOT_SUPPORTED

// -----------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------


SInt32 MSG_FGenEnableLog ( SInt8 Chan, SInt8 Enable ) {
  
//  MSG_VGALogClosed[Chan]  = Enable;
  MSG_VGALogEnabled[Chan] = Enable;

  if (Enable){
    
    if ( MSG_VGALogClosed[Chan] && (MSG_VGALogFile[Chan] != stdout ) ) {
      MSG_VGALogClosed[Chan] = 0;
      
      // 29/10/2010 => Alway overwrite log file
      
      // if ( ( MSG_VGALogFile[Chan] = fopen ( MSG_VGALogPath[Chan], "a" ) ) == NULL ) {
      //   MSG_VGALogFile[Chan] = fopen ( MSG_VGALogPath[Chan], "w" );
      // }
      
      MSG_VGALogFile[Chan] = fopen ( MSG_VGALogPath[Chan], "w" );
      
    }
  }
  
  // 23/05/2019 - GC try to remove the closing of the file
  
/*	else{
		if ( MSG_VGALogClosed[Chan] == 0){
			fclose(MSG_VGALogFile[Chan]);
			MSG_VGALogClosed[Chan] = 1;
			MSG_VGALogFile[Chan] = NULL;
		}
		
  }
*/  




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
  
  MSG_FGenSetLogFilePath ( Chan, FilePath );
  MSG_FGenEnableLog      ( Chan, Enable   );

  return (0);
}

SInt32 MSG_FBegin ( SInt8 Enable, char* FilePath ) {
  
 SInt8 ViChan; 
  
 // remove ( FilePath ); // 31/01/13 because file is not rewritten when lib used in LabVIEW via a DLL
                       // Don't know why, it should not be needed, but it's required
                       // 04 12 2017 MS removed because it seems that it makes prolblems

  // 30/11/2019 begin :
  // Upgrade in order to
  // - get one couple of log files per DLL instance loaded by application ( a suffix 0, 1, is added to log files names)
  // - keep one single couple of log files for the application itself, each time a library (compiled by src inclusion)
  // defines a new log file, it is ignored if one is laready opened => the first log file name configured is used
  //
  // DLL compilation         => DO NOT define APP_SAME_LOG_FILES_NAMES
  // Application compilation => Define APP_SAME_LOG_FILES_NAMES
  
          
  
  #ifndef APP_SAME_LOG_FILES_NAMES
  
    for ( ViChan = 0; ViChan < MSG_CHAN_NB; ViChan++ ) {
    
      MSG_VGALogClosed[ViChan] = 1;
    
      if ( MSG_VGALogFile[ViChan] != NULL ) {
        fflush ( MSG_VGALogFile[ViChan] );
        fclose ( MSG_VGALogFile[ViChan] );
      }
        
    }
  
  #endif
  
  // 30/11/2019 end
  

  // 23/05/2019 - GC : Code used since 23/05/2019 (functions calls swapped / before 23/05/2019)
  //
  // MS has fixed the bug of "multiple files opening" which requires to swap the call order of
  // the following functions. It has been done for error log lib, it seems it has not been fully
  // done for this lib, it is done for MSG_FGenBegin (...) but not here for MSG_FBegin (...),it is ok now.

  MSG_FGenSetLogFilePath ( MSG_CHAN_MSG, FilePath );
  MSG_FGenEnableLog      ( MSG_CHAN_MSG, Enable   );

  // 23/05/2019 - GC : Code used before 23/05/2019 => Msg file never created
  //
  //  MSG_FGenEnableLog      ( MSG_CHAN_MSG, Enable   );
  //  MSG_FGenSetLogFilePath ( MSG_CHAN_MSG, FilePath );
  
  return (0);
}


SInt32 MSG_FEnd () {

	SInt32 VIndex;
	
	for (VIndex = 0 ; VIndex < MSG_CHAN_NB ; VIndex++){

			if (MSG_VGALogClosed[VIndex] == 0){
				fclose(MSG_VGALogFile[VIndex]);
				MSG_VGALogClosed[VIndex] = 1;
			}



	
	}
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

    
  // 03/11/2020 - No log if called in class constructor
  
  #ifndef CC_FORCE_LOG_IN_CONSTRUCTORS
  
    if ( ERR_VGConstructorNoLog ) {
      return (0);
    }
  
  #endif
  
    
    
  if ( (MSG_VGFileLogLevel == 0) && (MSG_VGUserLogLevel == 0) ) {
    return (0);
  }
  
  sprintf ( VMsg, "MSG %.7d => %s \n", MSG_VGAMsgCnt[Chan]++, MSG_VGAStrMsg[Chan]);

  sprintf ( MSG_VGAStrMsgLong[Chan], "%s", VMsg );

  if ( (MSG_VGFileLogLevel != 0) && (MSG_VGFileLogLevel >= Level) ) {
  
    
    MSG_VGIsMsgAndLevel[Chan] = Level; // 08/04/2021
    
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

// GC - 20/02/2020

char* MSG_FGetLastMsgLong ( UInt16 Chan ) {
  
  static char VStrErr[GLB_CMT_SZ];
  
  if ( Chan >= MSG_CHAN_NB ) {
    sprintf ( VStrErr, "Error => Chan = %d > Max = %d", Chan, MSG_CHAN_NB - 1 );
    return ( VStrErr );
  }
  
  return ( MSG_VGAStrMsgLong[Chan] );
}

// GC - 20/02/2020

char* MSG_FGetLastMsgShort ( UInt16 Chan ) {
  
  
  static char VStrErr[GLB_CMT_SZ];
  
  if ( Chan >= MSG_CHAN_NB ) {
    sprintf ( VStrErr, "Error => Chan = %d > Max = %d", Chan, MSG_CHAN_NB - 1 );
    return ( VStrErr );
  }
  
  return ( MSG_VGAStrMsg[Chan] );
  
}



// Returns messages flag an reset it
// The message level must be <= Level
// Mainly for interface / LabVIEW => Testing errors messages presence by polling
// GC 08/04/2021

SInt8 MSG_FIsMsg ( UInt16 Chan, SInt8 Level ) {
  
  
  if ( Chan >= MSG_CHAN_NB ) {
    return (-1);
  }
  
  
  if ( (MSG_VGIsMsgAndLevel[Chan] != 0) && (MSG_VGIsMsgAndLevel[Chan] <= Level) ) {
    MSG_VGIsMsgAndLevel[Chan] = 0;
    return (1);
  }
  
  return (0);
}




#endif // Of #ifdef APP_ERR_MSG_MACROS_NOT_SUPPORTED



#endif
