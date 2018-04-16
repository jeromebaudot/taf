/* 24/02/05 */


#ifndef FIL_H

#include "func_header.def"

#include "files.def"
#include "files.typ"


FHEAD ( SInt32 FIL_FFileExists ( char* FilePath );)
FHEAD ( SInt32  FIL_FFileSize   ( char* FilePath );)

FHEAD ( SInt32 FIL_FCpyFile ( char* Src, char* Dest );)
FHEAD ( SInt32 FIL_FRemoveFile ( char* FilePath );)
FHEAD ( SInt32 FIL_FDirOutFile ( char* SrcDir, char* DestDirFile );)
FHEAD ( SInt32 FIL_FRmDirFiles ( char* SrcDir );)
FHEAD ( SInt32 FIL_FRmDir ( char* SrcDir );)
FHEAD ( SInt32 FIL_FDelDir ( char* SrcDir );)
FHEAD ( SInt32 FIL_FMkDir ( char* SrcDir );)
FHEAD ( SInt32 FIl_FListDirFiles ( char* SrcDir, FIL_TDirFilesList* PtList );)
FHEAD ( SInt32 FIL_FCntRmpFiles ( char* RmpDataPath, FIL_TDirFilesList* PtList );)
FHEAD ( SInt32 FIL_FWriteRecord ( char* FileName, void* PtSrc, SInt32 RecSz );)
FHEAD ( SInt32 FIL_FReadRecord ( char* FileName, void* PtDest, SInt32 RecSz );)

// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 FIL_FWriteRecord ( char* FileName, void* PtSrc, SInt32 RecSz );)
// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 FIL_FReadRecord ( char* FileName, void* PtDest, SInt32 RecSz );)


// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, ;)

// SInt32 FIL_FIndexFileDutCreate  ( char* FilePath );
// SInt32 FIL_FIndexFileDutAddEv   ( UInt32 EvId, UInt32 EvTag, UInt32 TrigLine, UInt32 TrigFrame, UInt32 TrigNbTot, UInt32 TrigNbAccepted, UInt32 SpareU32 );
// SInt32 FIL_FIndexFileDutRead    ( char* FilePath, UInt32* PtDest, SInt32 DestSz );

// SInt32 FIL_FIndexFileTelCreate  ( char* FilePath );
// SInt32 FIL_FIndexFileTelAddEv   ( UInt32 EvId, UInt32 AcqId, UInt32 FrId, UInt32 EvTag, UInt16 Trig0, UInt16 Trig1, UInt16 Trig2, UInt16 FrTrigNb, UInt32 SpareU32 );
// SInt32 FIL_FIndexFileTelRead    ( char* FilePath, UInt32* PtDest, SInt32 DestSz );


#ifdef CC_APP_WINDOWS
  FHEAD ( SInt32 FIL_FDosDirOutFile ( char* SrcDir, char* DestDirFile );)
  FHEAD ( SInt32 FIL_FDosRmDirFiles ( char* SrcDir );)
  FHEAD ( SInt32 FIl_FDosListDirFiles ( char* SrcDir, FIL_TDirFilesList* PtList );)
#endif


#ifndef APP_DAQ
  FHEAD ( SInt32 FIL_FDelRmpFiles ( char* RmpDataPath, SInt32 RunNo, SInt32 MaxFileCnt );)
#endif


#ifndef APP_DLL_IMPORT_EXPORT
  #ifndef FIL_H
    #define FIL_H
  #endif
#endif


#endif

