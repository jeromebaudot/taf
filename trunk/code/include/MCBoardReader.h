#ifndef _MCBoardReader_HXX
#define _MCBoardReader_HXX
/*!
  \file
  \version $Id: MCBoardReader.hxx,v 1.8 2003/07/08 18:54:19 mueller Exp $
  \brief   Declaration of MCBoardReader.
*/
/*------------------------------------------+---------------------------------*/

#include "BoardReader.h" // withROOT is define therein
#include <fstream>
#include <iostream>
#include <vector>
//using namespace std;
#include "TH2F.h"
#include "TString.h"
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "DGlobalTools.h"
#include "DEventMC.h"
#include "DSetup.h"

class TH2F;
class TH1F;

#define withROOT

const Int_t kMaxHits     = 4000;

const Int_t NMaxParticle = 100;      //Maximum number of particle save in the TAFTree
const Int_t NMaxHit      = 10000;    //Maximum number of hit save in the TAFTree
const Int_t NMaxPixel    = 10000000; //Maximum number of pixel save in the TAFTree
const Int_t NMaxSatu     = 10000;    //Maximum number of saturated lines

struct MCHitPixel_t {
  int       GlobalIdx;
  int       col;
  int       row;
  float     ChargeAnalog;
  int       sensorID;
  TVector2  PosUVmm;
  int       status;
  int       frame;
};

struct MCParticleHit_t {
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
  int       frame;
  int       Npixels;
  std::vector<MCHitPixel_t> ListOfHitPixels;
};

struct MCParticle_t {
  int       pdgID;
  int       BKGType;
  TVector3  ProdVtx;
  int       frame;
  std::vector<MCParticleHit_t> ListOfParticleHits;
};

//Non-sensitive sim-hits
struct MCNonSensitiveParticleHit_t {
  float     GlobalTime;
  TVector3  PosInXYZmm;
  TVector3  MomentumInXYZMeV;
  float     EnergyInMeV;
  TVector3  PosOutXYZmm;
  TVector3  MomentumOutXYZMeV;
  float     EnergyOutMeV;
  float     Geant4EdepMeV;
  int       frame;
};

struct MCNonSensitiveParticle_t {
  int       sensitiveIdx;
  int       pdgID;
  TVector3  ProdVtx;
  int       frame;
  std::vector<MCNonSensitiveParticleHit_t> ListOfNonSensitiveParticleHits;
};

//##############################################################################
class MCBoardReader : public TObject  {
    
public:

  MCBoardReader(int boardNumber, int Runnumber, TString pathName, TString fileName, TString TreeName, DSetup* aSetup);
  ~MCBoardReader();

  void  SetDebugLevel( int aLevel);
  bool  HasData();
  bool  ValidHistogram() { return fDisplay; }
  int   GetBoardNumber() { return fBoardNumber; }
  int   GetEventNumber() { return fEventNumber; }
  BoardReaderEvent*   GetEvent() { return fCurrentEvent; }
  void  PrintStatistics(ostream &stream);
  
  void SetMCInforHolder(DEventMC* AnMCInfoHolder) {MCInfoHolder = AnMCInfoHolder;};
  
  void FillUpMCParticlesFromFrame(int TheFrame);  //AP, 2016/04/21. Fill up the particle list for frame TheFrame
  void FillUpMCInfoHolderForThisEvent(void);      //AP, 2016/04/21. Fill up the MCInfoHolder for the current event
  
private:

  DSetup           *fSetup;          // pointer to the configuration
  
  int             fDebugLevel;       // debug level
  DGlobalTools    fTool;
  
  int                   fRunNumber;
  int                   fBoardNumber;
  int                   fNumberOfSensors;
  TString               fPathName;         // base file name
  TString               fFileName;         // base file name
  TString               fTreeName;         // base file name
  //TString             TheFullFileName;
  std::vector<TString>  ListOfInputROOTFiles;
  
  bool                  GotNonSensitiveInfo;
  DEventMC*             MCInfoHolder;
  
  TFile* file_in;
  
  //The MC tree
  //TTree* fTree;
  TChain* fChain;
  Int_t   fCurrent; //!current Tree number in a TChain
  
