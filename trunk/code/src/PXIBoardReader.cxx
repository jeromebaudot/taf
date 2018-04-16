/////////////////////////////////////////////////////////////
// Class Description of PXIBoardReader OLD                 //
//                                                         //
// !!! OLD FORMAT COMPATIBLE WIT PXI ONLY NO PXIe !!!
//
//
// Read PXI digital raw data files
//  originally for MIMOSA 26                               //
// and produce an event for getting the results            //
// This version compatible with data format                //
//  V1.0 - 12/08/2009					   //
//							   //
// Terminology						   //
//  Frame = data for one sensor from first to last line
//  DaqEvent = set of frames, one for each sensors
//  Event = data relevant to a given trigger,
//	which may encompass several frames of the same sensor (hence DAQEvent),
//	this is the unit relevant for physics analysis
//                                                         //
/////////////////////////////////////////////////////////////
//
// created JB, 2009/08/20
// Last Modified JB, 2009/08/30
// Last Modified JB, 2009/09/01 message level, file index
// Last Modified JB, 2009/09/09 overflow management, statistics printout, double reading
// Last Modified JB, 2009/09/14 Statistics
// Last Modified JB, 2009/09/16 Abort some specific events
// Last Modified JB, 2010/06/16 Allow several triggers in an event
// Last modified JB, 2011/07/07 to localize path names
// Last modified JB, 2011/10/30 PrintStatistics
// Last Modified: BH 2013/08/20, warning corrections from clang

#define APP__RMV_CLASSES
#define GLB_FILE_PATH_SZ 256
#define GLB_CMT_SZ 256
#define ROOT_ROOT

#include "pxi_daq_lib_v.0.0/globals.def"
#include "pxi_daq_lib_v.0.0/types.typ"
#include "pxi_daq_lib_v.0.0/asic.def"
#include "pxi_daq_lib_v.0.0/asic.typ"
#include "pxi_daq_lib_v.0.0/maps.def"
#include "pxi_daq_lib_v.0.0/mi26.def"
#include "pxi_daq_lib_v.0.0/mi26.typ"
#include "pxi_daq_lib_v.0.0/daq_pxi.def"
#include "TCanvas.h"

#include "PXIBoardReader.h"

ClassImp(PXIBoardReader)
ClassImp(PXIEvent)

// --------------------------------------------------------------------------------------

PXIBoardReader::PXIBoardReader( int boardNumber, const char *configFileName, int triggerMode, int endian) {

  // JB, 2009/08/18
  // Modified: JB 2009/09/10 for statistics
  // Modified: JB 2009/09/16 for statistics

  BoardNumber        = boardNumber;  

  sprintf( ConfigFileName, "%s", fTool.LocalizeDirName( configFileName)); // JB 2011/07/07
  ReadConfiguration();

  // Trigger mode:
  // 0: mode A-0, all frame are a new event (triggerless)
  // 1: mode A-1, frame contains the trigger position wrt line number
  // 2: mode B (not used yet)
  TriggerMode         = triggerMode;
  TriggerLineOffset   = 4;
  NextFirstLineToKeep = -1;
  NextFirstFrame      = -1;
  FirstLineToKeep     = -1;
  FirstFrame          = -1;
  LastLineToKeep      = -1;
  LastFrame           = -1;
  
  cout << "   Trigger mode:" << TriggerMode << endl;
  cout << "   Trigger offset:" << TriggerLineOffset << endl;
  //FramesPostTrigger = ?; // TBD for trigger mode B
  //cout << "   # frames to read for one trigger:" << FramesPostTrigger << endl;

  NumberOfFiles      = 0;
  CurrentFileNumber  = -1;
  SizeOfDaqEvent     = NSensors * sizeof(MI26__TZsFFrameRaw);
  Data               = new unsigned int[SizeOfDaqEvent];
  Endianness         = endian;
  ReadingEvent       = false; 
  ReadTwice          = false;
  EventReady         = false;
  Overflow           = false; // JB 2009/09/08
  CurrentEvent       = 0; // Allow to know wether data are correct, JB 2009/05/26
  CurrentEventNumber = 0;
  FramesReadFromFile = 0;

  // init counters and histos for statistics
  // JB 2009/0910
  EventsCount        = 0;
  EventsOverflow     = 0;
  EventsAborted      = 0; // JB 2009/09/16
  FramesReadTwice    = 0;
  for( int ib=0; ib<100; ib++) {
    NStatesInBlock[ib] = 0;
  }
  NStatesInLine = 0;
  h1BlockOccupancy = new TH1S("h1BlockOccupancy","Block state occupancy",11,-1,10);
  h1LineOccupancy = new TH1S("h1LineOccupancy","Line state occupancy",11,-1,10);

  printf( "   DaqEvent size = %d x %d = %d\n", NSensors, (int)sizeof(MI26__TZsFFrameRaw), (int)SizeOfDaqEvent);

}

