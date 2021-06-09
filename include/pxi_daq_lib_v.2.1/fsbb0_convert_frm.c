

/*******************************************************************************
Prototype : SInt8 SUZE02_FHammingDecoder8_4(UInt8 CodedIn)

Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    : PrintLvl = 5 => Print states 
Level     :
Date      : 10/07/2013
Doc date  : 
Rev       : 
 Author    : Matthieu SPECHT
E-mail    : matthieu.specht@iphc.cnrs.fr
Labo      : IPHC */
/******************************************************************************/

 UInt8 FSBB0_FHammingDecoder8_4(UInt8 CodedIn)
 {
	UInt8 	VResult;
	
	// 27/06/2014 - MS replaced the hamming code ( which doesn't seem to be working for FSBB) by the converting the code received according to the mail from GD
	//                 have to investigate when there will be some spare time........
	
		  switch (CodedIn){
		case 0:{
			VResult = 0;
		break;
		}
		case 0x87:{
			VResult = 1;
		break;
		}
		case 0x99:{
			VResult = 2;
		break;
		}
		case 0x1e:{
			VResult = 3;
		break;
		}
		case 0xaa:{
			VResult = 4;
		break;
		}
		case 0x2d:{
			VResult = 5;
		break;
		}
		case 0x33:{
			VResult = 6;
		break;
		}
		case 0xb4:{
			VResult = 7;
		break;
		}
		case 0x4b:{
			VResult = 8;
		break;
		}
		case 0xcc:{
			VResult = 9;
		break;
		}
		case 0xd2:{
			VResult = 0xa;
		break;
		}
		case 0x55:{
			VResult = 0xb;
		break;
		}
		case 0xe1:{
			VResult = 0xc;
		break;
		}
		case 0x66:{
			VResult = 0xd;
		break;
		}
		case 0x78:{
			VResult = 0xe;
		break;
		}
		case 0xff:{
			VResult = 0xf;
		break;
		}
	  
	  }/* end switch G0 */
	
	
	
/* 	UInt8 	VSyndrome;
 	UInt8   VParity;
	UInt8   VTempResult;
    UInt8   VError;
	
	VSyndrome = ((((CodedIn & 0x8)>>3)^((CodedIn & 0x10)>>4)^((CodedIn & 0x20)>>5)^((CodedIn & 0x40)>>6))<<2 +
				(((CodedIn & 0x2)>>1)^((CodedIn & 0x4)>>2)^((CodedIn & 0x20)>>5)^((CodedIn & 0x40)>>6))<<1  +
				((CodedIn & 0x1)^((CodedIn & 0x4)>>2)^((CodedIn & 0x10)>>4)^((CodedIn & 0x40)>>6)));
 	// Parity bit

 	VParity       = (CodedIn & 0x1)^((CodedIn & 0x2)>>1)^((CodedIn & 0x4)>>2)^((CodedIn & 0x8)>>3)^((CodedIn & 0x10)>>4)^((CodedIn & 0x20)>>5)^((CodedIn & 0x40)>>6);
	VTempResult = CodedIn;
   switch (VSyndrome )
    {
			case 0 : {
					// No Trans Error 
					// Nothing to do
				break;
				}
			case 1 :{
					// Error on bit 0
					VTempResult ^= 0x1;
				break;
				}
			case 2 : {
					// Error on bit 1
					VTempResult ^= 0x2;
				break;
				}
			case 3 : {
					// Error on bit 2
					VTempResult ^= 0x4;
				break;
				}
			case 4 : {
					// Error on bit 3
					VTempResult ^= 0x8;
				break;
				}
			case 5 : {
					// Error on bit 4
					VTempResult ^= 0x10;
				break;
				}
			case 6 : {
					// Error on bit 5
					VTempResult ^= 0x20;
				break;
				}
			default      : {
					// Error on bit 6
					VTempResult ^= 0x40;
				break;
				}
	}*/  /*  end switch  */
    // ------------------------------------------------------------- //
/*  if (VSyndrome == 0){
		VError = 0;
	}
	else{
		if (VParity == ((CodedIn & 0x80)>>7)){
			VError = 5;
			}
		else{
			VError = 3;
		}
	}
	VResult = ((VTempResult & 2)>>2) + ((VTempResult & 16)>>3) + ((VTempResult & 32)>>3) + ((VTempResult & 64)>>3);
  err_trace (( ERR_OUT, "FSBB0_FHammingDecoder8_4, result :%X, Error code : %X",VResult,VError  ));*/
	
	return (VResult);

} /* End FSBB0_FHammingDecoder8_4    */







