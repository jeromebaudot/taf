*********************************************************************************************************
# NOTES ON TAF code  

- basic recipes & doc
  ( releases -> moved to independant file releases.txt )
- examples
- comments on features
- to do list for developers
*********************************************************************************************************
Contact: baudot@in2p3.fr, auguste.besson@iphc.cnrs.fr, ziad.elbitar@iphc.cnrs.fr

Web page: http://www.iphc.cnrs.fr/TAF.html

*********************************************************************************************************
## BASIC RECIPES & DOC
*********************************************************************************************************

# Before doing anything, set the configuration:
- cd to the taf directory
- source Scripts/thistaf.sh (no configuration needed, except if asked during sourcing)
      (note the old method: source Script/TAF-config is not available anymore)

# Documentation is avalibale under the 'doc" directory
- taf_shortDoc.pdf : provides basic help on TAF
- maf_doc.pdf      : provides a detailed description of a former but similar software named MAF
- ClassIndex.html  : provides web-based browsing of classes
  -> You may generate the HTML doc yoursel using the Scripts/makeHTML.doc scripts, read instructions in the script source file.

# To run the software: (faster methods just below)
- simply issue the command: TAF
- first thing to do in TAF:
MimosaAnalysis *gTAF = new MimosaAnalysis() // optional, depends on your system
gTAF->InitSession([runNumber])
gTAF->SetDebug([level])
gTAF->Help();

# Note that TAF accepts arguments that can speed up the initialisation step:
- run TAF --help to know more
- run TAF on a specific run, use
    TAF -run runNumber
- run TAF on a specific run but a generic config file, use
    TAF -run runNumber -cfg ./myConfigDir/myConfigFile.cfg
- run TAF on a specific run but a generic config file and specify the data-path, use
    TAF -run runNumber -cfg ./myConfigDir/myConfigFile.cfg -datapath /mydatapath
- run TAF on a specific run but a generic config file and specific datafile, use
  (note the datapath is still taken from the config file unless you use -datapath option)
    TAF -run runNumber -cfg ./myConfigDir/myConfigFile.cfg -datafile myfile
- run TAF analysis with a specific DSFfile, use
    TAF -run runNumber -dsffile myDSFfile
- run TAF on a specific run and with config file in a specific directory, used
    TAF -run runNumber -cfgdir myConfigDir
- run TAF with any of the above options and open directly the GUI, use (for instance)
    TAF -run runNumber -gui
- run TAF with any of the above options and set a debug level
    TAF -run runNumber -debug 2

# To compile the software:
- Prefered method, use the command (from anywhere): maketaf
- Alternative method 1) (equivalent to maketaf script)
> cd code; make
- Alternative method 2) only since v2.6.1 <== deprecated, not checked since ages!!!
> root Scripts/compiltaf.C

To copy the software
- use the command: . Script/tarTAF
- it creates a ttaf_new.tar.gzip with all what you need to export elsewhere


*********************************************************************************************************
## EXAMPLES
*********************************************************************************************************

A number of configuration files are provided as examples inside the config_TEST directory. They shall work anywhere,
provided you have the raw data, contact us to get a sample of them. In order to test them you just need to copy the
corresponding file inside your config directory, and change the DataPath variable to the Path where the raw data is
located

- RUN 1040, June 2014
 2 MIMOSA 28, 1 MIMOSA 22THRb, 2 MIMOSA 18, 2 MIMOSA 28 at LNFrascati - Beam Test Facility

- RUN 34539, August 2013
 6 MIMOSA 26 (digital output) with one MIMOSA 34

- RUN 32824, August 2012
 6 MIMOSA 26 (digital output) with one MIMOSA 32 (binary output) and
  the synchronization of two BoardReaders.

- RUN 26000, February 2012
 2 MIMOSA 26 sensors data taken in lab, with no source, and with power pulsing,
 frames with trigger information indicates when the power was switched ON/OFF.

- RUN 226031, November 2011
 8 MIMOSA 26 sensors, 4 as reference planes in the telescope, 4 in the PLUME ladder (2 per side),
 SPS beam 120 GeV pions, normal incidence but beam located in between two PLUME sensors

- RUN 28510, July 2011
 6 MIMOSA 28 (ULTIMATE) sensors in a telescope (4 as references and 2 as DUT), SPS beam 120 GeV pions, normal incidence
 acquisition = new PXIexpress system where only triggered frames are written

