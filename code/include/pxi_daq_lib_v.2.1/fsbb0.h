
/*******************************************************************************
File      : x:\lib\com\maps\fsbb0\fsbb0.h
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


#ifndef FSBB0_H

#include "func_header.def"


// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, char* FSBB0_FGetVersion ();)
// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, ;)
// FHEAD ( SInt32 REF_FHello ();)

// $ FHEAD ( SInt32 FSBB0_FBegin ( SInt8 FileErrLogLvl, char* FileErrFile );)
// $ FHEAD ( SInt32 FSBB0_FEnd ();)
FHEAD ( UInt8 FSBB0_FHammingDecoder8_4(UInt8 CodedIn);)
FHEAD ( char * FSBB0_FGetConfigName(SInt16 ConfigNr);)
FHEAD ( FSBB0__TTestPattern FSBB0_FReadConfigFile(char * FilePath, SInt8 PrintLevel);)
FHEAD ( FSBB0__TMatDiscriBit FSBB0_FReadResultBitFile(char * FilePath, SInt8 PrintLevel);)
FHEAD ( SInt32 FSBB0_FPrintZsFFrameRawHeader ( FSBB0__TZsFFrameRaw* Pt );)
FHEAD ( SInt32 FSBB0_FPrintZsFFrameRaw ( FSBB0__TZsFFrameRaw* Pt, SInt16 FirstW32, SInt16 LastW32 );)

FHEAD ( SInt32 FSBB0__FRegDiscriConvW32ToBit ( UInt32* PtSrc, SInt8* PtDest, SInt16 DiscriW32Sz );)
FHEAD ( SInt32 FSBB0__FRegDiscriConvBitToW32 ( SInt8* PtSrc, UInt32* PtDest, SInt16 DiscriW32Sz );)
FHEAD ( SInt32 FSBB0__FDiscriMatConvW32ToBit ( FSBB0__TMatDiscriW32* PtSrc, FSBB0__TMatDiscriBit* PtDest );)
FHEAD ( SInt32 FSBB0__FDiscriMatConvBitToW32 ( FSBB0__TMatDiscriBit* PtSrc, FSBB0__TMatDiscriW32* PtDest );)

FHEAD ( SInt32 FSBB0__FDiscriMatPrintHit     ( FSBB0__TMatDiscriBit* PtMat, UInt32 Line, UInt32 FirstCol, UInt32 LastCol );)
FHEAD ( SInt32 FSBB0__FDiscriEmulHit         ( FSBB0__TMatDiscriW32* PtDest, SInt16 HitPattern, SInt16 Hit0Line, SInt16 Hit0Col, SInt16 Hit1Line, SInt16 Hit1Col, SInt16 Hit2Line, SInt16 Hit2Col, SInt16 Hits0AllCol, SInt16 Hits1AllCol, SInt16 Hits2AllCol );)

FHEAD ( SInt32 FSBB0__FMatDiscriPrintHit ( char* CmtStrTitle, SInt8 CmtSInt8MapsId, FSBB0__TMatDiscriBit* PtDest );) 



#ifndef APP_DLL_IMPORT_EXPORT
  #ifndef FSBB0_H
    #define FSBB0_H
	#endif
#endif


#endif