  //The MC tree structure
  //Particles Branch
  Int_t    ParticleNb;
  Int_t    ParticleBKGType[NMaxParticle];
  Int_t    ParticlepdgID[NMaxParticle];
  Float_t  ParticleTrkVtX[NMaxParticle];
  Float_t  ParticleTrkVtY[NMaxParticle];
  Float_t  ParticleTrkVtZ[NMaxParticle];
  Int_t    ParticleNHits[NMaxParticle];
  Int_t    ParticleFirstHitIdx[NMaxParticle];
   
  //Hits Branch
  Int_t   HitNb;
  Int_t   HitParticleIdx[NMaxHit];
  Int_t   HitsensorID[NMaxHit];
  Int_t   HitladderID[NMaxHit];
  Int_t   HitmoduleID[NMaxHit];
  Float_t HitposINmmX[NMaxHit]; 
  Float_t HitposINmmY[NMaxHit];
  Float_t HitposINmmZ[NMaxHit];
  Float_t HitposAVRmmU[NMaxHit];
  Float_t HitposAVRmmV[NMaxHit];
  Float_t HitposAVRmmW[NMaxHit];
  Float_t HitposAVRmmULadder[NMaxHit];
  Float_t HitposAVRmmVLadder[NMaxHit];
  Float_t HitmomMeVX[NMaxHit];
  Float_t HitmomMeVY[NMaxHit];
  Float_t HitmomMeVZ[NMaxHit];
  Float_t HitthetaLoc[NMaxHit];
  Float_t HitphiLoc[NMaxHit];
  Float_t HitglobalTime[NMaxHit];
  Float_t HitGeant4EdepMeV[NMaxHit];
  Float_t HitClusterSizeCol[NMaxHit];
  Float_t HitClusterSizeRow[NMaxHit];
  Float_t HitRecoUmm[NMaxHit];
  Float_t HitRecoVmm[NMaxHit];  
  Float_t HitRecoULaddermm[NMaxHit];
  Float_t HitRecoVLaddermm[NMaxHit];
  Float_t HitPhiPrincipalAxis[NMaxHit];
  Float_t HitRMSPrincipalAxis[NMaxHit];
  Int_t   HitNPixels[NMaxHit];
  Int_t   HitFirstPixelIdx[NMaxHit];
   
  //Pixels Branch 
  Int_t   PixelNb;
  Int_t   PixelHitIdx[NMaxPixel];
  Int_t   PixelGlobalIdx[NMaxPixel];
  Int_t   PixelColumn[NMaxPixel];
  Int_t   PixelRow[NMaxPixel];
  Float_t PixelAnalogCharge[NMaxPixel];
  Int_t   PixelSensorID[NMaxPixel];
  Float_t PixelUmm[NMaxPixel];
  Float_t PixelVmm[NMaxPixel];
  Int_t   PixelStatus[NMaxPixel];

  //Saturated line branch
  Int_t   SatuNb;
  Int_t   SatuLinIdx[NMaxSatu];
  Int_t   SatuSensorID[NMaxSatu];
  
  //Non-sensitive Hit variables
  //Definition of Particles Branch
  Int_t    NonSensitiveParticleNb;                         // number of particles hitting non-sensitive volumes in event
  Int_t    NonSensitiveParticleSensitiveIdx[NMaxParticle]; // index of this particle in Mimosa particle list
  Int_t    NonSensitiveParticlepdgID[NMaxParticle];        // particle pdg ID
  Float_t  NonSensitiveParticleTrkVtX[NMaxParticle];       // particle's origin vertex, X coordinate, in mm
  Float_t  NonSensitiveParticleTrkVtY[NMaxParticle];       // particle's origin vertex, Y coordinate, in mm
  Float_t  NonSensitiveParticleTrkVtZ[NMaxParticle];       // particle's origin vertex, Z coordinate, in mm
  Int_t    NonSensitiveParticleNHits[NMaxParticle];        // particle's number of hits
  Int_t    NonSensitiveParticleFirstHitIdx[NMaxParticle];  // index on hit block of particle's 1st hit
    