- RUN 28905, July 2011
 6 MIMOSA 28 (ULTIMATE) sensors in a telescope but data taken without beam,
 this run is to estimate the fake hit rate
  acquisition = new PXIexpress system where only triggered frames are written

- RUN 26519, August 2010
 6 MIMOSA 26 sensors in a telescope (4 as references and 2 as DUT), SPS beam 120 GeV pions, normal incidence
 acquisition = old PXI system where all frames are written

- RUN 22704,
 1 MIMOSA 22 AHR sensor in a telescope of 8 strip planes, SPS beam 120 GeV pions, normal incidence
 acquisition = Imager-USB system

- RUN 18400,
 1 MIMOSA 18 sensor exposed to a Rutenium source
 acquisition = TNT board

- RUN 18007,
 1 MIMOSA 18 sensor exposed to a Rutenium source
 acquisition = IMAGER board

 - RUN 226020
   PLUME Beam test @ CERN-SPS on Oct. 2011: 8 Mi26 chips.
   acquisition PXIe

 - RUN 28822 and 28848
   SALAT test beam @ DESY on Feb 2014: 8 Mi28 chips
   acquisition PXIe

 - RUN 18538
   Mi18 Beam test @ CERN-SPS on Sep. 2007: Telescope with 4 planes of Mi18 chips.
   acquisition IMG

 - RUN 35507 and 35604
   FSBB-M0 test beam @ CERN-SPS (Oct. 2014) and DESY (Mar. 2015): Telescope of 6 planes of FSBB-M0 chips.
   Acquisition PIXe

 - RUN 35705 and 35604
   FSBB-M0-bis test beam @ DESY (Jun. 2015) and CERN-SPS (Oct. 2015): Telescope of 6 planes of FSBB-M0-bis chips.
   Acquisition PIXe



*********************************************************************************************************
## COMMENTS ON FEATURES
*********************************************************************************************************

- compilation
  * PXIeBoardReader.cxx uses an #include to define which pxi_daq library version is used

- Variables which are hardcoded

  * A number of options in IMGBoardReader.

  * In the method find_tracks() of the class DTracker.cxx, you choose by commenting
    the method to extrapolate the track to the next plane.
    --> not true anymore since v2.8.1

  * In DTrack.cxx, method Analyze, you choose to use a vertex point
    to start your track or not by commenting/uncommenting the proper lines.
    --> not true anymore since v2.8.1

  * DAcq.cxx sets the "vetoOverflow" flag for the PXIeBoardReader objects (line 177)

  * Each DAQ Board type is associated with a unique integer id in DAcq.cxx
    1 = IMG, 2 = ?, 3 = TNT, 4 = PXI, 5 = PXIe, 6 = GIG

  * Max number of modules to be associated to a plane is 16 (from release 2.0, 4 in 1.0). This is hardcoded in DSetup.h and can be changed if needed.

  * max number of modules for a given module type is 16
    in DSetup.h for the declaration of *DeviceDataFile[16]

  * Max number of submatrix (in total, whatever the number of DUT) is 10, hardcoded in DSetup.h
    First subamtrix number is 0.

  * limits on shift and tilt change to stop alignment in MAlign.cxx

  * maximum size of the IndexOfGoodHitsInEvent array in MCommands.cxx

  * minimum noise to consider the value is reasonable and hence compute a S/N for a pixel
    min value is 0.5 in DHit::Analyse and in DPixel::GetPulseHeightToNoise

*********************************************************************************************************
## TO DO LIST FOR DEVELOPERS
*********************************************************************************************************

* HIGH PRIORITY

- Propose a display of pixel position (based on (col,lin)<->(u,v) methods)

- Allows Mrax class to work on any geometry

- Manage properly and uniformely the MimosaType variable everywhere:
   set it once for all

- Set a method to sparsify the data for analogue output with no noise recomputation

- Reshape MRaw to have histos booked and filled separately from the display
  => might be useless since the introduction of the MRax class

- Reshape the classes DHit and DTrack to allow easy switching between different
   clustering and tracking algorithm.
  => Currently managed with flags in the config file to switch to different find_xxx methods (JB July 2013)

- Insure all variables are stable against 64bits CPU (especially in BoardReader classes)
  => Currently work with a patch...(JB+LC, May 2012)

- Provide a Digitizer that simulates the pixel response to a list of energy
  depositions read from an input file

- Add some statistics computation on the noise distribution

* LOW PRIORITY

- Check in MimosaPro that hits are not selected several times by different tracks

- In each output file, store a canvas with text about the configuration and cuts

