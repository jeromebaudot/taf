// @(#)maf/dtools:$Name:  $:$Id: DGlobalTools.cxx,v.1 2005/10/02 18:03:46 sha Exp $
// Author   :  Dirk Meier   98/03/02
// Modified: JB 2011/07/07 add LocalizeDirName method
// Modified: JB 2011/09/19 all "extern" variables suppressed
// Modified: JB 2012/03/11 method to kill/veto pixels
// Modified: JB 2014/04/10 new method CLof6LegendrePol
// Modified: VR 2014/06/29 add the BuildVertex(DR3 ...) method
// Modified: JB 2015/04/01 add ComputeStripPosition 
// Modified: AP 2015/06/23 moved some multiple scattering functions from MMCGeneration to here
//                         In this way these fonction will be usable for all the TAF modules if needed

////////////////////////////////////////////////////////////////
//                                                            //
//         Collection of Global Tools                         //
//         this is the place for global constants, variables, //
//         functions and classes                              //
//         in particular place fit functions her              //
//                                                            //
//////////////////////////////////////////////////////////////// 

//*-- Modified :  
//*-- Copyright:  RD42
//*-- Author   :  Dirk Meier   98/03/02
//*KEEP,CopyRight.
/************************************************************************
 * Copyright(c) 1998, RD42@CERN, DiamondTracking.
 ***********************************************************************/
//*KEND.

//*KEEP,DGlobalTools.
#include "Riostream.h"
#include "DGlobalTools.h"
#include "vetoPixels.c" // JB 2012/03/11
// DAQ Libraries files, added JB 2012/07/19
//#include "daq_lib/daq_lib.h"
//#include "daq_lib/daq_lib.c"

//*KEND.
#include <iostream>
#include <assert.h>

using namespace std;

ClassImp(DGlobalTools)

//______________________________________________________________________________
// 
//DGlobalTools *gTool = new DGlobalTools();
DGlobalTools *DGlobalTools::fgInstance = 0; // returns pointer to global object