// --------------------------------------------------------------------------------------

PXIBoardReader::~PXIBoardReader() {

  // JB, 2008/09/27
	// Modified: JB 2010/06/16, several triggers

  delete CurrentEvent;
  delete InputFileName;
  delete Data;
  //delete NStatesInBlock; // removed, compilation error, JB 2012/10/30
  delete h1LineOccupancy;
  delete h1BlockOccupancy;
  ListOfTriggerPos.clear(); //JB 2010/06/16
  ListOfFrames.clear(); //JB 2010/06/16

	
}

// --------------------------------------------------------------------------------------

bool PXIBoardReader::ReadConfiguration() {

  // Read the configuration file which contains info usefull to read data
  //
  // JB, 2009/08/18

  MI26__TZSRunCnf  *configuration = new MI26__TZSRunCnf;

  ConfigFile = fopen( ConfigFileName, "r");
  if( !ConfigFile ) {
    cout << endl << "ERROR PXIBoardReader " << BoardNumber << " file " << ConfigFileName << " does not exist!!" << endl;
    return false;
  }
  cout << endl << "  --> PXIBoardReader " << BoardNumber << " Configuration file " << ConfigFileName << endl;  

  if( fread( configuration, sizeof( MI26__TZSRunCnf), 1, ConfigFile) == 0 ) {
    cout << endl << "ERROR PXIBoardReader " << BoardNumber << " file " << ConfigFileName << " is empty!!" << endl;
    return false;
  }

  NSensors          = configuration->AsicNb;
  NumberOfLines     = 576; // TBD configurable, JB 2009/08/18
  RunNumber         = configuration->RunNo;
  EventsPerFile     = configuration->RunFileEvNb;

  cout << "   sensor name:" << configuration->AsicName << endl;
  cout << "   # sensors:" << NSensors << endl;
  cout << "   Run number:" << RunNumber << endl;
  cout << "   # events in run:" << configuration->RunEvNb << endl;
  cout << "   # events per file:" << EventsPerFile << endl;

  if(DebugLevel>4) {
    for( int i=0; i<MI26__TZSRunCnf__HW_TRIG_PAR_NB; i++) {
      cout << "  trig config info " << i << ": " << configuration->HwTrigPar[i] << endl;
    }
  }

  fclose(ConfigFile);
 
  return true;	
}

// --------------------------------------------------------------------------------------
void PXIBoardReader::AddFileList(const char *prefixFileName, int firstIndex, int endIndex, const char *suffixFileName) {

  // Store the inputs to be able to read files one after the other
  // Files are expected to be numberred over four digits: 0001, 0002, 0003, ...
  // First file is assumed to be numbered 0
  // JB, 2009/08/14
  // Modified: JB, 2009/09/01 first index

  NumberOfFiles = endIndex;
  CurrentFileNumber = -1+firstIndex; // to accomadate any first index, JB 2009/09/01
  SuffixFileName = new char[10];
  PrefixFileName = new char[300];
  sprintf( PrefixFileName, "%s", prefixFileName);
  sprintf( SuffixFileName, "%s", suffixFileName);
  if(DebugLevel>0) cout << "  --> PXIBoardReader " << BoardNumber << " adding " << NumberOfFiles << " files like " << prefixFileName << "****" << SuffixFileName << endl;
  InputFileName = new char[300];

}

