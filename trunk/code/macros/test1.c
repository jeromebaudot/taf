{
for (Int_t i=1 ; i<8 ; i++){
  ccomp->cd(i);
  TPaveText *Pavetitle = (TPaveText*)(ccomp->GetPad(i))->GetPrimitive("title");
  Pavetitle->SetX2NDC(0.90);
  Pavetitle->Draw();
}
}
