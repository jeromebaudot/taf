    /////////////////////////////////////////////////////////////
// Class Description of BoardReaderMIMOSIS
//
// Dedicated to decode output files from the PXI acquisition system
//  dedicated to MIMOSIS sensors
//
// Use Gilles Claus library to decode rawdata file, see code/inculde/mimo_daq_lib
//
/////////////////////////////////////////////////////////////
//
// created JB, 2021/04/21
// Last Modified: Ziad, 2021/06/04


#define AP 0
#include "TCanvas.h"
#include "TApplication.h"

#include "BoardReaderMIMOSIS.h"
#include "mimo_daq_lib/mimo_daq_lib.h"
#include "mimo_daq_lib/mimo_daq_lib.c"

ClassImp(BoardReaderMIMOSIS)

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


//------------------------------------------+-----------------------------------
MIS1__TBtRunRead* APP_VGPtRunRead;

MIS1__TBtAcqRawRec* APP_VGPtAcqSrc; // 26/05/2021 V1.1
                                    // Pointer on current acq of run = source Acq to be decoded

MIS1__TBtAcqW16A*  APP_VGPtAcqW16A; // 26/05/2021 V1.1
                                    // AcqW16A record = Flex RIO data reoorganized
                                    // Contains Acq data reordered in order to have one array of W16 for each MSis1
                                    // data provied by Flex RIO are splitted in two different memory blocks : one for each outputs
                                    // having data in one array / MSis 1 will ease data decoding, at the cost a of memory copy



MIS1__TBtAcqDec*   APP_VGPtAcqDec;  // 26/05/2021 V1.1
                                    // AcqDec record = Decoded acquisition
                                    // Contains decoded pixels + info on Acq, Frames : reggions nb, fired pixels nb, etc ...


