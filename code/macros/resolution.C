{
// a.besson april 2006.
// .x code/macros/resolution.C
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const Int_t Npitch = 3;
Float_t TCArray_residual_U_smdiode_m20[Npitch]              = {1.88, 2.29, 3.08};
Float_t TCArray_residual_U_ladiode_m20[Npitch]              = {2.02, 2.16, 3.12};
Float_t TCArray_residual_U_smdiode_p20[Npitch]              = {1.86, 2.30, 2.98};
Float_t TCArray_residual_U_ladiode_p20[Npitch]              = {1.94, 2.14, 3.01};

Float_t TCArray_residual_V_smdiode_m20[Npitch]              = {2.21, 2.25, 3.24};
Float_t TCArray_residual_V_ladiode_m20[Npitch]              = {2.31, 2.25, 3.27};
Float_t TCArray_residual_V_smdiode_p20[Npitch]              = {1.75, 2.22, 2.97};
Float_t TCArray_residual_V_ladiode_p20[Npitch]              = {1.78, 2.20, 2.91};

Float_t TCArray_resolution_U_smdiode_m20[Npitch]              = {  0  , 0   , 0   };
Float_t TCArray_resolution_U_ladiode_m20[Npitch]              = {  0  , 0   , 0   };
Float_t TCArray_resolution_U_smdiode_p20[Npitch]              = {  0  , 0   , 0   };
Float_t TCArray_resolution_U_ladiode_p20[Npitch]              = {  0  , 0   , 0   };

Float_t TCArray_resolution_V_smdiode_m20[Npitch]              = {  0  , 0   , 0   };
Float_t TCArray_resolution_V_ladiode_m20[Npitch]              = {  0  , 0   , 0   };
Float_t TCArray_resolution_V_smdiode_p20[Npitch]              = {  0  , 0   , 0   };
Float_t TCArray_resolution_V_ladiode_p20[Npitch]              = {  0  , 0   , 0   };

Float_t TCArray_pitch[Npitch]     = {  20  , 30   , 40   };


// fixed planes:3,4,5,6  mobile planes:1,2,7,8
// H/U planes: 1,3,5,7 V/V planes: 2,4,6,8
const Int_t Nruns = 4;
Float_t TCArray_tracker_residual_V1[Nruns]     = {3.42, 4.38, 3.06, 4.55};
Float_t TCArray_tracker_residual_U2[Nruns]     = {3.18, 3.17, 3.63, 3.44};
Float_t TCArray_tracker_residual_V7[Nruns]     = {3.60, 3.43, 3.56, 3.74};
Float_t TCArray_tracker_residual_U8[Nruns]     = {2.72, 2.32, 2.37, 2.67};


const Int_t NPlanes = 8;
Float_t TCArray_tracker_Zposition[NPlanes] = {95.5,93.5,84.5,86.5,-84.5,-86.5,-95.5,-93.5};


Float_t TCArray_tracker_residual_V1_Average = 0.0;
Float_t TCArray_tracker_residual_U2_Average = 0.0;
Float_t TCArray_tracker_residual_V7_Average = 0.0;
Float_t TCArray_tracker_residual_U8_Average = 0.0;

Float_t TCArray_tracker_residual_V1_sigma = 0.0;
Float_t TCArray_tracker_residual_U2_sigma = 0.0;
Float_t TCArray_tracker_residual_V7_sigma = 0.0;
Float_t TCArray_tracker_residual_U8_sigma = 0.0;

Float_t TCArray_tracker_residual_V1_sigma_mean = 0.0;
Float_t TCArray_tracker_residual_U2_sigma_mean = 0.0;
Float_t TCArray_tracker_residual_V7_sigma_mean = 0.0;
Float_t TCArray_tracker_residual_U8_sigma_mean = 0.0;

Float_t TCArray_tracker_Resolution_V1_Average = 0.0;
Float_t TCArray_tracker_Resolution_U2_Average = 0.0;
Float_t TCArray_tracker_Resolution_V7_Average = 0.0;
Float_t TCArray_tracker_Resolution_U8_Average = 0.0;

Float_t TCArray_tracker_Resolution_V1_sigma_mean = 0.0;
Float_t TCArray_tracker_Resolution_U2_sigma_mean = 0.0;
Float_t TCArray_tracker_Resolution_V7_sigma_mean = 0.0;
Float_t TCArray_tracker_Resolution_U8_sigma_mean = 0.0;

//------------------------------------
// compute tracker resolution
//------------------------------------
//average residual:
for(Int_t i=0 ; i<Nruns ; i++){
  TCArray_tracker_residual_V1_Average += TCArray_tracker_residual_V1[i];
  TCArray_tracker_residual_U2_Average += TCArray_tracker_residual_U2[i];
  TCArray_tracker_residual_V7_Average += TCArray_tracker_residual_V7[i];
  TCArray_tracker_residual_U8_Average += TCArray_tracker_residual_U8[i];
}
TCArray_tracker_residual_V1_Average =    TCArray_tracker_residual_V1_Average /float(Nruns);
TCArray_tracker_residual_U2_Average =    TCArray_tracker_residual_U2_Average /float(Nruns);
TCArray_tracker_residual_V7_Average =    TCArray_tracker_residual_V7_Average /float(Nruns);
TCArray_tracker_residual_U8_Average =    TCArray_tracker_residual_U8_Average /float(Nruns);

for(Int_t i=0 ; i<Nruns ; i++){
  TCArray_tracker_residual_V1_sigma += (TCArray_tracker_residual_V1[i] - TCArray_tracker_residual_V1_Average) 
    * (TCArray_tracker_residual_V1[i] - TCArray_tracker_residual_V1_Average);
  TCArray_tracker_residual_U2_sigma += (TCArray_tracker_residual_U2[i] - TCArray_tracker_residual_U2_Average) 
    * (TCArray_tracker_residual_U2[i] - TCArray_tracker_residual_U2_Average);
  TCArray_tracker_residual_V7_sigma += (TCArray_tracker_residual_V7[i] - TCArray_tracker_residual_V7_Average) 
    * (TCArray_tracker_residual_V7[i] - TCArray_tracker_residual_V7_Average);
  TCArray_tracker_residual_U8_sigma += (TCArray_tracker_residual_U8[i] - TCArray_tracker_residual_U8_Average) 
    * (TCArray_tracker_residual_U8[i] - TCArray_tracker_residual_U8_Average);
}

TCArray_tracker_residual_V1_sigma = sqrt(TCArray_tracker_residual_V1_sigma/ float(Nruns-1) );
TCArray_tracker_residual_U2_sigma = sqrt(TCArray_tracker_residual_U2_sigma/ float(Nruns-1) );
TCArray_tracker_residual_V7_sigma = sqrt(TCArray_tracker_residual_V7_sigma/ float(Nruns-1) );
TCArray_tracker_residual_U8_sigma = sqrt(TCArray_tracker_residual_U8_sigma/ float(Nruns-1) );

TCArray_tracker_residual_V1_sigma_mean = TCArray_tracker_residual_V1_sigma/ sqrt(float(Nruns));
TCArray_tracker_residual_U2_sigma_mean = TCArray_tracker_residual_U2_sigma/ sqrt(float(Nruns));
TCArray_tracker_residual_V7_sigma_mean = TCArray_tracker_residual_V7_sigma/ sqrt(float(Nruns));
TCArray_tracker_residual_U8_sigma_mean = TCArray_tracker_residual_U8_sigma/ sqrt(float(Nruns));

//Float_t TCArray_tracker_Zposition[NPlanes] = {95.5,93.5,84.5,86.5,-84.5,-86.5,-95.5,-93.5}
// in the array:
// fixed planes:2,3,4,5,  mobile planes:0,1,6,7
// H/U planes: 0,2,4,6 V/V planes: 1,3,5,7
//

Float_t resL,resZZ1,resZZ2;

// plan 0 -> 2,4
resZZ1 = TCArray_tracker_Zposition[2] - TCArray_tracker_Zposition[0];
resZZ2 = TCArray_tracker_Zposition[4] - TCArray_tracker_Zposition[0];
resL = abs(TCArray_tracker_Zposition[2] - TCArray_tracker_Zposition[4]);
TCArray_tracker_Resolution_V1_Average = sqrt( TCArray_tracker_residual_V1_Average * TCArray_tracker_residual_V1_Average 
					      / (1+ (resZZ1*resZZ1 + resZZ2*resZZ2)/(resL*resL) ) );
TCArray_tracker_Resolution_V1_sigma_mean = sqrt( TCArray_tracker_residual_V1_sigma_mean * TCArray_tracker_residual_V1_sigma_mean 
						 / (1+ (resZZ1*resZZ1 + resZZ2*resZZ2)/(resL*resL) ) );

// plan 1 -> 3,5
resZZ1 = TCArray_tracker_Zposition[3] - TCArray_tracker_Zposition[1];
resZZ2 = TCArray_tracker_Zposition[5] - TCArray_tracker_Zposition[1];
resL = abs(TCArray_tracker_Zposition[3] - TCArray_tracker_Zposition[5]);
TCArray_tracker_Resolution_U2_Average = sqrt( TCArray_tracker_residual_U2_Average * TCArray_tracker_residual_U2_Average 
					      / (1+ (resZZ1*resZZ1 + resZZ2*resZZ2)/(resL*resL) ) );
TCArray_tracker_Resolution_U2_sigma_mean = sqrt( TCArray_tracker_residual_U2_sigma_mean * TCArray_tracker_residual_U2_sigma_mean 
						 / (1+ (resZZ1*resZZ1 + resZZ2*resZZ2)/(resL*resL) ) );

// plan 6 -> 2,4
resZZ1 = TCArray_tracker_Zposition[2] - TCArray_tracker_Zposition[6];
resZZ2 = TCArray_tracker_Zposition[4] - TCArray_tracker_Zposition[6];
resL = abs(TCArray_tracker_Zposition[2] - TCArray_tracker_Zposition[4]);
TCArray_tracker_Resolution_V7_Average = sqrt( TCArray_tracker_residual_V7_Average * TCArray_tracker_residual_V7_Average 
					      / (1+ (resZZ1*resZZ1 + resZZ2*resZZ2)/(resL*resL) ) );
TCArray_tracker_Resolution_V7_sigma_mean = sqrt( TCArray_tracker_residual_V7_sigma_mean * TCArray_tracker_residual_V7_sigma_mean 
						 / (1+ (resZZ1*resZZ1 + resZZ2*resZZ2)/(resL*resL) ) );

// plan 7 -> 3,5
resZZ1 = TCArray_tracker_Zposition[3] - TCArray_tracker_Zposition[7];
resZZ2 = TCArray_tracker_Zposition[5] - TCArray_tracker_Zposition[7];
resL = abs(TCArray_tracker_Zposition[3] - TCArray_tracker_Zposition[5]);
TCArray_tracker_Resolution_U8_Average = sqrt( TCArray_tracker_residual_U8_Average * TCArray_tracker_residual_U8_Average 
					      / (1+ (resZZ1*resZZ1 + resZZ2*resZZ2)/(resL*resL) ) );
TCArray_tracker_Resolution_U8_sigma_mean = sqrt( TCArray_tracker_residual_U8_sigma_mean * TCArray_tracker_residual_U8_sigma_mean 
						 / (1+ (resZZ1*resZZ1 + resZZ2*resZZ2)/(resL*resL) ) );

cout<<" Average residual V1 U2 V7 U8 "<<TCArray_tracker_residual_V1_Average<<" "<<TCArray_tracker_residual_U2_Average<<" "<<
  TCArray_tracker_residual_V7_Average<<" "<<TCArray_tracker_residual_U8_Average<<endl;

cout<<"tracker Resolution_V1 = "<<TCArray_tracker_Resolution_V1_Average<<" +- "<<TCArray_tracker_Resolution_V1_sigma_mean<<endl;
cout<<"tracker Resolution_U2 = "<<TCArray_tracker_Resolution_U2_Average<<" +- "<<TCArray_tracker_Resolution_U2_sigma_mean<<endl;
cout<<"tracker Resolution_V7 = "<<TCArray_tracker_Resolution_V7_Average<<" +- "<<TCArray_tracker_Resolution_V7_sigma_mean<<endl;
cout<<"tracker Resolution_U8 = "<<TCArray_tracker_Resolution_U8_Average<<" +- "<<TCArray_tracker_Resolution_U8_sigma_mean<<endl;


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}// END
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