//______________________________________________________________________________
// 
DGlobalTools::DGlobalTools()
{ 
  
  gPI  = M_PI;
  gEULER  = 2.7;
  gSpeedOfLight = 3.0; // for test purpose
  VetoPixel = NULL; // JB 2012/03/11

  // Multiple scattering considerations

  fMass["electron"]   = 511e-6;// GeV
  fCharge["electron"] = 1;
  fMass["pion"]       = 0.1396;
  fCharge["pion"]     = 1;
  fMass["kaon"]       = 0.4937;
  fCharge["kaon"]     = 1;
  fMass["proton"]     = 0.9383;
  fCharge["proton"]   = 1;
  fMass["muon"]       = 105.66e-6;
  fCharge["muon"]     = 1;

  fX0["copper"]    =    1.43e+4;
  fX0["silicon"]   =    9.36e+4;//um
  fX0["aluminum"]  =    8.8900e+4;
  fX0["beryllium"] =   35.28e+4;
  fX0["epoxy"]     =   30.e+4;
  fX0["kapton"]    =   28.e+4;
  fX0["glass"]     =   12.30e+4;
  fX0["nai"]       =    2.59e+4;
  fX0["csi"]       =    1.86e+4;
  fX0["tungsten"]  =    0.35e+4;
  fX0["iron"]      =    1.76e+4;
  fX0["water"]     =   36.08e+4;
  fX0["DryAir"]    =   30.39e+7;
  fX0["Vacuum"]    =    1.00e+20;
  fX0["mylar"]     =   28.7e+4;
  fX0["CarbonFiber"] =   23.7e+4;

  // the following table is taken from Bronstein/Semendjajew, 
  // "Taschenbuch der Mathematik", 24 Auflage, Harri Deutsch Verlag
 

  fProbabilityTest[0]  = 0.00;    fProbabilityPhiZ[0]  = 0.0000; 
  fProbabilityTest[1]  = 0.01;    fProbabilityPhiZ[1]  = 0.0040; 
  fProbabilityTest[2]  = 0.02;    fProbabilityPhiZ[2]  = 0.0080; 
  fProbabilityTest[3]  = 0.03;    fProbabilityPhiZ[3]  = 0.0120; 
  fProbabilityTest[4]  = 0.04;    fProbabilityPhiZ[4]  = 0.0160; 
  fProbabilityTest[5]  = 0.05;    fProbabilityPhiZ[5]  = 0.0199; 
  fProbabilityTest[6]  = 0.06;    fProbabilityPhiZ[6]  = 0.0239; 
  fProbabilityTest[7]  = 0.07;    fProbabilityPhiZ[7]  = 0.0279; 
  fProbabilityTest[8]  = 0.08;    fProbabilityPhiZ[8]  = 0.0319; 
  fProbabilityTest[9]  = 0.09;    fProbabilityPhiZ[9]  = 0.0359; 

  fProbabilityTest[10] = 0.10;    fProbabilityPhiZ[10] = 0.0398; 
  fProbabilityTest[11] = 0.11;    fProbabilityPhiZ[11] = 0.0438; 
  fProbabilityTest[12] = 0.12;    fProbabilityPhiZ[12] = 0.0478; 
  fProbabilityTest[13] = 0.13;    fProbabilityPhiZ[13] = 0.0517; 
  fProbabilityTest[14] = 0.14;    fProbabilityPhiZ[14] = 0.0557; 
  fProbabilityTest[15] = 0.15;    fProbabilityPhiZ[15] = 0.0596; 
  fProbabilityTest[16] = 0.16;    fProbabilityPhiZ[16] = 0.0636; 
  fProbabilityTest[17] = 0.17;    fProbabilityPhiZ[17] = 0.0675; 
  fProbabilityTest[18] = 0.18;    fProbabilityPhiZ[18] = 0.0714; 
  fProbabilityTest[19] = 0.19;    fProbabilityPhiZ[19] = 0.0753; 

  fProbabilityTest[20] = 0.20;    fProbabilityPhiZ[20] = 0.0793; 
  fProbabilityTest[21] = 0.21;    fProbabilityPhiZ[21] = 0.0832; 
  fProbabilityTest[22] = 0.22;    fProbabilityPhiZ[22] = 0.0871; 
  fProbabilityTest[23] = 0.23;    fProbabilityPhiZ[23] = 0.0910; 
  fProbabilityTest[24] = 0.24;    fProbabilityPhiZ[24] = 0.0948; 
  fProbabilityTest[25] = 0.25;    fProbabilityPhiZ[25] = 0.0987; 
  fProbabilityTest[26] = 0.26;    fProbabilityPhiZ[26] = 0.1026; 
  fProbabilityTest[27] = 0.27;    fProbabilityPhiZ[27] = 0.1064; 
  fProbabilityTest[28] = 0.28;    fProbabilityPhiZ[28] = 0.1103; 
  fProbabilityTest[29] = 0.29;    fProbabilityPhiZ[29] = 0.1141; 

  fProbabilityTest[30] = 0.30;    fProbabilityPhiZ[30] = 0.1179; 
  fProbabilityTest[31] = 0.31;    fProbabilityPhiZ[31] = 0.1217; 
  fProbabilityTest[32] = 0.32;    fProbabilityPhiZ[32] = 0.1255; 
  fProbabilityTest[33] = 0.33;    fProbabilityPhiZ[33] = 0.1293; 
  fProbabilityTest[34] = 0.34;    fProbabilityPhiZ[34] = 0.1331; 
  fProbabilityTest[35] = 0.35;    fProbabilityPhiZ[35] = 0.1368; 
  fProbabilityTest[36] = 0.36;    fProbabilityPhiZ[36] = 0.1406; 
  fProbabilityTest[37] = 0.37;    fProbabilityPhiZ[37] = 0.1443; 
  fProbabilityTest[38] = 0.38;    fProbabilityPhiZ[38] = 0.1480; 
  fProbabilityTest[39] = 0.39;    fProbabilityPhiZ[39] = 0.1517; 

  fProbabilityTest[40] = 0.40;    fProbabilityPhiZ[40] = 0.1554; 
  fProbabilityTest[41] = 0.41;    fProbabilityPhiZ[41] = 0.1591; 
  fProbabilityTest[42] = 0.42;    fProbabilityPhiZ[42] = 0.1628; 
  fProbabilityTest[43] = 0.43;    fProbabilityPhiZ[43] = 0.1664; 
  fProbabilityTest[44] = 0.44;    fProbabilityPhiZ[44] = 0.1700; 
  fProbabilityTest[45] = 0.45;    fProbabilityPhiZ[45] = 0.1736; 
  fProbabilityTest[46] = 0.46;    fProbabilityPhiZ[46] = 0.1772; 
  fProbabilityTest[47] = 0.47;    fProbabilityPhiZ[47] = 0.1808; 
  fProbabilityTest[48] = 0.48;    fProbabilityPhiZ[48] = 0.1844; 
  fProbabilityTest[49] = 0.49;    fProbabilityPhiZ[49] = 0.1879; 

  fProbabilityTest[50] = 0.50;    fProbabilityPhiZ[50] = 0.1915; 
  fProbabilityTest[51] = 0.51;    fProbabilityPhiZ[51] = 0.1950; 
  fProbabilityTest[52] = 0.52;    fProbabilityPhiZ[52] = 0.1985; 
  fProbabilityTest[53] = 0.53;    fProbabilityPhiZ[53] = 0.2019; 
  fProbabilityTest[54] = 0.54;    fProbabilityPhiZ[54] = 0.2054; 
  fProbabilityTest[55] = 0.55;    fProbabilityPhiZ[55] = 0.2088; 
  fProbabilityTest[56] = 0.56;    fProbabilityPhiZ[56] = 0.2123; 
  fProbabilityTest[57] = 0.57;    fProbabilityPhiZ[57] = 0.2157; 
  fProbabilityTest[58] = 0.58;    fProbabilityPhiZ[58] = 0.2190; 
  fProbabilityTest[59] = 0.59;    fProbabilityPhiZ[59] = 0.2224; 

  fProbabilityTest[60] = 0.60;    fProbabilityPhiZ[60] = 0.2257; 
  fProbabilityTest[61] = 0.61;    fProbabilityPhiZ[61] = 0.2291; 
  fProbabilityTest[62] = 0.62;    fProbabilityPhiZ[62] = 0.2324; 
  fProbabilityTest[63] = 0.63;    fProbabilityPhiZ[63] = 0.2357; 
  fProbabilityTest[64] = 0.64;    fProbabilityPhiZ[64] = 0.2389; 
  fProbabilityTest[65] = 0.65;    fProbabilityPhiZ[65] = 0.2422; 
  fProbabilityTest[66] = 0.66;    fProbabilityPhiZ[66] = 0.2454; 
  fProbabilityTest[67] = 0.67;    fProbabilityPhiZ[67] = 0.2486; 
  fProbabilityTest[68] = 0.68;    fProbabilityPhiZ[68] = 0.2517; 
  fProbabilityTest[69] = 0.69;    fProbabilityPhiZ[69] = 0.2549; 

  fProbabilityTest[70] = 0.70;    fProbabilityPhiZ[70] = 0.2580; 
  fProbabilityTest[71] = 0.71;    fProbabilityPhiZ[71] = 0.2611; 
  fProbabilityTest[72] = 0.72;    fProbabilityPhiZ[72] = 0.2642; 
  fProbabilityTest[73] = 0.73;    fProbabilityPhiZ[73] = 0.2673; 
  fProbabilityTest[74] = 0.74;    fProbabilityPhiZ[74] = 0.2703; 
  fProbabilityTest[75] = 0.75;    fProbabilityPhiZ[75] = 0.2734; 
  fProbabilityTest[76] = 0.76;    fProbabilityPhiZ[76] = 0.2764; 
  fProbabilityTest[77] = 0.77;    fProbabilityPhiZ[77] = 0.2794; 
  fProbabilityTest[78] = 0.78;    fProbabilityPhiZ[78] = 0.2823; 
  fProbabilityTest[79] = 0.79;    fProbabilityPhiZ[79] = 0.2852; 

  fProbabilityTest[80] = 0.80;    fProbabilityPhiZ[80] = 0.2881; 
  fProbabilityTest[81] = 0.81;    fProbabilityPhiZ[81] = 0.2910; 
  fProbabilityTest[82] = 0.82;    fProbabilityPhiZ[82] = 0.2939; 
  fProbabilityTest[83] = 0.83;    fProbabilityPhiZ[83] = 0.2967; 
  fProbabilityTest[84] = 0.84;    fProbabilityPhiZ[84] = 0.2995; 
  fProbabilityTest[85] = 0.85;    fProbabilityPhiZ[85] = 0.3023; 
  fProbabilityTest[86] = 0.86;    fProbabilityPhiZ[86] = 0.3051; 
  fProbabilityTest[87] = 0.87;    fProbabilityPhiZ[87] = 0.3078; 
  fProbabilityTest[88] = 0.88;    fProbabilityPhiZ[88] = 0.3106; 
  fProbabilityTest[89] = 0.89;    fProbabilityPhiZ[89] = 0.3133; 

  fProbabilityTest[90] = 0.90;    fProbabilityPhiZ[90] = 0.3159; 
  fProbabilityTest[91] = 0.91;    fProbabilityPhiZ[91] = 0.3186; 
  fProbabilityTest[92] = 0.92;    fProbabilityPhiZ[92] = 0.3212; 
  fProbabilityTest[93] = 0.93;    fProbabilityPhiZ[93] = 0.3238; 
  fProbabilityTest[94] = 0.94;    fProbabilityPhiZ[94] = 0.3264; 
  fProbabilityTest[95] = 0.95;    fProbabilityPhiZ[95] = 0.3289; 
  fProbabilityTest[96] = 0.96;    fProbabilityPhiZ[96] = 0.3315; 
  fProbabilityTest[97] = 0.97;    fProbabilityPhiZ[97] = 0.3340; 
  fProbabilityTest[98] = 0.98;    fProbabilityPhiZ[98] = 0.3365; 
  fProbabilityTest[99] = 0.99;    fProbabilityPhiZ[99] = 0.3389; 

  fProbabilityTest[100] = 1.00;    fProbabilityPhiZ[100] = 0.3413; 
  fProbabilityTest[101] = 1.01;    fProbabilityPhiZ[101] = 0.3438; 
  fProbabilityTest[102] = 1.02;    fProbabilityPhiZ[102] = 0.3461; 
  fProbabilityTest[103] = 1.03;    fProbabilityPhiZ[103] = 0.3485; 
  fProbabilityTest[104] = 1.04;    fProbabilityPhiZ[104] = 0.3508; 
  fProbabilityTest[105] = 1.05;    fProbabilityPhiZ[105] = 0.3531; 
  fProbabilityTest[106] = 1.06;    fProbabilityPhiZ[106] = 0.3554; 
  fProbabilityTest[107] = 1.07;    fProbabilityPhiZ[107] = 0.3577; 
  fProbabilityTest[108] = 1.08;    fProbabilityPhiZ[108] = 0.3599; 
  fProbabilityTest[109] = 1.09;    fProbabilityPhiZ[109] = 0.3621; 

  fProbabilityTest[110] = 1.10;    fProbabilityPhiZ[110] = 0.3643; 
  fProbabilityTest[111] = 1.11;    fProbabilityPhiZ[111] = 0.3665; 
  fProbabilityTest[112] = 1.12;    fProbabilityPhiZ[112] = 0.3686; 
  fProbabilityTest[113] = 1.13;    fProbabilityPhiZ[113] = 0.3708; 
  fProbabilityTest[114] = 1.14;    fProbabilityPhiZ[114] = 0.3729; 
  fProbabilityTest[115] = 1.15;    fProbabilityPhiZ[115] = 0.3749; 
  fProbabilityTest[116] = 1.16;    fProbabilityPhiZ[116] = 0.3770; 
  fProbabilityTest[117] = 1.17;    fProbabilityPhiZ[117] = 0.3790; 
  fProbabilityTest[118] = 1.18;    fProbabilityPhiZ[118] = 0.3810; 
  fProbabilityTest[119] = 1.19;    fProbabilityPhiZ[119] = 0.3830; 

  fProbabilityTest[120] = 1.20;    fProbabilityPhiZ[120] = 0.3849; 
  fProbabilityTest[121] = 1.21;    fProbabilityPhiZ[121] = 0.3869; 
  fProbabilityTest[122] = 1.22;    fProbabilityPhiZ[122] = 0.3888; 
  fProbabilityTest[123] = 1.23;    fProbabilityPhiZ[123] = 0.3907; 
  fProbabilityTest[124] = 1.24;    fProbabilityPhiZ[124] = 0.3925; 
  fProbabilityTest[125] = 1.25;    fProbabilityPhiZ[125] = 0.3944; 
  fProbabilityTest[126] = 1.26;    fProbabilityPhiZ[126] = 0.3962; 
  fProbabilityTest[127] = 1.27;    fProbabilityPhiZ[127] = 0.3980; 
  fProbabilityTest[128] = 1.28;    fProbabilityPhiZ[128] = 0.3997; 
  fProbabilityTest[129] = 1.29;    fProbabilityPhiZ[129] = 0.4015; 

  fProbabilityTest[130] = 1.30;    fProbabilityPhiZ[130] = 0.4032; 
  fProbabilityTest[131] = 1.31;    fProbabilityPhiZ[131] = 0.4049; 
  fProbabilityTest[132] = 1.32;    fProbabilityPhiZ[132] = 0.4066; 
  fProbabilityTest[133] = 1.33;    fProbabilityPhiZ[133] = 0.4082; 
  fProbabilityTest[134] = 1.34;    fProbabilityPhiZ[134] = 0.4099; 
  fProbabilityTest[135] = 1.35;    fProbabilityPhiZ[135] = 0.4115; 
  fProbabilityTest[136] = 1.36;    fProbabilityPhiZ[136] = 0.4131; 
  fProbabilityTest[137] = 1.37;    fProbabilityPhiZ[137] = 0.4147; 
  fProbabilityTest[138] = 1.38;    fProbabilityPhiZ[138] = 0.4162; 
  fProbabilityTest[139] = 1.39;    fProbabilityPhiZ[139] = 0.4177; 

  fProbabilityTest[140] = 1.40;    fProbabilityPhiZ[140] = 0.4192; 
  fProbabilityTest[141] = 1.41;    fProbabilityPhiZ[141] = 0.4207; 
  fProbabilityTest[142] = 1.42;    fProbabilityPhiZ[142] = 0.4222; 
  fProbabilityTest[143] = 1.43;    fProbabilityPhiZ[143] = 0.4236; 
  fProbabilityTest[144] = 1.44;    fProbabilityPhiZ[144] = 0.4251; 
  fProbabilityTest[145] = 1.45;    fProbabilityPhiZ[145] = 0.4265; 
  fProbabilityTest[146] = 1.46;    fProbabilityPhiZ[146] = 0.4279; 
  fProbabilityTest[147] = 1.47;    fProbabilityPhiZ[147] = 0.4292; 
  fProbabilityTest[148] = 1.48;    fProbabilityPhiZ[148] = 0.4306; 
  fProbabilityTest[149] = 1.49;    fProbabilityPhiZ[149] = 0.4319; 

  fProbabilityTest[150] = 1.50;    fProbabilityPhiZ[150] = 0.4332; 
  fProbabilityTest[151] = 1.51;    fProbabilityPhiZ[151] = 0.4345; 
  fProbabilityTest[152] = 1.52;    fProbabilityPhiZ[152] = 0.4357; 
  fProbabilityTest[153] = 1.53;    fProbabilityPhiZ[153] = 0.4370; 
  fProbabilityTest[154] = 1.54;    fProbabilityPhiZ[154] = 0.4382; 
  fProbabilityTest[155] = 1.55;    fProbabilityPhiZ[155] = 0.4394; 
  fProbabilityTest[156] = 1.56;    fProbabilityPhiZ[156] = 0.4406; 
  fProbabilityTest[157] = 1.57;    fProbabilityPhiZ[157] = 0.4418; 
  fProbabilityTest[158] = 1.58;    fProbabilityPhiZ[158] = 0.4429; 
  fProbabilityTest[159] = 1.59;    fProbabilityPhiZ[159] = 0.4441; 

  fProbabilityTest[160] = 1.60;    fProbabilityPhiZ[160] = 0.4452; 
  fProbabilityTest[161] = 1.61;    fProbabilityPhiZ[161] = 0.4463; 
  fProbabilityTest[162] = 1.62;    fProbabilityPhiZ[162] = 0.4474; 
  fProbabilityTest[163] = 1.63;    fProbabilityPhiZ[163] = 0.4484; 
  fProbabilityTest[164] = 1.64;    fProbabilityPhiZ[164] = 0.4495; 
  fProbabilityTest[165] = 1.65;    fProbabilityPhiZ[165] = 0.4505; 
  fProbabilityTest[166] = 1.66;    fProbabilityPhiZ[166] = 0.4515; 
  fProbabilityTest[167] = 1.67;    fProbabilityPhiZ[167] = 0.4525; 
  fProbabilityTest[168] = 1.68;    fProbabilityPhiZ[168] = 0.4535; 
  fProbabilityTest[169] = 1.69;    fProbabilityPhiZ[169] = 0.4545; 

  fProbabilityTest[170] = 1.70;    fProbabilityPhiZ[170] = 0.4554; 
  fProbabilityTest[171] = 1.71;    fProbabilityPhiZ[171] = 0.4564; 
  fProbabilityTest[172] = 1.72;    fProbabilityPhiZ[172] = 0.4573; 
  fProbabilityTest[173] = 1.73;    fProbabilityPhiZ[173] = 0.4582; 
  fProbabilityTest[174] = 1.74;    fProbabilityPhiZ[174] = 0.4591; 
  fProbabilityTest[175] = 1.75;    fProbabilityPhiZ[175] = 0.4599; 
  fProbabilityTest[176] = 1.76;    fProbabilityPhiZ[176] = 0.4608; 
  fProbabilityTest[177] = 1.77;    fProbabilityPhiZ[177] = 0.4616; 
  fProbabilityTest[178] = 1.78;    fProbabilityPhiZ[178] = 0.4625; 
  fProbabilityTest[179] = 1.79;    fProbabilityPhiZ[179] = 0.4633; 

  fProbabilityTest[180] = 1.80;    fProbabilityPhiZ[180] = 0.4641; 
  fProbabilityTest[181] = 1.81;    fProbabilityPhiZ[181] = 0.4649; 
  fProbabilityTest[182] = 1.82;    fProbabilityPhiZ[182] = 0.4656; 
  fProbabilityTest[183] = 1.83;    fProbabilityPhiZ[183] = 0.4664; 
  fProbabilityTest[184] = 1.84;    fProbabilityPhiZ[184] = 0.4671; 
  fProbabilityTest[185] = 1.85;    fProbabilityPhiZ[185] = 0.4678; 
  fProbabilityTest[186] = 1.86;    fProbabilityPhiZ[186] = 0.4686; 
  fProbabilityTest[187] = 1.87;    fProbabilityPhiZ[187] = 0.4693; 
  fProbabilityTest[188] = 1.88;    fProbabilityPhiZ[188] = 0.4699; 
  fProbabilityTest[189] = 1.89;    fProbabilityPhiZ[189] = 0.4706; 

  fProbabilityTest[190] = 1.90;    fProbabilityPhiZ[190] = 0.4713; 
  fProbabilityTest[191] = 1.91;    fProbabilityPhiZ[191] = 0.4719; 
  fProbabilityTest[192] = 1.92;    fProbabilityPhiZ[192] = 0.4726; 
  fProbabilityTest[193] = 1.93;    fProbabilityPhiZ[193] = 0.4732; 
  fProbabilityTest[194] = 1.94;    fProbabilityPhiZ[194] = 0.4738; 
  fProbabilityTest[195] = 1.95;    fProbabilityPhiZ[195] = 0.4744; 
  fProbabilityTest[196] = 1.96;    fProbabilityPhiZ[196] = 0.4750; 
  fProbabilityTest[197] = 1.97;    fProbabilityPhiZ[197] = 0.4756; 
  fProbabilityTest[198] = 1.98;    fProbabilityPhiZ[198] = 0.4761; 
  fProbabilityTest[199] = 1.99;    fProbabilityPhiZ[199] = 0.4767; 

  fProbabilityTest[200] = 2.00;    fProbabilityPhiZ[200] = 0.4772; 
  fProbabilityTest[201] = 2.01;    fProbabilityPhiZ[201] = 0.4778; 
  fProbabilityTest[202] = 2.02;    fProbabilityPhiZ[202] = 0.4783; 
  fProbabilityTest[203] = 2.03;    fProbabilityPhiZ[203] = 0.4788; 
  fProbabilityTest[204] = 2.04;    fProbabilityPhiZ[204] = 0.4793; 
  fProbabilityTest[205] = 2.05;    fProbabilityPhiZ[205] = 0.4798; 
  fProbabilityTest[206] = 2.06;    fProbabilityPhiZ[206] = 0.4803; 
  fProbabilityTest[207] = 2.07;    fProbabilityPhiZ[207] = 0.4808; 
  fProbabilityTest[208] = 2.08;    fProbabilityPhiZ[208] = 0.4812; 
  fProbabilityTest[209] = 2.09;    fProbabilityPhiZ[209] = 0.4817; 

  fProbabilityTest[210] = 2.10;    fProbabilityPhiZ[210] = 0.4821; 
  fProbabilityTest[211] = 2.11;    fProbabilityPhiZ[211] = 0.4826; 
  fProbabilityTest[212] = 2.12;    fProbabilityPhiZ[212] = 0.4830; 
  fProbabilityTest[213] = 2.13;    fProbabilityPhiZ[213] = 0.4834; 
  fProbabilityTest[214] = 2.14;    fProbabilityPhiZ[214] = 0.4838; 
  fProbabilityTest[215] = 2.15;    fProbabilityPhiZ[215] = 0.4842; 
  fProbabilityTest[216] = 2.16;    fProbabilityPhiZ[216] = 0.4846; 
  fProbabilityTest[217] = 2.17;    fProbabilityPhiZ[217] = 0.4850; 
  fProbabilityTest[218] = 2.18;    fProbabilityPhiZ[218] = 0.4854; 
  fProbabilityTest[219] = 2.19;    fProbabilityPhiZ[219] = 0.4857; 

  fProbabilityTest[220] = 2.20;    fProbabilityPhiZ[220] = 0.4860966; 
  fProbabilityTest[221] = 2.21;    fProbabilityPhiZ[221] = 0.4864474; 
  fProbabilityTest[222] = 2.22;    fProbabilityPhiZ[222] = 0.4867906; 
  fProbabilityTest[223] = 2.23;    fProbabilityPhiZ[223] = 0.4871263; 
  fProbabilityTest[224] = 2.24;    fProbabilityPhiZ[224] = 0.4874545; 
  fProbabilityTest[225] = 2.25;    fProbabilityPhiZ[225] = 0.4877755; 
  fProbabilityTest[226] = 2.26;    fProbabilityPhiZ[226] = 0.4880894; 
  fProbabilityTest[227] = 2.27;    fProbabilityPhiZ[227] = 0.4883962; 
  fProbabilityTest[228] = 2.28;    fProbabilityPhiZ[228] = 0.4886962; 
  fProbabilityTest[229] = 2.29;    fProbabilityPhiZ[229] = 0.4889893; 

  fProbabilityTest[230] = 2.30;    fProbabilityPhiZ[230] = 0.4892759; 
  fProbabilityTest[231] = 2.31;    fProbabilityPhiZ[231] = 0.4895559; 
  fProbabilityTest[232] = 2.32;    fProbabilityPhiZ[232] = 0.4898296; 
  fProbabilityTest[233] = 2.33;    fProbabilityPhiZ[233] = 0.4900969; 
  fProbabilityTest[234] = 2.34;    fProbabilityPhiZ[234] = 0.4903581; 
  fProbabilityTest[235] = 2.35;    fProbabilityPhiZ[235] = 0.4906133; 
  fProbabilityTest[236] = 2.36;    fProbabilityPhiZ[236] = 0.4908625; 
  fProbabilityTest[237] = 2.37;    fProbabilityPhiZ[237] = 0.4911060; 
  fProbabilityTest[238] = 2.38;    fProbabilityPhiZ[238] = 0.4913437; 
  fProbabilityTest[239] = 2.39;    fProbabilityPhiZ[239] = 0.4915758; 

  fProbabilityTest[240] = 2.40;    fProbabilityPhiZ[240] = 0.4918025; 
  fProbabilityTest[241] = 2.41;    fProbabilityPhiZ[241] = 0.4920237; 
  fProbabilityTest[242] = 2.42;    fProbabilityPhiZ[242] = 0.4922397; 
  fProbabilityTest[243] = 2.43;    fProbabilityPhiZ[243] = 0.4924506; 
  fProbabilityTest[244] = 2.44;    fProbabilityPhiZ[244] = 0.4926564; 
  fProbabilityTest[245] = 2.45;    fProbabilityPhiZ[245] = 0.4928572; 
  fProbabilityTest[246] = 2.46;    fProbabilityPhiZ[246] = 0.4930531; 
  fProbabilityTest[247] = 2.47;    fProbabilityPhiZ[247] = 0.4932443; 
  fProbabilityTest[248] = 2.48;    fProbabilityPhiZ[248] = 0.4934309; 
  fProbabilityTest[249] = 2.49;    fProbabilityPhiZ[249] = 0.4936128; 

  fProbabilityTest[250] = 2.50;    fProbabilityPhiZ[250] = 0.4937903; 
  fProbabilityTest[251] = 2.51;    fProbabilityPhiZ[251] = 0.4939634; 
  fProbabilityTest[252] = 2.52;    fProbabilityPhiZ[252] = 0.4941323; 
  fProbabilityTest[253] = 2.53;    fProbabilityPhiZ[253] = 0.4942969; 
  fProbabilityTest[254] = 2.54;    fProbabilityPhiZ[254] = 0.4944574; 
  fProbabilityTest[255] = 2.55;    fProbabilityPhiZ[255] = 0.4946139; 
  fProbabilityTest[256] = 2.56;    fProbabilityPhiZ[256] = 0.4947664; 
  fProbabilityTest[257] = 2.57;    fProbabilityPhiZ[257] = 0.4949151; 
  fProbabilityTest[258] = 2.58;    fProbabilityPhiZ[258] = 0.4950600; 
  fProbabilityTest[259] = 2.59;    fProbabilityPhiZ[259] = 0.4952012; 

  fProbabilityTest[260] = 2.60;    fProbabilityPhiZ[260] = 0.4953388; 
  fProbabilityTest[261] = 2.61;    fProbabilityPhiZ[261] = 0.4954729; 
  fProbabilityTest[262] = 2.62;    fProbabilityPhiZ[262] = 0.4956035; 
  fProbabilityTest[263] = 2.63;    fProbabilityPhiZ[263] = 0.4957308; 
  fProbabilityTest[264] = 2.64;    fProbabilityPhiZ[264] = 0.4958547; 
  fProbabilityTest[265] = 2.65;    fProbabilityPhiZ[265] = 0.4959754; 
  fProbabilityTest[266] = 2.66;    fProbabilityPhiZ[266] = 0.4960930; 
  fProbabilityTest[267] = 2.67;    fProbabilityPhiZ[267] = 0.4962074; 
  fProbabilityTest[268] = 2.68;    fProbabilityPhiZ[268] = 0.4963189; 
  fProbabilityTest[269] = 2.69;    fProbabilityPhiZ[269] = 0.4964274; 

  fProbabilityTest[270] = 2.70;    fProbabilityPhiZ[270] = 0.4965330; 
  fProbabilityTest[271] = 2.71;    fProbabilityPhiZ[271] = 0.4966358; 
  fProbabilityTest[272] = 2.72;    fProbabilityPhiZ[272] = 0.4967359; 
  fProbabilityTest[273] = 2.73;    fProbabilityPhiZ[273] = 0.4968333; 
  fProbabilityTest[274] = 2.74;    fProbabilityPhiZ[274] = 0.4969280; 
  fProbabilityTest[275] = 2.75;    fProbabilityPhiZ[275] = 0.4970202; 
  fProbabilityTest[276] = 2.76;    fProbabilityPhiZ[276] = 0.4971099; 
  fProbabilityTest[277] = 2.77;    fProbabilityPhiZ[277] = 0.4971972; 
  fProbabilityTest[278] = 2.78;    fProbabilityPhiZ[278] = 0.4972821; 
  fProbabilityTest[279] = 2.79;    fProbabilityPhiZ[279] = 0.4973646; 

  fProbabilityTest[280] = 2.80;    fProbabilityPhiZ[280] = 0.4974449; 
  fProbabilityTest[281] = 2.81;    fProbabilityPhiZ[281] = 0.4975229; 
  fProbabilityTest[282] = 2.82;    fProbabilityPhiZ[282] = 0.4975988; 
  fProbabilityTest[283] = 2.83;    fProbabilityPhiZ[283] = 0.4976726; 
  fProbabilityTest[284] = 2.84;    fProbabilityPhiZ[284] = 0.4977443; 
  fProbabilityTest[285] = 2.85;    fProbabilityPhiZ[285] = 0.4978140; 
  fProbabilityTest[286] = 2.86;    fProbabilityPhiZ[286] = 0.4978818; 
  fProbabilityTest[287] = 2.87;    fProbabilityPhiZ[287] = 0.4979476; 
  fProbabilityTest[288] = 2.88;    fProbabilityPhiZ[288] = 0.4980116; 
  fProbabilityTest[289] = 2.89;    fProbabilityPhiZ[289] = 0.4980738; 

  fProbabilityTest[290] = 2.90;    fProbabilityPhiZ[290] = 0.4981342; 
  fProbabilityTest[291] = 2.91;    fProbabilityPhiZ[291] = 0.4981929; 
  fProbabilityTest[292] = 2.92;    fProbabilityPhiZ[292] = 0.4982498; 
  fProbabilityTest[293] = 2.93;    fProbabilityPhiZ[293] = 0.4983052; 
  fProbabilityTest[294] = 2.94;    fProbabilityPhiZ[294] = 0.4983589; 
  fProbabilityTest[295] = 2.95;    fProbabilityPhiZ[295] = 0.4984111; 
  fProbabilityTest[296] = 2.96;    fProbabilityPhiZ[296] = 0.4984618; 
  fProbabilityTest[297] = 2.97;    fProbabilityPhiZ[297] = 0.4985110; 
  fProbabilityTest[298] = 2.98;    fProbabilityPhiZ[298] = 0.4985588; 
  fProbabilityTest[299] = 2.99;    fProbabilityPhiZ[299] = 0.4986051; 

  fProbabilityTest[300] = 3.00;    fProbabilityPhiZ[300] = 0.4986501; 
  fProbabilityTest[301] = 3.01;    fProbabilityPhiZ[301] = 0.4986938; 
  fProbabilityTest[302] = 3.02;    fProbabilityPhiZ[302] = 0.4987361; 
  fProbabilityTest[303] = 3.03;    fProbabilityPhiZ[303] = 0.4987772; 
  fProbabilityTest[304] = 3.04;    fProbabilityPhiZ[304] = 0.4988171; 
  fProbabilityTest[305] = 3.05;    fProbabilityPhiZ[305] = 0.4988558; 
  fProbabilityTest[306] = 3.06;    fProbabilityPhiZ[306] = 0.4988933; 
  fProbabilityTest[307] = 3.07;    fProbabilityPhiZ[307] = 0.4989297; 
  fProbabilityTest[308] = 3.08;    fProbabilityPhiZ[308] = 0.4989650; 
  fProbabilityTest[309] = 3.09;    fProbabilityPhiZ[309] = 0.4989992; 

  fProbabilityTest[310] = 3.10;    fProbabilityPhiZ[310] = 0.4990324; 
  fProbabilityTest[311] = 3.11;    fProbabilityPhiZ[311] = 0.4990646; 
  fProbabilityTest[312] = 3.12;    fProbabilityPhiZ[312] = 0.4990957; 
  fProbabilityTest[313] = 3.13;    fProbabilityPhiZ[313] = 0.4991260; 
  fProbabilityTest[314] = 3.14;    fProbabilityPhiZ[314] = 0.4991553; 
  fProbabilityTest[315] = 3.15;    fProbabilityPhiZ[315] = 0.4991836; 
  fProbabilityTest[316] = 3.16;    fProbabilityPhiZ[316] = 0.4992112; 
  fProbabilityTest[317] = 3.17;    fProbabilityPhiZ[317] = 0.4992378; 
  fProbabilityTest[318] = 3.18;    fProbabilityPhiZ[318] = 0.4992636; 
  fProbabilityTest[319] = 3.19;    fProbabilityPhiZ[319] = 0.4992886; 

  fProbabilityTest[320] = 3.20;    fProbabilityPhiZ[320] = 0.4993129; 
  fProbabilityTest[321] = 3.21;    fProbabilityPhiZ[321] = 0.4993363; 
  fProbabilityTest[322] = 3.22;    fProbabilityPhiZ[322] = 0.4993590; 
  fProbabilityTest[323] = 3.23;    fProbabilityPhiZ[323] = 0.4993810; 
  fProbabilityTest[324] = 3.24;    fProbabilityPhiZ[324] = 0.4994024; 
  fProbabilityTest[325] = 3.25;    fProbabilityPhiZ[325] = 0.4994230; 
  fProbabilityTest[326] = 3.26;    fProbabilityPhiZ[326] = 0.4994429; 
  fProbabilityTest[327] = 3.27;    fProbabilityPhiZ[327] = 0.4994623; 
  fProbabilityTest[328] = 3.28;    fProbabilityPhiZ[328] = 0.4994810; 
  fProbabilityTest[329] = 3.29;    fProbabilityPhiZ[329] = 0.4994991; 

  fProbabilityTest[330] = 3.30;    fProbabilityPhiZ[330] = 0.4995166; 
  fProbabilityTest[331] = 3.31;    fProbabilityPhiZ[331] = 0.4995335; 
  fProbabilityTest[332] = 3.32;    fProbabilityPhiZ[332] = 0.4995499; 
  fProbabilityTest[333] = 3.33;    fProbabilityPhiZ[333] = 0.4995658; 
  fProbabilityTest[334] = 3.34;    fProbabilityPhiZ[334] = 0.4995811; 
  fProbabilityTest[335] = 3.35;    fProbabilityPhiZ[335] = 0.4995959; 
  fProbabilityTest[336] = 3.36;    fProbabilityPhiZ[336] = 0.4996103; 
  fProbabilityTest[337] = 3.37;    fProbabilityPhiZ[337] = 0.4996242; 
  fProbabilityTest[338] = 3.38;    fProbabilityPhiZ[338] = 0.4996376; 
  fProbabilityTest[339] = 3.39;    fProbabilityPhiZ[339] = 0.4996505; 

  fProbabilityTest[340] = 3.40;    fProbabilityPhiZ[340] = 0.4996631; 
  fProbabilityTest[341] = 3.41;    fProbabilityPhiZ[341] = 0.4996752; 
  fProbabilityTest[342] = 3.42;    fProbabilityPhiZ[342] = 0.4996869; 
  fProbabilityTest[343] = 3.43;    fProbabilityPhiZ[343] = 0.4997982; 
  fProbabilityTest[344] = 3.44;    fProbabilityPhiZ[344] = 0.4997091; 
  fProbabilityTest[345] = 3.45;    fProbabilityPhiZ[345] = 0.4997197; 
  fProbabilityTest[346] = 3.46;    fProbabilityPhiZ[346] = 0.4997299; 
  fProbabilityTest[347] = 3.47;    fProbabilityPhiZ[347] = 0.4997398; 
  fProbabilityTest[348] = 3.48;    fProbabilityPhiZ[348] = 0.4997493; 
  fProbabilityTest[349] = 3.49;    fProbabilityPhiZ[349] = 0.4997585; 

  fProbabilityTest[350] = 3.50;    fProbabilityPhiZ[350] = 0.4997674; 
  fProbabilityTest[351] = 3.51;    fProbabilityPhiZ[351] = 0.4997759; 
  fProbabilityTest[352] = 3.52;    fProbabilityPhiZ[352] = 0.4997842; 
  fProbabilityTest[353] = 3.53;    fProbabilityPhiZ[353] = 0.4997922; 
  fProbabilityTest[354] = 3.54;    fProbabilityPhiZ[354] = 0.4998999; 
  fProbabilityTest[355] = 3.55;    fProbabilityPhiZ[355] = 0.4998074; 
  fProbabilityTest[356] = 3.56;    fProbabilityPhiZ[356] = 0.4998146; 
  fProbabilityTest[357] = 3.57;    fProbabilityPhiZ[357] = 0.4998215; 
  fProbabilityTest[358] = 3.58;    fProbabilityPhiZ[358] = 0.4998282; 
  fProbabilityTest[359] = 3.59;    fProbabilityPhiZ[359] = 0.4998347; 

  fProbabilityTest[360] = 3.60;    fProbabilityPhiZ[360] = 0.4998409; 
  fProbabilityTest[361] = 3.61;    fProbabilityPhiZ[361] = 0.4998469; 
  fProbabilityTest[362] = 3.62;    fProbabilityPhiZ[362] = 0.4998527; 
  fProbabilityTest[363] = 3.63;    fProbabilityPhiZ[363] = 0.4998583; 
  fProbabilityTest[364] = 3.64;    fProbabilityPhiZ[364] = 0.4998637; 
  fProbabilityTest[365] = 3.65;    fProbabilityPhiZ[365] = 0.4998689; 
  fProbabilityTest[366] = 3.66;    fProbabilityPhiZ[366] = 0.4998739; 
  fProbabilityTest[367] = 3.67;    fProbabilityPhiZ[367] = 0.4998787; 
  fProbabilityTest[368] = 3.68;    fProbabilityPhiZ[368] = 0.4998834; 
  fProbabilityTest[369] = 3.69;    fProbabilityPhiZ[369] = 0.4998879; 

  fProbabilityTest[370] = 3.70;    fProbabilityPhiZ[370] = 0.4998922; 
  fProbabilityTest[371] = 3.71;    fProbabilityPhiZ[371] = 0.4999964; 
  fProbabilityTest[372] = 3.72;    fProbabilityPhiZ[372] = 0.4999004; 
  fProbabilityTest[373] = 3.73;    fProbabilityPhiZ[373] = 0.4999043; 
  fProbabilityTest[374] = 3.74;    fProbabilityPhiZ[374] = 0.4999080; 
  fProbabilityTest[375] = 3.75;    fProbabilityPhiZ[375] = 0.4999116; 
  fProbabilityTest[376] = 3.76;    fProbabilityPhiZ[376] = 0.4999150; 
  fProbabilityTest[377] = 3.77;    fProbabilityPhiZ[377] = 0.4999184; 
  fProbabilityTest[378] = 3.78;    fProbabilityPhiZ[378] = 0.4999216; 
  fProbabilityTest[379] = 3.79;    fProbabilityPhiZ[379] = 0.4999247; 

  fProbabilityTest[380] = 3.80;    fProbabilityPhiZ[380] = 0.4999276; 
  fProbabilityTest[381] = 3.81;    fProbabilityPhiZ[381] = 0.4999305; 
  fProbabilityTest[382] = 3.82;    fProbabilityPhiZ[382] = 0.4999333; 
  fProbabilityTest[383] = 3.83;    fProbabilityPhiZ[383] = 0.4999359; 
  fProbabilityTest[384] = 3.84;    fProbabilityPhiZ[384] = 0.4999385; 
  fProbabilityTest[385] = 3.85;    fProbabilityPhiZ[385] = 0.4999409; 
  fProbabilityTest[386] = 3.86;    fProbabilityPhiZ[386] = 0.4999433; 
  fProbabilityTest[387] = 3.87;    fProbabilityPhiZ[387] = 0.4999456; 
  fProbabilityTest[388] = 3.88;    fProbabilityPhiZ[388] = 0.4999478; 
  fProbabilityTest[389] = 3.89;    fProbabilityPhiZ[389] = 0.4999499; 

  fProbabilityTest[390] = 3.90;    fProbabilityPhiZ[390] = 0.4999519; 
  fProbabilityTest[391] = 3.91;    fProbabilityPhiZ[391] = 0.4999539; 
  fProbabilityTest[392] = 3.92;    fProbabilityPhiZ[392] = 0.4999557; 
  fProbabilityTest[393] = 3.93;    fProbabilityPhiZ[393] = 0.4999575; 
  fProbabilityTest[394] = 3.94;    fProbabilityPhiZ[394] = 0.4999593; 
  fProbabilityTest[395] = 3.95;    fProbabilityPhiZ[395] = 0.4999609; 
  fProbabilityTest[396] = 3.96;    fProbabilityPhiZ[396] = 0.4999625; 
  fProbabilityTest[397] = 3.97;    fProbabilityPhiZ[397] = 0.4999641; 
  fProbabilityTest[398] = 3.98;    fProbabilityPhiZ[398] = 0.4999655; 
  fProbabilityTest[399] = 3.99;    fProbabilityPhiZ[399] = 0.4999670; 

  fProbabilityTest[400] = 4.00;    fProbabilityPhiZ[400] = 0.4999683; 
  fProbabilityTest[401] = 4.01;    fProbabilityPhiZ[401] = 0.4999696; 
  fProbabilityTest[402] = 4.02;    fProbabilityPhiZ[402] = 0.4999709; 
  fProbabilityTest[403] = 4.03;    fProbabilityPhiZ[403] = 0.4999721; 
  fProbabilityTest[404] = 4.04;    fProbabilityPhiZ[404] = 0.4999733; 
  fProbabilityTest[405] = 4.05;    fProbabilityPhiZ[405] = 0.4999744; 
  fProbabilityTest[406] = 4.06;    fProbabilityPhiZ[406] = 0.4999755; 
  fProbabilityTest[407] = 4.07;    fProbabilityPhiZ[407] = 0.4999765; 
  fProbabilityTest[408] = 4.08;    fProbabilityPhiZ[408] = 0.4999775; 
  fProbabilityTest[409] = 4.09;    fProbabilityPhiZ[409] = 0.4999784; 

  fProbabilityTest[410] = 4.10;    fProbabilityPhiZ[410] = 0.4999793; 
  fProbabilityTest[411] = 4.11;    fProbabilityPhiZ[411] = 0.4999802; 
  fProbabilityTest[412] = 4.12;    fProbabilityPhiZ[412] = 0.4999811; 
  fProbabilityTest[413] = 4.13;    fProbabilityPhiZ[413] = 0.4999819; 
  fProbabilityTest[414] = 4.14;    fProbabilityPhiZ[414] = 0.4999826; 
  fProbabilityTest[415] = 4.15;    fProbabilityPhiZ[415] = 0.4999834; 
  fProbabilityTest[416] = 4.16;    fProbabilityPhiZ[416] = 0.4999841; 
  fProbabilityTest[417] = 4.17;    fProbabilityPhiZ[417] = 0.4999848; 
  fProbabilityTest[418] = 4.18;    fProbabilityPhiZ[418] = 0.4999854; 
  fProbabilityTest[419] = 4.19;    fProbabilityPhiZ[419] = 0.4999861; 

  fProbabilityTest[420] = 4.20;    fProbabilityPhiZ[420] = 0.4999867; 
  fProbabilityTest[421] = 4.21;    fProbabilityPhiZ[421] = 0.4999872; 
  fProbabilityTest[422] = 4.22;    fProbabilityPhiZ[422] = 0.4999878; 
  fProbabilityTest[423] = 4.23;    fProbabilityPhiZ[423] = 0.4999883; 
  fProbabilityTest[424] = 4.24;    fProbabilityPhiZ[424] = 0.4999888; 
  fProbabilityTest[425] = 4.25;    fProbabilityPhiZ[425] = 0.4999893; 
  fProbabilityTest[426] = 4.26;    fProbabilityPhiZ[426] = 0.4999898; 
  fProbabilityTest[427] = 4.27;    fProbabilityPhiZ[427] = 0.4999902; 
  fProbabilityTest[428] = 4.28;    fProbabilityPhiZ[428] = 0.4999907; 
  fProbabilityTest[429] = 4.29;    fProbabilityPhiZ[429] = 0.4999911; 

  fProbabilityTest[430] = 4.30;    fProbabilityPhiZ[430] = 0.4999915; 
  fProbabilityTest[431] = 4.31;    fProbabilityPhiZ[431] = 0.4999918; 
  fProbabilityTest[432] = 4.32;    fProbabilityPhiZ[432] = 0.4999922; 
  fProbabilityTest[433] = 4.33;    fProbabilityPhiZ[433] = 0.4999925; 
  fProbabilityTest[434] = 4.34;    fProbabilityPhiZ[434] = 0.4999929; 
  fProbabilityTest[435] = 4.35;    fProbabilityPhiZ[435] = 0.4999932; 
  fProbabilityTest[436] = 4.36;    fProbabilityPhiZ[436] = 0.4999935; 
  fProbabilityTest[437] = 4.37;    fProbabilityPhiZ[437] = 0.4999938; 
  fProbabilityTest[438] = 4.38;    fProbabilityPhiZ[438] = 0.4999941; 
  fProbabilityTest[439] = 4.39;    fProbabilityPhiZ[439] = 0.4999943; 

  fProbabilityTest[440] = 4.40;    fProbabilityPhiZ[440] = 0.4999946; 
  fProbabilityTest[441] = 4.41;    fProbabilityPhiZ[441] = 0.4999948; 
  fProbabilityTest[442] = 4.42;    fProbabilityPhiZ[442] = 0.4999951; 
  fProbabilityTest[443] = 4.43;    fProbabilityPhiZ[443] = 0.4999953; 
  fProbabilityTest[444] = 4.44;    fProbabilityPhiZ[444] = 0.4999955; 
  fProbabilityTest[445] = 4.45;    fProbabilityPhiZ[445] = 0.4999957; 
  fProbabilityTest[446] = 4.46;    fProbabilityPhiZ[446] = 0.4999959; 
  fProbabilityTest[447] = 4.47;    fProbabilityPhiZ[447] = 0.4999961; 
  fProbabilityTest[448] = 4.48;    fProbabilityPhiZ[448] = 0.4999963; 
  fProbabilityTest[449] = 4.49;    fProbabilityPhiZ[449] = 0.4999964; 

  fProbabilityTest[450] = 4.50;    fProbabilityPhiZ[450] = 0.4999966; 
  fProbabilityTest[451] = 4.51;    fProbabilityPhiZ[451] = 0.4999968; 
  fProbabilityTest[452] = 4.52;    fProbabilityPhiZ[452] = 0.4999969; 
  fProbabilityTest[453] = 4.53;    fProbabilityPhiZ[453] = 0.4999971; 
  fProbabilityTest[454] = 4.54;    fProbabilityPhiZ[454] = 0.4999972; 
  fProbabilityTest[455] = 4.55;    fProbabilityPhiZ[455] = 0.4999973; 
  fProbabilityTest[456] = 4.56;    fProbabilityPhiZ[456] = 0.4999974; 
  fProbabilityTest[457] = 4.57;    fProbabilityPhiZ[457] = 0.4999976; 
  fProbabilityTest[458] = 4.58;    fProbabilityPhiZ[458] = 0.4999977; 
  fProbabilityTest[459] = 4.59;    fProbabilityPhiZ[459] = 0.4999978; 


  // table to lookup probabilities from chi-square values at given degree of freedom

  fChi2Probability[0]  = 0.99;
  fChi2Probability[1]  = 0.98;
  fChi2Probability[2]  = 0.95;
  fChi2Probability[3]  = 0.90;
  fChi2Probability[4]  = 0.80;
  fChi2Probability[5]  = 0.70;
  fChi2Probability[6]  = 0.50;
  fChi2Probability[7]  = 0.30;
  fChi2Probability[8]  = 0.20;
  fChi2Probability[9]  = 0.10;
  fChi2Probability[10] = 0.05;
  fChi2Probability[11] = 0.02;
  fChi2Probability[12] = 0.01;
  fChi2Probability[13] = 0.005;
  fChi2Probability[14] = 0.002;
  fChi2Probability[15] = 0.001;

  // the following table is taken from Bronstein/Semendjajew, 
  // "Taschenbuch der Mathematik", 24 Auflage, Harri Deutsch Verlag
  // 1.1.2.10 Obere 100alpha-prozentige Werte Xhi2alpha der Chi2-Verteilung (s.5.2.3.)
  
  // Chi2[DegreeOfFreedom-1][pBin];
  fChi2[0][0]   = 0.00016; fChi2[0][1]    = 0.0006;  fChi2[0][2]   = 0.0039;  fChi2[0][3]   = 0.016;
  fChi2[0][4]   = 0.064  ; fChi2[0][5]    = 0.148 ;  fChi2[0][6]   = 0.455 ;  fChi2[0][7]   = 1.07 ;
  fChi2[0][8]   = 1.64   ; fChi2[0][9]    = 2.7   ;  fChi2[0][10]  = 3.8   ;  fChi2[0][11]  = 5.4  ;
  fChi2[0][12]  = 6.6    ; fChi2[0][13]   = 7.9   ;  fChi2[0][14]  = 9.5   ;  fChi2[0][15]  = 10.83;
  
  fChi2[1][0]   = 0.020;  fChi2[1][1]   = 0.040;  fChi2[1][2]   = 0.103;  fChi2[1][3]   = 0.211;
  fChi2[1][4]   = 0.446;  fChi2[1][5]   = 0.713;  fChi2[1][6]   = 1.386;  fChi2[1][7]   = 2.41 ;
  fChi2[1][8]   = 3.22 ;  fChi2[1][9]   = 4.6  ;  fChi2[1][10]  = 6.0  ;  fChi2[1][11]  = 7.8  ;
  fChi2[1][12]  = 9.2  ;  fChi2[1][13]  = 10.6 ;  fChi2[1][14]  = 12.4 ;  fChi2[1][15]  = 13.8 ;
  
  fChi2[2][0]   = 0.115;  fChi2[2][1]   = 0.185;  fChi2[2][2]   = 0.352;  fChi2[2][3]   = 0.584;
  fChi2[2][4]   = 1.005;  fChi2[2][5]   = 1.424;  fChi2[2][6]   = 2.366;  fChi2[2][7]   = 3.67 ;
  fChi2[2][8]   = 4.64 ;  fChi2[2][9]   = 6.3  ;  fChi2[2][10]  = 7.8  ;  fChi2[2][11]  = 9.8  ;
  fChi2[2][12]  = 11.3 ;  fChi2[2][13]  = 12.8 ;  fChi2[2][14]  = 14.8 ;  fChi2[2][15]  = 16.3 ;
  
  fChi2[3][0]   = 0.30;  fChi2[3][1]   = 0.43;  fChi2[3][2]   = 0.71;  fChi2[3][3]   = 1.06;
  fChi2[3][4]   = 1.65;  fChi2[3][5]   = 2.19;  fChi2[3][6]   = 3.36;  fChi2[3][7]   = 4.9 ;
  fChi2[3][8]   = 6.0 ;  fChi2[3][9]   = 7.8 ;  fChi2[3][10]  = 9.5 ;  fChi2[3][11]  = 11.7;
  fChi2[3][12]  = 13.3;  fChi2[3][13]  = 14.9;  fChi2[3][14]  = 16.9;  fChi2[3][15]  = 18.5;
  
  fChi2[4][0]   = 0.55;  fChi2[4][1]   = 0.75;  fChi2[4][2]   = 1.14;  fChi2[4][3]   = 1.61;
  fChi2[4][4]   = 2.34;  fChi2[4][5]   = 3.00;  fChi2[4][6]   = 4.35;  fChi2[4][7]   = 6.1 ;
  fChi2[4][8]   = 7.3 ;  fChi2[4][9]   = 9.2 ;  fChi2[4][10]  = 11.1;  fChi2[4][11]  = 13.4;
  fChi2[4][12]  = 15.1;  fChi2[4][13]  = 16.8;  fChi2[4][14]  = 18.9;  fChi2[4][15]  = 20.5;
  
  fChi2[5][0]   = 0.87;  fChi2[5][1]   = 1.13;  fChi2[5][2]   = 1.63;  fChi2[5][3]   = 2.20;
  fChi2[5][4]   = 3.07;  fChi2[5][5]   = 3.83;  fChi2[5][6]   = 5.38;  fChi2[5][7]   = 7.2 ;
  fChi2[5][8]   = 8.6 ;  fChi2[5][9]   = 10.6;  fChi2[5][10]  = 12.6;  fChi2[5][11]  = 15.0;
  fChi2[5][12]  = 16.8;  fChi2[5][13]  = 18.5;  fChi2[5][14]  = 20.7;  fChi2[5][15]  = 22.5;

  fChi2[6][0]   = 1.24;  fChi2[6][1]   = 1.56;  fChi2[6][2]   = 2.17;  fChi2[6][3]   = 2.83;
  fChi2[6][4]   = 3.82;  fChi2[6][5]   = 4.67;  fChi2[6][6]   = 6.35;  fChi2[6][7]   = 8.4;
  fChi2[6][8]   = 9.8;  fChi2[6][9]   = 12.0;  fChi2[6][10]  = 14.1;  fChi2[6][11]  = 16.6;
  fChi2[6][12]  = 18.5;  fChi2[6][13]  = 20.3;  fChi2[6][14]  = 22.6;  fChi2[6][15]  = 24.3;

  fChi2[7][0]   = 1.65;  fChi2[7][1]   = 2.03;  fChi2[7][2]   = 2.73;  fChi2[7][3]   = 3.49;
  fChi2[7][4]   = 4.59;  fChi2[7][5]   = 5.53;  fChi2[7][6]   = 7.34;  fChi2[7][7]   = 9.5;
  fChi2[7][8]   = 11.0;  fChi2[7][9]   = 13.4;  fChi2[7][10]  = 15.5;  fChi2[7][11]  = 18.2;
  fChi2[7][12]  = 20.1;  fChi2[7][13]  = 22.0;  fChi2[7][14]  = 24.3;  fChi2[7][15]  = 26.1;

  fChi2[8][0]   = 2.09;  fChi2[8][1]   = 2.53;  fChi2[8][2]   = 3.32;  fChi2[8][3]   = 4.17;
  fChi2[8][4]   = 5.38;  fChi2[8][5]   = 6.39;  fChi2[8][6]   = 8.34;  fChi2[8][7]   = 10.7;
  fChi2[8][8]   = 12.2;  fChi2[8][9]   = 14.7;  fChi2[8][10]  = 16.9;  fChi2[8][11]  = 19.7;
  fChi2[8][12]  = 21.7;  fChi2[8][13]  = 23.6;  fChi2[8][14]  = 26.1;  fChi2[8][15]  = 27.9;

  fChi2[9][0]   = 2.56;  fChi2[9][1]   = 3.06;  fChi2[9][2]   = 3.94;  fChi2[9][3]   = 4.86;
  fChi2[9][4]   = 6.18;  fChi2[9][5]   = 7.27;  fChi2[9][6]   = 9.34;  fChi2[9][7]   = 11.8;
  fChi2[9][8]   = 13.4;  fChi2[9][9]   = 16.0;  fChi2[9][10]  = 18.3;  fChi2[9][11]  = 21.2;
  fChi2[9][12]  = 23.2;  fChi2[9][13]  = 25.2;  fChi2[9][14]  = 27.7;  fChi2[9][15]  = 29.6;

  fChi2[10][0]   = 3.1 ;  fChi2[10][1]   = 3.6 ;  fChi2[10][2]   = 4.6 ;  fChi2[10][3]   = 5.6 ;
  fChi2[10][4]   = 7.0 ;  fChi2[10][5]   = 8.1 ;  fChi2[10][6]   = 10.3;  fChi2[10][7]   = 12.9;
  fChi2[10][8]   = 14.6;  fChi2[10][9]   = 17.3;  fChi2[10][10]  = 19.7;  fChi2[10][11]  = 22.6;
  fChi2[10][12]  = 24.7;  fChi2[10][13]  = 26.8;  fChi2[10][14]  = 29.4;  fChi2[10][15]  = 31.3;

  fChi2[11][0]   = 3.6 ;  fChi2[11][1]   = 4.2 ;  fChi2[11][2]   = 5.2 ;  fChi2[11][3]   = 6.3 ;
  fChi2[11][4]   = 7.8 ;  fChi2[11][5]   = 9.0 ;  fChi2[11][6]   = 11.3;  fChi2[11][7]   = 14.0;
  fChi2[11][8]   = 15.8;  fChi2[11][9]   = 18.5;  fChi2[11][10]  = 21.0;  fChi2[11][11]  = 24.1;
  fChi2[11][12]  = 26.2;  fChi2[11][13]  = 28.3;  fChi2[11][14]  = 30.9;  fChi2[11][15]  = 32.9;

  fChi2[12][0]   = 4.1 ;  fChi2[12][1]   = 4.8 ;  fChi2[12][2]   = 5.9 ;  fChi2[12][3]   = 7.0 ;
  fChi2[12][4]   = 8.6 ;  fChi2[12][5]   = 9.9 ;  fChi2[12][6]   = 12.3;  fChi2[12][7]   = 15.1;
  fChi2[12][8]   = 17.0;  fChi2[12][9]   = 19.8;  fChi2[12][10]  = 22.4;  fChi2[12][11]  = 25.5;
  fChi2[12][12]  = 27.7;  fChi2[12][13]  = 29.8;  fChi2[12][14]  = 32.5;  fChi2[12][15]  = 34.5;

  fChi2[13][0]   = 4.7 ;  fChi2[13][1]   = 5.4 ;  fChi2[13][2]   = 6.6 ;  fChi2[13][3]   = 7.8 ;
  fChi2[13][4]   = 9.5 ;  fChi2[13][5]   = 10.8;  fChi2[13][6]   = 13.3;  fChi2[13][7]   = 16.2;
  fChi2[13][8]   = 18.2;  fChi2[13][9]   = 21.1;  fChi2[13][10]  = 23.7;  fChi2[13][11]  = 26.9;
  fChi2[13][12]  = 29.1;  fChi2[13][13]  = 31.3;  fChi2[13][14]  = 34.0;  fChi2[13][15]  = 36.1;

  fChi2[14][0]   = 5.2 ;  fChi2[14][1]   = 6.0 ;  fChi2[14][2]   = 7.3 ;  fChi2[14][3]   = 8.5 ;
  fChi2[14][4]   = 10.3;  fChi2[14][5]   = 11.7;  fChi2[14][6]   = 14.3;  fChi2[14][7]   = 17.3;
  fChi2[14][8]   = 19.3;  fChi2[14][9]   = 22.3;  fChi2[14][10]  = 25.0;  fChi2[14][11]  = 28.3;
  fChi2[14][12]  = 30.6;  fChi2[14][13]  = 32.8;  fChi2[14][14]  = 35.6;  fChi2[14][15]  = 37.7;

  fChi2[15][0]   = 5.8 ;  fChi2[15][1]   = 6.6 ;  fChi2[15][2]   = 8.0 ;  fChi2[15][3]   = 9.3 ;
  fChi2[15][4]   = 11.2;  fChi2[15][5]   = 12.6;  fChi2[15][6]   = 15.3;  fChi2[15][7]   = 18.4;
  fChi2[15][8]   = 20.5;  fChi2[15][9]   = 23.5;  fChi2[15][10]  = 26.3;  fChi2[15][11]  = 29.6;
  fChi2[15][12]  = 32.0;  fChi2[15][13]  = 34.3;  fChi2[15][14]  = 37.1;  fChi2[15][15]  = 39.3;

  fChi2[16][0]   = 6.4 ;  fChi2[16][1]   = 7.3 ;  fChi2[16][2]   = 8.7 ;  fChi2[16][3]   = 10.1;
  fChi2[16][4]   = 12.0;  fChi2[16][5]   = 13.5;  fChi2[16][6]   = 16.3;  fChi2[16][7]   = 19.5;
  fChi2[16][8]   = 21.6;  fChi2[16][9]   = 24.8;  fChi2[16][10]  = 27.6;  fChi2[16][11]  = 31.0;
  fChi2[16][12]  = 33.4;  fChi2[16][13]  = 35.7;  fChi2[16][14]  = 38.6;  fChi2[16][15]  = 40.8;

  fChi2[17][0]   = 7.0 ;  fChi2[17][1]   = 7.9 ;  fChi2[17][2]   = 9.4 ;  fChi2[17][3]   = 10.9;
  fChi2[17][4]   = 12.9;  fChi2[17][5]   = 14.4;  fChi2[17][6]   = 17.3;  fChi2[17][7]   = 20.6;
  fChi2[17][8]   = 22.8;  fChi2[17][9]   = 26.0;  fChi2[17][10]  = 28.9;  fChi2[17][11]  = 32.3;
  fChi2[17][12]  = 34.8;  fChi2[17][13]  = 37.2;  fChi2[17][14]  = 40.1;  fChi2[17][15]  = 42.3;

  fChi2[18][0]   = 7.6 ;  fChi2[18][1]   = 8.6 ;  fChi2[18][2]   = 10.1;  fChi2[18][3]   = 11.7;
  fChi2[18][4]   = 13.7;  fChi2[18][5]   = 15.4;  fChi2[18][6]   = 18.3;  fChi2[18][7]   = 21.7;
  fChi2[18][8]   = 23.9;  fChi2[18][9]   = 27.2;  fChi2[18][10]  = 30.1;  fChi2[18][11]  = 33.7;
  fChi2[18][12]  = 36.2;  fChi2[18][13]  = 38.6;  fChi2[18][14]  = 41.6;  fChi2[18][15]  = 43.8;

  fChi2[19][0]   = 8.3 ;  fChi2[19][1]   = 9.2 ;  fChi2[19][2]   = 10.9;  fChi2[19][3]   = 21.4;
  fChi2[19][4]   = 14.6;  fChi2[19][5]   = 16.3;  fChi2[19][6]   = 19.3;  fChi2[19][7]   = 22.8;
  fChi2[19][8]   = 25.0;  fChi2[19][9]   = 28.4;  fChi2[19][10]  = 31.4;  fChi2[19][11]  = 35.0;
  fChi2[19][12]  = 37.6;  fChi2[19][13]  = 40.0;  fChi2[19][14]  = 43.0;  fChi2[19][15]  = 45.3;
  
}
 

