// @(#)maf/dtools:$Name:  $:$Id:DSession.cxx  v.1 2005/10/02 18:03:46 sha Exp $
// Author   : ?
// Last Modified: JB 2008/10/13
// Last Modified: JB 2009/05/22,24,26
// Last Modified: JB 2009/07/17
// Last Modified: JB 2009/08/26 FillTree
// Last Modified: JB 2009/09/09 FillTree
// Last Modified: JB 2009/09/14 ResultDir added
// Last Modified: JB 2009/10/02 Finish
// Last Modified: JB 2010/10/13 Finish
// Last Modified: JB 2011/03/14 NextEvent, FillTree
// Last Modified: JB 2011/07/21 FillTree
// Last Modified: SS 2011/11/14 InitSession
// Last Modified: SS 2011/12/14 Finish
// Last Modified: SS 2012/08/10 NextRawEvent
// Last Modified: JB 2013/10/11 Get/SetTrackGeoLimitsForAlign
// Last Modified: JB 2013/06/22 Improved management of EventBuildingBoardMode (InitSession)
// Last Modified: JB 2013/07/14 TrackChi2Limit introducedForAlign
// Last Modified: BH 2013/08/20 Warning corrections from clang
// Last Modified: VR 2014/06/29 Add SetEventsToDo() and GetConfigFile() methods
// Last Modified: VR 2014/06/30 Simplification in InitSession
// Last Modified: VR 2014/07/13 Rename fEventCount by fCurrentEventNumber and add the GoToEvent and GoToNextEvent methods
// Last Modified: JB 2014/08/29 FillTree
// Last Modified: BB 2015/11/18 Modification of FillTree to avoid a Break segmentation

  ////////////////////////////////////////////////////////////
  // Class Description of DSession                          // 
  //                                                        //
  // + frame for analysis of raw event data                 //
  // + dumps data on standard output                        //
  // + fills events for storing in .root file               //
  //                                                        //
  ////////////////////////////////////////////////////////////

#include "Riostream.h"
#include "TString.h"
#include "DSession.h"
#include "DTracker.h"
#include "DPlane.h"
#include "DHit.h"
#include "DTrack.h"
//#include "DInp.h"
#include "DStrip.h"
#include "DSetup.h"
#include "DAcq.h"
//#include "DReader.h"
#include "DR3.h"
#include "TBits.h"

ClassImp(DSession) // DSession

DSession *DSession::fgInstance = 0; // returns pointer to global object

//_____________________________________________________________________________
//  
DSession::DSession(Option_t*)
{
   if (fgInstance) Warning("DSession", "object already instantiated");
   else            fgInstance = this;
   fDebugSession=0; //SS 2011.11.16 Initialization of debug level. It was set to one under certain circumstances

   fTracker = nullptr; // liuqy 2014/11/20: Avoid segment fault caused by "if(pointer) pointer->Method()" without pointer initialization.
   fAcq     = nullptr; // liuqy 2014/11/20: Avoid segment fault caused by "if(pointer) pointer->Method()" without pointer initialization.

}

//_____________________________________________________________________________
//  
DSession::DSession()
{ 

  fc       = nullptr;
  fAcq     = nullptr;
  fTracker = nullptr;

  cout << endl << " -*-*- DSession Constructor -*-*- " << endl;
  fc   = new DSetup(*this);                  // creation and initialization of DSetup
  fc->SetDebug( fDebugSession); // JB, 2008/10/13
  cout << " - Reading configuration " << endl;
  //-----------------------
  fc->ReadConfiguration();                 
  //-----------------------
  fAcq          = new DAcq(*fc);             // construct "DataAcquisition" object.
  fTracker      = new DTracker(*fc, *fAcq);  // construct the DTracker

  fEventsToDo = 0;
  fCurrentEventNumber = 0;

  // JB 2013/06/11
  fTrackLimitsForAlignX[0] = 0.;
  fTrackLimitsForAlignX[1] = 0.;
  fTrackLimitsForAlignY[0] = 0.;
  fTrackLimitsForAlignY[1] = 0.;
  fTrackChi2LimitForAlign = 0; // 2013/07/14
  
  cout << endl << " - Session now started " << endl;
  if (fgInstance) Warning("DSession", "object already instantiated");
  else            fgInstance = this;

  cout << endl << " -*-*- DSession Constructor DONE -*-*- " << endl;
}

