// @(#)maf/dtools:$Name:  $:$Id: DCut.cxx,v.2 2005/10/02 18:03:46 sha Exp $
// Author: ?
// Last Modified: JB 2009/08/30

  /////////////////////////////////////////////////////////////
  //                                                         //
  // Class Description of DCut                               //
  //                                                         //   
  ////////////////////////////////////////////////////////////

#include "DCut.h"
#include "DSetup.h"
#include "DPlane.h"
#include "DR3.h"


ClassImp(DCut) // Description of Cuts  

//______________________________________________________________________________
//  
DCut::DCut()
{
// Default DCut ctor.

}

//______________________________________________________________________________
//  
DCut::DCut(DPlane& aPlane)
{

  // DCut User constructor
  //
  // Modified by JB, 2009/05/07 to separate STRIPS and PIXELS plane
  // Modified by JB, 2009/08/30 to add limits in # pixels and in area of a cluster
  // Modified by JB, 2013/08/29 to add noise limits
  // Modified by VR, 2014/07/12 calcul of fStripsInClusterArea more coherent with DHit::Analyse 
  //                        if ( !aNeighbour->Found() && fPSeed->Distance( *aNeighbour) < fClusterLimit->Length() ...
  // Modified by VR, 2014/07/16 Add field ClusterLimitRadius for DHit::Analyse_2_cog()

  fDebugCut = aPlane.GetDebug(); 
  if (fDebugCut)  cout << " -+-+- Defining Cut ..." << endl ;

  fPlaneNumber                 =  aPlane.GetPlaneNumber();
  if (fDebugCut) printf("  for plane number= %d\n",fPlaneNumber);
  fc                           = &aPlane.GetSetup();
  fSeedPulseHeightToNoise      =  fc->GetPlanePar(fPlaneNumber).ThreshSeedSN;
  fNeighbourPulseHeightToNoise =  fc->GetPlanePar(fPlaneNumber).ThreshNeighbourSN;
  fMaximalNoise                =  fc->GetPlanePar(fPlaneNumber).MaximalNoise;
  fMinimalNoise                =  fc->GetPlanePar(fPlaneNumber).MinimalNoise;
//  fMaximalNoise = 100.; // temporary fixed value, JB 2013/08/29
  if (fDebugCut) {
    cout << "  fSeedPulseHeightToNoise=" << fSeedPulseHeightToNoise << endl;
    cout << "  fNeighbourPulseHeightToNoise=" << fNeighbourPulseHeightToNoise << endl;
    cout << "  fMaximalNoise=" << fMaximalNoise << endl;
  }

  fClusterLimit                = &fc->GetPlanePar(fPlaneNumber).ClusterLimit;

  DR3 tPitch                   =  fc->GetPlanePar(fPlaneNumber).Pitch;

  if( aPlane.GetAnalysisMode()==1){ // STRIPS
    if (fDebugCut) cout << "  fClusterLimit=" << (*fClusterLimit)(0)  << endl;
    fStripsInClusterArea          = (Int_t)( (*fClusterLimit)(0) * 2 / tPitch(0)) +1 ; // changed, JB 2012/08/21, was -2 ???
  }
  else // PIXELS
  {
    if (aPlane.GetHitFinder() != 2)
    {
      if (fDebugCut) cout << "  fClusterLimit=" << (*fClusterLimit)(0) << ", " << (*fClusterLimit)(1) << endl;
    // slightly modified JB 2009/09/23
    
      fStripsInClusterArea = ceil( ((*fClusterLimit)(0)*2./tPitch(0)+1) * ((*fClusterLimit)(1)*2./tPitch(1)+1) );
//      fStripsInClusterArea = (Int_t)( ((*fClusterLimit)(0)*2./tPitch(0)) * ((*fClusterLimit)(1)*2./tPitch(1)) );//VR 2014/07/12
    
      if (fDebugCut) cout << "  fStripsInClusterArea= " << fStripsInClusterArea << endl; 

      fStripsInClusterMin = fc->GetPlanePar(fPlaneNumber).MinNStrips; // JB 2009/08/30
      fStripsInClusterMax = fc->GetPlanePar(fPlaneNumber).MaxNStrips; // JB 2009/08/30
      if (fDebugCut) cout << "  # strips in cluster limits = " << fStripsInClusterMin << " to " << fStripsInClusterMax << endl; 

      // Check that the Cluster-Limit and the MaxNStrips-Limit match.
      // If not, update the MaxNStrips with the Cluster-Limit.
      // JB 2012/05/09
      if( fStripsInClusterMax != fStripsInClusterArea ) {
        fStripsInClusterMax = fStripsInClusterArea;
        printf( "WARNING, your configuration file has incoherent cluster maximum size (%d)  directives for plane %d\n   ---> It is now fixed to %d strips\n\n", fc->GetPlanePar(fPlaneNumber).MaxNStrips, aPlane.GetPlaneNumber(), fStripsInClusterArea);
      }
      // Check that the MinNStrips-Limit is coherent
      // If not, update the MinNStrips
      // VR 2014/07/16
      if( fStripsInClusterMin < 1 ) {
        fStripsInClusterMin = 1;
        printf( "WARNING, your configuration file has incoherent cluster minimum size (%d)  directives for plane %d\n   ---> It is now fixed to %d strips\n\n", fc->GetPlanePar(fPlaneNumber).MinNStrips, aPlane.GetPlaneNumber(), fStripsInClusterMin);
      }
    }
    if (aPlane.GetHitFinder() == 2) // VR 2014/07/16 Add the DHit::Analyse_2_cog() with Min/MaxNStrips and ClusterLimitRadius parameters
    {    
      fClusterLimitRadius = fc->GetPlanePar(fPlaneNumber).ClusterLimitRadius;
      if (fc->GetPlanePar(fPlaneNumber).MinNStrips < 1)
      {
        fStripsInClusterMin = 1;
        printf( "WARNING, your configuration file has incoherent cluster minimum size (%d)  directives for plane %d\n   ---> It is now fixed to %d strips\n\n", fc->GetPlanePar(fPlaneNumber).MinNStrips, aPlane.GetPlaneNumber(), fStripsInClusterMin);
      }
      else fStripsInClusterMin = fc->GetPlanePar(fPlaneNumber).MinNStrips;
      if (fc->GetPlanePar(fPlaneNumber).MaxNStrips < 1)
      {
        fStripsInClusterMax = 100; // to not be a limit
        printf( "WARNING, your configuration file has incoherent cluster maximum size (%d)  directives for plane %d\n   ---> It is now fixed to %d strips to not limit cluster size\n\n", fc->GetPlanePar(fPlaneNumber).MaxNStrips, aPlane.GetPlaneNumber(), fStripsInClusterMax);
      }
      else fStripsInClusterMax = fc->GetPlanePar(fPlaneNumber).MaxNStrips;
      
      

      
      if (fDebugCut) cout << "  # pixels in cluster min = " << fStripsInClusterMin << " and search radius is  " << fClusterLimitRadius << endl; 
    }
  }
    
  
  // the threshold on neighbourstrips should depend on the tilting angle between the
  // track and the detector plane. 
  if (fDebugCut)  cout << " ... Defining Cut done -+-+- " << endl ;
}

//______________________________________________________________________________
//  
DCut::~DCut()
{
  // Default DCut destructor
}

//______________________________________________________________________________
//  
void DCut::SetSeedPulseHeightToNoise(Float_t aPulseHeightToNoise)
{
  fSeedPulseHeightToNoise = aPulseHeightToNoise;
}

//______________________________________________________________________________
//  
void DCut::SetNeighbourPulseHeightToNoise(Float_t aPulseHeightToNoise)
{
  fNeighbourPulseHeightToNoise = aPulseHeightToNoise;
}