void DGlobalTools::Dummy(){}

void DGlobalTools::Swap(Float_t *a, Float_t *b){
  Float_t temp;
  temp = *a;
  *a   = *b;
  *b   = temp;
}

//______________________________________________________________________________
// 
float  DGlobalTools::gMyAge() { 
  // another test 
  return 29.;
}


//______________________________________________________________________________
// 
float  DGlobalTools::gPoly(float aArg ,float* p, int aOrder){
  float f = p[0];
  for (int k = 1; k < aOrder; k++){
    f += p[k] * pow(1.*aArg,1.*k);
  }
  return f; 
}


//______________________________________________________________________________
//  
void DGlobalTools::OrderIndexDes(Int_t *aIndex, Float_t *aArg, Int_t aN)
{

  // order an index vector, 
  // so that elements in another vector pointed at 
  // via the ordered vector are descending
  // uses BubbleSort Algorithm, not good for large vectors
  
  Float_t a;
  Int_t   j, i, indxt;

  for(j = 1; j < aN; j++) {
    indxt = aIndex[j];
    a     = aArg[indxt];
    i = j - 1;
    while(i>=0 && aArg[aIndex[i]] < a) {
      aIndex[i+1] = aIndex[i];
      i--;
    }
    aIndex[i+1] = indxt;
  }
}

