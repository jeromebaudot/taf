// @(#)maf/dtools:$Name:  $:$Id: DHit.cxx,v.1 2005/10/02 18:03:46 sha Exp $
// Author   :  ?
// Last Modified: JB, 2008/10/15
// Last Modified: JB, 2009/05/10
// Last Modified: JB, 2009/05/22
// Last Modified: JB, 2009/06/26
// Last Modified, RDM 2009/08/05
// Last Modified: JB, 2009/08/21
// Last Modified: JB, 2009/08/23
// Last Modified: JB+RDM 2009/08/27 in Distance
// last Modified: JB, 2009/08/30 in Analyze
// last Modified: JB, 2009/09/01 in Analyze
// last Modified: JB, 2010/10/05 in Analyze
// last Modified: JB, 2010/10/15 in Analyze
// Last Modified: JB, 2010/11/12 in Analyze, introduce #pixels with SN>2
// Last Modified: JB, 2010/11/25 Use new DPrecAlign mechanism from DPlane
// Last Modified: JB, 2011/07/21 in Analyse, seed pixel management for large clusters
// Last Modified: JB, 2011/07/21 new method distance to a point
// Last Modified: JB, 2012/05/11 cluster max size extended to 500
// Last Modified: JB, 2012/08/18 update of Analyse with DStrip objects
// Last Modified, JB+SS 2013/06/23 update of Analyse with DPixel objects
// Last Modified: BH 2013/08/20, warning corrections from clang
// Last Modified: JB, 2013/09/29 Analyse(*DPixel)
// Last Modified: JB, 2013/11/08 Analyse(*DPixel) and Analyse(*DStrip)
// Last Modified: VR, 2014/07/16 Add field ClusterLimitRadius and create DHit::Analyse_2_cog()
// Last Modified: VR, 2014/08/28 add mecanism to keep untracked hits from last event
// Last Modified: LC, 2014/12/08 add copy operator to save mini-vectors Hits. (See DLadder::MakeMiniVectors)
// Last Modified: LC, 2014/12/15 class DHitMonteCarlo included in class DHit
// Last Modified: LC, 2014/12/15 Now, DHit contains a new vector of MC Info : std::vector<Double_t> _monteCarloInfo
// Last Modified: AP, 2015/05/21 The iterative clustering algorithm (Analyse_Iterative), now identifies big clusters 
//                               with high pixel multiplicity (> 100) which are caused maybe by nuclear interactions.
//                               The pixels in these big clusters are excluded from futher analysis.
// Last Modified: JB, 2015/05/25 Hit timestamp added
// Last Modified: JB, 2015/05/26 usage of TimeLimit between seed and neighbours
// Last Modified: AP, 2015/07/30 added a new private variable fMCPartID for simulation purposes with the ID number of the particle generating the hit
// Last Modified: AP, 2015/07/30 added a couple of functions SetMCPartID and GetMCPartID to set and get fMCPartID
// Last Modified: AP, 2016/07/28 added a new private variable fStripsFromMCPartID for simulation purposes with the number of strips/pixels from MC particle fMCPartID
// Last Modified: AP, 2016/07/28 added a couple of functions SetStripsFromMCPartID and GetStripsFromMCPartID to set and get fStripsFromMCPartID
// Last Modified: AP, 2017/05/09 added a function DoMCA to perform main component analysis

  ////////////////////////////////////////////////////////////
  //                                                        //
  // Class Description of DHit                              //
  //              
  // When Plane readout mode is < 100 (no zero suppression)
  //   call Analyse( GetStrip( stPhys))
  // When Plane readout mode is >= 100 (zero suppression)
  //   HitFinder == 0 => Analyse( seed, fListOfPixels)
  //   HitFinder == 1 => Analyse_Interative( seed, fListOfPixels)
  //   HitFinder == 2 => Analyse_2_cgo( seed, fListOfPixels)
  //
  ////////////////////////////////////////////////////////////

#include "DHit.h"
#include "DTrack.h"
#include "DPlane.h"
#include "DCut.h"
#include "DR3.h"
#include "DStrip.h"
#include "DSetup.h"
#include "DGlobalTools.h"

//extern  int fopen_lockfile(); // JB, removed for inline compilation
//extern  FILE * VG_Lockfile;


ClassImp(DHit) // Description of a Hit

//______________________________________________________________________________
//  
DHit::DHit()
{
  // DHit default ctr
  fIsFromPreviousEvent  =  0;    // VR 2014.08.28
  fIfMonteCarlo         =  0;    // LC 2014/12/15
  fMCHitID              = -1;    // AP 2015/07/30
  fStripsFromMCHitID    =  0;    // AP 2016/07/28
  fStripsInClusterFound =  0;
  fNormMaxStd           = -1000; // AP 2017/05/09
  fNormMinStd           = -1000; // AP 2017/05/09
  fMainAxisPhi          = -1000; // AP 2017/05/09
}
//______________________________________________________________________________
//  
DHit::DHit(DR3 &aPosition, DPlane& aPlane, Int_t aHitNumber)
{
  // DHit constructor
  //
  // Last Modified: JB 2009/05/22
 
  fHitNumber          = aHitNumber;
  fFound              = kFALSE; // JB 2009/05/22
  
  if(fHitNumber>0) {
    fPositionHit        = new DR3(aPosition);
    fPositionHitCG      = new DR3(aPosition);
    fPositionHitEta     = new DR3(aPosition);
    fPositionHitCG33    = new DR3(aPosition);
    fPositionHitCG22    = new DR3(aPosition);
    fPositionHitEta22   = new DR3(aPosition);
    fResolutionHit      = new DR3(0.0,0.0,0.0);

    fPlane              = &aPlane;
    fDebugHit           = fPlane->GetDebug();
    fCut                =  fPlane->GetCut();
    fClusterPulseSum    =  0.0;
    fClusterLimit       =  new DR3(fCut->GetClusterLimit());
    fClusterLimitRadius = fCut->GetClusterLimitRadius();
  
    fStripPitch         = &fPlane->GetStripPitch();
    fStripsInClusterArea = fCut->GetStripsInClusterArea();

    fPositionAlgorithm = fPlane->GetSetup().GetPlanePar(fPlane->GetPlaneNumber()).HitPositionAlgorithm;
    fStripsInClusterDemanded = 0;

    //fTableSize = 700;// VR 2014.08.28
    fTableSize = (fPlane->GetSetup()).GetPlanePar(fPlane->GetPlaneNumber()).MaxNStrips;
  
    tPixelIndexList        = new Int_t[fTableSize];   //AP 2016/07/28
    fStripIndexArray       = new Int_t[fTableSize];   // sufficient big, [fStripsInClusterArea];
    fStripIndex            = new Int_t[fTableSize];   // JB 2009/05/12
    fStripPulseHeight      = new Float_t[fTableSize]; // [fStripsInClusterArea];
    fStripNoise            = new Float_t[fTableSize]; // YV 27/11/09
    fStripDistanceU        = new Float_t[fTableSize]; // [fStripsInClusterArea];
    fStripDistanceV        = new Float_t[fTableSize]; // [fStripsInClusterArea];
    fSNneighbour = -1.0;
  
    fStoNover2 = 0; // MB/12/11/2010 
    fIsFromPreviousEvent = 0;// VR 2014.08.28
    fTimestamp = 0; // JB 2015/05/25
  }
  
  // Hit Monte Carlo
  fIfMonteCarlo       = 0;

  fMCHitID              = -1; // AP 2015/07/30
  fStripsFromMCHitID    =  0; // AP 2016/07/28
  fStripsInClusterFound =  0;
  
  fNormMaxStd           = -1000; // AP 2017/05/09
  fNormMinStd           = -1000; // AP 2017/05/09
  fMainAxisPhi          = -1000; // AP 2017/05/09
  
}
//______________________________________________________________________________
//  
DHit::DHit(DR3 &aPosition, DPlane& aPlane, Int_t aHitNumber, std::vector<Double_t>& monteCarloVector)
{
  // DHit constructor
  //
  // Last Modified: JB 2009/05/22
 
  fHitNumber          = aHitNumber;
  fFound              = kFALSE; // JB 2009/05/22
  
  if(fHitNumber>0) {
    fPositionHit        = new DR3(aPosition);
    fPositionHitCG      = new DR3(aPosition);
    fPositionHitEta     = new DR3(aPosition);
    fPositionHitCG33    = new DR3(aPosition);
    fPositionHitCG22    = new DR3(aPosition);
    fPositionHitEta22   = new DR3(aPosition);
    fResolutionHit      = new DR3(0.0,0.0,0.0);

    fPlane              = &aPlane;
    fDebugHit           = fPlane->GetDebug();
    fCut                =  fPlane->GetCut();
    fClusterPulseSum    =  0.0;
    fClusterLimit       =  new DR3(fCut->GetClusterLimit());
    fClusterLimitRadius = fCut->GetClusterLimitRadius();
  
    fStripPitch         = &fPlane->GetStripPitch();
    fStripsInClusterArea = fCut->GetStripsInClusterArea();

    fPositionAlgorithm = fPlane->GetSetup().GetPlanePar(fPlane->GetPlaneNumber()).HitPositionAlgorithm;
    fStripsInClusterDemanded = 0;

    //fTableSize = 500;// VR 2014.08.28
    fTableSize = (fPlane->GetSetup()).GetPlanePar(fPlane->GetPlaneNumber()).MaxNStrips;
  
    tPixelIndexList        = new Int_t[fTableSize];   //AP 2016/07/28
    fStripIndexArray       = new Int_t[fTableSize];   // sufficient big, [fStripsInClusterArea];
    fStripIndex            = new Int_t[fTableSize]; // JB 2009/05/12
    fStripPulseHeight      = new Float_t[fTableSize]; // [fStripsInClusterArea];
    fStripNoise            = new Float_t[fTableSize]; // YV 27/11/09
    fStripDistanceU        = new Float_t[fTableSize]; // [fStripsInClusterArea];
    fStripDistanceV        = new Float_t[fTableSize]; // [fStripsInClusterArea];
    fSNneighbour = -1.0;
  
    fStoNover2 = 0; // MB/12/11/2010 
    fIsFromPreviousEvent = 0;// VR 2014.08.28
    fTimestamp = 0; // JB 2015/05/25
  }
  
  // Hit Monte Carlo
  fIfMonteCarlo       = 1;
  _monteCarloInfo = monteCarloVector; // LC 2014/12/15
  
  fMCHitID              = -1;  // AP 2015/07/30
  fStripsFromMCHitID    =  0;  // AP 2016/07/28
  fStripsInClusterFound =  0;
  
  fNormMaxStd           = -1000; // AP 2017/05/09
  fNormMinStd           = -1000; // AP 2017/05/09
  fMainAxisPhi          = -1000; // AP 2017/05/09

}
//______________________________________________________________________________
//
void DHit::copy(DHit* aHit, DPlane* aPlane)  // Copy with minimal information (For mini-vectors alignement)
{
  fPlane              = aPlane;
  fFound              = kFALSE; // JB 2009/05/22
  fPositionHit        = new DR3(*aHit->GetPositionCG());
  fPositionHitCG      = new DR3(*aHit->GetPositionCG());
  //fPositionHitEta     = new DR3(*aHit->GetPositionCG());
  //fPositionHitCG33    = new DR3(*aHit->GetPositionCG());
  //fPositionHitCG22    = new DR3(*aHit->GetPositionCG());
  //fPositionHitEta22   = new DR3(*aHit->GetPositionCG());

  fDebugHit           = fPlane->GetDebug();
  fIfMonteCarlo       = 0;
  //Next : Commented to reduce memory consumption
  /* 
  fCut                =  fPlane->GetCut();
  fClusterPulseSum    =  0.0;
  fClusterLimit       =  new DR3(fCut->GetClusterLimit());

  fStripPitch         = &fPlane->GetStripPitch();
  fStripsInClusterArea = fCut->GetStripsInClusterArea();

  fPositionAlgorithm = fPlane->GetSetup().GetPlanePar(fPlane->GetPlaneNumber()).HitPositionAlgorithm;
  fStripsInClusterDemanded = 0; 
  */
  /*
  tPixelIndexList        = new Int_t[fTableSize];   //AP 2016/07/28
  fStripIndexArray       = new Int_t[500];   // sufficient big, [fStripsInClusterArea];
  fStripIndex            = new Int_t[500]; // JB 2009/05/12
  fStripPulseHeight      = new Float_t[500]; // [fStripsInClusterArea];
  fStripNoise            = new Float_t[500]; // YV 27/11/09
  fStripDistanceU        = new Float_t[500]; // [fStripsInClusterArea];
  fStripDistanceV        = new Float_t[500]; // [fStripsInClusterArea];
  fSNneighbour = -1.0;
  */  
  //fStoNover2 = 0; // MB/12/11/2010
  
  fMCHitID              = -1;  // AP 2015/07/30
  fStripsFromMCHitID    =  0;  // AP 2016/07/28
  fStripsInClusterFound =  0;
  
  fNormMaxStd           = -1000; // AP 2017/05/09
  fNormMinStd           = -1000; // AP 2017/05/09
  fMainAxisPhi          = -1000; // AP 2017/05/09
  
}
//______________________________________________________________________________
//
void DHit::clone(DHit *original, Bool_t fullCopy)// VR 2014.08.28
{
  fDebugHit                     = original->fDebugHit;
  //fHitNumber                    = original->fHitNumber;
  fFound                        = original->fFound;
  fPositionAlgorithm            = original->fPositionAlgorithm;
  fPlane                        = original->fPlane;
  *fPositionHit                 = *(original->fPositionHit);
  *fPositionHitCG               = *(original->fPositionHitCG);
  *fPositionHitEta              = *(original->fPositionHitEta);
  *fPositionHitCG33             = *(original->fPositionHitCG33);
  *fPositionHitCG22             = *(original->fPositionHitCG22);
  *fPositionHitEta22            = *(original->fPositionHitEta22);
  fSeedU                        = original->fSeedU;
  fSeedV                        = original->fSeedV;
  fClusterPulseSum              = original->fClusterPulseSum;
  fClusterAreaPulseSum          = original->fClusterAreaPulseSum;
  fClusterSignalToNoise         = original->fClusterSignalToNoise;
  fClusterNoiseAverage          = original->fClusterNoiseAverage;
  fStripsInClusterFound         = original->fStripsInClusterFound;
  fStripsInClusterArea          = original->fStripsInClusterArea;
  *fClusterLimit                = *(original->fClusterLimit);
  fClusterLimitRadius           = original->fClusterLimitRadius;
  fSeed                         = original->fSeed ;
  fPSeed                        = original->fPSeed;
  fCut                          = original->fCut;
  fStripPitch                   = original->fStripPitch;
  fPhSeed                       = original->fPhSeed;
  fPosSeed                      = original->fPosSeed;
  fPhLofSeed                    = original->fPhLofSeed;
  fPosLofSeed                   = original->fPosLofSeed;
  fPhRofSeed                    = original->fPhRofSeed;
  fPosRofSeed                   = original->fPosRofSeed;
  fIndexSeed                    = original->fIndexSeed;
  fIndexLofSeed                 = original->fIndexLofSeed;
  fIndexRofSeed                 = original->fIndexRofSeed;
  fPhLeft                       = original->fPhLeft;
  fPosLeft                      = original->fPosLeft;
  fPhRight                      = original->fPhRight;
  fPosRight                     = original->fPosRight;
  fIndexLeft                    = original->fIndexLeft;
  fIndexRight                   = original->fIndexRight;
  fStoNover2                    = original->fStoNover2;
  fStripsInClusterDemanded      = original->fStripsInClusterDemanded;
  fChargeFractionDensity        = original->fChargeFractionDensity;
  fSNneighbour                  = original->fSNneighbour;
  fSNseed                       = original->fSNseed;
  fSeedPulseHeight              = original->fSeedPulseHeight;
  fSeedNoise                    = original->fSeedNoise;
  fTimestamp                    = original->fTimestamp; // JB 2015/05/25
  fIsFromPreviousEvent          = original->fIsFromPreviousEvent;
  fIfMonteCarlo                 = original->fIfMonteCarlo;
  _monteCarloInfo               = original->_monteCarloInfo; // LC 2014/12/15
  fMCHitID                      = original->fMCHitID;            // AP 2016/07/28
  fStripsFromMCHitID            = original->fStripsFromMCHitID;  // AP 2016/07/28
  
  fNormMaxStd                   = original->fNormMaxStd;         // AP 2017/05/09
  fNormMinStd                   = original->fNormMinStd;         // AP 2017/05/09
  fMainAxisPhi                  = original->fMainAxisPhi;        // AP 2017/05/09
  
   
  if(fullCopy)
  {
    for (Int_t i=0 ; i<fTableSize ; i++)
    {
      fStripNoise[i]                   = original->fStripNoise[i];
      tPixelIndexList[i]               = original->tPixelIndexList[i]; //AP 2016/07/28
      fStripIndexArray[i]              = original->fStripIndexArray[i];
      fStripIndex[i]                   = original->fStripIndex[i];
      fStripPulseHeight[i]             = original->fStripPulseHeight[i];
      fStripDistanceU[i]               = original->fStripDistanceU[i];
      fStripDistanceV[i]               = original->fStripDistanceV[i];
    }
  }
}
 
//______________________________________________________________________________
//  
DHit::~DHit()
{ 

  // DHit default destructor 
  
  delete fPositionHit;
  delete fPositionHitCG;
  delete fPositionHitEta;
  delete fPositionHitCG33;
  delete fPositionHitCG22;
  delete fPositionHitEta22;

  delete fClusterLimit;

  /*
  if(fClusterLimit) delete fClusterLimit;
  if(fSeed) delete fSeed;
  if(fStripPitch) delete fStripPitch;
  if(fCut) delete fCut;
  */

  delete tPixelIndexList;
  delete fStripIndexArray;
  delete fStripIndex;
  delete fStripPulseHeight;
  delete fStripNoise;       //YV 15/10/09
  delete fStripDistanceU;
  delete fStripDistanceV;

  _monteCarloInfo.clear();

}

//______________________________________________________________________________
//  
Float_t DHit::GetPositionUhitCG() const 
{
  Float_t tUhitPos;
  tUhitPos = (*fPositionHitCG)(0);
  return tUhitPos;
}

//______________________________________________________________________________
//  
Float_t DHit::GetPositionVhitCG() const 
{
  Float_t tVhitPos;
  tVhitPos = (*fPositionHitCG)(1);
  return tVhitPos;
}

//______________________________________________________________________________
//  
Float_t DHit::GetPositionWhitCG() const 
{
  Float_t tWhitPos;
  tWhitPos = (*fPositionHitCG)(2);
  return tWhitPos;
}


//______________________________________________________________________________
//  
Float_t DHit::GetPositionUhitEta() const 
{
  Float_t tUhitPos;
  tUhitPos = (*fPositionHitEta)(0);
  return tUhitPos;
}

//______________________________________________________________________________
//  
Float_t DHit::GetPositionVhitEta() const 
{
  Float_t tVhitPos;
  tVhitPos = (*fPositionHitEta)(1);
  return tVhitPos;
}

//______________________________________________________________________________
//  
Float_t DHit::GetPositionUhitCG33() const 
{
  Float_t tUhitPos;
  tUhitPos = (*fPositionHitCG33)(0);
  return tUhitPos;
}

//______________________________________________________________________________
//  
Float_t DHit::GetPositionVhitCG33() const 
{
  Float_t tVhitPos;
  tVhitPos = (*fPositionHitCG33)(1);
  return tVhitPos;
}

//______________________________________________________________________________
//  
Float_t DHit::GetPositionUhitCG22() const 
{
  Float_t tUhitPos;
  tUhitPos = (*fPositionHitCG22)(0);
  return tUhitPos;
}

//______________________________________________________________________________
//  
Float_t DHit::GetPositionVhitCG22() const 
{
  Float_t tVhitPos;
  tVhitPos = (*fPositionHitCG22)(1);
  return tVhitPos;
}

//______________________________________________________________________________
//  
Float_t DHit::GetPositionUhitEta22() const 
{
  Float_t tUhitPos;
  tUhitPos = (*fPositionHitEta22)(0);
  return tUhitPos;
}

//______________________________________________________________________________
//  
Float_t DHit::GetPositionVhitEta22() const 
{
  Float_t tVhitPos;
  tVhitPos = (*fPositionHitEta22)(1);
  return tVhitPos;
}
//______________________________________________________________________________
//  
Float_t DHit::GetPositionUhit() const 
{
  Float_t tUhitPos;

  tUhitPos = (*fPositionHit)(0);

  return tUhitPos;
}
//______________________________________________________________________________
//  
Float_t DHit::GetPositionVhit() const 
{
  Float_t tVhitPos;

  tVhitPos = (*fPositionHit)(1);

  return tVhitPos;
}
//______________________________________________________________________________
//
Float_t DHit::GetResolutionUhit() const 
{
  Float_t tUhitRes;

  tUhitRes = (*fResolutionHit)(0);

  return tUhitRes;
}
//______________________________________________________________________________
//  
Float_t DHit::GetResolutionVhit() const 
{
  Float_t tVhitRes;

  tVhitRes = (*fResolutionHit)(1);

  return tVhitRes;
}
//______________________________________________________________________________
//
void DHit::SetResolutionUVhit(Float_t resolutionU,
			      Float_t resolutionV) {

  fResolutionHit->SetValue(resolutionU,resolutionV,0.0);

  return;
}
//______________________________________________________________________________
//  
Float_t DHit::GetPositionWhit() const 
{
  Float_t tWhitPos;

  tWhitPos = (*fPositionHit)(2);

  return tWhitPos;
}



// //______________________________________________________________________________
// //  
// Removed bu JB, 2009/05/12
// Float_t DHit::GetPulseHeight(Int_t tStripIndex) 
// {
//   if (tStripIndex < fSeed->GetNeighbourCount() && tStripIndex >= 0)
//     return fSeed->GetNeighbour(tStripIndex)->GetPulseHeight();
//   else {
//     printf("WARNING DHit, request to non-existent strip neighbour");
//     return 0.;
//   }
// }

//______________________________________________________________________________
//  
Float_t DHit::Distance( DHit *aHit) {
  // Return the distance between this hit and the pointed hit
  // the pointed hit position (from another plane) is extrapolated to the plane of this hit
  // assuming a flat slope 
  //
  // JB, 2009/05/21
  // Modified: JB, 2010/11/25, use new method to transform from one plane to the other

  // Get position in the plane of the foreign hit
  DR3 hitPosition( *(aHit->GetPosition()) );
  // printf("  hitPos(plane %d) = %f,  %f\n", aHit->GetPlane()->GetPlaneNumber(), hitPosition(0), hitPosition(1));
  // Transform position in tracker (XYZ) frame
  hitPosition = aHit->GetPlane()->PlaneToTracker( hitPosition);
  //printf("  hitPos(tracker) = %f,  %f\n", hitPosition(0), hitPosition(1));
  // Transform position in UV frame of this hit plane
  hitPosition = fPlane->TrackerToPlane( hitPosition);
  // printf("  hitPos(plane %d) = %f,  %f\n", fPlane->GetPlaneNumber(), hitPosition(0), hitPosition(1));

  // Now compute the distance beetween the two hits
  hitPosition -= *(GetPosition());

  // Insure that z position is 0 for 2D length computation
  hitPosition.SetValue( hitPosition(0), hitPosition(1), (Float_t)0.);
  //printf("  dist = %f,  %f, %f = %f\n", hitPosition(0), hitPosition(1), hitPosition(2), hitPosition.Length());
  
  return hitPosition.Length();
}

//______________________________________________________________________________
//  
Float_t DHit::DistanceMC( DHit *aHit) {
  // Return the distance between this hit and the pointed hit
  // the pointed hit position (from another plane) is extrapolated to the plane of this hit
  // assuming a flat slope 
  //
  // JB, 2009/05/21
  // Modified: JB, 2010/11/25, use new method to transform from one plane to the other

  // Get position in the plane of the foreign hit
  DR3 hitPosition( aHit->GetMonteCarloPosition() );
  // printf("  hitPos(plane %d) = %f,  %ftPixelIndexList\n", aHit->GetPlane()->GetPlaneNumber(), hitPosition(0), hitPosition(1));
  // Transform position in tracker (XYZ) frame
  hitPosition = aHit->GetPlane()->PlaneToTracker( hitPosition);
  //printf("  hitPos(tracker) = %f,  %f\n", hitPosition(0), hitPosition(1));
  // Transform position in UV frame of this hit plane
  hitPosition = fPlane->TrackerToPlane( hitPosition);
  // printf("  hitPos(plane %d) = %f,  %f\n", fPlane->GetPlaneNumber(), hitPosition(0), hitPosition(1));

  // Now compute the distance beetween the two hits
  hitPosition -= *(GetPosition());

  // Insure that z position is 0 for 2D length computation
  hitPosition.SetValue( hitPosition(0), hitPosition(1), (Float_t)0.);
  //printf("  dist = %f,  %f, %f = %f\n", hitPosition(0), hitPosition(1), hitPosition(2), hitPosition.Length());
  
  return hitPosition.Length();
}

//______________________________________________________________________________
//  
Float_t DHit::Distance( DTrack *aTrack) {
  // Return the distance between this hit and the pointed track impact in the plane
  //
  // JB, 2009/06/26

  DR3 impactPosition( aTrack->Intersection( fPlane) );
  impactPosition -= *(GetPosition());

  // Insure that z position is 0 for 2D length computation
  impactPosition.SetValue( impactPosition(0), impactPosition(1), (Float_t)0.);
  //printf("  dist = %f,  %f, %f = %f\n", hitPosition(0), hitPosition(1), hitPosition(2), hitPosition.Length());
  
  return impactPosition.Length();
}

//______________________________________________________________________________
//  
Float_t DHit::DistanceMC( DTrack *aTrack) {
  // Return the distance between this hit and the pointed track impact in the plane
  //
  // JB, 2009/06/26

  DR3 impactPosition( aTrack->Intersection( fPlane) );
  impactPosition -= GetMonteCarloPosition();

  // Insure that z position is 0 for 2D length computation
  impactPosition.SetValue( impactPosition(0), impactPosition(1), (Float_t)0.);
  //printf("  dist = %f,  %f, %f = %f\n", hitPosition(0), hitPosition(1), hitPosition(2), hitPosition.Length());
  
  return impactPosition.Length();
}

//______________________________________________________________________________
//  
Float_t DHit::Distance( DR3 *aPoint) {
  // Return the distance between this hit and the point
  // which coordinates are assumed to be given in the frame of the hit plane.
  //
  // JB, 2011/07/25
  
  DR3 aPosition( *aPoint );
  aPosition -= *(GetPosition());
  
  // Insure that z position is 0 for 2D length computation
  aPosition.SetValue( aPosition(0), aPosition(1), (Float_t)0.);
  //printf("  dist = %f,  %f, %f = %f\n", hitPosition(0), hitPosition(1), hitPosition(2), hitPosition.Length());
  
  return aPosition.Length();
}