// --------------------------------------------------------------------------------------
bool PXIBoardReader::OpenNextFile() {

  // Try to open the next raw data file
  //
  // JB 2009/08/14
  // JB 2009/09/01
  
  // Close any previous open file
  if( RawFileStream.is_open() ) {
    RawFileStream.close();
    RawFileStream.clear();
    if(DebugLevel) cout << "  --> PXIBoardReader closing " << InputFileName << " (fail=" << RawFileStream.fail() << ")" << endl; // message only when dubuggign, JB 2009/09/01
  }
  
  // Check if some more files are to be read
  if( CurrentFileNumber < NumberOfFiles) {
    CurrentFileNumber++;
	if(DebugLevel>1) cout << "PXIBoardReader " << BoardNumber << " New file to read " << CurrentFileNumber << " over " << NumberOfFiles << endl;
	if( CurrentFileNumber < 10 ) {
	  sprintf( InputFileName, "%s000%d%s", PrefixFileName, CurrentFileNumber, SuffixFileName);
	}
	else if( CurrentFileNumber < 100 ) {
	  sprintf( InputFileName, "%s00%d%s", PrefixFileName, CurrentFileNumber, SuffixFileName);
	}
	else if( CurrentFileNumber < 1000 ) {
	  sprintf( InputFileName, "%s0%d%s", PrefixFileName, CurrentFileNumber, SuffixFileName);
	}
	else if( CurrentFileNumber < 10000 ) {
	  sprintf( InputFileName, "%s%d%s", PrefixFileName, CurrentFileNumber, SuffixFileName);
	}
   else {
      cout << "ERROR PXIBoardReader: trying to access a file number too large " << CurrentFileNumber << " > 9999!" << endl;
	  return false;
   }
   
   sprintf(InputFileName,"%s", fTool.LocalizeDirName( InputFileName)); // JB 2011/07/07
   cout << "  --> PXIBoardReader opening " << InputFileName << endl;
   RawFileStream.open( InputFileName);
   FramesReadFromFile = 0;
   if( RawFileStream.fail() ) { // end the reading if file opening failed
     cout << endl << "ERROR PXIBoardReader " << BoardNumber << " cannot open file " << InputFileName << endl;
     return false;
   }
  }

  // Otherwise no more file, end the reading
  else {
	cout << "  --> PXIBoardReader " << BoardNumber << ": No more files to read " << CurrentFileNumber << " > " << NumberOfFiles << " closing!" << endl;   //YV 23/06/09 to speed up DSF production
	return false;
  }

  return true;

}

// --------------------------------------------------------------------------------------

void PXIBoardReader::Close() {

  // Closes everything needed to be
  // 2008/09/27

  RawFileStream.close();
}

// --------------------------------------------------------------------------------------

unsigned int PXIBoardReader::SwapEndian( unsigned int data) {

  //
  //
  // JB 2009/08/18

  cout << "ERROR PXIBoardReader: DO NOT KNOW WHAT TO DO!!!!" << endl;

  //return ((data & 0xFF000000)>>24) + ((data & 0x00FF0000)>>8) + ((data & 0x0000FF00)<<8) + ((data & 0x000000FF)<<24);
  data = 0;
    
  return data;
}

// --------------------------------------------------------------------------------------

bool PXIBoardReader::GetNextDaqEvent( ) {

  // Read one Daq Event from the raw data file
  //  i.e. one frame for each sensor read by this board
  // 
  // The array of char Data points to the data
  // 
  // Endianness not yet dealt with !
  //
  // return "false" if nothing to read, "true" otherwise
  //
  // JB, 2009/14/08

  bool readPossible = true;

  // If end of file but still a file to read, open it
  if(DebugLevel>2) printf( "  PXIBoardReader board %d: status of input file open=%d, fail=%d, number of frames read %d <?> %d max\n", BoardNumber, RawFileStream.is_open(), RawFileStream.eof(), FramesReadFromFile, EventsPerFile);

  if( RawFileStream.eof() || !RawFileStream.is_open() || FramesReadFromFile==EventsPerFile ) {
    readPossible = OpenNextFile();
  } //end if eof

  // Now we can get the next data buffer
  if( readPossible ) {

    RawFileStream.read(reinterpret_cast<char *> ( &Data[0] ), SizeOfDaqEvent);
    FramesReadFromFile++;

    if(DebugLevel>2) printf( "  PXIBoardReader board %d: Got new data buffer (word %d bytes) with %d bytes at mem.pos %p\n", BoardNumber, (int)sizeof(char), (int)SizeOfDaqEvent, Data); // removing warning: cast 'unsigned long' then 'size_t' and 'unsigned int *' to 'int' BH 2013/08/20
    
  }

  return readPossible;

}