//______________________________________________________________________________
//  
void DGlobalTools::OrderIndexAsc(Int_t *aIndex, Float_t *aArg, Int_t aN)
{

  // order an index vector, 
  // so that elements in another vector pointed at 
  // via the ordered vector are ascending
  // uses BubbleSort Algorithm, not good for large vectors
  
  Float_t a;
  Int_t   j, i, indxt;

  for(j = 1; j < aN; j++) {
    indxt = aIndex[j];
    a     = aArg[indxt];
    i = j - 1;
    while(i>=0 && aArg[aIndex[i]] > a) {
      aIndex[i+1] = aIndex[i];
      i--;
    }
    aIndex[i+1] = indxt;
  }
}

//______________________________________________________________________________
//  
float DGlobalTools::Poly(float* pArg, float* pParameter)
{
  return poly(pArg[0], pParameter, fNparameter-1);
}

//______________________________________________________________________________
//  
float DGlobalTools::poly(float aArg ,float* p, int aOrder)
{
  float f = p[0];
  for (int k = 1; k < aOrder; k++){
    f += p[k] * pow(1.*aArg,1.*k);
  }
  return f; 
}

//______________________________________________________________________________
//  
void DGlobalTools::SetParameter(float* aParameter, int aN){
  fNparameter = aN;
  for(int k = 0; k < aN; k++)
    fParameter[k] = aParameter[k];
}

