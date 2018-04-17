// @(#)maf/maf:$Name:  $:$Id: MPrep.cxx v.1 2005/10/02 18:03:46 sha Exp $
// Author: A. Shabetai 
// Last modified: JB 2009/05/12
// Last modified: RDM 2009/08/26 removed request that there is only one track AND loop over all hits
// Last modified: JB 2009/09/14 InitCorPar
// Last modified: JB 2011/04/12 InitSession
// Last modified: JB 2011/07/07 to localize path names
// Last Modified: JB 2011/07/21 introduce fSession member pointer
// Last Modified: SS 2011/11/14 InitSession
// Last Modified: JB 2012/04/25 InitSession
// Last Modified: JB 2013/06/10 AlignTracker
// Last Modified: JB 2013/08/19 MakeEta
// Last Modified: JB 2014/01/10 InitCorPar
// Last Modified: VR 2014/06/30 InitSession and others fo ChangeDirectory or gSystem->cd
// Last modified: VR 2014/08/15 add OutputFilesPrefix and OutputFilesSuffix
// Last modified: AP 2015/06/08 added bool parameter (UseAllHits) to AlignTrackerMinuit to decide if doing alignment with all hits or the closest one
// Last modified: BB 2015/07/28 Add StudyDeformation to parametrize deviation of the track-hit residual
//
  /////////////////////////////////////////////////////////////
  //                                                         //
  //  Contains all methods to prepare an analysis with MAF   //
  //                                                         //
  //                                                         //   
  ////////////////////////////////////////////////////////////


#include "MAnalysis.h"

ClassImp(MimosaAnalysis) 

//______________________________________________________________________________
//
void  MimosaAnalysis::SetDebug(Int_t aDebug)
{
  // Always update the debug level for the session,
  // but only update positive levels for the analysis.
  //
  // JB 2010/12/8

  cout << "testing fSession pointer " << fSession << endl;
  if( fSession!=NULL ) fSession->SetDebug( aDebug);

  if( aDebug>=0 ) {
    MimoDebug  = aDebug;
    cout << "MimosaAnalysis debug updated to " << MimoDebug << endl;
  }

}

//_____________________________________________________________________________
  void MimosaAnalysis::InitSession(const Int_t TheRun,const unsigned int  ThePlane/*=0*/, Int_t EventBuildingMode, TString ConfigFile/*=""*/, TString ConfigDir/*=""*/) 
{

  // initialization of TAF
  // fSession is a global & static object, see DSession.h   
  //
  // Last modified JB 2009/05/12
  // Last modified JB 2011/04/12
  // Last Modified JB 2011/07/07 to localize path names
  // Last Modified SS 2011/11/14 to set event building mode
  // Last modified JB 2012/04/25 better management of debug mode
  // Last modified JB 2013/08/22 added ROOT display options
  // Last modified VR 2014/06/30 rewrite the files and directories management, especialy fot the config dir/file
  // Last modified JB 2016/08/17 added config directory option
  
  fSession = new DSession("options");
  
  //*******************************
  // Files and directories
  //*******************************
  TString aPath;
  //--------------
  // Working Directory
  //--------------
  //TAF installation dir.
  DTDIR = gSystem->Getenv("DTDIR"); // The TAF installation directory, set via environment variable
  DTDIR += "/";
  fTool.LocalizeDirName(&DTDIR);
  
  //TAF launched dir.
  fLaunchedDirectory = gSystem->WorkingDirectory();// The dir. where has been launched
  fLaunchedDirectory += "/";
  fTool.LocalizeDirName(&fLaunchedDirectory);
  
  // TAF Working Directory  
  fWorkingDirectory = fLaunchedDirectory;  
  //fWorkingDirectory = DTDIR;
  
  cout << " * TAF is working in: " << fWorkingDirectory << endl; 

    //--------------
  // Config directory
  //--------------
  if (ConfigDir.IsNull())// No ConfigDir specified, set the default one
  {
    cout << "Option with no config dir " << endl;
    aPath = DTDIR;
    aPath += "config/";
    fTool.LocalizeDirName(&aPath);
    fSession->SetConfigPath(aPath);
  }
  else
  {
    if(ConfigDir.First("/")==0) // ConfigDir String start with a "/" : it's an absolute path
    {
      fSession->SetConfigPath(ConfigDir);
    }
    else // the path is relative to the launch directory
    {
      aPath = fLaunchedDirectory;
      aPath += "/";
      aPath += ConfigDir;
      aPath += "/";
      fSession->SetConfigPath(aPath);
    }
  }
  
  //--------------
  // Config File
  //--------------
  if (ConfigFile.IsNull())// No ConfigFile specified, set the default one
  {    
    aPath = "run";
    aPath += TheRun;
    aPath +=".cfg";
    fTool.LocalizeDirName(&aPath);
    fSession->SetConfigFileName(aPath);
  }
  else
  {
    if (!ConfigFile.Contains("/")) //ConfigFile String do not contains a "/" : it's a file in the launch directly
    {
      fSession->SetConfigPath(fLaunchedDirectory);
      fSession->SetConfigFileName(ConfigFile);
    }
    else if (ConfigFile.Index("./")==0 ) //ConfigFile String starts with ./ and do not contains others /  it's a file in the launch directly
    {
      ConfigFile.Remove(0,2);
      fSession->SetConfigPath(fLaunchedDirectory);
      fSession->SetConfigFileName(ConfigFile);
    }
    else if(ConfigFile.First("/")==0) // ConfigFile String start with a "/" : it's an absolute path
    {
      TString TempConfigPath = "/";
      ConfigFile.Remove(0,1);

      while (ConfigFile.First("/")>0)
      {
        TempConfigPath += ConfigFile(0,ConfigFile.First("/")+1);
        ConfigFile.Remove(0,ConfigFile.First("/")+1);
      }
      fSession->SetConfigFileName(ConfigFile);
      fSession->SetConfigPath(TempConfigPath);
    }
    else if(ConfigFile.Index("../")==0) // ConfigFile String start with a "../" : it's a relative path to the current one
    {
      TString TempConfigPath = fLaunchedDirectory;
      TempConfigPath += "../";
      ConfigFile.Remove(0,3);
      
      while (ConfigFile.First("/")>0)
      {
        TempConfigPath += ConfigFile(0,ConfigFile.First("/")+1);
        ConfigFile.Remove(0,ConfigFile.First("/")+1);
      }
      fSession->SetConfigFileName(ConfigFile);
      fSession->SetConfigPath(TempConfigPath);
    }
  }
  cout << " * config file is: " << fSession->GetConfigFileName() << endl;
  cout << "               in: " << fSession->GetConfigPath()<< endl;
  
  //--------------
  // Outputs Files and Dir.
  //--------------
  aPath = fWorkingDirectory;
  aPath += "datDSF/";
  fTool.LocalizeDirName(&aPath);
  fSession->SetSummaryFilePath(aPath);
  
  aPath = fWorkingDirectory;
  aPath += "Results/";
  fSession->SetResultsDirParent(aPath);
  aPath += TheRun;
  aPath += "/";
  fTool.LocalizeDirName(&aPath);
  fSession->SetResultsDir(aPath); // JB 2011/04/12
  
  //TString WeightFile = fSession->GetResultDirName();
  //WeightFile += "eta";
  //WeightFile += TheRun;
  //WeightFile += ".root";
  //fTool.LocalizeDirName( &WeightFile); // JB 2011/07/07
  //fSession->SetWeightFile( WeightFile); // JB 2011/04/12

  
  fSession->SetRunNumber(TheRun);
  
  fSession->SetPlaneNumber(ThePlane);  
  
  fSession->SetEventBuildingMode( EventBuildingMode); //SS 2011.11.14 
  
  fSession->SetDebug(MimoDebug); // needed before init, JB 2012/04/25
  
  fSession->InitSession(); // call the initialization

  //gSystem->ChangeDirectory(fWorkingDirectory); // chdir to the correct dir to run
 
  InitRunNumber();
  InitPlaneNumber(); 
  InitMimosaType();
  CorStatus = 0; // JB, 2009/05/19

  // ROOT display default options, JB 2013/08/22
  gStyle->SetPalette(1);
  gStyle->SetOptStat(1111111);
  gStyle->SetOptFit(11111);

  cout<<"Run Number is "<<fSession->GetRunNumber()<<endl;
  cout<<"Plane number to study is "<<fSession->GetPlaneNumber()<<endl;
  
  fInitDone = kTRUE;

}

