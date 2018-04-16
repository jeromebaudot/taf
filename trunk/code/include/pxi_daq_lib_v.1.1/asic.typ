
/*******************************************************************************
File      : x:\lib\com\asic\asic.typ
Goal      : Types definition of ASIC common constants / strcutures librairy
Prj date  : 29/06/2009
File date : 29/06/2009
Doc date  : //200
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
----------------------------------------------------------------------------------
License   : You are free to use this source files for your own development as long
          : as it stays in a public research context. You are not allowed to use it
          : for commercial purpose. You must put this header with laboratory and
          : authors names in all development based on this library.
----------------------------------------------------------------------------------
Labo      : IPHC */
/*******************************************************************************/


#ifndef ASIC_TYP
#define ASIC_TYP


/* ============== */
/*  */
/* ============== */


typedef struct {
  
  SInt8  AsicNo;        // Index of Asic <0..N-1> in case more than one is acquired
  SInt32 AcqNo;         // Index of current acquisition
  SInt32 FrameNoInAcq;  // Index of frame in acquisition <0..AcqFrameNb-1>
  SInt32 FrameNoInRun;  // Index of frame in run <0..TotEventNb-1>
  
  SInt32 HitCnt;        // Counter of hits in frame
                        // Used for monitoring, may be not set, therefore HitCnt = -1
    
  
  SInt32 ATrigRes[ASIC__ENUM_TRIG_RES_NB]; // Information about trigger, see ASIC__MI26_TETrigRes in asic.def
                                           // Parameters list index is
                                           // ASIC__MI26_TRIG_RES__SIG_LINE
                                           // ASIC__MI26_TRIG_RES__SIG_CLK
                                           // ASIC__MI26_TRIG_RES__LINE
                                           // ASIC__MI26_TRIG_TOT_NB
                                             
} ASIC__TFrameStatus;



#endif
