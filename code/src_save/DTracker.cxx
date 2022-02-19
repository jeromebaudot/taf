// @(#)maf/dtools:$Name:  $:$Id:DTracker.cxx  v.1 2005/10/02 18:03:46 sha Exp $
//  Author   :   Dirk Meier   97/12/10
// Last Modified: JB 2009/05/22
// Last Modified: JB 2009/05/25
// Last Modified: JB 2009/07/17
// Last Modified: JB 2009/07/20
// Last Modified: JB 2009/08/21
// Last Modified: JB 2009/08/25, find_track
// Last Modified: JB 2009/08/26, nearest_track
// Last Modified: JB 2009/08/31, Update
// Last Modified: JB 2009/09/01, constructor & find_tracks
// Last Modified: JB 2009/09/08, constructor & find_tracks
// Last Modified: JB 2011/03/14, constructor & find_tracks
// Last Modified: JB 2011/06/21, constructor & SetAlignStatus
// Last Modified: SS 2011/10/26, find_tracks
// Last Modified: SS 2011/12/14, PrintStatistics
// Last Modified: JB 2012/04/02, find_tracks
// Last Modified: JB 2012/08/20, SetDebug
// Last Modified: JB 2012/08/24, find_tracks_withStrips
// Last Modified: JB 2012/08/28, condition on min ref plane for track changed for strip-telescope
// Last Modified: JB 2012/09/07, find_tracks allows several planes for seed track
// Last Modified: JB 2013/06/11, DTracker, Update
// Last Modified: JB 2013/06/21, find_tracks
// Last Modified: JB 2013/08/19, find_tracks_withStrips, SetAlignmentStatus
// Last Modified: BH 2013/08/20, warning corrections from clang
// Last Modified: LC 2013-2014,  add Ladder object made of several planes
// Last Modified: VR 2014/06/29, Fix a bug in find_tracks(), Use the TracksFinder config to choose the tracking method, and add the SearchMoreHitDistance config
// Last Modified: VR 2014/07/14, add new methods find_tracks_1_opt and find_tracks_2_ivi
// Last Modified: VR, 2014/08/28 add mecanism to keep untracked hits from last event
// Last Modified: JB 2014/08/29, nearest_hit
// Last Modified: VR, 2014/08/29 add fTrackingOrderLines, fTrackingOrderPreTrack and fTrackingOrderFulTrack : defines planes scan order during tracking for TracksFinder 2
// Last Modified: JB 2014/08/29, fTrackCountPerPlane added in all find_tracks methods
// Last Modified: JB 2014/12/15, subtrack mechanism added, DTracker, find_tracks, MakeSubTrack
// Last Modified: VR, 2014/12/18 Completely rebuild find_tracks2
// Last Modified: AP, 2015/03/11 Added UpdateMC function to perform tracking of MC simulated hits
//                               including multiple scattering and sensor spatial resolution.
//                               This is done of evaluating the telescope resolution on the DUT position.
// Last Modified: JB, 2015/08/21 SetPlanesForSubTrack
// Last Modified: BB, 2015/11/18 nearest_track change a for loop to a do...while loop to make sure that we return a pointer when we have one track only

  ////////////////////////////////////////////////////////////
  // Class Description of DTracker                          //
  //                                                        //
  ////////////////////////////////////////////////////////////

//*-- Modified : IG
//*-- Copyright:  RD42
//*-- Author   :  Dirk Meier   97/12/10
//*KEEP,CopyRight.
/************************************************************************
 * Copyright(c) 1997, RD42@CERN, DiamondTracking.
 ***********************************************************************/
//*KEND.

#include "TApplication.h"
#include "Riostream.h"
//*Keep,DTracker.
#include "DTracker.h"
//*Keep,DSetup.
#include "DSetup.h"
//*Keep,DAcq.
#include "DAcq.h"
//*Keep,DPlane.
#include "DPlane.h"
//*Keep,DTrack.
#include "DTrack.h"
//*KEEP,DLine.
#include "DLine.h"
//*Keep,DHit.
#include "DHit.h"
#include "TMinuit.h"
#include "DLadder.h"
//*KEND.
#include "DBeaster.h"



DTracker* DTracker::fgInstance = 0;

ClassImp(DTracker) // Description of Single Detector DTracker

//______________________________________________________________________________
//
DTracker::DTracker()
{
// DTracker default constructor
//  if (fgInstance) Warning("MimosaAlignAnalysis", "object already instantiated");
//  else fgInstance = this;
}

//______________________________________________________________________________
//
DTracker::DTracker(DSetup& c, DAcq& aAcq)
{

  // -----------------------------------------------
  // -+-+- Constructs the DTracker with its Planes
  // -----------------------------------------------
  //
  // Modified: JB 2009/08/21 to add list of DUTs
  // Modified: JB 2009/09/01 read frequiredhits from config file
  // Modified: JB 2009/09/08 init counter on tracks over all events
  // Modified: JB 2011/03/14 array of track seed devices added for planeStatus=0
  // Modified: JB 2011/06/21 check there is at least one seed plane if tracking required
  // Modified: JB 2011/06/21 separation of list for seed and primary ref.
  // Modified: JB 2013/06/11 introduce variable controling #vertex recosntructed
  // Modified: JB 2013/07/17 new select parameter
  // Modified: LC 2013-2014  introduce Ladder objects
  // Modified: JB 2014/02/14 introduce Custom ladder
  // Modified: VR 2014/06/29 Use the TracksFinder config. to select the tracking method
  // Modified: JB 2014/08/29 addition of fTrackCountPerPlane
  // Modified: JB 2014/12/15 all variable for subtracks introduced
  // Modified: VR 2014/12/18 Completely rebuild find_tracks2

  if (fgInstance) Warning("MimosaAlignAnalysis", "object already instantiated");
  else fgInstance = this;

  cout << endl << " -+-+- DTracker User Constructor starting -+-+- " << endl;

  fAcq    = &aAcq;
  fc      = &c;

  MCInfoHolder = NULL;
  if(fAcq->GetIfMCBoardReader()) MCInfoHolder = fAcq->GetMCInfoHolder();

  fDebugTracker = fc->GetDebug();
  fPlanesN = fc->GetTrackerPar().Planes;  // the number of planes in the tracker
  fPlanesStatus = 0 ; // initialize at "initalization" step

  fPlaneArray  = new TObjArray(fPlanesN);
  fLadderArray = new TObjArray(fPlanesN/12);

  fTrackerGeometry      = new TBRIK("Tracker","DT","void",40,40,90);
  fTrackerNode          = new TNode("Tracker","Tracker","Tracker");
  fTrackerNode->cd();

  fListTrackSeedDevs = new Int_t[fPlanesN]; // JB 2011/03/14
  fListFixRefDevs    = new Int_t[fPlanesN];
  fListVarRefDevs    = new Int_t[fPlanesN];
  fListTestDevs      = new Int_t[fPlanesN];

  Int_t ts  = 0;
  Int_t fx  = 0;
  Int_t vr  = 0;
  Int_t dut = 0; // added JB 2007 June

  DPlane *aPlane=0;            // a pointer to a plane
  DLadder *aLadder=0;          // a pointer to a Ladder.
  fNumberOfLadders = 0;        // Number of Ladders in the tracker.

  Int_t planeStatus, planeReadout;
  //Int_t planeAnalysisMode; // usefull state of plane

  fTracksMaximum = fc->GetTrackerPar().TracksMaximum;
  fHitsMaximum   = fPlanesN;               // preliminary
  fHitList       = new DHit*[fHitsMaximum];   // these are pointers to hits to make one track,
  // hits itself are generated in the device constructor
  
  fNumberOfLadders = fc->GetTrackerPar().Ladders;

  Int_t planesLadder = 0; // Number of planes in ladders.
  for(Int_t laddersIter=0 ; laddersIter<fNumberOfLadders ; ++laddersIter) planesLadder += fc->GetLadderPar(laddersIter).Planes;
  if (fDebugTracker) {
    cout << "  There are " << fNumberOfLadders << " ladders to declare." << endl;
    cout << "  There are " << planesLadder << " planes associated to ladders." << endl;
    cout << "      --> only " << fPlanesN-planesLadder << " planes will be declared as independent." << endl;
  }

  if(fPlanesN-planesLadder != 0)  { // if there is some individual planes

    for (Int_t pl = 1; pl <= fPlanesN-planesLadder; pl++){
      if (fDebugTracker) cout << "plane=" << pl << " ";
      aPlane  = new DPlane(*this, pl, 0);
      fPlaneArray->Add(aPlane);
      planeStatus = ((DPlane*)fPlaneArray->At(pl-1))->GetStatus();
      planeReadout = fc->GetPlanePar(pl).Readout;
      //planeAnalysisMode = fc->GetPlanePar(pl).AnalysisMode;
      if (fDebugTracker)  cout << " status=" << planeStatus << " readout=" << planeReadout;

      // Modify fix, var and DUT planes counting
      // JB, 2007 June and 2011/03/14 for status=0
      // JB, 2011/06/21 to separate seed and fixed ref.
      if( planeReadout>0 ) { // if 0, plane is not analyzed, exclude from tracking

        if ( planeStatus == 0 ){ // fixed reference for track seed
          if (fDebugTracker) cout << " seed ref=" << fx << endl;
          fListTrackSeedDevs[ts++]   = ((DPlane*)fPlaneArray->At(pl-1))->GetPlaneNumber();
        }

        else if ( planeStatus == 1){ // fixed reference not track seed
          if (fDebugTracker) cout << " fixed ref=" << fx << endl;
          fListFixRefDevs[fx++]   = ((DPlane*)fPlaneArray->At(pl-1))->GetPlaneNumber();
        }

        else if ( planeStatus == 2 || planeStatus == 3 ){ // variable reference or DUT
          if (fDebugTracker) cout << " variable ref=" << vr << endl;
          fListVarRefDevs[vr++] = ((DPlane*)fPlaneArray->At(pl-1))->GetPlaneNumber();
          if( planeStatus == 3) { // DUT
            if (fDebugTracker) cout << "   also device under test=" << vr << endl;
            fListTestDevs[dut++] = ((DPlane*)fPlaneArray->At(pl-1))->GetPlaneNumber();
          }
        }

        else {
          cout << endl << "WARNING plane " << pl << " has an unknown status " << planeStatus << endl;
        }

        //      if ( planeAnalysisMode==1 && planeStatus!=3 ) { // consider all strips as seed for tracking, JB 2012/08/23
        //		    if (fDebugTracker) cout << " seed ref=" << fx << endl;
        //		    fListTrackSeedDevs[ts++]   = ((DPlane*)fPlaneArray->At(pl-1))->GetPlaneNumber();
        //		  }

      } // end if readout !=0
      else {
        if (fDebugTracker) cout << " not analyzed -> out of tracker!" << endl;
      }
    } // end loop on planes
    cout << endl;
  } // end if there is some individual planes

  if(fNumberOfLadders !=0) {

    if (fDebugTracker) std::cout<<"Number Of Ladders = "<<fNumberOfLadders<<std::endl;

    for(Int_t laddersIter=0 ; laddersIter<fNumberOfLadders ; ++laddersIter) {

      if (fDebugTracker) std::cout<<"* Declaring ladder "<<laddersIter<<std::endl;

      Int_t ladderID =  fc->GetLadderPar(laddersIter).LadderID;
      Int_t planesPerLadder = fc->GetLadderPar(laddersIter).Planes;
      TString ladderName = fc->GetLadderPar(laddersIter).Name;
      DR3 tilt = fc->GetLadderPar(laddersIter).Tilt;
      Int_t status = fc->GetLadderPar(laddersIter).Status;

      if (fDebugTracker) std::cout<<"Ladder "<<laddersIter<<" ID="<<ladderID<<" has "<<planesPerLadder<<" planes."<<std::endl;

      if(ladderName=="Plume" || ladderName=="plume") aLadder = new DLadder(*this, ladderName, laddersIter+1, status, 19872/*sensorLengthX*/, 420/*sensorSeparator*/, 1044.5/*distanceFromFoam*/, tilt(2) /*rotationX*/, tilt(1) /*rotationY*/, tilt(0) /*rotationZ*/);
      else if(ladderName=="Simple" || ladderName=="simple")  aLadder = new DLadder(*this, ladderName, laddersIter+1, status, 19872, 420, 0.0, tilt(2), tilt(1), tilt(0) ); // correcting bug: restoring condition instead of assignment BH 2013/08/20
      else if(ladderName=="Salat" || ladderName=="salat") aLadder = new DLadder(*this, ladderName, laddersIter+1, status, 19872, 100., 50.0, tilt(2), tilt(1), tilt(0) );
      else if(ladderName=="Custom" || ladderName=="custom") aLadder = new DLadder(*this, ladderName, ladderID, status, 19872, 100., 1000., tilt(2), tilt(1), tilt(0) );
      aLadder->SetLadderType(ladderName);
      //fLadderArray->Add(aLadder);

      if (fDebugTracker) std::cout<<" ladder status = "<<status<<std::endl;

      for (Int_t iterPlanes = 1; iterPlanes <= planesPerLadder; iterPlanes++){

        Int_t planeNumber;
        // For custom ladder type, use the number declared in the configuration file as plane number
        // JB 2014/03/17
        if (ladderName=="Custom" || ladderName=="custom") {
          planeNumber = fc->GetLadderPar(laddersIter).PlaneList[iterPlanes-1];
        }
        else {
          planeNumber = fPlanesN-planesLadder + laddersIter*planesPerLadder+iterPlanes;
        }

        if (fDebugTracker) cout << endl << " DTracker: Adding tracker plane=" << planeNumber << " as " << iterPlanes << "th plane." << endl;

        aPlane  = new DPlane(*this, planeNumber, ladderID);
        fPlaneArray->Add(aPlane);
        //planeStatus = ((DPlane*)fPlaneArray->At(planeNumber-1))->GetStatus();
        planeStatus = status; // plane status shall be superseded by ladder's one
        planeReadout = fc->GetPlanePar(planeNumber-1).Readout;
        //planeAnalysisMode = fc->GetPlanePar(planeNumber-1).AnalysisMode;

        //std::cout<<"iterPlanes = "<<iterPlanes<<std::endl;
        aLadder->AddPlane(aPlane,iterPlanes);

        if (fDebugTracker)  cout << " Plane " << planeNumber << " with status=" << planeStatus << " readout=" << planeReadout << " added to ladder " << ladderID << " as plane " << iterPlanes << endl;

        // Modify fix, var and DUT planes counting
        // JB, 2007 June and 2011/03/14 for status=0
        // JB, 2011/06/21 to separate seed and fixed ref.
        // JB, 2014/03/17 to assign correct plane number
        if( planeReadout>0 ) { // if 0, plane is not analyzed, exclude from tracking

          if ( planeStatus == 0 ){ // fixed reference for track seed
            if (fDebugTracker) cout << " seed ref=" << fx << endl;
            fListTrackSeedDevs[ts++]   = planeNumber;
          }

          else if ( planeStatus == 1){ // fixed reference not track seed
            if (fDebugTracker) cout << " fixed ref=" << fx << endl;
            fListFixRefDevs[fx++]   = planeNumber;
          }

          else if ( planeStatus == 2 || planeStatus == 3 ){ // variable reference or DUT
            if (fDebugTracker) cout << " variable ref=" << vr << endl;
            fListVarRefDevs[vr++] = planeNumber;
            if( planeStatus == 3) { // DUT
              if (fDebugTracker) cout << "   also device under test=" << vr << endl;
              fListTestDevs[dut++] = planeNumber;
            }
          }

          else {
            cout << endl << "WARNING plane " << planeNumber << " has an unknown status " << planeStatus << endl;
          }

          //      if ( planeAnalysisMode==1 && planeStatus!=3 ) { // consider all strips as seed for tracking, JB 2012/08/23
          //		    if (fDebugTracker) cout << " seed ref=" << fx << endl;
          //		    fListTrackSeedDevs[ts++]   = ((DPlane*)fPlaneArray->At(pl-1))->GetPlaneNumber();
          //		  }

        } // end if readout !=0
        else {
          if (fDebugTracker) cout << " not analyzed -> out of tracker!" << endl;
        }
      } // end loop on planes
      cout << endl;
      fLadderArray->Add(aLadder);

    } // end loop on ladders.
  } // end test on number of ladders
  /*
   if(fPlanesN-planesPerLadder != 0)  {

   for (Int_t pl = 1; pl <= fPlanesN; pl++){
   if (fDebugTracker) cout << "plane=" << pl << " ";
   aPlane  = new DPlane(*this, pl, 0);
   fPlaneArray->Add(aPlane);
   planeStatus = ((DPlane*)fPlaneArray->At(pl-1))->GetStatus();
   planeReadout = fc->GetPlanePar(pl).Readout;
   planeAnalysisMode = fc->GetPlanePar(pl).AnalysisMode;
   if (fDebugTracker)  cout << " status=" << planeStatus << " readout=" << planeReadout;

   // Modify fix, var and DUT planes counting
   // JB, 2007 June and 2011/03/14 for status=0
   // JB, 2011/06/21 to separate seed and fixed ref.
   if( planeReadout>0 ) { // if 0, plane is not analyzed, exclude from tracking

   if ( planeStatus == 0 ){ // fixed reference for track seed
   if (fDebugTracker) cout << " seed ref=" << fx << endl;
   fListTrackSeedDevs[ts++]   = ((DPlane*)fPlaneArray->At(pl-1))->GetPlaneNumber();
   }

   else if ( planeStatus == 1){ // fixed reference not track seed
   if (fDebugTracker) cout << " fixed ref=" << fx << endl;
   fListFixRefDevs[fx++]   = ((DPlane*)fPlaneArray->At(pl-1))->GetPlaneNumber();
   }

   else if ( planeStatus == 2 || planeStatus == 3 ){ // variable reference or DUT
   if (fDebugTracker) cout << " variable ref=" << vr << endl;
   fListVarRefDevs[vr++] = ((DPlane*)fPlaneArray->At(pl-1))->GetPlaneNumber();
   if( planeStatus == 3) { // DUT
   if (fDebugTracker) cout << "   also device under test=" << vr << endl;
   fListTestDevs[dut++] = ((DPlane*)fPlaneArray->At(pl-1))->GetPlaneNumber();
   }
   }

   else {
   cout << endl << "WARNING plane " << pl << " has an unknown status " << planeStatus << endl;
   }

   //      if ( planeAnalysisMode==1 && planeStatus!=3 ) { // consider all strips as seed for tracking, JB 2012/08/23
   //		    if (fDebugTracker) cout << " seed ref=" << fx << endl;
   //		    fListTrackSeedDevs[ts++]   = ((DPlane*)fPlaneArray->At(pl-1))->GetPlaneNumber();
   //		  }

   } // end if readout !=0
   else {
   if (fDebugTracker) cout << " not analyzed -> out of tracker!" << endl;
   }
   } // end loop on planes
   cout << endl;
   }
   */
  
  fTracksFinder = fc->GetTrackerPar().TracksFinder; // JB 2013/07/17, then  VR 2014/06/29
  
  fTrackSeedDevs = ts; // number of track seed planes
  fFixRefDevs = fx; // number of fixed reference planes
  fVarRefDevs = vr; // number of variable reference planes
  fTestDevs   = dut;// number of DUT planes

  if( fTracksMaximum>0 ) { // if tracking required
    
    printf("DTracker, Track Seed         : %d:",ts);
    for( Int_t ip=0; ip<fTrackSeedDevs; ip++) { printf( " %d", fListTrackSeedDevs[ip]); }
    printf( ".\n");
    printf("DTracker, Fixed References   : %d:",fx);
    for( Int_t ip=0; ip<fFixRefDevs; ip++) { printf( " %d", fListFixRefDevs[ip]); }
    printf( ".\n");
    printf("DTracker, Variable References: %d:",vr);
    for( Int_t ip=0; ip<fVarRefDevs; ip++) { printf( " %d", fListVarRefDevs[ip]); }
    printf( ".\n");
    printf("DTracker, Detector under Test: %d:",dut);
    for( Int_t ip=0; ip<fTestDevs; ip++) { printf( " %d", fListTestDevs[ip]); }
    printf( ".\n");

    cout << endl << " ***** Tracking and vertexing configuration ***** " << endl << endl ;
    
    switch(fTracksFinder)
    {
      case 0 :
        cout << " -> The tracking method for pixel telescope is the original one : find_tracks()" << endl;
        cout << "   -> Using " << fSearchHitDistance << " max um distance between hits to a track" << endl;
        break;
      case 1 :
        cout << " -> The tracking method for pixel telescope is the one with more options : find_tracks_1_opt()" << endl;
        cout << "   -> Using " << fSearchHitDistance <<     " um max distance between hits to do the pre-track" << endl;
        cout << "   -> Using " << fSearchMoreHitDistance << " um max distance between hits and the pre-track to associate them" << endl;
        cout << "   -> Using the TrackingPlaneOrderType type " << fTrackingPlaneOrderType << endl;
        break;
      case 2 :
        cout << " -> The tracking method for pixel telescope is the IVI one : find_tracks_2_ivi()" << endl;
        //---------------------------------
        //   Tracking Pass
        //---------------------------------
        fTrackingPass           = fc->GetTrackerPar().TrackingPass;
        fPreTrackHitsNbMinimum  = fc->GetTrackerPar().PreTrackHitsNbMinimum;
        fPreTrackHitsTypeUsed   = fc->GetTrackerPar().PreTrackHitsTypeUsed;
        fPreTrackHitsMaxDist    = fc->GetTrackerPar().PreTrackHitsMaxDist;
        fExtTrackHitsNbMinimum  = fc->GetTrackerPar().ExtTrackHitsNbMinimum;
        fExtTrackHitsTypeUsed   = fc->GetTrackerPar().ExtTrackHitsTypeUsed;
        fExtTrackHitsMaxDist    = fc->GetTrackerPar().ExtTrackHitsMaxDist;
        fFullTrackHitsNbMinimum = fc->GetTrackerPar().FullTrackHitsNbMinimum;
        
        cout << "   -> Doing " << fTrackingPass << " pass : " << endl;
        for (Int_t iPass = 1 ; iPass <= fTrackingPass ; iPass++)
        {
          cout << "     -> Pass " << iPass << " : " << endl ;
          cout << "       -> Pre-tracks parameters :" << endl ;
          cout << "         ->  PreTrackHitsNbMinimum  = " << fPreTrackHitsNbMinimum[iPass-1] << " hits" << endl ;
          cout << "         ->  PreTrackHitsTypeUsed   = " << fPreTrackHitsTypeUsed[iPass-1];
          switch(fPreTrackHitsTypeUsed[iPass-1])
          {
            case 0 :
              cout << " : all hits can be used" << endl ;
              break;
            case 1 :
              cout << " : only new hits can be used" << endl ;
              break;
            case 2 :
              cout << " : only memorized hits can be used" << endl ;
              break;
            default :
              cout << " : !!! UNKNOWN !!! All hits will be used instead" << endl ;
              fPreTrackHitsTypeUsed[iPass-1] = 0 ;
              break;
          }
          cout << "         ->  PreTrackHitsMaxDist    = " << fPreTrackHitsMaxDist[iPass-1]   << " um" << endl ;
          cout << "       -> Ext-tracks parameters :" << endl ;
          cout << "         ->  ExtTrackHitsNbMinimum  = " << fExtTrackHitsNbMinimum[iPass-1] << " hits" << endl ;
          cout << "         ->  PreTrackHitsTypeUsed   = " << fExtTrackHitsTypeUsed[iPass-1] ;
          switch(fExtTrackHitsTypeUsed[iPass-1])
          {
            case 0 :
              cout << " : all hits can be used" << endl ;
              break;
            case 1 :
              cout << " : only new hits can be used" << endl ;
              break;
            case 2 :
              cout << " : only memorized hits can be used" << endl ;
              break;
            default :
              cout << " : !!! UNKNOWN !!! All hits will be used instead" << endl ;
              fPreTrackHitsTypeUsed[iPass-1] = 0 ;
              break;
          }
          cout << "         ->  ExtTrackHitsMaxDist    = " << fExtTrackHitsMaxDist[iPass-1] << " um" << endl ;
          cout << "       -> Full-tracks parameters :" << endl ;
          cout << "         ->  FullTrackHitsNbMinimum = " << fFullTrackHitsNbMinimum[iPass-1] << " hits" << endl ;
        }
        cout << endl;
        
        //---------------------------------
        //   Tracking Scann pass
        //---------------------------------
        Int_t nbPlanes, PlaneNb;
        fTrackingOrderLines     = fc->GetTrackerPar().TrackingOrderLines;
        cout << "   -> Doing " << fTrackingOrderLines << " scans for each tracking pass : " << endl;
        fTrackingOrderPreTrack  = new Int_t*[fTrackingOrderLines];
        fTrackingOrderExtTrack  = new Int_t*[fTrackingOrderLines];
        
        for (Int_t iStrategy = 0 ; iStrategy < fTrackingOrderLines ; iStrategy++)
        {
          cout << "     -> Tracking scan " << iStrategy +1 << " :" << endl ;
          //---------------------
          // Pre tracks
          //---------------------
          nbPlanes = fc->GetTrackerPar().TrackingOrderPreTrack[iStrategy][0];
          cout << "       -> First " << nbPlanes << " planes are scanned to try to build pre-tracks : "<< endl;
          fTrackingOrderPreTrack[iStrategy] = new Int_t[nbPlanes+1];
          fTrackingOrderPreTrack[iStrategy][0] = nbPlanes ;
          
          for (Int_t iPlane=1 ; iPlane <= nbPlanes ; iPlane++)
          {
            PlaneNb = fc->GetTrackerPar().TrackingOrderPreTrack[iStrategy][iPlane]-1;//WARNING take care with the -1 !
            fTrackingOrderPreTrack[iStrategy][iPlane] = PlaneNb;
            cout << "         -> " << iPlane << ") plane " << PlaneNb+1 << " : " << ((DPlane*)(fPlaneArray->At(PlaneNb)))->GetPlanePurpose() << endl;
          }
          
          //---------------------
          // Ext tracks
          //---------------------
          nbPlanes = fc->GetTrackerPar().TrackingOrderExtTrack[iStrategy][0];
          cout << "       -> Then " << nbPlanes << " planes are scanned to try to extend each pre-track : "<< endl;
          fTrackingOrderExtTrack[iStrategy] = new Int_t[nbPlanes+1];
          fTrackingOrderExtTrack[iStrategy][0] = nbPlanes ;
          
          for (Int_t iPlane=1 ; iPlane <= nbPlanes ; iPlane++)
          {
            PlaneNb = fc->GetTrackerPar().TrackingOrderExtTrack[iStrategy][iPlane]-1;
            fTrackingOrderExtTrack[iStrategy][iPlane] = PlaneNb;
            cout << "         -> " << iPlane << ") plane " << PlaneNb+1 << " : " << ((DPlane*)(fPlaneArray->At(PlaneNb)))->GetPlanePurpose() << endl;
          }
        }
        break;
      case 3 :
        cout << " -> BEAST II Pattern recognition on PLUME Ladders " << endl;
        fBeaster = new DBeaster(this);
        break;
      default :
        cout << " -> The tracking method for pixel telescope is unknown !!!" << endl;
        if(fTracksMaximum) gApplication->Terminate();
        break;
    }

  } // end if tracking required

  fSearchHitDistance          = (Double_t)fc->GetTrackerPar().SearchHitDistance; // JB, 2009/05/25
  fSearchMoreHitDistance      = (Double_t)fc->GetTrackerPar().SearchMoreHitDistance; // VR, 2014/06/29
  fKeepUnTrackedHitsBetw2evts = fc->GetTrackerPar().KeepUnTrackedHitsBetw2evts; // VR, 2014/08/26
  fTrackingPlaneOrderType     = (Int_t)   fc->GetTrackerPar().TrackingPlaneOrderType;// VR, 2014/07/14


	// Init of fRequiredHits removed since it is call by SetAlignStatus, JB 2013/08/27
  if(fTracksMaximum) SetAlignmentStatus(2); // assume alignment of tracker is done by default, JB 2011/06/21

  fVertexMaximum = fc->GetTrackerPar().VertexMaximum;
  if(fVertexMaximum) cout << " Vertex will be searched for (" << fVertexMaximum << " maximum)." << endl;

  fUseSlopeInExtrapolation = fc->GetTrackerPar().UseSlopeInTrackFinder; // JB 2013/06/21
  fUseVertexConstraint = fc->GetTrackerPar().VertexConstraint; // JB 2013/06/21


  fTrack         = new DTrack*[fTracksMaximum];
  fTracksN = 0;
  fTrackVoid     = new DTrack( fHitsMaximum);
  for (Int_t tr = 0; tr < fTracksMaximum; tr++)
	{
    fTrack[tr]   = new DTrack( fHitsMaximum);
    fTrack[tr]->SetMaxHitsPerPlane( fc->GetTrackerPar().HitsInPlaneTrackMaximum); // JB 2009/05/25
    //fTrack[tr]->SetVertex(0.,0.,0.);
  }

  // Counter of tracks over all event
  // [0]= all tracks, [i>0]=track with i planes
  // JB 2009/09/08
  fTrackCount = new Int_t[fPlanesN];
  for( Int_t ip=0; ip<fPlanesN; ip++ ) {
    fTrackCount[ip] = 0;
  }
  // [0]= all tracks, [i>0]= #track with plane i
  // JB 2014/08/29
  fTrackCountPerPlane = new Int_t[fPlanesN+1]; // correction, JB 2015/03/17
  for( Int_t ip=0; ip<fPlanesN+1; ip++ ) {
    fTrackCountPerPlane[ip] = 0;
  }

  // Variables to handle subtracks
  // JB 2014/12/15
  fSubTrackPlanesN = fc->GetTrackerPar().SubtrackNplanes; // 2
  if ( fTracksMaximum>0 && fSubTrackPlanesN>0 ) {
    printf( "\nDTracker will make subtracks of %d hits out of full tracks (%d hits):\n  with planes ", fSubTrackPlanesN, fHitsMaximum);
    fSubTrackPlaneIds = new Int_t[fSubTrackPlanesN];
    fSubTrack         = new DTrack*[fTracksMaximum];
//    fSubTrackPlaneIds[0] = 3; // adapted to M34 analysis August 2013 DESY
//    fSubTrackPlaneIds[1] = 4;
//    printf( "%d, %d\n", fSubTrackPlaneIds[0], fSubTrackPlaneIds[1]);
    for (Int_t iplane=0; iplane<fSubTrackPlanesN; iplane++) {
      fSubTrackPlaneIds[iplane] = fc->GetTrackerPar().SubtrackPlanes[iplane];
      printf( "%d ", fSubTrackPlaneIds[iplane]);
    }
    printf("\n************************\n");
    for (Int_t tr = 0; tr < fTracksMaximum; tr++) {
      fSubTrack[tr] = new DTrack( fSubTrackPlanesN);
    }

  }
  else {
    fSubTrackPlaneIds = NULL;
    fSubTrack = NULL;
  }

  // Variables for Kalman tracks
  // QL 2016/05/26
  fTrackFittingAlg = fc->GetTrackerPar().TrackFittingAlg;
  if ( fTrackFittingAlg == kKalman){
  //     fKalEnabled = kTRUE;
  //else
  //     fKalEnabled = kFALSE;
  //if(fKalEnabled){
    printf("\nDTracker will use KalmanFilter to refit the track!\n");
    //fKalTrack = new DTrack[fTracksMaximum];
  }
  cout << endl << " -+-+- DTracker User Constructor DONE -+-+- " << endl;
}

