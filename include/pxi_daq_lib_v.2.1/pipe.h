
/*******************************************************************************
File      : /dd/sdev_src/c/work/common/units/pipe/
Goal      : Functions prototypes of pipe unit.
Prj date  : 26/03/2003
File date : 26/03/2003
Doc date  : 26/03/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/*******************************************************************************/


#ifndef PIP_H
#define PIP_H

SInt32 PIP_FBegin    ( SInt8 ObjId,
                       SInt32 SendPipeId, SInt32 SendBuffSz, PIP_TSendFunc PtSendFunc,
                       SInt32 RecPipeId , SInt32 RecBuffSz , PIP_TGetFunc  PtGetFunc );
  
SInt32 PIP_FEnd      ( SInt8 ObjId );

SInt32 PIP_FPutDatas ( SInt8 ObjId, SInt8 MultiExec, SInt32 MsgType, UInt8 *PtSrc, SInt32 BlkSz, UInt32 TotSz );
SInt32 PIP_FGetDatas ( SInt8 ObjId, SInt8 MultiExec, UInt8 *PtDest, UInt32 MaxDestSz, UInt32 *PtGetTotDataSz );
  
SInt32 PIP_FSendBlk ( SInt8 ObjId, UInt8 FirstBlk, SInt32 MsgType, UInt32 *PtSrc, UInt32 BlkSz, UInt32 TotSz );
SInt32 PIP_FGetBlk   ( SInt8 ObjId, UInt32 *PtDest, UInt32 MaxDestSz, UInt32 *PtGetDataSz );
    
char* PIP_FStrSendStatus ( SInt8 ObjId );
char* PIP_FStrGetStatus  ( SInt8 ObjId ); 



#endif