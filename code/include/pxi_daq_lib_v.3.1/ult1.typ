
/*******************************************************************************
File      : x:\lib\com\maps\ult1\ult1.typ
Goal      : Types definition of Ultimate 1 library.
          : It provides Ultimate 1 types definition and data handling functions.
Prj date  : 28/02/2011
File date : 28/02/2011
Doc date  : //200
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


#ifndef ULT1_TYP
#define ULT1_TYP

#ifdef ROOT_ROOT
  typedef UInt32 ULT1__TColor;
  #define  clWhite 0
  #define  clBlack 0
  #define  clBlue  0
  #define  clRed 0  
#else  
  typedef TColor ULT1__TColor;
#endif





/* ============================= */
/*  Lib context                  */
/*  Contain all global variables */
/* ----------------------------- */
/* Date : 24/11/2008             */
/* ============================= */

// $ typedef struct {
// $
// $  SInt8 FileErrLogLvl;
// $  char  FileErrFile[GLB_FILE_PATH_SZ];
// $
// $} ULT1__TContext;

/* ============================================================================ */
/*  Discri register, 2 views                                                    */
/*  - Array of W32 words                                                        */
/*  - Array of bits                                                             */
/* ---------------------------------------------------------------------------- */
/* WARNING : It is not a bit mapping but conversion must be done by a function  */
/* ---------------------------------------------------------------------------- */
/* Date : 24/11/2008                                                            */
/* ============================================================================ */

typedef struct {

  UInt32 ADataW32[ULT1__REG_DISCRI_W32_SZ];
  SInt8  ADataBit[ULT1__REG_DISCRI_BIT_SZ];

} ULT1__TRegDiscri;

/* ============================================================================ */
/*  Discri register as array of W32                                             */
/* ---------------------------------------------------------------------------- */
/* Date : 25/11/2008                                                            */
/* ============================================================================ */

typedef struct {

  UInt32 AW32[ULT1__REG_DISCRI_W32_SZ];

} ULT1__TRegDiscriW32;


/* ============================================================================ */
/*  Discri register as array of bit                                             */
/* ---------------------------------------------------------------------------- */
/* Date : 25/11/2008                                                            */
/* ============================================================================ */

typedef struct {

  SInt8  ABit[ULT1__REG_DISCRI_BIT_SZ];

} ULT1__TRegDiscriBit;


/* ============================================================================ */
/*  Discri matrix dual view : W32 or Bit                                        */
/* ---------------------------------------------------------------------------- */
/* Date : 24/11/2008                                                            */
/* ============================================================================ */

typedef struct {

  ULT1__TRegDiscri ALine[ULT1__MAT_DISCRI_LINES_NB];

} ULT1__TMatDiscri;

/* ============================================================================ */
/*  Discri matrix view as W32 array                                             */
/* ---------------------------------------------------------------------------- */
/* Date : 25/11/2008                                                            */
/* ============================================================================ */

typedef struct {

  UInt32 AALineW32[ULT1__MAT_DISCRI_LINES_NB][ULT1__REG_DISCRI_W32_SZ];

} ULT1__TMatDiscriW32;

/* ============================================================================ */
/*  Discri matrix view as bit array                                             */
/* ---------------------------------------------------------------------------- */
/* Date : 25/11/2008                                                            */
/* ============================================================================ */

typedef struct {

  SInt8 AALineCol[ULT1__MAT_DISCRI_LINES_NB][ULT1__REG_DISCRI_BIT_SZ];

} ULT1__TMatDiscriBit;


typedef struct {

  float AALineCol[ULT1__MAT_DISCRI_LINES_NB][ULT1__REG_DISCRI_BIT_SZ];

} ULT1__TMatDiscriBitf;


/* ============================================================================ */
/*  Discri matrix view as bit array - BUT scale 1/4                             */
/* ---------------------------------------------------------------------------- */
/* Date : 23/07/2009                                                            */
/* ============================================================================ */

typedef struct {
  
  SInt8 AALineCol[ULT1__MAT_DISCRI_LINES_NB / 2][ULT1__REG_DISCRI_BIT_SZ / 2];
  
} ULT1__TMatDiscriBitHalfScale;



/* ============================================================================ */
/*  After Zero Sup register, 2 views                                            */
/*  - Array of W32 words                                                        */
/*  - Array of bits                                                             */
/* ---------------------------------------------------------------------------- */
/* WARNING : It is not a bit mapping but conversion must be done by a function  */
/* ---------------------------------------------------------------------------- */
/* Date : 24/11/2008                                                            */
/* ============================================================================ */

/* Not implemented on Ultimate

typedef struct {

  UInt32 ADataW32[ULT1__REG_AFTER_ZS_W32_SZ];
  SInt8  ADataBit[ULT1__REG_AFTER_ZS_BIT_SZ];

} ULT1__TRegAfterZs;
*/

/* ============================================================================ */
/*  After Zero Sup register as array of W32                                     */
/* ---------------------------------------------------------------------------- */
/* Date : 25/11/2008                                                            */
/* ============================================================================ */

/* Not implemented on Ultimate

typedef struct {

  UInt32 AW32[ULT1__REG_AFTER_ZS_W32_SZ];

} ULT1__TRegAfterZsW32;
*/

