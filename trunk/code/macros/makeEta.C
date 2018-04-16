{
  // Expect to have TTree in file f
  TTree *T = (TTree*)f->Get("T");

  Float_t pitch = 10.;

  TH1F *hdenu = new TH1F("hdenu","",250, -pitch/2., pitch/2.);
  TH1F *hdenv = new TH1F("hdenv","",250, -pitch/2., pitch/2.);

  T->Draw("Hv-Hsv>>hdenu","Hpk==1 && Hhk==1");
  T->Draw("Hv-Hsv>>hdenv","Hpk==1 && Hhk==1");

  TH1F *hintu = new TH1F("HEtaIntUPk1", "", 250, -pitch/2., pitch/2.);
  TH1F *hintv = new TH1F("HEtaIntVPk1", "", 250, -pitch/2., pitch/2.);

  Float_t content,total;
  content=total=0.;

  total=hdenu->Integral(1,hdenu->GetNbinsX());
  for(Int_t i=1; i<hdenu->GetNbinsX(); i++){
    content=hdenu->Integral(1,i);
    hintu->SetBinContent(i,content/total*pitch-pitch/2.);
  }    

  total=hdenv->Integral(1,hdenv->GetNbinsX());
  for(Int_t i=1; i<hdenv->GetNbinsX(); i++){
    content=hdenv->Integral(1,i);
    hintv->SetBinContent(i,content/total*pitch-pitch/2.);
  }    

  TCanvas ceta("ceta");
  ceta.Divide(2,2);
  ceta.cd(1);
  hdenu.Draw();
  ceta.cd(3);
  hintu->Draw();
  ceta.cd(2);
  hdenv.Draw();
  ceta.cd(4);
  hintv->Draw();

  TH1F *hintu2 = new TH1F(*hintu);
  hintu2->SetName("HEtaIntUPk2");
  TH1F *hintv2 = new TH1F(*hintv);
  hintv2->SetName("HEtaIntVPk2");

  TH1F *hintu3 = new TH1F(*hintu);
  hintu3->SetName("HEtaIntUPk3");
  TH1F *hintv3 = new TH1F(*hintv);
  hintv3->SetName("HEtaIntVPk3");

  TH1F *hintu4 = new TH1F(*hintu);
  hintu4->SetName("HEtaIntUPk4");
  TH1F *hintv4 = new TH1F(*hintv);
  hintv4->SetName("HEtaIntVPk4");

  TFile file("inf.root","RECREATE");
  hintu->Write();
  hintv->Write();
  hintu2->Write();
  hintv2->Write();
  hintu3->Write();
  hintv3->Write();
  hintu4->Write();
  hintv4->Write();
  file.Close();

}
