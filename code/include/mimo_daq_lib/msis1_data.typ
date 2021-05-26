
/**
* ----------------------------------------------------------------------------------
* \file              X:\lib\com\maps\msis1\data\msis1_data.typ
* \brief Goal      : Types definition of Mimosis 1 lib
* \brief
* \brief
* \version         : 1.0
* \date Prj date   : 03/05/2019
* \date File date  : 03/05/2019
* \date Doc date   : 03/05/2019
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



#ifndef MIMOSIS1_DATA_TYP
#define MIMOSIS1_DATA_TYP


#include "c4pi_test_team.h"
#include "msis1_data.def"
#include "globals_root.def"
#include "msg_light.h"

/**
===================================================================================
* \typedef MIS1__TMSis1DacList
*
* \brief Short explanation of typedef
*
* Also defined in app.typ => should be removed from app.typ
* ...
* ...
* ...
* can take many lines
*
* G.CLAUS 17/11/2020
*
===================================================================================
*/


typedef enum MIS1__EMSis1DacList {
  
  MIS1__DAC_IBIAS,      /*!<     */
  MIS1__DAC_ITHR,   /*!<  */
  MIS1__DAC_IDB,   /*!<  */
  MIS1__DAC_VRESET,   /*!<  */
  MIS1__DAC_VPL,   /*!<  */
  MIS1__DAC_VPH,   /*!<  */
  MIS1__DAC_VPH_FINE,   /*!<  */
  MIS1__DAC_VCASP,   /*!<  */
  MIS1__DAC_VCASNA,   /*!<  */
  MIS1__DAC_VCASNB,   /*!<  */
  MIS1__DAC_VCASNC,   /*!<  */
  MIS1__DAC_VCASND,   /*!<  */
  MIS1__DAC_VCASN2,   /*!<  */
  MIS1__DAC_VCLIP,   /*!<  */
  MIS1__DAC_IBUFBIAS,   /*!<  */
  MIS1__DAC_NB
  
} MIS1__TMSis1DacList;




/**
===================================================================================
* \struct MIS1__TFrameW128
*
* \brief Contains one frame as N x W128 
*
* Longer C structure explanation
* ...
* ...
* ...
* can take many lines
*
* G.CLAUS 26/07/2019
*
===================================================================================
*/


typedef struct {
  
  TW128As8W16 AW128[MIS1__DS_TOT_FRAME_SZ_W128]; /*!< Frame as an array of W128  */
  
} MIS1__TFrameW128;



/**
===================================================================================
* \struct MIS1__TDxErrCnt
*
* \brief Contains the data errors on each Dx (D0..D7), used by data checking functions
*
*
* G.CLAUS 14/11/2019
*
===================================================================================
*/


typedef struct {
  
  SInt32 AErrCnt[MIS1__MAX_OUT_NB]; /*!< Frame as an array of W128  */
  
} MIS1__TDxErrCnt;




/**
===================================================================================
* \union MIS1__TDsFrHeader
*
* \brief Data stream frame header
*
* \n - Fields W7 = W6 = W5 = W4 = 0xFE00, W3H = W2H = W1H = W0H = 0xFE
* \n - Frames counter can be built from fields FC0, FC1, FC2, FC3, FC0 = B0B7, FC1 = B24B31
*
* G.CLAUS 07/05/2019
*
===================================================================================
*/

typedef union {
  
  UInt16 AW16[MIS1__DS_FR_HEADER_SZ_W16]; /*!< Frame header seen as an array of W16 */
  
  struct
  {
    UInt8  FC0; /*!< Frame counter B00..B07, access via F.FC0 */
    UInt8  W0H; /*!< B08..B15 of W16 no 0 = LSW, access via F.W0H */
    
    UInt8  FC1; /*!< Frame counter B08..B15, access via F.FC1 */
    UInt8  W1H; /*!< B08..B15 of W16 no 1 = LSW, access via F.W1H */

    UInt8  FC2; /*!< Frame counter B16..B23, access via F.FC2 */
    UInt8  W2H; /*!< B08..B15 of W16 no 2 = LSW, access via F.W2H */

    UInt8  FC3; /*!< Frame counter B24..B31, access via F.FC3 */
    UInt8  W3H; /*!< B08..B15 of W16 no 3 = LSW, access via F.W3H */
    
    UInt16 W4; /*!< W16 no 4, access via F.W4 */
    UInt16 W5; /*!< W16 no 5, access via F.W5 */
    UInt16 W6; /*!< W16 no 6, access via F.W6 */
    UInt16 W7; /*!< W16 no 7, access via F.W7 */
  }	F;
  
} MIS1__TDsFrHeader;


/**
===================================================================================
* \union MIS1__TDsRegHeader
*
* \brief Data stream region header
*
*
* G.CLAUS 07/05/2019
*
===================================================================================
*/

typedef union {
  
  // UInt16 AW16[MIS1__DS_REG_HEADER_SZ_W16]; /*!< Region header seen as an array of W16 */
  
  UInt16 W16; /*!< Region header as one W16 */
  
  struct
  {
    UInt16 Reg  : 6; /*!< Region id, access via F.Reg */
    UInt16 Zero : 2; /*!< Two bits at 0, access via F.Zero */
    UInt16 Tag  : 8; /*!< Region header tag = 0xFD, access via F.Tag */
  }	F;
  
} MIS1__TDsRegHeader;


/**
===================================================================================
* \union MIS1__TDsPixData
*
* \brief Data stream checksun
*
*
* G.CLAUS 14/05/2019
*
===================================================================================
*/

typedef union {
  
  UInt16 W16; /*!< Pixels data as one W16 */
  
  struct
  {
    UInt16 Code         :  3; /*!< Code = state of 3 pixels following first one, access via F.Code*/
    UInt16 PeAddr       :  3; /*!< Priority encoder address, access via F.PeAddr */
    UInt16 FirstPixAddr : 10; /*!< Address of first pixel 0..503 = No pixel, it is not the PE/pixelline ! \n access via F.FirstPixAddr */
  }	F;
  
} MIS1__TDsPixData;




/**
===================================================================================
* \union MIS1__TDsCheckSum
*
* \brief Data stream checksun
*
*
* G.CLAUS 07/05/2019
*
===================================================================================
*/

typedef union {
  
  UInt16 W16; /*!< Checksum as one W16 */
  
  struct
  {
    UInt16 Low  : 8; /*!< Checksum low byte B0B7, access via W8.Low */
    UInt16 High : 8; /*!< Checksum high byte B8B15, access via W8.High */
  }	W8;
  
} MIS1__TDsCheckSum;



/**
===================================================================================
* \union MIS1__TDsFrTrailer
*
* \brief Data stream frame trailer
*
*
* G.CLAUS 07/05/2019
*
===================================================================================
*/

typedef union {
  
  UInt16 W16; /*!< Frame trailer seen as one W16 */
  
  struct
  {
    UInt8 Flags : 8; /*!< Frame trailer flags, access via F.Flags */
    UInt8 Tag   : 8; /*!< Frame trailer tag = 0xFF, access via F.Tag */
  }	F;
  
  struct
  {
    UInt16 FrOvf     : 1; /*!< Frameo verflow,access via B.FrOvf */
    UInt16 RegOvf    : 1; /*!< Region overflow, access via B.RegOvf */
    UInt16 SRegOvf   : 1; /*!< Super region overflow, access via B.SRegOvf */
    UInt16 FSLimit   : 1; /*!< Frame size limit active, access via B.FSLimit */
    UInt16 Bit4      : 1; /*!< NU, access via B.Bit4 */
    UInt16 Bit5      : 1; /*!< NU, access via B.Bit5 */
    UInt16 Bit6      : 1; /*!< NU, access via B.Bit6 */
    UInt16 Bit7      : 1; /*!< NU, access via B.Bit7 */
    
    UInt16 Bit8      : 1; /*!< Bit of tag field, access via B.Bit8  */
    UInt16 Bit9      : 1; /*!< Bit of tag field, access via B.Bit9  */
    UInt16 Bit10     : 1; /*!< Bit of tag field, access via B.Bit10 */
    UInt16 Bit11     : 1; /*!< Bit of tag field, access via B.Bit11 */
    UInt16 Bit12     : 1; /*!< Bit of tag field, access via B.Bit12 */
    UInt16 Bit13     : 1; /*!< Bit of tag field, access via B.Bit13 */
    UInt16 Bit14     : 1; /*!< Bit of tag field, access via B.Bit14 */
    UInt16 Bit15     : 1; /*!< Bit of tag field, access via B.Bit15 */
    
  }	B;
  
  
} MIS1__TDsFrTrailer;


/**
===================================================================================
* \struct MIS1__TDsFrEwChkSumTrailer
*
* \brief Mimosis 1 data stream frame trailer = Empty words + Check sum + Trailer
*
* It contains
* \n - Frame information generated by HW, FW or SW
* \n -- HW = generated by Mimosis, like HwFrNo
* \n -- FW = generated by FW, like FwTimeStamp
* \n -- SW = generated by SW, like SwFrNo
* \n
* \n - Pointer on data stream fields
*
* G.CLAUS 06/05/2019
*
===================================================================================
*/


typedef union {
  
  UInt16 AW16[8];         /*!< AW16[7] .. AW16[2] = Empty word = 0xFCAA, AW16[1] = Check sum, AW16[0] = Trailer = Flags */
  
  struct {
  
    MIS1__TDsFrTrailer Trailer; /*!< Trailer = Flags */

    MIS1__TDsCheckSum CheckSum;  /*!< Checksum */
    
    UInt16 AEmptyW16[6]; 
  
  } F;
  
} MIS1__TDsFrEwChkSumTrailer;



typedef struct {
  
  UInt16 AW16[8];         /*!< AW16[7] .. AW16[2] = Empty word = 0xFCAA, AW16[1] = Check sum, AW16[0] = Trailer = Flags */
  
  MIS1__TDsCheckSum CheckSum;  /*!< Checksum */
  
  MIS1__TDsFrTrailer Trailer; /*!< Trailer = Flags */
  
} MIS1__TDsFrEwChkSumTrailer___OLD;


/**
===================================================================================
* \struct MIS1__TDsFr
*
* \brief Mimosis 1 data stream frame stucture
*
* It contains 
* \n - Frame information generated by HW, FW or SW
* \n -- HW = generated by Mimosis, like HwFrNo
* \n -- FW = generated by FW, like FwTimeStamp
* \n -- SW = generated by SW, like SwFrNo
* \n
* \n - Pointer on data stream fields
*
* G.CLAUS 06/05/2019
*
===================================================================================
*/