//______________________________________________________________________________
//
DTracker::~DTracker()
{
  // DTracker default destructor

  delete [] fTrack;
  delete [] fHitList;
  delete [] fPlaneArray;
  delete [] fLadderArray;

  delete [] fTrackerNode;
  delete [] fTrackerGeometry;
  delete [] fListTrackSeedDevs;
  delete [] fListFixRefDevs;
  delete [] fListVarRefDevs;
  delete [] fListTestDevs;
  delete    fTrackVoid;

  delete [] fSubTrackPlaneIds; // JB 2014/12/15
  delete [] fSubTrack;

  //if(fKalEnabled){ // QL 2016/05/26
  //  delete [] fKalTrack;
  //}

}

//______________________________________________________________________________
//
DPlane* DTracker::GetPlane(Int_t aPlaneNumber)
{
  return (DPlane*)fPlaneArray->At(aPlaneNumber-1);
}

DLadder* DTracker::GetLadder(Int_t aLadderNumber)
{
  return (DLadder*)fLadderArray->At( aLadderNumber-1 );
}

//______________________________________________________________________________
//
void DTracker::Align(DTrack *aTrack)
{
  DPlane *p;

  for (Int_t pl = 0; pl < 1; pl++) {
    p = GetPlane(fListVarRefDevs[pl]);
    p->Align(aTrack, kTRUE);
  }

}

//______________________________________________________________________________
//
DTrack* DTracker::GetTrack(Int_t aTN)
{
  if(aTN <= fTracksN && aTN >= 1)
    return  fTrack[aTN-1];
  else{
    //    printf(" DTracker, request to nonexistent track #%d, returning Void-Track !\n",aTN);
    return  fTrackVoid;
  }
}

//______________________________________________________________________________
//
DTrack* DTracker::GetSubTrack(Int_t aTN)
{
  if(aTN <= fTracksN && aTN >= 1)
    return  fSubTrack[aTN-1];
  else{
    //    printf(" DTracker, request to nonexistent subtrack #%d, returning Void-Track !\n",aTN);
    return  fTrackVoid;
  }
}

//______________________________________________________________________________
//
void DTracker::SetAlignmentStatus(Int_t aStatusValue)
{
  // Set the alignment status of the tracker which drives
  // the types of planes used to build tracks
  // and the planes which is aligned in the AlignTracker procedure.
  // The minimal number of hits to make a track is set accordingly.
  //
  // Modified JB 2009/07/20
  // Modified JB 2009/09/01
  // Modified JB 2011/06/21
  // Modified JB 2012/08/28 add condition for strip-telescope
  // Modified JB 2013/08/19 correctly set required hits for status=0

  fAlignmentStatus = aStatusValue;

  cout << endl << "***********************************************" << endl;
  cout << "Tracker alignment status set to " << fAlignmentStatus << endl;

  if( fAlignmentStatus == 0 ) {
    cout << " Now only the " << fTrackSeedDevs << " fixed (primary) reference planes used for track seed are used in tracking." << endl << endl;
    SetRequiredHits( 1 ); // change from 1 to fTrackSeedDevs, JB 2013/08/19, and then changed back
  }

  else if( fAlignmentStatus == 1 ) {
    cout << " Now all the " << fFixRefDevs+fTrackSeedDevs << " fixed (primary) reference planes are used in tracking." << endl << endl;
    if( ((DPlane*)fPlaneArray->At(0))->GetAnalysisMode() == 1 ) { // STRIP telescope case
      cout << " --> STRIP tracking setup detected! <--" << endl;
      SetRequiredHits( fFixRefDevs+fTrackSeedDevs );
    }
    else { // PIXEL telescope case
      // here we assume that there is the same number of ref. facing each seed plane
      SetRequiredHits( fFixRefDevs/fTrackSeedDevs+1 );
    }
  }

  else if( fAlignmentStatus >= 2 ) {
    cout << " Now all reference planes ( " << fFixRefDevs+fTrackSeedDevs << " primaries and " << fVarRefDevs-fTestDevs << " secondaries) are used in tracking." << endl << endl;
    SetRequiredHits( fc->GetTrackerPar().PlanesForTrackMinimum );
  }

  else {
    cout << "WARNING: (" << aStatusValue << ") is not a valid option, assuming 2." << endl;
    SetAlignmentStatus(2);
//     fAlignmentStatus = 2;
//     cout << " Now all reference planes ( " << fFixRefDevs << " primaries and " << fVarRefDevs-fTestDevs << " secondaries) are used in tracking." << endl << endl;
//     SetRequiredHits( fc->GetTrackerPar().PlanesForTrackMinimum );
  }
  cout << "***********************************************" << endl;

}

//______________________________________________________________________________
//
void DTracker::SetDebug(Int_t aDebug)
{
  // Initialize or update the debug level
  // update also the level for planes
  // JB 2009/05/12
  // Modified: JB 2012/08/20 to set DTrack object debug level

  fDebugTracker = aDebug;

  for (Int_t pl = 1; pl <= fPlanesN; pl++) {
    GetPlane(pl)->SetDebug( aDebug);
  }

  for (Int_t tr = 0; tr < fTracksMaximum; tr++) {
    fTrack[tr]->SetDebug( aDebug);
  }

  cout << "DTracker debug updated to " << fDebugTracker << endl;
}

//______________________________________________________________________________
//
void DTracker::SetRequiredHits(Int_t aNumber)
{
  // JB 2009/09/18
  // Modified JB 2012/08/28 add condition for strip-telescope

  fRequiredHits=aNumber;
  cout<<endl<<"Number of planes required to make a track changed to "<<fRequiredHits<<endl<<endl;

  if(fTracksMaximum) { // test added JB 2011/06/21
    // test modified to cope with strip-telescope case without seed plane

    // STRIP telescope case
    if( ((DPlane*)fPlaneArray->At(0))->GetAnalysisMode() == 1 ) {
      if( fRequiredHits<2 ) Fatal("DTracker::SetRequiredHits", "----> ERROR, you are asking for tracking (TracksMaximum > 0) but not enough fixed strip-plane to define a track (Status: 0 or 1), STOPPING.");
      else if( fRequiredHits==2 ) Warning("DTracker::SetRequiredHits", "----> WARNING, you are asking for tracking (TracksMaximum > 0) but there are just enough (2) fixed strip-plane to define a track (Status: 0 or 1), tracks will have a ZERO SLOPE."); // 2 fixed strips allowed, JB 2013/08/27
    }

    // PIXEL telescope case
    else {
      if (fTrackSeedDevs==0) Fatal("DTracker::SetRequiredHits", "----> ERROR, you are asking for tracking (TracksMaximum > 0) but no plane has been declared as track seed (Status: 0), STOPPING.");
      else if (fRequiredHits==1) Warning("DTracker::SetRequiredHits"," ----> WARNING in DTracker, you have less than 2 fixed reference planes, tracks will have a ZERO SLOPE.\n"); // condition changed by JB, 2007 June
    }
  }

}

//______________________________________________________________________________
//
Int_t DTracker::Update()
{
  // Modified JB 2009/08/31, authorize track finding with 2 planes
  // Modified JB 2009/10/02, Init of plane
  // Modified JB 2013/06/11, Test to decide vertexing

  Int_t fOk = 0 ; // should stay at 0 if everything's OK
  Int_t fPlInit = 0 ; // to count how many planes are initialized

  for (Int_t plane = 1; plane <= fPlanesN; plane++) {
    //============
    fOk += (Int_t)(GetPlane(plane)->Update()); // fOK incremented if something's wrong
    //============
    // Check if the plane is initialized or not
    // also set init immediately in analysis mode >=100, JB 2009/10/02
    if( fc->GetPlanePar(plane).AnalysisMode>=100 || fc->GetPlanePar(plane).InitialNoise < fAcq->GetEventNumber()+1 ) fPlInit++; // JB 2009/05/26
    if(fDebugTracker) printf(" DTracker::Update plane %d updated, OK=%d, #init=%d \n", plane, fOk, fPlInit);
  }

  if (fKeepUnTrackedHitsBetw2evts)// VR 2014.08.28
  {
    if(fDebugTracker) printf("\n *-*-* Mecanism for adding previous'event'untracked'hits starts *-*-* \n\n");
    for( Int_t iPlane=0; iPlane < fPlanesN; iPlane++ )
    {
      DPlane *aPlane = (DPlane*)fPlaneArray->At(iPlane);
      if(fDebugTracker) printf(" *** Plane %d : %s ***\n",iPlane+1, aPlane->GetPlanePurpose() );

      // Add last'event'unstracked'hits to the current list
      aPlane->SetHitsNewN(aPlane->GetHitsN());
      aPlane->SetHitsOldN(aPlane->GetHitsUnTrackedLastEventN());
      aPlane->SetHitsN   (aPlane->GetHitsNewN() + aPlane->GetHitsOldN());
      if(fDebugTracker) printf("New hits : %d ; Old hits : %d ; Total hits : %d.\n",aPlane->GetHitsNewN(), aPlane->GetHitsOldN(), aPlane->GetHitsN());


      if(fDebugTracker) printf("->Setting new hits as new ones : hits # ");
      for (Int_t inewHit = 1 ; inewHit <= aPlane->GetHitsNewN() ; inewHit++)
      {
        aPlane->GetHit(inewHit)->SetIsFromPreviousEvent(0);
        if(fDebugTracker) printf("%d [%p]{%.0f;%.0f;%.0f}", aPlane->GetHit(inewHit)->GetNumber(),aPlane->GetHit(inewHit),aPlane->GetHit(inewHit)->GetPositionUhit(),aPlane->GetHit(inewHit)->GetPositionVhit(),aPlane->GetHit(inewHit)->GetPositionWhit() );
      }
      if(fDebugTracker) printf("\n");


      if(fDebugTracker) printf("->Adding olds hits to current list : hits # (hits memorized #) : ");
      for (Int_t ioldHit = 1 ; ioldHit <= aPlane->GetHitsOldN() ; ioldHit++)
      {
        aPlane->GetHit(aPlane->GetHitsNewN() + ioldHit)->clone(aPlane->GetHitUnTrackedLastEvent(ioldHit),0);
        //aPlane->GetHit((aPlane->GetHitsNewN() + ioldHit))->SetNumber((aPlane->GetHitsNewN() + ioldHit));
        if(fDebugTracker) printf("%d[%p]{%.0f;%.0f;%.0f} (%d[%p]{%.0f;%.0f;%.0f}) ", aPlane->GetHit((aPlane->GetHitsNewN() + ioldHit))->GetNumber(),aPlane->GetHit((aPlane->GetHitsNewN() + ioldHit)),aPlane->GetHit((aPlane->GetHitsNewN() + ioldHit))->GetPositionUhit(),aPlane->GetHit((aPlane->GetHitsNewN() + ioldHit))->GetPositionVhit(),aPlane->GetHit((aPlane->GetHitsNewN() + ioldHit))->GetPositionWhit(), aPlane->GetHitUnTrackedLastEvent(ioldHit)->GetNumber(), aPlane->GetHitUnTrackedLastEvent(ioldHit),aPlane->GetHitUnTrackedLastEvent(ioldHit)->GetPositionUhit(),aPlane->GetHitUnTrackedLastEvent(ioldHit)->GetPositionVhit(),aPlane->GetHitUnTrackedLastEvent(ioldHit)->GetPositionWhit());
      }
      if(fDebugTracker) printf("\n\n");

    }
    if(fDebugTracker) printf("\n *-*-* Mecanism for adding previous'event'untracked'hits finished *-*-* \n\n");
  }

  if (fPlanesStatus==0) { // still in initializing step
    // Update the status if all planes have been initialized
    if(fPlInit==fPlanesN) {
      fPlanesStatus = 1 ;
      cout << endl << "The Tracker status just changed to " << fPlanesStatus << endl;
    }
  }
  else { // initialize step done
    // If trackig required
    if(fPlanesN>=2 && fTracksMaximum){ // check config file asks for tracks (JB, June 2007, > upgraded to >= JB 2009/08/31)
      // Check if we build tracks with the strip telescope or not
      //============

      if( GetPlane(1)->GetAnalysisMode()==1 ) { // strip-telescope
        find_tracks_withStrips();
      }
      else { // pixel-telescope
	// VR 2014/06/29 tracking method depends on TracksFinder config.
        if(fTracksFinder==0)       find_tracks();
        else if (fTracksFinder==1) find_tracks_1_opt();
        else if (fTracksFinder==2) find_tracks_2_ivi();
        else if (fTracksFinder==3){
          std::cout << "***** find_tracks_Beast *****" << '\n';
           find_tracks_Beast();
         }
      }
      if( fVertexMaximum ) { // If vertexing required, JB/LC 2013/06/11
        find_vertex();
        find_tracks_and_vertex();
      }
      //============
      //if (1<=fTracksN && fTracksN<=fTracksMaximum) fOk--; // condition changed by JB, 2007 June
      //if(fDebugTracker) printf("DTracker::Update after find_tracks tracksN=%d, OK=%d\n", fTracksN, fOk);
    }
    else if(fTracksMaximum){// if no tracks found while tracking requested, issue a warning
      cout << " Event " << fAcq->GetRealEventNumber() << " no track finding because less than 2 planes (calibration?)" << endl;// only warns the use if fTrackmaximum was not set to 0 in cfg file
      fTracksN=0;
    }
  }

  if (fKeepUnTrackedHitsBetw2evts) {// VR 2014.08.28
    if(fDebugTracker) printf("\n *-*-* Mecanism for saving current'event'untracked'hits starts *-*-* \n\n");
    for( Int_t iPlane=0; iPlane < fPlanesN; iPlane++ ) {
      DPlane *aPlane = (DPlane*)fPlaneArray->At(iPlane);
      if(fDebugTracker) printf(" *** Plane %d : %s ***\n",iPlane+1, aPlane->GetPlanePurpose() );

      Int_t UnTracked = 0;
      if(fDebugTracker) printf("->Copying untracked hits to the untrack hits memory : hits memorized # : ");
      for (Int_t iHit = 1 ; iHit <= aPlane->GetHitsNewN() ; iHit++)
      {
        if (!aPlane->GetHit(iHit)->GetFound() && !aPlane->GetHit(iHit)->GetIsFromPreviousEvent())
        {
          aPlane->GetHitUnTrackedLastEvent(UnTracked+1)->clone(aPlane->GetHit(iHit),0);
          aPlane->GetHitUnTrackedLastEvent(UnTracked+1)->SetIsFromPreviousEvent(1);
          if(fDebugTracker) printf("%d[%p]{%.0f;%.0f;%.0f} ", aPlane->GetHitUnTrackedLastEvent(UnTracked+1)->GetNumber(), aPlane->GetHitUnTrackedLastEvent(UnTracked+1),aPlane->GetHitUnTrackedLastEvent(UnTracked+1)->GetPositionUhit(),aPlane->GetHitUnTrackedLastEvent(UnTracked+1)->GetPositionVhit(),aPlane->GetHitUnTrackedLastEvent(UnTracked+1)->GetPositionWhit());
          UnTracked += 1;
        }
      }
      if(fDebugTracker) printf("\n");

      aPlane->SetHitsUnTrackedLastEventN(UnTracked);
      if(fDebugTracker) printf("%d untracked hits memorized !\n\n",UnTracked);
    }
    if(fDebugTracker) printf("\n *-*-* Mecanism for saving current'event'untracked'hits finished *-*-* \n\n");
  }



  if(fDebugTracker) printf("DTracker::Update finaly OK=%d\n", fOk);
  return fOk;
}

