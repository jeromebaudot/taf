// @(#)maf/dtools:$Name:  $:$Id:DSetup.cxx  v.1 2005/10/02 18:03:46 sha Exp $
// Author   : ?
// Last Modified, JB, 2008/10/13
// Last Modified, JB, 2009/05/07
// Last Modified, JB, 2009/05/25
// Last Modified, RDM, 2009/08/25
// Last Modified, JB, 2009/08/25 add configuration for MimosaPro analysis
// Last Modified, JB, 2009/08/26 allow for multi-submatrix
// Last Modified, JB, 2009/09/01 new param PlanesForTrackMinimum
// Last Modified, JB, 2010/08/23 new param TriggerMode for AcqParameter
// Last Modified, JB, 2010/09/06 new param NoiseScope for AnalysisParameter
// Last Modified, JB, 2011/04/12 SetResultsPath
// Last Modified, JB, 2011/07/07 to localize path names
// Last Modified, SS, 2011/11/14 to initialize eventBuildingMode
// Last Modified, JB, 2012/04/25 debugLevel>1 to dump original config file
// Last Modified, JB, 2013/01/16 new parsing methods, compatible with ANY config file format
// Last Modified, JB, 2013/06/20 ReadDAQBoardParameters
// Last Modified, JB, 2013/06/21 ReadAnalysisParameters, ReadTrackerParameters
// Last Modified, JB, 2013/07/17 almost all ReadXXX methods
// Last Modified, JB, 2013/08/13-22 ReadPlaneParameters, ReadDAQBoardParameters
// Last Modified, JB, 2013/08/29 ReadPlaneParameters
// Last Modified, JB, 2013/09/12 ReadSubmatrixParameters, ReadAnalysisParameters
// Last Modified, JB, 2013/11/08 ReadSubmatrixParameters
// Last Modified, JB, 2014/01/07 ReadPlaneParameters
// Last Modified, JB, 2014/01/21 ReadSubmatrixParameters
// Last Modified, JB, 2014/04/21 ReadPlaneParameters
// Last Modified: VR, 2014/06/29 add the SearchMoreHitDistance parameter to tracker paramameters
// Last Modified: VR, 2014/06/30 run number is get from DSession , not read anymore from config file
// Last Modified: VR, 2014/06/30 add the DataSubDirPrefix config in RunParameters
// Last Modified: VR, 2014/07/14 add the TrackingPlaneOrderType parameter
// Last Modified: VR, 2014/07/16 new string field comparison method : strstr replaced by strcmp to prevent error with fields string that contains another field string + Add WARNING when a field is not understood
// Last Modified: VR, 2014/07/16 Add field ClusterLimitRadius for DHit::Analyse_2_cog()
// Last Modified: AP, 2014/07/16 Added two new input parameters for each plane:
//                               - A .root file name with the map of the fake rate
//                               - A cut on the single pixel fake rate to identify a list of hot pixels
// Last Modified: AP, 2014/07/16 Added a list of hotpixels (two lists, one for line and one for column)
//                               This list is filled whenever the .root file is espesified/exist, and the
//                               FakeRateCut is in (0,1)
//                               At the end print out the list of hot pixels per plane
// Last Modified: VR, 2014/08/28 add mecanism to keep untracked hits from last event
// Last Modified: VR, 2014/08/29 add TrackingOrderLines, TrackingOrderPreTrack and TrackingOrderExtTrack : defines planes scan order during tracking for TracksFinder 2
// Last Modified: VR, 2014/10/30 add the PreTrackHitsNbMinimum and HitsNbMinForFullTrack parameters
// Last Modified: VR, 2014/12/18 modify parameters for TracksFinder 2
// Last Modified: JB, 2014/12/23 add Subtrack parameters in ReadTrackerParameters
// Last Modified: AP, 2015/03/10 add to parameters to the plane parameters block: PlaneThickness and PlaneMaterial.
//                               To be used for MC studies of telescope resolution.
// Last Modified: AP, 2015/03/10 add to tracker parameters block: BeamType and BeamMomentum
//
// Last Modified: JB+CB+PRL, 2015/03/24 new params for INFN ReadDAQBoard
// Last Modified: LC, 2015/10/14 GlobalAlignment Parameter Reading : Now optional
// Last Modified: JB 2017/11/20 ReadDAQBoardParameters for zero suppression option
// Last Modified: JB 2018/07/04 ReadRunParameters, added PixelGainRun parameter
// Last Modified: JB 2021/05/01 Handle source path as datapath

///////////////////////////////////////////////////////////////
// Class Description of DSetup                               //
//                                                           //
// Read Setup of the Acquisition, Telescope and Analysis
//  from the configuration file (text format),
//  defined from the run number.
//
// All the parameters need not to be present in the text
//  file, but if they are indeed needed to run
//  they should be initialized !
// ** WARNING ** currently (June 2013) the situation is not cleaned
//  and many paramameters are not initialized.
//
// This is the list of all known parameters by category: (in construction)
//  MANDATORY means this parameter has to be in the config file
//  1st parameter means this parameter shall be the first in the list
//  --> if one of these 2 rules is broken, the file parsing will crash!
//
// #############################################################################
//                              Run Parameters
// #############################################################################
// Affiliation      = [optional] (char) {""} Labs involved in the data taking
// Signature        = [optional] (char) {""} One or several names
// BeamTime         = [optional] (char) {""} A date, any format
// Confidence       = [optional] (char) {""} Possibly a brief description of the data taking
// DataPath         = [MANDATORY](char)      directory of the raw data
// DataSubDirPrefix = [optional] (char) {""} sub directory for data files, stored in DataPath/DataSubDirPrefixXXX/ with XXX the run number
// Extension        = [optional] (char) {""}
// RunNumber        = [OBSOLETE]   RunNumber is get from DSession, set via gTAF->InitSession(...)
// EventsInFile     = [MANDATORY for IMGBoardReader] (int) {0}
// StartIndex       = [MANDATORY for IMGBoardReader] (int) {0}
// EndIndex         = [MANDATORY for IMGBoardReader] (int) {0}
// NoiseRun         = [optional] (int) {0}  defines either the file with the noise for each pixel, or a specific method to remove noisy pixels (see DGlobalTools::VetoPixels)
// PixelGainRun     = [optional] (int) {0}  if not 0, defines the file with individual pixel gain map
// -----------------------------------------------------------------------------


// #############################################################################
//                         Parameters of the Tracker
// #############################################################################
// "Planes" or "Ladders" has to be the first field
// ----------------------------------
//     Planes and their parameters
// ----------------------------------
// Planes                  = [MANDATORY] (int)       the nb of planes in the setup
// Ladders                 = [optional]  (int) {0}   the nb of ladders in the setup (may encompass several sensors)
// TimeLimit               = [optional]  (int) {-1}   for sensor with timestamping, defines the maximum duration of a triggered event
// Resolution              = [optional]  (float,um) {-1.} the knonw spatial resolution in um of the reference planes, use 4.00 for MIMOSA 26 and 1.00 for MIMOSA 18
// BeamType                = [optional]  (TString)  {"pion"} type of the beam particles
// BeamMomentum            = [optional]  (float,GeV/c) {120.0GeV/c} momentum of the beam particles
// MediumMaterial          = [optional]  (TString) {"vacuum"} material of the medium containing the sensors: e.g. DryAir or Vacuum
//
// ----------------------------------
//     Clustering in planes
// ----------------------------------
// HitsInPlaneMaximum      = [MANDATORY] (int)       the nb hits which will be reconstruted in each plane, 0 : not clustering
// KeepUnTrackedHitsBetw2evts =[optional]   (int) {0}
//                                                  1 memorise untracked hits between 2 evenements
//
// ----------------------------------
//     Tracking parameters
// ----------------------------------
// TracksMaximum           = [optional]  (int) {0}   the maximum nb of tracks which will be reconstructed, if set to 0, no tracking is performed
// TracksFinder            = [optional]  (int) {0}   select the tracking method
//                           0 for the original track finder : find_tracks()
//                           1 for more options: find_tracks_1_opt() allows to order plane
//                           2 for the one adapted for Interaction Vertex Imaging: find_tracks_2_ivi()
// TrackFittingAlg         = [optional]  (int) {0}: if 0, Use the default chi-square fitting; if 1, Enable MKalmanFilter fitting; if >1, not defined, use default
// HitsInPlaneTrackMaximum = [MANDATORY if TracksMaximum!=0 && TracksFinder==0] (int)
// PlanesForTrackMinimum   = [MANDATORY if TracksMaximum!=0 && TracksFinder==0 ] (int)
//                                                   the min nb hits required to make a track
//                                                   the max nb hits in a plane allowed to consider using its hits for tracking
// SearchHitDistance       = [MANDATORY if TracksMaximum<2] (float,um)
//                                                   the search distance to associate a hit to a track
// UseSlopeInTrackFinder   = [optional for TracksFinder=0]  (int) {1}
// SearchMoreHitDistance   = [MANDATORY if TracksMaximum<2]   (float,um)
//                                                   the search distance to associate a hit to a pre-track
//                                                   if 1, use the track slope to extrapolate track or if 0, use "flat" slope
// TrackingPlaneOrderType  = [optional for TracksFinder=1]  (int) {0} Try to make a track with looking on planes ...
//                                                   0 in the same order than in the config file
//                                                   1 ordered with the status (0,1,2)
// SubtrackNplanes         = [optional] {0}: if non-zero indicates that each track will be refitted with a subset of the planes
// SubtrackPlanes          = [MANDATORY if SubtrackNplanes!=0] list of planes (separated by ":" to be used by subtrack
// HitMonteCarlo           = [optional] (int) {0}:
// DPrecAlignMethod        = [optional] (int) {0} 0=Old DPrecAlign, 1=New DrecAlign -> Redifinitions of Matrices, Rotations and Plane Equations.
//
// ----------------------------------
//     Tracking parameters specifics for TracksFinder=2, all MANDATORY
// ----------------------------------
// context : PreTrack  = config. of algo. to build the start track (=pre-track) only made with hits in pre-track'planes
// context : ExtTrack  = config. of algo. to try to extend the pre-track with hits in ext-track'planes
// context : FullTrack = config. of algo. for the full track (can be a pre-track not extended !)
// -----------------------
//     Tracking pass
// -----------------------
// context : HitsNbMinimum = minimum number of hits to create/extend/save a pre/ext/full track
// context : HitsTypeUsed  = 0:any kind ; 1:only new hits ; 2:only memorized hits
// context : HitsMaxDist   = distance between hit/track and another hit to add it to the current track [um]
// context : [Pre/Ext]TrackPARAM = {valuePass1 valuePass2 ...}
//
// TrackingPass            = (int) number of pass of the tracking algo
// PreTrackHitsNbMinimum   = {(int)}
// PreTrackHitsTypeUsed    = {(int)}
// PreTrackHitsMaxDist     = {(float)}
// ExtTrackHitsNbMinimum   = {(int)}
// ExtTrackHitsTypeUsed    = {(int)}
// ExtTrackHitsMaxDist     = {(float)}
// FullTrackHitsNbMinimum  = {(int)}
//
// -----------------------
// Tracking Planes Order
// -----------------------
// context : TrackingOrderLineX:   [NbPlanesPreTrack]{PTpl1 PTpl2 ...} [NbPlanesExtTrack]{ETpl1 ETpl2 ETpl3 ...}
//
// TrackingOrderLines      = (int) number of lines like TrackingOrderLine1, ...,
// TrackingOrderLineN      = definition : TrackingOrderLineN:   [P#] {P1 P2 P3 ...} [P#] {P6 P7 P8 ...}
//                               example : TrackingOrderLine1:   [3] {5 6 7} [4] {1 2 3 4}
//                               means   : There is [3] planes to build a pre-track    : 5,6 and 7
//                                              and [4] planes to build the ext-track : 1,2,3 and 4

// ----------------------------------
//     Vertexing parameters
// ----------------------------------
// VertexMaximum           = [optional]  (int) {0}
//                          0 : no vertexing,
//                          1 : vertices will be searched with method find_tracks_and_vertex
// VertexConstraint        = [optional]  (int) {0}   use a vertex constraint to start track
//
// ----------------------------------
//     Alignement parameters
// ----------------------------------
// EventsForAlignmentFit   = [optional]  (int) {0}   the nb pairs (track-hit) for one iteration of the alignment procedure


// #############################################################################
//                         Parameters of the Ladder
// #############################################################################
// "LadderID" has to be the first field
// LadderID            = [MANDATORY] (int), ID of the ladder
// LadderName          = [MANDATORY] (char), name AND type of the ladder, used to define plane positions
//                  could be "Plume", "Simple", "Salat", "Custom"
// Status              = [MANDATORY] (int), same meaning as for Plane,
// LadderPositionX/Y/Z = [MANDATORY] (float,mm), same meaning as for Plane,
// LadderTiltX/Y/Z     = [MANDATORY] (float,deg), same meaning as for Plane,
// NbOfPlanes          = [MANDATORY] (int), nb of planes associated to this ladder
// If using "Custom", you need to define each planes associated
//   with the following lines
//  IncludedPlane   = [MANDATORY if "Custom"] (int) the plane number as defined later in the configuration
//  PlaneShiftU,V,W = [MANDATORY if "Custom"] (float,mm) relative shifts of this plane wrt ladder center
//  PlaneTiltU,V,W  = [MANDATORY if "Custom"] (float,deg) relative tilts of this plane wrt ladder center
//
// NOTE:
//  The positions/tilts of planes belonging to a ladder are defined
//   by the ladder definition. Any position settings of these planes
//   in the configuration files are superseeded.
//

// #############################################################################
//                      Parameters For Global Alignment
// #############################################################################
//
// Added : LC 31/08/2015
// Track constraints : to fix the telescope geometry
// "FixTrackParamX" has to be the first field
// FixTrackParamX = value  // value = 0 for telescope case !=0 if tilted telescope
// FixTrackParamY = value  // value = 0 for telescope case !=0 if titled telescope
// ResoTrackParamX = value // SPS  : value = 10-5 || DESY : value = 10-3
// ResoTrackParamY = value // SPS  : value = 10-5 || DESY : value = 10-3
// ResoAlignParamTr  = value // By default : reso = 100um                // Translations values set to initial translations
// ResoAlignParamRot = value // By default : reso = 0.1 rad = 5.7 deg    // Rotations values set to initial rotations
//

// #############################################################################
//                    Parameters for each detector plane
// #############################################################################
// "Inputs" has to be the first field
//
// ----------------------------------
//     Data decoding
// ----------------------------------
// Inputs          = [MANDATORY] (int), 1st parameter = number of inputs needed to build all the channels,
//    -> for each input, specify in the following order:
//      ModuleType    = [MANDATORY] (int) index for the desired type of modules
//                      according to the declaration order in Acquisition module
//      ModuleNumber  = [MANDATORY] (int) id in the set of modules of this type
//      InputNumber   = [MANDATORY] (int) id of the input of this module used
//      ChannelNumber = [MANDATORY] (int) channel shift so that
//                      plane_channel_nb = input_channel_nb + ChannelNumber
//      ChannelOffset = [optional] (int) {1} first channel in the input related
//                       to the plane
//      Channels      = [MANDATORY] (int) number of channels to read from this input
// StripselUse     = [obsolete] used to define dead strips
//
// ----------------------------------
//     Readout and Analysis of pixels
// ----------------------------------
// Name            = [MANDATORY] (char) just for display for now
// Purppose        = [MANDATORY] (char) just for display for now
// ParentLadder      = [optional] (int) indicate if the plane belong to a ladder
//                                 and which one
// Readout         = [MANDATORY] (int) specifies the type of raw data
//                   0 -> not read,
//                   1<Readout<100 -> data not sparsified,
//                   100<Readout -> sparsified data.
// AnalysisMode    = [MANDATORY] (int) controls the analysis
//                   0 -> data read but no clustering,
//                   1 -> strips,
//                   2 -> pixels with analog output,
//                   3 -> pixels with binary output.
// MimosaType      = [optional] (int) {0} not clear (sorry!)
// FixedGlobalAlign= [optional] (int) {0} to fix the plan for global alignment procedure
// InitialPedestal = [obsolete] (int) superseded by InitialNoise
//                   # events to analyze before the first computation
// InitialNoise    = [MANDATORY only if Readout<100] (int)
//                   # events to analyze before the first computation
// CacheSize       = [MANDATORY only if Readout<100] (int)
//                   size of the set of events from which one is picked
//                   for computing the noise and pedestal
// HotPixelMapFile = [optional] (char) ROOT file name with fake rate map
//   FakeRateCut     = [MANDATORY if HotPixelMapFile] (float) Single pixel fake rate cut
// IfDigitize      = [optional] (int) {0} # thresholds to emulate the digitization
//                   0 (default) means no-digitization
//     DigitizeThresholds = [MANDATORY if IfDigitize>0] (array of int)
//                          as many values as indicated, separated with ':'
//
// ----------------------------------
//     Position and size
// ----------------------------------
// PositionsXYZ    = [MANDATORY] (3 float,mm) position of the center of the plane,
//                    changed by the alignment procedure
// TiltZYX         = [MANDATORY] (3 float,deg)
//                    rotation angles defining plane orientation,
//                    changed by the alignment procedure
// PitchUVW        = [MANDATORY] (3 float,um) pitch in all directions
//                   (pitchW=sensitive layer thickness, not used yet)
// StripsUVW       = [MANDATORY] (3 int) # collecting noted in all directions
// SizeUVW         = [obsolete] computed from PitchUVW and StripsUVW
//                    size of the sensitive area, set to PitchUVW if not provided
// StripSizeUVW    = [optional] (3 float,um) not used yet
// AlignementU/V   = [obsolete] (float,mm) old alignment shift parameter
// AlignementTilt  = [obsolete] (float,deg) old alignment angle parameter
// Deformed        = [optional] (int) {0} if 0 then no deformation applied,
//                     if 1 then deformation applied from following coeff.
//  coeffLegendreU = [MANDATORY if Deformed==1] (6 floats) values (separated by ":")
//                     coeff of the 6 first Legendre polynomials
//                    used to parametrize delat_W with respect to U coordinate
//  coeffLegendreV = [MANDATORY if Deformed==1] (6 floats) 6 values (separated by ":")
//                     coeff of the 6 first Legendre polynomials
//                    used to parametrize delat_W with respect to V coordinate
// Mapping         = [MANDATORY] (int) drives pixel position computation,
//                   1 -> position at pixel center,
//                   2 -> position considers staggering
//                   3,4,5 -> ?
// Status            = [MANDATORY] controls how this plane is used by the tracking
//                 0 = Primary Reference, never aligned and used as track seed,
//                 1 = Primary Reference, never aligned and used in tracking (not for seed)
//                 2 = Secondary Reference, aligned and used in tracking (not for seed)
//                 3 = Device Under Test (DUT), aligned but never used in tracking
//
// ----------------------------------
//     Cluster (=Hit) finder
// ----------------------------------
// HitFinder       = [optional] (int) {0} select the hit finder method,
//                   0 -> standard
//                   1 -> connected pixel
//                   2 -> cog based with search radius, requires additional parameter
// ThreshNeighbourSN = [MANDATORY] (float) S/N or S cut on all the pixels
//                     (seed excluded) in the cluster for the hit finding
// ThreshSeedSN      = [MANDATORY] (float) S/N or S cut on the seed pixel
//                     for the hit finding
// MaxNStrips        = [optional] (int) {1000} maximal #strips allowed in cluster
//                     (corrected or set automaticaly by DCut depending on HitFinder)
// MinNStrips        = [optional] (int) {1} minimal #strips required in cluster
//                     (corrected or set automaticaly by DCut depending on HitFinder)
// ClusterLimitU     = [MANDATORY if HitFinder!=2] (float,um) maximal distance
//                     between the seed pixel and any other pixel in the hit
// ClusterLimitRadius= [MANDATORY if HitFinder==2] (float,um) maximal distance
//                     between the center of gravity and any other pixel in the hit
// CommonRegions     = [optional] (int) {1} # regions to define
//                     for the common noise shift computation per event
// HitPositionAlgorithm = [MANDATORY] (int) controls how the hit position
//                     is estimated from the pixels info
//                 1 = Center of Gravity,
//                 2 = eta,
//                 3 = kappa (not implemented yet)
// PlaneResolution   = [optional] (float,um) expected plane resolution in both direction
// PlaneResolutionU  = [optional] (float,um) expected plane resolution in U direction
// PlaneResolutionV  = [optional] (float,um) expected plane resolution in V direction
// ResolutionFile    = [optional] (char) not implemented yet
// ResolutionRegion  = [optional] (?) not implemented yet
// PlaneThickness    = [optional] (float,um) plane thickness. To be used for MC studies of telescope resolution
// PlaneMaterial     = [optional] (char)     plane material. Used to calculate multiple scattering angle from database.
//                                                           To be used for MC studies of telescope resolution.
//
//


// #############################################################################
//                    Parameters for DAQ
// #############################################################################
// "AcqModuleTypes" or "FileHeaderSize" has to be the first field
// AcqModuleTypes     = [MANDATORY] (int) # different boards used in the DAQ
// TriggerMode        = [optional] (int) method to deal with trigger info
//                      0 -> ignore trigger info
//                      1 -> each new trigger generates an event
//                      2 -> trigger info is used to start or stop event reading
//			                3 -> ignore trigger info but all events are 2 frames long
// EventBuildingMode  = [obsolote] (int) {0} replaced by EventBuildingBoardMode
// BinaryCoding       = [optional] (int) {0} 0 for one Endian, 1 for the other
// FileHeaderSize     = [obsolete] (int) {0} size of additional header file
// EventBufferSize    = [optional depends on DAQboard type, see below] (int) event size in Bytes
// FileHeaderLine     = [optional] (int) {0} event header size in Bytes
// EventTrailerSize   = [optional] (int) {4} event trailer size in Bytes
// TimeRefFile        = [optional] (char) name of the file where to get external time reference

// #############################################################################
//                    Parameters for each Acquisition module type
// #############################################################################
// "Name" has to be the first field
// Name                  = [MANDATORY] (char) generic name of such modules
//                        known names: "IMG", "TNT", "PXI", "PXIe", "GIG", "VME"
//                                     "DecoderM18", "ALI22", "DecoderGeant", "IHEP", "MC"
// Type                  = [MANDATORY] (int) unique identifier for the module type
// Devices               = [MANDATORY] (int) # module instances of this type,
//                        typically, one instance decode one file
// Inputs                = [optional] (int) # identical data block (one per sensor typically)
// Channels              = [optional] (int) # channels in one input (data block)
// EventBuildingBoardMode= [optional] (int) used to pass a flag
// Bits                  = [optional] (int) size in bits of words to read from file
// SignificantBits       = [optional] (int) # significant bits per word
// NColumns              = [optional] (int) # number of columns of sensor
// FirstTriggerChannel   = [optional] (int)
// LastTriggerChannel    = [optional] (int)
// NbOfFramesPerChannel  = [optional] (int) # frames stored for each trigger/event
// DataFile or DataFile1 = [optional] (char) core name of data file
//                          if not provided, TAF will look for files like:
//                            RUN_XXXX_N.Extension, run_XXXX_N.Ext, RUNXXXX_N.Ext, runXXXX_N.Ext,
//                            RUN_XXXX.Ext, runXXXX.Ext, XXXX_N.Ext, XXXX.Ext
//                            where XXXX is the run number and N is the file number
// IfZeroSuppress  = [optional] (int) {0} if non-zero rawdata are zero-suppress
//     ThresholdZero = [MANDATORY if IfZeroSuppress>0] threshold value for zero-suppression
// MCTreeName            = [optional] (char) tree name in the MC generated file
//
// --- Name: "IMG"
//  Type = 10 for pixels / 11 for strips / 12 for pixels with multi-frame
//         13 for pixels with specifi arrangement of frame-ref & frame-signal
//         14 for 16 parallel outputs
//  Inputs             = [MANDATORY] (int) # identical data block (one per sensor typically)
//  EventsInFile       = [MANDATORY] (int) expected # events in a file
//  StartIndex         = [MANDATORY] (int) index of first data file
//  EndIndex           = [MANDATORY] (int) index of last data file
//  Extension          = [MANDATORY] (char) {"rz"} extension of data file
//  TriggerMode        = [MANDATORY] (int) method to deal with trigger info
//  EventBuildingBoardMode or EventBuildingMode = [MANDATORY]
//  EventBufferSize    = [MANDATORY] (int)
//  FileHeaderLine     = [MANDATORY] (int)
//  Bits: [MANDATORY] (int) if negative
//  SignificantBits: [MANDATORY] (int) if negative
//  NColumns:        [MANDATORY] (int)
//  NMultiFrames:    [MANDATORY] (int) nb of frames registered when multiFraming
//  DataFile: [MANDATORY] (char) typically "RUN_32844_"
//
// --- Name: "TNT"
//  Type = 30 or 31
//  StartIndex         = [MANDATORY] (int) index of first data file
//  EndIndex           = [MANDATORY] (int) index of last data file
//  Extension          = [MANDATORY] (char) {"rz"} extension of data file
//  TimeLimit          = [MANDATORY] (int)
//  EventBufferSize    = [MANDATORY] (int)
//  TriggerMode        = [MANDATORY] (int) method to deal with trigger info
//  BinaryCoding       = [MANDATORY] (int)
//  Bits               = [MANDATORY] (int)
//  SignificantBits    = [MANDATORY] (int)
//  DataFile           = [MANDATORY] (char) typically "CardXXXX_000"
//  TriggerMode ->unused
//  EventBuildingBoardMode -> unused
//  FirstTriggerChannel, LastTriggerChannel, NbOfFramesPerChannel -> unused
//
// --- Name: "PXI"
//  Type = 40
//  TriggerMode        = [MANDATORY] (int) method to deal with trigger info
//  BinaryCoding       = [MANDATORY] (int)
//  DataFile           = [MANDATORY] typically "run_XXXX_"
//  EventBufferSize, FileHeaderLine -> unused
//  Channels, Bits, SignificantBits, EventBuildingBoardMode -> unused
//  FirstTriggerChannel, LastTriggerChannel, NbOfFramesPerChannel -> unused
//
// --- Name: "PXIe"
//  Type = 50
//  TriggerMode        = [MANDATORY] (int) method to deal with trigger info
//  BinaryCoding       = [MANDATORY] (int)
//  EventBuildingBoardMode
//  EventBufferSize, FileHeaderLine -> unused
//  Channels, Bits, SignificantBits -> unused
//  FirstTriggerChannel, LastTriggerChannel, NbOfFramesPerChannel, DataFile -> unused
//
// --- Name: "GIG"
//  Type = 60
//  TriggerMode, BinaryCoding, EventBufferSize, FileHeaderLine -> unused
//  Channels, Bits, SignificantBits, EventBuildingBoardMode -> unused
//  FirstTriggerChannel, LastTriggerChannel NbOfFramesPerChannel, DataFile -> unused
//
// --- Name: "VME"
//  Type = 70
//  Extension        = [MANDATORY] (char) {"rz"} extension of data file
//  Inputs           = [MANDATORY] (int) # identical data block (one per sensor typically)
//  DataFile         = [MANDATORY] (char) typically "FIFOdata_M28_RUN3_ch"
//  TriggerMode, BinaryCoding, EventBufferSize, FileHeaderLine -> unused
//  Channels, Bits, SignificantBits, EventBuildingBoardMode -> unused
//  FirstTriggerChannel, LastTriggerChannel NbOfFramesPerChannel -> unused
//
// --- Name: "ALI22"
//  Type = 80
//  DataFile         = [MANDATORY] (char) typically "FIFOdata_M22"
//  NbOfFramesPerChannel = [MANDATORY] (int) # frames stored for each trigger/event
//  TriggerMode, BinaryCoding, EventBufferSize, FileHeaderLine -> unused
//  Channels, Bits, SignificantBits, EventBuildingBoardMode -> unused
// FirstTriggerChannel, LastTriggerChannel, NbOfFramesPerChannel -> unused
//
// --- Name: "DecoderM18"
//  Type = 90
//  Extension       = [MANDATORY] (char) {"dat"} extension of data file
//  DataFile        = [MANDATORY] (char) typically "SIS3301dataZS_ch"
//  TriggerMode, BinaryCoding, EventBufferSize, FileHeaderLine -> unused
//  Channels, Bits, SignificantBits, EventBuildingBoardMode -> unused
// FirstTriggerChannel, LastTriggerChannel, NbOfFramesPerChannel -> unused
// PixelShift       = [Optional] (int) {3} (in)famous kShift
// PixelShiftMod    = [Optional] (int) {3} (in)famous kShift defined per Module
// AmpOffset        = [Optional] (int) {32768} values subtracted to pulseheight
// AmpFactor        = [Optional] (float) {1.} multiplier of the pulseheight
// Trailer          = [Optional] {int) {0xfafafafa} expected frame tailer