//______________________________________________________________________________
//  
DSession::DSession(const Int_t aRunNumber)
{
  cout << endl << " -*-*- DSession Constructor -*-*- " << endl;

  SetRunNumber(aRunNumber);
  cout << " done " << endl;
   if (fgInstance) Warning("DSession", "object already instantiated");
   else            fgInstance = this;

  cout << endl << " -*-*- DSession Constructor DONE -*-*- " << endl;
}

//______________________________________________________________________________
//  
void DSession::InitSession()
{
  // Initialize any session
  // this is the very first method to be called
  //
  // Modified: JB 2011/07/07 to localize path names
  // Modified: SS 2011/11/14 for EventBuildingMode
  // Modified: JB 2013/06/22 for EventBuildingBoardMode
  // Modified: VR 2014/06/30 simplification of SetConfigPath and SetConfigFileName

  cout << endl << " -+-+- DSession Session Initialization -+-+- " << endl;

  fStatus    = 0; // AP, 2015/12/07
  //

  // Get basic information
  Char_t textRunNumber[250];  
  sprintf(textRunNumber,"%d",fRunNumber);
  cout << endl << " - Run Number : " << fRunNumber << endl;

  fc   = nullptr;
  fc   = new DSetup(*this);                  // creation and initialization of DSetup
  if( abs(fDebugSession) < 10 ) {
    fc->SetDebug( abs(fDebugSession));
  }
  else{
    fc->SetDebug( 0);
  }
  fc->SetConfigPath(fConfigPath);
  fc->SetConfigFileName(fConfigFileName);

  // Read the .cfg file
  cout << " - Reading configuration " << endl;
  fc->ReadConfiguration(); // read config file with information about setup and DAQ

  // If fEventBuildingMode is positive, it will override the setting taken above by fc->ReadConfiguration(), SS 2011.11.14.
  // Also update the EventBuildingBoardMode param, JB 2013/06/22
  if(fEventBuildingMode>=0) {
    fc->GetAcqPar().EventBuildingMode=fEventBuildingMode;
    for (int imod = 1; imod<=fc->GetAcqPar().ModuleTypes; imod++) { // correcting bug: adding initialisation BH 2013/08/20
      fc->GetModulePar(imod).EventBuildingBoardMode = fEventBuildingMode;
    } 
  }

  // Not needed with new reading, JB 2008/10/13
  //fReader       = new DReader(*fc);          // construct a Run environment 
  //fReader->SetRawSourcePath(fRawSourcePath); // 
  // Read info from header file if any
  //cout << "Read header file " << endl;
  //fReader->ReadHeaderFile(*fc);

  // Build the run environment from previous info
  fAcq          = new DAcq(*fc);             // construct "DataAcquisition" object.
  if( fDebugSession<=0 ) fAcq->SetDebug( fDebugSession); // JB, 2010/11/25

  fTracker      = new DTracker(*fc, *fAcq);  // construct the DTracker
  if( fDebugSession>=0 ) fTracker->SetDebug( fDebugSession); // JB, 2010/11/25

  fEventsToDo = 0;
  fCurrentEventNumber = 0;

  // Test if the DAQ is able to go to a specif event
  fDaqAbleToGoToAspecificEvent = 0;
  if (((fc->GetModulePar(1).Type)/10)==5 && (fc->GetAcqPar().ModuleTypes)==1) // If DAQ is PXIeBoardReader with only one module
  {
    fDaqAbleToGoToAspecificEvent = 1;
    std::cout << " DSession, the DAQ is able to give a specific event number !" << std::endl;
  }


  TString   fnHead, fnExt;
  fnHead = "DSF";
  fnExt  = ".root";
  cout << "=====================================" << endl;
  fSummaryFileName  = fnHead + textRunNumber + fnExt;
  fSummaryFilePathAndName = fSummaryFilePath + "/" + fSummaryFileName;
  fTool.LocalizeDirName( &fSummaryFilePathAndName); // JB 2011/07/07

  fSummaryFileTitle = TString("TAF TTree ") + textRunNumber; // JB 2015/03/24

  printf(" DSession, Paths and Files :\n");
  printf("  Raw Data Source  Path is  %s\n", fc->GetRunPar().DataPath ); // fRawSourcePath.Data() ); JB 2009/05/12
//   printf("  Data Summary File Path (temporary OUTPUT)  %s\n", fSummaryFilePath.Data() );
//   printf("  Data Summary File Name %s\n",                   fSummaryFileName.Data() );
  printf("  Data Summary File Name is %s\n",         fSummaryFilePathAndName.Data() );
//   printf("  Data Summary File Title is %s\n",              fSummaryFileTitle.Data() );

  gSystem->mkdir( fResultDir.Data(), 1);
  printf("  Result directory is %s\n", fResultDir.Data());

  //printf("  Weight file is %s\n", fWeightFileName.Data());

  cout << endl << " -+-+- DSession Session Initialization DONE  -+-+- " << endl;

}