//______________________________________________________________________________
//
Int_t DTracker::UpdateMC()
{

  // Function to do tracking of hit generated by MC code which generates tracks
  // including with multiple scattering and sensor spatial resolution
  // Modified AP 2015/03/11

  Int_t fOk     = 0; // should stay at 0 if everything's OK
  Int_t fPlInit = 0; // to count how many planes are initialized

  for (Int_t plane = 1; plane <= fPlanesN; plane++) {
    //============
    //fOk += (Int_t)(GetPlane(plane)->Update()); // fOK incremented if something's wrong
    //============
    // Check if the plane is initialized or not
    // also set init immediately in analysis mode >=100, JB 2009/10/02
    //if( fc->GetPlanePar(plane).AnalysisMode>=100 || fc->GetPlanePar(plane).InitialNoise < fAcq->GetEventNumber()+1 ) fPlInit++; // JB 2009/05/26
    fPlInit++;
    if(fDebugTracker) printf(" DTracker::Update plane %d updated, OK=%d, #init=%d \n", plane, fOk, fPlInit);
  }

  if(fKeepUnTrackedHitsBetw2evts){ // VR 2014.08.28
    if(fDebugTracker) printf("\n *-*-* Mecanism for adding previous'event'untracked'hits starts *-*-* \n\n");
    for(Int_t iPlane=0; iPlane < fPlanesN; iPlane++ ) {
      DPlane *aPlane = (DPlane*)fPlaneArray->At(iPlane);
      if(fDebugTracker) printf(" *** Plane %d : %s ***\n",iPlane+1, aPlane->GetPlanePurpose() );

      // Add last'event'unstracked'hits to the current list
      aPlane->SetHitsNewN(aPlane->GetHitsN());
      aPlane->SetHitsOldN(aPlane->GetHitsUnTrackedLastEventN());
      aPlane->SetHitsN   (aPlane->GetHitsNewN() + aPlane->GetHitsOldN());
      if(fDebugTracker) printf("New hits : %d ; Old hits : %d ; Total hits : %d.\n",aPlane->GetHitsNewN(), aPlane->GetHitsOldN(), aPlane->GetHitsN());


      if(fDebugTracker) printf("->Setting new hits as new ones : hits # ");
      for (Int_t inewHit = 1 ; inewHit <= aPlane->GetHitsNewN() ; inewHit++) {
        aPlane->GetHit(inewHit)->SetIsFromPreviousEvent(0);
        if(fDebugTracker) printf("%d [%p]{%.0f;%.0f;%.0f}",
				 aPlane->GetHit(inewHit)->GetNumber(),
				 aPlane->GetHit(inewHit),
				 aPlane->GetHit(inewHit)->GetPositionUhit(),
				 aPlane->GetHit(inewHit)->GetPositionVhit(),
				 aPlane->GetHit(inewHit)->GetPositionWhit());
      }
      if(fDebugTracker) printf("\n");


      if(fDebugTracker) printf("->Adding olds hits to current list : hits # (hits memorized #) : ");
      for (Int_t ioldHit = 1 ; ioldHit <= aPlane->GetHitsOldN() ; ioldHit++) {
        aPlane->GetHit(aPlane->GetHitsNewN() + ioldHit)->clone(aPlane->GetHitUnTrackedLastEvent(ioldHit),0);
        if(fDebugTracker) printf("%d[%p]{%.0f;%.0f;%.0f} (%d[%p]{%.0f;%.0f;%.0f}) ",
				 aPlane->GetHit((aPlane->GetHitsNewN() + ioldHit))->GetNumber(),
				 aPlane->GetHit((aPlane->GetHitsNewN() + ioldHit)),
				 aPlane->GetHit((aPlane->GetHitsNewN() + ioldHit))->GetPositionUhit(),
				 aPlane->GetHit((aPlane->GetHitsNewN() + ioldHit))->GetPositionVhit(),
				 aPlane->GetHit((aPlane->GetHitsNewN() + ioldHit))->GetPositionWhit(),
				 aPlane->GetHitUnTrackedLastEvent(ioldHit)->GetNumber(),
				 aPlane->GetHitUnTrackedLastEvent(ioldHit),
				 aPlane->GetHitUnTrackedLastEvent(ioldHit)->GetPositionUhit(),
				 aPlane->GetHitUnTrackedLastEvent(ioldHit)->GetPositionVhit(),
				 aPlane->GetHitUnTrackedLastEvent(ioldHit)->GetPositionWhit());
      }
      if(fDebugTracker) printf("\n\n");

    }
    if(fDebugTracker) printf("\n *-*-* Mecanism for adding previous'event'untracked'hits finished *-*-* \n\n");
  }

  if(fPlanesStatus==0) { // still in initializing step
    // Update the status if all planes have been initialized
    if(fPlInit==fPlanesN) {
      fPlanesStatus = 1;
      cout << endl << "The Tracker status just changed to " << fPlanesStatus << endl;
    }
  }

  if(fPlanesStatus==0) { // still in initializing step
    // Update the status if all planes have been initialized
    if(fPlInit==fPlanesN) {
      fPlanesStatus = 1;
      cout << endl << "The Tracker status just changed to " << fPlanesStatus << endl;
    }
  }
  else { // initialize step done
    // If trackig required
    if(fPlanesN>=2 && fTracksMaximum) { // check config file asks for tracks (JB, June 2007, > upgraded to >= JB 2009/08/31)
      // Check if we build tracks with the strip telescope or not
      //============
      if(GetPlane(1)->GetAnalysisMode()==1) { // strip-telescope
        find_tracks_withStrips();
      }
      else { // pixel-telescope
	// VR 2014/06/29 tracking method depends on TracksFinder config.
        if(fTracksFinder==0) {
	  //cout << "DTracker::UpdateMC:: Calling function find_tracks()" << endl;
	  find_tracks();
	}
	else if (fTracksFinder==1) find_tracks_1_opt();
        else if (fTracksFinder==2) find_tracks_2_ivi();
        else if (fTracksFinder==3) find_tracks_Beast();
      }
      if( fVertexMaximum ) { // If vertexing required, JB/LC 2013/06/11
        find_vertex();
        find_tracks_and_vertex();
      }
      //============
      //if (1<=fTracksN && fTracksN<=fTracksMaximum) fOk--; // condition changed by JB, 2007 June
      //if(fDebugTracker) printf("DTracker::Update after find_tracks tracksN=%d, OK=%d\n", fTracksN, fOk);
    }
    else if(fTracksMaximum){// if no tracks found while tracking requested, issue a warning
      // only warns the use if fTrackmaximum was not set to 0 in cfg file
      cout << " Event " << fAcq->GetRealEventNumber() << " no track finding because less than 2 planes (calibration?)" << endl;
      fTracksN=0;
    }
  }

  if (fKeepUnTrackedHitsBetw2evts) {// VR 2014.08.28
    if(fDebugTracker) printf("\n *-*-* Mecanism for saving current'event'untracked'hits starts *-*-* \n\n");
    for( Int_t iPlane=0; iPlane < fPlanesN; iPlane++ ) {
      DPlane *aPlane = (DPlane*)fPlaneArray->At(iPlane);
      if(fDebugTracker) printf(" *** Plane %d : %s ***\n",iPlane+1, aPlane->GetPlanePurpose() );

      Int_t UnTracked = 0;
      if(fDebugTracker) printf("->Copying untracked hits to the untrack hits memory : hits memorized # : ");
      for (Int_t iHit = 1 ; iHit <= aPlane->GetHitsNewN() ; iHit++) {
        if (!aPlane->GetHit(iHit)->GetFound() && !aPlane->GetHit(iHit)->GetIsFromPreviousEvent()) {
          aPlane->GetHitUnTrackedLastEvent(UnTracked+1)->clone(aPlane->GetHit(iHit),0);
          aPlane->GetHitUnTrackedLastEvent(UnTracked+1)->SetIsFromPreviousEvent(1);
          if(fDebugTracker) printf("%d[%p]{%.0f;%.0f;%.0f} ", aPlane->GetHitUnTrackedLastEvent(UnTracked+1)->GetNumber(), aPlane->GetHitUnTrackedLastEvent(UnTracked+1),aPlane->GetHitUnTrackedLastEvent(UnTracked+1)->GetPositionUhit(),aPlane->GetHitUnTrackedLastEvent(UnTracked+1)->GetPositionVhit(),aPlane->GetHitUnTrackedLastEvent(UnTracked+1)->GetPositionWhit());
          UnTracked += 1;
        }
      }
      if(fDebugTracker) printf("\n");

      aPlane->SetHitsUnTrackedLastEventN(UnTracked);
      if(fDebugTracker) printf("%d untracked hits memorized !\n\n",UnTracked);
    }
    if(fDebugTracker) printf("\n *-*-* Mecanism for saving current'event'untracked'hits finished *-*-* \n\n");
  }

  if(fDebugTracker) printf("DTracker::Update finaly OK=%d\n", fOk);

  return fOk ;

}
//_____________________________________________________________________________
//
void DTracker::find_tracks(){
  // Original method to find tracks (TracksFinder==0)
  //--------------------------------
  // Try to make a track with all hits of the "seed" reference planes (status=0).
  // For each of such hits, a track is extrapolated to the other planes and
  //  the nearest hit within a search window is added to the track. If no hit
  //  lies within the search window, the plane is skipped.
  // When all planes have been searched, the track is fitted with all its associated hits.
  //
  // Note that there are two options for the track extrapolation:
  //  fUseSlopeInExtrapolation==0 => done with no slope,
  //    that means perpendicularly with respect to the seed plane.
  //  fUseSlopeInExtrapolation >0 => done with the current track slope.
  //  The parameter fUseSlopeInExtrapolation is set through the
  //
  // Planes are used in the tracking depending on their status
  //  and the value of the Tracker Align Status. The rule is that the plane
  //  is included if PlaneStatus<=TrackerAlignStatus.
  //
  // This method has been completely reshaped to allow for multi-track event
  //  and best hit selection.
  //
  //
  // JB 2009/05/22
  // Modified: JB 2009/08/25, track numbering
  // Modified: JB 2009/09/01, no more update of fRequiredHits
  // Modified: JB 2011/03/14, use several sensors for track seed
  // Modified: JB 2011/07/25, search the next hit along the temporary track path
  // Modified: SS 2011/10/26, rollback of the calculation of the hit-track distance
  // Modified: JB 2012/04/02, fix in condition to include planes depending on alignment
  // Modified: JB 2012/09/07, new condition on firstHit
  // Modified: JB 2013/06/21, new parameter fUseSlopeInExtrapolation
  // Modified: VR 2014/06/29, bug fixed : hits not associated to a track a cleared even if fRequiredHits is not reached

  DPlane *aPlane = NULL;
  DHit   *aHit   = NULL;
  DTrack  aTrack( fPlanesN);
  aTrack.SetDebug( fDebugTracker);
  DR3     extrapolation;
  Int_t   oldfHits = 0;
  DPlane *oldPlane = NULL;
  Double_t minDistance = 1.e9;
  Double_t aDistance, distanceWithoutSlope, distancewithSlope;
  DHit *bestHit=0;
  Float_t tPlaneResolution=fc->GetTrackerPar().Resolution;
  DR3 hitPosition; // usefull for temporary computation check

  // fRequiredHits is no longer updated here, JB 2009/09/01
  if( fDebugTracker) printf(" DTracker::find_tracks %d hits required, alignStatus %d\n", fRequiredHits, fAlignmentStatus);

  // Reset counters and arrays
  fTracksN = 0;
  for(Int_t trk = 1; trk <= fTracksMaximum; trk++)  fTrack[trk-1]->Reset();

  // ==================
  // Loop on reference planes usable for track seed
  DPlane *firstPlane;
  for( Int_t iseed=0; iseed<fTrackSeedDevs; iseed++ ) { // loop on seed planes

    firstPlane = GetPlane(fListTrackSeedDevs[iseed]);

    // ==================
    // Loop on all hits of the track seed plane
    // and try to make a track out of each one
    if(fDebugTracker) printf(" DTracker::find_tracks first plane is %d (%s) with %d hits\n",
			     firstPlane->GetPlaneNumber(),
			     firstPlane->GetPlanePurpose(),
			     firstPlane->GetHitsN());

    for( Int_t iFirstHit=1; iFirstHit<=firstPlane->GetHitsN() ; iFirstHit++) { // loop on first hits, pay attention that hit numbering start at 1
      if( fTracksN >= fTracksMaximum ) break; // if max track number reach, stop
      fHits    = 0;
      oldfHits = 0;
      oldPlane   = firstPlane;
      for(Int_t hit = 0; hit < fHitsMaximum; hit++)  fHitList[hit]=0;
      if(fDebugTracker>1) cout << "   DTracker::find_tracks Hit list reseted" << endl;

      // test if the hit was associated before,
      // this test allows to manage several firstPlanes
      // JB 2012/09/07
      if( firstPlane->GetHit( iFirstHit )->GetFound() ) continue;

      fHitList[fHits] = firstPlane->GetHit( iFirstHit );
      fHitList[fHits]->SetFound(); // not really needed, because never tested in the following
      if( fDebugTracker) printf("  DTracker::find_tracks hit %d number %d from plane %d (%s) found %d at (%.1f, %.1f)\n",
				fHits,
				fHitList[fHits]->GetNumber(),
				firstPlane->GetPlaneNumber(),
				firstPlane->GetPlanePurpose(),
				fHitList[fHits]->GetFound(),
				fHitList[fHits]->GetPositionUhit(),
				fHitList[fHits]->GetPositionVhit());
      fHits++;

      // ==================
      // Loop on all planes to find a matching hit in them
      // exclude the first plane
      // always exclude DUT
      // exclude Variable refenreces if alignement is not done yet
      for( Int_t iPlane=0; iPlane< fPlanesN; iPlane++ ) { // loop on planes
        aPlane = (DPlane*)fPlaneArray->At(iPlane);

        // check plane is valid
        if( aPlane->GetPlaneNumber() != firstPlane->GetPlaneNumber()
           //&& !( aPlane->GetStatus()==0) // not mandatory but could be faster, JB 2011/03/14. However, you need to comment this to have all the reference plane in status 0
           && ( aPlane->GetStatus()<=fAlignmentStatus ) // condition changed, JB 2012/04/02
           && !( aPlane->GetStatus()==3 ) ) { // if valid plane
          if(fDebugTracker>1) printf("  DTracker::find_tracks checking plane %d (%s) with %d hits\n",
				     aPlane->GetPlaneNumber(),
				     aPlane->GetPlanePurpose(),
				     aPlane->GetHitsN());

          // find the extrapolation of the current track to this plane:
          //  if the number of hits in the track has changed -> recompute the track,
          //  if the plane has changed -> recompute the intersection.
          // JB 2011/07/25
          if( fHits != oldfHits) {
            aTrack.Analyze( -1, fHitList, fHits, tPlaneResolution);
            if( fDebugTracker) printf(" track origin (%.1f, %.1f, %.1f) slope (%.1f, %.1f, %.1f)\n",
				      aTrack.GetLinearFit().GetOrigin()(0), aTrack.GetLinearFit().GetOrigin()(1), aTrack.GetLinearFit().GetOrigin()(2),
				      aTrack.GetLinearFit().GetSlopeZ()(0), aTrack.GetLinearFit().GetSlopeZ()(1), aTrack.GetLinearFit().GetSlopeZ()(2));
            oldfHits = fHits;
          }

          if( oldPlane->GetPlaneNumber() != aPlane->GetPlaneNumber() ) {
            extrapolation = aPlane->Intersection( &aTrack); //aTrack.Intersection( aPlane);
            if( fDebugTracker) printf(" track extrapolation at plane %d (%s) (z=%.1f) uvw= (%.1f, %.1f, %.1f)\n",
				      aPlane->GetPlaneNumber(),
				      aPlane->GetPlanePurpose(),
				      aPlane->GetPosition()(2),
				      extrapolation(0), extrapolation(1), extrapolation(2));
            oldPlane = aPlane;
          }

          // loop on all hits of this plane and keep the nearest one
          minDistance = 1.e9;
          aDistance=distanceWithoutSlope=distancewithSlope=1.e9;
          bestHit=0;
          for( Int_t iHit=1; iHit <= aPlane->GetHitsN(); iHit++ ) { // loop on plane hits
            aHit = aPlane->GetHit( iHit);
            if( fDebugTracker>1) printf("   DTracker::find_tracks trying for hit %d number %d from plane %d (%s) found %d\n",
					fHits, aHit->GetNumber(),
					aPlane->GetPlaneNumber(),
					aPlane->GetPlanePurpose(),
					aHit->GetFound());
            if( aHit->GetFound()==kTRUE ) continue; // skip hit already found

            distanceWithoutSlope = (fHitList[0])->Distance( aHit);
            distancewithSlope    = aHit->Distance( &extrapolation); // JB 2011/07/25

            //-------- IMPORTANT CHOICE for track extrapolation
            // switch the value of aDistance between distanceWithoutSlope or distancewithSlope
            // SS 2011/10/26 - rollback to the method with no slope.
            // now choice made by new parameter, JB 2013/06/21
            if(fUseSlopeInExtrapolation > 0) aDistance = distancewithSlope;
            else                             aDistance = distanceWithoutSlope;

            //-----------------------

            if(fDebugTracker > 1) {
              hitPosition.SetValue( *(fHitList[0]->GetPosition()));
              hitPosition = fHitList[0]->GetPlane()->PlaneToTracker( hitPosition);
              hitPosition = aPlane->TrackerToPlane( hitPosition);
              printf("#hits=%d (uvw) hit0(%.1f,%.1f)  extra(%.1f,%.1f)   newHit(%.1f,%.1f)   distWOslope=%.1f, distWslope=%.1f (lim %f)\n",
		     fHits,
		     hitPosition(0),
		     hitPosition(1),
		     extrapolation(0),
		     extrapolation(1),
		     aHit->GetPositionUhit(),
		     aHit->GetPositionVhit(),
		     distanceWithoutSlope,
		     distancewithSlope,
		     fSearchHitDistance);
              printf(" at (%.1f, %.1f)+(%.1f, %.1f)-(%.1f, %.1f)=(%.1f, %.1f) dist=%.1f <?> %f\n",
		     aHit->GetPositionUhit(), aHit->GetPositionVhit(),
		     aHit->GetPlane()->GetPosition()(0), aHit->GetPlane()->GetPosition()(1),
		     (fHitList[0])->GetPlane()->GetPosition()(0), (fHitList[0])->GetPlane()->GetPosition()(1),
		     (aHit->GetPositionUhit())+(aHit->GetPlane()->GetPosition()(0))-((fHitList[0])->GetPlane()->GetPosition()(0))-((fHitList[0])->GetPositionUhit()),
		     (aHit->GetPositionVhit())+(aHit->GetPlane()->GetPosition()(1))-((fHitList[0])->GetPlane()->GetPosition()(1))-((fHitList[0])->GetPositionVhit()),
		     aDistance,
		     fSearchHitDistance);
            }

            if( aDistance < fSearchHitDistance && aDistance < minDistance ) {
              minDistance = aDistance;
              bestHit = aHit;
            }
          } // end loop on plane hits

          // if a hit has been found, add the hit
          if( bestHit ) {
            fHitList[fHits]  = bestHit;
            fHitList[fHits]->SetFound();
            if( fDebugTracker) printf("   DTracker::find_tracks hit %d selected from plane %d (%s) hit #%d found %d\n",
				      fHits,
				      aPlane->GetPlaneNumber(),
				      aPlane->GetPlanePurpose(),
				      fHitList[fHits]->GetNumber(),
				      fHitList[fHits]->GetFound());
            fHits++;
          }
        } // end if valid plane
      } // end loop on planes

      // We have now a list of hits
      // if they are numerous enough, try to make the track
      if( fDebugTracker) printf(" DTracker::find_tracks trying with %d hits over %d required, alignStatus %d\n", fHits, fRequiredHits, fAlignmentStatus);
      if (fHits >= fRequiredHits) {

        // ***************************************************************
        // fTracksN+1 to start track numbering at 1, JB 2009/08/25
        // Increment counters over all events, JB 2009/09/08
        if (fTrack[fTracksN]->Analyze( fTracksN+1, fHitList, fHits, tPlaneResolution)) {
          fTrackCount[0]         += 1;
          fTrackCount[fHits]     += 1;
          fTrackCountPerPlane[0] += 1;
          for ( Int_t iHit=0; iHit<fHits; iHit++) {
            fTrackCountPerPlane[ fHitList[iHit]->GetPlane()->GetPlaneNumber() ] += 1;
          }
          // if subtrack mechanism requested, refit with subset of planes
          // JB 2015/12/15
          if ( fSubTrackPlanesN>0 ) {
            MakeSubTrack( fTrack[fTracksN], fHitList, fHits);
          }
	  // if fTrackFittingAlg == 1, refit the track with MKalmanFilter method
	  // QL 2016/05/26
	  if (fTrackFittingAlg == kKalman){
	    MakeKalTrack(fTrack[fTracksN], fHitList, fHits);
	  }
	  // if fTrackFittingAlg == 2, refit the track with MKalmanFilter method
	  // QL 2016/05/26
	  if (fTrackFittingAlg == kChi2MS){
	    MakeLeastChi2Track(fTrack[fTracksN], fHitList, fHits);
	  }
          fTracksN++;
        }
        else {
	  // if track not selected, free all hits
	  // JB 2011/03/14
          for( Int_t iHit=0; iHit<fHits; iHit++ ) {
            fHitList[iHit]->SetFound(kFALSE);
            if(fDebugTracker) printf("DTracker::find_track set hit[%d] # %d of plane %d (%s) free.\n",
				     iHit,
				     fHitList[iHit]->GetNumber(),
				     fHitList[iHit]->GetPlane()->GetPlaneNumber(),
				     fHitList[iHit]->GetPlane()->GetPlanePurpose());
          }
        }
        // ***************************************************************
      }
      else {
	// VR 2014/06/29
	// If track not selected due to fRequiredHits no reached, free all hits
        for( Int_t iHit=0; iHit<fHits; iHit++ ) {
          fHitList[iHit]->SetFound(kFALSE);
          if(fDebugTracker) printf("DTracker::find_track set hit[%d] # %d of plane %d (%s) free.\n",
				   iHit,
				   fHitList[iHit]->GetNumber(),
				   fHitList[iHit]->GetPlane()->GetPlaneNumber(),
				   fHitList[iHit]->GetPlane()->GetPlanePurpose());
        }
      }
      if( fDebugTracker) printf("     %d tracks found\n", fTracksN);
    } // end loop on first hits
  } // end loop on seed planes

}

