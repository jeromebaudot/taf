{
/////////////////////////////////////////  
//         INIT DISPLAYS
/////////////////////////////////////////
  //gTAF->GetRax()->DisplayPixelsInCmosFrameInPixel_Init();
  //gTAF->GetRax()->DisplayNoiseStudy_Init();
  //gTAF->GetRax()->DisplayPixelsInFirstBoardFrameInPixel_Init();
  
  //gTAF->GetRax()->DisplayHitsInCmosFrameInPixel_Init();
  //gTAF->GetRax()->DisplayHitsInFirstBoardFrameInPixel_Init();
  
  //gTAF->GetRax()->DisplayHitsInTrackerFrameInUm_Init();
  //gTAF->GetRax()->DisplayHitsInTrackerFrameInUmOverlayed_Init();
  
  //gTAF->GetRax()->DisplayTracksIn3D_Init();
  //gTAF->GetRax()->DisplayVerticesIn3D_Init();
  
  //gTAF->GetRax()->DisplayCmosOverFlow_Init();
  //gTAF->GetRax()->DisplayClustersSize_Init(/*Int_t nbBinX*/ 30);
  //gTAF->GetRax()->DisplayHitsByEvent_Init(/*Int_t nbBinX*/100);
  //gTAF->GetRax()->DisplayHitsTracksAssociation_Init(/*Int_t nbBinX*/ 30);
  //gTAF->GetRax()->DisplayTracksAndTriggersProperties_Init(/*Bool_t withTriggers=*/0, /*Int_t Chi2Max=*/10000,/*Int_t SlopeMax=*/30);
  //gTAF->GetRax()->DisplayChronogram_Init(/*Bool_t withTriggers*/0);
  
  
  
/////////////////////////////////////////  
//              PROCESS
/////////////////////////////////////////
  //ProcessEvents(Int_t aNbOfEvents, Bool_t resetBefore, Bool_t showDisplays)
  gTAF->GetRax()->ProcessEvents(100000,1,0);
  
  
  
/////////////////////////////////////////  
//           DISPLAYS
/////////////////////////////////////////
  //gTAF->GetRax()->DisplayPixelsInCmosFrameInPixel_Show(/*Bool_t WithHits*/0, /*Bool_t WithOverlapLine*/0, /*Bool_t WithZbar*/0,/* Int_t minPixOccu*/0);
  //gTAF->GetRax()->DisplayNoiseStudy_Show(/*Float_t relOccur_min*/0.00001, /*Float_t relOccur_max*/1, /*UInt_t relOccur_steps*/20, /*Bool_t xAxisLog0_Lin1*/0);
  //gTAF->GetRax()->DisplayPixelsInFirstBoardFrameInPixel_Show(/*Bool_t WithHits*/0, /*Bool_t WithOverlapLine*/0, /*Bool_t WithZbar*/1, /*Int_t minPixOccu*/0);
  
  //gTAF->GetRax()->DisplayHitsInCmosFrameInPixel_Show(/*Bool_t ShowAsso2TrackHits*/0, /*Bool_t WithOverlapLine*/0, /*Bool_t HitShowedAsPix*/1, /*Int_t minHitOccu*/0);
  //gTAF->GetRax()->DisplayHitsInFirstBoardFrameInPixel_Show(/*Bool_t ShowAsso2TrackHits*/0, /*Bool_t WithOverlapLine*/0, /*Bool_t HitShowedAsPix*/1, /*Int_t minHitOccu*/0);
  
  //gTAF->GetRax()->DisplayHitsInTrackerFrameInUm_Show(/*Int_t TrackDisplayMode*/0);
  //gTAF->GetRax()->DisplayHitsInTrackerFrameInUmOverlayed_Show(/*Bool_t *planes2display*/NULL);
  
  //gTAF->GetRax()->DisplayTracksIn3D_Show(/*Bool_t DrawTracks*/1, /*Bool_t DrawSensors*/1, /*Bool_t DrawTrackedHits*/1, /*Bool_t DrawAllHits*/1);
  //gTAF->GetRax()->DisplayVerticesIn3D_Show(/*Bool_t DrawSensors*/1, /*Bool_t DrawTracks*/1, /*Bool_t DrawTrackerDir*/1, /*Bool_t DrawTarget*/1, /*Bool_t DrawBeam*/1, /*Bool_t DrawVerticesPoints*/1, /*Bool_t DrawVerticesPointsSmall*/1, /*Bool_t DrawAcceptance*/0);
  
  //gTAF->GetRax()->DisplayCmosOverFlow_Show();
  //gTAF->GetRax()->DisplayClustersSize_Show(/*Bool_t All*/1, /*Bool_t Tracked*/0, /*Bool_t Untracked*/0);
  //gTAF->GetRax()->DisplayHitsByEvent_Show();
  //gTAF->GetRax()->DisplayHitsTracksAssociation_Show();
  //gTAF->GetRax()->DisplayTracksAndTriggersProperties_Show();
  //gTAF->GetRax()->DisplayChronogram_Show();
  
  
  
  
/////////////////////////////////////////
//              SAVE
/////////////////////////////////////////
  gTAF->GetRax()->SaveAllDisplays("pdf","root","png");
  gTAF->GetRax()->SaveAllHistograms();
  gTAF->GetRax()->PrintLogFile();
  
  
}