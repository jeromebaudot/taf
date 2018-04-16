
/*******************************************************************************
File      : /dd/sdev_src/c/work/common/units/pipe/
Goal      : Functions of pipe unit.
Prj date  : 26/03/2003
File date : 26/03/2003
Doc date  : 26/03/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/*******************************************************************************/

#ifndef PIP_C
#define PIP_C

/*******************************************************************************
Prototype : SInt32 PIP_FBegin ( SInt8 ObjId,
          : SInt32 SendPipeId, SInt32 SendBuffSz, PIP_TSendFunc PtSendFunc,
          : SInt32 RecPipeId , SInt32 RecBuffSz , PIP_TGetFunc  PtGetFunc )
          :
Goal      : Initialize the unit, must be called before using any unit function.
          :
Inputs    : ObjId         - the Object identifier ( the unit can handle many objects )
          : SendPipeId    - a number ( 1 to n ) which identify the data sender pipe
          : SendBuffSz    - the maximal block size for the sender pipe
          : PIP_TSendFunc - a pointer to the low level sender function
          : RecPipeId     - a number ( 1 to n ) which identify the data receiver pipe
          : RecBuffSz     - the maximal block size for the receiver pipe
          : PIP_TGetFunc  - a pointer to the low level receiver function
          : 
Ouputs    : O if ok, negative number if inialization fails ...
Globals   : 
Remark    :
Level     : This is a user level function
Date      : 26/03/2003
Doc date  : 12/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 PIP_FBegin ( SInt8 ObjId,
                    SInt32 SendPipeId, SInt32 SendBuffSz, PIP_TSendFunc PtSendFunc,
                    SInt32 RecPipeId , SInt32 RecBuffSz , PIP_TGetFunc  PtGetFunc ) {

  PIP_TContext* VPt;

  PIP_CHK_OBJ_ID (ObjId);
    
  VPt = &PIP_VGContext[ObjId];


  VPt->PtSendFunc    = NULL;
  VPt->PtSendBuff    = NULL;
  VPt->SendPipeId    = -1;
  VPt->SendStatus    = 0;
  VPt->SendBlkIndex  = 0;
  VPt->SendBlkTotSz  = 0;
  VPt->SendBlkSz     = 0;
  VPt->SendTransSz   = 0;
  VPt->SendDataSzDone = 0;
  
  
  VPt->PtGetFunc     = NULL;
  VPt->PtRecBuff     = NULL;
  VPt->RecPipeId     = -1;
  VPt->RecStatus     = 0;  
  VPt->RecBlkIndex   = 0;
  VPt->RecBlkTotSz   = 0;
  VPt->RecBlkSz      = 0;
  VPt->RecTransSz    = 0;
  VPt->RecDataSzDone = 0;
  
  
  if ( SendBuffSz > PIP_MAX_SEND_BUFF_SZ ) {
    err_retfail ( -1, (ERR_OUT,"SendBuffSz=%d > %d", SendBuffSz, PIP_MAX_SEND_BUFF_SZ ) );
  }
  
  VPt->SendBlkSz    = SendBuffSz;
  VPt->SendBlkTotSz = SendBuffSz + (PIP_DATA_HEAD_W32SZ * 4);

  if ( ( VPt->PtSendBuff = (SInt32*) malloc (VPt->SendBlkTotSz) ) == NULL ) {
    err_retfail ( -1, (ERR_OUT,"Malloc of SendBuff ( %d bytes ) fail !", VPt->SendBlkTotSz) );
  }
  
  VPt->SendPipeId = SendPipeId;
  
  if ( RecBuffSz > PIP_MAX_REC_BUFF_SZ ) {
    err_retfail ( -1, (ERR_OUT,"RecBuffSz=%d > %d", RecBuffSz, PIP_MAX_REC_BUFF_SZ ) );
  }
  
  VPt->RecBlkSz    = RecBuffSz;
  VPt->RecBlkTotSz = RecBuffSz + (PIP_DATA_HEAD_W32SZ * 4);
  
  if ( ( VPt->PtRecBuff = (SInt32*) malloc (VPt->RecBlkTotSz) ) == NULL ) {
    err_retfail ( -1, (ERR_OUT,"Malloc of RecBuff ( %d bytes ) fail !", VPt->RecBlkTotSz) );
  }
  
  VPt->RecPipeId = RecPipeId;

  VPt->PtSendFunc = PtSendFunc;
  VPt->PtGetFunc  = PtGetFunc;
    
  err_retok (( ERR_OUT, "" ));
}

/*******************************************************************************
Prototype : SInt32 PIP_FEnd ( SInt8 ObjId )
          : 
Goal      : Close unit ( free allocated memory, ... ) must be called at program end. 
Inputs    : The object identifier.
Ouputs    : 0 is ok, negative value in case of error.
Globals   : 
Remark    : Now, this function must be called for each object, next version
          : will scan itself for allocated objects and remove them.
Level     : This is a user level function
Date      : 26/03/2003
Doc date  : 12/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 PIP_FEnd ( SInt8 ObjId ) {
  
  PIP_TContext* VPt;

  PIP_CHK_OBJ_ID (ObjId);
    
  VPt = &PIP_VGContext[ObjId];
  
  if ( VPt->PtSendBuff != NULL ) {
    free ( VPt->PtSendBuff );
  }

  if ( VPt->PtRecBuff != NULL ) {
    free ( VPt->PtRecBuff );
  }

  return (0);
}

/*******************************************************************************
Prototype : char* PIP_FStrSendStatus ( SInt8 ObjId )
Goal      : Return a string with sender side status of object.
Inputs    : The object identifier.
Ouputs    : A pointer to the string ( stored in a function static variable ).
Globals   : 
Remark    : 
Level     : This is a user level function
Date      : 26/03/2003
Doc date  : 12/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

char* PIP_FStrSendStatus ( SInt8 ObjId ) {
  
  static char VStr[255];
  PIP_TContext* VPt;
  
  if ( ObjId >= PIP_OBJ_NB ) {
    err_error (( ERR_OUT, "ObjId=%d >= PIP_OBJ_NB=%d", ObjId, PIP_OBJ_NB ));
    return (NULL);
  }

  VPt = &PIP_VGContext[ObjId];
  
  sprintf ( VStr, "Obj=%d - Status=%d - Block alloc Sz=%d - Transaction Sz=%d - Block Sz=%d - Send sz done=%d - Block index=%d", ObjId, VPt->SendStatus, VPt->SendBlkTotSz, VPt->SendTransSz, VPt->SendBlkSz, VPt->SendDataSzDone, VPt->SendBlkIndex );
  
  return (VStr);
}

/*******************************************************************************
Prototype : char* PIP_FStrGetStatus ( SInt8 ObjId )
Goal      : Return a string with receiver side status of object.
Inputs    : The object identifier.
Ouputs    : A pointer to the string ( stored in a function static variable ).
Globals   :
Remark    :
Level     : This is a user level function
Date      : 26/03/2003
Doc date  : 12/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

char* PIP_FStrGetStatus ( SInt8 ObjId ) {
  
  static char VStr[255];
  PIP_TContext* VPt;
  
  if ( ObjId >= PIP_OBJ_NB ) {
    err_error (( ERR_OUT, "ObjId=%d >= PIP_OBJ_NB=%d", ObjId, PIP_OBJ_NB ));
    return (NULL);
  }
  
  VPt = &PIP_VGContext[ObjId];
  
  sprintf ( VStr, "Obj=%d - Status=%d - Transaction Sz=%d - Block Sz=%d - Received sz done=%d - Block index=%d", ObjId, VPt->RecStatus, VPt->RecTransSz, VPt->RecBlkSz, VPt->RecDataSzDone, VPt->RecBlkIndex );
  
  return (VStr);
}


/*******************************************************************************
Prototype : SInt32 PIP_FSendBlk ( SInt8 ObjId, UInt8 FirstBlk, SInt32 MsgType
          :                       UInt32 *PtSrc, UInt32 BlkSz, UInt32 TotSz ) 
          : 
Goal      : Send the block of BlkSz bytes pointed by PtSrc.
          :
Inputs    : ObjId    - object identifer
          : FirstBlk - 1 if it's the first block of a transaction, else 0.
          : MsgTYpe  - type of message
          :            PIP_TRANS_MSG_TYPE_COMMAND,
          :            PIP_TRANS_MSG_TYPE_DATA
          : PtSrc    - the block pointer
          : BlkSz    - the block size
          : TotSz    - the total transaction size
          :
Ouputs    : 0 if block send with success,
          : 1 if it's the transaction end ( last block to send ),
          : negative value in case of error.
Globals   :
Remark    : A copy of the block in object memory is done before starting to send,
          : therefore you can free the source block when the function returns,
          : the bloc is bufferised. 
Level     : This is a UNIT PRIVATE level function.
Date      : 26/03/2003
Doc date  : 12/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 PIP_FSendBlk ( SInt8 ObjId, UInt8 FirstBlk, SInt32 MsgType, /* UInt32 */ UInt8* PtSrc, UInt32 BlkSz, UInt32 TotSz ) {
  
  SInt32 VRet;
  PIP_TContext* VPt;

  PIP_CHK_OBJ_ID (ObjId);
  
  
  VPt = &PIP_VGContext[ObjId];

  if ( VPt->PtSendFunc == NULL ) {
    err_retfail ( -1, (ERR_OUT,"No SendFunction connected !") );
  }

  if ( BlkSz > VPt->SendBlkSz ) {
    err_retfail ( -1, (ERR_OUT,"BlkSz=%d > Buffer size=%d", BlkSz, VPt->SendBlkSz ) );
  }

  VPt->PtSendBuff[PIP_TRANS_MSG_TYPE_HD_INDEX] = MsgType;

  /* First block => write PIP_TRANS_TAG_BEGIN */

  if ( FirstBlk ) {
    VPt->SendTransSz    = TotSz;
    VPt->SendBlkIndex   = 0;
    VPt->SendDataSzDone = 0;
    VPt->SendStatus     = PIP_TRANS_STATUS_FIRST_BLK;
    VPt->PtSendBuff[PIP_TRANS_TAG_HD_INDEX]    = PIP_TRANS_TAG_BEGIN;
  }

  /* Next blocks => write block no */

  else {
    ++(VPt->SendBlkIndex);
    VPt->SendStatus = PIP_TRANS_STATUS_RUN;
    VPt->PtSendBuff[PIP_TRANS_TAG_HD_INDEX] = VPt->SendBlkIndex;   
  }

  VPt->PtSendBuff[PIP_TRANS_TOT_SZ_HD_INDEX] = TotSz;
  VPt->PtSendBuff[PIP_TRANS_BLK_SZ_HD_INDEX] = BlkSz;

  memcpy ( &(VPt->PtSendBuff[PIP_DATA_INDEX]), PtSrc, BlkSz );

  
  VRet = VPt->PtSendFunc ( ObjId, VPt->SendPipeId, VPt->SendStatus /* TransStatus */, NULL /* PtExtParam */, VPt->PtSendBuff, VPt->SendBlkTotSz, -1 /* MaxDataSz */ );

  
  err_retfail ( VRet, (ERR_OUT,"SendFunction fail => return %d", VRet) );

  VPt->SendDataSzDone += BlkSz;

  /* If end of transcation => return 1 */

  if ( VPt->SendDataSzDone >= VPt->SendTransSz ) {
    err_retval ( 1, ( ERR_OUT, "Transaction done %d data bytes send", VPt->SendTransSz ) );
  }

  /* Else, return 0 */

  err_retok (( ERR_OUT, "" ));
}

