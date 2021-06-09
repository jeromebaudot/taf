
/*******************************************************************************
File      : x:\lib\win\eudet_frio\eudet_frio.c
Goal      : Functions of flex rio board library for EUDET Mimosa 26 DAQ
Prj date  : 05/08/2010
File date : 05/08/2010
Doc date  : 05/08/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr

License   : You are free to use this source files for your own development as long
          : as it stays in a public research context. You are not allowed to use it
          : for commercial purpose. You must put this header with laboratory and
          : authors names in all development based on this library.
----------------------------------------------------------------------------------
Labo      : IPHC */
/*******************************************************************************/


#ifndef EUDET_FRIO_C
#define EUDET_FRIO_C

#define ERR_LOG_LVL_NONE             0
#define ERR_LOG_LVL_ALL              1
#define ERR_LOG_LVL_WARINGS_ERRORS   2
#define ERR_LOG_LVL_WARNINGS_ERRORS  2
#define ERR_LOG_LVL_ERRORS           3


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    : None
:
Ouputs    : The function returns
:  Acquisition size if ok
: -1 if an error occurs
:
Globals   :
:
Remark    :
:
Level     :
Date      : 15/02/2011
Doc date  : /2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FCpyAcq ( UInt8* PtDest, SInt32 MaxDestSz, SInt32 AcqSz  ) {
  
  EFRIO__TContext*     VPtCont   = &EFRIO__VGContext;
  EFRIO__TRunCont*     VPtRun    = &VPtCont->RunCont;
  UInt32*              VPtDest;
  SInt32               VTotSz;
    
  // Check destination buffer 

  err_retnull (  PtDest, (ERR_OUT,"PtDest == NULL") );
  
  // Calculate total size
  // = AcqSz + 4 because first W32 is used to store size of acquistion
  
  VTotSz = AcqSz + 4;

  // Test buffer size  
    
  if ( VTotSz > MaxDestSz ) {
    err_retfail ( -1, (ERR_OUT,"Destination buffer is too small => VTotSz=%d > MaxDestSz=%d", VTotSz, MaxDestSz ) );
  }

  // -----------------------------------------------
  // Copy data
  // -----------------------------------------------
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    // PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
  #endif
  
  // Add size on first elt
  
  VPtDest = (UInt32*) PtDest;
  
  *VPtDest = VTotSz;
  
  ++VPtDest;
  
  // Copy 
  
  memcpy ( VPtDest, VPtRun->PtFrame, AcqSz );
    
  #ifdef EFRIO__INCLUDE_PARA_PORT
    // PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
  #endif
  
  return (VTotSz);
  
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FBeginExt ( SInt8 MapsName, SInt8 ErrorLogLvl, char* ErrorLogFile, SInt8 MsgLogLvl, char* MsgLogFile )
          :
Goal      : Init lib
          :
Inputs    : MapsName     - Name of the MAPS ( ASIC__MI26, ASIC__ULT1, ... )
          : ErrorLogLvl  - Error log level can be
          :                ERR_LOG_LVL_NONE, ERR_LOG_LVL_ALL, ERR_LOG_LVL_WARNINGS_ERRORS, ERR_LOG_LVL_ERRORS
          :
          : ErrorLogFile - Error log file
          : MsgLogLvl    - Messages log level => 127 to get all messages
          : MsgLogFile   - Messages log file
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs 
          :
Globals   : 
          :
Remark    : Set default values to Acq emul header & trailer fields
          :
Level     : 
Date      : 05/08/2010
Rev       : 11/05/2011
          : - Handle version information -> majour, minor, comment
          :
          : 12/05/2011
          : - JTAG version Mi26 / Ult1 handling
          :
          : 19/05/2011
          : - Dma host size calculated for Ultimate because we don't know at this step which
          :   MAPS will be used and memory size for Ultimate is > the one for Mimosa 26
          :
          : 25/05/2011
          : - Renamed in EFRIO__FBeginExt
          : - Add parameter MapsName
          : - Create a new EFRIO__FBegin (...) which call EFRIO__FBeginExt ( ASIC__MI26, ... )
          :   This will keep compatibility with previous sw (EUDET), because EFRIO__FBegin call
          :   will configure Mimosa 26 as ASIC as it was done for first version of EUDET library.
          :
          : 28/09/2011
          : - Update code because EFRIO_VGJtagMi26.get_Info (...) has changed => returns one more param = SW version now
          :
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FBeginExt ( SInt8 MapsName, SInt8 ErrorLogLvl, char* ErrorLogFile, SInt8 MsgLogLvl, char* MsgLogFile ) {
  
  EFRIO__TContext*     VPtCont  = &EFRIO__VGContext;
  EFRIO__TBoardConf*   VPtBoard = &EFRIO__VGContext.ABoardsConf[0];
  EFRIO__TAcqEmul*     VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
  SInt32 VRet;
  SInt8  VEnableErrLog;
  SInt8  VEnableMsgLog;
  WideString VWsJtagStatus;
  WideString VWsSwVersion;

#ifdef EFRIO__INCLUDE_PARA_PORT
  HRESULT              VHrComErr;
  WideString           VStrComStatus;
#endif
  
  // Select JTAG Mi26 or Ultimate
  
  switch ( MapsName ) {
    
    case ASIC__MI26 : {
      EFRIO__VGJtagMi26Ult1 = 0; // 0 => Mi26, 1 => Ultimate
      break; }

    case ASIC__ULT1 : {
      EFRIO__VGJtagMi26Ult1 = 1; // 0 => Mi26, 1 => Ultimate
      break; }
    
    default : {
      err_retfail ( -1, (ERR_OUT,"Unknown MAPS name = %d => Abort", MapsName) );
      break; }
    
  }
  
  // Reset lib context record => Set all field to 0
  // BUT ! Set CmdRun to -1
  
  memset ( &EFRIO__VGContext, 0, sizeof (EFRIO__TContext) );

  VPtCont->RunCont.CmdRun = -1;
  
  // Set version information ( only for version >= 2.0 )

  #ifdef EFRIO__V20_AND_LATER
  
    #ifdef EFRIO__V20
      VPtCont->VersionMajor = 2;
      VPtCont->VersionMinor = 0;
      
      sprintf ( VPtCont->VersionCmt, "Extension of V1.0 to Ultimate 1" );
    #endif
    
  #endif

  
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
    
  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FBegin ( VEnableErrLog, ErrorLogFile );
    PPO_FOpen  ( 0x378 /* BaseAdr */, 0 /* Id */ );
    PPO_FEnableReadDataOutPortBeforeWrite ( 0 /* Id */, 1 /* Enable */ );
  #endif
    
  // Set default values to first board conf in order to get DmaHostSize initialized BEFORE fw loading
    
  VPtBoard->AsicNb        = EFRIO__MAX_ASIC_NB;    // Max possible number
  VPtBoard->FrameNbPerAcq = EFRIO__MAX_FRAME_NB_PER_ACQ; // "Nominal" value
  
  // DMA host size is the memory size allocated for DMA on CPU side
  // It must be equal AT LEAST to the size of one acquisition and higher value are not useful
  // VPtBoard->AsicNb + 1 => + 1 for extra channel

  // 25/05/2011 => Calculate DMA host size in function of ASIC name

  switch ( MapsName ) {
    
    case ASIC__MI26 : {
      VPtBoard->DmaHostSz = ((MI26__ZS_FFRAME_MODE_2X80MHZ_W8_SZ  * 2 * (EFRIO__MAX_ASIC_NB_FOR_DMA + 1) * VPtBoard->FrameNbPerAcq) / (1024 * 1024)) + 5;
      break; }
    
    case ASIC__ULT1 : {
      VPtBoard->DmaHostSz = ((ULT1__ZS_FFRAME_MODE_2X160MHZ_W8_SZ * 2 * (EFRIO__MAX_ASIC_NB_FOR_DMA + 1) * VPtBoard->FrameNbPerAcq) / (1024 * 1024)) + 5;
      break; }
    
    default : {
      err_retfail ( -1, (ERR_OUT,"Unknown MAPS name = %d => Abort", MapsName) );
      break; }
    
  }
  
  
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
  

#ifdef EFRIO__INCLUDE_JTAG


#ifdef OLD_COM_JTAG_CODE

  // Init JTAG
  
   if( ! EFRIO_VGJtagMi26.IsBound()) {
     OleCheck(CoMI26MasterConf::Create(EFRIO_VGJtagMi26));
    }
  
   if ( EFRIO_VGJtagMi26.IsBound () ) {
     OleCheck ( EFRIO_VGJtagMi26.get_Info( &VWsJtagStatus ));
      msg (( MSG_OUT, "JTAG => %s", TOOLS__FWideString2CStr (VWsJtagStatus, NULL, 0) ));
   }
  
   else {
     err_warning (( ERR_OUT, "JTAG control disabled => Because JTAG application not running !" ));
    }
    
  // Init JTAG end


#else
  
  // COM handling

  VHrComErr = CoInitialize (NULL);
  
  if ( FAILED (VHrComErr) ) {
    err_retfail ( -1, (ERR_OUT,"CoInitialize failed !" ) );
  }
  
  
  // JTAG Mi26
  
  if ( EFRIO__VGJtagMi26Ult1 == 0 ) {
    
    VHrComErr =  CoMI26MasterConf::Create( EFRIO_VGJtagMi26 );
    
    if ( FAILED (VHrComErr) ) {
      CoUninitialize();
      err_retfail ( -1, (ERR_OUT,"CoMI26MasterConf::Create failed !" ) );
    }

    // 28/09/2011
    // => Update code because EFRIO_VGJtagMi26.get_Info (...) has changed => returns one more param = SW version now

    if ( EFRIO_VGJtagMi26.IsBound () ) {
      OleCheck ( EFRIO_VGJtagMi26.get_Info( &VWsSwVersion, &VWsJtagStatus ));
      msg (( MSG_OUT, "JTAG => Version %s - Status %s", TOOLS__FWideString2CStr (VWsSwVersion, NULL, 0), TOOLS__FWideString2CStr (VWsJtagStatus, NULL, 0) ));
    }
    
    else {
      err_warning (( ERR_OUT, "JTAG control disabled => Because JTAG application not running !" ));
    }    
    
  }
  
  // JTAG Ult1
  
  else {

    VHrComErr =  CoMI28COM::Create( EFRIO_VGJtagUlt1 );
    
    if ( FAILED (VHrComErr) ) {
      CoUninitialize();
      err_retfail ( -1, (ERR_OUT,"CoMI28COM::Create failed !" ) );
    }
    
    if ( EFRIO_VGJtagUlt1.IsBound () ) {
      OleCheck ( EFRIO_VGJtagUlt1.Info( &VWsJtagStatus ));
      msg (( MSG_OUT, "JTAG => %s", TOOLS__FWideString2CStr (VWsJtagStatus, NULL, 0) ));
    }
    
    else {
      err_warning (( ERR_OUT, "JTAG control disabled => Because JTAG application not running !" ));
    }
    
  }
  
    
  // Init JTAG end
  
#endif



#else
  err_warning (( ERR_OUT, "JTAG control disabled by conditionnal compilation" ));
  
#endif
  
  VPtCont->InfInitDone = 0;


#ifndef EFRIO__V20_AND_LATER
  
  err_error (( ERR_OUT, "******************************************************************" ));
  err_error (( ERR_OUT, "EUDET Flex RIO DLL compiled on %s at %s", __DATE__, __TIME__ ));
  err_error (( ERR_OUT, "******************************************************************" ));
  
#else

  err_error (( ERR_OUT, "******************************************************************" ));
  err_error (( ERR_OUT, "- EUDET Flex RIO DLL V%d.%d compiled on %s at %s", VPtCont->VersionMajor, VPtCont->VersionMinor, __DATE__, __TIME__ ));
  err_error (( ERR_OUT, "------------------------------------------------------------------" ));
  err_error (( ERR_OUT, "%s", VPtCont->VersionCmt ));
  err_error (( ERR_OUT, "******************************************************************" ));

#endif
  
  err_retok (( ERR_OUT, "end" ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FBegin ( SInt8 ErrorLogLvl, char* ErrorLogFile, SInt8 MsgLogLvl, char* MsgLogFile )
          :
Goal      : Init lib
          :
Inputs    : ErrorLogLvl  - Error log level can be
          :                ERR_LOG_LVL_NONE, ERR_LOG_LVL_ALL, ERR_LOG_LVL_WARNINGS_ERRORS, ERR_LOG_LVL_ERRORS
          :
          : ErrorLogFile - Error log file
          : MsgLogLvl    - Messages log level => 127 to get all messages
          : MsgLogFile   - Messages log file
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   :
          :
Remark    : Set default values to Acq emul header & trailer fields
          :
Level     :
Date      : 05/08/2010
Rev       : 11/05/2011
          : - Handle version information -> majour, minor, comment
          :
          : 12/05/2011
          : - JTAG version Mi26 / Ult1 handling
          :
          : 19/05/2011
          : - Dma host size calculated for Ultimate because we don't know at this step which
          :   MAPS will be used and memory size for Ultimate is > the one for Mimosa 26
          :
          : 25/05/2011
          : - Body of function is " empty " => it has been moved in EFRIO__FBeginExt (...)
          : - Call EFRIO__FBeginExt ( ASIC__MI26, ... )
          : - EFRIO__FBeginExt (...) handles MapsName parameter ( Mi26 / Ultimate / ... )
          :   EFRIO__FBegin still configure Mimosa 26 as it was done in first version of lib.
          :
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FBegin ( SInt8 ErrorLogLvl, char* ErrorLogFile, SInt8 MsgLogLvl, char* MsgLogFile ) {
  
  
  FIL__VGDbgPtEFRIOContext = &EFRIO__VGContext; /* Debug ! */
  
  err_error (( ERR_OUT, "TRACE => Init done" ));
  
  return ( EFRIO__FBeginExt ( ASIC__MI26, ErrorLogLvl, ErrorLogFile, MsgLogLvl, MsgLogFile ) );
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FEnd ()
          :
Goal      : Terminate lib
          :
Inputs    : None
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   :
          :
Remark    :
          :
Level     :
Date      : 05/08/2010
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FEnd () {
  
  EFRIO__TContext*     VPtCont  = &EFRIO__VGContext;
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FEnd ();
  #endif
  
  // Close JTAG COM classe
  // Avoid "assertion message" when LabView is stop and then restarted

  err_error (( ERR_OUT, "1" ));
  
#ifdef EFRIO__INCLUDE_JTAG


  // JTAG Mi26
  
  if ( EFRIO__VGJtagMi26Ult1 == 0 ) {  
    EFRIO_VGJtagMi26.Unbind();
  }
  
  else {
    EFRIO_VGJtagUlt1.Unbind();
  }
  
  
#endif
  
  // Free frames buffer if allocated

  err_error (( ERR_OUT, "2" ));
  
  if ( VPtCont->RunCont.PtZsFFrameRaw !=NULL ) {
    free ( VPtCont->RunCont.PtZsFFrameRaw );
  }
  
  // Reset context record

  err_error (( ERR_OUT, "3" ));
  
  memset ( &EFRIO__VGContext, 0, sizeof (EFRIO__TContext) );
  
  err_retok (( ERR_OUT, "" ));
}

  
/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FBuildFrameListFromAcq ( SInt32 AcqStatus, SInt32 TrigStatus, SInt32 FrameNb, void* PtAcqData, EFRIO__TFrameList* PtList )
          :
Goal      : Build the frame list for one acquisition 
          :
Inputs    : AcqStatus  - ACquisition status ( < 0 -> HW error, 0 -> OK, > 0 -> Frame nb lost )
          : TrigStatus - No meaning now, reserved for future use
          : FrameNb    - The number of frames in the acquisition
          : PtAcqData  - A pointer to source data = all frames of one acquisition
          : PtList     - A pointer to the frame list to build
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   :
          :
Remark    : This function is called to build the frame list ( eg : AAcqFrameList field
          : of lib context record ) while reading data from run file.
          :
          : For more information, read comments on EFRIO__TFrameList record in *.typ file
          :  
Level     :
Date      : 06/11/2010
Rev       : 24/02/2011
          : - Add parameters AcqStatus, TrigStatus to field new fields of EFRIO__TFrameList
          :
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FBuildFrameListFromAcq ( SInt32 AcqStatus, SInt32 TrigStatus, SInt32 FrameNb, void* PtAcqData, EFRIO__TFrameList* PtList ) {

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
  
  PtList->AcqStatus  = AcqStatus;
  PtList->TrigStatus = TrigStatus;
  PtList->TotFrameNb = FrameNb;

  // Set frame pointer on first frame
  
  VPtFirstFr = (EFRIO__TFrame*) PtAcqData;
  
  // Fill first elt
  
  PtList->AFrameSz[0]  = VPtFirstFr->TotSz;
  PtList->AFramePtr[0] = VPtFirstFr;
  
  VPtNextFr = (EFRIO__TFrame*) ( ((UInt8*) VPtFirstFr) + VPtFirstFr->TotSz );
  
  // Fill following elt
  
  for ( ViFrame=1; ViFrame < FrameNb; ViFrame++ ) {
    PtList->AFrameSz[ViFrame]  = VPtNextFr->TotSz;
    PtList->AFramePtr[ViFrame] = VPtNextFr;
    VPtNextFr = (EFRIO__TFrame*) ( ((UInt8*) VPtNextFr) + VPtNextFr->TotSz );
  }
  

  err_retok (( ERR_OUT, "" ));
}
  
  
  
/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FEmuleBegin ( SInt8 RunInLabview )
          :
Goal      : Init DAQ emulation either in standalone DAQ emulation application ( without
          : HW ) or in LabView DAQ application. Selection done by RunInLabview parameter.
          :
Inputs    : RunInLabview = 0 => Run in C++ Builder DAQ emulation application
          :              = 1 => Run in Labview DAQ
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   :
          :
Remark    : If emulation is running under Labview ( RunInLabview = 1 ), frames emulation
          : function overwite memory already allocated for Flew RIO board. In case emulation
          : is runing in a standalone application ( RunInLabview = 0 ), this function allocates
          : memory in PC DRAM to emulate Flex RIO RAM.
          :
          : This function sets default values of DAQ emulation parameters.
          :
Level     :
Date      : 31/10/2010
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

// RunInLabview = 0 => Run in C++ Builder DAQ emulation application
// RunInLabview = 1 => Run in Labview DAQ

SInt32 EFRIO__FEmuleBegin ( SInt8 RunInLabview ) {
  
  EFRIO__TRunCont*   VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TAcqEmul*   VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
  EFRIO__TFrCheck*   VPtFrChk   = &EFRIO__VGContext.FrCheck;


  // Reset context records
  
  memset ( VPtAcqEmul, 0, sizeof (EFRIO__TAcqEmul) );
    
  memset ( VPtFrChk  , 0, sizeof (EFRIO__TFrCheck) );
    
  // Set default value of DAQ emulation parameters => Mainly for Labview which will not control all of them   
      
  VPtAcqEmul->ParAcqCycleMs       = 200;
  VPtAcqEmul->ParEmuleDRamReadMs  =   0;
  VPtAcqEmul->ParEmuleFunctNo     =   0;
  VPtAcqEmul->ParRandomDataSz     =   0;
  VPtAcqEmul->ParSetMaxDataSzOnOneMaps = 1;
  
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
  
  VPtAcqEmul->ParATrig[0] = 10;
  VPtAcqEmul->ParATrig[1] = 20;
  VPtAcqEmul->ParATrig[2] = 30;
  VPtAcqEmul->ParATrig[3] = 40;
  
  VPtAcqEmul->ParATS[0]   = 100;
  VPtAcqEmul->ParATS[1]   = 200;
  VPtAcqEmul->ParATS[2]   = 300;
  VPtAcqEmul->ParATS[3]   = 400;
  
    
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
Prototype : SInt32 EFRIO__FEmuleEnd ( )
          :
Goal      : Terminate DAQ emulation
          :
Inputs    :
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   :
          :
Remark    : Free DRAM if allocated in PC to emulate Flex RIO DRAM.
          :
Level     :
Date      : 31/10/2010
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FEmuleEnd ( ) {
  
  EFRIO__TRunCont*   VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TAcqEmul*   VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
  EFRIO__TFrCheck*   VPtFrChk   = &EFRIO__VGContext.FrCheck;
  
  
  if ( VPtAcqEmul->InfDRamPtr != NULL ) {
    free ( VPtAcqEmul->InfDRamPtr );
  }
  
  err_retok (( ERR_OUT, "" ));
  
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : UInt32* EFRIO__FEmuleReadDRam ( SInt8 Cmd )
          :
Goal      : Emulate Flex RIO DRAM read
          :
Inputs    : Cmd - 0 => Do nothing
          :     - 1 => Fill memory with zero
          :
Ouputs    : A pointer to DRAM or NULL if not allocated
          :
Globals   :
          :
Remark    : A delay should also be added here later to emulate Flex RIO DRAM access time
          : configured by field ParEmuleDRamReadMs of EFRIO__TAcqEmul.
          :
Level     :
Date      : 31/10/2010
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

UInt32* EFRIO__FEmuleReadDRam ( SInt8 Cmd ) {

  EFRIO__TAcqEmul*   VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
  
  
  if ( Cmd == 1 ) {
    memset ( VPtAcqEmul->InfDRamPtr, 0, VPtAcqEmul->InfDRamSz );
  }
  
  Sleep ( VPtAcqEmul->ParEmuleDRamReadMs );
  
  return ( VPtAcqEmul->InfDRamPtr );
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FSetFrameIdInTriggerRec ( SInt32 FrameId, SInt32 TrigNb, EFRIO__TTriggerRec* PtRec )
          :
Goal      : Used for DAQ emulation.
          : Set the FrameId fields ( TLU & Flex RIO triggers ) of all triggers info in
          : the record used to emulate triggers
          :
Inputs    : FrameId - The value of frame id to set in all triggers info
          : TrigNb  - The number of trigger info to set in record
          : PtRec   - Pointer to destination record
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   :
          :
Remark    : The field set are
          : - for TLU trigger      => F.FrameIdInAcq ( see record EFRIO__TTluTrigger in *.typ )
          : - for Flex RIO trigger => F.Mi26Frame    ( see record EFRIO__TFlexRioTimeStamp1 in *.typ )
          :
Level     : 
Date      : 03/11/2010
Doc date  : 07/11/2010
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

  

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : UInt32* EFRIO__FTrfData ( SInt8 CmdGetPtrCpyAlloc, UInt32 AllocW32Sz, UInt32* PtSrcW32, UInt32 SrcW32Sz )
          :
Goal      : Function used to avoid data copy while transfering data from one Vi to
          : another under Labview. Because sometimes, it seems that Labview use pointers
          : but it's not the case, a copy of data is done, and execution time is lost ...
          :
Inputs    : CmdGetPtrCpyAlloc = 0 => return buffer pointer
          :                   = 1 => store PtSrcW32 but NO copy of data
          :                   = 2 => copy src to buffer
          :                   = 3 => alloc buffer
          :
          : AllocW32Sz - Memory size to alloc in W32 ( if CmdGetPtrCpyAlloc = 3 )
          : PtSrcW32   - Pointer to source data to copy to buffer
          : SrcW32Sz   - Size of source data to copy to buffer
          :
Ouputs    : A pointer on the buffer or NULL if not allocated.
          :
Globals   :
          :
Remark    : It's mainly used to passe a pointer on board DRAM to DLL ( CmdGetPtrCpyAlloc = 1 )
          :
Level     :
Date      : 07/09/2010
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

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
Prototype : SInt32 EFRIO__FConfRun ( SInt8 Mi26Nb, SInt32 RunNo, SInt32 TotEvNb, SInt32
          :   EvNbPerFile, SInt32 FrameNbPerAcq, SInt8 DataTransferMode, char* DestDir,
          :   char* FileNamePrefix, SInt8 SaveToDisk, SInt8 SendOnEth, SInt8 SendOnEthPCent )
          :
Goal      : Config run parameters, eg : get them from GUI or Ethernet
          :
Inputs    : Mi26Nb            - Mimosa 26 number in the DAQ
          : RunNo             - Run no
          : TotEvNb           - Tot events number in run
          : EvNbPerFile       - Events number per file
          : FrameNbPerAcq     - Frames number per acquisition
          :
          : DataTransferMode  - Data transfert mode
          :
          : 0 - EFRIO__TRF_MODE_IPHC
          : => Demultiplex the data part, doesn't handle extra channel -> for compatibility with IPHC DAQ sw
          :
          : 1 - EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN
          : => Don't demultiplex data part, don't care about extra channel, send all frames
          :
          : 2 - EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES
          : => Don't demultiplex data part, extract trigger info from extra channel, send all frames
          :
          : 3 - EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG
          : => Don't demultiplex data part, extract trigger info from extra channel, send only frames with trigger ( use TriggerHandlingMode parameter )
          :
          : DestDir           - Destination directory for run file
          : FileNamePrefix    - Prefix of run file name ( eg : RUN_666 => "RUN" is the prefix )
          : SaveToDisk        - Save or not data to disk
          : SendOnEth         - Send or not data to Ethernet
          : SendOnEthPCent    - % of events send on Ethernet ( if SendOnEth = 1 )
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   :
          :
Remark    :
          :
Level     :
Date      : 06/08/2010 => Creation with all job done in function body
          :
Rev       : 12/05/2011
          : - Code moved to EFRIO__FConfRunExt (...)
          : - Call EFRIO__FConfRunExt ( ASIC__MI26, ... )
          :
          :
Doc date  : 12/05/2011
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FConfRun ( SInt8 Mi26Nb, SInt32 RunNo, SInt32 TotEvNb, SInt32 EvNbPerFile, SInt32 FrameNbPerAcq, SInt8 DataTransferMode, SInt8 TrigMode, char* DestDir, char* FileNamePrefix, SInt8 SaveToDisk, SInt8 SendOnEth, SInt8 SendOnEthPCent ) {

  SInt32 VRet;

  VRet = EFRIO__FConfRunExt ( ASIC__MI26, Mi26Nb, RunNo, TotEvNb, EvNbPerFile, FrameNbPerAcq, DataTransferMode, TrigMode, DestDir, FileNamePrefix, SaveToDisk, SendOnEth, SendOnEthPCent );
  
  return (VRet);
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__FConfRunExt ( SInt8 MapsName, SInt8 MapsNb, SInt32 RunNo, SInt32 TotEvNb, 
          :   SInt32 EvNbPerFile, SInt32 FrameNbPerAcq, SInt8 DataTransferMode, char* DestDir,
          :   char* FileNamePrefix, SInt8 SaveToDisk, SInt8 SendOnEth, SInt8 SendOnEthPCent ) 
          :
Goal      : Config run parameters, eg : get them from GUI or Ethernet
          :
Inputs    : MapsName          - Name of the MAPS ( ASIC__MI26, ASIC__ULT1, ... )
          : MapsNb            - MAPS number in the DAQ
          : RunNo             - Run no
          : TotEvNb           - Tot events number in run
          : EvNbPerFile       - Events number per file
          : FrameNbPerAcq     - Frames number per acquisition
          :
          : DataTransferMode  - Data transfert mode
          :
          : 0 - EFRIO__TRF_MODE_IPHC
          : => Demultiplex the data part, doesn't handle extra channel -> for compatibility with IPHC DAQ sw
          :
          : 1 - EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN
          : => Don't demultiplex data part, don't care about extra channel, send all frames
          :
          : 2 - EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES
          : => Don't demultiplex data part, extract trigger info from extra channel, send all frames
          :
          : 3 - EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG
          : => Don't demultiplex data part, extract trigger info from extra channel, send only frames with trigger ( use TriggerHandlingMode parameter )
          :
          : DestDir           - Destination directory for run file
          : FileNamePrefix    - Prefix of run file name ( eg : RUN_666 => "RUN" is the prefix )
          : SaveToDisk        - Save or not data to disk
          : SendOnEth         - Send or not data to Ethernet
          : SendOnEthPCent    - % of events send on Ethernet ( if SendOnEth = 1 )
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   :
          :
Remark    :
          :
Level     :
Date      : 06/08/2010 -> Creation of EFRIO__FConfRun (...)
          : 12/05/2011 -> Change name from EFRIO__FConfRun to EFRIO__FConfRunExt and add MapsName parameter
          :               See " Rev 12/05/2011 "
          ;
          :
Rev       : 04/11/2010
          : - Save to disk
          :
          : 21/02/2011
          : - Set new fields ( ParDaqVersion, ParMapsName )
          :
          : 12/05/2011
          : - A new parameter MapsName must be added, in order to keep compatibility with EUDET lib :
          : -> This function has been renamed EFRIO__FConfRunExt ( it was EFRIO__FConfRun before )
          : -> The new parameter MapsName had been added
          : -> The former EFRIO__FConfRun call now EFRIO__FConfRunExt with MapsName param = ASIC__MI26
          :
          :
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FConfRunExt ( SInt8 MapsName, SInt8 MapsNb, SInt32 RunNo, SInt32 TotEvNb, SInt32 EvNbPerFile, SInt32 FrameNbPerAcq, SInt8 DataTransferMode, SInt8 TrigMode, char* DestDir, char* FileNamePrefix, SInt8 SaveToDisk, SInt8 SendOnEth, SInt8 SendOnEthPCent ) {
  
  EFRIO__TContext*     VPtCont    = &EFRIO__VGContext;
  EFRIO__TBoardConf*   VPtBoard   = &EFRIO__VGContext.ABoardsConf[0];
  SInt32 VMaxFrameSz;
  SInt32 VRet;
  
    
  // Check function parameters
  
  err_retnull ( DestDir       , (ERR_OUT,"Abort : DestDir == NULL        !") );
  err_retnull ( FileNamePrefix, (ERR_OUT,"Abort : FileNamePrefix == NULL !") );
  
  // Debug print function parameters
  
  err_error (( ERR_OUT, "Call with : MapsNb=%d - RunNo=%d - TotEvNb=%d - EvNbPerFile=%d - FrameNbPerAcq=%d - DestDir=%s - FileNamePrefix=%s", MapsNb, RunNo, TotEvNb, EvNbPerFile, FrameNbPerAcq, DestDir, FileNamePrefix ));
    
  // Set hard coded parameters
  
  VPtCont->RunCont.ParMeasDataRate          =  1; // Enable data rate measurement
  VPtCont->RunCont.ParAcqNbToMeasDataRate   = 10; // Uses 10 acquistions to measure data rate
  
  // Set default values to new fields
  
  VPtCont->RunCont.ParDaqVersion = 0;
  
  // Copy run parameters to run context record
  
  VPtCont->RunCont.ParMapsName         = MapsName;
  VPtCont->RunCont.ParMi26Nb           = MapsNb;
  VPtCont->RunCont.ParRunNo            = RunNo;
  VPtCont->RunCont.ParTotEvNb          = TotEvNb;
  VPtCont->RunCont.ParEvNbPerFile      = EvNbPerFile;
  VPtCont->RunCont.ParFrameNbPerAcq    = FrameNbPerAcq;
  VPtCont->RunCont.ParDataTransferMode = DataTransferMode;
  VPtCont->RunCont.ParTrigMode         = TrigMode;
  
  VPtCont->RunCont.ParSaveOnDisk       = SaveToDisk;
  VPtCont->RunCont.ParSendOnEth        = SendOnEth;
  VPtCont->RunCont.ParSendOnEthPCent   = SendOnEthPCent;
  
  sprintf ( VPtCont->RunCont.ParDestDir       , "%s", DestDir );
  sprintf ( VPtCont->RunCont.ParFileNamePrefix, "%s", FileNamePrefix );
  
  sprintf ( VPtCont->RunCont.InfDataFileName, "%s\\%s%d.bin", VPtCont->RunCont.ParDestDir, VPtCont->RunCont.ParFileNamePrefix, VPtCont->RunCont.ParRunNo);
  
  sprintf ( VPtCont->RunCont.InfConfFileName, "%s\\%s%d.par", VPtCont->RunCont.ParDestDir, VPtCont->RunCont.ParFileNamePrefix, VPtCont->RunCont.ParRunNo);  

  VPtCont->RunCont.InfSaveDataOnDiskRunning = 0;

  // Compare run conf paramters to the max parameters used to calculate DmaHostSz in FBegin
  // If they have higher values => DmaHostSz send to board at fw loading is bad => Abort 
  
  if ( (VPtCont->RunCont.ParMi26Nb > EFRIO__MAX_ASIC_NB) || (VPtCont->RunCont.ParFrameNbPerAcq > EFRIO__MAX_FRAME_NB_PER_ACQ) ) {
    err_error   (( ERR_OUT, "Bad Mi26 nb          ? Run conf = %d - Max = %d", VPtCont->RunCont.ParMi26Nb, EFRIO__MAX_ASIC_NB ));
    err_error   (( ERR_OUT, "Bad frame nb per acq ? Run conf = %d - Max = %d", VPtCont->RunCont.ParFrameNbPerAcq, EFRIO__MAX_FRAME_NB_PER_ACQ ));
    err_retfail ( -1, (ERR_OUT,"Abort : Bad value of AsicNb=%d or FrameNbPerAcq=%d", VPtCont->RunCont.ParMi26Nb, VPtCont->RunCont.ParFrameNbPerAcq ) );
  }
  
  // Update monitor context fields
  
  VPtCont->MonCont.InfFrameNbToSend = (SInt32) ( (float) FrameNbPerAcq * (float) ( (float) SendOnEthPCent / (float) 100 ));
  
  VPtCont->MonCont.InfSzToSend = 0; // Because it's not known at this step
  
  err_error (( ERR_OUT, "TRACE => InfFrameNbToSend = %d", VPtCont->MonCont.InfFrameNbToSend ));
  
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
  
  err_trace (( ERR_OUT, "EnableExtraChannel=%d", VPtBoard->EnableExtraChannel ));
  
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
    
    // Reset frame list
  
    memset ( VPtCont->AAcqFrameList, 0, sizeof (EFRIO__TFrameList) );
      
    // Alloc 
    
    if ( VPtCont->RunCont.PtFrame != NULL ) {
      free ( VPtCont->RunCont.PtFrame );
    }
      
    VPtCont->RunCont.InfFrameBuffSz = 0;
    
    // Upgrade for Ultimate
    
    // VMaxFrameSz = ( sizeof ( EFRIO__TFrame ) + ( VPtCont->RunCont.ParMi26Nb * MI26__ZS_FFRAME_RAW_MAX_W8 ) + sizeof ( EFRIO__TTriggerRec ) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );

    VMaxFrameSz = ( sizeof ( EFRIO__TFrame ) + ( VPtCont->RunCont.ParMi26Nb * ULT1__ZS_FFRAME_RAW_MAX_W8 ) + sizeof ( EFRIO__TTriggerRec ) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );

    VPtCont->RunCont.InfFrameBuffSz = VPtCont->RunCont.ParFrameNbPerAcq * VMaxFrameSz;
          
    VPtCont->RunCont.PtFrame = (EFRIO__TFrame*) malloc ( VPtCont->RunCont.InfFrameBuffSz );
  
    err_retnull ( VPtCont->RunCont.PtFrame, (ERR_OUT,"Allocation of EUDET buffer for %d frames failed !", VPtCont->RunCont.ParFrameNbPerAcq) );
    
    err_error (( ERR_OUT, "TRACE => Frames buffer sz = %d Bytes", VPtCont->RunCont.InfFrameBuffSz ));
    err_error (( ERR_OUT, "TRACE => Frames buffer sz = %d MBytes", VPtCont->RunCont.InfFrameBuffSz / (1024 * 1024) ));
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
Prototype : EFRIO__F
          :
Goal      : 
          :
Inputs    : None
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   :
          :
Remark    :
          :
Level     :
Date      : 08/11/2010
Rev       : 15/02/2011
          : - Handle InfSaveDataOnDiskRunning
          : 07/03/2011
          : - Get disk sector size from OS
          :
          : 20/05/2011
          : - Use run param to calculate TCStreamFile max buffer size
          :
          : 13/10/2011
          : - Add creation of a copy of run conf file in x:\log ( VInfConfFileNameCpy ).
          :
          :          
Doc date  : /2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FStartSavingOnFile () {

  SInt32               VRet;
  char                 VDiskDrive[3];
  SInt32               VDiskSectorSz;
  SInt8                VUseThread;
  EFRIO__TRunCont*     VPtRunCont = &EFRIO__VGContext.RunCont;
  MI26__TZsFFrameRaw*  VPtZsFFrameRaw;           
  EFRIO__TFrame*       VPtFrame;
  SInt32               VStreamFileMaxBlocSz;
  char                 VInfConfFileNameCpy[GLB_FILE_PATH_SZ]; // Copy in x:\log of the run conf file
  

  VRet = 0;

  if ( VPtRunCont->ParSaveOnDisk > 0 ) {
    
    // Debug
    
    msg (( MSG_OUT, "EFRIO__FStartSavingOnFile () => Run No = %d", VPtRunCont->ParRunNo ));
    
    // Create & write conf file
    
    // msg (( MSG_OUT, "VPtRunCont->ResConfFileName=%s", VPtRunCont->ResConfFileName ));
    
    // Set pointer to 0 before saving and restor them afer
    
    VPtZsFFrameRaw = VPtRunCont->PtZsFFrameRaw;
    VPtFrame       = VPtRunCont->PtFrame;
    
    VPtRunCont->PtZsFFrameRaw = NULL;
    VPtRunCont->PtFrame       = NULL;
    
    VRet = VRet | EFRIO__VGRunConfFile.PubFConf   ( VPtRunCont->InfConfFileName, FIL__TCBinFile_RWB_MODE_WRITE, sizeof (EFRIO__TRunCont), sizeof (EFRIO__TRunCont), 1 /* FlushAfterWrite */, 0 /* MeasTime */ );
    VRet = VRet | EFRIO__VGRunConfFile.PubFCreate ();
    VRet = VRet | EFRIO__VGRunConfFile.PubFSeqWrite ( VPtRunCont, sizeof (EFRIO__TRunCont) );
    VRet = VRet | EFRIO__VGRunConfFile.PubFClose ();
    
    VPtRunCont->PtZsFFrameRaw = VPtZsFFrameRaw;
    VPtRunCont->PtFrame       = VPtFrame;
    
    err_retfail (  VRet, (ERR_OUT,"Run config file = %s creation failed !", VPtRunCont->InfConfFileName) );
    
    // Make a copy of run info file
    
    sprintf ( VInfConfFileNameCpy, "x:\\log\\%s%d.par", VPtRunCont->ParFileNamePrefix, VPtRunCont->ParRunNo ); // 13/10/2011
      
    VRet = FIL_FCpyFile ( VPtRunCont->InfConfFileName, VInfConfFileNameCpy );
    
    if ( VRet != 0 ) {
      err_warning (( ERR_OUT, "Copy of info = %s file i, %s failed !", VPtRunCont->InfConfFileName, VInfConfFileNameCpy ));
      err_error (( ERR_OUT, "Copy of info = %s file i, %s failed !", VPtRunCont->InfConfFileName, VInfConfFileNameCpy ));
    }

    // Create data file
    
    if ( VPtRunCont->ParSaveOnDisk == 1 ) {
      VUseThread = 1;
    }
    
    else {
      VUseThread = 0;      
    }
    
    
    // Get disk sector size
    
    strncpy ( VDiskDrive, VPtRunCont->InfDataFileName, 2 );
    
    VDiskDrive[2] = 0;
    
    VDiskSectorSz = FIL_FGetDiskSectorSz ( VDiskDrive );
    
    err_retfail ( VDiskSectorSz, (ERR_OUT,"Abort => Unable to get drive %s sector size !", VDiskDrive ) );
    
    msg (( MSG_OUT, "Disk sector sz = %d Bytes", VDiskSectorSz ));
    
    // Set TCStreamFile disk sector size
    
    VRet = VRet | EFRIO__VGRunDataFile.PubFSetDiskSectorSz ( VDiskSectorSz );
    
    // Conf TCStreamFile
    
    // VRet = VRet | EFRIO__VGRunDataFile.PubFConf ( &EFRIO__VGRunDataFile, VUseThread /* UseThread */, VPtRunCont->InfDataFileName, FIL__TCBinFile_RWB_MODE_WRITE, 0 /* FixedBlocSzMode */, EFRIO__MAX_DATA_FILE_BLOC_SZ /* Max */, EFRIO__MAX_DATA_FILE_BLOC_SZ /* Bloc */, 1 /* FlushAfterWrite */, 0 /* MeasTime */ );
    // VRet = VRet | EFRIO__VGRunDataFile.PubFCreate ();

    // 20/05/2011

    VStreamFileMaxBlocSz = ( VPtRunCont->ParFrameNbPerAcq * ( sizeof ( EFRIO__TFrame ) + ( EFRIO__MAX_ASIC_NB * ULT1__ZS_FFRAME_RAW_MAX_W8 ) + sizeof ( EFRIO__TTriggerRec ) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) ) );

    err_error (( ERR_OUT, "TRACE => Calc TCStreamFile max bloc sz - %d Fr/Acq - Bloc Sz = %d MB", VPtRunCont->ParFrameNbPerAcq, VStreamFileMaxBlocSz / (1024 * 1024) ));
    
    VRet = VRet | EFRIO__VGRunDataFile.PubFConf ( &EFRIO__VGRunDataFile, VUseThread /* UseThread */, VPtRunCont->InfDataFileName, FIL__TCBinFile_RWB_MODE_WRITE, 0 /* FixedBlocSzMode */, VStreamFileMaxBlocSz /* Max */, VStreamFileMaxBlocSz /* Bloc */, 1 /* FlushAfterWrite */, 0 /* MeasTime */ );
    VRet = VRet | EFRIO__VGRunDataFile.PubFCreate ();
    
    
    err_retfail (  VRet, (ERR_OUT,"Run data file = %s creation failed !", VPtRunCont->InfDataFileName) );
    
    VPtRunCont->InfSaveDataOnDiskRunning = 1;
    
  }
  
  
}
  

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : EFRIO__F
:
Goal      :
:
Inputs    : None
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    :
:
Level     :
Date      : 08/11/2010
Rev       : 15/02/2011
          : - Handle InfSaveDataOnDiskRunning
          : 16/02/2011
          : - Test if ParTotEvNb reached, if yes => call EFRIO__FStopSavingOnFile ()
          :
          : 24/02/2011
          : - Handle SpareW32Par as an array ASpareW32Par now
          :
Doc date  : /2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FSaveAcqOnFile () {
  
  SInt32                   VRet;
  SInt8                    VUseThread;
  EFRIO__TRunCont*         VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*       VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];  
  EFRIO__TFileSpareW32Info VSpareW32Info;
  
  
  VRet = 0;
    
  if ( VPtRunCont->InfSaveDataOnDiskRunning == 0 ) {
    return (0);
  }
  
  if ( VPtRunCont->ResEventCnt > VPtRunCont->ParTotEvNb ) {
    EFRIO__FStopSavingOnFile ();
    err_error (( ERR_OUT, "TRACE => End of run reached : %d events saved on disk", VPtRunCont->ResEventCnt ));
    return (0);
  }

  // Save acq on file if
  // - Run conf "save on disk" parameter is set
  // - There is something to save => ResAcqFunctRetCode = acquisition size <> 0

  if ( (VPtRunCont->ParSaveOnDisk > 0) && (VPtRunCont->ResAcqFunctRetCode > 0) ) {
    
    VSpareW32Info.AcqStatus    = VPtFrList->AcqStatus;
    VSpareW32Info.TrigStatus   = VPtFrList->TrigStatus;
    VSpareW32Info.TotFrameNb   = VPtFrList->TotFrameNb;
    VSpareW32Info.DiskSectorSz = EFRIO__VGRunDataFile.PubFGetDiskSectorSz ();    
    
    VRet = EFRIO__VGRunDataFile.PubFSeqWrite ( VPtRunCont->PtFrame, VPtRunCont->ResAcqFunctRetCode /* Acq sz */, VPtRunCont->ResEventCnt - 1 /* DbgCallPar */, 1 /* SpareW32InfoFormat */, (SInt32*) &VSpareW32Info, sizeof (VSpareW32Info) / 4 /* SpareW32InfoNb */ );
    
  }

  err_retfail ( VRet, (ERR_OUT,"Saving Acq=%d of %d bytes on file %s failed", VPtRunCont->ResAcqCnt - 1, VPtRunCont->ResAcqFunctRetCode, VPtRunCont->InfDataFileName ) );
  
  return (0);
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : EFRIO__F
:
Goal      :
:
Inputs    : None
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    :
:
Level     :
Date      : 08/11/2010
Rev       : 15/02/2011
          : - Handle InfSaveDataOnDiskRunning
Doc date  : /2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FStopSavingOnFile () {
  
  SInt32             VRet;
  EFRIO__TRunCont*   VPtRunCont = &EFRIO__VGContext.RunCont;

  VRet = 0;

  if ( VPtRunCont->ParDataTransferMode == EFRIO__TRF_MODE_IPHC ) {
    err_retfail ( -1, (ERR_OUT,"Abort => EFRIO__TRF_MODE_IPHC not hanlded") );
  }

  if ( VPtRunCont->ParSaveOnDisk > 0 ) {
    
    VPtRunCont->InfSaveDataOnDiskRunning = 0;
    
    VRet = VRet | EFRIO__VGRunDataFile.PubFFlush ();
    VRet = VRet | EFRIO__VGRunDataFile.PubFClose ();
    
    err_retfail ( VRet, (ERR_OUT,"Error while closing data file = %s", VPtRunCont->InfDataFileName) );
  }

  err_retok (( ERR_OUT, "" ));
}


#undef COMPIL_FTluTrigger2Str
#ifdef COMPIL_FTluTrigger2Str

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : char* EFRIO__FTluTrigger2Str ( UInt32 Trig, char* DestStr, SInt32 MaxDestSz )
          :
Goal      : Convert TLU trigger info record to string for print or display
          :
Inputs    : Trig      - Source trigger record ( it's only a W32 )
          : DestStr   - Destination string
          : MaxDestSz - Destination string size
          :
Ouputs    : The trigger as a string in an human readable format
          :
Globals   :
          :
Remark    : If DestStr = NULL or is too small, a pointer to static local variable is
          : returned. But please do a copy of the string, because if you use via the
          : pointer, string content may / will change at next function call !
          :
Level     :
Date      : 06/11/2010
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

char* EFRIO__FTluTrigger2Str ( UInt32 Trig, char* DestStr, SInt32 MaxDestSz )
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
Prototype : char* EFRIO__FTimeStamp2Str ( UInt32 Ts, char* DestStr, SInt32 MaxDestSz )
          :
Goal      : Convert Flex RIO trigger / ime stamp info record to string for print or display
          :
Inputs    : Ts        - Source time stamp record ( it's only a W32 )
          : DestStr   - Destination string
          : MaxDestSz - Destination string size
          :
Ouputs    : The trigger / timestamp as a string in an human readable format
          :
Globals   :
          :
Remark    : If DestStr = NULL or is too small, a pointer to static local variable is
          : returned. But please do a copy of the string, because if you use via the
          : pointer, string content may / will change at next function call !
          :
Level     :
Date      : 06/11/2010
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

char* EFRIO__FTimeStamp2Str ( UInt32 Ts, char* DestStr, SInt32 MaxDestSz )
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


#endif

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__MI26_FChkAcqIphcMode ( SInt32 PrevErrCnt, SInt8 Verbose )
          :
Goal      : Check all frames of current acquisition in IPHC data transfer mode.
          :
          : Test fields like Mimosa 26 header, frame counter, trailer.
          : Compare to values set in AcqEmul record of lib context.
          :
Inputs    : PrevErrCnt - Global error counter
          : Verbose    - Print errors in log file -> value read & expected
          :
Ouputs    : The function returns PrevErrCnt + error count during this function call
          :
Globals   :
          :
Remark    : For Mi26 frames counter testing, it takes the first frame of acquisition as
          : a starting point and check that counter increases frame by frame or one unit.
          :
Level     :
Date      : 31/10/2010
Doc date  : 07/11/2010 
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__MI26_FChkAcqIphcMode ( SInt32 PrevErrCnt, SInt8 Verbose ) {
   
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*  VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];
  EFRIO__TAcqEmul*    VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
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
Prototype : SInt32 EFRIO__MI26_FChkAcqEudetMode ( SInt32 PrevErrCnt, SInt8 Verbose )
          :
Goal      : Check all frames of current acquisition in EUDET 1,2,3 data transfer modes.
          :
          : Test fields like Mimosa 26 header, frame counter, trailer.
          : Compare to values set in AcqEmul record of lib context.
          :  
Inputs    : PrevErrCnt - Global error counter
          : Verbose    - Print errors in log file -> value read & expected
          :
Ouputs    : The function returns PrevErrCnt + error count during this function call
          :
Globals   :
          :
Remark    : For Mi26 frames counter testing, it takes the first frame of acquisition as
          : a starting point and check that counter increases frame by frame or one unit.
          :
Level     :
Date      : 31/10/2010
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


UInt32 EFRIO__MI26_FChkAcqEudetMode ( SInt32 PrevErrCnt, SInt8 Verbose ) {
  
  
  EFRIO__TRunCont*    VPtRunCont = &EFRIO__VGContext.RunCont;
  EFRIO__TFrameList*  VPtFrList  = &EFRIO__VGContext.AAcqFrameList[0];
  EFRIO__TAcqEmul*    VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
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
Date      : 29/04/2011 ( DPXI version of 20/08/2009 moved here )
Doc date  : 20/08/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__MI26_FChkFrameLight ( SInt16 FuncId, SInt32 CurFrame, EFRIO__TFrame* PtFrame, SInt8 Mi26Nb ) {


  SInt32                  VRet;
  EFRIO__TRunCont*        VPtRunCont  = &EFRIO__VGContext.RunCont;
  EFRIO__TTestOnDataCont* VPtTestCont = &EFRIO__VGContext.TestOnDataCont;
   
  SInt8  VMi26Nb;
  SInt8  ViMi26;
  static SInt8  VErrors;
  EFRIO__TFrameHeader* VPtFrHd;
  
  
  
  // -----------------------------------------------
  // Test disabled => Exit
  // -----------------------------------------------
  
  if ( VPtTestCont->ParModeEnable == 0 ) {
    return (0);
  }
  
  
  // -----------------------------------------------
  // Perform test
  // -----------------------------------------------
    
  // Init ptr
  
  VPtFrHd = &PtFrame->Header;
  
  // Check MAPS nb to test
  
  if ( Mi26Nb > VPtTestCont->ParMapsNbToTest ) {
    VMi26Nb = VPtTestCont->ParMapsNbToTest;
  }
  
  else {
    VMi26Nb = Mi26Nb;
  }
    
  // Debug tool  
    
  if ( VPtTestCont->ParPrintLvl == -1 ) {
    msg (( MSG_OUT, "Nb Mi26 to test : %d", VMi26Nb ));
  }


  // Init frame cnt array on first frame values and errors flag
  
  if ( CurFrame == 0 ) {
    
    VErrors = 0;
    
    VPtTestCont->InfMapsFrameCntRef = VPtFrHd->AMapsFrameCnt[0]; // Because CurFrame = 0 in this bloc => PtFrame[0] is first Mi26
    
    for ( ViMi26=1; ViMi26 < VMi26Nb; ViMi26++ ) {
      
      if ( VPtFrHd->AMapsFrameCnt[ViMi26] != VPtTestCont->InfMapsFrameCntRef ) {
        
        ++VPtTestCont->ResAMapsFrameCntErrCnt[ViMi26];
        ++VPtTestCont->ResAMapsErrCnt[ViMi26];
        ++VPtTestCont->ResTotErrCnt;
        VErrors = 1;
        
        if ( VPtTestCont->ParPrintLvl == 1 ) {
          msg (( MSG_OUT, "A - Frame cnt error [Acq %.4d - Frame in Acq %.4d - Mi26 No %d] : Get %.6d - Must be %.6d ", VPtFrHd->AcqId, VPtFrHd->FrameIdInAcq, ViMi26, VPtFrHd->AMapsFrameCnt[ViMi26], VPtTestCont->InfMapsFrameCntRef ));
        }
        
      }
      
    }
    
  }
  
  else {
    ++VPtTestCont->InfMapsFrameCntRef;
  }
  
  // Check frame cnt & header & trailer
  
  for ( ViMi26=0; ViMi26 < VMi26Nb; ViMi26++ ) {
        
    // Check frame counter of CurFrame > 0
    
    if ( CurFrame != 0 ) {
      
      if ( VPtFrHd->AMapsFrameCnt[ViMi26] != VPtTestCont->InfMapsFrameCntRef ) {
        
        ++VPtTestCont->ResAMapsFrameCntErrCnt[ViMi26];
        ++VPtTestCont->ResAMapsErrCnt[ViMi26];
        ++VPtTestCont->ResTotErrCnt;
        VErrors = 1;
        
        if ( VPtTestCont->ParPrintLvl == 1 ) {
          msg (( MSG_OUT, "B - Frame cnt error [Acq %.4d - Frame in Acq %.4d - Mi26 No %d] : Get %.6d - Must be %.6d ", VPtFrHd->AcqId, VPtFrHd->FrameIdInAcq, ViMi26, VPtFrHd->AMapsFrameCnt[ViMi26], VPtTestCont->InfMapsFrameCntRef ));
        }
        
      }
      
    }
    
    // Check header and trailer
    
    if ( VPtFrHd->AMapsHeader[ViMi26] != VPtTestCont->ParAMapsHeaderRef[ViMi26] ) {
      
      ++VPtTestCont->ResAMapsHeaderErrCnt[ViMi26];
      ++VPtTestCont->ResAMapsErrCnt[ViMi26];
      ++VPtTestCont->ResTotErrCnt;
      VErrors = 1;
      
      if ( VPtTestCont->ParPrintLvl == 1 ) {
        msg (( MSG_OUT, "Header error [Acq %.4d - Frame in Acq %.4dx - Mi26 No %d] : Get %8x - Must be %.8x ", VPtFrHd->AcqId, VPtFrHd->FrameIdInAcq, ViMi26, VPtFrHd->AMapsHeader[ViMi26], VPtTestCont->ParAMapsHeaderRef[ViMi26] ));
      }
      
    }
    
    if ( VPtFrHd->AMapsTrailer[ViMi26] != VPtTestCont->ParAMapsTrailerRef[ViMi26] ) {
      
      ++VPtTestCont->ResAMapsTrailerErrCnt[ViMi26];
      ++VPtTestCont->ResAMapsErrCnt[ViMi26];
      ++VPtTestCont->ResTotErrCnt;
      VErrors = 1;
      
      if ( VPtTestCont->ParPrintLvl == 1 ) {
        msg (( MSG_OUT, "Trailer error [Acq %.4d - Frame in Acq %.4d - Mi26 No %d] : Get %8x - Must be %.8x ", VPtFrHd->AcqId, VPtFrHd->FrameIdInAcq, ViMi26, VPtFrHd->AMapsTrailer[ViMi26], VPtTestCont->ParAMapsTrailerRef[ViMi26] ));
      }
      
    }
    
  }
  
  
  // Update counter of frames with error(s)
  
  if ( VErrors ) {
    ++VPtTestCont->ResFrameNbWithErr;
  }
  
  // FuncId = 1 => Emulate errors
  // Code not UP TO DATE !!!!!!!!!!!!!!!!!!!!!!!!
  
  if ( FuncId == 1 ) {
    
    if ( (VPtFrHd->AcqId % 10) == 0 ) {
      msg (( MSG_OUT, "Error emulation on Acq=%d", VPtFrHd->AcqId ));
      VErrors = 1;
    }
    
  }
  
  
  return (VErrors);
}


#ifndef NO_MI26

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__MI26_FFRioAcqDeserDataIphcMode1Mi26 (
          :   SInt32 BoardId  , UInt32*  PtSrcW32, SInt32 EltNb,
          :    SInt32 AcqStatus, SInt32 TrigStatus,  SInt8 DataConvertMode )
          :
Goal      : Flex RIO readout function for one Mi26 in IPHC mode
          : 
          : Read data of one acquisition from Flex RIO, format them in IPHC mode
          : by adding extra information and fill PC RAM buffer.
          : 
          :
Inputs    : BoardId          - Board identifier
          : PtSrcW32         - Pointer to board data ( output of Labview Vi )
          : EltNb            - Board data size in W32 unit ( 1 Elt = 1 W32 )
          : AcqStatus        - Acquisition status flag provided by labview 
          : TrigStatus       - Trigger status flag provided by labview
          :
          : DataConvertMode  - Not used => Global EFRIO__TRunCont.ParDataTransferMode is used
          :
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
Date      : 02/02/2010
Rev       : 07/05/2010
          : - Trigger calculation
          : 19/05/2010
          : - Add trigger counter field handling in ASIC__TFrameStatus
          :
          : 23/02/2011
          : - Modify types of AcqStatus, TrigStatus parameters to SInt32
          :
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__MI26_FFRioAcqDeserDataIphcMode1Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode ) {
  
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
  

  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FOutD5 ( 0 /* Id */, 1 /* State */ );
  #endif
  
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
  

  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FOutD5 ( 0 /* Id */, 0 /* State */ );
  #endif
  
  err_retok (( ERR_OUT, "MsgOk" ));
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__MI26_FFRioAcqDeserDataIphcMode6Mi26 (
          :   SInt32 BoardId  , UInt32*  PtSrcW32, SInt32 EltNb,
          :    SInt32 AcqStatus, SInt32 TrigStatus,  SInt8 DataConvertMode )
          :
Goal      : Flex RIO readout function for six Mi26 in IPHC mode
          :
          : Read data of one acquisition from Flex RIO, format them in IPHC mode
          : by adding extra information and fill PC RAM buffer.
          :
          :
Inputs    : BoardId          - Board identifier
          : PtSrcW32         - Pointer to board data ( output of Labview Vi )
          : EltNb            - Board data size in W32 unit ( 1 Elt = 1 W32 )
          : AcqStatus        - Acquisition status flag provided by labview
          : TrigStatus       - Trigger status flag provided by labview
          :
          : DataConvertMode  - Not used => Global EFRIO__TRunCont.ParDataTransferMode is used
          :
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
Rev       : 06/05/2010
          : - Add trigger extract
          :   Warning ! Copy Zero & Zero2 fields of frame chip No 0 to ALL others chips !
          :             All fields Zero & Zero2 are equals to the fields of first chip
          : 19/05/2010
          : - Add trigger counter field handling in ASIC__TFrameStatus
          :
          : 23/02/2011
          : - Modify types of AcqStatus, TrigStatus parameters to SInt32
          :          
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

 
SInt32 EFRIO__MI26_FFRioAcqDeserDataIphcMode6Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode ) {
  
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
    
  
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FOutD5 ( 0 /* Id */, 1 /* State */ );
  #endif
  
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
  
  // Extract data
  
  // VRunFrameCnt = PtAcq->InfRunFrameCnt;
  
  ViSrcW32 = 0;
  
  for ( ViFrame=0; ViFrame < VPtBoard->FrameNbPerAcq; ViFrame++ ) {
        
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
            
    // Copy all data fields = We don't care about DataLength field
    // We can decide to optimize later, but NOW I want to get ALL board RAM
    
    VPtDataW32Chip0 = (UInt32*) VptZsFFrameRaw[V6iFrame].ADataW16;
    VPtDataW32Chip1 = (UInt32*) VptZsFFrameRaw[V6iFrameP1].ADataW16;
    VPtDataW32Chip2 = (UInt32*) VptZsFFrameRaw[V6iFrameP2].ADataW16;
    VPtDataW32Chip3 = (UInt32*) VptZsFFrameRaw[V6iFrameP3].ADataW16;
    VPtDataW32Chip4 = (UInt32*) VptZsFFrameRaw[V6iFrameP4].ADataW16;
    VPtDataW32Chip5 = (UInt32*) VptZsFFrameRaw[V6iFrameP5].ADataW16;

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
              
    VptZsFFrameRaw[V6iFrame].Trailer = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * VADataLengthW32[0])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    
    VZero = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 6];
    VptZsFFrameRaw[V6iFrame].Zero = VZero;
    ++ViSrcW32;
    
    VZero2 = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 12];
    VptZsFFrameRaw[V6iFrame].Zero2 = VZero2;
    ++ViSrcW32;
    
    // DPXI__MI26_FFRioExtractFirstTrigger ( TrigStatus, ViFrame, VLastFrameWithTrigAllowed, VZero, VZero2, &VTrigLine, &VTrigClk, &VTrigNb );

    
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
    
    
    ++VRunFrameCnt;
    
  } // End for ViFrame
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FOutD5 ( 0 /* Id */, 0 /* State */ );
  #endif
  
  err_retok (( ERR_OUT, "MsgOk" ));  
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet1Mode1Mi26 (
          :   SInt32 BoardId  , UInt32*  PtSrcW32, SInt32 EltNb,
          :   SInt32 AcqStatus, SInt32 TrigStatus,  SInt8 DataConvertMode )
          :
Goal      : Flex RIO readout function for one Mi26 in EUDET mode 1
          :
          : Read data of one acquisition from Flex RIO, format them in EUDET mode 1
          : by adding extra information and fill PC RAM buffer.
          :
          : The extra channel is not enabled in EUDET mode 1, therefore TLU trigger is
          : ignored. Only the first three triggers are stored by Flex RIO and coded in
          : "Mi26 format" = line index of Mimosa 26 read when trigger occurs.
          :
          :***************************************************************************
          : All frames are stored in memory, regardless of trigger state
          :***************************************************************************
          :
Inputs    : BoardId          - Board identifier
          : PtSrcW32         - Pointer to board data ( output of Labview Vi )
          : EltNb            - Board data size in W32 unit ( 1 Elt = 1 W32 )
          : AcqStatus        - Acquisition status flag provided by labview
          : TrigStatus       - Trigger status flag provided by labview
          :
          : DataConvertMode  - Not used => Global EFRIO__TRunCont.ParDataTransferMode is used
          :
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
          : 16/02/2011
          : - Update ResEventCnt = ResFrameCnt, set to 0 before
          :
          : 23/02/2011
          : - Modify types of AcqStatus, TrigStatus parameters to SInt32
          :
          : 24/02/2011
          : - Update new fields AcqStatus, TrigStatus of AAcqFrameList
          :          
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet1Mode1Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode ) {
  
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
  
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FOutD5 ( 0 /* Id */, 1 /* State */ );
  #endif
  
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
  
  VPtFrList->AcqStatus  = AcqStatus;
  VPtFrList->TrigStatus = TrigStatus;
  VPtFrList->TotFrameNb = 0;
  
  memset ( VPtFrList->AFramePtr, 0, EFRIO__MAX_FRAME_NB_PER_ACQ * sizeof (EFRIO__TFrame*) );
        
  // Calculate size of records WITHOUT variable length part counted here as one elt
  
  VEmptyTrigRecSz  = sizeof ( EFRIO__TTriggerRec );
  VEmptyFrameRecSz = sizeof ( EFRIO__TFrame );
    
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
      
    }
    
  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtFrame->Tag        = EFRIO__FRAME_TAG;
    VPtFrame->Header.Tag = EFRIO__FRAME_TAG_HEADER;
  #endif
  
    VPtFrame->Header.AcqStatus    = AcqStatus;
    VPtFrame->Header.TrigStatus   = TrigStatus;  
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
        
    for ( ViDataW32=0; ViDataW32 < VDataLengthW32; ViDataW32++ ) {
      VPtFrame->Data.ADataW32[ViDataW32] = PtSrcW32[ViSrcW32];
      ++ViSrcW32;
    }
    
    ViSrcW32 = ViSrcW32 + ( MI26__ZS_FFRAME_RAW_MAX_W32 - VDataLengthW32 );
    
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
        
    // Check frame relevant fields ( DataLength, Header, frame counter, trailer => NOT data part )
    // Enable / Disable done by a call to EFRIO__FTestOnDataStartStop ( Start, PrintLvl )
    
    EFRIO__MI26_FChkFrameLight ( 0 /* FuncId */ , ViFrame, VPtFrame, 1 /* Mi26Nb */ );
    
    
    ++VPtFrList->TotFrameNb;
    
    ++VRunFrameCnt;
    
  } // End for ViFrame
  
  ++VPtCont->RunCont.ResAcqCnt;
  VPtCont->RunCont.ResFrameCnt = VPtCont->RunCont.ResAcqCnt * VPtCont->RunCont.ParFrameNbPerAcq;
  VPtCont->RunCont.ResEventCnt = VPtCont->RunCont.ResFrameCnt;
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FOutD5 ( 0 /* Id */, 0 /* State */ );
  #endif
  
  return (VTotAcqSz);
}




/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet1Mode6Mi26 (
          :   SInt32 BoardId , UInt32*  PtSrcW32, SInt32 EltNb,
          :   SInt32 AcqStatus, SInt32 TrigStatus,  SInt8 DataConvertMode )
          :
Goal      : Flex RIO readout function for six Mi26 in EUDET mode 1
          :
          : Read data of one acquisition from Flex RIO, format them in EUDET mode 1
          : by adding extra information and fill PC RAM buffer.
          :
          : The extra channel is not enabled in EUDET mode 1, therefore TLU trigger is
          : ignored. Only the first three triggers are stored by Flex RIO and coded in
          : "Mi26 format" = line index of Mimosa 26 read when trigger occurs.
          :
          :***************************************************************************
          : All frames are stored in memory, regardless of trigger state
          :***************************************************************************
          :
Inputs    : BoardId          - Board identifier
          : PtSrcW32         - Pointer to board data ( output of Labview Vi )
          : EltNb            - Board data size in W32 unit ( 1 Elt = 1 W32 )
          : AcqStatus        - Acquisition status flag provided by labview
          : TrigStatus       - Trigger status flag provided by labview
          :
          : DataConvertMode  - Not used => Global EFRIO__TRunCont.ParDataTransferMode is used
          :
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
          : 16/02/2011
          : - Update ResEventCnt = ResFrameCnt, set to 0 before
          :
          : 23/02/2011
          : - Modify types of AcqStatus, TrigStatus parameters to SInt32
          :
          : 24/02/2011
          : - Update new fields AcqStatus, TrigStatus of AAcqFrameList
          :          
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet1Mode6Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode ) {
  
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
  
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FOutD5 ( 0 /* Id */, 1 /* State */ );
  #endif
  
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
  
  // Reset frame list pointers
  
  VPtFrList->AcqStatus  = AcqStatus;
  VPtFrList->TrigStatus = TrigStatus;  
  VPtFrList->TotFrameNb = 0;
  
  memset ( VPtFrList->AFramePtr, 0, EFRIO__MAX_FRAME_NB_PER_ACQ * sizeof (EFRIO__TFrame*) );
    
  // Calculate size of records WITHOUT variable length part counted here as one elt
  
  VEmptyTrigRecSz  = sizeof ( EFRIO__TTriggerRec );
  VEmptyFrameRecSz = sizeof ( EFRIO__TFrame );
  
  
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
      
    }

  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtFrame->Tag        = EFRIO__FRAME_TAG;
    VPtFrame->Header.Tag = EFRIO__FRAME_TAG_HEADER;
  #endif
  

    VPtFrame->Header.AcqStatus    = AcqStatus;
    VPtFrame->Header.TrigStatus   = TrigStatus;
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

    // If length > max possible => Set it to 0
    
    if ( VDataLengthW16Max > 2304 ) {

      // 02/03/2011 Replace hard coded arrays size in memset by sizeof ()
  
      memset ( VADataLengthW8 , 0, 6 * sizeof (VADataLengthW8[0] ) );
      memset ( VADataLengthW16, 0, 6 * sizeof (VADataLengthW16[0]) );
      memset ( VADataLengthW32, 0, 6 * sizeof (VADataLengthW32[0]) );
      
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
    
    // Check frame relevant fields ( DataLength, Header, frame counter, trailer => NOT data part )
    // Enable / Disable done by a call to EFRIO__FTestOnDataStartStop ( Start, PrintLvl )
    
    EFRIO__MI26_FChkFrameLight ( 0 /* FuncId */ , ViFrame, VPtFrame, 6 /* Mi26Nb */ );
    
    
    ++VPtFrList->TotFrameNb;
    
    ++VRunFrameCnt;
    
  } // End for ViFrame
  
  ++VPtCont->RunCont.ResAcqCnt;
  VPtCont->RunCont.ResFrameCnt = VPtCont->RunCont.ResAcqCnt * VPtCont->RunCont.ParFrameNbPerAcq;
  VPtCont->RunCont.ResEventCnt = VPtCont->RunCont.ResFrameCnt;
  
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FOutD5 ( 0 /* Id */, 0 /* State */ );
  #endif
  
  return (VTotAcqSz);
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet2Mode1Mi26 (
          :   SInt32 BoardId , UInt32*  PtSrcW32, SInt32 EltNb,
          :   SInt32 AcqStatus, SInt32 TrigStatus,  SInt8 DataConvertMode )
          :
Goal      : Flex RIO readout function for one Mi26 in EUDET mode 2
          :
          : Read data of one acquisition from Flex RIO, format them in EUDET mode 2
          : by adding extra information and fill PC RAM buffer.
          :
          : The extra channel is enabled in EUDET mode 2, therefore for each trigger
          : two informations are stored
          :
          : - The TLU trigger                    -> see record EFRIO__TTluTrigger
          : - The Flex RIO trigger / time stamp  -> see record EFRIO__TFlexRioTimeStamp1
          :
          :***************************************************************************
          : All frames are stored in memory, regardless of trigger state
          :***************************************************************************
          :
Inputs    : BoardId             - Board identifier
          : PtSrcW32            - Pointer to board data ( output of Labview Vi )
          : EltNb               - Board data size in W32 unit ( 1 Elt = 1 W32 )
          : AcqStatus           - Acquisition status flag provided by labview
          : TrigStatus          - Trigger status flag provided by labview
          :
          : DataConvertMode     - Not used => Global EFRIO__TRunCont.ParDataTransferMode is used
          :
          : TriggerHandlingMode - Reserved for future use
          :
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
          : 16/02/2011
          : - Update ResEventCnt = ResFrameCnt, set to 0 before
          :
          : 23/02/2011
          : - Modify types of AcqStatus, TrigStatus parameters to SInt32
          :
          : 24/02/2011
          : - Update new fields AcqStatus, TrigStatus of AAcqFrameList
          :
          : 12/03/2011
          : - Add parameter TriggerHandlingMode
          :
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet2Mode1Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode ) {
  
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
  
   
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FOutD5 ( 0 /* Id */, 1 /* State */ );
  #endif
  
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
  
  VPtFrList->AcqStatus  = AcqStatus;
  VPtFrList->TrigStatus = TrigStatus;  
  VPtFrList->TotFrameNb = 0;
  
  memset ( VPtFrList->AFramePtr, 0, EFRIO__MAX_FRAME_NB_PER_ACQ * sizeof (EFRIO__TFrame*) );
  
  // Calculate size of records WITHOUT variable length part counted here as one elt
  
  VEmptyTrigRecSz  = sizeof ( EFRIO__TTriggerRec );
  VEmptyFrameRecSz = sizeof ( EFRIO__TFrame );
  
  
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
      
    }
    
  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtFrame->Tag        = EFRIO__FRAME_TAG;
    VPtFrame->Header.Tag                = EFRIO__FRAME_TAG_HEADER;
  #endif
  
    VPtFrame->Header.AcqStatus          = AcqStatus;
    VPtFrame->Header.TrigStatus         = TrigStatus;
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

    if ( VTrigNb > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ) {
      err_error (( ERR_OUT, "VTrigNb=%d > Max=%d => Limit to max !", VTrigNb, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ));
      VTrigNb = EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26;
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
    
    // Check frame relevant fields ( DataLength, Header, frame counter, trailer => NOT data part )
    // Enable / Disable done by a call to EFRIO__FTestOnDataStartStop ( Start, PrintLvl )
    
    EFRIO__MI26_FChkFrameLight ( 0 /* FuncId */ , ViFrame, VPtFrame, 1 /* Mi26Nb */ );
    
    
    ++VPtFrList->TotFrameNb;
    
    ++VRunFrameCnt;
    
  } // End for ViFrame


  ++VPtCont->RunCont.ResAcqCnt;
  VPtCont->RunCont.ResFrameCnt = VPtCont->RunCont.ResAcqCnt * VPtCont->RunCont.ParFrameNbPerAcq;
  VPtCont->RunCont.ResEventCnt = VPtCont->RunCont.ResFrameCnt;
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FOutD5 ( 0 /* Id */, 0 /* State */ );
  #endif
  
  return (VTotAcqSz);
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet2Mode6Mi26 (
          :   SInt32 BoardId , UInt32*  PtSrcW32, SInt32 EltNb,
          :   SInt32 AcqStatus, SInt32 TrigStatus,  SInt8 DataConvertMode )
          :
Goal      : Flex RIO readout function for six Mi26 in EUDET mode 2
          :
          : Read data of one acquisition from Flex RIO, format them in EUDET mode 2
          : by adding extra information and fill PC RAM buffer.
          :
          : The extra channel is enabled in EUDET mode 2, therefore for each trigger
          : two informations are stored
          :
          : - The TLU trigger                    -> see record EFRIO__TTluTrigger
          : - The Flex RIO trigger / time stamp  -> see record EFRIO__TFlexRioTimeStamp1
          :
          :***************************************************************************
          : All frames are stored in memory, regardless of trigger state
          :***************************************************************************
          :
Inputs    : BoardId             - Board identifier
          : PtSrcW32            - Pointer to board data ( output of Labview Vi )
          : EltNb               - Board data size in W32 unit ( 1 Elt = 1 W32 )
          : AcqStatus           - Acquisition status flag provided by labview
          : TrigStatus          - Trigger status flag provided by labview
          :
          : DataConvertMode     - Not used => Global EFRIO__TRunCont.ParDataTransferMode is used
          :
          : TriggerHandlingMode - Reserved for future use
          :
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
          : 16/02/2011
          : - Update ResEventCnt = ResFrameCnt, set to 0 before
          :
Rev       : 21/02/2011
          : - Add demux of data part if EFRIO__DEMUX_MI26_DATA_PART defined in eudet_frio.def
          :
          : 23/02/2011
          : - Modify types of AcqStatus, TrigStatus parameters to SInt32
          :
          : 24/02/2011
          : - Update new fields AcqStatus, TrigStatus of AAcqFrameList
          :
          : 12/03/2011
          : - Add parameter TriggerHandlingMode          
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet2Mode6Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode ) {
  
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
  UInt32*              VPtCpySrcW32;
  UInt32*              VAPtCpyDestW32[6];  
  UInt64*              VPtCpySrcW64;
  UInt64*              VPtCpyDestW64;
  UInt32*              VPtEChanSrcW32;
  SInt16               VEChanTrigFieldCnt; // Extra channel trigger fields counter
  UInt32               VEChanTrigField;
  SInt32               VTotAcqSz;
  SInt8                ViMi26ChkDataLength;

  
  #ifdef EFRIO__INCLUDE_PARA_PORT
     PPO_FOutD5 ( 0 /* Id */, 1 /* State */ );
  #endif
  
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

  VPtFrList->AcqStatus  = AcqStatus;
  VPtFrList->TrigStatus = TrigStatus;
  VPtFrList->TotFrameNb = 0;
  
  memset ( VPtFrList->AFramePtr, 0, EFRIO__MAX_FRAME_NB_PER_ACQ * sizeof (EFRIO__TFrame*) );

  // Reset mon size

  VPtCont->MonCont.InfSzToSend = 0;
  
  // Calculate size of records WITHOUT variable length part counted here as one elt
  
  VEmptyTrigRecSz  = sizeof ( EFRIO__TTriggerRec );
  VEmptyFrameRecSz = sizeof ( EFRIO__TFrame );
  

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
      
    }

  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtFrame->Tag        = EFRIO__FRAME_TAG;
    VPtFrame->Header.Tag          = EFRIO__FRAME_TAG_HEADER;
  #endif
  

    VPtFrame->Header.AcqStatus    = AcqStatus;
    VPtFrame->Header.TrigStatus   = TrigStatus;
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

      ++VDataLengthErrCnt;
      err_error (( ERR_OUT, "---------------------------------------------------------------------" ));
      err_error (( ERR_OUT, "HW error on data length -> Max W16 nb = %d -> Force 0 !", VDataLengthW16Max ));
      
      for ( ViMi26ChkDataLength = 0; ViMi26ChkDataLength < 6; ViMi26ChkDataLength++ ) {
        if ( VADataLengthW16[ViMi26ChkDataLength] > 2304 ) {
          err_error (( ERR_OUT, "Error chip[%d] = %d W16", ViMi26ChkDataLength, VADataLengthW16[ViMi26ChkDataLength] ));
        }
      }
      
      err_error (( ERR_OUT, "---------------------------------------------------------------------" ));
      
      // 02/03/2011 Replace hard coded arrays size in memset by sizeof ()
      
      memset ( VADataLengthW8 , 0, 6 * sizeof (VADataLengthW8[0] ) );
      memset ( VADataLengthW16, 0, 6 * sizeof (VADataLengthW16[0]) );
      memset ( VADataLengthW32, 0, 6 * sizeof (VADataLengthW32[0]) );

      VDataLengthW8Max  = 0;
      VDataLengthW32Max = 0;
      
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
    
  #ifndef EFRIO__DEMUX_MI26_DATA_PART
  
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


  #else

    VPtCpySrcW32      = (UInt32*) &PtSrcW32[ViSrcW32];
    VAPtCpyDestW32[0] = (UInt32*) VPtFrame->Data.ADataW32;
    VAPtCpyDestW32[1] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 +       VDataLengthW32Max   );
    VAPtCpyDestW32[2] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 2 * VDataLengthW32Max ) );
    VAPtCpyDestW32[3] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 3 * VDataLengthW32Max ) );
    VAPtCpyDestW32[4] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 4 * VDataLengthW32Max ) );
    VAPtCpyDestW32[5] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 5 * VDataLengthW32Max ) );
    
    for ( ViDataCpy=0; ViDataCpy < VDataLengthW32Max; ViDataCpy++ ) {
      
      *VAPtCpyDestW32[0] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[0];
      ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[1] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[1];
      ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[2] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[2];
      ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[3] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[3];
      ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[4] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[4];
      ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[5] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[5];
      ++VPtCpySrcW32;
      
      
      VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = *VPtCpySrcW32;
      ++VPtCpySrcW32;
      ++VEChanTrigFieldCnt;
    }
    
    VPtEChanSrcW32 = (UInt32*) VPtCpySrcW32;
    
  #endif
  
  
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

    if ( VTrigNb > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ) {
      err_warning (( ERR_OUT, "VTrigNb=%d > Max=%d => Limit to max !", VTrigNb, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ));
      VTrigNb = EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26;
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
    
    // Check frame relevant fields ( DataLength, Header, frame counter, trailer => NOT data part )
    // Enable / Disable done by a call to EFRIO__FTestOnDataStartStop ( Start, PrintLvl )
    
    EFRIO__MI26_FChkFrameLight ( 0 /* FuncId */ , ViFrame, VPtFrame, 6 /* Mi26Nb */ );
    
    ++VPtFrList->TotFrameNb;
    
    ++VRunFrameCnt;
    
    // Update size of acquisition to send on Ethernet for monitoring
    // 11/03/2011
    //
    // The update is done after processing each frame
    // -> it cost execution time
    // -> but we are sure that update has been done
    
    if ( ViFrame < VPtCont->MonCont.InfFrameNbToSend ) {
      VPtCont->MonCont.InfSzToSend = VTotAcqSz;
    }
    
    
  } // End for ViFrame
  
  ++VPtCont->RunCont.ResAcqCnt;
  VPtCont->RunCont.ResFrameCnt = VPtCont->RunCont.ResAcqCnt * VPtCont->RunCont.ParFrameNbPerAcq;
  VPtCont->RunCont.ResEventCnt = VPtCont->RunCont.ResFrameCnt;
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
     PPO_FOutD5 ( 0 /* Id */, 0 /* State */ );
  #endif
  
  return (VTotAcqSz);
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet2Mode8Mi26 (
          :   SInt32 BoardId , UInt32*  PtSrcW32, SInt32 EltNb,
          :   SInt32 AcqStatus, SInt32 TrigStatus,  SInt8 DataConvertMode )
          :
