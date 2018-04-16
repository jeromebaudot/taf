// @(#)maf/dtools:$Name:  $:$Id: DEvent.cxx,v.2 2005/10/02 18:03:46 sha Exp $
// Author: Dirk Meier   98/02/18
// Last Modified, JB 2009/05/12
// Last Modified, JB 2009/06/26
// Last Modified, JB 2009/09/08
// Last Modified, JB 2010/07/07 to managed list of triggers,...
// Last Modified, SS 2011/08/10 to limit #frames written in TTree
// Last Modified, JB 2013/11/08 DAuthenticHit
// Last Modified, JB 2014/08/29 DTransparentPlane
// Last Modified, JB 2014/12/23 DTransparentPlane

////////////////////////////////////////////////////////////////
// Class Description of DEvent                                //
//                                                            //
// Trigger event contains values from detector planes         //
// This event is later written to a data summary ".root" file //
//                                                            //
////////////////////////////////////////////////////////////////


//*-- Modified : IG  
//*-- Copyright:  RD42
//*-- Author   :  Dirk Meier   98/02/18
//*KEEP,CopyRight.
/************************************************************************
 * Copyright(c) 1998, RD42@CERN, DiamondTracking.
 ***********************************************************************/

#include "DEvent.h"
#include "DSetup.h"
#include "DPlane.h"
#include "DHit.h"
#include "DTracker.h"
#include "DTrack.h"
#include "DR3.h"
#include "DStrip.h"
#include "DLine.h"
#include "DCut.h"
#include "DAcq.h"
#include "DGlobalTools.h"


  ClassImp(DEvent)             
  ClassImp(DEventHeader)
  ClassImp(DAuthenticPlane)
  ClassImp(DAuthenticHit)
  ClassImp(DTransparentPlane)

  Int_t   fDebugEvent=0;   // flag for debugging

//_____________________________________________________________________________
//  
  void DEventHeader::Set(const Int_t aNEvent, 
			 const Int_t aNRun, 
			 const Int_t aDate, 
			 const Int_t aTime, 
			 const Int_t aType,
			 vector<int> *aListOfTriggers,
			 vector<int> *aListOfFrames,
			 vector<int> *aListOfTimestamps) 
{

  // Array sizes are limited, but it has no influence on the hits or tracks srored
  //  the information on the list of #frames, #triggers used in this event is simply lost
  //  beyond MaxNumberOf (defined in DEvent.h)
  
  // Modified JB 2010/07/07 to add list of triggers,...
  // Modified SS 2011/08/10 to limit lists to array size


  Ek          = aNEvent;
  Erunk       = aNRun;
  Edate       = aDate; // YYMMDD
  Etime       = aTime; // HHMMSS

  // calculate time in seconds relative to start of Month (fMtime)
  Int_t   tYears;   // YY
  Int_t   tMonths;  // MM
  Int_t   tDays;    // DD
  Int_t   tHours;   // HH
  Int_t   tMinutes; // MM
  Int_t   tSeconds; // SS
  tYears       = int( Edate/10000 );
  tMonths      = int( (Edate - 10000 * tYears) / 100 );
  tDays        = int( (Edate - 10000 * tYears) - 100 * tMonths );
  tHours       = int( Etime/10000 );
  tMinutes     = int( (Etime - 10000 * tHours) / 100 );
  tSeconds     = int( (Etime - 10000 * tHours) - 100 * tMinutes );
  Erelative   = tSeconds + 60 * tMinutes + 3600 * tHours + 86400 * tDays;
  Etype       = aType;

  ENumberOfTriggers = (aListOfTriggers!=NULL)?aListOfTriggers->size():0;
  // Mechanism to limit #frames to 50, SS 2011/08/10
  if (ENumberOfTriggers>MaxNumberOf){
    printf("DEvent:Too many (%d) triggers in the event %d, set back to %d\n",ENumberOfTriggers,aNEvent,MaxNumberOf);
    ENumberOfTriggers=MaxNumberOf;
  }  
  for( Int_t index=0; index<ENumberOfTriggers && index<MaxNumberOf; index++) {
    EListOfTriggers[index] = aListOfTriggers->at(index);
  }
  ENumberOfFrames = (aListOfFrames!=NULL)?aListOfFrames->size():0;
  // Mechanism to limit #frames to 50, SS 2011/08/10
  if (ENumberOfFrames>MaxNumberOf){
//    printf("DEvent:Too many (%d) frames in the event %d, set back to %d\n",ENumberOfFrames,aNEvent,MaxNumberOf);
    ENumberOfFrames=MaxNumberOf;
  }  
  for( Int_t index=0; index<ENumberOfFrames; index++) {
    EListOfFrames[index] = aListOfFrames->at(index);
  }
  ENumberOfTimestamps = (aListOfTimestamps!=NULL)?aListOfTimestamps->size():0;
  // Mechanism to limit #frames to 50, SS 2011/08/10
  if (ENumberOfTimestamps>MaxNumberOf){
//    printf("DEvent:Too many (%d) timestamps in the event %d, set back to %d\n",ENumberOfTimestamps,aNEvent,MaxNumberOf);
    ENumberOfTimestamps=MaxNumberOf;
  }  
  for( Int_t index=0; index<ENumberOfTimestamps && index<50; index++) {
    EListOfTimestamps[index] = aListOfTimestamps->at(index);
  }

}