//______________________________________________________________________________

void MimosaAnalysis::InitCorPar(Int_t aRun, Int_t aPlane, const char* objType)
{
  // Initialize the CorPar file (precision alignment and eta functions for the Mimosa plane)
  // Variable CorStatus is also set:
  // 0->existing file is used, 1->template file is used, 2->new file is used
  // 
  // The path including the directory eta was suppressed (JB 2009/05/19)
  //  due to unexplained crash (?!?)
  // CorPar file goes now into the "global result" directory, JB 2009/09/14
  //
  // Modified: JB 2011/07/07 to localize path names
  // Modified: JB 2014/01/10 deal with objType option

  if(!CheckIfDone("init")) return; // return 0
   
  if (MimoDebug) {
    Info( "InitCorPar", "Openint CorPar file for option %s (if empty=plane)\n", objType);
  }
  
  Char_t CorFileName[200];
  if( strstr( objType, "ladder" ) ) { // JB 2014/01/10
    sprintf(CorFileName,"%s/CorPar%d_Lader%d.root",CreateGlobalResultDir(),aRun,aPlane);
  }
  else {
    sprintf(CorFileName,"%s/CorPar%d_%d.root",CreateGlobalResultDir(),aRun,aPlane); // JB 2011/04/12
  }
    sprintf(CorFileName,"%s", fTool.LocalizeDirName( CorFileName)); // JB 2011/07/07
  
  // take a file with ETA distribution   
  if (!gSystem->AccessPathName(CorFileName)) 
    {
      theCorFile= new TFile(CorFileName, "UPDATE");
      cout << "== Old  CorParFile found! Let's use it : "<< CorFileName << endl;
      CorStatus = 0;
    }
  else if (!gSystem->AccessPathName("config/CorParDef.root"))  
    {
      cout << "==No  dedicated CorParFile but CorParDef.root is used to create a new one." << endl;
      cout << "Its name will be : " << CorFileName  <<endl;
      gSystem->CopyFile("config/CorParDef.root", CorFileName); 
      theCorFile= new TFile(CorFileName, "UPDATE");
      CorStatus = 1;
    }
  else {
    cout << "==No dedicated CorParFile! Creating a new one from scratch" << endl;
    cout << "Its name will be :" << CorFileName  <<endl;
    theCorFile = new TFile(CorFileName,"UPDATE");
    CorStatus = 2;
  }

  if(MimoDebug) cout << "Testing if Zombie and Opened...";
  if (theCorFile->IsZombie() || !theCorFile->IsOpen()) {
    theCorFile->Close();
    Fatal("InitCorPar","BadCorPar File!!");
  }
  
  // added JB 2011/04/12
  if(MimoDebug) cout << "Testing if some keys are there:" << theCorFile->GetNkeys() << " ...";
  if (theCorFile->GetNkeys() == 0) { // if no keys, set as new file
    cout << "There is nothing in the existing CorPar file, treated as new." << endl;
    CorStatus =2; 
  }
  
  if(MimoDebug) cout << "done, CorStatus = " << CorStatus << endl;
  //return theCorFile; // JB 2009/05/18
}


