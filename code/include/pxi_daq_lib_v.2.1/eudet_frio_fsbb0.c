
/*******************************************************************************
File      : x:\lib\win\eudet_frio\eudet_frio_fsbb0.c
Goal      : Fsbb0 functions of flex rio board library for EUDET
Prj date  : 01/10/2014
File date : 01/10/2014
Doc date  : 
Author    : Matthieu SPECHT
E-mail    : matthieu.specht@iphc.cnrs.fr
----------------------------------------------------------------------------------
License   : You are free to use this source files for your own development as long
          : as it stays in a public research context. You are not allowed to use it
          : for commercial purpose. You must put this header with laboratory and
          : authors names in all development based on this library.
----------------------------------------------------------------------------------
Labo      : IPHC */
/*******************************************************************************/


#ifndef EUDET_FRIO_FSBB0_C
#define EUDET_FRIO_FSBB0_C


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
Class     :
Prototype :
Goal      :
Inputs    :
Ouputs    :
Remark    :
Date      : 02/02/2009
Doc date  : //2004
Author    : Gilles CLAUS - Labo IPHC - DRS - gilles.claus@ires.in2p3.fr */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 EFRIO__FSBB0_FMatDiscriPrintHit ( char* CmtStrTitle, SInt8 CmtSInt8MapsId, FSBB0__TMatDiscriBit* PtDest ) {
  
  SInt32 ViLine;
  SInt32 ViCol;
  
  err_retnull ( PtDest, (ERR_OUT,"PtDest == NULL") );
  
  
  msg (( MSG_OUT, "***************************************************************************" ));
  msg (( MSG_OUT, "* FSBB M [%d] %s : Coordinates pixels with hit *", CmtSInt8MapsId, CmtStrTitle ));
  msg (( MSG_OUT, "***************************************************************************" ));
  
  // Scan matrix => Print coordinates if hit
  
  for ( ViLine=0; ViLine < FSBB0__MAT_DISCRI_USEFUL_LINES_NB; ViLine++ ) {
    
    for ( ViCol=0; ViCol < FSBB0__REG_DISCRI_BIT_SZ; ViCol++ ) {
      
      if ( PtDest->AALineCol[ViLine][ViCol] == 1 ) {
        msg (( MSG_OUT, "Hit => Line [%.4d] - Col [%.4d]", ViLine, ViCol ));
      }
      
    }
    
  }
  
  msg (( MSG_OUT, "" ));
  
  err_retok (( ERR_OUT, "" ));
}



/*******************************************************************************
Prototype : SInt8 SUZE02_FHammingDecoder8_4(UInt8 CodedIn)

Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    : 
Level     :
Date      : 10/07/2013
Doc date  : 
Rev       : 
 Author    : Matthieu SPECHT
E-mail    : matthieu.specht@iphc.cnrs.fr
Labo      : IPHC */
/******************************************************************************/

 UInt8 EFRIO__FSBB0_FHammingDecoder8_4(UInt8 CodedIn)
 {
	UInt8 	VResult;
	
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
	  
	  }/* end switch CodedIn */
	
	return (VResult);

} /* End FSBB0_FHammingDecoder8_4    */







