
/*******************************************************************************
File      : x:\lib\win\eudet_frio\eudet_frio.typ
Goal      : Types definition of flex rio board library for EUDET Mimosa 26 DAQ
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


#ifndef EUDET_FRIO_TYP
#define EUDET_FRIO_TYP



/* ============================================== */
/* Board parameters configuration record          */
/* ---------------------------------------------- */
/* Can be use to build a boards database          */ 
/* ---------------------------------------------- */
/* Date      : 05/08/2010                         */
/* Doc date  : 06/11/2010                         */
/* Author    : Gilles CLAUS                       */
/* E-mail    : gilles.claus@ires.in2p3.fr         */
/* Labo      : DRS - IPHC                         */
/* ============================================== */

// Remark for EUDET concerning trigger handling
//
// Most of the following trigger options will be useless for EUDET
// The operating mode for EUDET is : 
// - The board takes data all the time regardless of trigger state
// - The fw store all triggers from TLU ( up to 288 / Mimsoa 26 frame ) are stored
// - The sw extract frames with trigger + EFRIO__FRAME_NB_TO_READ_AFTER_TRIG frames after trigger

typedef struct {
  
  SInt32 BoardId;                   // The board identifier = a number associated to each board 0,1 ...
  
  char   AsicName[GLB_CMT_SZ];      // The ASIC read by the board
  
  SInt32 AsicNb;                    // The number of ASICs read by the board
  
  SInt32 ReadoutMode;               // The readout mode -> Future use
  
  SInt8  EmuleChannels;             // Copy data of first MAPS in all channels, useful for test & debugging
                                    // because it allows to run with one ASIC only but get data of all
                                  
  float  DataClkFrequency;          // Frequency of clock -> Future use, only useful in case board provide clock
  
  UInt32 DmaHostSz;                 // DMA size reserved on host CPU, must be >= size of one acquisition 
  
  SInt32 FrameNbPerAcq;             // Consecutives frames number stored during one acquisition
  
  SInt8  EnableExtraChannel;        // Enable one more channel ( default is one channel per ASIC )
                                    // which can be used to store extra information -> eg : TLU trigger
  
  SInt32 AcqNbPerTrig;              // TO BE CHECKED ! Number of consecutive acquisitions taken upon trigger detection by board 

  SInt8  TriggerMode;               // Trigger operating mode
  
  UInt32 TriggerDetectTimeWindow;   // Time window during which we count triggers and decide to start acquisition if >= TriggerDetectOccurNb
  
  UInt32 TriggerDetectOccurNb;      // Minimum trigger number during TriggerDetectTimeWindow to decide to start acquisition
  
  UInt32 TimeStampRes;              // Resolution of time stamp
  
  SInt8  EnableTimeStamping;        // Enable time stamping mode
  
  SInt8  EnableTrigCnt;             // Enable trigger counter 
  
  SInt8  TagEventsStoredByDUT;      // Tag in Flex RIO data stream the events stored by DUT DAQ ( HW line indicates that DUT has saved event )
  
  UInt32 ReadTluTrigCntEachNTrig;   // Period of reading  TLU trigger


} EFRIO__TBoardConf;


/* ============================================== */
/* Board status record                            */
/* ---------------------------------------------- */
/* Date      : 05/08/2010                         */
/* Doc date  : 06/11/2010                         */
/* Author    : Gilles CLAUS                       */
/* E-mail    : gilles.claus@ires.in2p3.fr         */
/* Labo      : DRS - IPHC                         */
/* ============================================== */