//______________________________________________________________________________
void MimosaAnalysis::MakeEta(int NEvt)
{
  // creates new eta-functions for the all planes 
  // requires init Session calls in order to run!
  //
  // modified by JB to have 2 kinds of eta correction, November 2007
  // modified by JB 2011/07/08 change output directory and localize file name
  // modified by JB 2013/08/19 include all analog output planes (not only telescope)

  if(!CheckIfDone("init")) return;

  DTracker *tTracker  =  fSession->GetTracker();
  Int_t nTotalPlanes  = tTracker->GetPlanesN();
  
  // Allow a variable number of planes, JB 2007 June
  // maximum #planes adjusted to tracker size, JB 2012/08/28
  Int_t* planeNo = new Int_t[nTotalPlanes];//[8]={0,0,0,0,0,0,0,0};
  Int_t* padNo   = new Int_t[nTotalPlanes];//[8]={0,0,0,0,0,0,0,0};

  DPlane**  RSplane = new DPlane*[nTotalPlanes];//[8]={0,0,0,0,0,0,0,0};
  Float_t* pitchU = new Float_t[nTotalPlanes];//[8]={0,0,0,0,0,0,0,0};
  Float_t* pitchV = new Float_t[nTotalPlanes];//[8]={0,0,0,0,0,0,0,0};

  TH1F** hEtaDen = new TH1F*[nTotalPlanes];//[8]={0,0,0,0,0,0,0,0}; // strips
  TH1F** hEtaInt = new TH1F*[nTotalPlanes];//[8]={0,0,0,0,0,0,0,0};
  TH1F** hEtaDenU = new TH1F*[nTotalPlanes];//[8]={0,0,0,0,0,0,0,0};// histos for CoG 3x3
  TH1F** hEtaIntU = new TH1F*[nTotalPlanes];//[8]={0,0,0,0,0,0,0,0};
  TH1F** hEtaDenV = new TH1F*[nTotalPlanes];//[8]={0,0,0,0,0,0,0,0};
  TH1F** hEtaIntV = new TH1F*[nTotalPlanes];//[8]={0,0,0,0,0,0,0,0};
  TH1F** hEtaDenU2 = new TH1F*[nTotalPlanes];//[8]={0,0,0,0,0,0,0,0}; // histo for CoG 2x2
  TH1F** hEtaIntU2 = new TH1F*[nTotalPlanes];//[8]={0,0,0,0,0,0,0,0};
  TH1F** hEtaDenV2 = new TH1F*[nTotalPlanes];//[8]={0,0,0,0,0,0,0,0};
  TH1F** hEtaIntV2 = new TH1F*[nTotalPlanes];//[8]={0,0,0,0,0,0,0,0};
  TString hPid;
  TString hName;

  // create histograms:
  // different ones depending on strips or pixels, JB, Sept 2008
  Int_t padCount=1;
  for(Int_t iPl=0; iPl<nTotalPlanes; iPl++) {

    // get planes and their number in the tracker
    if( iPl<tTracker->GetNFixRef()  ) {
      planeNo[iPl] = tTracker->GetListFixRef()[iPl];
    }
    else {
      planeNo[iPl] = tTracker->GetListVarRef()[iPl-tTracker->GetNFixRef()];
    }
    planeNo[iPl] = tTracker->GetPlane(iPl+1)->GetPlaneNumber();
    if(MimoDebug) printf("MPrep::MakeEta Getting plane %d as %d\n", iPl, planeNo[iPl]);
    RSplane[iPl] = tTracker->GetPlane(planeNo[iPl]);

    if( RSplane[iPl]->GetAnalysisMode()==1 ) { // STRIPS
      hPid  = TString("HEtaDen") + long(iPl);
      hName = TString("Eta Density Pl ") + long(iPl+1);
      hEtaDen[iPl]= new TH1F(hPid.Data(),hName.Data(),700,-0.2,1.2);
      hPid  = TString("HEtaDenPk") + long(iPl+1);
      hName = TString("Eta Integrated Pl ") + long(iPl+1);
      hEtaInt[iPl]= new TH1F(hPid.Data(),hName.Data(),700,-0.2,1.2);
            
      padNo[iPl] = padCount;
      padCount += 1; // only one pad needed
    }
    else { // PIXELS
      Int_t nBins = 100;
      
      pitchU[iPl] = RSplane[iPl]->GetStripPitch()(0);
      // 3x3
      hPid  = TString("HEtaDenU") + long(planeNo[iPl]);
      hName = TString("Eta Density U Pl ") + long(planeNo[iPl]);
      hEtaDenU[iPl]= new TH1F(hPid.Data(),hName.Data(),nBins,-pitchU[iPl]/2.,pitchU[iPl]/2.);
      hPid  = TString("HEtaIntUPk") + long(planeNo[iPl]);
      hName = TString("Eta Integrated U Pl ") + long(planeNo[iPl]);
      hEtaIntU[iPl]= new TH1F(hPid.Data(),hName.Data(),nBins,-pitchU[iPl]/2.,pitchU[iPl]/2.);
      // 2x2
      hPid  = TString("HEtaDen2U") + long(planeNo[iPl]);
      hName = TString("Eta Density U 2x2 Pl ") + long(planeNo[iPl]);
      hEtaDenU2[iPl]= new TH1F(hPid.Data(),hName.Data(),nBins,-pitchU[iPl]/2.,pitchU[iPl]/2.);
      hPid  = TString("HEtaInt2UPk") + long(planeNo[iPl]);
      hName = TString("Eta Integrated U 2x2 Pl ") + long(planeNo[iPl]);
      hEtaIntU2[iPl]= new TH1F(hPid.Data(),hName.Data(),nBins,-pitchU[iPl]/2.,pitchU[iPl]/2.);

      pitchV[iPl] = RSplane[iPl]->GetStripPitch()(1);
      // 3x3
      hPid  = TString("HEtaDenV") + long(planeNo[iPl]);
      hName = TString("Eta Density V Pl ") + long(planeNo[iPl]);
      hEtaDenV[iPl]= new TH1F(hPid.Data(),hName.Data(),nBins,-pitchV[iPl]/2.,pitchV[iPl]/2.);
      hPid  = TString("HEtaIntVPk") + long(planeNo[iPl]);
      hName = TString("Eta Integrated V Pl ") + long(planeNo[iPl]);
      hEtaIntV[iPl]= new TH1F(hPid.Data(),hName.Data(),nBins,-pitchV[iPl]/2.,pitchV[iPl]/2.);
      // 2x2
      hPid  = TString("HEtaDen2V") + long(planeNo[iPl]);
      hName = TString("Eta Density V 2x2 Pl ") + long(planeNo[iPl]);
      hEtaDenV2[iPl]= new TH1F(hPid.Data(),hName.Data(),nBins,-pitchV[iPl]/2.,pitchV[iPl]/2.);
      hPid  = TString("HEtaInt2VPk") + long(planeNo[iPl]);
      hName = TString("Eta Integrated V 2x2 Pl ") + long(planeNo[iPl]);
      hEtaIntV2[iPl]= new TH1F(hPid.Data(),hName.Data(),nBins,-pitchV[iPl]/2.,pitchV[iPl]/2.);

      padNo[iPl] = padCount;
      padCount += 2; // two pads needed
    }

  } // end loop on ref planes
  padCount--; // exact number of pads needed

  // desitnation directory changed to Results, JB 2011/07/08
  Char_t EtaFileName[200];
  sprintf(EtaFileName,"%s/eta%d.root",fSession->GetResultDirName().Data(),fSession->GetRunNumber());
  sprintf(EtaFileName,"%s", fTool.LocalizeDirName( EtaFileName));
  TString Orig =  TString(EtaFileName);
  fTool.LocalizeDirName( &Orig);
  TString Dest =  fSession->GetResultDirName();
  Dest += "/inf" + Orig(10,Orig.Length());
  fTool.LocalizeDirName( &Dest);

  // take a file with ETA distribution   
  if (!gSystem->AccessPathName(EtaFileName)) 
    {
      cout << "== Old file found! : "<< EtaFileName<<endl;
      TFile TMP(EtaFileName);
      if (TMP.GetNkeys() && !TMP.IsZombie()) {
	cout<<"Creating a backup "<< endl;
	gSystem->CopyFile(EtaFileName ,TString(TString(EtaFileName) + ".back").Data(),1);     
      }
      TMP.Close();
    }
  else {
    cout << "File doesn't exist, creating it from scratch..." << endl;
  }
  // cout<<"Copying "<< EtaFileName  <<" to "<< Dest.Data() <<endl;


 Char_t tWeightFileName[200];
 sprintf(tWeightFileName,"%s/inf%d.root",fSession->GetResultDirName().Data(),fSession->GetRunNumber()); 
 sprintf(tWeightFileName,"%s", fTool.LocalizeDirName( tWeightFileName)); // JB 2011/07/07
 cout<<"Backing-up : " << tWeightFileName <<endl; 
 gSystem->Rename(tWeightFileName ,TString(TString(tWeightFileName) + ".back").Data());   

 TFile f(EtaFileName,"RECREATE");

 //tTracker->SetAlignmentStatus(2); // NOT needed here, we don't care yet about track, JB Sept 2008
  // 1 = four reference planes, 2 = all reference planes

 
  fSession->SetEvents(NEvt); 

  TCanvas *EtaFunction = new TCanvas("Eta","Eta functions",0,0,600,600);
  EtaFunction->cd();
  // -- start read events:
  while(fSession->NextRawEvent() == kTRUE) {    

    // select events with 1 track found, take principal hits in each plane, build eta distributions for clusters
    tTracker->Update();
    //if ( fSession->GetCurrentEventNumber()% 50 == 0 ) printf ("%d Event Read\n", fSession->GetCurrentEventNumber() ); // useless since printout in DSession:NextRawEvent(), JB 2008/10/22
    if (tTracker->GetPlanesStatus() >=1) {
      if(fSession->GetStatus()==0) {
      	fSession->SetStatus(1);
	cout << "\n\n Session Status changed to 1 \n\n" ;
      }

      DHit *ahit=0;
      Float_t leftSignal,rightSignal,chargeFraction;
      leftSignal=rightSignal=chargeFraction=0.;
      
      for(Int_t iPl=0; iPl<nTotalPlanes; iPl++){ // loop on planes
	//RSplane[iPl]= tTracker->GetPlane(iPl+1); // moved above, JB, Sept 2008
        Int_t nHits=RSplane[iPl]->GetHitsN();
        
	//cout << "nHits for Plane " << iPl+1 << " is " << nHits << endl;
	
	if(nHits>0 && RSplane[iPl]->GetAnalysisMode()!=3 ){ //ensure there is a hit and no binary output (JB 2013/08/19)
	  
	  for(Int_t iHit=1; iHit<=nHits; iHit++){ //RDM260809	    
	    ahit=RSplane[iPl]->GetHit(iHit);   //RDM260809 from GetPrincipalHit to GetHit
	    
	    //==============================
	    if( RSplane[iPl]->GetAnalysisMode()==1 ) { // STRIPS
	      leftSignal=ahit->GetPulseHeightLeft();
	      rightSignal=ahit->GetPulseHeightRight();
	      if(leftSignal+rightSignal!=0){
		chargeFraction=leftSignal/(leftSignal+rightSignal);
		
		hEtaDen[iPl]->Fill(chargeFraction,1.);
	      }
	    }
	    //==============================
	    else { // PIXELS
	      hEtaDenU[iPl]->Fill(ahit->GetPositionUhitCG33()-ahit->GetSeedU());
	      hEtaDenV[iPl]->Fill(ahit->GetPositionVhitCG33()-ahit->GetSeedV());
	      hEtaDenU2[iPl]->Fill(ahit->GetPositionUhitCG22()-ahit->GetSeedU());
	      hEtaDenV2[iPl]->Fill(ahit->GetPositionVhitCG22()-ahit->GetSeedV());
	    }
	  }//end of loop on hits RDM260809
	} //end of ensure there is a hit
      } //end loop on planes
    }
  }  // end of while nextrawevent

  EtaFunction->Divide((Int_t)ceil((Float_t)padCount/4),4);
  for(Int_t iPl=0; iPl<nTotalPlanes; iPl++){
    //==============================
    if( RSplane[iPl]->GetAnalysisMode()==1 ) { // STRIPS
      EtaFunction->cd(padNo[iPl]);
      gPad->SetFillColor(0);
      hEtaDen[iPl]->Draw();
      hEtaDen[iPl]->Write();
    }
    //==============================
    else if( RSplane[iPl]->GetAnalysisMode()!=3 ) { // PIXELS
      EtaFunction->cd(padNo[iPl]);
      gPad->SetFillColor(0);
      hEtaDenU[iPl]->Draw();
      hEtaDenU[iPl]->Write();
      EtaFunction->cd(padNo[iPl]+1);
      gPad->SetFillColor(0);
      hEtaDenV[iPl]->Draw();
      hEtaDenV[iPl]->Write();

      hEtaDenU2[iPl]->Write();
      hEtaDenV2[iPl]->Write();
    }
  } 
  EtaFunction->Update();
  TCanvas *EtaFunction2 = new TCanvas("Eta2","Integrated Eta functions",0,0,600,600);
  EtaFunction2->Divide((Int_t)ceil((Float_t)padCount/4),4);

  Float_t content,total,content2,total2;
  content=total=content2=total2=0.;

  for(Int_t iPl=0; iPl<nTotalPlanes; iPl++){ // loop on planes

    //==============================
    if( RSplane[iPl]->GetAnalysisMode()==1 ) { // STRIPS
      total=hEtaDen[iPl]->Integral(0,700);
      if(total!=0){
	for(Int_t i=0; i<700; i++){
	  content=hEtaDen[iPl]->Integral(0,i);
	  hEtaInt[iPl]->SetBinContent(i,content/total);
	}    
      }else{
	Info("MakeEta()","WARNING.... hEtaDen %d is empty",iPl);
      }

      EtaFunction2->cd(padNo[iPl]);
      gPad->SetFillColor(0);
      hEtaInt[iPl]->Draw();
      hEtaInt[iPl]->Write();
    }

    //==============================
    else if( RSplane[iPl]->GetAnalysisMode()!=3 ) { // PIXELS
      // 3x3
      total=hEtaDenU[iPl]->Integral(1,hEtaDenU[iPl]->GetNbinsX());
      if(total!=0){
	for(Int_t i=1; i<hEtaDenU[iPl]->GetNbinsX(); i++){
	  content=hEtaDenU[iPl]->Integral(1,i);
	  hEtaIntU[iPl]->SetBinContent(i,content/total*pitchU[iPl]-pitchU[iPl]/2.);
	}    
      }else{
	Info("MakeEta()","WARNING.... hEtaDenU %d is empty",iPl);
      }
      total=hEtaDenV[iPl]->Integral(1,hEtaDenV[iPl]->GetNbinsX());
      if(total!=0){
	for(Int_t i=1; i<hEtaDenV[iPl]->GetNbinsX(); i++){
	  content=hEtaDenV[iPl]->Integral(1,i);
	  hEtaIntV[iPl]->SetBinContent(i,content/total*pitchV[iPl]-pitchV[iPl]/2.);
	}    
      }else{
	Info("MakeEta()","WARNING.... hEtaDenV %d is empty",iPl);
      }

      EtaFunction2->cd(padNo[iPl]);
      gPad->SetFillColor(0);
      hEtaIntU[iPl]->Draw();
      hEtaIntU[iPl]->Write();
      EtaFunction2->cd(padNo[iPl]+1);
      gPad->SetFillColor(0);
      hEtaIntV[iPl]->Draw();
      hEtaIntV[iPl]->Write();

      // 2x2
      total2=hEtaDenU2[iPl]->Integral(1,hEtaDenU2[iPl]->GetNbinsX());
      if(total2!=0){
	for(Int_t i=1; i<hEtaDenU2[iPl]->GetNbinsX(); i++){
	  content2=hEtaDenU2[iPl]->Integral(1,i);
	  hEtaIntU2[iPl]->SetBinContent(i,content2/total2*pitchU[iPl]-pitchU[iPl]/2.);
	}    
      }else{
	Info("MakeEta()","WARNING.... hEtaDenU2 %d is empty",iPl);
      }
      hEtaIntU2[iPl]->Write();
      total2=hEtaDenV2[iPl]->Integral(1,hEtaDenV2[iPl]->GetNbinsX());
      if(total2!=0){
	for(Int_t i=1; i<hEtaDenV2[iPl]->GetNbinsX(); i++){
	  content2=hEtaDenV2[iPl]->Integral(1,i);
	  hEtaIntV2[iPl]->SetBinContent(i,content2/total2*pitchV[iPl]-pitchV[iPl]/2.);
	}    
      }else{
	Info("MakeEta()","WARNING.... hEtaDenV2 %d is empty",iPl);
      }
      hEtaIntV2[iPl]->Write();
    }

  } // end loop on planes

  cout <<"gonna write in file..."<< endl;
  f.Write();
  f.Close();
  cout<<"make eta fonction done"<<endl;

  cout<<"Copying "<< EtaFileName  <<" to "<< Dest.Data() <<endl;
  gSystem->CopyFile(Orig.Data() ,Dest.Data(),1); 
  
// save all open canvas in a root file
//cd to result dir (JB 2011/07/08)
 gSystem->cd(fSession->GetResultDirName().Data());
 if(MimoDebug) cout<<"Curent Dir : "<<gSystem->pwd()<<endl; 
 Char_t Header[100];
 sprintf(Header,"EtaRS_%d.root",fSession->GetRunNumber());
 sprintf(Header,"%s", fTool.LocalizeDirName( Header)); // JB 2011/07/07

 TFile* AllRSPlots = new TFile(Header,"RECREATE");
 gROOT->GetListOfCanvases()->Write();
 AllRSPlots->Close(); delete AllRSPlots;
 gSystem->cd(fWorkingDirectory);// VR 2014/06/30 replace DTIR by fWorkingDirectory
  
  if (gROOT->IsBatch())
    {
      for(Int_t iPl=0; iPl<nTotalPlanes; iPl++){
	if( RSplane[iPl]->GetAnalysisMode()==1 ) { // STRIPS
	  delete hEtaDen[iPl];
	  delete hEtaInt[iPl];
	}
	else { // PIXELS
	  delete hEtaDenU[iPl];
	  delete hEtaIntU[iPl];
	  delete hEtaDenV[iPl];
	  delete hEtaIntV[iPl];
	  delete hEtaDenU2[iPl];
	  delete hEtaIntU2[iPl];
	  delete hEtaDenV2[iPl];
	  delete hEtaIntV2[iPl];
	}
      } 
      
      EtaFunction->Close();
      delete EtaFunction;
      EtaFunction2->Close();
      delete EtaFunction2;
    }
 fInitDone = kFALSE;
 
 return;
 
}