//______________________________________________________________________________
//  
void DGlobalTools::Confidence(int tGoodN, int tTotalN, float tConfidenceLimit, 
			      float& tUpperError, float& tLowerError)
{
  // code borrowed from Lee Holloway (UofI/CDF)
  // 980227 via William to me, original in FORTRAN
  // compute probability limits from confidence level

  float b[2]; // = {0.8415, 0.1585} is set by Confidence Level
  b[0] = (1.+ tConfidenceLimit) / 2;
  b[1] = (1.- tConfidenceLimit) / 2;
  
  // set up limits and check for special cases
  int   lim[2];
  lim[0] = tGoodN - 1;
  lim[1] = tGoodN;

  float ans[2];
  float p, d, r, s, term, tmp;
  float eye, entot;

  entot = 1.*tTotalN;
  
  if (tGoodN == 0)
    ans[0] = 0.;
  if (tGoodN == tTotalN)
    ans[1] = 1.;

  if (tGoodN != 0 && tGoodN != tTotalN) {
    for (int k = 0; k < 2; k++) {
      d = 1.;
      p = 1.;
      s = 0.;
      for (int j = 0; j < 20; j++){
	//	d    = sign(d/2, s-b[k]);
	d    = (s-b[k] >= 0) ? fabs(d/2) : -1.*fabs(d/2);
	p   += d; 
	r    = log( p / (1. - p) );
	term = tTotalN * log( 1 - p );
	s    = exp( term );
	for (int i = 1; i <= lim[k]; i++){
	  eye   = 1.*i;
	  tmp   = (entot + 1. - eye) / eye;
	  term += r + log( tmp );                  // log() is the natural logarithm
	  s    += exp( term );
	}
      }
      ans[k] = p;
    }
  }
  tLowerError = ans[0];
  tUpperError = ans[1];
}

//______________________________________________________________________________
//  
double DGlobalTools::ProbabilityIntegral(double z)
{
  // returns the value Phi(z)=1/sqrt(2pi) * Int_-inf^z dx exp(-0.5 * x2)
  // this is a lookup table

  double tProbabilityPhiZ =0.;

  int tProbabilityN = 460;

  int  k = 0;
  int  tFound = 0;

  if(fabs(z) > 4.59) 
    tProbabilityPhiZ = 0.5;
  else {
    while( tFound==0  && k < tProbabilityN) {
      if(fabs(z) >= fProbabilityTest[k] && fabs(z) < fProbabilityTest[k+1]) {
	tProbabilityPhiZ = fProbabilityPhiZ[k];
	tFound = 1;
      }
      k++;
     
    }
  }
  
  return tProbabilityPhiZ;

}

//______________________________________________________________________________
//  
double DGlobalTools::Chi2ToProbability(int tDoF, double tChi2)
{
  // chi-square characterizes the deviation between an approximation and measurements.
  // the approximation has a degree of freedom.
  // the measurements are normal distributed around measurement mean.
  // The distribution of chi-squares at the degree of freedom is known.
  // here: the probability for the event with this chi2 to be combatible with the
  // approximation is the area from chi2 to infinite under the chi2-distribution curve.
  // it is tabulated in e.g. Bronstein Semendjajew, 1.1.2.10 p. 21
  
  int tDoFMax   = 20;  // this value has to fit to the size of the array
  int pBinLimit = 16; // dito
  int pBin;
  if (tDoF <= tDoFMax && tDoF >=1) {
    pBin = 0;
    while(pBin < pBinLimit-1 && fChi2[tDoF-1][pBin] < tChi2 )
      pBin++;
    return fChi2Probability[pBin];
  } else {
    printf("<DGlobalTools::Chi2ToProbability> Degree of Freedom out of bounds\n");
    return 0;
  }
}

