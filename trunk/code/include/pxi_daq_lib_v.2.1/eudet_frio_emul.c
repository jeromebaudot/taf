
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
Prototype : SInt32 EFRIO__MI26_FFRioEmulDeserData1Mi26NoEChan (
          :  UInt32* PtDestW32, SInt32 EltNb, UInt32* PtFirstFrameFC, SInt32 FrameNb  )
          :
Goal      : Emulate one acquisition for one Mi26 in data transfer mode IPHC or EUDET 1
          :
          : In this case there is no extra channel for TLU triggers.
          :
          : Triggers are stored in Zero fields of Mi26 frame, they represent line of Mi26
          : read while trigger occurs in 80 MHz clock unit -> divide by 16 to get line.
          : Maximum number of triggers is 3.
          :
          : The number of triggers is hard coded to 3, their values are
          : Trig[0] = 16 => line 1
          : Trig[1] = 32 => line 2
          : Trig[2] = 64 => line 4
          :
          :
Inputs    : PtDestW32        - Pointer to Flex RIO DRAM to overwrite
          : EltNb            - Flex RIO DRAM size in W32 unit ( 1 Elt = 1 W32 )
          :
          : PtFirstFrameFC   - Pointer to a variable which contains a global frame counter
          :                     Used to set frame counter of each frame of current acq
          :                     Incremented by the number of frames of current acq at end of function
          :
          : FrameNb          - The number of frame to emulate
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   :
          :
          :
Level     : This is a user level function.
Date      : 02/02/2010
Rev       : 10/03/2010
          : - Add parameter PtFirstFrameFC
          : 26/10/2010
          : - Moved from daq_pxi lib
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__MI26_FFRioEmulDeserData1Mi26NoEChan ( UInt32* PtDestW32, SInt32 EltNb, UInt32* PtFirstFrameFC, SInt32 FrameNb  ) {
  
  EFRIO__TAcqEmul*     VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
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
  
  
  
  err_trace (( ERR_OUT, "DPXI__MI26_FFRioEmulDeserData1Mi26 (EltNb=%d, FrameNb=%d) !!!", EltNb, FrameNb ));
    
  // Pointers parameters check
  
  err_retnull ( PtDestW32     , (ERR_OUT,"PtDestW32 = NULL") );
  err_retnull ( PtFirstFrameFC, (ERR_OUT,"PtFirstFrameFC = NULL") );
    
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ); // It must be => ( (EltNb * 2) / (MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ * 2) );
  // But result is the SAME if we divide both part of fraction by 2 ;-)
    
  if ( VFrameNbFromBoardDrv != FrameNb ) {
    err_retfail ( -1, (ERR_OUT,"Dest buffer too small : %d frames buffer <> %d frames requested", VFrameNbFromBoardDrv, FrameNb ) );
  }
    
  // Alloc ZsFrameRaw
  
  VPtZsFFrameRaw = (MI26__TZsFFrameRaw*) malloc ( sizeof (MI26__TZsFFrameRaw) );
  
  err_retnull ( VPtZsFFrameRaw, (ERR_OUT,"Alloc ZsFFrameRaw failed !") );
    
  // Reset ZsFrameRaw
  
  memset ( VPtZsFFrameRaw, 0, sizeof (MI26__TZsFFrameRaw) );
    
  // Init ZsFrameRaw
  
  VPtZsFFrameRaw->Header     = VPtAcqEmul->ParAHeader[0];
  VPtZsFFrameRaw->FrameCnt   = *PtFirstFrameFC;
  
  if ( VPtAcqEmul->ParSetMaxDataSzOnOneMaps == 1 ) {
    VPtZsFFrameRaw->DataLength =  0x023A023A; // Max
  }
  
  else {
    VPtZsFFrameRaw->DataLength =  0x00200020;
  }
  
  VPtZsFFrameRaw->Trailer    = VPtAcqEmul->ParATrailer[0];
  
  VPtZsFFrameRaw->Zero       = 0x00030010;
  VPtZsFFrameRaw->Zero2      = 0x00200040;
  
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
  
  
  *PtFirstFrameFC += VPtZsFFrameRaw->FrameCnt;
  
  err_trace (( ERR_OUT, "End emul data loop" ));
  
  
  // Free ZsFrameRaw
  
  free ( VPtZsFFrameRaw );
  
  err_retok (( ERR_OUT, "ViFrame=%d - ViDestW32=%d", ViFrame, ViDestW32 ));
  
  return (0);
}



  
/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__MI26_FFRioEmulDeserData6Mi26NoEChan (
          :  UInt32* PtDestW32, SInt32 EltNb, UInt32* PtFirstFrameFC, SInt32 FrameNb  )
          :
Goal      : Emulate one acquisition for six Mi26 in data transfer mode IPHC or EUDET 1
          :
          : In this case there is no extra channel for TLU triggers.
          :
          : Triggers are stored in Zero fields of Mi26 frame, they represent line of Mi26
          : read while trigger occurs in 80 MHz clock unit -> divide by 16 to get line.
          : Maximum number of triggers is 3.
          :
          : The number of triggers is hard coded to 3, their values are
          : Trig[0] = 16 => line 1
          : Trig[1] = 32 => line 2
          : Trig[2] = 64 => line 4
          :
          :
Inputs    : PtDestW32        - Pointer to Flex RIO DRAM to overwrite
          : EltNb            - Flex RIO DRAM size in W32 unit ( 1 Elt = 1 W32 )
          :
          : PtFirstFrameFC   - Pointer to a variable which contains a global frame counter
          :                     Used to set frame counter of each frame of current acq
          :                     Incremented by the number of frames of current acq at end of function
          :
          : FrameNb          - The number of frame to emulate
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   :
          :
          :
Level     : This is a user level function.
Date      : 09/03/2010
Rev       : 10/03/2010
          : - Add parameter PtFirstFrameFC
          :
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__MI26_FFRioEmulDeserData6Mi26NoEChan ( UInt32* PtDestW32, SInt32 EltNb, UInt32* PtFirstFrameFC, SInt32 FrameNb  ) {
  
  EFRIO__TAcqEmul*     VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
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
  
      
  // Pointers parameters check
  
  err_retnull ( PtDestW32, (ERR_OUT,"PtDestW32 = NULL") );
  err_retnull ( PtFirstFrameFC, (ERR_OUT,"PtFirstFrameFC = NULL") );
    
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ) / 6; // It must be => ( (EltNb * 2) / (MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ * 2) );
  // But result is the SAME if we divide both part of fraction by 2 ;-)
  
  
  if ( VFrameNbFromBoardDrv != FrameNb ) {
    err_retfail ( -1, (ERR_OUT,"Dest buffer too small : %d frames buffer <> %d frames requested", VFrameNbFromBoardDrv, FrameNb ) );
  }
    
  // Alloc ZsFrameRaw
  
  VPtZsFFrameRaw = (MI26__TZsFFrameRaw*) malloc ( 6 * sizeof (MI26__TZsFFrameRaw) );
  
  err_retnull ( VPtZsFFrameRaw, (ERR_OUT,"Alloc ZsFFrameRaw failed !") );
    
  // Reset ZsFrameRaw
  
  memset ( VPtZsFFrameRaw, 0, 6 * sizeof (MI26__TZsFFrameRaw) );
    
  // Init ZsFrameRaw
  
  // RQ : Emulate trigger only on first Mi26 because it is used for trigger extraction
  // by readout function, information stored in Mi26 [2..5] are ignored
  
  VPtZsFFrameRaw[0].Header     = VPtAcqEmul->ParAHeader[0];
  VPtZsFFrameRaw[0].FrameCnt   = *PtFirstFrameFC;

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
  VPtZsFFrameRaw[1].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[1].DataLength = 0x00200020;
  VPtZsFFrameRaw[1].Trailer    = VPtAcqEmul->ParATrailer[1];
  VPtZsFFrameRaw[1].Zero       = 0x00000000;
  VPtZsFFrameRaw[1].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[2].Header     = VPtAcqEmul->ParAHeader[2];
  VPtZsFFrameRaw[2].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[2].DataLength = 0x00300030;
  VPtZsFFrameRaw[2].Trailer    = VPtAcqEmul->ParATrailer[2];
  VPtZsFFrameRaw[2].Zero       = 0x00000000;
  VPtZsFFrameRaw[2].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[3].Header     = VPtAcqEmul->ParAHeader[3];
  VPtZsFFrameRaw[3].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[3].DataLength = 0x00400040;
  VPtZsFFrameRaw[3].Trailer    = VPtAcqEmul->ParATrailer[3];
  VPtZsFFrameRaw[3].Zero       = 0x00000000;
  VPtZsFFrameRaw[3].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[4].Header     = VPtAcqEmul->ParAHeader[4];
  VPtZsFFrameRaw[4].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[4].DataLength = 0x00500050;
  VPtZsFFrameRaw[4].Trailer    = VPtAcqEmul->ParATrailer[4];
  VPtZsFFrameRaw[4].Zero       = 0x00000000;
  VPtZsFFrameRaw[4].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[5].Header     = VPtAcqEmul->ParAHeader[5];
  VPtZsFFrameRaw[5].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[5].DataLength = 0x00600060;
  VPtZsFFrameRaw[5].Trailer    = VPtAcqEmul->ParATrailer[5];
  VPtZsFFrameRaw[5].Zero       = 0x00000000;
  VPtZsFFrameRaw[5].Zero2      = 0x00000000;
      
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
  
  
  *PtFirstFrameFC += VPtZsFFrameRaw[0].FrameCnt;
    
  // Free ZsFrameRaw
  
  free ( VPtZsFFrameRaw );
    
  return (0);
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__MI26_FFRioEmulDeserData1Mi26EudetMode2 ( UInt32* PtDestW32,
          :  SInt32 EltNb, UInt32* PtFirstFrameFC, SInt32 FrameNb, SInt16 EmuleMode  )
          :
Goal      : Emulate one acquisition for one Mi26 in data transfer mode EUDET 2
          :
          : In this case there is an extra channel for TLU & Flex RIO trigger triggers.
          :
          : For each trigger two informations are stored in the following order
          : - TLU trigger                    -> see record EFRIO__TTluTrigger
          : - Flex RIO trigger / time stamp  -> see record EFRIO__TFlexRioTimeStamp1
          :
          : The EmuleMode parameter sets triggers nb.
          : The first three triggers + the last one are configured from GUI.
          : Their values are stored in fields ParATrig, ParATS of AcqEmul part 
          : ( type EFRIO__TAcqEmul ) of lib context record.
          : The other triggers are hard coded to 0.      
          :
          : In this case the three triggers stored in MI26 Zero fields are set to 0.
          : But he high 16 bits ( B31B16 ) of field Zero1 is still used to store the
          : number of triggers.
          :
          :
Inputs    : PtDestW32        - Pointer to Flex RIO DRAM to overwrite
          : EltNb            - Flex RIO DRAM size in W32 unit ( 1 Elt = 1 W32 )
          :
          : PtFirstFrameFC   - Pointer to a variable which contains a global frame counter
          :                     Used to set frame counter of each frame of current acq
          :                     Incremented by the number of frames of current acq at end of function
          :
          : FrameNb          - The number of frame to emulate
          :
          : EmuleMode        >= 0 -> No trigger emulation
          :                  <  0 -> Emulates | EmuleMode | on each frame
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   :
          :