//______________________________________________________________________________
//  
Float_t DHit::DistanceMC( DR3 *aPoint) {
  // Return the distance between this hit and the point
  // which coordinates are assumed to be given in the frame of the hit plane.
  //
  // JB, 2011/07/25
  
  DR3 aPosition( *aPoint );
  aPosition -= GetMonteCarloPosition();
  
  // Insure that z position is 0 for 2D length computation
  aPosition.SetValue( aPosition(0), aPosition(1), (Float_t)0.);
  //printf("  dist = %f,  %f, %f = %f\n", hitPosition(0), hitPosition(1), hitPosition(2), hitPosition.Length());
  
  return aPosition.Length();
}

//______________________________________________________________________________
//  
Bool_t DHit::Analyse(DStrip* aStrip)
{
  // Construct a hit (or cluster) from a seed strip:
  // 1) try to associate all neighbouring strips that pass selection cut
  // 2) check the build hit passes all selections
  // 3) order the array fStripIndexArray of neighbours by decreasing pulseheight
  // 4) compute cluster charge, noise and position
  //
  // Return a boolean: kTRUE if hit is selected, kFalse otherwise
  //
  // re-organized a little bit by JB, 2007 June
  // Different CoGs and Etas added by JB, 2007 Nov
  // Realeasing (Found=false) neighbour strips for rejected hit, JB 2012/08/18
  // Storing noise of strips, JB 2012/08/19
  
  fFound      = kFALSE; // JB 2009/05/22
  fSeed       = aStrip;
  fPSeed      = NULL;

  DStrip   *aNeighbour;
  Bool_t    valid    = kFALSE; // default return value
  Bool_t    tDigital = fPlane->GetStripResponseSetting();
  Int_t     tStripIndex;
  Int_t     tStripsInClusterPossible = 0; // remove a warning 

  fPositionHitCG->Zero();           // clear the position
  fPositionHitEta->Zero();
  fPositionHitEta22->Zero(); // JB 2010/12/8
  fPositionHit->Zero();
  fClusterSignalToNoise   = 0.0;

  fSNseed              = fSeed->GetPulseHeightToNoise(); // JB 2013/11/08
  fSeedPulseHeight     = fSeed->GetPulseHeight(); // Note that seed might change
  fSeedNoise           = fSeed->GetNoise();

  fSeedU      = fSeed->GetPosition()(0);
  fSeedV      = fSeed->GetPosition()(1); 
  fIndexSeed  = fSeed->GetStripIndex();
  
  // here, the neighbourhood of strips is assumed to be ordered, 
  // at index 0 is the seed, to higher indices
  // the neighbourstrips are further away from the seed.

  Float_t tClusterAreaNoise = 0.;
  fClusterAreaPulseSum      = 0.; // area means the cluster except the seed
  fClusterPulseSum        = fSeed->GetPulseHeight(); 
  fClusterNoiseAverage    = (fSeed->GetNoise())*(fSeed->GetNoise());  // => so it's only a seed  strip in the beginning
  fStripsInClusterFound   = 1;                       
  fSeed->SetFound(kTRUE);                            
  
  if (fStripsInClusterDemanded >0)     // exact number demanded
    tStripsInClusterPossible = fStripsInClusterDemanded;
  if (fStripsInClusterDemanded == 0)     // free number allowed
    tStripsInClusterPossible = fSeed->GetNeighbourCount();
  
  fStripIndexArray[0]  = 0;                            // store seed index
  //fStripIndex[0]       = fPSeed->GetPixelIndex();      // store seed index, JB 2009/05/12
  fStripIndex[0] = fSeed->GetStripIndex();              // store seed index,changing the previous line YV 27/11/09
  fStripPulseHeight[0] = fSeed->GetPulseHeight();      // store seed pulseheight
  fStripNoise[0]       = fSeed->GetNoise();            // JB 2012/08/19
  fStripDistanceU[0] = 0.;                             // distance seed to seed = 0.
  fStripDistanceV[0] = 0.;                             // distance seed to seed = 0.

  //===============
  // associate neighbouring strip
  //===============
  tStripIndex = 1; // start with the first neighbour, in the geometric ordered neighbourhood, avoid 0 because it is the strip itself!
 
  if(fDebugHit>1) printf("  DHit: analyse seed strip %d (pixel %d) (q=%f) with possibly %d neighbours\n", fIndexSeed, fSeed->GetPixelIndex(), fSeed->GetPulseHeight(), tStripsInClusterPossible);

  while ( tStripIndex < tStripsInClusterPossible ) { // Loop on all possible neighbouring strips
    // get the neighbour
    aNeighbour = fSeed->GetNeighbour(tStripIndex);  
    if(fDebugHit>2) printf( "  testing neighbour %d (strip index %d), found=%d\n", tStripIndex, aNeighbour->GetStripIndex(), aNeighbour->Found());
    //--------------------------------------------------
    if(!(aNeighbour->Found())){ // to avoid doublecounting of the same pixels

      fClusterAreaPulseSum     += aNeighbour->GetPulseHeight();
      tClusterAreaNoise +=(aNeighbour->GetNoise())*(aNeighbour->GetNoise());
      // clear 
      fStripIndexArray[tStripIndex]  = tStripIndex; // put strip index in array
      fStripIndex[tStripIndex]       = aNeighbour->GetStripIndex(); // this is the index in the matrix, JB 2009/05/12
      fStripPulseHeight[tStripIndex] = aNeighbour->GetPulseHeight();
      fStripNoise[tStripIndex]       = aNeighbour->GetNoise(); // JB 2012/08/19
      fStripDistanceU[tStripIndex] = fSeed->DistanceU(aNeighbour->GetPosition());
      fStripDistanceV[tStripIndex] = fSeed->DistanceV(aNeighbour->GetPosition());
      // check whether strip has sufficient pulsheight-to-noise ratio
      // and whether it is inside the geometrical cluster limits.
      
      fStripsInClusterFound++;
      // increment counter of continous adjacent strips
      aNeighbour->SetFound(kTRUE);
      if( fDebugHit>1) printf("  neighbour %d at index %d added as %dth pixel with noise %.1f and pulseheight %.1f!\n", tStripIndex, aNeighbour->GetStripIndex(), fStripsInClusterFound-1, fStripNoise[tStripIndex], fStripPulseHeight[tStripIndex]);
      fClusterNoiseAverage   += (aNeighbour->GetNoise())*(aNeighbour->GetNoise());
      fClusterPulseSum       += aNeighbour->GetPulseHeight();
      //---abesson september 2005
      //(fSeed->GetSetup())->GetPlanePar(fPlane->GetPlaneNumber()).Readout;  // do nothing, to be checked BH 2013/08/20
    } // end if pixel already counted

    else {
      //--instead of pixels taking part in other clusters put null charge
      fStripIndexArray[tStripIndex]  = tStripIndex; // put strip index in array
      fStripPulseHeight[tStripIndex] = 0.;
      fStripsInClusterFound++;
    }

    tStripIndex++;    // increment the strip index counter

  } // End Loop on all possible neighbouring strips


  //=============
  // order pixels
  //=============
  if(fPlane->GetAnalysisMode()!=3){ // skip for binary pixel output, JB 2013/11/05
    DGlobalTools aTool; // JB 2011/07/18
    aTool.OrderIndexDes(&fStripIndexArray[0], &fStripPulseHeight[0], tStripIndex-1);
    if( fDebugHit>1) {
      printf("DHit: Analyse after %d strips ordering by pulseheight:\n", fStripsInClusterFound);
      for( Int_t iStripIndex=0; iStripIndex < fStripsInClusterFound; iStripIndex++ ) {
        printf("    neighbour %d/%d/%d at strip index %d found with noise %.1f and pulseheight %.1f!\n", iStripIndex, fStripIndex[iStripIndex], fStripIndexArray[iStripIndex], fSeed->GetNeighbour(fStripIndexArray[iStripIndex])->GetStripIndex(), fStripNoise[fStripIndexArray[iStripIndex]], fStripPulseHeight[fStripIndexArray[iStripIndex]]); 
      }
    }
  } // end skip for binary pixel output
  
  //==============
  // Additional properties
  //==============  
  // The number of strips in the cluster and the cluster charge sum are now known
  // Depending wether the noise is calculated or not, 
  //  the SN ratio is a real ratio or simply the charge
  // JB 2010/10/05 and 2010/10/15 to separately compute Cluster and Area values
  
  tClusterAreaNoise  = sqrt(tClusterAreaNoise);
  
  if (fClusterNoiseAverage > 0.5) {
    fClusterSignalToNoise  = fClusterPulseSum / sqrt(fClusterNoiseAverage);
  }
  else {
    fClusterSignalToNoise  = fClusterPulseSum;
  }
  
  if (tClusterAreaNoise > 0.5) {
    fSNneighbour           = fClusterAreaPulseSum / tClusterAreaNoise;
  }
  else {
    fSNneighbour           = fClusterAreaPulseSum;
  }
  
  if(fDebugHit>1) printf("  DHit:Analyse  potential hit has : PulseSum=%f, areaPulseSum=%f, noise=%f, areaNoise=%f, clusterSignalToNoise=%f, areaSignalToNoise=%f <?> cut %f, # pixels=%d within? [%d, %d]\n", fClusterPulseSum, fClusterAreaPulseSum, fClusterNoiseAverage, tClusterAreaNoise, fClusterSignalToNoise, fSNneighbour, fCut->GetNeighbourPulseHeightToNoise(), fStripsInClusterFound, fCut->GetStripsInClusterMin(), fCut->GetStripsInClusterMax());
  
  
  //==============
  // Select or not the hit
  //==============  
  
  if( fSNneighbour >= fCut->GetNeighbourPulseHeightToNoise() 
     && fCut->GetStripsInClusterMin() <= fStripsInClusterFound
     && fStripsInClusterFound <= fCut->GetStripsInClusterMax()
     ){
    valid = kTRUE;
  
    fTimestamp = 0.; // JB 2017/10/13

    //===============
    // Compute hit position
    //===============
    
    // start to  calculate the exact hit position, which is in first order the seed strip position  
    (*fPositionHit)(0)    = (fSeed->GetPosition())(0);
    (*fPositionHit)(1)    = (fSeed->GetPosition())(1);
    (*fPositionHit)(2)    = (fSeed->GetPosition())(2);
    
    // This is the container (3D distance) for the correction to be computed with different algorithm
    DR3 tCorrection ; tCorrection.SetValue(0.,0.,0.);
    DR3 tCorTemp ;
    
    
    //===============
    //---- for planes using STRIPS:
    //
    // ref plane condition removed by JB, 2007 June
    //if(fPlane->GetStatus()<3 && fPlane->GetAnalysisMode()<2){ // select reference planes only
    if(fPlane->GetAnalysisMode()<2){ // select STRIP planes
      
      fPhSeed       =  fSeed->GetPulseHeight();
      fPosSeed      =  fSeedU;              
      fPhRofSeed    =  fSeed->GetNeighbour(1)->GetPulseHeight();
      fPhLofSeed    =  fSeed->GetNeighbour(2)->GetPulseHeight();
      fPosRofSeed   = (fSeed->GetNeighbour(1)->GetPosition())(0);
      fPosLofSeed   = (fSeed->GetNeighbour(2)->GetPosition())(0);
      fIndexRofSeed =  fSeed->GetNeighbour(1)->GetStripIndex();
      fIndexLofSeed =  fSeed->GetNeighbour(2)->GetStripIndex();
      fIndexSeed    =  fSeed->GetStripIndex();
      
      Float_t tSeedSign = fPhSeed/fabs(fPhSeed);
      
      if ( tSeedSign*fPhLofSeed > tSeedSign*fPhRofSeed ) {
        fPhLeft     = fPhLofSeed;
        fPhRight    = fPhSeed;
        fPosLeft    = fPosLofSeed;
        fPosRight   = fPosSeed;
        fIndexLeft  = fIndexLofSeed; 
        fIndexRight = fIndexSeed;
      } else {
        fPhLeft     = fPhSeed;
        fPhRight    = fPhRofSeed;
        fPosLeft    = fPosSeed;
        fPosRight   = fPosRofSeed;
        fIndexLeft  = fIndexSeed;
        fIndexRight = fIndexRofSeed;
      }
      
      
      // Charge Fraction algorithm optimizes hit position, comprises other known algorithms like
      // center of gravitiy           : weight_k = charge fraction
      // 2 strip linear eta algorithm : center of gravity with 2 strips in cluster
      // 2 strip non-linear eta       : 2 strips, weights calculated from charge fraction distribution on strips
      
      Float_t tChargeFraction, aWeight ;
      
      TH1F   *tChargeFractionDensityData;
      Int_t   tDensityBinsN, tBin;
      Float_t tLL, tUL;
      
      Int_t   tClusterType; 
      Int_t   tHk;
      
      tClusterType = 2;
      Float_t tDistU;
      if(fPositionAlgorithm==1){
        if(fPhLeft < fPhRight){
          tChargeFraction  = fPhLeft / (fPhLeft+fPhRight); // seed is on right
          aWeight          = tChargeFraction;              // this is C.o.G. Method
          tDistU           = fPosRight-fPosLeft;  
        }else {
          tChargeFraction  = fPhRight / (fPhLeft+fPhRight); // seed is right
          aWeight          = tChargeFraction;               // this is C.o.G. Method
          tDistU           = -fPosRight+fPosLeft;  
        }
        tCorTemp.SetValue(tDistU,(Float_t)0.,(Float_t)0.) ;
        tCorTemp *= aWeight ;
        
        tCorrection += tCorTemp ;
        
        // assign the digital strip position
        *fPositionHitCG    = *fPositionHit     ;
        // and now change u-value with CoG correction
        (*fPositionHitCG) -= tCorrection       ; 
        (*fPositionHit)    = (*fPositionHitCG) ; // needed if CoG is used
      }
      
      if ( tDigital ){
        if(fDebugHit>1) {
          printf("DHit.cxx:evaluateHitCluster = Digital response only set (forgot about etaXXX.root file?) for plane %d \n",fPlane->GetPlaneNumber());
        }
        
      }
      
      else if ( fPositionAlgorithm==2){
        
        tCorrection.SetValue(0.,0.,0.);
        // treat seed and strip with next highest pulse (2 strips):
        if(fPhLeft < fPhRight){
          tChargeFraction  = fPhLeft / (fPhLeft+fPhRight); // seed is right
          tDistU           = fPosRight-fPosLeft;  
          
        }else {
          tChargeFraction  = fPhRight / (fPhLeft+fPhRight); // seed is right
          tDistU           = -fPosRight+fPosLeft;  
        }
        tHk = 1 + (tClusterType-2)*2; // so tHk = 1??
        tChargeFractionDensityData = fPlane->GetChargeFractionDensity(tHk);
        tDensityBinsN = tChargeFractionDensityData->GetNbinsX();
        tLL           = tChargeFractionDensityData->GetXaxis()->GetXmin();
        tUL           = tChargeFractionDensityData->GetXaxis()->GetXmax();
        tBin = (Int_t)( (tChargeFraction-tLL) * tDensityBinsN/(tUL-tLL) );
        
        if (tBin>tDensityBinsN) tBin = tDensityBinsN - 50;
        if (tBin<0) tBin = 50;
        
        aWeight = tChargeFractionDensityData->GetBinContent(tBin);       
        tCorTemp.SetValue(tDistU,(Float_t)0.,(Float_t)0.) ;
        tCorTemp *= aWeight ;
        
        tCorrection += tCorTemp ;
        (*fPositionHit) -= tCorrection;
        
      } //end of if PositionAlgorithm ==2 
    } // end select STRIP planes
    
    
    //===============
    //--- for PIXEL planes
    //
    else {
      
      //==================
      //Center of gravity with all the neighbours
      // pay attention that pixels are ordered geometricaly from one corner to the other in the fStripDistanceU/V arrays
      
      tCorrection.SetValue( 0., 0., 0.);
      Float_t tClusterPulseSum = 0.;
      for( Int_t iStripIndex=0; iStripIndex < fStripsInClusterFound; iStripIndex++ ) {
        tCorTemp.SetValue( ((fSeed->GetPosition())(0)-fStripDistanceU[iStripIndex])*fStripPulseHeight[iStripIndex],
                          ((fSeed->GetPosition())(1)-fStripDistanceV[iStripIndex])*fStripPulseHeight[iStripIndex],
                          0);
        tCorrection += tCorTemp;
        tClusterPulseSum += fStripPulseHeight[iStripIndex];
        //printf("DHit::Analyse plane %d, hit %2d, strip %2d, corr(%.2f, %.2f)=dist(%.2f, %.2f)*%.0f/%.0f\n", fPlane->GetPlaneNumber(), fHitNumber, iStripIndex, tCorrection(0), tCorrection(1), fStripDistanceU[iStripIndex], fStripDistanceV[iStripIndex], fStripPulseHeight[iStripIndex], tClusterPulseSum);
      }
      
      *fPositionHitCG = tCorrection/tClusterPulseSum;
      
      //==================
      //Center of gravity restricted to a 3x3 cluster
      
      tCorrection.SetValue( 0., 0., 0.);
      Float_t tCluster33PulseSum = 0.;
      for( Int_t iStripIndex=0; iStripIndex < fStripsInClusterFound; iStripIndex++ ) {
        if(    fabs(fStripDistanceU[iStripIndex])<=fSeed->GetPlane()->GetStripPitch()(0)
           && fabs(fStripDistanceV[iStripIndex])<=fSeed->GetPlane()->GetStripPitch()(1)
           ) { // check neighbours is at most at 1 pitch from seed
          tCorTemp.SetValue( ((fSeed->GetPosition())(0)-fStripDistanceU[iStripIndex])*fStripPulseHeight[iStripIndex],
                            ((fSeed->GetPosition())(1)-fStripDistanceV[iStripIndex])*fStripPulseHeight[iStripIndex],
                            0);
          tCorrection += tCorTemp;
          tCluster33PulseSum += fStripPulseHeight[iStripIndex];
          //printf("DHit::Analyse plane %d, hit %2d, strip %2d, corr(%.2f, %.2f)=dist(%.2f, %.2f)*%.0f/%.0f\n", fPlane->GetPlaneNumber(), fHitNumber, iStripIndex, tCorrection(0), tCorrection(1), fStripDistanceU[iStripIndex], fStripDistanceV[iStripIndex], fStripPulseHeight[iStripIndex], tCluster33PulseSum);
        }
      }
      *fPositionHitCG33 = tCorrection/tCluster33PulseSum;
      
      
      //==================
      //Center of gravity restricted 2x2 neighbours with the highest charge, JB Nov 2007
      // 2x2 includes the seed
      
      // The index of the neighbours with the highest charges are ordered in fStripIndexArray
      
      tCorrection.SetValue( 0., 0., 0.);
      Float_t tCluster22PulseSum = 0.;
      Int_t tOrderedIndex;
      for( Int_t iStripIndex=0; iStripIndex < 4; iStripIndex++ ) {
        tOrderedIndex = fStripIndexArray[iStripIndex];
        tCorTemp.SetValue( (fSeed->GetPosition()(0)-fStripDistanceU[tOrderedIndex])*fStripPulseHeight[tOrderedIndex],
                          (fSeed->GetPosition()(1)-fStripDistanceV[tOrderedIndex])*fStripPulseHeight[tOrderedIndex],
                          0);
        tCorrection += tCorTemp;
        tCluster22PulseSum += fStripPulseHeight[tOrderedIndex];
      }
      *fPositionHitCG22 = tCorrection/tCluster22PulseSum; 
      
      
      //==================
      //Eta algorithm 3x3
      // correct the 3x3 center of gravity (tCorrection) using the integral histogram of the charge density
      // the eta position is the Y value of the bin with X value = tCorrection
      // if the tCorrection from CoG is out of the X axis range, do not correct
      
      if ( tDigital ){ // check info file was there and Eta is available
        if(fDebugHit>1) {
          printf("  DHit.cxx:evaluateHitCluster = Digital or CoG response only set for plane %d (forgot about etaXXX.root file?)\n",fPlane->GetPlaneNumber());
        }
      }
      else {
        
        TH1  *tEtaIntU = fPlane->GetEtaIntU();
        TH1  *tEtaIntV = fPlane->GetEtaIntV();
        Int_t iBinU    = tEtaIntU->FindBin( (*fPositionHitCG33-*fPositionHit)(0) );
        Int_t iBinV    = tEtaIntV->FindBin( (*fPositionHitCG33-*fPositionHit)(1) );
        Double_t corrU=0., corrV=0.;
        
        // correct wrt CoG if possible
        if( 1<=iBinU && iBinU<=tEtaIntU->GetNbinsX() ) corrU = tEtaIntU->GetBinContent( iBinU);
        if( 1<=iBinV && iBinV<=tEtaIntV->GetNbinsX() ) corrV = tEtaIntV->GetBinContent( iBinV);
        tCorrection.SetValue( corrU, corrV, 0.);
        
        *fPositionHitEta = *fPositionHit + tCorrection;
      }
      
      //==================
      //Eta algorithm 2x2
      // correct the 2x2 center of gravity (tCorrection) using the integral histogram of the charge density
      // the eta position is the Y value of the bin with X value = tCorrection
      // if the tCorrection from CoG is out of the X axis range, do not correct
      
      if ( tDigital ){ // check info file was there and Eta is available
        if(fDebugHit>1) {
          printf("  DHit.cxx:evaluateHitCluster = Digital or CoG response only set for plane %d (forgot about etaXXX.root file?)\n",fPlane->GetPlaneNumber());
        }
      }
      else {
        
        TH1  *tEtaIntU2 = fPlane->GetEtaIntU2();
        TH1  *tEtaIntV2 = fPlane->GetEtaIntV2();
        Int_t iBinU    = tEtaIntU2->FindBin( (*fPositionHitCG22-*fPositionHit)(0) );
        Int_t iBinV    = tEtaIntV2->FindBin( (*fPositionHitCG22-*fPositionHit)(1) );
        Double_t corrU=0., corrV=0.;
        
        // correct wrt CoG if possible
        if( 1<=iBinU && iBinU<=tEtaIntU2->GetNbinsX() ) corrU = tEtaIntU2->GetBinContent( iBinU);
        if( 1<=iBinV && iBinV<=tEtaIntV2->GetNbinsX() ) corrV = tEtaIntV2->GetBinContent( iBinV);
        tCorrection.SetValue( corrU, corrV, 0.);
        
        *fPositionHitEta22 = *fPositionHit + tCorrection;
      }
      
      //==================
      //Analog head-tail algorithm
      // Algorithm used for large incident angle when seed contains little info on position
      // Relies on the extreme pixels on the left and right
      // tCorrection.SetValue( (fStripDistanceU[iLeft]+fStripDistanceU[iRight])/2., , 0.);
      // *fPositionhitAHT = *fPositionHit - tCorrection;
      
      //==================
      // Now choose which position is stored
      if( fPositionAlgorithm==1 ) { // Center of Gravity
        *fPositionHit = *fPositionHitCG;
      }
      else if( fPositionAlgorithm==11 ) { // Center of gravity on 3x3
        *fPositionHit = *fPositionHitCG33;
      }
      else if( fPositionAlgorithm==12 ) { // Center of gravity on 2x2
        *fPositionHit = *fPositionHitCG22;
      }
      else if( fPositionAlgorithm==2 ) { // Eta corrected from CoG 3x3
        *fPositionHit = *fPositionHitEta;
      }
      else if( fPositionAlgorithm==22 ) { // Eta corrected from CoG 2x2
        *fPositionHit = *fPositionHitEta22;
      }
      else {
        printf("-*-*- WARNING: algorithm %d for position unknown, taking digital position\n", fPositionAlgorithm);
      }
      
    } // end of PIXEL planes
    
    if(fDebugHit>1) printf("        hit selected with %d pixels\n", fStripsInClusterFound);
    
  } // end if hit selected

  // ========================
  else{ // hit rejected
    valid = kFALSE;
    if(fDebugHit>1) printf("        hit rejected, releasing %d pixels\n", fStripsInClusterFound-1);
    
    // Release the neighbouring Strips associated to the seed
    // only the tested seed (Index=0) is not released
    // JB 2012/08/18
    for( Int_t iStripIndex=1; iStripIndex < fStripsInClusterFound; iStripIndex++ ) { // loop over associated strips
      if(fDebugHit>2) printf("  DHit.cxx:Analyse    releasing neighbour strip %d at index %d\n", iStripIndex, fStripIndex[iStripIndex]);
      fSeed->GetNeighbour(iStripIndex)->SetFound(kFALSE);
    } // end loop over associated strips

  }

  return valid;
}

