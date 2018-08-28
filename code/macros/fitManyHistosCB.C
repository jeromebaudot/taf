//
// This macros fits the individual seed charge distributions
//  and
//

#include "Riostream.h"
#include "TLegend.h"
#include "math.h" 
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TStyle.h"
#include "TString.h"

//*****************************************************************************
double CBfunction(double *x, double *par) {
  // Crystal ball function (https://en.wikipedia.org/wiki/Crystal_Ball_function)
  
  double mean     = par[0];
  double sigma    = par[1];
  double alpha    = par[2];
  double n        = par[3];
  double norm     = par[4];
  double absalpha = TMath::Abs(alpha);
  double frac     = (x[0]-mean)/sigma;
  double A        = pow(n/absalpha,n)*exp(-pow(absalpha,2)/2);
  double B        = (n/absalpha)-absalpha;
  
  double C        = sigma*(n/absalpha)*(1./(n-1))*exp(-0.5*pow(absalpha,2));
  double D        = sigma*sqrt(0.5*TMath::Pi())*(TMath::Erf(absalpha/sqrt(2)) + 1);
  double N        = norm/(C+D);
  
  double result = 0.0;
  if(frac > -alpha) result = exp(-0.5*pow(frac,2));
  else              result = A*pow(B-frac,-n);
  result *= N;
  
  return result;
  
}


//*****************************************************************************
void fitManyHistosCB( double min=850, double max=960, char *filename="Results/4182420/PixelSpectrum_run4182420_pixel128.root"   ){
  // Fit many histos from the same file with the same function
  //
  // Usage: 
  //
  
  
  const int nHistos = 768;
  char histoName[50] = "hhitseedq";

  
  // Initialisation
  TFile *inputfile = new TFile( filename, "read");;
  TH1F *histo, *hall;
  double *means = new double[nHistos];
  double *sigmas = new double[nHistos];
  TF1 *ffit = new TF1("ffit",CBfunction, min, max, 5);
  ffit->SetParameters( (max+min)/2, (max-min)/4, 1., 1., 1.);
  ffit->SetParNames( "mean", "#sigma", "#alpha", "n", "norm");
  TH1F *hmeans = new TH1F("hmeans", "Distribution of peak positions; peak position (ADCu)", 4*(max*1.3-min*.7), min*.7, max*1.3);
  TH1F *hsigmas = new TH1F("hsigmas", "Distribution of std deviations; std deviation (ADCu)", 100, 0, 50);
  TH2F *hpixelgain = new TH2F("hpixelgain", "Map of individual pixel gain; row index; column index", 128, 1, 129, 8, 1, 9);

  
  // Get each histos from the file and fit to extract the relevant parameters
  for (int ih=0; ih<nHistos; ih++) {
    cout << "getting histo nb " << ih;
    histo = (TH1F*)inputfile->Get(TString::Format("%s%d",histoName,ih));
    cout << " with " << histo->GetEntries() << " entries" << endl;
    ffit->SetRange( min, max);
    ffit->SetParameters( (max+min)/2, (max-min)/4, 1., 1., 1.);
    ffit->SetParLimits( 0, min, max);
    ffit->SetParLimits( 1, 0, max-min);
    ffit->SetParLimits( 2, 0.5, 1.5);
    ffit->SetParLimits( 3, 0.1, 2.);
    ffit->SetParLimits( 4, 0., 1.e8);
    histo->Fit( ffit, "QR");
    means[ih] = ffit->GetParameter(0);
    sigmas[ih] = ffit->GetParameter(1);
    printf( "   first estimation mean = %.0f, std-dev = %.1f, alpha = %.3f, n = %.3f\n", means[ih], sigmas[ih], ffit->GetParameter(2), ffit->GetParameter(3));
    hpixelgain->SetBinContent( (ih+128)%128+1, (ih+128)/128+1, means[ih]);
/*    ffit->SetRange( means[ih]-6*sigmas[ih], means[ih]+3*sigmas[ih]);
    ffit->SetParLimits( 0, min, max);
    ffit->SetParLimits( 1, 0, max-min);
    ffit->SetParLimits( 2, 0, 2.);
    ffit->SetParLimits( 3, 0, 2.);
    ffit->SetParLimits( 4, 0., 1.e8);
    histo->Fit( ffit, "QR");
    means[ih] = ffit->GetParameter(0);
    sigmas[ih] = ffit->GetParameter(1);
    printf( "   second estimation mean = %.0f, std-dev = %.1f, alpha = %.3f, n = %.3f\n", means[ih], sigmas[ih], ffit->GetParameter(2), ffit->GetParameter(3));*/
    hmeans->Fill( means[ih]);
    hsigmas->Fill( sigmas[ih]);
  }
  
  
  // Display
  cout << endl << "Now displaying" << endl;
  
  gStyle->SetOptFit(1);
  TCanvas *c = new TCanvas("c", "Distribution per pixels", 600, 800);
  c->Divide(1,3);
  c->cd(1);
  hall = (TH1F*)inputfile->Get("hhitseedqall");
  hall->Draw();
  ffit->SetRange( min, max);
  ffit->SetParameters( (max+min)/2, (max-min)/4, 1., 1., 1.);
  ffit->SetParLimits( 0, min, max);
  ffit->SetParLimits( 1, 0, max-min);
  ffit->SetParLimits( 2, 0.5, 1.5);
  ffit->SetParLimits( 3, 0.1, 2.);
  ffit->SetParLimits( 4, 0., 1.e8);
  hall->Fit( ffit, "QR");
  double averageMean = ffit->GetParameter(0);
  c->cd(2);
  hmeans->Draw();
  hmeans->Fit("gaus");
  c->cd(3);
  hsigmas->Draw();
  hsigmas->Fit("gaus");

  hpixelgain->Scale(1./averageMean);
  for (int ih=1; ih<129; ih++) {
    hpixelgain->SetBinContent( ih%128+1, ih/128+1, 1.);
  }
  for (int ih=768; ih<1025; ih++) {
    hpixelgain->SetBinContent( ih%128+1, ih/128+1, 1.);
  }
  
 TCanvas *c2 = new TCanvas("c2", "Distribution of gain per pixels", 100, 100, 600, 600);
 hpixelgain->Draw("colz");
  
  
  // Save
  cout << endl << "Now saving" << endl;

  TFile *fout = new TFile( "fitpeaks.root", "recreate");
  c->Write();
  hmeans->Write();
  hsigmas->Write();
  c2->Write();
  hpixelgain->Write();
  fout->Close();
}