/* ============================================================================ */
/*  After Zero Sup register as array of bit                                     */
/* ---------------------------------------------------------------------------- */
/* Date : 25/11/2008                                                            */
/* ============================================================================ */

/* Not implemented on Ultimate

typedef struct {

  SInt8  ABit[ULT1__REG_AFTER_ZS_BIT_SZ];

} ULT1__TRegAfterZsBit;
*/

/* ============================================================================ */
/*  After Mux register, 2 views                                                 */
/*  - Array of W32 words                                                        */
/*  - Array of bits                                                             */
/* ---------------------------------------------------------------------------- */
/* WARNING : It is not a bit mapping but conversion must be done by a function  */
/* ---------------------------------------------------------------------------- */
/* Date : 24/11/2008                                                            */
/* ============================================================================ */

typedef struct {

  UInt32 ADataW32[ULT1__REG_AFTER_MUX_W32_SZ];
  SInt8  ADataBit[ULT1__REG_AFTER_MUX_BIT_SZ];

} ULT1__TRegAfterMux;

/* ============================================================================ */
/*  After Mux register as array of W32                                          */
/* ---------------------------------------------------------------------------- */
/* Date : 25/11/2008                                                            */
/* ============================================================================ */

typedef struct {

  UInt32 AW32[ULT1__REG_AFTER_MUX_W32_SZ];

} ULT1__TRegAfterMuxW32;


/* ============================================================================ */
/*  After Mux register as array of bit                                          */
/* ---------------------------------------------------------------------------- */
/* Date : 25/11/2008                                                            */
/* ============================================================================ */

typedef struct {

  SInt8  ABit[ULT1__REG_AFTER_MUX_BIT_SZ];

} ULT1__TRegAfterMuxBit;



/* ======================================================= */
/* Frame provided by ULT1 DAQ, it's independent of output  */
/* mode BUT data are not organized as in ULT1__TZsFFrame   */
/* The format is :                                         */
/* - Header                                                */
/* - Frames counter                                        */
/* - Data length = W16 number of useful data ( excluding   */
/*   trailer and bits at zero at end of frame )            */
/* - Array of W16 data                                     */
/* ------------------------------------------------------- */
/* This is a FIXED size record which contains the maximum  */
/* possible W16 defined by ULT1__ZS_FFRAME_RAW_MAX_W16     */
/* ------------------------------------------------------- */
/* Date : 08/12/2008                                       */
/* ======================================================= */


 typedef struct {

   ASIC__TFrameStatus SStatus;
  
   UInt32 Header;
   UInt32 FrameCnt;
   UInt32 DataLength;
  
   UInt32 Trailer;
   UInt32 Zero;
   UInt32 Zero2;
  
  
   UInt16 ADataW16[ULT1__ZS_FFRAME_RAW_MAX_W16]; // MUST BE AT END OF RECORD !
  
  
 } xxx__TZsFFrameRaw;





 //typedef struct {
 //
 //  ASIC__TFrameStatus SStatus; // Informations about frame, see ASIC__TFrameStatus in asic.typ

 //  UInt32 Header;      // Header of Mimosa 26 frame
 //  UInt32 FrameCnt;    // Frame counter of Mimosa 26 frame

 //  UInt32 DataLength;  // Useful length in W16 unit of data contains in ADataW16 array
                      // - B00B16 -> Length on first output
                      // - B17B23 -> Length on second output
                      //
                      // Add the two values to get the total length

 //  UInt32 Trailer;     // Trailer of Mimosa 26 frame

 //  UInt32 Zero;        // Zero at end of Mimosa 26 frame -> But NOW they are set to 0xFFFFFFFF by DAQ sw
 //  UInt32 Zero2;       // Zero at end of Mimosa 26 frame -> But NOW they are set to 0xFFFFFFFF by DAQ sw
                      //
                      // It's strange ... please don't ask why it's sugar and it's written salt on the box ...
                      //
                      // At the beginning it was last fields of Mimosa 26 frame, which are set to 0, now it's
                      // overwritten by sw in order to mark the end of information fields before data fields
                      // and 0xFFFFFFFF is a better value than zero for this purpose.


 //  UInt16 ADataW16[ULT1__ZS_FFRAME_RAW_MAX_W16]; // MUST BE AT END OF RECORD !

 //} ULT1__TZsFFrameRaw; // F in FFrameRaw means Fixed size frame



/* =================================================== */
/*  Field States/Line of Zero Sup frame, 2 views       */
/*  - W16 word                                         */
/*  - Fields                                           */
/* --------------------------------------------------- */
/* It's bit mapping => No conversion function required */
/* --------------------------------------------------- */
/* Date : 24/11/2008                                   */
/* =================================================== */

 //typedef union {

 //  UInt16 W16;

 //  struct {

  //   UInt16 StateNb  :  4;
 //    UInt16 LineAddr : 11;
 //    UInt16 Ovf      :  1;

 //  } F;

 //} ULT1__TStatesLine;

/* =================================================== */
/*  Field State of Zero Sup frame, 2 views             */
/*  - W16 word                                         */
/*  - Fields                                           */
/* --------------------------------------------------- */
/* It's bit mapping => No conversion function required */
/* --------------------------------------------------- */
/* Date : 24/11/2008                                   */
/* =================================================== */

// typedef union {

//   UInt16 W16;

//   struct {

//     UInt16 HitNb   :  2;
//     UInt16 ColAddr : 11;
//     UInt16 NotUsed :  3;