//______________________________________________________________________________
//  
double DGlobalTools::CLof7LegendrePol( double *x, double *coeff)
{
  // Return the value of a combination of Legendre polynoms, evaluated at x.
  // The linear combination includes the 1st seventh polynoms 
  //   with the provided coefficients.
  // x SHALL BE BETWEEN -1 and 1 otherwise return 0.!
  //
  // JB 2014/04/10
  
  x[0] /= coeff[0]; 
  
  if(x[0]<-1 || 1<x[0] ) return 0;
  
  return coeff[1] 
      + coeff[2]*x[0] 
      + coeff[3]*(3*x[0]*x[0]-1)/8 
      + coeff[4]*x[0]*(5*x[0]*x[0]-3)/2
      + coeff[5]*(35*x[0]*x[0]*x[0]*x[0]-30*x[0]*x[0]+3)/8 
      + coeff[6]*x[0]*(63*x[0]*x[0]*x[0]*x[0]-70*x[0]*x[0]*x[0]+15)/8
      + coeff[7]*(232*x[0]*x[0]*x[0]*x[0]*x[0]*x[0]-315*x[0]*x[0]*x[0]*x[0]+105*x[0]*x[0]-5)/16;

}

//______________________________________________________________________________
//  
void DGlobalTools::TrackMultiplicity( Int_t nPlanes, Double_t *efficiencyPlane, Double_t *trackMultiplicity) {

  // Compute the expected distribution of #hits per track 
  //  given a number of planes (nPlanes) 
  //  and their individual detection efficiency (efficiencyPlane).
  // The result is stored in array trackMultiplicity;
  //  for which the index stands for the # of hits,
  //  and the value stands for the associated probability.
  //
  // No other effect but the (in)efficiency is taken into account.
  // Note that nPlanes shall indicate the total number of planes
  //  participating to the tracking (so excluding DUT).
  //
  //
  // Created: JB 2014/11/15
  

  // Initialization
  //Double_t *tempMultiplicity = new Double_t[nPlanes+1];
  Double_t *tempMultiplicity = new Double_t[nPlanes+1];
  for (Int_t i=0; i<=nPlanes; i++) {
    trackMultiplicity[i] = 1.;
  }

  // Loop over each plane and update the multiplicity
  Double_t noDetEff, detEff;
  for (Int_t iPlane=0; iPlane<nPlanes; iPlane++) {
    //cout << "Plane " << iPlane << " with eff " << efficiencyPlane[iPlane];
    for (Int_t iMult=0; iMult<=iPlane+1; iMult++) {
      if( iMult<iPlane+1 ) noDetEff = trackMultiplicity[iMult] * (1.-efficiencyPlane[iPlane]);
      else noDetEff = 0.;
      if( iMult>0 ) detEff = trackMultiplicity[iMult-1] * efficiencyPlane[iPlane];
      else detEff = 0.;
      tempMultiplicity[iMult] = noDetEff + detEff;
      //cout << " | mult " << iMult << " (no det " << noDetEff << ", det " << detEff << ")";
    }
    //cout << endl << "   current Sum:";
    for (Int_t iMult=0; iMult<=iPlane+1; iMult++) {
      trackMultiplicity[iMult] = tempMultiplicity[iMult];
      //cout << " | mult " << iMult << " -> " << trackMultiplicity[iMult];
    }
    //cout << endl;
  }

  return;

}

//______________________________________________________________________________
//  

void DGlobalTools::ComputeStripPosition( Int_t mapping, Int_t col, Int_t lin, Double_t &u, Double_t &v, Double_t &w, Int_t stripsNu, Int_t stripsNv, Double_t pitchU, Double_t pitchV, Double_t pitchW) 
{
  
  // Compute the 3D position of the strip at column "col" and line "lin",
  //  set the values in the "u, v, w" variables.
  // The strip position depends on the mapping, look below.
  //
  // BEWARE: if you add a new case here, add it as weel in ComputeStripPosition_UVToColRow
  //
  // JB 2012/11/21
  // Modified CLM 2012/11/24 correction for P31 of M32ter
  // Modified AB 2015/03/31  new case 7 adapted to M22thrb6,7
  // => copied from DPlane on 2015/04/01
  
  switch (mapping) {
      
      // When pixels are organized on an orthogonal grid
    case 1:
    default:
    {
      u = ((2*col - stripsNu + 1 ) * pitchU)/2 ;  
      //printf( " u = (2x%d-%d+1) * %f/2 = %d*%f = %f\n", col, stripsNu, pitchU, (2*col - stripsNu + 1 ), pitchU/2, u);
      v = ((2*lin - stripsNv + 1 ) * pitchV)/2 ;
      //printf( " v = (2x%d-%d+1) * %f/2 = %d*%f = %f\n", lin, stripsNv, pitchV, (2*lin - stripsNv + 1 ), pitchV/2, v);
      break;
    } 
    case 2:
    {
      // When pixels are staggered from one column to the other
      u = ((2*col - stripsNu + 1 ) * pitchU)/2 ;
      
      //if ( col%2 == 0 ) v = (((lin - stripsNv/2.0 + 1./2. ) * pitchV)) + 0.30 * pitchV;  //clm 2012.11.24
      if ( col%2 != 0 ) v = (((lin - stripsNv/2.0 + 1./2. ) * pitchV)) + 0.30 * pitchV;  //clm 2012.11.24
      else              v = (((lin - stripsNv/2.0 + 1./2. ) * pitchV)) - 0.19 * pitchV;  //clm 2012.11.24 
      break;
    }
    case 3:
    {
      //Clm Mapping For M32 L8_2
      if ( lin==7 && col == 10) u = ((2*col - stripsNu + 1 ) * pitchU)/2 ;  
      else           u = -9999; //((2*col+2 - stripsNu + 1 ) * pitchU)/2 ;  
      v = ((2*lin - stripsNv + 1 ) * pitchV)/2 ;
      break;
    }
    case 4:
    {
      // MonteCarlo Simulation. // LC 2014/01/10.
      u = ((2.*col - stripsNu + 2. ) * pitchU)/2. ;  // LC 2015/02/17 : +2 commented                                                                           
      //printf( " u = (2x%d-%d+1) * %f/2 = %d*%f = %f\n", col, stripsNu, pitchU, (2*col - stripsNu + 1 ), pitchU/2, u);
      v = ((2.*lin - stripsNv + 2. ) * pitchV)/2. ;  // LC 2015/02/17 : +2 commented 
      //printf( " v = (2x%d-%d+1) * %f/2 = %d*%f = %f\n", lin, stripsNv, pitchV, (2*lin - stripsNv + 1 ), pitchV/2, v);
      break;
    }
    case 5:
    {
      //Mapping FSBB
      u = (2*col + 1 - stripsNu) * pitchU/2.0;
      double fraction = 0.25;
      //double fraction = 0.75;
      if ( col%2 == 0 ) v = 0.5*(stripsNv - 2*lin - 2*(fraction    )) * pitchV;
      else              v = 0.5*(stripsNv - 2*lin - 2*(1 - fraction)) * pitchV;
      break;
    }
    case 6:
    {
      //Mapping FSBB-bis
      u = (2*col + 1 - stripsNu) * pitchU/2.0;
      //double fraction = 0.25;
      double fraction = 0.75;
      if ( col%2 == 0 ) v = 0.5*(stripsNv - 2*lin - 2*(fraction    )) * pitchV;
      else              v = 0.5*(stripsNv - 2*lin - 2*(1 - fraction)) * pitchV;
      break;
    }
    case 7:
    {
      //M22-THRB7 and B6 -> set the matrix type  and Mapping accordingly in the config file !
      u = ((2*col - stripsNu + 1 ) * pitchU)/2 ;       
      if ( col%2 == 0 ) v = (((lin - stripsNv/2.0 + 1./2. ) * pitchV)) - 0.50 * pitchV;  //correct +0.5
      else              v = (((lin - stripsNv/2.0 + 1./2. ) * pitchV)) - 0.00 * pitchV;  //correct -0.5
      //cout<<"  DPlane::ComputeStripPosition "<<u<<" "<<v<<" "<<col<<" "<<lin<<endl;
      break;
    }
      
  }
  
  
  w = pitchW;
  
  //printf(" --> N(%d,%d), pitch(%.1f,%.1f), (%d, %d): %.1f, %.1f\n", stripsNu, stripsNv, pitchU, pitchV, col, lin, u, v);
  
}

//______________________________________________________________________________
//  
void DGlobalTools::ComputeStripPosition_UVToColRow( Int_t mapping, Double_t u, Double_t v, double &col, double &lin, Int_t stripsNu, Int_t stripsNv, Double_t pitchU, Double_t pitchV)
{
  
  // Compute the 2D position of a strip in the variables u and v to the set of variables column and line
  // The strip position depends on the mapping, look below.
  //
  // BEWARE: if you add a new case here, add it as weel in ComputeStripPosition
  //
  // AP 2014/06/26
  // Modified AB 2015/03/31  new case 7 adapted to M22thrb6,7
  // => copied from DPlane on 2015/04/01

  //cout << pitchU << "  " << pitchV << endl;
  
  switch (mapping) {
      // When pixels are organized on an orthogonal grid
    case 1:
    default:
    {
      col = (u/pitchU) + ((stripsNu - 1)/2.);
      lin = (v/pitchV) + ((stripsNv - 1)/2.);
      break;
    }
    case 2:
    {
      // When pixels are staggered from one column to the other
      col = (u/pitchU) + ((stripsNu - 1)/2.);
      int IntCol = RoundOff(col);
      if(IntCol%2 != 0) lin = (v/pitchV) + ((stripsNv - 1)/2.) - 0.30;
      else              lin = (v/pitchV) + ((stripsNv - 1)/2.) + 0.19;
      break;
    }
    case 3:
    {
      //Clm Mapping For M32 L8_2
      col = (u/pitchU) + ((stripsNu - 1)/2.);
      lin = (v/pitchV) + ((stripsNv - 1)/2.);
      break;
    }
    case 4:
    {
      // MonteCarlo Simulation. // LC 2014/01/10.
      col = (u/pitchU) + ((stripsNu /*- 2*/)/2.); // LC 2015/02/17 : -2 commented
      lin = (v/pitchV) + ((stripsNv /*- 2*/)/2.); // LC 2015/02/17 : -2 commented
      break;
    } 
    case 5:
    {
      //Mapping FSBB
      col = (u/pitchU) + ((stripsNu - 1)/2.);
      
      //double fraction = 0.50;
      //lin = ((stripsNv - 2*(fraction    ))/2.0)  -  (v/pitchV);
      
      double fraction = 0.25;
      int IntCol = RoundOff(col);
      if(IntCol%2 == 0) lin = ((stripsNv - 2*(fraction    ))/2.0)  -  (v/pitchV);
      else              lin = ((stripsNv - 2*(1 - fraction))/2.0)  -  (v/pitchV);
      
      break;
    }
    case 6:
    {
      //Mapping FSBB-bis
      col = (u/pitchU) + ((stripsNu - 1)/2.);
      
      //double fraction = 0.50;
      //lin = ((stripsNv - 2*(fraction    ))/2.0)  -  (v/pitchV);
      
      double fraction = 0.75;
      int IntCol = RoundOff(col);
      if(IntCol%2 == 0) lin = ((stripsNv - 2*(fraction    ))/2.0)  -  (v/pitchV);
      else              lin = ((stripsNv - 2*(1 - fraction))/2.0)  -  (v/pitchV);
      
      break;
    }
    case 7:
    {
      //M22-THRB7 and B6 -> set the matrix type and Mapping accordingly in the config file !
      col = (u/pitchU) + ((stripsNu - 1)/2.);
      if(int(col)%2 == 0) lin = (v/pitchV) + ((stripsNv - 1)/2.) + 0.50; //correct -0.5
      else                lin = (v/pitchV) + ((stripsNv - 1)/2.) + 0.0; //correct +0.5
      //cout<<"  DPlane::ComputeStripPosition_UVToColRow "<<u<<" "<<v<<" "<<col<<" "<<lin<<endl;
      break;
    }
    
  }
  
  return;
  
}

//______________________________________________________________________________
//  
char* DGlobalTools::LocalizeDirName( const char *inputString) {

  // JB 2011/07/07
  
  TString *outputString = new TString( inputString);
  LocalizeDirName( outputString);
  return (Char_t*)outputString->Data();

}  
  //______________________________________________________________________________
//  
void DGlobalTools::LocalizeDirName( TString *inputString) {
  
  // Replace "/" or "\\" characters in path name for directories
  // with the correct charachter according to the system used
  //
  // JB 2011/07/07
  
  TString windowsSep = "\\";
  TString unixSep = "/";
  
  // By default we turn any \ into a / for LINUX/UNIX
  TString searchedStr = windowsSep;
  TString replacementStr = unixSep;
  
  // Is system is WINDOWS, we switch the strings
  TString sysType = gSystem->GetBuildArch();
  if( sysType.Contains("win") || sysType.Contains("WIN") ) {
    searchedStr = unixSep;
    replacementStr = windowsSep;
  }
  
  //cout << "Input string: " << inputString << endl;
  
  inputString->ReplaceAll( searchedStr.Data(), replacementStr.Data());
  
  //cout << "Output string: " << inputString->Data() << endl;
  
}


