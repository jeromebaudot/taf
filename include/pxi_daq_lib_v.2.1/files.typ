/* 24/02/05 */


#ifndef FIL_TYP
#define FIL_TYP

/* char FIL_TADirFilesList [FIL_DIR_FILES_LIST_MAX_CNT][GLB_FILE_PATH_SZ]; */


typedef struct {
  char AList [FIL_DIR_FILES_LIST_MAX_CNT][GLB_FILE_PATH_SZ];
} FIL_TDirFilesList;


// 30/01/2009

class FIL__TCBinFile {
  
  
  private :

  protected :
  
    SInt8 ProConfDone; // -1 => not done / +1 => done / 0 IS NOT allowed
    
    // Parameters from constructor
    
    char  ProParErrLogFile[GLB_FILE_PATH_SZ];
    SInt8 ProParEnableErrLog;
    SInt8 ProParErrLogLvl;
  
    // Parameters from conf
    
    char   ProParDataFile[GLB_FILE_PATH_SZ];
    SInt8  ProParRWBMode;
    SInt32 ProParMaxBlocSz;
    SInt32 ProParBlocSz;
    SInt8  ProParFlushAfterWrite;
    SInt8  ProParMeasTime;
    
    // Variables for internal processing

    SInt8  ProReadyToWrite; // -1 no / +1 yes / 0 IS NOT allowed
    SInt8  ProReadyToRead;  // -1 no / +1 yes / 0 IS NOT allowed
    
    SInt32 ProCurRdEltId;
    SInt32 ProCurRdSz;
    
    SInt32 ProCurWrEltId;
    UInt64 ProCurWrSz;      // Moved from SInt32 to UInt64 on 08/11/2010
    UInt64 ProTotWrSz;      // Moved from SInt32 to UInt64 on 08/11/2010
    
    void*  ProPtrBuffRdData;
    SInt32 ProSzBuffRdData;
    
    FILE*  ProPtFile;
    
    UInt32 ProTime1;
    UInt32 ProTime2;
    UInt32 ProTimeExec;
    
  
  
  public :
  
     FIL__TCBinFile ( char* ErrLogFile, SInt8 EnableErrLog, SInt8 ErrLogLvl );
    ~FIL__TCBinFile ();
    
    SInt32 PubFBegin ( char* ErrLogFile, SInt8 EnableErrLog, SInt8 ErrLogLvl );
    SInt32 PubFConf  ( char* DataFile, SInt8 RWBMode, SInt32 MaxBlocSz, SInt32 BlocSz, SInt8 FlushAfterWrite, SInt8 MeasTime );
    
    SInt32 PubFSetFileName  ( char* DataFile );
    SInt32 PubFSetFlushMode ( SInt8 FlushAfterWrite );
    
    SInt32 PubFGetFileSz  ();
    SInt32 PubFGetBlocNb  ();
    
    
    SInt32 PubFCreate ();
    SInt32 PubFOpen ();
    SInt32 PubFSeqWrite  ( void* PtrData, SInt32 DataSz );
    SInt32 PubFSeqRead   ( void* DestPtr, SInt32 MaxDestSz, SInt32 DataSzToRead );
    void*  PubFSeqRead   ( SInt32 DataSzToRead );
    SInt32 PubFGotoBloc  ( SInt32 BlocNo );
    SInt32 PubFBlocRead  ( SInt32 BlocNo, void* DestPtr, SInt32 MaxDestSz );
    void*  PubFBlocRead  ( SInt32 BlocNo );
    SInt32 PubFFlush ();
    SInt32 PubFClose ();
  
};


// 01/05/2010

typedef struct {
  
  SInt32        Version;
  TIME__TUDateL DateCreate;
  TIME__TUTime  TimeCreate;
  TIME__TUDateL DateClose;
  TIME__TUTime  TimeClose;
  SInt32        FixedBlocSz;
  UInt32        MaxBlocSz;
  UInt32        BlocSz;
  UInt32        BlocNb;
  UInt32        ABlocSz[1];
  
} FIL__TCStreamFile_Old_TRecInfFile;

// 05/11/10

