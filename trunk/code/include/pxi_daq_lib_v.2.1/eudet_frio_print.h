 
/*******************************************************************************
File      : x:\lib\win\eudet_frio\eudet_frio.h
Goal      : Functions prototypes of flex rio board library for EUDET Mimosa 26 DAQ
Prj date  : 05/08/2010
File date : 05/08/2010
Doc date  : 05/08/2010
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


#ifndef EUDET_FRIO_PRINT_H

#include "func_header.def"


// 06/11/2010 -> 11

// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, char* DIU_FGetVersion ();)
// FHEAD ( SInt32 REF_FHello ();)


FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FPrintRunContRec ( EFRIO__TRunCont* PtRec );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FPrintRunCont ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FPrintBoardConfRec ( EFRIO__TBoardConf* PtRec );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FPrintBoardConf ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FPrintBoardStatusRec ( EFRIO__TBoardStatus* PtRec );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FPrintBoardStatus ( SInt32 BoardId );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FPrintAcqEmulRec ( EFRIO__TAcqEmul* PtRec );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FPrintAcqEmul ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FPrintFrCheckRec ( EFRIO__TFrCheck* PtRec );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FPrintFrCheck ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FPrintFrameRec ( EFRIO__TFrame* PtRec, SInt8 PrintLevel );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FPrintFrameRecFsbb0 ( EFRIO__TFrame* PtRec, SInt8 PrintLevel );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FPrintMonContRec ( EFRIO__TMon* PtRec );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FPrintMonCont ();)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FPrintMsg ( SInt32 DummyS32In, SInt8 PrintAsMsg, SInt8 PrintAsTrace, SInt8 PrintAsWarning, SInt8 PrintAsError, char* Msg );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 EFRIO__FPrintMsgWithTimeStamp ( SInt32 DummyS32In, SInt8 PrintAsMsg, SInt8 PrintAsTrace, SInt8 PrintAsWarning, SInt8 PrintAsError, char* Msg );)




#ifndef APP_DLL_IMPORT_EXPORT
  #ifndef EUDET_FRIO_PRINT_H
    #define EUDET_FRIO_PRINT_H
    #endif
#endif


#endif