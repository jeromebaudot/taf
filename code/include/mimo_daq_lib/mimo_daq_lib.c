#include "mimo_daq_lib.h"

#include "errors_light.c"
#include "msg_light.c"



/* ========================== */
/* Application constants      */
/* ========================== */

//#define MIS1__TBtRunRead_DEF_ERR_LOG_FILE "./Results/err_MIS1__TBtRunRead.txt"
//#define MIS1__TBtRunRead_DEF_MSG_LOG_FILE "./Results/msg_MIS1__TBtRunRead.txt"

/* ========================== */
/* Application macros         */
/* ========================== */

#define APP__READ_CR { while ( getchar () != '\n' ); };
#define ERR_LOG_LVL_ALL 1

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
  
  //EFRIO__TRunCont   ResRunCont;             // Run context record = run parameter + additional info
  // Loaded from file RUN*.par
  
  //EFRIO__TFrameList ResFramesList;          // List of frames
  
} APP__TContext;


SInt32 APP_VGErrFileLogLvl = ERR_LOG_LVL_ALL; // Log level for log file
SInt32 APP_VGErrUserLogLvl = ERR_LOG_LVL_ALL; // Log level for user print function => not used

// General messages logging level ( file msg.txt ), 127 => log all messages

SInt32 APP_VGMsgFileLogLvl = 127; // Log level for log file
SInt32 APP_VGMsgUserLogLvl = 127; // Log level for user print function => not used


APP__TContext APP__VGContext; // generic pointer to the DAQ information
