
/*******************************************************************************
File      : /dd/sdev_src/c/work/common/units/com_uc/
Goal      : Functions of com_uc unit.
Prj date  : 26/03/2003
File date : 26/03/2003
Doc date  : 26/03/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/*******************************************************************************/

#ifndef COM_UC_C
#define COM_UC_C

/*******************************************************************************
Prototype : SInt32 CUC_FBegin ( SInt8 ObjId )
Goal      : Initialize com_uc unit, must be called before any other unit function.
Inputs    : ObjId - object identifier.
Ouputs    : 0 if ok, negative value in case of error.
Globals   : 
Remark    : => doc must be updated 
Level     : This is a user level function.
Date      : 27/03/2003
Doc date  : 12/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 CUC_FBegin ( SInt8 ObjId, SInt32 ComMedia, SInt32 ComId, SInt32 SendBuffSz, PIP_TSendFunc PtSendFunc, SInt32 RecBuffSz, PIP_TGetFunc PtGetFunc ) {
  
  SInt32        VRet;
  CUC_TContext* VPt;
  
  VPt = &CUC_VGContext[ObjId];
  
  CUC_CHK_OBJ_ID (ObjId);
  
  VPt->ComMedia = ComMedia;
  VPt->ComId    = ComId;
  
  VRet = PIP_FBegin ( ObjId,
  ComId + 1000, SendBuffSz, PtSendFunc,
  ComId, RecBuffSz, PtGetFunc );
  
  err_retfail ( VRet, (ERR_OUT,"PIP_FBegin fail => return %d ", VRet) );
  
  err_retok (( ERR_OUT, "" ));
}


/*******************************************************************************
Prototype : SInt32 CUC_FEnd ( SInt8 ObjId )
Goal      : Close the com_uc unit ( free mem, ... ), must be called at end of program.
Inputs    : ObjId - object identifier 
Ouputs    : 0 if ok, negative value in cas eof error.
Globals   :
Remark    : 
Level     : This is a user level function.
Date      : 27/03/2003
Doc date  : 12/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 CUC_FEnd ( SInt8 ObjId ) {
  
  CUC_TContext* VPt;
  
  VPt = &CUC_VGContext[ObjId];
  
  err_retok (( ERR_OUT, "" ));  
}

/*******************************************************************************
Prototype : SInt32 CUC_FPutDatas ( SInt8 ObjId, SInt8 MultiExec,
          :                        UInt8 *PtSrc, SInt32 BlkSz, UInt32 TotSz )
Goal      : Send datas.
          :
Inputs    : ObjId     - object identifier
          : MultiExec - if 0 the function will send all datas and return
          :           - if 1, it will send one block and return, the you need
          :           - to call it until it returns 1
          : PtSrc     - pointer to source buffer
          : BlkSz     - size of block ( the datas will be splitted )
          : TotSz     - total size to send
          :
Ouputs    : 0 if block send ( MultiExec mode - should not happen otherwise )
          : 1 if end of transaction reached with success
          : negative value in case of error
Globals   :
Remark    :
Level     : This is a user level function.
Date      : 27/03/2003
Doc date  : 12/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 CUC_FPutDatas ( SInt8 ObjId, SInt8 MultiExec, UInt8 *PtSrc, SInt32 BlkSz, UInt32 TotSz ) {
  
  SInt32        VRet;
  CUC_TContext* VPt;
  
  CUC_CHK_OBJ_ID (ObjId);
  
  VPt = &CUC_VGContext[ObjId];
  
  VRet = PIP_FPutDatas ( ObjId, MultiExec, PIP_TRANS_MSG_TYPE_DATA, PtSrc, BlkSz, TotSz );
  
  return (VRet);  
}

/*******************************************************************************
Prototype : SInt32 CUC_FGetDatas ( SInt8 ObjId, SInt8 MultiExec,
          :                        UInt8 *PtDest, UInt32 MaxDestSz, UInt32* PtGetDataSz )
          :
Goal      : Get datas.
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
Date      : 27/03/2003
Doc date  : 12/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 CUC_FGetDatas ( SInt8 ObjId, SInt8 MultiExec, UInt8 *PtDest, UInt32 MaxDestSz, UInt32* PtGetDataSz ) {
  
  SInt32        VRet;
  CUC_TContext* VPt;
  
  CUC_CHK_OBJ_ID (ObjId);
  
  VPt = &CUC_VGContext[ObjId];
  
  VRet = PIP_FGetDatas ( ObjId, MultiExec, PtDest, MaxDestSz, PtGetDataSz );
  
  return (VRet);
}



/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :  2 => a data message is ready 
          :  1 => a command message is ready
          : -1 => an error occurs
          : -2 => there is nothing to read
Globals   :
Remark    :
Level     :
Date      : 13/04/2003
Doc date  : 13/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 CUC_FCmdInterpreter ( SInt8 ObjId, char* StrStatus, COM_TCommand** PtPtCmd, UInt8** PtPtData ) {
  
  COM_TCommand* VPtCommand;
  UInt32 VRecSz;
  SInt32 VStatus;
  SInt32 VMsgType;
  SInt32        VRet;
  CUC_TContext* VPt;
  
  CUC_CHK_OBJ_ID (ObjId);
  
  VPt = &CUC_VGContext[ObjId];
  
  VStatus = CUC_FGetDatas ( ObjId, 1 /* MultiExec */, (UInt8*) &CUC_VGRecBuffer[ObjId][0], CUC_REC_BUFF_SZ /* MaxDestSz */, &VRecSz );
    
  if ( VStatus == -1 ) {
    sprintf ( StrStatus, "Error" );
    err_error (( ERR_OUT, "CPC_FGetDatas fail !" ));
  }
  
  if ( VStatus == -2 ) {
    sprintf ( StrStatus, "Nothing to do" );
    return (0);
  }
  
  if ( VStatus == 0 ) {
    return (0);
  }
  
  VMsgType = PIP_FGetRecMsgType ( ObjId );

  if ( VMsgType == PIP_TRANS_MSG_TYPE_DATA ) {
    sprintf ( StrStatus, "Data message received" );
    
    if ( PtPtData != NULL ) {
      *PtPtData = (UInt8*) &CUC_VGRecBuffer[ObjId][0];
    }
    
    return (2);
  }
    
  if ( VMsgType == PIP_TRANS_MSG_TYPE_COMMAND ) {
    
    VPtCommand = (COM_TCommand*) &CUC_VGRecBuffer[ObjId][0];
    
    if ( PtPtCmd != NULL ) {
      *PtPtCmd = VPtCommand;
    }
    
    sprintf ( StrStatus, "Command received - %s", VPtCommand->StrCmd );
       
    return (1);
  }

  return (0);
}



/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :  
Globals   :
Remark    :
Level     :
Date      : 13/04/2003
Doc date  : 13/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

char* CUC_FGetDataPtr ( SInt8 ObjId ) {

  CUC_TContext* VPt;

  if ( ObjId >= CUC_OBJ_NB ) {
    err_error (( ERR_OUT, "ObjId=%d >= CUC_OBJ_NB=%d", ObjId, CUC_OBJ_NB ));
    return (NULL);
  }
  
  VPt = &CUC_VGContext[ObjId];
  
  return ( &CUC_VGRecBuffer[ObjId][0] );
  
}

/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 27/03/2003
Doc date  : 12/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 CUC_FLoadProm () {
  return (0);
}

/* !!! How to send back debug information from uC to PC ? */

#endif