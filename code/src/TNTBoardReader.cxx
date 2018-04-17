/////////////////////////////////////////////////////////////
// Class Description of TNTBoardReader                     //
//                                                         //
// Read TNT raw data file                                  //
// and produce an event for getting the results            //
// This version compatible with May 2009 data format       //
//                                                         //
/////////////////////////////////////////////////////////////
//
// JB, 2008/10/14
// Last Modified: JB, 2009/05/08
// Last Modified: RDM, 2009/05/20
// Last Modified: JB, 2009/05/21
// Last Modified: JB, 2009/09/28 triggerMode, and Number Of Bits per Value
// Last Modified: NCS 2009/10/22 reading M20, new TNT format triggered pix
// Last Modified: NCS 2009/11/14 reading M20, TNT checks and upgrade for PLUME Beam Test analysis
// Last Modified: NCS 191109 in HasData()
// Last Modified: JB, 2010/08/26 new bit encoding and statistics
// Last Modified: JB, 2010/10/05 correction of new bit encoding
// Last Modified: JB, 2010/10/06 detect overflow
// Last modified JB, 2011/07/07 to localize path names

#include "TNTBoardReader.h"

ClassImp(TNTBoardReader)
ClassImp(TNTEvent)

// --------------------------------------------------------------------------------------

  TNTBoardReader::TNTBoardReader( int boardNumber, int sizeOfWord, int endian, int timelimit, int triggermode, int numberOfBits, int newformat) {

  // JB, 2008/10/14
  // Modified JB 2009/09/28, to add trigger mode and number of bits for pixel value
  // Modified JB 2010/10/26, to add new bit encoding possibilities and statistics
  // Modified JB 2010/10/05, corrected new bit encoding
  // Modified JB 2010/10/13, no more file variable

  NumberOfFiles      = 0;
  CurrentFileNumber  = 0;
  NoMoreFile         = false; // JB 2010/10/13
  BoardNumber        = boardNumber;  
  SizeOfWord         = sizeOfWord;
  CurrentWordIndex   = 0;
  SizeOfDataBuffer   = 0;
  Data               = new unsigned int[SizeOfWord];
  Endianness         = endian;
  NumberOfBitsValue  = (Int_t)TMath::Abs(numberOfBits); // # bits used to code the pixel value, JB 2009/09/28
  SignedValues       = (numberOfBits/(Int_t)TMath::Abs(numberOfBits)<0); // if negative, we have signed values
  if(NumberOfBitsValue>14)
  { 
      Error("TNTBoardReader constructor","NumberBitsValue = %d is outside of known range!", NumberOfBitsValue);
  }
  ReadingEvent       = false; 
  CurrentEvent       = 0; // Allow to know wether data are correct, JB 2009/05/26
  CurrentEventNumber = 0;
  CurrentTriggerNumber = 0; // 2009/09/28
  CurrentFrameNumber = 0;
  CurrentTimestamp = 0;
  TimeLimitBetweenTwoTriggers = timelimit; // default is 0, unit of 10 ns, JB, 2009/05/21 => file config, Parameters of the Tracker TimeLimit=4000000
  TriggerMode        = triggermode; // define limits of events: 0=with trigger, 1=with frames
  NewFormat=newformat; // use SignificantBitParameter (unused) to change TNT format version (old and new=triggeredpix)
  eventnumber=0;

  // Define bit masks to decode words
  Int_t mult = 0;
  for( Short_t ibit=0; ibit<13-NumberOfBitsValue; ibit++) {
    mult += 0x1 << ibit;
  } 
  MaskForValue   = 0x1FFF0000 - mult*0x00010000;
  ShiftForValue  = 16 + (13-NumberOfBitsValue);
  MaskForIndex   = 0x0000FFFF + mult*0x00010000;
  MaxSignedValue = (Int_t)pow(2.,NumberOfBitsValue-1);

  // Initialise statistics
  // JB 2010/08/26
  TotalNEvents = 0;
  TotalNTriggers = 0;
  TotalNFaultyTriggers = 0;
  TotalNFrames = 0;
  TotalNPixels = 0;
  TotalNErrors = 0;
  TotalOverflows = 0; // JB 2010/10/06
}

// --------------------------------------------------------------------------------------

TNTBoardReader::~TNTBoardReader() {

  // JB, 2008/09/27
  delete CurrentEvent;
  delete InputFileName;
  delete Data;
  ListOfTriggers.clear();
  ListOfTimestamps.clear(); //RDM150509
  ListOfFrames.clear();
  ListOfPixels.clear();

}