/*******************************************************************************
Prototype : SInt32 PIP_FPutDatas ( SInt8 ObjId, SInt8 MultiExec, SInt32 MsgType,
          :                        UInt8 *PtSrc, SInt32 BlkSz, UInt32 TotSz ) 
          :
Goal      : Send datas, this is the highest user level function in sender mode.
Inputs    : ObjId     - the object identifier 
          : MultiExec - if 0 the function will send all datas and return
          :           - if 1, it will send one block and return, then you need
          :           - to call it until it returns 1
          : MsgTYpe  - type of message
          :            PIP_TRANS_MSG_TYPE_COMMAND,
          :            PIP_TRANS_MSG_TYPE_DATA
          : PtSrc     - the source datas pointer
          : BlkSz     - the size in which the datas will be splitted 
          : TotSz     - the total datas size to send
          :
Ouputs    : 0 if block send ( MultiExec mode - should not happen otherwise )
          : 1 if end of transaction reached with success
          : negative value in case of error
Globals   : 
Remark    : BlkSz must be <= SendBuffSz configured in FBegin function, otherwise
          : the function will fail, ofcourse it can be less than SendBuffSz. 
          : If BlkSz < 0 => default value SendBlkSz configured by FBegin is used
Level     : This is a user level function.
Date      : 26/03/2003
Doc date  : 12/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 PIP_FPutDatas ( SInt8 ObjId, SInt8 MultiExec, SInt32 MsgType, UInt8 *PtSrc, SInt32 BlkSz, UInt32 TotSz ) {
  
  SInt32 VRet;
  static SInt32 VCallCnt = 0;
  static SInt8  VFirstBlk;
  static SInt32 Vi;
  static SInt32 VRemSz;
  static SInt32 VBlkSz;
  PIP_TContext* VPt;

  PIP_CHK_OBJ_ID (ObjId);
    
  VPt = &PIP_VGContext[ObjId];

  /* Overwrite BlkSz with default if < 0 - MUST BE DONE HERE & NOT LATER ! */

  if ( BlkSz < 0 ) {
    BlkSz = VPt->SendBlkSz;
  }

  VPt->SendMsgType = MsgType; /* Copy of MsgType only for debug ( not usefull ) */

  /* Initialization on first call */

  if ( VCallCnt == 0 ) {
    Vi        = 0;
    VFirstBlk = 1;
    VRemSz    = TotSz;    
    VBlkSz    = BlkSz;
  }

  do {
    
    if ( VRemSz < BlkSz ) {
      VBlkSz = VRemSz;
    }    

  
    VRet = PIP_FSendBlk ( ObjId, VFirstBlk /* FirstBlk */ , MsgType, &PtSrc[Vi*BlkSz], VBlkSz, TotSz );
    VFirstBlk = 0;
    Vi++;
  
    VRemSz -= VBlkSz;
        
  } while ( (VRet == 0) && (MultiExec == 0) );
  
  ++VCallCnt;
  
  if ( VRet == 1 ) {
    VCallCnt = 0;
    err_retval ( 1, ( ERR_OUT, "Transaction finished - %d bytes sent",  VPt->SendDataSzDone ) );
  }

  if ( VRet == 0 ) {
    err_retok (( ERR_OUT, "" ));
  }

  if ( VRet < 0 ) {
    VCallCnt = 0;
    err_retfail ( -1, (ERR_OUT,"Send datas fail") );
  }

  return (0);
}

