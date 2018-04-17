{
  // Example of macro that can be executed in batch mode
  // launch with: nohup TAF -b code/macros/batchrun.C > taf.out &
  // mote the "-b" option for batch (allows graphics to be created even if not displayed)
  //
  // JB 2009/10/06

//  gTAF->GetRaw()->DisplayImage(20000,3,"raw","tiff",1,1);
  
//  MimosaAnalysis *gTAF = new MimosaAnalysis();
  
//  gTAF->InitSession(18007);
  
  //gTAF->SetDebug(2);
  //gTAF->SetAlignStatus(2);
  //gTAF->GetRaw()-SkipEvent(1000);
  //gTAF->AlignTracker( 100, 0, 10);
//  gTAF->DSFProduction(500);

  //gTAF->MimosaPro(130000,500,6,0,0,2,"no");
  
  //gTAF->GetRaw()->SkipEvent(1000);
//  gTAF->GetRaw()->DisplayCumulatedRawData2D(10000000);
  gTAF->GetRaw()->DisplayCumulatedHits2D(10000000);
  
//  exit;
}