Level     : This is a user level function.
Date      : 28/10/2010
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__MI26_FFRioEmulDeserData1Mi26EudetMode2 ( UInt32* PtDestW32, SInt32 EltNb, UInt32* PtFirstFrameFC, SInt32 FrameNb, SInt16 EmuleMode  ) {
  
  EFRIO__TAcqEmul*            VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
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
  err_retnull ( PtFirstFrameFC, (ERR_OUT,"PtFirstFrameFC = NULL") );
  
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
  VPtZsFFrameRaw->FrameCnt   = *PtFirstFrameFC;
  
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
  
  
  *PtFirstFrameFC += VPtZsFFrameRaw->FrameCnt;
  
  // Free ZsFrameRaw
  
  free ( VPtZsFFrameRaw );
  
  // Free trigger record
  
  free ( VPtTrigRec );
  
  
  err_retok (( ERR_OUT, "ViFrame=%d - ViDestW32=%d", ViFrame, ViDestW32 ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__MI26_FFRioEmulDeserData6Mi26EudetMode2 ( UInt32* PtDestW32,
          :  SInt32 EltNb, UInt32* PtFirstFrameFC, SInt32 FrameNb, SInt16 EmuleMode  )
          :
Goal      : Emulate one acquisition for one Mi26 in data transfer mode EUDET 2
          :
          : In this case there is an extra channel for TLU & Flex RIO trigger triggers.
          :
          : For each trigger two informations are stored in the following order
          : - TLU trigger                    -> see record EFRIO__TTluTrigger
          : - Flex RIO trigger / time stamp  -> see record EFRIO__TFlexRioTimeStamp1
          :
          : The EmuleMode parameter sets triggers nb.
          : The first three triggers + the last one are configured from GUI.
          : Their values are stored in fields ParATrig, ParATS of AcqEmul part
          : ( type EFRIO__TAcqEmul ) of lib context record.
          : The other triggers are hard coded to 0.
          :
          : In this case the three triggers stored in MI26 Zero fields are set to 0.
          : But he high 16 bits ( B31B16 ) of field Zero1 is still used to store the
          : number of triggers.
          :
          :
Inputs    : PtDestW32        - Pointer to Flex RIO DRAM to overwrite
          : EltNb            - Flex RIO DRAM size in W32 unit ( 1 Elt = 1 W32 )
          :
          : PtFirstFrameFC   - Pointer to a variable which contains a global frame counter
          :                     Used to set frame counter of each frame of current acq
          :                     Incremented by the number of frames of current acq at end of function
          :
          : FrameNb          - The number of frame to emulate
          :
          : EmuleMode        >= 0 -> No trigger emulation
          :                  <  0 -> Emulates | EmuleMode | on each frame
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   :
          :
Level     : This is a user level function.
Date      : 29/10/2010
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__MI26_FFRioEmulDeserData6Mi26EudetMode2 ( UInt32* PtDestW32, SInt32 EltNb, UInt32* PtFirstFrameFC, SInt32 FrameNb, SInt16 EmuleMode ) {
  
  EFRIO__TAcqEmul*     VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
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

  MI26__TStatesLine          VStateLine;
  MI26__TState               VState;
  SInt16                     ViMi26Line;
  SInt16                     ViDataW16;
  
  
  
  err_trace (( ERR_OUT, "EFRIO__MI26_FFRioEmulDeserData6Mi26EudetMode2 (EltNb=%d, FrameNb=%d, ... )", EltNb, FrameNb ));
    
  // Pointers parameters check
  
  err_retnull ( PtDestW32, (ERR_OUT,"PtDestW32 = NULL") );
  err_retnull ( PtFirstFrameFC, (ERR_OUT,"PtFirstFrameFC = NULL") );
    
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ) / 7; // Divide by 7 because of extral channel
  
  // It must be => ( (EltNb * 2) / (MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ * 2) );
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
  
  VPtZsFFrameRaw = (MI26__TZsFFrameRaw*) malloc ( 6 * sizeof (MI26__TZsFFrameRaw) );
  
  err_retnull ( VPtZsFFrameRaw, (ERR_OUT,"Alloc ZsFFrameRaw failed !") );
    
  // Alloc trigger record
  
  VPtTrigRec = (EFRIO__TTriggerRec*) malloc ( sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );
  
  err_retnull ( VPtTrigRec, (ERR_OUT,"Allocate tmp trigger buff failed !" ) );
  
  
  // Reset ZsFrameRaw
  
  memset ( VPtZsFFrameRaw, 0, 6 * sizeof (MI26__TZsFFrameRaw) );
    
  // Init ZsFrameRaw
  
  // RQ : Emulate trigger only on first Mi26 because it is used for trigger extraction
  // by readout function, information stored in Mi26 [2..5] are ignored
  
  VPtZsFFrameRaw[0].Header     = VPtAcqEmul->ParAHeader[0];
  VPtZsFFrameRaw[0].FrameCnt   = *PtFirstFrameFC;
    
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
  VPtZsFFrameRaw[1].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[1].DataLength = 0x00200020;
  VPtZsFFrameRaw[1].Trailer    = VPtAcqEmul->ParATrailer[1];
  VPtZsFFrameRaw[1].Zero       = 0x00000000;
  VPtZsFFrameRaw[1].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[2].Header     = VPtAcqEmul->ParAHeader[2];
  VPtZsFFrameRaw[2].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[2].DataLength = 0x00300030;
  VPtZsFFrameRaw[2].Trailer    = VPtAcqEmul->ParATrailer[2];
  VPtZsFFrameRaw[2].Zero       = 0x00000000;
  VPtZsFFrameRaw[2].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[3].Header     = VPtAcqEmul->ParAHeader[3];
  VPtZsFFrameRaw[3].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[3].DataLength = 0x00400040;
  VPtZsFFrameRaw[3].Trailer    = VPtAcqEmul->ParATrailer[3];
  VPtZsFFrameRaw[3].Zero       = 0x00000000;
  VPtZsFFrameRaw[3].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[4].Header     = VPtAcqEmul->ParAHeader[4];
  VPtZsFFrameRaw[4].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[4].DataLength = 0x00500050;
  VPtZsFFrameRaw[4].Trailer    = VPtAcqEmul->ParATrailer[4];
  VPtZsFFrameRaw[4].Zero       = 0x00000000;
  VPtZsFFrameRaw[4].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[5].Header     = VPtAcqEmul->ParAHeader[5];
  VPtZsFFrameRaw[5].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[5].DataLength = 0x00600060;
  VPtZsFFrameRaw[5].Trailer    = VPtAcqEmul->ParATrailer[5];
  VPtZsFFrameRaw[5].Zero       = 0x00000000;
  VPtZsFFrameRaw[5].Zero2      = 0x00000000;

  // 21/02/2011
  // Emulate data
  // Set hits on column No corresponding to Mi26 index from lines 0 to 569
  // - Column 0 for Mimosa 26 No 0
  // - etc ...
  // - Column 5 for Mimosa 26 No 5
  

  ViDataW16 = 0;
  
  for ( ViMi26Line=0; ViMi26Line < 570; ViMi26Line++ ) {
    
    VStateLine.F.StateNb  = 1;
    VStateLine.F.LineAddr = ViMi26Line;
    VStateLine.F.Ovf      = 0;
    
    // Write StateLine info to all Mi 26
    
    VPtZsFFrameRaw[0].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[1].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[2].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[3].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[4].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[5].ADataW16[ViDataW16] = VStateLine.W16;
    
    ++ViDataW16;
    
    VState.F.HitNb        = 0; // 0 => 1 hit !!!
    VState.F.NotUsed      = 0;
    
    // Write State info to all Mi 26 BUT set ColAddr = Mi26 No + 1
    
    VState.F.ColAddr = 0;
    VPtZsFFrameRaw[0].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 1;
    VPtZsFFrameRaw[1].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 2;
    VPtZsFFrameRaw[2].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 3;
    VPtZsFFrameRaw[3].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 4;
    VPtZsFFrameRaw[4].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 5;
    VPtZsFFrameRaw[5].ADataW16[ViDataW16] = VState.W16;
    
    ++ViDataW16;
    
  }
  



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
  
  
  *PtFirstFrameFC += VPtZsFFrameRaw[0].FrameCnt;
    
  
  // Free ZsFrameRaw
  
  free ( VPtZsFFrameRaw );
  
  err_retok (( ERR_OUT, "MsgOk" ));
  
  return (0);
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__MI26_FFRioEmulDeserData8Mi26EudetMode2 ( UInt32* PtDestW32,
          :  SInt32 EltNb, UInt32* PtFirstFrameFC, SInt32 FrameNb, SInt16 EmuleMode  )
          :
Goal      : Emulate one acquisition for one Mi26 in data transfer mode EUDET 2
          :
          : In this case there is an extra channel for TLU & Flex RIO trigger triggers.
          :
          : For each trigger two informations are stored in the following order
          : - TLU trigger                    -> see record EFRIO__TTluTrigger
          : - Flex RIO trigger / time stamp  -> see record EFRIO__TFlexRioTimeStamp1
          :
          : The EmuleMode parameter sets triggers nb.
          : The first three triggers + the last one are configured from GUI.
          : Their values are stored in fields ParATrig, ParATS of AcqEmul part
          : ( type EFRIO__TAcqEmul ) of lib context record.
          : The other triggers are hard coded to 0.
          :
          : In this case the three triggers stored in MI26 Zero fields are set to 0.
          : But he high 16 bits ( B31B16 ) of field Zero1 is still used to store the
          : number of triggers.
          :
          :
Inputs    : PtDestW32        - Pointer to Flex RIO DRAM to overwrite
          : EltNb            - Flex RIO DRAM size in W32 unit ( 1 Elt = 1 W32 )
          :
          : PtFirstFrameFC   - Pointer to a variable which contains a global frame counter
          :                     Used to set frame counter of each frame of current acq
          :                     Incremented by the number of frames of current acq at end of function
          :
          : FrameNb          - The number of frame to emulate
          :
          : EmuleMode        >= 0 -> No trigger emulation
          :                  <  0 -> Emulates | EmuleMode | on each frame
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   :
          :
Level     : This is a user level function.
Date      : 28/04/2011 ( Upgrade to 8 Mi26 version of 29/10/2010 )
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__MI26_FFRioEmulDeserData8Mi26EudetMode2 ( UInt32* PtDestW32, SInt32 EltNb, UInt32* PtFirstFrameFC, SInt32 FrameNb, SInt16 EmuleMode ) {
  
  EFRIO__TAcqEmul*     VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
  MI26__TZsFFrameRaw*  VPtZsFFrameRaw;
  SInt32               VFrameNbFromBoardDrv;
  SInt32               ViFrame;
  UInt32               VADataLengthField[8];
  UInt32               VADataLengthW16[8];
  UInt32               VADataLengthW32[8];
  SInt32               ViDestW32;
  SInt32               ViDataW32;
  UInt32*              VPtDataW32Chip0;
  UInt32*              VPtDataW32Chip1;
  UInt32*              VPtDataW32Chip2;
  UInt32*              VPtDataW32Chip3;
  UInt32*              VPtDataW32Chip4;
  UInt32*              VPtDataW32Chip5;
  UInt32*              VPtDataW32Chip6;
  UInt32*              VPtDataW32Chip7;
  EFRIO__TTriggerRec*        VPtTrigRec;
  SInt32                     ViEChanTrigField;
  SInt32                     VTrigNbToEmulate;
  SInt32                      VAEmulDataSzW16[8];
  SInt32                      ViTrigToEmulate;
  EFRIO__TTluTrigger*         VPtEmulTrig;
  EFRIO__TFlexRioTimeStamp1*  VPtEmulTs;
  
  MI26__TStatesLine          VStateLine;
  MI26__TState               VState;
  SInt16                     ViMi26Line;
  SInt16                     ViDataW16;
  
  
  
  err_trace (( ERR_OUT, "EFRIO__MI26_FFRioEmulDeserData6Mi26EudetMode2 (EltNb=%d, FrameNb=%d, ... )", EltNb, FrameNb ));
  
  // Pointers parameters check
  
  err_retnull ( PtDestW32, (ERR_OUT,"PtDestW32 = NULL") );
  err_retnull ( PtFirstFrameFC, (ERR_OUT,"PtFirstFrameFC = NULL") );
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ) / 9; // Divide by 9 because of extral channel
  
  // It must be => ( (EltNb * 2) / (MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ * 2) );
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
  
  VPtZsFFrameRaw = (MI26__TZsFFrameRaw*) malloc ( 8 * sizeof (MI26__TZsFFrameRaw) );
  
  err_retnull ( VPtZsFFrameRaw, (ERR_OUT,"Alloc ZsFFrameRaw failed !") );
  
  // Alloc trigger record
  
  VPtTrigRec = (EFRIO__TTriggerRec*) malloc ( sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );
  
  err_retnull ( VPtTrigRec, (ERR_OUT,"Allocate tmp trigger buff failed !" ) );
  
  
  // Reset ZsFrameRaw
  
  memset ( VPtZsFFrameRaw, 0, 8 * sizeof (MI26__TZsFFrameRaw) );
  
  // Init ZsFrameRaw
  
  // RQ : Emulate trigger only on first Mi26 because it is used for trigger extraction
  // by readout function, information stored in Mi26 [2..5] are ignored
  
  VPtZsFFrameRaw[0].Header     = VPtAcqEmul->ParAHeader[0];
  VPtZsFFrameRaw[0].FrameCnt   = *PtFirstFrameFC;
  
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
  VPtZsFFrameRaw[1].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[1].DataLength = 0x00200020;
  VPtZsFFrameRaw[1].Trailer    = VPtAcqEmul->ParATrailer[1];
  VPtZsFFrameRaw[1].Zero       = 0x00000000;
  VPtZsFFrameRaw[1].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[2].Header     = VPtAcqEmul->ParAHeader[2];
  VPtZsFFrameRaw[2].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[2].DataLength = 0x00300030;
  VPtZsFFrameRaw[2].Trailer    = VPtAcqEmul->ParATrailer[2];
  VPtZsFFrameRaw[2].Zero       = 0x00000000;
  VPtZsFFrameRaw[2].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[3].Header     = VPtAcqEmul->ParAHeader[3];
  VPtZsFFrameRaw[3].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[3].DataLength = 0x00400040;
  VPtZsFFrameRaw[3].Trailer    = VPtAcqEmul->ParATrailer[3];
  VPtZsFFrameRaw[3].Zero       = 0x00000000;
  VPtZsFFrameRaw[3].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[4].Header     = VPtAcqEmul->ParAHeader[4];
  VPtZsFFrameRaw[4].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[4].DataLength = 0x00500050;
  VPtZsFFrameRaw[4].Trailer    = VPtAcqEmul->ParATrailer[4];
  VPtZsFFrameRaw[4].Zero       = 0x00000000;
  VPtZsFFrameRaw[4].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[5].Header     = VPtAcqEmul->ParAHeader[5];
  VPtZsFFrameRaw[5].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[5].DataLength = 0x00600060;
  VPtZsFFrameRaw[5].Trailer    = VPtAcqEmul->ParATrailer[5];
  VPtZsFFrameRaw[5].Zero       = 0x00000000;
  VPtZsFFrameRaw[5].Zero2      = 0x00000000;

  VPtZsFFrameRaw[6].Header     = VPtAcqEmul->ParAHeader[6];
  VPtZsFFrameRaw[6].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[6].DataLength = 0x00700070;
  VPtZsFFrameRaw[6].Trailer    = VPtAcqEmul->ParATrailer[6];
  VPtZsFFrameRaw[6].Zero       = 0x00000000;
  VPtZsFFrameRaw[6].Zero2      = 0x00000000;

  VPtZsFFrameRaw[7].Header     = VPtAcqEmul->ParAHeader[7];
  VPtZsFFrameRaw[7].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[7].DataLength = 0x00800080;
  VPtZsFFrameRaw[7].Trailer    = VPtAcqEmul->ParATrailer[7];
  VPtZsFFrameRaw[7].Zero       = 0x00000000;
  VPtZsFFrameRaw[7].Zero2      = 0x00000000;
  
  // 21/02/2011
  // Emulate data
  // Set hits on column No corresponding to Mi26 index from lines 0 to 569
  // - Column 0 for Mimosa 26 No 0
  // - etc ...
  // - Column 7 for Mimosa 26 No 7
  
  
  ViDataW16 = 0;
  
  for ( ViMi26Line=0; ViMi26Line < 570; ViMi26Line++ ) {
    
    VStateLine.F.StateNb  = 1;
    VStateLine.F.LineAddr = ViMi26Line;
    VStateLine.F.Ovf      = 0;
    
    // Write StateLine info to all Mi 26
    
    VPtZsFFrameRaw[0].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[1].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[2].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[3].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[4].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[5].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[6].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[7].ADataW16[ViDataW16] = VStateLine.W16;
    
    ++ViDataW16;
    
    VState.F.HitNb        = 0; // 0 => 1 hit !!!
    VState.F.NotUsed      = 0;
    
    // Write State info to all Mi 26 BUT set ColAddr = Mi26 No + 1
    
    VState.F.ColAddr = 0;
    VPtZsFFrameRaw[0].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 1;
    VPtZsFFrameRaw[1].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 2;
    VPtZsFFrameRaw[2].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 3;
    VPtZsFFrameRaw[3].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 4;
    VPtZsFFrameRaw[4].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 5;
    VPtZsFFrameRaw[5].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 6;
    VPtZsFFrameRaw[6].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 7;
    VPtZsFFrameRaw[7].ADataW16[ViDataW16] = VState.W16;
    
    ++ViDataW16;
    
  }
  
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
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[6].Header;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[7].Header;
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
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[6].FrameCnt;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[7].FrameCnt;
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
      VAEmulDataSzW16[6] = random (571);
      VAEmulDataSzW16[7] = random (571);
      
      VADataLengthField[0] = VAEmulDataSzW16[0] + (VAEmulDataSzW16[0] << 16);
      VADataLengthField[1] = VAEmulDataSzW16[1] + (VAEmulDataSzW16[1] << 16);
      VADataLengthField[2] = VAEmulDataSzW16[2] + (VAEmulDataSzW16[2] << 16);
      VADataLengthField[3] = VAEmulDataSzW16[3] + (VAEmulDataSzW16[3] << 16);
      VADataLengthField[4] = VAEmulDataSzW16[4] + (VAEmulDataSzW16[4] << 16);
      VADataLengthField[5] = VAEmulDataSzW16[5] + (VAEmulDataSzW16[5] << 16);
      VADataLengthField[6] = VAEmulDataSzW16[6] + (VAEmulDataSzW16[6] << 16);
      VADataLengthField[7] = VAEmulDataSzW16[7] + (VAEmulDataSzW16[7] << 16);
    }
    
    else {
      VADataLengthField[0] = VPtZsFFrameRaw[0].DataLength;
      VADataLengthField[1] = VPtZsFFrameRaw[1].DataLength;
      VADataLengthField[2] = VPtZsFFrameRaw[2].DataLength;
      VADataLengthField[3] = VPtZsFFrameRaw[3].DataLength;
      VADataLengthField[4] = VPtZsFFrameRaw[4].DataLength;
      VADataLengthField[5] = VPtZsFFrameRaw[5].DataLength;
      VADataLengthField[6] = VPtZsFFrameRaw[6].DataLength;
      VADataLengthField[7] = VPtZsFFrameRaw[7].DataLength;
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

    PtDestW32[ViDestW32] = VADataLengthField[6];
    ++ViDestW32;

    PtDestW32[ViDestW32] = VADataLengthField[7];
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
    
    VADataLengthW16[6] = (VADataLengthField[6] & 0x0000FFFF) + ((VADataLengthField[6] & 0xFFFF0000) >> 16);
    VADataLengthW32[6] = VADataLengthW16[6] / 2;

    VADataLengthW16[7] = (VADataLengthField[7] & 0x0000FFFF) + ((VADataLengthField[7] & 0xFFFF0000) >> 16);
    VADataLengthW32[7] = VADataLengthW16[7] / 2;
    
    
    VPtDataW32Chip0 = (UInt32*) VPtZsFFrameRaw[0].ADataW16;
    VPtDataW32Chip1 = (UInt32*) VPtZsFFrameRaw[1].ADataW16;
    VPtDataW32Chip2 = (UInt32*) VPtZsFFrameRaw[2].ADataW16;
    VPtDataW32Chip3 = (UInt32*) VPtZsFFrameRaw[3].ADataW16;
    VPtDataW32Chip4 = (UInt32*) VPtZsFFrameRaw[4].ADataW16;
    VPtDataW32Chip5 = (UInt32*) VPtZsFFrameRaw[5].ADataW16;
    VPtDataW32Chip6 = (UInt32*) VPtZsFFrameRaw[6].ADataW16;
    VPtDataW32Chip7 = (UInt32*) VPtZsFFrameRaw[7].ADataW16;
    
    
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
      PtDestW32[ViDestW32] = VPtDataW32Chip6[ViDataW32];
      ++ViDestW32;
      PtDestW32[ViDestW32] = VPtDataW32Chip7[ViDataW32];
      ++ViDestW32;      
      PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
      ++ViDestW32;
      ++ViEChanTrigField;
    }
    
    
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 27 + (9 * VADataLengthW32[0])    ] = VPtZsFFrameRaw[0].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) +  9] = VPtZsFFrameRaw[0].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 18] = VPtZsFFrameRaw[0].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 1 + 27 + (9 * VADataLengthW32[1])    ] = VPtZsFFrameRaw[1].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 1 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 9] = VPtZsFFrameRaw[1].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 1 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 18] = VPtZsFFrameRaw[1].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 2 + 27 + (9 * VADataLengthW32[2])    ] = VPtZsFFrameRaw[2].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 2 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 9] = VPtZsFFrameRaw[2].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 2 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 18] = VPtZsFFrameRaw[2].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 3 + 27 + (9 * VADataLengthW32[3])    ] = VPtZsFFrameRaw[3].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 3 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 9] = VPtZsFFrameRaw[3].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 3 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 18] = VPtZsFFrameRaw[3].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 4 + 27 + (9 * VADataLengthW32[4])    ] = VPtZsFFrameRaw[4].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 4 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 9] = VPtZsFFrameRaw[4].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 4 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 18] = VPtZsFFrameRaw[4].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 5 + 27 + (9 * VADataLengthW32[5])    ] = VPtZsFFrameRaw[5].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 5 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 9] = VPtZsFFrameRaw[5].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 5 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 18] = VPtZsFFrameRaw[5].Zero2;
    ++ViDestW32;

    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 6 + 27 + (9 * VADataLengthW32[5])    ] = VPtZsFFrameRaw[6].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 6 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 9] = VPtZsFFrameRaw[6].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 6 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 18] = VPtZsFFrameRaw[6].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 7 + 27 + (9 * VADataLengthW32[5])    ] = VPtZsFFrameRaw[7].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 7 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 9] = VPtZsFFrameRaw[7].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 7 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 18] = VPtZsFFrameRaw[7].Zero2;
    ++ViDestW32;
    
    // PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 8 + 27 + (9 * VADataLengthW32[5])    ] = VPtTrigRec->ATrig[ViEChanTrigField];;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 8 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32)    ] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 8 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 9] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 8 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 18] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    
    
    // Update ZsFrameRaw fields
    
    ++VPtZsFFrameRaw[0].FrameCnt;
    ++VPtZsFFrameRaw[1].FrameCnt;
    ++VPtZsFFrameRaw[2].FrameCnt;
    ++VPtZsFFrameRaw[3].FrameCnt;
    ++VPtZsFFrameRaw[4].FrameCnt;
    ++VPtZsFFrameRaw[5].FrameCnt;
    ++VPtZsFFrameRaw[6].FrameCnt;
    ++VPtZsFFrameRaw[7].FrameCnt;
    
  } // End for ViFrame
  
  
  *PtFirstFrameFC += VPtZsFFrameRaw[0].FrameCnt;
  
  
  // Free ZsFrameRaw
  
  free ( VPtZsFFrameRaw );
  
  err_retok (( ERR_OUT, "MsgOk" ));
  
  return (0);
}



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__MI26_FFRioEmulDeserData1Mi26EudetMode3 ( UInt32* PtDestW32,
          :  SInt32 EltNb, UInt32* PtFirstFrameFC, SInt32 FrameNb, SInt16 EmuleMode  )
          :