/*******************************************************************************
Prototype : SInt32 PIP_FGetBlk ( SInt8 ObjId, UInt32 *PtDest, UInt32 MaxDestSz,
          :                      UInt32 *PtGetDataSz ) 
          : 
Goal      : Get one block and copy it in memory pointed by PtDest
          :
Inputs    : ObjId       - the object identifier 
          : PtDest      - the destination buffer pointer 
          : MaxDestSz   - the destination buffer size ( to avoid gardening ... )
          : PtGetDataSz - the variable pointed will be set with received block size
          : 
Ouputs    : The function returns 
          :  0 if one block received
          :  1 if transaction if finished ( last block received )
          : -1 => error
          : -2 => there is nothing to read ( no block available )
          :
          : PtGetDataSz point to received block size
Globals   :
Remark    : The last block of a transaction can be less than BlockSz, you can
          : read it real size with PtGetDataSz.
Level     : This is a UNIT PRIVATE level function.
Date      : 26/03/2003
Doc date  : 12/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 PIP_FGetBlk ( SInt8 ObjId, UInt32 *PtDest, UInt32 MaxDestSz, UInt32 *PtGetDataSz ) {

  SInt32 VRet;
  PIP_TContext* VPt;
  SInt32 VRecBlkSz;

  PIP_CHK_OBJ_ID (ObjId);
  
  VPt = &PIP_VGContext[ObjId];

  if ( VPt->PtGetFunc == NULL ) {
    err_retfail ( -1, (ERR_OUT,"No GetFunc connected") );
  }

  VRet = VPt->PtGetFunc ( ObjId, VPt->RecPipeId, -1 /* TransStatus */, NULL /* PtExtParam */, VPt->PtRecBuff, VPt->RecBlkTotSz /* DataSz */, VPt->RecBlkTotSz /* MaxDataSz */ );

  if ( VRet == -2 ) {
    return (-2);
    err_warning (( ERR_OUT, "GetFunc fail => because nothing to read" ));
  }

  err_retfail ( VRet, (ERR_OUT,"GetFunc fail => return %d", VRet) );

  if ( VPt->PtRecBuff[PIP_TRANS_TAG_HD_INDEX] == 0 ) {
    err_retfail ( -1, (ERR_OUT,"No data available") );
  }

  VPt->RecMsgType = VPt->PtRecBuff[PIP_TRANS_MSG_TYPE_HD_INDEX];

  /* If this is the first block of a transaction */

  if ( VPt->PtRecBuff[PIP_TRANS_TAG_HD_INDEX] == PIP_TRANS_TAG_BEGIN ) {
    VPt->RecTransSz    = VPt->PtRecBuff[PIP_TRANS_TOT_SZ_HD_INDEX];
    VRecBlkSz          = VPt->PtRecBuff[PIP_TRANS_BLK_SZ_HD_INDEX];
    VPt->RecBlkIndex   = 0;
    VPt->RecDataSzDone = 0;

    if ( VRecBlkSz > VPt->RecBlkSz ) {
      err_retfail ( -1, (ERR_OUT,"Sender send blocks too bigs=%d bytes > RecBlkSz=%d", VRecBlkSz, VPt->RecBlkSz ) );
      VPt->RecStatus = -1;
    }

    VPt->RecStatus = 1;

  }

  /* Else */

  else {
    VRecBlkSz       = VPt->PtRecBuff[PIP_TRANS_BLK_SZ_HD_INDEX];    
    ++(VPt->RecBlkIndex);
  }

  memcpy ( PtDest, &(VPt->PtRecBuff[PIP_DATA_INDEX]), VRecBlkSz );

  if ( PtGetDataSz != NULL ) {
    *PtGetDataSz = VRecBlkSz;
  }

  VPt->RecDataSzDone += VRecBlkSz;

  /* If end of transaction => return 1 */

  if ( VPt->RecDataSzDone >= VPt->RecTransSz ) {
    err_retval ( 1, ( ERR_OUT, "Transaction done %d bytes received", VPt->RecTransSz ) );
  }

  /* Else, return  */

  err_retok (( ERR_OUT, "" ));
}

