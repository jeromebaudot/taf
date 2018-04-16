
/*******************************************************************************
File      : /dd/sdev_src/c/work/common/units/com_pc/
Goal      : Functions of com_pc unit.
Prj date  : 26/03/2003
File date : 27/03/2003
Doc date  : 26/03/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/*******************************************************************************/

#ifndef COM_PC_C
#define COM_PC_C

/*******************************************************************************
Prototype : CPC_FBegin ( SInt8 ObjId, SInt32 ComMedia, SInt32 ComId,
          :              SInt32 SendBuffSz, PIP_TSendFunc PtSendFunc,
          :              SInt32 RecBuffSz, PIP_TGetFunc PtGetFunc ) 
          :
Goal      : Initialize com_pc unit, must be called at beginning before
          : any other unit function.
          :
Inputs    : ObjId      - object identifier
          : ComMedia   - the communication media ( USB, RPC etc )
          : ComdId     - the communication identifier ( 1, 2 ... )
          : SendBuffSz - the sender block size ( max value )
          : PtSendFunc - a pointer to low level sender function
          : RecBuffSz  - the receiver block size ( max value )
          : PtGetFunc  - a pointer to low level receiver function 
          :
Ouputs    : 0 if ok, negative value if initialization fail.
Globals   :
Remark    : 
Level     : This is a user level function.
Date      : 27/03/2003
Doc date  : 12/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 CPC_FBegin ( SInt8 ObjId, SInt32 ComMedia, SInt32 ComId, SInt32 SendBuffSz, PIP_TSendFunc PtSendFunc, SInt32 RecBuffSz, PIP_TGetFunc PtGetFunc ) {

  SInt32        VRet;
  CPC_TContext* VPt;
  
  VPt = &CPC_VGContext[ObjId];

  CPC_CHK_OBJ_ID (ObjId);

  VPt->ComMedia = ComMedia;
  VPt->ComId    = ComId;

  VRet = PIP_FBegin ( ObjId,
                      ComId, SendBuffSz, PtSendFunc,
                      ComId + 1000, RecBuffSz, PtGetFunc );
  
  err_retfail ( VRet, (ERR_OUT,"PIP_FBegin fail => return %d ", VRet) );
  
  err_retok (( ERR_OUT, "" ));
}

/*******************************************************************************
Prototype : SInt32 CPC_FEnd ( SInt8 ObjId )
Goal      : Close the com_pc unit ( free memory ), must be called at end of program.
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     : This is a user level function.
Date      : 27/03/2003
Doc date  : 12/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 CPC_FEnd ( SInt8 ObjId ) {
  
  CPC_TContext* VPt;

  CPC_CHK_OBJ_ID (ObjId);
  
  VPt = &CPC_VGContext[ObjId];

  
  err_retok (( ERR_OUT, "" ));  
}

/*******************************************************************************
Prototype : SInt32 CPC_FPutDatas ( SInt8 ObjId, SInt8 MultiExec,
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

SInt32 CPC_FPutDatas ( SInt8 ObjId, SInt8 MultiExec, UInt8 *PtSrc, SInt32 BlkSz, UInt32 TotSz ) {
  
  SInt32        VRet;
  CPC_TContext* VPt;

  CPC_CHK_OBJ_ID (ObjId);
  
  VPt = &CPC_VGContext[ObjId];
  
  VRet = PIP_FPutDatas ( ObjId, MultiExec, PIP_TRANS_MSG_TYPE_DATA, PtSrc, BlkSz, TotSz );

  return (VRet);
}

/*******************************************************************************
Prototype : SInt32 CPC_FGetDatas ( SInt8 ObjId, SInt8 MultiExec,
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

SInt32 CPC_FGetDatas ( SInt8 ObjId, SInt8 MultiExec, UInt8 *PtDest, UInt32 MaxDestSz, UInt32* PtGetDataSz ) {
  
  SInt32        VRet;
  CPC_TContext* VPt;

  CPC_CHK_OBJ_ID (ObjId);
    
  VPt = &CPC_VGContext[ObjId];
  
  VRet = PIP_FGetDatas ( ObjId, MultiExec, PtDest, MaxDestSz, PtGetDataSz );
  
  return (VRet);
}

/*******************************************************************************
Prototype : SInt32 CPC_PRIV_FGetNewCmd ( SInt8 ObjId,
          :                               UInt32 ParamSz, COM_TCommand** PtCommand )
          : 
Goal      : Return a free command structure to be filled by a send command function.
          :
Inputs    : ObjId      - object identifier 
          : ParamSz    - size of command parameters
          : PtCommand  - pointer to pointer to command parameters structure
          : 
Ouputs    : The command index if success ( positive value )
          : a negative value in case of error
Globals   :
Remark    : NOW RETURN ALWAYS 0 ( index ) => single command mode 
Level     : This is a UNIT PRIVATE level function.
Date      : 27/03/2003
Doc date  : 12/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 CPC_PRIV_FGetNewCmd ( SInt8 ObjId, UInt32 ParamSz, COM_TCommand** PtCommand ) {

  SInt32        VRet;
  CPC_TContext* VPt;
  COM_TCommand* VPtCommand;
  UInt32        VTotCmdSz;

  CPC_CHK_OBJ_ID (ObjId);
  
  VPt = &CPC_VGContext[ObjId];
  
  /* Find free cell in CPC_VGCommandsInf */
  /* Now always return first cell        */

  VTotCmdSz = sizeof (COM_TCommand) + ParamSz; /* Record will exced required size from 4 bytes => because of field ParamBuff */
  
  VPtCommand = (COM_TCommand*) malloc ( VTotCmdSz );
  
  if ( VPtCommand == NULL ) {
    err_retfail ( -1, (ERR_OUT,"Cannot alloc command record - %d bytes", VTotCmdSz ) );
  }

  VPtCommand->TotSz = VTotCmdSz;

  CPC_VGCommandsPt[ObjId][0 /*CPC_CMD_REC_NB */] = VPtCommand;
  *PtCommand = VPtCommand;

  return (0); /* Return command index !!! - NOW it's always 0 */
}