typedef struct {
  
  SInt32 BoardId;                    // The board identifier = a number associated to each board 0,1 ...
  SInt8  BoardPresent;               // Board present => 1, otherwise 0 
  SInt8  FwLoaded;                   // Firmware has been loaded in board
  SInt8  ConfDone;                   // The board has been configured by sw
  
  SInt32 StatusCode;                 // Current board status code
  
  char   StatusStr[GLB_CMT_SZ];      // Status message associated to StatusCode

                                     // List of errors and last error

  char*  ErrorMsgList[EFRIO__ERROR_MSG_LIST_MAX_NB];
  char   LastErrorMsg[GLB_CMT_SZ];
    
  // Registers read from board

  UInt32 RegDmaHostSz;               // DMA host size in bytes
  SInt32 RegFrameNbPerAcq;           // Number of frames / acq
  SInt8  RegEnableExtraChannel;      // Extral channel state = enabled or not
  SInt32 RegAcqNbPerTrig;            // Consecutive acquisition nb per trigger
  
  SInt8  RegTriggerMode;             // Trigger mode
  UInt32 RegTriggerDetectTimeWindow; // Trigger detection window
  UInt32 RegTriggerDetectOccurNb;    // Number of triggers required during window to start acquisition
  
  UInt32 RegTimeStampRes;            // Resolution of time stamp                                   -> Check with CS if implemented !
  SInt8  RegEnableTimeStamping;      // Enable time stamping mode                                  -> Check with CS if implemented !
  
  SInt8  RegEnableTrigCnt;           // Enable trigger counter -> Check with CS if implemented !
  
  SInt8  RegTagEventsStoredByDUT;    // Tag in Flex RIO data stream the events stored by DUT DAQ   -> Check with CS if implemented !
  UInt32 RegReadTluTrigCntEachNTrig; // Period of reading  TLU trigger                             -> Check with CS if implemented !
  
  UInt64 RegTimeStamp;               // Time stamp value
  UInt32 RegTrigCnt;                 // Flex RIO rigger value
  UInt32 RegTluTrigCnt;              // TLU trigger value
 
} EFRIO__TBoardStatus;




/* ============================================== */
/* TLU trigger record                             */
/* ---------------------------------------------- */
/* Contains TLU trigger -> field TrigCnt          */
/* plus additional information                    */
/* ---------------------------------------------- */
/* Date      : 25/10/2010                         */
/* Doc date  : 06/11/2010                         */
/* Author    : Gilles CLAUS                       */
/* E-mail    : gilles.claus@ires.in2p3.fr         */
/* Labo      : DRS - IPHC                         */
/* ============================================== */


typedef union {
  
  UInt32 W32;
  
  struct {
    
    UInt32 TrigCnt         : 16; // Trigger counter read from TLU
    UInt32 FrameIdInAcq    : 11; // Index of frame in current acquisition during which trigger occurs ( 0 - 2407 )
    UInt32 EventTakenByDut :  1; // For future use : Flag at 1 if DUT has taken the event
    UInt32 Reserved        :  3;
    UInt32 InvalidInfo     :  1; // If 1 this field is not valid
    
  } F;
  
} EFRIO__TTluTrigger;


/* ============================================== */
/* Flex RIO time stamp 1 record                   */
/* ---------------------------------------------- */
/* This is the Flex RIO trigger, called           */
/* "time stamp" to avoid confusion / TLU          */
/* ---------------------------------------------- */
/* Date      : 25/10/2010                         */
/* Doc date  : 06/11/2010                         */
/* Author    : Gilles CLAUS                       */
/* E-mail    : gilles.claus@ires.in2p3.fr         */
/* Labo      : DRS - IPHC                         */
/* ============================================== */


typedef union {
  
  UInt32 W32;
  
  struct {
    
    UInt32 Mi26Line    : 10; // Line of Mi26 read during which trigger occurs
    UInt32 Mi26Frame   : 21; // Frame of Mi26 ( = frame counter field ) read during which trigger occurs
    UInt32 InvalidInfo :  1; // If 1 this field is not valid
    
  } F;
  
} EFRIO__TFlexRioTimeStamp1;


/* ============================================== */
/* Frame header                                   */
/* ---------------------------------------------- */
/* Each frame starts with a header which contains */
/* - DAQ system info                              */
/* - Mimosa 26 relevant fields                    */
/* ---------------------------------------------- */
/* Date      : 22/10/2010                         */
/* Rev       : 23/02/2011                         */
/*           : - Add fields AcqStatus, TrigStatus */
/* Doc date  : 06/11/2010                         */
/* Author    : Gilles CLAUS                       */
/* E-mail    : gilles.claus@ires.in2p3.fr         */
/* Labo      : DRS - IPHC                         */
/* ============================================== */