Goal      : Emulate one acquisition for one Mi26 in data transfer mode EUDET 3
          :
          : In this case there is an extra channel for TLU & Flex RIO trigger triggers.
          :
          : For each trigger two informations are stored in the following order
          : - TLU trigger                    -> see record EFRIO__TTluTrigger
          : - Flex RIO trigger / time stamp  -> see record EFRIO__TFlexRioTimeStamp1
          :
          : The EmuleMode parameter sets triggers nb.
          : The first three triggers + the last one are configured from GUI.
          : Their values are stored in fields ParATrig, ParATS of AcqEmul part
          : ( type EFRIO__TAcqEmul ) of lib context record.
          : The other triggers are hard coded to 0.
          :
          :***************************************************************************
          : For this data transfer mode - EUDET 3 - The triggers are not emulated on
          : each frame but on M consecutive frames each N frames. M & N are defined
          : from GUI, they are stored in fields ParTrigOnNConsecutiveFrames = M and
          : ParTrigOnOneFrameOverN = N of AcqEmul part ( type EFRIO__TAcqEmul ) of lib
          : context record.
          :***************************************************************************
          :
          : In this case the three triggers stored in MI26 Zero fields are set to 0.
          : But he high 16 bits ( B31B16 ) of field Zero1 is still used to store the
          : number of triggers.
          :
          :
