    /////////////////////////////////////////////////////////////
// Class Description of BoardReaderTREE
//
// Read data from a ROOT TTree
//
// Adapted from the original code by Szymon Bugiel https://gitlab.cern.ch/sbugiel/ce65_daq
//
/////////////////////////////////////////////////////////////
//
// created JB, 2021/11/05

#include "TCanvas.h"
#include "TApplication.h"

#include "BoardReaderTREE.h"

ClassImp(BoardReaderTREE)

//------------------------------------------+-----------------------------------
BoardReaderTREE::BoardReaderTREE(int boardNumber, int nSensors, int nColumns, int nRows, int triggerMode, int eventBuildingMode, int nBits) {
  // Board creator
  //
  // eventBuildingMode: used for frame substraction, currently only O (last-first)
  // triggerMode: O, not yet used
  //
  // JB, 2021/11/05


  fDebugLevel = 0; // Used for printing info later
  fBoardNumber = boardNumber;
  fNSensors = nSensors;
  fTriggerMode = triggerMode;
  fEventBuildingMode = eventBuildingMode;

  cout << "*****************************************" << endl;
  cout << "    < BoardReaderTREE constructor >      " << endl;
  cout << "*****************************************" << endl;
  cout << "Creating a BoardReaderTREE" << endl;
  cout << " * board nb: " << fBoardNumber << endl;
  cout << " * nb of sensors: " << fNSensors << endl;
  cout << " * trigger mode: " << fTriggerMode << endl;
  cout << " * event building mode: " << fEventBuildingMode << endl;

  // consistency check
  if( nColumns!=X_MX_SIZE || nRows!=Y_MX_SIZE ) {
    cout << "ERROR: BoardReaderTREE, inconsistent matrix size config(" << nColumns << ", " << nRows << ") != C65Event.h(" << X_MX_SIZE << ", " << Y_MX_SIZE << ")!"<< endl;
  } else {
    fNcolumns = X_MX_SIZE;
    fNrows = Y_MX_SIZE;
  }
  cout << " * matrix size, columns=" << fNcolumns << ", rows=" << fNrows << endl;

  if( pow(2,nBits)!=DAC_BOARD_ADC_MAX_COUNTS ) {
    cout << "ERROR: BoardReaderTREE, inconsistent nb of bits per value: " << nBits << " yielding max_value = " << pow(2,nBits) << " != " << DAC_BOARD_ADC_MAX_COUNTS << " from C65Event.h!"<< endl;
  } else {
    fNbOfBits = nBits;
  }

  // Initialization
  fRawFile = NULL;
  fRawTree = NULL;
  fCurrentEventNumber = 0;
  fCurrentTriggerNumber = 0;
  fTriggerCount = 0;
  fFrameCount = 0;
  fBadEventCount = 0;

  cout << "*****************************************" << endl;
  cout << "    < BoardReaderTREE constructor DONE >      " << endl;
  cout << "*****************************************" << endl;
}

//------------------------------------------+-----------------------------------
BoardReaderTREE::~BoardReaderTREE()
{

}

// --------------------------------------------------------------------------------------
bool BoardReaderTREE::AddFile(const char *inputFileName) {
  // Open the input Raw Data file,
  //

  fRawFile = TFile::Open( inputFileName, "read");
  if (!fRawFile)  {
    cout << "ERROR: BoardReaderTREE::AddFile, Failed to open input file: " << inputFileName << endl;
    return false;
  } else if( fRawFile->IsZombie()) {
    cout << "ERROR: BoardReaderTREE::AddFile, input file: " << inputFileName << " is a zombie!" << endl;
    return false;
  } else {
    cout << "INFO: BoardReaderTREE::AddFile, Successfully open input file: " << inputFileName << endl;
  }

  fRawTree = (TTree*)fRawFile->Get(TREENAME);
  if( !fRawTree ) {
    cout << "ERROR: BoardReaderTREE::AddFile, Failed to find tree: " << TREENAME << " in file "<< inputFileName << endl;
    return false;
  }
  fRawTree->SetBranchAddress("ev_number"  , &fRawEventNumber);
  fRawTree->SetBranchAddress("frames_per_event", &fFramesPerEvent);
  fRawTree->SetBranchAddress("frame", &fRawFrame);
  fNentries = fRawTree->GetEntries();
  cout << "INFO: BoardReaderTREE::AddFile,input tree opened with: " << fNentries << endl;
  cout << "------------------------------------------------------"   << endl;

  return true;
}

// --------------------------------------------------------------------------------------

