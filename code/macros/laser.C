void laser( Int_t run=18724, Int_t x=125, Int_t y=125) {
  // Call the laser study
  MimosaAnalysis *gTAF = new MimosaAnalysis();               
  gTAF->InitSession(run);
  gTAF->GetRaw()->SkipEvent(300); // always needed for TNT
  gTAF->GetRaw()->LaserStudy( x, y, 20, 20000);
}