{
#include <TFile.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TGraph.h>

TFile f1("../Results/3002/HitMap_run3002.root");

TCanvas *c1 = new TCanvas("c1", "Histograms", 100, 100, 900, 900);

TH1F* hhitpixpl1 = (TH1F*)f1.Get("hhitpixpl1");
TH1F* hhitpixpl2 = (TH1F*)f1.Get("hhitpixpl2");
TH1F* hhitpixpl3 = (TH1F*)f1.Get("hhitpixpl3");
TH1F* hhitpixpl4 = (TH1F*)f1.Get("hhitpixpl4");

Double_t th1[4] = {hhitpixpl1->GetEntries(), hhitpixpl2->GetEntries(), hhitpixpl3->GetEntries(), hhitpixpl4->GetEntries()};
for (int i=0; i<4; i++) th1[i] *= 1./928/960/10000;

f1.Close();

TFile f2("../Results/3003/HitMap_run3003.root");

hhitpixpl1 = (TH1F*)f2.Get("hhitpixpl1");
hhitpixpl2 = (TH1F*)f2.Get("hhitpixpl2");
hhitpixpl3 = (TH1F*)f2.Get("hhitpixpl3");
hhitpixpl4 = (TH1F*)f2.Get("hhitpixpl4");

Double_t th2[4] = {hhitpixpl1->GetEntries(), hhitpixpl2->GetEntries(), hhitpixpl3->GetEntries(), hhitpixpl4->GetEntries()};
for (int i=0; i<4; i++) th2[i] *= 1./928/960/1000;

f2.Close();

TFile f3("../Results/3004/HitMap_run3004.root");

hhitpixpl1 = (TH1F*)f3.Get("hhitpixpl1");
hhitpixpl2 = (TH1F*)f3.Get("hhitpixpl2");
hhitpixpl3 = (TH1F*)f3.Get("hhitpixpl3");
hhitpixpl4 = (TH1F*)f3.Get("hhitpixpl4");

Double_t th3[4] = {hhitpixpl1->GetEntries(), hhitpixpl2->GetEntries(), hhitpixpl3->GetEntries(), hhitpixpl4->GetEntries()};
for (int i=0; i<4; i++) th3[i] *= 1./928/960/10000;

f3.Close();

TFile f4("../Results/3005/HitMap_run3005.root");

hhitpixpl1 = (TH1F*)f4.Get("hhitpixpl1");
hhitpixpl2 = (TH1F*)f4.Get("hhitpixpl2");
hhitpixpl3 = (TH1F*)f4.Get("hhitpixpl3");
hhitpixpl4 = (TH1F*)f4.Get("hhitpixpl4");

Double_t th4[4] = {hhitpixpl1->GetEntries(), hhitpixpl2->GetEntries(), hhitpixpl3->GetEntries(), hhitpixpl4->GetEntries()};
for (int i=0; i<4; i++) th4[i] *= 1./928/960/10000;

f4.Close();

Double_t pl1[4] = {th1[0],th2[0],th3[0],th4[0]};
Double_t pl2[4] = {th1[1],th2[1],th3[1],th4[1]};
Double_t pl3[4] = {th1[2],th2[2],th3[2],th4[2]};
Double_t pl4[4] = {th1[3],th2[3],th3[3],th4[3]};

Double_t x[4] = {6, 8, 9.5, 11.5};

TGraph* gr1 = new TGraph(4, x, pl1);
gr1->SetMarkerStyle(21);
gr1->SetMarkerColor(1);
gr1->SetLineColor(1);
gr1->SetFillColor(0);
gr1->SetTitle("Plane 1");
TGraph* gr2 = new TGraph(4, x, pl2);
gr2->SetMarkerStyle(21);
gr2->SetMarkerColor(2);
gr2->SetLineColor(2);
gr2->SetFillColor(0);
gr2->SetTitle("Plane 2");
TGraph* gr3 = new TGraph(4, x, pl3);
gr3->SetMarkerStyle(21);
gr3->SetMarkerColor(3);
gr3->SetLineColor(3);
gr3->SetFillColor(0);
gr3->SetTitle("Plane 3");
TGraph* gr4 = new TGraph(4, x, pl4);
gr4->SetMarkerStyle(21);
gr4->SetMarkerColor(4);
gr4->SetLineColor(4);
gr4->SetFillColor(0);
gr4->SetTitle("Plane 4");

TMultiGraph* mg = new TMultiGraph();
mg->Add(gr1);
mg->Add(gr2);
mg->Add(gr3);
mg->Add(gr4);

mg->SetTitle("Fake Rate (10k events); Threshold (in sigma);");
c1->SetLogy(1);

mg->Draw("ACP");
TLegend *leg = c1->BuildLegend();
leg->SetFillColor(0);

c1->Update();
c1->Print("~/Work/M2_2022/FakeRate.pdf");
}