/*******************************************************************************
Prototype : SInt32 PIP_FGetRecMsgType ( SInt32 ObjId )
          :
Goal      : Get message type ( command / data ). This function can be called after
          : PIP_FGetDatas BUT NEVER BEFORE !
          :
Inputs    : ObjId          - the object identifier
          :
Ouputs    : The message type
          : <0 in cas of error
          : PIP_TRANS_MSG_TYPE_COMMAND if this a command message
          : PIP_TRANS_MSG_TYPE_DATA    if this is a data message
          : 
Globals   :
Remark    :
Level     : This is a user level function.
Date      : 13/04/2003
Doc date  : 13/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 PIP_FGetRecMsgType ( SInt32 ObjId ) {
  
    PIP_TContext* VPt;
    
    PIP_CHK_OBJ_ID (ObjId);
    
    VPt = &PIP_VGContext[ObjId];
    
    return ( VPt->RecMsgType );
    
}

/*******************************************************************************
Prototype : SInt32 PIP_FGetDatas ( SInt8 ObjId, SInt8 MultiExec, UInt8 *PtDest,
          :                        UInt32 MaxDestSz, UInt32 *PtGetTotDataSz )
          :
Goal      : Get datas on receiver side, this the highest user level function.
          : This function should be called in a polling loop or by an interrupt
          : handler, the return value shows the transaction status.
          :
Inputs    : ObjId          - the object identifier
          : MultiExec      - if 0 the function will read all blocks and return
          :                - at end of transaction
          :                - if 1 the function will read one block and return,
          :                - you will need to call it until it return 1
          : PtDest         - the destination buffer pointer
          : MaxDestSz      - the destination buffer size ( to avoid gardening ... )
          : PtGetTotDataSz - the variable pointed will be set with transaction size
          :
Ouputs    : The function returns
          :  0 if one block received
          :  1 if transaction if finished ( last block received )
          : -1 => error
          : -2 => there is nothing to read ( no block available )
Globals   :
Remark    :
Level     : This is a user level function.
Date      : 26/03/2003
Doc date  : 12/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 PIP_FGetDatas ( SInt8 ObjId, SInt8 MultiExec, UInt8 *PtDest, UInt32 MaxDestSz, UInt32 *PtGetTotDataSz ) {

  SInt32 VRet;
  static SInt32 VCallCnt = 0;
  static SInt32 Vi;
  static SInt32 VBlkSz;
  UInt32 VRetBlkSz;
  PIP_TContext* VPt;
  
  PIP_CHK_OBJ_ID (ObjId);
  
  VPt = &PIP_VGContext[ObjId];
  

  /* Initialization on first call */
  
  if ( VCallCnt == 0 ) {
    Vi     = 0;
    VBlkSz = VPt->RecBlkSz;
  }
  
  do {    
    VRet = PIP_FGetBlk ( ObjId, (UInt32*) &PtDest[Vi*VBlkSz], MaxDestSz, &VRetBlkSz );
  } while ( (VRet == 0) && (MultiExec == 0) );
  
  ++VCallCnt;
  
  /* End of transaction, last block read */
  
  if ( VRet == 1 ) {
    VCallCnt = 0;
    
    /* Reset input buffer - should be optimized later => init PIP_TRANS_TAG_HD_INDEX only */
    memset ( VPt->PtRecBuff, 0, VPt->RecBlkTotSz );
    
    *PtGetTotDataSz = VPt->RecDataSzDone;
    err_retval ( 1, ( ERR_OUT, "End of transaction" ) );
  }
  
  /* Block read ok, but it's not the last one - MultiExec call */
  
  if ( VRet == 0 ) {
    Vi++;
    VBlkSz = VRetBlkSz;
    err_retok (( ERR_OUT, "" ));
  }
  
  /* Error ? */
  
  if ( VRet < 0 ) {
        
    /* It's not a "real error" => there is nothing to read */    
        
    if ( VRet == -2 ) {
      err_trace (( ERR_OUT, "PIP_FGetBlk fail => because nothing to read"   ));
      return (-2);
    }
    
    /* If it's a real error => reset calls counter */
    
    VCallCnt = 0;      
    
    err_retfail ( -1, (ERR_OUT,"Rec datas fail") );
  }

  return (0);
}