//
// --- Name: "DecoderGeant"
//  Type = 100
//  DataFile        = [MANDATORY] (char) typically "FILEdata_Geant_RUN1_ch"
//  TriggerMode, BinaryCoding, EventBufferSize, FileHeaderLine -> unused
//  Inputs, Channels, Bits, SignificantBits, EventBuildingBoardMode -> unused
//  FirstTriggerChannel, LastTriggerChannelNbOfFramesPerChannel -> unused
//

// --- Name: "MC"
//  Type = 110
//  Inputs          = [MANDATORY] (int) at least 1
//  DataFile        = [MANDATORY] (char) typically "FILEdata_Geant_RUN1_ch"
//  MCTreeName      = [MANDATORY] (char) tree name in the MC generated file
//  TriggerMode, BinaryCoding, EventBufferSize, FileHeaderLine -> unused
//  Channels, Bits, SignificantBits, EventBuildingBoardMode -> unused
//  FirstTriggerChannel, LastTriggerChannelNbOfFramesPerChannel -> unused
//


// #############################################################################
//                      Parameter for Final Analysis
// #############################################################################
// "AnalysisGoal" or "MaxNbOfHits" or "StatisticCells" or "CmsNoiseCut" has to be the first field
// AnalysisGoal     = [optional] (char) {""} drives type of histograms created, could be:
//              cluster, track, calib, laser, vertex, fake, vector
// SavePlots        = [optional] (int) {0} set to 1 to save all plots into a PDF file
// StatisticCells   = [obsolete]
// CmsNoiseCut      = [optional] (float) {3} upper SNR cut to use a pixel value in the CMS computation
// MaxNbOfHits      = [MANDATORY] (int) max # hits allowed to consider events for analysis
// MinNbOfHits      = [MANDATORY] (int) min # hits allowed to consider events for analysis
// TrackChi2Limit   = [MANDATORY] (float) upper chi2 cut to consider a track for analysis
// MinHitsPerTrack  = [optional] (int) minimum #hits per track (default is set with PlanesForTrackMinimum)
// MaxTracksExGeom  = [optional] (int) {-1} inclusive max # tracks allowed in the ExGeomatrix below, set to -1 cancel this cut
// ExGeomatrix      = [optional] (int) {0} geomatrix (of submatrix 0) to be used in the previous cut
// UserFlag         = [optional] (int) whatever you want to use in User's stuff
// HistoChargeRange = [optional] (float) {5000} max charge displayed in histo
// HistoSNRRange    = [optional] (float) {250} max SNR displayed in histo
// HistoNoiseRange  = [optional] (int) {40} max noise displayed in histo
// Submatrices      = [MANDATORY] (int)  # submatrices defined below, at least 1 shall be specified
//
// Then for each submatrix declared
//   PixelSizeU     = [MANDATORY] (float)
//   PixelSizeV     = [MANDATORY] (float)
//   PixelsInRaw    = [MANDATORY] (int)
//   PixelsInColumn = [MANDATORY] (int)
//   Matrixtype     = [optional] (int) {1} defines the mapping pixel-position
//                1 = orthogonal pixel network,
//                2 = staggered pixel network for elongated pixel,
//                3,4,5 = ?
//   MaxNofPixelsInCluster = [optional] (int) {0} maximum # pixel allowed to consider a cluster for analysis, 0 means not used
//   MinNofPixelsInCluster = [optional] (int) {1} minimum # pixel allowed to consider a cluster for analysis
//   MinSeedCharge         = [optional] (int) {-1000} minimal charge on the seed pixel to consider a cluster, units depends on calibration
//   MinClusterCharge      = [optional] (int) {-1000} minimal total charge to consider a cluster, units depends on calibration
//   MinNeighbourCharge    = [optional] (int) {-1000} minimal charge on pixels neighbouring the seed pixel, units depends on calibration
//   MinSeedIndex          = [optional] (int) {0} defines limit index of pixels to take into account
//   MaxSeedIndex          = [optional] (int) {0} defines limit index of pixels to take into account
//   MinSeedCol            = [optional] (int) {0} defines limit index of col to take into account
//   MaxSeedCol            = [optional] (int) {0} defines limit index of col to take into account
//   MinSeedRow            = [optional] (int) {0} defines limit index of row to take into account
//   MaxSeedRow            = [optional] (int) {0} defines limit index of row to take into account
//   Calibration           = [optional] (float) {1.} conversion factor between ADCunit and electrons
//   NoiseScope            = [MANDATORY] (float) noise multiplication factor, if 0. -> noise not used
//   4 geomatrix shall be specified, they define 4 ROI (region of interest)
//     GeoMatrix0/1/2/3 = [MANDATORY] (float,um)  minU: maxU: minV: maxV
//
//                                                        //
////////////////////////////////////////////////////////////

#include "DSetup.h"
#include "DSession.h"
#include "DR3.h"
#include "DGlobalTools.h"
#include "TApplication.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TROOT.h"
#include "TVector2.h"

#include <assert.h>

ClassImp(DSetup) // DSetup of Test Beam Setup and Data Structure


DSetup::DSetup() {

    DSetupDebug = 0;
    fFieldMaxLength = 100;
    fFieldName = new Char_t[fFieldMaxLength];

}
//______________________________________________________________________________
//
DSetup::DSetup(DSession& aSession)
{
  // Modified JB 2012/12/20 new member data "fieldName"

  fSession = &aSession;

  fFieldMaxLength = 100;
  fFieldName = new Char_t[fFieldMaxLength];

}
//______________________________________________________________________________
//
void  DSetup::SetConfigPath(TString aCP)
{

  fConfigPath = aCP;
  if(DSetupDebug) cout << "DSetup::SetConfigPath "<< aCP << endl;

}
//______________________________________________________________________________
//
void DSetup::SetConfigFileName(TString aCFN)
{

  fConfigFileName = aCFN;
  if(DSetupDebug) cout << "DSetup::SetConfigFileName "<< aCFN << endl;

}
//______________________________________________________________________________
//
void  DSetup::SetSourcePath(TString aSP)
{

  fSourcePath = aSP;
  if(DSetupDebug) cout << "DSetup::SetSourcePath "<< aSP << endl;

}
//______________________________________________________________________________
//
void DSetup::ReadRunParameters()
{

  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // Run Parameter
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  //
  // JB 2013/01/16
  // Modified JB 2013/08/19: default parameters setup
  // Modified VR 2014/06/30: RunNumber is not read anymore, taken from DSession
  // Modified VR 2014/06/30: Add the DataSubDirPrefix and test of mandatory config
  // Modified JB 2018/07/04: Add PixelGainRun parameter

  cout << endl << " - Reading Run parameters" << endl;

  // ################################################
  //            Set default values
  // ################################################
  // Set default values for optional or mandatory parameters
  RunParameter.Number = -1;
  sprintf(RunParameter.Affiliation,"");
  sprintf(RunParameter.Signature,"");
  sprintf(RunParameter.BeamTime,"");
  sprintf(RunParameter.Confidence,"");
  sprintf(RunParameter.DataPath,"-1");// [MANDATORY]
  sprintf(RunParameter.DataSubDirPrefix,"");
  sprintf(RunParameter.Extension,"");
  RunParameter.EventsInFile = 0;// [MANDATORY for IMGBoardReader]
  RunParameter.StartIndex = 0;// [MANDATORY for IMGBoardReader]
  RunParameter.EndIndex = 0;// [MANDATORY for IMGBoardReader]
  RunParameter.NoiseRun = 0;
  RunParameter.PixelGainRun = 0;


  do {

    nextField();

    if ( ! strcmp( fFieldName, "Affiliation" ) ) {
      read_strings( RunParameter.Affiliation, RunParameter.tpsz);
    }
    else if( ! strcmp( fFieldName, "Signature" ) ) {
      read_strings( RunParameter.Signature, RunParameter.tpsz);
    }
    else if( ! strcmp( fFieldName, "BeamTime" ) ) {
      read_strings( RunParameter.BeamTime, RunParameter.tpsz);
    }
    else if( ! strcmp( fFieldName, "Confidence" ) ) {
      read_strings( RunParameter.Confidence, RunParameter.tpsz);
    }
    else if( ! strcmp( fFieldName, "DataPath" ) ) {
      if( fSourcePath.IsNull() ) { // Read the path only if not set already
        read_strings( RunParameter.DataPath, RunParameter.tpsz);
      } else {
        sprintf( RunParameter.DataPath, "%s", fSourcePath.Data());
      }
      DGlobalTools aTool; // JB 2011/07/18
      sprintf( RunParameter.DataPath, "%s", aTool.LocalizeDirName( RunParameter.DataPath)); // JB 2011/07/07
    }
    else if( ! strcmp( fFieldName, "DataSubDirPrefix" ) ) { // VR 2014/06/30
      read_strings( RunParameter.DataSubDirPrefix, RunParameter.tpsz);
    }
    else if( ! strcmp( fFieldName, "Extension" ) ) {
      read_strings( RunParameter.Extension, RunParameter.tpsz);
    }
    //VR 2014/06/30 RunNumber is set from DSession
    else if( ! strcmp( fFieldName, "RunNumber" ) ) {
      cout << "WARNING : parameter 'RunNumber' in config file is ignored, because it is redundant with InitSession command argument !" << endl;
      getRidOfLine();
    }
    else if( ! strcmp( fFieldName, "EventsInFile" ) ) {
      read_item(RunParameter.EventsInFile);
    }
    else if( ! strcmp( fFieldName, "StartIndex" ) ) {
      read_item(RunParameter.StartIndex);
    }
    else if( ! strcmp( fFieldName, "EndIndex" ) ) {
      read_item(RunParameter.EndIndex);
    }
    else if( ! strcmp( fFieldName, "NoiseRun" ) ) {
      read_item(RunParameter.NoiseRun);       //YV 27/11/09
    }
    else if( ! strcmp( fFieldName, "PixelGainRun" ) ) {
      read_item(RunParameter.PixelGainRun);   //JB 2018/07/04
    }
    else
    {
      if ( strcmp( fFieldName, "Planes") && strcmp( fFieldName, "Ladders") )
      {
        cout << "WARNING : parameter '" << fFieldName << "' in config file is not understood !" << endl;
        getRidOfLine();
      }
    }
  } while ( strcmp( fFieldName, "Planes") && strcmp( fFieldName, "Ladders") );


  // Test of MANDATORY field settings
  // VR 20014/06/30
  if ( !strcmp(RunParameter.DataPath,"-1"))
  {
    cout << " ERROR: The field 'DataPath' is MANDATORY in config file (section run parameters)" << endl;
    gApplication->Terminate();
  }

}
//______________________________________________________________________________
//
void DSetup::ReadTrackerParameters()
{

  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // Parameters of the Tracker
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  //
  // JB 2013/01/16
  // Modified: LC 2013/??/?? new parameters for vertex finder
  // Modified: JB 2013/06/21 new parameters to control track_finder
  // Modified: JB 2013/07/17 new parameters to select track_finder method
  // Modified: VR 2014/06/29 add the SearchMoreHitDistance parameter and test of mandatory config
  // Modified: VR 2014/10/30 add the PreTrackHitsNbMinimum and HitsNbMinForFullTrack parameters and test of mandatory config
  // Modified: VR 2014/12/18 modify all parameters for track_finder 2 (IVI)
  // Modified: JB 2014/12/23 add Subtrack parameters
  // Modified: LC 2015/01/?? add HitMonteCarlo
  // Modified: LC 2015/01/31 add DprecAlignMethod

  cout << endl << " - Reading Parameters of the Tracker" << endl;


  // ################################################
  //            Set default values
  // ################################################
  // *****************************
  //       Planes config
  // *****************************
  TrackerParameter.Planes       = 0;
  TrackerParameter.Ladders      = 0;
  TrackerParameter.TimeLimit    = -1;
  TrackerParameter.Resolution   = -1.;
  TrackerParameter.BeamType     = TString("");
  TrackerParameter.BeamMomentum = 0.0;
  TrackerParameter.MediumMaterial = TString("");
  TrackerParameter.HitsInPlaneMaximum = -1 ;//[MANDATORY]

  TrackerParameter.TracksMaximum = 0;
  TrackerParameter.TracksFinder = 0;
  TrackerParameter.TrackFittingAlg = 0; // QL 2016/05/26
  TrackerParameter.PlanesForTrackMinimum = -1;//[MANDATORY if TracksMaximum!=0 && TracksFinder==0 ]
  TrackerParameter.HitsInPlaneTrackMaximum = -1;//[MANDATORY if TracksMaximum!=0 && TracksFinder==0 ]
  TrackerParameter.SearchHitDistance = -1.;//[MANDATORY if TracksMaximum<2]
  TrackerParameter.SearchMoreHitDistance = -1.;//[MANDATORY if TracksMaximum<2]
  TrackerParameter.UseSlopeInTrackFinder = 1;
  TrackerParameter.TrackingPlaneOrderType = 0;
  TrackerParameter.KeepUnTrackedHitsBetw2evts = 0;
  TrackerParameter.HitMonteCarlo = 0; // LC 2015/01
  TrackerParameter.DPrecAlignMethod = 0; // LC 2015/01/31

  // *****************************
  //  Tracking with track_finder 2
  // *****************************
  // all MANDATORY
  TrackerParameter.TrackingPass = -1;
  TrackerParameter.PreTrackHitsNbMinimum  = NULL;
  TrackerParameter.PreTrackHitsTypeUsed   = NULL;
  TrackerParameter.PreTrackHitsMaxDist    = NULL;
  TrackerParameter.ExtTrackHitsNbMinimum  = NULL;
  TrackerParameter.ExtTrackHitsTypeUsed   = NULL;
  TrackerParameter.ExtTrackHitsMaxDist    = NULL;
  TrackerParameter.FullTrackHitsNbMinimum = NULL;

  TrackerParameter.TrackingOrderLines     = -1;
  TrackerParameter.TrackingOrderPreTrack = NULL;
  TrackerParameter.TrackingOrderExtTrack = NULL;

  TrackerParameter.SubtrackNplanes = 0; // JB 2014/12/23
  TrackerParameter.SubtrackPlanes = NULL;

  // *****************************
  //          Vertexing
  // *****************************
  TrackerParameter.VertexMaximum = 0; // no vertexing by default
  TrackerParameter.VertexConstraint = 0; // use a vertex constraint to start track
  // *****************************
  //         Alignment
  // *****************************
  TrackerParameter.EventsForAlignmentFit = 0;


  do {
    // ----------------------------------
    //     Planes and their parameters
    // ----------------------------------
    if ( ! strcmp( fFieldName, "Planes") ) {
      read_item(TrackerParameter.Planes);
    }
    else if ( ! strcmp( fFieldName, "Ladders") ) {
      read_item(TrackerParameter.Ladders);
    }
    else if( ! strcmp( fFieldName, "TimeLimit" ) ) {
      read_item(TrackerParameter.TimeLimit); //RDM260609
    }
    else if( ! strcmp( fFieldName, "Resolution" ) ) {
      read_item(TrackerParameter.Resolution); // RDM250809
    }
    else if( ! strcmp( fFieldName, "BeamMomentum" ) ) {
      read_item(TrackerParameter.BeamMomentum); // 2015/03/10, AP Beam momentum in GeV/c
    }
    else if( ! strcmp( fFieldName, "BeamType" ) ) {  // 2015/03/10, AP Beam type: e.g. e+/-, pi+/-, ...
      read_TStrings( TrackerParameter.BeamType, 350);
    }
    else if( ! strcmp( fFieldName, "MediumMaterial" ) ) {  // 2015/06/01, AP Material of the medium containing the sensors: e.g. Dry air, or Vacuum
      read_TStrings( TrackerParameter.MediumMaterial, 350);
    }

    // ----------------------------------
    //     Clustering in planes
    // ----------------------------------
    else if( ! strcmp( fFieldName, "HitsInPlaneMaximum" ) ) {
      read_item(TrackerParameter.HitsInPlaneMaximum);
    }
    // ----------------------------------
    //     Tracking parameters
    // ----------------------------------
    else if( ! strcmp( fFieldName, "TracksMaximum" ) ) {
      read_item(TrackerParameter.TracksMaximum);
    }
    else if( ! strcmp( fFieldName, "TracksFinder" ) ) { // JB 2013/07/17
      read_item(TrackerParameter.TracksFinder);
    }
    else if( ! strcmp( fFieldName, "TrackFittingAlg" ) ) { // QL 2016/05/26
      read_item(TrackerParameter.TrackFittingAlg);
    }
    else if( ! strcmp( fFieldName, "PlanesForTrackMinimum" ) ) {
      read_item(TrackerParameter.PlanesForTrackMinimum);
    }
    else if( ! strcmp( fFieldName, "HitsInPlaneTrackMaximum" ) ) {
      read_item(TrackerParameter.HitsInPlaneTrackMaximum);
    }
    else if( ! strcmp( fFieldName, "SearchHitDistance" ) ) {
      read_item(TrackerParameter.SearchHitDistance);
    }
    else if( ! strcmp( fFieldName, "SearchMoreHitDistance" ) ) {
      read_item(TrackerParameter.SearchMoreHitDistance);
    }
    else if( ! strcmp( fFieldName, "UseSlopeInTrackFinder" ) ) { // JB 2013/06/21
      read_item(TrackerParameter.UseSlopeInTrackFinder);
    }
    else if( ! strcmp( fFieldName, "TrackingPlaneOrderType" ) ) { // VR 2014/07/14
      read_item(TrackerParameter.TrackingPlaneOrderType);
    }
    else if( ! strcmp( fFieldName, "KeepUnTrackedHitsBetw2evts" ) ) { // VR 2014/08/26
      read_item(TrackerParameter.KeepUnTrackedHitsBetw2evts);
    }
    else if( ! strcmp( fFieldName, "HitMonteCarlo" ) ) {
      read_item(TrackerParameter.HitMonteCarlo);
    }
    else if( ! strcmp( fFieldName, "DPrecAlignMethod" ) ) {
      read_item(TrackerParameter.DPrecAlignMethod);
    }
    // -------------------------------------------
    //     Tracking parameters for track_finder 2
    // -------------------------------------------
    // -*-*-*- Tracking Pass -*-*-*-
    else if( ! strcmp( fFieldName, "TrackingPass" ) )
    {
      read_item(TrackerParameter.TrackingPass);
      cout << "    -> Tracking pass = " << TrackerParameter.TrackingPass << endl ;

      if(TrackerParameter.TrackingPass>0)
      {
        Int_t nbOfReadingLoop = 0 ;
        while(kTRUE)
        {
          // Exit the loop
          if(TrackerParameter.PreTrackHitsNbMinimum  && \
	     TrackerParameter.PreTrackHitsTypeUsed   && \
	     TrackerParameter.PreTrackHitsMaxDist    && \
	     TrackerParameter.ExtTrackHitsNbMinimum  && \
	     TrackerParameter.ExtTrackHitsTypeUsed   && \
	     TrackerParameter.ExtTrackHitsMaxDist    && \
	     TrackerParameter.FullTrackHitsNbMinimum) {
	    cout << "      -> Reading finished ! " << endl ;
	    break;
	  }

          if(nbOfReadingLoop > 20) {
            cout << "      -> ERROR: There is (a) missing parameter(s) : " << endl;
            if(! TrackerParameter.PreTrackHitsNbMinimum)  cout << "          * PreTrackHitsNbMinimum  list is not defined !" << endl ;
            if(! TrackerParameter.PreTrackHitsTypeUsed)   cout << "          * PreTrackHitsTypeUsed   list is not defined !" << endl ;
            if(! TrackerParameter.PreTrackHitsMaxDist)    cout << "          * PreTrackHitsMaxDist    list is not defined !" << endl ;
            if(! TrackerParameter.ExtTrackHitsNbMinimum)  cout << "          * ExtTrackHitsNbMinimum  list is not defined !" << endl ;
            if(! TrackerParameter.ExtTrackHitsTypeUsed)   cout << "          * ExtTrackHitsTypeUsed   list is not defined !" << endl ;
            if(! TrackerParameter.ExtTrackHitsMaxDist)    cout << "          * ExtTrackHitsMaxDist    list is not defined !" << endl ;
            if(! TrackerParameter.FullTrackHitsNbMinimum) cout << "          * FullTrackHitsNbMinimum list is not defined !" << endl ;

            gApplication->Terminate();
          }

          nextField();
          nbOfReadingLoop++;

          if ( ! strcmp( fFieldName, "PreTrackHitsNbMinimum") )
          {
            TrackerParameter.PreTrackHitsNbMinimum  = new Int_t[TrackerParameter.TrackingPass];
            cout << "      -> PreTrackHitsNbMinimum {" ;
            nextItem('{');
            for(Int_t iPass=1 ; iPass <=TrackerParameter.TrackingPass ; iPass++)
            {
              read_item(TrackerParameter.PreTrackHitsNbMinimum[iPass-1]);
              cout << " " << TrackerParameter.PreTrackHitsNbMinimum[iPass-1] << " ";
            }
            cout << "}" << endl;
            nextItem('}');
          }
          else if ( ! strcmp( fFieldName, "PreTrackHitsTypeUsed") )
          {
            TrackerParameter.PreTrackHitsTypeUsed   = new Int_t[TrackerParameter.TrackingPass];
            cout << "      -> PreTrackHitsTypeUsed {" ;
            nextItem('{');
            for(Int_t iPass=1 ; iPass <=TrackerParameter.TrackingPass ; iPass++)
            {
              read_item(TrackerParameter.PreTrackHitsTypeUsed[iPass-1]);
              cout << " " << TrackerParameter.PreTrackHitsTypeUsed[iPass-1] << " ";
            }
            cout << "}" << endl;
            nextItem('}');
          }
          else if ( ! strcmp( fFieldName, "PreTrackHitsMaxDist") )
          {
            TrackerParameter.PreTrackHitsMaxDist    = new Float_t[TrackerParameter.TrackingPass];
            cout << "      -> PreTrackHitsMaxDist {" ;
            nextItem('{');
            for(Int_t iPass=1 ; iPass <=TrackerParameter.TrackingPass ; iPass++)
            {
              read_item(TrackerParameter.PreTrackHitsMaxDist[iPass-1]);
              cout << " " << TrackerParameter.PreTrackHitsMaxDist[iPass-1] << " ";
            }
            cout << "}" << endl;
            nextItem('}');
          }
          else if ( ! strcmp( fFieldName, "ExtTrackHitsNbMinimum") )
          {
            TrackerParameter.ExtTrackHitsNbMinimum  = new Int_t[TrackerParameter.TrackingPass];
            cout << "      -> ExtTrackHitsNbMinimum {" ;
            nextItem('{');
            for(Int_t iPass=1 ; iPass <=TrackerParameter.TrackingPass ; iPass++)
            {
              read_item(TrackerParameter.ExtTrackHitsNbMinimum[iPass-1]);
              cout << " " << TrackerParameter.ExtTrackHitsNbMinimum[iPass-1] << " ";
            }
            cout << "}" << endl;
            nextItem('}');
          }
          else if ( ! strcmp( fFieldName, "ExtTrackHitsTypeUsed") )
          {
            TrackerParameter.ExtTrackHitsTypeUsed   = new Int_t[TrackerParameter.TrackingPass];
            cout << "      -> ExtTrackHitsTypeUsed {" ;
            nextItem('{');
            for(Int_t iPass=1 ; iPass <=TrackerParameter.TrackingPass ; iPass++)
            {
              read_item(TrackerParameter.ExtTrackHitsTypeUsed[iPass-1]);
              cout << " " << TrackerParameter.ExtTrackHitsTypeUsed[iPass-1] << " ";
            }
            cout << "}" << endl;
            nextItem('}');
          }
          else if ( ! strcmp( fFieldName, "ExtTrackHitsMaxDist") )
          {
            TrackerParameter.ExtTrackHitsMaxDist    = new Float_t[TrackerParameter.TrackingPass];
            cout << "      -> ExtTrackHitsMaxDist {" ;
            nextItem('{');
            for(Int_t iPass=1 ; iPass <=TrackerParameter.TrackingPass ; iPass++)
            {
              read_item(TrackerParameter.ExtTrackHitsMaxDist[iPass-1]);
              cout << " " << TrackerParameter.ExtTrackHitsMaxDist[iPass-1] << " ";
            }
            cout << "}" << endl;
            nextItem('}');
          }
          else if ( ! strcmp( fFieldName, "FullTrackHitsNbMinimum") )
          {
            TrackerParameter.FullTrackHitsNbMinimum = new Int_t[TrackerParameter.TrackingPass];
            cout << "      -> FullTrackHitsNbMinimum {" ;
            nextItem('{');
            for(Int_t iPass=1 ; iPass <=TrackerParameter.TrackingPass ; iPass++)
            {
              read_item(TrackerParameter.FullTrackHitsNbMinimum[iPass-1]);
              cout << " " << TrackerParameter.FullTrackHitsNbMinimum[iPass-1] << " ";
            }
            cout << "}" << endl;
            nextItem('}');
          }
        }
      }
      else
      {
        cout << "      -> ERROR: The field 'TrackingPass' must be >0 ! " << endl;
        gApplication->Terminate();
      }
    } // end if tracking pass


    // -*-*-*- Tracking Order -*-*-*-
    else if( ! strcmp( fFieldName, "TrackingOrderLines" ) )
    {
      read_item(TrackerParameter.TrackingOrderLines);
      cout << "    -> TrackingOrderLines = " << TrackerParameter.TrackingOrderLines << endl ;
      if(TrackerParameter.TrackingOrderLines>0)
      {
        TrackerParameter.TrackingOrderPreTrack = new Int_t*[TrackerParameter.TrackingOrderLines];
        TrackerParameter.TrackingOrderExtTrack = new Int_t*[TrackerParameter.TrackingOrderLines];

        Char_t field[100];
        Int_t  aNbOfPlanes;
        for(Int_t iOrder=1 ; iOrder <=TrackerParameter.TrackingOrderLines ; iOrder++)
        {
          nextField();
          sprintf(field, "TrackingOrderLine%d",iOrder);
          cout << "      -> Reading TrackingOrderLine " << iOrder << endl ;
          if( !strcmp( fFieldName, field ))
          {
            nextItem('[');// pre-track planes #
            read_item(aNbOfPlanes);
            nextItem(']');
            cout << "        -> Planes for pre-track = " << aNbOfPlanes << " : { ";
            TrackerParameter.TrackingOrderPreTrack[iOrder-1] = new Int_t[aNbOfPlanes+1];
            TrackerParameter.TrackingOrderPreTrack[iOrder-1][0] = aNbOfPlanes;

            nextItem('{');// pre-track planes enumeration starts
            for (Int_t iOrder2=1 ; iOrder2<=aNbOfPlanes ; iOrder2++)
            {
              read_item(TrackerParameter.TrackingOrderPreTrack[iOrder-1][iOrder2]);
              cout << TrackerParameter.TrackingOrderPreTrack[iOrder-1][iOrder2] << " ";
            }
            nextItem('}');// pre-track planes enumeration finished
            cout << "}" << endl;

            nextItem('[');// ful-track planes #
            read_item(aNbOfPlanes);
            nextItem(']');
            cout << "        -> Planes for ext-track = " << aNbOfPlanes << " : { ";
            TrackerParameter.TrackingOrderExtTrack[iOrder-1] = new Int_t[aNbOfPlanes+1];
            TrackerParameter.TrackingOrderExtTrack[iOrder-1][0] = aNbOfPlanes;

            nextItem('{');// ful-track planes enumeration starts
            for (Int_t iOrder2=1 ; iOrder2<=aNbOfPlanes ; iOrder2++)
            {
              read_item(TrackerParameter.TrackingOrderExtTrack[iOrder-1][iOrder2]);
              cout << TrackerParameter.TrackingOrderExtTrack[iOrder-1][iOrder2] << " ";
            }
            nextItem('}');// pre-track planes enumeration finished
            cout << "}" << endl ;
          }
          else
          {
            cout << "        -> Error : can't find line " << field << " in config file" << endl;
            gApplication->Terminate();
          }
        }
	cout << "      -> Reading finished ! " << endl ;
      }
      else
      {
        cout << "      -> ERROR: The field 'TrackingOrderLines' must be >0 ! " << endl;
        gApplication->Terminate();
      }
    } // end if tracking order


    // -*-*-*- Subtrack -*-*-*-
    // JB 2014/12/23
    else if( ! strcmp( fFieldName, "SubtrackNplanes" ) || ! strcmp( fFieldName, "SubtrackNPlanes" ) ) { // if subtrack
      read_item(TrackerParameter.SubtrackNplanes);
      if ( TrackerParameter.SubtrackNplanes>0 ) {
        TrackerParameter.SubtrackPlanes = new Int_t[TrackerParameter.SubtrackNplanes];
        cout << "  Creating subtrack with " << TrackerParameter.SubtrackNplanes << " planes requested" << endl << "   -> list of planes for subtrack: ";
        nextField();
        if ( ! strcmp( fFieldName, "SubtrackPlanes" ) ) {
          for ( Int_t iplane=0; iplane<TrackerParameter.SubtrackNplanes; iplane++) {
            if( iplane!=0 ) nextItem(':');
            read_item(TrackerParameter.SubtrackPlanes[iplane]);
            cout << TrackerParameter.SubtrackPlanes[iplane] << " ";
          }
          cout << endl;
        }
        else {
          cout << " WARNING: did not detect 'SubtrackPlanes' field so switching SubtrackNplanes to 0!" << endl;
          TrackerParameter.SubtrackNplanes = 0;
        }
      }
    } // end if subtrack


    // ----------------------------------
    //     Vertexing parameters
    // ----------------------------------
    else if( ! strcmp( fFieldName, "VertexMaximum" ) ) { // JB 2013/06/11
      std::cout<<fFieldName<<std::endl;
      read_item(TrackerParameter.VertexMaximum);
    }
    else if( ! strcmp( fFieldName, "VertexConstraint" ) ) { // JB 2013/06/21
      read_item(TrackerParameter.VertexConstraint);
    }
    // ----------------------------------
    //     Alignement parameters
    // ----------------------------------
    else if( ! strcmp( fFieldName, "EventsForAlignmentFit" ) ) {
      read_item(TrackerParameter.EventsForAlignmentFit);
    }
    else
    {
      if (strcmp( fFieldName, "Inputs") &&  strcmp( fFieldName, "LadderID" ))
      {
        cout << "WARNING : parameter '" << fFieldName << "' in config file is not understood !" << endl;
        getRidOfLine();
      }
    }
    nextField();

  } while ( strcmp( fFieldName, "Inputs") &&  strcmp( fFieldName, "LadderID" ) &&  strcmp( fFieldName, "GeometryName" ) &&  strcmp( fFieldName, "FixTrackParamX" ) );

  if(TrackerParameter.BeamType == TString("")) {
    TrackerParameter.BeamType = TString("pion");
    cout << "    BeamType        parameter not specified. Setting it to defaul value:  " << TrackerParameter.BeamType.Data() << endl;
  }
  else {
    cout << "    BeamType        parameter set to user value: " << TrackerParameter.BeamType.Data() << endl;
  }
  if(TrackerParameter.BeamMomentum == 0.0) {
    TrackerParameter.BeamMomentum = 120.0;
    cout << "    BeamMomentum    parameter not specified. Setting it to defaul value: " << TrackerParameter.BeamMomentum << " GeV/c" << endl;
  }
  else {
    cout << "    BeamMomentum    parameter set to user value: " << TrackerParameter.BeamMomentum << " GeV/c" << endl;
  }

  if(TrackerParameter.MediumMaterial == TString("")) {
    TrackerParameter.MediumMaterial = TString("Vacuum");
    cout << "    MediumMaterial  parameter not specified. Setting it to defaul value:  " << TrackerParameter.MediumMaterial.Data() << endl;
  }
  else {
    cout << "    MediumMaterial  parameter set to user value: " << TrackerParameter.MediumMaterial.Data() << endl;
  }

  cout << "    -> Reading finished ! " << endl ;

  // *******************************************************
  // Test of MANDATORY fields settings // VR 20014/06/30
  // *******************************************************
  if (TrackerParameter.HitsInPlaneMaximum == -1)
  {
    cout << " ERROR: The field 'HitsInPlaneMaximum' is MANDATORY in config file (section tracker parameters)" << endl;
    gApplication->Terminate();
  }

  if(TrackerParameter.TracksMaximum > 0)
  {
    if (TrackerParameter.TracksFinder == 0)
    {
      if (TrackerParameter.SearchHitDistance == -1)
      {
	cout << " ERROR: The field 'SearchHitDistance' is MANDATORY in config file (section tracker parameters)" << endl;
	gApplication->Terminate();
      }
      if (TrackerParameter.PlanesForTrackMinimum == -1)
      {
        cout << " ERROR: The field 'PlanesForTrackMinimum' is MANDATORY in config file (section tracker parameters)" << endl;
        gApplication->Terminate();
      }
      if (TrackerParameter.HitsInPlaneTrackMaximum == -1)
      {
        cout << " ERROR: The field 'HitsInPlaneTrackMaximum' is MANDATORY in config file (section tracker parameters)" << endl;
        gApplication->Terminate();
      }
    }
    else if (TrackerParameter.TracksFinder == 1)
    {
      if (TrackerParameter.PlanesForTrackMinimum == -1)
      {
        cout << " ERROR: The field 'PlanesForTrackMinimum' is MANDATORY in config file (section tracker parameters)" << endl;
        gApplication->Terminate();
      }
      if (TrackerParameter.HitsInPlaneTrackMaximum == -1)
      {
        cout << " ERROR: The field 'HitsInPlaneTrackMaximum' is MANDATORY in config file (section tracker parameters)" << endl;
        gApplication->Terminate();
      }
      if (TrackerParameter.SearchHitDistance == -1)
      {
	cout << " ERROR: The field 'SearchHitDistance' is MANDATORY in config file (section tracker parameters)" << endl;
	gApplication->Terminate();
      }
      if (TrackerParameter.SearchMoreHitDistance == -1)
      {
        cout << " ERROR: The field 'SearchMoreHitDistance' is MANDATORY in config file (section tracker parameters) when using TracksFinder: 1" << endl;
        gApplication->Terminate();
      }
    }
    else if (TrackerParameter.TracksFinder == 2)
    {
      if (TrackerParameter.TrackingPass == -1)
      {
        cout << " ERROR: The field 'TrackingPass' is MANDATORY in config file (section tracker parameters) when using TracksFinder: 2" << endl;
        gApplication->Terminate();
      }
      if (TrackerParameter.TrackingOrderLines == -1)
      {
        cout << " ERROR: The field 'TrackingOrderLines' is MANDATORY in config file (section tracker parameters) when using TracksFinder: 2" << endl;
        gApplication->Terminate();
      }
    }
  }

  if(DSetupDebug) {
    cout << " #planes = " << TrackerParameter.Planes << endl;
    if ( TrackerParameter.Ladders>0 ) {
      cout << " #ladders = " << TrackerParameter.Ladders << endl;
    }
  }


}
//______________________________________________________________________________
//
void DSetup::ReadExperimentGeometryParameters()
{

  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  //  Experiment Geometrical Parameters
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  //
  // VR 2015/01/16

  cout << endl << " - Reading Experiment Geometrical Parameters " << endl;

  // ################################################
  //            Set default values
  // ################################################
  Float_t initValue[3] = {-666.0, -666.0, -666.0};

  sprintf(IviGeometryParameter.GeometryName,"not defined");
  sprintf(IviGeometryParameter.GeometryVersion,"-1");
  IviGeometryParameter.BeamOrigin.SetValue(initValue);
  IviGeometryParameter.BeamSlope.SetValue(initValue);
  IviGeometryParameter.BeamDisplayStrongBegin.SetValue(initValue);
  IviGeometryParameter.BeamDisplayStrongStop.SetValue(initValue);
  IviGeometryParameter.BeamDisplayMediumBegin.SetValue(initValue);
  IviGeometryParameter.BeamDisplayMediumStop.SetValue(initValue);
  IviGeometryParameter.BeamDisplayLightBegin.SetValue(initValue);
  IviGeometryParameter.BeamDisplayLightStop.SetValue(initValue);
  sprintf(IviGeometryParameter.TargetType,"not defined");
  IviGeometryParameter.TargetSize.SetValue(initValue);
  IviGeometryParameter.TargetRadius = -1.;
  IviGeometryParameter.TargetLength = -1.;
  sprintf(IviGeometryParameter.TargetAxis,"O");
  IviGeometryParameter.TargetCenter.SetValue(initValue);
  IviGeometryParameter.TrackerOrigin.SetValue(initValue);
  IviGeometryParameter.TrackerTilt.SetValue(initValue);
  sprintf(IviGeometryParameter.VertexingMethod,"not defined");


  if ( !strcmp(fFieldName,"Inputs") ||  !strcmp(fFieldName, "LadderID") )
  {
    cout << "    -> No Experiment Geometrical Parameters to read (next item is '" << fFieldName << "')" << endl ;
    return ;
  }

  // ----------------------------------
  //     IVI
  // ----------------------------------
  cout << "    -> IVI geometry : " << endl ;
  do
  {
    if ( ! strcmp( fFieldName, "GeometryName") )
    {
      read_strings(IviGeometryParameter.GeometryName, IviGeometryParameter.tpsz);
      cout << "      -> GeometryName:          " << IviGeometryParameter.GeometryName << endl ;
    }
    else if ( ! strcmp( fFieldName, "GeometryVersion") )
    {
      read_strings(IviGeometryParameter.GeometryVersion, IviGeometryParameter.tpsz);
      cout << "      -> GeometryVersion:       " << IviGeometryParameter.GeometryVersion << endl ;
    }
    else if ( ! strcmp( fFieldName, "BeamOriginI") )
    {
      read_r3(IviGeometryParameter.BeamOrigin);
      cout << "      -> BeamOrigin [mm]:    I: " << IviGeometryParameter.BeamOrigin(0) << endl ;
      cout << "                             J: " << IviGeometryParameter.BeamOrigin(1) << endl ;
      cout << "                             K: " << IviGeometryParameter.BeamOrigin(2) << endl ;
    }
    else if ( ! strcmp( fFieldName, "BeamSlopeI") )
    {
      read_r3(IviGeometryParameter.BeamSlope);
      cout << "      -> BeamSlope [/K]  :   I: " << IviGeometryParameter.BeamSlope(0) << endl ;
      cout << "                             J: " << IviGeometryParameter.BeamSlope(1) << endl ;
      cout << "                             K: " << IviGeometryParameter.BeamSlope(2) << endl ;
    }
    else if ( ! strcmp( fFieldName, "BeamDisplayStrongBeginI") )
    {
      read_r3(IviGeometryParameter.BeamDisplayStrongBegin);
      cout << "      -> BeamDisplayStrongBegin [mm]: I: " << IviGeometryParameter.BeamDisplayStrongBegin(0) << endl ;
      cout << "                                      J: " << IviGeometryParameter.BeamDisplayStrongBegin(1) << endl ;
      cout << "                                      K: " << IviGeometryParameter.BeamDisplayStrongBegin(2) << endl ;
    }
    else if ( ! strcmp( fFieldName, "BeamDisplayStrongStopI") )
    {
      read_r3(IviGeometryParameter.BeamDisplayStrongStop);
      cout << "      -> BeamDisplayStrongStop [mm]:  I: " << IviGeometryParameter.BeamDisplayStrongStop(0) << endl ;
      cout << "                                      J: " << IviGeometryParameter.BeamDisplayStrongStop(1) << endl ;
      cout << "                                      K: " << IviGeometryParameter.BeamDisplayStrongStop(2) << endl ;
    }
    else if ( ! strcmp( fFieldName, "BeamDisplayMediumBeginI") )
    {
      read_r3(IviGeometryParameter.BeamDisplayMediumBegin);
      cout << "      -> BeamDisplayMediumBegin [mm]: I: " << IviGeometryParameter.BeamDisplayMediumBegin(0) << endl ;
      cout << "                                      J: " << IviGeometryParameter.BeamDisplayMediumBegin(1) << endl ;
      cout << "                                      K: " << IviGeometryParameter.BeamDisplayMediumBegin(2) << endl ;
    }
    else if ( ! strcmp( fFieldName, "BeamDisplayMediumStopI") )
    {
      read_r3(IviGeometryParameter.BeamDisplayMediumStop);
      cout << "      -> BeamDisplayMediumStop [mm]:  I: " << IviGeometryParameter.BeamDisplayMediumStop(0) << endl ;
      cout << "                                      J: " << IviGeometryParameter.BeamDisplayMediumStop(1) << endl ;
      cout << "                                      K: " << IviGeometryParameter.BeamDisplayMediumStop(2) << endl ;
    }
    else if ( ! strcmp( fFieldName, "BeamDisplayLightBeginI") )
    {
      read_r3(IviGeometryParameter.BeamDisplayLightBegin);
      cout << "      -> BeamDisplayLightBegin [mm]:  I: " << IviGeometryParameter.BeamDisplayLightBegin(0) << endl ;
      cout << "                                      J: " << IviGeometryParameter.BeamDisplayLightBegin(1) << endl ;
      cout << "                                      K: " << IviGeometryParameter.BeamDisplayLightBegin(2) << endl ;
    }
    else if ( ! strcmp( fFieldName, "BeamDisplayLightStopI") )
    {
      read_r3(IviGeometryParameter.BeamDisplayLightStop);
      cout << "      -> BeamDisplayLightStop [mm]:   I: " << IviGeometryParameter.BeamDisplayLightStop(0) << endl ;
      cout << "                                      J: " << IviGeometryParameter.BeamDisplayLightStop(1) << endl ;
      cout << "                                      K: " << IviGeometryParameter.BeamDisplayLightStop(2) << endl ;
    }
    else if ( ! strcmp( fFieldName, "TargetType") )
    {
      read_strings(IviGeometryParameter.TargetType, IviGeometryParameter.tpsz);
      cout << "      -> TargetType:            " << IviGeometryParameter.TargetType << endl ;
    }
    else if ( ! strcmp( fFieldName, "TargetSizeI") )
    {
      read_r3(IviGeometryParameter.TargetSize);
      cout << "      -> TargetSize [mm]  :  I: " << IviGeometryParameter.TargetSize(0) << endl ;
      cout << "                             J: " << IviGeometryParameter.TargetSize(1) << endl ;
      cout << "                             K: " << IviGeometryParameter.TargetSize(2) << endl ;
    }
    else if ( ! strcmp( fFieldName, "TargetRadius") )
    {
      read_item(IviGeometryParameter.TargetRadius);
      cout << "      -> TargetRadius [mm] :    " << IviGeometryParameter.TargetRadius << endl ;
    }
    else if ( ! strcmp( fFieldName, "TargetLength") )
    {
      read_item(IviGeometryParameter.TargetLength);
      cout << "      -> TargetLength [mm] :    " << IviGeometryParameter.TargetLength << endl ;
    }
    else if ( ! strcmp( fFieldName, "TargetAxis") )
    {
      read_strings(IviGeometryParameter.TargetAxis,1);
      cout << "      -> TargetAxis :           " << IviGeometryParameter.TargetAxis << endl ;
    }
    else if ( ! strcmp( fFieldName, "TargetCenterI") )
    {
      read_r3(IviGeometryParameter.TargetCenter);
      cout << "      -> TargetCenter [mm]:  I: " << IviGeometryParameter.TargetCenter(0) << endl ;
      cout << "                             J: " << IviGeometryParameter.TargetCenter(1) << endl ;
      cout << "                             K: " << IviGeometryParameter.TargetCenter(2) << endl ;
    }
    else if ( ! strcmp( fFieldName, "TrackerOriginI") )
    {
      read_r3(IviGeometryParameter.TrackerOrigin);
      cout << "      -> TrackerOrigin [mm]: I: " << IviGeometryParameter.TrackerOrigin(0) << endl ;
      cout << "                             J: " << IviGeometryParameter.TrackerOrigin(1) << endl ;
      cout << "                             K: " << IviGeometryParameter.TrackerOrigin(2) << endl ;
    }
    else if ( ! strcmp( fFieldName, "TrackerTiltI") )
    {
      read_r3(IviGeometryParameter.TrackerTilt);
      cout << "      -> TrackerTilt [deg]:  I: " << IviGeometryParameter.TrackerTilt(0) << endl ;
      cout << "                             J: " << IviGeometryParameter.TrackerTilt(1) << endl ;
      cout << "                             K: " << IviGeometryParameter.TrackerTilt(2) << endl ;
    }
    else if ( ! strcmp( fFieldName, "VertexingMethod") )
    {
      read_strings(IviGeometryParameter.VertexingMethod, IviGeometryParameter.tpsz);
      cout << "      -> VertexingMethod:       " << IviGeometryParameter.VertexingMethod << endl ;
    }

    nextField();
  } while ( strcmp(fFieldName,"Inputs") &&  strcmp(fFieldName,"LadderID") );

  cout << "    -> Reading finished ! (next item is '" << fFieldName << "')" << endl ;

}

