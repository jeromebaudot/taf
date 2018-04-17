
/*******************************************************************************
File      : x:\lib\com\math.c
Goal      : Functions of
Prj date  : //200
File date : //200
Doc date  : //200
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
----------------------------------------------------------------------------------
License   : You are free to use this source files for your own development as long
: as it stays in a public research context. You are not allowed to use it
: for commercial purpose. You must put this header with laboratory and
: authors names in all development based on this library.
----------------------------------------------------------------------------------
Labo      : IPHC */
/*******************************************************************************/


#ifndef MATH_C
#define MATH_C


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
Date      : 21/02/2008
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 MATH_FSInt32Ceil ( SInt32 Src ) {
  return ( ceil ( (double) Src ) );
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
Date      : 21/02/2008
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

float MATH_FFloatPow ( float x, float y ) {
  return ( pow ( x, y ) );
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


/* 03/05/04 */

SInt32 MATH_FCpySInt32ToSInt16Array ( SInt32* PtSrc, SInt32 SrcEltSz, SInt16* PtDest, SInt32 DestEltSz ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  
  if ( SrcEltSz > DestEltSz ) {
    err_retfail ( -1, (ERR_OUT,"SrcEltSz=%d > DestEltSz=%d", SrcEltSz, DestEltSz) );
  }
  
  for ( Vi=0; Vi < SrcEltSz; Vi++ ) {
    PtDest[Vi] = PtSrc[Vi];
  }
  
}

// 14/08/2007

float MATH_FUInt8Avg ( UInt8* Src, SInt32 EltNb ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32 Vi;
  double VSum;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VSum = 0;
  
  for ( Vi=0; Vi < EltNb; Vi++) {
    VSum = VSum + Src[Vi];
  }
  
  return ( VSum / (float) EltNb );
}


float MATH_FSInt8Avg ( SInt8* Src, SInt32 EltNb ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32 Vi;
  double VSum;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VSum = 0;
  
  for ( Vi=0; Vi < EltNb; Vi++) {
    VSum = VSum + Src[Vi];
  }
  
  return ( VSum / (float) EltNb );
}


float MATH_FUInt16Avg ( UInt16* Src, SInt32 EltNb ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  double VSum;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VSum = 0;
  
  for ( Vi=0; Vi < EltNb; Vi++) {
    VSum = VSum + Src[Vi];
  }
  
  return ( VSum / (float) EltNb );
}


float MATH_FSInt16Avg ( SInt16* Src, SInt32 EltNb ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  double VSum;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VSum = 0;
  
  for ( Vi=0; Vi < EltNb; Vi++) {
    VSum = VSum + Src[Vi];
  }
  
  return ( VSum / (float) EltNb );
}


// 09/11/2010
// Reject item == 0 from average calculation

float MATH_FSInt16AvgNonZero ( SInt16* Src, SInt32 EltNb ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32 Vi;
  SInt32 VGoodEltCnt;
  double VSum;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VSum        = 0;
  VGoodEltCnt = 0;
  
  for ( Vi=0; Vi < EltNb; Vi++) {
    
    if ( Src[Vi] != 0 ) {
      VSum = VSum + Src[Vi];
      ++VGoodEltCnt;
    }
    
  }
  
  if ( VGoodEltCnt == 0 ) {
    err_warning (( ERR_OUT, "No elt <> 0 to calculate average !" ));
    return (0);
  }
  
  return ( VSum / (float) VGoodEltCnt );
}



/* 21/06/2006 */
/* Calculates the mean value of Src array selected elements                      */
/* SelList[i] == 1 if the corresponding item must be used for processing */
  
float MATH_FSInt16DataListAvg ( SInt16* Src, SInt8* SelList, SInt32 EltNb ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  SInt32 VEltCnt;
  double VSum;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  err_retnull ( SelList, (ERR_OUT,"SelList == NULL") );
  
  VEltCnt = 0;
  VSum    = 0;
  
  for ( Vi=0; Vi < EltNb; Vi++) {
    
    if ( SelList[Vi] != 1 ) {
      continue;
    }
    
    ++VEltCnt;
    
    VSum = VSum + Src[Vi];
  }
  
  return ( VSum / (float) VEltCnt );
}

// 14/08/2007

float MATH_FUInt32Avg ( UInt32* Src, SInt32 EltNb ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32 Vi;
  double  VSum;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VSum = 0;
  
  for ( Vi=0; Vi < EltNb; Vi++) {
    VSum = VSum + Src[Vi];
  }
  
  return ( VSum / (float) EltNb );
}



float MATH_FSInt32Avg ( SInt32* Src, SInt32 EltNb ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  double  VSum;

  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VSum = 0;
  
  for ( Vi=0; Vi < EltNb; Vi++) {
    VSum = VSum + Src[Vi];
  }
  
  return ( VSum / (float) EltNb );
}

// 21/05/2010
// First call must be done with StartSearch = 1 and Src of this call IS NOT processed
// => Processing starts on next call
// Calculation is done by setting DoCalc at 1
// - current value of Src is not processed => use previous list of values


float MATH_FSInt32AvgNo1 ( SInt32 Src, SInt8 StartSearch, SInt8 DoCalc ) {
  
  static SInt64 VSum;
  static SInt32 VEltCnt;
         float  VAvg = 0;
  
  if ( StartSearch == 1 ) {
    VEltCnt = 0;
    VSum    = 0;
    return (0);
  }

  if ( DoCalc == 1 ) {

    if ( VEltCnt <= 0 ) {
      err_retfail ( -1, (ERR_OUT,"Calculation not possible => bad elt nb = %d", VEltCnt) );
    }

    VAvg = (float) VSum / (float) VEltCnt;
    err_retval ( VAvg, ( ERR_OUT, "Average of %d value = %.3f", VEltCnt, VAvg ) );
  }
  
  VSum += Src;
  ++VEltCnt;
  
  
  return (0);  
}


// 21/05/2010
// Calculation is done by setting DoCalc at 1
// - current value of Src is added to sum before calculation
// - if one wants to calculate average "later", set Src=0 and DoCalc=1


float MATH_FSInt32AvgNo2 ( SInt32 Src, SInt8 StartSearch, SInt8 DoCalc ) {
  
  static SInt64 VSum;
  static SInt32 VEltCnt;
  float  VAvg = 0;
  
  if ( StartSearch == 1 ) {
    VEltCnt = 0;
    VSum    = 0;
  }

  if ( DoCalc == 1 ) {

    if ( VEltCnt <= 0 ) {
      err_retfail ( -1, (ERR_OUT,"Calculation not possible => bad elt nb = %d", VEltCnt) );
    }

    VAvg = (float) VSum / (float) VEltCnt;
    err_retval ( VAvg, ( ERR_OUT, "Average of %d value = %.3f", VEltCnt, VAvg ) );
  }

  VSum += Src;
  ++VEltCnt;
  
  return (0);
}


// 11/08/2007

SInt32 MATH_FFloatMinMaxAvg ( float* Src, SInt32 EltNb, float* PtResMin, float* PtResMax, float* PtResAvg ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32 Vi;
  float  VMin;
  float  VMax;
  double VSum;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  err_retnull ( PtResMin, (ERR_OUT,"PtResMin == NULL") );
  err_retnull ( PtResMax, (ERR_OUT,"PtResMax == NULL") );
  err_retnull ( PtResAvg, (ERR_OUT,"PtResAvg == NULL") );
  
  VSum = 0;
  VMin = Src[0];
  VMax = Src[0];
  
  for ( Vi=0; Vi < EltNb; Vi++) {
    
    if ( Src[Vi] < VMin ) {
      VMin = Src[Vi];
    }

    if ( Src[Vi] > VMax ) {
      VMax = Src[Vi];
    }
    
    VSum = VSum + Src[Vi];
  }
  
  *PtResMin = VMin;
  *PtResMax = VMax;
  *PtResAvg = ( VSum / (float) EltNb );
  
  return (0);
}


float MATH_FFloatAvg ( float* Src, SInt32 EltNb ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  double VSum;

  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VSum = 0;
  
  for ( Vi=0; Vi < EltNb; Vi++) {
    VSum = VSum + Src[Vi];
  }
  
  return ( VSum / (float) EltNb );
}


// 19/01/2012
// First call must be done with StartSearch = 1 and Src of this call IS NOT processed
// => Processing starts on next call
// Calculation is done by setting DoCalc at 1
// - current value of Src is not processed => use previous list of values


float MATH_FFloatAvgNo1 ( float Src, SInt8 StartSearch, SInt8 DoCalc ) {
  
  static double VSum;
  static SInt32 VEltCnt;
  float  VAvg = 0;
  
  if ( StartSearch == 1 ) {
    VEltCnt = 0;
    VSum    = 0;
    return (0);
  }
  
  if ( DoCalc == 1 ) {
    
    if ( VEltCnt <= 0 ) {
      err_retfail ( -1, (ERR_OUT,"Calculation not possible => bad elt nb = %d", VEltCnt) );
    }
    
    VAvg = (float) VSum / (float) VEltCnt;
    err_retval ( VAvg, ( ERR_OUT, "Average of %d value = %.3f", VEltCnt, VAvg ) );
  }
  
  VSum += Src;
  ++VEltCnt;
  
  
  return (0);
}


// 19/01/2012
// First call must be done with StartSearch = 1 and Src of this call IS NOT processed
// => Processing starts on next call
// Calculation is done by setting DoCalc at 1
// - current value of Src is not processed => use previous list of values


float MATH_FFloatAvgNo2 ( float Src, SInt8 StartSearch, SInt8 DoCalc ) {
  
  static double VSum;
  static SInt32 VEltCnt;
  float  VAvg = 0;
  
  if ( StartSearch == 1 ) {
    VEltCnt = 0;
    VSum    = 0;
    return (0);
  }
  
  if ( DoCalc == 1 ) {
    
    if ( VEltCnt <= 0 ) {
      err_retfail ( -1, (ERR_OUT,"Calculation not possible => bad elt nb = %d", VEltCnt) );
    }
    
    VAvg = (float) VSum / (float) VEltCnt;
    err_retval ( VAvg, ( ERR_OUT, "Average of %d value = %.3f", VEltCnt, VAvg ) );
  }
  
  VSum += Src;
  ++VEltCnt;
  
  
  return (0);
}



// 19/01/2012
// First call must be done with StartSearch = 1 and Src of this call IS NOT processed
// => Processing starts on next call
// The mean value = Avg parameters MUST be given on first call !
// Calculation is done by setting DoCalc at 1
// - current value of Src is not processed => use previous list of values

float MATH_FFloatSigma1No1 ( float Src, float Avg, SInt8 StartSearch, SInt8 DoCalc ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  static SInt32 VEltCnt;
  static float  VAvg;
  static double VPrevSum2;
  static double VSum2;
  
  
  // Init
  
  if ( StartSearch == 1 ) {
    VEltCnt   = 0;
    VAvg      = Avg;
    VSum2     = 0;
    VPrevSum2 = 0;
    return (0);
  }
  
  // Calculates sigma
  
  if ( DoCalc == 1 ) {
    
    if ( VEltCnt <= 0 ) {
      err_error (( ERR_OUT, "Bad elt counter = %d", VEltCnt ));
      return (-1);
    }
    
    return ( sqrt ( VSum2 / (float) VEltCnt ) );
  }
  
  
  // Process data
  
  VSum2 = VSum2 + ( (Src - VAvg) * (Src - VAvg) );
  
  if ( VSum2 < VPrevSum2 ) {
    err_error (( ERR_OUT, "VSum2 overflow !" ));
    return (-1);
  }
  
  VPrevSum2 = VSum2;
  
  ++VEltCnt;
  
  return (0);
}


// 19/01/2012
// First call must be done with StartSearch = 1 and Src of this call IS NOT processed
// => Processing starts on next call
// The mean value = Avg parameters MUST be given on first call !
// Calculation is done by setting DoCalc at 1
// - current value of Src is not processed => use previous list of values

float MATH_FFloatSigma1No2 ( float Src, float Avg, SInt8 StartSearch, SInt8 DoCalc ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  static SInt32 VEltCnt;
  static float  VAvg;
  static double VPrevSum2;
  static double VSum2;
  
  
  // Init
  
  if ( StartSearch == 1 ) {
    VEltCnt   = 0;
    VAvg      = Avg;
    VSum2     = 0;
    VPrevSum2 = 0;
    return (0);
  }
  
  // Calculates sigma
  
  if ( DoCalc == 1 ) {
    
    if ( VEltCnt <= 0 ) {
      err_error (( ERR_OUT, "Bad elt counter = %d", VEltCnt ));
      return (-1);
    }
    
    return ( sqrt ( VSum2 / (float) VEltCnt ) );
  }
  
  
  // Process data
  
  VSum2 = VSum2 + ( (Src - VAvg) * (Src - VAvg) );
  
  if ( VSum2 < VPrevSum2 ) {
    err_error (( ERR_OUT, "VSum2 overflow !" ));
    return (-1);
  }
  
  VPrevSum2 = VSum2;
  
  ++VEltCnt;
  
  return (0);
}



float MATH_FFloatMinAndMinIndex ( float* Src, SInt32 EltNb, SInt32* PtMinIndex ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  SInt32 VMinIndex;
  float  VMin;

  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VMinIndex = -1;
  VMin      = Src[0];
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    if ( Src[Vi] < VMin ) {
      VMin      = Src[Vi];
      VMinIndex = Vi;
    }
  }
  
  if ( PtMinIndex != NULL ) {
    *PtMinIndex = VMinIndex;
  }
  
  return (VMin);
}

float MATH_FFloatMin ( float* Src, SInt32 EltNb ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32 Vi;
  float  VMin;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VMin = Src[0];
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    if ( Src[Vi] < VMin ) {
      VMin = Src[Vi];
    }
  }
  
  return (VMin);
}


