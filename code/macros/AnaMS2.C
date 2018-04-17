void AnaMS2(int aRun,int aPlane,int aStepMin,int aStepMax){
//MS2 jun06
   //gErrorAbortLevel=kSysError;
   //gErrorAbortLevel=kError;
   int aNEvent=50000;
   gMAF->InitSession(aRun,aPlane);

do {
   
if(aStepMin<= 1 && aStepMax>= 1)      {gMAF->MimosaPro(aNEvent,1500,5,2,1,0,"yes");  gMAF->Clear();aStepMin++;}
else if(aStepMin<= 2 && aStepMax>= 2) {gMAF->MimosaPro(aNEvent,400,5,2,1,1,"yes");  gMAF->Clear();aStepMin++;}
else if(aStepMin<= 3 && aStepMax>= 3) {gMAF->MimosaPro(aNEvent,1500,5,2,1,1,"yes");  gMAF->Clear();aStepMin++;}
else if(aStepMin<= 4 && aStepMax>= 4) {gMAF->MimosaPro(aNEvent,300,5,2,1,2,"yes");  gMAF->Clear();aStepMin++;}
else if(aStepMin<= 5 && aStepMax>= 5) {gMAF->MimosaPro(aNEvent,120,5,2,1,3,"yes");  gMAF->Clear();aStepMin++;}
else if(aStepMin<= 6 && aStepMax>= 6) 
{

gMAF->MimosaPro(aNEvent,120,5,2,1,3,"no");
gMAF->MimosaResolution();
gMAF->CheckMimosaAlign();
gMAF->MimosaClusterCharge();
gMAF->Checkreferencetracks();
gMAF->InspectEfficiency();
gMAF->CompareBadGoodRaw();
gMAF->SaveGifFile();
gMAF->ResultsStrore();


//gMAF->MimosaPro(aNEvent,150,5,2,1,3,"no");
//gMAF->Clear();

aStepMin++;
}
else {cout<<"\n\n\n Unknown Step\n\n\n"<<endl;}
} while (aStepMin<= 5 &&  aStepMax>= 5)

cout<<"\n\n\nThe end!!\n\n\n"<<endl;

}