//   } F;

// } ULT1__TState;


/* ======================================================= */
/* One list of states associated to one line               */
/* - States/Lines information                              */
/* - States list                                           */
/* ------------------------------------------------------- */
/* This is a FIXED size record which contains all states   */
/* of one line, max ULT1__ZS_FFRAME_MAX_STATES_NB_PER_LINE */
/* ------------------------------------------------------- */
/* Date : 24/11/2008                                       */
/* ======================================================= */

// typedef struct {

//  ULT1__TStatesLine StatesLine;
//   ULT1__TState      AStates[ULT1__ZS_FFRAME_MAX_STATES_NB_PER_STATES_REC];

// } ULT1__TZsFStatesRec; // F in FStatesRec means Fixed size record


/* ======================================================= */
/* Frame provided by ULT1, this is the final result after  */
/* data processing depending of output mode selected       */
/* - Header                                                */
/* - Frames counter                                        */
/* - Data length = W16 number of useful data ( excluding   */
/*   trailer and bits at zero at enbd of frame )           */
/* ------------------------------------------------------- */
/* This is a FIXED size record which contains all states   */
/* of one line, max ULT1__ZS_FFRAME_MAX_STATES_NB_PER_LINE */
/* ------------------------------------------------------- */
/* Date : 24/11/2008                                       */
/* ------------------------------------------------------- */

//typedef struct {

//ASIC__TFrameStatus SStatus;

//UInt32 Header;
//UInt32 FrameCnt;
//UInt32 DataLength;
// SInt16 TrigSignalLine;
//SInt8  TrigSignalClk;
// SInt16 TrigLine;

// UInt32 StatesRecNb; // It's NOT a ULT1 frame field, it's calculated by sw
                      // It's the number of valid record in AStatesRec

// ULT1__TZsFStatesRec AStatesRec[ULT1__ZS_FFRAME_MAX_STATES_REC];

//  UInt32 Trailer;
//   UInt32 Zero;
//   UInt32 Zero2;

// } ULT1__TZsFFrame; // F in FFrame means Fixed size frame




#ifndef APP__RMV_ULT1__TCDiscriFile
#ifndef APP__RMV_CLASSES

// 31/01/2009

class ULT1__TCDiscriFile : public FIL__TCBinFile {


  private :

  protected :


  public :

    ULT1__TCDiscriFile ( char* ErrLogFile, SInt8 EnableErrLog, SInt8 ErrLogLvl );
    ~ULT1__TCDiscriFile ();

    SInt32                 PubFConf  ( char* DataFile, SInt8 WriteRead, SInt8 FlushAfterWrite, SInt8 MeasTime );

    SInt32                 PubFSetFileName  ( char* DataFile );
    SInt32                 PubFSetFlushMode ( SInt8 FlushAfterWrite );

    SInt32                 PubFGetFileSz  ();
    SInt32                 PubFGetEvNb  ();

    SInt32                 PubFCreate ();
    SInt32                 PubFOpen ();

    SInt32                 PubFWriteEv      ( ULT1__TMatDiscriW32* PtSrcMat  );
    SInt32                 PubFReadNextEv   ( ULT1__TMatDiscriW32* PtDestMat, SInt32 MaxDestSz );
    ULT1__TMatDiscriW32*   PubFReadNextEv   ();
    SInt32                 PubFGotoEv       ( SInt32 EvNo );
    SInt32                 PubFReadEv       ( SInt32 EvNo, ULT1__TMatDiscriW32* PtDestMat, SInt32 MaxDestSz );
    ULT1__TMatDiscriW32*   PubFReadEv       ( SInt32 EvNo );

    SInt32                 PubFFlush ();
    SInt32                 PubFClose ();

};

#endif
#endif

/* ==================================================== */
/* Discriminators test information file record          */
/*                                                      */
/* - Test no                                            */
/* - Chip no / comment                                  */
/* - Number of steps                                    */
/* - Events nb / step                                   */
/* - Run no associated to each step                     */
/* - Bias used for each step                            */
/* - User defined parameters for each step              */
/*   The can be used to convert udac / mv etc ...       */
/*                                                      */
/* ---------------------------------------------------- */
/* The file discri_test_NNNN.cnf contains the structure */
/* ULT1__TDisTestCnfFile                                */
/* ---------------------------------------------------- */
/* Date : 10/02/2009                                    */
/* ==================================================== */

// A copy of bias used for the current run
// 10/02/2009

typedef struct {

  UInt8 ABias[ULT1__REG_BIAS_NB];

} ULT1__TBiasCnf;


// All information about current discri measurement step
// 10/02/2009

typedef struct {

  SInt32           RunNo;
  ULT1__TBiasCnf   Bias;
  float            APar[ULT1__DIS_MEAS_STEP_MAX_PAR_NB];

} ULT1__TDisMeasStep;


// Format of file discri_test_NNNN.cnf
// 10/02/2009