Inputs    : PtDestW32        - Pointer to Flex RIO DRAM to overwrite
          : EltNb            - Flex RIO DRAM size in W32 unit ( 1 Elt = 1 W32 )
          :
          : PtFirstFrameFC   - Pointer to a variable which contains a global frame counter
          :                     Used to set frame counter of each frame of current acq
          :                     Incremented by the number of frames of current acq at end of function
          :
          : FrameNb          - The number of frame to emulate
          :
          : EmuleMode        >= 0 -> No trigger emulation
          :                  <  0 -> Emulates | EmuleMode | on each frame
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   :
          :
Level     : This is a user level function.
Date      : 03/11/2010
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__MI26_FFRioEmulDeserData1Mi26EudetMode3 ( UInt32* PtDestW32, SInt32 EltNb, UInt32* PtFirstFrameFC, SInt32 FrameNb, SInt16 EmuleMode  ) {
  
  EFRIO__TAcqEmul*            VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
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
  
  MI26__TStatesLine           VStateLine;
  MI26__TState                VState;
  SInt16                      ViMi26Line;
  SInt16                      ViDataW16;
  
  
  err_trace (( ERR_OUT, "Begin" ));
  
  err_trace (( ERR_OUT, "EFRIO__MI26_FFRioEmulDeserData1Mi26EudetMode3 (EltNb=%d, FrameNb=%d) !!!", EltNb, FrameNb ));
  
  // Pointers parameters check
  
  err_retnull ( PtDestW32     , (ERR_OUT,"PtDestW32 = NULL") );
  err_retnull ( PtFirstFrameFC, (ERR_OUT,"PtFirstFrameFC = NULL") );
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ / 2 ); // Divide by 2 because of extra channel
  
  // It must be => ( (EltNb * 2) / (MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ * 2) / 2 );
  // But result is the SAME if we divide both part of fraction by 2 ;-)
  
  if ( VFrameNbFromBoardDrv != FrameNb ) {
    err_retfail ( -1, (ERR_OUT,"Dest buffer too small : %d frames buffer <> %d frames requested", VFrameNbFromBoardDrv, FrameNb ) );
  }
  
  // Check ParTrigOnOneFrameOverN --> Will create div by 0 error if NULL
  
  if ( VPtAcqEmul->ParTrigOnOneFrameOverN <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => ParTrigOnOneFrameOverN=%d <= 0 !", VPtAcqEmul->ParTrigOnOneFrameOverN) );
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
  VPtZsFFrameRaw->FrameCnt   = *PtFirstFrameFC;
  
  if ( VPtAcqEmul->ParSetMaxDataSzOnOneMaps == 1 ) {
    VPtZsFFrameRaw->DataLength =  0x023A023A; // Max
  }
  
  else {
    VPtZsFFrameRaw->DataLength = 0x00200020;
  }
  
  VPtZsFFrameRaw->Trailer    = VPtAcqEmul->ParATrailer[0];
  VPtZsFFrameRaw->Zero       = 0; // High W16 = trigger nb => Set in frames loop because it depends on frame Id
  VPtZsFFrameRaw->Zero2      = 0;
  
  // 22/12/2010
  // Emulate data
  // Set hits on column 1 from lines 0 to 569
  
  
  ViDataW16 = 0;
  
  for ( ViMi26Line=0; ViMi26Line < 570; ViMi26Line++ ) {
    
    VStateLine.F.StateNb  = 1;
    VStateLine.F.LineAddr = ViMi26Line;
    VStateLine.F.Ovf      = 0;
  
    VPtZsFFrameRaw->ADataW16[ViDataW16] = VStateLine.W16;
    ++ViDataW16;
  
    VState.F.HitNb        = 0; // 0 => 1 hit !!!
    VState.F.ColAddr      = 0;
    VState.F.NotUsed      = 0;

    VPtZsFFrameRaw->ADataW16[ViDataW16] = VState.W16;
    ++ViDataW16;
    
  }
  
  
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
  
  
  *PtFirstFrameFC += VPtZsFFrameRaw->FrameCnt;
  
  // Free ZsFrameRaw
  
  free ( VPtZsFFrameRaw );
  
  // Free trigger record
  
  free ( VPtTrigRec );
  
  err_trace (( ERR_OUT, "End" ));
  
  err_retok (( ERR_OUT, "ViFrame=%d - ViDestW32=%d", ViFrame, ViDestW32 ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__MI26_FFRioEmulDeserData6Mi26EudetMode3 ( UInt32* PtDestW32,
          :  SInt32 EltNb, UInt32* PtFirstFrameFC, SInt32 FrameNb, SInt16 EmuleMode  )
          :
Goal      : Emulate one acquisition for one Mi26 in data transfer mode EUDET 3
          :
          : In this case there is an extra channel for TLU & Flex RIO trigger triggers.
          :
          : For each trigger two informations are stored in the following order
          : - TLU trigger                    -> see record EFRIO__TTluTrigger
          : - Flex RIO trigger / time stamp  -> see record EFRIO__TFlexRioTimeStamp1
          :
          : The EmuleMode parameter sets triggers nb.
          : The first three triggers + the last one are configured from GUI.
          : Their values are stored in fields ParATrig, ParATS of AcqEmul part
          : ( type EFRIO__TAcqEmul ) of lib context record.
          : The other triggers are hard coded to 0.
          :
          :***************************************************************************
          : For this data transfer mode - EUDET 3 - The triggers are not emulated on
          : each frame but on M consecutive frames each N frames. M & N are defined
          : from GUI, they are stored in fields ParTrigOnNConsecutiveFrames = M and
          : ParTrigOnOneFrameOverN = N of AcqEmul part ( type EFRIO__TAcqEmul ) of lib
          : context record.
          :***************************************************************************
          :
          : In this case the three triggers stored in MI26 Zero fields are set to 0.
          : But he high 16 bits ( B31B16 ) of field Zero1 is still used to store the
          : number of triggers.
          :
          :
Inputs    : PtDestW32        - Pointer to Flex RIO DRAM to overwrite
          : EltNb            - Flex RIO DRAM size in W32 unit ( 1 Elt = 1 W32 )
          :
          : PtFirstFrameFC   - Pointer to a variable which contains a global frame counter
          :                     Used to set frame counter of each frame of current acq
          :                     Incremented by the number of frames of current acq at end of function
          :
          : FrameNb          - The number of frame to emulate
          :
          : EmuleMode        >= 0 -> No trigger emulation
          :                  <  0 -> Emulates | EmuleMode | on each frame
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   :
          :
Level     : This is a user level function.
Date      : 03/11/2010
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__MI26_FFRioEmulDeserData6Mi26EudetMode3 ( UInt32* PtDestW32, SInt32 EltNb, UInt32* PtFirstFrameFC, SInt32 FrameNb, SInt16 EmuleMode ) {
  
  EFRIO__TAcqEmul*           VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
  MI26__TZsFFrameRaw*        VPtZsFFrameRaw;
  SInt32                     VFrameNbFromBoardDrv;
  SInt32                     ViFrame;
  UInt32                     VADataLengthField[6];
  UInt32                     VADataLengthW16[6];
  UInt32                     VADataLengthW32[6];
  SInt32                     ViDestW32;
  SInt32                     ViDataW32;
  UInt32*                    VPtDataW32Chip0;
  UInt32*                    VPtDataW32Chip1;
  UInt32*                    VPtDataW32Chip2;
  UInt32*                    VPtDataW32Chip3;
  UInt32*                    VPtDataW32Chip4;
  UInt32*                    VPtDataW32Chip5;
  EFRIO__TTriggerRec*        VPtTrigRec;
  SInt32                     ViEChanTrigField;
  SInt32                     VTrigNbToEmulate;
  SInt32                     VAEmulDataSzW16[6];
  SInt32                     ViTrigToEmulate;
  EFRIO__TTluTrigger*        VPtEmulTrig;
  EFRIO__TFlexRioTimeStamp1* VPtEmulTs;

  MI26__TStatesLine          VStateLine;
  MI26__TState               VState;
  SInt16                     ViMi26Line;
  SInt16                     ViDataW16;
  
  
  
  err_trace (( ERR_OUT, "EFRIO__MI26_FFRioEmulDeserData6Mi26EudetMode3 (EltNb=%d, FrameNb=%d, ... )", EltNb, FrameNb ));
    
  // Pointers parameters check
  
  err_retnull ( PtDestW32, (ERR_OUT,"PtDestW32 = NULL") );
  err_retnull ( PtFirstFrameFC, (ERR_OUT,"PtFirstFrameFC = NULL") );
    
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ) / 7; // Divide by 7 because of extral channel
  
  // It must be => ( (EltNb * 2) / (MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ * 2) );
  // But result is the SAME if we divide both part of fraction by 2 ;-)
  
  
  if ( VFrameNbFromBoardDrv != FrameNb ) {
    err_retfail ( -1, (ERR_OUT,"Dest buffer too small : %d frames buffer <> %d frames requested", VFrameNbFromBoardDrv, FrameNb ) );
  }

  // Check ParTrigOnOneFrameOverN --> Will create div by 0 error if NULL
  
  if ( VPtAcqEmul->ParTrigOnOneFrameOverN <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => ParTrigOnOneFrameOverN=%d <= 0 !", VPtAcqEmul->ParTrigOnOneFrameOverN) );
  }
    
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
   
  // Alloc trigger record
  
  VPtTrigRec = (EFRIO__TTriggerRec*) malloc ( sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );
  
  err_retnull ( VPtTrigRec, (ERR_OUT,"Allocate tmp trigger buff failed !" ) );
  
  
  // Reset ZsFrameRaw
  
  memset ( VPtZsFFrameRaw, 0, 6 * sizeof (MI26__TZsFFrameRaw) );
    
  // Init ZsFrameRaw
  
  // RQ : Emulate trigger only on first Mi26 because it is used for trigger extraction
  // by readout function, information stored in Mi26 [2..5] are ignored
  
  VPtZsFFrameRaw[0].Header     = VPtAcqEmul->ParAHeader[0];
  VPtZsFFrameRaw[0].FrameCnt   = *PtFirstFrameFC;
  
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
  VPtZsFFrameRaw[1].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[1].DataLength = 0x00200020;
  VPtZsFFrameRaw[1].Trailer    = VPtAcqEmul->ParATrailer[1];
  VPtZsFFrameRaw[1].Zero       = 0x00000000;
  VPtZsFFrameRaw[1].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[2].Header     = VPtAcqEmul->ParAHeader[2];
  VPtZsFFrameRaw[2].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[2].DataLength = 0x00300030;
  VPtZsFFrameRaw[2].Trailer    = VPtAcqEmul->ParATrailer[2];
  VPtZsFFrameRaw[2].Zero       = 0x00000000;
  VPtZsFFrameRaw[2].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[3].Header     = VPtAcqEmul->ParAHeader[3];
  VPtZsFFrameRaw[3].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[3].DataLength = 0x00400040;
  VPtZsFFrameRaw[3].Trailer    = VPtAcqEmul->ParATrailer[3];
  VPtZsFFrameRaw[3].Zero       = 0x00000000;
  VPtZsFFrameRaw[3].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[4].Header     = VPtAcqEmul->ParAHeader[4];
  VPtZsFFrameRaw[4].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[4].DataLength = 0x00500050;
  VPtZsFFrameRaw[4].Trailer    = VPtAcqEmul->ParATrailer[4];
  VPtZsFFrameRaw[4].Zero       = 0x00000000;
  VPtZsFFrameRaw[4].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[5].Header     = VPtAcqEmul->ParAHeader[5];
  VPtZsFFrameRaw[5].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[5].DataLength = 0x00600060;
  VPtZsFFrameRaw[5].Trailer    = VPtAcqEmul->ParATrailer[5];
  VPtZsFFrameRaw[5].Zero       = 0x00000000;
  VPtZsFFrameRaw[5].Zero2      = 0x00000000;


  // 30/12/2010
  // Emulate data
  // Set hits on column No corresponding to Mi26 index from lines 0 to 569
  // - Column 0 for Mimosa 26 No 0
  // - etc ...
  // - Column 5 for Mimosa 26 No 5
  
  ViDataW16 = 0;
  
  for ( ViMi26Line=0; ViMi26Line < 570; ViMi26Line++ ) {
    
    VStateLine.F.StateNb  = 1;
    VStateLine.F.LineAddr = ViMi26Line;
    VStateLine.F.Ovf      = 0;

    // Write StateLine info to all Mi 26 

    VPtZsFFrameRaw[0].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[1].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[2].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[3].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[4].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[5].ADataW16[ViDataW16] = VStateLine.W16;
    
    ++ViDataW16;

    VState.F.HitNb        = 0; // 0 => 1 hit !!!
    VState.F.NotUsed      = 0;

    // Write State info to all Mi 26 BUT set ColAddr = Mi26 No + 1
    
    VState.F.ColAddr = 0;
    VPtZsFFrameRaw[0].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 1;
    VPtZsFFrameRaw[1].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 2;
    VPtZsFFrameRaw[2].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 3;
    VPtZsFFrameRaw[3].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 4;
    VPtZsFFrameRaw[4].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 5;
    VPtZsFFrameRaw[5].ADataW16[ViDataW16] = VState.W16;
    
    ++ViDataW16;
    
  }
  
  

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
  
  
  *PtFirstFrameFC += VPtZsFFrameRaw[0].FrameCnt;
    
  
  // Free ZsFrameRaw
  
  free ( VPtZsFFrameRaw );
  
  err_retok (( ERR_OUT, "MsgOk" ));
  
  return (0);
}




/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__MI26_FFRioEmulDeserData8Mi26EudetMode3 ( UInt32* PtDestW32,
          :  SInt32 EltNb, UInt32* PtFirstFrameFC, SInt32 FrameNb, SInt16 EmuleMode  )
          :
Goal      : Emulate one acquisition for one Mi26 in data transfer mode EUDET 3
          :
          : In this case there is an extra channel for TLU & Flex RIO trigger triggers.
          :
          : For each trigger two informations are stored in the following order
          : - TLU trigger                    -> see record EFRIO__TTluTrigger
          : - Flex RIO trigger / time stamp  -> see record EFRIO__TFlexRioTimeStamp1
          :
          : The EmuleMode parameter sets triggers nb.
          : The first three triggers + the last one are configured from GUI.
          : Their values are stored in fields ParATrig, ParATS of AcqEmul part
          : ( type EFRIO__TAcqEmul ) of lib context record.
          : The other triggers are hard coded to 0.
          :
          :***************************************************************************
          : For this data transfer mode - EUDET 3 - The triggers are not emulated on
          : each frame but on M consecutive frames each N frames. M & N are defined
          : from GUI, they are stored in fields ParTrigOnNConsecutiveFrames = M and
          : ParTrigOnOneFrameOverN = N of AcqEmul part ( type EFRIO__TAcqEmul ) of lib
          : context record.
          :***************************************************************************
          :
          : In this case the three triggers stored in MI26 Zero fields are set to 0.
          : But he high 16 bits ( B31B16 ) of field Zero1 is still used to store the
          : number of triggers.
          :
          :
Inputs    : PtDestW32        - Pointer to Flex RIO DRAM to overwrite
          : EltNb            - Flex RIO DRAM size in W32 unit ( 1 Elt = 1 W32 )
          :
          : PtFirstFrameFC   - Pointer to a variable which contains a global frame counter
          :                     Used to set frame counter of each frame of current acq
          :                     Incremented by the number of frames of current acq at end of function
          :
          : FrameNb          - The number of frame to emulate
          :
          : EmuleMode        >= 0 -> No trigger emulation
          :                  <  0 -> Emulates | EmuleMode | on each frame
          :
Ouputs    : The function returns
          :  0 if ok
          : -1 if an error occurs
          :
Globals   :
          :
Level     : This is a user level function.
Date      : 28/04/2011 ( Upgrade to 8 Mi26 of 03/11/2010 version )
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__MI26_FFRioEmulDeserData8Mi26EudetMode3 ( UInt32* PtDestW32, SInt32 EltNb, UInt32* PtFirstFrameFC, SInt32 FrameNb, SInt16 EmuleMode ) {
  
  EFRIO__TAcqEmul*           VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
  MI26__TZsFFrameRaw*        VPtZsFFrameRaw;
  SInt32                     VFrameNbFromBoardDrv;
  SInt32                     ViFrame;
  UInt32                     VADataLengthField[8];
  UInt32                     VADataLengthW16[8];
  UInt32                     VADataLengthW32[8];
  SInt32                     ViDestW32;
  SInt32                     ViDataW32;
  UInt32*                    VPtDataW32Chip0;
  UInt32*                    VPtDataW32Chip1;
  UInt32*                    VPtDataW32Chip2;
  UInt32*                    VPtDataW32Chip3;
  UInt32*                    VPtDataW32Chip4;
  UInt32*                    VPtDataW32Chip5;
  UInt32*                    VPtDataW32Chip6;
  UInt32*                    VPtDataW32Chip7;
  EFRIO__TTriggerRec*        VPtTrigRec;
  SInt32                     ViEChanTrigField;
  SInt32                     VTrigNbToEmulate;
  SInt32                     VAEmulDataSzW16[8];
  SInt32                     ViTrigToEmulate;
  EFRIO__TTluTrigger*        VPtEmulTrig;
  EFRIO__TFlexRioTimeStamp1* VPtEmulTs;
  
  MI26__TStatesLine          VStateLine;
  MI26__TState               VState;
  SInt16                     ViMi26Line;
  SInt16                     ViDataW16;
  
  
  
  err_trace (( ERR_OUT, "EFRIO__MI26_FFRioEmulDeserData8Mi26EudetMode3 (EltNb=%d, FrameNb=%d, ... )", EltNb, FrameNb ));
  
  // Pointers parameters check
  
  err_retnull ( PtDestW32, (ERR_OUT,"PtDestW32 = NULL") );
  err_retnull ( PtFirstFrameFC, (ERR_OUT,"PtFirstFrameFC = NULL") );
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ ) / 9; // Divide by 9 because of extral channel
  
  // It must be => ( (EltNb * 2) / (MI26__ZS_FFRAME_MODE_2X80MHZ_W16_SZ * 2) );
  // But result is the SAME if we divide both part of fraction by 2 ;-)
  
  
  if ( VFrameNbFromBoardDrv != FrameNb ) {
    err_retfail ( -1, (ERR_OUT,"Dest buffer too small : %d frames buffer <> %d frames requested", VFrameNbFromBoardDrv, FrameNb ) );
  }
  
  // Check ParTrigOnOneFrameOverN --> Will create div by 0 error if NULL
  
  if ( VPtAcqEmul->ParTrigOnOneFrameOverN <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => ParTrigOnOneFrameOverN=%d <= 0 !", VPtAcqEmul->ParTrigOnOneFrameOverN) );
  }
  
  // Calculate nb of trigger to emulate
  
  if ( EmuleMode >= 0 ) {
    VTrigNbToEmulate = 0;
  }
  
  else {
    VTrigNbToEmulate = abs ( EmuleMode );
  }
  
  
  // Alloc ZsFrameRaw
  
  VPtZsFFrameRaw = (MI26__TZsFFrameRaw*) malloc ( 8 * sizeof (MI26__TZsFFrameRaw) );
  
  err_retnull ( VPtZsFFrameRaw, (ERR_OUT,"Alloc ZsFFrameRaw failed !") );
  
  // Alloc trigger record
  
  VPtTrigRec = (EFRIO__TTriggerRec*) malloc ( sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );
  
  err_retnull ( VPtTrigRec, (ERR_OUT,"Allocate tmp trigger buff failed !" ) );
  
  
  // Reset ZsFrameRaw
  
  memset ( VPtZsFFrameRaw, 0, 8 * sizeof (MI26__TZsFFrameRaw) );
  
  // Init ZsFrameRaw
  
  // RQ : Emulate trigger only on first Mi26 because it is used for trigger extraction
  // by readout function, information stored in Mi26 [2..5] are ignored
  
  VPtZsFFrameRaw[0].Header     = VPtAcqEmul->ParAHeader[0];
  VPtZsFFrameRaw[0].FrameCnt   = *PtFirstFrameFC;
  
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
  VPtZsFFrameRaw[1].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[1].DataLength = 0x00200020;
  VPtZsFFrameRaw[1].Trailer    = VPtAcqEmul->ParATrailer[1];
  VPtZsFFrameRaw[1].Zero       = 0x00000000;
  VPtZsFFrameRaw[1].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[2].Header     = VPtAcqEmul->ParAHeader[2];
  VPtZsFFrameRaw[2].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[2].DataLength = 0x00300030;
  VPtZsFFrameRaw[2].Trailer    = VPtAcqEmul->ParATrailer[2];
  VPtZsFFrameRaw[2].Zero       = 0x00000000;
  VPtZsFFrameRaw[2].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[3].Header     = VPtAcqEmul->ParAHeader[3];
  VPtZsFFrameRaw[3].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[3].DataLength = 0x00400040;
  VPtZsFFrameRaw[3].Trailer    = VPtAcqEmul->ParATrailer[3];
  VPtZsFFrameRaw[3].Zero       = 0x00000000;
  VPtZsFFrameRaw[3].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[4].Header     = VPtAcqEmul->ParAHeader[4];
  VPtZsFFrameRaw[4].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[4].DataLength = 0x00500050;
  VPtZsFFrameRaw[4].Trailer    = VPtAcqEmul->ParATrailer[4];
  VPtZsFFrameRaw[4].Zero       = 0x00000000;
  VPtZsFFrameRaw[4].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[5].Header     = VPtAcqEmul->ParAHeader[5];
  VPtZsFFrameRaw[5].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[5].DataLength = 0x00600060;
  VPtZsFFrameRaw[5].Trailer    = VPtAcqEmul->ParATrailer[5];
  VPtZsFFrameRaw[5].Zero       = 0x00000000;
  VPtZsFFrameRaw[5].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[6].Header     = VPtAcqEmul->ParAHeader[6];
  VPtZsFFrameRaw[6].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[6].DataLength = 0x00700070;
  VPtZsFFrameRaw[6].Trailer    = VPtAcqEmul->ParATrailer[6];
  VPtZsFFrameRaw[6].Zero       = 0x00000000;
  VPtZsFFrameRaw[6].Zero2      = 0x00000000;

  VPtZsFFrameRaw[7].Header     = VPtAcqEmul->ParAHeader[7];
  VPtZsFFrameRaw[7].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[7].DataLength = 0x00800080;
  VPtZsFFrameRaw[7].Trailer    = VPtAcqEmul->ParATrailer[7];
  VPtZsFFrameRaw[7].Zero       = 0x00000000;
  VPtZsFFrameRaw[7].Zero2      = 0x00000000;
  
  // 30/12/2010
  // Emulate data
  // Set hits on column No corresponding to Mi26 index from lines 0 to 569
  // - Column 0 for Mimosa 26 No 0
  // - etc ...
  // - Column 7 for Mimosa 26 No 7
  
  ViDataW16 = 0;
  
  for ( ViMi26Line=0; ViMi26Line < 570; ViMi26Line++ ) {
    
    VStateLine.F.StateNb  = 1;
    VStateLine.F.LineAddr = ViMi26Line;
    VStateLine.F.Ovf      = 0;
    
    // Write StateLine info to all Mi 26
    
    VPtZsFFrameRaw[0].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[1].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[2].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[3].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[4].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[5].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[6].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[7].ADataW16[ViDataW16] = VStateLine.W16;
    
    ++ViDataW16;
    
    VState.F.HitNb        = 0; // 0 => 1 hit !!!
    VState.F.NotUsed      = 0;
    
    // Write State info to all Mi 26 BUT set ColAddr = Mi26 No + 1
    
    VState.F.ColAddr = 0;
    VPtZsFFrameRaw[0].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 1;
    VPtZsFFrameRaw[1].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 2;
    VPtZsFFrameRaw[2].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 3;
    VPtZsFFrameRaw[3].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 4;
    VPtZsFFrameRaw[4].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 5;
    VPtZsFFrameRaw[5].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 6;
    VPtZsFFrameRaw[6].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 7;
    VPtZsFFrameRaw[7].ADataW16[ViDataW16] = VState.W16;
    
    ++ViDataW16;
    
  }
  
  
  
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
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[6].Header;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[7].Header;
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
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[6].FrameCnt;
    ++ViDestW32;
    PtDestW32[ViDestW32] = VPtZsFFrameRaw[7].FrameCnt;
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
      VAEmulDataSzW16[6] = random (571);
      VAEmulDataSzW16[7] = random (571);
      
      VADataLengthField[0] = VAEmulDataSzW16[0] + (VAEmulDataSzW16[0] << 16);
      VADataLengthField[1] = VAEmulDataSzW16[1] + (VAEmulDataSzW16[1] << 16);
      VADataLengthField[2] = VAEmulDataSzW16[2] + (VAEmulDataSzW16[2] << 16);
      VADataLengthField[3] = VAEmulDataSzW16[3] + (VAEmulDataSzW16[3] << 16);
      VADataLengthField[4] = VAEmulDataSzW16[4] + (VAEmulDataSzW16[4] << 16);
      VADataLengthField[5] = VAEmulDataSzW16[5] + (VAEmulDataSzW16[5] << 16);
      VADataLengthField[6] = VAEmulDataSzW16[6] + (VAEmulDataSzW16[6] << 16);
      VADataLengthField[7] = VAEmulDataSzW16[7] + (VAEmulDataSzW16[7] << 16);
    }
    
    else {
      VADataLengthField[0] = VPtZsFFrameRaw[0].DataLength;
      VADataLengthField[1] = VPtZsFFrameRaw[1].DataLength;
      VADataLengthField[2] = VPtZsFFrameRaw[2].DataLength;
      VADataLengthField[3] = VPtZsFFrameRaw[3].DataLength;
      VADataLengthField[4] = VPtZsFFrameRaw[4].DataLength;
      VADataLengthField[5] = VPtZsFFrameRaw[5].DataLength;
      VADataLengthField[6] = VPtZsFFrameRaw[6].DataLength;
      VADataLengthField[7] = VPtZsFFrameRaw[7].DataLength;
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

    PtDestW32[ViDestW32] = VADataLengthField[6];
    ++ViDestW32;

    PtDestW32[ViDestW32] = VADataLengthField[7];
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
    
    VADataLengthW16[6] = (VADataLengthField[6] & 0x0000FFFF) + ((VADataLengthField[6] & 0xFFFF0000) >> 16);
    VADataLengthW32[6] = VADataLengthW16[6] / 2;

    VADataLengthW16[7] = (VADataLengthField[7] & 0x0000FFFF) + ((VADataLengthField[7] & 0xFFFF0000) >> 16);
    VADataLengthW32[7] = VADataLengthW16[7] / 2;
    
    
    VPtDataW32Chip0 = (UInt32*) VPtZsFFrameRaw[0].ADataW16;
    VPtDataW32Chip1 = (UInt32*) VPtZsFFrameRaw[1].ADataW16;
    VPtDataW32Chip2 = (UInt32*) VPtZsFFrameRaw[2].ADataW16;
    VPtDataW32Chip3 = (UInt32*) VPtZsFFrameRaw[3].ADataW16;
    VPtDataW32Chip4 = (UInt32*) VPtZsFFrameRaw[4].ADataW16;
    VPtDataW32Chip5 = (UInt32*) VPtZsFFrameRaw[5].ADataW16;
    VPtDataW32Chip6 = (UInt32*) VPtZsFFrameRaw[6].ADataW16;
    VPtDataW32Chip7 = (UInt32*) VPtZsFFrameRaw[7].ADataW16;
    
    
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
      PtDestW32[ViDestW32] = VPtDataW32Chip6[ViDataW32];
      ++ViDestW32;
      PtDestW32[ViDestW32] = VPtDataW32Chip7[ViDataW32];
      ++ViDestW32;
      PtDestW32[ViDestW32] = VPtTrigRec->ATrig[ViEChanTrigField];
      ++ViDestW32;
      ++ViEChanTrigField;
    }
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 27 + (9 * VADataLengthW32[0])    ] = VPtZsFFrameRaw[0].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) +  9] = VPtZsFFrameRaw[0].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 18] = VPtZsFFrameRaw[0].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 1 + 27 + (9 * VADataLengthW32[1])    ] = VPtZsFFrameRaw[1].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 1 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 9] = VPtZsFFrameRaw[1].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 1 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 18] = VPtZsFFrameRaw[1].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 2 + 27 + (9 * VADataLengthW32[2])    ] = VPtZsFFrameRaw[2].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 2 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 9] = VPtZsFFrameRaw[2].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 2 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 18] = VPtZsFFrameRaw[2].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 3 + 27 + (9 * VADataLengthW32[3])    ] = VPtZsFFrameRaw[3].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 3 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 9] = VPtZsFFrameRaw[3].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 3 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 18] = VPtZsFFrameRaw[3].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 4 + 27 + (9 * VADataLengthW32[4])    ] = VPtZsFFrameRaw[4].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 4 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 9] = VPtZsFFrameRaw[4].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 4 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 18] = VPtZsFFrameRaw[4].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 5 + 27 + (9 * VADataLengthW32[5])    ] = VPtZsFFrameRaw[5].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 5 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 9] = VPtZsFFrameRaw[5].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 5 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 18] = VPtZsFFrameRaw[5].Zero2;
    ++ViDestW32;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 6 + 27 + (9 * VADataLengthW32[5])    ] = VPtZsFFrameRaw[6].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 6 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 9] = VPtZsFFrameRaw[6].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 6 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 18] = VPtZsFFrameRaw[6].Zero2;
    ++ViDestW32;

    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 7 + 27 + (9 * VADataLengthW32[5])    ] = VPtZsFFrameRaw[7].Trailer;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 7 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 9] = VPtZsFFrameRaw[7].Zero;
    ++ViDestW32;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 7 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 18] = VPtZsFFrameRaw[7].Zero2;
    ++ViDestW32;
    
    
    // PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 8 + 27 + (9 * VADataLengthW32[5])    ] = VPtTrigRec->ATrig[ViEChanTrigField];;
    
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 8 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32)    ] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 8 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 9] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 9) + 8 + 27 + (9 * MI26__ZS_FFRAME_RAW_MAX_W32) + 18] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    
    
    // Update ZsFrameRaw fields
    
    ++VPtZsFFrameRaw[0].FrameCnt;
    ++VPtZsFFrameRaw[1].FrameCnt;
    ++VPtZsFFrameRaw[2].FrameCnt;
    ++VPtZsFFrameRaw[3].FrameCnt;
    ++VPtZsFFrameRaw[4].FrameCnt;
    ++VPtZsFFrameRaw[5].FrameCnt;
    ++VPtZsFFrameRaw[6].FrameCnt;
    ++VPtZsFFrameRaw[7].FrameCnt;
    
  } // End for ViFrame
  
  
  *PtFirstFrameFC += VPtZsFFrameRaw[0].FrameCnt;
  
  
  // Free ZsFrameRaw
  
  free ( VPtZsFFrameRaw );
  
  err_retok (( ERR_OUT, "MsgOk" ));
  
  return (0);
}



