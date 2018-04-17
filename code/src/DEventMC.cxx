// @(#)maf/dtools:$Name:  $:$Id: DEventMC.cxx,v.2 2016/04/20 17:02:46 sha Exp $
// Author: Alejandro Perez   2016/04/20
// Last Modified, AP 2016/04/20

//////////////////////////////////////////////////////////////////////
// Class Description of DEventMC                                    //
//                                                                  //
// Container of the MC truth information when reading a MC n-tuple  //
// This info can be used for hit and track truth matching           //
//                                                                  //
//////////////////////////////////////////////////////////////////////

#include "DEventMC.h"
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

Int_t   fDebugEventMC=0;   // flag for debugging
//_____________________________________________________________________________
//
DEventMC::DEventMC()
{
  
  ResetLists();
  
  return;
  
}
//______________________________________________________________________________
//  
DEventMC::~DEventMC()
{
  
  Clear();
  
  return;
  
}
//______________________________________________________________________________
//  
void DEventMC::Clear(const Option_t *) 
{

  if(fDebugEventMC) cout << "  DEventMC::Clear clearing" << endl;
  ResetLists();

  return;

}
//_____________________________________________________________________________
//
void DEventMC::ResetLists(void)
{
  
  ListOfSimParticles.clear();
  ListOfSimHits.clear();
  ListOfSimPixels.clear();
  
  ListOfNonSensitiveSimParticles.clear();
  ListOfNonSensitiveSimHits.clear();
  
  return;
  
}
//_____________________________________________________________________________
//
void  DEventMC::DoHitTruthMatching(DHit* aHit, int &MCHitID, int &NpixelsMCHitID)
{
  
  // This function performs a Truth-matching between the reconstructed hit (aHit) and the actual hit (MC-hit)
  // and returns the Id of the hit generating this reconstructed hit.
  // The algorithm
  // - Loops over the pixels of the aHit object
  // - Checks where those pixels come from, i.e. was this pixel from a MC-hit or noise
  // - Then counts how many different MC-hit generated pixels in this reconstructed hit
  // - Returns the ID of the MC-hit with the highest number of pixels in this reconstructed hit (MCHitID).
  // - Returns as well the number of pixels in the reconstructed hit belogning to the MC-hit with ID = MCHitID
  
  
  int NpixelsInCluster = aHit->GetStripsInCluster();
  
  if(NpixelsInCluster == 0) {
    MCHitID        = aHit->GetMCHitID();
    NpixelsMCHitID =  0;
    
    return;
  }
  
  std::vector<int> MCHitIdxList;
  MCHitIdxList.clear();
  for(int ipixInHit=0;ipixInHit < NpixelsInCluster;ipixInHit++) {
    MCHitIdxList.push_back((aHit->GetPlane()->GetPixelFromList(aHit->GetIndexInOriginalList(ipixInHit)))->GetPixelMCHitIdx());
  }
  
  //List of non-repeated 
  std::vector<int> ReducedMCHitIdxList;
  ReducedMCHitIdxList.clear();
  std::vector<int> ReducedMCHitIdxList_Npixels;
  ReducedMCHitIdxList_Npixels.clear();
  //Check how many different hit indexes are in this hit
  for(int i=0;i<int(MCHitIdxList.size());i++) {
    //Check if index  is already included in the list
    bool IsNotAlreadyInList = true;
    for(int iredu=0;iredu<int(ReducedMCHitIdxList.size());iredu++) {
      if(ReducedMCHitIdxList[iredu] == MCHitIdxList[i]) {
	IsNotAlreadyInList = false;
	break;
      }
    }
    
    if(IsNotAlreadyInList) {
      ReducedMCHitIdxList.push_back(MCHitIdxList[i]);
      ReducedMCHitIdxList_Npixels.push_back(0);
    }
  }
  
  //Now counts many many times a given hit index repeats itself
  for(int i=0;i<int(MCHitIdxList.size());i++) {
    for(int iredu=0;iredu<int(ReducedMCHitIdxList.size());iredu++) {
      if(ReducedMCHitIdxList[iredu] == MCHitIdxList[i]) {
	ReducedMCHitIdxList_Npixels[iredu]++;
	break;
      }
    }
  }
  
  //Now orders the list of according to the number of times a given index repeats
  for(int iii=2;iii<=int(ReducedMCHitIdxList.size());iii++) {
    for(int jjj=0;jjj<=int(ReducedMCHitIdxList.size())-iii;jjj++) {
      int N_jjj   = ReducedMCHitIdxList_Npixels[jjj];
      int N_jjjp1 = ReducedMCHitIdxList_Npixels[jjj+1];
      
      if(N_jjj > N_jjjp1) {
	int idx_aux                        = ReducedMCHitIdxList[jjj];
	ReducedMCHitIdxList[jjj]           = ReducedMCHitIdxList[jjj+1];
	ReducedMCHitIdxList[jjj+1]         = idx_aux;
	
	int N_aux                          = ReducedMCHitIdxList_Npixels[jjj];
	ReducedMCHitIdxList_Npixels[jjj]   = ReducedMCHitIdxList_Npixels[jjj+1];
	ReducedMCHitIdxList_Npixels[jjj+1] = N_aux;
      }
    }
  }
  
  if(NpixelsInCluster == 2 && ReducedMCHitIdxList.size() > 1) {
    // If reconstructed hit has only 2 pixels then take as the truth-matched 
    // one that which is higher than -1 (i.e. coming from a real particle)
    for(int iredu=0;iredu<int(ReducedMCHitIdxList.size());iredu++) {
      if(ReducedMCHitIdxList[iredu] != -1) {
	MCHitID        = ReducedMCHitIdxList[iredu];
	NpixelsMCHitID = ReducedMCHitIdxList_Npixels[iredu];
	break;
      }
    }
  }
  else {
    // If reconstructed hit is not exacty 2 pixels size, then take the 
    // MC-hit ID with the largest number of pixels in this cluster
    MCHitID        = ReducedMCHitIdxList[0];
    NpixelsMCHitID = ReducedMCHitIdxList_Npixels[0];
  }
  
  MCHitIdxList.clear();
  ReducedMCHitIdxList.clear();
  ReducedMCHitIdxList_Npixels.clear();
  
  return;
  
}
//_____________________________________________________________________________
//
void  DEventMC::DoTrackTruthMatching(DTrack* aTrack, int &MCPartID, int &NHitsMCPartID)
{
 
  // This function performs a Truth-matching between the reconstructed track (aTrack) and the actual track (MC-track)
  // and returns the Id of the particle generating this reconstructed track.
  // The algorithm
  // - Loops over the reconstruced hits of the aTrack object
  // - Checks where those hits come from, i.e. was this hit from a MC-particle or noise
  // - Then counts how many different MC-partiles generated hits in this reconstructed track
  // - Returns the ID of the MC-particle with the highest number of hits in this reconstructed track (MCPartID).
  // - Returns as well the number of hits in the reconstructed track belogning to the MC-particle with ID = MCPartID
  
  
  std::vector<int> MCPartIdxList;
  MCPartIdxList.clear();
  for(int ihitInTrk=0;ihitInTrk < aTrack->GetHitsNumber();ihitInTrk++) {
    int MCHitID = aTrack->GetHit(ihitInTrk)->GetMCHitID();
    if(MCHitID < 0) MCPartIdxList.push_back(-1);
    MCPartIdxList.push_back(GetASimHit(MCHitID).ParticleIdx);
  }

  //List of non-repeated 
  std::vector<int> ReducedMCPartIdxList;
  ReducedMCPartIdxList.clear();
  std::vector<int> ReducedMCPartIdxList_NHits;
  ReducedMCPartIdxList_NHits.clear();
  //Check how many different MC-particle indexes are in this track
  for(int i=0;i<int(MCPartIdxList.size());i++) {
    //Check if index  is already included in the list
    bool IsNotAlreadyInList = true;
    for(int iredu=0;iredu<int(ReducedMCPartIdxList.size());iredu++) {
      if(ReducedMCPartIdxList[iredu] == MCPartIdxList[i]) {
	IsNotAlreadyInList = false;
	break;
      }
    }
    
    if(IsNotAlreadyInList) {
      ReducedMCPartIdxList.push_back(MCPartIdxList[i]);
      ReducedMCPartIdxList_NHits.push_back(0);
    }
  }

  //Now counts many many times a given MC-particle index repeats itself
  for(int i=0;i<int(MCPartIdxList.size());i++) {
    for(int iredu=0;iredu<int(ReducedMCPartIdxList.size());iredu++) {
      if(ReducedMCPartIdxList[iredu] == MCPartIdxList[i]) {
	ReducedMCPartIdxList_NHits[iredu]++;
	break;
      }
    }
  }

  //Now orders the list of according to the number of times a given index repeats
  for(int iii=2;iii<=int(ReducedMCPartIdxList.size());iii++) {
    for(int jjj=0;jjj<=int(ReducedMCPartIdxList.size())-iii;jjj++) {
      int N_jjj   = ReducedMCPartIdxList_NHits[jjj];
      int N_jjjp1 = ReducedMCPartIdxList_NHits[jjj+1];
      
      if(N_jjj > N_jjjp1) {
	int idx_aux                       = ReducedMCPartIdxList[jjj];
	ReducedMCPartIdxList[jjj]         = ReducedMCPartIdxList[jjj+1];
	ReducedMCPartIdxList[jjj+1]       = idx_aux;
	
	int N_aux                         = ReducedMCPartIdxList_NHits[jjj];
	ReducedMCPartIdxList_NHits[jjj]   = ReducedMCPartIdxList_NHits[jjj+1];
	ReducedMCPartIdxList_NHits[jjj+1] = N_aux;
      }
    }
  }

  if(aTrack->GetHitsNumber() == 2 && ReducedMCPartIdxList.size() > 1) {
    // If reconstructed track has only 2 hits then take as the truth-matched 
    // one that which is higher than -1 (i.e. coming from a real particle)
    for(int iredu=0;iredu<int(ReducedMCPartIdxList.size());iredu++) {
      if(ReducedMCPartIdxList[iredu] != -1) {
	MCPartID      = ReducedMCPartIdxList[iredu];
	NHitsMCPartID = ReducedMCPartIdxList_NHits[iredu];
	break;
      }
    }
  }
  else {
    // If reconstructed tracks is has not exacty 2 hits, then take the 
    // MC-particle ID with the largest number of hits in this track
    MCPartID      = ReducedMCPartIdxList[0];
    NHitsMCPartID = ReducedMCPartIdxList_NHits[0];
  }

  MCPartIdxList.clear();
  ReducedMCPartIdxList.clear();
  ReducedMCPartIdxList_NHits.clear();
  
  return;
  
}
//_____________________________________________________________________________
//