/*typedef struct {

  SInt32 TestNo;                  // No of test
  char   ChipNoCmt[GLB_CMT_SZ];   // Comment about mimosa : name, number ...
  SInt32 StepNb;                  // Number of threshold steps used to perform this test = number of run
  SInt32 EvNbPerStep;             // Number of events per step = nb of events per run

  // Information about each step
  // - No of run
  // - Bias values used for this run
  // - An array of float user parameters, up to ULT1__DIS_MEAS_STEP_MAX_PAR_NB
  //   Parameters can be used to store threshold in mv or convertion ratio udac / mv etc ...

} ULT1__TDisMeasHeader;

typedef struct {
 ULT1__TDisMeasHeader Head;
 ULT1__TDisMeasStep AStep[ULT1__DIS_TEST_MAX_STEP_NB];
}ULT1__TDisTestCnfFile;*/


typedef struct {

  SInt32 TestNo;                  // No of test
  char   ChipNoCmt[GLB_CMT_SZ];   // Comment about mimosa : name, number ...
  SInt32 StepNb;                  // Number of threshold steps used to perform this test = number of run
  SInt32 EvNbPerStep;             // Number of events per step = nb of events per run

  // Information about each step
  // - No of run
  // - Bias values used for this run
  // - An array of float user parameters, up to ULT1__DIS_MEAS_STEP_MAX_PAR_NB
  //   Parameters can be used to store threshold in mv or convertion ratio udac / mv etc ...

  ULT1__TDisMeasStep AStep[ULT1__DIS_TEST_MAX_STEP_NB];


} ULT1__TDisTestCnfFile;



/* ==================================================== */
/* Discriminators data processing output file record    */
/* in sub matrices view mode => sub A, B, C, D          */
/*                                                      */
/* - Header                                             */
/* -- No of test                                        */
/* -- Step number = thresholds number                   */
/* -- Path of config file to get more informations      */
/*    Config file is discri_test_NNNN.cnf               */
/*                                                      */
/* - Data                                               */
/*   It's a 3D array which contains hit count in % for  */
/*   each pixel. It's ORGANIZED per submatrix.          */
/*   => Header.SubMatView = 1                           */
/* ---------------------------------------------------- */
/* The file discri_test_NNNN.dat contains the structure */
/* ULT1__TDisHitCntAllSubMatFile                        */
/* ---------------------------------------------------- */
/* WARNING : This structure AS IS allows only to access */
/* to file header and FIRST test data.                  */
/* In order to acces to next test data :                */
/* - init a BYTE pointer VPtB to FirstStepData addr     */
/* - increment with size of ULT1__TDisHitCntAllSubMatStepData */
/* - init a ULT1__TDisHitCntStepData pointer to VPtB    */
/* - and so on for next step ...                        */
/*                                                      */
/* ---------------------------------------------------- */
/* Date : 10/02/2009                                    */
/* ==================================================== */


typedef struct {

  SInt32 TestNo;
  SInt8  SubMatView; // 0 => whole matrix as single data piece
                     // 1 => submatrices A, B, C, D
  SInt32 StepNb;
  char   CnfFileName[GLB_FILE_PATH_SZ];


} ULT1__TDisHitCntHeader;


typedef struct {

  float AAASubMat[ULT1__SUB_MAT_NB][ULT1__MAT_DISCRI_LINES_NB][ULT1__REG_DISCRI_BIT_SZ / 4];

} ULT1__TDisHitCntAllSubMatStepData;

typedef struct {

  float AAAASubMat [ULT1__DIS_TEST_MAX_STEP_NB][ULT1__SUB_MAT_NB][ULT1__MAT_DISCRI_LINES_NB][ULT1__REG_DISCRI_BIT_SZ / 4];

} ULT1__TDisHitCntAllSubMatStepDataMG;

typedef struct {

  ULT1__TDisHitCntHeader            Header;
  ULT1__TDisHitCntAllSubMatStepData FirstStepData;

} ULT1__TDisHitCntAllSubMatFile;


/* ==================================================== */
/* Discriminators data processing output file record    */
/* in whole matrix view mode                            */
/*                                                      */
/* - Header                                             */
/* -- No of test                                        */
/* -- Step number = thresholds number                   */
/* -- Path of config file to get more informations      */
/*    Config file is discri_test_NNNN.cnf               */
/*                                                      */
/* - Data                                               */
/*   It's a 2D array which contains hit count in % for  */
/*   each pixel. It's ORGANIZED as ONE matrix.          */
/*   => Header.SubMatView = 0                           */
/* ---------------------------------------------------- */
/* The file discri_test_NNNN.dat contains the structure */
/* ULT1__TDisHitCntMatFile                              */
/* ---------------------------------------------------- */
/* WARNING : This structure AS IS allows only to access */
/* to file header and FIRST test data.                  */
/* In order to acces to next test data :                */
/* - init a BYTE pointer VPtB to FirstStepData addr     */
/* - increment with size of ULT1__TDisHitCntMatStepData */
/* - init a ULT1__TDisHitCntStepData pointer to VPtB    */
/* - and so on for next step ...                        */
/*                                                      */
/* ---------------------------------------------------- */
/* Date : 10/02/2009                                    */
/* ==================================================== */


typedef struct {

  float AAMat[ULT1__MAT_DISCRI_LINES_NB][ULT1__REG_DISCRI_BIT_SZ];

} ULT1__TDisHitCntMatStepData;


typedef struct {

  ULT1__TDisHitCntHeader      Header;
  ULT1__TDisHitCntMatStepData FirstStepData;

} ULT1__TDisHitCntMatFile;



typedef struct {

  SInt8 AALineCol[ULT1__MAT_DISCRI_LINES_NB][ULT1__REG_DISCRI_BIT_SZ / 4];

} ULT1__TQuaterMatDiscriBit;