// ---------------------------------------------------------------------------------
// Main : Mimosis 1 beam test run file access example, via class MIS1__TBtRunRead
// ---------------------------------------------------------------------------------

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


     // Size of records allocated for pixels decoding

     UInt32 VAcqW16ASz; // 26/05/2021 V1.1
                        // Size of AcqW16A record = Flex RIO data reoorganized
                        // Contains Acq data reordered in order to have one array of W16 for each MSis1
                        // data provied by Flex RIO are splitted in two different memory blocks : one for each outputs
                        // having data in one array / MSis 1 will ease data decoding, at the cost a of memory copy


     UInt32 VAcqDecSz;  // 26/05/2021 V1.1
                        // Size of AcqDec record = Decoded acquisition
                        // Contains decoded pixels + info on Acq, Frames : reggions nb, fired pixels nb, etc ...

     SInt32 ViFr;       // 26/05/2021 V1.1
                        // Frame index for pixels list printing

     UInt32  VPrintMaxPix; // 26/05/2021 V1.1
                           // Maximum number of pixels to print per frame, to avoid "endless loop" if lot of pixels in frame


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
     // Allocate record to convert one Acq
     // 26/05/2021 V1.1
     // ----------------------------------------------------------------

     // AcqW16A record

     APP_VGPtAcqW16A = MIS1__BT_FBtAcqW16AAlloc ( 1 /* Alloc */, &VAcqW16ASz );

     // Error

     if ( APP_VGPtAcqW16A == NULL ) {
       printf ( "\n" );
       printf ( "Allocation of AcqW16A of %.1f MB failed ! \n", VAcqW16ASz / 1024. / 1024. );
       printf ( "You can try to reduce max frames nb / Acq via constant MIS1__BT_VRS_MAX_FR_NB_PER_ACQ = %d \n", MIS1__BT_VRS_MAX_FR_NB_PER_ACQ );
       return (-1);
     }

     // OK

     printf ( "\n" );
     printf ( "Allocation of AcqW16A of %.1f MB  ... \n", VAcqW16ASz / 1024. / 1024. );
     printf ( "Max frames nb / Acq = %d \n", MIS1__BT_VRS_MAX_FR_NB_PER_ACQ );
     printf ( "Done :-) \n" );
     printf ( "\n" );


     // AcqDec

     APP_VGPtAcqDec = MIS1__BT_FBtAcqDecAlloc ( 1 /* Alloc */, &VAcqDecSz );

     // Error

     if ( APP_VGPtAcqDec == NULL ) {
       printf ( "\n" );
       printf ( "Allocation of AcqDec of %.1f MB failed ! \n", VAcqDecSz / 1024. / 1024. );
       printf ( "You can try to reduce max frames nb / Acq via constant   MIS1__BT_VRS_MAX_FR_NB_PER_ACQ   = %d \n", MIS1__BT_VRS_MAX_FR_NB_PER_ACQ );
       printf ( "You can try to reduce max pixels nb / Frame via constant MIS1__BT_FR_DEC_MAX_PIX_NB       = %d \n", MIS1__BT_FR_DEC_MAX_PIX_NB );
       return (-1);
     }


     // OK

     printf ( "\n" );
     printf ( "Allocation of AcqDec of %.1f MB  ... \n", VAcqDecSz / 1024. / 1024. );
     printf ( "Max frames nb / Acq   = %d \n", MIS1__BT_VRS_MAX_FR_NB_PER_ACQ );
     printf ( "Max pixels nb / Frame = %d \n", MIS1__BT_FR_DEC_MAX_PIX_NB );
     printf ( "Done :-) \n" );
     printf ( "\n" );



     // ----------------------------------------------------------------
     // Config run to read <=> Open run
     // ----------------------------------------------------------------

     // Sets run directory

     // sprintf ( VParRunRootDir, "%s", "C:/Data/msis1" );

     sprintf ( VParRunRootDir, "%s", "../../data" );


     // Sets run no

     VParRunNo = 2;

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
     // Print current Acq of run (Acq header fields + data as hex words)
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
     // Decode first Acq of run
     // Only for foirst MSis, a loop must done to convert all MSis
     // 26/05/2021 V1.1
     // ----------------------------------------------------------------

     // If can be used for enabling / disabling this part of the demo


     if (1) {

       // Get a pointer on current Acq

       APP_VGPtAcqSrc = APP_VGPtRunRead->FAcqGetPt ();

       if ( APP_VGPtAcqSrc == NULL    ) {
         printf ( "\n" );
         printf ( "Get a pointer on first Acq of run has failed ! \n" );
         printf ( "\n" );
         return (-1);
       }

       // Reset decoding memory, Rq : can be disable to reduce execution time, useful for debugging

       if ( 1 ) {
         memset ( APP_VGPtAcqW16A, 0, sizeof (MIS1__TBtAcqW16A) );
         memset ( APP_VGPtAcqDec , 0, sizeof (MIS1__TBtAcqDec) );
       }

       // Convert FleRIO memory image to list of W16 for each MSis

       VRet = MIS1__BT_FBtAcqW16AFill ( APP_VGPtAcqSrc , APP_VGPtAcqW16A, -1 /* FrNb, -1 => All */, 0 /* MeasExecTime */, 0 /* PrintLvl */, 80 /* FuncVers */ ); // A partir de là on peut lire des pixels.

       // Error

       if ( VRet < 0 ) {
         printf ( "Filling AcqW16A failed ! \n" );
         printf ( "\n" );
         return (-1);
       }

       // Decode MSis
       //
       // Done for first MSis = No 0
       // A loop with apram MSIsId = 0 to 5 must be done to convert the data of the full telescope
       // The conversion function MIS1__BT_FBtDecodeFrLight (...) will fail if the converted MSis does not exist in the run data

       VRet = MIS1__BT_FBtDecodeFr ( APP_VGPtAcqW16A, APP_VGPtAcqDec, 0 /* MSIsId 0 to 5 */, -1 /* FrNb */, 0 /* MeasExecTime */, 0 /* PrintLvl */ );

       if ( VRet < 0 ) {
         printf ( "Decoding MSis data failed ! \n" );
         printf ( "\n" );
         return (-1);
       }


       printf ( "\n" );
       printf ( "Decoding first Acq of run done :-) \n" );
       printf ( "\n" );


     }


     // ----------------------------------------------------------------
     // Print general info on first Acq of run, regions, pixels nb etc ...
     // Only for first MSis, a loop must done to print all MSis
     // 26/05/2021 V1.1
     // ----------------------------------------------------------------

     // If can be used for enabling / disabling this part of the demo

     if (1) {

       // Param PrintMode -  0 => Print all info, 1 => Print only region, fired pixels cnt

       VRet = MIS1__BT_FAcqDecPrintGen ( APP_VGPtAcqDec, 0 /* MSIsId 0 to 5 */, 0 /* PrintMode */ );

       if ( VRet < 0 ) {
         printf ( "Decoding MSis data failed ! \n" );
         printf ( "\n" );
         return (-1);
       }



       printf ( "\n" );
       printf ( "Printing info of first decoded Acq of run done :-) \n" );
       printf ( "\n" );
       printf ( "Results have been written in file %s \n", APP_VGPtRunRead->FMsgLogSetFilename () );
       printf ( "Results have been printed in console if macro CC_APP_MSG_LOG_VIA_PRINTF is enabled \n" );
       printf ( "\n" );

     }


     // ----------------------------------------------------------------
     // Print pixels of first Acq of run
     // Only for first MSis, a loop must done to print all MSis
     // Only frames with pixels are printed
     //
     // Where to find info to access pixels (which variable etc ) ?
     // - Read following sourcer code
     // - Read source code of MIS1__BT_FAcqDecPrintPix (...) in msis1_data.c
     // - Read type defintion of MIS1__TBtAcqDec  in msis1_data.typ
     //
     // 26/05/2021 V1.1
     // ----------------------------------------------------------------

     // If can be used for enabling / disabling this part of the demo

     if (1) {

       VPrintMaxPix = 100; // Maximum number of pixels to print per frame, to avoid "endless loop" if lot of pixels in frame

       // Loop to scan the frame of Acq, WARNING : Frame nb to scan is hard coded here to 100, is a run has less than 100 fr/acq it will generate an error

       for ( ViFr = 0; ViFr < 100; ViFr++ ) {

         // Only frames with pixels are printed

         if ( APP_VGPtAcqDec->ResAAFrHead[ 0 /* MSisId */][ViFr].FiredPixNb > 0 ) {

           VRet = MIS1__BT_FAcqDecPrintPix ( APP_VGPtAcqDec,  0 /* MSIsId 0 to 5 */, ViFr, VPrintMaxPix, 0 /* PrintMode */ );

           if ( VRet < 0 ) {
             printf ( "Printing pixels of frame No %d of MSis  No 0 failed ! \n", ViFr );
             printf ( "\n" );
             return (-1);
           }

         }

       }  // End for ( ViFr )

       // Param PrintMode -  0 => Not used now


       printf ( "\n" );
       printf ( "$$$ Printing pixels of MSis No 0 of first decoded Acq of run done :-) \n" );
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
     // Print current Acq of run (Acq header fields + data as hex words)
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
     // Free record to convert one Acq
     // 26/05/2021 V1.1
     // ----------------------------------------------------------------

     // AcqW16A record

     VRet = MIS1__BT_FBtAcqW16AFree ( APP_VGPtAcqW16A );

     APP_VGPtAcqW16A = NULL;

     // Error

     if ( VRet < 0 ) {
       printf ( "\n" );
       printf ( "Free of AcqW16A failed ! \n" );
       return (-1);
     }

     // OK

     printf ( "\n" );
     printf ( "Free of AcqW16A done :-) \n" );


      // AcqDec

     VRet = MIS1__BT_FBtAcqDecFree ( APP_VGPtAcqDec );

     APP_VGPtAcqDec = NULL;


     // Error

     if ( VRet < 0 ) {
       printf ( "\n" );
       printf ( "Free of AcqDec failed ! \n" );
       return (-1);
     }

     // OK

     printf ( "\n" );
     printf ( "Free of AcqDec done :-) \n" );



     // ----------------------------------------------------------------
     // Class destruction
     // ----------------------------------------------------------------

     // Class destruction

     delete ( APP_VGPtRunRead );

     printf ( "\n" );
     printf ( "**********************************************************\n" );
     printf ( "End of Mimosis 1 data run files access library demo \n" );
     printf ( "**********************************************************\n" );
     printf ( "\n" );

     return 0;
}
//------------------------------------------+-----------------------------------
BoardReaderMIMOSIS::BoardReaderMIMOSIS(int boardNumber, char* dataPath, int runNumber, int nSensors, int triggerMode, int eventBuildingMode, int endianness) {
  // Board creator
  // Load the rawdata file
  //
  // Ziad, 2021


  int VParHdPrintTriggers     = 5; // Nb of triggers to print -1 => All, 0 => None, N > 0 => N first triggers
  int VParHdPrintFrCnt        = 3; // Print frame counter of 6 x MSis 1 over N frames, -1 => All frames, 0 => None, N > 0 => N first frames
  int VParHdPrintFiredPixels  = 1; // Print fired pixels (per MSis1, per submatrices), 0 => No, 1 => Print, NOT CALCULATD NOW on 19/05/2021

  fDebugLevel = 0; // Used for printing info later
  fBoardNumber = boardNumber;
  fRunNumber = runNumber;
  fNSensors = nSensors;
  fTriggerMode = triggerMode;
  fEventBuildingMode = eventBuildingMode;
  fEndianness = endianness;
  fVetoOverflow = false; // set later by SetVetoPixel
  fisfirstAcq = true;

  fBadDecFrameCounter = 0;

  fReachEndOfRun = 0;

  SInt32 VRet;                              // Error code returned by functions ( 0 => ok, < 0 => error )

  UInt32 VAcqW16ASz; // 26/05/2021 V1.1
                          // Size of AcqW16A record = Flex RIO data reoorganized
                          // Contains Acq data reordered in order to have one array of W16 for each MSis1
                          // data provied by Flex RIO are splitted in two different memory blocks : one for each outputs
                          // having data in one array / MSis 1 will ease data decoding, at the cost a of memory copy


  UInt32 VAcqDecSz;  // 26/05/2021 V1.1
                          // Size of AcqDec record = Decoded acquisition
                          // Contains decoded pixels + info on Acq, Frames : reggions nb, fired pixels nb, etc ...

  cout << "*****************************************" << endl;
  cout << "    < BoardReaderMIMOSIS constructor >      " << endl;
  cout << "*****************************************" << endl;
  cout << "Creating a BoardReaderMIMOSIS" << endl;
  cout << " * board nb: " << fBoardNumber << endl;
  cout << " * runNumber : " << runNumber <<  endl;
  cout << " * nb of sensors: " << fNSensors << endl;
  cout << " * trigger mode: " << fTriggerMode << endl;
  cout << " * event building mode: " << fEventBuildingMode << endl;
  cout << " * Endiannes: " << fEndianness << " => " << (fEndianness==0?"NO SWAP":"SWAP") << endl;
  cout << " * usage of veto for event with overflow: " << fVetoOverflow << " => " << (fVetoOverflow?"YES":"NO") << endl;
  cout << " * dataPath : " << dataPath << endl;

  // Initialization
  fCurrentFileNumber = 0; // See also AddFileList
  fNoMoreFile = false;
  fCurrentEventNumber = 0;
  fCurrentTriggerNumber = 0;
  fTriggerCount = 0;
  fFrameCount = 0;
  fNEventsWithOverflow = 0;

  fCurrentAcqNumber = -1;
  fCurrentFrameNumber = 0;

  // Initialization of Current Acquisition Number and Current Frame Number

  // ----------------------------------------------------------------
  // Reading Class creation
  // ----------------------------------------------------------------
   MIS1__TBtRunCnfRec* VPtRunConf = NULL; // Pointer to run conf record
   MIS1__TBtAcqRawRec* VPtAcq     = NULL; // Pointer to current Acq

    APP_VGPtRunRead = new MIS1__TBtRunRead();

     if ( APP_VGPtRunRead == NULL ) {
        printf ( "\n" );
        printf ( "ERROR : Creation of MIS1__TBtRunRead  failed ! \n" );
       }

      printf ( "\n" );
      printf ( "MIS1__TBtRunRead class creation done :-) \n" );
      printf ( "\n" );

    // Initialisation required by DAQ library
    APP__TContext* VPtCont = &APP__VGContext;

    char msgFile[100], errFile[100]; // JB 2011/06/28
   // int runNumber ;
    sprintf( msgFile, "Results/%d/msg_run%d.txt", runNumber, runNumber);
    sprintf(msgFile,"%s", fTool.LocalizeDirName( msgFile)); // JB 2011/07/07
    sprintf( errFile, "Results/%d/err_run%d.txt", runNumber, runNumber);
    sprintf(errFile,"%s", fTool.LocalizeDirName( errFile)); // JB 2011/07/07

    cout << " * msgFile : " << msgFile << " errFile = " << errFile << endl;

    // ZE 2021/06/01 access configuration file

    VRet = APP_VGPtRunRead->FRunConf ( dataPath, runNumber, 0 /* PrintRunHeader */, MIS1__BT_RUN_RD_FILE_FORMAT );
    if( VRet<0 ) {
	cout << endl << "ERROR: cannot configure run -> return of RunConf = " << VRet << endl;
	cout << "  Consult " << errFile << " to investigate." << endl << endl;
	return;
  }

    ERR_FBegin             ( ( APP_VGErrFileLogLvl != 0 ) /* Enable */, errFile );
    ERR_FSetFileLogLevel   ( APP_VGErrFileLogLvl   );
    ERR_FSetUserLogLevel   ( APP_VGErrUserLogLvl   );
    MSG_FBegin             ( (APP_VGMsgFileLogLvl != 0) /* Enable */,  msgFile );
    MSG_FSetFileLogLevel   ( APP_VGMsgFileLogLvl   );
    MSG_FSetUserLogLevel   ( APP_VGMsgUserLogLvl   );

    // ZE 2021/06/03 two following steps to get the number of frames per acquisition

    VPtRunConf = APP_VGPtRunRead->FRunHeaderGet ( 1 /* Print */ );
    fnbFrPerAcq = VPtRunConf->FrNbPerAcq ;
    cout << " * Number of Frames per Acq : " << fnbFrPerAcq << endl;

    // In case of more info required in log_msg and log_err use the syntax below
    /*
    err_trace   (( ERR_OUT, "This is a trace message   - VMyVar=%d", VMyVar ));
    err_warning (( ERR_OUT, "This is a warning message - VMyVar=%d", VMyVar ));
    err_error   (( ERR_OUT, "This is an error message  - VMyVar=%d", VMyVar ));


    msg  (( MSG_OUT, "This is the log file of run : %d \n", runNumber));
    msg  (( MSG_OUT, "Data Path : %s \n", dataPath));
    msg  (( MSG_OUT, "Number of Frames per Acquisition : %d \n", VPtRunConf->FrNbPerAcq));
   */

    VRet = APP_VGPtRunRead->FAcqHeadPrintOptSet ( VParHdPrintTriggers, VParHdPrintFrCnt, VParHdPrintFiredPixels );

          if ( VRet < 0 ) {
            printf ( "\n" );
            printf ( "ERROR: Configure Acq header printing option has failed \n" );
            printf ( "\n" );
            return;
          }

    // AcqW16A record

       APP_VGPtAcqW16A = MIS1__BT_FBtAcqW16AAlloc ( 1 /* Alloc */, &VAcqW16ASz );

       if ( APP_VGPtAcqW16A == NULL ) {
           printf ( "\n" );
           printf ( "Allocation of AcqW16A of %.1f MB failed ! \n", VAcqW16ASz / 1024. / 1024. );
           printf ( "You can try to reduce max frames nb / Acq via constant MIS1__BT_VRS_MAX_FR_NB_PER_ACQ = %d \n", MIS1__BT_VRS_MAX_FR_NB_PER_ACQ );
         }

         // OK

        if(fDebugLevel>1) {
         printf ( "\n" );
         printf ( "Allocation of AcqW16A of %.1f MB  ... \n", VAcqW16ASz / 1024. / 1024. );
         printf ( "Max frames nb / Acq = %d \n", MIS1__BT_VRS_MAX_FR_NB_PER_ACQ );
         printf ( "Done :-) \n" );
         printf ( "\n" );
        }

       // AcqDec

         APP_VGPtAcqDec = MIS1__BT_FBtAcqDecAlloc ( 1 /* Alloc */, &VAcqDecSz );

         // Error

         if ( APP_VGPtAcqDec == NULL ) {
           printf ( "\n" );
           printf ( "Allocation of AcqDec of %.1f MB failed ! \n", VAcqDecSz / 1024. / 1024. );
           printf ( "You can try to reduce max frames nb / Acq via constant   MIS1__BT_VRS_MAX_FR_NB_PER_ACQ   = %d \n", MIS1__BT_VRS_MAX_FR_NB_PER_ACQ );
           printf ( "You can try to reduce max pixels nb / Frame via constant MIS1__BT_FR_DEC_MAX_PIX_NB       = %d \n", MIS1__BT_FR_DEC_MAX_PIX_NB );
         //  return (-1);
         }


         // OK
       //if(fDebugLevel>1) {
         printf ( "\n" );
         printf ( "Allocation of AcqDec of %.1f MB  ... \n", VAcqDecSz / 1024. / 1024. );
         printf ( "Max frames nb / Acq   = %d \n", MIS1__BT_VRS_MAX_FR_NB_PER_ACQ );
         printf ( "Max pixels nb / Frame = %d \n", MIS1__BT_FR_DEC_MAX_PIX_NB );
         //printf ( "Done :-) \n" );
         //printf ( "\n" );
       //}


    cout << "*****************************************" << endl;
    cout << "    < BoardReaderMIMOSIS constructor DONE >      " << endl;
    cout << "*****************************************" << endl;
}