typedef struct {
  
  void*  PtPrevFr;    /*!< Pointer on previous frame, should be casted to MIS1__TDsFr*  */
  void*  PtNextFr;    /*!< Pointer on next frame, should be casted to MIS1__TDsFr*      */
  
  UInt32 SwDataSrc;   /*!< Data source = No of Mimosis 1, etc, W32 available   */
  UInt32 FwTimeStamp; /*!< Frame time stamp generated by FW, resolution to be defined <= 5 us (5 us => 238 days) */
  UInt32 HwFrNo;      /*!< Index of the frame generated by Mimosis 1, starting at 0 */
  UInt32 SwFrNo;      /*!< Index of the frame generated by SW, starting at 0 */
  
  MIS1__TDsFrHeader*   PtFrHead;                              /*!< Pointer on frame header */
  
  MIS1__TDsRegHeader*  APtRegHead[MIS1__DS_MAX_REGION_NB];    /*!< Array of pointers on regions headers */
  
  UInt16*              APtRegData[MIS1__DS_MAX_REGION_NB];     /*!< Array of pointers on regions data */
  
  MIS1__TDsCheckSum*   PtCheckSum;
  
  MIS1__TDsFrTrailer*  PtFrTrailer;
  
} MIS1__TDsFr;


// ===================================================================================
// * Mimosis 1 emulation (comment not processed by DOXYGEN)
// *
// ===================================================================================


/**
===================================================================================
* \union MIS1__TPixXY
*
* \brief Mimosis 1 pixel coordinates union, mainly used for emulation
*
*
* G.CLAUS 22/05/2019
*
===================================================================================
*/


#ifdef MIS1__CC_DATA_FORMAT_SINCE_V211

  typedef union {
    
    SInt16 Axy[2]; /*!< Pixel coordinates array [0] = x, [1] = y */
    
    struct {
      SInt16 x; /*!< Pixel coordinates structure C.x, C.y */
      SInt16 y;
    } C;
    
  } MIS1__TPixXY;
  
#else

  typedef union {
    
    SInt32 Axy[2]; /*!< Pixel coordinates array [0] = x, [1] = y */
    
    struct {
      SInt32 x; /*!< Pixel coordinates structure C.x, C.y */
      SInt32 y;
    } C;
    
  } MIS1__TPixXY;
  

#endif
  
/**
===================================================================================
* \struct MIS1__TDecFrPixHeader
*
* \brief Mimosis 1 frame fired pixels list
*
*
* G.CLAUS 08/06/2019
*
===================================================================================
*/

#ifdef MIS1__CC_DATA_FORMAT_SINCE_V211

  typedef struct {
    
    UInt16 AcqIdInBuff;     /*!< AcqId <= 65535             */
    UInt16 FrIdInBuff;      /*!< FrId <= 65535              */
    UInt16 PixNb;        /*!< Number of pixels           */
    UInt32 MSis1FrCnt;   /*!< Frame counter from MSis 1  */
    UInt16 MSis1ChkSum;  /*!< Check sum from MSis 1      */
    UInt16 MSis1Flags;   /*!< Flags from MSis 1          */  
    
  } MIS1__TDecFrPixHeader;
  

#else

  typedef struct {
    
    UInt16 AcqIdInBuff;     /*!< AcqId <= 65535             */
    UInt16 FrIdInBuff;      /*!< FrId <= 65535              */
    UInt16 PixNb;        /*!< Number of pixels           */
    UInt32 MSis1FrCnt;   /*!< Frame counter from MSis 1  */
    UInt16 MSis1ChkSum;  /*!< Check sum from MSis 1      */
    UInt16 MSis1Flags;   /*!< Flags from MSis 1          */
    
    UInt32 Padding1;
    UInt32 Padding2;    
    
  } MIS1__TDecFrPixHeader;
  

#endif


/**
===================================================================================
* \struct MIS1__TDecFrPix
*
* \brief Mimosis 1 frame fired pixels list
*
*
* G.CLAUS 08/06/2019
*
===================================================================================
*/

typedef struct {

  MIS1__TDecFrPixHeader H; /*!< Header */

  MIS1__TPixXY APix[MIS1__FRAME_PIX_LIST_MAX_PIX_NB]; /*!< List of pixels in each frame */
  
} MIS1__TDecFrPix;






/**
===================================================================================
* \struct MIS1__TPixXYS
*
* \brief Mimosis 1 pixel coordinates + state struct, mainly used for emulation
*
*
* G.CLAUS 22/05/2019
*
===================================================================================
*/

typedef struct {
  
  MIS1__TPixXY PixXY; /*!< Pixel coordinates MIS1__TPixXY */
  
  SInt32 PixState;        /*!< Pixel state 0 / 1 or more (S32) => Can be used to count hits */
    
} MIS1__TPixXYS;


/**
===================================================================================
* \struct MIS1__TPixXYSP
*
* \brief Mimosis 1 pixel coordinates + state + PE info struct, mainly used for emulation
*
*
* G.CLAUS 22/05/2019
*
===================================================================================
*/

typedef struct {
  
  MIS1__TPixXYS PixXYS; /*!< Pixel coordinates + state MIS1__TPixXYS */
  
  SInt16 Region;         /*!< Region No 0..63 */
  SInt16 SRegion;        /*!< Super region No 0..15 */
  
  SInt16 PeNoInMatrix;   /*!< Priority encoder No in matrix       => 0..MIS1__PE_NB-1       = 511  */
  SInt16 PeNoInRegion;   /*!< Priority encoder No in region       => 0..MIS1__PE_PER_REG-1  = 7    */
  SInt16 PeNoInSRegion;  /*!< Priority encoder No in super region => 0..MIS1__PE_PER_SREG-1 = 31   */
  SInt16 PeRow;          /*!< Priority encoder row                => 0..MIS1__ROW_NB-1      = 503  */
  SInt16 PeAddr;         /*!< Priority encoder addr */
  
} MIS1__TPixXYSP;


/**
===================================================================================
* \typedef MIS1__TMatPixS32
*
* \brief Mimosis 1 pixels matrix as bit (in fact SInt32) Mat[x,y], mainly used for emulation and plot
*
*
* G.CLAUS 22/05/2019
*
===================================================================================
*/

// Two types definition because src editor can't dectect "array typedef"

typedef SInt32 MIS1__TTMatPixS32[MIS1__COL_NB][MIS1__ROW_NB] ;

typedef MIS1__TTMatPixS32 MIS1__TMatPixS32;


/**
===================================================================================
* \typedef MIS1__TMatPixFloat
*
* \brief Mimosis 1 pixels matrix as bit (in fact SInt32) Mat[x,y], mainly used for emulation and plot
*
*
* G.CLAUS 17/09/2020
*
===================================================================================
*/

// Two types definition because src editor can't dectect "array typedef"

typedef float MIS1__TTMatPixFloat[MIS1__COL_NB][MIS1__ROW_NB] ;

typedef  MIS1__TTMatPixFloat  MIS1__TMatPixFloat;



/**
===================================================================================
* \typedef MIS1__TMatScanFloat
*
* \brief Mimosis 1 
*
*
* G.CLAUS 30/09/2020
*
===================================================================================
*/

// Two types definition because src editor can't dectect "array typedef"

typedef float MIS1__TTMatScanFloat[MIS1__COL_NB][MIS1__ROW_NB][MIS1__CAR_MAX_STEP_NB];

typedef  MIS1__TTMatScanFloat  MIS1__TMatScanFloat;



/**
===================================================================================
* \typedef MIS1__TRecResScanPar
*
* \brief Mimosis 1
*
*
* G.CLAUS 24/11/2020
*
===================================================================================
*/



typedef struct {
  
  // -------------------------------
  // Parameters used for the scan
  // -------------------------------
  
  UInt32 ParFirstStep; // First val of scan reg in LSB
  UInt32 ParLastStep;  // Last val of scan reg in LSB
  UInt32 ParStepNb;    // Step nb of the scan
  
  UInt32 ParFrNb;
  
  UInt32 AParScanRegVal[MIS1__CAR_MAX_STEP_NB];  // 16/11/2020
  
  UInt32  ParADac[MIS1__DAC_NB];
  
  
  char   ParMatrixPulsed; // A, B, C, D
  
  UInt32 ParFirstRowPulsed;
  UInt32 ParLastRowPulsed;
  UInt32 ParFirstColPulsed;
  UInt32 ParLastColPulsed;
  
  UInt32 ParRowNbPulsedTogether; // Nb of rows pulsed at the same time
  
  UInt32  ParPixPulseAA;
  UInt32  ParPixPulseAB;
  
  
  char   ParScConfFile[GLB_FILE_PATH_SZ];
  
  SInt8  ParFileFormat; // 0 => U16, 1 => S32
  
  // -------------------------------
  // Parameters used for analysis
  // -------------------------------
  
  // Fired count in % used for calculation of AResScanRegValForLowPCent, AResScanRegValForHighPCent, AResScanRegValForMidPCent
  // The results ScanRegValLow, Mid, High are used to find if scan has been done in good dynamic range or if s-curves are cut
  
  UInt32 ParLowPCent;
  UInt32 ParMidPCent;
  UInt32 ParHighPCent;
  
  
  
} MIS1__TRecResScanPar;


/**
===================================================================================
* \typedef MIS1__TRecResScanPar
*
* \brief Mimosis 1
*
*
* G.CLAUS 24/11/2020
*
===================================================================================
*/



typedef struct {
  
  // -------------------------------
  // Results of scan
  // -------------------------------
  
  MIS1__TMatScanFloat ResMat;    // Matrix with fired count in % for each step
  
  
  float APCentMin [MIS1__COL_NB][MIS1__ROW_NB]; // Min value of % for each pixel, should be 0 %
  float APCentMax [MIS1__COL_NB][MIS1__ROW_NB]; // Max value of % for each pixel, should be<= 100 %
  
  float AResScanRegValForLowPCent [MIS1__COL_NB][MIS1__ROW_NB];  // Scan parameter value for low fired count %, example for 10 %
  float AResScanRegValForHighPCent[MIS1__COL_NB][MIS1__ROW_NB];  // Scan parameter value for high fired count %, example for 90 %
  float AResScanRegValForMidPCent [MIS1__COL_NB][MIS1__ROW_NB];  // Scan parameter value for mid fired count %, example for 50 %
  

} MIS1__TRecResScanRes;


/**
===================================================================================
* \typedef MIS1__TRecResScanPar
*
* \brief Mimosis 1
*
*
* G.CLAUS XX/10/2020
*
===================================================================================
*/


typedef struct {
  
  // WARNING => It is nor allowed to add fields here !!!
  //
  // if more fields are needed => add them in MIS1__TRecResScanPar or MIS1__TRecResScanRes
  //
  // because record can be saved as "P+R" or only "P" in order to save disk space if R (results) are not needed
  
  MIS1__TRecResScanPar P; // Run / scan parameters
  MIS1__TRecResScanRes R; // Run / scan results : pixels % for each scan step, etc ...
  
  
} MIS1__TRecResScan;



