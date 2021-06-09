// @(#)maf/dtools:$Name:  $:$Id: DCut.h,v.2 2005/10/02 18:03:46 sha Exp $
// Author: ?


#ifndef _DCut_included_
#define _DCut_included_


  /////////////////////////////////////////////////////////////
  //                                                         //
  // Class Description of DCut                               //
  //                                                         //   
  ////////////////////////////////////////////////////////////

#include <math.h>

// ROOT classes
#include "TObject.h"


class DSetup;
class DR3;
class DPlane;

class DCut : public TObject {
private:
  DSetup            *fc;                            // pointer to configuration
  Int_t              fPlaneNumber;
  Float_t            fSeedPulseHeightToNoise;
  Float_t            fNeighbourPulseHeightToNoise;
  Float_t            fMaximalNoise;
  Float_t            fMinimalNoise;
  Int_t              fStripsInClusterArea;          // # strips allowed in the cluster area
  Int_t              fStripsInClusterMin;           // min # strips allowed in the cluster
  Int_t              fStripsInClusterMax;           // max # strips allowed in the cluster
  DR3               *fClusterLimit;
  Float_t            fClusterLimitRadius;           // maximum search radius (in mm) from real center of gravity (in mm) to associate new pixels 
 Int_t               fDebugCut;                     // debug flag
 
 public:
  void          SetDebug(Int_t aDebug){fDebugCut = aDebug;}
  Int_t         GetDebug()    { return fDebugCut;}

  DCut();                                 
  DCut(DPlane& aPlane);
  ~DCut();
  Float_t            GetSeedPulseHeightToNoise()      const { return  fSeedPulseHeightToNoise;      }
  Float_t            GetNeighbourPulseHeightToNoise() const { return  fNeighbourPulseHeightToNoise; }
  Float_t            GetMaximalNoise() const { return  fMaximalNoise; } // JB 2013/08/29
  Float_t            GetMinimalNoise() const { return  fMinimalNoise; }
  Int_t              GetStripsInClusterArea()         const { return  fStripsInClusterArea;          } 
  Int_t              GetStripsInClusterMin()          const { return  fStripsInClusterMin;          } 
  Int_t              GetStripsInClusterMax()          const { return  fStripsInClusterMax;          } 
  DR3&               GetClusterLimit()                const { return *fClusterLimit;                } 
  Float_t            GetClusterLimitRadius()          const { return fClusterLimitRadius;}

  void               SetSeedPulseHeightToNoise(Float_t val);
  void               SetNeighbourPulseHeightToNoise(Float_t val);
 
  ClassDef(DCut,1)   // Describes DCut

};

#endif










