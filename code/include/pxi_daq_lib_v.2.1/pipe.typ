/*******************************************************************************
File      : /dd/sdev_src/c/work/common/units/pipe/
Goal      : Types definition of pipe unit.
Prj date  : 26/03/2003
File date : 26/03/2003
Doc date  : 26/03/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/*******************************************************************************/


#ifndef PIP_TYP
#define PIP_TYP

/* Low level sender / receiver functions */

typedef SInt32 (*PIP_TSendFunc) ( SInt8 ObjId, SInt32 PipeId, SInt32 TransStatus, SInt8* PtExtParam, SInt32* PtSrc , SInt32 DataSz, SInt32 MaxDataSz );
typedef SInt32 (*PIP_TGetFunc ) ( SInt8 ObjId, SInt32 PipeId, SInt32 TransStatus, SInt8* PtExtParam, SInt32* PtDest, SInt32 DataSz, SInt32 MaxDataSz );

/* Object context fields */

typedef struct PIP_TContext {

  SInt32 *PtSendBuff;    /* Pointer to private send buffer */
  SInt32 SendPipeId;     /* Sender pipe identifier         */
  SInt32 SendStatus;     /* Send status flag               */
  SInt32 SendMsgType;    /* Type of message                */
  SInt32 SendBlkIndex;   /* Index of current sent buffer   */
  SInt32 SendBlkTotSz;   /* Bloc size => include header    */
  SInt32 SendBlkSz;      /* Block size => data only        */
  SInt32 SendTransSz;    /* Total transaction size ( without headers ) */
  SInt32 SendDataSzDone; /* Current data size sent         */
  
  PIP_TSendFunc PtSendFunc;
  
  SInt32 *PtRecBuff;     /* Pointer to private rec buffer    */
  SInt32 RecPipeId;      /* Receiver pipe identifier         */
  SInt32 RecStatus;      /* Receive status flag              */
  SInt32 RecMsgType;     /* Type of message                  */
  SInt32 RecBlkIndex;    /* Index of current received buffer */
  SInt32 RecBlkTotSz;    /* Bloc size => include header      */
  SInt32 RecBlkSz;       /* Block size => data only          */
  SInt32 RecTransSz;     /* Total transaction size ( without headers ) */
  SInt32 RecDataSzDone;  /* Current data size received       */
  
  PIP_TGetFunc PtGetFunc;
  
} PIP_TContext;


#endif