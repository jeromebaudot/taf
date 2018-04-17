/*******************************************************************************
File      : x:\lib\com\msg\msg.typ
Goal      : Functions prototypes of user messages logging library
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

#ifndef MSG_H

#include "func_header.def"

#include "msg.def"
#include "msg.typ"


// FHEAD (SInt32 ERR_FBegin ( SInt8 Enable, char* FilePath );)

FHEAD (SInt32  MSG_FGenEnableLog ( SInt8 Chan, SInt8 Enable );)
FHEAD (SInt8   MSG_FGenGetLogEnabled ( SInt8 Chan );)
FHEAD (SInt32  MSG_FGenSetLogFilePath ( SInt8 Chan, char* LogFilePath );)
FHEAD (char*   MSG_FGenGetLogFilePath ( SInt8 Chan );)
FHEAD (SInt32  MSG_FGenStopLog ( SInt8 Chan, SInt8 Stop );)
FHEAD (char*   MSG_FGenGetMsgLogConfStr ( SInt8 Chan );)
FHEAD (SInt32  MSG_FGenBegin ( SInt8 Chan, SInt8 Enable, char* FilePath );)
FHEAD (SInt32  MSG_FGenMsg ( SInt8 Chan, SInt8 Level );)

FHEAD (SInt32  MSG_FBegin ( SInt8 Enable, char* FilePath );)
FHEAD (SInt32  MSG_EnableLog ( SInt8 Enable );)
FHEAD (SInt32  MSG_FEnableLog ( SInt8 Enable );)


FHEAD (SInt32 MSG_FSetFileLogLevel ( SInt8 Level );)
FHEAD (SInt32 MSG_FSetUserLogLevel ( SInt8 Level );)
  
FHEAD (SInt32  MSG_FSetLogFilePath ( char* LogFilePath );)

FHEAD (SInt32 MSG_FSetUserMsgFunc ( TUserMsgFunc Func );) 
  

FHEAD (SInt8 MSG_FGetLogEnabled ();)
FHEAD (SInt8 MSG_FGetFileLogLevel ();)
FHEAD (SInt8 MSG_FGetUserLogLevel ();)
FHEAD (char* MSG_FGetMsgLogConfStr ();)


FHEAD (char*   MSG_FGetLogFilePath ( void );)



#ifndef APP_DLL_IMPORT_EXPORT
  #ifndef MSG_H
    #define MSG_H
  #endif
#endif


#endif
