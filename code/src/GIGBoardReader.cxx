/////////////////////////////////////////////////////////////
// Class Description of GIGBoardReader                     //
//                                                         //
// Read simulated data from GEANT digitized by the DIGMAPS code
//
//
// Notice the current version assumes the data are contained
// only in one file
//                                                         //
/////////////////////////////////////////////////////////////
//
// created JB, 2012/04/24
// Modified LC, 2012/05/09
// Modified LC, 2014/12/16

#include "TCanvas.h"
#include "GIGBoardReader.h"

ClassImp(GIGBoardReader)
ClassImp(GIGEvent)

// --------------------------------------------------------------------------------------

GIGBoardReader::GIGBoardReader( int boardNumber, int nSensors) {

  // JB, 2012/04/22

  BoardNumber        = boardNumber;  
  NSensors           = nSensors;
  bufferPlane        = 0;
  
  cout << "DIGBoardRead " << BoardNumber << " created with " << NSensors << " sensors." << endl;
  
  ReadingOK      = true;
  EventReady     = false;
  EventStarted   = true;
  CurrentEventID = -1; // required to start with the first ID of the file
  NewEventID     = 0;
    
  // init counters and histos for statistics
  EventsCount        = 0;
  NumberOfFiles      = 0;
  CurrentFileNumber  = 0;
  AveragePixelCountTotal     = 0;
  AveragePixelCountTotalTemp = 0;
  AveragePixelCount      = new int[NSensors];
  AveragePixelCountTemp  = new int[NSensors];
  for( int is=0; is<NSensors; is++) {
    AveragePixelCount[is]     = 0;
    AveragePixelCountTemp[is] = 0;
  }
  
}

// --------------------------------------------------------------------------------------

GIGBoardReader::~GIGBoardReader() {

  // JB, 2012/04/09
  // LC, 2014/12/15

  if(DebugLevel) std::cout<<"GIGBoardReader Destructor"<<std::endl;

  delete CurrentEvent;
  
  std::vector<GIGPixel*>::iterator iterGIGPixel = ListOfPixels.begin();
  for( ; iterGIGPixel!=ListOfPixels.end() ; ++iterGIGPixel ) delete *iterGIGPixel;
  ListOfPixels.clear();
 
  std::vector<GIGMonteCarlo*>::iterator iterGIGMonteCarlo = ListOfMonteCarlo.begin();
  for( ; iterGIGMonteCarlo!=ListOfMonteCarlo.end() ; ++iterGIGMonteCarlo ) delete *iterGIGMonteCarlo;
  ListOfMonteCarlo.clear();

  delete AveragePixelCount;
  delete AveragePixelCountTemp;

  delete InputFileName;
  delete PrefixFileName;
  delete SuffixFileName;
	
}

// --------------------------------------------------------------------------------------

bool GIGBoardReader::AddFile(char *fileName) {
  
  // JB, 2012/04/09
  // LC, 2014/12/16

  NumberOfFiles = 1;
  CurrentFileNumber = 1;
  InputFileName = fileName;
  sprintf(InputFileName,"%s", fTool.LocalizeDirName( InputFileName));
 
  RawFileStream.open( InputFileName);

  if( RawFileStream.fail() ) {
    cout << endl << "ERROR GIGBoardReader " << BoardNumber << " file " << InputFileName << " does not exist!" << endl;
    return false;
  }
  // To read the first line
  //Char_t aLine[200];
  //RawFileStream.getline( &aLine, 200);
  cout << endl << "  --> GIGBoardReader " << BoardNumber << " New file " << InputFileName << endl;  
  
  return true;
}

// --------------------------------------------------------------------------------------

void GIGBoardReader::Close() {

  // Closes every files needed to be
  // 2012/04/09

  RawFileStream.close();
}

// --------------------------------------------------------------------------------------

void GIGBoardReader::SkipNextEvent() {

  // This method is used to ignore the next event,
  //  that means no event is generated
  //
  // JB, 2012/04/24

  if(DebugLevel) printf("  GIGBoardReader board %d, Skipping event %d\n", BoardNumber, CurrentEventID);

  while( !EventReady && ReadingOK ) {
    ReadLine();
  }

}

// --------------------------------------------------------------------------------------

