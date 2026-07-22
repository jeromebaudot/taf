void macroL32024(Int_t runNumber=24, Int_t nEvt=100000) {
  // Macro to start continuous SITRINEO analysis with evt by evt display
  // launch with: nohup TAF code/macros/sitrineocumul.C &
  //
 // JB 2021/10/02

  gTAF->InitSession(runNumber,1, 0, "./config/sitrineo-L3-2024.cfg");
//  gTAF->SetDebug(-2);

  gTAF->GetRaw()->DisplayCumulatedHits2D(nEvt);
  //gApplication->Terminate();
}