typedef struct {

#ifdef EFRIO__FRAME_TAGS_ENABLE
  UInt32 Tag;                                    // EFRIO__FRAME_TAG_HEADER
#endif

                                                 // New fields AcqStatus & TrigStatus 23/02/2011
                                                 // 
  SInt32 AcqStatus;                              // Status of acquistion board for this acquisition
                                                 // 0   = OK                                                  
                                                 // > 0 = frame nb lost
                                                 // < 0 = hardware error
                                                 // 
  SInt32 TrigStatus;                             // No meaning now = reserved for future use    

  UInt16 AcqId;                                  // Index of acquisition containing this frame
  UInt16 FrameIdInAcq;                           // Index of frame IN the CURRENT acquisition

  UInt16 MapsName;                               // MAPS name as a 16 bits code
  UInt16 MapsNb;                                 // Total number of MAPS in data
  
  UInt32 AMapsHeader[EFRIO__MAX_ASIC_NB];        // Mimosa 26 header field
  UInt32 AMapsFrameCnt[EFRIO__MAX_ASIC_NB];      // Mimosa 26 frame counter field
  UInt16 AMapsDataLength[EFRIO__MAX_ASIC_NB];    // Mimosa 26 data length in BYTES -> It's final result NOT the DataLength FIELD from data stream
  UInt32 AMapsTrailer[EFRIO__MAX_ASIC_NB];       // Mimosa 26 trailer field
  
  SInt16 TriggerNb;                              // Total triggers number during this frame
  
  UInt16 AMapsTrigInfo[EFRIO__MAX_TRIGGER_NB_STORED_IN_FRAME_DATA];   // First 3 "Mi26 trigger info" -> Line of Mi26 read during which trigger occurs
                                                                      // if more than 3 trigger => look in trigger info block where all trigger are stored
  
} EFRIO__TFrameHeader;


/* ============================================== */
/* Frame data                                     */
/* ---------------------------------------------- */
/* Each frame has a data part with variable size  */
/* ---------------------------------------------- */
/* Date      : 25/10/2010                         */
/* Doc date  : 06/11/2010                         */
/* Author    : Gilles CLAUS                       */
/* E-mail    : gilles.claus@ires.in2p3.fr         */
/* Labo      : DRS - IPHC                         */
/* ============================================== */

typedef struct {
    
#ifdef EFRIO__FRAME_TAGS_ENABLE
  UInt32 Tag;                     // EFRIO__FRAME_TAG_DATA
#endif
  UInt32 TotSz;                   // Total size of data bloc
  UInt32 OneMapsSz;               // Size of data of one MAPS
  
  UInt32 ADataW32[0];             // Beginning of data space
  
} EFRIO__TFrameData;


/* ============================================== */
/* Frame triggers list                             */
/* ---------------------------------------------- */
/* Each frame has a triggers list, up to          */
/* EFRIO__EXTRA_CHAN__MAX_TRIGGER_FIELD_NB fields */
/* which means up to                              */
/* EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB         */
/* trigger info                                   */
/* ---------------------------------------------- */
/* Date      : 25/10/2010                         */
/* Doc date  : 07/11/2010                         */
/* Author    : Gilles CLAUS                       */
/* E-mail    : gilles.claus@ires.in2p3.fr         */
/* Labo      : DRS - IPHC                         */
/* ============================================== */

typedef struct {

#ifdef EFRIO__FRAME_TAGS_ENABLE
  UInt32 Tag;          // EFRIO__FRAME_TAG_TRIG
#endif
  UInt32 TotSz;        // Total size of trigger info bloc
  UInt16 TrigNb;       // Total trigger nb
  UInt16 TrigType;     // Type of trigger info stored
  
  UInt32 ATrig[0];     // Beginning off triggers list
    
} EFRIO__TTriggerRec; 


/* ============================================== */
/* Frame record                                   */
/* ---------------------------------------------- */
/* Contains :                                     */
/* - Data handling fields ( size etc )            */
/* - The frame header                             */
/* - The frame data part ( variable length )      */
/* - Followed by the triggers info part           */
/* ---------------------------------------------- */
/* Date      : 25/10/2010                         */
/* Rev       : 21/02/2011                         */
/*           : - Add new field DaqVersion         */
/*           :                                    */
/* Doc date  : 07/11/2010                         */
/* Author    : Gilles CLAUS                       */
/* E-mail    : gilles.claus@ires.in2p3.fr         */
/* Labo      : DRS - IPHC                         */
/* ============================================== */