// $$$$$$$$$$


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__ULT1_FFRioEmulDeserData6Ult1EudetMode2 ( UInt32* PtDestW32,
:  SInt32 EltNb, UInt32* PtFirstFrameFC, SInt32 FrameNb, SInt16 EmuleMode  )
:
Goal      : Emulate one acquisition for one Mi26 in data transfer mode EUDET 2
:
: In this case there is an extra channel for TLU & Flex RIO trigger triggers.
:
: For each trigger two informations are stored in the following order
: - TLU trigger                    -> see record EFRIO__TTluTrigger
: - Flex RIO trigger / time stamp  -> see record EFRIO__TFlexRioTimeStamp1
:
: The EmuleMode parameter sets triggers nb.
: The first three triggers + the last one are configured from GUI.
: Their values are stored in fields ParATrig, ParATS of AcqEmul part
: ( type EFRIO__TAcqEmul ) of lib context record.
: The other triggers are hard coded to 0.
:
: In this case the three triggers stored in MI26 Zero fields are set to 0.
: But he high 16 bits ( B31B16 ) of field Zero1 is still used to store the
: number of triggers.
:
:
Inputs    : PtDestW32        - Pointer to Flex RIO DRAM to overwrite
: EltNb            - Flex RIO DRAM size in W32 unit ( 1 Elt = 1 W32 )
:
: PtFirstFrameFC   - Pointer to a variable which contains a global frame counter
:                     Used to set frame counter of each frame of current acq
:                     Incremented by the number of frames of current acq at end of function
:
: FrameNb          - The number of frame to emulate
:
: EmuleMode        >= 0 -> No trigger emulation
:                  <  0 -> Emulates | EmuleMode | on each frame
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Level     : This is a user level function.
Date      : 20/05/2011
Doc date  : 
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__ULT1_FFRioEmulDeserData6Ult1EudetMode2 ( UInt32* PtDestW32, SInt32 EltNb, UInt32* PtFirstFrameFC, SInt32 FrameNb, SInt16 EmuleMode ) {
  
  EFRIO__TAcqEmul*     VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
  ULT1__TZsFFrameRaw*  VPtZsFFrameRaw;
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
  
  ULT1__TStatesLine          VStateLine;
  ULT1__TState               VState;
  SInt16                     ViMi26Line;
  SInt16                     ViDataW16;
  
  
  
  err_trace (( ERR_OUT, "EFRIO__MI26_FFRioEmulDeserData6Mi26EudetMode2 (EltNb=%d, FrameNb=%d, ... )", EltNb, FrameNb ));
  
  // Pointers parameters check
  
  err_retnull ( PtDestW32, (ERR_OUT,"PtDestW32 = NULL") );
  err_retnull ( PtFirstFrameFC, (ERR_OUT,"PtFirstFrameFC = NULL") );
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / ULT1__ZS_FFRAME_MODE_2X160MHZ_W16_SZ ) / 7; // Divide by 7 because of extral channel
  
  // It must be => ( (EltNb * 2) / (ULT1__ZS_FFRAME_MODE_2X160MHZ_W16_SZ * 2) );
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
  
  VPtZsFFrameRaw = (ULT1__TZsFFrameRaw*) malloc ( 6 * sizeof (ULT1__TZsFFrameRaw) );
  
  err_retnull ( VPtZsFFrameRaw, (ERR_OUT,"Alloc ZsFFrameRaw failed !") );
  
  // Alloc trigger record
  
  VPtTrigRec = (EFRIO__TTriggerRec*) malloc ( sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );
  
  err_retnull ( VPtTrigRec, (ERR_OUT,"Allocate tmp trigger buff failed !" ) );
  
  
  // Reset ZsFrameRaw
  
  memset ( VPtZsFFrameRaw, 0, 6 * sizeof (ULT1__TZsFFrameRaw) );
  
  // Init ZsFrameRaw
  
  // RQ : Emulate trigger only on first Mi26 because it is used for trigger extraction
  // by readout function, information stored in Mi26 [2..5] are ignored
  
  VPtZsFFrameRaw[0].Header     = VPtAcqEmul->ParAHeader[0];
  VPtZsFFrameRaw[0].FrameCnt   = *PtFirstFrameFC;
  
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
  VPtZsFFrameRaw[1].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[1].DataLength = 0x00200020;
  VPtZsFFrameRaw[1].Trailer    = VPtAcqEmul->ParATrailer[1];
  VPtZsFFrameRaw[1].Zero       = 0x00000000;
  VPtZsFFrameRaw[1].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[2].Header     = VPtAcqEmul->ParAHeader[2];
  VPtZsFFrameRaw[2].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[2].DataLength = 0x00300030;
  VPtZsFFrameRaw[2].Trailer    = VPtAcqEmul->ParATrailer[2];
  VPtZsFFrameRaw[2].Zero       = 0x00000000;
  VPtZsFFrameRaw[2].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[3].Header     = VPtAcqEmul->ParAHeader[3];
  VPtZsFFrameRaw[3].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[3].DataLength = 0x00400040;
  VPtZsFFrameRaw[3].Trailer    = VPtAcqEmul->ParATrailer[3];
  VPtZsFFrameRaw[3].Zero       = 0x00000000;
  VPtZsFFrameRaw[3].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[4].Header     = VPtAcqEmul->ParAHeader[4];
  VPtZsFFrameRaw[4].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[4].DataLength = 0x00500050;
  VPtZsFFrameRaw[4].Trailer    = VPtAcqEmul->ParATrailer[4];
  VPtZsFFrameRaw[4].Zero       = 0x00000000;
  VPtZsFFrameRaw[4].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[5].Header     = VPtAcqEmul->ParAHeader[5];
  VPtZsFFrameRaw[5].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[5].DataLength = 0x00600060;
  VPtZsFFrameRaw[5].Trailer    = VPtAcqEmul->ParATrailer[5];
  VPtZsFFrameRaw[5].Zero       = 0x00000000;
  VPtZsFFrameRaw[5].Zero2      = 0x00000000;
  
  // 20/05/2011
  // Emulate data
  // Set hits on column No corresponding to Ult1 index from lines 0 to 927
  // - Column 0 for Mimosa 26 No 0
  // - etc ...
  // - Column 5 for Mimosa 26 No 5
  
  
  ViDataW16 = 0;
  
  for ( ViMi26Line=0; ViMi26Line < 928; ViMi26Line++ ) {
    
    VStateLine.F.StateNb  = 1;
    VStateLine.F.LineAddr = ViMi26Line;
    VStateLine.F.Ovf      = 0;
    
    // Write StateLine info to all Mi 26
    
    VPtZsFFrameRaw[0].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[1].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[2].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[3].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[4].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[5].ADataW16[ViDataW16] = VStateLine.W16;
    
    ++ViDataW16;
    
    VState.F.HitNb        = 0; // 0 => 1 hit !!!
    VState.F.NotUsed      = 0;
    
    // Write State info to all Mi 26 BUT set ColAddr = Mi26 No + 1
    
    VState.F.ColAddr = 0;
    VPtZsFFrameRaw[0].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 1;
    VPtZsFFrameRaw[1].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 2;
    VPtZsFFrameRaw[2].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 3;
    VPtZsFFrameRaw[3].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 4;
    VPtZsFFrameRaw[4].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 5;
    VPtZsFFrameRaw[5].ADataW16[ViDataW16] = VState.W16;
    
    ++ViDataW16;
    
  }
  
  
  
  
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
      VAEmulDataSzW16[0] = random (1851);
      VAEmulDataSzW16[1] = random (1851);
      VAEmulDataSzW16[2] = random (1851);
      VAEmulDataSzW16[3] = random (1851);
      VAEmulDataSzW16[4] = random (1851);
      VAEmulDataSzW16[5] = random (1851);
      
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
    
    
    for ( ViDataW32=0; ViDataW32 < ULT1__ZS_FFRAME_RAW_MAX_W32; ViDataW32++ ) {
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
    
    
    
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 21 + (7 * VADataLengthW32[0])    ] = VPtZsFFrameRaw[0].Trailer;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) +  7] = VPtZsFFrameRaw[0].Zero;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[0].Zero2;
    ++ViDestW32;
    
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 1 + 21 + (7 * VADataLengthW32[1])    ] = VPtZsFFrameRaw[1].Trailer;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 1 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtZsFFrameRaw[1].Zero;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 1 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[1].Zero2;
    ++ViDestW32;
    
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 2 + 21 + (7 * VADataLengthW32[2])    ] = VPtZsFFrameRaw[2].Trailer;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 2 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtZsFFrameRaw[2].Zero;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 2 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[2].Zero2;
    ++ViDestW32;
    
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 3 + 21 + (7 * VADataLengthW32[3])    ] = VPtZsFFrameRaw[3].Trailer;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 3 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtZsFFrameRaw[3].Zero;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 3 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[3].Zero2;
    ++ViDestW32;
    
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 4 + 21 + (7 * VADataLengthW32[4])    ] = VPtZsFFrameRaw[4].Trailer;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 4 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtZsFFrameRaw[4].Zero;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 4 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[4].Zero2;
    ++ViDestW32;
    
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 5 + 21 + (7 * VADataLengthW32[5])    ] = VPtZsFFrameRaw[5].Trailer;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 5 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtZsFFrameRaw[5].Zero;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 5 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[5].Zero2;
    ++ViDestW32;
    
    // PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 6 + 21 + (7 * VADataLengthW32[5])    ] = VPtTrigRec->ATrig[ViEChanTrigField];;
    
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 6 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32)    ] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 6 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 6 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtTrigRec->ATrig[ViEChanTrigField];
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
  
  
  *PtFirstFrameFC += VPtZsFFrameRaw[0].FrameCnt;
  
  
  // Free ZsFrameRaw
  
  free ( VPtZsFFrameRaw );
  
  err_retok (( ERR_OUT, "MsgOk" ));
  
  return (0);
}

