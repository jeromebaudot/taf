void sitrineodraw(){
  // Macro to plot the histograms from the SitrineoCumul analysis stored in Results/[runNb]/sitrineo_run[runNb].root
  // launch with: 
  //	root Results/[runNb]/sitrineo_run[runNb].root
  //    .x code/macros/sitrineodraw.C
  // JB 2024/02/19

  TPaveLabel* label = new TPaveLabel();
  Char_t canvasTitle[200];

  ccumulhit->Draw();

  // Track vector properties
  TCanvas *ccumultrack;
  TObject*  g = gROOT->FindObject("ccumultrack") ;
  if (g) {
    ccumultrack = (TCanvas*)g;
  }
  else {
    ccumultrack = new TCanvas("ccumultrack", "Track vector properties", 100, 100, 1100, 700);
  }
  ccumultrack->Clear();
  ccumultrack->UseCurrentStyle();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad4 = new TPad("pad4","",0.,0.,1.,0.965);
  pad4->Draw();
  pad4->Divide( 3, 4);
  pad4->cd(1);
  hTrackPairDX12->Draw();
  pad4->cd(2);
  hTrackPairDY12->Draw();
  pad4->cd(3);
  hTrackPairSlope12X->Draw();
  pad4->cd(4);
  hTrackPairSlope12Y->Draw();
  pad4->cd(5);
  hTrackPairDX34->Draw();
  pad4->cd(6);
  hTrackPairDY34->Draw();
  pad4->cd(7);
  //hTrackPairSlope34X->Draw();
  pad4->cd(8);
  hTrackPairSlope34Y->Draw();
  pad4->cd(9);
  hTrackPairDSlopeX->Draw();
  pad4->cd(10);
  hTrackPairMomentumX->Draw();
  pad4->cd(11);
  hTrackPairDSlopeY->Draw();
  pad4->cd(12);
  hTrackPairMomentumY->Draw();
  ccumultrack->Update();
  ccumultrack->Draw();

  // Track fit properties
  TCanvas *ccumultrack1;
  g = gROOT->FindObject("ccumultrack1") ;
  if (g) {
    ccumultrack1 = (TCanvas*)g;
  }
  else {
    ccumultrack1 = new TCanvas("ccumultrack1", "Track fit properties", 300, 300, 1100, 700);
  }
  ccumultrack1->Clear();
  ccumultrack1->UseCurrentStyle();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad5 = new TPad("pad5","",0.,0.,1.,0.965);
  pad5->Draw();
  pad5->Divide( 2, 4);
  pad5->cd(1);
  hTrackPairChi2X->Fit("fchi2X");
  hTrackPairChi2X->Draw("colz");
  pad5->cd(2);
  //hTrackPairChi2Y->Scale(1./hTrackPairChi2Y->Integral());
  hTrackPairChi2Y->Fit("fchi2Y");
  hTrackPairChi2Y->Draw("colz");
  pad5->cd(3);
  hTrackPairMomentum->Draw();
  pad5->cd(4);
  hTrackPairFitSlopeX->Draw();
  pad5->cd(5);
  hTrackPairFitSlopeY->Draw();
  pad5->cd(6);
  hTrackPairFitXA->Draw();
  pad5->cd(7);
  hTrackPairFitYA->Draw();
  ccumultrack1->Update();
  ccumultrack1->Draw();

  // Fit variables vs. Fit momentum
  TCanvas *ccumulvarp;
  g = gROOT->FindObject("ccumulvarp") ;
  if (g) {
    ccumulvarp = (TCanvas*)g;
  }
  else {
    ccumulvarp = new TCanvas("ccumulvarp", "Track vector properties", 100, 100, 1100, 700);
  }
  ccumulvarp->Clear();
  ccumulvarp->UseCurrentStyle();
  label->DrawPaveLabel(0.3,0.97,0.7,0.9999,canvasTitle);
  TPad *pad6 = new TPad("pad6","",0.,0.,1.,0.965);
  pad6->Draw();
  pad6->Divide( 2, 3);
  pad6->cd(1);
  hChi2XvsP->Draw();
  pad6->cd(2);
  hChi2YvsP->Draw();
  pad6->cd(3);
  hSlopeXvsP->Draw();
  pad6->cd(4);
  hSlopeYvsP->Draw();
  pad6->cd(5);
  hXAvsP->Draw();
  pad6->cd(6);
  hYAvsP->Draw();
  ccumulvarp->Update();
  ccumulvarp->Draw();



}
