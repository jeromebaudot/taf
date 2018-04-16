#ifndef Spatial_Resolution_BeamEScan_hh
#define Spatial_Resolution_BeamEScan_hh

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TH2D.h>
#include <TString.h>
#include <TLorentzVector.h>
#include <TLorentzRotation.h>
#include <TVector3.h>
#include <TString.h>
#include <TObject.h>

double GetResidue(double sigma_sp,
		  double E,
		  TH1F** hMC,
		  std::vector<double> _EValList);

void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);

void Spatial_Resolution_BeamEScan(const char* DataCard = "SomeMacros/DataCards/DataCard_FSBB_DS_Run35704_DESYJul2015_BeamEScan.txt");


#endif