typedef struct {
  
  // -------------------------------
  // Parameters used for the scan
  // -------------------------------
  
  UInt32 ParFirstStep;
  UInt32 ParLastStep;
  UInt32 ParStepNb;
  
  UInt32 ParFrNb;
  
  UInt32 AParScanRegVal[MIS1__CAR_MAX_STEP_NB];  // 16/11/2020
  
  UInt32  ParADac[MIS1__DAC_NB];
  
  
  char   ParMatrixPulsed; // A, B, C, D
  
  UInt32 ParFirstRowPulsed;
  UInt32 ParLastRowPulsed;
  UInt32 ParFirstColPulsed;
  UInt32 ParLastColPulsed;
  
  UInt32  ParPixPulseAA;
  UInt32  ParPixPulseAB;


  char   ParScConfFile[GLB_FILE_PATH_SZ];
  
  
  
  // -------------------------------
  // Parameters used for analysis
  // -------------------------------
  
  // Fired count in % used for calculation of AResScanRegValForLowPCent, AResScanRegValForHighPCent, AResScanRegValForMidPCent
  // The results ScanRegValLow, Mid, High are used to find if scan has been done in good dynamic range or if s-curves are cut
  
  UInt32 ParLowPCent;
  UInt32 ParMidPCent;
  UInt32 ParHighPCent;
  
  // -------------------------------
  // Results of scan
  // -------------------------------
    
  MIS1__TMatScanFloat ResMat;    // Matrix with fired count in % for each step
  
 
  float APCentMin [MIS1__COL_NB][MIS1__ROW_NB]; // Min value of % for each pixel, should be 0 %
  float APCentMax [MIS1__COL_NB][MIS1__ROW_NB]; // Max value of % for each pixel, should be<= 100 %
  
  float AResScanRegValForLowPCent [MIS1__COL_NB][MIS1__ROW_NB];  // Scan parameter value for low fired count %, example for 10 %
  float AResScanRegValForHighPCent[MIS1__COL_NB][MIS1__ROW_NB];  // Scan parameter value for high fired count %, example for 90 %
  float AResScanRegValForMidPCent [MIS1__COL_NB][MIS1__ROW_NB];  // Scan parameter value for mid fired count %, example for 50 %
  
  
} MIS1__TRecResScan__old;



/**
===================================================================================
* \typedef MIS1__TMatPixXYSP
*
* \brief Mimosis 1 pixels matrix MIS1__TMatPixXYSP, Mat[x,y], mainly used for emulation and plot
*
*
* G.CLAUS 22/05/2019
*
===================================================================================
*/

// Two types definition because src editor can't dectect "array typedef"


typedef MIS1__TPixXYSP MIS1__TTMatPixXYSP[MIS1__COL_NB][MIS1__ROW_NB];

typedef MIS1__TTMatPixXYSP MIS1__TMatPixXYSP;



#ifndef CC_MSIS1_BDF_LIGHT


/**
===================================================================================
* \typedef MIS1__TRecInfWarnErr
*
* \brief Mimosis 1
*
*
* G.CLAUS 24/02/2021
*
===================================================================================
*/



typedef struct {
  
  SInt8  MSisId;
  UInt32 RunId;
  UInt32 AcqId;
  UInt32 FrIdInAcq;
  UInt16 StepId;
  UInt16 SubStepId;
  
  UInt8  Type; // Warning, error type
               //
               // MIS1__WE_RECORD_EMPTY  /*!< Record is emmpty = not used  */
               // MIS1__WE_SW_ERR,        /*!< SW error  */
               // MIS1__WE_SW_REG_OVF,    /*!< SW region overflow  */
               // MIS1__WE_SW_ACQ_TRUNC,  /*!< SW acq truncated  */
               // MIS1__WE_MSIS_FR_OVF    /*!< MSis 1 frame overflow (flags bits of trailer field)  */
               
                
  
  UInt32 MSisFrCnt; // MSis frame counter of the frame on which the OVF has occured
  
  SInt32 Cnt;  // Warning, error counter
  SInt32 Code; // Warning, error code
  SInt32 Val;  // Warning, error value 
  
} MIS1__TWarnErr;



/**
===================================================================================
* \typedef MIS1__TRecScanStatusHeader
*
* \brief Mimosis 1
*
*
* G.CLAUS 24/02/2021
*
===================================================================================
*/



typedef struct {
  
  char Comment[GLB_CMT_SZ];
  
  
} MIS1__TRecScanStatusHeader;


/**
===================================================================================
* \typedef MIS1__TRecScanStatus
*
* \brief Mimosis 1
*
*
* G.CLAUS 24/02/2021
*
===================================================================================
*/



typedef struct {
  
  MIS1__TRecScanStatusHeader;
  
  MIS1__TWarnErr AWarnErr[MIS1__MAX_WARN_ERR_REC];
  
} MIS1__TRecScanStatus;



/**
===================================================================================
* \typedef MIS1__TMatPixCntU16
*
* \brief Mimosis 1 fired pixels count matrix 
*
*
* G.CLAUS 03/06/2020
*
===================================================================================
*/

// Two types definition because src editor can't dectect "array typedef"

typedef UInt16 MIS1__TTMatPixCntU16[MIS1__COL_NB][MIS1__ROW_NB];

typedef MIS1__TTMatPixCntU16 MIS1__TMatPixCntU16;

/**
===================================================================================
* \struct MIS1__TPe
*
* \brief Mimosis 1 priority encoder structure, mainly used for emulation
*
*
* G.CLAUS 14/05/2019
*
===================================================================================
*/

typedef struct {
  
  SInt8 AColLeft[MIS1__ROW_NB];  /*!< State (0/1) of pixels in left column of PE */
  SInt8 AColRight[MIS1__ROW_NB]; /*!< State (0/1) of pixels in right column of PE */
    
} MIS1__TPe;


/**
===================================================================================
* \struct MIS1__TReg
*
* \brief Mimosis 1 region structure, mainly used for emulation
*
*
* G.CLAUS 14/05/2019
*
===================================================================================
*/

typedef union {
  
  MIS1__TPe APe[MIS1__PE_PER_REG]; /*!< Priority encodeurs array - 8 PE / region */
  
  SInt8 AAPix[MIS1__COL_PER_REG][MIS1__ROW_NB]; /*!< Pixels array AAPix[x,y]  */
    
} MIS1__TReg;



// ===================================================================================
// * Union example (comment not processed by DOXYGEN)
// *
// ===================================================================================


/**
===================================================================================
* \union MIS1__TU8Union
*
* \brief Short explanation of C union
*
* Longer C union explanation
* ...
* ...
* ...
* can take many lines
*
* G.CLAUS 11/04/2019
*
===================================================================================
*/

typedef union {
  
 UInt8 w8; /*!< Full byte, access via w8 */
 
 struct
 {
   UInt8 b0 		: 1; /*!< Bit0, access via b.b0 */
   UInt8 b1 		: 1; /*!< Bit1, access via b.b1 */
   UInt8 b2 		: 1; /*!< Bit2, access via b.b2 */
   UInt8 b3 		: 1; /*!< Bit3, access via b.b3 */
   UInt8 b4 		: 1; /*!< Bit4, access via b.b4 */
   UInt8 b5 		: 1; /*!< Bit5, access via b.b5 */
   UInt8 b6 		: 1; /*!< Bit6, access via b.b6 */
   UInt8 b7 		: 1; /*!< Bit7, access via b.b7 */
  }	b;
 
} MIS1__TU8Union;


/**
===================================================================================
* \typedef MIS1__TDsTag
*
* \brief Type MIS1__TDsTag = MIS1__EDsTag
*
*
* G.CLAUS 06/05/2019
*
===================================================================================
*/

typedef MIS1__EDsTag MIS1__TDsTag;


/**
===================================================================================
* \typedef MIS1__TAcqFrList
*
* \brief Type MIS1__TAcqFrList = Acquisition frames pointers list
*
*
* G.CLAUS 08/04/2020
*
===================================================================================
*/

typedef struct {

  SInt32  ResAcqId;                               // Id of acquisition, field added on 12/05/2020
  UInt32  ResAcqSz;                               // Size of acquisition in W8, field added on 12/05/2020
  
  // UInt16  ResFrNb;                                // Frames number calculated by frames listing operation, should be equal to VReqFrNbPerAcq requested by user
  UInt32  ResFrNb;                                  // 12/05/2020 UInt16 => UInt32 - Frames number calculated by frames listing operation, should be equal to VReqFrNbPerAcq requested by user
  
  void*   ResPtAcqBeg;                            // Pointer on beginning of acq memory block
  void*   ResAPtFrHead[MIS1__MAX_FR_NB_PER_ACQ];  // Array of pointers on headers (available after frame listing execution)
  void*   ResAPtFrData[MIS1__MAX_FR_NB_PER_ACQ];  // Array of pointers on data parts (available after frame listing execution)
  void*   ResAPtFrTrail[MIS1__MAX_FR_NB_PER_ACQ]; // Array of pointers on trailers (available after frame listing execution)
  
} MIS1__TAcqFrList;


/**
===================================================================================
* \typedef MIS1__TAcqFrMap
*
* \brief Type MIS1__TAcqFrMap = Mapping of frames in one acq = offset in W8 / beginning
*
*
* G.CLAUS 08/04/2020
*
===================================================================================
*/


typedef struct {

  SInt32  ResAcqId;                               // Id of acquisition, field added on 12/05/2020
  UInt32  ResAcqSz;                               // Size of acquisition in W8, field added on 12/05/2020
  
  // UInt16  ResFrNb;                               // Frames number calculated by frames listing operation, should be equal to VReqFrNbPerAcq requested by user
  UInt32  ResFrNb;                                  // 12/05/2020 UInt16 => UInt32 - Frames number calculated by frames listing operation, should be equal to VReqFrNbPerAcq requested by user
  
  void*    ResPtAcqBeg;  // Pointer on beginning of acq memory block
  
  UInt32   ResAcqPos;                              // Position of Acq / Beginning of file in W8  
  UInt32   ResAPosFrHead[MIS1__MAX_FR_NB_PER_ACQ];  // Array of headers position in W8 / beginning of Acq (available after frame listing execution)
  UInt32   ResAPosFrData[MIS1__MAX_FR_NB_PER_ACQ];  // Array of data parts position in W8 / beginning of Acq (available after frame listing execution)
  UInt32   ResAPosFrTrail[MIS1__MAX_FR_NB_PER_ACQ]; // Array of trailers position in W8 / beginning of Acq (available after frame listing execution)
  
} MIS1__TAcqFrMap;




/**
===================================================================================
* \typedef MIS1__TDecFrList
*
* \brief Type MIS1__TDecFrList = Decoded data frames list
*
*
* G.CLAUS 09/06/2020
*
===================================================================================
*/