//_____________________________________________________________________________
//
void DTracker::find_tracks_1_opt()
{
  // Method to find tracks like find_tracks() but with more options.
  // Method chosen if fTracksFinder==1 (config param TracksFinder==1)
  //
  //  Allows to track through planes in a pre-defined order
  //   if fTrackingPlaneOrderType = 0, use order defined by plane nb
  //   if fTrackingPlaneOrderType = 1, order defined by plane status
  //                                   first Status 0, then 1 and 2
  //
  //  Allows to change the search distance depending on the nb of hits already associated
  //   when only 1 hit, use fSearchMoreHitDistance
  //   when at least 2 hits, use fSearchMoreHitDistance
  //
  //----------------------------------------------------------------
  //
  // Created : VR 2014/07/14, adapted from find_tracks()

  DPlane *aPlane;
  DHit   *aHit;
  DTrack  aTrack( fPlanesN);
  aTrack.SetDebug( fDebugTracker);
  DR3     extrapolation;
  Int_t   oldfHits = 0;
  DPlane *oldPlane;
  Double_t minDistance = 1.e9;
  Double_t aDistance, distanceWithoutSlope, distancewithSlope;
  DHit *bestHit=0;
  Float_t tPlaneResolution=fc->GetTrackerPar().Resolution;
  DR3 hitPosition; // usefull for temporary computation check

  // fRequiredHits is no longer updated here, JB 2009/09/01
  if( fDebugTracker) printf(" DTracker::find_tracks_1_opt %d hits required, alignStatus %d\n", fRequiredHits, fAlignmentStatus);

 std::vector<Int_t> PlanesToAnalyseOrder;
 std::vector<Int_t> PlanesStatus0;
 std::vector<Int_t> PlanesStatus1;
 std::vector<Int_t> PlanesStatus2;
 std::vector<Int_t> PlanesStatus3;
  PlanesToAnalyseOrder.clear();
  PlanesStatus0.clear();
  PlanesStatus1.clear();
  PlanesStatus2.clear();
  PlanesStatus3.clear();

  for( Int_t iPlane=0; iPlane< fPlanesN; iPlane++)
  {
    aPlane = (DPlane*)fPlaneArray->At(iPlane);
    if (aPlane->GetStatus() == 0) PlanesStatus0.push_back(iPlane);
    if (aPlane->GetStatus() == 1) PlanesStatus1.push_back(iPlane);
    if (aPlane->GetStatus() == 2) PlanesStatus2.push_back(iPlane);
    if (aPlane->GetStatus() == 3) PlanesStatus3.push_back(iPlane);
  }

  if (fTrackingPlaneOrderType == 0)
  {
    for( Int_t iPlane=0; iPlane< fPlanesN; iPlane++)
    {
      aPlane = (DPlane*)fPlaneArray->At(iPlane);
      if(aPlane->GetStatus() <= fAlignmentStatus) PlanesToAnalyseOrder.push_back(iPlane);
    }
  }

  if (fTrackingPlaneOrderType == 1)
  {
    if(fAlignmentStatus>=0) for( Int_t iPlane=0; iPlane<int(PlanesStatus0.size()); iPlane++) PlanesToAnalyseOrder.push_back(PlanesStatus0.at(iPlane));
    if(fAlignmentStatus>=1) for( Int_t iPlane=0; iPlane<int(PlanesStatus1.size()); iPlane++) PlanesToAnalyseOrder.push_back(PlanesStatus1.at(iPlane));
    if(fAlignmentStatus>=2) for( Int_t iPlane=0; iPlane<int(PlanesStatus2.size()); iPlane++) PlanesToAnalyseOrder.push_back(PlanesStatus2.at(iPlane));
    if(fAlignmentStatus>=3) for( Int_t iPlane=0; iPlane<int(PlanesStatus3.size()); iPlane++) PlanesToAnalyseOrder.push_back(PlanesStatus3.at(iPlane));
  }
  /*
  if (fTrackingPlaneOrderType == 2)
  {
    PlanesToAnalyseOrder.push_back(4);//4-Left
    PlanesToAnalyseOrder.push_back(3);//3-Left
    PlanesToAnalyseOrder.push_back(2);//2-Left
    PlanesToAnalyseOrder.push_back(1);//1-Left
    PlanesToAnalyseOrder.push_back(8);//4-Right
    PlanesToAnalyseOrder.push_back(7);//3-Right
    PlanesToAnalyseOrder.push_back(6);//2-Right
    PlanesToAnalyseOrder.push_back(5);//1-Right
  }
  */
  if(fDebugTracker)
  {
    printf(" DTracker::find_tracks_1_opt is looking for tracks in %d/%d planes with TrackingPlaneOrderType = %d \n",int(PlanesToAnalyseOrder.size()),fPlanesN,fTrackingPlaneOrderType);
    for( Int_t iPlane=0; iPlane<int(PlanesToAnalyseOrder.size()); iPlane++)
    {
      aPlane = (DPlane*)fPlaneArray->At(PlanesToAnalyseOrder.at(iPlane));
      printf("  %d) plane %d (%s) with status %d \n",iPlane,aPlane->GetPlaneNumber(),aPlane->GetPlanePurpose(),aPlane->GetStatus() );
    }
  }



  // Reset counters and arrays
  fTracksN = 0;
  for(Int_t trk = 1; trk <= fTracksMaximum; trk++)  fTrack[trk-1]->Reset();

  // ==================
  // Loop on reference planes usable for track seed
  DPlane *firstPlane;
  for( Int_t iseed=0; iseed<int(PlanesStatus0.size()); iseed++ ) // loop on seed planes
  {
    firstPlane = GetPlane(PlanesStatus0.at(iseed));

    // ==================
    // Loop on all hits of the track seed plane
    // and try to make a track out of each one
    if(fDebugTracker) printf(" DTracker::find_tracks_1_opt first plane is %d (%s) with %d hits\n", firstPlane->GetPlaneNumber(),firstPlane->GetPlanePurpose(), firstPlane->GetHitsN());

    for( Int_t iFirstHit=1; iFirstHit<=firstPlane->GetHitsN() ; iFirstHit++) // loop on first hits, pay attention that hit numbering start at 1
    {
      if( fTracksN >= fTracksMaximum ) break; // if max track number reach, stop
      fHits    = 0;
      oldfHits = 0;
      oldPlane   = firstPlane;
      for(Int_t hit = 0; hit < fHitsMaximum; hit++)  fHitList[hit]=0;
      if(fDebugTracker>1) cout << "   DTracker::find_tracks_1_opt Hit list reseted" << endl;

      // test if the hit was associated before,
      // this test allows to manage several firstPlanes
      // JB 2012/09/07
      if( firstPlane->GetHit( iFirstHit )->GetFound() ) continue;

      fHitList[fHits] = firstPlane->GetHit( iFirstHit );
      fHitList[fHits]->SetFound(); // not really needed, because never tested in the following
      if( fDebugTracker) printf("  DTracker::find_tracks_1_opt hit %d number %d from plane %d (%s) found %d at (%.1f, %.1f)\n", fHits, fHitList[fHits]->GetNumber(), firstPlane->GetPlaneNumber(),firstPlane->GetPlanePurpose(), fHitList[fHits]->GetFound(), fHitList[fHits]->GetPositionUhit(), fHitList[fHits]->GetPositionVhit());
      fHits++;

      // ==================
      // Loop on all planes to find a matching hit in them
      // exclude the first plane
      // always exclude DUT
      // exclude Variable refenreces if alignement is not done yet
      for( Int_t iPlane=0; iPlane< fPlanesN; iPlane++ )
      { // loop on planes
        aPlane = (DPlane*)fPlaneArray->At(PlanesToAnalyseOrder.at(iPlane));

        Bool_t preTrack = 0; // Does a "pre-track" made with 2 hits exists ?
        // check plane is valid
        if( aPlane->GetPlaneNumber() != firstPlane->GetPlaneNumber()
           //&& !( aPlane->GetStatus()==0) // not mandatory but could be faster, JB 2011/03/14. However, you need to comment this to have all the reference plane in status 0
           && ( aPlane->GetStatus()<=fAlignmentStatus ) // condition changed, JB 2012/04/02
           && !( aPlane->GetStatus()==3 ) )
        { // if valid plane
          if(fDebugTracker>1) printf("  DTracker::find_tracks_1_opt checking plane %d (%s) with %d hits\n", aPlane->GetPlaneNumber(),aPlane->GetPlanePurpose(), aPlane->GetHitsN());

          // find the extrapolation of the current track to this plane:
          //  if the number of hits in the track has changed -> recompute the track,
          //  if the plane has changed -> recompute the intersection.
          // JB 2011/07/25
          if( fHits != oldfHits)
          {
            aTrack.Analyze( -1, fHitList, fHits, tPlaneResolution);
            if( fDebugTracker) printf(" track origin (%.1f, %.1f, %.1f) slope (%.1f, %.1f, %.1f)\n", aTrack.GetLinearFit().GetOrigin()(0), aTrack.GetLinearFit().GetOrigin()(1), aTrack.GetLinearFit().GetOrigin()(2), aTrack.GetLinearFit().GetSlopeZ()(0), aTrack.GetLinearFit().GetSlopeZ()(1), aTrack.GetLinearFit().GetSlopeZ()(2));
            oldfHits = fHits;
          }
          if( oldPlane->GetPlaneNumber() != aPlane->GetPlaneNumber() )
          {
            extrapolation = aPlane->Intersection( &aTrack); //aTrack.Intersection( aPlane);
            if( fDebugTracker) printf(" track extrapolation at plane %d (%s) (z=%.1f) uvw= (%.1f, %.1f, %.1f)\n", aPlane->GetPlaneNumber(), aPlane->GetPlanePurpose(), aPlane->GetPosition()(2), extrapolation(0), extrapolation(1), extrapolation(2));
            oldPlane = aPlane;
          }

          // loop on all hits of this plane and keep the nearest one
          minDistance = 1.e9;
          aDistance=distanceWithoutSlope=distancewithSlope=1.e9;
          bestHit=0;
          for( Int_t iHit=1; iHit <= aPlane->GetHitsN(); iHit++ )
          { // loop on plane hits
            aHit = aPlane->GetHit( iHit);
            if( fDebugTracker>1) printf("   DTracker::find_tracks_1_opt trying for hit %d number %d from plane %d (%s) found %d\n",fHits, aHit->GetNumber(), aPlane->GetPlaneNumber(),aPlane->GetPlanePurpose(), aHit->GetFound());
            if( aHit->GetFound()==kTRUE ) continue; // skip hit already found

            distanceWithoutSlope = (fHitList[0])->Distance( aHit);
            distancewithSlope = aHit->Distance( &extrapolation); // JB 2011/07/25

            //-------- IMPORTANT CHOICE for track extrapolation
            // switch the value of aDistance between distanceWithoutSlope or distancewithSlope
            // SS 2011/10/26 - rollback to the method with no slope.
            // now choice made by new parameter, JB 2013/06/21
            if( fUseSlopeInExtrapolation > 0 ) aDistance = distancewithSlope;
            else                               aDistance = distanceWithoutSlope;
          //-----------------------

            if( fDebugTracker>1)
            {
              hitPosition.SetValue( *(fHitList[0]->GetPosition()));
              hitPosition = fHitList[0]->GetPlane()->PlaneToTracker( hitPosition);
              hitPosition = aPlane->TrackerToPlane( hitPosition);
              printf("#hits=%d (uvw) hit0(%.1f,%.1f)  extra(%.1f,%.1f)   newHit(%.1f,%.1f)   distWOslope=%.1f, distWslope=%.1f (lim %f)\n", fHits, hitPosition(0), hitPosition(1), extrapolation(0), extrapolation(1), aHit->GetPositionUhit(), aHit->GetPositionVhit(), distanceWithoutSlope, distancewithSlope, fSearchHitDistance);
              printf(" at (%.1f, %.1f)+(%.1f, %.1f)-(%.1f, %.1f)=(%.1f, %.1f) dist=%.1f <?> %f\n", aHit->GetPositionUhit(), aHit->GetPositionVhit(), aHit->GetPlane()->GetPosition()(0), aHit->GetPlane()->GetPosition()(1), (fHitList[0])->GetPlane()->GetPosition()(0), (fHitList[0])->GetPlane()->GetPosition()(1), (aHit->GetPositionUhit())+(aHit->GetPlane()->GetPosition()(0))-((fHitList[0])->GetPlane()->GetPosition()(0))-((fHitList[0])->GetPositionUhit()), (aHit->GetPositionVhit())+(aHit->GetPlane()->GetPosition()(1))-((fHitList[0])->GetPlane()->GetPosition()(1))-((fHitList[0])->GetPositionVhit()), aDistance, fSearchHitDistance);
            }

            if (!preTrack && aDistance < fSearchHitDistance) // Trying to associate a 2nd hit ?
            {
              if( aDistance < minDistance )
              {
                minDistance = aDistance;
                bestHit = aHit;
                preTrack = 1;
              }
            }
            else if (preTrack && aDistance < fSearchMoreHitDistance) // Trying to associate more hits than 2 firsts
            {
              if( aDistance < minDistance )
              {
                minDistance = aDistance;
                bestHit = aHit;
              }
            }
          } // end loop on plane hits

          // if a hit has been found, add the hit
          if( bestHit )
          {
            fHitList[fHits]  = bestHit;
            fHitList[fHits]->SetFound();
            if( fDebugTracker) printf("   DTracker::find_tracks_1_opt hit %d selected from plane %d (%s) hit #%d found %d\n", fHits, aPlane->GetPlaneNumber(),aPlane->GetPlanePurpose(), fHitList[fHits]->GetNumber(), fHitList[fHits]->GetFound());
            fHits++;
          }

        } // end if valid plane

      } // end loop on planes

      // We have now a list of hits
      // if they are numerous enough, try to make the track
      if( fDebugTracker) printf(" DTracker::find_tracks_1_opt trying with %d hits over %d required, alignStatus %d\n", fHits, fRequiredHits, fAlignmentStatus);
      if (fHits >= fRequiredHits)
      {
        // ***************************************************************
        // fTracksN+1 to start track numbering at 1, JB 2009/08/25
        // Increment counters over all events, JB 2009/09/08
        if (fTrack[fTracksN]->Analyze( fTracksN+1, fHitList, fHits, tPlaneResolution))
        {
          fTrackCount[0] += 1;
          fTrackCount[fHits] += 1;
          fTrackCountPerPlane[0] += 1;
          for ( Int_t iHit=0; iHit<fHits; iHit++) {
            fTrackCountPerPlane[ fHitList[iHit]->GetPlane()->GetPlaneNumber() ] += 1;
          }
          fTracksN++;
        }
        // if track not selected, free all hits
        // JB 2011/03/14
        else
        {
          for( Int_t iHit=0; iHit<fHits; iHit++ ) {
            fHitList[iHit]->SetFound(kFALSE);
            if(fDebugTracker) printf("DTracker::find_track set hit[%d] # %d of plane %d (%s) free.\n", iHit, fHitList[iHit]->GetNumber(), fHitList[iHit]->GetPlane()->GetPlaneNumber(),fHitList[iHit]->GetPlane()->GetPlanePurpose());
          }
        }
        // ***************************************************************
      }
      // VR 2014/06/29
      // If track not selected due to fRequiredHits no reached, free all hits
      else
      {
        for( Int_t iHit=0; iHit<fHits; iHit++ )
        {
          fHitList[iHit]->SetFound(kFALSE);
          if(fDebugTracker) printf("DTracker::find_track set hit[%d] # %d of plane %d (%s) free.\n", iHit, fHitList[iHit]->GetNumber(), fHitList[iHit]->GetPlane()->GetPlaneNumber(),fHitList[iHit]->GetPlane()->GetPlanePurpose());
        }
      }
      if( fDebugTracker) printf("     %d tracks found\n", fTracksN);
    } // end loop on first hits

  } // end loop on seed planes

}

