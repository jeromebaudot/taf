

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
  
  // Get acq id
  
  VAcqId = VPtCont->RunCont.ResAcqCnt;
  
  // Get last frame with trigger allowed / acq
  
  VLastFrameWithTrigAllowed = VPtBoard->FrameNbPerAcq - 1; // Last n frames of Acq MUST have NO trigger information - NOW n = 0 !!!
  
  // Check if buffer is allocated ( this is total buffer, not only the part where we will write )
  
  err_retnull ( VPtRun->PtFrame, (ERR_OUT,"Abort : EUDET frames buffer not allocated !") );
  
  // Init frame pointer on first elt of list & reset frame counter ( for acquistion )
  
  VPtFrame = VPtFrList->AFramePtr[0];
  
  VPtFrList->TotFrameNb = 0;
  
  // Check if pointer is valid
  
  err_retnull ( VPtFrame, (ERR_OUT,"Abort : No valid buffer for frame [%d]", VPtFrList->TotFrameNb ) );
  
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
  
  // Mark first three frames as frames with trigger
  
  VFrameWithTrigCnt    =  3;
  VLastFrameWithTrig   = -1;
  VPtFrameWithTrigList = VPtCont->AAAcqFrameWithTrigList[0];
  
  VPtFrameWithTrigList[0] = 0;
  VPtFrameWithTrigList[1] = 1;
  VPtFrameWithTrigList[2] = 2;
  
  for ( VFrameId=3; VFrameId < VPtBoard->FrameNbPerAcq; VFrameId++ ) {
    
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
      
      if ( (VLastFrameWithTrig != -1) && ((VFrameId - VLastFrameWithTrig) <= EFRIO__FRAME_NB_TO_READ_AFTER_TRIG) ) {
        VPtFrameWithTrigList[VFrameWithTrigCnt] = VFrameId;
        ++VFrameWithTrigCnt;
      }
      
    } // End else
    
  } // End for ( ViFrame )
  
  PPO_FOutD7 ( 0 /* Id */, 1 /* State */ );
  
  
  // Print list of frames to extract
  
  /*
  msg (( MSG_OUT, "=========================================================" ));
  
  for ( ViFrameWithTrig=0; ViFrameWithTrig < VFrameWithTrigCnt; ViFrameWithTrig++ ) {
    msg (( MSG_OUT, "Readout => Trig on frame %.4d", VPtCont->AAAcqFrameWithTrigList[0][ViFrameWithTrig] ));
  }
  */
  
  // Extract data
  
  ViSrcW32           = 0;
  VDataLengthErrCnt  = 0;
  
  // for ( ViFrame=0; ViFrame < VPtBoard->FrameNbPerAcq; ViFrame++ ) {
    
    for ( ViFrameWithTrig=0; ViFrameWithTrig < VFrameWithTrigCnt; ViFrameWithTrig++ ) {
      
      VFrameId           = VPtFrameWithTrigList[ViFrameWithTrig];
      VEChanTrigFieldCnt = 0;
      ViSrcW32           = VFrameId * 2 * MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32;
      
      // Update pointer to next frame
      
      if ( VPtFrList->TotFrameNb > 0 ) {
        
        if ( VPtFrList->TotFrameNb >= VPtCont->RunCont.ParFrameNbPerAcq ) {
          err_retfail ( -1, (ERR_OUT,"Abort try to process frame %d > Max = %d ", VPtFrList->TotFrameNb, VPtCont->RunCont.ParFrameNbPerAcq) );
        }
        
        VPtFrame = VPtFrList->AFramePtr[VPtFrList->TotFrameNb] = (EFRIO__TFrame*) ((UInt8*) VPtFrList->AFramePtr[VPtFrList->TotFrameNb - 1] + VFullFrameRecSz);
        
        // err_error (( ERR_OUT, "TRACE : Set pointer frame %d - pointer = %d [D]", VPtFrList->TotFrameNb, VPtFrList->AFramePtr[VPtFrList->TotFrameNb] ));
      }
      
      VPtFrame->Tag        = 0x55550000;
      
      VPtFrame->Header.Tag                = 0x00000001;
      VPtFrame->Header.AcqId              = VAcqId;
      VPtFrame->Header.FrameIdInAcq       = VFrameId;
      
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
      
      VPtFrame->Data.Tag       = 0x00000002;
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
      
      VPtTmpTrigRec->Tag      = 0x00000003;
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
    
    
    PPO_FOutD7 ( 0 /* Id */, 0 /* State */ );
    
    
    return (VFrameWithTrigCnt);
  }
  