/////////////////////////////////////////////////////////////
// Class Description of BoardReaderMIMOSIS
//
// Dedicated to decode output files from the PXI acquisition system
//  dedicated to MIMOSIS sensors
//
//
/////////////////////////////////////////////////////////////
//
// created JB, 2021/04/21
// Last Modified:
#define AP 0

#include "BoardReaderMIMOSIS.h"
#include "mimo_daq_lib/mimo_daq_lib.h"
#include "mimo_daq_lib/mimo_daq_lib.c"

    /* ========================================= */
    /* Conditional compilation for DAQ sources   */
    /* ========================================= */

    #define ROOT_ROOT // Disable code parts which can't compile under ROOT
    #define CC_UNIX   // Specify operating system
    //---------------------------------------------------------------------------------
    // Conditionnal compilation switches
    // ---------------------------------------------------------------------------------


    #undef  CC_APP_GUI_CPP       // Disables C++ Builder GUI application => Memo for error and general messagess logging

    #define CC_MSIS1_BDF_LIGHT   // Reduces functions, class integrated from library X:/lib/com/maps/msis1/Data
                                 // in oder to minimize the risk of compilation warnings / errors

    #define CC_NOT_CPP_BUILDER   // Removes items (cst, functions, etc ... ) only available under C++ Builder



    #undef CC_INTERFACE_FILE     // To get variables created in this module


    // #define APP_ERR_MSG_MACROS_NOT_SUPPORTED  // To be defined in case of compilation errors on errors and general messages libs
                                                 // ..\..\com\errors_light.c, errors_light.h
                                                 // ..\..\com\msg_light.c, msg_light.h
                                                     

    #undef CC_APP_ERR_LOG_VIA_PRINTF             // If enabled => Error log message are automatically printed in console via printf () function
                                                 // Rq : The sames messages are written in MIS1__TBtRunRead_DEF_ERR_LOG_FILE

    #define CC_APP_MSG_LOG_VIA_PRINTF            // If enabled => General message log are automatically  printed in console via printf () function
                                                 // Rq : The sames messages are written in MIS1__TBtRunRead_DEF_MSG_LOG_FILE

    //

ClassImp(BoardReaderMIMOSIS);

//------------------------------------------+-----------------------------------
MIS1__TBtRunRead* APP_VGPtRunRead;