//_____________________________________________________________________________
//
void DTracker::find_tracks_2_ivi()
{

  // Method to find tracks adapted for Interaction Vertex Imaging
  // Method chosen if fTracksFinder==2 (config param TracksFinder==2)
  //
  // Read the DSetup.cxx comments for a description of the parameters
  //
  //----------------------------------------------------------------
  //
  // Created : VR 2014/07/14, adapted from find_tracks()
  // TODO : add track computation between steps parameter


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 < CONFIGURATION EXEMPLE >
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// // ************************************
// //  	        General
// // ************************************
// TracksFinder:             2
// TracksMaximum:            1000
// KeepUnTrackedHitsBetw2evts: 1
// HitsInPlaneTrackMaximum:  1000
//
// // ************************************
// //      Specific to TracksFinder:2
// // ************************************
// // PreTrack  = config. of algo. to build the start track (=pre-track) only made with hits in pre-track'planes
// // ExtTrack  = config. of algo. to try to extend the pre-track with hits in ext-track'planes
// // FullTrack = config. of algo. for the full track (can be a pre-track not extended !)
// // -----------------------
// //     Tracking pass
// // -----------------------
// // HitsNbMinimum = minimum number of hits to create/extend/save a pre/ext/full track
// // HitsTypeUsed  = 0:any kind ; 1:only new hits ; 2:only memorized hits
// // HitsMaxDist   = distance between hit/track and another hit to add it to the current track [um]
// // [Pre/Ext]TrackPARAM = {valuePass1 valuePass2 ...}
//
// TrackingPass:             2
//
// PreTrackHitsNbMinimum:    {2 2}
// PreTrackHitsTypeUsed:     {0 2}
// PreTrackHitsMaxDist:      {8370 30000}
//
// ExtTrackHitsNbMinimum:    {1 0}
// ExtTrackHitsTypeUsed:     {0 2}
// ExtTrackHitsMaxDist:      {2000 2001}
//
// FullTrackHitsNbMinimum:   {3 2}
// // -----------------------
// // Tracking Planes Order
// // -----------------------
// // TrackingOrderLineX:   [NbPlanesPreTrack]{PTpl1 PTpl2 ...} [NbPlanesExtTrack]{ETpl1 ETpl2 ETpl3 ...}
//
// TrackingOrderLines:       4
// TrackingOrderLine1:       [4]{3 4 7 8} [4]{2 1 6 5}
// TrackingOrderLine2:       [4]{7 8 3 4} [4]{6 5 2 1}
// TrackingOrderLine3:       [4]{2 1 6 5} [4]{3 4 7 8}
// TrackingOrderLine4:       [4]{6 5 2 1} [4]{7 8 3 4}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 </ CONFIGURATION EXEMPLE >
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//--------------------------
//  Declarations + Initialize
//--------------------------
  // Parameters asked for a pass
  Int_t   PreTrackHitsNbMinimum;
  Int_t   PreTrackHitsTypeUsed;
  Float_t PreTrackHitsMaxDist;
  Int_t   ExtTrackHitsNbMinimum;
  Int_t   ExtTrackHitsTypeUsed;
  Float_t ExtTrackHitsMaxDist;
  Int_t   FullTrackHitsNbMinimum;

  // Parameters asked for a tracking line
  Int_t    nbPlanesPreTrack;
  Int_t    nbPlanesExtTrack;

  // Initialize tracks
  fTracksN = 0;
  for(Int_t trk = 1; trk <= fTracksMaximum; trk++)  fTrack[trk-1]->Reset();

  // Values during analysis
  //   Hits
  DHit    *hitFirst;
  DHit    *aHit;
  Int_t    nbHitsFirstPlanePreTrack;
  Int_t    nbHitsCurrentPlanePreTrack;
  Int_t    nbHitsCurrentPlaneExtTrack;
  Int_t    nbPreTrackHitsCurrentTrack;
  Int_t    nbExtTrackHitsCurrentTrack;
  Bool_t   isFromPreviousEvent;
  Bool_t   aCandidateHitInThisPlane;
  Double_t bestDistance;
  Double_t distance;
  //   Track
  DTrack   aTrack(fPlanesN);
  aTrack.SetDebug(fDebugTracker);
  //   Planes
  DR3      intersection;
  DPlane  *planeFirstPreTrack;
  DPlane  *planeCurrentPreTrack;
  DPlane  *planeCurrentExtTrack;

  // General values
  Float_t  tPlaneResolution=fc->GetTrackerPar().Resolution;

  // Display
  TString info1 = "  ";
  TString info2 = "    ";
  TString info3 = "      ";
  TString info4 = "        ";
  TString info5 = "          ";
  TString info6 = "            ";
  TString info7 = "              ";
  TString info8 = "                ";

//--------------------------
//  Work
//--------------------------
  if(fDebugTracker) printf("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-* \n");
  if(fDebugTracker) printf("*-*-* Tracking with DTracker::find_tracks_2_ivi() starts    *-*-* \n");
  if(fDebugTracker) printf("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-* \n\n");

  //**************************
  // for all tracking pass
  //**************************
  for(Int_t iTrackingPass = 0 ; iTrackingPass < fTrackingPass ; iTrackingPass++)
  {
    if(fDebugTracker) cout << info1 << "*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
    if(fDebugTracker) cout << info1 << "*-* Pass # " << iTrackingPass+1 << "/" << fTrackingPass << " starts   *-*" << endl;
    if(fDebugTracker) cout << info1 << "*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;

    PreTrackHitsNbMinimum  = fPreTrackHitsNbMinimum[iTrackingPass];
    PreTrackHitsTypeUsed   = fPreTrackHitsTypeUsed[iTrackingPass];
    PreTrackHitsMaxDist    = fPreTrackHitsMaxDist[iTrackingPass];
    ExtTrackHitsNbMinimum  = fExtTrackHitsNbMinimum[iTrackingPass];
    ExtTrackHitsTypeUsed   = fExtTrackHitsTypeUsed[iTrackingPass];
    ExtTrackHitsMaxDist    = fExtTrackHitsMaxDist[iTrackingPass];
    FullTrackHitsNbMinimum = fFullTrackHitsNbMinimum[iTrackingPass];

    if(fDebugTracker)
    {
      cout << info1 << "-> Parameters for pre-track :" << endl;
      cout << info2 << "-> PreTrackHitsNbMinimum  = " << PreTrackHitsNbMinimum << endl ;
      cout << info2 << "-> PreTrackHitsTypeUsed   = " << PreTrackHitsTypeUsed ;
      switch(PreTrackHitsTypeUsed)
      {
	case 0 :
	  cout << " : all hits can be used" << endl ;
	  break;
	case 1 :
	  cout << " : only new hits can be used" << endl ;
	  break;
	case 2 :
	  cout << " : only memorized hits can be used" << endl ;
	  break;
	default :
	  cout << " : !!! UNKNOWN !!!" << endl ;
	break;
      }
      cout << info2 << "-> PreTrackHitsMaxDist    = " << PreTrackHitsMaxDist   << endl ;

      cout << info1 << "-> Parameters for ext-track :" << endl;
      cout << info2 << "-> ExtTrackHitsNbMinimum  = " << ExtTrackHitsNbMinimum << endl ;
      cout << info2 << "-> ExtTrackHitsTypeUsed   = " << ExtTrackHitsTypeUsed ;
      switch(ExtTrackHitsTypeUsed)
      {
	case 0 :
	  cout << " : all hits can be used" << endl ;
	  break;
	case 1 :
	  cout << " : only new hits can be used" << endl ;
	  break;
	case 2 :
	  cout << " : only memorized hits can be used" << endl ;
	  break;
	default :
	  cout << " : !!! UNKNOWN !!!" << endl ;
	break;
      }
      cout << info2 << "-> ExtTrackHitsMaxDist    = " << ExtTrackHitsMaxDist   << endl ;

      cout << info1 << "-> Parameters for full-track :" << endl;
      cout << info2 << "-> FullTrackHitsNbMinimum = " << FullTrackHitsNbMinimum << endl ;
      cout << endl ;
    }

    //**************************
    // for each tracking line
    //**************************
    for(Int_t iTrackingLine = 0 ; iTrackingLine < fTrackingOrderLines ; iTrackingLine++)
    {
      if(fDebugTracker) cout << info2 << "***------------------------------***" << endl;
      if(fDebugTracker) cout << info2 << "*** Tracking Line # " << iTrackingLine+1 << "/" << fTrackingOrderLines << " starts   ***" << endl;
      if(fDebugTracker) cout << info2 << "***------------------------------***" << endl;

      //---------------------------------
      //  Initialize this tracking line
      //---------------------------------
      nbPlanesPreTrack = fTrackingOrderPreTrack[iTrackingLine][0];
      nbPlanesExtTrack = fTrackingOrderExtTrack[iTrackingLine][0];
      if(fDebugTracker)
      {
	cout << info2 << "-> " << nbPlanesPreTrack << " planes scanned for pre-track : " << endl ;
	for (Int_t iPlanePreTrack=1 ; iPlanePreTrack <= nbPlanesPreTrack ; iPlanePreTrack++)
        {
	  Int_t PlanePreTrackNb = fTrackingOrderPreTrack[iTrackingLine][iPlanePreTrack];
          cout << info3 << "-> " << iPlanePreTrack << ") plane " << PlanePreTrackNb+1 << " : " << ((DPlane*)(fPlaneArray->At(PlanePreTrackNb)))->GetPlanePurpose() << endl ;
        }

	cout << info2 << "-> " << nbPlanesExtTrack << " planes scanned for ext-track : " << endl ;
	for (Int_t iPlaneExtTrack=1 ; iPlaneExtTrack <= nbPlanesExtTrack ; iPlaneExtTrack++)
        {
	  Int_t PlaneExtTrackNb = fTrackingOrderExtTrack[iTrackingLine][iPlaneExtTrack];
          cout << info3 << "-> " << iPlaneExtTrack << ") plane " << PlaneExtTrackNb+1 << " : " << ((DPlane*)(fPlaneArray->At(PlaneExtTrackNb)))->GetPlanePurpose() << endl ;
        }
        cout << endl ;
      }

      //-----------------------------------------
      //  Scanning hits of first pre-track plane
      //-----------------------------------------
      // select the first plane in the pre-track-planes-list (in current tracking line)
      planeFirstPreTrack = (DPlane*)(fPlaneArray->At(fTrackingOrderPreTrack[iTrackingLine][1]));
      nbHitsFirstPlanePreTrack = planeFirstPreTrack->GetHitsN();
      if(fDebugTracker) cout << info2 << "-> First plane for pre-track is # " << fTrackingOrderPreTrack[iTrackingLine][1]+1 ;
      if(fDebugTracker) cout << " : " << planeFirstPreTrack->GetPlanePurpose() << " with " << nbHitsFirstPlanePreTrack << " hits" << endl;

      //-----------------------------------------------------------------
      // loop on these hits, pay attention that hit numbering start at 1 !
      //-----------------------------------------------------------------
      for(Int_t iHitFirstPlane=1 ; iHitFirstPlane <= nbHitsFirstPlanePreTrack ; iHitFirstPlane++)
      {
	//###################################
	// Tests and infos about this hit
	//###################################
	if( fTracksN >= fTracksMaximum )
        {
          cout << info2 << "!!! Tracking stops because max tracks nb reached : " << fTracksN << " tracks !!!" << endl;
          return ; // if max track number reach, stop
        }

        hitFirst = planeFirstPreTrack->GetHit(iHitFirstPlane);
	if(fDebugTracker) cout << info3 << "°°°°°°°°°°°" << endl;
        if(fDebugTracker) cout << info3 << "° Hit " << iHitFirstPlane << "/" << nbHitsFirstPlanePreTrack << " °" << endl;
        if(fDebugTracker) cout << info3 << "°°°°°°°°°°°" << endl;

	if(hitFirst->GetFound())
        {
          if(fDebugTracker) cout << info4 << "-> Is ever associated to a track, skip it" << endl;
          continue;
        }
        else
        {
          if(fDebugTracker) cout << info4 << "-> Is NOT ever associated to a track, keep it" << endl;
        }

        isFromPreviousEvent = hitFirst->GetIsFromPreviousEvent();
        if(isFromPreviousEvent)
	{
	  if(fDebugTracker) cout << info4 << "-> Is memorized from last event" << endl;
	  switch(PreTrackHitsTypeUsed)
	  {
	    case 0 :
	      if(fDebugTracker) cout << info5 << "-> All hits can be used, keep it" << endl;
	      break;
	    case 1 :
	      if(fDebugTracker) cout << info5 << "-> Only new hits can be used, skip it" << endl;
	      continue;
	      break;
	    case 2 :
	      if(fDebugTracker) cout << info5 << "-> Only memorized hits can be used, keep it" << endl;
	      break;
	    default :
	      cout << " There is a hit selection bug in code !!!" << endl;
	    break;
	  }
	}
	else
	{
	  if(fDebugTracker) cout << info4 << "-> Is NOT memorized from last event = is a new hit" << endl;
	  switch(PreTrackHitsTypeUsed)
	  {
	    case 0 :
	      if(fDebugTracker) cout << info5 << "-> All hits can be used, keep it" << endl;
	      break;
	    case 1 :
	      if(fDebugTracker) cout << info5 << "-> Only new hits can be used, keep it" << endl;
	      break;
	    case 2 :
	      if(fDebugTracker) cout << info5 << "-> Only memorized hits can be used, skip it" << endl;
	      continue;
	      break;
	    default :
	      cout << " There is a hit selection bug in code !!!" << endl;
	    break;
	  }
	}

        if( fDebugTracker)
	{
	  cout << info4 ;
	  printf("-> Is located at (u=%.1f, v=%.1f, w=%.1f)um in plane frame, (x=%.1f, y=%.1f, z=%.1f)um in tracker frame\n",\
	  hitFirst->GetPositionUhit(), hitFirst->GetPositionVhit(), hitFirst->GetPositionWhit(),\
	  planeFirstPreTrack->PlaneToTracker(*(hitFirst->GetPosition()))(0),planeFirstPreTrack->PlaneToTracker(*(hitFirst->GetPosition()))(1),planeFirstPreTrack->PlaneToTracker(*(hitFirst->GetPosition()))(2));
	}

	//###################################
	// Prepare a track
	//###################################
        for(Int_t hit = 0; hit < fHitsMaximum; hit++) fHitList[hit]=0; // reset the current-track'hits list

        hitFirst->SetFound(1);
        fHitList[0] = hitFirst;
        fHits = 1 ;

	//###################################
	//  BUILDING A PRE-TRACK
	//###################################
	if(fDebugTracker) cout << info4 << "-> Creating the pre-track # " << fTracksN+1 << endl;
	if(fDebugTracker) cout << info4 << "-----------------"<< endl;
	if(fDebugTracker) cout << info4 << "- Pre-Track # "<< fTracksN+1 << " -" << endl;
	if(fDebugTracker) cout << info4 << "-----------------"<< endl;

	//-----------------------------------------------------------------------------
	// 1) Pre-Track made with 1 hit, perpendicular the current plane (the first one !)
	//-----------------------------------------------------------------------------
	if( fDebugTracker) cout << info4 << "-> Computing the pre-track # " << fTracksN+1 << " (perpendicular to the plane)..." << endl ;
        aTrack.Analyze( -1, fHitList, fHits, tPlaneResolution);
        if( fDebugTracker) cout << info4 ;
	if( fDebugTracker)           printf("-> Pre-track # %d made with %d hit : origin (x=%.1f, y=%.1f, z=%.1f)um and slope (%.1f, %.1f, %.1f) in tracker frame\n",\
	  fTracksN+1, aTrack.GetHitsNumber(),\
	  aTrack.GetLinearFit().GetOrigin()(0), aTrack.GetLinearFit().GetOrigin()(1), aTrack.GetLinearFit().GetOrigin()(2),\
	  aTrack.GetLinearFit().GetSlopeZ()(0), aTrack.GetLinearFit().GetSlopeZ()(1), aTrack.GetLinearFit().GetSlopeZ()(2));

	//------------------------------------------------------------------------------------------------------------------------------------
        //  2) Trying to add hits to this pre-track : for each other pre-track planes, try to add the nearest hit from the preTrack
	//------------------------------------------------------------------------------------------------------------------------------------
	if(fDebugTracker) cout << info4 << "-> Trying to add hits to this pre-track # " << fTracksN+1 << " : scanning " << nbPlanesPreTrack-1 << " others pre-track planes" << endl;
	for(Int_t iOthersPlanesPreTrack=2 ; iOthersPlanesPreTrack <= nbPlanesPreTrack ; iOthersPlanesPreTrack++)
        {
	  planeCurrentPreTrack       = (DPlane*)(fPlaneArray->At(fTrackingOrderPreTrack[iTrackingLine][iOthersPlanesPreTrack]));
          nbHitsCurrentPlanePreTrack = planeCurrentPreTrack->GetHitsN();
          if(fDebugTracker) cout << info5 << "-> Scanning pre-track plane " << (iOthersPlanesPreTrack) << "/" << (nbPlanesPreTrack) << " : #" ;
	  if(fDebugTracker) cout          << fTrackingOrderPreTrack[iTrackingLine][iOthersPlanesPreTrack]+1 << " " << planeCurrentPreTrack->GetPlanePurpose() ;
	  if(fDebugTracker) cout          << " with " << nbHitsCurrentPlanePreTrack << " hits" << endl;

	  aCandidateHitInThisPlane=kFALSE;
          for( Int_t iHit=1; iHit <= nbHitsCurrentPlanePreTrack; iHit++ )
          {
            aHit = planeCurrentPreTrack->GetHit(iHit);
            if(fDebugTracker) cout << info6 << "-> Hit number " << aHit->GetNumber() << "/" << nbHitsCurrentPlanePreTrack << endl ;

            if( aHit->GetFound()==kTRUE )
            {
              if(fDebugTracker) cout << info7 << "-> Is ever associated to a track, skip it." << endl ;
              continue;
            }

	    isFromPreviousEvent = aHit->GetIsFromPreviousEvent();
	    if(isFromPreviousEvent)
	    {
	      if(fDebugTracker) cout << info7 << "-> Is memorized from last event" << endl;
	      switch(PreTrackHitsTypeUsed)
	      {
		case 0 :
		  if(fDebugTracker) cout << info8 << "-> All hits can be used, keep it" << endl;
		  break;
		case 1 :
		  if(fDebugTracker) cout << info8 << "-> Only new hits can be used, skip it" << endl;
		  continue;
		  break;
		case 2 :
		  if(fDebugTracker) cout << info8 << "-> Only memorized hits can be used, keep it" << endl;
		  break;
		default :
		  cout << " There is a hit selection bug in code !!!" << endl;
		break;
	      }
	    }
	    else
	    {
	      if(fDebugTracker) cout << info7 << "-> Is NOT memorized from last event = is a new hit" << endl;
	      switch(PreTrackHitsTypeUsed)
	      {
		case 0 :
		  if(fDebugTracker) cout << info8 << "-> All hits can be used, keep it" << endl;
		  break;
		case 1 :
		  if(fDebugTracker) cout << info8 << "-> Only new hits can be used, keep it" << endl;
		  break;
		case 2 :
		  if(fDebugTracker) cout << info8 << "-> Only memorized hits can be used, skip it" << endl;
		  continue;
		  break;
		default :
		  cout << " There is a hit selection bug in code !!!" << endl;
		break;
	      }
	    }

            distance = (fHitList[0])->Distance( aHit); //TODO Good distance ?
            if (distance>PreTrackHitsMaxDist)
            {
              if(fDebugTracker) printf("%s-> Is at %e.0 um from 1st hit, too far away (> %e.0 um), skip it.\n",\
		info7.Data(), distance, PreTrackHitsMaxDist);
              continue;
            }

	    if(! aCandidateHitInThisPlane)
	    {
	      if(fDebugTracker) printf("%s-> Is the first candidate, at %e.0 um from 1st hit, in search radius (<= %e.0 um)\n",\
		info7.Data(), distance, PreTrackHitsMaxDist);
	      fHitList[fHits] = aHit;
	      aCandidateHitInThisPlane=kTRUE;
	      bestDistance=distance;
	    }
	    else
	    {
	      if(fDebugTracker) printf("%s-> Is a new candidate, at %e.0 um from 1st hit, in search radius (<= %e.0 um), ",\
		info7.Data(), distance, PreTrackHitsMaxDist);
	      if(distance<bestDistance)
	      {
		if(fDebugTracker) printf("nearest that the actual best candidate (%e.0 um < %e.0 um), become now the best candidate\n",distance,bestDistance);
		fHitList[fHits] = aHit;
		bestDistance=distance;
	      }
	      else
	      {
		if(fDebugTracker) printf("farest that the actual best candidate (%e.0 um > %e.0 um), is skipped\n",distance,bestDistance);
	      }
	    }
	  }

	  if(aCandidateHitInThisPlane)
	  {
	    fHitList[fHits]->SetFound();
	    fHits++;
	    if(fDebugTracker) printf("%s-> A hit is considered in this plane, and associated to the pre-track # %d, wich is now made with %d hits (but not re-computed !)\n",\
	      info6.Data(), fTracksN+1, fHits);
	  }
	  else
	  {
	    if(fDebugTracker) printf("%s-> No hit was found in this plane, pre-track # %d is still made with %d hits\n",\
	      info6.Data(), fTracksN+1,fHits);
	  }
	}
	if(fDebugTracker) cout << info5 << "-> All other pre-track planes was scanned " << endl ;

	//------------------------------------------------------------------------------------------------------------------------------------
        //  3) Trying to build the pre-track :
	//------------------------------------------------------------------------------------------------------------------------------------
	if(fDebugTracker) cout << info4 << "-> Trying to build pre-track # " << fTracksN+1 << " with " << fHits << " hits" << endl ;

        if (fHits>=PreTrackHitsNbMinimum)
        {
          if(fDebugTracker) printf("%s-> Enought hits (%d >= %d) to build the pre-track # %d : computing slope ... \n",\
	    info6.Data(),fHits,PreTrackHitsNbMinimum,fTracksN+1);

	  if (fTrack[fTracksN]->Analyze( fTracksN+1, fHitList, fHits, tPlaneResolution))
	  {
	    //This pre-track is deleted -if needed- during the full-track processing
	    if(fDebugTracker) printf("%s-> Pre-track # %d is built with %d hits\n",info6.Data(), fTracksN+1,fHits);
	  }
	  else
	  {
	    if(fDebugTracker) printf("%s-> Pre-track # %d building crashed !!!!!\n",info6.Data(), fTracksN+1);
	    for (Int_t iHit=0; iHit<fHits; iHit++) fHitList[iHit]->SetFound(0);
	    if(fDebugTracker) printf("%s-> The %d hits are set unfounded.\n",info7.Data(), fHits);
	    continue;
	  }
        }
        else
	{
          if(fDebugTracker) printf("%s-> Not enought hits (%d < %d) to build the pre-track # %d \n",\
	    info6.Data(),fHits,PreTrackHitsNbMinimum,fTracksN+1);
	  for (Int_t iHit=0; iHit<fHits; iHit++) fHitList[iHit]->SetFound(0);
	  if(fDebugTracker) printf("%s-> The %d hits are set unfounded.\n",info7.Data(), fHits);
          continue;
	}

	//------------------------------------------------------------------------------------------------------------------------------------
        //  4) Trying to extend the pre-track :
	//------------------------------------------------------------------------------------------------------------------------------------
	nbPreTrackHitsCurrentTrack = fHits ;
	nbExtTrackHitsCurrentTrack = 0 ;

	if(fDebugTracker) cout << info4 << "-> Trying to extend pre-track # " << fTracksN+1 << " with " << nbPreTrackHitsCurrentTrack << " pre-track hits" << endl ;
	if(fDebugTracker) cout << info4 << "-----------------"<< endl;
	if(fDebugTracker) cout << info4 << "- Ext-Track # "<< fTracksN+1 << " -" << endl;
	if(fDebugTracker) cout << info4 << "-----------------"<< endl;

	//--------------------------
	//  4.1) For each ext-track planes, try to add the nearest hit from the preTrack
        //--------------------------
	if(fDebugTracker) cout << info4 << "-> Trying to add hits to pre-track # " << fTracksN+1 << " scanning " << nbPlanesExtTrack << " ext-track planes" << endl ;

	for(Int_t iPlanesExtTrack=1 ; iPlanesExtTrack <= nbPlanesExtTrack ; iPlanesExtTrack++)
        {
	  planeCurrentExtTrack       = (DPlane*)(fPlaneArray->At(fTrackingOrderExtTrack[iTrackingLine][iPlanesExtTrack]));
          nbHitsCurrentPlaneExtTrack = planeCurrentExtTrack->GetHitsN();
	  if(fDebugTracker) cout << info5 << "-> Scanning ext-track plane " << iPlanesExtTrack << "/" << nbPlanesExtTrack << " : #";
	  if(fDebugTracker) cout          << fTrackingOrderExtTrack[iTrackingLine][iPlanesExtTrack]+1 << " " << planeCurrentExtTrack->GetPlanePurpose();
	  if(fDebugTracker) cout          << " with " << nbHitsCurrentPlaneExtTrack<< " hits" << endl;

	  intersection = planeCurrentExtTrack->Intersection(fTrack[fTracksN]);
          if(fDebugTracker)  printf("%s-> Track # %d intersection with this plane at z=%.1f is (u=%.1f, v=%.1f, w=%.1f)um in plane frame, (x=%.1f, y=%.1f, z=%.1f)um in tracker frame\n",\
	    info6.Data(), fTracksN+1,\
	    planeCurrentExtTrack->GetPosition()(2), intersection(0), intersection(1), intersection(2),\
	    planeCurrentExtTrack->PlaneToTracker(intersection)(0),planeCurrentExtTrack->PlaneToTracker(intersection)(1),planeCurrentExtTrack->PlaneToTracker(intersection)(2));

	  aCandidateHitInThisPlane=kFALSE;
          for( Int_t iHit=1; iHit <= nbHitsCurrentPlaneExtTrack; iHit++ )
          {
            aHit = planeCurrentExtTrack->GetHit(iHit);
	    if(fDebugTracker) cout << info6 << "-> Hit number " << aHit->GetNumber() << "/" << nbHitsCurrentPlaneExtTrack << " " ;

            if( aHit->GetFound()==kTRUE )
            {
              if(fDebugTracker) cout << "is ever associated, skip it." << endl ;
              continue;
            }

	    isFromPreviousEvent = aHit->GetIsFromPreviousEvent();
	    if(isFromPreviousEvent)
	    {
	      if(fDebugTracker) cout << info7 << "-> Is memorized from last event" << endl;
	      switch(ExtTrackHitsTypeUsed)
	      {
		case 0 :
		  if(fDebugTracker) cout << info8 << "-> All hits can be used, keep it" << endl;
		  break;
		case 1 :
		  if(fDebugTracker) cout << info8 << "-> Only new hits can be used, skip it" << endl;
		  continue;
		  break;
		case 2 :
		  if(fDebugTracker) cout << info8 << "-> Only memorized hits can be used, keep it" << endl;
		  break;
		default :
		  cout << " There is a hit selection bug in code !!!" << endl;
		break;
	      }
	    }
	    else
	    {
	      if(fDebugTracker) cout << info7 << "-> Is NOT memorized from last event = is a new hit" << endl;
	      switch(ExtTrackHitsTypeUsed)
	      {
		case 0 :
		  if(fDebugTracker) cout << info8 << "-> All hits can be used, keep it" << endl;
		  break;
		case 1 :
		  if(fDebugTracker) cout << info8 << "-> Only new hits can be used, keep it" << endl;
		  break;
		case 2 :
		  if(fDebugTracker) cout << info8 << "-> Only memorized hits can be used, skip it" << endl;
		  continue;
		  break;
		default :
		  cout << " There is a hit selection bug in code !!!" << endl;
		break;
	      }
	    }

            distance = aHit->Distance( &intersection);//TODO Good distance ?
            if (distance>ExtTrackHitsMaxDist)
            {
              if(fDebugTracker) printf("is at %e.0 um from intersection, too far away (> %e.0 um), skip it.\n",distance,ExtTrackHitsMaxDist);
              continue;
            }

	    if(! aCandidateHitInThisPlane)
	    {
	      if(fDebugTracker) printf("is the first candidate, at %e.0 um from intersection, in search radius (<= %e.0 um)\n",distance,ExtTrackHitsMaxDist);
	      fHitList[fHits] = aHit;
	      aCandidateHitInThisPlane=kTRUE;
	      bestDistance=distance;
	    }
	    else
	    {
	      if(fDebugTracker) printf("is a new candidate, at %e.0 um from intersection, in search radius (<= %e.0 um), ",distance,ExtTrackHitsMaxDist);
	      if(distance<bestDistance)
	      {
		if(fDebugTracker) printf("nearest that the actual best candidate (%e.0 um < %e.0 um), become now the best candidate\n",distance,bestDistance);
		fHitList[fHits] = aHit;
		bestDistance=distance;
	      }
	      else
	      {
		if(fDebugTracker) printf("farest that the actual best candidate (%e.0 um > %e.0 um), is skipped\n",distance,bestDistance);
	      }
	    }
	  }

	  if(aCandidateHitInThisPlane)
	  {
	    fHitList[fHits]->SetFound();
	    fHits++;
	    nbExtTrackHitsCurrentTrack++;
	    if(fDebugTracker) printf("%s-> A hit is considered in this plane, and associated to the ext-track # %d, wich is now made with %d hits : %d pre-hit(s) and %d ext-hit(s) (but not re-computed !)\n",\
	      info6.Data(), fTracksN+1, fHits, nbPreTrackHitsCurrentTrack, nbExtTrackHitsCurrentTrack);
	  }
	  else
	  {
	    if(fDebugTracker) printf("%s-> No hit was found in this plane, ext-track # %d is still made with %d hits : %d pre-hit(s) and %d ext-hit(s)\n",\
	      info6.Data(), fTracksN+1,fHits, nbPreTrackHitsCurrentTrack, nbExtTrackHitsCurrentTrack);
	  }
	}
	if(fDebugTracker) cout << info5 << "-> All ext-track planes was scanned " << endl ;

	//--------------------------
	//  4.2) Trying to build a full track
        //--------------------------
	if(fDebugTracker) cout << info4 << "-> Trying to build full-track # " << fTracksN+1 << " with " << fHits << " hits " ;
	if(fDebugTracker) cout          << "( " << nbPreTrackHitsCurrentTrack << " + " << nbExtTrackHitsCurrentTrack << " ) " << endl ;
	if(fDebugTracker) cout << info4 << "------------------"<< endl;
	if(fDebugTracker) cout << info4 << "- Full-Track # "<< fTracksN+1 << " -" << endl;
	if(fDebugTracker) cout << info4 << "------------------"<< endl;

	if (fHits>=FullTrackHitsNbMinimum && nbExtTrackHitsCurrentTrack>=ExtTrackHitsNbMinimum)
        {
          if(fDebugTracker) printf("%s-> Enought ext-hits (%d >= %d) AND total hits (%d >= %d) to build the full-track # %d : computing slope ... \n",\
	    info4.Data(), nbExtTrackHitsCurrentTrack, ExtTrackHitsNbMinimum, fHits,FullTrackHitsNbMinimum,fTracksN+1);

	  if (fTrack[fTracksN]->Analyze( fTracksN+1, fHitList, fHits, tPlaneResolution))
	  {
	    fTracksN++;
	    fTrackCount[0] += 1;
	    fTrackCount[fHits] += 1;
	    fTrackCountPerPlane[0] += 1;
	    for ( Int_t iHit=0; iHit<fHits; iHit++) fTrackCountPerPlane[ fHitList[iHit]->GetPlane()->GetPlaneNumber() ] += 1;
	    if(fDebugTracker) printf("%s-> Full-track # %d is built with %d hits ( %d + %d )\n\n\n",\
	      info5.Data(), fTracksN, fHits, nbPreTrackHitsCurrentTrack, nbExtTrackHitsCurrentTrack);
	  }
	  else
	  {
	    if(fDebugTracker) printf("%s-> Full-track # %d building crashed !!!!!\n",info5.Data(), fTracksN+1);
	    for (Int_t iHit=0; iHit<fHits; iHit++) fHitList[iHit]->SetFound(0);
	    if(fDebugTracker) printf("%s-> The %d hits are set unfounded, track # %d is deleted.\n\n\n",\
	      info6.Data(), fHits, fTracksN+1);
	    fTrack[fTracksN]->Reset();
	    continue;
	  }
        }
        else
	{
	  if (fHits<FullTrackHitsNbMinimum)
	  {
	    if(fDebugTracker) printf("%s-> Not enought total hits (%d < %d) to build the full-track # %d\n",\
	    info5.Data(), fHits,FullTrackHitsNbMinimum, fTracksN+1);
	  }
	  else if (nbExtTrackHitsCurrentTrack<ExtTrackHitsNbMinimum)
	  {
	    if(fDebugTracker) printf("%s-> Not enought ext-hits (%d < %d) to build the ext-track # %d (and so the full-track)\n",\
	    info5.Data(), nbExtTrackHitsCurrentTrack, ExtTrackHitsNbMinimum, fTracksN+1);
	  }
	  else
	  {
	    printf(" !!! BUG !!! see code ... \n");
	  }

	  for (Int_t iHit=0; iHit<fHits; iHit++) fHitList[iHit]->SetFound(0);
	  if(fDebugTracker) printf("%s-> The %d hits are set unfounded, track # %d is deleted.\n\n\n",\
	      info6.Data(), fHits, fTracksN+1);
	  fTrack[fTracksN]->Reset();
          continue;
	}
      }


      if(fDebugTracker) cout << info2 << "***------------------------------***" << endl;
      if(fDebugTracker) cout << info2 << "*** Tracking Line # " << iTrackingLine+1 << "/" << fTrackingOrderLines << " finished ***" << endl;
      if(fDebugTracker) cout << info2 << "***------------------------------***" << endl << endl ;
    }

    if(fDebugTracker) cout << info1 << "*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
    if(fDebugTracker) cout << info1 << "*-* Pass # " << iTrackingPass+1 << "/" << fTrackingPass << " finished *-*" << endl;
    if(fDebugTracker) cout << info1 << "*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl << endl ;

  }

  if(fDebugTracker) printf("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-* \n");
  if(fDebugTracker) printf("*-*-* Tracking with DTracker::find_tracks_2_ivi() finished  *-*-* \n");
  if(fDebugTracker) printf("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-* \n");

}

