/*******************************************************************************
File      : /dd/sdev_src/c/work/common/units/u/u.h
Goal      : Function prototypes declaration of
Prj date  : 12/01/2004
File date : //2004
Doc date  : //2002
Rev       : 30/12/2005
          : - stb lib for DLL building
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/*******************************************************************************/

#ifndef PPORT_H

#include "func_header.def"

// FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, ;)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FBegin ( SInt8 Enable, char* FilePath );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FEnd ();)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FOpen ( UInt32 BaseAdr, SInt8 Id );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FEnableReadDataOutPortBeforeWrite ( SInt8 Id, SInt8 Enable );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FDisableHwAccess ( SInt8 Id, SInt8 Disable );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FClose ( SInt8 Id );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FOut ( SInt8 Id, UInt8 Data ) ;)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FCtrl ( SInt8 Id, UInt8 Data ) ;)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FIn ( SInt8 Id ) ;)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FOutD0 ( SInt8 Id, UInt8 State );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FOutD1 ( SInt8 Id, UInt8 State );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FOutD2 ( SInt8 Id, UInt8 State );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FOutD3 ( SInt8 Id, UInt8 State );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FOutD4 ( SInt8 Id, UInt8 State );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FOutD5 ( SInt8 Id, UInt8 State );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FOutD6 ( SInt8 Id, UInt8 State );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FOutD7 ( SInt8 Id, UInt8 State );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FInAck ( SInt8 Id );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FInSel ( SInt8 Id );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FInError ( SInt8 Id );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FInPE ( SInt8 Id );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FInBusy ( SInt8 Id );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FInAckRisingEdge ( SInt8 Id );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FInAckFallingEdge ( SInt8 Id );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FInSelRisingEdge ( SInt8 Id );)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FGetDataOut ( SInt8 Id );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FGetCtrlOut ( SInt8 Id );)

FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FCtrlStrobe ( SInt8 Id, UInt8 State );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FCtrlAutoFeed ( SInt8 Id, UInt8 State );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FCtrlInit ( SInt8 Id, UInt8 State );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 PPO_FCtrlSelect ( SInt8 Id, UInt8 State );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt32 PPO_SPIPol (SInt8 Id, UInt32 Out_Data, SInt8* PtDesync, SInt8 SpiEcho );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, void PPO_SPIReset (SInt8 Id);)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, void PPO_SPITrigger (SInt8 Id);)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, UInt32 PPO_TLU_FReadCnt (SInt8 PortId, SInt8 TryNb, UInt32 Out_Data, SInt8* PtDesync, SInt8 SpiEcho );)
  


#ifndef APP_DLL_IMPORT_EXPORT
  #ifndef PPORT_H
    #define PPORT_H
  #endif
#endif



#endif