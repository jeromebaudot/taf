// @(#)maf/maf:$Name:  $:$Id event_header.typ  v.1 2005/10/02 18:03:46 sha Exp $
//  Author : Gilles CLAUS    23/11/02 
//  Modification : JB 2014/10/15 S/UINT32 -> S/UINT32 to allow compilation with other libraries declaring also S/UINT32

#ifndef EVENT_HEADER_TYP
#define EVENT_HEADER_TYP


///////////////////////////////////////////////////////////////////////////////////////////////
// event_header.typ :                                                                        //
// MIMOSA 9 : has a special feature. Matrices 64 x 64 and 32 x 32 recorded                   //
// in the same time.                                                                         //
// So while one frame is read on the first submatrix, 4 frames are read on the               //
// second submatrix.                                                                         //
// This has to be taken into account mainly in DPlane.cxx, mi9_mat_a3.c, event_header.typ .  //
// Put readout = 8 in the config file.                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////


/*******************************************************************************
File      : /dd/sdev_src/c/work/common/units/das_fio/event_header.typ
Goal      : Event header definition of DAS datafile format.
Prj date  : 2000 - 2002
File date : 
Doc date  : 23/11/2002
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI 
*******************************************************************************/


typedef int            SInt32img;
typedef unsigned int   UInt32img;
typedef short          SInt16;
typedef unsigned short UInt16;

#define NB_MAX_VFAS 5
#define EVENT_HEADER_RESERVED_ESZ 10


/**************************/
/* Event header structure */
/**************************/


/* One run is split in N file of each FileEvNb events ( or less for last file )          */
/* for eg RUN 100 => RUN_100_0.rz, RUN_100_1.rz, RUN_100_2.rz ... RUN_rrr_fff.rz         */
/* RUN_rrr_fff.rz => rrr is the run number and fff is the file number                    */

/* Some fields seems to be useless                                                       */
/* I wrote my own I/O routines, then i need some fields for this                          */

typedef struct {
  
  
  UInt32img  EvTrig;   /* Event trigger : Increment from 0 at each trigger                      */
  UInt32img  EvNo  ;   /* Event number  : Increment from 0 at each event taken                  */
  UInt32img  EvPos ;   /* Event position in file : Increment from 0 after each event write      */
  
  /* Why EvNo & EvPos ? */
  
  /* There are two types of datas files with the same header format                           */
  /* - Files with all events taken => for off-line analysis                                   */
  /* - Files with on-line monitoring events => a percentage of all events                     */
  /* In data file for off-line analysis EvNo = EvPos                                          */
  /* In data file from on-line monitoring EvNo != EvPos                                       */
  /* For eg if we take 50 % the the sequence should be as follows                             */
  /* EvNo  = 0, 2, 4, 6, 8, 10                                                                */
  /* EvPos = 0, 1, 2, 3, 4,  5                                                                */
  
  
  SInt32img  EvTag;          /* Event tag : for future use - should be 0                         */
  SInt32img  EvDate;         /* Event date = 0                                                   */
  SInt32img  EvTime;         /* Event time = SSSS ssss ( S = second s = mS - 10 ms resolution )  */
  
  /* I am afraid TrigCnt isn't ok now  */
  UInt32img  TrigCnt;        /* Total triggers received by acquisition system : accepted or not  */
  
  UInt32img  EvVmeTime;      /* Time to read VME boards in ms [10 ms res]                        */
                          /* Set to 0 at first event, then increase                           */
                          /* Vme time Ev 0 = EvVmeTime[1] - EvVmeTime[0]                      */
                          /* Vme time Ev 1 = EvVmeTime[2] - EvVmeTime[1]                      */
                          
  SInt32img  VFasCnt[NB_MAX_VFAS];      /* Hardware Adc strip counter                            */
  SInt32img  VFasReg[NB_MAX_VFAS];
  
  /* New fields => must decrement Reserved size when you add field ! */
  
  SInt32img  EvNetTime;      /* Time to send one event by ethernet in ms [10 ms res]             */
                          /* Set to 0 at first event, then increase                           */
                          /* Net time Ev 0 = EvNetTime[1] - EvNetTime[0]                      */
                          /* Net time Ev 1 = EvNetTime[2] - EvNetTime[1]                      */
  
  SInt16  MeasNo;         /* Index of current measure                                         */
  SInt16  EvInMeasNo;     /* Index of event in current measure                                */

  SInt32img   Reserved[EVENT_HEADER_RESERVED_ESZ-2];

  
} TEventHeader;

typedef TEventHeader*  TPEventHeader;

#define EVENT_HEADER_SZ (sizeof (TEventHeader))


#endif

