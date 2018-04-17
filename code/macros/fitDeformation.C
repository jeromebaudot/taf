#include "TProfile.h"

void fitDeformation( const Char_t *filename, const Char_t *histoname, Double_t maxRange, Double_t angle) {

  // Open the file from the alignment procedure.
  // Fit the profile histo, like U-residual vs U or V-residual vs V
  //  using a linear combination of Legendre Polynomials.
  // The fit needs to know the maximum range of the X variable,
  //  = #pixels * pitch / 2.
  // Divide the coefficients by tan(incident angle), 
  //  so that the output of the linear combination = height deformation.
  //
  // Outputs:
  //  - print the coefficients
  //  - save a PNG image of the fit
  //
  // Units:
  //  - Range in microns
  //  - Angle in degrees.
  //
  // JB 2014/05/08
  
  TFile *inputFile = new TFile(filename,"update");
  TProfile *pfxAlign = (TProfile*)inputFile->Get(histoname);
  
  TF1 *f1legendre7 = new TF1("f1legendre7","[1] + [2]*x/[0] + [3]*(3*x*x/[0]/[0]-1)/8 + [4]*x/[0]*(5*x*x/[0]/[0]-3)/2 + [5]*(35*x*x*x*x/[0]/[0]/[0]/[0]-30*x*x/[0]/[0]+3)/8 + [6]*x/[0]*(63*x*x*x*x/[0]/[0]/[0]/[0]-70*x*x*x/[0]/[0]/[0]+15)/8 + [7]*(231*x*x*x*x*x*x/[0]/[0]/[0]/[0]/[0]/[0]-315*x*x*x*x/[0]/[0]/[0]/[0]+105*x*x/[0]/[0]-5)/16", -10000., 10000.);
  f1legendre7->SetParameters( 1152*18.4/2, 1., 0., 1., 0., 0., 0., 0.);
  f1legendre7->FixParameter( 0, 1152*18.4/2);
  pfxAlign->Fit("f1legendre7");
  
  f1legendre7->Write();
  
  cout << "coeffLegendreU";
  for (int i=1; i<8; i++) {
    printf(": %.1f ", f1legendre7->GetParameter(i)/tan(angle*TMath::DegToRad()) );
  }
  cout << endl;
  
  gPad->SaveAs(Form("%s.png",histoname));
  
}