// --------------------------------------------------------------------------------------

bool TNTBoardReader::AddFile(char *fileName) {

  // JB, 2008/09/27

  NumberOfFiles = 1;
  CurrentFileNumber = 1;
  InputFileName = fileName;
  sprintf(InputFileName,"%s", fTool.LocalizeDirName( InputFileName)); // JB 2011/07/07
  RawFileStream.open( InputFileName);
  if( RawFileStream.fail() ) {
    cout << endl << "ERROR TNTBoardReader " << BoardNumber << " file " << InputFileName << " does not exist!" << endl;
    return false;
  }
  cout << endl << "  --> TNTBoardReader " << BoardNumber << " New file " << InputFileName << endl;  

  return true;
}

// --------------------------------------------------------------------------------------
bool TNTBoardReader::AddFileList(const char *prefixFileName, int startIndex, int endIndex, const char *suffixFileName) {

  // Store the inputs to be able to read several files one after the other
  // JB, 2008/10/8
  // Modified: JB 2010/10/05 to allow a first index file not 1 and read file#>10

  NumberOfFiles = endIndex;
  CurrentFileNumber = startIndex; // JB 2010/10/05
  SuffixFileName = new char[10];//RDM310509
  PrefixFileName = new char[300];//RDM310509
  //  PrefixFileName = prefixFileName; //RDM310509
  //SuffixFileName = suffixFileName;//RDM310509
  sprintf( PrefixFileName, "%s", prefixFileName); //RDM310509
  sprintf( SuffixFileName, "%s", suffixFileName);//RDM310509
  if(DebugLevel>0) cout << "TNTBoardReader " << BoardNumber << " adding " << NumberOfFiles << " files like " << prefixFileName << "*" << SuffixFileName << endl;
  InputFileName = new char[300];
  if( CurrentFileNumber<10 ) {
    sprintf( InputFileName, "%s000%d%s", PrefixFileName, CurrentFileNumber, SuffixFileName);
  }
  else if( CurrentFileNumber<100 ) {
    sprintf( InputFileName, "%s00%d%s", PrefixFileName, CurrentFileNumber, SuffixFileName);
  }
  else if( CurrentFileNumber<1000 ) {
    sprintf( InputFileName, "%s0%d%s", PrefixFileName, CurrentFileNumber, SuffixFileName);
  }
  else {
    sprintf( InputFileName, "%s%d%s", PrefixFileName, CurrentFileNumber, SuffixFileName);
  } 
  sprintf(InputFileName,"%s", fTool.LocalizeDirName( InputFileName)); // JB 2011/07/07
  if(DebugLevel>0) cout << "  trying file " << InputFileName << endl;
  RawFileStream.open( InputFileName);
  if( RawFileStream.fail() ) {
    cout << endl << "ERROR TNTBoardReader " << BoardNumber << " file " << InputFileName << " does not exist!!" << endl;
    return false;
  }
  cout << endl << "  --> TNTBoardReader " << BoardNumber << " New file " << InputFileName << endl;  

  return true;
}

// --------------------------------------------------------------------------------------

void TNTBoardReader::Close() {

  // Closes everything needed to be
  // Close()2008/09/27
  cout << "eventnumber=" << eventnumber << endl ;
  RawFileStream.close();
}

// --------------------------------------------------------------------------------------

unsigned int TNTBoardReader::SwapEndian( unsigned int data) {

  // Swap the 4 bytes of a 32 bits word
  // A word like 0x12345678 will become 0x78563412
  // A.Bulgheroni, 2008/09/26

  return ((data & 0xFF000000)>>24) + ((data & 0x00FF0000)>>8) + ((data & 0x0000FF00)<<8) + ((data & 0x000000FF)<<24);
}

// --------------------------------------------------------------------------------------

