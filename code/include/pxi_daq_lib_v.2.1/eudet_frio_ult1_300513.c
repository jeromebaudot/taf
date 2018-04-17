
/*******************************************************************************
File      : x:\lib\win\eudet_frio\eudet_frio_ult1.c
Goal      : Ultimate 1 functions of flex rio board library for EUDET
Prj date  : 11/05/2011
File date : 11/05/2011
Doc date  : 
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


#ifndef EUDET_FRIO_ULT1_C
#define EUDET_FRIO_ULT1_C


// #define EFRIO__ULT1_BUG_FW_TRIG_DELAYED_BY_TWO_FRAMES


/*
#define ERR_LOG_LVL_NONE             0
#define ERR_LOG_LVL_ALL              1
#define ERR_LOG_LVL_WARINGS_ERRORS   2
#define ERR_LOG_LVL_WARNINGS_ERRORS  2
#define ERR_LOG_LVL_ERRORS           3
*/



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

SInt32 EFRIO__ULT1_FTestOnDataGetJtagRef () {
  
  SInt32                  VRet;
  EFRIO__TTestOnDataCont* VPtTestCont = &EFRIO__VGContext.TestOnDataCont;
  
  HRESULT    VRetCode;
  WideString VStatus;
  
  #ifdef EFRIO__INCLUDE_JTAG
    TCOMIMI28COM  VJtag;
    HRESULT       VHrComErr;
    WideString    VStrComStatus;
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
  
  VHrComErr =  CoMI28COM::Create( VJtag );
  
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
      
      OleCheck( VRetCode = VJtag.Mimosa28ConfGetHeaderTrailer ( 0 /* RegId */, &VHighTrailerFromJtag, &VRegValFromMi26, &VStrComStatus) );
      OleCheck( VRetCode = VJtag.Mimosa28ConfGetHeaderTrailer ( 1 /* RegId */, &VLowTrailerFromJtag , &VRegValFromMi26, &VStrComStatus ) );
      OleCheck( VRetCode = VJtag.Mimosa28ConfGetHeaderTrailer ( 2 /* RegId */, &VHighHeaderFromJtag , &VRegValFromMi26, &VStrComStatus ) );
      OleCheck( VRetCode = VJtag.Mimosa28ConfGetHeaderTrailer ( 3 /* RegId */, &VLowHeaderFromJtag  , &VRegValFromMi26, &VStrComStatus ) );
      
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
Date      : 19/05/2011 ( DPXI version of 20/08/2009 moved here )
Rev       : 15/05/2013
          : - Modification of frame counter test, the test Fr(n+1) = Fr(n) + 1 has been
          : removed because in beam test (EUDET3 mode) the frames in one acquisition are
          : not all consecutives, there it detects fake errors. Now the test is only done
          : by a comparison of all Mimosa 28 frame couters frame by frame.
Doc date  : 
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__ULT1_FChkFrameLight ( SInt16 FuncId, SInt32 CurFrame, EFRIO__TFrame* PtFrame, SInt8 Mi26Nb ) {
  
  
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
  
  // Reset errors flag
  
  VErrors = 0;
  
  // Check frame cnt & header & trailer

  // Store frame counter of first Mi28 to use it as a reference value
  
  VPtTestCont->InfMapsFrameCntRef = VPtFrHd->AMapsFrameCnt[0]; // 15/05/2013
  
  for ( ViMi26=0; ViMi26 < VMi26Nb; ViMi26++ ) {
    
    // Check frame counter => Compare all Mimosa 28 frame counters to the one of Mimosa28 [0]
    // This is done frame by frame = doesn't care about previous frame counter value - 15/05/2013
      
    if ( VPtFrHd->AMapsFrameCnt[ViMi26] != VPtTestCont->InfMapsFrameCntRef ) {
      
      ++VPtTestCont->ResAMapsFrameCntErrCnt[ViMi26];
      ++VPtTestCont->ResAMapsErrCnt[ViMi26];
      ++VPtTestCont->ResTotErrCnt;
      VErrors = 1;
      
      if ( VPtTestCont->ParPrintLvl == 1 ) {
        msg (( MSG_OUT, "B - Frame cnt error [Acq %.4d - Frame in Acq %.4d - Mi26 No %d] : Get %.6d - Must be %.6d ", VPtFrHd->AcqId, VPtFrHd->FrameIdInAcq, ViMi26, VPtFrHd->AMapsFrameCnt[ViMi26], VPtTestCont->InfMapsFrameCntRef ));
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
  
  // 25/04/2013
  
  VPtTestCont->ResErrOnCurrentAcq = VErrors;
  
  return (VErrors);
}




/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__ULT1_FFRioAcqDeserDataEudet1Mode1Ult1 (
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

SInt32 EFRIO__ULT1_FFRioAcqDeserDataEudet1Mode1Ult1 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode ) {
  
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
  SInt32               VErrorsOnData; // 26/04/2013
  
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FOutD5 ( 0 /* Id */, 1 /* State */ );
  #endif
  
  if ( VPtCont->InfInitDone == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort because : Init not done") );
  }
  
  err_trace (( ERR_OUT, "EFRIO__ULT1_FFRioAcqDeserDataEudet1Mode1Ult1 (P=%x, EltNb=%d)", PtSrcW32, EltNb ));
  
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
      
      // 25/04/2013
      
      err_retfail ( -2, (ERR_OUT,"Abort because bad data size !") );      
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
    
    EFRIO__ULT1_FChkFrameLight ( 0 /* FuncId */ , ViFrame, VPtFrame, 1 /* Mi26Nb */ );
    
    
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
Prototype : SInt32 EFRIO__ULT1_FFRioAcqDeserDataEudet1Mode6Ult1 (
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

SInt32 EFRIO__ULT1_FFRioAcqDeserDataEudet1Mode6Ult1 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode ) {
  
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
  SInt32               VErrorsOnData; // 26/04/2013
  
  
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
      
      // 25/04/2013
      
      err_retfail ( -2, (ERR_OUT,"Abort because bad data size !") );      
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
    
    EFRIO__ULT1_FChkFrameLight ( 0 /* FuncId */ , ViFrame, VPtFrame, 6 /* Mi26Nb */ );
    
    
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
Prototype : SInt32 EFRIO__ULT1_FFRioAcqDeserDataEudet2Mode1Ult1 (
          :   SInt32 BoardId , UInt32*  PtSrcW32, SInt32 EltNb,
          :   SInt32 AcqStatus, SInt32 TrigStatus,  SInt8 DataConvertMode )
          :
Goal      : Flex RIO readout function for one Ultimate 1 in EUDET mode 2
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
          : 11/05/2011
          : - Convert function fromM26 readout to Ultimate 1
          :
          : 09/06/2011
          : - Fix bug in "end of frame" triggers info calculation => 32 clk / line not 16 like Mi26 
          :
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__ULT1_FFRioAcqDeserDataEudet2Mode1Ult1 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode ) {
  
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
  SInt32               VErrorsOnData; // 26/04/2013
  
   
  
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
  
  VFrameNbFromBoardDrv = ( EltNb / ULT1__ZS_FFRAME_MODE_2X160MHZ_W16_SZ ) / 2; // Divide by 2 because of extra channel  
  
  // It must be => ( (EltNb * 2) / (ULT1__ZS_FFRAME_MODE_2X160MHZ_W16_SZ * 2) );
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
    VPtFrame->Header.MapsName           = (UInt16) ASIC__ULT1;
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
    
    if ( (VDbgDataLenghtD0 > ULT1__ZS_FFRAME_MODE_2X160MHZ_MAX_DATA_PART_W16_SZ) || (VDbgDataLenghtD1 > ULT1__ZS_FFRAME_MODE_2X160MHZ_MAX_DATA_PART_W16_SZ) ) {
      VDataLengthW16 = 0;
      ++VDataLengthErrCnt;
      err_error (( ERR_OUT, "HW error on data length - ViFrame=%d -> D0=%4d [D] - D1=%4d [D] -> Force 0 !", ViFrame, VDbgDataLenghtD0, VDbgDataLenghtD1 ));
      err_error (( ERR_OUT, "HW error on data length - ViFrame=%d -> D0=%4x [H] - D1=%4x [H] -> Force 0 !", ViFrame, VDbgDataLenghtD0, VDbgDataLenghtD1 ));
      
      // 25/04/2013
            
      err_retfail ( -2, (ERR_OUT,"Abort because bad data size !") );            
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
    
    // ViSrcW32 = ViSrcW32 + ( ULT1__ZS_FFRAME_RAW_MAX_W32 - VDataLengthW32 );
    
    ViSrcW32 = ViSrcW32BeforeDataCpyLoop + ( 2 * ULT1__ZS_FFRAME_RAW_MAX_W32 );
    
    
    VPtFrame->Header.AMapsTrailer[0] = PtSrcW32[(2 * ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame) + (2 * (3 + VDataLengthW32))];    // 3 = Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32; // Count Trailer field
    ++ViSrcW32; // Count extra channel trigger field
    
    // Read trigger number from "Mi26" frame
    
    if ( TrigStatus == 0 ) {
      VZero = PtSrcW32[(2 * ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame) + (2 * (3 + ULT1__ZS_FFRAME_RAW_MAX_W32 + 1))];
    }
    
    // Emulate trigger by overwriting info from "Mi26" frame
    
    else {
      VZero = (TrigStatus << 16);
    }
    
    ++ViSrcW32; // Count Zero field
    ++ViSrcW32; // Count extra channel trigger field
    
    VZero2 = PtSrcW32[(2 * ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame) + (2 * (3 + ULT1__ZS_FFRAME_RAW_MAX_W32 + 2))];
    ++ViSrcW32; // Count Zero2 field
    ++ViSrcW32; // Count extra channel trigger field
    
    // Extraction of trigger info from "Mimosa 26 frame" zero fields
    
    VTrigNb = (VZero  & 0xFFFF0000) >> 16;

    if ( VTrigNb > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ) {
      err_error (( ERR_OUT, "VTrigNb=%d > Max=%d => Limit to max compatible with Mi26 !", VTrigNb, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ));
      VTrigNb = EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26;
    }
    
    // 09/06/2011
    // => Bug fixed : 32 clock / line for Ultimate, not 16 like Mi26

    if ( VTrigNb != 0 ) {
      VATrigVal[0]    = (VZero  & 0x0000FFFF);
      VATrigVal[1]    = (VZero2 & 0xFFFF0000) >> 16;
      VATrigVal[2]    = (VZero2 & 0x0000FFFF);
      
      VATrigLine[0]   = VATrigVal[0] / 32;
      VATrigLine[1]   = VATrigVal[1] / 32;
      VATrigLine[2]   = VATrigVal[2] / 32;
      
      VATrigClk[0]    = VATrigVal[0] % 32;
      VATrigClk[1]    = VATrigVal[1] % 32;
      VATrigClk[2]    = VATrigVal[2] % 32;
      
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
    
    EFRIO__ULT1_FChkFrameLight ( 0 /* FuncId */ , ViFrame, VPtFrame, 1 /* Mi26Nb */ );
    
    
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
Prototype : SInt32 EFRIO__ULT1_FFRioAcqDeserDataEudet2Mode6Ult1 (
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
          :
          : 19/05/2011
          : - Convert function fromM26 readout to Ultimate 1
          :
          : 09/06/2011
          : - Fix bug in "end of frame" triggers info calculation => 32 clk / line not 16 like Mi26
          :          
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */




SInt32 EFRIO__ULT1_FFRioAcqDeserDataEudet2Mode6Ult1__fion_rewritten ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode ) {
  
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
  SInt32               VErrorsOnData; // 26/04/2013
  
  
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
  
  VFrameNbFromBoardDrv = ( EltNb / ULT1__ZS_FFRAME_MODE_2X160MHZ_W16_SZ ) / 7; // Divide by 7 because of extral channel
  
  // It must be => ( (EltNb * 2) / (ULT1__ZS_FFRAME_MODE_2X160MHZ_W16_SZ * 2) );
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
    VPtFrame->Header.MapsName     = (UInt16) ASIC__ULT1;
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
    
    
    
    if ( VDataLengthW16Max > ULT1__ZS_FFRAME_RAW_MAX_W16 ) {
      
      ++VDataLengthErrCnt;
      err_error (( ERR_OUT, "---------------------------------------------------------------------" ));
      err_error (( ERR_OUT, "HW error on data length -> Max W16 nb = %d -> Force 0 !", VDataLengthW16Max ));
      
      for ( ViMi26ChkDataLength = 0; ViMi26ChkDataLength < 6; ViMi26ChkDataLength++ ) {
        if ( VADataLengthW16[ViMi26ChkDataLength] > ULT1__ZS_FFRAME_RAW_MAX_W16 ) {
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

      // 26/04/2013
      
      err_retfail ( -2, (ERR_OUT,"Abort because bad data size !") );      
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
    
    #ifndef EFRIO__DEMUX_ULT1_DATA_PART
    
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
    
    
    ViSrcW32 += (7 * ULT1__ZS_FFRAME_RAW_MAX_W32);
    
    
    //    VptZsFFrameRaw[V6iFrame].Trailer = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * VADataLengthW32[0])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    //    ++ViSrcW32;
    
    //    VZero = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 6];
    //    VptZsFFrameRaw[V6iFrame].Zero = VZero;
    //    ++ViSrcW32;
    
    //    VZero2 = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 12];
    //    VptZsFFrameRaw[V6iFrame].Zero2 = VZero2;
    //    ++ViSrcW32;
    
    
    VPtFrame->Header.AMapsTrailer[0] = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + (7 * VADataLengthW32[0])];    // 21 = 7 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[1] = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + 1 + (7 * VADataLengthW32[1])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[2] = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + 2 + (7 * VADataLengthW32[2])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[3] = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + 3 + (7 * VADataLengthW32[3])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[4] = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + 4 + (7 * VADataLengthW32[4])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[5] = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + 5 + (7 * VADataLengthW32[5])];
    ++ViSrcW32;
    
    ++ViSrcW32; // To count extra channel
    
    
    // Read trigger number from "Mi26" frame
    
    if ( TrigStatus == 0 ) {
      VZero  = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) +  7];
    }
    
    // Emulate trigger by overwriting info from "Mi26" frame
    
    else {
      VZero = (TrigStatus << 16);
    }
    
    // VZero  = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) +  7];
    
    
    
    VZero2 = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 14];
    
    ViSrcW32 += 14; // 7 times 2 zero fields = 14
    
    // Extraction of trigger info from "Mimosa 26 frame" zero fields
    
    VTrigNb = (VZero  & 0xFFFF0000) >> 16;
    
    if ( VTrigNb > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ) {
      err_warning (( ERR_OUT, "VTrigNb=%d > Max=%d => Limit to max  compatible with Mi26 !", VTrigNb, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ));
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





// $$$$$$$$$$$$$$

SInt32 EFRIO__ULT1_FFRioAcqDeserDataEudet2Mode6Ult1 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode ) {
  
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
  SInt32               VErrorsOnData; // 25/04/2013
  
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FOutD5 ( 0 /* Id */, 1 /* State */ );
  #endif
  
  if ( VPtCont->InfInitDone == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort because : Init not done") );
  }
  
  err_trace (( ERR_OUT, "EFRIO__ULT1_FFRioAcqDeserDataEudet2Mode6Ult1 (P=%x, EltNb=%d)", PtSrcW32, EltNb ));
  
  // Pointers parameters check
  
  err_retnull ( PtSrcW32, (ERR_OUT,"PtSrcW32 = NULL") );
  
  // Init board conf record pointer
  
  if ( (BoardId < 0) || (BoardId >= EFRIO__MAX_BOARDS_NB) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Board Id=%d out of range [0..%d])", BoardId, EFRIO__MAX_BOARDS_NB - 1) );
  }
  
  VPtBoard = &EFRIO__VGContext.ABoardsConf[BoardId];
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / ULT1__ZS_FFRAME_MODE_2X160MHZ_W16_SZ ) / 7; // Divide by 7 because of extral channel
    
  // It must be => ( (EltNb * 2) / (ULT1__ZS_FFRAME_MODE_2X160MHZ_W16_SZ * 2) );
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
    VPtFrame->Header.MapsName     = (UInt16) ASIC__ULT1;
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
    
    // err_error (( ERR_OUT, "VDataLengthW16Max = %d", VDataLengthW16Max ));
    
    if ( VDataLengthW16Max > ULT1__ZS_FFRAME_RAW_MAX_W16 ) {

      ++VDataLengthErrCnt;
      err_error (( ERR_OUT, "---------------------------------------------------------------------" ));
      err_error (( ERR_OUT, "HW error on data length -> Max W16 nb = %d -> Force 0 !", VDataLengthW16Max ));
      
      for ( ViMi26ChkDataLength = 0; ViMi26ChkDataLength < 6; ViMi26ChkDataLength++ ) {
        if ( VADataLengthW16[ViMi26ChkDataLength] > ULT1__ZS_FFRAME_RAW_MAX_W16 ) {
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
      
      // 25/04/2013
      
      err_retfail ( -2, (ERR_OUT,"Abort because bad data size !") );
      
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
    
  #ifndef EFRIO__DEMUX_ULT1_DATA_PART
  
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
    
    
    ViSrcW32 += (7 * ULT1__ZS_FFRAME_RAW_MAX_W32);
    
    
    //    VptZsFFrameRaw[V6iFrame].Trailer = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * VADataLengthW32[0])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    //    ++ViSrcW32;
    
    //    VZero = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 6];
    //    VptZsFFrameRaw[V6iFrame].Zero = VZero;
    //    ++ViSrcW32;
    
    //    VZero2 = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 12];
    //    VptZsFFrameRaw[V6iFrame].Zero2 = VZero2;
    //    ++ViSrcW32;
    
    
    VPtFrame->Header.AMapsTrailer[0] = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + (7 * VADataLengthW32[0])];    // 21 = 7 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[1] = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + 1 + (7 * VADataLengthW32[1])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[2] = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + 2 + (7 * VADataLengthW32[2])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[3] = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + 3 + (7 * VADataLengthW32[3])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[4] = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + 4 + (7 * VADataLengthW32[4])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[5] = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + 5 + (7 * VADataLengthW32[5])];
    ++ViSrcW32;
    
    ++ViSrcW32; // To count extra channel
    
    
    // Read trigger number from "Mi26" frame
    
    if ( TrigStatus == 0 ) {
      VZero  = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) +  7];
    }
    
    // Emulate trigger by overwriting info from "Mi26" frame
    
    else {
      VZero = (TrigStatus << 16);
    }
    
    // VZero  = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) +  7];
    
    
    
    VZero2 = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 14];
    
    ViSrcW32 += 14; // 7 times 2 zero fields = 14
    
    // Extraction of trigger info from "Mimosa 26 frame" zero fields
    
    VTrigNb = (VZero  & 0xFFFF0000) >> 16;
     
    if ( VTrigNb > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ) {
      err_warning (( ERR_OUT, "VTrigNb=%d > Max=%d => Limit to max  compatible with Mi26 !", VTrigNb, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ));
      VTrigNb = EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26;
    }

    // 09/06/2011
    // => Bug fixed : 32 clock / line for Ultimate, not 16 like Mi26
    
    if ( VTrigNb != 0 ) {
      VATrigVal[0]    = (VZero  & 0x0000FFFF);
      VATrigVal[1]    = (VZero2 & 0xFFFF0000) >> 16;
      VATrigVal[2]    = (VZero2 & 0x0000FFFF);
      
      VATrigLine[0]   = VATrigVal[0] / 32;
      VATrigLine[1]   = VATrigVal[1] / 32;
      VATrigLine[2]   = VATrigVal[2] / 32;
      
      VATrigClk[0]    = VATrigVal[0] % 32;
      VATrigClk[1]    = VATrigVal[1] % 32;
      VATrigClk[2]    = VATrigVal[2] % 32;
      
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
  
    
    VPtTmpTrigRec->TrigNb   = VTrigNb; // 3; !!! 08/06/2011 => Force 3 triggers !!!
    VPtTmpTrigRec->TotSz    = VEmptyTrigRecSz + ( /*  !!! 08/06/2011 => Force 3 triggers !!! 3 */ VTrigNb * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ);
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
    
    VErrorsOnData = EFRIO__ULT1_FChkFrameLight ( 0 /* FuncId */ , ViFrame, VPtFrame, 6 /* Mi26Nb */ );
    
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
  
  // Update frames & events counters ONLY if there is no errors on data - 25/04/2013
  
  if ( VErrorsOnData == 0 ) {
    ++VPtCont->RunCont.ResAcqCnt;
    VPtCont->RunCont.ResFrameCnt = VPtCont->RunCont.ResAcqCnt * VPtCont->RunCont.ParFrameNbPerAcq;
    VPtCont->RunCont.ResEventCnt = VPtCont->RunCont.ResFrameCnt;
  }
  
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FOutD5 ( 0 /* Id */, 0 /* State */ );
  #endif
  
  return (VTotAcqSz);
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__ULT1_FFRioAcqDeserDataEudet2Mode8Ult1 (
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
          :
          : 09/06/2011
          : - Fix bug in "end of frame" triggers info calculation => 32 clk / line not 16 like Mi26
          :          
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__ULT1_FFRioAcqDeserDataEudet2Mode8Ult1 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode ) {
  
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
  SInt32               VErrorsOnData; // 26/04/2013
  
  
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
      
      // 25/04/2013
      
      err_retfail ( -2, (ERR_OUT,"Abort because bad data size !") );
      
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
    
    #ifndef EFRIO__DEMUX_ULT1_DATA_PART
    
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
      err_warning (( ERR_OUT, "VTrigNb=%d > Max=%d => Limit to max  compatible with Mi26 !", VTrigNb, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ));
      VTrigNb = EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26;
    }

    // 09/06/2011
    // => Bug fixed : 32 clock / line for Ultimate, not 16 like Mi26
    
    if ( VTrigNb != 0 ) {
      VATrigVal[0]    = (VZero  & 0x0000FFFF);
      VATrigVal[1]    = (VZero2 & 0xFFFF0000) >> 16;
      VATrigVal[2]    = (VZero2 & 0x0000FFFF);
      
      VATrigLine[0]   = VATrigVal[0] / 32;
      VATrigLine[1]   = VATrigVal[1] / 32;
      VATrigLine[2]   = VATrigVal[2] / 32;
      
      VATrigClk[0]    = VATrigVal[0] % 32;
      VATrigClk[1]    = VATrigVal[1] % 32;
      VATrigClk[2]    = VATrigVal[2] % 32;
      
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
    
    EFRIO__ULT1_FChkFrameLight ( 0 /* FuncId */ , ViFrame, VPtFrame, 8 /* Mi26Nb */ );
    
    
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
Prototype : SInt32 EFRIO__ULT1_FFRioAcqDeserDataEudet3Mode6Ult1 (
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
          : XX/05/2011
          : - Upgrade Mi26 function for Ultimate 1
          :
          : 07/06/2011
          : - Modify frames with trigger search loop to solve FW trigger bug ( delayed by 2 frames )
          :
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__ULT1_FFRioAcqDeserDataEudet3Mode6Ult1 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode ) {



  EFRIO__TContext*     VPtCont   = &EFRIO__VGContext;
  EFRIO__TBoardConf*   VPtBoard  = NULL;
  EFRIO__TRunCont*     VPtRun    = &VPtCont->RunCont;
  EFRIO__TFrameList*   VPtFrList = &VPtCont->AAcqFrameList[0];
  
  EFRIO__TFrame*       VPtFrame;
  EFRIO__TFrame*       VPtFrameMinus2; // Pointer two frames before VPtFrame to solve FW trigger bug
  
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
  EFRIO__TTriggerRec*  VPtTrigRecFrameMinus2;
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
  SInt16*              VPtFwTrigBugFrameWithTrigTrigNb;
  SInt16*              VPtFwTrigBugFrameWithTrigIsTrigListFrMinus2;
  
  static SInt32        VFrNbToTakeAtBeginOfAcqForLastTrig = -1; // Number of frames to take at beginning of Acq to complete last trigger of previous Acq
  SInt32               VTotAcqSz;
  
  // Limits of trigger info search in case of FW trigger bug
  // => trigger info appears N (should be 2) frames after trigger occurs
  
  SInt32               VFwTrigBugFirstFrameTrigInfAllowed;
  SInt32               VFwTrigBugLastFrameTrigInfAllowed;
  SInt32               VFwTrigBugLastFrameIdStored;
  SInt32               VErrorsOnData; // 26/04/2013
  
  
  #ifdef EFRIO__INCLUDE_PARA_PORT
    PPO_FOutD5 ( 0 /* Id */, 1 /* State */ );
  #endif
  
  if ( VPtCont->InfInitDone == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort because : Init not done") );
  }
  
  err_trace (( ERR_OUT, "EFRIO__ULT1_FFRioAcqDeserDataEudet3Mode6Ult1 (P=%x, EltNb=%d)", PtSrcW32, EltNb ));
  
  // Pointers parameters check
  
  err_retnull ( PtSrcW32, (ERR_OUT,"PtSrcW32 = NULL") );
  
  // Init board conf record pointer
  
  if ( (BoardId < 0) || (BoardId >= EFRIO__MAX_BOARDS_NB) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Board Id=%d out of range [0..%d])", BoardId, EFRIO__MAX_BOARDS_NB - 1) );
  }
  
  VPtBoard = &EFRIO__VGContext.ABoardsConf[BoardId];
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / ULT1__ZS_FFRAME_MODE_2X160MHZ_W16_SZ ) / 7; // Divide by 7 because of extral channel
  
  // It must be => ( (EltNb * 2) / (ULT1__ZS_FFRAME_MODE_2X160MHZ_W16_SZ * 2) );
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
    
  // 09/06/2011
  
  memset ( VPtCont->FwTrigBugAAAcqFrameWithTrigTrigNb[0], 0, EFRIO__MAX_FRAME_NB_PER_ACQ * sizeof (VPtCont->FwTrigBugAAAcqFrameWithTrigTrigNb[0][0]) );

  memset ( VPtCont->FwTrigBugAAAcqFrameWithTrigIsTrigListFrMinus2[0], 0, (EFRIO__MAX_FRAME_NB_PER_ACQ + 10) * sizeof (VPtCont->FwTrigBugAAAcqFrameWithTrigIsTrigListFrMinus2[0][0]) );
  

  // Init loop parameters
  
  VFrameWithTrigCnt             =  0;
  VLastFrameWithTrig            = -1;
  VPtFrameWithTrigList          = VPtCont->AAAcqFrameWithTrigList[0];
  
  VPtFwTrigBugFrameWithTrigTrigNb = VPtCont->FwTrigBugAAAcqFrameWithTrigTrigNb[0]; // 09/06/2011
  
  VPtFwTrigBugFrameWithTrigIsTrigListFrMinus2 = VPtCont->FwTrigBugAAAcqFrameWithTrigIsTrigListFrMinus2[0]; // 09/06/2011


  // If trigger mode = 1 => Take always first frame of acq EFRIO__FRAME_NB_TO_READ_AFTER_TRIG
  //
  // Done by overwriting VFrNbToTakeAtBeginOfAcqForLastTrig ( set on previous acq ) with EFRIO__FRAME_NB_TO_READ_AFTER_TRIG
  
  if ( TriggerHandlingMode == 1 ) {
    VFrNbToTakeAtBeginOfAcqForLastTrig = EFRIO__FRAME_NB_TO_READ_AFTER_TRIG;
  }
  



// Frame with trigger search loop in case of FW bug on trigger
// => Trigger info appears two frames after trigger

#define FRIO__ULT1_BUG_FW_TRIG_DELAYED_BY_TWO_FRAMES

#ifdef FRIO__ULT1_BUG_FW_TRIG_DELAYED_BY_TWO_FRAMES

  // ------------------------------------------------------------------
  // Trigger bug
  // - Info in zero fields at end of frame are at the right place
  // => The info extracted from zero fields are ok without any shift
  // => Trigger Nb & the first three triggers are OK
  // - But the triggers list stored in extra channel will appear two
  //   frames later
  // ------------------------------------------------------------------
  // The frame which contains trigger (in data) must be in current acq
  // = we don't want to handle trigger from one acq to the next one
  // We take 3 frames after the one which contains trigger
  // = constant EFRIO__FRAME_NB_TO_READ_AFTER_TRIG is ignored
  // ------------------------------------------------------------------
  // => we exit function if EFRIO__FRAME_NB_TO_READ_AFTER_TRIG is <> 3
  // => we reject trigger if info appears on frame > FrameNbPerAcq - 4


  if ( EFRIO__FRAME_NB_TO_READ_AFTER_TRIG != 3 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => EFRIO__FRAME_NB_TO_READ_AFTER_TRIG=%d <> 3 => NOT handled in trig fw bug correction mode !", EFRIO__FRAME_NB_TO_READ_AFTER_TRIG) );
  }


  VFwTrigBugFirstFrameTrigInfAllowed = 0;
  VFwTrigBugLastFrameTrigInfAllowed  = VPtBoard->FrameNbPerAcq - 4;

  VFwTrigBugLastFrameIdStored        = -1;

  // Start search on first frame trigger info is allowed

  for ( VFrameId=VFwTrigBugFirstFrameTrigInfAllowed; VFrameId < VPtBoard->FrameNbPerAcq; VFrameId++ ) {
    
    V7FrameId = 7 * VFrameId;
    
    if ( TrigStatus == 0 ) {
      VZero  = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) +  7];
    }
    
    // Emulate trigger by overwriting info from "Mi26" frame
    // BUT first read it from board RAM in order to get real execution time
    
    else {
      VZero  = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) +  7];
      VZero = (TrigStatus << 16);
    }
    
    VTrigNb = (VZero  & 0xFFFF0000) >> 16;

    if ( VTrigNb > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ) {
      err_warning (( ERR_OUT, "VTrigNb=%d > Max=%d => Limit to max  compatible with Mi26 !", VTrigNb, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ));
      VTrigNb = EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26;
    }
    
// --------------------

    if ( (VTrigNb != 0) && (VFrameId <= VFwTrigBugLastFrameTrigInfAllowed) ) {
      VLastFrameWithTrig = VFrameId;
      VPtFrameWithTrigList[VFrameWithTrigCnt] = VFrameId;
      
      if ( VPtFwTrigBugFrameWithTrigTrigNb[VFrameWithTrigCnt] != 0 ) {
        err_warning (( ERR_OUT, "Trigger error => Elt[%d] = frame %d already filled with %d", VFrameWithTrigCnt, VFrameId, VPtFwTrigBugFrameWithTrigTrigNb[VFrameWithTrigCnt] ));
      }
      
      VPtFwTrigBugFrameWithTrigTrigNb[VFrameWithTrigCnt]                 =  VTrigNb; // Store trigger cnt in frame which doesn't contain extra chan trig list
      
      VPtFwTrigBugFrameWithTrigIsTrigListFrMinus2[VFrameWithTrigCnt + 2] = 1; // Set flag "is trigger list" of frame No - 2
      
      ++VFrameWithTrigCnt;
    }
    
    else {
      
      if ( (VLastFrameWithTrig != -1) && ((VFrameId - VLastFrameWithTrig) <= EFRIO__FRAME_NB_TO_READ_AFTER_TRIG) ) {
        VPtFrameWithTrigList[VFrameWithTrigCnt]          = VFrameId;
        ++VFrameWithTrigCnt;
      }
      
    } // End else
    

// --------------------
    
  } // End for ( ViFrame )
    

// Frame with trigger search loop in case the is NO FW bug on trigger

#else

  for ( VFrameId=0; VFrameId < VPtBoard->FrameNbPerAcq; VFrameId++ ) {
    
    V7FrameId = 7 * VFrameId;
    
    if ( TrigStatus == 0 ) {
      VZero  = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) +  7];
    }
    
    // Emulate trigger by overwriting info from "Mi26" frame
    // BUT first read it from board RAM in order to get real execution time
    
    else {
      VZero  = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) +  7];
      VZero = (TrigStatus << 16);
    }
    
    VTrigNb = (VZero  & 0xFFFF0000) >> 16;

    if ( VTrigNb > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ) {
      err_warning (( ERR_OUT, "VTrigNb=%d > Max=%d => Limit to max  compatible with Mi26 !", VTrigNb, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ));
      VTrigNb = EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26;
    }
    
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
  