//______________________________________________________________________________
//  
double DGlobalTools::BuildVertex( double *point11, double *point12, double *point21, double *point22, double *vertex) {

  // Compute the point of closest approach (or vertex) between two straight lines.
  // Inputs are made of two 3D points (vectors of 3 values) of each line:
  //  line 1: point11, point12;
  //  line 2: point21, point22.
  // Output is a point made up of a vector of 3 coordinates. 
  //  In the compuation is impossible (div by 0), returns (0,0,0).
  // Return the shortest distance between the two lines (0. if no computation).
  //
  // Author: Pierre HENRIQUET, 2011
  // Adapted by JB, 2011/07/26

  Int_t debug = 0;
  
  // output
  vertex[0] = vertex[1] = vertex[2] = 0.;
  double smallestDist = 0.;
  
  // inputs
  double *A = point11;
  double *B = point12;
  double *C = point21;
  double *D = point22;
  
  if(debug) printf("Inputs: line1{ (%.1f,%.1f,%.1f), (%.1f,%.1f,%.1f)}, line2{ (%.1f,%.1f,%.1f), (%.1f,%.1f,%.1f)}\n", A[0],A[1],A[2], B[0],B[1],B[2], C[0],C[1],C[2], D[0],D[1],D[2]);

  // ====================
  // Nothing was changed from the original version below this line
  
  double AB[3] = {B[0]-A[0],B[1]-A[1],B[2]-A[2]};
  double CD[3] = {D[0]-C[0],D[1]-C[1],D[2]-C[2]};
  double BC[3] = {C[0]-B[0],C[1]-B[1],C[2]-B[2]};
  
  double ABvectCD[3];
  ABvectCD[0] = AB[1]*CD[2]-AB[2]*CD[1];
  ABvectCD[1] = AB[2]*CD[0]-AB[0]*CD[2];
  ABvectCD[2] = AB[0]*CD[1]-AB[1]*CD[0];
  double NormABvectCD = sqrt(pow(ABvectCD[0],2)+pow(ABvectCD[1],2)+pow(ABvectCD[2],2));
  
  if (NormABvectCD!=0. && ABvectCD[1]!=0. && CD[0]!=0.)
  {
    
    double n[3];
    n[0] = ABvectCD[0]/NormABvectCD;
    n[1] = ABvectCD[1]/NormABvectCD;
    n[2] = ABvectCD[2]/NormABvectCD;
    
    double distance = BC[0]*n[0]+BC[1]*n[1]+BC[2]*n[2];
    
    double Bprime[3];
    Bprime[0] = distance*n[0] + B[0];
    Bprime[1] = distance*n[1] + B[1];
    Bprime[2] = distance*n[2] + B[2];
    
    double U = C[2]*CD[0]-CD[2]*C[0];
    double V = Bprime[2]*AB[0]-AB[2]*Bprime[0];
    
    double P1[3];
    P1[0] = (CD[0]*V-AB[0]*U)/(-1.*ABvectCD[1]);
    P1[1] = (CD[1]*(P1[0]-C[0])/CD[0])+C[1];
    P1[2] = (CD[2]*(P1[0]-C[0])/CD[0])+C[2];
    
    double P2[3];
    P2[0] = P1[0]+B[0]-Bprime[0];
    P2[1] = P1[1]+B[1]-Bprime[1];
    P2[2] = P1[2]+B[2]-Bprime[2];
    
    vertex[0] = (P1[0]+P2[0])/2.;
    vertex[1] = (P1[1]+P2[1])/2.;
    vertex[2] = (P1[2]+P2[2])/2.;
    
    smallestDist = sqrt( (P1[0]-P2[0])*(P1[0]-P2[0])+(P1[1]-P2[1])*(P1[1]-P2[1])+(P1[2]-P2[2])*(P1[2]-P2[2]) );

  }
  else 
  {
    printf("BuildVertex: WARNING NormABvectCD=%f, ABvectCD[1]=%f, CD[0]=%f --> returning distance = -1 and position 0,0,0!!\n", NormABvectCD, ABvectCD[1], CD[0]);
    return -1;    
  }
  // ====================
  if(debug) printf("Output: vertex (%.1f,%.1f,%.1f), distance %f\n", vertex[0],vertex[1],vertex[2], smallestDist);

  return smallestDist;
}

//______________________________________________________________________________
//
Double_t DGlobalTools::BuildVertex( DR3 &pointA1, DR3 &pointA2, DR3 &pointB1, DR3 &pointB2, DR3 &intersection)
{
  // VR 2014/06/29 
  
  // inputs
  Double_t A[3] ;
  pointA1.Convert2DoubleArray(A) ;
  Double_t B[3] ;
  pointA2.Convert2DoubleArray(B) ;
  Double_t C[3] ;
  pointB1.Convert2DoubleArray(C) ;
  Double_t D[3] ;
  pointB2.Convert2DoubleArray(D) ;
  Double_t I[3];

  Double_t distance = BuildVertex(A, B, C, D, I);

  intersection.SetValue(I);

  return distance;

}
//______________________________________________________________________________
//  
void DGlobalTools::SetVetoPixel( int noiseRun) {
  
  // Match the corresponding vetoFunction with the noiseRun specified.
  // If no matching is found, the function pointer stays empty which
  //  will result in no veto.
  //
  // Created JB 2012/03/11
  
  if( noiseRun == 26166 )
  {
    printf( "  VETO PIXELS : configuration is IPNL telescope for GANIL 07.2012, RSB 8, 0.1%% over 10.000 evnts (run26166 )\n");
    VetoPixel = vetoPixel_IPNLtelescopeV01;
  }
  else if( noiseRun == 26167 )
  {
    printf( "  VETO PIXELS : configuration is IPNL telescope for HIT 12.2012, RSB 8, 0.1%% over 10.000 evnts (run1)\n");
    VetoPixel = vetoPixel_IPNLtelescopeV02;
  }
  else if( noiseRun == 26168 )
  {
    printf( "  VETO PIXELS : configuration is IPNL telescope for GSI 08.2012, RSB 8, 0.1%% over 10.000 evnts (run21+51)\n");
    VetoPixel = vetoPixel_IPNLtelescopeV03;
  }
  else if( noiseRun == 26169 )
  {
    printf( "  VETO PIXELS : configuration is IPNL telescope for GSI 08.2012, RSB 8, 0.01%% over 1.000.000 evnts (run51+21) EXCEPT for planes 1Left and 1Right 1%%\n");
    VetoPixel = vetoPixel_IPNLtelescopeV04;
  }
  else if( noiseRun == 26170 )
  {
    printf( "  VETO PIXELS : configuration is IPNL telescope for GSI 08.2012, RSB 6, 0.01%% over 1.000.000 evnts (run53) EXEPT for planes 1Left and 1Right (using 26169)\n");
    VetoPixel = vetoPixel_IPNLtelescopeV05;
  }
  else if( noiseRun == 26171 )
  {
    printf( "  VETO PIXELS : configuration is IPNL telescope for HIT2 04.2013, RSB 8, 0.01%% over 1.000.000 evnts (run55) EXEPT for planes 1Left and 1Right (using 26169)\n");
    VetoPixel = vetoPixel_IPNLtelescopeV06;
  }
  else if( noiseRun == 26180 ) 
  {
    printf( "  VETO PIXELS : configuration is IPNL new telescope for GANIL 04.2014, RSB 8, 0.01%% over 180.000 evnts (runs 100-103)\n");
    VetoPixel = vetoPixel_IPNLtelescopeV10;
  }
  else if ( noiseRun == 26666 ) {
    printf( "  VETO PIXELS : configuration is MIMOSA 26 telescope (run 26666)\n");
    VetoPixel = vetoPixel_Mi26telescope;
  }
  else if ( noiseRun == 28666 ) {
    printf( "  VETO PIXELS : configuration is MIMOSA 28 telescope (run 28666)\n");
    VetoPixel = vetoPixel_Mi28telescope;
  }
  else if ( noiseRun == 128666 ) {
    printf( "  VETO PIXELS : configuration is MIMOSA 28 - GSI telescope (run 128666)\n");
    VetoPixel = vetoPixel_Mi28telescopeGSI;
  }
  else if ( noiseRun == 26667 ) {
    printf( "  VETO PIXELS : configuration is MIMOSA 26 - Aarhus (run 26667)\n");
    VetoPixel = vetoPixel_Mi26Aarhus;
  }
  else if ( noiseRun == 700 ) {
    printf( "  VETO PIXELS : configuration is MIMOSA 26 - SOLEIL (run 700)\n");
    VetoPixel = vetoPixel_Mi26SOLEIL;
  }
  else {
    printf( "  no VETO PIXELS used.\n");
    VetoPixel = NULL;
  }
  
}
//______________________________________________________________________________
//
double DGlobalTools::scatteringAngle(string particle,
				     double momentum,
				     string material,
				     double thickness,
				     bool verbose) {

  // Return the theta_rms of multiple scattering
  // for a given momentum, thickness and material
  //
  // Note that:
  //  * momentum  unit is GeV/c.
  //  * thickness unit is microns (um).
  //
  // If particle or material unknown, return 0.
  //
  // JB 2012/10/31

  std::map<string,double> mass;   // in GeV/c2
  std::map<string,int>    charge; // in units of electron charge
  mass["electron"]   = 511e-6;
  charge["electron"] = 1;
  mass["pion"]       = 0.1396;
  charge["pion"]     = 1;
  mass["kaon"]       = 0.4937;
  charge["kaon"]     = 1;
  mass["proton"]     = 0.9383;
  charge["proton"]   = 1;
  mass["muon"]       = 105.66e-6;
  charge["muon"]     = 1;

  std::map<string,double> X0; // um
  X0["copper"]    =    1.43e+4;
  X0["silicon"]   =    9.36e+4;
  X0["aluminum"]  =    8.8900e+4;
  X0["beryllium"] =   35.28e+4;
  X0["epoxy"]     =   30.e+4;
  X0["kapton"]    =   28.e+4;
  X0["glass"]     =   12.30e+4;
  X0["nai"]       =    2.59e+4;
  X0["csi"]       =    1.86e+4;
  X0["tungsten"]  =    0.35e+4;
  X0["iron"]      =    1.76e+4;
  X0["water"]     =   36.08e+4;
  X0["DryAir"]    =   30.39e+7;
  X0["Vacuum"]    =    1.00e+20;
  X0["mylar"]     =   28.7e+4;
  X0["CarbonFiber"] =   23.7e+4;

  if(mass.count(particle)<1 /*mass[particle]<=0.*/ ) {
    fprintf( stderr, "Particle type %s is unknown!\n", particle.data());
    cout << "Paricle should be: ";
    for( map<string, double>::iterator i=mass.begin(); i!=mass.end(); ++i) cout << (*i).first << " ";
    cout << endl;

    assert(false);
  }

  if(X0.count(material)<1 ) {
    fprintf( stderr, "Material type %s is unknown!\n", material.data());
    cout << "Material should be: ";
    for(map<string, double>::iterator i=X0.begin(); i!=X0.end(); ++i) cout << (*i).first << " ";
    cout << endl;

    assert(false);
  }

  double energy = sqrt(pow(mass[particle],2.) + pow(momentum,2.));
  double beta   = momentum/energy;
  double sigMS  = (0.0136/(beta*momentum))*charge[particle]*sqrt(thickness/X0[material])*(1.+0.038*log(thickness/X0[material]));

  if(verbose) {
    printf("\n ========= mult. scat. ==========\n");  
    printf(" %s of energy %e GeV, momentum %e GeV/c, beta %e\n", particle.data(), energy, momentum, beta);
    printf(" particle is %s ==> beta*p = %ex%e = %e\n",          particle.data(), beta, momentum, beta*momentum);
    printf(" in %s of thickness %.0f um, X0=%f um\n",            material.data(), thickness, X0[material]);
    printf("   ==> Theta_rms = %e rad (%e deg)\n\n",             sigMS,sigMS*(180.0/TMath::Pi()));
  }

  return sigMS;

}
//______________________________________________________________________________
//
double DGlobalTools::Getb(string particle,
			  string material,
			  double momentum,
			  double thickness)
{

  //Implementing multiple scattering including non-gaussian tails as in 
  //PR Vol 89, Number 6, March 15, 1953
  //the b parameter is material dependent (material type and thickness), 
  //as well as particle dependent (it velocity beta)
  //Use equation (22) of that paper to evaluate it

  double alpha = 7.297352568e-3;
  double mass;
  int charge;

  if(TString("electron") == TString(particle.data())) {
    mass   = 511e-6;
    charge = 1;
  }
  else if(TString("pion") == TString(particle.data())) {
    mass   = 0.1396;
    charge = 1;
  }
  else if(TString("kaon") == TString(particle.data())) {
    mass   = 0.4937;
    charge = 1;
  }
  else if(TString("proton") == TString(particle.data())) {
    mass   = 0.9383;
    charge = 1;
  }
  else {
    cout << endl;
    cout << "Specified particle " << particle.data() << " is not include in our data base." << endl;
    cout << " - electron" << endl;
    cout << " - pion"     << endl;
    cout << " - kaon"     << endl;
    cout << " - proton"   << endl;
    cout << "Check your inputs. Exiting now!!!" << endl;
    cout << endl;
    assert(false);
  }

  //Matrial properties
  int    Z;   //atomic number
  double A;   //atomic weight, gr/mol
  double rho; //density, in gr/cm^3

  if(TString("copper") == TString(material.data())) {
    Z   = 29;
    A   = 63.546000;
    rho = 8.960;
  }
  else if(TString("silicon") == TString(material.data())) {
    Z   = 14;
    A   = 28.085500;
    rho = 2.330;
  }
  else if(TString("aluminum") == TString(material.data())) {
    Z   = 13;
    A   = 26.981539;
    rho = 2.700;
  }
  else if(TString("beryllium") == TString(material.data())) {
    Z   =  4;
    A   =  9.012182;
    rho =  1.848;
  }
  else if(TString("tungsten") == TString(material.data())) {
    Z   =  74;
    A   = 183.840000;
    rho =  19.300;
  }
  else if(TString("iron") == TString(material.data())) {
    Z   =  26;
    A   =  55.845000;
    rho =   7.870;
  }
  else if(TString("DryAir") == TString(material.data())) {
    double ZOA = 0.49919;
    A   =  28.97;
    rho =   1.205e-3;
    Z   =  ZOA*A;
  }
  else if(TString("Vacuum") == TString(material.data())) {
    double ZOA = 0.49919;
    A   =  28.97;
    rho =   1.0e-12;
    Z   =  ZOA*A;
  }
  else {
    cout << endl;
    cout << "Specified material " << material.data() << " is not include in our data base." << endl;
    cout << " - copper"    << endl;
    cout << " - silicom"   << endl;
    cout << " - aluminium" << endl;
    cout << " - beryllium" << endl;
    cout << " - tungsten"  << endl;
    cout << " - iron"      << endl;
    cout << " - DryAir"    << endl;
    cout << " - Vacuum"    << endl;
    cout << "Check your inputs. Exiting now!!!" << endl;
    cout << endl;
    assert(false);
  }

  double energy = sqrt(pow(mass,2) + pow(momentum,2));
  double beta   = momentum/energy;
  double t      = thickness*1.0e-4*rho;

  double b = ((6680*t)/pow(beta,2))*(((Z+1)*pow(Z,1/3.0)*pow((float)charge,2))/(A*(1.0 + 3.34*pow(alpha,2))));
  b = TMath::Log(b);

  return b;

}
//______________________________________________________________________________
//
double DGlobalTools::GetBfromb(double b)
{

  //Implementing multiple scattering including non-gaussian tails as in 
  //PR Vol 89, Number 6, March 15, 1953
  //B is an expansion parameter for corrections to the MS Gaussian model.
  //See Eq. (25) of the paper.
  //The B parameter is a function of b as in equation (23) of the paper.
  //This function solves the transcendental equation using the Newton 
  //iterative method

  double epsilon = 1.0e-8;

  double B0 = b;
  while(B0 < 1) {
    B0 *= 10;
  }

  double Bn   = B0;
  double Bnp1 = (Bn/(Bn-1))*(TMath::Log(Bn) + b - 1.0);
  
  while(TMath::Abs(Bnp1 - Bn) > epsilon) {
    Bn = Bnp1;
    Bnp1 = (Bn/(Bn-1))*(TMath::Log(Bn) + b - 1.0);
  }

  return Bnp1;

}
//______________________________________________________________________________
//
double DGlobalTools::Getf0(double Theta)
{

  //Implementing multiple scattering including non-gaussian tails as in 
  //PR Vol 89, Number 6, March 15, 1953
  //Guassian component of the MS model, the f0 function of Eq. (25) of the paper

  //Theta is actually the ratio Theta = theta_space/(sqrt(2)*theta_0),
  //Where: theta_space is the spatial scattering angle and theta_0 is the 
  //RMS of the guassian model of the scattering angle

  return 2.0*TMath::Exp(-pow(Theta,2));

}
//______________________________________________________________________________
//
void DGlobalTools::Getf1Andf2(double Theta,
			      double& f1,
			      double& f2)
{

  //Implementing multiple scattering including non-gaussian tails as in 
  //PR Vol 89, Number 6, March 15, 1953
  //This function returns the corrections to the Gaussian behaviour of the MS,
  //the functions f1 and f2 of Eq. (25) of the paper.
  //In this function there is no analytical implementation, but a linear 
  //interpolarion of the calculated points shown in table II of the paper.

  //Theta is actually the ratio Theta = theta_space/(sqrt(2)*theta_0),
  //Where: theta_space is the spatial scattering angle and theta_0 is the 
  //RMS of the guassian model of the scattering angle

  const int Npoints(29);

  double Theta_Values[Npoints];
  double F1_Values[Npoints];
  double F2_Values[Npoints];
  int counter = 0;
  Theta_Values[counter] =  0.0; F1_Values[counter] =  0.8456;  F2_Values[counter] =  2.4929;
  counter++;
  Theta_Values[counter] =  0.2; F1_Values[counter] =  0.7038;  F2_Values[counter] =  2.0694;
  counter++;
  Theta_Values[counter] =  0.4; F1_Values[counter] =  0.3437;  F2_Values[counter] =  1.0488;
  counter++;
  Theta_Values[counter] =  0.6; F1_Values[counter] = -0.0777;  F2_Values[counter] = -0.0044;
  counter++;
  Theta_Values[counter] =  0.8; F1_Values[counter] = -0.3981;  F2_Values[counter] = -0.6068;
  counter++;
  Theta_Values[counter] =  1.0; F1_Values[counter] = -0.5285;  F2_Values[counter] = -0.6359;
  counter++;
  Theta_Values[counter] =  1.2; F1_Values[counter] = -0.4770;  F2_Values[counter] = -0.3086;
  counter++;
  Theta_Values[counter] =  1.4; F1_Values[counter] = -0.3183;  F2_Values[counter] =  0.0525;
  counter++;
  Theta_Values[counter] =  1.6; F1_Values[counter] = -0.1396;  F2_Values[counter] =  0.2423;
  counter++;
  Theta_Values[counter] =  1.8; F1_Values[counter] = -0.0006;  F2_Values[counter] =  0.2386;
  counter++;
  Theta_Values[counter] =  2.0; F1_Values[counter] =  0.0782;  F2_Values[counter] =  0.1316;
  counter++;
  Theta_Values[counter] =  2.2; F1_Values[counter] =  0.1054;  F2_Values[counter] =  0.0196;
  counter++;
  Theta_Values[counter] =  2.4; F1_Values[counter] =  0.1008;  F2_Values[counter] = -0.0467;
  counter++;
  Theta_Values[counter] =  2.6; F1_Values[counter] =  0.08262; F2_Values[counter] = -0.0649;
  counter++;
  Theta_Values[counter] =  2.8; F1_Values[counter] =  0.06247; F2_Values[counter] = -0.0546;
  counter++;
  Theta_Values[counter] =  3.0; F1_Values[counter] =  0.04550; F2_Values[counter] = -0.03568;
  counter++;
  Theta_Values[counter] =  3.2; F1_Values[counter] =  0.03288; F2_Values[counter] = -0.01923;
  counter++;
  Theta_Values[counter] =  3.4; F1_Values[counter] =  0.02402; F2_Values[counter] = -0.00847;
  counter++;
  Theta_Values[counter] =  3.6; F1_Values[counter] =  0.01791; F2_Values[counter] = -0.00264;
  counter++;
  Theta_Values[counter] =  3.8; F1_Values[counter] =  0.01366; F2_Values[counter] =  0.00005;
  counter++;

  Theta_Values[counter] =  4.0; F1_Values[counter] =  10.638e-3;  F2_Values[counter] =  1.0741e-3;
  counter++;
  Theta_Values[counter] =  4.5; F1_Values[counter] =   6.140e-3;  F2_Values[counter] =  1.2294e-3;
  counter++;
  Theta_Values[counter] =  5.0; F1_Values[counter] =   3.831e-3;  F2_Values[counter] =  0.8326e-3;
  counter++;
  Theta_Values[counter] =  5.5; F1_Values[counter] =   2.527e-3;  F2_Values[counter] =  0.5368e-3;
  counter++;
  Theta_Values[counter] =  6.0; F1_Values[counter] =   1.739e-3;  F2_Values[counter] =  0.3495e-3;
  counter++;
  Theta_Values[counter] =  7.0; F1_Values[counter] =   0.9080e-3; F2_Values[counter] =  0.1584e-3;
  counter++;
  Theta_Values[counter] =  8.0; F1_Values[counter] =   0.5211e-3; F2_Values[counter] =  0.0783e-3;
  counter++;
  Theta_Values[counter] =  9.0; F1_Values[counter] =   0.3208e-3; F2_Values[counter] =  0.0417e-3;
  counter++;
  Theta_Values[counter] = 10.0; F1_Values[counter] =   0.2084e-3; F2_Values[counter] =  0.0237e-3;
  counter++;

  if(Theta > Theta_Values[Npoints-1]) {
    cout << endl;
    cout << "Currently only able to evaluate functions up to Theta = 10. Exiting now!!!" << endl;
    cout << endl;
    assert(false);
  }

  for(int i=0;i<Npoints-1;i++) {
    if(Theta >= Theta_Values[i] && 
       Theta <  Theta_Values[i+1]) {

      double a_f1 = (F1_Values[i+1] - F1_Values[i])/(Theta_Values[i+1] - Theta_Values[i]);
      double b_f1 =  F1_Values[i+1] - a_f1*Theta_Values[i+1];
      f1 = a_f1*Theta + b_f1;

      double a_f2 = (F2_Values[i+1] - F2_Values[i])/(Theta_Values[i+1] - Theta_Values[i]);
      double b_f2 =  F2_Values[i+1] - a_f2*Theta_Values[i+1];
      f2 = a_f2*Theta + b_f2;

      break;
    }
  }

  return;

}
//______________________________________________________________________________
//
double DGlobalTools::GetDistribution(double Theta,
				     double B)
{

  //Implementing multiple scattering including non-gaussian tails as in 
  //PR Vol 89, Number 6, March 15, 1953
  //This function returns the full distribution of spatial scattering angle
  // f(theta_space)(theta_space)d_theta_space = (Theta)(d_Theta)[f0(Theta) + Be-1*f1(Theta) + Be-2*f2(Theta)]
  // B is the parameter described in Function GetBfromb(double b)

  //Theta is actually the ratio Theta = theta_space/(sqrt(2)*theta_0),
  //Where: theta_space is the spatial scattering angle and theta_0 is the 
  //RMS of the guassian model of the scattering angle

  double f0 = Getf0(Theta);
  double f1,f2;
  Getf1Andf2(Theta,f1,f2);

  double f = f0 + (f1/B) + (f2/pow(B,2));

  return f;

}
//______________________________________________________________________________
//
int  DGlobalTools::RoundOff(double a)
{

  int a_int = int(a);
  double delta = a - a_int;
  if(delta <  0.5) return a_int;
  else             return a_int + 1;

}
//______________________________________________________________________________
//
//NEXT FUNCTIONS ADDED BY JH (4/4/2018) FOR FITTING PURPOSES IN MRAW::XRAYANALYSIS