/*******************************************************************************
Prototype : SInt32 PIP_FTestSend ( SInt32 PipeId, SInt32 TransStatus, SInt8* PtExtParam,
          :                        SInt32* PtSrc, SInt32 DataSz, SInt32 MaxDataSz ) 
          : 
Goal      : This is the low level send function, which send one block.
          : This version use a local memory block to transfer datas ( for demo ).
          :
Inputs    : PipeId      - the sender pipe identifier
          : TransStatus - the SendStatus field of object ( transaction status )
          : PtExtParam  - a pointer, in case more parameters are needed
          : PtSrc       - pointer to source datas to send 
          : DataSz      - size of datas to send ( = block size )
          : MaxDataSz   - transaction total size ( not used now => set to -1 )
          : 
Ouputs    : 0 if ok
          : negative value in case of error
Globals   :
Remark    : This function is sender side interface from pipe unit to the hardware.
          : It is written by " low level programmer " and called by pipe unit when
          : it wants to send a block.
          : I provide many parameters from the " high level object ", perhaps some
          : are useless. The required parameters are PtSrc, DataSz and TransStatus.
          : TransStatus is used to indicate the beginning / end of transaction, it
          : can take the following values : PIP_TRANS_STATUS_FIRST_BLK,
          : PIP_TRANS_STATUS_RUN, PIP_TRANS_STATUS_LAST_BLK
          :
          :
Level     : This is a UNIT PRIVATE level function.
Date      : 26/03/2003
Doc date  : 12/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 PIP_FTestSend ( SInt32 PipeId, SInt32 TransStatus, SInt8* PtExtParam, SInt32* PtSrc , SInt32 DataSz, SInt32 MaxDataSz ) {
  
  memcpy ( PIP_VGDummyPc2UcBuffer, PtSrc, DataSz );
  
  err_retok (( ERR_OUT, "" ));   
}


/*******************************************************************************
Prototype : SInt32 PIP_FTestGet ( SInt32 PipeId, SInt32 TransStatus, SInt8* PtExtParam,
          :                       SInt32* PtDest, SInt32 DataSz, SInt32 MaxDataSz )
          :
Goal      : This is the low level receiver function, which retrieve one block.
          : This version use a local memory block to transfer datas ( for demo ).
          :
Inputs    : PipeId      - the sender pipe identifier
          : TransStatus - transaction status, not used now ( set to -1 )
          : PtExtParam  - a pointer, in case more parameters are needed
          : PtDest      - pointer to destination buffer
          : DataSz      - size of datas to send ( = block size )
          : MaxDataSz   - transaction total size ( SET TO block size NOW )
          :
Ouputs    : 0 if ok
          : negative value in case of error
Globals   :
Remark    : This function is receiver side interface from pipe unit to the hardware.
          : It is written by " low level programmer " and called by pipe unit when
          : it wants to retrieve a block.
          : I provide many parameters from the " high level object ", perhaps some
          : are useless. The required parameters are PtSrc, DataSz and TransStatus.
          :
          : I must point out that DataSz parameter is the total block size ( datas +
          : header ) then it's greater than the configured block size on sender side.
          :
Level     : This is a UNIT PRIVATE level function.
Date      : 26/03/2003
Doc date  : 12/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 PIP_FTestGet ( SInt32 PipeId, SInt32 TransStatus, SInt8* PtExtParam, SInt32* PtDest, SInt32 DataSz, SInt32 MaxDataSz ) {
  
  memcpy ( PtDest, PIP_VGDummyPc2UcBuffer, MaxDataSz );
  
  err_retok (( ERR_OUT, "" ));
}


/* ------------------------------------------------------------------------------------- */
/* ---- FSend & FGet examples with a directory and files used a data transfert media --- */
/* ------------------------------------------------------------------------------------- */

