/////////////////////////////////////////////////////////////
// Class Description of PXIeBoardReader                     //
//                                                         //
// Read PXIexpress digital raw data files
//  originally for MIMOSA 26                               //
// and produce an event for getting the results            //
// This version compatible with pxi_daq library
//  V1.0 - 07/03/2011
//  V1.1
//  V1.2 - 08/01/2014
//
// Important parameters driving readout behavior:
//    TriggerMode
//    EventBuildingMode
//    FramesToSkipAtEndOfAcqForTrig
//    TriggerLineOffset
//    FramesPostTrigger
//  
// 
//							   
// Terminology:
//  Frame = data for one sensor from first to last line
//  DaqEvent = set of frames, one for each sensors
//  Event = data relevant to a given trigger,
//	which may encompass several frames of the same sensor (hence DAQEvent),
//	this is the unit relevant for physics analysis
//
// Notice the current version assumes the data are contained
// only in one file
//                                                         //
/////////////////////////////////////////////////////////////
//
// created JB, 2009/08/20
// Last Modified JB, 2009/08/30
// Last Modified JB, 2009/09/01 message level, file index
// Last Modified JB, 2009/09/09 overflow management, statistics printout, double reading
// Last Modified JB, 2009/09/14 Statistics
// Last Modified JB, 2009/09/16 Abort some specific events
// Last Modified JB, 2010/06/16 Allow several triggers in an event
// Last Modified JB, 2010/07/06 Overflow & event building management
// Last Modified JB, 2011/03/14 Class reshaped to use DAQ library from GC
// Last Modified JB, 2011/06/18 to add trigger counter
// Last Modified JB, 2011/06/28 improved trigger readtout, better MSG & ERR files management and automatic #rows
// Last Modified JB, 2011/06/30 ReadTriggersFromFrame
// Last modified JB, 2011/07/07 to localize path names
// Last modified JB, 2011/07/18 correct multi-triggers storage
// Last modified JB, 2011/10/30 PrintStatistics
// Last modified JB, 2011/10/31 ReadPixelsFromFrame & DefineEventEnd
// Last modified JB, 2011/11/02 count overflow per sensor with overflowSensor
// Last modified SS, 2011/11/04 many correction in the event start and end
// Last modified JB, 2012/02/23 introduce new trigger mode=2 for power pulsing
// Last modified JB, 2012/03/11 kill pixel in ReadPixelsFromFrame with gen. meth.
// Last modified MG, 2012/03/14 line overflow vector.
// Last modified JB, 2012/05/03 added method DumpEventHeaders
// Last modified JB, 2012/05/04 added timestamps
// Last modified JB, 1012/07/10 new methods CheckTriggerValidity & ResetDaqEvent
// Last modified JB, 1012/07/10 methods to read selected trigger nb or frame
// Last modified SS, 2012/08/01 handle new AMapsTrigInfo format
// Last modified JB, 2012/09/04 management of required frames not found 
// Last modified JB, 2012/09/25 ReadTriggersFromFrame, GetTriggerList
// Last modified JB, 2013/06/22 ReadTriggersFromFrame
// Last modified JB, 2013/07/20 introducing trigger mode 3 in ReadTriggersFromFrame, StartEventReading, DefineEventend and HasData
// Last Modified: BH 2013/08/20, warning corrections from clang
// Last Modified: JB, 2014/02/12 CheckPixelToAdd, AddDUTSensor, IsDUTSensor, PrintStatistics
// Last Modified: VR, 2014/03/20 New warning message when loading run failed, for the pxi_daq_lib_ versions 1.1 and 1.2
// Last Modified: VR, 2014/03/20 configuration of PXI DAQ LIB VERSION centralized in file pxi_daq_lib_config.h
// Last Modified: VR, 2014/06/30 Quit if ERROR occured during loading data Run
// Last Modified: JB, 2014/07/07 ResetDaqEvent, ReadTriggersFromFrame
// Last Modified: VR, 2014/07/13 correct a bug in acqNo and frNo calculation in HasData( int aTriggerNb) and change the return value
// Last Modified: AP, 2014/10/16 Added flag to choose between Mi26/28 and FSBB redout. Splitted function ReadPixelsFromFrame in two,
//                               dependening on the variable TypeFlag: 0 use ReadPixelsFromFrame_Mi26
//                                                                     1 use ReadPixelsFromFrame_FSBB
// Last Modified: JB, 2015/03/18 manage abort event, HasData, GetNextDAQEvent, ReadPixelsFromFrame_FSBB

#define AP 0

#include "TCanvas.h"
#include "TApplication.h"

#include "PXIeBoardReader.h"

#include "pxi_daq_lib_config.h" // PXI DAQ Library version configuration is made in this file 
#ifdef PXI_DAQ_LIB_VERSION_1_1 
#include "pxi_daq_lib_v.1.1/daq_lib.h" 
#include "pxi_daq_lib_v.1.1/daq_lib.c" 
#endif 
#ifdef PXI_DAQ_LIB_VERSION_1_2 
#include "pxi_daq_lib_v.1.2/daq_lib.h" 
#include "pxi_daq_lib_v.1.2/daq_lib.c" 
#endif 
#ifdef PXI_DAQ_LIB_VERSION_2_1 
#include "pxi_daq_lib_v.2.1/daq_lib.c" 
#define  PXI_DAQ_LIB_WITH_FSBB
#endif
#ifdef PXI_DAQ_LIB_VERSION_3_1 
#include "pxi_daq_lib_v.3.1/daq_lib.c" 
#define  PXI_DAQ_LIB_WITH_FSBB
#endif

ClassImp(PXIeBoardReader)
ClassImp(PXIeEvent)


// --------------------------------------------------------------------------------------