//______________________________________________________________________________
//  
void DSession::SetDebug(Int_t aDebug)
{
  // Initialize or update the debugging level
  // updates also the level for Acquisition and tracker
  //
  // Created by JB 2009/05/12
  // Last Modified: JB 2009/05/22 (cope with negative debug level)

  fDebugSession = aDebug;
  cout << "DSession debug updated to " << fDebugSession << endl;

  if( aDebug>=0 ) { // for positive level, update only the Tracker
    if( fTracker ) { fTracker->SetDebug( aDebug); }
  }
  if( aDebug<=0 ) { // for negative level, update only the Dacq
    if( fAcq ) { fAcq->SetDebug( aDebug); }
  }

}

//______________________________________________________________________________
//  
void DSession::SetTrackGeoLimitsForAlign(Double_t xmin, Double_t xmax, Double_t ymin, Double_t ymax) {

  // Define geometrical limits of tracks to be considered for alignment.
  // Those limits are given in the tracker frame AND FOR Z=0.
  //
  // JB 2013/06/10
  
  fTrackLimitsForAlignX[0] = xmin;
  fTrackLimitsForAlignX[1] = xmax;
  fTrackLimitsForAlignY[0] = ymin;
  fTrackLimitsForAlignY[1] = ymax;
  
  printf("--> Align procedure is restricted to tracks with (at Z=0):");
  printf("  %.2f < X < %.2f   and   %.2f < Y < %.2f\n\n", fTrackLimitsForAlignX[0], fTrackLimitsForAlignX[1], fTrackLimitsForAlignY[0], fTrackLimitsForAlignY[1]);
    
}

//______________________________________________________________________________
//  
void DSession::SetTrackChi2LimitForAlign(Double_t aLimit) {
  
  // Define upper chi2 limit of tracks to be considered for alignment.
  //
  // JB 2013/07/14
  
  fTrackChi2LimitForAlign = aLimit;
  
  printf("--> Align procedure is restricted to tracks with chi2<=%f.\n", fTrackChi2LimitForAlign);
  
}

//______________________________________________________________________________
//  
void DSession::GetTrackGeoLimitsForAlign(Double_t &xmin, Double_t &xmax, Double_t &ymin, Double_t &ymax) {
  
  // Store the geometrical limits of tracks to be considered for alignment,
  // in the variable references provided.
  // Those limits are given in the tracker frame AND FOR Z=0.
  //
  // JB 2013/06/11
  
  xmin = fTrackLimitsForAlignX[0];
  xmax = fTrackLimitsForAlignX[1];
  ymin = fTrackLimitsForAlignY[0];
  ymax = fTrackLimitsForAlignY[1];
    
}

//______________________________________________________________________________
//  
//void DSession::SetWeightFile( TString aFileName) {
//
//  // Open the file containing the paramettrization for the eta function
//  //
//  // JB 2011/04/12
//  // Modified JB 2011/07/07 to localize path name
//  
//  fWeightFileName = aFileName;
//  fTool.LocalizeDirName( &fWeightFileName); // JB 2011/07/07
//  if( gSystem->AccessPathName(fWeightFileName.Data())) Info("SetWeightFile","%s  doesn't exist, it is created.",fWeightFileName.Data());
//  fWeightFile = new TFile(fWeightFileName.Data(), "UPDATE"); // READ->UPDATE, JB 2011/04/12
//  if(fWeightFile->IsZombie()||fWeightFile->GetNkeys()==0){ // QL 2015/11/04, recreate the file if it is a zombie or empty to avoid TAF crash. 
//       fWeightFile->Close();
//       fWeightFile = new TFile(fWeightFileName.Data(), "RECREATE");
//  }
//
//}
//______________________________________________________________________________
//  