//_____________________________________________________________________________
//
void DTracker::find_tracks_and_vertex(){

  // Try to make a track with all hits of the "seed" reference planes (status=0).
  // For each of such hits, a track is extrapolated to the other planes and
  //  the nearest hit within a search window is added to the track. If no hit
  //  lies within the search window, the plane is skipped.
  // When all planes have been searched, the track is fitted with all its associated hits.
  //
  // Note that currently the track extrapolation is done with no slope,
  //  that means perpendicularly with respect to the seed plane.
  // The code to update the slope with each added hits exists but yielded
  //  slightly worst results in SPS beam test (120 GeV pions). So this second
  //  method is still not used. You can however choose it by changing the
  /// commented lines at the "IMPORTANT CHOICE" comments in the code.
  //
  // Planes are used in the tracking depending on their status
  //  and the value of the Tracker Align Status. The rule is that the plane
  //  is included if PlaneStatus<=TrackerAlignStatus.
  //
  // This method has been completely reshaped to allow for multi-track event
  //  and best hit selection.
  //
  //
  // JB 2009/05/22
  // Modified: JB 2009/08/25, track numbering
  // Modified: JB 2009/09/01, no more update of fRequiredHits
  // Modified: JB 2011/03/14, use several sensors for track seed
  // Modified: JB 2011/07/25, search the next hit along the temporary track path
  // Modified: SS 2011/10/26, rollback of the calculation of the hit-track distance
  // Modified: JB 2012/04/02, fix in condition to include planes depending on alignment
  // Modified: JB 2012/09/07, new condition on firstHit

/*
  NodeData* data = new NodeData(0,1,1.235); // nodeNumber, hitNumber, Chi2.

  TreeNode* T = new TreeNode();
  T->SetData(data);

  NodeData* data_1 = new NodeData(1,1,2.2365);
  T->AddChildren(1, data_1);  // hitNumber, data :)

  TreeNode* T1_1 = T->GetNode(1);

  NodeData* data_1_1 = new NodeData(1,2,3.215);
  T->AddChildren(2, data_1_1);

  TreeNode* T1_2 = T->GetNode(2);

  std::cout<<" Node Number = "<<T->GetData()->GetNodeNumber() << "  HitNumber = " << T->GetData()->GetHitNumber()
           <<"  Chi2 = "<< T->GetData()->GetChi2() << std::endl;

  std::cout<<" Node Number = "<<T->GetNode(1)->GetData()->GetNodeNumber() << "  HitNumber = " << T->GetNode(1)->GetData()->GetHitNumber()
           <<"  Chi2 = "<< T->GetNode(1)->GetData()->GetChi2() << std::endl;

  std::cout<<" Node Number = "<<T->GetNode(2)->GetData()->GetNodeNumber() << "  HitNumber = " << T->GetNode(2)->GetData()->GetHitNumber()
           <<"  Chi2 = "<< T->GetNode(2)->GetData()->GetChi2() << std::endl;
*/

  DPlane *aPlane;
  DHit   *aHit;
  DTrack  aTrack( fPlanesN);
  aTrack.SetDebug( fDebugTracker);
  DR3     extrapolation;
  Int_t   oldfHits = 0;
  DPlane *oldPlane;
  Double_t minDistance = 1.e9;
  Double_t aDistance, distanceWithoutSlope, distancewithSlope;
  DHit *bestHit=0;
  Float_t tPlaneResolution=fc->GetTrackerPar().Resolution;
  DR3 hitPosition; // usefull for temporary computation check

  // fRequiredHits is no longer updated here, JB 2009/09/01
  //if( fDebugTracker) printf(" DTracker::find_tracks %d hits required, alignStatus %d\n", fRequiredHits, fAlignmentStatus);

  // Reset counters and arrays
  fTracksN = 0;
  for(Int_t trk = 1; trk <= fTracksMaximum; trk++)  fTrack[trk-1]->Reset();

  // ==================
  // Loop on reference planes usable for track seed
  DPlane *firstPlane;
  for( Int_t iseed=0; iseed<fTrackSeedDevs; iseed++ ) { // loop on seed planes

    firstPlane = GetPlane(fListTrackSeedDevs[iseed]);

    // ==================
    // Loop on all hits of the track seed plane
    // and try to make a track out of each one
    //if(fDebugTracker) printf(" DTracker::find_tracks first plane is %d with %d hits\n", firstPlane->GetPlaneNumber(), firstPlane->GetHitsN());

    //std::vector<TreeNode*> vectorTree;

    for( Int_t iFirstHit=1; iFirstHit<=firstPlane->GetHitsN() ; iFirstHit++) { // loop on first hits, pay attention that hit numbering start at 1
      if( fTracksN >= fTracksMaximum ) break; // if max track number reach, stop
      fHits    = 0;
      oldfHits = 0;
      oldPlane   = firstPlane;
      for(Int_t hit = 0; hit < fHitsMaximum; hit++)  fHitList[hit]=0;
      //if(fDebugTracker>1) cout << "   DTracker::find_tracks Hit list reseted" << endl;

      // test if the hit was associated before,
      // this test allows to manage several firstPlanes
      // JB 2012/09/07
      if( firstPlane->GetHit( iFirstHit )->GetFound() ) continue;

      fHitList[fHits] = firstPlane->GetHit( iFirstHit );
      fHitList[fHits]->SetFound(); // not really needed, because never tested in the following
      //if( fDebugTracker) printf("  DTracker::find_tracks hit %d number %d from plane %d found %d at (%.1f, %.1f)\n", fHits, fHitList[fHits]->GetNumber(), firstPlane->GetPlaneNumber(), fHitList[fHits]->GetFound(), fHitList[fHits]->GetPositionUhit(), fHitList[fHits]->GetPositionVhit());

/*
      NodeData* data0 = new NodeData(0,1,0); // nodeNumber, hitNumber, Chi2.
      TreeNode* T0 = new TreeNode();
      T0->SetData(data);
*/

      fHits++;

      // ==================
      // Loop on all planes to find a matching hit in them
      // exclude the first plane
      // always exclude DUT
      // exclude Variable refenreces if alignement is not done yet
      for( Int_t iPlane=0; iPlane< fPlanesN; iPlane++ ) { // loop on planes
        aPlane = (DPlane*)fPlaneArray->At(iPlane);

        // check plane is valid
        if( aPlane->GetPlaneNumber() != firstPlane->GetPlaneNumber()
           && !( aPlane->GetStatus()==0) // not mandatory but could be faster, JB 2011/03/14.
           && ( aPlane->GetStatus()<=fAlignmentStatus ) // condition changed, JB 2012/04/02
           && !( aPlane->GetStatus()==3 ) ) { // if valid plane
          //if(fDebugTracker>1) printf("  DTracker::find_tracks checking plane %d with %d hits\n", aPlane->GetPlaneNumber(), aPlane->GetHitsN());

          // find the extrapolation of the current track to this plane:
          //  if the number of hits in the track has changed -> recompute the track,
          //  if the plane has changed -> recompute the intersection.
          // JB 2011/07/25
          if( fHits != oldfHits) {
            aTrack.Analyze( -1, fHitList, fHits, tPlaneResolution);
            //if( fDebugTracker) printf(" track origin (%.1f, %.1f, %.1f) slope (%.1f, %.1f, %.1f)\n", aTrack.GetLinearFit().GetOrigin()(0), aTrack.GetLinearFit().GetOrigin()(1), aTrack.GetLinearFit().GetOrigin()(2), aTrack.GetLinearFit().GetSlopeZ()(0), aTrack.GetLinearFit().GetSlopeZ()(1), aTrack.GetLinearFit().GetSlopeZ()(2));
            oldfHits = fHits;
          }
          if( oldPlane->GetPlaneNumber() != aPlane->GetPlaneNumber() ) {
            extrapolation = aPlane->Intersection( &aTrack); //aTrack.Intersection( aPlane);
            //if( fDebugTracker) printf(" track extrapolation at plane %d (z=%.1f) uvw= (%.1f, %.1f, %.1f)\n", aPlane->GetPlaneNumber(), aPlane->GetPosition()(2), extrapolation(0), extrapolation(1), extrapolation(2));
            oldPlane = aPlane;
          }

          // loop on all hits of this plane and keep the nearest one
          minDistance = 1.e9;
          aDistance=distanceWithoutSlope=distancewithSlope=1.e9;
          bestHit=0;
          for( Int_t iHit=1; iHit <= aPlane->GetHitsN(); iHit++ ) { // loop on plane hits
            aHit = aPlane->GetHit( iHit);
            if( fDebugTracker>1) printf("   DTracker::find_tracks trying for hit %d number %d from plane %d found %d\n",fHits, aHit->GetNumber(), aPlane->GetPlaneNumber(), aHit->GetFound());
            if( aHit->GetFound()==kTRUE ) continue; // skip hit already found

            distanceWithoutSlope = (fHitList[0])->Distance( aHit);
            distancewithSlope = aHit->Distance( &extrapolation); // JB 2011/07/25

            //-------- IMPORTANT CHOICE for track extrapolation
            // switch the value of aDistance between distanceWithoutSlope or distanceWithSlope
            // SS 2011/10/26 - rollback to the method with no slope.
            aDistance = distancewithSlope;
            //-----------------------

            //if( fDebugTracker>1) {
             // hitPosition.SetValue( *(fHitList[0]->GetPosition()));
             // hitPosition = fHitList[0]->GetPlane()->PlaneToTracker( hitPosition);
             // hitPosition = aPlane->TrackerToPlane( hitPosition);
             // printf("#hits=%d (uvw) hit0(%.1f,%.1f)  extra(%.1f,%.1f)   newHit(%.1f,%.1f)   distWOslope=%.1f, distWslope=%.1f (lim %f)\n", fHits, hitPosition(0), hitPosition(1), extrapolation(0), extrapolation(1), aHit->GetPositionUhit(), aHit->GetPositionVhit(), distanceWithoutSlope, distancewithSlope, fSearchHitDistance);
             // printf(" at (%.1f, %.1f)+(%.1f, %.1f)-(%.1f, %.1f)=(%.1f, %.1f) dist=%.1f <?> %f\n", aHit->GetPositionUhit(), aHit->GetPositionVhit(), aHit->GetPlane()->GetPosition()(0), aHit->GetPlane()->GetPosition()(1), (fHitList[0])->GetPlane()->GetPosition()(0), (fHitList[0])->GetPlane()->GetPosition()(1), (aHit->GetPositionUhit())+(aHit->GetPlane()->GetPosition()(0))-((fHitList[0])->GetPlane()->GetPosition()(0))-((fHitList[0])->GetPositionUhit()), (aHit->GetPositionVhit())+(aHit->GetPlane()->GetPosition()(1))-((fHitList[0])->GetPlane()->GetPosition()(1))-((fHitList[0])->GetPositionVhit()), aDistance, fSearchHitDistance);
            //}

            if( aDistance < fSearchHitDistance && aDistance < minDistance ) {
              minDistance = aDistance;
              bestHit = aHit;
            }
          } // end loop on plane hits

          // if a hit has been found, add the hit
          if( bestHit ) {
            fHitList[fHits]  = bestHit;
            fHitList[fHits]->SetFound();
            //if( fDebugTracker) printf("   DTracker::find_tracks hit %d selected from plane %d hit #%d found %d\n", fHits, aPlane->GetPlaneNumber(), fHitList[fHits]->GetNumber(), fHitList[fHits]->GetFound());
            fHits++;
          }

        } // end if valid plane

      } // end loop on planes

      // We have now a list of hits
      // if they are numerous enough, try to make the track
      //if( fDebugTracker) printf(" DTracker::find_tracks trying with %d hits over %d required, alignStatus %d\n", fHits, fRequiredHits, fAlignmentStatus);
      if (fHits >= fRequiredHits) {
        // ***************************************************************
        // fTracksN+1 to start track numbering at 1, JB 2009/08/25
        // Increment counters over all events, JB 2009/09/08
        if (fTrack[fTracksN]->Analyze( fTracksN+1, fHitList, fHits, tPlaneResolution)) {
          fTrackCount[0] += 1;
          fTrackCount[fHits] += 1;
          fTrackCountPerPlane[0] += 1;
          for ( Int_t iHit=0; iHit<fHits; iHit++) {
            fTrackCountPerPlane[ fHitList[iHit]->GetPlane()->GetPlaneNumber() ] += 1;
          }
          fTracksN++;
        }
        // if track not selected, free all hits
        // JB 2011/03/14
        else {
          for( Int_t iHit=0; iHit<fHits; iHit++ ) {
            fHitList[iHit]->SetFound(kFALSE);
            //if(fDebugTracker) printf("DTracker::find_track set hit[%d] # %d of plane %d free.\n", iHit, fHitList[iHit]->GetNumber(), fHitList[iHit]->GetPlane()->GetPlaneNumber());
          }
        }
        // ***************************************************************
        //if( fDebugTracker) printf("     %d tracks found\n", fTracksN);
      }

    } // end loop on first hits

  } // end loop on seed planes

}


//_____________________________________________________________________________
//
void DTracker::find_tracks_withStrips(){

  // Try to make a track with the most significant hit
  //  of each of the reference strip detectors.
  // Most significant means with the highest pulseheight on the seed pixel.
  // Only one track is built !
  //
  // Planes are used in the tracking depending on their status
  //  and the value of the Tracker Align Status. The rule is that the plane
  //  is included if PlaneStatus<=TrackerAlignStatus.
  //
  //
  // Copied from MAF, JB 2012/08/23
  // Modified: JB 2012/08/27 to add condition on hit presence for each plane

  DPlane *aPlane;
  //DHit   *aHit;
  DTrack  aTrack( fPlanesN);
  aTrack.SetDebug( fDebugTracker);
  Float_t tPlaneResolution=fc->GetTrackerPar().Resolution;

  // fRequiredHits is no longer updated here, JB 2009/09/01
  if( fDebugTracker) printf(" DTracker::find_tracks WITH STRIPS %d hits required, alignStatus %d\n", fRequiredHits, fAlignmentStatus);

  // Reset counters and arrays
  fTracksN = 0;
  for(Int_t trk = 1; trk <= fTracksMaximum; trk++)  fTrack[trk-1]->Reset();
  fHits    = 0;

  // ==================
  // Loop on all planes to find a matching hit in them
  //  exclude DUT
  //  exclude planed not aligned
  for( Int_t iPlane=0; iPlane< fPlanesN; iPlane++ ) { // loop on planes
    aPlane = (DPlane*)fPlaneArray->At(iPlane);

    // check plane is valid
    if(  aPlane->GetStatus()<=fAlignmentStatus
       && aPlane->GetStatus()!=3
       && (fAlignmentStatus>=2 || aPlane->GetHitsN()==1) // use single-hit event when aligning, JB 2012/10/29 --> condition removed JB 2013/08/19
       ) { // if valid plane
      if(fDebugTracker>1) printf("  DTracker::find_tracks checking plane %d with %d hits\n", aPlane->GetPlaneNumber(), aPlane->GetHitsN());
      if( aPlane->GetHitsN() > 0 ) { // if a hit is there, JB 2012/08/27
        fHitList[fHits]  = aPlane->GetPrincipalHit();
        fHitList[fHits]->SetFound();
        if( fDebugTracker) printf("   DTracker::find_tracks hit %d selected from plane %d hit #%d found %d\n", fHits, aPlane->GetPlaneNumber(), fHitList[fHits]->GetNumber(), fHitList[fHits]->GetFound());
        fHits++;
      } // end if a hit is there
    } // end if valid plane

  } // end loop on planes

  // We have now a list of hits
  // if they are numerous enough, try to make the track
  if( fDebugTracker) printf(" DTracker::find_tracks trying with %d hits over %d required, alignStatus %d\n", fHits, fRequiredHits, fAlignmentStatus);
  if (fHits >= fRequiredHits) {
    // ***************************************************************
    // fTracksN+1 to start track numbering at 1, JB 2009/08/25
    // Increment counters over all events, JB 2009/09/08
    if (fTrack[fTracksN]->Analyze( fTracksN+1, fHitList, fHits, tPlaneResolution)) {
      fTrackCount[0] += 1;
      fTrackCount[fHits] += 1;
      fTrackCountPerPlane[0] += 1;
      for ( Int_t iHit=0; iHit<fHits; iHit++) {
        fTrackCountPerPlane[ fHitList[iHit]->GetPlane()->GetPlaneNumber() ] += 1;
      }
      fTracksN++;
    }
    // if track not selected, free all hits
    // JB 2011/03/14
    else {
      for( Int_t iHit=0; iHit<fHits; iHit++ ) {
        fHitList[iHit]->SetFound(kFALSE);
        if(fDebugTracker) printf("DTracker::find_track set hit[%d] # %d of plane %d free.\n", iHit, fHitList[iHit]->GetNumber(), fHitList[iHit]->GetPlane()->GetPlaneNumber());
      }
    }
    // ***************************************************************
    if( fDebugTracker) printf("     %d tracks found\n", fTracksN);
  }

}


