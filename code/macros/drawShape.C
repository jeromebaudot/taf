{
TFile f("run6Pl1_ClCharge.root","read");
gStyle->SetOptStat(0);
gStyle->SetOptFit(0);
gStyle->SetTitleSize(0.03,"xyz");
gStyle->SetTitleOffset(0.4,"y");
gStyle->SetLabelSize(0.03,"xyz");
shape->Draw();
shape->Print("run6Pl1_ClCharge_tmpFile12.pdf[");
shape->Print("run6Pl1_ClCharge_tmpFile12.pdf");
shape->Print("run6Pl1_ClCharge_tmpFile12.pdf]");
TPaveStats *p = (TPaveStats*)(chargespread->FindObject("stats"));
p->SetX1NDC(0.9);
p->SetY1NDC(0.9);
chargespread->GetXaxis()->SetLabelSize(0.03);
chargespread->GetXaxis()->SetTitleSize(0.03);
chargespread->GetYaxis()->SetTitleOffset(1.7);
chargespread->Draw("AP");
shape->Print("run6Pl1_ClCharge_tmpFile13.pdf[");
shape->Print("run6Pl1_ClCharge_tmpFile13.pdf");
shape->Print("run6Pl1_ClCharge_tmpFile13.pdf]");
gApplication->Terminate();
}