/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    : NOW - CmdId IS NOT handled => Only default CmdId = 0 is used.
Level     : This is a user level function.
Date      : 12/01/2008
Doc date  : 
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 CPC_PRIV_FFreeCmd ( SInt8 ObjId, SInt8 CmdId ) {

  CmdId = 0; // => Force CmdId to 0

  err_retfailnull ( CPC_VGCommandsPt[ObjId][CmdId], (ERR_OUT,"Pt Cmd Record Id=%d == NULL", CmdId) );
    
  free ( CPC_VGCommandsPt[ObjId][CmdId] );
  
  return (0);
}

/*******************************************************************************
Prototype : SInt32 CPC_FSendCmd ( SInt8 ObjId,
          :                       SInt32 CmdType, SInt32 CmdSubType,
          :                       SInt32 CmdRetMode, SInt32 CmdRetSz,
          :                       char* CmdStr,
          :                       UInt8 *PtParam, UInt32 ParamSz ) {
          :  
Goal      : Send a command.
          : 
Inputs    : ObjId      - object identifier 
          : CmdType    - the type of command, ex     : COM_CMD_TYPE_SYS 
          : CmdSubType - the sub type of command, ex : COM_CMD_STYPE_SYS_USB_LISTEN
          : CmdRetMode - the answer mode
          :              COM_CMD_RET_MODE_WAIT => the function wait for answer
          :              COM_CMD_RET_MODE_NO_WAIT => the function returns, you must
          : call CPC_FGetCmdRes with command index as paramter to get the answer.
          : CmdRetSz   - the size of command answer
          : CmdStr     - a string to identify the command for human ( debug ) 
          : PtParam    - pointer to command parameters 
          : ParamSz    - command parameters size
          :
Ouputs    : Command index if ok, negative value in case of error.
Globals   :
Remark    : 
Level     : This is a user level function.
Date      : 27/03/2003
Rev       : 09/01/2008
          : - Return CmdIndex if ok
          : 12/01/2008
          : - Bug correction => Add CPC_PRIV_FFreeCmd call if PIP_FPutDatas failed
Doc date  : 12/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 CPC_FSendCmd ( SInt8 ObjId, SInt32 CmdType, SInt32 CmdSubType, SInt32 CmdRetMode, SInt32 CmdRetSz, char* CmdStr, UInt8 *PtParam, UInt32 ParamSz ) {

  SInt32        VRet;
  COM_TCommand* VPtCommand;
  SInt32        VCmdIndex;
  SInt32*       VPtCmdParam;
  CPC_TContext* VPt;

  CPC_CHK_OBJ_ID (ObjId);
  
  VPt = &CPC_VGContext[ObjId];

  /* Try to get a command structure */

  VCmdIndex = CPC_PRIV_FGetNewCmd ( ObjId, ParamSz, &VPtCommand );

  err_retfail ( VCmdIndex, (ERR_OUT,"No free command record") );

  /* Set command type & parameters */

  VPtCommand->Index      = VCmdIndex; /* Always 0 at the moment */
  VPtCommand->Type       = CmdType;
  VPtCommand->SubType    = CmdSubType;
  VPtCommand->RetMode    = CmdRetMode;
  VPtCommand->RetSz      = CmdRetSz;
  VPtCommand->PtRetBuff  = NULL;
  VPtCommand->ParamSz    = ParamSz;

  sprintf ( VPtCommand->StrCmd, "%s", CmdStr );
  
  memcpy ( &VPtCommand->ParamBuff, PtParam, ParamSz );  

	VRet = PIP_FPutDatas ( ObjId, 0 /* MultiExec */ , PIP_TRANS_MSG_TYPE_COMMAND, (UInt8*) VPtCommand, CPC_OBJ_CMD_BLK_SZ /* -1 => use default BlkSz */, VPtCommand->TotSz );
  
  if ( VRet < 0 ) {
    CPC_PRIV_FFreeCmd ( ObjId, VCmdIndex );
    err_retfail ( -1, (ERR_OUT,"CPC_FPutDatas fail => return %d ",VRet) );
  }

  return (VCmdIndex);
}