bool BoardReaderTREE::HasData( ) {
  // Try to find the pixel data for the next event in the raw data file
  //
  // ==> THIS MIGHT BE TOO SIMPLISTIC depending on the data format <==
  //
  //
  // If something goes wrong with the decoding or no more events,
  //  a flag (eventOK) is used to return "false"
  // Otherwise returns "true"
  //

  // -+-+- Initialization

  bool eventOK   = true;   // we start with a good event by default
  fCurrentEvent   = NULL;  // Allow to know wether data are correct
  fListOfTriggers.clear();  // and clear the vectors
  fListOfFrames.clear();
  fListOfPixels.clear();

 if(fDebugLevel) {
   cout << endl ;
   cout << "BoardReaderTREE::HasData() fBoardNumber " << fBoardNumber << " BoardReaderTREE::HasData() - currentEvent " << fCurrentEventNumber << " currentTrigger " << fCurrentTriggerNumber << endl;
 }

 if( DecodeNextEvent() ) {
   fCurrentEvent = new BoardReaderEvent( fCurrentEventNumber, fBoardNumber, 0, &fListOfPixels); // run number set to 0 for now
   fCurrentEvent->SetListOfTriggers( &fListOfTriggers);
   fCurrentEvent->SetListOfFrames( &fListOfFrames);
   if(fDebugLevel) cout << " fBoardNumber " << fBoardNumber << " created new event " << fCurrentEventNumber << " (raw nb " << fRawEventNumber << ") with " << fListOfPixels.size() << " pixels from " << fListOfTriggers.size() << " triggers and " << fListOfFrames.size() << " frames." << endl;
   fCurrentEventNumber++;
 } // getting next event went wrong
 else{
   eventOK = false;
   cout<<" -/-/- ERROR BoardReaderTREE::HasData() - Can't get next event !"<<endl;
 }

 // End
 // check there are still data to read
 return eventOK;
}

//------------------------------------------+-----------------------------------
bool BoardReaderTREE::DecodeNextEvent() {
  // One event contains fFrameCount frames
  //
  // EventBuildingMode = 0
  //   We build one physics frame from the difference of two consecutive frames

  if( fRawTree->GetEntry(fCurrentEventNumber) == 0 ) {
    printf( "WARNING: BoardReaderTREE::DecodeNextEvent, event count %d, no more entries in the TREE!\n", fCurrentEventNumber);
    return false;
  }

  if ( fRawFrame->size()<=1 ) { // Bad event, return empty list
    printf( "WARNING: BoardReaderTREE::DecodeNextEvent, event count %d, Event nb %lld has no or only 1 frame (frameNb=%d, frameSize=%ld)!\n", fCurrentEventNumber, fRawEventNumber, fFramesPerEvent, fRawFrame->size());
    fBadEventCount++;
    return true;
  }
  if(fDebugLevel>1) printf( "  Event count %d Event nb %lld contains %d frames = %lu (from frameSize)\n.", fCurrentEventNumber, fRawEventNumber, fFramesPerEvent, fRawFrame->size());

  short value;
  int index;

  switch ( fEventBuildingMode ) {

    case 1: // REconstruct each frame
    for(unsigned int iFrame=1; iFrame < fRawFrame->size(); iFrame++) {
  		for(int xPix = 0 ; xPix < fNcolumns ; xPix++) {
  		  for(int yPix = 0 ; yPix < fNrows ; yPix++) {
          index = yPix*fNcolumns + xPix;
          value = fRawFrame->at(iFrame).raw_amp[xPix][yPix]-fRawFrame->at(iFrame-1).raw_amp[xPix][yPix];
          if(fDebugLevel>2) cout << "    Filling frame: " << iFrame << " x: " << xPix << " y: " << yPix << " index: " << index << " value = " << fRawFrame->at(iFrame).raw_amp[xPix][yPix] << " - " << fRawFrame->at(iFrame).raw_amp[xPix][yPix] << " = " << value << endl;
          fListOfPixels.push_back( BoardReaderPixel( 0, value, index, iFrame) );
        }
  		}
      fListOfFrames.push_back( iFrame);
      fFrameCount++;
  	}
    break;

    case 0: // Reconstruct just one frame from last and first
    default:
    for(int xPix = 0 ; xPix < fNcolumns ; xPix++) {
      for(int yPix = 0 ; yPix < fNrows ; yPix++) {
        index = yPix*fNcolumns + xPix;
        value = fRawFrame->at(fRawFrame->size()-1).raw_amp[xPix][yPix]-fRawFrame->at(0).raw_amp[xPix][yPix];
        if(fDebugLevel>2) cout << "    Filling frame: " << fRawFrame->size()-1 << " x: " << xPix << " y: " << yPix << " index: " << index << " value = " << fRawFrame->at(fRawFrame->size()-1).raw_amp[xPix][yPix] << " - " << fRawFrame->at(0).raw_amp[xPix][yPix] << " = " << value << endl;
        fListOfPixels.push_back( BoardReaderPixel( 0, value, index, fRawFrame->size()-1) );
      }
    }
    fListOfFrames.push_back( fRawFrame->size()-1);
    fFrameCount++;

  };

  return true;

}

// --------------------------------------------------------------------------------------
void BoardReaderTREE::SkipNextEvent() {
  // This method might be useful to ignore the next event
  //

  if(fDebugLevel) printf("  BoardReaderTREE: %d skipping current event %d (raw nb %lld)\n", fBoardNumber, fCurrentEventNumber, fRawEventNumber);
  fCurrentEventNumber++;

}

// --------------------------------------------------------------------------------------
void BoardReaderTREE::PrintStatistics(ostream &stream) {
  // Print statistics on all the events read by this board

 stream << "***********************************************" << endl;
 stream << " Board MIMOSIS " << fBoardNumber << " found:" << endl;
 stream << fTriggerCount << " triggers read overall," << endl;
 stream << fCurrentEventNumber << " events in total," << endl;
 stream << fFrameCount << " frames read overall " << endl;
 stream << fBadEventCount << " events with decoding errors. " << endl;
 stream << "***********************************************" << endl;

}

// --------------------------------------------------------------------------------------