int BoardReaderMIMOSIS::test() {
    
    // ----------------------------------------------------------------
    // Variables
    // ----------------------------------------------------------------
    
    SInt32 VRet;                              // Error code returned by functions ( 0 => ok, < 0 => error )
    
    
    // Run parameters
    
    char   VParRunRootDir[GLB_FILE_PATH_SZ];  // Run root directory
    UInt32 VParRunNo;                         // No of run
    
    // Acqusition (Acq) header printing options
    
    SInt32 VParHdPrintTriggers     = 5; // Nb of triggers to print -1 => All, 0 => None, N > 0 => N first triggers
    SInt32 VParHdPrintFrCnt        = 3; // Print frame counter of 6 x MSis 1 over N frames, -1 => All frames, 0 => None, N > 0 => N first frames
    SInt32 VParHdPrintFiredPixels  = 1; // Print fired pixels (per MSis1, per submatrices), 0 => No, 1 => Print, NOT CALCULATD NOW on 19/05/2021
    
    // Acqusition (Acq) raw data printing options
    
    UInt32 VParWSz      = 32; // Size of the word to print, cab be 8, 16, 32, 64 bits
    UInt32 VParFirstW   =  0; // Position of the first word to print, unit is W8, W16, W32, W64 function of VParWSz value
    UInt32 VParNbW      = 10; // Nb of words to print, in this case with VParWSz = 32 => 10 words of 32 bits will be printed
    
    // Acquisition "results"
    
    UInt8 VResReachEndOfRun = 0; // Flag set to 1 when last Acq of run has been reached
    
    // Acq header and Acq pointers
    
    MIS1__TBtRunCnfRec* VPtRunConf = NULL; // Pointer to run conf record
    MIS1__TBtAcqRawRec* VPtAcq     = NULL; // Pointer to current Acq
      
    
  
    
    printf ( "\n" );
    printf ( "**********************************************************\n" );
    printf ( "Beginning of Mimosis 1 data run files access library demo \n" );
    printf ( "**********************************************************\n" );
    printf ( "\n" );
  
  
    // ----------------------------------------------------------------
    // Check record size Windows / Linux
    // ----------------------------------------------------------------
    
    
    // If can be used for enabling / disabling this part of the demo
  
    if (1) {
      
      UInt32 __VRunConfRecSzWin    = 1340;
      UInt32 __VIndexRecSzWin      = 4867212;
      UInt32 __VAcqHeadRecSzWin    = 40560;
      UInt32 __VAcqTotRecSzWin     = 3240560;
      UInt32 __VAcqDataOffsetW8Win = 40560;
      
      
      UInt32 __VRunConfRecSz;
      UInt32 __VIndexRecSz;
      UInt32 __VAcqHeadRecSz;
      UInt32 __VAcqTotRecSz;
      UInt32 __VAcqDataOffsetW8;
      MIS1__TBtAcqRawRec* __VPtAcq;
      
      
      printf ( "\n" );
      printf ( "Records size printing for checking if size Linux / Windows are equal \n" );
      printf ( "\n" );
    
      
      // alloc one acq
      
      __VPtAcq = (MIS1__TBtAcqRawRec*) malloc ( sizeof ( MIS1__TBtAcqRawRec ) );
         
      
      if ( __VPtAcq == NULL ) {
        printf ( "\n" );
        printf ( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! \n" );
        printf ( "Abort : Allocation of one Acq record has failed ! \n" );
        printf ( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! \n" );
        return (-1);
      }
      
      // Run conf record
    
      __VRunConfRecSz = sizeof ( MIS1__TBtRunCnfRec );
      
      printf ( "Run conf record MIS1__TBtRunCnfRec size = %d, Windows size = %d \n\n", __VRunConfRecSz, __VRunConfRecSzWin );
      
      // Index file
      
      __VIndexRecSz = sizeof ( MIS1__TBtRunRawIndexRec );
      
      printf ( "Index file record MIS1__TBtRunRawIndexRec size = %d, Windows size = %d \n\n", __VIndexRecSz, __VIndexRecSzWin );
      
      // Acq header
      
      __VAcqHeadRecSz = sizeof ( MIS1__TBtAcqRawHead );
      
      printf ( "Acq header record MIS1__TBtAcqRawHead size = %d, Windows size = %d \n\n", __VAcqHeadRecSz, __VAcqHeadRecSzWin );
      
      // Acq total record
      
      __VAcqTotRecSz = sizeof ( MIS1__TBtAcqRawRec );
      
      printf ( "Acq total record MIS1__TBtAcqRawRec size = %d, Windows size = %d \n\n", __VAcqTotRecSz, __VAcqTotRecSzWin );
      
      // Acq data part offset / beginning
      
      __VAcqDataOffsetW8 = (UInt32) ((UInt8*) &__VPtAcq->MSisData.Au64[0] - (UInt8*) __VPtAcq);
      
      printf ( "Acq data part offset = %d W8, Windows offset = %d \n\n", __VAcqDataOffsetW8, __VAcqDataOffsetW8Win );
      
  
      if ( (__VRunConfRecSz != __VRunConfRecSzWin) || (__VIndexRecSz != __VIndexRecSzWin) || (__VAcqHeadRecSz != __VAcqHeadRecSzWin) || (__VAcqTotRecSz != __VAcqTotRecSzWin) || (__VAcqDataOffsetW8 != __VAcqDataOffsetW8Win) ) {
        printf ( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! \n" );
        printf ( "Record(s) size is/are different Linux / Windows \n" );
        printf ( "\n" );
        printf ( "Please check compiler option, add padding word, etc ... \n" );
        printf ( "\n" );
        printf ( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! \n" );
        printf ( "\n" );
        return (-1);
      }
  
  
      printf ( "\n" );
      printf ( "Records have the same size under Linux / Windows => It SHOULD be ok :-) \n" );
      printf ( "\n" );
      
        
      free ( __VPtAcq );
    }
    
    
    // return (0);  // You can enable it in order to stop sw after record size priting
    
      
    // ----------------------------------------------------------------
    // Class creation
    // ----------------------------------------------------------------
    
    APP_VGPtRunRead = new ( MIS1__TBtRunRead );
    
  
    if ( APP_VGPtRunRead == NULL ) {
      printf ( "\n" );
      printf ( "ERROR : Creation of MIS1__TBtRunRead  failed ! \n" );
      return (-1);
    }
    
    printf ( "\n" );
    printf ( "MIS1__TBtRunRead class creation done :-) \n" );
    printf ( "\n" );
    
    
  
    // ----------------------------------------------------------------
    // Error messages logging configuration
    // ----------------------------------------------------------------
    //
    // Error messages generated by lib can be
    //
    // - written in a log file, its name is defined by MIS1__TBtRunRead_DEF_ERR_LOG_FILE
    // - printed in the console if CC_APP_ERR_LOG_VIA_PRINTF is enabled
    //
    // - additionnal actions on each error logging can be implemented, if needed, in function
    //   SInt32 MIS1__TBtRunRead_FErrLogUsrPrint ( char Type, char* ErrLocation, char* ErrUserMsg, char* FullMsg )
    //   which is located in msis1_data.c
    //
    // Errors logging configuration is done in the following lines
    //
    
      
    // Sets errors logging, it can be
    //
    // ERR_LOG_LVL_NONE
    // ERR_LOG_LVL_ALL
    // ERR_LOG_LVL_WARNINGS_ERRORS
    // ERR_LOG_LVL_ERRORS
    
    
    APP_VGPtRunRead->FErrLogSetLevel  ( ERR_LOG_LVL_WARNINGS_ERRORS );
      
    
    // ----------------------------------------------------------------
    // General messages logging configuration
    // ----------------------------------------------------------------
    //
    // General messages generated by lib can be
    //
    // - written in a log file, its name is defined by MIS1__TBtRunRead_DEF_MSG_LOG_FILE
    // - printed in the console if CC_APP_MSG_LOG_VIA_PRINTF is enabled
    //
    // - additionnal actions on each error logging can be implemented, if needed, in function
    //   SInt32 MIS1__TBtRunRead_FMsgLogUsrPrint ( char* Msg )
    //   which is located in msis1_data.c
    //
    // General messages logging configuration is done in the following lines
    //
    
      
    // Sets messages logging, 127 is the only value handled now
    
    APP_VGPtRunRead->FMsgLogSetLevel ( 127 );
    
    
    // ----------------------------------------------------------------
    // Print read run log files names location
    // ----------------------------------------------------------------
    
    printf ( "\n" );
    printf ( "============================================================= \n" );
    printf ( "Errors log file   => %s \n", APP_VGPtRunRead->FErrLogSetFilename () );
    printf ( "Messages log file => %s \n", APP_VGPtRunRead->FMsgLogSetFilename () );
    printf ( "============================================================= \n" );
    printf ( "\n" );
    
  
  
    // ----------------------------------------------------------------
    // Config run to read <=> Open run
    // ----------------------------------------------------------------
    
    // Sets run directory
    
    // sprintf ( VParRunRootDir, "%s", "C:/Data/msis1" );
    // ZE 2021/05/26 data directory is the same level of directories hierarchy 
    sprintf ( VParRunRootDir, "%s", "data" );
    //sprintf ( VParRunRootDir, "%s", "../../data" );
    
  
    // Sets run no
    
    VParRunNo = 777;
    
    // Tries to configure run
    
    VRet = APP_VGPtRunRead->FRunConf ( VParRunRootDir, VParRunNo, 0 /* PrintRunHeader */, MIS1__BT_RUN_RD_FILE_FORMAT );
    
    if ( VRet < 0 ) {
      printf ( "\n" );
      printf ( "Configuration of run no %d from root directory = %s \n", VParRunNo, VParRunRootDir );
      printf ( "Failed : %s \n", APP_VGPtRunRead->FErrGetLastMsg () );
      return (-1);
    }
    
    printf ( "\n" );
    printf ( "Configuration of run no %d from root directory = %s \n", VParRunNo, VParRunRootDir );
    printf ( "Done :-) \n" );
    printf ( "\n" );
    
    
    // ----------------------------------------------------------------
    // Print run header
    // ----------------------------------------------------------------
    
    // If can be used for enabling / disabling this part of the demo
    
    if (1) {
      
      // Prints run conf + get a pointer on run conf
      
      VPtRunConf = APP_VGPtRunRead->FRunHeaderGet ( 1 /* Print */ );
      
      // Remark : To get only a pointer on run conf withotu prionting => Set Print param to 0
      
      
      if ( VPtRunConf == NULL ) {
        printf ( "\n" );
        printf ( "Run configuration record printing failed ! \n" );
        printf ( "\n" );
        return (-1);
      }
      
      printf ( "\n" );
      printf ( "Run configuration record printing done :-) \n" );
      printf ( "\n" );
      printf ( "Results have been written in file %s \n", APP_VGPtRunRead->FMsgLogSetFilename () );
      printf ( "Results have been printed in console if macro CC_APP_MSG_LOG_VIA_PRINTF is enabled \n" );
      printf ( "\n" );
      
      
    }
  
  
    // ----------------------------------------------------------------
    // Goto first Acq of run
    // ----------------------------------------------------------------
    
    // If can be used for enabling / disabling this part of the demo
    
    if (1) {
      
      // Configure Acq header printing options
      //
      // If you don't want to print header
      // - Sets param PrintAcqHead of  FAcqFirst (...) to 0
      // - You don't need to call FAcqHeadPrintOptSet (...)
      
      
      VParHdPrintTriggers     = 5; // Nb of triggers to print -1 => All, 0 => None, N > 0 => N first triggers
      VParHdPrintFrCnt        = 3; // Print frame counter of 6 x MSis 1 over N frames, -1 => All frames, 0 => None, N > 0 => N first frames
      VParHdPrintFiredPixels  = 1; // Print fired pixels (per MSis1, per submatrices), 0 => No, 1 => Print, NOT CALCULATD NOW on 19/05/2021
    
      
      VRet = APP_VGPtRunRead->FAcqHeadPrintOptSet ( VParHdPrintTriggers, VParHdPrintFrCnt, VParHdPrintFiredPixels );
      
      if ( VRet < 0 ) {
        printf ( "\n" );
        printf ( "Configure Acq header printing option has failed \n" );
        printf ( "\n" );
        return (-1);
      }
      
  
      // Parameter ChkAcqHead 0/1 is used to enable header checking => Not implemented now
      // Acq header record can be printed if param PrintAcqHead iss et to 1
  
      VPtAcq = APP_VGPtRunRead->FAcqFirst ( 0 /* ChkAcqHead */, 1 /* PrintAcqHead */ );
          
      if ( VPtAcq == NULL ) {
        printf ( "\n" );
        printf ( "Goto first Acq of run has failed ! \n" );
        printf ( "\n" );
        return (-1);
      }
      
      printf ( "\n" );
      printf ( "Goto first Acq of run done :-) \n" );
      printf ( "\n" );
      printf ( "Acquisition header has been printed :-) \n" );
      printf ( "\n" );
      printf ( "Results have been written in file %s \n", APP_VGPtRunRead->FMsgLogSetFilename () );
      printf ( "Results have been printed in console if macro CC_APP_MSG_LOG_VIA_PRINTF is enabled \n" );
      printf ( "\n" );
      
    }
    
    
    // ----------------------------------------------------------------
    // Print current Acq of run
    // ----------------------------------------------------------------
    
    // If can be used for enabling / disabling this part of the demo
    
    // Remark 1 : To get a ptr on the current Acq =>  APP_VGPtRunRead->FAcqGetPt ()
    //
    // Remark 2 : To get a access to the raw data VPtAcq = APP_VGPtRunRead->FAcqGetPt ()
    //
    // VPtAcq->MSisData.Au8  => raw data as an array of W8
    // VPtAcq->MSisData.Au16 => raw data as an array of W16
    // VPtAcq->MSisData.Au32 => raw data as an array of W32
    // VPtAcq->MSisData.Au64 => raw data as an array of W64
    
    
    
    if (1) {
    
      // Header printing options
    
      VParHdPrintTriggers     = 5; // Nb of triggers to print -1 => All, 0 => None, N > 0 => N first triggers
      VParHdPrintFrCnt        = 3; // Print frame counter of 6 x MSis 1 over N frames, -1 => All frames, 0 => None, N > 0 => N first frames
      VParHdPrintFiredPixels  = 1; // Print fired pixels (per MSis1, per submatrices), 0 => No, 1 => Print, NOT CALCULATD NOW on 19/05/2021
      
      // Raw data printing options
      
      VParWSz      = 32; // Size of the word to print, cab be 8, 16, 32, 64 bits
      VParFirstW   =  0; // Position of the first word to print, unit is W8, W16, W32, W64 function of VParWSz value
      VParNbW      = 10; // Nb of words to print, in this case with VParWSz = 32 => 10 words of 32 bits will be printed
    
      
      VRet = APP_VGPtRunRead->FAcqPrint ( APP_VGPtRunRead->FAcqGetPt (), VParHdPrintTriggers, VParHdPrintFrCnt, VParHdPrintFiredPixels, VParWSz, VParFirstW, VParNbW, 0 /* VRS */ );
    
  
      if ( VRet < 0 ) {
        printf ( "\n" );
        printf ( "Printing first Acq of run has failed ! \n" );
        printf ( "\n" );
        return (-1);
      }
      
      printf ( "\n" );
      printf ( "Printing first Acq of run done :-) \n" );
      printf ( "\n" );
      printf ( "Acquisiiton has been printed :-) \n" );
      printf ( "\n" );
      printf ( "Results have been written in file %s \n", APP_VGPtRunRead->FMsgLogSetFilename () );
      printf ( "Results have been printed in console if macro CC_APP_MSG_LOG_VIA_PRINTF is enabled \n" );
      printf ( "\n" );
      
      
    }
  
  
    // ----------------------------------------------------------------
    // Goto next Acq of run
    // ----------------------------------------------------------------
    
    // If can be used for enabling / disabling this part of the demo
    
    if (1) {
      
      // Configure Acq header printing options
      //
      // If you don't want to print header
      // - Sets param PrintAcqHead of  FAcqFirst (...) to 0
      // - You don't need to call FAcqHeadPrintOptSet (...)
      
      
      VParHdPrintTriggers     = 5; // Nb of triggers to print -1 => All, 0 => None, N > 0 => N first triggers
      VParHdPrintFrCnt        = 3; // Print frame counter of 6 x MSis 1 over N frames, -1 => All frames, 0 => None, N > 0 => N first frames
      VParHdPrintFiredPixels  = 1; // Print fired pixels (per MSis1, per submatrices), 0 => No, 1 => Print, NOT CALCULATD NOW on 19/05/2021
      
      
      VRet = APP_VGPtRunRead->FAcqHeadPrintOptSet ( VParHdPrintTriggers, VParHdPrintFrCnt, VParHdPrintFiredPixels );
      
      if ( VRet < 0 ) {
        printf ( "\n" );
        printf ( "Configure Acq header printing option has failed \n" );
        printf ( "\n" );
        return (-1);
      }
      
      
      // Parameter ChkAcqHead 0/1 is used to enable header checking => Not implemented now
      // Acq header record can be printed if param PrintAcqHead iss et to 1
      
      VPtAcq = APP_VGPtRunRead->FAcqNext ( 0 /* ChkAcqHead */, 1 /* PrintAcqHead */, &VResReachEndOfRun );
      
      if ( VPtAcq == NULL ) {
        printf ( "\n" );
        printf ( "Goto next Acq of run has failed ! \n" );
        printf ( "\n" );
        return (-1);
      }
      
      printf ( "\n" );
      printf ( "Goto next Acq of run done :-) \n" );
      printf ( "\n" );
      printf ( "Acquisition header has been printed :-) \n" );
      printf ( "\n" );
      printf ( "Results have been written in file %s \n", APP_VGPtRunRead->FMsgLogSetFilename () );
      printf ( "Results have been printed in console if macro CC_APP_MSG_LOG_VIA_PRINTF is enabled \n" );
      printf ( "\n" );
      
      if ( VResReachEndOfRun ) {
        
        printf ( "\n" );
        printf ( "End of run has been reached \n" );
        printf ( "\n" );
        
        // Advise
        //
        // You easily modiy the code to scan the run here and priting the header of each Acq by
        // - Replacing the if (1) at the head of this block by a while (1)
        // - Enabling the hereafter break command, but warning => it can run for a while if many Acq in run ...
        //
        // break;
        
      }
      
    }
    
    
    // ----------------------------------------------------------------
    // Print current Acq of run
    // ----------------------------------------------------------------
    
    // If can be used for enabling / disabling this part of the demo
    
    // Remark 1 : To get a ptr on the current Acq =>  APP_VGPtRunRead->FAcqGetPt ()
    //
    // Remark 2 : To get a access to the raw data VPtAcq = APP_VGPtRunRead->FAcqGetPt ()
    //
    // VPtAcq->MSisData.Au8  => raw data as an array of W8
    // VPtAcq->MSisData.Au16 => raw data as an array of W16
    // VPtAcq->MSisData.Au32 => raw data as an array of W32
    // VPtAcq->MSisData.Au64 => raw data as an array of W64
    
    
    
    if (1) {
      
      // Header printing options
      
      VParHdPrintTriggers     = 5; // Nb of triggers to print -1 => All, 0 => None, N > 0 => N first triggers
      VParHdPrintFrCnt        = 3; // Print frame counter of 6 x MSis 1 over N frames, -1 => All frames, 0 => None, N > 0 => N first frames
      VParHdPrintFiredPixels  = 1; // Print fired pixels (per MSis1, per submatrices), 0 => No, 1 => Print, NOT CALCULATD NOW on 19/05/2021
      
      // Raw data printing options
      
      VParWSz      = 32; // Size of the word to print, cab be 8, 16, 32, 64 bits
      VParFirstW   =  0; // Position of the first word to print, unit is W8, W16, W32, W64 function of VParWSz value
      VParNbW      = 10; // Nb of words to print, in this case with VParWSz = 32 => 10 words of 32 bits will be printed
      
      
      VRet = APP_VGPtRunRead->FAcqPrint ( APP_VGPtRunRead->FAcqGetPt (), VParHdPrintTriggers, VParHdPrintFrCnt, VParHdPrintFiredPixels, VParWSz, VParFirstW, VParNbW, 0 /* VRS */ );
      
      
      if ( VRet < 0 ) {
        printf ( "\n" );
        printf ( "Printing next Acq of run has failed ! \n" );
        printf ( "\n" );
        return (-1);
      }
      
      printf ( "\n" );
      printf ( "Printing next Acq of run done :-) \n" );
      printf ( "\n" );
      printf ( "Acquisiiton has been printed :-) \n" );
      printf ( "\n" );
      printf ( "Results have been written in file %s \n", APP_VGPtRunRead->FMsgLogSetFilename () );
      printf ( "Results have been printed in console if macro CC_APP_MSG_LOG_VIA_PRINTF is enabled \n" );
      printf ( "\n" );
      
      
    }
      
    
    
    // ----------------------------------------------------------------
    // Close run
    // ----------------------------------------------------------------
    
    VRet = APP_VGPtRunRead->FRunClose ();
    
    if ( VRet < 0 ) {
      printf ( "\n" );
      printf ( "Close of run no %d from root directory = %s \n", VParRunNo, VParRunRootDir );
      printf ( "Failed : %s \n", APP_VGPtRunRead->FErrGetLastMsg () );
      return (-1);
    }
    
      
    printf ( "\n" );
    printf ( "Close of run no %d from root directory = %s \n", VParRunNo, VParRunRootDir );
    printf ( "Done :-) \n" );
    printf ( "\n" );
    
    
    // ----------------------------------------------------------------
    // Class destruction
    // ----------------------------------------------------------------
    
    // Class destruction
    
    delete ( APP_VGPtRunRead );
  
    printf ( "\n" );
    printf ( "**********************************************************\n" );
    printf ( "End of Mimossi 1 data run files access library demo \n" );
    printf ( "**********************************************************\n" );
    printf ( "\n" );

    
}
//------------------------------------------+-----------------------------------
BoardReaderMIMOSIS::BoardReaderMIMOSIS(int boardNumber, int nSensors, int triggerMode, int eventBuildingMode, int headerSize, int trailerSize, int endianness) {
  // Board creator
  // ==> OF COURSE the list of arguments might be changed if needed! <==

  fDebugLevel = 0; // set later by SetDebugLevel
  fBoardNumber = boardNumber;
  fNSensors = nSensors;
  fTriggerMode = triggerMode;
  fEventBuildingMode = eventBuildingMode;
  fSizeOfHeader = headerSize;
  fSizeOfTrailer = trailerSize;
  fEndianness = endianness;
  fVetoOverflow = false; // set later by SetVetoPixel


  cout << "*****************************************" << endl;
  cout << "    < BoardReaderMIMOSIS constructor >      " << endl;
  cout << "*****************************************" << endl;
  cout << "Creating a BoardReaderMIMOSIS" << endl;
  cout << " * board nb: " << fBoardNumber << endl;
  cout << " * nb of sensors: " << fNSensors << endl;
  cout << " * trigger mode: " << fTriggerMode << endl;
  cout << " * event building mode: " << fEventBuildingMode << endl;
  cout << " * event header & trailer sizes: " << fSizeOfHeader << " & " << fSizeOfTrailer << endl;
  cout << " * Endiannes: " << fEndianness << " => " << (fEndianness==0?"NO SWAP":"SWAP") << endl;
  cout << " * usage of veto for event with overflow: " << fVetoOverflow << " => " << (fVetoOverflow?"YES":"NO") << endl;

  // Initialization
  fCurrentFileNumber = 0; // See also AddFileList
  fNoMoreFile = false;
  fCurrentEventNumber = 0;
  fCurrentTriggerNumber = 0;
  fTriggerCount = 0;
  fFrameCount = 0;
  fNEventsWithOverflow = 0;

  cout << "    < BoardReaderMIMOSIS constructor DONE >      " << endl;
}

//------------------------------------------+-----------------------------------
BoardReaderMIMOSIS::~BoardReaderMIMOSIS()
{

//  delete fCurrentEvent;
//  delete fInputFileName;

}

// --------------------------------------------------------------------------------------
bool BoardReaderMIMOSIS::AddFileList(string prefixFileName, int startIndex, int endIndex, string suffixFileName) {
  // Try opening each input files built from name=prefix+index+suffix
  // Keep only the good one
  // Work also for a single file with startIndex=endIndex
  //
  // Return true if at least one file can be opened

  bool rc = true;
  std::string fileName;

  if(fDebugLevel) {
    cout << "BoardReaderMIMOSIS::AddFileList => Adding files with indexes from " << startIndex << " to " << endIndex;
    cout << " prefix=" << prefixFileName << ", suffix=" << suffixFileName << endl;
  }

  // Try each filename looping on indexes, and keep in the list only the good ones
  for (size_t iFile = startIndex; iFile <= endIndex; iFile++) {
    fileName = prefixFileName + "_" + std::to_string(iFile) + suffixFileName;
    if(fDebugLevel>1) cout << "  trying file " << fileName << endl;
  	fRawFileStream.open( fileName);
  	if( fRawFileStream.fail() ) {
  		cout << endl << "ERROR BoardReaderMIMOSIS " << fBoardNumber << " file " << fileName << " does not exist!!" << endl;
      fRawFileStream.close();
  		rc = false;
  	}
  	else{
      fListInputFileNames.push_back( fileName);
  		cout << "  --> BoardReaderMIMOSIS " << fBoardNumber << " New file " << fileName << ", total of " << fListInputFileNames.size() << " files." << endl;
      fRawFileStream.close();
    }
  }

  // If there is only one file, try a filename without an index
  if ( !rc &&  endIndex==startIndex ) {
    fileName = prefixFileName + suffixFileName;
    if(fDebugLevel>1) cout << "  trying file " << fileName << endl;
  	fRawFileStream.open( fileName);
  	if( fRawFileStream.fail() ) {
  		cout << endl << "ERROR BoardReaderMIMOSIS " << fBoardNumber << " file " << fileName << " does not exist!!" << endl;
      fRawFileStream.close();
  		rc = false;
  	}
  	else{
      fListInputFileNames.push_back( fileName);
  		cout << "  --> BoardReaderMIMOSIS " << fBoardNumber << " New file " << fileName << ", total of " << fListInputFileNames.size() << " files." << endl;
      fRawFileStream.close();
    }
  }

  cout << "#files OK = " << fListInputFileNames.size() << " out of " << endIndex-startIndex+1 << endl;

  // if at least one file is OK, reopens the very first file
  if ( fListInputFileNames.size() == 0 ) {
    fNoMoreFile = true;
    return false;
  }

  fCurrentFileNumber = 0;
  return LookUpRawFile();

}

// --------------------------------------------------------------------------------------
bool BoardReaderMIMOSIS::LookUpRawFile() {
  // Try to open the next rawdata file

  // If the stream is open, close it
  if( fRawFileStream.is_open() ) {
    CloseRawFile();
    fCurrentFileNumber++;
  }

  // Try to go to the next file, if any are left
  if( fCurrentFileNumber < fListInputFileNames.size() ) {
    if(fDebugLevel) cout << "  --> BoardReaderMIMOSIS " << fBoardNumber << " New file to read " << fCurrentFileNumber << " < " << fListInputFileNames.size() << "." << endl;
    return OpenRawFile();
  }
  else { // Otherwise no more file, end the reading
    cout << "  -+-+- INFO BoardReaderMIMOSIS " << fBoardNumber << ": No more files to read " << fCurrentFileNumber << " >= " << fListInputFileNames.size() << " closing!" << endl;
    fNoMoreFile = true;
    return false;
  }

}

// --------------------------------------------------------------------------------------
bool BoardReaderMIMOSIS::OpenRawFile() {
  // Open the next rawdata file

  fRawFileStream.open( fListInputFileNames[fCurrentFileNumber]);
  bool b = fRawFileStream.fail();
  if (b == 0)  {
    cout << " -+-+- INFO BoardReaderMIMOSIS " << fBoardNumber << ": File " << fListInputFileNames[fCurrentFileNumber] << " opened." << endl;
  }
  else {
    cout << " -/-/- INFO BoardReaderMIMOSIS " << fBoardNumber << ": File " << fListInputFileNames[fCurrentFileNumber] << " not opened, rc = " << b << "." << endl;
  }
  return !b;

}

// --------------------------------------------------------------------------------------
bool BoardReaderMIMOSIS::CloseRawFile() {
  // Closes a File of a Run

  fRawFileStream.close();
  bool b = fRawFileStream.fail();
  if (b == 0)
    cout << " -+-+- INFO BoardReaderMIMOSIS " << fBoardNumber << ": File " << fCurrentFileNumber << " closed " << endl;
  else
    cout << " -/-/- INFO BoardReaderMIMOSIS " << fBoardNumber << ":  File " << fCurrentFileNumber << " not closed, rc = " << b << "(eof="<< fRawFileStream.eof() << ", bad="<< fRawFileStream.bad() <<"." << endl;
  return b;

}

// --------------------------------------------------------------------------------------

bool BoardReaderMIMOSIS::HasData( ) {
  // Try to find the pixel data for the next event in the raw data file
  //
  // ==> THIS MIGHT BE TOO SIMPLISTIC depending on the data format <==
  //
  //
  // If something goes wrong with the decoding or no more events,
  //  a flag (eventOK) is used to return "false"
  // Otherwise returns "true"
  //

  // -+-+- Initialization

  bool eventOK   = true;   // we start with a good event by default
  fCurrentEvent   = NULL;  // Allow to know wether data are correct
  fListOfTriggers.clear();  // and clear the vectors
  fListOfTimestamps.clear();
  fListOfFrames.clear();
  fListOfPixels.clear();

 if(fDebugLevel) {
   cout << endl ;
   cout << "BoardReaderMIMOSIS::HasData() fBoardNumber " << fBoardNumber << " BoardReaderMIMOSIS::HasData() - currentEvent " << fCurrentEventNumber << " currentTrigger " << fCurrentTriggerNumber << endl;
 }

 if( DecodeNextEvent() ) {

   if(fDebugLevel>1) PrintEventHeader();

   fCurrentEvent = new BoardReaderEvent( fCurrentEventNumber, fBoardNumber, 0, &fListOfPixels); // run number set to 0 for now
   fCurrentEvent->SetListOfTriggers( &fListOfTriggers);
   fCurrentEvent->SetListOfFrames( &fListOfFrames);
   if(fDebugLevel) cout << " fBoardNumber " << fBoardNumber << " created new event " << fCurrentEventNumber << " with " << fListOfPixels.size() << " pixels from " << fListOfTriggers.size() << " triggers and " << fListOfFrames.size() << " frames." << endl;
   fCurrentEventNumber++;

 } // getting next buffer was not OK
 else{
   eventOK = false;
   cout<<" -/-/- ERROR BoardReaderMIMOSIS::HasData() - Can't get next event !"<<endl;
 }

 // End
 // check there are still data to read
 return eventOK;
}

//------------------------------------------+-----------------------------------
bool BoardReaderMIMOSIS::DecodeNextEvent() {
  // THIS IS THE MAIN PLACE FOR ROMA's WORK!
  //
  // This method should decide whether
  //  there is still something to read in the current file
  //  or if the next file should be open (LookUpRawFile())
  //  or if there is nothing to read anymore ==> ready = false

  // Decoding status
  bool ready = false;

  if(fDebugLevel>2) printf( "  BoardReaderMIMOSIS board %d::DecodeNextEvent() trying with event %d\n", fBoardNumber, fCurrentEventNumber);

  // Emulate stupid event with few pixels/sensor, one frame, one trigger
  fListOfTriggers.push_back( fCurrentEventNumber);
  fListOfTimestamps.push_back( 0);
  fListOfFrames.push_back( fCurrentEventNumber);
  for (size_t iSensor = 1; iSensor <= fNSensors; iSensor++) {
    // shoot 100 pixels in 1 row, which position depends on sensor
    for (size_t iCol = 0; iCol < 100; iCol++) {
      AddPixel( iSensor, 1, iSensor*10+50, iSensor*10+iCol);
    }
  }
  ready = true;


  return ready;

}

// --------------------------------------------------------------------------------------
void BoardReaderMIMOSIS::AddPixel( int iSensor, int value, int aLine, int aColumn, int aTime) {
   // Add a pixel to the vector of pixels
   // require the following info
   // - input = number of the sensors
   // - value = output value of this pixel
   // - line & column = position of the pixel in the matrix
   // - time = something related to the frame or the trigger

  if (fDebugLevel>3) printf("BoardReaderMIMOSIS::Addpixel adding pixel for sensor %d with value %d line %d row %d\n", iSensor, value, aLine, aColumn, aTime);

  fListOfPixels.push_back( BoardReaderPixel( iSensor, value, aLine, aColumn, aTime) );

}

// --------------------------------------------------------------------------------------

void BoardReaderMIMOSIS::SetVetoPixel( int noiseRun) {
  // Select the required function to veto the pixel depending on the run number.
  //

  if( fDebugLevel) printf( "  BoardReaderMIMOSIS board %d::SetVetoPixel with noise run number %d\n", fBoardNumber, noiseRun);
  if (noiseRun>0 ) fTool.SetVetoPixel( noiseRun);

}

// --------------------------------------------------------------------------------------
void BoardReaderMIMOSIS::PrintEventHeader() {
  // Print Event Header

}

// --------------------------------------------------------------------------------------
void BoardReaderMIMOSIS::SkipNextEvent() {
  // This method might be useful to ignore the next event
  //

  if(fDebugLevel) printf("  BoardReaderMIMOSIS: %d skipping current event %d\n", fBoardNumber, fCurrentEventNumber);

}

// --------------------------------------------------------------------------------------
void BoardReaderMIMOSIS::PrintStatistics(ostream &stream) {
  // Print statistics on all the events read by this board

 stream << "***********************************************" << endl;
 stream << " Board MIMOSIS " << fBoardNumber << " found:" << endl;
 stream << fTriggerCount << " triggers read overall," << endl;
 stream << fCurrentEventNumber << " events in total," << endl;
 stream << fNEventsWithOverflow << " events with an overflow (veto overflow " << fVetoOverflow << ")," << endl;
 stream << fFrameCount << " frames read overall." << endl;
 stream << "***********************************************" << endl;

}