Goal      : Flex RIO readout function for six Mi26 in EUDET mode 2
          :
          : Read data of one acquisition from Flex RIO, format them in EUDET mode 2
          : by adding extra information and fill PC RAM buffer.
          :
          : The extra channel is enabled in EUDET mode 2, therefore for each trigger
          : two informations are stored
          :
          : - The TLU trigger                    -> see record EFRIO__TTluTrigger
          : - The Flex RIO trigger / time stamp  -> see record EFRIO__TFlexRioTimeStamp1
          :
          :***************************************************************************
          : All frames are stored in memory, regardless of trigger state
          :***************************************************************************
          :
Inputs    : BoardId             - Board identifier
          : PtSrcW32            - Pointer to board data ( output of Labview Vi )
          : EltNb               - Board data size in W32 unit ( 1 Elt = 1 W32 )
          : AcqStatus           - Acquisition status flag provided by labview
          : TrigStatus          - Trigger status flag provided by labview
          :
          : DataConvertMode     - Not used => Global EFRIO__TRunCont.ParDataTransferMode is used
          :
          : TriggerHandlingMode - Reserved for future use
          :
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
Date      : 28/04/2011 ( Upgrade to 8 Mi26 from 29/10/2010 version handling 6 Mi26 )
Rev       :
          : 16/02/2011
          : - Update ResEventCnt = ResFrameCnt, set to 0 before
          :