//______________________________________________________________________________
//
void MimosaAnalysis::AlignTracker(const Double_t tiniBoundU /*= 480.*/, 
				  const Double_t tiniBoundV /*= 480.*/, 
				  Int_t nAlignEvents /*= 4000 */, 
				  Int_t nAdditionalEvents /* = 2000 */,
				  bool UseAllHits /*false*/)  
{  
  // Aligns the planes according to their status.
  // Wrapper to the the MimosaAlignAnalysis::AlignTracker method
  //
  // Last Modified: JB 2013/06/10, different bounds in U and V introduced
  // Last Modified: AP 2015/06/10, added bool parameter (UseAllHits) to decide if doing alignment with all hits (UseAllHits = true) or with the closest one to the track (UseAllHits = false).

  if(!CheckIfDone("init")) {return;}
  else 
    {
      gSystem->cd(CreateGlobalResultDir());
      //MimosaAlignAnalysis::Instance( fSession)->AlignTracker(tiniBoundU, tiniBoundV, nAlignEvents, nAdditionalEvents);
      MimosaAlignAnalysis::Instance( fSession)->AlignTracker(tiniBoundU, tiniBoundV, nAlignEvents, nAdditionalEvents,UseAllHits);
    }
}


//______________________________________________________________________________
//
void MimosaAnalysis::AlignTrackerMinuit(Bool_t modeAuto /*=1*/, const Double_t tiniBound /*= 480.*/, Int_t nAlignEvents /*= 5000 */, Int_t nAlignHitsInPlane /*=4000*/, Int_t nAdditionalEvents /* = 2000 */, Double_t chi2Limit /*=0*/, bool UseAllHits /*=true*/)  
{  
  // LC 2012/09/06 : Aligns the ref planes system with Minuit method.

  if(!CheckIfDone("init")) {return;}
  else 
    {
      gSystem->cd(CreateGlobalResultDir());
      //MimosaAlignAnalysis::Instance( fSession)->AlignTrackerMinuit(modeAuto, tiniBound, nAlignEvents, nAlignHitsInPlane, nAdditionalEvents, chi2Limit);
      MimosaAlignAnalysis::Instance( fSession)->AlignTrackerMinuit(modeAuto, tiniBound, nAlignEvents, nAlignHitsInPlane, nAdditionalEvents, chi2Limit,UseAllHits);
    }
}


