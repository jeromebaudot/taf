// Include file for linking the whole daq library to PXIeBoardReader
// It is enough to include this file in PXIeBoardReader.c
//   along with
//
// Originally written by Gille Claus
// Few additions:
// 2014/10/15, JB: Conditional directives on LIB CONFIGURATIO
// 2014/10/15, JB: addition of global var: VAcqNo, VFrNo, VPtFrame

/*******************************************************************************
File      : X:\prj\unix\read_daq_files\flex_rio_2011\main.c
Goal      : Demonstration program for Mi26 Flex RIO DAQ run file loading under Linux
Prj date  : 19/02/2011
File date : 19/02/2011
Doc date  : 19/02/2011
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


/* ======================= */
/* Standard C header files */
/* ======================= */


#define _FILE_OFFSET_BITS 64  // Mandatory for large files ( > 2 GB ) handling
                              // MUST be set before standard C header files

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // types
#include <string.h>
#include <errno.h>
#include <math.h>

/* ========================================= */
/* Conditional compilation for DAQ sources   */
/* ========================================= */

#define ROOT_ROOT // Disable code parts which can't compile under ROOT
#define CC_UNIX   // Specify operating system

#define CC_64B // Mandatory for 64bits machine

#define INT_MAX 128




#define EFRIO_FSBB0


//**********************************
//  LIB CONFIGURATION
//**********************************
// Made in the "pxi_daq_lib_config.h" file
//**********************************
// JB 2014/10/15
// The following include replaces definitions originally done here
#include "pxi_daq_lib_config.h"

// test to check if configuration in "pxi_daq_lib_config.h" is well done
// (is one of the APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1 variable defined ?)
#ifndef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_BEFORE_071112
  #ifndef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_071112_TO_220613
    #ifndef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_SINCE_220613
      #error in pxi_daq_lib_v.1.2 (daq_lib.h) : bad configuration for DATA_FORMAT_CC1 in file include/pxi_daq_lib_config.h 
    #endif
  #endif
#endif

#ifndef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC2_BEFORE_JULY_2012
  #ifndef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC2_SINCE_JULY_2012
    #error in pxi_daq_lib_v.1.2 (daq_lib.h) : bad configuration for DATA_FORMAT_CC2 in file include/pxi_daq_lib_config.h 
  #endif
#endif

/* ========================================= */
/* DAQ commun files -> types & constants     */
/* ========================================= */


#include "globals.def"
#include "globals_root.def"
#include "types.def"
#include "types.typ"

/* ================================================================ */
/* Definition of WIndows types & constant not available under Linux */
/* ---------------------------------------------------------------- */
/* Work like this now = allow to compile program, but should be     */
/* done in a better way later !!!!!!!!!                             */
/* ================================================================ */

#define HANDLE UInt32
#define DWORD  UInt32
#define WINAPI
#define LPVOID void*
#define INVALID_HANDLE_VALUE 0xFFFFFFFF


/* ========================================= */
/* Macros needed to compile DAQ sources      */
/* ========================================= */

#define EXTERN
#define VAR_STATIC
#define VAR_INIT(x) =x
#define VAR_INIT_A2(x,y) ={x,y}  
#define VAR_DCL(Extern,Static,Var) Extern Static Var;
#define VAR_DCL_INIT(Extern,Static,Var,Init) Extern Static Var VAR_INIT (Init);
#define VAR_DCL_INIT_A2(Extern,Static,Var,Init0,Init1) Extern Static Var VAR_INIT_A2 (Init0,Init1);
    

/* ========================================= */
/* Includes DAQ source files                 */
/* ========================================= */

// Errors messages logging library interface 

#include "errors.def"
#include "errors.typ"
#include "errors.var"

// General messages logging library interface

#include "msg.def"
#include "msg.typ"
#include "msg.var"

// ASIC library interface

#include "asic.def"
#include "asic.typ"
#include "asic.var"

// MAPS library interface

#include "maps.def"
#include "maps.typ"
#include "maps.var"

// Time library interface

#include "time.def"
#include "time.typ"
#include "time.var"


// Files library interface

#include "files.def"
#include "files.typ"
#include "files.var"


// Math library interface

#include "math.def"
#include "math.typ"
#include "math.var"



// Mimosa 26 library interface

#include "mi26_usr.def"
#include "mi26_usr.typ"

// Ultimate 1 library interface

#include "ult1_usr.def"
#include "ult1.def"
#include "ult1_usr.typ"
#include "ult1.typ"

// FSBB 0 library interface

#include "fsbb0_usr.def"
#include "fsbb0.def"
#include "fsbb0_usr.typ"
#include "fsbb0.typ"


// Eudet flex rio DAQ library interface

#include "eudet_frio.def"
#include "eudet_frio.typ"
#include "eudet_frio.var"
#include "eudet_frio_usr.def"
#include "eudet_frio_usr.typ"
#include "eudet_frio_usr.var"

// Libraries C source files