Rev       : 21/02/2011
          : - Add demux of data part if EFRIO__DEMUX_MI26_DATA_PART defined in eudet_frio.def
          :
          : 23/02/2011
          : - Modify types of AcqStatus, TrigStatus parameters to SInt32
          :
          : 24/02/2011
          : - Update new fields AcqStatus, TrigStatus of AAcqFrameList
          :
          : 12/03/2011
          : - Add parameter TriggerHandlingMode
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet2Mode8Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode ) {
  
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
  SInt32               V9iFrame;
  UInt32               VADataLengthField[8];
  UInt32               VADataLengthW8[8];
  UInt16               VADataLengthW16[8];
  UInt32               VADataLengthW32[8];
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
  UInt32*              VPtCpySrcW32;
  UInt32*              VAPtCpyDestW32[8];
  UInt64*              VPtCpySrcW64;
  UInt64*              VPtCpyDestW64;
  UInt32*              VPtEChanSrcW32;
  SInt16               VEChanTrigFieldCnt; // Extra channel trigger fields counter
  UInt32               VEChanTrigField;
  SInt32               VTotAcqSz;
  SInt8                ViMi26ChkDataLength;
  
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
  PPO_FOutD5 ( 0 /* Id */, 1 /* State */ );
  #endif
  
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
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ) / 9; // Divide by 9 because of extral channel
  
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
  
  VPtFrList->AcqStatus  = AcqStatus;
  VPtFrList->TrigStatus = TrigStatus;
  VPtFrList->TotFrameNb = 0;
  
  memset ( VPtFrList->AFramePtr, 0, EFRIO__MAX_FRAME_NB_PER_ACQ * sizeof (EFRIO__TFrame*) );
  
  // Reset mon size
  
  VPtCont->MonCont.InfSzToSend = 0;
  
  // Calculate size of records WITHOUT variable length part counted here as one elt
  
  VEmptyTrigRecSz  = sizeof ( EFRIO__TTriggerRec );
  VEmptyFrameRecSz = sizeof ( EFRIO__TFrame );
  
  
  // Reset tmp triggers buffer
  
  memset ( VPtTmpTrigRec, 0, sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );
  
  // Extract data
  
  ViSrcW32          = 0;
  VDataLengthErrCnt = 0;
  
  for ( ViFrame=0; ViFrame < VPtBoard->FrameNbPerAcq; ViFrame++ ) {
    
    V9iFrame = 9 * ViFrame;
    
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
      
    }
    
    #ifdef EFRIO__FRAME_TAGS_ENABLE
      VPtFrame->Tag        = EFRIO__FRAME_TAG;
      VPtFrame->Header.Tag          = EFRIO__FRAME_TAG_HEADER;
    #endif
    
    
    VPtFrame->Header.AcqStatus    = AcqStatus;
    VPtFrame->Header.TrigStatus   = TrigStatus;
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
    VPtFrame->Header.AMapsHeader[6]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsHeader[7]    = PtSrcW32[ViSrcW32];
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
    VPtFrame->Header.AMapsFrameCnt[6]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsFrameCnt[7]  = PtSrcW32[ViSrcW32];
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
    VADataLengthField[6]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[7]                = PtSrcW32[ViSrcW32];
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
    VADataLengthW16[6] = (VADataLengthField[6] & 0x0000FFFF) + ((VADataLengthField[6] & 0xFFFF0000) >> 16);
    VADataLengthW16[7] = (VADataLengthField[7] & 0x0000FFFF) + ((VADataLengthField[7] & 0xFFFF0000) >> 16);
    
    VDataLengthW16Max = MATH_FUInt16Max ( VADataLengthW16, 8 );
    
    
    // A better test ( on each VADataLengthW16[i] ? on each W16 on D0 & D1 ? ) should be done !
    
    if ( VDataLengthW16Max > 2304 ) {

      ++VDataLengthErrCnt;
      err_error (( ERR_OUT, "---------------------------------------------------------------------" ));
      err_error (( ERR_OUT, "HW error on data length -> Max W16 nb = %d -> Force 0 !", VDataLengthW16Max ));
      
      for ( ViMi26ChkDataLength = 0; ViMi26ChkDataLength < 8; ViMi26ChkDataLength++ ) {
        if ( VADataLengthW16[ViMi26ChkDataLength] > 2304 ) {
          err_error (( ERR_OUT, "Error chip[%d] = %d W16", ViMi26ChkDataLength, VADataLengthW16[ViMi26ChkDataLength] ));
        }
      }
      
      err_error (( ERR_OUT, "---------------------------------------------------------------------" ));
      
      // 02/03/2011 Replace hard coded arrays size in memset by sizeof ()
      
      memset ( VADataLengthW8 , 0, 8 * sizeof (VADataLengthW8[0] ) );
      memset ( VADataLengthW16, 0, 8 * sizeof (VADataLengthW16[0]) );
      memset ( VADataLengthW32, 0, 8 * sizeof (VADataLengthW32[0]) );
      
      VDataLengthW8Max  = 0;
      VDataLengthW32Max = 0;      
      
    }
    
    else {
      
      for ( ViMi26=0; ViMi26 < 8; ViMi26++ ) {
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
    VPtFrame->Header.AMapsDataLength[6] = VADataLengthW8[6];
    VPtFrame->Header.AMapsDataLength[7] = VADataLengthW8[7];
    
    // Copy useful data part
    
    VDataLengthW32ToCpy = VDataLengthW32Max * 8;
    VDataLengthW8ToCpy  = VDataLengthW32ToCpy * 4;
    
    #ifdef EFRIO__FRAME_TAGS_ENABLE
      VPtFrame->Data.Tag       = EFRIO__FRAME_TAG_DATA;
    #endif
    
    
    VPtFrame->Data.TotSz     = VDataLengthW8ToCpy;
    VPtFrame->Data.OneMapsSz = VDataLengthW8Max;
    
    #ifndef EFRIO__DEMUX_MI26_DATA_PART
    
      // WARNING 06/05/2011 => This part has been updated BUT not tested
    
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
        *VPtCpyDestW64 = *VPtCpySrcW64;
        ++VPtCpyDestW64;
        ++VPtCpySrcW64;
        
        VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = *((UInt32*) VPtCpySrcW64);
        (UInt32) VPtCpySrcW64 = (UInt32) (VPtCpySrcW64) + 4;
        ++VEChanTrigFieldCnt;
      }
      
      VPtEChanSrcW32 = (UInt32*) VPtCpySrcW64;
    
    #else
    
      VPtCpySrcW32      = (UInt32*) &PtSrcW32[ViSrcW32];
      VAPtCpyDestW32[0] = (UInt32*) VPtFrame->Data.ADataW32;
      VAPtCpyDestW32[1] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 +       VDataLengthW32Max   );
      VAPtCpyDestW32[2] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 2 * VDataLengthW32Max ) );
      VAPtCpyDestW32[3] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 3 * VDataLengthW32Max ) );
      VAPtCpyDestW32[4] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 4 * VDataLengthW32Max ) );
      VAPtCpyDestW32[5] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 5 * VDataLengthW32Max ) );
      VAPtCpyDestW32[6] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 6 * VDataLengthW32Max ) );
      VAPtCpyDestW32[7] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 7 * VDataLengthW32Max ) );
      
      for ( ViDataCpy=0; ViDataCpy < VDataLengthW32Max; ViDataCpy++ ) {
        
        *VAPtCpyDestW32[0] = *VPtCpySrcW32;
        ++VAPtCpyDestW32[0];
        ++VPtCpySrcW32;
        
        *VAPtCpyDestW32[1] = *VPtCpySrcW32;
        ++VAPtCpyDestW32[1];
        ++VPtCpySrcW32;
        
        *VAPtCpyDestW32[2] = *VPtCpySrcW32;
        ++VAPtCpyDestW32[2];
        ++VPtCpySrcW32;
        
        *VAPtCpyDestW32[3] = *VPtCpySrcW32;
        ++VAPtCpyDestW32[3];
        ++VPtCpySrcW32;
        
        *VAPtCpyDestW32[4] = *VPtCpySrcW32;
        ++VAPtCpyDestW32[4];
        ++VPtCpySrcW32;
        
        *VAPtCpyDestW32[5] = *VPtCpySrcW32;
        ++VAPtCpyDestW32[5];
        ++VPtCpySrcW32;
        
        *VAPtCpyDestW32[6] = *VPtCpySrcW32;
        ++VAPtCpyDestW32[6];
        ++VPtCpySrcW32;
        
        *VAPtCpyDestW32[7] = *VPtCpySrcW32;
        ++VAPtCpyDestW32[7];
        ++VPtCpySrcW32;
        
        VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = *VPtCpySrcW32;
        ++VPtCpySrcW32;
        ++VEChanTrigFieldCnt;
      }
      
      VPtEChanSrcW32 = (UInt32*) VPtCpySrcW32;
      
    #endif
    
    
    // Continue extraction of trigger fields from trigger channel WHILE needed
    
    VPtEChanSrcW32 += 8; // Bypass Mi26 x 8 data
    
    do {
      VEChanTrigField = *VPtEChanSrcW32;
      
      if ( VEChanTrigFieldCnt >= EFRIO__EXTRA_CHAN__MAX_TRIGGER_FIELD_NB ) {
        err_warning (( ERR_OUT, "Max trigger field nb reached = %d <=> %d max trigger info !", EFRIO__EXTRA_CHAN__MAX_TRIGGER_FIELD_NB, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ));
        break;
      }
      
      VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = VEChanTrigField;
      VPtEChanSrcW32 += 9;
      ++VEChanTrigFieldCnt;
    } while ( (VEChanTrigField & 0x80000000) == 0 );
    
    
    ViSrcW32 += (9 * MI26__ZS_FFRAME_RAW_MAX_W32);
    
    
    //    VptZsFFrameRaw[V6iFrame].Trailer = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * VADataLengthW32[0])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    //    ++ViSrcW32;
    
    //    VZero = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 6];
    //    VptZsFFrameRaw[V6iFrame].Zero = VZero;
    //    ++ViSrcW32;
    
    //    VZero2 = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 12];
    //    VptZsFFrameRaw[V6iFrame].Zero2 = VZero2;
    //    ++ViSrcW32;
    
    
    VPtFrame->Header.AMapsTrailer[0] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V9iFrame) + 27 + (9 * VADataLengthW32[0])];    // 27 = 9 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[1] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V9iFrame) + 27 + 1 + (9 * VADataLengthW32[1])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[2] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V9iFrame) + 27 + 2 + (9 * VADataLengthW32[2])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[3] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V9iFrame) + 27 + 3 + (9 * VADataLengthW32[3])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[4] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V9iFrame) + 27 + 4 + (9 * VADataLengthW32[4])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[5] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V9iFrame) + 27 + 5 + (9 * VADataLengthW32[5])];
    ++ViSrcW32;

    VPtFrame->Header.AMapsTrailer[6] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V9iFrame) + 27 + 6 + (9 * VADataLengthW32[6])];
    ++ViSrcW32;

    VPtFrame->Header.AMapsTrailer[7] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V9iFrame) + 27 + 7 + (9 * VADataLengthW32[7])];
    ++ViSrcW32;
    
    ++ViSrcW32; // To count extra channel
    
    
    // Read trigger number from "Mi26" frame
    
    if ( TrigStatus == 0 ) {
      VZero  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V9iFrame) + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) +  9]; // 9 = 9 x 1 Trailer
    }
    
    // Emulate trigger by overwriting info from "Mi26" frame
    
    else {
      VZero = (TrigStatus << 16);
    }
    
    // VZero  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) +  9];
                
    VZero2 = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V9iFrame) + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 18]; // 18 = 9 x ( 1 Trailer + 1 Zero )
    
    ViSrcW32 += 18; // 9 times 2 zero fields = 18
    
    // Extraction of trigger info from "Mimosa 26 frame" zero fields
    
    VTrigNb = (VZero  & 0xFFFF0000) >> 16;
    
    if ( VTrigNb > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ) {
      err_warning (( ERR_OUT, "VTrigNb=%d > Max=%d => Limit to max !", VTrigNb, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ));
      VTrigNb = EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26;
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
    
    
    // Check frame relevant fields ( DataLength, Header, frame counter, trailer => NOT data part )
    // Enable / Disable done by a call to EFRIO__FTestOnDataStartStop ( Start, PrintLvl )
    
    EFRIO__MI26_FChkFrameLight ( 0 /* FuncId */ , ViFrame, VPtFrame, 8 /* Mi26Nb */ );
    
    
    ++VPtFrList->TotFrameNb;
    
    ++VRunFrameCnt;
    
    // Update size of acquisition to send on Ethernet for monitoring
    // 11/03/2011
    //
    // The update is done after processing each frame
    // -> it cost execution time
    // -> but we are sure that update has been done
    
    if ( ViFrame < VPtCont->MonCont.InfFrameNbToSend ) {
      VPtCont->MonCont.InfSzToSend = VTotAcqSz;
    }
    
    
  } // End for ViFrame
  
  ++VPtCont->RunCont.ResAcqCnt;
  VPtCont->RunCont.ResFrameCnt = VPtCont->RunCont.ResAcqCnt * VPtCont->RunCont.ParFrameNbPerAcq;
  VPtCont->RunCont.ResEventCnt = VPtCont->RunCont.ResFrameCnt;
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FOutD5 ( 0 /* Id */, 0 /* State */ );
  #endif
  
  return (VTotAcqSz);
}