//_____________________________________________________________________________
//  
void DEventHeader::Clear(const Option_t *){

  //if(fDebugEvent) printf("  DEventHeader::Clear clearing\n");
    
}

//_____________________________________________________________________________
//  
void DEventHeader::SetTimeSinceLastEvent(const Int_t aDeltaTime){
  EtimeSinceLastEvent = aDeltaTime;
}

//_____________________________________________________________________________
//  
void DEventHeader::Print()
{

  // Modified JB 2010/07/07 to add list of triggers,...
  // Modified JB 2011/06/30 correction of printouts

  printf("-------------------- EventHeader ---------------------------------\n");
  printf(" RunNumber %d, EventNumber %d\n",Erunk, Ek);
  printf(" Date %d, Time %d \n",Edate,Etime);
  printf("# triggers: %d", ENumberOfTriggers);
  for( Int_t iTrig=0; iTrig<ENumberOfTriggers; iTrig++) {
    printf(": %d ", EListOfTriggers[iTrig]);
  }
  printf(".\n");
  printf("# frames: %d", ENumberOfFrames);
  for( Int_t iTrig=0; iTrig<ENumberOfFrames; iTrig++) {
    printf(": %d ", EListOfFrames[iTrig]);
  }
  printf(".\n");
  printf("# timestamps: %d", ENumberOfTimestamps);
  for( Int_t iTrig=0; iTrig<ENumberOfTimestamps; iTrig++) {
    printf(": %d ", EListOfTimestamps[iTrig]);
  }
  printf(".\n");

  printf("------------------------------------------------------------------\n");
}

//_____________________________________________________________________________
//  
DEvent::DEvent()
{ 
  fT1PlanesN = 0;
  fT1Planes = new TClonesArray("DTransparentPlane", 100);

  fAPlanesN = 0;
  fAPlanes   = new TClonesArray("DAuthenticPlane", 100);

  fAHitsN = 0;
  fAHits = new TClonesArray("DAuthenticHit", 1000);
  
}

//______________________________________________________________________________
//


DEvent::DEvent(DSetup& fc)
{ 

  //Int_t      tAPlanesN=0;
  //Int_t      tAHitsN=0;
  //Int_t      tT1PlanesN=0;
  //tT1PlanesN = fc.GetTrackerPar().Planes;
  //tAPlanesN  = fc.GetTrackerPar().Planes;
  //tAHitsN    = fc.GetTrackerPar().HitsInPlaneMaximum * tAPlanesN;
  //tT1PlanesN = 0;
  fT1Planes = new TClonesArray("DTransparentPlane",100);

  fAPlanesN = 0;
  fAPlanes   = new TClonesArray("DAuthenticPlane",100);

  fAHitsN = 0;
  fAHits = new TClonesArray("DAuthenticHit",1000);

  fFrameNb = -1;
  fLineNb = -1;
}