int PXIeBoardReader::test (char *path, int runNumber, int MimosaType) {
  
  // This method is kept as a test,
  // it is an exact copy of the original demo method by Gilles
  // to use the DAQ library
  //
  // Inputs are:
  //  path = the directory path to your binary files
  //  runNumber = the run number
  //
  // Find out (APP__FLoadRun) below where to change the file to read (hardcoded)
  //
  // GC 2011/03/07 (19/02/2011)
  
  APP__TContext* VPtCont = &APP__VGContext;
  SInt32 VMyVar          = 10;
  SInt32 VRet            = 0;     // Variable to store error code of functions called
  EFRIO__TFrame* VPtFrame;
  SInt32 VAcqNo;
  SInt32 VFrNo;
  
  
  /* ----------------------------------------- */
  /* Init application errors messages handling */
  /* ----------------------------------------- */
  
  ERR_FBegin             ( ( APP_VGErrFileLogLvl != 0 ) /* Enable */, (char*)APP_ERR_LOG_FILE );
  ERR_FSetFileLogLevel   ( APP_VGErrFileLogLvl   );
  ERR_FSetUserLogLevel   ( APP_VGErrUserLogLvl   );
  // ERR_FSetUserErrorFunc  ( APP_FUserErrorFunc    );
  
  /* ------------------------------------------ */
  /* Init application general messages handling */
  /* ------------------------------------------ */
  
  MSG_FBegin             ( (APP_VGMsgFileLogLvl != 0) /* Enable */,  (char*)APP_MSG_LOG_FILE );
  MSG_FSetFileLogLevel   ( APP_VGMsgFileLogLvl   );
  MSG_FSetUserLogLevel   ( APP_VGMsgUserLogLvl   );
  // MSG_FSetUserMsgFunc    ( APP_FUserMsgFunc      );
  
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
  msg  (( MSG_OUT, "sizeof (EFRIO__TRunCont) = %lu", sizeof (EFRIO__TRunCont) ));
  
  
  // --------------------------------------------------------------
  // Printf records size for alignment checking Unix / Windows
  // --------------------------------------------------------------
  
  APP__FPrintRecSzChkAlign ( (char*)"Linux" );
  
  // -----------------------------
  // Load run
  // -----------------------------
  
  printf ( "Try to load run \n" );
  
  //VRet = APP__FLoadRun ( "/Users/jeromeb/Data/cmos/mi26/30/" /* RunDir */, "RUN_" /* RunPrefix */, 30 /* RunNo */ );
  VRet = APP__FLoadRun ( path /* RunDir */, (char*)"RUN_" /* RunPrefix */, runNumber /* RunNo */ );
  printf ("return is %d\n", VRet);
  
  err_retfail ( VRet, (ERR_OUT,"Load of run failed !") );
  
  printf ( "Run loaded => Contains %d acquistions \n", VRet );
  
  // --------------------------------------------------------------
  // Printf run context record = run paramters from file RUN*.par
  // --------------------------------------------------------------
  
  EFRIO__FPrintRunContRec ( &VPtCont->ResRunCont );
  
  
  // -----------------------------
  // Allocate acquisition buffer
  // -----------------------------
  
  //New way of deciding which sensor to read
  SetASICType(MimosaType);
  
#ifdef APP_NEW_BUFFER_ALLOC
  APP__FAllocAcqBuffer (ASIC__SENSOR);
#else  
  APP__FAllocAcqBuffer ();
#endif
  //APP__FAllocAcqBuffer ();
  
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
    
    
    EFRIO__FPrintFrameRec ( VPtFrame, 3 /* PrintLevel */ );
    
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



// --------------------------------------------------------------------------------------

PXIeBoardReader::PXIeBoardReader( int boardNumber, char *configFileName, int runNumber, int triggerMode, int eventBuildingMode, bool vetoOverflow, int endian, int numberOfRows,
                                 int MimosaType) {
  
  // Trigger mode:
  // 0: mode A-0, all frame are a new event (triggerless)
  // 1: mode A-1, events are built from trigger position in frame
  // 2: mode B, use trigger as a marker for starting/ending event reading
  //
  // eventBuildingMode: (0 is default)
  //   %10=0 : event built from current frame at trigger line to next frame at trigger line-1
  //   %10=1 : event built from current frame at first line to next frame at last line
  //   /10=0 : if second trigger in same frame, extend current event accordingly
  //   /10=1 : if second trigger in same frame, built 2 differrent events or drop them both
  //   /10=2 : if second trigger in same frame, built 2 differrent events or drop second one
  //
  // vetoOverflow: (true is default)
  //    true  : event with overflow will be discarded, no pixel transmitted
  //    false : event with overflow are passed for further analysis
  //
  // JB, 2009/08/18
  // Modified: JB 2009/09/10 for statistics
  // Modified: JB 2009/09/16 for statistics
  // Modified: JB 2010/07/06 for overflow and event building management
  // Modified: JB 2011/03/14 to Use DAQlib methods and pointers
  // Modified: JB 2011/06/18 to add trigger counter
  // Modified: JB 2011/06/28 to modify msg and error log file names
  //                            configure automatically #rows
  // Modified: JB 2012/02/23 variable VetoEvent introduced
  // Modified: JB, 2012/05/12 get run time from DAQ file
  // Modified: JB, 2012/08/17 impose run number from input
  // Modified: JB, 2012/09/04 management of required frames not found
  
  cout << "*****************************************" << endl;
  cout << "    < PXIeBoardReader constructor >      " << endl; 
  cout << "*****************************************" << endl;  
  
  cout << "Creating a PXIeBoardReader" << endl;
  cout << " * for board : " << boardNumber << endl;
  cout << " * for runNumber : " << runNumber << endl;
  cout << " * with data in : " << configFileName << endl;
  cout << " * with eventBuildingMode : " << eventBuildingMode << endl;
  cout << " * with vetoOverflow : " << vetoOverflow << endl;
  cout << " * with endianess : " << endian << endl;
  cout << " * with trigger mode : " << triggerMode << endl;
  cout << " * with #rows : " << numberOfRows << endl;
#ifdef PXI_DAQ_LIB_VERSION_1_1
  cout << " * with pxi_daq_lib version 1.1, and configuration :" << endl;
  cout << "   * MAX_BLOC_NB_IN_VAR_BLOC_SZ_MODE = " << FIL__TCStreamFile_MAX_BLOC_NB_IN_VAR_BLOC_SZ_MODE << endl;
#endif
#ifdef PXI_DAQ_LIB_VERSION_1_2
  cout << " * with pxi_daq_lib version 1.2, and configuration :" << endl;
#ifdef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_BEFORE_071112
  cout << "   * APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1 is BEFORE_071112" << endl;
#endif
#ifdef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_071112_TO_220613
  cout << "   * APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1 is 071112_TO_220613" << endl;      
#endif
#ifdef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_SINCE_220613
  cout << "   * APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1 is SINCE_220613" << endl;      
#endif
  
#ifdef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC2_BEFORE_JULY_2012
  cout << "   * APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC2 is BEFORE_JULY_2012" << endl;      
#endif
#ifdef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC2_SINCE_JULY_2012
  cout << "   * APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1 is SINCE_JULY_2012" << endl;      
#endif
#endif
#ifdef PXI_DAQ_LIB_VERSION_2_1
  cout << " * with pxi_daq_lib version 2.1, and configuration :" << endl;
#ifdef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_BEFORE_071112
  cout << "   * APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1 is BEFORE_071112" << endl;
#endif
#ifdef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_071112_TO_220613
  cout << "   * APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1 is 071112_TO_220613" << endl;      
#endif
#ifdef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_SINCE_220613
  cout << "   * APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1 is SINCE_220613" << endl;      
#endif
  
#ifdef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC2_BEFORE_JULY_2012
  cout << "   * APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC2 is BEFORE_JULY_2012" << endl;      
#endif
#ifdef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC2_SINCE_JULY_2012
  cout << "   * APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1 is SINCE_JULY_2012" << endl;      
#endif
#endif
#ifdef PXI_DAQ_LIB_VERSION_3_1
  cout << " * with pxi_daq_lib version 3.1, and configuration :" << endl;
#ifdef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_BEFORE_071112
  cout << "   * APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1 is BEFORE_071112" << endl;
#endif
#ifdef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_071112_TO_220613
  cout << "   * APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1 is 071112_TO_220613" << endl;      
#endif
#ifdef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1_SINCE_220613
  cout << "   * APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1 is SINCE_220613" << endl;      
#endif
  
#ifdef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC2_BEFORE_JULY_2012
  cout << "   * APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC2 is BEFORE_JULY_2012" << endl;      
#endif
#ifdef APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC2_SINCE_JULY_2012
  cout << "   * APP_DLL__TELESCOPE_MI26_MI28_DATA_FORMAT_CC1 is SINCE_JULY_2012" << endl;      
#endif
#endif
  
  IsFSBBbis          = false;
  
  BoardNumber        = boardNumber;
  RunNumber          = runNumber; // JB 2012/08/17
  TriggerMode        = triggerMode;
  EventBuildingMode  = eventBuildingMode; // JB 2010/07/06
  VetoOverflow       = vetoOverflow; // JB 2010/07/06
  Endianness         = endian;
  NumberOfLines      = numberOfRows; // M26=576, M28=928, configurable, JB 2011/06/28
  //VetoPixel          = NULL; // default is no pixels are vetoed, JB 2012/03/11
  
  // Initialisation required by DAQ library
  APP__TContext* VPtCont = &APP__VGContext;
  SInt32 VMyVar          = 666;
  char msgFile[100], errFile[100]; // JB 2011/06/28
  sprintf( msgFile, "Results/%d/msg_run%d.txt", runNumber, runNumber);
  sprintf(msgFile,"%s", fTool.LocalizeDirName( msgFile)); // JB 2011/07/07
  sprintf( errFile, "Results/%d/errors_run%d.txt", runNumber, runNumber);
  sprintf(errFile,"%s", fTool.LocalizeDirName( errFile)); // JB 2011/07/07
  ERR_FBegin             ( ( APP_VGErrFileLogLvl != 0 ) /* Enable */, errFile );
  ERR_FSetFileLogLevel   ( APP_VGErrFileLogLvl   );
  ERR_FSetUserLogLevel   ( APP_VGErrUserLogLvl   );
  MSG_FBegin             ( (APP_VGMsgFileLogLvl != 0) /* Enable */,  msgFile );
  MSG_FSetFileLogLevel   ( APP_VGMsgFileLogLvl   );
  MSG_FSetUserLogLevel   ( APP_VGMsgUserLogLvl   );
  memset ( VPtCont, 0,  sizeof (APP__TContext) );
  VPtCont->ResRunLoaded = -1; // No run loaded
  VAcqNo = -1; // is updated to 0 the first time a frame is requested
  VFrNo = 0;
  
  // check endianness of CPU
  if( 1-endian != APP__FIsLittleEndian() ) { // DAQlib return Endian=0 when TAF consider 1
    cout << endl << "ERROR PXIeBoardReader " << BoardNumber << " incorrect ENDIANNESS " << endian;
    cout << " should be " << APP__FIsLittleEndian() << endl;
  }
  
  err_trace   (( ERR_OUT, "This is a trace message   - VMyVar=%d", VMyVar ));
  err_warning (( ERR_OUT, "This is a warning message - VMyVar=%d", VMyVar ));
  err_error   (( ERR_OUT, "This is an error message  - VMyVar=%d", VMyVar ));
  msg  (( MSG_OUT, "This is a general message - with default LogLvl = 1 - VMyVar=%d", VMyVar ));
  msg  (( MSG_OUT, "sizeof (EFRIO__TRunCont) = %lu", sizeof (EFRIO__TRunCont) ));
  APP__FPrintRecSzChkAlign ( (char*)"Linux" );
  
  ListOfEventTriggers_Frame.clear();
  ListOfEventTriggers_Line.clear();
  
  FramesToSkipAtEndOfAcqForTrig = 10; // JB 2011/06/30
  if(TypeFlag == 0)      TriggerLineOffset   =  4;  //AP, 2014/10/21
  else if(TypeFlag == 1) TriggerLineOffset   = 12;  //AP, 2014/10/21
  else                   TriggerLineOffset   =  4;  //AP, 2014/10/21
  FramesToReadInEvent = 5;                          //AP, 2014/10/21
  NextFirstLineToKeep = -1;
  NextFirstFrame      = -1;
  FirstLineToKeep     = -1;
  FirstFrame          = -1;
  LastLineToKeep      = -1;
  LastFrame           = -1;
  LastFrameDUT        = -1;
  OriginAcq           = -1;
  
  NumberOfFiles      = 0;
  CurrentFileNumber  = -1;
  
  //Data               = new unsigned int[SizeOfDaqEvent];
  ReadingEvent       = false;
  ReadTwice          = false;
  EventReady         = false;
  Overflow           = false; // JB 2009/09/08
  // Set the VetoEvent flag according to event building mode
  // JB 2012/02/23
  if( EventBuildingMode%10==1 ) { // start without veto
    VetoEvent          = false;
    PeriodsVetoOff     = 1; // reading already started
  }
  else { // start with veto, wait for a trigger to start reading
    VetoEvent          = true;
    PeriodsVetoOff     = 0;
  }
  CurrentEvent       = NULL; // Allow to know whether data are correct, JB 2009/05/26
  CurrentEventNumber = -1; // is increment to 0 at first event
  StopIfRequiredFrameNotFound = false; // JB 2012/09/04
  NoMoreData         = false;
  
  cout << "Infos : " << endl;
  cout << " * Trigger offset : " << TriggerLineOffset << endl;
  cout << " * Veto event: " << VetoEvent << endl;
  
  
  // init counters and histos for statistics
  // JB 2009/0910
  EventsCount        = 0;
  EventsOverflow     = 0;
  EventsAbortedByTrig= 0; // JB 2009/09/16
  EventsAbortedByTag = 0; // JB 2015/03/16
  EventsMissed       = 0; // JB 2012/09/04
  EventsWithBadTrigger = 0; // JB 2012/09/05
  AcqsReadFromFile   = 0; // JB 2011/03/14
  FramesReadFromFile = 0;
  FramesReadTwice    = 0;
  TriggerCount       = 0; // JB 2011/06/18
  TriggerInvalidCount= 0; // JB 2012/09/25
  for( int ib=0; ib<100; ib++) {
    NStatesInBlock[ib] = 0;
  }
  NStatesInLine = 0;
  h1BlockOccupancy = new TH1S("h1BlockOccupancy","Block state occupancy",11,-1,10);
  h1LineOccupancy  = new TH1S("h1LineOccupancy", "Line state occupancy", 11,-1,10);
  
  //Adding histogram for the distribution of over-flow of the sensor lines. One histo per sensor. 
  //AP 2014/06/03
  
  // Copy of method AddFileList
  Nacquisitions = APP__FLoadRun ( configFileName /* RunDir */, (char*)"RUN_" /* RunPrefix */, runNumber /* RunNo */ );
  if( Nacquisitions < 0) 
  {
    printf("\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    printf("PXIeBoardReader::PXIeBoardReader(...) ERROR from Board %d CAN'T LOAD FILE %sRUN_%d, erro=%d! \n", BoardNumber, configFileName, runNumber, Nacquisitions);
    printf("  This could possibly come from a wrong folder/file path, check first.\n");
    printf("  This could also come from a bad hardcoded library configuration :\n");;
    printf("    1) check the pxi_daq_lib VERSION and CONFIGURATION in file: include/pxi_daq_lib_config.h \n");
    printf("    2) then, re-compile TAF after 'make clean' \n"); 
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n");
    //return;
    //gApplication->Terminate();// VR 2014/06/30 quit TAF
  }
  printf ( " * run %d loaded \n", runNumber);
  printf ( " * contains %d acquistions (from DAQ)\n", Nacquisitions );
  //RunTimeInSec = APP__VGRunDataFile.GetTime();
  RunTimeInSec = -1; // short workaround for lib v.2.1, JB 2014/10/15
  printf ( " * was created at time %d sec.\n", RunTimeInSec);
  
  NFramesPerAcq = 1800; // default which is updated by ReadConfiguration()
  NumberOfFiles = 1; // 1 required because tested later to check a file was associated
  // Read the configuration to be able to decode the data file
  ReadConfiguration();
  // Allocate acquisition buffer
  
  //New way of deciding which sensor to read
  SetASICType(MimosaType);
  
#ifdef APP_NEW_BUFFER_ALLOC
  APP__FAllocAcqBuffer (ASIC__SENSOR);
#else  
  APP__FAllocAcqBuffer ();
#endif
  //APP__FAllocAcqBuffer ();
  
  //  AddDUTSensor(4);
  //  AddDUTSensor(5);
  //  AddDUTSensor(6);
  //  AddDUTSensor(7);
  
  cout << "*****************************************" << endl;
  cout << "    </PXIeBoardReader constructor >      " << endl; 
  cout << "*****************************************" << endl; 
  
  
}

// --------------------------------------------------------------------------------------

PXIeBoardReader::~PXIeBoardReader() {
  
  // JB, 2008/09/27
  // Modified: JB 2010/06/16, several triggers
  // Modified: JB 2011/07/18, several triggers corrected
  // Modified: JB 2012/05/03, check pointers are not null before deleting
  // Modified: JB 2012/05/04, introduced ListOfTimestamps
  
  if( CurrentEvent!=NULL) delete CurrentEvent;
  if( InputFileName!=NULL) delete InputFileName;
  if( h1LineOccupancy!=NULL) delete h1LineOccupancy;
  if( h1BlockOccupancy!=NULL) delete h1BlockOccupancy;
  ListOfTriggerPos.clear(); //JB 2010/06/16
  ListOfNextTriggerPos.clear(); // JB 2011/07/18
  ListOfFrames.clear(); //JB 2010/06/16
  ListOfTimestamps.clear(); // JB 2012/05/04
  ListOfNextTimestamps.clear(); // JB 2012/05/04
  
  ListOfEventTriggers_Frame.clear();  //AP, 2014/10/21
  ListOfEventTriggers_Line.clear();   //AP, 2014/10/21
  
#if 0
  //AP, 2014/10/21
  for(int Sensor_Idx=0;Sensor_Idx<NSensors;Sensor_Idx++) {  
    delete[] hHitMapInFrame[Sensor_Idx];
  }
  delete hHitMapInFrame;
#endif
  
	
}

// --------------------------------------------------------------------------------------

bool PXIeBoardReader::ReadConfiguration() {
  
  // Read the configuration file which contains info usefull to read data
  //
  // JB, 2009/08/18
  // Modified: JB, 2011/03/14 to use DAQ lib functions and pointers
  // Modified: MG, 2011/03/14 init ListOfLineOverflow
  // Modified: JB, 2012/08/17 ignore daq run number, but print it
  
  APP__TContext* VPtCont = &APP__VGContext;
  
  // DAQlib message
  EFRIO__FPrintRunContRec ( &VPtCont->ResRunCont );
  
  
  // Change Antonin Maire, debug, 17 oct 2014
  // On advice by Gilles Claus, following test developped.
  //
  //   Bool_t lDebug_ReadConfig = kTRUE;
  //   if(lDebug_ReadConfig){
  //       
  //         printf("%s / %s / %s : prelim check sizeof(ULong_t) ) : %d bytes\n", __FILE__, __FUNCTION__, __LINE__, sizeof(ULong_t) );
  // 
  //         int c;
  //         printf("Enter text. Include a dot ('.') in a sentence to exit:\n");
  //         do {
  //             c=getchar();
  //             putchar (c);
  //         } while (c != '.');
  // 
  // 
  //         // Force a pointer
  //         VPtCont->ResRunCont.PtZsFFrameRaw = (ULong_t) 0xFFFFFFFF;
  //         VPtCont->ResRunCont.PtFrame       = (ULong_t) 0xFFFFFFFF;      
  //         
  //         printf("PXIeBoardReader::ReadConfiguration() : re-check the values of the pointers in the struct, PtZsFFrameRaw, PtFrame\n");
  //         EFRIO__FPrintRunContRec ( &VPtCont->ResRunCont );
  //         
  //                 
  //         c= 0;
  //         printf("Enter text. Include a dot ('.') in a sentence to exit:\n");
  //         do {
  //             c=getchar();
  //             putchar (c);
  //         } while (c != '.');     
  //         
  //         VPtCont->ResRunCont.PtZsFFrameRaw = (ULong_t) 0x0;
  //         VPtCont->ResRunCont.PtFrame       = (ULong_t) 0x0; 
  //   }// end if lDebug_ReadConfig
  
  
  
  NSensors          = VPtCont->ResRunCont.ParMi26Nb;
  int daqRunNumber  = VPtCont->ResRunCont.ParRunNo;
  NFramesPerAcq     = VPtCont->ResRunCont.ParFrameNbPerAcq;
  
  
  // Init overflow count per sensor
  // JB 2011/11/02
  overflowSensor = new int[NSensors];
  for( int is=0; is<NSensors; is++) {
    overflowSensor[is] = 0;
  }
  //
  ListOfLineOverflow = new vector<int>[NSensors]; //MG 2012/02/15
  
  cout << "   sensor name (from DAQ): " << VPtCont->ResRunCont.ParMapsName << endl;
  cout << "   # sensors (from DAQ): " << NSensors << endl;
  cout << "   Run number (from DAQ): " << daqRunNumber << endl;
  cout << "   trigger mode (from DAQ): " << VPtCont->ResRunCont.ParTrigMode << endl;
  cout << "   # frames per Acq (from DAQ):" << NFramesPerAcq << endl;
  
  hLineOverFlowDist = new TH1F*[NSensors];
  double R_LineOverFlow[2];
  R_LineOverFlow[0] = 0.0 - 0.5;
  R_LineOverFlow[1] = NumberOfLines - 0.5;
#if 0
  hHitMapInFrame   = new TH2F*[NSensors];
  for(int Sensor_Idx=0;Sensor_Idx<NSensors;Sensor_Idx++) {
    hHitMapInFrame[Sensor_Idx] = new TH2F[FramesToReadInEvent];
  }
  double R_col[2];
  R_col[0] = -0.5;
  R_col[1] = NumberOfColumns - 0.5;
  double R_lin[2];
  R_lin[0] = -0.5;
  R_lin[1] = NumberOfLines   - 0.5;
#endif
  for(int Sensor_Idx=0;Sensor_Idx<NSensors;Sensor_Idx++) {
    TString HistName  = TString("hLineOverFlowDist_Sensor") + long(Sensor_Idx+1);
    TString HistTitle = TString("Number of overflow vs line number for sensor #") + long(Sensor_Idx+1);
    hLineOverFlowDist[Sensor_Idx] = new TH1F(HistName.Data(),
                                             HistTitle.Data(),
                                             NumberOfLines,
                                             R_LineOverFlow[0],
                                             R_LineOverFlow[1]);
    hLineOverFlowDist[Sensor_Idx]->SetXTitle("Line Number");
    hLineOverFlowDist[Sensor_Idx]->GetXaxis()->CenterTitle(true);
    hLineOverFlowDist[Sensor_Idx]->SetYTitle("Number of Frames");
    hLineOverFlowDist[Sensor_Idx]->GetYaxis()->CenterTitle(true);
    hLineOverFlowDist[Sensor_Idx]->SetLineColor(4);
    hLineOverFlowDist[Sensor_Idx]->SetLineWidth(2);
    cout << HistName.Data() << "   " << HistTitle.Data() << endl;
#if 0
    for(int iframe=0;iframe<FramesToReadInEvent;iframe++) {
      HistName  = TString("hHitMapInFrame_Sensor") + long(Sensor_Idx+1) + TString("_EvtFrame") + long(iframe);
      HistTitle = TString("Raw hit pixels map for sensor #") + long(Sensor_Idx+1) + TString(" in frame #") + long(iframe);
      hHitMapInFrame[Sensor_Idx][iframe] = TH2F(HistName.Data(),
                                                HistTitle.Data(),
                                                NumberOfColumns,
                                                R_col[0],R_col[1],
                                                NumberOfLines,
                                                R_lin[0],R_lin[1]);
      hHitMapInFrame[Sensor_Idx][iframe].SetXTitle("Col Number");
      hHitMapInFrame[Sensor_Idx][iframe].GetXaxis()->CenterTitle(true);
      hHitMapInFrame[Sensor_Idx][iframe].SetYTitle("Lin Number");
      hHitMapInFrame[Sensor_Idx][iframe].GetYaxis()->CenterTitle(true);
      hHitMapInFrame[Sensor_Idx][iframe].SetZTitle("");
      hHitMapInFrame[Sensor_Idx][iframe].GetZaxis()->CenterTitle(true);
      hHitMapInFrame[Sensor_Idx][iframe].SetLineColor(4);
      hHitMapInFrame[Sensor_Idx][iframe].SetLineWidth(2);
    }
#endif
  }
  
  
  
  return true;	
}

// --------------------------------------------------------------------------------------
void PXIeBoardReader::AddFileList(char *prefixFileName, int firstIndex, int endIndex, const char *suffixFileName, int MimosaType) {
  
  // THIS METHOD DOESN'T WORK AT 2011/03/14 FOR UNKNOWN REASONS,
  // it has been copied inside the constructor (with success) for now, JB.
  
  // Store the inputs to be able to read files one after the other
  // Files are expected to be numberred over four digits: 0001, 0002, 0003, ...
  // First file is assumed to be numbered 0
  // JB, 2009/08/14
  // Modified: JB, 2009/09/01 first index
  // Modified: JB, 2011/03/14 Use DAQlib method and pointers, especially endIndex=runNumber here
  
  Nacquisitions = APP__FLoadRun ( prefixFileName /* RunDir */, (char*)"RUN_" /* RunPrefix */, RunNumber /* RunNo */ );
  if( Nacquisitions < 0) {
    cout << "ERROR PXIeBoardReader: " << BoardNumber << " CAN'T LOAD FILE " << prefixFileName << "RUN_" << endIndex << ", erro=" << Nacquisitions << "!" << endl;
    return;
  }
  
  printf ( "Run %d loaded => Contains %d acquistions \n", endIndex, Nacquisitions );
  
  NumberOfFiles = 1; // 1 required because tested later to check a file was associated
  
  // following lines are useless while only one file is read
  /* CurrentFileNumber += firstIndex; // to accomadate any first index, JB 2009/09/01
   SuffixFileName = new char[10];
   PrefixFileName = new char[300];
   sprintf( PrefixFileName, "%s", prefixFileName);
   sprintf( SuffixFileName, "%s", suffixFileName);
   */
  
  // Read the configuration to be able to decode the data file
  ReadConfiguration();
  
  // Allocate acquisition buffer
#ifdef APP_NEW_BUFFER_ALLOC
  APP__FAllocAcqBuffer (ASIC__SENSOR);
#else  
  APP__FAllocAcqBuffer ();
#endif
  //APP__FAllocAcqBuffer ();
  
  if(DebugLevel>0) cout << "  --> PXIeBoardReader " << BoardNumber << " adding " << NumberOfFiles << " files like " << prefixFileName << "****" << SuffixFileName << endl;
  
}

// --------------------------------------------------------------------------------------

void PXIeBoardReader::Close() {
  
  // Closes everything needed to be
  //
  // 2008/09/27
  // Modified 2012/07/10 to close fileHeaders
  
  RawFileStream.close();
  if( fileHeaders!= NULL ) fileHeaders->Close();
  
}

// --------------------------------------------------------------------------------------

void PXIeBoardReader::SetVetoPixel( int noiseRun) {
  
  // Select the required function to veto the pixel depending on the run number.
  // JB 2012/03/11
  
  if( DebugLevel) printf( "  PXIeBoardReader board %d::SetVetoPixel with noise run number %d\n", BoardNumber, noiseRun);
  fTool.SetVetoPixel( noiseRun);
  
}

// --------------------------------------------------------------------------------------

void PXIeBoardReader::AddDUTSensor( int aSensorId) {
  
  // Add aSensorId to the list of DUT (corresponds to specific event limits).
  // Expects ID from 0 to max nb of sensors - 1.
  // ATTENTION: sensor ID is different from plane number used in TAF,
  //            here, ID is the DAQ channel.
  //
  // JB 2014/02/12
  // Modified JB 2014/03/13 Check new ID is not already in the list
  
  unsigned long iId = 0;
  while ( iId<ListOfDUTSensors.size() ) {
    if (ListOfDUTSensors.at(iId) == aSensorId) {
      return;
    }
    iId++;
  }
  
  ListOfDUTSensors.push_back( aSensorId);
  
  printf( "  PXIeBoardReader board %d: new sensor %d declared as DUT, now %ld such sensors\n", BoardNumber, ListOfDUTSensors.at(ListOfDUTSensors.size()-1), ListOfDUTSensors.size());
  
}

// --------------------------------------------------------------------------------------

bool PXIeBoardReader::IsDUTSensor( int aSensorId) {
  
  // Test if the given sensor ID matches one of the DUT sensors.
  // Expects ID from 0 to max nb of sensors - 1.
  //
  // JB 2014/02/12
  // Modified: JB 2014/12/15 bug on test condition corrected
  
  bool found = false;
  unsigned long id=0;
  
  while( !found && id<ListOfDUTSensors.size() ) {
    found = (aSensorId==ListOfDUTSensors.at(id));
    id++;
  }
  
  //  cout << "Sensor id " << aSensorId << " has been " << (found?"found":"not found") << " in the list of DUTs." << endl;
  
  return found;
  
}

// --------------------------------------------------------------------------------------

unsigned int PXIeBoardReader::SwapEndian( unsigned int data) {
  
  //
  //
  // JB 2009/08/18
  
  cout << "ERROR PXIeBoardReader: DO NOT KNOW WHAT TO DO!!!!" << endl;
  
  //return ((data & 0xFF000000)>>24) + ((data & 0x00FF0000)>>8) + ((data & 0x0000FF00)<<8) + ((data & 0x000000FF)<<24);
  data = 0;
    
  return data;
}

// --------------------------------------------------------------------------------------

bool PXIeBoardReader::ResetDaqEvent( int newAcqNo, int newFrNo) {
  
  // Reset the numbers of the current acquisition and frame read,
  //  so that the next event will be just before the one specified.
  //
  // Return false if frame or acquisition  number are not realistic 
  //  or if acquisition cannot be loaded,
  //  true otherwise.
  //
  // JB, 2012/07/10
  // Modified SS, 2012/08/01
  // Modified JB, 2012/09/05 new return false condition
  // Modified JB, 2014/07/07 when frame requested exceed max, switch to next acquisition
  
  bool returnCode = true;
  
  // Set the next frame and acquistion number as requested,
  //  but pay attention to limits.
  if( newAcqNo<0 && VFrNo<0 ) { //SS 2012.08.01 - Changed conditions from <= to <
    VAcqNo = -1;
    VFrNo  = 0;
  }
  else if( newAcqNo>=Nacquisitions ) {
    cout << "WARNING: PXIeBoardReader board " << BoardNumber << ", request for an Acquisition number " << newAcqNo << " larger than the maximum " << Nacquisitions << ", resetting to 1st acq and 1st frame." << endl;
    returnCode = false;
    VAcqNo = -1;
    VFrNo  = 0;
  }
  else if( newFrNo>=NFramesPerAcq ) {
    cout << "WARNING: PXIeBoardReader board " << BoardNumber << ", request for a Frame number " << newFrNo << " larger than the maximum " << NFramesPerAcq << ", resetting to 1st frame." << endl;
    returnCode = false;
    VAcqNo = newAcqNo+1;
    VFrNo  = -1; // will be incremented by 1 when reading
  }
  else {
    VAcqNo = newAcqNo;
    VFrNo  = newFrNo-1;
  }
  // Ensure we get the next Acquisition
  if( APP__FGotoAcq ( VAcqNo /* AcqNo */, NULL /* PtFrameNb */ ) < 0 ) {
    returnCode = false;
    cout << "ERROR: PXIeBoardReader " << BoardNumber << ", can't get acquisition " << VAcqNo << "!" << endl << endl;
  }
  
  // Reset flags to restart next event from scratch
  
  ReadingEvent        = false;
  NextFirstFrame      = -1;
  NextFirstLineToKeep = -1;
  NextLastLineToKeep  = -1;
  ReadTwice           = false;
  
  
  if( DebugLevel ) printf( "  PXIeBoardReader board %d::ResetDaqEvent() reading of DAQ file reset to acquisition %d and  frame %d, proceed ? %s.\n", BoardNumber, VAcqNo, VFrNo, returnCode?"yes":"NO");
  
  return returnCode;
}

// --------------------------------------------------------------------------------------

void PXIeBoardReader::ResetReading( ) {
  
  // Reset the acq and event counters, so as to restart with first event at next reading
  //  
  // JB 2015/03/02
  
  VAcqNo = -1;
  VFrNo  = 0;
  
  // Reset flags to restart next event from scratch
  
  ReadingEvent        = false;
  NextFirstFrame      = -1;
  NextFirstLineToKeep = -1;
  NextLastLineToKeep  = -1;
  ReadTwice           = false;
  
  
  if( DebugLevel ) printf( "  PXIeBoardReader board %d::ResetReading() next event shall be first one", BoardNumber);
  
  
}

// --------------------------------------------------------------------------------------

bool PXIeBoardReader::GetNextDaqEvent( ) {
  
  // Read one Daq Frame from the raw data file
  //  i.e. one frame for each sensor read by this board
  //
  // The pointer VPtFrame to the frame data is updated
  //
  // return "false" if either no more data either required frame not there,
  ///   "true" otherwise (correct data present)
  //
  // Two counters control which Acq/Frame will be read next:
  //  VAcqNo = current acquisition number,
  //  VFrNo  = current frame number in the current acquisition.
  // The next frame will be VFrNo+1 in acquisition VAcqNo,
  //  or 0 in acquisition VAcqNo+1 if no more frames in VAcqNo.
  //
  // JB, 2009/14/08
  // Modified: JB, 2011/03/14 Use DAQlin method and pointers
  // Modified: JB, 2011/06/28 add debug printout in MSG file
  // Modified: JB, 2012/05/04 set origin of acquisition numbering
  // Modified: JB, 2012/09/04 management of required frames not found
  // Modified: JB, 2015/03/18 manage abort event
  
  bool searchFrame = true;
  VPtFrame = NULL;
  AbortEvent = false; // JB 2015/03/16
  int currentFrNo = VFrNo;
  int currentAcqNo = VAcqNo;
  
  // Try to find the next existing frame
  // either by incrementing the frame number
  // or incrementing the acquisition number if frames of previous acq. are exhausted.
  // If last frame of last acquisition is reached, stop.
  // Note that when VAcqNo==-1, a new acquisition (0) is forced
  if(DebugLevel>2) printf( "  PXIeBoardReader board %d::GetNextDaqEvent() current frame %d over %d in current acquisition %d over %d\n",
                          BoardNumber,
                          VFrNo,
                          NFramesPerAcq,
                          VAcqNo,
                          Nacquisitions);
  
  while( searchFrame ) { // search for a frame
    
    // try the next frame in the current acquisition
    if( VAcqNo != -1 && VFrNo < NFramesPerAcq-1 ) {
      VFrNo++;
      //      if ( VFrNo-currentFrNo != 1 ) { // a frame was skipped, stop !, JB 2015/03/17
      //        searchFrame = false;
      //      }
    }
    // try the first frame in the next acquisition
    else if( VAcqNo < Nacquisitions-1 ) {
      AcqsReadFromFile++;
      VAcqNo++;
      VFrNo = 0;
      if( APP__FGotoAcq ( VAcqNo /* AcqNo */, NULL /* PtFrameNb */ ) < 0 ) {
        cout << "ERROR: PXIeBoardReader " << BoardNumber << ", can't get acquisition " << VAcqNo << "!" << endl << endl;
        VFrNo = NFramesPerAcq-1; // trick to force incrementation to next acq
      }
    }
    // no more acquisition neither frame
    else {
      printf( "  PXIeBoardReader board %d: No more data to read, last frame %d of last acquisition %d in file, STOPPING!\n", BoardNumber, VFrNo, VAcqNo);
      searchFrame = false;
      NoMoreData  = true;
    }
    
    if( searchFrame ) { // test the found frame if any
      VPtFrame = APP__FGetFramePt ( VFrNo /* FrameIdInAcq */ );
      //EFRIO__TFrameHeader* VPtHead = &VPtFrame->Header;
      //printf("debug1::%d\n",VPtHead->AMapsFrameCnt[0]); // SS debug
      if ( VPtFrame == NULL) { // failure
        if(DebugLevel>1) cout << "WARNING: PXIeBoardReader " << BoardNumber << ", can't get frame " << VFrNo << " from acquisition " << VAcqNo << "!" << endl << endl;
        if( StopIfRequiredFrameNotFound && !NoMoreData ) searchFrame = false; // to force STOP if the required frame is not found, JB 2012/09/04
      }
      else { // success
        FramesReadFromFile++;
        searchFrame = false;
        // Set the origin of Acq count if not set yet, JB 2012/05/04
        if( OriginAcq==-1 ) {
          OriginAcq = VAcqNo;
        }
        EFRIO__TFrameHeader* VPtHead = &VPtFrame->Header;
        //cout << "header tag = " << VPtHead->Tag << endl;
          // What does the following condition means??? VPtHead->Tag is unsigned int !!!
        if ( VPtHead->Tag == -1 ) { // Header Tag field indicates abort needed, JB 2015/03/16
          AbortEvent = true;
          if(DebugLevel>1) printf(  "PXIeBoardReader board %d: frame %d in acquisition %d has tag==%d ABORT event.\n", BoardNumber, VFrNo, VAcqNo, VPtHead->Tag);
          //          VPtFrame = NULL;
        }
        //VFrNo = VPtHead->FrameIdInAcq;
        if(DebugLevel>1) {
          printf( "  PXIeBoardReader board %d: Got frame %d(=%d) (absolute nb = %d) in acquisition %d(=%d).\n", BoardNumber, VFrNo, VPtHead->FrameIdInAcq, VPtHead->AMapsFrameCnt[0], VAcqNo, VPtHead->AcqId);
          EFRIO__FPrintFrameRec ( VPtFrame, 4 /* PrintLevel */ ); // for debugging in MSF file, JB 2011/06/28
        }
      }
    } // end test the found frame if any
    
  } // end search for a frame
  
  
  //  return (VPtFrame != NULL);
  return (VPtFrame != NULL && !AbortEvent);
  
}

// --------------------------------------------------------------------------------------

void PXIeBoardReader::SkipNextEvent() {
  
  // This method is used to ignore the next event
  //
  // JB, 2009/05/26
  
  if(DebugLevel) printf("  PXIeBoardReader board %d::SkipNextEvent Resetting readout because of bad event\n", BoardNumber);
  ReadingEvent       = false;
  //CurrentEventNumber = 0;
  //CurrentFrameNumber = 0;
  //CurrentTimestamp   = 0;
  CurrentEvent       = 0; // Allow to know wether data are correct, JB 2009/05/26
  
}

// --------------------------------------------------------------------------------------

bool PXIeBoardReader::CheckTriggerValidity() {
  
  // Check the trigger information in the header are valid
  //  according to criteria that have to be defined.
  //
  // JB 2012/07/10
  // Modified JB 2012/08/30, correct the new trigger info coding (%1024)
  // Modified JB 2012/09/25, count also invalid triggers
  
  EFRIO__TFrameHeader* VPtHead = &VPtFrame->Header;
  bool check = true;
  SInt16 ViTrig = 0;
  
  switch(TypeFlag) {
    case 0:
    default:
      //Mi26/28 readout
      
      // Exclude trigger in the 10 last frames of the acquisition
      // JB 2011/06/30
      if( VPtHead->FrameIdInAcq > NFramesPerAcq-FramesToSkipAtEndOfAcqForTrig ) {
        if(DebugLevel>1) printf("                    trigger[%d] at frame %d found -> ignoring all triggers in the last %d frames of the acquisition\n", ViTrig, VPtHead->FrameIdInAcq, FramesToSkipAtEndOfAcqForTrig);
        check = false;
      }
      
      // According to G.Claus documentation mi28_beam_test_template_files_v1.0.txt
      // event with triggers at line 0 shall be discarded
      // bug pxi_daq_V1.0 - 22/06/2011
      // JB 2011/06/28
      // Check only ViTrig<3 because array AMapsTrigInfo does not exceed this size, JB 2013/05/22
      for ( ViTrig=0; ViTrig < VPtHead->TriggerNb && ViTrig<3 ; ViTrig++ ) {
        if( VPtHead->AMapsTrigInfo[ViTrig]%1024==0 ) {
          if(DebugLevel>1) printf("                    trigger[%d] at line %d found in VPtHead->AMapsTrigInfo -> ignoring all trigger info for this event\n", ViTrig, VPtHead->AMapsTrigInfo[ViTrig]%1024);
          check = false;
          break;
        }
      }
      
      if( !check ) {
        TriggerInvalidCount += VPtHead->TriggerNb; // JB 2012/09/25
        EventsWithBadTrigger++; // JB 2012/09/05
      }
      
      break;
      
    case 1:
      //FSBB readout
      if(VPtHead->TriggerNb == 0)   check = false;
      if(VPtHead->TriggerNb > 1000) check = false;
      
      break;
      
  }
  
  return check;
}

// --------------------------------------------------------------------------------------

bool PXIeBoardReader::GetTriggerList() {
  
  // Extract the trigger positions (i.e. line read when trigger arrived)
  //  and store them in a temporary list.
  // Return trigger validity (see method CheckTriggerValidity)
  //
  // consider TLU triggers only.
  //
  // JB 2012/09/25
  
  bool rc = false;
  
  // -+-+- Pointers to useful frame information
  EFRIO__TFrameHeader* VPtHead = &VPtFrame->Header;
  EFRIO__TTriggerRec*  VPtTrig = (EFRIO__TTriggerRec*) ( (UInt8*) VPtFrame + VPtFrame->TrigRecOffset );
  SInt16               ViTrig;
  EFRIO__TFlexRioTimeStamp1 VTs;
  
  switch(TypeFlag) {
    case 0:
    default:
      //Mi26/28 readout
      if( !ReadTwice && VPtHead->TriggerNb > 0 ) { // if trigger info
        
        rc = CheckTriggerValidity(); // removing warning: separating assignment and condition BH 2013/08/20
        if( rc ) { // if trigger info valid
          
          ListOfNextTriggerPos.clear(); // JB 2011/07/18
          ListOfNextTimestamps.clear(); // JB 2012/05/04
          
          TriggerCount += VPtHead->TriggerNb; // JB, 2011/06/18
          
          for ( ViTrig=0; ViTrig < VPtHead->TriggerNb; ViTrig++ ) { // loop on trigger bank
            
            VTs.W32 = (UInt32)VPtTrig->ATrig[(2 * ViTrig) + 1];
            if(DebugLevel>1) printf("                    trigger[%d] at line %.4d+%.1d or %.4d (word %x)\n",
                                    ViTrig,
                                    VTs.F.Mi26Line,
                                    TriggerLineOffset,
                                    (ViTrig<3?(VPtHead->AMapsTrigInfo[ViTrig])%1024:-1),
                                    VTs.W32);
            
            if (VTs.F.Mi26Line==0 || VTs.F.Mi26Line != (VPtHead->AMapsTrigInfo[ViTrig])%1024){
              //SS 2011.11.07 - curing triggers at line 0 or not syncronous to AMapsTrigInfo with the data from AMapsTrigInfo
              if(ViTrig<300) { // limit enlarged from 3 to 300, JB 2013/05/22
                if(DebugLevel>1) printf("                    trigger[%d] that was at line %.4d+%.1d is changed to %.4d+%.1d\n",
                                        ViTrig,
                                        VTs.F.Mi26Line,
                                        TriggerLineOffset,
                                        (VPtHead->AMapsTrigInfo[ViTrig])%1024,
                                        TriggerLineOffset);
                VTs.F.Mi26Line=(VPtHead->AMapsTrigInfo[ViTrig])%1024; //SS 2012.08.06 - seems that this assignment doesn't work at all. Wrong trigger information is not updated. To verify!!!
                //SS 2012.08.08 - added %1024 to trigger info from the AMapsTrigInfo to cut usefull part	
              }
              else{
                if(DebugLevel>1) printf("                    trigger[%d] is discarded\n", ViTrig);
                continue;
              }
            }
            
            ListOfNextTriggerPos.push_back( VTs.F.Mi26Line + TriggerLineOffset ); // JB 2011/07/18
            ListOfNextTimestamps.push_back( ComputeTimestamp( VPtHead->AcqId, VPtHead->FrameIdInAcq, VTs.F.Mi26Line + TriggerLineOffset ) ); // JB 2012/05/04
            
          }  // end loop on trigger bank
          
        } // end if trigger info valid
        
      } // end if trigger info
      break;
      
    case 1:
      //FSBB readout
      
#if AP
      if(DebugLevel>1) {
        cout << "VPtHead->TriggerNb = " << VPtHead->TriggerNb << " for frame " << VPtHead->FrameIdInAcq << endl;
      }
#endif
      
      if(!ReadingEvent) {
        //GGG
        if(CurrentEventNumber+1 == 209470 && 
           RunNumber == 35510) NTriggersInEvent = 0;
        else {
          NTriggersInEvent = VPtHead->TriggerNb;
          if(NTriggersInEvent == -1) NTriggersInEvent = 0;
        }
      }
      
      ListOfNextTriggerPos.clear();
      ListOfNextTimestamps.clear();
      
      if(VPtHead->TriggerNb > 0 ) { // if trigger info
        
        rc = CheckTriggerValidity(); // removing warning: separating assignment and condition BH 2013/08/20
        if( rc ) { // if trigger info valid
          
#if AP
          if(DebugLevel>1) {
            cout << "Trigger of frame " << VPtHead->FrameIdInAcq << " is valid!!!" << endl;
          }
#endif
          
          ListOfNextTriggerPos.clear(); // JB 2011/07/18
          ListOfNextTimestamps.clear(); // JB 2012/05/04
          
          if(!ReadingEvent) {
            ListOfEventTriggers_Frame.clear();
            ListOfEventTriggers_Line.clear();
          }
          
          TriggerCount += VPtHead->TriggerNb; // JB, 2011/06/18
          
          //Only stock up to 3 triggers in the frame headers:
          int LoopTriggerMax = VPtHead->TriggerNb;
          if(LoopTriggerMax > 3) LoopTriggerMax = 3;
          
#if AP
          if(DebugLevel>1) {
            cout << "Reading " << LoopTriggerMax << " triggers of frame " << VPtHead->FrameIdInAcq << endl;
          }
#endif
          
          for ( ViTrig=0; ViTrig < LoopTriggerMax; ViTrig++ ) { // loop on trigger bank
            
#if AP
            if(DebugLevel>1) {
              cout << "Looking at trigger " << ViTrig << " of frame " << VPtHead->FrameIdInAcq << endl;
            }
#endif
            
            VTs.W32 = (UInt32)VPtTrig->ATrig[(2 * ViTrig) + 1];
            //if(DebugLevel>1) printf("                    trigger[%d] at line %.4d+%.1d or %.4d (word %x)\n",
            //		  ViTrig,
            //		  VTs.F.Mi26Line,
            //		  TriggerLineOffset,
            //		  (ViTrig<3?(VPtHead->AMapsTrigInfo[ViTrig])%1024:-1),
            //		  VTs.W32);
            
            //if (VTs.F.Mi26Line==0 || VTs.F.Mi26Line != (VPtHead->AMapsTrigInfo[ViTrig])%1024){
            //SS 2011.11.07 - curing triggers at line 0 or not syncronous to AMapsTrigInfo with the data from AMapsTrigInfo
            //if(ViTrig<300) { // limit enlarged from 3 to 300, JB 2013/05/22
            //if(DebugLevel>1) printf("                    trigger[%d] that was at line %.4d+%.1d is changed to %.4d+%.1d\n",
            //		  ViTrig,
            //		  VTs.F.Mi26Line,
            //		      TriggerLineOffset,
            //		      (VPtHead->AMapsTrigInfo[ViTrig])%1024,
            //		      TriggerLineOffset);
            //VTs.F.Mi26Line=(VPtHead->AMapsTrigInfo[ViTrig])%1024; //SS 2012.08.06 - seems that this assignment doesn't work at all. Wrong trigger information is not updated. To verify!!!
            //SS 2012.08.08 - added %1024 to trigger info from the AMapsTrigInfo to cut usefull part	
            //}
            //else{
            //if(DebugLevel>1) printf("                    trigger[%d] is discarded\n", ViTrig);
            //continue;
            //}
            //}
            
            ListOfNextTriggerPos.push_back(int( (VPtHead->AMapsTrigInfo[ViTrig] & 0x3FFF) / 32 ));
            ListOfNextTimestamps.push_back( ComputeTimestamp( VPtHead->AcqId, VPtHead->FrameIdInAcq, VTs.F.Mi26Line) );
            
            if(!ReadingEvent) {
              ListOfEventTriggers_Frame.push_back(int( (VPtHead->AMapsTrigInfo[ViTrig] & 0xC000) >> 14 ));
              ListOfEventTriggers_Line.push_back( int( (VPtHead->AMapsTrigInfo[ViTrig] & 0x3FFF) / 32 ));
            }
            
#if AP
            if(DebugLevel>1) {
              cout << "Trigger " << ViTrig << ", Position = " << VTs.F.Mi26Line 
              << ", Time-stamp = " << ComputeTimestamp( VPtHead->AcqId, VPtHead->FrameIdInAcq, VTs.F.Mi26Line)
              << endl;
            }
#endif
            
          }  // end loop on trigger bank
          
        } // end if trigger info valid
        
      } // end if trigger info
      
#if AP
      if(DebugLevel>1) {
        cout << "ListOfNextTriggerPos.size() = " << ListOfNextTriggerPos.size() << endl;
        cout << "ListOfNextTimestamps.size() = " << ListOfNextTimestamps.size() << endl;
      }
#endif
      
      break;
      
  }
  
  
  return rc;
}

// --------------------------------------------------------------------------------------

bool PXIeBoardReader::ReadTriggersFromFrame() {
  
  // Read the frame information related to the triggers
  // to decide when to start building or not an event.
  // This "building" is called "reading" in the code and 
  // is marked by the flag ReadingEvent which is returned 
  // by the function
  //
  // The decision depends on the trigger mode (TriggerMode).
  // 0: mode A-0, all frames are a new event (triggerless)
  // 1: mode A-1, events are built from trigger position in frame
  // 2: mode B, use trigger as a marker for starting/ending events reading
  // 3: mode A-2, all frames start a new event (triggerless), 
  //      but the event is 2 frames long
  //
  //
  // The decision is reprensented by the following limits for storing pixels:
  // o first line of first frame,
  // o last line of last frame, defined in method DefineEventEnd().
  //
  // Note that the decision to end the event reading or building is taken
  // in the method ReadPixelsFromFrame.
  //
  // IMPORTANT NOTICE
  // do not call for the next frame by indicating a number,
  // just check the next frame is the expected one.
  //
  // First part of the original old "DecodeFrame" method,
  // now based on the DAQlib methods.
  //
  // JB 2011/03/14
  // Modified: JB 2011/06/18 to add trigger counter
  // Modified: JB 2011/06/27 switch from VPtTrig->TrigNb to VPtHead->TriggerNb
  //                         and check bug pxi_daq_V1.0 - 22/06/2011
  // Modified: JB 2011/06/30 ignore trigger in the last 10 frames of the acquisition
  // Modified: JB 2011/07/18 correct multi-triggers storage
  // Modified: SS 2011/11/04 management of "bug" in trigger array read from DAQ
  // Modified: JB 2012/02/23 introduce trigger mode 2
  // Modified: JB 2012/05/04 include timestamp management
  // Modified: JB 2012/07/10 use new method to check trigger validity
  // Modified: JB 2012/09/25 use new GetTriggerList method, also for TriggerMode=0
  // Modified: JB 2013/06/22 include properly StopIfRequiredFrameNotFound condition
  // Modified: JB 2013/07/20 new trigger mode 3
  // Modified: JB 2013/07/20 new trigger mode 3
  // Modified: JB 2014/07/07 bug corrected for triger mode 3
  
  // -+-+- Pointers to useful frame information
  EFRIO__TFrameHeader* VPtHead = &VPtFrame->Header;
  
  if(DebugLevel>2) printf("  PXIeBoardReader board %d: Reading trigger from frame AcqId = %.4d - FrameIdInAcq = %.4d - TrigNb present = %.4d - FrCnt = %.8d.\n", 
                          BoardNumber, 
                          VPtHead->AcqId, 
                          VPtHead->FrameIdInAcq, 
                          VPtHead->TriggerNb, 
                          VPtHead->AMapsFrameCnt[0]);
  if(DebugLevel>1 && ReadingEvent ) {
    switch(TypeFlag) {
      case 0:
      default:
        //Mi26/28 readout
        printf("       frame %d: EventReading: %d from frame %d - line %d to frame %d - line %d with #Triggers %d\n", 
                                           VPtHead->FrameIdInAcq,
                                           ReadingEvent,
                                           FirstFrame,
                                           FirstLineToKeep,
                                           LastFrame,
                                           LastLineToKeep,
                                           (int)ListOfTriggerPos.size());
        break;
        
      case 1:
        //FSBB readout
        printf("       frame %d: EventReading: %d from FSBB_frame %d - line %d to FSBB_frame %d - line %d with #Triggers %d\n", 
               VPtHead->FrameIdInAcq,
               ReadingEvent,
               FSBB_FirstFrame,
               FirstLineToKeep,
               FSBB_LastFrame,
               LastLineToKeep,
               (int)ListOfTriggerPos.size());
    }
  }
  
  // ===============================================
  if( ReadTwice) { // this is the second reading, so we re-init the flag
    ReadTwice = false;
    if(DebugLevel>1) printf("                  ** Second and last reading!\n");
  }
  
  // ===============================================
  // In trigger mode A-0:
  //  any frame is considered as a single event,
  // start the event right away.
  // ===============================================
  if( TriggerMode == 0 ) { // trigger mode A-0
    //TriggerMode 0 the same for both Mi26/28 and FSBB readout
    
    NextFirstFrame      = VPtHead->FrameIdInAcq;
    NextFirstLineToKeep = 0;
    NextLastLineToKeep  = -1; // JB 2011/07/25
    ListOfFrames.push_back( FirstFrame );
    GetTriggerList(); // JB 2012/09/25
    ListOfTriggerPos.insert( ListOfTriggerPos.begin(), ListOfNextTriggerPos.begin(), ListOfNextTriggerPos.end()); // JB 2012/09/25
    ListOfTimestamps.push_back( ComputeTimestamp( VPtHead->AcqId, VPtHead->FrameIdInAcq, FirstLineToKeep) ); // JB 2012/05/04
    StartEventReading();
    
  } // end trigger mode A-0
  
  
  // ===============================================
  // In trigger mode A-1:
  //   first test if the frame is the one expected to start an event,
  //   if we are already reading, store the current frame in the list attached to the event.
  //   then look for a potential new trigger position information.
  // ===============================================
  else if( TriggerMode == 1 ) { // trigger mode A-1 with trigger info
    
    switch(TypeFlag) {
      case 0:
      default:
        //Mi26/28 readout
        
        // Test the condition to start the reading
        // = we are not yet reading AND the current frame is the first frame expected.
        // We don't need to define the reading end limit, it will be done at the frame when the reading starts
        // using both information: when the event started and the trigger info of this next frame.
        if( !ReadingEvent && VPtHead->FrameIdInAcq == NextFirstFrame ) {
          //VPtHead->AMapsFrameCnt[0];
          //ListOfTriggerPos.push_back( FirstLineToKeep );
          ListOfTriggerPos.insert( ListOfTriggerPos.begin(), ListOfNextTriggerPos.begin(), ListOfNextTriggerPos.end()); // JB 2011/07/18
          ListOfFrames.push_back( NextFirstFrame );
          ListOfTimestamps.insert( ListOfTimestamps.begin(), ListOfNextTimestamps.begin(), ListOfNextTimestamps.end()); // JB 2012/05/04
          StartEventReading();
          
#if AP
          cout << "Started definition of event " << CurrentEventNumber << ". In frame " << VPtHead->FrameIdInAcq
          << " . Initial definiton of event from frame " << FirstFrame << " up to frame " << LastFrame 
          << ". NextFirstFrame = " << NextFirstFrame 
          << ", NextFirstLineToKeep = " << NextFirstLineToKeep 
          << ", FirstLineToKeep = " << FirstLineToKeep
          << ", FirstFrame = " << FirstFrame
          << ", FirstLineToKeep = " << FirstLineToKeep
          << ", LastFrame = " << LastFrame
          << ", LastLineToKeep = " << LastLineToKeep
          << endl;
#endif
        }
        
        // If reading, attach the current frame to the event.
        // Also, check if we are in the conditions calling for a second reading.
        //else 
        if( ReadingEvent ) { // if reading
          ListOfFrames.push_back( VPtHead->FrameIdInAcq );
          if( LastFrame==VPtHead->FrameIdInAcq ) {
            CheckSecondReading();
#if AP
            cout << "Event " << CurrentEventNumber << ". In frame " << VPtHead->FrameIdInAcq
            << " . Initial definiton of event from frame " << FirstFrame << " up to frame " << LastFrame 
            << ". NextFirstFrame = " << NextFirstFrame 
            << ", NextFirstLineToKeep = " << NextFirstLineToKeep 
            << ", FirstLineToKeep = " << FirstLineToKeep
            << ", FirstFrame = " << FirstFrame
            << ", FirstLineToKeep = " << FirstLineToKeep
            << ", LastFrame = " << LastFrame
            << ", LastLineToKeep = " << LastLineToKeep
            << endl;
#endif
          }
        }  // else if reading
        
        // Read any potential trigger information,
        // If any info, sets the limits to start the next event.
        // DO NOT READ THIS INFO IF THE FRAME IS TO BE READ TWICE,
        // because it will be considered at the second reading!
        // Use new method GetTriggerList, JB 2012/09/25
        if ( GetTriggerList() ) { // if valid trigger info
          
          int trigLine;
#if AP
          cout << "Event " << CurrentEventNumber << ", found " << ListOfNextTriggerPos.size() << " triggers in header of frameIdInAcq  " << VPtHead->FrameIdInAcq 
          << " (frame counter = " << VPtHead->AMapsFrameCnt[0] << ")" << endl;
#endif
          for( Int_t iTrig=0; iTrig<(Int_t)ListOfNextTriggerPos.size(); iTrig++) {
            // removing warning: adding cast because of comparison between 'Int_t' (aka 'int') and 'size_type' (aka 'unsigned long') BH 2013/08/20
            
            trigLine = ListOfNextTriggerPos.at(iTrig);
            // update the first line to read if new trigger is before or if not set yet
            if( trigLine < NextFirstLineToKeep || NextFirstLineToKeep < 0 ) {
              NextFirstLineToKeep = trigLine; // + TriggerLineOffset removed because already in GetTriggerList(), JB 2013/06/22
            }
            // update the last line to read if new trigger is after or if not set yet
            if( trigLine > NextLastLineToKeep || NextLastLineToKeep < 0 ) {
              NextLastLineToKeep = trigLine + TriggerLineOffset - 1;
            }
            
#if AP
            cout << "Event " << CurrentEventNumber << ", trigger " << iTrig+1 
            << " found at line  = " << trigLine << endl;
#endif
            
          }
          
          //} // end if valid trigger info, moved below JB 2013/06/22
          
          // Set the first frame of the next event to read according to the first line number
          //     if the position is >= NumberOfLines, this trigger concerns the next to next frame,
          //     if the position is < NumberOfLines, this trigger concerns the next frame,
          NextFirstFrame = (VPtHead->FrameIdInAcq + 1)%NFramesPerAcq;
          if( NextFirstLineToKeep >= NumberOfLines ) {
            NextFirstFrame       = (NextFirstFrame + 1)%NFramesPerAcq;
          }
          NextFirstLineToKeep  = NextFirstLineToKeep%NumberOfLines; // SS 2011.11.04 - cutting line number > NumberOfLines when passing to the next to next frame
          FirstLineToKeep  = NextFirstLineToKeep; //SS 2011.11.07 - I think it is useless here. FirstLineToKeep is anyway defined in StartEventReading()
          
#if AP
          cout << "Event " << CurrentEventNumber << ". NextFirstFrame = " << NextFirstFrame 
          << ", NextFirstLineToKeep = " << NextFirstLineToKeep 
          << ", FirstLineToKeep = " << FirstLineToKeep 
          << ", NextLastLineToKeep = " << NextLastLineToKeep 
          << ", FirstFrame = " << FirstFrame
          << ", FirstLineToKeep = " << FirstLineToKeep
          << ", LastFrame = " << LastFrame
          << ", LastLineToKeep = " << LastLineToKeep
          << endl;
#endif
          
          if(DebugLevel>1) printf("                    Next event will start at frame %d - line %d\n", NextFirstFrame, NextFirstLineToKeep);
          
        } // end if valid trigger info (move down here, JB 2013/06/22)
        else {
          if(DebugLevel>1) printf("                    No valid trigger info in the frame\n");
          // If we are not yet reading and no trigger is there,
          // we needed to stop reading when a specific frame was required, 
          // JB 2013/06/22
          if( !ReadingEvent && StopIfRequiredFrameNotFound ) return false;
        }
        break;
        
      case 1:
        //FSBB readout
        
        //In this readout there is no time shifting event definition as in the Mi26/28 case,
        //e.i. Each frame can contain a trigger. The 2 consecutive frames define the time window 
        //to look for the hits corresponding to this trigger.
        //But is a trigger is found in any of those 2 consecutive frames the process repeats itself
        //This has the consecuense of the possibility of events made of several consecutive frames
        //
        //In the FSBB case, there are sets of 5 frames, in which the first (frame N) contains the trigger information. 
        //This trigger information is copied on the header of each of the 5 frames (frames N+1, N+2, N+3,N+4).
        //In the header are also stored the number of triggers and up to 3 triggers.
        //If more triggers are found their position is lost
        //Then the DAcq have some dead-time, and then is free to look for the next trigger
        //Event is then initially defined this set of 5 frames
        // - Event starts trigger frame N and at line zero
        // - Event end    at frame N+4 at line NumberOfLines-1
        // This event definition can be futher defined later
        
        bool GoodTriggerList = false;
        NextFirstFrame++; // this variable is probably useless
        if(!ReadingEvent) {
          
          ZeroTriggersCounter = 0;
          
          GoodTriggerList = GetTriggerList();
          if ( GoodTriggerList ) { // if valid trigger info	  
//            if(DebugLevel>1) printf("                    Next event will start at frame %d - line %d\n", NextFirstFrame, NextFirstLineToKeep);
            
          } // end if valid trigger info (move down here, JB 2013/06/22)
          else {
            ZeroTriggersCounter++;
            
            if(DebugLevel>1) printf("                    No valid trigger info in the frame\n");
          }
          
          StartEventReading();
          
#if AP
          if(DebugLevel>1) {
            cout << "Defining Event number " << CurrentEventNumber << ":" 
            << " Starts at frame " << FirstFrame << " and line " << FirstLineToKeep
            << " Ends   at frame " << LastFrame  << " and line " << LastLineToKeep
            << endl;
            cout << "Trigger tram of Event number " << CurrentEventNumber << ":" 
            << " Starts at frame " << FSBB_FirstFrame
            << " Ends   at frame " << FSBB_LastFrame
            << endl;
          }
#endif
          
        }
        
        if(ReadingEvent && 
           VPtHead->FrameIdInAcq >=  FirstFrame && 
           VPtHead->FrameIdInAcq <=  LastFrameDUT) {
          //VPtHead->FrameIdInAcq >=  FSBB_FirstFrame && 
          //VPtHead->FrameIdInAcq <=  FSBB_LastFrame) {
          //cout << "Event " << CurrentEventNumber << " :: Valid frame to read " << VPtHead->FrameIdInAcq << " (" << FirstFrame << "," << LastFrameDUT << ")" << endl;
          ListOfFrames.push_back(VPtHead->FrameIdInAcq);
          
#if AP
          if(DebugLevel>1) {
            cout << "ReadTriggersFromFrame:: pushing-back frame " << VPtHead->FrameIdInAcq << " to the frames list." << endl;
          }
#endif
          
          if(VPtHead->FrameIdInAcq >  FSBB_FirstFrame) GoodTriggerList = GetTriggerList();
        }
        //else return false;
        
        ListOfTriggerPos.insert( ListOfTriggerPos.begin(), ListOfNextTriggerPos.begin(), ListOfNextTriggerPos.end());
        ListOfTimestamps.insert( ListOfTimestamps.begin(), ListOfNextTimestamps.begin(), ListOfNextTimestamps.end());
#if AP
        if(DebugLevel>1) {
          cout << "ListOfTriggerPos.size() = " << ListOfTriggerPos.size() << endl;
          cout << "ListOfTimestamps.size() = " << ListOfTimestamps.size() << endl;
        }
#endif
        
        if(VPtHead->FrameIdInAcq >  FSBB_FirstFrame) {
          if ( GoodTriggerList ) { // if valid trigger info	  
//            if(DebugLevel>1) printf("                    Next event will start at frame %d - line %d\n", NextFirstFrame, NextFirstLineToKeep);
            
          } // end if valid trigger info (move down here, JB 2013/06/22)
          else {
            
            ZeroTriggersCounter++;
            
            if(DebugLevel>1) printf("                    No valid trigger info in the frame\n");
            //NextFirstFrame++;
            NextFirstLineToKeep = 0;  
          }
        }
        
#if AP
        if(DebugLevel>1) {
          cout << "ZeroTriggersCounter = " << ZeroTriggersCounter << endl;
        }
#endif
        
        //if(ZeroTriggersCounter == FramesToReadInEvent) return false;
        
        break;
    }
  } // end trigger mode A-1 with trigger info
  
  
  // ===============================================
  // In trigger mode B:
  //  any frame is considered as a single event,
  // BUT event is valid only if value VetoEvent is true.
  // The VetoEvent value is switched whenever a trigger is read
  // The current frame is written in the frame list of this event.
  // The number of the period of Veto=Off is written in the trigger list of this event,
  //  so that all events from the same period have the same trigger number.
  // ===============================================
  else if( TriggerMode == 2 ) { // trigger mode B
    
    // switch the veto status if a trigger is found
    if( VPtHead->TriggerNb > 0 ) {
      TriggerCount += VPtHead->TriggerNb;
      VetoEvent = !VetoEvent;
      if ( !VetoEvent) PeriodsVetoOff++;
    }
    
    // start the event only if there is no veto
    if( !VetoEvent ) {
      NextFirstFrame      = VPtHead->FrameIdInAcq;
      NextFirstLineToKeep = 0;
      NextLastLineToKeep  = -1;
      ListOfFrames.push_back( NextFirstFrame );
      ListOfTriggerPos.push_back( PeriodsVetoOff );
      ListOfTimestamps.push_back( ComputeTimestamp( VPtHead->AcqId, VPtHead->FrameIdInAcq, 0 ) ); // JB 2012/05/04
      
      StartEventReading();
    }
    
  } // end trigger mode B
  
  
  // ===============================================
  // In trigger mode A-2:
  //  any frame is considered as an event start like in mode A-0,
  //  but the event size is 3 frames (not 1 like in mode A-0)
  // ===============================================
  else if( TriggerMode == 3 ) { // trigger mode A-2
    
    if( ReadingEvent ) { // if reading
      ListOfFrames.push_back( VPtHead->FrameIdInAcq );
    } 
    else { // else if reading
      NextFirstFrame      = VPtHead->FrameIdInAcq;
      NextFirstLineToKeep = 0;
      NextLastLineToKeep  = -1;
      if( VPtHead->FrameIdInAcq < NFramesPerAcq-FramesToSkipAtEndOfAcqForTrig ) { // check frame position is OK for an event
        ListOfFrames.push_back( VPtHead->FrameIdInAcq );
        GetTriggerList();
        ListOfTriggerPos.insert( ListOfTriggerPos.begin(), ListOfNextTriggerPos.begin(), ListOfNextTriggerPos.end()); // JB 2012/09/25
        ListOfTimestamps.push_back( ComputeTimestamp( VPtHead->AcqId, VPtHead->FrameIdInAcq, FirstLineToKeep) );
        StartEventReading();
      } // end check frame position is OK for an event
      else {
        NextFirstFrame+=1; // ensure that the next frame is increment, corrected JB 2014/07/07
        if(DebugLevel>1) printf("                    frame %d is ignored because in the last %d frames of the acquisition\n", VPtHead->FrameIdInAcq, FramesToSkipAtEndOfAcqForTrig);
      }
    }
    
  } // end trigger mode A-2
  
  
  // ===============================================
  // trigger mode not recognized
  // ===============================================
  else /*if( TriggerMode < 0 || TriggerMode > 2  )*/ {
    cout << "ERROR: PXIeBoardReader trigger mode " << TriggerMode << " not managed!" << endl;
    return false;
  }
  
  
  // -+-+- End
  
  return true; // if you got there, it is safe!
  
}

// --------------------------------------------------------------------------------------

void PXIeBoardReader::StartEventReading() {
  
  // Start the event reading:
  // - the flag EventReadingEvent is set to true,
  // - increment the event number,
  // - the information on the next frame to read is reset,
  // - define when the event should end.
  //
  // JB 2011/03/14
  // Modified: JB 2011/07/18 call DefineEventEnd before resetting NextMarkers
  // Modified: SS 2011/11/04 correction for eventBuildingMode==1
  // Modified: JB 2013/07/20 introducing trigger mode 3
  
  // ===============================================
  
  switch(TypeFlag) {
    case 0:
    default:
      //Mi26/28 readout
      
      ReadingEvent        = true;
      CurrentEventNumber++;
      FirstFrame          = NextFirstFrame;
      FirstLineToKeep     = NextFirstLineToKeep;
      if( EventBuildingMode%10 == 1 ) { //If EventBuildingMode = 1 start reading from the first line of the first frame, SS 2011.11.04
        FirstLineToKeep = 0;
      }
      DefineEventEnd();
      if( TriggerMode==3 ) {
        NextFirstFrame      = FirstFrame+1;
      }
      else {
        NextFirstFrame      = -1;
      }
      NextFirstLineToKeep = -1;
      NextLastLineToKeep  = -1;
      
      if(DebugLevel>1) printf("                  ** Reading event %d with %lu triggers!\n", CurrentEventNumber, ListOfTriggerPos.size());
      
      break;
      
    case 1:
      //FSBB readout
      
      ReadingEvent        = true;
      CurrentEventNumber++;
      
      if(TriggerMode == 0) {
        FSBB_AcqNo   = VAcqNo; // JB 2015/03/17
        FSBB_FirstFrame     = (&VPtFrame->Header)->FrameIdInAcq;
        FirstFrame          = FSBB_FirstFrame;
        FirstLineToKeep     = 0;
      }
      else {
#if AP
        if(DebugLevel>1) {
          cout << "Building event " << CurrentEventNumber << endl;
          cout << "Found " << NTriggersInEvent << " triggers" << endl;
          cout << "Print out the information of the 1st " << ListOfEventTriggers_Frame.size() << " triggers in the event" << endl;
          int LoopTriggerMax = NTriggersInEvent;
          if(LoopTriggerMax > 3) LoopTriggerMax = 3;
          for(int itrigg=0; itrigg<LoopTriggerMax; itrigg++) {
            cout << "trigger " << itrigg+1 
            << " found in frame " << ListOfEventTriggers_Frame[itrigg] 
            << " at line " << ListOfEventTriggers_Line[itrigg] 
            << endl;
          }
        }
#endif
        
        FSBB_AcqNo          = VAcqNo; // JB 2015/03/17
        FSBB_FirstFrame     = (&VPtFrame->Header)->FrameIdInAcq;
        
        //FirstFrame          = FSBB_FirstFrame;  //Frame 0
        FirstFrame          = FSBB_FirstFrame + 1; //Frame1
        
        FirstLineToKeep     = 0;
        //if( EventBuildingMode%10 == 1 ) {
        //  FirstLineToKeep = 0;
        //}
      }
      
      DefineEventEnd();
      
      if(DebugLevel>1) printf("                  ** Reading event %d with %lu triggers!\n", CurrentEventNumber, ListOfTriggerPos.size());
      
      break;
  }
  
}

// --------------------------------------------------------------------------------------

void PXIeBoardReader::DefineEventEnd() {
  
  // Define the event end with two options:
  // (1): force the start at line 0 and end at line NumberOfLines-1 on the next frame,
  // (2=default): start at trigger position and end at trigger position+NumberOfLines on the next frame
  //
  // Option is chosen according to first digit of the EventBuildingMode flag.
  //
  // JB 2011/03/14
  // Modified: JB 2011/07/18 Take into account several triggers possibility to define last line to read end
  // Modified: JB 2011/10/31 Correct bug in lastFrame when lastLine=last frame line
  // Modified: SS 2011/11/08 Correction for LastFrame
  // Modified: JB 2013/07/20 introducing trigger mode 3
  // ===============================================
  
  switch(TypeFlag) {
    case 0:
    default:
      //Mi26/28 readout
      
      // default option (2)
      // event limits depend on the trigger position
      
      // Set the last frame according to the last line number
      //     if the position is < 0, last frame is also the first one,
      //     if the position is >= NumberOfLines, last frame is the next to next one after the first frame,
      //     if the position is < NumberOfLines, last frame is the next one after the first frame.
      if( NextLastLineToKeep < 0 ) {
        LastLineToKeep = NumberOfLines-1;
        LastFrame      = NextFirstFrame; // SS 2011.11.08 NextFirstFrame is used in place of FirstFrame. FirstFrame always remains the very first frame of the event
        if( TriggerMode==3 ) LastFrame      = NextFirstFrame+1;
      }
      else {
        LastLineToKeep = NextLastLineToKeep%NumberOfLines; // modulo added to take into account limits, JB 2011/10/31
        LastFrame      = (NextFirstFrame+1)%NFramesPerAcq; //SS 2011.11.08 NextFirstFrame is used in place of FirstFrame. FirstFrame always remains the very first frame of the event
      }
      // end option (2)
      
      // option (1)
      // event limits are always first and last frame line
      // but last frame does not change
      
      if( EventBuildingMode%10 == 1 ) {
        //FirstLineToKeep = 0; // now set in StartEventReading, SS 2011.11.04
        LastLineToKeep = NumberOfLines-1;
      } // end option (1)
      
      if(DebugLevel>1) printf("                  ** the event starting at frame %d and line %d will stop at frame %d and line %d!\n", FirstFrame, FirstLineToKeep, LastFrame, LastLineToKeep);
      
      break;
      
    case 1:
      //FSBB readout
      
      // default option for FSBB is the define the last frame as FirstFrame + 5
      // LastLineToKeep is NumberOfLines-1
      
      if(TriggerMode == 0) {
        FSBB_LastFrame  = FSBB_FirstFrame;
        LastFrame       = FSBB_LastFrame;
        LastFrameDUT    = FSBB_LastFrame;
        LastLineToKeep  = NumberOfLines-1;
      }
      else {
        FSBB_LastFrame  = FSBB_FirstFrame + FramesToReadInEvent-1;
        
        //LastFrame      = FSBB_LastFrame;
        //LastFrame      = FirstFrame + 1;
        //LastFrame      = FirstFrame + 2;
        //LastFrame        = FSBB_FirstFrame + 1; //Frame 1
        LastFrame        = FSBB_FirstFrame + 2; //Frame 2
        LastFrameDUT     = LastFrame + 1;
        
        LastLineToKeep = NumberOfLines-1;
        
        //if( EventBuildingMode%10 == 1 ) {
        //FirstLineToKeep = 0; // now set in StartEventReading, SS 2011.11.04
        //LastLineToKeep = NumberOfLines-1;
        //} // end option (1)
      }
      
      if(DebugLevel>1) printf("                  ** the event starting at frame %d or FSBB_frame %d and line %d will stop at frame %d or FSBB_frame %d and line %d!\n", FirstFrame, FSBB_FirstFrame, FirstLineToKeep, LastFrame, FSBB_LastFrame, LastLineToKeep);
      break;
      
  }
  
}

// --------------------------------------------------------------------------------------

void PXIeBoardReader::CheckSecondReading() {
  
  // CALL this function ONLY when reading the LAST FRAME of an event.
  //
  // Test the condition for reading twice the current frame
  // = we are reading the last frame of the current event
  //   AND this frame is also the beginning of the next exepected event.
  //
  // Then two situations may occur:
  // a) the last line of the current event lies before the first line of the next event,
  //   --> event information are NOT intricated in the same frame part,
  // b) the last line of the current event lies before the first line of the next event
  //   --> event information ARE intricated in the same frame part,
  //
  // (a) case, we simply read the frame twice.
  //
  // (b) case, we do not read the frame twice and we have several options:
  // - EventBuildingMode/10 == 1: we abort the two events
  // - EventBuildingMode/10 == 2: we abort the second event and keep the first
  // - Otherwise(default): the current event is extended to the end of the next one => they are merged.
  //
  // JB 2011/03/14
  // Modified: JB 2011/07/18 correct multi-triggers storage
  // Modified: SS 2011/11/04 correct redefinition of event end
  // Modified: JB 2012/05/04 include timestamp management
  
  // -+-+- Pointers to useful frame information
  //EFRIO__TFrameHeader* VPtHead = &VPtFrame->Header;
  //EFRIO__TTriggerRec*  VPtTrig = (EFRIO__TTriggerRec*) ( (UInt8*) VPtFrame + VPtFrame->TrigRecOffset );;
  //SInt16               ViTrig;
  
  if( NextFirstFrame == LastFrame  ) { // test if reading twice
    FramesReadTwice++;
    if(DebugLevel>1) printf("  PXIeBoardReader board %d: event %d shares its last frame %d with next event first frame %d -> READ TWICE potential condition.\n", BoardNumber, CurrentEventNumber, LastFrame, NextFirstFrame);
    
    // -----------------
    // case (a)
    if( NextFirstLineToKeep > LastLineToKeep ) { // test if events are NOT intricated
      ReadTwice = true;
      if(DebugLevel>1) printf("                  ** This frame %d will be read twice (for the two separated events starting at frame: %d and %d)!\n", LastFrame, FirstFrame, NextFirstFrame);
    }
    
    // -----------------
    // case (b)
    else { // test if events ARE intricated
      ReadTwice = false;
      
      // ----------
      // version where both events are aborted,
      // stop current reading
      // and erase all info.
      if( EventBuildingMode/10 == 1 ) {
        EventsAbortedByTrig += 2;
        ReadingEvent        = false;
        FirstLineToKeep     = -1;
        FirstFrame          = -1;
        NextFirstLineToKeep = -1;
        NextFirstFrame      = -1;
        ListOfFrames.clear();
        ListOfTriggerPos.clear();
        ListOfTimestamps.clear(); // JB 2012/05/04
        ListOfPixels.clear();
        if(DebugLevel>1) printf("                  ** Reading stop because trigger are mixed in the current frame %d (2 events starting at %d and %d aborted)!\n", LastFrame, FirstFrame, NextFirstFrame);
      }
      
      // ----------
      // Version where only the second event is aborted,
      // we don't stop the current event
      // but just to erase the info on the next event.
      if( EventBuildingMode/10 == 2 ) {
        EventsAbortedByTrig += 1;
        NextFirstLineToKeep = -1;
        NextFirstFrame      = -1;
        if(DebugLevel>1) printf("                  ** Will not read 2nd event (starting at frame %d) because trigger are mixed in the current frame %d which is the last of the event started at frame %d (1 event aborted)!\n", NextFirstFrame, LastFrame, FirstFrame);
      }
      
      // ----------
      // Version where the frames are merged into one event
      else { // if merged events
        
        if(DebugLevel>1) printf("                  ** The event %d started at frame %d - line %d is merged with event starting at frame %d - line %d\n", CurrentEventNumber, FirstFrame, FirstLineToKeep, NextFirstFrame, NextFirstLineToKeep );
        
        // add the new triggers to the current list
        //ListOfTriggerPos.push_back( NextFirstLineToKeep );
        ListOfTriggerPos.insert( ListOfTriggerPos.begin(), ListOfNextTriggerPos.begin(), ListOfNextTriggerPos.end()); // JB 2011/07/18
        ListOfTimestamps.insert( ListOfTimestamps.begin(), ListOfNextTimestamps.begin(), ListOfNextTimestamps.end()); // JB 2012/05/04
        
        // Trick: redefine the event start from this frame and with the latest trigger,
        // note the event will not "restart" because it is already being read.
        // 	FirstLineToKeep = 0; //SS 2011.11.04 - Second frame in the merged event should be read from the beginning. //2011.11.08 - commented out completely as FirstLineToKeep remains always as it was in the very first frame
        // 	FirstFrame      = NextFirstFrame; //SS 2011.11.08
        //Lines above are not needed as DefineEventEnd() now uses the NextFirstFrame and NextFirstLineToKeep to define the end
        
        // redefine event end
        DefineEventEnd();
        
        // erase the information on the next event
        NextFirstLineToKeep = -1;
        NextFirstFrame      = -1;
        NextLastLineToKeep = -1; //SS 2011.11.07 - to have correct definition of the last line of the multi-merged event.
        
      } // end if merged events
      
    } // end test if events ARE intricated
    
  } // end test if reading twice
  
  else {
    if(DebugLevel>1) printf("  PXIeBoardReader board %d: event %d DOES NOT share its last frame %d with next event first frame %d.\n", BoardNumber, CurrentEventNumber, LastFrame, NextFirstFrame);
  }
  
}

// --------------------------------------------------------------------------------------
bool PXIeBoardReader::ReadPixelsFromFrame() {
  
  switch(TypeFlag) {
    case 0:
      // Call function to read the frame information related to the pixels for Mimosa26/28
      return ReadPixelsFromFrame_Mi26();
      break;
      
    case 1:
      // Call function to read the frame information related to the pixels for FSBB
      return ReadPixelsFromFrame_FSBB();
      break;
      
    default:
      return ReadPixelsFromFrame_Mi26();
  }
  
  return true;
  
}
// --------------------------------------------------------------------------------------
bool PXIeBoardReader::ReadPixelsFromFrame_Mi26() {
  
  // Function to read the frame information related to the pixels for Mimosa26/28
  // It expects the method ReadTriggersFromFrame() was call just before
  // to decide about event building
  //
  // Contains the line overflow management.
  //
  // Second part of the original old "DecodeFrame" method,
  // now based on the DAQlib methods.
  //
  // JB 2011/03/14
  // Modified JB 2011/10/31 condition to stop the pixel reading
  // Modified JB 2012/03/11 new pixel veto mechanism
  // Modified MG 2012/03/14 fill the line overflow vector
  
  // -+-+- Pointers to useful frame information
  EFRIO__TFrameHeader* VPtHead = &VPtFrame->Header;
  EFRIO__TFrameData*   VPtData = &VPtFrame->Data;
  UInt16               VOneMapsSzW16  = VPtData->OneMapsSz / 2;
  UInt16               VDataW16Length;
  UInt16               VLastW16;
  SInt32               ViSrcW16;
  UInt16*              VPtSrcW16;
  MI26__TStatesLine    VStatesLine;
  MI26__TState         VState;
  SInt16               ViMi26;
  SInt16               ViStatesLine;
  SInt8                ViState;
  SInt8                VStatesNbPerLine;
  char                 VStrState[255];
  char                 VStrLine[255];
  
  if(DebugLevel>2) printf("  PXIeBoardReader board %d: Reading pixel from frame AcqId = %.4d - FrameIdInAcq = %.4d - Current event %d\n", 
                          BoardNumber, 
                          VPtHead->AcqId, 
                          VPtHead->FrameIdInAcq, 
                          CurrentEventNumber );
  
  // -+-+- Start loop on sensors
  for ( ViMi26=0; ViMi26 < VPtHead->MapsNb ; ViMi26++ ) { // loop on sensors
    
    VPtSrcW16       = (UInt16*) ( (UInt16*) VPtData->ADataW32 + ( ViMi26  * VOneMapsSzW16 ) );
    VDataW16Length  = VPtHead->AMapsDataLength[ViMi26] / 2;
    ViSrcW16        = 0;
    
    // =======
    if ( VDataW16Length != 0 ) { // check there is some data to read
      
      VLastW16 = VDataW16Length - 1;
      
      ViStatesLine  = 0;
      
      // ========
      while ( ViSrcW16 < VLastW16 ) { // loop over words to read
        
        // -- StatesLine field
        
        VStatesLine.W16  = VPtSrcW16[ViSrcW16];
        VStatesNbPerLine = VStatesLine.F.StateNb;
        
        if( VStatesLine.F.Ovf > 0 ) { // detect overflow
          Overflow = true;
          overflowSensor[ViMi26] += 1; // JB 2011/11/02
          
          ListOfLineOverflow[ViMi26].push_back( VStatesLine.F.LineAddr ); //MG 2012/02/15
          //cout << "Sensor " << ViMi26 << " Line  " << VStatesLine.F.LineAddr << endl;
          hLineOverFlowDist[ViMi26]->Fill(VStatesLine.F.LineAddr); //AP 2014/06/03
          
          // Overflow disable for a given line or column due to noisy pixels
          // line 318 of the 4+1th chip, SPS 2010
          // line 338 of the 2+1th chip, DESY March 2011
          // line 253 of the 5+1th chip, DESY March 2011
          //if(
          //     (ViMi26==2 && VStatesLine.F.LineAddr==338)
          //   ||(ViMi26==5 && VStatesLine.F.LineAddr==253)
          //   ) Overflow = false;
          
          //if(EventsOverflow%1000==0 && ReadingEvent && Overflow == true) printf("WARNING : overflow while reading %d event %d at frame %d, line %d and sensor %d, total overflow number is %d\n", ReadingEvent, CurrentEventNumber, VPtHead->FrameIdInAcq, VStatesLine.F.LineAddr, ViMi26, EventsOverflow);//MG 2010/06/02
        } // end detect overflow
        
        if(DebugLevel>3) printf("                  sensor %2d line %4d, #states %d, overflow %d, reading event ? %d\n", ViMi26, VStatesLine.F.LineAddr, VStatesLine.F.StateNb, VStatesLine.F.Ovf, ReadingEvent);
        
        ++ViSrcW16;
        
        // -- States fields
        
        // Init some statistics for this line
        // JB 2009/09/10
        for( int ib=0; ib<100; ib++) {
          NStatesInBlock[ib] = 0;
        }
        NStatesInLine = 0;
        
        for ( ViState=0; ViState < VStatesNbPerLine; ViState++ ) { // loop on states
          VState.W16 = VPtSrcW16[ViSrcW16];
          
          //compute some statistics, JB 2009/09/10
          NStatesInLine++;
          NStatesInBlock[VState.F.ColAddr%64] += 1;
          
          // A state contains HitNb+1 pixels
          // the first pixel being on the left at the column ColAddr
          
          for( int iPixel=0; iPixel<VState.F.HitNb+1; iPixel++) { // loop on pixels in the state
            
            /*
             // create a new pixel only if we are reading an event
             // and if the line is inside the proper limits
             // and the pixel is not vetoed
             if(   ReadingEvent 
             && (
             (VPtHead->FrameIdInAcq == FirstFrame && VStatesLine.F.LineAddr >= FirstLineToKeep)
             || (VPtHead->FrameIdInAcq == LastFrame && VStatesLine.F.LineAddr <= LastLineToKeep)
             || (FirstFrame < VPtHead->FrameIdInAcq && VPtHead->FrameIdInAcq < LastFrame) // MG, 2010/07/06
             || (4<=ViMi26 && ViMi26<=7) // force DUT-planes JB 2014/02/11
             )
             && ( fTool.VetoPixel==NULL || !fTool.VetoPixel( ViMi26, VStatesLine.F.LineAddr, VState.F.ColAddr) ) // JB, 2012/03/11
             ) {
             */
            if( CheckPixelToAdd( ViMi26, VPtHead->FrameIdInAcq, VStatesLine.F.LineAddr, VState.F.ColAddr) ) {
              AddPixel(  ViMi26+1, 1, VStatesLine.F.LineAddr, VState.F.ColAddr+iPixel);
              //cout << "chip " << ViMi26 << " row " << VStatesLine.F.LineAddr << " col " << VState.F.ColAddr << endl;
            }
            
          } // end loop on pixels in the state
          
          if(DebugLevel>3) printf("                  state %d, #pixels %d, column %d\n", ViState, VState.F.HitNb+1, VState.F.ColAddr);
          
          ++ViSrcW16;
        } // end loop on states
        
        // Fill the statistics histograms, JB 2009/09/10
        for( int ib=0; ib<18; ib++) {
          h1BlockOccupancy->Fill( NStatesInBlock[ib]);
        }
        h1LineOccupancy->Fill( NStatesInLine);
        
        ++ViStatesLine;
        
      } // end loop over words to read
      
    }  // end check there is some data to read
    
    if(DebugLevel>2) cout << "                 # pixels so far " << ListOfPixels.size() << endl;
    
    // =======
    // Condition to stop the reading after the data of the last sensor
    //  ==LastFrame changed to >=LastFrame to prevent to loop for ever for an unexisting frame, JB 2011/10/31
    if( ViMi26 == VPtHead->MapsNb-1 && ReadingEvent && VPtHead->FrameIdInAcq >= LastFrame) {
      
      ReadingEvent   = false;
      EventReady     = true;
      LastLineToKeep = -1;
      LastFrame      = -1;
      
      if(DebugLevel>1) printf("                  ** Reading event %d stopped!\n", CurrentEventNumber);
    }
    
  } // end loop on sensors
  
  if(DebugLevel>1) cout << "                 total # pixels found " << ListOfPixels.size() << endl;
  
  // -+-+- End
  
  return true; // if you got there, it is safe!
  
}
// --------------------------------------------------------------------------------------
bool PXIeBoardReader::ReadPixelsFromFrame_FSBB() {
  
  // Function to read the frame information related to the pixels for FSBB
  // It expects the method ReadTriggersFromFrame() was call just before
  // to decide about event building
  //
  // Contains the line overflow management.
  //
  // Second part of the original old "DecodeFrame" method,
  // now based on the DAQlib methods.
  //
  // AP 2014/10/16
  // Modified: JB 2015/03/18 additional condition on AcqNo to end event
  
  //#ifdef PXI_DAQ_LIB_VERSION_2_1
#ifdef PXI_DAQ_LIB_WITH_FSBB
  
  if(NTriggersInEvent == 0 && TriggerMode != 0) {
    
#if AP
    if(DebugLevel>1) {
      cout << "ZeroTriggersCounter = " << ZeroTriggersCounter << endl;
    }
#endif
    
    if(ZeroTriggersCounter < FramesToReadInEvent) return true;
    else if(ZeroTriggersCounter == FramesToReadInEvent) {
      ReadingEvent   = false;
      EventReady     = true;
      LastLineToKeep = -1;
      LastFrame      = -1;
      
      if(DebugLevel>1) printf("                  ** Reading event %d stopped!\n", CurrentEventNumber);
      
      return true;
    }
  }
  
  
  // -+-+- Pointers to useful frame information
  EFRIO__TFrameHeader*  VPtHead = &VPtFrame->Header;
  FSBB0__TZsFFrame      VZsFFrame;
  SInt16                ViFsbb0;
  
  SInt16                ViLine;
  SInt16                ViLine2;
  SInt16                ViFirstLine;
  SInt16                ViTopLine;
  SInt16                VFirstCol;
  SInt16                VLastCol;
  SInt16                ViCol;
  SInt16                ViStatesLine;
  SInt16                ViState;
  SInt16                VStatesNbG0;
  SInt16                VStatesNbG1;
  SInt32                VHitCnt;
  SInt32                VOfvCnt;
  SInt32                VMask;
  SInt32                PtOvfCnt; 
  
  if(DebugLevel>2) printf("  PXIeBoardReader board %d: Reading pixel from frame AcqId = %.4d - FrameIdInAcq = %.4d - Current event %d\n",
                          BoardNumber,
                          VPtHead->AcqId,
                          VPtHead->FrameIdInAcq,
                          CurrentEventNumber);
  
  err_retnull ( VPtFrame, (ERR_OUT,"PtRec == NULL !") );
  memset ( &VZsFFrame, 0, sizeof (VZsFFrame) );
  
  // -+-+- Start loop on sensors
  for ( ViFsbb0=0; ViFsbb0 < VPtHead->MapsNb ; ViFsbb0++ ) { // loop on sensors
    EFRIO__FSBB0_FConvEfrioFrameToZsFFrame ( VPtFrame /* Src */, ViFsbb0 /* MapsId */, &VZsFFrame, 0 /* PrintLvl */ );
    
    // StatesRec loop
    VHitCnt = 0;
    VOfvCnt = 0;
    if ( VZsFFrame.StatesRecNb > FSBB0__ZS_FFRAME_MAX_STATES_REC ) {
      err_warning (( ERR_OUT, "StatesRecNb=%d > FSBB0__ZS_FFRAME_MAX_STATES_REC=%d => Force %d", VZsFFrame.StatesRecNb, FSBB0__ZS_FFRAME_MAX_STATES_REC, FSBB0__ZS_FFRAME_MAX_STATES_REC ));
      err_error   (( ERR_OUT, "StatesRecNb=%d > FSBB0__ZS_FFRAME_MAX_STATES_REC=%d => Force %d", VZsFFrame.StatesRecNb, FSBB0__ZS_FFRAME_MAX_STATES_REC, FSBB0__ZS_FFRAME_MAX_STATES_REC ));
      VZsFFrame.StatesRecNb = FSBB0__ZS_FFRAME_MAX_STATES_REC;
    }
    
    if (VZsFFrame.StatesRecNb > 0){
      for ( ViStatesLine=0; ViStatesLine < VZsFFrame.StatesRecNb; ViStatesLine++ ) {
        
        VStatesNbG0 = VZsFFrame.AStatesRec[ViStatesLine].NbWinG0;
        VStatesNbG1 = VZsFFrame.AStatesRec[ViStatesLine].NbWinG1;
        ViTopLine   = ((VZsFFrame.AStatesRec[ViStatesLine].StatesLine.F.SLineAddr + 1) * 4) - 1 ;
        
        // States in one StateRec loop	
        if ( VStatesNbG0 > FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP ) {
          err_warning (( ERR_OUT, "StatesNb=%d > FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_STATES_REC=%d => Force %d",
                        VStatesNbG0,
                        FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP,
                        FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP ));
          VStatesNbG0 = FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP;
          ++VOfvCnt;
        }
        
        if ( VStatesNbG1 > FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP ) {
          err_warning (( ERR_OUT, "StatesNb=%d > FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_STATES_REC=%d => Force %d",
                        VStatesNbG1,
                        FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP,
                        FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP ));
          VStatesNbG1 = FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP;
          ++VOfvCnt;
        }
        
        if ( ViTopLine >= FSBB0__MAT_DISCRI_LINES_NB) {
          err_warning (( ERR_OUT, "ViTopLine=%d >= FSBB0__MAT_DISCRI_LINES_NB=%d => Force %d",
                        ViTopLine,
                        FSBB0__MAT_DISCRI_LINES_NB,
                        FSBB0__MAT_DISCRI_LINES_NB - 1 ));
          ViTopLine = FSBB0__MAT_DISCRI_LINES_NB - 1;
        }
        
        // Decode Hits for Group 1
        if ( VStatesNbG1 > 0 ){
          for ( ViState = 0; ViState < VStatesNbG1; ViState++ ) {
            
            VFirstCol = (VZsFFrame.AStatesRec[ViStatesLine].AStatesG1[ViState].F.ColAddr) + 224;
            
            if ( VFirstCol >= FSBB0__REG_DISCRI_BIT_SZ ) {
              err_warning (( ERR_OUT, "FirstCol=%d >= FSBB0__REG_DISCRI_BIT_SZ=%d => Force 0", VFirstCol, FSBB0__REG_DISCRI_BIT_SZ ));
              VFirstCol = 0;
            }
            
            // Hits in one State loop
            ViFirstLine = ViTopLine - (VZsFFrame.AStatesRec[ViStatesLine].AStatesG1[ViState].F.Delta); 
            VMask = 0x80000;
            
            for (ViLine = ViFirstLine; ViLine > (ViFirstLine - 4); ViLine-- ){
              if (ViLine < 0){
                break;
              }
              
              for ( ViCol = VFirstCol; ViCol > VFirstCol - 5; ViCol-- ) {
                if (ViCol < 0){
                  break;
                }
                
                if ( ((VZsFFrame.AStatesRec[ViStatesLine].AStatesG1[ViState].F.Code)  & VMask) != 0){
		  //Line inversion of bug in FSBB. Needs to be removed for FSBB-bis:
		  if(!IsFSBBbis) {
		    //If TypeFlag is 1 => reading FSBB     => IsFSSbis is false => Do double line inversion
                    if(int(ViLine)%2 == 0) ViLine2 = ViLine + 1;
                    else                   ViLine2 = ViLine - 1;
		  }
		  else {
		    //If TypeFlag is 2 => reading FSBB-bis => IsFSSbis is true  => No double line inversion
		    ViLine2 = ViLine;
		  }
                  
                  if( CheckPixelToAdd( ViFsbb0, VPtHead->FrameIdInAcq, ViLine2, ViCol) ) {
                    AddPixel(  ViFsbb0+1, 1, ViLine2, ViCol);
#if AP
                    if(DebugLevel>1) {
                      cout << "PXIeBoardReader::ReadPixelsFromFrame_FSBB : chip [" << ViFsbb0 << "] = row [" << ViLine2 << "], col [" << ViCol << "]" << endl;
                    }
#endif
                  }
                }
                VMask = VMask >> 1;
                
              } // End For ViCol
            } // End For ViLine
          } // End States in one StateRec loop for G1
        } /* end decoding groups for G1 */
        
        // Decode Hits for Group 0
        if ( VStatesNbG0 > 0 ){
          for ( ViState=0; ViState < VStatesNbG0; ViState++ ) {
            
            VFirstCol = VZsFFrame.AStatesRec[ViStatesLine].AStatesG0[ViState].F.ColAddr ;
            //VLastCol  = VFirstCol + VZsFFrame.AStatesRec[ViStatesLine].AStatesG1[ViState].F.HitNb; // HitNb = 0 => 1 hit, 1 => 2 hits etc ...
            // WARNING => Will slow down execution
            
            if ( VFirstCol >= FSBB0__REG_DISCRI_BIT_SZ ) {
              err_warning (( ERR_OUT, "FirstCol=%d >= FSBB0__REG_DISCRI_BIT_SZ=%d => Force 0", VFirstCol, FSBB0__REG_DISCRI_BIT_SZ ));
              VFirstCol = 0;
            }
            
            // Hits in one State loop
            ViFirstLine = ViTopLine - (VZsFFrame.AStatesRec[ViStatesLine].AStatesG0[ViState].F.Delta); 
            VMask = 0x80000;
            
            for (ViLine = ViFirstLine; ViLine > ViFirstLine - 4; ViLine-- ){
              if (ViLine < 0){
                break;
              }
              
              for ( ViCol=VFirstCol; ViCol > VFirstCol - 5; ViCol-- ) {
                if (ViCol < 0){
                  break;
                }
                if ( (VZsFFrame.AStatesRec[ViStatesLine].AStatesG0[ViState].F.Code  & VMask) != 0){
		  if(!IsFSBBbis) {
		    //If TypeFlag is 1 => reading FSBB     => IsFSSbis is false => Do double line inversion
		    if(int(ViLine)%2 == 0) ViLine2 = ViLine + 1;
		    else                   ViLine2 = ViLine - 1;
		  }
		  else {
		    //If TypeFlag is 2 => reading FSBB-bis => IsFSSbis is true  => No double line inversion
		    ViLine2 = ViLine;
		  }
                  
                  if( CheckPixelToAdd( ViFsbb0, VPtHead->FrameIdInAcq, ViLine2, ViCol) ) {
                    AddPixel(  ViFsbb0+1, 1, ViLine2, ViCol);
#if AP
                    if(DebugLevel>1) {
                      cout << "PXIeBoardReader::ReadPixelsFromFrame_FSBB : chip [" << ViFsbb0 << "] = row [" << ViLine << "], col [" << ViCol << "]" << endl;
                    }
#endif
                  }
                }
                VMask = VMask >> 1;
              } // End For ViCol
            } // End For ViLine
          } // End States in one StateRec loop for G0 
        } /* end decoding groups for G0 */
      } // End StatesRec loop
    }  /* end if (VZsFFrame.StatesRecNb > 0)*/
    
    if ( &PtOvfCnt != NULL ) {
      PtOvfCnt = VOfvCnt;
    }
    
    if(DebugLevel>2) cout << "                 # pixels so far " << ListOfPixels.size() << endl;
    
    // =======
    // Condition to stop the reading after the data of the last sensor
    //  ==LastFrame changed to >=LastFrame to prevent to loop for ever for an unexisting frame, JB 2011/10/31
    //  additional condition on AcqID > AcqNo also to prevent infinite loop, JB 2015/03/17
    if(DebugLevel>1 && ViFsbb0 == VPtHead->MapsNb-1 && ReadingEvent ) printf( "       testing ending condition with frame %d ?>= %d or acq %d >? %d\n", VPtHead->FrameIdInAcq, FSBB_LastFrame, VAcqNo, FSBB_AcqNo);
    if( ViFsbb0 == VPtHead->MapsNb-1 && ReadingEvent && (VPtHead->FrameIdInAcq >= FSBB_LastFrame || VAcqNo > FSBB_AcqNo ) ) {
      
      ReadingEvent   = false;
      EventReady     = true;
      LastLineToKeep = -1;
      LastFrame      = -1;
      
      if(DebugLevel>1) printf("                  ** Reading event %d stopped!\n", CurrentEventNumber);
    }
    
  } // end loop on sensors
  
  
  if(DebugLevel>1) cout << "                 total # pixels found " << ListOfPixels.size() << endl;
  
#endif
  
  // -+-+- End
  return true; // if you got there, it is safe!
  
  
}
// --------------------------------------------------------------------------------------

bool PXIeBoardReader::CheckPixelToAdd( Int_t aSensorId, Int_t aFrame, Int_t aLine, Int_t aColumn) {
  
  // Check the pixel can be added in the event or not.
  // Make a difference in the decision if the sensor is a DUT or not,
  //   in order to enlarge the information of the DUT / tracking planes (=telescope).
  //
  // JB 2012/02/11 from the previous ReadPixelsFromFrame version
  
  bool add = true;
  
  // if DUT:
  // include frames around the ones defining the event limit
  // do not care if we are reading an event or not
  if ( IsDUTSensor(aSensorId) /*4<=aSensorId && aSensorId<=7*/ ) {
    //add &= FirstFrame==-1 || FirstFrame-1<=aFrame;
    //add &= aFrame<=LastFrame+1;
    
    if(TypeFlag == 1) {
      //FSBB
      //int LastFrameDUT = LastFrame+1;
#if 1
      add &= ReadingEvent;
      add &=
      (aFrame == FirstFrame && aLine >= FirstLineToKeep)
      || (aFrame == LastFrameDUT && aLine <= LastLineToKeep)
      || (FirstFrame < aFrame && aFrame < LastFrameDUT);
#endif
    }
  } // end if DUT
  
  // else standard tracking sensors:
  // verify strict event limits for the frame
  // and check we are reading an event
  else {
    add &= ReadingEvent;
    add &=
    (aFrame == FirstFrame && aLine >= FirstLineToKeep)
    || (aFrame == LastFrame && aLine <= LastLineToKeep)
    || (FirstFrame < aFrame && aFrame < LastFrame);
  } // end else standard tracking sensors
  
  // In both cases, exclude vetoed pixels
  add &= fTool.VetoPixel==NULL || !fTool.VetoPixel( aSensorId, aLine, aColumn);
  
  if(DebugLevel>3) printf("            for sensor %d, frame %d [%d,%d], line %d [%d,%d], col %d => addPix = %d\n",
                          aSensorId,
                          aFrame,
                          FirstFrame,
                          LastFrame,
                          aLine,
                          FirstLineToKeep,
                          LastLineToKeep,
                          aColumn,
                          add);
  
  return add;
}

// --------------------------------------------------------------------------------------

bool PXIeBoardReader::HasData( int aTriggerNb) {
  
  // Go directly to the event containing the trigger numbered aTriggerNb,
  //  correspoding to the numerotation loaded in the fileHeaders file.
  // If the fileHeaders is empty, generate it.
  // The argument "aTriggerNb" is taken as a the trigger only in mode TriggerMode=1.
  //
  // If TriggerMode=0, "aTriggerNb" is taken as a frame nb.
  //
  // JB, 2012/07/10
  // VR 2014/07/13 correct a bug in acqNo and frNo calculation and change the return value
  
  int acqNo=-1, frNo=-1;
  
  // When trigger mode is not 1, aTriggerNb is a frame nb.
  if( TriggerMode!=1 ) {
    acqNo = aTriggerNb/NFramesPerAcq; //VR 2014/07/13 correct a bug
    frNo  = aTriggerNb%NFramesPerAcq;
    if( DebugLevel ) printf ( "  PXIeBoardReader::HasData board %d: overall frame %d corresponds to acquisition %d and frame %d\n", BoardNumber, aTriggerNb, acqNo, frNo);
  }
  
  // When trigger mode is 1.
  else {
    // Try to open the file with trigger numbering
    if( !OpenFileHeaders() ) {
      cout << " PXIeBoardReader::HasData, WAIT during the generation of the trigger numbering !" << endl;
      DumpEventHeaders( NFramesPerAcq*Nacquisitions ); // nb of events sufficiently large to cover whole file
    }
    
    // Now use the TTree to get the acquisition and frame numbers
    //  corresponding to the trigger required.
    // Note we start the loop on the TTree entries at triggerNb,
    //  since we expect Entry>triggerNb.
    struct header aHeader;
    treeHeaders->SetBranchAddress("header",&aHeader);
    for (int iEntry=aTriggerNb; iEntry<treeHeaders->GetEntries(); iEntry++) {
      treeHeaders->GetEntry( iEntry);
      if( aHeader.triggerNb == aTriggerNb ) {
        acqNo = aHeader.acqId;
        frNo  = aHeader.frameId;
        if( DebugLevel ) printf ( "  PXIeBoardReader::HasData board %d: found trigger %d at acquisition %d and frame %d\n", BoardNumber, aTriggerNb, acqNo, frNo);
        break;
      }
    }
  }
  
  // if the trigger was not found
  if( acqNo == -1 && frNo == -1 ) {
    printf ( "PXIeBoarder board %d: did not found trigger %d in the defined list, sorry\n", BoardNumber, aTriggerNb);
    return false;
  }
  // Finaly go directly to that frame
  else 
  {
    if (!ResetDaqEvent( acqNo, frNo)/*1 if ok*/) return 0; // VR 2014/07/13 return coherent with (un)ability to go where asked
    return HasData();//1 if OK
  }
  
}

// --------------------------------------------------------------------------------------

bool PXIeBoardReader::HasData( int acqNo, int frNo) {
  
  // Go directly to the event at acquisition acqNo and frame frNo
  //
  // JB, 2012/07/10
  // Modified: JB, 2012/09/04 management of required frames not found
  
  if( ResetDaqEvent( acqNo, frNo) ) {
    StopIfRequiredFrameNotFound = true;
    return HasData();
  }
  else {
    return false;
  }
  
}

// --------------------------------------------------------------------------------------

bool PXIeBoardReader::HasData( ) {
  
  // Fill a new event containing all the fired pixels of each sensors acquired,
  // return "true" if there is still some data to read, "false" otherwise.
  // The return code could be true AND the event empty, 
  //   ==> the user has to test the pointer returned by GetEvent().
  //
  // To build the event, frame(s) is(are) read, the number depending on the trigger configuration.
  //
  // JB, 2009/08/18
  // Modified: JB, 2009/09/08, return empty event if overflow
  // Modified: JB, 2009/09/09, change the way frames are read twice when necessary
  // Modified: JB, 2010/06/16, manage several triggers
  // Modified: MG, 2012/03/14, increment ListOfLineOverflow, the line overflow vector
  // Modified: JB  2012/05/04  include timestamp management
  // Modified: JB, 2012/09/04  management of required frames not found
  // Modified: JB  2013/07/20  introducing trigger mode 3
  // Modified: JB, 2015/03/18 manage abort event
  
  // -+-+- Initialization
  
  if(DebugLevel>1) cout << "  PXIeBoardReader board " << BoardNumber 
    << "::HasData - readingEvent " << ReadingEvent 
    << " event readiness " << EventReady 
    << " currentEvent " << CurrentEventNumber 
    << endl;
  
  bool readingOK = true; // flag to test the reading
  Overflow       = false; // init value, JB 2009/09/08
  CurrentEvent   = 0;    // Allow to know wether data are correct, JB 2009/05/26
  ListOfPixels.clear();
  ListOfTriggerPos.clear(); //JB 2010/06/16
  ListOfTimestamps.clear(); //JB 2012/05/04
  ListOfFrames.clear(); //JB 2010/06/16
  
  for (int i=0;i<NSensors;i++) ListOfLineOverflow[i].clear(); //MG 2012/02/21
  
  // -+-+- Check files have been associated
  
  if( NumberOfFiles==0 ) {
    cout << "ERROR: PXIeBoardReader NO RAW DATA FILE WAS ASSOCIATED WITH BOARD " << BoardNumber << ", STOPPING!" << endl << endl;
    return false;
  }
  
  // -+-+- Loop over Daq Events (or frames for all sensors)
  
  while( !EventReady && readingOK ) { // while some event has to be read & readout OK
    
    Bool_t goDecode = true;
    if( !ReadTwice ) { // If not second reading of same frame, JB 2009/09/09
      if( TriggerMode==3 && !ReadingEvent) ResetDaqEvent( VAcqNo, NextFirstFrame);
      goDecode = GetNextDaqEvent(); // Get the next Daq Event
    }
    
    if( goDecode ) { // If frame is OK
#if AP
      //cout << "HasData():: Reading frame " << (&VPtFrame->Header)->FrameIdInAcq << ", frame counter = " << (&VPtFrame->Header)->AMapsFrameCnt[0]<< endl;
      if(DebugLevel>1) {
        cout << "HasData():: Reading frame " << (&VPtFrame->Header)->FrameIdInAcq << endl;
      }
#endif
      
      readingOK &= ReadTriggersFromFrame();
      //if( ReadingEvent) 
      readingOK &= ReadPixelsFromFrame();
      
    } // end if frame is OK
    
    // wrong Daq frame
    else {
      readingOK = false;
      if( DebugLevel ) {
        cout << "WARNING: PXIeBoardReader board " << BoardNumber << ", Daq Event unreadable!" << endl;
        if(NTriggersInEvent == 0) {
          cout << "WARNING: PXIeBoardReader board " << BoardNumber << ", Found zero triggers for this event" << endl;
        }
      }
    }
    
  } // while some events have to be read
  
  // Add case when the event has to be aborted
  // JB 2015/03/16
  if( AbortEvent && !readingOK ) {
    readingOK = true;
    AbortEvent = false; 
    if( DebugLevel ) cout << "WARNING: PXIeBoardReader board " << BoardNumber << ", event was aborted!" << endl;
    EventsAbortedByTag++;
  }
  
  // Add case when the required event was not found
  //  readingOK is forced to true in order to continue the reading.
  else if( StopIfRequiredFrameNotFound && !readingOK ) {
    readingOK = true;
    StopIfRequiredFrameNotFound = false; // necessary for next event
    if( DebugLevel ) cout << "WARNING: PXIeBoardReader board " << BoardNumber << ", the required frame was not found!" << endl;
    EventsMissed++;
  }
  
  // -+-+- Create the event if event is complete and readout is OK
  
  if( EventReady && readingOK ) {
    
    if( Overflow) {  // return no pixels if overflow, JB 2009/09/09, 2010/07/06, JB 2011/07/26
      EventsOverflow++;
      if( VetoOverflow) {
        ListOfPixels.clear();
        if(DebugLevel) printf("    event listed as overflow, list of pixels cleared (%d events with overflow so far).\n", EventsOverflow);
      }
    }
    
    EventsCount++;
    ///    CurrentEvent = new PXIeEvent( CurrentEventNumber, BoardNumber, RunNumber, &ListOfPixels, &ListOfTriggerPos, &ListOfFrames);
    CurrentEvent = new PXIeEvent( CurrentEventNumber, BoardNumber, RunNumber, &ListOfPixels, &ListOfTriggerPos, &ListOfFrames, &ListOfTimestamps , ListOfLineOverflow);//MG 2012/02/21
    if(DebugLevel>1) printf("  New event %d from board %d with %ld pixels triggers %ld and %ld frames created (%d events so far)\n", CurrentEventNumber, BoardNumber, ListOfPixels.size(), ListOfTriggerPos.size(), ListOfFrames.size(), EventsCount); // removing warning: specifying format for arguments of type 'size_type' (aka 'unsigned long') BH 2013/08/20
    
    // re-init
    EventReady         = false;
    
  }
  
  
  // -+-+- End
  if(DebugLevel>1) printf("  readingOK=%d\n", readingOK);
  return readingOK;
  
}

// --------------------------------------------------------------------------------------

void PXIeBoardReader::AddPixel( int input, int value, int aLine, int aColumn) {
  
  // Add a pixel to the vector of pixels
  // require the following info
  // - input = number of the sensors
  // - value = analog value of this pixel
  // - line & column = position of the pixel in the matrix
  // JB, 2009/08/14
  
  ListOfPixels.push_back( PXIePixel( input, value, aLine, aColumn));
  
}

// --------------------------------------------------------------------------------------

void PXIeBoardReader::PrintStatistics(ostream &stream) {
  
  // Print statistics on the events read by this board
  //
  // JB, 2009/09/09
  // Modified JB 2009/10/14 to add count of events read twice
  // Modified JB 2009/10/16 to add count of events aborted
  // Modified JB 2011/06/18 to add trigger counter
  // Modified JB 2011/10/30 modes printout added
  // Modified SS 2011/12/14 output stream can be set from outside
  // Modified JB 2012/09/25 add invalid trigger count
  // Modified JB 2014/02/12 add list of DUT sensors
  
  stream << "***********************************************" << endl;
  stream << " Board PXIe " << BoardNumber << " with trigger mode " << TriggerMode << ", building mode " << EventBuildingMode << " found:" << endl;
  stream << TriggerCount << " triggers valid, " << TriggerInvalidCount << " triggers invalid." << endl;
  if( TriggerMode==2 ) {
    stream << PeriodsVetoOff << " periods of non-veto reading," << endl;
  }
  stream << EventsCount << " events in total," << endl;
  stream << EventsOverflow << " events with an overflow (veto overflow " << VetoOverflow << ")." << endl;
  if( EventsOverflow ) { // added, JB 2011/11/02
    for( int is=0; is<NSensors; is++) {
      stream << " s" << is << ": " << overflowSensor[is];
    }
    stream << endl;
  }
  stream << ListOfDUTSensors.size() << " DUT sensors defined: ";
  for (unsigned long id=0; id<ListOfDUTSensors.size(); id++) {
    stream << ListOfDUTSensors.at(id) << " ";
  }
  stream << endl;
  stream << AcqsReadFromFile << " acquisitions read overall." << endl;
  stream << FramesReadFromFile<< " frames read overall." << endl;
  stream << FramesReadTwice << " frames read twice." << endl;
  stream << EventsAbortedByTrig << " events aborted by faulty trigger." << endl;
  stream << EventsAbortedByTag << " events aborted by faulty tag." << endl;
  stream << EventsMissed << " events missed with a specific frame required." << endl;
  stream << EventsWithBadTrigger << " frames with an incorrect trigger info." << endl;
  stream << " average #states in line: " << h1LineOccupancy->GetMean() << " with RM: " << h1LineOccupancy->GetRMS() << endl;
  stream << " average #states in block: " << h1BlockOccupancy->GetMean() << " with RM: " << h1BlockOccupancy->GetRMS() << endl;
  stream << "***********************************************" << endl;
  
  char name[20], title[50];
  sprintf( name, "cPXI%d", BoardNumber);
  sprintf( title, "Statistics for board %d", BoardNumber);
  TCanvas c( name, title, 300, 600);
  c.Divide(1,2);
  c.cd(1);
  h1LineOccupancy->Draw();
  c.cd(2);
  h1BlockOccupancy->Draw();
  c.Update();
  
  int Nplots_per_line = 6;
  int Ncols = 0, Nrows = 0;
  if(NSensors == 1) {
    Nrows = 1;
    Ncols = 1;
  }
  else if(NSensors <= 4) {
    Nrows = 2;
    Ncols = 2;
  }
  if(NSensors <= Nplots_per_line) {
    Nrows = 2;
    Ncols = 3;
  }
  else {
    Nrows = NSensors/Nplots_per_line;
    if(NSensors%Nplots_per_line) Nrows++;
    Ncols = 6;
  }
  
  TCanvas *cLineOverFlow = new TCanvas("cLineOverFlow","Line Overflow distribution for all sensors",300,600);
  cLineOverFlow->Divide(Ncols,Nrows);
  for(int isensor=0;isensor<NSensors;isensor++) {
    cLineOverFlow->cd(isensor+1);
    hLineOverFlowDist[isensor]->Draw();
  }
  
  sprintf( title, "Results/%d/PxiStats.root", RunNumber);
  sprintf( title,"%s", fTool.LocalizeDirName( title)); // JB 2011/07/07
  
  cout << endl;
  cout << "Saving Board Statistics to file  " << title << endl;
  cout << endl;
  TFile f_BoardStat(title,"RECREATE");
  c.Write();
  h1LineOccupancy->Write();
  h1BlockOccupancy->Write();
  
  cLineOverFlow->Write();
  for(int iSensor=0;iSensor<NSensors;iSensor++) hLineOverFlowDist[iSensor]->Write();
  f_BoardStat.Close();
  
  
}

// --------------------------------------------------------------------------------------

int PXIeBoardReader::ComputeTimestamp( int aAcqId, int aFrameId, int aLineNumber) {
  
  // Compute the number of lines between:
  // - the first line of the first frame of the first (origin) acquisition
  // - a given line
  //
  // This number represents the Timestamp of the given line,
  //  multiply by the row readout time to get an actual time.
  //
  // JB 2012/05/04
  
  return
  // lines from all the previous Acquisition
  (aAcqId-OriginAcq)*NFramesPerAcq*NumberOfLines
  // lines from all past frames of this Acquisition
  //  the +1 takes into account that the trigger is read in the previous frame
  + (aFrameId+1)*NumberOfLines
  // lines from this frame
  + aLineNumber;
  
}

// --------------------------------------------------------------------------------------

bool PXIeBoardReader::OpenFileHeaders() {
  
  // Open the root file containing the Ttree with the trigger and frame information
  // if it is not opened yet.
  // Once the file is open, load the TTree inside if any and return 1.
  // If no TTree available, return 0.
  //
  // JB 2012/07/10
  
  bool returnCode = false;
  
  Char_t fileName[300];
  sprintf( fileName, "Results/%d/pxiE_headers_dump_run%d.root", RunNumber, RunNumber);
  sprintf( fileName,"%s", fTool.LocalizeDirName( fileName));
  
  // if tree is already loaded, do nothing
  if( treeHeaders!=NULL && treeHeaders->GetEntries()>0 ) {
    returnCode = true;
    if( DebugLevel ) printf( "PXIeBoardReader board %d: tree header already loaded with %lld entries\n", BoardNumber, treeHeaders->GetEntries()); // removing warning: specifying format for argument of type 'Long64_t' (aka 'long long') BH 2013/08/20
  }
  
  // otherwise try to open the file
  else if( fileHeaders==NULL ) {
    fileHeaders = new TFile(fileName,"UPDATE");
    if( DebugLevel ) printf( "PXIeBoardReader::OpenFileHeaders board %d: header file opened\n", BoardNumber);
    
    // if the file contains something, load the tree
    if( fileHeaders!= NULL && !fileHeaders->IsZombie() && fileHeaders->GetNkeys() ) {
      treeHeaders = (TTree*)fileHeaders->Get("theader");
      // there is a tree in the file
      if( treeHeaders!=NULL ) {
        printf( "PXIeBoardReader board %d: tree header loaded with %lld entries\n", BoardNumber, treeHeaders->GetEntries()); // removing warning: specifying format for argument of type 'Long64_t' (aka 'long long') BH 2013/08/20
        returnCode = true;
      }
      // there is no tree in the file
      else {
        printf( "PXIeBoardReader board %d: tree header needs to be created\n", BoardNumber );
        returnCode = false;
      }
    }
    // the file is epmty
    else {
      if( DebugLevel ) printf( "PXIeBoardReader::OpenFileHeaders board %d: header file empty so far.\n", BoardNumber);
      returnCode = false;
    }
    
  }
  
  return returnCode;
}

// --------------------------------------------------------------------------------------

void PXIeBoardReader::DumpEventHeaders( int nFrames) {
  
  // Dump the information of acquisition, frame and triggers,
  //  for nFrames frames.
  // The info is printed into a file.
  //
  // This method is intended to be used in standalone mode,
  //  without any actual analysis of the data.
  // One TAF is launched:
  //  instatiate an object: PXIeBoardReader board( ...);
  //  call the method: board.DumpEventHeaders( 1000);
  //
  // JB, 2012/05/03
  // Modified JB, 2012/05/04 use ComputeTimestamp method
  // Modified JB, 2012/07/10 addition of trigger counter
  // Modified: JB 2012/07/10 use new method to check trigger validity and add new colum #trigger
  
  // -+-+- Initialization
  
  int framesRead = 0;
  int triggerLine = 0;
  int time; // expressed in number of lines read (200 ns slot)
  bool badTrigger = false;
  int nTotalTriggers = 0;
  int nBadTriggers = 0;
  int triggerCounter = 0;
  
  struct header aHeader;
  TTree *tree = new TTree( "theader", "Event header");
  tree->Branch( "header", &aHeader, "triggerNb/I:frameNb/I:acqId/I:frameId/I:triggersInFrame/I:triggerPos/I:timestamp/I");
  
  
  if( NumberOfFiles==0 ) {
    cout << "ERROR: PXIeBoardReader NO RAW DATA FILE WAS ASSOCIATED WITH BOARD " << BoardNumber << ", STOPPING!" << endl << endl;
    return;
  }
  
  FILE *outFile;
  Char_t fileName[300];
  sprintf( fileName, "Results/%d/pxiE_headers_dump_run%d.txt", RunNumber, RunNumber);
  sprintf( fileName,"%s", fTool.LocalizeDirName( fileName));
  outFile = fopen( fileName, "w");
  if( outFile==NULL ) {
    cout << "ERROR: PXIeBoardReader CANNOT CREATE FILE " << fileName << " to dump header info." << endl;
    return;
  }
  fprintf( outFile, " trigger#  frame#  AcqId  FrameId  #Triggers triggerLine  timestamp\n");
  fprintf( outFile, "  starting time in the day is %d seconds\n", RunTimeInSec);
  
  // -+-+- Pointers to useful frame information
  
  EFRIO__TFrameHeader* VPtHead;
  EFRIO__TTriggerRec*  VPtTrig;
  SInt16               ViTrig;
  EFRIO__TFlexRioTimeStamp1 VTs;
  APP__TContext* VPtCont;
  
  // -+-+- Loop over Daq Events (or frames for all sensors)
  
  while( GetNextDaqEvent() && framesRead < nFrames ) { // while some event has to be read & readout OK
    
    VPtHead = &VPtFrame->Header;
    VPtTrig = (EFRIO__TTriggerRec*) ( (UInt8*) VPtFrame + VPtFrame->TrigRecOffset );
    VPtCont = &APP__VGContext;
    nTotalTriggers += VPtHead->TriggerNb;
    
    aHeader.frameNb = framesRead;
    aHeader.acqId   = VPtHead->AcqId;
    aHeader.frameId = VPtHead->FrameIdInAcq;
    aHeader.triggersInFrame = VPtHead->TriggerNb;
    
    if(DebugLevel>0) printf("** board %2d : count %.8d : AcqId = %.4d - FrameId = %.4d - #triggers = %.4d (or %.4d)\n", BoardNumber, framesRead, VPtHead->AcqId, VPtHead->FrameIdInAcq, VPtHead->TriggerNb, VPtTrig->TrigNb);
    
    badTrigger = !CheckTriggerValidity();
    
    // Loop over triggers in frame
    for ( ViTrig=VPtHead->TriggerNb-1; ViTrig>=0 ; ViTrig-- ) { //loop on triggers
      
      
      VTs.W32 = (UInt32)VPtTrig->ATrig[(2 * ViTrig) + 1];
      if( VPtHead->TriggerNb<=4 ) {
        triggerLine = (VPtHead->AMapsTrigInfo[ViTrig])%1024;
      }
      else {
        triggerLine = VTs.F.Mi26Line;
      }
      
      time = ComputeTimestamp( VPtHead->AcqId, VPtHead->FrameIdInAcq, triggerLine + TriggerLineOffset);
      
      if( badTrigger ) {
        if(DebugLevel>0) printf("    bad trigger[-1, in frame %d] at line %.4d (%.4d/%.4d & frame %.4d & invalid %.1d), time = %d\n", ViTrig, triggerLine, (ViTrig<4?(VPtHead->AMapsTrigInfo[ViTrig])%1024:-1), VTs.F.Mi26Line, VTs.F.Mi26Frame, VTs.F.InvalidInfo , time);
        nBadTriggers++;
        aHeader.triggerNb = -1;
      }
      else {
        if(DebugLevel>0) printf("    trigger[%d, in frame %d] at line %.4d (%.4d/%.4d & frame %.4d & invalid %.1d), time = %d\n", triggerCounter, ViTrig, triggerLine, (ViTrig<4?(VPtHead->AMapsTrigInfo[ViTrig])%1024:-1), VTs.F.Mi26Line, VTs.F.Mi26Frame, VTs.F.InvalidInfo , time);
        aHeader.triggerNb = triggerCounter++;
      }
      
      aHeader.triggerPos = triggerLine;
      aHeader.timestamp = time;
      tree->Fill();
      fprintf( outFile, "%d %d %d %d %d %d %d\n", aHeader.triggerNb, framesRead, VPtHead->AcqId, VPtHead->FrameIdInAcq, VPtHead->TriggerNb, triggerLine, time);
      
    } // end loop on triggers
    
    framesRead++;
  } // while some events have to be read
  
  
  // -+-+- End
  printf( " Over %d frames, %d triggers were found and among them %d were considered as bad (=ignored).\n", framesRead, nTotalTriggers, nBadTriggers);
  printf( "  -> Information dumped to file %s.\n\n", fileName);
  
  /*
   sprintf( fileName, "Results/%d/pxiE_headers_dump_run%d.root", RunNumber, RunNumber);
   sprintf( fileName,"%s", fTool.LocalizeDirName( fileName));
   TFile fRoot(fileName,"RECREATE");
   tree->Write();
   fRoot.Close();
   fclose( outFile);
   */
  OpenFileHeaders();
  tree->Write( tree->GetName(), kOverwrite);
  treeHeaders = tree;
  printf( "  -> New tree (theader) written in file %s\n", fileHeaders->GetName());
  
}

// --------------------------------------------------------------------------------------
void  PXIeBoardReader::SetASICType(int ASIC_type)
{
  
#ifdef PXI_DAQ_LIB_VERSION_3_1 
  switch (ASIC_type) {
    case 26:
      //Mimosa-26 ASIC
      cout << "MimosaType = " << ASIC_type << " => Reading Mimosa-26" << endl;
      ASIC__SENSOR = ASIC__MI26;
      break;
    case 28:
      //Mimosa-28/ULTIMATE ASIC
      cout << "MimosaType = " << ASIC_type << " => Reading Mimosa-28/Ultimate" << endl;
      ASIC__SENSOR = ASIC__ULT1;
      break;
    case 35: 
      //FSBB-M0 ASIC
      cout << "MimosaType = " << ASIC_type << " => Reading FSBB-M0" << endl;
      ASIC__SENSOR = ASIC__FSBB0;
      break;
    default:
      cout << "MimosaType " << ASIC_type << " unknown!!!" << endl;
      cout << "List of GOOD MimosaType values:" << endl;
      cout << " - 26 -> Mimosa-26" << endl;
      cout << " - 28 -> Mimosa-28/Ultimate" << endl;
      cout << " - 35 -> FSBB-M0" << endl;
      Fatal("DPlane:Update", "==> CHANGE CONFIG FILE, I AM STOPPING! <==");
  };
#endif
  
  return;
  
}
// --------------------------------------------------------------------------------------
PXIeEvent::PXIeEvent( int eventNumber, int boardNumber, int runNumber, vector<PXIePixel> *listOfPixels, vector<int> *listOfTriggerPos, vector<int> *listOfFrames, vector<int> *listOfTimestamps, vector<int> *listOfLineOverflow) {
  
  // JB, 2009/08/14
	// Modified JB 2010/06/16
	// Modified MG 2012/03/14 to add list of lines in overflow
  // Modified JB 2012/05/04 include management of timestamp
	
  EventNumber    = eventNumber;
  BoardNumber    = boardNumber;
  RunNumber      = runNumber;
  ListOfPixels   = listOfPixels;
  ListOfTriggerPos = listOfTriggerPos; //JB 2010/06/16
  ListOfFrames   = listOfFrames; //JB 2010/06/16
  ListOfLineOverflow = listOfLineOverflow; // MG 2012/02/15
  ListOfTimestamps = listOfTimestamps; // JB 2012/05/04
  
}

// --------------------------------------------------------------------------------------

PXIeEvent::~PXIeEvent() {
  
  // crashing for the time bein...?
  // JB, 2009/09/14
  
  //   delete ListOfPixels;
  
}


