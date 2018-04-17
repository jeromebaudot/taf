//
//  fitCalibrationPeak.c
//  
//
//  Created by jerome Baudot on 5/22/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
void fitCalibrationPeak() {
  
  int qminfit = 290;
  int qmaxfit = 390;
  
  int qmin = 290;
  int qmax = 430;
  int binWidth = 4;
  
  int nBins = (qmax-qmin)/binWidth;
  qmax = qmin + nBins*binWidth;
  
  TFile *_file0 = TFile::Open("datDSF/run60802_01.root");

  TH1F *hcalib = new TH1F("hcalib","seed charge", nBins, qmin, qmax);
  hcalib->SetXTitle("seed pixel charge (ADCu)");
  hcalib->SetMarkerStyle(20);
  hcalib->SetMarkerSize(.8);

  // -----------
  // --- Use polynomial background
  TCanvas *cpol = new TCanvas("cpol","Calibration with polynomial bck");
  cpol->Draw();
  T->Draw("Hq0>>hcalib","Hq0>290 && Hq0<420", "PE");

  TF1* fp2 = new TF1("fp2","pol2", qminfit, qmaxfit);
  fp2->SetLineColor(4);
  hcalib->Fit("fp2","R");
  
  
  TF1* fgp = new TF1("fgp","pol2(0)+gaus(3)", qminfit, qmaxfit);
  fgp->SetParNames( "c0", "c1", "c2", "norm", "mean", "#sigma");
  fgp->SetParameter(0, fp2->GetParameter(0));
  fgp->SetParameter(1, fp2->GetParameter(1));
  fgp->SetParameter(2, fp2->GetParameter(2));
//  fgp->FixParameter(3,155);
  fgp->SetParameter(4,355);
  fgp->SetParLimits(4,340,370);
  fgp->SetParameter(5,10);
  fgp->SetParLimits(5,1,20);
  hcalib->Fit("fgp","R");

  hcalib->DrawClone("PE");
  fp2->SetParameter(0, fgp->GetParameter(0));
  fp2->SetParameter(1, fgp->GetParameter(1));
  fp2->SetParameter(2, fgp->GetParameter(2));
  fp2->Draw("same");

  
  // -----------
  // --- Use exponenatial background
  TCanvas *cexp = new TCanvas("cexp","Calibration with exponential bck");
  cexp->Draw();
  T->Draw("Hq0>>hcalib","Hq0>290 && Hq0<420", "PE");
  
//  qmaxfit = 410;

  TF1* fexp = new TF1("fexp","expo", qminfit, qmaxfit);
  fexp->SetLineColor(4);
  hcalib->Fit("fexp","R");
  
  TF1* fge = new TF1("fge","expo(0)+gaus(2)", qminfit, qmaxfit);
  fge->SetParNames( "c0", "c1", "norm", "mean", "#sigma");
  fge->SetParameter(0, fexp->GetParameter(0));
  fge->SetParameter(1, fexp->GetParameter(1));
  //  fge->FixParameter(3,155);
  fge->SetParameter(3,355);
  fge->SetParLimits(3,350,370);
  fge->SetParameter(4,10);
  fge->SetParLimits(4,10,20);
  hcalib->Fit("fge","R");
  
  hcalib->DrawClone("PE");
  fexp->SetParameter(0, fge->GetParameter(0));
  fexp->SetParameter(1, fge->GetParameter(1));
  fexp->Draw("same");

  
}