// $$$$$$$



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__ULT1_FFRioEmulDeserData6Ult1EudetMode3 ( UInt32* PtDestW32,
:  SInt32 EltNb, UInt32* PtFirstFrameFC, SInt32 FrameNb, SInt16 EmuleMode  )
:
Goal      : Emulate one acquisition for one Mi26 in data transfer mode EUDET 3
:
: In this case there is an extra channel for TLU & Flex RIO trigger triggers.
:
: For each trigger two informations are stored in the following order
: - TLU trigger                    -> see record EFRIO__TTluTrigger
: - Flex RIO trigger / time stamp  -> see record EFRIO__TFlexRioTimeStamp1
:
: The EmuleMode parameter sets triggers nb.
: The first three triggers + the last one are configured from GUI.
: Their values are stored in fields ParATrig, ParATS of AcqEmul part
: ( type EFRIO__TAcqEmul ) of lib context record.
: The other triggers are hard coded to 0.
:
:***************************************************************************
: For this data transfer mode - EUDET 3 - The triggers are not emulated on
: each frame but on M consecutive frames each N frames. M & N are defined
: from GUI, they are stored in fields ParTrigOnNConsecutiveFrames = M and
: ParTrigOnOneFrameOverN = N of AcqEmul part ( type EFRIO__TAcqEmul ) of lib
: context record.
:***************************************************************************
:
: In this case the three triggers stored in MI26 Zero fields are set to 0.
: But he high 16 bits ( B31B16 ) of field Zero1 is still used to store the
: number of triggers.
:
:
Inputs    : PtDestW32        - Pointer to Flex RIO DRAM to overwrite
: EltNb            - Flex RIO DRAM size in W32 unit ( 1 Elt = 1 W32 )
:
: PtFirstFrameFC   - Pointer to a variable which contains a global frame counter
:                     Used to set frame counter of each frame of current acq
:                     Incremented by the number of frames of current acq at end of function
:
: FrameNb          - The number of frame to emulate
:
: EmuleMode        >= 0 -> No trigger emulation
:                  <  0 -> Emulates | EmuleMode | on each frame
:
Ouputs    : The function returns
:  0 if ok
: -1 if an error occurs
:
Globals   :
:
Level     : This is a user level function.
Date      : 03/11/2010
Doc date  : 07/11/2010
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__ULT1_FFRioEmulDeserData6Ult1EudetMode3 ( UInt32* PtDestW32, SInt32 EltNb, UInt32* PtFirstFrameFC, SInt32 FrameNb, SInt16 EmuleMode ) {
  
  EFRIO__TAcqEmul*           VPtAcqEmul = &EFRIO__VGContext.AcqEmul;
  ULT1__TZsFFrameRaw*        VPtZsFFrameRaw;
  SInt32                     VFrameNbFromBoardDrv;
  SInt32                     ViFrame;
  UInt32                     VADataLengthField[6];
  UInt32                     VADataLengthW16[6];
  UInt32                     VADataLengthW32[6];
  SInt32                     ViDestW32;
  SInt32                     ViDataW32;
  UInt32*                    VPtDataW32Chip0;
  UInt32*                    VPtDataW32Chip1;
  UInt32*                    VPtDataW32Chip2;
  UInt32*                    VPtDataW32Chip3;
  UInt32*                    VPtDataW32Chip4;
  UInt32*                    VPtDataW32Chip5;
  EFRIO__TTriggerRec*        VPtTrigRec;
  SInt32                     ViEChanTrigField;
  SInt32                     VTrigNbToEmulate;
  SInt32                     VAEmulDataSzW16[6];
  SInt32                     ViTrigToEmulate;
  EFRIO__TTluTrigger*        VPtEmulTrig;
  EFRIO__TFlexRioTimeStamp1* VPtEmulTs;
  
  ULT1__TStatesLine          VStateLine;
  ULT1__TState               VState;
  SInt16                     ViMi26Line;
  SInt16                     ViDataW16;
  
  
  
  err_trace (( ERR_OUT, "EFRIO__ULT1_FFRioEmulDeserData6Ult1EudetMode3 (EltNb=%d, FrameNb=%d, ... )", EltNb, FrameNb ));
  
  // Pointers parameters check
  
  err_retnull ( PtDestW32, (ERR_OUT,"PtDestW32 = NULL") );
  err_retnull ( PtFirstFrameFC, (ERR_OUT,"PtFirstFrameFC = NULL") );
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / ULT1__ZS_FFRAME_MODE_2X160MHZ_W16_SZ ) / 7; // Divide by 7 because of extral channel
  
  // It must be => ( (EltNb * 2) / (ULT1__ZS_FFRAME_MODE_2X160MHZ_W16_SZ * 2) );
  // But result is the SAME if we divide both part of fraction by 2 ;-)
  
  
  if ( VFrameNbFromBoardDrv != FrameNb ) {
    err_retfail ( -1, (ERR_OUT,"Dest buffer too small : %d frames buffer <> %d frames requested", VFrameNbFromBoardDrv, FrameNb ) );
  }
  
  // Check ParTrigOnOneFrameOverN --> Will create div by 0 error if NULL
  
  if ( VPtAcqEmul->ParTrigOnOneFrameOverN <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort => ParTrigOnOneFrameOverN=%d <= 0 !", VPtAcqEmul->ParTrigOnOneFrameOverN) );
  }
  
  // Calculate nb of trigger to emulate
  
  if ( EmuleMode >= 0 ) {
    VTrigNbToEmulate = 0;
  }
  
  else {
    VTrigNbToEmulate = abs ( EmuleMode );
  }
  
  
  // Alloc ZsFrameRaw
  
  VPtZsFFrameRaw = (ULT1__TZsFFrameRaw*) malloc ( 6 * sizeof (ULT1__TZsFFrameRaw) );
  
  err_retnull ( VPtZsFFrameRaw, (ERR_OUT,"Alloc ZsFFrameRaw failed !") );
  
  // Alloc trigger record
  
  VPtTrigRec = (EFRIO__TTriggerRec*) malloc ( sizeof (EFRIO__TTriggerRec) + (EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ) );
  
  err_retnull ( VPtTrigRec, (ERR_OUT,"Allocate tmp trigger buff failed !" ) );
  
  
  // Reset ZsFrameRaw
  
  memset ( VPtZsFFrameRaw, 0, 6 * sizeof (ULT1__TZsFFrameRaw) );
  
  // Init ZsFrameRaw
  
  // RQ : Emulate trigger only on first Mi26 because it is used for trigger extraction
  // by readout function, information stored in Mi26 [2..5] are ignored
  
  VPtZsFFrameRaw[0].Header     = VPtAcqEmul->ParAHeader[0];
  VPtZsFFrameRaw[0].FrameCnt   = *PtFirstFrameFC;
  
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
  VPtZsFFrameRaw[1].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[1].DataLength = 0x00200020;
  VPtZsFFrameRaw[1].Trailer    = VPtAcqEmul->ParATrailer[1];
  VPtZsFFrameRaw[1].Zero       = 0x00000000;
  VPtZsFFrameRaw[1].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[2].Header     = VPtAcqEmul->ParAHeader[2];
  VPtZsFFrameRaw[2].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[2].DataLength = 0x00300030;
  VPtZsFFrameRaw[2].Trailer    = VPtAcqEmul->ParATrailer[2];
  VPtZsFFrameRaw[2].Zero       = 0x00000000;
  VPtZsFFrameRaw[2].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[3].Header     = VPtAcqEmul->ParAHeader[3];
  VPtZsFFrameRaw[3].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[3].DataLength = 0x00400040;
  VPtZsFFrameRaw[3].Trailer    = VPtAcqEmul->ParATrailer[3];
  VPtZsFFrameRaw[3].Zero       = 0x00000000;
  VPtZsFFrameRaw[3].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[4].Header     = VPtAcqEmul->ParAHeader[4];
  VPtZsFFrameRaw[4].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[4].DataLength = 0x00500050;
  VPtZsFFrameRaw[4].Trailer    = VPtAcqEmul->ParATrailer[4];
  VPtZsFFrameRaw[4].Zero       = 0x00000000;
  VPtZsFFrameRaw[4].Zero2      = 0x00000000;
  
  VPtZsFFrameRaw[5].Header     = VPtAcqEmul->ParAHeader[5];
  VPtZsFFrameRaw[5].FrameCnt   = *PtFirstFrameFC;
  VPtZsFFrameRaw[5].DataLength = 0x00600060;
  VPtZsFFrameRaw[5].Trailer    = VPtAcqEmul->ParATrailer[5];
  VPtZsFFrameRaw[5].Zero       = 0x00000000;
  VPtZsFFrameRaw[5].Zero2      = 0x00000000;
  
  
  // 30/12/2010
  // Emulate data
  // Set hits on column No corresponding to Ultimate index from lines 0 to 927
  // - Column 0 for Mimosa 26 No 0
  // - etc ...
  // - Column 5 for Mimosa 26 No 5
  
  ViDataW16 = 0;
  
  for ( ViMi26Line=0; ViMi26Line < 928; ViMi26Line++ ) {
    
    VStateLine.F.StateNb  = 1;
    VStateLine.F.LineAddr = ViMi26Line;
    VStateLine.F.Ovf      = 0;
    
    // Write StateLine info to all Mi 26
    
    VPtZsFFrameRaw[0].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[1].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[2].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[3].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[4].ADataW16[ViDataW16] = VStateLine.W16;
    VPtZsFFrameRaw[5].ADataW16[ViDataW16] = VStateLine.W16;
    
    ++ViDataW16;
    
    VState.F.HitNb        = 0; // 0 => 1 hit !!!
    VState.F.NotUsed      = 0;
    
    // Write State info to all Mi 26 BUT set ColAddr = Mi26 No + 1
    
    VState.F.ColAddr = 0;
    VPtZsFFrameRaw[0].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 1;
    VPtZsFFrameRaw[1].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 2;
    VPtZsFFrameRaw[2].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 3;
    VPtZsFFrameRaw[3].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 4;
    VPtZsFFrameRaw[4].ADataW16[ViDataW16] = VState.W16;
    VState.F.ColAddr = 5;
    VPtZsFFrameRaw[5].ADataW16[ViDataW16] = VState.W16;
    
    ++ViDataW16;
    
  }
   
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
      VAEmulDataSzW16[0] = random (1851);
      VAEmulDataSzW16[1] = random (1851);
      VAEmulDataSzW16[2] = random (1851);
      VAEmulDataSzW16[3] = random (1851);
      VAEmulDataSzW16[4] = random (1851);
      VAEmulDataSzW16[5] = random (1851);
      
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
    
    
    for ( ViDataW32=0; ViDataW32 < ULT1__ZS_FFRAME_RAW_MAX_W32; ViDataW32++ ) {
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
    
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 21 + (7 * VADataLengthW32[0])    ] = VPtZsFFrameRaw[0].Trailer;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) +  7] = VPtZsFFrameRaw[0].Zero;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[0].Zero2;
    ++ViDestW32;
    
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 1 + 21 + (7 * VADataLengthW32[1])    ] = VPtZsFFrameRaw[1].Trailer;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 1 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtZsFFrameRaw[1].Zero;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 1 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[1].Zero2;
    ++ViDestW32;
    
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 2 + 21 + (7 * VADataLengthW32[2])    ] = VPtZsFFrameRaw[2].Trailer;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 2 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtZsFFrameRaw[2].Zero;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 2 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[2].Zero2;
    ++ViDestW32;
    
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 3 + 21 + (7 * VADataLengthW32[3])    ] = VPtZsFFrameRaw[3].Trailer;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 3 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtZsFFrameRaw[3].Zero;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 3 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[3].Zero2;
    ++ViDestW32;
    
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 4 + 21 + (7 * VADataLengthW32[4])    ] = VPtZsFFrameRaw[4].Trailer;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 4 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtZsFFrameRaw[4].Zero;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 4 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[4].Zero2;
    ++ViDestW32;
    
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 5 + 21 + (7 * VADataLengthW32[5])    ] = VPtZsFFrameRaw[5].Trailer;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 5 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtZsFFrameRaw[5].Zero;
    ++ViDestW32;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 5 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtZsFFrameRaw[5].Zero2;
    ++ViDestW32;
    
    // PtDestW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 6 + 21 + (7 * VADataLengthW32[5])    ] = VPtTrigRec->ATrig[ViEChanTrigField];;
    
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 6 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32)    ] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 6 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 7] = VPtTrigRec->ATrig[ViEChanTrigField];
    ++ViDestW32;
    ++ViEChanTrigField;
    PtDestW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * ViFrame * 7) + 6 + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 14] = VPtTrigRec->ATrig[ViEChanTrigField];
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
  
  
  *PtFirstFrameFC += VPtZsFFrameRaw[0].FrameCnt;
  
  
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










#endif
