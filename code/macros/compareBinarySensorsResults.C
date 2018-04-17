// Compare the results (efficiency, fake, resolution, pixel multiplicity)
// for different binary output sensors
//
// Usage:
//  Fill your data in the beginning of method compare(). 
//  Each sensor is a new structure containing arrays indexed on thresholds
//   which can be given in mV (ThreV) and/or unit of noise (Thre).
//  Note that if nThresholds is set to 0, this sensor is not drawn.
//  When entering results, please indicate the origin of the analysis,
//   in general a maximum of comments is welcome!
//
//  Usage in ROOT:
//   >.L compareBinarySensorsResults_new.C
//   >compare( ifSave, "directory where to save plots")
//   >plot_comparison( sensor1, sensor2)
//
// JB 2009 September from AB model
// Modified JB 2011/07/01 to allow non-drawing a given sensor
// Modified SS 2011/07/08 for compilation
// Modified JB 2011/07/11 corrected M22AHR results
// Modified SS 2011/07/11 added missing points from first July SPS session
// Modified JB 2011/11/01 method compare with arguments and possibility to draw Thre in Volts
// Modified JB 2011/11/21 method plot_comparison is an independent method

#include "TString.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TGraphErrors.h"
#include "iostream.h"
#include "TCanvas.h"
#include "TText.h"
#include "TGaxis.h"
#include "TMath.h"

#define nMaxThresholds 15 // Maximum number of thresholds

const Int_t nsensors = 100; // Maximum number of sensors managed

// Controlling the display
Bool_t ifPreliminary = kTRUE; // kFALSE;

Float_t effimin=85;
Float_t effimax=101;
Float_t resolmax = 7.0;
Float_t resolmin = 3.0;
Float_t fakemax = 1.0;
Float_t fakemin = 1.e-12;
Float_t multmax = 6.0;
Float_t multmin = 0.5;
Float_t thremax = 15.5;
Float_t thremin = 1.5;

Int_t resolcolor = 2;
Int_t fakecolor = 4;

TGraphErrors **geffi  = new TGraphErrors*[nsensors];
TGraphErrors **greso  = new TGraphErrors*[nsensors];
TGraphErrors **gresoV = new TGraphErrors*[nsensors];
TGraphErrors **gfake  = new TGraphErrors*[nsensors];

TGraphErrors **geffi_thre  = new TGraphErrors*[nsensors];
TGraphErrors **greso_thre  = new TGraphErrors*[nsensors];
TGraphErrors **gresoV_thre = new TGraphErrors*[nsensors];
TGraphErrors **gfake_thre  = new TGraphErrors*[nsensors];

struct sensor {
  Char_t shortName[50];
  Char_t longName[100];
  Char_t origin[200];
  Int_t  nThresholds;
  Int_t  color;
  Int_t  markerStyle;
  Int_t  lineStyle;
  Double_t pixMult[nMaxThresholds]; 
  Double_t pixMultErr[nMaxThresholds]; 
  Double_t Reso[nMaxThresholds]; 
  Double_t ResoErr[nMaxThresholds]; 
  Double_t ResoV[nMaxThresholds]; 
  Double_t ResoVErr[nMaxThresholds]; 
  Double_t TrackRes;
  Double_t Eff[nMaxThresholds]; 
  Double_t EffErr[nMaxThresholds];
  Double_t EffGoodTracks[nMaxThresholds];  
  Double_t EffGoodTracksBeyond[nMaxThresholds];  
  Double_t EffErrSyst1[nMaxThresholds]; 
  Double_t EffErrSyst2[nMaxThresholds]; 
  Double_t Fake[nMaxThresholds]; 
  Double_t FakeErr[nMaxThresholds]; 
  Double_t Thre[nMaxThresholds]; 
  Double_t ThreErr[nMaxThresholds]; 
  Double_t ThreV[nMaxThresholds]; 
  Double_t ThreVErr[nMaxThresholds]; 
};

sensor sensorList[nsensors];

void fill( Double_t *array, Double_t v0=0, Double_t v1=0, Double_t v2=0, Double_t v3=0, Double_t v4=0, Double_t v5=0, Double_t v6=0, Double_t v7=0, Double_t v8=0, Double_t v9=0) {
  array[0] = v0;
  array[1] = v1;
  array[2] = v2;
  array[3] = v3;
  array[4] = v4;
  array[5] = v5;
  array[6] = v6;
  array[7] = v7;
  array[8] = v8;
  array[9] = v9;
}

