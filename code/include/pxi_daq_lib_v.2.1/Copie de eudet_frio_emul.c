
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


#ifndef EUDET_FRIO_EMUL_C
#define EUDET_FRIO_EMUL_C


  


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__MI26_FFRioEmulDeserData6Mi26NoEChan (
          :  UInt32* PtDestW32, SInt32 EltNb, SInt32* PtFirstFrameNo, SInt32 FrameNb  )
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
Doc date  : 07/11/2010
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




#endif