typedef struct {
  
  UInt64 Offset;
  
  // UInt32 Sz;
  
  SInt32 Sz; // NEW 12/01/2012
  
  SInt32 SpareW32InfoFormat; // Number which identifies the meaning ASpareW32Info
  SInt32 SpareW32InfoNb;     // Number of spare parameters
  SInt32 ASpareW32Info[FIL__TCStreamFile_BLOC_INF_MAX_SPARE_W32_INFO_NB]; // Array for user parameters

  UInt32 Dummy; // For alignment
  
} FIL__TCStreamFile_TBlocInf;


// 05/11/10

typedef struct {

  
  SInt32        Version;
  TIME__TUDateL DateCreate;
  TIME__TUTime  TimeCreate;
  TIME__TUDateL DateClose;
  TIME__TUTime  TimeClose;
  SInt32        FixedBlocSz;
  UInt32        MaxBlocSz;
  UInt32        BlocSz;
  UInt32        BlocNb;

  UInt32 Dummy; // For alignment 

  FIL__TCStreamFile_TBlocInf ABlocInf[1];
  
  
} FIL__TCStreamFile_TRecInfFile;


class FIL__TCStreamFile {
  
  
  private :
  
    FIL__TCStreamFile* PriPtMyself;
    
  protected :
  
    SInt8 ProConfDone; // -1 => not done / +1 => done / 0 IS NOT allowed
    
    // Parameters from constructor
    
    char   ProParErrLogFile[GLB_FILE_PATH_SZ];
    SInt8  ProParEnableErrLog;
    SInt8  ProParErrLogLvl;
    SInt32 ProParDiskBlocSz;
    SInt8  ProParFixedBlocSzMode;
    
    // Parameters from conf
    
    SInt8  ProUseThread;
    char   ProParDataFile[GLB_FILE_PATH_SZ];
    SInt8  ProParRWBMode;
    SInt32 ProParRequestedMaxBlocSz; // Bloc size value asked by user
    SInt32 ProParMaxBlocSz;          // Adjusted depending on ProParBlocSz
    
    SInt32 ProParRequestedBlocSz;   // Bloc size value asked by user
    SInt32 ProParBlocSz;            // Bloc size value used ( multiple of disk bloc size ) 
    SInt8  ProParFlushAfterWrite;
    SInt8  ProParMeasTime;
    
    SInt32 ProAParBlocSz[FIL__TCStreamFile_MAX_BUFF_NB]; // 11/01/2012 NEW DBG
    
    // Variables for internal processing

    SInt8  ProFileHasBeenClosed; // 13/01/2012

    HANDLE ProFileHnd;
    
#ifdef CC_UNIX
    FILE* ProFilePtUx; // Pointer to data file for Linux / Unix
#endif

                                                // 12/01/2012 - Now two index table are used
    char   ProInfFileName[GLB_FILE_PATH_SZ];    // Store updated index table file = table with non saved blocs info removed
    char   ProInfFileNameAll[GLB_FILE_PATH_SZ]; // Store default index table file = table with ALL blocs info = included non saved
    FILE*  ProPtInfFile;
    FILE*  ProPtInfFileAll;
    
    FIL__TCStreamFile_TRecInfFile* ProPtRecInfFile;
    FIL__TCStreamFile_TRecInfFile* ProPtRecInfFileCpy; // NEW 12/01/2012 copy of RecInfFile record used to update table = to remove blocs on which saved has failed
    SInt32                         ProRecInfSz;
    
    SInt8  ProReadyToWrite; // -1 no / +1 yes / 0 IS NOT allowed
    SInt8  ProReadyToRead;  // -1 no / +1 yes / 0 IS NOT allowed
    
#ifndef CC_UNIX

  #ifndef FILE__NO_CRITICAL_SECTION_HANDLING
    CRITICAL_SECTION ProCsPrintMsg;
  #endif

#endif

    HANDLE ProThreadHnd;
    DWORD  ProThreadId;
  
    HANDLE ProSemWrBuffHnd;
    HANDLE ProSemRdBuffHnd;
    
    SInt16 ProIndexWrBuff;
    SInt16 ProIndexRdBuff;
    
    UInt8*  ProABuff[FIL__TCStreamFile_MAX_BUFF_NB];
    SInt32  ProBuffSz;
    
