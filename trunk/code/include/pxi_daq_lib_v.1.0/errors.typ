/*******************************************************************************
File      : x:\lib\com\errors\errors.typ
Goal      : Types definition if error messages logging library
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


#ifndef ERRORS_TYP
#define ERRORS_TYP

typedef SInt32 (*ERR_TUserErrorFunc) ( char Type, char* ErrLocation, char* ErrUserMsg, char* FullMsg );


#endif