float MATH_FFloatMaxAndMaxIndex ( float* Src, SInt32 EltNb, SInt32* PtMaxIndex ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32 Vi;
  SInt32 VMaxIndex;
  float  VMax;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VMaxIndex = -1;
  VMax      = Src[0];
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    if ( Src[Vi] > VMax ) {
      VMax      = Src[Vi];
      VMaxIndex = Vi;
    }
  }
  
  if ( PtMaxIndex != NULL ) {
    *PtMaxIndex = VMaxIndex;
  }
  
  return (VMax);
}


float MATH_FFloatMax ( float* Src, SInt32 EltNb ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  float  VMax;

  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VMax = Src[0];
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    if ( Src[Vi] > VMax ) {
      VMax = Src[Vi];
    }
  }
  
  return (VMax);
}


// 19/01/2012
// First call must be done with StartSearch = 1 and Src of this call IS NOT processed
// => Processing starts on next call

float MATH_FFloatMinNo1 ( float Src, SInt8 StartSearch ) {
  
  static SInt8 VGetFirstElt = 0;
  static float VMin;
  
  if ( StartSearch == 1 ) {
    VGetFirstElt = 1;
    return (0);
  }
  
  if ( VGetFirstElt == 1 ) {
    VGetFirstElt = 0;
    VMin = Src;
    return (VMin);
  }
  
  if ( Src < VMin ) {
    VMin = Src;
  }
  
  return (VMin);
}


// 19/01/2012
// First call must be done with StartSearch = 1 and Src of this call IS NOT processed
// => Processing starts on next call

float MATH_FFloatMinNo2 ( float Src, SInt8 StartSearch ) {
  
  static SInt8 VGetFirstElt = 0;
  static float VMin;
  
  if ( StartSearch == 1 ) {
    VGetFirstElt = 1;
    return (0);
  }
  
  if ( VGetFirstElt == 1 ) {
    VGetFirstElt = 0;
    VMin = Src;
    return (VMin);
  }
  
  if ( Src < VMin ) {
    VMin = Src;
  }
  
  return (VMin);
}


// 19/01/2012
// First call must be done with StartSearch = 1 and Src of this call IS NOT processed
// => Processing starts on next call

float MATH_FFloatMaxNo1 ( float Src, SInt8 StartSearch ) {
  
  static SInt8 VGetFirstElt = 0;
  static float VMax;
  
  if ( StartSearch == 1 ) {
    VGetFirstElt = 1;
    return (0);
  }
  
  if ( VGetFirstElt == 1 ) {
    VGetFirstElt = 0;
    VMax = Src;
    return (VMax);
  }
  
  if ( Src > VMax ) {
    VMax = Src;
  }
  
  return (VMax);
}

// 19/01/2012
// First call must be done with StartSearch = 1 and Src of this call IS NOT processed
// => Processing starts on next call

float MATH_FFloatMaxNo2 ( float Src, SInt8 StartSearch ) {
  
  static SInt8 VGetFirstElt = 0;
  static float VMax;
  
  if ( StartSearch == 1 ) {
    VGetFirstElt = 1;
    return (0);
  }
  
  if ( VGetFirstElt == 1 ) {
    VGetFirstElt = 0;
    VMax = Src;
    return (VMax);
  }
  
  if ( Src > VMax ) {
    VMax = Src;
  }
  
  return (VMax);
}



// 09/08/2007

SInt32 MATH_FUInt32Min ( UInt32* Src, SInt32 EltNb ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32 Vi;
  UInt32  VMin;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VMin = 0xFFFFFFFF;
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    if ( Src[Vi] < VMin ) {
      VMin = Src[Vi];
    }
  }
  
  return (VMin);
}



// 09/08/2007

SInt32 MATH_FUInt32Max ( UInt32* Src, SInt32 EltNb ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32 Vi;
  UInt32 VMax;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VMax = 0;
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    if ( Src[Vi] > VMax ) {
      VMax = Src[Vi];
    }
  }
  
  return (VMax);
}



SInt32 MATH_FSInt32Min ( SInt32* Src, SInt32 EltNb ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  SInt32  VMin;

  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VMin = 0x7FFFFFFF;
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    if ( Src[Vi] < VMin ) {
      VMin = Src[Vi];
    }
  }
  
  return (VMin);
}

// 21/05/2010
// First call must be done with StartSearch = 1 and Src of this call IS NOT processed
// => Processing starts on next call

SInt32 MATH_FSInt32MinNo1 ( SInt32 Src, SInt8 StartSearch ) {
  
  static SInt32 VMin;
  
  if ( StartSearch == 1 ) {
    VMin = TYP_MAX_SINT32;
    return (0);
  }
  
  if ( Src < VMin ) {
    VMin = Src;
  }
  
  return (VMin);
}

// 21/05/2010
// First call must be done with StartSearch = 1 and Src of this call IS NOT processed
// => Processing starts on next call

SInt32 MATH_FSInt32MinNo2 ( SInt32 Src, SInt8 StartSearch ) {
  
  static SInt32 VMin;
  
  if ( StartSearch == 1 ) {
    VMin = TYP_MAX_SINT32;
    return (0);
  }
  
  if ( Src < VMin ) {
    VMin = Src;
  }
  
  return (VMin);
}



SInt32 MATH_FSInt32Max ( SInt32* Src, SInt32 EltNb ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  SInt32 VMax;

  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VMax = ( ~ 0x7FFFFFFF );
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    if ( Src[Vi] > VMax ) {
      VMax = Src[Vi];
    }
  }
  
  return (VMax);
}

// 21/05/2010
// First call must be done with StartSearch = 1 and Src of this call IS NOT processed
// => Processing starts on next call

SInt32 MATH_FSInt32MaxNo1 ( SInt32 Src, SInt8 StartSearch ) {
  
  static SInt32 VMax;
  
  if ( StartSearch == 1 ) {
    VMax = TYP_MIN_SINT32;
    return (0);
  }
  
  if ( Src > VMax ) {
    VMax = Src;
  }
  
  return (VMax);
}

// 21/05/2010
// First call must be done with StartSearch = 1 and Src of this call IS NOT processed
// => Processing starts on next call

SInt32 MATH_FSInt32MaxNo2 ( SInt32 Src, SInt8 StartSearch ) {
  
  static SInt32 VMax;
  
  if ( StartSearch == 1 ) {
    VMax = TYP_MIN_SINT32;
    return (0);
  }
  
  if ( Src > VMax ) {
    VMax = Src;
  }
  
  return (VMax);
}


// 09/08/2007

SInt32 MATH_FUInt16Min ( UInt16* Src, SInt32 EltNb ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32 Vi;
  UInt32  VMin;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VMin = 65535;
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    if ( Src[Vi] < VMin ) {
      VMin = Src[Vi];
    }
  }
  
  return (VMin);
}

// 09/08/2007

SInt32 MATH_FUInt16Max ( UInt16* Src, SInt32 EltNb ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32 Vi;
  UInt32 VMax;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VMax = 0;
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    if ( Src[Vi] > VMax ) {
      VMax = Src[Vi];
    }
  }
  
  return (VMax);
}


/* -------------------- */

SInt32 MATH_FSInt16Min ( SInt16* Src, SInt32 EltNb ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  SInt32  VMin;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VMin = 32767;
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    if ( Src[Vi] < VMin ) {
      VMin = Src[Vi];
    }
  }
  
  return (VMin);
}

/* 20/06/2006 */
/* Calculates the min value Src array selected elements                      */
/* SelList[i] == 1 if the corresponding item must be used for min processing */

SInt32 MATH_FSInt16DataListMin ( SInt16* SrcData, SInt8* SelList, SInt32 EltNb ) {

#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  SInt32  VMin;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  err_retnull ( SelList, (ERR_OUT,"SelList == NULL") );
  
  VMin = 32767;
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    if ( (SelList[Vi] == 1) && (SrcData[Vi] < VMin) ) {
      VMin = SrcData[Vi];
    }
  }
  
  return (VMin);
}



SInt32 MATH_FSInt16Max ( SInt16* Src, SInt32 EltNb ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  SInt32 VMax;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VMax = -32768;
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    if ( Src[Vi] > VMax ) {
      VMax = Src[Vi];
    }
  }
  
  return (VMax);
}

/* 20/06/2006 */
/* Calculates the max value Src array selected elements                      */
/* SelList[i] == 1 if the corresponding item must be used for max processing */

SInt32 MATH_FSInt16DataListMax ( SInt16* SrcData, SInt8* SelList, SInt32 EltNb ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  SInt32 VMax;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }

  err_retnull ( SelList, (ERR_OUT,"SelList == NULL") );
  
  VMax = -32768;
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    if ( (SelList[Vi] == 1) && (SrcData[Vi] > VMax) ) {
      VMax = SrcData[Vi];
    }
  }
  
  return (VMax);
}







/* ---- */
// 12/04/2006 

