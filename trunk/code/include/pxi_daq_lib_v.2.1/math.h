
#ifndef MATH_H

#include "func_header.def"

// FHEAD (;)


FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 MATH_FSInt32MaxNo1 ( SInt32 Src, SInt8 StartSearch );)
FHEAD_DLL_IE (APP_DLL_IMPORT_EXPORT, SInt32 MATH_FSInt32MinNo1 ( SInt32 Src, SInt8 StartSearch );)


FHEAD ( SInt32 MATH_FCpySInt32ToSInt16Array ( SInt32* PtSrc, SInt32 SrcEltSz, SInt16* PtDest, SInt32 DestEltSz );)

FHEAD ( float MATH_FSInt32Avg ( SInt32* Src, SInt32 EltNb );)

FHEAD ( float MATH_FFloatAvg ( float* Src, SInt32 EltNb );)


FHEAD ( float MATH_FFloatMin ( float* Src, SInt32 EltNb );)
FHEAD ( float MATH_FFloatMax ( float* Src, SInt32 EltNb );)

FHEAD ( SInt32 MATH_FSInt32Min ( SInt32* Src, SInt32 EltNb );)
FHEAD ( SInt32 MATH_FSInt32Max ( SInt32* Src, SInt32 EltNb );)


FHEAD ( float MATH_FSInt32Sigma1 ( SInt32* Src, SInt32 EltNb, float Avg );)

FHEAD ( float MATH_FFloat32Sigma1 ( float* Src, SInt32 EltNb, float Avg );)

FHEAD ( void MATH_FDiffPedest ( SInt32* Src, float Pedest, float* Dest, SInt32 EltNb );)

FHEAD ( SInt32 MATH_FScaleFloatVect ( float* Src, float* Dest, SInt32 EltNb, float Offset, float Slope );)

FHEAD ( SInt32 MATH_FIntInRange   ( SInt32 Min, SInt32 Max, SInt32 Val );)
FHEAD ( SInt32 MATH_FFLoatInRange ( float  Min, float  Max, float  Val );)

FHEAD ( float MATH_UCONV_FVoltageGain2dB  ( float Gain   );)
FHEAD ( float MATH_UCONV_FdB2VoltageGain  ( float dB     );)
FHEAD ( float MATH_UCONV_FPowerGain2dB    ( float Gain   );)
FHEAD ( float MATH_UCONV_FdB2PowerGain    ( float dB     );)
FHEAD ( float MATH_UCONV_FV2dBV           ( float Volt   );)
FHEAD ( float MATH_UCONV_FdBV2V           ( float dBV    );)
FHEAD ( float MATH_UCONV_FmV2dBV          ( float mV     );)
FHEAD ( float MATH_UCONV_FdBV2mV          ( float dBV    );)

FHEAD ( float MATH_UCONV_FV2dBmV          ( float Volt   );)
FHEAD ( float MATH_UCONV_FdBmV2V          ( float dBmV   );)

FHEAD ( float MATH_UCONV_FV2dBm600        ( float Volt   );)
FHEAD ( float MATH_UCONV_FdBm600_2V       ( float dBm600 );)
FHEAD ( float MATH_UCONV_FmV2dBm600       ( float mV     );)
FHEAD ( float MATH_UCONV_FdBm50_2V        ( float dBm50  );)
FHEAD ( float MATH_UCONV_FV2dBm50         ( float Volt   );)
FHEAD ( float MATH_UCONV_FmV2dBm50        ( float mV     );)




#ifndef APP_DLL_IMPORT_EXPORT
  #ifndef MATH_H
    #define MATH_H
  #endif
#endif


#endif


