
/*******************************************************************************
File      : x:\lib\com\inter_app_com\inter_app_com.c
Goal      : Functions of inter applications communication library.
Prj date  : 05/12/2007
File date : //200
Doc date  : //200
Author    : Gilles CLAUS
----------------------------------------------------------------------------------
License   : You are free to use this source files for your own development as long
          : as it stays in a public research context. You are not allowed to use it
          : for commercial purpose. You must put this header with laboratory and
          : authors names in all development based on this library.
----------------------------------------------------------------------------------
Labo      : IPHC */
/*******************************************************************************/


#ifndef INTER_APP_COM_C
#define INTER_APP_COM_C

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : 
          :
Goal      : 
          :
Inputs    :
          :
Ouputs    : 
          :
Globals   : 
          :
Remark    :
          :
Level     : 
Date      : 05/12/2007
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 05/12/2007
					: 09/01/2008
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__FBegin ( SInt8 ErrLogLvl, char* ErrLogFilePath ) {

  IAC__TContext* PtCont = &IAC__VGContext;

  ERR_FBegin           ( (ErrLogLvl != ERR_LOG_LVL_NONE ) /* Enable */, ErrLogFilePath );
  ERR_FSetFileLogLevel ( ErrLogLvl );
  ERR_FSetUserLogLevel ( ErrLogLvl );

  memset ( &IAC__VGContext, 0, sizeof (IAC__VGContext) );

  IAC__PRIV_FSockInit ();

  PtCont->Rmp.CmdComPcObjId 									= 0;
  PtCont->Rmp.CmdComUcObjId 									= 1;
  PtCont->Rmp.CmdGetCmdResLoopPeriod = 50;
  PtCont->Rmp.CmdGetCmdResTryNb						= 60;
    
  PtCont->Rmp.DataComPcObjId 							 = 2;
  PtCont->Rmp.DataComUcObjId 								= 3;
  
  CPC_FBegin ( PtCont->Rmp.CmdComPcObjId /* ObjId */, 0 /* ComMedia */,  0 /* ComId */, CPC_OBJ_CMD_BLK_SZ 					/* SendBuffSz */, PIP_FFileSend /* PtSendFunc */, CPC_OBJ_CMD_RES_BLK_SZ /* RecBuffSz */, PIP_FFileGet /* PtGetFunc */ );
  CUC_FBegin ( PtCont->Rmp.CmdComUcObjId /* ObjId */, 0 /* ComMedia */,  0 /* ComId */, CPC_OBJ_CMD_RES_BLK_SZ  /* SendBuffSz */, PIP_FFileSend /* PtSendFunc */, CPC_OBJ_CMD_BLK_SZ 				/* RecBuffSz */, PIP_FFileGet /* PtGetFunc */ );
    
  CPC_FBegin ( PtCont->Rmp.DataComPcObjId /* ObjId */, 0 /* ComMedia */,  1 /* ComId */, IAC__RMP_DATA_BLOCK_SZ 	/* SendBuffSz */, PIP_FFileSend /* PtSendFunc */, IAC__RMP_DATA_BLOCK_SZ  /* RecBuffSz */, PIP_FFileGet /* PtGetFunc */ );
  CUC_FBegin ( PtCont->Rmp.DataComUcObjId /* ObjId */, 0 /* ComMedia */,  1 /* ComId */, IAC__RMP_DATA_BLOCK_SZ  /* SendBuffSz */, PIP_FFileSend /* PtSendFunc */, IAC__RMP_DATA_BLOCK_SZ 	/* RecBuffSz */, PIP_FFileGet /* PtGetFunc */ );
  
  err_retok (( ERR_OUT, "Lib compiled on %s at %s", __DATE__, __TIME__ ));
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 05/12/2007
          : 09/01/2008
          Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__FEnd () {

  IAC__TContext* PtCont = &IAC__VGContext;

  CPC_FEnd ( PtCont->Rmp.CmdComPcObjId /* ObjId */ );
  CUC_FEnd ( PtCont->Rmp.CmdComUcObjId /* ObjId */ );
  
  return (0);
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 09/01/2008
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__RMP_FSetFuncRunStatusAnswer ( IAC__RMP_TFuncRunStatusAnswer PtFunc ) {
  
  IAC__VGContext.Rmp.FuncRunStatusAnswer = PtFunc;

  return (0);
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 09/01/2008
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__RMP_FSetFuncRunStatusAnswer2 ( IAC__RMP_TFuncRunStatusAnswer2 PtFunc ) {
  
  IAC__VGContext.Rmp.FuncRunStatusAnswer2 = PtFunc;
  
  return (0);
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 10/01/2008
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__RMP_FSetFuncAskEventAnswer ( IAC__RMP_TFuncAskEventAnswer PtFunc ) {
  
  IAC__VGContext.Rmp.FuncAskEventAnswer = PtFunc;
  
  return (0);
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 09/01/2008
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__RMP_FCmdAskRunStatus ( IAC__RMP_TRunStatusAnswer* PtStatus ) {
  
  IAC__TContext* PtCont = &IAC__VGContext;
  SInt32        VRet;
  SInt32        VCmdIndex;
  IAC__RMP_TRunStatusAnswer* VPtRes;
  CPC_TContext* VPt;
  SInt8         ViCmdResTry;
    
  
  VPt = &CPC_VGContext[PtCont->Rmp.CmdComPcObjId];
  
  VCmdIndex = CPC_FSendCmd ( PtCont->Rmp.CmdComPcObjId,
    IAC__CMD_TYPE__RMP,
    IAC__CMD_STYPE__RMP__ASK_RUN_STATUS,
  	 COM_CMD_RET_MODE_WAIT  /* CmdRetMode */,
    sizeof ( IAC__RMP_TRunStatusAnswer ) /* CmdRetSz */,
  	 "Run Status" /* CmdStr */,
  	 NULL /* PtParam */,
  	 0 /* ParamSz */ );
  
  
  err_retfail ( VCmdIndex, (ERR_OUT,"FSendCmd fail => return %d", VCmdIndex) );
    
  for ( ViCmdResTry=0; ViCmdResTry < PtCont->Rmp.CmdGetCmdResTryNb; ViCmdResTry++ ) {
    
    VRet = CPC_FGetCmdRes ( PtCont->Rmp.CmdComPcObjId, VCmdIndex, (UInt8**) &VPtRes /* & ((UInt8*) VPtRes) 14/01/08 */ );
    
    if ( VRet >= 0 ) {
      break;
    }
    
    #ifdef ROOT_ROOT
    	gSystem->Sleep ( PtCont->Rmp.CmdGetCmdResLoopPeriod );
    #else
    	Sleep ( PtCont->Rmp.CmdGetCmdResLoopPeriod );
    #endif
    
  }
  
  if ( VRet < 0 ) {
    CPC_PRIV_FFreeCmd ( PtCont->Rmp.CmdComPcObjId, VCmdIndex );
    err_retfail ( -1, (ERR_OUT,"FGetCmdResfail => return %d after %d try each %d ms", VRet, PtCont->Rmp.CmdGetCmdResTryNb, PtCont->Rmp.CmdGetCmdResLoopPeriod ) );
  }
  
  
  err_warning (( ERR_OUT, "TRACE => Anser get after %d polling cyles of each %d ms ", ViCmdResTry, PtCont->Rmp.CmdGetCmdResLoopPeriod ));
  
  
  if ( PtStatus != NULL ) {
    *PtStatus = *VPtRes;
  }
  
  
  free ( VPtRes );

  CPC_PRIV_FFreeCmd ( PtCont->Rmp.CmdComPcObjId, VCmdIndex );
  
  
  err_retok (( ERR_OUT, "" ));
  
  
  return (0);
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 11/01/2008
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__RMP_FCmdAskRunStatus2 ( IAC__RMP_TRunStatusAnswer2* PtStatus ) {
  
  IAC__TContext*              PtCont = &IAC__VGContext;
  SInt32                      VRet;
  SInt32                      VCmdIndex;
  IAC__RMP_TRunStatusAnswer2* VPtRes;
  CPC_TContext*               VPt;
  SInt8                       ViCmdResTry;
  
  VPt = &CPC_VGContext[PtCont->Rmp.CmdComPcObjId];
  
  VCmdIndex = CPC_FSendCmd ( PtCont->Rmp.CmdComPcObjId,
  IAC__CMD_TYPE__RMP,
  IAC__CMD_STYPE__RMP__ASK_RUN_STATUS_2,
  COM_CMD_RET_MODE_WAIT  /* CmdRetMode */,
  sizeof ( IAC__RMP_TRunStatusAnswer2 ) /* CmdRetSz */,
  "Run Status 2" /* CmdStr */,
  NULL /* PtParam */,
  0 /* ParamSz */ );
  
  
  err_retfail ( VCmdIndex, (ERR_OUT,"FSendCmd fail => return %d", VCmdIndex) );
  
  for ( ViCmdResTry=0; ViCmdResTry < PtCont->Rmp.CmdGetCmdResTryNb; ViCmdResTry++ ) {
    
    VRet = CPC_FGetCmdRes ( PtCont->Rmp.CmdComPcObjId, VCmdIndex, (UInt8**) &VPtRes );
    
    if ( VRet >= 0 ) {
      break;
    }
    
    #ifdef ROOT_ROOT
      gSystem->Sleep ( PtCont->Rmp.CmdGetCmdResLoopPeriod );
    #else
      Sleep ( PtCont->Rmp.CmdGetCmdResLoopPeriod );
    #endif
    
  }
  
  if ( VRet < 0 ) {
    CPC_PRIV_FFreeCmd ( PtCont->Rmp.CmdComPcObjId, VCmdIndex );
    err_retfail ( -1, (ERR_OUT,"FGetCmdResfail => return %d after %d try each %d ms", VRet, PtCont->Rmp.CmdGetCmdResTryNb, PtCont->Rmp.CmdGetCmdResLoopPeriod ) );
  }
  
  err_warning (( ERR_OUT, "TRACE => Anser get after %d polling cyles of each %d ms ", ViCmdResTry, PtCont->Rmp.CmdGetCmdResLoopPeriod ));
  
  
  if ( PtStatus != NULL ) {
    *PtStatus = *VPtRes;
  }
  
  
  free ( VPtRes );

  CPC_PRIV_FFreeCmd ( PtCont->Rmp.CmdComPcObjId, VCmdIndex );
  
  err_retok (( ERR_OUT, "" ));
  
  
  return (0);
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 10/01/2008
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__RMP_FCmdAskEvent ( IAC__RMP_TAskEventAnswer* PtAns ) {
  
  IAC__TContext* 											PtCont = &IAC__VGContext;
  SInt32        												VRet;
  SInt32        												VCmdIndex;
  IAC__RMP_TAskEventAnswer* VPtRes;
  static IAC__RMP_TAskEventPar				 VCmdPar;
  CPC_TContext* 												VPt;
  SInt8         												ViCmdResTry;
  
  VPt = &CPC_VGContext[PtCont->Rmp.CmdComPcObjId];
  
  ++VCmdPar.EvNb;
  VCmdPar.TestTag = 666;
  
  VCmdIndex = CPC_FSendCmd ( PtCont->Rmp.CmdComPcObjId,
  	 IAC__CMD_TYPE__RMP,
  	 IAC__CMD_STYPE__RMP__ASK_EVENT,
  	 COM_CMD_RET_MODE_WAIT  /* CmdRetMode */,
  	 sizeof ( IAC__RMP_TAskEventAnswer ) /* CmdRetSz */,
  	 "Ask event" /* CmdStr */,
    (UInt8*) &VCmdPar /* PtParam */,
     sizeof ( VCmdPar ) /* ParamSz */ );
  
  
  err_retfail ( VCmdIndex, (ERR_OUT,"FSendCmd fail => return %d", VCmdIndex) );
  
  for ( ViCmdResTry=0; ViCmdResTry < PtCont->Rmp.CmdGetCmdResTryNb; ViCmdResTry++ ) {
    
    VRet = CPC_FGetCmdRes ( PtCont->Rmp.CmdComPcObjId, VCmdIndex, (UInt8**) &VPtRes );
    
    if ( VRet >= 0 ) {
      break;
    }

    #ifdef ROOT_ROOT
    	 gSystem->Sleep ( PtCont->Rmp.CmdGetCmdResLoopPeriod );
    #else
    	 Sleep ( PtCont->Rmp.CmdGetCmdResLoopPeriod );
    #endif
    
  }
  
  if ( VRet < 0 ) {
    CPC_PRIV_FFreeCmd ( PtCont->Rmp.CmdComPcObjId, VCmdIndex );
    err_retfail ( -1, (ERR_OUT,"FGetCmdResfail => return %d after %d try each %d ms", VRet, PtCont->Rmp.CmdGetCmdResTryNb, PtCont->Rmp.CmdGetCmdResLoopPeriod ) );
  }
  
  err_warning (( ERR_OUT, "TRACE => Anser get after %d polling cyles of each %d ms ", ViCmdResTry, PtCont->Rmp.CmdGetCmdResLoopPeriod ));
  
  
  if ( PtAns != NULL ) {
    *PtAns = *VPtRes;
  }
  
  free ( VPtRes );
    
  CPC_PRIV_FFreeCmd ( PtCont->Rmp.CmdComPcObjId, VCmdIndex );
  
  err_retok (( ERR_OUT, "" ));
  
  
  return (0);
}




/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 09/01/2008
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__RMP_FDaqCmdInterpreter () {
  
  IAC__TContext* PtCont = &IAC__VGContext;
  SInt32 VRet;
  SInt32 VRetInterpreter;
  char	  VStrStatus[GLB_CMT_SZ];
  COM_TCommand* 									VPtCmd;

  UInt8*	 					 VPtData;
	IAC__RMP_TRunStatusAnswer  VRunStatusAnswer;
  IAC__RMP_TRunStatusAnswer2 VRunStatusAnswer2;
  IAC__RMP_TAskEventAnswer   VAskEventAnswer;


  VRetInterpreter = CUC_FCmdInterpreter ( PtCont->Rmp.CmdComUcObjId, VStrStatus, &VPtCmd, &VPtData );
  
  
  switch ( VRetInterpreter ) {
    
    case -1 : {
      err_retfail ( VRetInterpreter, (ERR_OUT,"Interpreter Error !") );
      break; }
    
    // Command
    
    case 1 : {
    
      err_trace (( ERR_OUT, "Cmd = %s", VPtCmd->StrCmd ));
  
      if ( VPtCmd->Type != IAC__CMD_TYPE__RMP ) {
        err_trace (( ERR_OUT, "Not a RMP cmd !" ));
        break;
      }
      
      switch ( VPtCmd->SubType ) {
        
        
        case IAC__CMD_STYPE__RMP__ASK_RUN_STATUS : {
          
          if ( IAC__VGContext.Rmp.FuncRunStatusAnswer != NULL ) {
            
            err_trace (( ERR_OUT, "Cmd IAC__CMD_STYPE__RMP__ASK_RUN_STATUS received - Response done" ));
            
            IAC__VGContext.Rmp.FuncRunStatusAnswer ( VPtCmd, &VRunStatusAnswer );
            
            VRet = CUC_FPutDatas ( PtCont->Rmp.CmdComUcObjId, 0 /* MultiExec */, (UInt8*) &VRunStatusAnswer /* PtSrc */, CPC_OBJ_CMD_RES_BLK_SZ /* BlkSz */, VPtCmd->RetSz /* TotSz */ );
          
          	 err_retfail ( VRet, (ERR_OUT,"CUC_FPutDatas failed !") );
          }
          
          break; }


        case IAC__CMD_STYPE__RMP__ASK_RUN_STATUS_2 : {
          
          if ( IAC__VGContext.Rmp.FuncRunStatusAnswer2 != NULL ) {
            
            err_trace (( ERR_OUT, "Cmd IAC__CMD_STYPE__RMP__ASK_RUN_STATUS_2 received - Response done" ));
            
            IAC__VGContext.Rmp.FuncRunStatusAnswer2 ( VPtCmd, &VRunStatusAnswer2 );
            
            VRet = CUC_FPutDatas ( PtCont->Rmp.CmdComUcObjId, 0 /* MultiExec */, (UInt8*) &VRunStatusAnswer2 /* PtSrc */, CPC_OBJ_CMD_RES_BLK_SZ /* BlkSz */, VPtCmd->RetSz /* TotSz */ );
            
            err_retfail ( VRet, (ERR_OUT,"CUC_FPutDatas failed !") );
          }
          
          break; }
        

        case IAC__CMD_STYPE__RMP__ASK_EVENT : {
          
          if ( IAC__VGContext.Rmp.FuncAskEventAnswer != NULL ) {
            
            err_trace (( ERR_OUT, "Cmd IAC__CMD_STYPE__RMP__ASK_EVENT received - Resonse done" ));
            
            IAC__VGContext.Rmp.FuncAskEventAnswer ( VPtCmd, &VAskEventAnswer );
            
            VRet = CUC_FPutDatas ( PtCont->Rmp.CmdComUcObjId, 0 /* MultiExec */, (UInt8*) &VAskEventAnswer /* PtSrc */, CPC_OBJ_CMD_RES_BLK_SZ /* BlkSz */, VPtCmd->RetSz /* TotSz */ );
            
            err_retfail ( VRet, (ERR_OUT,"CUC_FPutDatas failed !") );
          }
          
          break; }
        

        default : {
          err_error (( ERR_OUT, "Cmd SubType=%d unknown", VPtCmd->SubType ));
          break;
        }
        
      }
      
      
      
      
      break; }
    
    // Data
    
    case 2 : {
      err_trace (( ERR_OUT, "Data transaction" ));
            
      break; }
    
  }
  

  return (0);
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 10/01/2008
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__RMP_FSendEvent ( UInt8* PtEv, SInt32 Sz ) {
  
  IAC__TContext* PtCont = &IAC__VGContext;
  SInt32 VRet;
  
  err_retnull ( PtEv, (ERR_OUT,"PtEv == NULL") );
  
  if ( Sz <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad Sz =%d <= 0", Sz) );
  }
  
  VRet =  CUC_FPutDatas ( IAC__VGContext.Rmp.DataComUcObjId /*  ObjId */, 0 /* MultiExec */, PtEv, -1 /* BlkSz */, Sz /* TotSz */ );
  
  err_retfail ( VRet, (ERR_OUT,"CUC_FPutDatas (...) failed => Ret=%d", VRet ) );
  
  return (0);
}
  


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    : 
:
Globals   :
:
Remark    :
:
Level     :
Date      : 12/01/2008
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__RMP_FReceiveEvent ( UInt8* PtEv, SInt32 MaxEvSz, SInt32* PtEvSz ) {
  
  IAC__TContext* PtCont = &IAC__VGContext;
  SInt32 VRet;
  UInt32 VEvSz;
  
  err_retnull ( PtEv  , (ERR_OUT,"PtEv   == NULL") );
  err_retnull ( PtEvSz, (ERR_OUT,"PtEvSz == NULL") );
  
  if ( MaxEvSz <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad MaxSz =%d <= 0", MaxEvSz) );
  }

  
  VRet = CPC_FGetDatas ( IAC__VGContext.Rmp.DataComPcObjId /*  ObjId */, 0 /* MultiExec */, PtEv, MaxEvSz, &VEvSz /* PtGetDataSz */ );
  
  
  switch ( VRet ) {
    
    case 0 : {
      err_retfail ( -1, (ERR_OUT,"This case SHOULD NOT happne => CPC_FGetDatas called with MultiExec = 0 which returns 0 ! = only one block read !") );
      break; }
    
    case 1 : {
      
      *PtEvSz = VEvSz;
      return (0); // Normal case
      break; }
    
    case -1 : {
      err_trace (( ERR_OUT, "Nothing to read"  ));
      return (-1);
      break; }
    
    case -2 : {
      err_retfail ( -1, (ERR_OUT,"Error reading data with CPC_FGetDatas (...) which return %d", VRet) );
      break; }
    
    default : {
      err_retfail ( -1, (ERR_OUT,"Unknown error => CPC_FGetDatas (...) return %d", VRet) );
      break;
    }
  }

}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 12/01/2008
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__RMP_FTestFillEvent ( UInt8* PtEv, SInt32 Sz ) {
  
  SInt32 Vi;
  
  err_retnull ( PtEv, (ERR_OUT,"PtEv == NULL") );
  
  if ( Sz <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad Sz =%d <= 0", Sz) );
  }
  
  for ( Vi=0; Vi < Sz; Vi++ ) {
    PtEv[Vi] = Vi;
  }

  return (0);  
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 12/01/2008
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__RMP_FTestCompareEvents ( UInt8* PtEvToCmp, UInt8* PtEvRef, SInt32 Sz ) {
  
  SInt32 VErrCnt;
  SInt32 Vi;
  
  err_retnull ( PtEvRef  , (ERR_OUT,"PtEvRef   == NULL") );
  err_retnull ( PtEvToCmp, (ERR_OUT,"PtEvToCmp == NULL") );
  
  if ( Sz <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad Sz =%d <= 0", Sz) );
  }

  VErrCnt = 0;

  for ( Vi=0; Vi < Sz; Vi++ ) {
    
    if ( PtEvToCmp[Vi] != PtEvRef[Vi]) {
      err_error (( ERR_OUT, "Cmp error Mem[%6d] : Received=%d - Reference=%d", Vi, PtEvToCmp[Vi], PtEvRef[Vi] ));
      ++VErrCnt;
    }
    
  }


  if ( VErrCnt >= 0 ) {
    err_retval ( VErrCnt, ( ERR_OUT, "Event compare => %d errors !", VErrCnt ) );
  }
  
  else {
    err_retok (( ERR_OUT, "" ));
  }

}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 12/01/2008
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

#ifndef ROOT_ROOT

SInt32 IAC__RMP_FTestPrintMemStatusRec ( LPMEMORYSTATUS PtRec ) {
  
  err_retnull ( PtRec, (ERR_OUT,"PtRec == NULL") );
  
  err_error (( ERR_OUT, "------------ Mem Status ------------" ));
  err_error (( ERR_OUT, "Percent of memory in use     = %d", PtRec->dwMemoryLoad ));
  err_error (( ERR_OUT, "Bytes of physical memory     = %d", PtRec->dwTotalPhys ));
  err_error (( ERR_OUT, "Free physical memory bytes   = %d", PtRec->dwAvailPhys ));
  err_error (( ERR_OUT, "Bytes of paging file         = %d", PtRec->dwTotalPageFile ));
  err_error (( ERR_OUT, "Free bytes of paging file    = %d", PtRec->dwAvailPageFile ));
  err_error (( ERR_OUT, "User bytes of address space  = %d", PtRec->dwTotalVirtual ));
  err_error (( ERR_OUT, "Free user bytes              = %d", PtRec->dwAvailVirtual ));
  err_error (( ERR_OUT, "------------------------------------" ));    
    
  return (0);  
}
  
#endif

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 12/01/2008
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

#ifndef ROOT_ROOT

SInt32 IAC__RMP_FTestGetMemStatus ( SInt8 Print, LPMEMORYSTATUS PtRec, SInt32* PtStackAvailSz ) {
  
  MEMORYSTATUS VMemStatus;
  SInt32       VStackAvailSz;


  VMemStatus.dwLength = sizeof ( MEMORYSTATUS );
  
  GlobalMemoryStatus ( &VMemStatus );

  VStackAvailSz = stackavail (); 

  if ( Print == 1 ) {
    IAC__RMP_FTestPrintMemStatusRec ( &VMemStatus );
    err_error (( ERR_OUT, "----------------------------------" ));
    err_error (( ERR_OUT, "Stack available = %d bytes",  VStackAvailSz ));
    err_error (( ERR_OUT, "----------------------------------" ));
  }


  if ( PtRec != NULL ) {
    *PtRec = VMemStatus;
  }
  
  if ( PtStackAvailSz != NULL ) {
    *PtStackAvailSz = VStackAvailSz;
  }

}

#endif

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 04/01/2008
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__FPrepareMsg ( SInt8 Priority, SInt8 Type, SInt16 SubType, IAC__TWho* From, IAC__TWho* To ) {
  
  
  
  return (0);
}





/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 10/11/2008
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */


struct sockaddr IAC__PRIV_FTcpFormatAdress ( char * Host, u_short Port )
{
  struct sockaddr_in addr;
  struct sockaddr addrRet;
  struct hostent FAR *lphost ;
  u_long IP;
  
  
  memset((char*)&addr, 0, sizeof(addr));
  
  /*	Soit on fournit une adresse IP, soit on fournit un nom	*/
  
  if ((IP = inet_addr(Host)) == (u_long)INADDR_NONE)
  {
    if ((lphost  = gethostbyname(Host))==NULL)
    {
      memset( (char * )&addrRet, 0, sizeof(addrRet) );
      return  addrRet;
    }
    
    addr.sin_family = lphost->h_addrtype;
    
    #ifdef _WIN16 /* A définir dans le projet WIN16	*/
    _fmemcpy (&addr.sin_addr, lphost->h_addr, lphost->h_length);
    #else /*	WIN32, UNIX*/
    memcpy (&addr.sin_addr, lphost->h_addr, lphost->h_length);
    #endif
    
  }
  
  else
  {
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = IP;
  }
  
  /*  Port destination    */
  
  addr.sin_port = htons((u_short)Port );
  
  memcpy( (char *)&addrRet, (char *)&addr, sizeof(addrRet) );
  
  return addrRet;
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 05/01/2008
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */





/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    : The Index of Rec port context record
:
Globals   :
:
Remark    :
:
Level     :
Date      : 10/11/2008
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 IAC__FSockSendShortMsg ( SInt32 ContextId, UInt8* PtMsg, UInt32 MsgSz ) {
  
  IAC__TSockSenderPort* VPtSocket;
  SInt16                ViTry;
  
    
  /* -------- */
  /* Check Id */
  /* -------- */
  
  err_retfail ( IAC__PRIV_FChkSenderContextId (ContextId), (ERR_OUT,"Bad Id=%d", ContextId ) );
  
  VPtSocket = &(IAC__VGContext.Sock.ASenderPort[ContextId]);
  
  
  /* ------------ */
  /* Check status */
  /* ------------ */
  
  if ( VPtSocket->Started != 1 ) {
    err_retfail ( -1, (ERR_OUT,"Access [%d] not started => Can't send message", VPtSocket->Id ) );
  }

  /* --------------- */
  /* Check port mode */
  /* --------------- */
  
  if ( VPtSocket->ParLongMsg == 1 ) {
    err_retfail ( -1, (ERR_OUT,"This port doesn't support short message ! ParLongMsg=%d", VPtSocket->ParLongMsg ) );
  }
  

  /* ---------------------------- */
  /* Check message pointer & size */
  /* ---------------------------- */
  
  err_retnull ( PtMsg, (ERR_OUT,"PtMsg == NULL !") );
  
  /* Only for first version of text header => "SZ="
  
  if ( ( MsgSz + IAC__SOCK_HEADER_TOT_SZ ) > IAC__SO_MAX_SHORT_MSG_SIZE ) {
    err_retfail ( -1, (ERR_OUT,"Message too long size=%d +  IAC__SOCK_HEADER_TOT_SZ=%d > IAC__SO_MAX_SHORT_MSG_SIZE=%d", MsgSz, IAC__SOCK_HEADER_TOT_SZ, IAC__SO_MAX_SHORT_MSG_SIZE ));
  }
  
  */
  
  if ( ( MsgSz + (VPtSocket->CstHeaderSz) + (VPtSocket->CstTrailerSz) ) > IAC__SO_MAX_SHORT_MSG_SIZE ) {
    err_retfail ( -1, (ERR_OUT,"Message too long size + (Header & Trailer) =%d  > IAC__SO_MAX_SHORT_MSG_SIZE=%d", MsgSz + (VPtSocket->CstHeaderSz) + (VPtSocket->CstTrailerSz), IAC__SO_MAX_SHORT_MSG_SIZE ));
  }
  
  
  VPtSocket->ParPtMsg = PtMsg;
  VPtSocket->ParMsgSz = MsgSz;

  /* ------------------------------------ */
  /* Try to send message ParRetryNb times */
  /* ------------------------------------ */
  
  for ( ViTry=0; ViTry < VPtSocket->ParRetryNb; ViTry++ ) {
    
    /* --------------- */
    /* Reset Status    */
    /* --------------- */
    
    VPtSocket->SendStatus = -1;
    
    /* -------------------- */
    /* Send event to thread */
    /* -------------------- */
    
    if ( PulseEvent ( VPtSocket->EvNewMsgHnd ) == TRUE ) {
      err_trace (( ERR_OUT, "Event EvNewMsgHnd sent" ));
    }
  
    /* ------------------ */
    /* Wait thread answer */
    /* ------------------ */
    
    err_retnull ( VPtSocket->EvAckMsgHnd, (ERR_OUT,"Ack event not installed !" ) );
      
    // Time out => Retry  
      
    if ( WaitForSingleObject ( VPtSocket->EvAckMsgHnd, VPtSocket->ParAckTimeOutMs ) == WAIT_TIMEOUT ) {
      err_error (( ERR_OUT, "No Ack => Exit on time out !! ViTry=%d - Msg : %s", ViTry, PtMsg ));
      continue;
    }
    
    // Error => Retry  
  
    if ( VPtSocket->SendStatus <= 0 ) {
      err_error (( ERR_OUT, "Send error - ViTry=%d - Msg : %s", ViTry, PtMsg ));
      continue;
    }
    
    // OK message sent, Ack received => break loop
    
    err_trace (( ERR_OUT, "Ack received"  ));
    break;
    
      
  } /* End for */
  
  return (VPtSocket->SendStatus);
}
  


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    : The Index of Rec port context record
:
Globals   :
:
Remark    :
:
Level     :
Date      : 10/11/2008
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 IAC__FSockSendLongMsg ( SInt32 ContextId, UInt8* PtMsg, UInt32 MsgSz ) {
  
  IAC__TSockSenderPort* VPtSocket;
  SInt16                ViTry;
  
  
  /* -------- */
  /* Check Id */
  /* -------- */
  
  err_retfail ( IAC__PRIV_FChkSenderContextId (ContextId), (ERR_OUT,"Bad Id=%d", ContextId ) );
  
  VPtSocket = &(IAC__VGContext.Sock.ASenderPort[ContextId]);
  
  
  /* ------------ */
  /* Check status */
  /* ------------ */
  
  if ( VPtSocket->Started != 1 ) {
    err_retfail ( -1, (ERR_OUT,"Access [%d] not started => Can't send message", VPtSocket->Id ) );
  }

  /* --------------- */
  /* Check port mode */
  /* --------------- */
  
  if ( VPtSocket->ParLongMsg == 0 ) {
    err_retfail ( -1, (ERR_OUT,"This port doesn't support long message ! ParLongMsg=%d", VPtSocket->ParLongMsg ) );
  }
  
  /* ---------------------------- */
  /* Check message pointer & size */
  /* ---------------------------- */
  
  err_retnull ( PtMsg, (ERR_OUT,"PtMsg == NULL !") );

  if ( VPtSocket->ParMsgSz > IAC__SOCK_MAX_LONG_MSG_SZ ) {
    err_retfail ( -1, (ERR_OUT,"Message too long size=%d > IAC__SOCK_MAX_LONG_MSG_SZ=%d", MsgSz, IAC__SOCK_MAX_LONG_MSG_SZ ));
  }

  /* --------------- */
  /* Init parameters */
  /* --------------- */

  VPtSocket->ParPtMsg = PtMsg;
  VPtSocket->ParMsgSz = MsgSz;
  
  err_trace (( ERR_OUT, "LongMsgSz = %d", VPtSocket->ParMsgSz ));

  /* ------------------------------------ */
  /* Try to send message ParRetryNb times */
  /* ------------------------------------ */
  
  for ( ViTry=0; ViTry < VPtSocket->ParRetryNb; ViTry++ ) {

    /* --------------- */
    /* Reset Status    */
    /* --------------- */
    
    VPtSocket->SendStatus = -1;
    
    /* -------------------- */
    /* Send event to thread */
    /* -------------------- */
    
    if ( PulseEvent ( VPtSocket->EvNewMsgHnd ) == TRUE ) {
      err_trace (( ERR_OUT, "Event EvNewMsgHnd sent" ));
    }
    
    /* ------------------ */
    /* Wait thread answer */
    /* ------------------ */
    
    err_retnull ( VPtSocket->EvAckMsgHnd, (ERR_OUT,"Ack event not installed !" ) );

    // Time out => Retry
    
    if ( WaitForSingleObject ( VPtSocket->EvAckMsgHnd, VPtSocket->ParAckTimeOutMs  ) == WAIT_TIMEOUT	 ) {
      err_error (( ERR_OUT, "No Ack => Exit on time out !! ViTry=%d - Msg : %s", ViTry, PtMsg ));
      continue;
    }

    // Error => Retry
    
    if ( VPtSocket->SendStatus <= 0 ) {
      err_error (( ERR_OUT, "Send error - ViTry=%d - Msg : %s", ViTry, PtMsg ));
      continue;
    }
    
    // OK message sent, Ack received => break loop
    
    err_trace (( ERR_OUT, "Ack received"  ));
    break;
  
  } /* End for */
    
  
  
  return (VPtSocket->SendStatus);
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 10/11/2008
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */



SInt32 IAC__PRIV_FSockInit ( ) {
  
  WSADATA VStackInf;
    
  // Initialize winsock
    
  WSAStartup( MAKEWORD(2,0), &VStackInf ) ;

  return (0);
}
  
/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 09/11/2008
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__PRIV_FSockGetSenderPortContext () {
  
  SInt32 Vi;
  SInt32 VPortContextId;
  
  VPortContextId = -1;
  
  // Search a free port context record
  
  for ( Vi=0; Vi < IAC__SOCK_MAX_SENDER_PORT_NB; Vi++ ) {
    
    if ( IAC__VGContext.Sock.ASenderPort[Vi].Used == 0 ) {
      IAC__VGContext.Sock.ASenderPort[Vi].Used = 1;
      VPortContextId                        = Vi;
      ++IAC__VGContext.Sock.SenderPortNb;
      break;
    }
    
  }
  
  // None found
  
  err_retfail ( VPortContextId, (ERR_OUT,"No free rec port context found => %d used", IAC__VGContext.Sock.SenderPortNb ) );
  
  // Found
  
  err_retval ( VPortContextId, ( ERR_OUT, "Get reception port context Id=%d", VPortContextId ) );
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 05/01/2008
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__PRIV_FSockGetReceiverPortContext () {
  
  SInt32 Vi;
  SInt32 VPortContextId;
  
  VPortContextId = -1;
  
  // Search a free port context record
  
  for ( Vi=0; Vi < IAC__SOCK_MAX_REC_PORT_NB; Vi++ ) {
    
    if ( IAC__VGContext.Sock.AReceiverPort[Vi].Used == 0 ) {
      IAC__VGContext.Sock.AReceiverPort[Vi].Used = 1;
      VPortContextId                        = Vi;
      ++IAC__VGContext.Sock.ReceiverPortNb;
      break;
    }
    
  }

  // None found
  
  err_retfail ( VPortContextId, (ERR_OUT,"No free rec port context found => %d used", IAC__VGContext.Sock.ReceiverPortNb ) );
  
  // Found
  
  err_retval ( VPortContextId, ( ERR_OUT, "Get reception port context Id=%d", VPortContextId ) );
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 09/11/2008
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__PRIV_FSockFreeSenderPortContext ( SInt32 ContextId ) {

  /* -------- */
  /* Check Id */
  /* -------- */
  
  err_retfail ( IAC__PRIV_FChkSenderContextId (ContextId), (ERR_OUT,"Bad Id=%d", ContextId ) );

  /* -------- */
  /* Free     */
  /* -------- */
  
  IAC__VGContext.Sock.ASenderPort[ContextId].Used = 0;
  
  if ( IAC__VGContext.Sock.SenderPortNb > 0 ) {
    --IAC__VGContext.Sock.SenderPortNb;
  }
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 05/01/2008
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__PRIV_FSockFreeReceiverPortContext ( SInt32 ContextId ) {

  /* -------- */
  /* Check Id */
  /* -------- */
  
  err_retfail ( IAC__PRIV_FChkReceiverContextId (ContextId), (ERR_OUT,"Bad Id=%d", ContextId ) );
  
  /* -------- */
  /* Free     */
  /* -------- */
  
  IAC__VGContext.Sock.AReceiverPort[ContextId].Used = 0;

  if ( IAC__VGContext.Sock.ReceiverPortNb > 0 ) {
    --IAC__VGContext.Sock.ReceiverPortNb;
  }

  err_retok (( ERR_OUT, "" ));
}




/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 09/11/2008
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__PRIV_FChkSenderContextId ( SInt32 ContextId ) {
  
  if ( (ContextId < 0) || (ContextId >= IAC__SOCK_MAX_SENDER_PORT_NB) ) {
    err_retfail ( -1, (ERR_OUT,"Context Id=%d out of range[0..%d] !)", ContextId, IAC__SOCK_MAX_SENDER_PORT_NB-1) );
  }
  
  if ( IAC__VGContext.Sock.ASenderPort[ContextId].Used == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Context record Id=%d IS NOT used !", ContextId ) );
  }
  
  return (0);
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 05/01/2008
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__PRIV_FChkReceiverContextId ( SInt32 ContextId ) {

  if ( (ContextId < 0) || (ContextId >= IAC__SOCK_MAX_REC_PORT_NB) ) {
    err_retfail ( -1, (ERR_OUT,"Context Id=%d out of range[0..%d] !)", ContextId, IAC__SOCK_MAX_REC_PORT_NB-1) );
  }

  if ( IAC__VGContext.Sock.AReceiverPort[ContextId].Used == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Context record Id=%d IS NOT used !", ContextId ) );
  }

  return (0);
}




/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    : The Index of Rec port context record
:
Globals   :
:
Remark    :
:
Level     :
Date      : 10/11/2008
Rev       : 11/11/2008
          : - New header, add trailer
          : 14/01/2009
          : - Detect lost of connection and try to reconnect by rebooting the sender
          : with a IAC__FSockRebootSenderPort call.
          : The goal is to allow receiver application to be stopped and restarted without
          : broking permanently the communication channel, but not to recover all kind of errors.
          :
          Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 IAC__PRIV_FSockSendShortMsg ( SInt32 ContextId ) {
  
  SInt32                 VRet;
  IAC__TSockSenderPort*  VPtSock;
  UInt32                 VTmpTotSz;
  UInt32                 VTotMsgSz;
  SInt32                 VSzSent;
  SInt32                 VSzRec;
  SInt32                 VW32ServerAns;
  SInt32                 VSockErrNo;
  
  
  VRet = -1;
    
  /* -------- */
  /* Check Id */
  /* -------- */
  
  err_retfail ( IAC__PRIV_FChkSenderContextId (ContextId), (ERR_OUT,"Bad Id=%d", ContextId ) );
  
  VPtSock = &(IAC__VGContext.Sock.ASenderPort[ContextId]);
  
  /* ------------ */
  /* Send message */
  /* ------------ */

  // Calculate message size
  
  VTotMsgSz = VPtSock->CstHeaderSz + VPtSock->ParMsgSz + VPtSock->CstTrailerSz;
  
  // Set size & CS in header
  
  VPtSock->MsgHeader.TotMsgSz = VTotMsgSz;
  
  if ( VPtSock->ParFuncCheckSum != NULL ) {
    VPtSock->MsgHeader.CheckSum = VPtSock->ParFuncCheckSum ( VPtSock->ParPtMsg, VPtSock->ParMsgSz );
  }
  
  else {
    VPtSock->MsgHeader.CheckSum = 0;
  }
  
  
        
  // Build messages
  
  memcpy (   VPtSock->PtBuffSendShortMsg                                    , &(VPtSock->MsgHeader) , VPtSock->CstHeaderSz  );
  memcpy ( &(VPtSock->PtBuffSendShortMsg[VPtSock->CstHeaderSz])             , VPtSock->ParPtMsg     , VPtSock->ParMsgSz     );
  memcpy ( &(VPtSock->PtBuffSendShortMsg[VTotMsgSz-(VPtSock->CstTrailerSz)]), &(VPtSock->MsgTrailer), VPtSock->CstTrailerSz );
  
  err_trace (( ERR_OUT, "Header.StartTag = %x [H]", VPtSock->MsgHeader.StartTag ));
  err_trace (( ERR_OUT, "Header.TotSz    = %d    ", VPtSock->MsgHeader.TotMsgSz ));
  err_trace (( ERR_OUT, "Header.CheckSum = %d    ", VPtSock->MsgHeader.CheckSum ));
  err_trace (( ERR_OUT, "Trailer.StopTag = %x [H]", VPtSock->MsgTrailer.StopTag ));
  
  // Send message
  
  VSzSent = send ( VPtSock->Socket, (char*) VPtSock->PtBuffSendShortMsg, VTotMsgSz, 0 );
  
  /* BEFORE 14/01/09 
  
  if ( VSzSent == SOCKET_ERROR ) {
    err_retfail ( -1, (ERR_OUT,"send () error %d - %s", IAC__SOCKET_ERRNO, strerror (IAC__SOCKET_ERRNO) ));
  }

  */
  
  

  if ( VSzSent == SOCKET_ERROR ) {
    
    VSockErrNo = IAC__SOCKET_ERRNO;
    
    // If connection reset => Try to connect and resend block
    
    if ( VSockErrNo == WSAECONNRESET ) {
      
      IAC__FSockRebootSenderPort ( ContextId );
      
      // Try again to send first block
      
      VSzSent = send( VPtSock->Socket, (char*) VPtSock->PtBuffSendShortMsg, VTotMsgSz, 0 );
      
      // Abort on send error
      
      if ( VSzSent == SOCKET_ERROR ) {
        err_retfail ( -1, (ERR_OUT,"send () error %d - Header => %s", VSockErrNo, strerror (VSockErrNo) ));
      }
      
    }
    
    // Others error => Abort
    
    else {
      err_retfail ( -1, (ERR_OUT,"send () error %d => %s", VSockErrNo, strerror (VSockErrNo) ));
    }
    
  }
  


  err_trace (( ERR_OUT, "Message of %d Bytes sent", VSzSent ));
  
  // Listen to Ack from destination host
  
  memset ( VPtSock->BuffRecAck, 0, IAC__SOCK_ACK_SZ );
  
  VSzRec = recv ( VPtSock->Socket, VPtSock->BuffRecAck, IAC__SOCK_ACK_SZ, 0 );
  
  // Ack reception error
  
  if ( VSzRec <= 0 ) {
    VRet = -1;
    err_error (( ERR_OUT, "Ack reception error %d - %s", IAC__SOCKET_ERRNO, strerror (IAC__SOCKET_ERRNO) ));
  }
  
  VW32ServerAns = *((UInt32*) VPtSock->BuffRecAck);
  
  // Check Ack
  
  if ( VW32ServerAns != VTotMsgSz ) {
    VRet = -1;
    err_error (( ERR_OUT, "Server ack bad size %d - should be %d", VW32ServerAns, VTotMsgSz ));
  }
  
  // OK : Message sent and Ack received
  
  VRet = VTotMsgSz;
  
  return (VRet);
}





/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 10/11/2008
Rev       : 11/11/2008
          : - New header & trailer
          : 14/01/2009
          : - Detect lost of connection and try to reconnect by rebooting the sender
          : with a IAC__FSockRebootSenderPort call.
          : The lost of connection test is only done at the beginning of a message ( on first
          : block send ), if it occurs the middle, therefore message sending is aborted.
          : This works like this because the goal is to allow receiver application to be
          : stopped and restarted without broking permanently the communication channel, but
          : not to recover all kind of errors.
          : 
          :
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 IAC__PRIV_FSockSendLongMsg ( SInt32 ContextId ) {
  
  IAC__TSockSenderPort* VPtSock;
  SInt32 VSzSent;
  SInt32 VSzRec;
  SInt32 VW32ServerAns;
  
  // UInt32 VTmpTotSz;
  UInt32 VTotMsgSz; // Including header and trailer
  
  UInt32 VHeaderBlockDataSz;
  UInt32 VHeaderBlockTotSz;
  UInt32 VDataBlocksTotSz;
  UInt32 VDataBlockSz;
  UInt32 VLastBlockSz;
  SInt32 VBlockNb;       // Excluding first one and last one
  SInt32 ViBlock;
  SInt32 VPosNextBlockInSrcBuff;
  SInt32 VTotSzSent;
  SInt8  VMsgSingleBlock;
  SInt32 VSockErrNo;
  
  
  /* -------- */
  /* Check Id */
  /* -------- */
  
  err_retfail ( IAC__PRIV_FChkSenderContextId (ContextId), (ERR_OUT,"Bad Id=%d", ContextId ) );
  
  VPtSock = &(IAC__VGContext.Sock.ASenderPort[ContextId]);
  
  /* ------------ */
  /* Send message */
  /* ------------ */
  
  // Evaluate blocks size
  
  
  err_trace (( ERR_OUT, "ContextId=%d - ParMsgSz=%d", ContextId, VPtSock->ParMsgSz ));
  
  VTotSzSent = 0;
  
  // Evaluate blocks size
  
  // In case of single block message, header and trailer are added to message
  
  if ( VPtSock->ParMsgSz <= (IAC__SO_MAX_SHORT_MSG_SIZE - (VPtSock->CstHeaderSz) - (VPtSock->CstTrailerSz)) )   {
    VMsgSingleBlock      = 1;
    VHeaderBlockDataSz   = VPtSock->ParMsgSz;
    VHeaderBlockTotSz    = VHeaderBlockDataSz + (VPtSock->CstHeaderSz) + (VPtSock->CstTrailerSz);
    VDataBlocksTotSz     = 0;
    VDataBlockSz         = 0;
    VBlockNb             = 0;
    VLastBlockSz         = 0; // 0 => no icomplete block at the end
  }
  
  // In case of multi block message, trailer is sent at the end by a specific call to send (...)
  // Therefore trailer size must not be taken into account for blocks size and number calculation
  
  else {
    VMsgSingleBlock      = 0;
    VHeaderBlockDataSz   = IAC__SO_MAX_SHORT_MSG_SIZE - (VPtSock->CstHeaderSz);
    VHeaderBlockTotSz    = IAC__SO_MAX_SHORT_MSG_SIZE;
    VDataBlocksTotSz     = VPtSock->ParMsgSz - VHeaderBlockDataSz;
    VDataBlockSz         = IAC__SO_MAX_SHORT_MSG_SIZE;
    VBlockNb             = VDataBlocksTotSz / VDataBlockSz;
    VLastBlockSz         = VDataBlocksTotSz % VDataBlockSz; // 0 => no icomplete block at the end
  }

  // Calculate message size
  
  VTotMsgSz = VPtSock->CstHeaderSz + VPtSock->ParMsgSz + VPtSock->CstTrailerSz;
  
  // Set size & CS in header
  
  VPtSock->MsgHeader.TotMsgSz = VTotMsgSz;
  
  if ( VPtSock->ParFuncCheckSum != NULL ) {
    VPtSock->MsgHeader.CheckSum = VPtSock->ParFuncCheckSum ( VPtSock->ParPtMsg, VPtSock->ParMsgSz );
  }
  
  else {
    VPtSock->MsgHeader.CheckSum = 0;
  }
    
  // Debug print
  
  err_trace (( ERR_OUT, "-----------------------------------------------------"  ));
  err_trace (( ERR_OUT, "User message size   = %d", VPtSock->ParMsgSz  ));
  err_trace (( ERR_OUT, "Total message size  = %d", VTotMsgSz          ));
  err_trace (( ERR_OUT, "VHeaderBlockDataSz  = %d", VHeaderBlockDataSz ));
  err_trace (( ERR_OUT, "VHeaderBlockTotSz   = %d", VHeaderBlockTotSz  ));
  err_trace (( ERR_OUT, "VDataBlocksTotSz    = %d", VDataBlocksTotSz   ));
  err_trace (( ERR_OUT, "VDataBlockSz        = %d", VDataBlockSz       ));
  err_trace (( ERR_OUT, "VBlockNb            = %d", VBlockNb           ));
  err_trace (( ERR_OUT, "VLastBlockSz        = %d", VLastBlockSz       ));
  err_trace (( ERR_OUT, "-----------------------------------------------------"  ));
    
  // Build header block

  memcpy (   VPtSock->PtBuffSendShortMsg                                    , &(VPtSock->MsgHeader) , VPtSock->CstHeaderSz  );
  memcpy ( &(VPtSock->PtBuffSendShortMsg[VPtSock->CstHeaderSz])             , VPtSock->ParPtMsg     , VHeaderBlockDataSz    );
  
  // Copy trailer ONLY for single block message
  
  if ( VMsgSingleBlock == 1 ) {
    memcpy ( &(VPtSock->PtBuffSendShortMsg[VTotMsgSz-(VPtSock->CstTrailerSz)]), &(VPtSock->MsgTrailer), VPtSock->CstTrailerSz );
  }
  
  VPosNextBlockInSrcBuff = VHeaderBlockDataSz;
  
  // Send first block
  
  VSzSent = send( VPtSock->Socket, (char*) VPtSock->PtBuffSendShortMsg, VHeaderBlockTotSz, 0 );
  
  if ( VSzSent == SOCKET_ERROR ) {
    
    VSockErrNo = IAC__SOCKET_ERRNO;
    
    // If connection reset => Try to connect and resend block
    
    if ( VSockErrNo == WSAECONNRESET ) {
      
      IAC__FSockRebootSenderPort ( ContextId );
            
      // Try again to send first block
  
      VSzSent = send( VPtSock->Socket, (char*) VPtSock->PtBuffSendShortMsg, VHeaderBlockTotSz, 0 );
  
      // Abort on send error
  
      if ( VSzSent == SOCKET_ERROR ) {
        err_retfail ( -1, (ERR_OUT,"send () error %d - Header => %s", VSockErrNo, strerror (VSockErrNo) ));
      }
      
    }
    
    // Others error => Abort
    
    else {
      err_retfail ( -1, (ERR_OUT,"send () error %d - Header => %s", VSockErrNo, strerror (VSockErrNo) ));    
    }
    
  }
  
  else {
    err_trace (( ERR_OUT, "Header block of %d Bytes sent", VSzSent ));
  }
  
  VTotSzSent += VSzSent;
  
  // Send complete blocks
  
  for ( ViBlock=0; ViBlock < VBlockNb; ViBlock++) {
    
    VSzSent = send ( VPtSock->Socket, &(VPtSock->ParPtMsg[VPosNextBlockInSrcBuff]), VDataBlockSz, 0 );
    
    if ( VSzSent == SOCKET_ERROR ) {
      err_retfail ( -1, (ERR_OUT,"send () error %d - Block[%d] => %s", IAC__SOCKET_ERRNO, ViBlock, strerror (IAC__SOCKET_ERRNO) ));
    }
    
    err_trace (( ERR_OUT, "Send block[%d] of %d bytes OK :-)", ViBlock, VSzSent ));
    
    VPosNextBlockInSrcBuff += VDataBlockSz;
    VTotSzSent += VSzSent;
    
    err_trace (( ERR_OUT, "Block[%d] of %d Bytes sent", ViBlock, VSzSent ));
    
  }
  
  // Send last block
  
  if ( VLastBlockSz > 0 ) {
    VSzSent = send ( VPtSock->Socket, &(VPtSock->ParPtMsg[VPosNextBlockInSrcBuff]), VLastBlockSz, 0 );
    
    if ( VSzSent == SOCKET_ERROR ) {
      err_retfail ( -1, (ERR_OUT,"send () error %d - Last block => %s", IAC__SOCKET_ERRNO, strerror (IAC__SOCKET_ERRNO) ));
    }
    
    err_trace (( ERR_OUT, "Send last block of %d bytes OK :-)", VSzSent ));
    
    VTotSzSent += VSzSent;
  }
  
  // Send trailer ONLY for multiple blocks message

  if ( VMsgSingleBlock == 0 ) {
    VSzSent = send(  VPtSock->Socket, (UInt8*) &(VPtSock->MsgTrailer), VPtSock->CstTrailerSz, 0 );
  
    if ( VSzSent == SOCKET_ERROR ) {
      err_retfail ( -1, (ERR_OUT,"send () error %d - Trailer => %s", IAC__SOCKET_ERRNO, strerror (IAC__SOCKET_ERRNO) ));
    }
  
    err_trace (( ERR_OUT, "Send last block of %d bytes OK :-)", VSzSent ));
  
    VTotSzSent += VSzSent;  
  }
  
  err_trace (( ERR_OUT, "Before waiting for host ACK" ));
  
  // Listen to Ack from destination host
  
  memset ( VPtSock->BuffRecAck, 0, IAC__SOCK_ACK_SZ );
  
  VSzRec = recv ( VPtSock->Socket, VPtSock->BuffRecAck, IAC__SOCK_ACK_SZ, 0 );
  
  // Ack reception error
  
  if ( VSzRec <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Ack receive error %d -  %s", IAC__SOCKET_ERRNO, strerror (IAC__SOCKET_ERRNO) ));
  }
  
  VW32ServerAns = *((UInt32*) (VPtSock->BuffRecAck) );
  
  // Check Ack
  
  if ( VW32ServerAns != VTotSzSent ) {
    err_retfail ( -1, (ERR_OUT,"Server ack bad size %d - should be %d", VW32ServerAns, VTotSzSent ));
  }
  
  err_trace (( ERR_OUT, "Message sent and Ack received" ));
  
  // OK : Message sent and Ack received
  
  return (VTotSzSent);
}




/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    : The Index of Rec port context record
:
Globals   :
:
Remark    :
:
Level     :
Date      : 09/11/2008
Rev       : 11/11/2008
          : - New header & trailer
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__FSockInstallSenderPort ( char* DestHost, UInt32 DestPort, SInt8 LongMsg, UInt32 AckTimeOutMs, UInt8 RetryNb, IAC__TFSockFuncCheckSum FuncCheckSum ) {
  
  SInt32                VSenderPortContextId;
  IAC__TSockSenderPort* VPtSocket;
  
  /* --------------------------------- */
  /* Find a free sender context record */
  /* --------------------------------- */
  
  VSenderPortContextId = IAC__PRIV_FSockGetSenderPortContext ();
  
  err_retfail ( VSenderPortContextId, (ERR_OUT,"No free context record !") );
  
  VPtSocket       = &(IAC__VGContext.Sock.ASenderPort[VSenderPortContextId]);
  VPtSocket->Id   = VSenderPortContextId;

  /* --------------------- */
  /* Init constants fields */
  /* --------------------- */

  VPtSocket->CstHeaderSz        = sizeof (IAC__TSockMsgHeader );
  VPtSocket->CstTrailerSz       = sizeof (IAC__TSockMsgTrailer);
    
  VPtSocket->MsgHeader.StartTag = IAC__SOCK_HEADER_START_TAG;
  VPtSocket->MsgHeader.Zero     = 0;
  VPtSocket->MsgHeader.TotMsgSz = 0; // Unknown at this step
  VPtSocket->MsgHeader.CheckSum = 0; // Unknown at this step
  
  VPtSocket->MsgTrailer.StopTag = IAC__SOCK_TRAILER_STOP_TAG;
  VPtSocket->MsgTrailer.Zero    = 0;
  

  /* --------------- */
  /* Init parameters */
  /* --------------- */
  
  strncpy ( VPtSocket->ParDestHost, DestHost, IAC__SOCK_MAX_STR_HOST_SZ - 1 );
  VPtSocket->ParDestPort     = DestPort;
  VPtSocket->ParLongMsg      = LongMsg;
  VPtSocket->ParAckTimeOutMs = AckTimeOutMs;
  VPtSocket->ParRetryNb      = RetryNb;
  VPtSocket->ParPtMsg        = NULL;
  VPtSocket->ParFuncCheckSum = FuncCheckSum;
  
  /* --------------- */
  /* Create socket   */
  /* --------------- */
  
  // Create
  
  VPtSocket->Socket = socket ( PF_INET, SOCK_STREAM, 0 );
  
  if ( VPtSocket->Socket == INVALID_SOCKET ) {
    err_retfail ( -1, (ERR_OUT,"socket () error %d => %s", IAC__SOCKET_ERRNO, strerror (IAC__SOCKET_ERRNO) ));
  }
  
  // Bind 
  
  VPtSocket->LocAddr.sin_family       = AF_INET ;
  VPtSocket->LocAddr.sin_addr.s_addr  = htonl (INADDR_ANY);
  VPtSocket->LocAddr.sin_port         = htons ((unsigned short)0 );
  
  if ( bind( VPtSocket->Socket, (struct sockaddr *)&(VPtSocket->LocAddr), sizeof(VPtSocket->LocAddr)) == SOCKET_ERROR ) {
    err_retfail ( -1, (ERR_OUT,"bind () error %d => %s", IAC__SOCKET_ERRNO, strerror (IAC__SOCKET_ERRNO) ));
  }
  
  // Transform the host/port into a struct sockaddr
    
  VPtSocket->DestAddr = IAC__PRIV_FTcpFormatAdress ( VPtSocket->ParDestHost, (u_short) (VPtSocket->ParDestPort) );
  
  /* -------------- */
  /*  Create events */
  /* -------------- */
  
  VPtSocket->EvNewMsgHnd = CreateEvent (
    NULL    /*  LPSECURITY_ATTRIBUTES */,
    FALSE   /* Manual Reset */,
    FALSE   /* Initial State */,
    NULL    /* Name STring */ );

  err_retnull ( VPtSocket->EvNewMsgHnd, (ERR_OUT,"CreateEvent EvNewMsgHnd failed !  - LastError=%d", GetLastError ()) );

  VPtSocket->EvAckMsgHnd = CreateEvent (
    NULL    /*  LPSECURITY_ATTRIBUTES */,
    FALSE   /* Manual Reset */,
    FALSE   /* Initial State */,
    NULL    /* Name STring */ );
  
  err_retnull ( VPtSocket->EvAckMsgHnd, (ERR_OUT,"CreateEvent EvAckMsgHnd failed !  - LastError=%d", GetLastError ()) );
  
  
  /* ------------- */
  /* Alloc buffers */
  /* ------------- */  
  
  VPtSocket->PtBuffSendShortMsg = (UInt8*) malloc ( IAC__SO_MAX_SHORT_MSG_SIZE );
  
  err_retnull ( VPtSocket->PtBuffSendShortMsg, (ERR_OUT,"Allocation sender buffer of %d bytes failed !", IAC__SO_MAX_SHORT_MSG_SIZE) );
  
  /* ------------- */
  /* Create thread */
  /* ------------- */
  
  // It will be done in IAC__FSockStartSenderPort ()
  
  /* ------------- */
  /* Set status    */
  /* ------------- */
  
  VPtSocket->Installed = 1;

  /* ------------- */
  /* Debug print   */
  /* ------------- */
  
  err_trace (( ERR_OUT, "----------------------------------------------------" ));
  err_trace (( ERR_OUT, "Sender port [%d] installed", VPtSocket->Id   ));
  err_trace (( ERR_OUT, "Dest host = %s", VPtSocket->ParDestHost     ));
  err_trace (( ERR_OUT, "Dest port = %d", VPtSocket->ParDestPort     ));
  err_trace (( ERR_OUT, "Ack TO    = %d", VPtSocket->ParAckTimeOutMs ));
  err_trace (( ERR_OUT, "----------------------------------------------------" ));
  
  err_retval ( VSenderPortContextId, ( ERR_OUT, "Emission port=%d installed - uses context record=%d", VPtSocket->ParDestPort, VSenderPortContextId ) );
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    : The Index of Rec port context record
:
Globals   :
:
Remark    :
:
Level     :
Date      : 14/01/2009
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__FSockRebootSenderPort ( SInt32 ContextId ) {
  
  IAC__TSockSenderPort* VPtSocket;
  
  /* -------- */
  /* Check ID */
  /* -------- */
  
  err_retfail ( IAC__PRIV_FChkSenderContextId (ContextId), (ERR_OUT,"Bad Id=%d", ContextId ) );
  
  VPtSocket = &IAC__VGContext.Sock.ASenderPort[ContextId];
  
  /* ------------ */
  /* Check status */
  /* ------------ *
  
  if ( VPtSocket->Installed != 1 ) {
    err_retfail ( -1, (ERR_OUT,"Access not installed => Can't be started" ) );
  }
    
  
  /* ------------ */
  /* Reset status */
  /* ------------ */
  
  VPtSocket->Started = 0;
  
  
  /* ------------ */
  /* Close Socket */
  /* ------------ */
  
  closesocket ( VPtSocket->Socket );
  
  
  /* --------------- */
  /* Create socket   */
  /* --------------- */
  
  // Create
  
  VPtSocket->Socket = socket ( PF_INET, SOCK_STREAM, 0 );
  
  if ( VPtSocket->Socket == INVALID_SOCKET ) {
    err_retfail ( -1, (ERR_OUT,"socket () error %d => %s", IAC__SOCKET_ERRNO, strerror (IAC__SOCKET_ERRNO) ));
  }
  
  // Bind
  
  VPtSocket->LocAddr.sin_family       = AF_INET ;
  VPtSocket->LocAddr.sin_addr.s_addr  = htonl (INADDR_ANY);
  VPtSocket->LocAddr.sin_port         = htons ((unsigned short)0 );
  
  if ( bind( VPtSocket->Socket, (struct sockaddr *)&(VPtSocket->LocAddr), sizeof(VPtSocket->LocAddr)) == SOCKET_ERROR ) {
    err_retfail ( -1, (ERR_OUT,"bind () error %d => %s", IAC__SOCKET_ERRNO, strerror (IAC__SOCKET_ERRNO) ));
  }
  
  // Transform the host/port into a struct sockaddr
  
  VPtSocket->DestAddr = IAC__PRIV_FTcpFormatAdress ( VPtSocket->ParDestHost, (u_short) (VPtSocket->ParDestPort) );

  // Connect
  
  while ( connect( VPtSocket->Socket, &(VPtSocket->DestAddr), sizeof(VPtSocket->DestAddr) ) == SOCKET_ERROR ) {
    err_error (( ERR_OUT, "Connect () error %d => %s WILL RETRY in 100 ms", IAC__SOCKET_ERRNO, strerror (IAC__SOCKET_ERRNO) ));
    Sleep ( 100 );
  }
  

  
  /* ---------- */
  /* Set status */
  /* ---------- */
  
  VPtSocket->Started = 1;
  
  err_trace (( ERR_OUT, "----------------------------------------------------" ));
  err_trace (( ERR_OUT, "Sender port [%d] started", VPtSocket->Id   ));
  err_trace (( ERR_OUT, "----------------------------------------------------" ));
  
  return (0);
  
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 09/11/2008
Rev       : 13/01/2009
          : - Add closesocket call
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__FSockUnInstallSenderPort ( SInt32 SenderPortContextId ) {
  
  IAC__TSockSenderPort* VPtSocket;
  
  err_retfail ( IAC__PRIV_FChkSenderContextId (SenderPortContextId), (ERR_OUT,"Bad Id=%d", SenderPortContextId ) );

  VPtSocket = &(IAC__VGContext.Sock.ASenderPort[SenderPortContextId]);

  /* ------------- */
  /* Set status    */
  /* ------------- */
  
  VPtSocket->Installed = 0;
  
  /* ------------- */
  /* Close events  */
  /* ------------- */
  
  if ( CloseHandle ( VPtSocket->EvNewMsgHnd ) == 0 ) {
    err_error (( ERR_OUT, "CloseHandle ( VPtSocket->EvNewMsgHnd ) failed !" ));
  }
  
  if ( CloseHandle ( VPtSocket->EvAckMsgHnd ) == 0 ) {
    err_error (( ERR_OUT, "CloseHandle ( VPtSocket->EvAckMsgHnd ) failed !" ));
  }

  /* ------------ */
  /* Close Socket */
  /* ------------ */
  
  closesocket ( VPtSocket->Socket );
    
  /* ------------ */
  /* Free buffers */
  /* ------------ */
  
  if ( VPtSocket->PtBuffSendShortMsg != NULL ) {
    free ( VPtSocket->PtBuffSendShortMsg );
  }
  
  else {
    err_error (( ERR_OUT, "Try to free PtBuffSendShortMsg, but it's already NULL => ???" ));
  }
  
  
  /* --------------- */
  /* Release context */
  /* --------------- */
  
  IAC__PRIV_FSockFreeSenderPortContext ( SenderPortContextId );
  
  err_retok (( ERR_OUT, "" ));
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 09/11/2008
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 IAC__FSockStartSenderPort ( SInt32 ContextId ) {
  
  IAC__TSockSenderPort* VPtSocket;
  
  /* -------- */
  /* Check ID */
  /* -------- */
  
  err_retfail ( IAC__PRIV_FChkSenderContextId (ContextId), (ERR_OUT,"Bad Id=%d", ContextId ) );
  
  VPtSocket = &IAC__VGContext.Sock.ASenderPort[ContextId];
  
  /* ------------ */
  /* Check status */
  /* ------------ *
  
  if ( VPtSocket->Installed != 1 ) {
    err_retfail ( -1, (ERR_OUT,"Access not installed => Can't be started" ) );
  }
      
  /* --------------------- */
  /* Create & Start thread */
  /* --------------------- */
  
  VPtSocket->ThreadHnd  = CreateThread( NULL, NULL, IAC__PRIV_FSockSenderThreadFunc, (LPVOID) VPtSocket, NULL, &(VPtSocket->ThreadId) );
  
  err_retnull ( VPtSocket->ThreadHnd, (ERR_OUT,"Thread creation failed !") );
  
  VPtSocket->ThreadFunc = IAC__PRIV_FSockSenderThreadFunc;
  

  /* ---------- */
  /* Set status */
  /* ---------- */
  
  VPtSocket->Started = 1;
  
  
  err_trace (( ERR_OUT, "----------------------------------------------------" ));
  err_trace (( ERR_OUT, "Sender port [%d] started", VPtSocket->Id   ));
  err_trace (( ERR_OUT, "----------------------------------------------------" ));
  
  return (0);
  
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 09/11/2008
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 IAC__FSockStopSenderPort ( SInt32 ContextId ) {
  
  IAC__TSockSenderPort* VPtSock;
  
  /* -------- */
  /* Check ID */
  /* -------- */
  
  err_retfail ( IAC__PRIV_FChkSenderContextId (ContextId), (ERR_OUT,"Bad Id=%d", ContextId ) );
  
  VPtSock = &IAC__VGContext.Sock.ASenderPort[ContextId];
  
  err_retnull ( VPtSock->ThreadHnd, (ERR_OUT,"Thread handle == NULL !") );

  /* ---------------- */
  /* Terminate thread */
  /* ---------------- */
  
  TerminateThread ( VPtSock->ThreadHnd, -1 /* Thread exit code */ );

  /* ---------- */
  /* Set status */
  /* ---------- */
  
  VPtSock->Started = 0;    
  
  err_trace   (( ERR_OUT, "TerminateThread sent - with exit code = -1" ));
  
  err_trace (( ERR_OUT, "----------------------------------------------------" ));
  err_trace (( ERR_OUT, "Sender port [%] terminated", VPtSock->Id   ));
  err_trace (( ERR_OUT, "----------------------------------------------------" ));
  
  return (0);
      
}





/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    : The Index of Rec port context record
:
Globals   :
:
Remark    :
:
Level     :
Date      : 05/01/2008
Rev       : 06/11/2008
          : - Add LongMsg Handling
          : 10/11/2008
          : - Rewritten => No socket close after each message
          : 11/11/2008
          : - New header & trailer
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__FSockInstallReceiverPort ( UInt32 RecPort, SInt8 LongMsg, IAC__TFSockUsrHandleRecMsg FuncUsr, IAC__TFSockFuncCheckSum FuncCheckSum ) {
  
  SInt32                  VRecPortContextId;
  IAC__TSockReceiverPort* VPtSocket;
  UInt8*                  VPtRecBuffer;

  /* --------------------------------- */
  /* Find a free sender context record */
  /* --------------------------------- */
  
  VRecPortContextId = IAC__PRIV_FSockGetReceiverPortContext ();
  
  err_retfail ( VRecPortContextId, (ERR_OUT,"No free context record !") );
  
  VPtSocket       = &(IAC__VGContext.Sock.AReceiverPort[VRecPortContextId]);
  VPtSocket->Id   = VRecPortContextId;

  /* --------------------- */
  /* Init constants fields */
  /* --------------------- */
  
  VPtSocket->CstHeaderSz        = sizeof (IAC__TSockMsgHeader );
  VPtSocket->CstTrailerSz       = sizeof (IAC__TSockMsgTrailer);
  
  VPtSocket->MsgHeader.StartTag = IAC__SOCK_HEADER_START_TAG;
  VPtSocket->MsgHeader.Zero     = 0;
  VPtSocket->MsgHeader.TotMsgSz = 0; // Unknown at this step
  VPtSocket->MsgHeader.CheckSum = 0; // Unknown at this step
  
  VPtSocket->MsgTrailer.StopTag = IAC__SOCK_TRAILER_STOP_TAG;
  VPtSocket->MsgTrailer.Zero    = 0;
  
  
  /* --------------- */
  /* Init parameters */
  /* --------------- */
  
  
  VPtSocket->Id              = VRecPortContextId;
  VPtSocket->ParLongMsg      = LongMsg;
  VPtSocket->ParPort         = RecPort;
  VPtSocket->ParUsrFunc      = FuncUsr;
  VPtSocket->ParFuncCheckSum = FuncCheckSum;


  /* ------------------------- */
  /* Allocate reception buffer */
  /* ------------------------- */
  

  if ( VPtSocket->ParLongMsg == 0 ) {
    VPtSocket->MaxMsgSz = IAC__SO_MAX_SHORT_MSG_SIZE * 2;
  }
  
  else {
    VPtSocket->MaxMsgSz = IAC__SOCK_MAX_LONG_MSG_SZ;
  }
  
  VPtRecBuffer = (UInt8*) malloc ( VPtSocket->MaxMsgSz );
  
  err_retnull ( VPtRecBuffer, (ERR_OUT,"Malloc of %d bytes for rec buffer failed !", VPtSocket->MaxMsgSz) );
  
  err_trace (( ERR_OUT, "Malloc of %d bytes for Receiver port[%d]", VPtSocket->MaxMsgSz, VRecPortContextId ));
  
  VPtSocket->PtMsg = VPtRecBuffer;

  /* --------------- */
  /* Create socket   */
  /* --------------- */
  
  // Create
  
  VPtSocket->LSocket = socket ( PF_INET, SOCK_STREAM, 0 );
  
  if ( VPtSocket->LSocket == INVALID_SOCKET ) {
    err_retfail ( -1, (ERR_OUT,"socket () error %d => %s", IAC__SOCKET_ERRNO, strerror (IAC__SOCKET_ERRNO) ));
  }
  
  // Bind
  
  VPtSocket->LocAddr.sin_family       = AF_INET ;
  VPtSocket->LocAddr.sin_addr.s_addr  = htonl (INADDR_ANY);
  VPtSocket->LocAddr.sin_port         = htons ((unsigned short) VPtSocket->ParPort );
  
  if ( bind( VPtSocket->LSocket, (struct sockaddr *)&(VPtSocket->LocAddr), sizeof(VPtSocket->LocAddr)) == SOCKET_ERROR ) {
    err_retfail ( -1, (ERR_OUT,"bind () error %d => %s", IAC__SOCKET_ERRNO, strerror (IAC__SOCKET_ERRNO) ));
  }
  
  /* ---------- */
  /* Set status */  
  /* ---------- */
  
  VPtSocket->Installed = 1;
  
  
  /* ------------- */
  /* Debug print   */
  /* ------------- */
  
  err_trace (( ERR_OUT, "----------------------------------------------------" ));
  err_trace (( ERR_OUT, "Receiver port [%d] installed", VPtSocket->Id   ));
  err_trace (( ERR_OUT, "Port     = %d", VPtSocket->ParPort     ));
  err_trace (( ERR_OUT, "Long msg = %d", VPtSocket->ParLongMsg ));
  err_trace (( ERR_OUT, "----------------------------------------------------" ));
  
  err_retval ( VRecPortContextId, ( ERR_OUT, "Reception port=%d installed - uses context record=%d", RecPort, VRecPortContextId ) );
}




/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    : The Index of Rec port context record
:
Globals   :
:
Remark    :
:
Level     :
Date      : 13/01/2009
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__FSockRebootReceiverPort ( SInt32 ContextId ) {

  IAC__TSockReceiverPort* VPtSock;
  
  /* -------- */
  /* Check ID */
  /* -------- */
  
  err_retfail ( IAC__PRIV_FChkReceiverContextId (ContextId), (ERR_OUT,"Bad Id=%d", ContextId ) );
  
  VPtSock = &IAC__VGContext.Sock.AReceiverPort[ContextId];

  /* ------------ */
  /* Reset status */
  /* ------------ */
  
  VPtSock->Installed = 0;
  
  /* ------------- */
  /* Close ASocket */
  /* ------------- */
  
  closesocket ( VPtSock->ASocket );
    
  /* ------------- */
  /* Close LSocket */
  /* ------------- */
  
  closesocket ( VPtSock->LSocket );
  

  /* --------------- */
  /* Create socket   */
  /* --------------- */
  
  // Create
  
  VPtSock->LSocket = socket ( PF_INET, SOCK_STREAM, 0 );
  
  if ( VPtSock->LSocket == INVALID_SOCKET ) {
    err_retfail ( -1, (ERR_OUT,"socket () error %d => %s", IAC__SOCKET_ERRNO, strerror (IAC__SOCKET_ERRNO) ));
  }
  
  // Bind
  
  VPtSock->LocAddr.sin_family       = AF_INET ;
  VPtSock->LocAddr.sin_addr.s_addr  = htonl (INADDR_ANY);
  VPtSock->LocAddr.sin_port         = htons ((unsigned short) VPtSock->ParPort );
  
  if ( bind( VPtSock->LSocket, (struct sockaddr *)&(VPtSock->LocAddr), sizeof(VPtSock->LocAddr)) == SOCKET_ERROR ) {
    err_retfail ( -1, (ERR_OUT,"bind () error %d => %s", IAC__SOCKET_ERRNO, strerror (IAC__SOCKET_ERRNO) ));
  }
  
  /* ---------- */
  /* Set status */
  /* ---------- */
  
  VPtSock->Installed = 1;


  /* ---------------------- */
  /* Wait client connection */
  /* ---------------------- */
  
  // Listen
  
  err_trace (( ERR_OUT, "Before listen" ));
  
  if ( listen( VPtSock->LSocket, 2) == SOCKET_ERROR ) {
    err_retfail ( -1, (ERR_OUT,"listen () error %d => %s", IAC__SOCKET_ERRNO, strerror (IAC__SOCKET_ERRNO) ));
  }
  
  err_trace (( ERR_OUT, "Before accept" ));
  
  // Accept
  
  VPtSock->ASocket = accept ( VPtSock->LSocket, NULL, NULL );
  
  if( VPtSock->ASocket == INVALID_SOCKET )  {
    err_retfail ( -1, (ERR_OUT,"accept () error %d => %s", IAC__SOCKET_ERRNO, strerror (IAC__SOCKET_ERRNO) ));
  }
  

  
  /* ------------- */
  /* Debug print   */
  /* ------------- */
  
  err_trace (( ERR_OUT, "----------------------------------------------------" ));
  err_trace (( ERR_OUT, "Reboot of Receiver port [%d] done", VPtSock->Id   ));
  err_trace (( ERR_OUT, "Port     = %d", VPtSock->ParPort     ));
  err_trace (( ERR_OUT, "Long msg = %d", VPtSock->ParLongMsg ));
  err_trace (( ERR_OUT, "----------------------------------------------------" ));
  
  err_retval ( ContextId, ( ERR_OUT, "Reception port Id=%d reboot", ContextId ) );
}




/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    : 
:
Globals   :
:
Remark    :
:
Level     :
Date      : 05/01/2008
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__FSockUnInstallReceiverPort ( SInt32 RecPortContextId ) {

  IAC__TSockReceiverPort* VPtSock;
  
  /* -------- */
  /* Check ID */
  /* -------- */
  
  err_retfail ( IAC__PRIV_FChkReceiverContextId (RecPortContextId), (ERR_OUT,"Bad Id=%d", RecPortContextId ) );
  VPtSock = &IAC__VGContext.Sock.AReceiverPort[RecPortContextId ];

  /* ------------- */
  /* Set status    */
  /* ------------- */
  
  VPtSock->Installed = 0;
  VPtSock->Started   = 0;
  
  /* ------------- */
  /* Close LSocket */
  /* ------------- */
  
  closesocket ( VPtSock->LSocket );
  
  /* ------------- */
  /* Free buffer   */
  /* ------------- */
  
  if ( VPtSock->PtMsg != NULL ) {
    free ( VPtSock->PtMsg );
  }
  
  VPtSock->MaxMsgSz = 0;

  IAC__PRIV_FSockFreeReceiverPortContext ( RecPortContextId );

  err_retok (( ERR_OUT, "" ));
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 05/01/2008
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 IAC__FSockStartReceiverPort ( SInt32 ContextId ) {

  IAC__TSockReceiverPort* VPtSock;

  /* -------- */
  /* Check ID */
  /* -------- */
  
  err_retfail ( IAC__PRIV_FChkReceiverContextId (ContextId), (ERR_OUT,"Bad Id=%d", ContextId ) );
  
  VPtSock = &IAC__VGContext.Sock.AReceiverPort[ContextId];

  /* --------------------- */
  /* Start thread function */
  /* --------------------- */
  
  err_trace (( ERR_OUT, "Call CreateThread (...)" ));
  
  VPtSock->ThreadHnd  = CreateThread( NULL, NULL, IAC__PRIV_FSockReceiverThreadFunc, (LPVOID) VPtSock, NULL, &(VPtSock->ThreadId) );
  
  err_retnull ( VPtSock->ThreadHnd, (ERR_OUT,"Thread creation failed !") );
  
  VPtSock->ThreadFunc = IAC__PRIV_FSockReceiverThreadFunc;
  

  /* ---------- */
  /* Set status */
  /* ---------- */
  
  VPtSock->Started = 1;
  
  return (0);  
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 05/01/2008
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 IAC__FSockStopReceiverPort ( SInt32 ContextId ) {
  
  SInt8                   VThreadKillOk;
  IAC__TSockReceiverPort* VPtSock;
  
  /* -------- */
  /* Check ID */
  /* -------- */
  
  err_retfail ( IAC__PRIV_FChkReceiverContextId (ContextId), (ERR_OUT,"Bad Id=%d", ContextId ) );
  
  VPtSock = &IAC__VGContext.Sock.AReceiverPort[ContextId];

  /* ---------- */
  /* Set status */
  /* ---------- */
  
  VPtSock->Started = 0;
  
  /* -------------------- */
  /* Kill thread function */
  /* -------------------- */
  
  err_retnull ( VPtSock->ThreadHnd, (ERR_OUT,"Thread handle == NULL !") );
  
  VThreadKillOk = TerminateThread ( VPtSock->ThreadHnd, -1 /* Thread exit code */ );
  
  err_trace   (( ERR_OUT, "TerminateThread sent - Return Ok ? = %d", VThreadKillOk ));

  /* ------------- */
  /* Close ASocket */
  /* ------------- */
  
  closesocket ( VPtSock->ASocket );

  err_retok (( ERR_OUT, "Receiver port Id=%d stopped", ContextId ));
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      : For debugging only => Disable call to IAC__PRIV_FSockLookForShortRecMsgOnPortAndProcessIt
          : in thread function
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 10/01/2008
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 IAC__FSockDbgReceiverPortDisableReception ( SInt32 ContextId, SInt8 DisableReception ) {
  
  IAC__TSockReceiverPort* VPtSock;
  
  /* -------- */
  /* Check ID */
  /* -------- */
  
  err_retfail ( IAC__PRIV_FChkReceiverContextId (ContextId), (ERR_OUT,"Bad Id=%d", ContextId ) );
  
  VPtSock = &IAC__VGContext.Sock.AReceiverPort[ContextId];
  
  /* ------------- */
  /* Set parameter */
  /* ------------- */
    
  VPtSock->DbgDisableReception = DisableReception;
    
  return (0);
}




/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 10/11/2008
Rev       : 11/11/2008
          : - New header, add trailer
          : 13/01/2009
          : - Return -2 if recv failed on connection lost in order to infrom caller to
          : " reboot " the receiver
          :
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 IAC__PRIV_FSockLookForShortRecMsgOnPortAndProcessIt ( UInt32 RecPortContextId ) {
  
  IAC__TSockReceiverPort* VPtSock;
  SInt8                   VMsgReceptionRuning;
  SInt32                  VRecSz;
  SInt32                  VAckSentSz;
  IAC__TSockMsgHeader*    VPtHeader;
  IAC__TSockMsgTrailer*   VPtTrailer;
  SInt32                  VUserDataSz;
  SInt32                  VCheckSum;
  SInt32                  VSockErrno;
  
  
  VPtSock = &(IAC__VGContext.Sock.AReceiverPort[RecPortContextId]);
  
  
  while (1) {
  
  
    // Init status
    
    VPtSock->MsgReady = 0;
    
    // Read
    
    VRecSz = recv ( VPtSock->ASocket, &(VPtSock->PtMsg[0]), VPtSock->MaxMsgSz, 0 );
    
    err_trace (( ERR_OUT, "RecSz=%d", VRecSz ));
    
    if( VRecSz <= 0 ) {
      
      VSockErrno = IAC__SOCKET_ERRNO;
      
      err_error (( ERR_OUT, "VRecSz=%d - IAC__SOCKET_ERRNO=%d", VRecSz, VSockErrno ));
      
      /* --------------------------------------------------------------------------------- */
      /* If connection has been lost                                                       */
      /*   => return -2                                                                    */
      /*   => caller will detected this special error and will try to reboot the receiver  */
      /* --------------------------------------------------------------------------------- */
      
      if ( (VRecSz == 0) || (VSockErrno == WSAECONNRESET)  ) {
        err_retfail ( -2, (ERR_OUT,"Connection lost ! VRecSz=%d - IAC__SOCKET_ERRNO=%d => %s", VRecSz, VSockErrno, strerror (VSockErrno) ));
      }
      
      /* ------------------------------- */
      /* Normal error => Not recoverable */
      /* ------------------------------- */
      
      else {
        err_retfail ( -1, (ERR_OUT,"recv () error %d => %s", VSockErrno, strerror (VSockErrno) ));
      }
      
    }
    
    // Check header & trailer & CS
    
    VPtHeader = (IAC__TSockMsgHeader*) VPtSock->PtMsg;
    
    if ( VPtHeader->StartTag != IAC__SOCK_HEADER_START_TAG ) {
      err_retfail ( -1, (ERR_OUT,"Bad header start tag=%x [H] - Must be %x [H]", VPtHeader->StartTag, IAC__SOCK_HEADER_START_TAG ) );
    }
    
    if ( VPtHeader->TotMsgSz != VRecSz ) {
      err_retfail ( -1, (ERR_OUT,"Header tot sz=%d <> Received sz=%d", VPtHeader->TotMsgSz, VRecSz ) );
    }
    
    VPtTrailer = (IAC__TSockMsgTrailer*) &(VPtSock->PtMsg[VRecSz-(VPtSock->CstTrailerSz)]);
    
    if ( VPtTrailer->StopTag != IAC__SOCK_TRAILER_STOP_TAG ) {
      err_retfail ( -1, (ERR_OUT,"Bad trailer stop tag=%x [H] - Must be %x [H]", VPtTrailer->StopTag, IAC__SOCK_TRAILER_STOP_TAG ) );
    }
    
    VUserDataSz = VRecSz - (VPtSock->CstHeaderSz) - (VPtSock->CstTrailerSz);
    
    if ( VPtSock->ParFuncCheckSum != NULL ) {
      VCheckSum = VPtSock->ParFuncCheckSum ( &(VPtSock->PtMsg[VPtSock->CstHeaderSz]), VUserDataSz );
      
      if ( VCheckSum != VPtHeader->CheckSum ) {
        err_retfail ( -1, (ERR_OUT,"Checksum error : Calculated=%d <> From Header=%d", VCheckSum, VPtHeader->CheckSum ) );
      }
      
      err_trace (( ERR_OUT, "Checksum OK = %d", VCheckSum ));
      
    }
    
    
    // Set status
    
    VPtSock->MsgSz    = VRecSz;
    VPtSock->MsgReady = 1;
    
    // Call user function
    
    if ( VPtSock->ParUsrFunc != NULL ) {
      VPtSock->ParUsrFunc ( VPtSock->PtMsg, VPtSock->MsgSz, &(VPtSock->PtMsg[VPtSock->CstHeaderSz]), VUserDataSz );
    }
    
    err_trace (( ERR_OUT, "Message received => %d bytes", VRecSz ));
    
    // Send back size of message
    
    VAckSentSz = send ( VPtSock->ASocket, (SInt8*) &VRecSz, 4, 0 );
    
    if( VAckSentSz == SOCKET_ERROR ) {
      err_retfail ( -1, (ERR_OUT,"send () Ack error %d => %s", IAC__SOCKET_ERRNO, strerror (IAC__SOCKET_ERRNO) ));
    }
    
    break; /* Exit of while (1) at end of processing */
    
  } /* End while (1) */
  
  return (VRecSz);
}





/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 10/11/2008
Rev       : 11/11/2008
          : - New header & trailer
          : 13/01/2009
          : - Return -2 if recv failed on connection lost
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 IAC__PRIV_FSockLookForLongRecMsgOnPortAndProcessIt ( UInt32 RecPortContextId ) {
  
  IAC__TSockReceiverPort* VPtSock;
  SInt8                   VMsgReceptionRuning;
  SInt32                  VMsgSzFromHeader;
  SInt32                  VCheckSumFromHeader;
  SInt32                  VAckSentSz;
  SInt32                  VRecBlocSz ;
  SInt32                  VTotRecSz ;
  IAC__TSockMsgHeader*    VPtMayBeHeader;
  IAC__TSockMsgTrailer*   VPtTrailer;
  SInt32                  VUserDataSz;
  SInt32                  VCheckSum;
  SInt32                  VSockErrNo;
  
  
  
  VPtSock = &(IAC__VGContext.Sock.AReceiverPort[RecPortContextId]);
  
  // Init status
  
  VPtSock->MsgReady   = 0;
  
  VMsgReceptionRuning = 0;
  VRecBlocSz          = 0;
  VTotRecSz           = 0;
  
  
  
  while (1) {
    
    // Read
    
    VRecBlocSz = recv ( VPtSock->ASocket, &(VPtSock->PtMsg[VTotRecSz]), VPtSock->MaxMsgSz, 0 );
    
    err_trace (( ERR_OUT, "VRecBlocSz=%d", VRecBlocSz ));
    
    // If no data        

/*  BEFORE 14/01/09

    if( VRecBlocSz <= 0 ) {

      if ( (VRecBlocSz == 0) ) {
        err_retfail ( -2, (ERR_OUT,"Connection lost !") );
      }
      
      else {
        err_warning (( ERR_OUT, "recv () error %d => %s", IAC__SOCKET_ERRNO, strerror (IAC__SOCKET_ERRNO) ));
        continue;
      }      
      
    }
    
*/    

    if( VRecBlocSz <= 0 ) {
      
      VSockErrNo = IAC__SOCKET_ERRNO;
      
      err_error (( ERR_OUT, "VRecSz=%d - IAC__SOCKET_ERRNO=%d", VRecBlocSz, VSockErrNo ));

      /* --------------------------------------------------------------------------------- */
      /* If connection has been lost                                                       */
      /*   => return -2                                                                    */
      /*   => caller will detected this special error and will try to reboot the receiver  */
      /* --------------------------------------------------------------------------------- */
      
      if ( (VRecBlocSz == 0) || (VSockErrNo == WSAECONNRESET)  ) {
        err_retfail ( -2, (ERR_OUT,"Connection lost ! VRecSz=%d - IAC__SOCKET_ERRNO=%d => %s", VRecBlocSz, VSockErrNo, strerror (VSockErrNo) ));
      }
      
      /* ------------------------- */
      /* Normal warning => No data */
      /* ------------------------- */
      
      else {
        err_warning (( ERR_OUT, "recv () error %d => %s", IAC__SOCKET_ERRNO, strerror (IAC__SOCKET_ERRNO) ));
        continue;
      }
      
    }
    


    // If data
    
    // If it's message header
  
    /* ---------------------------------------------------------------------------------- */
    /* -                                 WARNING                                        - */
    /* ---------------------------------------------------------------------------------- */
    /* VPtMayBeHeader points to a valid header ONLY when current received block is header */
    /* On next blocks VPtMayBeHeader points to a something which IS NOT a header          */
    /* Therefore if you need to access to header fields LATER you must do a copy of       */
    /* these fields WHEN header is detected and use this copy NOT VPtMayBeHeader fields   */
    /* ---------------------------------------------------------------------------------- */
  
    VPtMayBeHeader = (IAC__TSockMsgHeader*) &(VPtSock->PtMsg[VTotRecSz]);
    
    if ( VPtMayBeHeader->StartTag == IAC__SOCK_HEADER_START_TAG ) {
      
      // Do a copy of useful header fields
      
      VMsgSzFromHeader     = VPtMayBeHeader->TotMsgSz;
      VCheckSumFromHeader  = VPtMayBeHeader->CheckSum;
      
      VPtSock->MsgReady   = 0;
      
      VMsgReceptionRuning = 1;
      VTotRecSz           = 0;
      
      err_trace (( ERR_OUT, "Message header tag = %x [H] - MsgSz=%d", VPtMayBeHeader->StartTag, VMsgSzFromHeader ));
      err_trace (( ERR_OUT, "Message header received of %d bytes", VRecBlocSz ));
      
      VTotRecSz = VTotRecSz + VRecBlocSz;
    }
    
    // If it's not message header
    
    else {
      
      // If it's not message header AND we are not in message recpetion phase => Message not for us
      
      if ( VMsgReceptionRuning == 0 ) {
        err_retfail ( -1, (ERR_OUT,"Unknown message ! Not header / Not data part") );
      }
      
      // It's a data block => increment total size
      
      VTotRecSz = VTotRecSz + VRecBlocSz;
      
      err_trace (( ERR_OUT, "At this step tot received size of %d bytes", VTotRecSz ));
    }
    
    // End of message reached
    
    if ( VTotRecSz >= VMsgSzFromHeader ) {
      VMsgReceptionRuning = 0;
      
      // Check message size
      
      if ( VMsgSzFromHeader != VTotRecSz ) {
        err_retfail ( -1, (ERR_OUT,"Header tot sz=%d <> Received sz=%d", VMsgSzFromHeader, VTotRecSz ) );
      }
      
      // Check trailer value
      
      VPtTrailer = (IAC__TSockMsgTrailer*) &(VPtSock->PtMsg[VTotRecSz-(VPtSock->CstTrailerSz)]);
        
      if ( VPtTrailer->StopTag != IAC__SOCK_TRAILER_STOP_TAG ) {
        err_retfail ( -1, (ERR_OUT,"Bad trailer stop tag=%x [H] - Must be %x [H]", VPtTrailer->StopTag, IAC__SOCK_TRAILER_STOP_TAG ) );
      }
      
      // Calculate size of message user part

      VUserDataSz = VTotRecSz - (VPtSock->CstHeaderSz) - (VPtSock->CstTrailerSz);
            
      // Compare checksum
      
      if ( VPtSock->ParFuncCheckSum != NULL ) {
        VCheckSum = VPtSock->ParFuncCheckSum ( &(VPtSock->PtMsg[VPtSock->CstHeaderSz]), VUserDataSz );
        
        if ( VCheckSum != VCheckSumFromHeader ) {
          err_retfail ( -1, (ERR_OUT,"Checksum error : Calculated=%d <> From Header=%d", VCheckSum, VCheckSumFromHeader ) );
        }
        
        err_trace (( ERR_OUT, "Checksum OK = %d", VCheckSum ));
        
      }
      
      // Good message received
      
      VPtSock->MsgSz    = VTotRecSz;
      VPtSock->MsgReady = 1;
      
      if ( VPtSock->ParUsrFunc != NULL ) {
        VPtSock->ParUsrFunc ( VPtSock->PtMsg, VTotRecSz, &(VPtSock->PtMsg[VPtSock->CstHeaderSz]), VUserDataSz );
      }
      
      err_trace (( ERR_OUT, "Enf of message reception => %d bytes", VTotRecSz ));
      
      // Send back size of message
      
      VAckSentSz = send ( VPtSock->ASocket, (SInt8*) &VTotRecSz, 4, 0 );
      
      if( VAckSentSz == SOCKET_ERROR ) {
        err_retfail ( -1, (ERR_OUT,"send () Ack error %d => %s", IAC__SOCKET_ERRNO, strerror (IAC__SOCKET_ERRNO) ));
      }
      
      VMsgReceptionRuning = 0;
      VRecBlocSz          = 0;
      VTotRecSz           = 0;
      
      break;
    }
    
    
  } /* End while (1) */
  
  
  return (VTotRecSz);
  
}




/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 09/11/2008
Rev       : 13/01/2009
          : - Wait for a receicer if connect failed
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */



DWORD WINAPI IAC__PRIV_FSockSenderThreadFunc ( LPVOID lpParam ) {
  
  IAC__TSockSenderPort* VPtSock = (IAC__TSockSenderPort*) lpParam;


  err_error (( ERR_OUT, "BEGINNING of IAC__PRIV_FSockSenderThreadFunc" ));
  
  /* ------------- */
  /* Debug print   */
  /* ------------- */
  
  err_trace (( ERR_OUT, "----------------------------------------------------" ));
  err_trace (( ERR_OUT, "Thread execution of sender port [%d] started", VPtSock ->Id   ));
  err_trace (( ERR_OUT, "Dest host = %s", VPtSock ->ParDestHost     ));
  err_trace (( ERR_OUT, "Dest port = %d", VPtSock ->ParDestPort     ));
  err_trace (( ERR_OUT, "Ack TO    = %d", VPtSock ->ParAckTimeOutMs ));
  err_trace (( ERR_OUT, "----------------------------------------------------" ));
  

  /* -------------------- */
  /* Connect to dest host */  
  /* -------------------- */

  err_warning (( ERR_OUT, "IAC__PRIV_FSockSenderThreadFunc Try to connect ..." ));
  
  // Blocking mode socket

  /* ----------------------------------------------- */
  /* Try to connect until a receiver is detected     */
  /* if connection failed, it waits 100 ms and retry */
  /* ----------------------------------------------- */
  /* Before 13/01/09 it was a single call to connect */
  /* followed by retfail to abort in case of error   */
  /* ----------------------------------------------- */
  
  while ( connect( VPtSock->Socket, &(VPtSock->DestAddr), sizeof(VPtSock->DestAddr) ) == SOCKET_ERROR ) {
    err_error (( ERR_OUT, "Connect () error %d => %s WILL RETRY in 100 ms", IAC__SOCKET_ERRNO, strerror (IAC__SOCKET_ERRNO) ));
    Sleep ( 100 );
  }
    
  err_warning (( ERR_OUT, "Sender [%d] connection to Host %s Port %d done :-)", VPtSock->Id, VPtSock->ParDestHost, VPtSock->ParDestPort ));

  /* --------------------- */
  /* Messages sending loop */
  /* --------------------- */
  
  while (1) {
    
    // Wait event
    
    if ( VPtSock->EvNewMsgHnd != NULL ) {
      WaitForSingleObject ( VPtSock->EvNewMsgHnd, INFINITE /* dwTimeout */ );
    }
        
    // Send message
    
    if ( VPtSock->ParLongMsg == 0 ) {
      VPtSock->SendStatus = IAC__PRIV_FSockSendShortMsg ( VPtSock->Id );
    }
  
    else {
      VPtSock->SendStatus = IAC__PRIV_FSockSendLongMsg ( VPtSock->Id );
    }
    
    
    // Send Ack event
    
    if ( PulseEvent ( VPtSock->EvAckMsgHnd ) == TRUE ) {
      err_trace (( ERR_OUT, "Event EvAckMsgHnd sent" ));
    }
            
  
  } /* End while (1) */
        
  

  err_error (( ERR_OUT, "END of IAC__PRIV_FSockSenderThreadFunc" ));
  
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 05/01/2008
Rev       : 10/11/2008
Doc date  : //2008
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */



DWORD WINAPI IAC__PRIV_FSockReceiverThreadFunc ( LPVOID lpParam ) {
  
  IAC__TSockReceiverPort* VPtSock = (IAC__TSockReceiverPort*) lpParam;
  SInt32                  VRet;


  /* ------------- */
  /* Debug print   */
  /* ------------- */
  
  err_trace (( ERR_OUT, "----------------------------------------------------" ));
  err_trace (( ERR_OUT, "Thread execution of receiver port [%d] started", VPtSock->Id   ));
  err_trace (( ERR_OUT, "----------------------------------------------------" ));
  
  
  /* ---------------------- */
  /* Wait client connection */
  /* ---------------------- */
  
  // Listen

  err_trace (( ERR_OUT, "Before listen" ));
  
  if ( listen( VPtSock->LSocket, 2) == SOCKET_ERROR ) {
    err_retfail ( -1, (ERR_OUT,"listen () error %d => %s", IAC__SOCKET_ERRNO, strerror (IAC__SOCKET_ERRNO) ));
  }

  err_trace (( ERR_OUT, "Before accept" ));
  
  // Accept

  VPtSock->ASocket = accept ( VPtSock->LSocket, NULL, NULL );
  
  if( VPtSock->ASocket == INVALID_SOCKET )  {
    err_retfail ( -1, (ERR_OUT,"accept () error %d => %s", IAC__SOCKET_ERRNO, strerror (IAC__SOCKET_ERRNO) ));
  }

  err_trace (( ERR_OUT, "Before loop" ));
  
  /* ---------------------- */
  /* Reception loop         */
  /* ---------------------- */
  
  while (1) {
  
    if ( VPtSock->DbgDisableReception == 1 ) {
      continue;
    }
  
    if ( VPtSock->ParLongMsg == 0 ) {
      VRet = IAC__PRIV_FSockLookForShortRecMsgOnPortAndProcessIt ( VPtSock->Id );
    }
    
    else {
      VRet = IAC__PRIV_FSockLookForLongRecMsgOnPortAndProcessIt ( VPtSock->Id );      
    }
    
    if ( VRet == -2 ) {
      Sleep ( 100 );
      err_error (( ERR_OUT, "IAC__FSockRebootReceiverPort called !" ));
      IAC__FSockRebootReceiverPort ( VPtSock->Id );
    }
    
        
  } /* End while (1) */


  return (0);
}





#endif












