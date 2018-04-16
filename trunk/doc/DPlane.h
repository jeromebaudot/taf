// @(#)maf/dtools:$Name:  $:$Id: DPlane.h,v.3 2005/10/02 18:03:46 sha Exp $
// Author   : ?

#ifndef _DPlane_included_
#define _DPlane_included_


  /////////////////////////////////////////////////////////////
  // Class Description of DPlane                             //
  //                                                         //
  // + finds pedestal                                        //
  // + calculates pulseheight                                //
  // + corrects for common mode (shift)                      //
  // + calculates single noise                               //
  //                                                         //
  /////////////////////////////////////////////////////////////

#include <math.h>
#include <vector>

// ROOT classes
#include "TObject.h"
#include "TString.h"
#include "TBRIK.h"
#include "TNode.h"
#include "TRandom.h"
#include "TH1.h"
#include "TH2.h"
#include "TObjArray.h"
#include "TMath.h"

#include "DSession.h"
#include "DPixel.h"
#include "DMonteCarlo.h"

class DSetup;
class DStrip;
class DTrack;
class DTracker;
class DAcq;
class DAcqModule;
class DInp;
class DHit;
class DPrecAlign;
class DAlign;
class DCut;
class DR3;
class DSession;
// class DCMOSReader 
class DPlane : public TObject {

 private:

  Int_t        fDebugPlane;               // plane debug flag
  DSession    *fSession;                  // JB 2011/07/21 to get rid of global var
  DGlobalTools  fTool;                    // JB 2014/04/21
  Long_t       *fChannel;                  //! pointer to Channels 
  Long_t       *fStripIndices;             //! pointer to strip indices 
  Bool_t       fKillNoise;                // flag in case of noise suppression (call to KillNoise)

  Float_t     *fCommonShift;              //! Common Signal shift in plane regions
  Long_t       *fCommonChannels;           //! Number of Channels in these plane regions 
  Long_t       *fChannelGood;              //! list of good channels ( 1= good, 0 = bad)
  DStrip     **fStripList;                //! list of pointers to strips
  vector<DPixel*> *fListOfPixels;         // pointer to list of hit pixel
  //vector<int> *fListOfPixels;             // pointer to list of hit pixels
  vector<DMonteCarlo*> *fListOfMonteCarlo;
  Int_t        fPixelsN;                  // number of hit pixels
  Int_t        fStripsNu;                 // number of strips in u direction
  Int_t        fStripsNv;                 // number of strips in v direction
  Int_t        fStripsN;                  // sum of strips in u,v and w   
  Int_t        fRegions;                  // number of regions for common mode correction (e.g. 2 if 2 readout chips)
  Int_t        fPlaneNumber;              // the number given to the plane e.g. this Sirocco
  Int_t        fLadderNumber;             // LC 2013/01/14. number of the ladder.
  Char_t      *fPlaneName;                //! name of this device
  Char_t      *fPlanePurpose;             // purpose e.g. reference or test or whatever
  Float_t      fPulseRangeMaximum;        // maximum pulseheight expected on a strip in this plane
  Int_t        fAcqInputsN;               // Number of inputs the plane is connected to
  Int_t        fAcqModuleTypeNumber;      // this plane is connected to an input in fDacToolNumber
  Int_t        fAcqModuleNumber;          // number of the Acquisition Module 
  Int_t        fAcqInputNumber;           // input number this plane is connected to in the Acquisition Module
  Int_t        fAcqChannelNumber;         // first strip nb associated to the first channel number for this input (start at 1)
  Int_t        fAcqChannelOffset;         // the number of the first channel related to the plane for this input (start at 1)
  Int_t        fAcqModuleChannels;        // number of channels per input of the module type
  Int_t        fChannelsN;                // number of channels in this plane

  DHit       **fHit;                      //! list of pointers to hits
  DHit       **fHitUnTrackedLastEvent;     //Explicit   // VR 2014.08.28  
  Int_t        fHitsN;                    // number of hits found in plane (total)
  Int_t        fHitsUnTrackedLastEventN;  // number of hits not associated to a track in last event // VR 2014.08.28
  Int_t        fHitsNew;                  // number of new hits for this event // VR 2014.08.28
  Int_t        fHitsOld;                  // number of untracked hits from previous event added to the list // VR 2014.08.28
  
  Int_t        fHitMax;                   // maximum number of hits allowed in plane
  Float_t      fCDSvariance;              // variance of signal distributon in the plane after CDS
  Float_t      fThreshold;                // a threshold for measuring occupancy
  Long_t        fOverThresholdN;           // the number of channels which exceed threshold
  Float_t      fOverThresholdC;           // the pulseheight sum on strips over threshold
  Int_t        fKeepUnTrackedHitsBetw2evts; //explicit // VR 2014.08.28