// Mode 2 - 12 x Mi26
// 23/11/2011
// $$

SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet2Mode12Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode ) {
  
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
  SInt32               V13iFrame;
  UInt32               VADataLengthField[12];
  UInt32               VADataLengthW8[12];
  UInt16               VADataLengthW16[12];
  UInt32               VADataLengthW32[12];
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
  UInt32*              VPtCpySrcW32;
  UInt32*              VAPtCpyDestW32[12];
  UInt64*              VPtCpySrcW64;
  UInt64*              VPtCpyDestW64;
  UInt32*              VPtEChanSrcW32;
  SInt16               VEChanTrigFieldCnt; // Extra channel trigger fields counter
  UInt32               VEChanTrigField;
  SInt32               VTotAcqSz;
  SInt8                ViMi26ChkDataLength;
  
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FOutD5 ( 0 /* Id */, 1 /* State */ );
  #endif
  
  if ( VPtCont->InfInitDone == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort because : Init not done") );
  }
  
  err_trace (( ERR_OUT, "EFRIO__MI26_FFRioAcqDeserDataEudet1Mode12Mi26 (P=%x, EltNb=%d)", PtSrcW32, EltNb ));
  
  // Pointers parameters check
  
  err_retnull ( PtSrcW32, (ERR_OUT,"PtSrcW32 = NULL") );
  
  // Init board conf record pointer
  
  if ( (BoardId < 0) || (BoardId >= EFRIO__MAX_BOARDS_NB) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Board Id=%d out of range [0..%d])", BoardId, EFRIO__MAX_BOARDS_NB - 1) );
  }
  
  VPtBoard = &EFRIO__VGContext.ABoardsConf[BoardId];
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ) / 13; // Divide by 13 because of extral channel
  
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
  
  VPtFrList->AcqStatus  = AcqStatus;
  VPtFrList->TrigStatus = TrigStatus;
  VPtFrList->TotFrameNb = 0;
  
  memset ( VPtFrList->AFramePtr, 0, EFRIO__MAX_FRAME_NB_PER_ACQ * sizeof (EFRIO__TFrame*) );
  
  // Reset mon size
  
  VPtCont->MonCont.InfSzToSend = 0;
  
  // Calculate size of records WITHOUT variable length part counted here as one elt
  
  VEmptyTrigRecSz  = sizeof ( EFRIO__TTriggerRec );
  VEmptyFrameRecSz = sizeof ( EFRIO__TFrame );
  
  
  // Reset tmp triggers buffer
  
  memset ( VPtTmpTrigRec, 0, sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );
  
  // Extract data
  
  ViSrcW32          = 0;
  VDataLengthErrCnt = 0;
  
  for ( ViFrame=0; ViFrame < VPtBoard->FrameNbPerAcq; ViFrame++ ) {
    
    V13iFrame = 13 * ViFrame;
    
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
      
    }
    
    #ifdef EFRIO__FRAME_TAGS_ENABLE
      VPtFrame->Tag        = EFRIO__FRAME_TAG;
      VPtFrame->Header.Tag = EFRIO__FRAME_TAG_HEADER;
    #endif
    
    
    VPtFrame->Header.AcqStatus    = AcqStatus;
    VPtFrame->Header.TrigStatus   = TrigStatus;
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
    VPtFrame->Header.AMapsHeader[6]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsHeader[7]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;   
    VPtFrame->Header.AMapsHeader[8]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsHeader[9]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsHeader[10]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsHeader[11]    = PtSrcW32[ViSrcW32];
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
    VPtFrame->Header.AMapsFrameCnt[6]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsFrameCnt[7]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32; 
    VPtFrame->Header.AMapsFrameCnt[8]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsFrameCnt[9]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsFrameCnt[10]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsFrameCnt[11]  = PtSrcW32[ViSrcW32];
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
    VADataLengthField[6]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[7]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;  
    VADataLengthField[8]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[9]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[10]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[11]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;   
    VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    ++VEChanTrigFieldCnt;
    
    
    VADataLengthW16[0]  = (VADataLengthField[0]  & 0x0000FFFF) + ((VADataLengthField[0]  & 0xFFFF0000) >> 16);
    VADataLengthW16[1]  = (VADataLengthField[1]  & 0x0000FFFF) + ((VADataLengthField[1]  & 0xFFFF0000) >> 16);
    VADataLengthW16[2]  = (VADataLengthField[2]  & 0x0000FFFF) + ((VADataLengthField[2]  & 0xFFFF0000) >> 16);
    VADataLengthW16[3]  = (VADataLengthField[3]  & 0x0000FFFF) + ((VADataLengthField[3]  & 0xFFFF0000) >> 16);
    VADataLengthW16[4]  = (VADataLengthField[4]  & 0x0000FFFF) + ((VADataLengthField[4]  & 0xFFFF0000) >> 16);
    VADataLengthW16[5]  = (VADataLengthField[5]  & 0x0000FFFF) + ((VADataLengthField[5]  & 0xFFFF0000) >> 16);
    VADataLengthW16[6]  = (VADataLengthField[6]  & 0x0000FFFF) + ((VADataLengthField[6]  & 0xFFFF0000) >> 16);
    VADataLengthW16[7]  = (VADataLengthField[7]  & 0x0000FFFF) + ((VADataLengthField[7]  & 0xFFFF0000) >> 16);
    VADataLengthW16[8]  = (VADataLengthField[8]  & 0x0000FFFF) + ((VADataLengthField[8]  & 0xFFFF0000) >> 16);
    VADataLengthW16[9]  = (VADataLengthField[9]  & 0x0000FFFF) + ((VADataLengthField[9]  & 0xFFFF0000) >> 16);
    VADataLengthW16[10] = (VADataLengthField[10] & 0x0000FFFF) + ((VADataLengthField[10] & 0xFFFF0000) >> 16);
    VADataLengthW16[11] = (VADataLengthField[11] & 0x0000FFFF) + ((VADataLengthField[11] & 0xFFFF0000) >> 16);
    
    VDataLengthW16Max = MATH_FUInt16Max ( VADataLengthW16, 12 );
    
    
    // A better test ( on each VADataLengthW16[i] ? on each W16 on D0 & D1 ? ) should be done !
    
    if ( VDataLengthW16Max > 2304 ) {
      
      ++VDataLengthErrCnt;
      err_error (( ERR_OUT, "---------------------------------------------------------------------" ));
      err_error (( ERR_OUT, "HW error on data length -> Max W16 nb = %d -> Force 0 !", VDataLengthW16Max ));
      
      for ( ViMi26ChkDataLength = 0; ViMi26ChkDataLength < 12; ViMi26ChkDataLength++ ) {
        if ( VADataLengthW16[ViMi26ChkDataLength] > 2304 ) {
          err_error (( ERR_OUT, "Error chip[%d] = %d W16", ViMi26ChkDataLength, VADataLengthW16[ViMi26ChkDataLength] ));
        }
      }
      
      err_error (( ERR_OUT, "---------------------------------------------------------------------" ));
      
      // 02/03/2011 Replace hard coded arrays size in memset by sizeof ()
      
      memset ( VADataLengthW8 , 0, 12 * sizeof (VADataLengthW8[0] ) );
      memset ( VADataLengthW16, 0, 12 * sizeof (VADataLengthW16[0]) );
      memset ( VADataLengthW32, 0, 12 * sizeof (VADataLengthW32[0]) );
      
      VDataLengthW8Max  = 0;
      VDataLengthW32Max = 0;
      
    }
    
    else {
      
      for ( ViMi26=0; ViMi26 < 12; ViMi26++ ) {
        VADataLengthW8[ViMi26]  = VADataLengthW16[ViMi26] * 2;
        VADataLengthW32[ViMi26] = VADataLengthW16[ViMi26] / 2;
      }
      
      VDataLengthW8Max  = VDataLengthW16Max * 2;
      VDataLengthW32Max = VDataLengthW16Max / 2;
    }
    
    VPtFrame->Header.AMapsDataLength[0]  = VADataLengthW8[0];
    VPtFrame->Header.AMapsDataLength[1]  = VADataLengthW8[1];
    VPtFrame->Header.AMapsDataLength[2]  = VADataLengthW8[2];
    VPtFrame->Header.AMapsDataLength[3]  = VADataLengthW8[3];
    VPtFrame->Header.AMapsDataLength[4]  = VADataLengthW8[4];
    VPtFrame->Header.AMapsDataLength[5]  = VADataLengthW8[5];
    VPtFrame->Header.AMapsDataLength[6]  = VADataLengthW8[6];
    VPtFrame->Header.AMapsDataLength[7]  = VADataLengthW8[7];
    VPtFrame->Header.AMapsDataLength[8]  = VADataLengthW8[8];
    VPtFrame->Header.AMapsDataLength[9]  = VADataLengthW8[9];
    VPtFrame->Header.AMapsDataLength[10] = VADataLengthW8[10];
    VPtFrame->Header.AMapsDataLength[11] = VADataLengthW8[11];
    
    // Copy useful data part
    
    VDataLengthW32ToCpy = VDataLengthW32Max * 12;
    VDataLengthW8ToCpy  = VDataLengthW32ToCpy * 4;
    
    #ifdef EFRIO__FRAME_TAGS_ENABLE
      VPtFrame->Data.Tag       = EFRIO__FRAME_TAG_DATA;
    #endif
    
    
    VPtFrame->Data.TotSz     = VDataLengthW8ToCpy;
    VPtFrame->Data.OneMapsSz = VDataLengthW8Max;
    
    #ifndef EFRIO__DEMUX_MI26_DATA_PART
    
    // WARNING 06/05/2011 => This part has been updated BUT not tested
    
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
      *VPtCpyDestW64 = *VPtCpySrcW64;
      ++VPtCpyDestW64;
      ++VPtCpySrcW64;
      
      VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = *((UInt32*) VPtCpySrcW64);
      (UInt32) VPtCpySrcW64 = (UInt32) (VPtCpySrcW64) + 4;
      ++VEChanTrigFieldCnt;
    }
    
    VPtEChanSrcW32 = (UInt32*) VPtCpySrcW64;
    
    #else
    
    VPtCpySrcW32       = (UInt32*) &PtSrcW32[ViSrcW32];
    VAPtCpyDestW32[0]  = (UInt32*) VPtFrame->Data.ADataW32;
    VAPtCpyDestW32[1]  = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 +       VDataLengthW32Max   );
    VAPtCpyDestW32[2]  = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + (  2 * VDataLengthW32Max ) );
    VAPtCpyDestW32[3]  = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + (  3 * VDataLengthW32Max ) );
    VAPtCpyDestW32[4]  = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + (  4 * VDataLengthW32Max ) );
    VAPtCpyDestW32[5]  = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + (  5 * VDataLengthW32Max ) );
    VAPtCpyDestW32[6]  = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + (  6 * VDataLengthW32Max ) );
    VAPtCpyDestW32[7]  = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + (  7 * VDataLengthW32Max ) );
    VAPtCpyDestW32[8]  = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + (  8 * VDataLengthW32Max ) );
    VAPtCpyDestW32[9]  = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + (  9 * VDataLengthW32Max ) );
    VAPtCpyDestW32[10] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 10 * VDataLengthW32Max ) );
    VAPtCpyDestW32[11] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 11 * VDataLengthW32Max ) );
    

    for ( ViDataCpy=0; ViDataCpy < VDataLengthW32Max; ViDataCpy++ ) {
      
      *VAPtCpyDestW32[0] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[0];
      ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[1] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[1];
      ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[2] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[2];
      ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[3] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[3];
      ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[4] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[4];
      ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[5] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[5];
      ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[6] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[6];
      ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[7] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[7];
      ++VPtCpySrcW32;
  
      *VAPtCpyDestW32[8] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[8];
      ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[9] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[9];
      ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[10] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[10];
      ++VPtCpySrcW32;

      *VAPtCpyDestW32[11] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[11];
      ++VPtCpySrcW32;
      

      VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = *VPtCpySrcW32;
      ++VPtCpySrcW32;
      ++VEChanTrigFieldCnt;
    }
    
    VPtEChanSrcW32 = (UInt32*) VPtCpySrcW32;
    
    #endif
    
    
    // Continue extraction of trigger fields from trigger channel WHILE needed
    
    VPtEChanSrcW32 += 12; // Bypass Mi26 x 12 data
    
    do {
      VEChanTrigField = *VPtEChanSrcW32;
      
      if ( VEChanTrigFieldCnt >= EFRIO__EXTRA_CHAN__MAX_TRIGGER_FIELD_NB ) {
        err_warning (( ERR_OUT, "Max trigger field nb reached = %d <=> %d max trigger info !", EFRIO__EXTRA_CHAN__MAX_TRIGGER_FIELD_NB, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ));
        break;
      }
      
      VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = VEChanTrigField;
      VPtEChanSrcW32 += 13;
      ++VEChanTrigFieldCnt;
    } while ( (VEChanTrigField & 0x80000000) == 0 );
    
    
    ViSrcW32 += (13 * MI26__ZS_FFRAME_RAW_MAX_W32);
    
    
    //    VptZsFFrameRaw[V6iFrame].Trailer = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * VADataLengthW32[0])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    //    ++ViSrcW32;
    
    //    VZero = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 6];
    //    VptZsFFrameRaw[V6iFrame].Zero = VZero;
    //    ++ViSrcW32;
    
    //    VZero2 = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 12];
    //    VptZsFFrameRaw[V6iFrame].Zero2 = VZero2;
    //    ++ViSrcW32;
    
    
    VPtFrame->Header.AMapsTrailer[0]  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V13iFrame) + 39 + (13 * VADataLengthW32[0])];    // 39 = 13 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[1]  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V13iFrame) + 39 +  1 + (13 * VADataLengthW32[1])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[2]  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V13iFrame) + 39 +  2 + (13 * VADataLengthW32[2])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[3]  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V13iFrame) + 39 +  3 + (13 * VADataLengthW32[3])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[4]  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V13iFrame) + 39 +  4 + (13 * VADataLengthW32[4])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[5]  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V13iFrame) + 39 +  5 + (13 * VADataLengthW32[5])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[6]  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V13iFrame) + 39 +  6 + (13 * VADataLengthW32[6])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[7]  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V13iFrame) + 39 +  7 + (13 * VADataLengthW32[7])];
    ++ViSrcW32;

    VPtFrame->Header.AMapsTrailer[8]  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V13iFrame) + 39 +  8 + (13 * VADataLengthW32[8])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[9]  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V13iFrame) + 39 +  9 + (13 * VADataLengthW32[9])];
    ++ViSrcW32;

    VPtFrame->Header.AMapsTrailer[10] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V13iFrame) + 39 + 10 + (13 * VADataLengthW32[10])];
    ++ViSrcW32;

    VPtFrame->Header.AMapsTrailer[11] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V13iFrame) + 39 + 11 + (13 * VADataLengthW32[11])];
    ++ViSrcW32;
    

    ++ViSrcW32; // To count extra channel
    
    
    // Read trigger number from "Mi26" frame
    
    if ( TrigStatus == 0 ) {
      VZero  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V13iFrame) + 39 + (13 * MI26__ZS_FFRAME_RAW_MAX_W32) +  13]; // 13 = 13 x 1 Trailer
    }
    
    // Emulate trigger by overwriting info from "Mi26" frame
    
    else {
      VZero = (TrigStatus << 16);
    }
    
    
    VZero2 = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V13iFrame) + 39 + (13 * MI26__ZS_FFRAME_RAW_MAX_W32) + 26]; // 26 = 13 x ( 1 Trailer + 1 Zero )
    
    ViSrcW32 += 26; // 13 times 2 zero fields = 26
    
    // Extraction of trigger info from "Mimosa 26 frame" zero fields
    
    VTrigNb = (VZero  & 0xFFFF0000) >> 16;
    
    if ( VTrigNb > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ) {
      err_warning (( ERR_OUT, "VTrigNb=%d > Max=%d => Limit to max !", VTrigNb, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ));
      VTrigNb = EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26;
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
    
    
    // Check frame relevant fields ( DataLength, Header, frame counter, trailer => NOT data part )
    // Enable / Disable done by a call to EFRIO__FTestOnDataStartStop ( Start, PrintLvl )
    
    EFRIO__MI26_FChkFrameLight ( 0 /* FuncId */ , ViFrame, VPtFrame, 12 /* Mi26Nb */ );
    
    
    ++VPtFrList->TotFrameNb;
    
    ++VRunFrameCnt;
    
    // Update size of acquisition to send on Ethernet for monitoring
    // 11/03/2011
    //
    // The update is done after processing each frame
    // -> it cost execution time
    // -> but we are sure that update has been done
    
    if ( ViFrame < VPtCont->MonCont.InfFrameNbToSend ) {
      VPtCont->MonCont.InfSzToSend = VTotAcqSz;
    }
    
    
  } // End for ViFrame
  
  ++VPtCont->RunCont.ResAcqCnt;
  VPtCont->RunCont.ResFrameCnt = VPtCont->RunCont.ResAcqCnt * VPtCont->RunCont.ParFrameNbPerAcq;
  VPtCont->RunCont.ResEventCnt = VPtCont->RunCont.ResFrameCnt;
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
  PPO_FOutD5 ( 0 /* Id */, 0 /* State */ );
  #endif
  
  return (VTotAcqSz);
}



