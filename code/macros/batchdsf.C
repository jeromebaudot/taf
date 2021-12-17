void batchdsf(int nEvents=10000000)
{	

   //gMAF->InitSession(aRun,aPlane);
   gTAF->DSFProduction( nEvents);

  gApplication->Terminate();

}
