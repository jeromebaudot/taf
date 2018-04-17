/*******************************************************************************
File      : x:\lib\com\errors\errors.h
Goal      : Functions prototypes definition of error messages logging library
Prj date  : 2000 - 2002
File date : 20/02/2005
Doc date  :
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

#ifndef ERRORS_H

#include "func_header.def"


#include "errors.def"
#include "errors.typ"


FHEAD (SInt32 ERR_FBegin ( SInt8 Enable, char* FilePath );)
FHEAD (SInt32 ERR_FEnableLog ( SInt8 Enable );)
FHEAD (SInt32 ERR_EnableLog ( SInt8 Enable );)
FHEAD (SInt32 ERR_FSetLogFilePath ( char* LogFilePath );)
FHEAD (SInt32 ERR_FStopLog ( SInt8 Stop );)
FHEAD (SInt32 ERR_FGenError ( char MsgType );)
FHEAD (SInt32 ERR_FSetUserErrorFunc ( ERR_TUserErrorFunc Func );)
FHEAD (SInt32 ERR_FSetFileLogLevel ( SInt8 LogLevel );)
FHEAD (SInt32 ERR_FSetUserLogLevel ( SInt8 LogLevel );)

FHEAD (SInt32 ERR_FGetFileLogEnabled ();)
FHEAD (SInt8  ERR_FGetFileLogLevel ();)
FHEAD (char*  ERR_FGetFileLogLevelStr ();)

FHEAD (SInt32 ERR_FGetUserLogEnabled ();)
FHEAD (SInt8  ERR_FGetUserLogLevel ();)
FHEAD (char*  ERR_FGetUserLogLevelStr ();)

FHEAD (char*  ERR_FGetLogFilePath ();)

FHEAD (char* ERR_FGetErrLogConfStr ();)



#ifndef CC_APP_WINDOWS
  FHEAD (char *_strerror ( char* UsrStr );)
#endif


FHEAD (void FPrint ( void );)

// FHEAD (;)



#ifndef APP_DLL_IMPORT_EXPORT
  #ifndef ERRORS_H
    #define ERRORS_H
  #endif
#endif


#endif