//______________________________________________________________________________
//  

DEvent::~DEvent()
{ 
  Clear();
}

//______________________________________________________________________________
//  
void DEvent::Clear(const Option_t *) 
{

  if(fDebugEvent) printf("  DEvent::Clear clearing\n");

  fHeader.Clear();
  fAHits->Clear();
  fAPlanes->Clear();
  fT1Planes->Clear();
  fAHitsN     = 0;
  fAPlanesN   = 0;
  fT1PlanesN  = 0;
  fTDC        = 0.;

}

//______________________________________________________________________________
//  
void DEvent::AddAuthenticPlane(DPlane& aPlane, Int_t aNEvent)
{
  TClonesArray &tData = *fAPlanes;
  new(tData[fAPlanesN++]) DAuthenticPlane(aPlane, aNEvent);
}

//______________________________________________________________________________
//  
void DEvent::AddAuthenticHit(DHit& aHit, Int_t aNEvent, DTrack &aTrack)
{
  TClonesArray &tData = *fAHits;
  new(tData[fAHitsN++]) DAuthenticHit(aHit, aNEvent, aTrack);
}


//______________________________________________________________________________
//  
void DEvent::AddTransparentPlane(DPlane& aPlane, DTrack& aTrack, DHit& aHit, Bool_t hitAssociated, DTracker &aTracker)
{

  TClonesArray &tData = *fT1Planes;
  
  new(tData[fT1PlanesN++]) DTransparentPlane(aPlane, aTrack, aHit, hitAssociated, aTracker);

}

//______________________________________________________________________________
//  
void DEvent::SetHeader(const Int_t aNEvent, 
		       const Int_t aNRun, 
		       const Int_t aDate, 
		       const Int_t aTime, 
		       const Int_t aType,
		      std::vector<int> *aListOfTriggers,
		      std::vector<int> *aListOfFrames,
		      std::vector<int> *aListOfTimestamps) {

  // Modified JB 2010/07/07 to add list of triggers,...

  fHeader.Set(aNEvent, aNRun, aDate, aTime, aType, 
	      aListOfTriggers, aListOfFrames, aListOfTimestamps);
  fEvt = aNEvent;
}

//_____________________________________________________________________________
//
void DEvent::SetTimeInterval(const Int_t aDeltaTime)
{
  fHeader.SetTimeSinceLastEvent(aDeltaTime);
}

//______________________________________________________________________________
//  
void DEvent::SetFrame(const Int_t aFrameNb, const Int_t aLineNb) 
{

  fFrameNb = aFrameNb;
  fLineNb  = aLineNb;
}