//_____________________________________________________________________________
//
void DTracker::SetPlanesForSubTrack(Int_t nPlanes, Int_t *planeIds) {

  // Set the list of plane IDs to be used for fitting subtrack
  // technicaly also book memory for the necessary array
  //
  // JB 2015/08/21

  fSubTrackPlanesN = nPlanes;

  if ( fSubTrackPlanesN>0 ) {
    if ( fSubTrackPlaneIds==NULL ) {
      fSubTrackPlaneIds = new Int_t[fSubTrackPlanesN];
    }
    // reninit anyway the plane IDs, in case they changed
    for (Int_t iPlane=0; iPlane<fSubTrackPlanesN; iPlane++) {
      fSubTrackPlaneIds[iPlane] = planeIds[iPlane];
    }
    if ( fSubTrack==NULL ) {
      fSubTrack         = new DTrack*[fTracksMaximum];
      for (Int_t tr = 0; tr < fTracksMaximum; tr++) {
        fSubTrack[tr] = new DTrack( fSubTrackPlanesN);
      }
    }

    if( fDebugTracker) {
      printf("DTracker::SetPlanesForSubTrack with %d hits:", fSubTrackPlanesN);
      for ( Int_t iPlane=0; iPlane<fSubTrackPlanesN; iPlane++) {
        printf( "%d ", fSubTrackPlaneIds[iPlane]);
      }
      printf(".\n");
    }

  }

  // If nb of planes specified is 0, do nothing

}


//_____________________________________________________________________________
//
void DTracker::MakeSubTrack( DTrack *aTrack, DHit** aHitList, Int_t nHits) {

  // Generate a new track from the existing one,
  //  but with a subset of the hits.
  // Only the hits belonging to the planes requested will be kept.
  //
  // Created JB 2014/12/15

  if( fDebugTracker) printf("DTracker::MakeSubTrack for track %d from %d hits down to %d hits.\n", aTrack->GetNumber(), nHits, fSubTrackPlanesN);

  // First check a subset of planes has been specified
  if( fSubTrackPlanesN<=0 ) {
    printf("WARNING in DTracker, MakeSubTrack called when no (%d) subset of planes indicated -> NOP\n", fSubTrackPlanesN);
    return;
  }

  // Trim the original hit list
  DHit **hitSubset = new DHit*[fSubTrackPlanesN];
  Int_t nHitsSelected = 0;

  for ( Int_t iHit=0; iHit<nHits; iHit++) {
    for ( Int_t iPlane=0; iPlane<fSubTrackPlanesN; iPlane++) {
      if( aHitList[iHit]->GetPlane()->GetPlaneNumber() == fSubTrackPlaneIds[iPlane] ) {
        hitSubset[nHitsSelected] = aHitList[iHit];
        if( fDebugTracker>1 ) printf("    hit %d(in track) or %d(in plane) in specified plane %d -> selected as hit nb %d\n", iHit, hitSubset[nHitsSelected]->GetNumber(), fSubTrackPlaneIds[iPlane], nHitsSelected);
        nHitsSelected++;
      }
    }
  }

  // Finally refit the track
  Float_t tPlaneResolution=fc->GetTrackerPar().Resolution;
  Int_t subtrackNb = aTrack->GetNumber()-1;
  fSubTrack[subtrackNb]->Reset();

  if ( fSubTrack[subtrackNb]->Analyze( aTrack->GetNumber(), hitSubset, nHitsSelected, tPlaneResolution) ) {
    if( fDebugTracker ) printf("    subtrack %d successfully fit with %d hits\n", fSubTrack[subtrackNb]->GetNumber(), nHitsSelected);
  }

  delete[] hitSubset; // not sure

}


//_____________________________________________________________________________
//
void DTracker::MakeKalTrack( DTrack *aTrack, DHit** aHitList, Int_t nHits) {

  // Refit a track with Kalman Filter&Smoother,
  // Limits of this function(not Kalman Algorithm):
  //   ! Requires MC simulation parameters!
  //   ! Only deal with tracks in +z direction
  //   ! Only estimate for one DUT test (propagate the track to DUT)
  //   ! nHits >= 2 and hits are in right order (increasing z)
  //
  // Created QL 2016/05/26

  if( fDebugTracker) printf("DTracker::MakeKalTrack for track %d from %d hits.\n", aTrack->GetNumber(), nHits);

  string particle       = fc->GetTrackerPar().BeamType.Data();
  Double_t momentum       = fc->GetTrackerPar().BeamMomentum;
  //string MediumMaterial = fc->GetTrackerPar().MediumMaterial.Data();
  DGlobalTools aTool;
  Double_t mass = aTool.GetMass(particle);
  Double_t charge = aTool.GetCharge(particle);
  if (nHits < 2||momentum <= 0||mass <= 0||charge<=0){
       printf("DTracker::MakeKalTrack: The results of Kal are not guaranteed! KalmanFilter exit\n");
       return ;
  }

  if ( fHitList[0]->GetPlane()->GetAnalysisMode() < 2 ){ // if not pixels
       printf("DTracker::MakeKalTrack: Only implemented for pixels!\n");
       return ;// Quit
  }

  fDebugTracker = aHitList[0]->GetDebug(); // aHitList is always fHitList!
  if(fDebugTracker) printf("DTracker::MakeKalTrack: track number %d\n", aTrack->GetNumber());

  Double_t energy = TMath::Sqrt(mass*mass + momentum*momentum);

  //Prepare TList of Hits and Planes(Scatter Plane)
  TObjArray aPlaneArray = *fPlaneArray;
  aPlaneArray.Sort();
  TList aHitTList;
  for (Int_t i = 0; i < nHits; i++)
       aHitTList.Add(aHitList[i]);
  aHitTList.Sort();
  if(fDebugTracker){
    aHitTList.Print();
    aPlaneArray.Print();
  }

  Int_t planeStatus, planeReadout;

  DPrecAlign *hitAlignment;
  DR3 aPosition;
  DR3 tLineFitOrigin;  // origin in the tracker system
  DR3 tLineFitSlope;   // slope along z-axis in tracker system
  double resolutionU = fHitList[0]->GetResolutionUhit();
  double resolutionV = fHitList[0]->GetResolutionVhit();
  double sigmaMS = fHitList[0]->GetPlane()->GetSigmaThetaMS();
  Double_t meas[2] = {0,0}; // measurements in x(U) and y(V)
  Double_t measCov[4] = {0,0,0,0}; // measurement covariance

  Double_t thickness = 0;
  Double_t x0Layer = 0;
  //Kalman filter
  MKalmanFilter myKF(mass, energy, charge);
  myKF.setSmootherEnabled(kTRUE);
  Bool_t isSmootherUsed = myKF.isSmootherEnabled();// switch to smoother or not!

  //init
  hitAlignment = fHitList[0]->GetPlane()->GetPrecAlignment();
  aPosition = hitAlignment->TransformHitToTracker( *(fHitList[0]->GetPosition()) );
  DLine* preLineFit = aTrack->GetLinearFit2();
  DR3 aSlopeZ = preLineFit->GetSlopeZ();
  Double_t tKFpar[4] = {aPosition(0), aSlopeZ(0), aPosition(1), aSlopeZ(1)};// kal initializer
  //Double_t tKFpar[4] = {aPosition(0), 0, aPosition(1), 0};// kal initializer
  //Double_t tKFpar[4] = {0, 0, aPosition(1), 0};// kal initializer
  Double_t tKFcov22 = /*8e-7;*/sigmaMS*sigmaMS;
  Double_t tKFcov11 = resolutionU*resolutionU + tKFcov22*pow(aPosition(2),2);//Only for my simulation!
  Double_t tKFcov33 = resolutionV*resolutionV + tKFcov22*pow(aPosition(2),2);//Only for my simulation!
  Double_t tKFcov12 = 0;//(tKFcov22) * aPosition(2);
  tKFcov22 = 8e-7;
  //Double_t tKFcov[16] = {
  //     tKFcov11, sigmaMS*sigmaMS*aPosition(2), 0, 0,
  //     sigmaMS*sigmaMS*aPosition(2),sigmaMS*sigmaMS , 0,0,
  //     0,0,tKFcov33, sigmaMS*sigmaMS*aPosition(2),
  //     0,0,sigmaMS*sigmaMS*aPosition(2),sigmaMS*sigmaMS}; // Kal initializer // 8e-7
  //Double_t tKFcov[16] = {
  //     tKFcov11, 0, 0, 0,
  //     0,sigmaMS*sigmaMS , 0,0,
  //     0,0,tKFcov33, sigmaMS*sigmaMS*aPosition(2),
  //     0,0,sigmaMS*sigmaMS*aPosition(2),sigmaMS*sigmaMS}; // Kal initializer // 8e-7
  Double_t tKFcov[16] = {
       tKFcov11, tKFcov12, 0, 0,
       tKFcov12, tKFcov22, 0, 0,
       0,  0,tKFcov33, tKFcov12,
       0,  0,tKFcov12, tKFcov22}; // Kal initializer // 8e-7
  Double_t zLayer = aPosition(2); // Kal initializer
  Double_t z1stLayer = zLayer;

  // kF init
  myKF.initKalZParCov(zLayer, tKFpar, tKFcov); //

  DHit *aHit = (DHit*)aHitTList.First();
  DHit *rightHit = NULL; // The closest hit in the right side of DUT
  DPlane* aPlane = aHit->GetPlane();
  Int_t FirstPlane = aPlaneArray.IndexOf(aPlane);
  Int_t lastPlane = 0;
  Int_t iDUT = 999; // Have to be a large value!
  for (Int_t pl = 0; pl <aPlaneArray.GetEntriesFast(); pl++){
    // #pl plane properties(tracking planes or planes only for scattering)
    DPlane* planeInArr = (DPlane*)aPlaneArray.At(pl);
    planeStatus = planeInArr->GetStatus();
    Int_t planeNumber = planeInArr->GetPlaneNumber();
    Int_t planeNumber_pre = 1;
    if (pl != 0)
      planeNumber_pre = ((DPlane*)aPlaneArray.At(pl-1))->GetPlaneNumber();
    planeReadout = fc->GetPlanePar(planeNumber).Readout;
    if (fDebugTracker)  cout << "Plane " << planeNumber << " status=" << planeStatus << " readout=" << planeReadout<<endl;
    if(planeReadout>0&& planeStatus==3) iDUT = pl;
    if (pl < FirstPlane)
      continue; // start from the plane containing the 1st hit

    aPlane = aHit->GetPlane();
    Int_t iHitPlane = aPlaneArray.IndexOf(aPlane); // iHitPlane changes as aHit does

    // MS related: material before current layer!
    //cout << "here?" << endl;
    thickness = fc->GetPlanePar(planeNumber_pre).PlaneThickness;
    TString material = fc->GetPlanePar(planeNumber_pre).PlaneMaterial;
    //cout << "here1?" << endl;
    x0Layer = aTool.GetX0(material.Data());
    //cout << "here2?" << endl;
    if (fDebugTracker)  cout << "Plane " << planeNumber << " Material=" << material << " thickness=" << thickness<<endl;

    if (planeReadout>0&&planeStatus<3&&iHitPlane==pl){// a plane for tracking
	if(iHitPlane>iDUT && rightHit ==NULL){
	     rightHit = aHit;// find the closest hit in the right side of DUT
	}
        hitAlignment = aHit->GetPlane()->GetPrecAlignment();
        aPosition = hitAlignment->TransformHitToTracker( *(aHit->GetPosition()) );
        resolutionU = aHit->GetResolutionUhit();
        resolutionV = aHit->GetResolutionVhit();
	// KF propatete
	if(iHitPlane != FirstPlane)
          myKF.propagateTo(aPosition(2), thickness, thickness/x0Layer);

	// KF update
        if(fDebugTracker){
          printf("DTracker::MakeKalTrack: \n");
	  printf("At Z=%f, Propagated ", aPosition(2));
	  myKF.print();
          printf("INFO: plane %i, thickness(%f), x0(%f), pos(%f,%f,%f), resolution(%f, %f)\n", ((DPlane*)aPlaneArray.At(iHitPlane))->GetPlaneNumber(), thickness, x0Layer, aPosition(0), aPosition(1), aPosition(2), resolutionU, resolutionV);

        }
        meas[0] = aPosition(0);
        meas[1] = aPosition(1);
        measCov[0] = resolutionU*resolutionU;
        measCov[3] = resolutionV*resolutionV;
        zLayer = aPosition(2);
        myKF.updateAlt(zLayer, meas, measCov);
        if(fDebugTracker){
          printf("DTracker::MakeKalTrack: \n");
	  printf("At Z=%f, Updated ", zLayer);
	  myKF.print();
	}

	if (aHit == (DHit*)aHitTList.Last()){
	  lastPlane = iHitPlane;
	  break;// the last hit, end the loop
	}
	else
	  aHit = (DHit*)aHitTList.After(aHit);// Go to next hit
    }
    else{ // plane only for scattering
      DPlane* aPlane = (DPlane*)aPlaneArray.At(pl);
      hitAlignment = aPlane->GetPrecAlignment();
      DR3 intersectionPos = aPlane->Intersection(aTrack);
      intersectionPos = hitAlignment->TransformHitToTracker(intersectionPos);
      myKF.propagateTo(intersectionPos(2), thickness, thickness/x0Layer);
      if(fDebugTracker){
        printf("At Z=%f, Propagated ", intersectionPos(2));
        myKF.print();
      }
    }

  }//end plane loop
  if(fDebugTracker){
    printf("DTracker::MakeKalTrack (In Sorted Array) FirstDetLayer:%i, LastDetLayer:%i, DUT:%i\n", FirstPlane, lastPlane, iDUT);
  }

  // Extrapolate to DUT
  if(iDUT > lastPlane){
    for(Int_t iPlane = lastPlane+1; iPlane<=iDUT; iPlane++){
      aPlane = (DPlane*)aPlaneArray.At(iPlane-1);
      Int_t PlN_pre = aPlane->GetPlaneNumber();
      // MS related:
      thickness = fc->GetPlanePar(PlN_pre).PlaneThickness;
      TString material = fc->GetPlanePar(PlN_pre).PlaneMaterial;
      x0Layer = aTool.GetX0(material.Data());

      aPlane = (DPlane*)aPlaneArray.At(iPlane);
      hitAlignment = aPlane->GetPrecAlignment();
      DR3 intersectionPos = aPlane->Intersection(aTrack);
      intersectionPos = hitAlignment->TransformHitToTracker(intersectionPos);
      myKF.propagateTo(intersectionPos(2), thickness, thickness/x0Layer);
    }
  }
  else if(iDUT < FirstPlane){
    if(isSmootherUsed && myKF.smoothTo(z1stLayer)){
      if(fDebugTracker){
	printf("DTracker::MakeKalTrack: Smoothing finished successfully!\n");
	printf("At Z=%f, Smoothed ", z1stLayer);
	myKF.print();
      }
    }
    else{
      printf("DTracker::MakeKalTrack Smoothing failed! Using a backward filter instead!\n");
      //exit(0);
      //myKF.clearSmoother();
      //myKF.setSmootherEnabled(kFALSE);
      myKF.reset(); // reset some states but not change the initialization

      /// Backward filter!
      aHit = (DHit*)aHitTList.Before(aHit);
      for(Int_t iPlane = lastPlane-1; iPlane >= FirstPlane; iPlane--){
        DPlane* aHitPlane = (DPlane*)aHit->GetPlane();// hit plane
        aPlane = (DPlane*)aPlaneArray.At(iPlane); // current plane
        Int_t PlN = aPlane->GetPlaneNumber();
        // MS related:
        thickness = fc->GetPlanePar(PlN).PlaneThickness;
        TString material = fc->GetPlanePar(PlN).PlaneMaterial;
        x0Layer = aTool.GetX0(material.Data());

        hitAlignment = aPlane->GetPrecAlignment();
        if(aPlane != aHitPlane){
          DR3 intersectionPos = aPlane->Intersection(aTrack);
          intersectionPos = hitAlignment->TransformHitToTracker(intersectionPos);
          myKF.propagateBackTo(intersectionPos(2), thickness, thickness/x0Layer);
        }
        else{ // Hit plane
          aPosition = hitAlignment->TransformHitToTracker( *(aHit->GetPosition()));
          meas[0] = aPosition(0);
          meas[1] = aPosition(1);
          resolutionU = aHit->GetResolutionUhit();
          resolutionV = aHit->GetResolutionVhit();
          measCov[0] = resolutionU*resolutionU;
          measCov[1] = resolutionV*resolutionV;
	  zLayer = aPosition(2);
          myKF.propagateBackTo(zLayer, thickness, thickness/x0Layer);
          myKF.updateBack(zLayer, meas, measCov);
          if(fDebugTracker){
            printf("DTracker::MakeKalTrack Backward Filter\n");
	    printf("At Z=%f, Updated ", zLayer);
	    myKF.print();
	  }
          if(aHit == aHitTList.First())
            break;
          else
            aHit = (DHit*)aHitTList.Before(aHit);
        }
      }

    }

    for(Int_t iPlane = FirstPlane-1; iPlane>=iDUT; iPlane--){
      aPlane = (DPlane*)aPlaneArray.At(iPlane);
      Int_t PlN = aPlane->GetPlaneNumber();
      // MS related:
      thickness = fc->GetPlanePar(PlN).PlaneThickness;
      TString material = fc->GetPlanePar(PlN).PlaneMaterial;
      x0Layer = aTool.GetX0(material.Data());

      aPlane = (DPlane*)aPlaneArray.At(iPlane);
      hitAlignment = aPlane->GetPrecAlignment();
      DR3 intersectionPos = aPlane->Intersection(aTrack);
      intersectionPos = hitAlignment->TransformHitToTracker(intersectionPos);
      myKF.propagateBackTo(intersectionPos(2), thickness, thickness/x0Layer);
      if(fDebugTracker){
        printf("At Z=%f, Propagated ", intersectionPos(2));
        myKF.print();
      }
    }

  }
  else{ // iDUT is in the middle of the telescope
    //
    DPlane* rightPlane = rightHit->GetPlane();
    hitAlignment = rightPlane->GetPrecAlignment();
    aPosition = hitAlignment->TransformHitToTracker( *(rightHit->GetPosition()) );
    if(myKF.smoothTo(aPosition(2))){
      if(fDebugTracker){
	printf("DTracker::MakeKalTrack: Smoothing finished successfully!\n");
	printf("At Z=%f, Smoothed ", z1stLayer);
	myKF.print();
      }
    }
    else{
      printf("ERROR: DTracker::MakeKalTrack Smoothing failed!\n");
      exit(0);
    }
    Int_t iRightPlane = aPlaneArray.IndexOf(rightPlane);
    for(Int_t iPlane = iRightPlane-1; iPlane>=iDUT; iPlane--){
      aPlane = (DPlane*)aPlaneArray.At(iPlane);
      Int_t PlN = aPlane->GetPlaneNumber();
      // MS related:
      thickness = fc->GetPlanePar(PlN).PlaneThickness;
      TString material = fc->GetPlanePar(PlN).PlaneMaterial;
      x0Layer = aTool.GetX0(material.Data());

      aPlane = (DPlane*)aPlaneArray.At(iPlane);
      hitAlignment = aPlane->GetPrecAlignment();
      DR3 intersectionPos = aPlane->Intersection(aTrack);
      intersectionPos = hitAlignment->TransformHitToTracker(intersectionPos);
      myKF.propagateBackTo(intersectionPos(2), thickness, thickness/x0Layer);
      if(fDebugTracker){
        printf("At Z=%f, Propagated ", intersectionPos(2));
        myKF.print();
      }
    }

  }

  const Double_t* resultPar = myKF.getPar();
  const Double_t* resultCov = myKF.getCov();

  tLineFitOrigin(0) = /*fittedTrackOrigin(0);*/ resultPar[0];
  tLineFitOrigin(1) = /*fittedTrackOrigin(1);*/ resultPar[2];
  tLineFitOrigin(2) = 0;
  tLineFitSlope(0)  = /*fittedTrackDirections(0);*/ resultPar[1];
  tLineFitSlope(1)  = /*fittedTrackDirections(1);*/ resultPar[3];
  tLineFitSlope(2)  = 1.0;

  Float_t tDx, tDy, tDz;
  DR3 tLineFitDirection;
  tDx = tLineFitSlope(0);
  tDy = tLineFitSlope(1);
  tDz = 1/sqrt(tDx * tDx + tDy * tDy + 1);
  tLineFitDirection = (tLineFitSlope * tDz);

  DLine tFittedLine;
  tFittedLine.SetValue(tLineFitOrigin,tLineFitDirection,tLineFitSlope,0.);
  aTrack->SetLinearFit2(&tFittedLine);

  Double_t sigmX = sqrt(resultCov[0]);
  Double_t sigmY = sqrt(resultCov[10]);
  if(fDebugTracker){
    printf("DTracker::MakeKalTrack: TrackParam(x, slopeX, y, slopeY) = (%f, %f, %f, %f); Track Uncertainty(%f, %f)!\n",resultPar[0], resultPar[1], resultPar[2], resultPar[3],  sigmX, sigmY);
    printf("DTracker::MakeKalTrack: linearChisquareMethod Uncertainty(%f, %f)", aTrack->GetDeltaOrigineX(), aTrack->GetDeltaOrigineY());
  }
  aTrack->SetDeltaOrigineX(sigmX);
  aTrack->SetDeltaOrigineY(sigmY);
}


