// @(#)maf/dtools:$Name:  $:$Id:DSetup.h  v.1 2005/10/02 18:03:46 sha Exp $
// Author   : ?

#ifndef _DSetup_included_
#define _DSetup_included_

  ////////////////////////////////////////////////////////////
  // Class Description of DSetup                            //
  //                                                        //
  // + Read Setup of the Acquisition, Telescope and Analysis//
  //        for a run.                                      //
  //                                                        //
  ////////////////////////////////////////////////////////////

#include "Riostream.h"

// ROOT classes
#include "TString.h"
#include "TObject.h"
#include "TVector.h"
#include "TFile.h"
#include "TMath.h"

#include "DSession.h"
#include "DR3.h"
#include "TSystem.h"

using namespace std;

#define fMaxSubmatrices 10
#define fMaxGeomatrices 10
#define fMaxModules 30
#define fMaxDigitalThresholds 16

class DSession;
class DR3;

class DSetup : public TObject {

 public:
  //Defining a region for multiplicity dependent resolution information, added AP 2014/11/20
  struct Region_t {
      int R_col[2];
      int R_lin[2];
  };


 private:
  void         copy(const DSetup& a);

  DSession    *fSession;             // pointer to the current session

  void         ReadRunParameters(); // READ functions added JB 2012/12/21
  void         ReadTrackerParameters();
  void         ReadExperimentGeometryParameters();
  void         ReadLadderParameters( Int_t aLadderNumber=-1);
  void         ReadGlobalAlignmentParameters();
  void         ReadPlaneParameters( Int_t aPlaneNumber=-1);
  void         ReadDAQParameters();
  void         ReadDAQBoardParameters(Int_t aBoardNumber=-1);
  void         ReadAnalysisParameters();
  void         ReadSubmatrixParameters(Int_t aSubmatrixNumber=-1);
  void         nextField();
  void         nextItem(Char_t delimiter);
  void         read_r3(DR3 &arg);
  void         read_item(Int_t &arg);
  void         read_item(UInt_t &arg);
  void         read_item(Float_t &arg);
  void         read_strings(Char_t *aString, Int_t aLength);
  void         read_TStrings(TString &TheString, Int_t aLength);
  void         getRidOfLine();
  void         read_list(std::vector<Int_t>& arg);
  ifstream     fConfigFileStream;

  TString      fConfigPath;                 // name of the configuration path
  TString      fConfigFileName;             // name of the configuration file
  TString      fConfigPathAndFileName;      // both path and file name appended

  Int_t        DSetupDebug;

  Int_t        fFieldMaxLength;
  Int_t        fAddedPlanes;                // counter of planes added in config
  Int_t        fAddedLadders;               // counter of ladders added in config
  Char_t       *fFieldName;

  struct APixel_t {
    int    col;
    int    lin;
    double fake;
  };

  std::vector<Float_t>  _ResolutionList;
  std::vector<Float_t>  _ResolutionUList;
  std::vector<Float_t>  _ResolutionVList;
  std::vector<Region_t> _RegionList;
  std::vector<TString>  _ResolutionFileList;
  std::vector<Float_t>  _FractionToMaskList;
  std::vector<Float_t>  _FakeRateCutList;

  void InitializePerformancesParams(int aPlaneNumber);

  void CheckRegionsOverlaps(int aPlaneNumber);
  void CheckForBadRegions(int aPlaneNumber);

  void GetListOfHotPixelsToMask_FracToMask(int aPlaneNumber,
				           TString HotPixelMapFile);
  void GetListOfHotPixelsToMask_FakeRateCut(int aPlaneNumber,
				           TString HotPixelMapFile);

  void PrintListOfHotPixelsToMask(int aPlaneNumber);

 public:

  DSetup();
  DSetup(DSession& aSession);
  DSetup(const DSetup& c);

  DSetup& operator=(const DSetup&);

  //~DSetup() {};                                // BEWARE No Destructor ! Structure Added, Destructor should be implemented, LC 2015/03/10
  ~DSetup();

