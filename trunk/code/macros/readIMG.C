Bool_t readIMG( Char_t *fileName, Int_t nEvents=0, Char_t *extension=".rz", Int_t evts2skip=0 ) {
  
  // This macro reads a file through the IMGBoardReader class
  //
  //  fileName should not include the extension, which is specified later
  //  if nEvents = 0, reads all the events in the file
  //  evts2skip is what it says, these events will not be printed
  
  IMGBoardReader *reader;
  Int_t inputs, channels, eventBufferSize, eventsInFile, fileHeaderLine;
  Int_t bits, sigBits, binaryCoding, triggerMode, eventBuildingBoardMode;
  Int_t nColumns, nMultiFrames;
  
  Int_t debugLevel = 0;
  
  // Setup for MIMOSA-22-SX
  inputs = 1; // only one board
  channels = 32768; // = 128*256
  eventBufferSize = 4096; // = 128*256 pixels over 8 bits
  eventsInFile = 100000; // not really used for M-22-SX
  fileHeaderLine = 0; // no header
  bits = 16; // words are 16 bits long
  sigBits = 1; // pixel values are coded over 1 bit
  binaryCoding = 1; // 0= littleEndian, 1= BigEndian
  triggerMode = 0; // 0= no trigger info
  eventBuildingBoardMode = 4; // 4= PXI for MIMOSA-22-SX (16 parallel outputs)
  nColumns = 128;
  nMultiFrames = 0;
  
  // ===================
  // Initialisation

  Bool_t rc = true;
  
  reader = new IMGBoardReader( 0, inputs, &channels, eventBufferSize, eventsInFile, fileHeaderLine,
                              &bits, &sigBits, binaryCoding, triggerMode, eventBuildingBoardMode,
                              nColumns, nMultiFrames);
  reader->SetDebugLevel( debugLevel);
  rc = reader->AddFileList( fileName, 0, 0, extension );

  if( !rc ) return rc; // leave if something is wrong

  
  
  // ===================
  // Loop on events
  
  IMGEvent *event;
  IMGPixel *pixel;
  Int_t ievt = 0;
  
  for( int i=0;i<evts2skip;i++) reader->HasData();
  
  while ( rc && (nEvents==0 || ievt<nEvents) ) { // start loop on events
  
    rc = reader->HasData(); // ask for an event
    if ( rc ) { // if event read properly
      imgEvent = reader->GetEvent();
      if( imgEvent ) { // if event decoded properly
  
        cout << "In event " << ievt << " found " << imgEvent->GetNumberOfPixels() << " hit pixels with " << imgEvent->GetNumberOfTriggers() << " triggers and " << imgEvent->GetNumberOfFrames() << " frames and " << imgEvent->GetNumberOfTimestamps() << " timestamps from daq event " << imgEvent->GetEventNumber() << endl;

        for( Int_t iPix=0; iPix<imgEvent->GetNumberOfPixels(); iPix++) { // loop on Pixels
          imgPixel = imgEvent->GetPixelAt( iPix);
          cout << "  pixel " << iPix << " index " << imgPixel->GetIndex() << " with value " << imgPixel->GetValue() << endl;
        } // end loop on Pixels
        
      } // if event decoded properly
      ievt++;
    } // end if event read properly

  } // end loop on events
  
  
  return rc;
  
}
