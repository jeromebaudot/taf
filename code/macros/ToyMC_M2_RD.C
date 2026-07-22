/*
Toy MonteCarlo of the SITRNEO tracker

Simulate the momentum distribution obtained with SITRINEO
  taking into account :
  - Fermi distribution of particle momenta emmitted by the beta source
  - a given gaussian uncertainty

This is a ROOT macro, to be used with the ROOT package

* Compile in ROOT with with:
ROOT> .L ToyMC.C+

* Usage in ROOT:
ROOT> ToyMC( Z-of-source, beta-end-spectrum(keV), #events)

* Created by Romain Schotter & Vincent Juste, 2020
*/

#include"TF1Convolution.h"
#include"TMath.h"
#include"TH1F.h"
#include"TH3D.h"
#include"TCanvas.h"
#include"TRandom.h"
#include"TStyle.h"
#include"TGraph2D.h"
#include"TSystem.h"
#include"TFile.h"

using namespace std ;

//
// Display Beta spectrum
//

// ---------------------------------------------------------
double EtoP(double E) {return sqrt(E*E - 511*511);}


// ---------------------------------------------------------
Double_t fermiFunction( Double_t *x, Double_t *par ) {
  // Implementation of the Fermi function for Beta- spectrum

  Double_t Q = par[0]; // spectrum end
  Double_t Z = par[1]; // atomic number
  Double_t Norm = par[2];
  Double_t Cut_off = par[3] ; //energy cut off
  Double_t E = x[0]; // Beta energy

  if( E > Cut_off ){
    // constants
    Double_t alpha = 1./137.;
    Double_t eMass = 511; // keV/c2

    Double_t F1 = sqrt(E*E+2*eMass)*(Q-E)*(Q-E)*(E+eMass);
    Double_t g = TMath::TwoPi()*alpha*Z*(1+E/Q)*sqrt(E*E/Q/Q+2*E/Q);
    Double_t F2 = Z*Z*(E/Q+1)*(E/Q+1)*alpha*alpha+E*E/Q/Q+E/2/Q;

    Double_t p = F1*g/(exp(g)-1)*TMath::Power( F2, sqrt(1-Z*Z*alpha*alpha)-1);

    //printf("\n E=%.1f, F1=%.2f, F2=%.2f, g=%.2f\n", E, F1, F2, g);
    //printf("    g/(1-e(g))=%e, power=%.2f\n", g/(1-exp(g)), sqrt(1-Z*Z*alpha*alpha)-1);

    return Norm*p/1.e11;

  }

  else{
      return 0 ;
  }
}


// ---------------------------------------------------------

Double_t randomFermi( double Q, int Z, double cut_off) {
  // Draw an energy randomly distributed between 0 and Q
  //  according to the Fermi function with Z=atomic number.
  //  energies below cut-off are not generated (they don't go through SITRINEO)

  double par[4];
  par[0] = Q;
  par[1] = Z;
  par[2] = 1.;
  par[3] = cut_off ;
  double E[1] = { gRandom->Uniform()*Q };
  double max = 15.; // 15 for Sr90 6 for O15, 0.9 for C11, 0.2 for F18,
  while( max*gRandom->Uniform()>fermiFunction( E, par) ) {
    //printf( " essai avec E=%.2f\n", E[0]);
    E[0] = gRandom->Uniform()*Q;
  }

  return E[0];
}

//try to calculate convolution by ourselves. It works but it's not useful, that's why it is not used in the code
double conv(double *x, double *par){
    double result = 0 ;
    int N = 1000 ;
    double par_fermi[4] ; par_fermi[0] = par[0] ; par_fermi[1] = par[1] ; par_fermi[2] = par[2] ; par_fermi[3] = par[3];
    double du = (par[0] - 0.)/N ;
    double X = x[0] ;
    for( int i = 0 ;  i < N ; i++){
        double u = i*du ;
        x[0] = X - u ;
        result += fermiFunction(x, par_fermi)*par[4]*exp(-(u-par[5])*(u-par[5])/(2.*par[6]*par[6]))*du ;
    }
    return result ;
}

