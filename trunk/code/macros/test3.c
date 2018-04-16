{

const Int_t nn = 10;
Float_t x[nn] = {5,15,25,35,45,55,65,75,85,95};
Float_t ex[nn] = {5,5,5,5,5,5,5,5,5,5};
Float_t y[nn] = {
  hHOM_Charge_diodedist_00_10->GetFunction("landau")->GetParameter(1),
  hHOM_Charge_diodedist_10_20->GetFunction("landau")->GetParameter(1),
  hHOM_Charge_diodedist_20_30->GetFunction("landau")->GetParameter(1),
  hHOM_Charge_diodedist_30_40->GetFunction("landau")->GetParameter(1),
  hHOM_Charge_diodedist_40_50->GetFunction("landau")->GetParameter(1),
  hHOM_Charge_diodedist_50_60->GetFunction("landau")->GetParameter(1),
  hHOM_Charge_diodedist_60_70->GetFunction("landau")->GetParameter(1),
  hHOM_Charge_diodedist_70_80->GetFunction("landau")->GetParameter(1),
  hHOM_Charge_diodedist_80_90->GetFunction("landau")->GetParameter(1),
  hHOM_Charge_diodedist_90_inf->GetFunction("landau")->GetParameter(1)};
Float_t ey[nn] = {
  hHOM_Charge_diodedist_00_10->GetFunction("landau")->GetParError(1),
  hHOM_Charge_diodedist_10_20->GetFunction("landau")->GetParError(1),
  hHOM_Charge_diodedist_20_30->GetFunction("landau")->GetParError(1),
  hHOM_Charge_diodedist_30_40->GetFunction("landau")->GetParError(1),
  hHOM_Charge_diodedist_40_50->GetFunction("landau")->GetParError(1),
  hHOM_Charge_diodedist_50_60->GetFunction("landau")->GetParError(1),
  hHOM_Charge_diodedist_60_70->GetFunction("landau")->GetParError(1),
  hHOM_Charge_diodedist_70_80->GetFunction("landau")->GetParError(1),
  hHOM_Charge_diodedist_80_90->GetFunction("landau")->GetParError(1),
  hHOM_Charge_diodedist_90_inf->GetFunction("landau")->GetParError(1)};

TGraphErrors* grCharge_diodedist = new TGraphErrors(nn,x,y,ex,ey);
grCharge_diodedist->Draw("ALP");





}
