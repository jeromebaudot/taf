void batchrunumber(Int_t runNumber=0) {
  // Example of macro that can be executed in batch mode
  // launch with: nohup TAF -b code/macros/batchrunumber.C > taf.out &
  // mote the "-b" option for batch (allows graphics to be created even if not displayed)
  //
  // JB 2009/10/06

  MimosaAnalysis *gTAF = new MimosaAnalysis();

  gTAF->InitSession(runNumber,3);
// gTAF->InitSession(runNumber,planeNumber,-1,configFile);
//  gTAF->SetDebug(-2);

//  gTAF->SetAlignStatus(2);
//  gTAF->AlignTracker( 100, 0, 10);
//  gTAF->DSFProduction(1000);

  gTAF->MimosaPro(10000000,100,0,0,0,2,"no");
//  gTAF->MimosaCalibration(1000,15.,0.,5000,0,1);
//  gTAF->MimosaClusterCharge("gaus");
  gTAF->MimosaBinaryplots()
  gTAF->CheckAsymmetry();
  gTAF->ClusterShape();
  gTAF->CheckClusters();
  gTAF->HitMap();
  // If tracking
  gTAF->MimosaResolution();
  gTAF->CheckMimosaAlign();
  gTAF->InspectEfficiency();
  gTAF->Checkreferencetracks();
  gTAF->MimosaPixelHomogeneity();

  //gTAF->GetRaw()->SkipEvent(1000);

  gApplication->Terminate();
}