/*******************************************************************************
Prototype : UInt32 CPC_FGetCmdRes ( SInt8 ObjId, SInt32 CmdIndex, UInt8** PtPtRes )
Goal      : Get the answer to a command send by CPC_FSendCmd.
          :
Inputs    : ObjId    - object identifier 
          : CmdIndex - index of command send ( this the way to link command to answer )
          : PtPtRes  - this pointer parameter will point to the answer structure.
          :
Ouputs    : 0 if ok, negative value in case of error. 
Globals   :
Remark    :
Level     : This is a user level function.
Date      : 28/03/2003
Rev					: 10/01/2008
          : - Bug correction => 
Doc date  : 12/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

UInt32 CPC_FGetCmdRes ( SInt8 ObjId, SInt32 CmdIndex, UInt8** PtPtRes ) {

  SInt32        VRet;
  UInt32        VGetDataSz; 
  COM_TCommand* VPtCommand;
  CPC_TContext* VPt;

  CPC_CHK_OBJ_ID (ObjId);
  
  VPt = &CPC_VGContext[ObjId];
  

  err_retnull ( PtPtRes, (ERR_OUT,"PtPtRes == NULL") );
  
  VPtCommand = CPC_VGCommandsPt[ObjId][CmdIndex];
  
  /* Set PtRes to NULL by default - overwrite it later if succeed */
  
  *PtPtRes = NULL;
  
  if ( VPtCommand == NULL ) {
    err_retfail ( -1, (ERR_OUT,"No command of %d index registered !", CmdIndex) );
  }

  VPtCommand->PtRetBuff = (SInt8*) malloc ( VPtCommand->RetSz );
  
  if ( VPtCommand->PtRetBuff == NULL ) {
    err_retfail ( -1, (ERR_OUT,"Malloc of RetBuffer %d bytes fail", VPtCommand->RetSz ) );
  }

  *PtPtRes = (UInt8*) VPtCommand->PtRetBuff;
  
  VRet = CPC_FGetDatas ( ObjId, 0 /* MultiExec */, (UInt8*) VPtCommand->PtRetBuff, VPtCommand->RetSz, &VGetDataSz );

  if ( VRet == 1 ) {
    
    if ( VPtCommand->RetSz != VGetDataSz ) {
      err_retfail ( -1, (ERR_OUT,"Ask return sz = %d <> return sz = %d", VPtCommand->RetSz, VGetDataSz) );
    }
        
  }
  
  else {
    
    if ( VRet == -2 ) {
      err_trace (( ERR_OUT, "CPC_FGetDatas (...) Ret=-1 => No block to read" ));
      return (-1);
    }
    
    else {
    	err_retfail ( -1, (ERR_OUT,"CPC_FGetDatas (...) failed => Ret=%d", VRet) );    
    }
    
  }

      
  err_retok (( ERR_OUT, "" ));
}