typedef struct {

  ULT1__TColor AALineCol[ULT1__MAT_DISCRI_LINES_NB][ULT1__REG_DISCRI_BIT_SZ];

} ULT1__TMatDiscriColor;


typedef struct {
  
  ULT1__TColor AALineCol[ULT1__MAT_DISCRI_LINES_NB / 2][ULT1__REG_DISCRI_BIT_SZ / 2];
  
} ULT1__TMatDiscriColorHalfScale;


typedef struct {

  ULT1__TColor AALineCol[ULT1__MAT_DISCRI_LINES_NB][ULT1__REG_DISCRI_BIT_SZ / 4];

} ULT1__TQuaterMatDiscriColor;


// To store "1" count on each discri for N events

typedef struct {

  SInt32  ABit[ULT1__REG_DISCRI_BIT_SZ];

} ULT1__TRegDiscriCumul;


// To store % of "1" on each discri for N events

typedef struct {

  float  ABit[ULT1__REG_DISCRI_BIT_SZ];

} ULT1__TRegDiscriPerCent;

// To store "1" count on whole matrix for N events

typedef struct {

  UInt16 AALineCol[ULT1__MAT_DISCRI_LINES_NB][ULT1__REG_DISCRI_BIT_SZ];

} ULT1__TMatDiscriCumul;


typedef struct {
  
  UInt16 AALineCol[ULT1__MAT_DISCRI_LINES_NB / 2][ULT1__REG_DISCRI_BIT_SZ / 2];
  
} ULT1__TMatDiscriCumulHalfScale;


// To store % of "1" on whole matrix for N events

typedef struct {

  float AALineCol[ULT1__MAT_DISCRI_LINES_NB][ULT1__REG_DISCRI_BIT_SZ];

} ULT1__TMatDiscriPerCent;


typedef struct {

  float AALineCol[ULT1__MAT_DISCRI_LINES_NB][ULT1__REG_DISCRI_BIT_SZ];

} ULT1__TQuaterMatDiscriPerCent;



/* ============================================================================ */
/*                                                     */
/*                                                          */
/*                                                              */
/* ---------------------------------------------------------------------------- */
/*   */
/* ---------------------------------------------------------------------------- */
/* Date :                                                             */
/* ============================================================================ */


// DPXI__ULT1_NB_MAX_ULT1_PER_DAQ

typedef ULT1__TZsFFrame    ULT1__TAZsFFrame[ULT1__NB_MAX_ULT1_PER_DAQ];



typedef struct {
  
  // Date & Time
  
  UInt32 StartDate;            //    DD-MM-YY =           D23D16 - D15D08 - D07D00
  UInt32 StartTime;            // HH-MM-SS-CC =  D31D24 - D23D16 - D15D08 - D07D00
  
  // Asic conf
  
  UInt8  AsicName;             // Value of enumerated ASIC__TEAsicName defined in asic.def
  SInt8  AsicNb;               // Number of Asic read by DAQ
  
  // Trigger mode
  
  SInt8  SwTrigEnabled;        // Enable automatic start of acquisition on first trigger of a spill
                               // - 0 => Board starts upon acquisition request command, don't care about trigger
                               // - 1 => After acquisition request command, board wait on first trigger to start
                               // Trigger detection is done by sw, therefore it has a latency of N x 15 ms
                                
  SInt8  HwTrigModeSavedData;  // Trigger mode
                               // - 0 => Run contains all frames : with OR without trigger
                               // - 1 => Run contains ONLY frames with a trigger
                               // - More trigger modes may be defined later 
  
  SInt32 HwTrigPar[ULT1__TZSRunCnf__HW_TRIG_PAR_NB]; // Parameter about trigger handling -> enumerated DPXI__TEHwTrigPar in daq_pix.def
                                                     // - DPXI__HW_TRIG_PAR__OFFSET  -> Offset which must be added to trigger position to get real position
                                                     // - DPXI__HW_TRIG_PAR__WINDOW  -> Number of frame acquired after trigger
                                                     // - DPXI__HW_TRIG_PAR_NB       -> Max parameters number
                                                     
    
  // Run parameters
  
  SInt32 RunNo;                // Run number
  SInt8  RunSave;              // Save (1) or Not (0) data
  SInt8  RunSaveMode;          // Save mode -> Reserved for future use
  SInt32 RunEvNb;              // Event number in run
  SInt32 RunFileEvNb;          // Event number per file
  
} ULT1__TZSRunCnf;


typedef struct {
  
  // Date & Time
  
  UInt32 StopDate;             //    DD-MM-YY =           D23D16 - D15D08 - D07D00
  UInt32 StopTime;             // HH-MM-SS-CC =  D31D24 - D23D16 - D15D08 - D07D00
  
  // Status
  
  SInt32 EvNbTaken;            // Real number of events taken must be <= ULT1__TZSRunCnf.RunEvNb
  SInt32 RejAcqNb;             // Number of acquistion rejected in case of Mimosa 26 data error ( bad header, trailer etc ... )
  
  SInt32 ARejAcqList[ULT1__TZSRunRes__MAX_ACQ_REJ_NB]; // List of rejected acquisition
  
} ULT1__TZSRunRes;


// 09/07/2009

#ifndef APP__RMV_CLASSES

class ULT1__TCZsRunRW {
  
