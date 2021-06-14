// Compare the results (efficiency, fake, resolution, pixel mult)
// for different binary output sensors
//
// Inputs:
//  Fill your data in the beginning of method compare().
//  Each sensor/configuration is a new structure containing arrays indexed on thresholds.
//  Display and superimposing is controlled by the two first arguments
//  Some options can be tuned (like axis range) at the beginning of comapre()
//
// Usage:
//  In ROOT:
//   >.L compareBinarySensorsResults_mimosis.C
//   >compare()
//

#include "TString.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TGraphErrors.h"
#include <iostream>
#include "TCanvas.h"
#include "TText.h"
#include "TGaxis.h"
#include "TMath.h"

#define nMaxThresholds 10 // Maximum number of thresholds

struct sensor {
  Bool_t display;
  Bool_t superimpose;
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
  Double_t TrackRes;
  Double_t Eff[nMaxThresholds];
  Double_t EffErr[nMaxThresholds];
  Double_t Fake[nMaxThresholds];
  Double_t FakeErr[nMaxThresholds];
  Double_t Thre[nMaxThresholds];
  Double_t ThreErr[nMaxThresholds];
  Double_t ThreV[nMaxThresholds];
  Double_t ThreVErr[nMaxThresholds];
};

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
void compare(){

  // Controlling the display
  Bool_t ifPreliminary = kFALSE; // kTRUE;

  //Char_t outputDir[200];
  TString outputDir = "/Users/jeromeb/Documents/techdoc/cmos/M28/tbeam2011_plots/"; // "/Users/jeromeb/Documents/techdoc/cmos/M26/results_analysis_Sept09/"; // "./"

  //Saving histos or not
  Bool_t ifSave = kFALSE;

  Float_t effimin=94;
  Float_t effimax=101;
  Float_t resolmax = 6.0;
  Float_t resolmin = 1.5;
  Float_t fakemax = 2.e0;
  Float_t fakemin = 1.e-11;
  Float_t multmax = 6.0;
  Float_t multmin = 0.5;
  Float_t thremax = 15.5;
  Float_t thremin = 1.5;

  Float_t markerSize = 1.;
  Float_t lineWidth = 2.0;

  // Controlling the data
  Int_t nsensors = 0;
  const Int_t maxNsensors = 50;
  sensor sensorList[maxNsensors];
  Int_t N_superimosed_sensors = 0;
  Int_t superimposed_sensors[maxNsensors];

  // MIMOSA 22 AHR S7
  // results from Beam Test August-September 2010
  // fake rates from lab measurements
  sensorList[nsensors].display = true;
  sensorList[nsensors].superimpose = true;
  sprintf( sensorList[nsensors].shortName, "MI22AHR-S7");
  sprintf( sensorList[nsensors].longName, "MIMOSA 22 AHR - epi 15um - S7");
  sprintf( sensorList[nsensors].origin, "SPS  2010, pions 120 GeV");
  sensorList[nsensors].nThresholds = 5;
  fill( sensorList[nsensors].Thre, 4.7, 7.1, 8.2, 9.4, 12.9);
  fill( sensorList[nsensors].ThreV, 4.0,  6.0,  7.0,  8.0, 11.0);
  fill( sensorList[nsensors].Eff, 99.91, 99.6 , 99.79, 99.39, 96.7);
  fill( sensorList[nsensors].EffErr, 0.05, 0.1 , 0.07, 0.08, 0.3);
  fill( sensorList[nsensors].Fake, 10.4786e-6, 0.0197072e-6, 0.0014e-6,  0.0014e-6,  0.0014e-6);
  fill( sensorList[nsensors].FakeErr, 0.149e-6, 0.00645e-6, 0.001e-5, 0.001e-5, 0.001e-5);
  fill( sensorList[nsensors].Reso, 4.00, 3.79, 3.80, 3.75, 4.20);
  fill( sensorList[nsensors].ResoErr, 0.05, 0.06, 0.05, 0.03, 0.05);
  sensorList[nsensors].TrackRes = 1.5;
  fill( sensorList[nsensors].pixMult, 4.3, 3.3, 2.9, 2.6, 2.0);
  fill( sensorList[nsensors].pixMultErr, 0.01,  0.01,  0.01,  0.01,  0.01);
  sensorList[nsensors].color = kOrange;
  sensorList[nsensors].markerStyle = 24;
  sensorList[nsensors].lineStyle = 1;
  nsensors++;

  // MIMOSA 22 AHR - S7 - 30C
  // results from Beam Test August-September 2010
  // fake rates from lab measurements
  sensorList[nsensors].display = true;
  sensorList[nsensors].superimpose = true;
  sprintf( sensorList[nsensors].shortName, "MIMOSA22AHR-S7-30C");
  sprintf( sensorList[nsensors].longName, "MIMOSA 22 AHR - epi 15 um - S7 - 30 C");
  sprintf( sensorList[nsensors].origin, "SPS  2010, pions 120 GeV");
  sensorList[nsensors].nThresholds = 3;
  fill( sensorList[nsensors].Thre, 4.1, 6.4, 8.7);
  fill( sensorList[nsensors].ThreV, 4.0, 6.0, 8.0);
  fill( sensorList[nsensors].Eff, 99.89, 99.80, 99.72);
  fill( sensorList[nsensors].EffErr, 0.05, 0.10, 0.16);
  fill( sensorList[nsensors].Fake, 23.7387e-6, 2.67455e-008, 0.00140766e-6);
  fill( sensorList[nsensors].FakeErr, 0.182798e-6, 6.13584e-009, 0.00140766e-6);
  fill( sensorList[nsensors].Reso, 4.32, 3.97, 3.8);
  fill( sensorList[nsensors].ResoErr, 0.07, 0.07, 0.10);
  sensorList[nsensors].TrackRes = 1.5;
  fill( sensorList[nsensors].pixMult, 4.7, 3.4, 2.7);
  fill( sensorList[nsensors].pixMultErr, 0.01, 0.01, 0.01);
  sensorList[nsensors].color = kOrange+1;
  sensorList[nsensors].markerStyle = 24;
  sensorList[nsensors].lineStyle = 1;
  nsensors++;

  // MIMOSA 22 AHR - S7 - 150 kRad - 30 C
  // results from Beam Test August-September 2010
  // fake rates from lab measurements
  sensorList[nsensors].display = false;
  sensorList[nsensors].superimpose = false;
  sprintf( sensorList[nsensors].shortName, "MIMOSA22AHR-S7-150kRad");
  sprintf( sensorList[nsensors].longName, "MIMOSA 22 AHR - S7 - epi 15 um - 150kRad");
  sprintf( sensorList[nsensors].origin, "SPS  2010, pions 120 GeV");
  sensorList[nsensors].nThresholds = 3;
  fill( sensorList[nsensors].Thre, 4.5,  5.4,  7.2);
  fill( sensorList[nsensors].ThreV, 5.0,  6.0,  8.0);
  fill( sensorList[nsensors].Eff, 99.87, 99.80, 99.66);
  fill( sensorList[nsensors].EffErr, 0.05, 0.07, 0.11);
  fill( sensorList[nsensors].Fake, 0.0221734e-3, 0.00193694e-3, 2.53378e-08);
  fill( sensorList[nsensors].FakeErr, 0.00021667e-3, 0.00007385e-3, 0.00000844e-3);
  fill( sensorList[nsensors].Reso, 4.13, 3.92, 3.75);
  fill( sensorList[nsensors].ResoErr, 0.04, 0.05, 0.05);
  sensorList[nsensors].TrackRes = 1.5;
  fill( sensorList[nsensors].pixMult, 4.1, 3.6, 2.8);
  fill( sensorList[nsensors].pixMultErr, 0.03,  0.03,  0.03);
  sensorList[nsensors].color = kOrange+2;
  sensorList[nsensors].markerStyle = 24;
  sensorList[nsensors].lineStyle = 1;
  nsensors++;

  // MIMOSA 22 AHR - S7 - 150kRad 3.e12neq - 30 C
  // results from Beam Test August-September 2010
  // fake rates from lab measurements
  sensorList[nsensors].display = true;
  sensorList[nsensors].superimpose = true;
  sprintf( sensorList[nsensors].shortName, "MIMOSA22AHR15-S7-150kRad-3e12neq");
  sprintf( sensorList[nsensors].longName, "MIMOSA 22 AHR - epi 15 um - S7 - 150 kRad + 3e12 neq");
  sprintf( sensorList[nsensors].origin, "SPS  2010, pions 120 GeV");
  sensorList[nsensors].nThresholds = 3;
  fill( sensorList[nsensors].Thre, 4.5, 5.4,  7.2);
  fill( sensorList[nsensors].ThreV, 5.0, 6.0,  8.0);
  fill( sensorList[nsensors].Eff, 99.76, 99.84, 99.15);
  fill( sensorList[nsensors].EffErr, 0.09,  0.05, 0.13);
  fill( sensorList[nsensors].Fake, 0.00935811e-3, 0.000389921e-3, 0.00000140766e-3);
  fill( sensorList[nsensors].FakeErr, 0.0001477e-3, 0.00002343e-3, 0.00000140766e-3);
  fill( sensorList[nsensors].Reso, 3.91, 3.86, 3.94);
  fill( sensorList[nsensors].ResoErr, 0.05, 0.04, 0.05);
  sensorList[nsensors].TrackRes = 1.5;
  fill( sensorList[nsensors].pixMult, 3.4, 3.0, 2.5);
  fill( sensorList[nsensors].pixMultErr, 0.02, 0.03,  0.03);
  sensorList[nsensors].color = kOrange+3;
  sensorList[nsensors].markerStyle = 24;
  sensorList[nsensors].lineStyle = 1;
  nsensors++;


  // Some data may need some transformation
  for( Int_t is=0; is<nsensors; is++) {
    for( Int_t in=0; in<nMaxThresholds; in++) {
      sensorList[is].ThreErr[in] = 0.1;
      // deconvolute tracker uncertainty
      sensorList[is].Reso[in]  = sqrt( sensorList[is].Reso[in]*sensorList[is].Reso[in]-sensorList[is].TrackRes*sensorList[is].TrackRes);
      sensorList[is].ResoErr[in] = sensorList[is].ResoErr[in]+.1;
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

  TLegend *l = new TLegend(0.45,0.15,0.80,0.40, "");
  l->SetFillStyle(0);
  l->SetBorderSize(0);
  l->SetTextSize(.02);

  TGraphErrors **geffi_thre = new TGraphErrors*[maxNsensors];
  TGraphErrors **geffi_mult = new TGraphErrors*[maxNsensors];
  TGraphErrors **geffi_fake = new TGraphErrors*[maxNsensors];
  TGraphErrors **gfake_thre = new TGraphErrors*[maxNsensors];
  TGraphErrors **gfake_mult = new TGraphErrors*[maxNsensors];
  TGraphErrors **gfake_fakegeom = new TGraphErrors*[maxNsensors];
  TGraphErrors **greso_thre = new TGraphErrors*[maxNsensors];
  TGraphErrors **greso_mult = new TGraphErrors*[maxNsensors];
  TGraphErrors **gmult_thre = new TGraphErrors*[maxNsensors];

  for( Int_t i=0; i<nsensors; i++) {

    // print values
    cout << " ** " << sensorList[i].longName << "-------";
    if(!sensorList[i].display) {
      cout << " NOT DISPLAYED";
      sensorList[i].superimpose = false;
    }
    if(sensorList[i].superimpose) {
      cout << " for SUPERIMPOSITION";
      superimposed_sensors[N_superimosed_sensors] = i;
      N_superimosed_sensors++;
    }
    cout << endl;
    for( Int_t j=0; j<sensorList[i].nThresholds; j++){
      cout << " thres " << j << ": " << sensorList[i].Thre[j] << ", mult : " << sensorList[i].pixMult[j] << ", eff : " << sensorList[i].Eff[j] << ", fake : " << sensorList[i].Fake[j] << ", res : " << sensorList[i].Reso[j] << endl;
    }
    cout << endl;

    if( !sensorList[i].display ) continue; // JB 2011/07/01

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

  }


  //------------------------------------
  //-- Ploting

  gStyle->SetOptStat(0);

  cout << "Creating comparion plots for each variable." << endl;

  TCanvas *ceffthr = new TCanvas("ceffthr","Binary sensors comparison", 0, 0, 700, 700);
  TText *preliminary = new TText( .3, .3, "PRELIMINARY");
  preliminary->SetNDC();
  preliminary->SetTextSize(.12);
  preliminary->SetTextAngle(45.);
  preliminary->SetTextColor(17);
  heffi_thre->Draw();
  if( ifPreliminary) preliminary->Draw();
  for( Int_t i=0; i<nsensors; i++) {
    if( sensorList[i].display ) geffi_thre[i]->Draw("PEL");
  }
  l->Draw();
  gPad->SetGrid(1,1);
  if(ifSave) gPad->Print( (outputDir+TString("plot_effi_thre.eps")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_effi_thre.gif")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_effi_thre.root")).Data() );

  TCanvas *ceffmult = new TCanvas("ceffmult","Binary sensors comparison", 0, 20, 700, 700);
  heffi_mult->Draw();
  if( ifPreliminary) preliminary->Draw();
  for( Int_t i=0; i<nsensors; i++) {
    if( sensorList[i].display ) {
      geffi_mult[i]->Draw("PEL");
      l->AddEntry( geffi_mult[i], sensorList[i].longName, "pl");
    }
  }
  l->Draw();
  gPad->SetGrid(1,1);
  if(ifSave) gPad->Print( (outputDir+TString("plot_effi_mult.eps")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_effi_mult.gif")).Data() );

  TCanvas *cefffak = new TCanvas("cefffak","Binary sensors comparison", 0, 40, 700, 700);
  heffi_fake->Draw();
  if( ifPreliminary) preliminary->Draw();
  for( Int_t i=0; i<nsensors; i++) {
    if( sensorList[i].display ) geffi_fake[i]->Draw("PEL");
  }
  l->Draw();
  gPad->SetLogx();
  gPad->SetGrid(1,1);
  if(ifSave) gPad->Print( (outputDir+TString("plot_effi_fake.eps")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_effi_fake.gif")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_effi_fake.root")).Data() );

  TCanvas *cfakmul = new TCanvas("cfakmul","Binary sensors comparison", 0, 60, 700, 700);
  hfake_mult->Draw();
  if( ifPreliminary) preliminary->Draw();
  for( Int_t i=0; i<nsensors; i++) {
    if( sensorList[i].display ) gfake_mult[i]->Draw("PEL");
  }
  l->Draw();
  gPad->SetLogy();
  gPad->SetGrid(1,1);
  if(ifSave) gPad->Print( (outputDir+TString("plot_fake_mult.eps")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_fake_mult.gif")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_fake_mult.root")).Data() );

  TCanvas *cfakthr = new TCanvas("cfakthr","Binary sensors comparison", 0, 80, 700, 700);
  hfake_thre->Draw();
  if( ifPreliminary) preliminary->Draw();
  for( Int_t i=0; i<nsensors; i++) {
    if( sensorList[i].display ) gfake_thre[i]->Draw("PEL");
  }
  l->Draw();
  gPad->SetLogy();
  gPad->SetGrid(1,1);
  if(ifSave) gPad->Print( (outputDir+TString("plot_fake_thre.eps")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_fake_thre.gif")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_fake_thre.root")).Data() );

  TCanvas *cfakfag = new TCanvas("cfakfag","Binary sensors comparison", 0, 100, 700, 700);
  hfake_fakegeom->Draw();
  if( ifPreliminary) preliminary->Draw();
  for( Int_t i=0; i<nsensors; i++) {
    if( sensorList[i].display ) gfake_fakegeom[i]->Draw("PEL");
  }
  l->Draw();
  gPad->SetLogx();
  gPad->SetLogy();
  gPad->SetGrid(1,1);
  if(ifSave) gPad->Print( (outputDir+TString("plot_fake_fakegeom.eps")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_fake_fakegeom.gif")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_fake_fakegeom.root")).Data() );

  TCanvas *cresthr = new TCanvas("cresthr","Binary sensors comparison", 0, 120, 700, 700);
  hreso_thre->Draw();
  if( ifPreliminary) preliminary->Draw();
  for( Int_t i=0; i<nsensors; i++) {
    if( sensorList[i].display ) greso_thre[i]->Draw("PEL");
  }
  l->Draw();
  gPad->SetGrid(1,1);
  if(ifSave) gPad->Print( (outputDir+TString("plot_reso_thre.eps")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_reso_thre.gif")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_reso_thre.root")).Data() );

  TCanvas *cresmul = new TCanvas("cresmul","Binary sensors comparison", 0, 140, 700, 700);
  hreso_mult->Draw();
  if( ifPreliminary) preliminary->Draw();
  for( Int_t i=0; i<nsensors; i++) {
    if( sensorList[i].display ) greso_mult[i]->Draw("PEL");
  }
  l->Draw();
  gPad->SetGrid(1,1);
  if(ifSave) gPad->Print( (outputDir+TString("plot_reso_mult.eps")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_reso_mult.gif")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_reso_mult.root")).Data() );


  TCanvas *cmulthr = new TCanvas("cmulthr","Binary sensors comparison", 0, 160, 700, 700);
  hmult_thre->Draw();
  if( ifPreliminary) preliminary->Draw();
  for(Int_t i=0; i<nsensors; i++) {
    if( sensorList[i].display ) gmult_thre[i]->Draw("PEL");
  }
  l->Draw();
  gPad->SetGrid(1,1);
  if(ifSave) gPad->Print( (outputDir+TString("plot_mult_thre.eps")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_mult_thre.gif")).Data() );
  if(ifSave) gPad->Print( (outputDir+TString("plot_mult_thre.root")).Data() );


  //------------------------------------
  // Summary plot per individualy sensor

  Char_t cname[20], ctitle[100];
  TCanvas *ca;
  TGraphErrors **geffi = new TGraphErrors*[maxNsensors];
  TGraphErrors **greso = new TGraphErrors*[maxNsensors];
  TGraphErrors **gfake = new TGraphErrors*[maxNsensors];
  TH2F **hframe = new TH2F*[maxNsensors];

  Int_t resolcolor = 2;
  Float_t a = (effimax - effimin)/(resolmax-resolmin);
  Float_t b = effimin-resolmin*a;

  Int_t fakecolor = 4;
  //fakemax *= 500; // for better display
  Float_t af = (effimax-effimin)/(log10(fakemax)-log10(fakemin));
  Float_t bf = effimin-log10(fakemin)*af;

  gStyle->SetPadRightMargin(0.35);
   gStyle->SetTitleOffset(0.8,"y");

  for( Int_t i=0; i<nsensors; i++ ) { // Loop on sensors

    if( !sensorList[i].display )  continue; // JB 2011/07/01

    cout << "Creating summary plot for sensor " << i << "." << endl;

    sprintf( cname, "ca%d", i);
    sprintf( ctitle, "%s performances", sensorList[i].longName);
    ca = new TCanvas( cname, ctitle, 100, i*20, 800, 550);
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

    TGaxis *axisresol = new TGaxis(sensorList[i].Thre[sensorList[i].nThresholds-1]+2,effimin,sensorList[i].Thre[sensorList[i].nThresholds-1]+2,effimax,resolmin,resolmax,510,"+L");
    axisresol->SetLineColor(resolcolor);
    axisresol->SetLabelColor(resolcolor);
    axisresol->SetTitleColor(resolcolor);
    axisresol->SetTitle("Resolution (#mum)");
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

    TGaxis *axisfake = new TGaxis(sensorList[i].Thre[sensorList[i].nThresholds-1]+3.5,effimin,sensorList[i].Thre[sensorList[i].nThresholds-1]+3.5,effimax,fakemin,fakemax,510,"+LG");
    axisfake->SetLineColor(fakecolor);
    axisfake->SetLabelColor(fakecolor);
    axisfake->SetTitleColor(fakecolor);
    //axisfake->SetTitleOffset(1.80);
    axisfake->SetTitle("Average fake hit rate/pixel/event");
    axisfake->SetTitleOffset(1.2);
    //axisfake->SetTitleSize(0.04);
    axisfake->Draw();

    sprintf( ctitle, "%s/perf_%s.gif", outputDir.Data(), sensorList[i].shortName);
    if(ifSave) gPad->Print(ctitle);
    sprintf( ctitle, "%s/perf_%s.eps", outputDir.Data(), sensorList[i].shortName);
    if(ifSave) gPad->Print(ctitle);
    sprintf( ctitle, "%s/perf_%s.root", outputDir.Data(), sensorList[i].shortName);
    if(ifSave) gPad->Print(ctitle);

  } // end loop on sensors

  //------------------------------------
  //superimpose summary plots of two sensors

  cout << "Creating plot with " << N_superimosed_sensors << " superimposed sensors." << endl;

  sprintf( cname, "ca_superimposed");
  sprintf( ctitle, "%s vs. %s", sensorList[superimposed_sensors[0]].longName,sensorList[superimposed_sensors[1]].longName);
  TCanvas* ca_superimposed = new TCanvas( cname, ctitle, 200, 0, 800, 550);
  ca_superimposed->SetGrid(1,1);

  sprintf( cname, "hframe_superimposed");
  sprintf( ctitle, "%s vs. %s", sensorList[superimposed_sensors[0]].longName,sensorList[superimposed_sensors[1]].longName);

  TH2F* hframe_superimposed = new TH2F(cname, ctitle, TMath::Max(sensorList[superimposed_sensors[0]].nThresholds,sensorList[superimposed_sensors[1]].nThresholds)+1, TMath::Min(sensorList[superimposed_sensors[0]].Thre[0],sensorList[superimposed_sensors[1]].Thre[0])-1, TMath::Max(sensorList[superimposed_sensors[0]].Thre[sensorList[superimposed_sensors[0]].nThresholds-1],sensorList[superimposed_sensors[1]].Thre[sensorList[superimposed_sensors[1]].nThresholds-1])+1, (Int_t)(effimax-effimin), effimin, effimax);
//   TH2F *hframe_superimposed = new TH2F(cname,ctitle,7,3,11,(Int_t)(effimax-effimin), effimin, effimax);
  hframe_superimposed->SetXTitle("Threshold (/ noise)");
  hframe_superimposed->SetYTitle("Efficiency (%)");
  hframe_superimposed->Draw();
  if( ifPreliminary) preliminary->Draw();
  TLegend* legend_superimposed=new TLegend(0.15,0.80,0.65,0.90, "");
  legend_superimposed->SetFillStyle(1);
  legend_superimposed->SetFillColor(0);
  legend_superimposed->SetBorderSize(1.);
  legend_superimposed->SetTextSize(.03);

  for (Int_t i=0;i<N_superimosed_sensors;i++){
    geffi[superimposed_sensors[i]]->SetLineStyle(i+1);
    geffi[superimposed_sensors[i]]->Draw("LP");
    greso[superimposed_sensors[i]]->SetLineStyle(i+1);
    greso[superimposed_sensors[i]]->Draw("LP");
    gfake[superimposed_sensors[i]]->SetLineStyle(i+1);
    gfake[superimposed_sensors[i]]->Draw("LP");
    legend_superimposed->AddEntry(geffi[superimposed_sensors[i]],sensorList[superimposed_sensors[i]].longName, "l");
  }

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

}
