
/*******************************************************************************
File      : x:\lib\com\maps\fsbb0\fsbb0_usr.typ
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


#ifndef FSBB0_USR_TYP
#define FSBB0_USR_TYP

/* ================================= */
/*  Lib context                      */
/*  Contain all global variables     */
/* --------------------------------- */
/* Date : 24/11/2008 - GC            */
/* ================================= */

typedef struct {

  SInt8 FileErrLogLvl;
  char  FileErrFile[GLB_FILE_PATH_SZ];

} FSBB0__TContext;


/* ================================= */
/*  Union to extract                 */
/*  Header 0,1 from header 30 bits   */
/*  fro frame field                  */
/* --------------------------------- */
/* Date : 23/05/2014 - GC            */
/* ================================= */

typedef union {
  
  UInt32 W32;
  
  struct {
    UInt32 H0 :  15; // B00..B14
    UInt32 H1 :  15; // B15..B29
    UInt32 Nu :   2; // B30..B31
  } F;
  
} FSBB0__THeader;

/* ================================= */
/*  Union to extract                 */
/*  Trailer 0,1 from trailer 30 bits */
/*  fro frame field                  */
/* --------------------------------- */
/* Date : 23/05/2014 - GC            */
/* ================================= */

typedef union {
  
  UInt32 W32;
  
  struct {
    UInt32 T0 :  15; // B00..B14
    UInt32 T1 :  15; // B15..B29
    UInt32 Nu :   2; // B30..B31
  } F;
  
} FSBB0__TTrailer;


/* ================================= */
/*  Union to extract                 */
/*  Trigger fields from 30 bits word */
/*                                   */
/* --------------------------------- */
/* Date : 23/05/2014 - GC            */
/* ================================= */

typedef union {
  
  UInt32 W32;
  
  struct {
    UInt32 Mode   :  3; // B00..B02
    UInt32 NbLine :  8; // B03..B10
    UInt32 Nu     : 21; // B11..B31
  } F;
  
} FSBB0__TTriger;


/* ============================= */
/*  Union to extract             */
/*  Data length, rem, frames cnt */
/*  fro frame field              */
/* ----------------------------- */
/* Date : 22/05/2014             */
/* ============================= */

typedef union {
  
  UInt32 W32;
  
  struct {
    UInt32 DataLength :  8; // B00..B07
    UInt32 Rem        :  2; // B08..B09
    UInt32 FrameCnt   : 20; // B10..B29
  } F;
  
} FSBB0__TDataLengthRemFrCnt;

/* ======================================================= */
/* Frame provided by FSBB0 DAQ, it's independent of output */
/* mode BUT data are not organized as in FSBB0__TZsFFrame  */
/* The format is :                                         */
/* - Header                                                */
/* - Frames counter                                        */
/* - Data length = W16 number of useful data ( excluding   */
/*   trailer and bits at zero at end of frame )            */
/* - Array of W16 data                                     */
/* ------------------------------------------------------- */
/* This is a FIXED size record which contains the maximum  */
/* possible W16 defined by FSBB0__ZS_FFRAME_RAW_MAX_W16    */
/* ------------------------------------------------------- */
/* Date : 08/12/2008                                       */
/* Rev  : XX/05/2014 - MS : Mi28 -> FSBB0                  */
/* Rev  : 23/05/2014 - GC : Upd fields + THeader, TTRailer */
/* ======================================================= */