typedef struct {

#ifdef EFRIO__FRAME_TAGS_ENABLE
  UInt32              Tag;            // EFRIO__FRAME_TAG
#endif
  SInt32              DaqVersion;     // Version of DAQ - New field 21/02/2011
  SInt32              TotSz;          // Total size of this frame
  SInt32              TrigRecOffset;  // Offset ( in bytes ) from beginning of frame to trigger info part
  EFRIO__TFrameHeader Header;         // Frame header
  EFRIO__TFrameData   Data;           // Beginning of data part
  
  // The field EFRIO__TTriggerInfo is not defined here because "Data" has variable length
  // the field BegData of Data field is used a pointer to beginning of data part
  // The trigger info will be added at the end of this part but position is calculated dynamically
  
} EFRIO__TFrame;


/* ============================================== */
/* List of frames currently stored by lib         */
/* ---------------------------------------------- */
/* This record stores frames list corresponding   */
/* to one acquisition                             */
/*                                                */
/* It can be :                                    */
/* - frames acquired by DAQ                       */
/* - frames loaded from a run file                */
/* ---------------------------------------------- */
/* Date      : 25/10/2010                         */
/* Rev       :                                    */
/*           : 24/02/2011                         */
/*           : - Add fields AcqStatus, TrigStatus */
/*           :                                    */
/* Doc date  : 07/11/2010                         */
/* Author    : Gilles CLAUS                       */
/* E-mail    : gilles.claus@ires.in2p3.fr         */
/* Labo      : DRS - IPHC                         */
/* ============================================== */

// More information about frames storage in memory
//
// A buffer IS NOT allocated for EACH frame, a single bloc of memory is allocated for all frames of one acquisition.
// This will avoid copy frame by frame before sending them to Ethernet lib or saving them to disk. A single access
// can be done with a pointer on beginning of buffer and data size.
//
// But to process frames it's handly to have an array of pointers, each array item pointing on next frame,
// this is the goal of this list which provides :
// - The total number of frames in list
// - An array of pointers, eg : AFramePtr[2] points on third frame of acquisition
// - An array with the total size ( in bytes ) of each frame
//
// This list is built on-line while DAQ is running, or off-line via EFRIO__FBuildFrameListFromAcq (...) from
// the data of one acquisition stored in a run file.

typedef struct {
  
  // New fields AcqStatus & TrigStatus 24/02/2011
  //
  
  SInt32 AcqStatus;   // Status of acquistion board for this acquisition
  
                      // 0   = OK
                      // > 0 = frame nb lost
                      // < 0 = hardware error
  
  SInt32 TrigStatus;  // No meaning now = reserved for future use
  
  
  SInt32 TotFrameNb;                                       // Total frames nb in list
  EFRIO__TFrame* AFramePtr[EFRIO__MAX_FRAME_NB_PER_ACQ];   // Array of pointers on each frame
  UInt32 AFrameSz[EFRIO__MAX_FRAME_NB_PER_ACQ];            // Array of frames size

} EFRIO__TFrameList;


/* ============================================== */
/* Run configuration record                       */
/* ---------------------------------------------- */
/* Contains :                                     */
/* - Run parameters                     -> Par... */
/* - Informations built from Par etc    -> Inf... */
/* - Acquisition results ( ev cnt etc ) -> Res... */
/* - Pointers to frames buffers                   */
/* ---------------------------------------------- */
/* This record is filled by EFRIO__FConfRun (...) */
/* call with run parameters from GUI or Ethernet  */
/* ---------------------------------------------- */
/* This record can be saved to disk as run config */
/* file, but it can't be ONLY loaded from file !  */
/* A call to EFRIO__FConfRun (...) must be done   */
/* because it allocates mem and do other tasks    */
/* -> load record from file                       */
/* -> call EFRIO__FConfRun with record fields as  */
/* parameters, it will overwrite itself and all   */
/* " other tasks " will also be done              */
/* ---------------------------------------------- */
/* Date      : 05/08/2010                         */
/* Rev       : 21/02/2011                         */
/*           : - Add new fields                   */
/*           :   ParDaqVersion, ParMapsName       */
/*           : - Change type of ParMi26Nb to S16  */
/*           :                                    */
/* Doc date  : 07/11/2010                         */
/* Author    : Gilles CLAUS                       */
/* E-mail    : gilles.claus@ires.in2p3.fr         */
/* Labo      : DRS - IPHC                         */
/* ============================================== */

