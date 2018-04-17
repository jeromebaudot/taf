// @(#)maf/dtools:$Name:  $:$Id: DEventMC.h,v.2 2016/04/20 17:03:46 sha Exp $
// Author: Alejandro Perez   2016/04/20
// Last Modified, AP 2016/04/20
// 

#ifndef _DEventMC_included_
#define _DEventMC_included_


//////////////////////////////////////////////////////////////////////
// Class Description of DEventMC                                    //
//                                                                  //
// Container of the MC truth information when reading a MC n-tuple  //
// This info can be used for hit and track truth matching           //
//                                                                  //
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include <vector>
#include "Riostream.h"

// ROOT classes
#include "TObject.h"
#include "TClonesArray.h"
#include "TArrayF.h"
#include "TH1.h"
#include "TVector3.h"
#include "DPixel.h"
#include "DHit.h"
#include "DTrack.h"

class DSetup;
class DTracker;
class DPlane;          // used to fill data into event plane
class DTrack;
class DHit;
class DPixel;
class DAcq;

#define MaxNumberOf 50

struct SimParticle_t {
  int       pdgID;
  int       BKGType;
  TVector3  ProdVtx;
  int       NHits;
  int       FirstHitIdx;
  int       FrameNumber;
};

struct SimHit_t {
  int       ParticleIdx;
  int       sensorID;
  TVector3  PosInXYZmm;
  TVector2  PosAveUVmm;
  TVector2  PosAveUVLaddermm;
  TVector2  PosRecoAveUVmm;
  TVector2  PosRecoAveUVLaddermm;
  TVector3  MomentumInXYZMeV;
  float     ThetaLoc;
  float     PhiLoc;
  float     Geant4EdepMeV;
  float     GlobalTime;
  float     ClusterPhiPrincipalAxis;
  float     ClusterRMSPrincipalAxis;
  float     ClusterSizeCol;
  float     ClusterSizeRow;
  int       Npixels;
  int       FistPixelIdx;
  int       FrameNumber;
};

struct SimPixel_t {
  int       HitIdx;
  int       GlobalIdx;
  int       col;
  int       row;
  float     ChargeAnalog;
  int       sensorID;
  TVector2  PosUVmm;
  int       status;
  int       FrameNumber;
};

struct NonSensitiveSimParticle_t {
  int       SensitiveIdx;
  int       pdgID;
  TVector3  ProdVtx;
  int       NHits;
  int       FirstHitIdx;
  int       FrameNumber;
};

struct NonSensitiveSimHit_t {
  int       ParticleIdx;
  float     GlobalTime;
  TVector3  PosInXYZmm;
  TVector3  MomentumInXYZMeV;
  float     EnergyInMeV;  
  TVector3  PosOutXYZmm;
  TVector3  MomentumOutXYZMeV;
  float     EnergyOutMeV;
  float     Geant4EdepMeV;
  int       FrameNumber;
};

//__________________________________________________________________________________
//
class DEventMC : public TObject {

private:
  
  //AP 2016/05/15 : Set of structures with the variables of Sim particles, Hits and pixels, as well as the set of corresponding lists.

  //List of particles, hits and pixels from sim-hits in sensitive volumes
  std::vector<SimParticle_t>  ListOfSimParticles;
  std::vector<SimHit_t>       ListOfSimHits;
  std::vector<SimPixel_t>     ListOfSimPixels;
  
  //List of particles and hits from sim-hits in non-sensitive volumes
  std::vector<NonSensitiveSimParticle_t>    ListOfNonSensitiveSimParticles;
  std::vector<NonSensitiveSimHit_t>         ListOfNonSensitiveSimHits;
  
public:
  
  DEventMC();
  virtual ~DEventMC();
  void Clear(const Option_t * /*option*/ = "");
  
  //Set of functions to retrieve the lists of sim particles, hits and pixels. Only to be used when reading MC data
  
  void                    ResetLists(void); //Resets all the list
  
  //Set of functions to fill the particles, hits and pixels lists from sim-hits on sensitive volumes
  void                    PushBackASimParticle(SimParticle_t  ASimParticle)                         {ListOfSimParticles.push_back(ASimParticle);}
  void                    PushBackASimHit(SimHit_t  ASimHit)                                        {ListOfSimHits.push_back(ASimHit);}
  void                    PushBackASimPixel(SimPixel_t  ASimPixel)                                  {ListOfSimPixels.push_back(ASimPixel);}
  
  //Set of functions to fill the particles and hits lists from sim-hits on non-sensitive volumes
  void                    PushBackANonSensitiveSimParticle(NonSensitiveSimParticle_t  ASimParticle) {ListOfNonSensitiveSimParticles.push_back(ASimParticle);}
  void                    PushBackANonSensitiveSimHit(NonSensitiveSimHit_t  ANonSensitiveSimHit)    {ListOfNonSensitiveSimHits.push_back(ANonSensitiveSimHit);}
  
  //Set of functions to get the size of list of particles, hits and pixels from sim-hits on sensitive volumes
  int                     GetNSimParticles()             { return  int(ListOfSimParticles.size());}
  int                     GetNSimHits()                  { return  int(ListOfSimHits.size());}
  int                     GetNSimPixels()                { return  int(ListOfSimPixels.size());}
  
  //Set of functions to get the size of list of particles and hits from sim-hits on non-sensitive volumes
  int                     GetNNonSensitiveSimParticles() { return  int(ListOfNonSensitiveSimParticles.size());}
  int                     GetNNonSensitiveSimHits()      { return  int(ListOfNonSensitiveSimHits.size());}
  
  //Set of functions to get a given element from list of particles, hits and pixels from sim-hits on sensitive volumes
  SimParticle_t           GetASimParticle(int ParticleIdx)             {return ListOfSimParticles[ParticleIdx];}
  SimHit_t                GetASimHit(int HitIdx)                       {return ListOfSimHits[HitIdx];}
  SimPixel_t              GetASimPixel(int PixelIdx)                   {return ListOfSimPixels[PixelIdx];}
  
  //Set of functions to get a given element from list of particles, hits and pixels from sim-hits on non-sensitive volumes
  NonSensitiveSimParticle_t GetANonSensitiveSimParticle(int ParticleIdx) {return ListOfNonSensitiveSimParticles[ParticleIdx];}
  NonSensitiveSimHit_t      GetANonSensitiveSimHit(int HitIdx)           {return ListOfNonSensitiveSimHits[HitIdx];}
  
  void                    DoHitTruthMatching(DHit* aHit,  int &MCHitID, int &NpixelsMCHitID);    //Truth matching of a reconstructed hit
  
  void                    DoTrackTruthMatching(DTrack* aTrack, int &MCPartID, int &NHitsMCPartID); //Truth matching of a reconstructed track
  
  

  ClassDef(DEventMC,3)              // Describes EventMC
  
};

#endif