// Mode 2 - N x Mi26
// 17/11/2011
// $$

SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet2ModeNMi26 ( SInt8 Mi26Nb, SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode ) {
  
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
  UInt32               VADataLengthField[16];
  UInt32               VADataLengthW8[16];
  UInt16               VADataLengthW16[16];
  UInt32               VADataLengthW32[16];
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
  UInt32*              VPtCpySrcW32;
  UInt32*              VAPtCpyDestW32[16];
  UInt64*              VPtCpySrcW64;
  UInt64*              VPtCpyDestW64;
  UInt32*              VPtEChanSrcW32;
  SInt16               VEChanTrigFieldCnt; // Extra channel trigger fields counter
  UInt32               VEChanTrigField;
  SInt32               VTotAcqSz;
  SInt8                ViMi26ChkDataLength;
  
  SInt8                VMi26NbP1;
  
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
  PPO_FOutD5 ( 0 /* Id */, 1 /* State */ );
  #endif
  
  if ( VPtCont->InfInitDone == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort because : Init not done") );
  }
  
  err_trace (( ERR_OUT, "EFRIO__MI26_FFRioAcqDeserDataEudet2ModeNMi26 (Mi26Nb=%d, P=%x, EltNb=%d)", Mi26Nb, PtSrcW32, EltNb ));  

  // Mi26 nb check
  
  if ( (Mi26Nb < 0) || (Mi26Nb > 16) ) {
    err_retfail ( -1, (ERR_OUT,"Bad Mi26 Nb = %d => Out of range [0..16]", Mi26Nb) );
  }
  
  VMi26NbP1 = Mi26Nb + 1;
  
  // Pointers parameters check
  
  err_retnull ( PtSrcW32, (ERR_OUT,"PtSrcW32 = NULL") );
  
  // Init board conf record pointer
  
  if ( (BoardId < 0) || (BoardId >= EFRIO__MAX_BOARDS_NB) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Board Id=%d out of range [0..%d])", BoardId, EFRIO__MAX_BOARDS_NB - 1) );
  }
  
  VPtBoard = &EFRIO__VGContext.ABoardsConf[BoardId];
  
  // Check data size

  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ) / VMi26NbP1; // Divide by 7 because of extral channel

  // VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ) / 7; // Divide by 7 because of extral channel
  
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
  
  VPtFrList->AcqStatus  = AcqStatus;
  VPtFrList->TrigStatus = TrigStatus;
  VPtFrList->TotFrameNb = 0;
  
  memset ( VPtFrList->AFramePtr, 0, EFRIO__MAX_FRAME_NB_PER_ACQ * sizeof (EFRIO__TFrame*) );
  
  // Reset mon size

  
  VPtCont->MonCont.InfSzToSend = 0;
  
  // Calculate size of records WITHOUT variable length part counted here as one elt
  
  VEmptyTrigRecSz  = sizeof ( EFRIO__TTriggerRec );
  VEmptyFrameRecSz = sizeof ( EFRIO__TFrame );
  
  
  // Reset tmp triggers buffer
  
  memset ( VPtTmpTrigRec, 0, sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );
  
  // Extract data
  
  ViSrcW32          = 0;
  VDataLengthErrCnt = 0;
  
  for ( ViFrame=0; ViFrame < VPtBoard->FrameNbPerAcq; ViFrame++ ) {
    
    V7iFrame = VMi26NbP1 * ViFrame;
    
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
      
    }
    
    #ifdef EFRIO__FRAME_TAGS_ENABLE
      VPtFrame->Tag        = EFRIO__FRAME_TAG;
      VPtFrame->Header.Tag = EFRIO__FRAME_TAG_HEADER;
    #endif
    
    
    VPtFrame->Header.AcqStatus    = AcqStatus;
    VPtFrame->Header.TrigStatus   = TrigStatus;
    VPtFrame->Header.AcqId        = VAcqId;
    VPtFrame->Header.FrameIdInAcq = ViFrame;
    VPtFrame->Header.MapsName     = (UInt16) ASIC__MI26;
    VPtFrame->Header.MapsNb       = VPtRun->ParMi26Nb;
          
    for ( ViMi26=0; ViMi26 < Mi26Nb; ViMi26++ ) {
      VPtFrame->Header.AMapsHeader[ViMi26] = PtSrcW32[ViSrcW32];
      ++ViSrcW32;
    }
    
    /*
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
    */
    
    VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    ++VEChanTrigFieldCnt;
    
    
    for ( ViMi26=0; ViMi26 < Mi26Nb; ViMi26++ ) {
      VPtFrame->Header.AMapsFrameCnt[ViMi26] = PtSrcW32[ViSrcW32];
      ++ViSrcW32;
    }
    
    /*
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
    */
    
    VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    ++VEChanTrigFieldCnt;

 
    for ( ViMi26=0; ViMi26 < Mi26Nb; ViMi26++ ) {
      VADataLengthField[ViMi26] = PtSrcW32[ViSrcW32];
      ++ViSrcW32;
    }
    
    /*
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
    */
    
    VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    ++VEChanTrigFieldCnt;


    for ( ViMi26=0; ViMi26 < Mi26Nb; ViMi26++ ) {
      VADataLengthW16[ViMi26] = (VADataLengthField[ViMi26] & 0x0000FFFF) + ((VADataLengthField[ViMi26] & 0xFFFF0000) >> 16);
    }
    
    VDataLengthW16Max = MATH_FUInt16Max ( VADataLengthW16, Mi26Nb );
    
    /*
    VADataLengthW16[0] = (VADataLengthField[0] & 0x0000FFFF) + ((VADataLengthField[0] & 0xFFFF0000) >> 16);
    VADataLengthW16[1] = (VADataLengthField[1] & 0x0000FFFF) + ((VADataLengthField[1] & 0xFFFF0000) >> 16);
    VADataLengthW16[2] = (VADataLengthField[2] & 0x0000FFFF) + ((VADataLengthField[2] & 0xFFFF0000) >> 16);
    VADataLengthW16[3] = (VADataLengthField[3] & 0x0000FFFF) + ((VADataLengthField[3] & 0xFFFF0000) >> 16);
    VADataLengthW16[4] = (VADataLengthField[4] & 0x0000FFFF) + ((VADataLengthField[4] & 0xFFFF0000) >> 16);
    VADataLengthW16[5] = (VADataLengthField[5] & 0x0000FFFF) + ((VADataLengthField[5] & 0xFFFF0000) >> 16);
    
    VDataLengthW16Max = MATH_FUInt16Max ( VADataLengthW16, 6 );
    */
    
    
    // A better test ( on each VADataLengthW16[i] ? on each W16 on D0 & D1 ? ) should be done !
    
    if ( VDataLengthW16Max > 2304 ) {
      
      ++VDataLengthErrCnt;
      err_error (( ERR_OUT, "---------------------------------------------------------------------" ));
      err_error (( ERR_OUT, "HW error on data length -> Max W16 nb = %d -> Force 0 !", VDataLengthW16Max ));
      
      for ( ViMi26ChkDataLength = 0; ViMi26ChkDataLength < Mi26Nb /* 6 */; ViMi26ChkDataLength++ ) {
        if ( VADataLengthW16[ViMi26ChkDataLength] > 2304 ) {
          err_error (( ERR_OUT, "Error chip[%d] = %d W16", ViMi26ChkDataLength, VADataLengthW16[ViMi26ChkDataLength] ));
        }
      }
      
      err_error (( ERR_OUT, "---------------------------------------------------------------------" ));
      
      // 02/03/2011 Replace hard coded arrays size in memset by sizeof ()
      
      memset ( VADataLengthW8 , 0, Mi26Nb /* 6 */ * sizeof (VADataLengthW8[0] ) );
      memset ( VADataLengthW16, 0, Mi26Nb /* 6 */ * sizeof (VADataLengthW16[0]) );
      memset ( VADataLengthW32, 0, Mi26Nb /* 6 */ * sizeof (VADataLengthW32[0]) );
      
      VDataLengthW8Max  = 0;
      VDataLengthW32Max = 0;
      
    }
    
    else {
      
      
      for ( ViMi26=0; ViMi26 < Mi26Nb /* 6 */; ViMi26++ ) {
        VADataLengthW8[ViMi26]  = VADataLengthW16[ViMi26] * 2;
        VADataLengthW32[ViMi26] = VADataLengthW16[ViMi26] / 2;
      }
      
      VDataLengthW8Max  = VDataLengthW16Max * 2;
      VDataLengthW32Max = VDataLengthW16Max / 2;
    }
    
    
    
    for ( ViMi26=0; ViMi26 < Mi26Nb; ViMi26++ ) {
      VPtFrame->Header.AMapsDataLength[ViMi26] = VADataLengthW8[ViMi26];
    }
    
    /*
    VPtFrame->Header.AMapsDataLength[0] = VADataLengthW8[0];
    VPtFrame->Header.AMapsDataLength[1] = VADataLengthW8[1];
    VPtFrame->Header.AMapsDataLength[2] = VADataLengthW8[2];
    VPtFrame->Header.AMapsDataLength[3] = VADataLengthW8[3];
    VPtFrame->Header.AMapsDataLength[4] = VADataLengthW8[4];
    VPtFrame->Header.AMapsDataLength[5] = VADataLengthW8[5];
    */
    
    // Copy useful data part
    
    VDataLengthW32ToCpy = VDataLengthW32Max * Mi26Nb;
    
    // VDataLengthW32ToCpy = VDataLengthW32Max * 6;
        
    VDataLengthW8ToCpy  = VDataLengthW32ToCpy * 4;
    
    #ifdef EFRIO__FRAME_TAGS_ENABLE
      VPtFrame->Data.Tag       = EFRIO__FRAME_TAG_DATA;
    #endif
    
    
    VPtFrame->Data.TotSz     = VDataLengthW8ToCpy;
    VPtFrame->Data.OneMapsSz = VDataLengthW8Max;
    
    #ifndef EFRIO__DEMUX_MI26_DATA_PART
    
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
    
    
    #else
    
    
    VPtCpySrcW32      = (UInt32*) &PtSrcW32[ViSrcW32];
    
    for ( ViMi26=0; ViMi26 < Mi26Nb; ViMi26++ ) {
      VAPtCpyDestW32[ViMi26] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + (ViMi26 * VDataLengthW32Max) );
    }
    
    
    /*
    VAPtCpyDestW32[0] = (UInt32*) VPtFrame->Data.ADataW32;
    VAPtCpyDestW32[1] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 +       VDataLengthW32Max   );
    VAPtCpyDestW32[2] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 2 * VDataLengthW32Max ) );
    VAPtCpyDestW32[3] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 3 * VDataLengthW32Max ) );
    VAPtCpyDestW32[4] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 4 * VDataLengthW32Max ) );
    VAPtCpyDestW32[5] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 5 * VDataLengthW32Max ) );
    */
    

    
    for ( ViDataCpy=0; ViDataCpy < VDataLengthW32Max; ViDataCpy++ ) {

      for ( ViMi26=0; ViMi26 < Mi26Nb; ViMi26++ ) {
        *VAPtCpyDestW32[ViMi26] = *VPtCpySrcW32;
        ++VAPtCpyDestW32[ViMi26];
        ++VPtCpySrcW32;
      }
    
      /*
      *VAPtCpyDestW32[0] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[0];
      ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[1] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[1];
      ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[2] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[2];
      ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[3] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[3];
      ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[4] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[4];
      ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[5] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[5];
      ++VPtCpySrcW32;
      */     
      
      VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = *VPtCpySrcW32;
      ++VPtCpySrcW32;
      ++VEChanTrigFieldCnt;
    }
    
    VPtEChanSrcW32 = (UInt32*) VPtCpySrcW32;
    
    #endif
    
    
    // Continue extraction of trigger fields from trigger channel WHILE needed
    
    VPtEChanSrcW32 += Mi26Nb; // Bypass Mi26 x 6 data
    
    // VPtEChanSrcW32 += 6; // Bypass Mi26 x 6 data

    
    do {
      VEChanTrigField = *VPtEChanSrcW32;
      
      if ( VEChanTrigFieldCnt >= EFRIO__EXTRA_CHAN__MAX_TRIGGER_FIELD_NB ) {
        err_warning (( ERR_OUT, "Max trigger field nb reached = %d <=> %d max trigger info !", EFRIO__EXTRA_CHAN__MAX_TRIGGER_FIELD_NB, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ));
        break;
      }
      
      VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = VEChanTrigField;
      
      VPtEChanSrcW32 += VMi26NbP1;
      
      // VPtEChanSrcW32 += 7;
      
      ++VEChanTrigFieldCnt;
    } while ( (VEChanTrigField & 0x80000000) == 0 );
    

    ViSrcW32 += (VMi26NbP1 * MI26__ZS_FFRAME_RAW_MAX_W32);
    
    // ViSrcW32 += (7 * MI26__ZS_FFRAME_RAW_MAX_W32);
    
    
    for ( ViMi26=0; ViMi26 < Mi26Nb; ViMi26++ ) {
      VPtFrame->Header.AMapsTrailer[ViMi26] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + (3 * VMi26NbP1) + ViMi26 + (VMi26NbP1 * VADataLengthW32[ViMi26])];
      ++ViSrcW32;
    }


  
    // 21 = 7 x 3 Fields nb before first data = Header, Frame cnt, Data length
    /*
    VPtFrame->Header.AMapsTrailer[0] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + (7 * VADataLengthW32[0])];    
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
    */
    
    
    ++ViSrcW32; // To count extra channel

    
    // Read trigger number from "Mi26" frame
    
    if ( TrigStatus == 0 ) {
      VZero  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + (3 * VMi26NbP1) + (VMi26NbP1 * MI26__ZS_FFRAME_RAW_MAX_W32) +  VMi26NbP1];
      
      // VZero  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) +  7];
    }
    
    // Emulate trigger by overwriting info from "Mi26" frame
    
    else {
      VZero = (TrigStatus << 16);
    }
    
    
    VZero2 = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + (3 * VMi26NbP1) + (VMi26NbP1 * MI26__ZS_FFRAME_RAW_MAX_W32) + (2 * VMi26NbP1)];
    
    // VZero2 = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 14];
    
    ViSrcW32 += (2 * VMi26NbP1); // 7 times 2 zero fields = 14
    
    // ViSrcW32 += 14; // 7 times 2 zero fields = 14


    // Extraction of trigger info from "Mimosa 26 frame" zero fields
    
    VTrigNb = (VZero  & 0xFFFF0000) >> 16;
    
    if ( VTrigNb > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ) {
      err_warning (( ERR_OUT, "VTrigNb=%d > Max=%d => Limit to max !", VTrigNb, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ));
      VTrigNb = EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26;
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
    
    // Check frame relevant fields ( DataLength, Header, frame counter, trailer => NOT data part )
    // Enable / Disable done by a call to EFRIO__FTestOnDataStartStop ( Start, PrintLvl )
    

    
    EFRIO__MI26_FChkFrameLight ( 0 /* FuncId */ , ViFrame, VPtFrame, Mi26Nb );
    
    // EFRIO__MI26_FChkFrameLight ( 0 /* FuncId */ , ViFrame, VPtFrame, 6 /* Mi26Nb */ );
    
    ++VPtFrList->TotFrameNb;
    
    ++VRunFrameCnt;
    
    // Update size of acquisition to send on Ethernet for monitoring
    // 11/03/2011
    //
    // The update is done after processing each frame
    // -> it cost execution time
    // -> but we are sure that update has been done
    
    if ( ViFrame < VPtCont->MonCont.InfFrameNbToSend ) {
      VPtCont->MonCont.InfSzToSend = VTotAcqSz;
    }
    
    
  } // End for ViFrame

  
  ++VPtCont->RunCont.ResAcqCnt;
  VPtCont->RunCont.ResFrameCnt = VPtCont->RunCont.ResAcqCnt * VPtCont->RunCont.ParFrameNbPerAcq;
  VPtCont->RunCont.ResEventCnt = VPtCont->RunCont.ResFrameCnt;

  
  #ifdef EFRIO__INCLUDE_PARA_PORT
  PPO_FOutD5 ( 0 /* Id */, 0 /* State */ );
  #endif
  
  return (VTotAcqSz);
}




