#include "daq_lib.h"

#include "errors.c"
#include "msg.c"
#include "time.c"
#include "files.c"
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

APP__TContext APP__VGContext; // generic pointer to the DAQ information
//APP__TContext* VPtCont = &APP__VGContext;

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
  
  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "= Print records size for alignment checking Windows / Unix =" ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "= System = %s ", Os ));
  msg (( MSG_OUT, "============================================================" ));
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
 Doc date  : 19/02/2011
 Author    : Gilles CLAUS
 E-mail    : gilles.claus@ires.in2p3.fr
 Labo      : IPHC */
/* =================================================================================== */

SInt32 APP__FLoadRun ( char* RunDir, char* RunPrefix, SInt32 RunNo ) {
  
  APP__TContext* VPtCont = &APP__VGContext;
  SInt32         VAcqNb;
  SInt32         VRet=0; // init necessary, JB 2011/06/30
  
  
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
  
  //printf("pxi_daq: Opening .par file %s with RWBmode %d, MaxBlocSz %d, BlocSz %d\n", VPtCont->InfRunParFile, FIL__TCBinFile_RWB_MODE_READ, sizeof (EFRIO__TRunCont), sizeof (EFRIO__TRunCont));
  
  // Fix to adapt the structure size from 32 to 64 bits machine
  // Data are taken on a 32 bits processor with sizeof (EFRIO__TRunCont) = 1372
  // However on 64 bits processor, sizeof (EFRIO__TRunCont) = 1384
  // JB 2012/03/08
  SInt32 sizeof_EFRIO__TRunCont = 1372;
  
  VRet = VRet | APP__VGRunConfFile.PubFConf ( VPtCont->InfRunParFile, FIL__TCBinFile_RWB_MODE_READ, sizeof_EFRIO__TRunCont, sizeof_EFRIO__TRunCont, 0 /* FlushAfterWrite */, 0 /* MeasTime */ );
  VRet = VRet | APP__VGRunConfFile.PubFOpen ();
  VRet = VRet | APP__VGRunConfFile.PubFSeqRead ( &VPtCont->ResRunCont, sizeof_EFRIO__TRunCont, sizeof_EFRIO__TRunCont );
  
  // Exit if error
  
  err_retfail ( VRet, (ERR_OUT,"Load run parameter file = %s failed !", VPtCont->InfRunParFile ) );
  
  // -----------------------------
  // Open run data file
  // -----------------------------
  
  // Init & conf TCStreamFile class to do the job
  
  VRet = VRet | APP__VGRunDataFile.PubFConf ( &APP__VGRunDataFile, 0 /* UseThread */, VPtCont->InfRunDataFile, FIL__TCBinFile_RWB_MODE_READ, 0 /* FixedBlocSzMode */, EFRIO__MAX_DATA_FILE_BLOC_SZ /* Max */, EFRIO__MAX_DATA_FILE_BLOC_SZ /* Bloc */, 0 /* FlushAfterWrite */, 0 /* MeasTime */ );
  VRet = VRet | APP__VGRunDataFile.PubFOpen ();
  
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
  msg (( MSG_OUT, "==========================================" ));
  
  // ----------------------
  // Allocate acq buffer
  // ----------------------
  
  // Free if already allocated
  // removed because provokes crash when other BoardReader used in combination
  // SS 2012/08
  
  //if ( VPtCont->ResRunCont.PtFrame != NULL ) {
  //  free ( VPtCont->ResRunCont.PtFrame );
  //}
  
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