//______________________________________________________________________________
//
void DSetup::ReadLadderParameters( Int_t aLadderNumber)
{

  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // Parameters of a Ladder
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  //
  // JB 2013/01/16

  // ################################################
  //            Set default values
  // ################################################
  pLadderParameter[aLadderNumber].LadderID   = -1;
  pLadderParameter[aLadderNumber].Status     = -1;
  sprintf(pLadderParameter[aLadderNumber].Name,"");
  pLadderParameter[aLadderNumber].Planes     = 0;
  pLadderParameter[aLadderNumber].Position   = DR3(0.0,0.0,0.0);
  pLadderParameter[aLadderNumber].Tilt       = DR3(0.0,0.0,0.0);
  pLadderParameter[aLadderNumber].PlaneList  = NULL;
  pLadderParameter[aLadderNumber].PlaneShift = NULL;
  pLadderParameter[aLadderNumber].PlaneTilt  = NULL;

  if ( aLadderNumber<0 || TrackerParameter.Ladders<=aLadderNumber ) {
    printf( "WARNING in ReadLadderParameters: trying to add ladder number %d, outside the limits [1 - %d]\n  --> nothing done!\n", aLadderNumber+1, TrackerParameter.Ladders);
    return;
  }
  else {
    cout << endl << " - Reading Parameters of the Ladder " << aLadderNumber+1 << endl;
  }

  // Init
  Int_t nbOfPlanesAssociated = 0;

  do {

    if ( ! strcmp( fFieldName, "LadderID" ) ) {
      read_item(pLadderParameter[aLadderNumber].LadderID);
      if (DSetupDebug) cout << " ID = " << pLadderParameter[aLadderNumber].LadderID << endl;
    }
    else if( ! strcmp( fFieldName, "LadderName" ) ) {
      read_strings( pLadderParameter[aLadderNumber].Name, pLadderParameter[aLadderNumber].tpsz);
      if (DSetupDebug) cout << " Name = " << pLadderParameter[aLadderNumber].Name << endl;
    }
    else if( ! strcmp( fFieldName, "Status" ) ) {
      read_item( pLadderParameter[aLadderNumber].Status);
      /*if (DSetupDebug)*/ cout << " Status = " << pLadderParameter[aLadderNumber].Status << endl;
    }
    else if( ! strcmp( fFieldName, "LadderPositionX" ) ) {
      read_r3(pLadderParameter[aLadderNumber].Position);
      pLadderParameter[aLadderNumber].Position *= 1000.; // move from millimeters to microns
      if(DSetupDebug) cout << "  Position X = " << pLadderParameter[aLadderNumber].Position(0) << ",  Y = " << pLadderParameter[aLadderNumber].Position(1) << ", Z = " << pLadderParameter[aLadderNumber].Position(2) << " microns" << endl;
    }
    else if( ! strcmp( fFieldName, "LadderTiltZ" ) ) {
      read_r3(pLadderParameter[aLadderNumber].Tilt);
      pLadderParameter[aLadderNumber].Tilt *= TMath::Pi()/180.; // conversion degree to radian
      if(DSetupDebug) cout << "  Tilt X = " << pLadderParameter[aLadderNumber].Tilt(0) << ",  Y = " << pLadderParameter[aLadderNumber].Tilt(1) << ", Z = " << pLadderParameter[aLadderNumber].Tilt(2) << " rad" << endl;
    }
    else if ( ! strcmp( fFieldName, "NbOfPlanes" ) ) {
      read_item(pLadderParameter[aLadderNumber].Planes);
      pLadderParameter[aLadderNumber].PlaneList = new Int_t[pLadderParameter[aLadderNumber].Planes];
      pLadderParameter[aLadderNumber].PlaneShift = new DR3[pLadderParameter[aLadderNumber].Planes];
      pLadderParameter[aLadderNumber].PlaneTilt = new DR3[pLadderParameter[aLadderNumber].Planes];
      if (DSetupDebug) cout << " Nb of planes = " << pLadderParameter[aLadderNumber].Planes << endl;
    }
    else if( ! strcmp( fFieldName, "IncludedPlane" ) ) {
      if ( nbOfPlanesAssociated >= pLadderParameter[aLadderNumber].Planes ) {
        printf( "WARNING in ReadLadderParameters: trying to associate plane while maximum # planes (%d) already reached --> nothing done!\n", pLadderParameter[aLadderNumber].Planes);
      }
      else { // if planes can still be associated
        read_item(pLadderParameter[aLadderNumber].PlaneList[nbOfPlanesAssociated]);
        if(DSetupDebug) cout << "    Associated plane " << nbOfPlanesAssociated << " is " << pLadderParameter[aLadderNumber].PlaneList[nbOfPlanesAssociated] << endl;

        // Force the reading of the two next info
        nextField();
        if ( ! strcmp( fFieldName, "PlaneShiftU" ) ) {
          read_r3(pLadderParameter[aLadderNumber].PlaneShift[nbOfPlanesAssociated]);
          pLadderParameter[aLadderNumber].PlaneShift[nbOfPlanesAssociated] *= 1000.; // conversion from mm to um
          if(DSetupDebug) cout << "    Plane shift U = " << pLadderParameter[aLadderNumber].PlaneShift[nbOfPlanesAssociated](0) << ",  V = " << pLadderParameter[aLadderNumber].PlaneShift[nbOfPlanesAssociated](1) << ", W = " << pLadderParameter[aLadderNumber].PlaneShift[nbOfPlanesAssociated](2) << " microns" << endl;
        }
        else {
          printf( "WARNING in ReadLadderParameters: expecting Plane %d Shift information!\n", nbOfPlanesAssociated);
        }

        nextField();
        if ( ! strcmp( fFieldName, "PlaneTiltU" ) ) {
          read_r3(pLadderParameter[aLadderNumber].PlaneTilt[nbOfPlanesAssociated]);
          pLadderParameter[aLadderNumber].PlaneTilt[nbOfPlanesAssociated] *= M_PI/180.; // conversion degree to radian
          if(DSetupDebug) cout << "    Plane Tilt U = " << pLadderParameter[aLadderNumber].PlaneTilt[nbOfPlanesAssociated](0) << ",  V = " << pLadderParameter[aLadderNumber].PlaneTilt[nbOfPlanesAssociated](1) << ", W = " << pLadderParameter[aLadderNumber].PlaneTilt[nbOfPlanesAssociated](2) << " [rad]" << endl;
        }
        else {
          printf( "WARNING in ReadLadderParameters: expecting Plane %d Tilt information!\n", nbOfPlanesAssociated);
        }

        nbOfPlanesAssociated++;
      }  // end if planes can still be associated
    } // end if field is IncludedPlane
    else
    {
      if (  strcmp( fFieldName, "Inputs") &&  strcmp( fFieldName, "LadderID") &&  strcmp( fFieldName, "FileHeaderSize") )
      {
        cout << "WARNING : parameter '" << fFieldName << "' in config file is not understood !" << endl;
        getRidOfLine();
      }
    }

    nextField();

  } while (  strcmp( fFieldName, "Inputs") && strcmp( fFieldName, "LadderID") && strcmp( fFieldName, "FileHeaderSize") &&  strcmp( fFieldName, "FixTrackParamX") );

  if( nbOfPlanesAssociated < pLadderParameter[aLadderNumber].Planes ) {
    printf( "WARNING in ReadLadderParameters: only %d planes have been associated to ladder %d while %d were expected!\n", nbOfPlanesAssociated, pLadderParameter[aLadderNumber].LadderID, pLadderParameter[aLadderNumber].Planes);
  }

  fAddedLadders++;

  /*
  if(DSetupDebug) {
    printf("Parameters for ladder %d of type %s:\n", pLadderParameter[aLadderNumber].LadderID, pLadderParameter[aLadderNumber].Name);
    printf("  status = %d\n", pLadderParameter[aLadderNumber].Status);
    printf("  contains %d ladders:\n", pLadderParameter[aLadderNumber].Planes);
    for ( Int_t iplane=0; iplane<pLadderParameter[aLadderNumber].Planes; iplane++) {
      printf("    plane %d -> absolute nb = %d\n", iplane, pLadderParameter[aLadderNumber].PlaneList[iplane]);
      printf ("   relative tilt : %.2f %.2f %.2f \n"
              , pLadderParameter[aLadderNumber].PlaneTilt[iplane](0)
              , pLadderParameter[aLadderNumber].PlaneTilt[iplane](1)
              , pLadderParameter[aLadderNumber].PlaneTilt[iplane](2));
      printf ("   relative shift : %.2f %.2f %.2f \n"
              , pLadderParameter[aLadderNumber].PlaneShift[iplane](0)
              , pLadderParameter[aLadderNumber].PlaneShift[iplane](1)
              , pLadderParameter[aLadderNumber].PlaneShift[iplane](2));
    }
  }
   */

  if ( ! strcmp( fFieldName, "LadderID") ) { // new ladder
    ReadLadderParameters( fAddedLadders );
  } //end if new plane
  else if ( ! strcmp( fFieldName, "Inputs") ) { // new plane
    ReadPlaneParameters( fAddedPlanes );
  } //end if new plane

}
//______________________________________________________________________________
//
void DSetup::ReadGlobalAlignmentParameters()
{

  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // Global Alignment Parameters
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  //
  // Added : LC 31/08/2015

  cout << endl << " - Reading Global Alignment Parameters " << endl;

  pGlobalAlignmentParameter.FixTrackParamX = 0.;
  pGlobalAlignmentParameter.FixTrackParamY = 0.;
  pGlobalAlignmentParameter.ResoTrackParamX = 1e-3;
  pGlobalAlignmentParameter.ResoTrackParamY = 1e-3;
  pGlobalAlignmentParameter.ResoAlignParamTr = 100.;
  pGlobalAlignmentParameter.ResoAlignParamRot = 0.1; // rad
  pGlobalAlignmentParameter.ResoAlignParamZ = 1e-6; // rad
  pGlobalAlignmentParameter.ResoFixPlaneTr = 1e-6;
  pGlobalAlignmentParameter.ResoFixPlaneRot = 1e-4;


  if ( strcmp(fFieldName,"FixTrackParamX") )
  {
    cout << "    -> No Global Alignment Parameters to read (next item is '" << fFieldName << "')" << endl ;
    return ;
  }

  do {

    if ( ! strcmp( fFieldName, "FixTrackParamX" ) ) {
      read_item(pGlobalAlignmentParameter.FixTrackParamX);
      /*if (DSetupDebug)*/ cout << " Track parametrers X fixed to value " << pGlobalAlignmentParameter.FixTrackParamX << endl;
    }
    else if( ! strcmp( fFieldName, "FixTrackParamY" ) ) {
      read_item(pGlobalAlignmentParameter.FixTrackParamY);
      /*if (DSetupDebug)*/ cout << " Track parametrers Y fixed to value " << pGlobalAlignmentParameter.FixTrackParamY << endl;
    }
    else if( ! strcmp( fFieldName, "ResoTrackParamX" ) ) {
      read_item(pGlobalAlignmentParameter.ResoTrackParamX);
      /*if (DSetupDebug)*/ cout << " Track parameters X resolution = " << pGlobalAlignmentParameter.ResoTrackParamX << endl;
    }
    else if ( ! strcmp( fFieldName, "ResoTrackParamY" ) ) {
      read_item(pGlobalAlignmentParameter.ResoTrackParamY);
      /*if (DSetupDebug)*/ cout << " Track parameters Y resolution = " << pGlobalAlignmentParameter.ResoTrackParamY << endl;
    }
    else if( ! strcmp( fFieldName, "ResoAlignParamTr" ) ) {
      read_item(pGlobalAlignmentParameter.ResoAlignParamTr);
      /*if (DSetupDebug)*/ cout << " Alignemnt transaltions resolution = " << pGlobalAlignmentParameter.ResoAlignParamTr << endl;
    }
    else if( ! strcmp( fFieldName, "ResoAlignParamRot" ) ) {
      read_item(pGlobalAlignmentParameter.ResoAlignParamRot);
      pGlobalAlignmentParameter.ResoAlignParamRot *= TMath::Pi()/180.;
      /*if (DSetupDebug)*/ cout << " Alignemnt rotations resolution = " << pGlobalAlignmentParameter.ResoAlignParamRot << endl;
    }
    else if( ! strcmp( fFieldName, "ResoFixPlaneTr" ) ) {
      read_item(pGlobalAlignmentParameter.ResoFixPlaneTr);
      /*if (DSetupDebug)*/ cout << " Fix Plane Rotations Resolution = " << pGlobalAlignmentParameter.ResoFixPlaneTr << endl;
    }
    else if( ! strcmp( fFieldName, "ResoFixPlaneRot" ) ) {
      read_item(pGlobalAlignmentParameter.ResoFixPlaneRot);
      /*if (DSetupDebug)*/ cout << " Fix Plane Rotations Resolution = " << pGlobalAlignmentParameter.ResoFixPlaneRot << endl;
      pGlobalAlignmentParameter.ResoFixPlaneRot *= TMath::Pi()/180.;
    }
    else if( ! strcmp( fFieldName, "ResoAlignParamZ" ) ) {
      read_item(pGlobalAlignmentParameter.ResoAlignParamZ);
      /*if (DSetupDebug)*/ cout << " Alignemnt Translation Z Resolution = " << pGlobalAlignmentParameter.ResoAlignParamZ << endl;
    }

    nextField();

  } while ( strcmp( fFieldName, "Inputs") && strcmp( fFieldName, "FileHeaderSize") && strcmp( fFieldName, "GeometryName" ) && strcmp( fFieldName, "LadderID" ) );

  cout << "    -> Reading finished ! (next item is '" << fFieldName << "')" << endl ;

}
//______________________________________________________________________________
//
void DSetup::ReadPlaneParameters( Int_t aPlaneNumber)
{

  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // Parameters of the Planes
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  //
  // JB 2013/01/16
  // Modified JB 2013/07/17 new parameters HitFinder, IfDigitize, ...
  // Modified JB 2013/08/13 set default valus for MimosaType, Max/MinNStrips
  // Modified JB 2013/08/15 new ChannelOffset, PlaneResolution parameters
  // Modified JB 2013/08/29 new Digitizer parameters
  // Modified JB 2014/01/07 default for InitialPedestal and InitialNoise
  // Modified JB 2014/04/21 new deformation parameters
  // Modified AP 2014/07/31 new variables for hot pixel list: file name and fake rate cut
  // Modified AP 2014/12 ?  new Resolution options (U,V or by histograms)

  if ( aPlaneNumber<0 || TrackerParameter.Planes<=aPlaneNumber ) {
    printf( "WARNING in ReadPlaneParameters: trying to add plane number %d, outside the limits [1 - %d]\n  --> nothing done!\n", aPlaneNumber+1, TrackerParameter.Planes);
    return;
  }
  else {
    cout << endl << " - Reading Parameters of the Plane " << aPlaneNumber+1 << endl;
  }

  // Initialize some default values for mandatory parameters
  pPlaneParameter[aPlaneNumber].Inputs = 0;
  for(int imd=0;imd<fMaxModules;imd++) {
    pPlaneParameter[aPlaneNumber].ModuleType[imd]    = -1;
    pPlaneParameter[aPlaneNumber].ModuleNumber[imd]  = -1;
    pPlaneParameter[aPlaneNumber].InputNumber[imd]   = -1;
    pPlaneParameter[aPlaneNumber].ChannelNumber[imd] = -1;
    pPlaneParameter[aPlaneNumber].ChannelOffset[imd] =  0;
    pPlaneParameter[aPlaneNumber].Channels[imd]      =  0;
  }
  pPlaneParameter[aPlaneNumber].TimeLimit  = -1;   //Added, JB 2015/05/26
  sprintf(pPlaneParameter[aPlaneNumber].Name,"");
  sprintf(pPlaneParameter[aPlaneNumber].Purpose,"");
  pPlaneParameter[aPlaneNumber].Readout            = 0;
  pPlaneParameter[aPlaneNumber].MimosaType = 0;
  pPlaneParameter[aPlaneNumber].AnalysisMode       = -1;
  pPlaneParameter[aPlaneNumber].HitFinder = 0;
  pPlaneParameter[aPlaneNumber].FixedGlobalAlign = 0; // Added LC 2015/08/20
  pPlaneParameter[aPlaneNumber].InitialPedestal = 0;
  pPlaneParameter[aPlaneNumber].InitialNoise = 0;
  pPlaneParameter[aPlaneNumber].CacheSize          = 0;
  pPlaneParameter[aPlaneNumber].DistanceZero       = 0.0;
  pPlaneParameter[aPlaneNumber].Position           = DR3(0.0,0.0,0.0);
  pPlaneParameter[aPlaneNumber].Tilt               = DR3(0.0,0.0,0.0);
  pPlaneParameter[aPlaneNumber].AlignmentU         = 0.0;
  pPlaneParameter[aPlaneNumber].AlignmentV         = 0.0;
  pPlaneParameter[aPlaneNumber].AlignmentTilt      = 0.0;
  pPlaneParameter[aPlaneNumber].Size               = DR3(0.0,0.0,0.0);
  pPlaneParameter[aPlaneNumber].Strips             = DR3(0.0,0.0,0.0);
  pPlaneParameter[aPlaneNumber].Pitch              = DR3(0.0,0.0,0.0);
  pPlaneParameter[aPlaneNumber].StripSize          = DR3(0.0,0.0,0.0);

  pPlaneParameter[aPlaneNumber].PlaneResolution  = TrackerParameter.Resolution;
  pPlaneParameter[aPlaneNumber].PlaneResolutionU = -1;
  pPlaneParameter[aPlaneNumber].PlaneResolutionV = -1;
  pPlaneParameter[aPlaneNumber].PlaneThickness = -1;            //Added, AP 2015/03/10
  pPlaneParameter[aPlaneNumber].PlaneMaterial  = TString("");   //Added, AP 2015/03/10
  pPlaneParameter[aPlaneNumber].UsingTrackerResolution = false;
  pPlaneParameter[aPlaneNumber].PlanePerformancesList.clear();

  pPlaneParameter[aPlaneNumber].Mapping            = -1;
  pPlaneParameter[aPlaneNumber].ThreshNeighbourSN  = 1.0e+6;
  pPlaneParameter[aPlaneNumber].ThreshSeedSN       = 1.0e+6;
  pPlaneParameter[aPlaneNumber].MaximalNoise  = 1.0e+6;
  pPlaneParameter[aPlaneNumber].MinimalNoise  = -1.0e+6;
  pPlaneParameter[aPlaneNumber].MaxNStrips = 100;
  pPlaneParameter[aPlaneNumber].MinNStrips = 1;
  pPlaneParameter[aPlaneNumber].ClusterLimit      = DR3(-1,-1,-1);
  pPlaneParameter[aPlaneNumber].ClusterLimitRadius = -1;
  pPlaneParameter[aPlaneNumber].CommonRegions = 1; // JB 2014/12/29
  pPlaneParameter[aPlaneNumber].Status             = -1;
  pPlaneParameter[aPlaneNumber].ParentLadderID     = -1;
  pPlaneParameter[aPlaneNumber].HitPositionAlgorithm = -1;

  pPlaneParameter[aPlaneNumber].EtaCoefficientsN     = 0;
  for(int i=0;i<pPlaneParameter[aPlaneNumber].tpsz;i++) pPlaneParameter[aPlaneNumber].EtaCoefficient[i] = 0.0;
  pPlaneParameter[aPlaneNumber].EtaLowLimit          = -1.0;
  pPlaneParameter[aPlaneNumber].EtaHighLimit         = -1.0;

  pPlaneParameter[aPlaneNumber].KappaCoefficientsN   =  0;
  for(int i=0;i<pPlaneParameter[aPlaneNumber].tpsz;i++) pPlaneParameter[aPlaneNumber].KappaCoefficient[i] = 0.0;
  pPlaneParameter[aPlaneNumber].KappaLowLimit        = -1.0;
  pPlaneParameter[aPlaneNumber].KappaHighLimit       = -1.0;

  pPlaneParameter[aPlaneNumber].GammaCoefficientsN   =  0;
  for(int i=0;i<pPlaneParameter[aPlaneNumber].tpsz;i++) pPlaneParameter[aPlaneNumber].GammaCoefficient[i] = 0.0;
  pPlaneParameter[aPlaneNumber].GammaLowLimit        = -1.0;
  pPlaneParameter[aPlaneNumber].GammaLowLimit        = -1.0;

  pPlaneParameter[aPlaneNumber].NoisyStripsN         =  0;
  for(int i=0;i<pPlaneParameter[aPlaneNumber].tpsz;i++) pPlaneParameter[aPlaneNumber].NoisyStripsIndex[i] = 0.0;

  pPlaneParameter[aPlaneNumber].IfDigitize           =  0;
  for(int i=0;i<fMaxDigitalThresholds;i++) pPlaneParameter[aPlaneNumber].DigitizeThresholds[i] = -1;

  pPlaneParameter[aPlaneNumber].IfDeformed           =  0;
  for(int i=0;i<7;i++) {
    pPlaneParameter[aPlaneNumber].CoeffLegendreU[i] = 0.0;
    pPlaneParameter[aPlaneNumber].CoeffLegendreV[i] = 0.0;
  }

  pPlaneParameter[aPlaneNumber].HotPixelList_lin.clear();
  pPlaneParameter[aPlaneNumber].HotPixelList_col.clear();
  pPlaneParameter[aPlaneNumber].HotPixelList_index.clear();

  // Initialize some default values for non mandatory parameters
  TString  HotPixelMapFile(""); // ROOT file name with fake rate map, Added, AP 2014/07/31
  Float_t  FractionToMask  = 0.0;  // Single pixel fake rate cut,        Added, AP 2014/07/31
  Float_t  FakeRateCut     = 1.0;  // Single pixel fake rate cut,        Added, AP 2015/12/01
  Region_t TheRegion;
  TheRegion.R_col[0] = -1;
  TheRegion.R_col[1] = -1;
  TheRegion.R_lin[0] = -1;
  TheRegion.R_lin[1] = -1;

  TString ResolutionFile("");

  _ResolutionList.clear();
  _ResolutionUList.clear();
  _ResolutionVList.clear();
  _RegionList.clear();
  _FractionToMaskList.clear();
  _FakeRateCutList.clear();
  _ResolutionFileList.clear();

  do {

    if ( ! strcmp( fFieldName, "Inputs" ) ) {
      read_item(pPlaneParameter[aPlaneNumber].Inputs);
      if (DSetupDebug) cout << "   " << pPlaneParameter[aPlaneNumber].Inputs << " inputs:" << endl;
      // Force the reading in the given order
      for (Int_t tl = 0; tl < pPlaneParameter[aPlaneNumber].Inputs; tl++){
        nextField();
        read_item(pPlaneParameter[aPlaneNumber].ModuleType[tl]);
        nextField();
        read_item(pPlaneParameter[aPlaneNumber].ModuleNumber[tl]);
        nextField();
        read_item(pPlaneParameter[aPlaneNumber].InputNumber[tl]);
        nextField();
        read_item(pPlaneParameter[aPlaneNumber].ChannelNumber[tl]);
        nextField();
        if( ! strcmp( fFieldName, "ChannelOffset" ) ) { // JB 2013/08/15
          read_item(pPlaneParameter[aPlaneNumber].ChannelOffset[tl]);
          nextField();
        }
        else {
          pPlaneParameter[aPlaneNumber].ChannelOffset[tl] = 1;
        }
        read_item(pPlaneParameter[aPlaneNumber].Channels[tl]);
        if (DSetupDebug) cout << "    input " << tl
			      << " of type " << pPlaneParameter[aPlaneNumber].ModuleType[tl]
			      << " number " << pPlaneParameter[aPlaneNumber].ModuleNumber[tl]
			      << " input " << pPlaneParameter[aPlaneNumber].InputNumber[tl]
			      << " channel " << pPlaneParameter[aPlaneNumber].ChannelNumber[tl]
			      << " channels " << pPlaneParameter[aPlaneNumber].Channels[tl]
			      << endl;
      }
    }
    else if( ! strcmp( fFieldName, "StripselUse" ) ) {
      // do nothing but need to exclude it because of later field "Strips"
//      Int_t ALL = 64;
//      ChannelAllUse = new Int_t[ALL];
//      for (Int_t a = 0; a < ALL; a++)    ChannelAllUse[a] = 0xFFFF;
      getRidOfLine();
    }
    else if( ! strcmp( fFieldName, "Name" ) ) {
      read_strings( pPlaneParameter[aPlaneNumber].Name, pPlaneParameter[aPlaneNumber].tpsz);
    }
    else if( ! strcmp( fFieldName, "Purpose" ) ) {
      read_strings( pPlaneParameter[aPlaneNumber].Purpose, pPlaneParameter[aPlaneNumber].tpsz);
    }
    else if( ! strcmp( fFieldName, "Readout" ) ) {
      read_item(pPlaneParameter[aPlaneNumber].Readout);
      // Set #events for initialization to 0 for zero-suppressed data
      // JB 2014/12/29
      if ( pPlaneParameter[aPlaneNumber].Readout>=100 ) {
        pPlaneParameter[aPlaneNumber].InitialPedestal = 0;
        pPlaneParameter[aPlaneNumber].InitialNoise = 0;
      }
    }
    else if( ! strcmp( fFieldName, "MimosaType" ) ) {
      read_item(pPlaneParameter[aPlaneNumber].MimosaType); //RDM210509
    }
    else if( ! strcmp( fFieldName, "AnalysisMode" ) ) {
      read_item(pPlaneParameter[aPlaneNumber].AnalysisMode);
    }
    else if( ! strcmp( fFieldName, "HitFinder" ) ) { // JB 2013/07/17
      read_item(pPlaneParameter[aPlaneNumber].HitFinder);
    }
    else if( ! strcmp( fFieldName, "FixedGlobalAlign" ) ) { // JB 2013/07/17
      read_item(pPlaneParameter[aPlaneNumber].FixedGlobalAlign);
    }
    else if( ! strcmp( fFieldName, "HotPixelMapFile" ) ) {  // AP 2014/07/31
      read_TStrings( HotPixelMapFile, 350);
    }
    else if( ! strcmp( fFieldName, "FractionToMask" ) ) {      // AP 2014/07/31
      read_item(FractionToMask);
      _FractionToMaskList.push_back(FractionToMask);
    }
    else if( ! strcmp( fFieldName, "FakeRateCut" ) ) {      // AP 2015/12/01
      read_item(FakeRateCut);
      _FakeRateCutList.push_back(FakeRateCut);
    }
    else if( ! strcmp( fFieldName, "InitialPedestal" ) ) {
      read_item(pPlaneParameter[aPlaneNumber].InitialPedestal);
    }
    else if( ! strcmp( fFieldName, "InitialNoise" ) ) {
      read_item(pPlaneParameter[aPlaneNumber].InitialNoise);
      pPlaneParameter[aPlaneNumber].InitialPedestal = pPlaneParameter[aPlaneNumber].InitialNoise; // added to initialize when only InitialNoise is specified, JB 2014/01/07
    }
    else if( ! strcmp( fFieldName, "CacheSize" ) ) {
      read_item(pPlaneParameter[aPlaneNumber].CacheSize);
      if(DSetupDebug) printf("  CacheSize : %d     InitialPedestal : %d      InitialNoise : %d\n",
                             pPlaneParameter[aPlaneNumber].CacheSize,
                             pPlaneParameter[aPlaneNumber].InitialPedestal,
                             pPlaneParameter[aPlaneNumber].InitialNoise);
    }
    else if( ! strcmp( fFieldName, "IfDigitize" )  || ! strcmp( fFieldName, "Ifdigitize" ) ) { // JB 2013/07/17
      read_item(pPlaneParameter[aPlaneNumber].IfDigitize);
      // Force the reading of the thresholds right after the IfDigitize field, if non-zero
      if( pPlaneParameter[aPlaneNumber].IfDigitize>0 ) { // if digitization required
        if(DSetupDebug) printf( "  Digitization emulation required over %d thresholds:\n", pPlaneParameter[aPlaneNumber].IfDigitize);
        nextField();
        if( ! strcmp( fFieldName, "DigitizeThresholds" )  || ! strcmp( fFieldName, "Digitizethresholds" ) ) { // check there are the thresholds
          for (Int_t k = 0; k < pPlaneParameter[aPlaneNumber].IfDigitize; k++) {
            if( k>0 ) nextItem(':'); // already positionned for 1st value
            read_item(pPlaneParameter[aPlaneNumber].DigitizeThresholds[k]);
            if(DSetupDebug) printf( "  thre[%d] = %d\n", k, pPlaneParameter[aPlaneNumber].DigitizeThresholds[k]);
          }
        }
        else { // if the thresholds are missing
          Error("DSetup", "In the configuration file, the field 'DigitizeThresholds' is expected right after the 'IfDigitize' one, with %d thresholds !", pPlaneParameter[aPlaneNumber].IfDigitize);
        }
      }  // end if digitization required
    }
    else if( ! strcmp( fFieldName, "PositionsX" ) ) {
      read_r3(pPlaneParameter[aPlaneNumber].Position);
      pPlaneParameter[aPlaneNumber].Position *= 1000; // move from millimeters to microns
      if(DSetupDebug) cout << "  Position X = " << pPlaneParameter[aPlaneNumber].Position(0) << ",  Y = " << pPlaneParameter[aPlaneNumber].Position(1) << ", Z = " << pPlaneParameter[aPlaneNumber].Position(2) << " microns" << endl;
    }
    else if( ! strcmp( fFieldName, "TiltZ" ) || ! strcmp( fFieldName, "Tilt1" )  ) {
      // test with Tilt1 added to comply with MAF files, JB 2013/08/19
      read_r3(pPlaneParameter[aPlaneNumber].Tilt);
      pPlaneParameter[aPlaneNumber].Tilt *= M_PI/180.; // conversion degree to radian
    }
    else if( ! strcmp( fFieldName, "AlignementU" ) ) {
      read_item(pPlaneParameter[aPlaneNumber].AlignmentU);
    }
    else if( ! strcmp( fFieldName, "AlignementV" ) ) {
      read_item(pPlaneParameter[aPlaneNumber].AlignmentV);
    }
    else if( ! strcmp( fFieldName, "AlignementTilt" ) ) {
      read_item(pPlaneParameter[aPlaneNumber].AlignmentTilt);
      pPlaneParameter[aPlaneNumber].AlignmentTilt *= TMath::DegToRad();
      if(DSetupDebug) cout << "  AlignementU " << pPlaneParameter[aPlaneNumber].AlignmentU << ",  AlignementV " << pPlaneParameter[aPlaneNumber].AlignmentV << ",  AlignementTilt [deg] " << pPlaneParameter[aPlaneNumber].AlignmentTilt << endl;
    }
    else if( ! strcmp( fFieldName, "SizeU" ) ) {
      read_r3(pPlaneParameter[aPlaneNumber].Size);
      pPlaneParameter[aPlaneNumber].Size *= 1000;
      if(DSetupDebug) printf ("  Size : %f %f %f \n",pPlaneParameter[aPlaneNumber].Size(0),pPlaneParameter[aPlaneNumber].Size(1),pPlaneParameter[aPlaneNumber].Size(2));
      printf( "WARNING: you specify a Size for your strips, which is now ignored.\n The sensitive area is now computed from the Pitch and the nb of strips.\n");
    }
    else if( ! strcmp( fFieldName, "StripsU" ) ) {
      read_r3(pPlaneParameter[aPlaneNumber].Strips);
      if(DSetupDebug) printf ("  #strips : %f %f %f \n",pPlaneParameter[aPlaneNumber].Strips(0),pPlaneParameter[aPlaneNumber].Strips(1),pPlaneParameter[aPlaneNumber].Strips(2));
    }
    else if( ! strcmp( fFieldName, "PitchU" ) ) {
      read_r3(pPlaneParameter[aPlaneNumber].Pitch);
      pPlaneParameter[aPlaneNumber].Pitch *= 1000;
      if(DSetupDebug) printf ("  strip pitch : %f %f %f \n",pPlaneParameter[aPlaneNumber].Pitch(0),pPlaneParameter[aPlaneNumber].Pitch(1),pPlaneParameter[aPlaneNumber].Pitch(2));
      // If Strip Size is not defined yet, set to Pitch, JB 2014/04/21
      if( pPlaneParameter[aPlaneNumber].StripSize(0)<1. ) {
        pPlaneParameter[aPlaneNumber].StripSize = pPlaneParameter[aPlaneNumber].Pitch;
      }
    }
    else if( ! strcmp( fFieldName, "StripSize" ) || ! strcmp( fFieldName, "StripSizeU" )) {
      read_r3(pPlaneParameter[aPlaneNumber].StripSize);
      pPlaneParameter[aPlaneNumber].StripSize *= 1000;
      if(DSetupDebug) printf ("  strip size : %f %f %f \n",pPlaneParameter[aPlaneNumber].StripSize(0),pPlaneParameter[aPlaneNumber].StripSize(1),pPlaneParameter[aPlaneNumber].StripSize(2));
    }
    else if( ! strcmp( fFieldName, "Mapping" ) ) {
      read_item(pPlaneParameter[aPlaneNumber].Mapping);
      if(DSetupDebug) cout << "  Mapping = " << pPlaneParameter[aPlaneNumber].Mapping << endl;
    }
    else if( ! strcmp( fFieldName, "ThreshNeighbourSN" ) || ! strcmp( fFieldName, "ThresNeighbourSN" ) ) { // additional condition (typo), JB 2013/08/19
      read_item(pPlaneParameter[aPlaneNumber].ThreshNeighbourSN);
    }
    else if( ! strcmp( fFieldName, "ThreshSeedSN" ) ) {
      read_item(pPlaneParameter[aPlaneNumber].ThreshSeedSN);
      if(DSetupDebug) cout << "  Threshold SNR cuts, neighbour = " << pPlaneParameter[aPlaneNumber].ThreshNeighbourSN << " seed = " << pPlaneParameter[aPlaneNumber].ThreshSeedSN << endl ;
    }
    else if( ! strcmp( fFieldName, "MaximalNoise" ) || ! strcmp( fFieldName, "maximalNoise" )  || ! strcmp( fFieldName, "Maximalnoise" )) {
      read_item(pPlaneParameter[aPlaneNumber].MaximalNoise);
      if(DSetupDebug) cout << "  Maximal noise cut on seed = " << pPlaneParameter[aPlaneNumber].MaximalNoise << endl ;
    }
    else if( ! strcmp( fFieldName, "MinimalNoise" ) || ! strcmp( fFieldName, "minimalNoise" )  || ! strcmp( fFieldName, "Minimalnoise" )) {
      read_item(pPlaneParameter[aPlaneNumber].MinimalNoise);
      if(DSetupDebug) cout << "  Minimal noise cut on seed = " << pPlaneParameter[aPlaneNumber].MinimalNoise << endl ;
    }
    else if( ! strcmp( fFieldName, "MaxNStrips" ) ) {
      read_item(pPlaneParameter[aPlaneNumber].MaxNStrips);
    }
    else if( ! strcmp( fFieldName, "MinNStrips" ) ) {
      read_item(pPlaneParameter[aPlaneNumber].MinNStrips);
    }
    else if( ! strcmp( fFieldName, "ClusterLimit" ) || ! strcmp( fFieldName, "ClusterLimitU" )) {
      read_r3(pPlaneParameter[aPlaneNumber].ClusterLimit);
      pPlaneParameter[aPlaneNumber].ClusterLimit *= 1000;
    }
    else if( ! strcmp( fFieldName, "ClusterLimitRadius" ) ) { // VR 2014/07/16
      read_item(pPlaneParameter[aPlaneNumber].ClusterLimitRadius);
      pPlaneParameter[aPlaneNumber].ClusterLimitRadius *= 1000;
    }
    else if( ! strcmp( fFieldName, "CommonRegions" ) ) {
      read_item(pPlaneParameter[aPlaneNumber].CommonRegions);
    }
    else if( ! strcmp( fFieldName, "Status" ) ) {
      read_item(pPlaneParameter[aPlaneNumber].Status);
      if(DSetupDebug) cout << "  Status of plane = " << pPlaneParameter[aPlaneNumber].Status << endl;
    }
    else if( ! strcmp( fFieldName, "ParentLadder" ) ) {
      read_item(pPlaneParameter[aPlaneNumber].ParentLadderID);
      if(DSetupDebug) cout << "  Parent ladder ID = " << pPlaneParameter[aPlaneNumber].ParentLadderID << endl;
    }
    else if( ! strcmp( fFieldName, "PositionAlgorithm" ) ) {
      read_item(pPlaneParameter[aPlaneNumber].HitPositionAlgorithm);
    }
    else if( ! strcmp( fFieldName, "PlaneResolution" ) ) { // JB 2013/08/15
      Float_t resolution;
      read_item(resolution);
      _ResolutionList.push_back(resolution);
      //read_item(pPlaneParameter[aPlaneNumber].PlaneResolution);
    }
    else if( ! strcmp( fFieldName, "PlaneResolutionU" ) ) { // AP 2014/11/20
      Float_t resolution;
      read_item(resolution);
      _ResolutionUList.push_back(resolution);
      //read_item(pPlaneParameter[aPlaneNumber].PlaneResolutionU);
    }
    else if( ! strcmp( fFieldName, "PlaneResolutionV" ) ) { // AP 2014/11/20
      Float_t resolution;
      read_item(resolution);
      _ResolutionVList.push_back(resolution);
      //read_item(pPlaneParameter[aPlaneNumber].PlaneResolutionV);
    }
    else if( ! strcmp( fFieldName, "PlaneThickness" ) ) { // AP 2015/03/10
      read_item(pPlaneParameter[aPlaneNumber].PlaneThickness);
    }
    else if( ! strcmp( fFieldName, "PlaneMaterial" ) ) {  // AP 2015/03/10
      read_TStrings( pPlaneParameter[aPlaneNumber].PlaneMaterial, 350);
    }
    else if( ! strcmp( fFieldName, "ResolutionFile" ) ) {  // AP 2014/07/31
      read_TStrings( ResolutionFile, 350);
      _ResolutionFileList.push_back(ResolutionFile);
    }
    else if( ! strcmp( fFieldName, "ResolutionRegion" ) ) { // AP 2014/11/20
      read_item(TheRegion.R_col[0]);
      read_item(TheRegion.R_col[1]);
      read_item(TheRegion.R_lin[0]);
      read_item(TheRegion.R_lin[1]);

      if(TheRegion.R_col[0] == -1 ||
         TheRegion.R_col[1] == -1 ||
         TheRegion.R_lin[0] == -1 ||
         TheRegion.R_lin[1] == -1) {
        cout << endl;
        cout << "When specifying a region need to give 4 parameters. col_min, col_max, line_min and line_max. Check your inputs. Exiting now!!!" << endl;
        cout << endl;
        assert(false);
      }

      _RegionList.push_back(TheRegion);

    }
    else if( ! strcmp( fFieldName, "Digitizer" ) || ! strcmp( fFieldName, "Digitize" ) ) { // Digitization parameters, JB 2013/08/29
      read_item(pPlaneParameter[aPlaneNumber].IfDigitize);
      if( pPlaneParameter[aPlaneNumber].IfDigitize>fMaxDigitalThresholds ) {
        Warning("ReadPlaneParameters","Nb of digits specified %d, exceeds limit %d --> set back to limit", pPlaneParameter[aPlaneNumber].IfDigitize, fMaxDigitalThresholds);
        pPlaneParameter[aPlaneNumber].IfDigitize = fMaxDigitalThresholds;
      }
      if(DSetupDebug) cout << "  Digitize over " << pPlaneParameter[aPlaneNumber].IfDigitize << " thresholds:" << endl;
      for ( Int_t ithre=0; ithre<pPlaneParameter[aPlaneNumber].IfDigitize; ithre++) {
        nextItem(':');
        read_item( pPlaneParameter[aPlaneNumber].DigitizeThresholds[ithre]);
        if(DSetupDebug) cout << " " << pPlaneParameter[aPlaneNumber].DigitizeThresholds[ithre] << endl;
      }
      if(DSetupDebug) cout << endl;
    } // end Digitization parameters

    else if( ! strcmp( fFieldName, "Deformed" ) || ! strcmp( fFieldName, "deformed" ) ) { // JB 2014/04/21
      read_item(pPlaneParameter[aPlaneNumber].IfDeformed);
      if(DSetupDebug && pPlaneParameter[aPlaneNumber].IfDeformed>0) cout << "  Deforming plane surface according to parameters:" << endl;
    }
    else if( ! strcmp( fFieldName, "coeffLegendreU" ) || ! strcmp( fFieldName, "coefflegendreU" ) ) { // JB 2014/04/21
      Int_t i=0;
      if(DSetupDebug) cout << "     along U:" ;
      while (i<7) {
        read_item( pPlaneParameter[aPlaneNumber].CoeffLegendreU[i]);
        if(DSetupDebug) cout << " " << pPlaneParameter[aPlaneNumber].CoeffLegendreU[i];
        nextItem(':');
        i++;
      }
      if(DSetupDebug) cout << endl;
    } // end U Deformation parameters
    else if( ! strcmp( fFieldName, "coeffLegendreV" ) || ! strcmp( fFieldName, "coefflegendreV" ) ) { // JB 2014/04/21
      Int_t i=0;
      if(DSetupDebug) cout << "     along V:" ;
      while (i<7) {
        read_item( pPlaneParameter[aPlaneNumber].CoeffLegendreV[i]);
        if(DSetupDebug) cout << " " << pPlaneParameter[aPlaneNumber].CoeffLegendreV[i] << endl;
        nextItem(':');
        i++;
      }
      if(DSetupDebug) cout << endl;
    } // end V Deformation parameters
    else if( ! strcmp( fFieldName, "PlaneTimeLimit" ) || ! strcmp( fFieldName, "planetimelimit" ) ) { // JB 2015/05/26
      read_item(pPlaneParameter[aPlaneNumber].TimeLimit);
    }
    else
    {
      if ( strcmp( fFieldName, "Inputs") &&  strcmp( fFieldName, "LadderID") &&  strcmp( fFieldName, "FileHeaderSize") && strcmp( fFieldName, "ModuleTypes" ) && strcmp( fFieldName, "AcqModuleTypes" ) )
      {
        cout << "WARNING : parameter '" << fFieldName << "' in config file is not understood !" << endl;
        getRidOfLine();
      }
    }

    nextField();

  } while ( strcmp( fFieldName, "Inputs") &&
            strcmp( fFieldName, "LadderID") &&
            strcmp( fFieldName, "FileHeaderSize") &&
            strcmp( fFieldName, "ModuleTypes" ) &&
            strcmp( fFieldName, "AcqModuleTypes" ) &&
            strcmp( fFieldName, "FixTrackParamX")
	  );


  // Set specific parameters for HitFinder=2
  // AP ?

  if (pPlaneParameter[aPlaneNumber].HitFinder == 2) {
    if(pPlaneParameter[aPlaneNumber].ClusterLimitRadius == -1)
    {
      cout << " ERROR: The field 'ClusterLimitRadius' is MANDATORY in config file when using HitFinder = 2 (section planes parameters)" << endl;
      gApplication->Terminate();
    }
  }
  if (pPlaneParameter[aPlaneNumber].HitFinder != 2) {
    if(pPlaneParameter[aPlaneNumber].ClusterLimit(0) == -1 || pPlaneParameter[aPlaneNumber].ClusterLimit(1) == -1)
    {
      cout << " ERROR: The field 'ClusterLimit' is MANDATORY in config file when using HitFinder != 2 (section planes parameters)" << endl;
      gApplication->Terminate();
    }
  }

  //Set of specific parameters for the hit spatial resolution
  //AP 12/01/2015
  InitializePerformancesParams(aPlaneNumber);

  //Set specific parameters for hot pixel map
  //AP 01/12/2015
  if(_FakeRateCutList.size() > 0 && _FractionToMaskList.size() == 0) {
    GetListOfHotPixelsToMask_FakeRateCut(aPlaneNumber,HotPixelMapFile);
  }
  else if(_FakeRateCutList.size() == 0 && _FractionToMaskList.size() > 0) {
    GetListOfHotPixelsToMask_FracToMask(aPlaneNumber,HotPixelMapFile);
  }
  else if(_FakeRateCutList.size() > 0 && _FractionToMaskList.size() > 0) {
    cout << endl;
    cout << "WARNING:: Both lists of FakeRateCut and FractionToMask have size > 0" << endl;
    cout << "          In order to define the list of hot pixels to mask just one of the lists can have size > 0." << endl;
    cout << "          Check your inputs. Doing nothing!!!" << endl;
    cout << endl;
  }
  PrintListOfHotPixelsToMask(aPlaneNumber);

  _ResolutionList.clear();
  _ResolutionUList.clear();
  _ResolutionVList.clear();
  _RegionList.clear();
  _FractionToMaskList.clear();
  _FakeRateCutList.clear();
  _ResolutionFileList.clear();

  if(pPlaneParameter[aPlaneNumber].PlaneThickness < 0) {
    pPlaneParameter[aPlaneNumber].PlaneThickness = 1.0e-10;
    cout << "No specification of PlaneThickness. Use default value of " << pPlaneParameter[aPlaneNumber].PlaneThickness << "um. (Insignificant multiple scattering)" << endl;
  }
  else cout << "PlaneThickness set to user value: " << pPlaneParameter[aPlaneNumber].PlaneThickness << "um." << endl;

  if(pPlaneParameter[aPlaneNumber].PlaneMaterial  == TString("")) {
    pPlaneParameter[aPlaneNumber].PlaneMaterial = TString("silicon");
    cout << "No specification of PlaneMaterial.  Use default value of " << pPlaneParameter[aPlaneNumber].PlaneMaterial.Data() << "." << endl;
  }
  else {
    cout << "PlaneMaterial  set to user value: " << pPlaneParameter[aPlaneNumber].PlaneMaterial.Data() << endl;
  }

  // ---------
  // This is the end of plane parameters

  fAddedPlanes++;

  if ( ! strcmp( fFieldName, "Inputs") ) { // new plane
    ReadPlaneParameters( fAddedPlanes );
  } //end if new plane
  else if ( ! strcmp( fFieldName, "LadderID") ) { // new ladder
    ReadLadderParameters( fAddedLadders );
  } //end if new plane

  /*
   // Old parameters not used anymore
   // JB 2013/01/15
  read_item(pPlaneParameter[aPlaneNumber].EtaCoefficientsN);
  nextItem(':');
  for (j = 0; j < pPlaneParameter[aPlaneNumber].EtaCoefficientsN; j++) {
    fConfigFileStream >> pPlaneParameter[aPlaneNumber].EtaCoefficient[j];
  }
  if (DSetupDebug) cout << endl;
  read_item(pPlaneParameter[aPlaneNumber].EtaLowLimit);
  read_item(pPlaneParameter[aPlaneNumber].EtaHighLimit);

  read_item(pPlaneParameter[aPlaneNumber].KappaCoefficientsN);
  nextItem(':');
  for (j = 0; j < pPlaneParameter[aPlaneNumber].KappaCoefficientsN; j++) {
    fConfigFileStream >> pPlaneParameter[aPlaneNumber].KappaCoefficient[j];
  }
  if (DSetupDebug)  cout << endl;
  read_item(pPlaneParameter[aPlaneNumber].KappaLowLimit);
  read_item(pPlaneParameter[aPlaneNumber].KappaHighLimit);

  read_item(pPlaneParameter[aPlaneNumber].GammaCoefficientsN);
  nextItem(':');
  for (j = 0; j < pPlaneParameter[aPlaneNumber].GammaCoefficientsN; j++) {
    fConfigFileStream >> pPlaneParameter[aPlaneNumber].GammaCoefficient[j];
  }
  if (DSetupDebug)  cout << endl;
  read_item(pPlaneParameter[aPlaneNumber].GammaLowLimit);
  read_item(pPlaneParameter[aPlaneNumber].GammaHighLimit);


  read_item(pPlaneParameter[aPlaneNumber].NoisyStripsN);
  nextItem(':');
  for (j = 0; j < pPlaneParameter[aPlaneNumber].NoisyStripsN; j++) {
    fConfigFileStream >> pPlaneParameter[aPlaneNumber].NoisyStripsIndex[j];
    if (DSetupDebug) cout << pPlaneParameter[aPlaneNumber].NoisyStripsIndex[j] << " ";
  }
  */

}
//______________________________________________________________________________
//
void DSetup::ReadDAQParameters()
{

  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // Parameter of the Data Acquisition
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  //
  // JB 2013/01/16
  // Modified JB 2018/02/11 TimeReference option added

  cout << endl << " - Reading  Parameter of the Data Acquisition " << endl;

  // ################################################
  //            Set default values
  // ################################################
  AcqParameter.FileHeaderSize    = -1;
  AcqParameter.EventBufferSize   = -1;
  AcqParameter.FileHeaderLine    = -1;
  AcqParameter.EventTrailerSize  =  4;
  AcqParameter.ModuleTypes       = -1;
  AcqParameter.BinaryCoding      = -1;
  AcqParameter.TriggerMode       =  0;
  AcqParameter.EventBuildingMode =  1; //SS 2011.11.14. Setting EventBuildingMode=1 by default. It can be changed during initialisation of the session or from the config file
  sprintf(AcqParameter.TimeRefFile, ""); // JB 2018/02/11
  AcqParameter.IfExternalTimeRef = 0;

  do {

    if( ! strcmp( fFieldName, "FileHeaderSize" ) ) {
      read_item(AcqParameter.FileHeaderSize);
    }
    else if( ! strcmp( fFieldName, "EventBufferSize" ) ) {
      read_item(AcqParameter.EventBufferSize);
    }
    else if( ! strcmp( fFieldName, "FileHeaderLine" ) || ! strcmp( fFieldName, "FileHeaderLine[d]" )) {
      read_item(AcqParameter.FileHeaderLine);
    }
    else if( ! strcmp( fFieldName, "EventTrailerSize" ) || ! strcmp( fFieldName, "eventtrailersize" )) {
      read_item(AcqParameter.EventTrailerSize);
    }
    else if( ! strcmp( fFieldName, "ModuleTypes" ) || ! strcmp( fFieldName, "AcqModuleTypes" )  ) {
      read_item(AcqParameter.ModuleTypes);
    }
    else if( ! strcmp( fFieldName, "BinaryCoding" ) ) {
      read_item(AcqParameter.BinaryCoding);
    }
    else if( ! strcmp( fFieldName, "TriggerMode" ) ) {
      read_item(AcqParameter.TriggerMode);
    }
    else if( ! strcmp( fFieldName, "EventBuildingMode" ) ) {
      read_item(AcqParameter.EventBuildingMode);
    }
    else if( ! strcmp( fFieldName, "TimeRefFile" ) || ! strcmp( fFieldName, "timereffile" ) ) {
      read_strings( AcqParameter.TimeRefFile, 100);
      AcqParameter.IfExternalTimeRef = 1;
    }
    else
    {
      if (  strcmp( fFieldName, "Name") )
      {
        cout << "WARNING : parameter '" << fFieldName << "' in config file is not understood !" << endl;
        getRidOfLine();
      }
    }
    nextField();

  } while (  strcmp( fFieldName, "Name") );


  if( DSetupDebug) {
    cout << "   header file size " << AcqParameter.FileHeaderSize << endl;
    cout << "   event buffer size " << AcqParameter.EventBufferSize << endl;
    cout << "   event header size " << AcqParameter.FileHeaderLine << endl;
    cout << "   event trailer size " << AcqParameter.EventTrailerSize << endl;
    cout << "   nb of mod types " << AcqParameter.ModuleTypes << endl;
    cout << "   endian coding " << AcqParameter.BinaryCoding << endl;
    cout << "   trigger mode " << AcqParameter.TriggerMode << endl;
    cout << "   event building mode " << AcqParameter.EventBuildingMode << endl;
    if( AcqParameter.IfExternalTimeRef ) cout << "   file for external time ref " << AcqParameter.TimeRefFile << endl;
  }

}
//______________________________________________________________________________
//
void DSetup::ReadDAQBoardParameters( Int_t aBoardNumber)
{

  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // Parameter of the Data Acquisition boards in this run
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  //
  // JB 2013/01/16
  // Modified: JB 2013/06/20 correction to call ReadDAQBoardParameters at the end
  // Modified: JB 2013/06/22 new EventBuildingBoardMode param
  // Modified: JB 2013/08/16 manage different parameters for each input
  // Modified: JB+CB+PLR 2015/03/24 additional parameter for INFN decoder
  // Modified: JB 2016/09/20 new nMultiFrames param
  // Modified: JB 2017/11/20 new IfZeroSuppress mode available

  sprintf(pAcqModuleParameter[aBoardNumber].Name,"");
  pAcqModuleParameter[aBoardNumber].Devices                   = -1;
  pAcqModuleParameter[aBoardNumber].Type                      = -1;
  pAcqModuleParameter[aBoardNumber].EventBuildingBoardMode    = -1;
  pAcqModuleParameter[aBoardNumber].Inputs                    = -1;
  for(int i=0;i<fMaxModules;i++) {
    pAcqModuleParameter[aBoardNumber].Channels[i]             = -1;
    pAcqModuleParameter[aBoardNumber].Bits[i]                 = -1;
    pAcqModuleParameter[aBoardNumber].SigBits[i]              = -1;
    pAcqModuleParameter[aBoardNumber].DeviceDataFile[i]       = NULL;
    pAcqModuleParameter[aBoardNumber].NbOfFramesPerChannel[i] = -1;
    pAcqModuleParameter[aBoardNumber].PixelShiftMod[i]        = -1;
  }
  pAcqModuleParameter[aBoardNumber].NColumns                  = 64;
  pAcqModuleParameter[aBoardNumber].NMultiFrames              =  5;
  pAcqModuleParameter[aBoardNumber].FirstTriggerChannel       = -1;
  pAcqModuleParameter[aBoardNumber].LastTriggerChannel        = -1;
  pAcqModuleParameter[aBoardNumber].PixelShift                = -1;
  pAcqModuleParameter[aBoardNumber].AmpOffset                 = -1;
  pAcqModuleParameter[aBoardNumber].AmpFactor                 = -1;
  pAcqModuleParameter[aBoardNumber].Trailer                   = 0;
  pAcqModuleParameter[aBoardNumber].MCTreeName                = TString("");
  pAcqModuleParameter[aBoardNumber].IfZeroSuppress            =  0;
  pAcqModuleParameter[aBoardNumber].ThresholdZero             = -1000000;


  // ################################################
  //            Set default values
  // ################################################

  if ( aBoardNumber<0 || AcqParameter.ModuleTypes<=aBoardNumber ) {
    printf( "WARNING in ReadDAQBoardParameters: trying to add a board type %d, outside the limits [1 - %d]\n  --> nothing done!\n", aBoardNumber+1, AcqParameter.ModuleTypes);
    return;
  }
  else {
    cout << endl << " - Reading Parameters of the Data Acquisition board type " << aBoardNumber+1 << endl;
  }
  // Some default values
  pAcqModuleParameter[aBoardNumber].EventBuildingBoardMode = AcqParameter.EventBuildingMode; // default, JB 2013/06/22
  pAcqModuleParameter[aBoardNumber].FirstTriggerChannel = 0;
  pAcqModuleParameter[aBoardNumber].LastTriggerChannel = 0;
  pAcqModuleParameter[aBoardNumber].NbOfFramesPerChannel[0] = 2;
  pAcqModuleParameter[aBoardNumber].PixelShift = 3; // JB,CB,PLR 2015/03/24
  pAcqModuleParameter[aBoardNumber].AmpOffset = 32768; // JB,CB,PLR 2015/03/24
  pAcqModuleParameter[aBoardNumber].AmpFactor = 1.; // JB,CB,PLR 2015/03/24
  pAcqModuleParameter[aBoardNumber].Trailer = 0xfafafafa; // JB,CB,PLR 2015/03/24
  // Taking into account Pixel Shift, which could be defined:
  // - globally with a single PixelShift
  // - for each module with as many PixleShiftMod as module
  for( Int_t iMod=0; iMod<pAcqModuleParameter[aBoardNumber].Devices; iMod++) {
    pAcqModuleParameter[aBoardNumber].PixelShiftMod[iMod] = -1; // indicate not define
  }
  Int_t inputCounter = -1; // starting value for inputs, updated at the next "channels" field

  do {

    if ( ! strcmp( fFieldName, "Name") ) {
      read_strings( pAcqModuleParameter[aBoardNumber].Name, pAcqModuleParameter[aBoardNumber].tpsz);
    }
    else if( ! strcmp( fFieldName, "Type" ) ) {
      read_item(pAcqModuleParameter[aBoardNumber].Type);
      pAcqModuleParameter[aBoardNumber].EventBuildingBoardMode = pAcqModuleParameter[aBoardNumber].Type%10; // JB 2013/06/22
    }
    else if( ! strcmp( fFieldName, "EventBuildingBoardMode" ) ) {
      read_item(pAcqModuleParameter[aBoardNumber].EventBuildingBoardMode); // JB 2013/06/22
    }
    else if( ! strcmp( fFieldName, "Devices" ) ) {
      read_item(pAcqModuleParameter[aBoardNumber].Devices);
    }
    else if( ! strcmp( fFieldName, "Inputs" ) ) {
      read_item(pAcqModuleParameter[aBoardNumber].Inputs);
    }
    else if( ! strcmp( fFieldName, "Channels" ) ) {
      if( inputCounter<=fMaxModules) {
        inputCounter++;
      }
      else {
        printf("WARNING in DSetup:, trying to add more than %d inputs, ignored\n", fMaxModules);
      }
      read_item(pAcqModuleParameter[aBoardNumber].Channels[inputCounter]);
    }
    else if( ! strcmp( fFieldName, "Bits" ) ) {
      read_item(pAcqModuleParameter[aBoardNumber].Bits[inputCounter]);
    }
    else if( ! strcmp( fFieldName, "SignificantBits" ) ) {
      read_item(pAcqModuleParameter[aBoardNumber].SigBits[inputCounter]);
    }
    else if( ! strcmp( fFieldName, "NColumns" ) ) {
      read_item(pAcqModuleParameter[aBoardNumber].NColumns);
    }
    else if( ! strcmp( fFieldName, "NMultiFrames" ) ) {
      read_item(pAcqModuleParameter[aBoardNumber].NMultiFrames);
    }
    else if( ! strcmp( fFieldName, "FirstTriggerChannel" ) ) {
      read_item(pAcqModuleParameter[aBoardNumber].FirstTriggerChannel);
    }
    else if( ! strcmp( fFieldName, "LastTriggerChannel" ) ) {
      read_item(pAcqModuleParameter[aBoardNumber].LastTriggerChannel);
    }
    else if( ! strcmp( fFieldName, "NbOfFramesPerChannel" ) ) {
      read_item(pAcqModuleParameter[aBoardNumber].NbOfFramesPerChannel[inputCounter]);
    }
    else if( ! strcmp( fFieldName, "PixelShift" ) && strcmp( fFieldName, "PixelShiftMod" ) ) {  // JB,CB,PLR 2015/03/24
      read_item(pAcqModuleParameter[aBoardNumber].PixelShift);
      // if local PixelShiftMod is not defined, update with global PixelShift
      for( Int_t iMod=0; iMod<pAcqModuleParameter[aBoardNumber].Devices; iMod++) {
        if( pAcqModuleParameter[aBoardNumber].PixelShiftMod[iMod] == -1 ) {
          pAcqModuleParameter[aBoardNumber].PixelShiftMod[iMod] = pAcqModuleParameter[aBoardNumber].PixelShift;
        }
      }

    }
    else if( ! strcmp( fFieldName, "AmpOffset" ) ) { // JB,CB,PLR 2015/03/24
      read_item(pAcqModuleParameter[aBoardNumber].AmpOffset);
    }
    else if( ! strcmp( fFieldName, "AmpFactor" ) ) { // JB,CB,PLR 2015/03/24
      read_item(pAcqModuleParameter[aBoardNumber].AmpFactor);
    }
    else if( ! strcmp( fFieldName, "Trailer" ) ) { // JB,CB,PLR 2015/03/24
      read_item(pAcqModuleParameter[aBoardNumber].Trailer);
    }
    else if( ! strcmp( fFieldName, "MCTreeName" ) ) {  // 2016/04/18, AP MC tree name. Only for MCBoardReader to read MC simulation
      read_TStrings( pAcqModuleParameter[aBoardNumber].MCTreeName, 350);
    }
    else if( ! strcmp( fFieldName, "DataFile" ) || ! strcmp( fFieldName, "DataFile1" )) {
      // reading data file name for each device of this type, JB 2009/05/25
      for( Int_t iMod=0; iMod<pAcqModuleParameter[aBoardNumber].Devices; iMod++) {
        pAcqModuleParameter[aBoardNumber].DeviceDataFile[iMod] = new Char_t[pAcqModuleParameter[aBoardNumber].tpsz];
        pAcqModuleParameter[aBoardNumber].PixelShiftMod[iMod] = 3; // default value
        if( iMod>0 ) nextField();
        if( strstr( fFieldName, "DataFile" ) ) {
          read_strings( pAcqModuleParameter[aBoardNumber].DeviceDataFile[iMod], pAcqModuleParameter[aBoardNumber].tpsz);
        }
        else {
          printf( "WARNING in ReadDAQBoardParameters: field %s found while 'DataFile%d' expected!\n    -> Board %d of type %d lacks rawdata connection.", fFieldName, iMod+1, iMod+1, aBoardNumber);
        }
      }
    }
    else if( ! strcmp( fFieldName, "PixelShiftMod" ) || ! strcmp( fFieldName, "PixelShiftMod1" ) ) {  // JB,CB,PLR 2015/03/24
      // reading kShift for each device of this type
      for( Int_t iMod=0; iMod<pAcqModuleParameter[aBoardNumber].Devices; iMod++) {
        pAcqModuleParameter[aBoardNumber].PixelShiftMod[iMod] = 3; // default value
        if( iMod>0 ) nextField();
        if( strstr( fFieldName, "PixelShiftMod" ) ) {
          read_item(pAcqModuleParameter[aBoardNumber].PixelShiftMod[iMod]);
        }
        else {
          printf( "WARNING in ReadDAQBoardParameters: field %s found while 'PixelShiftMod%d' expected!\n    -> Board %d of type %d lacks kShift.", fFieldName, iMod+1, iMod+1, aBoardNumber);
        }
      }
    }
    else if( ! strcmp( fFieldName, "IfZeroSuppress" ) || ! strcmp( fFieldName, "Ifzerosuppress" ) ) { // JB 2017/11/20
      read_item(pAcqModuleParameter[aBoardNumber].IfZeroSuppress);
      // Force the reading of the threshold right after the IfZeroSuppress field, if non-zero
      if( pAcqModuleParameter[aBoardNumber].IfZeroSuppress>0 ) { // if zero suppression required
        nextField();
        if( ! strcmp( fFieldName, "ThresholdZero") || ! strcmp( fFieldName, "Thresholdzero" ) ) { // check there is a threshold
          read_item(pAcqModuleParameter[aBoardNumber].ThresholdZero);
        }
        else { // if the threshold is missing
          Error("DSetup", "In the configuration file, the field 'ThresholdZero' is expected right after the 'IfZeroSuppress' one!");
        }
      }  // end if zero suppression required
    }
    else
    {
      if (  strcmp( fFieldName, "StatisticCells") &&  strcmp( fFieldName, "CmsNoiseCut") &&  strcmp( fFieldName, "MaxNbOfHits") &&  strcmp( fFieldName, "Name") &&  strcmp( fFieldName, "AnalysisGoal") )
      {
        cout << "WARNING : parameter '" << fFieldName << "' in config file is not understood !" << endl;
        getRidOfLine();
      }
    }
    nextField();

  } while (  strcmp( fFieldName, "StatisticCells") &&  strcmp( fFieldName, "CmsNoiseCut") &&  strcmp( fFieldName, "MaxNbOfHits") &&  strcmp( fFieldName, "Name") &&  strcmp( fFieldName, "AnalysisGoal") );

  // Copy the parameters of the last specified input to the inputs
  //  for which they were not specified.
  // This mechanism allows to declare N inputs while providing info just for one.
  // JB 2013/08/16
  for (Int_t aInp=inputCounter+1; aInp<pAcqModuleParameter[aBoardNumber].Inputs; aInp++) {
    printf(" updating input %d with information from input %d\n", aInp, inputCounter);
    pAcqModuleParameter[aBoardNumber].Channels[aInp]             = pAcqModuleParameter[aBoardNumber].Channels[inputCounter];
    pAcqModuleParameter[aBoardNumber].Bits[aInp]                 = pAcqModuleParameter[aBoardNumber].Bits[inputCounter];
    pAcqModuleParameter[aBoardNumber].SigBits[aInp]              = pAcqModuleParameter[aBoardNumber].SigBits[inputCounter];
    pAcqModuleParameter[aBoardNumber].NbOfFramesPerChannel[aInp] = pAcqModuleParameter[aBoardNumber].NbOfFramesPerChannel[inputCounter];
  }

  if( DSetupDebug) {
    cout << "  Using " << pAcqModuleParameter[aBoardNumber].Devices << " " << pAcqModuleParameter[aBoardNumber].Name << " with " << pAcqModuleParameter[aBoardNumber].Inputs << " inputs and " << pAcqModuleParameter[aBoardNumber].Bits[0] << " bits(signif. " << pAcqModuleParameter[aBoardNumber].SigBits[0] << ") and data files ";
    for( Int_t iMod=0; iMod<pAcqModuleParameter[aBoardNumber].Devices; iMod++) {
      if( pAcqModuleParameter[aBoardNumber].DeviceDataFile[iMod] != NULL ) cout << " (" << iMod+1 << ")" << pAcqModuleParameter[aBoardNumber].DeviceDataFile[iMod];
    }
    cout << ", and PixelShift ";
    for( Int_t iMod=0; iMod<pAcqModuleParameter[aBoardNumber].Devices; iMod++) {
      cout << " (" << iMod+1 << ")" << pAcqModuleParameter[aBoardNumber].PixelShiftMod[iMod];
    }
    cout << endl;
    if( pAcqModuleParameter[aBoardNumber].IfZeroSuppress>0 ) {
      cout << "   ==> Zero suppression required with threshold = " << pAcqModuleParameter[aBoardNumber].ThresholdZero << endl;
    }
  }

  if ( ! strcmp( fFieldName, "Name") ) { // new board
    ReadDAQBoardParameters( aBoardNumber+1 );
  } //end if new board

}
//______________________________________________________________________________
//
void DSetup::ReadAnalysisParameters()
{

  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // Parameter for Analysis
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  //
  // JB 2013/01/16
  // Modified: JB 2013/06/21 add MaxTracksExGeom and ExGeomatrix params
  // Modified: JB 2013/07/17 new UserFlag param
  // Modified: JB 2013/09/21 new histoRange param
  // Modified: JB 2014/01/17 new AnalysisGoal param

  cout << endl << " - Reading Parameter for analysis" << endl;

  // ################################################
  //            Set default values
  // ################################################
  // Default values in case those parameters are set in the config file
  AnalysisParameter.SavePlots           = false;
  AnalysisParameter.DoTelResolutionMC   = false;
  AnalysisParameter.MCEvents            = 10000;
  AnalysisParameter.MCSeed              = 18383;
  AnalysisParameter.MCDoDisplay         = false;
  AnalysisParameter.DoGaussianMS        = true;
  AnalysisParameter.ResolutionScanSteps = 10;
  AnalysisParameter.ResolutionScanInit  =  2.0; // microns
  AnalysisParameter.ResolutionScanEnd   =  8.0; // microns

  AnalysisParameter.CacheSize      = 0;
  AnalysisParameter.StatisticCells = 50;
  AnalysisParameter.CmsNoiseCut    = 3.;
  AnalysisParameter.MaxNbOfHits    = 1000;
  AnalysisParameter.MinNbOfHits    = 0;
  AnalysisParameter.TrackChi2Limit = 1000.0;
  AnalysisParameter.MinHitsPerTrack = TrackerParameter.PlanesForTrackMinimum;
  AnalysisParameter.MaxTracksExGeom = -1;
  AnalysisParameter.ExGeomatrix     = 0;

  AnalysisParameter.Submatrices      = 0;
  AnalysisParameter.HistoChargeRange = 5000.; // e-, JB 2013/09/12
  AnalysisParameter.HistoSNRRange    = 250;
  AnalysisParameter.HistoNoiseRange  = 40; // e-
  for(int i=0;i<fMaxSubmatrices;i++) {
    AnalysisParameter.PixelSizeU[i]            = 0.0;
    AnalysisParameter.PixelSizeV[i]            = 0.0;
    AnalysisParameter.PixelsInRaw[i]           = 0;
    AnalysisParameter.PixelsInColumn[i]        = 0;
    AnalysisParameter.Matrixtype[i]            = -1;
    AnalysisParameter.MinSeedIndex[i]          = -1;
    AnalysisParameter.MaxSeedIndex[i]          = -1;
    AnalysisParameter.MinSeedCol[i]            = -1;
    AnalysisParameter.MaxSeedCol[i]            = -1;
    AnalysisParameter.MinSeedRow[i]            = -1;
    AnalysisParameter.MaxSeedRow[i]            = -1;
    AnalysisParameter.MaxNofPixelsInCluster[i] = 1000;
    AnalysisParameter.MinNofPixelsInCluster[i] = 1;
    AnalysisParameter.MinSeedCharge[i]         = -1.0e+6;
    AnalysisParameter.MinClusterCharge[i]      = -1.0e+6;
    AnalysisParameter.MinNeighbourCharge[i]    = -1.0e+6;
    AnalysisParameter.NoiseScope[i]            = 0.0;
    AnalysisParameter.Calibration[i]           = 1.0;
    AnalysisParameter.Geomatrices[i]           = 0;

    for(int j=0;j<fMaxGeomatrices;j++) {
      AnalysisParameter.Umin[i][j] = -1.0e+20;
      AnalysisParameter.Umax[i][j] = +1.0e+20;
      AnalysisParameter.Vmin[i][j] = -1.0e+20;
      AnalysisParameter.Vmax[i][j] = +1.0e+20;
    }
  }

  AnalysisParameter.UserFlag = 0;

  sprintf( AnalysisParameter.AnalysisGoal, ""); // Jb 2014/01/17


  do {

    if ( ! strcmp( fFieldName, "StatisticCells") ) {
      read_item(AnalysisParameter.StatisticCells);
    }
    else if( ! strcmp( fFieldName, "CmsNoiseCut" ) ) {
      read_item(AnalysisParameter.CmsNoiseCut);
    }
    else if( ! strcmp( fFieldName, "MaxNbOfHits" ) ) {
      read_item(AnalysisParameter.MaxNbOfHits);
    }
    else if( ! strcmp( fFieldName, "MinNbOfHits" ) ) {
      read_item(AnalysisParameter.MinNbOfHits);
    }
    else if( ! strcmp( fFieldName, "TrackChi2Limit" ) ) {
      read_item(AnalysisParameter.TrackChi2Limit);
    }
    else if( ! strcmp( fFieldName, "SavePlots" ) ) {
      int temp;
      read_item(temp);
      if(temp == 1) AnalysisParameter.SavePlots = true;
      else          AnalysisParameter.SavePlots = false;
    }
    else if( ! strcmp( fFieldName, "DoTelResolutionMC" ) ) {
      int temp;
      read_item(temp);
      if(temp == 1) AnalysisParameter.DoTelResolutionMC = true;
      else          AnalysisParameter.DoTelResolutionMC = false;
      if(AnalysisParameter.DoTelResolutionMC) {
	cout << endl;
	cout << "Analysis Parameters:: Doing MC generation for evaluation of Telescope resolution at DUT!" << endl;
	cout << endl;
      }
    }
    else if( ! strcmp( fFieldName, "MCEvents" ) ) {
      read_item(AnalysisParameter.MCEvents);
    }
    else if( ! strcmp( fFieldName, "MCSeed" ) ) {
      read_item(AnalysisParameter.MCSeed);
    }
    else if( ! strcmp( fFieldName, "MCDoDisplay" ) ) {
      int temp;
      read_item(temp);
      if(temp == 1) AnalysisParameter.MCDoDisplay = true;
      else          AnalysisParameter.MCDoDisplay = false;
    }
    else if( ! strcmp( fFieldName, "DoGaussianMS" ) ) {
      int temp;
      read_item(temp);
      if(temp == 1) AnalysisParameter.DoGaussianMS = true;
      else          AnalysisParameter.DoGaussianMS = false;
    }
    else if( ! strcmp( fFieldName, "TrackChi2Limit" ) ) {
      read_item(AnalysisParameter.TrackChi2Limit);
    }
    else if( ! strcmp( fFieldName, "ResolutionScanSteps" ) ) {
      read_item(AnalysisParameter.ResolutionScanSteps);
    }
    else if( ! strcmp( fFieldName, "ResolutionScanInit" ) ) {
      read_item(AnalysisParameter.ResolutionScanInit);
    }
    else if( ! strcmp( fFieldName, "ResolutionScanEnd" ) ) {
      read_item(AnalysisParameter.ResolutionScanEnd);
    }
    else if( ! strcmp( fFieldName, "MinHitsPerTrack" ) ) {
      read_item(AnalysisParameter.MinHitsPerTrack);
    }
    else if( ! strcmp( fFieldName, "MaxTracksExGeom" ) ) {
      read_item(AnalysisParameter.MaxTracksExGeom);
    }
    else if( ! strcmp( fFieldName, "ExGeomatrix" ) ) {
      read_item(AnalysisParameter.ExGeomatrix);
    }
    else if( ! strcmp( fFieldName, "UserFlag" ) ) { // JB 2013/07/17
      read_item(AnalysisParameter.UserFlag);
    }
    else if( ! strcmp( fFieldName, "HistoChargeRange" ) ) { // JB 2013/09/12
      read_item(AnalysisParameter.HistoChargeRange);
    }
    else if( ! strcmp( fFieldName, "HistoSNRRange" ) ) { // JB 2013/09/12
      read_item(AnalysisParameter.HistoSNRRange);
    }
    else if( ! strcmp( fFieldName, "HistoNoiseRange" ) ) { // JB 2013/09/12
      read_item(AnalysisParameter.HistoNoiseRange);
    }
    else if( ! strcmp( fFieldName, "AnalysisGoal" ) ) { // JB 2014/01/17
      read_strings(AnalysisParameter.AnalysisGoal, 50);
    }
    else if( ! strcmp( fFieldName, "Submatrices" ) || ! strcmp( fFieldName, "Subamtrices" )) { // note the second test to take into account typo propagated years ago !
      read_item(AnalysisParameter.Submatrices);
      if ( AnalysisParameter.Submatrices>fMaxSubmatrices ) {
        printf( "WARNING: required number of submatrices %d exceeds maximum allowed %d!\n  -> restricted to %d.\n", AnalysisParameter.Submatrices, fMaxSubmatrices, fMaxSubmatrices);
        AnalysisParameter.Submatrices = fMaxSubmatrices;
      }
    }
    else
    {
      if (  strcmp( fFieldName, "PixelSizeU" ) && !fConfigFileStream.eof() )
      {
        cout << "WARNING : parameter '" << fFieldName << "' in config file is not understood !" << endl;
        getRidOfLine();
      }
    }
    nextField();

  } while (  strcmp( fFieldName, "PixelSizeU" ) && !fConfigFileStream.eof() );

  if( DSetupDebug) {
    cout << " - Final analysis cuts: " << endl;
    cout << "  Statistic cell size " << AnalysisParameter.StatisticCells << endl;
    cout << "  CMS cut " << AnalysisParameter.CmsNoiseCut << endl;
    cout << "  # hits max " << AnalysisParameter.MaxNbOfHits << endl;
    cout << "  # hits min " << AnalysisParameter.MinNbOfHits << endl;
    cout << "  Chi2 max " << AnalysisParameter.TrackChi2Limit << endl;
    if ( AnalysisParameter.MaxTracksExGeom > -1 ) cout << "  Max #tracks allowed in geomatrix " << AnalysisParameter.ExGeomatrix << " is " << AnalysisParameter.MaxTracksExGeom << endl;
    cout << "  # Submatrices " << AnalysisParameter.Submatrices << endl;
    cout << endl;
  }


}
//______________________________________________________________________________
//
void DSetup::ReadSubmatrixParameters( Int_t aSubmatrixNumber)
{

  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // Parameter for Submatrix
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  //
  // JB 2013/01/16
  // Modified JB 2013/07/17 Matrixtype introduced
  // Modified: JB 2013/08/21,22 new SeedIndex/Col/Row limits
  // Modified: JB 2013/09/12 new params MinNofPixelsInCluster
  // Modified: JB 2013/11/08 new cuts on charges
  // Modified: JB 2014/01/21 new cut on cluster charge

  if ( aSubmatrixNumber<0 || fMaxSubmatrices<=aSubmatrixNumber ) {
    printf( "WARNING: trying to add submatrix %d beyond the maximum number allowed (%d)\n  --> nothing done!\n", aSubmatrixNumber+1, fMaxSubmatrices);
    return;
  }
  else {
    cout << "  * adding submatrix " << aSubmatrixNumber << endl;
  }

  // some initialization
  AnalysisParameter.Matrixtype[aSubmatrixNumber] = 1;
  AnalysisParameter.Calibration[aSubmatrixNumber] = 1.;
  AnalysisParameter.Geomatrices[aSubmatrixNumber] = 0;
  AnalysisParameter.MinSeedIndex[aSubmatrixNumber] = 0; // JB 2013/08/21
  AnalysisParameter.MaxSeedIndex[aSubmatrixNumber] = 0;
  AnalysisParameter.MinSeedCol[aSubmatrixNumber] = 0; // JB 2013/08/22
  AnalysisParameter.MaxSeedCol[aSubmatrixNumber] = 0;
  AnalysisParameter.MinSeedRow[aSubmatrixNumber] = 0; // JB 2013/08/22
  AnalysisParameter.MaxSeedRow[aSubmatrixNumber] = 0;
  AnalysisParameter.MinNofPixelsInCluster[aSubmatrixNumber] = 1; // JB 2013/09/12
  AnalysisParameter.MinSeedCharge[aSubmatrixNumber] = -1000.; // JB 2013/11/08
  AnalysisParameter.MinClusterCharge[aSubmatrixNumber] = -1000.; // JB 2014/01/21
  AnalysisParameter.MinNeighbourCharge[aSubmatrixNumber] = -1000.; // JB 2013/11/08

  do {
    if( ! strcmp( fFieldName, "PixelSizeU" ) ) {
      read_item(AnalysisParameter.PixelSizeU[aSubmatrixNumber]);
    }
    else if( ! strcmp( fFieldName, "PixelSizeV" ) ) {
      read_item(AnalysisParameter.PixelSizeV[aSubmatrixNumber]);
    }
    else if( ! strcmp( fFieldName, "PixelsInRaw" ) ) {
      read_item(AnalysisParameter.PixelsInRaw[aSubmatrixNumber]);
    }
    else if( ! strcmp( fFieldName, "PixelsInColumn" ) ) {
      read_item(AnalysisParameter.PixelsInColumn[aSubmatrixNumber]);
    }
    else if( ! strcmp( fFieldName, "MinSeedIndex" ) ) { // JB 2013/08/21
      read_item(AnalysisParameter.MinSeedIndex[aSubmatrixNumber]);
    }
    else if( ! strcmp( fFieldName, "MaxSeedIndex" ) ) { // JB 2013/08/21
      read_item(AnalysisParameter.MaxSeedIndex[aSubmatrixNumber]);
    }
    else if( ! strcmp( fFieldName, "MinSeedCol" ) ) { // JB 2013/08/21
      read_item(AnalysisParameter.MinSeedCol[aSubmatrixNumber]);
    }
    else if( ! strcmp( fFieldName, "MaxSeedCol" ) ) { // JB 2013/08/21
      read_item(AnalysisParameter.MaxSeedCol[aSubmatrixNumber]);
    }
    else if( ! strcmp( fFieldName, "MinSeedRow" ) ) { // JB 2013/08/22
      read_item(AnalysisParameter.MinSeedRow[aSubmatrixNumber]);
    }
    else if( ! strcmp( fFieldName, "MaxSeedRow" ) ) { // JB 2013/08/22
      read_item(AnalysisParameter.MaxSeedRow[aSubmatrixNumber]);
    }
    else if( ! strcmp( fFieldName, "MaxNofPixelsInCluster" ) ) {
      read_item(AnalysisParameter.MaxNofPixelsInCluster[aSubmatrixNumber]);
    }
    else if( ! strcmp( fFieldName, "MinNofPixelsInCluster" ) ) { // JB 2013/09/12
      read_item(AnalysisParameter.MinNofPixelsInCluster[aSubmatrixNumber]);
    }
    else if( ! strcmp( fFieldName, "MinSeedCharge" ) ) { // JB 2013/11/08
      read_item(AnalysisParameter.MinSeedCharge[aSubmatrixNumber]);
    }
    else if( ! strcmp( fFieldName, "MinClusterCharge" ) ) { // JB 2014/01/21
      read_item(AnalysisParameter.MinClusterCharge[aSubmatrixNumber]);
    }
    else if( ! strcmp( fFieldName, "MinNeighbourCharge" ) ) { // JB 2013/11/08
      read_item(AnalysisParameter.MinNeighbourCharge[aSubmatrixNumber]);
    }
    else if( ! strcmp( fFieldName, "Matrixtype" ) ) { // JB 2013/07/17
      read_item(AnalysisParameter.Matrixtype[aSubmatrixNumber]);
    }
    else if( ! strcmp( fFieldName, "Calibration" ) ) {
      read_item(AnalysisParameter.Calibration[aSubmatrixNumber]);
    }
    else if( ! strcmp( fFieldName, "NoiseScope" ) ) {
      read_item(AnalysisParameter.NoiseScope[aSubmatrixNumber]); // JB 2010/09/06
    }
    else if( strstr( fFieldName, "GeoMatrix" ) || strstr( fFieldName, "Geomatrix" ) ) {
      if( AnalysisParameter.Geomatrices[aSubmatrixNumber] == fMaxGeomatrices ) {
        printf( "WARNING in ReadSubmatrixParameters: maximum nb of Geomatrix reached, taking no more!\n");
      }
      else {
        read_item(AnalysisParameter.Umin[aSubmatrixNumber][AnalysisParameter.Geomatrices[aSubmatrixNumber]]);
        nextItem(':');
        read_item(AnalysisParameter.Umax[aSubmatrixNumber][AnalysisParameter.Geomatrices[aSubmatrixNumber]]);
        nextItem(':');
        read_item(AnalysisParameter.Vmin[aSubmatrixNumber][AnalysisParameter.Geomatrices[aSubmatrixNumber]]);
        nextItem(':');
        read_item(AnalysisParameter.Vmax[aSubmatrixNumber][AnalysisParameter.Geomatrices[aSubmatrixNumber]]);
        AnalysisParameter.Geomatrices[aSubmatrixNumber]++;
      }
    }
    else
    {
      if (  strcmp( fFieldName, "PixelSizeU" ))
      {
        cout << "WARNING : parameter '" << fFieldName << "' in config file is not understood !" << endl;
        getRidOfLine();
      }
    }

    nextField();

  } while (  strcmp( fFieldName, "PixelSizeU" ) && !fConfigFileStream.eof() );

  if( DSetupDebug )  {
    cout << "  * Submatrix " << aSubmatrixNumber << endl;
    cout << "    pixel size U " << AnalysisParameter.PixelSizeU[aSubmatrixNumber] << " V " << AnalysisParameter.PixelSizeV[aSubmatrixNumber] << endl;
    cout << "    matrix type " << AnalysisParameter.Matrixtype[aSubmatrixNumber] << endl;
    cout << "    # pixels " << AnalysisParameter.PixelsInRaw[aSubmatrixNumber] << " in raw, " << AnalysisParameter.PixelsInColumn[aSubmatrixNumber] << " in column" << endl;
    cout << "    min - max # pixels in hit " << AnalysisParameter.MinNofPixelsInCluster[aSubmatrixNumber] << " - " << AnalysisParameter.MaxNofPixelsInCluster[aSubmatrixNumber] << endl;
    cout << "    range of seed index: " << AnalysisParameter.MinSeedIndex[aSubmatrixNumber] << " to " << AnalysisParameter.MaxSeedIndex[aSubmatrixNumber] << endl;
    cout << "    calibration " << AnalysisParameter.Calibration[aSubmatrixNumber] << endl;
    cout << "    noiseScope " << AnalysisParameter.NoiseScope[aSubmatrixNumber] << endl;
    cout << "    # Geomatrices " << AnalysisParameter.Geomatrices[aSubmatrixNumber] << endl;
      for( Int_t il=0; il<AnalysisParameter.Geomatrices[aSubmatrixNumber]; il++) {
        cout << "    GeoMatrix " << il << " " << AnalysisParameter.Umin[aSubmatrixNumber][il] << " < U < " << AnalysisParameter.Umax[aSubmatrixNumber][il] << "; " << AnalysisParameter.Vmin[aSubmatrixNumber][il] << " < V < " << AnalysisParameter.Vmax[aSubmatrixNumber][il] << endl;
      }
    }

  if ( ! strcmp( fFieldName, "PixelSizeU" ) ) {
    ReadSubmatrixParameters( aSubmatrixNumber+1);
  }

}
//______________________________________________________________________________
//
void DSetup::ReadConfiguration()
{
  // Reads telescope plane and detector configuration from configuration file
  // The call order of each method matters!
  // 1) Run param.
  // 2) Tracker param.
  // 2bis) Geometrical Parameters for the IVI vertexing
  // 3) Plane and/or Ladder param.
  // 4) DAQ param.
  // 5) DAQ Module/Board param.
  // 6) Final analysis param.
  //
  // Modified: JB 2013/01/16, use new parsing methods
  // Modified: VR 2016/06/30, run number is get from DSession and DataSubDirPrefixXXX (XXX = run #) concatenated to DataPath if set

  //DSetupDebug = 2;

  cout << endl << " -*-*- DSetup User Constructor -*-*- " << endl;

  // -+-+-+-+-+--+-+-
  // --- Initialization
  // -+-+-+-+-+--+-+-
  fConfigPathAndFileName = fConfigPath + fConfigFileName;
  printf(" - Reading Setup from %s\n", fConfigPathAndFileName.Data());

  fAddedPlanes = 0;
  fAddedLadders = 0;

  // -+-+-+-+-+--+-+-
  // --- open config file:
  // -+-+-+-+-+--+-+-
  fConfigFileStream.open(fConfigPathAndFileName);

  Bool_t    answer = fConfigFileStream.fail();
  if(answer && gROOT->IsBatch()) {
    printf("ERROR ! Can't read file %s\nQuit (because in batch mode)\n", fConfigPathAndFileName.Data());
    gSystem->Exit(-1);
  }
  while( answer ) {
    cout << "enter correct file name \n";
    cin >> fConfigFileName;
    fConfigPathAndFileName = fConfigPath + fConfigFileName;
    printf(" - Reading Setup from %s\n", fConfigPathAndFileName.Data());
    fConfigFileStream.open(fConfigPathAndFileName);
    answer=fConfigFileStream.fail();
  }

  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // Run Parameter
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  ReadRunParameters();
  RunParameter.Number = fSession->GetRunNumber();// VR 20014/06/30 Set the run number from DSession(fRunNumber), which is set by MimosaAnalysis::InitSession(const Int_t TheRun)
  if ( strcmp(RunParameter.DataSubDirPrefix,""))// VR 20014/06/30
  {
    //if DataSubDirPrefix is given, concatenate it with the run number to the DataPath
    sprintf( RunParameter.DataPath, "%s/%s%d/", RunParameter.DataPath,RunParameter.DataSubDirPrefix, RunParameter.Number ); // VR 2014/06/30
  }

  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // Parameters of the Tracker
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  ReadTrackerParameters();

  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // Parameters For Global Alignement
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  ReadGlobalAlignmentParameters();

  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // Parameters of the Experiment Geometry
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  //ReadExperimentGeometryParameters(); //Valerian 2015/02/02@18h50 : if this method causes bug, tell me so I can correct bug (In a previous commit it was comment ?!)

  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // Parameters of the Planes
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  pPlaneParameter = NULL;
  ChannelUse      = NULL;
  pPlaneParameter  = new PlaneParameter_t[TrackerParameter.Planes];
  //ChannelUse      = new Int_t*[TrackerParameter.Planes];
  //for(int iplane=0;iplane<TrackerParameter.Planes;iplane++) ChannelUse[iplane] = NULL;
  if ( ! strcmp( fFieldName, "Inputs" ) ) ReadPlaneParameters(fAddedPlanes); // contains iterative call to subsequent planes

  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // Parameters of the Ladders
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  pLadderParameter  = NULL;
  pLadderParameter  = new LadderParameter_t[TrackerParameter.Ladders];
  //ChannelUse      = new Int_t*[TrackerParameter.Ladders];
  if ( ! strcmp( fFieldName, "LadderID" ) ) ReadLadderParameters(fAddedLadders); // contains iterative call to subsequent ladders

  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // Parameter of the Data Acquisition
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  ReadDAQParameters();

  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // Parameter of the Data Acquisition boards in this run
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  pAcqModuleParameter = NULL;
  pAcqModuleParameter = new AcqModuleParameter_t[AcqParameter.ModuleTypes];
  ReadDAQBoardParameters(0); // contains iterative call to subsequent Module

  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // Parameter for Analysis
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  ReadAnalysisParameters();
  if ( ! strcmp( fFieldName, "PixelSizeU" ) ) {
    ReadSubmatrixParameters( 0); // contains iterative call to subsequent submatrices
  }
  else {
    printf( "WARNING in ReadAnalysisParameter: Missing submatrix parameters (PixelSize, ...)!\n");
  }

  // -+-+-+-+-+--+-+-
  // --- closing config file:
  // -+-+-+-+-+--+-+-
  //if(fConfigFileStream) fConfigFileStream.close();  // TAF stop here : the file will be closed in the destructor call, so I comment this line

  /*
  Char_t    tWeightFileName[200];
  Int_t     tRunIndex = GetRunPar().Number;
  sprintf(tWeightFileName,"config/inf%d.root",tRunIndex); //was DTDIR

  fWeightFileName = tWeightFileName;

  if (gSystem->AccessPathName(tWeightFileName)) {
    Info("ReadConfiguration","%s  doesn't exist you have to make a new one (Make New Eta Function gTAF->MakeEta())! ",tWeightFileName);
    fWeightFile = new TFile(tWeightFileName,"CREATE");
  }
  else {
    fWeightFile = new TFile(tWeightFileName);
  }
  //fWeightFile->Close(); // JB, Sept 2008
  */

  cout << endl << " -*-*- DSetup User Constructor DONE -*-*- " << endl;

}
//______________________________________________________________________________
//
DSetup::DSetup(const DSetup& c)
{
  copy(c);
}

