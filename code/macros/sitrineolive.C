void sitrineolive(Int_t runNumber=1506, Int_t nEvt=1000000) {
  // Macro to start continuous SITRINEO analysis with evt by evt display
  // launch with: nohup TAF code/macros/sitrineolive.C &
  //
  // JB 2021/10/02

  gTAF->InitSession(runNumber,1, 0, "./config/sitrineol3-1cm.cfg");
//  gTAF->SetDebug(-2);

  gTAF->GetRaw()->SitrineoContinuous(nEvt, 2);

  gApplication->Terminate();
}