//------------------------------------------------------------------------------
void compare(Bool_t ifSave = kFALSE, TString outputDir = "Plots/"){


  Float_t markerSize = 1.;
  Float_t lineWidth = 2.0;
  
  Int_t iSens;
 
  // MIMOSA 22 AHR S7
  // results from Beam Test August-September 2010
  // fake rates from lab measurements
  iSens = 0;
  sprintf( sensorList[iSens].shortName, "MI22AHR-S7");
  sprintf( sensorList[iSens].longName, "MIMOSA 22 AHR - epi 15um - S7");
  sprintf( sensorList[iSens].origin, "SPS  2010, pions 120 GeV");
  sensorList[iSens].nThresholds = 0; //5;
  fill( sensorList[iSens].Thre, 4.7, 7.1, 8.2, 9.4, 12.9); // units of noise
  fill( sensorList[iSens].ThreV, 4.0,  6.0,  7.0,  8.0, 11.0); // mV
  fill( sensorList[iSens].Eff, 99.91, 99.6 , 99.79, 99.39, 96.7);
  fill( sensorList[iSens].EffErr, 0.05, 0.1 , 0.07, 0.08, 0.3);
  fill( sensorList[iSens].Fake, 10.4786e-6, 0.0197072e-6, 0.0014e-6,  0.0014e-6,  0.0014e-6); 
  fill( sensorList[iSens].FakeErr, 0.149e-6, 0.00645e-6, 0.001e-5, 0.001e-5, 0.001e-5);
  fill( sensorList[iSens].Reso, 4.00, 3.79, 3.80, 3.75, 4.20); 
  fill( sensorList[iSens].ResoErr, 0.05, 0.06, 0.05, 0.03, 0.05);
  sensorList[iSens].TrackRes = 1.5;
  fill( sensorList[iSens].pixMult, 4.3, 3.3, 2.9, 2.6, 2.0);
  fill( sensorList[iSens].pixMultErr, 0.01,  0.01,  0.01,  0.01,  0.01); 
  sensorList[iSens].color = kOrange;
  sensorList[iSens].markerStyle = 24;
  sensorList[iSens].lineStyle = 1;
  
  // MIMOSA 22 AHR - S7 - 30C 
  // results from Beam Test August-September 2010
  // fake rates from lab measurements
  iSens = 1;
  sprintf( sensorList[iSens].shortName, "MIMOSA22AHR-S7-30C");
  sprintf( sensorList[iSens].longName, "MIMOSA 22 AHR - epi 15 um - S7 - 30 C");
  sprintf( sensorList[iSens].origin, "SPS  2010, pions 120 GeV");
  sensorList[iSens].nThresholds = 0; //3;
  fill( sensorList[iSens].Thre, 4.1, 6.4, 8.7); 
  fill( sensorList[iSens].ThreV, 4.0, 6.0, 8.0); 
  fill( sensorList[iSens].Eff, 99.89, 99.80, 99.72);
  fill( sensorList[iSens].EffErr, 0.05, 0.10, 0.16);
  fill( sensorList[iSens].Fake, 23.7387e-6, 2.67455e-008, 0.00140766e-6); 
  fill( sensorList[iSens].FakeErr, 0.182798e-6, 6.13584e-009, 0.00140766e-6);
  fill( sensorList[iSens].Reso, 4.32, 3.97, 3.8); 
  fill( sensorList[iSens].ResoErr, 0.07, 0.07, 0.10);
  sensorList[iSens].TrackRes = 1.5;
  fill( sensorList[iSens].pixMult, 4.7, 3.4, 2.7);
  fill( sensorList[iSens].pixMultErr, 0.01, 0.01, 0.01); 
  sensorList[iSens].color = kOrange+1;
  sensorList[iSens].markerStyle = 24;
  sensorList[iSens].lineStyle = 1;
  
  // MIMOSA 22 AHR - S7 - 150 kRad - 30 C
  // results from Beam Test August-September 2010
  // fake rates from lab measurements
  iSens = 2;
  sprintf( sensorList[iSens].shortName, "MIMOSA22AHR-S7-150kRad");
  sprintf( sensorList[iSens].longName, "MIMOSA 22 AHR - S7 - epi 15 um - 150kRad");
  sprintf( sensorList[iSens].origin, "SPS  2010, pions 120 GeV");
  sensorList[iSens].nThresholds = 0; //3;
  fill( sensorList[iSens].Thre, 4.5,  5.4,  7.2); 
  fill( sensorList[iSens].ThreV, 5.0,  6.0,  8.0); 
  fill( sensorList[iSens].Eff, 99.87, 99.80, 99.66);
  fill( sensorList[iSens].EffErr, 0.05, 0.07, 0.11);
  fill( sensorList[iSens].Fake, 0.0221734e-3, 0.00193694e-3, 2.53378e-08); 
  fill( sensorList[iSens].FakeErr, 0.00021667e-3, 0.00007385e-3, 0.00000844e-3);
  fill( sensorList[iSens].Reso, 4.13, 3.92, 3.75); 
  fill( sensorList[iSens].ResoErr, 0.04, 0.05, 0.05);
  sensorList[iSens].TrackRes = 1.5;
  fill( sensorList[iSens].pixMult, 4.1, 3.6, 2.8);
  fill( sensorList[iSens].pixMultErr, 0.03,  0.03,  0.03); 
  sensorList[iSens].color = kOrange+2;
  sensorList[iSens].markerStyle = 24;
  sensorList[iSens].lineStyle = 1;
  
  // MIMOSA 22 AHR - S7 - 150kRad 3.e12neq - 30 C
  // results from Beam Test August-September 2010
  // fake rates from lab measurements
  iSens = 3;
  sprintf( sensorList[iSens].shortName, "MIMOSA22AHR15-S7-150kRad-3e12neq");
  sprintf( sensorList[iSens].longName, "MIMOSA 22 AHR - epi 15 um - S7 - 150 kRad + 3e12 neq");
  sprintf( sensorList[iSens].origin, "SPS  2010, pions 120 GeV");
  sensorList[iSens].nThresholds = 0; //3;
  fill( sensorList[iSens].Thre, 4.5, 5.4,  7.2); 
  fill( sensorList[iSens].ThreV, 5.0, 6.0,  8.0); 
  fill( sensorList[iSens].Eff, 99.76, 99.84, 99.15);
  fill( sensorList[iSens].EffErr, 0.09,  0.05, 0.13);
  fill( sensorList[iSens].Fake, 0.00935811e-3, 0.000389921e-3, 0.00000140766e-3); 
  fill( sensorList[iSens].FakeErr, 0.0001477e-3, 0.00002343e-3, 0.00000140766e-3);
  fill( sensorList[iSens].Reso, 3.91, 3.86, 3.94); 
  fill( sensorList[iSens].ResoErr, 0.05, 0.04, 0.05);
  sensorList[iSens].TrackRes = 1.5;
  fill( sensorList[iSens].pixMult, 3.4, 3.0, 2.5);
  fill( sensorList[iSens].pixMultErr, 0.02, 0.03,  0.03); 
  sensorList[iSens].color = kOrange+3;
  sensorList[iSens].markerStyle = 24;
  sensorList[iSens].lineStyle = 1;
  
  // MIMOSA 26 - std epi (chip 24, test beam 2009)
  // analysis from Marie
  iSens = 4;
  sprintf( sensorList[iSens].shortName, "MIMOSA26-epiStd");  
  sprintf( sensorList[iSens].longName, "MIMOSA 26 - epi standard");
  sprintf( sensorList[iSens].origin, "SPS September 2009, pions 120 GeV");
  sensorList[iSens].nThresholds = 0;//5
  fill( sensorList[iSens].Thre,		4,	5,	6,    8,    10); 
  fill( sensorList[iSens].ThreV,      3.7,	4.6,	5.5,  7.3,  9.2); 
  fill( sensorList[iSens].Eff,		99.89,	99.39,	98.09, 92.3,  83.7);
  fill( sensorList[iSens].EffErr,	0.02,	0.05,	0.08, 0.20, 0.22);
  fill( sensorList[iSens].Fake,		1.5e-4,	4.4e-5,	1.3e-5, 2.0e-6, 0.3e-6); 
  fill( sensorList[iSens].FakeErr,	2e-9,	2e-9,	0.2e-9, 0.2e-9, 0.2e-9);
  fill( sensorList[iSens].Reso,		3.98,	3.94,	4.02,	4.30,	4.43); 
  fill( sensorList[iSens].ResoErr,	0.02,	0.02,	0.02,	0.02,	0.02);
  sensorList[iSens].TrackRes = 1.8;
  fill( sensorList[iSens].pixMult,	3.74,	3.04,	2.57, 2.04, 1.73);
  fill( sensorList[iSens].pixMultErr,	0.01,	0.01,	0.01, 0.01, 0.01); 
  sensorList[iSens].color = kBlue+0;
  sensorList[iSens].markerStyle = 21;
    sensorList[iSens].lineStyle = 1;
  
  // MIMOSA 26 - HR10 epi (chip 27, test beam 2010)
  // analysis from Marie
  iSens = 5;
  sprintf( sensorList[iSens].shortName, "MIMOSA26-epiHR10");
  sprintf( sensorList[iSens].longName, "MIMOSA 26 - HighRes epi 10 um");
  sprintf( sensorList[iSens].origin, "SPS June 2010, pions 120 GeV");
  sensorList[iSens].nThresholds = 0; //5;
  fill( sensorList[iSens].Thre,		4,	5,	6,	8,	  12); 
  fill( sensorList[iSens].ThreV,	3.6,	4.4,	5.3,	7.1,	  10.7); 
  fill( sensorList[iSens].Eff,		99.99,	99.99,	99.93,  99.41,    94.84);
  fill( sensorList[iSens].EffErr,	0.01,	0.01,	0.01,	0.04,	  0,11);
  fill( sensorList[iSens].Fake,		3.35e-4,8.3e-5,	2.5e-5,	3.5e-6,	  1.1e-7); 
  fill( sensorList[iSens].FakeErr,	1e-7,	1e-8,	1e-8,	1e-8,	  1e-9);
  fill( sensorList[iSens].Reso,		3.91,	3.83,	3.7,	3.87,	  4.36); 
  fill( sensorList[iSens].ResoErr,	0.01,	0.01,	0.01,	0.01,	  0.01);
  sensorList[iSens].TrackRes = 1.8;
  fill( sensorList[iSens].pixMult,	3.44,	3.00,	2.64,	2.19,	  1.67);
  fill( sensorList[iSens].pixMultErr,	0.01,	0.01,	0.01,	0.01,	  0.01); 
  sensorList[iSens].color = kBlue+1;
  sensorList[iSens].markerStyle = 21;
  sensorList[iSens].lineStyle = 1;
  
  // MIMOSA 26 - HR15 epi (chip 26, test beam 2010)
  // analysis from Marie
  iSens = 6;
  sprintf( sensorList[iSens].shortName, "MIMOSA26-epiHR15");
  sprintf( sensorList[iSens].longName, "MIMOSA 26 - HighRes epi 15 um");
  sprintf( sensorList[iSens].origin, "SPS June 2010, pions 120 GeV");
  sensorList[iSens].nThresholds = 0; //5;
  fill( sensorList[iSens].Thre,		4.5,	5,	6,	8,	  10); 
  fill( sensorList[iSens].ThreV,	4.7,	5.2,	6.3,	8.4,	  10.5); 
  fill( sensorList[iSens].Eff,		99.99,	99.98,	99.94,  99.39,    97.77);
  fill( sensorList[iSens].EffErr,	0.01,	0.02,	0.01,	0.04,	  0,07);
  fill( sensorList[iSens].Fake,		1.6e-5,	1.0e-5,	2.8e-6,	2.5e-7,	  2.0e-8); 
  fill( sensorList[iSens].FakeErr,	1e-7,	1e-7,	1e-8,	1e-9,	  1e-9);
  fill( sensorList[iSens].Reso,		3.87,	3.69,	3.67,	3.95,	  4.13); 
  fill( sensorList[iSens].ResoErr,	0.01,	0.01,	0.01,	0.01,	  0.01);
  sensorList[iSens].TrackRes = 1.8;
  fill( sensorList[iSens].pixMult,	3.34,	3.15,	2.80,	2.31,	  1.95);
  fill( sensorList[iSens].pixMultErr,	0.01,	0.01,	0.01,	0.01,	  0.01); 
  sensorList[iSens].color = kBlue+2;
  sensorList[iSens].markerStyle = 21;
  sensorList[iSens].lineStyle = 1;
  
  // MIMOSA 26 - HR20 epi (chip 28, test beam 2010)
  // analysis from Marie
  iSens = 7;
  sprintf( sensorList[iSens].shortName, "MIMOSA26-epiHR10");
  sprintf( sensorList[iSens].longName, "MIMOSA 26 - HighRes epi 20 um");
  sprintf( sensorList[iSens].origin, "SPS June 2010, pions 120 GeV");
  sensorList[iSens].nThresholds = 0; //5;
  fill( sensorList[iSens].Thre,		4,	5,	6,	8,	  10); 
  fill( sensorList[iSens].ThreV,	4.2,	5.2,	6.3,	8.3,	  10.4); 
  fill( sensorList[iSens].Eff,	      100.0,  100.0,	99.97,  99.47,    97.83);
  fill( sensorList[iSens].EffErr,	0.01,	0.01,	0.01,	0.04,	  0,08);
  fill( sensorList[iSens].Fake,		1.9e-4,	3.9e-5,	9.9e-6,	9.3e-7,	  8.5e-8); 
  fill( sensorList[iSens].FakeErr,	1e-6,	1e-7,	1e-8,	1e-9,	  1e-9);
  fill( sensorList[iSens].Reso,		3.94,	3.98,	3.85,	3.83,	  3.98); 
  fill( sensorList[iSens].ResoErr,	0.02,	0.02,	0.02,	0.02,	  0.02);
  sensorList[iSens].TrackRes = 1.8;
  fill( sensorList[iSens].pixMult,	5.39,	4.46,	3.77,	2.82,	  2.30);
  fill( sensorList[iSens].pixMultErr,	0.01,	0.01,	0.01,	0.01,	  0.01); 
  sensorList[iSens].color = kBlue+3;
  sensorList[iSens].markerStyle = 21;
  sensorList[iSens].lineStyle = 1;
  
  // MIMOSA 28 - HR 20 (chip ?)
  // analysis from Sergey
  iSens = 8;
  sprintf( sensorList[iSens].shortName, "MIMOSA28-epiHR20");
  sprintf( sensorList[iSens].longName, "MIMOSA 28 - epi 20 um");
  sprintf( sensorList[iSens].origin, "SPS July 2011, pions 120 GeV");
  sensorList[iSens].nThresholds = 0; //4;
  fill( sensorList[iSens].Thre,		5,	6,	8,	10); 
  fill( sensorList[iSens].ThreV, 0); 
  fill( sensorList[iSens].Eff,		99.97,	99.96,	99.82,	99.08);
  fill( sensorList[iSens].EffErr,	0.01,	0.01,	0.02,	0.02);
  fill( sensorList[iSens].Fake,		2.34e-5,2.2e-6,	5.97e-8,3.62e-9); 
  fill( sensorList[iSens].FakeErr,	1e-7,	1e-8,	.11e-8,	.26e-9);
  fill( sensorList[iSens].Reso,		4.25,	4.15,	4.05,	4.11); 
  fill( sensorList[iSens].ResoErr,	0.01,	0.01,	0.01,	0.01);
  sensorList[iSens].TrackRes = 1.8;
  fill( sensorList[iSens].pixMult,	5.44,	4.49,	3.28,	2.75);
  fill( sensorList[iSens].pixMultErr,	0.01,	0.01,	0.01,	0.01); 
  sensorList[iSens].color = kViolet+0;
  sensorList[iSens].markerStyle = 22;
  sensorList[iSens].lineStyle = 1;
  
  // MIMOSA 28 150 kRad
  // analysis from Sergey
  iSens = 9;
  sprintf( sensorList[iSens].shortName, "MIMOSA28-epiHR20-150kRad");
  sprintf( sensorList[iSens].longName, "MIMOSA 28 - epi 20 um - 150kRad");
  sprintf( sensorList[iSens].origin, "SPS July 2011, pions 120 GeV");
  sensorList[iSens].nThresholds = 0; //4;
  fill( sensorList[iSens].Thre,		5,	6,	8,	10); 
  fill( sensorList[iSens].ThreV, 0); 
  fill( sensorList[iSens].Eff,		99.96,	99.94,	99.62,	98.20);
  fill( sensorList[iSens].EffErr,	0.01,	0.01,	0.04,	0.03);
  fill( sensorList[iSens].Fake,		1.12e-5,7.48e-7,9.27e-9,1.43e-9); 
  fill( sensorList[iSens].FakeErr,	1e-7,	4.e-9,	.44e-9,	.16e-9);
  fill( sensorList[iSens].Reso,		4.31,	4.21,	4.07,	4.24); 
  fill( sensorList[iSens].ResoErr,	0.01,	0.01,	0.02,	0.01);
  sensorList[iSens].TrackRes = 1.8;
  fill( sensorList[iSens].pixMult,	4.71,	4.00,	2.99,	2.45);
  fill( sensorList[iSens].pixMultErr,	0.01,	0.01,	0.01,	0.01); 
  sensorList[iSens].color = kViolet+0;
  sensorList[iSens].markerStyle = 22;
  sensorList[iSens].lineStyle = 2;
  
  // MIMOSA 28 HR15
  // analysis from Sergey
  iSens = 10;
  sprintf( sensorList[iSens].shortName, "MIMOSA28-epiHR15");
  sprintf( sensorList[iSens].longName, "MIMOSA 28 - epi 15 um");
  sprintf( sensorList[iSens].origin, "SPS October 2011, pions 120 GeV");
  sensorList[iSens].nThresholds = 0; //8;
  fill( sensorList[iSens].Thre,		4.5,	5.,	5.8,	7,	8,	9,	10.2,	11.5); 
  fill( sensorList[iSens].ThreV, 0); 
  fill( sensorList[iSens].Eff,		99.98,	99.98,	99.97,	99.90,	99.71,	99.22,	98.32,	96.99);
  fill( sensorList[iSens].EffErr,	0.01,	0.01,	0.01,	0.01,	0.01,	0.02,	0.03,	0.03);
  fill( sensorList[iSens].Fake,		8.58e-6,3.16e-6,9.24e-7,1.46e-7,3.69e-8,6.72e-9,8.8e-10,2.0e-10); 
  fill( sensorList[iSens].FakeErr,	2.21e-8,1.34e-8,4.35e-9,1.73e-9,8.7e-10,3.7e-10,1.3e-10,6.5e-11);
  fill( sensorList[iSens].Reso,		4.12,	4.12,	4.01,	3.93,	3.94,	4.06,	4.21,	4.35); 
  fill( sensorList[iSens].ResoErr,	0.01,	0.01,	0.01,	0.01,	0.01,	0.01,	0.01,	0.01);
  sensorList[iSens].TrackRes = 1.8;
  fill( sensorList[iSens].pixMult,	3.51,	3.52,	3.19,	2.83,	2.57,	2.30,	2.12,	1.96);
  fill( sensorList[iSens].pixMultErr,	0.01,	0.01,	0.01,	0.01,	0.01,	0.01,	0.01,	0.01); 
  sensorList[iSens].color = kViolet+1;
  sensorList[iSens].markerStyle = 22;
  sensorList[iSens].lineStyle = 1;
  
  
  // MIMOSA 28 HR15, SALAT, Plane 5
  double TelResol_SALAT_DESY_TB = 2.5;
  // analysis from Alejandro Perez
  //Data with beam in the center (Center-middle or CM) of the 4 SALAT sensor.
  //Touching planes 5, 6, 7 and 8
  iSens = 11;
  sprintf( sensorList[iSens].shortName, "SALAT_Pl5_CM_M28-epiHR15");
  sprintf( sensorList[iSens].longName,  "SALAT Pl5 (CM): M28 - epi 15 um");
  sprintf( sensorList[iSens].origin,    "DESY February 2014, electrons 5.4 GeV");
  sensorList[iSens].nThresholds = 0; //4;
  fill( sensorList[iSens].Thre,          5.0,      6.0,      8.0,       10.0);
  fill( sensorList[iSens].ThreV, 0);
  fill( sensorList[iSens].Eff,          99.92,    99.93,    99.85,    99.27);
  fill( sensorList[iSens].EffErr,        0.01,     0.01,     0.02,     0.03);
  fill( sensorList[iSens].Fake,          2.15e-5,  9.75e-6,  3.84e-7,  6.77e-8);
  fill( sensorList[iSens].FakeErr,       6.94e-9,  4.68e-9,  9.29e-10, 3.90e-10);
  fill( sensorList[iSens].Reso,          4.71,     4.68,     4.49,     4.60);
  fill( sensorList[iSens].ResoErr,       0.01,     0.01,     0.01,     0.01);
  sensorList[iSens].TrackRes = TelResol_SALAT_DESY_TB;
  fill( sensorList[iSens].pixMult,       4.80,     3.79,     3.12,     2.54);
  fill( sensorList[iSens].pixMultErr,    0.006,    0.004,    0.007,    0.006);
  sensorList[iSens].color = kGreen+2;
  sensorList[iSens].markerStyle = 22;
  sensorList[iSens].lineStyle = 1;


  // MIMOSA 28 HR15, SALAT, Plane 6
  // analysis from Alejandro Perez
  //Data with beam in the center (Center-middle or CM) of the 4 SALAT sensor.
  //Touching planes 5, 6, 7 and 8
  iSens = 12;
  sprintf( sensorList[iSens].shortName, "SALAT_Pl6_CM_M28-epiHR15");
  sprintf( sensorList[iSens].longName,  "SALAT Pl6 (CM): M28 - epi 15 um");
  sprintf( sensorList[iSens].origin,    "DESY February 2014, electrons 5.4 GeV");
  sensorList[iSens].nThresholds = 0; //4;
  fill( sensorList[iSens].Thre,          5.0,      6.0,      8.0,       10.0);
  fill( sensorList[iSens].ThreV, 0);
  fill( sensorList[iSens].Eff,          99.93,    99.93,    99.77,    99.04);
  fill( sensorList[iSens].EffErr,        0.01,     0.01,     0.02,     0.05);
  fill( sensorList[iSens].Fake,          2.28e-6,  1.76e-6,  1.30e-7,  1.56e-8);
  fill( sensorList[iSens].FakeErr,       2.26e-9,  1.99e-9,  5.40e-10, 1.87e-10);
  fill( sensorList[iSens].Reso,          4.58,     4.55,     4.46,     4.68);
  fill( sensorList[iSens].ResoErr,       0.02,     0.02,     0.02,     0.02);
  sensorList[iSens].TrackRes = TelResol_SALAT_DESY_TB;
  fill( sensorList[iSens].pixMult,       4.66,     3.90,     3.00,     2.45);
  fill( sensorList[iSens].pixMultErr,    0.010,    0.009,    0.008,    0.007);
  sensorList[iSens].color = kRed;
  sensorList[iSens].markerStyle = 22;

  // MIMOSA 28 HR15, SALAT, Plane 7
  // analysis from Alejandro Perez
  //Data with beam in the center (Center-middle or CM) of the 4 SALAT sensor.
  //Touching planes 5, 6, 7 and 8
  iSens = 13;
  sprintf( sensorList[iSens].shortName, "SALAT_Pl7_CM_M28-epiHR15");
  sprintf( sensorList[iSens].longName,  "SALAT P7 (CM): M28 - epi 15 um");
  sprintf( sensorList[iSens].origin,    "DESY February 2014, electrons 5.4 GeV");
  sensorList[iSens].nThresholds = 0; //4;
  fill( sensorList[iSens].Thre,          5.0,      6.0,      8.0,       10.0);
  fill( sensorList[iSens].ThreV, 0);
  fill( sensorList[iSens].Eff,          99.89,    99.89,    99.79,    99.14);
  fill( sensorList[iSens].EffErr,        0.02,     0.02,     0.02,     0.05);
  fill( sensorList[iSens].Fake,          8.61e-6,  3.21e-6,  3.81e-7,  7.62e-8);
  fill( sensorList[iSens].FakeErr,       4.40e-9,  2.69e-9,  9.24e-10, 4.14e-10);
  fill( sensorList[iSens].Reso,          4.63,     4.54,     4.44,     4.59);
  fill( sensorList[iSens].ResoErr,       0.02,     0.02,     0.02,     0.02);
  sensorList[iSens].TrackRes = TelResol_SALAT_DESY_TB;
  fill( sensorList[iSens].pixMult,       4.76,     3.89,     3.04,     2.49);
  fill( sensorList[iSens].pixMultErr,    0.010,    0.009,    0.008,    0.007);
  sensorList[iSens].color = kOrange;
  sensorList[iSens].markerStyle = 22;
  sensorList[iSens].lineStyle = 1;

  // MIMOSA 28 HR15, SALAT, Plane 8
  // analysis from Alejandro Perez
  //Data with beam in the center (Center-middle or CM) of the 4 SALAT sensor.
  //Touching planes 5, 6, 7 and 8
  iSens = 14;
  sprintf( sensorList[iSens].shortName, "SALAT_Pl8_CM_M28-epiHR15");
  sprintf( sensorList[iSens].longName,  "SALAT Pl8 (CM): M28 - epi 15 um");
  sprintf( sensorList[iSens].origin,    "DESY February 2014, electrons 5.4 GeV");
  sensorList[iSens].nThresholds = 0; //4;
  fill( sensorList[iSens].Thre,          5.0,      6.0,      8.0,       10.0);
  fill( sensorList[iSens].ThreV, 0);
  fill( sensorList[iSens].Eff,          99.89,    99.90,    99.83,    99.17);
  fill( sensorList[iSens].EffErr,        0.02,     0.02,     0.03,     0.06);
  fill( sensorList[iSens].Fake,          1.46e-5,  2.90e-6,  4.06e-7,  9.76e-8);
  fill( sensorList[iSens].FakeErr,       5.72e-9,  2.55e-9,  9.55e-10, 4.68e-10);
  fill( sensorList[iSens].Reso,          4.62,     4.63,     4.47,     4.59);
  fill( sensorList[iSens].ResoErr,       0.02,     0.02,     0.02,     0.02);
  sensorList[iSens].TrackRes = TelResol_SALAT_DESY_TB;
  fill( sensorList[iSens].pixMult,       4.66,     4.00,     3.05,     2.51);
  fill( sensorList[iSens].pixMultErr,    0.012,    0.011,    0.010,    0.010);
  sensorList[iSens].color = kBlue;
  sensorList[iSens].markerStyle = 22;
  sensorList[iSens].lineStyle = 1;

  // MIMOSA 28 HR15, SALAT, Plane 7
  // analysis from Alejandro Perez
  //Data with beam in the center top (CT) of the 4 SALAT sensor.
  //Touching planes 7 and 8
  iSens = 15;
  sprintf( sensorList[iSens].shortName, "SALAT_Pl7_CT_M28-epiHR15");
  sprintf( sensorList[iSens].longName,  "SALAT P7 (CT): M28 - epi 15 um");
  sprintf( sensorList[iSens].origin,    "DESY February 2014, electrons 5.4 GeV");
  sensorList[iSens].nThresholds = 0; //2;
  fill( sensorList[iSens].Thre,          6.0,      8.0);
  fill( sensorList[iSens].ThreV, 0);
  fill( sensorList[iSens].Eff,          99.36,    99.11);
  fill( sensorList[iSens].EffErr,        0.02,     0.02);
  fill( sensorList[iSens].Fake,          1.30e-5,  9.14e-6);
  fill( sensorList[iSens].FakeErr,       5.39e-9,  4.53e-9);
  fill( sensorList[iSens].Reso,          4.55,     4.47);
  fill( sensorList[iSens].ResoErr,       0.01,     0.01);
  sensorList[iSens].TrackRes = TelResol_SALAT_DESY_TB;
  fill( sensorList[iSens].pixMult,       3.58,     2.86);
  fill( sensorList[iSens].pixMultErr,    0.005,    0.004);
  sensorList[iSens].color = 1;
  sensorList[iSens].markerStyle = 20;
  sensorList[iSens].lineStyle = 1;


  // MIMOSA 28 HR15, SALAT, Plane 8
  // analysis from Alejandro Perez
  //Data with beam in the center (Center-middle or CM) of the 4 SALAT sensor.
  //Data with beam in the center top (CT) of the 4 SALAT sensor.
  //Touching planes 7 and 8
  iSens = 16;
  sprintf( sensorList[iSens].shortName, "SALAT_Pl8_CT_M28-epiHR15");
  sprintf( sensorList[iSens].longName,  "SALAT Pl8 (CT): M28 - epi 15 um");
  sprintf( sensorList[iSens].origin,    "DESY February 2014, electrons 5.4 GeV");
  sensorList[iSens].nThresholds = 0; //2;
  fill( sensorList[iSens].Thre,          6.0,      8.0);
  fill( sensorList[iSens].ThreV, 0);
  fill( sensorList[iSens].Eff,          99.41,    99.13);
  fill( sensorList[iSens].EffErr,        0.03,     0.03);
  fill( sensorList[iSens].Fake,          1.15e-5,  7.46e-6);
  fill( sensorList[iSens].FakeErr,       5.08e-9,  4.09e-9);
  fill( sensorList[iSens].Reso,          4.56,     4.48);
  fill( sensorList[iSens].ResoErr,       0.01,     0.01);
  sensorList[iSens].TrackRes = TelResol_SALAT_DESY_TB;
  fill( sensorList[iSens].pixMult,       3.69,     2.86);
  fill( sensorList[iSens].pixMultErr,    0.006,    0.005);
  sensorList[iSens].color = kMagenta;
  sensorList[iSens].markerStyle = 20;
  sensorList[iSens].lineStyle = 1;


  //FSBB M0a, Diode variation:
  //Diode size = 9mu, Transistor = 1.5 mu/0.28 mu
  //analysis from Alejandro Perez
  double Small_Diode_TN   = 0.6869; //mV
  double Small_Diode_FPN  = 0.7674; //mV
  double Small_Diode_TotN = sqrt(pow(Small_Diode_TN,2) + pow(Small_Diode_FPN,2)); //mV
  double SD_Regsurf = pow(100.0,2); //mu^2
  double SD_Pixsurf = 22.0*33.0; //mu^2
  iSens = 17;
  sprintf( sensorList[iSens].shortName, "FSBB_M0a_Small_Diode");
  sprintf( sensorList[iSens].longName,  "FSBB_M0a, Diode = 9 #mum^{2}, Transistor = 1.5/0.28");
  sprintf( sensorList[iSens].origin,    "SPF October 2014, #pi^{+} @ 120 GeV");
  sensorList[iSens].nThresholds = 6; //6;
  fill( sensorList[iSens].ThreV,                  4.0,        5.0,       6.0,       8.0,      10.0,      12.0);
  fill( sensorList[iSens].Eff,                   99.96,      99.93,     99.81,     99.04,     96.31,     88.61);
  fill( sensorList[iSens].EffErr,                 0.01,       0.01,      0.02,      0.07,      0.09,      0.15);
  fill( sensorList[iSens].EffGoodTracks,         40283,      39906,     32676,     18491,     42034,     43005);
  fill( sensorList[iSens].EffGoodTracksBeyond,      10,         16,        12,         8,        21,        24);
  fill( sensorList[iSens].Fake,                   5.88e-05,   1.02e-05,  2.77e-06,  8.33e-08,  1.23e-12,  1.23e-12);
  fill( sensorList[iSens].FakeErr,                1.0e-15,    1.0e-15,   1.0e-15,   1.0e-15,   1.0e-15,   1.0e-15);
  for(int i=0;i<sensorList[iSens].nThresholds;i++) {
    sensorList[iSens].Thre[i] = sensorList[iSens].ThreV[i]/Small_Diode_TotN;

    sensorList[iSens].EffErrSyst1[i]  = 0.5*(double)sensorList[iSens].EffGoodTracksBeyond[i]/sensorList[iSens].EffGoodTracks[i];
    sensorList[iSens].EffErrSyst1[i] *= 100.0;

    sensorList[iSens].EffErrSyst2[i]  = (1.0/sqrt(12.0))*(2.0*(SD_Regsurf/SD_Pixsurf)*sensorList[iSens].Fake[i])/sensorList[iSens].EffGoodTracks[i];
    sensorList[iSens].EffErrSyst2[i] *= 100.0;
  }
  fill( sensorList[iSens].Reso,                   5.03,       4.92,      4.71,      4.96,      5.47,      5.74);
  fill( sensorList[iSens].ResoErr,                0.02,       0.02,      0.02,      0.03,      0.02,      0.02);
  fill( sensorList[iSens].ResoV,                  5.10,       5.03,      4.91,      5.40,      6.02,      6.37);
  fill( sensorList[iSens].ResoVErr,               0.02,       0.02,      0.02,      0.03,      0.02,      0.02);
  sensorList[iSens].TrackRes = 0.0;
  fill( sensorList[iSens].pixMult,                3.65,       3.34,      2.85,      2.32,      1.93,      1.66);
  fill( sensorList[iSens].pixMultErr,             0.01,       0.01,      0.01,      0.01,      0.01,      0.01);
  sensorList[iSens].color       = kBlue;
  sensorList[iSens].markerStyle = 20;
  sensorList[iSens].lineStyle   =  1;

  //FSBB M0a, Diode variation:
  //Diode size = 10.9mu, Transistor = 1.5 mu/0.28 mu
  //analysis from Alejandro Perez
  double Big_Diode_TN   = 0.9348; //mV
  double Big_Diode_FPN  = 0.4598; //mV
  double Big_Diode_TotN = sqrt(pow(Big_Diode_TN,2) + pow(Big_Diode_FPN,2)); //mV
  iSens = 18;
  sprintf( sensorList[iSens].shortName, "FSBB_M0a_Big_Diode");
  sprintf( sensorList[iSens].longName,  "FSBB_M0a, Diode = 10.9 #mum^{2}, Transistor = 1.5/0.28");
  sprintf( sensorList[iSens].origin,    "SPF October 2014, #pi^{+} @ 120 GeV");
  sensorList[iSens].nThresholds = 6; //6;
  fill( sensorList[iSens].ThreV,                  4.0,        5.0,       6.0,       8.0,      10.0,      12.0);
  for(int i=0;i<sensorList[iSens].nThresholds;i++) {
    sensorList[iSens].Thre[i] = sensorList[iSens].ThreV[i]/Big_Diode_TotN;
  }
  fill( sensorList[iSens].Eff,                   99.95,      99.92,     99.82,     98.70,     95.78,     87.24);
  fill( sensorList[iSens].EffErr,                 0.01,       0.01,      0.02,      0.07,      0.09,      0.16);
  fill( sensorList[iSens].EffGoodTracks,         41664,      41614,     50082,     27539,     46705,     44752);
  fill( sensorList[iSens].EffGoodTracksBeyond,      17,         18,        31,        13,        25,        23);
  fill( sensorList[iSens].Fake,                   1.25e-04,   2.71e-05,  7.22e-06,  5.69e-07,  4.45e-08,  1.20e-09);
  fill( sensorList[iSens].FakeErr,                1.0e-15,    1.0e-15,   1.0e-15,   1.0e-15,   1.0e-15,   1.0e-15);
  for(int i=0;i<sensorList[iSens].nThresholds;i++) {
    sensorList[iSens].Thre[i] = sensorList[iSens].ThreV[i]/Small_Diode_TotN;

    sensorList[iSens].EffErrSyst1[i]  = 0.5*(double)sensorList[iSens].EffGoodTracksBeyond[i]/sensorList[iSens].EffGoodTracks[i];
    sensorList[iSens].EffErrSyst1[i] *= 100.0;

    sensorList[iSens].EffErrSyst2[i]  = (1.0/sqrt(12.0))*(2.0*(SD_Regsurf/SD_Pixsurf)*sensorList[iSens].Fake[i])/sensorList[iSens].EffGoodTracks[i];
    sensorList[iSens].EffErrSyst2[i] *= 100.0;
  }
  fill( sensorList[iSens].Reso,                   4.90,       4.80,      4.70,      5.02,      5.45,      5.91);
  fill( sensorList[iSens].ResoErr,                0.02,       0.02,      0.02,      0.02,      0.02,      0.02);
  fill( sensorList[iSens].ResoV,                  5.05,       5.00,      4.94,      5.67,      6.13,      6.40);
  fill( sensorList[iSens].ResoVErr,               0.02,       0.02,      0.02,      0.03,      0.02,      0.02);
  sensorList[iSens].TrackRes = 0.0;
  fill( sensorList[iSens].pixMult,                3.25,       2.98,      2.56,      2.10,      1.79,      1.56);
  fill( sensorList[iSens].pixMultErr,             0.01,       0.01,      0.01,      0.01,      0.01,      0.01);
  sensorList[iSens].color       = kRed;
  sensorList[iSens].markerStyle = 20;
  sensorList[iSens].lineStyle   =  1;

  
  // Some data may need some transformation
  for( Int_t is=0; is<nsensors; is++) {
    for( Int_t in=0; in<nMaxThresholds; in++) {
      sensorList[is].ThreErr[in] = 0.1;
      // deconvolute tracker uncertainty
      //sensorList[is].Reso[in]  = sqrt( sensorList[is].Reso[in]*sensorList[is].Reso[in]-sensorList[is].TrackRes*sensorList[is].TrackRes);
      //sensorList[is].ResoErr[in] = sensorList[is].ResoErr[in]+.1;
    }
  }

			
  //------------------------------------
  //-- Booking

  TH2F *heffi_thre = new TH2F("heffi_thre","Efficiency vs Threshold", ceil(thremax-thremin),thremin, thremax, 50,effimin,effimax);
  heffi_thre->SetXTitle("Threshold (/noise)");
  heffi_thre->SetYTitle("Efficiency (%)");

  TH2F *heffi_mult = new TH2F("heffi_mult","Efficiency vs Pixel multiplicity", ceil(multmax-multmin),multmin,multmax, 50,effimin,effimax);
  heffi_mult->SetXTitle("# pixels in hit");
  heffi_mult->SetYTitle("efficiency (%)");

  TH2F *heffi_fake = new TH2F("heffi_fake","Efficiency vs Fake hit rate", 100,fakemin,fakemax, 50,effimin,effimax);
  heffi_fake->SetXTitle("Fake hit rate");
  heffi_fake->SetYTitle("efficiency (%)");

  TH2F *hfake_thre = new TH2F("hfake_thre","Fake hit rate (whole sensor) vs Threshold", ceil(thremax-thremin),thremin, thremax, 100,fakemin,fakemax);
  hfake_thre->SetXTitle("Threshold (/noise)");
  hfake_thre->SetYTitle("Fake hit rate");

  TH2F *hfake_mult = new TH2F("hfake_mult","Fake hit rate (whole sensor) vs Pixel multiplicity", ceil(multmax-multmin),multmin,multmax, 100,fakemin,fakemax);
  hfake_mult->SetXTitle("# pixels in hit");
  hfake_mult->SetYTitle("Fake hit rate");

  TH2F *hfake_fakegeom = new TH2F("hfake_fakegeom","Fake hit rate (whole sensor) vs Fake hit rate", 100,fakemin,fakemax, 100,fakemin,fakemax);
  hfake_fakegeom->SetXTitle("Fake hit rate");
  hfake_fakegeom->SetYTitle("Fake hit rate (whole sensor)");

  TH2F *hreso_thre = new TH2F("hreso_thre","Resolution vs Threshold", ceil(thremax-thremin),thremin,thremax, 100,resolmin,resolmax );
  hreso_thre->SetXTitle("Threshold (/noise)");
  hreso_thre->SetYTitle("Resolution (#mum)");

  TH2F *hreso_mult = new TH2F("hreso_mult","Resolution vs Pixel multiplicity", ceil(multmax-multmin),multmin,multmax, 100,resolmin,resolmax);
  hreso_mult->SetXTitle("# pixels in hit");
  hreso_mult->SetYTitle("Resolution (#mum)");

  TH2F *hmult_thre = new TH2F("hmult_thre","Pixel multiplicity vs Threshold", ceil(thremax-thremin),thremin, thremax, ceil(multmax-multmin),multmin,multmax);
  hmult_thre->SetXTitle("Threshold (/noise)");
  hmult_thre->SetYTitle("# pixels in hit");

  //--- threshold in mV
  TH2F *heffi_threV = new TH2F("heffi_threV","Efficiency vs Threshold", ceil(thremax-thremin),thremin, thremax, 50,effimin,effimax);
  heffi_threV->SetXTitle("Threshold (mV)");
  heffi_threV->SetYTitle("Efficiency (%)");
  
  TH2F *hfake_threV = new TH2F("hfake_threV","Fake hit rate (whole sensor) vs Threshold", ceil(thremax-thremin),thremin, thremax, 100,fakemin,fakemax);
  hfake_threV->SetXTitle("Threshold (mV)");
  hfake_threV->SetYTitle("Fake hit rate");
  
  TH2F *hreso_threV = new TH2F("hreso_threV","Resolution vs Threshold", ceil(thremax-thremin),thremin,thremax, 100,resolmin,resolmax );
  hreso_threV->SetXTitle("Threshold (mV)");
  hreso_threV->SetYTitle("Resolution (#mum)");
  
  TH2F *hmult_threV = new TH2F("hmult_threV","Pixel multiplicity vs Threshold", ceil(thremax-thremin),thremin, thremax, ceil(multmax-multmin),multmin,multmax);
  hmult_threV->SetXTitle("Threshold (mV)");
  hmult_threV->SetYTitle("# pixels in hit");
  
  TGraphErrors **geffi_threV  = new TGraphErrors*[nsensors];
  TGraphErrors **gfake_threV  = new TGraphErrors*[nsensors];
  TGraphErrors **greso_threV  = new TGraphErrors*[nsensors];
  TGraphErrors **gresoV_threV = new TGraphErrors*[nsensors];
  TGraphErrors **gmult_threV  = new TGraphErrors*[nsensors];
  
  
  TLegend *l = new TLegend(0.45,0.15,0.80,0.40, "");
  l->SetFillStyle(0);
  l->SetBorderSize(0);
  l->SetTextSize(.02);

  TGraphErrors **geffi_mult = new TGraphErrors*[nsensors];
  TGraphErrors **geffi_fake = new TGraphErrors*[nsensors];
  TGraphErrors **gfake_mult = new TGraphErrors*[nsensors];
  TGraphErrors **gfake_fakegeom = new TGraphErrors*[nsensors];
  TGraphErrors **greso_mult = new TGraphErrors*[nsensors];
  TGraphErrors **gmult_thre = new TGraphErrors*[nsensors];

  for( Int_t i=0; i<nsensors; i++) {

    if( sensorList[i].nThresholds==0 ) continue; // JB 2011/07/01
    
    // print values
    cout << " ** " << sensorList[i].longName << "-------" << endl;
    for( Int_t j=0; j<sensorList[i].nThresholds; j++){
      cout << " thres " << j << ": " << sensorList[i].Thre[j] << ", mult : " << sensorList[i].pixMult[j] << ", eff : " << sensorList[i].Eff[j] << ", fake : " << sensorList[i].Fake[j] << ", res : " << sensorList[i].Reso[j] << endl;
    }

    geffi_thre[i] = new TGraphErrors( sensorList[i].nThresholds, sensorList[i].Thre, sensorList[i].Eff, sensorList[i].ThreErr, sensorList[i].EffErr);
    geffi_thre[i]->SetLineStyle(sensorList[i].lineStyle);
    geffi_thre[i]->SetLineColor(sensorList[i].color);
    geffi_thre[i]->SetMarkerStyle(sensorList[i].markerStyle);
    geffi_thre[i]->SetMarkerColor(sensorList[i].color);
    geffi_thre[i]->SetMarkerSize(markerSize);

    geffi_mult[i] = new TGraphErrors( sensorList[i].nThresholds, sensorList[i].pixMult, sensorList[i].Eff, sensorList[i].pixMultErr, sensorList[i].EffErr);
    geffi_mult[i]->SetLineStyle(sensorList[i].lineStyle);
    geffi_mult[i]->SetLineColor(sensorList[i].color);
    geffi_mult[i]->SetMarkerStyle(sensorList[i].markerStyle);
    geffi_mult[i]->SetMarkerColor(sensorList[i].color);
    geffi_mult[i]->SetMarkerSize(markerSize);

    geffi_fake[i] = new TGraphErrors( sensorList[i].nThresholds, sensorList[i].Fake, sensorList[i].Eff, sensorList[i].FakeErr, sensorList[i].EffErr);
    geffi_fake[i]->SetLineStyle(sensorList[i].lineStyle);
    geffi_fake[i]->SetLineColor(sensorList[i].color);
    geffi_fake[i]->SetMarkerStyle(sensorList[i].markerStyle);
    geffi_fake[i]->SetMarkerColor(sensorList[i].color);
    geffi_fake[i]->SetMarkerSize(markerSize);

    gfake_thre[i] = new TGraphErrors( sensorList[i].nThresholds, sensorList[i].Thre, sensorList[i].Fake, sensorList[i].ThreErr, sensorList[i].FakeErr);
    gfake_thre[i]->SetLineStyle(sensorList[i].lineStyle);
    gfake_thre[i]->SetLineColor(sensorList[i].color);
    gfake_thre[i]->SetMarkerStyle(sensorList[i].markerStyle);
    gfake_thre[i]->SetMarkerColor(sensorList[i].color);
    gfake_thre[i]->SetMarkerSize(markerSize);

    gfake_mult[i] = new TGraphErrors( sensorList[i].nThresholds, sensorList[i].pixMult, sensorList[i].Fake, sensorList[i].pixMultErr, sensorList[i].FakeErr);
    gfake_mult[i]->SetLineStyle(sensorList[i].lineStyle);
    gfake_mult[i]->SetLineColor(sensorList[i].color);
    gfake_mult[i]->SetMarkerStyle(sensorList[i].markerStyle);
    gfake_mult[i]->SetMarkerColor(sensorList[i].color);
    gfake_mult[i]->SetMarkerSize(markerSize);

    gfake_fakegeom[i] = new TGraphErrors( sensorList[i].nThresholds, sensorList[i].Fake, sensorList[i].Fake, sensorList[i].FakeErr, sensorList[i].FakeErr);
    gfake_fakegeom[i]->SetLineStyle(sensorList[i].lineStyle);
    gfake_fakegeom[i]->SetLineColor(sensorList[i].color);
    gfake_fakegeom[i]->SetMarkerStyle(sensorList[i].markerStyle);
    gfake_fakegeom[i]->SetMarkerColor(sensorList[i].color);
    gfake_fakegeom[i]->SetMarkerSize(markerSize);

    greso_thre[i] = new TGraphErrors( sensorList[i].nThresholds, sensorList[i].Thre, sensorList[i].Reso, sensorList[i].ThreErr, sensorList[i].ResoErr);
    greso_thre[i]->SetLineStyle(sensorList[i].lineStyle);
    greso_thre[i]->SetLineColor(sensorList[i].color);
    greso_thre[i]->SetMarkerStyle(sensorList[i].markerStyle);
    greso_thre[i]->SetMarkerColor(sensorList[i].color);
    greso_thre[i]->SetMarkerSize(markerSize);

    gresoV_thre[i] = new TGraphErrors( sensorList[i].nThresholds, sensorList[i].Thre, sensorList[i].ResoV, sensorList[i].ThreErr, sensorList[i].ResoVErr);
    gresoV_thre[i]->SetLineStyle(2);
    gresoV_thre[i]->SetLineColor(sensorList[i].color);
    gresoV_thre[i]->SetMarkerStyle(sensorList[i].markerStyle);
    gresoV_thre[i]->SetMarkerColor(sensorList[i].color);
    gresoV_thre[i]->SetMarkerSize(markerSize);

    greso_mult[i] = new TGraphErrors( sensorList[i].nThresholds, sensorList[i].pixMult, sensorList[i].Reso, sensorList[i].pixMultErr, sensorList[i].ResoErr);
    greso_mult[i]->SetLineStyle(sensorList[i].lineStyle);
    greso_mult[i]->SetLineColor(sensorList[i].color);
    greso_mult[i]->SetMarkerStyle(sensorList[i].markerStyle);
    greso_mult[i]->SetMarkerColor(sensorList[i].color);
    greso_mult[i]->SetMarkerSize(markerSize);

    gmult_thre[i] = new TGraphErrors( sensorList[i].nThresholds, sensorList[i].Thre, sensorList[i].pixMult, sensorList[i].ThreErr, sensorList[i].pixMultErr);
    gmult_thre[i]->SetLineStyle(sensorList[i].lineStyle);
    gmult_thre[i]->SetLineColor(sensorList[i].color);
    gmult_thre[i]->SetMarkerStyle(sensorList[i].markerStyle);
    gmult_thre[i]->SetMarkerColor(sensorList[i].color);
    gmult_thre[i]->SetMarkerSize(markerSize);

    //--- threshold in mV
    geffi_threV[i] = new TGraphErrors( sensorList[i].nThresholds, sensorList[i].ThreV, sensorList[i].Eff, sensorList[i].ThreErr, sensorList[i].EffErr);
    geffi_threV[i]->SetLineStyle(sensorList[i].lineStyle);
    geffi_threV[i]->SetLineColor(sensorList[i].color);
    geffi_threV[i]->SetMarkerStyle(sensorList[i].markerStyle);
    geffi_threV[i]->SetMarkerColor(sensorList[i].color);
    geffi_threV[i]->SetMarkerSize(markerSize);
    
    gfake_threV[i] = new TGraphErrors( sensorList[i].nThresholds, sensorList[i].ThreV, sensorList[i].Fake, sensorList[i].ThreErr, sensorList[i].FakeErr);
    gfake_threV[i]->SetLineStyle(sensorList[i].lineStyle);
    gfake_threV[i]->SetLineColor(sensorList[i].color);
    gfake_threV[i]->SetMarkerStyle(sensorList[i].markerStyle);
    gfake_threV[i]->SetMarkerColor(sensorList[i].color);
    gfake_threV[i]->SetMarkerSize(markerSize);
    
    greso_threV[i] = new TGraphErrors( sensorList[i].nThresholds, sensorList[i].ThreV, sensorList[i].Reso, sensorList[i].ThreErr, sensorList[i].ResoErr);
    greso_threV[i]->SetLineStyle(sensorList[i].lineStyle);
    greso_threV[i]->SetLineColor(sensorList[i].color);
    greso_threV[i]->SetMarkerStyle(sensorList[i].markerStyle);
    greso_threV[i]->SetMarkerColor(sensorList[i].color);
    greso_threV[i]->SetMarkerSize(markerSize);

    gresoV_threV[i] = new TGraphErrors( sensorList[i].nThresholds, sensorList[i].ThreV, sensorList[i].ResoV, sensorList[i].ThreErr, sensorList[i].ResoVErr);
    gresoV_threV[i]->SetLineStyle(1);
    gresoV_threV[i]->SetLineColor(sensorList[i].color);
    gresoV_threV[i]->SetMarkerStyle(sensorList[i].markerStyle);
    gresoV_threV[i]->SetMarkerColor(sensorList[i].color);
    gresoV_threV[i]->SetMarkerSize(markerSize);
    
    gmult_threV[i] = new TGraphErrors( sensorList[i].nThresholds, sensorList[i].ThreV, sensorList[i].pixMult, sensorList[i].ThreErr, sensorList[i].pixMultErr);
    gmult_threV[i]->SetLineStyle(sensorList[i].lineStyle);
    gmult_threV[i]->SetLineColor(sensorList[i].color);
    gmult_threV[i]->SetMarkerStyle(sensorList[i].markerStyle);
    gmult_threV[i]->SetMarkerColor(sensorList[i].color);
    gmult_threV[i]->SetMarkerSize(markerSize);
    

  }


  //------------------------------------
  //-- Ploting

  gStyle->SetOptStat(0);

  TCanvas *ceffthr = new TCanvas("ceffthr","Binary sensors comparison", 700, 700);
  TText *preliminary = new TText( .3, .3, "PRELIMINARY");
  preliminary->SetNDC();
  preliminary->SetTextSize(.12);
  preliminary->SetTextAngle(45.);
  preliminary->SetTextColor(17);
  heffi_thre->Draw();
  if( ifPreliminary) preliminary->Draw();
  for( Int_t i=0; i<nsensors; i++) {
    if( sensorList[i].nThresholds ) geffi_thre[i]->Draw("PEL");
  }
  l->Draw();
  gPad->SetGrid(1,1);
  if(ifSave) gPad->Print( (outputDir+TString("plot_effi_thre.eps")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_effi_thre.gif")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_effi_thre.root")).Data() );

  TCanvas *ceffmult = new TCanvas("ceffmult","Binary sensors comparison", 700, 700);
  heffi_mult->Draw();
  if( ifPreliminary) preliminary->Draw();
  for( Int_t i=0; i<nsensors; i++) {
    if( sensorList[i].nThresholds ) {
      geffi_mult[i]->Draw("PEL");
      l->AddEntry( geffi_mult[i], sensorList[i].longName, "pl");
    }
  }
  l->Draw();
  gPad->SetGrid(1,1);
  if(ifSave) gPad->Print( (outputDir+TString("plot_effi_mult.eps")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_effi_mult.gif")).Data() );
  
  TCanvas *cefffak = new TCanvas("cefffak","Binary sensors comparison", 700, 700);
  heffi_fake->Draw();
  if( ifPreliminary) preliminary->Draw();
  for( Int_t i=0; i<nsensors; i++) {
    if( sensorList[i].nThresholds ) geffi_fake[i]->Draw("PEL");
  }
  l->Draw();
  gPad->SetLogx();
  gPad->SetGrid(1,1);
  if(ifSave) gPad->Print( (outputDir+TString("plot_effi_fake.eps")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_effi_fake.gif")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_effi_fake.root")).Data() );

  TCanvas *cfakmul = new TCanvas("cfakmul","Binary sensors comparison", 700, 700);
  hfake_mult->Draw();
  if( ifPreliminary) preliminary->Draw();
  for( Int_t i=0; i<nsensors; i++) {
    if( sensorList[i].nThresholds ) gfake_mult[i]->Draw("PEL");
  }
  l->Draw();
  gPad->SetLogy();
  gPad->SetGrid(1,1);
  if(ifSave) gPad->Print( (outputDir+TString("plot_fake_mult.eps")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_fake_mult.gif")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_fake_mult.root")).Data() );

  TCanvas *cfakthr = new TCanvas("cfakthr","Binary sensors comparison", 700, 700);
  hfake_thre->Draw();
  if( ifPreliminary) preliminary->Draw();
  for( Int_t i=0; i<nsensors; i++) {
    if( sensorList[i].nThresholds ) gfake_thre[i]->Draw("PEL");
  }
  l->Draw();
  gPad->SetLogy();
  gPad->SetGrid(1,1);
  if(ifSave) gPad->Print( (outputDir+TString("plot_fake_thre.eps")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_fake_thre.gif")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_fake_thre.root")).Data() );

  TCanvas *cfakfag = new TCanvas("cfakfag","Binary sensors comparison", 700, 700);
  hfake_fakegeom->Draw();
  if( ifPreliminary) preliminary->Draw();
  for( Int_t i=0; i<nsensors; i++) {
    if( sensorList[i].nThresholds ) gfake_fakegeom[i]->Draw("PEL");
  }
  l->Draw();
  gPad->SetLogx();
  gPad->SetLogy();
  gPad->SetGrid(1,1);
  if(ifSave) gPad->Print( (outputDir+TString("plot_fake_fakegeom.eps")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_fake_fakegeom.gif")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_fake_fakegeom.root")).Data() );

  TCanvas *cresthr = new TCanvas("cresthr","Binary sensors comparison", 700, 700);
  hreso_thre->Draw();
  if( ifPreliminary) preliminary->Draw();
  for( Int_t i=0; i<nsensors; i++) {
    if( sensorList[i].nThresholds ) greso_thre[i]->Draw("PEL");
  }
  l->Draw();
  gPad->SetGrid(1,1);
  if(ifSave) gPad->Print( (outputDir+TString("plot_reso_thre.eps")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_reso_thre.gif")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_reso_thre.root")).Data() );

  TCanvas *cresmul = new TCanvas("cresmul","Binary sensors comparison", 700, 700);
  hreso_mult->Draw();
  if( ifPreliminary) preliminary->Draw();
  for( Int_t i=0; i<nsensors; i++) {
    if( sensorList[i].nThresholds ) greso_mult[i]->Draw("PEL");
  }
  l->Draw();
  gPad->SetGrid(1,1);
  if(ifSave) gPad->Print( (outputDir+TString("plot_reso_mult.eps")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_reso_mult.gif")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_reso_mult.root")).Data() );


  TCanvas *cmulthr = new TCanvas("cmulthr","Binary sensors comparison", 700, 700);
  hmult_thre->Draw();
  if( ifPreliminary) preliminary->Draw();
  for(Int_t i=0; i<nsensors; i++) {
    if( sensorList[i].nThresholds ) gmult_thre[i]->Draw("PEL");
  }
  l->Draw();
  gPad->SetGrid(1,1);
  if(ifSave) gPad->Print( (outputDir+TString("plot_mult_thre.eps")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_mult_thre.gif")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_mult_thre.root")).Data() );

  //--- threshold in mV

  TCanvas *ceffthrV = new TCanvas("ceffthrV","Binary sensors comparison", 700, 700);
  TText *preliminary = new TText( .3, .3, "PRELIMINARY");
  preliminary->SetNDC();
  preliminary->SetTextSize(.12);
  preliminary->SetTextAngle(45.);
  preliminary->SetTextColor(17);
  heffi_threV->Draw();
  if( ifPreliminary) preliminary->Draw();
  for( Int_t i=0; i<nsensors; i++) {
    if( sensorList[i].nThresholds ) geffi_threV[i]->Draw("PEL");
  }
  l->Draw();
  gPad->SetGrid(1,1);
  if(ifSave) gPad->Print( (outputDir+TString("plot_effi_threV.eps")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_effi_threV.gif")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_effi_threV.root")).Data() );
  
  TCanvas *cfakthrV = new TCanvas("cfakthrV","Binary sensors comparison", 700, 700);
  hfake_threV->Draw();
  if( ifPreliminary) preliminary->Draw();
  for( Int_t i=0; i<nsensors; i++) {
    if( sensorList[i].nThresholds ) gfake_threV[i]->Draw("PEL");
  }
  l->Draw();
  gPad->SetLogy();
  gPad->SetGrid(1,1);
  if(ifSave) gPad->Print( (outputDir+TString("plot_fake_threV.eps")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_fake_threV.gif")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_fake_threV.root")).Data() );
  
  TCanvas *cresthrV = new TCanvas("cresthrV","Binary sensors comparison", 700, 700);
  hreso_threV->Draw();
  if( ifPreliminary) preliminary->Draw();
  for( Int_t i=0; i<nsensors; i++) {
    if( sensorList[i].nThresholds ) greso_threV[i]->Draw("PEL");
  }
  l->Draw();
  gPad->SetGrid(1,1);
  if(ifSave) gPad->Print( (outputDir+TString("plot_reso_threV.eps")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_reso_threV.gif")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_reso_threV.root")).Data() );
    
  TCanvas *cmulthrV = new TCanvas("cmulthrV","Binary sensors comparison", 700, 700);
  hmult_threV->Draw();
  if( ifPreliminary) preliminary->Draw();
  for(Int_t i=0; i<nsensors; i++) {
    if( sensorList[i].nThresholds ) gmult_threV[i]->Draw("PEL");
  }
  l->Draw();
  gPad->SetGrid(1,1);
  if(ifSave) gPad->Print( (outputDir+TString("plot_mult_threV.eps")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_mult_threV.gif")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_mult_threV.root")).Data() );
  

  //------------------------------------
  // Summary plot per individualy sensor

  Char_t cname[20], ctitle[100];
  TCanvas *ca;
  TH2F **hframe = new TH2F*[nsensors];

  Float_t a = (effimax - effimin)/(resolmax-resolmin);
  Float_t b = effimin-resolmin*a;

  //fakemax *= 500; // for better display
  Float_t af = (effimax-effimin)/(log10(fakemax)-log10(fakemin));
  Float_t bf = effimin-log10(fakemin)*af;

  gStyle->SetPadRightMargin(0.35);
  gStyle->SetPadBottomMargin(0.25);
   gStyle->SetTitleOffset(0.8,"y");

  for( Int_t i=0; i<nsensors; i++ ) { // Loop on sensors

    if( sensorList[i].nThresholds==0 )  continue; // JB 2011/07/01
    
    cout << "Creating summary plot for sensor " << i << " " << sensorList[i].shortName << endl;

    sprintf( cname, "ca%d", i);
    sprintf( ctitle, "%s performances", sensorList[i].longName);
    ca = new TCanvas( cname, ctitle, 800, 550);
    ca->SetGrid(1,1);
    
    sprintf( cname, "hframe%d", i);
    sprintf( ctitle, "%s", sensorList[i].longName);
    hframe[i] = new TH2F(cname, ctitle, sensorList[i].nThresholds+1, sensorList[i].Thre[0]-1, sensorList[i].Thre[sensorList[i].nThresholds-1]+1, (Int_t)(effimax-effimin), effimin, effimax);
    hframe[i]->SetXTitle("Threshold (/ noise)");
    hframe[i]->SetYTitle("Efficiency (%)");
    hframe[i]->Draw();
    if( ifPreliminary) preliminary->Draw();
    
    //--efficiency:
    geffi[i] = new TGraphErrors( (TGraphErrors&)*geffi_thre[i] );
    geffi[i]->SetMarkerColor(1);
    geffi[i]->SetMarkerStyle(21);
    geffi[i]->SetMarkerSize(markerSize/1.5);
    geffi[i]->SetLineColor(1);
    geffi[i]->SetLineWidth(lineWidth);
    geffi[i]->Draw("LP");
    
    //--Resolution
    greso[i]= new TGraphErrors( (TGraphErrors&)*greso_thre[i] );
    greso[i]->SetMarkerColor(resolcolor);
    greso[i]->SetMarkerStyle(21);
    greso[i]->SetMarkerSize(markerSize/1.5);
    greso[i]->SetLineColor(resolcolor);
    greso[i]->SetLineWidth(lineWidth);
    greso[i]->SetLineStyle(1);  
    
    for (Int_t j=0; j<sensorList[i].nThresholds; j++){
      greso[i]->SetPoint( j, sensorList[i].Thre[j], sensorList[i].Reso[j]*a+b);
      greso[i]->SetPointError( j, sensorList[i].ThreErr[j], sensorList[i].ResoErr[j]*a);
    }
    greso[i]->Draw("LP");

    //--Resolution V
    gresoV[i]= new TGraphErrors( (TGraphErrors&)*gresoV_thre[i] );
    gresoV[i]->SetMarkerColor(resolcolor);
    gresoV[i]->SetMarkerStyle(21);
    gresoV[i]->SetMarkerSize(markerSize/1.5);
    gresoV[i]->SetLineColor(resolcolor);
    gresoV[i]->SetLineWidth(lineWidth);
    gresoV[i]->SetLineStyle(2);  
    
    for (Int_t j=0; j<sensorList[i].nThresholds; j++){
      gresoV[i]->SetPoint( j, sensorList[i].Thre[j], sensorList[i].ResoV[j]*a+b);
      gresoV[i]->SetPointError( j, sensorList[i].ThreErr[j], sensorList[i].ResoVErr[j]*a);
    }
    gresoV[i]->Draw("LP");
    
    TGaxis *axisresol = new TGaxis(sensorList[i].Thre[sensorList[i].nThresholds-1]+1,
				   effimin,
				   sensorList[i].Thre[sensorList[i].nThresholds-1]+1,
				   effimax,
				   resolmin,resolmax,510,"+L");
    axisresol->SetLineColor(resolcolor);
    axisresol->SetLabelColor(resolcolor);
    axisresol->SetTitleColor(resolcolor);
    //axisresol->SetTitle("Resolution (#mum)");
    axisresol->SetTitle("Residual (#mum)");
    //axisresol->SetTitleSize(0.04);
    axisresol->Draw("same");
    
    //--Fake rate:
    gfake[i] = new TGraphErrors( (TGraphErrors&)*gfake_thre[i] );
    gfake[i]->SetMarkerStyle(21);
    gfake[i]->SetMarkerSize(markerSize/1.5);
    gfake[i]->SetMarkerColor(fakecolor);
    gfake[i]->SetLineColor(fakecolor);
    gfake[i]->SetLineWidth(lineWidth);

    for (Int_t j=0 ; j<sensorList[i].nThresholds ; j++){
      gfake[i]->SetPoint(j,sensorList[i].Thre[j],log10(sensorList[i].Fake[j])*af+bf);
      gfake[i]->SetPointError(j,sensorList[i].ThreErr[j],log10(sensorList[i].FakeErr[j])*af);
    }
    gfake[i]->Draw("LP");
    
    TGaxis *axisfake = new TGaxis(sensorList[i].Thre[sensorList[i].nThresholds-1]+3.2,
				  effimin,
				  sensorList[i].Thre[sensorList[i].nThresholds-1]+3.2,
				  effimax,
				  fakemin,fakemax,510,"+LG");
    axisfake->SetLineColor(fakecolor);
    axisfake->SetLabelColor(fakecolor);
    axisfake->SetTitleColor(fakecolor);
    //axisfake->SetTitleOffset(1.80);
    axisfake->SetTitle("Average fake hit rate/pixel/event");
    axisfake->SetTitleOffset(1.2);
    //axisfake->SetTitleSize(0.04);
    axisfake->Draw();

    // Axis for threshold in mV
    //  only if information is provided
    if( sensorList[i].Thre[0] > 0. ) {
      TGaxis *axisThresV = new TGaxis(sensorList[i].Thre[0], effimin-2.7
				      ,sensorList[i].Thre[sensorList[i].nThresholds-1], effimin-2.7
				      ,sensorList[i].ThreV[0], sensorList[i].ThreV[sensorList[i].nThresholds-1]
				      ,510, "+L");
      axisThresV->SetLineColor(1);
      axisThresV->SetTitle("Threshold (mV)");
      axisThresV->SetTitleColor( 1);
      axisThresV->SetTitleSize( hframe[i]->GetTitleSize( "X") );
      axisThresV->SetTitleFont( hframe[i]->GetTitleFont( "X") );
      axisThresV->SetLabelColor( hframe[i]->GetLabelColor( "X") );
      axisThresV->SetLabelSize( hframe[i]->GetLabelSize( "X") );
      axisThresV->SetLabelFont( hframe[i]->GetLabelFont( "X") );
      axisThresV->Draw("same");      
    }
         
    sprintf( ctitle, "%s/perf_%s.gif", outputDir.Data(), sensorList[i].shortName);
    if(ifSave) gPad->Print(ctitle);	
    sprintf( ctitle, "%s/perf_%s.eps", outputDir.Data(), sensorList[i].shortName);
    if(ifSave) gPad->Print(ctitle);	
    sprintf( ctitle, "%s/perf_%s.root", outputDir.Data(), sensorList[i].shortName);
    if(ifSave) gPad->Print(ctitle);	
    
  } // end loop on sensors
  

}




