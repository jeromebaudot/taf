
/*******************************************************************************
File      : x:\lib\win\tools\tools.h
Goal      : Functions prototypes of tools library.
Prj date  : 31/07/2007
File date : 31/07/2007
Doc date  : //200
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
----------------------------------------------------------------------------------
License   : You are free to use this source files for your own development as long
          : as it stays in a public research context. You are not allowed to use it
          : for commercial purpose. You must put this header with laboratory and
          : authors names in all development based on this library.
----------------------------------------------------------------------------------
Labo      : IPHC */
/*******************************************************************************/


#ifndef WIN_TOOLS_H

#include "func_header.def"


FHEAD ( int    FEdit2DecInt     ( TEdit* Ed );)
FHEAD ( int    FEdit2HexInt     ( TEdit* Ed );)
FHEAD ( float  FEdit2Float      ( TEdit* Ed );)
FHEAD ( char*  FEdit2Char       ( TEdit* Ed );)
FHEAD ( void   FEdit2CharBuff   ( TEdit* Ed, char* PtDest );)
FHEAD ( float  FPCh2Float       ( char* Str );)
FHEAD ( void   FDecInt2Edit     ( int Data, TEdit* Ed );)
FHEAD ( void   FHexInt2Edit     ( int Data, TEdit* Ed );)
FHEAD ( void   FFloat2Edit      ( float Data, TEdit* Ed );)
FHEAD ( SInt32 FChar2Edit       ( char* Text, TEdit* Ed );)
FHEAD ( char*  FFloat2PCh       ( float Data );)
FHEAD ( char*  FStr2PCh         ( AnsiString* Str );)
FHEAD ( char*  FEdit2PCh        ( TEdit* Ed );)


#ifndef APP_DLL_IMPORT_EXPORT
  #ifndef WIN_TOOLS_H
    #define WIN_TOOLS_H
  #endif
#endif


#endif