#include "errors.c"
#include "msg.c"
#include "time.c"
#include "math.c"
#include "files.c"
#include "math.c"


#include "eudet_frio_fsbb0.c"
#include "eudet_frio_print.c"



/* ========================== */
/* Application constants      */
/* ========================== */

#define APP_ERR_LOG_FILE "./Results/errors.txt"
#define APP_MSG_LOG_FILE "./Results/msg.txt"

/* ========================== */
/* Application macros         */
/* ========================== */

#define APP__READ_CR { while ( getchar () != '\n' ); };
  

/* ============================= */
/* Application context type      */
/* ============================= */


typedef struct {
  
  // Parameters
  
  char   ParRunDir[GLB_FILE_PATH_SZ];       // Run directory
  char   ParFileNamePrefix[20];             // Run file prefix, eg : RUN_666 => RUN_ is the prefix
  SInt32 ParRunNo;
  
  SInt32 ParFrameNbPerAcq;                  // Nb of frame in the acquisition ( get from run file )
  SInt32 ParAcqNo;                          // Index of acquisition to select ( get from GUI )
  SInt32 ParFrameNo;                        // Index of frame to get
  
  // Intermediate variables
    
  char   InfRunParFile[GLB_FILE_PATH_SZ];   // File which contains run parameters *.par
  char   InfRunDataFile[GLB_FILE_PATH_SZ];  // File which contains run data *.bin
  SInt32 InfMaxFrameSz;                     // Maximal size of one frame = total size for N Mimosa 26
  
  // Results
  
  SInt8             ResRunLoaded;           // Flag indicates run state : -1 not loaded, +1 loaded
  
  EFRIO__TRunCont   ResRunCont;             // Run context record = run parameter + additional info
                                            // Loaded from file RUN*.par
  
  EFRIO__TFrameList ResFramesList;          // List of frames 
  
} APP__TContext;


/* ============================= */
/* Application global variables  */
/* ============================= */


// Error messages logging level ( file errors.txt ) , can be
// - ERR_LOG_LVL_NONE
// - ERR_LOG_LVL_ALL
// - ERR_LOG_LVL_WARNINGS_ERRORS
// - ERR_LOG_LVL_ERRORS


SInt32 APP_VGErrFileLogLvl = ERR_LOG_LVL_ALL; // Log level for log file
SInt32 APP_VGErrUserLogLvl = ERR_LOG_LVL_ALL; // Log level for user print function => not used

// General messages logging level ( file msg.txt ), 127 => log all messages

SInt32 APP_VGMsgFileLogLvl = 127; // Log level for log file
SInt32 APP_VGMsgUserLogLvl = 127; // Log level for user print function => not used

// Class to read run file
// - APP__VGRunConfFile for run conf file = RUN*.par
// - APP__VGRunDataFile for run data file = RUN*.bin (data) and RUN*.bin.inf (index file)

FIL__TCBinFile      APP__VGRunConfFile ( "./err_TCBinFile.txt"   , 1 /* EnableErrLog */, ERR_LOG_LVL_ERRORS );
FIL__TCStreamFile   APP__VGRunDataFile ( "./err_TCStreamFile.txt", 1 /* EnableErrLog */, ERR_LOG_LVL_ERRORS, 512 /* DiskBlocSz */ );


// Application context record
// - Contains all global variables needed => easier to move to an object
// - Should also include errors and messages log variables ( APP_VGErrFileLogLvl, etc ... ), not done now

APP__TContext APP__VGContext;

// Addition for PXIeBoardReader, JB 2014/10/15
SInt32 VAcqNo; // current acquisition number
SInt32 VFrNo; // current frame number
EFRIO__TFrame* VPtFrame; // pointer to the current frame


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 19/02/2011
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 APP__FIsLittleEndian ()
          :
Goal      : Test is CPU is little endian, return 1 in this case, 0 otherwise.
          :
Inputs    : None
          :
Ouputs    : The function returns
          : 0 - CPU is not little endian
          : 1 - CPU is little endian
          :
Globals   : None
          :
Remark    : One
          :
Level     :
Date      : 19/02/2011
Doc date  : 19/02/2011
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */

SInt32 APP__FIsLittleEndian () {
  
  SInt32   VTest = 0x11223344;
  char* VPt;
  
  VPt = (char*) &VTest;
  
  if ( (VPt[0] == 0x11) && (VPt[1] == 0x22) && (VPt[2] == 0x33) && (VPt[3] == 0x44) ) {
    return (0);
  }
  
  else {
    return (1);
  }
  
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 19/02/2011
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 APP__FPrintRecSzChkAlign ( char* Os ) {
  
  // In the logfile
  // Change Antonin Maire, based on an idea by Gilles Claus, Alejandro Perez (16 oct 2014)
  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "= Print records size for alignment checking Windows / Unix =" ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "= System = %s ", Os ));
  msg (( MSG_OUT, "============================================================"     )); 
  msg (( MSG_OUT, "sizeof(int*)                = %d",   sizeof(int*)              ));
  msg (( MSG_OUT, "sizeof(int)                 = %d",   sizeof(int)               ));
  msg (( MSG_OUT, "sizeof(unsigned)            = %d",   sizeof(unsigned)          ));
  msg (( MSG_OUT, "sizeof(unsigned int)        = %d",   sizeof(unsigned int)      ));
  msg (( MSG_OUT, "sizeof(unsigned long int)   = %d",   sizeof(unsigned long int) ));
  msg (( MSG_OUT, "sizeof(unsigned long)       = %d",   sizeof(unsigned long)     ));
  msg (( MSG_OUT, "sizeof(unsigned short)      = %d",   sizeof(unsigned short)    ));
  msg (( MSG_OUT, "sizeof(long int)            = %d",   sizeof(long int)          ));
  msg (( MSG_OUT, "sizeof(size_t)              = %d",   sizeof(size_t)            ));
  msg (( MSG_OUT, "sizeof(uint32_t)            = %d",   sizeof(uint32_t)          ));    
  msg (( MSG_OUT, "sizeof(uint64_t)            = %d",   sizeof(uint64_t)          ));    
  msg (( MSG_OUT, "sizeof(int64_t)             = %d",   sizeof(int64_t)           ));    
  
  msg (( MSG_OUT, "sizeof(float)               = %d",   sizeof(float)             ));    
  msg (( MSG_OUT, "sizeof(double)              = %d",   sizeof(double)            ));   
  msg (( MSG_OUT, "============================================================\n"   ));
  msg (( MSG_OUT, "" ));

  msg (( MSG_OUT, "FIL__TCStreamFile_TBlocInf    = %d ", sizeof (FIL__TCStreamFile_TBlocInf)    ));
  msg (( MSG_OUT, "FIL__TCStreamFile_TRecInfFile = %d ", sizeof (FIL__TCStreamFile_TRecInfFile) ));
  msg (( MSG_OUT, "EFRIO__TRunCont               = %d ", sizeof (EFRIO__TRunCont) ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "EFRIO__TFrame                 = %d ", sizeof (EFRIO__TFrame) ));
  msg (( MSG_OUT, "EFRIO__TFrameHeader           = %d ", sizeof (EFRIO__TFrameHeader) ));
  msg (( MSG_OUT, "EFRIO__TFrameData             = %d ", sizeof (EFRIO__TFrameData) ));
  msg (( MSG_OUT, "EFRIO__TTriggerRec            = %d ", sizeof (EFRIO__TTriggerRec) ));
  
  msg (( MSG_OUT, "" ));
  
  // On stdout.
  // Change Antonin Maire, based on an idea by Gilles Claus, Alejandro Perez (16 oct 2014)
  //    Check the size in terms of bytes of the different type on the current system.
  //    Very important to know, in order to read properly the words of the C struct of the written frame.
  //    Very likely the writing is done under a Windows 32 bits system, 
  //    while the analysis treatment is done under Mac/Linux 64 bits.
  printf("============================================================\n"   );
  printf("= Print records size for alignment checking Windows / Unix =\n"   );
  printf("------------------------------------------------------------\n"   );
  printf("= System = %s ", Os );
  printf("-------------------------------\n"                                );
  printf("sizeof(int*)                = %d \n",   sizeof(int*)              );
  printf("sizeof(int)                 = %d \n",   sizeof(int)               );
  printf("sizeof(unsigned)            = %d \n",   sizeof(unsigned)          );
  printf("sizeof(unsigned int)        = %d \n",   sizeof(unsigned int)      );
  printf("sizeof(unsigned long int)   = %d \n",   sizeof(unsigned long int) );
  printf("sizeof(unsigned long)       = %d \n",   sizeof(unsigned long)     );
  printf("sizeof(unsigned short)      = %d \n",   sizeof(unsigned short)    );
  printf("sizeof(long int)            = %d \n",   sizeof(long int)          );
  printf("sizeof(size_t)              = %d \n",   sizeof(size_t)            );
  printf("sizeof(uint32_t)            = %d \n",   sizeof(uint32_t)          );    
  printf("sizeof(uint64_t)            = %d \n",   sizeof(uint64_t)          );    
  printf("sizeof(int64_t)             = %d \n",   sizeof(int64_t)           );    
  
  printf("sizeof(float)               = %d \n",   sizeof(float)             );    
  printf("sizeof(double)              = %d \n",   sizeof(double)            );   
  printf("============================================================\n"   );
  printf(" ");

  printf("FIL__TCStreamFile_TBlocInf    = %d \n", sizeof (FIL__TCStreamFile_TBlocInf)       );
  printf("FIL__TCStreamFile_TRecInfFile = %d \n", sizeof (FIL__TCStreamFile_TRecInfFile)    );
  printf("EFRIO__TRunCont               = %d \n", sizeof (EFRIO__TRunCont)                  );
  printf("------------------------------------------------------------\n"       );
  printf("EFRIO__TFrame                 = %d \n", sizeof (EFRIO__TFrame)        );
  printf("EFRIO__TFrameHeader           = %d \n", sizeof (EFRIO__TFrameHeader)  );
  printf("EFRIO__TFrameData             = %d \n", sizeof (EFRIO__TFrameData)    );
  printf("EFRIO__TTriggerRec            = %d \n", sizeof (EFRIO__TTriggerRec)   );

  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 APP__FLoadRun ( char* RunDir, char* RunPrefix, SInt32 RunNo )
          :