  private:

    SInt32 PrivFInitForNewRunLoading ();

    // Object
    
    FIL__TCBinFile* ProPtBinFile;
    
  protected:
  
    // Parameters
  
    char   ProParRunDir[GLB_FILE_PATH_SZ];
    SInt32 ProParRunNo;
    SInt32 ProParCurULT1No;
    SInt32 ProParCurEvNo;
    
    SInt8  ProParMeasTime;
    SInt8  ProParPrintMsg;
    SInt8  ProParPrintEvHeader;
    
    // Informations from run conf file OR calculated
    
    char   ProRunCnfFile[GLB_FILE_PATH_SZ];
    char   ProRunResFile[GLB_FILE_PATH_SZ];
    
    ULT1__TZSRunCnf ProRecZSRunCnf;
    ULT1__TZSRunRes ProRecZSRunRes;
    
    SInt32 ProInfRunULT1Nb;
    SInt32 ProInfRunEvNb;
    SInt32 ProInfRunFileSz;
    SInt32 ProInfRunEvNbPerFile;
    SInt32 ProInfRunBlocNbPerFile;
    SInt32 ProInfZsFFrameRawSz;
    
    // Intermediate variables for processing
    
    SInt8  ProConfDone;
    SInt8  ProParEnableErrLog;
    SInt8  ProParErrLogLvl;
    
    char   ProParErrLogFile[GLB_FILE_PATH_SZ];
    
  
    SInt8  ProLastEvAccessDoneInOneULT1Mode;
    SInt32 ProCurBlocNoInRun;
    SInt32 ProCurFileNo;
    SInt32 ProCurBlocNoInFile;
    char   ProCurFileName[GLB_FILE_PATH_SZ];
    
    ULT1__TZsFFrameRaw* ProPtFFrameRaw;


  public:
  
    ULT1__TCZsRunRW ();
    ~ULT1__TCZsRunRW ();
    
    SInt32              PubFBegin            ( char* ErrLogFile, SInt8 EnableErrLog, SInt8 ErrLogLvl );
    
    SInt32              PubFSetMeasTime      ( SInt8 Yes   );
    SInt32              PubFSetPrintMsg      ( SInt8 Print );
    SInt32              PubFSetPrintEvHeader ( SInt8 Print );
    
    SInt32              PubFGetMeasTime      ();
    SInt32              PubFGetPrintMsg      ();
    SInt32              PubFGetPrintEvHeader ();
    
    SInt32              PubFGetULT1Nb ();
    SInt32              PubFGetEvNb ();
    SInt32              PubFGetEvNbPerFile ();
    
    SInt32              PubFLoadRun    ( char* RunDir, UInt32 RunNo );    
    
    ULT1__TZsFFrameRaw* PubFGotoEvOneULT1 ( SInt8 ULT1No, SInt32 EvNo );
    ULT1__TZsFFrameRaw* PubFGotoEvAllULT1 (  SInt32 EvNo );
    
    SInt32              PubFCloseRun  ();
  
};

#endif

// 01/08/09

typedef struct {
  
  UInt32             IndexInEvWithHitList;
  ASIC__TFrameStatus FrStatus;
  
} ULT1__TCTelMon_TEltListEvWithHitAllPlanes;


// 02/08/09
  
typedef struct {
  
  SInt16 x;
  SInt16 y;  
  
} ULT1__TCTelMon_THit;

// 02/08/09

typedef struct {

  ULT1__TCTelMon_THit AAHits[MAPS__TCDigTelMon_MAX_PLANE_NB][MAPS__TCDigTelMon_MAX_RES_RUN_HIT_NB_PER_PLANE];
  SInt32              AHitsNbPerPlane[MAPS__TCDigTelMon_MAX_PLANE_NB];
  SInt32              EvNoInRun;
  
} ULT1__TCTelMon_TTrack;

// 02/08/09

typedef struct {
  
  SInt8  Full;
  SInt8  PlaneNb;
  SInt32 EvNb;
  ULT1__TCTelMon_TTrack ATracks[MAPS__TCDigTelMon_MAX_RES_RUN_EV_NB];
  
  
} ULT1__TCTelMon_TResRun;


// 17/07/09

#ifndef ROOT_ROOT

#ifndef APP__RMV_CLASSES

class ULT1__TCTelMon : public MAPS__TCDigTelMon {
  
  private:
    
    SInt8 PrivStopProc;
    
    // -------------------------------------------
    // Plot colors of planes for coincidence mode
    // -------------------------------------------
    
    ULT1__TColor                   PrivAPlanePlotColor[MAPS__TCDigTelMon_MAX_PLANE_NB];
    
    // ------------------------------------------------------
    // Variables to store frames : pixel, cumul, plot color
    // ------------------------------------------------------
    
    ULT1__TZsFFrameRaw*            PrivPtAcqData;             // Copy of Acq data, enabled if MakeLocalCpy parameter of PubFAddEvents (...) is set
                                                              // PubFAddEvents ( SInt8 MapsName, SInt8 MapsNb, SInt16 EvNb, ULT1__TZsFFrameRaw* PtSrc, SInt8 MakeLocalCpy, SInt8 OffLineCall );
    
