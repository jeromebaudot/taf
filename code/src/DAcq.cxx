// @(#)maf/dtools:$Name:  $:$Id: DAcq.cxx,v.2 2005/10/02 18:03:46 sha Exp $
// Author: Dirk Meier   97/12/06
// Completly rewritten: JB 2008/10/13
// Last modified: JB 2009/05/06
// Last modified: JB 2009/05/22
// Last modified: JB 2009/05/25
// Last modified: JB 2009/08/20
// Last modified: RDM 2009/08/25
// Last modified: JB 2009/09/09
// Last modified: JB 2009/09/28 new TNT specs
// Last modified: JB 2010/06/16 and 07/07 get list of frames and triggers from BoardReaders
// Last modified: JB 2010/08/23 Allow multiple module types in the same acquisition
// Last modified: JB 2011/03/14 Declaration of PXIBoardReader to cope with DAQlib
// Last modified: JB 2011/06/19 Introduction of PXI & PXIe readers for backward compatibility
// Last modified: SS 2011/11/14 EventBuildingMode passed to PXIe
// Last modified: SS 2011/12/14 PrintStatistics
// Last modified: MG 2012/03/14 NextEvent
// Last modified: JB 2012/05/04 NextEvent
// Last modified: JB 2012/07/10 NextEvent
// Last modified: JB 2012/07/22 Introduction of IMGBoardEvent
// Last modified: SS 2012/08/10 Introduction of output bits for NextEvent
// Last modified: JB 2012/08/17 Counting missed events
// Last modified: JB 2013/08/14 DAcq, GetMatchingPlaneAndShift, NextEvent
// Last modified: LC 2013/08/20 GetSynchroInfo
// Last Modified: BH 2013/08/20 warning corrections from clang
// Last Modified: JB 2014/03/13 DAcq
// Last Modified: VR 2014/03/20 configuration of PXI DAQ LIB VERSION centralized in file pxi_daq_lib_config.h
// Last Modified: JB 2014/05/13-15 new BoardReader added VME, ALIM22, DecoderM18
// Last Modified: JB 2014/08/26 replace DecoderM18_fb with DecoderM18 and add param for ALIM22
// Last Modified: LC 2014/12/08 Adding MonteCarlo Parameters.
// Last Modified: LC 2014/12/15 Class DMonteCarlo Removed. Now MC info in DPixel Class. DPixel contain a vector of MC Info.
// Last Modified: LC 2014/12/15 Memory reduction if no MC + New infos easily added.
// Last Modified: LC 2014/12/15 New parameter in the config file : HitMonteCarlo If GIGBordReader HitMonteCarlo:1=enable :0=disable
// Last Modified: JB 2014/12/16 DAcq, NextEvent, PrintStatistics
// Last Modified: JB 2015/03/27 NextEvent and ___Synchro, for DecoderM18 synchro
// Last Modified: JB 2015/05/25 NextEvent
// Last Modified: AP 2016/04/15 Including MCBoardReader to read MC n-tuple
// Last Modified: AP 2016/04/21 Added an Object DEventMC which will serve as a MCInfoHolder, with the list of particles, hits and pixels (both physical and from noise)
// Last Modified: AP,JB 2016/09/20 DAcq for new param in IMGBoardReader
// Last Modified: JB 2017/11/20 DAcq
// Last Modified: JB 2018/02/11 InitTimeRefInfo -> updated 2018/03/21
// Last Modified: JB 2020/02/17 Introduction of vetoPixdl for VMEBoardreader
// Last Modofies: JB 2021/05/01 Adding BoardReaderMIMOSIS

//*-- Modified :  IG
//*-- Copyright:  RD42
//*-- Author   :  Dirk Meier   97/12/06
//*KEEP,CopyRight.
/************************************************************************
* Copyright(c) 1997, Diamond.Detector@cern.ch, DiamondTracking.
***********************************************************************/
//*KEND.

#include "DAcq.h"

#define MIMO_DAQ_LIB_VERSION_1_1 // (MIMOSIS1)
/*
#Include "pxi_daq_lib_config.h" // PXI DAQ Library version configuration is made in this file
#ifdef PXI_DAQ_LIB_VERSION_1_1
  #include "pxi_daq_lib_v.1.1/sync_index_rec.typ"
  #include "pxi_daq_lib_v.1.1/sync_index_rec.c"
#endif
#ifdef PXI_DAQ_LIB_VERSION_1_2
  #include "pxi_daq_lib_v.1.2/sync_index_rec.typ"
  #include "pxi_daq_lib_v.1.2/sync_index_rec.c"
#endif
#ifdef PXI_DAQ_LIB_VERSION_2_1
  #include "pxi_daq_lib_v.2.1/sync_index_rec.typ"
  #include "pxi_daq_lib_v.2.1/sync_index_rec.c"
#endif
#ifdef PXI_DAQ_LIB_VERSION_3_1
  #include "pxi_daq_lib_v.3.1/sync_index_rec.typ"
  #include "pxi_daq_lib_v.3.1/sync_index_rec.c"
#endif
 */
#ifdef MIMO_DAQ_LIB_VERSION_1_1
  #include "mimo_daq_lib/sync_index_rec.typ"
  #include "mimo_daq_lib/sync_index_rec.c"
#endif
    /////////////////////////////////////////////////////////////////////////////
    // Class Description of DAcq (DataAcquisition)                             //
    //                                                                         //
    // + construct the DataAcquisition with its devices, inputs and channels   //
    // + prepare buffers for data                                              //
    // + translation of binary data to variables                               //
    //                                                                         //
    /////////////////////////////////////////////////////////////////////////////


  ClassImp(DAcq) // Description of Single Detector DAcq

//______________________________________________________________________________
//
  DAcq::DAcq()
{
  // Default DAcq ctor.
}

