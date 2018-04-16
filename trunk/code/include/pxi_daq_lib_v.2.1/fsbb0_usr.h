
/*******************************************************************************
File      : x:\lib\com\maps\fsbb0\fsbb0_usr.h
Goal      : Functions prototypes of Ultimate 1 library.
          : It provides Ultimate 1 types definition and data handling functions.
Prj date  : 28/02/2011
File date : 28/02/2011
Doc date  : //200
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


#ifndef FSBB0_USR_H

#include "func_header.def"


// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, char* FSBB0_FGetVersion ();)
// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, ;)
// FHEAD ( SInt32 REF_FHello ();)

FHEAD ( SInt32 FSBB0_FBegin ( SInt8 FileErrLogLvl, char* FileErrFile );)
FHEAD ( SInt32 FSBB0_FEnd ();)


#ifndef APP_DLL_IMPORT_EXPORT
  #ifndef FSBB0_USR_H
    #define FSBB0_USR_H
	#endif
#endif


#endif