/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    : PrintLvl = 5 => Print states 
Level     :
Date      : 17/03/2009
Doc date  : 17/03/2009
Rev       : 04/05/2009
          : - Add a check of data length, if > 1140, force 1140, done to avoid bad
          :   data length due to transmission errors.
          :
          : 05/05/2009
          : - Add handling of odd number of W16 ( FSBB0 add one bad W16 in this case )
          :   Not done at beginning because FSBB0 documentation was not clear enough
          :   ( bad translation of " impair " in even ! ) and it was not needed for 
          :   tests done with emulated patterns.
          :          :
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FSBB0__FConvZsFFrameRawToZsFFrame ( FSBB0__TZsFFrameRaw* Src, FSBB0__TZsFFrame* Dest, SInt8 PrintLvl ) {
  
  SInt32 VRetW30Length;
  SInt32 ViData;
  UInt16 VDataW30Length;
  UInt16 VLastW30;
  SInt16 ViSrcW30;
  
  FSBB0__TStatesLine VStatesLine;
  SInt16  ViStatesLine;
  SInt8   ViState;
  SInt8   VStatesNbG0;
  SInt8   VStatesNbG1;
  SInt16  VPrevLine;
  SInt8   VG0Overflow;
  SInt8   VG1Overflow;
  
  
  err_retnull ( Src , (ERR_OUT,"Src == NULL") );
  err_retnull ( Dest, (ERR_OUT,"Dest == NULL") );
  
  // Get useful data length
  VDataW30Length     = Src->UsefulDataLengthW30;

  VRetW30Length = VDataW30Length;
  
  if ( (PrintLvl == 5) || (PrintLvl == 6) || (PrintLvl == 7)/*||(PrintLvl == 8)*/ ) {
    msg (( MSG_OUT, "TOTAL frame length : %d W30", VDataW30Length ));
    msg (( MSG_OUT, "" ));
  }
  
  // Add a check of data length, if > FSBB0__ZS_FFRAME_RAW_MAX_W30, force 0 and continue processing ( no exit on error )
  
  if ( VDataW30Length > FSBB0__ZS_FFRAME_RAW_MAX_W32 /* 1140 */ ) {
    err_error (( ERR_OUT, "Bad data length get from FSBB0 = %d W30 => Force 0 W30", VDataW30Length ));
    VDataW30Length = 0;
    Src->UsefulDataLengthW30 = 0;
     VRetW30Length = -1;
  }
  
  // Copy information fields
  
  Dest->SStatus        = Src->SStatus;
  
  Dest->Header         = Src->Header;
  Dest->FrameCnt       = Src->DataLengthRemFrCnt.F.FrameCnt;
  Dest->DataLength     = Src->UsefulDataLengthW30;
  Dest->Trailer        = Src->Trailer;
//#ifndef FSBB0__APP_IGNORE_GC_MOD_220514
  Dest->TrigSignalLine = Src->SStatus.ATrigRes[ASIC__FSBB0_TRIG_RES__SIG_LINE];
  Dest->TrigSignalClk  = Src->SStatus.ATrigRes[ASIC__FSBB0_TRIG_RES__SIG_CLK];
  Dest->TrigLine       = Src->SStatus.ATrigRes[ASIC__FSBB0_TRIG_RES__LINE];
//#endif
  
  // Process frame
  
  ViSrcW30      = 0;
  ViStatesLine  = 0;
  VPrevLine     = -1;
  
  if ( PrintLvl == 4 ) {
    msg (( MSG_OUT, "Frame data length = %d [W30]", VDataW30Length ));
    msg (( MSG_OUT, "" ));
  }
  
//#ifndef FSBB0__MI26_MI28_CODE_MUST_BE_UPDATED
    
  
  if ( VDataW30Length != 0 ) {
    
    // -------------------------------------------------------------------------------------------------
    // Odd W30 nb handling !
    //
    // It can seem strange that this can be done by processing one W30 less than total data length in all
    // cases, this is due to data processing method used in loop, read explanation below if needed.
    // -------------------------------------------------------------------------------------------------
    // If the total W30 number is odd, FSBB0 add one more bad W30 to get an even W30 number.
    // This bad W30 will be seen as a StatesLine field followed by NO state because it is the last W30.
    // Therefore if at the beginning of the while loop there is only one W30 to process, this W30 is the
    // bad one, because it is a StateLines field followed by no states. In others words, if the index of
    // the W30 at the beginning of loop is the index of last W30 this W30 is the bad one which must be
    // rejected, we must not enter the loop. In normal case, even W30 number, after processing of last
    // state of last line the index of W30 equal W30 number, therefore is > of index of last W30, and
    // we don't enter the loop.
    
    //VLastW30 = VDataW30Length - 1;
    
    while ( ViSrcW30 < VDataW30Length ) { // Odd W30 nb handling => Don't process last W30
            
      // Copy StatesLine field
      VStatesLine.W32 = Src->ADataW32[ViSrcW30];
      Dest->AStatesRec[ViStatesLine].StatesLine = VStatesLine;
	  VG0Overflow = 0;
	  VG1Overflow = 0;

	  VStatesNbG0 = FSBB0_FHammingDecoder8_4(VStatesLine.F.HitNbG0);
	  VStatesNbG1 = FSBB0_FHammingDecoder8_4(VStatesLine.F.HitNbG1);
	  //msg (( MSG_OUT, " ViStatesLine :%d :VStatesLine.F.HitNbG0 : %X - VStatesNbG0 : %x  - VStatesLine.F.HitNbG1 : %X - VStatesNbG1 : %x  ", ViStatesLine,  VStatesLine.F.HitNbG0,VStatesNbG0,VStatesLine.F.HitNbG1,VStatesNbG1 ));

	  Dest->AStatesRec[ViStatesLine].NbWinG0 = VStatesNbG0;
	  Dest->AStatesRec[ViStatesLine].NbWinG1 = VStatesNbG1;
	  Dest->AStatesRec[ViStatesLine].NbWinTot = VStatesNbG0 + VStatesNbG1;

	  if (VStatesNbG0 > FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP ){
		VG0Overflow =VStatesNbG0;		
		VStatesNbG0 = FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP;
	  }
	  if (VStatesNbG1 > FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP ){
		VG1Overflow =VStatesNbG1;		
		VStatesNbG1 = FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP; 
	  }	  
     // VStatesNbPerLine = VStatesLine.F.StateNb;

      
      if ( (PrintLvl == 5) || (PrintLvl == 7) ) {
		msg (( MSG_OUT, " ViStatesLine :%d :VStatesLine.W32 : %X - NbWinG0 : %d  - NbWinG1 : %d  ", ViStatesLine,  VStatesLine.W32,Dest->AStatesRec[ViStatesLine].NbWinG0,Dest->AStatesRec[ViStatesLine].NbWinG1 ));
	  
	    //msg (( MSG_OUT, "NbWinG0 : %4d - NbWinG1 : %d  - NbWinTot : %d  ", Dest->AStatesRec[ViStatesLine].NbWinG0,  Dest->AStatesRec[ViStatesLine].NbWinG1,Dest->AStatesRec[ViStatesLine].NbWinTot ));
        
        msg (( MSG_OUT, "Line %4d - %d HitNbG0 (Ovf:%d) - %d HitNbG1 (Ovf:%d)", VStatesLine.F.SLineAddr, VStatesNbG0, VG0Overflow,VStatesNbG1,VG1Overflow ));
        
        /* Print to show missing handling of odd W30 nb
        
        if (  VStatesLine.F.LineAddr <= VPrevLine ) {
          msg (( MSG_OUT, "Line %4d - %d states - %d Ovf ===============> ERROR odd W30 nb NOT HANDLED ! ", VStatesLine.F.LineAddr, VStatesLine.F.StateNb, VStatesLine.F.Ovf ));
        }
        
        else {
          msg (( MSG_OUT, "Line %4d - %d states - %d Ovf ", VStatesLine.F.LineAddr, VStatesLine.F.StateNb, VStatesLine.F.Ovf ));
          VPrevLine = VStatesLine.F.LineAddr;
        }
        
        */
        
      }
      
      ++ViSrcW30;
	  if ( ViSrcW30 >= VDataW30Length ){
		break;
	  }
      
      // Copy states G1
      if (VStatesNbG1 > 0){
		  for ( ViState=0; ViState < VStatesNbG1; ViState++ ) {
				// 22/05/14 - MS GCMODIF

			Dest->AStatesRec[ViStatesLine].AStatesG1[ViState].W32 = Src->ADataW32[ViSrcW30];
			if ( (PrintLvl == 5) || (PrintLvl == 7) ) msg (( MSG_OUT, "-->G1: State %d : Delta : %d - Col %4d - %X pixels", ViState, Dest->AStatesRec[ViStatesLine].AStatesG1[ViState].F.Delta ,  Dest->AStatesRec[ViStatesLine].AStatesG1[ViState].F.ColAddr, Dest->AStatesRec[ViStatesLine].AStatesG1[ViState].F.Code ));
			if ( (PrintLvl == 8)&&(((Dest->AStatesRec[ViStatesLine].AStatesG1[ViState].F.Code)& 0xFFFFF) == 0)&&(ViSrcW30 < VDataW30Length) ) {
				msg (( MSG_OUT, " G1 : Frame :%d ,Word : %d of %d, SLineAddr :%d :  Window word : %X - Code = 0,  %d state out of %d  ",  Dest->FrameCnt, ViSrcW30,VDataW30Length, VStatesLine.F.SLineAddr,  Dest->AStatesRec[ViStatesLine].AStatesG1[ViState].W32,ViState ,VStatesNbG0 ));
			}			
			++ViSrcW30;
			  if ( ViSrcW30 >= VDataW30Length ){
				break;
			  }		  
		  }
	  }
      // Copy states G0
      
      if (VStatesNbG0 > 0){
		  for ( ViState=0; ViState < VStatesNbG0; ViState++ ) {
				// 22/05/14 - MS GCMODIF

			Dest->AStatesRec[ViStatesLine].AStatesG0[ViState].W32 = Src->ADataW32[ViSrcW30];
			if ( (PrintLvl == 5) || (PrintLvl == 7) ) msg (( MSG_OUT, "-->G0: State %d : Delta : %d - Col %4d - %X pixels", ViState, Dest->AStatesRec[ViStatesLine].AStatesG0[ViState].F.Delta , Dest->AStatesRec[ViStatesLine].AStatesG0[ViState].F.ColAddr, Dest->AStatesRec[ViStatesLine].AStatesG0[ViState].F.Code ));
			if ( (PrintLvl == 8)&&(((Dest->AStatesRec[ViStatesLine].AStatesG0[ViState].F.Code)& 0xFFFFF) == 0)&&(ViSrcW30 < VDataW30Length) ) {
				msg (( MSG_OUT, " G0 : Frame :%d ,Word : %d out of %d, SLineAddr :%d :  Window word : %X - Code = 0, %d state out of %d  ",  Dest->FrameCnt, ViSrcW30,VDataW30Length, VStatesLine.F.SLineAddr,  Dest->AStatesRec[ViStatesLine].AStatesG0[ViState].W32,ViState ,VStatesNbG0 ));
			}			
			++ViSrcW30;
			  if ( ViSrcW30 >= VDataW30Length ){
				break;
			  }		  
		  }
	  }
      
      ++ViStatesLine;
	  if ( ViSrcW30 >= VDataW30Length ){
		break;
	  }		  
      
      // Add on 25/03/2011 during test with random frame size + with all data = 0
      
      if ( ViStatesLine >= FSBB0__ZS_FFRAME_MAX_STATES_REC ) {
        err_warning (( ERR_OUT, "Max number of states reached = %d => Abort => Corrupted data !", FSBB0__ZS_FFRAME_MAX_STATES_REC ));
        break;
      }
      
    } // End while
    
  } // End if ( VDataW30Length != 0 )
   
  Dest->StatesRecNb = ViStatesLine;

//#endif // Endif of #ifndef FSBB0__MI26_MI28_CODE_MUST_BE_UPDATED
  
  if ( (PrintLvl == 5)/* || (PrintLvl == 6)*/|| (PrintLvl == 7) /*|| (PrintLvl == 8)*/ ) {
    msg (( MSG_OUT, "Conv : %d StatesLines", Dest->StatesRecNb ));
    msg (( MSG_OUT, "" ));
  }

  return (VRetW30Length);
}