// --------------------------------------------------------------------------------------

void PXIBoardReader::SkipNextEvent() {

  // This method is used to ignore the next event
  //
  // JB, 2009/05/26

  if(DebugLevel) printf("  PXIBoardReader: %d Resetting readout because of bad event\n", BoardNumber);
  ReadingEvent       = false; 
  //CurrentEventNumber = 0;
  //CurrentFrameNumber = 0;
  //CurrentTimestamp   = 0;
  CurrentEvent       = 0; // Allow to know wether data are correct, JB 2009/05/26

}

// --------------------------------------------------------------------------------------

bool PXIBoardReader::DecodeFrame( int iSensor) {

  // Read the information of a frame for a given sensor
  // We use extensively the structure definined by Gille Clauss
  // see mi26.typ file
  //
  // JB, 2009/08/20
  // Modified: JB 2009/08/30 to stop at dummy 16 bits word for "odd" frame
  // Modified: JB 2009/09/09 set an overflow flag
  // Modified: JB 2009/09/16 abort events with specific conditions
	// Modified: Jb 2010/06/16 manage several triggers

  if( iSensor == 0 && ReadTwice) { // this is the second reading, so we re-init the flag
    ReadTwice = false;
    if(DebugLevel>1) printf("                  ** Second and last reading!\n");
  }

  // -+-+- Pointer to the beginning of the frame data
  MI26__TZsFFrameRaw *frame = (MI26__TZsFFrameRaw*)(Data);
  frame += iSensor; // correct position of data for sensor iSensor
  
  int dataLength  = ( ((frame->DataLength & 0xFFFF0000)>>16) + (frame->DataLength & 0x0000FFFF) ) * sizeof(UInt16);
  UInt16 *frameData = (UInt16*)frame->ADataW16;
  //int endAddress  = (int)(frameData)+dataLength;
  long int endAddress  = (long int)frameData + dataLength;

  if(DebugLevel>2) printf("  PXIBoardReader: Decoding frame %d, (header %x - trailer %x, sensor %d, dataLength %d bytes at mem.pos %ld, end.pos %ld\n", (unsigned int)frame->FrameCnt, (unsigned int)frame->Header, (unsigned int)frame->Trailer, iSensor, dataLength, (long int)frameData, endAddress); // removing warning: several casts for consistency BH 2013/08/20
  
  if(DebugLevel>1 && iSensor==0) printf("       frame %ld: Event reading: %d from frame %d - line %d to frame %d - line %d\n", frame->FrameCnt, ReadingEvent, FirstFrame, FirstLineToKeep, LastFrame, LastLineToKeep); // removing warning: specifying (unsigned int) BH 2013/08/20

  // -+-+- Pointer to the array for the frame status
  // In trigger mode A-0 
  // In trigger mode A-1
  //   provide the trigger position if any
  //   if the position is < NumberOfLines, this trigger concerns the next frame
  //   if the position is = NumberOfLines, this trigger concerns the next to next frame
  ASIC__TFrameStatus frameStatus = frame->SStatus;
  
  // Set the condition to start in trigger mode A-0
  if( iSensor == 0 && TriggerMode == 0 ) { // trigger mode A-0
    NextFirstFrame       = frame->FrameCnt;
    NextFirstLineToKeep  = 0;    
  } // end trigger mode A-0

  // =======
  // Test the condition to start the reading, before the data of the first sensor
  if( iSensor == 0  && !ReadingEvent && (int)frame->FrameCnt == NextFirstFrame ) { // test if starting
    ReadingEvent        = true;
    FirstFrame          = NextFirstFrame;
    FirstLineToKeep     = NextFirstLineToKeep;
    NextFirstFrame      = -1;
    NextFirstLineToKeep = -1;
    CurrentEventNumber  = FirstFrame;
		ListOfFrames.push_back( FirstFrame ); //JB 2010/06/16
		ListOfTriggerPos.push_back( FirstLineToKeep ); //JB 2010/06/16
    if( FirstLineToKeep == 0 ) {
      LastLineToKeep = NumberOfLines-1;
//      LastLineToKeep = NumberOfLines+20; //JB 2010/06/14
      LastFrame      = FirstFrame;
    }
    else {
      LastLineToKeep = FirstLineToKeep-1;
      LastFrame      = FirstFrame+1;
			ListOfFrames.push_back( LastFrame ); //JB 2010/06/16
    }

    if(DebugLevel>1) printf("                  ** Reading event %d started (to stop at line %d and frame %d)!\n", FirstFrame, LastLineToKeep, LastFrame); 

  } // end test if starting


  // =======
  // If we are reading through the last frame of the current event 
  // but this is also the beginning of the next event
  // set the flag for reading this frame twice
  // or adjust the current event length
  if( iSensor == 0  && ReadingEvent && NextFirstFrame == (int)frame->FrameCnt && LastFrame == (int)frame->FrameCnt ) { // test if reading twice
    ReadTwice = true;
    FramesReadTwice++;

    if( NextFirstLineToKeep > LastLineToKeep ) {
      if(DebugLevel>1) printf("                  ** This frame %d will be read twice!\n", (int)frame->FrameCnt);
    }

    // If the first line of the next trigger (NextFirstLineToKeep) 
    // is located before the last line of the current trigger (LastLineToKeep)
    // stop the reading of the second or both events corresponding to these triggers
    // or merge the frames into the same event
    else {

      // version where both events are aborted
      //EventsAborted += 2;
      //ReadingEvent        = false;
      //ReadTwice           = false;
      //FirstLineToKeep     = -1;
      //FirstFrame          = -1;
      //NextFirstLineToKeep = -1;
      //NextFirstFrame      = -1;
      //ListOfPixels.clear();
      //if(DebugLevel>1) printf("                  ** Reading stop because trigger are mixed in the same frame (2 events aborted)!\n", frame->FrameCnt); 

      // Version where only the second event is aborted
      // MG+JB 2010/06/03
      //EventsAborted += 1;
      //ReadTwice           = false;
      //NextFirstLineToKeep = -1;
      //NextFirstFrame      = -1;
      //if(DebugLevel>1) printf("                  ** Will not read 2nd event because trigger are mixed in the same frame (1 event aborted)!\n", frame->FrameCnt); 

      // Version where the frames are merged into one vent
      // MG 2010/06/16
      ReadTwice = false;
			ListOfTriggerPos.push_back( FirstLineToKeep ); //JB 2010/06/16
      if( NextFirstLineToKeep == 0 ) {
        LastLineToKeep = NumberOfLines-1;
        LastFrame      = NextFirstFrame;
      }
      else {
        LastLineToKeep = NextFirstLineToKeep-1;
        LastFrame      = NextFirstFrame+1;
				ListOfFrames.push_back( LastFrame ); //JB 2010/06/16
      }
      NextFirstLineToKeep = -1;
      NextFirstFrame      = -1;
      if(DebugLevel>1) printf("                  ** This event started at frame %d - line %d is extended to line %d at frame %d\n", FirstFrame, FirstLineToKeep, LastLineToKeep, LastFrame);

    }

  }


  // Set the condition to start in trigger mode A-1
  if( iSensor == 0 && TriggerMode == 1 && frameStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_LINE] > -1 ) { // trigger mode A-1 with trigger info

    // store the trigger info
    NextFirstLineToKeep = frameStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_LINE] + TriggerLineOffset;