Goal      : Load a run file = configure lib for this run but the run is not loaded
          : in memory.
          :
Inputs    : RunDir     - Run directory without \ at end
          : RunPrefix  - Run file prefix, eg : "RUN_"
          : RunNo      - Run no
          :
Ouputs    : The function returns
          :  Number of acquisitions in run file
          : -1 if an error occurs
          :
Globals   : Update APP__VGContext fields
          :
Remark    : None
          :
Level     :
Date      : 19/02/2011
Rev       : 06/07/2011
          : - Bug fix => VRet is used via a "OR" to cumlate errors and was not set to 0 !
Doc date  : 19/02/2011
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */

SInt32 APP__FLoadRun ( char* RunDir, char* RunPrefix, SInt32 RunNo ) {
  
  APP__TContext* VPtCont = &APP__VGContext;
  SInt32         VAcqNb;
  SInt32         VRet = 0;
  

  // -----------------------------
  // Reset run loaded flag
  // -----------------------------

  VPtCont->ResRunLoaded = -1;

  // -----------------------------
  // Set run parameters
  // -----------------------------

  sprintf ( VPtCont->ParRunDir        , "%s", RunDir    );
  sprintf ( VPtCont->ParFileNamePrefix, "%s", RunPrefix );
  
  VPtCont->ParRunNo = RunNo;
  
  // Calculate run par file & run data file names
  
  sprintf ( VPtCont->InfRunDataFile, "%s/%s%d.bin", VPtCont->ParRunDir, VPtCont->ParFileNamePrefix, VPtCont->ParRunNo );
  sprintf ( VPtCont->InfRunParFile , "%s/%s%d.par", VPtCont->ParRunDir, VPtCont->ParFileNamePrefix, VPtCont->ParRunNo );
  
  // Print run par file & run data file names for debugging
  
  msg (( MSG_OUT, "Run data file name = %s", VPtCont->InfRunDataFile ));
  msg (( MSG_OUT, "Run par file name  = %s", VPtCont->InfRunParFile ));
  
  
  // -----------------------------
  // Read run param file
  // -----------------------------

  // Init & conf TCBinFile class to do the job
  
  // Fix to adapt the structure size from 32 to 64 bits machine
  // Data are taken on a 32 bits processor with EFRIO__TRunCont = 1628 
  // However on 64 bits processor, EFRIO__TRunCont = 1758
  // JB 2012/10/16
  SInt32 sizeof_EFRIO__TRunCont = 1628;
  
  printf(" FLoadRun: reading param file with sizeof_EFRIO__TRunCont %d\n", sizeof_EFRIO__TRunCont);
  
//  VRet = VRet | APP__VGRunConfFile.PubFConf ( VPtCont->InfRunParFile, FIL__TCBinFile_RWB_MODE_READ, sizeof (EFRIO__TRunCont), sizeof (EFRIO__TRunCont), 0 /* FlushAfterWrite */, 0 /* MeasTime */ );
  VRet = VRet | APP__VGRunConfFile.PubFConf ( VPtCont->InfRunParFile, FIL__TCBinFile_RWB_MODE_READ, sizeof_EFRIO__TRunCont, sizeof_EFRIO__TRunCont, 0 /* FlushAfterWrite */, 0 /* MeasTime */ );
  printf(" FLoadRun: After param:PubFConf ret %d\n", VRet);  
  VRet = VRet | APP__VGRunConfFile.PubFOpen ();
  printf(" FLoadRun: After param:PubFOpen ret %d\n", VRet);  
//  VRet = VRet | APP__VGRunConfFile.PubFSeqRead ( &VPtCont->ResRunCont, sizeof_EFRIO__TRunCont, sizeof (EFRIO__TRunCont), sizeof (EFRIO__TRunCont) );
  VRet = VRet | APP__VGRunConfFile.PubFSeqRead ( &VPtCont->ResRunCont, sizeof_EFRIO__TRunCont, sizeof_EFRIO__TRunCont );
  printf(" FLoadRun: After param:PubFSeqRead ret %d\n", VRet);  
  
  // Exit if error
  
  err_retfail ( VRet, (ERR_OUT,"Load run parameter file = %s failed !", VPtCont->InfRunParFile ) );
  
  // -----------------------------
  // Open run data file
  // -----------------------------
  
  // Init & conf TCStreamFile class to do the job
  
  printf(" FLoadRun: opening bin file with max_bloc_sz %d\n", EFRIO__MAX_DATA_FILE_BLOC_SZ);
  
  VRet = VRet | APP__VGRunDataFile.PubFConf ( &APP__VGRunDataFile, 0 /* UseThread */, VPtCont->InfRunDataFile, FIL__TCBinFile_RWB_MODE_READ, 0 /* FixedBlocSzMode */, EFRIO__MAX_DATA_FILE_BLOC_SZ /* Max */, EFRIO__MAX_DATA_FILE_BLOC_SZ /* Bloc */, 0 /* FlushAfterWrite */, 0 /* MeasTime */ );
  printf(" FLoadRun: After data:PubFConf ret %d\n", VRet);  
  VRet = VRet | APP__VGRunDataFile.PubFOpen ();
  printf(" FLoadRun: After data:PubFOpen ret %d\n", VRet);  
  
  // Exit if error
  
  err_retfail ( VRet, (ERR_OUT,"Open run data file %s failed !", VPtCont->InfRunDataFile ) );
  

  // -----------------------------
  // Set run loaded flag
  // -----------------------------
  
  VPtCont->ResRunLoaded = 1;

  // -----------------------------
  // Return number of acq in file
  // -----------------------------
  
  VAcqNb = APP__VGRunDataFile.PubFGetBlocNb ();
  
  err_retval ( VAcqNb, ( ERR_OUT, "Run %d loaded => Contains %d acquistions", RunNo, VAcqNb ) );    
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 APP__FAllocAcqBuffer ( )
          :
Goal      : Allocates the buffer for one acqusition.
          :
Inputs    : None
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   : Read informations from APP__VGContext and set pointer to buffer.
          :
Remark    : None
          :
Level     :
Date      : 19/02/2011
Doc date  : /2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */

SInt32 APP__FAllocAcqBuffer () {

  APP__TContext* VPtCont = &APP__VGContext;


  // -------------------------------
  // Exit if there is no run loaded
  // -------------------------------
  
  err_retfail ( VPtCont->ResRunLoaded, (ERR_OUT,"Abort => NO run loaded" ) );

  
  // ----------------------
  // Calculates sizes
  // ----------------------
  
  VPtCont->InfMaxFrameSz          = ( sizeof ( EFRIO__TFrame ) + ( VPtCont->ResRunCont.ParMi26Nb * MI26__ZS_FFRAME_RAW_MAX_W8 ) + sizeof ( EFRIO__TTriggerRec ) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );
  VPtCont->ResRunCont.InfFrameBuffSz = VPtCont->ResRunCont.ParFrameNbPerAcq * VPtCont->InfMaxFrameSz;
  
  // ----------------------
  // Print results
  // ----------------------
  
  msg (( MSG_OUT, "==========================================" ));
  msg (( MSG_OUT, "InfMaxFrameSz            = %d", VPtCont->InfMaxFrameSz ));
  msg (( MSG_OUT, "ResRunCont.InfFrameBuffSz   = %d", VPtCont->ResRunCont.InfFrameBuffSz   ));
  msg (( MSG_OUT, "EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB = %d", EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ));
  msg (( MSG_OUT, "EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ     = %d", EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ ));
  msg (( MSG_OUT, "==========================================" ));
  
  
  // ----------------------
  // Allocate acq buffer
  // ----------------------
  
  // Free if already allocated
  
  if ( VPtCont->ResRunCont.PtFrame != NULL ) {
    free ( VPtCont->ResRunCont.PtFrame );
  }
  
  // Try to allocate

  VPtCont->ResRunCont.PtFrame = (EFRIO__TFrame*) malloc ( VPtCont->ResRunCont.InfFrameBuffSz );
    
  err_retnull ( VPtCont->ResRunCont.PtFrame, (ERR_OUT,"Allocation of EUDET buffer for %d frames failed !", VPtCont->ResRunCont.ParFrameNbPerAcq) );
  
  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 APP__FFreeAcqBuffer ()
          :
Goal      : Free acquisition buffer
          :
Inputs    : None
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   : Read informations from APP__VGContext and set buffer pointer to NULL.
          :
Remark    : None
          :
Level     :
Date      : 19/02/2011
Doc date  : /2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */

SInt32 APP__FFreeAcqBuffer ( ) {
  
  APP__TContext* VPtCont = &APP__VGContext;
  
    
  // -------------------------------
  // Free buffer
  // -------------------------------

  if ( VPtCont->ResRunCont.PtFrame != NULL ) {
    free ( VPtCont->ResRunCont.PtFrame );
    VPtCont->ResRunCont.PtFrame = NULL;
  }
  

  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 APP__FBuildFrameListFromAcq ( SInt32 FrameNb, void* PtAcqData, EFRIO__TFrameList* PtList )
          :
Goal      : Build the frame list for one acquisition
          :
Inputs    : FrameNb    - The number of frames in the acquisition
          : PtAcqData  - A pointer to source data = all frames of one acquisition
          : PtList     - A pointer to the frame list to build
          :    
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   :
          :
Remark    : This function is the copy of EFRIO__FBuildFrameListFromAcq for application
          : program demo.
          :
          : This function is called to build the frame list ( eg : AAcqFrameList field
          : of lib context record ) while reading data from run file.
          :
          : For more information, read comments on EFRIO__TFrameList record in *.typ file
          :
Level     :
Date      : 06/11/2010
Rev       : 19/02/2011
          : - Moved from eudet_fio.c
          :
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 APP__FBuildFrameListFromAcq ( SInt32 FrameNb, void* PtAcqData, EFRIO__TFrameList* PtList ) {
  
  SInt32 ViFrame;
  EFRIO__TFrame* VPtFirstFr;
  EFRIO__TFrame* VPtNextFr;
  
  // --------------
  // Check param
  // --------------
  
  if ( (FrameNb <= 0) || (FrameNb > EFRIO__MAX_FRAME_NB_PER_ACQ) ) {
    err_retfail ( -1, (ERR_OUT,"FrameNB=%d out of range 1..%d", FrameNb, EFRIO__MAX_FRAME_NB_PER_ACQ ) );
  }
  
  err_retnull ( PtAcqData, (ERR_OUT,"PtAcqData == NULL") );
  err_retnull ( PtList   , (ERR_OUT,"PtList == NULL   ") );
  
  // --------------
  // Reset list
  // --------------
  
  memset ( PtList,0 , sizeof (EFRIO__TFrameList) );
  
  
  // --------------
  // Build frames list
  // --------------
  
  PtList->TotFrameNb = FrameNb;
  
  // Set frame pointer on first frame
  
  VPtFirstFr = (EFRIO__TFrame*) PtAcqData;
  
  // Fill first elt
  
  PtList->AFrameSz[0]  = VPtFirstFr->TotSz;
  PtList->AFramePtr[0] = VPtFirstFr;
  
  VPtNextFr = (EFRIO__TFrame*) ( ((UInt8*) VPtFirstFr) + VPtFirstFr->TotSz );
  
  // Fill following elt
  
  for ( ViFrame=1; ViFrame < FrameNb; ViFrame++ ) {
    PtList->AFrameSz[ViFrame]  = VPtNextFr->TotSz;
    PtList->AFramePtr[ViFrame] = VPtNextFr;
    VPtNextFr = (EFRIO__TFrame*) ( ((UInt8*) VPtNextFr) + VPtNextFr->TotSz );
  }  
  
  err_retok (( ERR_OUT, "" ));
}

  
  
/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 APP__FGotoAcq ( SInt32 AcqNo, SInt32* PtFrameNb )
          :
Goal      : Load AcqNo in memory
          :
Inputs    : AcqNo     - No of acquisition to load
          : PtFrameNb - Pointer to a variable which will be set with frames nb in AcqNo
          :           - Set it to NULL if you don't need it
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   : Read information from APP__VGContext.
          :
Remark    : None.
          :
Level     :
Date      : 19/02/2011
Rev       : 07/03/2011
          : - New " spare W32 info " format 
Doc date  : /2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */

SInt32 APP__FGotoAcq ( SInt32 AcqNo, SInt32* PtFrameNb ) {
  
  APP__TContext* VPtCont = &APP__VGContext;
  SInt32                   VAcqInfFormat;
  EFRIO__TFileSpareW32Info VAcqInf;
  SInt32         VRet;
  
  // -------------------------------
  // Exit if there is no run loaded
  // -------------------------------
  
  err_retfail ( VPtCont->ResRunLoaded, (ERR_OUT,"Abort => NO run loaded" ) );
    
  // -----------------------------------
  // Load acquisition in memory
  // -----------------------------------
  
  // Load from run file
  
  // Before 07/03/2011
  //  VRet = APP__VGRunDataFile.PubFBlocRead ( AcqNo, VPtCont->ResRunCont.PtFrame, VPtCont->ResRunCont.InfFrameBuffSz /* Max dest size */, &VFrameNb );

  VRet = APP__VGRunDataFile.PubFBlocRead  ( AcqNo, VPtCont->ResRunCont.PtFrame, VPtCont->ResRunCont.InfFrameBuffSz /* Max dest size */, &VAcqInfFormat, sizeof (VAcqInf) / 4 /* MaxSpareW32InfoNb */, (SInt32*) &VAcqInf );

  err_retfail ( VRet, (ERR_OUT, "Goto acquisition No %d failed !", AcqNo ) );
  
  // Build frame list = array of pointer to access frames one by one
  
  VRet = APP__FBuildFrameListFromAcq ( VAcqInf.TotFrameNb, VPtCont->ResRunCont.PtFrame, &VPtCont->ResFramesList );
  
  err_retfail ( VRet, (ERR_OUT,"Build frames list for acquisition No %d failed !", AcqNo ) );
  
  // -----------------------------------
  // Update frame nb parameter
  // -----------------------------------

  if ( PtFrameNb != NULL ) {
    *PtFrameNb = VAcqInf.TotFrameNb;
  }
 
  err_retok (( ERR_OUT, "Acquisiton No %d loaded in memory - Contains %d frames", AcqNo, VAcqInf.TotFrameNb ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : EFRIO__TFrame* APP__FGetFramePt ( SInt32 FrameIdInAcq )
          :
Goal      : Return a pointer to one frame of the acquisition which is loaded in memory.
          :
Inputs    : FrameIdInAcq - The no of the frame
          :
Ouputs    : The function returns
          :  A valid pointer to the frame
          :  A NULL pointer if the operation failed
          :
Globals   : Read information from APP__VGContext.
          :
Remark    : None
          :
Level     :
Date      : 19/02/2011
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

EFRIO__TFrame* APP__FGetFramePt ( SInt32 FrameIdInAcq ) {

  APP__TContext*      VPtCont    = &APP__VGContext;
  EFRIO__TRunCont*    VPtRunCont = &VPtCont->ResRunCont;
  EFRIO__TFrameList*  VPtFrList  = &VPtCont->ResFramesList;
  
  
  
  if ( (FrameIdInAcq < 0) || (FrameIdInAcq >= VPtRunCont->ParFrameNbPerAcq) ) {
    err_retfailnull ( -1, (ERR_OUT,"Abort : Bad FrameId=%d out of range [0..%d]", FrameIdInAcq, VPtRunCont->ParFrameNbPerAcq-1 ) );
  }
  
  if ( VPtFrList->AFramePtr[FrameIdInAcq] == NULL ) {
    err_retfailnull ( -1, (ERR_OUT,"No frame=%d in list => Pointer NULL", FrameIdInAcq) );
  }
  
  return ( VPtFrList->AFramePtr[FrameIdInAcq] );
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : int main ()
:
Goal      : Demonstration function
          : - read run
          : - move between acquisitions and frames
          : - print frames information ( header, data, trigger ) in messages file
          :
Inputs    : None
          :
Ouputs    : Always 0 
          :
Globals   : All
          :
Remark    : None
          :
Level     :
Date      : 19/02/2011
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


int main ()
{    
  APP__TContext* VPtCont = &APP__VGContext;
  SInt32 VMyVar          = 10;
  SInt32 VRet            = 0;     // Variable to store error code of functions called
  EFRIO__TFrame* VPtFrame;
  SInt32 VAcqNo;
  SInt32 VFrNo;


  /* ----------------------------------------- */
  /* Init application errors messages handling */
  /* ----------------------------------------- */
  
  ERR_FBegin             ( ( APP_VGErrFileLogLvl != 0 ) /* Enable */, APP_ERR_LOG_FILE );
  ERR_FSetFileLogLevel   ( APP_VGErrFileLogLvl   );
  ERR_FSetUserLogLevel   ( APP_VGErrUserLogLvl   );
  // ERR_FSetUserErrorFunc  ( APP_FUserErrorFunc    );
  
  /* ------------------------------------------ */
  /* Init application general messages handling */
  /* ------------------------------------------ */
  
  MSG_FBegin             ( (APP_VGMsgFileLogLvl != 0) /* Enable */,  APP_MSG_LOG_FILE );
  MSG_FSetFileLogLevel   ( APP_VGMsgFileLogLvl   );
  MSG_FSetUserLogLevel   ( APP_VGMsgUserLogLvl   );
  // MSG_FSetUserMsgFunc    ( APP_FUserMsgFunc      );
  

  /* ------------------------------------------ */
  /* Print records sise                         */
  /* ------------------------------------------ */
  
  APP__FPrintRecSzChkAlign ( "Linux" );
  
  /* ------------------------------------------ */
  /* Reset application context record           */
  /* ------------------------------------------ */
  
  memset ( VPtCont, 0,  sizeof (APP__TContext) );
  
  VPtCont->ResRunLoaded = -1; // No run loaded
  
  
  /* ------------------------------------------ */
  /* Test if CPU is little / big endian         */
  /* ------------------------------------------ */
  
  printf ( "========================================================= \n" );
  printf ( "Little endian = %d \n", APP__FIsLittleEndian () );
  printf ( "========================================================= \n" );
    
  /* ------------------------------------------ */
  /* Error logging macros demo                  */
  /* ------------------------------------------ */
  
  err_trace   (( ERR_OUT, "This is a trace message   - VMyVar=%d", VMyVar ));
  err_warning (( ERR_OUT, "This is a warning message - VMyVar=%d", VMyVar ));
  err_error   (( ERR_OUT, "This is an error message  - VMyVar=%d", VMyVar ));
  
  /* ------------------------------------------ */
  /* Messages logging macros demo               */
  /* ------------------------------------------ */
  
  msg  (( MSG_OUT, "This is a general message - with default LogLvl = 1 - VMyVar=%d", VMyVar ));
  msg  (( MSG_OUT, "sizeof (EFRIO__TRunCont) = %d", sizeof (EFRIO__TRunCont) ));


  // --------------------------------------------------------------
  // Printf records size for alignment checking Unix / Windows
  // --------------------------------------------------------------
  
  // APP__FPrintRecSzChkAlign ( "Linux" );

  // -----------------------------
  // Load run
  // -----------------------------
  
  printf ( "Try to load run \n" );
  
  
  
//  VRet = APP__FLoadRun ( "/Projets/Caract_test/tmp/mi28_bt_run_file_format_var_length_040214__template_run/286000" /* RunDir */, "RUN_" /* RunPrefix */, 286000 /* RunNo */ );  
//  VRet = APP__FLoadRun ( "/Projets/Caract_test/tmp/mi28_bt_run_file_format_var_length_040214__template_run/288000" /* RunDir */, "RUN_" /* RunPrefix */, 288000 /* RunNo */ );
//  VRet = APP__FLoadRun ( "/Projets/Caract_test/tmp/mi28_bt_run_file_format_var_length_040214__template_run/286001" /* RunDir */, "RUN_" /* RunPrefix */, 286001 /* RunNo */ );
//  VRet = APP__FLoadRun ( "/Projets/Caract_test/tmp/mi28_bt_run_file_format_var_length_040214__template_run/288001" /* RunDir */, "RUN_" /* RunPrefix */, 288001 /* RunNo */ );
  
//  VRet = APP__FLoadRun ( "/Projets/Caract_test/tmp/mi28_bt_run_file_format_var_length_040214__template_run/288001" /* RunDir */, "RUN_" /* RunPrefix */, 288001 /* RunNo */ );
  VRet = APP__FLoadRun ( "/Users/jeromeb/Data/cmos/fsbb/35400" /* RunDir */, "RUN_" /* RunPrefix */, 35400 /* RunNo */ );

  printf ( "Run loaded => Contains %d acquistions \n", VRet );
  
  // --------------------------------------------------------------
  // Printf run context record = run paramters from file RUN*.par
  // --------------------------------------------------------------
  
  EFRIO__FPrintRunContRec ( &VPtCont->ResRunCont );
  

  // -----------------------------
  // Allocate acquisition buffer
  // -----------------------------
  
  APP__FAllocAcqBuffer ();
    
    
  
  EFRIO__FPrintRunContRec ( &VPtCont->ResRunCont );  // DBG 07/02/14
  
    
  // -----------------------------
  // Goto acq & print frame record
  // -----------------------------
  
  VAcqNo = 0;
  
  while ( VAcqNo >= 0 ) {
    
    // Print " menu "
    
    printf ( "\n" );
    printf ( "-------------------------------------------------------------------------- \n" );
    printf ( "Goto acquisition No, Frame No => print record in messages logfile \n" );
    printf ( "Set Acq No & Frame No to -1 in order to exit \n" );
    printf ( "Acquisition No ? " );
    scanf  ( "%d", &VAcqNo );
    APP__READ_CR;
    printf ( "Frame No ? " );
    scanf  ( "%d", &VFrNo );
    APP__READ_CR;
    
    // User wants to stop frames scanning
    
    if ( VAcqNo == -1 ) {
      break;
    }
    
    // Load acquisition in memory and get pointer on frame
    
    VRet = APP__FGotoAcq ( VAcqNo /* AcqNo */, NULL /* PtFrameNb */ );

    VPtFrame = APP__FGetFramePt ( VFrNo /* FrameIdInAcq */ );
    
    // Check if frame is available => Jump to menu in case of error 
    
    if ( (VRet < 0) || (VPtFrame == NULL) ) {
      printf ( "Goto acquistion %d frame %d failed ! \n", VAcqNo, VFrNo );
      continue;
    }
    
    // Acquistion loaded and frame found
    
    printf ( "Goto acquisition %d frame %d done => Result in messages logfile \n", VAcqNo, VFrNo );
    
    // Print frame record, information type depend on PrintLevel parameter
    // You can look at EFRIO__FPrintFrameRec to see how to access to all frame fields
  
    // PrintLevel  - 0 -> Print nothing
    //             - 1 -> Print AcqId & FrId
    //             - 2 -> Print AcqId, FrId ... Mi26 header, trailer ... + Trig nb
    //             - 3 -> Print AcqId, FrId ... Mi26 header, trailer ... + Trig nb + Data
    //             - 4 -> Print trigger list
    
    
    EFRIO__FPrintFrameRecFsbb0 ( VPtFrame, 3 /* PrintLevel */ );
    
  } // End while (1)
  
  
  // -----------------------------
  // Close run class
  // -----------------------------
  
  APP__VGRunDataFile.PubFClose ();
  APP__VGRunConfFile.PubFClose ();
  

  // -----------------------------
  // Free acquisition buffer
  // -----------------------------
  
  APP__FFreeAcqBuffer ();
  
  return 0;
}



//---------------------------------------------------------------------------