  void         SetConfigPath(TString aCP) ;
  void         SetConfigFileName(TString aCFN) ;
  void         SetDebug(Int_t aDebug)             { DSetupDebug = aDebug; cout << "DSetup debug updated to " << DSetupDebug << endl;}
  DSession    *GetSession()                       { return fSession;}
  Int_t        GetDebug()                         { return DSetupDebug;}
  TString      GetConfigPath()                    { return fConfigPath; }

  void         ReadConfiguration();


  struct AnalysisParameter_t {
    bool    SavePlots;
    bool    DoTelResolutionMC;
    int     MCEvents;
    int     MCSeed;
    bool    MCDoDisplay;
    bool    DoGaussianMS;
    int     ResolutionScanSteps;
    Float_t ResolutionScanInit;
    Float_t ResolutionScanEnd;
    Int_t   CacheSize;
    Int_t   StatisticCells;
    Int_t   CmsNoiseCut;
    Int_t   MaxNbOfHits;
    Int_t   MinNbOfHits;
    Float_t TrackChi2Limit;
    Int_t   MinHitsPerTrack; // JB 2013/06/22
    Int_t   MaxTracksExGeom; // JB 2013/06/21
    Int_t   ExGeomatrix; // JB 2013/06/21
    Int_t   Submatrices;
    Float_t HistoChargeRange; // JB 2013/09/12
    Float_t HistoSNRRange;
    Float_t HistoNoiseRange;
    Char_t  AnalysisGoal[50]; // Jb 2014/01/16
    Float_t PixelSizeU[fMaxSubmatrices];
    Float_t PixelSizeV[fMaxSubmatrices];
    Int_t   PixelsInRaw[fMaxSubmatrices];
    Int_t   PixelsInColumn[fMaxSubmatrices];
    Int_t   Matrixtype[fMaxSubmatrices]; // JB 2013/07/17
    Int_t   MinSeedIndex[fMaxSubmatrices]; // JB 2013/08/21
    Int_t   MaxSeedIndex[fMaxSubmatrices]; // JB 2013/08/21
    Int_t   MinSeedCol[fMaxSubmatrices]; // JB 2013/08/22
    Int_t   MaxSeedCol[fMaxSubmatrices]; // JB 2013/08/22
    Int_t   MinSeedRow[fMaxSubmatrices]; // JB 2013/08/22
    Int_t   MaxSeedRow[fMaxSubmatrices]; // JB 2013/08/22
    Int_t   MaxNofPixelsInCluster[fMaxSubmatrices];
    Int_t   MinNofPixelsInCluster[fMaxSubmatrices]; // JB 2013/09/12
    Float_t MinSeedCharge[fMaxSubmatrices]; // JB 2013/11/08
    Float_t MinClusterCharge[fMaxSubmatrices]; // JB 2014/01/21
    Float_t MinNeighbourCharge[fMaxSubmatrices]; // JB 2013/11/08
    Float_t NoiseScope[fMaxSubmatrices];
    Float_t Calibration[fMaxSubmatrices];
    Int_t   Geomatrices[fMaxSubmatrices]; // JB 2013/01/16
    Float_t Umin[fMaxSubmatrices][fMaxGeomatrices];
    Float_t Umax[fMaxSubmatrices][fMaxGeomatrices];
    Float_t Vmin[fMaxSubmatrices][fMaxGeomatrices];
    Float_t Vmax[fMaxSubmatrices][fMaxGeomatrices];
    Int_t   UserFlag; // JB 2013/07/17
  };

  AnalysisParameter_t AnalysisParameter;
  AnalysisParameter_t& GetAnalysisPar(){return AnalysisParameter;}