//    NextFirstLineToKeep = frameStatus.ATrigRes[ASIC__MI26_TRIG_RES__SIG_LINE] + TriggerLineOffset-20;//JB 
    NextFirstFrame = frame->FrameCnt + 1;      
    if( FirstLineToKeep >= NumberOfLines ) {
      NextFirstLineToKeep %= NumberOfLines;
      NextFirstFrame       = frame->FrameCnt + 2;
    }
    
  } // end trigger mode A-1

  if( TriggerMode != 0 && TriggerMode != 1  ) {
    cout << "ERROR: PXIBoardReader trigger mode " << TriggerMode << " not managed!" << endl;
    return false;
  }


  if(DebugLevel>1 && iSensor==0) printf("                    Next event will start at frame %d - line %d\n", NextFirstFrame, NextFirstLineToKeep);

  if(DebugLevel>2) {
    printf("                  Asic %d, AcqNo %d, Frame in acq %d in run %d, hit cnt %d \n", frameStatus.AsicNo, (int)frameStatus.AcqNo, (int)frameStatus.FrameNoInAcq, (int)frameStatus.FrameNoInRun, (int)frameStatus.HitCnt);  // removing warning: several casts BH 2013/08/20
    for( int iInfo=0; iInfo<ASIC__ENUM_TRIG_RES_NB; iInfo++ ) {
      printf( "                 Trig info %d: %d\n", iInfo, (int)frameStatus.ATrigRes[iInfo]);// removing warning: cast BH 2013/08/20
    }
  }


  // -+-+- Pointers AND LOOP to usefull data, i.e. line and states
  MI26__TStatesLine *lineStatus;
  MI26__TState *state;

  // ====================
  while( (long int)frameData < endAddress) { // Loop over usefull data

    // first 16 bits word is the Status/Line
    lineStatus = (MI26__TStatesLine*)frameData;
    frameData += 1; // goto next word

    if( lineStatus->F.Ovf > 0 ) { // Stop when overflow, JB 2009/09/08
//      if(EventsOverflow%1000==0 && ReadingEvent) printf("WARNING : overflow while reading %d event %d at frame %d and sensor %d, total overflow number is %d\n", ReadingEvent, CurrentEventNumber, frame->FrameCnt, iSensor, EventsOverflow);
       Overflow = true;
//     if(iSensor==3 && lineStatus->F.LineAddr ==498) Overflow = false; //MG 2010/06/07 --- chip43
//     if(iSensor==3 && lineStatus->F.LineAddr ==499) Overflow = false; //MG 2010/06/07 --- chip43
     if(iSensor==4 && lineStatus->F.LineAddr ==318) Overflow = false; //MG 2010/06/02
//There are noisy pixels on line 318 of the 4th chip --> overflow disable for this line
      if(EventsOverflow%1000==0 && ReadingEvent&&Overflow == true) printf("WARNING : overflow while reading %d event %d at frame %d, line %d and sensor %d, total overflow number is %d\n", ReadingEvent, CurrentEventNumber, (unsigned int)frame->FrameCnt,lineStatus->F.LineAddr, iSensor, EventsOverflow);//MG 2010/06/02  // removing warning with cast BH 2013/08/20

    }

    if(DebugLevel>3) printf("                  line %d, #states %d, overflow %d at mem.pos %ld, reading event ? %d\n", lineStatus->F.LineAddr, lineStatus->F.StateNb, lineStatus->F.Ovf, (long int)lineStatus, ReadingEvent); // removing warning specifying type for format BH 2013/08/20

    // Stop there if the state was the last usefull word to read
    // because it is a dummy 
    // JB 2009/08/30
    if( (long int)frameData >= endAddress ) break;
  
    // Init some statistics for this line
    // JB 2009/09/10
    for( int ib=0; ib<100; ib++) {
      NStatesInBlock[ib] = 0;
    }
    NStatesInLine = 0;

    // Next words are the states if any
    for( int iState=0; iState<lineStatus->F.StateNb; iState++ ) { // loop over states found on sensor
      state = (MI26__TState*)frameData;
      frameData += 1; // goto next word

      //compute some statistics, JB 2009/09/10
      NStatesInLine++;
      NStatesInBlock[state->F.ColAddr%64] += 1;

      // A state contains HitNb+1 pixels
      // the first pixel being on the left at the column ColAddr
      for( int iPixel=0; iPixel<state->F.HitNb+1; iPixel++) { // loop on pixels in the state

	// create a new pixel only if we are reading an event
	// and if the line is in the proper limit
	if( ReadingEvent && (
	    ((int)frame->FrameCnt == FirstFrame && (int)lineStatus->F.LineAddr >= FirstLineToKeep)
	    || ((int)frame->FrameCnt == LastFrame && (int)lineStatus->F.LineAddr <= LastLineToKeep)
	  ) ) {
	  AddPixel( iSensor+1, 1, lineStatus->F.LineAddr, state->F.ColAddr+iPixel);
	}

      }

      if(DebugLevel>3) printf("                  state %d, #pixels %d, column %d at mem.pos %ld\n", iState, state->F.HitNb+1, state->F.ColAddr, (long int)state); // removing warning specifying type for format BH 2013/08/20
    } // end loop over states

    // Fill the statistics histograms, JB 2009/09/10
    for( int ib=0; ib<18; ib++) {
      h1BlockOccupancy->Fill( NStatesInBlock[ib]);
    }
    h1LineOccupancy->Fill( NStatesInLine);
 

    if(DebugLevel>4) printf("               mem.pos %ld <?> %ld\n", (long int)frameData, endAddress); // removing warning specifying type for format BH 2013/08/20

  } // end loop over usefull data
  // =====================

  if(DebugLevel>2) cout << "                 # pixels so far " << ListOfPixels.size() << endl;


  // =======
  // Condition to stop the reading after the data of the last sensor
  if( iSensor == NSensors-1 && ReadingEvent && (int)frame->FrameCnt == LastFrame) {
    
    ReadingEvent   = false;
    EventReady     = true;
    LastLineToKeep = -1;
    LastFrame      = -1;

    if(DebugLevel>1) printf("                  ** Reading event %d stopped!\n", CurrentEventNumber); 
  }



  // -+-+- End

  return true; // if you got there, it is safe!

}