/*

How does it works ?

This is a SIMULATION of a transfer media ( USB, RPC ) using the file system
to build the data pipe.

Each block to send is written in a file with a name pipe_px_bx where
px is the pipe number ( 1, 2, ... ) and bx the block number ( 0, 1, ... ).
When a new block is ready and written a file pipe_px_w is created, when
the receiver see this file he knows there is a block to read.
Then the receiver delete the file pipe_px_w, therefore the sender knows
he can write the next block.

The directory where the files are written is set by PIP_FILE_TRF_DIR in pipe.def.

*/


/*******************************************************************************
Prototype : SInt32 PIP_FFileExists ( char* FilePath )
Goal      : Test if the file exists.
Inputs    : The file name ( full path if needed ).
Ouputs    : 1 if the file exists, else 0.
Globals   :
Remark    : The result is not guaranted in case of multi application lock to file.
Level     : This is a UNIT PRIVATE level function.
Date      : 06/04/2003
Rev       : 09/01/2008
          : - Bug on ROOT environnement ( stacked call => crash ) if fclose is called
          : when fopen fails. Now I call it only when fopen success.
          :
Doc date  : 12/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 PIP_FFileExists ( char* FilePath ) {
  
  SInt32 VRet;
  FILE*  VPf;

  VPf = fopen ( FilePath, "r" );
  
  if ( VPf == NULL ) {
    VRet = 0;
  }

  else {
    VRet = 1;
    fclose ( VPf );
  }

  return (VRet);
}



SInt32 PIP_FFileSize ( char* FilePath ) {
  
  FILE* VPf;
  SInt32 VFileSz;
  
  if ( ( VPf = fopen ( FilePath, "rb" ) ) == NULL ) {
    return (-1);
  }
  
  /* Calculate file size */
  
  if ( fseek ( VPf, 0, SEEK_SET ) != 0 ) {
    return (-1);
  }
  
  if ( fseek ( VPf, 0, SEEK_END ) != 0 ) {
    return (-1);
  }
  
  if ( (VFileSz = ftell ( VPf )) == -1 )  {
    return (-1);
  }
  
  if ( fseek ( VPf, 0, SEEK_SET ) != 0 ) {
    return (-1);
  }
  
  if ( fclose (VPf) != 0 ) {
    return (-1);
  }
  
  return (VFileSz);
}

/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    : 
Date      : 06/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 PIP_FDelFile ( char* FilePath ) {
  
  SInt32 VRet;

  VRet = remove ( FilePath );

  if ( VRet < 0 ) {
    err_warning (( ERR_OUT, "remove file %s fail - %s", FilePath, strerror(errno)  ));
    return (-1);
  }
  
  err_retok (( ERR_OUT, "" ));
}

