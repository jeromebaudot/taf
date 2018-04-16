// @(#)maf/dtools:$Name:  $:$Id:DTrack.h  v.1 2005/10/02 18:03:46 sha Exp $
//  Author   :  Dirk Meier   98/01/07

#ifndef _DTrack_included_
#define _DTrack_included_

  //////////////////////////////////////////////////////////////////
  // Class Description of DTrack                                  //
  //                                                              //
  // A particle track from e.g. accelerator passing the tracker   //
  // The track is measured by the tracker with its silicon        //
  // reference planes                                             //
  // The track is e.g. a straight line                            //
  // The line is found by a fit to the hits in the silicon planes // 
  //                                                              //
  //////////////////////////////////////////////////////////////////


#include <math.h>

// ROOT classes
#include "TObject.h"

class DHit;
class DLine;
class DR3;
class DPlane;
class DParticle;

class DTrack : public TObject {
 private:
  Int_t          fDebugTrack;
  Bool_t         fValid;                        // TRUE = track is valid/good track 
  Float_t        fChiSquare;                    // chisquare/ndf of track fit in 2D
  Float_t        fChiSquareU;                   // chisquare/ndf of track fit, U dim
  Float_t        fChiSquareV;                   // chisquare/ndf of track fit, V dim
  Float_t 	 fDistTr2Hit;			// Mean distance between hits to track
  Int_t          fTrackNumber;                  // number of the track
  DParticle     *fParticle;                     // particle, not implemented
  DLine         *fLineTrajectory;               // line trajectory
  DR3           *fTangent;                      // tangent to the track
  Bool_t         fit_trajectory(Float_t resol); // does  fit to hits in planes
  void           makeChiSquare(Float_t resol);  // calculates the chi square of the track fit

  Int_t          fHits;                         // number of hits to be used in track
  Int_t          fMaxNHits;                     // maximum nb of hits in a track
  DHit         **fHitList;                      //! pointer to list of hits to be used in track  
  Int_t          fShareHit;                     // number of the track with common hit(s), 0 otherwise, CD, Nov 2007
  Int_t          fMaxHitsPerPlane;              // max number of hits allowed per plane to accept the track, JB 2009/05/25

  Float_t        fDeltaOrigineX ;
  Float_t        fDeltaOrigineY ;
  
  Float_t        fVertexX;                      // LC 2012/12/17.
  Float_t        fVertexY;                      // Vertex position.
  Float_t        fVertexZ;

  void           vzero(Double_t *array, Int_t length);
  void           invert(Int_t n, Double_t *a, Double_t *b);
  void           copy(const DTrack& aTrack);  


 public:
  DTrack( Int_t maxNHits);                                 
  ~DTrack();
  DTrack(const DTrack& aTrack);
  DTrack();
  void           SetLinearFit(DLine* aLine) {fLineTrajectory = aLine;}
  Bool_t         Analyze( Int_t aNumber, DHit** aHitList, Int_t nHits, Float_t resol);
  Bool_t         ReFit( Int_t nPlanes, Int_t *ListOfPlanes);   // refit with subset of planes, JB 2013/08/24
  DR3            Intersection(DPlane *aPlane);
  void           Reset();
  Int_t          GetNumber()         const { return   fTrackNumber;    }
  DLine&         GetLinearFit()      const { return  *fLineTrajectory; }
  Float_t        GetChiSquare()      const { return   fChiSquare;      }
  Float_t        GetChiSquareU()     const { return   fChiSquareU;     }
  Float_t        GetChiSquareV()     const { return   fChiSquareV;     }
  Float_t	 GetDistTr2Hit()     const { return   fDistTr2Hit;     }
  DParticle&     GetParticle()       const { return  *fParticle;       }
  DR3&           GetTangent()        const { return  *fTangent;        }
  Int_t          GetHitsNumber()     const { return   fHits;           }
  Int_t          GetMaxNHits()       const { return   fMaxNHits;       } // JB 2012/05/07
  DHit&          GetHitArray()             { return **fHitList;        }
  DHit*          GetHit( Int_t iHit)       { return   fHitList[iHit];  } // JB 2011/07/26
  Bool_t         IsValid()           const { return   fValid;          }
  Int_t          GetShareHit()       const { return   fShareHit;       }// CD, Nov 2007
  Int_t          GetMaxHitsPerPlane() const{ return fMaxHitsPerPlane; } // JB 2009/05/25
  void           SetMaxHitsPerPlane( Int_t maxHits) { fMaxHitsPerPlane=maxHits; } // JB 2009/05/25

  Float_t        GetDeltaOrigineX() {return fDeltaOrigineX;}
  Float_t        GetDeltaOrigineY() {return fDeltaOrigineY;}

  void           SetVertex( Float_t vertexX, Float_t vertexY, Float_t vertexZ);  // Set vertex position. LC 2012/12/17.
  Float_t        GetVertexX()       { return fVertexX;}
  Float_t        GetVertexY()       { return fVertexY;}
  Float_t        GetVertexZ()       { return fVertexZ;}


  void           SetDebug( Int_t aLevel) { fDebugTrack = aLevel;} // JB 2012/0820
  Int_t          GetDebug()         const {return fDebugTrack;} // JB 2012/0820
  
  ClassDef(DTrack,1)                      // Describes DTrack
};

#endif

