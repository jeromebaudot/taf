void sitrineocumul(Int_t runNumber=1210, Int_t nEvt=100000, Int_t iffit=1) {
  // Macro to start SITRINEO analysis over a given nb of events
  //  iffit=1 fit tracks for momentum estimation, iffit=0 produce alignment plots without fit
  // launch with: nohup TAF 'code/macros/sitrineocumul.C([runNb], [nEvts], [iffit])' > taflog.txt &
  //
  // JB 2025/01/22

//  gTAF->SetDebug(-2);

  if( iffit==1) {
// Track with fit B field
    gTAF->InitSession(runNumber,1, 0, "./config/sitrineo-m2_B.cfg");
    gTAF->GetRaw()->SitrineoCumul(nEvt, 2); //
  }
  else {
// alignment without B field
    gTAF->InitSession(runNumber,1, 0, "./config/sitrineo-m2_align.cfg");
    gTAF->GetRaw()->SitrineoAlign(nEvt, 1, 0);
  }

  gApplication->Terminate();
}