    SInt32                         PrivAcqEvNb;               // Event nb of current Acq = parameter EvNb of last PubFAddEvents (...) call
                                                              // PubFAddEvents ( SInt8 MapsName, SInt8 MapsNb, SInt16 EvNb, ULT1__TZsFFrameRaw* PtSrc, SInt8 MakeLocalCpy, SInt8 OffLineCall );
                                                                                                                            
    
    ULT1__TZsFFrame                PrivZsFFrame;              // Tmp var to store ZsFFrameRaw converted to ZsFFrame
                                                              // Used for each frame converted
    
    ULT1__TMatDiscriBit            PrivMatDiscriCoin;         // Result for planeS coin plot AS PIXELS full scale matrix
    ULT1__TMatDiscriCumul          PrivMatDiscriCoinCum;      // Result for planeS coin cumul plot AS COUNT full scale matrix
    
    ULT1__TMatDiscriBitHalfScale   PrivMatDiscriBitHalfScale; // Result for planeS coin plot AS PIXELS 1/2 scale matrix
    ULT1__TMatDiscriCumulHalfScale PrivMatDiscriCumHalfScale; // Result for planeS coin cumul plot AS COUNT 1/2 scale matrix
    
    ULT1__TMatDiscriColor          PrivMatDispColor;          // Result in matrix color data for ALL KINDS of plots     
    ULT1__TMatDiscriColorHalfScale PrivMatDispColorHalfScale; // Result in 1/2 scale matrix color data for ALL KINDS of plots
    
    // =====================================================================
    // Lists to store informations results of events processing
    // =====================================================================
    
    // Flag to enable / disable list update
    // It's useful to disable list update for data re-processing ( off-line ) with information of list
    // because list informations should not been updates while using them for current data processing ...
    //
    // The following methods set this flag to one 
    // - PubFProcOnlyEvWithHitOnAllPlanes ( SInt8 Yes )
    
    SInt8 PriEnListEvWitHitUpdate;
    SInt8 PriEnListEvWithTrigUpdate;
    
    // ---------------------------
    // List of events with trigger
    // ---------------------------
    
    SInt32 PrivListEvWithTrigIndex;  // Current elt index
    SInt8  PrivListEvWithTrigFull;   // Full flag
    
    // The list array ( dynamic allocation )
    // Rq : AA and only one dimention because dynamic allocation of each elt of this array is done in PrivFInit ()
    
    ASIC__TFrameStatus* PrivAAListEvWithTrig[ULT1__TCTelMon__EV_LIST_MAX_CHAN_NB]; 
    
    // ---------------------------
    // List of events with hit(s)
    // ---------------------------
    
    SInt32 PrivListEvWithHitIndex;   // Current elt index
    SInt8  PrivListEvWithHitFull;    // Full flag

    // The list array ( dynamic allocation )
    // Rq : AA and only one dimention because dynamic allocation of each elt of this array is done in PrivFInit ()
    
    ASIC__TFrameStatus* PrivAAListEvWithHit[ULT1__TCTelMon__EV_LIST_MAX_CHAN_NB]; // List
    
    // -----------------------------------------
    // List of events with hit(s) on all planes
    // -----------------------------------------
    
    SInt32 PrivListEvWithHitAllPlanesIndex;       // Currenr elt index
    SInt8  PrivListEvWithHitAllPlanesFull;        // Full flag
    SInt8  PrivListEvWithHitAllPlanesCurProcMode; // Current processing mode
                                                  // Uses to know if data must be reprocessed in cas user
                                                  // has changed list mode via ProPar.ListHitAllPlanesMode

    // The list array ( dynamic allocation )
    
    ULT1__TCTelMon_TEltListEvWithHitAllPlanes* PrivAListEvWithHitAllPlanes;

    // ---------------------------------------------------------------------
    // Result run after data processing
    // ---------------------------------------------------------------------
    
    ULT1__TCTelMon_TResRun* PrivPtResRun;
    
    // ---------------------------------------------------------------------
    // General init function : reset all variables, called by constructor
    // ---------------------------------------------------------------------
    
    SInt32 PrivFInit ();

    // ---------------------------------------------------------------------
    // Processing function
    // ---------------------------------------------------------------------
    
    SInt32 PrivFConvZsFFrameToMatDiscriBitAndCumul ( SInt32 DbgEvNo, SInt8 Mode, SInt8 PlaneNo, SInt8 PlaneSelForCoin, SInt8 EvNo, SInt8 EvSelForPlot, ULT1__TZsFFrame* PtSrc, ULT1__TMatDiscriBit* PtDestFrameBit, ULT1__TMatDiscriBit* PtDestCoinBit,  ULT1__TMatDiscriCumul* PtDestFrameCum, ULT1__TMatDiscriCumul* PtDestCoinCum, SInt8 PrintLvl );
  

    // ---------------------------------------------------------------------
    // Lists ( events with trigger / hit ) add events functions
    // ---------------------------------------------------------------------
    
    SInt32 PrivFAddEvInListEvWithTrig ( SInt8 PlaneNo, ASIC__TFrameStatus* PtFrStatus, SInt32 HitCnt );
    SInt32 PrivFAddEvInListEvWithHit  ( SInt8 PlaneNo, ASIC__TFrameStatus* PtFrStatus, SInt32 HitCnt, SInt8 HitOnAllPlanes, SInt8 SingleHitOnEachPlane );
    
  protected:
  
    // -------------------------------------------
    // Data processing methods
    // -------------------------------------------
    