Bool_t DSession::NextRawEvent( Int_t aTrigger)
{

  // Generate the reading of the next event,
  //  optionally corresponding to a given trigger,
  //  aTrigger=-1 means no specific trigger required.
  //
  // This methods define the event number known to TAF.
  //
  // completely changed for new reading, JB 2008/10/13
  // Modified: JB 2011/03/14, to start event number at 0
  // Modified: JB 2012/07/10, request a specific trigger to DAQ
  // Modified: SS 2012/08/10, management of DAcq multi-bits output
  
  TBits* DAcqResult=new TBits(2);

  if (fCurrentEventNumber++ <= fEventsToDo) {
    //------------------------
    DAcqResult = fAcq->NextEvent( fCurrentEventNumber-1, aTrigger); // get the next event from DAcq, -1 to start at 0, JB 2011/03/14
    //------------------------
    if( !DAcqResult->TestBitNumber(0) ) { cout << "WARNING: DSession, event " << fCurrentEventNumber << " can't be retrieve!" << endl; }
    if( DAcqResult->TestBitNumber(1) ) { cout << "WARNING: DSession, event " << fCurrentEventNumber << " is missing from the list!" << endl; }

  } else {
    cout << "WARNING: DSession, enough events " << fCurrentEventNumber << " / " << fEventsToDo << "!"<<endl; // improved comment, JB
  }                   

  if (GetStatus()==0 && fTracker->GetPlanesStatus() ){ // Moved from Loop(), JB 2009/05/26
    SetStatus(fTracker->GetPlanesStatus()) ;
  }   

  // display current event number with variable frequency
  Int_t frequency = 1; // added by JB, April 2008
  if( fDebugSession) { cout << endl << endl; }
  else if( fCurrentEventNumber > 100000) frequency = 10000;
  else if( fCurrentEventNumber > 10000) frequency = 5000;
  else if( fCurrentEventNumber > 1000) frequency = 500;
  else if( fCurrentEventNumber > 10) frequency = 50;
  if( fCurrentEventNumber/frequency*frequency == fCurrentEventNumber) {
    cout << "Event " << fCurrentEventNumber << " over " << fEventsToDo << " ";
    fWatch.Print();  // JB, 2008 September, to monitor CPU time
    fWatch.Continue();    
  }

  return DAcqResult->TestBitNumber(0); // stop only when no more event readable
}

//______________________________________________________________________________
//  
Int_t DSession::GoToEvent(Int_t anEvent)
{
  // Specific method to ask the DAQ for a given event
  //
  // Created : VR 2014/07/13, from NextRawEvent( Int_t aTrigger)
  if (anEvent<0)
  {
    cout << "ERROR: DSession::GoToEvent : give a positive event nb value!" << endl;
    return -1;
  }
  
  if (fDaqAbleToGoToAspecificEvent)
  {
    TBits* DAcqResult=new TBits(2);
    
    DAcqResult = fAcq->NextEvent(anEvent, anEvent);
    if( !DAcqResult->TestBitNumber(0)) 
    {
      cout << "WARNING: DSession::GoToEvent : event " << anEvent << " can't be retrieve!" << endl;
      return -1;
    }
    if( DAcqResult->TestBitNumber(1))
    {
      cout << "WARNING: DSession::GoToEvent : event " << anEvent << " is missing from the list!" << endl;
      return -1;
    }
    
    fCurrentEventNumber = anEvent;
    
    if (GetStatus()==0 && fTracker->GetPlanesStatus() )
    {
      SetStatus(fTracker->GetPlanesStatus()) ;
    }
  
    return fCurrentEventNumber;
  }
  else
  {
    cout << "ERROR: DSession::GoToEvent : to use this function, the real event nb must be able to be calculated, not the case!" << endl;
    return -1;
  }
}