void MimosaAnalysis::AlignTrackerMinuitLadder(Bool_t modeAuto /*=1*/, const Double_t tiniBound/*=480.*/, Int_t nAlignEvents/*=5000*/, Int_t nAlignHitsInPlane/*=4000*/, Int_t nAdditionalEvents/*=2000*/, Double_t chi2Limit/*=0*/)
{
   // LC 2013/01/16 : Aligns the ladders with Minuit method.

  if(!CheckIfDone("init")) {return;}
  else 
    {
      gSystem->cd(CreateGlobalResultDir());
      MimosaAlignAnalysis::Instance( fSession)->AlignTrackerMinuitLadder(modeAuto, tiniBound, nAlignEvents, nAlignHitsInPlane, nAdditionalEvents, chi2Limit);
    }
  
}
/*
void MimosaAnalysis::AlignLadder(Bool_t modeAuto, const Double_t tiniBound, Int_t nAlignEvents, Int_t nAlignHitsInPlane, Int_t nAdditionalEvents, Int_t ladderFace, Double_t chi2Limit)
{
  if(!CheckIfDone("init")) {return;}
  else{
     gSystem->cd(CreateGlobalResultDir());
     MimosaAlignAnalysis::Instance( fSession)->AlignLadder(modeAuto, tiniBound, nAlignEvents, nAlignHitsInPlane, nAdditionalEvents, ladderFace, chi2Limit);
  }

}
*/
//______________________________________________________________________________
//
void MimosaAnalysis::AlignLadderMV(Bool_t modeAuto/*=1*/, const Double_t tiniBound/*=480.*/, const Double_t boundSlopes/*=100.*/, Int_t nAlignEvents/*=5000*/, Int_t nGoodTracks/*=1000*/, Int_t nAdditionalEvents/*=2000*/, Int_t chi2Limit/*=0.*/, Int_t mode/*=0*/) // LC 2013/11/10.
{
  if(!CheckIfDone("init")) {return;}
  else 
    {
      gSystem->cd(CreateGlobalResultDir());
      MimosaAlignAnalysis::Instance( fSession)->AlignLadderMV(modeAuto, tiniBound, boundSlopes, nAlignEvents, nGoodTracks, nAdditionalEvents, chi2Limit, mode);
    }  
}