//______________________________________________________________________________
//
DSetup& DSetup::operator=(const DSetup& a)
{
  copy(a);
  return *this;
}

void DSetup::copy(const DSetup& a)
{
  // Prepare a Copy into this class
}

//______________________________________________________________________________
//
void DSetup::nextItem(Char_t delimiter)
{
  // Move the file parsing pointer to the character "delimiter"

  Char_t c;
  do {
    fConfigFileStream >> c;
    if (DSetupDebug>1)  cout << c;
  } while (c != delimiter);
}

//______________________________________________________________________________
//
void DSetup::nextField()
{
  // Move the file parsing pointer to the character "delimiter"
  // store in fFieldName the previous chain of characters
  //  (skipping spaces, new lines and the delimiter)
  //
  // Modified JB 2012/12/20 include recording of fieldName
  // Modified JB 2013/01/16 skip comment lines

  Char_t delimiter = ':';
  Int_t k = 0;
  Char_t c, previousC;
  do {
    fConfigFileStream >> c;
    //cout << "|" << c << "|";
    if( c != '\n' && c != ' ' && c != delimiter && c != '.') {
      fFieldName[k] = c;
      previousC = c;
    }
    if( (c == '\n') || (c == ' ') || (k==fFieldMaxLength-1) || (c == '.'))
    {
      k = 0;
    }
    else if (c != delimiter) {
      k++;
    }
    if ( c=='/' && previousC=='/' ) {
      getRidOfLine();
      k = 0;
    }
    //cout << "k=" << k ;
  } while (c != delimiter && !fConfigFileStream.eof() );
  fFieldName[k]='\0';
  if (DSetupDebug>1)  cout << "field = " << fFieldName << endl;
}