//______________________________________________________________________________
//  
DAuthenticHit::DAuthenticHit(DHit& aHit, Int_t aNEvent, DTrack& aTrack) : TObject() 
{
  // Note that all positions here are in the Plane coord. system
  // Modified to get rid of call to DStrip class
  // JB, 2009/05/12
  // JB, 2009/06/26
  // JB, 2009/09/08
  // JB, 2013/11/08 Better management of seed strip/pixel information

  //DStrip        *tSeed; // JB, 2009/05/12
  Int_t          tNeighboursN;
  DPlane        *tPlane;
  DR3            tTrackPos;

  //cout << "DAuthenticHit adding a hit" << endl;
//  if(fDebugEvent) printf("  DAuthenticHit::DAuthenticHit adding hit %d (over %d)\n", aHit.GetNumber(), aHit.GetPlane()->GetHitsN());
  if(fDebugEvent) printf("  DAuthenticHit::DAuthenticHit adding hit %d (over %d) with seed %d\n", aHit.GetNumber(), aHit.GetPlane()->GetHitsN(), aHit.GetIndexSeed());

  Hevt             = aNEvent;
  Hpk              = aHit.GetPlane()->GetPlaneNumber();

  //tSeed            = aHit.GetSeed(); // JB, 2009/05/12
  Hhk              = aHit.GetNumber();
  HhN              = aHit.GetPlane()->GetHitsN();
//  cout << "Basic counts" << endl;

  if(aHit.GetStripsInCluster() > 0) {
    Hsu              = aHit.GetSeedU();
    Hsv              = aHit.GetSeedV(); // added by JB, 2007 June
  }
  else {
    Hsu              = aHit.GetPositionUhit();
    Hsv              = aHit.GetPositionVhit();
  }
  Hu               = aHit.GetPositionUhit();
  Hv               = aHit.GetPositionVhit();

  Hresu            = aHit.GetResolutionUhit();
  Hresv            = aHit.GetResolutionVhit();

  HTS              = aHit.GetTimestamp();

  if(aHit.GetStripsInCluster() > 0) { 
    HuCG             = aHit.GetPositionUhitCG();
    HvCG             = aHit.GetPositionVhitCG(); // added by JB, 2007 June
    HuEta            = aHit.GetPositionUhitEta();
    HvEta            = aHit.GetPositionVhitEta(); // added by JB, 2007 November
  }
  else {
    HuCG             = Hu;
    HvCG             = Hv;
    HuEta            = Hu;
    HvEta            = Hv;
  }
//  cout << "Positions" << endl;

  tPlane           = aHit.GetPlane();

  HtN              = tPlane->GetTracker()->GetTracksN();
//  cout << "# tracks" << HtN << endl;

  if( /*&aTrack*/ HtN>0 ) { // Check that tracking was actually done, condition changed JB 2017/02/04
    Htk              = aTrack.GetNumber(); // JB 2009/06/26
    HtHn             = aTrack.GetHitsNumber(); // JB 2009/09/08
    tTrackPos        = aTrack.Intersection(tPlane);   
    Htu              = tTrackPos(0);
    Htv              = tTrackPos(1);
    HtChi2           = aTrack.GetChiSquare();
    HtChi2u          = aTrack.GetChiSquareU();
    HtChi2v          = aTrack.GetChiSquareV();
  }
//  cout << "Track" << endl;

  if(aHit.GetStripsInCluster() > 0) { 
    Hqc              = aHit.GetClusterPulseSum();
    HsN              = aHit.GetSNseed(); // changed to SNRseed, JB 2013/11/08
    HSNc             = aHit.GetClusterSignalToNoise();
    //Hsn              = tSeed->GetNoise(); // JB, 2009/05/12
    //Hsn              = aHit.GetNoise(0);     //YV, 15/10/09
    Hsn              = aHit.GetSeedNoise();     //JB, 2013/11/08

    //HsnPulse              = tSeed->GetPulseHeight(); // JB, 2009/05/12
    //HsnPulse              = aHit.GetPulseHeight(0); // JB, 2009/05/12
    HsnPulse              = aHit.GetSeedPulseHeight(); // JB, 2013/11/08
    //HsnIndex              = tSeed->GetStripIndex(); // JB, 2009/05/12
    //HsnIndex              = aHit.GetIndex(0); // JB, 2009/05/12
    HsnIndex              = aHit.GetIndexSeed(); // JB 2013/11/08
  
    Hnc              = aHit.GetClusterNoiseAverage(); 
    if(Hsn!=0.0){
     HSNc1            = Hqc/Hsn;
    }
    //Hsk              = tSeed->GetStripIndex(); // JB, 2009/05/12
    Hsk              = aHit.GetIndex(0); // JB, 2009/05/12
  }
  else {
    Hqc              = 0.0;
    HsN              = 0.0;
    HSNc             = 0.0;
    Hsn              = 1.0;

    HsnPulse         = 0.0;
    
    double col,row;
    tPlane->ComputeStripPosition_UVToColRow(Hu,Hv,col,row);
    int index = int(col) + tPlane->GetStripsNu()*int(row);
    
    HsnIndex         = index;
  
    Hnc              = aHit.GetClusterNoiseAverage(); 
    if(Hsn!=0.0){
     HSNc1           = Hqc/Hsn;
    }
    Hsk              = index;
  }
//  cout << "Cluster" << endl;
  
  //tNeighboursN     = tSeed->GetNeighbourCount(); // JB, 2009/05/12
  tNeighboursN     = aHit.GetStripsInCluster(); // JB, 2009/05/12
  HNNS = tNeighboursN ; // same as HsN
    

  for(Int_t i = 0; i<100; i++)
    { // change stop condition with max neighbours possible, JB 2009/05/12
      HqM[i]=0 ;
      HkM[i]=0 ;
      HnM[i]=0;
    }

  for(Int_t i=0; i<HNNS; i++) {
    //HqM[i] = aHit.GetMinor(i)->GetPulseHeight();  // JB, 2009/05/12
    //HkM[i] = aHit.GetMinor(i)->GetStripIndex();
    //HnM[i] = aHit.GetMinor(i)->GetNoise();
    HqM[i] = aHit.GetPulseHeight(i);  // JB, 2009/05/12
    HkM[i] = aHit.GetIndex(i);
    HnM[i] = aHit.GetNoise(i);        //YV 21/10/09
  }

  //Hq0 = aHit.GetMinor(0)->GetPulseHeight(); 
  //Hk0 = aHit.GetMinor(0)->GetStripIndex(); 
  //Hn0 = aHit.GetMinor(0)->GetNoise();

  Hq0 = HqM[0];
  Hk0 = HkM[0];
  Hn0 = HnM[0];
  Hq1 = HqM[1];
  Hk1 = HkM[1];
  Hn1 = HnM[1];
  Hq2 = HqM[2];
  Hk2 = HkM[2];
  Hn2 = HnM[2];
  Hq3 = HqM[3];
  Hk3 = HkM[3];
  Hn3 = HnM[3];
  Hq4 = HqM[4];
  Hk4 = HkM[4];
  Hn4 = HnM[4];
  Hq5 = HqM[5];
  Hk5 = HkM[5];
  Hn5 = HnM[5];
  Hq6 = HqM[6];
  Hk6 = HkM[6];
  Hn6 = HnM[6];
  Hq7 = HqM[7];
  Hk7 = HkM[7];
  Hn7 = HnM[7];
  Hq8 = HqM[8];
  Hk8 = HkM[8];
  Hn8 = HnM[8];

  HqL   = aHit.GetPulseHeightLeft();
  HqR   = aHit.GetPulseHeightRight();
  HqLoS = aHit.GetPulseHeightLeftOfSeed();
  HqRoS = aHit.GetPulseHeightRightOfSeed();
  HkL   = aHit.GetIndexLeft();
  HuL   = aHit.GetPositionULeft();

  HSNneighbour = aHit.GetSNneighbour();

  //cout << " Hit created with: Hqc=" << Hqc << ", HsN=" << HsN << ", Hsn=" << Hsn << ", HsnPulse=" << HsnPulse << ", Hq1=" << Hq1 << "." << endl;
  
}