//______________________________________________________________________________
//
void MimosaAnalysis::AlignTrackerMillepede(Int_t nAlignEvents /*=4000*/)  
{  
  // LC end of 2012
  if(!CheckIfDone("init")) {return;}
  else 
    {
      gSystem->cd(CreateGlobalResultDir());
      MimosaAlignAnalysis::Instance( fSession)->AlignTrackerMillepede(nAlignEvents);
    }
}

//______________________________________________________________________________
//
void MimosaAnalysis::AlignTrackerGlobal(const Int_t refPlane, const Int_t nEvents, const Int_t nIterations, const Bool_t alignConstrainsts, const Bool_t trackConstrainsts, const Bool_t multipleScatteringFit)
 
{  
  // LC && LiuQ 2015/02/06 : Global Alignment Method

  if(!CheckIfDone("init")) {return;}
  else 
    {
      gSystem->cd(CreateGlobalResultDir());
      MimosaAlignAnalysis::Instance( fSession)->AlignTrackerGlobal(refPlane, nEvents, nIterations, alignConstrainsts, trackConstrainsts, multipleScatteringFit);
    }
}

//______________________________________________________________________________
//
void MimosaAnalysis::DSFProduction(Int_t NEvt, Int_t fillLevel)
{
  // Runs the analysis on raw data (hit and track finders) over "NEvt" events
  //  and generates DSF root file with the Ttree containing those hits and tracks.
  // The amount of stored info inside the Ttree depends on "fillLevel":
  //  fillLevel=0, maximum info stored,
  //  fillLevel=1, only info on DUT stored.
  //
  // Modified: JB 2011/07/07 to localize path names
  // Modified: JB 2011/07/21 for level of storage

  if(!CheckIfDone("init")) return;
 
  fSession->MakeTree(); 
  fSession->SetEvents(NEvt); // to be modified
  fSession->SetFillLevel( fillLevel); // JB 2011/07/21
  //fSession->GetTracker()->SetAlignmentStatus(2);  // 2 = all planes in telescope used for tracking, for NOW JB
  if( fSession->GetTracker()->GetAlignmentStatus()==1 ) {
    printf("\nDSFProduction: -*- WARNING -*- only fixed (or primary) reference planes will be used for tracking!\n");
    printf("                                use gTAF->SetAlignStatus(2) to modify.\n");
  }
  else {
    printf("\nDSFProduction: both fixed and variable reference planes will be used for tracking.\n");
  }
  fSession->Loop();
  fSession->Finish();
  Char_t New_File_Name[1000];
  Char_t Old_File_Name[1000];
  sprintf(Old_File_Name,"%s/%s",(const char*)fSession->GetSummaryFilePath(),fSession->GetSummaryFileName().Data());
  sprintf(Old_File_Name,"%s", fTool.LocalizeDirName( Old_File_Name)); // JB 2011/07/07
  sprintf(New_File_Name,"%srun%d_0%d.root",(const char*)fSession->GetSummaryFilePath(),fSession->GetRunNumber(),GetFileNumber()+1);
  sprintf(New_File_Name,"%s", fTool.LocalizeDirName( New_File_Name)); // JB 2011/07/07

  gSystem->Rename(Old_File_Name,New_File_Name);
  cout<< fSession->GetSummaryFileName().Data()  <<" had been renamed : "<< New_File_Name <<endl;
  ofstream logfile;
  logfile.open("DSFProd.log",ios::app);
  logfile<< fSession->GetSummaryFileName().Data()  <<" had been renamed : "<< New_File_Name <<endl;
  logfile.close();
}