/*******************************************************************************
Prototype :SInt32 EFRIO__FSBB0_FConvEfrioFrameToZsFFrame ( EFRIO__TFrame* Src, SInt8 MapsId, FSBB0__TZsFFrame* Dest, SInt8 PrintLvl )
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    : PrintLvl = 5 => Print states 
Level     :
Date      : 14/10/2014
Doc date  : 14/10/2014
Rev       : copy of APP__FConvEfrioFrameToZsFFrame from read_flex_rio_daq_file_v22_gc
          :
Author    : Matthieu SPECHT
E-mail    : matthieu.specht@iphc.cnrs.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 EFRIO__FSBB0_FConvEfrioFrameToZsFFrame ( EFRIO__TFrame* Src, SInt8 MapsId, FSBB0__TZsFFrame* Dest, SInt8 PrintLvl ) {
  
  SInt32  VRetW30Length;
  SInt32  ViData;
  UInt16  VDataW30Length;
  UInt16  VLastW30;
  SInt16  ViSrcW30;
  SInt32  VOneMapsSzW8;
  SInt32  VOneMapsSzW32;
  UInt32* VPtDataU32;
  
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

  VOneMapsSzW8    = Src->Data.OneMapsSz;
  VOneMapsSzW32   = VOneMapsSzW8 / 4;

  VDataW30Length  = Src->Header.AMapsDataLength[MapsId];
  VRetW30Length   = Src->Header.AMapsDataLength[MapsId];

  msg (( MSG_OUT, "Conv : VOneMapsSzW32=%d - TotSzW32=%d", VOneMapsSzW32, Src->Data.TotSz / 4 ));

  msg (( MSG_OUT, "Conv : VDataW30Length=%d", VDataW30Length ));


  if ( (PrintLvl == 5) || (PrintLvl == 6) || (PrintLvl == 7)/*||(PrintLvl == 8)*/ ) {
    msg (( MSG_OUT, "TOTAL frame length : %d W30", VDataW30Length ));
    msg (( MSG_OUT, "" ));
  }
  
  // Add a check of data length, if > FSBB0__ZS_FFRAME_RAW_MAX_W30, force 0 and continue processing ( no exit on error )
  
  if ( VDataW30Length > FSBB0__ZS_FFRAME_RAW_MAX_W32  ) {
    err_error (( ERR_OUT, "Bad data length get from FSBB0 = %d W30 => Force 0 W30", VDataW30Length ));
    VDataW30Length = 0;
    Src->Data.OneMapsSz = 0;
     VRetW30Length = -1;
  }
  
  // Copy information fields

  Dest->SStatus.AsicNo        = MapsId;        // Index of Asic <0..N-1> in case more than one is acquired
  Dest->SStatus.AcqNo         = Src->Header.AcqId;         // Index of current acquisition
  Dest->SStatus.FrameNoInAcq  = Src->Header.FrameIdInAcq;  // Index of frame in acquisition <0..AcqFrameNb-1>
  Dest->SStatus.FrameNoInRun  = -1;  // Index of frame in run <0..TotEventNb-1>
  
  Dest->SStatus.HitCnt        = 0;        // Counter of hits in frame
                                    // Used for monitoring, may be not set, therefore HitCnt = -1

  Dest->SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_CLK]  = Src->Header.AMapsTrigInfo[0];      // Trigger in [Fsbb bits]
  Dest->SStatus.ATrigRes[ASIC__MI26_TRIG_RES__LINE]     = Src->Header.AMapsTrigInfo[0] / 32; // Trigger in [Fsbb line]
  Dest->SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_LINE] = Src->Header.AMapsTrigInfo[0] % 32; // Trigger offset in Fsbb line in [Fsbb line]

  // -------------

  Dest->Header.F.H0    = Src->Header.AMapsHeader[MapsId] & 0x0000FFFF;
  Dest->Header.F.H1    = (Src->Header.AMapsHeader[MapsId] & 0xFFFF0000) >> 16;

  Dest->FrameCnt       = Src->Header.AMapsFrameCnt[MapsId];

  Dest->DataLength     = Src->Header.AMapsDataLength[MapsId];

  Dest->Trailer.F.T0   = Src->Header.AMapsTrailer[MapsId] & 0x0000FFFF;
  Dest->Trailer.F.T1   = (Src->Header.AMapsTrailer[MapsId] & 0xFFFF0000) >> 16;

  
  Dest->TrigSignalClk  = Dest->SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_CLK];
  Dest->TrigLine       = Dest->SStatus.ATrigRes[ASIC__MI26_TRIG_RES__LINE];
  Dest->TrigSignalLine = Dest->SStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_LINE];

  // Process frame
  
  ViSrcW30      = 0;
  ViStatesLine  = 0;
  VPrevLine     = -1;
  
  if ( PrintLvl == 4 ) {
    msg (( MSG_OUT, "Frame data length = %d [W30]", VDataW30Length ));
    msg (( MSG_OUT, "" ));
  }

  VPtDataU32 = &(Src->Data.ADataW32[MapsId * VOneMapsSzW32]);

  // msg (( MSG_OUT, "Msg" ));

  if ( VDataW30Length != 0 ) {
    
    while ( ViSrcW30 < VDataW30Length ) { // Odd W30 nb handling => Don't process last W30

      // Copy StatesLine field

      VStatesLine.W32 = VPtDataU32[ViSrcW30];
      Dest->AStatesRec[ViStatesLine].StatesLine = VStatesLine;
	    VG0Overflow = 0;
	    VG1Overflow = 0;

	  VStatesNbG0 = EFRIO__FSBB0_FHammingDecoder8_4(VStatesLine.F.HitNbG0);
	  VStatesNbG1 = EFRIO__FSBB0_FHammingDecoder8_4(VStatesLine.F.HitNbG1);
	  //msg (( MSG_OUT, " ViStatesLine :%d :VStatesLine.F.HitNbG0 : %X - VStatesNbG0 : %x  - VStatesLine.F.HitNbG1 : %X - VStatesNbG1 : %x  ", ViStatesLine,  VStatesLine.F.HitNbG0,VStatesNbG0,VStatesLine.F.HitNbG1,VStatesNbG1 ));

	  Dest->AStatesRec[ViStatesLine].NbWinG0 = VStatesNbG0;
	  Dest->AStatesRec[ViStatesLine].NbWinG1 = VStatesNbG1;
	  Dest->AStatesRec[ViStatesLine].NbWinTot = VStatesNbG0 + VStatesNbG1;

	  if (VStatesNbG0 > FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP ){
		  VG0Overflow = VStatesNbG0;
		  VStatesNbG0 = FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP;
	  }

	  if (VStatesNbG1 > FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP ){
		  VG1Overflow = VStatesNbG1;
		  VStatesNbG1 = FSBB0__ZS_FFRAME_MAX_STATES_NB_PER_GROUP; 
	  }	  
      
      if ( (PrintLvl == 5) || (PrintLvl == 7) ) {
		    msg (( MSG_OUT, " ViStatesLine :%d :VStatesLine.W32 : %X - NbWinG0 : %d  - NbWinG1 : %d  ", ViStatesLine,  VStatesLine.W32,Dest->AStatesRec[ViStatesLine].NbWinG0,Dest->AStatesRec[ViStatesLine].NbWinG1 ));
	  
			msg (( MSG_OUT, "Line %4d - %d HitNbG0 (Ovf:%d) - %d HitNbG1 (Ovf:%d)", VStatesLine.F.SLineAddr, VStatesNbG0, VG0Overflow,VStatesNbG1,VG1Overflow ));
        
      }
      
      ++ViSrcW30;
      
	  if ( ViSrcW30 >= VDataW30Length ){
		break;
	  }
      
	  
      // Copy states G1
      if (VStatesNbG1 > 0){
		  for ( ViState=0; ViState < VStatesNbG1; ViState++ ) {
				// 22/05/14 - MS GCMODIF

			Dest->AStatesRec[ViStatesLine].AStatesG1[ViState].W32 = VPtDataU32[ViSrcW30];
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

			Dest->AStatesRec[ViStatesLine].AStatesG0[ViState].W32 = VPtDataU32[ViSrcW30];
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
} // end EFRIO__FSBB0_FConvEfrioFrameToZsFFrame


/*******************************************************************************
Prototype :
Goal      :
Inputs    :
Ouputs    :
Globals   :
Remark    :
Level     :
Date      : 14/10/2014
Doc date  : 14/10/2014
Rev       : copy of APP__FConvZsFFrameToMatDiscriBit from read_flex_rio_daq_file_v22_gc
          :
Author    : Matthieu SPECHT
E-mail    : matthieu.specht@iphc.cnrs.fr
Labo      : IPHC */
/******************************************************************************/

SInt32 EFRIO__FSBB0_FConvZsFFrameToMatDiscriBit ( FSBB0__TZsFFrame* PtSrc, FSBB0__TMatDiscriBit* PtDest, SInt32* PtOvfCnt, SInt8 PrintLvl ) {

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

    if (PtSrc->StatesRecNb > 0){
  
	  for ( ViStatesLine=0; ViStatesLine < PtSrc->StatesRecNb; ViStatesLine++ ) {
		
		VStatesNbG0 = PtSrc->AStatesRec[ViStatesLine].NbWinG0;
		VStatesNbG1 = PtSrc->AStatesRec[ViStatesLine].NbWinG1;
		ViTopLine    = ((PtSrc->AStatesRec[ViStatesLine].StatesLine.F.SLineAddr + 1) * 4) - 1 ;


		// States in one StateRec loop
		
		
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

		
		if ( ViTopLine >= FSBB0__MAT_DISCRI_LINES_NB) {
		  err_warning (( ERR_OUT, "ViTopLine=%d >= FSBB0__MAT_DISCRI_LINES_NB=%d => Force %d", ViTopLine, FSBB0__MAT_DISCRI_LINES_NB, FSBB0__MAT_DISCRI_LINES_NB - 1 ));
		  ViTopLine = FSBB0__MAT_DISCRI_LINES_NB - 1;
		}
		// Decode Hits for Group 1
		if ( VStatesNbG1 > 0 ){
			for ( ViState = 0; ViState < VStatesNbG1; ViState++ ) {
					
			  VFirstCol = (PtSrc->AStatesRec[ViStatesLine].AStatesG1[ViState].F.ColAddr) + 224;
			
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


  if ( PtOvfCnt != NULL ) {
    *PtOvfCnt = VOfvCnt;
  }

  // err_error (( ERR_OUT, "TRACE - VHitCnt=%d", VHitCnt )); // 13/02/2013
 
  return (VHitCnt);
} // end EFRIO__FSBB0_FConvZsFFrameToMatDiscriBit








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


SInt32 EFRIO__FSBB0_FConvNI6562ToFlexRIOEudet2Mode ( SInt32 BoardId, void* PtFrmRaw, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode ) {
  
  EFRIO__TContext*     VPtCont   = &EFRIO__VGContext;
  EFRIO__TBoardConf*   VPtBoard  = NULL;
  EFRIO__TRunCont*     VPtRun    = &VPtCont->RunCont;
  EFRIO__TFrameList*   VPtFrList = &VPtCont->AAcqFrameList[0];
  EFRIO__TFrame*       VPtFrame;
  EFRIO__TTriggerRec*  VPtTmpTrigRec = VPtCont->PtTmpTrigRec;
  FSBB0__TZsFFrameRaw *  VPtSrcFsbb0FRmRaw ;
  
  SInt32               VAcqId;
  UInt8*               VPtAcqData;
  SInt32               VFrameNbFromBoardDrv;
  SInt32               ViFrame;
  //SInt32               V6iFrame;
    // 01/10/2014 - MS
  SInt32 ViFrameX6[6];

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
  SInt8                ViFSBB0;
  //UInt32*              VPtCpySrcW32;
  UInt32*              VPtDataW32FSBBNo0;
  UInt32*              VPtDataW32FSBBNo1;
  UInt32*              VPtDataW32FSBBNo2;
  UInt32*              VPtDataW32FSBBNo3;
  UInt32*              VPtDataW32FSBBNo4;
  UInt32*              VPtDataW32FSBBNo5;
  UInt32*              VAPtCpyDestW32[6];  
  UInt64*              VPtCpySrcW64;
  UInt64*              VPtCpyDestW64;
  UInt32*              VPtEChanSrcW32;
  SInt16               VEChanTrigFieldCnt; // Extra channel trigger fields counter
  UInt32               VEChanTrigField;
  SInt32               VTotAcqSz;
  SInt8                ViFSBB0ChkDataLength;

  
  #ifdef EFRIO__INCLUDE_PARA_PORT
     PPO_FOutD5 ( 0 /* Id */, 1 /* State */ );
  #endif
  
  if ( VPtCont->InfInitDone == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Abort because : Init not done") );
  }
  
  err_trace (( ERR_OUT, "EFRIO__FSBB0_FConvNI6562ToFlexRIOEudet2Mode (P=%x, EltNb=%d)", PtFrmRaw, EltNb ));
  
  // Pointers parameters check
  
  err_retnull ( PtFrmRaw, (ERR_OUT,"PtSrcW32 = NULL") );
  
  // Init board conf record pointer
  
  if ( (BoardId < 0) || (BoardId >= EFRIO__MAX_BOARDS_NB) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Board Id=%d out of range [0..%d])", BoardId, EFRIO__MAX_BOARDS_NB - 1) );
  }
  
  VPtBoard = &EFRIO__VGContext.ABoardsConf[BoardId];
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / FSBB0__ZS_FFRAME_MODE_1X160MHZ_W8_SZ ) / 6; // Divide by 7 because of extral channel
  
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
  
  VPtSrcFsbb0FRmRaw = (FSBB0__TZsFFrameRaw*)PtFrmRaw;
  
  ViSrcW32          = 0;
  VDataLengthErrCnt = 0;
  
  for ( ViFrame=0; ViFrame < VPtBoard->FrameNbPerAcq; ViFrame++ ) {
    
	ViFrameX6[0]= 6 * ViFrame;
	ViFrameX6[1]= ViFrameX6[0] + 1;
	ViFrameX6[2]= ViFrameX6[0] + 2;
	ViFrameX6[3]= ViFrameX6[0] + 3;
	ViFrameX6[4]= ViFrameX6[0] + 4;
	ViFrameX6[5]= ViFrameX6[0] + 5;
	
    
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
    VPtFrame->Header.MapsName     = (UInt16) ASIC__FSBB0;
    VPtFrame->Header.MapsNb       = VPtRun->ParMi26Nb;
    
    VPtFrame->Header.AMapsHeader[0]    = VPtSrcFsbb0FRmRaw[ViFrameX6[0]].Header.W32;
    VPtFrame->Header.AMapsHeader[1]    = VPtSrcFsbb0FRmRaw[ViFrameX6[1]].Header.W32;
    VPtFrame->Header.AMapsHeader[2]    = VPtSrcFsbb0FRmRaw[ViFrameX6[2]].Header.W32;
    VPtFrame->Header.AMapsHeader[3]    = VPtSrcFsbb0FRmRaw[ViFrameX6[3]].Header.W32;
    VPtFrame->Header.AMapsHeader[4]    = VPtSrcFsbb0FRmRaw[ViFrameX6[4]].Header.W32;
    VPtFrame->Header.AMapsHeader[5]    = VPtSrcFsbb0FRmRaw[ViFrameX6[5]].Header.W32;
   // VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = PtSrcW32[ViSrcW32];
   // ++ViSrcW32;
   // ++VEChanTrigFieldCnt;
   // Datalength
    VADataLengthW32[0] = VPtSrcFsbb0FRmRaw[ViFrameX6[0]].UsefulDataLengthW30;
    VADataLengthW32[1] = VPtSrcFsbb0FRmRaw[ViFrameX6[1]].UsefulDataLengthW30;
    VADataLengthW32[2] = VPtSrcFsbb0FRmRaw[ViFrameX6[2]].UsefulDataLengthW30;
    VADataLengthW32[3] = VPtSrcFsbb0FRmRaw[ViFrameX6[3]].UsefulDataLengthW30;
    VADataLengthW32[4] = VPtSrcFsbb0FRmRaw[ViFrameX6[4]].UsefulDataLengthW30;
    VADataLengthW32[5] = VPtSrcFsbb0FRmRaw[ViFrameX6[5]].UsefulDataLengthW30;
    
    VDataLengthW32Max = MATH_FUInt32Max ( VADataLengthW32, 6 );
	// Framecounter
	 VPtFrame->Header.AMapsFrameCnt[0] = VPtSrcFsbb0FRmRaw[ViFrameX6[0]].DataLengthRemFrCnt.F.FrameCnt;
	 VPtFrame->Header.AMapsFrameCnt[1] = VPtSrcFsbb0FRmRaw[ViFrameX6[1]].DataLengthRemFrCnt.F.FrameCnt;
	 VPtFrame->Header.AMapsFrameCnt[2] = VPtSrcFsbb0FRmRaw[ViFrameX6[2]].DataLengthRemFrCnt.F.FrameCnt;
	 VPtFrame->Header.AMapsFrameCnt[3] = VPtSrcFsbb0FRmRaw[ViFrameX6[3]].DataLengthRemFrCnt.F.FrameCnt;
	 VPtFrame->Header.AMapsFrameCnt[4] = VPtSrcFsbb0FRmRaw[ViFrameX6[4]].DataLengthRemFrCnt.F.FrameCnt;
	 VPtFrame->Header.AMapsFrameCnt[5] = VPtSrcFsbb0FRmRaw[ViFrameX6[5]].DataLengthRemFrCnt.F.FrameCnt;

    
    
    // A better test ( on each VADataLengthW16[i] ? on each W16 on D0 & D1 ? ) should be done !
    
    if ( VDataLengthW32Max > FSBB0__ZS_FFRAME_MODE_1X160MHZ_MAX_DATA_PART_W16_SZ ) {

      ++VDataLengthErrCnt;
      err_error (( ERR_OUT, "---------------------------------------------------------------------" ));
      err_error (( ERR_OUT, "HW error on data length -> Max W16 nb = %d -> Force 0 !", VDataLengthW32Max ));
      
      for ( ViFSBB0ChkDataLength = 0; ViFSBB0ChkDataLength < 6; ViFSBB0ChkDataLength++ ) {
        if ( VADataLengthW32[ViFSBB0ChkDataLength] > FSBB0__ZS_FFRAME_MODE_1X160MHZ_MAX_DATA_PART_W16_SZ ) {
          err_error (( ERR_OUT, "Error chip[%d] = %d W16", ViFSBB0ChkDataLength, VADataLengthW16[ViFSBB0ChkDataLength] ));
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
      
      for ( ViFSBB0=0; ViFSBB0 < 6; ViFSBB0++ ) {
        VADataLengthW8[ViFSBB0]  = VADataLengthW32[ViFSBB0] * 4;
        VADataLengthW16[ViFSBB0] = VADataLengthW32[ViFSBB0] * 2;
      }
      
      VDataLengthW8Max  = VDataLengthW32Max * 4;
      VDataLengthW16Max = VDataLengthW32Max * 2;
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
 
    VPtDataW32FSBBNo0 = (UInt32*) VPtSrcFsbb0FRmRaw[ ViFrameX6[0] ].ADataW32;
    VPtDataW32FSBBNo1 = (UInt32*) VPtSrcFsbb0FRmRaw[ ViFrameX6[1] ].ADataW32;
    VPtDataW32FSBBNo2 = (UInt32*) VPtSrcFsbb0FRmRaw[ ViFrameX6[2] ].ADataW32;
    VPtDataW32FSBBNo3 = (UInt32*) VPtSrcFsbb0FRmRaw[ ViFrameX6[3] ].ADataW32;
    VPtDataW32FSBBNo4 = (UInt32*) VPtSrcFsbb0FRmRaw[ ViFrameX6[4] ].ADataW32;
    VPtDataW32FSBBNo5 = (UInt32*) VPtSrcFsbb0FRmRaw[ ViFrameX6[5] ].ADataW32;

 
 
    //VPtCpySrcW32      = (UInt32*) &PtSrcW32[ViSrcW32];
    VAPtCpyDestW32[0] = (UInt32*) VPtFrame->Data.ADataW32;
    VAPtCpyDestW32[1] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 +       VDataLengthW32Max   );
    VAPtCpyDestW32[2] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 2 * VDataLengthW32Max ) );
    VAPtCpyDestW32[3] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 3 * VDataLengthW32Max ) );
    VAPtCpyDestW32[4] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 4 * VDataLengthW32Max ) );
    VAPtCpyDestW32[5] = (UInt32*) ( (UInt32*) VPtFrame->Data.ADataW32 + ( 5 * VDataLengthW32Max ) );
    
    for ( ViDataCpy=0; ViDataCpy < VDataLengthW32Max; ViDataCpy++ ) {
      
      *VAPtCpyDestW32[0] = VPtDataW32FSBBNo0[ViDataCpy];
      ++VAPtCpyDestW32[0];
      //++VPtCpySrcW32;
      
      *VAPtCpyDestW32[1] =  VPtDataW32FSBBNo1[ViDataCpy];;
      ++VAPtCpyDestW32[1];
     // ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[2] =  VPtDataW32FSBBNo2[ViDataCpy];;
      ++VAPtCpyDestW32[2];
      //++VPtCpySrcW32;
      
      *VAPtCpyDestW32[3] =  VPtDataW32FSBBNo3[ViDataCpy];;
      ++VAPtCpyDestW32[3];
     // ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[4] =  VPtDataW32FSBBNo4[ViDataCpy];;
      ++VAPtCpyDestW32[4];
     // ++VPtCpySrcW32;
      
      *VAPtCpyDestW32[5] =  VPtDataW32FSBBNo5[ViDataCpy];;
      ++VAPtCpyDestW32[5];
      //++VPtCpySrcW32;
      
      
     // VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = *VPtCpySrcW32;
     // ++VPtCpySrcW32;
     // ++VEChanTrigFieldCnt;
    }
    
   // VPtEChanSrcW32 = (UInt32*) VPtCpySrcW32;
    
  #endif
  
  
    // Continue extraction of trigger fields from trigger channel WHILE needed
        
   // VPtEChanSrcW32 += 6; // Bypass Mi26 x 6 data
    
    //do {
     // VEChanTrigField = *VPtEChanSrcW32;
      
    //  if ( VEChanTrigFieldCnt >= EFRIO__EXTRA_CHAN__MAX_TRIGGER_FIELD_NB ) {
    //    err_warning (( ERR_OUT, "Max trigger field nb reached = %d <=> %d max trigger info !", EFRIO__EXTRA_CHAN__MAX_TRIGGER_FIELD_NB, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB ));
     //   break;
      //}
      
    //  VPtTmpTrigRec->ATrig[VEChanTrigFieldCnt] = VEChanTrigField;
    //  VPtEChanSrcW32 += 7;
    //  ++VEChanTrigFieldCnt;
    //} while ( (VEChanTrigField & 0x80000000) == 0 );
    
    
    //ViSrcW32 += (6 * FSBB0__ZS_FFRAME_RAW_MAX_W32);
    
    
    //    VptZsFFrameRaw[V6iFrame].Trailer = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * VADataLengthW32[0])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    //    ++ViSrcW32;
    
    //    VZero = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 6];
    //    VptZsFFrameRaw[V6iFrame].Zero = VZero;
    //    ++ViSrcW32;
    
    //    VZero2 = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 12];
    //    VptZsFFrameRaw[V6iFrame].Zero2 = VZero2;
    //    ++ViSrcW32;
    
     VPtFrame->Header.AMapsTrailer[0] = VPtSrcFsbb0FRmRaw[ViFrameX6[0]].Trailer.W32;
    
    VPtFrame->Header.AMapsTrailer[1] = VPtSrcFsbb0FRmRaw[ViFrameX6[1]].Trailer.W32;
    
    VPtFrame->Header.AMapsTrailer[2] = VPtSrcFsbb0FRmRaw[ViFrameX6[2]].Trailer.W32;
    
    VPtFrame->Header.AMapsTrailer[3] = VPtSrcFsbb0FRmRaw[ViFrameX6[3]].Trailer.W32;
    
    VPtFrame->Header.AMapsTrailer[4] = VPtSrcFsbb0FRmRaw[ViFrameX6[4]].Trailer.W32;
    
    VPtFrame->Header.AMapsTrailer[5] = VPtSrcFsbb0FRmRaw[ViFrameX6[5]].Trailer.W32;   
    
    //++ViSrcW32; // To count extra channel
    
    
    // Read trigger number from "Mi26" frame
    
    if ( TrigStatus == 0 ) {
      //VZero  = PtSrcW32[(FSBB0__ZS_FFRAME_RAW_MAX_W32 * ViFrameX6[0]) + 18 + (6 * FSBB0__ZS_FFRAME_RAW_MAX_W32) +  7];
    }
    
    // Emulate trigger by overwriting info from "Mi26" frame
    
    else {
      VZero = (TrigStatus << 16);
    }
    
    // VZero  = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) +  7];
    
    
    
   // VZero2 = PtSrcW32[(MI26__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 14];
    
    //ViSrcW32 += 14; // 7 times 2 zero fields = 14
    
    // Extraction of trigger info from "Mimosa 26 frame" zero fields
    
   // VTrigNb = (VZero  & 0xFFFF0000) >> 16;

    //if ( VTrigNb > EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ) {
    //  err_warning (( ERR_OUT, "VTrigNb=%d > Max=%d => Limit to max !", VTrigNb, EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26 ));
    //  VTrigNb = EFRIO__EXTRA_CHAN__MAX_TRIGGER_INFO_NB_COMPATIBLE_WITH_MI26;
    //}
    
    //if ( VTrigNb != 0 ) {
    //  VATrigVal[0]    = (VZero  & 0x0000FFFF);
    //  VATrigVal[1]    = (VZero2 & 0xFFFF0000) >> 16;
    //  VATrigVal[2]    = (VZero2 & 0x0000FFFF);
      
    //  VATrigLine[0]   = VATrigVal[0] / 16;
    //  VATrigLine[1]   = VATrigVal[1] / 16;
    //  VATrigLine[2]   = VATrigVal[2] / 16;
      
    //  VATrigClk[0]    = VATrigVal[0] % 16;
   //   VATrigClk[1]    = VATrigVal[1] % 16;
    //  VATrigClk[2]    = VATrigVal[2] % 16;
            
      // Before 13/07/2012
      //
      // VAMi26Trig[0]   = VATrigLine[0] /* + ( VATrigClk[0] << 10 ) */;
      // VAMi26Trig[1]   = VATrigLine[1] /* + ( VATrigClk[1] << 10 ) */;
      // VAMi26Trig[2]   = VATrigLine[2] /* + ( VATrigClk[2] << 10 ) */;
      
      // Since 13/07/2012
      
    //  VAMi26Trig[0]   = VATrigLine[0]  + ( VATrigClk[0] << 10 );
    //  VAMi26Trig[1]   = VATrigLine[1]  + ( VATrigClk[1] << 10 );
    //  VAMi26Trig[2]   = VATrigLine[2]  + ( VATrigClk[2] << 10 );
            
    //}
    
    //else {
   //   VAMi26Trig[0] = VAMi26Trig[1] = VAMi26Trig[2] = 0;
   // }
    
    // Add trigger info in frame header
    
    VPtFrame->Header.TriggerNb        = VPtSrcFsbb0FRmRaw[ViFrameX6[0]].SStatus.ATrigRes[ASIC__ULT1_TRIG_TOT_NB];
    VPtFrame->Header.AMapsTrigInfo[0] = VPtSrcFsbb0FRmRaw[ViFrameX6[0]].SStatus.ATrigRes[0];
    VPtFrame->Header.AMapsTrigInfo[1] = VPtSrcFsbb0FRmRaw[ViFrameX6[0]].SStatus.ATrigRes[1];
    VPtFrame->Header.AMapsTrigInfo[2] = VPtSrcFsbb0FRmRaw[ViFrameX6[0]].SStatus.ATrigRes[2];
  
    // Add trigger info in trigger record
    
    //VPtTrigRec = (EFRIO__TTriggerRec*) (VPtFrame->Data.ADataW32 + VDataLengthW32ToCpy);
    
  #ifdef EFRIO__FRAME_TAGS_ENABLE
    VPtTmpTrigRec->Tag      = EFRIO__FRAME_TAG_TRIG;
  #endif
  
    
    //VPtTmpTrigRec->TrigNb   = VTrigNb;
    //VPtTmpTrigRec->TotSz    = VEmptyTrigRecSz + (VTrigNb * EFRIO__EXTRA_CHAN__TRIGGER_INFO_SZ);
    //VPtTmpTrigRec->TrigType = 2;
    
    //memcpy ( VPtTrigRec, VPtTmpTrigRec, VPtTmpTrigRec->TotSz );

    // Calculate size of frame record while filled ( fixed part + variable length (data & trigger) )
    
    // Calculate size will be 2 x W32 higher than needed, because following fields will be counted twice
    //
    // - Field UInt32 ADataW32[0] of data record -> EFRIO__TFrameData
    // - Field UInt32 ATrig[0] of trigger record -> EFRIO__TTriggerRec
    
    //VFullFrameRecSz = VEmptyFrameRecSz + VDataLengthW8ToCpy + VPtTrigRec->TotSz;

   // VTotAcqSz += VFullFrameRecSz;
    
    // Update frame field total size & trigger offset size
    
    VPtFrame->TotSz         = VFullFrameRecSz;
    VPtFrame->TrigRecOffset = VEmptyFrameRecSz + VDataLengthW8ToCpy;
    
    // Check frame relevant fields ( DataLength, Header, frame counter, trailer => NOT data part )
    // Enable / Disable done by a call to EFRIO__FTestOnDataStartStop ( Start, PrintLvl )
    
    //EFRIO__MI26_FChkFrameLight ( 0 /* FuncId */ , ViFrame, VPtFrame, 6 /* Mi26Nb */ );
    
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
}  /*end EFRIO__FSBB0_FConvNI6562ToFlexRIOEudet2Mode  */



/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 EFRIO__MI26_FFRioAcqDeserDataEudet3Mode6Mi26 (
          :   SInt32 BoardId , UInt32*  PtSrcW32, SInt32 EltNb,
          :   SInt32 AcqStatus, SInt32 TrigStatus,  SInt8 DataConvertMode )
          :
Goal      : Flex RIO readout function for six Fsbb0 in EUDET mode 3
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
Date      : 01/10/2014
Rev       :
Doc date  : 01/10/2014
Author    : Matthieu SPECHT
E-mail    : matthieu.specht@iphc.cnrs.fr
Labo      : DRS - IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 EFRIO__FSBB0_FConvNi6562ToFlexRIOEudet3Mode ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode ) {
  
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
  SInt8                ViFSBB0;
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
  
  err_trace (( ERR_OUT, "EFRIO__FSBB0_FConvNi6562ToFlexRIOEudet3Mode (P=%x, EltNb=%d)", PtSrcW32, EltNb ));
  
  // Pointers parameters check
  
  err_retnull ( PtSrcW32, (ERR_OUT,"PtSrcW32 = NULL") );
  
  // Init board conf record pointer
  
  if ( (BoardId < 0) || (BoardId >= EFRIO__MAX_BOARDS_NB) ) {
    err_retfail ( -1, (ERR_OUT,"Abort : Board Id=%d out of range [0..%d])", BoardId, EFRIO__MAX_BOARDS_NB - 1) );
  }
  
  VPtBoard = &EFRIO__VGContext.ABoardsConf[BoardId];
  
  // Check data size
  
  VFrameNbFromBoardDrv = ( EltNb / FSBB0__ZS_FFRAME_MODE_1X160MHZ_W16_SZ ) / 7; // Divide by 7 because of extral channel
  
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
      VZero  = PtSrcW32[(FSBB0__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + (7 * FSBB0__ZS_FFRAME_RAW_MAX_W32) +  7];
    }
    
    // Emulate trigger by overwriting info from "Mi26" frame
    // BUT first read it from board RAM in order to get real execution time
    
    else {
      VZero  = PtSrcW32[(FSBB0__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + (7 * FSBB0__ZS_FFRAME_RAW_MAX_W32) +  7];
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
    ViSrcW32           = VFrameId * 7 * FSBB0__ZS_FFRAME_RAW_TOTAL_SZ_W32;
    
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

      for ( ViFSBB0=0; ViFSBB0 < 6; ViFSBB0++ ) {
        VADataLengthW8[ViFSBB0]  = 0; 
        VADataLengthW16[ViFSBB0] = 0; 
        VADataLengthW32[ViFSBB0] = 0; 
      }
      
*/            
      VDataLengthW8Max  = 0;
      VDataLengthW32Max = 0;
      
      ++VDataLengthErrCnt;
      err_error (( ERR_OUT, "HW error on data length -> Max W16 nb = %d -> Force 0 !", VDataLengthW16Max ));
      
    }
    
    else {
      
      for ( ViFSBB0=0; ViFSBB0 < 6; ViFSBB0++ ) {
        VADataLengthW8[ViFSBB0]  = VADataLengthW16[ViFSBB0] * 2;
        VADataLengthW32[ViFSBB0] = VADataLengthW16[ViFSBB0] / 2;
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
    

  
    ViSrcW32 += (7 * FSBB0__ZS_FFRAME_RAW_MAX_W32);
    
    
    //    VptZsFFrameRaw[V6iFrame].Trailer = PtSrcW32[(FSBB0__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * VADataLengthW32[0])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    //    ++ViSrcW32;
    
    //    VZero = PtSrcW32[(FSBB0__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 6];
    //    VptZsFFrameRaw[V6iFrame].Zero = VZero;
    //    ++ViSrcW32;
    
    //    VZero2 = PtSrcW32[(FSBB0__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * MI26__ZS_FFRAME_RAW_MAX_W32) + 12];
    //    VptZsFFrameRaw[V6iFrame].Zero2 = VZero2;
    //    ++ViSrcW32;
    
    
    VPtFrame->Header.AMapsTrailer[0] = PtSrcW32[(FSBB0__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + (7 * VADataLengthW32[0])];    // 21 = 7 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[1] = PtSrcW32[(FSBB0__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + 1 + (7 * VADataLengthW32[1])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[2] = PtSrcW32[(FSBB0__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + 2 + (7 * VADataLengthW32[2])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[3] = PtSrcW32[(FSBB0__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + 3 + (7 * VADataLengthW32[3])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[4] = PtSrcW32[(FSBB0__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + 4 + (7 * VADataLengthW32[4])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[5] = PtSrcW32[(FSBB0__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + 5 + (7 * VADataLengthW32[5])];
    ++ViSrcW32;
    
    ++ViSrcW32; // To count extra channel
    

    // Read trigger number from "Mi26" frame
    
    if ( TrigStatus == 0 ) {
      VZero  = PtSrcW32[(FSBB0__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) +  7];
    }
    
    // Emulate trigger by overwriting info from "Mi26" frame
    
    else {
      VZero = (TrigStatus << 16);
    }
    
    // VZero  = PtSrcW32[(FSBB0__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) +  7];
    
    
    
    VZero2 = PtSrcW32[(FSBB0__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7FrameId) + 21 + (7 * MI26__ZS_FFRAME_RAW_MAX_W32) + 14];
    
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
      
      // Before 13/07/2012
      // 
      // VAMi26Trig[0]   = VATrigLine[0] /* + ( VATrigClk[0] << 10 ) */;
      // VAMi26Trig[1]   = VATrigLine[1] /* + ( VATrigClk[1] << 10 ) */;
      // VAMi26Trig[2]   = VATrigLine[2] /* + ( VATrigClk[2] << 10 ) */;
      
      // Since 13/07/2012
      
      VAMi26Trig[0]   = VATrigLine[0]  + ( VATrigClk[0] << 10 );
      VAMi26Trig[1]   = VATrigLine[1]  + ( VATrigClk[1] << 10 );
      VAMi26Trig[2]   = VATrigLine[2]  + ( VATrigClk[2] << 10 );
      
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
    
   // EFRIO__MI26_FChkFrameLight ( 0 /* FuncId */ , VFrameId, VPtFrame, 6 /* Mi26Nb */ );
    

  
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








// $$$$$$$$$$$$$$

//          : 03/02/2014
//          : - Add new tests on data
          

SInt32 EFRIO__FSBB0_FFRioAcqDeserDataEudet2Mode6Ult1 ( SInt32 BoardId, UInt32* PtSrcW32, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, SInt8 DataConvertMode, SInt8 TriggerHandlingMode ) {
  
  EFRIO__TContext*         VPtCont   = &EFRIO__VGContext;
  EFRIO__TBoardConf*       VPtBoard  = NULL;
  EFRIO__TRunCont*         VPtRun    = &VPtCont->RunCont;
  EFRIO__TFrameList*       VPtFrList = &VPtCont->AAcqFrameList[0];
  EFRIO__TFrame*           VPtFrame;
  EFRIO__TTriggerRec*      VPtTmpTrigRec = VPtCont->PtTmpTrigRec;
  EFRIO__TTestOnDataCont*  VPtDataTest   = &VPtCont->TestOnDataCont;
  
  
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
  SInt8                ViFsbb0ChkDataLength;
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
  
  VFrameNbFromBoardDrv = ( EltNb / FSBB0__ZS_FFRAME_MODE_1X160MHZ_W32_SZ ) / 7; // Divide by 7 because of extral channel
    
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
    
    if ( VDataLengthW16Max > FSBB0__ZS_FFRAME_RAW_MAX_W16 ) {

      ++VDataLengthErrCnt;
      err_error (( ERR_OUT, "---------------------------------------------------------------------" ));
      err_error (( ERR_OUT, "HW error on data length -> Max W16 nb = %d -> Force 0 !", VDataLengthW16Max ));
      
      for ( ViFsbb0ChkDataLength = 0; ViFsbb0ChkDataLength < 6; ViFsbb0ChkDataLength++ ) {
        if ( VADataLengthW16[ViFsbb0ChkDataLength] > FSBB0__ZS_FFRAME_RAW_MAX_W16 ) {
          err_error (( ERR_OUT, "Error chip[%d] = %d W16", ViFsbb0ChkDataLength, VADataLengthW16[ViFsbb0ChkDataLength] ));
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
    
    
    ViSrcW32 += (7 * FSBB0__ZS_FFRAME_RAW_MAX_W32);
    
    
    //    VptZsFFrameRaw[V6iFrame].Trailer = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * VADataLengthW32[0])];    // 18 = 6 x 3 Fields nb before first data = Header, Frame cnt, Data length
    //    ++ViSrcW32;
    
    //    VZero = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 6];
    //    VptZsFFrameRaw[V6iFrame].Zero = VZero;
    //    ++ViSrcW32;
    
    //    VZero2 = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V6iFrame) + 18 + (6 * ULT1__ZS_FFRAME_RAW_MAX_W32) + 12];
    //    VptZsFFrameRaw[V6iFrame].Zero2 = VZero2;
    //    ++ViSrcW32;
    
    
    VPtFrame->Header.AMapsTrailer[0] = PtSrcW32[(FSBB0__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + (7 * VADataLengthW32[0])];    // 21 = 7 x 3 Fields nb before first data = Header, Frame cnt, Data length
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[1] = PtSrcW32[(FSBB0__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + 1 + (7 * VADataLengthW32[1])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[2] = PtSrcW32[(FSBB0__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + 2 + (7 * VADataLengthW32[2])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[3] = PtSrcW32[(FSBB0__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + 3 + (7 * VADataLengthW32[3])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[4] = PtSrcW32[(FSBB0__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + 4 + (7 * VADataLengthW32[4])];
    ++ViSrcW32;
    
    VPtFrame->Header.AMapsTrailer[5] = PtSrcW32[(FSBB0__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + 5 + (7 * VADataLengthW32[5])];
    ++ViSrcW32;
    
    ++ViSrcW32; // To count extra channel
    
    
    // Read trigger number from "Mi26" frame
    
    if ( TrigStatus == 0 ) {
      VZero  = PtSrcW32[(FSBB0__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + (7 * FSBB0__ZS_FFRAME_RAW_MAX_W32) +  7];
    }
    
    // Emulate trigger by overwriting info from "Mi26" frame
    
    else {
      VZero = (TrigStatus << 16);
    }
    
    // VZero  = PtSrcW32[(ULT1__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + (7 * ULT1__ZS_FFRAME_RAW_MAX_W32) +  7];
    
    
    
    VZero2 = PtSrcW32[(FSBB0__ZS_FFRAME_RAW_TOTAL_SZ_W32 * V7iFrame) + 21 + (7 * FSBB0__ZS_FFRAME_RAW_MAX_W32) + 14];
    
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
    
        
    // 03/02/14
    // Check frame relevant fields ( DataLength, Header, frame counter, trailer => NOT data part )
    // Enable / Disable done by a call to EFRIO__FTestOnDataStartStop ( Start, PrintLvl )
    
    // msg (( MSG_OUT, VPtDataTest->ParTestType=%d", VPtDataTest->ParTestType ));
    
    switch ( VPtDataTest->ParTestType ) {
      
      case 1 : {
        //VErrorsOnData = EFRIO__ULT1_FChkFrameLight ( 0 /* FuncId */ , ViFrame, VPtFrame, 8 /* Mi26Nb */ );
        break; }
      
      case 2 : {
        //VErrorsOnData = EFRIO__ULT1_FChkFrameHeaderFrameCntIncTrailer ( 0 /* FuncId */ , VPtFrame, ViFrame, 8 /* Mi26Nb */, VPtFrList->AFramePtr[0]->Header.AMapsFrameCnt[0] /* FrameCntOfFirstFrameOfAcq */ ); // 23/01/2014 : For timing tests at lab
        break; }
      
      case 3 : {
       // VErrorsOnData = EFRIO__ULT1_FChkFrameHeaderFrameCntCmpTrailerMatrix ( 0 /* FuncId */ , VPtFrame, 8 /* Mi26Nb */ ); // 24/01/2014 : For timing tests at lab
        break; }
      
      case 4 : {
       // VErrorsOnData = EFRIO__ULT1_FChkFrameHeaderFrameCntIncTrailerMatrix ( 0 /* FuncId */ ,  VPtFrame, ViFrame, 8 /* Mi26Nb */, VPtFrList->AFramePtr[0]->Header.AMapsFrameCnt[0] /* FrameCntOfFirstFrameOfAcq */ ); // 24/01/2014 : For timing tests at lab
        break; }
      
      case 5 : {
       // VErrorsOnData = EFRIO__ULT1_FCheckTFrame ( VPtFrame, 0 /* EmulErr */ );
       // VErrorsOnData = VErrorsOnData || EFRIO__ULT1_FChkFrameHeaderFrameCntCmpTrailerMatrix ( 0 /* FuncId */ , VPtFrame, 8 /* Mi26Nb */ ); // 24/01/2014 : For timing tests at lab
        break; }
      
      case 6 : {
        //VErrorsOnData = EFRIO__ULT1_FCheckTFrame ( VPtFrame, 0 /* PrintLevel */ );
        break; }
      
      case 7 : {
        //VErrorsOnData = EFRIO__ULT1_FCheckTFrame ( VPtFrame, 1 /* EmulErr */ );
        //VErrorsOnData = VErrorsOnData || EFRIO__ULT1_FChkFrameHeaderFrameCntCmpTrailerMatrix ( 0 /* FuncId */ , VPtFrame, 8 /* Mi26Nb */ ); // 24/01/2014 : For timing tests at lab
        break; }
      
    }
    
        
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
          :
          : 17/01/2012
          : - Implementation of "sw" handling of Mi26 / "hard coded"
          :   It is enable by EFRIO__FREE_MI26_NB directive and ONLY implemented in mode EUDET 2
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
  

SInt32 EFRIO__FSBB0_FFRioAcqDeserDataFsbb0 ( SInt8 Fsbb0Nb, SInt32 BoardId, UInt32* PtSrcW32AsPt, UInt32 PtSrcW32AsInt, SInt32 EltNb, SInt32 AcqStatus, SInt32 TrigStatus, UInt32 WaitMsAtEnd, SInt8 DataConvertMode, SInt8 TriggerHandlingMode, SInt16 EmuleMode ) {
  
  EFRIO__TContext*     VPtCont       = &EFRIO__VGContext;
  EFRIO__TRunCont*     VPtRunCont    = &EFRIO__VGContext.RunCont;
  EFRIO__USR_TContext* VPtUsrContext = &EFRIO__USR_VGContext;
    
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



  while (1) {
    
    // IPHC mode
    
    if ( DataConvertMode == EFRIO__TRF_MODE_IPHC ) {
      
      switch ( Fsbb0Nb ) {
        
        case 1 : {
          err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_IPHC -> This mode is not handled now", Fsbb0Nb ) );
          break; }
        
        case 6 : {
          err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_IPHC -> This mode is not handled now", Fsbb0Nb ) );
          break; }
        
        default : {
          err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_IPHC -> This number of Mi26 = %d is not handled now", Fsbb0Nb ) );
          break; }
      }
      
      break;
    }
  
    // EUDET mode 1
    
    if ( DataConvertMode == EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN ) {
      
      switch ( Fsbb0Nb ) {
        
        case 1 : {
          err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN -> This mode is not handled now", Fsbb0Nb ) );
          break; }
        
        case 6 : {
          err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN -> This mode is not handled now", Fsbb0Nb ) );
          break; }
        
        default : {
          err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_EUDET_1__NO_TRG_CHAN -> This number of Mi26 = %d is not handled now", Fsbb0Nb ) );
          break; }
        
      }
      
      break;
    }

    // EUDET mode 2



    if ( DataConvertMode == EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES ) {

      
      #ifdef EFRIO__FREE_MI26_NB
      
        VRet = EFRIO__MI26_FFRioAcqDeserDataEudet2ModeNMi26 ( Fsbb0Nb, BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode, TriggerHandlingMode );

      #else
      
        switch ( Fsbb0Nb ) {
          
          case 1 : {
            err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES -> This number of Mi26 = %d is not handled now", Fsbb0Nb ) );
            break; }

          case 5 : {
            err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES -> This number of Mi26 = %d is not handled now", Fsbb0Nb ) );
            break; }
          
          case 6 : {
		  // to modify : fsbb0_frame raw not yet defined and initiialized
            VRet = EFRIO__FSBB0_FConvNI6562ToFlexRIOEudet2Mode ( BoardId,  PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus,  DataConvertMode,  TriggerHandlingMode );
            break; }
  
          case 8 : {
            err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES -> This number of Mi26 = %d is not handled now", Fsbb0Nb ) );
            break; }
  
          case 12 : {
            err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES -> This number of Mi26 = %d is not handled now", Fsbb0Nb ) );
            break; }
          
          default : {
            err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_EUDET_2__TRG_CHAN__SEND_ALL_FRAMES -> This number of Mi26 = %d is not handled now", Fsbb0Nb ) );
            break; }
          
        }
        
      #endif
      
      break;
    }

    // EUDET mode 3
    
    if ( DataConvertMode == EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG ) {
    
  
      #ifdef EFRIO__FREE_MI26_NB
      
        // 14/06/13 : Uses ReadoutMode command line parameter to select between normal readout & readout with etra trigger info (Debug BT June 2013)
      
        if ( VPtUsrContext->ParReadoutMode == 0) {
          // msg (( MSG_OUT, "Normal readout" ));
          VRet = EFRIO__MI26_FFRioAcqDeserDataEudet3ModeNMi26 ( Fsbb0Nb, BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode, TriggerHandlingMode );
        }
        
        else {
          // msg (( MSG_OUT, "Readout WITH extra trigger info" ));
          VRet = EFRIO__MI26_FFRioAcqDeserDataEudet3ModeNMi26ExtaTrigInfo ( Fsbb0Nb, BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode, TriggerHandlingMode );
        }
    
    
      #else
      
        switch ( Fsbb0Nb ) {
          
          case 1 : {
            err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG -> This number of Mi26 = %d is not handled now", Fsbb0Nb ) );
            break; }

          case 5 : {
            err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG -> This number of Mi26 = %d is not handled now", Fsbb0Nb ) );
            break; }
          
          case 6 : {
            //VRet = EFRIO__FSBB0_FConvNi6562ToFlexRIOEudet3Mode ( BoardId, PtSrcW32AsPt, EltNb, AcqStatus, TrigStatus, DataConvertMode, TriggerHandlingMode );
            break; }
  
          case 8 : {
            err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG -> This number of Mi26 = %d is not handled now", Fsbb0Nb ) );
            break; }
          
          default : {
            err_retfail ( -1, (ERR_OUT,"Abort : EFRIO__TRF_MODE_EUDET_3__TRG_CHAN__SEND_FRAMES_WITH_TRIG -> This number of Mi26 = %d is not handled now", Fsbb0Nb ) );
            break; }
          
        }
  
      #endif
      

      break;
    }
    
  } // End while (1)
    
  
  if ( WaitMsAtEnd != 0 ) {
  // 10/10/14 - MS : is it still usefull ??
   // Sleep ( WaitMsAtEnd );
  }
  
  VPtCont->RunCont.ResAcqFunctRetCode = VRet;
  
  if ( VRet > 0 ) {
    VPtRunCont->InfDataRateMeasTotalSz += VRet;
  }  
  
  return (VRet);
  
#endif // NO_MI26
  
}




#endif