/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet3Mode6Mi26 (
          :   SInt32 BoardId , UInt32*  PtSrcW32, SInt32 EltNb,
          :   SInt32 AcqStatus, SInt32 TrigStatus,  SInt8 DataConvertMode )
          :
Goal      : Flex RIO readout function for six Mi26 in EUDET mode 3
          :
          : Read data of one acquisition from Flex RIO, format them in EUDET mode 3
          : by adding extra information and fill PC RAM buffer.
          :
          : The extra channel is enabled in EUDET mode 3, therefore for each trigger
          : two informations are stored
          :
          : - The TLU trigger                    -> see record EFRIO__TTluTrigger
          : - The Flex RIO trigger / time stamp  -> see record EFRIO__TFlexRioTimeStamp1
          :
          :***************************************************************************
          : Only the frames with a trigger nb > 0 + EFRIO__FRAME_NB_TO_READ_AFTER_TRIG
          : frames after trigger are stored in memory.
          : The selection is done by this function.
          :***************************************************************************
          :
Inputs    : BoardId             - Board identifier
          : PtSrcW32            - Pointer to board data ( output of Labview Vi )
          : EltNb               - Board data size in W32 unit ( 1 Elt = 1 W32 )
          : AcqStatus           - Acquisition status flag provided by labview
          : TrigStatus          - Trigger status flag provided by labview
          :
          : DataConvertMode     - Not used => Global EFRIO__TRunCont.ParDataTransferMode is used
          :
          : TriggerHandlingMode - Reserved for future use
          :          
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
Rev       : 30/12/2010
          : - Add demux of data part if EFRIO__DEMUX_MI26_DATA_PART defined in eudet_frio.def
          :
          : 27/01/2011
          : - Improve sw robustness against corruped data from Flex RIO
          :
          : 15/02/2011
          : - Update MonitorCont record fields
          :
          : 16/02/2011
          : - Update ResEventCnt = ResFrameCnt, set to 0 before
          :
          : 23/02/2011
          : - Modify types of AcqStatus, TrigStatus parameters to SInt32
          :
          : 24/02/2011
          : - Update new fields AcqStatus, TrigStatus of AAcqFrameList
          :
          : 12/03/2011
          : - Update fields ResAcqCnt, ResFrameCnt, ResEventCnt in a different way
          : -- incement ResAcqCnt ONLY if there is at least one trigger in acq
          : -- ResFrameCnt = number of frames with trigger + N following one
          : -- ResEventCnt = ResFrameCnt
          :    
          : - Add parameter TriggerHandlingMode
          : - Always take first EFRIO__FRAME_NB_TO_READ_AFTER_TRIG frames if TriggerHandlingMode = 1
          :
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet3Mode6Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode ) {
  
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
//  SInt32               VRunFrameCnt;
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
  UInt32*              VPtCpySrcW32;
  UInt32*              VAPtCpyDestW32[6];
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
  
  
  
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FOutD5 ( 0 /* Id */, 1 /* State */ );
  #endif
  
  if ( VPtCont->InfInitDone == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort because : Init not done") );
  }
  
  err_trace (( ERR_OUT, "EFRIO__MI26_FFRioAcqDeserDataEudet3Mode6Mi26 (P=%x, EltNb=%d)", PtSrcW32, EltNb ));
  
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
  
  VPtFrList->AcqStatus  = AcqStatus;
  VPtFrList->TrigStatus = TrigStatus;
  VPtFrList->TotFrameNb = 0;
  
  memset ( VPtFrList->AFramePtr, 0, EFRIO__MAX_FRAME_NB_PER_ACQ * sizeof (EFRIO__TFrame*) );

  // Reset mon size
  
  VPtCont->MonCont.InfSzToSend = 0;
  
  // Calculate size of records WITHOUT variable length part counted here as one elt
  
  VEmptyTrigRecSz  = sizeof ( EFRIO__TTriggerRec );
  VEmptyFrameRecSz = sizeof ( EFRIO__TFrame );
    
  
  // Reset tmp triggers buffer
  
  memset ( VPtTmpTrigRec, 0, sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );


  #ifdef EFRIO__INCLUDE_PARA_PORT
    // PPO_FOutD5 ( 0 /* Id */, 0 /* State */ );
  #endif
  
  // ----------------------------------------------------------------------------------
  // List frames to extract => frame with trigger + 3 following frames
  // ----------------------------------------------------------------------------------
  
  // 02/03/2011 Replace hard coded size by sizeof ()
  
  memset ( VPtCont->AAAcqFrameWithTrigList[0], 0xFF, EFRIO__MAX_FRAME_NB_PER_ACQ * sizeof (VPtCont->AAAcqFrameWithTrigList[0][0]) );
    
  // Init loop parameters
  
  VFrameWithTrigCnt    =  0;
  VLastFrameWithTrig   = -1;
  VPtFrameWithTrigList = VPtCont->AAAcqFrameWithTrigList[0];


  // If trigger mode = 1 => Take always first frame of acq EFRIO__FRAME_NB_TO_READ_AFTER_TRIG
  //
  // Done by overwriting VFrNbToTakeAtBeginOfAcqForLastTrig ( set on previous acq ) with EFRIO__FRAME_NB_TO_READ_AFTER_TRIG
  
  if ( TriggerHandlingMode == 1 ) {
    VFrNbToTakeAtBeginOfAcqForLastTrig = EFRIO__FRAME_NB_TO_READ_AFTER_TRIG;
  }
  
  
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
  
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    // PPO_FOutD5 ( 0 /* Id */, 1 /* State */ );
  #endif 
  
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
      
    }
    
  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtFrame->Tag        = EFRIO__FRAME_TAG;
    VPtFrame->Header.Tag          = EFRIO__FRAME_TAG_HEADER;
  #endif
  
    VPtFrame->Header.AcqStatus    = AcqStatus;
    VPtFrame->Header.TrigStatus   = TrigStatus;
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

      // 02/03/2011 Replace loop for array reset by memset ()
      
      memset ( VADataLengthW8 , 0, 6 * sizeof (VADataLengthW8[0] ) );
      memset ( VADataLengthW16, 0, 6 * sizeof (VADataLengthW16[0]) );
      memset ( VADataLengthW32, 0, 6 * sizeof (VADataLengthW32[0]) );
      
/*    Removed on 02/03/2011

      for ( ViMi26=0; ViMi26 < 6; ViMi26++ ) {
        VADataLengthW8[ViMi26]  = 0; 
        VADataLengthW16[ViMi26] = 0; 
        VADataLengthW32[ViMi26] = 0; 
      }
      
*/            
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
    

    
  #ifndef EFRIO__DEMUX_MI26_DATA_PART
  
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
    
  #else

    VPtCpySrcW32      = (UInt32*) &PtSrcW32[ViSrcW32];
    VAPtCpyDestW32[0] = (UInt32*) VPtFrame->Data.ADataW32;
    VAPtCpyDestW32[1] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 +       VDataLengthW32Max   );
    VAPtCpyDestW32[2] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 2 * VDataLengthW32Max ) );
    VAPtCpyDestW32[3] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 3 * VDataLengthW32Max ) );
    VAPtCpyDestW32[4] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 4 * VDataLengthW32Max ) );
    VAPtCpyDestW32[5] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 5 * VDataLengthW32Max ) );
    
    for ( ViDataCpy=0; ViDataCpy < VDataLengthW32Max; ViDataCpy++ ) {
      
      *VAPtCpyDestW32[0] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[0];
      ++VPtCpySrcW32;

      *VAPtCpyDestW32[1] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[1];
      ++VPtCpySrcW32;

      *VAPtCpyDestW32[2] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[2];
      ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[3] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[3];
      ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[4] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[4];
      ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[5] = *VPtCpySrcW32;
      ++VAPtCpyDestW32[5];
      ++VPtCpySrcW32;
      
            
      VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = *VPtCpySrcW32;
      ++VPtCpySrcW32;
      ++VEChanTrigFieldCnt;
    }

    VPtEChanSrcW32 = (UInt32*) VPtCpySrcW32;
    
  #endif

  
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
    
    if ( VTrigNb > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ) {
      err_warning (( ERR_OUT, "VTrigNb=%d > Max=%d => Limit to max !", VTrigNb, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ));
      VTrigNb = EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26;
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
    
    // Check frame relevant fields ( DataLength, Header, frame counter, trailer => NOT data part )
    // Enable / Disable done by a call to EFRIO__FTestOnDataStartStop ( Start, PrintLvl )
    
    EFRIO__MI26_FChkFrameLight ( 0 /* FuncId */ , VFrameId, VPtFrame, 6 /* Mi26Nb */ );
    

  
    ++VPtFrList->TotFrameNb;
    
    // ++VRunFrameCnt;
    
    // Update size of acquisition to send on Ethernet for monitoring
    // 15/02/2011
    //
    // The update is done after processing each frame
    // -> it cost execution time
    // -> but we are sure that update has been done  
    
    if ( ViFrameWithTrig < VPtCont->MonCont.InfFrameNbToSend ) {
      VPtCont->MonCont.InfSzToSend = VTotAcqSz;
    }
    
  } // End for ViFrameWithTrig
  

  /* Before 12/03/2011
  
  ++VPtCont->RunCont.ResAcqCnt;
  VPtCont->RunCont.ResFrameCnt = VPtCont->RunCont.ResAcqCnt * VPtCont->RunCont.ParFrameNbPerAcq;
  VPtCont->RunCont.ResEventCnt = VPtCont->RunCont.ResFrameCnt;
  
  */
  
  if ( VPtFrList->TotFrameNb != 0 ) {
    ++VPtCont->RunCont.ResAcqCnt;
    VPtCont->RunCont.ResFrameCnt = VPtCont->RunCont.ResFrameCnt + VPtFrList->TotFrameNb;
    VPtCont->RunCont.ResEventCnt = VPtCont->RunCont.ResFrameCnt;
  }
    
  
  #ifdef EFRIO__INCLUDE_PARA_PORT  
    PPO_FOutD5 ( 0 /* Id */, 0 /* State */ );
  #endif
  
  return (VTotAcqSz);
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet3Mode8Mi26 (
          :   SInt32 BoardId , UInt32*  PtSrcW32, SInt32 EltNb,
          :   SInt32 AcqStatus, SInt32 TrigStatus,  SInt8 DataConvertMode )
          :
Goal      : Flex RIO readout function for six Mi26 in EUDET mode 3
          :
          : Read data of one acquisition from Flex RIO, format them in EUDET mode 3
          : by adding extra information and fill PC RAM buffer.
          :
          : The extra channel is enabled in EUDET mode 3, therefore for each trigger
          : two informations are stored
          :
          : - The TLU trigger                    -> see record EFRIO__TTluTrigger
          : - The Flex RIO trigger / time stamp  -> see record EFRIO__TFlexRioTimeStamp1
          :
          :***************************************************************************
          : Only the frames with a trigger nb > 0 + EFRIO__FRAME_NB_TO_READ_AFTER_TRIG
          : frames after trigger are stored in memory.
          : The selection is done by this function.
          :***************************************************************************
          :
          Inputs    : BoardId             - Board identifier
          : PtSrcW32            - Pointer to board data ( output of Labview Vi )
          : EltNb               - Board data size in W32 unit ( 1 Elt = 1 W32 )
          : AcqStatus           - Acquisition status flag provided by labview
          : TrigStatus          - Trigger status flag provided by labview
          :
          : DataConvertMode     - Not used => Global EFRIO__TRunCont.ParDataTransferMode is used
          :
          : TriggerHandlingMode - Reserved for future use
          :
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
Date      : 28/04/2011 ( Upgrade to 8 Mi26 from 03/11/2010 version handling 6 Mi26 )
          :
