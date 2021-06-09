 
/*******************************************************************************
File      : x:\lib\win\eudet_frio\eudet_frio_ult1.h
Goal      : Ultimate 1 functions prototypes of flex rio board library for EUDET
Prj date  : 11/05/2011
File date : 11/05/2011
Doc date  : 
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


#ifndef EUDET_FRIO_ULT1_H

#include "func_header.def"


// 06/11/2010 -> 21

// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, char* DIU_FGetVersion ();)
// FHEAD ( SInt32 REF_FHello ();)

  
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__ULT1_FTestOnDataGetJtagRef ();)
  
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__ULT1_FFRioAcqDeserDataIphcMode1Ult1   ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__ULT1_FFRioAcqDeserDataIphcMode6Ult1   ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__ULT1_FFRioAcqDeserDataEudet1Mode1Ult1 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__ULT1_FFRioAcqDeserDataEudet1Mode6Ult1 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__ULT1_FFRioAcqDeserDataEudet2Mode1Ult1 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__ULT1_FFRioAcqDeserDataEudet2Mode6Ult1 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__ULT1_FFRioAcqDeserDataEudet3Mode6Ult1 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__ULT1_FFRioAcqDeserDataEudet3Mode1Ult1 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__ULT1_FFRioAcqDeserDataUlt1            ( SInt8 Mi26Nb, SInt32 BoardId, UInt32* PtSrcW32AsPt, UInt32 PtSrcW32AsInt, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, UInt32 WaitMsAtEnd, SInt8 DataConvertMode, SInt8 TriggerHandlingMode, SInt16 EmuleMode, UInt32 DataTestTotErrCnt );)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__ULT1_FJtagLoadFile ( char* FileName );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__ULT1_FJtagLoadDefFile ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__ULT1_FJtagReset ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__ULT1_FJtagLoadChip ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__ULT1_FJtagStartChip ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__ULT1_FHwStartChip ( SInt32 SpareS32Par );)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__ULT1_FConvTFrameToZsFFrame ( EFRIO__TFrame* PtRec, ULT1__TZsFFrame* APtDest[EFRIO__MAX_ASIC_NB], SInt8 Mi28DestBuffersNb, SInt8 PrintLevel );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__ULT1_FCheckTFrame ( EFRIO__TFrame* PtRec, SInt8 EmulErr );)

#ifndef APP_DLL_IMPORT_EXPORT
  #ifndef EUDET_FRIO_ULT1_H
    #define EUDET_FRIO_ULT1_H
  #endif
#endif


#endif
