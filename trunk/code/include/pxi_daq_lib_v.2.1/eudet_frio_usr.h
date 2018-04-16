 
/*******************************************************************************
File      : x:\lib\win\eudet_frio\eudet_frio_usr.h
Goal      : Functions prototypes of USER PART of flex rio board library for EUDET Mimosa 26 DAQ
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


#ifndef EUDET_FRIO_USR_H

#include "func_header.def"



// Publised functions via DLL interface -> Can be called by application using the DLL

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO_USR__FBegin ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO_USR__FEnd ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO_USR__FGetLineCmdPar ();)



FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO_USR__FOnAppStart ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO_USR__FOnAppStartByRunCtrl    ( char* RunCtrlIPAddr, SInt32 RunCtrlPort, SInt32 DaqPort, SInt8 CrIndexFile, SInt32 ReadoutMode );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO_USR__FOnAppStartByRunCtrlExt ( SInt8 MapsName, char* RunCtrlIPAddr, SInt32 RunCtrlPort, SInt32 DaqPort, SInt8 CrIndexFile, SInt32 ReadoutMode, SInt8 ErrLogLvl, SInt8 MsgLogLvl );)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO_USR__FOnConf ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO_USR__FOnConfExt ( SInt8 MapsName );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO_USR__FOnConfByRunCtrl ( EFRIO__TRunCont* PtRunCont );)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO_USR__FOnStartRun ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO_USR__FOnStartRunByRunCtrl ();)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO_USR__FOnGetRunStatus ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO_USR__FOnGetRunStatusByRunCtrl ();)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO_USR__FOnStopRun ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO_USR__FOnStopRunByRunCtrl ();)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO_USR__FOnAppExit ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO_USR__FOnAppExitByRunCtrl ();)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, char*  EFRIO_USR__FGetLastMsgToGui ();)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO_USR__FSetRunCtrlMsgHandlingOptions ( SInt8 DisableRunCtrlMsgProcessing, SInt8 EnableRunCtrlMsgDisplay );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO_USR__FIsRunCtrlMsgReady ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, char*  EFRIO_USR__FGetCtrlMsg ();)

// Private functions private -> Can't be called by application using the DLL

FHEAD ( SInt32 EFRIO_USR_FPriv ();)
FHEAD ( DWORD WINAPI EFRIO_USR__FThreadRespondRunCtrl ( LPVOID lpParam );)



#ifndef APP_DLL_IMPORT_EXPORT
  #ifndef EUDET_FRIO_USR_H
    #define EUDET_FRIO_USR_H
    #endif
#endif


#endif