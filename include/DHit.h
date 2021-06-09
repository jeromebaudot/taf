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
  DR3               *fPositionHit;              // position of the hit, depends on fPositionAlgorithm
  DR3               *fPositionHitCG;            // position of the hit measured with center of gravity method
  DR3               *fPositionHitEta;           // position of the hit measured with eta correction from 3x3 cluster
  DR3               *fPositionHitCG33;          // position of the hit measured with center of gravity method over a 3x3 cluster
  DR3               *fPositionHitCG22;          // position of the hit measured with center of gravity method over a 2x2 cluster
  DR3               *fPositionHitEta22;         // position of the hit measured with eta correction from 2x2 cluster
  DR3               *fResolutionHit;            // position resolution of the hit, depends on resolution estimation, AP 24/11/2014
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
  //DMonteCarlo       *fPSeedMC;                  // pointer to the pixel Monte Carlo, LC 2014/12/15.
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
  Int_t             *tPixelIndexList;           //! array of pixel/strip index in the original list of pixels
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
  Int_t             fTimestamp;                 // timestamp of seed, JB 2015/05/25
  Bool_t            fIfMonteCarlo;
  std::vector<Double_t> _monteCarloInfo;         // Vector of Monte Carlo Informations. Vector to reduce memory if no MC Info + Add new params easily
  Int_t             fMCHitID;                   // MC hit ID number. This number gives MC hit ID generating the hit. Default value is -1
  Int_t             fStripsFromMCHitID;         // This gives the number of strips/pixels from the MC hit with ID = fMCHitID. Default value if 0

  //Main components analysis variables
  Float_t           fNormMaxStd;                //RMS of pixel position along principal axis
  Float_t           fNormMinStd;                //RMS of pixel position along axis perpendicualer to principal axis
  Float_t           fMainAxisPhi;               //Phi angle of principal axis

  int               fColMin;                    //Pixel with the lowest  column value
  int               fColMax;                    //Pixel with the highest column value
  int               fRowMin;                    //Pixel with the lowest  row    value
  int               fRowMax;                    //Pixel with the highest row    value