/*******************************************************************************
Prototype :
Goal      : Low level FSend implementation demo.
Inputs    :
Ouputs    :
Globals   :
Remark    :
Date      : 29/03/2003
Rev       : 16/04/2006 - BUG correction
          : - Add a loop on signal file test and creation, because sometimes the
          : file creation failed although there is no previous file on disk !
          : The variable VMaxCrSigFileTry sets the maximum number of tentatives.
          :
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 PIP_FFileSend ( SInt8 ObjId, SInt32 PipeId, SInt32 TransStatus, SInt8* PtExtParam, SInt32* PtSrc , SInt32 DataSz, SInt32 MaxDataSz ) {
  
  SInt32 VRet;
  static SInt32 VABlkNo[PIP_OBJ_NB];
  SInt32 VSigVal;
  FILE*  VPf;
  char   VFilePath[GLB_FILE_PATH_SZ];
  
  SInt32 VCrSigFileErrCnt;
  SInt32 VMaxCrSigFileTry   = 100;
  SInt32 VWaitSigFileErrCnt;
  SInt32 VMaxWaitSigFileTry = 1000;
  

  err_trace (( ERR_OUT, "5" ));
  
  /* Create data block file */

  if ( TransStatus == PIP_TRANS_STATUS_FIRST_BLK ) {
    VABlkNo[ObjId] = 0;
  }
  
  sprintf ( VFilePath, "%s\\pipe_%d_%d", PIP_FILE_TRF_DIR, PipeId, VABlkNo[ObjId] );
  
  VPf = fopen ( VFilePath, "wb" );
  
  if ( VPf == NULL ) {
    err_retfail ( -1, (ERR_OUT,"fopen file %s fail ", VFilePath) );
  }
  
  if ( fwrite ( PtSrc, DataSz, 1 /* cnt */, VPf ) != 1 ) {
    err_retfail ( -1, (ERR_OUT,"fwrite in file %d fail ", VFilePath) );
  }
  
  fflush ( VPf );
  fclose ( VPf );
  
  if ( TransStatus == PIP_TRANS_STATUS_LAST_BLK ) {
    VABlkNo[ObjId] = 0;
  }

  
  /* Wait */

  sprintf ( VFilePath, "%s\\pipe_%d_w", PIP_FILE_TRF_DIR, PipeId );
  
  // 22/01/08 => weak point => lock possible
  
  VWaitSigFileErrCnt = 0;
  
  while ( (PIP_FFileExists (VFilePath) == 1) && (VWaitSigFileErrCnt < VMaxWaitSigFileTry)  ) {

    #ifdef ROOT_ROOT
    		gSystem->Sleep (10);
    #else
    	 Sleep (10);
    #endif

    ++VWaitSigFileErrCnt;
	}


  if ( VWaitSigFileErrCnt >= VMaxWaitSigFileTry ) {
    err_retfail ( -1, (ERR_OUT,"Abort => Locked on sigfile present ! %d tests", VWaitSigFileErrCnt ) );
  }


  err_trace (( ERR_OUT, "8" ));
  
  /* Create signal file */

  sprintf ( VFilePath, "%s\\pipe_%d_w", PIP_FILE_TRF_DIR, PipeId );
  
  err_trace (( ERR_OUT, "Create signal file begin" ));

  VCrSigFileErrCnt = 0;

  err_trace (( ERR_OUT, "9" ));
  
  while ( VCrSigFileErrCnt < VMaxCrSigFileTry ) {
      
    VPf = fopen ( VFilePath, "wb" );
  
    if ( VPf == NULL ) {
      ++VCrSigFileErrCnt;
      
      #ifdef ROOT_ROOT
      	 gSystem->Sleep (100);
      #else
        Sleep (100);
      #endif      
      
      continue;
    }
  
    VSigVal = VABlkNo[ObjId];
     
    if ( fwrite ( &VSigVal, 4 /* sz */, 1 /* cnt */, VPf ) != 1 ) {
      ++VCrSigFileErrCnt;
      fclose ( VPf );

      #ifdef ROOT_ROOT
      	 gSystem->Sleep (100);
      #else
      	Sleep (100);
      #endif      

      continue;
    }
  
    fflush  ( VPf );
    fclose ( VPf );
    
    break;
  } /* End while */

  err_trace (( ERR_OUT, "10" ));
  
  err_trace (( ERR_OUT, "Create signal file end" ));
  
  if ( VCrSigFileErrCnt > 0 ) {
    err_error (( ERR_OUT, "Create signal file errors !!! => Try %d times", VCrSigFileErrCnt ));
  }
  
  if ( VCrSigFileErrCnt >= VMaxCrSigFileTry ) {
    err_retfail ( -1, (ERR_OUT,"Create signal file failed after %d try : %s", VCrSigFileErrCnt,  strerror(errno) ) );
  }
  
  ++VABlkNo[ObjId];
  
  err_retok (( ERR_OUT, "" ));
}