//______________________________________________________________________________
//
DAcq::DAcq(DSetup& c)
{
  // Constructs the Data Acquisition with value
  // read from configuration file
  //
  // JB, 2008/10/14
  // modified JB 2009/05/06
  // modified JB 2009/08/20 to add PXI boards
  // modified JB 2010/07/07 to init NULL pointers for list of triggers
  // modified JB 2010/08/23 to allow new triggerMode flag and use of moduleType/10
  // modified JB 2011/06/19 Introduction of PXI & PXIe readers for backward compatibility
  // modified JB 2011/06/28 passage of #rows to PXIe
  // modified SS 2011/11/14 passage of eventBuildingMode for PXIe
  // modified MG 2012/03/14 init ListOfLineOverflow
  // modified JB 2012/04/25 to add GIG boards (GEANT4-DIGMaps)
  // Modified JB 2012/07/22 to add IMG boards (analog Imager)
  // Modified JB 2012/08/10 to count missed events
  // Modified JB 2012/08/18 to pass eventsPerFile to IMGBoardReader
  // Modified JB 2012/08/19 to pass FileHeaderLine to IMGBoardReader
  // Modified JB 2013/06/22 passage of eventBuildingBoardMode
  // Modified JB 2013/08/14,15 manage input offset to allow severall planes on same input
  // Modified JB 2014/03/13 forward DUT status to Reader
  // Modified JB 2014/05/13 to add VME boards (Eleuterio's format)
  // Modified JB 2014/12/16 counter for NotOK events added
  // Modified LC 2014/12/15 Class DMonteCarlo removed --> included in DPixel
  // Modified JB 2015/05/26 Usage of timestamp added (fUseTimestamp)
  // Modified JB 2016/09/20 Transmission of Nframes and NColumns (done by AP) to IMGBoardReader
  // Modified JB 2017/11/20 allow Zero Suppression to IMGBoardReader
  // Modified JB 2018/02/11 Test if init of TimeRefInfo is neeeded

  cout << endl << " -*-*- DAcq Constructor -*-*- " << endl;

  fc            = &c;
  fDebugAcq     = fc->GetDebug();
  fRunNumber    = fc->GetRunPar().Number;
  fEventNumber  = 0;
  fRealEventNumber= 0;
  fEventsMissed = 0; // JB 2012/08/17
  fEventsDataNotOK = 0;  // JB 2014/12/16
  fEventsModuleNotOK = 0;// JB 2014/12/16
  Int_t          mdt, mdl;
  fModuleTypes  = fc->GetAcqPar().ModuleTypes;
  fMaxSegments = 50; // JB 2013/08/14

  // Timing information, 0 is the default which could be updated from data
  // JB 2018/02/12
  fEventReferenceTime = 0;
  fEventTime = 0;

  ListOfTriggers = NULL; // JB 2010/07/07
  ListOfFrames = NULL;
  ListOfTimestamps = NULL;
  ListOfLineOverflow = NULL; // MG 2012/02/15

  fIsMCBoardReader = false;

  Bool_t initOK = kTRUE; // we start with proper init...

  cout << " Dacq building events" << ( (fc->GetAcqPar().TriggerMode==0)?" without":" with" ) << " trigger." << endl;
  if (fDebugAcq) cout <<"  DAcq: ModuleTypes " << fModuleTypes << endl;

  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // Count the total number of modules
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // JB 2010/08/23
  Int_t totalNmodules = 0;
  for (mdt = 1; mdt <= fModuleTypes; mdt++){ // loop on module types
    totalNmodules += fc->GetModulePar(mdt).Devices;
  }
  if(fDebugAcq) cout <<"  DAcq: total modules " << totalNmodules << endl;

  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // Create the array structures to store the data
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-

  // ==========================
  //fRawData = new Int_t**[fModuleTypes]; // Pointer to raw data for each module type, only for readout=2
  fMatchingPlane = new Int_t***[totalNmodules];
  fIndexShift = new Int_t***[totalNmodules];
  fInputSegments = new vector<int>**[totalNmodules];
  fListOfPixels = new vector<DPixel*>[fc->GetTrackerPar().Planes];
  //fListOfMonteCarlo = new vector<DMonteCarlo*>[fc->GetTrackerPar().Planes]; // LC 2014/12/15 :: DMonteCarlo included in DPixel
  fLineOverflowN = new Int_t[10]; //MG 2012/02/15 : be carefull if the number of plane is bigger than 10 !
  fUseTimestamp = new Bool_t*[totalNmodules]; // JB 2015/05/26
  // ==========================


  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // Create the pointers to each module types
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // JB 2010/08/23
  // JB 2012/04/25, GIGBoard added
  // JB 2014/05/13, VMEBoard added

  // Create as many pointer as modules for each type even if some of them will not be used
  fTNT   = new TNTBoardReader*[totalNmodules];
  fPXI   = new PXIBoardReader*[totalNmodules];
   // Ziad EL BITAR 2021/05/25 disregard PXIeModule when using MimosisBoardReader
  //fPXIe  = new PXIeBoardReader*[totalNmodules];
  fGIG   = new GIGBoardReader*[totalNmodules];
  fIMG   = new IMGBoardReader*[totalNmodules];
  fVME   = new VMEBoardReader*[totalNmodules];
  fMC    = new MCBoardReader*[totalNmodules];
  fALI22 = new AliMIMOSA22RawStreamVASingle*[totalNmodules];
  fM18   = new DecoderM18*[totalNmodules];
  fGeant = new DecoderGeant*[totalNmodules];
  fIHEP  = new BoardReaderIHEP*[totalNmodules];
  fMSIS  = new BoardReaderMIMOSIS*[totalNmodules];

  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // Loop on each acquisition module type to set its properties
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // this include completing the array structure for the data

  Char_t aFileName[500], aBaseName[100];
  Int_t iModule=0; // module index, from 0 to totalNmodules

  //====================
  // Check for synchronization, JB 2012/07/22
  Bool_t synchroNeeded = false;
  if( fModuleTypes>1 ) synchroNeeded = true;

  for (mdt = 1; mdt <= fModuleTypes; mdt++){ // loop on module types

    if(fDebugAcq) cout <<"  DAcq: Starting building modules for type nb " << mdt << endl;

    // ==========================
    //fRawData[mdt-1] = new Int_t*[fc->GetModulePar(mdt).Devices]; // Pointer to raw data for this module type, used for not zero-suppressed data
      fMatchingPlane[mdt-1] = new Int_t**[fc->GetModulePar(mdt).Devices];
      fIndexShift[mdt-1] = new Int_t**[fc->GetModulePar(mdt).Devices];
      fInputSegments[mdt-1] = new vector<int>*[fc->GetModulePar(mdt).Devices];
      fUseTimestamp[mdt-1] = new Bool_t[fc->GetModulePar(mdt).Devices]; // JB 2015/05/26
    // ==========================

    for (mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type

      if (fDebugAcq)  cout << "    DAcq: building module " << mdl << " of type " << fc->GetModulePar(mdt).Type << endl;

      fMatchingPlane[mdt-1][mdl-1] = new Int_t*[fc->GetModulePar(mdt).Inputs];
      fIndexShift[mdt-1][mdl-1] = new Int_t*[fc->GetModulePar(mdt).Inputs];
      fInputSegments[mdt-1][mdl-1] = new vector<int>[fc->GetModulePar(mdt).Inputs];
      for ( Int_t iInp=0; iInp<fc->GetModulePar(mdt).Inputs; iInp++) {
        fMatchingPlane[mdt-1][mdl-1][iInp] = new Int_t[fMaxSegments];
        fIndexShift[mdt-1][mdl-1][iInp] = new Int_t[fMaxSegments];
      }


      // The real module type is defined by Type/10
      // JB 2010/08/23
      switch ( (fc->GetModulePar(mdt).Type)/10 ) {
        
        // -+-+- IMG modules
        case 1:

        fUseTimestamp[mdt-1][mdl-1] = kFALSE; // JB 2015/05/26
        //fIMG[iModule] = new IMGBoardReader( iModule, fc->GetModulePar(mdt).Inputs, fc->GetModulePar(mdt).Channels, fc->GetAcqPar().EventBufferSize, fc->GetRunPar().EventsInFile, fc->GetAcqPar().FileHeaderLine, fc->GetModulePar(mdt).Bits, fc->GetModulePar(mdt).SigBits, fc->GetAcqPar().BinaryCoding, fc->GetAcqPar().TriggerMode, fc->GetModulePar(mdt).EventBuildingBoardMode /*(fc->GetModulePar(mdt).Type)%10*/);
        fIMG[iModule] = new IMGBoardReader( iModule,
          fc->GetModulePar(mdt).Inputs,
          fc->GetModulePar(mdt).Channels,
          fc->GetAcqPar().EventBufferSize,
          fc->GetRunPar().EventsInFile,
          fc->GetAcqPar().FileHeaderLine,
          fc->GetAcqPar().EventTrailerSize, // trailer indicated, JB 2018/03/19
          fc->GetModulePar(mdt).Bits,
          fc->GetModulePar(mdt).SigBits,
          fc->GetAcqPar().BinaryCoding,
          fc->GetAcqPar().TriggerMode,
          fc->GetModulePar(mdt).EventBuildingBoardMode /*(fc->GetModulePar(mdt).Type)%10*/,
          fc->GetModulePar(mdt).NColumns,
          fc->GetModulePar(mdt).NMultiFrames);
        if( fc->GetModulePar(mdt).IfZeroSuppress > 0) { // JB 2017/11/20
          fIMG[iModule]->SetZeroSuppression( fc->GetModulePar(mdt).ThresholdZero);
        }
        fIMG[iModule]->SetDebugLevel( fDebugAcq);
        //cout << "Setting number of columns to " << fc->GetPlanePar(1).Strips(0) << endl;
        if( fc->GetModulePar(mdt).DeviceDataFile[mdl-1]!=NULL ) {
          if( strcmp(fc->GetModulePar(mdt).DeviceDataFile[mdl-1], "") ) {
            sprintf( aBaseName, "%s", fc->GetModulePar(mdt).DeviceDataFile[mdl-1]);
          } else {
            sprintf( aBaseName, "RUN_%d_", fRunNumber);
          }
        } else {
          sprintf( aBaseName, "RUN_%d_", fRunNumber);
        }
        sprintf( aFileName, "%s/%s", fc->GetRunPar().DataPath, aBaseName);
        if( !( fIMG[iModule]->AddFileList( aFileName, fc->GetRunPar().StartIndex, fc->GetRunPar().EndIndex, fc->GetRunPar().Extension ) ) ) {
          sprintf( aFileName, "%s/%d/%s", fc->GetRunPar().DataPath, fRunNumber, aBaseName);
          if( !( fIMG[iModule]->AddFileList( aFileName, fc->GetRunPar().StartIndex, fc->GetRunPar().EndIndex, fc->GetRunPar().Extension ) ) ) {
            sprintf( aBaseName, "%d", fRunNumber);
            sprintf( aFileName, "%s/%s", fc->GetRunPar().DataPath, aBaseName);
            if( !( fIMG[iModule]->AddFileList( aFileName, fc->GetRunPar().StartIndex, fc->GetRunPar().EndIndex, fc->GetRunPar().Extension ) ) ) {
              sprintf( aFileName, "%s/%d/%s", fc->GetRunPar().DataPath, fRunNumber, aBaseName);
              initOK &= fIMG[iModule]->AddFileList( aFileName, fc->GetRunPar().StartIndex, fc->GetRunPar().EndIndex, fc->GetRunPar().Extension );
            } else {
              initOK &= kTRUE;
            }
          } else {
            initOK &= kTRUE;
          }
        } else {
          initOK &= kTRUE;
        }
        break;


        // -+-+- TNT modules
        case 3:

        fUseTimestamp[mdt-1][mdl-1] = kTRUE; // JB 2015/05/26
        sprintf( aFileName, "%s/%s", fc->GetRunPar().DataPath, fc->GetModulePar(mdt).DeviceDataFile[mdl-1]); //RDM 140509, JB 2009/05/25
        fTNT[iModule] = new TNTBoardReader( iModule, fc->GetAcqPar().EventBufferSize, fc->GetAcqPar().BinaryCoding, fc->GetTrackerPar().TimeLimit, fc->GetAcqPar().TriggerMode, fc->GetModulePar(mdt).Bits[0], fc->GetModulePar(mdt).SigBits[0]);
        fTNT[iModule]->SetDebugLevel( fDebugAcq);
        initOK &= fTNT[iModule]->AddFileList( aFileName, fc->GetRunPar().StartIndex, fc->GetRunPar().EndIndex, fc->GetRunPar().Extension );
        break;


        // -+-+- PXI modules
        // JB 2009/08/20
        case 4:

        fUseTimestamp[mdt-1][mdl-1] = kTRUE; // JB 2015/05/26
        if(fc->GetTrackerPar().TimeLimit<0) fc->GetTrackerPar().TimeLimit = 2;
        sprintf( aFileName, "%s/%scnf.bin", fc->GetRunPar().DataPath, fc->GetModulePar(mdt).DeviceDataFile[mdl-1]);
        fPXI[iModule] = new PXIBoardReader( iModule, aFileName, fc->GetAcqPar().TriggerMode, fc->GetAcqPar().BinaryCoding);
        fPXI[iModule]->SetDebugLevel( fDebugAcq);
        sprintf( aFileName, "%s/%s", fc->GetRunPar().DataPath, fc->GetModulePar(mdt).DeviceDataFile[mdl-1]); //RDM 140509, JB 2009/05/25
        fPXI[iModule]->AddFileList( aFileName, fc->GetRunPar().StartIndex, fc->GetRunPar().EndIndex, fc->GetRunPar().Extension );
        break;


        // -+-+- PXIe modules
        // JB 2011/03/14
        // Ziad EL BITAR 2021/05/25 disregard PXIeModule when using MimosisBoardReader
              /*
        case 5:

        fUseTimestamp[mdt-1][mdl-1] = kTRUE; // JB 2015/05/26
        if(fc->GetTrackerPar().TimeLimit<0) fc->GetTrackerPar().TimeLimit = 2;
        sprintf( aFileName, "%s/", fc->GetRunPar().DataPath);
        fPXIe[iModule] = new PXIeBoardReader(iModule,
          aFileName,
          fRunNumber,
          fc->GetAcqPar().TriggerMode,
          fc->GetModulePar(mdt).EventBuildingBoardMode /*fc->GetAcqPar().EventBuildingMode*/
          /*    ,0,
          fc->GetAcqPar().BinaryCoding,
          fc->GetPlanePar(1).Strips(1),
          fc->GetPlanePar(1).MimosaType); //SS 2011.11.14 - EventBuildingMode can be loaded externally
              cout << "Ziad : fileName : " << aFileName << endl;
              cout << "Ziad : fRunNumber " << fRunNumber << endl;
              cout << "Ziad : fc->GetPlanePar(1).Strips(1) " << fc->GetPlanePar(1).Strips(1) << endl;
              cout << "Ziad : fc->GetAcqPar().BinaryCoding " <<  fc->GetAcqPar().BinaryCoding << endl;
                           
        fPXIe[iModule]->SetDebugLevel( fDebugAcq);
        fPXIe[iModule]->SetVetoPixel( fc->GetRunPar().NoiseRun);
        fPXIe[iModule]->SetFlag(((fc->GetModulePar(mdt).Type)%10));
        fPXIe[iModule]->SetNumberOfColumns(fc->GetPlanePar(1).Strips(0));

        break;

*/
        // -+-+- GIG modules (GEANT4 - DIGMaps)
        // JB 2011/03/14
        case 6:

        fUseTimestamp[mdt-1][mdl-1] = kFALSE; // JB 2015/05/26
        fGIG[iModule] = new GIGBoardReader( iModule, fc->GetTrackerPar().Planes);
        sprintf( aFileName, "%s", fc->GetRunPar().DataPath);
        fGIG[iModule]->AddFile( aFileName );
        fGIG[iModule]->SetDebugLevel( fDebugAcq);
        fIfMonteCarlo = fc->GetTrackerPar().HitMonteCarlo;
        break;


        // -+-+- VME modules
        // JB 2014/05/13
        case 7:

        fUseTimestamp[mdt-1][mdl-1] = kFALSE; // JB 2015/05/26
        sprintf( aFileName, "%s/", fc->GetRunPar().DataPath);
        fVME[iModule] = new VMEBoardReader( iModule, fc->GetRunPar().DataPath, fc->GetModulePar(mdt).DeviceDataFile[mdl-1], fc->GetRunPar().Extension, fRunNumber, fc->GetModulePar(mdt).Inputs, fc->GetPlanePar(1).Strips(1));
        fVME[iModule]->SetDebugLevel( fDebugAcq);
        fVME[iModule]->SetVetoPixel( fc->GetRunPar().NoiseRun);
        break;


        // -+-+- ALI modules for M-22
        // JB 2014/05/14
        case 8:

        fUseTimestamp[mdt-1][mdl-1] = kTRUE; // JB 2015/05/26
        if(fc->GetTrackerPar().TimeLimit<0) fc->GetTrackerPar().TimeLimit = 1;
        sprintf( aFileName, "%s/FIFOdata_M22.dat", fc->GetRunPar().DataPath);
        fALI22[iModule] = new AliMIMOSA22RawStreamVASingle();
        fALI22[iModule]->SetInputFile(aFileName);
        fALI22[iModule]->SetNFrames(fc->GetModulePar(mdt).NbOfFramesPerChannel[mdl-1]); // JB 2014/08/26
        fALI22[iModule]->SetDebugLevel( fDebugAcq);
        break;


        // -+-+- Decoder MIMOSA-18 modules
        // JB 2014/05/15
        // JB+CB+PRL, 2015/03/24 for additional param of decoder
        case 9:

        fUseTimestamp[mdt-1][mdl-1] = kTRUE; // JB 2015/05/26
        sprintf( aFileName, "%s/", fc->GetRunPar().DataPath);
        fM18[iModule] = new DecoderM18(iModule, fRunNumber, mdl-1);
        // JB,CB,PLR 2015/03/24
        // PixelShift set to module-specific value, JB 2015/05/12
        //          fM18[iModule]->SetShift( fc->GetModulePar(mdt).PixelShift );
        if (fDebugAcq)printf( "  DAqcq:: setting kShift=%d for iModule=%d and mdl-1=%d\n", fc->GetModulePar(mdt).PixelShiftMod[mdl-1], iModule, mdl-1);
        fM18[iModule]->SetShift( fc->GetModulePar(mdt).PixelShiftMod[mdl-1] );
        fM18[iModule]->SetOffset( fc->GetModulePar(mdt).AmpOffset );
        fM18[iModule]->SetMulFactor( fc->GetModulePar(mdt).AmpFactor );
        fM18[iModule]->SetTrailer( fc->GetModulePar(mdt).Trailer );
        fM18[iModule]->SetInputFile(Form("%s/%s%d%s",fc->GetRunPar().DataPath, fc->GetModulePar(mdt).DeviceDataFile[mdl-1], mdl-1, fc->GetRunPar().Extension));
        fM18[iModule]->SetDebugLevel( fDebugAcq);
        break;


        // -+-+- GEANT modules

        case 10:
        fUseTimestamp[mdt-1][mdl-1] = kFALSE; // JB 2015/05/26
        sprintf( aFileName, "%s/", fc->GetRunPar().DataPath);
        fGeant[iModule] = new DecoderGeant(iModule, fRunNumber);
        fGeant[iModule]->SetInputFile(Form("%s/%s%d%s",fc->GetRunPar().DataPath, fc->GetModulePar(mdt).DeviceDataFile[mdl-1], mdl-1, fc->GetRunPar().Extension));
        fGeant[iModule]->SetDebugLevel( fDebugAcq);
        break;

        // -+-+- MC modules
        // AP 2016/04/15
        case 11:

        fUseTimestamp[mdt-1][mdl-1] = kFALSE; // AP 2016/04/15
        sprintf( aFileName, "%s/", fc->GetRunPar().DataPath);
        fMC[iModule] = new MCBoardReader( iModule,
          fRunNumber,
          TString(fc->GetRunPar().DataPath),
          TString(fc->GetModulePar(mdt).DeviceDataFile[mdl-1]),
          TString(fc->GetModulePar(mdt).MCTreeName.Data()),
          fc);
        fMC[iModule]->SetDebugLevel( fDebugAcq);

        //Holder for the MC truth information
        //Only initialized when reading MC data
        MCInfoHolder = new DEventMC();

        //Passing the pointer of the MCInfoHolder to be filled up
        //Inside MCBoardReader for each event
        fMC[iModule]->SetMCInforHolder(MCInfoHolder);

        fIsMCBoardReader = true;

        break;

        // -+-+- IHEP modules
        case 12:

        fUseTimestamp[mdt-1][mdl-1] = kFALSE;
        fIHEP[iModule] = new BoardReaderIHEP( iModule, fc->GetAcqPar().TriggerMode, fc->GetModulePar(mdt).EventBuildingBoardMode);
        if( fc->GetModulePar(mdt).DeviceDataFile[mdl-1]!=NULL ) {
          if( strcmp(fc->GetModulePar(mdt).DeviceDataFile[mdl-1], "") ) {
            sprintf( aFileName, "%s/%s.%s", fc->GetRunPar().DataPath, fc->GetModulePar(mdt).DeviceDataFile[mdl-1], fc->GetRunPar().Extension);
          } else {
            sprintf( aFileName, "%s/1.%s", fc->GetRunPar().DataPath, fc->GetRunPar().Extension);
          }
        } else {
          sprintf( aFileName, "%s/1.%s", fc->GetRunPar().DataPath, fc->GetRunPar().Extension);
        }
        initOK &= fIHEP[iModule]->AddFile( aFileName );
        break;


        // -+-+- MIMOSIS modules
        case 13:

        fUseTimestamp[mdt-1][mdl-1] = kFALSE;
        fMSIS[iModule] = new BoardReaderMIMOSIS( iModule,
	    fc->GetModulePar(mdt).Inputs,
            fc->GetAcqPar().TriggerMode,
            fc->GetModulePar(mdt).EventBuildingBoardMode,
            fc->GetAcqPar().EventHeaderSize,
            fc->GetAcqPar().EventTrailerSize,
            fc->GetAcqPar().BinaryCoding);
        fMSIS[iModule]->SetDebugLevel( fDebugAcq);
        fMSIS[iModule]->SetVetoPixel( fc->GetRunPar().NoiseRun);
        if( fc->GetModulePar(mdt).DeviceDataFile[mdl-1]!=NULL ) {
          if( strcmp(fc->GetModulePar(mdt).DeviceDataFile[mdl-1], "") ) {
            sprintf( aBaseName, "%s%d", fc->GetModulePar(mdt).DeviceDataFile[mdl-1], fRunNumber);
          } else {
            sprintf( aBaseName, "RUN_%d", fRunNumber);
          }
        } else {
          sprintf( aBaseName, "RUN_%d", fRunNumber);
        }
        sprintf( aFileName, "%s/%s", fc->GetRunPar().DataPath, aBaseName);
        if( !( fMSIS[iModule]->AddFileList( aFileName, fc->GetRunPar().StartIndex, fc->GetRunPar().EndIndex, fc->GetRunPar().Extension ) ) ) {
          sprintf( aFileName, "%s/%d/%s", fc->GetRunPar().DataPath, fRunNumber, aBaseName);
          if( !( fMSIS[iModule]->AddFileList( aFileName, fc->GetRunPar().StartIndex, fc->GetRunPar().EndIndex, fc->GetRunPar().Extension ) ) ) {
            sprintf( aBaseName, "%d", fRunNumber);
            sprintf( aFileName, "%s/%s", fc->GetRunPar().DataPath, aBaseName);
            if( !( fMSIS[iModule]->AddFileList( aFileName, fc->GetRunPar().StartIndex, fc->GetRunPar().EndIndex, fc->GetRunPar().Extension ) ) ) {
              sprintf( aFileName, "%s/%d/%s", fc->GetRunPar().DataPath, fRunNumber, aBaseName);
              initOK &= fMSIS[iModule]->AddFileList( aFileName, fc->GetRunPar().StartIndex, fc->GetRunPar().EndIndex, fc->GetRunPar().Extension );
            } else {
              initOK &= kTRUE;
            }
          } else {
            initOK &= kTRUE;
          }
        } else {
          initOK &= kTRUE;
        }
        break;


        // -+-+- Other modules
          default:
          cout << "WARNING: DAcq, unknown module type " << fc->GetModulePar(mdt).Type << "!" << endl;
      }; // end switch on module types

        cout << "  DAcq: << " << fc->GetModulePar(mdt).Name << " module " << mdl << " build with " << fc->GetModulePar(mdt).Inputs << " inputs of " << fc->GetModulePar(mdt).Channels[0] << " channels" << endl << endl;

        iModule++;

    } // end loop on each modules of this type

  } // end loop on module types



  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // Loop on each plane to match its input
  // -+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+--+-+-+-+-+-
  // We associate a module to a plane
  //   and store a potential shift between the channel index of the input and the real pixel index
  // Also, if plane is a DUT, the module might need to know it

  Int_t aModuleType, aModuleNumber, aInput, aChannel, aOffset, aSegment;
  //Int_t aChannelNumber;
    cout << " Ziad Number of Planes: " << fc->GetTrackerPar().Planes << endl;
  for ( Int_t iPlane = 1; iPlane <= fc->GetTrackerPar().Planes; iPlane++){ // loop on planes
    for ( Int_t iInp = 0; iInp < fc->GetPlanePar(iPlane).Inputs; iInp++) { // loop on inputs for this plane
      if (fc->GetPlanePar(iPlane).ModuleType[iInp] == 0) {
        continue;
      }
      if (fDebugAcq)  {
        cout << "  DAcq: Associating input " << iInp << " of plane " << iPlane << endl;
        cout << "Input " << iInp << " of type " << fc->GetPlanePar(iPlane).ModuleType[iInp] << " number " << fc->GetPlanePar(iPlane).ModuleNumber[iInp] << " input " << fc->GetPlanePar(iPlane).InputNumber[iInp] << " shift " << fc->GetPlanePar(iPlane).ChannelNumber[iInp] << " offset " << fc->GetPlanePar(iPlane).ChannelOffset[iInp] << " #channels " << fc->GetPlanePar(iPlane).Channels[iInp] << endl;
      }
      aModuleType    = fc->GetPlanePar(iPlane).ModuleType[iInp];
      aModuleNumber  = fc->GetPlanePar(iPlane).ModuleNumber[iInp];
      aInput         = fc->GetPlanePar(iPlane).InputNumber[iInp];
      aChannel       = fc->GetPlanePar(iPlane).ChannelNumber[iInp];
      aOffset        = fc->GetPlanePar(iPlane).ChannelOffset[iInp]; // JB 2013/08/15
      //aChannelNumber = fc->GetPlanePar(iPlane).Channels[iInp];
      aSegment       = fInputSegments[aModuleType-1][aModuleNumber-1][aInput-1].size();

      if (aSegment==fMaxSegments) {
        cout << " WARNING in DACQ constructor: input " << aInput << " of module type " << aModuleType << " nb " << aModuleNumber << " reach the maximum nb of segments allowed (" << fMaxSegments << "), new segment added to previous one!" << endl;
        aSegment = fMaxSegments-1;
      }
      fMatchingPlane[aModuleType-1][aModuleNumber-1][aInput-1][aSegment] = iPlane;
      fIndexShift[aModuleType-1][aModuleNumber-1][aInput-1][aSegment] = aChannel;
      fInputSegments[aModuleType-1][aModuleNumber-1][aInput-1].push_back(aOffset);
      cout << "   segment " << aSegment << " with offset " << aOffset << " of input " << aInput << " of module type " << aModuleType << " nb " << aModuleNumber << " associated to plane " << iPlane << " with index shift " << aChannel << endl;

    } // end loop on inputs
  } // end loop on planes


  // Now, check that each input has some correct plane number associated
  // JB, 2009/05/25; modified JB 2013/08/14
  iModule = 0;
  Int_t aPlaneNumber;
  for (mdt = 1; mdt <= fModuleTypes; mdt++){ // loop on module types
    if( fc->GetAcqPar().IfExternalTimeRef ) {
      InitTimeRefInfo( ); // JB 2018/02/11
    }

for (mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each module

      for ( Int_t iInp = 1; iInp <= fc->GetModulePar(mdt).Inputs; iInp++) { // loop on inputs
        for ( Int_t iSeg=1; iSeg<=(Int_t)fInputSegments[mdt-1][mdl-1][iInp-1].size(); iSeg++) { //loop on segments

          aPlaneNumber = fMatchingPlane[mdt-1][mdl-1][iInp-1][iSeg-1];
          if( aPlaneNumber<1 && aPlaneNumber>fc->GetTrackerPar().Planes ) {
            printf( "ERROR: module type %d, module # %d, input %d segment %d is not associated properly to a plane! Getting %d instead of 1-%d.\n", mdt, mdl, iInp, iSeg, fMatchingPlane[mdt-1][mdl-1][iInp-1][iSeg-1], fc->GetTrackerPar().Planes);
          }

          // For DUT plane, indicate it to the module
          // Note the inputNumber-1  because PXIeBoardReader input index starts at 0
          // JB 2014/03/13
          else if( fc->GetPlanePar(aPlaneNumber).Status == 3 ) {
            if (fDebugAcq) cout << "Declaring plane " << aPlaneNumber << " as DUT to module " << mdl << " of type " << fc->GetModulePar(mdt).Type << " linked to input(from 0) " << fc->GetPlanePar(aPlaneNumber).InputNumber[0]-1 << endl;
            switch ( (fc->GetModulePar(mdt).Type)/10 ) {
                // -+-+- PXIe modules
          // Comment added by Ziad EL BITAR on May 25, 2021, disregard PXIeModule when using MimosisBoardReader
          //    case 5:
          //      fPXIe[iModule]->AddDUTSensor( fc->GetPlanePar(aPlaneNumber).InputNumber[0]-1 );
            };
          }

        } // end loop on segments
      } // end loop on each inputs

      iModule++;
    } // end loop on each modules of this type
  } // end loop on module types

  if( synchroNeeded ) {
    InitSynchroInfo( ); // JB 2012/07/19
  }

  if( fc->GetAcqPar().IfExternalTimeRef ) {
    InitTimeRefInfo( ); // JB 2018/02/11
  }


  // STOP when something was wrong in the init, JB 2009/05/25
  // only if rawdata reading required, JB 2013/08/21
  if( !initOK ) {
    Char_t choice;
    Warning( "DAcq", "Cannot construct Acquisition !!\n -> Choose your option: type [q] to quit or anything else to proceed (if you know you don't need acq).");
    cin >> choice;
    if (choice=='q') {
      Fatal("DAcq", "--> STOPPING because no acquisition <--");
    }
  }
  cout << endl << " -*-*- DAcq Constructor DONE -*-*- " << endl;
}