typedef struct {
  
  SInt32 ParDaqVersion;                        // Version of DAQ system - New field 21/02/2011
  UInt16 ParMapsName;                          // Name of MAPS - New field 21/02/2011
  
  UInt16 ParMi26Nb;                            // Mimosa 26 number - Moved from SInt8 to UInt16 on 21/02/2011
  SInt32 ParFrameNbPerAcq;                     // Frames number per acquisition
  
  SInt32 ParRunNo;                             // Run no
  SInt32 ParTotEvNb;                           // Total event number of run
  SInt32 ParEvNbPerFile;                       // Event number per file
  char   ParDestDir[GLB_FILE_PATH_SZ];         // Run file destination directory       
  char   ParFileNamePrefix[GLB_FILE_PATH_SZ];  // Prefix of run file name, eg : RUN_666 => "RUN" is the prefix
  
  char   ParJtagFileName[GLB_FILE_PATH_SZ];    // JTAG configuration file (*.mcf) -> New field 03/02/2011
  
  SInt8  ParDataTransferMode;                  // Transfer mode see enum EFRIO__TRF_MODE in *.def file

  SInt8  ParTrigMode;                          // Trigger mode -> Future use
  SInt8  ParSaveOnDisk;                        // Save data on disk
  SInt8  ParSendOnEth;                         // Send data on Ethernet
  SInt8  ParSendOnEthPCent;                    // % of data sent on Ethernet

  SInt8  ParMeasDataRate;                      // Enable data rate measurement, hard coded in EFRIO__FConfRun (...)
  SInt8  ParAcqNbToMeasDataRate;               // Acq number used to measure data rate, hard coded in EFRIO__FConfRun (...)

  // SInt32 InfMi26FrameSzFromFlexRio;         // Not used now
  
  SInt32 InfZsFFrameRawBuffSz;                 // If data ParDataTransferMode = IPHC        => Size of acquisition frames buffer
  SInt32 InfFrameBuffSz;                       // If data ParDataTransferMode = EUDET 1,2,3 => Size of acquisition frames buffer
    
  char   InfConfFileName[GLB_FILE_PATH_SZ];    // Run configuration file ( save EFRIO__TRunCont to disk ) name built form ParRunNo, ParDestDir
  char   InfDataFileName[GLB_FILE_PATH_SZ];    // Run data file name built from ParRunNo, ParFileNamePrefix, ParDestDir
  
                                               // Variables to measure data rate -> average over ParAcqNbToMeasDataRate acquisitions
  SInt32 InfDataRateMeasTotalSz;               // Total size acquired during ParAcqNbToMeasDataRate acquisitions
  SInt32 InfDataRateMeasStartTimeMs;           // Start time of measurement
  SInt32 InfDataRateMeasStopTimeMs;            // Stop time of measurement
  SInt32 InfDataRateMeasTotalTimeMs;           // Total time of measurement
  
  SInt8  InfSaveDataOnDiskRunning;             // Add on 15/02/2011
                                               // Because for run ctrl via Eth, stop run cmd will be seen on DLL
                                               // side before Labview side, which may cause trouble because saving
                                               // function are called on both sides => a lock must be implemented 
                                               // 
                                               // Indicates that data are saved on disk
                                               // Set to 1 by EFRIO__FStartSavingOnFile () call
                                               // Set to 0 by EFRIO__FStopSavingOnFile () call
                                               // Tested by EFRIO__FSaveAcqOnFile () => exit if at 0
  
  SInt32  CmdRun;                              // Add on 21/12/2010 for interface / EUDET DAQ
                                               // Field used to control Labview application "acquisition engine" from DLL
                                               // Set to 1 to take data, to 0 to stop taking data by EFRIO_FSetCmdRun ( 0/1 )
                                               // State tested from Labview by a call to EFRIO_FGetCmdRun () 
  
  SInt32 ResAcqFunctRetCode;                   // Return code of Acq function
                                               // - < 0 => Acquisition error
                                               // - = 0 => No data available 
                                               // > 0 = Total acquisition size ( in bytes )
                                               //     = size of data bloc after data processing ( for example : extraction of frames with trigger )
                                               //       This is the USEFUL size of buffer pointer by EFRIO__VGContext.RunCont.PtFrame which contains all frames of one acquisition
                                                          
  
  SInt32 ResAcqCnt;                            // Acquisitions counter
  SInt32 ResFrameCnt;                          // Frames counter
  SInt32 ResEventCnt;                          // Events counter -> By default events counter = frames counter
                                               // but they may be different as more than one frame is needed to build a physics event

  float  ResDataRateMBytesPerSec;
  

  // Buffer for frames
  // Only one of the two is allocated depending on ParDataTransferMode = IPHC / EUDET
  
  MI26__TZsFFrameRaw* PtZsFFrameRaw;           // If data ParDataTransferMode = IPHC         => Acquisition frames buffer 
  EFRIO__TFrame* PtFrame;                      // If data ParDataTransferMode = EUDET 1,2,3  => Acquisition frames buffer
    
} EFRIO__TRunCont;


