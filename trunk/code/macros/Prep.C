void Prep(int aRun,int aPlane,int aStepMin,int aStepMax){
   //gErrorAbortLevel=kSysError;
   //gErrorAbortLevel=kError;
   int aNEvent=4500000;
   gMAF->InitSession(aRun,aPlane);

do {
   
if(aStepMin<= 1 && aStepMax>= 1)      {gMAF->MakeEta();  gMAF->Clear();aStepMin++;}
else if(aStepMin<= 2 && aStepMax>= 2) { gMAF->AlignTracker();  gMAF->Clear();aStepMin++;}
else if(aStepMin<= 3 && aStepMax>= 3) { gMAF->DSFProduction(aNEvent);  gMAF->Clear();aStepMin++;}
else {cout<<"\n\n\n Unknown Step\n\n\n"<<endl;}
} while (aStepMin<= 5 &&  aStepMax>= 5)

cout<<"\n\n\nThe end!!\n\n\n"<<endl;

}