  struct TrackerParameter_t {
    enum       {tpsz = 20};
    Int_t      Planes;                 // # planes in this tracker
    Int_t      Ladders;                // # ladders in this tracker
    Int_t      TracksMaximum;          // maximum number of tracks to be allowed
    Int_t      TracksFinder;            // method for track finding
    Int_t      TrackFittingAlg;         // method for track fitting
    Int_t      PlanesForTrackMinimum;  // min # planes to build a track in an event
    Int_t      HitsInPlaneTrackMaximum;// max # hits allowed per plane to do tracking
    Float_t    SearchHitDistance;      // max distance hit-track to add hit to track
    Float_t    SearchMoreHitDistance;  // max distance hit-track to add hit to a pre-track
    Int_t      HitsInPlaneMaximum;     // maximum number of hits per plane to be allowed
    Float_t    Resolution;             // estimated spatial resolution of ref planes
    TString    BeamType;               // nature of the particles making the beam
    Float_t    BeamMomentum;           // momentum of the beam particles (in GeV/c)
    TString    MediumMaterial;         // Material of the medium containing the sensors. E.g. DryAir of Vacuum
    Int_t      VertexMaximum;          // maximum number of tracks to be allowed
    Int_t      VertexConstraint;       // use a vertex constraint to start track
    Int_t      UseSlopeInTrackFinder;  // use the track slope to extrapolate track
    Int_t      TrackingPlaneOrderType; // the planes ordering type for finding tracks
    Int_t      EventsForAlignmentFit;  // minimum number of events to fit alignement parameters
    Int_t      TimeLimit;              // maximum frame length (in 10ns units) //RDM260609 from Float to Int RDM250809
    Int_t      HitMonteCarlo;          // Enable/Disable Hit Monte Carlo (Default = 0)
    Int_t      KeepUnTrackedHitsBetw2evts; // explicit // VR 2014.08.28
    Int_t      DPrecAlignMethod;        // Default : (0) Old method | (1) New method
    // For TracksFinder=2 :
    Int_t      TrackingPass;            // nb of pass in the tracking loop
    Int_t*     PreTrackHitsNbMinimum;   // explicit
    Int_t*     PreTrackHitsTypeUsed;    // explicit
    Float_t*   PreTrackHitsMaxDist;     // explicit
    Int_t*     ExtTrackHitsNbMinimum;   // explicit
    Int_t*     ExtTrackHitsTypeUsed;    // explicit
    Float_t*   ExtTrackHitsMaxDist;     // explicit
    Int_t*     FullTrackHitsNbMinimum;  // explicit
    Int_t      TrackingOrderLines;      // Number of Lines to define planes tracking order
    Int_t**    TrackingOrderPreTrack;   // Planes'order to build pre-tracks
    Int_t**    TrackingOrderExtTrack;   // Planes'order to build ext-tracks
    Int_t      SubtrackNplanes;         // Number of planes in subtrack, JB 2014/12/22
    Int_t*     SubtrackPlanes;          // List of planes used in subtrack
  };

  TrackerParameter_t TrackerParameter;
  TrackerParameter_t& GetTrackerPar(){return TrackerParameter;}

  struct IviGeometryParameter_t
  {
    enum        {tpsz = 256};
    Char_t    GeometryName[tpsz];
    Char_t    GeometryVersion[tpsz];
    DR3       BeamOrigin;
    DR3       BeamSlope;
    DR3       BeamDisplayStrongBegin;
    DR3       BeamDisplayStrongStop;
    DR3       BeamDisplayMediumBegin;
    DR3       BeamDisplayMediumStop;
    DR3       BeamDisplayLightBegin;
    DR3       BeamDisplayLightStop;
    Char_t    TargetType[tpsz];
    DR3       TargetSize;
    Float_t   TargetRadius;
    Float_t   TargetLength;
    Char_t    TargetAxis[1];
    DR3       TargetCenter;
    DR3       TrackerOrigin;
    DR3       TrackerTilt;
    Char_t    VertexingMethod[tpsz];
  };

  IviGeometryParameter_t IviGeometryParameter;
  IviGeometryParameter_t& GetIviGeometryParameter() {return IviGeometryParameter;}