/* ============================================== */
/* Acquisition emulation record                   */
/* ---------------------------------------------- */
/* This record contains context of DAQ emulator   */
/* application, it means :                        */
/* - Parameters                       -> Par...   */
/* - Information, calculated from Par -> Inf...   */
/* - Results                          -> Res...   */
/* ---------------------------------------------- */
/* All emulation functions are implemented in the */
/* lib, therefore application task is only GUI.   */
/* That's why emulation context is defined here   */
/* ---------------------------------------------- */
/* All run param for emulation are taken from     */
/* run config record -> EFRIO__TRunCont           */
/* ---------------------------------------------- */
/* Date      : 30/10/2010                         */
/* Doc date  : 07/11/2010                         */
/* Author    : Gilles CLAUS                       */
/* E-mail    : gilles.claus@ires.in2p3.fr         */
/* Labo      : DRS - IPHC                         */
/* ============================================== */

typedef struct {
  
  SInt32  ParAcqCycleMs;                          // Delai between two acquisitions
  
  SInt32  ParEmuleDRamReadMs;                     // Delai added to PC DRAM access to emulate Flex RIO DRAM access time
  
  SInt32  ParEmuleFunctNo;                        // Select emulation function to call -> Future use = not implemented now
  
  SInt8   ParRandomDataSz;                        // Enables random generation of data size per Mimosa 26
                                                  // By default data size is fixed in emulation function
                                                  // Used to check if variabl length records are properly handled
  
  SInt8   ParSetMaxDataSzOnOneMaps;               // Set maximum possible data sze on first Mi26, overwrite value set by emulation
                                                  // function, but next Mi26 keep the data size value from emulation function
                                                  // Used to check if DAQ loose frames while Mi26 provides full frames
  
  
  UInt32  ParAHeader[EFRIO__MAX_ASIC_NB];         // Emulated header of each Mi26
  
  UInt32  ParATrailer[EFRIO__MAX_ASIC_NB];        // Emulated trailer of each Mi26

  SInt32  ParTrigNbPerFrame;                      // Number of trigger per frame, set the part trigger nb (B31B16) of Mi26 Zero1 field
  
                                                  // In data transfer modes EUDET 2 & 3 a more complex trigger emulation is done
                                                  // We don't emulate ParTrigNbPerFrame on each frame but on N consecutives frames
                                                  // each M frames
                                                  // 
  SInt32  ParTrigOnOneFrameOverN;                 // Start emulate ParTrigNbPerFrame on one frame over M = ParTrigOnOneFrameOverN 
  SInt32  ParTrigOnNConsecutiveFrames;            // Emulates on N consecutive frames = ParTrigOnNConsecutiveFrames
  
                                                  // TLU trigger & Flex RIO trigger emulation
                                                  // Up to 288 couples TLU & Flex RIO triggers can be emulated but only EFRIO__MAX_EMUL_GUI_TRIG_NB
                                                  // are configurabbles from GUI, now EFRIO__MAX_EMUL_GUI_TRIG_NB = 4
                                                  // - First three are configurable from GUI 
                                                  // - The last one is configurable from GUI
                                                  // - Others are configured in emulation function and set to 0
                                                  //
  SInt32  ParATrig[EFRIO__MAX_EMUL_GUI_TRIG_NB];  // Emulated TLU trigger
  SInt32  ParATS[EFRIO__MAX_EMUL_GUI_TRIG_NB];    // Emulated Flex RIO trigger, called "Time stamp 1"
  
                                                  // DRAM info to emulate Flex RIO readout ( we need a PC RAM bloc of same size as the board one )
                                                  //
  SInt32  InfDRamSzMb;                            // DRAM size in MB
  SInt32  InfDRamSz;                              // DRAM size in bytes
  UInt32* InfDRamPtr;                             // DRAM pointer

  SInt8   InfExtraChan;                           // Extra channel status ( enabled or not ) depends on data transfer mode ( -> run config )
  
  char    InfEmuleFuncCmt[GLB_CMT_SZ];            // A comment set by emulation function selected by ParEmuleFunctNo
                                                  // -> Future use = not implemented now
  
                                                  // DAQ emulation results
                                                  //
  SInt32  ResAcqCnt;                              // Acquisition counter
  SInt32  ResEvCnt;                               // Events counter
                                                  // 
  SInt32  ResAcqFunctRetCode;                     // Error code returned by acquisition function
  
} EFRIO__TAcqEmul;