/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    : 
Level     :
Date      : 14/08/2009
Doc date  : 
:
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FSBB0__FConvZsFFrameRawToZsFFrameHandleTrigger ( SInt8 MapsNb, SInt32 EvNo, SInt32 TotEvNb, FSBB0__TZsFFrameRaw* Src, FSBB0__TZsFFrame* Dest, SInt8 PrintLvl ) {

  FSBB0__TZsFFrameRaw* VPtSrcEv;
  SInt32 VTrigPosFromDaq;
  SInt32 VTrigLine;
  SInt8  VFirstEvent;
  static FSBB0__TZsFFrame VASrcZsFFrameEv[2];
  SInt32 VLine;
  SInt32 ViSrcStateRec;
  SInt32 ViDestStateRec;
  
  // Check parameters

  err_retnull ( Src , (ERR_OUT,"Src == NULL") );
  err_retnull ( Dest, (ERR_OUT,"Dest == NULL") );
  
  // Four events after this one are required => Abort if it's not the case
  
  if ( EvNo + 4 >= TotEvNb ) {
    err_retfail ( -1, (ERR_OUT,"Processing stop on event %d => Less than 4 events after it !" ,EvNo) );
  }
  

  // Trigger position ?
//#ifndef FSBB0__APP_IGNORE_GC_MOD_220514
  VTrigPosFromDaq = Src->SStatus.ATrigRes[ASIC__FSBB0_TRIG_RES__SIG_LINE];
//#endif


  if ( (VTrigPosFromDaq < 0) || (VTrigPosFromDaq > 575) ) {
    err_retfail ( -1, (ERR_OUT,"Bad trigger pos from DAQ = %d <> [0..575] !", VTrigPosFromDaq) );
  }
 

  // Set VPtEv on the first event to process
  // - Pos  < 572 => Pos correspond to current event + 1 ( next event )
  // - Pos >= 572 => Pos correspond to current event + 2

/* ORIGINAL => 1 plane

  if ( VTrigPosFromDaq < 572 ) {
    VPtSrcEv = Src + 1;
  }
  
  else {
    VPtSrcEv = Src + 2;
  }
  
*/

/* 6 planes

  if ( VTrigPosFromDaq < 572 ) {
    VPtSrcEv = Src + 7;
  }
  
  else {
    VPtSrcEv = Src + 8;
  }
  
*/

  // N planes

  if ( VTrigPosFromDaq < 572 ) {
    VPtSrcEv = Src + 1 + MapsNb;
  }
  
  else {
    VPtSrcEv = Src + 2 + MapsNb;
  }
  

  // Real trigger line = trigger position + 4 modulo 576
  
  VTrigLine = (VTrigPosFromDaq + 4) % 576;
  
  // Debug print
  
  if ( PrintLvl == 1 ) {
    // msg (( MSG_OUT, "=> Trig on Ev=%4d P=%4d L=%4d => Proc Ev=%4d - Ev=%4d", EvNo, VTrigPosFromDaq, VTrigLine, VPtSrcEv[0].SStatus.FrameNoInRun, VPtSrcEv[1].SStatus.FrameNoInRun ));
    err_error (( ERR_OUT, "=> Trig on Ev=%4d - P=%4d - L=%4d => Proc Ev=%4d - Ev=%4d", EvNo, VTrigPosFromDaq, VTrigLine, VPtSrcEv[0].SStatus.FrameNoInRun, VPtSrcEv[1].SStatus.FrameNoInRun ));
  }

  // Convert each event from ZsFFrameRaw to ZsFFrame

  FSBB0__FConvZsFFrameRawToZsFFrame ( VPtSrcEv    , VASrcZsFFrameEv    , 0 /* PrintLvl */ );
  FSBB0__FConvZsFFrameRawToZsFFrame ( VPtSrcEv + 1, VASrcZsFFrameEv + 1, 0 /* PrintLvl */ );
    
  // -------------------------  
  // Build destination event
  // -------------------------
  
  // Reset record 
  
  memset ( Dest, 0, sizeof (FSBB0__TZsFFrame) );
  
  // Copy information fields
  
  Dest->SStatus        = Src->SStatus;
  
  Dest->Header         = Src->Header;
  Dest->FrameCnt       = Src->DataLengthRemFrCnt.F.FrameCnt;
  Dest->DataLength     = Src->UsefulDataLengthW30;
  Dest->Trailer        = Src->Trailer;
  //Dest->Zero           = Src->Zero;
  //Dest->Zero2          = Src->Zero2;
//#ifndef FSBB0__APP_IGNORE_GC_MOD_220514
  Dest->TrigSignalLine = Src->SStatus.ATrigRes[ASIC__FSBB0_TRIG_RES__SIG_LINE];
  Dest->TrigSignalClk  = Src->SStatus.ATrigRes[ASIC__FSBB0_TRIG_RES__SIG_CLK];
  Dest->TrigLine       = Src->SStatus.ATrigRes[ASIC__FSBB0_TRIG_RES__LINE];
//#endif
  
  // Extract lines
  
  ViDestStateRec = 0;

  // First event => Copy lines >= trigger pos to dest
  
  for ( ViSrcStateRec=0; ViSrcStateRec < VASrcZsFFrameEv->StatesRecNb; ViSrcStateRec++ ) {
    
    VLine = VASrcZsFFrameEv->AStatesRec[ViSrcStateRec].StatesLine.F.SLineAddr;
    
    if ( VLine >= VTrigLine ) {
      Dest->AStatesRec[ViDestStateRec] = VASrcZsFFrameEv->AStatesRec[ViSrcStateRec];
      ++ViDestStateRec;
    }
    
  }
  
  if ( VTrigLine == 0 ) {
    Dest->StatesRecNb = ViDestStateRec;
    return (0);
  }

  // Second event => Copy lines < trigger pos to dest
  
  for ( ViSrcStateRec=0; ViSrcStateRec < VASrcZsFFrameEv[1].StatesRecNb; ViSrcStateRec++ ) {
    
    VLine = VASrcZsFFrameEv[1].AStatesRec[ViSrcStateRec].StatesLine.F.SLineAddr;
    
    if ( VLine < VTrigLine ) {
      Dest->AStatesRec[ViDestStateRec] = VASrcZsFFrameEv[1].AStatesRec[ViSrcStateRec];
      ++ViDestStateRec;
    }
    
  }
  
  // Update nb of StateRec
  
  Dest->StatesRecNb = ViDestStateRec;

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
Date      : 17/03/2009
Rev       : 22/07/2009
          : - Return number of hits found
Doc date  : 17/03/2009
Author    : Gilles CLAUS
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 FSBB0__FConvZsFFrameToMatDiscriBit ( FSBB0__TZsFFrame* PtSrc, FSBB0__TMatDiscriBit* PtDest, SInt32* PtOvfCnt, SInt8 PrintLvl ) {

  SInt16 ViLine;
  SInt16 ViFirstLine;
  SInt16 ViTopLine;
  SInt16 VFirstCol;
  SInt16 VLastCol;
  SInt16 ViCol;
  SInt16 ViStatesLine;
  SInt16  ViState;
  SInt16  VStatesNbG0;
  SInt16  VStatesNbG1;
  SInt32 VHitCnt;
  SInt32 VOfvCnt;
  SInt32 VMask;

  err_retnull ( PtSrc , (ERR_OUT,"PtSrc == NULL") );
  err_retnull ( PtDest, (ERR_OUT,"PtDest == NULL") );
  
  
  
  // --------------------------------------------
  // Reset destination matrix
  // --------------------------------------------
  
  memset ( PtDest, 0, sizeof (FSBB0__TMatDiscriBit) );
  
  // --------------------------------------------
  // Copy hits from ZsFFRame -> MatDiscriBit
  // --------------------------------------------

  // StatesRec loop

  VHitCnt = 0;
  VOfvCnt = 0;

  if ( PtSrc->StatesRecNb > FSBB0__ZS_FFRAME_MAX_STATES_REC ) {
    err_warning (( ERR_OUT, "StatesRecNb=%d > FSBB0__ZS_FFRAME_MAX_STATES_REC=%d => Force %d", PtSrc->StatesRecNb, FSBB0__ZS_FFRAME_MAX_STATES_REC, FSBB0__ZS_FFRAME_MAX_STATES_REC ));
    err_error   (( ERR_OUT, "StatesRecNb=%d > FSBB0__ZS_FFRAME_MAX_STATES_REC=%d => Force %d", PtSrc->StatesRecNb, FSBB0__ZS_FFRAME_MAX_STATES_REC, FSBB0__ZS_FFRAME_MAX_STATES_REC ));
    PtSrc->StatesRecNb = FSBB0__ZS_FFRAME_MAX_STATES_REC;
  }


//#ifndef FSBB0__MI26_MI28_CODE_MUST_BE_UPDATED
    if (PtSrc->StatesRecNb > 0){
  
	  for ( ViStatesLine=0; ViStatesLine < PtSrc->StatesRecNb; ViStatesLine++ ) {
		
		VStatesNbG0 = PtSrc->AStatesRec[ViStatesLine].NbWinG0;
		VStatesNbG1 = PtSrc->AStatesRec[ViStatesLine].NbWinG1;
		ViTopLine    = ((PtSrc->AStatesRec[ViStatesLine].StatesLine.F.SLineAddr + 1) * 4) - 1 ;

		//msg (( MSG_OUT, " ViStatesLine :%d :ViTopLine : %d - VStatesNbG0 : %d  -  VStatesNbG1 : %d  ", ViStatesLine,  ViTopLine,VStatesNbG0,VStatesNbG1 ));


		/*if (( PtSrc->AStatesRec[ViStatesLine].StatesLine.F.HitNbG0 > 9 )||( PtSrc->AStatesRec[ViStatesLine].StatesLine.F.HitNbG1 > 9 )) {
		  ++VOfvCnt;
		  // err_error (( ERR_OUT, "Ovf ! Count=%d", VOfvCnt ));
		}*/
		// States in one StateRec loop
		
		// WARNING => Will slow down execution
		
		if ( VStatesNbG0 > FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP ) {
		  err_warning (( ERR_OUT, "StatesNb=%d > FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_STATES_REC=%d => Force %d", VStatesNbG0, FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP, FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP ));
		  VStatesNbG0 = FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP;
		  ++VOfvCnt;
		}
		if ( VStatesNbG1 > FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP ) {
		  err_warning (( ERR_OUT, "StatesNb=%d > FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_STATES_REC=%d => Force %d", VStatesNbG1, FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP, FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP ));
		  VStatesNbG1 = FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP;
		  ++VOfvCnt;
		}

		// WARNING => Will slow down execution
		
		if ( ViTopLine >= FSBB0__MAT_DISCRI_LINES_NB) {
		  err_warning (( ERR_OUT, "ViTopLine=%d >= FSBB0__MAT_DISCRI_LINES_NB=%d => Force %d", ViTopLine, FSBB0__MAT_DISCRI_LINES_NB, FSBB0__MAT_DISCRI_LINES_NB - 1 ));
		  ViTopLine = FSBB0__MAT_DISCRI_LINES_NB - 1;
		}
		// Decode Hits for Group 1
		if ( VStatesNbG1 > 0 ){
			for ( ViState = 0; ViState < VStatesNbG1; ViState++ ) {
					
			  VFirstCol = (PtSrc->AStatesRec[ViStatesLine].AStatesG1[ViState].F.ColAddr) + 224;
			  // WARNING => Will slow down execution
			
			  if ( VFirstCol >= FSBB0__REG_DISCRI_BIT_SZ ) {
				err_warning (( ERR_OUT, "FirstCol=%d >= FSBB0__REG_DISCRI_BIT_SZ=%d => Force 0", VFirstCol, FSBB0__REG_DISCRI_BIT_SZ ));
				VFirstCol = 0;
			  }


			  // Hits in one State loop
			  ViFirstLine = ViTopLine - (PtSrc->AStatesRec[ViStatesLine].AStatesG1[ViState].F.Delta); 
			  VMask = 0x80000;
			  for (ViLine = ViFirstLine; ViLine > (ViFirstLine - 4); ViLine-- ){
				  if (ViLine < 0){
					break;
				  }
				  for ( ViCol = VFirstCol; ViCol > VFirstCol - 5; ViCol-- ) {
					if (ViCol < 0){
						break;
					}
					if ( ((PtSrc->AStatesRec[ViStatesLine].AStatesG1[ViState].F.Code)  & VMask) != 0){
						PtDest->AALineCol[ViLine][ViCol] = 1;
						++VHitCnt;
						// 01/08/2014 - MS : removed the printing of the decoding of the data words
						/*
						if (PrintLvl ==7){
							msg (( MSG_OUT, " G1 : ViStatesLine :%d :ViTopLine : %d : ViState:%d - Hit Line : %d  -  Col : %d , VMask :0x%X ", ViStatesLine,  ViTopLine,ViState, ViLine,ViCol,VMask ));
						}*/	
					}
					else{
						PtDest->AALineCol[ViLine][ViCol] = 0;
					
					}
					VMask = VMask >> 1;
					
				  } // End For ViCol
			  } // End For ViLine
			  
			} // End States in one StateRec loop for G1
		} /* end decoding groups for G1 */

		// Decode Hits for Group 0
		if ( VStatesNbG0 > 0 ){
			for ( ViState=0; ViState < VStatesNbG0; ViState++ ) {
					
			  VFirstCol = PtSrc->AStatesRec[ViStatesLine].AStatesG0[ViState].F.ColAddr ;
			  //VLastCol  = VFirstCol + PtSrc->AStatesRec[ViStatesLine].AStatesG1[ViState].F.HitNb; // HitNb = 0 => 1 hit, 1 => 2 hits etc ...
			  // WARNING => Will slow down execution
			
			  if ( VFirstCol >= FSBB0__REG_DISCRI_BIT_SZ ) {
				err_warning (( ERR_OUT, "FirstCol=%d >= FSBB0__REG_DISCRI_BIT_SZ=%d => Force 0", VFirstCol, FSBB0__REG_DISCRI_BIT_SZ ));
				VFirstCol = 0;
			  }

			  // Hits in one State loop
			  ViFirstLine = ViTopLine - (PtSrc->AStatesRec[ViStatesLine].AStatesG0[ViState].F.Delta); 
			  VMask = 0x80000;
			  for (ViLine = ViFirstLine; ViLine > ViFirstLine - 4; ViLine-- ){
				  if (ViLine < 0){
					break;
				  }
				  for ( ViCol=VFirstCol; ViCol > VFirstCol - 5; ViCol-- ) {
					if (ViCol < 0){
						break;
					}
					//msg (( MSG_OUT, " ViStatesLine :%d :ViLine : %d - ViCol : %d  -  PtSrc->AStatesRec[ViStatesLine].AStatesG0[ViState].F.Code : %d  ", ViStatesLine,  ViLine,ViCol,PtSrc->AStatesRec[ViStatesLine].AStatesG0[ViState].F.Code ));
					if ( (PtSrc->AStatesRec[ViStatesLine].AStatesG0[ViState].F.Code  & VMask) != 0){
						PtDest->AALineCol[ViLine][ViCol] = 1;
						++VHitCnt;
						// 01/08/2014 - MS : removed the printing of the decoding of the data words
						/*
						if (PrintLvl ==7){
							msg (( MSG_OUT, " G0 : ViStatesLine :%d :ViTopLine : %d : ViState:%d - Hit Line : %d  -  Col : %d , VMask :0x%x ", ViStatesLine,  ViTopLine,ViState, ViLine,ViCol,VMask ));
						}*/
					}
					VMask = VMask >> 1;
					
				  } // End For ViCol
			  } // End For ViLine
			  
			} // End States in one StateRec loop for G0 
		} /* end decoding groups for G0 */
	  } // End StatesRec loop
	}  /* end if (PtSrc->StatesRecNb > 0)*/

//#endif // Endif of #ifndef FSBB0__MI26_MI28_CODE_MUST_BE_UPDATED

  if ( PtOvfCnt != NULL ) {
    *PtOvfCnt = VOfvCnt;
  }


  // err_error (( ERR_OUT, "TRACE - VHitCnt=%d", VHitCnt )); // 13/02/2013
  

  return (VHitCnt);
}