  // Ladder information, added JB 2013/01/16
  struct LadderParameter_t {
    enum       {tpsz = 20};
    Int_t      LadderID;
    Int_t      Status;                // ladder status
    Char_t     Name[tpsz];            // name of device
    Int_t      Planes;                // # planes in this ladder
    DR3        Position;              // center position of the device in x,y,z system
    DR3        Tilt;                  // tilting angles [degree] in x,y,z system
    Int_t*     PlaneList;             // array of plane number associated
    DR3*       PlaneShift;            // array of shift vectors from plane center to ladder center
    DR3*       PlaneTilt;             // array of rotation vectors from plane orientation to ladder orientation
  };

  LadderParameter_t *pLadderParameter;
  LadderParameter_t& GetLadderPar(Int_t anID){return pLadderParameter[anID];}

  // Global Alignment Information, added LC 2015/08/31
  struct GlobalAlignmentParameter_t {
    enum       {tpsz = 20};
    Float_t      FixTrackParamX;
    Float_t      FixTrackParamY;
    Float_t      ResoTrackParamX;
    Float_t      ResoTrackParamY;
    Float_t      ResoAlignParamTr;
    Float_t      ResoAlignParamRot;
    Float_t      ResoFixPlaneTr;
    Float_t      ResoFixPlaneRot;
    Float_t      ResoAlignParamZ;
  };

  GlobalAlignmentParameter_t pGlobalAlignmentParameter;
  GlobalAlignmentParameter_t& GetGlobalAlignPar() {return pGlobalAlignmentParameter;}

  // Multiplicity dependent resolution informaton, added AP 2014/11/20
  struct PlanePerformances_t {
    Region_t Region;
    Float_t  GlobalPlaneResolution;
    Float_t  GlobalPlaneResolutionU;
    Float_t  GlobalPlaneResolutionV;
    std::vector<Float_t> MultProb;
    std::vector<Float_t> MultProbCumul;
    std::vector<Float_t> ResolutionU;
    std::vector<Float_t> ResolutionV;
  };