/*******************************************************************************
Prototype :
Goal      : Low level FGet implementation demo.
Inputs    :
Ouputs    :
Globals   :
Remark    :
Date      : 29/03/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 PIP_FFileGet ( SInt8 ObjId, SInt32 PipeId, SInt32 TransStatus, SInt8* PtExtParam, SInt32* PtDest, SInt32 DataSz, SInt32 MaxDataSz ) {
  
  SInt32 VRet;
  static SInt32 VABlkNo[PIP_OBJ_NB];
  SInt32 VSigVal;
  FILE*  VPf;
  char   VFilePath[GLB_FILE_PATH_SZ];

  SInt32 VRdSigFileErrCnt;
  SInt32 VMaxRdSigFileTry = 100;
  
  SInt32 VLockCnt;
  
  
  /* Check destination buffer */
  
  if ( PtDest == NULL ) {
    err_retfail ( -1, (ERR_OUT,"Destination buffer pointer is NULL !") );
  }
  
  /* Is there something to read ? */

  /* Check signal file */
  
  sprintf ( VFilePath, "%s\\pipe_%d_w", PIP_FILE_TRF_DIR, PipeId );

  err_trace (( ERR_OUT, "Signal file =%s exists ?", VFilePath ));

  if ( PIP_FFileSize (VFilePath) != 4 ) {
    err_trace (( ERR_OUT, "Nothing to read"));
    return (-2);
  }


  VPf = fopen ( VFilePath, "rb" );
      
  if ( VPf == NULL ) {
    // fclose  ( VPf );  // 21/01/08 => Call to fclose () removed because it's not useful and MAY cause problems
    err_warning (( ERR_OUT, "Nothing to read"));
    return (-2);
  }
  
  VRdSigFileErrCnt = 0;
  
  while ( VRdSigFileErrCnt < VMaxRdSigFileTry ) {
    
    if ( fread ( &VSigVal, 4 /* sz */, 1 /* cnt */, VPf ) != 1 ) {
      fclose ( VPf );
      ++VRdSigFileErrCnt;
      
      #ifdef ROOT_ROOT
      	gSystem->Sleep (100);
      #else
      	Sleep (100);
      #endif
      
      continue;
      // err_retfail ( -1, (ERR_OUT,"Error reading signal file %s", VFilePath ) );
    }
  
    break;
  } /* End while */
  
  if ( VRdSigFileErrCnt > 0 ) {
    err_error (( ERR_OUT, "Read signal file errors !!! => Try %d times", VRdSigFileErrCnt ));
  }
  
  if ( VRdSigFileErrCnt >= VMaxRdSigFileTry ) {
    err_retfail ( -1, (ERR_OUT,"Read signal file failed after %d try : %s", VRdSigFileErrCnt,  strerror(errno) ) );
  }
  
  VABlkNo[ObjId] = VSigVal; /* Now signal file contains the block no */
  
  fflush ( VPf );
  fclose ( VPf );  

  err_trace (( ERR_OUT, "Signal file exists - now close it" ));
  
  /* Read data from block file */

  sprintf ( VFilePath, "%s\\pipe_%d_%d", PIP_FILE_TRF_DIR, PipeId, VABlkNo[ObjId] );
  
  VPf = fopen ( VFilePath, "rb" );
  
  if ( VPf == NULL ) {
    err_retfail ( -1, (ERR_OUT,"Open (for read) data block file %s fail !", VFilePath ) );
  }
  
  if ( fread ( PtDest, DataSz, 1 /* cnt */, VPf ) != 1 ) {
    err_retfail ( -1, (ERR_OUT,"Read of %d bytes in %s file fail !", DataSz, VFilePath ) );
  }

  fclose ( VPf );
  
  /* Delete the signal file */

  sprintf ( VFilePath, "%s\\pipe_%d_w", PIP_FILE_TRF_DIR, PipeId );

  err_trace (( ERR_OUT, "Delete signal file" ));
  
   
  // Code 10/01/2008 => VLockCnt

  VLockCnt = 0;
  
  while ( PIP_FDelFile (VFilePath) < 0 ) {
  
    if ( VLockCnt > 1000 ) {
      err_retfail ( -1, (ERR_OUT,"Lock on PIP_FDelFile (%s)", VFilePath ) );
    }

    ++VLockCnt;
  }

  /* Delete the data file */
  
  sprintf ( VFilePath, "%s\\pipe_%d_%d", PIP_FILE_TRF_DIR, PipeId, VABlkNo[ObjId] );

  // Code 10/01/2008 => VLockCnt
  
  VLockCnt = 0;
  
  while ( PIP_FDelFile (VFilePath) < 0 ) {
    
    if ( VLockCnt > 1000 ) {
      err_retfail ( -1, (ERR_OUT,"Lock on PIP_FDelFile (%s)", VFilePath ) );
    }
    
    ++VLockCnt;
    
  }
  
  err_retok (( ERR_OUT, "" ));
}


#endif