// --------------------------------------------------------------------------------------

bool PXIBoardReader::HasData( ) {

  // Fill a new event containing all the fired pixels of each sensors acquired,
  // return "true" if the event is OK, "false" otherwise
  //
  // To build the event, frame(s) is(are) read, the number depending on the trigger configuration.
  //
  // JB, 2009/08/18
  // Modified: JB, 2009/09/08, return empty event if overflow
  // Modified: JB, 2009/09/09, change the way frames are read twice when necessary
  // Modified: JB, 2010/06/16, manage several triggers

  // -+-+- Initialization

  if(DebugLevel>1) cout << "  PXIBoardReader " << BoardNumber << "::HasData - readingEvent " << ReadingEvent << " event readiness " << EventReady << " currentEvent " << CurrentEventNumber << endl;

  bool readingOK = true; // flag to test the reading
  Overflow       = false; // init value, JB 2009/09/08
  CurrentEvent   = 0;    // Allow to know wether data are correct, JB 2009/05/26
  ListOfPixels.clear();
	ListOfTriggerPos.clear(); //JB 2010/06/16
	ListOfFrames.clear(); //JB 2010/06/16
  
  // -+-+- Check files have been associated

  if( NumberOfFiles==0 ) {
    cout << "ERROR: PXIBoardReader NO RAW DATA FILE WAS ASSOCIATED WITH BOARD " << BoardNumber << ", STOPPING!" << endl << endl;
    return false;
  }

  // -+-+- Loop over Daq Events (or frames for all sensors)

  while( !EventReady && readingOK ) { // while some event has to be read & readout OK

    Bool_t goDecode = true;
    if( !ReadTwice ) { // If not second reading of same frame, JB 2009/09/09
      goDecode = GetNextDaqEvent(); // Get the next Daq Event
    }

    if( goDecode ) { // If frame is OK
	for( int iSensor=0; iSensor< NSensors; iSensor++) { // loop on sensors
	  readingOK &= DecodeFrame( iSensor); // check decoding OK or no overflow, JB 2009/09/07
	} // end loop on sensors
    } // end if frame is OK

    // wrong Daq frame
    else {
      readingOK = false;
      cout << "WARNING: PXIBoardReader board " << BoardNumber << ", Daq Event unreadable!" << endl;  
    }
    
  } // while some events have to be read


  // -+-+- Create the event if event is complete and readout is OK

  if( EventReady && readingOK ) {

    if( Overflow ) {  // return no pixels if overflow, JB 2009/09/09
      ListOfPixels.clear(); 
      EventsOverflow++;
    if(DebugLevel) printf("    event listed as overflow, list of pixels cleared (%d events with overflow so far).\n", EventsOverflow);
    }

    EventsCount++;
    CurrentEvent = new PXIEvent( CurrentEventNumber, BoardNumber, RunNumber, &ListOfPixels, &ListOfTriggerPos, &ListOfFrames);
    if(DebugLevel>1) printf("  New event %d from board %d with %ld pixels %d frames and %ld triggers created (%d events so far)\n", CurrentEventNumber, BoardNumber, ListOfPixels.size(), (int)ListOfFrames.size(), ListOfTriggerPos.size(), EventsCount); // removing warning with specifying type for format and ordering frames and triggers BH 2013/08/20

    // re-init
    CurrentEventNumber = -1;
    EventReady         = false;

  }


  // -+-+- End
  return readingOK;

}