//______________________________________________________________________________
//  
Bool_t DHit::Analyse_Iterative(DStrip* aStrip, bool &IsBigCluster, int MaxClusterSize)
{
  // Construct a hit (or cluster) from a seed strip:
  // 1) try to associate all neighbouring strips that pass selection cut
  // 2) check the build hit passes all selections
  // 3) order the array fStripIndexArray of neighbours by decreasing pulseheight
  // 4) compute cluster charge, noise and position
  //
  // Return a boolean: kTRUE if hit is selected, kFalse otherwise
  //
  // re-organized a little bit by JB, 2007 June
  // Different CoGs and Etas added by JB, 2007 Nov
  // Realeasing (Found=false) neighbour strips for rejected hit, JB 2012/08/18
  // Storing noise of strips, JB 2012/08/19

  fFound      = kFALSE; // JB 2009/05/22
  fSeed       = aStrip;

  DStrip   *aNeighbour;
  DStrip   *aPixel;
  Bool_t    valid    = kFALSE; // default return value
  Bool_t    tDigital = fPlane->GetStripResponseSetting();
  Int_t     tStripIndex;
  Int_t     tStripsInClusterPossible = 0; // remove a warning 

  fPositionHitCG->Zero();           // clear the position
  fPositionHitEta->Zero();
  fPositionHitEta22->Zero(); // JB 2010/12/8
  fPositionHit->Zero();
  fClusterSignalToNoise   = 0.0;

  fSNseed              = fSeed->GetPulseHeightToNoise(); // JB 2013/11/08
  fSeedPulseHeight     = fSeed->GetPulseHeight(); // Note that seed might change
  fSeedNoise           = fSeed->GetNoise();

  fSeedU      = fSeed->GetPosition()(0);
  fSeedV      = fSeed->GetPosition()(1); 
  fIndexSeed  = fSeed->GetStripIndex();
  
  // here, the neighbourhood of strips is assumed to be ordered, 
  // at index 0 is the seed, to higher indices
  // the neighbourstrips are further away from the seed.

  Float_t tClusterAreaNoise = 0.;
  fClusterAreaPulseSum      = 0.; // area means the cluster except the seed
  fClusterPulseSum          = fSeed->GetPulseHeight(); 
  fClusterNoiseAverage      = (fSeed->GetNoise())*(fSeed->GetNoise());  // => so it's only a seed  strip in the beginning
  fStripsInClusterFound     = 1;                       
  fSeed->SetFound(kTRUE);
  
  if (fStripsInClusterDemanded >0)     // exact number demanded
    tStripsInClusterPossible = fStripsInClusterDemanded;
  if (fStripsInClusterDemanded == 0)     // free number allowed
    tStripsInClusterPossible = fSeed->GetNeighbourCount();
  
  fStripIndexArray[0]  = 0;                            // store seed index
  fStripIndex[0]       = fSeed->GetStripIndex();       // store seed index,changing the previous line YV 27/11/09
  fStripPulseHeight[0] = fSeed->GetPulseHeight();      // store seed pulseheight
  fStripNoise[0]       = fSeed->GetNoise();            // JB 2012/08/19
  fStripDistanceU[0]   = 0.0;                          // distance seed to seed = 0.
  fStripDistanceV[0]   = 0.0;                          // distance seed to seed = 0.

  //===============
  // associate neighbouring strip
  //===============
  tStripIndex = 1; // start with the first neighbour, in the geometric ordered neighbourhood, avoid 0 because it is the strip itself!
 
  if(fDebugHit>1) printf("  DHit: analyse seed strip %d (pixel %d) (q=%f) with possibly %d neighbours\n", fIndexSeed, fSeed->GetPixelIndex(), fSeed->GetPulseHeight(), tStripsInClusterPossible);

  if( fPlane->GetAnalysisMode() < 2) { // if analog strip readout
    while ( tStripIndex < tStripsInClusterPossible ) { // Loop on all possible neighbouring strips
      // get the neighbour
      aNeighbour = fSeed->GetNeighbour(tStripIndex);  
      if(fDebugHit>2) printf( "  testing neighbour %d (strip index %d), found=%d\n", tStripIndex, aNeighbour->GetStripIndex(), aNeighbour->Found());
      //--------------------------------------------------
      if(!(aNeighbour->Found())){ // to avoid doublecounting of the same pixels

        fClusterAreaPulseSum     += aNeighbour->GetPulseHeight();
        tClusterAreaNoise +=(aNeighbour->GetNoise())*(aNeighbour->GetNoise());
        // clear 
        fStripIndexArray[tStripIndex]  = tStripIndex; // put strip index in array
        fStripIndex[tStripIndex]       = aNeighbour->GetStripIndex(); // this is the index in the matrix, JB 2009/05/12
        fStripPulseHeight[tStripIndex] = aNeighbour->GetPulseHeight();
        fStripNoise[tStripIndex]       = aNeighbour->GetNoise(); // JB 2012/08/19
        fStripDistanceU[tStripIndex] = fSeed->DistanceU(aNeighbour->GetPosition());
        fStripDistanceV[tStripIndex] = fSeed->DistanceV(aNeighbour->GetPosition());
        // check whether strip has sufficient pulsheight-to-noise ratio
        // and whether it is inside the geometrical cluster limits.
      
        fStripsInClusterFound++;
        // increment counter of continous adjacent strips
        aNeighbour->SetFound(kTRUE);
        if( fDebugHit>1) printf("  neighbour %d at index %d added as %dth pixel with noise %.1f and pulseheight %.1f!\n", tStripIndex, aNeighbour->GetStripIndex(), fStripsInClusterFound-1, fStripNoise[tStripIndex], fStripPulseHeight[tStripIndex]);
        fClusterNoiseAverage   += (aNeighbour->GetNoise())*(aNeighbour->GetNoise());
        fClusterPulseSum       += aNeighbour->GetPulseHeight();
        //---abesson september 2005
        //(fSeed->GetSetup())->GetPlanePar(fPlane->GetPlaneNumber()).Readout;  // do nothing, to be checked BH 2013/08/20
      } // end if pixel already counted
      else {
        //--instead of pixels taking part in other clusters put null charge
        fStripIndexArray[tStripIndex]  = tStripIndex; // put strip index in array
        fStripPulseHeight[tStripIndex] = 0.;
        fStripsInClusterFound++;
      }

      tStripIndex++;    // increment the strip index counter

    } // End Loop on all possible neighbouring strips
  } // end if analog strip readout
  else { //if pixel analog or binary readout
    //Doing a different cultering for binary readout using a iterative algorithm (most likely not optimal)
    
    int seedIdx = fSeed->GetStripIndex();
    int seedCol = seedIdx%fPlane->GetStripsNu();
    int seedRow = seedIdx/fPlane->GetStripsNu();
    
    if(fPlane->GetAnalysisMode() == 2 && (seedCol == 0 && seedRow == 0)) return kFALSE;
    
    std::vector<int> _TheClusterPixelsList;
    _TheClusterPixelsList.clear();
    _TheClusterPixelsList.push_back(seedIdx);

    if(fDebugHit>1) {
      cout << endl;
      cout << "DHit:: Plane " << fPlane->GetPlaneNumber() << ". Seed pixel = " << fSeed->GetStripIndex() << " at (col,lin) = (" << seedCol << "," << seedRow << ")"; 
      if(fPlane->GetAnalysisMode() == 2) cout << ", charge = " << fSeed->GetPulseHeight() << ", S/N = " << fSeed->GetPulseHeightToNoise();
      cout << ". Initial cluster size  = " << _TheClusterPixelsList.size() << endl;
      cout << endl;
    }

    //SON cut for pixels in case of analog output
    float SON_cut = 3.5;
    
    //Define a region of interest (ROI) to look for pixels to add to the seed pixel
    int delta_col_ROI = 50;
    int delta_row_ROI = delta_col_ROI;
    std::vector<int> _ROIlist;
    _ROIlist.clear();
    for(int icol=0;icol<delta_col_ROI+1;icol++) {
      int testCol = seedCol + (-(delta_col_ROI/2) + icol);
      if(testCol < 0 || testCol > fPlane->GetStripsNu()) continue;
      
      for(int irow=0;irow<delta_row_ROI+1;irow++) {
	int testRow = seedRow + (-(delta_row_ROI/2) + irow);
	if(testRow < 0 || testRow > fPlane->GetStripsNv()) continue;
	
	if(testRow == seedRow && testCol == seedRow) continue;
	
	int testIdx = testCol + testRow*fPlane->GetStripsNu();
	aNeighbour = fPlane->GetStrip(testIdx);
	
	if(aNeighbour == NULL) {
	  cout << "WARNING:: Index " << testIdx << " is out of bounds!!!" << endl;
	  continue;
	}
	if(aNeighbour->Found()) continue;
	//Only consider pixels with a minimum S/N in case of analog output sensors
	if(fPlane->GetAnalysisMode() == 2 && aNeighbour->GetPulseHeightToNoise() < SON_cut) continue;
	
	_ROIlist.push_back(testIdx);
      }
    }

    if(fDebugHit>1) {
      cout << "DHit:: Plane " << fPlane->GetPlaneNumber() << ". Added " << _ROIlist.size() << " pixel to the ROI list."
	   << endl;
      cout << endl;
    }

    //Start iterations to include pixels neighbours to already included pixels in cluster
    //Stop when no more neighbours are found
    int iteration_counter = 0;
    std::vector<int> _FiredPixelsList;
    do {
      _FiredPixelsList.clear();
     
      //Check for pixels neighbour to already included pixels 
      for(Int_t iPix = 0; iPix < int(_ROIlist.size()); iPix++) { // being loop over ROI pixels
	aNeighbour = fPlane->GetStrip(_ROIlist[iPix]);
	
	int testIdx = aNeighbour->GetStripIndex();
	int testCol = testIdx%fPlane->GetStripsNu();
        int testRow = testIdx/fPlane->GetStripsNu();
	
	//Now check if test pixel is neighbour to any pixel already included in cluster
	for(int iPixClus=0; iPixClus < int(_TheClusterPixelsList.size()); iPixClus++) { // begin loop over pixel already included to cluster
	  if(_ROIlist[iPix] == _TheClusterPixelsList[iPixClus]) continue;
	  aPixel = fPlane->GetStrip(_TheClusterPixelsList[iPixClus]);
	
	  int clusIdx = aPixel->GetStripIndex();
	  int clusCol = clusIdx%fPlane->GetStripsNu();
          int clusRow = clusIdx/fPlane->GetStripsNu();
	  
	  int deltaRow = abs(clusRow - testRow);
          int deltaCol = abs(clusCol - testCol);
	  if((deltaRow ==  1 && deltaCol ==  0) || //pixel to the right/left 
	     (deltaRow ==  0 && deltaCol ==  1) || //pixel at the top/bottom
	     (deltaRow ==  1 && deltaCol ==  1)    //pixel at diagonal
            ) {
	    
	    //Check if this pixel has been already added to the list of fired pixels
	    bool IsNotAlreadyIn = true;
	    for(int iNewPix=0; iNewPix < int(_FiredPixelsList.size()); iNewPix++) {
	      if(_ROIlist[iPix] == _FiredPixelsList[iNewPix]) {
		IsNotAlreadyIn = false;
		break;
	      }
	    }
	    if(IsNotAlreadyIn) _FiredPixelsList.push_back(_ROIlist[iPix]);
	  }
        } // end loop over pixel already included to cluster
      } //end loop over ROI pixels

      //Now add the found pixels to the cluster and remove them from the ROI list
      if(fDebugHit>1) cout << "DHit:: Plane " << fPlane->GetPlaneNumber() << ". Found " << _FiredPixelsList.size() << " new pixels around seed pixel at iteration " << iteration_counter+1 << "!!!" << endl;
      for(int iNewPix=0; iNewPix < int(_FiredPixelsList.size()); iNewPix++) { //begin loop over new cluster pixels
	_TheClusterPixelsList.push_back(_FiredPixelsList[iNewPix]);
	
	if(fDebugHit>1) {
	  aPixel = fPlane->GetStrip(_FiredPixelsList[iNewPix]);
	  int clusIdx = aPixel->GetStripIndex();
	  int clusCol = clusIdx%fPlane->GetStripsNu();
          int clusRow = clusIdx/fPlane->GetStripsNu();
	  cout << "DHit:: Plane " << fPlane->GetPlaneNumber() << ". Adding pixel " <<  _FiredPixelsList[iNewPix] << " at (col,lin) = (" << clusCol << "," <<  clusRow << ")";
	  if(fPlane->GetAnalysisMode() == 2) cout << ", charge = " << aPixel->GetPulseHeight() << ", S/N = " << aPixel->GetPulseHeightToNoise();
	  cout << endl;
	}
	
	for(Int_t iPix = 0; iPix < int(_ROIlist.size()); iPix++) { // being loop over ROI pixels
	  if(_FiredPixelsList[iNewPix] == _ROIlist[iPix]) {
	    _ROIlist.erase(_ROIlist.begin() + iPix);
	    break;
	  }
	} //end loop over ROI pixels
	
      } //end loop over new cluster pixels
      if(fDebugHit>1 && int(_FiredPixelsList.size()) > 0) {
	cout << "DHit:: Plane " << fPlane->GetPlaneNumber() << ". Cluster size increased to " << _TheClusterPixelsList.size() << " at iteration " << iteration_counter+1 << endl;
	cout << "DHit:: Plane " << fPlane->GetPlaneNumber() << ". ROI list size reduced  to " << _ROIlist.size()              << " at iteration " << iteration_counter+1 << endl;
      }
      if(fDebugHit>1) cout << endl;

      iteration_counter++;
    }
    while(int(_FiredPixelsList.size()) > 0);
    _FiredPixelsList.clear();
    _ROIlist.clear();
    
    if(fDebugHit>1) {
      cout << "DHit:: Plane " << fPlane->GetPlaneNumber() << ". Final cluster size is " << _TheClusterPixelsList.size() << " after " << iteration_counter << " iterations!" << endl;
      cout << endl;
    }

    
    //Excluding repeated pixel. Only keeping the ones with the lowest time-stamp:
    std::vector<int> _TheClusterPixelsList_NotRepeated;
    _TheClusterPixelsList_NotRepeated.clear();
    std::vector<int> _TheClusterPixelsList_Repeated;
    _TheClusterPixelsList_Repeated.clear();
    std::vector<int> _TheClusterPixelsList_All;
    _TheClusterPixelsList_All.clear();
    for(int iPixClus1=0; iPixClus1 < int(_TheClusterPixelsList.size()); iPixClus1++) {
      _TheClusterPixelsList_All.push_back(_TheClusterPixelsList[iPixClus1]);
      int clusIdx = (fPlane->GetStrip(_TheClusterPixelsList[iPixClus1]))->GetStripIndex();
      int theRow  = clusIdx%fPlane->GetStripsNu();
      int theCol  = clusIdx/fPlane->GetStripsNu();

      //Only include in the list the pixels which are not repeated
      bool IsNotRepeated = true;
      for(int iPixClus2=0; iPixClus2 < int(_TheClusterPixelsList_NotRepeated.size()); iPixClus2++) {
	int clusIdx2 = (fPlane->GetStrip(_TheClusterPixelsList[iPixClus2]))->GetStripIndex();
	int theRow2  = clusIdx2%fPlane->GetStripsNu();
	int theCol2  = clusIdx2/fPlane->GetStripsNu();
	
	if(theRow == theRow2 && theCol == theCol2) {
	  if(fDebugHit>1) {
	    cout << "DHit:: Plane " << fPlane->GetPlaneNumber() << ". Pixel " << iPixClus1+1 
	         << " in list at (col,lin) = (" << theCol << "," << theRow << ") is repated, the other pixel is at (col,lin) = (" 
	         << theCol2 << "," << theRow2 << ")"
	         << endl;
	  }
	  //Repeated pixel
	  IsNotRepeated = false;
	  break;
	}
      }
      //If the pixel is not already in list inclue it
      if(IsNotRepeated) _TheClusterPixelsList_NotRepeated.push_back(_TheClusterPixelsList[iPixClus1]);
    }
    
    //Fill list with the removed pixes
    for(int iPixClus1=0; iPixClus1 < int(_TheClusterPixelsList.size()); iPixClus1++) {
      bool IsRepeated = true;
      for(int iPixClus2=0; iPixClus2 < int(_TheClusterPixelsList_NotRepeated.size()); iPixClus2++) {
	if(_TheClusterPixelsList[iPixClus1] == _TheClusterPixelsList_NotRepeated[iPixClus2]) IsRepeated = false;
      }
      if(IsRepeated) _TheClusterPixelsList_Repeated.push_back(_TheClusterPixelsList[iPixClus1]);
    }
    //Declare all the pixels in the repeated list as found
    for(int iPix=0;iPix<int(_TheClusterPixelsList_Repeated.size());iPix++) {
      aNeighbour = fPlane->GetStrip(_TheClusterPixelsList_Repeated[iPix]);
      aNeighbour->SetFound(kTRUE);
    }
    _TheClusterPixelsList_Repeated.clear();
    
    if(_TheClusterPixelsList.size() != _TheClusterPixelsList_NotRepeated.size()
       && fDebugHit>1
      ) {
      cout << endl;
      cout << "DHit:: Plane " << fPlane->GetPlaneNumber() << ". Cluster size previous to removing repeated pixels: " << _TheClusterPixelsList.size() 
           << ". Cluster size after removing repeated pixels: " << _TheClusterPixelsList_NotRepeated.size()
           << ". Removed " << _TheClusterPixelsList.size() - _TheClusterPixelsList_NotRepeated.size() << " repated pixels!!!"
           << endl;
      cout << endl;
    }

    //Now replace the ClusterPixelList by the one with the repeated pixels removed
    _TheClusterPixelsList.clear();
    for(int iPixClus=0; iPixClus < int(_TheClusterPixelsList_NotRepeated.size()); iPixClus++) _TheClusterPixelsList.push_back(_TheClusterPixelsList_NotRepeated[iPixClus]);
    _TheClusterPixelsList_NotRepeated.clear();

    //Calculate the position of the center of gravity:
    double COG_U = 0.0;
    double COG_V = 0.0;
    double TotH  = 0.0;
    for(int iPixClus=0; iPixClus < int(_TheClusterPixelsList.size()); iPixClus++) {
      aNeighbour = fPlane->GetStrip(_TheClusterPixelsList[iPixClus]);
      int clusIdx = aNeighbour->GetStripIndex();
      int clusRow  = clusIdx%fPlane->GetStripsNu();
      int clusCol  = clusIdx/fPlane->GetStripsNu();
      
      double PixelU,PixelV,w,height;
      fPlane->ComputeStripPosition(clusCol,clusRow, PixelU, PixelV, w);
      height  = aNeighbour->GetPulseHeight();
      
      COG_U += PixelU*height;
      COG_V += PixelV*height;
      TotH  += height;
    }
    COG_U /= TotH;
    COG_V /= TotH;

    //Order the pixels depending of analog or digital output
    for(int iii=2;iii<=int(_TheClusterPixelsList.size());iii++) {
      for(int jjj=0;jjj<=int(_TheClusterPixelsList.size())-iii;jjj++) {
	
	if(fPlane->GetAnalysisMode() == 2) {
	  //In case of analog output arrange the pixels according to charge,
	  //from highest (seed pixels) to lowest
	  
	  double charge_jjj   = (fPlane->GetStrip(_TheClusterPixelsList[jjj]))->GetPulseHeight();
	  double charge_jjjp1 = (fPlane->GetStrip(_TheClusterPixelsList[jjj]))->GetPulseHeight();
	  if(charge_jjj < charge_jjjp1) {
	    int aux_idx                  = _TheClusterPixelsList[jjj];
	    _TheClusterPixelsList[jjj]   = _TheClusterPixelsList[jjj+1];
	    _TheClusterPixelsList[jjj+1] = aux_idx;
	  }
	  
	}
	else if(fPlane->GetAnalysisMode() == 3) {
	  //In case of digital output arrange the pixels according to their distance to the 
	  //center of gravitiy, from closest (seed pixels) to farest
	  
	  double PixelU,PixelV,w;
	  int clusIdx, clusCol, clusRow;
	  
	  aNeighbour = fPlane->GetStrip(_TheClusterPixelsList[jjj]);
	  clusIdx = aNeighbour->GetStripIndex();
	  clusRow  = clusIdx%fPlane->GetStripsNu();
	  clusCol  = clusIdx/fPlane->GetStripsNu();
	  fPlane->ComputeStripPosition(clusCol,clusRow, PixelU, PixelV, w);
          double dist_jjj   = sqrt(pow(COG_U - PixelU,2) + pow(COG_V - PixelV,2));
	  
	  aNeighbour = fPlane->GetStrip(_TheClusterPixelsList[jjj+1]);
	  clusIdx = aNeighbour->GetStripIndex();
	  clusRow  = clusIdx%fPlane->GetStripsNu();
	  clusCol  = clusIdx/fPlane->GetStripsNu();
	  fPlane->ComputeStripPosition(clusCol,clusRow, PixelU, PixelV, w);
          double dist_jjjp1 = sqrt(pow(COG_U - PixelU,2) + pow(COG_V - PixelV,2));
	  
	  if(dist_jjj > dist_jjjp1) {
	    int aux_idx                  = _TheClusterPixelsList[jjj];
	    _TheClusterPixelsList[jjj]   = _TheClusterPixelsList[jjj+1];
	    _TheClusterPixelsList[jjj+1] = aux_idx;
	  }
	}
        
      }
    }

    std::vector<DPixel*> *aList = fPlane->GetListOfPixels();
    int diff = 0 - aList->at(0)->GetPixelIndex();
    //cout << "i = " << 0 << ", index = " << aList->at(0)->GetPixelIndex() << ", diff = " << diff << endl;
    //Hard-coded limit in the number of pixel in a cluster
    //This is a way to identify cluster due to low-momentum heavy ions from fragmentation
    //This should avoid crash of the code when such an event happends. This happends mainly 
    //for high fluxes (high trigger rates > 20kHz).
    //int TheNpixelsLimit = 100;
    int TheNpixelsLimit = MaxClusterSize;
    if(int(_TheClusterPixelsList.size()) >= TheNpixelsLimit) {
      IsBigCluster = true;
      cout << endl;
      double TheCol,TheLin;
      fPlane->ComputeStripPosition_UVToColRow(COG_U,COG_V,TheCol,TheLin);
      cout << "  DHit: Analyse method found too many pixels in cluster, N-pixels = " << _TheClusterPixelsList.size() << " (Limit is set to config file value MaxNStrips " << TheNpixelsLimit 
	   << "), for plane " << fPlane->GetPlaneNumber() << ". Seed pixel location is (col,lin) = (" << TheCol << "," << TheLin << ")" << endl;
      cout << "  DHit: It is likely a cluster of low-momentum heavy ion from fragmentation. Excluding pixels from further analysis" << endl;

      for(int iPix=0;iPix<int(_TheClusterPixelsList_All.size());iPix++) {
	aNeighbour = fPlane->GetStrip(_TheClusterPixelsList_All[iPix]);
	  
	if( fDebugHit>1) {
	  int clusIdx = aNeighbour->GetStripIndex();
	  int clusRow  = clusIdx%fPlane->GetStripsNu();
	  int clusCol  = clusIdx/fPlane->GetStripsNu();
	
	  cout << "  DHit:: idx = " << _TheClusterPixelsList_All[iPix] << ", (col,lin) = ("
	       << clusCol << "," << clusRow << ")"
	       << endl;
	}
	aNeighbour->SetFound(kTRUE);
	aList->at(_TheClusterPixelsList_All[iPix] + diff)->SetFound(kTRUE);
      }
      _TheClusterPixelsList.clear();
      _TheClusterPixelsList_All.clear();
      
      return false;
    }
    _TheClusterPixelsList_All.clear();
    
    
    //Now fillup  the cluster
    tClusterAreaNoise = 0.;
    fClusterAreaPulseSum      = 0.0;
    fClusterPulseSum          = 0.0;
    fClusterNoiseAverage      = 0.0;
    fStripsInClusterFound     = 0;
    
    fStripIndexArray[0]       = 0;
    fStripIndex[0]            = 0.0;
    fStripPulseHeight[0]      = 0.0;
    fStripNoise[0]            = 0.0;
    fStripDistanceU[0]        = 0.0;
    fStripDistanceV[0]        = 0.0;
    for(int iPix=0;iPix<int(_TheClusterPixelsList.size());iPix++) { //begin loop over cluster pixels
      aNeighbour = fPlane->GetStrip(_TheClusterPixelsList[iPix]);
      aNeighbour->SetFound(kTRUE);
      aList->at(_TheClusterPixelsList[iPix] + diff)->SetFound(kTRUE);
      
      if(iPix >= fTableSize) continue;

      fClusterAreaPulseSum     += aNeighbour->GetPulseHeight();
      tClusterAreaNoise        +=(aNeighbour->GetNoise())*(aNeighbour->GetNoise());
      
      // clear
      fStripIndexArray[iPix]  = iPix;
      fStripIndex[iPix]       = aNeighbour->GetStripIndex();
      fStripPulseHeight[iPix] = aNeighbour->GetPulseHeight();
      fStripNoise[iPix]       = aNeighbour->GetNoise();
      fStripDistanceU[iPix]   = fSeed->DistanceU(aNeighbour->GetPosition());
      fStripDistanceV[iPix]   = fSeed->DistanceV(aNeighbour->GetPosition());
      // check whether strip has sufficient pulsheight-to-noise ratio
      // and whether it is inside the geometrical cluster limits.
      
      fStripsInClusterFound++;
      // increment counter of continous adjacent strips
      if( fDebugHit>1) printf("  neighbour %d at index %d added as %dth pixel with noise %.1f and pulseheight %.1f!\n",
	                      iPix,
			      aNeighbour->GetStripIndex(),
			      fStripsInClusterFound-1,
			      fStripNoise[iPix],
			      fStripPulseHeight[iPix]);
      
      fClusterNoiseAverage   += (aNeighbour->GetNoise())*(aNeighbour->GetNoise());
      fClusterPulseSum       += aNeighbour->GetPulseHeight();
      
    } // end loop over cluster pixels
    _TheClusterPixelsList.clear();
    
  } // end if pixel analog or binary readout


  //=============
  // order pixels
  //=============
  if(fPlane->GetAnalysisMode() < 2){ // skip for binary pixel output, JB 2013/11/05
    DGlobalTools aTool; // JB 2011/07/18
    aTool.OrderIndexDes(&fStripIndexArray[0], &fStripPulseHeight[0], tStripIndex-1);
    if( fDebugHit>1) {
      printf("DHit: Analyse after %d strips ordering by pulseheight:\n", fStripsInClusterFound);
      for( Int_t iStripIndex=0; iStripIndex < fStripsInClusterFound; iStripIndex++ ) {
        printf("    neighbour %d/%d/%d at strip index %d found with noise %.1f and pulseheight %.1f!\n", iStripIndex, fStripIndex[iStripIndex], fStripIndexArray[iStripIndex], fSeed->GetNeighbour(fStripIndexArray[iStripIndex])->GetStripIndex(), fStripNoise[fStripIndexArray[iStripIndex]], fStripPulseHeight[fStripIndexArray[iStripIndex]]); 
      }
    }
  } // end skip for binary pixel output

  //==============
  // Additional properties
  //==============  
  // The number of strips in the cluster and the cluster charge sum are now known
  // Depending wether the noise is calculated or not, 
  //  the SN ratio is a real ratio or simply the charge
  // JB 2010/10/05 and 2010/10/15 to separately compute Cluster and Area values
  
  tClusterAreaNoise  = sqrt(tClusterAreaNoise);
  
  if(fClusterNoiseAverage > 0.5) fClusterSignalToNoise  = fClusterPulseSum / sqrt(fClusterNoiseAverage);
  else                           fClusterSignalToNoise  = fClusterPulseSum;
  
  if (tClusterAreaNoise > 0.5)   fSNneighbour           = fClusterAreaPulseSum / tClusterAreaNoise;
  else                           fSNneighbour           = fClusterAreaPulseSum;
  
  if(fDebugHit>1) printf("  DHit:Analyse  potential hit has : PulseSum=%f, areaPulseSum=%f, noise=%f, areaNoise=%f, clusterSignalToNoise=%f, areaSignalToNoise=%f <?> cut %f, # pixels=%d within? [%d, %d]\n", 
                         fClusterPulseSum, 
			 fClusterAreaPulseSum, 
			 fClusterNoiseAverage, 
			 tClusterAreaNoise, 
			 fClusterSignalToNoise, 
			 fSNneighbour, 
			 fCut->GetNeighbourPulseHeightToNoise(), 
			 fStripsInClusterFound,
			 fCut->GetStripsInClusterMin(),
			 fCut->GetStripsInClusterMax());
  

  //==============
  // Select or not the hit
  //==============  

  if(fSNneighbour >= fCut->GetNeighbourPulseHeightToNoise()){
    valid = kTRUE;
  
    if(fPlane->GetAnalysisMode() < 2) {
      if(not(fStripsInClusterFound >= fCut->GetStripsInClusterMin() && fStripsInClusterFound <= fCut->GetStripsInClusterMax())) return kFALSE;
    }
    
    //===============
    // Compute hit position
    //===============
    
    // start to  calculate the exact hit position, which is in first order the seed strip position  
    (*fPositionHit)(0)    = (fSeed->GetPosition())(0);
    (*fPositionHit)(1)    = (fSeed->GetPosition())(1);
    (*fPositionHit)(2)    = (fSeed->GetPosition())(2);
    
    // This is the container (3D distance) for the correction to be computed with different algorithm
    DR3 tCorrection ; tCorrection.SetValue(0.,0.,0.);
    DR3 tCorTemp ;
    
    
    //===============
    //---- for planes using STRIPS:
    //
    // ref plane condition removed by JB, 2007 June
    //if(fPlane->GetStatus()<3 && fPlane->GetAnalysisMode()<2){ // select reference planes only
    if(fPlane->GetAnalysisMode() < 2) { // select STRIP planes
      
      fPhSeed       =  fSeed->GetPulseHeight();
      fPosSeed      =  fSeedU;              
      fPhRofSeed    =  fSeed->GetNeighbour(1)->GetPulseHeight();
      fPhLofSeed    =  fSeed->GetNeighbour(2)->GetPulseHeight();
      fPosRofSeed   = (fSeed->GetNeighbour(1)->GetPosition())(0);
      fPosLofSeed   = (fSeed->GetNeighbour(2)->GetPosition())(0);
      fIndexRofSeed =  fSeed->GetNeighbour(1)->GetStripIndex();
      fIndexLofSeed =  fSeed->GetNeighbour(2)->GetStripIndex();
      fIndexSeed    =  fSeed->GetStripIndex();
      
      Float_t tSeedSign = fPhSeed/fabs(fPhSeed);
      
      if(tSeedSign*fPhLofSeed > tSeedSign*fPhRofSeed ) {
        fPhLeft     = fPhLofSeed;
        fPhRight    = fPhSeed;
        fPosLeft    = fPosLofSeed;
        fPosRight   = fPosSeed;
        fIndexLeft  = fIndexLofSeed; 
        fIndexRight = fIndexSeed;
      }
      else {
        fPhLeft     = fPhSeed;
        fPhRight    = fPhRofSeed;
        fPosLeft    = fPosSeed;
        fPosRight   = fPosRofSeed;
        fIndexLeft  = fIndexSeed;
        fIndexRight = fIndexRofSeed;
      }
      
      
      // Charge Fraction algorithm optimizes hit position, comprises other known algorithms like
      // center of gravitiy           : weight_k = charge fraction
      // 2 strip linear eta algorithm : center of gravity with 2 strips in cluster
      // 2 strip non-linear eta       : 2 strips, weights calculated from charge fraction distribution on strips
      
      Float_t tChargeFraction, aWeight ;
      
      TH1F   *tChargeFractionDensityData;
      Int_t   tDensityBinsN, tBin;
      Float_t tLL, tUL;
      
      Int_t   tClusterType; 
      Int_t   tHk;
      
      tClusterType = 2;
      Float_t tDistU;
      if(fPositionAlgorithm==1) {
        if(fPhLeft < fPhRight) {
          tChargeFraction  = fPhLeft / (fPhLeft+fPhRight); // seed is on right
          aWeight          = tChargeFraction;              // this is C.o.G. Method
          tDistU           = fPosRight-fPosLeft;  
        }
        else {
          tChargeFraction  = fPhRight / (fPhLeft+fPhRight); // seed is right
          aWeight          = tChargeFraction;               // this is C.o.G. Method
          tDistU           = -fPosRight+fPosLeft;  
        }
        tCorTemp.SetValue(tDistU,(Float_t)0.,(Float_t)0.) ;
        tCorTemp *= aWeight ;
        
        tCorrection += tCorTemp ;
        
        // assign the digital strip position
        *fPositionHitCG    = *fPositionHit     ;
        // and now change u-value with CoG correction
        (*fPositionHitCG) -= tCorrection       ; 
        (*fPositionHit)    = (*fPositionHitCG) ; // needed if CoG is used
      }
      
      if ( tDigital ){
        if(fDebugHit>1) {
          printf("DHit.cxx:evaluateHitCluster = Digital response only set (forgot about etaXXX.root file?) for plane %d \n",fPlane->GetPlaneNumber());
        }
        
      }
      
      else if(fPositionAlgorithm == 2) {
        
        tCorrection.SetValue(0.,0.,0.);
        // treat seed and strip with next highest pulse (2 strips):
        if(fPhLeft < fPhRight){
          tChargeFraction  = fPhLeft / (fPhLeft+fPhRight); // seed is right
          tDistU           = fPosRight-fPosLeft;  
          
        }else {
          tChargeFraction  = fPhRight / (fPhLeft+fPhRight); // seed is right
          tDistU           = -fPosRight+fPosLeft;  
        }
        tHk = 1 + (tClusterType-2)*2; // so tHk = 1??
        tChargeFractionDensityData = fPlane->GetChargeFractionDensity(tHk);
        tDensityBinsN = tChargeFractionDensityData->GetNbinsX();
        tLL           = tChargeFractionDensityData->GetXaxis()->GetXmin();
        tUL           = tChargeFractionDensityData->GetXaxis()->GetXmax();
        tBin = (Int_t)( (tChargeFraction-tLL) * tDensityBinsN/(tUL-tLL) );
        
        if (tBin>tDensityBinsN) tBin = tDensityBinsN - 50;
        if (tBin<0) tBin = 50;
        
        aWeight = tChargeFractionDensityData->GetBinContent(tBin);       
        tCorTemp.SetValue(tDistU,(Float_t)0.,(Float_t)0.) ;
        tCorTemp *= aWeight ;
        
        tCorrection += tCorTemp ;
        (*fPositionHit) -= tCorrection;
        
      } //end of if PositionAlgorithm ==2 
    } // end select STRIP planes
    
    
    //===============
    //--- for PIXEL planes
    //
    else {
      //==================
      //Center of gravity with all the neighbours
      // pay attention that pixels are ordered geometricaly from one corner to the other in the fStripDistanceU/V arrays
      tCorrection.SetValue( 0., 0., 0.);
      Float_t tClusterPulseSum = 0.;
      for(Int_t iStripIndex=0; iStripIndex < fStripsInClusterFound; iStripIndex++) {
        tCorTemp.SetValue(((fSeed->GetPosition())(0)-fStripDistanceU[iStripIndex])*fStripPulseHeight[iStripIndex],
                          ((fSeed->GetPosition())(1)-fStripDistanceV[iStripIndex])*fStripPulseHeight[iStripIndex],
                          0);
        tCorrection      += tCorTemp;
        tClusterPulseSum += fStripPulseHeight[iStripIndex];
      }      
      *fPositionHitCG = tCorrection/tClusterPulseSum;

      //==================
      //Center of gravity restricted to a 3x3 cluster      
      tCorrection.SetValue( 0., 0., 0.);
      Float_t tCluster33PulseSum = 0.;
      for(Int_t iStripIndex=0; iStripIndex < fStripsInClusterFound; iStripIndex++ ) {
        if(fabs(fStripDistanceU[iStripIndex]) <= fSeed->GetPlane()->GetStripPitch()(0) && 
           fabs(fStripDistanceV[iStripIndex]) <= fSeed->GetPlane()->GetStripPitch()(1)) {
	  // check neighbours is at most at 1 pitch from seed
          tCorTemp.SetValue(((fSeed->GetPosition())(0)-fStripDistanceU[iStripIndex])*fStripPulseHeight[iStripIndex],
                            ((fSeed->GetPosition())(1)-fStripDistanceV[iStripIndex])*fStripPulseHeight[iStripIndex],
                            0);
          tCorrection += tCorTemp;
          tCluster33PulseSum += fStripPulseHeight[iStripIndex];
          //printf("DHit::Analyse plane %d, hit %2d, strip %2d, corr(%.2f, %.2f)=dist(%.2f, %.2f)*%.0f/%.0f\n", fPlane->GetPlaneNumber(), fHitNumber, iStripIndex, tCorrection(0), tCorrection(1), fStripDistanceU[iStripIndex], fStripDistanceV[iStripIndex], fStripPulseHeight[iStripIndex], tCluster33PulseSum);
        }
      }
      *fPositionHitCG33 = tCorrection/tCluster33PulseSum;
      
      //==================
      //Center of gravity restricted 2x2 neighbours with the highest charge, JB Nov 2007
      // 2x2 includes the seed
      // The index of the neighbours with the highest charges are ordered in fStripIndexArray
      tCorrection.SetValue( 0., 0., 0.);
      Float_t tCluster22PulseSum = 0.;
      Int_t tOrderedIndex;
      for(Int_t iStripIndex=0; iStripIndex < 4; iStripIndex++ ) {
        tOrderedIndex = fStripIndexArray[iStripIndex];
        tCorTemp.SetValue((fSeed->GetPosition()(0)-fStripDistanceU[tOrderedIndex])*fStripPulseHeight[tOrderedIndex],
                          (fSeed->GetPosition()(1)-fStripDistanceV[tOrderedIndex])*fStripPulseHeight[tOrderedIndex],
                          0);
        tCorrection += tCorTemp;
        tCluster22PulseSum += fStripPulseHeight[tOrderedIndex];
      }
      *fPositionHitCG22 = tCorrection/tCluster22PulseSum;

      //==================
      //Eta algorithm 3x3
      // correct the 3x3 center of gravity (tCorrection) using the integral histogram of the charge density
      // the eta position is the Y value of the bin with X value = tCorrection
      // if the tCorrection from CoG is out of the X axis range, do not correct
      if(tDigital) { // check info file was there and Eta is available
        if(fDebugHit>1) printf("  DHit.cxx:evaluateHitCluster = Digital or CoG response only set for plane %d (forgot about etaXXX.root file?)\n",fPlane->GetPlaneNumber());
      }
      else {
        TH1  *tEtaIntU = fPlane->GetEtaIntU();
        TH1  *tEtaIntV = fPlane->GetEtaIntV();
        Int_t iBinU    = tEtaIntU->FindBin( (*fPositionHitCG33-*fPositionHit)(0) );
        Int_t iBinV    = tEtaIntV->FindBin( (*fPositionHitCG33-*fPositionHit)(1) );
        Double_t corrU=0., corrV=0.;
        
        // correct wrt CoG if possible
        if( 1<=iBinU && iBinU<=tEtaIntU->GetNbinsX() ) corrU = tEtaIntU->GetBinContent( iBinU);
        if( 1<=iBinV && iBinV<=tEtaIntV->GetNbinsX() ) corrV = tEtaIntV->GetBinContent( iBinV);
        tCorrection.SetValue( corrU, corrV, 0.);
        
        *fPositionHitEta = *fPositionHit + tCorrection;
      }

      //==================
      //Eta algorithm 2x2
      // correct the 2x2 center of gravity (tCorrection) using the integral histogram of the charge density
      // the eta position is the Y value of the bin with X value = tCorrection
      // if the tCorrection from CoG is out of the X axis range, do not correct
      
      if(tDigital) { // check info file was there and Eta is available
        if(fDebugHit>1) printf("  DHit.cxx:evaluateHitCluster = Digital or CoG response only set for plane %d (forgot about etaXXX.root file?)\n",fPlane->GetPlaneNumber());
      }
      else {
        TH1  *tEtaIntU2 = fPlane->GetEtaIntU2();
        TH1  *tEtaIntV2 = fPlane->GetEtaIntV2();
        Int_t iBinU    = tEtaIntU2->FindBin( (*fPositionHitCG22-*fPositionHit)(0) );
        Int_t iBinV    = tEtaIntV2->FindBin( (*fPositionHitCG22-*fPositionHit)(1) );
        Double_t corrU=0., corrV=0.;
        
        // correct wrt CoG if possible
        if( 1<=iBinU && iBinU<=tEtaIntU2->GetNbinsX() ) corrU = tEtaIntU2->GetBinContent( iBinU);
        if( 1<=iBinV && iBinV<=tEtaIntV2->GetNbinsX() ) corrV = tEtaIntV2->GetBinContent( iBinV);
        tCorrection.SetValue( corrU, corrV, 0.);
        
        *fPositionHitEta22 = *fPositionHit + tCorrection;
      }
      
      //==================
      //Analog head-tail algorithm
      // Algorithm used for large incident angle when seed contains little info on position
      // Relies on the extreme pixels on the left and right
      // tCorrection.SetValue( (fStripDistanceU[iLeft]+fStripDistanceU[iRight])/2., , 0.);
      // *fPositionhitAHT = *fPositionHit - tCorrection;

      //==================
      // Now choose which position is stored
      if(fPositionAlgorithm == 1)       *fPositionHit = *fPositionHitCG;   // Center of Gravity
      else if(fPositionAlgorithm == 11) *fPositionHit = *fPositionHitCG33; // Center of gravity on 3x3
      else if(fPositionAlgorithm == 12) *fPositionHit = *fPositionHitCG22; // Center of gravity on 2x2
      else if(fPositionAlgorithm == 2)  *fPositionHit = *fPositionHitEta; // Eta corrected from CoG 3x3
      else if(fPositionAlgorithm == 22) *fPositionHit = *fPositionHitEta22; // Eta corrected from CoG 2x2
      else {
        printf("-*-*- WARNING: algorithm %d for position unknown, taking digital position\n", fPositionAlgorithm);
      }

    } // end of PIXEL planes
    
    if(fDebugHit>1) printf("        hit selected with %d pixels\n", fStripsInClusterFound);
    
  } // end if hit selected

  // ========================
  else{ // hit rejected
    valid = kFALSE;
    if(fDebugHit>1) printf("        hit rejected, releasing %d pixels\n", fStripsInClusterFound-1);
    
    // Release the neighbouring Strips associated to the seed
    // only the tested seed (Index=0) is not released
    // JB 2012/08/18
    for( Int_t iStripIndex=1; iStripIndex < fStripsInClusterFound; iStripIndex++ ) { // loop over associated strips
      if(fDebugHit>2) printf("  DHit.cxx:Analyse    releasing neighbour strip %d at index %d\n", iStripIndex, fStripIndex[iStripIndex]);
      fSeed->GetNeighbour(iStripIndex)->SetFound(kFALSE);
    } // end loop over associated strips

  }

  return valid;
  
}
//______________________________________________________________________________
//  
Bool_t DHit::Analyse( Int_t aPixelIndexInList, std::vector<DPixel*> *aListOfPixels)
{
  // Valid for sparse data acquisition
  //
  // Construct a hit (or cluster) from a seed pixel and the list of all fired pixels:
  // 1) try to associate all neighbouring strips that pass selection cut
  // 2) check the build hit passes all selections
  // 3) order the array fStripIndexArray of neighbours by decreasing pulseheight
  // 4) compute cluster charge, noise and position
  //
  // Note that the behavior of this method is affected by the AnalysisMode chosen.
  //
  // Return a boolean: kTRUE if hit is selected, kFalse otherwise
  //
  // Constructed from the original Analyse( DStrip*) by JB, 2009 April 29
  // Last modified, JB 2009/05/12
  // Last Modified, RDM 2009/08/05 for Mimosa25
  // Last Modified, JB 2009/08/21 for binary output 
  // Last Modified, JB 2009/08/30 for cut on #strips in cluster
  // Last Modified, JB 2009/09/01 for CoG 2x2
  // Last Modified, YV 2009/11/27 for estimation of noise
  // Last Modified, JB 2010/10/05 automatic management of with or without noise
  // Last Modified, JB 2010/10/15 correction of previous management for Area S/N
  // Last Modified, MG 2010/06/28 corrected CGs computation
  // Last Modified, MB 2010/11/12 include new counting on #pixels with SN>2
  // Last Modified, JB 2011/07/21 for binary ouput, allow to reallocate seed for large clusters
  // Last Modified, JB 2011/11/07 test if pixels are considered twice as neighbours
  // Last Modified, JB+SS 2013/06/23 manage timestamp cut for multiframe
  // Last Modified, JB 2013/11/08 store initial seed information
  // Last Modified, VR 2014/07/12 Condition to look for a new seed change : cogRow type Int_t -> Double_t
  // Modified: JB 2015/05/26 to introduce timestamps and TimeLimit
  
  fFound      = kFALSE; // JB 2009/05/22
  fPSeed      = aListOfPixels->at( aPixelIndexInList);
  
  DPixel   *aNeighbour;
  DPixel   *aPixel=NULL;
  Bool_t    pixelTwice = kFALSE; // test if a pixel is present twice in the neighbour list
  Bool_t    valid    = kFALSE; // default return value
  Bool_t    tDigital = fPlane->GetStripResponseSetting();
  Int_t     tStripIndex;
  Int_t     tStripsInClusterPossible = 0; // remove a warning 
  //Int_t     tPixelIndexList[500];

  Int_t     tTimeLimit = fPlane->GetTimeLimit(); // 2015/05/26
  
  fPositionHitCG->Zero();           // clear the position
  fPositionHitEta->Zero();
  fPositionHitEta22->Zero(); // JB 2010/12/8
  fPositionHit->Zero();
  fClusterSignalToNoise   = 0.0;

  fSeedU      = fPSeed->GetPosition()(0);
  fSeedV      = fPSeed->GetPosition()(1); 
  fIndexSeed  = fPSeed->GetPixelIndex();
  
  Int_t seedRow = fPSeed->GetPixelLine(); 
  Int_t seedCol = fPSeed->GetPixelColumn(); 
  Double_t cogRow = fPSeed->GetPixelLine(); //VR 2014/07/12
  Double_t cogCol = fPSeed->GetPixelColumn(); 
  Int_t rowDiffMax = (Int_t)((*fClusterLimit)(0)/fPSeed->GetSize()(0))/2;
  Int_t colDiffMax = (Int_t)((*fClusterLimit)(1)/fPSeed->GetSize()(1))/2;
  
  Int_t iSeed = 0;

  // here, the neighbourhood of strips is assumed to be ordered, 
  // at index 0 is the seed, to higher indices
  // the neighbourstrips are further away from the seed.

  Float_t tClusterAreaNoise = 0.;
  fClusterAreaPulseSum      = 0.; // area means the cluster except the seed
  fClusterPulseSum        = fPSeed->GetPulseHeight(); 
  fClusterNoiseAverage    = (fPSeed->GetNoise())*(fPSeed->GetNoise());  // => so it's only a seed  strip in the beginning
  fStripsInClusterFound   = 1;                       
  fPSeed->SetFound(kTRUE);                            
  
  if (fStripsInClusterDemanded >0) {        // exact number demanded
    tStripsInClusterPossible = fStripsInClusterDemanded;
  }
  else if (fStripsInClusterDemanded == 0) { // number from config file, JB 2009/08/21
    tStripsInClusterPossible = fCut->GetStripsInClusterArea(); //(Int_t)pow( fClusterLimit->Length()/(fPSeed->GetSize())(0)*2+1, 2), JB 2013/08/29 to match really a rectangle
  }
  else { // JB 2009/08/21
    printf("DHit::Analyse WARNING limit on the # pixels in cluster %d is UNKNOWN !!\n", fStripsInClusterDemanded );
    return false;
  }

  tPixelIndexList[0]   = aPixelIndexInList;
  fStripIndexArray[0]  = 0;                            // place seed in position 0 of neighbour
  fStripIndex[0]       = fPSeed->GetPixelIndex();      // store seed index, JB 2009/05/12
  fStripPulseHeight[0] = fPSeed->GetPulseHeight();     // store seed pulseheight
  fStripNoise[0]       = fPSeed->GetNoise();           // store seed noise  //YV 27/11/09
  fSNseed              = fPSeed->GetPulseHeightToNoise(); // JB 2013/11/08
  fSeedPulseHeight     = fPSeed->GetPulseHeight(); // Note that seed might change
  fSeedNoise           = fPSeed->GetNoise();
  fStripDistanceU[0] = 0.;                             // distance seed to seed = 0.
  fStripDistanceV[0] = 0.;                             // distance seed to seed = 0.
  fStoNover2 = 0;                                      // MB/12/11/2010 

  //===============
  // associate neighbouring pixels to seed one
  //===============
  if(fDebugHit>1) printf("  DHit:Analyse seed pixel index %d (%d in list, r%d, c%d) (q=%f, time=%d) with possibly %d neighbours\n", fIndexSeed, aPixelIndexInList, fPSeed->GetPixelLine(), fPSeed->GetPixelColumn(), fPSeed->GetPulseHeight(), fPSeed->GetTimestamp(), tStripsInClusterPossible);

  tStripIndex = 1; // start with the first neighbour, in the geometric ordered neighbourhood, avoid 0 because it is the seed itself!
  for (Int_t iPix = 0; iPix < (Int_t)aListOfPixels->size(); iPix++){ // loop over hit pixels
    
    aNeighbour = aListOfPixels->at(iPix);
    // Test if the pixel can be associated to the seed 
    //   inside the geometrical cluster limits
    // and also in the time limit using the timestamp (which are all 0 if unavailable).
    //   JB+SS 2013/06/23
    // Geometrical limit test changed from a single 2D distance test
    //  to two 1D distance test.
    //  to JB 2013/08/29 to match really a rectangle
    if ( !aNeighbour->Found() 
        //&& fPSeed->Distance( *aNeighbour) < fClusterLimit->Length()
        && fabs(fPSeed->DistanceU(aNeighbour->GetPosition())) <= (*fClusterLimit)(0)
        && fabs(fPSeed->DistanceV(aNeighbour->GetPosition())) <= (*fClusterLimit)(1)
        //&& fPSeed->GetTimestamp() == aNeighbour->GetTimestamp()
        && abs(fPSeed->GetTimestamp()-aNeighbour->GetTimestamp())<=tTimeLimit
        ) 
    { // if neighbour pixel within limits
      
      // Additional test to avoid counting the same pixel twice
      // JB 2011/11/07
      pixelTwice = kFALSE;
      for ( Int_t jPix=0; jPix<fStripsInClusterFound; jPix++) { // loop on currently found pixels
        aPixel = aListOfPixels->at( tPixelIndexList[ jPix] );
	//if(aNeighbour->GetPixelIndex() == aPixel->GetPixelIndex()) {
	//  pixelTwice = kTRUE;
	//  break;
	//}
	pixelTwice &= aNeighbour->GetPixelIndex() == aPixel->GetPixelIndex();
      } // end loop on currently found pixels
      
      if( !pixelTwice ) { // if pixel is not counted twice
        
        fClusterAreaPulseSum     += aNeighbour->GetPulseHeight();
        tClusterAreaNoise +=(aNeighbour->GetNoise())*(aNeighbour->GetNoise());
        fClusterNoiseAverage   += (aNeighbour->GetNoise())*(aNeighbour->GetNoise());
        fClusterPulseSum       += aNeighbour->GetPulseHeight();
        tPixelIndexList[tStripIndex]   = iPix; // store the index in the original list of pixels
        fStripIndexArray[tStripIndex]  = tStripIndex; // put neighbour index in array (not index of full pixel list)
        fStripIndex[tStripIndex]       = aNeighbour->GetPixelIndex(); // this is the index in the matrix, JB 2009/05/12
        fStripPulseHeight[tStripIndex] = aNeighbour->GetPulseHeight();
        fStripNoise[tStripIndex] = aNeighbour->GetNoise();             //YV 27/11/09
        fStripDistanceU[tStripIndex]   = fPSeed->DistanceU(aNeighbour->GetPosition());
        fStripDistanceV[tStripIndex]   = fPSeed->DistanceV(aNeighbour->GetPosition());
        aNeighbour->SetFound(kTRUE);
        if( fDebugHit>1) printf("  neighbour %d at index %d (r%d, c%d) with pulse %.1f, time %d and distance %.1f(u=%.1f, v=%.1f) (<%.1f(u=%.1f, v=%.1f)) found!\n",
				tStripIndex,
				aNeighbour->GetPixelIndex(),
				aNeighbour->GetPixelLine(),
				aNeighbour->GetPixelColumn(),
				aNeighbour->GetPulseHeight(),
        aNeighbour->GetTimestamp(),
				fPSeed->Distance( *aNeighbour),
				fPSeed->DistanceU(aNeighbour->GetPosition()),
				fPSeed->DistanceV(aNeighbour->GetPosition()),
				fClusterLimit->Length(),
				(*fClusterLimit)(0),
				(*fClusterLimit)(1));
        tStripIndex++;    // increment the strip index counter
        fStripsInClusterFound++;
        
        // For binary ouput, dynamically change the seed pixel if 
        //   there are more than 4 pixels found already,
        //   and the current seed is distant by more than half of the defined cluster limit
        // If so, then choose the seed as the pixel nearest to the center of gravity of all pixels
        // JB 2011/07/21     
        if( fPlane->GetAnalysisMode() == 3) { // if binary readout
          aPixel=NULL;
          Int_t iNewSeed = iSeed;
          
          //VR 2014/07/12 calcul in Double_t
          cogRow = (Double_t(aNeighbour->GetPixelLine())   + cogRow * Double_t(fStripsInClusterFound-1)) / Double_t(fStripsInClusterFound); 
          cogCol = (Double_t(aNeighbour->GetPixelColumn()) + cogCol * Double_t(fStripsInClusterFound-1)) / Double_t(fStripsInClusterFound); 
          
          if(fDebugHit>1) printf( "DHit::Analyze updating cog(%1.2f,%1.2f) / seed(%d,%d)\n", cogRow, cogCol, seedRow, seedCol);
          
          if( fStripsInClusterFound>4 && (abs(cogRow-Double_t(seedRow))>rowDiffMax || abs(cogCol-Double_t(seedCol))>colDiffMax) ) { // if condition to update seed
            
            if(fDebugHit>1) printf( "DHit::Analyze need to update the seed pixel cog(%1.2f,%1.2f) - seed(%d,%d) > max(%d,%d)\n", cogRow, cogCol, seedRow, seedCol, rowDiffMax, colDiffMax);
            for( Int_t jPix=0; jPix<fStripsInClusterFound; jPix++) { // loop on currently found pixels
              aPixel = aListOfPixels->at( tPixelIndexList[ jPix] );
              if( pow((cogRow-Double_t(aPixel->GetPixelLine())),2) + pow((cogCol-Double_t(aPixel->GetPixelColumn())),2) < pow((cogRow-Double_t(seedRow)),2)+pow((cogCol-Double_t(seedCol)),2)) {
                iNewSeed   = jPix;
                fPSeed  = aPixel;
                seedRow = aPixel->GetPixelLine();
                seedCol = aPixel->GetPixelColumn();
                if(fDebugHit) printf( "          new seed pixel %d at index %d (r%d,c%d)\n", iNewSeed, fPSeed->GetPixelIndex(), seedRow, seedCol);
              }
            }  // end loop on currently found pixels
            
          } // end if condition to update seed
          
          if( iNewSeed != iSeed ) { // if seed was indeed changed
            iSeed = iNewSeed;
            iPix  = -1; // will restart the loop over hit pixels
            if(fDebugHit>1) printf( "          restarting the loop over pixels\n");
          } 
          
        } // end if binary readout
        // END of dynamical change of the seed pixel
      	
      } // end if pixel is not counted twice
      
    } // end if neighbour pixel within limits
    
  } // end loop over hit pixels

  if(fDebugHit>2) printf("  DHit:Analyse found %d valid neighbours\n", fStripsInClusterFound);

  
  //=============
  // order pixels
  //=============
  Int_t *nOfneighbours = new Int_t[fStripsInClusterFound];

  // For analog readout
  // re-order pixels from the highest charge to the lowest
  if( fPlane->GetAnalysisMode() == 2 ) { // if analog readout

    DGlobalTools aTool; // JB 2011/07/18
    aTool.OrderIndexDes(&fStripIndexArray[0], &fStripPulseHeight[0], fStripsInClusterFound);
    if( fDebugHit>2) { // display result of pixel ordering, JB 2009/05/12
      cout << "  DHit:Analyse  ordered list of " << fStripsInClusterFound << " pixels in candidate hit" << endl;
      for( Int_t iPix=0; iPix<fStripsInClusterFound ; iPix++) { // loop on pixels in candidate hit
        printf( "    pix %2d, neighbour %7d,  pulse %f\n", iPix, fStripIndexArray[iPix], fStripPulseHeight[fStripIndexArray[iPix]]);
      } // end loop on pixels
    }

  } // end if analog readout


  // For binary readout, update the seed information
  else if( fPlane->GetAnalysisMode() == 3 ) { // if binary readout

    /*
    // Redefine the seed as the one with the highest number of direct neighbours
    // This is only needed when the dynamic seed reallocation is not used
    // JB 2011/07/21
    DPixel *aPixel=NULL, *bPixel=NULL;
    for( Int_t iPix=0; iPix<fStripsInClusterFound; iPix++) { // loop on pixels in candidate hit
      nOfneighbours[ iPix] = 0;
      aPixel = aListOfPixels->at( tPixelIndexList[ iPix] );
      for( Int_t jPix=0; jPix<fStripsInClusterFound; jPix++) {
	if(fDebugHit>3) printf("     comparing pixels %d[%d] and %d[%d] in list of %d pixels\n", tPixelIndexList[ iPix], iPix, tPixelIndexList[ jPix], jPix, fStripsInClusterFound);
	bPixel = aListOfPixels->at( tPixelIndexList[ jPix] );
	if(fDebugHit>3) printf("     comparing pixels %d[%d] (%d-%d) and %d[%d] (%d-%d) in list of %d pixels\n", tPixelIndexList[ iPix], iPix, aPixel->GetPixelLine(), aPixel->GetPixelColumn(), tPixelIndexList[ jPix], jPix, bPixel->GetPixelLine(), bPixel->GetPixelColumn(), fStripsInClusterFound);
	// direct neighbours are defines as
	// abs(difference in line) + abs(difference in column) = 1
	if( (abs(aPixel->GetPixelLine() - bPixel->GetPixelLine()) + 
	     abs(aPixel->GetPixelColumn() - bPixel->GetPixelColumn()) ) == 1 ) {
	  nOfneighbours[ iPix] += 1;
	}
      }
      if(fDebugHit>3) printf("   DHit::Analyse pixel %d has %d direct neighbours <?> max%d\n", iPix, nOfneighbours[ iPix], nOfneighbours[ iSeed]);
      if( nOfneighbours[ iPix] > nOfneighbours[ iSeed] || (nOfneighbours[ iPix] == nOfneighbours[ iSeed] && (aPixel->GetPixelColumn()<aListOfPixels->at( tPixelIndexList[ iPix] )->GetPixelColumn() || aPixel->GetPixelLine()<aListOfPixels->at( tPixelIndexList[ iSeed] )->GetPixelLine()) ) ) { // force lower lefter seed, JB 2009/09/07
	iSeed = iPix;
      }
    } // end loop on pixels
    */
    
    // swap previous info at index 0 with the one at index iSeed
    // so that all arrays have seed info at index 0
    Int_t iTemp;
    Float_t temp;
    iTemp                    = tPixelIndexList[iSeed];
    tPixelIndexList[iSeed]   = tPixelIndexList[0];
    tPixelIndexList[0]       = iTemp;
    iTemp                    = fStripIndexArray[iSeed];
    fStripIndexArray[iSeed]  = fStripIndexArray[0];
    fStripIndexArray[0]      = iTemp;
    iTemp                    = fStripIndex[iSeed];
    fStripIndex[iSeed]       = fStripIndex[0];
    fStripIndex[0]           = iTemp;
    temp                     = fStripPulseHeight[iSeed];
    fStripPulseHeight[iSeed] = fStripPulseHeight[0];
    fStripPulseHeight[0]     = temp;

    // need to update some variables 
    // to take into account the new seed definition
    fPSeed = aListOfPixels->at( tPixelIndexList[ 0] );
    fSeedU      = fPSeed->GetPosition()(0);
    fSeedV      = fPSeed->GetPosition()(1); 
    fIndexSeed  = fPSeed->GetPixelIndex();
    for( Int_t iPix=1; iPix<fStripsInClusterFound ; iPix++) { // loop on pixels in candidate hit
      aNeighbour = aListOfPixels->at( tPixelIndexList[ iPix]);
      fStripDistanceU[iPix]   = fPSeed->DistanceU(aNeighbour->GetPosition());
      fStripDistanceV[iPix]   = fPSeed->DistanceV(aNeighbour->GetPosition());
    }

    if( fDebugHit>2 ) {
      printf("  DHit:Analyse  seed is now pixel %d in list, index %d, with %d direct neighbours\n", tPixelIndexList[ 0], fIndexSeed, nOfneighbours[ iSeed]);
    }
    
  } // end if binary readout

  else {
    printf("DHit::Analyse WARNING analysis mode %d UNKNOWN !!\n", fPlane->GetAnalysisMode() );
    return false;
  }
  
  delete[] nOfneighbours; // remove memory leaks, BH 2013/08/21
  
  //==============
  // additional properties
  //==============  
  // the number of strips in the cluster and the cluster charge sum are now known
  // Depending wether the noise is calculated or not, 
  // the SN ratio is a real ratio or simply the charge
  // JB 2010/10/05 and 2010/10/15 to separately compute Cluster and Area values

  tClusterAreaNoise  = sqrt(tClusterAreaNoise);
  
  if (fClusterNoiseAverage > 0.5) {
    fClusterSignalToNoise  = fClusterPulseSum     / sqrt(fClusterNoiseAverage);
  }
  else {
    fClusterSignalToNoise  = fClusterPulseSum;
  }
  if (tClusterAreaNoise > 0.5) {
    fSNneighbour           = fClusterAreaPulseSum / tClusterAreaNoise;
  }
  else {
    fSNneighbour           = fClusterAreaPulseSum;
  }

  // Number of pixels with 2*pitch of the seed with S/N>2.
  // MB/12/11/2010
  if( fPlane->GetAnalysisMode() <= 2 ) {
    for( Int_t iPix=0; iPix<fStripsInClusterFound; iPix++) { // loop on pixels in candidate hit
      aNeighbour = aListOfPixels->at( tPixelIndexList[iPix] );
      if ( fPSeed->Distance( *aNeighbour) <= 2*(*fStripPitch)(0) && aNeighbour->GetPulseHeightToNoise() >= 2 ){
        fStoNover2++;
      }
      if(fDebugHit>1)  cout<<"  DHit:Analyse  StoNover2  = "<<fStoNover2<<" neighbour "<<iPix<<"   fPSeed->Distance( *aNeighbour)   = "<<fPSeed->Distance( *aNeighbour)<<" SNR "<<aNeighbour->GetPulseHeightToNoise()<<"  aNeighbour->GetFound() = "<<  aNeighbour->Found()<<endl;
    } // end loop on pixels in candidate hit
  }
  
  fTimestamp = fPSeed->GetTimestamp(); // JB 2015/05/25
  
  if(fDebugHit>2) printf("  DHit:Analyse  potential hit has : PulseSum=%f, areaPulseSum=%f, noise=%f, areaNoise=%f, clusterSignalToNoise=%f, areaSignalToNoise=%f <?> cut %f, # pixels=%d within? [%d, %d], stover2=%d, time=%d\n", fClusterPulseSum, fClusterAreaPulseSum, fClusterNoiseAverage, tClusterAreaNoise, fClusterSignalToNoise, fSNneighbour, fCut->GetNeighbourPulseHeightToNoise(), fStripsInClusterFound, fCut->GetStripsInClusterMin(), fCut->GetStripsInClusterMax(), fStoNover2, fTimestamp);


  //===============
  // Compute hit position
  //===============

  // start to  calculate the exact hit position, which is in first order the seed strip position  
  (*fPositionHit)(0)    = (fPSeed->GetPosition())(0);
  (*fPositionHit)(1)    = (fPSeed->GetPosition())(1);
  (*fPositionHit)(2)    = (fPSeed->GetPosition())(2);

  // This is the container (3D distance) for the correction to be computed with different algorithm
  DR3 tCorrection ; tCorrection.SetValue(0.,0.,0.);
  DR3 tCorTemp ;

  //===============
  //--- for PIXEL planes
  //
  if( fPlane->GetAnalysisMode() >= 2 ) { // if pixel mode

    // Cut on the charge or on the charge/noise (See above)
    // Cut on # pixels in hit added, JB 2009/08/30

    if( fSNneighbour >= fCut->GetNeighbourPulseHeightToNoise() 
	&& fCut->GetStripsInClusterMin() <= fStripsInClusterFound
	&& fStripsInClusterFound <= fCut->GetStripsInClusterMax()
       //&& fStoNover2 >=4 // MB 2010/11/12
      ){
      valid = kTRUE;
    }else{
      valid = kFALSE;
    }

    //RDM050809 for M25 begin
//     Float_t cut1[2];
//     Float_t cut2[2];
//     if(fPlane->GetPlaneNumber()==1)     {cut1[0]=0.   ;cut1[1]=-2100.;cut2[0]=0.;cut2[1]=-2250.;}
//     else if(fPlane->GetPlaneNumber()==2){cut1[0]=2000.;cut1[1]=0.    ;cut2[0]=0.;cut2[1]=-2350.;}
//     else if(fPlane->GetPlaneNumber()==3){cut1[0]= 450.;cut1[1]=-1800.;cut2[0]=0.;cut2[1]=-2350;}
//     else if(fPlane->GetPlaneNumber()==4){cut1[0]=1700.;cut1[1]= -500.;cut2[0]=0.;cut2[1]=-2250;}

//     //for M24
//     if(fPlane->GetPlaneNumber()==1)     {cut1[0]=1000.;cut1[1]=-1600.;cut2[0]=1800.;cut2[1]=-2250.;}
//     else if(fPlane->GetPlaneNumber()==2){cut1[0]=2200.;cut1[1]=-600. ;cut2[0]=1800.;cut2[1]=-2350.;}
//     else if(fPlane->GetPlaneNumber()==3){cut1[0]=1000.;cut1[1]=-1500.;cut2[0]=1800.;cut2[1]=-2350;}
//     else if(fPlane->GetPlaneNumber()==4){cut1[0]=2000.;cut1[1]=-1000.;cut2[0]=2000.;cut2[1]=-2250;}*/

//     if((fPSeed->GetPosition()(0)>cut1[0] || fPSeed->GetPosition()(0)<cut1[1]
//        || fPSeed->GetPosition()(1)>cut2[0] || fPSeed->GetPosition()(1)<cut2[1]) 
//        && fPlane->GetPlaneNumber()<1 ) valid=kFALSE;
    //RDM050809 for M25 end //in case you want to use this cut, put <5 instead of <1 in the last "if" line

    // now compute position correction wrt seed position depending on the required algorithm
    if( valid) {

      if(fDebugHit>2) printf("  DHit:Analyse  hit is valid!\n");

      //==================
      //Center of gravity with all the neighbours
      // pay attention that pixels are ordered geometricaly from one corner to the other in the fStripDistanceU/V arrays

      tCorrection.SetValue( 0., 0., 0.);
      Float_t tClusterPulseSum = 0.;
      for( Int_t iStripIndex=0; iStripIndex < fStripsInClusterFound; iStripIndex++ ) {
	   
        // Computation without the fStripDistance array, MG 2010/06/28
        aNeighbour = aListOfPixels->at( tPixelIndexList[ iStripIndex]);
        tCorTemp.SetValue( aNeighbour->GetPosition()(0)*aNeighbour->GetPulseHeight(),
                          aNeighbour->GetPosition()(1)*aNeighbour->GetPulseHeight(), 0);
        tCorrection += tCorTemp;
        tClusterPulseSum += aNeighbour->GetPulseHeight();
        
        // "Old" computation with the fStripDistance array
	     /*
 	     tCorTemp.SetValue( ((fPSeed->GetPosition())(0)-fStripDistanceU[iStripIndex])*fStripPulseHeight[iStripIndex],
 		  ((fPSeed->GetPosition())(1)-fStripDistanceV[iStripIndex])*fStripPulseHeight[iStripIndex],
 		  0);
	     tCorrection += tCorTemp;
	     tClusterPulseSum += fStripPulseHeight[iStripIndex];
	     */
	     //printf("DHit::Analyse plane %d, hit %2d, strip %2d, corr(%.2f, %.2f)=dist(%.2f, %.2f)*%.0f/%.0f\n", fPlane->GetPlaneNumber(), fHitNumber, iStripIndex, tCorrection(0), tCorrection(1), fStripDistanceU[iStripIndex], fStripDistanceV[iStripIndex], fStripPulseHeight[iStripIndex], tClusterPulseSum);
      }

      *fPositionHitCG = tCorrection/tClusterPulseSum;
       fIfMonteCarlo = fPSeed->IfMonteCarlo();
       if(fIfMonteCarlo==1) _monteCarloInfo = fPSeed->GetMonteCarloInfo();
      //==================
      //Center of gravity restricted to a 3x3 cluster

      tCorrection.SetValue( 0., 0., 0.);
      Float_t tCluster33PulseSum = 0.;
      
      for( Int_t iStripIndex=0; iStripIndex < fStripsInClusterFound; iStripIndex++ ) {
	    
       if( fabs(fStripDistanceU[iStripIndex])<=fPSeed->GetSize()(0)
	    && fabs(fStripDistanceV[iStripIndex])<=fPSeed->GetSize()(1) ) { // check neighbours is at most at 1 pitch from seed
	      // Computation without the fStripDistance array, MG 2010/06/28
	      aNeighbour = aListOfPixels->at( tPixelIndexList[ iStripIndex]);
	      tCorTemp.SetValue( aNeighbour->GetPosition()(0)*aNeighbour->GetPulseHeight(),
			aNeighbour->GetPosition()(1)*aNeighbour->GetPulseHeight(),
			0);
	      tCorrection += tCorTemp;
	      tClusterPulseSum += aNeighbour->GetPulseHeight();

	  // "Old" computation with the fStripDistance array
	  /*
	    tCorTemp.SetValue( ((fPSeed->GetPosition())(0)-fStripDistanceU[iStripIndex])*fStripPulseHeight[iStripIndex],
	    ((fPSeed->GetPosition())(1)-fStripDistanceV[iStripIndex])*fStripPulseHeight[iStripIndex],
	    0);
	    tCorrection += tCorTemp;
	    tClusterPulseSum += fStripPulseHeight[iStripIndex];
	  */
	  //printf("DHit::Analyse plane %d, hit %2d, strip %2d, corr(%.2f, %.2f)=dist(%.2f, %.2f)*%.0f/%.0f\n", fPlane->GetPlaneNumber(), fHitNumber, iStripIndex, tCorrection(0), tCorrection(1), fStripDistanceU[iStripIndex], fStripDistanceV[iStripIndex], fStripPulseHeight[iStripIndex], tCluster33PulseSum);
 	    }
     }
      *fPositionHitCG33 = tCorrection/tCluster33PulseSum;


      //==================
      //Center of gravity restricted 2x2 neighbours with the highest charge, JB Nov 2007
      // 2x2 includes the seed

      // The index of the neighbours with the highest charges are ordered in fStripIndexArray

      tCorrection.SetValue( 0., 0., 0.);
      Float_t tCluster22PulseSum = 0.;
      Int_t tOrderedIndex;
      for( Int_t iStripIndex=0; iStripIndex < TMath::Min( 4, fStripsInClusterFound); iStripIndex++ ) { // limit changed for sparsified data, JB 2009/09/01
	tOrderedIndex = fStripIndexArray[iStripIndex];
	tCorTemp.SetValue( (fPSeed->GetPosition()(0)-fStripDistanceU[tOrderedIndex])*fStripPulseHeight[tOrderedIndex],
			   (fPSeed->GetPosition()(1)-fStripDistanceV[tOrderedIndex])*fStripPulseHeight[tOrderedIndex],
			   0);
	tCorrection += tCorTemp;
	tCluster22PulseSum += fStripPulseHeight[tOrderedIndex];
      }
      *fPositionHitCG22 = tCorrection/tCluster22PulseSum; 


      //==================
      //Eta algorithm 3x3
      // correct the 3x3 center of gravity (tCorrection) using the integral histogram of the charge density
      // the eta position is the Y value of the bin with X value = tCorrection
      // if the tCorrection from CoG is out of the X axis range, do not correct

      if ( tDigital ){ // check info file was there and Eta is available
	if(fDebugHit>1 && (fPositionAlgorithm==2 || fPositionAlgorithm==22)) printf("  DHit.cxx:Analyse  Digital or CoG response only set for plane %d (forgot about etaXXX.root file?)\n",fPlane->GetPlaneNumber());
      }
      else {
    
	TH1  *tEtaIntU = fPlane->GetEtaIntU();
	TH1  *tEtaIntV = fPlane->GetEtaIntV();
	Int_t iBinU    = tEtaIntU->FindBin( (*fPositionHitCG33-*fPositionHit)(0) );
	Int_t iBinV    = tEtaIntV->FindBin( (*fPositionHitCG33-*fPositionHit)(1) );
	Double_t corrU=0., corrV=0.;
	
	// correct wrt CoG if possible
	if( 1<=iBinU && iBinU<=tEtaIntU->GetNbinsX() ) corrU = tEtaIntU->GetBinContent( iBinU);
	if( 1<=iBinV && iBinV<=tEtaIntV->GetNbinsX() ) corrV = tEtaIntV->GetBinContent( iBinV);
	tCorrection.SetValue( corrU, corrV, 0.);
	if( fDebugHit>1) printf("    Eta correction/digital is (%.1f, %.1f)\n", corrU, corrV);
	*fPositionHitEta = *fPositionHit + tCorrection;

      //==================
      //Eta algorithm 2x2
      // correct the 2x2 center of gravity (tCorrection) using the integral histogram of the charge density
      // the eta position is the Y value of the bin with X value = tCorrection
      // if the tCorrection from CoG is out of the X axis range, do not correct
    
	tEtaIntU = fPlane->GetEtaIntU2();
	tEtaIntV = fPlane->GetEtaIntV2();
	iBinU    = tEtaIntU->FindBin( (*fPositionHitCG22-*fPositionHit)(0) );
	iBinV    = tEtaIntV->FindBin( (*fPositionHitCG22-*fPositionHit)(1) );
	corrU = corrV = 0.;
	
	// correct wrt CoG if possible
	if( 1<=iBinU && iBinU<=tEtaIntU->GetNbinsX() ) corrU = tEtaIntU->GetBinContent( iBinU);
	if( 1<=iBinV && iBinV<=tEtaIntV->GetNbinsX() ) corrV = tEtaIntV->GetBinContent( iBinV);
	tCorrection.SetValue( corrU, corrV, 0.);
	
	*fPositionHitEta22 = *fPositionHit + tCorrection;

      } // end if tDigital

      //==================
      //Analog head-tail algorithm
      // Algorithm used for large incident angle when seed contains little info on position
      // Relies on the extreme pixels on the left and right
      // tCorrection.SetValue( (fStripDistanceU[iLeft]+fStripDistanceU[iRight])/2., , 0.);
      // *fPositionhitAHT = *fPositionHit - tCorrection;

      //==================
      // Now choose which position is stored
      if( fPositionAlgorithm==0 ) { // keep Seed position
      }
      else if( fPositionAlgorithm==1 ) { // Center of Gravity
	*fPositionHit = *fPositionHitCG;
      }
      else if( fPositionAlgorithm==11 ) { // Center of gravity on 3x3
	*fPositionHit = *fPositionHitCG33;
      }
      else if( fPositionAlgorithm==12 ) { // Center of gravity on 2x2
	*fPositionHit = *fPositionHitCG22;
	}
      else if( fPositionAlgorithm==2 ) { // Eta corrected from CoG 3x3
	*fPositionHit = *fPositionHitEta;
      }
      else if( fPositionAlgorithm==22 ) { // Eta corrected from CoG 2x2
	*fPositionHit = *fPositionHitEta22;
      }
      else {
	printf("-*-*- WARNING: algorithm %d for position unknown, taking digital position\n", fPositionAlgorithm);
      }
      if(fDebugHit>1) printf("  DHit.cxx:Analyse pos algo %d requested, seed[%d-%d]=(%.1f, %.1f), CG(%.1f, %.1f), CG33(%.1f, %.1f), eta(%.1f, %.1f), eta22(%.1f, %.1f)\n", fPositionAlgorithm, fPSeed->GetPixelLine(), fPSeed->GetPixelColumn(), fSeedU, fSeedV, (*fPositionHitCG)(0), (*fPositionHitCG)(1), (*fPositionHitCG33)(0), (*fPositionHitCG33)(1), (*fPositionHitEta)(0), (*fPositionHitEta)(1), (*fPositionHitEta22)(0), (*fPositionHitEta22)(1));

    } //end if valid

  } // end if PIXEL mode


  // Release the neighbouring Pixels associated to the seed if the pixel is finally not selected 
  // only the tested seed (Index=0) is not released
  // JB 2009/05/10
  if( !valid ) { 
    if(fDebugHit>1) printf("  DHit.cxx:Analyse  hit NOT selected, releasing %d pixels\n", fStripsInClusterFound-1);
    for (Int_t iNeigh = 1; iNeigh < fStripsInClusterFound; iNeigh++){ // loop over associated pixels
      if(fDebugHit>2) printf("  DHit.cxx:Analyse    releasing pixel %d at index %d\n", iNeigh, fStripIndexArray[iNeigh]);
      aListOfPixels->at(fStripIndexArray[iNeigh])->SetFound(kFALSE);
    } // end loop over associated pixels
  } // end if !valid

  else if(fDebugHit>1) {
    printf("        hit selected with %d pixels\n", fStripsInClusterFound);
  }

  return valid;
}