//______________________________________________________________________________
//  
Int_t DSession::GoToNextEvent(void)
{
  // Specific method to ask the DAQ for a given event
  //
  // Created : VR 2014/07/13, from NextRawEvent( Int_t aTrigger)
  
  if (fDaqAbleToGoToAspecificEvent)
  {
    TBits* DAcqResult=new TBits(2);
    
    DAcqResult = fAcq->NextEvent(fCurrentEventNumber+1,fCurrentEventNumber+1);
    if( !DAcqResult->TestBitNumber(0)) 
    {
      cout << "WARNING: DSession::GoToEvent : event " << fCurrentEventNumber+1<< " can't be retrieve!" << endl;
      return -1;
    }
    if( DAcqResult->TestBitNumber(1))
    {
      cout << "WARNING: DSession::GoToEvent : event " << fCurrentEventNumber+1 << " is missing from the list!" << endl;
      return -1;
    }
    
    fCurrentEventNumber ++;
    
    if (GetStatus()==0 && fTracker->GetPlanesStatus() )
    {
      SetStatus(fTracker->GetPlanesStatus()) ;
    }
    
    // display current event number with variable frequency
    Int_t frequency = 1;
    if( fDebugSession) { cout << endl << endl; }
    else if( fCurrentEventNumber > 100000) frequency = 10000;
    else if( fCurrentEventNumber > 10000)  frequency = 5000;
    else if( fCurrentEventNumber > 1000)   frequency = 500;
    else if( fCurrentEventNumber > 10)     frequency = 50;
    if( fCurrentEventNumber/frequency*frequency == fCurrentEventNumber)
    {
      cout << "Real Event Nb " << fCurrentEventNumber << " ";
      fWatch.Print();
      fWatch.Continue();    
    }
      
    return fCurrentEventNumber;
  }
  else
  {
    if (!NextRawEvent(-1)) return -1; 
    return fCurrentEventNumber;
  }
}

//______________________________________________________________________________
//  

void DSession::MakeTree()
{
  // Create a new ROOT file with Tree.

  // Not needed with new reading, JB 2008/10/13
  //fReader->Reset();                
  //fCurrentEventNumber   = 0;                    
  fStatus       = 0;  
  Int_t   tCompressionLevel = 2; 

  // If level = 1, only branches containing integers will be compressed. 
  // If level >= 2, all branches will be compressed with compression level-1. 

  fSummaryFile = new TFile(fSummaryFilePathAndName, "RECREATE", fSummaryFileTitle);
  fSummaryFile->SetCompressionLevel(tCompressionLevel);
  fEvent = new DEvent(*fc);   

  // Create a ROOT Tree and one branch

  //TTree::SetMaxTreeSize(1000*Long64_t(2000000000));
  fEventTree = new TTree("T", fSummaryFileTitle); 
  fEventTree->SetAutoSave(100000000);  // autosave when 1 Mbyte written
  fEventTree->Print();
  TBranch *b = fEventTree->Branch("fEvent","DEvent",&fEvent,64000,99);
   b->SetAutoDelete(kFALSE);

   if( !fEventTree || !b ) printf("\n\n-*-*- ERROR DSession:MakeTree, something is wrong with the creation of the TTree!\n\n");

  printf("DSssion, EventTree is prepared and will be filled with event data \n");
}

//_____________________________________________________________________________
//  
void DSession::Loop()
{
  cout << endl << endl << "Session: " << fEventsToDo << " events will be read" << endl << endl ;

  fWatch.Start(); // Start to count processing time, JB 2008/08/08

  while(NextRawEvent() == kTRUE) { // event loop
    //==============
    Int_t Updt = fTracker->Update(); 
//cout << __FILE__ << __LINE__ << endl;
    //==============
    if( fDebugSession) printf("\n\nDSession::Loop Event=%d, Updt=%d, f(Session)Status=%d, fPlanesStatus=%d\n", GetCurrentEventNumber(), Updt, GetStatus(), fTracker->GetPlanesStatus());

//cout << __FILE__ << __LINE__ << endl;
    // kill events received after reset
    if (Updt == 0) {  // Condition "&& GetStatus() >=1" removed by JB, 2007 June
//cout << __FILE__ << __LINE__ << endl;
    //==============
      FillTree();
//cout << __FILE__ << __LINE__ << endl;
    //==============
    }
    else { 
//cout << __FILE__ << __LINE__ << endl;
      if( fDebugSession) cout << "DSession::Loop Something's wrong in Tracker update for event " << GetCurrentEventNumber() << ", No Tree filled" << endl;
//cout << __FILE__ << __LINE__ << endl;
    }   
//cout << __FILE__ << __LINE__ << endl;

  } // end event loop
//cout << __FILE__ << __LINE__ << endl;
  //fEventTree->Print();
}