//______________________________________________________________________________
//
DAcq::~DAcq()
{
  // Default DAcq destructor.
}

//______________________________________________________________________________
//
void DAcq::SetDebug(Int_t aDebug)
{
  // Set the debug level
  // JB 2009/05/22
  // Modified JB 2009/08/20, PXIBoard added
  // Modified JB 2009/09/09, debug set if level=0
  // Modified JB 2010/08/23 to use modulteType/10
  // Modified JB 2011/06/19 Introduction of PXI & PXIe readers for backward compatibility
  // Modified MG 2012/03/14 added lineOverflow for PXIeBoardReader
  // Modified JB 2012/04/25 to add GIG boards (GEANT4-DIGMaps)
  // Modified SS 2012/08/01 to add IMG boards
  // Modified JB 2014/05/13 to add VME boards
  // Modified JB 2018/10/09 to add IHEP boards

  Int_t iModule=0; // module index, from 0 to totalNmodules

  cout << "DAcq: debug set: " << aDebug << endl;

  if( aDebug<=0 ) { // if negative level, set acquisition module level

    for ( Int_t mdt = 1; mdt <= fModuleTypes; mdt++){ // loop on module types

      switch ( (fc->GetModulePar(mdt).Type)/10 ) {

          // -+-+- IMG modules
        case 1:
          for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
            fIMG[iModule]->SetDebugLevel( abs(aDebug) );
            iModule++;
          } // end loop on each module of this type
          break;

          // -+-+- TNT modules
        case 3:
          for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
            fTNT[iModule]->SetDebugLevel( abs(aDebug) );
            iModule++;
          } // end loop on each module of this type
          break;

          // -+-+- PXI modules
        case 4:
          for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
            fPXI[iModule]->SetDebugLevel( abs(aDebug) );
            iModule++;
          } // end loop on each module of this type
          break;

          // -+-+- PXIe modules
        // Ziad EL BITAR, 2021/05/25 comment PXIe module when using Mimosis1
        /*
        case 5:
          for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
            fPXIe[iModule]->SetDebugLevel( abs(aDebug) );
            iModule++;
          } // end loop on each module of this type
          break;
*/
          // -+-+- GIG modules
        case 6:
          for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
            fGIG[iModule]->SetDebugLevel( abs(aDebug) );
            iModule++;
          } // end loop on each module of this type
          break;

          // -+-+- VME modules
        case 7:
          for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
            fVME[iModule]->SetDebugLevel( abs(aDebug) );
            iModule++;
          } // end loop on each module of this type
          break;

          // -+-+- ALI22 modules
        case 8:
          for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
            fALI22[iModule]->SetDebugLevel( abs(aDebug) );
            iModule++;
          } // end loop on each module of this type
          break;

          // -+-+- DecoderM18 modules
        case 9:
          for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
            fM18[iModule]->SetDebugLevel( abs(aDebug) );
            iModule++;
          } // end loop on each module of this type
          break;

          // -+-+- DecoderGeant modules
        case 10:
          for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
            fGeant[iModule]->SetDebugLevel( abs(aDebug) );
            iModule++;
          } // end loop on each module of this type
          break;

          // -+-+- MC modules
        case 11:
          for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
            fMC[iModule]->SetDebugLevel( abs(aDebug) );
            iModule++;
          } // end loop on each module of this type
          break;

          // -+-+- IHEP modules
        case 12:
          for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
            fIHEP[iModule]->SetDebugLevel( abs(aDebug) );
            iModule++;
          } // end loop on each module of this type
          break;

          // -+-+- MIMOSIS modules
        case 13:
          for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
            fMSIS[iModule]->SetDebugLevel( abs(aDebug) );
            iModule++;
          } // end loop on each module of this type
          break;

      }; // end switch on module types


    } // end loop on module types

  } // end if negative level

  fDebugAcq = abs(aDebug);
  cout << "DAcq debug updated to " << abs(aDebug) << endl;

}

//______________________________________________________________________________
//
Int_t* DAcq::GetRawData( Int_t mdt, Int_t mdl, Int_t input)
{
  // Return the pointer to the address of the first data of
  // the given mdt module type, mdl module number and input
  // all these index are expected to start at 1
  //
  // JB, 2008/10/17
  // Modified: JB 2013/08/16 manage different channel numbers per input

  return fRawData[mdt-1][mdl-1]+(input-1)*fc->GetModulePar(mdt).Channels[input-1];

}

//______________________________________________________________________________
//
void DAcq::GetMatchingPlaneAndShift( Int_t mdt, Int_t mdl, Int_t input, Int_t channel, Int_t &aPlane, Int_t &aShift)
{
  // Set the plane and the shift matching the given
  //  module type, number, input and channel.
  // All indexes expected to start at 1.
  //
  // The vector fInputSegments provides the lower limits of a segment
  //  for a given module type and number and input number.
  // Its minimum size is 1.
  // This means that if an input has only one segment: fInputSegments[0] = 1,
  //  and with segments like 1-256 / 257-512:
  //   fInputSegments[0] = 1 and fInputSegments[1] = 257
  //
  // JB 2013/08/14

  if(fDebugAcq>3) {
    printf("DAcq::GetMatchingPlaneAndShift looking for plane associated with module type %d, nb %d, input %d, channel %d", mdt, mdl, input, channel);
    printf(" within %d segments",(int)fInputSegments[mdt-1][mdl-1][input-1].size()); // removing warning: cast 'size_type' (aka 'unsigned long') to 'int' BH 2013/08/20
    for (Int_t iSeg=0; iSeg<(Int_t)fInputSegments[mdt-1][mdl-1][input-1].size(); iSeg++) {  // removing warning: cast 'size_type' (aka 'unsigned long') to 'Int_t' (aka 'int') BH 2013/08/20
      printf(": %d", fInputSegments[mdt-1][mdl-1][input-1].at(iSeg));
    }
    printf("\n");
  }

  Int_t aSegment = (Int_t)fInputSegments[mdt-1][mdl-1][input-1].size()-1;

  while (0<aSegment && channel<fInputSegments[mdt-1][mdl-1][input-1].at(aSegment)) {
    aSegment--;
  }

  aPlane = fMatchingPlane[mdt-1][mdl-1][input-1][aSegment];
  aShift = fIndexShift[mdt-1][mdl-1][input-1][aSegment]
          - fInputSegments[mdt-1][mdl-1][input-1].at(aSegment)+1;

  if(fDebugAcq>3) printf("DAcq::GetMatchingPlaneAndShift found plane %d and shift %d from segment %d\n", aPlane, aShift, aSegment);

}

//______________________________________________________________________________
//
Bool_t DAcq::InitSynchroInfo( )
{
  // Decide the synchronization strategy depending on the types of BoardReader.
  //
  // * In general: do nothing
  //
  // * When PXIeBoardReader and IMGBoardReader:
  //  Read the file containing the synchronization information
  //  and store them in a buffer.
  //
  // Return true upon success.
  //
  // JB, 2012/07/19
  // Modified: JB 205/03/27 to allow for different strategy

  Int_t nIMGBoard = 0;
  Int_t nPXIeBoard = 0;
  Int_t nM18Decoder = 0;

  for (int mdt = 1; mdt <= fModuleTypes; mdt++){ // loop on module types
    switch ( (fc->GetModulePar(mdt).Type)/10 ) {
      case 1:
        nIMGBoard++;
        break;
      case 5:
        nPXIeBoard++;
        break;
      case 9:
        if ( nM18Decoder==0 ) {
          fSynchroFirstM18Decoder = mdt;
        }
        nM18Decoder++;
        break;
    };
  } // loop on module types

  //===================
    
  // Ziad EL BITAR 2021/05/25 disregard PXIeModule when using MimosisBoardReader
    /*
  if (nIMGBoard>0 && nPXIeBoard>0 ) { // if synchro IMG, PXIe

    // Set the file name
    fSynchroFileName = new Char_t[550];
    sprintf( fSynchroFileName, "%s/RUN_%d_sync.bin", fc->GetRunPar().DataPath, fRunNumber);
    if (fDebugAcq)  cout << " DAcq::InitSynchro(), opening synchronization file " << fSynchroFileName << endl;

    // Get the file size which is the size of the buffer needed
    Int_t bufferSize= GetFileSize( fSynchroFileName);
    if (fDebugAcq)  cout << " DAcq::InitSynchro(), synchronization file contains " << bufferSize << " Bytes, from structure of " << sizeof( APP__TSyncIndexRec) << " Bytes." << endl;

    // Then, load the file in the buffer
    fNbSynchroInfo = 0; // will stay at 0 if something fails
    if( bufferSize>0 && bufferSize%sizeof( APP__TSyncIndexRec)==0 ) {
      fSynchroInfo = new unsigned char[bufferSize];
      fNbSynchroInfo = GetSyncPointer( fSynchroFileName, fSynchroInfo, bufferSize);
      if (fDebugAcq)  cout << " DAcq::InitSynchro(), " << fNbSynchroInfo << " words have been loaded." << endl;
    }
    else {
      fSynchroInfo = NULL;
      cout << "ERROR, problem with the synchronization file which is either empty (size = " << bufferSize << " Bytes) or corrupted (file_size%structure_size = " << bufferSize%sizeof( APP__TSyncIndexRec) << ")." << endl;
    }

    // If success, the buffer size is not 0
    return bufferSize>0 && fNbSynchroInfo>0;

  } // end if synchro IMG, PXIe

*/
  //===================
  if ( nM18Decoder>=2 ) {
    cout << endl << "Synchronization of M18Decoders requires passing OMKDTransition information from one file to the other." << endl;
    cout << "  -> first M18Decoder used to read OMKDTransition is of type  " << fSynchroFirstM18Decoder << endl;
  }


  //===================
  // if no known case of synchro found, simply don't complain
  cout << endl << "Though several BoardTypes are declared, no specicif synchronization mechanism is needed." << endl;

  return kTRUE;
}