/* ============================================== */
/* Frames check record                            */
/* ---------------------------------------------- */
/* This is context of frames check functions      */
/* - EFRIO__MI26_FChkAcqIphcMode (...)            */
/* - EFRIO__MI26_FChkAcqEudetMode (...)           */
/*                                                */
/* They check frames integrity, can be used in    */
/* normal DAQ mode or emulation                   */
/* ---------------------------------------------- */
/* Date      : 31/10/2010                         */
/* Doc date  : 07/11/2010                         */
/* Author    : Gilles CLAUS                       */
/* E-mail    : gilles.claus@ires.in2p3.fr         */
/* Labo      : DRS - IPHC                         */
/* ============================================== */

typedef struct {
  
  SInt32 ParAcqNo;                                // Acquisition index
  
  SInt32 ParFrNo;                                 // Frame index
  
  SInt8  ParChkMode;                              // Check mode -> Allows to define a level of test
  
  SInt8  ParChkVerbose;                           // Verbose mode or not
  
  SInt8  ParFrPrintLevel;                         // Define the frame information printed in log file
                                                  // 0 -> No print
                                                  // 1 -> Print general info ( AcqId, FrameId, TrigNb ... ) + Mi26 header, frame cnt, trailer
                                                  // 2 -> Print also triggers list
  
  SInt8  InfMi26Nb;                               // Mi26 number runnig in the system
  
                                                  // Values provided by DAQ to check
                                                  // They are compared to the " same fields " of EFRIO_TAcqEmul
                                                  //
  UInt32 ResAHeader[EFRIO__MAX_ASIC_NB];          // Mi26 header
  UInt32 ResATrailer[EFRIO__MAX_ASIC_NB];         // Mi26 trailer
  UInt32 ResAFrameCnt[EFRIO__MAX_ASIC_NB];        // Mi26 frames counter
  UInt32 ResADataLenght[EFRIO__MAX_ASIC_NB];      // Mi26 data part length ( in W8 not in W16 as in DataLength fields of Mi26 data stream )
                                                  //
  SInt32 ResTrigNb;                               // Trigger info ( couple TLU & Flex RIO trigger ) number UNDER GUI control => limited to EFRIO__MAX_EMUL_GUI_TRIG_NB = 4
  SInt32 ResATrig[EFRIO__MAX_EMUL_GUI_TRIG_NB];   // TLU triggers list 
  SInt32 ResATS[EFRIO__MAX_EMUL_GUI_TRIG_NB];     // Flex RIO trigegrs list
  
  SInt32 ResErrCnt;                               // Total errors counter => Information provide by DAQ <> value set in EFRIO_TAcqEmul
  

} EFRIO__TFrCheck;

/* ============================================== */
/* Board check record                             */
/* ---------------------------------------------- */
/* This is context of board check Vi              */
/* The board test is done by a Vi on Labview side */
/* this record pass the parameters to the Vi      */
/*                                                */
/* Parameters are set via call to functions       */
/* EFRIO__FSetBoardChk...                         */
/*                                                */
/* Parameters are read via call to functions      */
/* EFRIO__FGetBoardChk...                         */
/* Theses functions are encapsulated in Vi        */
/* ---------------------------------------------- */
/*                                                */
/* The test results are written in board status   */
/* record ABoardsStatus [BoardId]                 */
/*                                                */
/* Test results are set or read functions         */
/* EFRIO__FSetBoardStatus...                      */
/* EFRIO__FGetBoardStatus...                      */
/* ---------------------------------------------- */
/* Date      : 22/12/2010                         */
/* Doc date  : 22/12/2010                         */
/* Author    : Gilles CLAUS                       */
/* E-mail    : gilles.claus@ires.in2p3.fr         */
/* Labo      : DRS - IPHC                         */
/* ============================================== */