//------------------------------------------------------------------------------
void plot_comparison(Int_t sensor1, Int_t sensor2){
  
  plot_comparison( sensorList, geffi, greso, gfake, sensor1, sensor2);
  
}




//------------------------------------------------------------------------------
void plot_comparison(sensor *sensorList, TGraphErrors **geffi, TGraphErrors **greso, TGraphErrors ** gfake, Int_t sensor1, Int_t sensor2){

  const Int_t N_superimosed_sensors=2;
  Int_t superimposed_sensors[N_superimosed_sensors]={sensor1,sensor2};
  Char_t cname[20], ctitle[100];
  TText *preliminary = new TText( .3, .3, "PRELIMINARY");

  sprintf( cname, "ca_superimposed");
  sprintf( ctitle, "%s vs. %s", sensorList[superimposed_sensors[0]].longName,sensorList[superimposed_sensors[1]].longName);
  TCanvas* ca_superimposed = new TCanvas( cname, ctitle, 800, 550);
  cout << "Canvas created" << endl;
  //ca_superimposed->Draw();
  //ca_superimposed->cd();
  ca_superimposed->SetGrid(1,1);
  ca_superimposed->Update();

  sprintf( cname, "hframe_superimposed");
  sprintf( ctitle, "%s vs. %s", sensorList[superimposed_sensors[0]].longName,sensorList[superimposed_sensors[1]].longName);
  TH2F* hframe_superimposed = new TH2F(cname, ctitle,
				       TMath::Max(sensorList[superimposed_sensors[0]].nThresholds,
						  sensorList[superimposed_sensors[1]].nThresholds)+1,
				       TMath::Min(sensorList[superimposed_sensors[0]].Thre[0],
						  sensorList[superimposed_sensors[1]].Thre[0])-1,
				       TMath::Max(sensorList[superimposed_sensors[0]].Thre[sensorList[superimposed_sensors[0]].nThresholds-1],
						  sensorList[superimposed_sensors[1]].Thre[sensorList[superimposed_sensors[1]].nThresholds-1])+1,
				       (Int_t)(effimax-effimin), effimin, effimax);
  hframe_superimposed->SetXTitle("Threshold (/ noise)");
  hframe_superimposed->SetYTitle("Efficiency (%)");
  hframe_superimposed->DrawClone();
#if 0
  if( ifPreliminary) preliminary->DrawClone();
  TLegend* legend_superimposed=new TLegend(0.15,0.80,0.65,0.90, "");
  legend_superimposed->SetFillStyle(1);
  legend_superimposed->SetFillColor(0);
  legend_superimposed->SetBorderSize(1.);
  legend_superimposed->SetTextSize(.03);
  cout << "hframe created" << endl;
  
  for (Int_t i=0;i<N_superimosed_sensors;i++){
    geffi[superimposed_sensors[i]]->SetLineStyle(i+1);
    geffi[superimposed_sensors[i]]->DrawClone("LP");
    greso[superimposed_sensors[i]]->SetLineStyle(i+1);
    greso[superimposed_sensors[i]]->DrawClone("LP");
    gfake[superimposed_sensors[i]]->SetLineStyle(i+1);
    gfake[superimposed_sensors[i]]->DrawClone("LP");
    legend_superimposed->AddEntry(geffi[superimposed_sensors[i]],sensorList[superimposed_sensors[i]].longName, "l");
  }
  cout << "graphs created" << endl;
  
  TGaxis *axisresol = new TGaxis(TMath::Max(sensorList[superimposed_sensors[0]].Thre[sensorList[superimposed_sensors[0]].nThresholds-1],sensorList[superimposed_sensors[1]].Thre[sensorList[superimposed_sensors[1]].nThresholds-1])+2,effimin,TMath::Max(sensorList[superimposed_sensors[0]].Thre[sensorList[superimposed_sensors[0]].nThresholds-1],sensorList[superimposed_sensors[1]].Thre[sensorList[superimposed_sensors[1]].nThresholds-1])+2,effimax,resolmin,resolmax,510,"+L");
  axisresol->SetLineColor(resolcolor);
  axisresol->SetLabelColor(resolcolor);
  axisresol->SetTitleColor(resolcolor);
  axisresol->SetTitle("Resolution (#mum)");
  //axisresol->SetTitleSize(0.04);
  axisresol->Draw("same");
  
  TGaxis *axisfake = new TGaxis(TMath::Max(sensorList[superimposed_sensors[0]].Thre[sensorList[superimposed_sensors[0]].nThresholds-1],sensorList[superimposed_sensors[1]].Thre[sensorList[superimposed_sensors[1]].nThresholds-1])+3.5,effimin,TMath::Max(sensorList[superimposed_sensors[0]].Thre[sensorList[superimposed_sensors[0]].nThresholds-1],sensorList[superimposed_sensors[1]].Thre[sensorList[superimposed_sensors[1]].nThresholds-1])+3.5,effimax,fakemin,fakemax,510,"+LG");
  axisfake->SetLineColor(fakecolor);
  axisfake->SetLabelColor(fakecolor);
  axisfake->SetTitleColor(fakecolor);
  //axisfake->SetTitleOffset(1.80);
  axisfake->SetTitle("Average fake hit rate/pixel/event");
  axisfake->SetTitleOffset(1.2);
  //axisfake->SetTitleSize(0.04);
  axisfake->Draw();
  
  legend_superimposed->Draw();
#endif  
  //   return ca_superimposed;
}


