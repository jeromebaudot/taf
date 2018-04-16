
/*******************************************************************************
File      : x:\lib\com\maps\ult1\ult1.h
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


#ifndef ULT1_H

#include "func_header.def"


// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, char* ULT1_FGetVersion ();)
// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, ;)
// FHEAD ( SInt32 REF_FHello ();)

  

// $ FHEAD ( SInt32 ULT1_FBegin ( SInt8 FileErrLogLvl, char* FileErrFile );)
// $ FHEAD ( SInt32 ULT1_FEnd ();)

FHEAD ( SInt32 ULT1__FRegDiscriConvW32ToBit ( UInt32* PtSrc, UInt8* PtDest);)
FHEAD ( SInt32 ULT1__FRegDiscriConvBitToW32 ( UInt8* PtSrc, UInt32* PtDest );)
FHEAD ( SInt32 ULT1__FDiscriMatConvW32ToBit ( ULT1__TMatDiscriW32* PtSrc, ULT1__TMatDiscriBit* PtDest );)
FHEAD ( SInt32 ULT1__FDiscriMatConvBitToW32 ( ULT1__TMatDiscriBit* PtSrc, ULT1__TMatDiscriW32* PtDest );)

FHEAD ( SInt32 ULT1__FDiscriMatPrintHit     ( ULT1__TMatDiscriBit* PtMat, UInt32 Line, UInt32 FirstCol, UInt32 LastCol );)
FHEAD ( SInt32 ULT1__FDiscriEmulHit         ( ULT1__TMatDiscriBit* PtMat, UInt16 Hit0Line, UInt16 Hit0Col, UInt16 Hit1Line, UInt16 Hit1Col, UInt16 Hit2Line, UInt16 Hit2Col );)

FHEAD ( SInt32 ULT1__FMatDiscriPrintHit ( char* CmtStrTitle, SInt8 CmtSInt8MapsId, ULT1__TMatDiscriBit* PtDest );) 
  

#ifndef APP_DLL_IMPORT_EXPORT
  #ifndef ULT1_H
    #define ULT1_H
	#endif
#endif


#endif