typedef struct {
  
  SInt32  ResAcqIdInRun;      // Id of acquisition
  SInt16  ResFrNbInAcq;    // Frames nb in the acq which contains this frame
  SInt32  ResFrIdInAcq;  // Id frame in its acquisition 0 to max fr nb per acq  - 1
  UInt32  ResFrSz;       // Size of frame in W8
  
  void*   ResPtFrBeg; 
     
} MIS1__TDecFrList;





/**
===================================================================================
* \typedef MIS1__TDecFrMap
*
* \brief Type MIS1__TDecFrMap = Decoded data frames mapping
*
*
* G.CLAUS 09/06/2020
*
===================================================================================
*/

typedef struct {
  
  SInt32  ResAcqIdInRun;   // Id of acquisition
  SInt16  ResFrNbInAcq;    // Frames nb in the acq which contains this frame
  SInt32  ResFrIdInAcq;    // Id frame in its acquisition 0 to max fr nb per acq  - 1
  UInt32  ResFrSz;         // Size of frame in W8
  
  void*   ResPtFrBeg;
  
  UInt32  ResFrPos; // Position of fr / Beginning of file in W8
  
} MIS1__TDecFrMap;




/* ================================================== */
/* 15/05/2020 moved from app.typ                      */
/* 06/04/2020 definition in app.typ                   */
/* ================================================== */


// Enum for data processing mode

typedef enum {
  
  PM_NONE,
  PM_DECODE_FR,
  PM_DECODE_FR_CNT_PIX,
  PM_EMUL_DECODE_FR_DAQ_TEST,
  PM_EMUL_DECODE_FR_FPC_DAQ_TEST,
  PM_MODE_NB
  
} MIS1__TProcMode;


/* ================================================== */
/* 15/05/2020 moved from app.typ                      */
/* 06/04/2020 definition in app.typ                   */
/* ================================================== */


// Enum for FPC saving mode mode
// WARNING => GUI control list must be in the same order as below

typedef enum {
  
  MIS1__SFPC_NONE,
  MIS1__SFPC_SUB_STEPS,
  MIS1__SFPC_STEPS,
  MIS1__SFPC_ALL,
  MIS1__SFPC_NB
  
} MIS1__TSaveFpc;




/* ================================================== */
/* 15/05/2020 moved from app.typ                      */
/* 06/04/2020 definition in app.typ                   */
/* ================================================== */

// Run parameters

typedef struct {
  
  
  // --------------------------------------------------------------
  // File format, is here because it CAN'T be in MIS1__TRunConf
  // --------------------------------------------------------------
  
  UInt32 FileFormat; // MUST be equal to MIS1__RUN_CONF_FFORMAT_TAG
  
  // --------------------------------------------------------------
  // Run parameters
  // --------------------------------------------------------------
  
  SInt8  ParAllowRc; // Allow remote control, 0 => No, 1 => Yes
  
  // Most of fields have a clear name which doesn't need explanation
  
  UInt32 ParRunNo;
  UInt32 ParTotFrNb;
  UInt32 ParFrNbPerAcq;
  UInt32 ParFrNbPerFile;  // Field not used on 17/06/2020, set to 0 by GUI controls capture function
  UInt32 ParAcqNbPerFile;
  UInt32 ParBufferedAcqNb;
  
  // Processing mode
  // - PM_NONE
  // - PM_DECODE_FR
  // - PM_DECODE_FR_CNT_PIX
  // - PM_EMUL_DECODE_FR_DAQ_TEST
  // - PM_EMUL_DECODE_FR_FPC_DAQ_TEST
  
  
  MIS1__TProcMode  ParProcMode;
  
  SInt8            ParProcPrintLvl;
    
  // Saving modes
  
  SInt8  ParSaveRaw; // Save raw data
  SInt8  ParSaveDec; // Save decode data
  SInt8  ParSaveFPC; // Save fired pixels count
  
  SInt8  ParDecFormat; // Decoded data format, only value 0 now = pixels list
  
  SInt8  ParProc; // Processing enabled = decode data, fired pixels count, etc ...
  
  SInt8  ParRunForever;  // 0 => Run stops when TotFrNb is reached,
  // 1 => Run continues after TotFrNb until "Stop Run" button is pressed
  
  char   ParRunPrefix[GLB_FILE_PATH_SZ];
  char   ParRunRootDir[GLB_FILE_PATH_SZ];

  // --------------------------------------------------------------
  // Characterization parameters
  // --------------------------------------------------------------
  
  SInt8  ParCarMode;          // Characterisation mode
                              // The hereafter parameters are used one if ParCarMode == 1
                              // they are also hidden in MIS1__FRunParamInfPrint () if ParCarMode ==0
                              //
                              // 0 => Normal run
                              // 1 => Characterization procedure
                              // - each file of a run corresponds to a characterization step
                              // - two modes
                              // - one file = scan of full matrix for one threshold, all sub steps are stored in the file
                              // this is the case for FPC data
                              // - one file = scan of a matrix fraction for one threshold, one sub step
                              // this is the case for RAW or DEC data
      
   

  SInt8  ParCarStepNb;        // Number of pulse / threshold steps for the S curve scan
  SInt16 ParCarSubStepNb;     // Number of substep = measure of a fraction of the matrix ( max 800 pixels)
  
  SInt16 SubStepPixConfId;    // Configuration used for pixels selection for characterization
  
  SInt16 ParCarSubStepFrNb;   // Frames nb required to get "good results" = fr nb acquired for each sub step
                              // the values of ParSubStepAcqNb, ParFrNbPerAcq will be set to get ParCarSubStepFrNb
                              // with best efficiency
                            
  SInt16 ParCarSubStepAcqNb;  // Number of acq for each sub step, please read ParCarSubStepFrNb comment
  SInt16 ParCarFrNbPerAcq;    // Number of frames per acq, please read ParCarSubStepFrNb comment
  
  UInt16 ParCarPulsFirst;     // First pulse level used for S curve scan
  UInt16 ParCarPulsStep;      // Pulse step level used for S curve scan
  UInt16 ParCarPulsLast;      // Last pulse level used for S curve scan

  
  UInt16 ParCarAAStepBias [MIS1__CAR_MAX_STEP_NB][MSIS1__CAR_NB_MAX_BIAS_REG]; // Bias registers to configure for each thresholdscan step
  
  SInt16 ParCarSubStepConfId[MIS1__CAR_MAX_SUB_STEP_NB]; // Selected pixels configuration for each sun step

                    
  
} MIS1__TRunPar;



/* ================================================== */
/* 15/05/2020 moved from app.typ                      */
/* 06/04/2020 definition in app.typ                   */
/* ================================================== */

// Info variables for run, data taking, ...

typedef struct {
  
  // Info, tmp variables, etc ...
  
  // --------------------------------------------------------------
  // Info
  // --------------------------------------------------------------
  
  SInt8  InfSave; // Data will be saved InfSave = ParSaveRaw || ParSaveDec || ParSaveFPC
    
  UInt32 InfSaveFileNo;
  
  char   InfRunDir[GLB_FILE_PATH_SZ];            // Run directory = ParRunRootDir + "Run No"
  char   InfSCFileName[GLB_FILE_PATH_SZ];        // File path of slow control config file
  char   InfRunConfFileName[GLB_FILE_PATH_SZ];   // File path of run config file
  char   InfRunStatusFileName[GLB_FILE_PATH_SZ]; // File path of run status file
  
  char   InfRawIndexFileName[GLB_FILE_PATH_SZ];  // File path of raw data index file
  char   InfRawDataFileName[GLB_FILE_PATH_SZ];   // File path of raw data file

  char   InfDecIndexFileName[GLB_FILE_PATH_SZ];       // File path of decoded data file
  char   InfDecDataFileName[GLB_FILE_PATH_SZ];       // File path of decoded data file
  
  char   InfFPCSubStepsFileName[GLB_FILE_PATH_SZ];   // File path of fired pixels count sub steps data file
  char   InfFPCStepFileName[GLB_FILE_PATH_SZ];       // File path of fired pixels count step data file

  char   InfCurFPCSubStepsFileName[GLB_FILE_PATH_SZ];   // Current file path of fired pixels count sub steps data file
  char   InfCurFPCStepFileName[GLB_FILE_PATH_SZ];       // Current file path of fired pixels count step data file
  
} MIS1__TRunInf;


/* ================================================== */
/* 15/05/2020 moved from app.typ                      */
/* 06/04/2020 definition in app.typ                   */
/* ================================================== */

// Run status

typedef struct {
  
  UInt32 FileFormat; // MUST be equal to MIS1__RUN_STATUS_FFORMAT_TAG
  
  SInt16 ParGuiUpdRate;       // GUI update rate 1 acq each ParGuiUpdRate acq
  
  SInt8  ParUpdOnlyAcqErrCnt; // Update only acq and total errors counters
  
  SInt8  ParOptDaqSpeed;      // For future use
  
  SInt8  ResRunInProgress;    // Data taking is running, 0 => No, 1 => Yes
  
  UInt32 ResAcqCnt;           // Acquisition counter
  UInt32 ResFrCnt;            // Frames counter, Rq : frame nb / acq is not constant
  float  ResAcqRateHz;        // Acquisition rate Hz
  float  ResFrRateHz;         // Frames rate Hz, mean value over ... ?
  
  float  ResMSis1FrRateHz;    // MSis1 frame rate Hz is MSis1 or emulator is in constant fr nb mode
                              // othervise this result is NA
  
  float  ResDaqEffPCent;      // DAQ efficiency = DAQ frame rate Hz / MSis1 frame rate Hz
                              // if MSis1 or emulator is in constant fr nb mode, otherwise NA
  
  UInt32 ResFrNbInCurAcq;     // Frame number in current acquisition displayed in GUI
  
} MIS1__TRunStatus;


/* ================================================== */
/* 19/05/2020                      */
/* For run conf file                   */
/* ================================================== */

// Run status

typedef struct {
      
  // ---------------------------- WARNING -----------------------------
  // No field can be added or remove in this record
  // ------------------------------------------------------------------
  // This record MUST contain the following fields, not less, not more
  // - MIS1__TRunPar RunPar;   
  // - MIS1__TRunInf RunInf;
  // ------------------------------------------------------------------
  // Because in DAQ app MIS1__TRunConf is not used, but two consecutive
  // fields  MIS1__TRunPar, MIS1__TRunInfand and a pointer cast on
  // MIS1__TRunPar* as MIS1__TRunConf* to access ...
  // Bad coding, not fixed due to lack of time 
  // ------------------------------------------------------------------
  
  MIS1__TRunPar RunPar;
  
  MIS1__TRunInf RunInf;
  
} MIS1__TRunConf;



/* ================================================== */
/* 19/05/2020                      */
/* Test file header                   */
/* ================================================== */

typedef struct {
  
  UInt32 StepNb;

  // Etc ...
  
  
} MIS1__TTestFileHead;



/* ================================================== */
/* 19/05/2020                      */
/* Test file step                   */
/* ================================================== */