  struct PlaneParameter_t {
    enum       {tpsz = 20};
    Int_t      Inputs;                // Number of inputs used for this plane max 4, JB 2009/05/07
    Int_t      ModuleType[fMaxModules];        // the Module (Sirocco type 1, or 2 or something else)
    Int_t      ModuleNumber[fMaxModules];      // connected to which acquisition module number
    Int_t      InputNumber[fMaxModules];       // number of the input plug
    Int_t      ChannelNumber[fMaxModules];     // first strip nb associated to the first channel number for this input (start at 1)
    Int_t      ChannelOffset[fMaxModules];     // the number of the first channel related to the plane for this input (start at 1)
    Int_t      Channels[fMaxModules];          // Number of channels taken for this input
    Int_t      TimeLimit;             // limit in timestamp distance
    Char_t     Name[tpsz];            // name of device
    Char_t     Purpose[tpsz];         // purpose of device e.g. reference
    Int_t      Readout;               // readout status Alice128c
    Int_t      MimosaType;            // pitch of Mimo25 RDM210509
    Int_t      AnalysisMode;          // normal 0 , read noise file 1...
    Int_t      HitFinder;             // method for the hit finder2
    Int_t      FixedGlobalAlign;      // To fix the plane for global alignment. // LC 2015/08/20.
    Int_t      InitialPedestal;       // nb of events required for pedestal
    Int_t      InitialNoise;          // nb of events required for noise
    Int_t      CacheSize ;            // Size of the cache for the hit suppression in pedestal and noise computation
    Float_t    DistanceZero;          // distance to zero in [mm] // old
    DR3        Position;              // center position of the device in x,y,z system
    DR3        Tilt;                  // tilting angles [degree], u,v,w vectors rotated to x,y,z vectors
    Float_t    AlignmentU;            // offset perpendicular to strip direction, to be added to position
    Float_t    AlignmentV;            // offset parallel to strip direction, to be added to position
    Float_t    AlignmentTilt;         // w-tilting angle about 0 in the device coordinates
    DR3        Size;                  // assume rectangular shape, extensions in a,b,c
    DR3        Strips;                // number of strips in u,v,w directions
    DR3        Pitch;                 // pitch in mm in u,v,w directions
    DR3        StripSize;             // size of a strip in u,v,w directions
    Float_t    PlaneResolution;       // expected resolution, JB 2013/06/22
    Float_t    PlaneResolutionU;      // expected resolution in U, AP 2014/11/20
    Float_t    PlaneResolutionV;      // expected resolution in V, AP 2014/11/20
    Float_t    PlaneThickness;        // plane thickness in mu,    AP 2015/03/10
    TString    PlaneMaterial;         // plane material,           AP 2015/03/10
    bool       UsingTrackerResolution; //Bool to decide if using tracker resolution
    std::vector<PlanePerformances_t>  PlanePerformancesList;  //Resolution map vs multiplicity, added AP 2014/11/20
    Int_t      Mapping;               // 980106: only Mapping = 1 is supported
    Float_t    MaximalNoise;          // Maximum noise for seed strip
    Float_t    MinimalNoise;          // Minimal noise for seed strip
    Float_t    ThreshNeighbourSN;     // threshold of mean Signal-to-Noise on neighbour strips (adjcnt seed)
    Float_t    ThreshSeedSN;          // threshold of mean Signal-to-Noise on seed strip
    Int_t      MaxNStrips;            // maximum number of strips in the cluster
    Int_t      MinNStrips;            // maximum number of strips in the cluster
    DR3        ClusterLimit;          // maximum extension of clusters in u,v,w direction
    Float_t    ClusterLimitRadius;    // maximum radius from center of gravity to associate a pixel to a cluster
    Int_t      CommonRegions;         // number of regions for common mode/shift correction (2 in case of 2 VA)
    Int_t      Status;                // Status: Primary Reference = 1., Secondary Reference = 2. Test = 3.
    Int_t      ParentLadderID;        // ID of parent ladder, -1 if none, JB 2013/01/16
    Int_t      HitPositionAlgorithm;  // 1= Center of Gravity, 2 = eta, 3 = kappa
    Int_t      EtaCoefficientsN;      // number of eta correction coefficients
    Float_t    EtaCoefficient[tpsz];  // the coefficients
    Float_t    EtaLowLimit;           // use eta correction from lower limit
    Float_t    EtaHighLimit;          // .. to high limit
    Int_t      KappaCoefficientsN;    // number of kappa correction coefficients
    Float_t    KappaCoefficient[tpsz];  // the coefficients
    Float_t    KappaLowLimit;         // use kappa correction from lower limit
    Float_t    KappaHighLimit;        // .. to high limi
    Int_t      GammaCoefficientsN;    // number of gamma correction coefficients
    Float_t    GammaCoefficient[tpsz];// the coefficients
    Float_t    GammaLowLimit;         // kappa-to-eta limit, low limit
    Float_t    GammaHighLimit;        // eta-to-kappa limit, high limit
    Int_t      NoisyStripsN;          // number of noisy strips
    Float_t    NoisyStripsIndex[tpsz];  //  noisy strips index
    Int_t      IfDigitize;            // >0 if to emulate digitization over IfDigitization(<=4) bits
    Int_t      DigitizeThresholds[fMaxDigitalThresholds]; // 2^(IfDigitization) thresholds to emulate digitization
    Int_t      IfDeformed;            // >0 to take deformation into account
    Float_t    CoeffLegendreU[7];     // Deformation coeff, U-direction
    Float_t    CoeffLegendreV[7];     // Deformation coeff, V-direction

    std::vector<Int_t> HotPixelList_lin;     //List of hot pixels: line
    std::vector<Int_t> HotPixelList_col;     //List of hot pixels: column
    std::vector<Int_t> HotPixelList_index;   //List of hot pixels: index
  };

  PlaneParameter_t  *pPlaneParameter;
  PlaneParameter_t& GetPlanePar(Int_t aPN) {return pPlaneParameter[aPN-1];}

  Int_t**          ChannelUse;       //! pointer to bits on good channels
  Int_t*           ChannelAllUse;    //! use all channels in DAcq