//______________________________________________________________________________
//  
void DSession::Scan()
{

  DPlane *thePl = GetTracker()->GetPlane(fPlaneToScan) ;
  
  Int_t nu =thePl->GetStripsNu() ;
  Int_t nv =thePl->GetStripsNv() ;

  char c = 'a';
  char stop = 's';
  while(NextRawEvent() == kTRUE && c != stop) {
    if (fTracker->Update() != 0 ) cout << "Problem with some planes " << endl ;
    if (GetStatus() >=1) {
      Int_t ist = 1 ;
      for (Int_t iu=1 ; iu<nu ; iu++){
	for (Int_t iv=1 ; iv<nv ; iv++){      
	  cout << thePl->GetRawValue(ist) << " " ;
	  ist++ ;
	}

      }
      cout << "s to stop" << endl ;
      cin >> c ;
    }
  }

  cout << "------------------------" << endl ;

}

//______________________________________________________________________________
//  
void DSession::Finish()
{
  // Modified JB 2009/09/08 to print statistics of tracking
  // Modified JB 2009/10/02 to print statistics of tree
  // Modified JB 2010/10/13 to print end time
  // Modified JB 2011/04/12 close weight file
 //  Modified SS 2011/12/14 save statistics to the logfile

  TDatime aTime;
  fTracker->PrintStatistics();
  fAcq->PrintStatistics();
  ofstream logfile;
  logfile.open("DSFProd.log",ios::app);
  logfile<<"============================================"<<endl;
  logfile<<aTime.AsString()<<endl;
  fTracker->PrintStatistics(logfile);
  fAcq->PrintStatistics(logfile);
  logfile.close();

  printf("************************************\n");
  printf("Entries in TTree: %ld\n.", (long int)fEventTree->GetEntries()); // JB 2009/10/02, could be Long64_t
  printf("************************************\n");
  
  cout << endl << "TAF ends at ";
  aTime.Print();

  fSummaryFile->cd();
  fEventTree->Write();
  fSummaryFile->Close();
  //fWeightFile->Close(); // JB 2011/04/12

}

