
/*******************************************************************************
File      : x:\lib\com\maps\mi26\mi26_usr.h
Goal      : Functions prototypes of Mi26 library.
          : It provides Mi26 types definition and data handling functions.
Prj date  : 05/12/2007
File date : 24/11/2008
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


#ifndef MI26_USR_H

#include "func_header.def"


// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, char* MI26_FGetVersion ();)
// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, ;)
// FHEAD ( SInt32 REF_FHello ();)

FHEAD ( SInt32 MI26_FBegin ( SInt8 FileErrLogLvl, char* FileErrFile );)
FHEAD ( SInt32 MI26_FEnd ();)


#ifndef APP_DLL_IMPORT_EXPORT
  #ifndef MI26_USR_H
    #define MI26_USR_H
	#endif
#endif


#endif