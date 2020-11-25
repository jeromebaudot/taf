{
  gEnv->SetValue("Rint.History","./taf_hist");
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat(1111111);
//  gStyle->SetPadBottomMargin(2);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.15);
  gStyle->SetTitleX(0.5);
  gStyle->SetTitleAlign(23);
  gStyle->SetTitleSize(0.03,"xyz");
  gStyle->SetTitleOffset(0.3,"y");
  gStyle->SetLabelSize(0.03,"xyz");
  //  simply launch root instead of TAF
  gSystem->Load("libMinuit.so");
  gSystem->Load("libGeom.so");
  gSystem->Load("libEG.so");
  gSystem->Load("libGui.so");
  gSystem->Load("libGed.so");
  gSystem->Load("libFoam.so");
  gSystem->Load("libTreePlayer.so");
  //gSystem->Load("libRooFit.so");
  //gSystem->Load("libRooFitCore.so");
  gSystem->Load("libPostscript.so");
  gSystem->Load("libPhysics.so");
  gSystem->Load("libRint.so");
  //gSystem->Load("libSpectrum.so");
  gSystem->Load("libTMVA.so");
  gSystem->Load("bin/lib/libDTools.so");
  gSystem->Load("bin/lib/libTAF.so");
//  MimosaAnalysis *gTAF = new MimosaAnalysis();
}