//------------------------------------------+-----------------------------------
BoardReaderMIMOSIS::~BoardReaderMIMOSIS()
{

    // ----------------------------------------------------------------
        // Free record to convert one Acq
        // 26/05/2021 V1.1
        // ----------------------------------------------------------------

        // AcqW16A record
    SInt32 VRet;                              // Error code returned by functions ( 0 => ok, < 0 => error )

        VRet = MIS1__BT_FBtAcqW16AFree ( APP_VGPtAcqW16A );

        APP_VGPtAcqW16A = NULL;

        // Error

        if ( VRet < 0 ) {
          printf ( "\n" );
          printf ( "Free of AcqW16A failed ! \n" );
         // return (-1);
        }

        // OK
        if (fDebugLevel > 0) {
        printf ( "\n" );
        printf ( "Free of AcqW16A done :-) \n" );
         }
      // Free Memory AcqDec

        VRet = MIS1__BT_FBtAcqDecFree ( APP_VGPtAcqDec );

        APP_VGPtAcqDec = NULL;

        // Error

        if ( VRet < 0 ) {
          printf ( "\n" );
          printf ( "Free of AcqDec failed ! \n" );
        //  return (-1);
        }

        // OK
       if (fDebugLevel > 0) {
        printf ( "\n" );
        printf ( "Free of AcqDec done :-) \n" );
      }


//  delete fCurrentEvent;
//  delete fInputFileName;

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
//  fCurrentEvent   = NULL;  // Allow to know wether data are correct
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

     fListOfTriggers.push_back( fCurrentEventNumber);
     fListOfTimestamps.push_back( 0);
     fListOfFrames.push_back( fCurrentEventNumber);

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

   SInt32 VRet;                              // Error code returned by functions ( 0 => ok, < 0 => error )


   MIS1__TBtAcqRawRec* VPtAcq     ; // Pointer to current Acq

  // UInt32 VAcqW16ASz; // 26/05/2021 V1.1
                        // Size of AcqW16A record = Flex RIO data reoorganized
                        // Contains Acq data reordered in order to have one array of W16 for each MSis1
                        // data provied by Flex RIO are splitted in two different memory blocks : one for each outputs
                        // having data in one array / MSis 1 will ease data decoding, at the cost a of memory copy


  //  UInt32 VAcqDecSz;  // 26/05/2021 V1.1
                        // Size of AcqDec record = Decoded acquisition
                        // Contains decoded pixels + info on Acq, Frames : reggions nb, fired pixels nb, etc ...

    UInt8 VResReachEndOfRun = (UInt8) fReachEndOfRun;


  // Decoding status

    bool ready = false;

  if(fDebugLevel>1) printf( "  BoardReaderMIMOSIS board %d::DecodeNextEvent() trying with event %d (current Acq=%d, frame=%d)\n", fBoardNumber, fCurrentEventNumber, fCurrentAcqNumber, fCurrentFrameNumber);

  // Check if first Acquisition or if CurrentFrameNumber less than fnbFrPerAcq; fnbFrPerAcq is not mandatory equal to MIS1__BT_VRS_MAX_FR_NB_PER_ACQ

    if ((fCurrentFrameNumber % fnbFrPerAcq == 0) || (fCurrentFrameNumber % MIS1__BT_VRS_MAX_FR_NB_PER_ACQ == 0)) {

        if (fisfirstAcq == true) {
            if(fDebugLevel>1) cout << "BoardReaderMIMOSIS  Getting the pointer for the first acquisition " << endl;
            VPtAcq = APP_VGPtRunRead->FAcqFirst ( 0 /* ChkAcqHead */, 0 /* PrintAcqHead */ );
            fCurrentAcqNumber ++;
            fisfirstAcq = false;
            fCurrentFrameNumber = 0; // ZE 2021/06/04
            }
        else {
             if(fDebugLevel>1) printf (" BoardReaderMIMOSIS Changing to NextAcq % d since fCurrentFrameNumber = % d \n", fCurrentAcqNumber + 1, fCurrentFrameNumber);
             VPtAcq = APP_VGPtRunRead->FAcqNext ( 0 /* ChkAcqHead */, 0 /* PrintAcqHead */, &VResReachEndOfRun );
             fCurrentAcqNumber ++;
             fCurrentFrameNumber = 0; // ZE 2021/06/04
            }
    }

    fReachEndOfRun = (int) VResReachEndOfRun;

    if(fDebugLevel>1) {
    printf("fReachEndOfRun = %d ", fReachEndOfRun);
    printf("\n");
    }

    // Get a pointer for the first Acquisition
    APP_VGPtAcqSrc = (MIS1__TBtAcqRawRec*) APP_VGPtRunRead->FAcqGetPt ();

    if ( APP_VGPtAcqSrc == NULL    ) {
      printf ( "\n" );
      printf ( "WARNING: BoardReaderMIMOSIS Get a pointer on first Acq of run has failed ! \n" );
      printf ( "\n" );
      return (-1);
    }
    else {
    ready = true;

        if(fDebugLevel>1) {
            printf ( "\n" );
            printf ( " BoardReaderMIMOSIS Get a pointer on first Acq of run done ! \n" );
            printf ( "\n" );
            }

    }
    /*
      if ( 1 ) {
        memset ( APP_VGPtAcqW16A, 0, sizeof (MIS1__TBtAcqW16A) );
        memset ( APP_VGPtAcqDec , 0, sizeof (MIS1__TBtAcqDec) );
      }
    */
    // Convert FleRIO memory image to list of W16 for each MSis
    VRet = MIS1__BT_FBtAcqW16AFill ( APP_VGPtAcqSrc , APP_VGPtAcqW16A, -1 /* FrNb, -1 => All */, 0 /* MeasExecTime */, 0 /* PrintLvl */, 80 /* FuncVers */ );

    // Error

      if ( VRet < 0 ) {

          printf ( "WARNING: BoardReaderMIMOSIS Filling AcqW16A failed ! \n" );
          printf ( "\n" );
          return (-1);

        }
       else if(fDebugLevel>1) {

           printf ( "\n" );
           printf ( " BoardReaderMIMOSIS Filling AcqW16A done ! \n" );
           printf ( "\n" );

        }

        // ZE 2021/06/02, just for test. Has to write a method specific to TAF
        //VRet = MIS1__BT_FAcqDecPrintGen ( APP_VGPtAcqDec, 0 /* MSIsId 0 to 5 */, 0 /* PrintMode */ );

         // Gilles's method rewritten for TAF in DecodeFrame() method
        //VRet = MIS1__BT_FAcqDecPrintPix ( APP_VGPtAcqDec,  0 /* MSIsId 0 to 5 */, ViFr, VPrintMaxPix, 0 /* PrintMode */ );
        DecodeFrame();

    if (fReachEndOfRun > 0){
        ready = false;
        printf(" End Of Run Reached --> fReachEndOfRun = % d \n", fReachEndOfRun);
        printf(" Number of processed events/acquisitions : %d \n ", fCurrentAcqNumber );
        }


    return ready;

}