- Add protection if DTDIR not defined

- Provide an estimation of the telescope resolution from geometry and available plane resolution

- Management of cuts (both for DSFProduction and for final analyses) especially for bookkeeping

- Create a GetCuts method in MCommands managing one or two planes

- Better management of readout types and mimosa types in DPLane::Update (it is a mess currently)

- Take care of sensor aspect ratio in MRaw.cxx method for display

- In the align tracking procedure, ask the user to store or not the new parameters,
   could be usefull in the case where the fit procedure went wrong

- Improve the MimosaFakerate command to get rate for each submatrix and per column and line

- change the computation of the geoMatrix limit
	from position in microns
	to index (row and column) of pixels

- for eta algorithm in tracker switch from histograms to arrays (like done in MimosaAnalysis)

- include AlignStatus of tracker in config file (maybe useless?)

* DONE

- Allow different event building mode for the DUT and the Ref planes in PXIeBoardReader,
   for instance, DUT event may use one additional frame compared to REF event
  => done, JB 2014

- Introduce a digitization emulation algorithm in DPlane
  => done, JB August 2013

- Add fit of residual (DV/DU) vs (V/U) in Minuit Alignment procedure
  => done, JB August 2013

- Change the reading method for the configuration file in DSetup
  to allow for new variables BUT keeping the compatibility with old files
  => done, JB January 2013

- include the precision alignment of DUT in the method AlignTracker
  => done, LC September 2012

- Cope with strip-telescope data
  => done, JB August 2012

- Provide a ImagerBoardReader to decode rawdata from the IPHC Imager-USB board
  => done, JB+SS August 2012

- Generate a printout method for MCommand.cxx (#splitline{top}{bottom})
  => mostly done, SS+MG 2012/?

- CrossCheck coherence between cuts ClusterLimitU and MaxNStrips
  => done, JB 2012/05/09

- Allow usage of run number with any digits (especially for directory naming)
  => done, JB+VR 2012/05/04

- Provide a MCBoardReader which would read a GEANT4 output file, digitize the energy deposition and then provide a list of pixels as any other BoardReader
  => done JB+LC, 2012/04/24 (GIGBoardReader)

- Allow display of position in tracker coordinates and not only in plane coord. for MRaw Display functions
  => done JB+VR, 2012/04/02

- Move the Hot Hit Map mechanism used in MimosaPro method as a generic methods usable by others MCommands.cxx methods.
  => done JB, 2011/11/23

- Provide a method to deal with clusters containing several occurenres of the same pixel. This happens when several frames are merged to produce a single physical event.
  => done JB+SS, 2011/11/07

- Modify DSFProduction to choose the level of information actually stored (hits of all plane or not, tracks...)
  => done JB 2011/07/21

- Get rid of all global/external variables for compilation under WINDOWS
  => done JB 2011/07/21

- Manage efficiency computation in MCommands.cxx methods as separate and general methods in the MAnalysis.cxx class. Also include efficiency computation dependant on X and Y.
  => done MG+JB 2011/11/04

- Make the generation of directories compatible with any system (add Windows!)
  => done JB 2011/07/18

- Use the Root Documentation generation for the classes.
  => done JB 2011/07/20

- Allow to increment above 9 files for the TNT input files
  => done JB 2010/10/06

- Add new word case in TNT word=50000000 for overflow
  => done JB 2010/10/06

- Include statistics printout for TNTBoardReader
  => done JB 2010/09/27

- Re-shuffle MimosaPro function for clarity, all histogram filling should be externalized
  => done JB 2010/07/29

- mechanism to compute noise from TNT + to retrieve it during data mining
  => done YV 2009/?

- implement histograms for statistics on data mining during the DSFProduction
  => done JB 2009/09/10

- have a mechanism to change the debug level after InitSession
  => done JB 2009/05/12

- read the rawdata filename from config file
  => done JB 2009/05/25

- setup a mechanism to reject event when too many hits in a plane
  => done  JB 2009/08/26

- allow best track over all possible ones when several hits
  => done JB 2009/05/21

- upgrade to new TNT format of May 2009
  => done RDM 2009/05/20

- include harcoded parameters in config file (# events for alignment, max hit-track distance...)
  => done JB 2009/05/25

- Use the globalTool function info, error instead of simple cout or printf
  => probably useless

- Check that all modules declared are actually associated to a plane, otherwise STOP.
  => done JB 2009/05/25

- If possible get rid of the MPara class, everything should come from the config file
  => done JB 2009/08/26