typedef struct {
  
  char   InfSCFileName[GLB_FILE_PATH_SZ];  // File path of slow control config file
  
  // Following fields will to be redefined when doc will be available
  
  UInt16 MatFirstLine;
  UInt16 MatLinesNb;
  
  UInt16 Threshold; // Etc ..
  
  
} MIS1__TTestFileStep;



/* ================================================== */
/* 19/05/2020                      */
/* Test file trailer                   */
/* ================================================== */

typedef struct {
  
  UInt32 TestErrors;
  
  // Etc ...
  
  
} MIS1__TTestFileTrail;


/**
===================================================================================
* \typedef MIS1__TTMatPixSelPix
*
* \brief Mimosis 1 pixels select matrix for test, emul, mask pixels
*
*
* G.CLAUS 18/06/2020
*
===================================================================================
*/

// Two types definition because src editor can't dectect "array typedef"

typedef UInt8 MIS1__TTMatPixSelPix[MIS1__COL_NB][MIS1__ROW_NB];

typedef MIS1__TTMatPixSelPix MIS1__TMatPixSelPix;



/**
===================================================================================
* \typedef MIS1__TAcqProc
*
* \brief Type MIS1__TAcqProc = Acq processing
*
*
* G.CLAUS 09/06/2020
*
===================================================================================
*/



typedef struct {
  
  UInt16  HeadPattern[8]; // Expected header pattern { W0, W1, W2, W3, W4, W5, W6, W7 }
  // { 0xFE00, 0xFE00, 0xFE00, 0xFE00, 0xFE04, 0xFE05, 0xFE06, 0xFE07 };
  // B0B7 of W0 to W3 contains frames counter, W0_B0B7 = FrCnt_B0B7, W3_B0B7 = FrCnt_B24B31
  // Pattern "Vers1" set at initialization, pattern configured in GUI is captured late
  
  // MIS1__TAcqFrList* AAcqFrListRaw [MIS1__MAX_BUFFERED_ACQ_NB];
  
  MIS1__TAcqFrList* AAcqFrListRaw;
  MIS1__TAcqFrMap*  AAcqFrMapRaw;
  
  MIS1__TDecFrList* AFrListDec;
  MIS1__TDecFrMap*  AFrMapDec;
  
  void*            AAcqFrDecBuff; // Buffers for data decoding, allocated at beginning of run
  
  
  // MIS1__TAcqFrPixList* AAcqFrPixList; // Pixels list in acq,fr
  
  MIS1__TDecFrPix* ADecFrPix; // Pixels list in each frame
  
  
  MIS1__TMatPixCntU16* PtSubStepPixCnt; // Fired pixels count for each sub step, same array used for all sub steps of one step
  
  MIS1__TMatPixCntU16* PtStepPixCnt;    // Fired pixels count for each step, same array used for all steps
  
  
  // Pointer to others records in order to reduce processing functions parameters number
  // Init is done by MIS1__FAcqProcInitPtrFields ( PtAcqProc, Pt
  
  MIS1__TRunPar*    PtRunPar;
  MIS1__TRunInf*    PtRunInf;
  MIS1__TRunStatus* PtRunStatus;
  FIL__TCBinFile*   PtOStepFile;
  FIL__TCBinFile*   PtOSubStepsFile;
  
  
} MIS1__TAcqProc;




/**
===================================================================================
* \struct MIS1__TRunFiles
*
* \brief Record which contains all fields to access MSis 1 DAQ run files
*
* This record contains ...
*
* G.CLAUS 25/05/2020
*
===================================================================================
*/



typedef struct {
  
  SInt8 FBeginDone;
  SInt8 FOpenDone;
  
  MIS1__TRunConf RunConf; /*!< Contains RunPar, RunInf */ 
  
  // MIS1__TRunPar    RunPar;   /*!<  */
  // MIS1__TRunInf    RunInf;
  
  MIS1__TRunStatus RunStatus;  /*!< Run status record */
  
  UInt16 RawFBuffIndexId;  /*!< Id of OBuff for raw data index file */ 
  UInt16 RawFBuffDataId;   /*!< Id of OBuff for raw data index file */ 
  
  UInt16 DecFBuffIndexId;  /*!< Id of OBuff for raw data index file - 10/06/2020 */
  UInt16 FBuffDecDataId;   /*!< Id of OBuff for raw data index file - 10/06/2020 */
  
  UInt32 AcqMaxSzW64;      /*!< Maximum size of one acq in W64 */
  UInt32 AcqMaxSzW8;       /*!< Maximum size of one acq in W8 */
  
  SInt32 RawLoadedFileSuffix; /*!< Raw data file loaded in memory, -1 if no file loaded */
  
  SInt32 DecLoadedFileSuffix; /*!< Dec data file loaded in memory, -1 if no file loaded */

  SInt32 FpcStepLoadedFileSuffix; /*!< Fpc step data file loaded in memory, -1 if no file loaded */

  SInt32 FpcSubStepLoadedFileSuffix; /*!< Fpc sub step data file loaded in memory, -1 if no file loaded */
  

  // -------------------------------------------------------------
  // Current acq or frame
  // Fields used to access one acquisition or one frames
  // -------------------------------------------------------------
  
  SInt32 RawCurAcqId;          /*!< Set by MIS1__FRunFilesRawPrivFindAcq ( AcqId, ObjId ) = Acq  to read, -1 if error */
  SInt32 RawCurFrId;           /*!< Set by MIS1__FRunFilesRawPrivFindFr ( FrId, ObjId ) = Fr to read, -1 if error     */
  
  
  SInt32 DecCurAcqId;          /*!< Set by MIS1__FRunFilesDecPrivFindAcq ( AcqId, ObjId ) = Acq  to read, -1 if error */
  SInt32 DecCurFrId;           /*!< Set by MIS1__FRunFilesDecPrivFindFr ( FrId, ObjId ) = Fr to read, -1 if error     */
  
  
  SInt32 RawCurAcqFileSuffix;  /*!< Suffix of file containing the acq or fr, calculated by, -1 if error            */
                            /*!< MIS1__FRunFilesRawPrivFindAcq () or MIS1__FRunFilesRawPrivFindFr (), -1 if error     */

  SInt32 RawCurAcqIdInFile;    /*!< Calculated but not used on 10/06/20, set by MIS1__FRunFilesRawPrivFindAcq ( AcqId, ObjId ) = Acq Id in file, -1 if error */
  
  
  SInt32 DecCurAcqFileSuffix;  /*!< Suffix of file containing the acq or fr, calculated by, -1 if error            */
                               /*!< MIS1__FRunFilesDecPrivFindAcq () or MIS1__FRunFilesDecPrivFindFr (), -1 if error     */
  
  SInt32 DecCurAcqIdInFile;    /*!< Set by MIS1__FRunFilesDecPrivFindAcq ( AcqId, ObjId ) = Acq Id in file, -1 if error */
  
  

  MIS1__TAcqFrMap* PtFileRawFrMap; /*!< Pointer to RAW acq frames map records of cutrent file loaded  */
  UInt8*           PtAcqRawMem;    /*!< Pointer first acq of current RAW file loaded  */
  
  MIS1__TDecFrMap* PtFileDecFrMap;    /*!< Pointer to DEC frames map records of cutrent file loaded  */
  UInt8*           PtAcqDecMem;    /*!< Pointer first acq of current DEC file loaded  */
  
  
  MIS1__TAcqFrList CurAcqRawFrList; /*!< Frames list of current RAW acq */
  
  MIS1__TDecFrList ACurAcqDecFrList[MIS1__MAX_FR_NB_PER_ACQ]; /*!< Frames list of current DEC acq */
  
  // -------------------------------------------------------------
  // FPC fields
  // 
  // -------------------------------------------------------------

  SInt32 FpcCurStepId;          /*!< Set by MIS1__FRunFilesFpcPrivFindStep ( StepId, ObjId ) = Step  to read, -1 if error */
  
  SInt32 FpcCurSubStepId;       /*!< Set by MIS1__FRunFilesFpcPrivFindSubStep ( SubStepId, ObjId ) = Step  to read, -1 if error */
  
  SInt32 FpcCurStepFileSuffix;  /*!< Suffix of file containing the step, calculated by, -1 if error            */
                                /*!< MIS1__FRunFilesFpcPrivFindStep () , -1 if error     */
  
  SInt32 FpcCurSubStepFileSuffix;  /*!< Suffix of file containing the sub step, calculated by, -1 if error            */
                                   /*!< MIS1__FRunFilesFpcPrivFindSubStep () , -1 if error     */
  
  MIS1__TMatPixCntU16 StepMatPixCnt;    /*!< Step fired pixels counter matrix  */
  
  MIS1__TMatPixCntU16 SubStepMatPixCnt; /*!< Sub step fired pixels counter matrix  */
  
  FIL__TCBinFile* PtFPCSubStepsBinFile; 
  FIL__TCBinFile* PtFPCStepBinFile;    
  
} MIS1__TRunFiles;




/**
===================================================================================
* \struct MIS1__TPulseParam
*
* \brief Pulse parameters ( A/D, length, 1f/n, etc ... it does not include test pixels coordinates )
*
* This record contains ...
*
* G.CLAUS 27/10/2020
*
===================================================================================
*/

#define CC_DBG_021120

#ifdef CC_DBG_021120

typedef struct {

  UInt16 PixPulse_A               ; // = 25; // default 2,5 us => 25   // 1 us => 25
  UInt16 PixPulse_B               ; // = 75; // default 2,5 us => 75   // 1 us => 45
  UInt8  ResetMatrixBeforeSetting ; // = 1;
  UInt8  StartChipAfterSetting    ; // = 1;
  UInt8  PulseMask                ; // = 2; // 0 = Mask, 1 = Pulse_D, 2 = Pulse_A
  UInt8  ModPulse                 ; // = APP__VGMSisPulseMode; // 3; // 0 = 1f/1, 1 = 1f/2, 2 = 1f/4, 3 = 1f/8, 4 = 1f/16, 5 = 1f/32, 6 = 1f/64, 7 = 1f/128

} MIS1__TPulseParam;


#else

typedef struct {

  uint16_t PixPulse_A               ; // = 25; // default 2,5 us => 25   // 1 us => 25
  uint16_t PixPulse_B               ; // = 75; // default 2,5 us => 75   // 1 us => 45
  uint8_t  ResetMatrixBeforeSetting ; // = 1;
  uint8_t  StartChipAfterSetting    ; // = 1;
  uint8_t  PulseMask                ; // = 2; // 0 = Mask, 1 = Pulse_D, 2 = Pulse_A
  uint8_t  ModPulse                 ; // = APP__VGMSisPulseMode; // 3; // 0 = 1f/1, 1 = 1f/2, 2 = 1f/4, 3 = 1f/8, 4 = 1f/16, 5 = 1f/32, 6 = 1f/64, 7 = 1f/128

} MIS1__TPulseParam;