  struct AcqParameter_t {
    Int_t      FileHeaderSize;       // size of the FileHeader
    Int_t      EventBufferSize;      // size given by ExaByte format
    Int_t      FileHeaderLine;       // size of event header in Bytes
    Int_t      EventTrailerSize;     // size of event trailer in Bytes
    Int_t      ModuleTypes;          // number of acquisition modul types
                                     // e.g. a Sirocco of Type A, B, or LBL-Pixel device
    Int_t      BinaryCoding;         // 0 for BigEndian, 1 for LittleEndian
    Int_t      TriggerMode;          // Expect a Trigger (1) or not (0) to separate event, JB 2010/08/23
    Int_t      EventBuildingMode;    // SS 2011.11.14
    Char_t     TimeRefFile[100];     // JB 2018/02/11
    Int_t      IfExternalTimeRef;
  } AcqParameter;

  AcqParameter_t& GetAcqPar(){return AcqParameter;}

  struct AcqModuleParameter_t {
    enum       {tpsz = 100};
    Char_t     Name[tpsz];           // Name of the Acquisition Module (e.g. Sirocco Type A)
    Int_t      Devices;              // Quantity of Devices of this Module type
    Int_t      Type;                 // Type of this Module ( e.g. 1 = Sirocco Type A... )
    Int_t      EventBuildingBoardMode;    // JB 2013.06.22
    Int_t      Inputs;               // Number of Inputs per Module
    Int_t      Channels[fMaxModules];// Number of Channels acquired via an input
    Int_t      Bits[fMaxModules];    // Number of Bits reserved for value
    Int_t      SigBits[fMaxModules]; // Number of significant bits, coding the value
    Int_t      NColumns;             // Number of columns of sensor
    Int_t      NMultiFrames;         // Nb of frames stored when MultiFraming
    Char_t    *DeviceDataFile[fMaxModules];   // Data file name for each device, JB 2009/05/25
    Int_t      NbOfFramesPerChannel[fMaxModules];
    Int_t      PixelShiftMod[fMaxModules]; // JB 2015/05/12
    Int_t      FirstTriggerChannel;
    Int_t      LastTriggerChannel;
    Int_t      PixelShift; // JB,CB,PLR 2015/03/24
    Int_t      AmpOffset; // JB,CB,PLR 2015/03/24
    Float_t    AmpFactor; // JB,CB,PLR 2015/03/24
    UInt_t     Trailer; // JB,CB,PLR 2015/03/24
    TString    MCTreeName; //AP 2016/05/18
    Int_t      IfZeroSuppress;        // >0 if to suppress rawdata values below a threshold
    Int_t      ThresholdZero;         // threshold for zero suppression
  };

  //Char_t*      GetModuleDataFile( Int_t aMod) { return pModuleDataFile[aMod]; }

  AcqModuleParameter_t  *pAcqModuleParameter;  //! don''t put in Streamer
  AcqModuleParameter_t& GetModulePar(Int_t aMTN) { return pAcqModuleParameter[aMTN-1]; }

  struct RunParameter_t {
    enum       {tpsz = 250};
    Char_t     Affiliation[tpsz];    // your group
    Char_t     Signature[tpsz];      // whom to blame on this analysis result
    Char_t     BeamTime[tpsz];       // when this data was taken
    Char_t     Confidence[tpsz*3];     // state of alignement or other comments
    Char_t     DataPath[tpsz*3];       // Path to the data
    Char_t     DataSubDirPrefix[tpsz];// Prefix of the subdir that contains data files, concatenated with run number
    Char_t     Extension[tpsz];      // Extension for a data file
    Int_t      Number;               // Run Number to be analysed as a String
    Int_t      EventsInFile;         // How many events are in a file
    Int_t      StartIndex;           // start index of the file for processing
    Int_t      EndIndex;             // end index
    Int_t      NoiseRun;             // Run number of noise run YV 27/11/09
    Int_t      PixelGainRun;         // Run number of gain calibration JB 2018/07/04
    //    Int_t      FileCountOut;         // maximum number of files possible
  } RunParameter;

  RunParameter_t& GetRunPar()         {return RunParameter;}


  ClassDef(DSetup,2)   // Configuration of the Telescope and Detectors under Test

};

#endif
