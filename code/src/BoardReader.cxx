/////////////////////////////////////////////////////////////
// Class Description of BoardReader                     //
//                                                         //
// //                                                         //
/////////////////////////////////////////////////////////////
//
// created JB, 2014/05/13

#include "BoardReader.h"

ClassImp(BoardReaderEvent)

// --------------------------------------------------------------------------------------

BoardReaderEvent::BoardReaderEvent( int eventNumber, int boardNumber, int runNumber) {
  
  // JB, 2014/05/13
  
  EventNumber = eventNumber; 
  BoardNumber = boardNumber; 
  RunNumber = runNumber;
  
}

// --------------------------------------------------------------------------------------

BoardReaderEvent::BoardReaderEvent( int eventNumber, int boardNumber, int runNumber, vector<BoardReaderPixel> *aListOfPixels) {
  
  // JB, 2014/05/13
  
  EventNumber = eventNumber; 
  BoardNumber = boardNumber; 
  RunNumber = runNumber;
  ListOfPixels = aListOfPixels;
  
}

// --------------------------------------------------------------------------------------

BoardReaderEvent::~BoardReaderEvent() {

  // JB, 2014/05/13

//   delete ListOfPixels;

}