/* ------------------ USB & VME command set ------------------ */

/*******************************************************************************
Prototype : SInt32 CPC_FUsbCmdListenRq ( SInt8 ObjId )
Goal      : Send the USB listen command.
Inputs    : ObjId - object identifier
Ouputs    : 0 if ok, negative value if fail.
Globals   :
Remark    : Should be checked, answer / return value / error code
Level     :
Date      : 13/04/2003
Doc date  : 13/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 CPC_FCmdHello ( SInt8 ObjId ) {
  
  SInt32        VRet;
  SInt32        VCmdIndex;
  UInt8*        VPtRes;
  CPC_TContext* VPt;
  
  CPC_CHK_OBJ_ID (ObjId);
  
  VPt = &CPC_VGContext[ObjId];
  
  VCmdIndex = CPC_FSendCmd ( ObjId,
  	COM_CMD_TYPE_SYS,
  	COM_CMD_STYPE_SYS_HELLO,
  	COM_CMD_RET_MODE_WAIT  /* CmdRetMode */,
  	4 /* CmdRetSz */,
  	"Hello world" /* CmdStr */,
  	NULL /* PtParam */,
  	0 /* ParamSz */ );
  
  
  err_retfail ( VCmdIndex, (ERR_OUT,"FSendCmd fail => return %d", VCmdIndex) );
  
  VRet = CPC_FGetCmdRes ( ObjId, VCmdIndex, &VPtRes );

  free ( VPtRes );
  
  err_retfail ( VRet, (ERR_OUT,"FGetCmdResfail => return %d", VRet) );
  
  
  err_retok (( ERR_OUT, "" ));
}


/*******************************************************************************
Prototype : SInt32 CPC_FUsbCmdListenRq ( SInt8 ObjId )
Goal      : Send the USB listen command.
Inputs    : ObjId - object identifier 
Ouputs    : 0 if ok, negative value if fail.
Globals   :
Remark    : Should be checked, answer / return value / error code 
Level     :
Date      : 28/03/2003
Doc date  : 12/04/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/******************************************************************************/

SInt32 CPC_FUsbCmdListenRq ( SInt8 ObjId ) {

  SInt32        VRet;
  SInt32        VCmdIndex;
  UInt8*        VPtRes;
  CPC_TContext* VPt;

  CPC_CHK_OBJ_ID (ObjId);
  
  VPt = &CPC_VGContext[ObjId];
  
  VCmdIndex = CPC_FSendCmd ( ObjId,
                             COM_CMD_TYPE_SYS,
                             COM_CMD_STYPE_SYS_USB_LISTEN,
                             COM_CMD_RET_MODE_WAIT  /* CmdRetMode */,
                             4 /* CmdRetSz */,
                             "Listen request" /* CmdStr */,
                             NULL /* PtParam */,
                             0 /* ParamSz */ );
        
        
  err_retfail ( VCmdIndex, (ERR_OUT,"FSendCmd fail => return %d", VCmdIndex) );
  
  VRet = CPC_FGetCmdRes ( ObjId, VCmdIndex, &VPtRes );

  free ( VPtRes );
  
  err_retfail ( VRet, (ERR_OUT,"FGetCmdResfail => return %d", VRet) );
  
  err_retok (( ERR_OUT, "" ));
}

SInt32 CPC_FCUsbCmdDataTrfRq () {
  return (0);
}

SInt32 CPC_FUsbCmdLoadEepromRq () {
  return (0);
}

SInt32 CPC_FUsbCmdLoadEepromExec ( char *SrcSvfFile ) {
  return (0);
}

SInt32 CPC_FCmdUsbLoadEepromAbort () {
  return (0);
}

SInt32 CPC_FUsbCmdLoadEepromDone () {
  return (0);
}

SInt32 CPC_FUsbCmdJtagWriteRq () {
  return (0);
}

SInt32 CPC_FUsbCmdJtagReadRq () {
  return (0);
}



/* !!! How to send back debug information from uC to PC ? */

#endif