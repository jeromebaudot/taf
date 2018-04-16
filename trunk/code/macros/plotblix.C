#define _FILE_OFFSET_BITS 64
#include "TMath.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TFile.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TApplication.h"
#include "TLegend.h"
#include <iostream>
#include "TTree.h"
//#include "DEvent.h"

void plotblix( ){

  // analyse  obtained with some x-ray source
  //  distribute #pixels/hit overall and in various image area
  //  display image with various cut on #pixels/hit
  //
  // JB 

  int cutneighbours = 70;
  Int_t cutLowSignal   =  300, cutHighSignal  =  400;
  Int_t cutLowSignal2  =   80, cutHighSignal2 =  140;
  Int_t cutLowSignal3  =  800, cutHighSignal3 = 2000;

  // For run 1005
//  Int_t cutROI1minX = -2500, cutROI1minY = -1800;
//  Int_t cutROI1maxX =  2500, cutROI1maxY =  1000;
//  Int_t cutROI2minX = -2500, cutROI2minY = -2500;
//  Int_t cutROI2maxX =  2500, cutROI2maxY = -2100;
//  Int_t cutROI3minX =  -900, cutROI3minY =  1300;
//  Int_t cutROI3maxX =     0, cutROI3maxY =  2200;
  
  // For run 1007
  Int_t cutROI1minX =   500, cutROI1minY = -1000;
  Int_t cutROI1maxX =  2400, cutROI1maxY =  2000;
  Int_t cutROI2minX = -2500, cutROI2minY = -2000;
  Int_t cutROI2maxX =  -900, cutROI2maxY =  2000;
  Int_t cutROI3minX =  -600, cutROI3minY =  -200;
  Int_t cutROI3maxX =  -200, cutROI3maxY =   500;
  
  Int_t maxCharge = 1000;
  Int_t maxSNR = 100;
  Float_t binMap = 5.; // um
  Float_t binCharge = 2.5; // uADC
  
  
  Bool_t logCharge = true;
  Bool_t logMap = true;
  
  Int_t nPixelsU = 512;
  Int_t nPixelsV = 512;
  Float_t pitchU = 10; // um
  Float_t pitchV = 10; // um
  
  Int_t plane = 1;
  
  Int_t centerXmin = 340, centerYmin = 440;
  Int_t centerXmax = 520, centerYmax = 600;
  Int_t strongsectorXmin = 500, strongsectorYmin =  50;
  Int_t strongsectorXmax = 700, strongsectorYmax = 200;
  Int_t weaksectorXmin = 760, weaksectorYmin = 400;
  Int_t weaksectorXmax = 940, weaksectorYmax = 600;
  
  Float_t lowScale = 0.5;
  Float_t highScale = 15;
  
  Char_t histotitle[300];
  
  sprintf( histotitle, "Pixel multiplicity, all map; # pixels");
  TH1F *multall = new TH1F("multall", histotitle, 51, -0.5, 50.5);
  multall->SetFillColor(2);
  
  sprintf( histotitle, "SNR neighbours vs seed; SNR seed; SNR neighbours");
  TH2F *snrneighbourseedall = new TH2F("snrneighbourseedall", histotitle, maxSNR/2,0,maxSNR, (maxSNR/2+10)/2,-10,maxSNR/2);

  sprintf( histotitle, "Signal neighbours vs seed; Signal seed (ADCu); Signal neighbours (ADCu)");
  TH2F *signeighbourseedall = new TH2F("signeighbourseedall", histotitle, maxCharge/binCharge,0,maxCharge, (maxCharge+100)/binCharge,-100,maxCharge);
  
  sprintf( histotitle, "Signal cluster vs seed; Signal seed (ADCu); Signal cluster (ADCu)");
  TH2F *sigclusterseedall = new TH2F("sigclusterseedall", histotitle, maxCharge/binCharge,0,maxCharge,maxCharge/binCharge,0,maxCharge);
  
  sprintf( histotitle, "Signal seed; Signal seed (ADCu)");
  TH1F *sigseedall = new TH1F("sigseedall", histotitle, maxCharge/binCharge,0,maxCharge);
  
  sprintf( histotitle, "Signal seed with no signal neighbours < %d; Signal seed (ADCu)", cutneighbours);
  TH1F *sigseedpeak = new TH1F("sigseedpeak", histotitle, maxCharge/binCharge,0,maxCharge);
  
  sprintf( histotitle, "Signal cluster; Signal cluster (ADCu)");
  TH1F *sigclusterall = new TH1F("sigclusterall", histotitle, maxCharge/binCharge,0,maxCharge);
  
  
  
  sprintf( histotitle, "Signal 2 highest pix vs seed; Signal seed (ADCu); Signal cluster (ADCu)");
  TH2F *sig2pixseedall = new TH2F("sig2pixseedall", histotitle, maxCharge/binCharge,0,maxCharge,maxCharge/binCharge,0,maxCharge);
  
  sprintf( histotitle, "Signal 3 highest pix vs seed; Signal seed (ADCu); Signal cluster (ADCu)");
  TH2F *sig3pixseedall = new TH2F("sig3pixseedall", histotitle, maxCharge/binCharge,0,maxCharge,maxCharge/binCharge,0,maxCharge);
  
  sprintf( histotitle, "Signal 4 highest pix vs seed; Signal seed (ADCu); Signal cluster (ADCu)");
  TH2F *sig4pixseedall = new TH2F("sig4pixseedall", histotitle, maxCharge/binCharge,0,maxCharge,maxCharge/binCharge,0,maxCharge);
  
  sprintf( histotitle, "Signal 5 highest pix vs seed; Signal seed (ADCu); Signal cluster (ADCu)");
  TH2F *sig5pixseedall = new TH2F("sig5pixseedall", histotitle, maxCharge/binCharge,0,maxCharge,maxCharge/binCharge,0,maxCharge);
  
  sprintf( histotitle, "Signal 6 highest pix vs seed; Signal seed (ADCu); Signal cluster (ADCu)");
  TH2F *sig6pixseedall = new TH2F("sig6pixseedall", histotitle, maxCharge/binCharge,0,maxCharge,maxCharge/binCharge,0,maxCharge);
  
  sprintf( histotitle, "Signal 7 highest pix vs seed; Signal seed (ADCu); Signal cluster (ADCu)");
  TH2F *sig7pixseedall = new TH2F("sig7pixseedall", histotitle, maxCharge/binCharge,0,maxCharge,maxCharge/binCharge,0,maxCharge);
  
  sprintf( histotitle, "Signal 8 highest pix vs seed; Signal seed (ADCu); Signal cluster (ADCu)");
  TH2F *sig8pixseedall = new TH2F("sig8pixseedall", histotitle, maxCharge/binCharge,0,maxCharge,maxCharge/binCharge,0,maxCharge);
  
  sprintf( histotitle, "Signal 9 highest pix vs seed; Signal seed (ADCu); Signal cluster (ADCu)");
  TH2F *sig9pixseedall = new TH2F("sig9pixseedall", histotitle, maxCharge/binCharge,0,maxCharge,maxCharge/binCharge,0,maxCharge);
  
  sprintf( histotitle, "Signal 10 highest pix vs seed; Signal seed (ADCu); Signal cluster (ADCu)");
  TH2F *sig10pixseedall = new TH2F("sig10pixseedall", histotitle, maxCharge/binCharge,0,maxCharge,maxCharge/binCharge,0,maxCharge);
  
  sprintf( histotitle, "Signal 11 highest pix vs seed; Signal seed (ADCu); Signal cluster (ADCu)");
  TH2F *sig11pixseedall = new TH2F("sig11pixseedall", histotitle, maxCharge/binCharge,0,maxCharge,maxCharge/binCharge,0,maxCharge);
  
  sprintf( histotitle, "Signal 12 highest pix vs seed; Signal seed (ADCu); Signal cluster (ADCu)");
  TH2F *sig12pixseedall = new TH2F("sig12pixseedall", histotitle, maxCharge/binCharge,0,maxCharge,maxCharge/binCharge,0,maxCharge);
  
  sprintf( histotitle, "Signal 13 highest pix vs seed; Signal seed (ADCu); Signal cluster (ADCu)");
  TH2F *sig13pixseedall = new TH2F("sig13pixseedall", histotitle, maxCharge/binCharge,0,maxCharge,maxCharge/binCharge,0,maxCharge);
  
  sprintf( histotitle, "Signal 14 highest pix vs seed; Signal seed (ADCu); Signal cluster (ADCu)");
  TH2F *sig14pixseedall = new TH2F("sig14pixseedall", histotitle, maxCharge/binCharge,0,maxCharge,maxCharge/binCharge,0,maxCharge);
  
  sprintf( histotitle, "Signal 15 highest pix vs seed; Signal seed (ADCu); Signal cluster (ADCu)");
  TH2F *sig15pixseedall = new TH2F("sig15pixseedall", histotitle, maxCharge/binCharge,0,maxCharge,maxCharge/binCharge,0,maxCharge);
  
  sprintf( histotitle, "Signal 16 highest pix vs seed; Signal seed (ADCu); Signal cluster (ADCu)");
  TH2F *sig16pixseedall = new TH2F("sig16pixseedall", histotitle, maxCharge/binCharge,0,maxCharge,maxCharge/binCharge,0,maxCharge);
  
  sprintf( histotitle, "Signal 17 highest pix vs seed; Signal seed (ADCu); Signal cluster (ADCu)");
  TH2F *sig17pixseedall = new TH2F("sig17pixseedall", histotitle, maxCharge/binCharge,0,maxCharge,maxCharge/binCharge,0,maxCharge);
  
  sprintf( histotitle, "Signal 18 highest pix vs seed; Signal seed (ADCu); Signal cluster (ADCu)");
  TH2F *sig18pixseedall = new TH2F("sig18pixseedall", histotitle, maxCharge/binCharge,0,maxCharge,maxCharge/binCharge,0,maxCharge);
  
  sprintf( histotitle, "Signal 19 highest pix vs seed; Signal seed (ADCu); Signal cluster (ADCu)");
  TH2F *sig19pixseedall = new TH2F("sig19pixseedall", histotitle, maxCharge/binCharge,0,maxCharge,maxCharge/binCharge,0,maxCharge);
  
  
  
  sprintf( histotitle, "Map of hits; horizonthal position (#mum); vertical position (#mum)");
  TH2F *mapall = new TH2F("mapall", histotitle, nPixelsU*pitchU/binMap,-nPixelsU*pitchU/2,nPixelsU*pitchU/2, nPixelsV*pitchV/binMap,-nPixelsV*pitchV/2,nPixelsV*pitchV/2);
  
  sprintf( histotitle, "Map of hits for signal in [%d, %d]; horizonthal position (#mum); vertical position (#mum)", cutLowSignal, cutHighSignal);
  TH2F *mapsig = new TH2F("mapsig", histotitle, nPixelsU*pitchU/binMap,-nPixelsU*pitchU/2,nPixelsU*pitchU/2, nPixelsV*pitchV/binMap,-nPixelsV*pitchV/2,nPixelsV*pitchV/2);
  
  sprintf( histotitle, "Map of hits for signal in [%d, %d]; horizonthal position (#mum); vertical position (#mum)",cutLowSignal2, cutHighSignal2);
  TH2F *mapsig2 = new TH2F("mapsig2", histotitle, nPixelsU*pitchU/binMap,-nPixelsU*pitchU/2,nPixelsU*pitchU/2, nPixelsV*pitchV/binMap,-nPixelsV*pitchV/2,nPixelsV*pitchV/2);
  
  sprintf( histotitle, "Map of hits for signal in [%d, %d]; horizonthal position (#mum); vertical position (#mum)",cutLowSignal3, cutHighSignal3);
  TH2F *mapsig3 = new TH2F("mapsig3", histotitle, nPixelsU*pitchU/binMap,-nPixelsU*pitchU/2,nPixelsU*pitchU/2, nPixelsV*pitchV/binMap,-nPixelsV*pitchV/2,nPixelsV*pitchV/2);
  
  
  
  sprintf( histotitle, "Signal cluster vs seed for ROI-1; Signal seed (ADCu); Signal cluster (ADCu)");
  TH2F *sigclusterseedsroi1 = new TH2F("sigclusterseedsroi1", histotitle, maxCharge/binCharge,0,maxCharge,maxCharge/binCharge,0,maxCharge);
  
  sprintf( histotitle, "Signal cluster vs seed for ROI-2; Signal seed (ADCu); Signal cluster (ADCu)");
  TH2F *sigclusterseedsroi2 = new TH2F("sigclusterseedsroi2", histotitle, maxCharge/binCharge,0,maxCharge,maxCharge/binCharge,0,maxCharge);
  
  sprintf( histotitle, "Signal cluster vs seed for ROI-3; Signal seed (ADCu); Signal cluster (ADCu)");
  TH2F *sigclusterseedsroi3 = new TH2F("sigclusterseedsroi3", histotitle, maxCharge/binCharge,0,maxCharge,maxCharge/binCharge,0,maxCharge);
  

  
  
//  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);

  Char_t cut[200];
  Char_t plot[200];
  Int_t counts;
  
  // Display hit maps
  TCanvas *cImage = new TCanvas("cImage","Hit maps", 900, 700);
  cImage->Divide(2,2);
  
  cImage->cd(1);
  sprintf( plot, "Hv:Hu>>mapall");
  sprintf( cut, "Hpk==%d", plane);
  counts = T->Draw( plot, cut, "colz");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
//  hitmapsmall->SetMaximum(50);
  if( logMap ) gPad->SetLogz();

  cImage->cd(2);
  sprintf( plot, "Hv:Hu>>mapsig");
  sprintf( cut, "Hpk==%d && %d<Hqc && Hqc<%d", plane, cutLowSignal, cutHighSignal);
  counts = T->Draw( plot, cut, "colz");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  if( logMap ) gPad->SetLogz();

  cImage->cd(3);
  sprintf( plot, "Hv:Hu>>mapsig2");
  sprintf( cut, "Hpk==%d && %d<Hqc && Hqc<%d", plane, cutLowSignal2, cutHighSignal2);
  counts = T->Draw( plot, cut, "colz");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  if( logMap ) gPad->SetLogz();
  
  cImage->cd(4);
  sprintf( plot, "Hv:Hu>>mapsig3");
  sprintf( cut, "Hpk==%d && %d<Hqc && Hqc<%d", plane, cutLowSignal3, cutHighSignal3);
  counts = T->Draw( plot, cut, "colz");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  if( logMap ) gPad->SetLogz();
  

  
  // Display hit properties
  TCanvas *cproperties = new TCanvas("cproperties","Hit properties", 100, 0, 900, 700);
  cproperties->Divide(3,3);
  
  cproperties->cd(1);
  sprintf( plot, "HSNneighbour:HsN>>snrneighbourseedall");
  sprintf( cut, "Hpk==%d", plane);
  counts = T->Draw( plot, cut, "colz");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  gPad->SetGrid(1,1);
  if( logCharge ) gPad->SetLogz();

  cproperties->cd(2);
  sprintf( plot, "Hqc-Hq0:Hq0>>signeighbourseedall");
  sprintf( cut, "Hpk==%d", plane);
  counts = T->Draw( plot, cut, "colz");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  gPad->SetGrid(1,1);
  if( logCharge ) gPad->SetLogz();

  cproperties->cd(3);
  sprintf( plot, "Hqc:Hq0>>sigclusterseedall");
  sprintf( cut, "Hpk==%d", plane);
  counts = T->Draw( plot, cut, "colz");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  gPad->SetGrid(1,1);
  if( logCharge ) gPad->SetLogz();
  
  cproperties->cd(4);
  sprintf( plot, "Hq0>>sigseedall");
  sprintf( cut, "Hpk==%d", plane);
  counts = T->Draw( plot, cut, "");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  gPad->SetGrid(1,1);
  
  cproperties->cd(5);
  sprintf( plot, "Hq0>>sigseedpeak");
  sprintf( cut, "Hpk==%d && Hqc-Hq0<%d", plane, cutneighbours);
  counts = T->Draw( plot, cut, "");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  gPad->SetGrid(1,1);
  
  cproperties->cd(6);
  sprintf( plot, "Hqc>>sigclusterall");
  sprintf( cut, "Hpk==%d", plane);
  counts = T->Draw( plot, cut, "");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  gPad->SetGrid(1,1);

  cproperties->cd(7);
  sprintf( plot, "HNNS>>multall");
  sprintf( cut, "Hpk==%d", plane);
  counts = T->Draw( plot, cut, "");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  gPad->SetGrid(1,1);

  cproperties->cd(8);
  T->Draw("HhN:Hevt","Hhk==1 && Hevt<60000 && HhN<10","colz");
  

  // Display various hit charge computations
  TCanvas *ccharges = new TCanvas("ccharges","Charges", 200, 0, 900, 700);
  ccharges->Divide(3,3);
  
  ccharges->cd(1);
  sprintf( plot, "Hq0+Hq1:Hq0>>sig2pixseedall");
  sprintf( cut, "Hpk==%d", plane);
  counts = T->Draw( plot, cut, "colz");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  gPad->SetGrid(1,1);
  if( logCharge ) gPad->SetLogz();
  
  ccharges->cd(2);
  sprintf( plot, "Hq0+Hq1+Hq2:Hq0>>sig3pixseedall");
  sprintf( cut, "Hpk==%d", plane);
  counts = T->Draw( plot, cut, "colz");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  gPad->SetGrid(1,1);
  if( logCharge ) gPad->SetLogz();
  
  ccharges->cd(3);
  sprintf( plot, "Hq0+Hq1+Hq2+Hq3:Hq0>>sig4pixseedall");
  sprintf( cut, "Hpk==%d", plane);
  counts = T->Draw( plot, cut, "colz");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  gPad->SetGrid(1,1);
  if( logCharge ) gPad->SetLogz();
  
  ccharges->cd(4);
  sprintf( plot, "Hq0+Hq1+Hq2+Hq3+Hq4:Hq0>>sig5pixseedall");
  sprintf( cut, "Hpk==%d", plane);
  counts = T->Draw( plot, cut, "colz");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  gPad->SetGrid(1,1);
  if( logCharge ) gPad->SetLogz();
  
  ccharges->cd(5);
  sprintf( plot, "Hq0+Hq1+Hq2+Hq3+Hq4+Hq5:Hq0>>sig6pixseedall");
  sprintf( cut, "Hpk==%d", plane);
  counts = T->Draw( plot, cut, "colz");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  gPad->SetGrid(1,1);
  if( logCharge ) gPad->SetLogz();
  
  ccharges->cd(6);
  sprintf( plot, "Hq0+Hq1+Hq2+Hq3+Hq4+Hq5+Hq6:Hq0>>sig7pixseedall");
  sprintf( cut, "Hpk==%d", plane);
  counts = T->Draw( plot, cut, "colz");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  gPad->SetGrid(1,1);
  if( logCharge ) gPad->SetLogz();
  
  ccharges->cd(7);
  sprintf( plot, "Hq0+Hq1+Hq2+Hq3+Hq4+Hq5+Hq6+Hq7:Hq0>>sig8pixseedall");
  sprintf( cut, "Hpk==%d", plane);
  counts = T->Draw( plot, cut, "colz");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  gPad->SetGrid(1,1);
  if( logCharge ) gPad->SetLogz();
  
  ccharges->cd(8);
  sprintf( plot, "Hq0+Hq1+Hq2+Hq3+Hq4+Hq5+Hq6+Hq7+Hq8:Hq0>>sig9pixseedall");
  sprintf( cut, "Hpk==%d", plane);
  counts = T->Draw( plot, cut, "colz");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  gPad->SetGrid(1,1);
  if( logCharge ) gPad->SetLogz();
  
  ccharges->cd(9);
//  sprintf( plot, "Hq0+Hq1+Hq2+Hq3+Hq4+Hq5+Hq6+Hq7+Hq8+HqM[9]:Hq0>>sig10pixseedall");
//  sprintf( cut, "Hpk==%d", plane);
//  counts = T->Draw( plot, cut, "colz");
//  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  sigclusterseedall->Draw("colz");
  gPad->SetGrid(1,1);
  if( logCharge ) gPad->SetLogz();
  
  
//  TCanvas *ccharges2 = new TCanvas("ccharges2","More Charges", 300, 0, 900, 700);
//  ccharges2->Divide(3,3);
//  
//  ccharges2->cd(1);
//  sprintf( plot, "Hq0+Hq1+Hq2+Hq3+Hq4+Hq5+Hq6+Hq7+Hq8+HqM[9]+HqM[10]:Hq0>>sig11pixseedall");
//  sprintf( cut, "Hpk==%d", plane);
//  counts = T->Draw( plot, cut, "colz");
//  printf("Counts = %d for %s and %s\n", counts, plot, cut);
//  gPad->SetGrid(1,1);
//  
//  ccharges2->cd(2);
//  sprintf( plot, "Hq0+Hq1+Hq2+Hq3+Hq4+Hq5+Hq6+Hq7+Hq8+HqM[9]+HqM[10]+HqM[11]:Hq0>>sig12pixseedall");
//  sprintf( cut, "Hpk==%d", plane);
//  counts = T->Draw( plot, cut, "colz");
//  printf("Counts = %d for %s and %s\n", counts, plot, cut);
//  gPad->SetGrid(1,1);
//  if( logCharge ) gPad->SetLogz();
//  
//  ccharges2->cd(3);
//  sprintf( plot, "Hq0+Hq1+Hq2+Hq3+Hq4+Hq5+Hq6+Hq7+Hq8+HqM[9]+HqM[10]+HqM[11]+HqM[12]:Hq0>>sig13pixseedall");
//  sprintf( cut, "Hpk==%d", plane);
//  counts = T->Draw( plot, cut, "colz");
//  printf("Counts = %d for %s and %s\n", counts, plot, cut);
//  gPad->SetGrid(1,1);
//  if( logCharge ) gPad->SetLogz();
//  
//  ccharges2->cd(4);
//  sprintf( plot, "Hq0+Hq1+Hq2+Hq3+Hq4+Hq5+Hq6+Hq7+Hq8+HqM[9]+HqM[10]+HqM[11]+HqM[12]+HqM[13]:Hq0>>sig14pixseedall");
//  sprintf( cut, "Hpk==%d", plane);
//  counts = T->Draw( plot, cut, "colz");
//  printf("Counts = %d for %s and %s\n", counts, plot, cut);
//  gPad->SetGrid(1,1);
//  if( logCharge ) gPad->SetLogz();
//  
//  ccharges2->cd(5);
//  sprintf( plot, "Hq0+Hq1+Hq2+Hq3+Hq4+Hq5+Hq6+Hq7+Hq8+HqM[9]+HqM[10]+HqM[11]+HqM[12]+HqM[13]+HqM[14]:Hq0>>sig15pixseedall");
//  sprintf( cut, "Hpk==%d", plane);
//  counts = T->Draw( plot, cut, "colz");
//  printf("Counts = %d for %s and %s\n", counts, plot, cut);
//  gPad->SetGrid(1,1);
//  if( logCharge ) gPad->SetLogz();
//  
//  ccharges2->cd(6);
//  sprintf( plot, "Hq0+Hq1+Hq2+Hq3+Hq4+Hq5+Hq6+Hq7+Hq8+HqM[9]+HqM[10]+HqM[11]+HqM[12]+HqM[13]+HqM[14]+HqM[15]:Hq0>>sig16pixseedall");
//  sprintf( cut, "Hpk==%d", plane);
//  counts = T->Draw( plot, cut, "colz");
//  printf("Counts = %d for %s and %s\n", counts, plot, cut);
//  gPad->SetGrid(1,1);
//  if( logCharge ) gPad->SetLogz();
//  
//  ccharges2->cd(7);
//  sprintf( plot, "Hq0+Hq1+Hq2+Hq3+Hq4+Hq5+Hq6+Hq7+Hq8+HqM[9]+HqM[10]+HqM[11]+HqM[12]+HqM[13]+HqM[14]+HqM[15]+HqM[16]:Hq0>>sig17pixseedall");
//  sprintf( cut, "Hpk==%d", plane);
//  counts = T->Draw( plot, cut, "colz");
//  printf("Counts = %d for %s and %s\n", counts, plot, cut);
//  gPad->SetGrid(1,1);
//  if( logCharge ) gPad->SetLogz();
//  
//  ccharges2->cd(8);
//  sprintf( plot, "Hq0+Hq1+Hq2+Hq3+Hq4+Hq5+Hq6+Hq7+Hq8+HqM[9]+HqM[10]+HqM[11]+HqM[12]+HqM[13]+HqM[14]+HqM[15]+HqM[16]+HqM[17]:Hq0>>sig18pixseedall");
//  sprintf( cut, "Hpk==%d", plane);
//  counts = T->Draw( plot, cut, "colz");
//  printf("Counts = %d for %s and %s\n", counts, plot, cut);
//  gPad->SetGrid(1,1);
//  if( logCharge ) gPad->SetLogz();
//  
//  ccharges2->cd(9);
//  sprintf( plot, "Hq0+Hq1+Hq2+Hq3+Hq4+Hq5+Hq6+Hq7+Hq8+HqM[9]+HqM[10]+HqM[11]+HqM[12]+HqM[13]+HqM[14]+HqM[15]+HqM[16]+HqM[17]+HqM[18]:Hq0>>sig19pixseedall");
//  sprintf( cut, "Hpk==%d", plane);
//  counts = T->Draw( plot, cut, "colz");
//  printf("Counts = %d for %s and %s\n", counts, plot, cut);
//  gPad->SetGrid(1,1);
//  if( logCharge ) gPad->SetLogz();
  
  

  // Display Signal in ROIs
  TCanvas *croi = new TCanvas("croi","Signal in ROIs", 400, 0, 900, 700);
  croi->Divide(2,2);
  
  croi->cd(1);
  sprintf( plot, "Hqc:Hq0>>sigclusterseedsroi1");
  sprintf( cut, "Hpk==%d && %d<Hu && Hu<%d && %d<Hv && Hv<%d", plane, cutROI1minX, cutROI1maxX, cutROI1minY, cutROI1maxY);
  counts = T->Draw( plot, cut, "colz");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  gPad->SetGrid(1,1);
  if( logCharge ) gPad->SetLogz();
  
  croi->cd(2);
  sprintf( plot, "Hqc:Hq0>>sigclusterseedsroi2");
  sprintf( cut, "Hpk==%d && %d<Hu && Hu<%d && %d<Hv && Hv<%d", plane, cutROI2minX, cutROI2maxX, cutROI2minY, cutROI2maxY);
  counts = T->Draw( plot, cut, "colz");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  gPad->SetGrid(1,1);
  if( logCharge ) gPad->SetLogz();
  
  croi->cd(3);
  sprintf( plot, "Hqc:Hq0>>sigclusterseedsroi3");
  sprintf( cut, "Hpk==%d && %d<Hu && Hu<%d && %d<Hv && Hv<%d", plane, cutROI3minX, cutROI3maxX, cutROI3minY, cutROI3maxY);
  counts = T->Draw( plot, cut, "colz");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  gPad->SetGrid(1,1);
  if( logCharge ) gPad->SetLogz();
  
  croi->cd(4);
  mapall->Draw("colz");
  if( logMap ) gPad->SetLogz();
  
  TBox *roi1 = new TBox( cutROI1minX, cutROI1minY, cutROI1maxX, cutROI1maxY );
  roi1->SetFillStyle(0);
  roi1->SetLineStyle(1);
  roi1->SetLineWidth(6);
  roi1->SetLineColor(1);
  roi1->Draw();
  TText *text1 = new TText( (cutROI1minX+cutROI1maxX)/2, (cutROI1minY+cutROI1maxY)/2, "ROI-1");
  text1->SetTextColor(1);
  text1->Draw();
  TBox *roi2 = new TBox( cutROI2minX, cutROI2minY, cutROI2maxX, cutROI2maxY );
  roi2->SetFillStyle(0);
  roi2->SetLineStyle(1);
  roi2->SetLineWidth(6);
  roi2->SetLineColor(4);
  roi2->Draw();
  TText *text2 = new TText( (cutROI2minX+cutROI2maxX)/2, (cutROI2minY+cutROI2maxY)/2, "ROI-2");
  text2->SetTextColor(4);
  text2->Draw();
  TBox *roi3 = new TBox( cutROI3minX, cutROI3minY, cutROI3maxX, cutROI3maxY );
  roi3->SetFillStyle(0);
  roi3->SetLineStyle(1);
  roi3->SetLineWidth(6);
  roi3->SetLineColor(6);
  roi3->Draw();
  TText *text3 = new TText( (cutROI3minX+cutROI3maxX)/2, (cutROI3minY+cutROI3maxY)/2, "ROI-3");
  text3->SetTextColor(6);
  text3->Draw();
  


  
  // Save to file
  TFile fRoot("plotblix.root","RECREATE");
  cImage->Write();
  cproperties->Write();
  ccharges->Write();
  multall->Write();
  croi->Write();
  snrneighbourseedall->Write();
  signeighbourseedall->Write();
  sigclusterseedall->Write();
  sigseedall->Write();
  sigseedpeak->Write();
  sigclusterall->Write();
  sig2pixseedall->Write();
  sig3pixseedall->Write();
  sig4pixseedall->Write();
  sig5pixseedall->Write();
  sig6pixseedall->Write();
  sig7pixseedall->Write();
  sig8pixseedall->Write();
  sig9pixseedall->Write();
  sigclusterseedsroi1->Write();
  sigclusterseedsroi2->Write();
  sigclusterseedsroi3->Write();
  mapall->Write();
  mapsig->Write();
  mapsig2->Write();
  mapsig3->Write();
  fRoot.Close();
  
  cout << endl << "==> Histos & canvas saved in " << fRoot.GetName() << endl << endl;
  
}