// ---------------------------------------------------------



void ToyMC_M2( Double_t Z = 39., Double_t Q = 2280, Int_t Nevts=1000 ){
  // Build the beta energy spectrum for a dedicated nuclei.
  // Fit the distribution with a simple function.
  //
  // Energy unit in keV
  //
  // JB 2013/02/28
  //
  // Nuclei caracteristics
  // Beta-: H3 18, C14 156, S32 167, P33 250, Ca45 257, P32 1710, Sr39 2280
  // Beta+: C11 961, N13 1119, O15 1720, F18 650


  // init and reset of Root
   //gROOT->Reset();
   gStyle->SetOptFit(1);

  Char_t text[100];
  sprintf( text, "Z=%d, Q=%.0f keV, N = %d", (int)Z, Q, Nevts);


  //1D histos
  TH1F *hspectrum_F = new TH1F( "hspectrum_F", "Fermi spectrum", 100, 0., 1.2*Q);
  hspectrum_F->SetXTitle("Energy (keV)");
  hspectrum_F->SetFillStyle(1001);
  hspectrum_F->SetFillColor(3);
  hspectrum_F->SetStats(0);

  TH1F *hspectrum_L = new TH1F( "hspectrum_L", "Landau spectrum", 100, -Q, Q);
  hspectrum_L->SetXTitle("Energy (keV)");
  hspectrum_L->SetFillStyle(1001);
  hspectrum_L->SetFillColor(3);
  hspectrum_L->SetStats(0);

  TH1F *hspectrum_FL = new TH1F( "hspectrum_FL", "Fermi & Landau Convolution", 100, 0., 1e4);
  hspectrum_FL->SetXTitle("Energy (keV)");
  hspectrum_FL->SetFillStyle(1001);
  hspectrum_FL->SetFillColor(3);
  hspectrum_FL->SetStats(0);



  double Cutoff_initial = 100 ; // cutoff is the minimal energy to go through SITRINEO (keV)
  double Cutoff_final = 1800 ;
  int NCuts = 40 ;

  double sigma_initial = 0.1 ;
  double sigma_final = 0.4 ;
  int NSigma = 30 ;


  //contain the value of the MPV or mean or rms or FWHM or skewness or kurtosis as a function of the cutoff and sigma,
  // in order to find "good" observables to identify the cutoff and the sigma ( sigma = sigma of the gaussian )
  //in the end, good observable for the cutoff is the mean because it is almost constant with sigma.

  vector<double> Sigma, cutoff, mean, mean_cutoff, MPV, MPV_cutoff, rms, fwhm, skew, kurt ;
  //search to get mean, MPV, rms, FWHM, skewness, kurtosis for different cutoff and sigma

  for( int iCut = 0 ; iCut < NCuts ; iCut++ ){//loop on the cutoff
      //if( iCut%10 == 0) cout << "Cut " << iCut << endl;

      double cut_off = Cutoff_initial + iCut*(Cutoff_final - Cutoff_initial)/NCuts ;

      for( int iSigma = 0 ; iSigma < NSigma ; iSigma++ ){//loop on sigma

            double sigma = sigma_initial + iSigma*(sigma_final - sigma_initial)/NSigma ;

            for ( int i=0; i<Nevts; i++) {//loop on the number of events
                double_t x=randomFermi(Q,(int)Z, cut_off);
                double_t y=gRandom->Landau(0.,sigma*x);
                double_t z = EtoP(x)+y ; //is y already a p ?
                hspectrum_FL->Fill(z);

            }//end loop on the number of events

            hspectrum_FL->Scale(1./Nevts);

            //find Full Width at Half Maxima
            double low_edge = 0 ; double upper_edge = 0 ; double FWHM = 0 ;
            for( int i = 0 ; i < hspectrum_FL->GetNbinsX() ; i++){
                if( i < hspectrum_FL->GetMaximumBin() ){
                    if( hspectrum_FL->GetBinContent(i) < (hspectrum_FL->GetBinContent( hspectrum_FL->GetMaximumBin() ))/2. ){
                        low_edge = hspectrum_FL->GetBinCenter(i);
                    }
                    else{ continue ;}
                }
                if( i > hspectrum_FL->GetMaximumBin() ){
                    if( hspectrum_FL->GetBinContent(i) > (hspectrum_FL->GetBinContent( hspectrum_FL->GetMaximumBin() ))/2. ){
                        upper_edge = hspectrum_FL->GetBinCenter(i);
                    }
                    else{ continue ;}
                }
            }

            FWHM = upper_edge-low_edge;

            cutoff.push_back(cut_off);
            Sigma.push_back(sigma);
            mean.push_back(hspectrum_FL->GetMean());
            mean_cutoff.push_back(hspectrum_FL->GetMean() -cut_off);
            MPV.push_back(hspectrum_FL->GetBinCenter( hspectrum_FL->GetMaximumBin()));
            MPV_cutoff.push_back(hspectrum_FL->GetBinCenter( hspectrum_FL->GetMaximumBin()) - cut_off );
            rms.push_back(hspectrum_FL->GetRMS());
            fwhm.push_back(FWHM);
            skew.push_back(hspectrum_FL->GetSkewness());
            kurt.push_back(hspectrum_FL->GetKurtosis());

            hspectrum_FL->Reset();


      }//end loop on sigma


  }//end loop on cutoff


  TGraph2D* hMean = new TGraph2D(Sigma.size(), Sigma.data(), cutoff.data(), mean.data());
  hMean->SetTitle("Mean(Sigma, Cutoff); Sigma ; Cutoff (keV); Mean (keV)");
  hMean->GetXaxis()->SetTitleOffset(0.05);
  hMean->GetYaxis()->SetTitleOffset(0.05);
  hMean->GetZaxis()->SetTitleOffset(0.05);

  TGraph2D* hMean_cutoff = new TGraph2D(Sigma.size(), Sigma.data(), cutoff.data(), mean_cutoff.data());
  hMean_cutoff->SetTitle("Mean(Sigma, Cutoff) - Cutoff; Sigma ; Cutoff (keV); Mean -cutoff (keV)");
  hMean_cutoff->GetXaxis()->SetTitleOffset(0.05);
  hMean_cutoff->GetYaxis()->SetTitleOffset(0.05);
  hMean_cutoff->GetZaxis()->SetTitleOffset(0.05);
  hMean_cutoff->SetMarkerStyle(kOpenSquare);

  TGraph2D* hMPV = new TGraph2D(Sigma.size(), Sigma.data(), cutoff.data(), MPV.data());
  hMPV->SetTitle("MPV(Sigma, Cutoff); Sigma ; Cutoff (keV); MPV (keV)");
  hMPV->GetXaxis()->SetTitleOffset(0.05);
  hMPV->GetYaxis()->SetTitleOffset(0.05);
  hMPV->GetZaxis()->SetTitleOffset(0.05);
  hMPV->SetMarkerStyle(kOpenSquare);

  TGraph2D* hMPV_cutoff = new TGraph2D(Sigma.size(), Sigma.data(), cutoff.data(), MPV_cutoff.data());
  hMPV_cutoff->SetTitle("MPV(Sigma, Cutoff) - Cutoff; Sigma ; Cutoff (keV); MPV -cutoff (keV)");
  hMPV_cutoff->GetXaxis()->SetTitleOffset(0.05);
  hMPV_cutoff->GetYaxis()->SetTitleOffset(0.05);
  hMPV_cutoff->GetZaxis()->SetTitleOffset(0.05);
  hMPV_cutoff->SetMarkerStyle(kOpenSquare);

  TGraph2D* hRMS = new TGraph2D(Sigma.size(), Sigma.data(), cutoff.data(), rms.data());
  hRMS->SetTitle("RMS(Sigma, Cutoff); Sigma (keV); Cutoff (keV); RMS (keV)");
  hRMS->GetXaxis()->SetTitleOffset(0.05);
  hRMS->GetXaxis()->SetLabelSize(0.025);
  hRMS->GetYaxis()->SetTitleOffset(0.05);
  hRMS->GetZaxis()->SetTitleOffset(0.05);

  TGraph2D* hFWHM = new TGraph2D(Sigma.size(), Sigma.data(), cutoff.data(), fwhm.data());
  hFWHM->SetTitle("FWHM(Sigma, Cutoff); Sigma (keV); Cutoff (keV); FWHM (keV)");
  hFWHM->GetXaxis()->SetTitleOffset(0.05);
  hFWHM->GetXaxis()->SetLabelSize(0.025);
  hFWHM->GetYaxis()->SetTitleOffset(0.05);
  hFWHM->GetZaxis()->SetTitleOffset(0.05);
  hFWHM->SetMarkerStyle(kOpenSquare);

  TGraph2D* hSKEW = new TGraph2D(Sigma.size(), Sigma.data(), cutoff.data(), skew.data());
  hSKEW->SetTitle("Skew(Sigma, Cutoff); Sigma (keV); Cutoff (keV); Skewness");
  hSKEW->GetXaxis()->SetTitleOffset(0.05);
  hSKEW->GetXaxis()->SetLabelSize(0.025);
  hSKEW->GetYaxis()->SetTitleOffset(0.05);
  hSKEW->GetZaxis()->SetTitleOffset(0.05);
  hSKEW->SetMarkerStyle(kOpenSquare);

  TGraph2D* hKURT = new TGraph2D(Sigma.size(), Sigma.data(), cutoff.data(), kurt.data());
  hKURT->SetTitle("Kurt(Sigma, Cutoff); Sigma (keV); Cutoff (keV); Kurtosis");
  hKURT->GetXaxis()->SetTitleOffset(0.05);
  hKURT->GetXaxis()->SetLabelSize(0.025);
  hKURT->GetYaxis()->SetTitleOffset(0.05);
  hKURT->GetZaxis()->SetTitleOffset(0.05);
  hKURT->SetMarkerStyle(kOpenSquare);


  //try to calculate analytically to the convolution of the fermi function and a gaussian
  //and to fit the spectrum obtain in the next lines with this convolution
  //It didnt work, so you wont get any fit result related to this in the terminal
  TF1 *fspectrum = new TF1( "fspectrum", fermiFunction, 0., Q, 4);
  fspectrum->SetParName( 0, "Q");
  fspectrum->SetParName( 1, "Z");
  fspectrum->SetParName( 2, "norm");
  fspectrum->SetParName( 3, "cutoff");
  fspectrum->SetParameters( Q, Z, 1., 900);

  TF1 *gspectrum = new TF1( "gspectrum", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2]))", -Q, Q);
  gspectrum->SetTitle("gspectrum");
  gspectrum->SetParName( 0, "Amp");
  gspectrum->SetParName( 1, "mu");
  gspectrum->SetParName( 2, "sigma");
  gspectrum->SetParameters( 1., 0., 400);

  TF1Convolution *fgspectrum = new TF1Convolution("fspectrum", "gspectrum", 0, Q, true);
  fgspectrum->SetRange(0., Q);
  fgspectrum->SetNofPointsFFT(1000);
  TF1 *f = new TF1("f",*fgspectrum, 0., Q, fgspectrum->GetNpar());
  f->SetParName(0, "Q");
  f->SetParName(1, "Z");
  f->SetParName(2, "norm");
  f->SetParName(3, "cutoff");
  f->SetParName(4, "Amp");
  f->SetParName(5, "mu");
  f->SetParName(6, "sigma");
  f->SetParameters(Q, Z, 1., 900, 1., 0., 400);


  //Generates the spectrum, resulting from the convolution of the fermi function and a gaussian, for a given sigma and cutoff
  double cut_off = 1100;
  double sigma = 0.3 ;
   for ( int i=0; i<Nevts; i++) {
    double_t x=randomFermi(Q,(int)Z, cut_off);
    double_t y=gRandom->Landau(0.,sigma*x);
    double_t z = EtoP(x+y);
    hspectrum_F->Fill(x);
    hspectrum_L->Fill(y);
    hspectrum_FL->Fill(z);

  }
  hspectrum_F->Scale(1./Nevts);
  hspectrum_L->Scale(1./Nevts);
  hspectrum_FL->Scale(1./Nevts);

  //The left part of the obtained spectrum is dominated by the gaussian, so fitting the left side of the obtained spectrum with a gaussian allows us to access to sigma. Thanks to sigma and the mean value, we can deduce the cutoff
  TF1 *fit = new TF1("fit", "Landau", 0, hspectrum_FL->GetMean());
  hspectrum_FL->Fit("fit", "RN");
  double sigma_fit = fit->GetParameter(2)/fit->GetParameter(1) ;
  std::cout << "Sigma fit relative = " << sigma_fit << std::endl ;
  std::cout << "Sigma fit = " << fit->GetParameter(2) << std::endl ;
  std::cout << "Mean value = " << hspectrum_FL->GetMean() << " ==> Cut off = "  << std::endl ;
  std::cout << "Mean value Fermi = " << hspectrum_F->GetMean() << std::endl ;
  std::cout << "Std. deviation value Landau = " << hspectrum_L->GetRMS() << std::endl ;

  //plotting
  //canvas with all the spectrums
  TCanvas *c = new TCanvas("c", text, 0,0, 900, 900);
  c->Divide(3,2);
  c->cd(1);
  hspectrum_F->Draw("");
  c->cd(2);
  hspectrum_L->Draw("");
  c->cd(3);
  hspectrum_FL->Draw("");
  //h_ExpGauss_test->Draw("SAME");
  c->cd(4);
  fspectrum->Draw("");
  c->cd(5);
  gspectrum->Draw("");
  //f->Draw();
  c->cd(6);
  //hspectrum_FL->Fit("f");
  f->Draw("");


  //canvas with all observables
  TCanvas *c1 = new TCanvas("Cutoff", text, 0,0, 1800, 900);
  c1->Divide(4,2);
  c1->cd(1);
  hMean->Draw("surf1");
  c1->cd(2);
  hMean_cutoff->Draw("surf1");
  c1->cd(3);
  hMPV->Draw("surf1");
  c1->cd(4);
  hMPV_cutoff->Draw("surf1");
  c1->cd(5);
  hRMS->Draw("surf1");
  c1->cd(6);
  hFWHM->Draw("surf1");
  c1->cd(7);
  hSKEW->Draw("surf1");
  c1->cd(8);
  hKURT->Draw("surf1");

  //canvas with all observables
  TCanvas *c2 = new TCanvas("ExperimentalSpectrum", text, 0,0, 800, 800);
  hspectrum_FL->Draw("");
  double xmin = hspectrum_FL->GetXaxis()->GetXmin();
  double xmax = hspectrum_FL->GetXaxis()->GetXmax();
  hspectrum_FL->GetXaxis()->SetLimits(xmin/1000,xmax/1000);
  hspectrum_FL->GetXaxis()->SetTitle("Momentum (MeV/c)");
  TFile fRoot("~/Work/M2_2022/sitrineo_2mil_chi2_cuts_10_60.root", "UPDATE");
  fRoot.Delete("hspectrum_FL;1");
  hspectrum_FL->Write();
  fRoot.Close();

}