UInt8 MATH_FUInt8Min ( UInt8* Src, SInt32 EltNb ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  UInt32  VMin;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VMin = 255;
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    if ( Src[Vi] < VMin ) {
      VMin = Src[Vi];
    }
  }
  
  return (VMin);
}


// 12/04/2006


UInt8 MATH_FUInt8Max ( UInt8* Src, SInt32 EltNb ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  UInt32 VMax;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VMax = 0;
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    if ( Src[Vi] > VMax ) {
      VMax = Src[Vi];
    }
  }
  
  return (VMax);
}


/* ---- */
// 07/08/2007

SInt8 MATH_FSInt8Min ( SInt8* Src, SInt32 EltNb ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32 Vi;
  SInt32  VMin;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VMin = 127;
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    if ( Src[Vi] < VMin ) {
      VMin = Src[Vi];
    }
  }
  
  return (VMin);
}


// 07/08/2007


SInt8 MATH_FSInt8Max ( SInt8* Src, SInt32 EltNb ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32 Vi;
  SInt32 VMax;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VMax = -128;
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    if ( Src[Vi] > VMax ) {
      VMax = Src[Vi];
    }
  }
  
  return (VMax);
}



float MATH_FSInt32Sigma1 ( SInt32* Src, SInt32 EltNb, float Avg ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  double  VPrevSum2;
  double  VSum2;
  double  VAvg2;

  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VSum2     = 0;
  VPrevSum2 = 0;
  VAvg2 = Avg * Avg;
  
  for ( Vi=0; Vi < EltNb; Vi++) {
    
    /* VSum2 = VSum2 + ( (Src[Vi] *Src[Vi]) - VAvg2 );   */
    VSum2 = VSum2 + ( (Src[Vi] - Avg) * (Src[Vi] - Avg) );
    
    if ( VSum2 < VPrevSum2 ) {
      return (-1);  
    }
    
    VPrevSum2 = VSum2;
    
  } /* End for */

  if ( VSum2 <= 0 ) {
    err_warning (( ERR_OUT, "Sigma calc error : Sum2=%.3f <= 0 => Sigma=0", VSum2 ));
    return (0);
  }
  
  return ( sqrt ( VSum2 / (float) EltNb ) );
}


// 19/01/2012
// First call must be done with StartSearch = 1 and Src of this call IS NOT processed
// => Processing starts on next call
// The mean value = Avg parameters MUST be given on first call !
// Calculation is done by setting DoCalc at 1
// - current value of Src is not processed => use previous list of values

float MATH_FSInt32Sigma1No1 ( SInt32 Src, float Avg, SInt8 StartSearch, SInt8 DoCalc ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  static SInt32 VEltCnt;
  static float  VAvg;
  static double VPrevSum2;
  static double VSum2;
  
  
  // Init
  
  if ( StartSearch == 1 ) {
    VEltCnt   = 0;
    VAvg      = Avg;
    VSum2     = 0;
    VPrevSum2 = 0;
    return (0);
  }
  
  // Calculates sigma
  
  if ( DoCalc == 1 ) {
    
    if ( VEltCnt <= 0 ) {
      err_error (( ERR_OUT, "Bad elt counter = %d", VEltCnt ));
      return (-1);
    }
    
    return ( sqrt ( VSum2 / (float) VEltCnt ) );
  }
  
  
  // Process data
  
  VSum2 = VSum2 + ( (Src - VAvg) * (Src - VAvg) );
  
  if ( VSum2 < VPrevSum2 ) {
    err_error (( ERR_OUT, "VSum2 overflow !" ));
    return (-1);
  }
  
  VPrevSum2 = VSum2;
  
  return (0);
}


float MATH_FSInt16Sigma1 ( SInt16* Src, SInt32 EltNb, float Avg ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  double  VPrevSum2;
  double  VSum2;
  double  VAvg2;
  SInt32 VOutOfRangeCnt;

  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VSum2 = 0;
  VPrevSum2 = 0;
  VAvg2 = Avg * Avg;
  
  VOutOfRangeCnt = 0;
  
  for ( Vi=0; Vi < EltNb; Vi++) {
    
    if ( ( Src[Vi] < (Avg-6) ) || (Src[Vi] > (Avg+6) ) ) {
      ++VOutOfRangeCnt;
    }
    
    VSum2 = VSum2 + ( (Src[Vi] - Avg) * (Src[Vi] - Avg) );
    
    if ( VSum2 < VPrevSum2 ) {
      return (-1);
    }
    
    VPrevSum2 = VSum2;    
    
  } /* End for */

  if ( VSum2 <= 0 ) {
    err_warning (( ERR_OUT, "Sigma calc error : Sum2=%.3f <= 0 => Sigma=0", VSum2 ));
    return (0);
  }
  
  return ( sqrt ( VSum2 / (float) EltNb ) );
}

// 14/08/2007

float MATH_FSInt8Sigma1 ( SInt8* Src, SInt32 EltNb, float Avg ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32 Vi;
  double  VPrevSum2;
  double  VSum2;
  double  VAvg2;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VSum2 = 0;
  VPrevSum2 = 0;
  VAvg2 = Avg * Avg;


  for ( Vi=0; Vi < EltNb; Vi++) {
    
    
    VSum2 = VSum2 + ( (Src[Vi] - Avg) * (Src[Vi] - Avg) );
    
    if ( VSum2 < VPrevSum2 ) {
      return (-1);
    }
    
    VPrevSum2 = VSum2;
    
  } /* End for */
  
  if ( VSum2 <= 0 ) {
    err_warning (( ERR_OUT, "Sigma calc error : Sum2=%.3f <= 0 => Sigma=0", VSum2 ));
    return (0);
  }
  
  return ( sqrt ( VSum2 / (float) EltNb ) );
}


// 14/08/2007

float MATH_FUInt32Sigma1 ( UInt32* Src, SInt32 EltNb, float Avg ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32 Vi;
  double  VPrevSum2;
  double  VSum2;
  double  VAvg2;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VSum2 = 0;
  VPrevSum2 = 0;
  VAvg2 = Avg * Avg;
  

  for ( Vi=0; Vi < EltNb; Vi++) {
    
    
    VSum2 = VSum2 + ( (Src[Vi] - Avg) * (Src[Vi] - Avg) );
    
    if ( VSum2 < VPrevSum2 ) {
      return (-1);
    }
    
    VPrevSum2 = VSum2;
    
  } /* End for */

  if ( VSum2 <= 0 ) {
    err_warning (( ERR_OUT, "Sigma calc error : Sum2=%.3f <= 0 => Sigma=0", VSum2 ));
    return (0);
  }
  
  return ( sqrt ( VSum2 / (float) EltNb ) );
}

// 14/08/2007

float MATH_FUInt16Sigma1 ( UInt16* Src, SInt32 EltNb, float Avg ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32 Vi;
  double  VPrevSum2;
  double  VSum2;
  double  VAvg2;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VSum2 = 0;
  VPrevSum2 = 0;
  VAvg2 = Avg * Avg;
  

  for ( Vi=0; Vi < EltNb; Vi++) {
    
    
    VSum2 = VSum2 + ( (Src[Vi] - Avg) * (Src[Vi] - Avg) );
    
    if ( VSum2 < VPrevSum2 ) {
      return (-1);
    }
    
    VPrevSum2 = VSum2;
    
  } /* End for */

  if ( VSum2 <= 0 ) {
    err_warning (( ERR_OUT, "Sigma calc error : Sum2=%.3f <= 0 => Sigma=0", VSum2 ));
    return (0);
  }
  
  return ( sqrt ( VSum2 / (float) EltNb ) );
}

// 14/08/2007

float MATH_FUInt8Sigma1 ( UInt8* Src, SInt32 EltNb, float Avg ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32 Vi;
  double  VPrevSum2;
  double  VSum2;
  double  VAvg2;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VSum2 = 0;
  VPrevSum2 = 0;
  VAvg2 = Avg * Avg;
  
  
  for ( Vi=0; Vi < EltNb; Vi++) {
    
    
    VSum2 = VSum2 + ( (Src[Vi] - Avg) * (Src[Vi] - Avg) );
    
    if ( VSum2 < VPrevSum2 ) {
      return (-1);
    }
    
    VPrevSum2 = VSum2;
    
  } /* End for */

  if ( VSum2 <= 0 ) {
    err_warning (( ERR_OUT, "Sigma calc error : Sum2=%.3f <= 0 => Sigma=0", VSum2 ));
    return (0);
  }
  
  return ( sqrt ( VSum2 / (float) EltNb ) );
}


/* 21/06/2006 */
/* Calculates the sigma value of Src array selected elements              */
/* SelList[i] == 1 if the corresponding item must be used for  processing */
  
float MATH_FSInt16DataListSigma1 ( SInt16* Src, SInt8* SelList, SInt32 EltNb, float Avg ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32  Vi;
  SInt32  VEltCnt;
  double  VPrevSum2;
  double  VSum2;
  double  VAvg2;
  SInt32  VOutOfRangeCnt;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  err_retnull ( SelList, (ERR_OUT,"SelList == NULL") );
  
  VSum2 = 0;
  VPrevSum2 = 0;
  VAvg2 = Avg * Avg;
  
  VEltCnt        = 0;
  VOutOfRangeCnt = 0;
  
  for ( Vi=0; Vi < EltNb; Vi++) {
    
    if ( SelList[Vi] != 1 ) {
      continue;
    }
    
    ++VEltCnt;
    
    if ( ( Src[Vi] < (Avg-6) ) || (Src[Vi] > (Avg+6) ) ) {
      ++VOutOfRangeCnt;
    }
    
    VSum2 = VSum2 + ( (Src[Vi] - Avg) * (Src[Vi] - Avg) );
    
    if ( VSum2 < VPrevSum2 ) {
      return (-1);
    }
    
    VPrevSum2 = VSum2;
    
  } /* End for */
  
  //  return ( VOutOfRangeCnt );
  
  if ( VSum2 <= 0 ) {
    err_warning (( ERR_OUT, "Sigma calc error : Sum2=%.3f <= 0 => Sigma=0", VSum2 ));
    return (0);
  }
  
  return ( sqrt ( VSum2 / (float) VEltCnt ) );
}




float MATH_FFloatSigma1 ( float* Src, SInt32 EltNb, float Avg ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  double  VSum2;
  double VAvg2;

  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VSum2 = 0;
  VAvg2 = Avg * Avg;
  
  for ( Vi=0; Vi < EltNb; Vi++) {
    VSum2 = VSum2 + ( (Src[Vi] *Src[Vi]) - VAvg2 );
    /* VSum2 = VSum2 + ( (Src[Vi] - Avg) * (Src[Vi] - Avg) ); */
  }

  if ( VSum2 <= 0 ) {
    err_warning (( ERR_OUT, "Sigma calc error : Sum2=%.3f <= 0 => Sigma=0", VSum2 ));
    return (0);
  }
  
  
  return ( sqrt ( VSum2 / (float) EltNb ) );
}


void MATH_FDiffPedest ( SInt32* Src, float Pedest, float* Dest, SInt32 EltNb ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;

  if ( EltNb <= 0 ) {
    err_error (( ERR_OUT, "Bad EltNb=%d => must be > 0", EltNb ));
    return;
  }
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    Dest[Vi] = Src[Vi] - Pedest;
  }
  
}