  DCut        *fCut;
  TBRIK       *fGeometry;
  TNode       *fPlaneNode;
  Char_t      *fPlaneNodeName;	          //!			   
  Int_t        fStatus;                   // 1=Primary, 2=Secondary Reference, or 3=Test Plane
  Int_t        fAnalysisMode;             // 0, 1=Strips, 2=Pixels
  Int_t        fReadout;                  // 1=normal (look Update() method)
  Int_t        fMimosaType;               // RDM210509
  Int_t        fMapping;                  // JB 2012/11/21
  Int_t        fHitFinder;                // type of hit finder, JB 2013/07/17
  Int_t        fIfDigitize;               // control digitization emulation, JB 2013/07/17
  Int_t       *fDigitizeThresholds;       // thresholds for digitization emulation, JB 2013/07/17
  DR3         *fPitch;                    // pitch of strips in u,v,w direction
  Double_t     fTiltW;                    // the tilt in W
  Float_t      fPositionU;                // the distance between 0 plane and z, perpendicular to strips
  Float_t      fPositionV;                // the distance between 0 plane and z, perpendicular to strips
  DR3         *fPosition;                 // Position of the center in the tracker coordinate system
  Int_t        fIfDeformation;            // Flag to model or not deformation, JB 2014/04/10
  Double_t     fUDeformationCoef[8];      // Coeff. for U-deformation model, JB 2014/04/10
  Double_t     fVDeformationCoef[8];      // Coeff. for V-deformation model, JB 2014/04/10

  TH1F        *fChargeFractionDensity;       // Charge Fraction density distribution, not used (JB 2007) 
  TObjArray   *fChargeFractionDensityList;        // list of charge fraction densities
  TObjArray   *fChargeFractionDistributionList;   // list of charge fraction distrubutions
  Bool_t       fDigitalStripResponse;     // kTRUE = forces digital strip response, hitposition = seed
  TH1F        *fEtaIntU;                  // Histo for Eta algorithm in 2D, JB Sept 2007
  TH1F        *fEtaIntV;                  // Histo for Eta algorithm in 2D, JB Sept 2007
  TH1F        *fEtaIntU2;                 // Histo for Eta algorithm 2x2 in 2D, JB Novemb 2007
  TH1F        *fEtaIntV2;                 // Histo for Eta algorithm 2x2 in 2D, JB Novemb 2007

  // YV 21/10/09
  TH2F        *fHNoise;                   // Histo for noise computation 
  TH2F        *fHPedestal;                // Histo for pedestal computation 
  Int_t        fNoiseRun;                 // Id of noise run
  TFile       *fNoiseFile;                // File containing histos with peds and noises

  DR3         *fSize;                     // rectangular extensions 
  DR3         *fTilt;                     // tilting angles [radian], three Euler angles, comes later
  DAcq        *fAcq;                      // pointer to Data Acquisition
  DSetup      *fc;                        // pointer to Configuration
  DTracker    *fTracker;                  // pointer to the Tracker
 
  Int_t       *fRawData;                  //! pointer to Raw Data in Data Acquisition
  Float_t     *aCacheRawData;             //! pointer to temporary copy of Raw Data 
  DAlign      *fAlign;                    // plane alignement
  DPrecAlign  *fPrecAlign;                // store plane precision alignement
  DAcqModule  *fAcqModule;                // pointer to Acquisition Module (e.g. Sirocco) 
  DInp        *fInput;                    // pointer to the Input in the Acquisition 
                                          // where this plane is plugged in
  void         analyze_basics();          // do basic calculation on raw data: 
  void         CalculateCommonMode();     // calculates common mode in regions of the plane
  void         SetPedandNoiseFromHisto( Int_t col, Int_t row, DPixel *aPixel); // set the pixel ped and noise
  
  Int_t        fInitialStatus;            // 0 before init, 1 after
  Int_t        fInitialCounter;           // counter for intialization of pedestal
  Int_t        fInitialPedestal;          // number of events  for intialization of pedestal
  Int_t        fInitialNoise;             // number of events  for intialization of noise
    
  Float_t      aHitSupressedMean(Float_t* data, const Int_t entries);

  Float_t      aHitSupressedValue(Float_t* data, const Int_t entries);
  Int_t        extremumIndex(const Float_t* data, const Int_t N);
  Float_t      median(Float_t* data, const Int_t N);
  Float_t      mean(Float_t* data, const Int_t N);
  Float_t      minimum(const Float_t* data, const Int_t N);


  Float_t      runningMean(const Float_t om,      // former mean
			   const Float_t o,       // former value
			   const Float_t n,       // new value
			   const Int_t   N);      // number of values    
    
  Float_t      runningVariance(const Float_t ov,  // former variance
			       const Float_t o,   // former value
			       const Float_t n,   // new value
			       const Int_t   N);  // number of values