#endif


/**
===================================================================================
* \struct MIS1__TPixCoordRangeU16
*
* \brief A range of pixels coordinates FirstCol, LastCol, FirstRow, LastRow, U16 => for minimum space ...
*
* This record contains ...
*
* G.CLAUS 07/10/2020
*
===================================================================================
*/

typedef struct {
  
  UInt16 ParFirstCol;
  UInt16 ParLastCol;
  UInt16 ParFirstRow;
  UInt16 ParLastRow;
  
  UInt32 resPixNb; // Calculated from above parameters
  
} MIS1__TPixCoordRangeU16;



/**
===================================================================================
* \struct MIS1__TPixCoordRangeU32
*
* \brief A range of pixels coordinates FirstCol, LastCol, FirstRow, LastRow, U32 => for Python easy readout ...
*
* This record contains ...
*
* G.CLAUS 07/10/2020
*
===================================================================================
*/

typedef struct {
  
  UInt32 ParFirstCol;
  UInt32 ParLastCol;
  UInt32 ParFirstRow;
  UInt32 ParLastRow;
  
  UInt32 ResPixNb; // Calculated from above parameters
  
} MIS1__TPixCoordRangeU32;



/**
===================================================================================
* \struct MIS1__TSubStepPar
*
* \brief Sun step parameters
*
* This record contains ...
*
* G.CLAUS 07/10/2020
*
===================================================================================
*/

typedef struct {
  
  MIS1__TPixCoordRangeU32 Pixels;

} MIS1__TSubStepPar;


/**
===================================================================================
* \struct MIS1__TMSis1Reg
*
* \brief MSis 1 registers
*
* This record contains ...
*
* G.CLAUS 07/10/2020
*
===================================================================================
*/

typedef struct {
  
  UInt32 AReg[MIS1__CAR_MSIS1_MAX_REG_NB];
  
  // ...
  
} MIS1__TMSis1Reg;



/**
===================================================================================
* \struct MIS1__TMSis1ScanPar
*
* \brief SMSis 1 scan parameters
*
* This record contains ...
*
* G.CLAUS 07/10/2020
*
===================================================================================
*/

typedef struct {
  
  UInt32 AScanPar[MIS1__CAR_MSIS1_MAX_SCAN_PAR_NB];
  
}  MIS1__TMSis1ScanPar;



/**
===================================================================================
* \struct MIS1__TStepPar
*
* \brief Sun step parameters
*
* This record contains ...
*
* G.CLAUS 07/10/2020
*
===================================================================================
*/

typedef struct {
  
  
  char SCConfFile[GLB_FILE_PATH_SZ];
  
  MIS1__TMSis1Reg       Reg;
  MIS1__TMSis1ScanPar   ScanPar;
  
  
} MIS1__TStepPar;




/**
===================================================================================
* \struct MIS1__TRunFilesLight
*
* \brief Record which contains all fields to access MSis 1 DAQ LIGHT run files
*
* This record contains ...
*
* G.CLAUS 07/10/2020
*
===================================================================================
*/

typedef struct {
  
  // Param 
  
  UInt32 ParFormat;                /*!< Data format version  */
  char   ParFormatStr[GLB_CMT_SZ]; /*!< Data format version in a human readable way  */
  
  UInt32 ParRunNo;       /*!< Run number  */
  UInt32 ParStepNb;      /*!< Steps nb = scan param step nb  */
  UInt32 ParSubStepNb;   /*!< Sub steps nb = number of pixels groups needed to perform a step  */
  
  UInt32 ParSubStepMode;                 /*!< Sub step mode = how is configured each sub step  */
  char   ParSubStepModeStr[GLB_CMT_SZ];  /*!< Sub step mode string = how is configured each sub step  */
  
  UInt32 ParFrNb;        /*!< Frame number used for each sustep / step */
  
  MIS1__TStepPar    ParASteps[MIS1__CAR_MAX_STEP_NB];    /*!< Parametrs of each step   */
  MIS1__TSubStepPar ParASubSteps[MIS1__CAR_MAX_SUB_STEP_NB]; /*!< Parametrs of each sub step = pixels tested  */
  
  UInt32 ParSaveRaw;          /*!< Save raw data for each sub steps  */
  UInt32 ParSaveSubStepFpc;   /*!< Save FPC for each sub step  */
  UInt32 ParSaveStepFpc;      /*!< Save FPC for each step  */
  
  
  char   ParRootDir[GLB_FILE_PATH_SZ]; // Ex : C:/tmp/iphc/msis1/res/
  

  // Inf
  
  char Dir[GLB_FILE_PATH_SZ];     // Ex : C:/tmp/iphc/msis1/res/run_0
  char DirData[GLB_FILE_PATH_SZ]; // Ex : C:/tmp/iphc/msis1/res/run_0/data
  char DirTxt[GLB_FILE_PATH_SZ];  // Ex : C:/tmp/iphc/msis1/res/run_0/txt
  char DirPlot[GLB_FILE_PATH_SZ]; // Ex : C:/tmp/iphc/msis1/res/run_0/plot
  
  // Files
  
  
  char CurStepFileFpcBin[GLB_FILE_PATH_SZ];
  char CurSubStepsFileFpcBin[GLB_FILE_PATH_SZ];
  char CurSubStepsFileRawBin[GLB_FILE_PATH_SZ];
  
  // Var
  
  UInt32 CurStepNo;
  UInt32 CurSubStepNo;
  
  
  
} MIS1__TRunCnfLight;



/**
===================================================================================
* \class APP__TCCarWarnErr
*
* \brief Characterization warnig or error listing object
*
* Longer C structure explanation
* ...
* ...
* ...
* can take many lines
*
* G.CLAUS 26/02/2021
*
===================================================================================
*/




class MIS1__TCCarWarnErr {
  
  public:
  
    MIS1__TCCarWarnErr ();
    ~MIS1__TCCarWarnErr ();
    
    
  
    SInt32 FChkMSisId ( SInt8 MSisId );
  
    // General reset functions
    
    SInt32 FReset ();
    SInt32 FResetList ();
  
  
    // Reset total counters
  
    SInt32 FDaqSwErrRstTotCnt ( SInt8 MSisId );
    SInt32 FDaqSwRegOvfRstTotCnt ( SInt8 MSisId );
    SInt32 FDaqAcqTruncRstTotCnt ( SInt8 MSisId );
    SInt32 FMSisFrOvfRstTotCnt ( SInt8 MSisId );
    
    SInt32 FRstAllTotCnt ( SInt8 MSisId );
    
    // Get total counters 
    
    SInt32 FDaqSwErrGetTotCnt ( SInt8 MSisId );
    SInt32 FDaqSwRegOvfGetTotCnt ( SInt8 MSisId  );
    SInt32 FDaqAcqTruncGetTotCnt ( SInt8 MSisId );
    SInt32 FMSisFrOvfGetTotCnt ( SInt8 MSisId );
    
    // Reset counters
    
    SInt32 FDaqSwErrRstCnt ( SInt8 MSisId );
    SInt32 FDaqSwRegOvfRstCnt ( SInt8 MSisId );
    SInt32 FDaqAcqTruncRstCnt ( SInt8 MSisId );
    SInt32 FMSisFrOvfRstCnt ( SInt8 MSisId );
    
    SInt32 FRstAllCnt ( SInt8 MSisId );
    
    // Get counters
    
    SInt32 FDaqSwErrGetCnt ( SInt8 MSisId );
    SInt32 FDaqSwRegOvfGetCnt ( SInt8 MSisId  );
    SInt32 FDaqAcqTruncGetCnt ( SInt8 MSisId );
    UInt32 FMSisFrOvfGetCnt ( SInt8 MSisId );
    
    
    // Add errors to the list
    
    SInt32 FDaqErrAdd       ( SInt8 MSisId, UInt32 RunId, UInt32 AcqId, UInt32 FrId, UInt16 StepId, UInt16 SubStepId, UInt32 MSisFrCnt, SInt32 Code, SInt32 Val, MIS1__EWarnErr ErrType );

    SInt32 FDaqSwErrAdd     ( SInt8 MSisId, UInt32 RunId, UInt32 AcqId, UInt32 FrId, UInt16 StepId, UInt16 SubStepId, UInt32 MSisFrCnt, SInt32 Code, SInt32 Val );
    SInt32 FDaqSwRegOvfrAdd ( SInt8 MSisId, UInt32 RunId, UInt32 AcqId, UInt32 FrId, UInt16 StepId, UInt16 SubStepId, UInt32 MSisFrCnt, SInt32 Code, SInt32 Val );
    SInt32 FDaqAcqTruncAdd  ( SInt8 MSisId, UInt32 RunId, UInt32 AcqId, UInt32 FrId, UInt16 StepId, UInt16 SubStepId, UInt32 MSisFrCnt, SInt32 Code, SInt32 Val );
    
    
    SInt32 FAcqMSisFrOvfAdd ( SInt8 MSisId, UInt8 OvfFlags, UInt32 MSisFrCnt, UInt32 AcqFrId );
    SInt32 FAcqMSisFrOvfCount ( SInt8 List, SInt8 MSisId, UInt32 RunId, UInt32 AcqId, UInt16 StepId, UInt16 SubStepId, UInt32 AcqFrNb  );


    
    
    
            
  
    
    UInt32 FAcqMSisFrOvfGetFlags ( SInt8 MSisId, UInt32 FrId );
    UInt32 FAcqMSisFrOvfGetFrCnt ( SInt8 MSisId, UInt32 FrId );
  
    SInt32 FWarnErrGetNb ();
    SInt32 FWarnErrGetNbTot ();
    SInt32 FWarnErrPrint ( UInt32 First, UInt32 Nb, SInt8 PrintListHeader, SInt8 PrintDest, TMemo* Memo, FILE* PtFile );
  
  
    SInt32 FBinFileSave ( char* FileName );
    SInt32 FBinFileLoad ( char* FileName );
    
    SInt32 FBinFileSave ( char* Directory, char* FileName );
    SInt32 FBinFileLoad ( char* Directory, char* FileName );
    
  
  private:
  
    // Functions
    
    SInt32 _FClearAcqTrailInfo  ( SInt8 MSisId ); // Set to 0 ATrailFlags, ATrailFrCnt of MSisId
  
  
    // Fields
  
    char CstWarnErrType2Str [MIS1__WE_NB][50];
    
    
    
    
    SInt8  ListIsFull; // Flag used to stop printing warning messages when list is fulkl    

    UInt32 AWarnErrTotCnt[MIS1__WE_MAX_MSIS_NB][MIS1__WE_NB]; // Warning, errors total couters, one for each type of warning / error
                                        // MIS1__WE_SW_ERR,        /*!< SW acq truncated  */
                                        // MIS1__WE_SW_REG_OVF,    /*!< SW region overflow  */
                                        // MIS1__WE_SW_ACQ_TRUNC,  /*!< SW acq truncated  */
                                        // MIS1__WE_MSIS_FR_OVF,    /*!< MSis 1 frame overflow (flags bits of trailer field)  */
                                        // MIS1__WE_NB
                                     