//_____________________________________________________________________________
//
void DTracker::MakeLeastChi2Track( DTrack *aTrack, DHit** aHitList, Int_t nHits) {

  // Refit a track by using Least Chi-square fitting,
  // Limits of this function(not this Algorithm):
  //   ! Requires MC simulation parameters!
  //   ! Only deal with tracks in +z direction
  //   ! Only estimate for one DUT test (propagate the track to DUT)
  //   ! nHits >= 2 and hits are in right order (increasing z)
  //
  // Created QL 2016/06/08

  if( fDebugTracker) printf("DTracker::MakeLeastChi2Track for track %d from %d hits.\n", aTrack->GetNumber(), nHits);

  string particle       = fc->GetTrackerPar().BeamType.Data();
  Double_t momentum       = fc->GetTrackerPar().BeamMomentum;
  //string MediumMaterial = fc->GetTrackerPar().MediumMaterial.Data();
  DGlobalTools aTool;
  Double_t mass = aTool.GetMass(particle);
  Double_t charge = aTool.GetCharge(particle);
  if (nHits < 2||momentum <= 0||mass <= 0||charge<=0){
       printf("DTracker::MakeLeastChi2Track: The results of Kal are not guaranteed! KalmanFilter exit\n");
       return ;
  }

  if ( fHitList[0]->GetPlane()->GetAnalysisMode() < 2 ){ // if not pixels
       printf("DTracker::MakeLeastChi2Track: Only implemented for pixels!\n");
       return ;// Quit
  }

  fDebugTracker = aHitList[0]->GetDebug(); // aHitList is always fHitList!
  if(fDebugTracker) printf("DTracker::MakeLeastChi2Track: track number %d\n", aTrack->GetNumber());

  //Double_t energy = TMath::Sqrt(mass*mass + momentum*momentum);

  //Prepare TList of Hits and Planes(Scatter Plane)
  TObjArray aPlaneArray = *fPlaneArray;
  aPlaneArray.Sort();
  TList aHitTList;
  for (Int_t i = 0; i < nHits; i++)
       aHitTList.Add(aHitList[i]);
  aHitTList.Sort();
  if(fDebugTracker){
    aHitTList.Print();
    aPlaneArray.Print();
  }

  Int_t planeStatus, planeReadout;

  DPrecAlign *preAlignment;
  DR3 aPosition;
  DR3 tLineFitOrigin;  // origin in the tracker system
  DR3 tLineFitSlope;   // slope along z-axis in tracker system
  double resolutionU;
  double resolutionV;
  double sigmaMS;


  ///Least Chi-Square
  MLeastChiSquare myLChi2X; // Just test it with two line fitting in X/Y.
  MLeastChiSquare myLChi2Y;
  // a temporary implementation
  std::vector<Double_t> *tXMeasArr = myLChi2X.GetPtrMeasArray();
  std::vector<Double_t> *tXSigmaArr = myLChi2X.GetPtrSigmaArray();
  std::vector<Double_t> *tXZArr = myLChi2X.GetPtrZArray();
  std::vector<Double_t> *tXSigmaMSArr = myLChi2X.GetPtrSigmaMSArray();
  std::vector<Double_t> *tXZScatterArr = myLChi2X.GetPtrZScatterArray();

  std::vector<Double_t> *tYMeasArr = myLChi2Y.GetPtrMeasArray();
  std::vector<Double_t> *tYSigmaArr = myLChi2Y.GetPtrSigmaArray();
  std::vector<Double_t> *tYZArr = myLChi2Y.GetPtrZArray();
  std::vector<Double_t> *tYSigmaMSArr = myLChi2Y.GetPtrSigmaMSArray();
  std::vector<Double_t> *tYZScatterArr = myLChi2Y.GetPtrZScatterArray();

  Int_t iDUT = 999;
  DHit* aHit = (DHit*)aHitTList.First();
  for (Int_t pl = 0; pl <aPlaneArray.GetEntriesFast(); pl++){
    // #pl plane properties(tracking planes or planes only for scattering)
    DPlane* planeInArr = (DPlane*)aPlaneArray.At(pl);
    planeStatus = planeInArr->GetStatus();
    Int_t planeNumber = planeInArr->GetPlaneNumber();
    planeReadout = fc->GetPlanePar(planeNumber).Readout;

    if (fDebugTracker)  cout << "Plane " << planeNumber << " status=" << planeStatus << " readout=" << planeReadout<<endl;
    if(planeReadout>0&& planeStatus==3) iDUT = pl;

    DPlane* aPlane = aHit->GetPlane();
    Int_t iHitPlane = aPlaneArray.IndexOf(aPlane); // iHitPlane changes as aHit does

    // MS related: material before current layer!
    sigmaMS = (planeInArr->GetSigmaThetaMS());///sqrt(2); // average in x and y
    Double_t zLayer = planeInArr->GetPositionZ();


    if (planeReadout>0&&planeStatus<3&&iHitPlane==pl){// a plane for tracking

	preAlignment = aHit->GetPlane()->GetPrecAlignment();
        aPosition = preAlignment->TransformHitToTracker( *(aHit->GetPosition()) );
        resolutionU = aHit->GetResolutionUhit();
        resolutionV = aHit->GetResolutionVhit();
        zLayer = aPosition(2);

	tXMeasArr    -> push_back(aPosition(0));
	tXSigmaArr -> push_back(resolutionU);
	tXZArr       -> push_back(zLayer);
	tYMeasArr    -> push_back(aPosition(1));
	tYSigmaArr -> push_back(resolutionV);
	tYZArr       -> push_back(zLayer);
	if (aHit != aHitTList.Last())
	   aHit = (DHit*)aHitTList.After(aHit);// Go to next hit
    }

    tXSigmaMSArr  -> push_back(sigmaMS);
    tXZScatterArr -> push_back(zLayer);
    tYSigmaMSArr  -> push_back(sigmaMS);
    tYZScatterArr -> push_back(zLayer);
  }//end plane loop

  myLChi2X.Estimate();
  myLChi2Y.Estimate();

  DPlane* dutPlane = (DPlane*)aPlaneArray.At(iDUT);
  Double_t zDUT = dutPlane->GetPositionZ();
  Double_t resultPar[4];
  //Double_t resultCov[16];
  //resultPar[0] = myLChi2X.GetSigmaAtZ(zDUT);
  resultPar[0] = myLChi2X.GetPar()[0];
  resultPar[1] = myLChi2X.GetPar()[1];
  resultPar[2] = myLChi2Y.GetPar()[0];
  resultPar[3] = myLChi2Y.GetPar()[1];

  tLineFitOrigin(0) = /*fittedTrackOrigin(0);*/ resultPar[0];
  tLineFitOrigin(1) = /*fittedTrackOrigin(1);*/ resultPar[2];
  tLineFitOrigin(2) = 0;
  tLineFitSlope(0)  = /*fittedTrackDirections(0);*/ resultPar[1];
  tLineFitSlope(1)  = /*fittedTrackDirections(1);*/ resultPar[3];
  tLineFitSlope(2)  = 1.0;

  Float_t tDx, tDy, tDz;
  DR3 tLineFitDirection;
  tDx = tLineFitSlope(0);
  tDy = tLineFitSlope(1);
  tDz = 1/sqrt(tDx * tDx + tDy * tDy + 1);
  tLineFitDirection = (tLineFitSlope * tDz);

  DLine tFittedLine;
  tFittedLine.SetValue(tLineFitOrigin,tLineFitDirection,tLineFitSlope,0.);
  aTrack->SetLinearFit2(&tFittedLine);

  Double_t sigmX = (myLChi2X.GetSigmaAtZ(zDUT));
  Double_t sigmY = (myLChi2Y.GetSigmaAtZ(zDUT));
  if(fDebugTracker){
    cout << "------------ Array print for singleton macro usage ---------\n";
    cout << "enum {kZPos, kTrackingLayer, kThickness, kXOverX0, kSigmaLayer,kNProperty};\n";
    cout << "Double_t Layers[][kNProperty] = {\n";
    //cout << "{      0,   0, 50, 5.34188034188034242e-04, 3.5}, ";
    size_t iTele = 0;
    for(size_t i = 0; i < tXZScatterArr->size(); i++){
      if(i != 0) cout << ",\n";
      Int_t status = 0; // 1 -> used in tracking;
      Double_t res = 999;
      if((*tXZScatterArr)[i] == (*tXZArr)[iTele]){
	   status = 1;
	   res = (*tXSigmaArr)[iTele];
	   iTele++;
      }
      DPlane* planeInArr = (DPlane*)aPlaneArray.At(i);
      Double_t thickness = planeInArr->GetPlaneThickness();
      TString material = planeInArr->GetPlaneMaterial();
      Double_t x0Layer = aTool.GetX0(material.Data());
      Double_t xOverX0  = thickness/x0Layer;
      cout << "     {\t"<<(*tXZScatterArr)[i]<<",\t"<<status<<",\t"<<thickness<<",\t"<<xOverX0<<",\t"<<res<<"}";
    }
    cout <<"\n};\n";

    cout << "-------------------------------------------------------------\n";
    printf("DTracker::MakeLeastChi2Track: TrackParam(x, slopeX, y, slopeY) = (%f, %f, %f, %f); Track Uncertainty(%f, %f)!\n",resultPar[0], resultPar[1], resultPar[2], resultPar[3],  sigmX, sigmY);
    printf("DTracker::MakeLeastChi2Track: linearChisquareMethod Uncertainty(%f, %f)", aTrack->GetDeltaOrigineX(), aTrack->GetDeltaOrigineY());
  }
  aTrack->SetDeltaOrigineX(sigmX);
  aTrack->SetDeltaOrigineY(sigmY);
}


//_____________________________________________________________________________
//
DHit* DTracker::nearest_hit( DTrack *aTrack, Int_t aPlaneNumber, Bool_t &hitAssociated){

  // For a given track,
  // check if the track has an associated hit in the given plane,
  // if so return this hit and set hitAssociated to true,
  // otherwise return the the nearest hit in the given plane
  // and set hitAssociated to false.
  //
  // Created by JB, 2009/07/17
  // Modified JB 2014/08/29, return hit associated to track if existing

  DPlane *aPlane = this->GetPlane( aPlaneNumber);
  DHit *tHit = nullptr; // nullptr instead of 0

  // Check if one the hit of the track belongs to this plane
//  printf("DTracker::nearest_hit track %d, has %d hits to test.\n", aTrack->GetNumber(), aTrack->GetHitsNumber());
  for ( Int_t iHit=0; iHit<aTrack->GetHitsNumber(); iHit++) {
    tHit = aTrack->GetHit( iHit);
//    printf("       trying hit %d from plane %d <?> tested plane %d.\n", iHit, tHit->GetPlane()->GetPlaneNumber(), aPlaneNumber);
    if (tHit->GetPlane()->GetPlaneNumber() == aPlaneNumber) {
//      cout << "      --> plane associated: " << hitAssociated << endl;
      hitAssociated = kTRUE;
      return tHit;
    }
  }

  // Search for nearest hit in the plane
  hitAssociated = kFALSE;
  Double_t minDist(1.e9);
  Double_t distance(0.);
  DHit *tNearest = nullptr; // nullptr instead of 0
  for (Int_t ht = 1; ht <= aPlane->GetHitsN(); ht++) { // loop on hits
    tHit = aPlane->GetHit(ht);
    distance = tHit->Distance( aTrack) ;
    if( distance < minDist) {
      minDist = distance;
      tNearest = tHit;
    }
  } //end loop on hits
  return tNearest;
}


//_____________________________________________________________________________
//
DTrack* DTracker::nearest_track( DHit *aHit){

  // For a given hit of a given plane, return a pointer to the nearest track
  //
  // Created by JB, 2009/07/17
  // Modified JB 2009/08/26 to count track from 1 (not 0)
  // Modified BB 2015/11/18 using do...while instead of for loop to avoid a breaking segmentation value while running DSFProduction

  Double_t minDist(1.e9);
  Double_t distance(0.);
  DTrack *tTrack = nullptr;
  Int_t it = 1;

  DTrack *tNearest = nullptr; // nullptr instead of 0
  do{
    tTrack = this->GetTrack(it);
    distance = aHit->Distance( tTrack);
    if( distance < minDist) {
      minDist = distance;
      tNearest = tTrack;
    }
    it++;
  }while(it <= fTracksN); // end loop on tracks

  return tNearest;
}
//_____________________________________________________________________________
//
void DTracker::PrintStatistics(ostream &stream) {
  // SS 2011/12/14 - Save statistics to the chosen output stream
  // Modified JB 2014/08/29
  // Modified JB 2015/05/27 added plane status

  Int_t status;
  Char_t use[4][20] = { "seed", "primary", "secondary", "DUT"};

  stream<<"************************************"<<endl;
  if( fTracksMaximum >= 1 ) { // if tracking was required
    stream<<" Track reconstructed: "<<fTrackCount[0]<<endl;
    for( Int_t ip=1; ip<=fPlanesN; ip++ ) {
      stream<<"    with "<<ip<<" hits: "<<fTrackCount[ip]<<endl;
    }
    stream<<" Number of tracks reconstructed from plane number: "<<endl<<"   ";
    for( Int_t ip=1; ip<=fPlanesN; ip++ ) {
      stream<<"  pl "<<ip<<": "<<fTrackCountPerPlane[ip];
    }
    stream<<"  Status of plane for tracking: "<< endl<<"   ";
    for( Int_t ip=1; ip<=fPlanesN; ip++ ) {
      status = ((DPlane*)(fPlaneArray->At(ip-1)))->GetStatus();
      stream<<"  pl "<<ip<<": "<<status<<" ("<<use[status]<<")";
    }
    cout << endl;
  } // end if tracking was required

  else {
    stream<<" No tracking required"<<endl;
  }
  stream<<"************************************"<<endl;

}
//_____________________________________________________________________________
//
void FCNAlignVertex(Int_t &n, Double_t *gin, Double_t &f, Double_t *par , Int_t iflag)
{
  // static fcn wrapper for Minuit

  DTracker::Instance()->fcn(n,gin,f,par,iflag);
  //fgInstance->fcn(n,gin,f,par,iflag);

}
//_____________________________________________________________________________
//
void DTracker::fcn(Int_t &npar, Double_t *gin, Double_t &fValue, Double_t *x, Int_t iflag)
{

  DTrack* aTrack;
  fValue = 0;
  Double_t chi2 = 0.0;

  //std::cout<<"fcn function 0"<<endl;

  for (Int_t tr = 0; tr<fTracksN; tr++) {

    //std::cout<<"fTracksN = "<<fTracksN<<endl;

    aTrack =  fTrack[tr];

    Double_t xp = aTrack->GetLinearFit().GetOrigin()(0);
    Double_t yp = aTrack->GetLinearFit().GetOrigin()(1);
    Double_t zp = aTrack->GetLinearFit().GetOrigin()(2);

    Double_t xi = x[0];
    Double_t yi = x[1];
    Double_t zi = x[2];

    Double_t ux = aTrack->GetLinearFit().GetSlopeZ()(0);
    Double_t uy = aTrack->GetLinearFit().GetSlopeZ()(1);
    Double_t uz = aTrack->GetLinearFit().GetSlopeZ()(2);

    Double_t sigmaXi = 3.5;  // Resolution for X axis.
    Double_t sigmaYi = 3.5;  // Resolution for Y axis.
    Double_t sigmaZi = 50.0;  // Resolution for Z axis.

    Double_t sigmaUx = 0.05;
    Double_t sigmaUy = 0.05;
    Double_t sigmaUz = 0.05;

    Double_t di2 = (xi-xp)*(xi-xp) + (yi-yp)*(yi-yp) + (zi-zp)*(zi-zp) - ( ((xi-xp)*ux + (yi-yp)*uy + (zi-zp)*uz) * ((xi-xp)*ux + (yi-yp)*uy + (zi-zp)*uz) );

    Double_t incertXi = ( (1-ux*ux)*(xi-xp) - (yi-yp)*uy*ux - (zi-zp)*uz*ux ) * ( (1-ux*ux)*(xi-xp) - (yi-yp)*uy*ux - (zi-zp)*uz*ux ) * sigmaXi * sigmaXi / (4.0*di2);
    Double_t incertYi = ( (1-uy*uy)*(yi-yp) - (xi-xp)*uy*ux - (zi-zp)*uy*ux ) * ( (1-uy*uy)*(yi-yp) - (xi-xp)*uy*ux - (zi-zp)*uz*uy ) * sigmaYi * sigmaYi / (4.0*di2);
    Double_t incertZi = ( (1-uz*uz)*(zi-zp) - (xi-xp)*uz*ux - (yi-yp)*uy*uz ) * ( (1-uz*uz)*(zi-zp) - (xi-xp)*ux*uz - (yi-yp)*uz*uy ) * sigmaZi * sigmaZi / (4.0*di2);

    Double_t incertUx = ( (xi-xp)*( 2*(xi-xp)*ux + (yi-yp)*uy + (zi-zp)*uz ) ) * ( (xi-xp)*( 2*(xi-xp)*ux + (yi-yp)*uy + (zi-zp)*uz ) ) * sigmaUx * sigmaUx / (4.0*di2);
    Double_t incertUy = ( (yi-yp)*( 2*(yi-yp)*uy + (xi-xp)*ux + (zi-zp)*uz ) ) * ( (yi-yp)*( 2*(yi-yp)*uy + (xi-xp)*ux + (zi-zp)*uz ) ) * sigmaUy * sigmaUy / (4.0*di2);
    Double_t incertUz = ( (zi-zp)*( 2*(zi-zp)*uz + (xi-xp)*ux + (yi-yp)*uy ) ) * ( (zi-zp)*( 2*(zi-zp)*uz + (xi-xp)*ux + (yi-yp)*uy ) ) * sigmaUz * sigmaUz / (4.0*di2);

    if( (incertXi + incertYi + incertZi + incertUx + incertUy + incertUz) != 0) chi2 += di2/( incertXi + incertYi + incertZi + incertUx + incertUy + incertUz );
    else chi2 = di2;
    //std::cout<<"chi2 = "<<chi2<<std::endl;

  }

  fValue = chi2;
  //std::cout<<" chi2 = "<< chi2 <<std::endl;
}
//_____________________________________________________________________________
//
void DTracker::find_vertex()
{

  TMinuit* fit = new TMinuit(3);
  fit->SetPrintLevel(-1);
  fit->SetFCN(FCNAlignVertex);

  Double_t arglist[3];
  Int_t ierflg = 0;
  //fit->mnhelp();

  arglist[0] = 1;
  fit->mnexcm("SET ERR", arglist , 1, ierflg);
  arglist[0] = 2;
  fit->mnexcm("SET STRATEGY ", arglist, 1, ierflg); // JB 2012/09/05

  Double_t vstart[3] = {0.0, 0.0, 0.0};

  static Double_t step[3] = {0.01, 0.01, 0.01};
  fit->mnparm(0, "x", vstart[0], step[0], 0,0,ierflg);
  fit->mnparm(1, "y", vstart[1], step[1], 0,0,ierflg);
  fit->mnparm(2, "z", vstart[2], step[2], 0,0,ierflg);

  // Now ready for minimization step
  arglist[0] = 500;
  arglist[1] = 1.;
  gMinuit->mnexcm("MIGRAD", arglist, 2, ierflg);

  Double_t outPar[3];
  Double_t err[3];

  for (Int_t i=0; i<3; ++i) {
    fit->GetParameter(i,outPar[i],err[i]);

  }

  DTrack* aTrack;

  for (Int_t tr = 0; tr<fTracksN; tr++) {
    aTrack =  fTrack[tr];
    aTrack->SetVertex(outPar[0], outPar[1], outPar[2]);
  }

  if( fDebugTracker) {
    std::cout<<"//--- Vertex coordinate -------------------------//"<<std::endl;
    std::cout<<" x = "<<outPar[0]<<std::endl;
    std::cout<<" y = "<<outPar[1]<<std::endl;
    std::cout<<" z = "<<outPar[2]<<std::endl;
    std::cout<<"//-----------------------------------------------//"<<std::endl;
  }

}
//_____________________________________________________________________________
//
void   DTracker::MCTracksTruthMatching(void)
{
  //AP 2016/08/08: Function to perform the truth matching of the reconstructed tracks

  for(int itrk=0;itrk<fTracksN;itrk++) {
    int MCPartID, NHitsMCPartID;
    MCInfoHolder->DoTrackTruthMatching(fTrack[itrk],MCPartID,NHitsMCPartID);

    fTrack[itrk]->SetMCPartID(MCPartID);
    fTrack[itrk]->SetHitsFromMCPartID(NHitsMCPartID);
  }

  return;

}
//_____________________________________________________________________________
//

void DTracker::find_tracks_Beast(){
std::cout << "******************INSIDE  TRAKS BEAST************************" << '\n';
DPlane *aPlane = NULL;
DPlane *aPlaneHit;
DHit   *aHit   = NULL;
DTrack  aTrack( fPlanesN);
DLadder *aLadder;
aTrack.SetDebug( fDebugTracker);
// DTracker *tTracker     = fSession->GetTracker();
// tTracker->GetLadder
DR3     extrapolation;
Int_t   oldfHits = 0;
DPlane *oldPlane = NULL;
Double_t minDistance = 1.e9;
Double_t aDistance, distanceWithoutSlope, distancewithSlope;
DHit *bestHit=0;
Float_t tPlaneResolution=fc->GetTrackerPar().Resolution;
DR3 *hitPositionPlane; // usefull for temporary computation check
DR3 hitPositionLadder;
int LadderNumber=0;
int PlaneIdx;
DBeaster *aBeaster;
aBeaster=GetBeaster();
for(int ladderId=1;ladderId<3;ladderId++){
  cout <<"****** Ladder " <<ladderId <<"**********"<<endl;
  // aBeaster->ListOfBeastHitsOnM1.clear();
  // aBeaster->ListOfBeastHitsOnM2.clear();
  // aBeaster->ListOfRecoParts_SP.clear();
  aBeaster->Clear_List();
  // std::cout << "Sp Reco Part size Avant = " << aBeaster->ListOfRecoParts_SP.size() <<'\n';

  aBeaster->Fill_Modules_HitList(ladderId);
  int BeastHitOnM1=aBeaster->ListOfBeastHitsOnM1.size();
  int BeastHitOnM2=aBeaster->ListOfBeastHitsOnM2.size();
  int HitSeuil=14;
  std::cout << "HitsOnM1 = " << aBeaster->ListOfBeastHitsOnM1.size()<<'\n';
  std::cout << "HitsOnM2 = " << aBeaster->ListOfBeastHitsOnM2.size()<<'\n';
  if((BeastHitOnM1+BeastHitOnM2)!=0){
    	  aBeaster->Particle_Reconstruction(ladderId);
    	  aBeaster->RecoCategorieClassification(ladderId,HitSeuil);

        // aBeaster->SimuCategorieClassification(ladderId,HitSeuil);
    	  // VertexPosition();
    	  // Angular_DistributionSimuTS();
    	  // Angular_DistributionSimuTSL();
    	  // PurityTest();
    	  // PurityTest_Partner();
    	  //Categorie_Momentum();
        // MomentumCoherence();
        // std::cout << "Sp Reco Part size = " << aBeaster->ListOfRecoParts_SP.size() <<'\n';
  }else{
    if(ladderId==1) aBeaster->ListOfBeastRecoParts_L1.clear();
    if(ladderId==2) aBeaster->ListOfBeastRecoParts_L2.clear();
  }//END IF !=0

}//End loop on ladder
// delete aBeaster;
}
//_____________________________________________________________________________
//
