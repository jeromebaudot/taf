void compareSeedCharge() {

  TFile *fw = new TFile("Results/4182430/HitMap_run4182405.root");
  TH1F * hw = (TH1F*)fw->Get("hhitseedq1");
  hw->SetTitle("with pixel gain correction");
  hw->SetLineColor(2);
  hw->GetXaxis()->SetRange(1,2000);

  TFile *fwo = new TFile("Results/4182430/PixelSpectrum_run4182405_pixel128.root");
  TH1F * hwo = (TH1F*)fwo->Get("hhitseedqall");
  hwo->SetTitle("without correction");
  
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  
  hw->Draw();
  hwo->Draw("same");
  gPad->BuildLegend(0.20, 0.55, 0.6, 0.8);

}