    UInt32 AWarnErrCnt[MIS1__WE_MAX_MSIS_NB][MIS1__WE_NB]; // Warning, errors couters, one for each type of warning / error
                                     // MIS1__WE_SW_ERR,        /*!< SW acq truncated  */
                                     // MIS1__WE_SW_REG_OVF,    /*!< SW region overflow  */
                                     // MIS1__WE_SW_ACQ_TRUNC,  /*!< SW acq truncated  */
                                     // MIS1__WE_MSIS_FR_OVF,    /*!< MSis 1 frame overflow (flags bits of trailer field)  */
                                     // MIS1__WE_NB
                                     


    UInt8          ATrailFlags[MIS1__WE_MAX_MSIS_NB][MIS1__MAX_FR_NB_PER_ACQ]; // Copie of MSis 1 OVF flags of all frames of current acq
    UInt32         ATrailFrCnt[MIS1__WE_MAX_MSIS_NB][MIS1__MAX_FR_NB_PER_ACQ]; // Copie of MSis 1 frame counter of all frames of current acq
    
    
    
    // Data to be written to the binay file
    // in the follwing order !
    
    UInt32 SafetyTag; // To check if loaded file has been created by this class 
    
    UInt32 DateVersion; // Format 0xDDMMYYVV, with VV = version, ex : 23/03/2021 version 1 => 0x23032101
    
    SInt32 WarnErrCnt; // Cpt of warning /error = index of last record in AWarnErr, LIMITED to MIS1__MAX_CAR_WARN_ERR_NB maximum
    
    SInt32 WarnErrCntTot; // Total cpt of warning /error = NOT LIMITED to MIS1__MAX_CAR_WARN_ERR_NB
    
    MIS1__TWarnErr AWarnErr[MIS1__MAX_CAR_WARN_ERR_NB];
    
  
};


#endif


/**
===================================================================================
* \struct MIS1__TAcqIds
*
* \brief Identifiers of one acq
*
*
* G.CLAUS 01/05/2021
*
===================================================================================
*/

typedef struct {
  
  SInt32 AcqIdInDaq;  /*!<  Acq id in DAQ, -1 if not used, squence can be 0, 2 , 3 if Trig < Min  */
  SInt32 AcqIdInRun;  /*!<  Acq id in run, -1 if not used                          */
  SInt32 AcqIdInBuff; /*!<  Acq id in memory buffer, -1 if not used                */
  SInt32 AcqIdInFile; /*!<  Acq id in file, -1 if not used                         */
  SInt32 FileId;      /*!<  No of the file which contains the acq, -1 if not used  */
  
  UInt32 TimeStamp1;  /*!<  Time stamp, TBD                                        */
  UInt32 TimeStamp2;
  
  SInt32 DataType;    /*!<  Data organization                                      */
   
} MIS1__TBtAcqIds;


/**
===================================================================================
* \struct MIS1__TAcqRes
*
* \brief Results of one acq
*
*
* G.CLAUS 01/05/2021
*
===================================================================================
*/

typedef struct {
  
  SInt32 ErrNb;           /*!< Errors number, -1 if not used                                  */
  
  SInt32 TrigNb;          /*!< Triggers number, -1 if not used                                */
  SInt32 FrNb;            /*!< Frames number, -1 if not used                                  */
  SInt32 FrNbTruncated;   /*!< Frames number truncated ( > sw buffer sz ), -1 if not used     */
  SInt32 FrNbOvf;         /*!< Frames number with Mimosis OVF, -1 if not used                 */
  SInt32 FrNbSwErr;       /*!< Frames number with a DAQ sw error, -1 if not used              */

  SInt32 FrCnt[MIS1__BT_FRS_MAX_FR_NB_PER_ACQ][MIS1__BT_MAX_REAL_MSIS_NB_ACQ]; /*!< Signed => can be set < 0 in case cnt are desynchronized , Max fr nb ~ 3 H   */
  
  // 14/05/2021

  UInt32 FiredPixNb; /*!< Total fired pixels nb for ALL matrices   */
  UInt32 AFiredPixNb[MIS1__BT_MAX_REAL_MSIS_NB_ACQ][MIS1__MAT_NB]; /*!< Fired pixels nb per matrice  */

} MIS1__TBtAcqRes;




/**
===================================================================================
* \struct MIS1__TTrigRec
*
* \brief MSis DAQ triggers infp
*
*
* G.CLAUS 01/05/2021
*
===================================================================================
*/

typedef struct {
    
  SInt32 TrigNb;          /*!< Triggers number, -1 if not used, redundant with MIS1__TBtAcqRes.TrigNb   */
  
  // Triggers, TBD
  
  UInt32 ATrig[MIS1__BT_MAX_TRIG_NB_PER_ACQ];
  
  
} MIS1__TBtTrigRec;


/**
===================================================================================
* \struct MIS1__TTrigRec
*
* \brief MSis DAQ triggers infp
*
*
* G.CLAUS 01/05/2021
*
===================================================================================
*/

#ifndef CC_MIS1__TBtAcqRawRec_BEFORE_110521

typedef struct {
    
    UInt32 TagBeg;         /*!<  Record beginning  tag = first W32 of a record, B7B0 = Data format version   */
        
    UInt32 Offset;    /*!< Position in W8 of record / beginning of file       */
    UInt32 FullRecSz; /*!< Effective size in W8 of the acq <= sizeof (MIS1__TAcqRec)       */
    UInt32 DataSz;    /*!< Effective size in W8 of the data part <=       */
    
    MIS1__TBtAcqIds Ids;

    MIS1__TBtTrigRec Trigs;
    
    MIS1__TBtAcqRes Res; // MUST BE THE LAST FIELD (because it could become a variable size record)
    
    UInt32 Padding1_W32; // Added for compilation under Linux on 19/05/2021
    
} MIS1__TBtAcqRawHead;
  

#endif

/**
===================================================================================
* \struct MIS1__TBtAcqRec
*
* \brief MSis DAQ triggers infp
*
*
* G.CLAUS 01/05/2021
*
===================================================================================
*/

typedef struct {


#ifdef CC_MIS1__TBtAcqRawRec_BEFORE_110521

  struct {
  
    UInt32 FullRecSz; /*!< Effective size of the acq <= sizeof (MIS1__TAcqRec)       */
    UInt32 DataSz;    /*!< Effective size of the data part <=       */
  
    MIS1__TBtAcqIds Ids;
  
    MIS1__TBtAcqRes Res;
  
    MIS1__TBtTrigRec Trigs;
  
  } Head;
  
#else

  MIS1__TBtAcqRawHead Head;

#endif

  
  // Acq, TBD
  
  union {
    
    
    UInt8  Au8 [MIS1__BT_FRS_MAX_ACQ_DATA_SZ_W8];  /*!< Data part as a U arrray, effecive size is DataSz (in bytes) */
    UInt16 Au16[MIS1__BT_FRS_MAX_ACQ_DATA_SZ_W16]; /*!< Data part as a U arrray, effecive size is DataSz (in bytes) */
    UInt32 Au32[MIS1__BT_FRS_MAX_ACQ_DATA_SZ_W32]; /*!< Data part as a U arrray, effecive size is DataSz (in bytes) */
    UInt64 Au64[MIS1__BT_FRS_MAX_ACQ_DATA_SZ_W64]; /*!< Data part as a U arrray, effecive size is DataSz (in bytes) */
    
    
  } MSisData;
  
  
} MIS1__TBtAcqRawRec;


/**
===================================================================================
* \struct MIS1__TBtRecHeader
*
* \brief Record header, MUST be at beginning of each conf / param record to be save to disk
*
*
* G.CLAUS 13/05/2021
*
===================================================================================
*/


typedef struct {

  UInt32 TagBeg;         /*!< Should be == MIS1__BT_REC_TAG_BEGIN, with B7B0 = Data format version */
  UInt32 TagEndian;      /*!< Should be == MIS1__BT_REC_TAG_ENDIAN */
  
  
} MIS1__TBtRecHeadChk;

/**
===================================================================================
* \struct MIS1__TBtRunCnfRec
*
* \brief Msis beam test DAQ run configuration record
*
*
* G.CLAUS 13/05/2021
*
===================================================================================
*/


typedef struct {
  
  MIS1__TBtRecHeadChk  HeadChk; /*!< Record header, MUST be at beginning of each conf / param record to be save to disk */
  
  // WHen a parameter is not used it can be set to -1
  
  SInt32 RunNo;          /*!< No of run */
  SInt32 AcqNb;          /*!< Acquisitions nb in run  */
  SInt32 FrNbPerAcq;     /*!< Frames nb / acquisition */
  SInt32 AcqNbPerFile;   /*!< Acquisitions nb / file */
  SInt32 MinTrigNb;      /*!< Minimun triggers nb to stored and acquisition */
  SInt32 MaxTrigNb;      /*!< Maximum triggers nb to stored and acquisition */
  SInt32 RunMode;        /*!< Run mode => TB defined later => Probably RAW / TRIGGERS ONLY */
  SInt32 ProcMode;       /*!< Processing  mode => TB defined later  */
  SInt32 SaveMode;       /*!< Save  mode => TB defined later, 0 => don't save */
  SInt32 TrigMode;       /*!< Trigger  mode => TB defined later */
  SInt32 RunTrgPreFrNb;  /*!< For trigger only runs => Nb of frames stored before trigger */
  SInt32 RunTrgPostFrNb; /*!< For trigger only runs => Nb of frames stored after trigger */
  
  
  SInt32 DmaPacketW64Sz; /*!< DMA packet size used for Flex RIO => for info only */
  
  char   RunCmt[GLB_CMT_SZ];                       /*!< User run comment */
  
  char   RunRawIndexFilePrefix[GLB_FILE_PATH_SZ];   /*!< Name of the index file prefix for raw data run  */
  char   RunRawDataFilePrefix[GLB_FILE_PATH_SZ];    /*!< Name of the raw data file prefix = name without the file number ext RUN_777_ */
  
  char   RunTrgIndexFilePrefix[GLB_FILE_PATH_SZ];   /*!< Name of the index file prefix for "triggers only" data run  */
  char   RunTrgDataFilePrefix[GLB_FILE_PATH_SZ];    /*!< Name of the "triggers only" data file prefix = name without the file number ext RUN_777_ */
  
  
} MIS1__TBtRunCnfRec;



/**
===================================================================================
* \struct MIS1__TBtRunCnfRec
*
* \brief Msis beam test DAQ run configuration record
*
*
* G.CLAUS 13/05/2021
*
===================================================================================
*/