//______________________________________________________________________________
//
Bool_t DAcq::GetSynchroInfo( int anEventId, int &anAcqId, int &aFrameId)
{

  // Get the synchornization information between two files (two boards).
  // anEventId is the event number of the first file.
  // anAcqId and aFrameId are the returned position in the second file of
  //  the corresponding event.
  //
  // Return true upon success, if failure IDs are set to -1.
  //
  // JB, 2012/07/19
  // Modified: SS, 2012/08/01 bug fix for iEvent init
  // Modified: SS, 2013/06/25 bug fix for iEvent exceeding fNbSynchroInfo
  // Modified: LC, 2013/08/20 bug fix for iEvent < 0

  Bool_t eventFound = false;
  anAcqId = -1;
  aFrameId = -1;
  int absoluteFrameNb = -1;

  if (fDebugAcq>1)  cout << " DAcq::GetSynchroInfo(), searching for event " << anEventId << " among " << fNbSynchroInfo << " entries." << endl;

  if( fSynchroInfo != NULL ) {

    APP__TSyncIndexRec* pointer2Event = (APP__TSyncIndexRec*)fSynchroInfo;

    unsigned int iEvent=anEventId; //SS 2012.08.06 Reset iEvent counter always starts at 0 to account for missed events, Changed to expected event, JB 2012
    if (iEvent>=(unsigned int)fNbSynchroInfo) iEvent=fNbSynchroInfo-1; //SS 2013.06.25 protection against that iEvent exceeds the limit of fNbSynchroInfo
    //printf( "   anEventID=%d, iEvent=%d", anEventId, iEvent);
    //printf( ", DutEvId=%d\n", (int)pointer2Event[iEvent].DutEvId);

    // Added protection against iEvent<0 in the loop below, LC 2013/08/20
    while( 0<iEvent && iEvent<(unsigned int)fNbSynchroInfo && anEventId < (int)pointer2Event[iEvent].DutEvId) {
      //printf( "   anEventID=%d, iEvent=%d", anEventId, iEvent);
      //printf( ", DutEvId=%d", (int)pointer2Event[iEvent].DutEvId);
      if (pointer2Event[iEvent].DutEvId - anEventId > iEvent) {
        iEvent -= pointer2Event[iEvent].DutEvId - anEventId;
      }
      else {
        iEvent = 0;
      }
      //printf(" --> new iEvent=%d\n", iEvent);
    }
    while( iEvent<(unsigned int)fNbSynchroInfo && anEventId>=(int)pointer2Event[iEvent].DutEvId && !eventFound) {
      if (fDebugAcq>1)  cout << " DAcq::GetSynchroInfo(), testing entry " << iEvent << " with eventId = " << pointer2Event[iEvent].DutEvId << endl;
      if( anEventId == (int)pointer2Event[iEvent].DutEvId ) {
        anAcqId = pointer2Event[iEvent].TelTrigAcqId;
        aFrameId = pointer2Event[iEvent].TelTrigFrIdInAcq; // add a -1 for TEMPORARY FIX runs 32703 to 32705 DESY June 2013 beam test, JB 2013/06/22
        absoluteFrameNb = pointer2Event[iEvent].TelTrigEvTag;
        eventFound = true;
      }
      iEvent++;
    }
    if (fDebugAcq>1)  cout << " DAcq::GetSynchroInfo(), event " << anEventId << " has " << (eventFound?"":"NOT") << " been found and corresponds to AckID = " << anAcqId << ", FrameID = " << aFrameId << " absolute nb = " << absoluteFrameNb << endl;

  }
  else {
    cout << "WARNING DAcq::GetSynchroInfo(), synchronization information has not been initialized properly." << endl;
  }

  return eventFound;
}

//______________________________________________________________________________
//
Bool_t DAcq::InitTimeRefInfo( )
{
  // Open the external file with time reference information
  //
  // Return true upon success.
  //
  // JB, 2018/02/11
  // Modified JB 2018/03/21 Check both case of time ref file location w and wo runNumber in path

  FILE *timeRefFile;

  // Set the file name
  // Get the file size which is the size of the buffer needed
  fTimeRefFileName = new Char_t[550];
  sprintf( fTimeRefFileName, "%s/time_stamp_log.bin", fc->GetRunPar().DataPath);
  if (fDebugAcq)  cout << " DAcq::InitTimeRef(), opening time reference file " << fTimeRefFileName << endl;
  Int_t bufferSize = GetFileSize( fTimeRefFileName);
  if( !(bufferSize>0) ) {
    sprintf( fTimeRefFileName, "%s/%d/time_stamp_log.bin", fc->GetRunPar().DataPath, fRunNumber);
    if (fDebugAcq) cout << " DAcq::InitTimeRef(), opening time reference file " << fTimeRefFileName << endl;
    bufferSize = GetFileSize( fTimeRefFileName);
  }
  if (fDebugAcq)  {
    cout << " DAcq::InitTimeRef(), synchronization file contains " << bufferSize << " Bytes, from structure of " << sizeof( SEXP_TTsRec) << " Bytes, which should be " << fTimeRefRecordSize << " Bytes." << endl;
    cout << "                     nb of records expected = " << bufferSize/sizeof( SEXP_TTsRec) << " -> is that an integer? " << !(bufferSize%sizeof( SEXP_TTsRec)) << endl;
  }

  // Then, load the file in the buffer
  fNbTimeRefInfo = 0; // will stay at 0 if something fails
  fTimeRefInfo = NULL;
  if( bufferSize>0 && bufferSize%sizeof( SEXP_TTsRec)==0 && sizeof( SEXP_TTsRec)==fTimeRefRecordSize ) {
    timeRefFile = fopen ( fTimeRefFileName, "rb" );
    if( timeRefFile != NULL ) {

      int expectedInfoNb = bufferSize / sizeof( SEXP_TTsRec);
      fTimeRefInfo = new SEXP_TTsRec[expectedInfoNb];
      fNbTimeRefInfo = fread ( fTimeRefInfo, sizeof( SEXP_TTsRec), expectedInfoNb, timeRefFile );
      if( fNbTimeRefInfo != expectedInfoNb ) {
        fNbTimeRefInfo = 0;
        fTimeRefInfo = NULL;
        cout << "ERROR, cannot read info in time reference file " << fTimeRefFileName << "." << endl;
      }
      else {
        if (fDebugAcq)  cout << " DAcq::InitTimeRef(), " << fNbTimeRefInfo << " records have been loaded." << endl;
      }

      if( fclose( timeRefFile ) ) {
        cout << "WARNING, cannot close time reference file " << fTimeRefFileName << "." << endl;
      }

    } // timeRefFile != NULL

    else { // File cannot be opened
      cout << "ERROR, cannot open time reference file " << fTimeRefFileName << "." << endl;
    }

  }
  else { // in case the file cannot be read properly, switch off
    cout << "ERROR, problem with the synchronization file which is either empty (size = " << bufferSize << " Bytes) or corrupted (file_size%structure_size = " << bufferSize%sizeof( APP__TSyncIndexRec) << ")." << endl;
  }

  if ( fTimeRefInfo == NULL ) {
    cout << "ERROR, CANNOT use external time reference, reverting to not using it!" << endl;
    fc->GetAcqPar().IfExternalTimeRef = 0;
    fCurrentTimeRefInfo = 0;
    return kFALSE;
  }

  fCurrentTimeRefInfo = 0;
  return kTRUE;
}


//______________________________________________________________________________
//
Bool_t DAcq::GetTimeRef( int index, int &recordID, int &cycleID, int &rtcTime, int &ntpTime)
{
  // Get the time reference information at the given index
  // The returned format of rtcTime and ntpTime is the number of seconds since
  //
  // Return true upon success.
  //
  // JB, 2018/02/11

  if( fTimeRefInfo == NULL || index>=fNbTimeRefInfo ) return kFALSE;

  // Possibly test that ResCnt = ReStartCnt
  recordID = fTimeRefInfo[index].RecCnt;
  cycleID = fTimeRefInfo[index].InfoCyclicTS;
  // Still need to decide how to format ime
  struct std::tm atime;
  atime.tm_year = fTimeRefInfo[index].RtcDateTime.Date.Year+100; // Gille's info is year since 2000 while years since 1900 is expected
  atime.tm_mon = fTimeRefInfo[index].RtcDateTime.Date.Month;
  atime.tm_mday = fTimeRefInfo[index].RtcDateTime.Date.Day;
  atime.tm_hour = fTimeRefInfo[index].RtcDateTime.Time.Hour;
  atime.tm_min = fTimeRefInfo[index].RtcDateTime.Time.Min;
  atime.tm_sec = fTimeRefInfo[index].RtcDateTime.Time.Sec;
  atime.tm_isdst = 0; // no daylight saving time is not used in Japan
  rtcTime = mktime( &atime);
  //  rtcTime = fTimeRefInfo[index].RtcDateTime.Time.Sec;

  struct std::tm btime;
  btime.tm_year = fTimeRefInfo[index].NtpDateTime.Date.Year+100; // Gille's info is year since 2000 while years since 1900 is expected
  btime.tm_mon = fTimeRefInfo[index].NtpDateTime.Date.Month;
  btime.tm_mday = fTimeRefInfo[index].NtpDateTime.Date.Day;
  btime.tm_hour = fTimeRefInfo[index].NtpDateTime.Time.Hour;
  btime.tm_min = fTimeRefInfo[index].NtpDateTime.Time.Min;
  btime.tm_sec = fTimeRefInfo[index].NtpDateTime.Time.Sec;
  btime.tm_isdst = 0; // no daylight saving time is not used in Japan
  rtcTime = mktime( &btime);
  //  ntpTime = fTimeRefInfo[index].NtpDateTime.Time.Sec;

  if ( fDebugAcq>2 ) {
    cout << " DAcq::GetTimeRef for index " << index << ", record " << recordID;
    cout << ", cycle " << cycleID << ", rtc time " << rtcTime << ", ntp time " << ntpTime << " seconds since Epoch."<< endl;
    cout << "    Detail RTC info: year/month/day = " << 1900+atime.tm_year << "/" << atime.tm_mon << "/" << atime.tm_mday << ", hour/min/sec/ms = " << atime.tm_hour << "/" << atime.tm_min << "/" << atime.tm_sec << "/" << fTimeRefInfo[index].RtcDateTime.Time.Ms << endl;
    cout << "    Detail NTP info: year/month/day = " << 1900+btime.tm_year << "/" << btime.tm_mon << "/" << btime.tm_mday << ", hour/min/sec/ms = " << btime.tm_hour << "/" << btime.tm_min << "/" << btime.tm_sec << "/" << fTimeRefInfo[index].NtpDateTime.Time.Ms << endl;
  }

  return kTRUE;
}


//______________________________________________________________________________
//
void DAcq::Reset()
{
  // Reset event reading at 0
  // So that next event is the very first one
  //
  // NOT FULLY FUNCTIONAL YET
  //
  // JB 2015/03/02

  if( fModuleTypes>1 ) {
   cout << "CANNOT RESET DAQ when more than one module type -> nop." << endl << endl;
  }

  //====================
  if (fDebugAcq)  cout << " DAcq: resetting to first event" << endl;
  Int_t iModule=0; // module index, from 0 to totalNmodules
  for ( Int_t mdt = 1; mdt <= fModuleTypes; mdt++){ // loop on module types

    switch ( (fc->GetModulePar(mdt).Type)/10 ) {

      // -+-+- PXIe modules
/*
    case 5:
        fPXIe[iModule]->ResetReading();
        iModule++;
        break;
*/
      // No other case implemented
      default:
        cout << " RESET DAQ not yet implemented for this module type -> nop." << endl << endl;
        iModule++;

    }

  } // end loop on module types

}