typedef struct {

  SInt32 ParBoardId;    // Board to test
  
  SInt8  ParDoTheTest;  // Set to 1 to request the test
                        // Reset once test has been started
  
  SInt32 ParTestId;     // Identifier to select different tests
  
  SInt8  ResTestDone;   // Set to 1 when test is finished
  
  SInt32 ResTestResult; // Test result
                        // < 0 => Test has failed
                        // = 0 => Test OK

} EFRIO__TBoardCheck;


/* ============================================== */
/* Spare W32 bloc of index file                   */
/* ---------------------------------------------- */
/* This record contains spare info for index file */
/* ---------------------------------------------- */
/* Date      : 24/02/2011                         */
/* Doc date  : 24/02/2011                         */
/* Author    : Gilles CLAUS                       */
/* E-mail    : gilles.claus@ires.in2p3.fr         */
/* Labo      : DRS - IPHC                         */
/* ============================================== */

typedef struct {
  
  SInt32 AcqStatus;    // Status of acquistion board for this acquisition
  
                       // 0   = OK
                       // > 0 = frame nb lost
                       // < 0 = hardware error
  
  SInt32 TrigStatus;   // No meaning now = reserved for future use
  
  
  SInt32 TotFrameNb;   // Total frames nb in list
  
  SInt32 DiskSectorSz; // Size of disk sector
  
} EFRIO__TFileSpareW32Info;


/* ============================================== */
/* Monitoring record                              */
/* ---------------------------------------------- */
/* This record contains monitoring via Eth conf   */
/* ---------------------------------------------- */
/* Date      : 15/02/2011                         */
/* Doc date  : 15/02/2011                         */
/* Author    : Gilles CLAUS                       */
/* E-mail    : gilles.claus@ires.in2p3.fr         */
/* Labo      : DRS - IPHC                         */
/* ============================================== */

typedef struct {
  
  SInt32 InfFrameNbToSend; // Frame nb to send on Eth = " Frame nb per acq * % monitoring "
  SInt32 InfSzToSend;      // Size corresponding to InfFrameNbToSend
  
} EFRIO__TMon;



/* ============================================== */
/* Lib context record                             */
/* ---------------------------------------------- */
/* This record contains all lib global variables  */
/* ---------------------------------------------- */
/* Date      : 07/08/2010                         */
/* Doc date  : 06/11/2010                         */
/* Author    : Gilles CLAUS                       */
/* E-mail    : gilles.claus@ires.in2p3.fr         */
/* Labo      : DRS - IPHC                         */
/* ============================================== */


typedef struct {
  
  SInt8 InfInitDone;                                       // Lib iit done or not
      
  EFRIO__TBoardConf   ABoardsConf[EFRIO__MAX_BOARDS_NB];   // Acquisition boards config
  EFRIO__TBoardStatus ABoardsStatus[EFRIO__MAX_BOARDS_NB]; // Acquisition boards status
  
  EFRIO__TBoardCheck  BoardChk;                            // Reserved for future implementation
                                                           // Parameters record to check
                                                           // - Boards
                                                           // - Mimosa 26 Clk & Sync signals 
  
  EFRIO__TAcqEmul     AcqEmul;                             // DAQ emulation context
  EFRIO__TFrCheck     FrCheck;                             // Frames check functions context
  
  EFRIO__TRunCont     RunCont;                             // Run context = parameters, memory allocated, results
  
  EFRIO__TMon         MonCont;                             // Monitoring context
  
  EFRIO__TFrameList   AAcqFrameList[1];                    // Frame list of acquistion - Can be extended to more than one acq if needed
  
  // List of frame Id to read ( Eudet3Mode => Trigger + 2 following frames ) / acquistion - Can be extended to more than one acq if needed
  
  SInt16              AAAcqFrameWithTrigList[1][EFRIO__MAX_FRAME_NB_PER_ACQ];
  
 
  EFRIO__TTriggerRec* PtTmpTrigRec;                        // Temporary triggers record used for internal processing
    
} EFRIO__TContext;


#endif
