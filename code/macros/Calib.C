void Calib(int aRun,int aPlane,int aStepMin,int aStepMax)
{	
//MS2 juin06
   //gErrorAbortLevel=kSysError;
   //gErrorAbortLevel=kError;
   int aNEvent=40000;
   gMAF->InitSession(aRun,aPlane);

do {
   
if(aStepMin<= 1 && aStepMax>= 1)  {gMAF->MimosaCalibration(aNEvent,15); aStepMin++;}
else {cout<<"\n\n\n Unknown Step\n\n\n"<<endl;}

} while (aStepMin<= 5 &&  aStepMax>= 5)

cout<<"\n\n\nThe end!!\n\n\n"<<endl;

}