// --------------------------------------------------------------------------------------

void PXIBoardReader::AddPixel( int input, int value, int aLine, int aColumn) {

  // Add a pixel to the vector of pixels
  // require the following info
  // - input = number of the sensors
  // - value = analog value of this pixel
  // - line & column = position of the pixel in the matrix
  // JB, 2009/08/14

  ListOfPixels.push_back( PXIPixel( input, value, aLine, aColumn));

}

// --------------------------------------------------------------------------------------

void PXIBoardReader::PrintStatistics(ostream &stream) {

  // Print statistics on the events read by this board
  //
  // JB, 2009/09/09
  // Modified JB 2009/10/14 to add count of events read twice
  // Modified JB 2009/10/16 to add count of events aborted
  // Modified JB 2011/10/30 triggermode printout added
  // Modified SS 2011/12/14 output stream can be set from outside	

  stream << "***********************************************" << endl;
  stream << " Board PXI " << BoardNumber << " with trigger mode " << TriggerMode << " found:" << endl;
  stream << EventsCount << " events in total," << endl;
  stream << EventsOverflow << " events with an overflow." << endl;
  stream << FramesReadTwice << " frames read twice." << endl;
  stream << EventsAborted << " events aborted." << endl;
  stream << " average #states in line: " << h1LineOccupancy->GetMean() << " with RM: " << h1LineOccupancy->GetRMS() << endl;
  stream << " average #states in block: " << h1BlockOccupancy->GetMean() << " with RM: " << h1BlockOccupancy->GetRMS() << endl;
  stream << "***********************************************" << endl;

  char name[20], title[50];
  sprintf( name, "cPXI%d", BoardNumber);
  sprintf( title, "Statistics for board %d", BoardNumber);
  TCanvas c( name, title, 300, 600);
  c.Divide(1,2);
  c.cd(1);
  h1LineOccupancy->Draw();
  c.cd(2);
  h1BlockOccupancy->Draw();
  c.Update();
  sprintf( title, "Results/%d/PxiStats.root", RunNumber);
  sprintf(title,"%s", fTool.LocalizeDirName( title)); // JB 2011/07/07
  c.Print( title);

}

// --------------------------------------------------------------------------------------

PXIEvent::PXIEvent( int eventNumber, int boardNumber, int runNumber, vector<PXIPixel> *listOfPixels, vector<int> *listOfTriggerPos, vector<int> *listOfFrames) {

  // JB, 2009/08/14
	// Modified JB 2010/06/16
	
  EventNumber    = eventNumber;
  BoardNumber    = boardNumber;
  RunNumber      = runNumber;
  ListOfPixels   = listOfPixels;
  ListOfTriggerPos = listOfTriggerPos; //JB 2010/06/16
  ListOfFrames   = listOfFrames; //JB 2010/06/16

}

// --------------------------------------------------------------------------------------

PXIEvent::~PXIEvent() {

  // crashing for the time bein...?
  // JB, 2009/09/14

//   delete ListOfPixels;

}