public:
  DHit();
  DHit(DR3 &aPosition, DPlane& aPlane, Int_t aHitNumber);
  DHit(DR3 &aPosition, DPlane& aPlane, Int_t aHitNumber, std::vector<Double_t>& monteCarloVector);
  void               copy(DHit* aHit, DPlane* aPlane); // LC 2014/07/15 // Not full a copy :: just hit positions :)
  void               clone(DHit *original, Bool_t fullCopy=1);// VR 2014.08.28, cannot call it Clone otherwise hidden virtual function
  //DHit(DR3 &aPosition, Int_t aHitNumber);
  virtual           ~DHit();
  Bool_t             Analyse(DStrip *s);            // cluster charge, noise, hit position
  Bool_t             Analyse_Iterative(DStrip *s, bool &IsBigCluster, int MaxClusterSize);  //AP, 07/01/2014, Iterative clustering for digital output
  Bool_t             Analyse( Int_t aPixelIndexInList, std::vector<DPixel*> *aListOfPixels);   // cluster charge, noise, hit position
  Bool_t             Analyse_2_cgo( Int_t aPixelIndexInList, std::vector<DPixel*> *aListOfPixels); // make hit with pixels considering maximum search radius (in mm) from real center of gravity (in mm) to associate new pixels
  Bool_t             Analyse_Iterative( Int_t aPixelIndexInList, std::vector<DPixel*> *aListOfPixels, bool &IsBigCluster, int MaxClusterSize);   //AP, 07/01/2014, Iterative clustering for digital output
  //Bool_t             AnalyseMC( std::vector<DMonteCarlo*> *aListOfPixelsMonteCarlo)                    //LC, 2014/12/15, Get MC Hit position
  Int_t              GetNumber()                      const { return  fHitNumber;            }
  void               SetNumber(Int_t aNb)                   { fHitNumber = aNb;}// VR 2014.08.28
  Bool_t             GetFound()                       const { return fFound; }
  void               SetFound( Bool_t found=kTRUE)          { fFound = found; } // JB 2011/03/14 to allow to free the hit
  Int_t              GetDebug()                             { return fDebugHit;              }
  void               SetDebug( Int_t aDebug)                { fDebugHit = aDebug;            }
  DPixel            *GetPSeed()                             { return  fPSeed;                } // JB 2012/08/17
  DStrip            *GetSeed()                              { return  fSeed;                 }
  DStrip            *GetMinor(Int_t aSk);             // strip with index lower pulseheight in neighourhood to seed
  DR3               *GetPosition()                    const { return fPositionHit;}
  DR3               *GetPositionCG()                  const { return fPositionHitCG;}
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
  Float_t            GetResolutionUhit() const;
  Float_t            GetResolutionVhit() const;
  void               SetResolutionUVhit(Float_t resolutionU,Float_t resolutionV);
  Float_t            GetClusterPulseSum()             const { return  fClusterPulseSum;      }
  Float_t            GetClusterAreaPulseSum()         const { return  fClusterAreaPulseSum;  }
  Int_t              GetIndex(Int_t tSk)              const { return  fStripIndex[fStripIndexArray[tSk]];      } // JB 2009/05/12
  Int_t              GetIndexInOriginalList(Int_t tSk) const { return tPixelIndexList[tSk]; }  //AP 2016/07/28
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
  Int_t              GetStoNover2()                   const { return  fStoNover2;            }

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

  Int_t              GetTimestamp()                   const { return fTimestamp; }; // JB 2015/05/25
  
  Bool_t             IfMonteCarlo()                   const { return fIfMonteCarlo; }; // LC 2014/12/15 -> Test if MC Infos are included.

  Float_t            DistanceMC( DHit *aHit);     // LC 2014/12/15
  Float_t            DistanceMC( DTrack *aTrack); // LC 2014/12/15
  Float_t            DistanceMC( DR3 *aPoint);    // LC 2014/12/15

  void               SetMCHitID(Int_t aHitID) {fMCHitID = aHitID;}  //AP 2015/07/30
  Int_t              GetMCHitID()             {return fMCHitID;}    //AP 2015/07/30

  void               SetStripsFromMCHitID(Int_t aStripsFromMCHitID) {fStripsFromMCHitID = aStripsFromMCHitID;}  //AP 2016/07/28
  Int_t              GetStripsFromMCHitID()                         {return fStripsFromMCHitID;}                //AP 2016/07/28

  void               SetHitPosition(DR3 aPosition,DR3 aResolution);

  DR3                GetMonteCarloPosition();
  std::vector<Double_t>& GetMonteCarloInfo()          { return _monteCarloInfo; } // LC 2014/12/15

  //Main components analysis functions
  void               DoMCA(void);  //AP 2017/05/09
  Float_t            GetNormMinStd()                  { return  fNormMinStd;  }  //AP 2017/05/09
  Float_t            GetNormMaxStd()                  { return  fNormMaxStd;  }  //AP 2017/05/09
  Float_t            GetMainAxisPhi()                 { return  fMainAxisPhi; }  //AP 2017/05/09

  void               CalcHitLimits(void); //AP 2017/05/11
  int                GetColMin()                      { return fColMin; }  //AP 2017/05/11
  int                GetColMax()                      { return fColMax; }  //AP 2017/05/11
  int                GetRowMin()                      { return fRowMin; }  //AP 2017/05/11
  int                GetRowMax()                      { return fRowMax; }  //AP 2017/05/11

  Bool_t             IsSortable() const { return kTRUE; } // QL 04/06/2016
  Int_t              Compare( const TObject * obj) const; // QL 04/06/2016
  virtual void       Print(const Option_t* ="") const; // QL 05/06/2016

  ClassDef(DHit,1)                              // Hit in a Plane
};

#endif