Rev       : 30/12/2010
          : - Add demux of data part if EFRIO__DEMUX_MI26_DATA_PART defined in eudet_frio.def
          :
          : 27/01/2011
          : - Improve sw robustness against corruped data from Flex RIO
          :
          : 15/02/2011
          : - Update MonitorCont record fields
          :
          : 16/02/2011
          : - Update ResEventCnt = ResFrameCnt, set to 0 before
          :
          : 23/02/2011
          : - Modify types of AcqStatus, TrigStatus parameters to SInt32
          :
          : 24/02/2011
          : - Update new fields AcqStatus, TrigStatus of AAcqFrameList
          :
          : 12/03/2011
          : - Update fields ResAcqCnt, ResFrameCnt, ResEventCnt in a different way
          : -- incement ResAcqCnt ONLY if there is at least one trigger in acq
          : -- ResFrameCnt = number of frames with trigger + N following one
          : -- ResEventCnt = ResFrameCnt
          :
          : - Add parameter TriggerHandlingMode
          : - Always take first EFRIO__FRAME_NB_TO_READ_AFTER_TRIG frames if TriggerHandlingMode = 1
          :
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet3Mode8Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode ) {
  
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
  SInt32               V9FrameId;
  UInt32               VADataLengthField[8];
  UInt32               VADataLengthW8[8];
  UInt16               VADataLengthW16[8];
  UInt32               VADataLengthW32[8];
  UInt32               VDataLengthW8Max;
  UInt32               VDataLengthW16Max;
  UInt32               VDataLengthW32Max;
  UInt32               VDataLengthW32ToCpy;
  UInt32               VDataLengthW8ToCpy;
  SInt32               ViSrcW32;
  SInt32               ViDataCpy;
  SInt32               ViDataCpyP1;
  SInt32               ViDataCpyP2;
  //  SInt32               VRunFrameCnt;
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
  UInt32*              VPtCpySrcW32;
  UInt32*              VAPtCpyDestW32[8];
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
  
  
  
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FOutD5 ( 0 /* Id */, 1 /* State */ );
  #endif
  
  if ( VPtCont->InfInitDone == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort because : Init not done") );
  }
  
  err_trace (( ERR_OUT, "EFRIO__MI26_FFRioAcqDeserDataEudet3Mode6Mi26 (P=%x, EltNb=%d)", PtSrcW32, EltNb ));
  
  // Pointers parameters check
  
  err_retnull ( PtSrcW32, (ERR_OUT,"PtSrcW32 = NULL") );
  
  // Init board conf record pointer
  
  if ( (BoardId < 0) || (BoardId >= EFRIO__MAX_BOARDS_NB) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Board Id=%d out of range [0..%d])", BoardId, EFRIO__MAX_BOARDS_NB - 1) );
  }
  
  VPtBoard = &EFRIO__VGContext.ABoardsConf[BoardId];
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ) / 9; // Divide by 9 because of extral channel
  
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
  
  VPtFrList->AcqStatus  = AcqStatus;
  VPtFrList->TrigStatus = TrigStatus;
  VPtFrList->TotFrameNb = 0;
  
  memset ( VPtFrList->AFramePtr, 0, EFRIO__MAX_FRAME_NB_PER_ACQ * sizeof (EFRIO__TFrame*) );
  
  // Reset mon size
  
  VPtCont->MonCont.InfSzToSend = 0;
  
  // Calculate size of records WITHOUT variable length part counted here as one elt
  
  VEmptyTrigRecSz  = sizeof ( EFRIO__TTriggerRec );
  VEmptyFrameRecSz = sizeof ( EFRIO__TFrame );
  
  
  // Reset tmp triggers buffer
  
  memset ( VPtTmpTrigRec, 0, sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );
  
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
  // PPO_FOutD5 ( 0 /* Id */, 0 /* State */ );
  #endif
  
  // ----------------------------------------------------------------------------------
  // List frames to extract => frame with trigger + 3 following frames
  // ----------------------------------------------------------------------------------
  
  // 02/03/2011 Replace hard coded size by sizeof ()
  
  memset ( VPtCont->AAAcqFrameWithTrigList[0], 0xFF, EFRIO__MAX_FRAME_NB_PER_ACQ * sizeof (VPtCont->AAAcqFrameWithTrigList[0][0]) );
  
  // Init loop parameters
  
  VFrameWithTrigCnt    =  0;
  VLastFrameWithTrig   = -1;
  VPtFrameWithTrigList = VPtCont->AAAcqFrameWithTrigList[0];
  
  
  // If trigger mode = 1 => Take always first frame of acq EFRIO__FRAME_NB_TO_READ_AFTER_TRIG
  //
  // Done by overwriting VFrNbToTakeAtBeginOfAcqForLastTrig ( set on previous acq ) with EFRIO__FRAME_NB_TO_READ_AFTER_TRIG
  
  if ( TriggerHandlingMode == 1 ) {
    VFrNbToTakeAtBeginOfAcqForLastTrig = EFRIO__FRAME_NB_TO_READ_AFTER_TRIG;
  }
  
  
  for ( VFrameId=0; VFrameId < VPtBoard->FrameNbPerAcq; VFrameId++ ) {
    
    V9FrameId = 9 * VFrameId;
    
    if ( TrigStatus == 0 ) {
      VZero  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V9FrameId) + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) +  9]; // 27 = 9 x 3 Fields nb before first data = Header, Frame cnt, Data length
    }                                                                                                                  //  9 = 9 x 1 Trailer
    
    // Emulate trigger by overwriting info from "Mi26" frame
    // BUT first read it from board RAM in order to get real execution time
    
    else {
      VZero  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V9FrameId) + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) +  9];
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
  
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
  // PPO_FOutD5 ( 0 /* Id */, 1 /* State */ );
  #endif
  
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
    ViSrcW32           = VFrameId * 9 * MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32;
    
    V9FrameId = 9 * VFrameId;
    
    // Update pointer to next frame
    
    if ( VPtFrList->TotFrameNb == 0 ) {
      VPtFrame = VPtFrList->AFramePtr[0] = VPtRun->PtFrame;
    }
    
    else {
      
      if ( VPtFrList->TotFrameNb >= VPtCont->RunCont.ParFrameNbPerAcq ) {
        err_retfail ( -1, (ERR_OUT,"Abort try to process frame %d > Max = %d ", VPtFrList->TotFrameNb, VPtCont->RunCont.ParFrameNbPerAcq) );
      }
      
      VPtFrame = VPtFrList->AFramePtr[VPtFrList->TotFrameNb] = (EFRIO__TFrame*) ((UInt8*) VPtFrList->AFramePtr[VPtFrList->TotFrameNb - 1] + VFullFrameRecSz);
      
    }
    
    #ifdef EFRIO__FRAME_TAGS_ENABLE
      VPtFrame->Tag        = EFRIO__FRAME_TAG;
      VPtFrame->Header.Tag          = EFRIO__FRAME_TAG_HEADER;
    #endif
    
    VPtFrame->Header.AcqStatus    = AcqStatus;
    VPtFrame->Header.TrigStatus   = TrigStatus;
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
    VPtFrame->Header.AMapsHeader[6]    = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsHeader[7]    = PtSrcW32[ViSrcW32];
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
    VPtFrame->Header.AMapsFrameCnt[6]  = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VPtFrame->Header.AMapsFrameCnt[7]  = PtSrcW32[ViSrcW32];
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
    VADataLengthField[6]                = PtSrcW32[ViSrcW32];
    ++ViSrcW32;
    VADataLengthField[7]                = PtSrcW32[ViSrcW32];
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
    VADataLengthW16[6] = (VADataLengthField[6] & 0x0000FFFF) + ((VADataLengthField[6] & 0xFFFF0000) >> 16);
    VADataLengthW16[7] = (VADataLengthField[7] & 0x0000FFFF) + ((VADataLengthField[7] & 0xFFFF0000) >> 16);
    
    VDataLengthW16Max = MATH_FUInt16Max ( VADataLengthW16, 8 );
    
    
    // A better test ( on each VADataLengthW16[i] ? on each W16 on D0 & D1 ? ) should be done !
    
    if ( VDataLengthW16Max > 2304 ) {
      
      // 02/03/2011 Replace loop for array reset by memset ()
      
      memset ( VADataLengthW8 , 0, 8 * sizeof (VADataLengthW8[0] ) );
      memset ( VADataLengthW16, 0, 8 * sizeof (VADataLengthW16[0]) );
      memset ( VADataLengthW32, 0, 8 * sizeof (VADataLengthW32[0]) );
      
      /*    Removed on 02/03/2011
      
      for ( ViMi26=0; ViMi26 < 8; ViMi26++ ) {
        VADataLengthW8[ViMi26]  = 0;
        VADataLengthW16[ViMi26] = 0;
        VADataLengthW32[ViMi26] = 0;
      }
      
      */
      
      VDataLengthW8Max  = 0;
      VDataLengthW32Max = 0;
      
      ++VDataLengthErrCnt;
      err_error (( ERR_OUT, "HW error on data length -> Max W16 nb = %d -> Force 0 !", VDataLengthW16Max ));
      
    }
    
    else {
      
      for ( ViMi26=0; ViMi26 < 8; ViMi26++ ) {
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
    VPtFrame->Header.AMapsDataLength[6] = VADataLengthW8[6];
    VPtFrame->Header.AMapsDataLength[7] = VADataLengthW8[7];
    
    // Copy useful data part
    
    VDataLengthW32ToCpy = VDataLengthW32Max * 8;
    VDataLengthW8ToCpy  = VDataLengthW32ToCpy * 4;
    
    #ifdef EFRIO__FRAME_TAGS_ENABLE
      VPtFrame->Data.Tag       = EFRIO__FRAME_TAG_DATA;
    #endif
    
    
    VPtFrame->Data.TotSz     = VDataLengthW8ToCpy;
    VPtFrame->Data.OneMapsSz = VDataLengthW8Max;
    
    
    
    #ifndef EFRIO__DEMUX_MI26_DATA_PART

      // WARNING 06/05/2011 => This part has been updated BUT not tested
      
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
        *VPtCpyDestW64 = *VPtCpySrcW64;
        ++VPtCpyDestW64;
        ++VPtCpySrcW64;
        
        VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = *((UInt32*) VPtCpySrcW64);
        (UInt32) VPtCpySrcW64 = (UInt32) (VPtCpySrcW64) + 4;
        ++VEChanTrigFieldCnt;
      }
      
      VPtEChanSrcW32 = (UInt32*) VPtCpySrcW64;
      
    #else
    
      VPtCpySrcW32      = (UInt32*) &PtSrcW32[ViSrcW32];
      VAPtCpyDestW32[0] = (UInt32*) VPtFrame->Data.ADataW32;
      VAPtCpyDestW32[1] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 +       VDataLengthW32Max   );
      VAPtCpyDestW32[2] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 2 * VDataLengthW32Max ) );
      VAPtCpyDestW32[3] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 3 * VDataLengthW32Max ) );
      VAPtCpyDestW32[4] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 4 * VDataLengthW32Max ) );
      VAPtCpyDestW32[5] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 5 * VDataLengthW32Max ) );
      VAPtCpyDestW32[6] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 6 * VDataLengthW32Max ) );
      VAPtCpyDestW32[7] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 7 * VDataLengthW32Max ) );
      
      for ( ViDataCpy=0; ViDataCpy < VDataLengthW32Max; ViDataCpy++ ) {
        
        *VAPtCpyDestW32[0] = *VPtCpySrcW32;
        ++VAPtCpyDestW32[0];
        ++VPtCpySrcW32;
        
        *VAPtCpyDestW32[1] = *VPtCpySrcW32;
        ++VAPtCpyDestW32[1];
        ++VPtCpySrcW32;
        
        *VAPtCpyDestW32[2] = *VPtCpySrcW32;
        ++VAPtCpyDestW32[2];
        ++VPtCpySrcW32;
        
        *VAPtCpyDestW32[3] = *VPtCpySrcW32;
        ++VAPtCpyDestW32[3];
        ++VPtCpySrcW32;
        
        *VAPtCpyDestW32[4] = *VPtCpySrcW32;
        ++VAPtCpyDestW32[4];
        ++VPtCpySrcW32;
        
        *VAPtCpyDestW32[5] = *VPtCpySrcW32;
        ++VAPtCpyDestW32[5];
        ++VPtCpySrcW32;
        
        *VAPtCpyDestW32[6] = *VPtCpySrcW32;
        ++VAPtCpyDestW32[6];
        ++VPtCpySrcW32;

        *VAPtCpyDestW32[7] = *VPtCpySrcW32;
        ++VAPtCpyDestW32[7];
        ++VPtCpySrcW32;
        
        VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = *VPtCpySrcW32;
        ++VPtCpySrcW32;
        ++VEChanTrigFieldCnt;
      }
      
      VPtEChanSrcW32 = (UInt32*) VPtCpySrcW32;
      
    #endif
    
    
    // Continue extraction of trigger fields from trigger channel WHILE needed
    
    VPtEChanSrcW32 += 8; // Bypass Mi26 x 8 data
    
    do {
      VEChanTrigField = *VPtEChanSrcW32;
      
      if ( VEChanTrigFieldCnt >= EFRIO__EXTRA_CHAN__MAX_TRIGGER_FIELD_NB ) {
        err_warning (( ERR_OUT, "Max trigger field nb reached = %d <=> %d max trigger info !", EFRIO__EXTRA_CHAN__MAX_TRIGGER_FIELD_NB, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ));
        break;
      }
      
      VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = VEChanTrigField;
      VPtEChanSrcW32 += 9;
      ++VEChanTrigFieldCnt;
    } while ( (VEChanTrigField & 0x80000000) == 0 );
    
    
    
    ViSrcW32 += (9 * MI26__ZS_FFRAME_RAW_MAX_W32);
    
    
    //    VptZsFFrameRaw[V6iFrame].Trailer = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * VADataLengthW32[0])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    //    ++ViSrcW32;
    
    //    VZero = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 6];
    //    VptZsFFrameRaw[V6iFrame].Zero = VZero;
    //    ++ViSrcW32;
    
    //    VZero2 = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 12];
    //    VptZsFFrameRaw[V6iFrame].Zero2 = VZero2;
    //    ++ViSrcW32;
    
    
    VPtFrame->Header.AMapsTrailer[0] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V9FrameId) + 27 + (9 * VADataLengthW32[0])];    // 27 = 9 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;                                                                                                  
    
    VPtFrame->Header.AMapsTrailer[1] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V9FrameId) + 27 + 1 + (9 * VADataLengthW32[1])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[2] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V9FrameId) + 27 + 2 + (9 * VADataLengthW32[2])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[3] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V9FrameId) + 27 + 3 + (9 * VADataLengthW32[3])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[4] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V9FrameId) + 27 + 4 + (9 * VADataLengthW32[4])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[5] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V9FrameId) + 27 + 5 + (9 * VADataLengthW32[5])];
    ++ViSrcW32;

    VPtFrame->Header.AMapsTrailer[6] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V9FrameId) + 27 + 6 + (9 * VADataLengthW32[6])];
    ++ViSrcW32;

    VPtFrame->Header.AMapsTrailer[7] = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V9FrameId) + 27 + 7 + (9 * VADataLengthW32[7])];
    ++ViSrcW32;
    
    ++ViSrcW32; // To count extra channel
    
    
    // Read trigger number from "Mi26" frame
    
    if ( TrigStatus == 0 ) {
      VZero  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V9FrameId) + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) +  9]; // 27 = 9 x 3 Fields nb before first data = Header, Frame cnt, Data length
    }                                                                                                                  //  9 = 9 x 1 Trailer
    
    // Emulate trigger by overwriting info from "Mi26" frame
    
    else {
      VZero = (TrigStatus << 16);
    }
    
    // VZero  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V9FrameId) + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) +  9];
    
    VZero2 = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V9FrameId) + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 18];
    
    ViSrcW32 += 18; // 9 times 2 zero fields = 18
    
    // Extraction of trigger info from "Mimosa 26 frame" zero fields
    
    VTrigNb = (VZero  & 0xFFFF0000) >> 16;
    
    if ( VTrigNb > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ) {
      err_warning (( ERR_OUT, "VTrigNb=%d > Max=%d => Limit to max !", VTrigNb, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ));
      VTrigNb = EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26;
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
    
    // Check frame relevant fields ( DataLength, Header, frame counter, trailer => NOT data part )
    // Enable / Disable done by a call to EFRIO__FTestOnDataStartStop ( Start, PrintLvl )
    
    EFRIO__MI26_FChkFrameLight ( 0 /* FuncId */ , VFrameId, VPtFrame, 8 /* Mi26Nb */ );

    
    
    ++VPtFrList->TotFrameNb;
    
    // ++VRunFrameCnt;
    
    // Update size of acquisition to send on Ethernet for monitoring
    // 15/02/2011
    //
    // The update is done after processing each frame
    // -> it cost execution time
    // -> but we are sure that update has been done
    
    if ( ViFrameWithTrig < VPtCont->MonCont.InfFrameNbToSend ) {
      VPtCont->MonCont.InfSzToSend = VTotAcqSz;
    }
    
  } // End for ViFrameWithTrig
  
  
  /* Before 12/03/2011
  
  ++VPtCont->RunCont.ResAcqCnt;
  VPtCont->RunCont.ResFrameCnt = VPtCont->RunCont.ResAcqCnt * VPtCont->RunCont.ParFrameNbPerAcq;
  VPtCont->RunCont.ResEventCnt = VPtCont->RunCont.ResFrameCnt;
  
  */
  
  if ( VPtFrList->TotFrameNb != 0 ) {
    ++VPtCont->RunCont.ResAcqCnt;
    VPtCont->RunCont.ResFrameCnt = VPtCont->RunCont.ResFrameCnt + VPtFrList->TotFrameNb;
    VPtCont->RunCont.ResEventCnt = VPtCont->RunCont.ResFrameCnt;
  }
  
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FOutD5 ( 0 /* Id */, 0 /* State */ );
  #endif
  
  return (VTotAcqSz);
}

#endif // NO_MI26


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__MI26_FFRioAcqDeserDataMi26 (
          :  SInt8 Mi26Nb, SInt32 BoardId, UInt32* PtSrcW32AsPt, UInt32 PtSrcW32AsInt,
          :  SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, UInt32 WaitMsAtEnd,
          :  SInt8 DataConvertMode, SInt8 TriggerHandlingMode, SInt16 EmuleMode )
          :
Goal      : This function is the upper level of Flex RIO readout functions, it calls
          : the right redaout function depending on Mi26Nb & DataConvertMode parameters.
          : On Labview side, this function is encapsulated in a Vi of the same name,
          : which is called each time an acquisition is finished.
          :
          : This function also calls the frames emulation functions if emulation mode
          : is enabled.
          :
          :
Inputs    : Mi26Nb               - Number of Mimosa 26 to acquire
          : BoardId              - Board identifier
          :
          : PtSrcW32AsPt         - Pointer on Flex RIO DRAM as pointer
          : PtSrcW32AsInt        - Pointer on Flex RIO DRAM as an integer
          :
          : EltNb                - Size of flex RIO DRAM in W32 ( 1 Elt = 1 W32 )
          : AcqStatus            - Acquisition status flag provided by board
          : TrigStatus           - Trigger status flag provided by board
          : WaitMsAtEnd          - Wait ( in ms ) at end of function to measure free time
          :
          : DataConvertMode      - = DataTransferMode of EFRIO__FConfRun
          :                        See EFRIO__FConfRun for more inforation
          :                        Read also Rev 27/01/2011 comment about DataConvertMode handling
          :
          : TriggerHandlingMode  - Mode of trigger operation
          
          : EmuleMode            - Enable frames emulation mode
          :
          :                      - 0   -> No frames emulation
          :
          :                      - 1   -> Emulation mode
          :                               Mode IPHC & EUDET 1 -> 3 triggers / frame
          :                               Mode EUDET 2 & 3    -> no trigger / frame
          :
          :                      - < 0 -> Emulation mode
          :                               Mode IPHC & EUDET 1 -> 3 triggers / frame
          :                               Mode EUDET 2 & 3    -> | EmuleMode | triggers / frame
          :
          :
Ouputs    : The function returns
          : -1 if an error occurs
          : > 0 = if OK = Total acquisition size ( in bytes ) = size of data bloc after data processing ( for example : extraction of frames with trigger )
          :       This is the USEFUL size of buffer pointer by EFRIO__VGContext.RunCont.PtFrame which contains all frames of one acquisition
          :
Globals   :
          :
Remark    :
          :
Level     :
Date      : 11/08/2010
Rev       : 25/10/2010
          : - EUDET data formatting mode + trigger handling implementation
          :
          : 27/01/2011
          : - Modify handling of parameter DataConvertMode
          :   If DataConvertMode == -1 => Use EFRIO__FConfRun.ParDataTransferMode
          :   otherwise use DataConvertMode ( as is was before 27/01/2011 )
          :
          : 23/02/2011
          : - Modify types of AcqStatus, TrigStatus parameters to SInt32
          :          :          
Doc date  : 07/11/2010
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
  

SInt32 EFRIO__MI26_FFRioAcqDeserDataMi26 ( SInt8 Mi26Nb, SInt32 BoardId, UInt32* PtSrcW32AsPt, UInt32 PtSrcW32AsInt, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, UInt32 WaitMsAtEnd, SInt8 DataConvertMode, SInt8 TriggerHandlingMode, SInt16 EmuleMode ) {
  
  EFRIO__TContext*     VPtCont    = &EFRIO__VGContext;
  EFRIO__TRunCont*     VPtRunCont = &EFRIO__VGContext.RunCont;
  
  SInt32 VRet = 0;
  SInt32 VEmuleFrameNb;
  static UInt32 VEmuleFirstFrameNo = 0;
  
  SInt32 VDbgOffset;
  
#ifndef NO_MI26


  // 27/01/11

  if ( DataConvertMode == -1 ) {
    DataConvertMode = VPtRunCont->ParDataTransferMode;
  }


  if ( PtSrcW32AsInt != 0 ) {
    PtSrcW32AsPt = (UInt32*) PtSrcW32AsInt;
  }


/* Uncomment to enable data dump

  msg (( MSG_OUT, "-------------------------------------" ));
  msg (( MSG_OUT, "Data dump" ));
  msg (( MSG_OUT, "-------------------------------------" ));
  
  msg (( MSG_OUT, "Header [H]" ));
  msg (( MSG_OUT, "U32  0 = %4x", PtSrcW32AsPt[0] ));
  msg (( MSG_OUT, "U32  1 = %4x", PtSrcW32AsPt[1] ));
  msg (( MSG_OUT, "U32  2 = %4x", PtSrcW32AsPt[2] ));
  msg (( MSG_OUT, "U32  3 = %4x", PtSrcW32AsPt[3] ));
  msg (( MSG_OUT, "U32  4 = %4x", PtSrcW32AsPt[4] ));
  msg (( MSG_OUT, "U32  5 = %4x", PtSrcW32AsPt[5] ));
  msg (( MSG_OUT, "U32  6 = %4x", PtSrcW32AsPt[6] ));
  
  msg (( MSG_OUT, "Frame cnt [D]" ));
  msg (( MSG_OUT, "U32  7 = %4d", PtSrcW32AsPt[7] ));
  msg (( MSG_OUT, "U32  8 = %4d", PtSrcW32AsPt[8] ));
  msg (( MSG_OUT, "U32  9 = %4d", PtSrcW32AsPt[9] ));
  msg (( MSG_OUT, "U32 10 = %4d", PtSrcW32AsPt[10] ));
  msg (( MSG_OUT, "U32 11 = %4d", PtSrcW32AsPt[11] ));
  msg (( MSG_OUT, "U32 12 = %4d", PtSrcW32AsPt[12] ));
  msg (( MSG_OUT, "U32 13 = %4d", PtSrcW32AsPt[13] ));

  msg (( MSG_OUT, "Data length [D]" ));
  msg (( MSG_OUT, "U32  7 = %4x", PtSrcW32AsPt[14] ));
  msg (( MSG_OUT, "U32  8 = %4x", PtSrcW32AsPt[15] ));
  msg (( MSG_OUT, "U32  9 = %4x", PtSrcW32AsPt[16] ));
  msg (( MSG_OUT, "U32 10 = %4x", PtSrcW32AsPt[17] ));
  msg (( MSG_OUT, "U32 11 = %4x", PtSrcW32AsPt[18] ));
  msg (( MSG_OUT, "U32 12 = %4x", PtSrcW32AsPt[19] ));
  msg (( MSG_OUT, "U32 13 = %4x", PtSrcW32AsPt[20] ));
  
  msg (( MSG_OUT, "Data [H]" ));
  msg (( MSG_OUT, "U32 14 = %4x", PtSrcW32AsPt[21] ));
  msg (( MSG_OUT, "U32 15 = %4x", PtSrcW32AsPt[22] ));
  msg (( MSG_OUT, "U32 16 = %4x", PtSrcW32AsPt[23] ));
  msg (( MSG_OUT, "U32 17 = %4x", PtSrcW32AsPt[24] ));
  msg (( MSG_OUT, "U32 19 = %4x", PtSrcW32AsPt[25] ));
  msg (( MSG_OUT, "U32 20 = %4x", PtSrcW32AsPt[26] ));
  msg (( MSG_OUT, "U32 21 = %4x", PtSrcW32AsPt[27] ));
 
*/
  
  if ( VPtRunCont->ParMeasDataRate == 1  ) {
    
    if ( VPtRunCont->ResAcqCnt == 0 ) {  
      VPtRunCont->InfDataRateMeasTotalSz     = 0;
      VPtRunCont->InfDataRateMeasStartTimeMs = GetTickCount ();
      VPtRunCont->InfDataRateMeasStopTimeMs  = 0;
      VPtRunCont->InfDataRateMeasTotalTimeMs = 0;
      VPtRunCont->ResDataRateMBytesPerSec    = 0;
     }
    
    else {
      
      if ( (VPtRunCont->ResAcqCnt % VPtRunCont->ParAcqNbToMeasDataRate) == 0  ) {
        
        // Calculate data rate
        
        VPtRunCont->InfDataRateMeasStopTimeMs  = GetTickCount ();
        VPtRunCont->InfDataRateMeasTotalTimeMs = VPtRunCont->InfDataRateMeasStopTimeMs - VPtRunCont->InfDataRateMeasStartTimeMs;
        
        if ( VPtRunCont->InfDataRateMeasTotalTimeMs > 0 ) {
          VPtRunCont->ResDataRateMBytesPerSec =  1000 * ( (float) VPtRunCont->InfDataRateMeasTotalSz / (float) VPtRunCont->InfDataRateMeasTotalTimeMs ) / (float) ( 1024 * 1024 );
        }
        
        // msg (( MSG_OUT, "Data rate - ResAcqCnt=%d - Time=%d [ms] - Size=%d [Bytes] - DR=%.3f [MB/s]))", VPtRunCont->ResAcqCnt, VPtRunCont->InfDataRateMeasTotalTimeMs, VPtRunCont->InfDataRateMeasTotalSz, VPtRunCont->ResDataRateMBytesPerSec ));
        
        // Reset variables for next measure
        
        VPtRunCont->InfDataRateMeasTotalSz     = 0;
        VPtRunCont->InfDataRateMeasStopTimeMs  = 0;
        VPtRunCont->InfDataRateMeasTotalTimeMs = 0;
        VPtRunCont->InfDataRateMeasStartTimeMs = GetTickCount ();
      }
      
    }
    
  }
  

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

          case 8 : {
            EFRIO__MI26_FFRioEmulDeserData8Mi26EudetMode2 ( PtSrcW32AsPt, EltNb, &VEmuleFirstFrameNo, VEmuleFrameNb, EmuleMode  );
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

          case 8 : {
            EFRIO__MI26_FFRioEmulDeserData8Mi26EudetMode3 ( PtSrcW32AsPt, EltNb, &VEmuleFirstFrameNo, VEmuleFrameNb, EmuleMode  );
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

      
      // VRet = EFRIO__MI26_FFRioAcqDeserDataEudet2ModeNMi26 ( Mi26Nb, BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode, TriggerHandlingMode );

      
      switch ( Mi26Nb ) {
        
        case 1 : {
          VRet = EFRIO__MI26_FFRioAcqDeserDataEudet2Mode1Mi26 ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode, TriggerHandlingMode );
          break; }
        
        case 6 : {
          VRet = EFRIO__MI26_FFRioAcqDeserDataEudet2Mode6Mi26 ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode, TriggerHandlingMode );
          break; }

        case 8 : {
          VRet = EFRIO__MI26_FFRioAcqDeserDataEudet2Mode8Mi26 ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode, TriggerHandlingMode );
          break; }

        case 12 : {
          VRet = EFRIO__MI26_FFRioAcqDeserDataEudet2Mode12Mi26 ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode, TriggerHandlingMode );
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
          VRet = EFRIO__MI26_FFRioAcqDeserDataEudet3Mode1Mi26 ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode, TriggerHandlingMode );
          break; }
        
        case 6 : {
          VRet = EFRIO__MI26_FFRioAcqDeserDataEudet3Mode6Mi26 ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode, TriggerHandlingMode );
          break; }

        case 8 : {
          VRet = EFRIO__MI26_FFRioAcqDeserDataEudet3Mode8Mi26 ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode, TriggerHandlingMode );
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
  
  VPtCont->RunCont.ResAcqFunctRetCode = VRet;
  
  if ( VRet > 0 ) {
    VPtRunCont->InfDataRateMeasTotalSz += VRet;
  }  
  
  return (VRet);
  
#endif // NO_MI26
  
}
  
  
/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    : None
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    :
:
Level     :
Date      : 11/05/2011
Rev       : 
          :
Doc date  : 
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FJtagLoadFile ( char* FileName ) {

  SInt32               VRet;
  EFRIO__TContext*     VPtCont   = &EFRIO__VGContext;
  EFRIO__TRunCont*     VPtRun    = &VPtCont->RunCont;
  
  switch ( VPtRun->ParMapsName ) {
    
    case ASIC__MI26 : {
      VRet = EFRIO__MI26_FJtagLoadFile (FileName);
      break; }

    case ASIC__ULT1 : {
      VRet = EFRIO__ULT1_FJtagLoadFile (FileName);
      break; }

    default : {
      err_retfail ( -1, (ERR_OUT,"Unknown MAPS name = %d", VPtRun->ParMapsName) );
      break; }
    
  }
  
  return (VRet);  
}
  

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
          :
Goal      :
          :
Inputs    : None
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   :
:
Remark    :
:
Level     :
Date      : 09/11/2010
Rev       : 04/02/2011
          : - Use a local ( in function ) JTAG COM object instance, global used before
          : but this was incompatible with multithreading ( => application crash )
          :