void MATH_FDiffPedestUInt8FloatFloat ( UInt8* Src, float* Pedest, float* Dest, SInt32 EltNb ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32 Vi;
  
  if ( EltNb <= 0 ) {
    err_error (( ERR_OUT, "Bad EltNb=%d => must be > 0", EltNb ));
    return;
  }
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    Dest[Vi] = Src[Vi] - Pedest[Vi];
  }
  
}


/* 07/12/04 */

SInt32 MATH_FScaleFloatVect ( float* Src, float* Dest, SInt32 EltNb, float Offset, float Slope ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;

  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    Dest[Vi] = ( Src[Vi] * Slope ) + Offset;
  }

  
}

/* 02/01/05 */

SInt32 MATH_FIntInRange ( SInt32 Min, SInt32 Max, SInt32 Val ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  if ( (Val >= Min) && (Val <= Max)) {
    return (1);
  }

  err_retfail ( -1, (ERR_OUT,"Value = %d out of range <%d..%d> ", Val, Min, Max) );
}

/* 02/01/05 */

SInt32 MATH_FFloatInRange ( float Min, float Max, float Val ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  if ( (Val >= Min) && (Val <= Max)) {
    return (1);
  }
  
  err_retfail ( -1, (ERR_OUT,"Value = %d out of range <%d..%d> ", Val, Min, Max) );
}


/* 26/04/06 */

SInt32 MATH_FOffsetSInt16Vect ( SInt16* Src, SInt16* Dest, SInt32 EltNb, SInt16 Offset ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  err_retnull ( Src , (ERR_OUT,"Src  == NULL") );
  err_retnull ( Dest, (ERR_OUT,"Dest == NULL") );
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    Dest[Vi] = Src[Vi] + Offset;
  }
  
  return (0);
}


/* 26/04/06 */

SInt32 MATH_FTranslateSInt16ToUInt16Vect ( SInt16* Src, UInt16* Dest, SInt32 EltNb ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  err_retnull ( Src , (ERR_OUT,"Src  == NULL") );
  err_retnull ( Dest, (ERR_OUT,"Dest == NULL") );
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    Dest[Vi] = Src[Vi] + 32768;
  }
  
  return (0);
}

/* 30/04/06 */
  

SInt32 MATH_FScaleSInt32Vect ( SInt32* Src, SInt32* Dest, SInt32 EltNb, SInt32 Offset, SInt32 Slope ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  err_retnull ( Src , (ERR_OUT,"Src  == NULL") );
  err_retnull ( Dest, (ERR_OUT,"Dest == NULL") );
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    Dest[Vi] = Offset + ( Src[Vi] * Slope );
  }
  
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
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 26/04/2006
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */



SInt32 MATH_FHistoSInt16Book ( SInt16 HistoId, SInt16 Min, SInt16 Max, SInt16 BinW ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  err_warning (( ERR_OUT, "Do nothing for the moment : reserved for future use" ));
  
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
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 26/04/2006
Modif     : 04/05/2006
          : - Add a test on BinNb <= MATH_HISTO_MAX_BIN_NB
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */



SInt32 MATH_FHistoSInt16Build ( SInt16 HistoId, SInt16* PtSrc, SInt32 EltNb, SInt16 Min, SInt16 Max, SInt16 BinW, SInt16* PtCalcMax, SInt32* PtUdf, SInt32* PtOvf, SInt32** PtPtDestX, SInt32** PtPtDestY, SInt8 DbgPlot ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32  VRet;
  SInt32  VUdfCnt;
  SInt32  VOvfCnt;
  SInt16  VMin;
  SInt16  VMax;
  SInt16  VBinW;
  SInt16  VBinNb;
  SInt32  Vi;
  SInt32  VBinSIndex;
  SInt32  VFirstBinSIndex;
  SInt32  VBinUIndex;
  
  static SInt32 VADestX [MATH_HISTO_MAX_ID_NB][MATH_HISTO_MAX_BIN_NB + 1];
  static SInt32 VADestY [MATH_HISTO_MAX_ID_NB][MATH_HISTO_MAX_BIN_NB + 1];
  
  /* ------------------------- */
  /* Check function parameters */
  /* ------------------------- */
  
  /* Check histo ID */
  
  err_retfail ( HistoId, (ERR_OUT,"HistoId=%d MUST be > 0", HistoId) );
  
  if ( HistoId >= MATH_HISTO_MAX_ID_NB  ) {
    err_retfail ( -1, (ERR_OUT,"HistoId=%d >= MATH_HISTO_MAX_ID_NB=%d", HistoId, MATH_HISTO_MAX_ID_NB ) );
  }
  
  /* Check pointers parameters */
  
  err_retnull ( PtSrc    , (ERR_OUT,"PtSrc == NULL") );
  err_retnull ( PtCalcMax, (ERR_OUT,"PtCalcMax == NULL") );
  err_retnull ( PtPtDestX, (ERR_OUT,"PtPtDestX == NULL") );
  err_retnull ( PtPtDestY, (ERR_OUT,"PtPtDestY == NULL") );

  /* ------------------------ */
  /* Reset result arrays      */
  /* ------------------------ */
  
  for ( Vi = 0; Vi < MATH_HISTO_MAX_BIN_NB + 1; Vi++ ) {
    VADestX[HistoId][Vi] = 0;
    VADestY[HistoId][Vi] = 0;
  }
  

  /* ------------------------ */
  /* Calculate histo bounds   */
  /* ------------------------ */
  
  VMin  = Min;
  VBinW = BinW;
  
  VBinNb  = MATH_FSInt32Ceil (  ( Max - Min ) / VBinW );
  
  if ( VBinNb > MATH_HISTO_MAX_BIN_NB ) {
    err_retfail ( -1, (ERR_OUT,"Too much bin nb = %d > MATH_HISTO_MAX_BIN_NB = %d", VBinNb, MATH_HISTO_MAX_BIN_NB) );
  }
  
  VMax = VMin + ( VBinW * VBinNb );
  *PtCalcMax = VMax;
  
  if ( VMax > Max ) {
    err_warning (( ERR_OUT, "Max recalculated Set value=%d - Calculated value=%d", Max, VMax ));
  }
  
  /* ------------------------ */
  /* Build bin content        */
  /* ------------------------ */
  
  VUdfCnt = 0;
  VOvfCnt = 0;
  
  VFirstBinSIndex = VMin / VBinW;
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    
    if (PtSrc[Vi] < VMin) {
      ++VUdfCnt;
      continue;
    }
    
    if (PtSrc[Vi] > VMax) {
      ++VOvfCnt;
      continue;
    }
    
    VBinSIndex = PtSrc[Vi] / VBinW;
    
    if ( VBinSIndex < 0 ) {
      --VBinSIndex;
    }
    
    VBinUIndex = VBinSIndex - VFirstBinSIndex;
    
    ++VADestY[HistoId][VBinUIndex];
    
  } /* End for */
  
  /* ------------------------ */
  /* Update Udf & Ovf         */
  /* ------------------------ */
  
  if ( PtUdf != NULL ) {
    *PtUdf = VUdfCnt;
  }
  
  if ( PtOvf != NULL ) {
    *PtOvf = VOvfCnt;
  }
  
  
  /* ------------------------ */
  /* Build X array content    */
  /* ------------------------ */
  
  for ( Vi=0; Vi < (VBinNb + 1); Vi++ ) {
    
    VADestX[HistoId][Vi] = VMin + ( Vi * VBinW );
    
  } /* End for */
  
  /* ---------------------------------------- */
  /* Init paramters pointers to result arrays */
  /* ---------------------------------------- */
  
  *PtPtDestX = VADestX[HistoId];
  *PtPtDestY = VADestY[HistoId];
  
  
  /* ------------------------ */
  /* Plot arrays              */
  /* ------------------------ */
  
  if ( DbgPlot ) {
    
    for ( Vi=0; Vi < VBinNb; Vi++ ) {
      msg (( MSG_OUT, "Bin [Index=%4d] [Range=%4d..%4d] Count = %4d", Vi, VADestX[HistoId][Vi], VADestX[HistoId][Vi+1], VADestY[HistoId][Vi] ));
    } /* End for */
    
    for ( Vi=0; Vi < VBinNb; Vi++ ) {
      msg (( MSG_OUT, "X[%4d]=%4d - Y[%4d]=%4d", Vi, VADestX[HistoId][Vi], Vi, VADestY[HistoId][Vi] ));
    } /* End for */
    

  } /* End if */
  
  return (VBinNb);
}


/* 30/04/06 */

SInt32 MATH_FCpyUInt16ArrayToSInt32Array ( UInt16* PtSrc, SInt32 SrcEltSz, SInt32* PtDest, SInt32 DestEltSz ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  
  if ( SrcEltSz > DestEltSz ) {
    err_retfail ( -1, (ERR_OUT,"SrcEltSz=%d > DestEltSz=%d", SrcEltSz, DestEltSz) );
  }
  
  for ( Vi=0; Vi < SrcEltSz; Vi++ ) {
    PtDest[Vi] = PtSrc[Vi];
  }
  
}


/* 30/04/06 */

SInt32 MATH_FCpySInt16ArrayToSInt32Array ( SInt16* PtSrc, SInt32 SrcEltSz, SInt32* PtDest, SInt32 DestEltSz ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  
  SInt32 Vi;
  
  if ( SrcEltSz > DestEltSz ) {
    err_retfail ( -1, (ERR_OUT,"SrcEltSz=%d > DestEltSz=%d", SrcEltSz, DestEltSz) );
  }
  
  for ( Vi=0; Vi < SrcEltSz; Vi++ ) {
    PtDest[Vi] = PtSrc[Vi];
  }
  
}


/* TO REMOVE ONE DAY - Following lines are in comment */

/*

UInt16 MATH_FSInt16Min ( UInt16* Src, SInt32 EltNb ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  UInt16 VMin;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VMin = Src[0];
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    if ( Src[Vi] < VMin ) {
      VMin = Src[Vi];
    }
  }
  
  return (VMin);
}




UInt16 MATH_FSInt16Max ( UInt16* Src, SInt32 EltNb ) {
  
#ifdef APP_ROOT
  static char VFuncName[] = "";
#endif

  SInt32 Vi;
  UInt16 VMax;
  
  if ( EltNb <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Bad EltNb=%d => must be > 0", EltNb) );
  }
  
  VMax = Src[0];
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    if ( Src[Vi] > VMax ) {
      VMax = Src[Vi];
    }
  }
  
  return (VMax);
}

*/


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
Date      : 01/08/2007
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