  void         find_hits();               // finds hits in plane
  void         UpdatePedestalAndNoise();
  Bool_t       CheckSaturation();         // Checks if the event is saturated (both frames are at maximum -> CDS==0)
  void         FindNeighbours();           // finds neighbour pixels/strips for cluster building

  TRandom      fRandomGenerator;          // random generator (seed is set at initialization)

 public:
  DPlane();                                 
  DPlane(DTracker& aTracker, const Int_t aPlaneNumber, const Int_t aLadderNumber);
  ~DPlane();

  //FILE * GetFilePointer();

  Bool_t       Update();
  Bool_t       Align(DTrack  *aTrack, Bool_t ifFit=kTRUE);         // aligns this plane to the track
  Bool_t       AlignAllHits(DTrack  *aTrack, Bool_t ifFit=kTRUE);         // aligns this plane to the track
  
  // LC
  void         AlignPrec(DTrack  *aTrack, const Float_t tiniBound);        // LC 2012/09/06 : Fill data to align with Minuit method.
  void         AlignData(DTrack *aTrack, const Float_t tiniBound, std::vector<Float_t>& myData);
  void         UpdatePositions(); // LC 2013/09/05 

  void         AlignPrecUpdateConfig();                            // LC 2012/09/06 : Update config file after Minuit.

  void         ShowAlignment();
  DStrip      *NearestStrip(DR3& aPosition);
  DSetup&      GetSetup()                         { return *fc;         }
/*   DAcqModule&  GetAcqModule()                     { return *fAcqModule; } */
/*   DInp&        GetInput()                         { return *fInput;     } */
  Int_t        GetMaxHits()                 const { return  fHitMax;    }
  
  Long_t       GetChannelsN()                     { return  fChannelsN; } 
  Int_t        GetStripsN()                       { return  fStripsN;   }
  Int_t        GetStripsNu()                      { return  fStripsNu;   }
  Int_t        GetStripsNv()                      { return  fStripsNv;   }
  void         ComputeStripPosition( Int_t col, Int_t lin, Double_t &u, Double_t &v, Double_t &w); // JB 2012/11/21
  void         ComputeStripPosition_UVToColRow( Double_t u, Double_t v, double &col, double &lin); // AP 2012/06/26

  Long_t       *GetStripIndices()                  { return  fStripIndices;       }
  Long_t       *GetChannelIndices()                { return  fChannel;            }
  Int_t     *GetRawData()                         { return  fRawData;            }
  vector<DPixel*> *GetListOfPixels()              { return  fListOfPixels; }
  vector<DMonteCarlo*> *GetListOfMonteCarlo()     { return   fListOfMonteCarlo; }
  Long_t&       GetChannel(Int_t aSk)              { return  fChannel[aSk-1];     }
  Float_t      GetCommonMode(Int_t aSk);
  Float_t      GetRawValue(Int_t aSk);
  Float_t      GetRawFrame1Value(Int_t aSk);
  Float_t      GetRawFrame2Value(Int_t aSk);
  Float_t      GetPedestal(Int_t aSk);
  Float_t      GetPulseHeight(Int_t aSk);
  Float_t      GetNoise(Int_t aSk);
  Float_t      GetPulseHeightToNoise(Int_t aSk);
  Float_t      GetPulseRangeMaximum();
  Int_t        GetInitialNoise()            const { return  fInitialNoise; } //JB, Sept 2007
  Int_t        GetCommonRegions()           const { return  fRegions;     }
  Int_t        GetAcqInputNumber()          const { return  fAcqInputNumber;};// VR 2014/07/21

  Int_t        Digitize( Float_t aValue ); // JB 2013/08/29
  void         DigitizeMatrix(); // JB 2013/08/29
  Int_t        GetDigitalThresholdNb()            { return fIfDigitize; }
  Int_t       *GetDigitialThresholds()            { return fDigitizeThresholds; }
  
  DAlign      *GetAlignment()                     { return  fAlign;       }
  DPrecAlign  *GetPrecAlignment()                 { return  fPrecAlign;   } // JB 2010/11/25
  DStrip      *GetStrip(Int_t aStripNumber); 
  DR3&         GetStripPitch()              const { return *fPitch;       }

  Int_t        GetHitsN()                   const { return  fHitsN;       }// VR 2014.08.28
  Int_t        GetHitsNewN()                const { return  fHitsNew;     }// VR 2014.08.28
  Int_t        GetHitsOldN()                const { return  fHitsOld;     }// VR 2014.08.28
  Int_t        GetHitsUnTrackedLastEventN() const { return  fHitsUnTrackedLastEventN;}// VR 2014.08.28
  void         SetHitsN                  (Int_t aNb) {fHitsN                  = aNb;}// VR 2014.08.28
  void         SetHitsNewN               (Int_t aNb) {fHitsNew                = aNb;}// VR 2014.08.28
  void         SetHitsOldN               (Int_t aNb) {fHitsOld                = aNb;}// VR 2014.08.28
  void         SetHitsUnTrackedLastEventN(Int_t aNb) {fHitsUnTrackedLastEventN= aNb;}// VR 2014.08.28
  
