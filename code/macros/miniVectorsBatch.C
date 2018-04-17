void miniVectorsBatch(Int_t numberOfGoodEvents, Int_t eventNumber, Int_t runNumber)
{
  // LC 2014/02/17

     MimosaAnalysis *gTAF = new MimosaAnalysis();
  
     gTAF->InitSession(runNumber);

     gTAF->GetRaw()->SkipEvent(eventNumber); // retourne au dernier evt de la derniere minimisation :)
     gTAF->AlignLadderMV(1, 800, 30, numberOfGoodEvents, eventNumber, 100, 20, 0);
     
     gTAF->Exit();

     MimosaAnalysis *gTAF = new MimosaAnalysis();
  
     gTAF->InitSession(runNumber);

     gTAF->GetRaw()->SkipEvent(eventNumber); // retourne au dernier evt de la derniere minimisation :)
     gTAF->AlignLadderMV(1, 65, 20, numberOfGoodEvents, eventNumber, 100, 20, 0);
     
     gTAF->Exit();

}