void GIGBoardReader::ReadLine() {
  
  // Read a line from the text input file.
  // The eventID is used to decide whether
  // this line starts a new event or not.
  //
  // The format of input file follows specification by Loic Cousin 
  // eventID sensorID row column pixelNumber pixelCoordX pixelCoordY pixelCoordZ
  //  as of 2012/04/12.
  //
  // JB 2012/04/22

  
  if(DebugLevel>2) printf("  GIGBoardReader board %d: Reading line - current event id %d\n", BoardNumber, CurrentEventID );

  // If there is no more line to read, stop
  if( RawFileStream.eof() || CurrentFileNumber<NumberOfFiles ) {
    cout << "DIGBoarReader board " << BoardNumber << " reached end of file " << CurrentFileNumber << " over " << NumberOfFiles << " files." << endl;
    ReadingOK = false;
    return;
  }
  
  int eventID, sensorID = 0;
  int line, column, pixelNumber = 0;
  double coordX, coordY, coordZ = 0;
  double realEnergy, digitizedEnergy = 0; // LC, 2012/05/09

  // Read the event field first,
  //  but only if it was not read before (EventStarted==false).
  // If the currentEventID is not set yet (<0), set it.
  if( EventStarted ) {
    RawFileStream >> eventID;
    
    if( CurrentEventID<0 ) CurrentEventID = eventID;
    
    if(DebugLevel>2) printf("    event %d\n", eventID );
  }
  else {
    eventID = CurrentEventID;
  }
  
  // If the eventID is different from the current event number,
  //  indicate the event is ready.
  // The pixel fields of this line is not read yet.
  if( eventID != CurrentEventID ) {
    if(DebugLevel) printf("GIGBoardReader::ReadLine New event %d detected, current event is %d\n", eventID, CurrentEventID);

    EventReady = true; // will generate a new event
    EventStarted = false; // will prevent next reading of eventID
    NewEventID = eventID;
  }
  
  // Read the sensor and pixel fields when this is still the same event
  else {
    
    RawFileStream >> sensorID >> line >> column >> pixelNumber >> realEnergy >> digitizedEnergy >> coordX >> coordY >> coordZ;  // LC, 2012/05/09
    
    AddPixel(  sensorID, 1, line, column, coordX, coordY, coordZ);
    EventStarted = true; // will force next reading of eventID
  
    if(DebugLevel>2) printf("    event %d sensor %d, line %d, col %d, pixel %d\n", eventID, sensorID, line, column, pixelNumber );
  }
  
}

// --------------------------------------------------------------------------------------

bool GIGBoardReader::HasData( ) {

  // Fill a new event containing all the fired pixels of each sensors acquired,
  // return "true" if the event is OK, "false" otherwise
  //
  //
  // JB, 2012/04/09

  // -+-+- Initialization

  if(DebugLevel>1) cout << "  GIGBoardReader board " << BoardNumber << "::HasData - reading OK "
    << ReadingOK << " event readiness " << EventReady << " event started " << EventStarted << endl 
    << " events generated " << EventsCount << " current event " << CurrentEventID << endl;
  if(ListOfPixels.size()>0) {
    std::vector<GIGPixel*>::iterator iterGIGPixel = ListOfPixels.begin();
    for( ; iterGIGPixel!=ListOfPixels.end() ; ++iterGIGPixel ) delete *iterGIGPixel;
    ListOfPixels.clear();
  }
 
  if(ListOfMonteCarlo.size()>0) {
    std::vector<GIGMonteCarlo*>::iterator iterGIGMonteCarlo = ListOfMonteCarlo.begin();
    for( ; iterGIGMonteCarlo!=ListOfMonteCarlo.end() ; ++iterGIGMonteCarlo ) delete *iterGIGMonteCarlo;
    ListOfMonteCarlo.clear();
  }

  // -+-+- Check files have been associated

  if( NumberOfFiles==0 ) {
    cout << "ERROR: GIGBoardReader NO RAW DATA FILE WAS ASSOCIATED WITH BOARD " << BoardNumber << ", STOPPING!" << endl << endl;
    return false;
  }

  // -+-+- Loop over Daq Events (or frames for all sensors)

  while( !EventReady && ReadingOK ) {
    ReadLine();
  }


  // -+-+- Create the event when ready

  if( EventReady ) {
    GenerateNewEvent();
  }


  // -+-+- End
  return ReadingOK;

}

