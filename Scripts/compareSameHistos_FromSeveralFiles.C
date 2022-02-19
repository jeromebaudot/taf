#include "Riostream.h"
#include "TLegend.h"
#include "math.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TStyle.h"
#include "TPad.h"

void compareSameHistos_FromSeveralFiles(){
  // Compare the same histo from different several files
  //
  // Usage:
  //  First edit the file (see below), and then simply execute it in ROOT
  //  - list the files in the fileName array
  //  - indicate a short description of each file in the legendText array
  //  - provide the directory where to find all the files in dirName
  //  - indicate histo ROOT name in histoName,
  //      this is the name that appears when you do .ls in ROOT
  //  - provide new title for histo, X axis and legend
  //


  Int_t nFiles = 3;
  const Int_t maxFiles = 30;

  Bool_t scaleHisto = kFALSE;

  Bool_t coloredHisto = kTRUE;
  Bool_t filledHisto = kFALSE;
  Bool_t dottedLine = kTRUE;
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  Char_t fileNames[maxFiles][100] ={
    "./3111/HitMap_run3111.root",
    "./3113/HitMap_run3113.root",   
    "./3115/HitMap_run3115.root"
  };
  Char_t legendText[maxFiles][50] ={
    "4sigma",
    "8sigma",
    "11.5sigma"
  };

  Char_t dirName[400] = "~/physics/taf/Results/";

  Char_t histoName[20] = "hhitpixpl1";
  Char_t histoTitle[300] = "Pixel multiplicity in hits of plane 1 - MIMOSA-28 - PCB-7";
  Char_t histoXTitle[100] = "# pixels";
  Char_t legendTitle[50] = "Threshold:";

  Short_t colors[maxFiles] = { 1, 2, 3, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 ,16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26};
  Short_t styles[maxFiles] = { 1};


  //-- No modification required below this line

  TFile **files  = new TFile*[maxFiles];
  TH1F **histos = new TH1F*[maxFiles];

  Char_t name[500];
  for( Short_t ifile=0; ifile<nFiles; ifile++) {
    sprintf( name, "%s/%s", dirName, fileNames[ifile]);
    cout << "getting file " << name << endl;
    files[ifile] = new TFile( name, "read");
    cout << "getting histo " << ifile << endl;
    histos[ifile] = (TH1F*)files[ifile]->Get(histoName);
    //histos[ifile]->Sumw2(); // cause problem with FillStyle !
    if (scaleHisto) {
      cout << "scaling with " << 1./histos[ifile]->Integral() << endl;
      histos[ifile]->Scale( 1./histos[ifile]->Integral() ); // normalize to 1
    }
    histos[ifile]->SetTitle(histoTitle);
    histos[ifile]->SetXTitle(histoXTitle);
    if( coloredHisto ) {
      histos[ifile]->SetLineColor(colors[ifile]);
    } else {
      histos[ifile]->SetLineColor(1);
    }
    histos[ifile]->SetLineStyle(styles[ifile]);
    histos[ifile]->SetLineWidth(4.5);
    histos[ifile]->SetFillColor(colors[ifile]);
    if( filledHisto ) {
      histos[ifile]->SetFillStyle(3005);
    } else {
      histos[ifile]->SetFillStyle(0);
      if( dottedLine && ifile>0 ) histos[ifile]->SetLineStyle(2);
    }
    histos[ifile]->SetStats(0);
  }

  cout << "now displaying" << endl;

  TCanvas *ccomp = new TCanvas("ccomp", "Comparison", 600,600);

  TLegend *l = new TLegend(0.55,0.65,0.90,0.90, legendTitle);
  l->SetFillStyle(0);
  l->SetBorderSize(0);
  l->SetTextSize(.04);

  // Set the maximum to highest histo
  Float_t max = 0.;
  for( Short_t ifile=0; ifile<nFiles; ifile++) {
    if( histos[ifile]->GetMaximum() > max ) {
      max = histos[ifile]->GetMaximum();
    }
  }
  histos[0]->SetMaximum( 4500);

  // Then draw all other histos on top
  for( Short_t ifile=0; ifile<nFiles; ifile++) {
    histos[ifile]->Draw(ifile?"same":"");
    if( filledHisto ) {
      l->AddEntry( histos[ifile], legendText[ifile], "f");
    } else {
      l->AddEntry( histos[ifile], legendText[ifile], "l");
    }
    cout << "histo " << ifile << " drawn." << endl;
  }
  cout << "doing legend" << endl;
  l->Draw();

}