//------------------------------------------+-----------------------------------

bool BoardReaderMIMOSIS::DecodeFrame() {

   SInt32 VRet;

   MIS1__TPixXY VPix;
   SInt32 VPrintMaxPix = 100;

   // ZE 2021/06/02. This is Gilles's standalone methode. Had to write a method specific to TAF.
   //VRet = MIS1__BT_FAcqDecPrintPix ( APP_VGPtAcqDec,  0 /* MSIsId 0 to 5 */, frameID, VPrintMaxPix, 0 /* PrintMode */ );

  /*
    fListOfTriggers.push_back( fCurrentEventNumber);
    fListOfTimestamps.push_back( 0);
    fListOfFrames.push_back( fCurrentEventNumber);
    */
    // ZE 2021/06/03. Loop over sensors to decode pixels of current frame.
    for ( int MSisId = 0; MSisId < fNSensors; MSisId++ ) {


        VRet = MIS1__BT_FBtDecodeFr ( APP_VGPtAcqW16A, APP_VGPtAcqDec, MSisId, fCurrentFrameNumber, 0 /* MeasExecTime */, 0 /* PrintLvl */ );

        if ( VRet < 0 ) {
            printf ( "WARNING: Decoding MSis data failed for Sensor : %d and FrameNb : %d \n", MSisId, fCurrentFrameNumber );
            printf ( "\n" );
            fBadDecFrameCounter ++;
            return (-1);
        }
 //       VRet = MIS1__BT_FAcqDecPrintGen ( APP_VGPtAcqDec, MSisId, 0 /* PrintMode */ );

 //           if ( VRet < 0 ) {
 //               printf ( "Printing info of decoded Acq failed for sensor : % d :-( \n", MSisId );
 //               printf ( "\n" );
 //            return (-1);
 //          }

       // Decode pixels only for frames with FiredPixNb > 0
         if ( APP_VGPtAcqDec->ResAAFrHead[MSisId][fCurrentFrameNumber].FiredPixNb > 0 ) {

             if (fDebugLevel > 3) {
                 if (fCurrentFrameNumber >= APP_VGPtAcqDec->ResAFrNb[MSisId]) //ZE 2021/06/04 Check if frameID > nbFramesPerAcq

                     printf("fCurrentFrameNumber %d Exceeded the number of frames %d for sensor %d \n ", fCurrentFrameNumber, APP_VGPtAcqDec->ResAFrNb[MSisId], MSisId);
                     printf(" nbFiredPix : %.4d in AcqID : %.4d - frID :  %.4d - SensorID : %.4d \n ", APP_VGPtAcqDec->ResAAFrHead[MSisId][fCurrentFrameNumber].FiredPixNb, fCurrentAcqNumber, fCurrentFrameNumber, MSisId );
             }

            for ( int ViPix = 0; ViPix < APP_VGPtAcqDec->ResAAFrHead[MSisId][fCurrentFrameNumber].FiredPixNb; ViPix++ ) {

                VPix = APP_VGPtAcqDec->ResAAAFrPix[MSisId][fCurrentFrameNumber][ViPix];

                if (fDebugLevel > 3) {
                        printf(" Pixel [%.4d] : Y = %.4d - X  = %.4d - frameID = %d \n ", ViPix, VPix.C.y, VPix.C.x, fCurrentFrameNumber );
                }
                AddPixel( MSisId, 1, VPix.C.y ,VPix.C.x );

            }// End of Loop over pixels in a sensor for the current frame

        } // End of if condition : FiredPixNb > 0

    }// End of Loop over sensors

    fCurrentFrameNumber ++ ;

    return 0;
}


// --------------------------------------------------------------------------------------
void BoardReaderMIMOSIS::AddPixel( int iSensor, int value, int aLine, int aColumn, int aTime) {
   // Add a pixel to the vector of pixels
   // require the following info
   // - input = number of the sensors
   // - value = output value of this pixel
   // - line & column = position of the pixel in the matrix
   // - time = something related to the frame or the trigger

  //if (fDebugLevel>3)
    //  printf("BoardReaderMIMOSIS::Addpixel adding pixel for sensor %d with value %d line %d row %d\n", iSensor, value, aLine, aColumn, aTime);

    fListOfPixels.push_back( BoardReaderPixel( iSensor + 1, value, aLine, aColumn, 0 /*TimeStamp has to be given for the constructor*/) );

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
 stream << fFrameCount << " frames read overall " << endl;
 stream << fBadDecFrameCounter << " frames with decoding errors. " << endl;
 stream << "***********************************************" << endl;

}

// --------------------------------------------------------------------------------------
