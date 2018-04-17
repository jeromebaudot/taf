
/*******************************************************************************
File      : x:\lib\win\eudet_frio\eudet_frio.c
Goal      : Functions of flex rio board library for EUDET Mimosa 26 DAQ
Prj date  : 05/08/2010
File date : 05/08/2010
Doc date  : 05/08/2010
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


#ifndef EUDET_FRIO_C
#define EUDET_FRIO_C


// EFRIO__EXTRA_CHAN__MAX_TRIGGER_FIELD_NB
// EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB

// EFRIO__EXTRA_CHAN__TRIGGER_FIELD_SZ
// EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ



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
Date      : 05/08/2010
Doc date  : 05/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FBegin ( SInt8 ErrorLogLvl, char* ErrorLogFile, SInt8 MsgLogLvl, char* MsgLogFile ) {
  
  EFRIO__TContext*     VPtCont  = &EFRIO__VGContext;
  EFRIO__TBoardConf*   VPtBoard = &EFRIO__VGContext.ABoardsConf[0];
  EFRIO_TAcqEmul*      VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
  SInt32 VRet;
  SInt8  VEnableErrLog;
  SInt8  VEnableMsgLog;
  
  
  err_trace (( ERR_OUT, "DPXI_FBegin ( ErrorLogLvl=%d, ErrorLogFile=%s, MsgLogLvl=%d, MsgLogFile=%s )", ErrorLogLvl, ErrorLogFile, MsgLogLvl, MsgLogFile ));
    
  memset ( &EFRIO__VGContext, 0, sizeof (EFRIO__TContext) );
  
  // Conf errors logging
  
  VEnableErrLog = ( ErrorLogLvl != 0 );
  
  VRet = ERR_FBegin ( VEnableErrLog, ErrorLogFile );
  
  err_retfail ( VRet, (ERR_OUT,"ERR_FBegin failed !" ) );
  
  VRet = ERR_FSetFileLogLevel ( ErrorLogLvl );

  err_retfail ( VRet, (ERR_OUT,"ERR_FSetFileLogLevel failed !" ) );
  
  // Conf messages logging
  
  VEnableMsgLog = ( MsgLogLvl != 0 );

  VRet = MSG_FBegin ( VEnableMsgLog, MsgLogFile );
  
  err_retfail ( VRet, (ERR_OUT,"MSG_FBegin failed !" ) );
  
  VRet = MSG_FSetFileLogLevel ( MsgLogLvl );

  err_retfail ( VRet, (ERR_OUT,"MSG_FSetFileLogLevel failed !" ) );
  
  // Init // port
  
  #ifdef DLL_INCLUDE_PARA_PORT
    PPO_FBegin ( VEnableErrLog, ErrorLogFile );
    PPO_FOpen  ( 0x378 /* BaseAdr */, 0 /* Id */ );
    PPO_FEnableReadDataOutPortBeforeWrite ( 0 /* Id */, 1 /* Enable */ );
  #endif
  
  // Reset lib context
  
  memset ( VPtCont, 0 , sizeof (EFRIO__TContext) );
  
  // Set default values to first board conf in order to get DmaHostSize initialized BEFORE fw loading
  
  VPtBoard->AsicNb        = EFRIO__MAX_ASIC_NB;    // Max possible number
  VPtBoard->FrameNbPerAcq = EFRIO__MAX_FRAME_NB_PER_ACQ; // "Nominal" value
  
  // DMA host size is the memory size allocated for DMA on CPU side
  // It must be equal AT LEAST to the size of one acquisition and higher value are not useful
  // VPtBoard->AsicNb + 1 => + 1 for extra channel
  
  VPtBoard->DmaHostSz     = ((MI26__ZS_FFRAME_MODE_2X80MHZ_W8_SZ * 2 * (VPtBoard->AsicNb + 1) * VPtBoard->FrameNbPerAcq) / (1024 * 1024)) + 5;
  
  
  // Set default values of Header & Trailer for DQ emulation
  
  VPtAcqEmul->ParAHeader[0] = 0x80008001;
  VPtAcqEmul->ParAHeader[1] = 0x80008002;
  VPtAcqEmul->ParAHeader[2] = 0x80008003;
  VPtAcqEmul->ParAHeader[3] = 0x80008004;
  VPtAcqEmul->ParAHeader[4] = 0x80008005;
  VPtAcqEmul->ParAHeader[5] = 0x8000800;

  VPtAcqEmul->ParATrailer[0] = 0xAAAAAAAA;
  VPtAcqEmul->ParATrailer[1] = 0xBBBBBBBB;
  VPtAcqEmul->ParATrailer[2] = 0xCCCCCCCC;
  VPtAcqEmul->ParATrailer[3] = 0xDDDDDDDD;
  VPtAcqEmul->ParATrailer[4] = 0xEEEEEEEE;
  VPtAcqEmul->ParATrailer[5] = 0xFFFFFFFF;
  
  
  VPtCont->InfInitDone = 0;
  
  msg (( MSG_OUT, "EFRIO_FBegin Done :-)" ));
  
  err_error (( ERR_OUT, "********************************************************************" ));
  err_error (( ERR_OUT, "Info => EUDET Flex RIO DLL compiled on %s at %s", __DATE__, __TIME__ ));
  err_error (( ERR_OUT, "********************************************************************" ));
  
  err_error (( ERR_OUT, "TRACE ASIC__TFrameStatus=%d Bytes", sizeof (ASIC__TFrameStatus) ));
  err_error (( ERR_OUT, "TRACE MI26__TZsFFrameRaw=%d Bytes", sizeof (MI26__TZsFFrameRaw) ));
  
  err_error (( ERR_OUT, "TRACE UInt64=%d", sizeof (UInt64) ));
  
  err_retok (( ERR_OUT, "end" ));
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
Date      : 05/08/2010
Doc date  : 05/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FEnd () {
  
  EFRIO__TContext*     VPtCont  = &EFRIO__VGContext;
  
  #ifdef DLL_INCLUDE_PARA_PORT
    PPO_FEnd ();
  #endif
  
  // Free frames buffer if allocated
  
  if ( VPtCont->RunCont.PtZsFFrameRaw !=NULL ) {
    free ( VPtCont->RunCont.PtZsFFrameRaw );
  }
  
  // Reset context record
  
  memset ( &EFRIO__VGContext, 0, sizeof (EFRIO__TContext) );
  
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
Remark    : AcqId param is not used => Set it to -1
:
Level     :
Date      : 06/11/2010
Doc date  : 06/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FBuildFrameListFromAcq ( SInt32 FrameNb, void* PtAcqData, EFRIO__TFrameList* PtList ) {

  SInt32 ViFrame;
  EFRIO__TFrame* VPtFirstFr;
  EFRIO__TFrame* VPtNextFr;
  
  // --------------
  // Check param
  // --------------
  
  if ( (FrameNb <= 0) || (FrameNb > EFRIO__MAX_FRAME_NB_PER_ACQ) ) {
    err_retfail ( -1, (ERR_OUT,"FrameNB=%d out of range 1..%d", FrameNb, EFRIO__MAX_FRAME_NB_PER_ACQ ) );
  }

  err_retnull ( PtAcqData, (ERR_OUT,"PtAcqData == NULL") );
  err_retnull ( PtList   , (ERR_OUT,"PtList == NULL   ") );
    
  // --------------
  // Reset list
  // --------------
  
  memset ( PtList,0 , sizeof (EFRIO__TFrameList) );
  
  
  // --------------
  // Build frames list
  // --------------
  
  PtList->TotFrameNb = FrameNb;

  // Set frame pointer on first frame
  
  VPtFirstFr = (EFRIO__TFrame*) PtAcqData;
  
  // Fill first elt
  
  PtList->AFrameSz[0]  = VPtFirstFr->TotSz;
  PtList->AFramePtr[0] = VPtFirstFr;
  
  VPtNextFr = (EFRIO__TFrame*) ( ((UInt8*) VPtFirstFr) + VPtFirstFr->TotSz );
  
  // Fill followinf elt
  
  for ( ViFrame=1; ViFrame < FrameNb; ViFrame++ ) {
    PtList->AFrameSz[ViFrame]  = VPtNextFr->TotSz;
    PtList->AFramePtr[ViFrame] = VPtNextFr;
    VPtNextFr = (EFRIO__TFrame*) ( ((UInt8*) VPtNextFr) + VPtNextFr->TotSz );
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
Date      : 31/10/2010
Doc date  : 31/10/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

// RunInLabview = 0 => Run in C++ Builder DAQ emulation application
// RunInLabview = 1 => Run in Labview DAQ

SInt32 EFRIO__FEmuleBegin ( SInt8 RunInLabview ) {
  
  EFRIO__TRunCont*   VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO_TAcqEmul*    VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
  EFRIO_TFrCheck*    VPtFrChk   = &EFRIO__VGContext.FrCheck;


  // Reset context records
  
  memset ( VPtAcqEmul, 0, sizeof (EFRIO_TAcqEmul) );
    
  memset ( VPtFrChk  , 0, sizeof (EFRIO_TFrCheck) );
    
  // Set default value of DAQ emulation parameters => Mainly for Labview which will not control all of them   
      
  VPtAcqEmul->ParAcqCycleMs       = 200;
  VPtAcqEmul->ParEmuleDRamReadMs  =   0;
  VPtAcqEmul->ParEmuleFunctNo     =   0;
  VPtAcqEmul->ParRandomDataSz     =   0;
  VPtAcqEmul->ParSetMaxDataSzOnOneMaps = 0;
  
  VPtAcqEmul->ParAHeader[0]       = 0x80008001;
  VPtAcqEmul->ParAHeader[1]       = 0x80008002;
  VPtAcqEmul->ParAHeader[2]       = 0x80008003;
  VPtAcqEmul->ParAHeader[3]       = 0x80008004;
  VPtAcqEmul->ParAHeader[4]       = 0x80008005;
  VPtAcqEmul->ParAHeader[5]       = 0x80008006;  
  
  VPtAcqEmul->ParATrailer[0]      = 0xAAAA0001;
  VPtAcqEmul->ParATrailer[1]      = 0xAAAA0002;
  VPtAcqEmul->ParATrailer[2]      = 0xAAAA0003;
  VPtAcqEmul->ParATrailer[3]      = 0xAAAA0004;
  VPtAcqEmul->ParATrailer[4]      = 0xAAAA0005;
  VPtAcqEmul->ParATrailer[5]      = 0xAAAA0006;
  
  VPtAcqEmul->ParTrigNbPerFrame           = 0;
  VPtAcqEmul->ParTrigOnOneFrameOverN      = 1;
  VPtAcqEmul->ParTrigOnNConsecutiveFrames = 1;
  
  VPtAcqEmul->ParATrig[0] = 0;
  VPtAcqEmul->ParATrig[1] = 0;
  VPtAcqEmul->ParATrig[2] = 0;
  VPtAcqEmul->ParATrig[3] = 0;
  
  VPtAcqEmul->ParATS[0]   = 0;
  VPtAcqEmul->ParATS[1]   = 0;
  VPtAcqEmul->ParATS[2]   = 0;
  VPtAcqEmul->ParATS[3]   = 0;
  
  
    
  // Set Mi26 nb

  VPtFrChk->InfMi26Nb = VPtRunCont->ParMi26Nb;
  
  // Extra channel or not ?
  
  if ( (VPtRunCont->ParDataTransferMode == EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES) || (VPtRunCont->ParDataTransferMode == EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG)  ) {
    VPtAcqEmul->InfExtraChan = 1;
  }
  
  else {
    VPtAcqEmul->InfExtraChan = 0;
  }

  if ( RunInLabview == 0 ) {
    
    // Calculate DRAM size
    
    VPtAcqEmul->InfDRamSz = (VPtRunCont->ParMi26Nb + VPtAcqEmul->InfExtraChan) * VPtRunCont->ParFrameNbPerAcq * MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * 4;
    
    VPtAcqEmul->InfDRamSzMb = VPtAcqEmul->InfDRamSz / (1024 * 1024);
    
    // Alloc RAM to emulate Flex RIO DRAM
    
    VPtAcqEmul->InfDRamPtr = (UInt32*) malloc ( VPtAcqEmul->InfDRamSz );
    
    err_retnull ( VPtAcqEmul->InfDRamPtr, (ERR_OUT,"Allocation of %d bytes for Flex RIO DRAM emulation failed !", VPtAcqEmul->InfDRamSz) );
       
  } // End if ( RunInLabview == 0 )

  
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
Date      : 31/10/2010
Doc date  : 31/10/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FEmuleEnd ( ) {
  
  EFRIO__TRunCont*   VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO_TAcqEmul*    VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
  EFRIO_TFrCheck*    VPtFrChk   = &EFRIO__VGContext.FrCheck;
  
  
  if ( VPtAcqEmul->InfDRamPtr != NULL ) {
    free ( VPtAcqEmul->InfDRamPtr );
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
Date      : 31/10/2010
Doc date  : 31/10/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

UInt32* EFRIO__FEmuleReadDRam ( SInt8 Cmd ) {

  EFRIO_TAcqEmul*    VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
  
  
  if ( Cmd == 1 ) {
    memset ( VPtAcqEmul->InfDRamPtr, 0, VPtAcqEmul->InfDRamSz );
  }
  
  
  return ( VPtAcqEmul->InfDRamPtr );
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    : EmuleMode parameter is used to define number of trigger to emulate
: - EmuleMode >= 0 -> No trigger
: - EmuleMode < 0  -> Nb trigger = absolute value of EmuleMode
:
Level     : This is a user level function.
Date      : 03/11/2010
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__FSetFrameIdInTriggerRec ( SInt32 FrameId, SInt32 TrigNb, EFRIO__TTriggerRec* PtRec ) {
  
  SInt32 ViTrig;
  EFRIO__TTluTrigger*         VPtTrig;
  EFRIO__TFlexRioTimeStamp1*  VPtTs;
  
  err_retnull ( PtRec, (ERR_OUT,"PtRec == NULL") );
  
  if ( TrigNb > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ) {
    err_retfail ( -1, (ERR_OUT,"TrigNb=%d > Max=%d", TrigNb, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ) );
  }
  
  for ( ViTrig=0; ViTrig < TrigNb; ViTrig++ ) {
    
    VPtTrig = (EFRIO__TTluTrigger*)        &PtRec->ATrig[2 * ViTrig];
    VPtTs   = (EFRIO__TFlexRioTimeStamp1*) &PtRec->ATrig[(2 * ViTrig)+1];
    
    VPtTrig->F.FrameIdInAcq = FrameId;
    VPtTs->F.Mi26Frame      = FrameId;
  }
  
  return (0);
}

  
// 07/09/2010

// CmdGetPtrCpyAlloc = 0 => return buffer pointer
// CmdGetPtrCpyAlloc = 1 => store PtSrcW32 but NO copy of data
// CmdGetPtrCpyAlloc = 2 => copy src to buffer
// CmdGetPtrCpyAlloc = 3 => alloc mem

UInt32* EFRIO__FTrfData ( SInt8 CmdGetPtrCpyAlloc, UInt32 AllocW32Sz, UInt32* PtSrcW32, UInt32 SrcW32Sz ) {
  
  static UInt32* VPtBuff     = NULL;
  static UInt32  VAllocW32Sz = 0;
         UInt32  VAllocSz;
         SInt32  Vi; 
  
  // Get pointer request
  
  if ( CmdGetPtrCpyAlloc == 0 ) {
    return (VPtBuff);
  }

  // Store pointer request
  
  if ( CmdGetPtrCpyAlloc == 1 ) {
    VPtBuff = PtSrcW32;
    return (VPtBuff);
  }
  

  // Copy request
  
  if ( CmdGetPtrCpyAlloc == 2 ) {
    
    if ( (VPtBuff == NULL) || (SrcW32Sz > VAllocW32Sz) ) {
      err_error (( ERR_OUT, "VPtBuff = %x = NULL ? / SrcW32Sz=%d > VAllocW32Sz=%d", VPtBuff, SrcW32Sz, VAllocW32Sz ));
      return (NULL);
    }
    
    for ( Vi=0; Vi < SrcW32Sz; Vi++ ) {
      VPtBuff[Vi] = PtSrcW32[Vi];
    }
    
    return (VPtBuff);
  }
  
  
  // Allocation request
  
  if ( CmdGetPtrCpyAlloc == 3 ) {
    
    // Free mem if already allocated
    
    if ( VPtBuff != NULL ) {
      free ( VPtBuff );
      VPtBuff = NULL;
    }
    
    // Alloc new mem
    
    VAllocW32Sz = AllocW32Sz;
    VAllocSz    = VAllocW32Sz * 4;
    
    VPtBuff = (UInt32*) malloc ( VAllocSz );
    
    // If allocation failed => error message + ret NULL
    
    if ( VPtBuff == NULL ) {
      err_error (( ERR_OUT, "Allocation of %d bytes failed !", VAllocSz ));
      return (NULL);
    }
    
    // If allocation OK => ret buffer pointer
    
    return (VPtBuff);   
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
Date      : 20/10/2010
Doc date  : 20/10/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

// Cmd = 0 => do nothing
// Cmd = 1 => alloc mem
// Cmd = 2 => free mem

// Cmd = 3 => Copy with memcpy
// Cmd = 4 => Copy with W8 loop Vi ++
// Cmd = 5 => Copy with W8 loop Vi --
// Cmd = 6 => Copy with W32 loop Vi ++
// Cmd = 7 => Copy with W32 loop Vi --
// Cmd = 8 => Copy with memmove

SInt32 EFRIO__FDbgMeasRamCpyTime ( SInt8 Cmd, SInt32 SzMb ) {

  static UInt8* VBuffSrc  = NULL;
  static UInt8* VBuffDest = NULL;
  auto   SInt32 _VSzW32;
  auto   SInt32 _Vi;
  register   SInt32 r_VSzW32;
  register   SInt32 r_Vi;
  SInt32  VSzW8;
          SInt32 VSzW32;
          SInt32 VSzW64;
          SInt32 Vi;
  UInt8*  VCpyBuffSrc;
  UInt8*  VCpyBuffDest;
  UInt64* VCpyBuffSrcW64;
  UInt64* VCpyBuffDestW64;
  

  if ( (Cmd < 0) || (Cmd > 7) ) {
    err_retfail ( Cmd, (ERR_OUT,"Bad Cmd=%d Our of range 0..7", Cmd) );
  }

  VSzW8    = SzMb * 1024 * 1024;
  VSzW32   = VSzW8 / 4;
  _VSzW32  = VSzW8 / 4;
  r_VSzW32 = VSzW8 / 4;
  VSzW64   = VSzW8 / 8;
  
  // err_error (( ERR_OUT, "TRACE => VSzW8=%d - VSzW32=%d", VSzW8, VSzW32 ));
  
  while (1) {
    
    // Allocation mem
    
    if ( Cmd == 1 ) {
      
      err_error (( ERR_OUT, "TRACE => Cmd=1 Alloc mem" ));
      
      // Source
      
      if ( VBuffSrc != NULL ) {
        free ( VBuffSrc );
      }
      
      VBuffSrc = (UInt8*) malloc ( VSzW8 );
      
      err_retnull ( VBuffSrc, (ERR_OUT,"Allocation of src buffer %d bytes failed !", VSzW8 ) );

      // Dest

      if ( VBuffDest != NULL ) {
        free ( VBuffDest );
      }
      
      VBuffDest = (UInt8*) malloc ( VSzW8 );
      
      err_retnull ( VBuffDest, (ERR_OUT,"Allocation of dest buffer %d bytes failed !", VSzW8 ) );
      
      break;
    }

    // Free mem
    
    if ( Cmd == 2 ) {

      err_error (( ERR_OUT, "TRACE => Cmd=2 => Free mem" ));
      
      if ( VBuffSrc != NULL ) {
        free ( VBuffSrc );
        VBuffSrc = NULL;
      }
      
      if ( VBuffDest != NULL ) {
        free ( VBuffDest );
        VBuffDest = NULL;
      }      
      
     break; 
    }

    // Copy with memcpy
    
    if ( Cmd == 3 ) {
      
      PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
        
      memcpy ( VBuffDest, VBuffSrc, VSzW8 );
      
      PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
      
      break;
    }

    // Copy with W8 loop Vi ++
    
    if ( Cmd == 4 ) {
      
      PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
            
      for ( Vi=0; Vi < VSzW8; Vi++ ) {
        VBuffDest[Vi] = VBuffSrc[Vi];
      }
           
      PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
      
      break;
    }

    // Copy with W8 loop Vi --
    
    if ( Cmd == 5 ) {
      
      PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
      
      for ( Vi=VSzW8; Vi >= 0; Vi-- ) {
        VBuffDest[Vi] = VBuffSrc[Vi];
      }
      
      PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
      
      break;
    }

    // Copy with W32 loop Vi ++
    
    if ( Cmd == 6 ) {
      
      PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
      
      for ( Vi=0; Vi < VSzW32; Vi++ ) {
        VBuffDest[Vi] = VBuffSrc[Vi];
      }
      
      PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
      
      break;
    }

    // Copy with W32 loop Vi --
    
    if ( Cmd == 7 ) {
      
      PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
      
      for ( Vi=VSzW32; Vi >= 0; Vi-- ) {
        VBuffDest[Vi] = VBuffSrc[Vi];
      }
      
      PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
      
      break;
    }
    
    // Copy with memmove
    
    if ( Cmd == 8 ) {
      
      PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
      
      memmove ( VBuffDest, VBuffSrc, VSzW8 );
      
      PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
      
      break;
    }


    // Copy with W32 loop Vi ++ with auto var
    
    if ( Cmd == 9 ) {
      
      PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
      
      for ( _Vi=0; _Vi < _VSzW32; _Vi++ ) {
        VBuffDest[_Vi] = VBuffSrc[_Vi];
      }
      
      PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
      
      break;
    }

    // Copy with W32 loop Vi ++ with local ptr
    
    if ( Cmd == 10 ) {
      
      PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
  
      VCpyBuffSrc  = VBuffSrc;
      VCpyBuffDest = VBuffDest;
  
      for ( Vi=0; Vi < VSzW32; Vi++ ) {
        VCpyBuffDest[Vi] = VCpyBuffSrc[Vi];
      }
      
      PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
      
      break;
    }

    // Copy with W32 loop Vi ++ with register var
    
    if ( Cmd == 11 ) {
      
      PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
      
      for ( r_Vi=0; r_Vi < r_VSzW32; r_Vi++ ) {
        VBuffDest[r_Vi] = VBuffSrc[r_Vi];
      }
      
      PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
      
      break;
    }
    

    // Copy with W64 loop Vi ++ with local ptr
    
    if ( Cmd == 12 ) {
      
      PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
      
      VCpyBuffSrcW64  = (UInt64*) VBuffSrc;
      VCpyBuffDestW64 = (UInt64*) VBuffDest;
      
      for ( Vi=0; Vi < VSzW64; Vi++ ) {
        VCpyBuffDestW64[Vi] = VCpyBuffSrcW64[Vi];
      }
      
      PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
      
      break;
    }
    

    // End while (1)
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
Date      : 22/10/2010
Doc date  : 22/10/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */



// Cmd = 0 -> Reset frame counter & flags ( don't care about trigger param )
// Cmd = 1 -> Insert text line separator "=============" ( do nothing else )
// Cmd = 2 -> Emule frame

SInt32 EFRIO__FDbgCheckTrigAlgo ( SInt8 Cmd, SInt8 TriggerNb, SInt8 AddFrameBefore, SInt8 AddFrameAfter ) {

  static SInt32 VFrameCnt        = 0;
  static SInt8  VAddFrameNp1     = 0;
  static SInt8  VAddFrameNm1Done = 0;
         SInt32 VRetCurFrameId;
  
  while (1) {
    
    if ( Cmd == 0 ) {
      VFrameCnt        = 0;
      VAddFrameNp1     = 0;
      VAddFrameNm1Done = 0;
      VRetCurFrameId   = -1;
      msg (( MSG_OUT, "Cmd = 0 -> Reset frame cnt done" ));
      break;
    }
    
    
    if ( Cmd == 1 ) {
      VRetCurFrameId   = -1;
      msg (( MSG_OUT, "*******************************************************" ));
      break;
    }
    
    
    if ( Cmd == 2 ) {
      
      VRetCurFrameId = VFrameCnt;
      
      msg (( MSG_OUT, "-------------------------------------------------------" ));
      msg (( MSG_OUT, "Emule frame %d - %d triggers - Add F before = %d - Add F after = %d", VFrameCnt, TriggerNb, AddFrameBefore, AddFrameAfter ));
      msg (( MSG_OUT, "-------------------------------------------------------" ));
      

      if ( TriggerNb > 0 ) {
        
        if ( AddFrameAfter == 1 ) {
          VAddFrameNp1 = 1;
        }
        
        
        // Must add Fn-1
        
        if ( (VAddFrameNm1Done == 0) && (AddFrameBefore == 1) ) {
          msg (( MSG_OUT, "Add Fn-1 = %d", VFrameCnt-1 ));
        }
        
        msg (( MSG_OUT, "Add Fn = %d", VFrameCnt ));
        VAddFrameNm1Done = 1;
        
      } // End if ( TriggerNb > 0 )
      
      
      // TriggerNb == 0
      
      else {
      
        // Must add Fn+1
      
        if ( VAddFrameNp1 == 1 ) {
          VAddFrameNp1 = 0;

          msg (( MSG_OUT, "Add Fn = %d", VFrameCnt ));
          VAddFrameNm1Done = 1;
        }
        
        // Don't add Fn+1
        
        else {
          VAddFrameNm1Done = 0;
        }
    
      } // End TriggerNb == 0
      
      
      break; // End frame emulation
    }
    
    
    
    msg (( MSG_OUT, "Unknown command = %d", Cmd ));
    break;
  }
    
  ++VFrameCnt;
  
  return (VRetCurFrameId);
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
Date      : 06/08/2010
Rev       : 04/11/2010
          : - Save to disk
Doc date  : 06/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */
  

// DataConvertMode
// 0 - IPHC  mode   = Demultiplex the data part, doesn't handle extra channel -> for compatibility with IPHC DAQ sw
// 1 - EUDET mode 1 = Don't demultiplex data part, don't care about extra channel, send all frames
// 2 - EUDET mode 2 = Don't demultiplex data part, extract trigger info from extra channel, send all frames
// 3 - EUDET mode 3 = Don't demultiplex data part, extract trigger info from extra channel, send only frames with trigger ( use TriggerHandlingMode parameter )

// 0 - EFRIO__TRF_MODE_IPHC
// 1 - EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN,
// 2 - EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES,
// 3 - EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG


SInt32 EFRIO__FConfRun ( SInt8 Mi26Nb, SInt32 RunNo, SInt32 TotEvNb, SInt32 EvNbPerFile, SInt32 FrameNbPerAcq, SInt8 DataTransferMode, char* DestDir, char* FileNamePrefix, SInt8 SaveToDisk, SInt8 SendOnEth, SInt8 SendOnEthPCent ) {
  
  EFRIO__TContext*     VPtCont  = &EFRIO__VGContext;
  EFRIO__TBoardConf*   VPtBoard = &EFRIO__VGContext.ABoardsConf[0];
  SInt32 VMaxFrameSz;
  SInt32 VRet;
  
  // Check function parameters
  
  err_retnull ( DestDir       , (ERR_OUT,"Abort : DestDir == NULL        !") );
  err_retnull ( FileNamePrefix, (ERR_OUT,"Abort : FileNamePrefix == NULL !") );
  
  // Debug print function parameters
  
  err_trace (( ERR_OUT, "Call with : Mi26Nb=%d - RunNo=%d - TotEvNb=%d - EvNbPerFile=%d - FrameNbPerAcq=%d - DestDir=%s - FileNamePrefix=%s", Mi26Nb, RunNo, TotEvNb, EvNbPerFile, FrameNbPerAcq, DestDir, FileNamePrefix ));
  
  // Copy context parameters
  
  VPtCont->RunCont.ParDataTransferMode = DataTransferMode;
  
  // Copy run parameters to run context record
  
  VPtCont->RunCont.ParMi26Nb           = Mi26Nb;
  VPtCont->RunCont.ParRunNo            = RunNo;
  VPtCont->RunCont.ParTotEvNb          = TotEvNb;
  VPtCont->RunCont.ParEvNbPerFile      = EvNbPerFile;
  VPtCont->RunCont.ParFrameNbPerAcq    = FrameNbPerAcq;
  VPtCont->RunCont.ParDataTransferMode = DataTransferMode;
  
  VPtCont->RunCont.ParSaveOnDisk       = SaveToDisk;
  VPtCont->RunCont.ParSendOnEth        = SendOnEth;
  VPtCont->RunCont.ParSendOnEthPCent   = SendOnEthPCent;
  
  sprintf ( VPtCont->RunCont.ParDestDir       , "%s", DestDir );
  sprintf ( VPtCont->RunCont.ParFileNamePrefix, "%s", FileNamePrefix );
  
  sprintf ( VPtCont->RunCont.ResDataFileName, "%s\\%s%d.bin", VPtCont->RunCont.ParDestDir, VPtCont->RunCont.ParFileNamePrefix, VPtCont->RunCont.ParRunNo);
  
  sprintf ( VPtCont->RunCont.ResConfFileName, "%s\\%s%d.par", VPtCont->RunCont.ParDestDir, VPtCont->RunCont.ParFileNamePrefix, VPtCont->RunCont.ParRunNo);
  


  // Compare run conf paramters to the max parameters used to calculate DmaHostSz in FBegin
  // If they have higher values => DmaHostSz send to board at fw loading is bad => Abort 
  
  if ( (VPtCont->RunCont.ParMi26Nb > EFRIO__MAX_ASIC_NB) || (VPtCont->RunCont.ParFrameNbPerAcq > EFRIO__MAX_FRAME_NB_PER_ACQ) ) {
    err_error   (( ERR_OUT, "Bad Mi26 nb          ? Run conf = %d - Max = %d", VPtCont->RunCont.ParMi26Nb, EFRIO__MAX_ASIC_NB ));
    err_error   (( ERR_OUT, "Bad frame nb per acq ? Run conf = %d - Max = %d", VPtCont->RunCont.ParFrameNbPerAcq, EFRIO__MAX_FRAME_NB_PER_ACQ ));
    err_retfail ( -1, (ERR_OUT,"Abort : Bad value of AsicNb=%d or FrameNbPerAcq=%d", VPtCont->RunCont.ParMi26Nb, VPtCont->RunCont.ParFrameNbPerAcq ) );
  }
  
  // Overwrite default values of AsicNb & FrameNbPerAcq
  
  VPtBoard->AsicNb        = VPtCont->RunCont.ParMi26Nb;
  VPtBoard->FrameNbPerAcq = VPtCont->RunCont.ParFrameNbPerAcq;
  
  // DMA host size => Already initialized in FBegin function
  // VPtBoard->DmaHostSz     = (MI26__ZS_FFRAME_MODE_2X80MHZ_W8_SZ * 2 * VPtBoard->AsicNb * VPtBoard->FrameNbPerAcq) / (1024 * 1024);

  // Configure others board conf parameters

  VPtBoard->BoardId = 0;
  
  sprintf ( VPtBoard->AsicName, "%s", "Mimosa 26" );
  
  VPtBoard->ReadoutMode              =  0; // Reserved for future use
  VPtBoard->DataClkFrequency         = 80; // 80 MHz
  
  if ( (DataTransferMode == EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES) || (DataTransferMode == EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG) ) {
    VPtBoard->EnableExtraChannel = 1; // Yes
  }
  
  else {
    VPtBoard->EnableExtraChannel = 0; // No
  }
  
  err_error (( ERR_OUT, "TRACE => EnableExtraChannel=%d", VPtBoard->EnableExtraChannel ));
  
  VPtBoard->TriggerMode              = 0; // Trigger mode -> disabled
  VPtBoard->TriggerDetectTimeWindow  = 0; // Trigger mode -> Window during which we count triggers to detect beginning of spill
  VPtBoard->TriggerDetectOccurNb     = 0; // Trigger mode -> Number of trigger required during "TriggerDetectTimeWindow" to decide that spill has begun
  VPtBoard->AcqNbPerTrig             = 0; // Trigger mode -> Number of consecutive Acq stored after beginning of spill detection
  
  VPtBoard->EnableTimeStamping       = 0; // On board time stamping -> disabled
  VPtBoard->TimeStampRes             = 0; // On board time stamp resolution in [ns]
  
  VPtBoard->EnableTrigCnt            = 0; // On board trigger counter
  
  VPtBoard->TagEventsStoredByDUT     = 0; // Tag events taken dy DUT -> disabled
  VPtBoard->ReadTluTrigCntEachNTrig  = 0; // Read event counter provided by TLU -> disabled
  
  
  // Allocate memory
    
  // IPHC data transfer mode
  
  if ( DataTransferMode == EFRIO__TRF_MODE_IPHC ) {
    
    // Free tmp trigger record if allocated
    
    if ( VPtCont->PtTmpTrigRec != NULL ) {
      free ( VPtCont->PtTmpTrigRec );
    }
    
    // Free EUDET mode buffer
    
    if ( VPtCont->RunCont.PtFrame != NULL ) {
      free ( VPtCont->RunCont.PtFrame );
    }
    
    VPtCont->RunCont.PtFrame        = NULL;
    VPtCont->RunCont.InfFrameBuffSz = 0;
    
    // Alloc IPHC mode buffer

    if ( VPtCont->RunCont.PtZsFFrameRaw != NULL ) {
      free ( VPtCont->RunCont.PtZsFFrameRaw );
    }
  
    VPtCont->RunCont.PtZsFFrameRaw = NULL;
  
    VPtCont->RunCont.InfZsFFrameRawBuffSz = VPtCont->RunCont.ParMi26Nb * (VPtCont->RunCont.ParFrameNbPerAcq + 200) * sizeof (MI26__TZsFFrameRaw);
  
    VPtCont->RunCont.PtZsFFrameRaw = (MI26__TZsFFrameRaw*) malloc ( VPtCont->RunCont.InfZsFFrameRawBuffSz );
  
    err_retnull ( VPtCont->RunCont.PtZsFFrameRaw, (ERR_OUT,"Allocation of IPHC buffer for %d frames failed !", VPtCont->RunCont.ParFrameNbPerAcq ) );
  }

  // EUDET data transfer mode
    
  else {
      
    // Alloc tmp trigger record
  
    if ( VPtCont->PtTmpTrigRec != NULL ) {
      free ( VPtCont->PtTmpTrigRec );
    }
    
    VPtCont->PtTmpTrigRec = (EFRIO__TTriggerRec*) malloc ( sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );
          
    err_retnull ( VPtCont->PtTmpTrigRec, (ERR_OUT,"Allocate tmp trigger buff failed !" ) );
    
    // Free IPHC mode buffer
    
    if ( VPtCont->RunCont.PtZsFFrameRaw != NULL ) {
      free ( VPtCont->RunCont.PtZsFFrameRaw );
    }
    
    VPtCont->RunCont.PtZsFFrameRaw        = NULL;
    VPtCont->RunCont.InfZsFFrameRawBuffSz = 0;

    err_error (( ERR_OUT, "TRACE => EUDET - Reset frame list " ));
    
    // Reset frame list
        
    memset ( VPtCont->AAcqFrameList, 0, sizeof (EFRIO__TFrameList) );
    
    // Alloc 

    err_error (( ERR_OUT, "TRACE => EUDET - Alloc - Free revious buffer " ));
    
    if ( VPtCont->RunCont.PtFrame != NULL ) {
      free ( VPtCont->RunCont.PtFrame );
    }
    
    VPtCont->RunCont.InfFrameBuffSz = 0;
    
    VMaxFrameSz = ( sizeof ( EFRIO__TFrame ) + ( VPtCont->RunCont.ParMi26Nb * MI26__ZS_FFRAME_RAW_MAX_W8 ) + sizeof ( EFRIO__TTriggerRec ) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );
    
    VPtCont->RunCont.InfFrameBuffSz = VPtCont->RunCont.ParFrameNbPerAcq * VMaxFrameSz;
    
    err_error (( ERR_OUT, "TRACE => EUDET VMaxFrameSz    = %d Bytes", VMaxFrameSz ));
    err_error (( ERR_OUT, "TRACE => EUDET InfFrameBuffSz = %d Bytes", VPtCont->RunCont.InfFrameBuffSz ));

    err_error (( ERR_OUT, "TRACE => EUDET - Alloc - Alloc new buffer" ));
    
    VPtCont->RunCont.PtFrame = (EFRIO__TFrame*) malloc ( VPtCont->RunCont.InfFrameBuffSz );
    
    err_retnull ( VPtCont->RunCont.PtFrame, (ERR_OUT,"Allocation of EUDET buffer for %d frames failed !", VPtCont->RunCont.ParFrameNbPerAcq) );
    
    err_error (( ERR_OUT, "TRACE => EUDET - end" ));
    
  }
  
  // Reset run context results fields
      
  VPtCont->RunCont.ResAcqCnt    = 0;
  VPtCont->RunCont.ResFrameCnt  = 0;
  VPtCont->RunCont.ResEventCnt  = 0;
  
  // Set status conf done
  
  VPtCont->ABoardsStatus[0].ConfDone = 1;
  
  // Set flag init done
  
  VPtCont->InfInitDone = 1;
  
  // Init for Labview because some emulation parameters are not controlled by GUI
  
  #ifdef APP_DLL
    EFRIO__FEmuleBegin ( 1 /* RunInLabview */ );
  #endif
  
  
  err_retok (( ERR_OUT, "end" ));
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
Date      : 09/08/2010
Doc date  : 09/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FPrintRunContRec ( EFRIO__TRunCont* PtRec ) {

  err_retnull ( PtRec, (ERR_OUT,"Abort : PtRec == NULL") );

  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "Run context record" ));
  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "ParMi26Nb                 = %.4d", PtRec->ParMi26Nb ));
  msg (( MSG_OUT, "ParFrameNbPerAcq          = %.4d", PtRec->ParFrameNbPerAcq ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "ParRunNo                  = %.4d", PtRec->ParRunNo ));
  msg (( MSG_OUT, "ParTotEvNb                = %.4d", PtRec->ParTotEvNb ));
  msg (( MSG_OUT, "ParEvNbPerFile            = %.4d", PtRec->ParEvNbPerFile ));
  msg (( MSG_OUT, "ParDataTransferMode       = %.4d", PtRec->ParDataTransferMode ));
  msg (( MSG_OUT, "ParSaveOnDisk             = %.4d", PtRec->ParSaveOnDisk ));
  msg (( MSG_OUT, "ParSendOnEth              = %.4d", PtRec->ParSendOnEth ));
  msg (( MSG_OUT, "ParSendOnEthPCent         = %.4d", PtRec->ParSendOnEthPCent ));
  msg (( MSG_OUT, "ParDestDir                = %s"  , PtRec->ParDestDir ));
  msg (( MSG_OUT, "ParFileNamePrefix         = %s"  , PtRec->ParFileNamePrefix ));
  msg (( MSG_OUT, "InfMi26FrameSzFromFlexRio = %.4d", PtRec->InfMi26FrameSzFromFlexRio ));
  msg (( MSG_OUT, "InfZsFFrameRawBuffSz      = %.4d", PtRec->InfZsFFrameRawBuffSz ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "ResDataFileName           = %s"  , PtRec->ResDataFileName        ));
  msg (( MSG_OUT, "ResAcqCnt                 = %.4d", PtRec->ResAcqCnt           ));
  msg (( MSG_OUT, "ResFrameCnt               = %.4d", PtRec->ResFrameCnt         ));
  msg (( MSG_OUT, "ResEventCnt               = %.4d", PtRec->ResEventCnt         ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "PtZsFFrameRaw             = %.8x", PtRec->PtZsFFrameRaw ));
  msg (( MSG_OUT, "============================================================" ));

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
Date      : 09/08/2010
Doc date  : 09/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FPrintRunCont () {

  return ( EFRIO__FPrintRunContRec (&EFRIO__VGContext.RunCont) ); 
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
Date      : 09/08/2010
Doc date  : 09/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */
  

SInt32 EFRIO__FPrintBoardConfRec ( EFRIO__TBoardConf* PtRec ) {

  err_retnull ( PtRec, (ERR_OUT,"Abort : PtRec == NULL") );

  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "Board conf record" ));
  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "BoardId                 = %.4d", PtRec->BoardId ));
  msg (( MSG_OUT, "AsicName                = %s", PtRec->AsicName ));
  msg (( MSG_OUT, "AsicNb                  = %.4d", PtRec->AsicNb ));
  msg (( MSG_OUT, "ReadoutMode             = %.4d", PtRec->ReadoutMode ));
  msg (( MSG_OUT, "DataClkFrequency        = %2.f", PtRec->DataClkFrequency ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "DmaHostSz               = %.4d", PtRec->DmaHostSz ));
  msg (( MSG_OUT, "FrameNbPerAcq           = %.4d", PtRec->FrameNbPerAcq ));
  msg (( MSG_OUT, "EnableExtraChannel      = %.4d", PtRec->EnableExtraChannel ));
  msg (( MSG_OUT, "AcqNbPerTrig            = %.4d", PtRec->AcqNbPerTrig ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "TriggerMode             = %.4d", PtRec->TriggerMode ));
  msg (( MSG_OUT, "TriggerDetectTimeWindow = %.4d", PtRec->TriggerDetectTimeWindow ));
  msg (( MSG_OUT, "TriggerDetectOccurNb    = %.4d", PtRec->TriggerDetectOccurNb ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "TimeStampRes            = %.4d", PtRec->TimeStampRes ));
  msg (( MSG_OUT, "EnableTimeStamping      = %.4d", PtRec->EnableTimeStamping ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "EnableTrigCnt           = %.4d", PtRec->EnableTrigCnt ));
  msg (( MSG_OUT, "TagEventsStoredByDUT    = %.4d", PtRec->TagEventsStoredByDUT ));
  msg (( MSG_OUT, "ReadTluTrigCntEachNTrig = %.4d", PtRec->ReadTluTrigCntEachNTrig ));
  msg (( MSG_OUT, "============================================================" ));
  

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
Date      : 09/08/2010
Doc date  : 09/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FPrintBoardConf ( SInt32 BoardId ) {

  EFRIO__CHK_BOARD_ID (BoardId);

  return ( EFRIO__FPrintBoardConfRec ( &EFRIO__VGContext.ABoardsConf[BoardId] ) ); 
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
Date      : 09/08/2010
Doc date  : 09/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FPrintBoardStatusRec ( EFRIO__TBoardStatus* PtRec ) {

  err_retnull ( PtRec, (ERR_OUT,"Abort : PtRec == NULL") );
  
  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "Board status record" ));
  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "BoardId                    = %.4d", PtRec->BoardId ));
  msg (( MSG_OUT, "BoardPresent               = %.4d", PtRec->BoardPresent ));
  msg (( MSG_OUT, "FwLoaded                   = %.4d", PtRec->FwLoaded ));
  msg (( MSG_OUT, "ConfDone                   = %.4d", PtRec->ConfDone ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "StatusCode                 = %.4d", PtRec->StatusCode ));
  msg (( MSG_OUT, "StatusStr                  = %.4d", PtRec->StatusStr ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "ErrorMsgList               = %s", PtRec->ErrorMsgList ));
  msg (( MSG_OUT, "LastErrorMsg               = %s", PtRec->LastErrorMsg ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "RegDmaHostSz               = %.4d", PtRec->RegDmaHostSz ));
  msg (( MSG_OUT, "RegFrameNbPerAcq           = %.4d", PtRec->RegFrameNbPerAcq ));
  msg (( MSG_OUT, "RegEnableExtraChannel      = %.4d", PtRec->RegEnableExtraChannel ));
  msg (( MSG_OUT, "RegAcqNbPerTrig            = %.4d", PtRec->RegAcqNbPerTrig ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "RegTriggerMode             = %.4d", PtRec->RegTriggerMode ));
  msg (( MSG_OUT, "RegTriggerDetectTimeWindow = %.4d", PtRec->RegTriggerDetectTimeWindow ));
  msg (( MSG_OUT, "RegTriggerDetectOccurNb    = %.4d", PtRec->RegTriggerDetectOccurNb ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "RegTimeStampRes            = %.4d", PtRec->RegTimeStampRes ));
  msg (( MSG_OUT, "RegEnableTimeStamping      = %.4d", PtRec->RegEnableTimeStamping ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "RegEnableTrigCnt           = %.4d", PtRec->RegEnableTrigCnt ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "RegTagEventsStoredByDUT    = %.4d", PtRec->RegTagEventsStoredByDUT ));
  msg (( MSG_OUT, "RegReadTluTrigCntEachNTrig = %.4d", PtRec->RegReadTluTrigCntEachNTrig ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "RegTimeStamp               = %.4d", PtRec->RegTimeStamp ));
  msg (( MSG_OUT, "RegTrigCnt                 = %.4d", PtRec->RegTrigCnt ));
  msg (( MSG_OUT, "RegTluTrigCnt              = %.4d", PtRec->RegTluTrigCnt ));
  msg (( MSG_OUT, "============================================================" ));
  
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
Date      : 09/08/2010
Doc date  : 09/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FPrintBoardStatus ( SInt32 BoardId ) {

  EFRIO__CHK_BOARD_ID (BoardId);
    
  return ( EFRIO__FPrintBoardStatusRec ( &EFRIO__VGContext.ABoardsStatus[BoardId] ) );
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
Date      : 31/10/2010
Doc date  : 31/10/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FPrintAcqEmulRec ( EFRIO_TAcqEmul* PtRec ) {
  
  err_retnull ( PtRec, (ERR_OUT,"Abort : PtRec == NULL") );

  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "Acquistion emulation record" ));
  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "ParAcqCycleMs       = %.4d", PtRec->ParAcqCycleMs ));
  msg (( MSG_OUT, "ParEmuleDRamReadMs  = %.4d", PtRec->ParEmuleDRamReadMs ));
  msg (( MSG_OUT, "ParEmuleFunctNo     = %.4d", PtRec->ParEmuleFunctNo ));
  msg (( MSG_OUT, "InfEmuleFuncCmt     = %s"  , PtRec->InfEmuleFuncCmt ));
  msg (( MSG_OUT, "ParRandomDataSz     = %d"  , PtRec->ParRandomDataSz ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "H [0]=%.8x [1]=%.8x [2]=%.8x [3]=%.8x [4]=%.8x [5]=%.8x", PtRec->ParAHeader[0] , PtRec->ParAHeader[1] , PtRec->ParAHeader[2] , PtRec->ParAHeader[3] , PtRec->ParAHeader[4] , PtRec->ParAHeader[5] ));
  msg (( MSG_OUT, "T [0]=%.8x [1]=%.8x [2]=%.8x [3]=%.8x [4]=%.8x [5]=%.8x", PtRec->ParATrailer[0], PtRec->ParATrailer[1], PtRec->ParATrailer[2], PtRec->ParATrailer[3], PtRec->ParATrailer[4], PtRec->ParATrailer[5] ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "ParTrigNbPerFrame            = %.4d", PtRec->ParTrigNbPerFrame ));
  msg (( MSG_OUT, "ParTrigOnOneFrameOverN       = %.4d", PtRec->ParTrigOnOneFrameOverN ));
  msg (( MSG_OUT, "ParTrigOnNConsecutiveFrames  = %.4d", PtRec->ParTrigOnNConsecutiveFrames ));
  msg (( MSG_OUT, "Trig [0]=%.4d [1]=%.4d [2]=%.4d [Last]=%.4d", PtRec->ParATrig[0], PtRec->ParATrig[1], PtRec->ParATrig[2], PtRec->ParATrig[3] ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "InfDRamSzMb         = %.4d", PtRec->InfDRamSzMb ));
  msg (( MSG_OUT, "InfDRamSz           = %.4d", PtRec->InfDRamSz ));
  msg (( MSG_OUT, "InfDRamPtr          = %.8x", PtRec->InfDRamPtr ));
  msg (( MSG_OUT, "InfExtraChan        = %.4d", PtRec->InfExtraChan ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "ResAcqFunctRetCode  = %.4d", PtRec->ResAcqFunctRetCode ));
  msg (( MSG_OUT, "ResAcqCnt           = %.4d", PtRec->ResAcqCnt ));
  msg (( MSG_OUT, "ResEvCnt            = %.4d", PtRec->ResEvCnt ));
  msg (( MSG_OUT, "============================================================" ));
  
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
Date      : 06/11/2010
Doc date  : 06/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FPrintAcqEmul () {
  
  return ( EFRIO__FPrintAcqEmulRec ( &EFRIO__VGContext.AcqEmul ) );
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
Date      : 31/10/2010
Doc date  : 31/10/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__FPrintFrCheckRec ( EFRIO_TFrCheck* PtRec ) {
  
  err_retnull ( PtRec, (ERR_OUT,"Abort : PtRec == NULL") );
  
  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "Frames check record" ));
  msg (( MSG_OUT, "============================================================" ));
  msg (( MSG_OUT, "ParAcqNo        = %.4d", PtRec->ParAcqNo ));
  msg (( MSG_OUT, "ParFrNo         = %.4d", PtRec->ParFrNo ));
  msg (( MSG_OUT, "ParChkMode      = %.4d", PtRec->ParChkMode ));
  msg (( MSG_OUT, "ParFrPrintLevel = %.4d", PtRec->ParFrPrintLevel ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "InfMi26Nb     = %.4d", PtRec->InfMi26Nb ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "H  [0]=%.8x [1]=%.8x [2]=%.8x [3]=%.8x [4]=%.8x [5]=%.8x", PtRec->ResAHeader[0]   , PtRec->ResAHeader[1]     , PtRec->ResAHeader[2]    , PtRec->ResAHeader[3]    , PtRec->ResAHeader[4]    , PtRec->ResAHeader[5]     ));
  msg (( MSG_OUT, "T  [0]=%.8x [1]=%.8x [2]=%.8x [3]=%.8x [4]=%.8x [5]=%.8x", PtRec->ResATrailer[0]  , PtRec->ResATrailer[1]    , PtRec->ResATrailer[2]   , PtRec->ResATrailer[3]   , PtRec->ResATrailer[4]   , PtRec->ResATrailer[5]    ));
  msg (( MSG_OUT, "FC [0]=%.8d [1]=%.8d [2]=%.8d [3]=%.8d [4]=%.8d [5]=%.8d", PtRec->ResAFrameCnt[0]  , PtRec->ResAFrameCnt[1]  , PtRec->ResAFrameCnt[2]  , PtRec->ResAFrameCnt[3]  , PtRec->ResAFrameCnt[4]  , PtRec->ResAFrameCnt[5]   ));
  msg (( MSG_OUT, "DL [0]=%.8d [1]=%.8d [2]=%.8d [3]=%.8d [4]=%.8d [5]=%.8d", PtRec->ResADataLenght[0], PtRec->ResADataLenght[1], PtRec->ResADataLenght[2], PtRec->ResADataLenght[3], PtRec->ResADataLenght[4], PtRec->ResADataLenght[5] ));
  msg (( MSG_OUT, "------------------------------------------------------------" ));
  msg (( MSG_OUT, "ResTrigNb      = %.4d", PtRec->ResTrigNb ));
  msg (( MSG_OUT, "TRG [0]=%.8d [1]=%.8d [3]=%.8d [Last]=%.8d", PtRec->ResATrig[0], PtRec->ResATrig[1], PtRec->ResATrig[2], PtRec->ResATrig[3] ));
  msg (( MSG_OUT, "============================================================" ));
  
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
Date      : 06/11/2010
Doc date  : 06/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__FPrintFrCheck () {
  
  return ( EFRIO__FPrintFrCheckRec ( &EFRIO__VGContext.FrCheck ) );
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
Date      : 06/11/2010
Doc date  : 06/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

char* EFRIO__FTluTrigger2Str ( SInt32 Trig, char* DestStr, SInt32 MaxDestSz )
{
  static char VStr [30];
  EFRIO__TTluTrigger VTrig;
  
  VTrig.W32 = Trig;
  
  // Convert in string
  
  sprintf ( VStr, "F%.4d - T%.4d", VTrig.F.FrameIdInAcq, VTrig.F.TrigCnt );
  
  // Fill DestStr param if possible
  
  if ( (DestStr != NULL) && (MaxDestSz >= 30) ) {
    sprintf (DestStr, "%s", VStr );
    return (DestStr);
  }
  
  // Return local var if no valid DestStr
  
  return (VStr);  
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
Date      : 06/11/2010
Doc date  : 06/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

char* EFRIO__FTimeStamp2Str ( SInt32 Ts, char* DestStr, SInt32 MaxDestSz )
{
  static char VStr [30];
  EFRIO__TFlexRioTimeStamp1 VTs;
  
  VTs.W32 = Ts;

  // Convert in string
  
  sprintf ( VStr, "F%.4d - L%.4d", VTs.F.Mi26Frame, VTs.F.Mi26Line );

  // Fill DestStr param if possible
  
  if ( (DestStr != NULL) && (MaxDestSz >= 30) ) {
    sprintf ( DestStr, "%s", VStr );
    return (DestStr);
  }
  
  // Return local var if no valid DestStr
  
  return (VStr);
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
Date      : 06/11/2010
Doc date  : 06/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FPrintFrameRec ( EFRIO__TFrame* PtRec, SInt8 PrintLevel ) {
  
  EFRIO__TFrameHeader* VPtHead;
  EFRIO__TFrameData*   VPtData;
  EFRIO__TTriggerRec*  VPtTrig;
  SInt16               ViTrig;
  char                 VStrTrig[30];
  char                 VStrTs  [30];
  
  
  err_retnull ( PtRec, (ERR_OUT,"PtRec == NULL !") );
  
  if ( PrintLevel == 0 ) {
    return (0);
  }
  
  // -------------------------------------
  // Init pointers on TFrame sub records
  // -------------------------------------
  
  VPtHead = &PtRec->Header;
  VPtData = &PtRec->Data;
  
  // Trigger record follows the data record which has a VARIABLE size
    
  VPtTrig = (EFRIO__TTriggerRec*) ( (UInt8*) PtRec + PtRec->TrigRecOffset );

  // -------------------------------------
  // Print TFrame fields
  // -------------------------------------
  
  msg (( MSG_OUT, "==============================================" ));
#ifdef EFRIO__FRAME_TAGS_ENABLE
  msg (( MSG_OUT, "Tag            = %.8X [H]", PtRec->Tag ));
#endif 
  msg (( MSG_OUT, "TotSz          = %.4d [D]", PtRec->TotSz ));
  msg (( MSG_OUT, "TrigRecOffset  = %.4d [D]", PtRec->TrigRecOffset ));
  msg (( MSG_OUT, "----------------------------------------------" ));
#ifdef EFRIO__FRAME_TAGS_ENABLE
  msg (( MSG_OUT, "H.Tag          = %.8X [H]", VPtHead->Tag ));
#endif
  msg (( MSG_OUT, "H.AcqId        = %.4d [D]", VPtHead->AcqId ));
  msg (( MSG_OUT, "H.FrameIdInAcq = %.4d [D]", VPtHead->FrameIdInAcq ));
  msg (( MSG_OUT, "H.MapsName     = %.4d [D]", VPtHead->MapsName ));
  msg (( MSG_OUT, "H.MapsNb       = %.4d [D]", VPtHead->MapsNb ));
  msg (( MSG_OUT, "----------------------------------------------" ));
  msg (( MSG_OUT, "H.Header   [0]=%.8X [1]=%.8X [2]=%.8X [3]=%.8X [4]=%.8X [5]=%.8X"      , VPtHead->AMapsHeader[0]    , VPtHead->AMapsHeader[1]    , VPtHead->AMapsHeader[2]    , VPtHead->AMapsHeader[3]    , VPtHead->AMapsHeader[4]    , VPtHead->AMapsHeader[5]      ));
  msg (( MSG_OUT, "H.FrCnt    [0]=%8d [1]=%8d [2]=%8d [3]=%8d [4]=%8d [5]=%8d", VPtHead->AMapsFrameCnt[0]  , VPtHead->AMapsFrameCnt[1]  , VPtHead->AMapsFrameCnt[2]  , VPtHead->AMapsFrameCnt[3]  , VPtHead->AMapsFrameCnt[4]  , VPtHead->AMapsFrameCnt[5]    ));
  msg (( MSG_OUT, "H.DataSz   [0]=%8d [1]=%8d [2]=%8d [3]=%8d [4]=%8d [5]=%8d", VPtHead->AMapsDataLength[0], VPtHead->AMapsDataLength[1], VPtHead->AMapsDataLength[2], VPtHead->AMapsDataLength[3], VPtHead->AMapsDataLength[4], VPtHead->AMapsDataLength[5] ));
  msg (( MSG_OUT, "H.Trailer  [0]=%.8X [1]=%.8X [2]=%.8X [3]=%.8X [4]=%.8X [5]=%.8X"      , VPtHead->AMapsTrailer[0]   , VPtHead->AMapsTrailer[1]   , VPtHead->AMapsTrailer[2]   , VPtHead->AMapsTrailer[3]   , VPtHead->AMapsTrailer[4]   , VPtHead->AMapsTrailer[5]     ));
  msg (( MSG_OUT, "----------------------------------------------" ));
  msg (( MSG_OUT, "H.TriggerNb    = %.4d [D]", VPtHead->TriggerNb ));
  msg (( MSG_OUT, "H.TrigInfo [0]=%.8X [1]=%.8X [2]=%.8X ", VPtHead->AMapsTrigInfo[0], VPtHead->AMapsTrigInfo[1], VPtHead->AMapsTrigInfo[2] ));
  msg (( MSG_OUT, "----------------------------------------------" ));
#ifdef EFRIO__FRAME_TAGS_ENABLE
  msg (( MSG_OUT, "D.Tag          = %.8X [H]", VPtData->Tag        ));
#endif
  msg (( MSG_OUT, "D.TotSz        = %.4d  [D]", VPtData->TotSz      ));
  msg (( MSG_OUT, "D.OneMapsSz    = %.4d  [D]", VPtData->OneMapsSz  ));

  if ( PrintLevel < 2 ) {
    msg (( MSG_OUT, "==============================================" ));
    return (0);
  }
  
  msg (( MSG_OUT, "----------------------------------------------" ));
#ifdef EFRIO__FRAME_TAGS_ENABLE
  msg (( MSG_OUT, "T.Tag          = %X   [H]", VPtTrig->Tag        ));
#endif
  msg (( MSG_OUT, "T.TotSz        = %.4d  [D]", VPtTrig->TotSz      ));
  msg (( MSG_OUT, "T.TrigNb       = %.4d  [D]", VPtTrig->TrigNb     ));
  msg (( MSG_OUT, "T.TrigType     = %d   [D]", VPtTrig->TrigType   ));
  

  for ( ViTrig=0; ViTrig < VPtTrig->TrigNb; ViTrig++ ) {
    EFRIO__FTluTrigger2Str ( VPtTrig->ATrig[(2 * ViTrig)]    , VStrTrig, 30 /* MaxDestSz */ );
    EFRIO__FTimeStamp2Str  ( VPtTrig->ATrig[(2 * ViTrig) + 1], VStrTs  , 30 /* MaxDestSz */ );
    
    msg (( MSG_OUT, "T.[%.3d] Trig = %s - Ts = %s", ViTrig, VStrTrig, VStrTs ));
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
Remark    : Return the value of the input parameter name DummyS32In ( SInt32 )
          : It can be used to insert this function call on an integer datapath under LabView
:
Level     :
Date      : 09/08/2010
Rev       : 25/10/2010
          : - Implementation -> Empty function before
Doc date  : 09/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FPrintMsg ( SInt32 DummyS32In, SInt8 PrintAsMsg, SInt8 PrintAsTrace, SInt8 PrintAsWarning, SInt8 PrintAsError, char* Msg ) {
  
  if ( PrintAsMsg     ) msg         (( MSG_OUT, "%s", Msg ));
  if ( PrintAsTrace   ) err_trace   (( ERR_OUT, "%s", Msg ));
  if ( PrintAsWarning ) err_warning (( ERR_OUT, "%s", Msg ));
  if ( PrintAsError   ) err_error   (( ERR_OUT, "%s", Msg ));
  
  return (DummyS32In);
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
Remark    : Param AcqId not used NOW
:
Level     :
Date      : 09/08/2010
Rev       : 25/10/2010
          : - Rename EFRIO__FGetZsFFrameRawFields in EFRIO__FGetFrameRawFields
          : - Handle both IPHC "ZsFFrameRaw" and EUDET "Frame"
Doc date  : 09/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


UInt32* EFRIO__FGetFrameFields ( SInt32 AcqId, SInt8 Mi26Id, SInt32 FrameIdInAcq, UInt32* PtDest, SInt8 DestW32Sz ) {
  
  
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*  VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];
  MI26__TZsFFrameRaw* VPtFrame;
  SInt32              ViFrame;
  SInt8               ViField;
  SInt8               VFieldNb = 15;
  EFRIO__TTriggerRec* VPtTrigRec;
  SInt8               ViTrigInf;
  
  static UInt32 VAFields[15];
  
  
  if ( AcqId >= 0 ) {
    err_warning (( ERR_OUT, "AcqId=%d NOT hanlded NOW", AcqId ));
  }
  
  if ( (Mi26Id < 0) ||(Mi26Id >= VPtRunCont->ParMi26Nb) ) {
    err_retfailnull ( -1, (ERR_OUT,"Abort : Bad Mi26Id=%d out of range [0..%d]", Mi26Id, VPtRunCont->ParMi26Nb-1 ) );
  }
  
  if ( (FrameIdInAcq < 0) || (FrameIdInAcq >= VPtRunCont->ParFrameNbPerAcq) ) {
    err_retfailnull ( -1, (ERR_OUT,"Abort : Bad FrameId=%d out of range [0..%d]", FrameIdInAcq, VPtRunCont->ParFrameNbPerAcq-1 ) );
  }

  for ( ViField=0; ViField < VFieldNb; ViField++ ) {
    VAFields[ViField] = 0;
  }


  // Set default values used in case of error

  VAFields [0] = 0x11111111; // Header
  VAFields [1] = 100;        // FrameCnt
  VAFields [2] = 200;        // DataLength
  VAFields [3] = 0x11111111; // Trailer
  VAFields [4] = 0x01;       // Zero
  VAFields [5] = 0x02;       // Zero2
  VAFields [6] = 0;          // Trigger number
  VAFields [7] = 0;          // Trigger 1
  VAFields [8] = 0;          // Trigger 2
  VAFields [9] = 0;          // Trigger 3
  VAFields[10] = 0;          // Last trigger
  VAFields[11] = 0;          // Time stamp 1
  VAFields[12] = 0;          // Time stamp 2
  VAFields[13] = 0;          // Time stamp 3
  VAFields[14] = 0;          // Last stamp


  while (1) {
    
    // IPHC mode
    
    if ( VPtRunCont->ParDataTransferMode == EFRIO__TRF_MODE_IPHC ) {
            
      ViFrame  = (FrameIdInAcq * VPtRunCont->ParMi26Nb) + Mi26Id;
      
      // err_error (( ERR_OUT, "TRACE => EFRIO__TRF_MODE_IPHC - ViFrame = %d", ViFrame ));
      
      VPtFrame = &EFRIO__VGContext.RunCont.PtZsFFrameRaw[ViFrame];
      
      VAFields[0] = VPtFrame->Header;
      VAFields[1] = VPtFrame->FrameCnt;
      VAFields[2] = VPtFrame->DataLength;
      VAFields[3] = VPtFrame->Trailer;
      VAFields[4] = VPtFrame->Zero;
      VAFields[5] = VPtFrame->Zero2;
      VAFields [6] = (VPtFrame->Zero & 0xFFFF0000) >> 16;    // Trigger number
      VAFields [7] = (VPtFrame->Zero & 0x0000FFFF);          // Trigger 1
      VAFields [8] = (VPtFrame->Zero2 & 0xFFFF0000) >> 16;   // Trigger 2
      VAFields [9] = (VPtFrame->Zero2 & 0x0000FFFF);         // Trigger 3
      VAFields[10] = 0;          // Last trigger
      VAFields[11] = 0;          // Time stamp 1
      VAFields[12] = 0;          // Time stamp 2
      VAFields[13] = 0;          // Time stamp 3
      VAFields[14] = 0;          // Last stamp
      
      break;
    }

    // EUDET 1 mode
    
    if ( VPtRunCont->ParDataTransferMode == EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN ) {

      if ( VPtFrList->AFramePtr[FrameIdInAcq] == NULL ) {
        break;
      }

      // err_error (( ERR_OUT, "TRACE => EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN - FrameIdInAcq = %d", FrameIdInAcq ));

      VPtTrigRec = (EFRIO__TTriggerRec*) ( ((UInt8*) VPtFrList->AFramePtr[FrameIdInAcq]) + VPtFrList->AFramePtr[FrameIdInAcq]->TrigRecOffset );
            
      VAFields[0] = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsHeader[Mi26Id];
      VAFields[1] = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsFrameCnt[Mi26Id];
      VAFields[2] = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsDataLength[Mi26Id];
      VAFields[3] = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsTrailer[Mi26Id];
      VAFields[4] = 0; // Zero
      VAFields[5] = 0; // Zero2

      VAFields[6] = VPtTrigRec->TrigNb;
      
      if ( VPtTrigRec->TrigNb > 0 ) {
        VAFields [7] = VPtTrigRec->ATrig[0]; // Trigger 1
        VAFields [8] = VPtTrigRec->ATrig[1]; // Trigger 2
        VAFields [9] = VPtTrigRec->ATrig[2]; // Trigger 3
        VAFields[10] = 0;          // Last trigger
        VAFields[11] = 0;          // Time stamp 1
        VAFields[12] = 0;          // Time stamp 2
        VAFields[13] = 0;          // Time stamp 3
        VAFields[14] = 0;          // Last stamp
      }
      
      else {
        VAFields [7] = 0; // Trigger 1
        VAFields [8] = 0; // Trigger 2
        VAFields [9] = 0; // Trigger 3
        VAFields[10] = 0; // Last trigger
        VAFields[11] = 0; // Time stamp 1
        VAFields[12] = 0; // Time stamp 2
        VAFields[13] = 0; // Time stamp 3
        VAFields[14] = 0; // Last stamp
      }
      

      break;
    }

    // EUDET 2 & 3 mode
    
    if ( (VPtRunCont->ParDataTransferMode == EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES) ||(VPtRunCont->ParDataTransferMode == EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG) ) {
      
      // err_error (( ERR_OUT, "TRACE => EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES - FrameIdInAcq = %d", FrameIdInAcq ));

      if ( VPtFrList->AFramePtr[FrameIdInAcq] == NULL ) {
        if ( AcqId >= 0 ) {
    err_warning (( ERR_OUT, "AcqId=%d NOT hanlded NOW", AcqId ));
  }
  break;
      }
              
      VPtTrigRec = (EFRIO__TTriggerRec*) ( ((UInt8*) VPtFrList->AFramePtr[FrameIdInAcq]) + VPtFrList->AFramePtr[FrameIdInAcq]->TrigRecOffset );
            
      VAFields[0] = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsHeader[Mi26Id];
      VAFields[1] = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsFrameCnt[Mi26Id];
      VAFields[2] = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsDataLength[Mi26Id];
      VAFields[3] = VPtFrList->AFramePtr[FrameIdInAcq]->Header.AMapsTrailer[Mi26Id];
      VAFields[4] = 0; // Zero
      VAFields[5] = 0; // Zero2
      
      VAFields[6] = VPtTrigRec->TrigNb;
       
      if ( VPtTrigRec->TrigNb > 0 ) {
        
        switch ( VPtTrigRec->TrigNb ) {
          
          case 1 : {
            VAFields [7] = VPtTrigRec->ATrig[0]; // Trigger 1
            VAFields[11] = VPtTrigRec->ATrig[1]; // Time stamp 1
            break; }
          
          case 2: {
            VAFields [7] = VPtTrigRec->ATrig[0]; // Trigger 1
            VAFields [8] = VPtTrigRec->ATrig[2]; // Trigger 2
            VAFields[11] = VPtTrigRec->ATrig[1]; // Time stamp 1
            VAFields[12] = VPtTrigRec->ATrig[3]; // Time stamp 2
            break; }
          
          case 3 : {
            VAFields [7] = VPtTrigRec->ATrig[0]; // Trigger 1
            VAFields [8] = VPtTrigRec->ATrig[2]; // Trigger 2
            VAFields [9] = VPtTrigRec->ATrig[4]; // Trigger 3
            VAFields[11] = VPtTrigRec->ATrig[1]; // Time stamp 1
            VAFields[12] = VPtTrigRec->ATrig[3]; // Time stamp 2
            VAFields[13] = VPtTrigRec->ATrig[5]; // Time stamp 3
            break; }
          
          default : {
            VAFields [7] = VPtTrigRec->ATrig[0]; // Trigger 1
            VAFields [8] = VPtTrigRec->ATrig[2]; // Trigger 2
            VAFields [9] = VPtTrigRec->ATrig[4]; // Trigger 3
            VAFields[11] = VPtTrigRec->ATrig[1]; // Time stamp 1
            VAFields[12] = VPtTrigRec->ATrig[3]; // Time stamp 2
            VAFields[13] = VPtTrigRec->ATrig[5]; // Time stamp 3
            break; }          

        } // End switch
        
        if ( VPtTrigRec->TrigNb > 3 ) {
          VAFields[10] = VPtTrigRec->ATrig[(2 * (VPtTrigRec->TrigNb - 1))];     // Last trigger
          VAFields[14] = VPtTrigRec->ATrig[(2 * (VPtTrigRec->TrigNb - 1)) + 1]; // Last time stamp
        }
        
        else {
          VAFields[10] = 0;          // Last trigger
          VAFields[14] = 0;          // Last time stamp
        }
        
      } // End if ( VPtTrigRec->TrigNb > 0 )
      
      else {
        VAFields [7] = 0; // Trigger 1
        VAFields [8] = 0; // Trigger 2
        VAFields [9] = 0; // Trigger 3
        VAFields[10] = 0; // Last trigger
        VAFields[11] = 0; // Time stamp 1
        VAFields[12] = 0; // Time stamp 2
        VAFields[13] = 0; // Time stamp 3
        VAFields[14] = 0; // Last stamp        
      }
      
      break;
    }

    // If case not handled -> Return default values

    VAFields [0] = 0x11111111; // Header
    VAFields [1] = 100;        // FrameCnt
    VAFields [2] = 200;        // DataLength
    VAFields [3] = 0x11111111; // Trailer
    VAFields [4] = 0x01;       // Zero
    VAFields [5] = 0x02;       // Zero2
    VAFields [6] = 0;          // Trigger number
    VAFields [7] = -1;         // Trigger 1
    VAFields [8] = -1;         // Trigger 2
    VAFields [9] = -1;         // Trigger 3
    VAFields[10] = -1;         // Last trigger
    VAFields[11] = -1;         // Time stamp 1
    VAFields[12] = -1;         // Time stamp 2
    VAFields[13] = -1;         // Time stamp 3
    VAFields[14] = -1;         // Last stamp
    break;
  
  } // End while (1)
  
  
  if ( (PtDest != NULL) && (DestW32Sz >= VFieldNb) ) {
    
    for ( ViField=0; ViField < VFieldNb; ViField++ ) {
      PtDest[ViField] = VAFields[ViField];
    }
    
    return (PtDest);
  }
  
  err_warning (( ERR_OUT, "Use local array because : PtDest=%x == NULL or DestW32Sz=%d < 10", PtDest, DestW32Sz ));
  
  return (VAFields);
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
Remark    : Param AcqId not used NOW
:
Level     :
Date      : 06/11/2010
Rev       : 
Doc date  : 
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


EFRIO__TFrame* EFRIO__FGetFramePt ( SInt32 AcqId, SInt32 FrameIdInAcq ) {
  
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*  VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];


  if ( AcqId >= 0 ) {
    err_warning (( ERR_OUT, "AcqId=%d NOT hanlded NOW", AcqId ));
  }
  
  if ( (FrameIdInAcq < 0) || (FrameIdInAcq >= VPtRunCont->ParFrameNbPerAcq) ) {
    err_retfailnull ( -1, (ERR_OUT,"Abort : Bad FrameId=%d out of range [0..%d]", FrameIdInAcq, VPtRunCont->ParFrameNbPerAcq-1 ) );
  }
          
  if ( VPtFrList->AFramePtr[FrameIdInAcq] == NULL ) {
    err_retfailnull ( -1, (ERR_OUT,"No frame=%d in list => Pointer NULL", FrameIdInAcq) );
  }
  
  return ( VPtFrList->AFramePtr[FrameIdInAcq] );
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
Date      : 31/10/2010
Rev       : 
Doc date  : 
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__MI26_FChkAcqIphcMode ( SInt32 PrevErrCnt, SInt8 Verbose ) {
   
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*  VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];
  EFRIO_TAcqEmul*     VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
  MI26__TZsFFrameRaw* VPtFrame;
  SInt32              ViFrame;
  SInt32              VNiFrame;
  SInt32              VNiFramePMi26;
  SInt8               ViMi26;
  SInt32              VErrCnt;
  UInt32              VAFrCnt[EFRIO__MAX_ASIC_NB];


  VErrCnt = 0;
  
  VPtFrame = &EFRIO__VGContext.RunCont.PtZsFFrameRaw[0];

  for ( ViFrame=0; ViFrame < VPtRunCont->ParFrameNbPerAcq; ViFrame++ ) {
    
    VNiFrame = ViFrame * VPtRunCont->ParMi26Nb;
    
    
    for ( ViMi26=0; ViMi26 < VPtRunCont->ParMi26Nb; ViMi26++ ) {
      
      VNiFramePMi26 = VNiFrame + ViMi26;
      
      if ( VPtFrame[VNiFramePMi26].Header != VPtAcqEmul->ParAHeader[ViMi26] ) {
        if ( Verbose ) err_error (( ERR_OUT, "Header error Frame=%d - Mi26=%d : Read %.8x <> JTAG %.8x", ViFrame, ViMi26, VPtFrame[VNiFramePMi26].Header, VPtAcqEmul->ParAHeader[ViMi26] ));
        ++VErrCnt;
      }
      
      if ( VPtFrame[VNiFramePMi26].Trailer != VPtAcqEmul->ParATrailer[ViMi26] ) {
        if ( Verbose ) err_error (( ERR_OUT, "Trailer error Frame=%d - Mi26=%d : Read %.8x <> JTAG %.8x", ViFrame, ViMi26, VPtFrame[VNiFramePMi26].Trailer, VPtAcqEmul->ParATrailer[ViMi26] ));
        ++VErrCnt;
      }

      // Store frame counter of first frame of acq  
      
      if ( ViFrame == 0 ) {
        VAFrCnt[ViMi26] = VPtFrame[VNiFramePMi26].FrameCnt;
      }
      
      else {
        ++VAFrCnt[ViMi26];
        
        if ( VPtFrame[VNiFramePMi26].FrameCnt != VAFrCnt[ViMi26] ) {
          if ( Verbose ) err_error (( ERR_OUT, "Frame cnt error Frame=%d - Mi26=%d : Read %.8d <> %.8d", ViFrame, ViMi26, VPtFrame[VNiFramePMi26].FrameCnt, VAFrCnt[ViMi26] ));
          ++VErrCnt;
        }

      }
    


    }
    
  }
        

  return ( PrevErrCnt + VErrCnt );
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
Date      : 31/10/2010
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


UInt32 EFRIO__MI26_FChkAcqEudetMode ( SInt32 PrevErrCnt, SInt8 Verbose ) {
  
  
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*  VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];
  EFRIO_TAcqEmul*     VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
  EFRIO__TFrame*      VPtFr;
  SInt32              ViFrame;
  EFRIO__TTriggerRec* VPtTrigRec;
  SInt8               ViMi26;
  SInt32              VErrCnt;
  UInt32              VAFrCnt[EFRIO__MAX_ASIC_NB];
  
     
  VErrCnt = 0;
    
  for ( ViFrame=0; ViFrame < VPtRunCont->ParFrameNbPerAcq; ViFrame++ ) {    
    
    VPtFr = VPtFrList->AFramePtr[ViFrame];
    
    for ( ViMi26=0; ViMi26 < VPtRunCont->ParMi26Nb; ViMi26++ ) {
            
      if ( VPtFr->Header.AMapsHeader[ViMi26] != VPtAcqEmul->ParAHeader[ViMi26] ) {
        if ( Verbose ) err_error (( ERR_OUT, "Header error Frame=%d - Mi26=%d : Read %.8x <> JTAG %.8x", ViFrame, ViMi26, VPtFr->Header.AMapsHeader[ViMi26], VPtAcqEmul->ParAHeader[ViMi26] ));
        ++VErrCnt;
      }
      
      if ( VPtFr->Header.AMapsTrailer[ViMi26] != VPtAcqEmul->ParATrailer[ViMi26] ) {
        if ( Verbose ) err_error (( ERR_OUT, "Trailer error Frame=%d - Mi26=%d : Read %.8x <> JTAG %.8x", ViFrame, ViMi26, VPtFr->Header.AMapsTrailer[ViMi26], VPtAcqEmul->ParATrailer[ViMi26] ));
        ++VErrCnt;
      }
      
      
      // Store frame counter of first frame of acq
      
      if ( ViFrame == 0 ) {
        VAFrCnt[ViMi26] = VPtFr->Header.AMapsFrameCnt[ViMi26];
      }
      
      else {
        ++VAFrCnt[ViMi26];
        
        if ( VPtFr->Header.AMapsFrameCnt[ViMi26] != VAFrCnt[ViMi26] ) {
          if ( Verbose ) err_error (( ERR_OUT, "Frame cnt error Frame=%d - Mi26=%d : Read %.8d <> %.8d", ViFrame, ViMi26, VPtFr->Header.AMapsFrameCnt[ViMi26], VAFrCnt[ViMi26] ));
          ++VErrCnt;
        }
        
      }
      
    }
    
  }
  
  
  return ( PrevErrCnt + VErrCnt );
}
  


  


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    :
:
Level     : This is a user level function.
Date      : 09/03/2010
Rev       : 10/03/2010
: - Add parameter PtFirstFrameNo
:
Doc date  : 09/03/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__MI26_FFRioEmulDeserData6Mi26NoEChan ( UInt32* PtDestW32, SInt32 EltNb, SInt32* PtFirstFrameNo, SInt32 FrameNb  ) {
  
  EFRIO_TAcqEmul*      VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
  MI26__TZsFFrameRaw*  VPtZsFFrameRaw;
  SInt32               VFrameNbFromBoardDrv;
  SInt32               ViFrame;
  UInt32               VADataLengthField[6];
  UInt32               VADataLengthW16[6];
  UInt32               VADataLengthW32[6];
  SInt32               ViDestW32;
  SInt32               ViDataW32;
  UInt32*              VPtDataW32Chip0;
  UInt32*              VPtDataW32Chip1;
  UInt32*              VPtDataW32Chip2;
  UInt32*              VPtDataW32Chip3;
  UInt32*              VPtDataW32Chip4;
  UInt32*              VPtDataW32Chip5;
  SInt32               VAEmulDataSzW16[6];
  
  
  
  err_error (( ERR_OUT, "TRACE =>DPXI__MI26_FFRioEmulDeserData6Mi26 (EltNb=%d, FrameNb=%d) !!!", EltNb, FrameNb ));
  
  err_trace (( ERR_OUT, "1" ));
  
  // Pointers parameters check
  
  err_retnull ( PtDestW32, (ERR_OUT,"PtDestW32 = NULL") );
  err_retnull ( PtFirstFrameNo, (ERR_OUT,"PtFirstFrameNo = NULL") );
  
  err_trace (( ERR_OUT, "2" ));
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ) / 6; // It must be => ( (EltNb * 2) / (MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ * 2) );
  // But result is the SAME if we divide both part of fraction by 2 ;-)
  
  err_trace (( ERR_OUT, "3" ));
  
  
  if ( VFrameNbFromBoardDrv != FrameNb ) {
    err_retfail ( -1, (ERR_OUT,"Dest buffer too small : %d frames buffer <> %d frames requested", VFrameNbFromBoardDrv, FrameNb ) );
  }
  
  err_trace (( ERR_OUT, "4" ));
  
  // Alloc ZsFrameRaw
  
  VPtZsFFrameRaw = (MI26__TZsFFrameRaw*) malloc ( 6 * sizeof (MI26__TZsFFrameRaw) );
  
  err_retnull ( VPtZsFFrameRaw, (ERR_OUT,"Alloc ZsFFrameRaw failed !") );
  
  err_trace (( ERR_OUT, "5" ));
  
  // Reset ZsFrameRaw
  
  memset ( VPtZsFFrameRaw, 0, 6 * sizeof (MI26__TZsFFrameRaw) );
  
  err_trace (( ERR_OUT, "6" ));
  
  // Init ZsFrameRaw
  
  // RQ : Emulate trigger only on first Mi26 because it is used for trigger extraction
  // by readout function, information stored in Mi26 [2..5] are ignored
  
  VPtZsFFrameRaw[0].Header     = VPtAcqEmul->ParAHeader[0];
  VPtZsFFrameRaw[0].FrameCnt   = *PtFirstFrameNo;

  if ( VPtAcqEmul->ParSetMaxDataSzOnOneMaps == 1 ) {
    VPtZsFFrameRaw[0].DataLength =  0x023A023A; // Max
  }
  
  else {
    VPtZsFFrameRaw[0].DataLength = 0x00100010;
  }
  
  VPtZsFFrameRaw[0].Trailer    = VPtAcqEmul->ParATrailer[0];
  VPtZsFFrameRaw[0].Zero       = 0x00030010; 
  VPtZsFFrameRaw[0].Zero2      = 0x00200040;
  
  VPtZsFFrameRaw[1].Header     = VPtAcqEmul->ParAHeader[1];
  VPtZsFFrameRaw[1].FrameCnt   = *PtFirstFrameNo;
  VPtZsFFrameRaw[1].DataLength = 0x00200020;
  VPtZsFFrameRaw[1].Trailer    = VPtAcqEmul->ParATrailer[1];
  VPtZsFFrameRaw[1].Zero       = 0x00000000;
  VPtZsFFrameRaw[1].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[2].Header     = VPtAcqEmul->ParAHeader[2];
  VPtZsFFrameRaw[2].FrameCnt   = *PtFirstFrameNo;
  VPtZsFFrameRaw[2].DataLength = 0x00300030;
  VPtZsFFrameRaw[2].Trailer    = VPtAcqEmul->ParATrailer[2];
  VPtZsFFrameRaw[2].Zero       = 0x00000000;
  VPtZsFFrameRaw[2].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[3].Header     = VPtAcqEmul->ParAHeader[3];
  VPtZsFFrameRaw[3].FrameCnt   = *PtFirstFrameNo;
  VPtZsFFrameRaw[3].DataLength = 0x00400040;
  VPtZsFFrameRaw[3].Trailer    = VPtAcqEmul->ParATrailer[3];
  VPtZsFFrameRaw[3].Zero       = 0x00000000;
  VPtZsFFrameRaw[3].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[4].Header     = VPtAcqEmul->ParAHeader[4];
  VPtZsFFrameRaw[4].FrameCnt   = *PtFirstFrameNo;
  VPtZsFFrameRaw[4].DataLength = 0x00500050;
  VPtZsFFrameRaw[4].Trailer    = VPtAcqEmul->ParATrailer[4];
  VPtZsFFrameRaw[4].Zero       = 0x00000000;
  VPtZsFFrameRaw[4].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[5].Header     = VPtAcqEmul->ParAHeader[5];
  VPtZsFFrameRaw[5].FrameCnt   = *PtFirstFrameNo;
  VPtZsFFrameRaw[5].DataLength = 0x00600060;
  VPtZsFFrameRaw[5].Trailer    = VPtAcqEmul->ParATrailer[5];
  VPtZsFFrameRaw[5].Zero       = 0x00000000;
  VPtZsFFrameRaw[5].Zero2      = 0x00000000;
  
  err_trace (( ERR_OUT, "7" ));
  
  err_trace (( ERR_OUT, "Begin emul data loop" ));
  
  // Emul data data
  
  if ( VPtAcqEmul->ParRandomDataSz ) {
    randomize ();
  }
  
  ViDestW32 = 0;
  
  for ( ViFrame=0; ViFrame < FrameNb; ViFrame++ ) {
    
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[0].Header;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[1].Header;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[2].Header;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[3].Header;
    ++ViDestW32;   
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[4].Header;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[5].Header;
    ++ViDestW32;
    
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[0].FrameCnt;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[1].FrameCnt;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[2].FrameCnt;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[3].FrameCnt;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[4].FrameCnt;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[5].FrameCnt;
    ++ViDestW32;
    
    if ( VPtAcqEmul->ParRandomDataSz ) {
      VAEmulDataSzW16[0] = random (571);
      VAEmulDataSzW16[1] = random (571);
      VAEmulDataSzW16[2] = random (571);
      VAEmulDataSzW16[3] = random (571);
      VAEmulDataSzW16[4] = random (571);
      VAEmulDataSzW16[5] = random (571);
      
      VADataLengthField[0] = VAEmulDataSzW16[0] + (VAEmulDataSzW16[0] << 16);
      VADataLengthField[1] = VAEmulDataSzW16[1] + (VAEmulDataSzW16[1] << 16);
      VADataLengthField[2] = VAEmulDataSzW16[2] + (VAEmulDataSzW16[2] << 16);
      VADataLengthField[3] = VAEmulDataSzW16[3] + (VAEmulDataSzW16[3] << 16);
      VADataLengthField[4] = VAEmulDataSzW16[4] + (VAEmulDataSzW16[4] << 16);
      VADataLengthField[5] = VAEmulDataSzW16[5] + (VAEmulDataSzW16[5] << 16);
    }
    
    else {
      VADataLengthField[0] = VPtZsFFrameRaw[0].DataLength;
      VADataLengthField[1] = VPtZsFFrameRaw[1].DataLength;
      VADataLengthField[2] = VPtZsFFrameRaw[2].DataLength;
      VADataLengthField[3] = VPtZsFFrameRaw[3].DataLength;
      VADataLengthField[4] = VPtZsFFrameRaw[4].DataLength;
      VADataLengthField[5] = VPtZsFFrameRaw[5].DataLength;
    }
    
    

    PtDestW32[ViDestW32] = VADataLengthField[0];
    ++ViDestW32;
    
    PtDestW32[ViDestW32] = VADataLengthField[1];
    ++ViDestW32;
    
    PtDestW32[ViDestW32] = VADataLengthField[2];
    ++ViDestW32;
    
    PtDestW32[ViDestW32] = VADataLengthField[3];
    ++ViDestW32;
    
    PtDestW32[ViDestW32] = VADataLengthField[4];
    ++ViDestW32;
    
    PtDestW32[ViDestW32] = VADataLengthField[5];
    ++ViDestW32;
    
    
    
    VADataLengthW16[0] = (VADataLengthField[0] & 0x0000FFFF) + ((VADataLengthField[0] & 0xFFFF0000) >> 16);
    VADataLengthW32[0] = VADataLengthW16[0] / 2;
    
    VADataLengthW16[1] = (VADataLengthField[1] & 0x0000FFFF) + ((VADataLengthField[1] & 0xFFFF0000) >> 16);
    VADataLengthW32[1] = VADataLengthW16[1] / 2;
    
    VADataLengthW16[2] = (VADataLengthField[2] & 0x0000FFFF) + ((VADataLengthField[2] & 0xFFFF0000) >> 16);
    VADataLengthW32[2] = VADataLengthW16[2] / 2;
    
    VADataLengthW16[3] = (VADataLengthField[3] & 0x0000FFFF) + ((VADataLengthField[3] & 0xFFFF0000) >> 16);
    VADataLengthW32[3] = VADataLengthW16[3] / 2;
    
    VADataLengthW16[4] = (VADataLengthField[4] & 0x0000FFFF) + ((VADataLengthField[4] & 0xFFFF0000) >> 16);
    VADataLengthW32[4] = VADataLengthW16[4] / 2;
    
    VADataLengthW16[5] = (VADataLengthField[5] & 0x0000FFFF) + ((VADataLengthField[5] & 0xFFFF0000) >> 16);
    VADataLengthW32[5] = VADataLengthW16[5] / 2;
    
    
    
    VPtDataW32Chip0 = (UInt32*) VPtZsFFrameRaw[0].ADataW16;
    VPtDataW32Chip1 = (UInt32*) VPtZsFFrameRaw[1].ADataW16;
    VPtDataW32Chip2 = (UInt32*) VPtZsFFrameRaw[2].ADataW16;
    VPtDataW32Chip3 = (UInt32*) VPtZsFFrameRaw[3].ADataW16;
    VPtDataW32Chip4 = (UInt32*) VPtZsFFrameRaw[4].ADataW16;
    VPtDataW32Chip5 = (UInt32*) VPtZsFFrameRaw[5].ADataW16;
    
    
    for ( ViDataW32=0; ViDataW32 < MI26__ZS_FFRAME_RAW_MAX_W32; ViDataW32++ ) {
      PtDestW32[ViDestW32] = VPtDataW32Chip0[ViDataW32];
      ++ViDestW32;
      PtDestW32[ViDestW32] = VPtDataW32Chip1[ViDataW32];
      ++ViDestW32;
      PtDestW32[ViDestW32] = VPtDataW32Chip2[ViDataW32];
      ++ViDestW32;
      PtDestW32[ViDestW32] = VPtDataW32Chip3[ViDataW32];
      ++ViDestW32;
      PtDestW32[ViDestW32] = VPtDataW32Chip4[ViDataW32];
      ++ViDestW32;
      PtDestW32[ViDestW32] = VPtDataW32Chip5[ViDataW32];
      ++ViDestW32;
    }
    


    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 6) + 18 + (6* VADataLengthW32[0])    ] = VPtZsFFrameRaw[0].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 6) + 18 + (6* MI26__ZS_FFRAME_RAW_MAX_W32) +  6] = VPtZsFFrameRaw[0].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 6) + 18 + (6* MI26__ZS_FFRAME_RAW_MAX_W32) + 12] = VPtZsFFrameRaw[0].Zero2;
    ++ViDestW32;

    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 6) + 1 + 18 + (6* VADataLengthW32[1])    ] = VPtZsFFrameRaw[1].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 6) + 1 + 18 + (6* MI26__ZS_FFRAME_RAW_MAX_W32) + 6] = VPtZsFFrameRaw[1].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 6) + 1 + 18 + (6* MI26__ZS_FFRAME_RAW_MAX_W32) + 12] = VPtZsFFrameRaw[1].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 6) + 2 + 18 + (6* VADataLengthW32[2])    ] = VPtZsFFrameRaw[2].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 6) + 2 + 18 + (6* MI26__ZS_FFRAME_RAW_MAX_W32) + 6] = VPtZsFFrameRaw[2].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 6) + 2 + 18 + (6* MI26__ZS_FFRAME_RAW_MAX_W32) + 12] = VPtZsFFrameRaw[2].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 6) + 3 + 18 + (6* VADataLengthW32[3])    ] = VPtZsFFrameRaw[3].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 6) + 3 + 18 + (6* MI26__ZS_FFRAME_RAW_MAX_W32) + 6] = VPtZsFFrameRaw[3].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 6) + 3 + 18 + (6* MI26__ZS_FFRAME_RAW_MAX_W32) + 12] = VPtZsFFrameRaw[3].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 6) + 4 + 18 + (6* VADataLengthW32[4])    ] = VPtZsFFrameRaw[4].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 6) + 4 + 18 + (6* MI26__ZS_FFRAME_RAW_MAX_W32) + 6] = VPtZsFFrameRaw[4].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 6) + 4 + 18 + (6* MI26__ZS_FFRAME_RAW_MAX_W32) + 12] = VPtZsFFrameRaw[4].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 6) + 5 + 18 + (6* VADataLengthW32[5])    ] = VPtZsFFrameRaw[5].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 6) + 5 + 18 + (6* MI26__ZS_FFRAME_RAW_MAX_W32) + 6] = VPtZsFFrameRaw[5].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 6) + 5 + 18 + (6* MI26__ZS_FFRAME_RAW_MAX_W32) + 12] = VPtZsFFrameRaw[5].Zero2;
    ++ViDestW32;
    
    
    // Update ZsFrameRaw fields
    
    ++VPtZsFFrameRaw[0].FrameCnt;
    ++VPtZsFFrameRaw[1].FrameCnt;
    ++VPtZsFFrameRaw[2].FrameCnt;
    ++VPtZsFFrameRaw[3].FrameCnt;
    ++VPtZsFFrameRaw[4].FrameCnt;
    ++VPtZsFFrameRaw[5].FrameCnt;
    
  } // End for ViFrame
  
  
  *PtFirstFrameNo = VPtZsFFrameRaw[0].FrameCnt;
  
  err_trace (( ERR_OUT, "End emul data loop" ));
  
  
  // Free ZsFrameRaw
  
  free ( VPtZsFFrameRaw );
  
  err_retok (( ERR_OUT, "MsgOk" ));
  
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
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    :
:
Level     : This is a user level function.
Date      : 29/10/2010
Rev       : 

:
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__MI26_FFRioEmulDeserData6Mi26EudetMode2 ( UInt32* PtDestW32, SInt32 EltNb, SInt32* PtFirstFrameNo, SInt32 FrameNb, SInt16 EmuleMode ) {
  
  EFRIO_TAcqEmul*      VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
  MI26__TZsFFrameRaw*  VPtZsFFrameRaw;
  SInt32               VFrameNbFromBoardDrv;
  SInt32               ViFrame;
  UInt32               VADataLengthField[6];
  UInt32               VADataLengthW16[6];
  UInt32               VADataLengthW32[6];
  SInt32               ViDestW32;
  SInt32               ViDataW32;
  UInt32*              VPtDataW32Chip0;
  UInt32*              VPtDataW32Chip1;
  UInt32*              VPtDataW32Chip2;
  UInt32*              VPtDataW32Chip3;
  UInt32*              VPtDataW32Chip4;
  UInt32*              VPtDataW32Chip5;  
  EFRIO__TTriggerRec*        VPtTrigRec;
  SInt32                     ViEChanTrigField;
  SInt32                     VTrigNbToEmulate;
  SInt32                      VAEmulDataSzW16[6];
  SInt32                      ViTrigToEmulate;
  EFRIO__TTluTrigger*         VPtEmulTrig;
  EFRIO__TFlexRioTimeStamp1*  VPtEmulTs;
  
  
  // err_error (( ERR_OUT, "TRACE =>EFRIO__MI26_FFRioEmulDeserData6Mi26EudetMode2 (EltNb=%d, FrameNb=%d) !!!", EltNb, FrameNb ));
  
  err_trace (( ERR_OUT, "1" ));
  
  // Pointers parameters check
  
  err_retnull ( PtDestW32, (ERR_OUT,"PtDestW32 = NULL") );
  err_retnull ( PtFirstFrameNo, (ERR_OUT,"PtFirstFrameNo = NULL") );
  
  err_trace (( ERR_OUT, "2" ));
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ) / 7; // Divide by 7 because of extral channel
  
  // It must be => ( (EltNb * 2) / (MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ * 2) );
  // But result is the SAME if we divide both part of fraction by 2 ;-)
  
  err_trace (( ERR_OUT, "3" ));
  
  
  if ( VFrameNbFromBoardDrv != FrameNb ) {
    err_retfail ( -1, (ERR_OUT,"Dest buffer too small : %d frames buffer <> %d frames requested", VFrameNbFromBoardDrv, FrameNb ) );
  }
  
  err_trace (( ERR_OUT, "4" ));

  // Calculate nb of trigger to emulate
  
  if ( EmuleMode >= 0 ) {
    VTrigNbToEmulate = 0;
  }
  
  else {
    VTrigNbToEmulate = abs ( EmuleMode );
  }
  

  // Alloc ZsFrameRaw
  
  VPtZsFFrameRaw = (MI26__TZsFFrameRaw*) malloc ( 6 * sizeof (MI26__TZsFFrameRaw) );
  
  err_retnull ( VPtZsFFrameRaw, (ERR_OUT,"Alloc ZsFFrameRaw failed !") );
  
  err_trace (( ERR_OUT, "5" ));
  
  // Alloc trigger record
  
  VPtTrigRec = (EFRIO__TTriggerRec*) malloc ( sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );
  
  err_retnull ( VPtTrigRec, (ERR_OUT,"Allocate tmp trigger buff failed !" ) );
  
  
  // Reset ZsFrameRaw
  
  memset ( VPtZsFFrameRaw, 0, 6 * sizeof (MI26__TZsFFrameRaw) );
  
  err_trace (( ERR_OUT, "6" ));
  
  // Init ZsFrameRaw
  
  // RQ : Emulate trigger only on first Mi26 because it is used for trigger extraction
  // by readout function, information stored in Mi26 [2..5] are ignored
  
  VPtZsFFrameRaw[0].Header     = VPtAcqEmul->ParAHeader[0];
  VPtZsFFrameRaw[0].FrameCnt   = *PtFirstFrameNo;
    
  if ( VPtAcqEmul->ParSetMaxDataSzOnOneMaps == 1 ) {
    VPtZsFFrameRaw[0].DataLength =  0x023A023A; // Max
  }
  
  else {
    VPtZsFFrameRaw[0].DataLength = 0x00100010;
  }
  
  VPtZsFFrameRaw[0].Trailer    = VPtAcqEmul->ParATrailer[0];
  VPtZsFFrameRaw[0].Zero       = (VTrigNbToEmulate << 16); // High W16 = trigger nb
  VPtZsFFrameRaw[0].Zero2      = 0;
  
  VPtZsFFrameRaw[1].Header     = VPtAcqEmul->ParAHeader[1];
  VPtZsFFrameRaw[1].FrameCnt   = *PtFirstFrameNo;
  VPtZsFFrameRaw[1].DataLength = 0x00200020;
  VPtZsFFrameRaw[1].Trailer    = VPtAcqEmul->ParATrailer[1];
  VPtZsFFrameRaw[1].Zero       = 0x00000000;
  VPtZsFFrameRaw[1].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[2].Header     = VPtAcqEmul->ParAHeader[2];
  VPtZsFFrameRaw[2].FrameCnt   = *PtFirstFrameNo;
  VPtZsFFrameRaw[2].DataLength = 0x00300030;
  VPtZsFFrameRaw[2].Trailer    = VPtAcqEmul->ParATrailer[2];
  VPtZsFFrameRaw[2].Zero       = 0x00000000;
  VPtZsFFrameRaw[2].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[3].Header     = VPtAcqEmul->ParAHeader[3];
  VPtZsFFrameRaw[3].FrameCnt   = *PtFirstFrameNo;
  VPtZsFFrameRaw[3].DataLength = 0x00400040;
  VPtZsFFrameRaw[3].Trailer    = VPtAcqEmul->ParATrailer[3];
  VPtZsFFrameRaw[3].Zero       = 0x00000000;
  VPtZsFFrameRaw[3].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[4].Header     = VPtAcqEmul->ParAHeader[4];
  VPtZsFFrameRaw[4].FrameCnt   = *PtFirstFrameNo;
  VPtZsFFrameRaw[4].DataLength = 0x00500050;
  VPtZsFFrameRaw[4].Trailer    = VPtAcqEmul->ParATrailer[4];
  VPtZsFFrameRaw[4].Zero       = 0x00000000;
  VPtZsFFrameRaw[4].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[5].Header     = VPtAcqEmul->ParAHeader[5];
  VPtZsFFrameRaw[5].FrameCnt   = *PtFirstFrameNo;
  VPtZsFFrameRaw[5].DataLength = 0x00600060;
  VPtZsFFrameRaw[5].Trailer    = VPtAcqEmul->ParATrailer[5];
  VPtZsFFrameRaw[5].Zero       = 0x00000000;
  VPtZsFFrameRaw[5].Zero2      = 0x00000000;
  
  err_trace (( ERR_OUT, "7" ));
  
  // Reset trigger record
  
  memset ( VPtTrigRec, 0xFF, sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );

  // Init trigger record
  
  // VPtTrigRec->Tag      -> don't care in this case
  // VPtTrigRec->TotSz    -> don't care in this case
  // VPtTrigRec->TrigNb   -> don't care in this case
  // VPtTrigRec->TrigType -> don't care in this case
      
  // Fill all used trigger fields with 0, because :
  // - More than 4 triggers be emulated BUT only 4 will be filled with a significant value
  // - The first trigger field at -1 will stop trigger extraction by DAQ
  // Therefore, triggers not controlled by GUI must be set at something <> -1, for example 0
  
  // Limit here nb of trigger to emulate to max nb allowed
  
  if ( VTrigNbToEmulate > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ) {
    err_warning (( ERR_OUT, "Request %d trigger > Max = %d => Limit to max", VTrigNbToEmulate, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ));
    VTrigNbToEmulate = EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB;
  }
  
  for ( ViTrigToEmulate=0; ViTrigToEmulate < (2 * VTrigNbToEmulate); ViTrigToEmulate++) {
    VPtTrigRec->ATrig[ViTrigToEmulate] = 0;
  }
  
  // Fill first three trigger info => Trigger + Time stamp
  
  VPtEmulTrig = (EFRIO__TTluTrigger*)        VPtTrigRec->ATrig;
  VPtEmulTs   = (EFRIO__TFlexRioTimeStamp1*) VPtTrigRec->ATrig;
  
  VPtEmulTrig[0].F.TrigCnt  = VPtAcqEmul->ParATrig[0];
  VPtEmulTs  [1].F.Mi26Line = VPtAcqEmul->ParATS[0];
  VPtEmulTrig[2].F.TrigCnt  = VPtAcqEmul->ParATrig[1];
  VPtEmulTs  [3].F.Mi26Line = VPtAcqEmul->ParATS[1];
  VPtEmulTrig[4].F.TrigCnt  = VPtAcqEmul->ParATrig[2];
  VPtEmulTs  [5].F.Mi26Line = VPtAcqEmul->ParATS[2];
  
  // Set last trigger

  if ( VTrigNbToEmulate >= 1 ) {
    VPtEmulTrig = (EFRIO__TTluTrigger*)        &VPtTrigRec->ATrig[2 * (VTrigNbToEmulate-1)];
    VPtEmulTs   = (EFRIO__TFlexRioTimeStamp1*) &VPtTrigRec->ATrig[2 * (VTrigNbToEmulate-1)+1];
    
    VPtEmulTrig->F.TrigCnt  = VPtAcqEmul->ParATrig[3];
    VPtEmulTs->F.Mi26Line   = VPtAcqEmul->ParATS[3];
  }
  

  // Emul data data

  if ( VPtAcqEmul->ParRandomDataSz ) {
    randomize ();
  }

  ViDestW32 = 0;
  
  for ( ViFrame=0; ViFrame < FrameNb; ViFrame++ ) {
    
    ViEChanTrigField = 0;
    
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[0].Header;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[1].Header;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[2].Header;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[3].Header;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[4].Header;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[5].Header;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;    

    PtDestW32[ViDestW32] = VPtZsFFrameRaw[0].FrameCnt;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[1].FrameCnt;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[2].FrameCnt;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[3].FrameCnt;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[4].FrameCnt;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[5].FrameCnt;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    
    
    if ( VPtAcqEmul->ParRandomDataSz ) {
      VAEmulDataSzW16[0] = random (571);
      VAEmulDataSzW16[1] = random (571);
      VAEmulDataSzW16[2] = random (571);
      VAEmulDataSzW16[3] = random (571);
      VAEmulDataSzW16[4] = random (571);
      VAEmulDataSzW16[5] = random (571);
      
      VADataLengthField[0] = VAEmulDataSzW16[0] + (VAEmulDataSzW16[0] << 16);
      VADataLengthField[1] = VAEmulDataSzW16[1] + (VAEmulDataSzW16[1] << 16);
      VADataLengthField[2] = VAEmulDataSzW16[2] + (VAEmulDataSzW16[2] << 16);
      VADataLengthField[3] = VAEmulDataSzW16[3] + (VAEmulDataSzW16[3] << 16);
      VADataLengthField[4] = VAEmulDataSzW16[4] + (VAEmulDataSzW16[4] << 16);
      VADataLengthField[5] = VAEmulDataSzW16[5] + (VAEmulDataSzW16[5] << 16);
    }
    
    else {
      VADataLengthField[0] = VPtZsFFrameRaw[0].DataLength;
      VADataLengthField[1] = VPtZsFFrameRaw[1].DataLength;
      VADataLengthField[2] = VPtZsFFrameRaw[2].DataLength;
      VADataLengthField[3] = VPtZsFFrameRaw[3].DataLength;
      VADataLengthField[4] = VPtZsFFrameRaw[4].DataLength;
      VADataLengthField[5] = VPtZsFFrameRaw[5].DataLength;
    }
    

    PtDestW32[ViDestW32] = VADataLengthField[0];
    ++ViDestW32;
    
    PtDestW32[ViDestW32] = VADataLengthField[1];
    ++ViDestW32;
    
    PtDestW32[ViDestW32] = VADataLengthField[2];
    ++ViDestW32;
    
    PtDestW32[ViDestW32] = VADataLengthField[3];
    ++ViDestW32;
    
    PtDestW32[ViDestW32] = VADataLengthField[4];
    ++ViDestW32;
    
    PtDestW32[ViDestW32] = VADataLengthField[5];
    ++ViDestW32;

    PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;    
    

    VADataLengthW16[0] = (VADataLengthField[0] & 0x0000FFFF) + ((VADataLengthField[0] & 0xFFFF0000) >> 16);
    VADataLengthW32[0] = VADataLengthW16[0] / 2;
    
    VADataLengthW16[1] = (VADataLengthField[1] & 0x0000FFFF) + ((VADataLengthField[1] & 0xFFFF0000) >> 16);
    VADataLengthW32[1] = VADataLengthW16[1] / 2;
    
    VADataLengthW16[2] = (VADataLengthField[2] & 0x0000FFFF) + ((VADataLengthField[2] & 0xFFFF0000) >> 16);
    VADataLengthW32[2] = VADataLengthW16[2] / 2;
    
    VADataLengthW16[3] = (VADataLengthField[3] & 0x0000FFFF) + ((VADataLengthField[3] & 0xFFFF0000) >> 16);
    VADataLengthW32[3] = VADataLengthW16[3] / 2;
    
    VADataLengthW16[4] = (VADataLengthField[4] & 0x0000FFFF) + ((VADataLengthField[4] & 0xFFFF0000) >> 16);
    VADataLengthW32[4] = VADataLengthW16[4] / 2;
    
    VADataLengthW16[5] = (VADataLengthField[5] & 0x0000FFFF) + ((VADataLengthField[5] & 0xFFFF0000) >> 16);
    VADataLengthW32[5] = VADataLengthW16[5] / 2;
    
    
    
    VPtDataW32Chip0 = (UInt32*) VPtZsFFrameRaw[0].ADataW16;
    VPtDataW32Chip1 = (UInt32*) VPtZsFFrameRaw[1].ADataW16;
    VPtDataW32Chip2 = (UInt32*) VPtZsFFrameRaw[2].ADataW16;
    VPtDataW32Chip3 = (UInt32*) VPtZsFFrameRaw[3].ADataW16;
    VPtDataW32Chip4 = (UInt32*) VPtZsFFrameRaw[4].ADataW16;
    VPtDataW32Chip5 = (UInt32*) VPtZsFFrameRaw[5].ADataW16;
    
    
    for ( ViDataW32=0; ViDataW32 < MI26__ZS_FFRAME_RAW_MAX_W32; ViDataW32++ ) {
      PtDestW32[ViDestW32] = VPtDataW32Chip0[ViDataW32];
      ++ViDestW32;
      PtDestW32[ViDestW32] = VPtDataW32Chip1[ViDataW32];
      ++ViDestW32;
      PtDestW32[ViDestW32] = VPtDataW32Chip2[ViDataW32];
      ++ViDestW32;
      PtDestW32[ViDestW32] = VPtDataW32Chip3[ViDataW32];
      ++ViDestW32;
      PtDestW32[ViDestW32] = VPtDataW32Chip4[ViDataW32];
      ++ViDestW32;
      PtDestW32[ViDestW32] = VPtDataW32Chip5[ViDataW32];
      ++ViDestW32;
      PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
      ++ViDestW32;
      ++ViEChanTrigField;    
    }
    
    
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 21 + (7 * VADataLengthW32[0])    ] = VPtZsFFrameRaw[0].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) +  7] = VPtZsFFrameRaw[0].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[0].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 1 + 21 + (7 * VADataLengthW32[1])    ] = VPtZsFFrameRaw[1].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 1 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtZsFFrameRaw[1].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 1 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[1].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 2 + 21 + (7 * VADataLengthW32[2])    ] = VPtZsFFrameRaw[2].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 2 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtZsFFrameRaw[2].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 2 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[2].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 3 + 21 + (7 * VADataLengthW32[3])    ] = VPtZsFFrameRaw[3].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 3 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtZsFFrameRaw[3].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 3 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[3].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 4 + 21 + (7 * VADataLengthW32[4])    ] = VPtZsFFrameRaw[4].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 4 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtZsFFrameRaw[4].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 4 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[4].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 5 + 21 + (7 * VADataLengthW32[5])    ] = VPtZsFFrameRaw[5].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 5 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtZsFFrameRaw[5].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 5 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[5].Zero2;
    ++ViDestW32;

    // PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 6 + 21 + (7 * VADataLengthW32[5])    ] = VPtTrigRec->ATrig[ViEChanTrigField];;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 6 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32)    ] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 6 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 6 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    
    
    // Update ZsFrameRaw fields
    
    ++VPtZsFFrameRaw[0].FrameCnt;
    ++VPtZsFFrameRaw[1].FrameCnt;
    ++VPtZsFFrameRaw[2].FrameCnt;
    ++VPtZsFFrameRaw[3].FrameCnt;
    ++VPtZsFFrameRaw[4].FrameCnt;
    ++VPtZsFFrameRaw[5].FrameCnt;
    
  } // End for ViFrame
  
  
  *PtFirstFrameNo = VPtZsFFrameRaw[0].FrameCnt;
  
  err_trace (( ERR_OUT, "End emul data loop" ));
  
  
  // Free ZsFrameRaw
  
  free ( VPtZsFFrameRaw );
  
  err_retok (( ERR_OUT, "MsgOk" ));
  
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
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    :
:
Level     : This is a user level function.
Date      : 03/11/2010
Rev       :

:
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__MI26_FFRioEmulDeserData6Mi26EudetMode3 ( UInt32* PtDestW32, SInt32 EltNb, SInt32* PtFirstFrameNo, SInt32 FrameNb, SInt16 EmuleMode ) {
  
  EFRIO_TAcqEmul*      VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
  MI26__TZsFFrameRaw*  VPtZsFFrameRaw;
  SInt32               VFrameNbFromBoardDrv;
  SInt32               ViFrame;
  UInt32               VADataLengthField[6];
  UInt32               VADataLengthW16[6];
  UInt32               VADataLengthW32[6];
  SInt32               ViDestW32;
  SInt32               ViDataW32;
  UInt32*              VPtDataW32Chip0;
  UInt32*              VPtDataW32Chip1;
  UInt32*              VPtDataW32Chip2;
  UInt32*              VPtDataW32Chip3;
  UInt32*              VPtDataW32Chip4;
  UInt32*              VPtDataW32Chip5;
  EFRIO__TTriggerRec*        VPtTrigRec;
  SInt32                     ViEChanTrigField;
  SInt32                     VTrigNbToEmulate;
  SInt32                      VAEmulDataSzW16[6];
  SInt32                      ViTrigToEmulate;
  EFRIO__TTluTrigger*         VPtEmulTrig;
  EFRIO__TFlexRioTimeStamp1*  VPtEmulTs;
  
  
  // err_error (( ERR_OUT, "TRACE =>DPXI__MI26_FFRioEmulDeserData6Mi26 (EltNb=%d, FrameNb=%d) !!!", EltNb, FrameNb ));
  
  err_trace (( ERR_OUT, "1" ));
  
  // Pointers parameters check
  
  err_retnull ( PtDestW32, (ERR_OUT,"PtDestW32 = NULL") );
  err_retnull ( PtFirstFrameNo, (ERR_OUT,"PtFirstFrameNo = NULL") );
  
  err_trace (( ERR_OUT, "2" ));
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ) / 7; // Divide by 7 because of extral channel
  
  // It must be => ( (EltNb * 2) / (MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ * 2) );
  // But result is the SAME if we divide both part of fraction by 2 ;-)
  
  err_trace (( ERR_OUT, "3" ));
  
  
  if ( VFrameNbFromBoardDrv != FrameNb ) {
    err_retfail ( -1, (ERR_OUT,"Dest buffer too small : %d frames buffer <> %d frames requested", VFrameNbFromBoardDrv, FrameNb ) );
  }
  
  err_trace (( ERR_OUT, "4" ));
  
  // Calculate nb of trigger to emulate
  
  if ( EmuleMode >= 0 ) {
    VTrigNbToEmulate = 0;
  }
  
  else {
    VTrigNbToEmulate = abs ( EmuleMode );
  }
  
  
  // Alloc ZsFrameRaw
  
  VPtZsFFrameRaw = (MI26__TZsFFrameRaw*) malloc ( 6 * sizeof (MI26__TZsFFrameRaw) );
  
  err_retnull ( VPtZsFFrameRaw, (ERR_OUT,"Alloc ZsFFrameRaw failed !") );
  
  err_trace (( ERR_OUT, "5" ));
  
  // Alloc trigger record
  
  VPtTrigRec = (EFRIO__TTriggerRec*) malloc ( sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );
  
  err_retnull ( VPtTrigRec, (ERR_OUT,"Allocate tmp trigger buff failed !" ) );
  
  
  // Reset ZsFrameRaw
  
  memset ( VPtZsFFrameRaw, 0, 6 * sizeof (MI26__TZsFFrameRaw) );
  
  err_trace (( ERR_OUT, "6" ));
  
  // Init ZsFrameRaw
  
  // RQ : Emulate trigger only on first Mi26 because it is used for trigger extraction
  // by readout function, information stored in Mi26 [2..5] are ignored
  
  VPtZsFFrameRaw[0].Header     = VPtAcqEmul->ParAHeader[0];
  VPtZsFFrameRaw[0].FrameCnt   = *PtFirstFrameNo;
  
  if ( VPtAcqEmul->ParSetMaxDataSzOnOneMaps == 1 ) {
    VPtZsFFrameRaw[0].DataLength =  0x023A023A; // Max
  }
  
  else {
    VPtZsFFrameRaw[0].DataLength = 0x00100010;
  }

  VPtZsFFrameRaw[0].Trailer    = VPtAcqEmul->ParATrailer[0];
  VPtZsFFrameRaw[0].Zero       = 0; // High W16 = trigger nb => Set in frames loop because it depends on frame Id
  VPtZsFFrameRaw[0].Zero2      = 0;
  
  VPtZsFFrameRaw[1].Header     = VPtAcqEmul->ParAHeader[1];
  VPtZsFFrameRaw[1].FrameCnt   = *PtFirstFrameNo;
  VPtZsFFrameRaw[1].DataLength = 0x00200020;
  VPtZsFFrameRaw[1].Trailer    = VPtAcqEmul->ParATrailer[1];
  VPtZsFFrameRaw[1].Zero       = 0x00000000;
  VPtZsFFrameRaw[1].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[2].Header     = VPtAcqEmul->ParAHeader[2];
  VPtZsFFrameRaw[2].FrameCnt   = *PtFirstFrameNo;
  VPtZsFFrameRaw[2].DataLength = 0x00300030;
  VPtZsFFrameRaw[2].Trailer    = VPtAcqEmul->ParATrailer[2];
  VPtZsFFrameRaw[2].Zero       = 0x00000000;
  VPtZsFFrameRaw[2].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[3].Header     = VPtAcqEmul->ParAHeader[3];
  VPtZsFFrameRaw[3].FrameCnt   = *PtFirstFrameNo;
  VPtZsFFrameRaw[3].DataLength = 0x00400040;
  VPtZsFFrameRaw[3].Trailer    = VPtAcqEmul->ParATrailer[3];
  VPtZsFFrameRaw[3].Zero       = 0x00000000;
  VPtZsFFrameRaw[3].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[4].Header     = VPtAcqEmul->ParAHeader[4];
  VPtZsFFrameRaw[4].FrameCnt   = *PtFirstFrameNo;
  VPtZsFFrameRaw[4].DataLength = 0x00500050;
  VPtZsFFrameRaw[4].Trailer    = VPtAcqEmul->ParATrailer[4];
  VPtZsFFrameRaw[4].Zero       = 0x00000000;
  VPtZsFFrameRaw[4].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[5].Header     = VPtAcqEmul->ParAHeader[5];
  VPtZsFFrameRaw[5].FrameCnt   = *PtFirstFrameNo;
  VPtZsFFrameRaw[5].DataLength = 0x00600060;
  VPtZsFFrameRaw[5].Trailer    = VPtAcqEmul->ParATrailer[5];
  VPtZsFFrameRaw[5].Zero       = 0x00000000;
  VPtZsFFrameRaw[5].Zero2      = 0x00000000;
  
  err_trace (( ERR_OUT, "7" ));
  
  // Reset trigger record
  
  memset ( VPtTrigRec, 0xFF, sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );
  
  // Init trigger record
  
  // VPtTrigRec->Tag      -> don't care in this case
  // VPtTrigRec->TotSz    -> don't care in this case
  // VPtTrigRec->TrigNb   -> don't care in this case
  // VPtTrigRec->TrigType -> don't care in this case
  
  // Fill all used trigger fields with 0, because :
  // - More than 4 triggers be emulated BUT only 4 will be filled with a significant value
  // - The first trigger field at -1 will stop trigger extraction by DAQ
  // Therefore, triggers not controlled by GUI must be set at something <> -1, for example 0
  
  // Limit here nb of trigger to emulate to max nb allowed
  
  if ( VTrigNbToEmulate > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ) {
    err_warning (( ERR_OUT, "Request %d trigger > Max = %d => Limit to max", VTrigNbToEmulate, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ));
    VTrigNbToEmulate = EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB;
  }
  
  for ( ViTrigToEmulate=0; ViTrigToEmulate < (2 * VTrigNbToEmulate); ViTrigToEmulate++) {
    VPtTrigRec->ATrig[ViTrigToEmulate] = 0;
  }
  
  // Fill first three trigger info => Trigger + Time stamp
  
  VPtEmulTrig = (EFRIO__TTluTrigger*)        VPtTrigRec->ATrig;
  VPtEmulTs   = (EFRIO__TFlexRioTimeStamp1*) VPtTrigRec->ATrig;
  
  VPtEmulTrig[0].F.TrigCnt  = VPtAcqEmul->ParATrig[0];
  VPtEmulTs  [1].F.Mi26Line = VPtAcqEmul->ParATS[0];
  VPtEmulTrig[2].F.TrigCnt  = VPtAcqEmul->ParATrig[1];
  VPtEmulTs  [3].F.Mi26Line = VPtAcqEmul->ParATS[1];
  VPtEmulTrig[4].F.TrigCnt  = VPtAcqEmul->ParATrig[2];
  VPtEmulTs  [5].F.Mi26Line = VPtAcqEmul->ParATS[2];
  
  // Set last trigger
  
  if ( VTrigNbToEmulate >= 1 ) {
    VPtEmulTrig = (EFRIO__TTluTrigger*)        &VPtTrigRec->ATrig[2 * (VTrigNbToEmulate-1)];
    VPtEmulTs   = (EFRIO__TFlexRioTimeStamp1*) &VPtTrigRec->ATrig[2 * (VTrigNbToEmulate-1)+1];
    
    VPtEmulTrig->F.TrigCnt  = VPtAcqEmul->ParATrig[3];
    VPtEmulTs->F.Mi26Line   = VPtAcqEmul->ParATS[3];
  }
  
  
  // Emul data data
  
  if ( VPtAcqEmul->ParRandomDataSz ) {
    randomize ();
  }
  
  ViDestW32 = 0;
  
  for ( ViFrame=0; ViFrame < FrameNb; ViFrame++ ) {
    
    ViEChanTrigField = 0;

    EFRIO__FSetFrameIdInTriggerRec ( ViFrame, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB, VPtTrigRec );
    
    // Emulate trigger on one frame over ParTrigOnOneFrameOverN and on ParTrigOnNConsecutiveFrames consecutive frames
    
    if ( (ViFrame % VPtAcqEmul->ParTrigOnOneFrameOverN) <= (VPtAcqEmul->ParTrigOnNConsecutiveFrames - 1) ) {
      VPtZsFFrameRaw[0].Zero = (VTrigNbToEmulate << 16); // High W16 = trigger nb
    }
    
    // othewise => no trigger
    
    else {
      VPtZsFFrameRaw[0].Zero = 0;
    }
    

    PtDestW32[ViDestW32] = VPtZsFFrameRaw[0].Header;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[1].Header;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[2].Header;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[3].Header;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[4].Header;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[5].Header;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[0].FrameCnt;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[1].FrameCnt;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[2].FrameCnt;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[3].FrameCnt;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[4].FrameCnt;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[5].FrameCnt;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    
    
    if ( VPtAcqEmul->ParRandomDataSz ) {
      VAEmulDataSzW16[0] = random (571);
      VAEmulDataSzW16[1] = random (571);
      VAEmulDataSzW16[2] = random (571);
      VAEmulDataSzW16[3] = random (571);
      VAEmulDataSzW16[4] = random (571);
      VAEmulDataSzW16[5] = random (571);
      
      VADataLengthField[0] = VAEmulDataSzW16[0] + (VAEmulDataSzW16[0] << 16);
      VADataLengthField[1] = VAEmulDataSzW16[1] + (VAEmulDataSzW16[1] << 16);
      VADataLengthField[2] = VAEmulDataSzW16[2] + (VAEmulDataSzW16[2] << 16);
      VADataLengthField[3] = VAEmulDataSzW16[3] + (VAEmulDataSzW16[3] << 16);
      VADataLengthField[4] = VAEmulDataSzW16[4] + (VAEmulDataSzW16[4] << 16);
      VADataLengthField[5] = VAEmulDataSzW16[5] + (VAEmulDataSzW16[5] << 16);
    }
    
    else {
      VADataLengthField[0] = VPtZsFFrameRaw[0].DataLength;
      VADataLengthField[1] = VPtZsFFrameRaw[1].DataLength;
      VADataLengthField[2] = VPtZsFFrameRaw[2].DataLength;
      VADataLengthField[3] = VPtZsFFrameRaw[3].DataLength;
      VADataLengthField[4] = VPtZsFFrameRaw[4].DataLength;
      VADataLengthField[5] = VPtZsFFrameRaw[5].DataLength;
    }
    
    
    PtDestW32[ViDestW32] = VADataLengthField[0];
    ++ViDestW32;
    
    PtDestW32[ViDestW32] = VADataLengthField[1];
    ++ViDestW32;
    
    PtDestW32[ViDestW32] = VADataLengthField[2];
    ++ViDestW32;
    
    PtDestW32[ViDestW32] = VADataLengthField[3];
    ++ViDestW32;
    
    PtDestW32[ViDestW32] = VADataLengthField[4];
    ++ViDestW32;
    
    PtDestW32[ViDestW32] = VADataLengthField[5];
    ++ViDestW32;
    
    PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    
    
    VADataLengthW16[0] = (VADataLengthField[0] & 0x0000FFFF) + ((VADataLengthField[0] & 0xFFFF0000) >> 16);
    VADataLengthW32[0] = VADataLengthW16[0] / 2;
    
    VADataLengthW16[1] = (VADataLengthField[1] & 0x0000FFFF) + ((VADataLengthField[1] & 0xFFFF0000) >> 16);
    VADataLengthW32[1] = VADataLengthW16[1] / 2;
    
    VADataLengthW16[2] = (VADataLengthField[2] & 0x0000FFFF) + ((VADataLengthField[2] & 0xFFFF0000) >> 16);
    VADataLengthW32[2] = VADataLengthW16[2] / 2;
    
    VADataLengthW16[3] = (VADataLengthField[3] & 0x0000FFFF) + ((VADataLengthField[3] & 0xFFFF0000) >> 16);
    VADataLengthW32[3] = VADataLengthW16[3] / 2;
    
    VADataLengthW16[4] = (VADataLengthField[4] & 0x0000FFFF) + ((VADataLengthField[4] & 0xFFFF0000) >> 16);
    VADataLengthW32[4] = VADataLengthW16[4] / 2;
    
    VADataLengthW16[5] = (VADataLengthField[5] & 0x0000FFFF) + ((VADataLengthField[5] & 0xFFFF0000) >> 16);
    VADataLengthW32[5] = VADataLengthW16[5] / 2;
    
    
    
    VPtDataW32Chip0 = (UInt32*) VPtZsFFrameRaw[0].ADataW16;
    VPtDataW32Chip1 = (UInt32*) VPtZsFFrameRaw[1].ADataW16;
    VPtDataW32Chip2 = (UInt32*) VPtZsFFrameRaw[2].ADataW16;
    VPtDataW32Chip3 = (UInt32*) VPtZsFFrameRaw[3].ADataW16;
    VPtDataW32Chip4 = (UInt32*) VPtZsFFrameRaw[4].ADataW16;
    VPtDataW32Chip5 = (UInt32*) VPtZsFFrameRaw[5].ADataW16;
    
    
    for ( ViDataW32=0; ViDataW32 < MI26__ZS_FFRAME_RAW_MAX_W32; ViDataW32++ ) {
      PtDestW32[ViDestW32] = VPtDataW32Chip0[ViDataW32];
      ++ViDestW32;
      PtDestW32[ViDestW32] = VPtDataW32Chip1[ViDataW32];
      ++ViDestW32;
      PtDestW32[ViDestW32] = VPtDataW32Chip2[ViDataW32];
      ++ViDestW32;
      PtDestW32[ViDestW32] = VPtDataW32Chip3[ViDataW32];
      ++ViDestW32;
      PtDestW32[ViDestW32] = VPtDataW32Chip4[ViDataW32];
      ++ViDestW32;
      PtDestW32[ViDestW32] = VPtDataW32Chip5[ViDataW32];
      ++ViDestW32;
      PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
      ++ViDestW32;
      ++ViEChanTrigField;
    }
    
    
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 21 + (7 * VADataLengthW32[0])    ] = VPtZsFFrameRaw[0].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) +  7] = VPtZsFFrameRaw[0].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[0].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 1 + 21 + (7 * VADataLengthW32[1])    ] = VPtZsFFrameRaw[1].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 1 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtZsFFrameRaw[1].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 1 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[1].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 2 + 21 + (7 * VADataLengthW32[2])    ] = VPtZsFFrameRaw[2].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 2 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtZsFFrameRaw[2].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 2 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[2].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 3 + 21 + (7 * VADataLengthW32[3])    ] = VPtZsFFrameRaw[3].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 3 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtZsFFrameRaw[3].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 3 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[3].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 4 + 21 + (7 * VADataLengthW32[4])    ] = VPtZsFFrameRaw[4].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 4 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtZsFFrameRaw[4].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 4 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[4].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 5 + 21 + (7 * VADataLengthW32[5])    ] = VPtZsFFrameRaw[5].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 5 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtZsFFrameRaw[5].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 5 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[5].Zero2;
    ++ViDestW32;
    
    // PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 6 + 21 + (7 * VADataLengthW32[5])    ] = VPtTrigRec->ATrig[ViEChanTrigField];;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 6 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32)    ] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 6 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 6 + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    
    
    // Update ZsFrameRaw fields
    
    ++VPtZsFFrameRaw[0].FrameCnt;
    ++VPtZsFFrameRaw[1].FrameCnt;
    ++VPtZsFFrameRaw[2].FrameCnt;
    ++VPtZsFFrameRaw[3].FrameCnt;
    ++VPtZsFFrameRaw[4].FrameCnt;
    ++VPtZsFFrameRaw[5].FrameCnt;
    
  } // End for ViFrame
  
  
  *PtFirstFrameNo = VPtZsFFrameRaw[0].FrameCnt;
  
  err_trace (( ERR_OUT, "End emul data loop" ));
  
  
  // Free ZsFrameRaw
  
  free ( VPtZsFFrameRaw );
  
  err_retok (( ERR_OUT, "MsgOk" ));
  
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
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    :
:
Level     : This is a user level function.
Date      : 29/10/2010
Rev       :

:
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__MI26_FFRioPrintDataMi26WithEChan ( UInt32* PtSrcW32, SInt32 TotFrameNb, SInt32 FrameNo, SInt32 NbDataW32 ) {

  SInt32 Vi;
  SInt32 ViFirstW32;
  SInt32 ViFrame;
  UInt32 VADlField[6];
  UInt32 VADlW16[6];
  UInt32 VADlW32[6];
  UInt32* Va;
  SInt32  VAiTrailer[6];
  SInt32  ViZ1;
  SInt32  ViZ2;
  
  if ( FrameNo >= TotFrameNb ) {
    err_retfail ( -1, (ERR_OUT,"FrameNo=%d > Tot frame nb=%d", FrameNo, TotFrameNb ) );
  }
  
  ViFirstW32 = FrameNo * MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * 7;
  
  
  Va = &PtSrcW32[ViFirstW32];

  VADlField[0] = Va[14];
  VADlField[1] = Va[15];
  VADlField[2] = Va[16];
  VADlField[3] = Va[17];
  VADlField[4] = Va[18];
  VADlField[5] = Va[19];
  
  for ( Vi=0; Vi < 6; Vi++ ) {
    VADlW16[Vi]    = ( (VADlField[Vi] & 0xFFFF0000) >> 16 ) + (VADlField[Vi] & 0x0000FFFF);
    VADlW32[Vi]    = VADlW16[Vi] / 2;
    VAiTrailer[Vi] = 21 + ( VADlW32[Vi] * 7 ) + Vi;
  }
  
  ViZ1 = 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) +  7; // 21 = (H + FC + DL) x 7 chips
  ViZ2 = 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 14;
  
  
  
  msg (( MSG_OUT, "===============================================================================================================================" ));
  msg (( MSG_OUT, "H  [0]=%.8x [1]=%.8x [2]=%.8x [3]=%.8x [4]=%.8x [5]=%.8x (H) - E=%.8d (D)", Va[ 0], Va[ 1], Va[ 2], Va[ 3], Va[ 4], Va[ 5], Va[ 6] ));
  msg (( MSG_OUT, "FC [0]=%.8d [1]=%.8d [2]=%.8d [3]=%.8d [4]=%.8d [5]=%.8d (H) - E=%.8d (D)", Va[ 7], Va[ 8], Va[ 9], Va[10], Va[11], Va[12], Va[13] ));
  msg (( MSG_OUT, "DL [0]=%.8x [1]=%.8x [2]=%.8x [3]=%.8x [4]=%.8x [5]=%.8x (H) - E=%.8d (D)", Va[14], Va[15], Va[16], Va[17], Va[18], Va[19], Va[20] ));

  msg (( MSG_OUT, "T  [0]=%.8x [1]=%.8x [2]=%.8x [3]=%.8x [4]=%.8x [5]=%.8x (H)", Va[VAiTrailer[0]], Va[VAiTrailer[1]], Va[VAiTrailer[2]], Va[VAiTrailer[3]], Va[VAiTrailer[4]], Va[VAiTrailer[5]] ));
  

  msg (( MSG_OUT, "Z1 [0]=%.8x [1]=%.8x [2]=%.8x [3]=%.8x [4]=%.8x [5]=%.8x (H) - E=%.8d (D)", Va[ViZ1], Va[ViZ1+1], Va[ViZ1+2], Va[ViZ1+3], Va[ViZ1+4], Va[ViZ1+5], Va[ViZ1+6] ));
  msg (( MSG_OUT, "Z2 [0]=%.8x [1]=%.8x [2]=%.8x [3]=%.8x [4]=%.8x [5]=%.8x (H) - E=%.8d (D)", Va[ViZ2], Va[ViZ2+1], Va[ViZ2+2], Va[ViZ2+3], Va[ViZ2+4], Va[ViZ2+5], Va[ViZ2+6] ));

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
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    : Trigger handling NOT done by this function => To be implemented in fw !
: A crosscheck with sw can be done, but therefore we need on bit of RAM to
: store trigger => acq of 15 Mi26 instead of 16.
:
Level     : This is a user level function.
Date      : 02/02/2010
Rev       : 10/03/2010
: - Add parameter PtFirstFrameNo
          : 26/10/2010
          : - Moved from daq_pxi lib
Doc date  : 02/02/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__MI26_FFRioEmulDeserData1Mi26NoEChan ( UInt32* PtDestW32, SInt32 EltNb, SInt32* PtFirstFrameNo, SInt32 FrameNb  ) {
  
  EFRIO_TAcqEmul*      VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
  MI26__TZsFFrameRaw*  VPtZsFFrameRaw;
  SInt32               VFrameNbFromBoardDrv;
  SInt32               ViFrame;
  UInt32               VDataLengthField;
  UInt32               VDataLengthW16;
  UInt32               VDataLengthW32;
  SInt32               ViDestW32;
  SInt32               ViDataW32;
  UInt32*              VPtDataW32;
  SInt32               VEmulDataSzW16;
  
  
  // err_trace (( ERR_OUT, "DPXI__MI26_FFRioEmulDeserData1Mi26 (P=%x, EltNb=%d, FrameNb=%d)", PtDestW32, EltNb, FrameNb ));
  
  err_trace (( ERR_OUT, "DPXI__MI26_FFRioEmulDeserData1Mi26 (EltNb=%d, FrameNb=%d) !!!", EltNb, FrameNb ));
  
  err_trace (( ERR_OUT, "1" ));
  
  // Pointers parameters check
  
  err_retnull ( PtDestW32     , (ERR_OUT,"PtDestW32 = NULL") );
  err_retnull ( PtFirstFrameNo, (ERR_OUT,"PtFirstFrameNo = NULL") );
  
  err_trace (( ERR_OUT, "2" ));
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ); // It must be => ( (EltNb * 2) / (MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ * 2) );
  // But result is the SAME if we divide both part of fraction by 2 ;-)
  
  err_trace (( ERR_OUT, "3" ));
  
  
  if ( VFrameNbFromBoardDrv != FrameNb ) {
    err_retfail ( -1, (ERR_OUT,"Dest buffer too small : %d frames buffer <> %d frames requested", VFrameNbFromBoardDrv, FrameNb ) );
  }
  
  err_trace (( ERR_OUT, "4" ));
  
  // Alloc ZsFrameRaw
  
  VPtZsFFrameRaw = (MI26__TZsFFrameRaw*) malloc ( sizeof (MI26__TZsFFrameRaw) );
  
  err_retnull ( VPtZsFFrameRaw, (ERR_OUT,"Alloc ZsFFrameRaw failed !") );
  
  err_trace (( ERR_OUT, "5" ));
  
  // Reset ZsFrameRaw
  
  memset ( VPtZsFFrameRaw, 0, sizeof (MI26__TZsFFrameRaw) );
  
  err_trace (( ERR_OUT, "6" ));
  
  // Init ZsFrameRaw
  
  VPtZsFFrameRaw->Header     = VPtAcqEmul->ParAHeader[0];
  VPtZsFFrameRaw->FrameCnt   = *PtFirstFrameNo;
  
  if ( VPtAcqEmul->ParSetMaxDataSzOnOneMaps == 1 ) {
    VPtZsFFrameRaw->DataLength =  0x023A023A; // Max
  }
  
  else {
    VPtZsFFrameRaw->DataLength =  0x00200020;        
  }
  
  VPtZsFFrameRaw->Trailer    = VPtAcqEmul->ParATrailer[0];
  
  VPtZsFFrameRaw->Zero       = 0x00030010;
  VPtZsFFrameRaw->Zero2      = 0x00200040;
  
  
//  VPtZsFFrameRaw->Zero       = 0x00030020;
//  VPtZsFFrameRaw->Zero2      = 0x00400080;
  
  err_trace (( ERR_OUT, "7" ));
  
  err_trace (( ERR_OUT, "Begin emul data loop" ));
  
  
  // Emul data data

  if ( VPtAcqEmul->ParRandomDataSz == 1 ) {
    randomize ();
  }
  

  ViDestW32 = 0;
  
  for ( ViFrame=0; ViFrame < FrameNb; ViFrame++ ) {
      
    if ( VPtAcqEmul->ParRandomDataSz == 1 ) {
      VEmulDataSzW16   = random (571);
      VDataLengthField = VEmulDataSzW16 + (VEmulDataSzW16 << 16);
    }
    
    else {
      VDataLengthField = VPtZsFFrameRaw->DataLength;
    }  
      
    PtDestW32[ViDestW32] = VPtZsFFrameRaw->Header;    
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw->FrameCnt;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VDataLengthField;
    ++ViDestW32;
    
    VDataLengthW16 = (VDataLengthField & 0x0000FFFF) + ((VDataLengthField & 0xFFFF0000) >> 16);
    VDataLengthW32 = VDataLengthW16 / 2;
    
    VPtDataW32 = (UInt32*) VPtZsFFrameRaw->ADataW16;
    
    for ( ViDataW32=0; ViDataW32 < MI26__ZS_FFRAME_RAW_MAX_W32; ViDataW32++ ) {
      PtDestW32[ViDestW32] = VPtDataW32[ViDataW32];
      ++ViDestW32;
    }
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame) + (3 + VDataLengthW32)]     = VPtZsFFrameRaw->Trailer;
    ++ViDestW32;
        
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame) + (3 + MI26__ZS_FFRAME_RAW_MAX_W32 + 1)] = VPtZsFFrameRaw->Zero;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame) + (3 + MI26__ZS_FFRAME_RAW_MAX_W32 + 2)] = VPtZsFFrameRaw->Zero2;
    ++ViDestW32;
    
    // Update ZsFrameRaw fields
    
    ++VPtZsFFrameRaw->FrameCnt;
    
  } // End for ViFrame
  
  
  *PtFirstFrameNo = VPtZsFFrameRaw->FrameCnt;
  
  err_trace (( ERR_OUT, "End emul data loop" ));
  
  
  // Free ZsFrameRaw
  
  free ( VPtZsFFrameRaw );
  
  err_retok (( ERR_OUT, "ViFrame=%d - ViDestW32=%d", ViFrame, ViDestW32 ));
  
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
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    : EmuleMode parameter is used to define number of trigger to emulate
          : - EmuleMode >= 0 -> No trigger
          : - EmuleMode < 0  -> Nb trigger = absolute value of EmuleMode
          :
Level     : This is a user level function.
Date      : 28/10/2010
Rev       : 
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__MI26_FFRioEmulDeserData1Mi26EudetMode2 ( UInt32* PtDestW32, SInt32 EltNb, SInt32* PtFirstFrameNo, SInt32 FrameNb, SInt16 EmuleMode  ) {
  
  EFRIO_TAcqEmul*             VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
  MI26__TZsFFrameRaw*         VPtZsFFrameRaw;
  SInt32                      VFrameNbFromBoardDrv;
  SInt32                      ViFrame;
  UInt32                      VDataLengthField;
  UInt32                      VDataLengthW16;
  UInt32                      VDataLengthW32;
  SInt32                      ViDestW32;
  SInt32                      ViDataW32;
  UInt32*                     VPtDataW32;
  EFRIO__TTriggerRec*         VPtTrigRec;
  SInt32                      ViEChanTrigField;
  SInt32                      VTrigNbToEmulate;
  SInt32                      ViTrigToEmulate;
  SInt32                      VEmulDataSzW16;
  EFRIO__TTluTrigger*         VPtEmulTrig;
  EFRIO__TFlexRioTimeStamp1*  VPtEmulTs;
            
      
  err_trace (( ERR_OUT, "EFRIO__MI26_FFRioEmulDeserData1Mi26EudetMode2 (EltNb=%d, FrameNb=%d) !!!", EltNb, FrameNb ));
    
  // Pointers parameters check
  
  err_retnull ( PtDestW32     , (ERR_OUT,"PtDestW32 = NULL") );
  err_retnull ( PtFirstFrameNo, (ERR_OUT,"PtFirstFrameNo = NULL") );
    
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ / 2 ); // Divide by 2 because of extra channel
   
  // It must be => ( (EltNb * 2) / (MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ * 2) / 2 );
  // But result is the SAME if we divide both part of fraction by 2 ;-)
    
  if ( VFrameNbFromBoardDrv != FrameNb ) { 
    err_retfail ( -1, (ERR_OUT,"Dest buffer too small : %d frames buffer <> %d frames requested", VFrameNbFromBoardDrv, FrameNb ) );
  }
    
  // Calculate nb of trigger to emulate  
    
  if ( EmuleMode >= 0 ) {
    VTrigNbToEmulate = 0;
  }
  
  else {
    VTrigNbToEmulate = abs ( EmuleMode );
  }
    
  
    
  // Alloc ZsFrameRaw
  
  VPtZsFFrameRaw = (MI26__TZsFFrameRaw*) malloc ( sizeof (MI26__TZsFFrameRaw) );
  
  err_retnull ( VPtZsFFrameRaw, (ERR_OUT,"Alloc ZsFFrameRaw failed !") );
  
  // Alloc trigger record
  
  VPtTrigRec = (EFRIO__TTriggerRec*) malloc ( sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );
    
  err_retnull ( VPtTrigRec, (ERR_OUT,"Allocate tmp trigger buff failed !" ) );
      
  // Reset ZsFrameRaw
  
  memset ( VPtZsFFrameRaw, 0, sizeof (MI26__TZsFFrameRaw) );
    
  // Init ZsFrameRaw
  
  VPtZsFFrameRaw->Header     = VPtAcqEmul->ParAHeader[0];
  VPtZsFFrameRaw->FrameCnt   = *PtFirstFrameNo;
  
  if ( VPtAcqEmul->ParSetMaxDataSzOnOneMaps == 1 ) {
    VPtZsFFrameRaw->DataLength =  0x023A023A; // Max
  }

  else {
    VPtZsFFrameRaw->DataLength = 0x00200020;  
  }
  
  VPtZsFFrameRaw->Trailer    = VPtAcqEmul->ParATrailer[0];
  VPtZsFFrameRaw->Zero       = (VTrigNbToEmulate << 16); // High W16 = trigger nb
  VPtZsFFrameRaw->Zero2      = 0;

  // Reset trigger record

  memset ( VPtTrigRec, 0xFF, sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );

  
  // Init trigger record
  
  // VPtTrigRec->Tag      -> don't care in this case 
  // VPtTrigRec->TotSz    -> don't care in this case
  // VPtTrigRec->TrigNb   -> don't care in this case
  // VPtTrigRec->TrigType -> don't care in this case
  

  // Fill all used trigger fields with 0, because :
  // - More than 4 triggers be emulated BUT only 4 will be filled with a significant value
  // - The first trigger field at -1 will stop trigger extraction by DAQ
  // Therefore, triggers not controlled by GUI must be set at something <> -1, for example 0

  // Limit here nb of trigger to emulate to max nb allowed
  
  if ( VTrigNbToEmulate > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ) {
    err_warning (( ERR_OUT, "Request %d trigger > Max = %d => Limit to max", VTrigNbToEmulate, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ));
    VTrigNbToEmulate = EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB;
  }
  

  for ( ViTrigToEmulate=0; ViTrigToEmulate < (2 * VTrigNbToEmulate); ViTrigToEmulate++) {
    VPtTrigRec->ATrig[ViTrigToEmulate] = 0;
  }
  
  // Fill first three trigger info => Trigger + Time stamp
  
  VPtEmulTrig = (EFRIO__TTluTrigger*)        VPtTrigRec->ATrig;
  VPtEmulTs   = (EFRIO__TFlexRioTimeStamp1*) VPtTrigRec->ATrig;

  VPtEmulTrig[0].F.TrigCnt  = VPtAcqEmul->ParATrig[0];
  VPtEmulTs  [1].F.Mi26Line = VPtAcqEmul->ParATS[0];
  VPtEmulTrig[2].F.TrigCnt  = VPtAcqEmul->ParATrig[1];
  VPtEmulTs  [3].F.Mi26Line = VPtAcqEmul->ParATS[1];
  VPtEmulTrig[4].F.TrigCnt  = VPtAcqEmul->ParATrig[2];
  VPtEmulTs  [5].F.Mi26Line = VPtAcqEmul->ParATS[2];

  // Set last trigger

  if ( VTrigNbToEmulate >= 1 ) {
    VPtEmulTrig = (EFRIO__TTluTrigger*)        &VPtTrigRec->ATrig[2 * (VTrigNbToEmulate-1)];
    VPtEmulTs   = (EFRIO__TFlexRioTimeStamp1*) &VPtTrigRec->ATrig[2 * (VTrigNbToEmulate-1)+1];
    
    VPtEmulTrig->F.TrigCnt  = VPtAcqEmul->ParATrig[3];
    VPtEmulTs->F.Mi26Line   = VPtAcqEmul->ParATS[3];
  }

  // Emul data data

  if ( VPtAcqEmul->ParRandomDataSz == 1 ) {
    randomize ();
  }

  ViDestW32        = 0;
  
  
  for ( ViFrame=0; ViFrame < FrameNb; ViFrame++ ) {
  
    ViEChanTrigField = 0;
  
    PtDestW32[ViDestW32] = VPtZsFFrameRaw->Header;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    
    PtDestW32[ViDestW32] = VPtZsFFrameRaw->FrameCnt;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;


    if ( VPtAcqEmul->ParRandomDataSz == 1 ) {
      VEmulDataSzW16   = random (571);
      VDataLengthField = VEmulDataSzW16 + (VEmulDataSzW16 << 16);
    }
    
    else {
      VDataLengthField = VPtZsFFrameRaw->DataLength;
    }
    

    PtDestW32[ViDestW32] = VDataLengthField;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    
    VDataLengthW16 = (VDataLengthField & 0x0000FFFF) + ((VDataLengthField & 0xFFFF0000) >> 16);
    VDataLengthW32 = VDataLengthW16 / 2;
    
    VPtDataW32 = (UInt32*) VPtZsFFrameRaw->ADataW16;
    
    for ( ViDataW32=0; ViDataW32 < MI26__ZS_FFRAME_RAW_MAX_W32; ViDataW32++ ) {
      PtDestW32[ViDestW32] = VPtDataW32[ViDataW32];
      ++ViDestW32;
      PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
      ++ViDestW32;
      ++ViEChanTrigField;    
    }
    
    PtDestW32[(2 * MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame) + (2 * (3 + VDataLengthW32))]     = VPtZsFFrameRaw->Trailer;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    
    PtDestW32[(2 * MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame) + (2 * (3 + MI26__ZS_FFRAME_RAW_MAX_W32 + 1))] = VPtZsFFrameRaw->Zero;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    
    PtDestW32[(2 * MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame) + (2 * (3 + MI26__ZS_FFRAME_RAW_MAX_W32 + 2))] = VPtZsFFrameRaw->Zero2;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    
    // Update ZsFrameRaw fields
    
    ++VPtZsFFrameRaw->FrameCnt;
    
  } // End for ViFrame
  
  
  *PtFirstFrameNo = VPtZsFFrameRaw->FrameCnt;  
  
  // Free ZsFrameRaw
  
  free ( VPtZsFFrameRaw );
  
  // Free trigger record
  
  free ( VPtTrigRec );


  err_retok (( ERR_OUT, "ViFrame=%d - ViDestW32=%d", ViFrame, ViDestW32 ));
}




/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    : EmuleMode parameter is used to define number of trigger to emulate
: - EmuleMode >= 0 -> No trigger
: - EmuleMode < 0  -> Nb trigger = absolute value of EmuleMode
:
Level     : This is a user level function.
Date      : 03/11/2010
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__MI26_FFRioEmulDeserData1Mi26EudetMode3 ( UInt32* PtDestW32, SInt32 EltNb, SInt32* PtFirstFrameNo, SInt32 FrameNb, SInt16 EmuleMode  ) {
  
  EFRIO_TAcqEmul*             VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
  MI26__TZsFFrameRaw*         VPtZsFFrameRaw;
  SInt32                      VFrameNbFromBoardDrv;
  SInt32                      ViFrame;
  UInt32                      VDataLengthField;
  UInt32                      VDataLengthW16;
  UInt32                      VDataLengthW32;
  SInt32                      ViDestW32;
  SInt32                      ViDataW32;
  UInt32*                     VPtDataW32;
  EFRIO__TTriggerRec*         VPtTrigRec;
  SInt32                      ViEChanTrigField;
  SInt32                      VTrigNbToEmulate;
  SInt32                      ViTrigToEmulate;
  SInt32                      VEmulDataSzW16;
  EFRIO__TTluTrigger*         VPtEmulTrig;
  EFRIO__TFlexRioTimeStamp1*  VPtEmulTs;


  err_error (( ERR_OUT, "TRACE => Begin" ));
  
  err_trace (( ERR_OUT, "EFRIO__MI26_FFRioEmulDeserData1Mi26EudetMode3 (EltNb=%d, FrameNb=%d) !!!", EltNb, FrameNb ));
  
  // Pointers parameters check
  
  err_retnull ( PtDestW32     , (ERR_OUT,"PtDestW32 = NULL") );
  err_retnull ( PtFirstFrameNo, (ERR_OUT,"PtFirstFrameNo = NULL") );
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ / 2 ); // Divide by 2 because of extra channel
  
  // It must be => ( (EltNb * 2) / (MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ * 2) / 2 );
  // But result is the SAME if we divide both part of fraction by 2 ;-)
  
  if ( VFrameNbFromBoardDrv != FrameNb ) {
    err_retfail ( -1, (ERR_OUT,"Dest buffer too small : %d frames buffer <> %d frames requested", VFrameNbFromBoardDrv, FrameNb ) );
  }
  
  // Calculate nb of trigger to emulate
  
  if ( EmuleMode >= 0 ) {
    VTrigNbToEmulate = 0;
  }
  
  else {
    VTrigNbToEmulate = abs ( EmuleMode );
  }
  
  
  
  // Alloc ZsFrameRaw
  
  VPtZsFFrameRaw = (MI26__TZsFFrameRaw*) malloc ( sizeof (MI26__TZsFFrameRaw) );
  
  err_retnull ( VPtZsFFrameRaw, (ERR_OUT,"Alloc ZsFFrameRaw failed !") );
  
  // Alloc trigger record
  
  VPtTrigRec = (EFRIO__TTriggerRec*) malloc ( sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );
  
  err_retnull ( VPtTrigRec, (ERR_OUT,"Allocate tmp trigger buff failed !" ) );
  
  // Reset ZsFrameRaw
  
  memset ( VPtZsFFrameRaw, 0, sizeof (MI26__TZsFFrameRaw) );
  
  // Init ZsFrameRaw
  
  VPtZsFFrameRaw->Header     = VPtAcqEmul->ParAHeader[0];
  VPtZsFFrameRaw->FrameCnt   = *PtFirstFrameNo;
  
  if ( VPtAcqEmul->ParSetMaxDataSzOnOneMaps == 1 ) {
    VPtZsFFrameRaw->DataLength =  0x023A023A; // Max
  }
  
  else {
    VPtZsFFrameRaw->DataLength = 0x00200020;
  }  
  
  VPtZsFFrameRaw->Trailer    = VPtAcqEmul->ParATrailer[0];
  VPtZsFFrameRaw->Zero       = 0; // High W16 = trigger nb => Set in frames loop because it depends on frame Id
  VPtZsFFrameRaw->Zero2      = 0;
  
  // Reset trigger record
  
  memset ( VPtTrigRec, 0xFF, sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );
  
  
  // Init trigger record
  
  // VPtTrigRec->Tag      -> don't care in this case
  // VPtTrigRec->TotSz    -> don't care in this case
  // VPtTrigRec->TrigNb   -> don't care in this case
  // VPtTrigRec->TrigType -> don't care in this case
  
  
  // Fill all used trigger fields with 0, because :
  // - More than 4 triggers be emulated BUT only 4 will be filled with a significant value
  // - The first trigger field at -1 will stop trigger extraction by DAQ
  // Therefore, triggers not controlled by GUI must be set at something <> -1, for example 0
  
  // Limit here nb of trigger to emulate to max nb allowed
  
  if ( VTrigNbToEmulate > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ) {
    err_warning (( ERR_OUT, "Request %d trigger > Max = %d => Limit to max", VTrigNbToEmulate, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ));
    VTrigNbToEmulate = EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB;
  }
  
  
  for ( ViTrigToEmulate=0; ViTrigToEmulate < (2 * VTrigNbToEmulate); ViTrigToEmulate++) {
    VPtTrigRec->ATrig[ViTrigToEmulate] = 0;
  }
  
  // Fill first three trigger info => Trigger + Time stamp
  
  VPtEmulTrig = (EFRIO__TTluTrigger*)        VPtTrigRec->ATrig;
  VPtEmulTs   = (EFRIO__TFlexRioTimeStamp1*) VPtTrigRec->ATrig;
  
  VPtEmulTrig[0].F.TrigCnt  = VPtAcqEmul->ParATrig[0];
  VPtEmulTs  [1].F.Mi26Line = VPtAcqEmul->ParATS[0];
  VPtEmulTrig[2].F.TrigCnt  = VPtAcqEmul->ParATrig[1];
  VPtEmulTs  [3].F.Mi26Line = VPtAcqEmul->ParATS[1];
  VPtEmulTrig[4].F.TrigCnt  = VPtAcqEmul->ParATrig[2];
  VPtEmulTs  [5].F.Mi26Line = VPtAcqEmul->ParATS[2];
  
  // Set last trigger
  
  if ( VTrigNbToEmulate >= 1 ) {
    VPtEmulTrig = (EFRIO__TTluTrigger*)        &VPtTrigRec->ATrig[2 * (VTrigNbToEmulate-1)];
    VPtEmulTs   = (EFRIO__TFlexRioTimeStamp1*) &VPtTrigRec->ATrig[2 * (VTrigNbToEmulate-1)+1];
    
    VPtEmulTrig->F.TrigCnt  = VPtAcqEmul->ParATrig[3];
    VPtEmulTs->F.Mi26Line   = VPtAcqEmul->ParATS[3];
  }
  
  // Emul data data
  
  if ( VPtAcqEmul->ParRandomDataSz == 1 ) {
    randomize ();
  }
  
  ViDestW32        = 0;
  
  
//  msg (( MSG_OUT, "********************************************" ));
  
  
  for ( ViFrame=0; ViFrame < FrameNb; ViFrame++ ) {
    
    ViEChanTrigField = 0;
    
    EFRIO__FSetFrameIdInTriggerRec ( ViFrame, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB, VPtTrigRec );
         
    // Emulate trigger on one frame over ParTrigOnOneFrameOverN and on ParTrigOnNConsecutiveFrames consecutive frames
  
    if ( (ViFrame % VPtAcqEmul->ParTrigOnOneFrameOverN) <= (VPtAcqEmul->ParTrigOnNConsecutiveFrames - 1) ) {
      VPtZsFFrameRaw->Zero = (VTrigNbToEmulate << 16); // High W16 = trigger nb
      
//      msg (( MSG_OUT, "Emul => Trig on frame %.4d", ViFrame ));
    }
    
    // otherwise => no trigger
    
    else {
      VPtZsFFrameRaw->Zero = 0;
    }
    
    
    PtDestW32[ViDestW32] = VPtZsFFrameRaw->Header;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    
    PtDestW32[ViDestW32] = VPtZsFFrameRaw->FrameCnt;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    
    
    if ( VPtAcqEmul->ParRandomDataSz == 1 ) {
      VEmulDataSzW16   = random (571);
      VDataLengthField = VEmulDataSzW16 + (VEmulDataSzW16 << 16);
    }
    
    else {
      VDataLengthField = VPtZsFFrameRaw->DataLength;
    }
    
    
    PtDestW32[ViDestW32] = VDataLengthField;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    
    VDataLengthW16 = (VDataLengthField & 0x0000FFFF) + ((VDataLengthField & 0xFFFF0000) >> 16);
    VDataLengthW32 = VDataLengthW16 / 2;
    
    VPtDataW32 = (UInt32*) VPtZsFFrameRaw->ADataW16;
    
    for ( ViDataW32=0; ViDataW32 < MI26__ZS_FFRAME_RAW_MAX_W32; ViDataW32++ ) {
      PtDestW32[ViDestW32] = VPtDataW32[ViDataW32];
      ++ViDestW32;
      PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
      ++ViDestW32;
      ++ViEChanTrigField;
    }
    
    PtDestW32[(2 * MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame) + (2 * (3 + VDataLengthW32))]     = VPtZsFFrameRaw->Trailer;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    
    PtDestW32[(2 * MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame) + (2 * (3 + MI26__ZS_FFRAME_RAW_MAX_W32 + 1))] = VPtZsFFrameRaw->Zero;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    
    PtDestW32[(2 * MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame) + (2 * (3 + MI26__ZS_FFRAME_RAW_MAX_W32 + 2))] = VPtZsFFrameRaw->Zero2;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    
    // Update ZsFrameRaw fields
    
    ++VPtZsFFrameRaw->FrameCnt;
    
  } // End for ViFrame
  
  
  *PtFirstFrameNo = VPtZsFFrameRaw->FrameCnt;
  
  // Free ZsFrameRaw
  
  free ( VPtZsFFrameRaw );
  
  // Free trigger record
  
  free ( VPtTrigRec );
  
  err_error (( ERR_OUT, "TRACE => End" ));
  
  err_retok (( ERR_OUT, "ViFrame=%d - ViDestW32=%d", ViFrame, ViDestW32 ));
}




/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    : Trigger handling NOT done by this function => To be implemented in fw !
: A crosscheck with sw can be done, but therefore we need on bit of RAM to
: store trigger => acq of 15 Mi26 instead of 16.
:
Level     : This is a user level function.
Date      : 02/02/2010
Rev       : 07/05/2010
: - Trigger calculation
: 19/05/2010
: - Add trigger counter field handling in ASIC__TFrameStatus

Doc date  : 02/02/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__MI26_FFRioAcqDeserDataIphcMode1Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt8 AcqStatus, SInt16 TrigStatus, SInt8 DataConvertMode ) {
  
  EFRIO__TContext*     VPtCont  = &EFRIO__VGContext;
  EFRIO__TBoardConf*   VPtBoard = NULL;
    
  SInt32               VAcqId;
  UInt8*               VPtAcqData;
  MI26__TZsFFrameRaw*  VptZsFFrameRaw;
  SInt32               VFrameNbFromBoardDrv;
  SInt32               ViFrame;
  UInt32               VDataLengthField;
  UInt32               VDataLengthW16;
  UInt32               VDataLengthW32;
  SInt32               ViSrcW32;
  SInt32               ViDataW32;
  UInt32*              VPtDataW32;
  SInt32               VRunFrameCnt;
  SInt32               VLastFrameWithTrigAllowed;
  UInt32               VZero;
  UInt32               VZero2;
  SInt32               VTrigLine;
  SInt32               VTrigClk;
  SInt32               VTrigNb;
  

  PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );

  if ( VPtCont->InfInitDone == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort because : Init not done") );
  }
  
  err_trace (( ERR_OUT, "EFRIO__MI26_FFRioAcqDeserDataIphcMode1Mi26 (P=%x, EltNb=%d)", PtSrcW32, EltNb ));
  
  // Pointers parameters check
  
  err_retnull ( PtSrcW32, (ERR_OUT,"PtSrcW32 = NULL") );

  // Init board conf record pointer
  
  if ( (BoardId < 0) || (BoardId >= EFRIO__MAX_BOARDS_NB) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Board Id=%d out of range [0..%d])", BoardId, EFRIO__MAX_BOARDS_NB - 1) );
  }
  
  VPtBoard = &EFRIO__VGContext.ABoardsConf[BoardId];

  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ); // It must be => ( (EltNb * 2) / (MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ * 2) );
  // But result is the SAME if we divide both part of fraction by 2 ;-)
  
  if ( VFrameNbFromBoardDrv != VPtBoard->FrameNbPerAcq ) {
    err_retfail ( -1, (ERR_OUT,"Bad number of samples : %d frames acquired <> %d frames requested", VFrameNbFromBoardDrv, VPtBoard->FrameNbPerAcq ) );
  }

  // Get acq id

  VAcqId = VPtCont->RunCont.ResAcqCnt;

  // Get last frame with trigger allowed / acq
  
  VLastFrameWithTrigAllowed = VPtBoard->FrameNbPerAcq - 1; // Last n frames of Acq MUST have NO trigger information - NOW n = 0 !!!

  // msg (( MSG_OUT, "==> DPXI__MI26_FFRioAcqDeserData1Mi26 : Mi26NbToRead=%d - EltNb=%d - TotAnsSz=%d", VPtServ->PtParAcqReqFunc->AsicNbToRead, EltNb, PtAcq->InfTotAnsSz ));
  
  // Copy Acq inf pointers to local pointers
  
  // $$$ VPtAcqData     = PtAcq->InfPtAcqData;
  // $$$ VptZsFFrameRaw = PtAcq->InfPtZsFFrameRaw;
  // $$$ msg (( MSG_OUT, "DPXI__MI26_FFRioAcqDeserData1Mi26 => AcqReqFuncSz=%d - MaxDataSz=%d - VTotAnsSz=%d", PtAcq->InfParAcqReqFuncSz, VPtServ->PtParAcqReqFunc->MaxDataSz, PtAcq->InfTotAnsSz ));
  // $$$ memset ( VPtAcqData, 0, PtAcq->InfTotAnsSz );
  // $$$ err_trace (( ERR_OUT, "Start extract data for FrameNb=%d", VPtServ->PtParAcqReqFunc->FrameNb ));

  // Init pointer to dest FFrameRaw buffer
  
  VptZsFFrameRaw = VPtCont->RunCont.PtZsFFrameRaw;
  
  // Check if buffer is allocated
  
  err_retnull ( VptZsFFrameRaw, (ERR_OUT,"Abort : FFrameRaw buffer not allocated !") );
  
  // Reset destination FFrameRaw buffer => !!! Can be removed if it makes loosing too much execution time !!!
  
  // memset ( VptZsFFrameRaw, 0, VPtCont->RunCont.InfZsFFrameRawBuffSz );
   
  // Extract data
  
  // $$$ VRunFrameCnt = PtAcq->InfRunFrameCnt;
    
  ViSrcW32 = 0;
  
  for ( ViFrame=0; ViFrame < VPtBoard->FrameNbPerAcq; ViFrame++ ) {
    
    VptZsFFrameRaw[ViFrame].Header     = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VptZsFFrameRaw[ViFrame].FrameCnt   = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VDataLengthField                   = PtSrcW32[ViSrcW32];
    VptZsFFrameRaw[ViFrame].DataLength = VDataLengthField;
    ++ViSrcW32;
    
    VDataLengthW16 = (VDataLengthField & 0x0000FFFF) + ((VDataLengthField & 0xFFFF0000) >> 16);
    VDataLengthW32 = VDataLengthW16 / 2;
    
    // Copy all data fields = We don't care about DataLength field
    // We can decide to optimize later, but NOW I want to get ALL board RAM
    
    VPtDataW32 = (UInt32*) VptZsFFrameRaw[ViFrame].ADataW16;
    
    for ( ViDataW32=0; ViDataW32 < MI26__ZS_FFRAME_RAW_MAX_W32; ViDataW32++ ) {
      VPtDataW32[ViDataW32] = PtSrcW32[ViSrcW32];
      ++ViSrcW32;
    }
    
    VptZsFFrameRaw[ViFrame].Trailer = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame) + (3 + VDataLengthW32)];    // 3 = Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    
    VZero = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame) + (3 + MI26__ZS_FFRAME_RAW_MAX_W32 + 1)];
    VptZsFFrameRaw[ViFrame].Zero = VZero;
    ++ViSrcW32;
    
    VZero2 = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame) + (3 + MI26__ZS_FFRAME_RAW_MAX_W32 + 2)];
    VptZsFFrameRaw[ViFrame].Zero2 = VZero2;
    ++ViSrcW32;
    
    // $$$ DPXI__MI26_FFRioExtractFirstTrigger ( TrigStatus, ViFrame, VLastFrameWithTrigAllowed, VZero, VZero2, &VTrigLine, &VTrigClk, &VTrigNb );
    
    VptZsFFrameRaw[ViFrame].SStatus.AcqNo                                   = VAcqId;
    VptZsFFrameRaw[ViFrame].SStatus.AsicNo                                  = 0;
    VptZsFFrameRaw[ViFrame].SStatus.FrameNoInAcq                            = ViFrame;
    VptZsFFrameRaw[ViFrame].SStatus.FrameNoInRun                            = VRunFrameCnt;
    VptZsFFrameRaw[ViFrame].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_LINE] = VTrigLine;
    VptZsFFrameRaw[ViFrame].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_CLK]  = VTrigClk;
    VptZsFFrameRaw[ViFrame].SStatus.ATrigRes[ASIC__MI26_TRIG_TOT_NB]        = VTrigNb;
    VptZsFFrameRaw[ViFrame].SStatus.HitCnt                                  = -1;
    
    if ( 0 /* VPtServ->PtParAcqReqFunc->AsicChkFunc >= 0 */ ) {
      
      // No neeed to reset ResAsicErrorsRejCurAcq ( it's done on DPXI__MI26_FAsicChkFunc0Emul6Mi26 with ViFrame = 0 )
      // => ResAsicErrorsRejCurAcq = 1 if errors in current Acq
      
      // $$$ PtAcq->ResAsicErrorsRejCurAcq = DPXI__MI26_FAsicChkFuncMi26 ( VPtServ->PtParAcqReqFunc->AsicChkFunc, ViFrame, VptZsFFrameRaw, 1 /* Mi26Nb */ );
      // $$$ PtAcq->ResAsicErrorsRejCurAcq = 0;
      
      // $$$ if ( PtAcq->ResAsicErrorsRejCurAcq != 0 ) {
      // $$$   msg (( MSG_OUT, "ViFrame=%d - VAsicErrorsRejCurAcq=%d", ViFrame, PtAcq->ResAsicErrorsRejCurAcq ));
      // $$$ }
    }
    
    
    ++VRunFrameCnt;
    
  } // End for ViFrame
  

  PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
  
  err_retok (( ERR_OUT, "MsgOk" ));
}




/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    : Trigger handling NOT done by this function => To be implemented in fw !
: A crosscheck with sw can be done, but therefore we need on bit of RAM to
: store trigger => acq of 15 Mi26 instead of 16.
:
Level     : This is a user level function.
Date      : 09/03/2010
Rev       : 06/05/2010
: - Add trigger extract
:   Warning ! Copy Zero & Zero2 fields of frame chip No 0 to ALL others chips !
:             All fields Zero & Zero2 are equals to the fields of first chip
: 19/05/2010
: - Add trigger counter field handling in ASIC__TFrameStatus
Doc date  : 09/03/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

 
SInt32 EFRIO__MI26_FFRioAcqDeserDataIphcMode6Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt8 AcqStatus, SInt16 TrigStatus, SInt8 DataConvertMode ) {
  
  EFRIO__TContext*     VPtCont  = &EFRIO__VGContext;
  EFRIO__TBoardConf*   VPtBoard = NULL;
  
  SInt32               VAcqId;
  UInt8*               VPtAcqData;
  MI26__TZsFFrameRaw*  VptZsFFrameRaw;
  SInt32               VFrameNbFromBoardDrv;
  SInt32               ViFrame;
  SInt32               V6iFrame;
  SInt32               V6iFrameP1;
  SInt32               V6iFrameP2;
  SInt32               V6iFrameP3;
  SInt32               V6iFrameP4;
  SInt32               V6iFrameP5;
  UInt32               VADataLengthField[6];
  UInt16               VADataLengthW16[6];
  UInt16               VADataLengthW32[6];
  UInt16               VDataLengthW32Max;
  register  SInt32               ViSrcW32;
  register  SInt32               ViDataW32;
  UInt32*              VPtDataW32Chip0;
  UInt32*              VPtDataW32Chip1;
  UInt32*              VPtDataW32Chip2;
  UInt32*              VPtDataW32Chip3;
  UInt32*              VPtDataW32Chip4;
  UInt32*              VPtDataW32Chip5;
  SInt32               VRunFrameCnt;
  SInt32               VLastFrameWithTrigAllowed;
  UInt32               VZero;
  UInt32               VZero2;
  SInt32               VTrigLine;
  SInt32               VTrigClk;
  SInt32               VTrigNb;
    
  
  
  PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
  
  if ( VPtCont->InfInitDone == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort because : Init not done") );
  }
  
  err_trace (( ERR_OUT, "EFRIO__MI26_FFRioAcqDeserDataIphcMode6Mi26 (P=%x, EltNb=%d)", PtSrcW32, EltNb ));
  
  // Pointers parameters check
  
  err_retnull ( PtSrcW32, (ERR_OUT,"PtSrcW32 = NULL") );

  // Init board conf record pointer
  
  if ( (BoardId < 0) || (BoardId >= EFRIO__MAX_BOARDS_NB) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Board Id=%d out of range [0..%d])", BoardId, EFRIO__MAX_BOARDS_NB - 1) );
  }
  
  VPtBoard = &EFRIO__VGContext.ABoardsConf[BoardId];
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ) / 6; // It must be => ( (EltNb * 2) / (MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ * 2) );
  // But result is the SAME if we divide both part of fraction by 2 ;-)
  
  if ( VFrameNbFromBoardDrv != VPtBoard->FrameNbPerAcq ) {
    err_retfail ( -1, (ERR_OUT,"Bad number of samples : %d frames acquired <> %d frames requested", VFrameNbFromBoardDrv, VPtBoard->FrameNbPerAcq ) );
  }
  
  // Get AcqId
  
  VAcqId = VPtCont->RunCont.ResAcqCnt;
    
  // Get last frame with trigger allowed / acq
  
  VLastFrameWithTrigAllowed = VPtBoard->FrameNbPerAcq - 1; // Last n frames of Acq MUST have NO trigger information - NOW n = 0 !!!
  

  // Init pointer to dest FFrameRaw buffer
  
  VptZsFFrameRaw = VPtCont->RunCont.PtZsFFrameRaw;
  
  // Check if buffer is allocated
  
  err_retnull ( VptZsFFrameRaw, (ERR_OUT,"Abort : FFrameRaw buffer not allocated !") );
  
  // Reset destination FFrameRaw buffer => !!! Can be removed if it makes loosing too much execution time !!!
  
  // err_warning (( ERR_OUT, "TRACE : Memset buffer"));
  
  // memset ( VptZsFFrameRaw, 0, VPtCont->RunCont.InfZsFFrameRawBuffSz );

  // err_warning (( ERR_OUT, "TRACE : Extract data"));
  
  // Extract data
  
  // VRunFrameCnt = PtAcq->InfRunFrameCnt;
  
  ViSrcW32 = 0;
  
  for ( ViFrame=0; ViFrame < VPtBoard->FrameNbPerAcq; ViFrame++ ) {
    
    // err_warning (( ERR_OUT, "TRACE : Extract frame = %d - 1", ViFrame ));
      
    
    V6iFrame   = 6 * ViFrame;
    V6iFrameP1 = V6iFrame + 1;
    V6iFrameP2 = V6iFrame + 2;
    V6iFrameP3 = V6iFrame + 3;
    V6iFrameP4 = V6iFrame + 4;
    V6iFrameP5 = V6iFrame + 5;
    
    
    VptZsFFrameRaw[V6iFrame].Header     = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP1].Header   = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP2].Header   = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP3].Header   = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP4].Header   = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP5].Header   = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    
    
    VptZsFFrameRaw[V6iFrame].FrameCnt   = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP1].FrameCnt = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP2].FrameCnt = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP3].FrameCnt = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP4].FrameCnt = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP5].FrameCnt = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    
    
    VADataLengthField[0]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[1]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[2]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[3]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[4]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[5]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    
    
    
    VptZsFFrameRaw[V6iFrame].DataLength = VADataLengthField[0];
    
    VADataLengthW16[0] = (VADataLengthField[0] & 0x0000FFFF) + ((VADataLengthField[0] & 0xFFFF0000) >> 16);
    VADataLengthW32[0] = VADataLengthW16[0] / 2;
    
    VptZsFFrameRaw[V6iFrameP1].DataLength = VADataLengthField[1];
    
    VADataLengthW16[1] = (VADataLengthField[1] & 0x0000FFFF) + ((VADataLengthField[1] & 0xFFFF0000) >> 16);
    VADataLengthW32[1] = VADataLengthW16[1] / 2;
    
    
    VptZsFFrameRaw[V6iFrameP2].DataLength = VADataLengthField[2];
    
    VADataLengthW16[2] = (VADataLengthField[2] & 0x0000FFFF) + ((VADataLengthField[2] & 0xFFFF0000) >> 16);
    VADataLengthW32[2] = VADataLengthW16[2] / 2;
    
    VptZsFFrameRaw[V6iFrameP3].DataLength = VADataLengthField[3];
    
    VADataLengthW16[3] = (VADataLengthField[3] & 0x0000FFFF) + ((VADataLengthField[3] & 0xFFFF0000) >> 16);
    VADataLengthW32[3] = VADataLengthW16[3] / 2;
    
    VptZsFFrameRaw[V6iFrameP4].DataLength = VADataLengthField[4];
    
    VADataLengthW16[4] = (VADataLengthField[4] & 0x0000FFFF) + ((VADataLengthField[4] & 0xFFFF0000) >> 16);
    VADataLengthW32[4] = VADataLengthW16[4] / 2;
    
    VptZsFFrameRaw[V6iFrameP5].DataLength = VADataLengthField[5];
    
    VADataLengthW16[5] = (VADataLengthField[5] & 0x0000FFFF) + ((VADataLengthField[5] & 0xFFFF0000) >> 16);
    VADataLengthW32[5] = VADataLengthW16[5] / 2;
    
    // Find max data length of the six Mi26
        
    VDataLengthW32Max = MATH_FUInt16Max ( VADataLengthW32, 6 ); 
        
    
    // err_error (( ERR_OUT, "TRACE => Length W32 [0]=%d - [1]=%d - [2]=%d - [3]=%d - [4]=%d - [5]=%d", VADataLengthW32[0], VADataLengthW32[1], VADataLengthW32[2], VADataLengthW32[3], VADataLengthW32[4], VADataLengthW32[5] ));
    // err_error (( ERR_OUT, "TRACE => MI26__ZS_FFRAME_RAW_MAX_W32 = %d", MI26__ZS_FFRAME_RAW_MAX_W32 ));
    
    // Copy all data fields = We don't care about DataLength field
    // We can decide to optimize later, but NOW I want to get ALL board RAM
    
    VPtDataW32Chip0 = (UInt32*) VptZsFFrameRaw[V6iFrame].ADataW16;
    VPtDataW32Chip1 = (UInt32*) VptZsFFrameRaw[V6iFrameP1].ADataW16;
    VPtDataW32Chip2 = (UInt32*) VptZsFFrameRaw[V6iFrameP2].ADataW16;
    VPtDataW32Chip3 = (UInt32*) VptZsFFrameRaw[V6iFrameP3].ADataW16;
    VPtDataW32Chip4 = (UInt32*) VptZsFFrameRaw[V6iFrameP4].ADataW16;
    VPtDataW32Chip5 = (UInt32*) VptZsFFrameRaw[V6iFrameP5].ADataW16;
    
    // err_warning (( ERR_OUT, "TRACE : Extract frame = %d - 2", ViFrame ));


    for ( ViDataW32=0; ViDataW32 < MI26__ZS_FFRAME_RAW_MAX_W32; ViDataW32++ ) {
      VPtDataW32Chip0[ViDataW32] = PtSrcW32[ViSrcW32];
      ++ViSrcW32;
      VPtDataW32Chip1[ViDataW32] = PtSrcW32[ViSrcW32];
      ++ViSrcW32;
      VPtDataW32Chip2[ViDataW32] = PtSrcW32[ViSrcW32];
      ++ViSrcW32;
      VPtDataW32Chip3[ViDataW32] = PtSrcW32[ViSrcW32];
      ++ViSrcW32;
      VPtDataW32Chip4[ViDataW32] = PtSrcW32[ViSrcW32];
      ++ViSrcW32;
      VPtDataW32Chip5[ViDataW32] = PtSrcW32[ViSrcW32];
      ++ViSrcW32;
    }    
          
    // err_warning (( ERR_OUT, "TRACE : Extract frame = %d - 3", ViFrame ));
    
    VptZsFFrameRaw[V6iFrame].Trailer = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * VADataLengthW32[0])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    
    VZero = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 6];
    VptZsFFrameRaw[V6iFrame].Zero = VZero;
    ++ViSrcW32;
    
    VZero2 = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 12];
    VptZsFFrameRaw[V6iFrame].Zero2 = VZero2;
    ++ViSrcW32;
    
    // DPXI__MI26_FFRioExtractFirstTrigger ( TrigStatus, ViFrame, VLastFrameWithTrigAllowed, VZero, VZero2, &VTrigLine, &VTrigClk, &VTrigNb );

    // err_warning (( ERR_OUT, "TRACE : Extract frame = %d - 4", ViFrame ));
    
    VptZsFFrameRaw[V6iFrame].SStatus.AcqNo                                   = VAcqId;
    VptZsFFrameRaw[V6iFrame].SStatus.AsicNo                                  = 0;
    VptZsFFrameRaw[V6iFrame].SStatus.FrameNoInAcq                            = ViFrame;
    VptZsFFrameRaw[V6iFrame].SStatus.FrameNoInRun                            = VRunFrameCnt;
    VptZsFFrameRaw[V6iFrame].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_LINE] = VTrigLine;
    VptZsFFrameRaw[V6iFrame].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_CLK]  = VTrigClk;
    VptZsFFrameRaw[V6iFrame].SStatus.ATrigRes[ASIC__MI26_TRIG_TOT_NB]        = VTrigNb;
    VptZsFFrameRaw[V6iFrame].SStatus.HitCnt                                  = -1;
    
    
    VptZsFFrameRaw[V6iFrameP1].Trailer = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 1 + 18 + (6 * VADataLengthW32[1])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP1].Zero      = VZero;
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP1].Zero2     = VZero2;
    ++ViSrcW32;
    
    VptZsFFrameRaw[V6iFrameP1].SStatus.AcqNo                                   = VAcqId;
    VptZsFFrameRaw[V6iFrameP1].SStatus.AsicNo                                  = 1;
    VptZsFFrameRaw[V6iFrameP1].SStatus.FrameNoInAcq                            = ViFrame;
    VptZsFFrameRaw[V6iFrameP1].SStatus.FrameNoInRun                            = VRunFrameCnt;
    VptZsFFrameRaw[V6iFrameP1].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_LINE] = VTrigLine;
    VptZsFFrameRaw[V6iFrameP1].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_CLK]  = VTrigClk;
    VptZsFFrameRaw[V6iFrameP1].SStatus.ATrigRes[ASIC__MI26_TRIG_TOT_NB]        = VTrigNb;
    VptZsFFrameRaw[V6iFrameP1].SStatus.HitCnt                                  = -1;
    
    
    VptZsFFrameRaw[V6iFrameP2].Trailer = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 2 + 18 + (6 * VADataLengthW32[2])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP2].Zero      = VZero;
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP2].Zero2     = VZero2;
    ++ViSrcW32;
    
    VptZsFFrameRaw[V6iFrameP2].SStatus.AcqNo                                   = VAcqId;
    VptZsFFrameRaw[V6iFrameP2].SStatus.AsicNo                                  = 2;
    VptZsFFrameRaw[V6iFrameP2].SStatus.FrameNoInAcq                            = ViFrame;
    VptZsFFrameRaw[V6iFrameP2].SStatus.FrameNoInRun                            = VRunFrameCnt;
    VptZsFFrameRaw[V6iFrameP2].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_LINE] = VTrigLine;
    VptZsFFrameRaw[V6iFrameP2].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_CLK]  = VTrigClk;
    VptZsFFrameRaw[V6iFrameP2].SStatus.ATrigRes[ASIC__MI26_TRIG_TOT_NB]        = VTrigNb;
    VptZsFFrameRaw[V6iFrameP2].SStatus.HitCnt                                  = -1;
    
    
    VptZsFFrameRaw[V6iFrameP3].Trailer = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 3 + 18 + (6 * VADataLengthW32[3])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP3].Zero      = VZero;
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP3].Zero2     = VZero2;
    ++ViSrcW32;
    
    VptZsFFrameRaw[V6iFrameP3].SStatus.AcqNo                                   = VAcqId;
    VptZsFFrameRaw[V6iFrameP3].SStatus.AsicNo                                  = 3;
    VptZsFFrameRaw[V6iFrameP3].SStatus.FrameNoInAcq                            = ViFrame;
    VptZsFFrameRaw[V6iFrameP3].SStatus.FrameNoInRun                            = VRunFrameCnt;
    VptZsFFrameRaw[V6iFrameP3].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_LINE] = VTrigLine;
    VptZsFFrameRaw[V6iFrameP3].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_CLK]  = VTrigClk;
    VptZsFFrameRaw[V6iFrameP3].SStatus.ATrigRes[ASIC__MI26_TRIG_TOT_NB]        = VTrigNb;
    VptZsFFrameRaw[V6iFrameP3].SStatus.HitCnt                                  = -1;
    
    
    VptZsFFrameRaw[V6iFrameP4].Trailer = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 4 + 18 + (6 * VADataLengthW32[4])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP4].Zero      = VZero;
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP4].Zero2     = VZero2;
    ++ViSrcW32;
    
    VptZsFFrameRaw[V6iFrameP4].SStatus.AcqNo                                   = VAcqId;
    VptZsFFrameRaw[V6iFrameP4].SStatus.AsicNo                                  = 4;
    VptZsFFrameRaw[V6iFrameP4].SStatus.FrameNoInAcq                            = ViFrame;
    VptZsFFrameRaw[V6iFrameP4].SStatus.FrameNoInRun                            = VRunFrameCnt;
    VptZsFFrameRaw[V6iFrameP4].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_LINE] = VTrigLine;
    VptZsFFrameRaw[V6iFrameP4].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_CLK]  = VTrigClk;
    VptZsFFrameRaw[V6iFrameP4].SStatus.ATrigRes[ASIC__MI26_TRIG_TOT_NB]        = VTrigNb;
    VptZsFFrameRaw[V6iFrameP4].SStatus.HitCnt                                  = -1;
    
    
    VptZsFFrameRaw[V6iFrameP5].Trailer = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 5 + 18 + (6 * VADataLengthW32[5])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP5].Zero      = VZero;
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP5].Zero2     = VZero2;
    ++ViSrcW32;
    
    VptZsFFrameRaw[V6iFrameP5].SStatus.AcqNo                                   = VAcqId;
    VptZsFFrameRaw[V6iFrameP5].SStatus.AsicNo                                  = 5;
    VptZsFFrameRaw[V6iFrameP5].SStatus.FrameNoInAcq                            = ViFrame;
    VptZsFFrameRaw[V6iFrameP5].SStatus.FrameNoInRun                            = VRunFrameCnt;
    VptZsFFrameRaw[V6iFrameP5].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_LINE] = VTrigLine;
    VptZsFFrameRaw[V6iFrameP5].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_CLK]  = VTrigClk;
    VptZsFFrameRaw[V6iFrameP5].SStatus.ATrigRes[ASIC__MI26_TRIG_TOT_NB]        = VTrigNb;
    VptZsFFrameRaw[V6iFrameP5].SStatus.HitCnt                                  = -1;
    
    
    
    
    if ( 0 /* VPtServ->PtParAcqReqFunc->AsicChkFunc >= 0 */ ) {
      
      // No neeed to reset ResAsicErrorsRejCurAcq ( it's done on DPXI__MI26_FAsicChkFunc0Emul6Mi26 with ViFrame = 0 )
      // => ResAsicErrorsRejCurAcq = 1 if errors in current Acq
      
      // PtAcq->ResAsicErrorsRejCurAcq = DPXI__MI26_FAsicChkFuncMi26 ( VPtServ->PtParAcqReqFunc->AsicChkFunc, ViFrame, VptZsFFrameRaw, 6 /* Mi26Nb */ );
      // PtAcq->ResAsicErrorsRejCurAcq = 0;
      
      // if ( PtAcq->ResAsicErrorsRejCurAcq != 0 ) {
      //   msg (( MSG_OUT, "ViFrame=%d - VAsicErrorsRejCurAcq=%d", ViFrame, PtAcq->ResAsicErrorsRejCurAcq ));
      // }
    }
    

    // err_warning (( ERR_OUT, "TRACE : Extract frame = %d - 5", ViFrame ));
    
    ++VRunFrameCnt;
    
  } // End for ViFrame
  
  PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
  
  err_retok (( ERR_OUT, "MsgOk" ));  
}






SInt32 OLD___EFRIO__MI26_FFRioAcqDeserDataIphcMode6Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt8 AcqStatus, SInt16 TrigStatus, SInt8 DataConvertMode ) {
  
  EFRIO__TContext*     VPtCont  = &EFRIO__VGContext;
  EFRIO__TBoardConf*   VPtBoard = NULL;
  
  SInt32               VAcqId;
  UInt8*               VPtAcqData;
  MI26__TZsFFrameRaw*  VptZsFFrameRaw;
  SInt32               VFrameNbFromBoardDrv;
  SInt32               ViFrame;
  SInt32               V6iFrame;
  SInt32               V6iFrameP1;
  SInt32               V6iFrameP2;
  SInt32               V6iFrameP3;
  SInt32               V6iFrameP4;
  SInt32               V6iFrameP5;
  UInt32               VADataLengthField[6];
  // UInt32               VADataLengthW16[6];
  // UInt32               VADataLengthW32[6];
  UInt16               VADataLengthW16[6];
  UInt16               VADataLengthW32[6];
  UInt16               VDataLengthW32Max;
  register  SInt32               ViSrcW32;
  register  SInt32               ViDataW32;
  UInt32*              VPtDataW32Chip0;
  UInt32*              VPtDataW32Chip1;
  UInt32*              VPtDataW32Chip2;
  UInt32*              VPtDataW32Chip3;
  UInt32*              VPtDataW32Chip4;
  UInt32*              VPtDataW32Chip5;
  SInt32               VRunFrameCnt;
  SInt32               VLastFrameWithTrigAllowed;
  UInt32               VZero;
  UInt32               VZero2;
  SInt32               VTrigLine;
  SInt32               VTrigClk;
  SInt32               VTrigNb;
  SInt32               VNbW8ToCpy;
  register  SInt32               VNbS32ToCpy;
  UInt32               VNbU32ToCpy;
  UInt32*              VPtCpySrc;
  UInt32*              VPtCpyDest;
  
  SInt32               ViSrcS32;
  SInt32               ViDataS32;
  SInt32               ViSrcU32;
  SInt32               ViDataU32;
  
  
  //  SInt32               ViDataS32;
  //  UInt32               ViDataU32;
  //  SInt32               ViSrcS32;
  //  UInt32               ViSrcU32;
  
  UInt32               ViU32Dummy;
  
  
  PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
  
  if ( VPtCont->InfInitDone == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort because : Init not done") );
  }
  
  err_trace (( ERR_OUT, "EFRIO__MI26_FFRioAcqDeserDataIphcMode6Mi26 (P=%x, EltNb=%d)", PtSrcW32, EltNb ));
  
  // Pointers parameters check
  
  err_retnull ( PtSrcW32, (ERR_OUT,"PtSrcW32 = NULL") );
  
  // Init board conf record pointer
  
  if ( (BoardId < 0) || (BoardId >= EFRIO__MAX_BOARDS_NB) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Board Id=%d out of range [0..%d])", BoardId, EFRIO__MAX_BOARDS_NB - 1) );
  }
  
  VPtBoard = &EFRIO__VGContext.ABoardsConf[BoardId];
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ) / 6; // It must be => ( (EltNb * 2) / (MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ * 2) );
  // But result is the SAME if we divide both part of fraction by 2 ;-)
  
  if ( VFrameNbFromBoardDrv != VPtBoard->FrameNbPerAcq ) {
    err_retfail ( -1, (ERR_OUT,"Bad number of samples : %d frames acquired <> %d frames requested", VFrameNbFromBoardDrv, VPtBoard->FrameNbPerAcq ) );
  }
  
  // Get AcqId
  
  VAcqId = VPtCont->RunCont.ResAcqCnt;
  
  // Get last frame with trigger allowed / acq
  
  VLastFrameWithTrigAllowed = VPtBoard->FrameNbPerAcq - 1; // Last n frames of Acq MUST have NO trigger information - NOW n = 0 !!!
  
  
  // Init pointer to dest FFrameRaw buffer
  
  VptZsFFrameRaw = VPtCont->RunCont.PtZsFFrameRaw;
  
  // Check if buffer is allocated
  
  err_retnull ( VptZsFFrameRaw, (ERR_OUT,"Abort : FFrameRaw buffer not allocated !") );
  
  // Reset destination FFrameRaw buffer => !!! Can be removed if it makes loosing too much execution time !!!
  
  err_warning (( ERR_OUT, "TRACE : Memset buffer"));
  
  memset ( VptZsFFrameRaw, 0, VPtCont->RunCont.InfZsFFrameRawBuffSz );
  
  // err_warning (( ERR_OUT, "TRACE : Extract data"));
  
  // Extract data
  
  // VRunFrameCnt = PtAcq->InfRunFrameCnt;
  
  ViSrcW32 = 0;
  
  for ( ViFrame=0; ViFrame < VPtBoard->FrameNbPerAcq; ViFrame++ ) {
    
    // err_warning (( ERR_OUT, "TRACE : Extract frame = %d - 1", ViFrame ));
    
    
    V6iFrame   = 6 * ViFrame;
    V6iFrameP1 = V6iFrame + 1;
    V6iFrameP2 = V6iFrame + 2;
    V6iFrameP3 = V6iFrame + 3;
    V6iFrameP4 = V6iFrame + 4;
    V6iFrameP5 = V6iFrame + 5;
    
    
    VptZsFFrameRaw[V6iFrame].Header     = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP1].Header   = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP2].Header   = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP3].Header   = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP4].Header   = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP5].Header   = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    
    
    VptZsFFrameRaw[V6iFrame].FrameCnt   = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP1].FrameCnt = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP2].FrameCnt = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP3].FrameCnt = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP4].FrameCnt = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP5].FrameCnt = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    
    
    VADataLengthField[0]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[1]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[2]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[3]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[4]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[5]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    
    
    
    VptZsFFrameRaw[V6iFrame].DataLength = VADataLengthField[0];
    
    VADataLengthW16[0] = (VADataLengthField[0] & 0x0000FFFF) + ((VADataLengthField[0] & 0xFFFF0000) >> 16);
    VADataLengthW32[0] = VADataLengthW16[0] / 2;
    
    VptZsFFrameRaw[V6iFrameP1].DataLength = VADataLengthField[1];
    
    VADataLengthW16[1] = (VADataLengthField[1] & 0x0000FFFF) + ((VADataLengthField[1] & 0xFFFF0000) >> 16);
    VADataLengthW32[1] = VADataLengthW16[1] / 2;
    
    
    VptZsFFrameRaw[V6iFrameP2].DataLength = VADataLengthField[2];
    
    VADataLengthW16[2] = (VADataLengthField[2] & 0x0000FFFF) + ((VADataLengthField[2] & 0xFFFF0000) >> 16);
    VADataLengthW32[2] = VADataLengthW16[2] / 2;
    
    VptZsFFrameRaw[V6iFrameP3].DataLength = VADataLengthField[3];
    
    VADataLengthW16[3] = (VADataLengthField[3] & 0x0000FFFF) + ((VADataLengthField[3] & 0xFFFF0000) >> 16);
    VADataLengthW32[3] = VADataLengthW16[3] / 2;
    
    VptZsFFrameRaw[V6iFrameP4].DataLength = VADataLengthField[4];
    
    VADataLengthW16[4] = (VADataLengthField[4] & 0x0000FFFF) + ((VADataLengthField[4] & 0xFFFF0000) >> 16);
    VADataLengthW32[4] = VADataLengthW16[4] / 2;
    
    VptZsFFrameRaw[V6iFrameP5].DataLength = VADataLengthField[5];
    
    VADataLengthW16[5] = (VADataLengthField[5] & 0x0000FFFF) + ((VADataLengthField[5] & 0xFFFF0000) >> 16);
    VADataLengthW32[5] = VADataLengthW16[5] / 2;
    
    // Find max data length of the six Mi26
    
    VDataLengthW32Max = MATH_FUInt16Max ( VADataLengthW32, 6 );
    
    
    // err_error (( ERR_OUT, "TRACE => Length W32 [0]=%d - [1]=%d - [2]=%d - [3]=%d - [4]=%d - [5]=%d", VADataLengthW32[0], VADataLengthW32[1], VADataLengthW32[2], VADataLengthW32[3], VADataLengthW32[4], VADataLengthW32[5] ));
    // err_error (( ERR_OUT, "TRACE => MI26__ZS_FFRAME_RAW_MAX_W32 = %d", MI26__ZS_FFRAME_RAW_MAX_W32 ));
    
    // Copy all data fields = We don't care about DataLength field
    // We can decide to optimize later, but NOW I want to get ALL board RAM
    
    VPtDataW32Chip0 = (UInt32*) VptZsFFrameRaw[V6iFrame].ADataW16;
    VPtDataW32Chip1 = (UInt32*) VptZsFFrameRaw[V6iFrameP1].ADataW16;
    VPtDataW32Chip2 = (UInt32*) VptZsFFrameRaw[V6iFrameP2].ADataW16;
    VPtDataW32Chip3 = (UInt32*) VptZsFFrameRaw[V6iFrameP3].ADataW16;
    VPtDataW32Chip4 = (UInt32*) VptZsFFrameRaw[V6iFrameP4].ADataW16;
    VPtDataW32Chip5 = (UInt32*) VptZsFFrameRaw[V6iFrameP5].ADataW16;
    
    // err_warning (( ERR_OUT, "TRACE : Extract frame = %d - 2", ViFrame ));
    
    
    switch ( DataConvertMode ) {
      
      // IPHC mode
      
      case EFRIO__TRF_MODE_IPHC : {
        
        for ( ViDataW32=0; ViDataW32 < MI26__ZS_FFRAME_RAW_MAX_W32; ViDataW32++ ) {
          VPtDataW32Chip0[ViDataW32] = PtSrcW32[ViSrcW32];
          ++ViSrcW32;
          VPtDataW32Chip1[ViDataW32] = PtSrcW32[ViSrcW32];
          ++ViSrcW32;
          VPtDataW32Chip2[ViDataW32] = PtSrcW32[ViSrcW32];
          ++ViSrcW32;
          VPtDataW32Chip3[ViDataW32] = PtSrcW32[ViSrcW32];
          ++ViSrcW32;
          VPtDataW32Chip4[ViDataW32] = PtSrcW32[ViSrcW32];
          ++ViSrcW32;
          VPtDataW32Chip5[ViDataW32] = PtSrcW32[ViSrcW32];
          ++ViSrcW32;
        }
        
        break; }
      
      // Simple copy by array with W32 loop -> Vi signed
      
      case 2 : {
        
        VNbS32ToCpy = MI26__ZS_FFRAME_RAW_MAX_W32 * 6;
        
        for ( ViDataW32=0; ViDataW32 < VNbS32ToCpy; ViDataW32++, ++ViSrcW32 ) {
          VPtDataW32Chip0[ViDataW32] = PtSrcW32[ViSrcW32];
        }
        
        break; }
      
      
      // Simple copy by array with W32 loop -> Vi signed
      
      case 20 : {
        
        VNbS32ToCpy = MI26__ZS_FFRAME_RAW_MAX_W32 * 6;
        ViSrcS32    = ViSrcW32;
        
        for ( ViDataS32=0; ViDataS32 < VNbS32ToCpy; ViDataS32++, ++ViSrcS32 ) {
          VPtDataW32Chip0[ViDataS32] = PtSrcW32[ViSrcS32];
        }
        
        ViSrcW32 = ViSrcS32;
        
        // for ( ViDataW32=0; ViDataW32 < VNbS32ToCpy; ViDataW32++, ++ViSrcW32 ) {
          //   VPtDataW32Chip0[ViDataW32] = PtSrcW32[ViSrcW32];
          // }
        
        
        break; }
      
      // Simple copy by array with W32 loop -> Vi unsigned
      
      case 21 : {
        
        VNbU32ToCpy = MI26__ZS_FFRAME_RAW_MAX_W32 * 6;
        ViSrcU32    = ViSrcW32;
        
        for ( ViDataU32=0; ViDataU32 < VNbU32ToCpy; ViDataU32++, ++ViSrcU32 ) {
          VPtDataW32Chip0[ViDataU32] = PtSrcW32[ViSrcU32];
        }
        
        ViSrcW32 = ViSrcU32;
        
        break; }
      
      // Simple copy by array with W32 loop -> Vi unsigned + increment dummy U32
      
      case 22 : {
        
        VNbU32ToCpy = MI26__ZS_FFRAME_RAW_MAX_W32 * 6;
        ViSrcU32    = ViSrcW32;
        
        for ( ViDataU32=0; ViDataU32 < VNbU32ToCpy; ViDataU32++, ++ViSrcU32, ++ViU32Dummy ) {
          VPtDataW32Chip0[ViDataU32] = PtSrcW32[ViSrcU32];
        }
        
        ViSrcW32 = ViSrcU32;
        
        break; }
      
      // Simple copy by ptr with W32 loop
      
      case 3 : {
        
        VNbS32ToCpy = MI26__ZS_FFRAME_RAW_MAX_W32 * 6;
        VPtCpySrc   = VPtDataW32Chip0;
        VPtCpyDest  = &PtSrcW32[ViSrcW32];
        ViSrcW32    = ViSrcW32 + VNbS32ToCpy; // Not useful here, but for processing after this bloc
        
        
        for ( ViDataW32=0; ViDataW32 < VNbS32ToCpy; ViDataW32++, ++VPtCpySrc, ++VPtCpyDest ) {
          *VPtCpyDest = *VPtCpySrc;
        }
        
        break; }
      
      
      
      // Simple copy with memcpy
      
      case 4 : {
        
        VNbW8ToCpy = MI26__ZS_FFRAME_RAW_MAX_W32 * 6 * 4;
        
        memcpy ( (UInt8*) VPtDataW32Chip0, (UInt8*) &PtSrcW32[ViSrcW32], VNbW8ToCpy );
        
        break; }
      
      
    }
    
    // err_warning (( ERR_OUT, "TRACE : Extract frame = %d - 3", ViFrame ));
    
    VptZsFFrameRaw[V6iFrame].Trailer = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * VADataLengthW32[0])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    
    VZero = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 6];
    VptZsFFrameRaw[V6iFrame].Zero = VZero;
    ++ViSrcW32;
    
    VZero2 = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 12];
    VptZsFFrameRaw[V6iFrame].Zero2 = VZero2;
    ++ViSrcW32;
    
    // DPXI__MI26_FFRioExtractFirstTrigger ( TrigStatus, ViFrame, VLastFrameWithTrigAllowed, VZero, VZero2, &VTrigLine, &VTrigClk, &VTrigNb );
    
    // err_warning (( ERR_OUT, "TRACE : Extract frame = %d - 4", ViFrame ));
    
    VptZsFFrameRaw[V6iFrame].SStatus.AcqNo                                   = VAcqId;
    VptZsFFrameRaw[V6iFrame].SStatus.AsicNo                                  = 0;
    VptZsFFrameRaw[V6iFrame].SStatus.FrameNoInAcq                            = ViFrame;
    VptZsFFrameRaw[V6iFrame].SStatus.FrameNoInRun                            = VRunFrameCnt;
    VptZsFFrameRaw[V6iFrame].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_LINE] = VTrigLine;
    VptZsFFrameRaw[V6iFrame].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_CLK]  = VTrigClk;
    VptZsFFrameRaw[V6iFrame].SStatus.ATrigRes[ASIC__MI26_TRIG_TOT_NB]        = VTrigNb;
    VptZsFFrameRaw[V6iFrame].SStatus.HitCnt                                  = -1;
    
    
    VptZsFFrameRaw[V6iFrameP1].Trailer = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 1 + 18 + (6 * VADataLengthW32[1])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP1].Zero      = VZero;
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP1].Zero2     = VZero2;
    ++ViSrcW32;
    
    VptZsFFrameRaw[V6iFrameP1].SStatus.AcqNo                                   = VAcqId;
    VptZsFFrameRaw[V6iFrameP1].SStatus.AsicNo                                  = 1;
    VptZsFFrameRaw[V6iFrameP1].SStatus.FrameNoInAcq                            = ViFrame;
    VptZsFFrameRaw[V6iFrameP1].SStatus.FrameNoInRun                            = VRunFrameCnt;
    VptZsFFrameRaw[V6iFrameP1].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_LINE] = VTrigLine;
    VptZsFFrameRaw[V6iFrameP1].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_CLK]  = VTrigClk;
    VptZsFFrameRaw[V6iFrameP1].SStatus.ATrigRes[ASIC__MI26_TRIG_TOT_NB]        = VTrigNb;
    VptZsFFrameRaw[V6iFrameP1].SStatus.HitCnt                                  = -1;
    
    
    VptZsFFrameRaw[V6iFrameP2].Trailer = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 2 + 18 + (6 * VADataLengthW32[2])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP2].Zero      = VZero;
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP2].Zero2     = VZero2;
    ++ViSrcW32;
    
    VptZsFFrameRaw[V6iFrameP2].SStatus.AcqNo                                   = VAcqId;
    VptZsFFrameRaw[V6iFrameP2].SStatus.AsicNo                                  = 2;
    VptZsFFrameRaw[V6iFrameP2].SStatus.FrameNoInAcq                            = ViFrame;
    VptZsFFrameRaw[V6iFrameP2].SStatus.FrameNoInRun                            = VRunFrameCnt;
    VptZsFFrameRaw[V6iFrameP2].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_LINE] = VTrigLine;
    VptZsFFrameRaw[V6iFrameP2].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_CLK]  = VTrigClk;
    VptZsFFrameRaw[V6iFrameP2].SStatus.ATrigRes[ASIC__MI26_TRIG_TOT_NB]        = VTrigNb;
    VptZsFFrameRaw[V6iFrameP2].SStatus.HitCnt                                  = -1;
    
    
    VptZsFFrameRaw[V6iFrameP3].Trailer = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 3 + 18 + (6 * VADataLengthW32[3])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP3].Zero      = VZero;
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP3].Zero2     = VZero2;
    ++ViSrcW32;
    
    VptZsFFrameRaw[V6iFrameP3].SStatus.AcqNo                                   = VAcqId;
    VptZsFFrameRaw[V6iFrameP3].SStatus.AsicNo                                  = 3;
    VptZsFFrameRaw[V6iFrameP3].SStatus.FrameNoInAcq                            = ViFrame;
    VptZsFFrameRaw[V6iFrameP3].SStatus.FrameNoInRun                            = VRunFrameCnt;
    VptZsFFrameRaw[V6iFrameP3].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_LINE] = VTrigLine;
    VptZsFFrameRaw[V6iFrameP3].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_CLK]  = VTrigClk;
    VptZsFFrameRaw[V6iFrameP3].SStatus.ATrigRes[ASIC__MI26_TRIG_TOT_NB]        = VTrigNb;
    VptZsFFrameRaw[V6iFrameP3].SStatus.HitCnt                                  = -1;
    
    
    VptZsFFrameRaw[V6iFrameP4].Trailer = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 4 + 18 + (6 * VADataLengthW32[4])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP4].Zero      = VZero;
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP4].Zero2     = VZero2;
    ++ViSrcW32;
    
    VptZsFFrameRaw[V6iFrameP4].SStatus.AcqNo                                   = VAcqId;
    VptZsFFrameRaw[V6iFrameP4].SStatus.AsicNo                                  = 4;
    VptZsFFrameRaw[V6iFrameP4].SStatus.FrameNoInAcq                            = ViFrame;
    VptZsFFrameRaw[V6iFrameP4].SStatus.FrameNoInRun                            = VRunFrameCnt;
    VptZsFFrameRaw[V6iFrameP4].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_LINE] = VTrigLine;
    VptZsFFrameRaw[V6iFrameP4].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_CLK]  = VTrigClk;
    VptZsFFrameRaw[V6iFrameP4].SStatus.ATrigRes[ASIC__MI26_TRIG_TOT_NB]        = VTrigNb;
    VptZsFFrameRaw[V6iFrameP4].SStatus.HitCnt                                  = -1;
    
    
    VptZsFFrameRaw[V6iFrameP5].Trailer = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 5 + 18 + (6 * VADataLengthW32[5])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP5].Zero      = VZero;
    ++ViSrcW32;
    VptZsFFrameRaw[V6iFrameP5].Zero2     = VZero2;
    ++ViSrcW32;
    
    VptZsFFrameRaw[V6iFrameP5].SStatus.AcqNo                                   = VAcqId;
    VptZsFFrameRaw[V6iFrameP5].SStatus.AsicNo                                  = 5;
    VptZsFFrameRaw[V6iFrameP5].SStatus.FrameNoInAcq                            = ViFrame;
    VptZsFFrameRaw[V6iFrameP5].SStatus.FrameNoInRun                            = VRunFrameCnt;
    VptZsFFrameRaw[V6iFrameP5].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_LINE] = VTrigLine;
    VptZsFFrameRaw[V6iFrameP5].SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_CLK]  = VTrigClk;
    VptZsFFrameRaw[V6iFrameP5].SStatus.ATrigRes[ASIC__MI26_TRIG_TOT_NB]        = VTrigNb;
    VptZsFFrameRaw[V6iFrameP5].SStatus.HitCnt                                  = -1;
    
    
    
    
    if ( 0 /* VPtServ->PtParAcqReqFunc->AsicChkFunc >= 0 */ ) {
      
      // No neeed to reset ResAsicErrorsRejCurAcq ( it's done on DPXI__MI26_FAsicChkFunc0Emul6Mi26 with ViFrame = 0 )
      // => ResAsicErrorsRejCurAcq = 1 if errors in current Acq
      
      // PtAcq->ResAsicErrorsRejCurAcq = DPXI__MI26_FAsicChkFuncMi26 ( VPtServ->PtParAcqReqFunc->AsicChkFunc, ViFrame, VptZsFFrameRaw, 6 /* Mi26Nb */ );
      // PtAcq->ResAsicErrorsRejCurAcq = 0;
      
      // if ( PtAcq->ResAsicErrorsRejCurAcq != 0 ) {
        //   msg (( MSG_OUT, "ViFrame=%d - VAsicErrorsRejCurAcq=%d", ViFrame, PtAcq->ResAsicErrorsRejCurAcq ));
        // }
    }
    
    
    // err_warning (( ERR_OUT, "TRACE : Extract frame = %d - 5", ViFrame ));
    
    ++VRunFrameCnt;
    
  } // End for ViFrame
  
  PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
  
  err_retok (( ERR_OUT, "MsgOk" ));
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    :
:
Level     : This is a user level function.
Date      : 25/10/2010
Rev       : 
Doc date  : 
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet1Mode1Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt8 AcqStatus, SInt16 TrigStatus, SInt8 DataConvertMode ) {
  
  EFRIO__TContext*     VPtCont   = &EFRIO__VGContext;
  EFRIO__TBoardConf*   VPtBoard  = NULL;
  EFRIO__TRunCont*     VPtRun    = &VPtCont->RunCont;
  EFRIO__TFrameList*   VPtFrList = &VPtCont->AAcqFrameList[0];
  EFRIO__TFrame*       VPtFrame;    
  
  SInt32               VAcqId;
  UInt8*               VPtAcqData;
  SInt32               VFrameNbFromBoardDrv;
  SInt32               ViFrame;
  UInt32               VDataLengthField;
  UInt32               VDataLengthW8;
  UInt32               VDataLengthW16;
  UInt32               VDataLengthW32;
  SInt32               ViSrcW32;
  SInt32               ViDataW32;
  SInt32               VRunFrameCnt;
  SInt32               VLastFrameWithTrigAllowed;
  UInt32               VZero;
  UInt32               VZero2;
  SInt16               VTrigNb;
  UInt16               VATrigVal[3];
  UInt16               VATrigLine[3];
  UInt16               VATrigClk[3];
  UInt16               VAMi26Trig[3];
  EFRIO__TTriggerRec*  VPtTrigRec;
  UInt32               VFullFrameRecSz;
  UInt32               VEmptyFrameRecSz;   
  UInt32               VEmptyTrigRecSz;
  SInt32               VDataLengthErrCnt;
  
  UInt16               VDbgDataLenghtD0;
  UInt16               VDbgDataLenghtD1;
  SInt32               VTotAcqSz;
  
  
  PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
  
  if ( VPtCont->InfInitDone == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort because : Init not done") );
  }
  
  err_trace (( ERR_OUT, "EFRIO__MI26_FFRioAcqDeserDataEudet1Mode1Mi26 (P=%x, EltNb=%d)", PtSrcW32, EltNb ));
  
  // Pointers parameters check
  
  err_retnull ( PtSrcW32, (ERR_OUT,"PtSrcW32 = NULL") );
  
  // Init board conf record pointer
  
  if ( (BoardId < 0) || (BoardId >= EFRIO__MAX_BOARDS_NB) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Board Id=%d out of range [0..%d])", BoardId, EFRIO__MAX_BOARDS_NB - 1) );
  }
  
  VPtBoard = &EFRIO__VGContext.ABoardsConf[BoardId];
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ); // It must be => ( (EltNb * 2) / (MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ * 2) );
  // But result is the SAME if we divide both part of fraction by 2 ;-)
  
  if ( VFrameNbFromBoardDrv != VPtBoard->FrameNbPerAcq ) {
    err_retfail ( -1, (ERR_OUT,"Bad number of samples : %d frames acquired <> %d frames requested", VFrameNbFromBoardDrv, VPtBoard->FrameNbPerAcq ) );
  }

  // Reset total size of acquisition
  
  VTotAcqSz = 0;
  
  // Get acq id
  
  VAcqId = VPtCont->RunCont.ResAcqCnt;
  
  // Get last frame with trigger allowed / acq
  
  VLastFrameWithTrigAllowed = VPtBoard->FrameNbPerAcq - 1; // Last n frames of Acq MUST have NO trigger information - NOW n = 0 !!!
        
  // Check if buffer is allocated ( this is total buffer, not only the part where we will write )
  
  err_retnull ( VPtRun->PtFrame, (ERR_OUT,"Abort : EUDET frames buffer not allocated !") );

  // Reset frame list pointer
  
  VPtFrList->TotFrameNb = 0;
  
  memset ( VPtFrList->AFramePtr, 0, EFRIO__MAX_FRAME_NB_PER_ACQ * sizeof (EFRIO__TFrame*) );
        
  // Calculate size of records WITHOUT variable length part counted here as one elt
  
  VEmptyTrigRecSz  = sizeof ( EFRIO__TTriggerRec );
  VEmptyFrameRecSz = sizeof ( EFRIO__TFrame );
  
//  err_error (( ERR_OUT, "TRACE => VEmptyTrigRecSz  = %d", VEmptyTrigRecSz  ));
//  err_error (( ERR_OUT, "TRACE => VEmptyFrameRecSz = %d", VEmptyFrameRecSz ));
  
  // Extract data
    
  ViSrcW32          = 0;
  VDataLengthErrCnt = 0;

  for ( ViFrame=0; ViFrame < VPtBoard->FrameNbPerAcq; ViFrame++ ) {

    // Update pointer to next frame
    
    if ( VPtFrList->TotFrameNb == 0 ) {
      VPtFrame = VPtFrList->AFramePtr[0] = VPtRun->PtFrame;
    }
    
    else {
      
      if ( VPtFrList->TotFrameNb >= VPtCont->RunCont.ParFrameNbPerAcq ) {
        err_retfail ( -1, (ERR_OUT,"Abort try to process frame %d > Max = %d ", VPtFrList->TotFrameNb, VPtCont->RunCont.ParFrameNbPerAcq) );
      }
      
      VPtFrame = VPtFrList->AFramePtr[VPtFrList->TotFrameNb] = (EFRIO__TFrame*) ((UInt8*) VPtFrList->AFramePtr[VPtFrList->TotFrameNb - 1] + VFullFrameRecSz);
      
      // err_error (( ERR_OUT, "TRACE : Set pointer frame %d - pointer = %d [D]", VPtFrList->TotFrameNb, VPtFrList->AFramePtr[VPtFrList->TotFrameNb] ));
    }
    
  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtFrame->Tag        = EFRIO__FRAME_TAG;
    VPtFrame->Header.Tag = EFRIO__FRAME_TAG_HEADER;
  #endif
  
    VPtFrame->Header.AcqId        = VAcqId;
    VPtFrame->Header.FrameIdInAcq = ViFrame;
    VPtFrame->Header.MapsName     = (UInt16) ASIC__MI26;
    VPtFrame->Header.MapsNb       = VPtRun->ParMi26Nb;
    
    VPtFrame->Header.AMapsHeader[0]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsFrameCnt[0]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VDataLengthField                   = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    
    VDataLengthW16 = (VDataLengthField & 0x0000FFFF) + ((VDataLengthField & 0xFFFF0000) >> 16);

    VDbgDataLenghtD0 = (VDataLengthField & 0x0000FFFF);
    VDbgDataLenghtD1 = ((VDataLengthField & 0xFFFF0000) >> 16);
    
    if ( (VDbgDataLenghtD0 > 570) || (VDbgDataLenghtD1 > 570) ) {
      VDataLengthW16 = 0;
      ++VDataLengthErrCnt;
      err_error (( ERR_OUT, "HW error on data length -> D0=%4d [D] - D1=%4d [D] -> Force 0 !", VDbgDataLenghtD0, VDbgDataLenghtD1 ));
      err_error (( ERR_OUT, "HW error on data length -> D0=%4x [H] - D1=%4x [H] -> Force 0 !", VDbgDataLenghtD0, VDbgDataLenghtD1 ));
    }

    VDataLengthW8  = VDataLengthW16 * 2;
    VDataLengthW32 = VDataLengthW16 / 2;
    
    
    VPtFrame->Header.AMapsDataLength[0] = VDataLengthW16 * 2;
    
    // Copy all data fields = We don't care about DataLength field
    // We can decide to optimize later, but NOW I want to get ALL board RAM
    
  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtFrame->Data.Tag       = EFRIO__FRAME_TAG_DATA;
  #endif
  
    VPtFrame->Data.TotSz     = MI26__ZS_FFRAME_RAW_MAX_W8;
    VPtFrame->Data.OneMapsSz = MI26__ZS_FFRAME_RAW_MAX_W8;
    
    // err_error (( ERR_OUT, "TRACE -> Before loop - ViFrame=%d - VDataLengthW32=%d", ViFrame, VDataLengthW32 ));
    
    for ( ViDataW32=0; ViDataW32 < VDataLengthW32; ViDataW32++ ) {
      VPtFrame->Data.ADataW32[ViDataW32] = PtSrcW32[ViSrcW32];
      ++ViSrcW32;
    }
    
    ViSrcW32 = ViSrcW32 + ( MI26__ZS_FFRAME_RAW_MAX_W32 - VDataLengthW32 );

    // err_error (( ERR_OUT, "TRACE -> After loop - ViFrame=%d - VDataLengthW32=%d", ViFrame, VDataLengthW32 ));
    
    VPtFrame->Header.AMapsTrailer[0] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame) + (3 + VDataLengthW32)];    // 3 = Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;

    VZero = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame) + (3 + MI26__ZS_FFRAME_RAW_MAX_W32 + 1)];
    ++ViSrcW32;
    
    VZero2 = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame) + (3 + MI26__ZS_FFRAME_RAW_MAX_W32 + 2)];
    ++ViSrcW32;

    // Extraction of trigger info from "Mimosa 26 frame" zero fields
  
    VTrigNb = (VZero  & 0xFFFF0000) >> 16;
  
    if ( VTrigNb != 0 ) {
      VATrigVal[0]    = (VZero  & 0x0000FFFF);
      VATrigVal[1]    = (VZero2 & 0xFFFF0000) >> 16;
      VATrigVal[2]    = (VZero2 & 0x0000FFFF);
      
      VATrigLine[0]   = VATrigVal[0] / 16;
      VATrigLine[1]   = VATrigVal[1] / 16;
      VATrigLine[2]   = VATrigVal[2] / 16;
      
      VATrigClk[0]    = VATrigVal[0] % 16;
      VATrigClk[1]    = VATrigVal[1] % 16;
      VATrigClk[2]    = VATrigVal[2] % 16;
      
      VAMi26Trig[0]   = VATrigLine[0] /* + ( VATrigClk[0] << 10 ) */;
      VAMi26Trig[1]   = VATrigLine[1] /* + ( VATrigClk[1] << 10 ) */;
      VAMi26Trig[2]   = VATrigLine[2] /* + ( VATrigClk[2] << 10 ) */;
    }
    
    else {
      VAMi26Trig[0] = VAMi26Trig[1] = VAMi26Trig[2] = 0;
    }

    // Add trigger info in frame header

    VPtFrame->Header.TriggerNb        = VTrigNb;
    VPtFrame->Header.AMapsTrigInfo[0] = VAMi26Trig[0];
    VPtFrame->Header.AMapsTrigInfo[1] = VAMi26Trig[1];
    VPtFrame->Header.AMapsTrigInfo[2] = VAMi26Trig[2];
    
    // Add trigger info in trigger record

    VPtTrigRec = (EFRIO__TTriggerRec*) (VPtFrame->Data.ADataW32 + VDataLengthW32);

  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtTrigRec->Tag      = EFRIO__FRAME_TAG_TRIG;
  #endif
  
    VPtTrigRec->TrigNb   = VTrigNb;
    VPtTrigRec->TotSz    = VEmptyTrigRecSz + (VTrigNb * EFRIO__EXTRA_CHAN__TRIGGER_FIELD_SZ);
    VPtTrigRec->TrigType = 1;
    VPtTrigRec->ATrig[0] = VAMi26Trig[0];
    VPtTrigRec->ATrig[1] = VAMi26Trig[1];
    VPtTrigRec->ATrig[2] = VAMi26Trig[2];
    
    
//    err_error (( ERR_OUT, "TRACE => VAMi26Trig[0]=%d - VAMi26Trig[1]=%d - VAMi26Trig[2]=%d", VAMi26Trig[0], VAMi26Trig[1], VAMi26Trig[2] ));
//    err_error (( ERR_OUT, "TRACE => ATrig[0]     =%d - ATrig[1]     =%d - ATrig[2]     =%d", VPtTrigRec->ATrig[0], VPtTrigRec->ATrig[1], VPtTrigRec->ATrig[2] ));
    
  
  
    // Calculate size of frame record while filled ( fixed part + variable length (data & trigger) )
  
    // Calculate size will be 2 x W32 higher than needed, because following fields will be counted twice
    //
    // - Field UInt32 ADataW32[0] of data record -> EFRIO__TFrameData
    // - Field UInt32 ATrig[0] of trigger record -> EFRIO__TTriggerRec
  
    VFullFrameRecSz = VEmptyFrameRecSz + VDataLengthW8 + VPtTrigRec->TotSz; 

    VTotAcqSz += VFullFrameRecSz;
    
    // Update frame field total size & trigger offset size
  
    VPtFrame->TotSz         = VFullFrameRecSz;
    VPtFrame->TrigRecOffset = VEmptyFrameRecSz + VDataLengthW8;
        
    if ( 0 /* VPtServ->PtParAcqReqFunc->AsicChkFunc >= 0 */ ) {
      
      // No neeed to reset ResAsicErrorsRejCurAcq ( it's done on DPXI__MI26_FAsicChkFunc0Emul6Mi26 with ViFrame = 0 )
      // => ResAsicErrorsRejCurAcq = 1 if errors in current Acq
      
      // $$$ PtAcq->ResAsicErrorsRejCurAcq = DPXI__MI26_FAsicChkFuncMi26 ( VPtServ->PtParAcqReqFunc->AsicChkFunc, ViFrame, VptZsFFrameRaw, 1 /* Mi26Nb */ );
      // $$$ PtAcq->ResAsicErrorsRejCurAcq = 0;
      
      // $$$ if ( PtAcq->ResAsicErrorsRejCurAcq != 0 ) {
        // $$$   msg (( MSG_OUT, "ViFrame=%d - VAsicErrorsRejCurAcq=%d", ViFrame, PtAcq->ResAsicErrorsRejCurAcq ));
        // $$$ }
    }
    
    
    ++VPtFrList->TotFrameNb;
    
    ++VRunFrameCnt;
    
  } // End for ViFrame
  
  ++VPtCont->RunCont.ResAcqCnt;
  
  PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
  
  return (VTotAcqSz);
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    :
:
Level     : This is a user level function.
Date      : 27/10/2010
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet1Mode6Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt8 AcqStatus, SInt16 TrigStatus, SInt8 DataConvertMode ) {
  
  EFRIO__TContext*     VPtCont   = &EFRIO__VGContext;
  EFRIO__TBoardConf*   VPtBoard  = NULL;
  EFRIO__TRunCont*     VPtRun    = &VPtCont->RunCont;
  EFRIO__TFrameList*   VPtFrList = &VPtCont->AAcqFrameList[0];
  EFRIO__TFrame*       VPtFrame;
  
  SInt32               VAcqId;
  UInt8*               VPtAcqData;
  SInt32               VFrameNbFromBoardDrv;
  SInt32               ViFrame;
  SInt32               V6iFrame;
  UInt32               VADataLengthField[6];
  UInt32               VADataLengthW8[6];
  UInt16               VADataLengthW16[6];
  UInt32               VADataLengthW32[6];
  UInt32               VDataLengthW8Max;
  UInt32               VDataLengthW16Max;
  UInt32               VDataLengthW32Max;
  UInt32               VDataLengthW32ToCpy;
  UInt32               VDataLengthW8ToCpy;
  SInt32               ViSrcW32;
  SInt32               ViDataW32;
  SInt32               VRunFrameCnt;
  SInt32               VLastFrameWithTrigAllowed;
  UInt32               VZero;
  UInt32               VZero2;
  SInt16               VTrigNb;
  UInt16               VATrigVal[3];
  UInt16               VATrigLine[3];
  UInt16               VATrigClk[3];
  UInt16               VAMi26Trig[3];
  EFRIO__TTriggerRec*  VPtTrigRec;
  UInt32               VFullFrameRecSz;
  UInt32               VEmptyFrameRecSz;
  UInt32               VEmptyTrigRecSz;
  SInt32               VDataLengthErrCnt;
  SInt8                ViMi26;
  SInt32               VTotAcqSz;
  
  
  PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
  
  if ( VPtCont->InfInitDone == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort because : Init not done") );
  }
  
  err_trace (( ERR_OUT, "EFRIO__MI26_FFRioAcqDeserDataEudet1Mode1Mi26 (P=%x, EltNb=%d)", PtSrcW32, EltNb ));
  
  // Pointers parameters check
  
  err_retnull ( PtSrcW32, (ERR_OUT,"PtSrcW32 = NULL") );
  
  // Init board conf record pointer
  
  if ( (BoardId < 0) || (BoardId >= EFRIO__MAX_BOARDS_NB) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Board Id=%d out of range [0..%d])", BoardId, EFRIO__MAX_BOARDS_NB - 1) );
  }
  
  VPtBoard = &EFRIO__VGContext.ABoardsConf[BoardId];
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ) / 6; // It must be => ( (EltNb * 2) / (MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ * 2) );
  // But result is the SAME if we divide both part of fraction by 2 ;-)
  
  if ( VFrameNbFromBoardDrv != VPtBoard->FrameNbPerAcq ) {
    err_retfail ( -1, (ERR_OUT,"Bad number of samples : %d frames acquired <> %d frames requested", VFrameNbFromBoardDrv, VPtBoard->FrameNbPerAcq ) );
  }

  // Reset total size of acquisition
  
  VTotAcqSz = 0;
  
  // Get acq id
  
  VAcqId = VPtCont->RunCont.ResAcqCnt;
  
  // Get last frame with trigger allowed / acq
  
  VLastFrameWithTrigAllowed = VPtBoard->FrameNbPerAcq - 1; // Last n frames of Acq MUST have NO trigger information - NOW n = 0 !!!
  
  // Check if buffer is allocated ( this is total buffer, not only the part where we will write )
  
  err_retnull ( VPtRun->PtFrame, (ERR_OUT,"Abort : EUDET frames buffer not allocated !") );
  
  // Reset frame list pointer
  
  VPtFrList->TotFrameNb = 0;
  
  memset ( VPtFrList->AFramePtr, 0, EFRIO__MAX_FRAME_NB_PER_ACQ * sizeof (EFRIO__TFrame*) );
    
  // Calculate size of records WITHOUT variable length part counted here as one elt
  
  VEmptyTrigRecSz  = sizeof ( EFRIO__TTriggerRec );
  VEmptyFrameRecSz = sizeof ( EFRIO__TFrame );
  
  //  err_error (( ERR_OUT, "TRACE => VEmptyTrigRecSz  = %d", VEmptyTrigRecSz  ));
  //  err_error (( ERR_OUT, "TRACE => VEmptyFrameRecSz = %d", VEmptyFrameRecSz ));
  
  // Extract data
  
  ViSrcW32          = 0;
  VDataLengthErrCnt = 0;
  
  for ( ViFrame=0; ViFrame < VPtBoard->FrameNbPerAcq; ViFrame++ ) {
    
    V6iFrame = 6 * ViFrame;
    
    // Update pointer to next frame
    
    if ( VPtFrList->TotFrameNb == 0 ) {
      VPtFrame = VPtFrList->AFramePtr[0] = VPtRun->PtFrame;
    }
    
    else {
      
      if ( VPtFrList->TotFrameNb >= VPtCont->RunCont.ParFrameNbPerAcq ) {
        err_retfail ( -1, (ERR_OUT,"Abort try to process frame %d > Max = %d ", VPtFrList->TotFrameNb, VPtCont->RunCont.ParFrameNbPerAcq) );
      }
      
      VPtFrame = VPtFrList->AFramePtr[VPtFrList->TotFrameNb] = (EFRIO__TFrame*) ((UInt8*) VPtFrList->AFramePtr[VPtFrList->TotFrameNb - 1] + VFullFrameRecSz);
      
      // err_error (( ERR_OUT, "TRACE : Set pointer frame %d - pointer = %d [D]", VPtFrList->TotFrameNb, VPtFrList->AFramePtr[VPtFrList->TotFrameNb] ));
    }

  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtFrame->Tag        = EFRIO__FRAME_TAG;
    VPtFrame->Header.Tag          = EFRIO__FRAME_TAG_HEADER;
  #endif
  
    VPtFrame->Header.AcqId        = VAcqId;
    VPtFrame->Header.FrameIdInAcq = ViFrame;
    VPtFrame->Header.MapsName     = (UInt16) ASIC__MI26;
    VPtFrame->Header.MapsNb       = VPtRun->ParMi26Nb;
    
    VPtFrame->Header.AMapsHeader[0]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsHeader[1]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsHeader[2]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsHeader[3]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsHeader[4]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsHeader[5]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsFrameCnt[0]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsFrameCnt[1]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsFrameCnt[2]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsFrameCnt[3]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsFrameCnt[4]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsFrameCnt[5]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    
    VADataLengthField[0]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[1]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[2]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[3]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[4]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[5]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    
    VADataLengthW16[0] = (VADataLengthField[0] & 0x0000FFFF) + ((VADataLengthField[0] & 0xFFFF0000) >> 16);
    VADataLengthW16[1] = (VADataLengthField[1] & 0x0000FFFF) + ((VADataLengthField[1] & 0xFFFF0000) >> 16);
    VADataLengthW16[2] = (VADataLengthField[2] & 0x0000FFFF) + ((VADataLengthField[2] & 0xFFFF0000) >> 16);
    VADataLengthW16[3] = (VADataLengthField[3] & 0x0000FFFF) + ((VADataLengthField[3] & 0xFFFF0000) >> 16);
    VADataLengthW16[4] = (VADataLengthField[4] & 0x0000FFFF) + ((VADataLengthField[4] & 0xFFFF0000) >> 16);
    VADataLengthW16[5] = (VADataLengthField[5] & 0x0000FFFF) + ((VADataLengthField[5] & 0xFFFF0000) >> 16);
        
    VDataLengthW16Max = MATH_FUInt16Max ( VADataLengthW16, 6 );
    
    
    // A better test ( on each VADataLengthW16[i] ? on each W16 on D0 & D1 ? ) should be done !
        
    if ( VDataLengthW16Max > 2304 ) {      
      memset ( VADataLengthW8 , 0, 12 );
      memset ( VADataLengthW16, 0, 12 );
      memset ( VADataLengthW32, 0, 12 );
      
      VDataLengthW8Max  = 0;
      VDataLengthW32Max = 0;      
      
      ++VDataLengthErrCnt;
      err_error (( ERR_OUT, "HW error on data length -> Max W16 nb = %d -> Force 0 !", VDataLengthW16Max ));
    }
    
    else {
      
      for ( ViMi26=0; ViMi26 < 6; ViMi26++ ) {
        VADataLengthW8[ViMi26]  = VADataLengthW16[ViMi26] * 2;
        VADataLengthW32[ViMi26] = VADataLengthW16[ViMi26] / 2;
      }
    
      VDataLengthW8Max  = VDataLengthW16Max * 2;
      VDataLengthW32Max = VDataLengthW16Max / 2;    
    }
                
    VPtFrame->Header.AMapsDataLength[0] = VADataLengthW8[0];
    VPtFrame->Header.AMapsDataLength[1] = VADataLengthW8[1];
    VPtFrame->Header.AMapsDataLength[2] = VADataLengthW8[2];
    VPtFrame->Header.AMapsDataLength[3] = VADataLengthW8[3];
    VPtFrame->Header.AMapsDataLength[4] = VADataLengthW8[4];
    VPtFrame->Header.AMapsDataLength[5] = VADataLengthW8[5];
    
    // Copy useful data part

    VDataLengthW32ToCpy = VDataLengthW32Max * 6;
    VDataLengthW8ToCpy  = VDataLengthW32ToCpy * 4;
    
  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtFrame->Data.Tag       = EFRIO__FRAME_TAG_DATA;
  #endif
  
    
    VPtFrame->Data.TotSz     = VDataLengthW8ToCpy;
    VPtFrame->Data.OneMapsSz = VDataLengthW8Max;
    
    // err_error (( ERR_OUT, "TRACE -> Before loop - ViFrame=%d - VDataLengthW32=%d", ViFrame, VDataLengthW32 ));
    
    
    // A remplacer par memcpy !!!!!!!!!
    
    
    memcpy ( VPtFrame->Data.ADataW32, &PtSrcW32[ViSrcW32], VDataLengthW8ToCpy  );
    
    // err_error (( ERR_OUT, "TRACE => VDataLengthW8ToCpy=%d", VDataLengthW8ToCpy ));
    
    // for ( ViDataW32=0; ViDataW32 < VDataLengthW32ToCpy; ViDataW32++ ) {
    //   VPtFrame->Data.ADataW32[ViDataW32] = PtSrcW32[ViSrcW32];
    //   ++ViSrcW32;
    // }
    
    // ViSrcW32 = ViSrcW32 + ( (6 * MI26__ZS_FFRAME_RAW_MAX_W32) - VDataLengthW32ToCpy );
    
     ViSrcW32 += (6 * MI26__ZS_FFRAME_RAW_MAX_W32);
    
    
//    VptZsFFrameRaw[V6iFrame].Trailer = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * VADataLengthW32[0])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
//    ++ViSrcW32;
    
//    VZero = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 6];
//    VptZsFFrameRaw[V6iFrame].Zero = VZero;
//    ++ViSrcW32;
    
//    VZero2 = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 12];
//    VptZsFFrameRaw[V6iFrame].Zero2 = VZero2;
//    ++ViSrcW32;
    

    VPtFrame->Header.AMapsTrailer[0] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * VADataLengthW32[0])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[1] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + 1 + (6 * VADataLengthW32[1])];   
    ++ViSrcW32;

    VPtFrame->Header.AMapsTrailer[2] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + 2 + (6 * VADataLengthW32[2])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[3] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + 3 + (6 * VADataLengthW32[3])];
    ++ViSrcW32;

    VPtFrame->Header.AMapsTrailer[4] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + 4 + (6 * VADataLengthW32[4])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[5] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + 5 + (6 * VADataLengthW32[5])];
    ++ViSrcW32;    

    VZero  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) +  6];
    VZero2 = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 12];
    
    ViSrcW32 += 12; // 6 times 2 zero fields = 12
        
    // Extraction of trigger info from "Mimosa 26 frame" zero fields
    
    VTrigNb = (VZero  & 0xFFFF0000) >> 16;
    
    if ( VTrigNb != 0 ) {
      VATrigVal[0]    = (VZero  & 0x0000FFFF);
      VATrigVal[1]    = (VZero2 & 0xFFFF0000) >> 16;
      VATrigVal[2]    = (VZero2 & 0x0000FFFF);
      
      VATrigLine[0]   = VATrigVal[0] / 16;
      VATrigLine[1]   = VATrigVal[1] / 16;
      VATrigLine[2]   = VATrigVal[2] / 16;
      
      VATrigClk[0]    = VATrigVal[0] % 16;
      VATrigClk[1]    = VATrigVal[1] % 16;
      VATrigClk[2]    = VATrigVal[2] % 16;
      
      VAMi26Trig[0]   = VATrigLine[0] /* + ( VATrigClk[0] << 10 ) */;
      VAMi26Trig[1]   = VATrigLine[1] /* + ( VATrigClk[1] << 10 ) */;
      VAMi26Trig[2]   = VATrigLine[2] /* + ( VATrigClk[2] << 10 ) */;
    }
    
    else {
      VAMi26Trig[0] = VAMi26Trig[1] = VAMi26Trig[2] = 0;
    }
    
    // Add trigger info in frame header
    
    VPtFrame->Header.TriggerNb        = VTrigNb;
    VPtFrame->Header.AMapsTrigInfo[0] = VAMi26Trig[0];
    VPtFrame->Header.AMapsTrigInfo[1] = VAMi26Trig[1];
    VPtFrame->Header.AMapsTrigInfo[2] = VAMi26Trig[2];
    
    // Add trigger info in trigger record
    
    VPtTrigRec = (EFRIO__TTriggerRec*) (VPtFrame->Data.ADataW32 + VDataLengthW32ToCpy);
    
  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtTrigRec->Tag      = EFRIO__FRAME_TAG_TRIG;
  #endif
  
    VPtTrigRec->TrigNb   = VTrigNb;
    VPtTrigRec->TotSz    = VEmptyTrigRecSz + (VTrigNb * EFRIO__EXTRA_CHAN__TRIGGER_FIELD_SZ);
    VPtTrigRec->TrigType = 1;
    VPtTrigRec->ATrig[0] = VAMi26Trig[0];
    VPtTrigRec->ATrig[1] = VAMi26Trig[1];
    VPtTrigRec->ATrig[2] = VAMi26Trig[2];
    
    // err_error (( ERR_OUT, "TRACE => VAMi26Trig[0]=%d - VAMi26Trig[1]=%d - VAMi26Trig[2]=%d", VAMi26Trig[0], VAMi26Trig[1], VAMi26Trig[2] ));
    
    // Calculate size of frame record while filled ( fixed part + variable length (data & trigger) )
    
    // Calculate size will be 2 x W32 higher than needed, because following fields will be counted twice
    //
    // - Field UInt32 ADataW32[0] of data record -> EFRIO__TFrameData
    // - Field UInt32 ATrig[0] of trigger record -> EFRIO__TTriggerRec
    
    VFullFrameRecSz = VEmptyFrameRecSz + VDataLengthW8ToCpy + VPtTrigRec->TotSz;

    VTotAcqSz += VFullFrameRecSz;
    
    // Update frame field total size & trigger offset size
    
    VPtFrame->TotSz         = VFullFrameRecSz;
    VPtFrame->TrigRecOffset = VEmptyFrameRecSz + VDataLengthW8ToCpy;
    
    if ( 0 /* VPtServ->PtParAcqReqFunc->AsicChkFunc >= 0 */ ) {
      
      // No neeed to reset ResAsicErrorsRejCurAcq ( it's done on DPXI__MI26_FAsicChkFunc0Emul6Mi26 with ViFrame = 0 )
      // => ResAsicErrorsRejCurAcq = 1 if errors in current Acq
      
      // $$$ PtAcq->ResAsicErrorsRejCurAcq = DPXI__MI26_FAsicChkFuncMi26 ( VPtServ->PtParAcqReqFunc->AsicChkFunc, ViFrame, VptZsFFrameRaw, 1 /* Mi26Nb */ );
      // $$$ PtAcq->ResAsicErrorsRejCurAcq = 0;
      
      // $$$ if ( PtAcq->ResAsicErrorsRejCurAcq != 0 ) {
        // $$$   msg (( MSG_OUT, "ViFrame=%d - VAsicErrorsRejCurAcq=%d", ViFrame, PtAcq->ResAsicErrorsRejCurAcq ));
        // $$$ }
    }
    
    
    ++VPtFrList->TotFrameNb;
    
    ++VRunFrameCnt;
    
  } // End for ViFrame
  
  ++VPtCont->RunCont.ResAcqCnt;
  
  PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
  
  return (VTotAcqSz);
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    :
:
Level     : This is a user level function.
Date      : 28/10/2010
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet2Mode1Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt8 AcqStatus, SInt16 TrigStatus, SInt8 DataConvertMode ) {
  
  EFRIO__TContext*     VPtCont   = &EFRIO__VGContext;
  EFRIO__TBoardConf*   VPtBoard  = NULL;
  EFRIO__TRunCont*     VPtRun    = &VPtCont->RunCont;
  EFRIO__TFrameList*   VPtFrList = &VPtCont->AAcqFrameList[0];
  EFRIO__TFrame*       VPtFrame;
  EFRIO__TTriggerRec*  VPtTmpTrigRec = VPtCont->PtTmpTrigRec;
  
  
  
  SInt32               VAcqId;
  UInt8*               VPtAcqData;
  SInt32               VFrameNbFromBoardDrv;
  SInt32               ViFrame;
  UInt32               VDataLengthField;
  UInt32               VDataLengthW8;
  UInt32               VDataLengthW16;
  UInt32               VDataLengthW32;
  SInt32               ViSrcW32;
  SInt32               ViSrcW32BeforeDataCpyLoop;
  SInt32               ViDataW32;
  SInt32               VRunFrameCnt;
  SInt32               VLastFrameWithTrigAllowed;
  UInt32               VZero;
  UInt32               VZero2;
  SInt16               VTrigNb;
  UInt16               VATrigVal[3];
  UInt16               VATrigLine[3];
  UInt16               VATrigClk[3];
  UInt16               VAMi26Trig[3];
  EFRIO__TTriggerRec*  VPtTrigRec;
  UInt32               VFullFrameRecSz;
  UInt32               VEmptyFrameRecSz;
  UInt32               VEmptyTrigRecSz;
  SInt32               VDataLengthErrCnt;
  
  UInt16               VDbgDataLenghtD0;
  UInt16               VDbgDataLenghtD1;
  
  SInt16               VEChanTrigFieldCnt; // Extra channel trigger fields counter
  UInt32               VEChanTrigField;
  SInt32               VTotAcqSz;
  
   
  
  PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
  
  if ( VPtCont->InfInitDone == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort because : Init not done") );
  }
  
  // err_trace (( ERR_OUT, "EFRIO__MI26_FFRioAcqDeserDataEudet1Mode1Mi26 (P=%x, EltNb=%d)", PtSrcW32, EltNb ));
  

  // Pointers parameters check
  
  err_retnull ( PtSrcW32, (ERR_OUT,"PtSrcW32 = NULL") );
  
  // Init board conf record pointer
  
  if ( (BoardId < 0) || (BoardId >= EFRIO__MAX_BOARDS_NB) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Board Id=%d out of range [0..%d])", BoardId, EFRIO__MAX_BOARDS_NB - 1) );
  }
  
  VPtBoard = &EFRIO__VGContext.ABoardsConf[BoardId];
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ) / 2; // Divide by 2 because of extra channel
  
  // It must be => ( (EltNb * 2) / (MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ * 2) );
  // But result is the SAME if we divide both part of fraction by 2 ;-)
  
  if ( VFrameNbFromBoardDrv != VPtBoard->FrameNbPerAcq ) {
    err_retfail ( -1, (ERR_OUT,"Bad number of samples : %d frames acquired <> %d frames requested", VFrameNbFromBoardDrv, VPtBoard->FrameNbPerAcq ) );
  }
  
  // Reset total size of acquisition 
  
  VTotAcqSz = 0;
  
  // Get acq id
  
  VAcqId = VPtCont->RunCont.ResAcqCnt;
  
  // Get last frame with trigger allowed / acq
  
  VLastFrameWithTrigAllowed = VPtBoard->FrameNbPerAcq - 1; // Last n frames of Acq MUST have NO trigger information - NOW n = 0 !!!
  
  // Check if buffer is allocated ( this is total buffer, not only the part where we will write )
  
  err_retnull ( VPtRun->PtFrame, (ERR_OUT,"Abort : EUDET frames buffer not allocated !") );
  
  // Reset frame list pointer
  
  VPtFrList->TotFrameNb = 0;
  
  memset ( VPtFrList->AFramePtr, 0, EFRIO__MAX_FRAME_NB_PER_ACQ * sizeof (EFRIO__TFrame*) );
  
  // Calculate size of records WITHOUT variable length part counted here as one elt
  
  VEmptyTrigRecSz  = sizeof ( EFRIO__TTriggerRec );
  VEmptyFrameRecSz = sizeof ( EFRIO__TFrame );
  
  //  err_error (( ERR_OUT, "TRACE => VEmptyTrigRecSz  = %d", VEmptyTrigRecSz  ));
  //  err_error (( ERR_OUT, "TRACE => VEmptyFrameRecSz = %d", VEmptyFrameRecSz ));
  
  // Reset tmp triggers buffer

  memset ( VPtTmpTrigRec, 0, sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );
  
  // Extract data
  
  ViSrcW32           = 0;
  VDataLengthErrCnt  = 0;
  
  for ( ViFrame=0; ViFrame < VPtBoard->FrameNbPerAcq; ViFrame++ ) {

    VEChanTrigFieldCnt = 0;
  
    // Update pointer to next frame
    
    if ( VPtFrList->TotFrameNb == 0 ) {
      VPtFrame = VPtFrList->AFramePtr[0] = VPtRun->PtFrame;
    }
    
    else {
      
      if ( VPtFrList->TotFrameNb >= VPtCont->RunCont.ParFrameNbPerAcq ) {
        err_retfail ( -1, (ERR_OUT,"Abort try to process frame %d > Max = %d ", VPtFrList->TotFrameNb, VPtCont->RunCont.ParFrameNbPerAcq) );
      }
      
      VPtFrame = VPtFrList->AFramePtr[VPtFrList->TotFrameNb] = (EFRIO__TFrame*) ((UInt8*) VPtFrList->AFramePtr[VPtFrList->TotFrameNb - 1] + VFullFrameRecSz);
      
      // err_error (( ERR_OUT, "TRACE : Set pointer frame %d - pointer = %d [D]", VPtFrList->TotFrameNb, VPtFrList->AFramePtr[VPtFrList->TotFrameNb] ));
    }
    
  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtFrame->Tag        = EFRIO__FRAME_TAG;
    VPtFrame->Header.Tag                = EFRIO__FRAME_TAG_HEADER;
  #endif
  
    VPtFrame->Header.AcqId              = VAcqId;
    VPtFrame->Header.FrameIdInAcq       = ViFrame;
    VPtFrame->Header.MapsName           = (UInt16) ASIC__MI26;
    VPtFrame->Header.MapsNb             = VPtRun->ParMi26Nb;
    
    VPtFrame->Header.AMapsHeader[0]          = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    ++VEChanTrigFieldCnt;
    
    VPtFrame->Header.AMapsFrameCnt[0]        = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    ++VEChanTrigFieldCnt;
    
    
    VDataLengthField                         = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    ++VEChanTrigFieldCnt;
    
    VDataLengthW16 = (VDataLengthField & 0x0000FFFF) + ((VDataLengthField & 0xFFFF0000) >> 16);
    
    VDbgDataLenghtD0 = (VDataLengthField & 0x0000FFFF);
    VDbgDataLenghtD1 = ((VDataLengthField & 0xFFFF0000) >> 16);
    
    if ( (VDbgDataLenghtD0 > 570) || (VDbgDataLenghtD1 > 570) ) {
      VDataLengthW16 = 0;
      ++VDataLengthErrCnt;
      err_error (( ERR_OUT, "HW error on data length - ViFrame=%d -> D0=%4d [D] - D1=%4d [D] -> Force 0 !", ViFrame, VDbgDataLenghtD0, VDbgDataLenghtD1 ));
      err_error (( ERR_OUT, "HW error on data length - ViFrame=%d -> D0=%4x [H] - D1=%4x [H] -> Force 0 !", ViFrame, VDbgDataLenghtD0, VDbgDataLenghtD1 ));
    }
    
    VDataLengthW8  = VDataLengthW16 * 2;
    VDataLengthW32 = VDataLengthW16 / 2;
    
    
    VPtFrame->Header.AMapsDataLength[0] = VDataLengthW16 * 2;
    
    // Copy only the useful data

  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtFrame->Data.Tag       = EFRIO__FRAME_TAG_DATA;
  #endif
  
    VPtFrame->Data.TotSz     = VDataLengthW8;
    VPtFrame->Data.OneMapsSz = VDataLengthW8;
    
    // err_error (( ERR_OUT, "TRACE -> Before loop - ViFrame=%d - VDataLengthW32=%d", ViFrame, VDataLengthW32 ));
    
    ViSrcW32BeforeDataCpyLoop = ViSrcW32;
    
    for ( ViDataW32=0; ViDataW32 < VDataLengthW32; ViDataW32++ ) {
      VPtFrame->Data.ADataW32[ViDataW32] = PtSrcW32[ViSrcW32];
      ++ViSrcW32;
      VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = PtSrcW32[ViSrcW32];
      ++ViSrcW32;
      ++VEChanTrigFieldCnt;
    }
        
    // Continue extraction of trigger fields from trigger channel WHILE needed

    // WARNING => Add test to avoid to read after end of current frame in case no last trigger info is found !!!

    ++ViSrcW32; // To bypass current W32 with is Mi26 data NOT trigger channel field
        
    do {

      VEChanTrigField = PtSrcW32[ViSrcW32];
      
      if ( VEChanTrigFieldCnt >= EFRIO__EXTRA_CHAN__MAX_TRIGGER_FIELD_NB ) {
        err_warning (( ERR_OUT, "Max trigger field nb reached = %d <=> %d max trigger info !", EFRIO__EXTRA_CHAN__MAX_TRIGGER_FIELD_NB, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ));
        break;
      }
      
      VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = VEChanTrigField; 
      ViSrcW32 += 2;
      ++VEChanTrigFieldCnt;
    } while ( (VEChanTrigField & 0x80000000) == 0 );      

    // Update ViSrcW32 for following processing
    
    // ViSrcW32 = ViSrcW32 + ( MI26__ZS_FFRAME_RAW_MAX_W32 - VDataLengthW32 );
    
    ViSrcW32 = ViSrcW32BeforeDataCpyLoop + ( 2 * MI26__ZS_FFRAME_RAW_MAX_W32 );
    
    // err_error (( ERR_OUT, "TRACE -> After loop - ViFrame=%d - VDataLengthW32=%d", ViFrame, VDataLengthW32 ));
    
    VPtFrame->Header.AMapsTrailer[0] = PtSrcW32[(2 * MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame) + (2 * (3 + VDataLengthW32))];    // 3 = Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32; // Count Trailer field
    ++ViSrcW32; // Count extra channel trigger field
    
    // Read trigger number from "Mi26" frame
    
    if ( TrigStatus == 0 ) {
      VZero = PtSrcW32[(2 * MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame) + (2 * (3 + MI26__ZS_FFRAME_RAW_MAX_W32 + 1))];
    }
    
    // Emulate trigger by overwriting info from "Mi26" frame
    
    else {
      VZero = (TrigStatus << 16);
    }
    
    ++ViSrcW32; // Count Zero field
    ++ViSrcW32; // Count extra channel trigger field
    
    VZero2 = PtSrcW32[(2 * MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame) + (2 * (3 + MI26__ZS_FFRAME_RAW_MAX_W32 + 2))];
    ++ViSrcW32; // Count Zero2 field
    ++ViSrcW32; // Count extra channel trigger field
    
    // Extraction of trigger info from "Mimosa 26 frame" zero fields
    
    VTrigNb = (VZero  & 0xFFFF0000) >> 16;

    if ( VTrigNb > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ) {
      err_error (( ERR_OUT, "VTrigNb=%d > Max=%d => Limit to max !", VTrigNb, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ));
      VTrigNb = EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB;
    }
    

    if ( VTrigNb != 0 ) {
      VATrigVal[0]    = (VZero  & 0x0000FFFF);
      VATrigVal[1]    = (VZero2 & 0xFFFF0000) >> 16;
      VATrigVal[2]    = (VZero2 & 0x0000FFFF);
      
      VATrigLine[0]   = VATrigVal[0] / 16;
      VATrigLine[1]   = VATrigVal[1] / 16;
      VATrigLine[2]   = VATrigVal[2] / 16;
      
      VATrigClk[0]    = VATrigVal[0] % 16;
      VATrigClk[1]    = VATrigVal[1] % 16;
      VATrigClk[2]    = VATrigVal[2] % 16;
      
      VAMi26Trig[0]   = VATrigLine[0] /* + ( VATrigClk[0] << 10 ) */;
      VAMi26Trig[1]   = VATrigLine[1] /* + ( VATrigClk[1] << 10 ) */;
      VAMi26Trig[2]   = VATrigLine[2] /* + ( VATrigClk[2] << 10 ) */;
    }
    
    else {
      VAMi26Trig[0] = VAMi26Trig[1] = VAMi26Trig[2] = 0;
    }
    
    // Add trigger info in frame header
    
    VPtFrame->Header.TriggerNb        = VTrigNb;
    VPtFrame->Header.AMapsTrigInfo[0] = VAMi26Trig[0];
    VPtFrame->Header.AMapsTrigInfo[1] = VAMi26Trig[1];
    VPtFrame->Header.AMapsTrigInfo[2] = VAMi26Trig[2];
    
    // Add trigger info in trigger record
    
    VPtTrigRec = (EFRIO__TTriggerRec*) (VPtFrame->Data.ADataW32 + VDataLengthW32);
    
  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtTmpTrigRec->Tag      = EFRIO__FRAME_TAG_TRIG;
  #endif
  
    VPtTmpTrigRec->TrigNb   = VTrigNb;
    VPtTmpTrigRec->TotSz    = VEmptyTrigRecSz + (VTrigNb * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ);
    VPtTmpTrigRec->TrigType = 2;
    
    memcpy ( VPtTrigRec, VPtTmpTrigRec, VPtTmpTrigRec->TotSz );
  
    // Calculate size of frame record while filled ( fixed part + variable length (data & trigger) )
    
    // Calculate size will be 2 x W32 higher than needed, because following fields will be counted twice
    //
    // - Field UInt32 ADataW32[0] of data record -> EFRIO__TFrameData
    // - Field UInt32 ATrig[0] of trigger record -> EFRIO__TTriggerRec
    
    VFullFrameRecSz = VEmptyFrameRecSz + VDataLengthW8 + VPtTrigRec->TotSz;
    
    VTotAcqSz += VFullFrameRecSz;
    
    // Update frame field total size & trigger offset size
    
    VPtFrame->TotSz         = VFullFrameRecSz;
    VPtFrame->TrigRecOffset = VEmptyFrameRecSz + VDataLengthW8;
    
    if ( 0 /* VPtServ->PtParAcqReqFunc->AsicChkFunc >= 0 */ ) {
      
      // No neeed to reset ResAsicErrorsRejCurAcq ( it's done on DPXI__MI26_FAsicChkFunc0Emul6Mi26 with ViFrame = 0 )
      // => ResAsicErrorsRejCurAcq = 1 if errors in current Acq
      
      // $$$ PtAcq->ResAsicErrorsRejCurAcq = DPXI__MI26_FAsicChkFuncMi26 ( VPtServ->PtParAcqReqFunc->AsicChkFunc, ViFrame, VptZsFFrameRaw, 1 /* Mi26Nb */ );
      // $$$ PtAcq->ResAsicErrorsRejCurAcq = 0;
      
      // $$$ if ( PtAcq->ResAsicErrorsRejCurAcq != 0 ) {
        // $$$   msg (( MSG_OUT, "ViFrame=%d - VAsicErrorsRejCurAcq=%d", ViFrame, PtAcq->ResAsicErrorsRejCurAcq ));
        // $$$ }
    }
    
    
    ++VPtFrList->TotFrameNb;
    
    ++VRunFrameCnt;
    
  } // End for ViFrame


  ++VPtCont->RunCont.ResAcqCnt;

  PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );  

  return (VTotAcqSz);
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    :
:
Level     : This is a user level function.
Date      : 29/10/2010
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet2Mode6Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt8 AcqStatus, SInt16 TrigStatus, SInt8 DataConvertMode ) {
  
  EFRIO__TContext*     VPtCont   = &EFRIO__VGContext;
  EFRIO__TBoardConf*   VPtBoard  = NULL;
  EFRIO__TRunCont*     VPtRun    = &VPtCont->RunCont;
  EFRIO__TFrameList*   VPtFrList = &VPtCont->AAcqFrameList[0];
  EFRIO__TFrame*       VPtFrame;
  EFRIO__TTriggerRec*  VPtTmpTrigRec = VPtCont->PtTmpTrigRec;
  
  SInt32               VAcqId;
  UInt8*               VPtAcqData;
  SInt32               VFrameNbFromBoardDrv;
  SInt32               ViFrame;
  SInt32               V7iFrame;
  UInt32               VADataLengthField[6];
  UInt32               VADataLengthW8[6];
  UInt16               VADataLengthW16[6];
  UInt32               VADataLengthW32[6];
  UInt32               VDataLengthW8Max;
  UInt32               VDataLengthW16Max;
  UInt32               VDataLengthW32Max;
  UInt32               VDataLengthW32ToCpy;
  UInt32               VDataLengthW8ToCpy;
  SInt32               ViSrcW32;
  SInt32               ViDataCpy;
  SInt32               ViDataCpyP1;
  SInt32               ViDataCpyP2;
  SInt32               VRunFrameCnt;
  SInt32               VLastFrameWithTrigAllowed;
  UInt32               VZero;
  UInt32               VZero2;
  SInt16               VTrigNb;
  UInt16               VATrigVal[3];
  UInt16               VATrigLine[3];
  UInt16               VATrigClk[3];
  UInt16               VAMi26Trig[3];
  EFRIO__TTriggerRec*  VPtTrigRec;
  UInt32               VFullFrameRecSz;
  UInt32               VEmptyFrameRecSz;
  UInt32               VEmptyTrigRecSz;
  SInt32               VDataLengthErrCnt;
  SInt8                ViMi26;
  UInt64*              VPtCpySrcW64;
  UInt64*              VPtCpyDestW64;
  UInt32*              VPtEChanSrcW32;
  SInt16               VEChanTrigFieldCnt; // Extra channel trigger fields counter
  UInt32               VEChanTrigField;
  SInt32               VTotAcqSz;
  

  
  PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
  
  if ( VPtCont->InfInitDone == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort because : Init not done") );
  }
  
  err_trace (( ERR_OUT, "EFRIO__MI26_FFRioAcqDeserDataEudet1Mode1Mi26 (P=%x, EltNb=%d)", PtSrcW32, EltNb ));
  
  // Pointers parameters check
  
  err_retnull ( PtSrcW32, (ERR_OUT,"PtSrcW32 = NULL") );
  
  // Init board conf record pointer
  
  if ( (BoardId < 0) || (BoardId >= EFRIO__MAX_BOARDS_NB) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Board Id=%d out of range [0..%d])", BoardId, EFRIO__MAX_BOARDS_NB - 1) );
  }
  
  VPtBoard = &EFRIO__VGContext.ABoardsConf[BoardId];
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ) / 7; // Divide by 7 because of extral channel
  
  // It must be => ( (EltNb * 2) / (MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ * 2) );
  // But result is the SAME if we divide both part of fraction by 2 ;-)
  
  if ( VFrameNbFromBoardDrv != VPtBoard->FrameNbPerAcq ) {
    err_retfail ( -1, (ERR_OUT,"Bad number of samples : %d frames acquired <> %d frames requested", VFrameNbFromBoardDrv, VPtBoard->FrameNbPerAcq ) );
  }

  // Reset total size of acquisition
  
  VTotAcqSz = 0;
  
  // Get acq id
  
  VAcqId = VPtCont->RunCont.ResAcqCnt;
  
  // Get last frame with trigger allowed / acq
  
  VLastFrameWithTrigAllowed = VPtBoard->FrameNbPerAcq - 1; // Last n frames of Acq MUST have NO trigger information - NOW n = 0 !!!
  
  // Check if buffer is allocated ( this is total buffer, not only the part where we will write )
  
  err_retnull ( VPtRun->PtFrame, (ERR_OUT,"Abort : EUDET frames buffer not allocated !") );
  
  // Reset frame list pointer
  
  VPtFrList->TotFrameNb = 0;
  
  memset ( VPtFrList->AFramePtr, 0, EFRIO__MAX_FRAME_NB_PER_ACQ * sizeof (EFRIO__TFrame*) );
  
  // Calculate size of records WITHOUT variable length part counted here as one elt
  
  VEmptyTrigRecSz  = sizeof ( EFRIO__TTriggerRec );
  VEmptyFrameRecSz = sizeof ( EFRIO__TFrame );
  
  //  err_error (( ERR_OUT, "TRACE => VEmptyTrigRecSz  = %d", VEmptyTrigRecSz  ));
  //  err_error (( ERR_OUT, "TRACE => VEmptyFrameRecSz = %d", VEmptyFrameRecSz ));


  // Reset tmp triggers buffer
  
  memset ( VPtTmpTrigRec, 0, sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );

  // Extract data
  
  ViSrcW32          = 0;
  VDataLengthErrCnt = 0;
  
  for ( ViFrame=0; ViFrame < VPtBoard->FrameNbPerAcq; ViFrame++ ) {
    
    V7iFrame = 7 * ViFrame;
    
    VEChanTrigFieldCnt = 0;
    
    // Update pointer to next frame
    
    if ( VPtFrList->TotFrameNb == 0 ) {
      VPtFrame = VPtFrList->AFramePtr[0] = VPtRun->PtFrame;
    }
    
    else {
      
      if ( VPtFrList->TotFrameNb >= VPtCont->RunCont.ParFrameNbPerAcq ) {
        err_retfail ( -1, (ERR_OUT,"Abort try to process frame %d > Max = %d ", VPtFrList->TotFrameNb, VPtCont->RunCont.ParFrameNbPerAcq) );
      }
      
      VPtFrame = VPtFrList->AFramePtr[VPtFrList->TotFrameNb] = (EFRIO__TFrame*) ((UInt8*) VPtFrList->AFramePtr[VPtFrList->TotFrameNb - 1] + VFullFrameRecSz);
      
      // err_error (( ERR_OUT, "TRACE : Set pointer frame %d - pointer = %d [D]", VPtFrList->TotFrameNb, VPtFrList->AFramePtr[VPtFrList->TotFrameNb] ));
    }

  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtFrame->Tag        = EFRIO__FRAME_TAG;
    VPtFrame->Header.Tag          = EFRIO__FRAME_TAG_HEADER;
  #endif
  
    
    VPtFrame->Header.AcqId        = VAcqId;
    VPtFrame->Header.FrameIdInAcq = ViFrame;
    VPtFrame->Header.MapsName     = (UInt16) ASIC__MI26;
    VPtFrame->Header.MapsNb       = VPtRun->ParMi26Nb;
    
    VPtFrame->Header.AMapsHeader[0]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsHeader[1]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsHeader[2]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsHeader[3]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsHeader[4]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsHeader[5]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    ++VEChanTrigFieldCnt;
    
    
    VPtFrame->Header.AMapsFrameCnt[0]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsFrameCnt[1]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsFrameCnt[2]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsFrameCnt[3]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsFrameCnt[4]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsFrameCnt[5]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    ++VEChanTrigFieldCnt;
    

    VADataLengthField[0]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[1]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[2]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[3]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[4]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[5]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    ++VEChanTrigFieldCnt;
    

    VADataLengthW16[0] = (VADataLengthField[0] & 0x0000FFFF) + ((VADataLengthField[0] & 0xFFFF0000) >> 16);
    VADataLengthW16[1] = (VADataLengthField[1] & 0x0000FFFF) + ((VADataLengthField[1] & 0xFFFF0000) >> 16);
    VADataLengthW16[2] = (VADataLengthField[2] & 0x0000FFFF) + ((VADataLengthField[2] & 0xFFFF0000) >> 16);
    VADataLengthW16[3] = (VADataLengthField[3] & 0x0000FFFF) + ((VADataLengthField[3] & 0xFFFF0000) >> 16);
    VADataLengthW16[4] = (VADataLengthField[4] & 0x0000FFFF) + ((VADataLengthField[4] & 0xFFFF0000) >> 16);
    VADataLengthW16[5] = (VADataLengthField[5] & 0x0000FFFF) + ((VADataLengthField[5] & 0xFFFF0000) >> 16);
    
    VDataLengthW16Max = MATH_FUInt16Max ( VADataLengthW16, 6 );
    
    
    // A better test ( on each VADataLengthW16[i] ? on each W16 on D0 & D1 ? ) should be done !
    
    if ( VDataLengthW16Max > 2304 ) {
      memset ( VADataLengthW8 , 0, 12 );
      memset ( VADataLengthW16, 0, 12 );
      memset ( VADataLengthW32, 0, 12 );
      
      VDataLengthW8Max  = 0;
      VDataLengthW32Max = 0;
      
      ++VDataLengthErrCnt;
      err_error (( ERR_OUT, "HW error on data length -> Max W16 nb = %d -> Force 0 !", VDataLengthW16Max ));
    }
    
    else {
      
      for ( ViMi26=0; ViMi26 < 6; ViMi26++ ) {
        VADataLengthW8[ViMi26]  = VADataLengthW16[ViMi26] * 2;
        VADataLengthW32[ViMi26] = VADataLengthW16[ViMi26] / 2;
      }
      
      VDataLengthW8Max  = VDataLengthW16Max * 2;
      VDataLengthW32Max = VDataLengthW16Max / 2;
    }
    
    VPtFrame->Header.AMapsDataLength[0] = VADataLengthW8[0];
    VPtFrame->Header.AMapsDataLength[1] = VADataLengthW8[1];
    VPtFrame->Header.AMapsDataLength[2] = VADataLengthW8[2];
    VPtFrame->Header.AMapsDataLength[3] = VADataLengthW8[3];
    VPtFrame->Header.AMapsDataLength[4] = VADataLengthW8[4];
    VPtFrame->Header.AMapsDataLength[5] = VADataLengthW8[5];
    
    // Copy useful data part
    
    VDataLengthW32ToCpy = VDataLengthW32Max * 6;
    VDataLengthW8ToCpy  = VDataLengthW32ToCpy * 4;
    
  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtFrame->Data.Tag       = EFRIO__FRAME_TAG_DATA;
  #endif
  
    
    VPtFrame->Data.TotSz     = VDataLengthW8ToCpy;
    VPtFrame->Data.OneMapsSz = VDataLengthW8Max;
    
    // err_error (( ERR_OUT, "TRACE -> Before loop - ViFrame=%d - VDataLengthW32=%d", ViFrame, VDataLengthW32 ));
    
    
    // A remplacer par memcpy !!!!!!!!!
    
    
    // memcpy ( VPtFrame->Data.ADataW32, &PtSrcW32[ViSrcW32], VDataLengthW8ToCpy  );
    
    // err_error (( ERR_OUT, "TRACE => VDataLengthW8ToCpy=%d", VDataLengthW8ToCpy ));
    
    VPtCpySrcW64  = (UInt64*) &PtSrcW32[ViSrcW32];
    VPtCpyDestW64 = (UInt64*) VPtFrame->Data.ADataW32;



    for ( ViDataCpy=0; ViDataCpy < VDataLengthW32Max; ViDataCpy++ ) {
            
      *VPtCpyDestW64 = *VPtCpySrcW64;
      ++VPtCpyDestW64;
      ++VPtCpySrcW64;
      *VPtCpyDestW64 = *VPtCpySrcW64;
      ++VPtCpyDestW64;
      ++VPtCpySrcW64;
      *VPtCpyDestW64 = *VPtCpySrcW64;
      ++VPtCpyDestW64;
      ++VPtCpySrcW64;
      
      VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = *((UInt32*) VPtCpySrcW64);
      (UInt32) VPtCpySrcW64 = (UInt32) (VPtCpySrcW64) + 4;
      ++VEChanTrigFieldCnt;
    }
    
  
    

    VPtEChanSrcW32 = (UInt32*) VPtCpySrcW64;
  
    // Continue extraction of trigger fields from trigger channel WHILE needed
        
    VPtEChanSrcW32 += 6; // Bypass Mi26 x 6 data
    
    do {
      VEChanTrigField = *VPtEChanSrcW32;
      
      if ( VEChanTrigFieldCnt >= EFRIO__EXTRA_CHAN__MAX_TRIGGER_FIELD_NB ) {
        err_warning (( ERR_OUT, "Max trigger field nb reached = %d <=> %d max trigger info !", EFRIO__EXTRA_CHAN__MAX_TRIGGER_FIELD_NB, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ));
        break;
      }
      
      VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = VEChanTrigField;
      VPtEChanSrcW32 += 7;
      ++VEChanTrigFieldCnt;
    } while ( (VEChanTrigField & 0x80000000) == 0 );
    
    
    ViSrcW32 += (7 * MI26__ZS_FFRAME_RAW_MAX_W32);
    
    
    //    VptZsFFrameRaw[V6iFrame].Trailer = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * VADataLengthW32[0])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    //    ++ViSrcW32;
    
    //    VZero = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 6];
    //    VptZsFFrameRaw[V6iFrame].Zero = VZero;
    //    ++ViSrcW32;
    
    //    VZero2 = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 12];
    //    VptZsFFrameRaw[V6iFrame].Zero2 = VZero2;
    //    ++ViSrcW32;
    
    
    VPtFrame->Header.AMapsTrailer[0] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + (7 * VADataLengthW32[0])];    // 21 = 7 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[1] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + 1 + (7 * VADataLengthW32[1])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[2] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + 2 + (7 * VADataLengthW32[2])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[3] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + 3 + (7 * VADataLengthW32[3])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[4] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + 4 + (7 * VADataLengthW32[4])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[5] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + 5 + (7 * VADataLengthW32[5])];
    ++ViSrcW32;
    
    ++ViSrcW32; // To count extra channel
    
    
    // Read trigger number from "Mi26" frame
    
    if ( TrigStatus == 0 ) {
      VZero  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) +  7];
    }
    
    // Emulate trigger by overwriting info from "Mi26" frame
    
    else {
      VZero = (TrigStatus << 16);
    }
    
    // VZero  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) +  7];
    
    
    
    VZero2 = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 14];
    
    ViSrcW32 += 14; // 7 times 2 zero fields = 14
    
    // Extraction of trigger info from "Mimosa 26 frame" zero fields
    
    VTrigNb = (VZero  & 0xFFFF0000) >> 16;
  
    


    if ( VTrigNb > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ) {
      err_warning (( ERR_OUT, "VTrigNb=%d > Max=%d => Limit to max !", VTrigNb, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ));
      VTrigNb = EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB;
    }
    
    if ( VTrigNb != 0 ) {
      VATrigVal[0]    = (VZero  & 0x0000FFFF);
      VATrigVal[1]    = (VZero2 & 0xFFFF0000) >> 16;
      VATrigVal[2]    = (VZero2 & 0x0000FFFF);
      
      VATrigLine[0]   = VATrigVal[0] / 16;
      VATrigLine[1]   = VATrigVal[1] / 16;
      VATrigLine[2]   = VATrigVal[2] / 16;
      
      VATrigClk[0]    = VATrigVal[0] % 16;
      VATrigClk[1]    = VATrigVal[1] % 16;
      VATrigClk[2]    = VATrigVal[2] % 16;
      
      VAMi26Trig[0]   = VATrigLine[0] /* + ( VATrigClk[0] << 10 ) */;
      VAMi26Trig[1]   = VATrigLine[1] /* + ( VATrigClk[1] << 10 ) */;
      VAMi26Trig[2]   = VATrigLine[2] /* + ( VATrigClk[2] << 10 ) */;
    }
    
    else {
      VAMi26Trig[0] = VAMi26Trig[1] = VAMi26Trig[2] = 0;
    }
    
    // Add trigger info in frame header
    
    VPtFrame->Header.TriggerNb        = VTrigNb;
    VPtFrame->Header.AMapsTrigInfo[0] = VAMi26Trig[0];
    VPtFrame->Header.AMapsTrigInfo[1] = VAMi26Trig[1];
    VPtFrame->Header.AMapsTrigInfo[2] = VAMi26Trig[2];
  
    // Add trigger info in trigger record
    
    VPtTrigRec = (EFRIO__TTriggerRec*) (VPtFrame->Data.ADataW32 + VDataLengthW32ToCpy);
    
  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtTmpTrigRec->Tag      = EFRIO__FRAME_TAG_TRIG;
  #endif
  
    
    VPtTmpTrigRec->TrigNb   = VTrigNb;
    VPtTmpTrigRec->TotSz    = VEmptyTrigRecSz + (VTrigNb * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ);
    VPtTmpTrigRec->TrigType = 2;
    
    memcpy ( VPtTrigRec, VPtTmpTrigRec, VPtTmpTrigRec->TotSz );

    // Calculate size of frame record while filled ( fixed part + variable length (data & trigger) )
    
    // Calculate size will be 2 x W32 higher than needed, because following fields will be counted twice
    //
    // - Field UInt32 ADataW32[0] of data record -> EFRIO__TFrameData
    // - Field UInt32 ATrig[0] of trigger record -> EFRIO__TTriggerRec
    
    VFullFrameRecSz = VEmptyFrameRecSz + VDataLengthW8ToCpy + VPtTrigRec->TotSz;

    VTotAcqSz += VFullFrameRecSz;
    
    // Update frame field total size & trigger offset size
    
    VPtFrame->TotSz         = VFullFrameRecSz;
    VPtFrame->TrigRecOffset = VEmptyFrameRecSz + VDataLengthW8ToCpy;
    
    if ( 0 /* VPtServ->PtParAcqReqFunc->AsicChkFunc >= 0 */ ) {
      
      // No neeed to reset ResAsicErrorsRejCurAcq ( it's done on DPXI__MI26_FAsicChkFunc0Emul6Mi26 with ViFrame = 0 )
      // => ResAsicErrorsRejCurAcq = 1 if errors in current Acq
      
      // $$$ PtAcq->ResAsicErrorsRejCurAcq = DPXI__MI26_FAsicChkFuncMi26 ( VPtServ->PtParAcqReqFunc->AsicChkFunc, ViFrame, VptZsFFrameRaw, 1 /* Mi26Nb */ );
      // $$$ PtAcq->ResAsicErrorsRejCurAcq = 0;
      
      // $$$ if ( PtAcq->ResAsicErrorsRejCurAcq != 0 ) {
        // $$$   msg (( MSG_OUT, "ViFrame=%d - VAsicErrorsRejCurAcq=%d", ViFrame, PtAcq->ResAsicErrorsRejCurAcq ));
        // $$$ }
    }
    
    
    ++VPtFrList->TotFrameNb;
    
    ++VRunFrameCnt;
    
  } // End for ViFrame
  
  ++VPtCont->RunCont.ResAcqCnt;
  
  PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
  
  return (VTotAcqSz);
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    :
:
Level     : This is a user level function.
Date      : 03/11/2010
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet3Mode6Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt8 AcqStatus, SInt16 TrigStatus, SInt8 DataConvertMode ) {
    
  EFRIO__TContext*     VPtCont   = &EFRIO__VGContext;
  EFRIO__TBoardConf*   VPtBoard  = NULL;
  EFRIO__TRunCont*     VPtRun    = &VPtCont->RunCont;
  EFRIO__TFrameList*   VPtFrList = &VPtCont->AAcqFrameList[0];
  EFRIO__TFrame*       VPtFrame;
  EFRIO__TTriggerRec*  VPtTmpTrigRec = VPtCont->PtTmpTrigRec;
  
  SInt32               VAcqId;
  UInt8*               VPtAcqData;
  SInt32               VFrameNbFromBoardDrv;
  SInt32               VFrameId;
  SInt16               ViFrameWithTrig;
  SInt32               V7FrameId;
  UInt32               VADataLengthField[6];
  UInt32               VADataLengthW8[6];
  UInt16               VADataLengthW16[6];
  UInt32               VADataLengthW32[6];
  UInt32               VDataLengthW8Max;
  UInt32               VDataLengthW16Max;
  UInt32               VDataLengthW32Max;
  UInt32               VDataLengthW32ToCpy;
  UInt32               VDataLengthW8ToCpy;
  SInt32               ViSrcW32;
  SInt32               ViDataCpy;
  SInt32               ViDataCpyP1;
  SInt32               ViDataCpyP2;
  SInt32               VRunFrameCnt;
  SInt32               VLastFrameWithTrigAllowed;
  UInt32               VZero;
  UInt32               VZero2;
  SInt16               VTrigNb;
  UInt16               VATrigVal[3];
  UInt16               VATrigLine[3];
  UInt16               VATrigClk[3];
  UInt16               VAMi26Trig[3];
  EFRIO__TTriggerRec*  VPtTrigRec;
  UInt32               VFullFrameRecSz;
  UInt32               VEmptyFrameRecSz;
  UInt32               VEmptyTrigRecSz;
  SInt32               VDataLengthErrCnt;
  SInt8                ViMi26;
  UInt64*              VPtCpySrcW64;
  UInt64*              VPtCpyDestW64;
  UInt32*              VPtEChanSrcW32;
  SInt16               VEChanTrigFieldCnt; // Extra channel trigger fields counter
  UInt32               VEChanTrigField;
  
  SInt32               VFrameWithTrigCnt;
  SInt32               VLastFrameWithTrig;
  SInt16*              VPtFrameWithTrigList;
  static SInt32        VFrNbToTakeAtBeginOfAcqForLastTrig = -1; // Number of frames to take at beginning of Acq to complete last trigger of previous Acq
  SInt32               VTotAcqSz;
  
  
  
  PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
  
  if ( VPtCont->InfInitDone == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort because : Init not done") );
  }
  
  err_trace (( ERR_OUT, "EFRIO__MI26_FFRioAcqDeserDataEudet1Mode1Mi26 (P=%x, EltNb=%d)", PtSrcW32, EltNb ));
  
  // Pointers parameters check
  
  err_retnull ( PtSrcW32, (ERR_OUT,"PtSrcW32 = NULL") );
  
  // Init board conf record pointer
  
  if ( (BoardId < 0) || (BoardId >= EFRIO__MAX_BOARDS_NB) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Board Id=%d out of range [0..%d])", BoardId, EFRIO__MAX_BOARDS_NB - 1) );
  }
  
  VPtBoard = &EFRIO__VGContext.ABoardsConf[BoardId];
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ) / 7; // Divide by 7 because of extral channel
  
  // It must be => ( (EltNb * 2) / (MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ * 2) );
  // But result is the SAME if we divide both part of fraction by 2 ;-)
  
  if ( VFrameNbFromBoardDrv != VPtBoard->FrameNbPerAcq ) {
    err_retfail ( -1, (ERR_OUT,"Bad number of samples : %d frames acquired <> %d frames requested", VFrameNbFromBoardDrv, VPtBoard->FrameNbPerAcq ) );
  }

  // Reset total size of acquisition
  
  VTotAcqSz = 0;
  
  // Get acq id
  
  VAcqId = VPtCont->RunCont.ResAcqCnt;
  
  // Get last frame with trigger allowed / acq
  
  VLastFrameWithTrigAllowed = VPtBoard->FrameNbPerAcq - 1; // Last n frames of Acq MUST have NO trigger information - NOW n = 0 !!!
  
  // Check if buffer is allocated ( this is total buffer, not only the part where we will write )
  
  err_retnull ( VPtRun->PtFrame, (ERR_OUT,"Abort : EUDET frames buffer not allocated !") );
  
  // Reset frame list pointer
  
  VPtFrList->TotFrameNb = 0;
  
  memset ( VPtFrList->AFramePtr, 0, EFRIO__MAX_FRAME_NB_PER_ACQ * sizeof (EFRIO__TFrame*) );
  
  // Calculate size of records WITHOUT variable length part counted here as one elt
  
  VEmptyTrigRecSz  = sizeof ( EFRIO__TTriggerRec );
  VEmptyFrameRecSz = sizeof ( EFRIO__TFrame );
  
  //  err_error (( ERR_OUT, "TRACE => VEmptyTrigRecSz  = %d", VEmptyTrigRecSz  ));
  //  err_error (( ERR_OUT, "TRACE => VEmptyFrameRecSz = %d", VEmptyFrameRecSz ));
  
  
  // Reset tmp triggers buffer
  
  memset ( VPtTmpTrigRec, 0, sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );


  PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
  
  // ----------------------------------------------------------------------------------
  // List frames to extract => frame with trigger + 3 following frames
  // ----------------------------------------------------------------------------------
  
  memset ( VPtCont->AAAcqFrameWithTrigList[0], 0xFF, EFRIO__MAX_FRAME_NB_PER_ACQ * 2 );
    
  // Init loop parameters
  
  VFrameWithTrigCnt    =  0;
  VLastFrameWithTrig   = -1;
  VPtFrameWithTrigList = VPtCont->AAAcqFrameWithTrigList[0];
  
  for ( VFrameId=0; VFrameId < VPtBoard->FrameNbPerAcq; VFrameId++ ) {
    
    V7FrameId = 7 * VFrameId;
    
    if ( TrigStatus == 0 ) {
      VZero  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) +  7];
    }
    
    // Emulate trigger by overwriting info from "Mi26" frame
    // BUT first read it from board RAM in order to get real execution time
    
    else {
      VZero  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) +  7];
      VZero = (TrigStatus << 16);
    }
    
    VTrigNb = (VZero  & 0xFFFF0000) >> 16;
    
    if ( VTrigNb != 0 ) {
      VLastFrameWithTrig = VFrameId;
      VPtFrameWithTrigList[VFrameWithTrigCnt] = VFrameId;
      ++VFrameWithTrigCnt;
    }
    
    else {
      
      if ( (VFrameId < VFrNbToTakeAtBeginOfAcqForLastTrig) || ((VLastFrameWithTrig != -1) && ((VFrameId - VLastFrameWithTrig) <= EFRIO__FRAME_NB_TO_READ_AFTER_TRIG)) ) {
        VPtFrameWithTrigList[VFrameWithTrigCnt] = VFrameId;
        ++VFrameWithTrigCnt;
      }
      
    } // End else
        
  } // End for ( ViFrame )


  // Calculate nb of frame to store on beginning of next acq to complete last trigger of current one
  // If last trigger occurs before the last EFRIO__FRAME_NB_TO_READ_AFTER_TRIG frames, the result will be 0 or < 0
  // in this case it will be ignored by processing loop
  
  VFrNbToTakeAtBeginOfAcqForLastTrig = EFRIO__FRAME_NB_TO_READ_AFTER_TRIG - (VPtBoard->FrameNbPerAcq - 1 - VLastFrameWithTrig);
  
  
  PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
  
  
  // Print list of frames to extract

/*
  msg (( MSG_OUT, "=========================================================" ));
  
  for ( ViFrameWithTrig=0; ViFrameWithTrig < VFrameWithTrigCnt; ViFrameWithTrig++ ) {
    msg (( MSG_OUT, "Readout => Trig on frame %.4d", VPtCont->AAAcqFrameWithTrigList[0][ViFrameWithTrig] ));
  }  
*/

  // Extract data
  
  VDataLengthErrCnt = 0;
  
  for ( ViFrameWithTrig=0; ViFrameWithTrig < VFrameWithTrigCnt; ViFrameWithTrig++ ) {
    
    VFrameId           = VPtFrameWithTrigList[ViFrameWithTrig];
    VEChanTrigFieldCnt = 0;
    ViSrcW32           = VFrameId * 7 * MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32;
    
    V7FrameId = 7 * VFrameId;
        
    // Update pointer to next frame
    
    if ( VPtFrList->TotFrameNb == 0 ) {
      VPtFrame = VPtFrList->AFramePtr[0] = VPtRun->PtFrame;
    }
    
    else {
      
      if ( VPtFrList->TotFrameNb >= VPtCont->RunCont.ParFrameNbPerAcq ) {
        err_retfail ( -1, (ERR_OUT,"Abort try to process frame %d > Max = %d ", VPtFrList->TotFrameNb, VPtCont->RunCont.ParFrameNbPerAcq) );
      }
      
      VPtFrame = VPtFrList->AFramePtr[VPtFrList->TotFrameNb] = (EFRIO__TFrame*) ((UInt8*) VPtFrList->AFramePtr[VPtFrList->TotFrameNb - 1] + VFullFrameRecSz);
      
      // err_error (( ERR_OUT, "TRACE : Set pointer frame %d - pointer = %d [D]", VPtFrList->TotFrameNb, VPtFrList->AFramePtr[VPtFrList->TotFrameNb] ));
    }
    
  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtFrame->Tag        = EFRIO__FRAME_TAG;
    VPtFrame->Header.Tag          = EFRIO__FRAME_TAG_HEADER;
  #endif
  
    
    VPtFrame->Header.AcqId        = VAcqId;
    VPtFrame->Header.FrameIdInAcq = VFrameId;
    VPtFrame->Header.MapsName     = (UInt16) ASIC__MI26;
    VPtFrame->Header.MapsNb       = VPtRun->ParMi26Nb;
    
    VPtFrame->Header.AMapsHeader[0]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsHeader[1]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsHeader[2]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsHeader[3]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsHeader[4]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsHeader[5]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    ++VEChanTrigFieldCnt;
    
    
    VPtFrame->Header.AMapsFrameCnt[0]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsFrameCnt[1]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsFrameCnt[2]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsFrameCnt[3]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsFrameCnt[4]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsFrameCnt[5]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    ++VEChanTrigFieldCnt;
    
    
    VADataLengthField[0]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[1]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[2]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[3]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[4]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[5]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    ++VEChanTrigFieldCnt;
    
    
    VADataLengthW16[0] = (VADataLengthField[0] & 0x0000FFFF) + ((VADataLengthField[0] & 0xFFFF0000) >> 16);
    VADataLengthW16[1] = (VADataLengthField[1] & 0x0000FFFF) + ((VADataLengthField[1] & 0xFFFF0000) >> 16);
    VADataLengthW16[2] = (VADataLengthField[2] & 0x0000FFFF) + ((VADataLengthField[2] & 0xFFFF0000) >> 16);
    VADataLengthW16[3] = (VADataLengthField[3] & 0x0000FFFF) + ((VADataLengthField[3] & 0xFFFF0000) >> 16);
    VADataLengthW16[4] = (VADataLengthField[4] & 0x0000FFFF) + ((VADataLengthField[4] & 0xFFFF0000) >> 16);
    VADataLengthW16[5] = (VADataLengthField[5] & 0x0000FFFF) + ((VADataLengthField[5] & 0xFFFF0000) >> 16);
    
    VDataLengthW16Max = MATH_FUInt16Max ( VADataLengthW16, 6 );
    
    
    // A better test ( on each VADataLengthW16[i] ? on each W16 on D0 & D1 ? ) should be done !
    
    if ( VDataLengthW16Max > 2304 ) {
      memset ( VADataLengthW8 , 0, 12 );
      memset ( VADataLengthW16, 0, 12 );
      memset ( VADataLengthW32, 0, 12 );
      
      VDataLengthW8Max  = 0;
      VDataLengthW32Max = 0;
      
      ++VDataLengthErrCnt;
      err_error (( ERR_OUT, "HW error on data length -> Max W16 nb = %d -> Force 0 !", VDataLengthW16Max ));
    }
    
    else {
      
      for ( ViMi26=0; ViMi26 < 6; ViMi26++ ) {
        VADataLengthW8[ViMi26]  = VADataLengthW16[ViMi26] * 2;
        VADataLengthW32[ViMi26] = VADataLengthW16[ViMi26] / 2;
      }
      
      VDataLengthW8Max  = VDataLengthW16Max * 2;
      VDataLengthW32Max = VDataLengthW16Max / 2;
    }
    
    VPtFrame->Header.AMapsDataLength[0] = VADataLengthW8[0];
    VPtFrame->Header.AMapsDataLength[1] = VADataLengthW8[1];
    VPtFrame->Header.AMapsDataLength[2] = VADataLengthW8[2];
    VPtFrame->Header.AMapsDataLength[3] = VADataLengthW8[3];
    VPtFrame->Header.AMapsDataLength[4] = VADataLengthW8[4];
    VPtFrame->Header.AMapsDataLength[5] = VADataLengthW8[5];
    
    // Copy useful data part
    
    VDataLengthW32ToCpy = VDataLengthW32Max * 6;
    VDataLengthW8ToCpy  = VDataLengthW32ToCpy * 4;
    
  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtFrame->Data.Tag       = EFRIO__FRAME_TAG_DATA;
  #endif
  
    
    VPtFrame->Data.TotSz     = VDataLengthW8ToCpy;
    VPtFrame->Data.OneMapsSz = VDataLengthW8Max;
    
    // err_error (( ERR_OUT, "TRACE -> Before loop - VFrameId=%d - VDataLengthW32=%d", VFrameId, VDataLengthW32 ));
    
    
    // A remplacer par memcpy !!!!!!!!!
    
    
    // memcpy ( VPtFrame->Data.ADataW32, &PtSrcW32[ViSrcW32], VDataLengthW8ToCpy  );
    
    // err_error (( ERR_OUT, "TRACE => VDataLengthW8ToCpy=%d", VDataLengthW8ToCpy ));
    
    VPtCpySrcW64  = (UInt64*) &PtSrcW32[ViSrcW32];
    VPtCpyDestW64 = (UInt64*) VPtFrame->Data.ADataW32;
    
    
    
    for ( ViDataCpy=0; ViDataCpy < VDataLengthW32Max; ViDataCpy++ ) {
      
      *VPtCpyDestW64 = *VPtCpySrcW64;
      ++VPtCpyDestW64;
      ++VPtCpySrcW64;
      *VPtCpyDestW64 = *VPtCpySrcW64;
      ++VPtCpyDestW64;
      ++VPtCpySrcW64;
      *VPtCpyDestW64 = *VPtCpySrcW64;
      ++VPtCpyDestW64;
      ++VPtCpySrcW64;
      
      VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = *((UInt32*) VPtCpySrcW64);
      (UInt32) VPtCpySrcW64 = (UInt32) (VPtCpySrcW64) + 4;
      ++VEChanTrigFieldCnt;
    }
    
    
    
    
    VPtEChanSrcW32 = (UInt32*) VPtCpySrcW64;
    
    // Continue extraction of trigger fields from trigger channel WHILE needed
    
    VPtEChanSrcW32 += 6; // Bypass Mi26 x 6 data
    
    do {
      VEChanTrigField = *VPtEChanSrcW32;
      
      if ( VEChanTrigFieldCnt >= EFRIO__EXTRA_CHAN__MAX_TRIGGER_FIELD_NB ) {
        err_warning (( ERR_OUT, "Max trigger field nb reached = %d <=> %d max trigger info !", EFRIO__EXTRA_CHAN__MAX_TRIGGER_FIELD_NB, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ));
        break;
      }
      
      VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = VEChanTrigField;
      VPtEChanSrcW32 += 7;
      ++VEChanTrigFieldCnt;
    } while ( (VEChanTrigField & 0x80000000) == 0 );
    
    
    ViSrcW32 += (7 * MI26__ZS_FFRAME_RAW_MAX_W32);
    
    
    //    VptZsFFrameRaw[V6iFrame].Trailer = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * VADataLengthW32[0])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    //    ++ViSrcW32;
    
    //    VZero = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 6];
    //    VptZsFFrameRaw[V6iFrame].Zero = VZero;
    //    ++ViSrcW32;
    
    //    VZero2 = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 12];
    //    VptZsFFrameRaw[V6iFrame].Zero2 = VZero2;
    //    ++ViSrcW32;
    
    
    VPtFrame->Header.AMapsTrailer[0] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + (7 * VADataLengthW32[0])];    // 21 = 7 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[1] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + 1 + (7 * VADataLengthW32[1])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[2] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + 2 + (7 * VADataLengthW32[2])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[3] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + 3 + (7 * VADataLengthW32[3])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[4] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + 4 + (7 * VADataLengthW32[4])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[5] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + 5 + (7 * VADataLengthW32[5])];
    ++ViSrcW32;
    
    ++ViSrcW32; // To count extra channel
    
    
    // Read trigger number from "Mi26" frame
    
    if ( TrigStatus == 0 ) {
      VZero  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) +  7];
    }
    
    // Emulate trigger by overwriting info from "Mi26" frame
    
    else {
      VZero = (TrigStatus << 16);
    }
    
    // VZero  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) +  7];
    
    
    
    VZero2 = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 14];
    
    ViSrcW32 += 14; // 7 times 2 zero fields = 14
    
    // Extraction of trigger info from "Mimosa 26 frame" zero fields
    
    VTrigNb = (VZero  & 0xFFFF0000) >> 16;
    
    
    
    
    if ( VTrigNb > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ) {
      err_warning (( ERR_OUT, "VTrigNb=%d > Max=%d => Limit to max !", VTrigNb, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ));
      VTrigNb = EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB;
    }
    
    if ( VTrigNb != 0 ) {
      VATrigVal[0]    = (VZero  & 0x0000FFFF);
      VATrigVal[1]    = (VZero2 & 0xFFFF0000) >> 16;
      VATrigVal[2]    = (VZero2 & 0x0000FFFF);
      
      VATrigLine[0]   = VATrigVal[0] / 16;
      VATrigLine[1]   = VATrigVal[1] / 16;
      VATrigLine[2]   = VATrigVal[2] / 16;
      
      VATrigClk[0]    = VATrigVal[0] % 16;
      VATrigClk[1]    = VATrigVal[1] % 16;
      VATrigClk[2]    = VATrigVal[2] % 16;
      
      VAMi26Trig[0]   = VATrigLine[0] /* + ( VATrigClk[0] << 10 ) */;
      VAMi26Trig[1]   = VATrigLine[1] /* + ( VATrigClk[1] << 10 ) */;
      VAMi26Trig[2]   = VATrigLine[2] /* + ( VATrigClk[2] << 10 ) */;
    }
    
    else {
      VAMi26Trig[0] = VAMi26Trig[1] = VAMi26Trig[2] = 0;
    }
    
    // Add trigger info in frame header
    
    VPtFrame->Header.TriggerNb        = VTrigNb;
    VPtFrame->Header.AMapsTrigInfo[0] = VAMi26Trig[0];
    VPtFrame->Header.AMapsTrigInfo[1] = VAMi26Trig[1];
    VPtFrame->Header.AMapsTrigInfo[2] = VAMi26Trig[2];
    
    // Add trigger info in trigger record
    
    VPtTrigRec = (EFRIO__TTriggerRec*) (VPtFrame->Data.ADataW32 + VDataLengthW32ToCpy);
    
  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtTmpTrigRec->Tag      = EFRIO__FRAME_TAG_TRIG;
  #endif
  
    
    VPtTmpTrigRec->TrigNb   = VTrigNb;
    VPtTmpTrigRec->TotSz    = VEmptyTrigRecSz + (VTrigNb * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ);
    VPtTmpTrigRec->TrigType = 2;
    
    memcpy ( VPtTrigRec, VPtTmpTrigRec, VPtTmpTrigRec->TotSz );
    
    // Calculate size of frame record while filled ( fixed part + variable length (data & trigger) )
    
    // Calculate size will be 2 x W32 higher than needed, because following fields will be counted twice
    //
    // - Field UInt32 ADataW32[0] of data record -> EFRIO__TFrameData
    // - Field UInt32 ATrig[0] of trigger record -> EFRIO__TTriggerRec
    
    VFullFrameRecSz = VEmptyFrameRecSz + VDataLengthW8ToCpy + VPtTrigRec->TotSz;

    VTotAcqSz += VFullFrameRecSz;
    
    // Update frame field total size & trigger offset size
    
    VPtFrame->TotSz         = VFullFrameRecSz;
    VPtFrame->TrigRecOffset = VEmptyFrameRecSz + VDataLengthW8ToCpy;
    
    if ( 0 /* VPtServ->PtParAcqReqFunc->AsicChkFunc >= 0 */ ) {
      
      // No neeed to reset ResAsicErrorsRejCurAcq ( it's done on DPXI__MI26_FAsicChkFunc0Emul6Mi26 with ViFrame = 0 )
      // => ResAsicErrorsRejCurAcq = 1 if errors in current Acq
      
      // $$$ PtAcq->ResAsicErrorsRejCurAcq = DPXI__MI26_FAsicChkFuncMi26 ( VPtServ->PtParAcqReqFunc->AsicChkFunc, ViFrame, VptZsFFrameRaw, 1 /* Mi26Nb */ );
      // $$$ PtAcq->ResAsicErrorsRejCurAcq = 0;
      
      // $$$ if ( PtAcq->ResAsicErrorsRejCurAcq != 0 ) {
        // $$$   msg (( MSG_OUT, "ViFrame=%d - VAsicErrorsRejCurAcq=%d", ViFrame, PtAcq->ResAsicErrorsRejCurAcq ));
        // $$$ }
    }
    
    
    ++VPtFrList->TotFrameNb;
    
    ++VRunFrameCnt;
    
  } // End for ViFrameWithTrig
  
  ++VPtCont->RunCont.ResAcqCnt;
  
  PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
  
  return (VTotAcqSz);
}





/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      : Set board conf fields
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
Date      : 11/08/2010
Rev       : 25/10/2010
          : - EUDET data formatting mode + trigger handling implementation
Doc date  : 11/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

// Use data source pointer as pointer  => Set PtSrcW32AsInt to 0
// Use data source pointer as integer => Set pointer value in PtSrcW32AsInt, don't care about PtSrcW32AsPt

// DataConvertMode
// 0 - IPHC  mode   = Demultiplex the data part, doesn't handle extra channel -> for compatibility with IPHC DAQ sw
// 1 - EUDET mode 1 = Don't demultiplex data part, don't care about extra channel, send all frames
// 2 - EUDET mode 2 = Don't demultiplex data part, extract trigger info from extra channel, send all frames
// 3 - EUDET mode 3 = Don't demultiplex data part, extract trigger info from extra channel, send only frames with trigger ( use TriggerHandlingMode parameter )

// 0 - EFRIO__TRF_MODE_IPHC
// 1 - EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN,
// 2 - EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES,
// 3 - EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG
  

SInt32 EFRIO__MI26_FFRioAcqDeserDataMi26 ( SInt8 Mi26Nb, SInt32 BoardId, UInt32* PtSrcW32AsPt, UInt32 PtSrcW32AsInt, SInt32 EltNb, SInt8 AcqStatus, SInt16 TrigStatus, UInt32 WaitMsAtEnd, SInt8 DataConvertMode, SInt8 TriggerHandlingMode, SInt16 EmuleMode ) {
  
  EFRIO__TContext*     VPtCont  = &EFRIO__VGContext;
  
  SInt32 VRet = 0;
  SInt32 VEmuleFrameNb;
  SInt32 VEmuleFirstFrameNo;
  
  SInt32 VDbgOffset;
  
  if ( PtSrcW32AsInt != 0 ) {
    PtSrcW32AsPt = (UInt32*) PtSrcW32AsInt;
  }

/*

  msg (( MSG_OUT, "-------------------------------------" ));
  msg (( MSG_OUT, "Data dump" ));
  msg (( MSG_OUT, "-------------------------------------" ));
  
  msg (( MSG_OUT, "U32  0 = %4x", PtSrcW32AsPt[0] ));
  msg (( MSG_OUT, "U32  1 = %4x", PtSrcW32AsPt[1] ));
  msg (( MSG_OUT, "U32  2 = %4x", PtSrcW32AsPt[2] ));
  msg (( MSG_OUT, "U32  3 = %4x", PtSrcW32AsPt[3] ));
  msg (( MSG_OUT, "U32  4 = %4x", PtSrcW32AsPt[4] ));
  msg (( MSG_OUT, "U32  5 = %4x", PtSrcW32AsPt[5] ));

  msg (( MSG_OUT, "U32  6 = %4x", PtSrcW32AsPt[6] ));
  msg (( MSG_OUT, "U32  7 = %4x", PtSrcW32AsPt[7] ));
  msg (( MSG_OUT, "U32  8 = %4x", PtSrcW32AsPt[8] ));
  msg (( MSG_OUT, "U32  9 = %4x", PtSrcW32AsPt[9] ));
  msg (( MSG_OUT, "U32 10 = %4x", PtSrcW32AsPt[10] ));
  msg (( MSG_OUT, "U32 11 = %4x", PtSrcW32AsPt[11] ));
  
*/
        
  VEmuleFrameNb      = VPtCont->RunCont.ParFrameNbPerAcq; 
  VEmuleFirstFrameNo = 0;

  // Emule frames if needed
  
  if ( EmuleMode != 0 ) {
        
    while (1) {

      if ( (DataConvertMode == EFRIO__TRF_MODE_IPHC) || (DataConvertMode == EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN)) {
        
        switch ( Mi26Nb ) {
          
          case 1 : {
            EFRIO__MI26_FFRioEmulDeserData1Mi26NoEChan ( PtSrcW32AsPt, EltNb, &VEmuleFirstFrameNo, VEmuleFrameNb  );
            break; }
          
          case 6 : {
            EFRIO__MI26_FFRioEmulDeserData6Mi26NoEChan ( PtSrcW32AsPt, EltNb, &VEmuleFirstFrameNo, VEmuleFrameNb  );
            break; }
          
          default : {
            err_warning (( ERR_OUT, "This number of M26 = %d is not handled for frame emulation WITHOUT extra channel !", Mi26Nb ));
            break; }
          
        } // End switch
        
        break;
      } // End if ( EFRIO__TRF_MODE_IPHC )
      

      if ( DataConvertMode == EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES ) {
        
        switch ( Mi26Nb ) {
          
          case 1 : {
            EFRIO__MI26_FFRioEmulDeserData1Mi26EudetMode2 ( PtSrcW32AsPt, EltNb, &VEmuleFirstFrameNo, VEmuleFrameNb, EmuleMode  );
            break; }
          
          case 6 : {
            EFRIO__MI26_FFRioEmulDeserData6Mi26EudetMode2 ( PtSrcW32AsPt, EltNb, &VEmuleFirstFrameNo, VEmuleFrameNb, EmuleMode  );
            break; }
          
          default : {
            err_warning (( ERR_OUT, "This number of M26 = %d is not handled for frame emulation WITH extra channel !", Mi26Nb ));
            break; }
          
        } // End switch
        
        break;
      } // End if ( EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES )


      if ( DataConvertMode == EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG ) {
        
        switch ( Mi26Nb ) {
          
          case 1 : {
            EFRIO__MI26_FFRioEmulDeserData1Mi26EudetMode3 ( PtSrcW32AsPt, EltNb, &VEmuleFirstFrameNo, VEmuleFrameNb, EmuleMode  );
            break; }
          
          case 6 : {
            EFRIO__MI26_FFRioEmulDeserData6Mi26EudetMode3 ( PtSrcW32AsPt, EltNb, &VEmuleFirstFrameNo, VEmuleFrameNb, EmuleMode  );
            break; }
          
          default : {
            err_warning (( ERR_OUT, "This number of M26 = %d is not handled for frame emulation WITH extra channel !", Mi26Nb ));
            break; }
          
        } // End switch
        
        break;
      } // End if ( EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG )
      
  
    } // End while        
    
  } // End if ( EmuleMode == 1 )


  while (1) {
    
    // IPHC mode
    
    if ( DataConvertMode == EFRIO__TRF_MODE_IPHC ) {
      
      switch ( Mi26Nb ) {
        
        case 1 : {
          VRet = EFRIO__MI26_FFRioAcqDeserDataIphcMode1Mi26 ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode );
          break; }
        
        case 6 : {
          VRet = EFRIO__MI26_FFRioAcqDeserDataIphcMode6Mi26 ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode );
          break; }
        
        default : {
          err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_IPHC -> This number of Mi26 = %d is not handled now", Mi26Nb ) );
          break; }
      }
      
      break;
    }
  
    // EUDET mode 1
    
    if ( DataConvertMode == EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN ) {
      
      switch ( Mi26Nb ) {
        
        case 1 : {
          VRet = EFRIO__MI26_FFRioAcqDeserDataEudet1Mode1Mi26 ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode );
          break; }
        
        case 6 : {
          VRet = EFRIO__MI26_FFRioAcqDeserDataEudet1Mode6Mi26 ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode );
          break; }
        
        default : {
          err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN -> This number of Mi26 = %d is not handled now", Mi26Nb ) );
          break; }
        
      }
      
      break;
    }

    // EUDET mode 2
    
    if ( DataConvertMode == EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES ) {

      switch ( Mi26Nb ) {
        
        case 1 : {
          VRet = EFRIO__MI26_FFRioAcqDeserDataEudet2Mode1Mi26 ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode );
          break; }
        
        case 6 : {
          VRet = EFRIO__MI26_FFRioAcqDeserDataEudet2Mode6Mi26 ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode );
          break; }
        
        default : {
          err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES -> This number of Mi26 = %d is not handled now", Mi26Nb ) );
          break; }
        
      }

      break;
    }

    // EUDET mode 3
    
    if ( DataConvertMode == EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG ) {
      
      switch ( Mi26Nb ) {
        
        case 1 : {
          VRet = EFRIO__MI26_FFRioAcqDeserDataEudet3Mode1Mi26 ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode );
          break; }
        
        case 6 : {
          VRet = EFRIO__MI26_FFRioAcqDeserDataEudet3Mode6Mi26 ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode );
          break; }
        
        default : {
          err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG -> This number of Mi26 = %d is not handled now", Mi26Nb ) );
          break; }
        
      }
      
      break;
    }
    
  } // End while (1)
    
  
  if ( WaitMsAtEnd != 0 ) {
    Sleep ( WaitMsAtEnd );
  }
  
  return (VRet);
}
  

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      : Set board conf fields
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
Date      : 09/08/2010
Doc date  : 09/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FSetBoardConf ( SInt32 BoardId, EFRIO__TBoardConf* PtSrc ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  err_retnull ( PtSrc, (ERR_OUT,"Abort : PtSrc == NULL") );
  
  EFRIO__VGContext.ABoardsConf[BoardId] = *PtSrc;
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      : Set board conf fields
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
Date      : 10/08/2010
Doc date  : 10/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

// 10/08/2010

SInt32 EFRIO__FSetBoardConfBoardId ( SInt32 BoardId ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].BoardId = BoardId;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfAsicName ( SInt32 BoardId, char* AsicName ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
    
  snprintf ( EFRIO__VGContext.ABoardsConf[BoardId].AsicName, GLB_CMT_SZ - 1, "%s", AsicName );
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfAsicNb ( SInt32 BoardId, SInt32 AsicNb ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].AsicNb = AsicNb;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfReadoutMode ( SInt32 BoardId, SInt32 ReadoutMode ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].ReadoutMode = ReadoutMode;
  
  err_retok (( ERR_OUT, "" ));
}

// 11/08/2010

SInt32 EFRIO__FSetBoardConfEmuleChannels ( SInt32 BoardId, SInt8 EmuleChannels ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].EmuleChannels = EmuleChannels;
  
  err_retok (( ERR_OUT, "" ));
}


// 10/08/2010

SInt32 EFRIO__FSetBoardConfDataClkFrequency ( SInt32 BoardId, float DataClkFrequency ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].DataClkFrequency = DataClkFrequency;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfDmaHostSz ( SInt32 BoardId, UInt32 DmaHostSz ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].DmaHostSz = DmaHostSz;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfFrameNbPerAcq ( SInt32 BoardId, SInt32 FrameNbPerAcq ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].FrameNbPerAcq = FrameNbPerAcq;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfEnableExtraChannel ( SInt32 BoardId, SInt8 EnableExtraChannel ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].EnableExtraChannel = EnableExtraChannel;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfAcqNbPerTrig ( SInt32 BoardId, SInt32 AcqNbPerTrig ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].AcqNbPerTrig = AcqNbPerTrig;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfTriggerMode ( SInt32 BoardId, SInt8 TriggerMode ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].TriggerMode = TriggerMode;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfTriggerDetectTimeWindow ( SInt32 BoardId, UInt32 TriggerDetectTimeWindow ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].TriggerDetectTimeWindow = TriggerDetectTimeWindow;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfTriggerDetectOccurNb ( SInt32 BoardId, UInt32 TriggerDetectOccurNb ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].TriggerDetectOccurNb = TriggerDetectOccurNb;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfTimeStampRes ( SInt32 BoardId, UInt32 TimeStampRes ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].TimeStampRes = TimeStampRes;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfEnableTimeStamping ( SInt32 BoardId, SInt8 EnableTimeStamping ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].EnableTimeStamping = EnableTimeStamping;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfEnableTrigCnt ( SInt32 BoardId, SInt8 EnableTrigCnt ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].EnableTrigCnt = EnableTrigCnt;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfTagEventsStoredByDUT ( SInt32 BoardId, SInt8 TagEventsStoredByDUT ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].TagEventsStoredByDUT = TagEventsStoredByDUT;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardConfReadTluTrigCntEachNTrig ( SInt32 BoardId, UInt32 ReadTluTrigCntEachNTrig ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsConf[BoardId].ReadTluTrigCntEachNTrig = ReadTluTrigCntEachNTrig;
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      : Get board conf fields
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
Date      : 09/08/2010
Doc date  : 09/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FGetBoardConf ( SInt32 BoardId, EFRIO__TBoardConf* PtDest ) {
  
  err_trace (( ERR_OUT, "begin" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  err_retnull ( PtDest, (ERR_OUT,"Abort : PtDest == NULL") );
  
  *PtDest = EFRIO__VGContext.ABoardsConf[BoardId];
  
  err_retok (( ERR_OUT, "end" ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      : Get board conf fields
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
Date      : 10/08/2010
Doc date  : 10/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

// 10/08/2010

SInt32 EFRIO__FGetBoardConfBoardId ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].BoardId);
}

// 10/08/2010

char* EFRIO__FGetBoardConfAsicName ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  if ( (BoardId < 0) || (BoardId > EFRIO__MAX_BOARDS_NB) ) {
    return (NULL);
  }
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].AsicName);
}

// 10/08/2010

SInt32 EFRIO__FGetBoardConfAsicNb ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].AsicNb);
}

// 10/08/2010

SInt32 EFRIO__FGetBoardConfReadoutMode ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].ReadoutMode);
}

// 11/08/2010

SInt8 EFRIO__FGetBoardConfEmuleChannels ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].EmuleChannels);
}


// 10/08/2010

float EFRIO__FGetBoardConfDataClkFrequency ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].DataClkFrequency);
}

// 10/08/2010

UInt32 EFRIO__FGetBoardConfDmaHostSz ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].DmaHostSz);
}

// 10/08/2010

SInt32 EFRIO__FGetBoardConfFrameNbPerAcq ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].FrameNbPerAcq);
}

// 10/08/2010

SInt8 EFRIO__FGetBoardConfEnableExtraChannel ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].EnableExtraChannel);
}

// 10/08/2010

SInt32 EFRIO__FGetBoardConfAcqNbPerTrig ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].AcqNbPerTrig);
}

// 10/08/2010

SInt8 EFRIO__FGetBoardConfTriggerMode ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].TriggerMode);
}

// 10/08/2010

UInt32 EFRIO__FGetBoardConfTriggerDetectTimeWindow ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].TriggerDetectTimeWindow);
}

// 10/08/2010

UInt32 EFRIO__FGetBoardConfTriggerDetectOccurNb ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].TriggerDetectOccurNb);
}

// 10/08/2010

UInt32 EFRIO__FGetBoardConfTimeStampRes ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].TimeStampRes);
}

// 10/08/2010

SInt8 EFRIO__FGetBoardConfEnableTimeStamping ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].EnableTimeStamping);
}

// 10/08/2010

SInt8 EFRIO__FGetBoardConfEnableTrigCnt ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].EnableTrigCnt);
}

// 10/08/2010

SInt8 EFRIO__FGetBoardConfTagEventsStoredByDUT ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].TagEventsStoredByDUT);
}

// 10/08/2010

UInt32 EFRIO__FGetBoardConfReadTluTrigCntEachNTrig ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsConf[BoardId].ReadTluTrigCntEachNTrig);
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      : Set board status fields
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
Date      : 10/08/2010
Doc date  : 10/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

// 10/08/2010

SInt32 EFRIO__FSetBoardStatusBoardId ( SInt32 BoardId ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsStatus[BoardId].BoardId = BoardId;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardStatusBoardPresent ( SInt32 BoardId, SInt8 BoardPresent ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsStatus[BoardId].BoardPresent = BoardPresent;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardStatusFwLoaded ( SInt32 BoardId, SInt8 FwLoaded ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsStatus[BoardId].FwLoaded = FwLoaded;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardStatusConfDone ( SInt32 BoardId, SInt8 ConfDone ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsStatus[BoardId].ConfDone = ConfDone;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardStatusStatusCode ( SInt32 BoardId, SInt32 StatusCode ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  EFRIO__VGContext.ABoardsStatus[BoardId].StatusCode = StatusCode;
  
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardStatusStatusStr ( SInt32 BoardId, char* StatusStr ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  err_retnull ( StatusStr, (ERR_OUT,"Abort : StatusStr == NULL") );
  
  snprintf ( EFRIO__VGContext.ABoardsStatus[BoardId].StatusStr, GLB_CMT_SZ - 1, "%s", StatusStr );
    
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardStatusErrorMsgList ( SInt32 BoardId, char* ErrorMsg, SInt32 Index ) {
  
  SInt32 VMsgLen;
  EFRIO__TBoardStatus* VPtStatus;
  
  EFRIO__CHK_BOARD_ID (BoardId);

  err_retnull ( ErrorMsg, (ERR_OUT,"Abort : ErrorMsg == NULL") );
  
  VPtStatus = &EFRIO__VGContext.ABoardsStatus[BoardId];
  
  // Check index limits
  
  if ( (Index < 0) || (Index >= EFRIO__ERROR_MSG_LIST_MAX_NB ) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Index=%d out of range [0..%d]", Index, EFRIO__ERROR_MSG_LIST_MAX_NB-1 ) );
  }
  
  // Get message length
  
  VMsgLen = strlen ( ErrorMsg );
  
  // Free item if already allocated
  
  if ( VPtStatus->ErrorMsgList[Index] != NULL ) {
    free ( VPtStatus->ErrorMsgList[Index] );
  }
  
  // Allocate memory for message
  
  VPtStatus->ErrorMsgList[Index] = (char*) malloc ( VMsgLen + 1 );
  
  err_retnull ( VPtStatus->ErrorMsgList[Index], (ERR_OUT,"Abort : Allocation of message [%d] failed !", Index) );
  
  // Copy message

  snprintf ( VPtStatus->ErrorMsgList[Index], VMsgLen, "%s", ErrorMsg );
    
  err_retok (( ERR_OUT, "" ));
}

// 10/08/2010

SInt32 EFRIO__FSetBoardStatusLastErrorMsg ( SInt32 BoardId, char* LastErrorMsg ) {
  
  EFRIO__CHK_BOARD_ID (BoardId);

  err_retnull ( LastErrorMsg, (ERR_OUT,"Abort : LastErrorMsg == NULL") );
  
  snprintf ( EFRIO__VGContext.ABoardsStatus[BoardId].LastErrorMsg, GLB_CMT_SZ - 1, "%s", LastErrorMsg );  
  
  err_retok (( ERR_OUT, "" ));
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      : Get board status fields
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
Date      : 10/08/2010
Doc date  : 10/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

// 10/08/2010

SInt32 EFRIO__FGetBoardStatusBoardId ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsStatus[BoardId].BoardId);
}

// 10/08/2010

SInt8  EFRIO__FGetBoardStatusBoardPresent ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsStatus[BoardId].BoardPresent);
}

// 10/08/2010

SInt8 EFRIO__FGetBoardStatusFwLoaded ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsStatus[BoardId].FwLoaded);
}

// 10/08/2010

SInt8 EFRIO__FGetBoardStatusConfDone ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsStatus[BoardId].ConfDone);
}

// 10/08/2010

SInt32 EFRIO__FGetBoardStatusStatusCode ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
  
  EFRIO__CHK_BOARD_ID (BoardId);
  
  return (EFRIO__VGContext.ABoardsStatus[BoardId].StatusCode);
}

// 10/08/2010

char* EFRIO__FGetBoardStatusStatusStr ( SInt32 BoardId ) {
  
  err_trace (( ERR_OUT, "" ));
    
  if ( (BoardId < 0) || (BoardId > EFRIO__MAX_BOARDS_NB) ) {
    err_error (( ERR_OUT, "Abort : BoardId=%d out of range [0..%d]", BoardId, EFRIO__MAX_BOARDS_NB-1 ));
    return (NULL);
  }
  
  return (EFRIO__VGContext.ABoardsStatus[BoardId].StatusStr);
}


// 10/08/2010

char* EFRIO__FGetBoardStatusErrorMsgList ( SInt32 BoardId, SInt32 Index ) {
  
  SInt32 VMsgLen;
  EFRIO__TBoardStatus* VPtStatus;
  
  if ( (BoardId < 0) || (BoardId > EFRIO__MAX_BOARDS_NB) ) {
    err_error (( ERR_OUT, "Abort : BoardId=%d out of range [0..%d]", BoardId, EFRIO__MAX_BOARDS_NB-1 ));
    return (NULL);
  }
  
  VPtStatus = &EFRIO__VGContext.ABoardsStatus[BoardId];
  
  // Check index limits
  
  if ( (Index < 0) || (Index >= EFRIO__ERROR_MSG_LIST_MAX_NB ) ) {
    err_error (( ERR_OUT, "Abort : Index=%d out of range [0..%d]", Index, EFRIO__ERROR_MSG_LIST_MAX_NB-1 ));
    return (NULL);
  }
  
  return ( VPtStatus->ErrorMsgList[Index] );  
}

// 10/08/2010

char* EFRIO__FGetBoardStatusLastErrorMsg ( SInt32 BoardId ) {
  
  if ( (BoardId < 0) || (BoardId > EFRIO__MAX_BOARDS_NB) ) {
    err_error (( ERR_OUT, "Abort : BoardId=%d out of range [0..%d]", BoardId, EFRIO__MAX_BOARDS_NB-1 ));
    return (NULL);
  }
  
  return (EFRIO__VGContext.ABoardsStatus[BoardId].LastErrorMsg);
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    : DOIT ETRE A LA FIN DU FICHIER !
          : Suite a ouverture avec C++B, pose probleme a code source manager sinon !!!
:
Level     : This is a user level function.
Date      : 03/11/2010
Rev       :
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet3Mode1Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt8 AcqStatus, SInt16 TrigStatus, SInt8 DataConvertMode ) {
  
  EFRIO__TContext*     VPtCont   = &EFRIO__VGContext;
  EFRIO__TBoardConf*   VPtBoard  = NULL;
  EFRIO__TRunCont*     VPtRun    = &VPtCont->RunCont;
  EFRIO__TFrameList*   VPtFrList = &VPtCont->AAcqFrameList[0];
  EFRIO__TFrame*       VPtFrame;
  EFRIO__TTriggerRec*  VPtTmpTrigRec = VPtCont->PtTmpTrigRec;
  
  SInt32               VAcqId;
  UInt8*               VPtAcqData;
  SInt32               VFrameNbFromBoardDrv;
  SInt32               VFrameId;
  UInt32               VDataLengthField;
  UInt32               VDataLengthW8;
  UInt32               VDataLengthW16;
  UInt32               VDataLengthW32;
  SInt32               ViSrcW32;
  SInt32               ViSrcW32BeforeDataCpyLoop;
  SInt32               ViDataW32;
  SInt32               VRunFrameCnt;
  SInt32               VLastFrameWithTrigAllowed;
  UInt32               VZero;
  UInt32               VZero2;
  SInt16               VTrigNb;
  UInt16               VATrigVal[3];
  UInt16               VATrigLine[3];
  UInt16               VATrigClk[3];
  UInt16               VAMi26Trig[3];
  EFRIO__TTriggerRec*  VPtTrigRec;
  UInt32               VFullFrameRecSz;
  UInt32               VEmptyFrameRecSz;
  UInt32               VEmptyTrigRecSz;
  SInt32               VDataLengthErrCnt;
  
  UInt16               VDbgDataLenghtD0;
  UInt16               VDbgDataLenghtD1;
  
  SInt16               VEChanTrigFieldCnt; // Extra channel trigger fields counter
  UInt32               VEChanTrigField;
  
  SInt32               VFrameWithTrigCnt;
  SInt32               VLastFrameWithTrig;
  SInt16*              VPtFrameWithTrigList;
  
  SInt16               ViFrameWithTrig;
  static SInt32        VFrNbToTakeAtBeginOfAcqForLastTrig = -1; // Number of frames to take at beginning of Acq to complete last trigger of previous Acq
  SInt32               VTotAcqSz;
  
  
  
  
  PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
  
  if ( VPtCont->InfInitDone == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort because : Init not done") );
  }
  
  // err_trace (( ERR_OUT, "EFRIO__MI26_FFRioAcqDeserDataEudet1Mode1Mi26 (P=%x, EltNb=%d)", PtSrcW32, EltNb ));
  
  
  // Pointers parameters check
  
  err_retnull ( PtSrcW32, (ERR_OUT,"PtSrcW32 = NULL") );
  
  // Init board conf record pointer
  
  if ( (BoardId < 0) || (BoardId >= EFRIO__MAX_BOARDS_NB) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Board Id=%d out of range [0..%d])", BoardId, EFRIO__MAX_BOARDS_NB - 1) );
  }
  
  VPtBoard = &EFRIO__VGContext.ABoardsConf[BoardId];
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ) / 2; // Divide by 2 because of extra channel
  
  // It must be => ( (EltNb * 2) / (MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ * 2) );
  // But result is the SAME if we divide both part of fraction by 2 ;-)
  
  if ( VFrameNbFromBoardDrv != VPtBoard->FrameNbPerAcq ) {
    err_retfail ( -1, (ERR_OUT,"Bad number of samples : %d frames acquired <> %d frames requested", VFrameNbFromBoardDrv, VPtBoard->FrameNbPerAcq ) );
  }

  // Reset total size of acquisition
  
  VTotAcqSz = 0;
  
  // Get acq id
  
  VAcqId = VPtCont->RunCont.ResAcqCnt;
  
  // Get last frame with trigger allowed / acq
  
  VLastFrameWithTrigAllowed = VPtBoard->FrameNbPerAcq - 1; // Last n frames of Acq MUST have NO trigger information - NOW n = 0 !!!
  
  // Check if buffer is allocated ( this is total buffer, not only the part where we will write )
  
  err_retnull ( VPtRun->PtFrame, (ERR_OUT,"Abort : EUDET frames buffer not allocated !") );
  
  // Reset frame list pointer

  VPtFrList->TotFrameNb = 0;
  
  memset ( VPtFrList->AFramePtr, 0, EFRIO__MAX_FRAME_NB_PER_ACQ * sizeof (EFRIO__TFrame*) );
            
  // Calculate size of records WITHOUT variable length part counted here as one elt
  
  VEmptyTrigRecSz  = sizeof ( EFRIO__TTriggerRec );
  VEmptyFrameRecSz = sizeof ( EFRIO__TFrame );
  
  //  err_error (( ERR_OUT, "TRACE => VEmptyTrigRecSz  = %d", VEmptyTrigRecSz  ));
  //  err_error (( ERR_OUT, "TRACE => VEmptyFrameRecSz = %d", VEmptyFrameRecSz ));
  
  // Reset tmp triggers buffer
  
  memset ( VPtTmpTrigRec, 0, sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );
  
  
  PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
  
  // ----------------------------------------------------------------------------------
  // List frames to extract => frame with trigger + 3 following frames
  // ----------------------------------------------------------------------------------
  
  memset ( VPtCont->AAAcqFrameWithTrigList[0], 0xFF, EFRIO__MAX_FRAME_NB_PER_ACQ * 2 );
  
  // Init loop parameters
  
  VFrameWithTrigCnt    =  0;
  VLastFrameWithTrig   = -1;
  VPtFrameWithTrigList = VPtCont->AAAcqFrameWithTrigList[0];
    
  for ( VFrameId=0; VFrameId < VPtBoard->FrameNbPerAcq; VFrameId++ ) {
    
    if ( TrigStatus == 0 ) {
      VZero = PtSrcW32[(2 * MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * VFrameId) + (2 * (3 + MI26__ZS_FFRAME_RAW_MAX_W32 + 1))];
    }
    
    // Emulate trigger by overwriting info from "Mi26" frame
    // BUT first read it from board RAM in order to get real execution time
    
    else {
      VZero = PtSrcW32[(2 * MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * VFrameId) + (2 * (3 + MI26__ZS_FFRAME_RAW_MAX_W32 + 1))];
      VZero = (TrigStatus << 16);
    }
    
    VTrigNb = (VZero  & 0xFFFF0000) >> 16;
    
    if ( VTrigNb != 0 ) {
      VLastFrameWithTrig = VFrameId;
      VPtFrameWithTrigList[VFrameWithTrigCnt] = VFrameId;
      ++VFrameWithTrigCnt;
    }
    
    else {
      
      if ( (VFrameId < VFrNbToTakeAtBeginOfAcqForLastTrig) || ((VLastFrameWithTrig != -1) && ((VFrameId - VLastFrameWithTrig) <= EFRIO__FRAME_NB_TO_READ_AFTER_TRIG)) ) {
        VPtFrameWithTrigList[VFrameWithTrigCnt] = VFrameId;
        ++VFrameWithTrigCnt;
      }
      
    } // End else
    
  } // End for ( ViFrame )
  
  // Calculate nb of frame to store on beginning of next acq to complete last trigger of current one
  // If last trigger occurs before the last EFRIO__FRAME_NB_TO_READ_AFTER_TRIG frames, the result will be 0 or < 0
  // in this case it will be ignored by processing loop
  
  VFrNbToTakeAtBeginOfAcqForLastTrig = EFRIO__FRAME_NB_TO_READ_AFTER_TRIG - (VPtBoard->FrameNbPerAcq - 1 - VLastFrameWithTrig);
  
  PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
  
  
  // Print list of frames to extract
  
  /*
  msg (( MSG_OUT, "=========================================================" ));
  
  for ( ViFrameWithTrig=0; ViFrameWithTrig < VFrameWithTrigCnt; ViFrameWithTrig++ ) {
    msg (( MSG_OUT, "Readout => Trig on frame %.4d", VPtCont->AAAcqFrameWithTrigList[0][ViFrameWithTrig] ));
  }
  */
  
  // Extract data
  
  VDataLengthErrCnt  = 0;
      
  for ( ViFrameWithTrig=0; ViFrameWithTrig < VFrameWithTrigCnt; ViFrameWithTrig++ ) {
    
    VFrameId           = VPtFrameWithTrigList[ViFrameWithTrig];
    VEChanTrigFieldCnt = 0;
    ViSrcW32           = VFrameId * 2 * MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32;
    
    // Update pointer to next frame
  
    if ( VPtFrList->TotFrameNb == 0 ) {
      VPtFrame = VPtFrList->AFramePtr[0] = VPtRun->PtFrame;
    }
    
    else {
      
      if ( VPtFrList->TotFrameNb >= VPtCont->RunCont.ParFrameNbPerAcq ) {
        err_retfail ( -1, (ERR_OUT,"Abort try to process frame %d > Max = %d ", VPtFrList->TotFrameNb, VPtCont->RunCont.ParFrameNbPerAcq) );
      }
      
      VPtFrame = VPtFrList->AFramePtr[VPtFrList->TotFrameNb] = (EFRIO__TFrame*) ((UInt8*) VPtFrList->AFramePtr[VPtFrList->TotFrameNb - 1] + VFullFrameRecSz);
      
      // err_error (( ERR_OUT, "TRACE : Set pointer frame %d - pointer = %d [D]", VPtFrList->TotFrameNb, VPtFrList->AFramePtr[VPtFrList->TotFrameNb] ));
    }
    
  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtFrame->Tag        = EFRIO__FRAME_TAG;
    VPtFrame->Header.Tag                = EFRIO__FRAME_TAG_HEADER;
  #endif
  
    
    VPtFrame->Header.AcqId              = VAcqId;
    VPtFrame->Header.FrameIdInAcq       = VFrameId;
    VPtFrame->Header.MapsName           = (UInt16) ASIC__MI26;
    VPtFrame->Header.MapsNb             = VPtRun->ParMi26Nb;
    
    VPtFrame->Header.AMapsHeader[0]          = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    ++VEChanTrigFieldCnt;
    
    VPtFrame->Header.AMapsFrameCnt[0]        = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    ++VEChanTrigFieldCnt;
    
    
    VDataLengthField                         = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    ++VEChanTrigFieldCnt;
    
    VDataLengthW16 = (VDataLengthField & 0x0000FFFF) + ((VDataLengthField & 0xFFFF0000) >> 16);
    
    VDbgDataLenghtD0 = (VDataLengthField & 0x0000FFFF);
    VDbgDataLenghtD1 = ((VDataLengthField & 0xFFFF0000) >> 16);
    
    if ( (VDbgDataLenghtD0 > 570) || (VDbgDataLenghtD1 > 570) ) {
      VDataLengthW16 = 0;
      ++VDataLengthErrCnt;
      err_error (( ERR_OUT, "HW error on data length - VFrameId=%d -> D0=%4d [D] - D1=%4d [D] -> Force 0 !", VFrameId, VDbgDataLenghtD0, VDbgDataLenghtD1 ));
      err_error (( ERR_OUT, "HW error on data length - VFrameId=%d -> D0=%4x [H] - D1=%4x [H] -> Force 0 !", VFrameId, VDbgDataLenghtD0, VDbgDataLenghtD1 ));
    }
    
    VDataLengthW8  = VDataLengthW16 * 2;
    VDataLengthW32 = VDataLengthW16 / 2;
    
    
    VPtFrame->Header.AMapsDataLength[0] = VDataLengthW16 * 2;
    
    // Copy only the useful data
    
  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtFrame->Data.Tag       = EFRIO__FRAME_TAG_DATA;
  #endif
  
    VPtFrame->Data.TotSz     = VDataLengthW8;
    VPtFrame->Data.OneMapsSz = VDataLengthW8;
    
    // err_error (( ERR_OUT, "TRACE -> Before loop - ViFrame=%d - VDataLengthW32=%d", ViFrame, VDataLengthW32 ));
    
    ViSrcW32BeforeDataCpyLoop = ViSrcW32;
    
    for ( ViDataW32=0; ViDataW32 < VDataLengthW32; ViDataW32++ ) {
      VPtFrame->Data.ADataW32[ViDataW32] = PtSrcW32[ViSrcW32];
      ++ViSrcW32;
      VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = PtSrcW32[ViSrcW32];
      ++ViSrcW32;
      ++VEChanTrigFieldCnt;
    }
    
    // Continue extraction of trigger fields from trigger channel WHILE needed
    
    // WARNING => Add test to avoid to read after end of current frame in case no last trigger info is found !!!
    
    ++ViSrcW32; // To bypass current W32 with is Mi26 data NOT trigger channel field
    
    do {
      
      VEChanTrigField = PtSrcW32[ViSrcW32];
      
      if ( VEChanTrigFieldCnt >= EFRIO__EXTRA_CHAN__MAX_TRIGGER_FIELD_NB ) {
        err_warning (( ERR_OUT, "Max trigger field nb reached = %d <=> %d max trigger info !", EFRIO__EXTRA_CHAN__MAX_TRIGGER_FIELD_NB, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ));
        break;
      }
      
      VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = VEChanTrigField;
      ViSrcW32 += 2;
      ++VEChanTrigFieldCnt;
    } while ( (VEChanTrigField & 0x80000000) == 0 );
    
    // Update ViSrcW32 for following processing
    
    // ViSrcW32 = ViSrcW32 + ( MI26__ZS_FFRAME_RAW_MAX_W32 - VDataLengthW32 );
    
    ViSrcW32 = ViSrcW32BeforeDataCpyLoop + ( 2 * MI26__ZS_FFRAME_RAW_MAX_W32 );
    
    // err_error (( ERR_OUT, "TRACE -> After loop - ViFrame=%d - VDataLengthW32=%d", ViFrame, VDataLengthW32 ));
    
    VPtFrame->Header.AMapsTrailer[0] = PtSrcW32[(2 * MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * VFrameId) + (2 * (3 + VDataLengthW32))];    // 3 = Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32; // Count Trailer field
    ++ViSrcW32; // Count extra channel trigger field
    
    // Read trigger number from "Mi26" frame
    
    if ( TrigStatus == 0 ) {
      VZero = PtSrcW32[(2 * MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * VFrameId) + (2 * (3 + MI26__ZS_FFRAME_RAW_MAX_W32 + 1))];
    }
    
    // Emulate trigger by overwriting info from "Mi26" frame
    
    else {
      VZero = (TrigStatus << 16);
    }
    
    ++ViSrcW32; // Count Zero field
    ++ViSrcW32; // Count extra channel trigger field
    
    VZero2 = PtSrcW32[(2 * MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * VFrameId) + (2 * (3 + MI26__ZS_FFRAME_RAW_MAX_W32 + 2))];
    ++ViSrcW32; // Count Zero2 field
    ++ViSrcW32; // Count extra channel trigger field
    
    // Extraction of trigger info from "Mimosa 26 frame" zero fields
    
    VTrigNb = (VZero  & 0xFFFF0000) >> 16;
    
    if ( VTrigNb > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ) {
      err_warning (( ERR_OUT, "VTrigNb=%d > Max=%d => Limit to max !", VTrigNb, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ));
      VTrigNb = EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB;
    }
    
    
    if ( VTrigNb != 0 ) {
      VATrigVal[0]    = (VZero  & 0x0000FFFF);
      VATrigVal[1]    = (VZero2 & 0xFFFF0000) >> 16;
      VATrigVal[2]    = (VZero2 & 0x0000FFFF);
      
      VATrigLine[0]   = VATrigVal[0] / 16;
      VATrigLine[1]   = VATrigVal[1] / 16;
      VATrigLine[2]   = VATrigVal[2] / 16;
      
      VATrigClk[0]    = VATrigVal[0] % 16;
      VATrigClk[1]    = VATrigVal[1] % 16;
      VATrigClk[2]    = VATrigVal[2] % 16;
      
      VAMi26Trig[0]   = VATrigLine[0] /* + ( VATrigClk[0] << 10 ) */;
      VAMi26Trig[1]   = VATrigLine[1] /* + ( VATrigClk[1] << 10 ) */;
      VAMi26Trig[2]   = VATrigLine[2] /* + ( VATrigClk[2] << 10 ) */;
    }
    
    else {
      VAMi26Trig[0] = VAMi26Trig[1] = VAMi26Trig[2] = 0;
    }
    
    // Add trigger info in frame header
    
    VPtFrame->Header.TriggerNb        = VTrigNb;
    VPtFrame->Header.AMapsTrigInfo[0] = VAMi26Trig[0];
    VPtFrame->Header.AMapsTrigInfo[1] = VAMi26Trig[1];
    VPtFrame->Header.AMapsTrigInfo[2] = VAMi26Trig[2];
    
    // Add trigger info in trigger record
    
    VPtTrigRec = (EFRIO__TTriggerRec*) (VPtFrame->Data.ADataW32 + VDataLengthW32);
    
  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtTmpTrigRec->Tag      = EFRIO__FRAME_TAG_TRIG;
  #endif
  
    VPtTmpTrigRec->TrigNb   = VTrigNb;
    VPtTmpTrigRec->TotSz    = VEmptyTrigRecSz + (VTrigNb * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ);
    VPtTmpTrigRec->TrigType = 2;
    
    memcpy ( VPtTrigRec, VPtTmpTrigRec, VPtTmpTrigRec->TotSz );
    
    // Calculate size of frame record while filled ( fixed part + variable length (data & trigger) )
    
    // Calculate size will be 2 x W32 higher than needed, because following fields will be counted twice
    //
    // - Field UInt32 ADataW32[0] of data record -> EFRIO__TFrameData
    // - Field UInt32 ATrig[0] of trigger record -> EFRIO__TTriggerRec
    
    VFullFrameRecSz = VEmptyFrameRecSz + VDataLengthW8 + VPtTrigRec->TotSz;

    VTotAcqSz += VFullFrameRecSz;

    // Update frame field total size & trigger offset size
    
    VPtFrame->TotSz         = VFullFrameRecSz;
    VPtFrame->TrigRecOffset = VEmptyFrameRecSz + VDataLengthW8;
    
    if ( 0 /* VPtServ->PtParAcqReqFunc->AsicChkFunc >= 0 */ ) {
      
      // No neeed to reset ResAsicErrorsRejCurAcq ( it's done on DPXI__MI26_FAsicChkFunc0Emul6Mi26 with ViFrame = 0 )
      // => ResAsicErrorsRejCurAcq = 1 if errors in current Acq
      
      // $$$ PtAcq->ResAsicErrorsRejCurAcq = DPXI__MI26_FAsicChkFuncMi26 ( VPtServ->PtParAcqReqFunc->AsicChkFunc, ViFrame, VptZsFFrameRaw, 1 /* Mi26Nb */ );
      // $$$ PtAcq->ResAsicErrorsRejCurAcq = 0;
      
      // $$$ if ( PtAcq->ResAsicErrorsRejCurAcq != 0 ) {
        // $$$   msg (( MSG_OUT, "ViFrame=%d - VAsicErrorsRejCurAcq=%d", ViFrame, PtAcq->ResAsicErrorsRejCurAcq ));
        // $$$ }
    }
    
    
    ++VPtFrList->TotFrameNb;
    
    ++VRunFrameCnt;
    
  } // End for ViFrameWithTrig
  

    ++VPtCont->RunCont.ResAcqCnt;
  
    PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
    
    
    return (VTotAcqSz);
  }
  


#endif