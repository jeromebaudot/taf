
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

#endif