//______________________________________________________________________________
//
TBits* DAcq::NextEvent( Int_t eventNumber, Int_t aTrigger)
{
  // Read next event from file(s),
  //  the event number is defined by DSession.
  // If aTrigger != -1 (default is -1) then the event with this specific trigger
  //  number is searched for (valid only for PXIeBoardReader yet).
  // The return code is a pointer to 3 bits :
  //  bit 0: 1 if readout is OK (meaning there are still some events to read),
  //  bit 1: 1 if synchronization between modules was required and succesfull,
  //  bit 2: 1 if event data is OK (meaning analysis can be performed).
  //
  // JB, 208/10/13
  // Last modified JB 2009/05/12
  // Last modified JB 2009/08/20 to add PXI boards
  // Last modified JB 2010/06/16 to store full trigger and frame info from BoardReaders
  // Last modified JB 2010/08/23 to use modulteType/10
  // Last modified JB 2011/06/19 Introduction of PXI & PXIe readers for backward compatibility
  // Last modified MG 2012/03/14 Line overflow vector for PXIe
  // Last modified JB 2012/04/25 to add GIG boards (GEANT4-DIGMaps)
  // Last modified JB 2012/07/10 to allow selection of event by trigger
  // Last modified JB 2012/07/22 to add IMG boards (analog Imager)
  // Last modified JB 2012/07/22 to synchronized IMG and PXIe boards
  // Last modified SS 2012/08/01 bug fixes for synchronization
  // Last modified SS 2012/08/10 introducing TBits output and missed events
  // Last modified JB 2013/06/22 concatenation of lists from different modules
  // Last modified JB 2013/08/14 usage of GetMatchingPlaneAndShift
  // Last modified JB 2014/05/13 to add VME boards
  // Last modified JB 2015/03/27 to synchronize two DecoderM18
  // Last modified JB 2015/05/25 TimeStamp added for Ali22
  // Last modified JB 2018/02/21 reads external time reference if required

  Bool_t eventOK = kTRUE; // init at true, end-up false if one module fails
  Bool_t moduleOK = true; // init at true, end-up false if HasData() fails
  Bool_t eventMissed = kFALSE; // to check correct synchronization, SS 2012/08/10
  Bool_t dataOK = kTRUE; //  to check the event data can be processed, JB 2012/08/18

  TBits* DAcqResult=new TBits(3);
  Int_t  aPlaneNumber, aShift;

  BoardReaderEvent *readerEvent;
  BoardReaderPixel *readerPixel;

  fEventNumber = eventNumber; // JB 2009/05/26
  if (fDebugAcq)  cout << " DAcq::NextEvent(), getting event " << fEventNumber << endl;

  // Init added because each module types update those lists
  // JB 2013/06/22
  fTriggersN    = 0;
  fFramesN      = 0;
  fTimestampsN  = 0;
  if (ListOfTriggers!=NULL) ListOfTriggers->clear();
  if (ListOfFrames!=NULL) ListOfFrames->clear();
  if (ListOfTimestamps!=NULL) ListOfTimestamps->clear();

  //====================
  // erasing pixel and pixel list for all planes
  if (fDebugAcq)  cout << " DAcq::NextEvent(), erasing Pixel list for all planes." << endl;
  for( Int_t iPlane = 0; iPlane<fc->GetTrackerPar().Planes; iPlane++) {
    for( Int_t iPix=0; iPix<(Int_t)fListOfPixels[iPlane].size(); iPix++) {
      delete fListOfPixels[iPlane].at(iPix);
    }
    /*
     for( Int_t iMonteCarlo=0; iMonteCarlo<(Int_t)fListOfMonteCarlo[iPlane].size(); iMonteCarlo++) {
     delete fListOfMonteCarlo[iPlane].at(iMonteCarlo);
     }
     */
    fListOfPixels[iPlane].clear();
    //fListOfMonteCarlo[iPlane].clear();
  }

  //====================
  // Check for synchronization, JB 2012/07/22
  Bool_t synchroNeeded = false;
  if( fModuleTypes>1 ) synchroNeeded = true;

  //====================
  // Check for external time reference, JB 2018/02/12
  //  ==> JUST READ THE FIRST ONE for now, JB 2018/03/21
  int recordID, cycleID;
  int rtcTime, ntpTime;
  if( fc->GetAcqPar().IfExternalTimeRef ) {
    if( fCurrentTimeRefInfo==0 ) { // first reading the timeRef info ?
      if( GetTimeRef( fCurrentTimeRefInfo, recordID, cycleID, rtcTime, ntpTime) ) {
        fEventReferenceTime = rtcTime;
        fCurrentTimeRefInfo++;
        if ( fDebugAcq ) cout << " -> Got new external time reference " << fEventReferenceTime << ", from index " << fCurrentTimeRefInfo << endl;
      }
      else { // timeRef info already read
        cout << "WARNING: DAcq, Cannot read time reference !!." << endl;
      }
    }

    else { // timeRef info already read
      if ( fDebugAcq ) cout << " -> Keep external time reference " << fEventReferenceTime << ", from index " << fCurrentTimeRefInfo << endl;
    }
  }

  else { // IfExternalTimeRef is false
    if ( fDebugAcq ) cout << " -> No more external time reference, sticking to " << fEventReferenceTime << endl;
  }


  //====================
  if (fDebugAcq)  cout << " DAcq: getting raw data:" << endl;
  Int_t iModule=0; // module index, from 0 to totalNmodules
  for ( Int_t mdt = 1; mdt <= fModuleTypes; mdt++){ // loop on module types

    switch ( (fc->GetModulePar(mdt).Type)/10 ) {

        // -+-+- IMG modules (added JB 2012/07/22)
      case 1:
        IMGEvent *imgEvent;
        IMGPixel *imgPixel;
        for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type

          // 	if (fDebugAcq)  cout << " DAcq: erasing fRawData for module " << mdl << " of type " << fc->GetModulePar(mdt).Type << " # indexes " << fc->GetModulePar(mdt).Channels[0]*fc->GetModulePar(mdt).Inputs << endl;
          // 	for( Int_t iPix=0; iPix<fc->GetModulePar(mdt).Channels[0]*fc->GetModulePar(mdt).Inputs; iPix++) {
          // 	  fRawData[mdt-1][mdl-1][iPix] = 0;
          // 	}


          moduleOK = fIMG[iModule]->HasData(); // ask for an event
          eventOK &= moduleOK;
          if (fDebugAcq)  cout << " DAcq: getting raw data for module " << iModule << " or " << mdl << " of type " << fc->GetModulePar(mdt).Type << ", OK? " << moduleOK << endl;

          imgEvent = fIMG[iModule]->GetEvent(); // get the event
          if( imgEvent ) { // If event pointer correct
            fRealEventNumber = imgEvent->GetEventNumber();
            fTriggersN      += imgEvent->GetNumberOfTriggers();
            fFramesN        += imgEvent->GetNumberOfFrames();
            fTimestampsN    += imgEvent->GetNumberOfTimestamps();
            if (ListOfTriggers==NULL) {
              ListOfTriggers = imgEvent->GetTriggers();
            }
            else {
              ListOfTriggers->insert( ListOfTriggers->end(), (imgEvent->GetTriggers())->begin(), (imgEvent->GetTriggers())->end());
              ;
            }
            if (ListOfFrames==NULL) {
              ListOfFrames     = imgEvent->GetFrames();
            }
            else {
              ListOfFrames->insert( ListOfFrames->begin(), (imgEvent->GetFrames())->begin(), (imgEvent->GetFrames())->end());
              ;
            }
            if (ListOfTimestamps==NULL) {
              ListOfTimestamps     = imgEvent->GetTimestamps();
            }
            else {
              ListOfTimestamps->insert( ListOfTimestamps->begin(), (imgEvent->GetTimestamps())->begin(), (imgEvent->GetTimestamps())->end());
              ;
            }
            if (fDebugAcq) {
              cout << "   module " << mdl << " found " << imgEvent->GetNumberOfPixels() << " hit pixels with " << fTriggersN << " triggers: ";
              for( Int_t iTrig=0; iTrig<fTriggersN; iTrig++) {
                cout << ListOfTriggers->at( iTrig) <<  ", ";
              }
              cout << " and " << fFramesN << " frames: ";
              for( Int_t iFr=0; iFr<fFramesN; iFr++) {
                cout << ListOfFrames->at( iFr) <<  ", ";
              }
              cout << " and " << fTimestampsN << " timestamps: ";
              for( Int_t iTs=0; iTs<fTimestampsN; iTs++) {
                cout << ListOfTimestamps->at( iTs) <<  ", ";
              }
              cout << " from daq event " << fRealEventNumber << endl << endl;
            }
            // Set values for hit pixels
            for( Int_t iPix=0; iPix<imgEvent->GetNumberOfPixels(); iPix++) { // loop on Pixels
              imgPixel = imgEvent->GetPixelAt( iPix);
              GetMatchingPlaneAndShift( mdt, mdl, imgPixel->GetInput()+1, imgPixel->GetIndex()+1, aPlaneNumber, aShift);
              //aPlaneNumber = fMatchingPlane[mdt-1][mdl-1][imgPixel->GetInput()];
              //aShift = fIndexShift[mdt-1][mdl-1][imgPixel->GetInput()];
              if(fDebugAcq>2) cout << "  pixel " << iPix << " index " << imgPixel->GetIndex() << " from input " << imgPixel->GetInput() << " with value " << imgPixel->GetValue() << ", associated to plane " << aPlaneNumber << " with an index shift of " << aShift << " Timestamp " << imgPixel->GetTimeStamp() << endl;
              //if (imgPixel->GetValue()<0) {cout << " we also have negative pulseheights " << imgPixel->GetValue() << endl; } //YV check 22/07/09

              DPixel* APixel = new DPixel( aPlaneNumber, imgPixel->GetIndex()+aShift, (Double_t)imgPixel->GetValue(), imgPixel->GetTimeStamp());
              //fListOfPixels[aPlaneNumber-1].push_back( new DPixel( aPlaneNumber, imgPixel->GetIndex()+aShift, (Double_t)imgPixel->GetValue(), imgPixel->GetTimeStamp()));
              fListOfPixels[aPlaneNumber-1].push_back(APixel);

            } // end loop on Pixels

          }  // End if event pointer correct
          else {
            if (fEventsDataNotOK%1000==0) {
              cout << "WARNING: DAcq, Pointer to imgevent incorrect, " << fEventsDataNotOK << " such events so far!" << endl;
            }
            dataOK &= kFALSE;
          }

          iModule++;
        } // end loop on each module of this type
        break;


        // -+-+- TNT modules
      case 3:
        TNTEvent *tntEvent;
        TNTPixel *tntPixel;
        for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type

          // 	if (fDebugAcq)  cout << " DAcq: erasing fRawData for module " << mdl << " of type " << fc->GetModulePar(mdt).Type << " # indexes " << fc->GetModulePar(mdt).Channels[0]*fc->GetModulePar(mdt).Inputs << endl;
          // 	for( Int_t iPix=0; iPix<fc->GetModulePar(mdt).Channels[0]*fc->GetModulePar(mdt).Inputs; iPix++) {
          // 	  fRawData[mdt-1][mdl-1][iPix] = 0;
          // 	}


          moduleOK = fTNT[iModule]->HasData(); // ask for an event
          eventOK &= moduleOK;
          if (fDebugAcq)  cout << " DAcq: getting raw data for module " << iModule << " or " << mdl << " of type " << fc->GetModulePar(mdt).Type << ", OK? " << moduleOK << endl;

          tntEvent = fTNT[iModule]->GetEvent(); // get the event
          if( tntEvent ) { // If event pointer correct, JB 2009/05/26
            fRealEventNumber = tntEvent->GetEventNumber();
            fTriggersN      += tntEvent->GetNumberOfTriggers();
            fFramesN        += tntEvent->GetNumberOfFrames();
            fTimestampsN    += tntEvent->GetNumberOfTimestamps();
            if (ListOfTriggers==NULL) {
              ListOfTriggers = tntEvent->GetTriggers();
            }
            else {
              ListOfTriggers->insert( ListOfTriggers->end(), (tntEvent->GetTriggers())->begin(), (tntEvent->GetTriggers())->end());
              ;
            }
            if (ListOfFrames==NULL) {
              ListOfFrames     = tntEvent->GetFrames();
            }
            else {
              ListOfFrames->insert( ListOfFrames->begin(), (tntEvent->GetFrames())->begin(), (tntEvent->GetFrames())->end());
              ;
            }
            if (ListOfTimestamps==NULL) {
              ListOfTimestamps     = tntEvent->GetTimestamps();
            }
            else {
              ListOfTimestamps->insert( ListOfTimestamps->begin(), (tntEvent->GetTimestamps())->begin(), (tntEvent->GetTimestamps())->end());
              ;
            }
            if (fDebugAcq) {
              cout << "   module " << mdl << " found " << tntEvent->GetNumberOfPixels() << " hit pixels with " << fTriggersN << " triggers: ";
              for( Int_t iTrig=0; iTrig<fTriggersN; iTrig++) {
                cout << ListOfTriggers->at( iTrig) <<  ", ";
              }
              cout << " and " << fFramesN << " frames: ";
              for( Int_t iFr=0; iFr<fFramesN; iFr++) {
                cout << ListOfFrames->at( iFr) <<  ", ";
              }
              cout << " and " << fTimestampsN << " timestamps: ";
              for( Int_t iTs=0; iTs<fTimestampsN; iTs++) {
                cout << ListOfTimestamps->at( iTs) <<  ", ";
              }
              cout << " from daq event " << fRealEventNumber << endl << endl;
            }
            // Set values for hit pixels
            for( Int_t iPix=0; iPix<tntEvent->GetNumberOfPixels(); iPix++) { // loop on Pixels

              tntPixel = tntEvent->GetPixelAt( iPix);
              GetMatchingPlaneAndShift( mdt, mdl, tntPixel->GetInput()+1, tntPixel->GetIndex()+1, aPlaneNumber, aShift);
              //aPlaneNumber = fMatchingPlane[mdt-1][mdl-1][tntPixel->GetInput()]; // JB 2009/05/06
              //aShift = fIndexShift[mdt-1][mdl-1][tntPixel->GetInput()]; // JB 2009/05/06
              if(fDebugAcq>2) cout << "  pixel " << iPix << " index " << tntPixel->GetIndex() << " from input " << tntPixel->GetInput() << " with value " << tntPixel->GetValue() << ", associated to plane " << aPlaneNumber << " with an index shift of " << aShift << endl;
              //if (tntPixel->GetValue()<0) {cout << " we also have negative pulseheights " << tntPixel->GetValue() << endl; } //YV check 22/07/09
              if( tntPixel->GetValue()>-4000.) {

                DPixel* APixel =  new DPixel( aPlaneNumber, tntPixel->GetIndex()+aShift, tntPixel->GetValue());
                //fListOfPixels[aPlaneNumber-1].push_back( new DPixel( aPlaneNumber, tntPixel->GetIndex()+aShift, tntPixel->GetValue())); //YV move the cut of the raw value of the pixel from 0 to -4000 22/07/09
                fListOfPixels[aPlaneNumber-1].push_back(APixel); //YV move the cut of the raw value of the pixel from 0 to -4000 22/07/09
              }
              //fRawData[mdt-1][mdl-1][tntPixel->GetIndex()+(fc->GetModulePar(mdt).Channels[0])*(tntPixel->GetInput())] = tntPixel->GetValue();

            } // end loop on Pixels

          }  // End if event pointer correct, JB 2009/05/26
          else {
            dataOK &= kFALSE;
          }

          iModule++;
        } // end loop on each module of this type
        break;



        // -+-+- PXI modules
        // JB 2009/08/20
      case 4:
        PXIEvent *pxiEvent;
        PXIPixel *pxiPixel;
        for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type

          // 	if (fDebugAcq)  cout << " DAcq: erasing fRawData for module " << mdl << " of type " << fc->GetModulePar(mdt).Type << " # indexes " << fc->GetModulePar(mdt).Channels[0]*fc->GetModulePar(mdt).Inputs << endl;
          // 	for( Int_t iPix=0; iPix<fc->GetModulePar(mdt).Channels[0]*fc->GetModulePar(mdt).Inputs; iPix++) {
          // 	  fRawData[mdt-1][mdl-1][iPix] = 0;
          // 	}


          moduleOK = fPXI[iModule]->HasData(); // ask for an event
          eventOK &= moduleOK;
          if (fDebugAcq)  cout << " DAcq: getting raw data for module " << iModule << " or " << mdl << " of type " << fc->GetModulePar(mdt).Type << ", OK? " << moduleOK << endl;

          pxiEvent = fPXI[iModule]->GetEvent(); // get the event
          if( pxiEvent ) { // If event pointer correct
            fRealEventNumber = pxiEvent->GetEventNumber();
            fTriggersN      += pxiEvent->GetNumberOfTriggers();
            fFramesN        += pxiEvent->GetNumberOfFrames();
            if (ListOfTriggers==NULL) {
              ListOfTriggers = pxiEvent->GetTriggers();
            }
            else {
              ListOfTriggers->insert( ListOfTriggers->end(), (pxiEvent->GetTriggers())->begin(), (pxiEvent->GetTriggers())->end());
              ;
            }
            if (ListOfFrames==NULL) {
              ListOfFrames     = pxiEvent->GetFrames();
            }
            else {
              ListOfFrames->insert( ListOfFrames->begin(), (pxiEvent->GetFrames())->begin(), (pxiEvent->GetFrames())->end());
              ;
            }
            if (fDebugAcq) {
              cout << "   module " << mdl << " found " << pxiEvent->GetNumberOfPixels() << " hit pixels  with " << fTriggersN << " triggers: ";
              for( Int_t iTrig=0; iTrig<fTriggersN; iTrig++) {
                cout <<  ", " << ListOfTriggers->at( iTrig);
              }
              cout << " and " << fFramesN << " frames: ";
              for( Int_t iFr=0; iFr<fFramesN; iFr++) {
                cout <<  ", " << ListOfFrames->at( iFr);
              }
              cout << " from daq event " << fRealEventNumber << endl << endl;
            }
            // Set values for hit pixels
            for( Int_t iPix=0; iPix<pxiEvent->GetNumberOfPixels(); iPix++) { // loop on Pixels

              pxiPixel = pxiEvent->GetPixelAt( iPix);
              aPlaneNumber = fMatchingPlane[mdt-1][mdl-1][pxiPixel->GetInput()-1][0];
              if(fDebugAcq>2) cout << "  pixel " << iPix << " line " << pxiPixel->GetLineNumber() << " column " << pxiPixel->GetColumnNumber() << " from input " << pxiPixel->GetInput() << " with value " << pxiPixel->GetValue() << ", associated to plane " << aPlaneNumber << endl;

              DPixel* APixel = new DPixel( aPlaneNumber, pxiPixel->GetLineNumber(), pxiPixel->GetColumnNumber(), (Double_t)pxiPixel->GetValue());
              //fListOfPixels[aPlaneNumber-1].push_back( new DPixel( aPlaneNumber, pxiPixel->GetLineNumber(), pxiPixel->GetColumnNumber(), (Double_t)pxiPixel->GetValue()));
              fListOfPixels[aPlaneNumber-1].push_back(APixel);

            } // end loop on Pixels

          }  // End if event pointer correct, JB 2009/05/26
          else {
            dataOK &= kFALSE;
          }

          iModule++;
        } // end loop on each module of this type
        break;


        // -+-+- PXIe modules
        // JB 2011/06/19
            /*
        // ZE 2021/05/25
      case 5:
        cout << "Ziad --> Reading Data in PXIe module" << endl;
        // This is the place for the snchronization
        int acqId, frId;
        acqId=-1;
        frId=-1;  //SS 2012.08.01 - acqId and frId were not initialized
        if( synchroNeeded ) {
          eventMissed |= !GetSynchroInfo( fRealEventNumber, acqId, frId);
          if( eventMissed ) {
            cout << "WARNING: Acquisition missed the synchronization of event " << fRealEventNumber
            << ", corresponding to acquisition " << acqId
            << " and frame " << frId
            << " in PXIexpress."
            << endl;
            fEventsMissed++;
          }
        }

        if(!eventMissed){ // check event is not missed (always true if synchro not needed), SS 2012/08/10
          PXIeEvent *pxieEvent;
          PXIePixel *pxiePixel;

          for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type

            // 	if (fDebugAcq)  cout << " DAcq: erasing fRawData for module " << mdl << " of type " << fc->GetModulePar(mdt).Type << " # indexes " << fc->GetModulePar(mdt).Channels[0]*fc->GetModulePar(mdt).Inputs << endl;
            // 	for( Int_t iPix=0; iPix<fc->GetModulePar(mdt).Channels[0]*fc->GetModulePar(mdt).Inputs; iPix++) {
            // 	  fRawData[mdt-1][mdl-1][iPix] = 0;
            // 	}

            if( aTrigger!=-1 ) {
              moduleOK = fPXIe[iModule]->HasData( aTrigger); // ask for the event matching trigger, JB 2012/07/10
            }
            else if( acqId!=-1 && frId!=-1 ) {
              moduleOK = fPXIe[iModule]->HasData( acqId, frId); // ask for the event matching the frame, JB 2012/07/22
            }
            else {
              moduleOK = fPXIe[iModule]->HasData(); // ask for the next event
            }
            eventOK &= moduleOK;
            if (fDebugAcq)  cout << " DAcq: getting raw data for module " << iModule << " or " << mdl << " of type " << fc->GetModulePar(mdt).Type << ", OK? " << moduleOK << endl;

            pxieEvent = fPXIe[iModule]->GetEvent(); // get the event
            if( pxieEvent!=NULL ) { // If event pointer correct
              fRealEventNumber = pxieEvent->GetEventNumber();
              fTriggersN      += pxieEvent->GetNumberOfTriggers();
              fFramesN        += pxieEvent->GetNumberOfFrames();
              fTimestampsN    += pxieEvent->GetNumberOfTimestamps();
              if (ListOfTriggers==NULL) {
                ListOfTriggers = pxieEvent->GetTriggers();
              }
              else {
                ListOfTriggers->insert( ListOfTriggers->begin(), (pxieEvent->GetTriggers())->begin(), (pxieEvent->GetTriggers())->end());
              }
              if (ListOfFrames==NULL) {
                ListOfFrames     = pxieEvent->GetFrames();
              }
              else {
                if(fPXIe[iModule]->GetFlag() == 0) {
                  ListOfFrames->insert( ListOfFrames->begin(), (pxieEvent->GetFrames())->begin(), (pxieEvent->GetFrames())->end());
                }
              }
              if (ListOfTimestamps==NULL) {
                ListOfTimestamps     = pxieEvent->GetTimestamps();
              }
              else {
                ListOfTimestamps->insert( ListOfTimestamps->begin(), (pxieEvent->GetTimestamps())->begin(), (pxieEvent->GetTimestamps())->end());
              }

              fEventTime = fEventReferenceTime + ListOfTimestamps->at(0)*115.e-6; // seconds since Epoch JB 2018/02/12
              ListOfLineOverflow = pxieEvent->GetLineOverflow(); //MG 2012/02/15
              if (fDebugAcq) cout << "    event time " << fEventTime << " sec" << endl;

              for(int i=0;i<fc->GetTrackerPar().Planes;i++) fLineOverflowN[i]=ListOfLineOverflow[i].size() ;//MG 2012/02/15

              if (fDebugAcq) {
                cout << "   module " << mdl << " found " << pxieEvent->GetNumberOfPixels() << " hit pixels  with " << fTriggersN << " triggers: ";
                for( Int_t iTrig=0; iTrig<fTriggersN; iTrig++) {
                  cout <<  ", " << ListOfTriggers->at( iTrig);
                }
                cout << " and " << fTimestampsN << " timestamps: ";
                for( Int_t iTs=0; iTs<fTimestampsN; iTs++) {
                  cout << ListOfTimestamps->at( iTs) <<  ", ";
                }
                cout << " and " << fFramesN << " frames: ";
                for( Int_t iFr=0; iFr<fFramesN; iFr++) {
                  cout <<  ", " << ListOfFrames->at( iFr);
                }

                cout << " and lines in overflow: ";
                for(int i=0;i<fc->GetTrackerPar().Planes;i++){
                  cout << ", " << fLineOverflowN[i] << " for pl "<< i; //MG 2012/02/15
                }
                cout << " from daq event " << fRealEventNumber << endl << endl;
              }
              // Set values for hit pixels
              for( Int_t iPix=0; iPix<pxieEvent->GetNumberOfPixels(); iPix++) { // loop on Pixels

                pxiePixel = pxieEvent->GetPixelAt( iPix);
                aPlaneNumber = fMatchingPlane[mdt-1][mdl-1][pxiePixel->GetInput()-1][0];
                if(fDebugAcq>2) cout << "  pixel " << iPix << " line " << pxiePixel->GetLineNumber() << " column " << pxiePixel->GetColumnNumber() << " from input " << pxiePixel->GetInput() << " with value " << pxiePixel->GetValue() << ", associated to plane " << aPlaneNumber << endl;

                DPixel* APixel = new DPixel( aPlaneNumber, pxiePixel->GetLineNumber(), pxiePixel->GetColumnNumber(), (Double_t)pxiePixel->GetValue(), ListOfTimestamps->at(0)); // relative timestamp added JB 2018/02/12
                //fListOfPixels[aPlaneNumber-1].push_back( new DPixel( aPlaneNumber, pxiePixel->GetLineNumber(), pxiePixel->GetColumnNumber(), (Double_t)pxiePixel->GetValue()));
                fListOfPixels[aPlaneNumber-1].push_back(APixel);

              } // end loop on Pixels

            }  // End if event pointer correct, JB 2009/05/26
            else {
              if (fEventsDataNotOK%1000==0) {
                cout << "WARNING: DAcq, Pointer to pxievent incorrect, " << fEventsDataNotOK << " such events so far!" << endl;
              }
              dataOK &= kFALSE;
            }

            iModule++;
          } // end loop on each module of this type
        } // end check event is not missed
        break;

*/
        // -+-+- GIG modules
        // JB 2012/04/25
      case 6:
        GIGEvent *gigEvent;
        GIGPixel *gigPixel;
        GIGMonteCarlo *gigMonteCarlo;

        for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type

          moduleOK = fGIG[iModule]->HasData(); // ask for an event
          eventOK &= moduleOK;
          if (fDebugAcq)  cout << " DAcq: getting raw data for module " << iModule << " or " << mdl << " of type " << fc->GetModulePar(mdt).Type << ", OK? " << moduleOK << endl;

          gigEvent = fGIG[iModule]->GetEvent(); // get the event
          if( gigEvent ) { // If event pointer correct
            fRealEventNumber = gigEvent->GetEventNumber();
            if (fDebugAcq) {
              cout << "   module " << mdl << " found " << gigEvent->GetNumberOfPixels() << " hit pixels";
              cout << " from daq event " << fRealEventNumber << endl << endl;
            }
            // Set values for hit pixels
            for( Int_t iPix=0; iPix<gigEvent->GetNumberOfPixels(); iPix++) { // loop on Pixels

              gigPixel = gigEvent->GetPixelAt( iPix);
              aPlaneNumber = fMatchingPlane[mdt-1][mdl-1][gigPixel->GetInput()-1][0];

              if(fDebugAcq>2) cout << "  pixel " << iPix << " line " << gigPixel->GetLineNumber() << " column " << gigPixel->GetColumnNumber() << " from input " << gigPixel->GetInput() << " with value " << gigPixel->GetValue() << ", associated to plane " << aPlaneNumber << endl;

              DPixel* APixel = new DPixel( aPlaneNumber, gigPixel->GetLineNumber(), gigPixel->GetColumnNumber(), (Double_t)gigPixel->GetValue());

              if( fIfMonteCarlo == 1) {
                gigMonteCarlo = gigEvent->GetMonteCarloAt(iPix);
                APixel->SetMonteCarlo(gigMonteCarlo->GetMonteCarloX(), gigMonteCarlo->GetMonteCarloY(), gigMonteCarlo->GetMonteCarloZ(), gigMonteCarlo->GetLine(), gigMonteCarlo->GetColumn() );
              }

              fListOfPixels[aPlaneNumber-1].push_back(APixel);
            } // end loop on Pixels
            /*
             for( Int_t iMonteCarlo=0; iMonteCarlo<gigEvent->GetNumberOfMonteCarlo(); iMonteCarlo++) { // loop on Pixels

             gigMonteCarlo = gigEvent->GetMonteCarloAt( iMonteCarlo);
             aPlaneNumber = fMatchingPlane[mdt-1][mdl-1][gigMonteCarlo->GetInput()-1][0];
             fListOfMonteCarlo[aPlaneNumber-1].push_back( new DMonteCarlo( aPlaneNumber, gigMonteCarlo->GetMonteCarloX(), gigMonteCarlo->GetMonteCarloY(), gigMonteCarlo->GetMonteCarloZ(),  gigMonteCarlo->GetValue(), gigMonteCarlo->GetLine(), gigMonteCarlo->GetColumn(), 0 ) );  // 0 = timestamp = non def.
             //std::cout<<" DAcq Monte Carlo X = "<<gigMonteCarlo->GetMonteCarloX()<<std::endl;
             } // end loop on Pixels
             */
          }  // End if event pointer correct, JB 2009/05/26
          else {
            dataOK &= kFALSE;
          }

          iModule++;
        } // end loop on each module of this type
        break;


        // -+-+- VME modules
        // JB 2014/05/13
      case 7:
        for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type

          moduleOK = fVME[iModule]->HasData(); // ask for an event
          eventOK &= moduleOK;
          if (fDebugAcq)  cout << " DAcq: getting raw data for module " << iModule << " or " << mdl << " of type " << fc->GetModulePar(mdt).Type << ", OK? " << moduleOK << endl;

          readerEvent = (BoardReaderEvent*)fVME[iModule]->GetEvent(); // get the event
          if( readerEvent ) { // If event pointer correct
            fRealEventNumber = readerEvent->GetEventNumber();
            fTriggersN      += readerEvent->GetNumberOfTriggers();
            //            fFramesN        += readerEvent->GetNumberOfFrames();
            if (ListOfTriggers==NULL) {
              ListOfTriggers = readerEvent->GetTriggers();
            }
            else {
              ListOfTriggers->insert( ListOfTriggers->end(), (readerEvent->GetTriggers())->begin(), (readerEvent->GetTriggers())->end());
              ;
            }
            if (ListOfFrames==NULL) {
              ListOfFrames     = readerEvent->GetFrames();
            }
            else {
              ListOfFrames->insert( ListOfFrames->begin(), (readerEvent->GetFrames())->begin(), (readerEvent->GetFrames())->end());
              ;
            }
            if (fDebugAcq) {
              cout << "   module " << mdl << " found " << readerEvent->GetNumberOfPixels() << " hit pixels  with " << fTriggersN << " triggers: ";
              for( Int_t iTrig=0; iTrig<fTriggersN; iTrig++) {
                cout <<  ", " << ListOfTriggers->at( iTrig);
              }
              cout << " and " << fFramesN << " frames: ";
              for( Int_t iFr=0; iFr<fFramesN; iFr++) {
                cout <<  ", " << ListOfFrames->at( iFr);
              }
              cout << " from daq event " << fRealEventNumber << endl << endl;
            }
            // Set values for hit pixels
            for( Int_t iPix=0; iPix<readerEvent->GetNumberOfPixels(); iPix++) { // loop on Pixels

              readerPixel = (BoardReaderPixel*)readerEvent->GetPixelAt( iPix);
              aPlaneNumber = fMatchingPlane[mdt-1][mdl-1][readerPixel->GetInput()-1][0];
              if(fDebugAcq>2) cout << "  pixel " << iPix << " line " << readerPixel->GetLineNumber() << " column " << readerPixel->GetColumnNumber() << " from input " << readerPixel->GetInput() << " with value " << readerPixel->GetValue() << ", associated to plane " << aPlaneNumber << endl;

              DPixel* APixel = new DPixel( aPlaneNumber, readerPixel->GetLineNumber(), readerPixel->GetColumnNumber(), (Double_t)readerPixel->GetValue());
              //fListOfPixels[aPlaneNumber-1].push_back( new DPixel( aPlaneNumber, readerPixel->GetLineNumber(), readerPixel->GetColumnNumber(), (Double_t)readerPixel->GetValue()));
              fListOfPixels[aPlaneNumber-1].push_back(APixel);

            } // end loop on Pixels

          }  // End if event pointer correct, JB 2009/05/26
          else {
            dataOK &= kFALSE;
          }

          iModule++;
        } // end loop on each module of this type
        break;


        // -+-+- ALI22 modules
        // JB 2014/05/13, 2015/05/25
      case 8:
        for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type

          moduleOK = fALI22[iModule]->HasData(); // ask for an event
          eventOK &= moduleOK;
          if (fDebugAcq)  cout << " DAcq: getting raw data for module " << iModule << " or " << mdl << " of type " << fc->GetModulePar(mdt).Type << ", OK? " << moduleOK << endl;

          readerEvent = (BoardReaderEvent*)fALI22[iModule]->GetEvent(); // get the event
          if( readerEvent ) { // If event pointer correct
            fRealEventNumber = fALI22[iModule]->GetCDHEventCounter();
            //            fTriggersN      += readerEvent->GetNumberOfTriggers();
            //            fFramesN        += readerEvent->GetNumberOfFrames();
            if (fDebugAcq) {
              cout << "   module " << mdl << " found " << readerEvent->GetNumberOfPixels() << " hit pixels";
              cout << " from daq event " << fRealEventNumber << endl << endl;
            }
            // Set values for hit pixels
            // Pass only pixels with a value>0
            for( Int_t iPix=0; iPix<readerEvent->GetNumberOfPixels(); iPix++) { // loop on Pixels

              readerPixel = (BoardReaderPixel*)readerEvent->GetPixelAt( iPix);
              aPlaneNumber = fMatchingPlane[mdt-1][mdl-1][readerPixel->GetInput()-1][0];
              if(fDebugAcq>2) cout << "  pixel " << iPix << " line " << readerPixel->GetLineNumber() << " column " << readerPixel->GetColumnNumber() << " frame " << readerPixel->GetTimeStamp() << " from input " << readerPixel->GetInput() << " with value " << readerPixel->GetValue() << ", associated to plane " << aPlaneNumber << endl;
              if(readerPixel->GetValue()>0) fListOfPixels[aPlaneNumber-1].push_back( new DPixel( aPlaneNumber, readerPixel->GetLineNumber(), readerPixel->GetColumnNumber(), (Double_t)readerPixel->GetValue(), (Int_t)readerPixel->GetTimeStamp()));

            } // end loop on Pixels

          }  // End if event pointer correct, JB 2009/05/26
          else {
            dataOK &= kFALSE;
          }

          iModule++;
        } // end loop on each module of this type
        break;


        // -+-+- DecoderM18 modules
        // JB 2014/05/15
        // modified by INFN, 2014/08/26 to use new DecoderM18 class
      case 9:
        for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type

          // Set OMKDTransition info if this is not the "master" or first Decoder
          // JB 2015/03/27
          if( synchroNeeded && !eventMissed && mdt!=fSynchroFirstM18Decoder ) {
            fM18[iModule]->SetOMKDTransition( fSynchroOMKDTransition );
            fM18[iModule]->SetStopPointerTransition( fSynchroStopPointerTransition );
          }

          moduleOK = fM18[iModule]->ReadData(); // ask for an event
          eventOK &= moduleOK;
          if (fDebugAcq)  cout << " DAcq: getting raw data for module " << iModule << " or " << mdl << " of type " << fc->GetModulePar(mdt).Type << ", OK? " << moduleOK << endl;

          if( moduleOK ) { // If event pointer correct
            fRealEventNumber = fM18[iModule]->Get_EvNumber();
            if (fDebugAcq) {
              cout << "   module " << mdl << " found " << fM18[iModule]->Get_NHitPixel() << " hit pixels";
              cout << " from daq event " << fRealEventNumber << endl << endl;
            }
            // Set values for hit pixels
            // Pass only pixels with a value>0
            for( Int_t iPix=0; iPix<fM18[iModule]->Get_NHitPixel(); iPix++) { // loop on Pixels

              GetMatchingPlaneAndShift( mdt, mdl, 1, 1, aPlaneNumber, aShift);
              //aPlaneNumber = fMatchingPlane[mdt-1][mdl-1][0][0];
              if(fDebugAcq>2) cout << "  pixel " << iPix << " index " << fM18[iModule]->GetIndex( iPix) << " line " << fM18[iModule]->GetRow( iPix) << " column " << fM18[iModule]->GetCol( iPix) << " from input " << 0 << " with value " << fM18[iModule]->GetAmp( iPix) << ", associated to plane " << aPlaneNumber << " with an index shift of " << aShift << endl;

              if( (Double_t)fM18[iModule]->GetAmp( iPix)>0 ) { // cut tails //!!!!scommentare

                //fListOfPixels[aPlaneNumber-1].push_back( new DPixel( aPlaneNumber, fM18[iModule]->GetIndex(iPix)+aShift+1, (Double_t)fM18[iModule]->GetAmp( iPix)) );//added +1 in shift 17/6
                DPixel* APixel =  new DPixel( aPlaneNumber, fM18[iModule]->GetIndex(iPix)+aShift+1, /*(Double_t)fM18[iModule]->GetAmp( iPix)*/TMath::Abs((Double_t)fM18[iModule]->GetAmp( iPix)) ); //added +1 in shift 17/6
                fListOfPixels[aPlaneNumber-1].push_back(APixel);
              } // end cut tails

            } // end loop on Pixels

            // if synchro is required, read OMKDTransition info
            //  BUT store it only from the first module
            // JB 2015/03/27
            if( synchroNeeded && mdt==fSynchroFirstM18Decoder ) {
              Int_t OMKDinfo = fM18[iModule]->GetOMKDTransition();
              Int_t StopPointerinfo = fM18[iModule]->GetStopPointerTransition();
              if( fDebugAcq>1 ) {
                cout << " ==> Synchronization info found for module " << iModule;
                cout << " OMKDTransition = " <<  OMKDinfo << endl;
                cout << " StopPointerTransition = " <<  StopPointerinfo << endl;
              }
              if ( iModule==0 ){
                fSynchroOMKDTransition = OMKDinfo;
                fSynchroStopPointerTransition = StopPointerinfo;

              }
            }
	    }  // End if event pointer correct, JB 2009/05/26

            else {
            // set eventMissed flag if synchro was required
            // JB 2015/03/27
            if( synchroNeeded ) {
              eventMissed |= kTRUE;
              cout << "WARNING: Acquisition missed the synchronization of event " << fRealEventNumber << " in DecoderM18." << endl;
              fEventsMissed++;
            }
            dataOK &= kFALSE;
          }

          iModule++;
        } // end loop on each module of this type
        break;


        // -+-+- DecoderGeant modules
        //
      case 10:
        for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type

          moduleOK = fGeant[iModule]->ReadData(); // ask for an event
          eventOK &= moduleOK;
          if (fDebugAcq)  cout << " DAcq: getting raw data for module " << iModule << " or " << mdl << " of type " << fc->GetModulePar(mdt).Type << ", OK? " << moduleOK << endl;

          if( moduleOK ) { // If event pointer correct
            fRealEventNumber = fGeant[iModule]->Get_EvNumber();
            if (fDebugAcq) {
              cout << "   module " << mdl << " found " << fGeant[iModule]->Get_NHitPixel() << " hit pixels";
              cout << " from daq event " << fRealEventNumber << endl << endl;
            }
            // Set values for hit pixels
            // Pass only pixels with a value>0
            for( Int_t iPix=0; iPix<fGeant[iModule]->Get_NHitPixel(); iPix++) { // loop on Pixels

              GetMatchingPlaneAndShift( mdt, mdl, 1, 1, aPlaneNumber, aShift);
              //aPlaneNumber = fMatchingPlane[mdt-1][mdl-1][0][0];
              if(fDebugAcq>2) cout << "  pixel " << iPix << " index " << fGeant[iModule]->GetIndex( iPix) << " line " << fGeant[iModule]->GetRow( iPix) << " column " << fGeant[iModule]->GetCol( iPix) << " from input " << 0 << " with value " << fGeant[iModule]->GetAmp( iPix) << ", associated to plane " << aPlaneNumber << endl;
              if((Double_t)fGeant[iModule]->GetAmp( iPix)>0) { // cut tails
                //                fListOfPixels[aPlaneNumber-1].push_back( new DPixel( aPlaneNumber, fGeant[iModule]->GetIndex(iPix), (Double_t)fGeant[iModule]->GetAmp( iPix)) );
                //		  fListOfPixels[aPlaneNumber-1].push_back( new DPixel( aPlaneNumber, fGeant[iModule]->GetRow( iPix), fGeant[iModule]->GetCol(iPix), (Double_t)fGeant[iModule]->GetAmp( iPix)) );
                DPixel* APixel =  new DPixel( aPlaneNumber, fGeant[iModule]->GetRow( iPix), fGeant[iModule]->GetCol(iPix), (Double_t)fGeant[iModule]->GetAmp( iPix));
                fListOfPixels[aPlaneNumber-1].push_back(APixel);
              } // end cut tails

            } // end loop on Pixels

          }  // End if event pointer correct, JB 2009/05/26
          else {
            dataOK &= kFALSE;
          }

          iModule++;
        } // end loop on each module of this type
        break;

	// -+-+- MC modules
        // AP 2016/04/15
      case 11:
        for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type

          moduleOK = fMC[iModule]->HasData(); // ask for an event
          eventOK &= moduleOK;
          if (fDebugAcq)  cout << " DAcq: getting raw data for module " << iModule << " or " << mdl << " of type " << fc->GetModulePar(mdt).Type << ", OK? " << moduleOK << endl;

          readerEvent = (BoardReaderEvent*)fMC[iModule]->GetEvent(); // get the event
          if( readerEvent ) { // If event pointer correct
            fRealEventNumber = readerEvent->GetEventNumber();
            fTriggersN      += readerEvent->GetNumberOfTriggers();
            //fFramesN        += readerEvent->GetNumberOfFrames();

            if(ListOfTriggers == NULL) ListOfTriggers = readerEvent->GetTriggers();
            else                       ListOfTriggers->insert( ListOfTriggers->end(), (readerEvent->GetTriggers())->begin(), (readerEvent->GetTriggers())->end());

            if(ListOfFrames == NULL)   ListOfFrames = readerEvent->GetFrames();
            else                       ListOfFrames->insert( ListOfFrames->begin(), (readerEvent->GetFrames())->begin(), (readerEvent->GetFrames())->end());

            if(fDebugAcq) {
              cout << "   module " << mdl << " found " << readerEvent->GetNumberOfPixels() << " hit pixels  with " << fTriggersN << " triggers: ";
              for( Int_t iTrig=0; iTrig<fTriggersN; iTrig++) cout <<  ", " << ListOfTriggers->at( iTrig);
              cout << " and " << fFramesN << " frames: ";
              for( Int_t iFr=0; iFr<fFramesN; iFr++) cout <<  ", " << ListOfFrames->at( iFr);
              cout << " from daq event " << fRealEventNumber << endl << endl;
            }

            if(fDebugAcq) {
	      cout << endl;
	      cout << "Got it this event " << MCInfoHolder->GetNSimParticles() << " sim-particles" << endl;
	      for(int ipart=0;ipart<MCInfoHolder->GetNSimParticles();ipart++) {
		cout << "  This particle of frame " << MCInfoHolder->GetASimParticle(ipart).FrameNumber << " has " << MCInfoHolder->GetASimParticle(ipart).NHits << " hits" << endl;
		for(int ihit=0;ihit<MCInfoHolder->GetASimParticle(ipart).NHits;ihit++) {
		  int HitIdx = MCInfoHolder->GetASimParticle(ipart).FirstHitIdx + ihit;
		  cout << "    This hit has " << MCInfoHolder->GetASimHit(HitIdx).Npixels << " pixels" << endl;
	        }
	      }
	      cout << "Total sim-hits   = " << MCInfoHolder->GetNSimHits()   << endl;
	      cout << "Total sim-pixels = " << MCInfoHolder->GetNSimPixels() << endl;
	      int NNoisePixels = 0;
	      for(int ipix=0;ipix<MCInfoHolder->GetNSimPixels();ipix++) {
		if(MCInfoHolder->GetASimPixel(ipix).HitIdx == -1) NNoisePixels++;
	      }
	      cout << "Noise sim-pixels = " << NNoisePixels << endl;
	      cout << endl;
	    }

            // Set values for hit pixels
            //New method to includ the index of the hit turning on this pixel from the MC info
            //Index is -1 if pixel is turned on by noise
            for( Int_t iPix=0; iPix < MCInfoHolder->GetNSimPixels(); iPix++) { // loop on Pixels
	      aPlaneNumber = MCInfoHolder->GetASimPixel(iPix).sensorID+1;
	      Double_t Value = 1.0;
	      if(fc->GetPlanePar(aPlaneNumber).AnalysisMode == 2) Value = MCInfoHolder->GetASimPixel(iPix).ChargeAnalog;
	      if(fDebugAcq>2) cout << " pixel " << iPix
		                   << " line " << MCInfoHolder->GetASimPixel(iPix).row
		                   << " column " << MCInfoHolder->GetASimPixel(iPix).col
				   << " from input " << MCInfoHolder->GetASimPixel(iPix).sensorID
				   << " with value " << Value
				   << ", associated to plane " << aPlaneNumber
				   << endl;
	      DPixel* APixel = new DPixel(aPlaneNumber,
					  MCInfoHolder->GetASimPixel(iPix).row,
					  MCInfoHolder->GetASimPixel(iPix).col,
					  Value);   //The value. Suppose digital readout
	      APixel->SetPixelMCHitIdx(MCInfoHolder->GetASimPixel(iPix).HitIdx); //Set the MC hit index for this pixel
              fListOfPixels[aPlaneNumber-1].push_back(APixel);

	    } // end loop on Pixels

            /*
            for( Int_t iPix=0; iPix<readerEvent->GetNumberOfPixels(); iPix++) { // loop on Pixels
              readerPixel = (BoardReaderPixel*)readerEvent->GetPixelAt(iPix);
              aPlaneNumber = fMatchingPlane[mdt-1][mdl-1][readerPixel->GetInput()-1][0];
              if(fDebugAcq>2) cout << "  pixel " << iPix
		                   << " line " << readerPixel->GetLineNumber()
		                   << " column " << readerPixel->GetColumnNumber()
				   << " from input " << readerPixel->GetInput()
				   << " with value " << readerPixel->GetValue()
				   << ", associated to plane " << aPlaneNumber
				   << endl;

              DPixel* APixel = new DPixel( aPlaneNumber, readerPixel->GetLineNumber(), readerPixel->GetColumnNumber(), (Double_t)readerPixel->GetValue());
              fListOfPixels[aPlaneNumber-1].push_back(APixel);
            } // end loop on Pixels
            */

          }  // End if event pointer correct, JB 2009/05/26
          else {
            dataOK &= kFALSE;
          }

          iModule++;
        } // end loop on each module of this type
        break;


        // -+-+- IHEP modules
        // JB 2018/10/09
      case 12:
        for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type

          moduleOK = fIHEP[iModule]->HasData(); // ask for an event
          eventOK &= moduleOK;
          if (fDebugAcq)  cout << " DAcq: getting raw data for module " << iModule << " or " << mdl << " of type " << fc->GetModulePar(mdt).Type << ", OK? " << moduleOK << endl;

          readerEvent = (BoardReaderEvent*)fIHEP[iModule]->GetEvent(); // get the event
          if( readerEvent ) { // If event pointer correct
            fRealEventNumber = readerEvent->GetEventNumber();
            fTriggersN      += readerEvent->GetNumberOfTriggers();
            fFramesN        += readerEvent->GetNumberOfFrames();
            if (ListOfTriggers==NULL) {
              ListOfTriggers = readerEvent->GetTriggers();
            }
            else {
              ListOfTriggers->insert( ListOfTriggers->end(), (readerEvent->GetTriggers())->begin(), (readerEvent->GetTriggers())->end());
              ;
            }
            if (ListOfFrames==NULL) {
              ListOfFrames     = readerEvent->GetFrames();
            }
            else {
              ListOfFrames->insert( ListOfFrames->begin(), (readerEvent->GetFrames())->begin(), (readerEvent->GetFrames())->end());
              ;
            }
            if (fDebugAcq) {
              cout << "   module " << mdl << " found " << readerEvent->GetNumberOfPixels() << " hit pixels  with " << fTriggersN << " triggers: ";
              for( Int_t iTrig=0; iTrig<fTriggersN; iTrig++) {
                cout <<  ", " << ListOfTriggers->at( iTrig);
              }
              cout << " and " << fFramesN << " frames: ";
              for( Int_t iFr=0; iFr<fFramesN; iFr++) {
                cout <<  ", " << ListOfFrames->at( iFr);
              }
              cout << " from daq event " << fRealEventNumber << endl << endl;
            }
            // Set values for hit pixels
            for( Int_t iPix=0; iPix<readerEvent->GetNumberOfPixels(); iPix++) { // loop on Pixels

              readerPixel = (BoardReaderPixel*)readerEvent->GetPixelAt( iPix);
              aPlaneNumber = fMatchingPlane[mdt-1][mdl-1][readerPixel->GetInput()-1][0];
              if(fDebugAcq>2) cout << "  pixel " << iPix << " line " << readerPixel->GetLineNumber() << " column " << readerPixel->GetColumnNumber() << " at timestamp " << readerPixel->GetTimeStamp() << " from input " << readerPixel->GetInput() << " with value " << readerPixel->GetValue() << ", associated to plane " << aPlaneNumber << endl;

              DPixel* APixel = new DPixel( aPlaneNumber, readerPixel->GetLineNumber(), readerPixel->GetColumnNumber(), (Double_t)readerPixel->GetValue(), readerPixel->GetTimeStamp());
              // if(readerPixel->GetInput()!=5 && readerPixel->GetInput()!=6 && readerPixel->GetTimeStamp()==1)
              fListOfPixels[aPlaneNumber-1].push_back(APixel);

            } // end loop on Pixels

          }  // End if event pointer correct
          else {
            dataOK &= kFALSE;
          }

          iModule++;
        } // end loop on each module of IHEP type
        break;


        // -+-+- MIMOSIS modules
        // JB 2021/05/01
      case 13:
        for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type

          moduleOK = fMSIS[iModule]->HasData(); // ask for an event
          eventOK &= moduleOK;
          if (fDebugAcq)  cout << " DAcq: getting raw data for module " << iModule << " or " << mdl << " of type " << fc->GetModulePar(mdt).Type << ", OK? " << moduleOK << endl;

          readerEvent = (BoardReaderEvent*)fMSIS[iModule]->GetEvent(); // get the event
          if( readerEvent ) { // If event pointer correct
            fRealEventNumber = readerEvent->GetEventNumber();
            fTriggersN      += readerEvent->GetNumberOfTriggers();
            fFramesN        += readerEvent->GetNumberOfFrames();
            if (ListOfTriggers==NULL) {
              ListOfTriggers = readerEvent->GetTriggers();
            }
            else {
              ListOfTriggers->insert( ListOfTriggers->end(), (readerEvent->GetTriggers())->begin(), (readerEvent->GetTriggers())->end());
              ;
            }
            if (ListOfFrames==NULL) {
              ListOfFrames     = readerEvent->GetFrames();
            }
            else {
              ListOfFrames->insert( ListOfFrames->begin(), (readerEvent->GetFrames())->begin(), (readerEvent->GetFrames())->end());
              ;
            }
            if (fDebugAcq) {
              cout << "   module " << mdl << " found " << readerEvent->GetNumberOfPixels() << " hit pixels  with " << fTriggersN << " triggers: ";
              for( Int_t iTrig=0; iTrig<fTriggersN; iTrig++) {
                cout <<  ", " << ListOfTriggers->at( iTrig);
              }
              cout << " and " << fFramesN << " frames: ";
              for( Int_t iFr=0; iFr<fFramesN; iFr++) {
                cout <<  ", " << ListOfFrames->at( iFr);
              }
              cout << " from daq event " << fRealEventNumber << endl << endl;
            }
            // Set values for hit pixels
            for( Int_t iPix=0; iPix<readerEvent->GetNumberOfPixels(); iPix++) { // loop on Pixels
              readerPixel = (BoardReaderPixel*)readerEvent->GetPixelAt( iPix);
	              cout << " Got pixel " << iPix << " for input " << readerPixel->GetInput() << endl;
              aPlaneNumber = fMatchingPlane[mdt-1][mdl-1][readerPixel->GetInput()-1][0];
              if(fDebugAcq>2) cout << "  pixel " << iPix << " line " << readerPixel->GetLineNumber() << " column " << readerPixel->GetColumnNumber() << " at timestamp " << readerPixel->GetTimeStamp() << " from input " << readerPixel->GetInput() << " with value " << readerPixel->GetValue() << ", associated to plane " << aPlaneNumber << endl;
              DPixel* APixel = new DPixel( aPlaneNumber, readerPixel->GetLineNumber(), readerPixel->GetColumnNumber(), (Double_t)readerPixel->GetValue(), readerPixel->GetTimeStamp());
              // if(readerPixel->GetInput()!=5 && readerPixel->GetInput()!=6 && readerPixel->GetTimeStamp()==1)
              fListOfPixels[aPlaneNumber-1].push_back(APixel);
            } // end loop on Pixels

          }  // End if event pointer correct
          else {
            dataOK &= kFALSE;
          }

          iModule++;
        } // end loop on each module of MIMSOSIS type
        break;


    }; // end switch on module types

    // Interrupt the loop on modules if one event data is not OK
    // JB 2012/08/18 -> undesired behavior JB 2015/03/25
    //if( !dataOK ) {
    //  break;
    //}

  } // end loop on module types


  //Subtracting hot pixels from hit pixels list
  for(int iPlane=0;iPlane<fc->GetTrackerPar().Planes;iPlane++) { // Loop on planes

    if(   fc->GetPlanePar(iPlane+1).HotPixelList_lin.size() == 0
       && fc->GetPlanePar(iPlane+1).HotPixelList_index.size() == 0) {
      if (fDebugAcq>2) cout << "DAcq: no hot pixel to subtract." << endl;
      continue;
    }

    if (fDebugAcq>1) cout << "DAcq: Subtracting hot pixels, from a list of " << int(fc->GetPlanePar(iPlane+1).HotPixelList_lin.size()) << " ordered in 2D, or from a list of " << int(fc->GetPlanePar(iPlane+1).HotPixelList_index.size()) << " ordered in 1D, from plane " << iPlane+1 << " with " << int(fListOfPixels[iPlane].size()) << " fired pixels." << endl;

    //temporal list with hit pixels excluding hot-pixels
   std::vector<DPixel*> Temp_list;
    Temp_list.clear();
    for(int iPix=0;iPix<int(fListOfPixels[iPlane].size());iPix++) { // loop on read pixels
      //Test if current pixel is in hot-pixel-list:
      bool IsAHotPixel = false;
      for(int i_hot = 0; i_hot<int(fc->GetPlanePar(iPlane+1).HotPixelList_lin.size());i_hot++) { // loop on 2D ordered hot pixel list
        if (fDebugAcq>2) printf("DAcq: 2D-testing pixel[%d] (%d,%d) against known hot pixel[%d] (%d,%d)\n", iPix, fListOfPixels[iPlane][iPix]->GetPixelLine(), fListOfPixels[iPlane][iPix]->GetPixelColumn(), i_hot, fc->GetPlanePar(iPlane+1).HotPixelList_lin[i_hot], fc->GetPlanePar(iPlane+1).HotPixelList_col[i_hot] );
        if((fListOfPixels[iPlane][iPix]->GetPixelLine()   == fc->GetPlanePar(iPlane+1).HotPixelList_lin[i_hot])
           &&
           (fListOfPixels[iPlane][iPix]->GetPixelColumn() == fc->GetPlanePar(iPlane+1).HotPixelList_col[i_hot])) {
          IsAHotPixel = true;
          if (fDebugAcq>2) printf( "    hot pixel found at row %d, col %d\n", fListOfPixels[iPlane][iPix]->GetPixelLine(), fListOfPixels[iPlane][iPix]->GetPixelColumn() );
          break;
        }
      }  // end loop on 2D ordered hot pixel list

      for(int i_hot = 0; i_hot<int(fc->GetPlanePar(iPlane+1).HotPixelList_index.size());i_hot++) { // loop on 1D ordered hot pixel list
        if (fDebugAcq>2) printf("DAcq: 1D-testing pixel[%d] (%d) against known hot pixel[%d] (%d)\n", iPix, fListOfPixels[iPlane][iPix]->GetDAQIndex(), i_hot, fc->GetPlanePar(iPlane+1).HotPixelList_index[i_hot] );
        if( fListOfPixels[iPlane][iPix]->GetDAQIndex() == fc->GetPlanePar(iPlane+1).HotPixelList_index[i_hot] ) {
          IsAHotPixel = true;
          if (fDebugAcq>2) printf( "    hot pixel found at index %d\n", fListOfPixels[iPlane][iPix]->GetDAQIndex() );
          break;
        }
      }  // end loop on 1D ordered hot pixel list

      if(!IsAHotPixel) Temp_list.push_back(fListOfPixels[iPlane][iPix]);
    } // end loop on read pixels

    fListOfPixels[iPlane].clear();
    for(int iPix=0;iPix<int(Temp_list.size());iPix++) {
      fListOfPixels[iPlane].push_back(Temp_list[iPix]);
    }
    Temp_list.clear();

  } // end Loop on planes


  /*
   //====================
   // In case something went wrong, skip the next event, JB 2009/05/26
   if( !eventOK ) {
   if (fDebugAcq)  cout << " DAcq: we need to skip the next event:" << endl;
   for ( Int_t mdt = 1; mdt <= fModuleTypes; mdt++){ // loop on module types

   switch (fc->GetModulePar(mdt).Type) {

   case 3:// -+-+- TNT modules
   for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
   fTNT[iModule]->SkipNextEvent();
   } // end loop on each module of this type
   break;

   }; // end switch on module types

   } // end loop on module types

   } //end if something went wrong
   */

  // Update some counters
  // JB 2014/12/16
  if( !moduleOK ) fEventsModuleNotOK++;
  if( !dataOK ) fEventsDataNotOK++;

  if (fDebugAcq) {
    cout << "Over all modules, DAQ found: " << fTriggersN << " triggers, " << fTimestampsN << " timestamps, " << fFramesN << " frames from daq event " << fRealEventNumber << endl << endl;

    cout << " DAcq: getting event done, OK? " << eventOK << " and missed event? " << eventMissed << " and data OK? " << dataOK << endl;

    cout << "      so far: " << fEventNumber << " events read, wrong ones with: module pb = " << fEventsModuleNotOK << ", data pb = " << fEventsDataNotOK << " and missed synchro = " << fEventsMissed << endl;
  }

  // multi-bit return code, SS 2012/08/10
  DAcqResult->SetBitNumber(0,eventOK);
  DAcqResult->SetBitNumber(1,eventMissed);
  DAcqResult->SetBitNumber(2,dataOK);
  return DAcqResult;

}