//______________________________________________________________________________
//  
Bool_t DHit::Analyse_2_cgo( Int_t aPixelIndexInList, std::vector<DPixel*> *aListOfPixels)
{
  // Based on Analyse( Int_t aPixelIndexInList, vector<DPixel*> *aListOfPixels) but 
  //     Compute the center of gravity with floating point value in "pixel unit"
  //     Compare the ditance between cog to a pixel position (in pixel unit in floating point) with fClusterLimit to associate or not a pixel
  //     "Pixel unit" is choose for clarity (for debug this unity is more clear !)
  // Hit is valid if  fCut->GetStripsInClusterMin() <= fStripsInClusterFound && fCut->GetStripsInClusterMax() >= fStripsInClusterFound
  //
  // Construct a hit (or cluster) from a seed pixel and the list of all fired pixels:
  // 1) try to associate all neighbouring strips that pass selection cut
  // 2) check the build hit passes all selections
  // 3) order the array fStripIndexArray of neighbours by decreasing pulseheight
  // 4) compute cluster charge, noise and position
  //
  // Note that the behavior of this method is affected by the AnalysisMode chosen.
  //
  // Return a boolean: kTRUE if hit is selected, kFalse otherwise
  //
  // Constructed from the original Analyse( Int_t aPixelIndexInList, vector<DPixel*> *aListOfPixels) by VR 2014/07/16
  // Modified: JB 2015/05/26 to introduce timestamps and TimeLimit
  
  fFound      = kFALSE; // JB 2009/05/22
  fPSeed      = aListOfPixels->at( aPixelIndexInList);
  
  DPixel   *aNeighbour;
  DPixel   *aPixel=NULL;
  Bool_t    pixelTwice = kFALSE; // test if a pixel is present twice in the neighbour list
  Bool_t    valid    = kFALSE; // default return value
  Bool_t    tDigital = fPlane->GetStripResponseSetting();
  Int_t     tStripIndex;
  //Int_t     tPixelIndexList[500];
  
  Int_t     tTimeLimit = fPlane->GetTimeLimit(); // 2015/05/26
  
  fPositionHitCG->Zero();           // clear the position
  fPositionHitEta->Zero();
  fPositionHitEta22->Zero(); // JB 2010/12/8
  fPositionHit->Zero();
  fClusterSignalToNoise   = 0.0;

  fSeedU      = fPSeed->GetPosition()(0);
  fSeedV      = fPSeed->GetPosition()(1); 
  fIndexSeed  = fPSeed->GetPixelIndex();
  
  Float_t ClusterLimitRadiusPix = fClusterLimitRadius * 2 / (fPSeed->GetSize()(0)+fPSeed->GetSize()(1)); // assume square pixels !!
  
  DR3 cog(fPSeed->GetPixelLine(),fPSeed->GetPixelColumn(),0); //VR 2014/07/12 cog is in "pixels unit" for an easiest debug
  
  Int_t iSeed = 0;

  // here, the neighbourhood of strips is assumed to be ordered, 
  // at index 0 is the seed, to higher indices
  // the neighbourstrips are further away from the seed.

  Float_t tClusterAreaNoise = 0.;
  fClusterAreaPulseSum      = 0.; // area means the cluster except the seed
  fClusterPulseSum        = fPSeed->GetPulseHeight(); 
  fClusterNoiseAverage    = (fPSeed->GetNoise())*(fPSeed->GetNoise());  // => so it's only a seed  strip in the beginning
  fStripsInClusterFound   = 1;                       
  fPSeed->SetFound(kTRUE);                            
  

  
  tPixelIndexList[0]   = aPixelIndexInList;
  fStripIndexArray[0]  = 0;                            // place seed in position 0 of neighbour
  fStripIndex[0]       = fPSeed->GetPixelIndex();      // store seed index, JB 2009/05/12
  fStripPulseHeight[0] = fPSeed->GetPulseHeight();     // store seed pulseheight
  fStripNoise[0]       = fPSeed->GetNoise();           // store seed noise  //YV 27/11/09
  fSNseed              = fPSeed->GetPulseHeightToNoise(); // JB 2013/11/08
  fSeedPulseHeight     = fPSeed->GetPulseHeight(); // Note that seed might change
  fSeedNoise           = fPSeed->GetNoise();
  fStripDistanceU[0] = 0.;                             // distance seed to seed = 0.
  fStripDistanceV[0] = 0.;                             // distance seed to seed = 0.
  fStoNover2 = 0;                                      // MB/12/11/2010 

  //===============
  // associate neighbouring pixels to seed one
  //===============
  if(fDebugHit>1) printf("  DHit:Analyse_2_cgo seed pixel index %d (%d in list, r%d, c%d) (q=%f) with search radius max %1.2f [pixels unit] \n", fIndexSeed, aPixelIndexInList, fPSeed->GetPixelLine(), fPSeed->GetPixelColumn(), fPSeed->GetPulseHeight(), ClusterLimitRadiusPix);

  tStripIndex = 1; // start with the first neighbour, in the geometric ordered neighbourhood, avoid 0 because it is the seed itself!
  for (Int_t iPix = 0; iPix < (Int_t)aListOfPixels->size(); iPix++)
  { // loop over hit pixels
    
    aNeighbour = aListOfPixels->at(iPix);
    // Test if the pixel can be associated to the seed 
    //   inside the geometrical cluster limits
    // and also in the time limit using the timestamp (which are all 0 if unavailable).
    //   JB+SS 2013/06/23
    // Geometrical limit test changed from a single 2D distance test
    //  to two 1D distance test.
    //  to JB 2013/08/29 to match really a rectangle
    if ( !aNeighbour->Found() 
        //&& fPSeed->GetTimestamp() == aNeighbour->GetTimestamp()
        && abs(fPSeed->GetTimestamp()-aNeighbour->GetTimestamp())<=tTimeLimit
        ) 
    { // if neighbour pixel pass first test_delta_Col
      if ( sqrt(pow((cog(0)-Float_t(aNeighbour->GetPixelLine())),2) + pow((cog(1)-Float_t(aNeighbour->GetPixelColumn())),2)) <= ClusterLimitRadiusPix)
      { // is in radius ?
      
        // Additional test to avoid counting the same pixel twice
        // JB 2011/11/07
        pixelTwice = kFALSE;
        for ( Int_t jPix=0; jPix<fStripsInClusterFound; jPix++) 
        { // loop on currently found pixels
          aPixel = aListOfPixels->at( tPixelIndexList[ jPix] );
          //if(aNeighbour->GetPixelIndex() == aPixel->GetPixelIndex()) {
          //  pixelTwice = kTRUE;
          //  break;
          //}
          pixelTwice &= aNeighbour->GetPixelIndex() == aPixel->GetPixelIndex();
        } // end loop on currently found pixels
        
        if( !pixelTwice ) 
        { // if pixel is not counted twice
          
          fClusterAreaPulseSum     += aNeighbour->GetPulseHeight();
          tClusterAreaNoise +=(aNeighbour->GetNoise())*(aNeighbour->GetNoise());
          fClusterNoiseAverage   += (aNeighbour->GetNoise())*(aNeighbour->GetNoise());
          fClusterPulseSum       += aNeighbour->GetPulseHeight();
          tPixelIndexList[tStripIndex]   = iPix; // store the index in the original list of pixels
          fStripIndexArray[tStripIndex]  = tStripIndex; // put neighbour index in array (not index of full pixel list)
          fStripIndex[tStripIndex]       = aNeighbour->GetPixelIndex(); // this is the index in the matrix, JB 2009/05/12
          fStripPulseHeight[tStripIndex] = aNeighbour->GetPulseHeight();
          fStripNoise[tStripIndex] = aNeighbour->GetNoise();             //YV 27/11/09
          fStripDistanceU[tStripIndex]   = fPSeed->DistanceU(aNeighbour->GetPosition());
          fStripDistanceV[tStripIndex]   = fPSeed->DistanceV(aNeighbour->GetPosition());
          aNeighbour->SetFound(kTRUE);
          if( fDebugHit>1) printf("  neighbour %d at index %d (r%d, c%d) with pulse %1.2f and distance %1.2f [pixels] (u=%1.2f, v=%1.2f) (<%1.2f) [pixels] found!\n", 
                                  tStripIndex,
                                  aNeighbour->GetPixelIndex(),
                                  aNeighbour->GetPixelLine(),
                                  aNeighbour->GetPixelColumn(),
                                  aNeighbour->GetPulseHeight(),
                                  sqrt(pow(cog(0)-Float_t(aNeighbour->GetPixelLine()),2)+pow(cog(1)-Float_t(aNeighbour->GetPixelColumn()),2)),
                                  fabs(cog(0)-Float_t(aNeighbour->GetPixelLine())),
                                  fabs(cog(1)-Float_t(aNeighbour->GetPixelColumn())),
                                  ClusterLimitRadiusPix);
          tStripIndex++;    // increment the strip index counter
          fStripsInClusterFound++;
          
          // For binary ouput, dynamically change the seed pixel if 
          //   there are more than 4 pixels found already,
          //   and the current seed is distant by more than half of the defined cluster limit
          // If so, then choose the seed as the pixel nearest to the center of gravity of all pixels
          // JB 2011/07/21     
          if( fPlane->GetAnalysisMode() == 3) 
          { // if binary readout
            aPixel=NULL;
            
            //VR 2014/07/12 calcul in Double_t
            if(fDebugHit>1) printf( "    updating cog : (%1.2f,%1.2f) -> ",cog(0), cog(1));
            cog(0) = (Double_t(aNeighbour->GetPixelLine())   + cog(0) * Double_t(fStripsInClusterFound-1)) / Double_t(fStripsInClusterFound); 
            cog(1) = (Double_t(aNeighbour->GetPixelColumn()) + cog(1) * Double_t(fStripsInClusterFound-1)) / Double_t(fStripsInClusterFound); 
            if(fDebugHit>1) printf( "(%1.2f,%1.2f) \n", cog(0), cog(1));
            
            
          } // end if binary readout
          // END of dynamical change of the seed pixel
          
        } // end if pixel is not counted twice
        
      } // end if neighbour pixel in search radius 
      else
      {
        if( fDebugHit>1) printf("    neighbour %d at index %d (r%d, c%d) with pulse %1.2f and distance %1.2f [pixels] (u=%1.2f, v=%1.2f) (>%1.2f) [pixels] to far away !\n", 
                        tStripIndex,
                        aNeighbour->GetPixelIndex(),
                        aNeighbour->GetPixelLine(),
                        aNeighbour->GetPixelColumn(),
                        aNeighbour->GetPulseHeight(),
                        sqrt(pow(cog(0)-Float_t(aNeighbour->GetPixelLine()),2)+pow(cog(1)-Float_t(aNeighbour->GetPixelColumn()),2)),
                        fabs(cog(0)-Float_t(aNeighbour->GetPixelLine())),
                        fabs(cog(1)-Float_t(aNeighbour->GetPixelColumn())),
                        ClusterLimitRadiusPix);
      }
    }// end if neighbour pixel pass pre-tests
    
  } // end loop over hit pixels

  if(fDebugHit>2) printf("  DHit:Analyse_2_cgo found %d valid neighbours\n", fStripsInClusterFound);

  
  //=============
  // order pixels
  //=============
  Int_t *nOfneighbours = new Int_t[fStripsInClusterFound];

  // For analog readout
  // re-order pixels from the highest charge to the lowest
  if( fPlane->GetAnalysisMode() == 2 )
  { // if analog readout

    DGlobalTools aTool; // JB 2011/07/18
    aTool.OrderIndexDes(&fStripIndexArray[0], &fStripPulseHeight[0], fStripsInClusterFound);
    if( fDebugHit>2) { // display result of pixel ordering, JB 2009/05/12
      cout << "  DHit:Analyse_2_cgo  ordered list of " << fStripsInClusterFound << " pixels in candidate hit" << endl;
      for( Int_t iPix=0; iPix<fStripsInClusterFound ; iPix++) { // loop on pixels in candidate hit
        printf( "    pix %2d, neighbour %7d,  pulse %f\n", iPix, fStripIndexArray[iPix], fStripPulseHeight[fStripIndexArray[iPix]]);
      } // end loop on pixels
    }

  } // end if analog readout


  // For binary readout, update the seed information
  else if( fPlane->GetAnalysisMode() == 3 ) 
  { // if binary readout

    /*
    // Redefine the seed as the one with the highest number of direct neighbours
    // This is only needed when the dynamic seed reallocation is not used
    // JB 2011/07/21
    DPixel *aPixel=NULL, *bPixel=NULL;
    for( Int_t iPix=0; iPix<fStripsInClusterFound; iPix++) { // loop on pixels in candidate hit
      nOfneighbours[ iPix] = 0;
      aPixel = aListOfPixels->at( tPixelIndexList[ iPix] );
      for( Int_t jPix=0; jPix<fStripsInClusterFound; jPix++) {
        if(fDebugHit>3) printf("     comparing pixels %d[%d] and %d[%d] in list of %d pixels\n", tPixelIndexList[ iPix], iPix, tPixelIndexList[ jPix], jPix, fStripsInClusterFound);
        bPixel = aListOfPixels->at( tPixelIndexList[ jPix] );
        if(fDebugHit>3) printf("     comparing pixels %d[%d] (%d-%d) and %d[%d] (%d-%d) in list of %d pixels\n", tPixelIndexList[ iPix], iPix, aPixel->GetPixelLine(), aPixel->GetPixelColumn(), tPixelIndexList[ jPix], jPix, bPixel->GetPixelLine(), bPixel->GetPixelColumn(), fStripsInClusterFound);
        // direct neighbours are defines as
        // abs(difference in line) + abs(difference in column) = 1
        if( (abs(aPixel->GetPixelLine() - bPixel->GetPixelLine()) + 
             abs(aPixel->GetPixelColumn() - bPixel->GetPixelColumn()) ) == 1 ) {
          nOfneighbours[ iPix] += 1;
        }
      }
      if(fDebugHit>3) printf("   DHit::Analyse_2_cgo pixel %d has %d direct neighbours <?> max%d\n", iPix, nOfneighbours[ iPix], nOfneighbours[ iSeed]);
      if( nOfneighbours[ iPix] > nOfneighbours[ iSeed] || (nOfneighbours[ iPix] == nOfneighbours[ iSeed] && (aPixel->GetPixelColumn()<aListOfPixels->at( tPixelIndexList[ iPix] )->GetPixelColumn() || aPixel->GetPixelLine()<aListOfPixels->at( tPixelIndexList[ iSeed] )->GetPixelLine()) ) ) { // force lower lefter seed, JB 2009/09/07
        iSeed = iPix;
      }
    } // end loop on pixels
    */
    
    // swap previous info at index 0 with the one at index iSeed
    // so that all arrays have seed info at index 0
    Int_t iTemp;
    Float_t temp;
    iTemp                    = tPixelIndexList[iSeed];
    tPixelIndexList[iSeed]   = tPixelIndexList[0];
    tPixelIndexList[0]       = iTemp;
    iTemp                    = fStripIndexArray[iSeed];
    fStripIndexArray[iSeed]  = fStripIndexArray[0];
    fStripIndexArray[0]      = iTemp;
    iTemp                    = fStripIndex[iSeed];
    fStripIndex[iSeed]       = fStripIndex[0];
    fStripIndex[0]           = iTemp;
    temp                     = fStripPulseHeight[iSeed];
    fStripPulseHeight[iSeed] = fStripPulseHeight[0];
    fStripPulseHeight[0]     = temp;

    // need to update some variables 
    // to take into account the new seed definition
    fPSeed = aListOfPixels->at( tPixelIndexList[ 0] );
    fSeedU      = fPSeed->GetPosition()(0);
    fSeedV      = fPSeed->GetPosition()(1); 
    fIndexSeed  = fPSeed->GetPixelIndex();
    for( Int_t iPix=1; iPix<fStripsInClusterFound ; iPix++)
    { // loop on pixels in candidate hit
      aNeighbour = aListOfPixels->at( tPixelIndexList[ iPix]);
      fStripDistanceU[iPix]   = fPSeed->DistanceU(aNeighbour->GetPosition());
      fStripDistanceV[iPix]   = fPSeed->DistanceV(aNeighbour->GetPosition());
    }

    if( fDebugHit>2 ) 
    {
      printf("  DHit:Analyse_2_cgo  seed is now pixel %d in list, index %d, with %d direct neighbours\n", tPixelIndexList[ 0], fIndexSeed, nOfneighbours[ iSeed]);
    }
    
  } // end if binary readout

  else {
    printf("DHit::Analyse_2_cgo WARNING analysis mode %d UNKNOWN !!\n", fPlane->GetAnalysisMode() );
    return false;
  }
  
  delete[] nOfneighbours; // remove memory leaks, BH 2013/08/21
  
  //==============
  // additional properties
  //==============  
  // the number of strips in the cluster and the cluster charge sum are now known
  // Depending wether the noise is calculated or not, 
  // the SN ratio is a real ratio or simply the charge
  // JB 2010/10/05 and 2010/10/15 to separately compute Cluster and Area values

  tClusterAreaNoise  = sqrt(tClusterAreaNoise);
  
  if (fClusterNoiseAverage > 0.5) {
    fClusterSignalToNoise  = fClusterPulseSum     / sqrt(fClusterNoiseAverage);
  }
  else {
    fClusterSignalToNoise  = fClusterPulseSum;
  }
  if (tClusterAreaNoise > 0.5) {
    fSNneighbour           = fClusterAreaPulseSum / tClusterAreaNoise;
  }
  else {
    fSNneighbour           = fClusterAreaPulseSum;
  }

  // Number of pixels with 2*pitch of the seed with S/N>2.
  // MB/12/11/2010
  if( fPlane->GetAnalysisMode() <= 2 ) {
    for( Int_t iPix=0; iPix<fStripsInClusterFound; iPix++) { // loop on pixels in candidate hit
      aNeighbour = aListOfPixels->at( tPixelIndexList[iPix]);
      if ( fPSeed->Distance( *aNeighbour) <= 2*(*fStripPitch)(0) && fPSeed->GetPulseHeightToNoise() >= 2 ){
        fStoNover2++;
      }
      if(fDebugHit>1)  cout<<"  DHit:Analyse_2_cgo  StoNover2  = "<<fStoNover2<<"   fPSeed->Distance( *aNeighbour)   = "<<fPSeed->Distance( *aNeighbour)<<"  fPSeed->GetFound() = "<<  fPSeed->Found()<<endl;
    } // end loop on pixels in candidate hit
  }  
  
  fTimestamp = fPSeed->GetTimestamp(); // JB 2015/05/25
  
  if(fDebugHit>2) printf("  DHit:Analyse  potential hit has : PulseSum=%f, areaPulseSum=%f, noise=%f, areaNoise=%f, clusterSignalToNoise=%f, areaSignalToNoise=%f <?> cut %f, # pixels=%d within? [%d, %d], stover2=%d, time=%d\n", fClusterPulseSum, fClusterAreaPulseSum, fClusterNoiseAverage, tClusterAreaNoise, fClusterSignalToNoise, fSNneighbour, fCut->GetNeighbourPulseHeightToNoise(), fStripsInClusterFound, fCut->GetStripsInClusterMin(), fCut->GetStripsInClusterMax(), fStoNover2, fTimestamp);


  //===============
  // Compute hit position
  //===============

  // start to  calculate the exact hit position, which is in first order the seed strip position  
  (*fPositionHit)(0)    = (fPSeed->GetPosition())(0);
  (*fPositionHit)(1)    = (fPSeed->GetPosition())(1);
  (*fPositionHit)(2)    = (fPSeed->GetPosition())(2);

  // This is the container (3D distance) for the correction to be computed with different algorithm
  DR3 tCorrection ; tCorrection.SetValue(0.,0.,0.);
  DR3 tCorTemp ;

  //===============
  //--- for PIXEL planes
  //
  if( fPlane->GetAnalysisMode() >= 2 ) { // if pixel mode

    // Cut on the charge or on the charge/noise (See above)
    // Cut on # pixels in hit added, JB 2009/08/30

    if( fSNneighbour >= fCut->GetNeighbourPulseHeightToNoise() 
        && fCut->GetStripsInClusterMin() <= fStripsInClusterFound
        && fCut->GetStripsInClusterMax() >= fStripsInClusterFound
      ){
      valid = kTRUE;
    }else{
      valid = kFALSE;
    }

    //RDM050809 for M25 begin
//     Float_t cut1[2];
//     Float_t cut2[2];
//     if(fPlane->GetPlaneNumber()==1)     {cut1[0]=0.   ;cut1[1]=-2100.;cut2[0]=0.;cut2[1]=-2250.;}
//     else if(fPlane->GetPlaneNumber()==2){cut1[0]=2000.;cut1[1]=0.    ;cut2[0]=0.;cut2[1]=-2350.;}
//     else if(fPlane->GetPlaneNumber()==3){cut1[0]= 450.;cut1[1]=-1800.;cut2[0]=0.;cut2[1]=-2350;}
//     else if(fPlane->GetPlaneNumber()==4){cut1[0]=1700.;cut1[1]= -500.;cut2[0]=0.;cut2[1]=-2250;}

//     //for M24
//     if(fPlane->GetPlaneNumber()==1)     {cut1[0]=1000.;cut1[1]=-1600.;cut2[0]=1800.;cut2[1]=-2250.;}
//     else if(fPlane->GetPlaneNumber()==2){cut1[0]=2200.;cut1[1]=-600. ;cut2[0]=1800.;cut2[1]=-2350.;}
//     else if(fPlane->GetPlaneNumber()==3){cut1[0]=1000.;cut1[1]=-1500.;cut2[0]=1800.;cut2[1]=-2350;}
//     else if(fPlane->GetPlaneNumber()==4){cut1[0]=2000.;cut1[1]=-1000.;cut2[0]=2000.;cut2[1]=-2250;}*/

//     if((fPSeed->GetPosition()(0)>cut1[0] || fPSeed->GetPosition()(0)<cut1[1]
//        || fPSeed->GetPosition()(1)>cut2[0] || fPSeed->GetPosition()(1)<cut2[1]) 
//        && fPlane->GetPlaneNumber()<1 ) valid=kFALSE;
    //RDM050809 for M25 end //in case you want to use this cut, put <5 instead of <1 in the last "if" line

    // now compute position correction wrt seed position depending on the required algorithm
    if( valid) 
    {

      if(fDebugHit>2) printf("  DHit:Analyse_2_cgo  hit is valid!\n");

      //==================
      //Center of gravity with all the neighbours
      // pay attention that pixels are ordered geometricaly from one corner to the other in the fStripDistanceU/V arrays

      tCorrection.SetValue( 0., 0., 0.);
      Float_t tClusterPulseSum = 0.;
      for( Int_t iStripIndex=0; iStripIndex < fStripsInClusterFound; iStripIndex++ ) {
        // Computation without the fStripDistance array, MG 2010/06/28
        aNeighbour = aListOfPixels->at( tPixelIndexList[ iStripIndex]);
        tCorTemp.SetValue( aNeighbour->GetPosition()(0)*aNeighbour->GetPulseHeight(),
                           aNeighbour->GetPosition()(1)*aNeighbour->GetPulseHeight(),
                           0);
        tCorrection += tCorTemp;
        tClusterPulseSum += aNeighbour->GetPulseHeight();

        // "Old" computation with the fStripDistance array
        /*
        tCorTemp.SetValue( ((fPSeed->GetPosition())(0)-fStripDistanceU[iStripIndex])*fStripPulseHeight[iStripIndex],
                           ((fPSeed->GetPosition())(1)-fStripDistanceV[iStripIndex])*fStripPulseHeight[iStripIndex],
                           0);
        tCorrection += tCorTemp;
        tClusterPulseSum += fStripPulseHeight[iStripIndex];
        */
        //printf("DHit::Analyse_2_cgo plane %d, hit %2d, strip %2d, corr(%.2f, %.2f)=dist(%.2f, %.2f)*%.0f/%.0f\n", fPlane->GetPlaneNumber(), fHitNumber, iStripIndex, tCorrection(0), tCorrection(1), fStripDistanceU[iStripIndex], fStripDistanceV[iStripIndex], fStripPulseHeight[iStripIndex], tClusterPulseSum);
      }
      *fPositionHitCG = tCorrection/tClusterPulseSum;
      fIfMonteCarlo = fPSeed->IfMonteCarlo();
      if(fIfMonteCarlo==1) _monteCarloInfo = fPSeed->GetMonteCarloInfo();

      //==================
      //Center of gravity restricted to a 3x3 cluster

      tCorrection.SetValue( 0., 0., 0.);
      Float_t tCluster33PulseSum = 0.;
      for( Int_t iStripIndex=0; iStripIndex < fStripsInClusterFound; iStripIndex++ ) {
        if(    fabs(fStripDistanceU[iStripIndex])<=fPSeed->GetSize()(0)
            && fabs(fStripDistanceV[iStripIndex])<=fPSeed->GetSize()(1)
            ) { // check neighbours is at most at 1 pitch from seed
          // Computation without the fStripDistance array, MG 2010/06/28
          aNeighbour = aListOfPixels->at( tPixelIndexList[ iStripIndex]);
          tCorTemp.SetValue( aNeighbour->GetPosition()(0)*aNeighbour->GetPulseHeight(),
                             aNeighbour->GetPosition()(1)*aNeighbour->GetPulseHeight(),
                             0);
          tCorrection += tCorTemp;
          tClusterPulseSum += aNeighbour->GetPulseHeight();

          // "Old" computation with the fStripDistance array
          /*
            tCorTemp.SetValue( ((fPSeed->GetPosition())(0)-fStripDistanceU[iStripIndex])*fStripPulseHeight[iStripIndex],
            ((fPSeed->GetPosition())(1)-fStripDistanceV[iStripIndex])*fStripPulseHeight[iStripIndex],
            0);
            tCorrection += tCorTemp;
            tClusterPulseSum += fStripPulseHeight[iStripIndex];
          */
          //printf("DHit::Analyse_2_cgo plane %d, hit %2d, strip %2d, corr(%.2f, %.2f)=dist(%.2f, %.2f)*%.0f/%.0f\n", fPlane->GetPlaneNumber(), fHitNumber, iStripIndex, tCorrection(0), tCorrection(1), fStripDistanceU[iStripIndex], fStripDistanceV[iStripIndex], fStripPulseHeight[iStripIndex], tCluster33PulseSum);
        }
      }
      *fPositionHitCG33 = tCorrection/tCluster33PulseSum;


      //==================
      //Center of gravity restricted 2x2 neighbours with the highest charge, JB Nov 2007
      // 2x2 includes the seed

      // The index of the neighbours with the highest charges are ordered in fStripIndexArray

      tCorrection.SetValue( 0., 0., 0.);
      Float_t tCluster22PulseSum = 0.;
      Int_t tOrderedIndex;
      for( Int_t iStripIndex=0; iStripIndex < TMath::Min( 4, fStripsInClusterFound); iStripIndex++ ) { // limit changed for sparsified data, JB 2009/09/01
        tOrderedIndex = fStripIndexArray[iStripIndex];
        tCorTemp.SetValue( (fPSeed->GetPosition()(0)-fStripDistanceU[tOrderedIndex])*fStripPulseHeight[tOrderedIndex],
                           (fPSeed->GetPosition()(1)-fStripDistanceV[tOrderedIndex])*fStripPulseHeight[tOrderedIndex],
                           0);
        tCorrection += tCorTemp;
        tCluster22PulseSum += fStripPulseHeight[tOrderedIndex];
      }
      *fPositionHitCG22 = tCorrection/tCluster22PulseSum; 


      //==================
      //Eta algorithm 3x3
      // correct the 3x3 center of gravity (tCorrection) using the integral histogram of the charge density
      // the eta position is the Y value of the bin with X value = tCorrection
      // if the tCorrection from CoG is out of the X axis range, do not correct

      if ( tDigital ){ // check info file was there and Eta is available
        if(fDebugHit>1 && (fPositionAlgorithm==2 || fPositionAlgorithm==22)) printf("  DHit.cxx:Analyse_2_cgo  Digital or CoG response only set for plane %d (forgot about etaXXX.root file?)\n",fPlane->GetPlaneNumber());
      }
      else {
    
        TH1  *tEtaIntU = fPlane->GetEtaIntU();
        TH1  *tEtaIntV = fPlane->GetEtaIntV();
        Int_t iBinU    = tEtaIntU->FindBin( (*fPositionHitCG33-*fPositionHit)(0) );
        Int_t iBinV    = tEtaIntV->FindBin( (*fPositionHitCG33-*fPositionHit)(1) );
        Double_t corrU=0., corrV=0.;
        
        // correct wrt CoG if possible
        if( 1<=iBinU && iBinU<=tEtaIntU->GetNbinsX() ) corrU = tEtaIntU->GetBinContent( iBinU);
        if( 1<=iBinV && iBinV<=tEtaIntV->GetNbinsX() ) corrV = tEtaIntV->GetBinContent( iBinV);
        tCorrection.SetValue( corrU, corrV, 0.);
        if( fDebugHit>1) printf("    Eta correction/digital is (%.1f, %.1f)\n", corrU, corrV);
        *fPositionHitEta = *fPositionHit + tCorrection;

      //==================
      //Eta algorithm 2x2
      // correct the 2x2 center of gravity (tCorrection) using the integral histogram of the charge density
      // the eta position is the Y value of the bin with X value = tCorrection
      // if the tCorrection from CoG is out of the X axis range, do not correct
    
        tEtaIntU = fPlane->GetEtaIntU2();
        tEtaIntV = fPlane->GetEtaIntV2();
        iBinU    = tEtaIntU->FindBin( (*fPositionHitCG22-*fPositionHit)(0) );
        iBinV    = tEtaIntV->FindBin( (*fPositionHitCG22-*fPositionHit)(1) );
        corrU = corrV = 0.;
        
        // correct wrt CoG if possible
        if( 1<=iBinU && iBinU<=tEtaIntU->GetNbinsX() ) corrU = tEtaIntU->GetBinContent( iBinU);
        if( 1<=iBinV && iBinV<=tEtaIntV->GetNbinsX() ) corrV = tEtaIntV->GetBinContent( iBinV);
        tCorrection.SetValue( corrU, corrV, 0.);
        
        *fPositionHitEta22 = *fPositionHit + tCorrection;

      } // end if tDigital

      //==================
      //Analog head-tail algorithm
      // Algorithm used for large incident angle when seed contains little info on position
      // Relies on the extreme pixels on the left and right
      // tCorrection.SetValue( (fStripDistanceU[iLeft]+fStripDistanceU[iRight])/2., , 0.);
      // *fPositionhitAHT = *fPositionHit - tCorrection;

      //==================
      // Now choose which position is stored
      if( fPositionAlgorithm==0 ) { // keep Seed position
      }
      else if( fPositionAlgorithm==1 ) { // Center of Gravity
        *fPositionHit = *fPositionHitCG;
      }
      else if( fPositionAlgorithm==11 ) { // Center of gravity on 3x3
        *fPositionHit = *fPositionHitCG33;
      }
      else if( fPositionAlgorithm==12 ) { // Center of gravity on 2x2
        *fPositionHit = *fPositionHitCG22;
        }
      else if( fPositionAlgorithm==2 ) { // Eta corrected from CoG 3x3
        *fPositionHit = *fPositionHitEta;
      }
      else if( fPositionAlgorithm==22 ) { // Eta corrected from CoG 2x2
        *fPositionHit = *fPositionHitEta22;
      }
      else {
        printf("-*-*- WARNING: algorithm %d for position unknown, taking digital position\n", fPositionAlgorithm);
      }
      if(fDebugHit>1) printf("  DHit.cxx:Analyse_2_cgo pos algo %d requested, seed[%d-%d]=(%.1f, %.1f), CG(%.1f, %.1f), CG33(%.1f, %.1f), eta(%.1f, %.1f), eta22(%.1f, %.1f)\n", fPositionAlgorithm, fPSeed->GetPixelLine(), fPSeed->GetPixelColumn(), fSeedU, fSeedV, (*fPositionHitCG)(0), (*fPositionHitCG)(1), (*fPositionHitCG33)(0), (*fPositionHitCG33)(1), (*fPositionHitEta)(0), (*fPositionHitEta)(1), (*fPositionHitEta22)(0), (*fPositionHitEta22)(1));

    } //end if valid

  } // end if PIXEL mode


  // Release the neighbouring Pixels associated to the seed if the pixel is finally not selected 
  // only the tested seed (Index=0) is not released
  // JB 2009/05/10
  if( !valid ) { 
    if(fDebugHit>1) printf("  DHit.cxx:Analyse_2_cgo  hit NOT selected, releasing %d pixels\n", fStripsInClusterFound-1);
    for (Int_t iNeigh = 1; iNeigh < fStripsInClusterFound; iNeigh++){ // loop over associated pixels
      if(fDebugHit>2) printf("  DHit.cxx:Analyse_2_cgo    releasing pixel %d at index %d\n", iNeigh, fStripIndexArray[iNeigh]);
      aListOfPixels->at(fStripIndexArray[iNeigh])->SetFound(kFALSE);
    } // end loop over associated pixels
  } // end if !valid

  else if(fDebugHit>1) {
    printf("        hit selected with %d pixels\n", fStripsInClusterFound);
  }

  return valid;
}

