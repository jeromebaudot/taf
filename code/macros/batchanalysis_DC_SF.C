{
  // Example of macro that can be executed in batch mode
  // launch with: nohup TAF -b code/macros/batchnalysis.C > taf.out &
  // mote the "-b" option for batch (allows graphics to be created even if not displayed)
  //


//  gTAF->SetDebug(-2);

  gTAF->DSFProduction(1000000); // needed if DSF not produced already

  // One the following two command is mandatory
//  gTAF->MimosaCalibration(1000,15.,0.,5000,0,1);
  gTAF->MimosaCluster(1000000,7.,-5.,2,1);

  // The following is for display
  // if SavePlots is set to 1 in config file, a PDF will plots is saved
  gTAF->MimosaClusterCharge("gaus"); // use "gaus" with Fe source data and "landau" with beam data
//  gTAF->MimosaBinaryplots()
//  gTAF->CheckAsymmetry();
  gTAF->ClusterShape();
  gTAF->CheckClusters();
  gTAF->HitMap();
  gTAF->pixelcharge();
  gTAF->MimosaSN();
  gTAF->CheckClusters();

  gApplication->Terminate();
}
