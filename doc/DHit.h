// @(#)maf/dtools:$Name:  $:$Id: DHit.h,v.1 2005/10/02 18:03:46 sha Exp $
// Author   :  ?

#ifndef _DHit_included_ 
#define _DHit_included_ 

  ////////////////////////////////////////////////////////////
  //                                                        //
  // Class Description of DHit                              //
  //                                                        //
  ////////////////////////////////////////////////////////////
 
#include "Riostream.h" 
#include <math.h> 
#include <vector>

// ROOT classes 
#include "TObject.h" 
#include "TArrayF.h"
#include "TAxis.h"
#include "DPixel.h"
//#include "DPlane.h"

class DTrack;                  // forward declarations 
class DStrip; 
class DPlane; 
class DCut;
class DR3;

class DHit : public TObject {

 private:
  Int_t              fTableSize;                // Size of Arrays allocated in constructor // VR 2014.08.28
  Int_t              fDebugHit;
  Int_t              fHitNumber;                // hit number
  Bool_t             fFound;                    // kTRUE is associated to a track, JB 2009/05/22
  Int_t              fPositionAlgorithm;        // 1 = Center of Gravity, 2 = eta, 3= kappa, 4 = gamma
  DPlane            *fPlane;                    // pointer to the device which got this hit
  DR3               *fPositionHit;              // position of the hit, measured with 2-strip eta 
  DR3               *fPositionHitCG;            // position of the hit measured with center of gravity method
  DR3               *fPositionHitEta;           // position of the hit measured with eta correction from 3x3 cluster
  DR3               *fPositionHitCG33;          // position of the hit measured with center of gravity method over a 3x3 cluster
  DR3               *fPositionHitCG22;          // position of the hit measured with center of gravity method over a 2x2 cluster
  DR3               *fPositionHitEta22;         // position of the hit measured with eta correction from 2x2 cluster
  Float_t            fSeedU;                    // U position of hit seed strip
  Float_t            fSeedV;                    // V position of hit seed strip
  Float_t            fClusterPulseSum;          // sum of pulseheight on strips in hit cluster, involves noise cuts
  Float_t            fClusterAreaPulseSum;      // sum of pulseheight on strips in hit cluster area, no noise cuts
  Float_t            fClusterSignalToNoise;     // hit cluster signal to noise ratio
  Float_t            fClusterNoiseAverage;      // hit cluster signal noise average
  Int_t              fStripsInClusterFound;     // number of strips in the hit cluster
  Int_t              fStripsInClusterArea;       // # strips in cluster area
  DR3               *fClusterLimit;             // maximum extension in u,v,w for hit analysis
  Float_t           fClusterLimitRadius;        // maximum search radius (in mm) from real center of gravity (in mm) to associate new pixels 
  DStrip            *fSeed;                     // pointer to the hit seed strip 
  DPixel            *fPSeed;                    // pointer to the hit seed pixel, JB 2009/05/01 
  DCut              *fCut;                      // pointer to cuts
  DR3               *fStripPitch;               // pointer to strip pitch in plane
  Float_t            fPhSeed;                   // pulseheight on seed strip
  Float_t            fPosSeed;                  // position of the seed strip
  Float_t            fPhLofSeed;                // pulseheight on strip left from seed
  Float_t            fPosLofSeed;               // position of strip left of seed
  Float_t            fPhRofSeed;                // pulseheight on strip right from seed
  Float_t            fPosRofSeed;               // position of strip right from seed
  Int_t              fIndexSeed;                // index of seed strip
  Int_t              fIndexLofSeed;             // index of strip left from seed
  Int_t              fIndexRofSeed;             // index of strip right from seed
  Float_t            fPhLeft;                   // pulseheight on left most strip in two strip cluster
  Float_t            fPosLeft;                  // position of left most strip in two strip cluster
  Float_t            fPhRight;                  // pulseheight on right most strip in two strip cluster
  Float_t            fPosRight;                 // position of strip in right most cluster
  Int_t              fIndexLeft;                // index of left most strip
  Int_t              fIndexRight;               // index of right most strip
  Int_t              fStoNover2;                // MB 2010/11/12

  Float_t           *fStripNoise;               //! array of noise from strips in hit cluster  YV 27/11/09
  Int_t             *fStripIndexArray;          //! array of indices to strips inside the hit
  Int_t             *fStripIndex;               //| array of index of strip in the full plane matrix
  Float_t           *fStripPulseHeight;         //! array of pulseheights from strips in hit cluster
  Float_t           *fStripDistanceU;           //! array of seed distanceU from strips in hit cluster
  Float_t           *fStripDistanceV;           //! array of seed distanceV from strips in hit cluster
  Int_t              fStripsInClusterDemanded;  // exact number of strips required in Hit, set by user
  Float_t            evaluateChargeFraction(Float_t aChargeFraction);  // evaluates the charge fraction on strip 
  TArrayF           *fChargeFractionDensity;    // Array of charge fraction densities for hit measurement