    // Process one frame = one event of plane specified by Id (0..5)
  
    SInt32 ProFProcessPlane   ( SInt32 DbdEvNo, SInt8 Id, ULT1__TZsFFrameRaw* PtSrc );
    
      
    
  public:

    // --------------------------------------------------------------------------
    // Flag to select one Acq off-line processing / full RUN off-line processing
    // --------------------------------------------------------------------------
    // Will be done via a method later
    // 06/08/2009
    // --------------------------------------------------------------------------
    
    SInt8 PubAcqOffLineProcOrRunOffLineProc;

    // -------------------------------------------
    // Constructor & Destructor
    // -------------------------------------------
    
    ULT1__TCTelMon ();
    ~ULT1__TCTelMon ();

    // ------------------------------------------------------------
    // Begin method => MUST be called before ANY other function !
    // ------------------------------------------------------------
    
    SInt32   PubFBegin ( char* ErrLogFile, SInt8 EnableErrLog, SInt8 ErrLogLvl, SInt32 MaxBuffEvNb );

    // -------------------------------------------
    // GUI handling methods
    // -------------------------------------------
    
    #ifndef ROOT_ROOT
    
      // GUI interface
      
      SInt32 PubFConnectGui ();
      
      SInt32 PubFGetGuiPar ();
    
    #endif
    
    // -------------------------------------------
    // Run control methods
    // -------------------------------------------
    // Allocate / free buffers
    // -------------------------------------------
    
    SInt32 PubFStartRun ( SInt8 MapsNb );
    SInt32 PubFStopRun ();
    
    // -------------------------------------------
    // Monitoring control methods
    // -------------------------------------------
    
    SInt32 PubFStartMon ();
    SInt32 PubFStopMon ();
    
    // -------------------------------------------
    // Data processing methods
    // -------------------------------------------
    
    // Add events in on-line monitoring mode and call data processing methods ( protected )
    
    SInt32 PubFAddEvents ( SInt8 MapsName, SInt8 MapsNb, SInt32 EvNb, ULT1__TZsFFrameRaw* PtSrc, SInt8 MakeLocalCpy, SInt8 OffLineCall );
        
    // Off-line processing of data
    // WARNING : NOW - 26/07/2009 - IT'S ONLY data of LAST acquisition ( data provided by last call to PubFAddEvents )
    // => Uses as events number the one of last acq
    // => Force mode to "Process one Acq"  AND update GUI parameters
  
    SInt32 PubFProcessOffLineCurAcq ();
    SInt32 PubFProcessOffLineCurAcq2 ();
  
    // Select frame to plot
    
    SInt32 PubFGotoFirstFrame ();
    SInt32 PubFGotoPrevFrame ();
    SInt32 PubFGotoNextFrame ();
    SInt32 PubFGotoLastFrame ();
    
    // -------------------------------------------
    // Results print methods
    // -------------------------------------------
    
    // Print list of events with a trigger detected => print SStatus record
    
    SInt32 PubPrintListEvWithTrig ( SInt8 Verbose );
    
    // Print list of events with at least one hit in one plane => print SStatus record 
    
    SInt32 PubPrintListEvWithHit  ( SInt8 Verbose );

    // Print list of events with hit on ALL planes => print SStatus record
    
    SInt32 PubPrintListEvWithHitOnAllPlanes  ( SInt8 Verbose );
    
    // Print res run
    
    SInt32 PubAllocResRun ();
    SInt32 PubFreeResRun ();
    SInt32 PubPrintResRun ();
    SInt32 PubSaveResRun  ( char* FileName );
    
    
    SInt32 PubFIsEvInListEvWithHitOnAllPlanes ( SInt32 EvNo );
    SInt32 PubFIsEvLastOfListEvWithHitOnAllPlanes ( SInt32 EvNo );

    SInt32 PubFProcOnlyEvWithHitOnAllPlanes ( SInt8 Yes );


    // -------------------------------------------
    // Results plot methods
    // -------------------------------------------

    // There is NO plot method in class.
    //
    // Because a call of a plot function ( from plot lib ) in DAQ supervisor thread doesn't work
    // - nothing is displayed !
    // - this may crash the application
    // I don't know why and have no time to study and understand this bug. I have found a workarround
    // by calling the plot function in an application timer.
    //
    // The class request plot by setting the flag ProRequestPlot to 1
    // plot is done by "plot timer" callback in application which
    // reset the flag ProRequestPlot after plot.
    
    // The application "plot timer" must know :
    // - when he must plot                       --> Test variable pointed by PubFGetPtPlotRq  ()
    // - the matrix to plot : full / half scale  --> Test variable pointed by PubFGetPtDispFullMatrix ()
    // - the data to plot                        --> Read via pointer given by PubFGetPtFullMatCol () / PubFGetPtHalfMatCol ()
    //
    // Polling of plot request and access to data is NOT DONE via method calls because
    // - calling class methods in timer while other methods may be called in DAQ supervisor thread MAY cause problems => No detailed understanding, no time ...
    // - it will save execution time by avoiding function call
       
    SInt8*                            PubFGetPtDispFullMatrix ();
    ULT1__TMatDiscriColor*            PubFGetPtFullMatCol ();
    ULT1__TMatDiscriColorHalfScale*   PubFGetPtHalfMatCol ();
    
    
};

#endif


#endif


#endif
