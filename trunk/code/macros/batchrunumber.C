void batchrunumber(Int_t runNumber=0) {
  // Example of macro that can be executed in batch mode
  // launch with: nohup TAF -b code/macros/batchrun.C > taf.out &
  // mote the "-b" option for batch (allows graphics to be created even if not displayed)
  //
  // JB 2009/10/06

  MimosaAnalysis *gTAF = new MimosaAnalysis();
  
  gTAF->InitSession(runNumber,1);
//  gTAF->SetDebug(-2);

//  gTAF->SetAlignStatus(2);
//  gTAF->AlignTracker( 100, 0, 10);
//  gTAF->DSFProduction(1000);

//  gTAF->MimosaPro(130000,500,6,0,0,2,"no");
  gTAF->MimosaCalibration(1000,15.,0.,5000,0,1);
  gTAF->MimosaClusterCharge("gaus");
  gTAF->CheckClusters();
  gTAF->ClusterShape();
  gTAF->HitMap();
  
  //gTAF->GetRaw()->SkipEvent(1000);
  
  gApplication->Terminate();
}