typedef struct {

  ASIC__TFrameStatus SStatus; // Informations about frame, see ASIC__TFrameStatus in asic.typ

  FSBB0__THeader Header;      // Header of FSBB0 frame
  FSBB0__TTriger Trigger;     // Trigger of FSBB0 frame - 22/05/14 GC
  
  FSBB0__TDataLengthRemFrCnt DataLengthRemFrCnt;  // 22/05/14 GC
                                                  // Field from FSBB0 frame which contains
                                                  // - DataLength  B00..B07
                                                  // - Rem         B08..B09
                                                  // - FrCnt       B10..B29

  // UInt32 FrameCnt; // 22/05/14 GC => Replaced by DataLengthRemFrCnt
                      // Frame counter of FSBB0 frame
  

UInt8  TrigLine;
  UInt8  TrigMode;
  
  // 22/05/14 GC => Replaced by CalcDataLength 
  //
  // UInt32 DataLength;  // Useful length in W32 unit of data contains in ADataW32 array
                      // Calculation to be defined
                      // - B00B16 -> Length on first output
                      // - B17B23 -> Length on second output
                      //
                      // Add the two values to get the total length
  UInt8  FsbbFieldDataLength;       // 28/08/14 - GC : New fields to handle FSBB0 datalength bugs
  UInt8  FsbbFieldRem;              // 28/08/14 - GC : New fields to handle FSBB0 datalength bugs
  UInt32 UsefulDataLengthW30;       // 28/08/14 - GC : New fields to handle FSBB0 datalength bugs
  UInt32 TotalDataLengthW30;        // 28/08/14 - GC : New fields to handle FSBB0 datalength bugs
  SInt8  BugFieldDataLengthEqual0;  // 28/08/14 - GC : New fields to handle FSBB0 datalength bugs
 
  UInt32 CalcDataLength;   // 22/05/14 GC - Replace fields "DataLength" but it has not the same meaning
                           // 
                           // - "DataLength" (Mi26, 28) contains the fields data length from Mi26/28 frame
                           // - "CalcDataLength" = W32 nb in ADataW32 array calculated from DataLengthRemFrCnt
	

  FSBB0__TTrailer Trailer;     // Trailer of FSBB0 frame

// 22/05/14 GC
// Set zeo fields in comments
//
//  UInt32 Zero;        // Zero at end of Mimosa 26 frame -> But NOW they are set to 0xFFFFFFFF by DAQ sw
//  UInt32 Zero2;       // Zero at end of Mimosa 26 frame -> But NOW they are set to 0xFFFFFFFF by DAQ sw
                      //
                      // It's strange ... please don't ask why it's sugar and it's written salt on the box ...
                      //
                      // At the beginning it was last fields of Mimosa 26 frame, which are set to 0, now it's
                      // overwritten by sw in order to mark the end of information fields before data fields
                      // and 0xFFFFFFFF is a better value than zero for this purpose.

// 22/05/14 GC
// U16 -> U32 because data are serialized on U30 words on FSBB
//
// UInt16 ADataW16[FSBB0__ZS_FFRAME_RAW_MAX_W16]; // MUST BE AT END OF RECORD !

  UInt32 ADataW32[FSBB0__ZS_FFRAME_RAW_MAX_W32]; // MUST BE AT END OF RECORD !


} FSBB0__TZsFFrameRaw; // F in FFrameRaw means Fixed size frame 

 

/* =================================================== */
/*  Field States/Line of Zero Sup frame, 2 views       */
/*  - W16 word                                         */
/*  - Fields                                           */
/* --------------------------------------------------- */
/* It's bit mapping => No conversion function required */
/* --------------------------------------------------- */
/* Date : 22/05/2014 - GC                              */
/* =================================================== */

typedef union {

  UInt32 W32;

  struct {

    UInt32 HitNbG0   :  8;
    UInt32 HitNbG1   :  8;
    UInt32 SLineAddr :  9;
    UInt32 NU        :  7;
    
  } F;

} FSBB0__TStatesLine;

/* =================================================== */
/*  Hit window of Zero Sup frame, 2 views              */
/*  - W32 word                                         */
/*  - Fields                                           */
/* --------------------------------------------------- */
/* It's bit mapping => No conversion function required */
/* --------------------------------------------------- */
/* Date : 22/05/2014 - GC                              */
/* =================================================== */

typedef union {

  UInt32 W32;

  struct {

    UInt32 Delta   : 2;
    UInt32 Code    : 20;
    UInt32 ColAddr : 8;
    UInt32 NU      : 2;

  } F;

} FSBB0__TState;


/* ======================================================= */
/* One list of states associated to one line               */
/* - States/Lines information                              */
/* - States list                                           */
/* ------------------------------------------------------- */
/* This is a FIXED size record which contains all states   */
/* of one line, max FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_LINE */
/* ------------------------------------------------------- */
/* Date : 24/11/2008                                       */
/* Rev  : XX/05/2014 - MS : Mi28 -> FSBB0                  */
/* Rev  : 05/06/2014 - GC : Update fields list to handle   */
/* easily groups G0, G1                                    */
/* ======================================================= */

