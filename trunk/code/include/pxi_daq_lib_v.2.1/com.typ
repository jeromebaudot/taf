/*******************************************************************************
File      : /dd/sdev_src/c/work/common/units/com/
Goal      : Types definition common to com_pc & com_uc units.
Prj date  : 26/03/2003
File date : 26/03/2003
Doc date  : 26/03/2003
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/*******************************************************************************/


#ifndef COM_TYP
#define COM_TYP


typedef struct {
  SInt32  Index;
  SInt32  TotSz; /* Header + parameters */
  SInt32  Type;
  SInt32  SubType;
  SInt32  RetMode;
  SInt32  RetSz;
  SInt32  ParamSz;
  SInt8*  PtRetBuff;
  char    StrCmd[COM_STR_CMD_SZ];
  SInt32  ParamBuff;
} COM_TCommand;



#endif