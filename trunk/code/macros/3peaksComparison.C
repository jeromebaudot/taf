{
  gROOT->Reset();
  gStyle->SetPalette(1);

  TCanvas* c = new TCanvas("c", "Histogrammes", 0, 0, 1000, 900);
  

  c->Divide(3,4);

  TFile f("/rawcmos13/taf/datDSF/run17514_01.root");
  //TFile f("/rawcmos13/taf/datDSF/run17515_01.root");

  TTree *t= (TTree*)f->Get("T");
/*
  c->cd(1);
  t->Draw("Htv-Hv>>h1","Hpk==3 && Htv-Hv>-30 && Htv-Hv<0");
  c->cd(2);
  t->Draw("Htv-Hv>>h2","Hpk==3 && Htv-Hv>0 && Htv-Hv<30");
  c->cd(3);
  t->Draw("Htv-Hv>>h3","Hpk==3 && Htv-Hv>30 && Htv-Hv<60"); 

  c->cd(4);
  t->Draw("Hq0/Hq1>>h4","Hpk==3 && Htv-Hv>-30 && Htv-Hv<0");
  c->cd(5);
  t->Draw("Hq0/Hq1>>h5","Hpk==3 && Htv-Hv>0 && Htv-Hv<30");
  c->cd(6);
  t->Draw("Hq0/Hq1>>h6","Hpk==3 && Htv-Hv>30 && Htv-Hv<60");
*/
  
  c->cd(1);
  t->Draw("Htv-Hv>>h1","Hpk==3 && Htv-Hv>-30 && Htv-Hv<5 && HtN>0");
  c->cd(2);
  t->Draw("Htv-Hv>>h2","Hpk==3 && Htv-Hv>5 && Htv-Hv<30 && HtN>0");
  c->cd(3);
  t->Draw("Htv-Hv>>h3","Hpk==3 && Htv-Hv>30 && Htv-Hv<60 && HtN>0");
  
  c->cd(4);
  t->Draw("Hq0/Hq1>>h4","Hpk==3 && Htv-Hv>-30 && Htv-Hv<5 && HtN>0");
  c->cd(5);
  t->Draw("Hq0/Hq1>>h5","Hpk==3 && Htv-Hv>5 && Htv-Hv<30 && HtN>0");
  c->cd(6);
  t->Draw("Hq0/Hq1>>h6","Hpk==3 && Htv-Hv>30 && Htv-Hv<60 && HtN>0");

  c->cd(7)->SetGridy();
  c->cd(7)->SetGridx();
  t->Draw("Hq0/Hn0:Hq1/Hn1>>h7(500,0,80,500,0,80)","Hpk==3 && Htv-Hv>-30 && Htv-Hv<5 && HtN>0","colz");
  c->cd(8)->SetGridy();
  c->cd(8)->SetGridx();
  t->Draw("Hq0/Hn0:Hq1/Hn1>>h8(500,0,80,500,0,80)","Hpk==3 && Htv-Hv>5   && Htv-Hv<30 && HtN>0","colz");
  c->cd(9)->SetGridy();
  c->cd(9)->SetGridx();
  t->Draw("Hq0/Hn0:Hq1/Hn1>>h9(500,0,80,500,0,80)","Hpk==3 && Htv-Hv>30  && Htv-Hv<60 && HtN>0","colz");
  
  c->cd(10);
    t->Draw("Htv-Hv:Hevt>>h10","Hpk==3","colz");
  c->cd(11);
  
  c->cd(12);
  
  


 
  TCanvas* c1 = new TCanvas("c1", "Histogrammes_2", 0, 0, 1000, 900);
  c1->Divide(3,4);

  c1->cd(1);
  t->Draw("Hsv-Hv>>h11","Hpk==3 && Htv-Hv>-30 && Htv-Hv<5 && HtN>0");
  c1->cd(2);
  t->Draw("Hsv-Hv>>h12","Hpk==3 && Htv-Hv>5 && Htv-Hv<30 && HtN>0");
  c1->cd(3);
  t->Draw("Hsv-Hv>>h13","Hpk==3 && Htv-Hv>30 && Htv-Hv<60 && HtN>0");

  c1->cd(4);
  t->Draw("Htv-Hsv>>h14","Hpk==3 && Htv-Hv>-30 && Htv-Hv<5 && HtN>0");
  c1->cd(5);
  t->Draw("Htv-Hsv>>h15","Hpk==3 && Htv-Hv>5 && Htv-Hv<30 && HtN>0");
  c1->cd(6);
  t->Draw("Htv-Hsv>>h16","Hpk==3 && Htv-Hv>30 && Htv-Hv<60 && HtN>0");

  c1->cd(7);
  t->Draw("Hv-HvCG>>h17","Hpk==2 && HtN>0");
  c1->cd(8);
  t->Draw("Hv-HvCG>>h18","Hpk==3 && HtN>0");
  c1->cd(9);
  t->Draw("Hv-HvCG>>h19","Hpk==4 && HtN>0");
  
  c1->cd(10)->SetLogy();
  t->Draw("Hu-HuCG>>h20","Hpk==2 && HtN>0");
  c1->cd(11)->SetLogy();
  t->Draw("Hu-HuCG>>h21","Hpk==3 && HtN>0");
  c1->cd(12)->SetLogy();
  t->Draw("Hu-HuCG>>h22","Hpk==4 && HtN>0");
 
}