typedef struct {
  
  // Before 05/06/2014 - GC
  //
  //  FSBB0__TStatesLine  StatesLine;
  //  FSBB0__TState       AStates[FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_STATES_REC];
  
  // Since 05/06/2014 - GC
  
  FSBB0__TStatesLine  StatesLine;
  FSBB0__TState       AStatesG0[FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP];
  FSBB0__TState       AStatesG1[FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP];
  
  SInt8               NbWinTot;
  SInt8               NbWinG0;
  SInt8               NbWinG1;
  
} FSBB0__TZsFStatesRec; // F in FStatesRec means Fixed size record


/* ======================================================= */
/* One list of states associated to one line               */
/* - States/Lines information                              */
/* - States list                                           */
/* ------------------------------------------------------- */
/* This is a FIXED size record which contains all states   */
/* of one line, max FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_LINE */
/* ------------------------------------------------------- */
/* Date : 24/11/2008                                       */
/* ======================================================= */

// typedef struct {
//
//   FSBB0__TStatesLine  StatesLine;
//   FSBB0__TState       AStates[FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_STATES_REC];
//
// } FSBB0__TZsFStatesRec; // F in FStatesRec means Fixed size record


/* ======================================================= */
/* Frame provided by FSBB0, this is the final result after  */
/* data processing depending of output mode selected       */
/* - Header                                                */
/* - Frames counter                                        */
/* - Data length = W16 number of useful data ( excluding   */
/*   trailer and bits at zero at enbd of frame )           */
/* ------------------------------------------------------- */
/* This is a FIXED size record which contains all states   */
/* of one line, max FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_LINE */
/* ------------------------------------------------------- */
/* Date : 24/11/2008                                       */
/* Rev  : XX/05/2014 - MS : Mi28 -> FSBB0                  */
/* Rev  : 23/05/2014 - GC : Upd fields + THeader, TTRailer */
/* ------------------------------------------------------- */

typedef struct {

  ASIC__TFrameStatus SStatus;

  FSBB0__THeader Header;
  UInt32 FrameCnt;
  FSBB0__TTriger Trigger;     // Trigger of FSBB0 frame - 22/05/14 GC
  
  UInt32 DataLength;
  SInt16 TrigSignalLine;
  SInt8  TrigSignalClk;
  SInt16 TrigLine;
  
  UInt32 StatesRecNb; // It's NOT a FSBB0 frame field, it's calculated by sw
                      // It's the number of valid record in AStatesRec

  FSBB0__TZsFStatesRec AStatesRec[FSBB0__ZS_FFRAME_MAX_STATES_REC];

  FSBB0__TTrailer Trailer;
  
  // 22/05/14 GC
  // Set zero fields in comments
  //
  // UInt32 Zero;
  // UInt32 Zero2;

} FSBB0__TZsFFrame; // F in FFrame means Fixed size frame



/* ======================================================= */
/* Structure containing the pattern to be set via the JTAG */
/* link to perform the automatic tests of the FSBB0        */
/* structure                                               */
/* ------------------------------------------------------- */
/* Date : 17/06/2014                                       */
/* ------------------------------------------------------- */

typedef struct {

  SInt16 ConfigIndex;
  SInt16 FileNumberMajor;
  SInt16 FileNumberMinor;
  UInt32 Pattern[8][13];
  SInt16 Zone11;
  SInt16 Zone10;
  SInt16 Zone01;
  SInt16 Zone00;
 

} FSBB0__TTestPattern; // F in FFrame means Fixed size frame





typedef struct {

  char   ConfigFile[FSBB0__TEST_FILES_NAME_MAX_SZ];
  char   ResultFileBit[FSBB0__TEST_FILES_NAME_MAX_SZ];
  char   ResultFileW32[FSBB0__TEST_FILES_NAME_MAX_SZ];
  SInt16 FileNumberMajor;
  SInt16 FileNumberMinor;
 

} FSBB0__TTestFiles; // F in FFrame means Fixed size frame



/* ======================================================= */
/* Structure containing the test parameters :              */
/* files name for all the tests to be done                 */
/* structure                                               */
/* ------------------------------------------------------- */
/* Date : 20/06/2014                                       */
/* ------------------------------------------------------- */

typedef struct {

  FSBB0__TTestFiles Files[FSBB0__TEST_MAX_NUMBER];

} FSBB0__TTestParams; // F in FFrame means Fixed size frame





#endif