//______________________________________________________________________________
//  

DAuthenticPlane::DAuthenticPlane(DPlane& aPlane, Int_t aNEvent) : TObject() 
{

  // Modified for readout conditional variables, JB 2011/06/17

  Pevt        = aNEvent;
  Ppk         = aPlane.GetPlaneNumber();

  if(fDebugEvent) printf("  DAuthenticPlane::DAuthenticPlane adding plane %d with readout %d\n", Ppk, aPlane.GetReadout());

  PhN         = aPlane.GetHitsN();
  PtN         = aPlane.GetTracker()->GetTracksN(); // JB 2009/06/26

  Pt          = aPlane.GetThreshold();
  PotN        = aPlane.GetOverThresholdN();
  PotQ        = aPlane.GetOverThresholdC();

  PCDSvar  =aPlane.GetCDSvariance();

  if( aPlane.GetReadout() < 100 ) { // for readouts without 0-supression only, JB 2011/06/17
    Pcom1       = aPlane.GetCommonModeRegion(1);
    if(Ppk > 1){ // condition changed, JB 2011/06/17
      Pcom2       = aPlane.GetCommonModeRegion(2);
      Pcom3       = aPlane.GetCommonModeRegion(3);
      Pcom4       = aPlane.GetCommonModeRegion(4);
      Pcom5       = aPlane.GetCommonModeRegion(5);
      Pcom6       = aPlane.GetCommonModeRegion(6);
    }
    Int_t Npixels = aPlane.GetStripsN();
    PFq= 0.0;
    PFn= 0.0;
    PFp= 0.0;
    PFr= 0.0;
    PFrfr1= 0.0;
    PFrfr2= 0.0;

    for(Int_t aPix=0; aPix < Npixels; aPix++){
      PFq+= aPlane.GetPulseHeight(aPix);  //CDS - pedestal - common mode.
      PFn+= aPlane.GetNoise(aPix); //Noise
      PFp+= aPlane.GetPedestal(aPix); //GetCommonMode(aPix); //GetPedestal(aPix); //pedestal
      PFr+= aPlane.GetRawValue(aPix); // CDS ?
      PFrfr1+= aPlane.GetRawFrame1Value(aPix); // GetRawFrame1Value(aPix); // raw 1
      PFrfr2+= aPlane.GetRawFrame2Value(aPix); // GetRawFrame2Value(aPix); // raw 2
    }
    if(Npixels!=0){
      PFq= PFq/float(Npixels);
      PFn= PFn/float(Npixels);
      PFp= PFp/float(Npixels);
      PFr= PFr/float(Npixels);
      PFrfr1=PFrfr1/float(Npixels);
      PFrfr2=PFrfr2/float(Npixels);
    }

    // get a random strip
    Int_t mcStrip = (Int_t)(gRandom->Uniform(1.)*Npixels);
    if( mcStrip == Npixels ) mcStrip--;
    DStrip *aStrip = (DStrip*)aPlane.GetStrip(mcStrip);
    Psk = aStrip->GetStripIndex();
    Psq = aStrip->GetPulseHeight();
    Pnq = aStrip->GetNoise();
    Ppq = aStrip->GetPedestal();

  } // end for readouts without 0-supression only

}