  //Definition of Hits Branch
  Int_t   NonSensitiveHitNb;                       // Number of hits in non-sensitive volumes in event
  Int_t   NonSensitiveHitParticleIdx[kMaxHits];    // index in particle block of particle producing this hit
    
  Float_t NonSensitiveHitposINmmX[kMaxHits];       // enter position in global ref. frame, X coordinate, in mm
  Float_t NonSensitiveHitposINmmY[kMaxHits];       // enter position in global ref. frame, Y coordinate, in mm
  Float_t NonSensitiveHitposINmmZ[kMaxHits];       // enter position in global ref. frame, Z coordinate, in mm
  Float_t NonSensitiveHitglobalTimeINns[kMaxHits]; // enter global time, in ns
  Float_t NonSensitiveHitmomINMeVX[kMaxHits];      // enter momentum, X coordinate, in MeV
  Float_t NonSensitiveHitmomINMeVY[kMaxHits];      // enter momentum, Y coordinate, in MeV
  Float_t NonSensitiveHitmomINMeVZ[kMaxHits];      // enter momentum, Z coordinate, in MeV
  Float_t NonSensitiveHitEnergyINMeV[kMaxHits];    // enter Energy, in MeV
  Float_t NonSensitiveHitposOUTmmX[kMaxHits];      // exit  position in global ref. frame, X coordinate, in mm
  Float_t NonSensitiveHitposOUTmmY[kMaxHits];      // exit  position in global ref. frame, Y coordinate, in mm
  Float_t NonSensitiveHitposOUTmmZ[kMaxHits];      // exit  position in global ref. frame, Z coordinate, in mm
  Float_t NonSensitiveHitmomOUTMeVX[kMaxHits];     // exit  momentum, X coordinate, in MeV
  Float_t NonSensitiveHitmomOUTMeVY[kMaxHits];     // exit  momentum, Y coordinate, in MeV
  Float_t NonSensitiveHitmomOUTMeVZ[kMaxHits];     // exit  momentum, Z coordinate, in MeV
  Float_t NonSensitiveHitEnergyOUTMeV[kMaxHits];   // exit  Energy, in MeV
  Float_t NonSensitiveHitGeant4EdepMeV[kMaxHits];  // particles deposited energy by ionization, in MeV
  
  std::vector<MCParticle_t> TheListOfMCParticles;
  std::vector<MCHitPixel_t> ListOfNoisePixels;
  std::vector<MCNonSensitiveParticle_t> TheListOfMCNonSensitiveParticles;
  
  TH1F          **hLineOverFlowDist; //Distribution of line Overflow

  int             fEventSize;        // size of the event
  bool            fDisplay;          // decide histo display or not
  
  unsigned int    fCurrentTriggerCnt;
  int             fEventNumber;
  int             fFrameCounter;
  
  BoardReaderEvent *fCurrentEvent;
  vector<BoardReaderPixel>  ListOfPixels;
  vector<int>               ListOfTriggers;
  vector<int>               ListOfFrames;
  vector<int>               ListOfLineOverflow;

  bool fOverflow;           // flag to indicate overflow
  int  fEventsOverflow;     // count total events with overflow
  int  fNStatesInLine;      // count total # states
  int  fFramesReadFromFile; // count total # frames read
  
  //Opening the MC data file
  bool      Open();
  void      Init(const char* ROOTfilename, const int num);
  bool      GetAFrame(Long64_t aFrame);
  Long64_t  LoadTree(Long64_t entry);
 
  //! Build event by creating the list of frames of the event
  bool FetchEvent();
    
  //! Loop over the event's list of frames
  bool DecodeFrames();
  
  // Add the pixels of frame TheFrame to the list of pixels of the event
  void FillPixelsFromFrame(int TheFrame);
    
  //! Add pixel to list
  void AddPixel(int input, int value, int aLine, int aColumn);
    
  //! Check trigger counts
  bool CheckTriggerCnt(unsigned int trig);
  
  bool CheckIfNonSensitiveVariables(TTree* tree);
   
  ClassDef(MCBoardReader,0)
  
};

#endif
