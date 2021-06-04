
/**
* ----------------------------------------------------------------------------------
* \file              X:\lib\com\maps\msis1\data\msis1_data_exp.h
* \brief Goal      : Functions prototypes of Mimosis 1 lib
* \brief
* \brief
* \version         : 1.0
* \date Prj date   : 12/05/2021
* \date File date  : 12/05/2021
* \date Doc date   : 12/05/2021
* \author          : Gilles CLAUS
* \author          : gilles.claus@iphc.cnrs.fr
* \author          : CNRS - IN2P3 - IPHC 23 Rue du Loess 67037 STYRASBOURG
*
* Remark : None
*
* ----------------------------------------------------------------------------------
* License   : GNU General Public License
*
* ----------------------------------------------------------------------------------
*/
 

#ifndef MIMOSIS1_DATA_EXP__H
#define MIMOSIS1_DATA_EXP__H


int32_t MIS1__FIsCpuBigEndian ();

SInt32 MIS1__FTrigRecPrint ( UInt32 TrigNb, MIS1__TBtTrigRec* Pt );
SInt32 MIS1__FAcqRawPrint ( MIS1__TBtAcqRawRec* Pt, UInt8 DataWSize, UInt32 FirstDataW, UInt32 NbDataW, UInt8 VRS );

SInt32 MIS1__TBtRunRead_FErrLogUsrPrint ( char Type, char* ErrLocation, char* ErrUserMsg, char* FullMsg );
SInt32 MIS1__TBtRunRead_FMsgLogUsrPrint ( char* Msg );


// 22/05/2021

MIS1__TBtAcqW16A* MIS1__BT_FBtAcqW16AAlloc ( UInt8 Alloc, UInt32* PtRecSz );
SInt32            MIS1__BT_FBtAcqW16AFree ( MIS1__TBtAcqW16A* PtRec );

double            MIS1__BT_FBtAcqW16AFill ( MIS1__TBtAcqRawRec* PtSrc, MIS1__TBtAcqW16A* PtDest, SInt32 FrNb, UInt8 MeasExecTime, UInt8 PrintLvl, UInt8 FuncVers ) ;
double            MIS1__BT_FBtAcqW16AFill_v0 ( MIS1__TBtAcqRawRec* PtSrc, MIS1__TBtAcqW16A* PtDest, SInt32 FrNb, UInt8 MeasExecTime, UInt8 PrintLvl );
double            MIS1__BT_FBtAcqW16AFill_v10 ( MIS1__TBtAcqRawRec* PtSrc, MIS1__TBtAcqW16A* PtDest, SInt32 FrNb, UInt8 MeasExecTime, UInt8 PrintLvl );
double            MIS1__BT_FBtAcqW16AFill_v80 ( MIS1__TBtAcqRawRec* PtSrc, MIS1__TBtAcqW16A* PtDest, SInt32 FrNb, UInt8 MeasExecTime, UInt8 PrintLvl );
SInt32            MIS1__BT_FBtAcqW16APrintMSisU16 ( MIS1__TBtAcqW16A* Pt, UInt8 MSisId, UInt32 FirstW16, UInt32 W16Nb );

MIS1__TBtAcqDec*  MIS1__BT_FBtAcqDecAlloc ( UInt8 Alloc, UInt32* PtRecSz );

SInt32            MIS1__BT_FBtAcqDecFree ( MIS1__TBtAcqDec* PtRec );
double            MIS1__BT_FBtDecodeFrLight ( MIS1__TBtAcqW16A* PtSrc, MIS1__TBtAcqDec* PtDest, SInt8 MSisId, SInt32 FrNb, UInt8 MeasExecTime, UInt8 PrintLvl );
SInt32            MIS1__BT_FBtDecodeFrGetWarnErr ( UInt8 Reset );

double            MIS1__BT_FBtDecodeFr ( MIS1__TBtAcqW16A* PtSrc, MIS1__TBtAcqDec* PtDest, SInt8 MSisId, SInt32 FrNb, UInt8 MeasExecTime, UInt8 PrintLvl );
SInt32            MIS1__BT_FFrDecHeadPrint ( MIS1__TBtFrDecHead* Pt, UInt8 PrintMode );
SInt32            MIS1__BT_FAcqDecPrintGen ( MIS1__TBtAcqDec* Pt, SInt8 MSisId, UInt8 PrintMode );
SInt32            MIS1__BT_FAcqDecPrintPix ( MIS1__TBtAcqDec* Pt, SInt8 MSisId, UInt32 FrId, UInt32 MaxPixToPrint, UInt8 PrintMode );

SInt32            MIS1__BT_FAcqDecCheckFrCntOneMSis ( MIS1__TBtAcqDec* Pt, SInt8 MSisId, UInt8 PrintLvl, UInt32* PtErrNb );
SInt32            MIS1__BT_FAcqDecCheckFrCnt6MSis ( MIS1__TBtAcqDec* Pt, UInt8 PrintLvl, UInt32* PtErrNb );

char*  MIS1__FFrHeader2Str ( MIS1__TDsFrHeader* PtSrc, char* DestStr, SInt32 DestStrSz, SInt32 Mode );
char* MIS1__FFrTrailer2Str ( MIS1__TDsFrTrailer* PtSrc, char* DestStr, SInt32 DestStrSz, SInt32 Mode );




#endif