bool TNTBoardReader::GetNextWord( ) {

  // Try to get the next word index from the Data buffer
  // either by reading in the current Data buffer
  // either by getting a new Data buffer from file and reading first word
  //
  // Change the bit order according to the endianess
  //
  // return "false" if nothing to read, "true" otherwise
  //
  // JB, 2008/10/10
  // Last Modified, JB 2009/05/21
  // Last Modified, JB 2010/10/13 set no more file flag

  // If we can still read a word from this Data buffer, do it
  if( SizeOfDataBuffer && (size_t)CurrentWordIndex < SizeOfDataBuffer-1 ) 
  {
    CurrentWordIndex++;
  }

  // If we have read the full current Data buffer alreay,
  // try to get a new Data buffer
  else 
  {    	// If end of file but still a file to read, open it
    	if( RawFileStream.eof() ) 
    	{

      		if( CurrentFileNumber < NumberOfFiles) 
		{
		  if(DebugLevel>1) cout << "  --> TNTBoardReader " << BoardNumber << " New file to read " << CurrentFileNumber << " <= " << NumberOfFiles << " closing and opening." << endl;
		  RawFileStream.close();
                  CurrentFileNumber++;
		  if( CurrentFileNumber<10 ) {
		    sprintf( InputFileName, "%s000%d%s", PrefixFileName, CurrentFileNumber, SuffixFileName);
		  }
		  else if( CurrentFileNumber<100 ) {
		    sprintf( InputFileName, "%s00%d%s", PrefixFileName, CurrentFileNumber, SuffixFileName);
		  }
		  else if( CurrentFileNumber<1000 ) {
		    sprintf( InputFileName, "%s0%d%s", PrefixFileName, CurrentFileNumber, SuffixFileName);
		  }
		  else {
		    sprintf( InputFileName, "%s%d%s", PrefixFileName, CurrentFileNumber, SuffixFileName);
		  }
		  sprintf(InputFileName,"%s", fTool.LocalizeDirName( InputFileName)); // JB 2011/07/07
		  RawFileStream.open( InputFileName);
		  if( RawFileStream.fail() ) 
		  { // end the reading if file opening failed
		    cout << endl << "ERROR TNTBoardReader " << BoardNumber << " file " << InputFileName << " does not exist!!!" << endl;
		    return false;
		  }
		  else {
		    cout << endl << "  --> TNTBoardReader " << BoardNumber << " New file " << InputFileName << endl;  
		  }
      		} // end if( CurrentFileNumber < NumberOfFiles)
      		else // Otherwise no more file, end the reading
		{
		  cout << "  --> TNTBoardReader " << BoardNumber << ": No more files to read " << CurrentFileNumber << " > " << NumberOfFiles << " closing!" << endl;   //YV 23/06/09 to speed up DSF production

			RawFileStream.close();
			NoMoreFile = true; // JB 2010/10/13
			return false;
      		}
    	}//end if eof

    // Now we can get the next data buffer
    RawFileStream.read(reinterpret_cast<char *> ( &Data[0] ), sizeof(int) * SizeOfWord);
    SizeOfDataBuffer = RawFileStream.gcount() / sizeof ( size_t ); // number of available words for this buffer
    CurrentWordIndex = 0; //re-init the reading position in Data    
    if(DebugLevel>7) cout << "  TNTBoardReader " << BoardNumber << ": Got new data buffer with " << SizeOfDataBuffer << " (1st word at " << CurrentWordIndex << ")." << " gcount=" << RawFileStream.gcount() << " sizeof=" << sizeof ( size_t ) << " SizeOfWord=" << SizeOfWord << endl;
    
  }// fin else du if ( SizeOfDataBuffer && (size_t)CurrentWordIndex < SizeOfDataBuffer-1 ) {CurrentWordIndex++;}


  //if(DebugLevel>5) 
  //{
  //   cout << "TNTBoardReader (Before SwapEndian)" << BoardNumber << ": Getting new word at CurrentWordIndex=" <<CurrentWordIndex << endl ;
  //   cout << "Data["<< CurrentWordIndex<<"]= "<< Data[ CurrentWordIndex] << " and = " << hex << Data[ CurrentWordIndex]<< dec << endl;
  //}
  // change the endian of the 32 bts binary word
  // moved from HasData, JB 2009/05/21
  if(Endianness==1) Data[CurrentWordIndex] = SwapEndian(Data[CurrentWordIndex]); // we should check wether or not to do it (how ?)
  //if(DebugLevel>5) 
  //{ 
  //  cout << "TNTBoardReader (After SwapEndian)" << BoardNumber << ": CurrentWordIndex= " << CurrentWordIndex <<  endl;
  //  cout << "Data["<< CurrentWordIndex<<"]= "<< Data[ CurrentWordIndex] << " and = " << hex << Data[ CurrentWordIndex]<< dec << endl;
  //}
  return true;

}

// --------------------------------------------------------------------------------------

