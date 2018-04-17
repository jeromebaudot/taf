 
/*******************************************************************************
File      : x:\lib\win\eudet_frio\eudet_frio_fsbb0.h
Goal      : Fsbb0 functions prototypes of flex rio board library for EUDET
Prj date  : 01/10/2014
File date : 01/10/2014
Doc date  : 
Author    : Matthieu SPECHT
E-mail    : matthieu.specht@iphc.cnrs.fr
----------------------------------------------------------------------------------
License   : You are free to use this source files for your own development as long
          : as it stays in a public research context. You are not allowed to use it
          : for commercial purpose. You must put this header with laboratory and
          : authors names in all development based on this library.
----------------------------------------------------------------------------------
Labo      : IPHC */
/*******************************************************************************/


#ifndef EUDET_FRIO_FSBB0_H

#include "func_header.def"


// 06/11/2010 -> 21

// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, char* DIU_FGetVersion ();)
// FHEAD ( SInt32 REF_FHello ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSBB0_FMatDiscriPrintHit ( char* CmtStrTitle, SInt8 CmtSInt8MapsId, FSBB0__TMatDiscriBit* PtDest );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt8 EFRIO__FSBB0_FHammingDecoder8_4(UInt8 CodedIn);)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSBB0_FConvEfrioFrameToZsFFrame ( EFRIO__TFrame* Src, SInt8 MapsId, FSBB0__TZsFFrame* Dest, SInt8 PrintLvl );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSBB0_FConvZsFFrameToMatDiscriBit ( FSBB0__TZsFFrame* PtSrc, FSBB0__TMatDiscriBit* PtDest, SInt32* PtOvfCnt, SInt8 PrintLvl );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSBB0_FConvNI6562ToFlexRIOEudet2Mode ( SInt32 BoardId, void* PtFrmRaw, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode );)  
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSBB0_FConvNi6562ToFlexRIOEudet3Mode ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode );)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FSBB0_FFRioAcqDeserDataFsbb0 ( SInt8 Fsbb0Nb, SInt32 BoardId, UInt32* PtSrcW32AsPt, UInt32 PtSrcW32AsInt, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, UInt32 WaitMsAtEnd, SInt8 DataConvertMode, SInt8 TriggerHandlingMode, SInt16 EmuleMode );)

#ifndef APP_DLL_IMPORT_EXPORT
  #ifndef EUDET_FRIO_FSBB0_H
    #define EUDET_FRIO_FSBB0_H
  #endif
#endif


#endif
