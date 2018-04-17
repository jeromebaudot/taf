void DSFProd(int aRun,int aPlane,int aNEvent)
{	
  cout <<"nDSF Production for run "<<aRun<<" plane "<<aPlane<<" with "<<aNEvent<<" events"<<endl;

   gMAF->InitSession(aRun,aPlane);
   gMAF->DSFProduction(aNEvent);

   cout<<"\n\n\nThe end!!\n\n\n"<<endl;

}