void TNTBoardReader::SkipNextEvent() {

  // This method is used to ignore the next event
  //
  // JB, 2009/05/26

  if(DebugLevel) printf("  TNTBoardReader: %d Resetting readout because of bad event\n", BoardNumber);
  ReadingEvent       = false; 
  //CurrentEventNumber = 0;
  //CurrentFrameNumber = 0;
  //CurrentTimestamp   = 0;
  CurrentEvent       = 0; // Allow to know wether data are correct, JB 2009/05/26

}

// --------------------------------------------------------------------------------------

bool TNTBoardReader::HasData( ) {

  // Try to find the data for the next event in the file
  // loop over the raw data file and decode each word in it
  //
  // New events are decided when a new trigger is read 
  //  with a timestamp later than Timelimit wrt to the current timestamp
  //
  // If something goes wrong with an unexpected word, 
  //  a flag (eventOK) is used to return "false"
  // Otherwise returns "true"
  //
  // JB, AB, 2008/10/14
  // Last modified RDM 2009/05/20, decode new TimeStamp word
  // Last modified JB 2009/05/21, decide new event from timestamp
  // Last Modified JB 2009/05/26, to cope with case without proper timestamp
  // Last Modified JB 2009/05/28 to deal with trigger modes
  // Last Modified NCS 2009/11/14 reading M20 new TNT format (triggered pixel) Beam Test PLUME nov09
  // Last Modified NCS 191109 to check the difference between TS by taking into account the Timstamp 2^30 cycle
  // Last Modified JB 2009/10/26 to add statistics
  // Last Modified JB 2010/10/06 detect overflow word
  // Last Modified JB 2010/10/13 detect no more file flag
  
  // -+-+- Initialization

  bool eventDone = false;   // we start with event incomplete
  bool eventOK   = true;    // we start with a good event by default, JB 2009/05/26
  CurrentEvent   = 0;       // Allow to know wether data are correct, JB 2009/05/26
  ListOfTriggers.clear(); // and clear the vectors
  ListOfTimestamps.clear(); // RDM 150509
  ListOfFrames.clear();
  ListOfPixels.clear();
  Int_t frameNpreviousMsg = 0; // JB 2010/10/06 to limit overflow message / frame

  if(DebugLevel>0) 
  {
  	cout << endl ;
	cout << "  BoardNumber " << BoardNumber << " TNTBoardReader::HasData() - readingEvent " << ReadingEvent << " currentEvent " << CurrentEventNumber << " currentTrigger " << CurrentTriggerNumber << " currentFrame " <<	CurrentFrameNumber << endl;
  }


  if( ReadingEvent ) 
  {    // we have already detected the new trigger from a previous reading
    ListOfTriggers.push_back( CurrentTriggerNumber ); // add the first event trigger to the list
    ListOfTimestamps.push_back( CurrentTimestamp ); // add the first event trigger to the list RDM 150509
    ListOfFrames.push_back( CurrentFrameNumber ); // add the first event frame to the list
  }


  // -+-+- Check files have been associated

  if( NumberOfFiles==0 ) 
  {
    cout << "ERROR: TNTBoardReader NO RAW DATA FILE WAS ASSOCIATED WITH BOARD " << BoardNumber << ", STOPPING!" << endl << endl;
    return false;
  }


  // -+-+- Loop over the Data buffer until an event has been fully read
  // (start='new trigger'  and end='new frame' found)
  // or till the end of the file

  bool isNewTrigger=false;
  bool isNewFrame=false;
  bool isDummy=false;
  bool isPixel=false;
  bool isOverflow=false;
  int  input=0, value=0, index=0, trigger=0, frame=0, timestamp=0; //RDM 130509 added timestamp
  int triggeredpix=0; // NCS 22/10/09 new TNT format
  int timestampOK=0;
  int triggeredpixOK=0;  
  int TSdiff=0;
  
  while( !eventDone && GetNextWord() ) 
  { // next word

    if(DebugLevel>4) 
    {
      cout << endl ;
      cout << "*BoardNumber " << BoardNumber << endl ;
      cout << "  CurrentEventNumber=" << CurrentEventNumber << endl ;
      cout << "  ReadingEvent=" << ReadingEvent <<endl ;
      cout << "  TNTBoardReader while (!eventDone && GetNextWord()) : Data[" << CurrentWordIndex << "]= "  << Data[ CurrentWordIndex] << " or " << hex << Data[CurrentWordIndex] << dec << endl;
    }

    // ===========================
    // decode the word
    // This format is described in Cayetano Santos Document from 2009/05/12
    isPixel      = ((Data[CurrentWordIndex] & 0x80000000) >>31)==1; // NCS 08/10/2009
    isNewFrame   = ((Data[CurrentWordIndex] & 0xFF000000) >>24)==0x7F;
    isOverflow   =   Data[CurrentWordIndex] == 0x50000000;
    isDummy      = ((Data[CurrentWordIndex] & 0xFF000000) >>24)==0x40;
    input        = (Data[CurrentWordIndex] & 0x60000000) >>29; //channel of the DAQ board
    value        = (Data[CurrentWordIndex] & MaskForValue) >>ShiftForValue; //raw value of the pixel
    index        = (Data[CurrentWordIndex] & MaskForIndex);      //pixel index
    trigger      = (Data[CurrentWordIndex] & 0x3FFFFFFF); //trigger number
    //timestamp    = (Data[CurrentWordIndex] & 0x3FFFFFFF); //timestamp RDM 130509
    frame        = (Data[CurrentWordIndex] & 0x00FFFFFF);      //frame number

    if(DebugLevel>4)
    {
	cout << "    isPixel=" << isPixel << endl ;
	cout << "    isNewFrame=" << isNewFrame << endl ;
	cout << "    isDummy =" << isDummy << endl;
	cout << "    input= (Data[CurrentWordIndex] & 0x60000000) >>29 =" << input << endl ;
      	cout << "    value= (Data[CurrentWordIndex] & " << hex << MaskForValue << dec << ") >>" << ShiftForValue << " =" << value << endl;
      	cout << "    index= (Data[CurrentWordIndex] & " << hex << MaskForIndex << dec << ") ="<< index << endl;
	cout << "    trigger   = (Data[CurrentWordIndex] & 0x3FFFFFFF)=" << trigger << endl;
    	cout << "    frame     = (Data[CurrentWordIndex] & 0x00FFFFFF)=" << frame << endl;
    }
    
    // To decide wether we have a real trigger word or a lonely timestamp word
    // If the word decoded starts with 2 bits '00' => it's a new trigger 
    // 1) word not 0x00000000 or 0x00000000 but this is the first trigger (CurrentTriggerNumber<1) NCS 2009/11/25
    // 2) four highest bits = 0x00 = 0000
    // 3) the trigger number is within 200000 units of the previous trigger
    //  this last condition allows to avoid timestamps 
    //    which increase at least by 10 us = 1000 units of 10 ns
    // 4) there is a timestamp word just after
    // 5) in the new format, there is a word with the index of the pixel read when trigger arrived
    // JB 2009/05/26
    
    //*****************************   NEW TRIGGER  *********************************
    if(    (Data[CurrentWordIndex]!=0 || (Data[CurrentWordIndex]==0 && CurrentTriggerNumber<1))
	   && Data[CurrentWordIndex] < 0x40000000
	   && trigger-CurrentEventNumber < 200000 //RDM300509 increased to 20000 for sparsified data
      ) 
    {    
      //******************************************************************************
      if(DebugLevel>5) 
      {
        cout << "  Potential trigger : trigger - CurrentTriggerNumber must be < 200000" << trigger << " - " << CurrentTriggerNumber << " = " << trigger-CurrentTriggerNumber << endl; 
      }

      // ---- Decoding the new TimeStamp word just after the trigger, RDM 130509 begin ---- //
      GetNextWord();
      if ((Data[CurrentWordIndex] < 0x40000000) && Data[CurrentWordIndex]!=0) 
      {
      	timestamp = (Data[CurrentWordIndex] & 0x3FFFFFFF);
	timestampOK=1;
      }
      else  
      {
	TotalNFaultyTriggers++; // JB 2010/09/26
	timestamp = -1; // JB 2009/05/26
	timestampOK=0;
	printf("  Trigger = %d -- TNTBoardReader %d ERROR:: trigger found, but not timestamp : Data[%5d]= %8x ! \n", trigger, BoardNumber, CurrentWordIndex,Data[CurrentWordIndex]);
      }
      if(DebugLevel>5) {
	cout << "    Getting the next word to decode the time stamp : Data[" << CurrentWordIndex << "]= "  << Data[ CurrentWordIndex] << " or " << hex << Data[CurrentWordIndex] << dec << endl;
	cout << "     timestamp = (Data["<<CurrentWordIndex<<"] & 0x3FFFFFFF)=" << timestamp << endl;
	cout << "     timestampOK = " << timestampOK << endl;
      }
      // ---- END Decoding the new TimeStamp word just after the trigger, RDM 130509 begin ---- //
      
      // ---- Decoding the triggered pixel address word just after the timestamp, NCS 22/10/09 ----//     
      if(NewFormat==1 && timestampOK==1)
      {   
         GetNextWord();
         if (Data[CurrentWordIndex] < 0x40000000) // A pixel# can be zero
         {
	   triggeredpix = (Data[CurrentWordIndex] & MaskForIndex); 
	   triggeredpixOK=1;
         }
         else  
	 {
	   TotalNFaultyTriggers++; // JB 2010/09/26
	   triggeredpix = -1; // JB 2009/05/26
	   triggeredpixOK=0;
	   printf("  Trigger = %d -- TNTBoardReader %d ERROR:: trigger & timestamp found, but not triggeredpix: Data[%5d]= %8x!!!!! \n", trigger, BoardNumber, CurrentWordIndex, Data[CurrentWordIndex]);
         }
	 if(DebugLevel>5) {
	   cout << "    Getting the next word to decode the triggered pixel address : Data[" << CurrentWordIndex << "]= "  << Data[ CurrentWordIndex] << " or " << hex << Data[CurrentWordIndex] << dec << endl;
	   cout << "     triggeredpix = (Data["<<CurrentWordIndex<<"] & " << hex << MaskForIndex << dec << ")=" << triggeredpix << endl;
	   cout << "     triggeredpixOK = " << timestampOK << endl;
	 }
      }
      else if(NewFormat==0) // for old format, set flag to true
      {
	triggeredpixOK=1;
      }
      // ---- END Decoding the triggered pixel address word just after the timestamp, NCS 22/10/09 ---- //

      // ---- CHECK if the potential trigger is a real one or not
      if( timestampOK && triggeredpixOK )
      {
	isNewTrigger = true;
	// compute the distance since last timestamp
	int diff = timestamp-CurrentTimestamp;
	if(diff<0) // TS has restarted from scratch 2^30=1073741824 NCS 191109
	  { TSdiff=1073741824-CurrentTimestamp + timestamp ;}
	else { TSdiff=diff; }
      }
      else 
      {
	eventOK = false;
	isNewTrigger = false;
      }

    } // end if new trigger
    
    else 
    {
      isNewTrigger = false; 
    }    
    if(DebugLevel>4) {
      cout << "    isNewTrigger =" << isNewTrigger << endl;
      cout << "    timestamp = (Data[CurrentWordIndex] & 0x3FFFFFFF)=" << timestamp << endl;
      cout << "    TSdiff / (currentTS " << CurrentTimestamp << ") =" << TSdiff << " <?> limit=" << TimeLimitBetweenTwoTriggers << endl;
      cout << "    indexTrigPixel= (Data[CurrentWordIndex] & " << hex << MaskForIndex << dec << ") ="<< triggeredpix << endl;
    }
    
    //*****************************   Wrong Reading   *******************************    
    if ((isPixel + isNewTrigger + isNewFrame) >1) // isDummy removed from test, JB 2010/08/26
    {
      TotalNErrors++; // JB 2010/09/26
      //******************************************************************************    
      printf(" TNTBoardReader %d  ERRORRRRRRRRRRROAR word %x in unknown case\n", BoardNumber, Data[CurrentWordIndex]);
    }

    
    //*****************************   NEW EVENT ?  *******************************    
    // Test if we should end/start an old/new event
    //
    // Trigger mode 0:
    // A new event is created for each new frame
    //
    // Trigger mode 1:
    // A new event is created if the new timestamp arrive after timeLimit
    // wrt the current time stamp
    //
    // JB 2009/09/28, NCS 2010/
    //******************************************************************************    
    //cout << "just decoded timestamp=" << timestamp << " CurrentTimestamp=" << CurrentTimestamp << " diff=" << TSdiff << endl ;
      
    if ( eventOK && (
       (isNewTrigger && TriggerMode==1 && TSdiff >= TimeLimitBetweenTwoTriggers) // mode 0
       ||
       (isNewFrame && TriggerMode==0) // mode 1
       ) ) { // if new event

      eventnumber ++;
      TotalNEvents++; // JB 2010/08/26
	
      if( ReadingEvent ) // if an event was being read
      {
	CurrentEvent = new TNTEvent( CurrentEventNumber, BoardNumber, &ListOfTriggers, &ListOfTimestamps, &ListOfFrames, &ListOfPixels);//RDM150509
	eventDone = true; // event complete, will make the reading loop stop!	
	if(DebugLevel>1) {
	  printf("  TNTBoardReader: New event %d stopped and stored from board %d with %d pixels, reading resumes\n", CurrentEventNumber, BoardNumber, (int)ListOfPixels.size());
	  if (DebugLevel>4) 
          {
	    cout << "  ---------------------------------------"<< endl ;
	    cout << "    EVENT (in ReadingEvent) BoardNumber=" << BoardNumber << endl ;
	    cout << "    CurrentEventNumber = "<< CurrentEventNumber << " CurrentTriggerNumber = "<< CurrentEventNumber << " trigger   = " << trigger << endl ;
	    cout << "    timestamp= " << timestamp << " CurrentTimestamp=" << CurrentTimestamp  << " TSdiff=" << TSdiff<< ">limit=" << TimeLimitBetweenTwoTriggers<<endl ;
	    cout << "    frame    = " << frame << " CurrentFrameNumber= " << CurrentFrameNumber << endl ; 
	    cout << "  ---------------------------------------"<< endl ;
	  }		
	}	  
  
	CurrentTriggerNumber++; // NCS 20/11/09
	  
      } // end if an event was being read

      // otherwise start a new event
      else 
      {
	  ReadingEvent = true; // once set to true, never updated to false unless SkipNextEvent has been called, JB 2009/05/26
	  ListOfTriggers.push_back( trigger );
	  ListOfTimestamps.push_back( timestamp ); //RDM150509
	  ListOfFrames.push_back( frame );
      } // end start a new event

      // in any case store the trigger and frame number for further reading
      CurrentEventNumber = CurrentTriggerNumber ; //trigger; NCS 19/11/09
      CurrentFrameNumber = frame;
      CurrentTimestamp = timestamp; // JB, 2009/05/21
	
      if (DebugLevel>4)
      {
	cout << "      New CurrentEventNumber = "<< CurrentEventNumber << endl;
	cout << "      New CurrentTriggerNumber = "<< CurrentTriggerNumber << endl;
	cout << "      trigger   = " << trigger << endl ;
	cout << "      New CurrentTimestamp   = " << CurrentTimestamp  <<endl ;
	cout << "      New CurrentFrameNumber = " << CurrentFrameNumber << endl ; 
      }	
	
    } // end if new event


    //*****************************   IsPixel & ReadingEvent  ***********************
    //else if( isPixel && ReadingEvent ) // NCS ici j'ai rajoutÈ un else 210110
	if( isPixel ) // && ReadingEvent )
    {
      TotalNPixels++; // JB 2010/09/26
      //******************************************************************************    
      // If pixel and in an event, store    

      // ---- For signed values, when SignedValues==true
      // Case for laboratory tests to study noise ---- // NCS 29/09/09
      // complement the current value
      if( SignedValues) {
	value -= MaxSignedValue;
      }
      
      // ---- To remove CS false marker ---- // NCS 30/10/09 
      // specific for MIMOSA 20
      if( NumberOfBitsValue==12 && (index==0 || index==105600) ) {
	value=0 ; 
      }
      
      if(DebugLevel>4) 
      {
      	printf("   *IN isPixel and ReadingEvent %d, input %d, value %d, index %d ", CurrentEventNumber, input, value, index);
      }
      
      // ---- AddPixel Method ---- //
      // to associate a pixel and its value with an input 
      //printf("event %d, pixel %d, channel %d, value %d \n",CurrentEventNumber,index,input,value);
      if( value<10000 && value>-10000 )AddPixel( input, value, index); //RDM190509 

    } // end if( isPixel && ReadingEvent ) 


    //*****************************   IsNewTrigger  ********************************
    else if( isNewTrigger)  
    {
      TotalNTriggers++; // JB 2010/09/26
      //******************************************************************************    
      if(DebugLevel>1) 
      {
	printf("+++* IN isNewTrigger, event=%d, trigger=%d, timestamp=%d (time since previous TS %d <?>%d)\n", CurrentEventNumber, trigger, timestamp, TSdiff, TimeLimitBetweenTwoTriggers);
      }
      
      //CurrentTriggerNumber = trigger; // NCS 2009
      CurrentFrameNumber = frame; // in principle, useless
      CurrentTimestamp = timestamp; // Update the reference time, JB 2009/05/21 
	
      if( !eventDone ) 
      { 
	ListOfTriggers.push_back(trigger);
	ListOfTimestamps.push_back(timestamp);//RDM150509
      }

    } // end if isNewTrigger 


    //*****************************   IsNewFrame  ********************************
    // If new frame, just update the current frame
    else if( isNewFrame && ReadingEvent ) 
    {   
      TotalNFrames++; // JB 2010/09/26
      //******************************************************************************
      if(DebugLevel>1) 
      {
	printf("+++* IN isNewFrame & ReadingEvent, event=%d, frame=%d\n", CurrentEventNumber, frame);
      }

      CurrentFrameNumber = frame;

      if( !eventDone ) 
      { 
	ListOfFrames.push_back( frame ); // don't include the frame if the event is done, JB 2009/09/28
      }

    } // end if isNewFrame


    //*****************************  Overflow  ********************************
    else if( isOverflow ) 
    {
      //******************************************************************************    
      // simply count how many times it happens but print Msg only once per frame
      // eventOK = false; // we may consider getting rid of this event...
      TotalOverflows++;
      if( frameNpreviousMsg!=frame ) printf("+++* DATA OVERFLOW detected\n\n");
      frameNpreviousMsg = frame;
    }

    //*****************************   Dummy  ********************************
    else if( isDummy ) 
    {
      //******************************************************************************    
      eventOK = false;
      printf("+++* IN Dummy Word %8x=(isPix %d/ isNewTrig %d/ IsNewFrame %5x) dummy %4d\n\n", Data[CurrentWordIndex], isPixel, isNewTrigger, isNewFrame, frame);
    }

    //*****************************   Unknown case  ********************************
    else 
    {
      //******************************************************************************    
	//eventOK = false; // removed by JB 2010/10/05
      printf("+++* CurrentEventNumber %d BoardNumber %d IN Unknown Word %8x=(isPix %d/ isNewTrig %d/ IsNewFrame %5x) unknown case!\n", CurrentEventNumber,BoardNumber, Data[CurrentWordIndex], isPixel, isNewTrigger, isNewFrame);
    }




  } // end loop on get next word
  


  // End
  // check there are still data to read, JB 2010/10/13
  return eventOK && !NoMoreFile;

}

