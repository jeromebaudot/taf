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
#include "DEvent.h"

void xraygun( Int_t plane=3, Int_t smallPixelCut=5, Int_t largePixelCut=9, Int_t binning=2 ){

  // analyse radiographic image obtained with some x-ray source
  //  distribute #pixels/hit overall and in various image area
  //  display image with various cut on #pixels/hit


  Int_t nPixelsU = 960;
  Int_t nPixelsV = 928;

  Int_t centerXmin = 340, centerYmin = 440;
  Int_t centerXmax = 520, centerYmax = 600;
  Int_t strongsectorXmin = 500, strongsectorYmin =  50;
  Int_t strongsectorXmax = 700, strongsectorYmax = 200;
  Int_t weaksectorXmin = 760, weaksectorYmin = 400;
  Int_t weaksectorXmax = 940, weaksectorYmax = 600;
  
  Float_t lowScale = 0.5;
  Float_t highScale = 15;
  
  Char_t histotitle[300] = "hit map all hits; column; row";
  TH2F *hitmap = new TH2F("hitmap", histotitle, nPixelsU/binning,0,nPixelsU, nPixelsV/binning,0,nPixelsV);
  hitmap->SetStats(0);
  sprintf( histotitle, "hit map with hits <= %d pixels; column; row", smallPixelCut);
  TH2F *hitmapsmall = new TH2F("hitmapsmall", histotitle, nPixelsU/binning,0,nPixelsU, nPixelsV/binning,0,nPixelsV);
  hitmapsmall->SetStats(0);
  sprintf( histotitle, "hit map with hits %d-%d pixels; column; row", smallPixelCut, largePixelCut);
  TH2F *hitmapmedium = new TH2F("hitmapmedium", histotitle, nPixelsU/binning,0,nPixelsU, nPixelsV/binning,0,nPixelsV);
  hitmapmedium->SetStats(0);
  sprintf( histotitle, "hit map with hits >= %d pixels; column; row", smallPixelCut);
  TH2F *hitmaplarge = new TH2F("hitmaplarge", histotitle, nPixelsU/binning,0,nPixelsU, nPixelsV/binning,0,nPixelsV);
  hitmaplarge->SetStats(0);
  sprintf( histotitle, "hit map with hits >= %d pixels; column; row", largePixelCut);
  TH2F *hitmapverylarge = new TH2F("hitmapverylarge", histotitle, nPixelsU/binning,0,nPixelsU, nPixelsV/binning,0,nPixelsV);
  hitmapverylarge->SetStats(0);

  sprintf( histotitle, "Pixel multiplicity, all map; # pixels");
  TH1F *multall = new TH1F("multall", histotitle, 20, -0.5, 19.5);
  multall->SetFillColor(2);
  sprintf( histotitle, "Pixel multiplicity, IQI center; # pixels");
  TH1F *mulcenter = new TH1F("multcenter", histotitle, 20, -0.5, 19.5);
  multcenter->SetFillColor(5);
  sprintf( histotitle, "Pixel multiplicity, IQI strong sector; # pixels");
  TH1F *multstrongsector = new TH1F("multstrongsector", histotitle, 20, -0.5, 19.5);
  multstrongsector->SetFillColor(3);
  sprintf( histotitle, "Pixel multiplicity, IQI weak sector; # pixels");
  TH1F *multweaksector = new TH1F("multweaksector", histotitle, 20, -0.5, 19.5);
  multweaksector->SetFillColor(3);

  TBox *bIQIcenter = new TBox( centerXmin, centerYmin, centerXmax, centerYmax );
  bIQIcenter->SetFillStyle(0);
  bIQIcenter->SetLineStyle(1);
  bIQIcenter->SetLineWidth(4);
  bIQIcenter->SetLineColor(1);
  TBox *bIQIstrongsector = new TBox( strongsectorXmin, strongsectorYmin, strongsectorXmax, strongsectorYmax );
  bIQIstrongsector->SetFillStyle(0);
  bIQIstrongsector->SetLineStyle(1);
  bIQIstrongsector->SetLineWidth(4);
  bIQIstrongsector->SetLineColor(1);
  TBox *bIQIweaksector = new TBox( weaksectorXmin, weaksectorYmin, weaksectorXmax, weaksectorYmax );
  bIQIweaksector->SetFillStyle(0);
  bIQIweaksector->SetLineStyle(1);
  bIQIweaksector->SetLineWidth(4);
  bIQIweaksector->SetLineColor(1);

  
//  gStyle->SetOptTitle(0);
//  gStyle->SetOptStat(0);

  Char_t cut[200];
  Char_t plot[200];
  Int_t counts;
  
  // Display hit maps
  TCanvas *cImage = new TCanvas("cImage","Hit maps", 700, 700);
  cImage->Divide(2,2);
  
  cImage->cd(1);
  sprintf( plot, "Hsk/%d:Hsk%%%d>>hitmapsmall", nPixelsU, nPixelsU, nPixelsU, nPixelsU, nPixelsV, nPixelsV);
  sprintf( cut, "Hpk==%d && HNNS<=%d", plane, smallPixelCut);
  counts = T->Draw( plot, cut, "colz");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
//  hitmapsmall->SetMaximum(50);
  
  cImage->cd(2);
  sprintf( plot, "Hsk/%d:Hsk%%%d>>hitmaplarge", nPixelsU, nPixelsU, nPixelsU, nPixelsU, nPixelsV, nPixelsV);
  sprintf( cut, "Hpk==%d && %d<=HNNS", plane, smallPixelCut);
  counts = T->Draw( plot, cut, "colz");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  //  hitmap->SetMaximum(50);

  cImage->cd(3);
  sprintf( plot, "Hsk/%d:Hsk%%%d>>hitmapmedium", nPixelsU, nPixelsU, nPixelsU, nPixelsU, nPixelsV, nPixelsV);
  sprintf( cut, "Hpk==%d && %d<=HNNS && HNNS<=%d", plane, smallPixelCut, largePixelCut);
  counts = T->Draw( plot, cut, "colz");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
//  hitmaplarge->SetMaximum(30);
  
  cImage->cd(4);
  sprintf( plot, "Hsk/%d:Hsk%%%d>>hitmapverylarge", nPixelsU, nPixelsU, nPixelsU, nPixelsU, nPixelsV, nPixelsV);
  sprintf( cut, "Hpk==%d && %d<=HNNS", plane, largePixelCut);
  counts = T->Draw( plot, cut, "colz");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  

  
  // Display pixle multiplicity
  TCanvas *cMultiplicity = new TCanvas("cMultiplicity","Multiplicity distribution", 350, 10, 700, 700);
  cMultiplicity->Divide(2,2);
  
  cMultiplicity->cd(1);
  sprintf( plot, "HNNS>>multall");
  sprintf( cut, "Hpk==%d", plane);
  counts = T->Draw( plot, cut, "");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  gPad->SetGrid(1,1);
  
  cMultiplicity->cd(2);
  sprintf( plot, "HNNS>>multcenter");
  sprintf( cut, "Hpk==%d && %d<Hsk%%%d && Hsk%%%d<%d && %d<Hsk/%d && Hsk/%d<%d", plane, centerXmin, nPixelsU, nPixelsU, centerXmax, centerYmin, nPixelsU, nPixelsU, centerYmax);
  counts = T->Draw( plot, cut, "");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  gPad->SetGrid(1,1);

  cMultiplicity->cd(3);
  sprintf( plot, "HNNS>>multweaksector");
  sprintf( cut, "Hpk==%d && %d<Hsk%%%d && Hsk%%%d<%d && %d<Hsk/%d && Hsk/%d<%d", plane, weaksectorXmin, nPixelsU, nPixelsU, weaksectorXmax, weaksectorYmin, nPixelsU, nPixelsU, weaksectorYmax);
  counts = T->Draw( plot, cut, "");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  sprintf( plot, "HNNS>>multstrongsector");
  sprintf( cut, "Hpk==%d && %d<Hsk%%%d && Hsk%%%d<%d && %d<Hsk/%d && Hsk/%d<%d", plane, strongsectorXmin, nPixelsU, nPixelsU, strongsectorXmax, strongsectorYmin, nPixelsU, nPixelsU, strongsectorYmax);
  counts = T->Draw( plot, cut, "");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  gPad->SetGrid(1,1);
  
  
  cMultiplicity->cd(4);
  //  sprintf( plot, "Hsk/%d:Hsk%%%d>>(%d,0,%d,%d,0,%d)", nPixelsU, nPixelsU, nPixelsU, nPixelsU, nPixelsV, nPixelsV);
  sprintf( plot, "Hsk/%d:Hsk%%%d>>hitmap", nPixelsU, nPixelsU, nPixelsU, nPixelsU, nPixelsV, nPixelsV);
  sprintf( cut, "Hpk==%d", plane);
  counts = T->Draw( plot, cut, "colz");
  //  counts = T->Draw( "Hsk/960:Hsk%960>>(480,0,960,464,0,928)", "Hpk==3", "colz");
  printf("Counts = %d for %s and %s\n", counts, plot, cut);
  //  hitmap->SetMaximum(70);
  bIQIcenter->Draw("l");
  bIQIstrongsector->Draw("l");
  bIQIweaksector->Draw("l");

  
  // Here compute average hits per pixel for various multiplicity cuts
  //  and set the Z range of 2D hit map
  Float_t averagePerPixel = multstrongsector->GetEntries() / (centerXmax-centerXmin) / (centerYmax-centerYmin) * binning * binning;
  printf("Average counts in strongsector = %.0f\n", averagePerPixel);
  hitmap->SetMinimum(averagePerPixel*lowScale);
  averagePerPixel = multstrongsector->Integral(1, 1+smallPixelCut) / (centerXmax-centerXmin) / (centerYmax-centerYmin) * binning * binning;
  printf("Average counts in strongsector for multiplicity<=%d = %.0f\n", smallPixelCut, averagePerPixel);
  hitmapsmall->SetMinimum(averagePerPixel*lowScale);
  averagePerPixel = multstrongsector->Integral( 1+smallPixelCut, 1+largePixelCut) / (centerXmax-centerXmin) / (centerYmax-centerYmin) * binning * binning;
  printf("Average counts in strongsector for %d<=multiplicity<=%d = %.0f\n", smallPixelCut, largePixelCut, averagePerPixel);
  hitmapmedium->SetMinimum(averagePerPixel*lowScale);
  averagePerPixel = multstrongsector->Integral( 1+smallPixelCut, 20) / (centerXmax-centerXmin) / (centerYmax-centerYmin) * binning * binning;
  printf("Average counts in strongsector for %d<=multiplicity = %.0f\n", smallPixelCut, averagePerPixel);
  hitmaplarge->SetMinimum(averagePerPixel*lowScale);
  averagePerPixel = multstrongsector->Integral( 1+largePixelCut, 20) / (centerXmax-centerXmin) / (centerYmax-centerYmin) * binning * binning;
  printf("Average counts in strongsector for %d<=multiplicity = %.0f\n", largePixelCut, averagePerPixel);
  hitmapverylarge->SetMinimum(averagePerPixel*lowScale);

  averagePerPixel = multweaksector->GetEntries() / (centerXmax-centerXmin) / (centerYmax-centerYmin) * binning * binning;
  printf("Average counts in weaksector = %.0f\n", averagePerPixel);
  hitmap->SetMaximum(averagePerPixel*highScale);
  averagePerPixel = multweaksector->Integral(1, 1+smallPixelCut) / (centerXmax-centerXmin) / (centerYmax-centerYmin) * binning * binning;
  printf("Average counts in weaksector for multiplicity<=%d = %.0f\n", smallPixelCut, averagePerPixel);
  hitmapsmall->SetMaximum(averagePerPixel*highScale);
  averagePerPixel = multweaksector->Integral( 1+smallPixelCut, 1+largePixelCut) / (centerXmax-centerXmin) / (centerYmax-centerYmin) * binning * binning;
  printf("Average counts in weaksector for %d<=multiplicity<=%d = %.0f\n", smallPixelCut, largePixelCut, averagePerPixel);
  hitmapmedium->SetMaximum(averagePerPixel*highScale);
  averagePerPixel = multweaksector->Integral( 1+smallPixelCut, 20) / (centerXmax-centerXmin) / (centerYmax-centerYmin) * binning * binning;
  printf("Average counts in weaksector for %d<=multiplicity = %.0f\n", smallPixelCut, averagePerPixel);
  hitmaplarge->SetMaximum(averagePerPixel*highScale);
  averagePerPixel = multweaksector->Integral( 1+largePixelCut, 20) / (centerXmax-centerXmin) / (centerYmax-centerYmin) * binning * binning;
  printf("Average counts in weaksector for %d<=multiplicity = %.0f\n", largePixelCut, averagePerPixel);
  hitmapverylarge->SetMaximum(averagePerPixel*highScale);


  
  // Save to file
  TFile fRoot("xraygun.root","RECREATE");
  cImage->Write();
  cMultiplicity->Write();
  hitmap->Write();
  hitmapsmall->Write();
  hitmaplarge->Write();
  hitmapmedium->Write();
  hitmapverylarge->Write();
  multall->Write();
  multcenter->Write();
  multstrongsector->Write();
  multweaksector->Write();
  fRoot.Close();
  
}