  Float_t           fSNneighbour;               // signal / noise of neighbours
  Float_t           fSNseed;                    // signal / noise of seed pixel, JB 2013/11/08
  Float_t           fSeedPulseHeight;           // pulseheight on seed, JB 2013/11/08
  Float_t           fSeedNoise;                 // noise on seed, JB 2013/11/08
  Bool_t            fIsFromPreviousEvent;       // explicit // VR 2014.08.28

public:
  DHit();                                 
  DHit(DR3 &aPosition, DPlane& aPlane, Int_t aHitNumber);
  void               Clone(DHit *original, Bool_t fullCopy=1);// VR 2014.08.28
  //DHit(DR3 &aPosition, Int_t aHitNumber);
  virtual           ~DHit();
  Bool_t             Analyse(DStrip *s);   // cluster charge, noise, hit position
  Bool_t             Analyse( Int_t aPixelIndexInList, vector<DPixel*> *aListOfPixels);   // cluster charge, noise, hit position
  Bool_t             Analyse_2_cgo( Int_t aPixelIndexInList, vector<DPixel*> *aListOfPixels); // make hit with pixels considering maximum search radius (in mm) from real center of gravity (in mm) to associate new pixels
  Bool_t             Analyse_Dynamic( Int_t aPixelIndexInList, vector<DPixel*> *aListOfPixels);   //AP, 07/01/2014, Dynamic clustering for digital output
  Int_t              GetNumber()                      const { return  fHitNumber;            }
  void               SetNumber(Int_t aNb)                   { fHitNumber = aNb;}// VR 2014.08.28
  Bool_t             GetFound()                       const { return fFound; }
  void               SetFound( Bool_t found=kTRUE)          { fFound = found; } // JB 2011/03/14 to allow to free the hit
  Int_t              GetDebug()                             { return fDebugHit;              }
  void               SetDebug( Int_t aDebug)                { fDebugHit = aDebug;            }
  DPixel            *GetPSeed()                             { return  fPSeed;                 } // JB 2012/08/17
  DStrip            *GetSeed()                              { return  fSeed;                 }
  DStrip            *GetMinor(Int_t aSk);             // strip with index lower pulseheight in neighourhood to seed     
  DR3               *GetPosition()                    const { return fPositionHit;}
  Float_t            GetPositionUhit() const;
  Float_t            GetPositionVhit() const;
  Float_t            GetPositionWhit() const;
  Float_t            GetPositionUhitCG() const;
  Float_t            GetPositionVhitCG() const;
  Float_t            GetPositionWhitCG() const;
  Float_t            GetPositionUhitEta() const;
  Float_t            GetPositionVhitEta() const;
  Float_t            GetPositionUhitCG33() const;
  Float_t            GetPositionVhitCG33() const;
  Float_t            GetPositionUhitCG22() const;
  Float_t            GetPositionVhitCG22() const;
  Float_t            GetPositionUhitEta22() const;
  Float_t            GetPositionVhitEta22() const;
  Float_t            GetClusterPulseSum()             const { return  fClusterPulseSum;      }
  Float_t            GetClusterAreaPulseSum()         const { return  fClusterAreaPulseSum;  }
  Int_t              GetIndex(Int_t tSk)              const { return  fStripIndex[fStripIndexArray[tSk]];      } // JB 2009/05/12
  Bool_t             GetIsFromPreviousEvent()         const { return fIsFromPreviousEvent;}// VR 2014.08.28
  void               SetIsFromPreviousEvent(Bool_t value=1) {fIsFromPreviousEvent=value;}// VR 2014.08.28
  Float_t            GetPulseHeight(Int_t tSk)        const { return  fStripPulseHeight[fStripIndexArray[tSk]];} // JB 2009/05/12
  Float_t            GetNoise(Int_t tSk)              const { return  fStripNoise[fStripIndexArray[tSk]];}  //YV 27/11/09
  Float_t            GetClusterSignalToNoise()        const { return  fClusterSignalToNoise; }
  Float_t            GetClusterNoiseAverage()         const { return  fClusterNoiseAverage;  }
  Int_t              GetStripsInCluster()             const { return  fStripsInClusterFound; }
  DPlane            *GetPlane()                       const { return  fPlane;                }
  Float_t            GetSeedU()                       const { return  fSeedU;                }
  Float_t            GetSeedV()                       const { return  fSeedV;                }
  Float_t            GetPulseHeightLeft()             const { return  fPhLeft;               } 
  Float_t            GetPulseHeightRight()            const { return  fPhRight;              } 
  Float_t            GetPulseHeightLeftOfSeed()       const { return  fPhLofSeed;            } 
  Float_t            GetPulseHeightRightOfSeed()      const { return  fPhRofSeed;            } 
  Float_t            GetPositionULeft()               const { return  fPosLeft;              } 
  Int_t		           GetStoNover2()                   const { return  fStoNover2;            }

  Int_t              GetIndexLeft()                   const { return  fIndexLeft;            }
  Int_t              GetIndexRight()                  const { return  fIndexRight;           }
  Int_t              GetIndexSeed()                   const { return  fIndexSeed;            } 

  Float_t            GetSNneighbour()                 const { return  fSNneighbour;         } 
  Float_t            GetSNseed()                      const { return  fSNseed;          } // JB 2013/11/08
  Float_t            GetSeedPulseHeight()             const { return  fSeedPulseHeight;          } // JB 2013/11/08
  Float_t            GetSeedNoise()                   const { return  fSeedNoise;          } // JB 2013/11/08
  
  Float_t            Distance( DHit *aHit);
  Float_t            Distance( DTrack *aTrack); // JB 2009/06/26
  Float_t            Distance( DR3 *aPoint); // JB 2011/07/25


  ClassDef(DHit,1)                              // Hit in a Plane
};

#endif
