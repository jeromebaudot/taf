{
#include <TFile.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TGraph.h>

TFile f1("../Results/3101/HitMap_run3101.root");

TCanvas *c1 = new TCanvas("c1", "Histograms", 100, 100, 900, 900);

TH1F* hhitpixpl1 = (TH1F*)f1.Get("hhitpixpl1");
TH1F* hhitpixpl2 = (TH1F*)f1.Get("hhitpixpl2");
TH1F* hhitpixpl3 = (TH1F*)f1.Get("hhitpixpl3");
TH1F* hhitpixpl4 = (TH1F*)f1.Get("hhitpixpl4");

Double_t th1[4] = {hhitpixpl1->GetMean(), hhitpixpl2->GetMean(), hhitpixpl3->GetMean(), hhitpixpl4->GetMean()};

f1.Close();

TFile f2("../Results/3102/HitMap_run3102.root");

hhitpixpl1 = (TH1F*)f2.Get("hhitpixpl1");
hhitpixpl2 = (TH1F*)f2.Get("hhitpixpl2");
hhitpixpl3 = (TH1F*)f2.Get("hhitpixpl3");
hhitpixpl4 = (TH1F*)f2.Get("hhitpixpl4");

Double_t th2[4] = {hhitpixpl1->GetMean(), hhitpixpl2->GetMean(), hhitpixpl3->GetMean(), hhitpixpl4->GetMean()};

f2.Close();

TFile f3("../Results/3104/HitMap_run3104.root");

hhitpixpl1 = (TH1F*)f3.Get("hhitpixpl1");
hhitpixpl2 = (TH1F*)f3.Get("hhitpixpl2");
hhitpixpl3 = (TH1F*)f3.Get("hhitpixpl3");
hhitpixpl4 = (TH1F*)f3.Get("hhitpixpl4");

Double_t th3[4] = {hhitpixpl1->GetMean(), hhitpixpl2->GetMean(), hhitpixpl3->GetMean(), hhitpixpl4->GetMean()};

f3.Close();

TFile f4("../Results/3105/HitMap_run3105.root");

hhitpixpl1 = (TH1F*)f4.Get("hhitpixpl1");
hhitpixpl2 = (TH1F*)f4.Get("hhitpixpl2");
hhitpixpl3 = (TH1F*)f4.Get("hhitpixpl3");
hhitpixpl4 = (TH1F*)f4.Get("hhitpixpl4");

Double_t th4[4] = {hhitpixpl1->GetMean(), hhitpixpl2->GetMean(), hhitpixpl3->GetMean(), hhitpixpl4->GetMean()};

f4.Close();

Double_t pl1[4] = {th1[0],th2[0],th3[0],th4[0]};
Double_t pl2[4] = {th1[1],th2[1],th3[1],th4[1]};
Double_t pl3[4] = {th1[2],th2[2],th3[2],th4[2]};
Double_t pl4[4] = {th1[3],th2[3],th3[3],th4[3]};

Double_t x[4] = {7.5, 9.5, 13.5, 15.5};

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

mg->SetTitle("Cluster Size (1k events); Threshold (in sigma); Mean size");
//c1->SetLogy(1);

mg->Draw("ACP");
TLegend *leg = c1->BuildLegend();
leg->SetFillColor(0);

c1->Update();
c1->Print("~/Work/M2_2022/ClusterSize.pdf");
}