//______________________________________________________________________________
//  
void DSession::FillTree()
{

  // Fill the tree with the followinf information:
  // statistics (ped, noise, # hits, ...) of each plane
  // each hit info of each plane, note that each hit is associated to the nearest existing track
  // each track info crossing each plane 
  //  (note hit nb is negative for hits used in the track fit).
  //
  // Some plane may be skipped (use SetFillLevel(aLevel) to set):
  // fFillLevel = 0 -> info (hits+tracks) of all planes written,
  // fFillLevel = 1 -> only the full info of planes with status=3 (DUT) is written,
  //                for other planes, only the track info is written.
  //
  // Last modified: JB 2009/05/24, 2009/07/17 (too allow for multi-track)
  // Last modified: JB 2009/08/26, to count track from 1 (not 0)
  // Last modified: JB 2009/9/09, event and frame(-realEvent) number stored
  // Last modified: JB 2011/03/14, plane status condition removed
  // Last modified: JB 2011/06/30, printout of events
  // Last modified: JB 2011/07/21, select which plane info are stored
  // Last modified: JB 2014/08/29, indicate if track fitted with hit and new fill conditions
  // Last modified: BB 2015/11/17, change for loop on track to do...while to be sure that the event is read

  DPlane        *tPlane=nullptr; //nullptr instead of 0
  DTrack        *tTrack=nullptr; //nullptr instead of 0
  DHit          *tHit=nullptr;
  Int_t         tracksN(0), hitsN(0), pl(0);
  Bool_t        hitAssociated(false);
  
  // Event number is the counter of DSession, 
  // while the "real" event number from DAQ is stored as the frame number
  // JB 2009/09/09
  fEvent->SetHeader( GetCurrentEventNumber(), fAcq->GetRunNumber(), 0, 0, 0, 
                    fAcq->GetTriggers(), fAcq->GetFrames(), fAcq->GetTimestamps());
  fEvent->SetFrame( fAcq->GetRealEventNumber(), 0 );
  
  tracksN = fTracker->GetTracksN(); // Number of tracks for this event
  if (fDebugSession) printf("DSession::FillTree, Event Trig %d, current Event %d: %d track\n", fAcq->GetRealEventNumber(), GetCurrentEventNumber(), tracksN);
  
  // Loop on planes
  //Int_t Npl(0);
  //Npl=fTracker->GetPlanesN();
  for (pl = 1; pl <= fTracker->GetPlanesN(); pl++) { // Loop on planes
    
    tPlane  = fTracker->GetPlane(pl);
    if( fDebugSession) printf("DSession::FillTree got plane %d with status %d\n", pl, tPlane->GetStatus());
    
    // for all planes which have been analyzed
    // condition on Readout added by JB, 2007 June
    // condition on Status removed by JB, 2011/03/14
    if ( GetCurrentEventNumber()>1 && tPlane->GetReadout()!=0 ) { // condition for storing info, modified JB 2014/08/29
     
      if( fDebugSession) printf("DSession::FillTree adding authenticPlane %d\n", pl);
      fEvent->AddAuthenticPlane(*tPlane, fAcq->GetRealEventNumber()); // basic info on planes
      
      // store hits, test condition on status and FillLevel added JB 2014/08/29
      if ( !fFillLevel || (tPlane->GetStatus() == 3)) { // 
        hitsN = tPlane->GetHitsN();
        if( fDebugSession) printf("DSession::FillTree adding %d authenticHit for plane %d\n", hitsN, pl);

        for (Int_t ht = 1; ht <= hitsN; ht++) { // loop on hits
          tHit = tPlane->GetHit(ht);
          tTrack = fTracker->nearest_track(tHit); // nearest track, JB 2009/07/17
//          printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", GetCurrentEventNumber(), tHit->GetNumber(), tPlane->GetHitsN(), tHit->GetStripsInCluster(), tHit->GetClusterPulseSum(), tHit->GetPulseHeight(0), tHit->GetPositionUhit(), tHit->GetPositionVhit()); // JB 2017/11/09
          if( fDebugSession>1 ) printf("DSession::FillTree got hit %d with seed %d and %d neighbours, nearest track %d\n", ht, tHit->GetIndexSeed(), tHit->GetStripsInCluster(), tTrack?tTrack->GetNumber():-1); // JB 2009/05/12
          fEvent->AddAuthenticHit(*tHit, fAcq->GetRealEventNumber(), *tTrack); // store hit along with nearest track
        } //end loop on hits
      } // end condition for storing hits

      // store track (A track is stored for each plane it crosses)
      if(tracksN>0){ // condition added for calibration:
        if( fDebugSession) printf("DSession::FillTree adding %d transparentPlane (track) for plane %d\n", tracksN, pl);
        Int_t iteratorTrack = 1;
        //for( Int_t it=1; it<=tracksN; it++) { // loop on tracks

        do{
          tTrack = fTracker->GetTrack(iteratorTrack);
          tHit = fTracker->nearest_hit( tTrack, pl, hitAssociated); // nearest hit, JB 2009/07/17
          if( fDebugSession>1 ) printf("DSession::FillTree got track %d, %s hit %d\n", tTrack->GetNumber(), hitAssociated?"associated":"nearest", tHit?tHit->GetNumber():0); // JB 2014,08/29
          if(tHit != nullptr){ // BB 2015/11/18 : assuming that we add a transparent plane only when there is a hit, otherwise go to the next event
            fEvent->AddTransparentPlane(*tPlane, *tTrack, *tHit, hitAssociated, *fTracker ); // store track along with nearest hit
          }
          iteratorTrack++;
        }while(iteratorTrack <= tracksN); // end loop on tracks
     } // end condition added for calibration

    } // end condition for storing info

  } // end of loop over planes

  if( fDebugSession) {
    printf("DSession::FillTree gonna fill the tree with event:\n");
    fEvent->GetHeader().Print(); // JB 2011/06/30
    if( fDebugSession > 2 ) fEventTree->Print(); // JB 2011/06/30
  }
  fEventTree->Fill();
  
  fEvent->Clear();

}

void DSession::ResetDaq()
{ 
  fEventsToDo = 0;
  fCurrentEventNumber = 0;

  fAcq->Reset();   
}