//______________________________________________________________________________
//
void DSetup::read_r3(DR3 &arg)
{
  // Modified BH 2013/08/21 memory leak removed

  Double_t co[3] = {0., 0., 0.}; // BH 2013/08/21
  for (Int_t k = 0; k < 3; k++) {
    if( k>0 ) nextItem(':'); // already positionned for 1st value
    fConfigFileStream >> co[k];
    if (DSetupDebug>1) cout << co[k] << endl;
    arg.SetValue(co);
  }
}

//______________________________________________________________________________
//
void DSetup::read_item(Int_t &arg)
{
  //nextItem(':');
  fConfigFileStream >> arg;
  if (DSetupDebug>1){
    cout << "value = " << arg << endl;
  }
}

//______________________________________________________________________________
//
void DSetup::read_item(UInt_t &arg)
{
  //nextItem(':');
  fConfigFileStream >> arg;
  if (DSetupDebug>1){
    printf("value =%d/%x\n",arg,arg);
   }
}

//______________________________________________________________________________
//
void DSetup::read_item(Float_t &arg)
{

  // reads values from configuration file

  //nextItem(':');
  fConfigFileStream >> arg;
  if (DSetupDebug>1) cout << "value = " << arg << endl;
}

//______________________________________________________________________________
//
void DSetup::read_strings(Char_t *aString, Int_t aLength)
{

  // reads a string of a given max length from configuration file
  // The strings is expected to be contained by double quotes : "
  // JB 2009/05/25

  Int_t k = 0;
  Char_t c;
  // First, go to the " delimiter
  //nextItem('"');
  do {
    fConfigFileStream >> c;
    //cout << c;
  } while (c != '"');
  // Now, read the value up to the next " delimiter
  do {
    fConfigFileStream >> c;
    //cout << c;
    if ((c != '"') && (k < aLength)) {
      aString[k] = c;
      k++;
    }
  } while (c != '"');
  aString[k]='\0'; // end properly the string when it is shorter than max length
  if (DSetupDebug>1) cout << "value = " << aString << endl;

}
//______________________________________________________________________________
//
void DSetup::read_TStrings(TString& TheString, Int_t aLength)
{

  Char_t aString[500];
  // reads a string of a given max length from configuration file
  // The strings is expected to be contained by double quotes : "
  // JB 2009/05/25

  Int_t k = 0;
  Char_t c;
  // First, go to the " delimiter
  //nextItem('"');
  do {
    fConfigFileStream >> c;
    //cout << c;
  } while (c != '"');
  // Now, read the value up to the next " delimiter
  do {
    fConfigFileStream >> c;
    //cout << c;
    if ((c != '"') && (k < aLength)) {
      aString[k] = c;
      k++;
    }
  } while (c != '"');
  aString[k]='\0'; // end properly the string when it is shorter than max length
  if (DSetupDebug>1) cout << "value = " << aString << endl;

  TheString = TString(aString);

}
//______________________________________________________________________________
//
void DSetup::getRidOfLine()
{

  // Simply get rid of all character till the line ends,
  // line is not expected to exceeds 200 charaters.
  //
  // JB, 2013/01/16

  Char_t line[250];
  fConfigFileStream.getline( line, 250);

}
//______________________________________________________________________________
//
void DSetup::InitializePerformancesParams(int aPlaneNumber)
{

  pPlaneParameter[aPlaneNumber].PlanePerformancesList.clear();

  if(_ResolutionFileList.size() > 0) {
    pPlaneParameter[aPlaneNumber].UsingTrackerResolution = false;

    //NOTE: currently using histogram with residues (huCGwidth_vs_Mult and hvCGwidth_vs_Mult) instead of resolution. Will correction this later
    TString TheMultName         = TString("npix_c");
    TString TheHistoName_ResolU = TString("huCGwidth_vs_Mult");
    TString TheHistoName_ResolV = TString("hvCGwidth_vs_Mult");
    if(_ResolutionFileList.size() == 1) {
      for(int ireg=0;ireg<int(_ResolutionFileList.size());ireg++) {
        TH1F* hMultiplicity = NULL;
        TH1F* hResolutionU  = NULL;
        TH1F* hResolutionV  = NULL;

        TFile ResolutionVsMultFile(_ResolutionFileList[ireg].Data(),"READ");
	if(ResolutionVsMultFile.IsOpen()) {
          TString TheHistoName;

          TheHistoName  = TheMultName;
          hMultiplicity = (TH1F*)ResolutionVsMultFile.Get(TheHistoName.Data());
          if(hMultiplicity == NULL) {
            cout << endl;
            cout << "Histogram with name " << TheHistoName.Data() << " is not found in ROOT file " << _ResolutionFileList[ireg].Data()
                 << " for plane " << aPlaneNumber << " and region " << ireg+1 << "doesn't exits. Check your inputs. Exiting now!!!" << endl;
            cout << endl;
            assert(false);
          }
          hMultiplicity->Scale(1.0/hMultiplicity->GetEntries());

          TheHistoName = TheHistoName_ResolU;
          hResolutionU = (TH1F*)ResolutionVsMultFile.Get(TheHistoName.Data());
          if(hResolutionU == NULL) {
            cout << endl;
            cout << "Histogram with name " << TheHistoName.Data() << " is not found in ROOT file " << _ResolutionFileList[ireg].Data()
                 << " for plane " << aPlaneNumber << " and region " << ireg+1 << "doesn't exits. Check your inputs. Exiting now!!!" << endl;
            cout << endl;
            assert(false);
          }

          TheHistoName = TheHistoName_ResolV;
          hResolutionV = (TH1F*)ResolutionVsMultFile.Get(TheHistoName.Data());
          if(hResolutionV == NULL) {
            cout << endl;
            cout << "Histogram with name " << TheHistoName.Data() << " is not found in ROOT file " << _ResolutionFileList[ireg].Data()
                 << " for plane " << aPlaneNumber << " and region " << ireg+1 << "doesn't exits. Check your inputs. Exiting now!!!" << endl;
            cout << endl;
            assert(false);
          }

          PlanePerformances_t AResolutionVsMult;
          AResolutionVsMult.Region.R_col[0] = 0;
          AResolutionVsMult.Region.R_col[1] = pPlaneParameter[aPlaneNumber].Strips(0);
          AResolutionVsMult.Region.R_lin[0] = 0;
          AResolutionVsMult.Region.R_lin[1] = pPlaneParameter[aPlaneNumber].Strips(1);

          AResolutionVsMult.GlobalPlaneResolution  = -1.0;
          AResolutionVsMult.GlobalPlaneResolutionU = -1.0;
          AResolutionVsMult.GlobalPlaneResolutionV = -1.0;

          AResolutionVsMult.MultProb.clear();
          AResolutionVsMult.MultProbCumul.clear();
          AResolutionVsMult.ResolutionU.clear();
          AResolutionVsMult.ResolutionV.clear();
          for(int imult=0;imult<hResolutionU->GetXaxis()->GetNbins();imult++) {
            AResolutionVsMult.ResolutionU.push_back(hResolutionU->GetBinContent(imult+1));
            AResolutionVsMult.ResolutionV.push_back(hResolutionV->GetBinContent(imult+1));
          }
          for(int imult=0;imult<hResolutionU->GetXaxis()->GetNbins();imult++) {
            if(imult+1 < hResolutionU->GetXaxis()->GetNbins()) {
              double prob = hMultiplicity->GetBinContent(hMultiplicity->FindBin(imult+1));
              AResolutionVsMult.MultProb.push_back(prob);
            }
            else {
              double prob = 0.0;
              for(int kkk=0;kkk<hMultiplicity->GetXaxis()->GetNbins();kkk++) {
                double c = hMultiplicity->GetBinCenter(kkk+1);
                if(c >= imult+1) {
                  prob += hMultiplicity->GetBinContent(kkk+1);
                }
              }
              AResolutionVsMult.MultProb.push_back(prob);
            }
          }
          double prob_cumul = 0.0;
          for(int imult=0;imult<int(AResolutionVsMult.MultProb.size());imult++) {
            prob_cumul += AResolutionVsMult.MultProb[imult];
            AResolutionVsMult.MultProbCumul.push_back(prob_cumul);
          }
          pPlaneParameter[aPlaneNumber].PlanePerformancesList.push_back(AResolutionVsMult);

          ResolutionVsMultFile.Close();
        } // end if of ROOT file exists
        else {
          cout << endl;
          cout << "Resolution vs multiplicity ROOT file " << _ResolutionFileList[ireg].Data()
               << " for plane " << aPlaneNumber << " and region " << ireg+1 << "doesn't exits. Check your inputs. Exiting now!!!"
	       << endl;
          cout << endl;
          assert(false);
        }
      }
    }
    else {
      if(_ResolutionFileList.size() != _RegionList.size()) {
        cout << endl;
        cout << "Number of Resolution files given (" << _ResolutionFileList.size()
	     << ") is different from number of Regions given (" << _RegionList.size()
	     << "). Check you inputs. Exiting now!!!!" << endl;
        cout << endl;
        assert(false);
      }

      //Check that the regions defined agree with the sensor limits and that they not overlap with each others
      CheckRegionsOverlaps(aPlaneNumber);
      CheckForBadRegions(aPlaneNumber);

      for(int ireg=0;ireg<int(_RegionList.size());ireg++) {
        for(int jreg=ireg+1;jreg<int(_RegionList.size());jreg++) {
          if(_ResolutionFileList[ireg] == _ResolutionFileList[jreg]) {
            cout << "WARNING: files for regions " << ireg+1 << " and " << jreg+1 << " for Plane " << aPlaneNumber+1 << " are the same!!!." << endl;
          }
        }
      }

      for(int ireg=0;ireg<int(_ResolutionFileList.size());ireg++) {
        TH1F* hMultiplicity = NULL;
        TH1F* hResolutionU  = NULL;
        TH1F* hResolutionV  = NULL;
        TFile ResolutionVsMultFile(_ResolutionFileList[ireg].Data(),"READ");
	if(ResolutionVsMultFile.IsOpen()) {
          TString TheHistoName;

          TheHistoName  = TheMultName;
          hMultiplicity = (TH1F*)ResolutionVsMultFile.Get(TheHistoName.Data());
          if(hMultiplicity == NULL) {
            cout << endl;
            cout << "Histogram with name " << TheHistoName.Data() << " is not found in ROOT file " << _ResolutionFileList[ireg].Data()
                 << " for plane " << aPlaneNumber << " and region " << ireg+1 << "doesn't exits. Check your inputs. Exiting now!!!" << endl;
            cout << endl;
            assert(false);
          }
          hMultiplicity->Scale(1.0/hMultiplicity->GetEntries());

          TheHistoName = TheHistoName_ResolU;
          hResolutionU = (TH1F*)ResolutionVsMultFile.Get(TheHistoName.Data());
          if(hResolutionU == NULL) {
            cout << endl;
            cout << "Histogram with name " << TheHistoName.Data() << " is not found in ROOT file " << _ResolutionFileList[ireg].Data()
                 << " for plane " << aPlaneNumber << " and region " << ireg+1 << "doesn't exits. Check your inputs. Exiting now!!!" << endl;
            cout << endl;
            assert(false);
          }

          TheHistoName = TheHistoName_ResolV;
          hResolutionV = (TH1F*)ResolutionVsMultFile.Get(TheHistoName.Data());
          if(hResolutionV == NULL) {
            cout << endl;
            cout << "Histogram with name " << TheHistoName.Data() << " is not found in ROOT file " << _ResolutionFileList[ireg].Data()
                 << " for plane " << aPlaneNumber << " and region " << ireg+1 << "doesn't exits. Check your inputs. Exiting now!!!" << endl;
            cout << endl;
            assert(false);
          }

          PlanePerformances_t AResolutionVsMult;
          AResolutionVsMult.Region.R_col[0] = _RegionList[ireg].R_col[0];
          AResolutionVsMult.Region.R_col[1] = _RegionList[ireg].R_col[1];
          AResolutionVsMult.Region.R_lin[0] = _RegionList[ireg].R_lin[0];
          AResolutionVsMult.Region.R_lin[1] = _RegionList[ireg].R_lin[1];

          AResolutionVsMult.GlobalPlaneResolution  = -1.0;
          AResolutionVsMult.GlobalPlaneResolutionU = -1.0;
          AResolutionVsMult.GlobalPlaneResolutionV = -1.0;

          AResolutionVsMult.MultProb.clear();
          AResolutionVsMult.MultProbCumul.clear();
          AResolutionVsMult.ResolutionU.clear();
          AResolutionVsMult.ResolutionV.clear();
          for(int imult=0;imult<hResolutionU->GetXaxis()->GetNbins();imult++) {
            AResolutionVsMult.ResolutionU.push_back(hResolutionU->GetBinContent(imult+1));
            AResolutionVsMult.ResolutionV.push_back(hResolutionV->GetBinContent(imult+1));
          }
          for(int imult=0;imult<hResolutionU->GetXaxis()->GetNbins();imult++) {
            if(imult+1 < hResolutionU->GetXaxis()->GetNbins()) {
              double prob = hMultiplicity->GetBinContent(hMultiplicity->FindBin(imult+1));
              AResolutionVsMult.MultProb.push_back(prob);
            }
            else {
              double prob = 0.0;
              for(int kkk=0;kkk<hMultiplicity->GetXaxis()->GetNbins();kkk++) {
                double c = hMultiplicity->GetBinCenter(kkk+1);
                if(c >= imult+1) {
                  prob += hMultiplicity->GetBinContent(kkk+1);
                }
              }
              AResolutionVsMult.MultProb.push_back(prob);
            }
          }
          double prob_cumul = 0.0;
          for(int imult=0;imult<int(AResolutionVsMult.MultProb.size());imult++) {
            prob_cumul += AResolutionVsMult.MultProb[imult];
            AResolutionVsMult.MultProbCumul.push_back(prob_cumul);
          }
          pPlaneParameter[aPlaneNumber].PlanePerformancesList.push_back(AResolutionVsMult);

          ResolutionVsMultFile.Close();
        } // end if of ROOT file exists
        else {
          cout << endl;
          cout << "Resolution vs multiplicity ROOT file " << _ResolutionFileList[ireg].Data()
               << " for plane " << aPlaneNumber << " and region " << ireg+1 << "doesn't exits. Check your inputs. Exiting now!!!" << endl;
          cout << endl;
          assert(false);
        }

      }
    }
  }
  else if(_ResolutionUList.size() > 0 || _ResolutionVList.size() > 0) {
    pPlaneParameter[aPlaneNumber].UsingTrackerResolution = false;
    if(_ResolutionUList.size() != _ResolutionVList.size()) {
      cout << endl;
      cout << "Need to specify both the same number of PlaneResolutionU and PlaneResolutionV parameters for Plane " << aPlaneNumber+1
           << ". Check your inputs. Exiting now!!!"
           << endl;
      cout << endl;
      assert(false);
    }

    if(_ResolutionUList.size() == 1 && _ResolutionVList.size() == 1) {
      PlanePerformances_t AResolutionVsMult;
      AResolutionVsMult.Region.R_col[0] = 0;
      AResolutionVsMult.Region.R_col[1] = pPlaneParameter[aPlaneNumber].Strips(0);
      AResolutionVsMult.Region.R_lin[0] = 0;
      AResolutionVsMult.Region.R_lin[1] = pPlaneParameter[aPlaneNumber].Strips(1);

      AResolutionVsMult.GlobalPlaneResolution  = -1.0;
      AResolutionVsMult.GlobalPlaneResolutionU = _ResolutionUList[0];
      AResolutionVsMult.GlobalPlaneResolutionV = _ResolutionVList[0];

      AResolutionVsMult.MultProb.clear();
      AResolutionVsMult.MultProbCumul.clear();
      AResolutionVsMult.ResolutionU.clear();
      AResolutionVsMult.ResolutionV.clear();
      pPlaneParameter[aPlaneNumber].PlanePerformancesList.push_back(AResolutionVsMult);
    }
    else {
      if(_ResolutionUList.size() != _RegionList.size()) {
        cout << endl;
        cout << "Number of PlaneResolutionU/PlaneResolutionV parameters given (" << _ResolutionUList.size()
        << ") is different from number of Regions given (" << _RegionList.size()
        << "). Check you inputs. Exiting now!!!!" << endl;
        cout << endl;
        assert(false);
      }

      //Check that the regions defined agree with the sensor limits and that they not overlap with each others
      CheckRegionsOverlaps(aPlaneNumber);
      CheckForBadRegions(aPlaneNumber);

      for(int ireg=0;ireg<int(_ResolutionUList.size());ireg++) {
        PlanePerformances_t AResolutionVsMult;
        AResolutionVsMult.Region.R_col[0] = _RegionList[ireg].R_col[0];
        AResolutionVsMult.Region.R_col[1] = _RegionList[ireg].R_col[1];
        AResolutionVsMult.Region.R_lin[0] = _RegionList[ireg].R_lin[0];
        AResolutionVsMult.Region.R_lin[1] = _RegionList[ireg].R_lin[1];

        AResolutionVsMult.GlobalPlaneResolution  = -1.0;
        AResolutionVsMult.GlobalPlaneResolutionU = _ResolutionUList[ireg];
        AResolutionVsMult.GlobalPlaneResolutionV = _ResolutionVList[ireg];

        AResolutionVsMult.MultProb.clear();
        AResolutionVsMult.MultProbCumul.clear();
        AResolutionVsMult.ResolutionU.clear();
        AResolutionVsMult.ResolutionV.clear();
        pPlaneParameter[aPlaneNumber].PlanePerformancesList.push_back(AResolutionVsMult);
      }
    }
  }
  else if(_ResolutionList.size() > 0) {
    pPlaneParameter[aPlaneNumber].UsingTrackerResolution = false;
    if(_ResolutionList.size() == 1) {
      PlanePerformances_t AResolutionVsMult;
      AResolutionVsMult.Region.R_col[0] = 0;
      AResolutionVsMult.Region.R_col[1] = pPlaneParameter[aPlaneNumber].Strips(0);
      AResolutionVsMult.Region.R_lin[0] = 0;
      AResolutionVsMult.Region.R_lin[1] = pPlaneParameter[aPlaneNumber].Strips(1);

      AResolutionVsMult.GlobalPlaneResolution  = _ResolutionList[0];
      AResolutionVsMult.GlobalPlaneResolutionU = -1;
      AResolutionVsMult.GlobalPlaneResolutionV = -1;

      AResolutionVsMult.MultProb.clear();
      AResolutionVsMult.MultProbCumul.clear();
      AResolutionVsMult.ResolutionU.clear();
      AResolutionVsMult.ResolutionV.clear();
      pPlaneParameter[aPlaneNumber].PlanePerformancesList.push_back(AResolutionVsMult);
    }
    else {
      if(_ResolutionList.size() != _RegionList.size()) {
        cout << endl;
        cout << "Number of PlaneResolution parameters given (" << _ResolutionUList.size()
        << ") is different from number of Regions given (" << _RegionList.size()
        << "). Check you inputs. Exiting now!!!!" << endl;
        cout << endl;
        assert(false);
      }

      //Check that the regions defined agree with the sensor limits and that they not overlap with each others
      CheckRegionsOverlaps(aPlaneNumber);
      CheckForBadRegions(aPlaneNumber);

      for(int ireg=0;ireg<int(_ResolutionList.size());ireg++) {
        PlanePerformances_t AResolutionVsMult;
        AResolutionVsMult.Region.R_col[0] = _RegionList[ireg].R_col[0];
        AResolutionVsMult.Region.R_col[1] = _RegionList[ireg].R_col[1];
        AResolutionVsMult.Region.R_lin[0] = _RegionList[ireg].R_lin[0];
        AResolutionVsMult.Region.R_lin[1] = _RegionList[ireg].R_lin[1];

        AResolutionVsMult.GlobalPlaneResolution  = _ResolutionList[ireg];
        AResolutionVsMult.GlobalPlaneResolutionU = -1.0;
        AResolutionVsMult.GlobalPlaneResolutionV = -1.0;

        AResolutionVsMult.MultProb.clear();
        AResolutionVsMult.MultProbCumul.clear();
        AResolutionVsMult.ResolutionU.clear();
        AResolutionVsMult.ResolutionV.clear();
        pPlaneParameter[aPlaneNumber].PlanePerformancesList.push_back(AResolutionVsMult);
      }
    }
  }
  else if(_ResolutionList.size() == 0) {
    pPlaneParameter[aPlaneNumber].UsingTrackerResolution = true;
    PlanePerformances_t AResolutionVsMult;
    AResolutionVsMult.Region.R_col[0] = 0;
    AResolutionVsMult.Region.R_col[1] = pPlaneParameter[aPlaneNumber].Strips(0);
    AResolutionVsMult.Region.R_lin[0] = 0;
    AResolutionVsMult.Region.R_lin[1] = pPlaneParameter[aPlaneNumber].Strips(1);

    AResolutionVsMult.GlobalPlaneResolution  = TrackerParameter.Resolution;
    AResolutionVsMult.GlobalPlaneResolutionU = -1.0;
    AResolutionVsMult.GlobalPlaneResolutionV = -1.0;

    AResolutionVsMult.MultProb.clear();
    AResolutionVsMult.MultProbCumul.clear();
    AResolutionVsMult.ResolutionU.clear();
    AResolutionVsMult.ResolutionV.clear();
    pPlaneParameter[aPlaneNumber].PlanePerformancesList.push_back(AResolutionVsMult);
  }

  ///////////////////////////////////
  //Printing out the resolution parameters obtained from config file:
  ///////////////////////////////////
  if(pPlaneParameter[aPlaneNumber].PlanePerformancesList.size() == 1) {
    //No region defined:
    if(pPlaneParameter[aPlaneNumber].PlanePerformancesList[0].ResolutionU.size() == 0) {
      //No multiplicity dependent resolution defined
      if(pPlaneParameter[aPlaneNumber].PlanePerformancesList[0].GlobalPlaneResolutionU == -1) {
	//Using same resolution for U and V
	if(pPlaneParameter[aPlaneNumber].UsingTrackerResolution) {
	  //Using same resolution as the one defined on tracker block
	  cout << "Using global TrakerResolution for PlaneResolution for Plane " << aPlaneNumber+1
	       << ". Resolution = " << pPlaneParameter[aPlaneNumber].PlanePerformancesList[0].GlobalPlaneResolution << "um" << endl;
	  cout << "Using the same resolution for U and V directions!!!" << endl;
	}
	else {
	  //Using user defined global resolution
	  cout << "Using user defined global PlaneResolution for Plane " << aPlaneNumber+1
	       << ". Resolution = " << pPlaneParameter[aPlaneNumber].PlanePerformancesList[0].GlobalPlaneResolution << "um" << endl;
	  cout << "Using the same resolution for U and V directions!!!" << endl;
	}
      }
      else {
	//Using different global resolutions for U and
	cout << "Using different PlaneResolutions for Plane " << aPlaneNumber+1 << " for U and V directions." << endl;
	cout << "ResolutionU = " << pPlaneParameter[aPlaneNumber].PlanePerformancesList[0].GlobalPlaneResolutionU << "um" << endl;
	cout << "ResolutionV = " << pPlaneParameter[aPlaneNumber].PlanePerformancesList[0].GlobalPlaneResolutionV << "um" << endl;
      }
    }
    else {
      //Using multiplicity dependent resolutions for U and V
      cout << "Multiplicity dependent resolution for Plane" << aPlaneNumber+1 << ":" << endl;
      for(unsigned long imult=0;imult<pPlaneParameter[aPlaneNumber].PlanePerformancesList[0].ResolutionU.size();imult++) {
        cout << "(ResolutionU,ResolutionV) = ("
        << pPlaneParameter[aPlaneNumber].PlanePerformancesList[0].ResolutionU[imult] << ","
        << pPlaneParameter[aPlaneNumber].PlanePerformancesList[0].ResolutionV[imult] << ") um (prob = "
        << pPlaneParameter[aPlaneNumber].PlanePerformancesList[0].MultProb[imult]*100.0 << " %, prob_cumul = "
        << pPlaneParameter[aPlaneNumber].PlanePerformancesList[0].MultProbCumul[imult]*100.0 << " %) for ";
        if(imult < pPlaneParameter[aPlaneNumber].PlanePerformancesList[0].ResolutionU.size()-1) {
          cout << " mult = " << imult+1;
        }
        else cout << " mult >= " << imult+1;
        cout << endl;
      }
      cout << endl;
    }
  }
  else {
    //Different regions defined:
    cout << "Found " << pPlaneParameter[aPlaneNumber].PlanePerformancesList.size()
    << " regions for Plane " << aPlaneNumber+1 << "."
    << endl;
    for(int ireg=0;ireg<int(pPlaneParameter[aPlaneNumber].PlanePerformancesList.size());ireg++) {
      cout << "Region " << ireg+1 << ": col = ("
      << pPlaneParameter[aPlaneNumber].PlanePerformancesList[ireg].Region.R_col[0] << ","
      << pPlaneParameter[aPlaneNumber].PlanePerformancesList[ireg].Region.R_col[1] << "), lin = ("
      << pPlaneParameter[aPlaneNumber].PlanePerformancesList[ireg].Region.R_lin[0] << ","
      << pPlaneParameter[aPlaneNumber].PlanePerformancesList[ireg].Region.R_lin[1] << ")."
      //<< " Estimated fake rate = " << pPlaneParameter[aPlaneNumber].PlanePerformancesList[ireg].FakeRate
      << endl;
      if(pPlaneParameter[aPlaneNumber].PlanePerformancesList[ireg].ResolutionU.size() == 0) {
        //No multiplicity dependent resolution defined
        if(pPlaneParameter[aPlaneNumber].PlanePerformancesList[ireg].GlobalPlaneResolutionU == -1) {
          //Using same resolution for U and V
          if(pPlaneParameter[aPlaneNumber].UsingTrackerResolution) {
            //Using same resolution as the one defined on tracker block
            cout << "Using global TrakerResolution for PlaneResolution for Plane " << aPlaneNumber+1 << " and Region " << ireg+1
            << ". Resolution = " << pPlaneParameter[aPlaneNumber].PlanePerformancesList[ireg].GlobalPlaneResolution << "um" << endl;
            cout << "Using the same resolution for U and V directions!!!" << endl;
          }
          else {
            //Using user defined global resolution
            cout << "Using user defined global PlaneResolution for Plane " << aPlaneNumber+1 << " and Region " << ireg+1
            << ". Resolution = " << pPlaneParameter[aPlaneNumber].PlanePerformancesList[ireg].GlobalPlaneResolution << "um" << endl;
            cout << "Using the same resolution for U and V directions!!!" << endl;
          }
        }
        else {
          //Using different global resolutions for U and V
          cout << "Using different PlaneResolutions for Plane " << aPlaneNumber+1 << " for U and V directions." << endl;
          cout << "ResolutionU = " << pPlaneParameter[aPlaneNumber].PlanePerformancesList[ireg].GlobalPlaneResolutionU << "um" << endl;
          cout << "ResolutionV = " << pPlaneParameter[aPlaneNumber].PlanePerformancesList[ireg].GlobalPlaneResolutionV << "um" << endl;
        }
      }
      else {
        //Using multiplicity dependent resolutions for U and V
        for(unsigned long imult=0;imult<pPlaneParameter[aPlaneNumber].PlanePerformancesList[ireg].ResolutionU.size();imult++) {
          cout << "(ResolutionU,ResolutionV) = ("
          << pPlaneParameter[aPlaneNumber].PlanePerformancesList[ireg].ResolutionU[imult] << ","
          << pPlaneParameter[aPlaneNumber].PlanePerformancesList[ireg].ResolutionV[imult] << ") um (prob = "
          << pPlaneParameter[aPlaneNumber].PlanePerformancesList[ireg].MultProb[imult]*100.0 << " %, prob_cumul = "
          << pPlaneParameter[aPlaneNumber].PlanePerformancesList[ireg].MultProbCumul[imult]*100.0 << " %) for ";
          if(imult < pPlaneParameter[aPlaneNumber].PlanePerformancesList[ireg].ResolutionU.size()-1) {
            cout << " mult = " << imult+1;
          }
          else cout << " mult >= " << imult+1;
          cout << endl;
        }
        cout << endl;
      }
    }
  }

  return;

}
//______________________________________________________________________________
//
void DSetup::CheckRegionsOverlaps(int aPlaneNumber)
{

  bool RegionsOverlap = false;
  for(int ireg=0;ireg<int(_RegionList.size());ireg++) {
    for(int jreg=ireg+1;jreg<int(_RegionList.size());jreg++) {
      int Corners[4][2];
      Corners[0][0] = _RegionList[jreg].R_col[0]; Corners[0][1] = _RegionList[jreg].R_lin[0];
      Corners[1][0] = _RegionList[jreg].R_col[1]; Corners[1][1] = _RegionList[jreg].R_lin[0];
      Corners[2][0] = _RegionList[jreg].R_col[0]; Corners[2][1] = _RegionList[jreg].R_lin[1];
      Corners[3][0] = _RegionList[jreg].R_col[1]; Corners[3][1] = _RegionList[jreg].R_lin[1];

      for(int icorners=0;icorners<4;icorners++) {
	if((Corners[icorners][0] >= _RegionList[ireg].R_col[0] && Corners[icorners][0] <= _RegionList[ireg].R_col[1]) &&
           (Corners[icorners][1] >= _RegionList[ireg].R_lin[0] && Corners[icorners][1] <= _RegionList[ireg].R_lin[1])) {
	  RegionsOverlap = true;
	  cout << "region " << ireg+1 << " overlaps with region " << jreg+1 << endl;
          break;
        }
      }
      if(RegionsOverlap) break;
    }
    if(RegionsOverlap) break;
  }
  if(RegionsOverlap) {
    cout << endl;
    cout << "At least a couple of the regions defined for plane " << aPlaneNumber << " overlap. Check you inputs. Exiting now!!!!" << endl;
    cout << endl;
    assert(false);
  }

  return;

}
//______________________________________________________________________________
//
void DSetup::CheckForBadRegions(int aPlaneNumber)
{

  bool BadRegion = false;
  for(int ireg=0;ireg<int(_RegionList.size());ireg++) {
    if(_RegionList[ireg].R_col[0] < 0 ||
       _RegionList[ireg].R_col[1] > pPlaneParameter[aPlaneNumber].Strips(0)-1 ||
       _RegionList[ireg].R_lin[0] < 0 ||
       _RegionList[ireg].R_lin[1] > pPlaneParameter[aPlaneNumber].Strips(1)-1
       ) {
      BadRegion = true;
      cout << "Region " << ireg+1 << " has limits outside sensor limits!. Region col = (" << _RegionList[ireg].R_col[0] << "," << _RegionList[ireg].R_col[1] << "), lin = ("
           << _RegionList[ireg].R_lin[0] << "," << _RegionList[ireg].R_lin[1] << "). Sensor col = (" << 0 << "," << pPlaneParameter[aPlaneNumber].Strips(0)-1
           << "), lin = (" << 0 << "," << pPlaneParameter[aPlaneNumber].Strips(1)-1 << ")"
           << endl;
      break;
    }
  }
  if(BadRegion) {
    cout << endl;
    cout << "At least a of the regions defined for Plane " << aPlaneNumber+1 << " overlap is outside of the sensor limits. Check you inputs. Exiting now!!!!" << endl;
    cout << endl;
    assert(false);
  }

  return;

}
//______________________________________________________________________________
//
void DSetup::GetListOfHotPixelsToMask_FracToMask(int aPlaneNumber,
                                                 TString HotPixelMapFile)
{

  pPlaneParameter[aPlaneNumber].HotPixelList_lin.clear();
  pPlaneParameter[aPlaneNumber].HotPixelList_col.clear();
  pPlaneParameter[aPlaneNumber].HotPixelList_index.clear();

  //Do nothing if hot pixel list for masking is not requested
  if(_FractionToMaskList.size() == 0 || HotPixelMapFile == TString("")) return;

  TH2F* h2HotPixelMap    = NULL;
  TH1F* h1HotPixelList   = NULL;
  TFile FakeRateMapFile(HotPixelMapFile.Data(),"READ");
  if(!FakeRateMapFile.IsOpen()) {
    //Warning message when ROOT file doesn't exists. Do nothing.
    cout << endl;
    cout << " WARNNING: ROOT file " << HotPixelMapFile.Data()
    << " with fake rate map for plane " << aPlaneNumber+1
    << " doesn't exist. Doing nothing, list of hot is left empty." << endl;
    cout << endl;

    return;
  }
  /*
   else {
   cout << endl;
   cout << "Opening file " << HotPixelMapFile.Data() << ", with contents:" << endl;
   FakeRateMapFile.ls();
   cout << endl;
   }
   */

  h2HotPixelMap = (TH2F*)FakeRateMapFile.Get("h2HotPixelMap");
  if(h2HotPixelMap == NULL) { // if occupancy h2 map doest not exists
    //Warning message when ROOT file exists, but fake rate map histogram is not found. Do nothing.
    cout << endl;
    cout << " WARNNING: Fake rate map histogram h2HotPixelMap is not found in ROOT file " << HotPixelMapFile.Data()
    << " for plane " << aPlaneNumber+1 << ". Doing nothing, list of hot is left empty." << endl;
    cout << endl;
  }  // end if occupancy h2 map does not exist
  else {
    //if h2 map exists
    //h2HotPixelMap->SetDirectory(0);
    cout << "Looping over the fake-rate map to find hot pixels. Plane " << aPlaneNumber+1 << endl;
    //Loop over the fake rate map to identify the hot pixels

    if(_FractionToMaskList.size() == 1) {
      cout << "Specified a single Fraction to Mask of " << _FractionToMaskList[0]*100 << "% for whole matrix of plane " << aPlaneNumber+1 << endl;
      //If only specified a singl FakeRateCut, then apply the cut over the full matrix of pixels
      double RU_tmp[2];
      RU_tmp[0] = h2HotPixelMap->GetXaxis()->GetXmin()+0.5;
      RU_tmp[1] = h2HotPixelMap->GetXaxis()->GetXmax()+0.5;
      double RV_tmp[2];
      RV_tmp[0] = h2HotPixelMap->GetYaxis()->GetXmin()+0.5;
      RV_tmp[1] = h2HotPixelMap->GetYaxis()->GetXmax()+0.5;

      if(!(RU_tmp[0] == 0 && RU_tmp[1] == pPlaneParameter[aPlaneNumber].Strips(0) &&
           RV_tmp[0] == 0 && RV_tmp[1] == pPlaneParameter[aPlaneNumber].Strips(1))) {
        cout << "WARNING: the range of the hot pixels map histrograms don't agree with the number of pixels of the matrix:" << endl;
        cout << "WARNING: U-range = (" << RU_tmp[0] << "," << RU_tmp[1] << "), and expects (0," << pPlaneParameter[aPlaneNumber].Strips(0) << ")" << endl;
        cout << "WARNING: V-range = (" << RV_tmp[0] << "," << RV_tmp[1] << "), and expects (0," << pPlaneParameter[aPlaneNumber].Strips(1) << ")" << endl;
        cout << "WARNING: Maybe need to check your inputs!!!" << endl;
      }

      //Put the pixels in a map which is ordered from the lowest to the highest fake-rate
      std::vector<APixel_t> _fake_map;
      _fake_map.clear();
      for(Int_t iy=0;iy<h2HotPixelMap->GetYaxis()->GetNbins();iy++) {
        Int_t lin = Int_t(h2HotPixelMap->GetYaxis()->GetBinCenter(iy+1));
        for(Int_t ix=0;ix<h2HotPixelMap->GetXaxis()->GetNbins();ix++) {
          Int_t col = Int_t(h2HotPixelMap->GetXaxis()->GetBinCenter(ix+1));
          double fake_rate = h2HotPixelMap->GetBinContent(ix+1,iy+1)/100.0;

          if(fake_rate > 0.0) {
            APixel_t Mypixel;
            Mypixel.col  = col;
            Mypixel.lin  = lin;
            Mypixel.fake = fake_rate;
            _fake_map.push_back(Mypixel);
          }

        }
      }
      for(int iii=2;iii<=int(_fake_map.size());iii++) {
        for(int jjj=0;jjj<=int(_fake_map.size())-iii;jjj++) {
          double fake_jjj   = _fake_map[jjj].fake;
          double fake_jjjp1 = _fake_map[jjj+1].fake;

          if(fake_jjj < fake_jjjp1) {
            APixel_t aux_pixel  = _fake_map[jjj];
            _fake_map[jjj]      = _fake_map[jjj+1];
            _fake_map[jjj+1]    = aux_pixel;
          }
        }
      }

      //Define the number of pixels to mask based on mask fraction
      int Npixels = h2HotPixelMap->GetXaxis()->GetNbins()*h2HotPixelMap->GetYaxis()->GetNbins();
      int NtoMask = int(Npixels*_FractionToMaskList[0]);

      int counter = 0;
      for(int i=0;i<int(_fake_map.size());i++) {
        counter++;
        if(counter > NtoMask) break;
        int col     = _fake_map[i].col;
        int lin     = _fake_map[i].lin;
        //double fake = _fake_map[i].fake;
        //cout << "(col,lin) = (" << col << "," << lin << "), value = " << fake << endl;

        pPlaneParameter[aPlaneNumber].HotPixelList_lin.push_back(lin);
        pPlaneParameter[aPlaneNumber].HotPixelList_col.push_back(col);
      }

      _fake_map.clear();
    }
    else if(_FractionToMaskList.size() > 1) {
      //Specified more than one FakeRateCut

      if(_FractionToMaskList.size() != _RegionList.size()) {
        //Check that the FakeRateCut list has the same size as the Region list. If not exit and do nothing.
        cout << endl;
        cout << "WARNING: FractionToMaskList size (" << _FractionToMaskList.size() << ") if different from region list size (" << _RegionList.size() << ")" << endl;
        cout << "WARNING: no attempt to build hot pixels list. Check your inputs!!!" << endl;
        cout << endl;

//        FakeRateMapFile.Close();

//        return;
      }
      else {
        //Check that the regions defined agree with the sensor limits and that they not overlap with each others
        CheckRegionsOverlaps(aPlaneNumber);
        CheckForBadRegions(aPlaneNumber);

        cout << "List of Fraction to Mask for different regions:" << endl;
        for(int ireg=0;ireg<int(_RegionList.size());ireg++) {
          cout << "Region " << ireg+1 << ": col = ("
          << _RegionList[ireg].R_col[0] << ","
          << _RegionList[ireg].R_col[1] << "), lin = ("
          << _RegionList[ireg].R_lin[0] << ","
          << _RegionList[ireg].R_lin[1] << "), Fraction to Mask = "
          << _FractionToMaskList[ireg]*100 << "%"
          << endl;
        }
        cout << endl;

        for(int ireg=0;ireg<int(_RegionList.size());ireg++) { //Loop over regions
          int Npixels = 0;
          std::vector<APixel_t> _fake_map;
          _fake_map.clear();

          //Apply the fake rate cut depending on the regions defined
          for(Int_t iy=0;iy<h2HotPixelMap->GetYaxis()->GetNbins();iy++) {
            Int_t lin = Int_t(h2HotPixelMap->GetYaxis()->GetBinCenter(iy+1));
            for(Int_t ix=0;ix<h2HotPixelMap->GetXaxis()->GetNbins();ix++) {
              Int_t col = Int_t(h2HotPixelMap->GetXaxis()->GetBinCenter(ix+1));

              if((col >= _RegionList[ireg].R_col[0] && col <= _RegionList[ireg].R_col[1]) && (lin >= _RegionList[ireg].R_lin[0] && lin <= _RegionList[ireg].R_lin[1])) {
                double fake_rate = h2HotPixelMap->GetBinContent(ix+1,iy+1)/100.0;
                Npixels++;

                if(fake_rate > 0.0) {
                  APixel_t Mypixel;
                  Mypixel.col  = col;
                  Mypixel.lin  = lin;
                  Mypixel.fake = fake_rate;
                  _fake_map.push_back(Mypixel);
                }
              }

            }
          }
          for(int iii=2;iii<=int(_fake_map.size());iii++) {
            for(int jjj=0;jjj<=int(_fake_map.size())-iii;jjj++) {
              double fake_jjj   = _fake_map[jjj].fake;
              double fake_jjjp1 = _fake_map[jjj+1].fake;

              if(fake_jjj < fake_jjjp1) {
                APixel_t aux_pixel  = _fake_map[jjj];
                _fake_map[jjj]      = _fake_map[jjj+1];
                _fake_map[jjj+1]    = aux_pixel;
              }
            }
          }

          int NtoMask = int(Npixels*_FractionToMaskList[ireg]);

          int counter = 0;
          for(int i=0;i<int(_fake_map.size());i++) {
            counter++;
            if(counter > NtoMask) break;
            int col     = _fake_map[i].col;
            int lin     = _fake_map[i].lin;
            //double fake = _fake_map[i].fake;
            //cout << "Region " << ireg+1 << ":  (col,lin) = (" << col << "," << lin << "), value = " << fake << endl;
            pPlaneParameter[aPlaneNumber].HotPixelList_lin.push_back(lin);
            pPlaneParameter[aPlaneNumber].HotPixelList_col.push_back(col);
          }
          _fake_map.clear();

        } //end of loop over regions

      } // end of if fake-rate list size is equal to ragion list size

    } // end of if fake-rate list size is higher than one
//    FakeRateMapFile.Close();

//    return;

  } // end if occupancy h2 map exists

  if(_FractionToMaskList.size() == 1) {
    cout << "Looping over the fake-rate list to find hot pixels. Plane " << aPlaneNumber+1 << endl;
    cout << "Specified a single Fraction to Mask of " << _FractionToMaskList[0]*100 << "% for whole matrix of plane " << aPlaneNumber+1 << endl;
    //Only use hot pixel list by global index when fake-rate list size is equal to 1
    h1HotPixelList = (TH1F*)FakeRateMapFile.Get(Form("h1HotPixelListPl%d",aPlaneNumber+1));
    if(h1HotPixelList != NULL) { // if occupancy h1 list exists
      //h1HotPixelList->SetDirectory(0);

      int Npixels = 0;
      std::vector<APixel_t> _fake_map;
      _fake_map.clear();
      for(Int_t iindex=0;iindex<h1HotPixelList->GetXaxis()->GetNbins();iindex++) {
        double fake_rate = h1HotPixelList->GetBinContent(iindex+1)/100.0;
        Npixels++;

        if(fake_rate > 0.0) {
          //Use the col variable of APixel_t object to store the value of the pixels index
          //Set always the lin varianle to -1
          APixel_t Mypixel;
          Mypixel.col  = iindex;
          Mypixel.lin  = -1;
          Mypixel.fake = fake_rate;
          _fake_map.push_back(Mypixel);
        }
      }
      for(int iii=2;iii<=int(_fake_map.size());iii++) {
        for(int jjj=0;jjj<=int(_fake_map.size())-iii;jjj++) {
          double fake_jjj   = _fake_map[jjj].fake;
          double fake_jjjp1 = _fake_map[jjj+1].fake;

          if(fake_jjj < fake_jjjp1) {
            APixel_t aux_pixel  = _fake_map[jjj];
            _fake_map[jjj]      = _fake_map[jjj+1];
            _fake_map[jjj+1]    = aux_pixel;
          }
        }
      }

      int NtoMask = int(Npixels*_FractionToMaskList[0]);

      int counter = 0;
      for(int i=0;i<int(_fake_map.size());i++) {
        counter++;
        if(counter > NtoMask) break;
        int index     = _fake_map[i].col;
        //double fake = _fake_map[i].fake;
        //cout << "index = " << index << ", value = " << fake << endl;
        pPlaneParameter[aPlaneNumber].HotPixelList_index.push_back(index);
      }
      _fake_map.clear();

//      FakeRateMapFile.Close();

//      return;

    } // end if occupancy h1 list exists
  } // end if FakeRateCut list is == 1

  FakeRateMapFile.Close();

  return;

}
//______________________________________________________________________________
//
void DSetup::GetListOfHotPixelsToMask_FakeRateCut(int aPlaneNumber,
                                                  TString HotPixelMapFile)
{


  pPlaneParameter[aPlaneNumber].HotPixelList_lin.clear();
  pPlaneParameter[aPlaneNumber].HotPixelList_col.clear();
  pPlaneParameter[aPlaneNumber].HotPixelList_index.clear();

  //Do nothing if hot pixel list for masking is not requested
  if(_FakeRateCutList.size() == 0 || HotPixelMapFile == TString("")) return;

  TH2F* h2HotPixelMap    = NULL;
  TH1F* h1HotPixelList   = NULL;
  TFile FakeRateMapFile(HotPixelMapFile.Data(),"READ");
  if(!FakeRateMapFile.IsOpen()) {
    //Warning message when ROOT file doesn't exists. Do nothing.
    cout << endl;
    cout << " WARNNING: ROOT file " << HotPixelMapFile.Data()
    << " with fake rate map for plane " << aPlaneNumber+1
    << " doesn't exist. Doing nothing, list of hot is left empty." << endl;
    cout << endl;

    return;
  }
  /*
   else {
   cout << endl;
   cout << "Opening file " << HotPixelMapFile.Data() << ", with contents:" << endl;
   FakeRateMapFile.ls();
   cout << endl;
   }
   */

  h2HotPixelMap = (TH2F*)FakeRateMapFile.Get("h2HotPixelMap");
  if(h2HotPixelMap == NULL) { // if occupancy h2 map doest not exists
    //Warning message when ROOT file exists, but fake rate map histogram is not found. Do nothing.
    cout << endl;
    cout << " WARNNING: Fake rate map histogram h2HotPixelMap is not found in ROOT file " << HotPixelMapFile.Data()
    << " for plane " << aPlaneNumber+1 << ". Doing nothing, list of hot is left empty." << endl;
    cout << endl;
  }  // end if occupancy h2 map does not exist
  else {
    //if h2 map exists
    //h2HotPixelMap->SetDirectory(0);
    cout << "Looping over the fake-rate map to find hot pixels. Plane " << aPlaneNumber+1 << endl;
    //Loop over the fake rate map to identify the hot pixels

    if(_FakeRateCutList.size() == 1) {
      cout << "Specified a single FakeRateCut of " << _FakeRateCutList[0]*100 << "% for whole matrix of plane " << aPlaneNumber+1 << endl;
      //If only specified a singl FakeRateCut, then apply the cut over the full matrix of pixels
      double RU_tmp[2];
      RU_tmp[0] = h2HotPixelMap->GetXaxis()->GetXmin()+0.5;
      RU_tmp[1] = h2HotPixelMap->GetXaxis()->GetXmax()+0.5;
      double RV_tmp[2];
      RV_tmp[0] = h2HotPixelMap->GetYaxis()->GetXmin()+0.5;
      RV_tmp[1] = h2HotPixelMap->GetYaxis()->GetXmax()+0.5;

      if(!(RU_tmp[0] == 0 && RU_tmp[1] == pPlaneParameter[aPlaneNumber].Strips(0) &&
           RV_tmp[0] == 0 && RV_tmp[1] == pPlaneParameter[aPlaneNumber].Strips(1))) {
        cout << "WARNING: the range of the hot pixels map histrograms don't agree with the number of pixels of the matrix:" << endl;
        cout << "WARNING: U-range = (" << RU_tmp[0] << "," << RU_tmp[1] << "), and expects (0," << pPlaneParameter[aPlaneNumber].Strips(0) << ")" << endl;
        cout << "WARNING: V-range = (" << RV_tmp[0] << "," << RV_tmp[1] << "), and expects (0," << pPlaneParameter[aPlaneNumber].Strips(1) << ")" << endl;
        cout << "WARNING: Maybe need to check your inputs!!!" << endl;
      }

      //Put the pixels in a map which is ordered from the lowest to the highest fake-rate
      for(Int_t iy=0;iy<h2HotPixelMap->GetYaxis()->GetNbins();iy++) {
        Int_t lin = Int_t(h2HotPixelMap->GetYaxis()->GetBinCenter(iy+1));
        for(Int_t ix=0;ix<h2HotPixelMap->GetXaxis()->GetNbins();ix++) {
          Int_t col = Int_t(h2HotPixelMap->GetXaxis()->GetBinCenter(ix+1));
          double fake_rate = h2HotPixelMap->GetBinContent(ix+1,iy+1)/100.0;

          if(fake_rate >= _FakeRateCutList[0]) {
            pPlaneParameter[aPlaneNumber].HotPixelList_lin.push_back(lin);
            pPlaneParameter[aPlaneNumber].HotPixelList_col.push_back(col);
          }

        }
      }

    }
    else if(_FakeRateCutList.size() > 1) {
      //Specified more than one FakeRateCut

      if(_FakeRateCutList.size() != _RegionList.size()) {
        //Check that the FakeRateCut list has the same size as the Region list. If not exit and do nothing.
        cout << endl;
        cout << "WARNING: FakeRateCutList size (" << _FakeRateCutList.size() << ") if different from region list size (" << _RegionList.size() << ")" << endl;
        cout << "WARNING: no attempt to build hot pixels list. Check your inputs!!!" << endl;
        cout << endl;

//        FakeRateMapFile.Close();

//        return;
      }
      else {
        //Check that the regions defined agree with the sensor limits and that they not overlap with each others
        CheckRegionsOverlaps(aPlaneNumber);
        CheckForBadRegions(aPlaneNumber);

        cout << "List of fake-rate cuts for different regions:" << endl;
        for(int ireg=0;ireg<int(_RegionList.size());ireg++) {
          cout << "Region " << ireg+1 << ": col = ("
          << _RegionList[ireg].R_col[0] << ","
          << _RegionList[ireg].R_col[1] << "), lin = ("
          << _RegionList[ireg].R_lin[0] << ","
          << _RegionList[ireg].R_lin[1] << "), fake-rate cut = "
          << _FakeRateCutList[ireg]*100 << "%"
          << endl;
        }
        cout << endl;

        for(int ireg=0;ireg<int(_RegionList.size());ireg++) { //Loop over regions

          //Apply the fake rate cut depending on the regions defined
          for(Int_t iy=0;iy<h2HotPixelMap->GetYaxis()->GetNbins();iy++) {
            Int_t lin = Int_t(h2HotPixelMap->GetYaxis()->GetBinCenter(iy+1));
            for(Int_t ix=0;ix<h2HotPixelMap->GetXaxis()->GetNbins();ix++) {
              Int_t col = Int_t(h2HotPixelMap->GetXaxis()->GetBinCenter(ix+1));

              if((col >= _RegionList[ireg].R_col[0] && col <= _RegionList[ireg].R_col[1]) && (lin >= _RegionList[ireg].R_lin[0] && lin <= _RegionList[ireg].R_lin[1])) {
                double fake_rate = h2HotPixelMap->GetBinContent(ix+1,iy+1)/100.0;
                if(fake_rate >= _FakeRateCutList[ireg]) {
                  pPlaneParameter[aPlaneNumber].HotPixelList_lin.push_back(lin);
                  pPlaneParameter[aPlaneNumber].HotPixelList_col.push_back(col);
                }
              }

            }
          }

        } //end of loop over regions

      } // end of if fake-rate list size is equal to ragion list size
    } // end of if fake-rate list size is higher than one

//    FakeRateMapFile.Close();

//    return;

  } // end if occupancy h2 map exists

  if(_FakeRateCutList.size() == 1) {
    cout << "Looping over the fake-rate list to find hot pixels. Plane " << aPlaneNumber+1 << endl;
    cout << "Specified a single Fake-rate cut of " << _FakeRateCutList[0]*100 << "% for whole matrix of plane " << aPlaneNumber+1 << endl;
    //Only use hot pixel list by global index when fake-rate list size is equal to 1
    h1HotPixelList = (TH1F*)FakeRateMapFile.Get(Form("h1HotPixelListPl%d",aPlaneNumber+1));
    if(h1HotPixelList != NULL) { // if occupancy h1 list exists
      //h1HotPixelList->SetDirectory(0);

      for(Int_t iindex=0;iindex<h1HotPixelList->GetXaxis()->GetNbins();iindex++) {
        double fake_rate = h1HotPixelList->GetBinContent(iindex+1)/100.0;

//        cout << "Testing index " << iindex << "in list with " << fake_rate << " against " << _FakeRateCutList[0] << endl;
        if(fake_rate >= _FakeRateCutList[0]) pPlaneParameter[aPlaneNumber].HotPixelList_index.push_back(iindex);
      }

//      FakeRateMapFile.Close();

//      return;

    } // end if occupancy h1 list exists
  } // end if FakeRateCut list is == 1

  FakeRateMapFile.Close();

  return;

}
//______________________________________________________________________________
//
void DSetup::PrintListOfHotPixelsToMask(int aPlaneNumber)
{

  if(pPlaneParameter[aPlaneNumber].HotPixelList_lin.size() > 0) {
    cout << endl;
    cout << "Hot pixels for Plane " << aPlaneNumber+1 << ": The " << pPlaneParameter[aPlaneNumber].HotPixelList_lin.size()
    << " hottest pixels (from 2D map) are masked"
    << endl;
    if(DSetupDebug && pPlaneParameter[aPlaneNumber].HotPixelList_lin.size() > 0) {
      //if(pPlaneParameter[aPlaneNumber].HotPixelList_lin.size() > 0) {
      cout << "      line    column" << endl;
      for(int i_hot = 0;i_hot<int(pPlaneParameter[aPlaneNumber].HotPixelList_lin.size());i_hot++) {
        cout << i_hot+1 << "      "
        << pPlaneParameter[aPlaneNumber].HotPixelList_lin[i_hot] << "        "
        << pPlaneParameter[aPlaneNumber].HotPixelList_col[i_hot]
        << endl;
      }
      cout << endl;
    }
  }

  if(pPlaneParameter[aPlaneNumber].HotPixelList_index.size() > 0) {
    cout << endl;
    cout << "  --> same list ordered by 1D index with " << pPlaneParameter[aPlaneNumber].HotPixelList_index.size() << " pixels." << endl;
  }

  return;

}
//______________________________________________________________________________
//
DSetup::~DSetup()
{

  delete pPlaneParameter;

  delete pLadderParameter;

  delete pAcqModuleParameter;

}
//______________________________________________________________________________
//