//______________________________________________________________________________
//
Bool_t DAcq::DumpHexToTerm()
{
  // print Event Buffer sedezimal (hexadecimal)

  //  Int_t i = 0;

  cout << setbase(16) << endl;

//   cout << " DAcq: BufferLength: " << fRawEventBufferLength  << endl;
//   while(i <= GetRawEventBufferLength()){
//     if ((i == 0) ||
// 	(i % 0x10) == 0)
//       cout << endl << i << ": ";
//     cout << theValue(fRawEventStart + i,1) << " ";
//     i++;
//   }

  cout << "******************" << endl;
  cout << setbase(10) << endl;
  cout << "******************" << endl;
  return kTRUE;
}

//______________________________________________________________________________
//
void DAcq::PrintStatistics(ostream &stream)
{
  // print Statistics for each acquisition module
  //
  // JB 2009/09/09
  // Last modified JB 2010/08/23 to use modulteType/10
  // Modified SS 2011/12/14 output stream can be set from outside
  // Modified JB 2012/08/17 printout nb of events (total and missed)
  // Modified JB 2014/12/16 printout nb of events with module or data pb

  Int_t iModule=0; // module index, from 0 to totalNmodules
  for (Int_t mdt = 1; mdt <= fModuleTypes; mdt++){ // loop on module types

    switch ( (fc->GetModulePar(mdt).Type)/10 ) {

      // -+-+- IMG modules
      case 1:

      for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
        fIMG[iModule]->PrintStatistics(stream);
        iModule++;
      } // end loop on each modules of this type
      break;



      // -+-+- TNT modules
      case 3:

      for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
        fTNT[iModule]->PrintStatistics(stream); // JB 2010/08/26
        iModule++;
      } // end loop on each modules of this type
      break;



      // -+-+- PXI modules
      case 4:

      for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
        fPXI[iModule]->PrintStatistics(stream);
        iModule++;
      } // end loop on each modules of this type
      break;



      // -+-+- PXIe modules
      // JB 2011/06/21
      // ZE 2021/05/25
            /*
      case 5:

      for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
        fPXIe[iModule]->PrintStatistics(stream);
        iModule++;
      } // end loop on each modules of this type
      break;

*/

      // -+-+- GIG modules
      case 6:

      for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
        fGIG[iModule]->PrintStatistics(stream);
        iModule++;
      } // end loop on each modules of this type
      break;


      // -+-+- VME modules
      case 7:

      for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
        fVME[iModule]->PrintStatistics(stream);
        iModule++;
      } // end loop on each modules of this type
      break;


      // -+-+- ALI22 modules
      case 8:

      for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
        fALI22[iModule]->PrintStatistics(stream);
        iModule++;
      } // end loop on each modules of this type
      break;


      // -+-+- DecoderM18 modules
      case 9:

      for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
        fM18[iModule]->PrintStatistics(stream);
        iModule++;
      } // end loop on each modules of this type
      break;


      // -+-+- DecoderGeant
      case 10:

      for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
        fGeant[iModule]->PrintStatistics(stream);
        iModule++;
      } // end loop on each modules of this type
      break;

      // -+-+- MC modules
      case 11:

      for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
        fMC[iModule]->PrintStatistics(stream);
        iModule++;
      } // end loop on each modules of this type
      break;


      // -+-+- IHEP modules
      case 12:

      for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
        fIHEP[iModule]->PrintStatistics(stream);
        iModule++;
      } // end loop on each modules of this type
      break;


      // -+-+- MIMOSIS modules
      case 13:

      for (Int_t mdl = 1; mdl <= fc->GetModulePar(mdt).Devices; mdl++){ // loop on each modules of this type
        fMSIS[iModule]->PrintStatistics(stream);
        iModule++;
      } // end loop on each modules of this type
      break;


      // -+-+- Other modules
      default:
      stream << "WARNING: DAcq, unknown module type " << fc->GetModulePar(mdt).Type << "!" << endl;

    };


  } // end loop on module types

  stream << "DAcq: Number of events read: " << fEventNumber << "." << endl;
  stream << "DAcq: Number of events with synchro missed: " << fEventsMissed << "." << endl;
  stream << "DAcq: Number of events with data pb: " << fEventsDataNotOK << "." << endl;  // JB 2014/12/16
  stream << "DAcq: Number of events with module pb: " << fEventsModuleNotOK << "." << endl;// JB 2014/12/16

}