//______________________________________________________________________________
//  

Bool_t DHit::Analyse_Iterative( Int_t aPixelIndexInList, std::vector<DPixel*> *aListOfPixels, bool &IsBigCluster, int MaxClusterSize)
{

  // Valid for sparse data acquisition
  //
  // For analog readout use the same algorithm in which construct a hit (or cluster) from a seed pixel and the list of all fired pixels:
  // 1) try to associate all neighbouring strips that pass selection cut
  // 2) check the build hit passes all selections
  // 3) order the array fStripIndexArray of neighbours by decreasing pulseheight
  // 4) compute cluster charge, noise and position
  // For digital readout implement a dynamic clustering algorithm
  //
  // Return a boolean: kTRUE if hit is selected, kFalse otherwise
  //
  // Constructed from the original Analyse( Int_t aPixelIndexInList, std::vector<DPixel*> *aListOfPixels ) by AP, 2014 July 01
  // Modified: JB 2015/05/26 to introduce timestamps and TimeLimit
  // 
  
  fFound      = kFALSE; // JB 2009/05/22
  fPSeed      = aListOfPixels->at(aPixelIndexInList);
  
  DPixel   *aNeighbour;
  DPixel   *aPixel=NULL;
  Bool_t    pixelTwice = kFALSE; // test if a pixel is present twice in the neighbour list
  Bool_t    valid    = kFALSE; // default return value
  Bool_t    tDigital = fPlane->GetStripResponseSetting();
  Int_t     tStripIndex;
  Int_t     tStripsInClusterPossible = 0; // remove a warning 
  //Int_t     tPixelIndexList[500];
  
  Int_t     tTimeLimit = fPlane->GetTimeLimit(); // 2015/05/26

  fPositionHitCG->Zero();           // clear the position
  fPositionHitEta->Zero();
  fPositionHitEta22->Zero(); // JB 2010/12/8
  fPositionHit->Zero();
  fClusterSignalToNoise   = 0.0;

  fSeedU      = fPSeed->GetPosition()(0);
  fSeedV      = fPSeed->GetPosition()(1); 
  fIndexSeed  = fPSeed->GetPixelIndex();
  
  Int_t seedRow = fPSeed->GetPixelLine(); 
  Int_t seedCol = fPSeed->GetPixelColumn(); 
  Int_t iSeed = 0;

  // here, the neighbourhood of strips is assumed to be ordered, 
  // at index 0 is the seed, to higher indices
  // the neighbourstrips are further away from the seed.

  Float_t tClusterAreaNoise = 0.;
  fClusterAreaPulseSum      = 0.; // area means the cluster except the seed
  fClusterPulseSum        = fPSeed->GetPulseHeight(); 
  fClusterNoiseAverage    = (fPSeed->GetNoise())*(fPSeed->GetNoise());  // => so it's only a seed  strip in the beginning
  fStripsInClusterFound   = 1;
  fPSeed->SetFound(kTRUE);
  
  if (fStripsInClusterDemanded >0) {        // exact number demanded
    tStripsInClusterPossible = fStripsInClusterDemanded;
  }
  else if (fStripsInClusterDemanded == 0) { // number from config file, JB 2009/08/21
    tStripsInClusterPossible = fCut->GetStripsInClusterArea(); //(Int_t)pow( fClusterLimit->Length()/(fPSeed->GetSize())(0)*2+1, 2), JB 2013/08/29 to match really a rectangle
  }
  else { // JB 2009/08/21
    printf("DHit::Analyse_dynamic WARNING limit on the # pixels in cluster %d is UNKNOWN !!\n", fStripsInClusterDemanded );
    return false;
  }

  tPixelIndexList[0]   = aPixelIndexInList;
  fStripIndexArray[0]  = 0;                            // place seed in position 0 of neighbour
  fStripIndex[0]       = fPSeed->GetPixelIndex();      // store seed index, JB 2009/05/12
  fStripPulseHeight[0] = fPSeed->GetPulseHeight();     // store seed pulseheight
  fStripNoise[0]       = fPSeed->GetNoise();           // store seed noise  //YV 27/11/09
  fSNseed              = fPSeed->GetPulseHeightToNoise(); // JB 2013/11/08
  fSeedPulseHeight     = fPSeed->GetPulseHeight(); // Note that seed might change
  fSeedNoise           = fPSeed->GetNoise();
  fStripDistanceU[0] = 0.;                             // distance seed to seed = 0.
  fStripDistanceV[0] = 0.;                             // distance seed to seed = 0.
  fStoNover2 = 0;                                      // MB/12/11/2010 

  //===============
  // associate neighbouring pixels to seed one
  //===============
  if(fDebugHit>1) printf("  DHit:Analyse seed pixel index %d (%d in list, r%d, c%d) (q=%f) with possibly %d neighbours\n",
			 fIndexSeed,
			 aPixelIndexInList,
			 fPSeed->GetPixelLine(),
			 fPSeed->GetPixelColumn(),
			 fPSeed->GetPulseHeight(),
			 tStripsInClusterPossible);


  //if( fPlane->GetAnalysisMode() != 3) { // if analog readout
  if( fPlane->GetAnalysisMode() < 2) { // if analog strip readout
    tStripIndex = 1; // start with the first neighbour, in the geometric ordered neighbourhood, avoid 0 because it is the seed itself!
    for (Int_t iPix = 0; iPix < (Int_t)aListOfPixels->size(); iPix++){ // loop over hit pixels
    
      aNeighbour = aListOfPixels->at(iPix);
      // Test if the pixel can be associated to the seed 
      //   inside the geometrical cluster limits
      // and also in the time limit using the timestamp (which are all 0 if unavailable).
      //   JB+SS 2013/06/23
      // Geometrical limit test changed from a single 2D distance test
      //  to two 1D distance test.
      //  to JB 2013/08/29 to match really a rectangle
      if ( !aNeighbour->Found() 
	   //&& fPSeed->Distance( *aNeighbour) < fClusterLimit->Length()
	   && fabs(fPSeed->DistanceU(aNeighbour->GetPosition())) <= (*fClusterLimit)(0)
	   && fabs(fPSeed->DistanceV(aNeighbour->GetPosition())) <= (*fClusterLimit)(1)
           //&& fPSeed->GetTimestamp() == aNeighbour->GetTimestamp()
           && abs(fPSeed->GetTimestamp()-aNeighbour->GetTimestamp())<=tTimeLimit
	 ) { // if neighbour pixel within limits
        
	// Additional test to avoid counting the same pixel twice
	// JB 2011/11/07
	pixelTwice = kFALSE;
	for ( Int_t jPix=0; jPix<fStripsInClusterFound; jPix++) { // loop on currently found pixels
	  aPixel = aListOfPixels->at( tPixelIndexList[ jPix] );
	  pixelTwice &= aNeighbour->GetPixelIndex() == aPixel->GetPixelIndex();
	} // end loop on currently found pixels
      
	if( !pixelTwice ) { // if pixel is not counted twice        
	  fClusterAreaPulseSum          += aNeighbour->GetPulseHeight();
	  tClusterAreaNoise             +=(aNeighbour->GetNoise())*(aNeighbour->GetNoise());
	  fClusterNoiseAverage          += (aNeighbour->GetNoise())*(aNeighbour->GetNoise());
	  fClusterPulseSum              += aNeighbour->GetPulseHeight();
	  tPixelIndexList[tStripIndex]   = iPix; // store the index in the original list of pixels
	  fStripIndexArray[tStripIndex]  = tStripIndex; // put neighbour index in array (not index of full pixel list)
	  fStripIndex[tStripIndex]       = aNeighbour->GetPixelIndex(); // this is the index in the matrix, JB 2009/05/12
	  fStripPulseHeight[tStripIndex] = aNeighbour->GetPulseHeight();
	  fStripNoise[tStripIndex] = aNeighbour->GetNoise();             //YV 27/11/09
	  fStripDistanceU[tStripIndex]   = fPSeed->DistanceU(aNeighbour->GetPosition());
	  fStripDistanceV[tStripIndex]   = fPSeed->DistanceV(aNeighbour->GetPosition());
	  aNeighbour->SetFound(kTRUE);

	  if( fDebugHit>1) printf("  neighbour %d at index %d (r%d, c%d) with pulse %.1f and distance %.1f(u=%.1f, v=%.1f) (<%.1f(u=%.1f, v=%.1f)) and timestamp %d / seedTimestamp %d within %d slot found!\n", 
				  tStripIndex,
				  aNeighbour->GetPixelIndex(),
				  aNeighbour->GetPixelLine(),
				  aNeighbour->GetPixelColumn(),
				  aNeighbour->GetPulseHeight(),
				  fPSeed->Distance( *aNeighbour),
				  fPSeed->DistanceU(aNeighbour->GetPosition()),
				  fPSeed->DistanceV(aNeighbour->GetPosition()),
				  fClusterLimit->Length(),
				  (*fClusterLimit)(0),
				  (*fClusterLimit)(1),
				  aNeighbour->GetTimestamp(),
				  fPSeed->GetTimestamp(),
				  tTimeLimit);
	  tStripIndex++;    // increment the strip index counter
	  fStripsInClusterFound++;
	} // end if pixel is not counted twice
	
      } // end if neighbour pixel within limits
      
    } // end loop over hit pixels
  } // end if analog strip readout
  else { //if pixel analog or binary readout
    
    //Doing a different cultering for binary readout using a iterative algorithm (most likely not optimal)
    std::vector<int> _TheClusterPixelsList;
    _TheClusterPixelsList.clear();
    _TheClusterPixelsList.push_back(aPixelIndexInList);
    
    if(fDebugHit>1) {
      cout << endl;
      cout << "DHit:: Plane " << fPlane->GetPlaneNumber() << ". Seed pixel = " << aPixelIndexInList << " at (col,lin) = (" << seedCol << "," << seedRow << ")" 
	   << ", and time-stamp = " << fPSeed->GetTimestamp() << ". Initial cluster size  = " << _TheClusterPixelsList.size() << endl;
      cout << endl;
    }

    //SON cut for pixels in case of analog output
    float SON_cut = 3.0;
    
    //Define a region of interest (ROI) to look for pixels to add to the seed pixel
    int delta_row_ROI = 70;
    int delta_col_ROI = 70;
    std::vector<int> _ROIlist;
    _ROIlist.clear();
    for(Int_t iPix = 0; iPix < (Int_t)aListOfPixels->size(); iPix++){ // loop over hit pixels
      if(aPixelIndexInList == iPix) continue;
      aNeighbour = aListOfPixels->at(iPix);
      if(aNeighbour->Found()) continue;
      if(abs(fPSeed->GetTimestamp() - aNeighbour->GetTimestamp()) > tTimeLimit) continue;
      int testRow = aNeighbour->GetPixelLine(); 
      int testCol = aNeighbour->GetPixelColumn();
      
      //Only consider pixels with a minimum S/N in case of analog output sensors
      if(fPlane->GetAnalysisMode() == 2 && aNeighbour->GetPulseHeightToNoise() < SON_cut) continue;
      
      //Only conside pixels in ROI region w.r.t seed pixel
      if(not( (abs(seedRow - testRow) < delta_row_ROI) &&  (abs(seedCol - testCol) < delta_col_ROI))) continue;
      
      _ROIlist.push_back(iPix);
    } //end of loop over hit pixels

    if(fDebugHit>1) {
      cout << "DHit:: Plane " << fPlane->GetPlaneNumber() << ". Added " << _ROIlist.size() << " pixel to the ROI list."
	   << endl;
      cout << endl;
    }

    //Start iterations to include pixels neighbours to already included pixels in cluster
    //Stop when no more neighbours are found
    int iteration_counter = 0;
    std::vector<int> _FiredPixelsList;
    do {
      _FiredPixelsList.clear();
     
      //Check for pixels neighbour to already included pixels 
      for(Int_t iPix = 0; iPix < int(_ROIlist.size()); iPix++) { // being loop over ROI pixels
	aNeighbour = aListOfPixels->at(_ROIlist[iPix]);
        if(aNeighbour->Found()) continue;
	if(abs(fPSeed->GetTimestamp() - aNeighbour->GetTimestamp()) > tTimeLimit) continue;
	
	int testRow = aNeighbour->GetPixelLine(); 
        int testCol = aNeighbour->GetPixelColumn();
	
	//Now check if test pixel is neighbour to any pixel already included in cluster
	for(int iPixClus=0; iPixClus < int(_TheClusterPixelsList.size()); iPixClus++) { // begin loop over pixel already included to cluster
	  if(_ROIlist[iPix] == _TheClusterPixelsList[iPixClus]) continue;
	  aPixel = aListOfPixels->at(_TheClusterPixelsList[iPixClus]);
	  
	  int clusRow = aPixel->GetPixelLine(); 
          int clusCol = aPixel->GetPixelColumn();
	  
	  int deltaRow = abs(clusRow - testRow);
          int deltaCol = abs(clusCol - testCol);
	  if((deltaRow ==  1 && deltaCol ==  0) || //pixel to the right/left 
	     (deltaRow ==  0 && deltaCol ==  1) || //pixel at the top/bottom
	     (deltaRow ==  1 && deltaCol ==  1)    //pixel at diagonal
            ) {
	    
	    //Check if this pixel has been already added to the list of fired pixels
	    bool IsNotAlreadyIn = true;
	    for(int iNewPix=0; iNewPix < int(_FiredPixelsList.size()); iNewPix++) {
	      if(_ROIlist[iPix] == _FiredPixelsList[iNewPix]) {
		IsNotAlreadyIn = false;
		break;
	      }
	    }
	    if(IsNotAlreadyIn) _FiredPixelsList.push_back(_ROIlist[iPix]);
	  }
        } // end loop over pixel already included to cluster
      } //end loop over ROI pixels
      
      //Now add the found pixels to the cluster and remove them from the ROI list
      if(fDebugHit>1) cout << "DHit:: Plane " << fPlane->GetPlaneNumber() << ". Found " << _FiredPixelsList.size() << " new pixels around seed pixel at iteration " << iteration_counter+1 << "!!!" << endl;
      for(int iNewPix=0; iNewPix < int(_FiredPixelsList.size()); iNewPix++) { //begin loop over new cluster pixels
	_TheClusterPixelsList.push_back(_FiredPixelsList[iNewPix]);
	
	if(fDebugHit>1) {
	  cout << "DHit:: Plane " << fPlane->GetPlaneNumber() << ". Adding pixel " <<  _FiredPixelsList[iNewPix] << " at (col,lin) = (" << aListOfPixels->at(_FiredPixelsList[iNewPix])->GetPixelColumn() << "," 
	       <<  aListOfPixels->at(_FiredPixelsList[iNewPix])->GetPixelLine()
	       << "), time-stamp = " << aListOfPixels->at(_FiredPixelsList[iNewPix])->GetTimestamp() << " (" << tTimeLimit << ")" << endl;
	}
	
	for(Int_t iPix = 0; iPix < int(_ROIlist.size()); iPix++) { // being loop over ROI pixels
	  if(_FiredPixelsList[iNewPix] == _ROIlist[iPix]) {
	    _ROIlist.erase(_ROIlist.begin() + iPix);
	    break;
	  }
	} //end loop over ROI pixels
	
      } //end loop over new cluster pixels
      if(fDebugHit>1 && int(_FiredPixelsList.size()) > 0) {
	cout << "DHit:: Plane " << fPlane->GetPlaneNumber() << ". Cluster size increased to " << _TheClusterPixelsList.size() << " at iteration " << iteration_counter+1 << endl;
	cout << "DHit:: Plane " << fPlane->GetPlaneNumber() << ". ROI list size reduced  to " << _ROIlist.size()              << " at iteration " << iteration_counter+1 << endl;
      }
      if(fDebugHit>1) cout << endl;
      
      iteration_counter++;
    }
    while(int(_FiredPixelsList.size()) > 0);
    _FiredPixelsList.clear();
    _ROIlist.clear();
    
    if(fDebugHit>1) {
      cout << "DHit:: Plane " << fPlane->GetPlaneNumber() << ". Final cluster size is " << _TheClusterPixelsList.size() << " after " << iteration_counter << " iterations!" << endl;
      cout << endl;
    }
    
    //Excluding repeated pixel. Only keeping the ones with the lowest time-stamp:
    std::vector<int> _TheClusterPixelsList_NotRepeated;
    _TheClusterPixelsList_NotRepeated.clear();
    std::vector<int> _TheClusterPixelsList_Repeated;
    _TheClusterPixelsList_Repeated.clear();
    std::vector<int> _TheClusterPixelsList_All;
    _TheClusterPixelsList_All.clear();
    DPixel *aNeighbour_tmp;
    for(int iPixClus1=0; iPixClus1 < int(_TheClusterPixelsList.size()); iPixClus1++) {
      _TheClusterPixelsList_All.push_back(_TheClusterPixelsList[iPixClus1]);
      aNeighbour = aListOfPixels->at(_TheClusterPixelsList[iPixClus1]);
      Int_t theRow = aNeighbour->GetPixelLine();
      Int_t theCol = aNeighbour->GetPixelColumn();
      int TS1      = aNeighbour->GetTimestamp();

      //Only include in the list the pixels which are not repeated
      bool IsNotRepeated = true;
      for(int iPixClus2=0; iPixClus2 < int(_TheClusterPixelsList_NotRepeated.size()); iPixClus2++) {
	aNeighbour_tmp = aListOfPixels->at(_TheClusterPixelsList_NotRepeated[iPixClus2]);
	Int_t theRow2 = aNeighbour_tmp->GetPixelLine();
        Int_t theCol2 = aNeighbour_tmp->GetPixelColumn();
	int TS2       = aNeighbour_tmp->GetTimestamp();
	
	if(theRow == theRow2 && theCol == theCol2) {
	  if(fDebugHit>1) {
	    cout << "DHit:: Plane " << fPlane->GetPlaneNumber() << ". Pixel " << iPixClus1+1 
	         << " in list at (col,lin,TS) = (" << theCol << "," << theRow << "," << TS1 << ") is repated, the other pixel is at (col,lin,TS) = (" 
	         << theCol2 << "," << theRow2 << "," << TS2 << ")"
	         << endl;
	  }
	  //Repeated pixel
	  IsNotRepeated = false;
	  //Take the pixel with the earliest time-stamp
	  if(TS2 > TS1) _TheClusterPixelsList_NotRepeated[iPixClus2] = _TheClusterPixelsList[iPixClus1];
	  break;
	}
      }
      //If the pixel is not already in list inclue it
      if(IsNotRepeated) _TheClusterPixelsList_NotRepeated.push_back(_TheClusterPixelsList[iPixClus1]);
    }
    
    //Fill list with the removed pixes
    for(int iPixClus1=0; iPixClus1 < int(_TheClusterPixelsList.size()); iPixClus1++) {
      bool IsRepeated = true;
      for(int iPixClus2=0; iPixClus2 < int(_TheClusterPixelsList_NotRepeated.size()); iPixClus2++) {
	if(_TheClusterPixelsList[iPixClus1] == _TheClusterPixelsList_NotRepeated[iPixClus2]) IsRepeated = false;
      }
      if(IsRepeated) _TheClusterPixelsList_Repeated.push_back(_TheClusterPixelsList[iPixClus1]);
    }
    //Declare all the pixels in the repeated list as found
    for(int iPix=0;iPix<int(_TheClusterPixelsList_Repeated.size());iPix++) {
      aNeighbour = aListOfPixels->at(_TheClusterPixelsList_Repeated[iPix]);
      aNeighbour->SetFound(kTRUE);
    }
    _TheClusterPixelsList_Repeated.clear();
    
    if(_TheClusterPixelsList.size() != _TheClusterPixelsList_NotRepeated.size()
       && fDebugHit>1
      ) {
      cout << endl;
      cout << "DHit:: Plane " << fPlane->GetPlaneNumber() << ". Cluster size previous to removing repeated pixels: " << _TheClusterPixelsList.size() 
           << ". Cluster size after removing repeated pixels: " << _TheClusterPixelsList_NotRepeated.size()
           << ". Removed " << _TheClusterPixelsList.size() - _TheClusterPixelsList_NotRepeated.size() << " repated pixels!!!"
           << endl;
      cout << endl;
    }
    
    //Now replace the ClusterPixelList by the one with the repeated pixels removed
    _TheClusterPixelsList.clear();
    for(int iPixClus=0; iPixClus < int(_TheClusterPixelsList_NotRepeated.size()); iPixClus++) _TheClusterPixelsList.push_back(_TheClusterPixelsList_NotRepeated[iPixClus]);
    _TheClusterPixelsList_NotRepeated.clear();

    //Calculate the position of the center of gravity:
    double COG_U = 0.0;
    double COG_V = 0.0;
    double TotH  = 0.0;
    for(int iPixClus=0; iPixClus < int(_TheClusterPixelsList.size()); iPixClus++) {
      aNeighbour = aListOfPixels->at(_TheClusterPixelsList[iPixClus]);
      double height = aNeighbour->GetPulseHeight();
      double PixelU = aNeighbour->GetPosition()(0);
      double PixelV = aNeighbour->GetPosition()(1);
      
      COG_U += PixelU*height;
      COG_V += PixelV*height;
      TotH  += height;
    }
    COG_U /= TotH;
    COG_V /= TotH;
    
    //Order the pixels depending of analog or digital output
    for(int iii=2;iii<=int(_TheClusterPixelsList.size());iii++) {
      for(int jjj=0;jjj<=int(_TheClusterPixelsList.size())-iii;jjj++) {
	
	if(fPlane->GetAnalysisMode() == 2) {
	  //In case of analog output arrange the pixels according to charge,
	  //from highest (seed pixels) to lowest
	  
	  double charge_jjj   = aListOfPixels->at(_TheClusterPixelsList[jjj])->GetPulseHeight();
	  double charge_jjjp1 = aListOfPixels->at(_TheClusterPixelsList[jjj])->GetPulseHeight();
	  if(charge_jjj < charge_jjjp1) {
	    int aux_idx                  = _TheClusterPixelsList[jjj];
	    _TheClusterPixelsList[jjj]   = _TheClusterPixelsList[jjj+1];
	    _TheClusterPixelsList[jjj+1] = aux_idx;
	  }
	  
	}
	else if(fPlane->GetAnalysisMode() == 3) {
	  //In case of digital output arrange the pixels according to their distance to the 
	  //center of gravitiy, from closest (seed pixels) to farest
	  
	  double PixelU,PixelV;
	  
	  aNeighbour = aListOfPixels->at(_TheClusterPixelsList[jjj]);
          PixelU = aNeighbour->GetPosition()(0);
          PixelV = aNeighbour->GetPosition()(1);
          double dist_jjj   = sqrt(pow(COG_U - PixelU,2) + pow(COG_V - PixelV,2));
	  
	  aNeighbour = aListOfPixels->at(_TheClusterPixelsList[jjj+1]);
          PixelU = aNeighbour->GetPosition()(0);
          PixelV = aNeighbour->GetPosition()(1);
          double dist_jjjp1 = sqrt(pow(COG_U - PixelU,2) + pow(COG_V - PixelV,2));
	  
	  if(dist_jjj > dist_jjjp1) {
	    int aux_idx                  = _TheClusterPixelsList[jjj];
	    _TheClusterPixelsList[jjj]   = _TheClusterPixelsList[jjj+1];
	    _TheClusterPixelsList[jjj+1] = aux_idx;
	  }
	}
        
      }
    }
    
    //Hard-coded limit in the number of pixel in a cluster
    //This is a way to identify cluster due to low-momentum heavy ions from fragmentation
    //This should avoid crash of the code when such an event happends. This happends mainly 
    //for high fluxes (high trigger rates > 20kHz).
    //int TheNpixelsLimit = 100;
    int TheNpixelsLimit = MaxClusterSize;
    if(int(_TheClusterPixelsList.size()) >= TheNpixelsLimit) {
      IsBigCluster = true;
      cout << endl;
      double TheCol,TheLin;
      fPlane->ComputeStripPosition_UVToColRow(COG_U,COG_V,TheCol,TheLin);
      cout << "  DHit: Analyse method found too many pixels in cluster, N-pixels = " << _TheClusterPixelsList.size() << " (Limit is set to config file value MaxNStrips " << TheNpixelsLimit 
	   << "), for plane " << fPlane->GetPlaneNumber() << ". Seed pixel location is (col,lin) = (" << TheCol << "," << TheLin << ")" << endl;
      cout << "  DHit: It is likely a cluster of low-momentum heavy ion from fragmentation. Excluding pixels from further analysis" << endl;

      for(int iPix=0;iPix<int(_TheClusterPixelsList_All.size());iPix++) {
	aNeighbour = aListOfPixels->at(_TheClusterPixelsList_All[iPix]);
	if( fDebugHit>1) {
	  cout << "  DHit:: idx = " << _TheClusterPixelsList_All[iPix] << ", (col,lin) = ("
	       << aNeighbour->GetPixelColumn() << "," << aNeighbour->GetPixelLine() << ")"
	       << endl;
	}
	aNeighbour->SetFound(kTRUE);
      }
      _TheClusterPixelsList.clear();
      _TheClusterPixelsList_All.clear();
      
      return false;
    }
    _TheClusterPixelsList_All.clear();

    tClusterAreaNoise     = 0.0;
    fClusterAreaPulseSum  = 0.0;
    fClusterPulseSum      = 0.0;
    fClusterNoiseAverage  = 0.0;
    fStripsInClusterFound = int(_TheClusterPixelsList.size());
    
    //Fill up the tables with the quantities of the cluster
    for(int iPix=0;iPix<int(_TheClusterPixelsList.size());iPix++) {
      aNeighbour = aListOfPixels->at(_TheClusterPixelsList[iPix]);
      if(iPix == 0) {
	fPSeed      = aNeighbour;
	fSeedU      = fPSeed->GetPosition()(0);
	fSeedV      = fPSeed->GetPosition()(1); 
	fIndexSeed  = fPSeed->GetPixelIndex();
  
	seedRow     = fPSeed->GetPixelLine(); 
	seedCol     = fPSeed->GetPixelColumn();
	fPSeed->SetFound(kTRUE);

	tPixelIndexList[0]   = _TheClusterPixelsList[0];
	fStripIndexArray[0]  = 0;                               // place seed in position 0 of neighbour
	fStripIndex[0]       = fPSeed->GetPixelIndex();         // store seed index, JB 2009/05/12
	fStripPulseHeight[0] = fPSeed->GetPulseHeight();        // store seed pulseheight
	fStripNoise[0]       = fPSeed->GetNoise();              // store seed noise  //YV 27/11/09
	fSNseed              = fPSeed->GetPulseHeightToNoise(); // JB 2013/11/08
	fSeedPulseHeight     = fPSeed->GetPulseHeight();        // Note that seed might change
	fSeedNoise           = fPSeed->GetNoise();
	fStripDistanceU[0] = 0.;                                // distance seed to seed = 0.
	fStripDistanceV[0] = 0.;                                // distance seed to seed = 0.
	fStoNover2 = 0;                                         // MB/12/11/2010 
      }
      else {
	fClusterAreaPulseSum          += aNeighbour->GetPulseHeight();
	tClusterAreaNoise             +=(aNeighbour->GetNoise())*(aNeighbour->GetNoise());
	fClusterNoiseAverage          += (aNeighbour->GetNoise())*(aNeighbour->GetNoise());
	fClusterPulseSum              += aNeighbour->GetPulseHeight();
	tPixelIndexList[iPix]          = _TheClusterPixelsList[iPix]; // store the index in the original list of pixels
	fStripIndexArray[iPix]         = iPix; // put neighbour index in array (not index of full pixel list)
	fStripIndex[iPix]              = aNeighbour->GetPixelIndex(); // this is the index in the matrix, JB 2009/05/12
	fStripPulseHeight[iPix]        = aNeighbour->GetPulseHeight();
	fStripNoise[iPix]              = aNeighbour->GetNoise();             //YV 27/11/09
	fStripDistanceU[iPix]          = fPSeed->DistanceU(aNeighbour->GetPosition());
	fStripDistanceV[iPix]          = fPSeed->DistanceV(aNeighbour->GetPosition());
	aNeighbour->SetFound(kTRUE);

	if( fDebugHit>1) printf("  neighbour %d at index %d (r%d, c%d) with pulse %.1f and distance %.1f(u=%.1f, v=%.1f) (<%.1f(u=%.1f, v=%.1f)) found!\n", 
				iPix,
				aNeighbour->GetPixelIndex(),
				aNeighbour->GetPixelLine(),
				aNeighbour->GetPixelColumn(),
				aNeighbour->GetPulseHeight(),
				fPSeed->Distance( *aNeighbour),
				fPSeed->DistanceU(aNeighbour->GetPosition()),
				fPSeed->DistanceV(aNeighbour->GetPosition()),
				fClusterLimit->Length(),
				(*fClusterLimit)(0),
				(*fClusterLimit)(1));
      }
    }
    _TheClusterPixelsList.clear();

  } //end if pixel analog or binary readout


  if(fDebugHit>2) printf("  DHit:Analyse found %d valid neighbours\n", fStripsInClusterFound);


  //=============
  // order pixels
  //=============
  Int_t *nOfneighbours = new Int_t[fStripsInClusterFound];

  // For analog readout
  // re-order pixels from the highest charge to the lowest
  if( fPlane->GetAnalysisMode() == 2 ) { // if analog readout

    DGlobalTools aTool; // JB 2011/07/18
    aTool.OrderIndexDes(&fStripIndexArray[0], &fStripPulseHeight[0], fStripsInClusterFound);
    if( fDebugHit>2) { // display result of pixel ordering, JB 2009/05/12
      cout << "  DHit:Analyse  ordered list of " << fStripsInClusterFound << " pixels in candidate hit" << endl;
      for( Int_t iPix=0; iPix<fStripsInClusterFound ; iPix++) { // loop on pixels in candidate hit
        printf( "     pix %2d, neighbour %7d,  pulse %f\n", iPix, fStripIndexArray[iPix], fStripPulseHeight[fStripIndexArray[iPix]]);
      } // end loop on pixels
    }

  } // end if analog readout
  else if( fPlane->GetAnalysisMode() == 3 ) { // if binary readout
    // For binary readout, update the seed information
    
    // swap previous info at index 0 with the one at index iSeed
    // so that all arrays have seed info at index 0
    Int_t iTemp;
    Float_t temp;
    iTemp                    = tPixelIndexList[iSeed];
    tPixelIndexList[iSeed]   = tPixelIndexList[0];
    tPixelIndexList[0]       = iTemp;
    iTemp                    = fStripIndexArray[iSeed];
    fStripIndexArray[iSeed]  = fStripIndexArray[0];
    fStripIndexArray[0]      = iTemp;
    iTemp                    = fStripIndex[iSeed];
    fStripIndex[iSeed]       = fStripIndex[0];
    fStripIndex[0]           = iTemp;
    temp                     = fStripPulseHeight[iSeed];
    fStripPulseHeight[iSeed] = fStripPulseHeight[0];
    fStripPulseHeight[0]     = temp;

    // need to update some variables 
    // to take into account the new seed definition
    fPSeed = aListOfPixels->at( tPixelIndexList[ 0] );
    fSeedU      = fPSeed->GetPosition()(0);
    fSeedV      = fPSeed->GetPosition()(1); 
    fIndexSeed  = fPSeed->GetPixelIndex();
    for( Int_t iPix=1; iPix<fStripsInClusterFound ; iPix++) { // loop on pixels in candidate hit
      aNeighbour = aListOfPixels->at( tPixelIndexList[ iPix]);
      fStripDistanceU[iPix]   = fPSeed->DistanceU(aNeighbour->GetPosition());
      fStripDistanceV[iPix]   = fPSeed->DistanceV(aNeighbour->GetPosition());
    }

    if( fDebugHit>2 ) {
      printf("  DHit:Analyse  seed is now pixel %d in list, index %d, with %d direct neighbours\n", tPixelIndexList[ 0], fIndexSeed, nOfneighbours[ iSeed]);
    }
    
  } // end if binary readout

  else {
    printf("DHit::Analyse_dynamic WARNING analysis mode %d UNKNOWN !!\n", fPlane->GetAnalysisMode() );
    return false;
  }
  
  delete[] nOfneighbours; // remove memory leaks, BH 2013/08/21
  
  //==============
  // additional properties
  //==============  
  // the number of strips in the cluster and the cluster charge sum are now known
  // Depending wether the noise is calculated or not, 
  // the SN ratio is a real ratio or simply the charge
  // JB 2010/10/05 and 2010/10/15 to separately compute Cluster and Area values

  tClusterAreaNoise  = sqrt(tClusterAreaNoise);
  
  if (fClusterNoiseAverage > 0.5) {
    fClusterSignalToNoise  = fClusterPulseSum     / sqrt(fClusterNoiseAverage);
  }
  else {
    fClusterSignalToNoise  = fClusterPulseSum;
  }
  if (tClusterAreaNoise > 0.5) {
    fSNneighbour           = fClusterAreaPulseSum / tClusterAreaNoise;
  }
  else {
    fSNneighbour           = fClusterAreaPulseSum;
  }

  // Number of pixels with 2*pitch of the seed with S/N>2.
  // MB/12/11/2010
  if( fPlane->GetAnalysisMode() <= 2 ) {
    for( Int_t iPix=0; iPix<fStripsInClusterFound; iPix++) { // loop on pixels in candidate hit
      aNeighbour = aListOfPixels->at(iPix);
      if ( fPSeed->Distance( *aNeighbour) <= 2*(*fStripPitch)(0) && fPSeed->GetPulseHeightToNoise() >= 2 ){
	fStoNover2++;
      }
      if(fDebugHit>1)  cout<<"  DHit:Analyse  StoNover2  = "<<fStoNover2<<"   fPSeed->Distance( *aNeighbour)   = "<<fPSeed->Distance( *aNeighbour)<<"  fPSeed->GetFound() = "<<  fPSeed->Found()<<endl;
    } // end loop on pixels in candidate hit
  }  

  fTimestamp = fPSeed->GetTimestamp(); // JB 2015/05/25
  
  if(fDebugHit>2) printf("  DHit:Analyse  potential hit has : PulseSum=%f, areaPulseSum=%f, noise=%f, areaNoise=%f, clusterSignalToNoise=%f, areaSignalToNoise=%f <?> cut %f, # pixels=%d within? [%d, %d], stover2=%d, time=%d\n", fClusterPulseSum, fClusterAreaPulseSum, fClusterNoiseAverage, tClusterAreaNoise, fClusterSignalToNoise, fSNneighbour, fCut->GetNeighbourPulseHeightToNoise(), fStripsInClusterFound, fCut->GetStripsInClusterMin(), fCut->GetStripsInClusterMax(), fStoNover2, fTimestamp);


  //===============
  // Compute hit position
  //===============

  // start to  calculate the exact hit position, which is in first order the seed strip position  
  (*fPositionHit)(0)    = (fPSeed->GetPosition())(0);
  (*fPositionHit)(1)    = (fPSeed->GetPosition())(1);
  (*fPositionHit)(2)    = (fPSeed->GetPosition())(2);

  // This is the container (3D distance) for the correction to be computed with different algorithm
  DR3 tCorrection ; tCorrection.SetValue(0.,0.,0.);
  DR3 tCorTemp ;

  //===============
  //--- for PIXEL planes
  //
  if( fPlane->GetAnalysisMode() >= 2 ) { // if pixel mode

    // Cut on the charge or on the charge/noise (See above)
    // Cut on # pixels in hit added, JB 2009/08/30

    if( fPlane->GetAnalysisMode() != 3) { //if analog readout
      if( fSNneighbour >= fCut->GetNeighbourPulseHeightToNoise() 
	  && fCut->GetStripsInClusterMin() <= fStripsInClusterFound
	  && fStripsInClusterFound <= fCut->GetStripsInClusterMax()
	  //&& fStoNover2 >=4 // MB 2010/11/12
	  ){
	valid = kTRUE;
      }else{
	valid = kFALSE;
      }
    }
    else { //if digital readout
      if( fSNneighbour >= fCut->GetNeighbourPulseHeightToNoise() 
	  && fCut->GetStripsInClusterMin() <= fStripsInClusterFound
	  ){
	valid = kTRUE;
      }else{
	valid = kFALSE;
      }
    }

    // now compute position correction wrt seed position depending on the required algorithm
    if( valid) {

      if(fDebugHit>2) printf("  DHit:Analyse  hit is valid!\n");

      //==================
      //Center of gravity with all the neighbours
      // pay attention that pixels are ordered geometricaly from one corner to the other in the fStripDistanceU/V arrays

      tCorrection.SetValue( 0., 0., 0.);
      Float_t tClusterPulseSum = 0.;
      for( Int_t iStripIndex=0; iStripIndex < fStripsInClusterFound; iStripIndex++ ) {
	// Computation without the fStripDistance array, MG 2010/06/28
	aNeighbour = aListOfPixels->at( tPixelIndexList[ iStripIndex]);
	tCorTemp.SetValue( aNeighbour->GetPosition()(0)*aNeighbour->GetPulseHeight(),
			   aNeighbour->GetPosition()(1)*aNeighbour->GetPulseHeight(),
			   0);
	tCorrection += tCorTemp;
	tClusterPulseSum += aNeighbour->GetPulseHeight();
      }
      *fPositionHitCG = tCorrection/tClusterPulseSum;
       fIfMonteCarlo = fPSeed->IfMonteCarlo();
       if(fIfMonteCarlo==1) _monteCarloInfo = fPSeed->GetMonteCarloInfo();

      //==================
      //Center of gravity restricted to a 3x3 cluster

      tCorrection.SetValue( 0., 0., 0.);
      Float_t tCluster33PulseSum = 0.;
      for( Int_t iStripIndex=0; iStripIndex < fStripsInClusterFound; iStripIndex++ ) {
	if(    fabs(fStripDistanceU[iStripIndex])<=fPSeed->GetSize()(0)
	    && fabs(fStripDistanceV[iStripIndex])<=fPSeed->GetSize()(1)
	    ) { // check neighbours is at most at 1 pitch from seed
	  // Computation without the fStripDistance array, MG 2010/06/28
	  aNeighbour = aListOfPixels->at( tPixelIndexList[ iStripIndex]);
	  tCorTemp.SetValue( aNeighbour->GetPosition()(0)*aNeighbour->GetPulseHeight(),
			     aNeighbour->GetPosition()(1)*aNeighbour->GetPulseHeight(),
			     0);
	  tCorrection += tCorTemp;
	  tClusterPulseSum += aNeighbour->GetPulseHeight();
 	}
      }
      *fPositionHitCG33 = tCorrection/tCluster33PulseSum;


      //==================
      //Center of gravity restricted 2x2 neighbours with the highest charge, JB Nov 2007
      // 2x2 includes the seed

      // The index of the neighbours with the highest charges are ordered in fStripIndexArray

      tCorrection.SetValue( 0., 0., 0.);
      Float_t tCluster22PulseSum = 0.;
      Int_t tOrderedIndex;
      for( Int_t iStripIndex=0; iStripIndex < TMath::Min( 4, fStripsInClusterFound); iStripIndex++ ) { // limit changed for sparsified data, JB 2009/09/01
	tOrderedIndex = fStripIndexArray[iStripIndex];
	tCorTemp.SetValue( (fPSeed->GetPosition()(0)-fStripDistanceU[tOrderedIndex])*fStripPulseHeight[tOrderedIndex],
			   (fPSeed->GetPosition()(1)-fStripDistanceV[tOrderedIndex])*fStripPulseHeight[tOrderedIndex],
			   0);
	tCorrection += tCorTemp;
	tCluster22PulseSum += fStripPulseHeight[tOrderedIndex];
      }
      *fPositionHitCG22 = tCorrection/tCluster22PulseSum; 


      //==================
      //Eta algorithm 3x3
      // correct the 3x3 center of gravity (tCorrection) using the integral histogram of the charge density
      // the eta position is the Y value of the bin with X value = tCorrection
      // if the tCorrection from CoG is out of the X axis range, do not correct

      if ( tDigital ){ // check info file was there and Eta is available
	if(fDebugHit>1 && (fPositionAlgorithm==2 || fPositionAlgorithm==22)) printf("  DHit.cxx:Analyse  Digital or CoG response only set for plane %d (forgot about etaXXX.root file?)\n",fPlane->GetPlaneNumber());
      }
      else {
    
	TH1  *tEtaIntU = fPlane->GetEtaIntU();
	TH1  *tEtaIntV = fPlane->GetEtaIntV();
	Int_t iBinU    = tEtaIntU->FindBin( (*fPositionHitCG33-*fPositionHit)(0) );
	Int_t iBinV    = tEtaIntV->FindBin( (*fPositionHitCG33-*fPositionHit)(1) );
	Double_t corrU=0., corrV=0.;
	
	// correct wrt CoG if possible
	if( 1<=iBinU && iBinU<=tEtaIntU->GetNbinsX() ) corrU = tEtaIntU->GetBinContent( iBinU);
	if( 1<=iBinV && iBinV<=tEtaIntV->GetNbinsX() ) corrV = tEtaIntV->GetBinContent( iBinV);
	tCorrection.SetValue( corrU, corrV, 0.);
	if( fDebugHit>1) printf("    Eta correction/digital is (%.1f, %.1f)\n", corrU, corrV);
	*fPositionHitEta = *fPositionHit + tCorrection;

      //==================
      //Eta algorithm 2x2
      // correct the 2x2 center of gravity (tCorrection) using the integral histogram of the charge density
      // the eta position is the Y value of the bin with X value = tCorrection
      // if the tCorrection from CoG is out of the X axis range, do not correct
    
	tEtaIntU = fPlane->GetEtaIntU2();
	tEtaIntV = fPlane->GetEtaIntV2();
	iBinU    = tEtaIntU->FindBin( (*fPositionHitCG22-*fPositionHit)(0) );
	iBinV    = tEtaIntV->FindBin( (*fPositionHitCG22-*fPositionHit)(1) );
	corrU = corrV = 0.;
	
	// correct wrt CoG if possible
	if( 1<=iBinU && iBinU<=tEtaIntU->GetNbinsX() ) corrU = tEtaIntU->GetBinContent( iBinU);
	if( 1<=iBinV && iBinV<=tEtaIntV->GetNbinsX() ) corrV = tEtaIntV->GetBinContent( iBinV);
	tCorrection.SetValue( corrU, corrV, 0.);
	
	*fPositionHitEta22 = *fPositionHit + tCorrection;

      } // end if tDigital

      //==================
      //Analog head-tail algorithm
      // Algorithm used for large incident angle when seed contains little info on position
      // Relies on the extreme pixels on the left and right
      // tCorrection.SetValue( (fStripDistanceU[iLeft]+fStripDistanceU[iRight])/2., , 0.);
      // *fPositionhitAHT = *fPositionHit - tCorrection;

      //==================
      // Now choose which position is stored
      if( fPositionAlgorithm==0 ) { // keep Seed position
      }
      else if( fPositionAlgorithm==1 ) { // Center of Gravity
	*fPositionHit = *fPositionHitCG;
      }
      else if( fPositionAlgorithm==11 ) { // Center of gravity on 3x3
	*fPositionHit = *fPositionHitCG33;
      }
      else if( fPositionAlgorithm==12 ) { // Center of gravity on 2x2
	*fPositionHit = *fPositionHitCG22;
	}
      else if( fPositionAlgorithm==2 ) { // Eta corrected from CoG 3x3
	*fPositionHit = *fPositionHitEta;
      }
      else if( fPositionAlgorithm==22 ) { // Eta corrected from CoG 2x2
	*fPositionHit = *fPositionHitEta22;
      }
      else {
	printf("-*-*- WARNING: algorithm %d for position unknown, taking digital position\n", fPositionAlgorithm);
      }
      if(fDebugHit>1) printf("  DHit.cxx:Analyse pos algo %d requested, seed[%d-%d]=(%.1f, %.1f), CG(%.1f, %.1f), CG33(%.1f, %.1f), eta(%.1f, %.1f), eta22(%.1f, %.1f)\n", fPositionAlgorithm, fPSeed->GetPixelLine(), fPSeed->GetPixelColumn(), fSeedU, fSeedV, (*fPositionHitCG)(0), (*fPositionHitCG)(1), (*fPositionHitCG33)(0), (*fPositionHitCG33)(1), (*fPositionHitEta)(0), (*fPositionHitEta)(1), (*fPositionHitEta22)(0), (*fPositionHitEta22)(1));

    } //end if valid

  } // end if PIXEL mode


  // Release the neighbouring Pixels associated to the seed if the pixel is finally not selected 
  // only the tested seed (Index=0) is not released
  // JB 2009/05/10
  if( !valid ) { 
    if(fDebugHit>1) printf("  DHit.cxx:Analyse  hit NOT selected, releasing %d pixels\n", fStripsInClusterFound-1);
    for (Int_t iNeigh = 1; iNeigh < fStripsInClusterFound; iNeigh++){ // loop over associated pixels
      if(fDebugHit>2) printf("  DHit.cxx:Analyse    releasing pixel %d at index %d\n", iNeigh, fStripIndexArray[iNeigh]);
      aListOfPixels->at(fStripIndexArray[iNeigh])->SetFound(kFALSE);
    } // end loop over associated pixels
  } // end if !valid

  else if(fDebugHit>1) {
    printf("        hit selected with %d pixels\n", fStripsInClusterFound);
  }

  return valid;
}

