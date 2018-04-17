/*******************************************************************************
File      : /dd/sdev_src/c/work/common/units/u/u.c
Goal      : Functions definition of 
Prj date  : 12/01/2004
File date : //2004
Versions  : //2004 => Creation
Doc date  : //2004
Rev       : 30/12/2005
          : - stb lib for DLL building
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/*******************************************************************************/


#ifndef PPORT_C
#define PPORT_C

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : //2007
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 PPO_FBegin ( SInt8 Enable, char* FilePath ) {

  if ( Enable >= 0 ) {
    ERR_FBegin   ( Enable, FilePath );
  }

  
  
  
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
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 22/03/2010
Doc date  : 22/03/2010
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 PPO_FEnableReadDataOutPortBeforeWrite ( SInt8 Id, SInt8 Enable ) {
   
  if ( PPO_VGNoHwAccess[Id] ) {
    err_warning (( ERR_OUT, "Debug mode => No HW access !" ));
    return (0);
  }
  
  PPO_VGReadDataOutPortBeforeWrite[Id] = Enable;

  err_retok (( ERR_OUT, "OK" ));
}


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype :
:
Goal      :
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : //2007
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 PPO_FEnd () {
  
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
Ouputs    : 
          :
Globals   : 
          :
Remark    :
          :
Level     : 
Date      : //2004
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */


/* 12/01/04 */



SInt32 PPO_FOpen ( UInt32 BaseAdr, SInt8 Id ) {

#ifdef APP_USING_DLL_FOR_LOW_LEVEL
	HWPPO_FOpen(  BaseAdr,  Id );

#else

  
  #ifdef CC_APP_WINDOWS
	#ifdef CC_APP_SPI_INOUTWRAPPER
 
     hLib = LoadLibrary("inpout32.dll");        // Charger DLL
     if (hLib == NULL)                          // si erreur de chargement
     {                                          // L'indiquer 
        err_warning ( (ERR_OUT,"Erreur DLL !!"  ) );
        return(-1);    // Et sortir
     }
     Out32 = (oupfuncPtr) GetProcAddress(hLib, "Out32");
     if (Out32 == NULL) {
           err_warning ( (ERR_OUT,"GetProcAddress for Oup32 Failed.\n"));
          return -1;
     }
     Inp32 = (inpfuncPtr) GetProcAddress(hLib, "Inp32");
	 if (Inp32 == NULL) {
           err_warning ( (ERR_OUT,"GetProcAddress for Inp32 Failed.\n"));
          return -1;
     }
	 #endif  
  
  #else
  
  if ( ioperm ( BaseAdr, PPO_ADR_BYTES_SPACE, 1 /* Enable */ ) ) {
    err_retfail ( -1, (ERR_OUT,"ioperm failed") );
  }
  
  #endif
#endif  
  PPO_VGBaseAdr[Id]   = BaseAdr;
  PPO_VGOutAdr [Id]   = BaseAdr;
  PPO_VGIn1Adr [Id]   = BaseAdr + 1;
  PPO_VGCtrlAdr[Id]   = BaseAdr + 2;
  
  PPO_VGPortReady[Id]  = 1;
 
  err_error (( ERR_OUT, "PPO_FOpen /********************WARNING***********************/" ));
  err_error (( ERR_OUT, "the signals Strobe, Autofeed and Select in have been inverted" ));
  err_error (( ERR_OUT, "because these signals are in negative logic" ));
  err_error (( ERR_OUT, "All signal are now in positive logic" ));
  err_error (( ERR_OUT, "27/05/2014 - MS" ));

  msg (( MSG_OUT, "PPO_FOpen /********************WARNING***********************/" ));
  msg (( MSG_OUT, "the signals Strobe, Autofeed and Select in have been inverted" ));
  msg (( MSG_OUT, "because these signals are in negative logic" ));
  msg (( MSG_OUT, "All signal are now in positive logic" ));
  msg (( MSG_OUT, "27/05/2014 - MS" ));


 
  err_retok (( ERR_OUT, "" ));
}

SInt32 PPO_FClose ( SInt8 Id ) {
#ifdef APP_USING_DLL_FOR_LOW_LEVEL
	HWPPO_FClose( Id );

#else
  
  #ifdef CC_APP_WINDOWS
 #ifdef CC_APP_SPI_INOUTWRAPPER

   FreeLibrary(hLib); //Liberer mem  ->DLL
  #endif
  
  #else
  
  if ( ioperm ( PPO_VGBaseAdr[Id], PPO_ADR_BYTES_SPACE, 0 /* Disable */ ) ) {
    err_retfail ( -1, (ERR_OUT,"ioperm failed") );
  }
  
  #endif
#endif  
  PPO_VGPortReady[Id] = 0;

  return (0);
  
}

/* 11/03/2007 */

SInt32 PPO_FDisableHwAccess ( SInt8 Id, SInt8 Disable ) {
  
  PPO_VGNoHwAccess[Id] = Disable;

  err_retok (( ERR_OUT, "" ));
}



/******************************************************************************/

#ifdef CC_APP_WINDOWS

SInt32 PPO_PRIV_FWinOut( UInt8 Data, UInt16 Adr ) {
// 11/04/2014 - MS added conditionnal compilation to be able to use a DLL for the low level function
// PB compil 31/05/2013 => Missing # before ifdef
#ifdef APP_USING_DLL_FOR_LOW_LEVEL
	HWPPO_PRIV_FWinOut(  Data,  Adr );

#else
#ifdef CC_APP_SPI_INOUTWRAPPER
        //output32 (Adr,Data);
        Out32(Adr,Data);

#else
  asm {
    push ax;
    push dx;

    mov  al, Data;
    mov  dx, Adr;
    out  dx, al;

    pop dx;
    pop ax;
  }


#endif
#endif
  return (0);
}

UInt8 PPO_PRIV_FWinIn ( UInt16 Adr ) {
// 11/04/2014 - MS added conditionnal compilation to be able to use a DLL for the low level function
  
  UInt8 VRet = 0;
  //err_warning (( ERR_OUT, "PPO_Priv_FWinIn!" ));
  
#ifdef APP_USING_DLL_FOR_LOW_LEVEL
	VRet = HWPPO_PRIV_FWinIn(  Adr );

#else  
  
  
 
#ifdef CC_APP_SPI_INOUTWRAPPER
VRet = Inp32(Adr);
  //VRet = input32 (Adr);
#else

  asm {
    push ax;
    push dx;

    mov  dx, Adr;
    in  al, dx;
    mov VRet,al;

    pop dx;
    pop ax;
  }


  //err_warning (( ERR_OUT, "PPO_Priv_FWinIn, done" ));
#endif 
#endif  
  return ( VRet );
}

#endif

/******************************************************************************/


SInt32 PPO_PRIV_FOut ( SInt8 Id ) {
  
  SInt32 VRet;
    
  if ( PPO_VGNoHwAccess[Id] ) {
    err_warning (( ERR_OUT, "Debug mode => No HW access !" ));
    return (0);
  }

  #ifdef CC_APP_WINDOWS
  VRet = PPO_PRIV_FWinOut ( PPO_VGOutData[Id], PPO_VGOutAdr [Id] );
  #else
  outb ( PPO_VGOutData[Id], PPO_VGOutAdr [Id] );
  VRet = 0;
  #endif
  
  return (VRet);
}

/* 27/10/2006 */

SInt32 PPO_PRIV_FCtrl ( SInt8 Id ) {
  
  SInt32 VRet;

  if ( PPO_VGNoHwAccess[Id] ) {
    err_warning (( ERR_OUT, "Debug mode => No HW access !" ));
    return (0);
  }
  
  #ifdef CC_APP_WINDOWS      
    VRet = PPO_PRIV_FWinOut ( PPO_VGCtrlData[Id], PPO_VGCtrlAdr [Id] );
  #else
    outb ( PPO_VGCtrlData[Id], PPO_VGCtrlAdr [Id] );
    VRet = 0;
  #endif
  
  return (VRet);
}


/* 22/03/2010 */

SInt32 PPO_PRIV_FReadDataOutPortBeforeWrite ( SInt8 Id ) {
  
  if ( PPO_VGReadDataOutPortBeforeWrite[Id] == 1 ) {
    PPO_VGOutData[Id] = PPO_PRIV_FWinIn ( PPO_VGOutAdr[Id] );
  }
  
  
  return (0);  
}

  


SInt32 PPO_FOut ( SInt8 Id, UInt8 Data ) {
  
  if ( PPO_VGPortReady[Id] == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Port Id=%d not ready", Id ) );
  }

  PPO_PRIV_FReadDataOutPortBeforeWrite ( Id );
  
  PPO_VGOutData[Id] =  Data;
  
  return ( PPO_PRIV_FOut (Id) );
}

/* 27/10/2006 */

SInt32 PPO_FCtrl ( SInt8 Id, UInt8 Data ) {
  
  if ( PPO_VGPortReady[Id] == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Port Id=%d not ready", Id ) );
  }
    
  PPO_VGCtrlData[Id] =  Data;
  
  return ( PPO_PRIV_FCtrl (Id) );
}


SInt32 PPO_FIn ( SInt8 Id ) {
  
  SInt32 VReadVal;
  
  if ( PPO_VGPortReady[Id] == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Port Id=%d not ready", Id ) );
  }
  
  
  if ( PPO_VGNoHwAccess[Id] ) {
    err_warning (( ERR_OUT, "Debug mode => No HW access !" ));
    return (0);
  }
  
  #ifdef CC_APP_WINDOWS
  VReadVal = PPO_PRIV_FWinIn ( PPO_VGIn1Adr [Id] );
  #else
  VReadVal = inb ( PPO_VGIn1Adr [Id] );
  #endif
  
  return (VReadVal);
}

// 13/10/2011
// 27/05/2014 - MS bug removed :
// the read back value is the actual physical state of the ctrl register, and Strobe Autofeed and SelectIn signals are in negative logic.
// the negative logic signals had to be inverted

SInt32 PPO_FReadBackCtrl ( SInt8 Id ) {
  
  SInt32 VReadVal;
  
  if ( PPO_VGPortReady[Id] == 0 ) {
    err_retfail ( -1, (ERR_OUT,"Port Id=%d not ready", Id ) );
  }
  
  
  if ( PPO_VGNoHwAccess[Id] ) {
    err_warning (( ERR_OUT, "Debug mode => No HW access !" ));
    return (0);
  }
  
  #ifdef CC_APP_WINDOWS
  VReadVal = PPO_PRIV_FWinIn ( PPO_VGCtrlAdr [Id] );
  #else
  VReadVal = inb ( PPO_VGCtrlAdr [Id] );
  #endif

  // inversion of the Strobe Autofeed and SelectIn signals 
	//                  Strobe neg logic  LSB
	//            Autofeed Neg logic
	//       Init 
	//  SelectIn neg logic                MSB
	//  1    0    1    1  = 0xB
   // err_warning (( ERR_OUT, "VReadVal (before mask): %X" ,VReadVal));
	   
  //VReadVal = VReadVal ^ 0xb;
   // err_warning (( ERR_OUT, "VReadVal (after mask) : %X" ,VReadVal));
  
  return (VReadVal);
}






SInt32 PPO_FOutD0 ( SInt8 Id, UInt8 State ) {
  
  PPO_PRIV_FReadDataOutPortBeforeWrite ( Id );
  
  if ( State ) {
    PPO_VGOutData[Id] = PPO_VGOutData[Id] | 0x01;
  }
  
  else {
    PPO_VGOutData[Id] = PPO_VGOutData[Id] & 0xFE;
  }
  
  return ( PPO_PRIV_FOut (Id) );
}


SInt32 PPO_FOutD1 ( SInt8 Id, UInt8 State ) {
  
  PPO_PRIV_FReadDataOutPortBeforeWrite ( Id );
  
  if ( State ) {
    PPO_VGOutData[Id] = PPO_VGOutData[Id] | 0x02;
  }
  
  else {
    PPO_VGOutData[Id] = PPO_VGOutData[Id] & 0xFD;
  }
  
  return ( PPO_PRIV_FOut (Id) );
}

/* 05/02/2004 */

SInt32 PPO_FOutD2 ( SInt8 Id, UInt8 State ) {
  
  PPO_PRIV_FReadDataOutPortBeforeWrite ( Id );
  
  if ( State ) {
    PPO_VGOutData[Id] = PPO_VGOutData[Id] | 0x04;
  }
  
  else {
    PPO_VGOutData[Id] = PPO_VGOutData[Id] & 0xFB;
  }
  
  return ( PPO_PRIV_FOut (Id) );
}

/* 05/02/2004 */

SInt32 PPO_FOutD3 ( SInt8 Id, UInt8 State ) {
  
  PPO_PRIV_FReadDataOutPortBeforeWrite ( Id );
  
  if ( State ) {
    PPO_VGOutData[Id] = PPO_VGOutData[Id] | 0x08;
  }
  
  else {
    PPO_VGOutData[Id] = PPO_VGOutData[Id] & 0xF7;
  }
  
  return ( PPO_PRIV_FOut (Id) );
}

/* 05/02/2004 */

SInt32 PPO_FOutD4 ( SInt8 Id, UInt8 State ) {
  
  PPO_PRIV_FReadDataOutPortBeforeWrite ( Id );
  
  if ( State ) {
    PPO_VGOutData[Id] = PPO_VGOutData[Id] | 0x10;
  }
  
  else {
    PPO_VGOutData[Id] = PPO_VGOutData[Id] & 0xEF;
  }
  
  return ( PPO_PRIV_FOut (Id) );
}

/* 02/06/2004 */

SInt32 PPO_FOutD5 ( SInt8 Id, UInt8 State ) {
  
  PPO_PRIV_FReadDataOutPortBeforeWrite ( Id );
  
  if ( State ) {
    PPO_VGOutData[Id] = PPO_VGOutData[Id] | 0x20;
  }
  
  else {
    PPO_VGOutData[Id] = PPO_VGOutData[Id] & 0xDF;
  }
  
  return ( PPO_PRIV_FOut (Id) );
}

/* 02/06/2004 */

SInt32 PPO_FOutD6 ( SInt8 Id, UInt8 State ) {

  PPO_PRIV_FReadDataOutPortBeforeWrite ( Id );
  
  if ( State ) {
    PPO_VGOutData[Id] = PPO_VGOutData[Id] | 0x40;
  }
  
  else {
    PPO_VGOutData[Id] = PPO_VGOutData[Id] & 0xBF;
  }
  
  return ( PPO_PRIV_FOut (Id) );
}


/* 05/02/2004 */

SInt32 PPO_FOutD7 ( SInt8 Id, UInt8 State ) {
  
  PPO_PRIV_FReadDataOutPortBeforeWrite ( Id );
  
  // msg (( MSG_OUT, "PPO_FOutD7 ( Id=%d, State=%d )", Id, State ));
  
  if ( State ) {
    PPO_VGOutData[Id] = PPO_VGOutData[Id] | 0x80;
  }
  
  else {
    PPO_VGOutData[Id] = PPO_VGOutData[Id] & 0x7F;
  }
  
  return ( PPO_PRIV_FOut (Id) );
}


SInt32 PPO_FInAck ( SInt8 Id ) {
  
  SInt32 VIn;
  
  VIn = PPO_FIn ( Id );
  
  if ( VIn < 0 ) {
    err_error (( ERR_OUT, "Port Id=%d is not ready", Id ));
    return (0);
  }
  
  if ( VIn  & 0x40 ) {
    return (1);
  }
  else {
    return (0);
  }
  
}

/* 02/07/2005 */

SInt32 PPO_FInSel ( SInt8 Id ) {
  
  SInt32 VIn;
  
  VIn = PPO_FIn ( Id );
  
  if ( VIn < 0 ) {
    err_error (( ERR_OUT, "Port Id=%d is not ready", Id ));
    return (0);
  }
  
  if ( VIn  & 0x10 ) {
    return (1);
  }
  else {
    return (0);
  }
  
}


/* 10/02/2007 */

SInt32 PPO_FInError ( SInt8 Id ) {
  
  SInt32 VIn;
  
  VIn = PPO_FIn ( Id );
  
  if ( VIn < 0 ) {
    err_error (( ERR_OUT, "Port Id=%d is not ready", Id ));
    return (0);
  }
  
  if ( VIn  & 0x08 ) {
    return (1);
  }
  else {
    return (0);
  }
  
}


/* 10/02/2007 */

SInt32 PPO_FInPE ( SInt8 Id ) {
  
  SInt32 VIn;
  
  VIn = PPO_FIn ( Id );
  
  if ( VIn < 0 ) {
    err_error (( ERR_OUT, "Port Id=%d is not ready", Id ));
    return (0);
  }
  
  if ( VIn  & 0x20 ) {
    return (1);
  }
  else {
    return (0);
  }
  
}


/* 10/02/2007 */

SInt32 PPO_FInBusy ( SInt8 Id ) {
  
  SInt32 VIn;
  
  VIn = PPO_FIn ( Id );
  
  if ( VIn < 0 ) {
    err_error (( ERR_OUT, "Port Id=%d is not ready", Id ));
    return (0);
  }
  
  if ( VIn  & 0x80 ) {
    return (0);
  }
  else {
    return (1);
  }
  
}


SInt32 PPO_FInAckRisingEdge ( SInt8 Id ) {
  
  UInt8 VCurState;
  static UInt8 VOldState;
  static UInt8 VFirstCall = 1;
  
  if ( VFirstCall ) {
    VFirstCall = 0;
    VOldState  = PPO_FInAck (Id);
    
    if ( VOldState < 0 ) {
      err_error (( ERR_OUT, "Port Id=%d is not ready", Id ));
      return (0);
    }
    
  }
  
  else {
    
    VCurState = PPO_FInAck (Id);
    
    if ( VCurState < 0 ) {
      err_error (( ERR_OUT, "Port Id=%d is not ready", Id ));
      return (0);
    }
    
    if ( (VOldState == 0) && (VCurState == 1) ) {
      VOldState = VCurState;
      return (1);
    }
    
    else {
      VOldState = VCurState;
      return (0);
    }
    
  }
   return(0);
  
}




SInt32 PPO_FInAckFallingEdge ( SInt8 Id ) {
  
  UInt8 VCurState;
  static UInt8 VOldState;
  static UInt8 VFirstCall = 1;
  
  if ( VFirstCall ) {
    VFirstCall = 0;
    VOldState  = PPO_FInAck (Id);
    
    if ( VOldState < 0 ) {
      err_error (( ERR_OUT, "Port Id=%d is not ready", Id ));
      return (0);
    }
    
  }
  
  else {
    
    VCurState = PPO_FInAck (Id);
    
    if ( VCurState < 0 ) {
      err_error (( ERR_OUT, "Port Id=%d is not ready", Id ));
      return (0);
    }
    
    if ( (VOldState == 1) && (VCurState == 0) ) {
      VOldState = VCurState;
      return (1);
    }
    
    else {
      VOldState = VCurState;
      return (0);
    }
    
  }
 return(0); 
  
}

/* 02/07/2005 */

SInt32 PPO_FInSelRisingEdge ( SInt8 Id ) {
  
  UInt8  VTryCnt;
  UInt8  VCurState;
  static UInt8 VOldState;
  static UInt8 VFirstCall = 1;
  
  if ( VFirstCall ) {
    VFirstCall = 0;
    VOldState  = PPO_FInSel (Id);
    
    if ( VOldState < 0 ) {
      err_error (( ERR_OUT, "Port Id=%d is not ready", Id ));
      return (0);
    }
    
  }
  
  else {
    
    VCurState = PPO_FInSel (Id);
    
    /* To avoid glitch => 1 is checked 10 times */
    
    if ( VCurState == 1 ) {
      
      for ( VTryCnt=0; VTryCnt < 10; VTryCnt++ ) {
        VCurState = PPO_FInSel (Id);
        
        if ( VCurState < 0 ) {
          err_error (( ERR_OUT, "Port Id=%d is not ready", Id ));
          return (0);
        }
        
        if ( VCurState == 0 ) {
          break;
        }
        
      } /* End for */
      
    }
    
    if ( (VOldState == 0) && (VCurState == 1) ) {
      VOldState = VCurState;
      return (1);
    }
    
    else {
      VOldState = VCurState;
      return (0);
    }
    
  }
  return(0);
  
}



/* 03/03/2010 */

SInt32 PPO_FInPERisingEdge ( SInt8 Id ) {
  
  UInt8  VTryCnt;
  UInt8  VCurState;
  static UInt8 VOldState;
  static UInt8 VFirstCall = 1;
  
  if ( VFirstCall ) {
    VFirstCall = 0;
    VOldState  = PPO_FInPE (Id);
    
    if ( VOldState < 0 ) {
      err_error (( ERR_OUT, "Port Id=%d is not ready", Id ));
      return (0);
    }
    
  }
  
  else {
    
    VCurState = PPO_FInPE (Id);
    
    /* To avoid glitch => 1 is checked 10 times */
    
    if ( VCurState == 1 ) {
      
      for ( VTryCnt=0; VTryCnt < 10; VTryCnt++ ) {
        VCurState = PPO_FInPE (Id);
        
        if ( VCurState < 0 ) {
          err_error (( ERR_OUT, "Port Id=%d is not ready", Id ));
          return (0);
        }
        
        if ( VCurState == 0 ) {
          break;
        }
        
      } /* End for */
      
    }
    
    if ( (VOldState == 0) && (VCurState == 1) ) {
      VOldState = VCurState;
      return (1);
    }
    
    else {
      VOldState = VCurState;
      return (0);
    }
    
  }
 return(0); 
  
}



/* 10/02/2007 */

SInt32 PPO_FGetDataOut ( SInt8 Id ) {
  
  return ( PPO_VGOutData[Id] );
}


/* 10/02/2007 */

SInt32 PPO_FGetCtrlOut ( SInt8 Id ) {
  
  return ( PPO_VGCtrlData[Id] );
}


/* 10/02/2007 */
// modified the 27/05/2014 - MS
// inverted the level of the output, because this signal has negative logic

SInt32 PPO_FCtrlStrobe ( SInt8 Id, UInt8 State ) {
  
  PPO_VGCtrlData[Id] = PPO_FReadBackCtrl ( Id  );

  if ( State == 0 ) {
    PPO_VGCtrlData[Id] = PPO_VGCtrlData[Id] | 0x01;
  }
  
  else {
    PPO_VGCtrlData[Id] = PPO_VGCtrlData[Id] & 0xFE;
  }
  
  return ( PPO_PRIV_FCtrl (Id) );
}


/* 10/02/2007 */
// modified the 27/05/2014 - MS
// inverted the level of the output, because this signal has negative logic

SInt32 PPO_FCtrlAutoFeed ( SInt8 Id, UInt8 State ) {
  
  PPO_VGCtrlData[Id] = PPO_FReadBackCtrl ( Id  );
  
  if ( State == 0 ) {
    PPO_VGCtrlData[Id] = PPO_VGCtrlData[Id] | 0x02;
  }
  
  else {
    PPO_VGCtrlData[Id] = PPO_VGCtrlData[Id] & 0xFD;
  }
  
  return ( PPO_PRIV_FCtrl (Id) );
}


/* 10/02/2007 */

SInt32 PPO_FCtrlInit ( SInt8 Id, UInt8 State ) {
  
  PPO_VGCtrlData[Id] = PPO_FReadBackCtrl ( Id  );
  
  if ( State == 1 ) {
    PPO_VGCtrlData[Id] = PPO_VGCtrlData[Id] | 0x04;
  }
  
  else {
    PPO_VGCtrlData[Id] = PPO_VGCtrlData[Id] & 0xFB;
  }
  
  return ( PPO_PRIV_FCtrl (Id) );
}

/* 10/02/2007 */
// modified the 27/05/2014 - MS
// inverted the level of the output, because this signal has negative logic

SInt32 PPO_FCtrlSelect ( SInt8 Id, UInt8 State ) {
  
  PPO_VGCtrlData[Id] = PPO_FReadBackCtrl ( Id  );
  
  if ( State == 0 ) {
    PPO_VGCtrlData[Id] = PPO_VGCtrlData[Id] | 0x08;
  }
  
  else {
    PPO_VGCtrlData[Id] = PPO_VGCtrlData[Id] & 0xF7;
  }
  
  return ( PPO_PRIV_FCtrl (Id) );
}

// 13/10/2011

SInt32 PPO_FReadBackCtrlStrobe ( SInt8 Id ) {
  
  SInt32 VIn;
  
  VIn = PPO_FReadBackCtrl ( Id  );
  
  if ( VIn < 0 ) {
    err_error (( ERR_OUT, "Port Id=%d is not ready", Id ));
    return (0);
  }
  
  if ( VIn  & 0x01 ) {
    return (1);
  }
  else {
    return (0);
  }
  
}

// 16/10/2011

SInt32 PPO_FReadBackCtrlAutoFeed ( SInt8 Id ) {
  
  SInt32 VIn;
  
  VIn = PPO_FReadBackCtrl ( Id  );
  
  if ( VIn < 0 ) {
    err_error (( ERR_OUT, "Port Id=%d is not ready", Id ));
    return (0);
  }
  
  if ( VIn  & 0x02 ) {
    return (1);
  }
  else {
    return (0);
  }
  
}

// 16/10/2011

SInt32 PPO_FReadBackCtrlInit ( SInt8 Id ) {
  
  SInt32 VIn;
  
  VIn = PPO_FReadBackCtrl ( Id  );
  
  if ( VIn < 0 ) {
    err_error (( ERR_OUT, "Port Id=%d is not ready", Id ));
    return (0);
  }
  
  if ( VIn  & 0x04 ) {
    return (1);
  }
  else {
    return (0);
  }
  
}

// 16/10/2011

SInt32 PPO_FReadBackCtrlSelect ( SInt8 Id ) {
  
  SInt32 VIn;
  
  VIn = PPO_FReadBackCtrl ( Id  );
  
  if ( VIn < 0 ) {
    err_error (( ERR_OUT, "Port Id=%d is not ready", Id ));
    return (0);
  }
  
  if ( VIn  & 0x08 ) {
    return (1);
  }
  else {
    return (0);
  }
  
}




/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : UInt32 PPO_SPIPol (SInt8 Id, UInt32 Out_Data)
          :
Goal      : Send and receive one serial data
          :
Inputs    :  
          :
Ouputs    : 
          :
Globals   : 
          :
Remark    :
          :
Level     : 
Date      : 10/06/2008
Doc date  : //
Author    : Matthieu SPECHT
E-mail    : matthieu.specht@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

UInt32 PPO_SPIPol (SInt8 Id, UInt32 Out_Data, SInt8* PtDesync, SInt8 SpiEcho ) {
  UByte i;
  UInt32 Spi_Data;
  UInt32 Spi_Mask;
  SInt32 VFin;
  UInt8 VFout;
   
  Spi_Mask = 0x80000000;
  Spi_Data = 0;
    
  PPO_FOutD1( Id, 1/* Value */);// Enable Chip Select for the SPI interface : Start of transmission
  //PPO_WAIT (PPO_WT);
  VFout = PPO_VGOutData[Id];
  for ( i = 1 ; i <= 32 ; i++ ){
    
    VFout |= 0x04;  /*  Data bus :   0 0 0 0 0 1 1 0*/
                  /*                       D7 . . . . D3  = 0*/
                  /*                                         D2 = 1    : Rising Edge on SCLK*/
                  /*                                           D1 = 1 : Chip Select for the SPI Interface still active  */
                  /*                                               D0 = 0 */
    if (i > 1 ){
      if (SpiEcho){
        if ((Spi_Data & (Spi_Mask<<1))!=0)/*((Out_Data & Spi_Mask) !=0)*/{
          VFout |= 0x08;
        }
        else{
          VFout &= 0xF7;
        }
      }
      else{
        if ((Out_Data & Spi_Mask) !=0){
          VFout |= 0x08;
        }
        else{
          VFout &= 0xF7;
        }     
      }
    }/*End if*/
    PPO_FOut (Id, VFout);
    PPO_FOutD2 (Id, 0); 		/*  SCLK falling edge : retrieve data from bus*/
    VFin = PPO_FIn (Id);
    if (VFin & 0x08) {  
      Spi_Data += Spi_Mask;
    } /* end if */
    Spi_Mask = Spi_Mask >> 1; 
  } /* End For */
 
  
  PPO_FOut (Id, VFout & 0xF1); // release the spi interface : reset SCLK, reset SDIN, reset SSEL

  if ( PtDesync != NULL ) {
    *PtDesync = (Spi_Data & 1);
  }

  return ( (Spi_Data >> 1) & 0x7FFFFFFF);
  
}/* End PPO_SPIPol (SInt8 Id, UInt32 Out_Data)*/


/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : SInt32 PPO_TLU_FReadCnt (SInt8 Id, SInt8 TryNb, UInt32 Out_Data, SInt8* PtDesync, SInt8 SpiEcho )
:
Goal      : Send and receive one serial data
:
Inputs    :
:
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 10/06/2008
Doc date  : //
Author    : CLAUS Gilles
E-mail    : gilles.claus@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

UInt32 PPO_TLU_FReadCnt (SInt8 PortId, SInt8 TryNb, UInt32 Out_Data, SInt8* PtDesync, SInt8 SpiEcho ) {
  
  SInt8 Vi;
  UInt32 VCnt;
  SInt8  VDesync;
  
  for ( Vi=0; Vi < TryNb; Vi++ ) {
    VCnt = PPO_SPIPol ( PortId, Out_Data, &VDesync, SpiEcho );
    
    if ( VCnt != 0 ) {
      break;
    }
  }
  
  if ( Vi > 0 ) {
    err_error (( ERR_OUT, "TLU readout error : %d try", Vi ));
  }
  
  if ( VCnt == 0 ) {
    err_error (( ERR_OUT, "TLU readout error : Read 0 after %d try !", TryNb ));
  }
  
  if ( VDesync != 0 ) {
    err_error (( ERR_OUT, "TLU desync !" ));
  }
  
  if ( PtDesync != NULL ) {
    *PtDesync = VDesync;
  }
  
  return (VCnt);
}
  

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : PPO_SPIAutoTrigger (SInt8 Id,SInt8 Count)
          :
Goal      :  Generate Count number of trigger
          :
Inputs    :
          :
Ouputs    : 
          :
Globals   : 
          :
Remark    :
          :
Level     : 
Date      : 02/07/2008
Doc date  : //
Author    : Matthieu SPECHT
E-mail    : matthieu.specht@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

void PPO_SPIAutoTrigger (SInt8 Id,SInt32 Count) {
  SInt32 i;
  
  for (i=0;i<Count;i++){
    PPO_FOutD0( Id, 1/*0 *//* Value */);// Set Trigger signal
    //PPO_WAIT (10);
    PPO_FOutD0( Id, 0 /* Value */);// Reset Trigger signal
  } // End For
  
  //return ( NULL );
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : PPO_SPIReset (SInt8 Id)
          :
Goal      :  Generate a Reset 
          :
Inputs    :
          :
Ouputs    : 
          :
Globals   : 
          :
Remark    :
          :
Level     : 
Date      : 03/07/2008
Doc date  : //
Author    : Matthieu SPECHT
E-mail    : matthieu.specht@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

void PPO_SPIReset (SInt8 Id) {

  SInt8 ViDelay;

  PPO_FOutD4( Id, 1/*0 *//* Value */); // Set Reset signal
  
  // Delay 10 us
  
  for ( ViDelay=0; ViDelay < 10; ViDelay++ ) {
    PPO_FOutD4( Id, 1/*0 *//* Value */);
  }
  
  PPO_FOutD4( Id, 0 /* Value */);      // Reset Reset signal
  
}

/* DOC_FUNC_BEGIN */
/* ===================================================================================
Prototype : PPO_SPITrigger (SInt8 Id)
          :
Goal      :  Generate a trigger
          :
Inputs    :
          :
Ouputs    : 
          :
Globals   : 
          :
Remark    :
          :
Level     : 
Date      : 03/07/2008
Doc date  : //
Author    : Matthieu SPECHT
E-mail    : matthieu.specht@ires.in2p3.fr
Labo      : IPHC */
/* =================================================================================== */
/* DOC_FUNC_END */

void PPO_SPITrigger (SInt8 Id) {
  
  SInt8 ViDelay;
  
  PPO_FOutD0( Id, 1/*0 *//* Value */);// Set Reset signal
  
  // Delay 10 us
  
  for ( ViDelay=0; ViDelay < 10; ViDelay++ ) {
    PPO_FOutD0( Id, 1/*0 *//* Value */);// 1 us Delay
  }
  
  PPO_FOutD0( Id, 0 /* Value */);// Reset Reset signal
    
}
// err_retfail ( Code, (ERR_OUT,"MsgFail",par) );
// err_error (( ERR_OUT, "", par ));

#endif