Doc date  : 09/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__MI26_FJtagLoadFile ( char* FileName ) {
  
  HRESULT    VRetCode;
  WideString VStatus;
  WideString VFileName;

#ifdef EFRIO__INCLUDE_PARA_PORT
  TCOMIMI26MasterConf  VJtag;
  HRESULT              VHrComErr;
  WideString           VStrComStatus;
#endif

  

#ifdef EFRIO__INCLUDE_JTAG

  // COM handling
  
  VHrComErr = CoInitialize (NULL);
  
  if ( FAILED (VHrComErr) ) {
    err_retfail ( -1, (ERR_OUT,"CoInitialize failed !" ) );
  }
  
  VHrComErr =  CoMI26MasterConf::Create( VJtag );
  
  if ( FAILED (VHrComErr) ) {
    CoUninitialize();
    err_retfail ( -1, (ERR_OUT,"CoMI26MasterConf::Create failed !" ) );
  }
  

  if ( VJtag.IsBound () ) {
    
    VFileName = FileName;
    
    OleCheck( VRetCode = VJtag.MasterConfLoadFile( VFileName , &VStatus ) );
    
    if ( VRetCode != S_OK ) {
      CoUninitialize();
      err_retfail ( -1, (ERR_OUT,"Load JTAG file = %s failed !", FileName) );
    }
    
  }
  
  else {
    CoUninitialize();
    err_retfail ( -1, (ERR_OUT,"JTAG com interface problem") );
  }

  CoUninitialize();
  err_retok (( ERR_OUT, "" ));
  
#else
  err_warning (( ERR_OUT, "JTAG control disabled by conditionnal compilation" ));
  err_retok (( ERR_OUT, "" ));
  
#endif

}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    : None
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    :
:
Level     :
Date      : 11/05/2011
Rev       :
:
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FJtagReset ( ) {

  SInt32               VRet;
  EFRIO__TContext*     VPtCont   = &EFRIO__VGContext;
  EFRIO__TRunCont*     VPtRun    = &VPtCont->RunCont;


  switch ( VPtRun->ParMapsName ) {
    
    case ASIC__MI26 : {
      VRet = EFRIO__MI26_FJtagReset ();
      break; }
    
    case ASIC__ULT1 : {
      VRet = EFRIO__ULT1_FJtagReset ();
      break; }
    
    default : {
      err_retfail ( -1, (ERR_OUT,"Unknown MAPS name = %d", VPtRun->ParMapsName) );
      break; }
    
  }

  return (VRet);
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    : None
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    :
:
Level     :
Date      : 09/11/2010
Rev       : 04/02/2011
          : - Use a local ( in function ) JTAG COM object instance, global used before
          : but this was incompatible with multithreading ( => application crash )
          :
Doc date  : 09/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__MI26_FJtagReset ( ) {
  
  HRESULT    VRetCode;
  WideString VStatus;

#ifdef EFRIO__INCLUDE_JTAG
  TCOMIMI26MasterConf  VJtag;
  HRESULT              VHrComErr;
  WideString           VStrComStatus;
#endif


#ifdef EFRIO__INCLUDE_JTAG

  // COM handling
  
  VHrComErr = CoInitialize (NULL);
  
  if ( FAILED (VHrComErr) ) {
    err_retfail ( -1, (ERR_OUT,"CoInitialize failed !" ) );
  }
  
  VHrComErr =  CoMI26MasterConf::Create( VJtag );
  
  if ( FAILED (VHrComErr) ) {
    CoUninitialize();
    err_retfail ( -1, (ERR_OUT,"CoMI26MasterConf::Create failed !" ) );
  }
  
  if ( VJtag.IsBound () ) {
        
    OleCheck( VRetCode = VJtag.MasterConfReset (&VStatus) );
    
    if ( VRetCode != S_OK ) {
      CoUninitialize();
      err_retfail ( -1, (ERR_OUT,"JTAG -> Reset chip failed !") );
    }
    
  }
  
  else {
    CoUninitialize();
    err_retfail ( -1, (ERR_OUT,"JTAG com interface problem") );
  }

  CoUninitialize();
  err_retok (( ERR_OUT, "" ));
  
#else
  err_warning (( ERR_OUT, "JTAG control disabled by conditionnal compilation" ));
  err_retok (( ERR_OUT, "" ));
#endif

}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    : None
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    :
:
Level     :
Date      : 11/05/2011
Rev       :
:
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FJtagLoadChip ( ) {
  
  SInt32               VRet;
  EFRIO__TContext*     VPtCont   = &EFRIO__VGContext;
  EFRIO__TRunCont*     VPtRun    = &VPtCont->RunCont;
  
  
  switch ( VPtRun->ParMapsName ) {
    
    case ASIC__MI26 : {
      VRet = EFRIO__MI26_FJtagLoadChip ();
      break; }
    
    case ASIC__ULT1 : {
      VRet = EFRIO__ULT1_FJtagLoadChip ();
      break; }
    
    default : {
      err_retfail ( -1, (ERR_OUT,"Unknown MAPS name = %d", VPtRun->ParMapsName) );
      break; }
    
  }
  
  return (VRet);
  
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    : None
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
: -n = error number on JTAG readback
:
Globals   :
:
Remark    :
:
Level     :
Date      : 09/11/2010
Rev       : 04/02/2011
          : - Use a local ( in function ) JTAG COM object instance, global used before
          : but this was incompatible with multithreading ( => application crash )
          :
          : 28/09/2011
          : - Update code because MasterConfReadBack (...) has changed => return one more param = Reaad back errors nb
          :   If <> 0 return -(errors number)
          :
Doc date  : 09/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__MI26_FJtagLoadChip ( ) {
  
  HRESULT    VRetCode;
  WideString VStatus;
  long       VReadBackErrorsNb;

#ifdef EFRIO__INCLUDE_JTAG
  TCOMIMI26MasterConf  VJtag;
  HRESULT              VHrComErr;
  WideString           VStrComStatus;
#endif



#ifdef EFRIO__INCLUDE_JTAG
  
  // COM handling
  
  VHrComErr = CoInitialize (NULL);
  
  if ( FAILED (VHrComErr) ) {
    err_retfail ( -1, (ERR_OUT,"CoInitialize failed !" ) );
  }
  
  VHrComErr =  CoMI26MasterConf::Create( VJtag );
  
  if ( FAILED (VHrComErr) ) {
    CoUninitialize();
    err_retfail ( -1, (ERR_OUT,"CoMI26MasterConf::Create failed !" ) );
  }
  
  if ( VJtag.IsBound () ) {
    
    OleCheck( VRetCode = VJtag.MasterConfUpdateAll (&VStatus) );
    
    if ( VRetCode != S_OK ) {
      CoUninitialize();
      err_retfail ( -1, (ERR_OUT,"JTAG -> Load chip parameters failed !") );
    }

    // 28/09/2011
    // Update code because MasterConfReadBack (...) has changed => return one more param = Reaad back errors nb
    // If <> 0 return -(errors number)

    OleCheck( VRetCode = VJtag.MasterConfReadBack ( &VReadBackErrorsNb, &VStatus) );
    
    if ( VRetCode != S_OK ) {
      CoUninitialize();
      err_retfail ( -1, (ERR_OUT,"JTAG -> Read back chip parameters failed !") );
    }

    if ( VReadBackErrorsNb > 0 ) {
      err_retfail ( -VReadBackErrorsNb, (ERR_OUT,"JTAG -> Read back chip => Write <> Read => %d errors !", VReadBackErrorsNb ) );
    }

    
  }
  
  else {
    CoUninitialize();
    err_retfail ( -1, (ERR_OUT,"JTAG com interface problem") );
  }

  CoUninitialize();
  err_retok (( ERR_OUT, "" ));
  
#else
  err_warning (( ERR_OUT, "JTAG control disabled by conditionnal compilation" ));
  
#endif
  
  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    : None
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    :
:
Level     :
Date      : 11/05/2011
Rev       :
:
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FJtagStartChip ( ) {

  SInt32               VRet;
  EFRIO__TContext*     VPtCont   = &EFRIO__VGContext;
  EFRIO__TRunCont*     VPtRun    = &VPtCont->RunCont;
  
  
  switch ( VPtRun->ParMapsName ) {
    
    case ASIC__MI26 : {
      VRet = EFRIO__MI26_FJtagStartChip ();
      break; }
    
    case ASIC__ULT1 : {
      VRet = EFRIO__ULT1_FJtagStartChip ();
      break; }
    
    default : {
      err_retfail ( -1, (ERR_OUT,"Unknown MAPS name = %d", VPtRun->ParMapsName) );
      break; }
    
  }
  
  return (VRet);
  
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    : None
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    :
:
Level     :
Date      : 09/11/2010
Rev       : 04/02/2011
          : - Use a local ( in function ) JTAG COM object instance, global used before
          : but this was incompatible with multithreading ( => application crash )
          :
Doc date  : 09/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__MI26_FJtagStartChip ( ) {
  
  HRESULT    VRetCode;
  WideString VStatus;

#ifdef EFRIO__INCLUDE_JTAG
  TCOMIMI26MasterConf  VJtag;
  HRESULT              VHrComErr;
  WideString           VStrComStatus;
#endif


#ifdef EFRIO__INCLUDE_JTAG

  // COM handling
  
  VHrComErr = CoInitialize (NULL);
  
  if ( FAILED (VHrComErr) ) {
    err_retfail ( -1, (ERR_OUT,"CoInitialize failed !" ) );
  }
  
  VHrComErr =  CoMI26MasterConf::Create( VJtag );
  
  if ( FAILED (VHrComErr) ) {
    CoUninitialize();
    err_retfail ( -1, (ERR_OUT,"CoMI26MasterConf::Create failed !" ) );
  }
  
  if ( VJtag.IsBound () ) {
    
    OleCheck( VRetCode = VJtag.MasterConfStart (&VStatus) );
    
    if ( VRetCode != S_OK ) {
      CoUninitialize();
      err_retfail ( -1, (ERR_OUT,"JTAG -> Load chip parameters failed !") );
    }
    
  }
  
  else {
    CoUninitialize();
    err_retfail ( -1, (ERR_OUT,"JTAG com interface problem") );
  }

  CoUninitialize();
  err_retok (( ERR_OUT, "" ));

#else

  err_warning (( ERR_OUT, "JTAG control disabled by conditionnal compilation" ));
  err_retok (( ERR_OUT, "" ));
  
#endif

}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    : None
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    :
:
Level     :
Date      : 11/05/2011
Rev       :
:
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FHwStartChip ( SInt32 SpareS32Par ) {

  SInt32               VRet;
  EFRIO__TContext*     VPtCont   = &EFRIO__VGContext;
  EFRIO__TRunCont*     VPtRun    = &VPtCont->RunCont;
  
  
  switch ( VPtRun->ParMapsName ) {
    
    case ASIC__MI26 : {
      VRet = EFRIO__MI26_FHwStartChip ( SpareS32Par );
      break; }
    
    case ASIC__ULT1 : {
      VRet = EFRIO__ULT1_FHwStartChip ( SpareS32Par );
      break; }
    
    default : {
      err_retfail ( -1, (ERR_OUT,"Unknown MAPS name = %d", VPtRun->ParMapsName) );
      break; }
    
  }
  
  return (VRet);
  
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    : None
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    :
:
Level     :
Date      : 10/11/2010
Doc date  : 10/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__MI26_FHwStartChip ( SInt32 SpareS32Par ) {
  
  err_warning (( ERR_OUT, "EFRIO__MI26_FHwStartChip (Par=%d)", SpareS32Par ));
  
  // Start = D6, Speak = D7*
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FOutD6 ( 0 /* Id */, 0  );
    PPO_FOutD6 ( 0 /* Id */, 1  );
    PPO_FOutD6 ( 0 /* Id */, 1  );
    PPO_FOutD6 ( 0 /* Id */, 1  );
    PPO_FOutD6 ( 0 /* Id */, 1  );
    PPO_FOutD6 ( 0 /* Id */, 1  );
    PPO_FOutD6 ( 0 /* Id */, 1  );
    PPO_FOutD6 ( 0 /* Id */, 1  );
    PPO_FOutD6 ( 0 /* Id */, 1  );
    PPO_FOutD6 ( 0 /* Id */, 1  );
    PPO_FOutD6 ( 0 /* Id */, 1  );
    
    Sleep ( 100 ); // 08/06/2011 => Debug for test with Flex RIO trig emulator
    
    PPO_FOutD6 ( 0 /* Id */, 0  );
  
  #else
    err_warning (( ERR_OUT, "HW start not done -> // port not enabled !" ));
  #endif

  err_retok (( ERR_OUT, "" ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : EFRIO__F
:
Goal      :
:
Inputs    : None
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    :
:
Level     :
Date      : 19/05/2011
Rev       :
:
Doc date  : /2011
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__MI26_FTestOnDataGetJtagRef () {
  
  SInt32                  VRet;
  EFRIO__TTestOnDataCont* VPtTestCont = &EFRIO__VGContext.TestOnDataCont;
  
  HRESULT    VRetCode;
  WideString VStatus;
  
#ifdef EFRIO__INCLUDE_JTAG
  TCOMIMI26MasterConf  VJtag;
  HRESULT              VHrComErr;
  WideString           VStrComStatus;
#endif
  
  SInt32 VRegValFromMi26;
  SInt32 VLowHeaderFromJtag;
  SInt32 VHighHeaderFromJtag;
  SInt32 VLowTrailerFromJtag;
  SInt32 VHighTrailerFromJtag;
  
  SInt8  ViMaps;


#ifdef EFRIO__INCLUDE_JTAG
  
  // COM handling
  
  VHrComErr = CoInitialize (NULL);
  
  if ( FAILED (VHrComErr) ) {
    err_retfail ( -1, (ERR_OUT,"CoInitialize failed !" ) );
  }
  
  VHrComErr =  CoMI26MasterConf::Create( VJtag );
  
  if ( FAILED (VHrComErr) ) {
    CoUninitialize();
    err_retfail ( -1, (ERR_OUT,"CoMI26MasterConf::Create failed !" ) );
  }
  
  if ( VJtag.IsBound () ) {
    
    for ( ViMaps=0; ViMaps < VPtTestCont->ParMapsNb; ViMaps++ ) {
      
      // Sel Mi26
      
      OleCheck( VRetCode = VJtag.MasterConfSetDevNum ( ViMaps, &VStrComStatus ) );
      
      if ( VRetCode != S_OK ) {
        CoUninitialize();
        err_retfail ( -1, (ERR_OUT,"Select Maps Id = %d failed !", ViMaps) );
      }
      
      OleCheck( VRetCode = VJtag.Mimosa26ConfGetHeaderTrailer ( 0 /* RegId */, &VHighTrailerFromJtag, &VRegValFromMi26, &VStrComStatus) );
      OleCheck( VRetCode = VJtag.Mimosa26ConfGetHeaderTrailer ( 1 /* RegId */, &VLowTrailerFromJtag , &VRegValFromMi26, &VStrComStatus ) );
      OleCheck( VRetCode = VJtag.Mimosa26ConfGetHeaderTrailer ( 2 /* RegId */, &VHighHeaderFromJtag , &VRegValFromMi26, &VStrComStatus ) );
      OleCheck( VRetCode = VJtag.Mimosa26ConfGetHeaderTrailer ( 3 /* RegId */, &VLowHeaderFromJtag  , &VRegValFromMi26, &VStrComStatus ) );
      
      VPtTestCont->ParAMapsHeaderRef[ViMaps]  = VLowHeaderFromJtag  + (VHighHeaderFromJtag << 16); // Mimosa 26 header field
      VPtTestCont->ParAMapsTrailerRef[ViMaps] = VLowTrailerFromJtag + (VHighTrailerFromJtag << 16); // Mimosa 26 trailer field
      
    }
    
  }
  
  else {
    
    for ( ViMaps=0; ViMaps < VPtTestCont->ParMapsNb; ViMaps++ ) {
      VPtTestCont->ParAMapsHeaderRef[ViMaps]  = 0; // Mimosa 26 header field
      VPtTestCont->ParAMapsTrailerRef[ViMaps] = 0; // Mimosa 26 trailer field
    }
    
    CoUninitialize();
    err_retfail ( -1, (ERR_OUT,"JTAG com interface problem") );
  }
  

  CoUninitialize();

#else
  err_warning (( ERR_OUT, "JTAG control disabled by conditionnal compilation" ));
  err_retok (( ERR_OUT, "" ));  
#endif
  

  err_retok (( ERR_OUT, "" ));
}
  

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : EFRIO__F
:
Goal      :
:
Inputs    : None
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    :
:
Level     :
Date      : 29/04/2011
          :
Rev       : 19/05/2011
          : - Handle both Mi26 and Ult1
:
Doc date  : /2011
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FTestOnDataStartStop ( SInt8 Start, SInt8 MapsNbToTest, SInt8 PrintLvl ) {
  
  SInt32                  VRet;
  EFRIO__TRunCont*        VPtRunCont  = &EFRIO__VGContext.RunCont;
  EFRIO__TTestOnDataCont* VPtTestCont = &EFRIO__VGContext.TestOnDataCont;
    
  SInt32 VRegValFromMi26;
  SInt32 VLowHeaderFromJtag;
  SInt32 VHighHeaderFromJtag;
  SInt32 VLowTrailerFromJtag;
  SInt32 VHighTrailerFromJtag;
  
  SInt8  ViMaps;



  VPtTestCont->ParPrintLvl = PrintLvl;


  // Stop and exit

  if ( Start == 0 ) {
    
    // If already stopped => Do nothing & exit
    
    if ( VPtTestCont->ParModeEnable == 0 ) {
      return (0);
    }
    
    // If running => Stop + message
    
    VPtTestCont->ParModeEnable = 0;

    // Print errors count
  
    EFRIO__FPrintTestOnDataCont ( "Errors count after Stop" );
  
    err_retok (( ERR_OUT, "Stop command executed" ));
  }


  // Start procedure

  // If already running => Do nothing & exit
    
  if ( VPtTestCont->ParModeEnable == 1 ) {
    return (0);
  }
  
  // If not running => Start + Message
  
  memset ( VPtTestCont, 0, sizeof (EFRIO__TTestOnDataCont) );
  
  VPtTestCont->ParMapsNb       = VPtRunCont->ParMi26Nb;
  VPtTestCont->ParMapsNbToTest = MapsNbToTest;
  
  
  switch ( VPtRunCont->ParMapsName ) {
    
    case ASIC__MI26 : {
      VRet = EFRIO__MI26_FTestOnDataGetJtagRef ();
      break; }
    
    case ASIC__ULT1 : {
      VRet = EFRIO__ULT1_FTestOnDataGetJtagRef ();
      break; }
    
    default : {
      err_retfail ( -1, (ERR_OUT,"Unknown MAPS name = %d", VPtRunCont->ParMapsName) );
      break; }
    
  }
  
  err_retfail ( VRet, (ERR_OUT,"Test aborted => Unable to get param from JTAG") );
  
  // Enable test
  
  VPtTestCont->ParModeEnable = 1;
  
  // Print test context
  
  EFRIO__FPrintTestOnDataCont ( "Record state after Start" );
    
  err_retok (( ERR_OUT, "Start command executed" ));
    
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : EFRIO__F
:
Goal      :
:
Inputs    : None
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Remark    :
:
Level     :
Date      : 29/04/2011
Rev       :
:
Doc date  : /2011
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FTestOnDataResetErrCnt () {
  
  SInt32                  VRet;
  EFRIO__TRunCont*        VPtRunCont  = &EFRIO__VGContext.RunCont;
  EFRIO__TTestOnDataCont* VPtTestCont = &EFRIO__VGContext.TestOnDataCont;
  SInt8                   ViMaps;
  

  // Print test context => To get errors count before reset
  
  EFRIO__FPrintTestOnDataCont ( "Errors count before Reset" );    

  for ( ViMaps=0; ViMaps < VPtTestCont->ParMapsNb; ViMaps++ ) {
    
    VPtTestCont->ResAMapsErrCnt[ViMaps]           = 0;
    VPtTestCont->ResAMapsHeaderErrCnt[ViMaps]     = 0; // Mimosa 26 header field
    VPtTestCont->ResAMapsFrameCntErrCnt[ViMaps]   = 0; // Mimosa 26 frame counter field
    VPtTestCont->ResAMapsDataLengthErrCnt[ViMaps] = 0; // Mimosa 26 data length in BYTES -> It's final result NOT the DataLength FIELD from data stream
    VPtTestCont->ResAMapsTrailerErrCnt[ViMaps]    = 0; // Mimosa 26 trailer field
    
  }

  VPtTestCont->ResTotErrCnt      = 0;
  VPtTestCont->ResFrameNbWithErr = 0;

  return (0);
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet3Mode1Mi26 (
          :   SInt32 BoardId , UInt32*  PtSrcW32, SInt32 EltNb,
          :   SInt32 AcqStatus, SInt32 TrigStatus,  SInt8 DataConvertMode )
          :
Goal      : Flex RIO readout function for one Mi26 in EUDET mode 3
          :
          : Read data of one acquisition from Flex RIO, format them in EUDET mode 3
          : by adding extra information and fill PC RAM buffer.
          :
          : The extra channel is enabled in EUDET mode 3, therefore for each trigger
          : two informations are stored
          :
          : - The TLU trigger                    -> see record EFRIO__TTluTrigger
          : - The Flex RIO trigger / time stamp  -> see record EFRIO__TFlexRioTimeStamp1
          :
          :***************************************************************************
          : Only the frames with a trigger nb > 0 + EFRIO__FRAME_NB_TO_READ_AFTER_TRIG
          : frames after trigger are stored in memory.
          : The selection is done by this function.
          :***************************************************************************
          :
Inputs    : BoardId             - Board identifier
          : PtSrcW32            - Pointer to board data ( output of Labview Vi )
          : EltNb               - Board data size in W32 unit ( 1 Elt = 1 W32 )
          : AcqStatus           - Acquisition status flag provided by labview
          : TrigStatus          - Trigger status flag provided by labview
          :
          : DataConvertMode     - Not used => Global EFRIO__TRunCont.ParDataTransferMode is used
          :
          : TriggerHandlingMode - Reserved for future use
          :          
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
          : 16/02/2011
          : - Update ResEventCnt = ResFrameCnt, set to 0 before
          :
          :
          : 23/02/2011
          : - Modify types of AcqStatus, TrigStatus parameters to SInt32
          :
          : 24/02/2011
          : - Update new fields AcqStatus, TrigStatus of AAcqFrameList
          :
          : 12/03/2011
          : - Update fields ResAcqCnt, ResFrameCnt, ResEventCnt in a different way
          : -- incement ResAcqCnt ONLY if there is at least one trigger in acq
          : -- ResFrameCnt = number of frames with trigger + N following one
          : -- ResEventCnt = ResFrameCnt
          :
          :
          : - Add parameter TriggerHandlingMode
          : - Always take first EFRIO__FRAME_NB_TO_READ_AFTER_TRIG frames if TriggerHandlingMode = 1          
          :          
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

#ifndef NO_MI26


SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet3Mode1Mi26 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode ) {
  
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
//  SInt32               VRunFrameCnt;
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
  
  
  
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FOutD5 ( 0 /* Id */, 1 /* State */ );
  #endif
  
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

  VPtFrList->AcqStatus  = AcqStatus;
  VPtFrList->TrigStatus = TrigStatus;
  VPtFrList->TotFrameNb = 0;
  
  memset ( VPtFrList->AFramePtr, 0, EFRIO__MAX_FRAME_NB_PER_ACQ * sizeof (EFRIO__TFrame*) );
            
  // Calculate size of records WITHOUT variable length part counted here as one elt
  
  VEmptyTrigRecSz  = sizeof ( EFRIO__TTriggerRec );
  VEmptyFrameRecSz = sizeof ( EFRIO__TFrame );
  
  
  // Reset tmp triggers buffer
  
  memset ( VPtTmpTrigRec, 0, sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );
  
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FOutD5 ( 0 /* Id */, 0 /* State */ );
  #endif
  
  // ----------------------------------------------------------------------------------
  // List frames to extract => frame with trigger + 3 following frames
  // ----------------------------------------------------------------------------------
  
  memset ( VPtCont->AAAcqFrameWithTrigList[0], 0xFF, EFRIO__MAX_FRAME_NB_PER_ACQ * 2 );
  
  // Init loop parameters
  
  VFrameWithTrigCnt    =  0;
  VLastFrameWithTrig   = -1;
  VPtFrameWithTrigList = VPtCont->AAAcqFrameWithTrigList[0];


  // If trigger mode = 1 => Take always first frame of acq EFRIO__FRAME_NB_TO_READ_AFTER_TRIG
  //
  // Done by overwriting VFrNbToTakeAtBeginOfAcqForLastTrig ( set on previous acq ) with EFRIO__FRAME_NB_TO_READ_AFTER_TRIG
  
  if ( TriggerHandlingMode == 1 ) {
    VFrNbToTakeAtBeginOfAcqForLastTrig = EFRIO__FRAME_NB_TO_READ_AFTER_TRIG;
  }
  
  
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
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FOutD5 ( 0 /* Id */, 1 /* State */ );
  #endif
    
  
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
      
    }
    
  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtFrame->Tag        = EFRIO__FRAME_TAG;
    VPtFrame->Header.Tag                = EFRIO__FRAME_TAG_HEADER;
  #endif
  
  
    VPtFrame->Header.AcqStatus          = AcqStatus;
    VPtFrame->Header.TrigStatus         = TrigStatus;  
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
    
    if ( VTrigNb > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ) {
      err_warning (( ERR_OUT, "VTrigNb=%d > Max=%d => Limit to max !", VTrigNb, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ));
      VTrigNb = EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26;
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
    
    // Check frame relevant fields ( DataLength, Header, frame counter, trailer => NOT data part )
    // Enable / Disable done by a call to EFRIO__FTestOnDataStartStop ( Start, PrintLvl )
    
    EFRIO__MI26_FChkFrameLight ( 0 /* FuncId */ , VFrameId, VPtFrame, 1 /* Mi26Nb */ );
    
    
    ++VPtFrList->TotFrameNb;
    
    // ++VRunFrameCnt;
    
  } // End for ViFrameWithTrig
  

    /* Before 12/03/2011
    
    ++VPtCont->RunCont.ResAcqCnt;
    VPtCont->RunCont.ResFrameCnt = VPtCont->RunCont.ResAcqCnt * VPtCont->RunCont.ParFrameNbPerAcq;
    VPtCont->RunCont.ResEventCnt = VPtCont->RunCont.ResFrameCnt;
    
    */
    
    if ( VPtFrList->TotFrameNb != 0 ) {
      ++VPtCont->RunCont.ResAcqCnt;
      VPtCont->RunCont.ResFrameCnt = VPtCont->RunCont.ResFrameCnt + VPtFrList->TotFrameNb;
      VPtCont->RunCont.ResEventCnt = VPtCont->RunCont.ResFrameCnt;
    }
    


  
    #ifdef EFRIO__INCLUDE_PARA_PORT
      PPO_FOutD5 ( 0 /* Id */, 0 /* State */ );
    #endif
     
    
    return (VTotAcqSz);
  }
  
#endif // NO_MI26

#endif