typedef struct {

  MIS1__TBtRecHeadChk  HeadChk; /*!< Record header, MUST be at beginning of each conf / param record to be save to disk */

  UInt32 AcqNbInFile; /*!< Acquisition number in the file (index and data) */
  
  MIS1__TBtAcqRawHead AAcqHead[MIS1__BT_MAX_ACQ_PER_FILE];


  // UInt32 AAcqOffset[MIS1__BT_MAX_ACQ_PER_FILE]; /*!< Position in W8 of each acquisiiton record in file */
  // UInt16 AAcqTrigNb[MIS1__BT_MAX_ACQ_PER_FILE]; /*!< Nb of triggers in each acquisiiton  */
  // UInt16 AAcqErrNb[MIS1__BT_MAX_ACQ_PER_FILE];  /*!< Nb of errors in each acquisition */
  
  
} MIS1__TBtRunRawIndexRec;



/**
===================================================================================
* \class MIS1__TBtRunRead
*
* \brief Beam test run file reading class
*
* Longer C structure explanation
* ...
* ...
* ...
* can take many lines
*
* G.CLAUS 16/05/2021
*
===================================================================================
*/




class MIS1__TBtRunRead {
  
  public:
  
    MIS1__TBtRunRead  ();
    ~MIS1__TBtRunRead  ();
  
    // --------------------------------------------------------------
    // Public methods
    // --------------------------------------------------------------
    
    // Gets CPU endianness, returns : 0 if little endian, 1 if big endian, -1 if unknown
    
    SInt32 FIsCpuBigEndian ();
    
    
    // Sets errors messages log file name (value hard coded in function body ;-)
    // Can be called to locate the error file directory and name
    // Errors file MUST be set at beginning of application by MIS1__TBtRunRead_DEF_ERR_LOG_FILE
    
    char*  FErrLogSetFilename ();
    
    // Sets general messages log file name (value hard coded in function body ;-)
    // Can be called to locate the general messages file directory and name
    // Messages file MUST be set at beginning of applicationMIS1__TBtRunRead_DEF_MSG_LOG_FILE
    
    char*  FMsgLogSetFilename (); 
    
    // Sets error log level, by default it is set to ERR_LOG_LVL_ALL, it can be
    // ERR_LOG_LVL_NONE
    // ERR_LOG_LVL_ALL
    // ERR_LOG_LVL_WARNINGS_ERRORS
    // ERR_LOG_LVL_ERRORS
                                                   
    SInt32 FErrLogSetLevel ( SInt8 LogLevel );
    
    // Sets general log level, by default it is set 127 <=> ALL, it can be 0..127
    
    SInt32 FMsgLogSetLevel ( SInt8 LogLevel ); 
                             
    // Gets last error message as a string              
                  
    char* FErrGetLastMsg ();              
  
    // Gets last general message as a string
    
    char* FMsgGetLastMsg ();
    
                  
    // Configures the run to read
    
    SInt32 FRunConf ( char* RunRootDir, UInt32 RunNo, UInt8 PrintRunHeader, UInt8 FormatVers );
    
    // Closes a run (free mem, reset variables, etc ...)
    
    SInt32 FRunClose ();
        
    
    // Returns a pointer to current run header, prints info in log file if Print == 1
    
    MIS1__TBtRunCnfRec* FRunHeaderGet ( UInt8 Print );
    
    
    // Returns a pointer to the first Acq of run
    
    MIS1__TBtAcqRawRec* FAcqFirst ( UInt8 ChkHead, UInt8 PrintHead );
    
    // Returns a pointer to the next Acq of run
    
    MIS1__TBtAcqRawRec* FAcqNext ( UInt8 ChkHead, UInt8 PrintHead, UInt8* PtReachEnd );
    
    
    // Returns a pointer to the Acq of no = AcqId of run
    
    MIS1__TBtAcqRawRec* FAcqGoto ( UInt32 AcqId, UInt8 ChkHead, UInt8 PrintHead, UInt8* PtReachEnd );
    
    // Returns a pointer to current Acq loaded in memory
    
    MIS1__TBtAcqRawRec* FAcqGetPt ();
    
    // Print one Acq : the one pointer by Pt or the current one loaded n memory if Pt == NULL
    
    SInt32 FAcqPrint ( MIS1__TBtAcqRawRec* Pt, SInt32 PrintTriggers, SInt32 PrintFrCnt, SInt32 PrintFiredPixels, UInt8 DataWSize, UInt32 FirstDataW, UInt32 NbDataW, UInt8 VRS );
    
    
    // Configures the header printing option used by Acq scanning functions like FAcqFirst (...), FAcqNext (...), FAcqGoto (...)
    
    SInt32 FAcqHeadPrintOptSet ( SInt32 PrintTriggers, SInt32 PrintFrCnt, SInt32 PrintFiredPixels );
    
    
    // Starts execution time measurement
    
    UInt32 FMeasTimeStart ();
    
    // Stops execution time measurement
    
    UInt32 FMeasTimeStop ();
    
    // Gets result in ms of execution time measurement
    
    UInt32 FMeasTimeGetMs ();
  
    // Gets result in HH:MM:SS:1/100 of execution time measurement
    
    UInt32 FMeasTimeGetHMSMS ( UInt8* PtHour, UInt8* PtMin, UInt8* PtSec, UInt16* PtMs );
    
    
    // 
    
    SInt32 F ();
    
    // --------------------------------------------------------------
    // Privates methods in public part => Will be move later in Private part
    // --------------------------------------------------------------
    
    
    // Class initilization = Init variables, alloc meme, eror + general messages enable / disable
    // Called by constructor => End user should not need to call it
    
    SInt32 _FBegin ();
    
    // Class finalization = free meme etc
    // Called by destructor => End user should not need to call it
    
    SInt32 _FEnd ();
    
    
    // Create the file name which should contains one Acq, result sets in _CurRawFileId, _CurRawFileName
    
    SInt32 _FBuildNewAcqFile ( UInt32 AcqNo, UInt8 Print );
    
    //  Create the file name which corresponds to FileId, result sets in _NewRawFileId, _NewRawFileName, _NewIndexFileName
    
    SInt32 _FBuildNewFile ( UInt32 FileId, UInt8 Print );
    
    // Returns a pointer to the first Acq of one raw data file
    
    MIS1__TBtAcqRawRec* _FFirstAcqOfRawFileGetSeq ( UInt32 RawFileId, UInt8 ChkHead, UInt8 PrintHead );
    
    // Returns a pointer to next Acq of one raw data file
        
    MIS1__TBtAcqRawRec* _FNextAcqOfRawFileGetSeq  ( UInt32 RawFileId, UInt8 ChkHead, UInt8 PrintHead, UInt8* PtReachEnd );
  
    // Returns a pointer to the first Acq of run
    
    MIS1__TBtAcqRawRec* _FAcqFirstSeq ( UInt8 ChkHead, UInt8 PrintHead );
    
    // Returns a pointer to the next Acq of run
    
    MIS1__TBtAcqRawRec* _FAcqNextSeq ( UInt8 ChkHead, UInt8 PrintHead, UInt8* PtReachEnd );
    
    // Returns a pointer to the Acq of no = AcqId of run
    
    MIS1__TBtAcqRawRec* _FAcqGotoSeq ( UInt32 AcqId, UInt8 ChkHead, UInt8 PrintHead, UInt8* PtReachEnd );




    private:
  

    // --------------------------------------------------------------
    // Private fields
    // --------------------------------------------------------------
    
    
    UInt8 _DbgPrint;
    
    SInt8 _ErrLogSetLevel;
    SInt8 _MsgLogSetLevel;
    
      
    SInt32 _CpuIsBigEndian; // 0 => CPU is little endian, 1 => CPU is big endian, -1 => CPU ???  
      
    
    SInt32 _LastError; // Last error
    
    // Run conf
    
    MIS1__TBtRunCnfRec _RunCnfRec; // Run configuration / header record
    
    SInt8 _RunConfLoaded;                        // 0 => No run conf loaded, 1 => run conf loaded
    
    UInt32 _RunNo;
    char   _RunRootDir[GLB_FILE_PATH_SZ];          // Run root directory
    char   _RunConfFileName[GLB_FILE_PATH_SZ];     // File name of run config file ( == header file)
    
    SInt32 _LastRawFileId;                         // Id of last raw file of run, calculated from _RunCnfRec
    
    
    // Size calculated from Run conf parameters
    
    UInt32 _AcqHeadSzW8;   // Size of Acq header, can't be calculated by  sizeof ( MIS1__TBtAcqRawHead ) => Read in FRunConf
    UInt32 _AcqMaxTotSzW8; // Maximum size of one Acq, it is calculated from Fr nb / Acq read at run configuration from run header record    

    // Acq
    
    // MIS1__TBtAcqRawRec  _AcqRaw;
        
    MIS1__TBtAcqRawRec* _PtAcqRaw;
        
    // New Acq file id, rw finename, index filename, built by _FBuildNewAcqFile ( UInt32 AcqNo, UInt8 Print )
    
    SInt32 _NewAcqId;                            // The Id of the new acq
    
    SInt32 _NewRawFileId;                        // No of the new raw data file
    SInt32 _NewAcqIdInFile;                      // Posiiton of new acq in its file 0 .. N  
    char   _NewRawFileName[GLB_FILE_PATH_SZ];    // Full file name + path of new raw data file
    char   _NewIndexFileName[GLB_FILE_PATH_SZ];  // Full file name + path of new index file (corresponding to _CurRawFileName )
    
    
    // Current run raw data file
    
    SInt32 _CurAcqId;                            // The Id of current acq
    
    SInt32 _CurRawFileId;                        // No of the current raw data file
    SInt32 _CurAcqIdInFile;                      // Posiiton of current acq in its file 0 .. N
    char   _CurRawFileName[GLB_FILE_PATH_SZ];    // Full file name + path of current raw data file
    char   _CurIndexFileName[GLB_FILE_PATH_SZ];  // Full file name + path of current index file (corresponding to _CurRawFileName )

    FILE*  _CurRawFilePt;                        // Current raw data file pinter
    FILE*  _CurIndexFilePt;                      // Current index data file pinter
  
  
    // Acq header printing options, used by run scanning functions like FAcqFirst (...), FAcqNext (...), FAcqGoto (...)
  
    SInt32 _HeadPrintTriggers;                   // Print triggers, -1 => All, 0 => No, > 0 => value = nb of triggers to print
    SInt32 _HeadPrintFrCnt;                      // Print frames counters, -1 => All, 0 => No, > 0 => value = nb of frames to print
    SInt32 _HeadPrintFiredPixels;                // Print fired pixels, 0 / 1
    
    
  
    // Execution time measurement
  
    UInt32 _MeasTimeBegMS;
    UInt32 _MeasTimeEndMS;
    UInt32 _MeasTimeMS;
    
  

    // --------------------------------------------------------------
    // Private methods
    // --------------------------------------------------------------
    
  
  

};

  

#endif