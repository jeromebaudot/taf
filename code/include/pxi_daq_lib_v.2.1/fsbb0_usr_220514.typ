
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

/* ============================= */
/*  Lib context                  */
/*  Contain all global variables */
/* ----------------------------- */
/* Date : 24/11/2008             */
/* ============================= */

typedef struct {

  SInt8 FileErrLogLvl;
  char  FileErrFile[GLB_FILE_PATH_SZ];

} FSBB0__TContext;


/* ======================================================= */
/* Frame provided by FSBB0 DAQ, it's independent of output  */
/* mode BUT data are not organized as in FSBB0__TZsFFrame   */
/* The format is :                                         */
/* - Header                                                */
/* - Frames counter                                        */
/* - Data length = W16 number of useful data ( excluding   */
/*   trailer and bits at zero at end of frame )            */
/* - Array of W16 data                                     */
/* ------------------------------------------------------- */
/* This is a FIXED size record which contains the maximum  */
/* possible W16 defined by FSBB0__ZS_FFRAME_RAW_MAX_W16     */
/* ------------------------------------------------------- */
/* Date : 08/12/2008                                       */
/* ======================================================= */


typedef struct {

  ASIC__TFrameStatus SStatus; // Informations about frame, see ASIC__TFrameStatus in asic.typ

  UInt32 Header;      // Header of Mimosa 26 frame
  UInt32 FrameCnt;    // Frame counter of Mimosa 26 frame
	UInt8  TrigLine;
	UInt8  TrigMode;  
  UInt32 DataLength;  // Useful length in W16 unit of data contains in ADataW16 array
                      // - B00B16 -> Length on first output
                      // - B17B23 -> Length on second output
                      //
                      // Add the two values to get the total length

  UInt32 Trailer;     // Trailer of Mimosa 26 frame
  
  UInt32 Zero;        // Zero at end of Mimosa 26 frame -> But NOW they are set to 0xFFFFFFFF by DAQ sw
  UInt32 Zero2;       // Zero at end of Mimosa 26 frame -> But NOW they are set to 0xFFFFFFFF by DAQ sw
                      //
                      // It's strange ... please don't ask why it's sugar and it's written salt on the box ...
                      //
                      // At the beginning it was last fields of Mimosa 26 frame, which are set to 0, now it's
                      // overwritten by sw in order to mark the end of information fields before data fields
                      // and 0xFFFFFFFF is a better value than zero for this purpose.


  UInt16 ADataW16[FSBB0__ZS_FFRAME_RAW_MAX_W16]; // MUST BE AT END OF RECORD !

} FSBB0__TZsFFrameRaw; // F in FFrameRaw means Fixed size frame

 

/* =================================================== */
/*  Field States/Line of Zero Sup frame, 2 views       */
/*  - W16 word                                         */
/*  - Fields                                           */
/* --------------------------------------------------- */
/* It's bit mapping => No conversion function required */
/* --------------------------------------------------- */
/* Date : 24/11/2008                                   */
/* =================================================== */

typedef union {

  UInt16 W16;

  struct {

    UInt16 StateNb  :  4;
    UInt16 LineAddr : 11;
    UInt16 Ovf      :  1;

  } F;

} FSBB0__TStatesLine;

/* =================================================== */
/*  Field State of Zero Sup frame, 2 views             */
/*  - W16 word                                         */
/*  - Fields                                           */
/* --------------------------------------------------- */
/* It's bit mapping => No conversion function required */
/* --------------------------------------------------- */
/* Date : 24/11/2008                                   */
/* =================================================== */

typedef union {

  UInt16 W16;

  struct {

    UInt16 HitNb   :  2;
    UInt16 ColAddr : 11;
    UInt16 NotUsed :  3;

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
/* ======================================================= */

typedef struct {

  FSBB0__TStatesLine StatesLine;
  FSBB0__TState      AStates[FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_STATES_REC];

} FSBB0__TZsFStatesRec; // F in FStatesRec means Fixed size record


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
/* ------------------------------------------------------- */

typedef struct {

  ASIC__TFrameStatus SStatus;

  UInt32 Header;
  UInt32 FrameCnt;
  UInt32 DataLength;
  SInt16 TrigSignalLine;
  SInt8  TrigSignalClk;
  SInt16 TrigLine;
  
  UInt32 StatesRecNb; // It's NOT a FSBB0 frame field, it's calculated by sw
                      // It's the number of valid record in AStatesRec

  FSBB0__TZsFStatesRec AStatesRec[FSBB0__ZS_FFRAME_MAX_STATES_REC];

  UInt32 Trailer;
  UInt32 Zero;
  UInt32 Zero2;

} FSBB0__TZsFFrame; // F in FFrame means Fixed size frame



#endif