float MATH_UCONV_FVoltageGain2dB ( float Gain ) {

  float VdB;

  if ( Gain <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Gain = %.3f must be > 0", Gain) );
  }

  VdB = 20 * log10 ( Gain );

  return (VdB);  
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
Date      : 01/08/2007
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

float MATH_UCONV_FdB2VoltageGain ( float dB ) {
  
  float VGain;
    
  VGain = MATH_FFloatPow ( 10, ((float) dB / (float) 20) );
  
  return (VGain);
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
Date      : 01/08/2007
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

float MATH_UCONV_FPowerGain2dB ( float Gain ) {
  
  float VdB;

  if ( Gain <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Gain = %.3f must be > 0", Gain) );
  }
  
  VdB = 10 * log10 ( Gain );
  
  return (VdB);
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
Date      : 01/08/2007
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

float MATH_UCONV_FdB2PowerGain ( float dB ) {
  
  float VGain;

  
  VGain = MATH_FFloatPow ( 10, ((float) dB / (float) 10) );
  
  return (VGain);
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
Date      : 01/08/2007
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

float MATH_UCONV_FV2dBV ( float Volt ) {
  
  float VdBV;

  if ( Volt <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Vlot = %.3f must be > 0", Volt) );
  }
  
  VdBV = MATH_UCONV_FVoltageGain2dB ( Volt );
    
  return (VdBV);
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
Date      : 01/08/2007
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

float MATH_UCONV_FdBV2V ( float dBV ) {
  
  float Volt;

  
  Volt = MATH_UCONV_FdB2VoltageGain ( dBV );
    
  return (Volt);
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
Date      : 01/08/2007
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

float MATH_UCONV_FmV2dBV ( float mV ) {
  
  float VdBV;
  
  VdBV = MATH_UCONV_FV2dBV ( (float) mV / (float) 1000 );
  
  return (VdBV);
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
Date      : 01/08/2007
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

float MATH_UCONV_FdBV2mV ( float dBV ) {
  
  float VmV;
  
  VmV = 1000 * MATH_UCONV_FdBV2V ( dBV );
  
  return (VmV);
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
Date      : 01/08/2007
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

float MATH_UCONV_FV2dBmV ( float Volt ) {
  
  float VdBmV;
  
  if ( Volt <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Vlot = %.3f must be > 0", Volt) );
  }
  
  VdBmV = MATH_UCONV_FVoltageGain2dB ( (float) Volt / (float) 0.001 );
  
  return (VdBmV);
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
Date      : 01/08/2007
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

float MATH_UCONV_FdBmV2V ( float dBmV ) {
  
  float Volt;
  
  
  Volt = 0.001 * MATH_UCONV_FdB2VoltageGain ( dBmV );
  
  return (Volt);
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
Date      : 01/08/2007
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

float MATH_UCONV_FV2dBm600 ( float Volt ) {
  
  float VPowerW;
  float VdBm600;

  if ( Volt <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Vlot = %.3f must be > 0", Volt) );
  }
  
  VPowerW = (float) (( Volt * Volt )) / (float) 600;
  
  VdBm600  = 10 * log10 ( VPowerW / 0.001 );
    
  err_trace (( ERR_OUT, "V = %.3f - Power mW = %.3f - dBm = %.3f ", Volt, VPowerW, VdBm600 ));
  
  return (VdBm600);
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
Date      : 01/08/2007
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

float MATH_UCONV_FdBm600_2V ( float dBm600 ) {
  
  float VPowerW;
  float VVolt;
  
  VPowerW = 0.001 * MATH_UCONV_FdB2PowerGain ( dBm600 );
  
  VVolt = sqrt ( VPowerW * 600 );
  
  return (VVolt);
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
Date      : 01/08/2007
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

float MATH_UCONV_FmV2dBm600 ( float mV ) {
  
  float VdBm600;
    
  VdBm600  = MATH_UCONV_FV2dBm600 ( (float) mV / (float) 1000 );

  err_trace (( ERR_OUT, "mV = %.3f - dBm = %.3f ", mV, VdBm600 ));
  
  return (VdBm600);
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
Date      : 01/08/2007
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

float MATH_UCONV_FV2dBm50 ( float Volt ) {
  
  float VPowerW;
  float VdBm50;

  if ( Volt <= 0 ) {
    err_retfail ( -1, (ERR_OUT,"Vlot = %.3f must be > 0", Volt) );
  }
  
  VPowerW = (float) (( Volt * Volt )) / (float) 50;
  
  VdBm50  = 10 * log10 ( VPowerW / 0.001 );
  
  return (VdBm50);
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
Date      : 01/08/2007
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

float MATH_UCONV_FdBm50_2V ( float dBm50 ) {
  
  float VPowerW;
  float VVolt;
  
  VPowerW = 0.001 * MATH_UCONV_FdB2PowerGain ( dBm50 );
  
  VVolt = sqrt ( VPowerW * 50 );
  
  return (VVolt);
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
Date      : 01/08/2007
Doc date  : //2004
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

float MATH_UCONV_FmV2dBm50 ( float mV ) {
  
  float VdBm50;
    
  VdBm50  = MATH_UCONV_FV2dBm50 ( (float) mV / (float) 1000 ) ;
  
  return (VdBm50);
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
Date      : 08/08/2007
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

SInt32 DAM_FAllocResizeFloatBuffer ( SInt32 NewBuffESz, SInt32* PtCurBuffESz, float LowAdjustTheresholdRatio, float** PtPtBuff ) {
  
  float VBuffESzRatio;
  
  if ( (*PtCurBuffESz) != 0 ) {
    VBuffESzRatio = (float) NewBuffESz / (float) (*PtCurBuffESz);
  }
  
  else {
    VBuffESzRatio = 1.1;
  }
  
  if ( (NewBuffESz > (*PtCurBuffESz)) || (VBuffESzRatio < LowAdjustTheresholdRatio) ) {
    
    if ( (*PtPtBuff) != NULL ) {
      free (*PtPtBuff);
    }
    
    (*PtPtBuff) = (float*) malloc ( NewBuffESz * (sizeof (float)) );
    
    err_retnull ( (*PtPtBuff), (ERR_OUT,"Allocation of %d SInt32 failed !", NewBuffESz) );
    
    err_trace (( ERR_OUT, "Buffer size adjusted : Previous = %d SInt32 - New = %d SInt32", (*PtCurBuffESz), NewBuffESz ));
    
    (*PtCurBuffESz) = NewBuffESz;
  }
  
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
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 08/08/2007
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */

// 08/08/2007

SInt32 DAM_FCpyVectUInt8ToFloat ( UInt8* PtSrc, float* PtDest, SInt32 NbElt ) {

  SInt32 Vi;

  err_retnull ( PtSrc , (ERR_OUT,"PtSrc  == NULL") );
  err_retnull ( PtDest, (ERR_OUT,"PtDest == NULL") );
  
  for ( Vi=0; Vi < NbElt; Vi++ ) {
    PtDest[Vi] = PtSrc[Vi];
  }

  return (0);
}

// 08/08/2007

SInt32 DAM_FCpyVectSInt8ToFloat ( SInt8* PtSrc, float* PtDest, SInt32 NbElt ) {
  
  SInt32 Vi;
  
  err_retnull ( PtSrc , (ERR_OUT,"PtSrc  == NULL") );
  err_retnull ( PtDest, (ERR_OUT,"PtDest == NULL") );
  
  for ( Vi=0; Vi < NbElt; Vi++ ) {
    PtDest[Vi] = PtSrc[Vi];
  }
  
  return (0);
}

// 08/08/2007

SInt32 DAM_FCpyVectUInt16ToFloat ( UInt16* PtSrc, float* PtDest, SInt32 NbElt ) {
  
  SInt32 Vi;
  
  err_retnull ( PtSrc , (ERR_OUT,"PtSrc  == NULL") );
  err_retnull ( PtDest, (ERR_OUT,"PtDest == NULL") );
  
  for ( Vi=0; Vi < NbElt; Vi++ ) {
    PtDest[Vi] = PtSrc[Vi];
  }
  
  return (0);
}

// 08/08/2007

SInt32 DAM_FCpyVectSInt16ToFloat ( SInt16* PtSrc, float* PtDest, SInt32 NbElt ) {
  
  SInt32 Vi;
  
  err_retnull ( PtSrc , (ERR_OUT,"PtSrc  == NULL") );
  err_retnull ( PtDest, (ERR_OUT,"PtDest == NULL") );
  
  for ( Vi=0; Vi < NbElt; Vi++ ) {
    PtDest[Vi] = PtSrc[Vi];
  }
  
  return (0);
}

// 08/08/2007

SInt32 DAM_FCpyVectUInt32ToFloat ( UInt32* PtSrc, float* PtDest, SInt32 NbElt ) {
  
  SInt32 Vi;
  
  err_retnull ( PtSrc , (ERR_OUT,"PtSrc  == NULL") );
  err_retnull ( PtDest, (ERR_OUT,"PtDest == NULL") );
  
  for ( Vi=0; Vi < NbElt; Vi++ ) {
    PtDest[Vi] = PtSrc[Vi];
  }
  
  return (0);
}

// 08/08/2007

SInt32 DAM_FCpyVectSInt32ToFloat ( SInt32* PtSrc, float* PtDest, SInt32 NbElt ) {
  
  SInt32 Vi;
  
  err_retnull ( PtSrc , (ERR_OUT,"PtSrc  == NULL") );
  err_retnull ( PtDest, (ERR_OUT,"PtDest == NULL") );
  
  for ( Vi=0; Vi < NbElt; Vi++ ) {
    PtDest[Vi] = PtSrc[Vi];
  }
  
  return (0);
}


// 08/08/2007

SInt32 DAM_FCpyVectFloatToFloat ( float* PtSrc, float* PtDest, SInt32 NbElt ) {
  
  SInt32 Vi;
  
  err_retnull ( PtSrc , (ERR_OUT,"PtSrc  == NULL") );
  err_retnull ( PtDest, (ERR_OUT,"PtDest == NULL") );
  
  for ( Vi=0; Vi < NbElt; Vi++ ) {
    PtDest[Vi] = PtSrc[Vi];
  }
  
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
Ouputs    :
:
Globals   :
:
Remark    :
:
Level     :
Date      : 14/08/2007
Modif     :
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */



SInt32 MATH_FHistoVectUInt8 ( UInt8* PtSrc, SInt32 EltNb, UInt8 Min, UInt8 Max, UInt8 BinW, float* PtCalcMax, SInt32* PtUdf, SInt32* PtOvf, float* PtDestX, float* PtDestY, SInt32 DestESz, SInt8 DbgPlot ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32  VRet;
  SInt32  VUdfCnt;
  SInt32  VOvfCnt;
  UInt8   VMin;
  UInt8   VMax;
  UInt8   VBinW;
  SInt32  VBinNb;
  SInt32  Vi;
  SInt32  VBinSIndex;
  SInt32  VFirstBinSIndex;
  SInt32  VBinUIndex;
    
  /* ------------------------- */
  /* Check function parameters */
  /* ------------------------- */
  
  
  /* Check pointers parameters */
  
  err_retnull ( PtSrc    , (ERR_OUT,"PtSrc == NULL") );
  err_retnull ( PtCalcMax, (ERR_OUT,"PtCalcMax == NULL") );
  err_retnull ( PtDestX  , (ERR_OUT,"PtDestX == NULL") );
  err_retnull ( PtDestY  , (ERR_OUT,"PtDestY == NULL") );
  
  if ( DestESz < MATH_HISTO_MAX_BIN_NB + 1 ) {
    err_retfail ( -1, (ERR_OUT,"Destination array too small %d elt < MATH_HISTO_MAX_BIN_NB + 1 = %d", DestESz, MATH_HISTO_MAX_BIN_NB + 1 ) );
  }
  
  /* ------------------------ */
  /* Reset result arrays      */
  /* ------------------------ */
  
  for ( Vi = 0; Vi < MATH_HISTO_MAX_BIN_NB + 1; Vi++ ) {
    PtDestX[Vi] = 0;
    PtDestY[Vi] = 0;
  }
  
   
  /* ------------------------ */
  /* Calculate histo bounds   */
  /* ------------------------ */
  
  VMin  = Min;
  VBinW = BinW;
  
  VBinNb  = MATH_FSInt32Ceil (  ( Max - Min ) / VBinW ) + 1;
  
  if ( VBinNb > MATH_HISTO_MAX_BIN_NB ) {
    err_retfail ( -1, (ERR_OUT,"Too much bin nb = %d > MATH_HISTO_MAX_BIN_NB = %d", VBinNb, MATH_HISTO_MAX_BIN_NB) );
  }
  
  VMax = VMin + ( VBinW * (VBinNb - 1) );
  *PtCalcMax = VMax;
  
  if ( VMax > Max ) {
    err_warning (( ERR_OUT, "Max recalculated Set value=%d - Calculated value=%d", Max, VMax ));
  }
  
  err_trace (( ERR_OUT, "Min=%d - Max=%d - BinW=%d - BinNb=%d", VMin, VMax, VBinW, VBinNb ));
  
  /* ------------------------ */
  /* Build bin content        */
  /* ------------------------ */
  
  VUdfCnt = 0;
  VOvfCnt = 0;
  
  VFirstBinSIndex = VMin / VBinW;
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    
    if (PtSrc[Vi] < VMin) {
      ++VUdfCnt;
      continue;
    }
    
    if (PtSrc[Vi] > VMax) {
      ++VOvfCnt;
      continue;
    }
    
    VBinSIndex = PtSrc[Vi] / VBinW;
    
    if ( VBinSIndex < 0 ) {
      --VBinSIndex;
    }
    
    VBinUIndex = VBinSIndex - VFirstBinSIndex;
    
    ++PtDestY[VBinUIndex];
    
  } /* End for */
  
  /* ------------------------ */
  /* Update Udf & Ovf         */
  /* ------------------------ */
  
  if ( PtUdf != NULL ) {
    *PtUdf = VUdfCnt;
  }
  
  if ( PtOvf != NULL ) {
    *PtOvf = VOvfCnt;
  }
  
  
  /* ------------------------ */
  /* Build X array content    */
  /* ------------------------ */
  
  for ( Vi=0; Vi < (VBinNb + 1); Vi++ ) {
    
    PtDestX[Vi] = VMin + ( Vi * VBinW );
    
  } /* End for */
    
  
  /* ------------------------ */
  /* Plot arrays              */
  /* ------------------------ */
  
  if ( DbgPlot ) {
    
    for ( Vi=0; Vi < VBinNb; Vi++ ) {
      msg (( MSG_OUT, "Bin [Index=%4d] [Range=%4d..%4d] Count = %4d", Vi, PtDestX[Vi], PtDestX[Vi+1], PtDestY[Vi] ));
    } /* End for */
    
    for ( Vi=0; Vi < VBinNb; Vi++ ) {
      msg (( MSG_OUT, "X[%4d]=%4d - Y[%4d]=%4d", Vi, PtDestX[Vi], Vi, PtDestY[Vi] ));
    } /* End for */
    
    
  } /* End if */
  
  return (VBinNb);
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
Date      : 14/08/2007
Modif     :
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */



SInt32 MATH_FHistoVectUInt16 ( UInt16* PtSrc, SInt32 EltNb, UInt16 Min, UInt16 Max, UInt16 BinW, float* PtCalcMax, SInt32* PtUdf, SInt32* PtOvf, float* PtDestX, float* PtDestY, SInt32 DestESz, SInt8 DbgPlot ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32  VRet;
  SInt32  VUdfCnt;
  SInt32  VOvfCnt;
  UInt16  VMin;
  UInt16  VMax;
  UInt16  VBinW;
  SInt32  VBinNb;
  SInt32  Vi;
  SInt32  VBinSIndex;
  SInt32  VFirstBinSIndex;
  SInt32  VBinUIndex;
  
  /* ------------------------- */
  /* Check function parameters */
  /* ------------------------- */
  
  
  /* Check pointers parameters */
  
  err_retnull ( PtSrc    , (ERR_OUT,"PtSrc == NULL") );
  err_retnull ( PtCalcMax, (ERR_OUT,"PtCalcMax == NULL") );
  err_retnull ( PtDestX  , (ERR_OUT,"PtDestX == NULL") );
  err_retnull ( PtDestY  , (ERR_OUT,"PtDestY == NULL") );
  
  if ( DestESz < MATH_HISTO_MAX_BIN_NB + 1 ) {
    err_retfail ( -1, (ERR_OUT,"Destination array too small %d elt < MATH_HISTO_MAX_BIN_NB + 1 = %d", DestESz, MATH_HISTO_MAX_BIN_NB + 1 ) );
  }
  
  /* ------------------------ */
  /* Reset result arrays      */
  /* ------------------------ */
  
  for ( Vi = 0; Vi < MATH_HISTO_MAX_BIN_NB + 1; Vi++ ) {
    PtDestX[Vi] = 0;
    PtDestY[Vi] = 0;
  }
  
  
  /* ------------------------ */
  /* Calculate histo bounds   */
  /* ------------------------ */
  
  VMin  = Min;
  VBinW = BinW;
  
  VBinNb  = MATH_FSInt32Ceil (  ( Max - Min ) / VBinW ) + 1;
  
  if ( VBinNb > MATH_HISTO_MAX_BIN_NB ) {
    err_retfail ( -1, (ERR_OUT,"Too much bin nb = %d > MATH_HISTO_MAX_BIN_NB = %d", VBinNb, MATH_HISTO_MAX_BIN_NB) );
  }
  
  VMax = VMin + ( VBinW * (VBinNb - 1) );
  *PtCalcMax = VMax;
  
  if ( VMax > Max ) {
    err_warning (( ERR_OUT, "Max recalculated Set value=%d - Calculated value=%d", Max, VMax ));
  }
  
  /* ------------------------ */
  /* Build bin content        */
  /* ------------------------ */
  
  VUdfCnt = 0;
  VOvfCnt = 0;
  
  VFirstBinSIndex = VMin / VBinW;
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    
    if (PtSrc[Vi] < VMin) {
      ++VUdfCnt;
      continue;
    }
    
    if (PtSrc[Vi] > VMax) {
      ++VOvfCnt;
      continue;
    }
    
    VBinSIndex = PtSrc[Vi] / VBinW;
    
    if ( VBinSIndex < 0 ) {
      --VBinSIndex;
    }
    
    VBinUIndex = VBinSIndex - VFirstBinSIndex;
    
    ++PtDestY[VBinUIndex];
    
  } /* End for */
  
  /* ------------------------ */
  /* Update Udf & Ovf         */
  /* ------------------------ */
  
  if ( PtUdf != NULL ) {
    *PtUdf = VUdfCnt;
  }
  
  if ( PtOvf != NULL ) {
    *PtOvf = VOvfCnt;
  }
  
  
  /* ------------------------ */
  /* Build X array content    */
  /* ------------------------ */
  
  for ( Vi=0; Vi < (VBinNb + 1); Vi++ ) {
    
    PtDestX[Vi] = VMin + ( Vi * VBinW );
    
  } /* End for */
  
  
  /* ------------------------ */
  /* Plot arrays              */
  /* ------------------------ */
  
  if ( DbgPlot ) {
    
    for ( Vi=0; Vi < VBinNb; Vi++ ) {
      msg (( MSG_OUT, "Bin [Index=%4d] [Range=%4d..%4d] Count = %4d", Vi, PtDestX[Vi], PtDestX[Vi+1], PtDestY[Vi] ));
    } /* End for */
    
    for ( Vi=0; Vi < VBinNb; Vi++ ) {
      msg (( MSG_OUT, "X[%4d]=%4d - Y[%4d]=%4d", Vi, PtDestX[Vi], Vi, PtDestY[Vi] ));
    } /* End for */
    
    
  } /* End if */
  
  return (VBinNb);
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
Date      : 14/08/2007
Modif     :
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */



SInt32 MATH_FHistoVectUInt32 ( UInt32* PtSrc, SInt32 EltNb, UInt32 Min, UInt32 Max, UInt32 BinW, float* PtCalcMax, SInt32* PtUdf, SInt32* PtOvf, float* PtDestX, float* PtDestY, SInt32 DestESz, SInt8 DbgPlot ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32  VRet;
  SInt32  VUdfCnt;
  SInt32  VOvfCnt;
  UInt32  VMin;
  UInt32  VMax;
  UInt32  VBinW;
  SInt32  VBinNb;
  SInt32  Vi;
  SInt32  VBinSIndex;
  SInt32  VFirstBinSIndex;
  SInt32  VBinUIndex;
  
  /* ------------------------- */
  /* Check function parameters */
  /* ------------------------- */
  
  
  /* Check pointers parameters */
  
  err_retnull ( PtSrc    , (ERR_OUT,"PtSrc == NULL") );
  err_retnull ( PtCalcMax, (ERR_OUT,"PtCalcMax == NULL") );
  err_retnull ( PtDestX  , (ERR_OUT,"PtDestX == NULL") );
  err_retnull ( PtDestY  , (ERR_OUT,"PtDestY == NULL") );
  
  if ( DestESz < MATH_HISTO_MAX_BIN_NB + 1 ) {
    err_retfail ( -1, (ERR_OUT,"Destination array too small %d elt < MATH_HISTO_MAX_BIN_NB + 1 = %d", DestESz, MATH_HISTO_MAX_BIN_NB + 1 ) );
  }
  
  /* ------------------------ */
  /* Reset result arrays      */
  /* ------------------------ */
  
  for ( Vi = 0; Vi < MATH_HISTO_MAX_BIN_NB + 1; Vi++ ) {
    PtDestX[Vi] = 0;
    PtDestY[Vi] = 0;
  }
  
  
  /* ------------------------ */
  /* Calculate histo bounds   */
  /* ------------------------ */
  
  VMin  = Min;
  VBinW = BinW;
  
  VBinNb  = MATH_FSInt32Ceil (  ( Max - Min ) / VBinW ) + 1;
  
  if ( VBinNb > MATH_HISTO_MAX_BIN_NB ) {
    err_retfail ( -1, (ERR_OUT,"Too much bin nb = %d > MATH_HISTO_MAX_BIN_NB = %d", VBinNb, MATH_HISTO_MAX_BIN_NB) );
  }
  
  VMax = VMin + ( VBinW * (VBinNb - 1) );
  *PtCalcMax = VMax;
  
  if ( VMax > Max ) {
    err_warning (( ERR_OUT, "Max recalculated Set value=%d - Calculated value=%d", Max, VMax ));
  }
  
  /* ------------------------ */
  /* Build bin content        */
  /* ------------------------ */
  
  VUdfCnt = 0;
  VOvfCnt = 0;
  
  VFirstBinSIndex = VMin / VBinW;
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    
    if (PtSrc[Vi] < VMin) {
      ++VUdfCnt;
      continue;
    }
    
    if (PtSrc[Vi] > VMax) {
      ++VOvfCnt;
      continue;
    }
    
    VBinSIndex = PtSrc[Vi] / VBinW;
    
    if ( VBinSIndex < 0 ) {
      --VBinSIndex;
    }
    
    VBinUIndex = VBinSIndex - VFirstBinSIndex;
    
    ++PtDestY[VBinUIndex];
    
  } /* End for */
  
  /* ------------------------ */
  /* Update Udf & Ovf         */
  /* ------------------------ */
  
  if ( PtUdf != NULL ) {
    *PtUdf = VUdfCnt;
  }
  
  if ( PtOvf != NULL ) {
    *PtOvf = VOvfCnt;
  }
  
  
  /* ------------------------ */
  /* Build X array content    */
  /* ------------------------ */
  
  for ( Vi=0; Vi < (VBinNb + 1); Vi++ ) {
    
    PtDestX[Vi] = VMin + ( Vi * VBinW );
    
  } /* End for */
  
  
  /* ------------------------ */
  /* Plot arrays              */
  /* ------------------------ */
  
  if ( DbgPlot ) {
    
    for ( Vi=0; Vi < VBinNb; Vi++ ) {
      msg (( MSG_OUT, "Bin [Index=%4d] [Range=%4d..%4d] Count = %4d", Vi, PtDestX[Vi], PtDestX[Vi+1], PtDestY[Vi] ));
    } /* End for */
    
    for ( Vi=0; Vi < VBinNb; Vi++ ) {
      msg (( MSG_OUT, "X[%4d]=%4d - Y[%4d]=%4d", Vi, PtDestX[Vi], Vi, PtDestY[Vi] ));
    } /* End for */
    
    
  } /* End if */
  
  return (VBinNb);
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
Date      : 14/08/2007
Modif     :
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */


SInt32 MATH_FHistoVectSInt8 ( SInt8* PtSrc, SInt32 EltNb, SInt8 Min, SInt8 Max, UInt8 BinW, float* PtCalcMax, SInt32* PtUdf, SInt32* PtOvf, float* PtDestX, float* PtDestY, SInt32 DestESz, SInt8 DbgPlot ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32  VRet;
  SInt32  VUdfCnt;
  SInt32  VOvfCnt;
  SInt8   VMin;
  SInt8   VMax;
  UInt8   VBinW;
  SInt32  VBinNb;
  SInt32  Vi;
  SInt32  VBinSIndex;
  SInt32  VFirstBinSIndex;
  SInt32  VBinUIndex;
  
  /* ------------------------- */
  /* Check function parameters */
  /* ------------------------- */
  
  
  /* Check pointers parameters */
  
  err_retnull ( PtSrc    , (ERR_OUT,"PtSrc == NULL") );
  err_retnull ( PtCalcMax, (ERR_OUT,"PtCalcMax == NULL") );
  err_retnull ( PtDestX  , (ERR_OUT,"PtDestX == NULL") );
  err_retnull ( PtDestY  , (ERR_OUT,"PtDestY == NULL") );
  
  if ( DestESz < MATH_HISTO_MAX_BIN_NB + 1 ) {
    err_retfail ( -1, (ERR_OUT,"Destination array too small %d elt < MATH_HISTO_MAX_BIN_NB + 1 = %d", DestESz, MATH_HISTO_MAX_BIN_NB + 1 ) );
  }
  
  /* ------------------------ */
  /* Reset result arrays      */
  /* ------------------------ */
  
  for ( Vi = 0; Vi < MATH_HISTO_MAX_BIN_NB + 1; Vi++ ) {
    PtDestX[Vi] = 0;
    PtDestY[Vi] = 0;
  }
  
  
  /* ------------------------ */
  /* Calculate histo bounds   */
  /* ------------------------ */
  
  VMin  = Min;
  VBinW = BinW;
  
  VBinNb  = MATH_FSInt32Ceil (  ( Max - Min ) / VBinW ) + 1;
  
  if ( VBinNb > MATH_HISTO_MAX_BIN_NB ) {
    err_retfail ( -1, (ERR_OUT,"Too much bin nb = %d > MATH_HISTO_MAX_BIN_NB = %d", VBinNb, MATH_HISTO_MAX_BIN_NB) );
  }
  
  VMax = VMin + ( VBinW * (VBinNb - 1) );
  *PtCalcMax = VMax;
  
  if ( VMax > Max ) {
    err_warning (( ERR_OUT, "Max recalculated Set value=%d - Calculated value=%d", Max, VMax ));
  }
  
  /* ------------------------ */
  /* Build bin content        */
  /* ------------------------ */
  
  VUdfCnt = 0;
  VOvfCnt = 0;
  
  VFirstBinSIndex = VMin / VBinW;
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    
    if (PtSrc[Vi] < VMin) {
      ++VUdfCnt;
      continue;
    }
    
    if (PtSrc[Vi] > VMax) {
      ++VOvfCnt;
      continue;
    }
    
    VBinSIndex = PtSrc[Vi] / VBinW;
    
    if ( VBinSIndex < 0 ) {
      --VBinSIndex;
    }
    
    VBinUIndex = VBinSIndex - VFirstBinSIndex;
    
    ++PtDestY[VBinUIndex];
    
  } /* End for */
  
  /* ------------------------ */
  /* Update Udf & Ovf         */
  /* ------------------------ */
  
  if ( PtUdf != NULL ) {
    *PtUdf = VUdfCnt;
  }
  
  if ( PtOvf != NULL ) {
    *PtOvf = VOvfCnt;
  }
  
  
  /* ------------------------ */
  /* Build X array content    */
  /* ------------------------ */
  
  for ( Vi=0; Vi < (VBinNb + 1); Vi++ ) {
    
    PtDestX[Vi] = VMin + ( Vi * VBinW );
    
  } /* End for */
  
  
  /* ------------------------ */
  /* Plot arrays              */
  /* ------------------------ */
  
  if ( DbgPlot ) {
    
    for ( Vi=0; Vi < VBinNb; Vi++ ) {
      msg (( MSG_OUT, "Bin [Index=%4d] [Range=%4d..%4d] Count = %4d", Vi, PtDestX[Vi], PtDestX[Vi+1], PtDestY[Vi] ));
    } /* End for */
    
    for ( Vi=0; Vi < VBinNb; Vi++ ) {
      msg (( MSG_OUT, "X[%4d]=%4d - Y[%4d]=%4d", Vi, PtDestX[Vi], Vi, PtDestY[Vi] ));
    } /* End for */
    
    
  } /* End if */
  
  return (VBinNb);
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
Date      : 14/08/2007
Modif     :
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */



SInt32 MATH_FHistoVectSInt16 ( SInt16* PtSrc, SInt32 EltNb, SInt16 Min, SInt16 Max, UInt16 BinW, float* PtCalcMax, SInt32* PtUdf, SInt32* PtOvf, float* PtDestX, float* PtDestY, SInt32 DestESz, SInt8 DbgPlot ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32  VRet;
  SInt32  VUdfCnt;
  SInt32  VOvfCnt;
  SInt16  VMin;
  SInt16  VMax;
  UInt16  VBinW;
  SInt32  VBinNb;
  SInt32  Vi;
  SInt32  VBinSIndex;
  SInt32  VFirstBinSIndex;
  SInt32  VBinUIndex;
  
  /* ------------------------- */
  /* Check function parameters */
  /* ------------------------- */
  
  
  /* Check pointers parameters */
  
  err_retnull ( PtSrc    , (ERR_OUT,"PtSrc == NULL") );
  err_retnull ( PtCalcMax, (ERR_OUT,"PtCalcMax == NULL") );
  err_retnull ( PtDestX  , (ERR_OUT,"PtDestX == NULL") );
  err_retnull ( PtDestY  , (ERR_OUT,"PtDestY == NULL") );
  
  if ( DestESz < MATH_HISTO_MAX_BIN_NB + 1 ) {
    err_retfail ( -1, (ERR_OUT,"Destination array too small %d elt < MATH_HISTO_MAX_BIN_NB + 1 = %d", DestESz, MATH_HISTO_MAX_BIN_NB + 1 ) );
  }
  
  /* ------------------------ */
  /* Reset result arrays      */
  /* ------------------------ */
  
  for ( Vi = 0; Vi < MATH_HISTO_MAX_BIN_NB + 1; Vi++ ) {
    PtDestX[Vi] = 0;
    PtDestY[Vi] = 0;
  }
  
  
  /* ------------------------ */
  /* Calculate histo bounds   */
  /* ------------------------ */
  
  VMin  = Min;
  VBinW = BinW;
  
  VBinNb  = MATH_FSInt32Ceil (  ( Max - Min ) / VBinW ) + 1;
  
  if ( VBinNb > MATH_HISTO_MAX_BIN_NB ) {
    err_retfail ( -1, (ERR_OUT,"Too much bin nb = %d > MATH_HISTO_MAX_BIN_NB = %d", VBinNb, MATH_HISTO_MAX_BIN_NB) );
  }
  
  VMax = VMin + ( VBinW * (VBinNb - 1) );
  *PtCalcMax = VMax;
  
  if ( VMax > Max ) {
    err_warning (( ERR_OUT, "Max recalculated Set value=%d - Calculated value=%d", Max, VMax ));
  }
  
  /* ------------------------ */
  /* Build bin content        */
  /* ------------------------ */
  
  VUdfCnt = 0;
  VOvfCnt = 0;
  
  VFirstBinSIndex = VMin / VBinW;
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    
    if (PtSrc[Vi] < VMin) {
      ++VUdfCnt;
      continue;
    }
    
    if (PtSrc[Vi] > VMax) {
      ++VOvfCnt;
      continue;
    }
    
    VBinSIndex = PtSrc[Vi] / VBinW;
    
    if ( VBinSIndex < 0 ) {
      --VBinSIndex;
    }
    
    VBinUIndex = VBinSIndex - VFirstBinSIndex;
    
    ++PtDestY[VBinUIndex];
    
  } /* End for */
  
  /* ------------------------ */
  /* Update Udf & Ovf         */
  /* ------------------------ */
  
  if ( PtUdf != NULL ) {
    *PtUdf = VUdfCnt;
  }
  
  if ( PtOvf != NULL ) {
    *PtOvf = VOvfCnt;
  }
  
  
  /* ------------------------ */
  /* Build X array content    */
  /* ------------------------ */
  
  for ( Vi=0; Vi < (VBinNb + 1); Vi++ ) {
    
    PtDestX[Vi] = VMin + ( Vi * VBinW );
    
  } /* End for */
  
  
  /* ------------------------ */
  /* Plot arrays              */
  /* ------------------------ */
  
  if ( DbgPlot ) {
    
    for ( Vi=0; Vi < VBinNb; Vi++ ) {
      msg (( MSG_OUT, "Bin [Index=%4d] [Range=%4d..%4d] Count = %4d", Vi, PtDestX[Vi], PtDestX[Vi+1], PtDestY[Vi] ));
    } /* End for */
    
    for ( Vi=0; Vi < VBinNb; Vi++ ) {
      msg (( MSG_OUT, "X[%4d]=%4d - Y[%4d]=%4d", Vi, PtDestX[Vi], Vi, PtDestY[Vi] ));
    } /* End for */
    
    
  } /* End if */
  
  return (VBinNb);
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
Date      : 14/08/2007
Modif     :
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */



SInt32 MATH_FHistoVectSInt32 ( SInt32* PtSrc, SInt32 EltNb, SInt32 Min, SInt32 Max, UInt32 BinW, float* PtCalcMax, SInt32* PtUdf, SInt32* PtOvf, float* PtDestX, float* PtDestY, SInt32 DestESz, SInt8 DbgPlot ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32  VRet;
  SInt32  VUdfCnt;
  SInt32  VOvfCnt;
  SInt32  VMin;
  SInt32  VMax;
  UInt32  VBinW;
  SInt32  VBinNb;
  SInt32  Vi;
  SInt32  VBinSIndex;
  SInt32  VFirstBinSIndex;
  SInt32  VBinUIndex;
  
  /* ------------------------- */
  /* Check function parameters */
  /* ------------------------- */
  
  
  /* Check pointers parameters */
  
  err_retnull ( PtSrc    , (ERR_OUT,"PtSrc == NULL") );
  err_retnull ( PtCalcMax, (ERR_OUT,"PtCalcMax == NULL") );
  err_retnull ( PtDestX  , (ERR_OUT,"PtDestX == NULL") );
  err_retnull ( PtDestY  , (ERR_OUT,"PtDestY == NULL") );
  
  if ( DestESz < MATH_HISTO_MAX_BIN_NB + 1 ) {
    err_retfail ( -1, (ERR_OUT,"Destination array too small %d elt < MATH_HISTO_MAX_BIN_NB + 1 = %d", DestESz, MATH_HISTO_MAX_BIN_NB + 1 ) );
  }
  
  /* ------------------------ */
  /* Reset result arrays      */
  /* ------------------------ */
  
  for ( Vi = 0; Vi < MATH_HISTO_MAX_BIN_NB + 1; Vi++ ) {
    PtDestX[Vi] = 0;
    PtDestY[Vi] = 0;
  }
  
  
  /* ------------------------ */
  /* Calculate histo bounds   */
  /* ------------------------ */
  
  VMin  = Min;
  VBinW = BinW;
  
  VBinNb  = MATH_FSInt32Ceil (  ( Max - Min ) / VBinW ) + 1;
  
  if ( VBinNb > MATH_HISTO_MAX_BIN_NB ) {
    err_retfail ( -1, (ERR_OUT,"Too much bin nb = %d > MATH_HISTO_MAX_BIN_NB = %d", VBinNb, MATH_HISTO_MAX_BIN_NB) );
  }
  
  VMax = VMin + ( VBinW * (VBinNb - 1) );
  *PtCalcMax = VMax;
  
  if ( VMax > Max ) {
    err_warning (( ERR_OUT, "Max recalculated Set value=%d - Calculated value=%d", Max, VMax ));
  }
  
  /* ------------------------ */
  /* Build bin content        */
  /* ------------------------ */
  
  VUdfCnt = 0;
  VOvfCnt = 0;
  
  VFirstBinSIndex = VMin / VBinW;
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    
    if (PtSrc[Vi] < VMin) {
      ++VUdfCnt;
      continue;
    }
    
    if (PtSrc[Vi] > VMax) {
      ++VOvfCnt;
      continue;
    }
    
    VBinSIndex = PtSrc[Vi] / VBinW;
    
    if ( VBinSIndex < 0 ) {
      --VBinSIndex;
    }
    
    VBinUIndex = VBinSIndex - VFirstBinSIndex;
    
    ++PtDestY[VBinUIndex];
    
  } /* End for */
  
  /* ------------------------ */
  /* Update Udf & Ovf         */
  /* ------------------------ */
  
  if ( PtUdf != NULL ) {
    *PtUdf = VUdfCnt;
  }
  
  if ( PtOvf != NULL ) {
    *PtOvf = VOvfCnt;
  }
  
  
  /* ------------------------ */
  /* Build X array content    */
  /* ------------------------ */
  
  for ( Vi=0; Vi < (VBinNb + 1); Vi++ ) {
    
    PtDestX[Vi] = VMin + ( Vi * VBinW );
    
  } /* End for */
  
  
  /* ------------------------ */
  /* Plot arrays              */
  /* ------------------------ */
  
  if ( DbgPlot ) {
    
    for ( Vi=0; Vi < VBinNb; Vi++ ) {
      msg (( MSG_OUT, "Bin [Index=%4d] [Range=%4d..%4d] Count = %4d", Vi, PtDestX[Vi], PtDestX[Vi+1], PtDestY[Vi] ));
    } /* End for */
    
    for ( Vi=0; Vi < VBinNb; Vi++ ) {
      msg (( MSG_OUT, "X[%4d]=%4d - Y[%4d]=%4d", Vi, PtDestX[Vi], Vi, PtDestY[Vi] ));
    } /* End for */
    
    
  } /* End if */
  
  return (VBinNb);
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
Date      : 14/08/2007
Modif     :
Doc date  : //2007
Author    : Gilles CLAUS
E-mail    : claus@lepsi.in2p3.fr
Labo      : LEPSI */
/* =================================================================================== */
/* DOC_FUNC_END */



SInt32 MATH_FHistoVectFloat ( float* PtSrc, SInt32 EltNb, float Min, float Max, float BinW, float* PtCalcMax, SInt32* PtUdf, SInt32* PtOvf, float* PtDestX, float* PtDestY, SInt32 DestESz, SInt8 DbgPlot ) {
  
  #ifdef APP_ROOT
  static char VFuncName[] = "";
  #endif
  
  SInt32  VRet;
  SInt32  VUdfCnt;
  SInt32  VOvfCnt;
  float   VMin;
  float   VMax;
  float   VBinW;
  SInt32  VBinNb;
  SInt32  Vi;
  SInt32  VBinSIndex;
  SInt32  VFirstBinSIndex;
  SInt32  VBinUIndex;
  
  /* ------------------------- */
  /* Check function parameters */
  /* ------------------------- */
  
  
  /* Check pointers parameters */
  
  err_retnull ( PtSrc    , (ERR_OUT,"PtSrc == NULL") );
  err_retnull ( PtCalcMax, (ERR_OUT,"PtCalcMax == NULL") );
  err_retnull ( PtDestX  , (ERR_OUT,"PtDestX == NULL") );
  err_retnull ( PtDestY  , (ERR_OUT,"PtDestY == NULL") );
  
  if ( DestESz < MATH_HISTO_MAX_BIN_NB + 1 ) {
    err_retfail ( -1, (ERR_OUT,"Destination array too small %d elt < MATH_HISTO_MAX_BIN_NB + 1 = %d", DestESz, MATH_HISTO_MAX_BIN_NB + 1 ) );
  }
  
  /* ------------------------ */
  /* Reset result arrays      */
  /* ------------------------ */
  
  for ( Vi = 0; Vi < MATH_HISTO_MAX_BIN_NB + 1; Vi++ ) {
    PtDestX[Vi] = 0;
    PtDestY[Vi] = 0;
  }
  
  
  /* ------------------------ */
  /* Calculate histo bounds   */
  /* ------------------------ */
  
  VMin  = Min;
  VBinW = BinW;
  
  VBinNb  = MATH_FSInt32Ceil (  ( Max - Min ) / VBinW ) + 1;
  
  if ( VBinNb > MATH_HISTO_MAX_BIN_NB ) {
    err_retfail ( -1, (ERR_OUT,"Too much bin nb = %d > MATH_HISTO_MAX_BIN_NB = %d", VBinNb, MATH_HISTO_MAX_BIN_NB) );
  }
  
  VMax = VMin + ( VBinW * (VBinNb - 1) );
  *PtCalcMax = VMax;
  
  if ( VMax > Max ) {
    err_warning (( ERR_OUT, "Max recalculated Set value=%d - Calculated value=%d", Max, VMax ));
  }
  
  /* ------------------------ */
  /* Build bin content        */
  /* ------------------------ */
  
  VUdfCnt = 0;
  VOvfCnt = 0;
  
  VFirstBinSIndex = VMin / VBinW;
  
  for ( Vi=0; Vi < EltNb; Vi++ ) {
    
    if (PtSrc[Vi] < VMin) {
      ++VUdfCnt;
      continue;
    }
    
    if (PtSrc[Vi] > VMax) {
      ++VOvfCnt;
      continue;
    }
    
    VBinSIndex = PtSrc[Vi] / VBinW;
    
    if ( VBinSIndex < 0 ) {
      --VBinSIndex;
    }
    
    VBinUIndex = VBinSIndex - VFirstBinSIndex;
    
    ++PtDestY[VBinUIndex];
    
  } /* End for */
  
  /* ------------------------ */
  /* Update Udf & Ovf         */
  /* ------------------------ */
  
  if ( PtUdf != NULL ) {
    *PtUdf = VUdfCnt;
  }
  
  if ( PtOvf != NULL ) {
    *PtOvf = VOvfCnt;
  }
  
  
  /* ------------------------ */
  /* Build X array content    */
  /* ------------------------ */
  
  for ( Vi=0; Vi < (VBinNb + 1); Vi++ ) {
    
    PtDestX[Vi] = VMin + ( Vi * VBinW );
    
  } /* End for */
  
  
  /* ------------------------ */
  /* Plot arrays              */
  /* ------------------------ */
  
  if ( DbgPlot ) {
    
    for ( Vi=0; Vi < VBinNb; Vi++ ) {
      msg (( MSG_OUT, "Bin [Index=%4d] [Range=%4d..%4d] Count = %4d", Vi, PtDestX[Vi], PtDestX[Vi+1], PtDestY[Vi] ));
    } /* End for */
    
    for ( Vi=0; Vi < VBinNb; Vi++ ) {
      msg (( MSG_OUT, "X[%4d]=%4d - Y[%4d]=%4d", Vi, PtDestX[Vi], Vi, PtDestY[Vi] ));
    } /* End for */
    
    
  } /* End if */
  
  return (VBinNb);
}



// 14/08/2007

SInt8 MATH_FChkUInt8Range ( float Data ) {
  
  if ( (Data < 0) || (Data > 255) ) {
    err_retfail ( -1, (ERR_OUT,"Data=%.3f is out of UInt8 range [0..255]") );
  }
  
  return (0);
}


// 14/08/2007

SInt8 MATH_FChkUInt16Range ( float Data ) {
  
  if ( (Data < 0) || (Data > 65535) ) {
    err_retfail ( -1, (ERR_OUT,"Data=%.3f is out of UInt8 range [0..65535]") );
  }
  
  return (0);
}

// 14/08/2007

SInt8 MATH_FChkUInt32Range ( float Data ) {
  
  if ( (Data < 0) || (Data > 0xFFFFFFFF) ) {
    err_retfail ( -1, (ERR_OUT,"Data=%.3f is out of UInt8 range [0..%d]", 0xFFFFFFFF) );
  }
  
  return (0);
}




// 14/08/2007

SInt8 MATH_FChkSInt8Range ( float Data ) {
  
  if ( (Data < -128) || (Data > 127) ) {
    err_retfail ( -1, (ERR_OUT,"Data=%.3f is out of UInt8 range [-128..127]") );
  }
  
  return (0);
}


// 14/08/2007

SInt8 MATH_FChkSInt16Range ( float Data ) {
  
  if ( (Data < -32768) || (Data > 32767) ) {
    err_retfail ( -1, (ERR_OUT,"Data=%.3f is out of UInt8 range [-32768..32767]") );
  }
  
  return (0);
}

// 14/08/2007

SInt8 MATH_FChkSInt32Range ( float Data ) {
  
  
#ifdef INT_MAX
  if ( (Data < (-INT_MAX-1)) || (Data > INT_MAX) ) {
    err_retfail ( -1, (ERR_OUT,"Data=%.3f is out of UInt8 range [%d..%d]", (-INT_MAX-1), INT_MAX) );
  }
#else
  if ( (Data < (-MaxInt-1)) || (Data > MaxInt) ) {
    err_retfail ( -1, (ERR_OUT,"Data=%.3f is out of UInt8 range [%d..%d]", (-MaxInt-1), MaxInt) );
  }
#endif
  
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




#endif

// msg (( MSG_OUT, "Msg" ));
