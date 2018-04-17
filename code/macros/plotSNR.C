{

  // This macro plots the SNR (hSNReal histo) for various ClCharge files

  gStyle->SetOptTitle(0);
  gStyle->SetOptStat("10");
  gStyle->SetPadRightMargin(.05);
  
  Int_t binResizing = 4;
  Int_t fillStyle = 3001;
  Char_t title[50];
  Double_t mpv;
  
  TText *preliminary = new TText( .3, .3, "PRELIMINARY");
  preliminary->SetNDC();
  preliminary->SetTextSize(.12);
  preliminary->SetTextAngle(45.);
  preliminary->SetTextColor(17);

  TCanvas *c = new TCanvas("c","SNR",1);
  
  // 1st submatrix
  Color_t col1 = kBlue;
  TFile file1("results_ana_M22/mi22thra-HR20_S1-20degC_ClCharge.root");
  TH1F *hs1=(TH1F*)file1.Get("hSNReal");
  //hs1->Sumw2();
  hs1->SetTitle("Submatrix 1");
  hs1->Scale( 1./hs1->GetEntries() );
  hs1->Rebin(binResizing);
  hs1->SetMaximum(.05);
  hs1->SetFillStyle(fillStyle);
  hs1->SetFillColor(col1);
  hs1->SetLineColor(col1);
  hs1->SetStats(0);
  hs1->Draw();
  //TPaveStats *stat1 = (TPaveStats*)hs1->GetListOfFunctions()->FindObject("stats");
  //stat1->SetOptStat("10");
  //stat1->SetTextColor(col1);

  // 2nd submatrix
  Color_t col2 = kRed;
  TFile file2("results_ana_M22/mi22thra-HR20_S2-20degC_ClCharge.root");
  TH1F *hs2=(TH1F*)file2.Get("hSNReal");
  //hs2->Sumw2();
  hs2->SetTitle("Submatrix 2");
  hs2->Scale( 1./hs2->GetEntries() );
  hs2->Rebin(binResizing);
  hs2->SetFillStyle(fillStyle);
  hs2->SetFillColor(col2);
  hs2->SetLineColor(col2);
  hs2->SetStats(0);
  hs2->Draw("same");
  //TPaveStats *stat2 = (TPaveStats*)hs2->GetListOfFunctions()->FindObject("stats");
  //stat2->SetOptStat(10);
  //stat2->SetTextColor(col2);
  
  // 3rd submatrix
  Color_t col3 = kMagenta;
  TFile file3("results_ana_M22/mi22thra-HR20_S3-20degC_ClCharge.root");
  TH1F *hs3=(TH1F*)file3.Get("hSNReal");
  //hs3->Sumw2();
  hs3->SetTitle("Submatrix 3");
  hs3->Scale( 1./hs3->GetEntries() );
  hs3->Rebin(binResizing);
  hs3->SetFillStyle(fillStyle);
  hs3->SetFillColor(col3);
  hs3->SetLineColor(col3);
  hs3->SetStats(0);
  hs3->Draw("same");
  //TPaveStats *stat3 = (TPaveStats*)hs3->GetListOfFunctions()->FindObject("stats");
  //stat3->SetOptStat(10);
  //stat3->SetTextColor(col3);
  
  // Now rebin all and fit
  hs1->Fit("landau");
  TF1 *f1 = hs1->GetFunction("landau");
  f1->SetLineColor(col1);
  sprintf(title,"%s - MPV = %.1f #pm %.1f", hs1->GetTitle(), f1->GetParameter(1), f1->GetParError(0));
  hs1->SetTitle(title);
  hs2->Fit("landau");
  TF1 *f2 = hs2->GetFunction("landau");
  f2->SetLineColor(col2);
  sprintf(title,"%s - MPV = %.1f #pm %.1f", hs2->GetTitle(), f2->GetParameter(1), f2->GetParError(0));
  hs2->SetTitle(title);
  hs3->Fit("landau");
  TF1 *f3 = hs3->GetFunction("landau");
  f3->SetLineColor(col3);
  sprintf(title,"%s - MPV = %.1f #pm %.1f", hs3->GetTitle(), f3->GetParameter(1), f3->GetParError(0));
  hs3->SetTitle(title);

  hs1->Draw();
  hs2->Draw("same");
  hs3->Draw("same");
  TLegend *leg = c->BuildLegend( 0.5,0.6,0.85,0.89,"MIMOSA 22 THRA - HR20 - 20^{o}C");
  leg->SetEntryOption("F");
  leg->SetFillColor(0);
  leg->SetTextSize(.03);
  preliminary->Draw();

  //return;
  
  
  TCanvas *c2 = new TCanvas("c2","SNR",1);
    
  // 1st submatrix 2nd chip
  Color_t col4 = kRed+3;
  TFile file4("results_ana_M22/mi22thra-HR18_S1-20degC_ClCharge.root");
  TH1F *hs4=(TH1F*)file4.Get("hSNReal");
  //hs4->Sumw2();
  hs4->SetTitle("HR18");
  hs4->Scale( 1./hs4->GetEntries() );
  hs4->Rebin(binResizing);
  hs4->SetFillStyle(fillStyle);
  hs4->SetFillColor(col4);
  hs4->SetLineColor(col4);
  hs4->SetStats(0);
  hs4->Draw("same");
  //TPaveStats *stat3 = (TPaveStats*)hs3->GetListOfFunctions()->FindObject("stats");
  //stat3->SetOptStat(10);
  //stat3->SetTextColor(col3);
  hs4->Fit("landau");
  TF1 *f4 = hs4->GetFunction("landau");
  f4->SetLineColor(col4);
  sprintf(title,"%s - 20^{o}C - MPV = %.1f #pm %.1f", hs4->GetTitle(), f4->GetParameter(1), f4->GetParError(0));
  hs4->SetTitle(title);

  sprintf(title,"HR20 - 20^{o}C - MPV = %.1f #pm %.1f", f1->GetParameter(1), f1->GetParError(0));
  hs1->SetTitle(title);

  hs1->Draw();
  hs4->Draw("same");
  TLegend *leg = c2->BuildLegend( 0.5,0.6,0.85,0.89,"MIMOSA 22 THRA - Submatrix 1");
  leg->SetEntryOption("F");
  leg->SetFillColor(0);
  leg->SetTextSize(.03);
  preliminary->Draw();

  //return;
/* 
  TCanvas *c3 = new TCanvas("c3","SNR",1);
  
  // 1st submatrix
  Color_t col1 = kBlue;
  TFile file1("results_ana_M22/mi22thra-HR18_S1-20degC_ClCharge.root");
  TH1F *hs1=(TH1F*)file1.Get("hSNReal");
  //hs1->Sumw2();
  hs1->SetTitle("Submatrix 1");
  hs1->Scale( 1./hs1->GetEntries() );
  hs1->Rebin(binResizing);
  hs1->SetMaximum(.05);
  hs1->SetFillStyle(fillStyle);
  hs1->SetFillColor(col1);
  hs1->SetLineColor(col1);
  hs1->SetStats(0);
  hs1->Draw();
  //TPaveStats *stat1 = (TPaveStats*)hs1->GetListOfFunctions()->FindObject("stats");
  //stat1->SetOptStat("10");
  //stat1->SetTextColor(col1);
  
  // 2nd submatrix
  Color_t col2 = kRed;
  TFile file2("results_ana_M22/mi22thra-HR18_S2-20degC_ClCharge.root");
  TH1F *hs2=(TH1F*)file2.Get("hSNReal");
  //hs2->Sumw2();
  hs2->SetTitle("Submatrix 2");
  hs2->Scale( 1./hs2->GetEntries() );
  hs2->Rebin(binResizing);
  hs2->SetFillStyle(fillStyle);
  hs2->SetFillColor(col2);
  hs2->SetLineColor(col2);
  hs2->SetStats(0);
  hs2->Draw("same");
  //TPaveStats *stat2 = (TPaveStats*)hs2->GetListOfFunctions()->FindObject("stats");
  //stat2->SetOptStat(10);
  //stat2->SetTextColor(col2);
  
  // 3rd submatrix
  Color_t col3 = kMagenta;
  TFile file3("results_ana_M22/mi22thra-HR18_S3-20degC_ClCharge.root");
  TH1F *hs3=(TH1F*)file3.Get("hSNReal");
  //hs3->Sumw2();
  hs3->SetTitle("Submatrix 3");
  hs3->Scale( 1./hs3->GetEntries() );
  hs3->Rebin(binResizing);
  hs3->SetFillStyle(fillStyle);
  hs3->SetFillColor(col3);
  hs3->SetLineColor(col3);
  hs3->SetStats(0);
  hs3->Draw("same");
  //TPaveStats *stat3 = (TPaveStats*)hs3->GetListOfFunctions()->FindObject("stats");
  //stat3->SetOptStat(10);
  //stat3->SetTextColor(col3);
  
  // Now rebin all and fit
  hs1->Fit("landau");
  TF1 *f1 = hs1->GetFunction("landau");
  f1->SetLineColor(col1);
  sprintf(title,"%s - MPV = %.1f #pm %.1f", hs1->GetTitle(), f1->GetParameter(1), f1->GetParError(0));
  hs1->SetTitle(title);
  hs2->Fit("landau");
  TF1 *f2 = hs2->GetFunction("landau");
  f2->SetLineColor(col2);
  sprintf(title,"%s - MPV = %.1f #pm %.1f", hs2->GetTitle(), f2->GetParameter(1), f2->GetParError(0));
  hs2->SetTitle(title);
  hs3->Fit("landau");
  TF1 *f3 = hs3->GetFunction("landau");
  f3->SetLineColor(col3);
  sprintf(title,"%s - MPV = %.1f #pm %.1f", hs3->GetTitle(), f3->GetParameter(1), f3->GetParError(0));
  hs3->SetTitle(title);
  
  hs1->Draw();
  hs2->Draw("same");
  hs3->Draw("same");
  TLegend *leg = c3->BuildLegend( 0.5,0.6,0.85,0.89,"MIMOSA 22 THRA - HR18 - 20^{o}C");
  leg->SetEntryOption("F");
  leg->SetFillColor(0);
  leg->SetTextSize(.03);
  preliminary->Draw();
*/
  
}