//______________________________________________________________________________
  void MimosaAnalysis::Help()
{
  //prints the common used commands

  cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
  cout<<"       COMMONLY USED COMMANDS "<<endl;
  cout<<"        arguments in [] are facultative"<<endl;
  cout<<"       --> last update v2.9.6 September 2013"<<endl;
  cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
  cout<<"                              "<<endl;
  cout<<"gTAF->Help()   "<<endl;  //  cout<<" "<<endl;
  cout<<"gTAF->SetDebug( [-]integer)"<<"  : the higher, the more messages; a negative number applies only to DAQ reading"<<endl;
  cout<<"gTAF->InitSession( runNumber [, planeNumber])  : init the session MANDATORY "<<endl;
  cout<<"gTAF->GetRaw()"<<"  : Open a dedicated menu for data inspection"<<endl;
  cout<<"  any method of this menu can be accessed directly with more options with:"<<endl;
  cout<<"    gTAF->GetRawDisplayxxx(...)"<< endl;
  /*  cout<<"----------------------------- "<<endl;
  cout<<"------------2/ RAW DATA "<<endl;
  cout<<"----------------------------- "<<endl;
  cout<<"gTAF->"<<endl;
  void InitScan(Int_t Events2Scan = 1600 , Int_t Events4Ped = 200 ,Float_t SignalOverNoiseCut = 5);  // Accumulate certain amount of events for scanning
  void RSDisplay(); //Display Ref.system events
  void MimosaDisplay(Int_t NEVENT = 1); //MIMOSA event display. 
  //RUN IT AFTER Inialisation + Init Scan");
  void InspectScan(); // INSPECT events // RUN IT AFTER MIMOSA event display
  void Inspectfake(); //INSPECT FAKES //"RUN IT AFTER MIMOSA event display
  void MimosaJump(Int_t Njump =10); // JUMP EVENTS" // "RUN IT AFTER MIMOSA event display
  */

  cout<<"                              "<<endl;
  cout<<"----------------------------- "<<endl;
  cout<<"------------2/ RAW DATA RECONSTRUCTION "<<endl;
  cout<<"----------------------------- "<<endl;
  cout<<"--->Make eta function for all planes)"<<endl;
  cout<<"gTAF->MakeEta( [events])"<<endl;
  cout<<"--->Align tracker (DISTANCE CUT primary planes - secondary plane(microns) )"<<endl;
  cout<<"gTAF->AlignTracker( [hit-track max distance], [events], [events for check]) "<<endl;
  cout<<"gTAF->AlignTracker( [hit-track max distance in U], [hit-track max distance in V], [events], [events for check]) "<<endl;
  cout<<"gTAF->AlignTrackerMinuit( )"<<endl;
  cout<<"--->Force the alignment status (usefull when you do not rerun AlignTracker)"<<endl;
  cout<<"gTAF->SetAlignStatus(2) "<<endl;
  cout<<"--->Align tracker with ladder"<<endl;
  cout<<"gTAF->AlignLadder( modeAuto, tiniBound, nAlignEvents, nAlignHitsInPlane, nAdditionalEvents, ladderFace, chi2Limit)"<<endl;
  cout<<"gTAF->AlignLadderMV( modeAuto, tiniBound, boundSlopes, nAlignEvents, nAdditionalEvents, chi2Limit)"<<endl;
  cout<<"gTAF->AlignTrackerMinuitLadder(Bool_t modeAuto, const Double_t tiniBound, Int_t nAlignEvents, Int_t nAlignHitsInPlane, Int_t nAdditionalEvents, Double_t chi2Limit)"<<endl;
  cout<<"--->Parametrization of the deviations"<<endl;
  cout<<"gTAF->StudyDeformation(tiniBound, nEvents, fitAuto)"<<endl;
  cout<<"--->Reconstruction"<<endl;
  cout<<"gTAF->DSFProduction( events, [fillLevel]) "<<endl;
  cout<<"                              "<<endl;
  cout<<"----------------------------- "<<endl;
  cout<<"------------3/ ANALYSIS       "<<endl;
  cout<<"----------------------------- "<<endl;
  cout<<"---> Define DSF file or Plane to analyse"<<endl;
  cout<<"gTAF->SetPlaneNumber(aPlaneNumber)"<<endl;
  cout<<"gTAF->SetFileNumber(aUserFileNumber)"<<endl;
  cout<<"gTAF->SetDSFFile(aFileName)"<<endl;
  cout<<"---> Analyse hit - track association"<<endl;
  cout<<"gTAF->SetCUT_MaxHitRatePerPixel( aRate)"<<endl;
  cout<<"gTAF->MimosaPro( events, TrackHitDist,S2N_seed,S2N_neighbour,submatrix,  GeoMatrix, SaveAlign (yes/no) , hitmap (0/1))"<<endl;
  cout<<"---> Analyse fake hit rate"<<endl;
  cout<<"gTAF->MimosaFakerate( events, S2N_seed, S2N_neighbour, submatrix, GeoMatrix)"<<endl;
  cout<<"---> Analyse calibration data"<<endl;
  cout<<"gTAF->MimosaCalibration( events, S2N_seed, S2N_neighbour, ChargeAroundSeedCut, submatrix, GeoMatrix)"<<endl;
  cout<<"---> Alignment of the DUT(s)"<<endl;
  cout<<"gTAF->AlignMimosa( aDistance)"<<endl;
  cout<<"---> Analyse dedicated to minivector configuration"<<endl;
  cout<<"gTAF->MimosaMiniVectors(MaxEvt, TrackHitDist, plane0, S2N_seed0, S2N_neighbour0, submatrix0, GeoMatrix0, plane1, S2N_seed1, S2N_neighbour1, submatrix1, GeoMatrix1)"<<endl;
  cout<<"---> Analyse 2 planes simultaneously"<<endl;
  cout<<"gTAF->MimosaPro2Planes(MaxEvt, TrackHitDist, plane0, S2N_seed0, S2N_neighbour0, submatrix0, plane1, S2N_seed1, S2N_neighbour1, submatrix1, GeoMatrix)"<<endl;
//  cout<<"---> Analyse "<<endl;
//  cout<<"gTAF->MimosaVertex(Int_t MaxEvt, Short_t plane1, Short_t plane2)"<<endl;
//  cout<<"---> Analyse "<<endl;
//  cout<<"gTAF->MimosaVertexFinder(Int_t MaxEvt, Int_t submatrix , Int_t GeoMatrix, Float_t chi2Cut)"<<endl;
  cout<<"---> Clear all histos to redo an analysis"<<endl;
  cout<<"gTAF->Clear()"<<endl;
  cout<<"                              "<<endl;
  cout<<"----------------------------- "<<endl;
  cout<<"------------4/ Plots       "<<endl;
  cout<<"------------ Menu opened after each analysis function"<<endl;
  cout<<"----------------------------- "<<endl;
  cout<<"gTAF->MimosaClusterCharge()"<<endl;
  cout<<"gTAF->CompareBadGoodRaw()  : compare the baseline of good events and bad events"<<endl;
  cout<<"gTAF->MimosaBinaryplots()  : for sensors with binary output"<<endl;
  cout<<"gTAF->CheckAsymmetry()"<<endl;
  cout<<"gTAF->pixelcharge()  : charge Distributions in each pixel ordered by charge"<<endl;
  cout<<"gTAF->MimosaSN()  : S/N Distribution in each pixel ordered by charge"<<endl;
  cout<<"gTAF->ClusterShape()  : Study cluster shape and multiplicity"<<endl;
  cout<<"gTAF->MimosaOptimize()  : Optimize cuts to minimize fake hit rate"<<endl;
  cout<<"gTAF->FakeRate()  : Compute and plot fake hit rates"<<endl;
  cout<<"gTAF->MimosaResolution()  : Compute resolution for different algorithms"<<endl;
  cout<<"gTAF->CheckMimosaAlign()  : "<<endl;
  cout<<"gTAF->InspectEfficiency() : check how evolves eff during the run"<<endl;
  cout<<"gTAF->Checkreferencetracks()  : Distributions of tracks position"<<endl;
  cout<<"gTAF->checkRefPlane()  : check Telescope properties"<<endl;
  cout<<"gTAF->MimosaPixelHomogeneity()  : Check pixel homogeneity depending on real track position"<<endl;
  cout<<"gTAF->Calibration()  : Fit the calibration peak"<<endl;
  cout<<"gTAF->UserAnalysis()  : Perform user's analysis"<<endl;
  cout<<"gTAF->SaveGifFile()  : Save in GIF and EPS files, in Results/#/ directory"<<endl;
  cout<<"gTAF->CreateNewEta()  : "<<endl;
  cout<<"gTAF->CDHist()  : "<<endl;

  //cout<<"gTAF->()  : "<<endl;
  //  cout<<"gTAF->SettHitMapReadOpt(1) (0=store, 1= read)"<<endl;
}