//====================================================================================================================================================
double DGlobalTools::fpeaks(double *x, double *par, int npeaks)
{

  // Gaussian Peaks fitting function
  double result = 0;
  for(int p=0; p<npeaks; p++) {
    double norm  = par[5*p];// Fit parameters start at 3*p to avoid double writing with another function
    double mean  = par[5*p+1];
    double sigma = par[5*p+2];
    result += norm*TMath::Gaus(x[0],mean,sigma,kTRUE); //kTrue for normalisation
  }

  return result;

}
//====================================================================================================================================================
double DGlobalTools::CBfunction(double x,
                  double mean,
		  double sigma,
		  double alpha,
		  double n)
{
  // Crystal ball function (https://en.wikipedia.org/wiki/Crystal_Ball_function)

  double absalpha = TMath::Abs(alpha);
  double frac     = (x-mean)/sigma;
  double A        = pow(n/absalpha,n)*exp(-pow(absalpha,2)/2);
  double B        = (n/absalpha)-absalpha;

  double C        = sigma*(n/absalpha)*(1/(n-1))*exp(-0.5*pow(absalpha,2));
  double D        = sigma*sqrt(0.5*TMath::Pi())*(TMath::Erf(absalpha/sqrt(2)) + 1);
  double N        = 1.0/(C+D);

  double result = 0.0;
  if(frac > -alpha) result = exp(-0.5*pow(frac,2));
  else              result = A*pow(B-frac,-n);
  result *= N;

  return result;

}
//====================================================================================================================================================
double DGlobalTools::BifurcatedCBfunction(double x,
			    double mean,
			    double sigmaL,
			    double sigmaR,
			    double alpha,
			    double n)
{
  // Crystal ball function (https://en.wikipedia.org/wiki/Crystal_Ball_function)

  double absalpha = TMath::Abs(alpha);
  double fracL    = (x-mean)/sigmaL;
  double fracR    = (x-mean)/sigmaR;
  double A        = pow(n/absalpha,n)*exp(-pow(absalpha,2)/2);
  double B        = (n/absalpha) - absalpha;

  double C        = sigmaL*(n/absalpha)*(1/(n-1))*exp(-0.5*pow(absalpha,2));
  double DL       = sigmaL*sqrt(0.5*TMath::Pi())*(TMath::Erf(absalpha/sqrt(2)));
  double DR       = sigmaR*sqrt(0.5*TMath::Pi());
  double N        = 1.0/(C + DL + DR);

  double result = 0.0;
  if(x-mean >= 0.0)    result = exp(-0.5*pow(fracR,2));
  else {
    if(fracL > -alpha) result = exp(-0.5*pow(fracL,2));
    else               result = A*pow(B-fracL,-n);
  }
  result *= N;

  return result;

}



//====================================================================================================================================================
double DGlobalTools::DoublePeakCBFitFunction55Fe(double x,
			       double mean1,
			       double mean2,
			       double sigma1,
			       double sigma2,
			       double alpha1,
			       double alpha2,
			       double n1,
			       double n2,
			       double N1,
			       double N2)
{

  // model to fit the two lines of the Fe55 source with two CB functions with the same
  // sigma, alpha, n and mean values differing by the same amount as te Kalpha and Kbeta lines
  // The N1 and N2 parameters are the number of events of each peak

/*  double mean1  = mean;
  double mean2  = mean1*(6.49045/5.89502);
  double sigma1 = sigma;
  double sigma2 = sigma;
  double alpha1 = alpha;
  double alpha2 = alpha;
  double n1     = n;
  double n2     = n;*/

  double result  = 0.0;
  result        += N1*DGlobalTools::CBfunction(x,mean1,sigma1,alpha1,n1);
  result        += N2*DGlobalTools::CBfunction(x,mean2,sigma2,alpha2,n2);

  return result;

}
//====================================================================================================================================================
double DGlobalTools::fpeaksCB55Fe(double *x, double *par)
{

  //fit function to fit the

  //parameter list
  //par[0] = mean;
  //par[1] = sigma
  //par[2] = alpha
  //par[3] = n
  //par[4] = N1
  //par[5] = N2

  double mean1  = par[0];
  double mean2  = par[1];
  double sigma1 = par[2];
  double sigma2 = par[3];
  double alpha1 = par[4];
  double alpha2 = par[5];
  double n1     = par[6];
  double n2     = par[7];
  double N1    = par[8];
  double N2    = par[9];

  return DGlobalTools::DoublePeakCBFitFunction55Fe(x[0],mean1,mean2,sigma1,sigma2,alpha1,alpha2,n1,n2,N1,N2);

}
//====================================================================================================================================================
double DGlobalTools::DoublePeakBifurcatedCBFitFunction55Fe(double x,
					 double mean1,
					 double mean2,
					 double sigmaL1,
					 double sigmaL2,
					 double sigmaR1,
					 double sigmaR2,
					 double alpha1,
					 double alpha2,
					 double n1,
					 double n2,
					 double N1,
					 double N2)
{

  // model to fit the two lines of the Fe55 source with two CB functions with the same
  // sigma, alpha, n and mean values differing by the same amount as te Kalpha and Kbeta lines
  // The N1 and N2 parameters are the number of events of each peak

/*  double mean1   = mean;
  double mean2   = mean1*(6.49045/5.89502);
  double sigmaL1 = sigmaL;
  double sigmaL2 = sigmaL;
  double sigmaR1 = sigmaR;
  double sigmaR2 = sigmaR;
  double alpha1  = alpha;
  double alpha2  = alpha;
  double n1      = n;
  double n2      = n;*/
  //mean2   = mean1*(6.49045/5.89502);

  double result  = 0.0;
  result        += N1*DGlobalTools::BifurcatedCBfunction(x,mean1,sigmaL1,sigmaR1,alpha1,n1);
  result        += N2*DGlobalTools::BifurcatedCBfunction(x,mean2,sigmaL2,sigmaR2,alpha2,n2);

  return result;

}
//====================================================================================================================================================
double DGlobalTools::fpeaksBifurcatedCB55Fe(double *x, double *par)
{

  //fit function to fit the

  //parameter list
  //par[0] = mean;
  //par[1] = sigmaL
  //par[2] = sigmaR
  //par[3] = alpha
  //par[4] = n
  //par[5] = N1
  //par[6] = N2

  double mean1  = par[0];
  double mean2  = par[1];
  double sigmaL1= par[2];
  double sigmaL2= par[3];
  double sigmaR1= par[4];
  double sigmaR2= par[5];
  double alpha1 = par[6];
  double alpha2 = par[7];
  double n1     = par[8];
  double n2     = par[9];
  double N1     = par[10];
  double N2     = par[11];

  return DGlobalTools::DoublePeakBifurcatedCBFitFunction55Fe(x[0],mean1, mean2, sigmaL1, sigmaL2, sigmaR1, sigmaR2, alpha1, alpha2, n1, n2, N1, N2);

}

//====================================================================================================================================================