    SInt32* ProAPtBlocSz[FIL__TCStreamFile_MAX_BUFF_NB]; // Pointer to the "sz" field of FIL__TCStreamFile_TBlocInf
                                                         // PubFSeqWrite fill the field of FIL__TCStreamFile_TBlocInf with size of block
                                                         // If writing to disk in thread functions failed, a "-" is added to this size
                                                         // This will be used by the function which updates the index file table in order
                                                         // to skip blocs not saved and ajust offset list 
    
    
    SInt32 ProCurRdBlocId;
    SInt32 ProCurRdSz;
    
    SInt32 ProCurWrBlocId;
    SInt32 ProCurWrSz;
    SInt64 ProTotWrSz;
    
    void*  ProPtrBuffRdData;
    SInt32 ProSzBuffRdData;
      
    UInt32 ProTime1;
    UInt32 ProTime2;
    UInt32 ProTimeExec;
    
    // Results
    
    SInt32 ProResWrBlocFailCnt;
    
    SInt32 ProFCalcProParBlocSz ( SInt32 DataSz );
  
    SInt32 ProFUpdateIndexTable ();
  
  public :
  
    FIL__TCStreamFile ( char* ErrLogFile, SInt8 EnableErrLog, SInt8 ErrLogLvl, SInt32 DiskBlocSz );
    ~FIL__TCStreamFile ();
    
    friend DWORD WINAPI FIL__TCStreamFile_FThread (  LPVOID lpParam );
    
    SInt32 PubFBegin    ( char* ErrLogFile, SInt8 EnableErrLog, SInt8 ErrLogLvl, SInt32 DiskBlocSz );
    SInt32 PubFSetDiskSectorSz ( SInt32 DiskBlocSz );
    SInt32 PubFGetDiskSectorSz ();
    
    void   PubFPrintMsg ( char* Msg, SInt8 MsgLevel );
    SInt32 PubFConf     ( FIL__TCStreamFile* PtMyself, SInt8 UseThread, char* DataFile, SInt8 RWBMode, SInt8 FixedBlocSzMode, SInt32 MaxBlocSz, SInt32 BlocSz, SInt8 FlushAfterWrite, SInt8 MeasTime );
    
    SInt32 PubFSetFileName  ( char* DataFile );
    SInt32 PubFSetFlushMode ( SInt8 FlushAfterWrite );
    
    SInt32 PubFPrintInfFileRec ( FIL__TCStreamFile_TRecInfFile* Pt, char* Cmt, char* TagStr );
    SInt32 PubFPrintInfFile ();
    SInt32 PubFGetFileSz  ();
    SInt32 PubFGetBlocNb  ();
    
    
    SInt32 PubFCreate ();
    SInt32 PubFOpen ();
    
    // SInt32 PubFSeqWrite  ( void* PtrData, SInt32 DataSz, SInt32 SpareInfo );
    
    SInt32 PubFSeqWrite ( void* PtrData, SInt32 DataSz, SInt32 DbgCallPar, SInt32 SpareW32InfoFormat, SInt32* PtSpareW32Info, SInt16 SpareW32InfoNb );
    
    SInt32 PubFSeqRead   ( void* DestPtr, SInt32 MaxDestSz, SInt32 DataSzToRead );
    void*  PubFSeqRead   ( SInt32 DataSzToRead );
    SInt32 PubFGotoBloc  ( SInt32 BlocNo );
    SInt32 PubFBlocRead  ( SInt32 BlocNo, void* DestPtr, SInt32 MaxDestSz, SInt32* PtSpareW32InfoFormat, SInt16 MaxSpareW32InfoNb, SInt32* PtSpareW32Info );
    void*  PubFBlocRead  ( SInt32 BlocNo );
    SInt32 PubFFlush ();
    SInt32 PubFClose ();
    
};



// 09/07/2012

typedef struct {
  
  char   FilePath[GLB_FILE_PATH_SZ];
  FILE*  PfDest;
  FILE*  PfDestCpy;
  
  SInt32 EltCnt;
    
  
} FIL__TIndexFileDut;


// 09/07/2012

typedef struct {
  
  char   FilePath[GLB_FILE_PATH_SZ];
  FILE*  PfDest;
  FILE*  PfDestCpy;
  
  SInt32 EltCnt;
  
  
} FIL__TIndexFileTel;


// 24/07/2012

typedef struct {
  
  SInt32 EventNo;
  SInt32 TrigLine;  
  
} FIL__TMi32ABTIneffEvRec;



#endif