  DHit        *GetPrincipalHit();
 
  DHit        *GetHit(Int_t aHk)                   const { return  fHit[aHk-1];  }
  DHit        *GetHitUnTrackedLastEvent(Int_t aHk) const { return  fHitUnTrackedLastEvent[aHk-1];  }// VR 2014.08.28
  Char_t      *GetPlaneName()               const { return  fPlaneName;   }
  Char_t      *GetPlanePurpose()            const { return  fPlanePurpose;}
  Int_t        GetPlaneNumber()             const { return  fPlaneNumber; }
  Int_t        GetLadderNumber()            const { return  fLadderNumber;}
  DCut        *GetCut()                     const { return  fCut;         }
  Int_t       GetHitFinder()                const { return  fHitFinder;}//VR 2014/07/16

  Float_t      GetPositionU()               const { return  fPositionU;   }
  Float_t      GetPositionV()               const { return  fPositionV;   }
  Float_t      GetTiltW()                   const { return  fTiltW;       } // changed from Z to W, JB 2010/11/25

  void         SetPositionU(Float_t posU); 
  void         SetPositionV(Float_t posV);
  void         SetTiltW(Float_t tiltW);

  void ChangePositionByOffset(DR3& offset);//VR 2014/06/29
  void ChangePositionByValue(DR3& newvalue);//VR 2014/06/29
  void ChangeTiltByOffset(DR3& offset);//VR 2014/06/29
  void ChangeTiltByValue(DR3& newvalue);//VR 2014/06/29
  
  void PrintPosition(void);
  void PrintTilt(void);
  
  
  DR3&         GetPosition()                const { return *fPosition;    }
  DR3&         GetSize()                    const { return *fSize;        }
  DR3&         GetTilt()                    const { return *fTilt;        }
  Int_t        GetStatus()                  const { return  fStatus;      }
  Int_t        GetAnalysisMode()            const { return  fAnalysisMode;}
  Int_t        GetReadout()                 const { return  fReadout;}
  Int_t        GetMimoType()                const { return  fMimosaType;}//RDM210509
  void         SetStatus(Int_t aStatusValue){ fStatus = aStatusValue;     }
  DR3          TrackerToPlane(DR3& aTrackerCoordinate); // JB 2010/11/25
  DR3          PlaneToTracker(DR3& aPlaneCoordinate); // JB 2010/11/25
  DR3          Intersection(DTrack  *aTrack);
  DR3          DeformedLocalPoint(DR3& aPlaneCoordinate);
  // the five following methods are useless since we use DPrecAlign from 2010/11/25
  //DR3          Rotate_TrackerToPlane(DR3& aTrackerCoordinate);
  //DR3          Rotate_PlaneToTracker(DR3& aPlaneCoordinate);
  //void         SetPositionU(Float_t aU);
  //void         SetPositionV(Float_t aV);
  //void         SetTiltZ(Float_t aTilt);   // sets Z tilt [rad]
  void         SetCDSvariance(Float_t aVar);
  Float_t      GetCDSvariance()                   { return   fCDSvariance;   }
  Float_t      GetThreshold()                     { return  fThreshold;      }
  Int_t        GetOverThresholdN()                { return  fOverThresholdN; }
  Float_t      GetOverThresholdC()                { return  fOverThresholdC; }
  DTracker    *GetTracker()                       { return  fTracker;        }

  Float_t      GetCommonModeRegion(Int_t aRegion) const;

  TH1F        *GetChargeFractionUniv() {return fChargeFractionDensity;  }     
  TH1F        *GetEtaIntU() {return fEtaIntU;  }     
  TH1F        *GetEtaIntV() {return fEtaIntV;  }     
  TH1F        *GetEtaIntU2(){return fEtaIntU2;  }     
  TH1F        *GetEtaIntV2(){return fEtaIntV2;  }     

  TH1F        *GetChargeFractionDensity(Int_t tSk);     
  TH1F        *GetChargeFractionDistribution(Int_t tSk);
  Bool_t       GetStripResponseSetting()          { return  fDigitalStripResponse;  }

  //YV21/10/09
  TH2F        *GetHNoise() { return fHNoise; }  
  TH2F        *GetHPedestal() { return fHPedestal; }  
  Int_t       GetNoiseRun() {return fNoiseRun; }
  TFile       *GetNoiseFile() {return fNoiseFile; }

  void         SetDebug(Int_t aDebug);
  Int_t        GetDebug()    { return fDebugPlane;}
                             
  ClassDef(DPlane,3)                      // Detector Plane

};

#endif