#endif

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
    ViSrcW32           = VFrameId * 7 * ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32;
    
    V7FrameId = 7 * VFrameId;
        
    // Update pointer to next frame
    
    if ( VPtFrList->TotFrameNb == 0 ) {
      VPtFrame       = VPtFrList->AFramePtr[0] = VPtRun->PtFrame;
      VPtFrameMinus2 = NULL; // 07/06/2011
    }
    
    else {
      
      if ( VPtFrList->TotFrameNb >= VPtCont->RunCont.ParFrameNbPerAcq ) {
        err_retfail ( -1, (ERR_OUT,"Abort try to process frame %d > Max = %d ", VPtFrList->TotFrameNb, VPtCont->RunCont.ParFrameNbPerAcq) );
      }
      
      VPtFrame = VPtFrList->AFramePtr[VPtFrList->TotFrameNb] = (EFRIO__TFrame*) ((UInt8*) VPtFrList->AFramePtr[VPtFrList->TotFrameNb - 1] + VFullFrameRecSz);
      
      // 07/06/2011
      
      if ( VPtFrList->TotFrameNb > 1 ) {
        VPtFrameMinus2 = VPtFrList->AFramePtr[VPtFrList->TotFrameNb - 2];
      }
      
    }
    
  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtFrame->Tag          = EFRIO__FRAME_TAG;
    VPtFrame->Header.Tag   = EFRIO__FRAME_TAG_HEADER;
  #endif
  
    VPtFrame->Header.AcqStatus    = AcqStatus;
    VPtFrame->Header.TrigStatus   = TrigStatus;
    VPtFrame->Header.AcqId        = VAcqId;
    VPtFrame->Header.FrameIdInAcq = VFrameId;
    VPtFrame->Header.MapsName     = (UInt16) ASIC__ULT1;
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
    
    if ( VDataLengthW16Max > ULT1__ZS_FFRAME_RAW_MAX_W16 ) {

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
      
      // 25/04/2013
      
      err_retfail ( -2, (ERR_OUT,"Abort because bad data size !") );      
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
    

    
  #ifndef EFRIO__DEMUX_ULT1_DATA_PART
  
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
    

  
    ViSrcW32 += (7 * ULT1__ZS_FFRAME_RAW_MAX_W32);
    
    
    //    VptZsFFrameRaw[V6iFrame].Trailer = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * VADataLengthW32[0])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    //    ++ViSrcW32;
    
    //    VZero = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 6];
    //    VptZsFFrameRaw[V6iFrame].Zero = VZero;
    //    ++ViSrcW32;
    
    //    VZero2 = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 12];
    //    VptZsFFrameRaw[V6iFrame].Zero2 = VZero2;
    //    ++ViSrcW32;
    
    
    VPtFrame->Header.AMapsTrailer[0] = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + (7 * VADataLengthW32[0])];    // 21 = 7 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[1] = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + 1 + (7 * VADataLengthW32[1])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[2] = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + 2 + (7 * VADataLengthW32[2])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[3] = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + 3 + (7 * VADataLengthW32[3])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[4] = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + 4 + (7 * VADataLengthW32[4])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[5] = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + 5 + (7 * VADataLengthW32[5])];
    ++ViSrcW32;
    
    ++ViSrcW32; // To count extra channel
    

    // Read trigger number from "Mi26" frame
    
    if ( TrigStatus == 0 ) {
      VZero  = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) +  7];
    }
    
    // Emulate trigger by overwriting info from "Mi26" frame
    
    else {
      VZero = (TrigStatus << 16);
    }
    
    // VZero  = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) +  7];
    
    
    
    VZero2 = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 14];
    
    ViSrcW32 += 14; // 7 times 2 zero fields = 14
  
    // Extraction of trigger info from "Mimosa 26 frame" zero fields
    
    VTrigNb = (VZero  & 0xFFFF0000) >> 16;

    if ( VTrigNb > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ) {
      err_warning (( ERR_OUT, "VTrigNb=%d > Max=%d => Limit to max  compatible with Mi26 !", VTrigNb, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ));
      VTrigNb = EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26;
    }
    
    if ( VTrigNb != 0 ) {
      VATrigVal[0]    = (VZero  & 0x0000FFFF);
      VATrigVal[1]    = (VZero2 & 0xFFFF0000) >> 16;
      VATrigVal[2]    = (VZero2 & 0x0000FFFF);
      
      VATrigLine[0]   = VATrigVal[0] / 32;
      VATrigLine[1]   = VATrigVal[1] / 32;
      VATrigLine[2]   = VATrigVal[2] / 32;
      
      VATrigClk[0]    = VATrigVal[0] % 32;
      VATrigClk[1]    = VATrigVal[1] % 32;
      VATrigClk[2]    = VATrigVal[2] % 32;
      
      VAMi26Trig[0]   = VATrigLine[0] /* + ( VATrigClk[0] << 10 ) */;
      VAMi26Trig[1]   = VATrigLine[1] /* + ( VATrigClk[1] << 10 ) */;
      VAMi26Trig[2]   = VATrigLine[2] /* + ( VATrigClk[2] << 10 ) */;
            
    }
    
    else {
      VAMi26Trig[0] = VAMi26Trig[1] = VAMi26Trig[2] = 0;
    }
    
    // Add trigger info in frame header
    
    // Test coherence of trigger nb list / current frame trigger nb info
    
    if ( (VTrigNb > 0) && (VPtFwTrigBugFrameWithTrigTrigNb[ViFrameWithTrig] != VTrigNb) ) {
      err_warning (( ERR_OUT, "Trigger nb list error => ViFrameWithTrig=%d - FrameId=%d - TrigNb=%d <> Trig nb from list=%d", ViFrameWithTrig, VPtFrameWithTrigList[ViFrameWithTrig], VTrigNb, VPtFwTrigBugFrameWithTrigTrigNb[ViFrameWithTrig] ));
    }
      
    
    #ifdef FRIO__ULT1_BUG_FW_TRIG_DELAYED_BY_TWO_FRAMES
      VPtFrame->Header.TrigStatus = VPtFwTrigBugFrameWithTrigIsTrigListFrMinus2[ViFrameWithTrig];
    #else
      VPtFrame->Header.TrigStatus = 0;
    #endif
  
    
    VPtFrame->Header.TriggerNb        = VTrigNb;
    VPtFrame->Header.AMapsTrigInfo[0] = VAMi26Trig[0];
    VPtFrame->Header.AMapsTrigInfo[1] = VAMi26Trig[1];
    VPtFrame->Header.AMapsTrigInfo[2] = VAMi26Trig[2];
    
    // Add trigger info in trigger record
    
    VPtTrigRec = (EFRIO__TTriggerRec*) (VPtFrame->Data.ADataW32 + VDataLengthW32ToCpy);
    
  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtTmpTrigRec->Tag      = EFRIO__FRAME_TAG_TRIG;
  #endif
  
    
    VPtTmpTrigRec->TrigNb   = VTrigNb; // 3; 08/06/2011 !!! Force 3 triggers !!!
    VPtTmpTrigRec->TotSz    = VEmptyTrigRecSz + ( /* !!! 08/06/2011 Force 3 triggers !!! */ VTrigNb * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ);
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
    
    EFRIO__ULT1_FChkFrameLight ( 0 /* FuncId */ , VFrameId, VPtFrame, 6 /* Mi26Nb */ );
    


    #ifdef FRIO__ULT1_BUG_FW_TRIG_DELAYED_BY_TWO_FRAMES

    // If current frame contains triggers list of frame N-2 => Update frame N-2
    
      if ( VPtFwTrigBugFrameWithTrigIsTrigListFrMinus2[ViFrameWithTrig] == 1 ) {
        
        // Do it only if pointer is valid
        
        if ( VPtFrameMinus2 != NULL ) {
          
          // Trigger record should have the same size as current frame because same trigger nb
          // But check in case of ...
          
          VPtTrigRecFrameMinus2 = (EFRIO__TTriggerRec*) ( (UInt32) VPtFrameMinus2 + VPtFrameMinus2->TrigRecOffset );
          
          if ( VPtTrigRecFrameMinus2->TotSz == VPtTmpTrigRec->TotSz ) {
            memcpy ( VPtTrigRecFrameMinus2, VPtTmpTrigRec, VPtTmpTrigRec->TotSz );
          }
          
          else {
            err_warning (( ERR_OUT, "Abort update trig rec of Fr-2 because TrigRecSz=%d <> Current frame[%d] TrigRecSz=%d", VPtTrigRecFrameMinus2->TotSz, ViFrameWithTrig, VPtTmpTrigRec->TotSz ));
          }
          
        }
        
        
      }
    
   #endif
    

  
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
Prototype : SInt32 EFRIO__ULT1_FFRioAcqDeserDataEudet3Mode8Ult1 (
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
          : XX/05/2011
          : - Upgrade Mi26 function for Ultimate 1
          :
          : 07/06/2011
          : - Modify frames with trigger search loop to solve FW trigger bug ( delayed by 2 frames )
          :          
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__ULT1_FFRioAcqDeserDataEudet3Mode8Ult1 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode ) {
  
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
  SInt32               VErrorsOnData; // 26/04/2013
  
  
  
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
  
  

  // Frame with trigger search loop in case of FW bug on trigger
  // => Trigger info appears two frames after trigger

#ifdef EFRIO__ULT1_BUG_FW_TRIG_DELAYED_BY_TWO_FRAMES

  // ------------------------------------------------------------------
  // The frame which contains trigger (in data) must be in current acq
  // = we don't want to handle trigger from one acq to the next one
  // We take 3 frames after the one which contains trigger
  // = constant EFRIO__FRAME_NB_TO_READ_AFTER_TRIG is ignored
  // ------------------------------------------------------------------
  // => we exit function if EFRIO__FRAME_NB_TO_READ_AFTER_TRIG is <> 3
  // => we reject trigger if info appears on frame no < 2
  // => we reject trigger if info appears on frame > FrameNbPerAcq - 3 + 1
  
  
  err_retfail ( -1, (ERR_OUT,"Abort => Correction for FW trigger bug NOT IMPLEMENTED yet !") );
  
  
// Frame with trigger search loop in case the is NO FW bug on trigger

#else
  
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
  

#endif


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
    
      // 25/04/2013
      
      err_retfail ( -2, (ERR_OUT,"Abort because bad data size !") );      
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
    
    
    
    #ifndef EFRIO__DEMUX_ULT1_DATA_PART

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
      err_warning (( ERR_OUT, "VTrigNb=%d > Max=%d => Limit to max  compatible with Mi26 !", VTrigNb, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ));
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
    
    EFRIO__ULT1_FChkFrameLight ( 0 /* FuncId */ , VFrameId, VPtFrame, 8 /* Mi26Nb */ );

    
    
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
Prototype : SInt32 EFRIO__MI26_FFRioAcqDeserDataUlt1 (
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
          :
          : 27/03/2013
          : - Add param DataTestTotErrCnt and return it as run status in RunCont.ParDaqVersion
          : using bits B00B23, bits B24B31 are reserved for Daq version.  
          :
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
  
  
SInt32 EFRIO__ULT1_FFRioAcqDeserDataUlt1 ( SInt8 Mi26Nb, SInt32 BoardId, UInt32* PtSrcW32AsPt, UInt32 PtSrcW32AsInt, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, UInt32 WaitMsAtEnd, SInt8 DataConvertMode, SInt8 TriggerHandlingMode, SInt16 EmuleMode, UInt32 DataTestTotErrCnt ) {
  
  EFRIO__TContext*     VPtCont    = &EFRIO__VGContext;
  EFRIO__TRunCont*     VPtRunCont = &EFRIO__VGContext.RunCont;
  
  SInt32 VRet = 0;
  SInt32 VEmuleFrameNb;
  static UInt32 VEmuleFirstFrameNo = 0;
  
  SInt32 VDbgOffset;
  
  
  
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
            err_retfail ( -1, (ERR_OUT,"Emul - IPHC / EUDET 1 - %d Ultimates => Not hanled now !", Mi26Nb ) );
            EFRIO__MI26_FFRioEmulDeserData1Mi26NoEChan ( PtSrcW32AsPt, EltNb, &VEmuleFirstFrameNo, VEmuleFrameNb  );
            break; }
          
          case 6 : {
            err_retfail ( -1, (ERR_OUT,"Emul - IPHC / EUDET 1 - %d Ultimates => Not hanled now !", Mi26Nb ) );
            EFRIO__MI26_FFRioEmulDeserData6Mi26NoEChan ( PtSrcW32AsPt, EltNb, &VEmuleFirstFrameNo, VEmuleFrameNb  );
            break; }
          
          default : {
            err_warning (( ERR_OUT, "This number of Ultimate = %d is not handled for frame emulation WITHOUT extra channel !", Mi26Nb ));
            break; }
          
        } // End switch
        
        break;
      } // End if ( EFRIO__TRF_MODE_IPHC )
      

      if ( DataConvertMode == EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES ) {
        
        switch ( Mi26Nb ) {
          
          case 1 : {
            err_retfail ( -1, (ERR_OUT,"Emul - EUDET 2 - %d Ultimates => Not hanled now !", Mi26Nb ) );
            EFRIO__MI26_FFRioEmulDeserData1Mi26EudetMode2 ( PtSrcW32AsPt, EltNb, &VEmuleFirstFrameNo, VEmuleFrameNb, EmuleMode  );
            break; }
          
          case 6 : {
            EFRIO__ULT1_FFRioEmulDeserData6Ult1EudetMode2 ( PtSrcW32AsPt, EltNb, &VEmuleFirstFrameNo, VEmuleFrameNb, EmuleMode  );
            break; }

          case 8 : {
            err_retfail ( -1, (ERR_OUT,"Emul - EUDET 2 - %d Ultimates => Not hanled now !", Mi26Nb ) );
            EFRIO__MI26_FFRioEmulDeserData8Mi26EudetMode2 ( PtSrcW32AsPt, EltNb, &VEmuleFirstFrameNo, VEmuleFrameNb, EmuleMode  );
            break; }
          
          default : {
            err_warning (( ERR_OUT, "This number of Ultimate = %d is not handled for frame emulation WITH extra channel !", Mi26Nb ));
            break; }
          
        } // End switch
        
        break;
      } // End if ( EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES )


      if ( DataConvertMode == EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG ) {
        
        switch ( Mi26Nb ) {
          
          case 1 : {
            err_retfail ( -1, (ERR_OUT,"Emul - EUDET 3 - %d Ultimates => Not hanled now !", Mi26Nb ) );
            EFRIO__MI26_FFRioEmulDeserData1Mi26EudetMode3 ( PtSrcW32AsPt, EltNb, &VEmuleFirstFrameNo, VEmuleFrameNb, EmuleMode  );
            break; }
          
          case 6 : {
            EFRIO__ULT1_FFRioEmulDeserData6Ult1EudetMode3 ( PtSrcW32AsPt, EltNb, &VEmuleFirstFrameNo, VEmuleFrameNb, EmuleMode  );
            break; }

          case 8 : {
            err_retfail ( -1, (ERR_OUT,"Emul - EUDET 3 - %d Ultimates => Not hanled now !", Mi26Nb ) );
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
      err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_IPHC is not handled for Ultimate" ) );
      break;
    }
  
    // EUDET mode 1
    
    if ( DataConvertMode == EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN ) {
      
      switch ( Mi26Nb ) {
        
        case 1 : {
          err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN -> This number of Mi26 = %d is not handled now", Mi26Nb ) );
          VRet = EFRIO__ULT1_FFRioAcqDeserDataEudet1Mode1Ult1 ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode );
          break; }
        
        case 6 : {
          err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN -> This number of Mi26 = %d is not handled now", Mi26Nb ) );
          VRet = EFRIO__ULT1_FFRioAcqDeserDataEudet1Mode6Ult1 ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode );
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
          VRet = EFRIO__ULT1_FFRioAcqDeserDataEudet2Mode1Ult1 ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode, TriggerHandlingMode );
          break; }
        
        case 6 : {
          VRet = EFRIO__ULT1_FFRioAcqDeserDataEudet2Mode6Ult1 ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode, TriggerHandlingMode );
          break; }

        case 8 : {
          err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES -> This number of Mi26 = %d is not handled now", Mi26Nb ) );
          VRet = EFRIO__ULT1_FFRioAcqDeserDataEudet2Mode8Ult1 ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode, TriggerHandlingMode );
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
          err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN -> This number of Mi26 = %d is not handled now", Mi26Nb ) );
          VRet = EFRIO__ULT1_FFRioAcqDeserDataEudet3Mode1Ult1 ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode, TriggerHandlingMode );
          break; }
        
        case 6 : {
          VRet = EFRIO__ULT1_FFRioAcqDeserDataEudet3Mode6Ult1 ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode, TriggerHandlingMode );
          break; }

        case 8 : {
          err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN -> This number of Mi26 = %d is not handled now", Mi26Nb ) );
          VRet = EFRIO__ULT1_FFRioAcqDeserDataEudet3Mode8Ult1 ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode, TriggerHandlingMode );
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
  
  // WARNING ! Use ParDaqVersion to return Data test total errors counter
  // Overwrite the ParDaqVersion field with the errors count converted in a NEGATIVE NUMBER
  
  VPtCont->RunCont.ParDaqVersion = DataTestTotErrCnt & 0x00FFFFFF; // B24B31 reserved for Daq version
  
   
  
  if ( VRet > 0 ) {
    VPtRunCont->InfDataRateMeasTotalSz += VRet;
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

SInt32 EFRIO__ULT1_FJtagLoadFile ( char* FileName ) {
  
  HRESULT    VRetCode;
  WideString VStatus;
  WideString VFileName;

#ifdef EFRIO__INCLUDE_PARA_PORT
  TCOMIMI28COM  VJtag;
  HRESULT       VHrComErr;
  WideString    VStrComStatus;
#endif

  

#ifdef EFRIO__INCLUDE_JTAG

  // COM handling
  
  VHrComErr = CoInitialize (NULL);
  
  if ( FAILED (VHrComErr) ) {
    err_retfail ( -1, (ERR_OUT,"CoInitialize failed !" ) );
  }
  
  VHrComErr =  CoMI28COM::Create( VJtag );
  
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
Date      : 13/03/2013
Rev       :
:
Doc date  :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__ULT1_FJtagLoadDefFile () {
  
  SInt32               VRet;
  EFRIO__TContext*     VPtCont   = &EFRIO__VGContext;
  EFRIO__TRunCont*     VPtRun    = &VPtCont->RunCont;
  char                 VFileName[GLB_FILE_PATH_SZ];
  
  sprintf ( VFileName, "%s", "c:\\ccmos_sctrl\\Mimosa28_jtag\\config_files\\Default.mcf" );
  
  msg (( MSG_OUT, "### EFRIO__ULT1_FJtagLoadDefFile (FileName=%s)", VFileName ));
  
  VRet = EFRIO__ULT1_FJtagLoadFile (VFileName);
  
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

SInt32 EFRIO__ULT1_FJtagReset ( ) {
  
  HRESULT    VRetCode;
  WideString VStatus;

#ifdef EFRIO__INCLUDE_JTAG
  TCOMIMI28COM  VJtag;
  HRESULT       VHrComErr;
  WideString    VStrComStatus;
#endif


#ifdef EFRIO__INCLUDE_JTAG

  // COM handling
  
  VHrComErr = CoInitialize (NULL);
  
  if ( FAILED (VHrComErr) ) {
    err_retfail ( -1, (ERR_OUT,"CoInitialize failed !" ) );
  }
  
  VHrComErr =  CoMI28COM::Create( VJtag );
  
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
Date      : 09/11/2010
Rev       : 04/02/2011
          : - Use a local ( in function ) JTAG COM object instance, global used before
          : but this was incompatible with multithreading ( => application crash )
          :
          : 20/03/2013
          : - Exit if r/w errors detected on MasterConfReadBack () call
          :
Doc date  : 09/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__ULT1_FJtagLoadChip ( ) {
  
  HRESULT    VRetCode;
  WideString VStatus;
  SInt32     VRbErr;

#ifdef EFRIO__INCLUDE_JTAG
  TCOMIMI28COM  VJtag;
  HRESULT       VHrComErr;
  WideString    VStrComStatus;
#endif




#ifdef EFRIO__INCLUDE_JTAG

//  msg (( MSG_OUT, "EFRIO__ULT1_FJtagLoadChip ()"));
    

  // COM handling
  
  VHrComErr = CoInitialize (NULL);
  
  if ( FAILED (VHrComErr) ) {
    err_retfail ( -1, (ERR_OUT,"CoInitialize failed !" ) );
  }
  
  VHrComErr =  CoMI28COM::Create( VJtag );
  
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
    
    OleCheck( VRetCode = VJtag.MasterConfReadBack (&VRbErr, &VStatus) );
    
//    msg (( MSG_OUT, "EFRIO__ULT1_FJtagLoadChip ( ) => Readback code=%d - errnb=%d", VRetCode, VRbErr ));
    
    if ( VRetCode != S_OK ) {
      CoUninitialize();
      err_retfail ( -1, (ERR_OUT,"JTAG -> Read back chip parameters failed !") );
    }
  
    if ( VRbErr > 0 ) {
      err_retfail ( -VRbErr, (ERR_OUT,"JTAG -> Read back chip => Write <> Read => %d errors !", VRbErr ) );
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

SInt32 EFRIO__ULT1_FJtagStartChip ( ) {
  
  HRESULT    VRetCode;
  WideString VStatus;

#ifdef EFRIO__INCLUDE_JTAG
  TCOMIMI28COM  VJtag;
  HRESULT       VHrComErr;
  WideString    VStrComStatus;
#endif


#ifdef EFRIO__INCLUDE_JTAG

  // COM handling
  
  VHrComErr = CoInitialize (NULL);
  
  if ( FAILED (VHrComErr) ) {
    err_retfail ( -1, (ERR_OUT,"CoInitialize failed !" ) );
  }
  
  VHrComErr =  CoMI28COM::Create( VJtag );
  
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
Date      : 10/11/2010
Doc date  : 10/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__ULT1_FHwStartChip ( SInt32 SpareS32Par ) {
  
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
    PPO_FOutD6 ( 0 /* Id */, 0  );
  
  #else
    err_warning (( ERR_OUT, "HW start not done -> // port not enabled !" ));
  #endif

  err_retok (( ERR_OUT, "" ));
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__ULT1_FFRioAcqDeserDataEudet3Mode1Ult1 (
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

SInt32 EFRIO__ULT1_FFRioAcqDeserDataEudet3Mode1Ult1 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode ) {
  
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
      err_warning (( ERR_OUT, "VTrigNb=%d > Max=%d => Limit to max  compatible with Mi26 !", VTrigNb, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ));
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
    
    EFRIO__ULT1_FChkFrameLight ( 0 /* FuncId */ , VFrameId, VPtFrame, 1 /* Mi26Nb */ );
    
    
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
  


#endif