//______________________________________________________________________________
//
void DAcq::DumpSynchroInfo( Int_t nEvents)
{
  // Print the information on synchronisation in a file
  //  for the required nb of events
  //
  // JB 2013/08/20

  if( nEvents<0 ) nEvents = fNbSynchroInfo;

  if( fSynchroInfo != NULL ) {

    Char_t fileName[300];
    sprintf( fileName, "Results/%d/synchroInfo_dump.txt", fRunNumber);
    sprintf( fileName,"%s", fTool.LocalizeDirName( fileName));
    FILE *outFile;
    outFile = fopen( fileName, "w");

    APP__TSyncIndexRec* pointer2Event = (APP__TSyncIndexRec*)fSynchroInfo;

    if(fDebugAcq) printf( " line  EventId  AcqId  FrameId  EventTag\n");
    fprintf( outFile, " line  EventId  AcqId  FrameId  EventTag\n");
    for ( Int_t iEvt=0; iEvt<nEvents && iEvt<fNbSynchroInfo; iEvt++) {
      fprintf( outFile,  "%d  %d  %d  %d  %d\n", iEvt, (int)pointer2Event[iEvt].DutEvId, pointer2Event[iEvt].TelTrigAcqId, pointer2Event[iEvt].TelTrigFrIdInAcq, pointer2Event[iEvt].TelTrigEvTag);
      if(fDebugAcq) printf( "%d  %d  %d  %d  %d\n", iEvt, (int)pointer2Event[iEvt].DutEvId, pointer2Event[iEvt].TelTrigAcqId, pointer2Event[iEvt].TelTrigFrIdInAcq, pointer2Event[iEvt].TelTrigEvTag);
    }

    fclose( outFile);
    cout << endl << "List of synchronization information written in " << fileName << endl;

  }
  else {
    cout << "WARNING DAcq::GetSynchroInfo(), synchronization information has not been initialized properly." << endl;
  }


}
