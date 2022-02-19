/////////////////////////////////////////////////////////////
// Class Description of MCBoardReader                      //
//                                                         //
// Dedicated to decode MC n-tuples (TTree)                 //
//                                                         //
// Originally written by Alejandro Perez                   //
//  Class based on VMEBoardReader                          //
//                                                         //
// Expects one file per channel, one channel per sensor    //
//                                                         //
/////////////////////////////////////////////////////////////
//
// created JB, 2014/05/12

#include "MCBoardReader.h"
#include "TFile.h"
#include "TSystem.h"
#include "TCanvas.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <math.h>
#include <iomanip>

ClassImp(MCBoardReader);

//------------------------------------------+-----------------------------------
//! Default constructor.
MCBoardReader::MCBoardReader(int boardNumber, int Runnumber, TString pathName, TString fileName, TString TreeName, DSetup* aSetup)
{

  GotNonSensitiveInfo = false;
  fDisplay = false;
  fDebugLevel = 0;
  
  fBoardNumber      = boardNumber;
  fRunNumber        = Runnumber;
  fPathName         = pathName;
  fFileName         = fileName;
  fTreeName         = TreeName;
  fSetup            = aSetup;
  fNumberOfSensors  = fSetup->GetTrackerPar().Planes;
  ListOfInputROOTFiles.clear();
  
  cout << "*****************************************" << endl;
  cout << "    < MCBoardReader constructor >        " << endl; 
  cout << "*****************************************" << endl;  
  cout << "Creating a MCBoardReader" << endl;
  cout << " * for board         : " << fBoardNumber << endl;
  cout << " * DataPath          : " << fPathName.Data() << endl;
  if(fFileName.EndsWith(".root")) {
    TString TheFullFileName = fPathName + TString("/") +  fFileName;
    ListOfInputROOTFiles.push_back(TheFullFileName);
    cout << " * FileName          : " << fFileName.Data() << endl;
  }
  else if(fFileName.EndsWith(".txt")) {
    TString TheFullFileName = fPathName + TString("/") + fFileName;
    char ccc[500];
    ifstream fp(TheFullFileName.Data());
    cout << TheFullFileName.Data() << endl;
    while(fp >> ccc) {
      TString Tmp_file_name = fPathName + TString("/") +  TString(ccc);
      ListOfInputROOTFiles.push_back(Tmp_file_name);
    }
    fp.close();
    
    cout << " * FileName          : " << fFileName.Data() << " is a list of root files with contents:" << endl;
    for(int ifile=0;ifile<int(ListOfInputROOTFiles.size());ifile++) {
      cout << "   - " << ListOfInputROOTFiles[ifile].Data() << endl;
    }
    
  }
  else {
    cout << endl;
    cout << "MCBoardReader::ERROR: The File extension is not either .root or .txt (file list). Check your inputs. Extiting now!!!" << endl;
    cout << endl;
    assert(false);
  }
  cout << " * TreeName          : " << fTreeName.Data() << endl;
  cout << " * nb of sensors     : " << fNumberOfSensors << endl;
  
  fFrameCounter       = 0;
  fEventNumber        = 0;
  fCurrentTriggerCnt  = 0;
  fEventsOverflow     = 0;
  fFramesReadFromFile = 0;
  fCurrentEvent       = NULL;
  MCInfoHolder        = NULL;
  
  if(!Open()) {
    if(fFileName.EndsWith(".root")) {
      TString TheFullFileName = fPathName + fFileName;
      cout << endl;
      cout << "Problem loading TTree with name " << fTreeName.Data() << " from file " << TheFullFileName.Data() << ". Check your inputs. Exiting now!!!" << endl;
      cout << endl;
      assert(false);
    }
    else if(fFileName.EndsWith(".txt")) {
      TString TheFullFileName = fPathName + fFileName;
      cout << endl;
      cout << "Problem loading TTree with name " << fTreeName.Data() << " from file list " << TheFullFileName.Data() << ". Check your inputs. Exiting now!!!" << endl;
      cout << endl;
      assert(false);
    }
  }
  
  double R_LineOverFlow[2];
  hLineOverFlowDist = new TH1F*[fNumberOfSensors];
  for(int iSensor=0; iSensor<fNumberOfSensors; iSensor++) {
    int fNumberOfRows = fSetup->GetPlanePar(iSensor+1).Strips(1);
    R_LineOverFlow[0] = 0.0 - 0.5;
    R_LineOverFlow[1] = fNumberOfRows - 0.5;
    
    TString HistName  = TString("hLineOverFlowDist_Sensor") + long(iSensor+1);
    TString HistTitle = TString("Number of overflow vs line number for sensor #") + long(iSensor+1);
    hLineOverFlowDist[iSensor] = new TH1F(HistName.Data(),
                                          HistTitle.Data(),
					  fNumberOfRows,
					  R_LineOverFlow[0],
					  R_LineOverFlow[1]);
    hLineOverFlowDist[iSensor]->SetXTitle("Line Number");
    hLineOverFlowDist[iSensor]->GetXaxis()->CenterTitle(true);
    hLineOverFlowDist[iSensor]->SetYTitle("Number of Frames");
    hLineOverFlowDist[iSensor]->GetYaxis()->CenterTitle(true);
    hLineOverFlowDist[iSensor]->SetLineColor(4);
    hLineOverFlowDist[iSensor]->SetLineWidth(2);
  }

  return;
  
}
//------------------------------------------+-----------------------------------
//! Destructor.
MCBoardReader::~MCBoardReader()
{
  
  TheListOfMCParticles.clear();
  ListOfNoisePixels.clear();
  TheListOfMCNonSensitiveParticles.clear();
  
}
//------------------------------------------+-----------------------------------
bool MCBoardReader::Open()
{
  
  // Open the root file and extract the n-tuple (TTree) with all the MC information
  // Return false if one file cannot be opened or cannot retrieve the TTree with name fTreeName, true otherwise.
  
  //fDebugLevel = 1;
  
  fChain = new TChain(fTreeName.Data());
  TTree* tree = NULL;
  
  GotNonSensitiveInfo = false;
  for(int ifile=0;ifile<int(ListOfInputROOTFiles.size());ifile++) {
    tree = NULL;
    TFile file_in(ListOfInputROOTFiles[ifile].Data());
    tree = (TTree*)file_in.Get(fTreeName.Data());
    
    if(tree != NULL) {
      bool ParatialTest = CheckIfNonSensitiveVariables(tree);
      if(GotNonSensitiveInfo && !ParatialTest) {
	GotNonSensitiveInfo = false;
	break;
      }
      if(!GotNonSensitiveInfo && ParatialTest) GotNonSensitiveInfo = true;
    }
  }
  
  if(GotNonSensitiveInfo) {
    cout << endl;
    cout << "MCBoardReader::   all the trees in list had the NonSensitive variables. Loading them up!!!" << endl;
    cout << endl;
  }
  else {
    cout << endl;
    cout << "MCBoardReader::   at least one of the trees in list didn't have the NonSensitive variables. Variables not available!!!" << endl;
    cout << endl;
  }
  
  for(int ifile=0;ifile<int(ListOfInputROOTFiles.size());ifile++) {
    tree = NULL;
    TFile file_in(ListOfInputROOTFiles[ifile].Data());
    tree = (TTree*)file_in.Get(fTreeName.Data());
    
    if(tree != NULL) {
      cout << "MCBoardReader::   Adding " << ListOfInputROOTFiles[ifile].Data() << " file to Chaine fChain" << endl;
      Init(ListOfInputROOTFiles[ifile].Data(),ifile+1);      
    } else {
      cout << endl;
      cout << "MCBoardReader::Error::   " << fTreeName.Data() << " tree from " << ListOfInputROOTFiles[ifile].Data() << " file doesn't exist. Exiting now." << endl;
      cout << endl;
      assert(false);
    }
    
  }
  
  if(fDebugLevel) fChain->Print();

  return true;
  
}
//------------------------------------------+-----------------------------------
void MCBoardReader::Init(const char* ROOTfilename, const int num)
{
  
  // Set branch addresses and branch pointers
  fChain->Add(ROOTfilename);
  fCurrent = -1;
  fChain->SetMakeClass(1);

  if(num == 1) {
    fChain->SetBranchAddress("ParticleNb",            &ParticleNb);
    fChain->SetBranchAddress("ParticleBKGType",        ParticleBKGType);
    fChain->SetBranchAddress("ParticlepdgID",          ParticlepdgID);
    fChain->SetBranchAddress("ParticleTrkVtX",         ParticleTrkVtX);
    fChain->SetBranchAddress("ParticleTrkVtY",         ParticleTrkVtY);
    fChain->SetBranchAddress("ParticleTrkVtZ",         ParticleTrkVtZ);
    fChain->SetBranchAddress("ParticleNHits",          ParticleNHits);
    fChain->SetBranchAddress("ParticleFirstHitIdx",    ParticleFirstHitIdx);
  
    fChain->SetBranchAddress("HitNb",                 &HitNb);
    fChain->SetBranchAddress("HitParticleIdx",         HitParticleIdx);
    fChain->SetBranchAddress("HitsensorID",            HitsensorID);
    fChain->SetBranchAddress("HitladderID",            HitladderID);
    fChain->SetBranchAddress("HitmoduleID",            HitmoduleID);
    fChain->SetBranchAddress("HitposINmmX",            HitposINmmX);
    fChain->SetBranchAddress("HitposINmmY",            HitposINmmY);
    fChain->SetBranchAddress("HitposINmmZ",            HitposINmmZ);
    fChain->SetBranchAddress("HitposAVRmmU",           HitposAVRmmU);
    fChain->SetBranchAddress("HitposAVRmmV",           HitposAVRmmV);
    fChain->SetBranchAddress("HitposAVRmmW",           HitposAVRmmW);
    fChain->SetBranchAddress("HitposAVRmmULadder",     HitposAVRmmULadder);
    fChain->SetBranchAddress("HitposAVRmmVLadder",     HitposAVRmmVLadder);
    fChain->SetBranchAddress("HitmomMeVX",             HitmomMeVX);
    fChain->SetBranchAddress("HitmomMeVY",             HitmomMeVY);
    fChain->SetBranchAddress("HitmomMeVZ",             HitmomMeVZ);
    fChain->SetBranchAddress("HitthetaLoc",            HitthetaLoc);
    fChain->SetBranchAddress("HitphiLoc",              HitphiLoc);
    fChain->SetBranchAddress("HitglobalTime",          HitglobalTime);
    fChain->SetBranchAddress("HitGeant4EdepMeV",       HitGeant4EdepMeV);
    fChain->SetBranchAddress("HitClusterSizeCol",      HitClusterSizeCol);
    fChain->SetBranchAddress("HitClusterSizeRow",      HitClusterSizeRow);
    fChain->SetBranchAddress("HitRecoUmm",             HitRecoUmm);
    fChain->SetBranchAddress("HitRecoVmm",             HitRecoVmm);
    fChain->SetBranchAddress("HitRecoULaddermm",       HitRecoULaddermm);
    fChain->SetBranchAddress("HitRecoVLaddermm",       HitRecoVLaddermm);
    fChain->SetBranchAddress("HitPhiPrincipalAxis",    HitPhiPrincipalAxis);
    fChain->SetBranchAddress("HitRMSPrincipalAxis",    HitRMSPrincipalAxis);
    fChain->SetBranchAddress("HitNPixels",             HitNPixels);
    fChain->SetBranchAddress("HitFirstPixelIdx",       HitFirstPixelIdx);

    fChain->SetBranchAddress("PixelNb",                &PixelNb);
    fChain->SetBranchAddress("PixelHitIdx",             PixelHitIdx);
    fChain->SetBranchAddress("PixelGlobalIdx",          PixelGlobalIdx);
    fChain->SetBranchAddress("PixelColumn",             PixelColumn);
    fChain->SetBranchAddress("PixelRow",                PixelRow);
    fChain->SetBranchAddress("PixelAnalogCharge",       PixelAnalogCharge);
    fChain->SetBranchAddress("PixelSensorID",           PixelSensorID);
    fChain->SetBranchAddress("PixelUmm",                PixelUmm);
    fChain->SetBranchAddress("PixelVmm",                PixelVmm);
    fChain->SetBranchAddress("PixelStatus",             PixelStatus);
  
    fChain->SetBranchAddress("SaturationInfoNb",       &SatuNb);
    fChain->SetBranchAddress("SaturationInfoLinIdx",    SatuLinIdx);
    fChain->SetBranchAddress("SaturationInfoSensorID",  SatuSensorID);
    
    if(GotNonSensitiveInfo) {      
      fChain->SetBranchAddress("NonSensitiveParticleNb",           &NonSensitiveParticleNb);
      fChain->SetBranchAddress("NonSensitiveParticleSensitiveIdx",  NonSensitiveParticleSensitiveIdx);
      fChain->SetBranchAddress("NonSensitiveParticlepdgID",         NonSensitiveParticlepdgID);
      fChain->SetBranchAddress("NonSensitiveParticleTrkVtX",        NonSensitiveParticleTrkVtX);
      fChain->SetBranchAddress("NonSensitiveParticleTrkVtY",        NonSensitiveParticleTrkVtY);
      fChain->SetBranchAddress("NonSensitiveParticleTrkVtZ",        NonSensitiveParticleTrkVtZ);
      fChain->SetBranchAddress("NonSensitiveParticleNHits",         NonSensitiveParticleNHits);
      fChain->SetBranchAddress("NonSensitiveParticleFirstHitIdx",   NonSensitiveParticleFirstHitIdx);

      fChain->SetBranchAddress("NonSensitiveHitNb",                &NonSensitiveHitNb);
      fChain->SetBranchAddress("NonSensitiveHitParticleIdx",        NonSensitiveHitParticleIdx);
      fChain->SetBranchAddress("NonSensitiveHitposINmmX",           NonSensitiveHitposINmmX);
      fChain->SetBranchAddress("NonSensitiveHitposINmmY",           NonSensitiveHitposINmmY);
      fChain->SetBranchAddress("NonSensitiveHitposINmmZ",           NonSensitiveHitposINmmZ);
      fChain->SetBranchAddress("NonSensitiveHitglobalTimeINns",     NonSensitiveHitglobalTimeINns);
      fChain->SetBranchAddress("NonSensitiveHitmomINMeVX",          NonSensitiveHitmomINMeVX);
      fChain->SetBranchAddress("NonSensitiveHitmomINMeVY",          NonSensitiveHitmomINMeVY);
      fChain->SetBranchAddress("NonSensitiveHitmomINMeVZ",          NonSensitiveHitmomINMeVZ);
      fChain->SetBranchAddress("NonSensitiveHitEnergyINMeV",        NonSensitiveHitEnergyINMeV);
      fChain->SetBranchAddress("NonSensitiveHitposOUTmmX",          NonSensitiveHitposOUTmmX);
      fChain->SetBranchAddress("NonSensitiveHitposOUTmmY",          NonSensitiveHitposOUTmmY);
      fChain->SetBranchAddress("NonSensitiveHitposOUTmmZ",          NonSensitiveHitposOUTmmZ);
      fChain->SetBranchAddress("NonSensitiveHitmomOUTMeVX",         NonSensitiveHitmomOUTMeVX);
      fChain->SetBranchAddress("NonSensitiveHitmomOUTMeVY",         NonSensitiveHitmomOUTMeVY);
      fChain->SetBranchAddress("NonSensitiveHitmomOUTMeVZ",         NonSensitiveHitmomOUTMeVZ);
      fChain->SetBranchAddress("NonSensitiveHitEnergyOUTMeV",       NonSensitiveHitEnergyOUTMeV);
      fChain->SetBranchAddress("NonSensitiveHitGeant4EdepMeV",      NonSensitiveHitGeant4EdepMeV);
    }
    
  }
  
  return;
  
}
//------------------------------------------+-----------------------------------
bool  MCBoardReader::GetAFrame(Long64_t aFrame)
{
  
  Long64_t ientry = LoadTree(aFrame);
  if (ientry < 0) return false;
  fChain->GetEntry(aFrame);
  
  return true;
  
}
//------------------------------------------+-----------------------------------
Long64_t  MCBoardReader::LoadTree(Long64_t entry)
{
  
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (!fChain->InheritsFrom(TChain::Class()))  return centry;
  TChain *chain = (TChain*)fChain;
  if (chain->GetTreeNumber() != fCurrent) {
    fCurrent = chain->GetTreeNumber();
  }
  return centry;
  
}
//------------------------------------------+-----------------------------------
bool MCBoardReader::HasData()
{
  
  // Try to built the next event:
  //  1) read the raw data
  //  2) decode them
  //
  // This method is the one to call from the outside
  
  if(fDebugLevel) printf("MCBoardReader::HasData reading event %d\n", fEventNumber);

  fCurrentTriggerCnt = 0;
  if(fCurrentEvent != NULL) {
    delete fCurrentEvent;
    fCurrentEvent = NULL;
  }
  ListOfPixels.clear();
  ListOfTriggers.clear();
  ListOfFrames.clear();
  ListOfLineOverflow.clear();
  
  if(FetchEvent()) {
    if(fDebugLevel) printf("MCBoardReader::HasData start decoding event\n");
    if(DecodeFrames()) {
      if(fDebugLevel) printf("MCBoardReader::HasData new event %d will be generated with %d triggers\n", fEventNumber, int(ListOfTriggers.size()));
      
      if(fDebugLevel) cout << "MCBoardReader::Got " << ListOfPixels.size() << " pixels added to ListOfPixels" << endl;
      
      fCurrentEvent = new BoardReaderEvent( fEventNumber, fBoardNumber, fRunNumber, &ListOfPixels);
      fCurrentEvent->SetListOfTriggers( &ListOfTriggers);
      fCurrentEvent->SetListOfFrames( &ListOfFrames);
    }
    
    return true;
  }
  else {
    if (fDebugLevel) printf("MCBoardReader::HasData wrong event building...stopping!\n");
    return false;
  }

  return false;
  
}
//------------------------------------------+-----------------------------------
bool MCBoardReader::FetchEvent()
{
  // Read the necessary frames for all sensors from the raw data files.
  // Will store in ListOfFrames the frames which make an event
  // Assumes no trigger, so the ListOfTriggers is always empty
  // For the moment one event is one frame
  
  if(fFrameCounter > fChain->GetEntries()-1) return false;
  ListOfFrames.push_back(fFrameCounter);
  fFrameCounter++;
  
  fEventNumber++;

  return true;
  
}
// --------------------------------------------------------------------------------------
bool MCBoardReader::DecodeFrames() 
{
  
  // Read the information of frames in the ListOfFrames
  
  fFramesReadFromFile += ListOfFrames.size();
  
  TheListOfMCParticles.clear();
  ListOfNoisePixels.clear();
  TheListOfMCNonSensitiveParticles.clear();
  for(int iframe=0;iframe<int(ListOfFrames.size());iframe++) {
    if(fDebugLevel) cout << "MCBoardReader::Frame " << ListOfFrames[iframe] << " Added to list of frame of event " << fEventNumber << endl;
    FillPixelsFromFrame(ListOfFrames[iframe]);
  }
  
  //Filling up the MC information into the MCInfoHolder
  FillUpMCInfoHolderForThisEvent();
  
  return true;
  
}
// --------------------------------------------------------------------------------------
void MCBoardReader::FillPixelsFromFrame(int TheFrame)
{

  //fTree->GetEntry(TheFrame);
  GetAFrame(TheFrame);
  
  //Adding the MC particle of this frame to the list
  FillUpMCParticlesFromFrame(TheFrame);
  
  //Looping over the pixels in this frame
  for(int ipix=0;ipix<PixelNb;ipix++) {
    if(PixelStatus[ipix] == 0) continue; // Only fill the pixels not loss by saturation (status = 1)
    
    if(PixelSensorID[ipix]+1 > fNumberOfSensors) {
      cout << endl;
      cout << "MCBoardReader::Error::  Pixel plane number " << PixelSensorID[ipix]+1 << " is bigger than number of sensors (" << fNumberOfSensors << "). Check your inputs. Exiting now!!!" << endl;
      cout << endl;
      assert(false);
    }
    
    if(PixelColumn[ipix] < 0 || PixelColumn[ipix] > fSetup->GetPlanePar(PixelSensorID[ipix]+1).Strips(0)-1) {
      cout << endl;
      cout << "MCBoardReader::Error::  Pixel column " << PixelColumn[ipix] << " is outside sensor limits (0," << fSetup->GetPlanePar(PixelSensorID[ipix]+1).Strips(0)-1 << ")." 
           << " From hit = " << PixelHitIdx[ipix] << "." 
           << " Check your inputs. Exiting now!!!" 
	   << endl;
      cout << endl;
      assert(false);
    }
    
    if(PixelRow[ipix] < 0 || PixelRow[ipix] > fSetup->GetPlanePar(PixelSensorID[ipix]+1).Strips(1)-1) {
      cout << endl;
      cout << "MCBoardReader::Error::  Pixel row    " << PixelRow[ipix]    << " is outside sensor limits (0," << fSetup->GetPlanePar(PixelSensorID[ipix]+1).Strips(1)-1 << ")." 
           << " From hit = " << PixelHitIdx[ipix] << "." 
           << " Check your inputs. Exiting now!!!" 
	   << endl;
      cout << endl;
      assert(false);
    }
    
    if(fDebugLevel) cout << "MCBoardReader:: Adding pixel (col,row) = (" << PixelColumn[ipix] << "," << PixelRow[ipix] << ") of sensor " << PixelSensorID[ipix] << " of frame " << TheFrame << endl;
    
    Double_t Value = 1.0;
    if(fSetup->GetPlanePar(PixelSensorID[ipix]+1).AnalysisMode == 2) Value = PixelAnalogCharge[ipix];
    
    AddPixel( PixelSensorID[ipix], Value, PixelRow[ipix], PixelColumn[ipix]);
    
  } //End of pixel loop
  
  //Fill up the lines with overflow
  for(int ioverflow=0;ioverflow<SatuNb;ioverflow++) {
    if(SatuSensorID[ioverflow]+1 > fNumberOfSensors) {
      cout << endl;
      cout << "MCBoardReader::Error::  Line sensor number " << SatuSensorID[ioverflow]+1 << " is bigger than number of sensors (" << fNumberOfSensors << "). Check your inputs. Exiting now!!!" << endl;
      cout << endl;
      assert(false);
    }
    hLineOverFlowDist[SatuSensorID[ioverflow]]->Fill(SatuLinIdx[ioverflow]);
  }

  return;
  
}
// --------------------------------------------------------------------------------------
void MCBoardReader::AddPixel( int iSensor, int value, int aLine, int aColumn) 
{
   // Add a pixel to the vector of pixels
   // require the following info
   // - input = number of the sensors (start at 1 !!!!!)
   // - value = analog value of this pixel
   // - line & column = position of the pixel in the matrix

  if(fDebugLevel > 2) printf("MCBoardReader::  Addpixel adding pixel for sensor %d with value %d line %d row %d\n", iSensor, value, aLine, aColumn);

  ListOfPixels.push_back( BoardReaderPixel(iSensor+1, value, aLine, aColumn, 0));

  return;
  
}
// --------------------------------------------------------------------------------------
void MCBoardReader::FillUpMCParticlesFromFrame(int TheFrame)
{
  
  //Add the list of particles (with their hits, and hits with their pixels) of frame TheFrame into the TheListOfMCParticles list
  //Also add the list of pixel from noise from this frame to the ListOfNoisePixels list
  
  std::vector<int> _partIdx;
  
  //Looping over the particles in this frame
  for(int ipart=0;ipart<ParticleNb;ipart++) {
    MCParticle_t AMCParticle;
    AMCParticle.pdgID   = ParticlepdgID[ipart];
    AMCParticle.BKGType = ParticleBKGType[ipart];
    AMCParticle.ProdVtx = TVector3(ParticleTrkVtX[ipart],ParticleTrkVtY[ipart],ParticleTrkVtZ[ipart]);
    AMCParticle.frame   = TheFrame;
    
    AMCParticle.ListOfParticleHits.clear();
    
    _partIdx.push_back(TheListOfMCParticles.size());
    
    for(int ihit=0;ihit<ParticleNHits[ipart];ihit++) {
      int HitIdx = ParticleFirstHitIdx[ipart] + ihit;
  
      MCParticleHit_t  AMCParticleHit;
      AMCParticleHit.sensorID                = HitsensorID[HitIdx];
      AMCParticleHit.PosInXYZmm              = TVector3(HitposINmmX[HitIdx],HitposINmmY[HitIdx],HitposINmmZ[HitIdx]);
      AMCParticleHit.PosAveUVmm              = TVector2(HitposAVRmmU[HitIdx],HitposAVRmmV[HitIdx]);
      AMCParticleHit.PosAveUVLaddermm        = TVector2(HitposAVRmmULadder[HitIdx],HitposAVRmmVLadder[HitIdx]);
      AMCParticleHit.PosRecoAveUVmm          = TVector2(HitRecoUmm[HitIdx],HitRecoVmm[HitIdx]);
      AMCParticleHit.PosRecoAveUVLaddermm    = TVector2(HitRecoULaddermm[HitIdx],HitRecoVLaddermm[HitIdx]);
      AMCParticleHit.MomentumInXYZMeV        = TVector3(HitmomMeVX[HitIdx],HitmomMeVY[HitIdx],HitmomMeVZ[HitIdx]);
      AMCParticleHit.ThetaLoc                = HitthetaLoc[HitIdx];
      AMCParticleHit.PhiLoc                  = HitphiLoc[HitIdx];
      AMCParticleHit.Geant4EdepMeV           = HitGeant4EdepMeV[HitIdx];
      AMCParticleHit.GlobalTime              = HitglobalTime[HitIdx];
      AMCParticleHit.ClusterSizeCol          = HitClusterSizeCol[HitIdx];
      AMCParticleHit.ClusterSizeRow          = HitClusterSizeRow[HitIdx];
      AMCParticleHit.ClusterPhiPrincipalAxis = HitPhiPrincipalAxis[HitIdx];
      AMCParticleHit.ClusterRMSPrincipalAxis = HitRMSPrincipalAxis[HitIdx];
      AMCParticleHit.frame                   = TheFrame;
      AMCParticleHit.Npixels                 = HitNPixels[HitIdx];
      
      AMCParticleHit.ListOfHitPixels.clear();
      
      // Check if digitization was performed
      if(HitNPixels[HitIdx] >= 0) {
        for(int ipix=0;ipix<HitNPixels[HitIdx];ipix++) {
          int PixelIdx = HitFirstPixelIdx[HitIdx] + ipix;
	
	  MCHitPixel_t AMCHitPixel;
	  AMCHitPixel.GlobalIdx    = PixelGlobalIdx[PixelIdx];
	  AMCHitPixel.col          = PixelColumn[PixelIdx];
	  AMCHitPixel.row          = PixelRow[PixelIdx];
	  AMCHitPixel.ChargeAnalog = PixelAnalogCharge[PixelIdx];
	  AMCHitPixel.sensorID     = PixelSensorID[PixelIdx];
	  AMCHitPixel.PosUVmm      = TVector2(PixelUmm[PixelIdx],PixelVmm[PixelIdx]);
	  AMCHitPixel.status       = PixelStatus[PixelIdx];
	  AMCHitPixel.frame        = TheFrame;
	
	  AMCParticleHit.ListOfHitPixels.push_back(AMCHitPixel);
        } //End of Loop over this hit pixels
      }
      
      AMCParticle.ListOfParticleHits.push_back(AMCParticleHit);
    } //End of Loop over this particle hits
    
    TheListOfMCParticles.push_back(AMCParticle);
  } //End of loop over particles
  
  //Filling up the list of pixel from noise
  for(int ipix=0;ipix<PixelNb;ipix++) {
    if(PixelHitIdx[ipix] != -1) continue;
    int PixelIdx = ipix;
    
    MCHitPixel_t AMCHitPixel;
    AMCHitPixel.GlobalIdx    = PixelGlobalIdx[PixelIdx];
    AMCHitPixel.col          = PixelColumn[PixelIdx];
    AMCHitPixel.row          = PixelRow[PixelIdx];
    AMCHitPixel.ChargeAnalog = PixelAnalogCharge[PixelIdx];
    AMCHitPixel.sensorID     = PixelSensorID[PixelIdx];
    AMCHitPixel.PosUVmm      = TVector2(PixelUmm[PixelIdx],PixelVmm[PixelIdx]);
    AMCHitPixel.status       = PixelStatus[PixelIdx];
    AMCHitPixel.frame        = TheFrame;
    
    ListOfNoisePixels.push_back(AMCHitPixel);
  }
  
  if(fDebugLevel) {
    cout << "MCBoardReader::Total number of pixels of frame " << TheFrame << " is " <<  PixelNb << endl;
    cout << "MCBoardReader::Noise pixels of frame " << TheFrame << " are " << ListOfNoisePixels.size() << endl;
  }
  
  if(GotNonSensitiveInfo) {
    //Looping over the particles in this frame
    for(int ipart=0;ipart<NonSensitiveParticleNb;ipart++) {
      MCNonSensitiveParticle_t AMCParticle;
      AMCParticle.pdgID   = NonSensitiveParticlepdgID[ipart];
      AMCParticle.ProdVtx = TVector3(NonSensitiveParticleTrkVtX[ipart],NonSensitiveParticleTrkVtY[ipart],NonSensitiveParticleTrkVtZ[ipart]);
      AMCParticle.frame   = TheFrame;
      AMCParticle.ListOfNonSensitiveParticleHits.clear();

      AMCParticle.sensitiveIdx = -1;
      for(int ipart2=0;ipart2<int(_partIdx.size());ipart2++) {
	if(ipart2 == NonSensitiveParticleSensitiveIdx[ipart]) {
	  AMCParticle.sensitiveIdx = _partIdx[ipart2];
	  break;
	}
      }
      
      _partIdx.push_back(ipart);
      
      
      for(int ihit=0;ihit<NonSensitiveParticleNHits[ipart];ihit++) {
        int HitIdx = NonSensitiveParticleFirstHitIdx[ipart] + ihit;
	
        MCNonSensitiveParticleHit_t  AMCParticleHit;
	AMCParticleHit.GlobalTime              = NonSensitiveHitglobalTimeINns[HitIdx];
        AMCParticleHit.PosInXYZmm              = TVector3(NonSensitiveHitposINmmX[HitIdx],NonSensitiveHitposINmmY[HitIdx],NonSensitiveHitposINmmZ[HitIdx]);
        AMCParticleHit.MomentumInXYZMeV        = TVector3(NonSensitiveHitmomINMeVX[HitIdx],NonSensitiveHitmomINMeVY[HitIdx],NonSensitiveHitmomINMeVZ[HitIdx]);
	AMCParticleHit.EnergyInMeV             = NonSensitiveHitEnergyINMeV[HitIdx];
	AMCParticleHit.PosOutXYZmm             = TVector3(NonSensitiveHitposOUTmmX[HitIdx],NonSensitiveHitposOUTmmY[HitIdx],NonSensitiveHitposOUTmmZ[HitIdx]);
        AMCParticleHit.MomentumOutXYZMeV       = TVector3(NonSensitiveHitmomOUTMeVX[HitIdx],NonSensitiveHitmomOUTMeVY[HitIdx],NonSensitiveHitmomOUTMeVZ[HitIdx]);
	AMCParticleHit.EnergyOutMeV            = NonSensitiveHitEnergyOUTMeV[HitIdx];
        AMCParticleHit.Geant4EdepMeV           = NonSensitiveHitGeant4EdepMeV[HitIdx];
        AMCParticleHit.frame                   = TheFrame;
      
        AMCParticle.ListOfNonSensitiveParticleHits.push_back(AMCParticleHit);
      } //End of Loop over this particle hits
      TheListOfMCNonSensitiveParticles.push_back(AMCParticle);
    } //End of loop over particles from non-sensitive volumes
    
  } // End if GotNonSensitiveInfo
  
  return;
  
}
// --------------------------------------------------------------------------------------
void MCBoardReader::FillUpMCInfoHolderForThisEvent(void)
{
  
  //Use the lists of particles (with their hits, and hits with their pixels) of current event (TheListOfMCParticles) as well as 
  //the list of pixel from noise of current event (ListOfNoisePixels) to fill the MCInfoHolder object
  
  if(MCInfoHolder == NULL) {
    cout << endl;
    cout << "MCBoardReader::Error::  The MCInfoHolder object is null. It needs to be initialized in DAcq." << endl;
    cout << "MCBoardReader::Error::  Check your inputs. Exiting now!!!" << endl;
    cout << endl;
    assert(false);
  }
  MCInfoHolder->ResetLists();
  
  int HitNbCount   = 0;
  int PixelNbCount = 0;
  
  for(int ipart=0;ipart<int(TheListOfMCParticles.size());ipart++) {
    SimParticle_t ASimParticle;
    ASimParticle.pdgID       = TheListOfMCParticles[ipart].pdgID;
    ASimParticle.BKGType     = TheListOfMCParticles[ipart].BKGType;
    ASimParticle.ProdVtx     = TheListOfMCParticles[ipart].ProdVtx;
    ASimParticle.NHits       = TheListOfMCParticles[ipart].ListOfParticleHits.size();
    ASimParticle.FrameNumber = TheListOfMCParticles[ipart].frame;
    if(ASimParticle.NHits == 0) ASimParticle.FirstHitIdx = -1;
    else                        ASimParticle.FirstHitIdx = HitNbCount;
    
    MCInfoHolder->PushBackASimParticle(ASimParticle);
  
    for(int ihit=0;ihit<int(TheListOfMCParticles[ipart].ListOfParticleHits.size());ihit++) {
      SimHit_t ASimHit;
      ASimHit.ParticleIdx             = ipart;
      ASimHit.sensorID                = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].sensorID;
      ASimHit.PosInXYZmm              = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].PosInXYZmm;
      ASimHit.PosAveUVmm              = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].PosAveUVmm;
      ASimHit.PosAveUVLaddermm        = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].PosAveUVLaddermm;
      ASimHit.PosRecoAveUVmm          = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].PosRecoAveUVmm;
      ASimHit.PosRecoAveUVLaddermm    = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].PosRecoAveUVLaddermm;
      ASimHit.MomentumInXYZMeV        = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].MomentumInXYZMeV;
      ASimHit.ThetaLoc                = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].ThetaLoc;
      ASimHit.PhiLoc                  = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].PhiLoc;
      ASimHit.Geant4EdepMeV           = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].Geant4EdepMeV;
      ASimHit.GlobalTime              = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].GlobalTime;
      ASimHit.ClusterSizeCol          = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].ClusterSizeCol;
      ASimHit.ClusterSizeRow          = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].ClusterSizeRow;
      ASimHit.ClusterPhiPrincipalAxis = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].ClusterPhiPrincipalAxis;
      ASimHit.ClusterRMSPrincipalAxis = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].ClusterRMSPrincipalAxis;
      ASimHit.FrameNumber             = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].frame;
      //ASimHit.Npixels                 = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].ListOfHitPixels.size();
      ASimHit.Npixels                 = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].Npixels;
      if(ASimHit.Npixels == 0 || ASimHit.Npixels == -1) ASimHit.FistPixelIdx = -1;
      else                                              ASimHit.FistPixelIdx = PixelNbCount;
      
      MCInfoHolder->PushBackASimHit(ASimHit);
            
      for(int ipix=0;ipix<int(TheListOfMCParticles[ipart].ListOfParticleHits[ihit].ListOfHitPixels.size());ipix++) {
	
	SimPixel_t ASimPixel;
	ASimPixel.HitIdx       = HitNbCount;
	ASimPixel.GlobalIdx    = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].ListOfHitPixels[ipix].GlobalIdx;
	ASimPixel.col          = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].ListOfHitPixels[ipix].col;
	ASimPixel.row          = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].ListOfHitPixels[ipix].row;
	ASimPixel.ChargeAnalog = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].ListOfHitPixels[ipix].ChargeAnalog;
	ASimPixel.sensorID     = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].ListOfHitPixels[ipix].sensorID;
	ASimPixel.PosUVmm      = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].ListOfHitPixels[ipix].PosUVmm;
	ASimPixel.status       = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].ListOfHitPixels[ipix].status;
	ASimPixel.FrameNumber  = TheListOfMCParticles[ipart].ListOfParticleHits[ihit].ListOfHitPixels[ipix].frame;
	
	MCInfoHolder->PushBackASimPixel(ASimPixel);

	PixelNbCount++;
      } //End Loop on pixelMap
      HitNbCount++; 
    } // End Loop on ListOfParticleHits
  } //End loop on Particle [ipart]

  //Loop over pixels from noise
  for(int ipix=0;ipix<int(ListOfNoisePixels.size());ipix++) {
    SimPixel_t ASimPixel;
    ASimPixel.HitIdx       = -1;
    ASimPixel.GlobalIdx    = ListOfNoisePixels[ipix].GlobalIdx;
    ASimPixel.col          = ListOfNoisePixels[ipix].col;
    ASimPixel.row          = ListOfNoisePixels[ipix].row;
    ASimPixel.ChargeAnalog = ListOfNoisePixels[ipix].ChargeAnalog;
    ASimPixel.sensorID     = ListOfNoisePixels[ipix].sensorID;
    ASimPixel.PosUVmm      = ListOfNoisePixels[ipix].PosUVmm;
    ASimPixel.status       = ListOfNoisePixels[ipix].status;
    ASimPixel.FrameNumber  = ListOfNoisePixels[ipix].frame;
    
    MCInfoHolder->PushBackASimPixel(ASimPixel);
  } //End Loop on pixelMap
  
  //TheListOfMCNonSensitiveParticles.push_back(AMCParticle);
  if(GotNonSensitiveInfo) {
    HitNbCount   = 0;
    for(int ipart=0;ipart<int(TheListOfMCNonSensitiveParticles.size());ipart++) {
      NonSensitiveSimParticle_t ASimParticle;
      ASimParticle.SensitiveIdx = TheListOfMCNonSensitiveParticles[ipart].sensitiveIdx;
      ASimParticle.pdgID        = TheListOfMCNonSensitiveParticles[ipart].pdgID;
      ASimParticle.ProdVtx      = TheListOfMCNonSensitiveParticles[ipart].ProdVtx;
      ASimParticle.NHits        = TheListOfMCNonSensitiveParticles[ipart].ListOfNonSensitiveParticleHits.size();
      ASimParticle.FrameNumber  = TheListOfMCNonSensitiveParticles[ipart].frame;
      if(ASimParticle.NHits == 0) ASimParticle.FirstHitIdx = -1;
      else                        ASimParticle.FirstHitIdx = HitNbCount;

      MCInfoHolder->PushBackANonSensitiveSimParticle(ASimParticle);

      for(int ihit=0;ihit<int(TheListOfMCNonSensitiveParticles[ipart].ListOfNonSensitiveParticleHits.size());ihit++) {
        NonSensitiveSimHit_t ASimHit;

        ASimHit.ParticleIdx             = ipart;
	ASimHit.GlobalTime              = TheListOfMCNonSensitiveParticles[ipart].ListOfNonSensitiveParticleHits[ihit].GlobalTime;
        ASimHit.PosInXYZmm              = TheListOfMCNonSensitiveParticles[ipart].ListOfNonSensitiveParticleHits[ihit].PosInXYZmm;
        ASimHit.MomentumInXYZMeV        = TheListOfMCNonSensitiveParticles[ipart].ListOfNonSensitiveParticleHits[ihit].MomentumInXYZMeV;
	ASimHit.EnergyInMeV             = TheListOfMCNonSensitiveParticles[ipart].ListOfNonSensitiveParticleHits[ihit].EnergyInMeV;
	ASimHit.PosOutXYZmm             = TheListOfMCNonSensitiveParticles[ipart].ListOfNonSensitiveParticleHits[ihit].PosOutXYZmm;
        ASimHit.MomentumOutXYZMeV       = TheListOfMCNonSensitiveParticles[ipart].ListOfNonSensitiveParticleHits[ihit].MomentumOutXYZMeV;
	ASimHit.EnergyOutMeV            = TheListOfMCNonSensitiveParticles[ipart].ListOfNonSensitiveParticleHits[ihit].EnergyOutMeV;
	ASimHit.Geant4EdepMeV           = TheListOfMCNonSensitiveParticles[ipart].ListOfNonSensitiveParticleHits[ihit].Geant4EdepMeV;
	ASimHit.FrameNumber             = TheListOfMCNonSensitiveParticles[ipart].ListOfNonSensitiveParticleHits[ihit].frame; 
        MCInfoHolder->PushBackANonSensitiveSimHit(ASimHit);
	
        HitNbCount++; 
      } // End Loop on ListOfNonSensitiveParticleHits
    } //End loop on Particle [ipart]
  }  // End if GotNonSensitiveInfo
  
  return;
  
}
// --------------------------------------------------------------------------------------
bool MCBoardReader::CheckTriggerCnt(unsigned int trig)
{

  if(fCurrentTriggerCnt == 0) {
    fCurrentTriggerCnt = trig;
    ListOfTriggers.push_back(trig);
    
    return true;
  }
  
  if(fCurrentTriggerCnt != trig) return false;

  return true;
  
}
// --------------------------------------------------------------------------------------
void MCBoardReader::PrintStatistics(ostream &stream) {
  
  // Print statistics on the events read by this board
  //
  // AP, 2016/04/19

  stream << "***********************************************" << endl;
  stream << " Board MC "        << fBoardNumber << " found:" << endl;
  stream << fCurrentTriggerCnt  << " triggers." << endl;
  stream << fEventNumber        << " events in total," << endl;
  //stream << fEventsOverflow     << " events with an overflow (veto overflow " << fVetoOverflow << ")." << endl;
  stream << fFramesReadFromFile << " frames read overall." << endl;
  stream << "***********************************************" << endl;

  //char name[20];
  char title[50];
  
  int Nplots_per_line = 6;
  int Ncols = 0, Nrows = 0;
  if(fNumberOfSensors == 1) {
    Nrows = 1;
    Ncols = 1;
  }
  else if(fNumberOfSensors <= 4) {
    Nrows = 2;
    Ncols = 2;
  }
  if(fNumberOfSensors <= Nplots_per_line) {
    Nrows = 2;
    Ncols = 3;
  }
  else {
    Nrows = fNumberOfSensors/Nplots_per_line;
    if(fNumberOfSensors%Nplots_per_line) Nrows++;
    Ncols = 6;
  }
  
  TCanvas *cLineOverFlow = new TCanvas("cLineOverFlow","Line Overflow distribution for all sensors",300,600);
  cLineOverFlow->Divide(Ncols,Nrows);

  for(int isensor=0;isensor<fNumberOfSensors;isensor++) {
    cLineOverFlow->cd(isensor+1);
    hLineOverFlowDist[isensor]->Draw();
  }

  sprintf( title, "Results/%d/MCBoardReaderStats.root", fRunNumber);
  sprintf( title,"%s", fTool.LocalizeDirName( title)); // AP 2016/04/20
  cout << endl;
  cout << "Saving Board Statistics to file  " << title << endl;
  cout << endl;
  TFile f_BoardStat(title,"RECREATE");
  cLineOverFlow->Write();
  for(int iSensor=0;iSensor<fNumberOfSensors;iSensor++) hLineOverFlowDist[iSensor]->Write();
  f_BoardStat.Close();
  
  return;
  
}
// --------------------------------------------------------------------------------------
bool MCBoardReader::CheckIfNonSensitiveVariables(TTree* tree)
{
 
  TBranch* branch = NULL;
  branch = tree->FindBranch("NonSensitiveParticleNb");
  
  if(branch) return true;
  else       return false;
  
}
// --------------------------------------------------------------------------------------
void MCBoardReader::SetDebugLevel(int aLevel) {
  
  fDebugLevel = aLevel;
  
  cout << "MCBoardReader::Setting debug level to " << fDebugLevel << endl;
  
  return;
  
}
// --------------------------------------------------------------------------------------