//______________________________________________________________________________
void MimosaAnalysis::CreateConfig(int RunNumberBigin, int NumberOfFilesPerRun, int RunNumberEnd, int RunNumberStep )
{
  //create a new conf based on an existing one

if(!CheckIfDone("init")) {return;}
 if(RunNumberBigin==fSession->GetRunNumber()) Fatal("CreateConfig()","You can`t load the config you want to modify!");
  
if(RunNumberEnd==0) RunNumberEnd=RunNumberBigin;
if(NumberOfFilesPerRun==0) NumberOfFilesPerRun=(*fSession->GetSetup()).GetRunPar().EndIndex-1;

  //*****************************************
    // call the script to create the runXXX.cfg file
    cout<<"Creating new config....based on run "<<fSession->GetRunNumber()<<" for Run from "<<RunNumberBigin<<" to "<<RunNumberEnd<<" with step "<<RunNumberStep<<" ("<<NumberOfFilesPerRun<<" files per Run)\n"<<endl;
    //gSystem->cd(""); //back to fWorkingDirectory
  Char_t create_cfg_string[200];
  sprintf(create_cfg_string,"Create_conf %s/run%d.cfg %d %d %d %d no",
          fSession->GetConfigPath().Data(),fSession->GetRunNumber(),
          RunNumberBigin,RunNumberEnd,RunNumberStep,NumberOfFilesPerRun);

  cout<<create_cfg_string<<endl;
  gSystem->Exec(create_cfg_string);

  //***************************************

}


//______________________________________________________________________________
void MimosaAnalysis::CreateLinks(int RunNumberBigin, int NumberOfFilesPerRun, int RunNumberEnd, int RunNumberStep )
{
  //create links to .rz Raw data files

if(!CheckIfDone("init")) return ;
if(RunNumberEnd==0) RunNumberEnd=RunNumberBigin;
if(NumberOfFilesPerRun==0) NumberOfFilesPerRun=(*fSession->GetSetup()).GetRunPar().EndIndex;


cout<<"Creating links for Run from "<<RunNumberBigin<<" to "<<RunNumberEnd<<" with step "<<RunNumberStep<<" ("<<NumberOfFilesPerRun<<" files per Run)\n"<<endl;

  for (int i_run=RunNumberBigin;i_run<=RunNumberEnd;i_run+=RunNumberStep)
    {

      //*****************************************
        // call the script to create limks
        //gSystem->cd(""); //back to fWorkingDirectory
        Char_t create_links_string[200];
      sprintf(create_links_string,"renameRawData.sh %d %d %s",
              i_run,
              NumberOfFilesPerRun-1,gSystem->Getenv("RZDIR"));

      cout<<create_links_string<<endl;
      // ex :renameRawData.sh 14352 3 /rawcmos9/data/mimostar2_mai_2006_calibration_vme
      gSystem->Exec(create_links_string);

      //***************************************

      }

}

//______________________________________________________________________________
void MimosaAnalysis::PrepareRun(int RunNumberBigin,Option_t* aType, int NumberOfFilesPerRun, int RunNumberEnd, int RunNumberStep )
{
 //create links to .rz Raw data files and prepare config for a new run

 TString opt = aType;
 opt.ToLower();
 
if(!CheckIfDone("init")) return ;
if(RunNumberEnd==0) RunNumberEnd=RunNumberBigin;


 if (opt.Contains("all")) opt = TString("create,get,link");
 
 if (opt.Contains("create")) CreateConfig(RunNumberBigin, NumberOfFilesPerRun,  RunNumberEnd,RunNumberStep );

  if (opt.Contains("get"))
    {
      for (int i_run=RunNumberBigin;i_run<=RunNumberEnd;i_run+=RunNumberStep)
	{
	  
	  //gSystem->cd(""); //back to fWorkingDirectory
	  Char_t copy_string[200];
	  
	  sprintf(copy_string,"%s/%d %s/",gSystem->Getenv("MAF_DATA_URL"),i_run ,gSystem->Getenv("RZDIR"));
	  
	  cout<<copy_string<<endl;
	  
	  gSystem->Exec(copy_string);
	}
    }

  if (opt.Contains("link")) CreateLinks(RunNumberBigin, NumberOfFilesPerRun,  RunNumberEnd,RunNumberStep );

}

//______________________________________________________________________________
//

void MimosaAnalysis::StudyDeformation(const Float_t tiniBound /*= 480.*/, Int_t nEvents /*= 4000 */, Bool_t fitAuto /* = 0 */){
  // Create profile plots from the ones created by the AlignTracker method to study the deviations of the track-hit residual.
  // A fit option is available to use 7th order Legendre polynomials to parametrize the deviations.
  // Wrapper to the the MimosaAlignAnalysis::StudyDeformation method
  //
  // BB 2014/05/20

  if(!CheckIfDone("init")) {return;}
  else 
    {
      gSystem->cd(CreateGlobalResultDir());
      MimosaAlignAnalysis::Instance( fSession)->StudyDeformation(tiniBound, nEvents, fitAuto);
    }
}