// --------------------------------------------------------------------------------------

void GIGBoardReader::GenerateNewEvent() {

  // Generate a new event
  //
  // JB 2012/04/22
  
  CurrentEvent = new GIGEvent( EventsCount, BoardNumber, ListOfPixels, ListOfMonteCarlo );
  if(DebugLevel>1) printf("  New event %d (id %d) from board %d with %d pixels\n", EventsCount, CurrentEventID, BoardNumber, (int)ListOfPixels.size());
  
  EventsCount++;
  
  // compute average of pixel counts per event
  AveragePixelCountTotal = ( AveragePixelCountTotal*(EventsCount-1) + AveragePixelCountTotalTemp ) / EventsCount;
  AveragePixelCountTotalTemp = 0;
  for( int is=0; is<NSensors; is++) {
    AveragePixelCount[is] = ( AveragePixelCount[is]*(EventsCount-1) + AveragePixelCountTemp[is] ) / EventsCount;
    AveragePixelCountTemp[is] = 0;
  }

  EventReady = false;
  CurrentEventID = NewEventID;
  
}

// --------------------------------------------------------------------------------------

void GIGBoardReader::AddPixel( int input, int value, int aLine, int aColumn, double aCoordStepX, double aCoordStepY, double aCoordStepZ ) {

  // Add a pixel to the vector of pixels
  // require the following info
  // - input = number of the sensors
  // - value = analog value of this pixel
  // - line & column = position of the pixel in the matrix
  // JB, 2012/04/09
  // LC, 2014/12/15

  GIGPixel* aGIGPixel = new GIGPixel(input, value, aLine, aColumn);
  ListOfPixels.push_back( aGIGPixel );
  
  GIGMonteCarlo* aGIGMonteCarlo = new GIGMonteCarlo( input, value, aCoordStepX, aCoordStepY, aCoordStepZ, aLine, aColumn );
  ListOfMonteCarlo.push_back( aGIGMonteCarlo ); 
  
  // increment pixel counts for the current event
  AveragePixelCountTotalTemp++;
  AveragePixelCountTemp[ input] += 1;
}

// --------------------------------------------------------------------------------------

void GIGBoardReader::PrintStatistics(ostream &stream) {

  // Print statistics on the events read by this board
  //
  // JB, 2012/04/09

  stream << "***********************************************" << endl;
  stream << " Board DIG " << BoardNumber << " found:" << endl;
  stream << EventsCount << " events in total," << endl;
  stream << AveragePixelCountTotal << " pixels per event in average:" << endl;
  for( int is=0; is<NSensors; is++) {
    stream << " sensor " << is << ": " << AveragePixelCount[is];
  }
  stream << endl;    
  stream << "***********************************************" << endl;

}

// --------------------------------------------------------------------------------------

GIGEvent::GIGEvent( int eventNumber, int boardNumber, vector<GIGPixel*>& listOfPixels, vector<GIGMonteCarlo*>& listOfMonteCarlo ) {

  // JB, 2012/04/09
  // LC, 2014/12/15 : Now vectors passing by ref
  	
  EventNumber      =  eventNumber;
  BoardNumber      =  boardNumber;
  ListOfPixels     =  listOfPixels;
  ListOfMonteCarlo =  listOfMonteCarlo; // LC 2012/10/17

}

// --------------------------------------------------------------------------------------

GIGEvent::~GIGEvent() {

  // crashing for the time bein...?
  // JB, 2009/09/14
  // LC, 2014/12/16
  
  //if(DebugLevel) std::cout<<"In Destructor GIGEvent"<<std::endl;
  //Passing by ref --> So don't delete these objects.
/*  
  std::vector<GIGPixel*>::iterator iterGIGPixel = ListOfPixels.begin();
  for( ; iterGIGPixel!=ListOfPixels.end() ; ++iterGIGPixel ) delete *iterGIGPixel;
  ListOfPixels.clear();
 
  std::vector<GIGMonteCarlo*>::iterator iterGIGMonteCarlo = ListOfMonteCarlo.begin();
  for( ; iterGIGMonteCarlo!=ListOfMonteCarlo.end() ; ++iterGIGMonteCarlo ) delete *iterGIGMonteCarlo;
  ListOfMonteCarlo.clear();
*/

}