//______________________________________________________________________________
//  
DTransparentPlane::DTransparentPlane(DPlane& aPlane, DTrack& aTrack, DTracker &aTracker) : TObject() 
{

  DTransparentPlane( aPlane, aTrack, *(aPlane.GetPrincipalHit()), kFALSE, aTracker);

}
//______________________________________________________________________________
//  
DTransparentPlane::DTransparentPlane(DPlane& aPlane, DTrack& aTrack, DHit& aHit, Bool_t hitAssociated, DTracker &aTracker) : TObject() 
{

  // Basic storage of a track associated with a hit.
  // With this method, the track is not systematically associated to the so called "Principal" hit.
  //
  // JB, September 2007
  // Modified to get rid of call to DStrip class, JB 2009/05/12 and 2009/06/26
  // Modified to store # hits in the track, JB 2009/09/08
  // Modified for readout conditional variables, JB 2011/06/17
  // Modified to indicate if hit is associated to track or not, JB 2014/08/29
  // Modified to replace track by subtrack if any, JB 2014/12/15

  TString        tPurpose = aPlane.GetPlanePurpose() ;

  Tpk = aPlane.GetPlaneNumber();
  TtN = aPlane.GetTracker()->GetTracksN(); // JB 2009/06/26
  if(fDebugEvent) printf("  DTransparentPlane::DTransparentPlane adding track %d, with plane %d\n", aTrack.GetNumber(), aPlane.GetPlaneNumber());

  // track information
  Ttk      = aTrack.GetNumber();
  TtHn     = aTrack.GetHitsNumber(); // JB 2009/09/08
  // Consider the subtrack rather than the full track if it exists
  DR3 trackPos;
  DTrack *aSubTrack;
  if( aTracker.GetNPlanesForSubTrack()>1 ) {
    aSubTrack = (DTrack*)(aTracker.GetSubTrack( Ttk));
    trackPos = aSubTrack->Intersection(&aPlane); // subtrack position in device frame (uvw)  
//    DR3 fullTrackPos = aTrack.Intersection(&aPlane);
//    printf(" Subtrack %d pos (%.2f, %.2f) versus full track %d pos (%.2f, %.2f)\n", aSubTrack->GetNumber(), trackPos(0), trackPos(1), aTrack.GetNumber(), fullTrackPos(0), fullTrackPos(1));
  }
  else {
    trackPos = aTrack.Intersection(&aPlane); // track position in device frame (uvw)
  }
  Tu     = trackPos(0);
  Tv     = trackPos(1);
  TDOX   = aTrack.GetDeltaOrigineX() ;
  TDOY   = aTrack.GetDeltaOrigineY() ;
  Tx     = aTrack.GetLinearFit().GetOrigin()(0);
  Ty     = aTrack.GetLinearFit().GetOrigin()(1);
  Tz     = aTrack.GetLinearFit().GetOrigin()(2);
  Tdx    = aTrack.GetLinearFit().GetSlopeZ()(0);
  Tdy    = aTrack.GetLinearFit().GetSlopeZ()(1);
  Tchi2  = aTrack.GetChiSquare();
  Tchi2u = aTrack.GetChiSquareU();
  Tchi2v = aTrack.GetChiSquareV();
  TvertexU         = aTrack.GetVertexX();    // LC 201212/17
  TvertexV         = aTrack.GetVertexY();
  TvertexW         = aTrack.GetVertexZ();

  if(fDebugEvent) printf("  DTransparentPlane::DTransparentPlane   pos u,v=(%.1f, %.1f), x,y=(%.1f, %.1f), slope=(%.3f, %.3f), chi2=%f\n", Tu, Tv, Tx, Ty, Tdx, Tdy, Tchi2);

  DR3 tSlopeInPlane;
  tSlopeInPlane = aPlane.TrackerToPlane( aTrack.GetLinearFit().GetSlopeZ() );
  Tdu    = tSlopeInPlane(0);
  Tdv    = tSlopeInPlane(1);

  //RDM250909 begin initialization

  Tud              = 9999.;
  Tvd              = 9999.;
  ThN              = 0;
  Thk              = 0;
  TsN              = 0;
  Tqc              = 0;
  Tq0              = 0;
  //RDM250909 end initialization


  // Compare position with nearest hit reconstructed
  // Hit number is set <0 if hit was used to fit track
  if( &aHit ) { // JB 2009/06/26
    if(fDebugEvent) printf("  DTransparentPlane::DTransparentPlane   hit pos u,v=(%.1f, %.1f), number %d from %d, with %d strips, charge = %f, qseed = %f\n", aHit.GetPositionUhit(), aHit.GetPositionVhit(), aHit.GetNumber(), aPlane.GetHitsN(), aHit.GetStripsInCluster(), aHit.GetClusterPulseSum(), aHit.GetPulseHeight(0));
    Tud              = aHit.GetPositionUhit() - Tu;
    Tvd              = aHit.GetPositionVhit() - Tv;
    ThN              = aPlane.GetHitsN();
    Thk              = (hitAssociated?-1:1)*aHit.GetNumber();
    TsN              = aHit.GetStripsInCluster();
    //This condition corresponds to some MC generated hits where they are generated with a position but with not pixel/strips associated.
    // in this case they have a cluster size of 0 pixels/strips
    if(aHit.GetStripsInCluster() == 0) Tqc = 0;
    else                               Tqc = aHit.GetClusterPulseSum();
    //Tq0              = aHit.GetSeed()->GetPulseHeight(); // added, JB 2007 Sept
    //Tn0              = aHit.GetSeed()->GetNoise(); // added, JB 2007 Sept
    if(aHit.GetStripsInCluster() == 0) Tq0 = 0;
    else                               Tq0 = aHit.GetPulseHeight(0); // modified, JB 2009/05/12
    //Tn0              = aHit.GetNoise(0); // modified, JB 2009/05/12
  }


  // Investigate signal on pixels neighbouring the track impact
  // only available for non sparsified data!
  if( aPlane.GetReadout() < 100 ) { // for readouts without 0-supression only

    DStrip        *tNearest;
    DStrip        *tNextNeighbour;
    DR3            trackPos;

    //DStrip        *nl;
    //DStrip        *nr;
    //DStrip        *n1;
    //DStrip        *n2;
    //DStrip        *no;
    Int_t          tNeighboursN;
    Int_t          tIndex[10];
    Float_t        tDist[10];
    Float_t        tPos[10];
    Float_t        tPulseHeight[10];

    tNearest         = aPlane.NearestStrip(trackPos);  // bef980830 (trackPosOff);
    tNeighboursN = tNearest->GetNeighbourCount();  
    Int_t st;
    for (st = 0; st < (tNeighboursN < 10 ? tNeighboursN : 10); st++){
      tNextNeighbour    =  tNearest->GetNeighbour(st);
      tPos[st]          =  tNextNeighbour->DistanceU(trackPos);
      tDist[st]         =  fabs(tPos[st]);
      tPulseHeight[st]  =  tNextNeighbour->GetPulseHeight();
      tIndex[st]        =  st;
    }

    // now order the index set (st)  so that tDist is in ascending order, leaves tDist unchanged

    Int_t tN = (tNeighboursN-1) < 9 ? (tNeighboursN-1) : 9;

    DGlobalTools aTool; // JB 2011/07/18
    aTool.OrderIndexAsc(&tIndex[0], &tDist[0], tN); // find tIndex order so that tDist is ascending
 
    Tq1 = tPulseHeight[tIndex[0]];
    if (tN > 1) Tq2 = tPulseHeight[tIndex[1]]; else Tq2 = 0.;
    if (tN > 2) Tq3 = tPulseHeight[tIndex[2]]; else Tq3 = 0.;
    if (tN > 3) Tq4 = tPulseHeight[tIndex[3]]; else Tq4 = 0.;
    if (tN > 4) Tq5 = tPulseHeight[tIndex[4]]; else Tq5 = 0.;
    if (tN > 5) Tq6 = tPulseHeight[tIndex[5]]; else Tq6 = 0.;
    if (tN > 6) Tq7 = tPulseHeight[tIndex[6]]; else Tq7 = 0.;
    if (tN > 7) Tq8 = tPulseHeight[tIndex[7]]; else Tq8 = 0.;
    if (tN > 8) Tq9 = tPulseHeight[tIndex[8]]; else Tq9 = 0.;

    aTool.OrderIndexAsc(&tIndex[0], &tPos[0], tN); // find tIndex order so that tPos is ascending

    if( tNeighboursN>=3 ) { // test added, JB 2012/08/20
      //n1 = tNearest->GetNeighbour(1);
      //n2 = tNearest->GetNeighbour(2);
      
      //if (fabs(n1->DistanceU(trackPos)) < fabs(n2->DistanceU(trackPos)) )
      //  no = n1;
      //else 
      //  no = n2;
      
      //if (no->GetPosition()(0) < tNearest->GetPosition()(0)){
      //  nl = no;
      //  nr = tNearest;
      //}
      //else {
      //  nl = tNearest;
      //  nr = no;
      //}
    }
    
    Tu1           = tNearest->GetPosition()(0);
    Tk1           = tNearest->GetStripIndex();

  } // end for readouts without 0-supression only
  else {
    Tu1 = 0;
    Tk1 = 0;
  }

}