// --------------------------------------------------------------------------------------

void TNTBoardReader::AddPixel( int input, int value, int index) {

  // Add a pixel to the vector of pixels
  // require the following info
  // - input = input number of the board
  // - value = analog value of this pixel
  // - index = index of tis pixel on this input
  // JB, 2008/09/27

  ListOfPixels.push_back( TNTPixel( input, value, index));

}


// --------------------------------------------------------------------------------------

void TNTBoardReader::PrintStatistics(ostream &stream) {

  // Print statistics on the events read by this board
  //
  // JB, 2010/08/26
  // Modified SS 2011/12/14 output stream can be set from outside

  stream << "***********************************************" << endl;
  stream << " Board TNT " << BoardNumber << " found:" << endl;
  stream << TotalNEvents << " events in total," << endl;
  stream << TotalNTriggers << " correct triggers detected." << endl;
  stream << TotalNFaultyTriggers << " triggers with missing information." << endl;
  stream << TotalNFrames << " frames detected while reading an event." << endl;
  stream << TotalNPixels << " pixels detected while reading an event." << endl;
  stream << TotalNErrors << " unrecognized words." << endl;
  stream << "***********************************************" << endl;

}

// --------------------------------------------------------------------------------------

TNTEvent::TNTEvent( int eventNumber, int boardNumber, vector<int> *listOfTriggers, vector<int> *listOfTimestamps, vector<int> *listOfFrames, vector<TNTPixel> *listOfPixels) {  //RDM150509

  // JB, 2008/09/27

  EventNumber    = eventNumber;
  BoardNumber    = boardNumber;
  ListOfTriggers = listOfTriggers;
  ListOfTimestamps = listOfTimestamps; //RDM150509
  ListOfFrames   = listOfFrames;
  ListOfPixels   = listOfPixels;

}

// --------------------------------------------------------------------------------------

TNTEvent::~TNTEvent() {

  // crashing for the time bein...?
  // JB, 2008/09/27

//   delete ListOfTriggers;
//   delete ListOfTimestamps; //RDM150509
//   delete ListOfPixels;
//   delete ListOfFrames;

}