//______________________________________________________________________________
//

DStrip* DHit::GetMinor(Int_t aSk) 
{
    // GetMinor(0) is fSeed, GetMinor(1) is strip with lower pulseheight 
    // than fSeed

  if (aSk < fSeed->GetNeighbourCount() ) {
    return fSeed->GetNeighbour(fStripIndexArray[aSk]);
  }
  else {
        printf("DHit, Request to non existing minor strip %d because only %d neighbours, returning seed strip\n", aSk, fSeed->GetNeighbourCount());
    return fSeed;
  }
}
//______________________________________________________________________________
//
DR3 DHit::GetMonteCarloPosition()
{
  if(_monteCarloInfo.size()>=3 && fIfMonteCarlo==1) return DR3(_monteCarloInfo[0], _monteCarloInfo[1], _monteCarloInfo[2]);
  else {
    std::cout<<"Monte Carlo Position Disable ! --> MC Position Set to (0,0,0)"<<std::endl;
    return DR3(0,0,0);
  }
}
//______________________________________________________________________________
//
void DHit::SetHitPosition(DR3 aPosition,
			  DR3 aResolution)
{

  // AP, 2015/03/11
  // Function to set the hit position from used value:

  fFound = kFALSE;
  
  if(fHitNumber>0) {
    fPositionHit->SetValue(aPosition(0),aPosition(1),aPosition(2));
    fPositionHitCG->SetValue(aPosition(0),aPosition(1),aPosition(2));
    fPositionHitEta->SetValue(aPosition(0),aPosition(1),aPosition(2));
    fPositionHitCG33->SetValue(aPosition(0),aPosition(1),aPosition(2));
    fPositionHitCG22->SetValue(aPosition(0),aPosition(1),aPosition(2));
    fPositionHitEta22->SetValue(aPosition(0),aPosition(1),aPosition(2));
    fResolutionHit->SetValue(aResolution(0),aResolution(1),aResolution(2));
  }

  return;
}
//______________________________________________________________________________
//
Int_t DHit::Compare( const TObject * obj) const { // QL 04/06/2016
  // QL 04/06/2016
  // to enable Sort method in a TList
  // Sorted by comparing zPosition of the plane center 

  DPlane *objPlane = ((DHit*)obj)->GetPlane();
  return fPlane->Compare( objPlane);
}
//______________________________________________________________________________
//
void  DHit::DoMCA(void)
{
  //AP 2017/05/09
  //This function performs the Main component analysis of the cluster
  
  float meanU     = 0.0;
  float meanV     = 0.0;
  float meanU2    = 0.0;
  float meanV2    = 0.0;
  float meanUV    = 0.0;
  float TotCharge = 0.0;
  //cout << "  N pixels in hit = " << GetStripsInCluster() << endl;
  for(int ipixInHit=0; ipixInHit < GetStripsInCluster(); ipixInHit++) {
    
    double  PixelU, PixelV, height;
    if(fPlane->GetReadout() > 100) {
      std::vector<DPixel*> *aList = GetPlane()->GetListOfPixels();
      PixelU = aList->at(GetIndexInOriginalList(ipixInHit))->GetPosition()(0);
      PixelV = aList->at(GetIndexInOriginalList(ipixInHit))->GetPosition()(1);
      height = aList->at(GetIndexInOriginalList(ipixInHit))->GetPulseHeight();
    }
    else {
      int idex_pixel   = GetIndex(ipixInHit);
      int col = idex_pixel%fPlane->GetStripsNu();
      int row = idex_pixel/fPlane->GetStripsNu();
      
      double w;
      fPlane->ComputeStripPosition(col,row, PixelU, PixelV, w);
      height  = GetPulseHeight(ipixInHit);
    }
    //height = TMath::Abs(height);
    if(height < 0.0) height = 0.0; 
    
    meanU     += PixelU*height;
    meanU2    += pow(PixelU,2)*height;
    meanV     += PixelV*height;
    meanV2    += pow(PixelV,2)*height;
    meanUV    += PixelU*PixelV*height;
    TotCharge += height;
  }
    
  if(TotCharge <= 1.0e-3) return;
  
  meanU  /= TotCharge;
  meanU2 /= TotCharge;
  meanV  /= TotCharge;
  meanV2 /= TotCharge;
  meanUV /= TotCharge;
  
  meanU2 -= pow(meanU,2);
  meanV2 -= pow(meanV,2);
  meanUV -= meanU*meanV;
  
  int Nphi = 50;
  double Rphi[2];
  Rphi[0] = 0.0;
  Rphi[1] = TMath::Pi();
  fNormMaxStd = -1.0e+20;
  for(int iphi=0;iphi<Nphi+1;iphi++) {
    float phi_val    = Rphi[0] + iphi*(Rphi[1] - Rphi[0])/Nphi;
    float cos2_phi    = pow(TMath::Cos(phi_val),2);
    float sin2_phi    = pow(TMath::Sin(phi_val),2);
    float sin_2phi    = TMath::Sin(2.0*phi_val);
    
    float RMS_Uprime = meanU2*cos2_phi + meanV2*sin2_phi + meanUV*sin_2phi;
    float RMS_Vprime = meanU2*sin2_phi + meanV2*cos2_phi - meanUV*sin_2phi;
    
    if(fNormMaxStd < RMS_Uprime) {
      fNormMaxStd   = RMS_Uprime;
      fNormMinStd   = RMS_Vprime;
      fMainAxisPhi = phi_val;
    }
  }
  
  if(fNormMaxStd < 0.0 || fNormMinStd < 0.0) {
    fNormMaxStd  = -1000;
    fNormMinStd  = -1000;
    fMainAxisPhi = -1000;
  }
  else {
    fNormMaxStd = sqrt(fNormMaxStd);
    fNormMinStd = sqrt(fNormMinStd);
  }
  
  return;
  
}
//______________________________________________________________________________
//
void  DHit::CalcHitLimits(void)
{
  
  fColMin =  2*fPlane->GetStripsNu();
  fColMax = -1;
  fRowMin =  2*fPlane->GetStripsNv();
  fRowMax = -1;
  
  for(int ipix=0;ipix < GetStripsInCluster();ipix++) {
    int col,row;
    if(fPlane->GetReadout() > 100) {
      std::vector<DPixel*> *aList = fPlane->GetListOfPixels();
      col = aList->at(GetIndexInOriginalList(ipix))->GetPixelColumn();
      row = aList->at(GetIndexInOriginalList(ipix))->GetPixelLine();
    }
    else {
      int idex_pixel   = GetIndex(ipix);
      col = idex_pixel%fPlane->GetStripsNu();
      row = idex_pixel/fPlane->GetStripsNu();
    }
    
    if(fColMin > col) fColMin = col;
    if(fColMax < col) fColMax = col;
    if(fRowMin > row) fRowMin = row;
    if(fRowMax < row) fRowMax = row;
  }
  
  return;
  
}
//______________________________________________________________________________
//
void DHit::Print(const Option_t*) const
{ 
  fPlane->Print();
}

//______________________________________________________________________________
//  
